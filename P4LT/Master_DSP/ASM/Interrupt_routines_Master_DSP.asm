/*-------------------------------------------------------------------------*
 * $Workfile: Interrupt_routines_Master_DSP.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Interrupt routine
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/ASM/Interrupt_routines_Master_DSP.asm $
 * 
 * 8     10.08.05 9:51 Chchlee
 * t_internalTime name changed
 * 
 * 7     17.05.05 18:11 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 6     14.02.05 17:39 Chbrbus
 * Bugfix: DMA Buffer Pointers for TDM busses are always in the valid DMA
 * buffer range. No read/write in invalid memory areas possible
 * 
 * 5     14.02.05 16:52 Chmibuh
 * 
 * 4     2.02.05 17:32 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem) 
 * 
 * 3     25.01.05 18:32 Chmibuh
 * FLG7 removed (SPI_DMArx_int)
 * 
 * 2     25.01.05 15:13 Chmibuh
 * FLG9 removed
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 8     11.01.05 13:33 Chharyt
 * Back to version 05.01.2005
 * 
 * 7     10.01.05 15:39 Chbrbus
 * SPORT02 IR now different between Rx and Tx, debug flag used 
 * 
 * 6     5.01.05 15:25 Chbrbus
 * Interrupt routine cleaned
 * 
 * 5     27.10.04 17:53 Buhl01
 * moved _spiRecvBuffer to segment seg_dma; DMA !
 * 
 * 4     26.10.04 15:38 Buhl01
 * fractional_ms new name (_)  _fractional_ms and global
 * 
 * 3     21.09.04 18:23 Buhl01
 * internal Time is now in "SPORT13-Receive = internal TDM Receive
 * Interrupt"
 * 
 * 2     22.07.04 10:40 Buhl01
 * _UartRequest_int & _SPI_DMArx_int added
 * 
 * 1     9.06.04 13:54 Rytz01
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

//.IMPORT "D:\ADSP\$P4LT\Master_DSP\h\rtc.h";
.IMPORT "rtc.h";

#define TOGGLE_MINUTE	0x00000001

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
.var			_II_SPORT13_ValueForSupervision[2];
.global			_II_SPORT13_ValueForSupervision;
.var			_II_SPORT02_ValueForSupervision[2];
.global			_II_SPORT02_ValueForSupervision;
.var			_SPORT02InterruptCounter;
.global			_SPORT02InterruptCounter;
.var			_global_dmaCount;
.global			_global_dmaCount;
//.var			_spiRecvBuffer[16]; // !!!!!!!!!!!!!!!!!
//.global			_spiRecvBuffer;
.var            _spiRecvindex;
.global         _spiRecvindex;
.var			_uartRequest;
.global			_uartRequest;
.var			_fractional_ms = 0;
/// test
.global			_fractional_ms;


// save mode1 register
.var			_TempMode1_IRQ0;
.var			_TempMode1_Timer;
.var			_TempMode1_SPI_Tx;
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

.segment/dm seg_dma;
.var			_spiRecvBuffer[32]; // !!!!!!!!!!!!!!!!!
.global			_spiRecvBuffer;
.endseg;

.segment/pm seg_pmco;
.global		_timer_int;
.global		_SPR0_int;				// SPORT0 Interrupt (Rx from SPORT02)
.global		_SPR1_int;				// SPORT1 Interrupt (Rx from SPORT13)
.global		_SPI_DMArx_int;
.global		_UartRequest_int;


.EXTERN STRUCT t_internalTime _internalTime;

//*********************************************************************************
//********** IRQ0 Interrupt Uart Request ******************************************
//*********************************************************************************
_UartRequest_int:
			dm(_TempMode1_IRQ0) = mode1;
			bit clr mode1 PEYEN;				// Switch off SIMD (speeddsp library uses it)

			bit set mode1 SRRFH|SRRFL|SRD1L;	// use secondary registers
			nop;
			r4=ASTAT;							// save ALU status
			
			r8 = 0x01;					
			dm(_uartRequest) = r8;				// set flag
			
			ASTAT=r4;
 			mode1 = dm(_TempMode1_IRQ0);		// use primary registers and restore mode1 register
			nop;
			RTS;
_UartRequest_int.end:
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
//********** SPI DMA TX Interrupt *************************************************
//*********************************************************************************
_SPI_DMArx_int: 
			dm(_TempMode1_SPI_Tx) = mode1;
			bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

			bit set mode1 SRRFH|SRRFL|SRD1L;	// use secondary registers
			nop;
			r8=ASTAT;							// save ALU status
						
			r2 = DM(SPIRX);					// read received data

			r0 = dm(_global_dmaCount);
			r0 = r0-1;
			dm(_global_dmaCount) = r0;
			r0 = pass r0;


			IF NE jump SPI_not_done_yet;

			ustat2=DM(IOFLAG);	

	// for Prototype 2 HW use #define P4LW_PTTY2 - same as in spi_dsp.h, SPI_access.h & Init_Master_DSP.c
#ifdef P4LW_PTTY2																

				bit set ustat2 FLG8|FLG6|FLG5|FLG4;
			
#endif					
			nop;
		
			r1 = 0x00000;
			dm(SPICTL)=r1;
			DM(IOFLAG) = ustat2;

SPI_not_done_yet:

			r3 = dm(_spiRecvindex);
			i0 = _spiRecvBuffer;
			m0 = r3;
			dm(m0,i0) = r2;
			r3 = r3 + 1;
			dm(_spiRecvindex) = r3;
			ASTAT=r8;
 			mode1 = dm(_TempMode1_SPI_Tx);	// use primary registers and restore mode1 register
			nop;
			RTS;
_SPI_DMArx_int.end:
//*********************************************************************************
//********** SPORT02-Receive = external TDM Receive Interrupt (SPORT0) ************
//*********************************************************************************
_SPR0_int:	dm(_TempMode1_SPORT02) = mode1;
			bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

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
			r9 = i0;
			r10 = i1;
			r11 = i2;
	
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
			
			// For SPORT13 supervision
			r5 = dm(II1A);					// Get the actual pointer for Buffer Rx
			r5 = r5 OR r3;
			i2 = _II_SPORT13_ValueForSupervision;
			r6 = dm(1,i2);
			dm(0,i2) = r6;
			dm(1,i2) = r5;					// Update II2 Register for SPORT2 DMA order check
			

			r7 = 0;
			r4=dm(_IR_BlockCounter);
			r7= r4 + r7;
			if gt r4=r4-1;					// if r4>0 decrement r4
			dm(_IR_BlockCounter)=r4;
			
	//////////////////////// internal Time /////////////////////////////////////
	// this depends extremly on the int. TDM bus configuration and assumes an interrupt every 93.75us (1/10.667KHz)
	// maincycle = 93.75us: 93.75us * 32 = 3 ms; 
	// 1. ms = 11 * 93.75 us = 1.03125 ms (little bit longer)
	// 2. ms = 11 * 93.75 us = 1.03125 ms (little bit longer) 
	// 3. ms = 10 * 93.75 us = 0.9375 ms  (little bit shorter)
	// after 3 ms we hit exact a ms bounday

			r0 = dm(_fractional_ms);		// count maincycles
			r0 = r0 + 1;
			dm(_fractional_ms) = r0;

			r2 = 0x0000000B;			// 11 times 93.75 us
			comp(r0, r2);
			if eq jump _add_milliSec;

			r2 = 0x00000016;			// 11 times 93.75 us
			comp(r0, r2);
			if eq jump _add_milliSec;

			r2 = 0x00000020;			// 10 times 93.75 us
			comp(r0, r2);
			if eq jump _add_milliSec_new;
			nop;
			jump _no_time_increment;
	_add_milliSec_new:
			r2 = 0;
			dm(_fractional_ms) = r2;
	_add_milliSec:
		//if(++internalTime.milisecond==1000)
			i0 = _internalTime->milisecond;
			r0 = dm(m5,i0);				// get millisecond
			r0 = r0 + 1;				// inc millisecond
			dm(m5,i0) = r0;				// write millisecond back
			r2 = 0x000003E8;			// set compare reg to 1000
			comp(r0, r2);
			if ne jump _no_time_increment;	
			r0 = 0;						// millisecond = 1000 -> reset
			dm(m5,i0) = r0;				// write millisecond back
		//if (++internalTime.second==60) 
			i0 = _internalTime->second;
			r0 = dm(m5,i0);				// get second
			r0 = r0 + 1;				// inc second
			dm(m5,i0) = r0;				// write second back
		// minute toggle - bit toggle every minute (all 60 sec) at second 30. (half minute) 
			r2 = 0x0000001E;
			comp(r0, r2);				// set compare reg to 30
			if ne jump _no_minute_toggle;
			i1 = _internalTime->togglebits;
			r2 = dm(m5,i1);
			r3 = TOGGLE_MINUTE;
			r2 = r2 xor r3;
			dm(m5,i1) = r2;
		_no_minute_toggle:
			r2 = 0x0000003C;			// set compare reg to 60
			comp(r0, r2);
			if ne jump _no_time_increment;	
			r0 = 0;						// second = 60 -> reset
			dm(m5,i0) = r0;				// write second back
		//if (++internalTime.minute==60)
			i0 = _internalTime->minute;
			r0 = dm(m5,i0);				// get minute
			r0 = r0 + 1;				// inc minute
			dm(m5,i0) = r0;				// write minute back
			//r2 = 0x0000003C;			// set compare reg to 60
			comp(r0, r2);
			if ne jump _no_time_increment;	
			r0 = 0;						// minute = 60 -> reset
			dm(m5,i0) = r0;				// write minute back
		//if (++internalTime.hour==24)
			i0 = _internalTime->hour;
			r0 = dm(m5,i0);				// get hour
			r0 = r0 + 1;				// inc hour
			dm(m5,i0) = r0;				// write hour back
			r2 = 0x00000018;			// set compare reg to 24
			comp(r0, r2);
			if ne jump _no_time_increment;	
			r0 = 0;						// hour = 24 -> reset
			dm(m5,i0) = r0;				// write hour back
		////////////////////////////////////
		// handel date!
		////////////////////////////////////
			i1 = _internalTime->month;  // i1 holds adr of month for the complete calculation
			r3 = dm(m5,i1);				// r3 holds month for the complete calculation
			i0 = _internalTime->date;	// i0 holds adr of date for the complete calculation	
			r0 = dm(m5,i0);				// get date
			r0 = r0 + 1;				// inc date
			dm(m5,i0) = r0;				// write date back
			r2 = 0x00000020;			// set compare reg to 32 -> check 31 days month (pre-modify) -> 32
			comp(r0, r2);
			if ne jump _check_for30days_month;
			//31 days month
			r0 = 1;						// date = 32 -> set date to 1
			dm(m5,i0) = r0;				// write date back
			i0 = _internalTime->month;
			r3 = r3 + 1;				// inc month
			dm(m5,i1) = r3;				// write month back
			jump _check_year;
	_check_for30days_month:
			r2 = 0x0000001F;			// set compare reg to 31 -> check 30 days month (pre-modify) -> 31
			comp(r0, r2);
			if ne jump _check_for29days_month;
			//30 days month - April, June, September & November
			r2 = 0x00000004;			// April?
			comp(r3, r2);
			if eq jump _inc30days_month;
			r2 = 0x00000006;			// June?
			comp(r3, r2);
			if eq jump _inc30days_month;
			r2 = 0x00000009;			// September?
			comp(r3, r2);
			if eq jump _inc30days_month;
			r2 = 0x0000000b;			// November?
			comp(r3, r2);
			if ne jump _no_time_increment;
		_inc30days_month:
			r3 = r3 + 1;				// inc month
			dm(m5,i1) = r3;				// write back month
			r3 = 1;						// reset date
			dm(m5,i0) = r3;				// write back date
			jump _no_time_increment;
	_check_for29days_month:
			r2 = 0x0000001E;			// set compare reg to 30 -> check 29 days month (pre-modify) -> 30
			comp(r0, r2);
			if ne jump _check_for28days_month;
			//29 day month
			r2 = 0x00000002;			// February?
			comp(r3, r2);
			if ne jump _no_time_increment;
			r3 = r3 + 1;				// inc month
			dm(m5,i1) = r3;				// write back month
			r3 = 1;						// reset date
			dm(m5,i0) = r3;				// write back date
			jump _no_time_increment;
	_check_for28days_month:
			r2 = 0x0000001D;			// set compare reg to 29 -> check 28 days month (pre-modify) -> 29
			comp(r0, r2);
			if ne jump _no_time_increment;
			//28 day month
			r2 = 0x00000002;			// February?
			comp(r3, r2);
			if ne jump _no_time_increment;
			i2 = _internalTime->year;
			r4 = dm(m5,i2);				// get year
			r0 = 0x03;					// mask for the two LSB - if the two LSB = 0 -> leap year
			r0 = r4 and r0;				//
			r0 = pass r0;
			if eq jump _no_time_increment;
			// no leap year
			r3 = r3 + 1;				// inc month
			dm(m5,i1) = r3;				// write back month
			r3 = 1;						// reset date
			dm(m5,i0) = r3;				// write back date
			jump _no_time_increment;
	_check_year: // r3 holds actual month, and dm(m5,i1) holds address 
			r2 = 0x0000000D;			// set compare reg to 13 -> check if December (12. month) (pre-modify) -> 13
			comp(r3, r2);
			if ne jump _no_time_increment;
			r3 = 1;						// new year!
			dm(m5,i1) = r3;				// set month to 1 - January
			i0 = _internalTime->year;
			r0 = dm(m5,i0);				// get year
			r0 = r0 + 1;				// inc year
			dm(m5,i0) = r0;				// write year back

	_no_time_increment:
			nop;
			i0 = r9;
			i1 = r10;
			i2 = r11;	

			ASTAT=r8;
 			mode1 = dm(_TempMode1_SPORT13);	// use primary registers and restore mode1 register
			nop;
			RTS;
_SPR1_int.end:

.endseg;

