/*-------------------------------------------------------------------------*
 * $Workfile: Rx_Guard_NSD.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : P.Plüer
 * Remarks      :  
 * Purpose      : Rx-Guard Detector for NSD600
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSD_DSP/C/Rx_Guard_NSD.c $ 
 * 
 * 27    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 24    17.04.07 11:23 Chhamaa
 * Update from AES branch
 * 
 * 23    29.03.07 15:48 Chhamaa
 * Reset persistent command if AES or LP commands are active.
 * 
 * 22    14.11.06 13:40 Chpeplu
 * Function init_rx_guard_nsd() changed for custom mode
 * 
 * 21    10.10.06 13:46 Chhamaa
 * Comment changed.
 * 
 * 20    20.09.06 16:32 Chhamaa
 * Unused code removed
 * 
 * 19    19.09.06 10:42 Chhamaa
 * tangWindow_init() modified
 * 
 * 18    12.09.06 15:02 Chhamaa
 * Test code removed.
 * 
 * 17    12.09.06 14:32 Chhamaa
 * TANG window functions added.
 * 
 * 16    8.09.06 16:08 Chhamaa
 * Functions for rx guard signaling added.
 * 
 * 15    29.08.06 11:36 Chhamaa
 * 
 * 14    25.08.06 15:04 Chhamaa
 * 
 * 13    21.08.06 17:59 Chhamaa
 * Persistent command changed.
 * 
 * 12    15.08.06 9:25 Chhamaa
 * AES persistent command detector added.
 * 
 * 9     31.01.06 16:23 Chpeplu
 * Init function extended with new HSD mode
 * 
 * 7     20.09.05 17:17 Chpeplu
 * SNR from FE-DSP take over after a Rx command after a delay from 10
 * seconds!
 * 
 * 6     23.08.05 14:56 Chpeplu
 * hysteresis added for low and high rx level alarm for solving bug 184
 * 
 * 5     30.06.05 8:33 Chpeplu
 * Code walkthrough Pl
 * 
 * 4     22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 3     24.03.05 13:45 Chpeplu
 * Some changed made for variable guard level
 * 
 * 2     1.03.05 12:00 Chpeplu
 * ActualGuardLevel is now GuardRxSignal[0] for NSD Status
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 10    6.01.05 17:22 Chpeplu
 * Update NSD_Power_Speech_Guard.GuardPower only if no command is active
 * 
 * 9     3.12.04 13:24 Chpeplu
 * alarm update in NSD Status added
 * 
 * 8     2.12.04 13:32 Chpeplu
 * init_rx_guard_nsd() moved to extern memory and functins changed,
 * function RxGuardLevelMonitoring() changed for two channel operation
 * 
 * 7     1.12.04 17:23 Chpeplu
 * Update GuardPower added -> used for Unblocking function
 * 
 * 6     8.09.04 14:23 Plueer01
 * void init_rx_guard_nsd(t_NSD_DSP_Config *Config_NSD, long Channel) ->
 * config struct changed
 * 
 * 5     19.07.04 10:58 Plueer01
 * functions now usable for 2 channels
 * 
 * 4     8.07.04 11:17 Plueer01
 * Changes according new NSD-config struct 
 * 
 * 3     21.06.04 12:04 Plueer01
 * Inint value sin and cos for gaurd set correct
 * 
 * 2     21.06.04 9:52 Plueer01
 * 
 * 1     21.06.04 9:19 Plueer01
 * initial version
 * 
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include <math.h>
#include "basetype.h"
#include "NSD_Types.h"
#include "Rx_Guard_NSD.h"
#include "NSD_Detector.h"
#include "diag.h"
#include "NSD_Monitor.h"
#include "AES600.h"
#include "tdm_ext_types.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// Guardschwelle -6dB falls Guard da 
#define TH_GUARD_IS_PRESENT	0.251

// Guardschwelle -2dB falls Guard nicht da
// das ergibt eine Hysterese von 4dB, Pilot tastet um 3dB um + 1dB Reserve
#define TH_GUARD_NOT_PRESENT	0.631
//
#define MEAN_COEF_1				0.8
#define MEAN_COEF_2				0.2

#define	NUMB_OF_GUARD_STATES	2
//
#define	FILTER_STATE_TWO_SEC	4
#define FILTER_STATE_ONE_SEC	2
//
#define	FILTER_COEF_PER_SEC		4
#define	FILTER_SEC_LP_RX_GUARD	2

#define	FILTER_SEC_SMOOTHING	1
#define	SAMPLE_FOR_SMOOTHING	1

// define hysterese for reset rx level alarm low 0.4dB above alarm threshold
#define	RX_LEVEL_HYSTERESE_LOW	1.1

// define hysterese for reset rx level alarm high 0.4dB below alarm threshold
#define RX_LEVEL_HYSTERESE_HIGH 0.9


// defines for persistent command detection
//
#define NCORREC_CC			(1.0/INT_LENGHT_CC)

// 0.9 -> security < 1*10-5, dependability Pmc1% in 1.3*T0(24ms) = 9dB SNR first version till 1.4.2003!
// N=20 alpha = 0.84699 -> security = 1*10-4
// new alpha value for N=18 0.87981 ->  Puc=1.0*10-4
// N=22 alpha = 0.814434 Puc = 1.0*10-4, shift frequenzy = 48.48Hz
// N=32 alpha = 0.670492 Puc = 1.0*10-4, shift frequency = 33.33Hz
// N=32 alpha = 0.767644 Puc = 1.0*10-6, shift frequency = 33.33Hz
#define	TRIP_ALPHA			0.670492

// holdtime of persistent command depending on the evaluation time
// holdtime= INT_LENGHT_CC * HOLD_PERSISTENT
// new after 3rd feedback from Romanov from 1.4.2003 (old value 0.4)
// new after forth feedback from Romanov from 4.4.2003 (old value 1.5)
// new 1.0 with N=32
#define	HOLD_PERSISTENT		1.3

//TANG-Window length = 200ms
#define TANG_WINDOW		213

// for TANG-window special GUARD_ALPHA value with increased security
// N=32 alpha=0.767644 Puc=1*10-6
#define GUARD_ALPHA			0.767644

#define MSEC_TO_MAIN_LOOP_CYC_5K	5.33333		// 1 msec is 5.33 main loop cycles
#define MSEC_TO_MAIN_LOOP_CYC_1K	1.06667		// 1 msec is 1.07 main loop cycles


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {

	BOOL	RxGuardState;      				// guard state FALSE = no guard ; TRUE = guard present
	float	GuardMean;						// Mean of GuardRx	
	float   GuardRxSignal[NUMBER_OF_RX_SAMPLE];// Signal for GuardRx	
	float	SinGuard;
	float	CosGuard;
	float	SinDeltaGuard;
	float	CosDeltaGuard;
	long	RxGuardLevelAlarm;				// Alarm bit 
	long	Channel;						// channel 1 or 2 
	float	RxGuardAlarmThresholdLow;		// activ Rx level alarm threshold low
	float	RxGuardAlarmThresholdLowSet;	// configured rx level alarm threshold low 
	float	RxGuardAlarmThresholdLowReset;	// reset rx level alarm threshold low with hysterese 
	float	RxGuardAlarmThresholdHigh;		// activ Rx level alarm threshold high
	float	RxGuardAlarmThresholdHighSet;	// configured rx level alarm threshold high
	float	RxGuardAlarmThresholdHighReset;	// reset rx level alarm threshold high with hysterese

	BOOL  GuardPresent;						// true = guard present , false = guard not received
	float GrdThreshold[NUMB_OF_GUARD_STATES];
	float FilterOutI[FILTER_STATE_TWO_SEC];
	float FilterOutQ[FILTER_STATE_TWO_SEC];
	float dm FilterStateI[FILTER_STATE_TWO_SEC];		
	float dm FilterStateQ[FILTER_STATE_TWO_SEC];		
	float dm FilterStateTP2[FILTER_STATE_ONE_SEC];

} t_GuardStatus;


// persitent command detector
typedef struct {

	float SinOffset;
	float CosOffset;
	float SinDeltaG_CC;

	float CorrelInp[2];
	float Sum_MF1_0[INT_LENGHT_CC];
	float Sum_MF1_1[INT_LENGHT_CC];
	float Sum_PF1_0[INT_LENGHT_CC];
	float Sum_PF1_1[INT_LENGHT_CC];
	float Power_G[INT_LENGHT_CC];
	float Sum_Verif[5];
	float Sum_Out[4];
	float Guard_Power_WB;
	long  Actual_int;

	long  PersistentCommand;
	long  HoldtimeSet;
	long  Actual_Persistent;
	long  ActualHoldtime;

} t_PersCmdDetectorData;


// rx guard signaling
typedef enum {

	e_OutputOff,
	e_Pickup,
	e_OutputOn,
	e_Hold
}
e_StateRxGuardSignaling;

typedef struct {

	long pickupTime;
	long pickupCounter;
	long holdTime;
	long holdCounter;
	e_StateRxGuardSignaling state;

} t_RxGuardSignaling;


// TANG window
typedef enum {

	e_Idle,
	e_OnDelay,
	e_BlockCmdOutputs,
	e_OffDelay
}
e_StateTangWindow;

typedef struct {

	BOOL on;
	long onDelay;
	long onDelayCounter;
	long offDelay;
	long offDelayCounter;
	e_StateTangWindow state;

} t_TangWindow;


/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/

extern float sintab[];

static float SinGuard_Ch1, CosGuard_Ch1;
static float SinGuard_Ch2, CosGuard_Ch2;
static long	 BlockLength = 2;
static float pm  *ptrNsdTp1; 
static t_GuardStatus GuardStatusCh1, GuardStatusCh2;

t_NSD_Power_Levels *ptr1_NSD_Power_Speech_Guard;

// data struct for AES persistent command detection in channel 1
static t_PersCmdDetectorData persCmdDet;

// rx guard signaling data
static t_RxGuardSignaling rxGuardSignalingCh1;
static t_RxGuardSignaling rxGuardSignalingCh2;

// TANG window
static t_TangWindow tangWindow;

// Bandbegrenzungs Tiefpassfilter 150Hz
// siehe GuardRx5.m   
static float pm NSD_BandLimterTP_4KHZ[FILTER_COEF_PER_SEC*FILTER_SEC_LP_RX_GUARD+1] =
{   
	-9.022256e-001f, 
	 1.897507e+000f, 
	 1.000000e+000f, 
	-1.847896e+000f, 
	-9.688921e-001f, 
	 1.958891e+000f, 
	 1.000000e+000f, 
	-1.969154e+000f, 
	 GUARD_LEVEL_SQRT*9.773337e-003f
};

// siehe NSD_F6.m
static float pm NSD_Smoothing_TP[FILTER_COEF_PER_SEC*FILTER_SEC_SMOOTHING+1] =
{   
	-0.67700f,
	 1.56558f,
	 1.00000f,
	-1.02013f,
	 0.10736f
};


/* LOCAL FUNCTIONS ***********************************************************/

