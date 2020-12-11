/*-------------------------------------------------------------------------*
 * $Workfile: BiIIR_Filter.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Init function for assembly routine BiIIR()
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/C/BiIIR_Filter.c $ 
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 2     1.11.04 14:15 Maag01
 * 
 * 1     22.07.04 16:40 Maag01
 * Initial version
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "BiIIR_Filter.h"


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void BiIIR_Filter_init(FilterDataType *filterData, float *coef,
								       float *state1, float *state2, long numOfSec)
  Description:	Initializes filter parameters
  Inputs:		Fitler parameters
  Outputs:		Initalized filter parameters
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/

void SEGMENT_EXT_CODE BiIIR_Filter_init(FilterDataType *filterData, float pm *coef, 
									 float *state1, float *state2, int numOfSec)
{
  register long i;
  
  filterData->numOfSec = numOfSec;
  filterData->coefPtr = coef;
  filterData->state1Ptr = state1;
  filterData->state2Ptr = state2;
  
  // initialize filter state arrays 	
  for (i = 0; i < (2*numOfSec); i++) {                                          
	state1[i] = 0.0;
	state2[i] = 0.0;
  }
}
