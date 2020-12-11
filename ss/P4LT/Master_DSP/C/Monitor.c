
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/Master_DSP/C/Monitor.c $
 * 
 * 86    11-09-02 15:13 Chhamaa
 * Power supply alarm bug fixed.
 * 
 * 85    11-03-14 10:21 Chhamaa
 * ALM_PSU2_failed only in case of redundant power supply.
 * 
 * 84    10-11-24 16:15 Chhamaa
 * Handling of new rel. 4 alarms implemented.
 * 
 * 83    10-10-28 11:23 Chhamaa
 * monitor_evaluateDataAlarmsAndWarnings() modified for transit alarms.
 * 
 * 81    18.06.08 11:32 Chhamaa
 * monitor_init() modified for NSD.
 * 
 * 79    18.03.08 17:05 Chchlee
 * Supress MUX sync alarm if ofdm_syncstate <> V.11 sync
 * 
 * 78    08-02-25 10:55 Chtognu
 * Merged changes from ad.equ.branch:
 * ALM_DCD_NSK1..4 added
 * 
 * 77    2.11.07 16:05 Chhamaa
 * Minor modification in monitor_evaluatePlcIfAlarmsAndWarnings().
 * 
 * 76    19.01.07 15:30 Chhamaa
 * Load re-balancing
 * 
 * 75    16.01.07 17:37 Chhamaa
 * Unused code removed.
 * 
 * 74    10.01.07 16:29 Chhamaa
 * O4CV modifications
 * 
 * 72    24.11.06 11:00 Chpeplu
 * In function monitor_recordRxCmdEvents() cmd E and F added!
 * 
 * 71    14.11.06 10:41 Chchlee
 * TPE1: Commands E & F added
 * 
 * 70    14.11.06 10:04 Chpeplu
 * Function monitor_init() changed for custom mode
 * 
 * 69    12.10.06 15:33 Chhamaa
 * AES modification
 * 
 * 68    19.09.06 10:48 Chhamaa
 * ALM_NSD_Command_Outputs_Blocked added.
 * 
 * 67    18.09.06 16:30 Chhamaa
 * Manual alarm reset for AES added.
 * 
 * 66    7.09.06 14:51 Chchlee
 * no comment
 * 
 * 65    25.08.06 18:36 Chchlee
 * g_isR1LAPresent replaced by g_isR1LAConfigured
 * 
 * 64    22.08.06 17:22 Chhamaa
 * Code for recording of AES Tx/Rx commands optimized -> less memory
 * 
 * 63    18.08.06 16:26 Chhamaa
 * Persistent command for recording added.
 * 
 * 62    16.08.06 14:47 Chhamaa
 * Recording of AES Tx/Rx commands added.
 * 
 * 61    11.08.06 16:29 Chchlee
 * R1LA lifesign supervision only active if any LAN service configured
 * 
 * 60    9.08.06 16:49 Chchlee
 * R1LA lifesign supervision only active if any LAN service configured
 * 
 * 59    19.07.06 15:25 Chchlee
 * ALM_R1LA_Life_Sign alarm added
 * 
 * 58    6.06.06 9:37 Chalgyg
 * G.703 LOS alarm added to dataAlarmsAndWarnings.
 * 
 * 57    5.05.06 15:07 Chchlee
 * Issue #300 fixed
 * 
 * 55    7.04.06 15:55 Chmibuh
 * Comment added: where to add the monitoring of R1LA status flags
 * nskAlarms.LANNotConnected and nskAlarms.LANNotBooted
 * 
 * 54    5.04.06 16:15 Chchlee
 * monitor_evaluatePlcLinkAlarms(): suppression of link alarms introduced
 * 
 * 52    24.10.05 8:41 Chchlee
 * Suppress TDM bus traffic/alarms at startup for 5s
 * 
 * 51    13.10.05 19:23 Chchlee
 * Debug code deactivated
 * 
 * 50    7.10.05 11:47 Chchlee
 * If lifesign stopped counting, the alarms of the respective DSP are now
 * suppressed
 * 
 * 49    7.10.05 8:49 Chchlee
 * Comments added
 * 
 * 48    5.10.05 11:40 Chchlee
 * isInDebugMode() considered in DAC supervision
 * 
 * 47    28.09.05 14:46 Chchlee
 * isInDebugMode() moved to IO_BUFFER.H
 * 
 * 46    27.09.05 15:04 Chchlee
 * ErrorStatus -> chksumErrorCnt
 * 
 * 45    21.09.05 16:32 Chchlee
 * TIME_FACTOR for pickup and hold times adjusted
 * 
 * 44    8.09.05 16:36 Chchlee
 * New names for BASIC_G4AI_... constants
 * 
 * 43    8.09.05 14:18 Chhamaa
 * ALARM_EVENT_DEBUG2 added. Used in case of DAC init failure.
 * 
 * 42    8.09.05 11:37 Chchlee
 * Comments added
 * 
 * 41    1.09.05 16:20 Chchlee
 * dlEOL... replaced by dlEOC...
 * 
 * 40    22.07.05 8:17 Chchlee
 * monitor_enableReset renamed
 * 
 * 39    21.07.05 10:00 Chchlee
 * DEBUG_FLAG_MASK moved to IO_HW_BUFFER.H
 * 
 * 38    14.07.05 9:34 Chharyt
 * logic for all alarm relays (P4LT and R1BC) changed. The alarm state
 * must be the non energized state at the relay.
 * 
 * 37    6.07.05 13:50 Chchlee
 * 
 * 36    29.06.05 8:53 Chchlee
 * Warnings generated again for event recorder. IF alarm bug fixed
 * 
 * 35    14.06.05 17:14 Chpeplu
 * in function monitor_evaluateNsdAlarms() alarm rx continuous alarm added
 * for TPE1/2
 * 
 * 34    9.06.05 19:51 Chchlee
 * WARN_Event_Recorder_block_rotation added
 * 
 * 33    6.06.05 17:08 Chpeplu
 * in function monitor_generateAlarmEvents() warning events for plc and
 * NSD1 and NSD2 added
 * 
 * 32    6.06.05 10:30 Chpeplu
 * New Alarm Events for NSD manual looptest added for Ch1 and Ch2
 * 
 * 31    3.06.05 10:32 Chmibuh
 * monitor_ifCRCerror_Reset() added
 * 
 * 30    27.05.05 9:04 Chchlee
 * adapted to new G4AI basic time slot (alarms)
 * 
 * 29    24.05.05 11:43 Chpeplu
 * record TPE1 loop test events and record TPE2 loop test events added
 * 
 * 28    23.05.05 8:05 Chchlee
 * 
 * 27    19.05.05 12:55 Chalgyg
 * ALM_HW_Piggyback Alarm added.
 * 
 * 26    18.05.05 13:20 Chmibuh
 * Re-enable all CRC alarms
 * 
 * 25    18.05.05 10:00 Chchlee
 * ALM_UC_Programm_Memory_CRC
 * 
 * 24    17.05.05 16:56 Chchlee
 * ofdmAlarms.configAlm added
 * 
 * 23    13.05.05 16:29 Chchlee
 * CRC alarms temporarily commented out
 * 
 * 22    12.05.05 18:34 Chmibuh
 * ALM_Master_Programm_Memory_CRC added
 * 
 * 21    18.04.05 10:02 Chchlee
 * Lifesign supervision suppression at startup. Bug in lifesign supervision
 * fixed
 * 
 * 20    11.04.05 16:56 Chmibuh
 * At plcHwAlarms voltage Alarms added
 * 
 * 19    10.04.05 17:29 Chmibuh
 * 
 * 18    10.04.05 13:36 Chmibuh
 * licenceInsufficientAlarm() added
 * 
 * 17    6.04.05 11:37 Chchlee
 * errorLimit for lifesign is now 1s longer than pickup time
 * 
 * 16    29.03.05 19:39 Chchlee
 * warning event removed / P4LT alarm LED corrected
 * 
 * 15    14.03.05 10:46 Chchlee
 * Skip initial pickup time for HW and Link alarms
 * 
 * 14    9.03.05 11:01 Chpeplu
 * New config used (UsedExtTimeSlots and u_NSD_AES)
 * 
 * 13    4.03.05 15:20 Chharyt
 * R1BC_CAB_AL, R1BC_RES_1 and R1BC_RES_2 moved to the right Alarm1,
 * Alarm2 and Alarm3 for R1BC
 * 
 * 12    1.03.05 11:51 Chchlee
 * lifesign activation now controlled by JTAG board jumper (debug flag)
 * 
 * 11    1.03.05 8:14 Chharyt
 * Prototyp2 changes added.
 * 
 * 10    24.02.05 18:20 Chmibuh
 * iButH_licenceErrorAlarm added
 * 
 * 9     24.02.05 11:45 Chmibuh
 * rtc_incorrectRTCDateAlarm added
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include <def21161.h>

#include "monitor.h"

#include "io_hw_buffer.h"
#include "status.h"
#include "diag.h"

#include "tdm_int_types.h"
#include "tdm_ext_types.h"
#include "configuration.h"
#include "eventrec.h"
#include "irigB.h"
#include "rtc.h"
#include "iButHigh.h"
#include "licenceCheck.h" 
#include "SPI_functions.h"
#include "MemoryCheck.h"
#include "msgHandler.h"
#include "configuration.h"
#include "R1LA_Types.h"

#include "default.h" // for FW_COMPATIBILITY_INDEX

#include "externalTdm_BusBridge.h" // using extTDMBusBridge_getPHardTimingTelegramInfoFrom/ToTDM

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define	CABINET_ALARM_RELAY		RELAY1  
#define	LINK_ALARM_RELAY		RELAY2
#define	HW_ALARM_RELAY	    	RELAY3

// Slow mute: liegt zwischen 1 und 31 gibt wie lange integriert wird
#define		SLOW_MUTE_INT_TIME				25			
#define		MAX_SLOW_MUTE_NUMBER			33

/// with the TIME_FACTOR  you have to adjust the setup and hold time 
/// of the alarm relays. You indicate the time in 100ms units.  
//  TIME_FACTOR 100/N, where N is the number of cases in your subcase
#define	TIME_FACTOR  (10.667f)

// pickup and hold time for warnings
#define	WARNING_PICKUP_TIME 100  // pickup time = 10s
#define	WARNING_HOLD_TIME   10   // hold time = 1s

#define RDY_LED_BLINKING_RATE         200 // toggle frequency of RDY LED: 2s
#define WARNING_LED_BLINKING_RATE     50  // toggle frequency of blinking waning LED: 0.5s

// defines for lifesign supervision of slave DSP
#define SLAVE_DSP_FE_ID    0
#define SLAVE_DSP_UC_ID    1
#define SLAVE_DSP_NSD_ID   2
#define SLAVE_DSP_NSK_ID   3
#define SLAVE_DSP_OFDM_ID  4
#define MAX_SLAVE_DSP_ID   4

#define ADDITIONAL_RESET_DELAY_COUNT	150		// if called in a subcase -> delay about 1.5 sec

// G4AI toggle bit in basic time slot
#define TOGGLE_BIT_G4AI					BIT5

//no toggling time to detect G4AI reset
#define NO_TOGGLING_MAX					250		// 2.34 s


/* LOCAL TYPE DECLARATIONS ***************************************************/

enum e_AlarmState { AlarmNotInitialized,
					AlarmIdle, 
                    AlarmDelay, 
					AlarmWait, 
					AlarmHold };   

typedef struct 
{
	// state fields
  	enum e_AlarmState    state;
    long	             counter;
    unsigned long	     lastAlarm;
	unsigned long	     outAlarm;

	BOOL                 infiniteHoldTime;
	BOOL                 deleteLinkAlarm;

	// configuration fields
	unsigned long        pickupTime;
	unsigned long        holdTime;

	BOOL			     manualAlarmReset;
}
t_Alarm;

typedef struct 
{
	BOOL			enabled;                 // life sign supervision is enabled for this DSP
	unsigned long   currentLifesignValue;
	unsigned long   lastLifesignValue;
	unsigned long   errors;                  // current number of errors
	unsigned long   errorLimit;
}
t_LifesignState;


/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx pointers
#define g_intTDM_RxStruct_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr)


/* LOCAL VARIABLES ***********************************************************/


static t_LifesignState g_LifesignState[MAX_SLAVE_DSP_ID + 1];

