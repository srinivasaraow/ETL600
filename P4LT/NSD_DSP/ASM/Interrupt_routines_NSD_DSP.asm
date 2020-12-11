/*-------------------------------------------------------------------------*
 * $Workfile: Interrupt_routines_NSD_DSP.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Interrupt routine
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/NSD_DSP/ASM/Interrupt_routines_NSD_DSP.asm $
 * 
 * 2     14.02.05 18:12 Chbrbus
 * Bugfix: DMA Buffer Pointers for TDM busses are always in the valid DMA
 * buffer range. No read/write in invalid memory areas possible
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 8     11.01.05 13:44 Chharyt
 * Back to version 04.01.2005
 * 
 * 5     4.01.05 15:07 Chbrbus
 * Changed IR routines after walkthrough - not completely tested !!!!!!!
 * 
 * 4     9.07.04 15:17 Bussli02
 * Communication with 2 G4AI, static words, tested  and working (N22 and
 * N28, AES FW)
 * 
 * 3     8.07.04 16:27 Plueer01
 * var _internalTDM_Rx_Ptr[2] and _internalTDM_Tx_Ptr[2] corrected as
 * array of 2
 * 
 * 2     8.07.04 14:06 Plueer01
 * interrupt routine  _SPR1_int copied from FE-DSP (it is now the same)
 * 
 * 1     9.06.04 13:49 Rytz01
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
.var			_externalTDM_Rx_Ptr[2];
.global			_externalTDM_Rx_Ptr;
.var			_externalTDM_Tx_Ptr[2];
.global			_externalTDM_Tx_Ptr;
.var			_internalTDM_Rx_Ptr[2];
.global			_internalTDM_Rx_Ptr;
.var			_internalTDM_Tx_Ptr[2];
.global			_internalTDM_Tx_Ptr;
.var			_II_SPORT02_ValueForSupervision[2];
.global			_II_SPORT02_ValueForSupervision;
.var			_II_SPORT13_ValueForSupervision[2];
.global			_II_SPORT13_ValueForSupervision;
.var			_SPORT02InterruptCounter;
.global			_SPORT02InterruptCounter;
// For saving mode1 register
.var			_TempMode1_Timer;
.var			_TempMode1_SPORT13;
.var			_TempMode1_SPORT02;

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
_SPR0_int:	dm(_TempMode1_SPORT02) = mode1;
			bit clr mode1 PEYEN;			 // Switch off SIMD (speeddsp library uses it)
				
			bit set mode1 SRRFH|SRRFL|SRD1L;// use secondary registers
			nop;
			r8=ASTAT;						// save ALU status

			// update always 2 buffer pointers for SPORT02 5.333 kHz cycle on FE and TP DSP
			i0 = _externalTDM_Rx_Ptr;
			r1 = dm(1,i0);					// actual Rx DMA puffer is now the previous old one
			dm(0,i0) = r1;					// Copy actual pointer to the old one
			i1 = _externalTDM_Tx_Ptr;
			r1 = dm(1,i1);					// actual Tx DMA puffer is now the previous old one	 
			dm(0,i1) = r1;					// Copy actual pointer to the old one

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
			dm(1, i0)=r1;

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
			dm(1, i1)=r2;

			// For SPORT02 supervision
			r5 = dm(II0A);					// Get the actual pointer for Buffer Rx
			r5 = r5 OR r3;
			i2 = _II_SPORT02_ValueForSupervision;
			r6 = dm(1,i2);
			dm(0,i2) = r6;
			dm(1,i2) = r5;					// Update II2 Register for SPORT2 DMA order check


			r4=dm(_SPORT02InterruptCounter);
			r4= r4 + 1;
			dm(_SPORT02InterruptCounter)=r4;

			ASTAT=r8;
 			mode1 = dm(_TempMode1_SPORT02); // use primary registers and restore mode1 register
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
			
			// Update flag for while() 
			r7 = 0;							// if _IR_BlockCounter > 0 then decrement _IR_BlockCounter
			r4=dm(_IR_BlockCounter);		// nesessary for while (IR_BlockCounter) in main loop
			r7= r4 + r7;
			if gt r4=r4-1;
			dm(_IR_BlockCounter)=r4;					

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
			
			// For SPORT13 supervision
			r5 = dm(II1A);					// Get the actual pointer for Buffer Rx
			r5 = r5 OR r3;
			i2 = _II_SPORT13_ValueForSupervision;
			r6 = dm(1,i2);
			dm(0,i2) = r6;
			dm(1,i2) = r5;					// Update II2 Register for SPORT13 DMA order check
		
			ASTAT=r8;						// Restore ALU status
			mode1 = dm(_TempMode1_SPORT13); // use primary registers and restore mode1 register
			nop;
			RTS;
_SPR1_int.end:

.endseg;

