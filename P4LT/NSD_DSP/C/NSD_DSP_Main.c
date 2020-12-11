/*-------------------------------------------------------------------------*
 * $Workfile: NSD_DSP_Main.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : DSP main loop
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSD_DSP/C/NSD_DSP_Main.c $ 
 * 
 * 61    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 58    08-02-25 10:17 Chtognu
 * Merged changes from ad.equ.branch:
 * NSD trip and guard alarm boolian added for RUENE (set flag whene enough
 * power in largest/second detector comparing to wide band power used
 * additionaly for the alarm "trip and guard")
 * 
 * 57    28.08.07 9:09 Chtognu
 * DetectorActive variable for Ch1 and Ch2 added
 * 
 * 55    27.11.06 16:55 Chpeplu
 * Function setAckBits() changed for Cmd E and F
 * 
 * 54    14.11.06 13:24 Chpeplu
 * New Defines for LOOP Test command used RX_COM_T and TX_COM_T,
 * different Functions extended for costum modes, 
 * 
 * 53    8.11.06 14:33 Chtognu
 * Bug 392 fixed, new input value for CompensationOfEvaluationTime() to
 * reset prolongation after the guard is detected. Changes active for NSD
 * TPE (2 and 4KHz).
 * 
 * 52    31.10.06 16:00 Chhamaa
 * AES rx ack is blocked in case of blocked outputs.
 * 
 * 51    5.10.06 11:28 Chhamaa
 * setAckBits() modified: pers. command added
 * 
 * 50    20.09.06 13:13 Chhamaa
 * AESLocalCommandLoop() moved to AES_Transmitter.c
 * 
 * 49    19.09.06 10:46 Chhamaa
 * Command outputs blocked alarm update added.
 * 
 * 48    18.09.06 17:00 Chhamaa
 * 
 * 47    12.09.06 16:52 Chhamaa
 * TANG window functionality and AES status added.
 * 
 * 46    8.09.06 16:05 Chhamaa
 * Rx guard signalling added.
 * 
 * 45    6.09.06 16:11 Chhamaa
 * Function setAckBits() modified.
 * 
 * 44    4.09.06 14:40 Chhamaa
 * Function setAckBits() added.
 * 
 * 43    30.08.06 18:40 Chhamaa
 * 
 * 42    29.08.06 11:35 Chhamaa
 * AES detector added.
 * 
 * 41    21.08.06 18:09 Chhamaa
 * 
 * 40    15.08.06 9:25 Chhamaa
 * 
 * 39    10.08.06 18:08 Chhamaa
 * 
 * 38    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 40    20.07.06 17:31 Chhamaa
 * 
 * 39    18.07.06 18:44 Chhamaa
 * 
 * 38    7.07.06 18:50 Chhamaa
 * 
 * 35    11.04.06 11:18 Chtognu
 * Bug fixed in "set suppress alarms bits"
 * 
 * 34    5.04.06 16:53 Chtognu
 * Logic for filling TPE1_Suppress_Alarms and TPE2_Suppress_Alarms bits
 * added
 * 
 * 33    28.02.06 13:41 Chpeplu
 * function ChangeConfigToHSD() removed -> config settings are done with
 * HMI now!
 * 
 * 32    8.02.06 13:27 Chpeplu
 * HSD Mode 2+1 removed, code for configurable integration time removed
 * 
 * 31    31.01.06 16:25 Chpeplu
 * new mode HSD integrated, config map to HSD on, 
 * 
 * 29    28.10.05 8:11 Chchlee
 * some ASSERTs removed
 * 
 * 28    4.10.05 7:57 Chchlee
 * SEGMENT_EXT_CODE added to getConfiguration
 * 
 * 27    20.09.05 17:22 Chpeplu
 * New function Freeze_CommandOutput() used and function rx_guard_nsd()
 * with additional parmeter 
 * 
 * 26    30.06.05 8:35 Chpeplu
 * Code walkthrough Pl
 * 
 * 25    22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 24    16.06.05 13:22 Chpeplu
 * Do not record command events in case of nsd link alarm if outputs are
 * set to command state
 * 
 * 23    2.06.05 9:48 Chpeplu
 * In function UpdateNSD_Events new events Manual_LT_Sent and Manual
 * LT_Failed added (for Ch1 and Ch2)
 * 
 * 22    1.06.05 16:39 Chpeplu
 * During local loop on reset received loop test -> Rx_Command_Ch1_Det
 * &=(~COM_T); for Ch1 and Rx_Command_Ch2_Det &=(~COM_T); for Ch2
 * 
 * 21    27.05.05 14:10 Chpeplu
 * Some test paramter removed, sampling buffer removed, comment removed,
 * not used code removed
 * 
 * 20    24.05.05 11:42 Chpeplu
 * function UpdateNSD_Events() moved to extern, Reset LT_Events_Ch1/Ch2
 * corrected
 * 
 * 19    23.05.05 17:01 Chpeplu
 * LT-Events and Start Events added for event recorder
 * 
 * 18    19.05.05 11:17 Chmibuh
 * memoryCheckRespondToMaster();  added
 * dummy memoryCheck removed
 * 
 * 17    12.05.05 18:27 Chmibuh
 * 
 * 16    11.05.05 20:14 Chmibuh
 * CRC Check added
 * 
 * 15    9.05.05 13:19 Chbrbus
 * MemoryCheck and CRC Patcher initial version added
 * 
 * 14    12.04.05 14:54 Chpeplu
 * Some comment lines added for process time measurements
 * 
 * 13    4.04.05 9:40 Chpeplu
 * Some comment added for tests signal NSD600 
 * 
 * 12    15.03.05 17:46 Chpeplu
 * For tests HMI600 -> packedTableCh2 added 
 * 
 * 11    14.03.05 13:30 Chpeplu
 * Some changes for up to six commands made
 * 
 * 10    11.03.05 16:26 Chpeplu
 * Testfunction for CustomTable added
 * 
 * 9     10.03.05 9:38 Chpeplu
 * Receive buffer g_niRxData[500] increased and moved to extern
 * 
 * 8     9.03.05 10:27 Chpeplu
 * New config used (UsedExtTimeSlots, UsedCommands)
 * 
 * 7     3.03.05 14:00 Chpeplu
 * prepared for NewConfigNSD without custom table
 * 
 * 6     1.03.05 12:05 Chpeplu
 * Prototyp 1 changes form version 95 take over
 * 
 * 5     23.02.05 16:27 Chmibuh
 * test_iButton removed
 * 
 * 4     14.02.05 18:12 Chbrbus
 * Bugfix: DMA Buffer Pointers for TDM busses are always in the valid DMA
 * buffer range. No read/write in invalid memory areas possible
 * 
 * 3     25.01.05 13:56 Chpeplu
 * New function UpdateBoostManager() added and Local Loop Mode for TPE1/2
 * added
 * 
 * 2     21.01.05 7:47 Chpeplu
 * AGC-Blocking_Ch1...Ch3 and Pilot_enable_Ch1...Ch3 update in developer
 * status added
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 87    19.01.05 13:01 Chpeplu
 * Pilot enable signal removed now on master dsp
 * 
 * 86    18.01.05 17:18 Chpeplu
 * Rx_Command_Ch1/2_Det and Rx_Command_Ch1/2_ToG4AI added
 * 
 * 85    17.01.05 15:54 Chpeplu
 * function SNR_Monitoring_Guard() additional parameter added for snr from
 * FE, if NSD uses etl pilot as guard
 * 
 * 84    14.01.05 17:28 Chpeplu
 * SNR from FE via internal TDM-Bus added
 * 
 * 83    13.01.05 16:00 Chharyt
 * Garbage removed and IOFLAG's aktive
 * 
 * 82    13.01.05 15:23 Chpeplu
 * function ArrayFloatCheck() for Rx-AF_Ch1[] and Rx_Af_Ch2[] added
 * 
 * 81    12.01.05 17:54 Chpeplu
 * Functions for CompemsationOfEvalauatinTime() and
 * RxCommandDurationMonit() and Update_DevStatus_LT() added
 * 
 * 80    11.01.05 16:34 Chharyt
 * SendDataToG4AI(&DataToSendToG4AI[0]); changed
 * 
 * 79    11.01.05 13:57 Chharyt
 * Saved_externalTDM_Tx_Ptr[0] = externalTDM_Tx_Ptr[0]; inserted
 * IOFLAG's for measuring disabled
 * 
 * 78    7.01.05 16:29 Chharyt
 * synchronisation NSD to Master improved. Timer adjusted. cast internal
 * TDM bus rx and tx pointers moved. 
 * 
 * 77    4.01.05 17:27 Chpeplu
 * Init NSD Alarms, update Developer status changed
 * 
 * 76    3.01.05 17:00 Chpeplu
 * function updateAlarms() added for internal tdm-bus communication to
 * master dsp
 * 
 * 75    23.12.04 15:21 Chpeplu
 * SNR monitoring added
 * 
 * 74    21.12.04 15:52 Chpeplu
 * struct t_NSD_DSP_Status changed to t_IntTeleprotStatus
 * 
 * 73    20.12.04 15:44 Chpeplu
 * tdm-Service e_TDMServiceMsgType_NSD_Read_Developer_Status_Req added
 * 
 * 72    20.12.04 12:48 Chchlee
 * break added in Status Req
 * 
 * 71    17.12.04 10:07 Chpeplu
 * TDM-Service Request e_TDMServiceMsgType_StatusData_Req added
 * 
 * 70    16.12.04 10:35 Chpeplu
 * new function Save_StateWithoutRxAlarm() added
 * 
 * 69    15.12.04 17:32 Chpeplu
 * New functions for NSD Output at Rx Alarms added
 * 
 * 68    14.12.04 13:05 Chpeplu
 * function StartSetMax1Sec() added
 * 
 * 67    7.12.04 11:56 Chpeplu
 * Update Status Data and Developer Status data added
 * 
 * 66    6.12.04 15:42 Chpeplu
 * Update Status and Developer-Status added
 * 
 * 65    3.12.04 13:25 Chpeplu
 * NSD_Status and NSD_Devewlopper_Status added 
 * 
 * 64    2.12.04 13:35 Chpeplu
 * New function RxGuardLevelMonitoring() added and init function for
 * RxGuard changed
 * 
 * 63    1.12.04 17:24 Chpeplu
 * Monitoring functions Unblocking and MonitoringTripGuard added
 * 
 * 62    30.11.04 19:30 Buhl01
 * test_iButton(); added
 * 
 * 61    19.11.04 11:37 Maag01
 * g_niReceiveFlag bug fixed
 * 
 * 60    19.11.04 8:20 Chharyt
 * SendDataToG4AI at the end removed
 * 
 * 59    19.11.04 8:12 Chharyt
 * Sync to Master DSP for external TDM inserted
 * 
 * 58    19.11.04 7:38 Plueer01
 * Add function ServeManualLoopTest_byHMI for manal loop test by HMI,
 * TDM-service for NSD DSP Loop test added
 * 
 * 57    15.11.04 9:27 Leeb02
 * Function call in ASSERT() removed
 * 
 * 56    12.11.04 15:04 Plueer01
 * function getConfiguration() changed -> tdmServiceActivateNonInitiator()
 * is called to send ack
 * 
 * 55    12.11.04 10:17 Plueer01
 * Function InitNSDMode() improved to set up Rx and Tx Command mask and
 * Start Mask, Tx-Acknowl. and Rx Guard signal for G4AI added
 * 
 * 54    2.11.04 15:08 Maag01
 * Case e_TDMServiceMsgType_Cfg_Send added in main loop message handler
 * 
 * 53    2.11.04 14:46 Plueer01
 * LT function for Ch2 added, some test functions for LT added
 * 
 * 52    27.10.04 11:12 Plueer01
 * LT functions for NSD600 Ch1 added
 * 
 * 51    5.10.04 14:26 Rytz01
 * SendDataToG4AI moved to the end of the NSD Main case that we can handle
 * the most actual  samples. Investigation are needed to verify that this
 * routine runs in second half of the computations time otherwise we get
 * the promlems back. Runs together with version 25 of the
 * ExternalTDM_BusG4AI_HL.c
 * 
 * 50    1.10.04 16:10 Plueer01
 * Test variables config_BoostCh1, config_BoostCh2 added to check
 * Configuration from HMI600
 * 
 * 49    30.09.04 12:08 Leeb02
 * t_Boost_Mgr_Input initialized
 * 
 * 48    28.09.04 12:02 Plueer01
 * Pilot_enable for ch1 and ch2 added and send via internal tdm-bus 
 * 
 * 47    28.09.04 9:03 Plueer01
 * RxGuardStateCh2 corrected! 
 * 
 * 46    27.09.04 17:19 Plueer01
 * Copy Config for NSD600 Ch2 to test data for tests only, Ch2 AF samples
 * from FE DSP  
 * 
 * 45    24.09.04 14:22 Leeb02
 * 
 * 44    20.09.04 17:41 Maag01
 * tdmServiceActivateNonInitiator() added in the main loop, read idle
 * counters using HMI enabled,
 * 
 * 43    16.09.04 7:46 Plueer01
 * Do not use default config of NSD600
 * 
 * 42    13.09.04 13:21 Leeb02
 * tdmInit_SetDSPId added
 * 
 * 41    10.09.04 17:14 Plueer01
 * Variabel ReceivedDataFromG4AI[] and DataSendToG4AI[]  size replaced by
 * NO_TDM_BASIC_TIMESLOTS
 * 
 * 40    9.09.04 17:19 Rytz01
 * In function call initialize_DSP parameter added. A second call of it
 * after getting the config to init the SPORT02 correct.
 * 
 * 39    8.09.04 14:26 Plueer01
 * config struct for NSD changed generally to -> t_NSD_DSP_Config
 * 
 * 38    3.09.04 10:37 Leeb02
 * 
 * 37    1.09.04 14:02 Rytz01
 * Unused var removed
 * 
 * 36    30.08.04 10:12 Rytz01
 * TDM Bus off-set problem solved
 * Write Debug Words the internal TDM bus!!!
 * 
 * 35    27.08.04 16:04 Plueer01
 * Function InitNSDMode() added
 * 
 * 34    27.08.04 8:23 Plueer01
 * Serve internal tdm bus with data to master DSP (command status ch1 and
 * ch2 for tx and rx side)
 * 
 * 33    24.08.04 16:33 Maag01
 * 
 * 32    24.08.04 15:40 Plueer01
 * Boost Req and Start signal for Boost manager on Master DSP added
 * 
 * 31    23.08.04 19:00 Maag01
 * 
 * 30    23.08.04 17:05 Plueer01
 * Configuratin extended and use of sample from FE for ch2 tx and rx side
 * 
 * 29    20.08.04 16:44 Plueer01
 * NSD detector for ch2 with AF loop signal added, memory check is now on 
 * 
 * 28    19.08.04 13:44 Leeb02
 * 
 * 27    11.08.04 15:37 Buhl01
 * Memory Check revoved with //
 * 
 * 26    11.08.04 11:36 Buhl01
 * dummy memory check added for boot control
 * 
 * 25    9.08.04 19:14 Buhl01
 * 
 * 24    6.08.04 11:30 Buhl01
 * toggle FLAG1 for boot testing -> REMOVE later
 * 
 * 23    6.08.04 11:26 Leeb02
 * 
 * 22    5.08.04 15:47 Leeb02
 * Lifesign added
 * 
 * 21    26.07.04 16:07 Maag01
 * 
 * 20    22.07.04 14:57 Plueer01
 * Modifications made for dual channel NSD detector
 * 
 * 19    19.07.04 16:13 Plueer01
 * HMI500 changes update -> struct member in nsd-config removed 
 * 
 * 18    19.07.04 11:00 Plueer01
 * Functions for Transmitter NSD and RxGuard NSD now usable for two
 * channels and some order changed (transmitter beforeserve  TDM-intern
 * bus ) 
 * 
 * 17    9.07.04 15:17 Bussli02
 * Communication with 2 G4AI, static words, tested  and working (N22 and
 * N28, AES FW)
 * 
 * 16    9.07.04 9:45 Plueer01
 * New HMI600 defines used from NSD_Types.h
 * 
 * 15    8.07.04 17:10 Plueer01
 * NSD_Transmitter.c functions for two channel usable
 * 
 * 14    8.07.04 16:23 Plueer01
 * Function InitNSDConfig () extended for Ch2
 * 
 * 13    8.07.04 14:12 Plueer01
 * Config struct NSD changed, internal tdm-bus access changed 
 * 
 * 12    28.06.04 9:43 Plueer01
 * Command input and output for NSD via Master DSP added 
 * 
 * 11    23.06.04 16:11 Rytz01
 * 
 * 10    21.06.04 9:51 Plueer01
 * Some functions for RxGuard  detection NSD added
 * 
 * 9     17.06.04 16:06 Rytz01
 * 
 * 8     8.06.04 17:16 Plueer01
 * function InitNSDConfig() changed
 * 
 * 7     4.06.04 16:55 Plueer01
 * init config NSD changed
 * 
 * 6     1.06.04 16:37 Plueer01
 * Amplitude correction for NSD detector tones added
 * 
 * 5     28.05.04 10:44 Plueer01
 * NSD Detector added
 * 
 * 4     27.05.04 9:29 Rytz01
 * 
 * 3     7.05.04 16:30 Rytz01
 * 5.333kHz loop tested
 * 
 * 2     7.05.04 11:20 Rytz01
 * 
 * 1     5.05.04 14:59 Rytz01
 * 
 * 2     7.04.04 11:42 Leeb02
 * 
 * 1     31.03.04 11:30 Maag01
 * Initial Version
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include <signal.h>

#include <def21161.h>
asm("#include <def21161.h>");

#include "basetype.h"
#include "diag.h"
#include "idlecounter.h"

#include "NSD_Types.h"
#include "NSD_DSP_Types.h"
#include "Init_NSD_DSP.h"
#include "NSD_DSP_Main.h"
#include "NSD_Transmitter.h"
#include "NSD_Detector.h"
#include "Rx_Guard_NSD.h"
#include "NSD600_LT_Constants.h"
#include "NSD_Monitor.h"
#include "NSD_TEST.H"
#include "nsdCustomTable.H"

#include "AES600.h"
#include "AES_Transmitter.h"
#include "AES_Detector.h"

#include "TDM_Init.h"
#include "tdm_int_types.h"
#include "TDM_Service.h"
#include "tdm_ext_types.h"
#include "ExternalTDM_BusG4AI_HL.h"

#include "MemoryCheck.h"
#include "Timer.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define		ONE_SECOND_MAINCASE		1067
//
#define		OR_MASKE_OUT_AT_RXALARM	0x00000000
#define		AND_MASK_OUT_AT_RXALARM	0xffffffff

#define		TPE_LOCAL_LOOP_ON		0x00000001
#define		TPE_LOCAL_LOOP_OFF		0x00000000

#define		CH1_LOCAL_LOOP			0x00010000
#define		CH2_LOCAL_LOOP			0x00020000


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTIONS ***********************************************************/
//
static void getConfiguration(t_NSD_DSP_Config *config);
static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize);
static void niConfirm (void);
static void InitNSDMode(float *ptr_AF_Mask);


