/*-------------------------------------------------------------------------*
 * $Workfile: APLC_Types.h $
 * Part of      : ETL600 / APLC
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : APLC global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/APLC_Types.h $
 * 
 * 46    13.09.12 16:44 Chmaste
 * New APLC channel mode fixed
 * 
 * 45    12-09-11 11:48 Chhamaa
 * New APLC channel mode APLC_CHANNEL_MODE_A1_25 I/E  for 2.5 kHz TPE
 * added.
 * 
 * 43    6.06.08 15:54 Chsanoh
 * 3rd MAS implementation phase
 * 
 * 42    21.05.08 15:06 Chsanoh
 * 2nd MAS implementation phase
 * 
 * 41    9.05.08 11:37 Chsanoh
 * APLC macros adapted.
 * 
 * 40    5.05.08 14:24 Chhamaa
 * GET_APLC_BANDWIDTH() modified.
 * 
 * 39    30.04.08 10:17 Chhamaa
 * New APLC channel modes added for MOD600 above speech.
 * 
 * 33    24.05.05 18:34 Chsanoh
 * 
 * 32    28.04.05 16:26 Chpttra1
 * New constant BW2100 for AES Status was added
 * 
 * 31    9.03.05 13:34 Chhesch5
 * 
 * 30    9.03.05 11:12 Chhesch5
 * 
 * 29    2.02.05 17:45 Chutpra
 * Add APLC_BANDWITH
 * 
 * 28    1.02.05 17:33 Chutpra
 * add APLC_BANDWIDTHS
 * 
 * 27    15.12.04 17:22 Chhesch5
 * 
 * 26    30.11.04 11:46 Chsanoh
 * 
 * 25    29.11.04 11:50 Maag01
 * t_APLC_Status modified
 * 
 * 24    26.11.04 15:18 Maag01
 * t_PilotModemStatus modified
 * 
 * 23    23.11.04 17:07 Maag01
 * t_APLC_Status added
 * 
 * 22    19.11.04 9:08 Nohl01
 * 
 * 21    3.11.04 15:35 Nohl01
 * 
 * 20    3.11.04 10:04 Nohl01
 * 
 * 19    28.10.04 18:19 Maag01
 * APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I added
 * 
 * 18    8.10.04 9:04 Leeb02
 * APLC_CH_NONE added
 * 
 * 17    23.09.04 10:59 Maag01
 * 
 * 16    21.09.04 18:38 Nohl01
 * 
 * 15    20.09.04 18:16 Nohl01
 * 
 * 14    27.08.04 16:39 Nohl01
 * 
 * 13    26.08.04 15:19 Maag01
 * 
 * 12    6.08.04 17:28 Nohl01
 * 
 * 11    3.06.04 15:53 Maag01
 * 
 * 10    1.06.04 14:19 Nohl01
 * 
 * 9     24.05.04 17:19 Nohl01
 * 
 * 8     16.04.04 17:24 Nohl01
 * 
 * 7     8.04.04 17:30 Nohl01
 * 
 * 6     8.04.04 10:19 Nohl01
 * 
 * 5     7.04.04 10:58 Leeb02
 * 
 * 4     30.03.04 17:34 Nohl01
 * 
 * 3     29.03.04 16:53 Nohl01
 * None-bitmask variables of type UINT32 changed to INT32. Bitmask
 * variables commented.
 * 
 * 2     15.03.04 16:49 Maag01
 * 
 * 1     5.03.04 11:15 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef APLC_TYPES_H
#define APLC_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"equalizerTypes.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define	N_APLC_CHANNELS				3		// number of APLC channels
#define N_APLC_BANDWIDTHS           6       // max amount of BandWidths inside APLC channels
											// APLC bandwith [kHz]: {0, 2, 2.5, 4, 3.2, 2.4} 
#define	MIN_NO_APLC_CHANNELS		0		// Minimal number of APLC channels
#define	MAX_NO_APLC_CHANNELS		3		// Maximum number of APLC channels
#define	APLC_CH1					0		// index for APLC channel 1
#define	APLC_CH2					1		// index for APLC channel 2
#define	APLC_CH3					2		// index for APLC channel 3
#define	APLC_CH_NONE				0xFF   	// No APLC channel

#define	ETL600_PILOT_BANDWIDTH		500		// Bandwidth of ETL600 pilot in Hz

#define BW8000						8000	// Bandwidth 8000Hz	// used by HMI600 (also defined in konfo4le.h)
#define BW4000						4000	// Bandwidth 4000Hz	// used by HMI600 (also defined in konfo4le.h)
#define BW2000						2000	// Bandwidth 2000Hz	// (also defined in konfo4le.h)
#define BW2100						2100	// Bandwidth 2100Hz	// for AES Status
#define BW2500						2500	// Bandwidth 2500Hz	// (also defined in konfo4le.h)

// Defines for for "unsigned long apFilterFlags"
#define AP_FILTER_CH1_ON			BIT0	// bit 1: group delay equalizer ch1	// used by HMI600
#define AP_FILTER_CH1_OFF			~AP_FILTER_CH1_ON	// used by HMI600	
#define AP_FILTER_CH2_ON			BIT1		// bit 2: group delay equalizer ch2	// used by HMI600
#define AP_FILTER_CH2_OFF			~AP_FILTER_CH2_ON	// used by HMI600
#define AP_FILTER_CH3_ON			BIT2		// bit 3: group delay equalizer ch3	// used by HMI600
#define AP_FILTER_CH3_OFF			~AP_FILTER_CH3_ON	// used by HMI600

// Defines for "long channelMode"
// @NS@ Don't change define long values otherwise configuration files or configurations on targets
// dating from 02.11.04 or older cannot be correctly opened or uploaded anymore.
#define	APLC_CHANNEL_MODE_NONE					(long)  0	// No analog channels
#define	APLC_CHANNEL_MODE_A1_20I				(long)  1	// Ch1 2.0 kHz inverted
#define	APLC_CHANNEL_MODE_A1_40I				(long)  2	// Ch1 4.0 kHz inverted
#define	APLC_CHANNEL_MODE_A1_40I_A2_40I			(long)  3   // Ch1 4.0 kHz inverted, Ch2 4.0 kHz inverted
#define	APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I	(long)  4	// Ch1 4.0 kHz inverted, Ch2 4.0 kHz inverted,	
															// Ch3 4.0 kHz inverted		
#define	APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E	(long)  5	// Ch1 3.2 kHz inverted, Ch2 2.4 kHz erect,	
															// Ch3 2.4 kHz erect
#define	APLC_CHANNEL_MODE_A1_20E				(long)  6	// Ch1 2.0 kHz erect // not used by HMI600
#define APLC_CHANNEL_MODE_A1_40E 				(long)  7	// Ch1 4.0 kHz erect // not used by HMI600
#define	APLC_CHANNEL_MODE_A1_40I_A2_40E			(long)  8	// Ch1 4.0 kHz inverted, Ch2 4.0 kHz erect // not used by HMI600
#define	APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E	(long)  9	// Ch1 4.0 kHz inverted, Ch2 4.0 kHz erect,
															// Ch3 4.0 kHz erect // not used by HMI600
#define APLC_CHANNEL_MODE_A1_20E_MAS			(long)  10	// Ch1 2.0 kHz erect (used for MOD600 above speech)
#define APLC_CHANNEL_MODE_A1_40I_A2_20E			(long)  11  // Ch1 4.0 kHz inverted, Ch2 2.0 kHz erect (used for MOD600 above speech)
#define APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E	(long)  12  // Ch1 4.0 kHz inverted, Ch2 4.0 kHz inverted, 
															// Ch3 2.0 kHz erect (used for MOD600 above speech)
#define	APLC_CHANNEL_MODE_A1_25I				(long)  13	// Ch1 2.5 kHz inverted
#define	APLC_CHANNEL_MODE_A1_25E				(long)  14	// Ch1 2.5 kHz erect // not used by HMI600

// Bit maskes für PilotModemStatusType.general
//
#define	RTS_PILOT_SET			BIT0 
#define	RTS_PILOT_CLEAR			~RTS_PILOT_SET
#define	DCD_PILOT_SET			BIT1 
#define	DCD_PILOT_CLEAR			~DCD_PILOT_SET
#define	CHANNELSAREID_SET		BIT2 
#define	CHANNELSAREID_CLEAR		~CHANNELSAREID_SET
#define	OUTOFDATA_SET			BIT3 
#define	OUTOFDATA_CLEAR			~OUTOFDATA_SET
#define	REC_MW_SET				BIT4 
#define	REC_MW_CLEAR			~REC_MW_SET 
#define	FAST_MUTE_SET			BIT5 
#define	FAST_MUTE_CLEAR			~FAST_MUTE_SET
#define	SLOW_MUTE_SET			BIT6 
#define	SLOW_MUTE_CLEAR			~SLOW_MUTE_SET
#define	SNR_ALARM_SET			BIT7 
#define	SNR_ALARM_CLEAR			~SNR_ALARM_SET
#define	RX_PILOT_OK_SET			BIT8 
#define	RX_PILOT_OK_CLEAR		~RX_PILOT_OK_SET 
#define PILOT_LEVEL_ALARM_SET	BIT9
#define PILOT_LEVEL_ALARM_CLEAR ~PILOT_LEVEL_ALARM_SET
#define AGC_FAIL_SET			BIT10
#define AGC_FAIL_CLEAR			~AGC_FAIL_SET


// Defines for APLC status flags
#define	AFC_FAIL_SET			BIT0 
#define	AFC_FAIL_CLEAR			~AFC_FAIL_SET


/* GLOBAL TYPE DECLARATIONS ************************************************/

