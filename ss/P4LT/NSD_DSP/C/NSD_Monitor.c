/*----------------------------------------------------------------------------*
 * $Workfile: NSD_Monitor.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : P.Plüer
 * Remarks      : 
 * Purpose      : NSD600 Monitor Functions  
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSD_DSP/C/NSD_Monitor.c $ 
 * 
 * 34    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 31    29.10.07 16:24 Chtognu
 * Update from AES branch: Init_Monitor_Trip_Guard() changed variable
 * initialisation to origin (checked in 5.09.07)
 * 
 * 30    28.08.07 9:09 Chtognu
 * Init_Monitor_Trip_Guard() initialization for AES mode changed for
 * correct Trip and Guard monitoring. Additional "Detector active check"
 * added for AES in Monitor_Trip_Guard()
 * 
 * 29    17.04.07 11:20 Chhamaa
 * Update from AES branch
 * 
 * 29    2.04.07 17:00 Chhamaa
 * Scaling of Unbl_ThresSpeech for AES modified.
 * 
 * 28    21.03.07 10:28 Chhamaa
 * AES SNR alarm thresholds added.
 * 
 * 27    5.12.06 14:11 Chpeplu
 * Function Init_SNR_Monitoring_Guard() corrected for AES SNR alarm
 * threshold; new define AES_TRIP_GUARD_CORRECTION added
 * 
 * 26    14.11.06 13:40 Chpeplu
 * Function Init_NSDUnblocking() changed for custom mode, Function
 * Init_Monitor_Trip_Guard() changed for custom mode, Function
 * Init_SNR_Monitoring_Guard() changed for custom mode, Function
 * Init_RxCommandDurationMonit() changed for custom mode
 * 
 * 25    8.11.06 14:35 Chhamaa
 * resetRxCmdMask added to struct Monitor_Trip_Guard_Type. AES Rx command
 * is not reset in case of trip and guard.
 * 
 * 24    20.09.06 16:26 Chhamaa
 * Defines for AES SNR threshold added.
 * 
 * 23    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 23    7.07.06 18:50 Chhamaa
 * 
 * 20    8.02.06 15:03 Chpeplu
 * New defines for SNR-Alarm thresholds and SNR hysterese for HSD600 added
 * 
 * 19    31.01.06 16:24 Chpeplu
 * All init function (SNR-, Unblocking, TripandGuard) with new Mode HSD
 * extended
 * 
 * 17    2.11.05 17:11 Chpeplu
 * SNR alarm thresholds set for blocking permissive and direct to 7dB, and
 * hysterese set to 3dB
 * 
 * 16    28.10.05 8:11 Chchlee
 * some ASSERTs removed
 * 
 * 15    20.09.05 17:19 Chpeplu
 * Level and SNR monitoring also activ if trip and guard is received
 * 
 * 14    30.06.05 8:34 Chpeplu
 * Code walkthrough Pl
 * 
 * 13    22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 12    21.06.05 14:13 Chpeplu
 * New calculation of SNR_Correction_Trip_to_Guard for SNR aalarm
 * threshold added
 * 
 * 11    17.06.05 15:00 Chpeplu
 * SNR-level correction for Guard weight >= 1.0 corrected
 * 
 * 10    16.06.05 8:26 Chpeplu
 * 
 * 9     14.06.05 15:25 Chpeplu
 * SNR-Alarm threshold changed, scaling of SNR changed, SNR-status in case
 * off NSD set to 1.0 -> 0dB 
 * 
 * 8     27.05.05 14:06 Chpeplu
 * Some comments removed, in function Monitor_Trip_Guard() counter removed
 * 
 * 7     24.05.05 14:34 Chpeplu
 * some function optimized and use of #pragma optimize_off
 * 
 * 6     14.04.05 12:41 Chpeplu
 * in struct t-SNR_Monitoring_Type new member SNR_AlarmThreshold added
 * 
 * 5     24.03.05 13:46 Chpeplu
 * Some changed made for variable guard level, and some minor bugs fixed
 * 
 * 4     9.03.05 10:28 Chpeplu
 * New config used (NsdMode)
 * 
 * 3     4.03.05 15:23 Chpeplu
 * Prepared for NewConfigNSD without custom table
 * 
 * 2     1.03.05 12:02 Chpeplu
 * SNR-Alarm Thresholds corrected by 3dB  paramter for function
 * SNR_Monitoring_Guard() changed conditon for next unblocking changed ->
 * only if trip or guard received next unblocking can be set again time
 * correction for Unblocking times added ONE_MSEC_MAINCASE
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 14    17.01.05 15:52 Chpeplu
 * SNR monitoring improved for etl guard (get snr value from FE BW=134Hz)
 * and make bandwidth correction to NSD own guard bandwidth (BW =480Hz)
 * 
 * 13    14.01.05 17:28 Chpeplu
 * New SNR Alarm Thresholds set
 * 
 * 12    11.01.05 11:42 Chpeplu
 * new functions for Rx command duration monitoring added
 * 
 * 11    6.01.05 17:23 Chpeplu
 * Some modification for SNR calulation done -> not yet finished
 * 
 * 10    3.01.05 16:59 Chpeplu
 * SNR-Alarm set and reset implemented
 * 
 * 9     23.12.04 15:20 Chpeplu
 * SNR monitoring added
 * 
 * 8     15.12.04 17:31 Chpeplu
 * defiens for ON and OFF removed (now in NSD_DSP_Types.h)
 * 
 * 7     7.12.04 11:53 Chpeplu
 * New function Update_Dev_Status_Guard() added
 * 
 * 6     3.12.04 13:24 Chpeplu
 * Alarm update in NSD_Status added
 * 
 * 5     2.12.04 13:34 Chpeplu
 * New define NOMINAL_GUARD_LEVEL used for Unblocking level
 * 
 * 4     1.12.04 17:25 Chpeplu
 * Unblocking function added and function MonitoringtripGuard() improved
 * 
 * 3     29.11.04 17:41 Plueer01
 * Function Monitor_Trip_Guard() and init functions improved and defines
 * added
 * 
 * 2     17.11.04 16:49 Plueer01
 * second version -> NSD project is compilable now
 * 
 * 1     17.11.04 16:37 Plueer01
 * Initial version 
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "NSD_Monitor.h"
#include "NSD_DSP_Types.h"
#include "NSD_DSP_Main.h"
#include "Rx_Guard_NSD.h"
#include "AES600.h"
#include "diag.h"
//
//
/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef struct {
	BOOL	Channel_activ;
	long	BlockTimeCounter;
	long	Unbl_Status;
	long	Unbl_SetDelayCounter;
	long	UnblockDelayCounter;
	long	UnblockHoldCounter;
	long	Unbl_SetHoldCounter;
	long	Unbl_Flag;
	float	Unbl_ThresSpeech;
	float	Unbl_ThresGuard;
	long	Unbl_Counter;					// for tests only
} Unblocking_State_Type;
//
//
typedef struct {
	long			Delay;
	unsigned long	Trip_and_Guard_Alarm;
	unsigned long	No_Trip_and_Guard_Alarm;
	unsigned long	resetRxCmdMask;
	long			Timer_No_T_G;					// timer no trip and guard 
	long			Timer_T_G;						// timer trip and guard present not used
	BOOL			Channel_activ;					// 
} Monitor_Trip_Guard_Type;
//
//
typedef struct {
	BOOL			Channel_active;
	long			NSD_ETL_Guard_Config;			// 0 = ETL guard, 1 = NSD guard
	float			GuardEnvelope;
	float			meanGuardEnvelope;
	float			meanGuardEnvelope_I;
	float			meanGuardEnvelope_Q;
	float			Variance;
	float			Variance_I;
	float			Variance_Q;
	float			NoiseLevel_I;
	float			NoiseLevel_Q;
	float			NoiseLevel;
	float			Signal_Level;
	float			SNR_AlarmThreshold;				//  SNR-Threshold for HMI
	float			SNR_AlarmThreshold_Set;			//	SNR-Threshold for set SNR-alarm
	float			SNR_AlarmThreshold_Reset;		//  SNR-Threshold for reset SNR-alarm
	unsigned long	SNR_Alarm;						//  SNR-Alarm bit 
	float			Actual_SNR;						//  actual measured SNR
	long			SNR_Delay_Time_after_rx_Cmd;	//  delay time for SNR from FE DSP to suppress SNR alarm after a long rx command
} t_SNR_Monitoring_Type;
//
//
typedef struct {
	BOOL			Channel_Active;
	unsigned long	RxDurationAlarm;
	long			RxDurationTimeConfig[MAX_NUMBER_LP_COMMANDS];
	long			TripDurationCounter[MAX_NUMBER_LP_COMMANDS];
	unsigned long	RxDurationMonit_On_Off[MAX_NUMBER_LP_COMMANDS];

} t_Monit_RxTripDuration_Type;
//
/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// 
// defines for Unblocking
// start up blocking time for Unblocking commands 10sec
#define	BLOCK_TIME_UNBLOCKING	10000
#define UNBLOCKING_BIT			0x00000001
// states for Unblocking state machine
//
#define	UNBLO_IDLE	0x0007
#define	UNBLO_DELAY	0x0009
#define UNBLO_HOLD	0x000c
#define	UNBLO_WAIT	0x000d
//
// defines for Trip Guard monitoring

// defines for SNR Monitoring
#define GUARD_COEF_1						0.985         // guard level low pass filter constant  0.975
#define GUARD_COEF_2						0.015         // guard level low pass filter constant
// snr low pass filter constant
#define SNR_COEF_1							0.999        
// snr low pass filter constant
#define SNR_COEF_2            				0.001         
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!! Attention !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Change of SNR Threshold:
// !!!!!!!!!!!!!!!!!!!!!!!!!!
// !!! In the code the assumption is made, that the		Pmc1% blocking < Pmc1% permissive < Pmc1% direct !!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// SNR-Alarm thresholds for NSD600 
// -> values must be set according dependability Pmc 1% +3dB according to TRS NSD600
// -> Thresholds set to 7dB make and 10dB reset for all applications blocking, permissive, direct (2.11.2005/Pl)
// 
// Threshold = 10^((SNR_Thres +9.2dB -3dB)/10)
// These SNR-Levels are referenced to 4kHz noise bandwidth
#define	NOISE_BANDWIDTH_DEPENDABILITY		4000.0
// noise bandwidth of guard filter
#define NOISE_BANDWIDTH_GUARD				480.0
//
#define SNR_BANDWIDTH_CORRECTION			(NOISE_BANDWIDTH_DEPENDABILITY/NOISE_BANDWIDTH_GUARD)
// SNR-Threshold =7dB default blocking Pmc1% = 4.0dB
// Threshold = 10^((SNR_Thres_Pmc1% +3dB)/10)
#define SNR_ALARM_THRESHOLD_BLOCKING_PMC1	5.012
#define SNR_ALARM_THRESHOLD_BLOCKING		(SNR_ALARM_THRESHOLD_BLOCKING_PMC1 * SNR_BANDWIDTH_CORRECTION)
// SNR-Threshold =7dB	default permissive  Pmc1% = 4.0dB
// Threshold = 10^((SNR_Thres_Pmc1% +3dB)/10)
#define SNR_ALARM_THRESHOLD_PERMISSIVE_PMC1	5.012
#define SNR_ALARM_THRESHOLD_PERMISSIVE		(SNR_ALARM_THRESHOLD_PERMISSIVE_PMC1 * SNR_BANDWIDTH_CORRECTION)
// SNR-Threshold = 7dB default direct Pmc1% = 4dB
// Threshold = 10^((SNR_Thres_Pmc1% +3dB)/10)	
#define SNR_ALARM_THRESHOLD_DIRECT_PMC1		5.012
#define SNR_ALARM_THRESHOLD_DIRECT			(SNR_ALARM_THRESHOLD_DIRECT_PMC1 * 	SNR_BANDWIDTH_CORRECTION)	
// Hysterese for SNR alarm threshold set to 3dB
#define HYSTERESE_SNR_ALARM				2.0
//
// HSD600 snr alarm thresholds and hysterese 
//
// -> now set to the same value as NSD600 ->   7dB set / 10dB reset
//
#define HSD_SNR_ALARM_THRESHOLD_BLOCKING_PMC1	5.012
#define HSD_SNR_ALARM_THRESHOLD_BLOCKING		(HSD_SNR_ALARM_THRESHOLD_BLOCKING_PMC1 * SNR_BANDWIDTH_CORRECTION)
// SNR-Threshold =7dB	default permissive  Pmc1% = 4.0dB
// Threshold = 10^((SNR_Thres_Pmc1% +3dB)/10)
#define HSD_SNR_ALARM_THRESHOLD_PERMISSIVE_PMC1	5.012
#define HSD_SNR_ALARM_THRESHOLD_PERMISSIVE		(HSD_SNR_ALARM_THRESHOLD_PERMISSIVE_PMC1 * SNR_BANDWIDTH_CORRECTION)
// SNR-Threshold = 7dB default direct Pmc1% = 4dB
// Threshold = 10^((SNR_Thres_Pmc1% +3dB)/10)	
#define HSD_SNR_ALARM_THRESHOLD_DIRECT_PMC1		5.012
#define HSD_SNR_ALARM_THRESHOLD_DIRECT			(HSD_SNR_ALARM_THRESHOLD_DIRECT_PMC1 * 	SNR_BANDWIDTH_CORRECTION)	
// Hysterese for SNR alarm threshold set to 3dB
#define HSD_HYSTERESE_SNR_ALARM				2.0

// AES600 snr alarm thresholds and hysterese 
//
// T01, high security
// SNR-Threshold = 12 dB
#define AES_SNR_ALARM_THRESHOLD_T01_HIGH_SECURITY_BW_4K	15.849	//linear, BW 4 kHz
#define AES_SNR_ALARM_THRESHOLD_T01_HIGH_SECURITY		(AES_SNR_ALARM_THRESHOLD_T01_HIGH_SECURITY_BW_4K * SNR_BANDWIDTH_CORRECTION)

// T01, low security
// SNR-Threshold = 9 dB
#define AES_SNR_ALARM_THRESHOLD_T01_LOW_SECURITY_BW_4K	7.943	//linear, BW 4 kHz
#define AES_SNR_ALARM_THRESHOLD_T01_LOW_SECURITY		(AES_SNR_ALARM_THRESHOLD_T01_LOW_SECURITY_BW_4K * SNR_BANDWIDTH_CORRECTION)
	
// T02, high security
// SNR-Threshold = 9 dB
#define AES_SNR_ALARM_THRESHOLD_T02_HIGH_SECURITY_BW_4K	7.943	//linear, BW 4 kHz
#define AES_SNR_ALARM_THRESHOLD_T02_HIGH_SECURITY		(AES_SNR_ALARM_THRESHOLD_T02_HIGH_SECURITY_BW_4K * SNR_BANDWIDTH_CORRECTION)

// T01, low security
// SNR-Threshold = 6 dB
#define AES_SNR_ALARM_THRESHOLD_T02_LOW_SECURITY_BW_4K	3.981	//linear, BW 4 kHz
#define AES_SNR_ALARM_THRESHOLD_T02_LOW_SECURITY		(AES_SNR_ALARM_THRESHOLD_T02_LOW_SECURITY_BW_4K * SNR_BANDWIDTH_CORRECTION)

// Hysterese for SNR alarm threshold set to 3dB
#define AES_HYSTERESE_SNR_ALARM				2.0
//
#define AES_TRIP_GUARD_CORRECTION			1.0
//
#define AES_SPEECH_UNBL_TH_CORRECTION		0.5		// AES wideband power calculation is different to NSD

// trip to guard power level difference with 0dB Boost
#define	TRIP_GUARD_POWER_0DB			2
// SNR-Alarm Thresholds defines for ETL600 pilot for 134Hz bandwidth to 480Hz bandwidth nsd guard snr
// the snr from FE is only half the real snr 
// correction = 1.0/(480/134)
#define	BW_CORRECTION_ETL_PILOT_SNR_TO_NSD_SNR		0.2792
// conversion from seconds to number of cycles in main of NSD des 5.333kHz
#define	SECONDS_NSD_DSP_MAIN			5333
// delay time to suppress SNR from FE after a rx cmd (otherwise a snr alarm will be activ after a long rx command)
// delay in seconds
#define	SNR_FE_DELAY_AFTER_RX_CMD		10
//	
/* PUBLIC CONSTANT DECLARATIONS **********************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
//
Unblocking_State_Type t_State_UnblockingCh1, t_State_UnblockingCh2;
//
Monitor_Trip_Guard_Type t_Monitor_Trip_GuardCh1, t_Monitor_Trip_GuardCh2;
//
t_SNR_Monitoring_Type t_SNR_MonitoringCh1, t_SNR_MonitoringCh2;
//
t_Monit_RxTripDuration_Type t_RxTripDurationMonitCh1, t_RxTripDurationMonitCh2;
//
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
/* PUBLIC VARIABLE DECLARATION ***********************************************/
t_NSD_Power_Levels NSD_Power_Speech_Guard;
//
#pragma optimize_off
//
t_NSD_Power_Levels SEGMENT_EXT_CODE * getPtr_NSD_Power_Speech_Guard(void)
{
	//ASSERT(bInitCalled);
	return &NSD_Power_Speech_Guard;
}