/* LOCAL VARIABLES ***********************************************************/

float *ptr_RxIF_Sin_Ch1, *ptr_RxIF_Cos_Ch1;
float *ptr_RxIF_Sin_Ch2, *ptr_RxIF_Cos_Ch2;
//
// config NSD
t_NSD_DSP_Config		g_NSD_DSP_Config;
// 
t_IntTeleprotStatus		g_NSD_DSP_Status;
//
t_NSD_DevelopperStatus_Type g_Develop_Status_NSD;
//
// NSD transmitter side
//
float Tx_AF_Ch1_I[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
float Tx_AF_Ch1_Q[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
float Tx_AF_Ch2_I[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
float Tx_AF_Ch2_Q[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
//

BOOL boostRequestCh1 = FALSE, boostRequestCh2 = FALSE;

unsigned long Pilot_enable1, Pilot_enable2, Pilot_enable3;
unsigned long AGC_Blocking1, AGC_Blocking2, AGC_Blocking3;
//
//NSD test signal generator
NSDTestDataConfig *ptr_Konfig_NSD_Testsignal_Gen;
float NSD_Tx_TestSignal_Q[NUMBER_OF_RX_SAMPLE];
float NSD_Tx_TestSignal_I[NUMBER_OF_RX_SAMPLE];
//
long Command_Input_Ch1;
long Command_Input_Ch2;
//
// used rx commands (not switched off)
long RxCommandMaskCh1 = 0;
long RxCommandMaskCh2 = 0;
//// used tx commands 
long TxCommandMaskCh1 = 0;
long TxCommandMaskCh2 = 0;
//
long Start_Ch1, Start_Ch2;				// start signal for boost manager
// 
long StartInput_Ch1, StartInput_Ch2;	// start signal input form G4AI
//
long StartMask_Ch1 = 0;
long StartMask_Ch2 = 0;

BOOL RxGuardMask_Ch1 = FALSE;
BOOL RxGuardMask_Ch2 = FALSE;

// change_gn AES trip and guard alarm
BOOL DetectorActive_Ch1 = FALSE;
BOOL DetectorActive_Ch2 = FALSE;

long UnblockingBitCh1, UnblockingBitCh2;
long GuardLevelAlarmCh1, GuardLevelAlarmCh2; 
//
unsigned long Rx_Alarm_OR_MaskCh1, Rx_Alarm_AND_MaskCh1;
unsigned long Rx_Alarm_OR_MaskCh2, Rx_Alarm_AND_MaskCh2;
//
static unsigned long LT_Event_Ch1, LT_Event_Ch2;
//
// NSD receiver side
//
float *ptr_Det_Sin_Ch1, *ptr_Det_Cos_Ch1;
float *ptr_Det_Sin_Ch2, *ptr_Det_Cos_Ch2;

long Rx_Command_Ch1_Det, Rx_Command_Ch2_Det;
long Rx_Command_Ch1_ToG4AI, Rx_Command_Ch2_ToG4AI; 
BOOL RxGuardStateCh1, RxGuardStateCh2;
BOOL RxGuardStateCh1ToG4AI, RxGuardStateCh2ToG4AI;

float Rx_AF_Ch1[NUMBER_OF_RX_SAMPLE];
float Rx_AF_Ch2[NUMBER_OF_RX_SAMPLE];

float FE_snr_Ch1, FE_snr_Ch2;

extern volatile long *externalTDM_Tx_Ptr[2];
extern volatile long *externalTDM_Rx_Ptr[2];

extern volatile long *internalTDM_Tx_Ptr[2];
extern volatile long *internalTDM_Rx_Ptr[2];

static t_AllDSP_input_tdm_block *g_intTDM_RxStruct0_Ptr;
static t_AllDSP_input_tdm_block *g_intTDM_RxStruct1_Ptr;

static t_NSD_output_tdm_block *g_intNSDTDM_TxStruct0_Ptr;
static t_NSD_output_tdm_block *g_intNSDTDM_TxStruct1_Ptr;

// TDM service variables
static SEGMENT_EXT_DATA unsigned long        g_niRxData[500]; // receive buffer
static BOOL                 g_niReceiveFlag;
static e_TDMServiceMsgType	g_niRxMsgType;
static unsigned long	    g_niRxMsgSize;

// buffer for idle counters in case of TDM service request
static t_IdleCounters       g_idleCountersBuf; 

static unsigned long g_LoopTestMode;
static unsigned long g_NSDLoopTestDelay;

static unsigned long g_NSDLocalLoopModeCh1, g_NSDLocalLoopModeCh2, g_NSDLocalLoopMode;
static unsigned long Looped_Cmd_Ch1, Looped_Cmd_Ch2;
static BOOL NSD_Mode_Changed;

// AES rx/tx command acknowledge hold time
static long aesCmdAckHoldTime;


/* IMPLEMENTATION *************************************************************/

/*****************************************************************************
  Function:		getConfiguration()	
  Description:	Waits for the configuration sent by the master DSP
  Inputs:		config: pointer to configuration data structure 
  Outputs:		Received configuration data
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void SEGMENT_EXT_CODE getConfiguration(t_NSD_DSP_Config *config)
{
	int i;
	e_TDMServiceResult tdmServiceResult;


	tdmServiceNI_Init(e_TDMServiceAddr_NSD,  // initialize non-initiator TDM service
					  g_niRxData,
					  sizeof(g_niRxData) / sizeof(unsigned long),
					  &niReceive,
					  &niConfirm);

	niConfirm(); // enable TDM service receiver

	// wait for the configuration
	while(!g_niReceiveFlag) // the flag is set when the TDM service received a valid configuration
	{
		// repetiton rate = 5.333 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		IR_BlockCounter = 2;
		RestartTimer5k();
		IR_TimerCounter = 1;
	
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[0], (unsigned long *) internalTDM_Tx_Ptr[0]);
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[1], (unsigned long *) internalTDM_Tx_Ptr[1]);
	}

	g_niReceiveFlag = FALSE;

	// configuration received
	ASSERT(e_TDMServiceMsgType_Cfg_Send == g_niRxMsgType);	// check if tx message type is correct
	ASSERT(sizeof(*config) == g_niRxMsgSize);				// check if size is correct

	// send TDM service reply

	tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    						               e_TDMServiceMsgType_Ack); // txMsgType = acknowledge

	ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);
	
	MEMCPY (config, g_niRxData, sizeof(*config)); // copy received message into config
	//
	// tdmServiceActivateNonInitiator() is called to send ack
	for(i = 0; i < 20; i++)
	{
		// repetiton rate = 5.333 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		IR_BlockCounter = 2;
		RestartTimer5k();
		IR_TimerCounter = 1;

		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[0], (unsigned long *) internalTDM_Tx_Ptr[0]);
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[1], (unsigned long *) internalTDM_Tx_Ptr[1]);
	}
}


static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize)
{
	g_niReceiveFlag = TRUE;
	g_niRxMsgType = rxMsgType;
	g_niRxMsgSize = rxMsgSize;
}


static void niConfirm (void)
{
	tdmServiceNI_Receive();
}


/*****************************************************************************
  Function:		void StartSetMax1Sec(void)	
  Description:	The start signal switch off speech (or if no speech disconnectable channels) 
				signals in advanced before a real command will be sent. But 
				the signal should only be activ during max. one second to disconnect
				speech and disconnectable channels. this function limits the switch off 
				condition to one second.
  Inputs:		-
  Outputs:		- 
  Return:		-
  Globals:		-
  Important:	Input signal from G4AI is StartInput_Ch1/2 and output signal 
				to boost mangager is Start_Ch1/2

******************************************************************************/

static void SEGMENT_EXT_CODE StartSetMax1Sec(void)
{
	static unsigned long ActivStartCounterCh1 = 0, ActivStartCounterCh2 = 0;
	// **************************************************************
	// *** Test Start input Ch1   ***********************************
	// **************************************************************
	if(StartInput_Ch1 == TDM_START_BIT_NSD)
	{
		// start input signal active
		if(ActivStartCounterCh1 < ONE_SECOND_MAINCASE)
		{
			// Set start signal for discconect speech active
			Start_Ch1 = TDM_START_BIT_NSD;		// set start signal for boost manager
			ActivStartCounterCh1++;				// increase counter for time monitoring
		}
		else
		{
			// start input signal active but was active longer then one second
			Start_Ch1 = 0;						// reset Start signal for boost manager 	
		}
	}
	else
	{
		// start input signal not active 
		Start_Ch1 = 0;							// reset Start signal 
		ActivStartCounterCh1 = 0;				// reset counter for max activ time 
	}

	// **************************************************************
	// *** One second reset Start input Ch2   ***********************
	// **************************************************************
	if(StartInput_Ch2 == TDM_START_BIT_NSD)
	{
		// start input signal active
		if(ActivStartCounterCh2 < ONE_SECOND_MAINCASE)
		{
			// Set start signal for discconect speech active
			Start_Ch2 = TDM_START_BIT_NSD;		// set start signal for boost manager
			ActivStartCounterCh2++;				// increase counter for time monitoring
		}
		else
		{
			// start input signal active but was activ longer then one second
			Start_Ch2 = 0;						// reset Start signal for boost manager 	
		}
	}
	else
	{
		// start input signal not active 
		Start_Ch2 = 0;							// reset Start signal 
		ActivStartCounterCh2 = 0;				// reset counter for max active time 
	}
}


/*****************************************************************************
  Function:		InitNSDMode(float *ptr_AF_Mask)	
  Description:	Set the command mask for Rx and tx side and the AF Mask to reset sample to and from FE DSP to 0.0
  Inputs:		Start pointer of Array for AF-Mask value 1.0 or 0.0
  Outputs:		- (Overwrite Array AF mask of size N_APLC_CHANNELS)
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void InitNSDMode(float *ptr_AF_Mask)
{
	//
	int i;
	//
	long Temp, Basic_NSD_Mode = 0; 

	// @Pl_ext
	if ((g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK) != INT_TELPROT_MODE_AES)
	{
		//
		// ****************************************************************	
		// *** Set Command mask  Rx , Tx and start mask *******************
		// ****************************************************************
		// ******************************************
		// *** Command Mask Ch1 switched off  *******
		// ******************************************
		RxCommandMaskCh1 = 0;
		//
		for (i=0;i<MAX_NUMBER_LP_COMMANDS; i++)
		{
			if(g_NSD_DSP_Config.NSD[0].CommandApplication[i] != COMMAND_OFF)
			{
				RxCommandMaskCh1 |= (0x00000001<<i);
			}
			else
			{

			}
		}
		//
		TxCommandMaskCh1 = RxCommandMaskCh1; // not correct yet, see below
		//
		// ******************************************
		// *** Command Mask Ch2 switched off ********
		// ****************************************** 
		RxCommandMaskCh2 = 0;
		//
		for (i=0;i<MAX_NUMBER_LP_COMMANDS; i++)
		{
			if(g_NSD_DSP_Config.NSD[1].CommandApplication[i] != COMMAND_OFF)
			{
				RxCommandMaskCh2 |= (0x00000001<<i);
			}
			else
			{

			}
		}
		//
		TxCommandMaskCh2 = RxCommandMaskCh2; // 
		//
		// ****************************************************************************
		// *** Remove not used Commands in Rx and Tx Command mask *********************
		// ****************************************************************************
		// -> this value tells which command input and output is not used on G4AI
		// @Pl_ext
		RxCommandMaskCh1 = ((RxCommandMaskCh1&(g_NSD_DSP_Config.NSD[0].UsedCommands		& 0x0000003f))| RX_COM_T);
		RxCommandMaskCh2 = ((RxCommandMaskCh2&(g_NSD_DSP_Config.NSD[1].UsedCommands		& 0x0000003f))| RX_COM_T);
		TxCommandMaskCh1 = ((TxCommandMaskCh1&((g_NSD_DSP_Config.NSD[0].UsedCommands	& 0x0003f000)>>12))|TX_COM_T);
		TxCommandMaskCh2 = ((TxCommandMaskCh2&((g_NSD_DSP_Config.NSD[1].UsedCommands	& 0x0003f000)>>12))|TX_COM_T);
		//
	};

	//
	// ****************************************************************************
	// *** Check NSD off for Ch1 and Ch2 to set Command mask correct **************
	// ****************************************************************************
	// This must be done because the HMI600  do not swich off the commands in the NSD600 instance which is not enabled!
	//
	g_NSD_DSP_Status.mode = g_NSD_DSP_Config.mode;
	// @Pl_ext
	Basic_NSD_Mode = (g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK);
	switch(Basic_NSD_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			*ptr_AF_Mask++ = 0.0;			// set send mask ch1 to 0.0
			*ptr_AF_Mask++ = 0.0;			// set send mask ch2 to 0.0
			*ptr_AF_Mask = 0.0;				// set send mask ch3 to 0.0
			//
			RxCommandMaskCh1 = 0x00000000;	// reset all rx-commands for Ch1
			TxCommandMaskCh1 = 0x00000000;	// reset all tx-commands for Ch1
			RxCommandMaskCh2 = 0x00000000;	// reset all rx-commands for Ch2
			TxCommandMaskCh2 = 0x00000000;	// reset all tx-commands for Ch2
			//
			StartMask_Ch1 = 0x00000000;		// no start signal enabled
			StartMask_Ch2 = 0x00000000;		// no start signal enabled
			//
			RxGuardMask_Ch1 = FALSE;		// no rx Guard signal enabled for Ch1
			RxGuardMask_Ch2 = FALSE;		// no rx Guard signal enabled for Ch2
			//
			break;
		case INT_TELPROT_MODE_AES:
			*ptr_AF_Mask++ = 1.0;			// set send mask ch1 to 1.0
			*ptr_AF_Mask++ = 0.0;			// set send mask ch2 to 0.0
			*ptr_AF_Mask = 0.0;				// set send mask ch3 to 0.0

			RxCommandMaskCh1 = 0x00000000;	// reset all rx-commands for Ch1
			TxCommandMaskCh1 = 0x00000000;	// reset all tx-commands for Ch1
			RxCommandMaskCh2 = 0x00000000;	// reset all rx-commands for Ch2
			TxCommandMaskCh2 = 0x00000000;	// reset all tx-commands for Ch2
			
			// enable LP commands and loop test command
			// @Pl_ext
			TxCommandMaskCh1 |= (COM_ABCD | TX_COM_T);
			RxCommandMaskCh1 |= (COM_ABCD | RX_COM_T);

			// enable AES commands
			if (g_NSD_DSP_Config.AES.AES600_Mode & AES600_AES_MODE)
			{
				TxCommandMaskCh1 |= AES_20_TX_CMD_MASK;
				RxCommandMaskCh1 |= AES_RX_CMD_MASK;
			}
			else
			{
				TxCommandMaskCh1 |= AES_12_TX_CMD_MASK;
				RxCommandMaskCh1 |= AES_RX_CMD_MASK;
			}

			// enable persistent command
			if(g_NSD_DSP_Config.AES.AES600_Mode & AES600_PERSIST_ON_OFF)
			{
				TxCommandMaskCh1 |= EXTENDED1_G4AI_TRIP_P_BIT;
				RxCommandMaskCh1 |= EXTENDED1_G4AI_TRIP_P_BIT;
			}

			StartMask_Ch1 = TDM_START_BIT_NSD;	// start signal enable for Ch1
			StartMask_Ch2 = 0x00000000;			// no start signal enabled for Ch2

			RxGuardMask_Ch1 = TRUE;			// rx Guard signal enabled for Ch1
			RxGuardMask_Ch2 = FALSE;		// no rx Guard signal enabled for Ch2

			aesCmdAckHoldTime = ONE_SECOND*g_NSD_DSP_Config.AES.HoldTime_Tx_Rx_Ack;

			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			*ptr_AF_Mask++ = 1.0;			// set send mask ch1 to 1.0 -> enable send signal for FE-DSP
			*ptr_AF_Mask++ = 0.0;			// set send mask ch2 to 0.0
			*ptr_AF_Mask = 0.0;				// set send mask ch3 to 0.0
			//
			RxCommandMaskCh2 = 0x00000000;	// reset all rx-commands for Ch2
			TxCommandMaskCh2 = 0x00000000;	// reset all tx-commands for Ch2
			//
			if (Basic_NSD_Mode == INT_TELPROT_MODE_HSD_25)
			{
				StartMask_Ch1 = 0x00000000;			// no start signal enabled for Ch1 HSD 2.5 kHz
			}
			else
			{
				StartMask_Ch1 = TDM_START_BIT_NSD;	// start signal enable for Ch1
			}
			StartMask_Ch2 = 0x00000000;			// no start signal enabled for Ch2
			//
			RxGuardMask_Ch1 = TRUE;				// rx Guard signal enabled for Ch1
			RxGuardMask_Ch2 = FALSE;			// no rx Guard signal enabled for Ch2
			//
			g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[7] = INTEGR_TIME_DETECTOR;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			*ptr_AF_Mask++ = 1.0;	// set send mask ch1 to 1.0 -> enable send signal in ch1 for FE-DSP
			*ptr_AF_Mask++ = 1.0;	// set send mask ch2 to 1.0 -> enable send signal in ch2 for FE-DSP
			*ptr_AF_Mask = 0.0;		// set send mask ch3 to 0.0
			//
			// command maske are set correctly
			// -> do nothing
			// set start mask for 2 channel mode
			StartMask_Ch1 = TDM_START_BIT_NSD;	// start signal enable for Ch1
			StartMask_Ch2 = TDM_START_BIT_NSD;	// start signal enable for Ch2
			//
			RxGuardMask_Ch1 = TRUE;				// rx Guard signal enabled for Ch1
			RxGuardMask_Ch2 = TRUE;				// rx Guard signal enabled for Ch2
			//
			g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[7] = INTEGR_TIME_DETECTOR;
			//
			break;
		case INT_TELPROT_MODE_HSD:
			*ptr_AF_Mask++ = 1.0;			// set send mask ch1 to 1.0 -> enable send signal for FE-DSP
			*ptr_AF_Mask++ = 0.0;			// set send mask ch2 to 0.0
			*ptr_AF_Mask = 0.0;				// set send mask ch3 to 0.0
			//
			RxCommandMaskCh2 = 0x00000000;	// reset all rx-commands for Ch2
			TxCommandMaskCh2 = 0x00000000;	// reset all tx-commands for Ch2
			//
			StartMask_Ch1 = 0x00000000;			// no start signal enabled for Ch1 HSD
			StartMask_Ch2 = 0x00000000;			// no start signal enabled for Ch2
			//
			RxGuardMask_Ch1 = TRUE;				// rx Guard signal enabled for Ch1
			RxGuardMask_Ch2 = FALSE;			// no rx Guard signal enabled for Ch2
			//
			g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[7] = INTEGR_TIME_DETECTOR_HSD;
			//
			break;
		default:
			// unknowed mode of NSD or AES
			*ptr_AF_Mask++ = 0.0;			// set send mask ch1 to 0.0
			*ptr_AF_Mask++ = 0.0;			// set send mask ch2 to 0.0
			*ptr_AF_Mask = 0.0;				// set send mask ch3 to 0.0
			//
			// -> reset all commands 
			RxCommandMaskCh1 = 0x00000000;	// reset all rx-commands for Ch1
			TxCommandMaskCh1 = 0x00000000;	// reset all tx-commands for Ch1
			RxCommandMaskCh2 = 0x00000000;	// reset all rx-commands for Ch2
			TxCommandMaskCh2 = 0x00000000;	// reset all tx-commands for Ch2
			//
			StartMask_Ch1 = 0x00000000;		// no start signal enabled for Ch1
			StartMask_Ch2 = 0x00000000;		// no start signal enabled for Ch2
			//
			RxGuardMask_Ch1 = FALSE;		// no rx Guard signal enabled for Ch1
			RxGuardMask_Ch2 = FALSE;		// no rx Guard signal enabled for Ch2
			//
			break;
	}
	g_Develop_Status_NSD.FW_Version = FW_VERSION_NSD;
	//
	Command_Input_Ch1 = 0;	// send pilot
	Rx_Command_Ch1_Det = 0;
	Rx_Command_Ch1_ToG4AI = 0;
	RxGuardStateCh1 = FALSE;
	//
	Command_Input_Ch2 = 0;
	Rx_Command_Ch2_Det = 0;
	Rx_Command_Ch2_ToG4AI = 0;
	RxGuardStateCh2 = FALSE;

	Start_Ch1 = 0;
	Start_Ch2 = 0;

	UnblockingBitCh1 = 0;
	UnblockingBitCh2 = 0;

	GuardLevelAlarmCh1 = 0;
	GuardLevelAlarmCh2 = 0;

	Rx_Alarm_OR_MaskCh1 = OR_MASKE_OUT_AT_RXALARM;
	Rx_Alarm_AND_MaskCh1 = AND_MASK_OUT_AT_RXALARM;

	Rx_Alarm_OR_MaskCh2 = OR_MASKE_OUT_AT_RXALARM;
	Rx_Alarm_AND_MaskCh2 = AND_MASK_OUT_AT_RXALARM;

	g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms = 0;	// reset all alarms for NSD Ch1
	g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms = 0;	// reset all alarms for NSD Ch2

	g_NSDLocalLoopModeCh1 = 0;						// Local Loop off Ch1
	g_NSDLocalLoopModeCh2 = 0;						// Local Loop off Ch2

	NSD_Mode_Changed = FALSE;						// 

	Looped_Cmd_Ch1 = 0;
	Looped_Cmd_Ch2 = 0;

}


/*****************************************************************************
  Function:		updateAlarms()	
  Description:	Map the alarms to the internal tdm_bus Alarm word
  Inputs:		Alarms in g_NSD_DSP_Status.NSD[0/1].NSD_Alarms 
  Outputs:		g_intNSDTDM_TxStruct0/1_Ptr->alarm 
  Return:		-
  Globals:		g_NSD_DSP_Status.NSD[0].NSD_Alarms, g_NSD_DSP_Status.NSD[1].NSD_Alarms, 
				g_intNSDTDM_TxStruct0_Ptr->alarm, g_intNSDTDM_TxStruct1_Ptr->alarm
  Important:	-

******************************************************************************/

static void SEGMENT_EXT_CODE updateAlarms(void) 
{
	//
	// **************************************************************
	// *** update NSD Alarms for Ch1 ********************************
	// **************************************************************
	//
	// update no trip and no guard
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_NO_TRIP_AND_GUARD_ALARM) == NSD_NO_TRIP_AND_GUARD_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1NoTripAndNoGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1NoTripAndNoGuardAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1NoTripAndNoGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1NoTripAndNoGuardAlarm = FALSE;
	}
	// update trip and guard
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_TRIP_AND_GUARD_ALARM) == NSD_TRIP_AND_GUARD_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1TripAndGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1TripAndGuardAlarm = TRUE; 
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1TripAndGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1TripAndGuardAlarm = FALSE;
	}
	// update rx guard low level alarm
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_RX_GUARD_LOW_LEVEL_ALARM) == NSD_RX_GUARD_LOW_LEVEL_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1RxLowLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1RxLowLevelAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1RxLowLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1RxLowLevelAlarm = FALSE;
	}
	// update rx guard high level alarm
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_RX_GUARD_HIGH_LEVEL_ALARM) == NSD_RX_GUARD_HIGH_LEVEL_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1RxHighLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1RxHighLevelAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1RxHighLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1RxHighLevelAlarm = FALSE;
	}
	// update SNR alarm
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_SNR_ALARM) == NSD_SNR_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1SNRAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1SNRAlarm = TRUE; 
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1SNRAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1SNRAlarm = FALSE;
	}
	// update output set to predefined state
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_OUTPUT_SET_ALARM) == NSD_OUTPUT_SET_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1OutputSetToPredefinedStateAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1OutputSetToPredefinedStateAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1OutputSetToPredefinedStateAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1OutputSetToPredefinedStateAlarm = FALSE;
	}

	// update command outputs blocked alarm
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_CMD_OUTPUTS_BLOCKED_ALARM) == NSD_CMD_OUTPUTS_BLOCKED_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1CommandOutputsBlockedAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1CommandOutputsBlockedAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1CommandOutputsBlockedAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1CommandOutputsBlockedAlarm = FALSE;
	}

	// update nsd loop test alarm
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_LOOP_TEST_ALARM) == NSD_LOOP_TEST_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1LoopTestAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1LoopTestAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1LoopTestAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1LoopTestAlarm = FALSE;
	}
	// update nsd loop test warning
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_LOOP_TEST_WARNING) == NSD_LOOP_TEST_WARNING)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1LocalLoopTestWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1LocalLoopTestWarning = TRUE; 
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1LocalLoopTestWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1LocalLoopTestWarning = FALSE;
	}
	// update Rx trip continuouse alarm
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_RX_TRIP_DURATION_ALARM) == NSD_RX_TRIP_DURATION_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1RxContinuousAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1RxContinuousAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1RxContinuousAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1RxContinuousAlarm = FALSE;
	}
	// update local loop warning
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_LOCAL_TEST_MODE_WARNING) == NSD_LOCAL_TEST_MODE_WARNING)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1LocalLoopActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1LocalLoopActiveWarning = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1LocalLoopActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1LocalLoopActiveWarning = FALSE;
	}
	// update test signal warning
	if((g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms & NSD_TEST_SIGNAL_WARNING) == NSD_TEST_SIGNAL_WARNING)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1TestSignalActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1TestSignalActiveWarning = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe1TestSignalActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe1TestSignalActiveWarning = FALSE;
	}
	//
	// **************************************************************
	// *** update NSD Alarms for Ch2 ********************************
	// **************************************************************
	//
	// update no trip and no guard
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_NO_TRIP_AND_GUARD_ALARM) == NSD_NO_TRIP_AND_GUARD_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2NoTripAndNoGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2NoTripAndNoGuardAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2NoTripAndNoGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2NoTripAndNoGuardAlarm = FALSE;
	}
	// update trip and guard
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_TRIP_AND_GUARD_ALARM) == NSD_TRIP_AND_GUARD_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2TripAndGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2TripAndGuardAlarm = TRUE; 
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2TripAndGuardAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2TripAndGuardAlarm = FALSE;
	}
	// update rx guard low level alarm
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_RX_GUARD_LOW_LEVEL_ALARM) == NSD_RX_GUARD_LOW_LEVEL_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2RxLowLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2RxLowLevelAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2RxLowLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2RxLowLevelAlarm = FALSE;
	}
	// update rx guard high level alarm
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_RX_GUARD_HIGH_LEVEL_ALARM) == NSD_RX_GUARD_HIGH_LEVEL_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2RxHighLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2RxHighLevelAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2RxHighLevelAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2RxHighLevelAlarm = FALSE;
	}
	// update SNR alarm
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_SNR_ALARM) == NSD_SNR_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2SNRAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2SNRAlarm = TRUE; 
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2SNRAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2SNRAlarm = FALSE;
	}
	// update output set to predefined state
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_OUTPUT_SET_ALARM) == NSD_OUTPUT_SET_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2OutputSetToPredefinedStateAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2OutputSetToPredefinedStateAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2OutputSetToPredefinedStateAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2OutputSetToPredefinedStateAlarm = FALSE;
	}
	// update nsd loop test alarm
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_LOOP_TEST_ALARM) == NSD_LOOP_TEST_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2LoopTestAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2LoopTestAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2LoopTestAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2LoopTestAlarm = FALSE;
	}
	// update nsd loop test warning
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_LOOP_TEST_WARNING) == NSD_LOOP_TEST_WARNING)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2LocalLoopTestWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2LocalLoopTestWarning = TRUE; 
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2LocalLoopTestWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2LocalLoopTestWarning = FALSE;
	}
	// update Rx trip continuouse alarm
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_RX_TRIP_DURATION_ALARM) == NSD_RX_TRIP_DURATION_ALARM)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2RxContinuousAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2RxContinuousAlarm = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2RxContinuousAlarm = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2RxContinuousAlarm = FALSE;
	}
	// update local loop warning
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_LOCAL_TEST_MODE_WARNING) == NSD_LOCAL_TEST_MODE_WARNING)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2LocalLoopActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2LocalLoopActiveWarning = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2LocalLoopActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2LocalLoopActiveWarning = FALSE;
	}
	// update test signal warning
	if((g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms & NSD_TEST_SIGNAL_WARNING) == NSD_TEST_SIGNAL_WARNING)
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2TestSignalActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2TestSignalActiveWarning = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.tpe2TestSignalActiveWarning = g_intNSDTDM_TxStruct1_Ptr->alarm.tpe2TestSignalActiveWarning = FALSE;
	}
	////////////////////// Common ///////////////////////////////////////////////////////
	if(memoryCheckDSP_CRC_Alarm())
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.progMemCRCErr = g_intNSDTDM_TxStruct1_Ptr->alarm.progMemCRCErr = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->alarm.progMemCRCErr = g_intNSDTDM_TxStruct1_Ptr->alarm.progMemCRCErr = FALSE;
	}

}


