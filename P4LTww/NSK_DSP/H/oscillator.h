/*----------------------------------------------------------------------------*
 * $Workfile: oscillator.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/oscillator.h $
 * 
 * 1     20.01.05 8:33 Chchlee
 * 
 * 1     18.05.04 17:45 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef OSCILLATOR_H
#define OSCILLATOR_H

/* INCLUDE FILES **************************************************************/

#include "speeddsp.h"


/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {
	
	float phi;
	float deltaPhi;
	float cosPhi;
	float sinPhi;
	float cosDeltaPhi;
	float sinDeltaPhi;
	float amplitude;
	float ampCorrFactor;
	
} t_OscData;


/* PUBLIC FUNCTIONS ***********************************************************/

void oscillator_init(float amplitude, float oscFreq, float samplingFreq, t_OscData *oscData);
void oscillator_oscF(float *re, float *im, t_OscData *oscData, long blockLength);
void oscillator_oscC(complex_float *vOut, t_OscData *oscData, long blockLength);
void oscillator_AGC(t_OscData *oscData);


#endif
