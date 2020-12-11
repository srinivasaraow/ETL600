/*---------------------------------------------------------------------------*
 * $Workfile: freqShift.c $
 * Part of      : ETL600 / Front end DSP
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Frequency shift routines
 * Copyright    : ABB Switzerland Ltd., 2003
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/FE_DSP/C/freqShift.c $
 * 
 * 12    12-09-14 16:48 Chhamaa
 * Modifications for APLC BW = 2.5 kHz.
 * 
 * 10    9.05.08 18:23 Chhamaa
 * New APLC modes added for MOD above speech.
 * 
 * 5     31.01.06 16:28 Chpeplu
 * new define SHIFT_FREQ_APLC_1, new channel mode APLC_CHANNEL_MODE_A1_20I
 * used
 * 
 * 3     28.10.05 14:13 Chhamaa
 * Temp. variable shiftFreq set to 0.0  at initialization
 * 
 * 2     19.08.05 16:53 Chhamaa
 * segment ("seg_pmData") replaced with SEGMENT_PM_DATA2
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 9     2.12.04 14:58 Chhamaa
 * New functions getShiftFreqAPLC(), getShiftFreqCh3() created
 * 
 * 8     28.10.04 18:19 Maag01
 * APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I added
 * 
 * 7     6.10.04 17:48 Maag01
 * 
 * 6     26.08.04 15:20 Maag01
 * 
 * 5     14.07.04 12:21 Leeb02
 * Dependencies removed
 * 
 * 4     12.07.04 17:57 Maag01
 * 
 * 3     8.07.04 19:24 Maag01
 * 
 * 2     17.06.04 16:10 Maag01
 * 
 * 1     30.04.04 14:45 Maag01
 * Initial version
 * 
 *
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include "basetype.h"
#include "DIAG.H"

#include "speedDsp.h"
#include "FE_DSP_Const.h"
#include "APLC_Types.h"
#include "freqShift.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define SHIFT_FREQ_APLC_0		 15000.0	// used for 1 channel mode, BW = 2 kHz
#define	SHIFT_FREQ_APLC_1		 14000.0	// used for 1 channel mode, BW = 4 kHz
#define	SHIFT_FREQ_APLC_2		 12000.0	// used for 2 and 3 channel mode, BW = 8, 12 kHz
#define SHIFT_FREQ_APLC_3		 12800.0	// used for 3 channel mode, BW = 8 kHz
#define SHIFT_FREQ_APLC_4		 14750.0	// used for 1 channel mode, BW = 2.5 kHz

#define SHIFT_FREQ_CH3_1	 	  6000.0	// used for 3 channel mode, BW = 12 kHz
#define SHIFT_FREQ_CH3_2 		  4400.0	// used for 3 channel mode, BW = 8 kHz

#define OSC_AMPLITUDE				 1.0


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

static float getShiftFreqCh3(unsigned long channelMode);


/* IMPLEMENTATION ************************************************************/

// optimization is turned off for initialization functions in ext RAM
#pragma optimize_off

/*****************************************************************************
  Function:		getShiftFreqAPLC()	
  Description:	Returns the APLC shift frequency based on the channel mode 
  Inputs:		channelMode
  Outputs:		None
  Return:		shiftFreq
  Globals:		None
  Important:	-

******************************************************************************/

float SEGMENT_EXT_CODE getShiftFreqAPLC(unsigned long channelMode)
{
	float shiftFreq = 0.0;


	switch(channelMode)
	{	 
		case APLC_CHANNEL_MODE_NONE:

			shiftFreq = 0.0;
			break;

		case APLC_CHANNEL_MODE_A1_20E:
		case APLC_CHANNEL_MODE_A1_20I:

			shiftFreq = SHIFT_FREQ_APLC_0;
			break;

		case APLC_CHANNEL_MODE_A1_25E:
		case APLC_CHANNEL_MODE_A1_25I:

			shiftFreq = SHIFT_FREQ_APLC_4;
			break;

		case APLC_CHANNEL_MODE_A1_20E_MAS:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40E:
		case APLC_CHANNEL_MODE_A1_40I:

			shiftFreq = SHIFT_FREQ_APLC_1;
			break;
		
		case APLC_CHANNEL_MODE_A1_40I_A2_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:

			shiftFreq = SHIFT_FREQ_APLC_2;
			break;

		case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:

			shiftFreq = SHIFT_FREQ_APLC_3;
			break;

		default: ASSERT(FALSE);
	};

	return(shiftFreq);
};


/*****************************************************************************
  Function:		initFreqShiftAPLC()	
  Description:	Initializes frequency shift parameters used for aggregate 
  				APLC signal.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	Call before using freqShift().

******************************************************************************/