//*************************** Alarm Variables   ***************************       
static		t_Alarm		g_dlyState_plcHwAlarms;
static		t_Alarm	    g_dlyState_plcLinkAlarms;
static		t_Alarm	    g_dlyState_plcIfAlarms;
static		t_Alarm	    g_dlyState_plcWarnings;

static		t_Alarm	    g_dlyState_dataHwAlarms;
static		t_Alarm	    g_dlyState_dataLinkAlarms;
static		t_Alarm	    g_dlyState_dataIfAlarms;
static		t_Alarm	    g_dlyState_dataWarnings;

static		t_Alarm	    g_dlyState_tpe1HwAlarms;
static		t_Alarm	    g_dlyState_tpe1LinkAlarms;
static		t_Alarm		g_dlyState_tpe1LevelAlarm;
static		t_Alarm		g_dlyState_tpe1SNRAlarm;
static		t_Alarm	    g_dlyState_tpe1IfAlarms;
static		t_Alarm	    g_dlyState_tpe1Warnings;

static		t_Alarm	    g_dlyState_tpe2HwAlarms;
static		t_Alarm	    g_dlyState_tpe2LinkAlarms;
static		t_Alarm	    g_dlyState_tpe2IfAlarms;
static		t_Alarm	    g_dlyState_tpe2Warnings;


static		t_Alarm	    g_dlyState_R1BCAlarmBus1;
static		t_Alarm	    g_dlyState_R1BCAlarmBus2;
static		t_Alarm	    g_dlyState_R1BCAlarmBus3;

static      t_Alarm     g_dlyState_TDMBusAlarm;

static t_P4LT_Config         * g_p_Cfg;
static t_FastStatusTelegram  * g_pRemoteFastStatusData;
static t_P4LT_Status         * g_p_StatusData;

static unsigned long         g_nTPE;


static t_TDMTimeslotList                     g_G4AI_BasicTimeslotListForTPE1;
static t_TDMTimeslotList                     g_G4AI_BasicTimeslotListForTPE2;
static t_TDMTimeslotList                     g_O4LE_BasicTimeslotList;
static t_TDMTimeslotList                     g_O4CV_BasicTimeslotList;

static t_TDM_HardTimingTelegramInformation * g_pHardTimingInformationFromTDM;
static t_TDM_HardTimingTelegramInformation * g_pHardTimingInformationToTDM;

static BOOL g_ConfiguredBasicTimeslotArr[NO_TDM_BASIC_TIMESLOTS];

static t_AlarmsAndWarnings    g_AlarmsAndWarnings;

static t_TestPlcLinkData    * g_pTestSwitches;

static unsigned long          g_UndelayedHighLevelAlarms;

static BOOL                   g_blinkWarningLED;

static BOOL					  g_isResetEnabled; //Disable Reset in case of FW Update

static BOOL					  g_isR1LAConfigured;

/* EXTERN VARIABLES ***********************************************************/
extern volatile long *internalTDM_Rx_Ptr;

/* LOCAL FUNCTIONS ***********************************************************/


static void alarmCreate (t_Alarm      * theAlarm,
						 unsigned long  pickupTimeIn100ms,
						 unsigned long  holdTimeInIn100ms,
						 BOOL           bSkipInitialPickupTime,
						 BOOL			bManualAlarmReset);

static unsigned long alarmDelay(t_Alarm *     theAlarm,
					            unsigned long alarmPending);

static void          alarmAcknowledge(t_Alarm * theAlarm);

static BOOL          integrateSlowMute(unsigned long aplcChannel, BOOL snrAlarmSet);
static unsigned long highLevelAlarmsOf (t_AlarmsAndWarnings * pLowLevelAlarms);



/* IMPLEMENTATION ************************************************************/


  //////////////////////////////////////////////////////////////////////////
  /// diese Funktion initialisiert die Alarm-Statemachine
  /// Input: -
  /// Output: - 
  /// History: Creation 12.8. Si
  /////////////////////////////////////////////////////////////////////////				
void SEGMENT_EXT_CODE monitor_init(void)
{   
	int basicTimeSlot;
	BOOL manualAlarmResetEnabled;

	// get P4LT configuration
	g_p_Cfg                 = configuration_getPCurrentConfig();

	g_p_StatusData          = status_getPStatus();
	g_pRemoteFastStatusData = status_getRemoteFastStatus();
	g_pTestSwitches         = status_getPTestSwitches();

	g_pHardTimingInformationFromTDM = extTDMBusBridge_getPHardTimingTelegramInfoFromTDM();
	g_pHardTimingInformationToTDM   = extTDMBusBridge_getPHardTimingTelegramInfoToTDM();

	g_isResetEnabled = TRUE;

	// Extract all configured basic time slot information for alarm monitoring purposes
	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		// if a board is plugged in
		if (g_p_Cfg->system.TDM_Config[basicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK)
		{
			g_ConfiguredBasicTimeslotArr[basicTimeSlot] = TRUE;
		}

		// find the O4LEs
		if (O4LE_BOARD == (g_p_Cfg->system.TDM_Config[basicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK))
		{
			g_O4LE_BasicTimeslotList.TimeslotNumber[g_O4LE_BasicTimeslotList.nEntries++] = basicTimeSlot;
		}

		// find the O4CVs
		if (O4CV_BOARD == (g_p_Cfg->system.TDM_Config[basicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK))
		{
			g_O4CV_BasicTimeslotList.TimeslotNumber[g_O4CV_BasicTimeslotList.nEntries++] = basicTimeSlot;
		}

		// check if AES enabled
		// @Pl_ext
		if ((g_p_Cfg->ITS.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
		{
			// find the G4AIs for TPE1
			if (BIT0 == ((g_p_Cfg->ITS.u_NSD_AES.AES.UsedExtTimeSlots>>basicTimeSlot) & BIT0))
			{
				g_G4AI_BasicTimeslotListForTPE1.TimeslotNumber[g_G4AI_BasicTimeslotListForTPE1.nEntries++] = basicTimeSlot;
			}
		}
		else if ((g_p_Cfg->ITS.mode & INT_TPE_MODE_MASK) != INT_TELPROT_MODE_NONE) // NSD
		{
			// find the G4AIs for TPE1
			if (BIT0 == ((g_p_Cfg->ITS.u_NSD_AES.NSD[0].UsedExtTimeSlots>>basicTimeSlot) & BIT0))
			{
				g_G4AI_BasicTimeslotListForTPE1.TimeslotNumber[g_G4AI_BasicTimeslotListForTPE1.nEntries++] = basicTimeSlot;
			}
			// find the G4AIs for TPE2
			if (BIT0 == ((g_p_Cfg->ITS.u_NSD_AES.NSD[1].UsedExtTimeSlots>>basicTimeSlot) & BIT0))
			{
				g_G4AI_BasicTimeslotListForTPE2.TimeslotNumber[g_G4AI_BasicTimeslotListForTPE2.nEntries++] = basicTimeSlot;
			}
		}
	}

	CNT_TPE(g_p_Cfg->ITS, g_nTPE); // get the number of active TPEs

	// initialize alarm delay objects
	alarmCreate(&g_dlyState_plcHwAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_plcLinkAlarms,
		        g_p_Cfg->system.configLinkAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configLinkAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_plcIfAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_plcWarnings,
		        WARNING_PICKUP_TIME,
				WARNING_HOLD_TIME,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_dataHwAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_dataLinkAlarms,
		        g_p_Cfg->system.configLinkAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configLinkAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_dataIfAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_dataWarnings,
		        WARNING_PICKUP_TIME,
				WARNING_HOLD_TIME,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset
				
	alarmCreate(&g_dlyState_tpe1HwAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_tpe1LinkAlarms,
		        g_p_Cfg->system.configLinkAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configLinkAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	// check if manual alarm reset in AES mode is enabled
	// @Pl_ext
	if ((g_p_Cfg->ITS.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
	{
		if (g_p_Cfg->ITS.u_NSD_AES.AES.AES600_Mode & AES600_RESET_RX_ALARM)
		{
			manualAlarmResetEnabled = TRUE;
		}
		else
		{
			manualAlarmResetEnabled = FALSE;
		}
	}
	else
	{
		manualAlarmResetEnabled = FALSE;
	}

	alarmCreate(&g_dlyState_tpe1LevelAlarm,
		        g_p_Cfg->system.configLinkAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configLinkAlarmRelay >> 16,
				TRUE,						// Skip initial pickup time
				manualAlarmResetEnabled);	// Manual alarm reset

	alarmCreate(&g_dlyState_tpe1SNRAlarm,
		        g_p_Cfg->system.configLinkAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configLinkAlarmRelay >> 16,
				TRUE,						// Skip initial pickup time
				manualAlarmResetEnabled);	// Manual alarm reset


	alarmCreate(&g_dlyState_tpe1IfAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_tpe1Warnings,
		        WARNING_PICKUP_TIME,
				WARNING_HOLD_TIME,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_tpe2HwAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_tpe2LinkAlarms,
		        g_p_Cfg->system.configLinkAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configLinkAlarmRelay >> 16,
				TRUE,       // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_tpe2IfAlarms,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_tpe2Warnings,
		        WARNING_PICKUP_TIME,
				WARNING_HOLD_TIME,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset


	alarmCreate(&g_dlyState_R1BCAlarmBus1,
		        g_p_Cfg->system.R1BC_Config[0].setupAndHoldTime & MASK_16LSB,
				g_p_Cfg->system.R1BC_Config[0].setupAndHoldTime >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_R1BCAlarmBus2,
		        g_p_Cfg->system.R1BC_Config[1].setupAndHoldTime & MASK_16LSB,
				g_p_Cfg->system.R1BC_Config[1].setupAndHoldTime >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	alarmCreate(&g_dlyState_R1BCAlarmBus3,
		        g_p_Cfg->system.R1BC_Config[2].setupAndHoldTime & MASK_16LSB,
				g_p_Cfg->system.R1BC_Config[2].setupAndHoldTime >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset


	alarmCreate(&g_dlyState_TDMBusAlarm,
		        g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB,
				g_p_Cfg->system.configHWAlarmRelay >> 16,
				FALSE,      // Skip initial pickup time
				FALSE);     // Manual alarm reset

	// initialize lifesign objects 
	{
		// error limit time-out shall be 1s longer than pickup time to make sure
		// the event is recorded
	
		// 20ms units becasue the lifesign supervision runs with 100Hz / 2
		unsigned long errorLimit = (100 + 10 * g_p_Cfg->system.configHWAlarmRelay & MASK_16LSB) / 2; // in 20ms units

		g_LifesignState[SLAVE_DSP_FE_ID].errorLimit   = errorLimit;
		g_LifesignState[SLAVE_DSP_UC_ID].errorLimit   = errorLimit;
		g_LifesignState[SLAVE_DSP_NSD_ID].errorLimit  = errorLimit;
		g_LifesignState[SLAVE_DSP_NSK_ID].errorLimit  = errorLimit;
		g_LifesignState[SLAVE_DSP_OFDM_ID].errorLimit = errorLimit;
	}

	g_LifesignState[SLAVE_DSP_FE_ID].enabled   = TRUE;
	g_LifesignState[SLAVE_DSP_UC_ID].enabled   = TRUE;
	g_LifesignState[SLAVE_DSP_NSD_ID].enabled  = (g_nTPE > 0);

	// enable NSK lifesign supervision if MOD600 or any NSK modem configured
	if (g_p_Cfg->OFDM.channelBW > 0)
	{
		g_LifesignState[SLAVE_DSP_NSK_ID].enabled  = TRUE;
	}
	else
	{
		int nskIndex;

		for(nskIndex = 0; nskIndex < N_NSK_MODEMS; nskIndex++)
		{
			// if any NSK configured
			if (g_p_Cfg->NSK.modem[nskIndex].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS)
			{
				g_LifesignState[SLAVE_DSP_NSK_ID].enabled = TRUE;
				break;
			}
		}
	}

	g_LifesignState[SLAVE_DSP_OFDM_ID].enabled = (g_p_Cfg->OFDM.channelBW > 0) && 
		                                         (g_p_Cfg->OFDM.mode & OFDM_CFG_MODE_ON_OFF_BIT);

	// enable R1LA lifesign supervision if at least one LAN service is configured
	g_isR1LAConfigured = g_p_Cfg->system.serviceData.LANSettings & LAN_SERVICES_MASK;
}	

void monitor_recordRxCmdEvents(void) // called in 5.3kHz task
{
	static BOOL cmdActive1A;
	static BOOL cmdActive1B;
	static BOOL cmdActive1C;
	static BOOL cmdActive1D;
	static BOOL cmdActive1E;
	static BOOL cmdActive1F;
	static BOOL cmdActive1P;
	static BOOL cmdActive1U;

	static BOOL cmdActive2A;
	static BOOL cmdActive2B;
	static BOOL cmdActive2C;
	static BOOL cmdActive2D;
	static BOOL cmdActive2U;

	static BOOL aesCmdActive;

	static unsigned long lastInput1, lastInput2;

	unsigned long change;
	unsigned long input; 

	unsigned long aesCmd;
	static unsigned long lastAesCmd;

		
	// NSD 1
	input  = g_intTDM_RxStruct_Ptr->Channel1_OutputCommandStatusData_TS13;
	change = (input ^ lastInput1);

	if(change & EXTENDED1_G4AI_TRIP_A_BIT)     { cmdActive1A ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_A, cmdActive1A, cmdActive1A); }
	if(change & EXTENDED1_G4AI_TRIP_B_BIT)     { cmdActive1B ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_B, cmdActive1B, cmdActive1B); }
	if(change & EXTENDED1_G4AI_TRIP_C_BIT)     { cmdActive1C ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_C, cmdActive1C, cmdActive1C); }
	if(change & EXTENDED1_G4AI_TRIP_D_BIT)     { cmdActive1D ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_D, cmdActive1D, cmdActive1D); }
	if(change & EXTENDED1_G4AI_TRIP_E_BIT)	   { cmdActive1E ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_E, cmdActive1E, cmdActive1E); }
	if(change & EXTENDED1_G4AI_TRIP_F_BIT)     { cmdActive1F ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_F, cmdActive1F, cmdActive1F); }
	if(change & EXTENDED1_G4AI_TRIP_P_BIT)     { cmdActive1P ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_P, cmdActive1P, cmdActive1P); }
	if(change & EXTENDED1_G4AI_UNBLOCKING_BIT) { cmdActive1U ^= TRUE; eventRec_recordEvent(NSD1_RX_CMD_U, cmdActive1U, cmdActive1U); }

	// AES commands
	if (change & EXTENDED1_G4AI_AECS_5BIT_MASK)		// only one command at once possible, but two or more in a row 
	{
		aesCmd = (((input >> 11) & 0x0000001f) + (AES_RX_CMD_1 - 1));	// AES cmd number + offset for AES cmd event
		if (aesCmdActive)
		{
			if (aesCmd == (AES_RX_CMD_1 - 1))	// no new AES cmd received
			{
				aesCmdActive = FALSE;
				eventRec_recordEvent(lastAesCmd, aesCmdActive, aesCmdActive);	// reset last AES cmd
			}
			else	// new AES cmd received
			{
				eventRec_recordEvent(lastAesCmd, 0, 0);						// reset last AES cmd
				eventRec_recordEvent(aesCmd, aesCmdActive, aesCmdActive);	// set new AES cmd
			}

		}
		else	// new AES cmd received
		{
			aesCmdActive = TRUE;
			eventRec_recordEvent(aesCmd, aesCmdActive, aesCmdActive);	// set new AES cmd
		}

		lastAesCmd = aesCmd;
	}

	lastInput1 = input;

	// NSD 2
	input  = g_intTDM_RxStruct_Ptr->Channel2_OutputCommandStatusData_TS15;
	change = (input ^ lastInput2);

	lastInput2 = input;

	if(change & EXTENDED1_G4AI_TRIP_A_BIT)     { cmdActive2A ^= TRUE; eventRec_recordEvent(NSD2_RX_CMD_A, cmdActive2A, cmdActive2A); }
	if(change & EXTENDED1_G4AI_TRIP_B_BIT)     { cmdActive2B ^= TRUE; eventRec_recordEvent(NSD2_RX_CMD_B, cmdActive2B, cmdActive2B); }
	if(change & EXTENDED1_G4AI_TRIP_C_BIT)     { cmdActive2C ^= TRUE; eventRec_recordEvent(NSD2_RX_CMD_C, cmdActive2C, cmdActive2C); }
	if(change & EXTENDED1_G4AI_TRIP_D_BIT)     { cmdActive2D ^= TRUE; eventRec_recordEvent(NSD2_RX_CMD_D, cmdActive2D, cmdActive2D); }
	if(change & EXTENDED1_G4AI_UNBLOCKING_BIT) { cmdActive2U ^= TRUE; eventRec_recordEvent(NSD2_RX_CMD_U, cmdActive2U, cmdActive2U); }
}


void monitor_recordTxCmdEvents(void)  // called in 5.3kHz task
{

	static BOOL cmdActive1A;
	static BOOL cmdActive1B;
	static BOOL cmdActive1C;
	static BOOL cmdActive1D;
	static BOOL cmdActive1E;
	static BOOL cmdActive1F;
	static BOOL cmdActive1P;
	static BOOL cmdActive1S;

	static BOOL cmdActive2A;
	static BOOL cmdActive2B;
	static BOOL cmdActive2C;
	static BOOL cmdActive2D;
	static BOOL cmdActive2S;

	static BOOL aesCmdActive[NO_OF_AES_COMMANDS];

	static unsigned long lastInput1, lastInput2;

	int i;
	unsigned long change;
	unsigned long input;

	unsigned long bitMask;
	unsigned long aesTxCmdEvent;

	// NSD1
	input  = g_intTDM_RxStruct_Ptr->Channel1_InputCommandStatusData_TS12;
	change = (input ^ lastInput1);

	lastInput1 = input;

	if(change & EXTENDED1_G4AI_TRIP_A_BIT) { cmdActive1A ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_A, cmdActive1A, cmdActive1A); }
	if(change & EXTENDED1_G4AI_TRIP_B_BIT) { cmdActive1B ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_B, cmdActive1B, cmdActive1B); }
	if(change & EXTENDED1_G4AI_TRIP_C_BIT) { cmdActive1C ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_C, cmdActive1C, cmdActive1C); }
	if(change & EXTENDED1_G4AI_TRIP_D_BIT) { cmdActive1D ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_D, cmdActive1D, cmdActive1D); }
	if(change & EXTENDED1_G4AI_TRIP_E_BIT) { cmdActive1E ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_E, cmdActive1E, cmdActive1E); }
	if(change & EXTENDED1_G4AI_TRIP_F_BIT) { cmdActive1F ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_F, cmdActive1F, cmdActive1F); }
	if(change & EXTENDED1_G4AI_TRIP_P_BIT) { cmdActive1P ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_P, cmdActive1P, cmdActive1P); }
	if(change & EXTENDED1_G4AI_START_BIT)  { cmdActive1S ^= TRUE; eventRec_recordEvent(NSD1_TX_CMD_S, cmdActive1S, cmdActive1S); }

	// AES commands
	bitMask = EXTENDED1_G4AI_AES_TX_CMD_1_BIT;
	aesTxCmdEvent = AES_TX_CMD_1;

	for (i = 0; i < NO_OF_AES_COMMANDS; i++)
	{
		if(change & bitMask)   { aesCmdActive[i] ^= TRUE; eventRec_recordEvent(aesTxCmdEvent, aesCmdActive[i], aesCmdActive[i]); }
		aesTxCmdEvent++;
		bitMask >>= 1;
	}

