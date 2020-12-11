/*-------------------------------------------------------------------------*
 * $Workfile: txSignalProcessingOfdm.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : processing of OFDM tx signal
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/P4LT/FE_DSP/C/txSignalProcessingOfdm.c $ 
 * 
 * 12    22.09.08 15:46 Chhamaa
 * Frequency shift for MOD BW 1700 Hz modified.
 * 
 * 11    26.08.08 9:40 Chhamaa
 * Frequency shift for MOD BW 1700 Hz modified.
 * 
 * 10    23.07.08 10:27 Chhamaa
 * Shift frequency for DPLC BW 6 kHz modified.
 * 
 * 9     9.05.08 18:25 Chhamaa
 * Filtering for new DPLC BW 2, 6 kHz added.
 * 
 * 6     14.11.06 13:48 Chpeplu
 * Function txSignalProcessingOfdm_init() changed for custom mode
 * 
 * 4     21.07.06 10:53 Chhamaa
 * Comment see below.
 * 
 * 4     21.06.06 13:51 Chtognu
 * FREQ_OFFSET_BW_3500 set to 2272 Hz
 * 
 * 3     6.04.06 14:01 Chhamaa
 * FREQ_OFFSET_BW_3500 set to 2210 Hz
 * 
 * 2     15.03.06 14:43 Chhamaa
 * - FREQ_OFFSET_BW_8000 set to 4000
 * - DPLC BW 3.5 kHz: Lowpass filter, BW = 4000 Hz, Low Ripple
 * 
 * 1     7.02.06 9:52 Chhamaa
 * Initial version.
 * OFDM BW 3.5 kHz makes use of IIR filter BW 4.0 kHz.
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "DIAG.H"
#include "NSD_Types.h"

#include "txSignalProcessingOfdm.h"
#include "FE_DSP_Const.h"
#include "ofdmFilterCoef.h"
#include "BiIIR_Filter.h"
#include "freqShift.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define OSC_AMPLITUDE				1.0
#define PILOT_BW					500	// [Hz]
#define SSB_OFFSET					150 // [Hz]
#define SPEECH_BW_2000			   2000 // [Hz]

#define NOMINAL_TX_FREQ			 -16000	// [Hz]

#define FREQ_OFFSET_BW_1700		   3309 // [Hz] 3259 + 50 for ETL guard at 2220 Hz

#define FREQ_OFFSET_BW_3500		   2272 // [Hz]
#define FREQ_OFFSET_BW_4000		   2000	// [Hz]

#define FREQ_OFFSET_BW_5300		   5467 // [Hz] 5427 +40hz=5467 +60hz=5487
   
#define FREQ_OFFSET_BW_7500		   4278	// [Hz]
#define FREQ_OFFSET_BW_8000		   4000	// 4005 [Hz]


/* LOCAL VARIABLES ***********************************************************/

static FilterDataType filterData;
static t_OscData osc;
static float dm filterStateI[OFDM_LPF_STATE_MAX];
static float dm filterStateQ[OFDM_LPF_STATE_MAX];

// for testing
static float g_shiftFreq;


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		txSignalProcessingOfdm_init()	
  Description:	Initialises parameters for processing of OFDM tx signal
  Inputs:		config: pointer to FE DSP configuration
  Return:		None
  Globals:		
  Important:	Call this function before using txSignalProcessingOfdm_activate().

******************************************************************************/

#pragma optimize_off