/*****************************************************************************
  Function:		Init_NSDUnblocking(void)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
void SEGMENT_EXT_CODE Init_NSDUnblocking(t_NSD_DSP_Config *g_NSD_Config)
{
	float Nominal_GuardLevel;
	// @Pl_ext
	long TPE_Basic_Mode = 0;
	// 
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);
	//
	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			t_State_UnblockingCh1.Channel_activ = OFF;
			t_State_UnblockingCh2.Channel_activ = OFF;
			//
			break;
		case INT_TELPROT_MODE_AES:
			t_State_UnblockingCh1.BlockTimeCounter = BLOCK_TIME_UNBLOCKING;
			t_State_UnblockingCh1.Unbl_Status = UNBLO_IDLE;
			t_State_UnblockingCh1.UnblockDelayCounter = (long) (g_NSD_Config->AES.UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetDelayCounter = (long) (g_NSD_Config->AES.UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetHoldCounter = (long) (g_NSD_Config->AES.UnblockingPulsDuration * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.UnblockHoldCounter = 0;
			t_State_UnblockingCh1.Unbl_Flag = 0;
			t_State_UnblockingCh1.Channel_activ = ON;
			t_State_UnblockingCh1.Unbl_ThresSpeech = AES_SPEECH_UNBL_TH_CORRECTION*g_NSD_Config->AES.UnblockingThreshold;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = g_NSD_Config->AES.GuardLevel* g_NSD_Config->AES.GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			t_State_UnblockingCh1.Unbl_ThresGuard = g_NSD_Config->AES.UnblockingThreshold * Nominal_GuardLevel;
			//
			t_State_UnblockingCh2.Channel_activ = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			// init Ch1 data 
			t_State_UnblockingCh1.BlockTimeCounter = BLOCK_TIME_UNBLOCKING;
			t_State_UnblockingCh1.Unbl_Status = UNBLO_IDLE;
			t_State_UnblockingCh1.UnblockDelayCounter = (long) (g_NSD_Config->NSD[0].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetDelayCounter = (long) (g_NSD_Config->NSD[0].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetHoldCounter = (long) (g_NSD_Config->NSD[0].UnblockingPulsDuration * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.UnblockHoldCounter = 0;
			t_State_UnblockingCh1.Unbl_Flag = 0;
			t_State_UnblockingCh1.Channel_activ = ON;
			t_State_UnblockingCh1.Unbl_ThresSpeech = g_NSD_Config->NSD[0].UnblockingThreshold;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = g_NSD_Config->NSD[0].GuardLevel* g_NSD_Config->NSD[0].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			t_State_UnblockingCh1.Unbl_ThresGuard = g_NSD_Config->NSD[0].UnblockingThreshold * Nominal_GuardLevel;
			t_State_UnblockingCh2.Channel_activ = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// init ch1 
			t_State_UnblockingCh1.BlockTimeCounter = BLOCK_TIME_UNBLOCKING;
			t_State_UnblockingCh1.Unbl_Status = UNBLO_IDLE;
			t_State_UnblockingCh1.UnblockDelayCounter = (long) (g_NSD_Config->NSD[0].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetDelayCounter = (long) (g_NSD_Config->NSD[0].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetHoldCounter = (long) (g_NSD_Config->NSD[0].UnblockingPulsDuration * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.UnblockHoldCounter = 0;
			t_State_UnblockingCh1.Unbl_Flag = 0;
			t_State_UnblockingCh1.Channel_activ = ON;
			t_State_UnblockingCh1.Unbl_ThresSpeech = g_NSD_Config->NSD[0].UnblockingThreshold;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = g_NSD_Config->NSD[0].GuardLevel* g_NSD_Config->NSD[0].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			t_State_UnblockingCh1.Unbl_ThresGuard = g_NSD_Config->NSD[0].UnblockingThreshold * Nominal_GuardLevel;
			//
			// init ch2
			t_State_UnblockingCh2.BlockTimeCounter = BLOCK_TIME_UNBLOCKING;
			t_State_UnblockingCh2.Unbl_Status = UNBLO_IDLE;
			t_State_UnblockingCh2.UnblockDelayCounter = (long) (g_NSD_Config->NSD[1].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh2.Unbl_SetDelayCounter = (long) (g_NSD_Config->NSD[1].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh2.Unbl_SetHoldCounter = (long) (g_NSD_Config->NSD[1].UnblockingPulsDuration * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh2.UnblockHoldCounter = 0;
			t_State_UnblockingCh2.Unbl_Flag = 0;
			t_State_UnblockingCh2.Channel_activ = ON;
			t_State_UnblockingCh2.Unbl_ThresSpeech = g_NSD_Config->NSD[1].UnblockingThreshold;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = g_NSD_Config->NSD[1].GuardLevel* g_NSD_Config->NSD[1].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			t_State_UnblockingCh2.Unbl_ThresGuard = g_NSD_Config->NSD[1].UnblockingThreshold * Nominal_GuardLevel;
			//
			break;
		case INT_TELPROT_MODE_HSD:
			// init Ch1 data 
			t_State_UnblockingCh1.BlockTimeCounter = BLOCK_TIME_UNBLOCKING;
			t_State_UnblockingCh1.Unbl_Status = UNBLO_IDLE;
			t_State_UnblockingCh1.UnblockDelayCounter = (long) (g_NSD_Config->NSD[0].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetDelayCounter = (long) (g_NSD_Config->NSD[0].UnblockingPickupTime * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.Unbl_SetHoldCounter = (long) (g_NSD_Config->NSD[0].UnblockingPulsDuration * ONE_MSEC_MAINCASE);
			t_State_UnblockingCh1.UnblockHoldCounter = 0;
			t_State_UnblockingCh1.Unbl_Flag = 0;
			t_State_UnblockingCh1.Channel_activ = ON;
			t_State_UnblockingCh1.Unbl_ThresSpeech = g_NSD_Config->NSD[0].UnblockingThreshold;
			// new for guard with configuration dependent weight 
			Nominal_GuardLevel = g_NSD_Config->NSD[0].GuardLevel* g_NSD_Config->NSD[0].GuardLevel * GUARD_LEVEL * HALF_SQRT_OF_TWO;
			t_State_UnblockingCh1.Unbl_ThresGuard = g_NSD_Config->NSD[0].UnblockingThreshold * Nominal_GuardLevel;
			t_State_UnblockingCh2.Channel_activ = OFF;
			//
			break;
		default:
			t_State_UnblockingCh1.Channel_activ = OFF;
			t_State_UnblockingCh2.Channel_activ = OFF;
			break;
	}
}

//
/*****************************************************************************
  Function:		NSDUnblocking(void)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		long LSB bit set -> unblocking output aktiv
  Globals:		
  Important:	The Unblocking function call should be in a 1.066kHz case in main programm

******************************************************************************/
//
long NSDUnblocking(long Channel)
{

	Unblocking_State_Type *ptr_t_State_Unblocking;
	volatile float SpeechPower, GuardPower;
	volatile long No_Trip_and_Guard;
  
  // *******************************************************************************
  // *** Do channel selection ******************************************************
  // *******************************************************************************
  if(Channel == TPE1)
  {
		// Channel 1 
		ptr_t_State_Unblocking = &t_State_UnblockingCh1;
		No_Trip_and_Guard = t_Monitor_Trip_GuardCh1.No_Trip_and_Guard_Alarm;
		SpeechPower = NSD_Power_Speech_Guard.SpeechPowerCh1[1]; // tev2 power
		GuardPower = NSD_Power_Speech_Guard.GuardPower[Channel];
  }
  else
  {		
	  if(Channel == TPE2)
	  {
	  // Channel 2
		ptr_t_State_Unblocking = &t_State_UnblockingCh2;
		No_Trip_and_Guard = t_Monitor_Trip_GuardCh2.No_Trip_and_Guard_Alarm;
		SpeechPower = NSD_Power_Speech_Guard.SpeechPowerCh2[1]; // tev2 power
		GuardPower = NSD_Power_Speech_Guard.GuardPower[Channel];
	  }
	  else
	  {
			ASSERT(FALSE);
	  }
  }
  // *******************************************************************************
  // *** Check if selected channel is activ ****************************************
  // *******************************************************************************
  if(ptr_t_State_Unblocking->Channel_activ == ON)
  {
	  // ***************************************************************************
	  // *** Channel is activ  *****************************************************
	  // ***************************************************************************
	  //
	  // after start up do not set Unblocking 
	  if(ptr_t_State_Unblocking->BlockTimeCounter == 0)       // skip unblocking handler if block counter after reset is not zero!
	  {
		//
		// *************************************************************************
		// *** Unblocking state machine ********************************************
		// *************************************************************************
		switch (ptr_t_State_Unblocking->Unbl_Status)
		{
		 case UNBLO_IDLE: 
     		// **********************************************************
     		// inital state check if unblocking condition is activ ******
     		// **********************************************************
			//
			if((((No_Trip_and_Guard) !=0))&&(ptr_t_State_Unblocking->Unbl_ThresGuard > GuardPower)&&(ptr_t_State_Unblocking->Unbl_ThresSpeech > SpeechPower))    // check if still unblocking is active
			{
   				// Unblocking condition is true
				ptr_t_State_Unblocking->UnblockDelayCounter = ptr_t_State_Unblocking->Unbl_SetDelayCounter ;
   				ptr_t_State_Unblocking->Unbl_Status = UNBLO_DELAY;
   			 }
   			else
   			 {
   			 }
	
   			break;
   		  case UNBLO_DELAY: 
   	  		 // **********************************************************
     		 // * Unblocking delay status ********************************
     		 // ********************************************************** 
     		 // wait till unblocking delay is over an check if unblocking status is still active
	   	     if((((No_Trip_and_Guard) !=0))&&(ptr_t_State_Unblocking->Unbl_ThresGuard > GuardPower)&&(ptr_t_State_Unblocking->Unbl_ThresSpeech > SpeechPower))    // check if still unblocking is active
   			  {
				// unblocking condition is true
   	      		if (ptr_t_State_Unblocking->UnblockDelayCounter == 0)                       	// check unblocking delay counter
   	      		 {
   	      	  		
   	      	  	  		// Set Unblocking output active pick up time unblocking is over
   	      	  	  		ptr_t_State_Unblocking->Unbl_Status = UNBLO_HOLD;                           // set unblocking hold status
   	      	  	  		ptr_t_State_Unblocking->UnblockHoldCounter = ptr_t_State_Unblocking->Unbl_SetHoldCounter; // set unblocking hold time
   	      	  	  		ptr_t_State_Unblocking->Unbl_Flag = UNBLOCKING_BIT; // set unblocking output bit
   	      	  	  		//
   	      	  			// update unblocking counter
						ptr_t_State_Unblocking->Unbl_Counter +=1;			// update unblocking counter for test only
   	      		 }
   	      		 else
   	      		 {  // unblocking delay not over -> decrease counter
   	      	 		ptr_t_State_Unblocking->UnblockDelayCounter -=1;
   	      		 }	
   			  }
   			 else
   			  { // unblocking status not yet active go back to idle state
   	      		ptr_t_State_Unblocking->Unbl_Status = UNBLO_IDLE;          	// set unblocking status idle
   			  }
	
   			break;
   		  case UNBLO_HOLD:
			 // **********************************************************
			 // *** Unblocking hold output *******************************
			 // **********************************************************
			 // make the unblocking pulse as long as programmed without checking unblocking condition
   	      	 if (ptr_t_State_Unblocking->UnblockHoldCounter > 0)
   	      	 {
   	      	 	 ptr_t_State_Unblocking->UnblockHoldCounter -=1;	// decrease unblocking hold counter 
   	      	 }
   	      	 else
   	      	 {                                                                
   	      	 	 ptr_t_State_Unblocking->Unbl_Flag = 0;				// unblocking hold time over -> reset unblocking output bit
				 ptr_t_State_Unblocking->Unbl_Status = UNBLO_WAIT;	// set unblocking wait status
			 }
   			break;
   		  case UNBLO_WAIT:
   	  		// **********************************************************
     		// * Unblocking wait status ********************************
     		// *********************************************************
     		// Unblocking status still active, but unblocking output not longer active 
		  	//if((((No_Trip_and_Guard) !=0))&&(ptr_t_State_Unblocking->Unbl_ThresGuard > GuardPower)&&(ptr_t_State_Unblocking->Unbl_ThresSpeech > SpeechPower))    // check if still unblocking is active
	   	    if(No_Trip_and_Guard != 0)    // check if still unblocking is active
			{
   				// unblocking still active do nothing
	   	    }
	   	    else
	   	    {

				ptr_t_State_Unblocking->Unbl_Status = UNBLO_IDLE;	// no unblocking status -> set status unblocking idle
			}
   			break;
   		  default:
   			ptr_t_State_Unblocking->Unbl_Status = UNBLO_IDLE;
   			ptr_t_State_Unblocking->Unbl_Flag = 0;
   			break;
		}	// end switch

	  }
	  else
	  { // skip unblocking handler if block time not zero (start up not finished)
		ptr_t_State_Unblocking->BlockTimeCounter -=1; 
	  }
	 }
	else
	{
		// channel is not active
	}
	return(ptr_t_State_Unblocking->Unbl_Flag);
}
//
/*****************************************************************************
  Function:		Init_Monitor_Trip_Guard(void)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
void SEGMENT_EXT_CODE Init_Monitor_Trip_Guard(t_NSD_DSP_Config *g_NSD_Config)
{
	// @Pl_ext
	long TPE_Basic_Mode = 0;
	// 
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);
	
	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			t_Monitor_Trip_GuardCh1.Channel_activ = OFF;
			t_Monitor_Trip_GuardCh2.Channel_activ = OFF;
			//
			break;
		case INT_TELPROT_MODE_AES:
			// init Ch1 data
			t_Monitor_Trip_GuardCh1.Delay = INTEGR_TIME_DETECTOR*2;
			t_Monitor_Trip_GuardCh1.Timer_No_T_G = 0;
			t_Monitor_Trip_GuardCh1.Timer_T_G = 0;
			t_Monitor_Trip_GuardCh1.No_Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Channel_activ = ON;
			t_Monitor_Trip_GuardCh1.resetRxCmdMask = 0xffffffff; // do not reset rx commands
																 // leave for AES: not possible cause of
																 // guard sequences between commands

			//
			t_Monitor_Trip_GuardCh2.Channel_activ = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			// init Ch1 data 
			t_Monitor_Trip_GuardCh1.Delay = INTEGR_TIME_DETECTOR*2;
			t_Monitor_Trip_GuardCh1.Timer_No_T_G = 0;
			t_Monitor_Trip_GuardCh1.Timer_T_G = 0;
			t_Monitor_Trip_GuardCh1.No_Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Channel_activ = ON;
			t_Monitor_Trip_GuardCh1.resetRxCmdMask = 0; // reset rx commands
			//
			t_Monitor_Trip_GuardCh2.Channel_activ = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// init Ch1 data 
			t_Monitor_Trip_GuardCh1.Delay = INTEGR_TIME_DETECTOR*2;
			t_Monitor_Trip_GuardCh1.Timer_No_T_G = 0;
			t_Monitor_Trip_GuardCh1.Timer_T_G = 0;
			t_Monitor_Trip_GuardCh1.No_Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Channel_activ = ON;
			t_Monitor_Trip_GuardCh1.resetRxCmdMask = 0; // reset rx commands
			// init Ch2 data 
			t_Monitor_Trip_GuardCh2.Delay = INTEGR_TIME_DETECTOR*2;
			t_Monitor_Trip_GuardCh2.Timer_No_T_G = 0;
			t_Monitor_Trip_GuardCh2.Timer_T_G = 0;
			t_Monitor_Trip_GuardCh2.No_Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh2.Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh2.Channel_activ = ON;
			t_Monitor_Trip_GuardCh2.resetRxCmdMask = 0; // reset rx commands
			//
			break;
		case INT_TELPROT_MODE_HSD:
			// init Ch1 data 
			t_Monitor_Trip_GuardCh1.Delay = INTEGR_TIME_DETECTOR_HSD*2;
			t_Monitor_Trip_GuardCh1.Timer_No_T_G = 0;
			t_Monitor_Trip_GuardCh1.Timer_T_G = 0;
			t_Monitor_Trip_GuardCh1.No_Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Trip_and_Guard_Alarm = 0;
			t_Monitor_Trip_GuardCh1.Channel_activ = ON;
			t_Monitor_Trip_GuardCh1.resetRxCmdMask = 0; // reset rx commands
			//
			t_Monitor_Trip_GuardCh2.Channel_activ = OFF;
			//
			break;
		default:
			ASSERT(FALSE);
	}

}
//
//
#pragma optimize_as_cmd_line
//
/*****************************************************************************
  Function:		Monitor_Trip_Guard()
  Description:	 
  Inputs:		Rx_Command (direct output from detector ->without rx prolongation time!) 
  Outputs:		None
  Return:		Alarm for "Trip and Guard" and "No trip and Guard"
  Globals:		
  Important:	This function is used to use in the RT-part of the DSP main programm
				!!!This function clear a received command in case if the guard signal is received!
******************************************************************************/
//
void Monitor_Trip_Guard(long *ptr_Rx_Command, BOOL Rx_Guard, unsigned long *ptr_NSD_Alarm, long Channel, BOOL detectorOutputActive)
{
	Monitor_Trip_Guard_Type *ptr_t_Monitor_Trip_Guard;	
	//
	// ************************************************************** 
	// *** Do channel selection *************************************
	// **************************************************************
	// init pointer for selected channel

	ptr_t_Monitor_Trip_Guard = (Channel == TPE1) ? &t_Monitor_Trip_GuardCh1:&t_Monitor_Trip_GuardCh2;

	if(ptr_t_Monitor_Trip_Guard->Channel_activ == ON)
	{
		//
		// Channel is ON
		//
		// **************************************************************
		// *** Check No trip and no guard  ******************************
		// **************************************************************
		//
		if((*ptr_Rx_Command == 0) && !Rx_Guard )
		{
			// trip and guard are not present 
			if(ptr_t_Monitor_Trip_Guard->Timer_No_T_G < ptr_t_Monitor_Trip_Guard->Delay)
			{
				// delay time not over wait for alarm
				ptr_t_Monitor_Trip_Guard->Timer_No_T_G +=1;
				ptr_t_Monitor_Trip_Guard->No_Trip_and_Guard_Alarm = 0;
				*ptr_NSD_Alarm &= (~NSD_NO_TRIP_AND_GUARD_ALARM);
			}
			else
			{
				// delay time over set alarm
				ptr_t_Monitor_Trip_Guard->No_Trip_and_Guard_Alarm = NSD_NO_TRIP_AND_GUARD_ALARM;
				*ptr_NSD_Alarm |= NSD_NO_TRIP_AND_GUARD_ALARM;
			}

		}
		else
		{
			// trip or guard present reset alarm no trip and guard and timer
			ptr_t_Monitor_Trip_Guard->No_Trip_and_Guard_Alarm = 0;
			ptr_t_Monitor_Trip_Guard->Timer_No_T_G = 0;					// reset timer 
			*ptr_NSD_Alarm &= (~NSD_NO_TRIP_AND_GUARD_ALARM);
		}
		//
		// **************************************************************
		// *** Check Trip and guard at the same time ********************
		// **************************************************************
		//
		// This part has to be in a real time part, because an Rx command must be reset 
		// in case if an Rx Command and a rx guard signal is active at the same time!
		
		// check detectoroutput also, used for AES, be sure in NSD case "detectorOutputActive" not used (not TRUE)!
		if(((detectorOutputActive)||(*ptr_Rx_Command != 0)) && (Rx_Guard))
		{
			// trip and guard are present 	
			// -> set Alarm without pickup time and reset all Rx Commands
			ptr_t_Monitor_Trip_Guard->Trip_and_Guard_Alarm = NSD_TRIP_AND_GUARD_ALARM;
			*ptr_NSD_Alarm |= NSD_TRIP_AND_GUARD_ALARM;
			*ptr_Rx_Command = *ptr_Rx_Command & ptr_t_Monitor_Trip_Guard->resetRxCmdMask;	// reset rx commands
		}
		else
		{
			// trip or guard present -> reset alarm trip and guard and timer
			ptr_t_Monitor_Trip_Guard->Trip_and_Guard_Alarm = 0;
			*ptr_NSD_Alarm &= (~NSD_TRIP_AND_GUARD_ALARM);
			//ptr_t_Monitor_Trip_Guard->Timer_T_G = 0;					// reset timer 
		}
	}
	else
	{
		// channel is off 
		ptr_t_Monitor_Trip_Guard->No_Trip_and_Guard_Alarm = 0;
		ptr_t_Monitor_Trip_Guard->Trip_and_Guard_Alarm = 0;
		*ptr_NSD_Alarm &= (~(NSD_TRIP_AND_GUARD_ALARM|NSD_NO_TRIP_AND_GUARD_ALARM));
	}
}
//
#pragma optimize_off
//
/*****************************************************************************
  Function:		Update_Dev_Status_Guard(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD)
  Description:	Update the Guard Input signal and the Guard Mean power in the struct 
				NSD_Developper_Status for Channel 1 and Channel 2
  Inputs:		Pointer to struct t_NSD_DevelopperStatus_Type
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
//
void SEGMENT_EXT_CODE Update_Dev_Status_Guard(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD)
{
	//
	ptr_Develop_Status_NSD->GuardMeanLevelCh1 =	NSD_Power_Speech_Guard.GuardPowerMean[1]; // Channel 1
	ptr_Develop_Status_NSD->InputGuardSignalCh1 = NSD_Power_Speech_Guard.GuardPower[1];	// Channel 1
	//
	ptr_Develop_Status_NSD->GuardMeanLevelCh2 =	NSD_Power_Speech_Guard.GuardPowerMean[2]; // Channel 2
	ptr_Develop_Status_NSD->InputGuardSignalCh2 = NSD_Power_Speech_Guard.GuardPower[2];	// Channel 2
	//
	ptr_Develop_Status_NSD->f_Development_3 = t_SNR_MonitoringCh1.Actual_SNR;
	ptr_Develop_Status_NSD->f_Development_4 = t_SNR_MonitoringCh2.Actual_SNR;
	//
}


//
/*****************************************************************************
  Function:		Init_SNR_Monitoring_Guard(t_NSD_DSP_Config *g_NSD_Config)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	SNR-Alarm Threshold will be set according to the sensitivest command application
				configured (blocking permissve, direct) 
				
******************************************************************************/
void SEGMENT_EXT_CODE Init_SNR_Monitoring_Guard(t_NSD_DSP_Config *g_NSD_Config)
{
	int i;
	long	temp_Application;
	float   SNR_Correction_Trip_to_Guard;

	// @Pl_ext
	long TPE_Basic_Mode = 0;
	// 
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);

	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			t_SNR_MonitoringCh1.Channel_active = OFF;
			t_SNR_MonitoringCh2.Channel_active = OFF;
			//
			break;
		case INT_TELPROT_MODE_AES:
			// set SNR correction for ETL or NSD pilot
			// init Ch1 data
			if((g_NSD_Config->AES.AES600_Mode & AES600_NSD_ETL_GUARD) == AES600_NSD_ETL_GUARD)
			{
				// nsd own guard used
				t_SNR_MonitoringCh1.NSD_ETL_Guard_Config = 1;
			}
			else
			{
				// etl guard used for nsd
				t_SNR_MonitoringCh1.NSD_ETL_Guard_Config = 0;
			}
		
			t_SNR_MonitoringCh1.Channel_active = ON;

			//
			// check guard level for SNR correction factor
			//
			if(g_NSD_Config->AES.GuardLevel >= TRIP_WEIGHT_DEFAULT) 
			{
				// @Pl_AES
				// SNR_Correction_Trip_to_Guard = TRIP_GUARD_POWER_0DB;
				SNR_Correction_Trip_to_Guard = AES_TRIP_GUARD_CORRECTION;
			}
			else
			{
				// @Pl_AES
				//SNR_Correction_Trip_to_Guard = (TRIP_GUARD_POWER_0DB * (g_NSD_Config->AES.GuardLevel) * (g_NSD_Config->AES.GuardLevel));
				SNR_Correction_Trip_to_Guard = (AES_TRIP_GUARD_CORRECTION * (g_NSD_Config->AES.GuardLevel) * (g_NSD_Config->AES.GuardLevel));

			}

			if ((g_NSD_Config->AES.AES600_Mode & AES600_T0) == 0)	// T01
			{

				if (g_NSD_Config->AES.AES600_Mode & AES600_SEC_DEP_CMD)
				{
					// SNR threshold high security
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = AES_SNR_ALARM_THRESHOLD_T01_HIGH_SECURITY * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = AES_SNR_ALARM_THRESHOLD_T01_HIGH_SECURITY * AES_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold = AES_SNR_ALARM_THRESHOLD_T01_HIGH_SECURITY;
				}
				else
				{
					// SNR threshold low security
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = AES_SNR_ALARM_THRESHOLD_T01_LOW_SECURITY * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = AES_SNR_ALARM_THRESHOLD_T01_LOW_SECURITY * AES_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold = AES_SNR_ALARM_THRESHOLD_T01_LOW_SECURITY;
				}
			}
			else	// T02
			{
				if (g_NSD_Config->AES.AES600_Mode & AES600_SEC_DEP_CMD)
				{
					// SNR threshold high security
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = AES_SNR_ALARM_THRESHOLD_T02_HIGH_SECURITY * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = AES_SNR_ALARM_THRESHOLD_T02_HIGH_SECURITY * AES_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold = AES_SNR_ALARM_THRESHOLD_T02_HIGH_SECURITY;
				}
				else
				{
					// SNR threshold low security
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = AES_SNR_ALARM_THRESHOLD_T02_LOW_SECURITY * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = AES_SNR_ALARM_THRESHOLD_T02_LOW_SECURITY * AES_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
					t_SNR_MonitoringCh1.SNR_AlarmThreshold = AES_SNR_ALARM_THRESHOLD_T02_LOW_SECURITY;
				}
			}
			//
			t_SNR_MonitoringCh1.meanGuardEnvelope = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_I = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_Q = 0.0;
			t_SNR_MonitoringCh1.Variance = 0.0;
			t_SNR_MonitoringCh1.Variance_I = 0.0;
			t_SNR_MonitoringCh1.Variance_Q = 0.0;
			t_SNR_MonitoringCh1.Actual_SNR = 0.0;
			t_SNR_MonitoringCh1.SNR_Alarm = 0;
			t_SNR_MonitoringCh1.NoiseLevel = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_I = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_Q = 0.0;
			t_SNR_MonitoringCh1.Signal_Level = 0.0;
			t_SNR_MonitoringCh1.SNR_Delay_Time_after_rx_Cmd = 0;
			//
			t_SNR_MonitoringCh2.Channel_active = OFF;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			// set SNR correction for ETL or NSD pilot
			// init Ch1 data
			if((g_NSD_Config->NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				// nsd own guard used
				t_SNR_MonitoringCh1.NSD_ETL_Guard_Config = 1;
			}
			else
			{
				// etl guard used for nsd
				t_SNR_MonitoringCh1.NSD_ETL_Guard_Config = 0;
			}

			t_SNR_MonitoringCh1.Channel_active = ON;
			//
			// check guard level for SNR correction factor
			//
			if(g_NSD_Config->NSD[0].GuardLevel >= TRIP_WEIGHT_DEFAULT) 
			{
				SNR_Correction_Trip_to_Guard = TRIP_GUARD_POWER_0DB;
			}
			else
			{
				SNR_Correction_Trip_to_Guard = (TRIP_GUARD_POWER_0DB * (g_NSD_Config->NSD[0].GuardLevel) * (g_NSD_Config->NSD[0].GuardLevel));

			}
			
			// setting of the Threshold must be made dependable from the configuration application of command 
			// 
			// @Pl@ der folgende Code geht von der Annahme aus, dass für die Anwendung Blocking die Verlässlichkeit am besten ist und für direct am schlechtesten
			// -> wenn dies nicht der Fall ist muss der Code angepasst werden
			temp_Application = 0;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				if(temp_Application < g_NSD_Config->NSD[0].CommandApplication[i])
				{
					temp_Application = g_NSD_Config->NSD[0].CommandApplication[i];
				}
				else
				{

				}
			}
			switch(temp_Application)
			{
			case BLOCKING:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_BLOCKING * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			case PERMISSIVE:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_PERMISSIVE * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_PERMISSIVE * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_PERMISSIVE;
				break;
			case DIRECT:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_DIRECT * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_DIRECT * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_DIRECT;
				break;
			case COMMAND_OFF:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_BLOCKING * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			default:
				ASSERT(FALSE);
			}
			//
			t_SNR_MonitoringCh1.meanGuardEnvelope = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_I = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_Q = 0.0;
			t_SNR_MonitoringCh1.Variance = 0.0;
			t_SNR_MonitoringCh1.Variance_I = 0.0;
			t_SNR_MonitoringCh1.Variance_Q = 0.0;
			t_SNR_MonitoringCh1.Actual_SNR = 0.0;
			t_SNR_MonitoringCh1.SNR_Alarm = 0;
			t_SNR_MonitoringCh1.NoiseLevel = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_I = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_Q = 0.0;
			t_SNR_MonitoringCh1.Signal_Level = 0.0;
			t_SNR_MonitoringCh1.SNR_Delay_Time_after_rx_Cmd = 0;
			//
			t_SNR_MonitoringCh2.Channel_active = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// set SNR correction for ETL or NSD pilot
			// settings for Ch1
			if((g_NSD_Config->NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				// nsd own guard used
				t_SNR_MonitoringCh1.NSD_ETL_Guard_Config = 1;
			}
			else
			{
				// etl guard used for nsd
				t_SNR_MonitoringCh1.NSD_ETL_Guard_Config = 0;
			}
			//// settings for Ch2
			if((g_NSD_Config->NSD[1].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				// nsd own guard used
				t_SNR_MonitoringCh2.NSD_ETL_Guard_Config = 1;
			}
			else
			{
				// etl guard used for nsd
				t_SNR_MonitoringCh2.NSD_ETL_Guard_Config = 0;
			}

			// init ch1 
			t_SNR_MonitoringCh1.Channel_active = ON;
			// setting of the Threshold must be make dependable from the configuration of command application
			//
			// check guard level for SNR correction factor
			//
			if(g_NSD_Config->NSD[0].GuardLevel >= TRIP_WEIGHT_DEFAULT) 
			{
				SNR_Correction_Trip_to_Guard = TRIP_GUARD_POWER_0DB;
			}
			else
			{
				SNR_Correction_Trip_to_Guard = (TRIP_GUARD_POWER_0DB * (g_NSD_Config->NSD[0].GuardLevel) * (g_NSD_Config->NSD[0].GuardLevel));
			}
			// 
			// @Pl@ der folgende Code geht von der Annahme aus, dass für die Anwendung Blocking die Verlässlichkeit am besten ist und für direct am schlechtesten
			// -> wenn dies nicht der Fall ist muss der Code angepasst werden
			temp_Application = 0;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				if(temp_Application < g_NSD_Config->NSD[0].CommandApplication[i])
				{
					temp_Application = g_NSD_Config->NSD[0].CommandApplication[i];
				}
				else
				{

				}
			}
			switch(temp_Application)
			{
			case BLOCKING:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_BLOCKING * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			case PERMISSIVE:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_PERMISSIVE * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_PERMISSIVE * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_PERMISSIVE;
				break;
			case DIRECT:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_DIRECT * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_DIRECT * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_DIRECT;
				break;
			case COMMAND_OFF:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_BLOCKING * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			default:
				ASSERT(FALSE);
			}
			//
			t_SNR_MonitoringCh1.meanGuardEnvelope = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_I = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_Q = 0.0;
			t_SNR_MonitoringCh1.Variance = 0.0;
			t_SNR_MonitoringCh1.Variance_I = 0.0;
			t_SNR_MonitoringCh1.Variance_Q = 0.0;
			t_SNR_MonitoringCh1.Actual_SNR = 0.0;
			t_SNR_MonitoringCh1.SNR_Alarm = 0;
			t_SNR_MonitoringCh1.NoiseLevel = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_I = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_Q = 0.0;
			t_SNR_MonitoringCh1.Signal_Level = 0.0;
			t_SNR_MonitoringCh1.SNR_Delay_Time_after_rx_Cmd = 0;
			//
			// init ch2
			//
			t_SNR_MonitoringCh2.Channel_active = ON;
			// setting of the Threshold must be make dependable from the configuration of teh command application
			//
			//
			// check guard level for SNR correction factor
			//
			if(g_NSD_Config->NSD[1].GuardLevel >= TRIP_WEIGHT_DEFAULT) 
			{
				SNR_Correction_Trip_to_Guard = TRIP_GUARD_POWER_0DB;
			}
			else
			{
				SNR_Correction_Trip_to_Guard = (TRIP_GUARD_POWER_0DB * (g_NSD_Config->NSD[1].GuardLevel) * (g_NSD_Config->NSD[1].GuardLevel));
			}
			//
			// @Pl@ der folgende Code geht von der Annahme aus, dass für die Anwendung Blocking die Verlässlichkeit am besten ist und für direct am schlechtesten
			// -> wenn dies nicht der Fall ist muss der Code angepasst werden
			temp_Application = 0;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				if(temp_Application < g_NSD_Config->NSD[1].CommandApplication[i])
				{
					temp_Application = g_NSD_Config->NSD[1].CommandApplication[i];
				}
				else
				{

				}
			}
			switch(temp_Application)
			{
			case BLOCKING:
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_BLOCKING * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			case PERMISSIVE:
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_PERMISSIVE * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_PERMISSIVE * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_PERMISSIVE;
				break;
			case DIRECT:
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_DIRECT * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_DIRECT * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_DIRECT;
				break;
			case COMMAND_OFF:
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Set = SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold_Reset = SNR_ALARM_THRESHOLD_BLOCKING * HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh2.SNR_AlarmThreshold = SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			default:
				ASSERT(FALSE);
			}
			//
			t_SNR_MonitoringCh2.meanGuardEnvelope = 0.0;
			t_SNR_MonitoringCh2.meanGuardEnvelope_I = 0.0;
			t_SNR_MonitoringCh2.meanGuardEnvelope_Q = 0.0;
			t_SNR_MonitoringCh2.Variance = 0.0;
			t_SNR_MonitoringCh2.Variance_I = 0.0;
			t_SNR_MonitoringCh2.Variance_Q = 0.0;
			t_SNR_MonitoringCh2.Actual_SNR = 0.0;
			t_SNR_MonitoringCh2.SNR_Alarm = 0;
			t_SNR_MonitoringCh2.NoiseLevel = 0.0;
			t_SNR_MonitoringCh2.NoiseLevel_I = 0.0;
			t_SNR_MonitoringCh2.NoiseLevel_Q = 0.0;
			t_SNR_MonitoringCh2.Signal_Level = 0.0;
			t_SNR_MonitoringCh2.SNR_Delay_Time_after_rx_Cmd = 0;
			//
			break;
		case INT_TELPROT_MODE_HSD:
			// init Ch1 data 
			t_SNR_MonitoringCh1.Channel_active = ON;
			//
			// check guard level for SNR correction factor
			//
			if(g_NSD_Config->NSD[0].GuardLevel >= TRIP_WEIGHT_DEFAULT) 
			{
				SNR_Correction_Trip_to_Guard = TRIP_GUARD_POWER_0DB;
			}
			else
			{
				SNR_Correction_Trip_to_Guard = (TRIP_GUARD_POWER_0DB * (g_NSD_Config->NSD[0].GuardLevel) * (g_NSD_Config->NSD[0].GuardLevel));

			}
			
			// setting of the Threshold must be made dependable from the configuration application of command 
			// 
			// @Pl@ der folgende Code geht von der Annahme aus, dass für die Anwendung Blocking die Verlässlichkeit am besten ist und für direct am schlechtesten
			// -> wenn dies nicht der Fall ist muss der Code angepasst werden
			temp_Application = 0;
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				if(temp_Application < g_NSD_Config->NSD[0].CommandApplication[i])
				{
					temp_Application = g_NSD_Config->NSD[0].CommandApplication[i];
				}
				else
				{

				}
			}
			switch(temp_Application)
			{
			case BLOCKING:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = HSD_SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = HSD_SNR_ALARM_THRESHOLD_BLOCKING * HSD_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = HSD_SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			case PERMISSIVE:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = HSD_SNR_ALARM_THRESHOLD_PERMISSIVE * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = HSD_SNR_ALARM_THRESHOLD_PERMISSIVE * HSD_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = HSD_SNR_ALARM_THRESHOLD_PERMISSIVE;
				break;
			case DIRECT:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = HSD_SNR_ALARM_THRESHOLD_DIRECT * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = HSD_SNR_ALARM_THRESHOLD_DIRECT * HSD_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = HSD_SNR_ALARM_THRESHOLD_DIRECT;
				break;
			case COMMAND_OFF:
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Set = HSD_SNR_ALARM_THRESHOLD_BLOCKING * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold_Reset = HSD_SNR_ALARM_THRESHOLD_BLOCKING * HSD_HYSTERESE_SNR_ALARM * SNR_Correction_Trip_to_Guard;
				t_SNR_MonitoringCh1.SNR_AlarmThreshold = HSD_SNR_ALARM_THRESHOLD_BLOCKING;
				break;
			default:
				ASSERT(FALSE);
			}
			//
			t_SNR_MonitoringCh1.meanGuardEnvelope = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_I = 0.0;
			t_SNR_MonitoringCh1.meanGuardEnvelope_Q = 0.0;
			t_SNR_MonitoringCh1.Variance = 0.0;
			t_SNR_MonitoringCh1.Variance_I = 0.0;
			t_SNR_MonitoringCh1.Variance_Q = 0.0;
			t_SNR_MonitoringCh1.Actual_SNR = 0.0;
			t_SNR_MonitoringCh1.SNR_Alarm = 0;
			t_SNR_MonitoringCh1.NoiseLevel = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_I = 0.0;
			t_SNR_MonitoringCh1.NoiseLevel_Q = 0.0;
			t_SNR_MonitoringCh1.Signal_Level = 0.0;
			t_SNR_MonitoringCh1.SNR_Delay_Time_after_rx_Cmd = 0;
			//
			t_SNR_MonitoringCh2.Channel_active = OFF;
			//
			break;
		default:
			ASSERT(FALSE);
	}

}
//
/*****************************************************************************
  Function:		SNR_Monitoring_Guard(long Rx_Command, long Rx_Guard, t_NSD_Status *NSD_Status, long Channel)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	The SNR_Monitoring function call should be in a 1.066kHz case in main programm

******************************************************************************/
void SEGMENT_EXT_CODE SNR_Monitoring_Guard(long Rx_Command, float SNR_from_FE, t_NSD_Status *NSD_Status, long Channel)
{

	t_SNR_Monitoring_Type *ptr_SNR_Monit;
	float	RxEnvelope, f_Dif_Variance, snr;
	Monitor_Trip_Guard_Type *ptr_t_Monitor_Trip_Guard;

	// **************************************************************
	// *** Select active channel ************************************
	// **************************************************************
	if(Channel == TPE1)
	{
		// set pointer for Ch1
		ptr_SNR_Monit = &t_SNR_MonitoringCh1;
		RxEnvelope = NSD_Power_Speech_Guard.GuardPower[Channel];
		ptr_t_Monitor_Trip_Guard = &t_Monitor_Trip_GuardCh1;
	}
	else
	{
		if(Channel == TPE2)
		{
			// set pointer for Ch2
			ptr_SNR_Monit = &t_SNR_MonitoringCh2;
			RxEnvelope = NSD_Power_Speech_Guard.GuardPower[Channel];
			ptr_t_Monitor_Trip_Guard = &t_Monitor_Trip_GuardCh2;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	if(ptr_SNR_Monit->Channel_active == ON)
	{
		// **********************************************************
		// *** Channel is active do SNR monitoring ******************
		// **********************************************************
		if((Rx_Command == 0)||((ptr_t_Monitor_Trip_Guard->Trip_and_Guard_Alarm  & NSD_TRIP_AND_GUARD_ALARM )== NSD_TRIP_AND_GUARD_ALARM))
		{
			//
			if(ptr_SNR_Monit->NSD_ETL_Guard_Config == 1)
			{
				// NSD own guard active -> do calculation of snr
				ptr_SNR_Monit->GuardEnvelope = RxEnvelope;
				ptr_SNR_Monit->meanGuardEnvelope = ptr_SNR_Monit->meanGuardEnvelope * GUARD_COEF_1 + GUARD_COEF_2 * RxEnvelope;
				f_Dif_Variance = ptr_SNR_Monit->meanGuardEnvelope - RxEnvelope;
				ptr_SNR_Monit->Variance = ptr_SNR_Monit->Variance * SNR_COEF_1 + SNR_COEF_2 * f_Dif_Variance * f_Dif_Variance;
				snr = 2.0*((ptr_SNR_Monit->meanGuardEnvelope * ptr_SNR_Monit->meanGuardEnvelope / (0.00001 + ptr_SNR_Monit->Variance))- 0.9);
				//ptr_SNR_Monit->Actual_SNR = snr;
				// use of constant 0.00001 is necessary for start up otherwise division by 0
				ptr_SNR_Monit->Actual_SNR = ptr_SNR_Monit->Actual_SNR * SNR_COEF_1 + SNR_COEF_2 * snr;
			}
			else
			{
				//
				// etl guard active -> get active snr form FE
				//
				if(ptr_SNR_Monit->SNR_Delay_Time_after_rx_Cmd > 0) 
				{
					// delay after a rx command not over wait
					ptr_SNR_Monit->SNR_Delay_Time_after_rx_Cmd -=1;	
				}
				else
				{
					// delay time after a rx command is over take over snr from FE
					ptr_SNR_Monit->Actual_SNR = SNR_from_FE * BW_CORRECTION_ETL_PILOT_SNR_TO_NSD_SNR;
				}
			}
			// check if SNR-alarm is active
			if(ptr_SNR_Monit->Actual_SNR <= ptr_SNR_Monit->SNR_AlarmThreshold_Set) 
			{
				// snr alarm is active 
				ptr_SNR_Monit->SNR_Alarm = NSD_SNR_ALARM;
				NSD_Status->NSD_Alarms |= NSD_SNR_ALARM;
			}
			else
			{
				if(ptr_SNR_Monit->Actual_SNR >= ptr_SNR_Monit->SNR_AlarmThreshold_Reset)
				{
					// reset snr alarm
					ptr_SNR_Monit->SNR_Alarm = 0;
					NSD_Status->NSD_Alarms &= (~NSD_SNR_ALARM);
				}
				else
				{
					// do not reset snr alarm reset threshold not reached
				}
			}
		}
		else
		{
			// Rx Command received do not update SNR monitoring for guard channel
			// set delay time for SNR from FE-DSP
			ptr_SNR_Monit->SNR_Delay_Time_after_rx_Cmd = SNR_FE_DELAY_AFTER_RX_CMD * (SECONDS_NSD_DSP_MAIN/NUMBER_OF_MAINCASE);
		}
		// update NSD Status
		NSD_Status->ActualSNR = ptr_SNR_Monit->Actual_SNR;
		NSD_Status->ActualSNRAlarmThreshold = ptr_SNR_Monit->SNR_AlarmThreshold;
	}
	else
	{
		// channel is off do nothing
		NSD_Status->ActualSNR = 1.0;
		NSD_Status->ActualSNRAlarmThreshold = 1.0;
	}
}
//
/*****************************************************************************
  Function:		 Init_RxCommandDurationMonit(t_NSD_DSP_Config *g_NSD_Config)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	The SNR_Monitoring function call should be in a 1.066kHz case in main programm

******************************************************************************/
void SEGMENT_EXT_CODE Init_RxCommandDurationMonit(t_NSD_DSP_Config *g_NSD_Config)
{

	unsigned long i;

	// @Pl_ext
	long TPE_Basic_Mode = 0;
	// 
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);
	//
	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			t_RxTripDurationMonitCh1.Channel_Active = OFF;
			t_RxTripDurationMonitCh2.Channel_Active = OFF;
			//
			break;
		case INT_TELPROT_MODE_AES:
			// init Ch1 data 
			t_RxTripDurationMonitCh1.Channel_Active = OFF;
			t_RxTripDurationMonitCh2.Channel_Active = OFF;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			// init Ch1 data 
			// set channel 1 active
			t_RxTripDurationMonitCh1.Channel_Active = ON;
			// set configured times
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				t_RxTripDurationMonitCh1.RxDurationTimeConfig[i] = g_NSD_Config->NSD[0].RxOutComMonit_Duration[i] * SECONDS_NSD_DSP_MAIN;
				// check if monitoring is on -> when time not equal zero!
				// -> @Pl@ if monitoring is off value should be 0xffffffff from HMI but now it is 0x00000000
				if( t_RxTripDurationMonitCh1.RxDurationTimeConfig[i] != 0) t_RxTripDurationMonitCh1.RxDurationMonit_On_Off[i] = ON;
				else t_RxTripDurationMonitCh1.RxDurationMonit_On_Off[i] = OFF;
			}
			// reset alarm
			t_RxTripDurationMonitCh1.RxDurationAlarm = 0;
			// init Ch2;
			t_RxTripDurationMonitCh2.Channel_Active = OFF;
			t_RxTripDurationMonitCh2.RxDurationAlarm = 0;

			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// init ch1 
			// set channel 1 active
			t_RxTripDurationMonitCh1.Channel_Active = ON;
			// set configured times
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				t_RxTripDurationMonitCh1.RxDurationTimeConfig[i] = g_NSD_Config->NSD[0].RxOutComMonit_Duration[i] * SECONDS_NSD_DSP_MAIN;
				// check if monitoring is on -> when time not equal zero!
				// -> @Pl@ if monitoring is off value should be 0xffffffff from HMI but now it is 0x00000000
				if( t_RxTripDurationMonitCh1.RxDurationTimeConfig[i] != 0) t_RxTripDurationMonitCh1.RxDurationMonit_On_Off[i] = ON;
				else t_RxTripDurationMonitCh1.RxDurationMonit_On_Off[i] = OFF;
			}
			// reset alarm
			t_RxTripDurationMonitCh1.RxDurationAlarm = 0;
			//
			// init Ch2;
			//
			t_RxTripDurationMonitCh2.Channel_Active = ON;
			// set configured times
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				t_RxTripDurationMonitCh2.RxDurationTimeConfig[i] = g_NSD_Config->NSD[1].RxOutComMonit_Duration[i] * SECONDS_NSD_DSP_MAIN;
				// check if monitoring is on -> when time not equal zero!
				// -> @Pl@ if monitoring is off value should be 0xffffffff from HMI but now it is 0x00000000
				if( t_RxTripDurationMonitCh2.RxDurationTimeConfig[i] != 0) t_RxTripDurationMonitCh2.RxDurationMonit_On_Off[i] = ON;
				else t_RxTripDurationMonitCh2.RxDurationMonit_On_Off[i] = OFF;
			}
			// reset alarm
			t_RxTripDurationMonitCh2.RxDurationAlarm = 0;
			//
			break;
		case INT_TELPROT_MODE_HSD:
			// init Ch1 data HSD
			// set channel 1 active
			t_RxTripDurationMonitCh1.Channel_Active = ON;
			// set configured times
			for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
			{
				t_RxTripDurationMonitCh1.RxDurationTimeConfig[i] = g_NSD_Config->NSD[0].RxOutComMonit_Duration[i] * SECONDS_NSD_DSP_MAIN;
				// check if monitoring is on -> when time not equal zero!
				// -> @Pl@ if monitoring is off value should be 0xffffffff from HMI but now it is 0x00000000
				if( t_RxTripDurationMonitCh1.RxDurationTimeConfig[i] != 0) t_RxTripDurationMonitCh1.RxDurationMonit_On_Off[i] = ON;
				else t_RxTripDurationMonitCh1.RxDurationMonit_On_Off[i] = OFF;
			}
			// reset alarm
			t_RxTripDurationMonitCh1.RxDurationAlarm = 0;
			// init Ch2;
			t_RxTripDurationMonitCh2.Channel_Active = OFF;
			t_RxTripDurationMonitCh2.RxDurationAlarm = 0;
			//
			break;
		default:
			ASSERT(FALSE);
	}

}
//
#pragma optimize_as_cmd_line
//
/*****************************************************************************
  Function:		long RxCommandDurationMonit(long RxCommandsFromReceiver, unsigned long *ptr_Alarm, long Channel)
  Description:	 
  Inputs:		RxCommandsFromReceiver
  Outputs:		None
  Return:		RxCommandsFromReceiver
  Globals:		
  Important:	The RxCommandDurationMonit function call should be in a 5.33kHz case in main programm

******************************************************************************/

