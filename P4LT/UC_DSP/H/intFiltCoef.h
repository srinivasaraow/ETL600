/*--------------------------------------------------------------------------
 * $Workfile: intFiltCoef.h $
 * Part of		: ETL600 / Up-converter DSP
 * Language		: C
 * Created by	: 
 * Remarks		:  
 * Purpose		: Interpolation filter coefficients declaration
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/UC_DSP/H/intFiltCoef.h $ 
 * 
 * 2     19.08.05 16:50 Chhamaa
 * segment ("seg_pmData") replaced with SEGMENT_PM_DATA2
 * 
 * 1     20.01.05 8:33 Chchlee
 * 
 * 3     6.07.04 13:41 Maag01
 * Interpolation filter for first stage replaced
 * 
 * 2     5.07.04 16:28 Maag01
 * 
 * 1     6.05.04 14:53 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


#ifndef INT_FILT_COEF_H
#define INT_FILT_COEF_H


/* INCLUDE FILES ***********************************************************/

#include "interpolation.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

complex_float SEGMENT_PM_DATA2 pm interp4FiltCoeff1[L_INT_FILT1] =
{
	#include "h4x44_0.dat"    
	#include "h4x44_1.dat"
	#include "h4x44_2.dat"
	#include "h4x44_3.dat"
};

complex_float SEGMENT_PM_DATA2 pm interp4FiltCoeff2[L_INT_FILT2] =
{
	#include "h02.dat"
	#include "h12.dat"
	#include "h22.dat"
	#include "h32.dat"
};

complex_float SEGMENT_PM_DATA2 pm interp4FiltCoeff3[L_INT_FILT3] =
{
	#include "h03.dat"
	#include "h13.dat"
	#include "h23.dat"
	#include "h33.dat"
};


#endif