/*	if(change & EXTENDED1_G4AI_AES_TX_CMD_1_BIT)   { aesCmdActive[0] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_1, aesCmdActive[0], aesCmdActive[0]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_2_BIT)   { aesCmdActive[1] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_2, aesCmdActive[1], aesCmdActive[1]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_3_BIT)   { aesCmdActive[2] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_3, aesCmdActive[2], aesCmdActive[2]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_4_BIT)   { aesCmdActive[3] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_4, aesCmdActive[3], aesCmdActive[3]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_5_BIT)   { aesCmdActive[4] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_5, aesCmdActive[4], aesCmdActive[4]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_6_BIT)   { aesCmdActive[5] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_6, aesCmdActive[5], aesCmdActive[5]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_7_BIT)   { aesCmdActive[6] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_7, aesCmdActive[6], aesCmdActive[6]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_8_BIT)   { aesCmdActive[7] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_8, aesCmdActive[7], aesCmdActive[7]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_9_BIT)   { aesCmdActive[8] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_9, aesCmdActive[8], aesCmdActive[8]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_10_BIT)  { aesCmdActive[9] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_10, aesCmdActive[9], aesCmdActive[9]); }

	if(change & EXTENDED1_G4AI_AES_TX_CMD_11_BIT)  { aesCmdActive[10] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_11, aesCmdActive[10], aesCmdActive[10]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_12_BIT)  { aesCmdActive[11] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_12, aesCmdActive[11], aesCmdActive[11]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_13_BIT)  { aesCmdActive[12] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_13, aesCmdActive[12], aesCmdActive[12]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_14_BIT)  { aesCmdActive[13] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_14, aesCmdActive[13], aesCmdActive[13]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_15_BIT)  { aesCmdActive[14] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_15, aesCmdActive[14], aesCmdActive[14]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_16_BIT)  { aesCmdActive[15] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_16, aesCmdActive[15], aesCmdActive[15]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_17_BIT)  { aesCmdActive[16] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_17, aesCmdActive[16], aesCmdActive[16]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_18_BIT)  { aesCmdActive[17] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_18, aesCmdActive[17], aesCmdActive[17]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_19_BIT)  { aesCmdActive[18] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_19, aesCmdActive[18], aesCmdActive[18]); }
	if(change & EXTENDED1_G4AI_AES_TX_CMD_20_BIT)  { aesCmdActive[19] ^= TRUE; eventRec_recordEvent(AES_TX_CMD_20, aesCmdActive[19], aesCmdActive[19]); }
*/
	// NSD2
	input  = g_intTDM_RxStruct_Ptr->Channel2_InputCommandStatusData_TS14;
	change = (input ^ lastInput2);

	lastInput2 = input;

	if(change & EXTENDED1_G4AI_TRIP_A_BIT) { cmdActive2A ^= TRUE; eventRec_recordEvent(NSD2_TX_CMD_A, cmdActive2A, cmdActive2A); }
	if(change & EXTENDED1_G4AI_TRIP_B_BIT) { cmdActive2B ^= TRUE; eventRec_recordEvent(NSD2_TX_CMD_B, cmdActive2B, cmdActive2B); }
	if(change & EXTENDED1_G4AI_TRIP_C_BIT) { cmdActive2C ^= TRUE; eventRec_recordEvent(NSD2_TX_CMD_C, cmdActive2C, cmdActive2C); }
	if(change & EXTENDED1_G4AI_TRIP_D_BIT) { cmdActive2D ^= TRUE; eventRec_recordEvent(NSD2_TX_CMD_D, cmdActive2D, cmdActive2D); }
	if(change & EXTENDED1_G4AI_START_BIT)  { cmdActive2S ^= TRUE; eventRec_recordEvent(NSD2_TX_CMD_S, cmdActive2S, cmdActive2S); }
}


static void SEGMENT_EXT_CODE alarmCreate (   t_Alarm      * theAlarm,
											 unsigned long  pickupTimeIn100ms,
											 unsigned long  holdTimeIn100ms,
											 BOOL           bSkipInitialPickupTime,
						                     BOOL			bManualAlarmReset)
{
	theAlarm->pickupTime = TIME_FACTOR * (float)pickupTimeIn100ms;
	theAlarm->holdTime   = TIME_FACTOR * (float)holdTimeIn100ms;

	theAlarm->state = bSkipInitialPickupTime? AlarmDelay: AlarmIdle;

	theAlarm->manualAlarmReset = bManualAlarmReset;
}

static void SEGMENT_EXT_CODE alarmAcknowledge(t_Alarm * theAlarm)
{
	theAlarm->deleteLinkAlarm = TRUE;
}

  //////////////////////////////////////////////////////////////////////////
  /// diese Funktion erzeugt den Delay und den Hold fuer die Alarme
  /// der Datenaustausch erfolgt via Speicher (Status und Konfig)
  /// Input: alarmPending: bol signal, ob dieser Alarm anliegt
  ///		 theAlarm: die noetigen Daten
  /// Output: verzoegerter/gehaltener Alarm 0/1  
  ///			
  /////////////////////////////////////////////////////////////////////////