long RxCommandDurationMonit(long RxCommandsFromReceiver, unsigned long *ptr_Alarm, long Channel)
{
	unsigned long i;

	t_Monit_RxTripDuration_Type *ptr_RxDuration_Monit;

	// **************************************************************
	// *** Select active channel ************************************
	// **************************************************************
	if(Channel == TPE1)
	{
		// set pointer for Ch1
		ptr_RxDuration_Monit = &t_RxTripDurationMonitCh1;
	}
	else
	{
		if(Channel == TPE2)
		{
			// set pointer for Ch2
			ptr_RxDuration_Monit = &t_RxTripDurationMonitCh2;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	//
	if(ptr_RxDuration_Monit->Channel_Active == ON)
	{
		// channel is active do rx trip duration monitoring for all commands
		for(i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
		{
			if(RxCommandsFromReceiver & (BIT0 <<i))
			{
				// selected command is active and monitoring is configured on
				if(ptr_RxDuration_Monit->TripDurationCounter[i]-- <= 0)
				{
					ptr_RxDuration_Monit->TripDurationCounter[i] = 0;
					if(ptr_RxDuration_Monit->RxDurationMonit_On_Off[i] == ON)
					{
						//Reset active command
						RxCommandsFromReceiver &= ~(BIT0<<i);
						// Set alarm active
						ptr_RxDuration_Monit->RxDurationAlarm |= (BIT0<<i);
					}
					else
					{
						// rx trip duration monitoring is configured off 
						ptr_RxDuration_Monit->TripDurationCounter[i] = 0;
					}
				}
			}
			else
			{
				// selected command is not active -> reset alarm, set counter to delay time
				ptr_RxDuration_Monit->TripDurationCounter[i] = ptr_RxDuration_Monit->RxDurationTimeConfig[i];
				// Reset alarm active
				ptr_RxDuration_Monit->RxDurationAlarm &= (~(BIT0<<i));
			}

		}
		// check if alarm is active and set/reset alarm in nsd status
		if( ptr_RxDuration_Monit->RxDurationAlarm != 0) *ptr_Alarm |= NSD_RX_TRIP_DURATION_ALARM;
		else *ptr_Alarm &= (~NSD_RX_TRIP_DURATION_ALARM);
	}
	else
	{
		// channel is not active -> do nothing
	}
	//
	return(RxCommandsFromReceiver);
	
}
