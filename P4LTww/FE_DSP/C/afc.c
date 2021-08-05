/*-------------------------------------------------------------------------*
 * $Workfile: afc.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Functions to synchronize APLC rx frequency
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/FE_DSP/C/afc.c $ 
 * 
 * 14    12-09-14 16:48 Chhamaa
 * Modifications for APLC BW = 2.5 kHz.
 * 
 * 12    13.05.08 18:51 Chhamaa
 * New APLC modes added for MOD above speech.
 * 
 * 6     28.10.05 14:00 Chhamaa
 * ASSERT removed in getSgn()
 * 
 * 5     21.07.05 17:04 Chhamaa
 * Functions afc_init() and afc_reset() modified
 * 
 * 4     19.04.05 12:04 Chhamaa
 * MAX_FREQ_DEV modified
 * 
 * 3     4.03.05 18:50 Chhamaa
 * Constants related to pilot weight removed
 * 
 * 2     21.02.05 12:09 Chhamaa
 * In function afc_init(): afc->on set to FALSE
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 1     8.12.04 17:49 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <math.h>

#include "basetype.h"
#include "DIAG.H"
#include "FE_DSP_Const.h"
#include "afc.h"
#include "freqShift.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define KI_AFC_FAST				115.0/106.67	// 40 115 integral constant for start up (division by loop sampling rate)
#define KP_AFC_FAST				  0.1			// 0.8 0.2 proprotional constant for start up       
#define KI_AFC_SLOW				  1.0/106.67	// 0.05/106.67 0.1 integral constant for low jitter (division by loop sampling rate)
#define KP_AFC_SLOW				  0.0			// 0.0001 proprotional constant for low jitter
#define LF_INTEGRATOR_MAX        20.0           // [Hz] max. link frequency offset
#define MAX_FREQ_DEV		      0.05			// max. allowed frequency deviation: approx. 2 Hz
#define FAST_TRACKING_TIME     3000             // [s*10e-2] min. time to spend in fast tracking mode after exeeding max. frequency deviation
#define SYNC_START_UP		   3000             // [s*10e-2] time to guarantee sync. at start up
                                                
#define FDD_COEF_1                0.91          // fdd low pass filter constant
#define FDD_COEF_2				  0.09          // fdd low pass filter constant (corrected compared to 0.01 in ETL500) 
#define FDD_COEF_3                0.99          // fdd low pass filter constant
#define FDD_COEF_4				  0.01          // fdd low pass filter constant


/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

static float getSgn(unsigned long channelMode);


/* IMPLEMENTATION ************************************************************/

#pragma optimize_off

/*****************************************************************************
  Function:		afc_init()	
  Description:	Initializes APLC frequency synchronisation
  Inputs:		channelMode	
  Outputs:		afc				
  Return:		-
  Globals:		-
  Important:	Has to be called once before using frequencySync_activate()

*******************************************************************************/

void SEGMENT_EXT_CODE afc_init(t_AFC *afc, unsigned long channelMode)
{	
	afc->on = FALSE;
	afc->freqOffset = 0.0;
	afc->oscFreq = getShiftFreqAPLC(channelMode);
	afc->sgn = getSgn(channelMode);
	afc->dfLpState = 0.0;
	afc->ferr = 0.0;
	afc->stateI = 0.0;
	afc->fastTrackingCounter = FAST_TRACKING_TIME;  
}


/*****************************************************************************
  Function:		getSgn()	
  Description:	Returns the AFC sign based on the channel mode
  Inputs:		channelMode		
  Outputs:		-		
  Return:		sgn		
  Globals:		-
  Important:	-

*******************************************************************************/

static float SEGMENT_EXT_CODE getSgn(unsigned long channelMode)
{
	float sgn;


	switch(channelMode)
	{	 
		case APLC_CHANNEL_MODE_NONE:
		case APLC_CHANNEL_MODE_A1_20I:
		case APLC_CHANNEL_MODE_A1_25I:
		case APLC_CHANNEL_MODE_A1_40I:
		case APLC_CHANNEL_MODE_A1_40I_A2_20E:			// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:
		case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:
		
			sgn = 1.0;
			break;

		case APLC_CHANNEL_MODE_A1_20E_MAS:				// MOD above speech
		case APLC_CHANNEL_MODE_A1_40E:

			sgn = -1.0;
			break;

		default: sgn = 1.0;
	};

	return(sgn);
}


/*****************************************************************************
  Function:		afc_reset()	
  Description:	Resets some AFC parameters. Used for error handling in 
				afc_activate().
  Inputs:		-		
  Outputs:		afc		
  Return:		-		
  Globals:		-
  Important:	Do not use this function for initialisation. 

*******************************************************************************/

