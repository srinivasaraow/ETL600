/*-------------------------------------------------------------------------*
 * $Workfile: Interrupt_routines_UC_DSP.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Interrupt routine
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/UC_DSP/ASM/Interrupt_routines_UC_DSP.asm $
 * 
 * 4     11.02.05 14:44 Chbrbus
 * Added DMA buffer boundary check and SIMD mode off during interrupt
 * 
 * 3     10.02.05 16:09 Chbrbus
 * Reduced upconverter TDM bus DMA buffer - changed interrupt routine
 * 
 * 2     8.02.05 14:46 Chbrbus
 * UC DAC interrupt syncronized with SPORT13 interrupt - reduced DAC
 * buffers from 3 to 2 ! Cleaned Interrupt functions and EP1
 * initialisation.
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 1     9.06.04 13:46 Rytz01
 * 
 * 11    4.06.04 11:40 Rytz01
 * SPORT02 counter added
 * 
 * 10    3.06.04 13:32 Maag01
 * 
 * 9     2.06.04 18:58 Maag01
 * 
 * 8     27.05.04 9:29 Rytz01
 * 
 * 7     12.05.04 9:33 Rytz01
 * 
 * 6     11.05.04 14:25 Rytz01
 * 
 * 5     10.05.04 15:50 Rytz01
 * 
 * 4     7.05.04 14:35 Rytz01
 * 
 * 3     5.05.04 7:47 Rytz01
 * "*.end" of function added
 * 
 * 2     29.04.04 15:10 Rytz01
 * 
 * 1     8.04.04 16:17 Rytz01
 * Initial version
 *
 * Initial Version
 *-------------------------------------------------------------------------*/

#include <def21161.h>
#include <asm_sprt.h>

.segment/dm seg_dmda;
.var			_IR_TimerCounter;
.global			_IR_TimerCounter;
.var			_IR_BlockCounter;
.global			_IR_BlockCounter;
.var			_externalTDM_Rx_Ptr;
.global			_externalTDM_Rx_Ptr;
.var			_externalTDM_Tx_Ptr;
.global			_externalTDM_Tx_Ptr;
.var			_internalTDM_Rx_Ptr;
.global			_internalTDM_Rx_Ptr;
.var			_internalTDM_Tx_Ptr;
.global			_internalTDM_Tx_Ptr;
.var			_DAC_Tx_Ptr;
.global			_DAC_Tx_Ptr;
.var			_PreviousDAC_Tx_Ptr;
.global			_PreviousDAC_Tx_Ptr;
.var			_II_SPORT1_ValueActual;
.global			_II_SPORT1_ValueActual;
.var			_SPORT02InterruptCounter;
.global			_SPORT02InterruptCounter;

// save mode1 register
.var			_TempMode1_Timer;
.var			_TempMode1_SPORT02;
.var			_TempMode1_SPORT13;
.var			_TempMode1_DMA11;
.endseg;

.endseg;

.extern			_SPORT02_RxBufferLookupTable;
.extern			_SPORT02_RxBufferSize;

// DMA buffer adress check variables - preventing reading and writing invalid adresses
.extern			_SPORT02_FirstDMA_RxBufferAdress;
.extern			_SPORT02_LastDMA_RxBufferAdress; 
.extern			_SPORT02_FirstDMA_TxBufferAdress;
.extern			_SPORT02_LastDMA_TxBufferAdress; 
.extern			_SPORT13_FirstDMA_RxBufferAdress;
.extern			_SPORT13_LastDMA_RxBufferAdress; 
.extern			_SPORT13_FirstDMA_TxBufferAdress;
.extern			_SPORT13_LastDMA_TxBufferAdress;
.extern			_EP1_FirstDMA_BufferAdress;
.extern			_EP1_LastDMA_BufferAdress;

.segment/pm seg_pmco;
.global		_timer_int;
.global		_SPR0_int;				// SPORT0 Interrupt (Rx from SPORT02)
.global		_SPR1_int;				// SPORT1 Interrupt (Rx from SPORT13)
.global		_DMA11_int; 

//*********************************************************************************
//********** Timer Interrupt ******************************************************
//*********************************************************************************
_timer_int:	dm(_TempMode1_Timer) = mode1;
			bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

			bit set mode1 SRRFH|SRRFL;		// use secondary registers
			nop;
			r8=ASTAT;
			r2 = 0;							// save ALU status
			r1=dm(_IR_TimerCounter);
			r2= r1 + r2;
			if gt r1=r1-1;					// if r1>0 decrement r1
			dm(_IR_TimerCounter)=r1;
			ASTAT=r8;
			mode1 = dm(_TempMode1_Timer);	// use primary registers and restore mode1 register
			nop;
			RTS;
_timer_int.end:


//*********************************************************************************
//********** SPORT02-Receive = external TDM Receive Interrupt (SPORT0) ************
//*********************************************************************************


// CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION * CAUTION *
//
// Concept of acessing SPORT02 Rx Buffer pointer has changed: NEW CONCEPT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Because we don't use SPORT02 Tx for anything on UC, no change  with DMA2
//
//
//
//	SPORT02 runs with 6*10.666 kHz
//  UC DSP is running with 10.666 kHz - 6 SPORT02 Interrups will occur during a main cycle of UC DSP
//  For optimal low delay only 12 DMA buffers are required. But we have to update the DMA buffer pointers
//  6 times during a Main() cycle !!!!!!!
//  There will be tough timing requirements for data exchange.
//
//	TRICK: We use a buffer pointer array which has the start adress of every DMA buffer.
//	Acessing data can be done by incrementing the adress of the interrupt routine returns  
//
//
//	------------------------------ 10.666 kHz cycle
//			|TCB #0      GP=0  | -> DMA Rx Buffer #0
//			--------------------
//			|TCB #1      GP=1  | -> DMA Rx Buffer #1			N = 6
//			--------------------
//			|TCB #2      GP=2  | -> DMA Rx Buffer #2
//		N	--------------------
//			|TCB #3      GP=3  | -> DMA Rx Buffer #3
//			--------------------
//			|TCB #4      GP=4  | -> DMA Rx Buffer #4
//			--------------------
//			|TCB #5      GP=5  | -> DMA Rx Buffer #5
//	------------------------------ 10.666 kHz cycle
//			|TCB #6      GP=6  | -> DMA Rx Buffer #6
//			--------------------
//			|TCB #7      GP=7  | -> DMA Rx Buffer #7
//			--------------------
//			|TCB #8      GP=8  | -> DMA Rx Buffer #8
//		N	--------------------
//			|TCB #9      GP=9  | -> DMA Rx Buffer #9
//			--------------------
//			|TCB #10     GP=10 | -> DMA Rx Buffer #10
//			--------------------
//			|TCB #11     GP=11 | -> DMA Rx Buffer #11
//	------------------------------ 10.666 kHz cycle
//							|
//							|
//					Is used in Interrupt function (is checked if range 0...11 is ok)
//							|
//							------
//								  |		SPORT02_RxBufferLookupTable, is initialized in TDM_Init.c
//			----------------------------------------------------------
//								|   |   Adress Position 0 Buffer #10 |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #9  |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #8  |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #7  |
//					|			--------------------------------------
//					|			|   |   Adress Position 0 Buffer #6  |
//					|			--------------------------------------
//					|			| 11|   Adress Position 0 Buffer #5  |
//					|			--------------------------------------
//					|			| 10|   Adress Position 0 Buffer #4  |
//					|			--------------------------------------
//					|			|  9|   Adress Position 0 Buffer #3  |
//					|			--------------------------------------
//				(3 * N) - 1		|  8|   Adress Position 0 Buffer #2  |  ---------------------->
//					|			--------------------------------------	Interrupt routine returns 
//					|			|  7|   Adress Position 0 Buffer #1  |	in _externalTDM_Rx_Ptr the 
//					|			--------------------------------------	Adress to the SPORT02_RxBufferLookupTable 
//					|			|  6|   Adress Position 0 Buffer #0  |	containing the adress of the oldest	
//					|			--------------------------------------	DMA Rx Buffer
//					|			|  5|   Adress Position 0 Buffer #11 |	
//					|			--------------------------------------	acessing the 6 previous received frames is	 
//					|			|  4|   Adress Position 0 Buffer #10 |	done by saving the returned externalTDM_Rx_Ptr
//					|			--------------------------------------  and then using this temporary adress by getting
//					|			|  3|   Adress Position 0 Buffer #9  |  the values out of the buffers by	
//					|			--------------------------------------  *(TempAdress)     : oldest DMA Rx buffer Adress Position 0
//					|			|  2|   Adress Position 0 Buffer #8  |  *(TempAdress + 1) :			.			.
//					|			--------------------------------------  *(TempAdress + 2) :			.
//					|			|  1|   Adress Position 0 Buffer #7  |  *(TempAdress + 3) :			.
//					|			--------------------------------------  *(TempAdress + 4) :			.
//								|  0|   Adress Position 0 Buffer #6  |  *(TempAdress + 5) : newest DMA Rx Buffer Adress Position 0
//			---------------------------------------------------------- 
//	
//		CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !! CAUTION TIMING !!!!!!!!!!!!!!!!!!!!!!!!!
//		Because this externalTDM_Rx_Ptr changes during every SPORT02 Rx Interrupt, getting the values out of  
//		the DMA buffers has to be VERY QUICK and always at the beginning of the Main case after the while (.....).
//		ALL NESESSARY VALUES has to be copied out of the  DMA buffer BEFORE a NEW SPORT02 INTERRUPT occurs !!!!!!!!!!!!!
//
//      /END CAUTION