/*****************************************************************************
  Function:		CheckLocalLoopMode()	
  Description:	Checks if the Local loop mode for TPE 1/2 has changed and set the 
				Local loop mode for the corresponding channel on or off
  Inputs:		NSD_Mode_Changed, g_NSDLocalLoopMode 
  Outputs:		g_NSDLocalLoopModeCh1, g_NSDLocalLoopModeCh2
  Return:		-
  Globals:		-
  Important:	-

******************************************************************************/

void SEGMENT_EXT_CODE CheckLocalLoopMode()
{

	if(NSD_Mode_Changed == TRUE)
	{
		// NSD mode have changed -> do the settings according the Mode received from HMI
		NSD_Mode_Changed = FALSE;
		// select channel
		if((g_NSDLocalLoopMode & CH1_LOCAL_LOOP) == CH1_LOCAL_LOOP)
		{
			// settings for Ch1
			if((g_NSDLocalLoopMode & TPE_LOCAL_LOOP_ON) == TPE_LOCAL_LOOP_ON)	
			{
				g_NSDLocalLoopModeCh1 = TPE_LOCAL_LOOP_ON;
				g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms |= NSD_LOCAL_TEST_MODE_WARNING;	
			}
			else 
			{
				g_NSDLocalLoopModeCh1 = TPE_LOCAL_LOOP_OFF;
				g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms &= ~NSD_LOCAL_TEST_MODE_WARNING;
			}
		}
		else
		{
			// settings for ch2
			if((g_NSDLocalLoopMode & TPE_LOCAL_LOOP_ON) == TPE_LOCAL_LOOP_ON)	
			{
				g_NSDLocalLoopModeCh2 = TPE_LOCAL_LOOP_ON;
				g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms |= NSD_LOCAL_TEST_MODE_WARNING;	
			}
			else 
			{
				g_NSDLocalLoopModeCh2 = TPE_LOCAL_LOOP_OFF;
				g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms &= ~NSD_LOCAL_TEST_MODE_WARNING;
			}
		}
	}
	else
	{
		// ITS local loop mode not changed
	}
}


