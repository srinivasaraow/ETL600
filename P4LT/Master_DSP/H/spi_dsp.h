/*-------------------------------------------------------------------------*
 * $Workfile: spi_dsp.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/Master_DSP/H/spi_dsp.h $ 
 * 
 * 13    14-02-05 11:27 Chhamaa
 * SPI_SETTINGS_FOR_DDS modified to solve communication problems with DDS
 * chip.
 * 
 * 6     17.05.05 18:11 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 5     15.02.05 12:09 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem) 
 * 
 * 4     2.02.05 17:33 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem)
 * 
 * 3     25.01.05 18:36 Chmibuh
 * adapt DESELECT_SPI_SLAVE (flag7 removed)
 * 
 * 2     20.01.05 16:03 Chmibuh
 * PSSE | FLS1 added to all settings (automatic slave select with Flag1)
 * Flag 9 is no longer for DCD7005
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 9     18.10.04 17:48 Buhl01
 * SPI_SETTINGS_FOR_DDS_VIA_DMA added
 * 
 * 8     14.09.04 16:27 Buhl01
 * 
 * 7     11.08.04 13:56 Buhl01
 * 
 * 6     22.07.04 10:40 Buhl01
 * SPI_SETTINGS_FOR_UART_VIA_DMA added
 * 
 * 5     3.06.04 7:47 Rytz01
 * SPI_SETTINGS_FOR_DDS geändert.
 * 
 * 4     19.05.04 16:27 Buhl01
 * 
 * 3     18.05.04 13:45 Buhl01
 * 
 * 2     10.05.04 15:26 Buhl01
 * Initial
 *-------------------------------------------------------------------------*/

#ifndef SPI_DSP_H
#define SPI_DSP_H

#include <def21161.h>

//// Modify Flag 4-9 only! Only this Flags are used for SPI Chip Select
//#define CS_SPI_DDS						0xFFFFFFEF	//Flag8 low

//#define DESELECT_SPI_SLAVE				0x00000017  //IOFLAG |= DESELECT_SPI_SLAVE;
													//set Flag 4,5,6 & 8 high in every case.


	// for Prototype 2 HW use #define P4LW_PTTY2 - same as in IrigB.c & Init_Master_DSP.c, Interrupt_routines_Master_DSP.asm
#ifdef P4LW_PTTY2																
	// Use Flag 0 as IRIG-B#1 (in) & Flag 1 as N-SPI-CS-EN#1 (out)

	#define CS_SPI_DSP_AFE					0xFFFFFFF0	//IOFLAG &= CS_SPI_DSP_AFE;
	#define CS_SPI_DSP_AFE2					0xFFFFFFF1	
	#define CS_SPI_DSP_OFDM					0xFFFFFFF2
	#define CS_SPI_DSP_NSK					0xFFFFFFF3
	#define CS_SPI_DSP_TP					0xFFFFFFF4
	#define CS_SPI_MIC						0xFFFFFFF5
	#define CS_SPI_RTC						0xFFFFFFF6	
	#define CS_SPI_SER_FLASH				0xFFFFFFF7
	#define CS_SPI_DDS						0xFFFFFFEF	//Flag8 low

	#define DESELECT_SPI_SLAVE				0x00000017  //IOFLAG |= DESELECT_SPI_SLAVE;
														//set Flag 4,5,6 & 8 high in every case.

	// MIC = Microcontroller = UART
	#define SPI_SETTINGS_FOR_MIC			(MS | WL8 | BAUDR5 | SPIEN | PSSE | FLS1 | NSMLS | SENDZ | GM | SPTINT)
	#define SPI_SETTINGS_FOR_UART_VIA_DMA	(MS | WL8 | BAUDR5 | SPIEN | PSSE | FLS1 | NSMLS | SENDZ | GM | SPTINT | SPRINT | TDMAEN)

	#define SPI_SETTINGS_FOR_DSP_BOOT		(MS | WL32 | BAUDR2 | SPIEN | PSSE | FLS1 | NSMLS | SPTINT)
	#define SPI_SETTINGS_FOR_DSP			(MS | WL32 | BAUDR2 | SPIEN | PSSE | FLS1 | NSMLS | SPTINT)

	#define SPI_SETTINGS_FOR_RTC			(MS | WL8 | BAUDR5 | SPIEN | PSSE | FLS1 | NSMLS | SENDZ | GM | SPTINT | DF)
	#define SPI_SETTINGS_FOR_RTC_VIA_DMA	(MS | WL8 | BAUDR5 | SPIEN | PSSE | FLS1 | NSMLS | SENDZ | GM | SPTINT | SPRINT | TDMAEN | DF)

	#define SPI_SETTINGS_FOR_DDS			(MS | WL16 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | DF | CP)
	#define SPI_SETTINGS_FOR_DDS_VIA_DMA	(MS | WL16 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | SPRINT | TDMAEN  | DF | CP)

	#define SPI_SETTINGS_FOR_7005			(MS | WL32 | BAUDR2 | SPIEN | PSSE | FLS1 | NSMLS| SENDZ | GM | SPTINT | DF) 

	#define SPI_SETTINGS_FOR_SER_FLASH		(MS | WL8 | BAUDR2 | SPIEN | PSSE | FLS1 | NSMLS | SENDZ | GM | SPTINT | DF | CP | CPHASE)
