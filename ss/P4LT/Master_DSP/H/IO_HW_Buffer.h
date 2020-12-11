/*---------------------------------------------------------------------------*
 * $Workfile: IO_HW_Buffer.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P.Rytz  
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/H/IO_HW_Buffer.h $
 * 
 * 4     5.10.05 9:21 Chchlee
 * Comments added
 * 
 * 3     28.09.05 14:43 Chchlee
 * isInDebugMode() macro added
 * 
 * 2     21.07.05 9:59 Chchlee
 * DEBUG_FLAG_MASK added
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 6     9.12.04 19:41 Chmibuh
 * HW_BIT_MASK added
 * 
 * 5     2.08.04 13:20 Leeb02
 * IO_BUFFER_DSP
 * 
 * 4     17.06.04 13:00 Rytz01
 * 
 * 3     16.06.04 17:39 Rytz01
 * 
 * 2     16.06.04 17:35 Rytz01
 * New defines added
 * 
 * 1     16.06.04 17:31 Rytz01
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef _IO_HW_BUFFER
#define _IO_HW_BUFFER

/* INCLUDE FILES *************************************************************/
#include "basetype.h"
#include <def21161.h>

/* PUBLIC TYPE DECLARATIONS **************************************************/

/* PUBLIC CONSTANT DECLARATIONS **********************************************/

#define DEBUG_FLAG_MASK			0x00000008  // Flag 7


#define	IO_BUFFER_DSP			0x08000000	//external I/O Buffer Address (/MS2)

// use the unused DSP registers MSGR0 and MSGR1 for intermediate I/O buffer storage
#define IO_BUFFER_WRITE			(*(long*)(MSGR0))
#define IO_BUFFER_READ			(*(long*)(MSGR1))

// Out (write) Buffer D16 - D21
#define	HW_AL_LED_DSP				BIT0
#define	LINK_AL_LED					BIT1
#define	SYS_AL_LED					BIT2
#define	P4LT_AL_LED					BIT3
#define	WARNING_LED					BIT4
#define	READY_LED					BIT5
#define TP1101						BIT6
#define TP1102						BIT7

// Out (write) Buffer D24 - D31
#define	RELAY1						BIT8
#define	RELAY2						BIT9
#define	RELAY3						BIT10
#define	R1BC_CAB_AL					BIT11
#define R1BC_RES_1					BIT12
#define R1BC_RES_2					BIT13
#define N_EN_CLK_INT_TDM			BIT14
#define N_EN_CLK_EXT_TDM			BIT15



// IN (read) Buffer D16 -D23 (Mask not tested!!!)
#define		HW_BIT0    				BIT0   // HW version P4LW, Bit 0
#define		HW_BIT1    				BIT1   // HW version P4LW, Bit 1
#define		HW_BIT2    				BIT2   // HW version P4LW, Bit 2
#define		PLUG_OUT3				BIT3   // no hybrid plugged
#define		PLUG_OUT4  				BIT4   // no rx filter plugged
#define		WARNING_IN    			BIT5   // warning from 80W amp
#define		TXRF_AL					BIT6   // tx level too low alarm
#define		TXRF_INPUT				BIT7   // dummyload plugged

#define		HW_BIT_MASK				(HW_BIT0 | HW_BIT1 | HW_BIT2)

/* PUBLIC MACRO DEFINITIONS **************************************************/


// read debug flag
// check for Debug Flag (Flag7) Flag7 = 0 if JtagBoard with J1 closed is plugged, else Flag7 = high 

#define isInDebugMode() (!((*(long*)IOFLAG) & (DEBUG_FLAG_MASK)))


/* PUBLIC FUNCTIONS **********************************************************/


#endif