/*****************************************************************************
  Function:		UpdateBoostManager()	
  Description:	Update the signalas TPE1/2_BoostReq, TPE1/2_Test, TPE1/2_Start_or_Rx
				according the actual input signals
  Inputs:		Command_Input_Ch1/2, Rx_Command_Ch1/2_Det, Start_Ch1/2 
  Outputs:		
  Return:		-
  Globals:		-
  Important:	-

******************************************************************************/

void UpdateBoostManager()
{
		// ************************************************
		// *** Handling for Ch1 ***************************
		// ************************************************
		//
		// boost request for Ch 1 
		//
		g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_BoostReq = boostRequestCh1;
		g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_BoostReq = boostRequestCh1;
		
		//
		// Handling Test signal NSD Ch1
		// @Pl_ext
		if(Command_Input_Ch1 == TX_COM_T)
		{
			// loop test send is activ
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Test = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Test = TRUE;
		}
		else
		{
			// loop test is not activ
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Test = FALSE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Test = FALSE;
		}
		//
		// Handling start and Rx command for Ch1
		//
		if(((Rx_Command_Ch1_Det) != 0)|| (Start_Ch1 == TDM_START_BIT_NSD))
		{
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Start_or_Rx  = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Start_or_Rx  = TRUE;
		}
		else
		{
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Start_or_Rx  = FALSE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Start_or_Rx  = FALSE;
		}
		//	
		// ************************************************
		// *** Handling for Ch2 ***************************
		// ************************************************
		//
		// boost request for Ch2
		//
		g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_BoostReq = boostRequestCh2;
		g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_BoostReq = boostRequestCh2;

		//
		// Handling Test signal NSD Ch2
		// @Pl_ext
		if(Command_Input_Ch2 == TX_COM_T)
		{
			// loop test send is activ
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_Test = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_Test = TRUE;
		}
		else
		{
			// loop test is not activ
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_Test = FALSE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_Test = FALSE;
		}

		//
		// Handling start and Rx command for Ch2
		//
		if((Rx_Command_Ch2_Det != 0)|| (Start_Ch2 == TDM_START_BIT_NSD))
		{
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_Start_or_Rx  = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_Start_or_Rx  = TRUE;
		}
		else
		{
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_Start_or_Rx  = FALSE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_Start_or_Rx  = FALSE;
		}
}