static unsigned long SEGMENT_EXT_CODE alarmDelay(t_Alarm * theAlarm, unsigned long alarmPending)
{
	switch(theAlarm->state)
	{
		case AlarmIdle:
			if (alarmPending) // i.e. any bit is set
			{
				if (theAlarm->manualAlarmReset > 0)			
				{
					theAlarm->infiniteHoldTime = TRUE;		
					theAlarm->deleteLinkAlarm  = FALSE;		
				}
				else									
				{
					theAlarm->infiniteHoldTime = FALSE;		
				}

				theAlarm->state   = AlarmDelay;
				theAlarm->counter = theAlarm->pickupTime;
			}
			return 0;

		case AlarmDelay:
			if (alarmPending) // if alarm still pending
			{
				if ((--theAlarm->counter) < 0) // decrement delay counter
				{
					theAlarm->state    = AlarmWait; // alarm long enough 
					theAlarm->outAlarm = alarmPending;
					theAlarm->counter  = theAlarm->pickupTime; 
				}
			}
			else 
			{   
				theAlarm->state = AlarmIdle; // Alarm too short -> back to state idle
			}
			return 0;
			
		case AlarmWait:
			theAlarm->counter--;

			if (alarmPending) // alarm still pending 
			{ 
				if(theAlarm->counter < 0) // re-sample input after pickup time
				{
					theAlarm->outAlarm = alarmPending;
				    theAlarm->counter = theAlarm->pickupTime; 
				}
			}
			else
			{   // alarm gone -> hold it
				theAlarm->counter = theAlarm->holdTime; // alarm hold
				theAlarm->state = AlarmHold;
			}

			return theAlarm->outAlarm;

		case AlarmHold:

			if ((--theAlarm->counter) < 0)
			{
				if (theAlarm->infiniteHoldTime && (!(theAlarm->deleteLinkAlarm))) // Fall unendl. Holdzeit
				{
					theAlarm->state = AlarmWait;		
				}
				else									
				{
					theAlarm->state = AlarmIdle; // end of hold time
				}									
			}

			if (alarmPending)
			{
				theAlarm->state = AlarmWait;	
			}

			return theAlarm->outAlarm;
		
		default:
			ASSERT(FALSE);
			return 0; // avoids compiler warning
	}
}


//////////////////////////////////////////////////////////////////////////    
/// diese Funktion integriert (addiert einsen) die letzten 32 binären Werte des SNRs und bestimmt
/// damit LowMute. Die 32 Werte sind in einem longeger abgespeichert
/// Input: SNR ON/OFF
/// Output: long: wurde die nötige Schwelle überschritten 
/// History: 	Creation 16.10. Si                                      
/////////////////////////////////////////////////////////////////////////		
	    
static BOOL SEGMENT_EXT_CODE integrateSlowMute(unsigned long aplcChannel, BOOL snrAlarmSet)
{
	static long sumCh[N_APLC_CHANNELS];
	
	(snrAlarmSet && (sumCh[aplcChannel] < MAX_SLOW_MUTE_NUMBER)) ? sumCh[aplcChannel]++ : sumCh[aplcChannel]--; 

	if(sumCh[aplcChannel] < 0) 
	{
		sumCh[aplcChannel] = 0;
	}
	
	return	(sumCh[aplcChannel] > SLOW_MUTE_INT_TIME);		   	
}                                        
                                     
	
void SEGMENT_EXT_CODE monitor_evaluateLifesigns(void)
{
    static int  startupCounter;
	static BOOL toggle;

	unsigned int lifesignIx;


	if (startupCounter < 100) // suppress lifesign supervision at startup for 1s
	{
		startupCounter++;
		return;
	}


	// activate lifesign supervision only with half the frequency of the lifesign increment frequency
	if(toggle = !toggle)      
	{
		return;
	}


	// clear all lifesign alarms
	g_AlarmsAndWarnings.plcHwAlarms           &= ~(ALM_FE_Life_Sign  | ALM_UC_Life_Sign);
	g_AlarmsAndWarnings.dataAlarmsAndWarnings &= ~(ALM_NSK_Life_Sign | ALM_MOD_Life_Sign);
	g_AlarmsAndWarnings.tpe1AlarmsAndWarnings &= ~ALM_NSD_Life_Sign;
	g_AlarmsAndWarnings.tpe2AlarmsAndWarnings &= ~ALM_NSD_Life_Sign;

	// lifesign supervision

	// fetch currentLifesignValue
	g_LifesignState[SLAVE_DSP_FE_ID].currentLifesignValue   = g_intTDM_RxStruct_Ptr->FE_DSP_AlarmStatusData_TS08.lifesign;
	g_LifesignState[SLAVE_DSP_UC_ID].currentLifesignValue   = g_intTDM_RxStruct_Ptr->UpConverter_DSP_AlarmStatusData_TS20.lifesign;
	g_LifesignState[SLAVE_DSP_NSD_ID].currentLifesignValue  = g_intTDM_RxStruct_Ptr->NSD_DSP_AlarmStatusData_TS10.lifesign;
	g_LifesignState[SLAVE_DSP_NSK_ID].currentLifesignValue  = g_intTDM_RxStruct_Ptr->NSK600_DSP_AlarmStatusData_TS18.lifesign;
	g_LifesignState[SLAVE_DSP_OFDM_ID].currentLifesignValue = g_intTDM_RxStruct_Ptr->OFDM_DSP_AlarmStatusData_TS16.lifesign;


	for (lifesignIx = 0; lifesignIx < MAX_SLAVE_DSP_ID+1; lifesignIx++)
	{
		if ( g_LifesignState[lifesignIx].enabled &&
			(g_LifesignState[lifesignIx].lastLifesignValue == g_LifesignState[lifesignIx].currentLifesignValue))
		{   
			// lifesign has not changed --> set alarm immediately
			switch (lifesignIx)
			{
				case SLAVE_DSP_FE_ID:
					g_AlarmsAndWarnings.plcHwAlarms |= ALM_FE_Life_Sign; 
					break;
				case SLAVE_DSP_UC_ID:
					g_AlarmsAndWarnings.plcHwAlarms |= ALM_UC_Life_Sign; 
					break;
				case SLAVE_DSP_NSD_ID:
					g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Life_Sign; 

					if (g_nTPE > 1)
						g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Life_Sign; 

					break;
				case SLAVE_DSP_NSK_ID:
					g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_NSK_Life_Sign;
					break;
				case SLAVE_DSP_OFDM_ID:
					g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_MOD_Life_Sign;
					break;
				default: ASSERT(FALSE);
			}

			// check if error limit has been reached
			if (g_LifesignState[lifesignIx].errors++ > g_LifesignState[lifesignIx].errorLimit)
			{
				if (!isInDebugMode()) // do not perform reset if debug flag is active
				{
					resetP4LT();  
				}
			}
		}
		else
		{
			g_LifesignState[lifesignIx].errors = 0; // reset error counter
		}
		
		// update lifesign
		g_LifesignState[lifesignIx].lastLifesignValue = g_LifesignState[lifesignIx].currentLifesignValue;
	}
}

#define LOCAL_PLC_HW_ALM_MASK (ALM_M12V_Underv	  		      | \
							   ALM_M12V_Overv  		  	      | \
							   ALM_P12V_Underv			      | \
							   ALM_P12V_Overv			      | \
							   ALM_P5V_Underv			      | \
							   ALM_P5V_Overv			      | \
							   ALM_P3_3V_Underv			      | \
							   ALM_P3_3V_Overv			      | \
							   ALM_P1_8V_Underv			      | \
							   ALM_P1_8V_Overv			      | \
							   ALM_Plut_out_3_RF_Hybrid       | \
							   ALM_Plut_out_4_Rx_Filter       | \
							   ALM_Tx_RF                      | \
                               ALM_RF_Rx_overflow             | \
							   ALM_Master_Programm_Memory_CRC | \
							   ALM_UC_Programm_Memory_CRC     | \
							   ALM_FE_Programm_Memory_CRC     | \
                               ALM_Event_Recorder_disabled	  | \
							   ALM_PSU1_failed				  | \
							   ALM_PSU2_failed)

void SEGMENT_EXT_CODE monitor_evaluatePlcHwAlarms(void)
{
	static long dacInitFailureCounter;
	BOOL bDacInitFailure;
	//static BOOL bDacInitFailureEventFlag;  // debug code


	// clear locally generated PLC HW alarms
	g_AlarmsAndWarnings.plcHwAlarms &= ~LOCAL_PLC_HW_ALM_MASK;

	/////////////////////////////////////////////////////////////////////////////////////////
	/// HW Alarms from I/O buffer
	/////////////////////////////////////////////////////////////////////////////////////////

	if (IO_BUFFER_READ & PLUG_OUT3)  
	{
		if (g_p_Cfg->system.equipmentType == EQ_TYPE_ETL600_R3)
		{
			g_AlarmsAndWarnings.plcHwAlarms |= ALM_Plut_out_3_RF_Hybrid;
		}
	}
	else if ((g_p_Cfg->system.equipmentType != EQ_TYPE_ETL600_R3) &&
			 (g_p_Cfg->system.hwSettings.powerSupplyVoltage & PS_REDUNDANT_MODULE_PLUGGED))
	{
		g_AlarmsAndWarnings.plcHwAlarms |= ALM_PSU2_failed;
	}

	if (!(IO_BUFFER_READ & TXRF_INPUT) && (g_p_Cfg->system.equipmentType != EQ_TYPE_ETL600_R3))
	{
		g_AlarmsAndWarnings.plcHwAlarms |= ALM_PSU1_failed;
	}

	if (!(IO_BUFFER_READ & TXRF_AL)) g_AlarmsAndWarnings.plcHwAlarms |= ALM_Tx_RF;

	
    if ((IO_BUFFER_READ & PLUG_OUT4) && // no rx filter plugged
		 (g_p_Cfg->system.systemSettings & SYSTEM_SETTINGS_ENABLE_RX_FILTER_MONITORING))
	{	
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_Plut_out_4_Rx_Filter;
	}

	if (g_intTDM_RxStruct_Ptr->FE_DSP_AlarmStatusData_TS08.rxRfOverflow)
	{	
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_RF_Rx_overflow;
	}

	if (g_intTDM_RxStruct_Ptr->FE_DSP_AlarmStatusData_TS08.progMemCRCErr)
	{	
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_FE_Programm_Memory_CRC;
	}

	if (g_intTDM_RxStruct_Ptr->UpConverter_DSP_AlarmStatusData_TS20.progMemCRCErr)
	{	
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_UC_Programm_Memory_CRC;
	}


	if (!eventRec_isEnabled())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_Event_Recorder_disabled;
	}

	if(memoryCheckDSP_CRC_Alarm())
	{
		g_AlarmsAndWarnings.plcHwAlarms |= ALM_Master_Programm_Memory_CRC;
	}

	if ((iButH_licenceErrorAlarm()) || (licenceInsufficientAlarm()))
	{
		g_AlarmsAndWarnings.plcHwAlarms |= ALM_License;
	}
	if (spiFuncM12V_UndervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_M12V_Underv;
	}
	if (spiFuncM12V_OvervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_M12V_Overv;
	}
	if (spiFuncP12V_UndervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P12V_Underv;
	}
	if (spiFuncP12V_OvervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P12V_Overv;
	}
	if (spiFuncP5V_UndervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P5V_Underv;
	}
	if (spiFuncP5V_OvervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P5V_Overv;
	}
	if (spiFuncP3_3V_UndervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P3_3V_Underv;
	}
	if (spiFuncP3_3V_OvervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P3_3V_Overv;
	}
	if (spiFuncP1_8V_UndervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P1_8V_Underv;
	}
	if (spiFuncP1_8V_OvervAlarm())
	{
		 g_AlarmsAndWarnings.plcHwAlarms |= ALM_P1_8V_Overv;
	}

	if (g_intTDM_RxStruct_Ptr->UpConverter_DSP_AlarmStatusData_TS20.dacInitFailure)
	{	
		 bDacInitFailure = TRUE;
	}

	// reset board after 100 successive DAC failure reports
	if (bDacInitFailure /*&& !bDacInitFailureEventFlag*/) // debug code
	{
		dacInitFailureCounter++;
		if (dacInitFailureCounter >= 100)
		{
			//eventRec_recordEvent(ALARM_EVENT_DEBUG2, 0, FALSE); // for debugging, this event can be produced
			//bDacInitFailureEventFlag = TRUE; 

			if (!isInDebugMode())
			{
				resetP4LT();
			}
		}
	}
	else 
	{
		dacInitFailureCounter = 0;
	}
}


