/*-------------------------------------------------------------------------*
 * $Workfile: Interrupt_routines_NSK_DSP.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Interrupt routine
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source O4CV/P4LT/NSK_DSP/ASM/Interrupt_routines_NSK_DSP.asm $
 * 
 * 7     08-02-25 9:58 Chtognu
 * Merged changes from ad.equ.branch:
 * Interrupt counter changes
 * 
 * 2     15.02.05 10:10 Chbrbus
 * Changed interrupt routine - TDM buffer pointers all time valid in DMA
 * buffer range
 * 
 * 1     19.01.05 15:35 Chchlee
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
.var			_IR_Counter;
.global			_IR_Counter;
.var			_externalTDM_Rx_Ptr;
.global			_externalTDM_Rx_Ptr;
.var			_externalTDM_Tx_Ptr;
.global			_externalTDM_Tx_Ptr;
.var			_internalTDM_Rx_Ptr;
.global			_internalTDM_Rx_Ptr;
.var			_internalTDM_Tx_Ptr;
.global			_internalTDM_Tx_Ptr;
.var			_II_SPORT1_ValueActual;
.global			_II_SPORT1_ValueActual;
.var			_SPORT02InterruptCounter;
.global			_SPORT02InterruptCounter;
.var			_Flag_IQData_Transmitted;
.global			_Flag_IQData_Transmitted;

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
			r8=ASTAT;						// save ALU status
			
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
			i0=_internalTDM_Rx_Ptr;
			dm(i0,0)=r1;

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
			i1=_internalTDM_Tx_Ptr;
			dm(i1,0)=r2;
			
			r1 = dm(II1A);					// Get the actual pointer for Buffer Rx
			i0=_II_SPORT1_ValueActual;
			dm(i0,0)=r1;
			
								
//			r2 = dm(GPT1B);					// Get the last pointer for Buffer Tx
//			i1=_internalTDM_Tx_Ptr;
//			dm(i1,0)=r2;					// _TDM_Tx_Ptr=*(GP7);

			r7 = 0;
			r4=dm(_IR_BlockCounter);
			r7= r4 + r7;
			if gt r4=r4-1;					// if r4>0 decrement r4
			dm(_IR_BlockCounter)=r4;

			r4=dm(_IR_Counter);
			r4= r4 + 1;
			dm(_IR_Counter)=r4;


			ASTAT=r8;
 			mode1 = dm(_TempMode1_SPORT13); // use primary registers and restore mode1 register
			nop;
			RTS;
_SPR1_int.end:

.endseg;