static BOOL GuardDet(float* RxInI, float* RxInQ, long ActivRxCommand,t_GuardStatus *GuardStatus, unsigned long NSD_Alarm, long Channel);
static void ShiftToZeroHerz(float *input, float *outputI, float *outputQ, t_GuardStatus *GuardStatus, long blockSize);

static void AES_persCmd_init(t_NSD_DSP_Config *config);
static void rxGuardSignaling_init(t_NSD_DSP_Config *config, long channel);
static void tangWindow_init(t_NSD_DSP_Config *config);


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		init_rx_guard_nsd()
  Description:	
  Inputs:		
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

*******************************************************************************/

void SEGMENT_EXT_CODE init_rx_guard_nsd(t_NSD_DSP_Config *Config_NSD)
{
	float	Nominal_GuardLevel;
	//
	// @Pl_ext
	long	TPE_Basic_Mode = 0;
	//
	TPE_Basic_Mode = (Config_NSD->mode & INT_TPE_MODE_MASK);

	// disable TANG window (only used in AES mode)
	tangWindow.on = FALSE;		

	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			
			//
			break;
		case INT_TELPROT_MODE_AES:
			//
			// init Ch1 data 
	 		GuardStatusCh1.SinDeltaGuard = sinf(TWO_PI*Config_NSD->AES.PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.CosDeltaGuard = cosf(TWO_PI*Config_NSD->AES.PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.SinGuard = 0.0;	// init value sine 
			GuardStatusCh1.CosGuard = 1.0;	// init value cos
			//
			GuardStatusCh1.GuardPresent = TRUE;
			GuardStatusCh1.GrdThreshold[0] = TH_GUARD_NOT_PRESENT;
			GuardStatusCh1.GrdThreshold[1] = TH_GUARD_IS_PRESENT;
			GuardStatusCh1.FilterStateI[0] = 0.0;
			GuardStatusCh1.FilterStateI[1] = 0.0;
			GuardStatusCh1.FilterStateI[2] = 0.0;
			GuardStatusCh1.FilterStateI[3] = 0.0;
			//
			GuardStatusCh1.FilterStateQ[0] = 0.0;
			GuardStatusCh1.FilterStateQ[1] = 0.0;
			GuardStatusCh1.FilterStateQ[2] = 0.0;
			GuardStatusCh1.FilterStateQ[3] = 0.0;
			//
			GuardStatusCh1.FilterStateTP2[0] = 0.0;
			GuardStatusCh1.FilterStateTP2[1] = 0.0;
			GuardStatusCh1.Channel = TPE1;
			//
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = Config_NSD->AES.GuardLevel* Config_NSD->AES.GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			// set low level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->AES.Guard_Alarm_Threshold * Nominal_GuardLevel;
			GuardStatusCh1.RxGuardAlarmThresholdLowSet = GuardStatusCh1.RxGuardAlarmThresholdLow;
			GuardStatusCh1.RxGuardAlarmThresholdLowReset = GuardStatusCh1.RxGuardAlarmThresholdLow * RX_LEVEL_HYSTERESE_LOW;
			// set high level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdHigh = (Nominal_GuardLevel/Config_NSD->AES.Guard_Alarm_Threshold);
			GuardStatusCh1.RxGuardAlarmThresholdHighSet = GuardStatusCh1.RxGuardAlarmThresholdHigh;
			GuardStatusCh1.RxGuardAlarmThresholdHighReset = GuardStatusCh1.RxGuardAlarmThresholdHigh * RX_LEVEL_HYSTERESE_HIGH;
			
			// initialization of persistent command detection
			AES_persCmd_init(Config_NSD);

			// initialization of TANG window
			tangWindow_init(Config_NSD);
			
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			// init Ch1 data 
			// set up for NSD600 in channel 1
			GuardStatusCh1.SinDeltaGuard = sinf(TWO_PI*Config_NSD->NSD[0].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.CosDeltaGuard = cosf(TWO_PI*Config_NSD->NSD[0].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.SinGuard = 0.0;	// init value sine 
			GuardStatusCh1.CosGuard = 1.0;	// init value cos
			//
			GuardStatusCh1.GuardPresent = TRUE;
			GuardStatusCh1.GrdThreshold[0] = TH_GUARD_NOT_PRESENT;
			GuardStatusCh1.GrdThreshold[1] = TH_GUARD_IS_PRESENT;
			GuardStatusCh1.FilterStateI[0] = 0.0;
			GuardStatusCh1.FilterStateI[1] = 0.0;
			GuardStatusCh1.FilterStateI[2] = 0.0;
			GuardStatusCh1.FilterStateI[3] = 0.0;
			//
			GuardStatusCh1.FilterStateQ[0] = 0.0;
			GuardStatusCh1.FilterStateQ[1] = 0.0;
			GuardStatusCh1.FilterStateQ[2] = 0.0;
			GuardStatusCh1.FilterStateQ[3] = 0.0;
			//
			GuardStatusCh1.FilterStateTP2[0] = 0.0;
			GuardStatusCh1.FilterStateTP2[1] = 0.0;
			GuardStatusCh1.Channel = TPE1;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = Config_NSD->NSD[0].GuardLevel* Config_NSD->NSD[0].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			// set low level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->NSD[0].Guard_Alarm_Threshold * Nominal_GuardLevel;
			GuardStatusCh1.RxGuardAlarmThresholdLowSet = GuardStatusCh1.RxGuardAlarmThresholdLow;
			GuardStatusCh1.RxGuardAlarmThresholdLowReset = GuardStatusCh1.RxGuardAlarmThresholdLow * RX_LEVEL_HYSTERESE_LOW;
			// set high level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdHigh = (Nominal_GuardLevel/Config_NSD->NSD[0].Guard_Alarm_Threshold);
			GuardStatusCh1.RxGuardAlarmThresholdHighSet = GuardStatusCh1.RxGuardAlarmThresholdHigh;
			GuardStatusCh1.RxGuardAlarmThresholdHighReset = GuardStatusCh1.RxGuardAlarmThresholdHigh * RX_LEVEL_HYSTERESE_HIGH;
			//
			//GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->NSD[0].Guard_Alarm_Threshold * NOMINAL_GUARD_LEVEL;
			//GuardStatusCh1.RxGuardAlarmThresholdHigh = (NOMINAL_GUARD_LEVEL/Config_NSD->NSD[0].Guard_Alarm_Threshold);
			
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// init Ch1 data 
			// set up for NSD600 in channel 1
			GuardStatusCh1.SinDeltaGuard = sinf(TWO_PI*Config_NSD->NSD[0].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.CosDeltaGuard = cosf(TWO_PI*Config_NSD->NSD[0].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.SinGuard = 0.0;	// init value sine 
			GuardStatusCh1.CosGuard = 1.0;	// init value cos
			//
			GuardStatusCh1.GuardPresent = TRUE;
			GuardStatusCh1.GrdThreshold[0] = TH_GUARD_NOT_PRESENT;
			GuardStatusCh1.GrdThreshold[1] = TH_GUARD_IS_PRESENT;
			GuardStatusCh1.FilterStateI[0] = 0.0;
			GuardStatusCh1.FilterStateI[1] = 0.0;
			GuardStatusCh1.FilterStateI[2] = 0.0;
			GuardStatusCh1.FilterStateI[3] = 0.0;
			//
			GuardStatusCh1.FilterStateQ[0] = 0.0;
			GuardStatusCh1.FilterStateQ[1] = 0.0;
			GuardStatusCh1.FilterStateQ[2] = 0.0;
			GuardStatusCh1.FilterStateQ[3] = 0.0;
			//
			GuardStatusCh1.FilterStateTP2[0] = 0.0;
			GuardStatusCh1.FilterStateTP2[1] = 0.0;
			GuardStatusCh1.Channel = TPE1;
			//
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = Config_NSD->NSD[0].GuardLevel* Config_NSD->NSD[0].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			// set low level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->NSD[0].Guard_Alarm_Threshold * Nominal_GuardLevel;
			GuardStatusCh1.RxGuardAlarmThresholdLowSet = GuardStatusCh1.RxGuardAlarmThresholdLow;
			GuardStatusCh1.RxGuardAlarmThresholdLowReset = GuardStatusCh1.RxGuardAlarmThresholdLow * RX_LEVEL_HYSTERESE_LOW;
			// set high level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdHigh = (Nominal_GuardLevel/Config_NSD->NSD[0].Guard_Alarm_Threshold);
			GuardStatusCh1.RxGuardAlarmThresholdHighSet = GuardStatusCh1.RxGuardAlarmThresholdHigh;
			GuardStatusCh1.RxGuardAlarmThresholdHighReset = GuardStatusCh1.RxGuardAlarmThresholdHigh * RX_LEVEL_HYSTERESE_HIGH;
			//
			//GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->NSD[0].Guard_Alarm_Threshold * NOMINAL_GUARD_LEVEL;
			//GuardStatusCh1.RxGuardAlarmThresholdHigh = (NOMINAL_GUARD_LEVEL/Config_NSD->NSD[0].Guard_Alarm_Threshold);
			//
			// set up for NSD600 in channel 2
			GuardStatusCh2.SinDeltaGuard = sinf(TWO_PI*Config_NSD->NSD[1].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh2.CosDeltaGuard = cosf(TWO_PI*Config_NSD->NSD[1].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh2.SinGuard = 0.0;	// init value sine 
			GuardStatusCh2.CosGuard = 1.0;	// init value cos
			//
			GuardStatusCh2.GuardPresent = TRUE;
			GuardStatusCh2.GrdThreshold[0] = TH_GUARD_NOT_PRESENT;
			GuardStatusCh2.GrdThreshold[1] = TH_GUARD_IS_PRESENT;
			GuardStatusCh2.FilterStateI[0] = 0.0;
			GuardStatusCh2.FilterStateI[1] = 0.0;
			GuardStatusCh2.FilterStateI[2] = 0.0;
			GuardStatusCh2.FilterStateI[3] = 0.0;
			//
			GuardStatusCh2.FilterStateQ[0] = 0.0;
			GuardStatusCh2.FilterStateQ[1] = 0.0;
			GuardStatusCh2.FilterStateQ[2] = 0.0;
			GuardStatusCh2.FilterStateQ[3] = 0.0;
			//
			GuardStatusCh2.FilterStateTP2[0] = 0.0;
			GuardStatusCh2.FilterStateTP2[1] = 0.0;
			GuardStatusCh2.Channel = TPE2;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = Config_NSD->NSD[1].GuardLevel* Config_NSD->NSD[1].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			// set low level alarm thresholds set, reset and activ threshold
			GuardStatusCh2.RxGuardAlarmThresholdLow = Config_NSD->NSD[1].Guard_Alarm_Threshold * Nominal_GuardLevel;
			GuardStatusCh2.RxGuardAlarmThresholdLowSet = GuardStatusCh2.RxGuardAlarmThresholdLow;
			GuardStatusCh2.RxGuardAlarmThresholdLowReset = GuardStatusCh2.RxGuardAlarmThresholdLow * RX_LEVEL_HYSTERESE_LOW;
			// set high level alarm thresholds set, reset and activ threshold
			GuardStatusCh2.RxGuardAlarmThresholdHigh = (Nominal_GuardLevel/Config_NSD->NSD[1].Guard_Alarm_Threshold);
			GuardStatusCh2.RxGuardAlarmThresholdHighSet = GuardStatusCh2.RxGuardAlarmThresholdHigh;
			GuardStatusCh2.RxGuardAlarmThresholdHighReset = GuardStatusCh2.RxGuardAlarmThresholdHigh * RX_LEVEL_HYSTERESE_HIGH;
			//
			//GuardStatusCh2.RxGuardAlarmThresholdLow = Config_NSD->NSD[1].Guard_Alarm_Threshold * NOMINAL_GUARD_LEVEL;
			//GuardStatusCh2.RxGuardAlarmThresholdHigh = (NOMINAL_GUARD_LEVEL/Config_NSD->NSD[1].Guard_Alarm_Threshold);
			//
			break;
		case INT_TELPROT_MODE_HSD:
			// init Ch1 data 
			// set up for NSD600 in channel 1
			GuardStatusCh1.SinDeltaGuard = sinf(TWO_PI*Config_NSD->NSD[0].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.CosDeltaGuard = cosf(TWO_PI*Config_NSD->NSD[0].PilotFrequency/AF_SAMPLE_FREQ);
			GuardStatusCh1.SinGuard = 0.0;	// init value sine 
			GuardStatusCh1.CosGuard = 1.0;	// init value cos
			//
			GuardStatusCh1.GuardPresent = TRUE;
			GuardStatusCh1.GrdThreshold[0] = TH_GUARD_NOT_PRESENT;
			GuardStatusCh1.GrdThreshold[1] = TH_GUARD_IS_PRESENT;
			GuardStatusCh1.FilterStateI[0] = 0.0;
			GuardStatusCh1.FilterStateI[1] = 0.0;
			GuardStatusCh1.FilterStateI[2] = 0.0;
			GuardStatusCh1.FilterStateI[3] = 0.0;
			//
			GuardStatusCh1.FilterStateQ[0] = 0.0;
			GuardStatusCh1.FilterStateQ[1] = 0.0;
			GuardStatusCh1.FilterStateQ[2] = 0.0;
			GuardStatusCh1.FilterStateQ[3] = 0.0;
			//
			GuardStatusCh1.FilterStateTP2[0] = 0.0;
			GuardStatusCh1.FilterStateTP2[1] = 0.0;
			GuardStatusCh1.Channel = TPE1;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = Config_NSD->NSD[0].GuardLevel* Config_NSD->NSD[0].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			// set low level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->NSD[0].Guard_Alarm_Threshold * Nominal_GuardLevel;
			GuardStatusCh1.RxGuardAlarmThresholdLowSet = GuardStatusCh1.RxGuardAlarmThresholdLow;
			GuardStatusCh1.RxGuardAlarmThresholdLowReset = GuardStatusCh1.RxGuardAlarmThresholdLow * RX_LEVEL_HYSTERESE_LOW;
			// set high level alarm thresholds set, reset and activ threshold
			GuardStatusCh1.RxGuardAlarmThresholdHigh = (Nominal_GuardLevel/Config_NSD->NSD[0].Guard_Alarm_Threshold);
			GuardStatusCh1.RxGuardAlarmThresholdHighSet = GuardStatusCh1.RxGuardAlarmThresholdHigh;
			GuardStatusCh1.RxGuardAlarmThresholdHighReset = GuardStatusCh1.RxGuardAlarmThresholdHigh * RX_LEVEL_HYSTERESE_HIGH;
			//
			//GuardStatusCh1.RxGuardAlarmThresholdLow = Config_NSD->NSD[0].Guard_Alarm_Threshold * NOMINAL_GUARD_LEVEL;
			//GuardStatusCh1.RxGuardAlarmThresholdHigh = (NOMINAL_GUARD_LEVEL/Config_NSD->NSD[0].Guard_Alarm_Threshold);
			//
			break;
		default:
			ASSERT(FALSE);
	}

	// initialization of rx guard signaling (on/off delay)
	rxGuardSignaling_init(Config_NSD, TPE1);
	rxGuardSignaling_init(Config_NSD, TPE2);

	// Common settings for Ch1 and Ch2
	// block length and low pass filter are the same for both NSD600 in channel 1 and channel 2
	BlockLength = 2;
	ptrNsdTp1   = NSD_BandLimterTP_4KHZ;
	//
	ptr1_NSD_Power_Speech_Guard = getPtr_NSD_Power_Speech_Guard();
}


/*****************************************************************************
  Function:		GuardDet()
  Description:	Filtering of the input signal for the I- and Q-signal, then calculate the square of I and Q signal
				and filter teh squared signal. Do the update the mean value only if no command is activ or trip and guard alarm is activ. 
				Update gaurd mean power and gaurd power for SNR and level monitoring functions
				Do the decision guard present or not present 
  Inputs:		
  Outputs:		None
  Return:		None
  Globals:		
  Important:	This function is called form the function rx_guard_nsd()

*******************************************************************************/

static BOOL GuardDet(float* RxInI, float* RxInQ, long ActivRxCommand, t_GuardStatus *GuardStatus, unsigned long NSD_Alarm, long Channel)
{	
	
	// Guard Filter: Bandbegrenzung 
	BiIIR(RxInI, GuardStatus->FilterOutI, ptrNsdTp1, GuardStatus->FilterStateI, FILTER_SEC_LP_RX_GUARD, BlockLength);  
	BiIIR(RxInQ, GuardStatus->FilterOutQ, ptrNsdTp1, GuardStatus->FilterStateQ, FILTER_SEC_LP_RX_GUARD, BlockLength);

	// save samples for persistent command detection in channel 1
	if (Channel == TPE1)
	{
		persCmdDet.CorrelInp[0] = GuardStatus->FilterOutI[0];
		persCmdDet.CorrelInp[1] = GuardStatus->FilterOutQ[0];
	}
		
	// Quadieren und summiern
	GuardStatus->GuardRxSignal[0] = (GuardStatus->FilterOutQ[0]*GuardStatus->FilterOutQ[0]) + (GuardStatus->FilterOutI[0]*GuardStatus->FilterOutI[0]);   
	
	// Filterung nach Quadrierung 
	BiIIR(GuardStatus->GuardRxSignal, GuardStatus->GuardRxSignal,NSD_Smoothing_TP, GuardStatus->FilterStateTP2, FILTER_SEC_SMOOTHING, SAMPLE_FOR_SMOOTHING);

	// nun nachregeln falls kein Rx-Befehl vorhanden
	if((ActivRxCommand == 0) || ((NSD_Alarm & NSD_TRIP_AND_GUARD_ALARM) == NSD_TRIP_AND_GUARD_ALARM))	
	{
		GuardStatus->GuardMean= 
			(GuardStatus->GuardMean<GuardStatus->GuardRxSignal[0])? ++(GuardStatus->GuardMean) :--(GuardStatus->GuardMean);		
		// update level guard for snr monitoring 
		ptr1_NSD_Power_Speech_Guard->GuardPower[GuardStatus->Channel] = GuardStatus->GuardRxSignal[0];
	}
	//
	// update power level Guard for monitoring functions
	//NSD_Power_Speech_Guard.GuardPower[GuardStatus->Channel] = GuardStatus->GuardRxSignal[0];
	ptr1_NSD_Power_Speech_Guard->GuardPower[GuardStatus->Channel] = GuardStatus->GuardRxSignal[0];

	ptr1_NSD_Power_Speech_Guard->GuardPowerMean[GuardStatus->Channel] = GuardStatus->GuardMean;
	
	// eventually replace GrdThreshold with a define so save time
	return GuardStatus->GuardPresent=(((GuardStatus->GuardMean)*GuardStatus->GrdThreshold[GuardStatus->GuardPresent])<GuardStatus->GuardRxSignal[0])? TRUE:FALSE;
 
}


/*****************************************************************************
  Function:		RxGuardLevelMonitoring(t_NSD_Status *ptr_NSD_Status, long Channel)
  Description:	Do the level monitoring of NSD guard for low and high level
  Inputs:		Channel number (1 or 2)
  Outputs:		Alarmword set in NSD Status and guard level update in NSD Status
  Return:		None
  Globals:		
  Important:	

*******************************************************************************/

void SEGMENT_EXT_CODE RxGuardLevelMonitoring(t_NSD_Status *ptr_NSD_Status, long Channel)
{
	t_GuardStatus *ptr_GuardStatus;
	//
	if(Channel == TPE1)
	{
		// set pointer for Channel 1
		ptr_GuardStatus = &GuardStatusCh1;
	}
	else
	{
		if(Channel == TPE2)
		{
			// set pointer for Channel 2
			ptr_GuardStatus = &GuardStatusCh2;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	// *****************************************************
	// *** check rx guard level high and low threshold *****
	// *****************************************************
	// Reset alarms 
	ptr_GuardStatus->RxGuardLevelAlarm &= ~(NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM);
	ptr_NSD_Status->NSD_Alarms &= ~(NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM);
	//
 	// check low level alarm
	//
 	if(ptr_GuardStatus->GuardMean < ptr_GuardStatus->RxGuardAlarmThresholdLow)
 	{
		//Mittelwert zu tief -> Alarm
 		ptr_GuardStatus->RxGuardLevelAlarm |= NSD_RX_GUARD_LOW_LEVEL_ALARM;
		ptr_NSD_Status->NSD_Alarms |= NSD_RX_GUARD_LOW_LEVEL_ALARM;
		// set reset threshold with hysterese
		ptr_GuardStatus->RxGuardAlarmThresholdLow = ptr_GuardStatus->RxGuardAlarmThresholdLowReset;
		// frezze guard mean level
		if(ptr_GuardStatus->GuardMean < ptr_GuardStatus->RxGuardAlarmThresholdLowSet) ptr_GuardStatus->GuardMean = ptr_GuardStatus->RxGuardAlarmThresholdLowSet;
 	}
	else
	{
		// low level i.o.
		// -> set nominal low level alarm threshold
		ptr_GuardStatus->RxGuardAlarmThresholdLow = ptr_GuardStatus->RxGuardAlarmThresholdLowSet;
	}
	//
	// Check high level alarm
 	// 
 	if(ptr_GuardStatus->GuardMean > ptr_GuardStatus->RxGuardAlarmThresholdHigh)
	{  
		// Mittelwert zu hoch -> Alarm
 		ptr_GuardStatus->RxGuardLevelAlarm |= NSD_RX_GUARD_HIGH_LEVEL_ALARM;
		ptr_NSD_Status->NSD_Alarms |= NSD_RX_GUARD_HIGH_LEVEL_ALARM;
		// set reset threshold with hysteresis
		ptr_GuardStatus->RxGuardAlarmThresholdHigh = ptr_GuardStatus->RxGuardAlarmThresholdHighReset;
		// frezze guard mean level 
		if(ptr_GuardStatus->GuardMean > ptr_GuardStatus->RxGuardAlarmThresholdHighSet) ptr_GuardStatus->GuardMean = ptr_GuardStatus->RxGuardAlarmThresholdHighSet;
 	}
 	else
 	{
 		// high level i.o.
		// -> set nominal high level alarm threshold
		ptr_GuardStatus->RxGuardAlarmThresholdHigh = ptr_GuardStatus->RxGuardAlarmThresholdHighSet;
 	}
	// update guard actual level
	ptr_NSD_Status->ActualGuardLevel = ptr_GuardStatus->GuardRxSignal[0];
	// 
}



/*****************************************************************************
  Function:		ShiftToZeroHerz()
  Description:	
  Inputs:		
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

*******************************************************************************/

static void ShiftToZeroHerz(float *input, float *outputI, float *outputQ, t_GuardStatus *GuardStatus, long blockSize)
{
	int i;
	register float Sin_Value_new, Cos_Value_new;
	register float SinDelta, CosDelta;
	register float SinValue, CosValue;


	SinDelta = GuardStatus->SinDeltaGuard;
	CosDelta = GuardStatus->CosDeltaGuard;
	//
	SinValue = GuardStatus->SinGuard;
	CosValue = GuardStatus->CosGuard;
	// 
	// Calculate sin and cos and shift to zero 
	//
  	for(i=0;i<blockSize;i++)
		{
			  Sin_Value_new = SinValue * CosDelta + CosValue * SinDelta;
			  Cos_Value_new = CosValue * CosDelta - SinValue * SinDelta;

			  SinValue = Sin_Value_new;
			  CosValue = Cos_Value_new;
			  //
			  outputI[i] = input[i] * SinValue;
			  outputQ[i] = input[i] * CosValue;
		}
	//
	GuardStatus->SinGuard = SinValue;
	GuardStatus->CosGuard = CosValue;
	//
}


/*****************************************************************************
  Function:		rx_guard_nsd()
  Description:	
  Inputs:		
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

*******************************************************************************/

BOOL rx_guard_nsd(float *RxSample, long Channel, long ActivRxCommand, unsigned long *ptr_NSD_Alarm)
{
	float SinShift[NUMBER_OF_RX_SAMPLE], CosShift[NUMBER_OF_RX_SAMPLE];
	t_GuardStatus *ptrGuardStatus;

	if (Channel == TPE1)
	{
		ptrGuardStatus = &GuardStatusCh1;
	}
	else
	{
		ptrGuardStatus = &GuardStatusCh2;
	}
	// Calculate the GuardDetector
	// Auslagern in eine Funktion benötigt 1% mehr rechenzeit 
	ShiftToZeroHerz(RxSample, SinShift, CosShift, ptrGuardStatus, BlockLength);
	
	return ptrGuardStatus->RxGuardState = GuardDet(SinShift, CosShift, ActivRxCommand, ptrGuardStatus, *ptr_NSD_Alarm, Channel); 
	
}


/*****************************************************************************
  Function:		RxGuardIFAmplCorr()
  Description:	
  Inputs:		
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

*******************************************************************************/

void RxGuardIFAmplCorr()
{
	float	f_sin, f_cos;
	
	// *******************************************************
	// *** do amplitude correction for RxGuardIFCh1 signal ***
	// *******************************************************
	//
	f_sin = GuardStatusCh1.SinGuard;
	f_cos = GuardStatusCh1.CosGuard;
	// correction algorithmus : Si' = Si*(3/2 - (Si^2 + Ci^2)/(2*A^2))
	//							Ci' = Ci*(3/2 - (Si^2 + Ci^2)/(2*A^2))
	GuardStatusCh1.SinGuard = f_sin*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
	GuardStatusCh1.CosGuard = f_cos*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
	//
	// *******************************************************
	// *** do amplitude correction for RxGuardIFCh2 signal ***
	// *******************************************************
	//
	f_sin = GuardStatusCh2.SinGuard;
	f_cos = GuardStatusCh2.CosGuard;
	// correction algorithmus : Si' = Si*(3/2 - (Si^2 + Ci^2)/(2*A^2))
	//							Ci' = Ci*(3/2 - (Si^2 + Ci^2)/(2*A^2))
	GuardStatusCh2.SinGuard = f_sin*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
	GuardStatusCh2.CosGuard = f_cos*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));

}


/*****************************************************************************
  Function:		void AES_persCmd_init()	
  Description:	Initialization of AES persitstent command detector data.
  Inputs:		pointer to NSD DSP configuration data		
  Outputs:		-		
  Return:		-		
  Globals:		struct persCmdDet
  Important:	This function has to be called once before using AES_persCmd_detect()

*******************************************************************************/

static void SEGMENT_EXT_CODE AES_persCmd_init(t_NSD_DSP_Config *config)
{
	int i;


	persCmdDet.SinOffset =  0.0;
	persCmdDet.CosOffset = 64.0;
	persCmdDet.SinDeltaG_CC = (256*(GUARD_FREQ_SHIFT)/(AF_SAMPLE_FREQ/10.0));

	// init integration sum with 0.0
	for (i = 0; i < INT_LENGHT_CC; i++)
	{
		persCmdDet.Sum_MF1_0[i] = 0.0;
		persCmdDet.Sum_MF1_1[i] = 0.0;
		persCmdDet.Sum_PF1_0[i] = 0.0;
		persCmdDet.Sum_PF1_1[i] = 0.0;
		persCmdDet.Power_G[i] = 0.0;
	}

	for(i = 0; i < 4; i++)
	{
		persCmdDet.Sum_Out[i] = 0.0;
		persCmdDet.Sum_Verif[i] = 0.0;
	}

	persCmdDet.Sum_Verif[4] = 0.0;

	persCmdDet.Guard_Power_WB = 0.0;

	// set persistent command according to NSD_Mode
	if (config->AES.AES600_Mode & AES600_PERSIST_ON_OFF)
	{
		persCmdDet.PersistentCommand = EXTENDED1_G4AI_TRIP_P_BIT;
	}
	else 
	{
		persCmdDet.PersistentCommand = 0;
	}

	persCmdDet.Actual_int = 0;
	persCmdDet.HoldtimeSet = (long)(INT_LENGHT_CC * HOLD_PERSISTENT);
	persCmdDet.Actual_Persistent = 0;
	persCmdDet.ActualHoldtime = 0;

}


/*****************************************************************************
  Function:		long AES_persCmd_detect(long rxCmdActive)	
  Description:	AES persistent command detection.
  Inputs:		active rx commands		
  Outputs:		-		
  Return:		If persistent cmd is received: pers. cmd bit set
				else: 0
  Globals:		struct persCmdDet		
  Important:	Before using this function call AES_persCmd_init() once.

*******************************************************************************/

long AES_persCmd_detect(long rxCmdActive)
{
	// calcualte sin and cos value
	register float SinValue, CosValue, Temp_Power_G;

	// static used for better performance
	static float Correl_x[2];
	static float Correl_y[2];
	static float Sum_Input[4];
	static float DetOut_CC;
	static float DetOut_G;
	static long  NoPersistent;
	  
	
    // Calculate the sinus and cosinus (Table method) 
	persCmdDet.SinOffset += persCmdDet.SinDeltaG_CC;
	persCmdDet.CosOffset += persCmdDet.SinDeltaG_CC;
	persCmdDet.SinOffset = (persCmdDet.SinOffset < 256.0) ? persCmdDet.SinOffset:persCmdDet.SinOffset-256.0; 
	persCmdDet.CosOffset = (persCmdDet.CosOffset < 256.0) ? persCmdDet.CosOffset:persCmdDet.CosOffset-256.0; 
	SinValue = *(sintab + (long) persCmdDet.SinOffset);
	CosValue = *(sintab + (long) persCmdDet.CosOffset); 

	Correl_x[0] = persCmdDet.CorrelInp[1] * CosValue;
	Correl_x[1] = persCmdDet.CorrelInp[1] * SinValue;

	Correl_y[0] = persCmdDet.CorrelInp[0] * SinValue;
	Correl_y[1] = persCmdDet.CorrelInp[0] * CosValue;
	
	Sum_Input[0] = (Correl_x[0] + Correl_y[0])*NCORREC_CC;
	Sum_Input[1] = (Correl_x[1] - Correl_y[1])*NCORREC_CC;

	Sum_Input[2] = (Correl_y[0] - Correl_x[0])*NCORREC_CC;
	Sum_Input[3] = (Correl_y[1] + Correl_x[1])*NCORREC_CC;

	Temp_Power_G = ((persCmdDet.CorrelInp[0]*persCmdDet.CorrelInp[0]) + (persCmdDet.CorrelInp[1]*persCmdDet.CorrelInp[1]))*NCORREC_CC;
	persCmdDet.Guard_Power_WB = persCmdDet.Guard_Power_WB - persCmdDet.Power_G[persCmdDet.Actual_int] + Temp_Power_G;
	persCmdDet.Power_G[persCmdDet.Actual_int] = Temp_Power_G;

	persCmdDet.Sum_Verif[0] = persCmdDet.Sum_Verif[0] + Temp_Power_G;
	persCmdDet.Sum_Verif[1] = persCmdDet.Sum_Verif[1] + Sum_Input[0];
	persCmdDet.Sum_Verif[2] = persCmdDet.Sum_Verif[2] + Sum_Input[1];
	persCmdDet.Sum_Verif[3] = persCmdDet.Sum_Verif[3] + Sum_Input[2];
	persCmdDet.Sum_Verif[4] = persCmdDet.Sum_Verif[4] + Sum_Input[3];

	// update output integration summe
	persCmdDet.Sum_Out[0] = persCmdDet.Sum_Out[0] - persCmdDet.Sum_MF1_0[persCmdDet.Actual_int] + Sum_Input[0];
	persCmdDet.Sum_Out[1] = persCmdDet.Sum_Out[1] - persCmdDet.Sum_MF1_1[persCmdDet.Actual_int] + Sum_Input[1];
	persCmdDet.Sum_Out[2] = persCmdDet.Sum_Out[2] - persCmdDet.Sum_PF1_0[persCmdDet.Actual_int] + Sum_Input[2];
	persCmdDet.Sum_Out[3] = persCmdDet.Sum_Out[3] - persCmdDet.Sum_PF1_1[persCmdDet.Actual_int] + Sum_Input[3];
	
	// update integration summe values
	persCmdDet.Sum_MF1_0[persCmdDet.Actual_int] = Sum_Input[0];
	persCmdDet.Sum_MF1_1[persCmdDet.Actual_int] = Sum_Input[1];
	persCmdDet.Sum_PF1_0[persCmdDet.Actual_int] = Sum_Input[2];
	persCmdDet.Sum_PF1_1[persCmdDet.Actual_int] = Sum_Input[3];

	persCmdDet.Actual_int++;
	if(persCmdDet.Actual_int >= INT_LENGHT_CC) 
	{
		persCmdDet.Actual_int = 0;

		// update integration sum with verfication sum
		persCmdDet.Guard_Power_WB = persCmdDet.Sum_Verif[0];
		persCmdDet.Sum_Out[0] = persCmdDet.Sum_Verif[1];
		persCmdDet.Sum_Out[1] = persCmdDet.Sum_Verif[2];
		persCmdDet.Sum_Out[2] = persCmdDet.Sum_Verif[3];
		persCmdDet.Sum_Out[3] = persCmdDet.Sum_Verif[4];

		persCmdDet.Sum_Verif[0] = 0.0;
		persCmdDet.Sum_Verif[1] = 0.0;
		persCmdDet.Sum_Verif[2] = 0.0;
		persCmdDet.Sum_Verif[3] = 0.0;
		persCmdDet.Sum_Verif[4] = 0.0;
	}

	DetOut_G = persCmdDet.Sum_Out[0]*persCmdDet.Sum_Out[0] + persCmdDet.Sum_Out[1]*persCmdDet.Sum_Out[1];
	DetOut_CC = persCmdDet.Sum_Out[2]*persCmdDet.Sum_Out[2] + persCmdDet.Sum_Out[3]*persCmdDet.Sum_Out[3];

	// decision if persistent-command or aes-guard is recieved
	if((DetOut_CC > (DetOut_G + TRIP_ALPHA*(persCmdDet.Guard_Power_WB - DetOut_G))) && (DetOut_G < (persCmdDet.Guard_Power_WB*(1 - TRIP_ALPHA)))) 
	{
		// persistent command is received
		persCmdDet.Actual_Persistent = persCmdDet.PersistentCommand;	
		persCmdDet.ActualHoldtime = persCmdDet.HoldtimeSet;
		NoPersistent = 0;
	}
	else
	{
		// persistent command is not received
		NoPersistent = 1;
		if (persCmdDet.ActualHoldtime > 0)
		{
			// hold last persistent command
			persCmdDet.ActualHoldtime--;
		}
		else
		{
			persCmdDet.Actual_Persistent = 0;
		}
	}

	// reset persistent command if AES or LP commands are active
	if (rxCmdActive)
	{
		persCmdDet.Actual_Persistent = 0;
		persCmdDet.ActualHoldtime = 0;
	}
	
	return(persCmdDet.Actual_Persistent);
}


/*****************************************************************************
  Function:		void rxGuardSignaling_init()	
  Description:	Initialization of AES Rx guard signaling function.
  Inputs:		pointer to NSD DSP configuration data, channel number		
  Outputs:		-		
  Return:		-		
  Globals:		struct rxGuardSignalingCh1, rxGuardSignalingCh2
  Important:	This function has to be called once before using 
				rxGuardSignaling_activate()

*******************************************************************************/

static void SEGMENT_EXT_CODE rxGuardSignaling_init(t_NSD_DSP_Config *config, long channel)
{
	t_RxGuardSignaling *ptrRxGuardSignaling;
	long pickupTime;
	long holdTime;


	if (channel == TPE1)
	{

		ptrRxGuardSignaling = &rxGuardSignalingCh1;
		// @Pl_ext
		if ((config->mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
		{
			pickupTime = config->AES.RxGuardOnOffDelay & RX_GUARD_ON_DELAY_MASK;
			holdTime = (config->AES.RxGuardOnOffDelay & RX_GUARD_OFF_DELAY_MASK) >> 16;
		}
		else
		{
			pickupTime = config->NSD[0].RxGuardOnOffDelay & RX_GUARD_ON_DELAY_MASK;
			holdTime = (config->NSD[0].RxGuardOnOffDelay & RX_GUARD_OFF_DELAY_MASK) >> 16;
		}
	}
	else
	{
		ptrRxGuardSignaling = &rxGuardSignalingCh2;
		// #Pl_ext
		if ((config->mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)	// AES only in ch1
		{
			pickupTime = 0;
			holdTime = 0;
		}
		else
		{
			pickupTime = config->NSD[1].RxGuardOnOffDelay & RX_GUARD_ON_DELAY_MASK;
			holdTime = (config->NSD[1].RxGuardOnOffDelay & RX_GUARD_OFF_DELAY_MASK) >> 16;
		}

	}

	ptrRxGuardSignaling->pickupTime = pickupTime*MSEC_TO_MAIN_LOOP_CYC_5K;	
	ptrRxGuardSignaling->holdTime = holdTime*MSEC_TO_MAIN_LOOP_CYC_5K;

	ptrRxGuardSignaling->state = e_OutputOff;
	ptrRxGuardSignaling->pickupCounter = 0;
	ptrRxGuardSignaling->holdCounter = 0;
}


/*****************************************************************************
  Function:		void rxGuardSignaling_activate()	
  Description:	The input signal is delayed and hold according to the parameters
				pickup and hold time.
  Inputs:		signal input, channel number		
  Outputs:		-		
  Return:		output signal		
  Globals:		struct rxGuardSignalingCh1, rxGuardSignalingCh1
  Important:	Before using this function call rxGuardSignaling_init() once.

*******************************************************************************/

BOOL rxGuardSignaling_activate(BOOL input, long channel)
{
	t_RxGuardSignaling *ptrRxGuardSignaling;
	BOOL output;


	// set pointer
	if (channel == TPE1)
	{
		ptrRxGuardSignaling = &rxGuardSignalingCh1;
	}
	else
	{
		ptrRxGuardSignaling = &rxGuardSignalingCh2;
	}

	// state machine
	switch(ptrRxGuardSignaling->state)
	{
		case e_OutputOff:	// idle state -> output off

			if (input)
			{
				if (ptrRxGuardSignaling->pickupTime > 0)
				{
					ptrRxGuardSignaling->pickupCounter = 0;
					ptrRxGuardSignaling->state = e_Pickup;
					output = FALSE;
				}
				else		// no pick up time
				{
					ptrRxGuardSignaling->state = e_OutputOn;
					output = TRUE;
				}

			}
			else 
			{
				output = FALSE;
			}
			
			break;

		case e_Pickup:		// state during pickup time
			
			if (input)
			{
				ptrRxGuardSignaling->pickupCounter++;

				if (ptrRxGuardSignaling->pickupCounter >= ptrRxGuardSignaling->pickupTime)
				{
					ptrRxGuardSignaling->state = e_OutputOn;
					output = TRUE;
				}
				else
				{
					output = FALSE;
				}
			}
			else
			{
				ptrRxGuardSignaling->state = e_OutputOff;
				output = FALSE;
			}
			
			break;
		
		case e_OutputOn:	// output active

			if (!input)
			{
				if (ptrRxGuardSignaling->holdTime > 0)
				{
					ptrRxGuardSignaling->holdCounter = 0;
					ptrRxGuardSignaling->state = e_Hold;
					output = TRUE;
				}
				else	// no hold time
				{
					ptrRxGuardSignaling->state = e_OutputOff;
					output = FALSE;
				}
			}
			else 
			{
				output = TRUE;
			}

			break;

		case e_Hold:	// state during hold time

			if (input)
			{
				ptrRxGuardSignaling->state = e_OutputOn;
				output = TRUE;
			}
			else
			{
				ptrRxGuardSignaling->holdCounter++;

				if (ptrRxGuardSignaling->holdCounter >= ptrRxGuardSignaling->holdTime)
				{
					ptrRxGuardSignaling->state = e_OutputOff;
					output = FALSE;
				}
				else
				{
					output = TRUE;
				}
			}
			
			break;

		default:
			
			ptrRxGuardSignaling->state = e_OutputOff;
			ptrRxGuardSignaling->pickupCounter = 0;
			ptrRxGuardSignaling->holdCounter = 0;
	}
	
	return(output);
}


/*****************************************************************************
  Function:		tangWindow_init()	
  Description:	Initialization of TANG window function.
  Inputs:		pointer to NSD DSP configuration data		
  Outputs:		-		
  Return:		-		
  Globals:		struct tangWindow
  Important:	This function has to be called once before using 
				tangWindow_activate()

*******************************************************************************/

static void SEGMENT_EXT_CODE tangWindow_init(t_NSD_DSP_Config *config)
{
	long onDelay;
	long offDelay;


	if (config->AES.AES600_Mode & AES600_TANG_ON_OFF)
	{
		tangWindow.on = TRUE;
	}
	else
	{
		tangWindow.on = FALSE;
	}

	onDelay = config->AES.TANG_DelayOnOff & TANG_DELAY_ON_MASK;
	offDelay = (config->AES.TANG_DelayOnOff & TANG_DELAY_OFF_MASK) >> 16;

	tangWindow.onDelay = onDelay*MSEC_TO_MAIN_LOOP_CYC_1K;
	tangWindow.offDelay = offDelay*MSEC_TO_MAIN_LOOP_CYC_1K;

	tangWindow.onDelayCounter = 0;
	tangWindow.offDelayCounter = 0;
	tangWindow.state = e_Idle;
}


/*****************************************************************************
  Function:		tangWindow_activate()	
  Description:	Activation of TANG window. Command outputs have to be blocked
				if the guard is not present and no command has been received
				for a predefiend time interval (on delay).
  Inputs:		rxGuardOn, rxCmd: rx command word		
  Outputs:		-		
  Return:		blockCmdOutputs		
  Globals:		struct tangWindow
  Important:	Call tangWindow_init() once before using this function.

*******************************************************************************/

BOOL SEGMENT_EXT_CODE tangWindow_activate(BOOL rxGuardOn, long rxCmd)
{
	BOOL blockCmdOutputs;


	if (tangWindow.on)		// TANG window enabled
	{
		// state machine
		switch(tangWindow.state)
		{
			case e_Idle:	// idle state

				if ((!rxGuardOn) && (rxCmd == 0))
				{
					tangWindow.onDelayCounter = 0;
					tangWindow.state = e_OnDelay;
				}
				blockCmdOutputs = FALSE;	

				break;

			case e_OnDelay:		// on delay state

				if ((!rxGuardOn) && (rxCmd == 0))
				{
					tangWindow.onDelayCounter++;

					if (tangWindow.onDelayCounter >= tangWindow.onDelay)
					{
						tangWindow.state = e_BlockCmdOutputs;
						blockCmdOutputs = TRUE;
					}
					else
					{
						blockCmdOutputs = FALSE;
					}
				}
				else
				{
					tangWindow.state = e_Idle;
					blockCmdOutputs = FALSE;
				}

				break;

			case e_BlockCmdOutputs:		// block command outputs

				if (rxGuardOn)
				{
					if (tangWindow.offDelay > 0)
					{
						tangWindow.offDelayCounter = 0;
						tangWindow.state = e_OffDelay;
						blockCmdOutputs = TRUE;
					}
					else	// no off delay
					{
						tangWindow.state = e_Idle;
						blockCmdOutputs = FALSE;
					}
				}
				else
				{
					blockCmdOutputs = TRUE;
				}

				break;

			case e_OffDelay:	// off delay state

				if (rxGuardOn)
				{
					tangWindow.offDelayCounter++;

					if (tangWindow.offDelayCounter >= tangWindow.offDelay)
					{
						tangWindow.state = e_Idle;
						blockCmdOutputs = FALSE;
					}
					else
					{
						blockCmdOutputs = TRUE;
					}
				}
				else
				{
					tangWindow.state = e_BlockCmdOutputs;
					blockCmdOutputs = TRUE;
				}

				break;

			default:

				tangWindow.onDelayCounter = 0;
				tangWindow.offDelayCounter = 0;
				tangWindow.state = e_Idle;

		}
	}
	else	// TANG window disabled
	{
		blockCmdOutputs = FALSE;
	}
	
	return(blockCmdOutputs);
}
