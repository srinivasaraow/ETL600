/*----------------------------------------------------------------------------*
 * $Workfile: BiIIR_Filter.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/BiIIR_Filter.h $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 2     1.11.04 14:16 Maag01
 * 
 * 1     22.07.04 16:41 Maag01
 * Initial version
 *
 *
 *----------------------------------------------------------------------------*/
 
#ifndef BIIIR_FILTER_H
#define BIIIR_FILTER_H

/* INCLUDE FILES **************************************************************/
/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {

  int numOfSec;          		// number of biquad filter sections
#ifndef _WIN32
  float pm *coefPtr;      		// pointer to filter coefficients
#else
  float    *coefPtr;      		// pointer to filter coefficients
#endif
  float *state1Ptr;       		// pointer to state array 1
  float *state2Ptr;       		// pointer to state array 2
  
} FilterDataType;


/* PUBLIC FUNCTIONS ***********************************************************/

void BiIIR_Filter_init(FilterDataType *filterData, float pm *coef, 
					   float *state1, float *state2, int numOfSec);
void BiIIR(float *inSample, float *outSample, float pm *coef, float *state, 
		   int sections, int length);

#endif