void SEGMENT_EXT_CODE initFreqShiftAPLC(t_OscData *oscShiftTx, t_OscData *oscShiftRx, 
											unsigned long channelMode)
{
	float shiftFreqTx;
	float shiftFreqRx;


	shiftFreqTx = -getShiftFreqAPLC(channelMode);
	shiftFreqRx =  getShiftFreqAPLC(channelMode);

	oscillator_init(OSC_AMPLITUDE, shiftFreqTx, INT_SAMPLE_FREQ, oscShiftTx);
	oscillator_init(OSC_AMPLITUDE, shiftFreqRx, INT_SAMPLE_FREQ, oscShiftRx);
};


/*****************************************************************************
  Function:		getShiftFreqCh3()	
  Description:	Returns the Ch3 shift frequency based on the channel mode 
  Inputs:		channelMode
  Outputs:		None
  Return:		shiftFreq
  Globals:		None
  Important:	-

******************************************************************************/

static float SEGMENT_EXT_CODE getShiftFreqCh3(unsigned long channelMode)
{
	float shiftFreq = 0.0;

	switch(channelMode)
	{	 
		case APLC_CHANNEL_MODE_NONE:
		case APLC_CHANNEL_MODE_A1_20E:
		case APLC_CHANNEL_MODE_A1_20I:
		case APLC_CHANNEL_MODE_A1_25E:
		case APLC_CHANNEL_MODE_A1_25I:
		case APLC_CHANNEL_MODE_A1_20E_MAS:		// MOD above speech
		case APLC_CHANNEL_MODE_A1_40E:
		case APLC_CHANNEL_MODE_A1_40I:
		case APLC_CHANNEL_MODE_A1_40I_A2_20E:	// MOD above speech	
		case APLC_CHANNEL_MODE_A1_40I_A2_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I:

			shiftFreq = 0.0;
			break;

		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	// MOD above speech
		case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E:
		case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:

			shiftFreq = SHIFT_FREQ_CH3_1;
			break;

		case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:

			shiftFreq = SHIFT_FREQ_CH3_2;
			break;

		default: ASSERT(FALSE);
	};

	return(shiftFreq);
};


/*****************************************************************************
  Function:		initFreqShiftCh3()
  Description:	Initializes frequency shift parameters used for APLC  
 				channel 3.			
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	Call before using freqShift().

******************************************************************************/

void SEGMENT_EXT_CODE initFreqShiftCh3(t_OscData *oscShiftTx, t_OscData *oscShiftRx,
									   unsigned long channelMode)
{
	float shiftFreqTx;
	float shiftFreqRx;


	shiftFreqTx =  getShiftFreqCh3(channelMode);
	shiftFreqRx = -getShiftFreqCh3(channelMode);

	oscillator_init(OSC_AMPLITUDE, shiftFreqTx, INT_SAMPLE_FREQ/3.0, oscShiftTx);
	oscillator_init(OSC_AMPLITUDE, shiftFreqRx, INT_SAMPLE_FREQ/3.0, oscShiftRx);
};


// optimization is turned on as specified on the command line
#pragma optimize_as_cmd_line

/*****************************************************************************
  Function:		freqShift()	
  Description:	Performs a frequency shift of a complex signal.
  Inputs:		in: pointer to input sample array
  				osc: pointer to oscillator parameters
  				nSamples: number of complex samples (max. 12)
  Outputs:		out: pointer to output sample array
  Return:		None
  Globals:		None
  Important:	Call initFreqShiftAPLC() or initFreqShiftCh3() before using.

******************************************************************************/

void freqShift(float *in, float *out, t_OscData *osc, int nSamples)
{
	// static used to improve speed
	// seg_pmData used for even word alignment
	static float SEGMENT_PM_DATA2 pm vSin[N_SAMPLES_IF_C], vCos[N_SAMPLES_IF_C];
	static float SEGMENT_PM_DATA2 pm mulSin[N_SAMPLES_IF_C];
	
	
	// generate oscillator samples
	oscillator_oscF(vCos, vSin, osc, nSamples);
	oscillator_AGC(osc);

	// in-phase component:
	// out[i] = in[i]*cos - in[i + nSamples]*sin;
	vmul_f_PDP(mulSin, &in[nSamples], vSin, nSamples);
	vmulsub_f_DDPP(out, in, vCos, mulSin, nSamples);
	
	// quadrature component
	// out[i + nSamples] = in[i]*sin + in[i + nSamples]*cos;
	vmul_f_PDP(mulSin, in, vSin, nSamples);
	vmuladd_f_DDPP(&out[nSamples], &in[nSamples], vCos, mulSin, nSamples);
	
};



	
 