typedef struct {

	long 	frequency;					// tx and rx frequency [Hz]
	float	snrAlarmThresholdLin;		// [linear scale]
	float 	snrAlarmThreshold_dB; 		// [log scale]
	float	levelAlarmThresholdLin;		// [linear scale]
	float 	levelAlarmThreshold_dB;		// [log scale]
	float	pilotLevelLowThreshold;		// pilot rx-level low threshold (HMI600: AGC blocking level)
	float	weight;						// pilot weight

} t_PilotModemConfig;


typedef struct {

	float	frequency;
	float	levelLin;	// [linear scale]
	float	level_dB;	// [log scale]
	
} t_TestToneConfig;



typedef struct {

	t_PilotModemConfig	pilot[N_APLC_CHANNELS];							
	t_EqualizerConfig	equalizer[N_APLC_CHANNELS];
	//t_TestToneConfig	testTone[N_APLC_CHANNELS];
	float				rxAFLevelAdjust[N_APLC_CHANNELS];
	long				channelMode;						// constants APLC_CHANNEL_MODE_...
	unsigned long 		apFilterFlags;						// bitfield: group delay equalizer flags
															// for APLC channels
	// reserved for futur extensions
	long		Reserved1;
	long		Reserved2;
	long		Reserved3;
	long		Reserved4;	

} t_APLC_Config;



