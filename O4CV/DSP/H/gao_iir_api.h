/*****************************************************************************
*             Copyright (c) 1995 - 20xx by GAO Research Inc.                 *
*                          All rights reserved.                              *
*                                                                            *
*  This program is a confidential and an unpublished work of GAO Research    *
*  Inc. that is protected under international and Canadian copyright laws.   *
*  This program is a trade secret and constitutes valuable property of GAO   *
*  Research Inc. All use, reproduction and/or disclosure in whole or in      *
*  part of this program is strictly prohibited unless expressly authorized   *
*  in writing by GAO Research Inc.                                           *
*****************************************************************************/

#ifndef _GAO_IIR_BF_API_H
#define _GAO_IIR_BF_API_H

/*-------------------------------------------------------------------
 *                            Data types
 *-------------------------------------------------------------------*/

/*------- Global type definition for bit sensitive data -------*/
typedef unsigned char         UBYTE;  /* Unsigned 8 bit data   */
typedef signed char           SBYTE;  /* Signed 8 bit data     */

typedef unsigned short int    UWORD;  /* Unsigned 16 bit data  */
typedef signed short int      SWORD;  /* Signed 16 bit data    */

typedef unsigned int          UDWORD; /* Unsigned 32 bit data  */
typedef signed int            SDWORD; /* Signed 32 bit data    */

/*---------- DSP data type definition in fixed point ----------*/
typedef SWORD                 QWORD;
typedef SDWORD                QDWORD;


/*---------- Data stucture for IIR direct-form I --------------*/
typedef struct iir_df1_state_t {
	const fract16  *c;
	      fract16  *x;
	      fract16  *y;
	      int       k;
} iir_df1_05_state;


/*-------------------------------------------------------------------
 *                            API functions
 *-------------------------------------------------------------------*/
#define iir_df1_05_init(state, coeffs, xbuff, ybuff, stage_number) \
	(state).c = (coeffs);  \
	(state).x = (xbuff);  \
	(state).y = (ybuff);  \
	(state).k = (stage_number)

void iir_df1_05(const fract16 input[], fract16 output[], const int length, iir_df1_05_state *filter_state);

#endif  // _GAO_IIR_BF_API_H