void SEGMENT_EXT_CODE txSignalProcessingOfdm_init(t_FrontEndDSP_Config *config)
{
	int numOfSec;
	float pm *pCoef;
	float shiftFreq;
	long aplcBw;
	long dplcBw;


	GET_APLC_BANDWIDTH((*config), aplcBw);

	// check if APLC pilot is in DPLC band
	// @Pl_ext
	if ((config->DPLC_BW > 0) && (config->DPLC_BW <= 8000) && 
		((config->channelMode == APLC_CHANNEL_MODE_NONE) ||
		 ((config->intTeleprotMode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_DPLC) ||
		 ((config->intTeleprotMode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC))) {

		dplcBw = config->DPLC_BW - PILOT_BW;
	}
	else {

		dplcBw = config->DPLC_BW;
	};

	switch(dplcBw) {

		case 2000:

			numOfSec = COPY_IN2OUT_FILTER_SEC;
			pCoef = copyIn2OutFilterCoef;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + FREQ_OFFSET_BW_1700 - SPEECH_BW_2000 - SSB_OFFSET;
			break;

		case 3500:
			
			numOfSec = OFDM_FILTER_SEC_4000_LR;		//OFDM_FILTER_SEC_3500;
			pCoef = ofdmFilterCoef4000LR;			//ofdmFilterCoef3500;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + FREQ_OFFSET_BW_3500;
			break;

		case 4000:
			
			numOfSec = OFDM_FILTER_SEC_4000;
			pCoef = ofdmFilterCoef4000;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + FREQ_OFFSET_BW_4000;
			break;

		case 6000:

			numOfSec = COPY_IN2OUT_FILTER_SEC;
			pCoef = copyIn2OutFilterCoef;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + FREQ_OFFSET_BW_5300 - SPEECH_BW_2000 - SSB_OFFSET;
			break;

		case 7500:
			
			numOfSec = OFDM_FILTER_SEC_7500;
			pCoef = ofdmFilterCoef7500;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + FREQ_OFFSET_BW_7500;
			break;

		case 8000:
			
			numOfSec = OFDM_FILTER_SEC_8000;
			pCoef = ofdmFilterCoef8000;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + FREQ_OFFSET_BW_8000;
			break;

		default:

			numOfSec = COPY_IN2OUT_FILTER_SEC;
			pCoef = copyIn2OutFilterCoef;
			shiftFreq = NOMINAL_TX_FREQ + aplcBw + dplcBw/2;
	};

	// for testing
	g_shiftFreq = shiftFreq;

	// initialize IIR filters
	BiIIR_Filter_init(&filterData, pCoef, filterStateI, filterStateQ, numOfSec);

	// initialize oscillator
	oscillator_init(OSC_AMPLITUDE, shiftFreq, INT_SAMPLE_FREQ, &osc);

}

#pragma optimize_as_cmd_line


/*****************************************************************************
  Function:		txSignalProcessingOfdm_activate()	
  Description:	Performs processing of OFDM tx signal.
  Inputs:		vIn/vOut: pointers to input/output sample arrays
  Return:		None
  Globals:		
  Important:	Call txSignalProcessingOfdm_init() before this function.

******************************************************************************/

void txSignalProcessingOfdm_activate(complex_float vIn[], complex_float vOut[])
{
	static float SEGMENT_DM_DATA2 vInSplit[BLOCK_LENGTH_C_IF];
	static float SEGMENT_DM_DATA2 vLpSplit[BLOCK_LENGTH_C_IF];
	static float SEGMENT_DM_DATA2 vOutSplit[BLOCK_LENGTH_C_IF];


	// split complex input vector into real and imaginary part
	vsplit_c_DDD(vInSplit, &vInSplit[N_SAMPLES_IF_C], vIn, N_SAMPLES_IF_C);

	// filtering
	BiIIR(vInSplit, vLpSplit, filterData.coefPtr, filterStateI, filterData.numOfSec, N_SAMPLES_IF_C);
	BiIIR(&vInSplit[N_SAMPLES_IF_C], &vLpSplit[N_SAMPLES_IF_C], filterData.coefPtr, filterStateQ, filterData.numOfSec, N_SAMPLES_IF_C);

	// shift signal
	freqShift(vLpSplit, vOutSplit, &osc, N_SAMPLES_IF_C);

	// combine real and imaginary part
	vcombine_c_DDD(vOut, vOutSplit, &vOutSplit[N_SAMPLES_IF_C], N_SAMPLES_IF_C);

	// adjust oscillator amplitude
	oscillator_AGC(&osc);
	
}