typedef struct { 

  	unsigned long general;			// bitfield: bit0..10 = rts|dcd|channelsAreId|outOfData|recMW|fastMute|slowMute|snrAlarm|PilotIsOk|pilotLevelAlarm|agcFail
  	float         rxLevel;			// rx pilot signal level
  	float         noiseLevel;		// rx pilot noise level
	float         snr;         		// snr pilot channel
  	float         rxGain;			// receiver gain factor
	
} t_PilotModemStatus; 


typedef struct {

	t_PilotModemStatus	pilot[N_APLC_CHANNELS];
	float 	            rxTestToneLevel[N_APLC_CHANNELS];
	float	            linkFreqOffset;
	unsigned long		statusFlags;

} t_APLC_Status;


/***************************************************************************************************
Macro:			CNT_APLC_CHANNELS(APLC_Config, iCntAPLC)
Description:	Counts the number of APLC channels on the basis of the member variable "channelMode"
				of the struct t_APLC_Config.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
				iCntAPLC: Integer variable to store the number of APLC channels.
Outputs:		iCntAPLC: Integer variable containing the number of APLC channels.
Return:			None
Globals:		None	
Important:		-
***************************************************************************************************/

#define CNT_APLC_CHANNELS(APLC_Config, iCntAPLC) \
switch(APLC_Config.channelMode) \
{ \
	case APLC_CHANNEL_MODE_A1_20E: \
	case APLC_CHANNEL_MODE_A1_20I: \
	case APLC_CHANNEL_MODE_A1_20E_MAS: \
	case APLC_CHANNEL_MODE_A1_25E: \
	case APLC_CHANNEL_MODE_A1_25I: \
	case APLC_CHANNEL_MODE_A1_40E: \
	case APLC_CHANNEL_MODE_A1_40I:					iCntAPLC = 1;	break; \
	case APLC_CHANNEL_MODE_A1_40I_A2_40E: \
	case APLC_CHANNEL_MODE_A1_40I_A2_40I: \
	case APLC_CHANNEL_MODE_A1_40I_A2_20E:			iCntAPLC = 2;	break; \
	case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E: \
	case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I: \
	case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E: \
	case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	iCntAPLC = 3;	break; \
	case APLC_CHANNEL_MODE_NONE: \
	default:										iCntAPLC = 0;	break; \
} \

