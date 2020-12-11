/*-------------------------------------------------------------------------*
 * $Workfile: OFDM_DSP_Types.h $
 * Part of      : ETL600 / OFDM
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : OFDM global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/P4LT/Master_DSP/H/OFDM_DSP_Types.h $
 * 
 * 9     23.07.08 10:50 Chhamaa
 * FB/FF parameters added.
 * 
 * 8     7.05.08 17:07 Chhamaa
 * Phase control parameter added.
 * 
 * 2     16.03.05 10:25 Chchlee
 * leftPilotPresent added
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 9     29.09.04 12:04 Maag01
 * 
 * 7     13.09.04 14:09 Leeb02
 * 
 * 6     26.08.04 16:45 Maag01
 * 
 * 5     20.04.04 15:48 Maag01
 * 
 * 4     16.04.04 18:05 Maag01
 * 
 * 3     7.04.04 11:00 Leeb02
 * 
 * 2     04-03-15 7:44 Leeb02
 * Terminology harmonized
 * 
 * 1     04-03-12 17:30 Leeb02
 * Initial version
 *-------------------------------------------------------------------------*/
 
#ifndef OFDM_DSP_TYPES_H
#define OFDM_DSP_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"ofdm_types.h"

/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define OFDM_SPECTRUM_POS_0HZ		0
#define OFDM_SPECTRUM_POS_4000HZ	1
#define OFDM_SPECTRUM_POS_8000HZ	2
#define OFDM_SPECTRUM_POS_12000HZ	3

// fall back/forward parameter
#define BURST_ERROR_WINDOW_SIZE		31
#define FALL_BACK_WINDOW_SIZE		1000
#define BURST_ERROR_PROB_LIMIT		0.8
#define FALL_BACK_PROB_LIMIT		0.1
#define FALL_FORWARD_SIGMA_LIMIT	15

// number of phase control parameters (for testing)
#define N_PH_CTRL_PARAM				4


/* GLOBAL TYPE DECLARATIONS ************************************************/

typedef struct {

	unsigned long   mode;						// OFDM_CFG_MODE bits, see OFDM_TYPES.H							
	unsigned long   channelBW;					// channel bandwidth [Hz]
	unsigned long   dataRate[N_DATA_RATES];		// [bps], [0]-[4]: aggregate datarate
	unsigned long   spectrumPosition;			// 0 = 0Hz, 1 = 4000Hz, 2 = 8000Hz, 3 = 12000Hz
	BOOL            leftPilotPresent;

	unsigned long	burstErrorWindowSize;		// fall back parameter
	unsigned long	fallBackWindowSize;			// fall back parameter
	float			burstErrorProbLimit;		// fall back parameter
	float			fallBackProbLimit;			// fall back parameter
	float			fallForwardSigmaLimit;		// fall forward parameter

	float			phaseControlParam[N_PH_CTRL_PARAM];	// for testing
	
} t_OFDM_DSP_Config;


#endif