/*****************************************************************************
  Function:		UpdateNSD_Events()	
  Description:	Update the NSD events for TPE1 and TPE2  
  Inputs:		- 
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	-

******************************************************************************/

static void SEGMENT_EXT_CODE UpdateNSD_Events()
{
	//
	// update LT events for Ch1
	//
	if((LT_Event_Ch1&CYCLIC_LT_SENT_EVENT) == CYCLIC_LT_SENT_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_LT_Sent = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_LT_Sent = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_LT_Sent = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_LT_Sent = FALSE;
	}
	//
	if((LT_Event_Ch1&LT_REFLECTED_EVENT) == LT_REFLECTED_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_LT_Reflected = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_LT_Reflected = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_LT_Reflected = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_LT_Reflected = FALSE;
	}
	//
	if((LT_Event_Ch1&LT_FAILED_EVENT) == LT_FAILED_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_LT_Failed = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_LT_Failed = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_LT_Failed = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_LT_Failed = FALSE;
	}
	//
	if((LT_Event_Ch1&MANUAL_LT_SENT_EVENT) == MANUAL_LT_SENT_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_Manual_LT_Sent = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_Manual_LT_Sent = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_Manual_LT_Sent = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_Manual_LT_Sent = FALSE;
	}
	//
	if(LT_Event_Ch1&MANUAL_LT_FAILED_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_Manual_LT_Failed = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_Manual_LT_Failed = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_Manual_LT_Failed = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_Manual_LT_Failed = FALSE;
	}
	//
	// update LT events for Ch2
	//
	if((LT_Event_Ch2&CYCLIC_LT_SENT_EVENT) == CYCLIC_LT_SENT_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_LT_Sent = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_LT_Sent = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_LT_Sent = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_LT_Sent = FALSE;
	}
	//
	if((LT_Event_Ch2&LT_REFLECTED_EVENT) == LT_REFLECTED_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_LT_Reflected = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_LT_Reflected = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_LT_Reflected = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_LT_Reflected = FALSE;
	}
	//
	if((LT_Event_Ch2&LT_FAILED_EVENT) == LT_FAILED_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_LT_Failed = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_LT_Failed = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_LT_Failed = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_LT_Failed = FALSE;
	}
	//
	if((LT_Event_Ch2&MANUAL_LT_SENT_EVENT) == MANUAL_LT_SENT_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_Manual_LT_Sent = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_Manual_LT_Sent = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_Manual_LT_Sent = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_Manual_LT_Sent = FALSE;
	}
	//
	if(LT_Event_Ch2&MANUAL_LT_FAILED_EVENT)
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_Manual_LT_Failed = TRUE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_Manual_LT_Failed = TRUE;
	}
	else
	{
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_Manual_LT_Failed = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_Manual_LT_Failed = FALSE;
	}
}


/*****************************************************************************
  Function:		setAckBits()		
  Description:	Returns the active acknowledge bits
  Inputs:		command inputs, received commands
  Outputs:		-
  Return:		acknowledge bits
  Globals:		g_NSD_DSP_Config
  Important:	-

******************************************************************************/

