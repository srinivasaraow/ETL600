/*****************************************************************************
*             Copyright (c) 1995 - 2006 by GAO Research Inc.                 *
*                          All rights reserved.                              *
*                                                                            *
*  This program is a confidential and an unpublished work of GAO Research    *
*  Inc. that is protected under international and Canadian copyright laws.   *
*  This program is a trade secret and constitutes valuable property of GAO   *
*  Research Inc. All use, reproduction and/or disclosure in whole or in      *
*  part of this program is strictly prohibited unless expressly authorized   *
*  in writing by GAO Research Inc.                                           *
*****************************************************************************/

#ifndef _CPDEF_H_
#define _CPDEF_H_

#define RM_DC_ENABLE			(1)		/* Remove DC offset of input enable or not */

#define BASIC_FRAMEUNIT			(3)		/* how many 10ms subframes in one frame */
#define TONE_THRESHOLD			(17)	/* Absolut peak tone thresold for CP 
										   detection: 
											    T_T = 14222.6*(2^(min_dB/3))
											T_T = 140 for -20dB - original setting
											T_T =  17 for -29dB - new setting
											==> about 5 leading 0bits possible  
											Note: dB refer to amplitude of PCM  */
#define TOTALEGY_THR_ENABLE		(1)		/* Total Energy threshold enable or not */
#define CP_SAMPLE_PERFRAME		(80)	/* for 10ms subframe, 8k Hz sample rate */
#define HISTORY_BUF_LEN			(100)	/* Define how many flags. One flag per frame. 
										   The maximum duration of one cadence of any
										   CP tone CANNOT be greater than 
										   HISTORY_BUF_LEN*BASIC_FRAMEUNIT*10ms.
										   Otherwise, that tone cannot be detected. */
#define NOTONE_DETECTED			(0xFF)
#define MORETONE_DETECTED		(0xFE)

/*** used for debug ***/
#define CP_DEBUG		0
#if CP_DEBUG
#include <stdio.h>
#define PRINTJ0(a)			printf(a)
#define PRINTJ1(a,b)		printf(a,b)
#define PRINTJ2(a,b,c)		printf(a,b,c)
#define PRINTJ3(a,b,c,d)	printf(a,b,c,d)
#else
#define PRINTJ0(a)
#define PRINTJ1(a,b)
#define PRINTJ2(a,b,c)
#define PRINTJ3(a,b,c,d)
#endif

#endif
