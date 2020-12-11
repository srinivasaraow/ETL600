/*-------------------------------------------------------------------------*
 * $Workfile: Interrupt_routines_FE_DSP.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Interrupt routine
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/ASM/Interrupt_routines_FE_DSP.asm $
 * 
 * 2     14.02.05 14:27 Chbrbus
 * Interrupt routine cleaned, preventing read and write in non DMA buffer
 * segments anytime
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 7     1.09.04 13:42 Rytz01
 * comments removed
 * 
 * 6     2.07.04 14:32 Bussli02
 * internalTDM_Rx_Ptr[2] has now "reverse order] than previous revision
 * 
 * 5     2.07.04 14:22 Bussli02
 * Reversed order buffer pointer array Internal TDM bus
 * 
 * 4     30.06.04 18:29 Maag01
 * 
 * 3     30.06.04 15:02 Bussli02
 * New concept with internal TDM bus 5.333 kHz cycle: Rx and Tx TDM
 * buffers are now a pointer array with old [1] and actual[0] 
 * 
 * 2     16.06.04 9:32 Rytz01
 * 
 * 1     9.06.04 13:53 Rytz01
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


.segment/dm seg_dmda;
.var			_IR_TimerCounter;
.global			_IR_TimerCounter;
.var			_IR_BlockCounter;
.global			_IR_BlockCounter;
.var			_externalTDM_Rx_Ptr;
.global			_externalTDM_Rx_Ptr;
.var			_externalTDM_Tx_Ptr;
.global			_externalTDM_Tx_Ptr;
.var			_internalTDM_Rx_Ptr[2];
.global			_internalTDM_Rx_Ptr;
.var			_internalTDM_Tx_Ptr[2];
.global			_internalTDM_Tx_Ptr;
.var			_SPORT02InterruptCounter;
.global			_SPORT02InterruptCounter;
.var			_Flag_IQData_Transmitted;
.global			_Flag_IQData_Transmitted;

// For saving mode1 register
.var			_TempMode1_SPORT13;
.var			_TempMode1_SPORT02;
.var			_TempMode1_Timer;

.endseg;

// DMA buffer adress check variables - preventing reading and writing invalid adresses
.extern			_SPORT02_FirstDMA_RxBufferAdress;
.extern			_SPORT02_LastDMA_RxBufferAdress; 
.extern			_SPORT02_FirstDMA_TxBufferAdress;
.extern			_SPORT02_LastDMA_TxBufferAdress; 
.extern			_SPORT13_FirstDMA_RxBufferAdress;
.extern			_SPORT13_LastDMA_RxBufferAdress; 
.extern			_SPORT13_FirstDMA_TxBufferAdress;
.extern			_SPORT13_LastDMA_TxBufferAdress;

.segment/pm seg_pmco;
.global		_timer_int;
.global		_SPR0_int;				// SPORT0 Interrupt (Rx from SPORT02)
.global		_SPR1_int;				// SPORT1 Interrupt (Rx from SPORT13)

//*********************************************************************************
//********** Timer Interrupt ******************************************************
//*********************************************************************************
_timer_int:	dm(_TempMode1_Timer) = mode1;	// Switch off SIMD (speeddsp library uses it)
			bit clr mode1 PEYEN;

			bit set mode1 SRRFH|SRRFL;		// use secondary registers
			nop;
			r8=ASTAT;
			r2 = 0;							// save ALU status
			r1=dm(_IR_TimerCounter);
			r2= r1 + r2;
			if gt r1=r1-1;					// if r1>0 decrement r1
			dm(_IR_TimerCounter)=r1;
			ASTAT=r8;
			mode1 = dm(_TempMode1_Timer);	 // use primary registers and restore mode1 register
			nop;
			RTS;
_timer_int.end:


//*********************************************************************************
//********** SPORT02-Receive = external TDM Receive Interrupt (SPORT0) ************
//********** FE_DSP <--> Downconverter AD6620
//*********************************************************************************
_SPR0_int:	dm(_TempMode1_SPORT02) = mode1;
			bit clr mode1 PEYEN;			 // Switch off SIMD (speeddsp library uses it)
			bit set mode1 SRRFH|SRRFL|SRD1L;// use secondary registers
			nop;
			r8=ASTAT;						// save ALU status

			r3 = 0x40000;					// PCI Bit
			r1 = dm(GP0A);					// Get the last pointer for Buffer Rx (SPORT0)
			r1 = r1 OR r3;

			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT02_FirstDMA_RxBufferAdress);
			r5 = r1 - r4;
			if LT r1 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT02_LastDMA_RxBufferAdress);
			r5 = r4 - r1;
			if LT r1 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			// Write back buffer pointer of last active DMA buffer
			i0=_externalTDM_Rx_Ptr;
			dm(i0,0)=r1;

			r2 = dm(GP2A);					// Get the last pointer for Buffer Tx (SPORT2)
			r2 = r2 OR r3;

			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT02_FirstDMA_TxBufferAdress);
			r5 = r2 - r4;
			if LT r2 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT02_LastDMA_TxBufferAdress);
			r5 = r4 - r2;
			if LT r2 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			// Write back buffer pointer of last active DMA buffer
			i1=_externalTDM_Tx_Ptr;
			dm(i1,0)=r2;
			
			// SPORT02 Interrupt Counter
			r4=dm(_SPORT02InterruptCounter);// stuff for testing, maybe later used
			r4= r4 + 1;						// counts the Interrupts of SPORT02
			dm(_SPORT02InterruptCounter)=r4;

			ASTAT=r8;						 // Restore ALU status
			mode1 = dm(_TempMode1_SPORT02);	 // use primary registers and restore mode1 register
			nop;
			RTS;
_SPR0_int.end:
//*********************************************************************************
//********** SPORT13-Receive = internal TDM Receive Interrupt (SPORT1) ************
//*********************************************************************************
_SPR1_int:	dm(_TempMode1_SPORT13) = mode1;
			bit clr mode1 PEYEN;			 // Switch off SIMD (speeddsp library uses it)
			bit set mode1 SRRFH|SRRFL|SRD1L; // use secondary registers
			nop;
			r8=ASTAT;						 // save ALU status
			
			// update always 2 buffer pointers for SPORT13 5.333 kHz cycle on FE and TP DSP
			i0 = _internalTDM_Rx_Ptr;
			r1 = dm(1,i0);					// actual Rx DMA puffer is now the previous old one
			dm(0,i0) = r1;					// Copy actual pointer to the old one
			i1 = _internalTDM_Tx_Ptr;
			r1 = dm(1,i1);					// actual Tx DMA puffer is now the previous old one	 
			dm(0,i1) = r1;					// Copy actual pointer to the old one

			r3 = 0x40000;					// PCI Bit
			r1 = dm(GP1A);					// Get the last pointer for Buffer Rx
			r1 = r1 OR r3;

			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT13_FirstDMA_RxBufferAdress);
			r5 = r1 - r4;
			if LT r1 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT13_LastDMA_RxBufferAdress);
			r5 = r4 - r1;
			if LT r1 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			// Write back buffer pointer of last active DMA buffer
			dm(1,i0)=r1;

			r2 = dm(GP3A);					// Get the last pointer for Buffer Tx
			r2 = r2 OR r3;

			// do check - preventing writing in non DMA buffer adresses
			r4 = dm(_SPORT13_FirstDMA_TxBufferAdress);
			r5 = r2 - r4;
			if LT r2 = r4;					// adress is less than first DMA buffer - set adress hard to first DMA buffer
			r4 = dm(_SPORT13_LastDMA_TxBufferAdress);
			r5 = r4 - r2;
			if LT r2 = r4;					// adress is higher than last DMA buffer - set adress hard to last DMA buffer
			// Write back buffer pointer of last active DMA buffer
			dm(1,i1)=r2;		
			
			r7 = 0;							// if _IR_BlockCounter > 0 then decrement _IR_BlockCounter
			r4=dm(_IR_BlockCounter);		// nesessary for while (IR_BlockCounter) in main loop
			r7= r4 + r7;
			if gt r4=r4-1;					
			dm(_IR_BlockCounter)=r4;
			
			ASTAT=r8;						// Restore ALU status
			mode1 = dm(_TempMode1_SPORT13); // use primary registers and restore mode1 register
			nop;
			RTS;
_SPR1_int.end:

.endseg;