void SEGMENT_EXT_CODE monitor_evaluatePlcLinkAlarms(void)
{	
    // *************************************************
	// PLC link alarms
    // *************************************************

	// front end DSP alarms
	t_FE_Alarm feAlarms = g_intTDM_RxStruct_Ptr->FE_DSP_AlarmStatusData_TS08;

	// NSD events
	BOOL enableAlarmsFromTPE1 = !g_intTDM_RxStruct_Ptr->NSD_Events_TS83.TPE1_Suppress_Alarms;
	BOOL enableAlarmsFromTPE2 = !g_intTDM_RxStruct_Ptr->NSD_Events_TS83.TPE2_Suppress_Alarms;

	// clear all PLC link alarms (they are all generated in this function)
	g_AlarmsAndWarnings.plcLinkAlarms = 0;

	// suppress FE alarms if FE lifesign stopped counting
	if (!(g_AlarmsAndWarnings.plcHwAlarms & ALM_FE_Life_Sign))
	{
		// channel 1 alarms
  		if (enableAlarmsFromTPE1)
		{
			// slow mute
			if(integrateSlowMute(APLC_CH1, 
								 feAlarms.pilotSNRAlarmCh1)) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Slow_Mute_Ch1;			 	
			// AGC not ok
			if (feAlarms.agcFailCh1)         g_AlarmsAndWarnings.plcLinkAlarms |= ALM_AGC_fail_Ch1;

			// rx pilot not ok
			if (feAlarms.rxPilotFailCh1)     g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Rx_pilot_fail_Ch1;

			// rx pilot level alarm
			if (feAlarms.pilotLevelAlarmCh1) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Pilot_level_Ch1;
		}

		// channel 2 alarms
		if (enableAlarmsFromTPE2)
		{
			// slow mute
			if(integrateSlowMute(APLC_CH2, 
								 feAlarms.pilotSNRAlarmCh2)) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Slow_Mute_Ch2;			 	
			// AGC not ok
			if (feAlarms.agcFailCh2)         g_AlarmsAndWarnings.plcLinkAlarms |= ALM_AGC_fail_Ch2;

			// rx pilot not ok
			if (feAlarms.rxPilotFailCh2)     g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Rx_pilot_fail_Ch2;

			// rx pilot level alarm
			if (feAlarms.pilotLevelAlarmCh2) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Pilot_level_Ch2;
		}

		// channel 3 alarms
		if(integrateSlowMute(APLC_CH3, 
							 feAlarms.pilotSNRAlarmCh3)) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Slow_Mute_Ch3;			 	

		// AGC not ok
		if (feAlarms.agcFailCh3)         g_AlarmsAndWarnings.plcLinkAlarms |= ALM_AGC_fail_Ch3;

		// rx pilot not ok
		if (feAlarms.rxPilotFailCh3)     g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Rx_pilot_fail_Ch3;

		// rx pilot level alarm
		if (feAlarms.pilotLevelAlarmCh3) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_Pilot_level_Ch3;


		// AFC (only in APLC channel 1)
		if (enableAlarmsFromTPE1 && enableAlarmsFromTPE2)
		{
			if (feAlarms.afcFail) g_AlarmsAndWarnings.plcLinkAlarms |= ALM_AFC_fail;
		}
	}
}

void SEGMENT_EXT_CODE monitor_evaluatePlcIfAlarmsAndWarnings(void)
{	
    // *************************************************
	// PLC interface alarms and warnings
    // *************************************************
	// clear all interface alarms and warnings but WARN_O4LE_board and WARN_O4CV_board which are handled in "monitor_checkTDMBus"
	g_AlarmsAndWarnings.plcIfAlarmsAndWarnings &= (WARN_O4LE_board | WARN_O4CV_board); 

	if (!(IO_BUFFER_READ & WARNING_IN)) g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Ext_in;

	if ((IO_BUFFER_READ & TXRF_INPUT) && (g_p_Cfg->system.equipmentType == EQ_TYPE_ETL600_R3))
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Dummyload_plugged;
	}

	// compare local FW revision with remote 
	if ((FW_COMPATIBILITY_INDEX != g_pRemoteFastStatusData[APLC_CH1].fwCompatibilityIndex) &&
         // "0" meaning remote data was not received
		(0                      != g_pRemoteFastStatusData[APLC_CH1].fwCompatibilityIndex)    )
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Different_Fw_revision;	
	}

	// PLC link Test Warnings: check if any test bit is set
	if (g_pTestSwitches->testSwitches & TEST_SWITCHES_BIT_MASK)
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Any_test_switch_active;
	}

	// check if local RF loopback is on (rel. 4 amplifier)
	if (g_pTestSwitches->testSwitches & TEST_SWITCH_LOCAL_RF_LOOPBACK_ON)
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Local_RF_loopback_on;
	}

	// check if channel measurement is running
	if (g_p_StatusData->system.deviceState & CH_MEASUREMENT_RUNNING)
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Equalizer_action;
	}

	if (msgHandler_equipmentIsNotInNormalOpMode())
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Not_in_normal_operational_mode;
	}

	if (irigB_signalLostAlarm()) 
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= ALM_IRIG_signal_lost;
	}

	if ((irigB_incorrectRTCDateAlarm()) || (rtc_incorrectRTCDateAlarm())) 
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= ALM_Incorrect_RTC_date;
	}

	if (dlEOC_DataTxWarning())
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_No_EOC_data_tx_Ch1;
	}

	if (dlEOC_DataRxWarning())
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_No_EOC_data_rx_Ch1;
	}

	if (dlEOC_DataWarning())
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_EOC_Ch1;
	}

	if (configuration_isTemporary())
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Temporary_configuration_active;
	}

	if (eventRec_blockRotationIsOngoing())
	{
		g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_Event_Recorder_block_rotation;
	}
}

void SEGMENT_EXT_CODE monitor_evaluateDataAlarmsAndWarnings(void)
{
    // *************************************************
	// DATA  alarms and warnings
    // *************************************************

	t_NSK_Alarm  nskAlarms  = g_intTDM_RxStruct_Ptr->NSK600_DSP_AlarmStatusData_TS18;
	t_OFDM_Alarm ofdmAlarms = g_intTDM_RxStruct_Ptr->OFDM_DSP_AlarmStatusData_TS16;

	// clear all DATA alarms and warnings except MOD and NSK lifesign
	g_AlarmsAndWarnings.dataAlarmsAndWarnings &= (ALM_NSK_Life_Sign | ALM_MOD_Life_Sign);

	// suppress NSK alarms if NSK lifesign stopped counting
	if (!(g_AlarmsAndWarnings.dataAlarmsAndWarnings & ALM_NSK_Life_Sign))
	{
		if(nskAlarms.FPGALoadFailure) g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_FPGA_not_operational;
		if(nskAlarms.SubprintAlarm)	  g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_HW_Piggyback;
		if(nskAlarms.progMemCRCErr)   g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_NSK_Programm_Memory_CRC;

		if(nskAlarms.LinkAlm1)        g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_NSK1;
		if(nskAlarms.LinkAlm2)        g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_NSK2;
		if(nskAlarms.LinkAlm3)        g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_NSK3;
		if(nskAlarms.LinkAlm4)        g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_NSK4;

		if(nskAlarms.DCDAlm1)         g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_DCD_NSK1;
		if(nskAlarms.DCDAlm2)         g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_DCD_NSK2;
		if(nskAlarms.DCDAlm3)         g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_DCD_NSK3;
		if(nskAlarms.DCDAlm4)         g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_DCD_NSK4;

		if(nskAlarms.JabberTmo1)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT1;
		if(nskAlarms.JabberTmo2)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT2;
		if(nskAlarms.JabberTmo3)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT3;
		if(nskAlarms.JabberTmo4)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT4;
		if(nskAlarms.JabberTmo5)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT5;
		if(nskAlarms.JabberTmo6)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT6;

		if (nskAlarms.muxLinkAlarm && (OFDM_SYNCSTATE_V11_sync == ofdmAlarms.ofdm_syncstate))
			                          g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_MUX;

		if(nskAlarms.G703LOSAlarm)	  g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_INTERFACE_PORT10;

		if(nskAlarms.transitNoSync)   g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_TRANSIT_NO_SYNC;
		if(nskAlarms.transitMissingRxData) g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_TRANSIT_MISSING_RX_DATA;

		if(nskAlarms.LANAlarm)		  g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LAN;
		if(nskAlarms.LANWarning)	  g_AlarmsAndWarnings.dataAlarmsAndWarnings |= WARN_LAN;

	// The "LANNotBooted" is a kind of life sign - it toggles with a period of 200 ms
	// repetiton rate of this function = 0.1067 kHz

		if (g_isR1LAConfigured)
		{
		    static int  startupCounter;

			if (startupCounter < 1000) // suppress lifesign supervision at startup for 10s
			{
				startupCounter++;
			}
			else
			{
				static int  timeOut;
				static BOOL lastLifesign;

				// get current lifesign value
				BOOL currentLifesign = nskAlarms.LANNotBooted;

				if (lastLifesign == currentLifesign)
				{	// the lifesign has not toggled, check for how long

					if (timeOut < 3 * R1LA_LIFE_SIGN_TOGGLE_PERIOD) // 600ms
					{
						timeOut++;
					}
					else // time-out
					{
						// set alarm
						g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_R1LA_Life_Sign;
					}
				}
				else // the lifesign has toggled
				{
					// (clearing of alarm is done at the beginning of this function)
					timeOut = 0;
					lastLifesign = currentLifesign;
				}
			}
		}
	}

	// suppress MOD600 alarms if...  
	if (!(g_AlarmsAndWarnings.dataAlarmsAndWarnings & ALM_MOD_Life_Sign) &&    // ...MOD600 lifesign stopped counting or
		 (g_p_Cfg->OFDM.mode & OFDM_CFG_MODE_ON_OFF_BIT)                    )  //    MOD600 off
	{
		if(ofdmAlarms.syncAlm)        g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_OFDM_SYNC;
		if(ofdmAlarms.pilotAlm)       g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_LINK_OFDM_PILOT;
		if(ofdmAlarms.configAlm)      g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_OFDM_Config;
		// not implemented in R1
		//	if(ofdmAlarms.progMemCRCErr)  g_AlarmsAndWarnings.dataAlarmsAndWarnings |= ALM_MOD_Programm_Memory_CRC;
	}
}  

void SEGMENT_EXT_CODE monitor_evaluateNsdAlarms(void)
{
	t_NSD_Alarm    nsdAlarms   = g_intTDM_RxStruct_Ptr->NSD_DSP_AlarmStatusData_TS10;
	t_NSD_Events   nsdEvents   = g_intTDM_RxStruct_Ptr->NSD_Events_TS83;

	static BOOL bLastState_TPE1LooptestSent; 
	static BOOL bLastState_TPE1LooptestFailed; 
	static BOOL bLastState_TPE1LooptestReplied;
	static BOOL bLastState_TPE1ManualLooptestSent; 
	static BOOL bLastState_TPE1ManualLooptestFailed;

	static BOOL bLastState_TPE2LooptestSent; 
	static BOOL bLastState_TPE2LooptestFailed; 
	static BOOL bLastState_TPE2LooptestReplied; 
	static BOOL bLastState_TPE2ManualLooptestSent; 
	static BOOL bLastState_TPE2ManualLooptestFailed; 

	if (0 == g_nTPE) return; // no TPE configured
	
	// suppress NSD600 alarms if NSD600 lifesign stopped counting
	if (g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & ALM_NSD_Life_Sign) return;

	if (g_nTPE > 0) // at least TPE1 configured
	{
		//
		// record TPE1 loop test events
		//
		// record LT sent event
		if (nsdEvents.TPE1_LT_Sent && !bLastState_TPE1LooptestSent)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE1_LOOPTEST_SENT,0,FALSE);    
		}

		bLastState_TPE1LooptestSent = nsdEvents.TPE1_LT_Sent;
		//
		// record LT replied event
		if (nsdEvents.TPE1_LT_Reflected && !bLastState_TPE1LooptestReplied)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE1_LOOPTEST_REPLIED,0,FALSE);    
		}

		bLastState_TPE1LooptestReplied = nsdEvents.TPE1_LT_Reflected;
		//
		// record LT failed event
		if (nsdEvents.TPE1_LT_Failed && !bLastState_TPE1LooptestFailed)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE1_LOOPTEST_FAILED,0,FALSE);    
		}

		bLastState_TPE1LooptestFailed = nsdEvents.TPE1_LT_Failed;

		// record Manual LT sent event
		if (nsdEvents.TPE1_Manual_LT_Sent && !bLastState_TPE1ManualLooptestSent)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE1_MANUAL_LOOPTEST_SENT,0,FALSE);    
		}

		bLastState_TPE1ManualLooptestSent = nsdEvents.TPE1_Manual_LT_Sent;
		//
		// record Manual LT failed event
		if (nsdEvents.TPE1_Manual_LT_Failed && !bLastState_TPE1ManualLooptestFailed)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE1_MANUAL_LOOPTEST_FAILED,0,FALSE);    
		}

		bLastState_TPE1ManualLooptestFailed = nsdEvents.TPE1_Manual_LT_Failed;
		//
		//

		// clear all TPE1 alarms and warnings except NSD lifesign
		g_AlarmsAndWarnings.tpe1AlarmsAndWarnings &= ALM_NSD_Life_Sign;

		// HW alarms
		if(nsdAlarms.progMemCRCErr)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Programm_Memory_CRC;

		// TPE1 alarms and warnings
		if(nsdAlarms.tpe1RxLowLevelAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Rx_Low_Level;

		if(nsdAlarms.tpe1RxHighLevelAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Rx_High_Level;

		if(nsdAlarms.tpe1SNRAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Rx_SNR;

		if(nsdAlarms.tpe1NoTripAndNoGuardAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_No_Trip_and_No_Guard;

		if(nsdAlarms.tpe1TripAndGuardAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Trip_and_Guard;

		if(nsdAlarms.tpe1LoopTestAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Loop_Test;

		if(nsdAlarms.tpe1OutputSetToPredefinedStateAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Output_Set_To_Predefined_State;

		if(nsdAlarms.tpe1RxContinuousAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_G4AI_Rx_Continuous;

		if(nsdAlarms.tpe1CommandOutputsBlockedAlarm)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_Command_Outputs_Blocked;

		if(nsdAlarms.tpe1TestCfgActiveWarning)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= WARN_NSD_Test_Config_Active;

		if(nsdAlarms.tpe1LocalLoopActiveWarning)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= WARN_NSD_Local_Loop_Active;

		if(nsdAlarms.tpe1LocalLoopTestWarning)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= WARN_NSD_Loop_Test;

		if(nsdAlarms.tpe1TestSignalActiveWarning)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= WARN_NSD_Test_Signal_Active;
	}

	if (g_nTPE > 1) // TPE2 also configured
	{
		// record TPE2 loop test events
		//
		// record LT sent event
		if (nsdEvents.TPE2_LT_Sent && !bLastState_TPE2LooptestSent)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE2_LOOPTEST_SENT,0,FALSE);    
		}

		bLastState_TPE2LooptestSent = nsdEvents.TPE2_LT_Sent;
		//
		// record LT replied event
		if (nsdEvents.TPE2_LT_Reflected && !bLastState_TPE2LooptestReplied)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE2_LOOPTEST_REPLIED,0,FALSE);    
		}

		bLastState_TPE2LooptestReplied = nsdEvents.TPE2_LT_Reflected;
		//
		// record LT failed event
		if (nsdEvents.TPE2_LT_Failed && !bLastState_TPE2LooptestFailed)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE2_LOOPTEST_FAILED,0,FALSE);    
		}

		bLastState_TPE2LooptestFailed = nsdEvents.TPE2_LT_Failed;
		//
		// record Manual LT sent event
		if (nsdEvents.TPE2_Manual_LT_Sent && !bLastState_TPE2ManualLooptestSent)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE2_MANUAL_LOOPTEST_SENT,0,FALSE);    
		}

		bLastState_TPE2ManualLooptestSent = nsdEvents.TPE2_Manual_LT_Sent;
		//
		// record Manual LT failed event
		if (nsdEvents.TPE2_Manual_LT_Failed && !bLastState_TPE2ManualLooptestFailed)
		{ // transition from 0-->1
			eventRec_recordEvent(ALARM_EVENT_TPE2_MANUAL_LOOPTEST_FAILED,0,FALSE);    
		}

		bLastState_TPE2ManualLooptestFailed = nsdEvents.TPE2_Manual_LT_Failed;
		//

		g_AlarmsAndWarnings.tpe2AlarmsAndWarnings &= ALM_NSD_Life_Sign;

		// HW alarms
		if(nsdAlarms.progMemCRCErr)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Programm_Memory_CRC;

		// TPE2 alarms and warnings
		if(nsdAlarms.tpe2RxLowLevelAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Rx_Low_Level;

		if(nsdAlarms.tpe2RxHighLevelAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Rx_High_Level;

		if(nsdAlarms.tpe2SNRAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Rx_SNR;

		if(nsdAlarms.tpe2NoTripAndNoGuardAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_No_Trip_and_No_Guard;

		if(nsdAlarms.tpe2TripAndGuardAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Trip_and_Guard;

		if(nsdAlarms.tpe2LoopTestAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Loop_Test;

		if(nsdAlarms.tpe2OutputSetToPredefinedStateAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_Output_Set_To_Predefined_State;

		if(nsdAlarms.tpe2RxContinuousAlarm)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_G4AI_Rx_Continuous;

		if(nsdAlarms.tpe2TestCfgActiveWarning)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= WARN_NSD_Test_Config_Active;

		if(nsdAlarms.tpe2LocalLoopActiveWarning)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= WARN_NSD_Local_Loop_Active;

		if(nsdAlarms.tpe2LocalLoopTestWarning)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= WARN_NSD_Loop_Test;

		if(nsdAlarms.tpe2TestSignalActiveWarning)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= WARN_NSD_Test_Signal_Active;
	}
}

/* !!!Important!!!:
   This function must be called after all other cyclically
   activated monitor functions and before monitor_evaluateTopLevelAlarms.
   Reason: if an alarmDelay with "bSkipInitialPickupTime = TRUE" is created,
   the respective alarm function (e.g. monitor_evaluatePlcLinkAlarms) must 
   have been processed at least once. Else, alarmDelay will switch back to state AlarmIdle when
   activated the first time.
*/

void SEGMENT_EXT_CODE monitor_delayLowLevelAlarms(void)
{
	unsigned long tpe1LevelAlarm;
	unsigned long tpe1SNRAlarm;


	// *************************************
	// delay low level alarms
	// *************************************

	/// delay PLC HW alarms 
	g_p_StatusData->system.alarmsAndWarnings.plcHwAlarms = 
		      alarmDelay(&g_dlyState_plcHwAlarms, g_AlarmsAndWarnings.plcHwAlarms);

	// delay PLC link alarms
	g_p_StatusData->system.alarmsAndWarnings.plcLinkAlarms = 
		      alarmDelay(&g_dlyState_plcLinkAlarms, g_AlarmsAndWarnings.plcLinkAlarms);

	// delay PLC interface alarms
	g_p_StatusData->system.alarmsAndWarnings.plcIfAlarmsAndWarnings = 
		      alarmDelay(&g_dlyState_plcIfAlarms, 
			             g_AlarmsAndWarnings.plcIfAlarmsAndWarnings & PLC_IF_ALM_MASK)
              |
	// delay PLC warnings
		      alarmDelay(&g_dlyState_plcWarnings, 
			             g_AlarmsAndWarnings.plcIfAlarmsAndWarnings & PLC_WARN_MASK);

	// delay DATA alarms and warnings
	g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings = 
			  alarmDelay(&g_dlyState_dataHwAlarms, 
						 g_AlarmsAndWarnings.dataAlarmsAndWarnings & DATA_HW_ALM_MASK)
			  |
			  alarmDelay(&g_dlyState_dataLinkAlarms, 
						 g_AlarmsAndWarnings.dataAlarmsAndWarnings & DATA_LINK_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_dataIfAlarms, 
						 g_AlarmsAndWarnings.dataAlarmsAndWarnings & DATA_IF_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_dataWarnings, 
						 g_AlarmsAndWarnings.dataAlarmsAndWarnings & DATA_WARN_MASK);

	// delay TPE1 alarms and warnings
	tpe1LevelAlarm = alarmDelay(&g_dlyState_tpe1LevelAlarm, 
						 g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_LEVEL_ALM_MASK);

	tpe1SNRAlarm = alarmDelay(&g_dlyState_tpe1SNRAlarm, 
						 g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & ALM_NSD_Rx_SNR);

	g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings = 
			  alarmDelay(&g_dlyState_tpe1HwAlarms, 
						 g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_HW_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_tpe1LinkAlarms, 
						 g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & (NSD_LINK_ALM_MASK & ~AES_MANUAL_RESET_ALM_MASK))

              | tpe1LevelAlarm | tpe1SNRAlarm |

			  alarmDelay(&g_dlyState_tpe1IfAlarms, 
						 g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_IF_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_tpe1Warnings, 
						 g_AlarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_WARN_MASK);

	// delay TPE2 alarms and warnings
	g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings = 
			  alarmDelay(&g_dlyState_tpe2HwAlarms, 
						 g_AlarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_HW_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_tpe2LinkAlarms, 
						 g_AlarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_LINK_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_tpe2IfAlarms, 
						 g_AlarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_IF_ALM_MASK)
              |
			  alarmDelay(&g_dlyState_tpe2Warnings, 
						 g_AlarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_WARN_MASK);
}

void SEGMENT_EXT_CODE monitor_evaluateTopLevelAlarms (void)
{
	unsigned long delayedHighLevelAlarms;

	// *************************************
	// generate delayed high level alarms
	// *************************************

	delayedHighLevelAlarms = highLevelAlarmsOf(&g_p_StatusData->system.alarmsAndWarnings);
  
	// clear all alarm LED's outputs (negative logic)
	IO_BUFFER_WRITE |= (WARNING_LED | P4LT_AL_LED | HW_AL_LED_DSP | LINK_AL_LED | SYS_AL_LED);
	// clear all alarm relays internal and R1BC 
	IO_BUFFER_WRITE &= ~(HW_ALARM_RELAY | LINK_ALARM_RELAY | CABINET_ALARM_RELAY |
						 R1BC_CAB_AL	| R1BC_RES_1	   | R1BC_RES_2);


	// *************************************
	// generate top level alarms
	// *************************************


	// set HW alarm outputs
	if(delayedHighLevelAlarms & TOP_LEVEL_ALM_HW_MASK)
	{
		IO_BUFFER_WRITE &= ~HW_AL_LED_DSP;
		IO_BUFFER_WRITE |= HW_ALARM_RELAY;
	}

	// set LINK alarm outputs
	if(delayedHighLevelAlarms & TOP_LEVEL_ALM_LINK_MASK)
	{
		IO_BUFFER_WRITE &= ~LINK_AL_LED;
		IO_BUFFER_WRITE |= LINK_ALARM_RELAY;
	}

	// P4LT HW alarm LED is set on high level P4LT hardware or P4LT interface alarm.
	// That is to say on 
	// - high level hardware alarms excluding the PLC HW alarms given in the
	//   mask LOCAL_P4LT_PLC_HW_ALM_MASK and
	// - high level interface alarms excluding ALM_Lifesign_G4AI/ALM_NSD_G4AI_board

	// All HW alarms but the given below should signal a P4LT HW alarm
	#define LOCAL_P4LT_PLC_HW_ALM_MASK (ALM_Plut_out_3_RF_Hybrid | \
									    ALM_Plut_out_4_Rx_Filter | \
									    ALM_HW_O4LE_board        | \
									    ALM_Lifesign_O4LE_board  | \
									    ALM_Tx_RF				 | \
										ALM_HW_O4CV_board		 | \
										ALM_Lifesign_O4CV_board)

	if ((g_p_StatusData->system.alarmsAndWarnings.plcHwAlarms            & ~LOCAL_P4LT_PLC_HW_ALM_MASK) ||
		(g_p_StatusData->system.alarmsAndWarnings.plcIfAlarmsAndWarnings & PLC_IF_ALM_MASK)             ||
		(g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings & 
		        (DATA_HW_ALM_MASK | DATA_IF_ALM_MASK))                                                  ||
		(g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & 
				((NSD_HW_ALM_MASK & ~(ALM_Lifesign_G4AI | ALM_NSD_G4AI_board)) | NSD_IF_ALM_MASK))      ||
		(g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & 
				((NSD_HW_ALM_MASK & ~(ALM_Lifesign_G4AI | ALM_NSD_G4AI_board)) | NSD_IF_ALM_MASK))        )
	{
		IO_BUFFER_WRITE &= ~P4LT_AL_LED;
	}

	// drive warning LED
	if (g_blinkWarningLED) // LED is supposed to blink
	{
		static int  blinkCounter; 
		static BOOL LEDisOn;

		if (LEDisOn) IO_BUFFER_WRITE &= ~WARNING_LED; // turn on warning LED

		// blink warning LED
    	if(--blinkCounter <= 0)
    	{
			LEDisOn ^= TRUE; // toggle LEDisOn

    		blinkCounter = WARNING_LED_BLINKING_RATE;
    	}

	}
	else // normal warning LED operation: display warnings
	{
		if(delayedHighLevelAlarms & TOP_LEVEL_WARN_MASK)	    IO_BUFFER_WRITE &= ~WARNING_LED;
	}

	// set SYSTEM alarm outputs
	if(delayedHighLevelAlarms & (TOP_LEVEL_ALM_HW_MASK | TOP_LEVEL_ALM_LINK_MASK | TOP_LEVEL_ALM_IF_MASK))
	{
		static int blinkCounter; 

		IO_BUFFER_WRITE &= ~SYS_AL_LED;
		IO_BUFFER_WRITE |= CABINET_ALARM_RELAY;

		// blink ready LED on system alarm
    	if(--blinkCounter <= 0)
    	{
    		IO_BUFFER_WRITE ^= READY_LED;  	/// toggle RDY_LED every 2 sec
    		blinkCounter = RDY_LED_BLINKING_RATE;
    	}
	}
	else
	{   // no system alarm -> turn on ready LED
		IO_BUFFER_WRITE &= ~READY_LED;
	}

	

	//*******************************************************
	//  drive R1BC relay board
	//*******************************************************

	g_UndelayedHighLevelAlarms = highLevelAlarmsOf(&g_AlarmsAndWarnings); 

	{
		// add remote alarms to upper 16 bits of local highLevelAlarms
		unsigned long localAndRemoteUndelayedHighLevelAlarms = 
			                           g_UndelayedHighLevelAlarms |                             // undelayed local alarms
									   g_pRemoteFastStatusData[APLC_CH1].highLevelAlarms << 16; // undelayed remote alarms

		g_p_StatusData->system.R1BC_State = 0;

		/// delay and hold R1BC user alarm 1 (former cabinet alarm or system alarm)
   		if (alarmDelay(&g_dlyState_R1BCAlarmBus1, localAndRemoteUndelayedHighLevelAlarms & g_p_Cfg->system.R1BC_Config[0].enabledAlarmsAndWarnings)) 	
		{
			IO_BUFFER_WRITE |= R1BC_CAB_AL;
			g_p_StatusData->system.R1BC_State |= R1BC_STATE_ALARM_BUS1;
		}

		/// delay and hold R1BC user alarm 2 (former alarm 1)
   		if (alarmDelay(&g_dlyState_R1BCAlarmBus2, localAndRemoteUndelayedHighLevelAlarms & g_p_Cfg->system.R1BC_Config[1].enabledAlarmsAndWarnings)) 	
		{
			IO_BUFFER_WRITE |= R1BC_RES_1;
			g_p_StatusData->system.R1BC_State |= R1BC_STATE_ALARM_BUS2;
		}

		/// delay and hold R1BC user alarm 3 (former alarm 2)
   		if (alarmDelay(&g_dlyState_R1BCAlarmBus3, localAndRemoteUndelayedHighLevelAlarms & g_p_Cfg->system.R1BC_Config[2].enabledAlarmsAndWarnings)) 	
		{
			IO_BUFFER_WRITE |= R1BC_RES_2;
			g_p_StatusData->system.R1BC_State |= R1BC_STATE_ALARM_BUS3;
		}
	}
}

void SEGMENT_EXT_CODE monitor_generateAlarmEvents(void)
{

	static unsigned long lastPlcHwAlm;
	static unsigned long lastPlcLinkAlm;
	static unsigned long lastPlcIfAlm;
	static unsigned long lastPlcWarning;

	static unsigned long lastNsd1HwAlm;
	static unsigned long lastNsd1LinkAlm;
	static unsigned long lastNsd1IfAlm;
	static unsigned long lastNsd1Warning;

	static unsigned long lastNsd2HwAlm;
	static unsigned long lastNsd2LinkAlm;
	static unsigned long lastNsd2IfAlm;
	static unsigned long lastNsd2Warning;

	static unsigned long lastDataHwAlm;
	static unsigned long lastDataLinkAlm;
	static unsigned long lastDataIfAlm;
	static unsigned long lastDataWarning;

	unsigned long tmp;

	// PLC alarm events
	tmp = g_p_StatusData->system.alarmsAndWarnings.plcHwAlarms;
	if(tmp != lastPlcHwAlm)	    eventRec_recordEvent(ALARM_EVENT_PLC_HW_ALM,   lastPlcHwAlm = tmp,     FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.plcLinkAlarms;
	if(tmp != lastPlcLinkAlm)	eventRec_recordEvent(ALARM_EVENT_PLC_LINK_ALM, lastPlcLinkAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.plcIfAlarmsAndWarnings & PLC_IF_ALM_MASK;
	if(tmp != lastPlcIfAlm)	        eventRec_recordEvent(ALARM_EVENT_PLC_IF_ALM, lastPlcIfAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.plcIfAlarmsAndWarnings & PLC_WARN_MASK;
	if(tmp != lastPlcWarning)	    eventRec_recordEvent(ALARM_EVENT_PLC_WARN,   lastPlcWarning = tmp, FALSE);

	// TPE1 alarm events
	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_HW_ALM_MASK;
	if(tmp != lastNsd1HwAlm)	eventRec_recordEvent(ALARM_EVENT_NSD1_HW_ALM,   lastNsd1HwAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_LINK_ALM_MASK;
	if(tmp != lastNsd1LinkAlm)	eventRec_recordEvent(ALARM_EVENT_NSD1_LINK_ALM, lastNsd1LinkAlm = tmp, FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_IF_ALM_MASK;
	if(tmp != lastNsd1IfAlm)	eventRec_recordEvent(ALARM_EVENT_NSD1_IF_ALM,   lastNsd1IfAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_WARN_MASK;
	if(tmp != lastNsd1Warning)	eventRec_recordEvent(ALARM_EVENT_NSD1_WARN,   lastNsd1Warning = tmp,   FALSE);

	// TPE2 alarm events
	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_HW_ALM_MASK;
	if(tmp != lastNsd2HwAlm)	eventRec_recordEvent(ALARM_EVENT_NSD2_HW_ALM,   lastNsd2HwAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_LINK_ALM_MASK;
	if(tmp != lastNsd2LinkAlm)	eventRec_recordEvent(ALARM_EVENT_NSD2_LINK_ALM, lastNsd2LinkAlm = tmp, FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_IF_ALM_MASK;
	if(tmp != lastNsd2IfAlm)	eventRec_recordEvent(ALARM_EVENT_NSD2_IF_ALM,   lastNsd2IfAlm = tmp,   FALSE);
	
	tmp = g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_WARN_MASK;
	if(tmp != lastNsd2Warning)	eventRec_recordEvent(ALARM_EVENT_NSD2_WARN,   lastNsd2Warning = tmp,   FALSE);

	// DATA alarm events
	tmp = g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings & DATA_HW_ALM_MASK;
	if(tmp != lastDataHwAlm)	eventRec_recordEvent(ALARM_EVENT_DATA_HW_ALM,   lastDataHwAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings & DATA_LINK_ALM_MASK;
	if(tmp != lastDataLinkAlm)	eventRec_recordEvent(ALARM_EVENT_DATA_LINK_ALM, lastDataLinkAlm = tmp, FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings & DATA_IF_ALM_MASK;
	if(tmp != lastDataIfAlm)	eventRec_recordEvent(ALARM_EVENT_DATA_IF_ALM,   lastDataIfAlm = tmp,   FALSE);

	tmp = g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings & DATA_WARN_MASK;
	if(tmp != lastDataWarning)	eventRec_recordEvent(ALARM_EVENT_DATA_WARN,   lastDataWarning = tmp,   FALSE);
}


void SEGMENT_EXT_CODE monitor_checkTDMBus(void) // runs with 100Hz
{
	static   unsigned int startupCounter;

	unsigned long undelayedTdmBusAlarmWord = 0;
	unsigned long alarmsToG4AI = 0;

	unsigned long alarmsToG4AI1, alarmsToG4AI2;
	int           iBasicTimeSlot;
	int           index;

	// Suppress TDM bus traffic/alarms at startup for 5s because the TDM bus slave cards are reset again
	// by the P4LW (by suppressing the P4LW lifesign). See also module "ExternalTDM_BusBridge.c"
	if (startupCounter < 500) 
	{
		startupCounter++;
		return;
	}	
	
	// ***************************************
	// build TDM bus alarm word:  for every slot we have the following settings: 
	//	                          3*iBasicTimeSlot + 0 = Communication error
	//                            3*iBasicTimeSlot + 1 = Slave Board Warning
	//                            3*iBasicTimeSlot + 2 = Slave Board Hardware Error
	// ***************************************

	// for all slots do
	for(iBasicTimeSlot = 0; iBasicTimeSlot < NO_TDM_BASIC_TIMESLOTS; iBasicTimeSlot++)
	{	
	 	if (g_ConfiguredBasicTimeslotArr[iBasicTimeSlot]) // if slot if occupied
	 	{
			switch(g_p_Cfg->system.TDM_Config[iBasicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK)
			{
				case O4LE_BOARD:

					// Set HW alarm and warning bit
					undelayedTdmBusAlarmWord |= ((g_pHardTimingInformationFromTDM[iBasicTimeSlot].Additional_7Bit &  
							 (BASIC_O4LE_WARNING_BIT | BASIC_O4LE_HW_ALARM_BIT)) << (3 * iBasicTimeSlot));

				break;

				case O4CV_BOARD:

					// Set HW alarm and warning bit
					undelayedTdmBusAlarmWord |= ((g_pHardTimingInformationFromTDM[iBasicTimeSlot].Additional_7Bit &  
							 (BASIC_O4CV_WARNING_BIT | BASIC_O4CV_HW_ALARM_BIT)) << (3 * iBasicTimeSlot));

				break;


				case G4AI_BOARD:

					// Set warning bit
					undelayedTdmBusAlarmWord |= ((g_pHardTimingInformationFromTDM[iBasicTimeSlot].Additional_7Bit &  
							 (BASIC_G4AI_WARNING_BIT)) << (3 * iBasicTimeSlot));
					// note: BASIC_G4AI_WARNING_BIT is BIT1 which is then shifted by (3 * iBasicTimeSlot)


					// Set HW alarm
					if (g_pHardTimingInformationFromTDM[iBasicTimeSlot].Additional_7Bit &  
							 (BASIC_G4AI_HW_ALARM_BIT                |
							  BASIC_G4AI_TX_SINGLE_COMP_ALARM_BIT    |
							  BASIC_G4AI_TX_CONT_CMD_ALARM_BIT       |
							  BASIC_G4AI_RX_OVER_CURRENT_ALARM_BIT     ))
					{
						undelayedTdmBusAlarmWord |= 1 << (2 + 3 * iBasicTimeSlot); // the "2" is for the HW alarm bit
					}

				break;
			}

			// Set communication error bit
			if(g_pHardTimingInformationFromTDM[iBasicTimeSlot].chksumErrorCnt) 
			{
				undelayedTdmBusAlarmWord |= (1 << (3 * iBasicTimeSlot));
			}
		}
	}

	// delay TDM bus alarms
	g_p_StatusData->system.alarmsFromTDMBus = alarmDelay(&g_dlyState_TDMBusAlarm, undelayedTdmBusAlarmWord);

	{   // Record changes of TDM bus alarms
		static unsigned long lastTdmBusAlarmword;

		if(g_p_StatusData->system.alarmsFromTDMBus != lastTdmBusAlarmword)	
		{
			eventRec_recordEvent(ALARM_EVENT_TDM_BUS_ALARM, 
								 lastTdmBusAlarmword = g_p_StatusData->system.alarmsFromTDMBus, 
								 FALSE);
		}
	}

	// ***************************************
	// Write/read the alarm bits of slave boards
	// ***************************************

	// reset local alarms bits in "g_AlarmsAndWarnings"
	g_AlarmsAndWarnings.tpe1AlarmsAndWarnings &= ~(ALM_Lifesign_G4AI   |
			                                       ALM_NSD_G4AI_board  |
												   WARN_NSD_G4AI_board   );

	g_AlarmsAndWarnings.tpe2AlarmsAndWarnings &= ~(ALM_Lifesign_G4AI   |
			                                       ALM_NSD_G4AI_board  |
												   WARN_NSD_G4AI_board   );

	g_AlarmsAndWarnings.plcHwAlarms &= ~(ALM_HW_O4LE_board       |
			                             ALM_Lifesign_O4LE_board |
										 ALM_HW_O4CV_board		 |
										 ALM_Lifesign_O4CV_board   );

	g_AlarmsAndWarnings.plcIfAlarmsAndWarnings &= ~(WARN_O4LE_board |
													WARN_O4CV_board);

	// set the common TPE1 & TPE2 alarms in "alarmsToG4AI"
	if(g_p_StatusData->system.R1BC_State & R1BC_STATE_ALARM_BUS1)  
		alarmsToG4AI |= BASIC_G4AI_USER_ALARM_1;

	if(g_p_StatusData->system.R1BC_State & R1BC_STATE_ALARM_BUS2)  
		alarmsToG4AI |= BASIC_G4AI_USER_ALARM_2;

	// Set the alarm bits in additional 7 bits for all G4AIs connected to TPE1
	alarmsToG4AI1 = alarmsToG4AI;

	if (g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_LINK_ALM_MASK)
		alarmsToG4AI1 |= BASIC_G4AI_TPE_LINK_ALARM;

	if (g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & NSD_HW_ALM_MASK)
		alarmsToG4AI1 |= BASIC_G4AI_TPE_HW_ALARM;

	if (g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings & (NSD_HW_ALM_MASK   |
		                                                                  NSD_LINK_ALM_MASK |
																		  NSD_IF_ALM_MASK     ))
		alarmsToG4AI1 |= BASIC_G4AI_TPE_SUMMARY_ALARM;

	// read/write alarms from G4AIs connected to TPE1
	for(index = 0; index < g_G4AI_BasicTimeslotListForTPE1.nEntries; index++)
	{	
		unsigned long mask = 1 << (3 * g_G4AI_BasicTimeslotListForTPE1.TimeslotNumber[index]);

		g_pHardTimingInformationToTDM[g_G4AI_BasicTimeslotListForTPE1.TimeslotNumber[index]].Additional_7Bit &= CLEAR_BASIC_G4AI_ALARMS;
		g_pHardTimingInformationToTDM[g_G4AI_BasicTimeslotListForTPE1.TimeslotNumber[index]].Additional_7Bit |= alarmsToG4AI1;

		if (undelayedTdmBusAlarmWord & mask)
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_Lifesign_G4AI;
	
		if (undelayedTdmBusAlarmWord & (mask << 1))
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= WARN_NSD_G4AI_board;

		if (undelayedTdmBusAlarmWord & (mask << 2))
			g_AlarmsAndWarnings.tpe1AlarmsAndWarnings |= ALM_NSD_G4AI_board;
	}

	// Set the alarm bits in additional 7 bits for all G4AIs connected to TPE2
	alarmsToG4AI2 = alarmsToG4AI;

	if (g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_LINK_ALM_MASK)
		alarmsToG4AI2 |= BASIC_G4AI_TPE_LINK_ALARM;

	if (g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & NSD_HW_ALM_MASK)
		alarmsToG4AI2 |= BASIC_G4AI_TPE_HW_ALARM;

	if (g_p_StatusData->system.alarmsAndWarnings.tpe2AlarmsAndWarnings & (NSD_HW_ALM_MASK|NSD_LINK_ALM_MASK|NSD_IF_ALM_MASK))
		alarmsToG4AI2 |= BASIC_G4AI_TPE_SUMMARY_ALARM;

	// read/write alarms from G4AIs connected to TPE2
	for(index = 0; index < g_G4AI_BasicTimeslotListForTPE2.nEntries; index++)
	{	
		unsigned long mask = 1 << (3 * g_G4AI_BasicTimeslotListForTPE2.TimeslotNumber[index]);

		g_pHardTimingInformationToTDM[g_G4AI_BasicTimeslotListForTPE2.TimeslotNumber[index]].Additional_7Bit &= CLEAR_BASIC_G4AI_ALARMS;
		g_pHardTimingInformationToTDM[g_G4AI_BasicTimeslotListForTPE2.TimeslotNumber[index]].Additional_7Bit |= alarmsToG4AI2;

		if (undelayedTdmBusAlarmWord & mask)
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_Lifesign_G4AI;

		if (undelayedTdmBusAlarmWord & (mask << 1))
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= WARN_NSD_G4AI_board;

		if (undelayedTdmBusAlarmWord & (mask << 2))
			g_AlarmsAndWarnings.tpe2AlarmsAndWarnings |= ALM_NSD_G4AI_board;
	}

	// read the O4LE alarm and warning bits
	for(index = 0; index < g_O4LE_BasicTimeslotList.nEntries; index++)
	{	
		unsigned long mask = 1 << (3 * g_O4LE_BasicTimeslotList.TimeslotNumber[index]);

		if (undelayedTdmBusAlarmWord & mask)
			g_AlarmsAndWarnings.plcHwAlarms            |= ALM_Lifesign_O4LE_board;

		if (undelayedTdmBusAlarmWord & (mask << 1))
			g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_O4LE_board;
	
		if (undelayedTdmBusAlarmWord & (mask << 2))
			g_AlarmsAndWarnings.plcHwAlarms            |= ALM_HW_O4LE_board;
	}

	// read the O4CV alarm and warning bits
	for(index = 0; index < g_O4CV_BasicTimeslotList.nEntries; index++)
	{	
		unsigned long mask = 1 << (3 * g_O4CV_BasicTimeslotList.TimeslotNumber[index]);

		if (undelayedTdmBusAlarmWord & mask)
			g_AlarmsAndWarnings.plcHwAlarms            |= ALM_Lifesign_O4CV_board;

		if (undelayedTdmBusAlarmWord & (mask << 1))
			g_AlarmsAndWarnings.plcIfAlarmsAndWarnings |= WARN_O4CV_board;
	
		if (undelayedTdmBusAlarmWord & (mask << 2))
			g_AlarmsAndWarnings.plcHwAlarms            |= ALM_HW_O4CV_board;
	}
}


t_AlarmsAndWarnings SEGMENT_EXT_CODE * monitor_get_pAlarmsAndWarnings(void)
{
	return &g_AlarmsAndWarnings;
}


unsigned long SEGMENT_EXT_CODE * monitor_get_pHighLevelAlarms(void)
{
	return &g_UndelayedHighLevelAlarms;
}


unsigned long SEGMENT_EXT_CODE highLevelAlarmsOf (t_AlarmsAndWarnings * pLowLevelAlarms)
{

	unsigned long highLevelAlarms = 0;

	// *************************************
	// generate high level alarms
	// *************************************

	// PLC alarms and warnings
	if (pLowLevelAlarms->plcHwAlarms)
		highLevelAlarms |= R1BCALM_LOCAL_PLC_HW_ALM;

	if (pLowLevelAlarms->plcLinkAlarms)
		highLevelAlarms |= R1BCALM_LOCAL_PLC_LINK_ALM;

	if (pLowLevelAlarms->plcIfAlarmsAndWarnings & PLC_IF_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_PLC_IF_ALM;

	if (pLowLevelAlarms->plcIfAlarmsAndWarnings & PLC_WARN_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_PLC_WARN;
	
	// NSD1 alarms and warnings
	if (pLowLevelAlarms->tpe1AlarmsAndWarnings & NSD_HW_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE1_HW_ALM;

	if (pLowLevelAlarms->tpe1AlarmsAndWarnings & NSD_LINK_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE1_LINK_ALM;

	if (pLowLevelAlarms->tpe1AlarmsAndWarnings & NSD_IF_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE1_IF_ALM;

	if (pLowLevelAlarms->tpe1AlarmsAndWarnings & NSD_WARN_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE1_WARN;


	// NSD2 alarms and warnings
	if (pLowLevelAlarms->tpe2AlarmsAndWarnings & NSD_HW_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE2_HW_ALM;

	if (pLowLevelAlarms->tpe2AlarmsAndWarnings & NSD_LINK_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE2_LINK_ALM;

	if (pLowLevelAlarms->tpe2AlarmsAndWarnings & NSD_IF_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE2_IF_ALM;

	if (pLowLevelAlarms->tpe2AlarmsAndWarnings & NSD_WARN_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_TPE2_WARN;


	// DATA alarms and warnings
	if (pLowLevelAlarms->dataAlarmsAndWarnings & DATA_HW_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_DATA_HW_ALM;

	if (pLowLevelAlarms->dataAlarmsAndWarnings & DATA_LINK_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_DATA_LINK_ALM;

	if (pLowLevelAlarms->dataAlarmsAndWarnings & DATA_IF_ALM_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_DATA_IF_ALM;

	if (pLowLevelAlarms->dataAlarmsAndWarnings & DATA_WARN_MASK)
		highLevelAlarms |= R1BCALM_LOCAL_DATA_WARN;

	return highLevelAlarms;
}


void SEGMENT_EXT_CODE monitor_blinkWarningLED(BOOL enable)
{
	g_blinkWarningLED = enable;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//// if a delayed CRC error of one of the DSPs is detected, than perform a system reset after an additional delay
//// to get an event recorder entry
void SEGMENT_EXT_CODE monitor_ifCRCerror_Reset(void)
{
	static unsigned long additional_delay = ADDITIONAL_RESET_DELAY_COUNT;	

	if(((g_p_StatusData->system.alarmsAndWarnings.plcHwAlarms) & CRC_RESET_PLC_HW_ALM_MASK) || 
		((g_p_StatusData->system.alarmsAndWarnings.dataAlarmsAndWarnings) & CRC_RESET_DATA_HW_ALM_MASK) ||
		((g_p_StatusData->system.alarmsAndWarnings.tpe1AlarmsAndWarnings) & CRC_RESET_NSD_HW_ALM_MASK))
	{	//Any CRC error detected - now delay the reset to get an event recorder entry
		
		additional_delay--;
	}

	if(!additional_delay)
	{ 
		if (!isInDebugMode() && g_isResetEnabled)
		{
			resetP4LT();  
		}
		additional_delay = ADDITIONAL_RESET_DELAY_COUNT;	// set again if reset was disabled!
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//// Disable Reset in case of FW Update -> see msgHandler

void SEGMENT_EXT_CODE monitor_enableReset(BOOL isEnabled)
{
	g_isResetEnabled = isEnabled;
}


/*****************************************************************************
  Function:		BOOL monitor_detectResetG4AI()	
  Description:	Detection of G4AI reset. Used to detect reset of G4AI at 
				rightmost position in case of manual link alarms acknowledge.
  Inputs:		received value of basic time slot 6			
  Outputs:		-	
  Return:		reset detected: TRUE/FALSE		
  Globals:		-	
  Important:	Call this function in a 106.67 Hz subcase.

******************************************************************************/

void SEGMENT_EXT_CODE monitor_detectResetG4AI(unsigned long basicTimeSlot)
{
	static unsigned long toggleBitOld;
	static long noTogglingCounter = 0;

	unsigned long toggleBit;
	BOOL resetDetected;


	toggleBit = basicTimeSlot & TOGGLE_BIT_G4AI;

	if (toggleBit == toggleBitOld)	// toggle bit has not changed
	{
		noTogglingCounter++;

		if (noTogglingCounter > NO_TOGGLING_MAX)
		{
			noTogglingCounter = 0;
			resetDetected = TRUE;
		}
		else
		{
			resetDetected = FALSE;
		}
	}
	else	// toggle bit has changed
	{
		noTogglingCounter = 0;
		resetDetected = FALSE;
	}

	toggleBitOld = toggleBit;
	
	if (resetDetected)
	{
		alarmAcknowledge(&g_dlyState_tpe1LevelAlarm);
		alarmAcknowledge(&g_dlyState_tpe1SNRAlarm);
	}

}