static unsigned long setAckBits(long txCmd, long rxCmd, long holdTime, BOOL resetRxAckAES)
{
	unsigned long ackBits;

	static long txAckOn = 0;
	static long txTimer = 0;
	static long txCmdOld = 0;

	static long rxAckOn = 0;
	static long rxTimer = 0;
	static long rxCmdOld = 0;

	// check if AES enabled
	// @Pl_ext
	if ((g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
	{
		// remove unwanted bits
		// @Pl_ext
		txCmd &= (ALL_LP_AES_TX_COMMANDS | EXTENDED1_G4AI_TRIP_P_BIT);
		rxCmd &= (ALL_LP_AES_RX_COMMANDS | EXTENDED1_G4AI_TRIP_P_BIT);
		// set tx cmd ack
		if (txAckOn)	// tx cmd ack bit on
		{
			if ((txCmd ^ txCmdOld ) & txCmd)		// detect positive edge (command switching on)
			{
				txTimer = holdTime;
			}
			else if (txTimer < 1)
			{
				txAckOn = 0;
			}
			txTimer--;
		}
		else		// tx cmd ack bit off
		{
			if ((txCmd ^ txCmdOld ) & txCmd)		// detect positive edge (command switching on)	
			{
				txTimer = holdTime;
				txAckOn = EXTENDED1_G4AI_AES_TX_CMD_ACK_SUM;
			}
		
		}
		txCmdOld = txCmd;

		// set rx cmd ack
		if (rxAckOn)	// rx cmd ack bit on
		{
			if ((rxCmd ^ rxCmdOld) & rxCmd)		// detect positive edge (command switching on)
			{
				rxTimer = holdTime;
			}
			else if (rxTimer < 1)
			{
				rxAckOn = 0;
			}
			rxTimer--;
		}
		else		// rx cmd ack bit off
		{
			if ((rxCmd ^ rxCmdOld) & rxCmd)		// detect positive edge (command switching on)
			{
				rxTimer = holdTime;
				rxAckOn = EXTENDED1_G4AI_AES_RX_CMD_ACK_SUM;
			}
		
		}
		rxCmdOld = rxCmd;		

		if (resetRxAckAES)
		{
			ackBits = txAckOn;
		}
		else
		{
			ackBits = txAckOn | rxAckOn;
		}

	}
	else	// no AES commands
	{
		// @Pl_ext
		txCmd &= (ALL_LP_COMMANDS_A_F); 
		ackBits = txCmd << 9;
	}

	return(ackBits);
}



// ********************************************************************************************************
// ********************************************************************************************************
// ********************************************************************************************************
// *** NSD-DSP-MAIN-PROGRAMM  *****************************************************************************
// ********************************************************************************************************
// ********************************************************************************************************
// ********************************************************************************************************
//
void main (void)
{
	// idle counters
	static unsigned long startValue;
	static unsigned long idleCounter;
	static unsigned long idleCounterMinIndex;

	static unsigned long mainCaseCounter;
	static unsigned long subCaseCounter4;

	static unsigned long Last_Event_Ch1, Last_Event_Ch2;

	static float AF_Mask[N_APLC_CHANNELS];

	static unsigned long persCmdRx;

	volatile long *Int_RxBufferPtr;
	volatile long Input_Flag;

	static unsigned long ackBitsCh1;
	static unsigned long ackBitsCh2;

	static BOOL blockCmdOutputs;
	static BOOL resetRxAckBitAES;

	// Test variables G4AI
	static long ReceivedDataFromG4AI[NO_TDM_BASIC_TIMESLOTS] = {0,0,0,0,0,0,0,0,0,0};
	static long DataToSendToG4AI[NO_TDM_BASIC_TIMESLOTS] = {0,0,0,0,0,0,0,0,0,0};
	
	//
	int i;
	static long CounterI0 = 0;
//**************************** new init *******************************************************
	Init_DSP_Register();
	asm("nop;");			// Emulator: run up to here and reload *.dxe again (SDRAM settings)

	// init SPORT DMA pointers - acessing the DMA pointer is possible anytime - no more: if (internalTDM_Tx_Ptr) ....
	Init_InternalTDM_BufferPtr((unsigned long*) &internalTDM_Rx_Ptr[0], (unsigned long*) &internalTDM_Tx_Ptr[0]);
	Init_InternalTDM_BufferPtr((unsigned long*) &internalTDM_Rx_Ptr[1], (unsigned long*) &internalTDM_Tx_Ptr[1]);
	Init_ExternalTDM_BufferPtr((unsigned long*) &externalTDM_Rx_Ptr[0], (unsigned long*) &externalTDM_Tx_Ptr[0]);
	Init_ExternalTDM_BufferPtr((unsigned long*) &externalTDM_Rx_Ptr[1], (unsigned long*) &externalTDM_Tx_Ptr[1]);

	MemoryCheckStartupSHARC(TRUE);

	memoryCheckRespondToMaster();

	initialize_DSP(&g_NSD_DSP_Config); // attention: no config!!!!!!!@HPR@
	// ***********************************************************
	// *** Init NSD **********************************************
	// ***********************************************************
	getConfiguration(&g_NSD_DSP_Config);
	//
	initialize_DSP(&g_NSD_DSP_Config);
	//
	// @Pl_ext
	if ((g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
	{
		AES_Transmitter_init(&g_NSD_DSP_Config);
		AES_Detector_init(&g_NSD_DSP_Config);
	}
	else
	{
		Init_NSD_Transmitter(&g_NSD_DSP_Config);
		init_Detector(&g_NSD_DSP_Config);
	};

	Get_Ptr_Det_Amp_Correction(&ptr_Det_Sin_Ch1, &ptr_Det_Cos_Ch1, &ptr_Det_Sin_Ch2, &ptr_Det_Cos_Ch2);
	Get_Ptr_RXIF_Amp_Correction(&ptr_RxIF_Sin_Ch1, &ptr_RxIF_Cos_Ch1, &ptr_RxIF_Sin_Ch2, &ptr_RxIF_Cos_Ch2);
	init_rx_guard_nsd(&g_NSD_DSP_Config);
	InitNSDMode(&AF_Mask[0]);
	Init_NSDUnblocking(&g_NSD_DSP_Config);
	Init_Monitor_Trip_Guard(&g_NSD_DSP_Config);
	Init_Cyclic_LT(&g_NSD_DSP_Config);
	init_Output_At_RxAlarm(&g_NSD_DSP_Config, &g_NSD_DSP_Status);
	Init_SNR_Monitoring_Guard(&g_NSD_DSP_Config);
	Init_RxCommandDurationMonit(&g_NSD_DSP_Config);
	Init_CompensationOfEvaluationTime(&g_NSD_DSP_Config);
	initNSD_Testsignal();
	ptr_Konfig_NSD_Testsignal_Gen = getNSDTestsignal_Gen_Konfig();
	//
	//*(long*)(IOFLAG) &= (~(FLG11O|FLG10O|FLG9O|FLG8O));					// set Flags (4-7) as input for NSD commands
	//*(long*)(IOFLAG) |= FLG11|FLG10|FLG9|FLG8;							// set Flags (8-11) High
	//
	// ***********************************************************
	// *** End Init NSD ******************************************
	// ***********************************************************
	RestartTimer5k();
	IR_TimerCounter = 1;
	//
	asm("bit set mode2  FLG1O;");				// temporary set as output for boot testing -> REMOVE
	// ***********************************************************
	// *** init Boostmanager paramters ***************************
	// ***********************************************************
	g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_BoostReq = FALSE;
	g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_BoostReq = FALSE;

	g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_BoostReq = FALSE;
	g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_BoostReq = FALSE;

	g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Start_or_Rx = FALSE;
	g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Start_or_Rx = FALSE;

	g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_Start_or_Rx = FALSE;
	g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_Start_or_Rx = FALSE;

	g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Test = FALSE;
	g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Test = FALSE;

	g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE2_Test = FALSE;
	g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE2_Test = FALSE;
	//
	//
	LT_Event_Ch1 = 0;
	LT_Event_Ch2 = 0;
	Last_Event_Ch1 = 0;
	Last_Event_Ch2 = 0;
	//
	//
	// ********************************************************************************************
	// *** main loop ******************************************************************************
	// ********************************************************************************************
	// ********************************************************************************************
    //	
    while (1) {
		// repetiton rate = 5.333 kHz
		static unsigned long myLifesign;
		// changegn
		static BOOL StopCompensation_Ch1 = FALSE;
		static BOOL StopCompensation_Ch2 = FALSE;
		// changegn end 

		//
		// **********************************************
		// *** Set Output flag 8...11 to high -> high shows the actual not used calculation time
		// **********************************************
		//
		*(long*)(IOFLAG) |= FLG11|FLG10|FLG9|FLG8;
		// 
		startValue = count_start();
		//
		while(IR_TimerCounter && IR_BlockCounter )
		{
			asm("nop;");
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}
		idleCounter = count_end(startValue);
		idleCounter_update(idleCounterMinIndex, idleCounter);
		//
		// **********************************************
		// *** Reset Output flags 8..11 to low -> low means used calculation time on the dsp
		// **********************************************
		*(long*)(IOFLAG) &=(~(FLG11|FLG10|FLG9|FLG8));
		// cast internal TDM bus rx and tx pointers
		//
		g_intTDM_RxStruct0_Ptr = (t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr[0];
		g_intTDM_RxStruct1_Ptr = (t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr[1];

		g_intNSDTDM_TxStruct0_Ptr = (t_NSD_output_tdm_block*)internalTDM_Tx_Ptr[0];
		g_intNSDTDM_TxStruct1_Ptr = (t_NSD_output_tdm_block*)internalTDM_Tx_Ptr[1];
		
		RestartTimer5k();

		// *******************************************************************
		// This makes the synchronisation NSD to Master to send/receive always
		// the right extended timeslot data (generated on NSD) on external TDM 
		// bus synchronous to the basic timeslot data generated by the master
		// do not move this function!!!!!!!!! must be in the first 1/3 of the main!!!
		// *******************************************************************
		if(g_intTDM_RxStruct0_Ptr->NSD_Sync_TS00 & BIT0)
		{
			IR_BlockCounter = 2;	// decremented by interrupt SPRT1
			IR_TimerCounter = 1;
		}
		else
		{
			IR_BlockCounter = 3;	// decremented by interrupt SPRT1
			IR_TimerCounter = 2;	// Attention! In the else condition the counter 
									// must be twice otherwise no change to sync!!!
		}
		// *******************************************************************
		// *** Send/Receive external TDM-bus data to/from G4AI ***************
		// *******************************************************************
		//
		ReceiveDataFromG4AI(&ReceivedDataFromG4AI[0]);	// do not move this function!!!!!!!!! must be in the first 1/3 of the main!!!
		SendDataToG4AI(&DataToSendToG4AI[0]);			// do not move this function!!!!!!!!! must be in the first 1/3 of the main!!!
		//
		// *******************************************************************
		// *** Read data from external TDM-Bus (G4AI)                *********
		// *******************************************************************

		// update tx commands
		Command_Input_Ch1 = (((ReceivedDataFromG4AI[0]) | (Command_Input_Ch1 & TX_COM_T)) & TxCommandMaskCh1);
		//
		// check local loop mode for ch1
		if(g_NSDLocalLoopModeCh1 == TPE_LOCAL_LOOP_ON)
		{
			// @Pl_ext
			if ((g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
			{
				Looped_Cmd_Ch1 = AES_Transmitter_localCommandLoop(Command_Input_Ch1);
			}
			else	// NSD
			{
				Looped_Cmd_Ch1 = Command_Input_Ch1;
			}

			Command_Input_Ch1 = 0;
		}
		//
		Command_Input_Ch2 = (((ReceivedDataFromG4AI[1]) | (Command_Input_Ch2 & TX_COM_T)) & TxCommandMaskCh2);
		//
		// check local loop mode for ch2
		if(g_NSDLocalLoopModeCh2 == TPE_LOCAL_LOOP_ON)
		{
			Looped_Cmd_Ch2 = Command_Input_Ch2;
			Command_Input_Ch2 = 0;
		}

		//Update Start signal
		StartInput_Ch1 = (ReceivedDataFromG4AI[0]&StartMask_Ch1);
		StartInput_Ch2 = (ReceivedDataFromG4AI[1]&StartMask_Ch2);

		//
		// **********************************************************************
		// *** NSD Transmitter **************************************************
		// **********************************************************************
		//
		// @Pl_ext
		if ((g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
		{
			boostRequestCh1 = AES_Transmitter_activate(&Tx_AF_Ch1_I[0], &Tx_AF_Ch1_Q[0], Command_Input_Ch1);
			boostRequestCh2 = FALSE;
		}
		else
		{
			boostRequestCh1 = Transmitter(&Tx_AF_Ch1_I[0], &Tx_AF_Ch1_Q[0], Command_Input_Ch1, TPE1);
			boostRequestCh2 = Transmitter(&Tx_AF_Ch2_I[0], &Tx_AF_Ch2_Q[0], Command_Input_Ch2, TPE2);
		}
		//
		// TDM bus services
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[0], (unsigned long *) internalTDM_Tx_Ptr[0]);
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[1], (unsigned long *) internalTDM_Tx_Ptr[1]);
		//
		// *******************************************************************
		// *** Write NSD - Transmitt Signal to internal TDM-Bus to FE-DSP  ***
		// *******************************************************************
		//
		// Write af-transmit-signal NSD Ch1 to internal tdm-bus
		g_intNSDTDM_TxStruct0_Ptr->nsd_tx[0].re = Tx_AF_Ch1_I[0] * AF_Mask[0];
		g_intNSDTDM_TxStruct0_Ptr->nsd_tx[0].im = Tx_AF_Ch1_Q[0] * AF_Mask[0];
		//
		g_intNSDTDM_TxStruct1_Ptr->nsd_tx[0].re = Tx_AF_Ch1_I[1] * AF_Mask[0];
		g_intNSDTDM_TxStruct1_Ptr->nsd_tx[0].im = Tx_AF_Ch1_Q[1] * AF_Mask[0];
		//
		// Write af-transmit-signal NSD Ch2 to internal tdm-bus
		g_intNSDTDM_TxStruct0_Ptr->nsd_tx[1].re = Tx_AF_Ch2_I[0] * AF_Mask[1];
		g_intNSDTDM_TxStruct0_Ptr->nsd_tx[1].im = Tx_AF_Ch2_Q[0] * AF_Mask[1];
		//
		g_intNSDTDM_TxStruct1_Ptr->nsd_tx[1].re = Tx_AF_Ch2_I[1] * AF_Mask[1];
		g_intNSDTDM_TxStruct1_Ptr->nsd_tx[1].im = Tx_AF_Ch2_Q[1] * AF_Mask[1];
		//
		// Write af-transmit-signal APLC Ch3 to internal tdm-bus
		// -> always set to zero 0.0 because ch3 is not used for NSD at all
		g_intNSDTDM_TxStruct0_Ptr->nsd_tx[2].re = 0.0;
		g_intNSDTDM_TxStruct0_Ptr->nsd_tx[2].im = 0.0;
		//
		g_intNSDTDM_TxStruct1_Ptr->nsd_tx[2].re = 0.0;
		g_intNSDTDM_TxStruct1_Ptr->nsd_tx[2].im = 0.0;
		//
		// *******************************************************************
		// *** Read Rx-data internal tdm-bus (from FE-DSP)       *************
		// *******************************************************************
		// Read Data for Rx channel 1
		Rx_AF_Ch1[0] = g_intTDM_RxStruct0_Ptr->APLC_RxSignalNotEqualized[0] * AF_Mask[0];
		Rx_AF_Ch1[1] = g_intTDM_RxStruct1_Ptr->APLC_RxSignalNotEqualized[0] * AF_Mask[0];
		//
		// do float check for rx af signal samples for ch1
		ArrayFloatCheck(Rx_AF_Ch1, 2);
		// Read Data for Rx channel 2
		//
		Rx_AF_Ch2[0] = g_intTDM_RxStruct0_Ptr->APLC_RxSignalNotEqualized[1] * AF_Mask[1];
		Rx_AF_Ch2[1] = g_intTDM_RxStruct1_Ptr->APLC_RxSignalNotEqualized[1] * AF_Mask[1];
		//
		// do float check for rx af signal samples for ch2
		ArrayFloatCheck(Rx_AF_Ch2, 2);
		//
		FE_snr_Ch1 = g_intTDM_RxStruct0_Ptr->APLC_SNR_Pilot[0];
		FE_snr_Ch2 = g_intTDM_RxStruct0_Ptr->APLC_SNR_Pilot[1];
		//
		// To send Debug words over the internal TDM Bus use this:
		g_intNSDTDM_TxStruct0_Ptr->debug_word_1 = 0x5555AAAA;
		g_intNSDTDM_TxStruct1_Ptr->debug_word_1 = 0x5555AAAA;
		g_intNSDTDM_TxStruct0_Ptr->debug_word_2 = 0xFF00FF00;
		g_intNSDTDM_TxStruct1_Ptr->debug_word_2 = 0xFF00FF00;
		//
		// *******************************************************************
		// *** NSD detector    ***********************************************
		// *******************************************************************

		if ((g_NSD_DSP_Config.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
		{
		// change_gn AES trip and guard alarm boolian added
			Rx_Command_Ch1_Det = AES_Detector_activate(&Rx_AF_Ch1[0], RxGuardStateCh1, &DetectorActive_Ch1);	
		}
		else 
		{
			// change_gn NSD trip and guard alarm boolian added
			Rx_Command_Ch1_Det = Detector_NSD(&Rx_AF_Ch1[0], TPE1, &DetectorActive_Ch1);
			Rx_Command_Ch2_Det = Detector_NSD(&Rx_AF_Ch2[0], TPE2, &DetectorActive_Ch2);
		}
		//
		// *******************************************************************
		// *** Do compensation of evaluation time for commands ***************
		// *******************************************************************
		//
		// changegn
		Rx_Command_Ch1_Det = CompensationOfEvaluationTime(Rx_Command_Ch1_Det, TPE1, StopCompensation_Ch1);
		Rx_Command_Ch2_Det = CompensationOfEvaluationTime(Rx_Command_Ch2_Det, TPE2, StopCompensation_Ch2);
		//		Rx_Command_Ch1_Det = CompensationOfEvaluationTime(Rx_Command_Ch1_Det, TPE1);
		//		Rx_Command_Ch2_Det = CompensationOfEvaluationTime(Rx_Command_Ch2_Det, TPE2);
		// changegn end
		//
		// *******************************************************************
		// *** Rx_Guard detector NSD *****************************************
		// *******************************************************************
		// *** NSD Ch1 
		RxGuardStateCh1 = rx_guard_nsd(Rx_AF_Ch1, TPE1, Rx_Command_Ch1_Det, &g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms);
		//
		RxGuardStateCh1 = (RxGuardStateCh1 && RxGuardMask_Ch1);
		//
		// *** NSD Ch2
		RxGuardStateCh2 = rx_guard_nsd(Rx_AF_Ch2, TPE2, Rx_Command_Ch2_Det, &g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms);
		//
		RxGuardStateCh2 = (RxGuardStateCh2 && RxGuardMask_Ch2);
		//
		// *******************************************************************
		// *** Monitoring "Trip and Guard" and "No trip and Guard" ***********
		// *******************************************************************
		//
		Monitor_Trip_Guard(&Rx_Command_Ch1_Det, RxGuardStateCh1, &g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms, TPE1, DetectorActive_Ch1);
		Monitor_Trip_Guard(&Rx_Command_Ch2_Det, RxGuardStateCh2, &g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms, TPE2, DetectorActive_Ch2);
		// changegn
		StopCompensation_Ch1 = RxGuardStateCh1;
		StopCompensation_Ch2 = RxGuardStateCh2;
		// changegn end
		
		//
		// *******************************************************************
		// *** Set "Suppress Alarms" Bit if valid command received and HSD ***
		// *******************************************************************
		// 
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_Suppress_Alarms = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_Suppress_Alarms = FALSE;
		g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_Suppress_Alarms = FALSE;
		g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_Suppress_Alarms = FALSE;
		// @Pl_ext
		// HSD mode: INT_TELPROT_MODE_HSD or INT_TELPROT_MODE_HSD_25
		if ((Rx_Command_Ch1_Det != 0) && (IS_HSD600(g_NSD_DSP_Config) || IS_HSD600_25(g_NSD_DSP_Config)))
		{
			g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE1_Suppress_Alarms = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE1_Suppress_Alarms = TRUE;
		}
		// @Pl_ext
		// HSD mode: INT_TELPROT_MODE_HSD or INT_TELPROT_MODE_HSD_25
		if ((Rx_Command_Ch2_Det != 0) && (IS_HSD600(g_NSD_DSP_Config) || IS_HSD600_25(g_NSD_DSP_Config)))
		{
			g_intNSDTDM_TxStruct0_Ptr->NSD_Events.TPE2_Suppress_Alarms = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->NSD_Events.TPE2_Suppress_Alarms = TRUE;
		}
		
		//
		// *******************************************************************
		// *** Monitoring rx trip duration monitoring ************************
		// *******************************************************************
		// NSD Ch1
		Rx_Command_Ch1_ToG4AI = RxCommandDurationMonit(Rx_Command_Ch1_Det, &g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms, TPE1);
		// NSD Ch2
		Rx_Command_Ch2_ToG4AI = RxCommandDurationMonit(Rx_Command_Ch2_Det, &g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms, TPE2);
		// 
		// *******************************************************************
		// *** Command freeze if no trip and guard      **********************
		// *******************************************************************
		//
		Freeze_CommandOutput(Rx_Command_Ch1_ToG4AI, RxGuardStateCh1, &Rx_Alarm_OR_MaskCh1, &Rx_Alarm_AND_MaskCh1, TPE1);
		Freeze_CommandOutput(Rx_Command_Ch2_ToG4AI, RxGuardStateCh2, &Rx_Alarm_OR_MaskCh2, &Rx_Alarm_AND_MaskCh2, TPE2);
		//
		
		// *******************************************************************
		// *** Send Data to G4AI external TDM-Bus ****************************
		// *******************************************************************

		resetRxAckBitAES = blockCmdOutputs || !(Rx_Alarm_AND_MaskCh1 & BIT0);	// in case of blocked cmd outputs no rx ack is generated

		ackBitsCh1 = setAckBits(Command_Input_Ch1, Rx_Command_Ch1_Det | persCmdRx, aesCmdAckHoldTime, resetRxAckBitAES);
		// @Pl_ext
		ackBitsCh2 = (Command_Input_Ch2 & (~TX_COM_T)) << 9;	// no AES commands on Ch2

		RxGuardStateCh1ToG4AI = rxGuardSignaling_activate(RxGuardStateCh1, TPE1);
		RxGuardStateCh2ToG4AI = rxGuardSignaling_activate(RxGuardStateCh2, TPE2);

		// TANG window
		if (blockCmdOutputs)
		{
			// reset cmd output bits
			Rx_Command_Ch1_ToG4AI &= ~ALL_LP_AES_RX_COMMANDS;
			persCmdRx = 0;

			// set alarm bit
			g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms |= NSD_CMD_OUTPUTS_BLOCKED_ALARM;
		}
		else	// reset alarm bit
		{
			g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms &= ~NSD_CMD_OUTPUTS_BLOCKED_ALARM;
		}

		// Set output signals for G4AI (RxCommands bit 0-3, Unblocking Bit 7, Rx Guard Bit 8, Tx Acknowl. Bit 9 - 15
		// Rx Command without loop test signal, TxAcknowl. signal without loop test signal
		// @Pl_ext
		DataToSendToG4AI[0] = (((Rx_Command_Ch1_ToG4AI & (~RX_COM_T) & Rx_Alarm_AND_MaskCh1) | Rx_Alarm_OR_MaskCh1) | ackBitsCh1 | ((UnblockingBitCh1 & BIT0) << 7) | persCmdRx);
		if(RxGuardStateCh1ToG4AI) DataToSendToG4AI[0] |= (BIT0 << 8);
		//
		// Check if local loop is enabled for Ch1
		if(g_NSDLocalLoopModeCh1 == TPE_LOCAL_LOOP_ON)
		{
			DataToSendToG4AI[0] = Looped_Cmd_Ch1;			// only input commands are loop to G4AI!!!
			// @Pl_ext
			Rx_Command_Ch1_Det &= (~RX_COM_T);				// reset Rx Loop Test -> do not reflect loop test in case of Local loop 
		}
		// Set output signals for G4AI (RxCommands bit 0-3, Unblocking Bit 7, Rx Guard Bit 8, Tx Acknowl. Bit 9 - 15
		// Rx Command without loop test signal, TxAcknowl. signal without loop test signal
		// @Pl_ext
		DataToSendToG4AI[1] = (((Rx_Command_Ch2_ToG4AI & (~RX_COM_T) & Rx_Alarm_AND_MaskCh2) | Rx_Alarm_OR_MaskCh2) | ackBitsCh2 | ((UnblockingBitCh2 & BIT0) << 7));
		if(RxGuardStateCh2ToG4AI) DataToSendToG4AI[1] |= (BIT0 << 8);
		//
		// Check if local loop is enabled for Ch2
		if(g_NSDLocalLoopModeCh2 == TPE_LOCAL_LOOP_ON)
		{
			DataToSendToG4AI[1] = Looped_Cmd_Ch2;			// only input commands are loop to G4AI!!!
			// @Pl_ext
			Rx_Command_Ch2_Det &= (~RX_COM_T);				// reset Rx Loop Test -> do not reflect loop test in case of Local loop 
		}
		//
		// *******************************************************************
		// *** Update lifesign ***********************************************
		// *******************************************************************
		//
		g_intNSDTDM_TxStruct0_Ptr->alarm.lifesign = g_intNSDTDM_TxStruct1_Ptr->alarm.lifesign = myLifesign;
		//
		updateAlarms();
		//
		UpdateNSD_Events();
		//
		// *******************************************************************
		// *** update start,Pilot enable,boost request signals for Master dsp*
		// *******************************************************************
		//
		UpdateBoostManager();
		//
		// *******************************************************************
		// *** Serve internal TDM-bus with data from NSD *********************
		// *******************************************************************
		// serve NSD ch1 data to internal tdm bus
		//
		// send tx command Ch1 to master DSP
			g_intNSDTDM_TxStruct0_Ptr->ch1_input_cmd_status = (Command_Input_Ch1 | StartInput_Ch1);
			g_intNSDTDM_TxStruct1_Ptr->ch1_input_cmd_status = (Command_Input_Ch1 | StartInput_Ch1);
		// send rx commands ch1 to master DSP (must be changed for AES!!)
			g_intNSDTDM_TxStruct0_Ptr->ch1_output_cmd_status = ((Rx_Command_Ch1_Det) | ((RxGuardStateCh1&0x00000001)<<8) | ((UnblockingBitCh1&0x00000001)<<7) | persCmdRx);
			g_intNSDTDM_TxStruct1_Ptr->ch1_output_cmd_status = ((Rx_Command_Ch1_Det) | ((RxGuardStateCh1&0x00000001)<<8) | ((UnblockingBitCh1&0x00000001)<<7) | persCmdRx);
		//
		// serve NSD ch2 data to internal tdm bus
		//
		// send tx command Ch2 to master DSP
			g_intNSDTDM_TxStruct0_Ptr->ch2_input_cmd_status = (Command_Input_Ch2 | StartInput_Ch2);
			g_intNSDTDM_TxStruct1_Ptr->ch2_input_cmd_status = (Command_Input_Ch2 | StartInput_Ch2);
		// send rx commands ch2 to master DSP (must be changed for AES!!)
			g_intNSDTDM_TxStruct0_Ptr->ch2_output_cmd_status = ((Rx_Command_Ch2_Det) | ((RxGuardStateCh2&0x00000001)<<8) | ((UnblockingBitCh2&0x00000001)<<7));
			g_intNSDTDM_TxStruct1_Ptr->ch2_output_cmd_status = ((Rx_Command_Ch2_Det) | ((RxGuardStateCh2&0x00000001)<<8) | ((UnblockingBitCh2&0x00000001)<<7));
		//
		// *******************************************************************
		// *** update Developer Status data for NSD **************************
		// *******************************************************************
		g_Develop_Status_NSD.G4AI_InputCh1 = ReceivedDataFromG4AI[0];
		g_Develop_Status_NSD.G4AI_InputCh2 = ReceivedDataFromG4AI[1];
		g_Develop_Status_NSD.G4AI_OutputCh1 = DataToSendToG4AI[0];
		g_Develop_Status_NSD.G4AI_OutputCh2 = DataToSendToG4AI[1];

		// update developper status alarms
		g_Develop_Status_NSD.DeveloppmentStatusBitsCh1 = g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms;
		g_Develop_Status_NSD.DeveloppmentStatusBitsCh2 = g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms;
		//
		g_Develop_Status_NSD.f_Development_1 = FE_snr_Ch1;
		g_Develop_Status_NSD.f_Development_2 = FE_snr_Ch2;

		// update developer status for pilot enable
		Pilot_enable1 = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.Pilot_Enable_Ch1;
		Pilot_enable2 = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.Pilot_Enable_Ch2;
		Pilot_enable3 = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.Pilot_Enable_Ch3;
		//
		g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[0] = Pilot_enable1|0x00001000;
		g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[1] = Pilot_enable2|0x00001000;
		g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[2] = Pilot_enable3|0x00001000;

		// update developer status for agc blocking
		AGC_Blocking1 = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.AGC_Blocking_Ch1;
		AGC_Blocking2 = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.AGC_Blocking_Ch2;
		AGC_Blocking3 = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.AGC_Blocking_Ch3;
		//
		g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[3] = AGC_Blocking1|0x00001000;
		g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[4] = AGC_Blocking2|0x00001000;
		g_Develop_Status_NSD.G4AI_TDM_Bus_Failure_Counter_Ch1[5] = AGC_Blocking3|0x00001000;
		//
		// ********************************************************************
		// *** NSD Test signal generator **************************************
		// ********************************************************************
		if(ptr_Konfig_NSD_Testsignal_Gen->TestNumber != NO_TEST)
		{
			// Diese Test-Funktion überschreibt die NF-sample 
			Testsignal_Gen(&NSD_Tx_TestSignal_Q[0],&NSD_Tx_TestSignal_I[0]); 
    		//
			// Write af-transmit-signal NSD Ch1 to internal tdm-bus
			g_intNSDTDM_TxStruct0_Ptr->nsd_tx[0].re = NSD_Tx_TestSignal_I[0] * AF_Mask[0];
			g_intNSDTDM_TxStruct0_Ptr->nsd_tx[0].im = NSD_Tx_TestSignal_Q[0] * AF_Mask[0];
			//
			g_intNSDTDM_TxStruct1_Ptr->nsd_tx[0].re = NSD_Tx_TestSignal_I[1] * AF_Mask[0];
			g_intNSDTDM_TxStruct1_Ptr->nsd_tx[0].im = NSD_Tx_TestSignal_Q[1] * AF_Mask[0];
		
			//NSDStatus.NSDAlarmsIn |= WARNING_NSD_TESTSIGNAL;
			g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms |=NSD_TEST_SIGNAL_WARNING;	// set warning
			// overwrite start signal to switch off speech 
			g_intNSDTDM_TxStruct0_Ptr->boostManagerInput.TPE1_Start_or_Rx  = TRUE;
			g_intNSDTDM_TxStruct1_Ptr->boostManagerInput.TPE1_Start_or_Rx  = TRUE;
			// switch off guard in case of testsignal on
			// @Pl_ext
			g_intNSDTDM_TxStruct0_Ptr->ch1_input_cmd_status = TX_COM_T;
			g_intNSDTDM_TxStruct1_Ptr->ch1_input_cmd_status = TX_COM_T;

		}
		else
		{
			//NSDStatus.NSDAlarmsIn &= (~WARNING_NSD_TESTSIGNAL);
			g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms &=(~NSD_TEST_SIGNAL_WARNING); // reset warning
		}
		//
		// ****************************************************************************************
		// *** End RT-part ************************************************************************
		// ****************************************************************************************
		// ****************************************************************************************
		//
		switch(mainCaseCounter)
		{
			// WARNING: don´t change number of cases
			// repetiton rate of case functions = 1.067 kHz

			case MAINCASE_0:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_0;

				// repetiton rate = 1.067 kHz
				// ******************************************************
				// *** Do loop test function for CH1 ********************
				// ******************************************************
				// serve LT-repeater Ch1
				LT_Repeater(&Command_Input_Ch1, &LT_Event_Ch1, Rx_Command_Ch1_Det, TPE1);
				// serve LT Handler for Ch1
				LT_Handler(&Command_Input_Ch1, Rx_Command_Ch1_Det, TPE1);
				//
				// ******************************************************
				// *** Do loop test function for CH2 ********************
				// ******************************************************
				// serve LT-repeater Ch2
				LT_Repeater(&Command_Input_Ch2, &LT_Event_Ch2, Rx_Command_Ch2_Det, TPE2);
				// serve LT Handler for Ch1
				LT_Handler(&Command_Input_Ch2, Rx_Command_Ch2_Det, TPE2);
				//
				//
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_1:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_1;
				asm("bit tgl FLAGS FLG1;");							// for boot testing -> REMOVE
				
				ServeManualLoopTest_byHMI(&g_LoopTestMode, &g_NSDLoopTestDelay);
				// Reset Start Signal after 1 second 
				// Alarm monitoring of the Start input will be done of G4AI
				StartSetMax1Sec();
				
				// Check if NSD Testsignal generator is active
				if(ptr_Konfig_NSD_Testsignal_Gen->TestNumber != NO_TEST) NSD600_Testsignal(); // NSD test signal
				//

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_2:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_2;

				SNR_Monitoring_Guard(Rx_Command_Ch1_Det, FE_snr_Ch1, &g_NSD_DSP_Status.NSD_Status[0], TPE1);
				SNR_Monitoring_Guard(Rx_Command_Ch2_Det, FE_snr_Ch2, &g_NSD_DSP_Status.NSD_Status[1], TPE2);

				persCmdRx = AES_persCmd_detect(Rx_Command_Ch1_Det);

				mainCaseCounter++;                                                         
				break;

			case MAINCASE_3:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_3;
				
				UnblockingBitCh1 = NSDUnblocking(TPE1);
				UnblockingBitCh2 = NSDUnblocking(TPE2);

				Update_Dev_Status_Det(&g_Develop_Status_NSD);
				Update_Dev_Status_Guard(&g_Develop_Status_NSD);
				Update_Dev_Status_LT(&g_Develop_Status_NSD);

				// TANG window for Ch1 (used only in AES mode)
				blockCmdOutputs = tangWindow_activate(RxGuardStateCh1, Rx_Command_Ch1_Det);

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_4:
			
				switch(subCaseCounter4) {

					// WARNING: don´t change number of cases
					// repetiton rate of case functions = 0.1067 kHz
					
					case SUBCASE_0:	
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_0;
						
						CounterI0++;
							if (CounterI0 == 10)
							{
								//
								// do 100ms case 
								//
								// ****************************************
								// *** Reset LT-events after 100ms  *******
								// ****************************************
								// Comment: The event recorder on the master dsp write the manipulation events every 100ms! 
								//          The events must be active during a minimum time of 100ms to be saved.  
								// 
								if(Last_Event_Ch1 == LT_Event_Ch1) LT_Event_Ch1 = 0;
								if(Last_Event_Ch2 == LT_Event_Ch2) LT_Event_Ch2 = 0;
								//
								//
								Determine_LT_Cases(Command_Input_Ch1, Rx_Command_Ch1_Det, &g_NSD_DSP_Status.NSD_Status[0], &LT_Event_Ch1, TPE1);
								//
								Determine_LT_Cases(Command_Input_Ch2, Rx_Command_Ch2_Det, &g_NSD_DSP_Status.NSD_Status[1], &LT_Event_Ch2, TPE2);
								//
								CounterI0 = 0;
								
								// update LT-events
								Last_Event_Ch1 = LT_Event_Ch1;
								Last_Event_Ch2 = LT_Event_Ch2;
							}
							else
							{

							}
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_1:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_1;

						//
						// *** Amplitude correction of NSD transmitter *********
						// 
						AmpCorrTx(TPE1);
						AmpCorrTx(TPE2);

						//
						// *** Amplitude correction of NSD Rx-IF Signal generator
						//
						AmpCorr(ptr_RxIF_Sin_Ch1, ptr_RxIF_Cos_Ch1, 1);
						AmpCorr(ptr_RxIF_Sin_Ch2, ptr_RxIF_Cos_Ch2, 1);

						//
						// *** Amplitude correction of NSD Detectors Signal generator sin and cos path of all detector frequencies
						//
						AmpCorr(ptr_Det_Sin_Ch1, ptr_Det_Cos_Ch1, NUMBER_OF_RX_DET_TONES);
						AmpCorr(ptr_Det_Sin_Ch2, ptr_Det_Cos_Ch2, NUMBER_OF_RX_DET_TONES);

						//
						// *** Amplitude correction of NSD Rx Guard IF singal generator Ch1 and Ch2
						//
						RxGuardIFAmplCorr();

						//
						// *** RxGuard level Monitoring ********************
						//
						RxGuardLevelMonitoring(&g_NSD_DSP_Status.NSD_Status[0], TPE1);
						RxGuardLevelMonitoring(&g_NSD_DSP_Status.NSD_Status[1], TPE2);

						// for tests only
						GuardLevelAlarmCh1 = (g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms)&(NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM);
						GuardLevelAlarmCh2 = (g_NSD_DSP_Status.NSD_Status[1].NSD_Alarms)&(NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM);

						subCaseCounter4++;
						break;

					case SUBCASE_2:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_2;
						//
						Delay_Hold_RxAlarm(TPE1);
						Delay_Hold_RxAlarm(TPE2);
						//
						Set_Output_At_RxAlarm(&Rx_Alarm_OR_MaskCh1, &Rx_Alarm_AND_MaskCh1, TPE1);
						Set_Output_At_RxAlarm(&Rx_Alarm_OR_MaskCh2, &Rx_Alarm_AND_MaskCh2, TPE2);
						//
						subCaseCounter4++;
						break;
						
					case SUBCASE_3:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_3;
						
						DoMemoryCheck(TRUE);
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_4:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_4;
						


						subCaseCounter4++;
						break;

					case SUBCASE_5:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_5;
						
						
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_6:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_6;
						
						

						subCaseCounter4++;
						break;
			
					case SUBCASE_7:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_7;
						
						if (g_niReceiveFlag)
						{
							g_niReceiveFlag = FALSE;

							switch (g_niRxMsgType)
							{	
								e_TDMServiceResult tdmServiceResult;

								case e_TDMServiceMsgType_IdleCounter_Req:

									MEMCPY (&g_idleCountersBuf, idleCounter_read(), sizeof(g_idleCountersBuf));
									idleCounter_reset(); // reset idle counters

									tdmServiceResult =  
										tdmServiceNI_Reply ((unsigned long *)&g_idleCountersBuf, 
															sizeof(g_idleCountersBuf),
    														e_TDMServiceMsgType_IdleCounter_Resp); //txMsgType,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;

								case e_TDMServiceMsgType_StatusData_Req:

									//NSD status Ch1 is also used in case of AES mode
									g_NSD_DSP_Status.AES_Status.ActualTransmissionTime	= g_NSD_DSP_Status.NSD_Status[0].ActualTransmissionTime;
									g_NSD_DSP_Status.AES_Status.ActualGuardLevel		= g_NSD_DSP_Status.NSD_Status[0].ActualGuardLevel;
									g_NSD_DSP_Status.AES_Status.ActualSNR				= g_NSD_DSP_Status.NSD_Status[0].ActualSNR;
									g_NSD_DSP_Status.AES_Status.ActualSNRAlarmThreshold = g_NSD_DSP_Status.NSD_Status[0].ActualSNRAlarmThreshold;
									g_NSD_DSP_Status.AES_Status.AES_Alarms				= g_NSD_DSP_Status.NSD_Status[0].NSD_Alarms;

									tdmServiceResult = 
										tdmServiceNI_Reply((unsigned long *)&g_NSD_DSP_Status,
															sizeof(g_NSD_DSP_Status),
															e_TDMServiceMsgType_NSD_StatusData_Resp); //tx MsgType

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_NSD_Read_Developer_Status_Req:
									tdmServiceResult = 
										tdmServiceNI_Reply((unsigned long *)&g_Develop_Status_NSD,
															sizeof(g_Develop_Status_NSD),
															e_TDMServiceMsgType_NSD_Read_Developer_Status_Resp); //tx MsgType

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_NSD_Read_TestsignalConfig_Req:
									tdmServiceResult = 
										tdmServiceNI_Reply((unsigned long *)ptr_Konfig_NSD_Testsignal_Gen,
															sizeof(NSDTestDataConfig),
															e_TDMServiceMsgType_NSD_Read_TestsignalConfig_Resp); //tx MsgType

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_NSD_Write_TestsignalConfig_Req: 

									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    																	   e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									MEMCPY(ptr_Konfig_NSD_Testsignal_Gen, g_niRxData, sizeof(NSDTestDataConfig));

									break;

								case e_TDMServiceMsgType_NSDLooptest_Trigger_Req: 
								
									g_LoopTestMode = g_niRxData[0];


									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    																	   e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;

								case e_TDMServiceMsgType_NSDLooptest_GetResult_Req: {

									
									
									//g_NSDLoopTestDelay = 700;

									
									tdmServiceResult =  tdmServiceNI_Reply (&g_NSDLoopTestDelay, 
																			sizeof(g_NSDLoopTestDelay),
    																		e_TDMServiceMsgType_NSDLooptest_GetResult_Res); //txMsgType,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;
								}
								case e_TDMServiceMsgType_Cfg_Send:	// this case should not occur 

									// send TDM service reply
									tdmServiceResult = 
											tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    															e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;

								case e_TDMServiceMsgType_NSD_SetLocalLoop_Req: 
								
									g_NSDLocalLoopMode = g_niRxData[0];	// set/reset NSD local loop 
									
									NSD_Mode_Changed = TRUE;

									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    																	   e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;

								default:
									// send TDM service reply "no acknowledge"
									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    															           e_TDMServiceMsgType_Nack);// txMsgType = no acknowledge

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);
							}
						}

						subCaseCounter4++;
						break;
						
					case SUBCASE_8:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_8;
					
						myLifesign++;

						CheckLocalLoopMode();
					
						subCaseCounter4++;
						break;
					
					case SUBCASE_9:
					{
						static BOOL bFlag = FALSE;


						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_9;

						if (!bFlag)
						{
							idleCounter_reset();
							bFlag = TRUE;
						}
											
						subCaseCounter4 = 0;
						break;
					}	
					default:
						subCaseCounter4 = 0;
						break;
				}; // end switch(subCaseCounter4)
					
				mainCaseCounter = 0;
				break;
    
			default:
				mainCaseCounter = 0;
				break;
		}; // end switch(mainCaseCounter)

    }

}

