/*----------------------------------------------------------------------------*
 * $Workfile: apFilter.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/apFilter.h $
 * 
 * 1     12.04.05 15:48 Chhamaa
 * Initial version
 * 
 *----------------------------------------------------------------------------*/
 
#ifndef AP_FILTER_H
#define AP_FILTER_H


/* PUBLIC FUNCTIONS ***********************************************************/

void apFilter_init(t_FrontEndDSP_Config *feDspConfig);
void apFilter_activate(float *in, float *out, int nSamples, long channel);


#endif
