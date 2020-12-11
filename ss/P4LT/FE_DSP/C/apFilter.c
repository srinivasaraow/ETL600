/*-------------------------------------------------------------------------*
 * $Workfile: apFilter.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Allpass filter functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/FE_DSP/C/apFilter.c $ 
 * 
 * 5     28.10.05 14:26 Chhamaa
 * ASSERT replaced by default settings in apFilter_init()
 * 
 * 4     28.06.05 11:58 Chchlee
 * ASSERTs checked
 * 
 * 3     23.05.05 11:42 Chchlee
 * some init functions moved to external RAM
 * 
 * 2     18.04.05 11:35 Chhamaa
 * 
 * 1     12.04.05 15:47 Chhamaa
 * Initial version
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES *************************************************************/

#include "DIAG.H"

#include "APLC_Types.h"
#include "FE_DSP_Types.h"

#include "apFilter.h"
#include "apFilterCoef.h"

#include "BiIIR_Filter.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef float dm t_ApFilterDelayLine[2*N_AP_SEC_MAX];

/* LOCAL VARIABLES ***********************************************************/

static long g_nApSections[N_APLC_CHANNELS];
static float pm *g_apCoef[N_APLC_CHANNELS];
static t_ApFilterDelayLine g_apFilterDelayLine[N_APLC_CHANNELS];


/* IMPLEMENTATION ************************************************************/


/*****************************************************************************
  Function:		void apFilter_init()		
  Description:	Initializes AP filters.
  Inputs:		pointer to t_FrontEndDSP_Config		
  Outputs:		none					
  Return:		none		
  Globals:		g_nApSections, g_apCoef		
  Important:	Call this function before using apFilter_activate()	

*******************************************************************************/

void SEGMENT_EXT_CODE apFilter_init(t_FrontEndDSP_Config *feDspConfig)
{
	long i;
	long channel;


	// set filter parameters
	switch(feDspConfig->channelMode) {

		case APLC_CHANNEL_MODE_A1_40I:

			g_apCoef[APLC_CH1] = ap0Coef;
			g_nApSections[APLC_CH1] = N_AP0_SEC;

			g_apCoef[APLC_CH2] = apCopyIn2OutCoef;
			g_nApSections[APLC_CH2] = N_AP_COPY_IN2OUT_SEC;

			g_apCoef[APLC_CH3] = apCopyIn2OutCoef;
			g_nApSections[APLC_CH3] = N_AP_COPY_IN2OUT_SEC;

			break;

		case APLC_CHANNEL_MODE_A1_40I_A2_40I:

			g_apCoef[APLC_CH1] = ap1Coef;
			g_nApSections[APLC_CH1] = N_AP1_SEC;

			g_apCoef[APLC_CH2] = ap2Coef;
			g_nApSections[APLC_CH2] = N_AP2_SEC;

			g_apCoef[APLC_CH3] = apCopyIn2OutCoef;
			g_nApSections[APLC_CH3] = N_AP_COPY_IN2OUT_SEC;
			break;

		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:

			g_apCoef[APLC_CH1] = ap1Coef;
			g_nApSections[APLC_CH1] = N_AP1_SEC;

			g_apCoef[APLC_CH2] = ap2Coef;
			g_nApSections[APLC_CH2] = N_AP2_SEC;

			g_apCoef[APLC_CH3] = ap3Coef;
			g_nApSections[APLC_CH3] = N_AP3_SEC;
			break;

		case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:

			g_apCoef[APLC_CH1] = ap1Coef;
			g_nApSections[APLC_CH1] = N_AP1_SEC;

			g_apCoef[APLC_CH2] = ap1Coef; 
			g_nApSections[APLC_CH2] = N_AP1_SEC;

			g_apCoef[APLC_CH3] = ap0Coef;
			g_nApSections[APLC_CH3] = N_AP0_SEC;
			break;

		default:

			g_apCoef[APLC_CH1] = apCopyIn2OutCoef;
			g_nApSections[APLC_CH1] = N_AP_COPY_IN2OUT_SEC;

			g_apCoef[APLC_CH2] = apCopyIn2OutCoef; 
			g_nApSections[APLC_CH2] = N_AP_COPY_IN2OUT_SEC;

			g_apCoef[APLC_CH3] = apCopyIn2OutCoef;
			g_nApSections[APLC_CH3] = N_AP_COPY_IN2OUT_SEC;
	}


	// check if ap filter has to be disabled
	if (!(feDspConfig->apFilterFlags & AP_FILTER_CH1_ON)) {

		g_apCoef[APLC_CH1] = apCopyIn2OutCoef;
		g_nApSections[APLC_CH1] = N_AP_COPY_IN2OUT_SEC;
	};

	if (!(feDspConfig->apFilterFlags & AP_FILTER_CH2_ON)) {

		g_apCoef[APLC_CH2] = apCopyIn2OutCoef;
		g_nApSections[APLC_CH2] = N_AP_COPY_IN2OUT_SEC;
	};

	if (!(feDspConfig->apFilterFlags & AP_FILTER_CH3_ON)) {

		g_apCoef[APLC_CH3] = apCopyIn2OutCoef;
		g_nApSections[APLC_CH3] = N_AP_COPY_IN2OUT_SEC;
	};

	
	// initialize filter delay lines
	for(channel = 0; channel <= APLC_CH3; channel++) {

		for(i = 0; i < 2*N_AP_SEC_MAX; i++) {

			((float*)&g_apFilterDelayLine[channel])[i] = 0.0;
		};
	};

}


/*****************************************************************************
  Function:		void apFilter_activate()		
  Description:	Calls the filter functions of the specified channel.
  Inputs:		Pointer to input/output samples: in, out; number of samples:
				nSamples, channel number: channel
  Outputs:		none					
  Return:		none	
  Globals:		g_nApSections, g_apCoef				
  Important:	Call apFilter_init() befor using this function.	

*******************************************************************************/

void apFilter_activate(float *in, float *out, int nSamples, long channel)
{

	BiIIR(in, out, g_apCoef[channel], (float*)&g_apFilterDelayLine[channel], 
		  g_nApSections[channel], nSamples);

}





