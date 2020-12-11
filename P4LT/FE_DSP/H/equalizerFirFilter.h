/*----------------------------------------------------------------------------*
 * $Workfile: equalizerFirFilter.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/equalizerFirFilter.h $
 * 
 * 1     16.02.05 13:59 Chhamaa
 * 
 * 2     28.01.05 18:22 Chhamaa
 * 
 * 1     26.01.05 13:25 Chhamaa
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef EQUALIZER_FIR_FILTER_H
#define EQUALIZER_FIR_FILTER_H


/* INCLUDE FILES **************************************************************/

#include "equalizerTypes.h"


/* GLOBAL TYPE DECLARATIONS ***************************************************/

typedef float t_EqualizerFirCoef[EQUALIZER_MAX_LENGTH];


/* PUBLIC FUNCTIONS ***********************************************************/

void equalizerFirFilter_init(t_EqualizerFirCoef *coef, long channel);
void equalizerFirFilter_activate(float *in, float *out, long nSamples, long channel);


#endif
