/*--------------------------------------------------------------------------
 * $Workfile: interpolation.h $
 * Part of		: ETL600 / Up-converter DSP
 * Language		: C
 * Created by	: 
 * Remarks		:  
 * Purpose		: Interpolation function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/interpolation.h $ 
 * 
 * 1     20.01.05 8:33 Chchlee
 * 
 * 2     6.07.04 13:41 Maag01
 * Interpolation filter for first stage replaced
 * 
 * 1     6.05.04 14:53 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


#ifndef INTERPOLATION_H
#define INTERPOLATION_H


/* INCLUDE FILES ***********************************************************/

#include <complex.h>


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define L_INT_FILT1	44
#define L_INT_FILT2 16
#define L_INT_FILT3 16


/* PUBLIC FUNCTIONS ********************************************************/

void Interp4_init_filt_c(complex_float *Delay_Line1, complex_float *Delay_Line2 ,
						 complex_float *Delay_Line3);

void Int4x44_filt_c(complex_float vOut[], complex_float vIn[], int nIn, 
					complex_float pm *vCoeffs, complex_float  *vDelay);
void Interp4_filt_c_L24(complex_float vOut[], complex_float vIn[], int nIn, 
						complex_float pm *vCoeffs, complex_float *vDelay); 
void Interp4_filt_c_L16(complex_float vOut[], complex_float vIn[], int nIn, 
						complex_float pm *vCoeffs, complex_float *vDelay);

#endif