_SPR0_int:	dm(_TempMode1_SPORT02) = mode1;
			bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

			bit set mode1 SRRFH|SRRFL|SRD1L;// use secondary registers
			nop;
			r8=ASTAT;							// save ALU status
												
			r1 = dm(GP0A);						// Get the last pointer for Buffer Rx (SPORT0)
			r2 = dm(GP2A);						// Get the last pointer for Buffer Tx (SPORT2)
			r3 = 0x40000;						// PCI Bit
			r2 = r2 OR r3;						// Calculate adress of previous used DMA Tx Pointer
			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT02_FirstDMA_TxBufferAdress);
			r5 = r2 - r4;
			if LT r2 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT02_LastDMA_TxBufferAdress);
			r5 = r4 - r2;
			if LT r2 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			i0=_externalTDM_Rx_Ptr;				// Get adress of variable to write DMA Buffer adresses Rx
			i1=_externalTDM_Tx_Ptr;				// Get adress of variable to write DMA Buffer adresses Tx
			dm(i1,0)=r2;						// Write back DMA Buffer Tx adress for high level functions (DMA2)

			// This is belonging to the new concept
			r7 = _SPORT02_RxBufferLookupTable;	// Get the adress of this new DMA Rx Pointer lookup table
			r4 = dm(_SPORT02_RxBufferSize);		// Get the # DMA Buffers for boundary check
			r4 = r4 - 1;
			r5 = r4 - r1;
			if LT r1 = r4 - r4;					// Check if offset in BufferPointerArray is within array all the time
			r6 = r1 + r7;						// If it is outside, set it to first position
			dm(i0,0)= r6;						// Write the first adress of the buffer pointer array to the acess pointer


//			r5 = dm(II0A);					// Get the actual pointer for Buffer Rx
//			r5 = r5 OR r3;
//			i2 = _II_SPORT4_ValueActual;
//			i3 = _II_SPORT4_ValueOld;
//			r6 = dm(i3,0);
//			dm(i2,0) = r6;
//			dm(i3,0) = r5;					// Update II2 Register for SPORT2 DMA order check


			r4=dm(_SPORT02InterruptCounter);
			r4= r4 + 1;
			dm(_SPORT02InterruptCounter)=r4;

			ASTAT=r8;
 			mode1 = dm(_TempMode1_SPORT02);	// use primary registers and restore mode1 register
			nop;
			RTS;
_SPR0_int.end:
//*********************************************************************************
//********** SPORT13-Receive = internal TDM Receive Interrupt (SPORT1) ************
//*********************************************************************************
_SPR1_int:	dm(_TempMode1_SPORT13) = mode1;
			bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

			bit set mode1 SRRFH|SRRFL|SRD1L; // use secondary registers
			nop;
			r8=ASTAT;						// save ALU status
			
			r3 = 0x40000;					// PCI Bit
			r1 = dm(GP1A);					// Get the last pointer for Buffer Rx
			r1 = r1 OR r3;					// r1 = Adress of last DMA Rx Buffer
			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT13_FirstDMA_RxBufferAdress);
			r5 = r1 - r4;
			if LT r1 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT13_LastDMA_RxBufferAdress);
			r5 = r4 - r1;
			if LT r1 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			i0=_internalTDM_Rx_Ptr;
			dm(i0,0)=r1;

			r2 = dm(GP3A);					// Get the last pointer for Buffer Tx
			r2 = r2 OR r3;					// r2 = Adress of last DMA Tx Buffer
			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT13_FirstDMA_TxBufferAdress);
			r5 = r2 - r4;
			if LT r2 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT13_LastDMA_TxBufferAdress);
			r5 = r4 - r2;
			if LT r2 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			i1=_internalTDM_Tx_Ptr;
			dm(i1,0)=r2;
			
			r1 = dm(II1A);					// Get the actual pointer for Buffer Rx
			i0=_II_SPORT1_ValueActual;
			dm(i0,0)=r1;

			r7 = 0;
			r4=dm(_IR_BlockCounter);
			r7= r4 + r7;
			if gt r4=r4-1;					// if r4>0 decrement r4
			dm(_IR_BlockCounter)=r4;

			ASTAT=r8;
 			mode1 = dm(_TempMode1_SPORT13);	// use primary registers and restore mode1 register
			nop;
			RTS;
_SPR1_int.end:

.endseg;

//*********************************************************************************
//********** DMA11 Interrupt ******************************************************  
//*********************************************************************************
_DMA11_int:	dm(_TempMode1_DMA11) = mode1;
			bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

			bit set mode1 SRRFH|SRRFL|SRD1L;	// use secondary registers
			nop;
			r8=ASTAT;							// save ALU status
			
			r0 = dm(_DAC_Tx_Ptr);				// save previous DMA access Ptr for supersision
			dm(_PreviousDAC_Tx_Ptr) = r0;

			r2 = 0x40000;						// PCI Bit: Note that bit set  cmd is not working
			r1 = dm(GPEP1);
			r1 = r1 OR r2;
			// do check - preventing writing in non DMA buffer adresses
			r3 = dm(_EP1_FirstDMA_BufferAdress);
			r4 = r1 - r3;
			if LT r1 = r3;						// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r3 = dm(_EP1_LastDMA_BufferAdress);
			r4 = r3 - r1;
			if LT r1 = r3;						// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			 
			dm(_DAC_Tx_Ptr) = r1;				// save access pointer for data exchange DMA buffer DAC
	
			ASTAT=r8;
 			mode1 = dm(_TempMode1_DMA11);	// use primary registers and restore mode1 register
			nop;
			RTS;
_DMA11_int.end: