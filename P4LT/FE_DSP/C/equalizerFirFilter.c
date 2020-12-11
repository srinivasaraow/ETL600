/*-------------------------------------------------------------------------*
 * $Workfile: equalizerFirFilter.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : FIR filter functions to equalize APLC channels
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/FE_DSP/C/equalizerFirFilter.c $ 
 * 
 * 3     19.08.05 16:51 Chhamaa
 * segment ("seg_pmData") replaced with SEGMENT_PM_DATA2
 * segment ("seg_dmData") replaced with SEGMENT_DM_DATA2
 * 
 * 2     1.04.05 17:04 Chhamaa
 * 
 * 1     16.02.05 17:53 Chhamaa
 * 
 * 3     28.01.05 18:21 Chhamaa
 * 
 * 2     27.01.05 19:11 Chhamaa
 * 
 * 1     26.01.05 13:26 Chhamaa
 * Initial version
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "speeddsp.h"
#include "APLC_Types.h"
#include "equalizerFirFilter.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef float t_EqualizerDelayLine[EQUALIZER_MAX_LENGTH + 2];

/* LOCAL VARIABLES ***********************************************************/

// seg_dmData, seg_pmData used for even word alignment 
// (use only even numbers of vector elements)
t_EqualizerFirCoef	 SEGMENT_PM_DATA2 g_eqFirCoef[N_APLC_CHANNELS];
t_EqualizerDelayLine SEGMENT_DM_DATA2 g_eqDelayLine[N_APLC_CHANNELS];


/* IMPLEMENTATION ************************************************************/


/*****************************************************************************
  Function:		equalizerFirFilter_init()	
  Description:	Initalization of equalizer FIR filter delay line and coefficients
  Inputs:		channel number: channel	
  Outputs:		-			
  Return:		-
  Globals:		g_eqDelayLine		
  Important:	Call before using equalizerFirFilter_activate().

*******************************************************************************/

void SEGMENT_EXT_CODE equalizerFirFilter_init(t_EqualizerFirCoef *coef, long channel)
{
	long i;


	if ((channel >= APLC_CH1) && (channel <= APLC_CH3)) {
		
		// init delay line
		fir_f_init((float*)&g_eqDelayLine[channel], EQUALIZER_MAX_LENGTH);

		// set fir filter coefs
		for(i = 0; i < EQUALIZER_MAX_LENGTH; i++) {

			((float*)&g_eqFirCoef[channel])[i] = ((float*)coef)[i];
		};
	};
}


/*****************************************************************************
  Function:		equalizerFirFilter_activate()	
  Description:	FIR filter for APLC channel equalization
  Inputs:		pointer to input/output samples: in/out, number of samples: 
				nSamples, channel number: channel
  Outputs:		output samples
  Return:		-
  Globals:		g_eqFirCoef, g_eqDelayLine	
  Important:	Call equalizerFirFilter_init() before using this function.

*******************************************************************************/

void equalizerFirFilter_activate(float *in, float *out, long nSamples, long channel)
{

	vfir_f(out, in, nSamples, (pm float*)&g_eqFirCoef[channel], EQUALIZER_MAX_LENGTH, 
		   (float*)&g_eqDelayLine[channel]);

}