#else	
//	heritage of Prototype 2 HW, now use Flag0,1,2,3 as SPI CS
	#define CS_SPI_DSP_AFE					(PSSE | FLS0|FLS1|FLS2|FLS3)
	#define CS_SPI_DSP_AFE2					(PSSE | FLS1|FLS2|FLS3)	
	#define CS_SPI_DSP_OFDM					(PSSE | FLS0|FLS2|FLS3)
	#define CS_SPI_DSP_NSK					(PSSE | FLS2|FLS3)
	#define CS_SPI_DSP_TP					(PSSE | FLS0|FLS1|FLS3)
	#define CS_SPI_MIC						(PSSE | FLS1|FLS3)
	#define CS_SPI_RTC						(PSSE | FLS0|FLS3)	
	#define CS_SPI_DDS						(PSSE | FLS3)

	#define DESELECT_SPI_SLAVE					0x00000000  //IOFLAG |= DESELECT_SPI_SLAVE;
														//do not set any Flag high - in every case.

	// MIC = Microcontroller = UART
	#define SPI_SETTINGS_FOR_MIC			(MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT)
	#define SPI_SETTINGS_FOR_UART_VIA_DMA	(MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT | SPRINT | TDMAEN)

	#define SPI_SETTINGS_FOR_DSP_BOOT		(MS | WL32 | BAUDR2 | SPIEN | NSMLS | SPTINT)
	#define SPI_SETTINGS_FOR_DSP			(MS | WL32 | BAUDR2 | SPIEN | NSMLS | SPTINT)

	#define SPI_SETTINGS_FOR_RTC			(MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT | DF)
	#define SPI_SETTINGS_FOR_RTC_VIA_DMA	(MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT | SPRINT | TDMAEN | DF)

	#define SPI_SETTINGS_FOR_DDS			(MS | WL16 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | DF | CP)
	#define SPI_SETTINGS_FOR_DDS_VIA_DMA	(MS | WL16 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | SPRINT | TDMAEN  | DF | CP)

	#define SPI_SETTINGS_FOR_7005			(MS | WL32 | BAUDR2 | SPIEN | NSMLS| SENDZ | GM | SPTINT | DF) 

	#define SPI_SETTINGS_FOR_SER_FLASH		(MS | WL8 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | DF | CP | CPHASE)

#endif //#ifdef P4LW_PTTY2	







#endif //#ifndef SPI_DSP_H