/***************************************************************************************************
Macro:			GET_APLC_BANDWIDTH(APLC_Config, lAPLC_Bandwidth)
Description:	Gets the bandwidth in Hz of all available APLC channels on the basis of the member
				variable "channelMode" of the struct t_APLC_Config.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
				lAPLC_Bandwidth: Long variable to store the bandwith of all available APLC 
								 channels.
Outputs:		lAPLC_Bandwidth: Long variable containing the bandwith of all available APLC 
								 channels.
Return:			None
Globals:		None	
Important:		-
***************************************************************************************************/

#define GET_APLC_BANDWIDTH(APLC_Config, lAPLC_Bandwidth) \
switch(APLC_Config.channelMode) \
{ \
	case APLC_CHANNEL_MODE_A1_20E: \
	case APLC_CHANNEL_MODE_A1_20I: \
	case APLC_CHANNEL_MODE_A1_20E_MAS:				lAPLC_Bandwidth =  2000; break; \
	case APLC_CHANNEL_MODE_A1_25E: \
	case APLC_CHANNEL_MODE_A1_25I:					lAPLC_Bandwidth =  2500; break; \
	case APLC_CHANNEL_MODE_A1_40E: \
	case APLC_CHANNEL_MODE_A1_40I:					lAPLC_Bandwidth =  4000; break; \
	case APLC_CHANNEL_MODE_A1_40I_A2_20E:			lAPLC_Bandwidth =  6000; break; \
	case APLC_CHANNEL_MODE_A1_40I_A2_40E: \
	case APLC_CHANNEL_MODE_A1_40I_A2_40I:			lAPLC_Bandwidth =  8000; break; \
	case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	lAPLC_Bandwidth = 10000; break; \
	case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E: \
	case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:	lAPLC_Bandwidth = 12000; break; \
	case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:	lAPLC_Bandwidth =  8000; break; \
	case APLC_CHANNEL_MODE_NONE: \
	default:										lAPLC_Bandwidth =	  0; break; \
} \

#endif