void afc_reset(t_AFC *afc)
{  
	afc->freqOffset = 0.0;
	afc->dfLpState = 0.0;
	afc->ferr = 0.0;
	afc->stateI = 0.0;
	afc->fastTrackingCounter = FAST_TRACKING_TIME;  
}


/*****************************************************************************
  Function:		afc_activate()	
  Description:	Adjusts the intermediate rx frequency. The integration  
				controller loop forces the pilot frequency offset to zero by
				increasing or decreasing  the rx frequency. The rx oscillator
				parameters are adjusted accordingly.			                                              
  Inputs:		afc, gStatus, pilotDemod		
  Outputs:		osc: oscillator parameters		
  Return:		-		
  Globals:		-
  Important:	Call afc_init() once before using this function.

*******************************************************************************/

void afc_activate(t_AFC *afc, GuardDataType *gStatus, PilotDemodDataType *pilotDemod,
				  t_OscData *osc)
{ 
	register float diffI, diffQ;
	register float in, quad;
	register float df, freqOffset;
	register float dPhi;


	if (afc->on) { 

		// scaling
		in = pilotDemod->filteredI*gStatus->rxGain*pilotDemod->rxLevelAtt;
		quad = pilotDemod->filteredQ*gStatus->rxGain*pilotDemod->rxLevelAtt;

		// differentiator
		diffI = in - afc->inOld;
		diffQ = quad - afc->quadOld;

		// save old values
		afc->inOld = in;
		afc->quadOld = quad;

		// calculate frequency error
		df = diffI*quad - diffQ*in; 

		// low pass filtering of frequency error
		afc->dfLpState = FDD_COEF_1*afc->dfLpState + FDD_COEF_2*df;
		afc->ferr = FDD_COEF_3*afc->ferr + FDD_COEF_4*afc->dfLpState;

		// PI controller
		afc->stateI += afc->ki*afc->dfLpState;
		afc->freqOffset = afc->kp*afc->dfLpState + afc->stateI;

		// error handling
		if (fabsf(afc->freqOffset) > LF_INTEGRATOR_MAX) afc_reset(afc);	// frequency out of range 

		freqOffset = afc->sgn*afc->freqOffset;

		// adjust IF oscillator
		dPhi = TWO_PI*(afc->oscFreq + freqOffset)/INT_SAMPLE_FREQ;
		osc->cosDeltaPhi = cosf(dPhi);
		osc->sinDeltaPhi = sinf(dPhi);

	}
}


/*****************************************************************************
  Function:		afc_monitor()	
  Description:	Monitors the AFC, generates afc fail alarm and updates APLC 
				status link frequncy offset.
  Inputs:		afc, gStatus, afcEnabled		
  Outputs:		aplc: APLC status		
  Return:		-		
  Globals:		-
  Important:	- 

*******************************************************************************/

void afc_monitor(t_AFC *afc, GuardDataType *gStatus, t_APLC_Status *aplc, BOOL afcEnabled)
{ 
	register float ferr;


	ferr = fabsf(afc->ferr);

	// evaluate frequency deviation 
	if ((ferr > MAX_FREQ_DEV) && (afc->fastTrackingCounter == 0)) {

		afc->fastTrackingCounter = FAST_TRACKING_TIME;
	}
	else if (afc->fastTrackingCounter > 0) {

		afc->fastTrackingCounter--;
	};

	// determine tracking mode
	if (afc->fastTrackingCounter > 0) {

		afc->ki = KI_AFC_FAST;
		afc->kp = KP_AFC_FAST;
		aplc->statusFlags |= AFC_FAIL_SET;
	}  
	else {

		afc->ki = KI_AFC_SLOW;
		afc->kp = KP_AFC_SLOW;
		aplc->statusFlags &= AFC_FAIL_CLEAR;
	}

	// decrement initCounter (initCounter is set only after new initalisation of the equipment)
	if (afc->initCounter > 0) afc->initCounter--; 

	// evaluate alarms
	afc->on = (gStatus->guardIsPresent && !gStatus->fastMute && !gStatus->snrAlarm 
		       && !gStatus->guardLevelAlarm && afcEnabled) || (afc->initCounter && afcEnabled);  

	if (!afc->on) aplc->statusFlags |= AFC_FAIL_SET;
	if (!afcEnabled) aplc->statusFlags &= AFC_FAIL_CLEAR;	// afc is set to off by the user -> do not generate a afc fail alarm

	// update status information
	aplc->linkFreqOffset = afc->freqOffset;
  
}
