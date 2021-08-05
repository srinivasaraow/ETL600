/*-------------------------------------------------------------------------*
 * $Workfile: interpolation.c $
 * Part of      : ETL600 / Up-converter DSP
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Interpolation functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/UC_DSP/C/interpolation.c $ 
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 1     6.05.04 14:47 Maag01
 * Initial version
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <complex.h>
#include "interpolation.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/

/* IMPLEMENTATION ************************************************************/


void Interp4_init_filt_c(complex_float *Delay_Line1, complex_float *Delay_Line2, complex_float *Delay_Line3)
{
	register complex_float **fc_ptr;
   
    vset_i_D((int *)Delay_Line1,0,2*(L_INT_FILT1/4+1));
    fc_ptr = (complex_float **)(((int *)Delay_Line1)+L_INT_FILT1/2+1);
    *fc_ptr = Delay_Line1;
   
    vset_i_D((int *)Delay_Line2,0,2*(L_INT_FILT2/4+1));
    fc_ptr = (complex_float **)(((int *)Delay_Line2)+L_INT_FILT2/2+1);
    *fc_ptr = Delay_Line2;
 
    vset_i_D((int *)Delay_Line3,0,2*(L_INT_FILT3/4+1));
    fc_ptr = (complex_float **)(((int *)Delay_Line3)+L_INT_FILT3/2+1);
    *fc_ptr = Delay_Line3;
 
}