/***************************************************************************************************
Macro:			GET_APLC_CHANNELS_BANDWIDTH(APLC_Config, lCH1_Bandwidth, lCH2_Bandwidth, 
											lCH3_Bandwidth)
Description:	Gets the bandwidth in Hz of each individual APLC channel on the basis of the member
				variable "channelMode" of the struct t_APLC_Config.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
				lCH1_Bandwidth: Long variable to store the bandwith of APLC channel 1
				lCH2_Bandwidth: Long variable to store the bandwith of APLC channel 2
				lCH3_Bandwidth: Long variable to store the bandwith of APLC channel 3
Outputs:		lCH1_Bandwidth: Long variable containing the bandwith of APLC channel 1
				lCH2_Bandwidth: Long variable containing the bandwith of APLC channel 2
				lCH3_Bandwidth: Long variable containing the bandwith of APLC channel 3
Return:			None
Globals:		None	
Important:		-
***************************************************************************************************/
#define GET_APLC_CHANNELS_BANDWIDTH(APLC_Config, lCH1_Bandwidth, lCH2_Bandwidth, lCH3_Bandwidth) \
switch(APLC_Config.channelMode) \
{ \
	case APLC_CHANNEL_MODE_NONE: 		/* No analog channels*/ \
	{ \
		lCH1_Bandwidth = 0; \
		lCH2_Bandwidth = 0; \
		lCH3_Bandwidth = 0; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_20I:		/* Ch1 2.0 kHz inverted*/ \
	{ \
		lCH1_Bandwidth = 2000; \
		lCH2_Bandwidth = 0; \
		lCH3_Bandwidth = 0; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_20E_MAS:	/* Ch1 2.0 kHz erect*/ \
	{ \
		lCH1_Bandwidth = 2000; \
		lCH2_Bandwidth = 0; \
		lCH3_Bandwidth = 0; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_25I:		/* Ch1 2.5 kHz inverted*/ \
	{ \
		lCH1_Bandwidth = 2500; \
		lCH2_Bandwidth = 0; \
		lCH3_Bandwidth = 0; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_40I:		/* Ch1 4.0 kHz inverted*/ \
	{ \
		lCH1_Bandwidth = 4000; \
		lCH2_Bandwidth = 0; \
		lCH3_Bandwidth = 0; \
\
		 break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_40I_A2_20E:			/* Ch1 4.0 kHz inverted, Ch2 2.0 kHz erect*/ \
	{ \
		lCH1_Bandwidth = 4000; \
		lCH2_Bandwidth = 2000; \
		lCH3_Bandwidth = 0; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_40I_A2_40I:			/* Ch1 4.0 kHz inverted, Ch2 4.0 kHz inverted*/ \
	{ \
		lCH1_Bandwidth = 4000; \
		lCH2_Bandwidth = 4000; \
		lCH3_Bandwidth = 0; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:	/* Ch1 3.2 kHz inverted, Ch2 2.4 kHz erect,*/ \
	{												/* Ch3 2.4 kHz erect*/ \
		lCH1_Bandwidth = 3200; \
		lCH2_Bandwidth = 2400; \
		lCH3_Bandwidth = 2400; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	/* Ch1 4.0 kHz inverted, Ch2 4.0 kHz inverted,*/ \
	{												/* Ch3 2.0 kHz erect*/ \
		lCH1_Bandwidth = 4000; \
		lCH2_Bandwidth = 4000; \
		lCH3_Bandwidth = 2000; \
\
		break; \
	} \
\
	case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:	/* Ch1 4.0 kHz inverted, Ch2 4.0 kHz inverted,*/ \
	{												/* Ch3 4.0 kHz inverted*/ \
		lCH1_Bandwidth = 4000; \
		lCH2_Bandwidth = 4000; \
		lCH3_Bandwidth = 4000; \
\
		break; \
	} \
\
	default:	break;								/* invalid case <tbd> log error*/ \
} \


/***************************************************************************************************
Macro:			IS_HSD(APLC_Config)
Description:	Determines whether the current APLC channel allocation is HSD.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if the current APLC channel allocation is
					  HSD, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_HSD(APLC_Config) \
((APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_20I) ? TRUE : FALSE) \


/***************************************************************************************************
Macro:			IS_HSD_25(APLC_Config)
Description:	Determines whether the current APLC channel allocation is HSD 2.5 kHz.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if the current APLC channel allocation is
					  HSD 2.5 kHz, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_HSD_25(APLC_Config) \
((APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_25I) ? TRUE : FALSE) \


/***************************************************************************************************
Macro:			IS_MAS_NARROW_DPLC(APLC_Config, OFDM_Config)
Description:	Determines whether the current APLC channel allocation is one of ModAboveSpeech and
				the DPLC bandwidth is equal to 2 kHz.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
				OFDM_Config: Variable of type t_OFDM_Config.
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if the current APLC channel allocation is one
					  of ModAboveSpeech and the DPLC bandwidth is equal to 2 kHz, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_MAS_NARROW_DPLC(APLC_Config, OFDM_Config) \
(((APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_20E_MAS) || \
  (APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_40I_A2_20E) || \
  (APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E)) && \
  (OFDM_Config.channelBW == 2000) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_MAS_BROAD_DPLC(APLC_Config, OFDM_Config)
Description:	Determines whether the current APLC channel allocation is one of ModAboveSpeech and
				the DPLC bandwidth is equal to 6 kHz.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
				OFDM_Config: Variable of type t_OFDM_Config.
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if the current APLC channel allocation is one
					  of ModAboveSpeech and the DPLC bandwidth is equal to 6 kHz, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_MAS_BROAD_DPLC(APLC_Config, OFDM_Config) \
(((APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_20E_MAS) || \
  (APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_40I_A2_20E) || \
  (APLC_Config.channelMode == APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E)) && \
  (OFDM_Config.channelBW == 6000) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_MAS(APLC_Config, OFDM_Config)
Description:	Determines whether the current APLC channel allocation is one of ModAboveSpeech.
Inputs:			APLC_Config: Variable of type t_APLC_Config.
				OFDM_Config: Variable of type t_OFDM_Config.
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if the current APLC channel allocation is one
					  of ModAboveSpeech, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_MAS(APLC_Config, OFDM_Config) \
((IS_MAS_NARROW_DPLC(APLC_Config, OFDM_Config) || IS_MAS_BROAD_DPLC(APLC_Config, OFDM_Config)) ? \
 TRUE : FALSE) \

