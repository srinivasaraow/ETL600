/*-------------------------------------------------------------------------*
 * $Workfile: msgHandler.c $
 * Part of	: ETl600
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: P4LT spezifische Funktionen für die Ausführung der Telegramme
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/Master_DSP/C/msgHandler.c $
 * 
 * 72    11-12-08 13:52 Chhamaa
 * ETL600 equipment type added to the alarmWords (used for alarm polling).
 * 
 * 71    11-09-07 18:23 Chhamaa
 * msgHandler_getOperationMode() added.
 * 
 * 70    11-02-10 9:51 Chhamaa
 * New message added: MSG_MUX_SET_VOICE_TRANSIT_TEST_CFG
 * 
 * 69    10-10-29 14:23 Chhamaa
 * 
 * 68    25.06.10 14:30 Chhamaa
 * New message for O4CV developer status upolaod
 * (MSG_MUX_GET_VOICE_TRANSIT_STATUS) added.
 * 
 * 65    08-02-25 10:56 Chtognu
 * Merged changes from ad.equ.branch:
 * ad.Eq. Logger communication added
 * 
 * 64    23.07.07 16:53 Chtognu
 * memcopy of NSK adaptive Equalizer structs added
 * 
 * 63    28.02.07 15:44 Chhamaa
 * t_MUX_O4CV_Status added to P4LT status upload.
 * 
 * 61    5.10.06 16:39 Chchlee
 * Issue #386 "Sometimes an R1LA is detected on a P4LT" fixed
 * 
 * 60    19.09.06 16:39 Chchlee
 * R1LA detection with P4LT corrected
 * 
 * 59    25.08.06 18:56 Chchlee
 * Function R1LAIsPlugged added
 * 
 * 56    29.03.06 11:25 Chmibuh
 * replaced resetP4LT() with resetP4LTandR1LA()
 * 
 * 55    17.03.06 10:48 Chmibuh
 * g_NSD_Developer_Status.l_Development_1 = TestcountDSP;
 * 
 * 54    27.02.06 14:14 Chmibuh
 * new message CONVERT_RTC_DATA_FOR_READ added. For LAN600 without write
 * access
 * 
 * 52    19.01.06 16:49 Chmibuh
 * extern const long CRC_Checksum_over_all_code;
 * 
 * 51    22.11.05 20:04 Chmibuh
 * PIGGYBACK_HW_ID_NO_PIGGYBACK << 3 added. It's in part for Emergency FW
 * only
 * 
 * 50    5.10.05 12:02 Chchlee
 * No RF tx reactivate after FW-DL anymore. 
 * Emergency FW version removed
 * 
 * 49    4.10.05 14:59 Chchlee
 * END_OF_PROG_MESSAGE_P4LQ: record event before disabling event recorder
 * 
 * 48    3.10.05 18:39 Chmibuh
 * use of FW_VERSION_NAME_EMERGENCY added
 * 
 * 47    28.09.05 14:46 Chchlee
 * isInDebugMode() moved to IO_BUFFER.H
 * 
 * 46    28.09.05 8:11 Chchlee
 * 
 * 45    22.09.05 10:02 Chchlee
 * READ_EQUIPMENT_ID --> READ_EQUIPMENT_INFO
 * 
 * 44    22.09.05 9:42 Chchlee
 * EquipmentConfigInfo added
 * 
 * 43    10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 42    29.07.05 14:38 Chmibuh
 * 
 * 41    22.07.05 8:16 Chchlee
 * monitor_enableReset renamed
 * 
 * 40    21.07.05 12:01 Chmibuh
 * 
 * 39    21.07.05 10:57 Chchlee
 * debug flag moved to io:hw_buffer.h
 * 
 * 38    20.07.05 15:19 Chchlee
 * EquipmentSWID replaced by t_EquipmentSWID
 * 
 * 37    19.07.05 17:13 Chchlee
 * GET_EQUALIZER_STATUS now more robust
 * 
 * 36    19.07.05 16:55 Chchlee
 * Comments added
 * 
 * 35    16.07.05 18:44 Chmibuh
 * new massage MSG_ENABLE_EMERGENCY_FW_UPDATE
 * 
 * 34    13.07.05 8:14 Chchlee
 * BOARD_HW_VERSION_MASK / BOARD_HW_ID_MASK modified
 * 
 * 33    30.06.05 8:14 Chchlee
 * cleaned up
 * 
 * 32    28.06.05 13:53 Chchlee
 * ASSERTs consolidated
 * 
 * 31    13.06.05 7:33 Chchlee
 * additional manipulation events added
 * 
 * 30    6.06.05 15:20 Chmibuh
 * g_equipmentSWIdentification.HwVersion = ... added
 * 
 * 29    3.06.05 10:33 Chmibuh
 * iButH_getLicenceSerieNum() added
 * 
 * 28    27.05.05 9:07 Chchlee
 * write access enabled
 * 
 * 27    24.05.05 17:57 Chchlee
 * some P4LW message IDs modified
 * 
 * 26    24.05.05 7:24 Chchlee
 * 
 * 25    20.05.05 15:36 Chmibuh
 * DOWNLOAD_P4LQ_PASSWORD adapt for Emergency FW
 * 
 * 24    19.05.05 20:14 Chchlee
 * debug flag considered in fw download
 * 
 * 23    18.05.05 17:20 Chchlee
 * 
 * 22    18.05.05 12:06 Chchlee
 * cleaned up EMERGENCY_FW switch
 * 
 * 21    18.05.05 10:19 Chchlee
 * RF signal now turned off during firmware download
 * 
 * 20    10.05.05 12:47 Chmibuh
 * MAX_OFDM_SYMBOLDURATION_IN_TICKS  2170	// 2170 * 1/10.66kHz = 203ms
 * 
 * 19    12.04.05 20:15 Chmibuh
 * #ifndef EMERGENCY_FW added again
 * 
 * 18    12.04.05 17:39 Chpeplu
 * compile failure corrected -> only lines removed not really corrected!
 * 
 * 17    10.04.05 13:09 Chmibuh
 * 
 * 16    9.04.05 18:23 Chmibuh
 * g_equipmentSWIdentification.LicenseCode = iButH_getLicenceCode(); added
 * 
 * 15    29.03.05 20:41 Chmibuh
 * UPLOAD_RTC_DATA event bug removed (bug was at DOWNLOAD_RTC_DATA)
 * 
 * 14    29.03.05 16:38 Chhamaa
 * 
 * 13    25.02.05 18:18 Chhamaa
 * Msg COMMISSIONING_CFG_SEND modified
 * 
 * 12    24.02.05 14:53 Chhamaa
 * Msg COMMISSIONING_CFG_SEND modified
 * 
 * 11    23.02.05 15:37 Chhamaa
 * Bug fixed
 * 
 * 10    23.02.05 14:49 Chhamaa
 * CHANGE_BETRIEBSKONFIGURATION_P4LQ renamed with SET_OPERATION_MODE
 * 
 * 9     21.02.05 19:41 Chchlee
 * temporary configuration warning cleaned up
 * 
 * 8     18.02.05 19:09 Chhamaa
 * Msgs CHANGE_BETRIEBSKONFIGURATION_P4LQ and COMMISSIONING_CFG_SEND
 * modified
 * 
 * 7     17.02.05 18:40 Chhamaa
 * Msgs CHANGE_BETRIEBSKONFIGURATION_P4LQ and COMMISSIONING_CFG_SEND
 * implemented
 * 
 *-------------------------------------------------------------------------*/

// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION
//
// If you add something, either if you write new code or comment-in previously commented-out 
// code fragments than bracket it with 
// #ifndef EMERGENCY_FW
//		"your code"
// #endif
//
// This prevents the emergency FW from needless code. The size of emergency FW is limited!!!
// This belongs to msgHandler.c, bridge.c, SPI_functions.c and Init_Master_DSP only.
//
// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION

/* INCLUDE FILES **************************************************************/

#include <stddef.h> // using offsetof()
#include <stdlib.h> // using __min()

#include "msg_P4LQ.h"
#include "msgHandler.h"

#include "tdm_service.h"
#include "diag.h"

#include "default.h"   
#include "constant.h"

#include "P4LT_Types.h"
#include "tdm_service.h"

#include "idlecounter.h"
#include "configuration.h"
#include "status.h"

#include "epromhi.h" 
#include "eprommed.h" // used for eprom_emptyStack()
#include "epromlow.h" // used for SECTORs

#include "io_hw_buffer.h"

#include "eventRec.h" 

#include "rtc.h"

#include "Equalizer.h"

#include "sampling.h"

#include "commissioning.h"

#include "tdmUpConvSettings.h"

#include "ChannelSim_Types.h" // RF channel simulator

#include "NSD_TEST.H"

#include "iButHigh.h"
#include "licenceCheck.h"

#include "SPI_functions.h"

#include "Piggyback.h"

#include "NSK_DSP_Types.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define MAX_WRITEACCESS_TIMER     7200000	    // WriteAccess wird nach ca. 20 h Inaktivität entzogen 

// maximum OFDM symbol duration in TDM Service ticks
// maximum OFDM symbol duration = 144ms (N=512, L=64, BW = 4kHz)
//#define MAX_OFDM_SYMBOLDURATION_IN_TICKS  (1E5 * 144 / DSP_CYCLES_PER_MAINLOOP)				
////// Test with about 200ms
#define MAX_OFDM_SYMBOLDURATION_IN_TICKS  2170	// 2170 * 1/10.66kHz = 203ms			

// additional states for deferred message handler
#define STATE_SEND_LOOPTEST                         (MAX_MESSAGE_ID + 0)
#define UPLOAD_P4LT_STATUS_DATA_W4_OFDM_STATUS      (MAX_MESSAGE_ID + 1)
#define UPLOAD_P4LT_STATUS_DATA_W4_NSK_STATUS       (MAX_MESSAGE_ID + 2)
#define UPLOAD_P4LT_STATUS_DATA_W4_NSD_STATUS       (MAX_MESSAGE_ID + 3)
#define UPLOAD_P4LT_STATUS_DATA_W4_FE_STATUS        (MAX_MESSAGE_ID + 4)
#define STATE_CLEAR_EVENT_RECORDER                  (MAX_MESSAGE_ID + 5)

#define SAMPLE_BUFFER_SIZE		32			// sampled data are read in blocks of 32

// must be external; it's an inmported const from Interrupt_table_Master_DSP.asm
extern const long CRC_Checksum_over_all_code;


/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/



/* LOCAL VARIABLES ***********************************************************/

static t_P4LT_Config * g_pCurrentCfg;
static t_P4LT_Config * g_pNewCfg;

static t_EpromLocalConfiguration * g_p_EpromLocalConfiguration;

static t_P4LT_Status  * g_p_StatusData;

static t_EquipmentSWID  g_equipmentSWIdentification;

static unsigned long	g_WriteAccessTimer;
static BOOL				g_WriteAccessAvailable; 

static unsigned long    g_messageTypeDelayedCommand;

static t_IdleCounters                            g_idleCounters;

static e_ExecMsgStatus                           g_delayedMessageTypesStatus;

static                  t_OFDM_ShortStatus       g_OFDMShortStatus;
static SEGMENT_EXT_DATA t_OFDM_LongStatus        g_OFDMLongStatusRes;

static                  unsigned long	   	     g_NSKModemNumber;
static SEGMENT_EXT_DATA t_NSK_ShortStatus        g_NSK_ShortStatus;
static SEGMENT_EXT_DATA t_NSK_AdapEquTF          g_NSK_AdapEquTF;

static SEGMENT_EXT_DATA t_MUX_VoiceTransitDevStatus	g_MUX_VoiceTransitDevStatus;
static SEGMENT_EXT_DATA t_MUX_VoiceTransitTestCfg	g_MUX_VoiceTransitTestCfg;

static t_TimeDate                               *m_timeDatePtr;

static unsigned long g_NSDLoopTestDelay;
static unsigned long g_LoopTestMode;

static unsigned long g_NSDLocalLoopMode;

static t_NSD_DevelopperStatus_Type g_NSD_Developer_Status;

static NSDTestDataConfig g_NSD_TestSignalConfig;

static t_SamplingBufRdParam	g_samplingBufRdParam;
static float g_sampleBuffer[SAMPLE_BUFFER_SIZE];

static t_ChannelSimGetSetData g_ChannelSimGetSetParameter;
static unsigned long          g_ChannelSimGetParameter;

static t_TestPlcLinkData    * g_pTestPlcLinkCfg;

static t_Commissioning g_commissioningCfg;
static float g_commissionningRxBuffer;

static unsigned long g_operationMode = OP_MODE_NORMAL;

static 	BOOL                (*g_pReceiveBuffersEmpty)(void);
static  BOOL                 g_tdmServiceConfirmCalled;
static e_TDMServiceResult    g_TDMServiceResult;
static e_TDMServiceMsgType   g_rxMessageType;
static unsigned long         g_rxMessageSize;

static long                  blockSizeInLongs;		  // used in many CASEs

static unsigned long		 *g_firmware_startAddress_Ptr;

/* LOCAL FUNCTIONS ***********************************************************/

static void iConfirm (e_TDMServiceResult  result, 
					  e_TDMServiceMsgType messageType,
					  unsigned long       rxMsgSize);


/* IMPLEMENTATION ************************************************************/

static void iConfirm (e_TDMServiceResult  result, 
					  e_TDMServiceMsgType messageType,
					  unsigned long       rxMsgSize)
{
	g_TDMServiceResult = result;
	g_rxMessageType    = messageType;
	g_rxMessageSize    = rxMsgSize;

	g_tdmServiceConfirmCalled = TRUE;
}


//***************************************************************************************
//	void msgHandler_init(BOOL (* receiveBuffersEmpty)(void))
//
//	- initialisiert die globalen Variablen für die Frame Generierung 
//  - kopiert die FW_version in g_equipmentSWIdentification Struktur 
//	- initialisiert die generischen Funktionen
//		void ApplicationLayer()
//		void makeFrameHeader()
//****************************************************************************************

void SEGMENT_EXT_CODE msgHandler_init(BOOL (* receiveBuffersEmpty)(void))
{
    long *tempLongPtr = (long*)&g_equipmentSWIdentification.SWIdentification;
	int i;
	
	const char Version[sizeof(FW_VERSION_NAME)] = FW_VERSION_NAME;   // Groesse des Arrays = Anzahl Character + \0 (end char)	
	// make sure the version string fits into SWIdentification array
	ASSERT(sizeof(FW_VERSION_NAME) <= NR_OF_BYTES_IN_LONG * SW_ID_LENGTH);

	g_pCurrentCfg = configuration_getPCurrentConfig();

    g_equipmentSWIdentification.MMICompatibilityVersion = MMI_COMPATIBILITY_VERSION_OF_P4LT;				
	
	// PIGGYBACK_HW_VERSION_MASK information
	g_equipmentSWIdentification.HwVersion = IO_BUFFER_READ & HW_BIT_MASK; // Add HW-VERSION


#ifdef EMERGENCY_FW

	// overwrite equipmentInfo
	g_pCurrentCfg->system.equipmentInfo.address = 0;
	g_pCurrentCfg->system.equipmentInfo.configurationCounter = 3;
	g_pCurrentCfg->system.equipmentInfo.identification[0] = 0x52454D45;				//REME   EMERGENCY FW
	g_pCurrentCfg->system.equipmentInfo.identification[0] = 0x434E4547;				//CNEG
	g_pCurrentCfg->system.equipmentInfo.identification[0] = 0x57462059;				//WF_Y

	// set full licence if Emergency FW
	g_equipmentSWIdentification.LicenseCode = LICENCE_DEF_1MOD600_4NSK600;	
	g_equipmentSWIdentification.EquipmentStateInfo = EQUIPMENT_STATE_EMERGENCY_FW;

	// NO PIGGYBACK_HW_VERSION_MASK INFORMATION available. Pretend there is no piggyback
	g_equipmentSWIdentification.HwVersion |= ((PIGGYBACK_HW_ID_NO_PIGGYBACK << 3) & PIGGYBACK_HW_ID_MASK);

#else

	tdmServiceI_Init(iConfirm);

	g_pNewCfg                  = configuration_getPNewConfig();
	g_p_EpromLocalConfiguration = configuration_getPEpromLocalConfiguration();
 	g_p_StatusData              = status_getPStatus();
	g_pTestPlcLinkCfg           = status_getPTestSwitches();

	m_timeDatePtr				= rtc_getTimeDatePtr();

	g_equipmentSWIdentification.LicenseCode = iButH_getLicenceCode();
	g_equipmentSWIdentification.EquipmentStateInfo = EQUIPMENT_STATE_OK;
	
	///// Add Piggyback HW_VERSION
	g_equipmentSWIdentification.HwVersion |= getPiggyBackInfo();

	iButH_getLicenceSerieNum(g_p_StatusData);


	g_equipmentSWIdentification.CRC_over_all_code |= (CRC_Checksum_over_all_code & 0x0000FFFF);


	// set EQUIPMENT_CFG_INFO_HAS_DPLC bit if MOD600 configured
	if (g_pCurrentCfg->OFDM.channelBW > 0)
	{
		g_equipmentSWIdentification.EquipmentConfigInfo |=	EQUIPMENT_CFG_INFO_HAS_DPLC;
	}

	{  // set EQUIPMENT_CFG_N_APLC_CH0/1: number of configured APLC channels
		int nAPLCChannels;
		CNT_APLC_CHANNELS(g_pCurrentCfg->APLC, nAPLCChannels);
		g_equipmentSWIdentification.EquipmentConfigInfo |= nAPLCChannels;
	}

#endif

	// build software identification structure: pack array to long
	for(i=0; i < sizeof(FW_VERSION_NAME); i+=NR_OF_BYTES_IN_LONG) 
	{
		*tempLongPtr++ = Version[i] | Version[i+1]<<SB8 | Version[i+2]<<SB16 | Version[i+3]<<SB24;
	}	

	g_pReceiveBuffersEmpty = receiveBuffersEmpty;  // remember function pointer

	g_firmware_startAddress_Ptr = eprom_get_firmware_startAddress_Ptr();
}	

//***************************************************************************************
// A setter function. Is called to set the EQUIPMENT_CFG_R1LA_PLUGGED bit in  
// EquipmentConfigInfo. The bit is suppressed if the is no P1LX (= piggyback) present.
// Preconditions: msgHandler_init() called.
//***************************************************************************************

void SEGMENT_EXT_CODE R1LAIsPlugged(void)
{

#ifndef EMERGENCY_FW

	// ignore if no P1LX reported
	if ((g_equipmentSWIdentification.HwVersion & PIGGYBACK_HW_ID_MASK) != (PIGGYBACK_HW_ID_NO_PIGGYBACK << 3))
	{
		g_equipmentSWIdentification.EquipmentConfigInfo |= EQUIPMENT_CFG_R1LA_PLUGGED;
	}

#endif
}

//***************************************************************************************
// void EquipmentFirmwareIdentification() 
//
// - generiert das Antworttelegramm für den Functioncode RXFUNCTION_RESPOND_EQ_ID:
//***************************************************************************************

void msgHandler_equipmentFirmwareIdentification(void) 
{	
	makeFrameHeader(&g_equipmentSWIdentification, sizeof(g_equipmentSWIdentification));
}


//***************************************************************************************
// BOOL msgHandler_equipmentIsNotInNormalOpMode() 
//
// Checks if equipment is not in normal operation mode.
//***************************************************************************************

BOOL msgHandler_equipmentIsNotInNormalOpMode(void)
{
	return(g_operationMode != OP_MODE_NORMAL);
}


//***************************************************************************************
// unsigned long msgHandler_getOperationMode()
//
// Returns the current operation mode.
//***************************************************************************************

unsigned long msgHandler_getOperationMode(void)
{
	return(g_operationMode);
}


//***************************************************************************************
// void executeMessage(ETL500TelegramFrameBufferType *Frame)
//
// - führt die Telegramme, welche in message.h für den P4LQ definiert sind aus.
// 
//***************************************************************************************

e_ExecMsgStatus SEGMENT_EXT_CODE msgHandler_executeMessage(t_FT12TelegramBuffer *frame)
{

	register long *pRxUD; // pointer to received user data
    register long *TempPtr1,*EndPtr;
	
	register long NrOfBytesToWrite, DestinationAddress;
	long TempStorage;
	long numberOfReceivedLongValues;

	long addressOffset;           // used in many CASEs

   	static long EraseMessageACK  = 'O';        
   	static long EraseMessageNACK = 'F';

	e_ExecMsgStatus result = e_ExecMsgStatus_Ok;
	
  	pRxUD = ((long *)frame) + offsetof(t_FT12TelegramBuffer, TelegrammDataArr) + 4; // Pointer To User Data (5. Word)
	numberOfReceivedLongValues =  FT12_SIZEOF(frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG;	

	g_WriteAccessTimer = MAX_WRITEACCESS_TIMER; // inactivity timer wird bei jeder msg neu initialisiert

	switch(frame->MessageType)
    {
		// zuerst die Lesebefehle:

		case READ_EQUIPMENT_INFO:       
	     	makeFrameHeader(&g_pCurrentCfg->system.equipmentInfo, sizeof(g_pCurrentCfg->system.equipmentInfo));
			break;

		case GET_EPROM_ACTUAL_STATUS_P4LQ:	
		{
			static long epromStatus; // must be static, because this variable must live longer than this function!

			epromStatus	= eprom_commandsInStack() ? TRUE: FALSE;

	     	makeFrameHeader( &epromStatus, sizeof(epromStatus));
			break;                        
		}

#ifndef EMERGENCY_FW

		case UPLOAD_P4LQ_CONFIGURATION_DATA:
			blockSizeInLongs      = *pRxUD++;
			addressOffset         = *pRxUD;
	     	makeFrameHeader( (long*)g_pCurrentCfg + addressOffset, blockSizeInLongs);
			break;

        case UPLOAD_P4LQ_STATUS_DATA:   
		{
			blockSizeInLongs      = *pRxUD++;
			addressOffset         = *pRxUD++;

			if (0 == addressOffset) // reading first segement -> fetch data from OFDM DSP
			{
				// remote call
				g_tdmServiceConfirmCalled = FALSE; // reset flag
				g_messageTypeDelayedCommand = UPLOAD_P4LT_STATUS_DATA_W4_FE_STATUS;
					 
				// get APLC status from frontend
				// send no data, receive data
				tdmServiceI_Request( e_TDMServiceAddr_FE,                             // destination address
									 e_TDMServiceMsgType_StatusData_Req,              // txMsgType,
									 NULL, 0,
									 (unsigned long *)&g_p_StatusData->APLC, 
									 sizeof(g_p_StatusData->APLC),                                         
									 TDM_SERVICE_TIMEOUT);

				result = e_ExecMsgStatus_Deferred;
				g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
			}
	     	else
			{
				makeFrameHeader( (long*)g_p_StatusData + addressOffset, blockSizeInLongs);
			}

			break;                                                
		}
			
		case SAMPLE_DATA_MESSAGE_P4LQ:
		{
			static t_SamplingCfg samplingCfg;

			
			// get sampling config
			samplingCfg.signalSource = *pRxUD++;
			samplingCfg.signalPath	= *pRxUD;

			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				
			// send samplingCfg data
			tdmServiceI_Request( e_TDMServiceAddr_FE,					// destination address
								 e_TDMServiceMsgType_SamplingCfg_Send,	// txMsgType,
								 (unsigned long *)&samplingCfg,			// tx message
								 sizeof(samplingCfg),					// tx message size
								 NULL, 0,								// receive buffer (no reply expected)
								 TDM_SERVICE_TIMEOUT); 

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
			break;
		}

		case READ_FLOAT_ARRAY_MESSAGE: 

			g_samplingBufRdParam.nSamples = *pRxUD++;
			g_samplingBufRdParam.startIndex = *pRxUD;

			// check number of requested samples
			if (g_samplingBufRdParam.nSamples <= SAMPLE_BUFFER_SIZE) {
	     	
				// remote call
				g_tdmServiceConfirmCalled = FALSE; // reset flag
				g_messageTypeDelayedCommand = frame->MessageType;
					
				// send samplingCfg data
				tdmServiceI_Request( e_TDMServiceAddr_FE,						// destination address
									 e_TDMServiceMsgType_SignalBuffer_Req,		// txMsgType,
									 (unsigned long *)&g_samplingBufRdParam,	// tx message
									 sizeof(g_samplingBufRdParam),				// tx message size
									 (unsigned long *)g_sampleBuffer,			// receive buffer
									  sizeof(g_sampleBuffer),					// receive buffer size
									 TDM_SERVICE_TIMEOUT); 

				result = e_ExecMsgStatus_Deferred;
				g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
			}
			else {	// requested number of samples > sample buffer
				
				g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
				makeFrameHeader( NULL, 0);
			};
			break;

		//	schnelles Alarmmonitoring
		case READ_ALARM_WORDS:
		{
			static t_AlarmWords alarmWords = { {0,0,0,0,0,0},0,0, 
				                                BIT28                // signals that this is a P4LT being polled
											 };


			switch(g_pCurrentCfg->system.equipmentType)
			{
				case EQ_TYPE_ETL600_R4_4Slots:
					
					alarmWords.dummy3 |= (EQ_TYPE_ETL600_R4_4Slots << ALARM_POLLING_ETL600_EQ_TYPE_SHIFT);
					break;

				case EQ_TYPE_ETL600_R4_7Slots:
					
					alarmWords.dummy3 |= (EQ_TYPE_ETL600_R4_7Slots << ALARM_POLLING_ETL600_EQ_TYPE_SHIFT);
					break;

				default:
					
					alarmWords.dummy3 |= (EQ_TYPE_ETL600_R3 << ALARM_POLLING_ETL600_EQ_TYPE_SHIFT);
			}

			MEMCPY(&alarmWords.alarmsAndWarnings,
			       &g_p_StatusData->system.alarmsAndWarnings,
				   sizeof(alarmWords.alarmsAndWarnings));

			alarmWords.alarmsFromTDMBus = g_p_StatusData->system.alarmsFromTDMBus;

			makeFrameHeader(&alarmWords, sizeof(alarmWords));
		}
			break;

		case READ_IDLE_COUNTERS:    
		{
			if (*pRxUD == e_TDMServiceAddr_Master)  // *pRxUD contains DSP address
			{   // local call
				MEMCPY (&g_idleCounters, idleCounter_read(), sizeof(g_idleCounters));
				idleCounter_reset(); // reset idle counters

	     		makeFrameHeader(&g_idleCounters, sizeof(g_idleCounters));
			}
			else
			{   
				if (*pRxUD <= e_TDMServiceAddr_Last) // check validity of DSP Id
				{
					// remote call
					g_tdmServiceConfirmCalled = FALSE; // reset flag
					g_messageTypeDelayedCommand = frame->MessageType;
						
					// send no data, receive data
					tdmServiceI_Request( (e_TDMServiceAddr)*pRxUD, // destination address
										 e_TDMServiceMsgType_IdleCounter_Req, // txMsgType,
										 NULL, 0, // no tx message
										 (unsigned long *) &g_idleCounters, // receive buffer
										 sizeof(g_idleCounters), // size of receive buffer
										 TDM_SERVICE_TIMEOUT); 

					result = e_ExecMsgStatus_Deferred;
					g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
				}
				else // invalid DSP Id
				{
					result = e_ExecMsgStatus_DataNotAvailable; // NACK: Requested data not available
					makeFrameHeader( NULL, 0); 
				}
			}
			break;                                                
		}

		case UPLOAD_P4LQ_TEST_SWITCHES:    
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send no data, receive data
			tdmServiceI_Request( e_TDMServiceAddr_FE,                                      // destination address
								 e_TDMServiceMsgType_GetTestPlcLinkCfg_Req,                // txMsgType,
								 NULL, 0,                                                  // no tx message
								 (unsigned long *) g_pTestPlcLinkCfg,                      // receive buffer
								 sizeof(*g_pTestPlcLinkCfg),                               // size of receive buffer
								 TDM_SERVICE_TIMEOUT);

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case MSG_NSK_GET_SHORT_STATUS:    
		{
			// remote call to NSK DSP
			g_NSKModemNumber = *pRxUD; // 0-3

			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send no data, receive data
			tdmServiceI_Request( e_TDMServiceAddr_NSK,	                                   // destination address
								 e_TDMServiceMsgType_NSKShortStatusData_Req,               // txMsgType,
								 &g_NSKModemNumber, sizeof(g_NSKModemNumber),
								 (unsigned long *) &g_NSK_ShortStatus,                     // receive buffer
								 sizeof(g_NSK_ShortStatus),                                // size of receive buffer
								 TDM_SERVICE_TIMEOUT );

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case MSG_NSK_GET_ADAPT_EQU_TF:    
		{
			blockSizeInLongs = *pRxUD++;
			addressOffset    = *pRxUD++;

			if (0 == addressOffset) // reading first segement -> fetch data from NSK DSP
			{
				// remote call to NSK DSP
				g_NSKModemNumber = *pRxUD; // 0-3

				g_tdmServiceConfirmCalled = FALSE; // reset flag
				g_messageTypeDelayedCommand = frame->MessageType;
					 
				// send no data, receive data
				tdmServiceI_Request( e_TDMServiceAddr_NSK,	                                   // destination address
									 e_TDMServiceMsgType_NSKGetAdapEquTF_Req,                  // txMsgType,
									 &g_NSKModemNumber, sizeof(g_NSKModemNumber),
									 (unsigned long *) &g_NSK_AdapEquTF,                       // receive buffer
									 sizeof(g_NSK_AdapEquTF),                                  // size of receive buffer
									 TDM_SERVICE_TIMEOUT );

				result = e_ExecMsgStatus_Deferred;
				g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
			}
	     	else
			{
				makeFrameHeader( (long*)&g_NSK_AdapEquTF + addressOffset, blockSizeInLongs);
			}


			break;                                                
		}

		case MSG_MUX_GET_VOICE_TRANSIT_STATUS:
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send no data, receive data
			tdmServiceI_Request( e_TDMServiceAddr_NSK,                                     // destination address
								 e_TDMServiceMsgType_MUXGetVoiceTransitStatus_Req,         // txMsgType,
								 NULL, 0,                                                  // no tx message
								 (unsigned long *) &g_MUX_VoiceTransitDevStatus,           // receive buffer
								 sizeof(g_MUX_VoiceTransitDevStatus),                      // size of receive buffer
								 TDM_SERVICE_TIMEOUT); 

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;   
		}

		case MSG_MUX_SET_VOICE_TRANSIT_TEST_CFG:
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 	
			MEMCPY(&g_MUX_VoiceTransitTestCfg, pRxUD, sizeof(g_MUX_VoiceTransitTestCfg)); // get parameters of request message

			// send data, receive no data
			tdmServiceI_Request( e_TDMServiceAddr_NSK,                                  // destination address
								 e_TDMServiceMsgType_MUXSetVoiceTransitTestCfg_Send,    // txMsgType,
								 (unsigned long *) &g_MUX_VoiceTransitTestCfg,          // transmit buffer
								 sizeof(g_MUX_VoiceTransitTestCfg),                     // size of transmit buffer
								 NULL, 0,                                               // no rx message
								 TDM_SERVICE_TIMEOUT); 

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;  
			
		}

		case MSG_OFDM_GET_SHORT_STATUS:    
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send no data, receive data
			tdmServiceI_Request( e_TDMServiceAddr_OFDM,                                    // destination address
								 e_TDMServiceMsgType_OFDMShortStatusData_Req,              // txMsgType,
								 NULL, 0,                                                  // no tx message
								 (unsigned long *) &g_OFDMShortStatus,                     // receive buffer
								 sizeof(g_OFDMShortStatus),                                // size of receive buffer
								 TDM_SERVICE_TIMEOUT + MAX_OFDM_SYMBOLDURATION_IN_TICKS);  // await maximum OFDM symbol duration

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case MSG_OFDM_GET_LONG_STATUS:    
		{
			static t_OFDM_LongStatusHeader  OFDMLongStatusReq;

			blockSizeInLongs      = *pRxUD++;
			addressOffset         = *pRxUD++;

			if (0 == addressOffset) // reading first segment -> fetch data from OFDM DSP
			{
				// save header
                MEMCPY (&OFDMLongStatusReq, pRxUD, sizeof(OFDMLongStatusReq));

				// remote call
				g_tdmServiceConfirmCalled = FALSE; // reset flag
				g_messageTypeDelayedCommand = frame->MessageType;
					 
				// send no data, receive data
				tdmServiceI_Request( e_TDMServiceAddr_OFDM,                                          // destination address
									 e_TDMServiceMsgType_OFDMLongStatusData_Req,                     // txMsgType,
									 (unsigned long*)&OFDMLongStatusReq,                             // tx message
									 sizeof(OFDMLongStatusReq),                                      // size of tx message
									 (unsigned long*)&g_OFDMLongStatusRes,                           // receive buffer
									 sizeof(t_OFDM_LongStatusHeader) + OFDMLongStatusReq.datalength, // size of receive buffer
									 TDM_SERVICE_TIMEOUT + MAX_OFDM_SYMBOLDURATION_IN_TICKS);        // await maximum OFDM symbol duration

				result = e_ExecMsgStatus_Deferred;
				g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
			}
	     	else
			{   // reading other than first segement
				makeFrameHeader( (long*)(&g_OFDMLongStatusRes) + addressOffset, blockSizeInLongs);
			}

			break;                                                
		}

		case CS_GET:    
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			g_ChannelSimGetParameter = *pRxUD;  // first long word in message is parameter number

			// send no data, receive data
			tdmServiceI_Request( e_TDMServiceAddr_FE,                                     // destination address
								 e_TDMServiceMsgType_CS_GetParameter_Req,                 // txMsgType,
								 &g_ChannelSimGetParameter,                               // transmit buffer
								 sizeof(g_ChannelSimGetParameter),                        // size of transmit buffer
								 (unsigned long *) &g_ChannelSimGetSetParameter,          // receive buffer
								 sizeof(g_ChannelSimGetSetParameter),                     // size of receive buffer
								 TDM_SERVICE_TIMEOUT); 

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case CS_SET:    
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 	
			MEMCPY(&g_ChannelSimGetSetParameter, pRxUD, sizeof(g_ChannelSimGetSetParameter)); // get parameters of request message

			// send no data, receive data
			tdmServiceI_Request( e_TDMServiceAddr_FE,                                     // destination address
								 e_TDMServiceMsgType_CS_SetParameter_Send,                // txMsgType,
								 (unsigned long *) &g_ChannelSimGetSetParameter,          // transmit buffer
								 sizeof(g_ChannelSimGetSetParameter),                     // size of transmit buffer
								 NULL, 0,                                                 // no rx message
								 TDM_SERVICE_TIMEOUT); 

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case SEND_LOOPTEST:
		{
			// Delayed command, Ack must be transmitted before reset
			g_messageTypeDelayedCommand = frame->MessageType;
			makeFrameHeader( NULL, 0); // send acknowledge

			g_LoopTestMode = *pRxUD;   // save looptest mode
			break;                                                
		}

		case GET_LOOPTEST_TIME:
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send data, receive no data
			tdmServiceI_Request( e_TDMServiceAddr_NSD,                          // destination address
								 e_TDMServiceMsgType_NSDLooptest_GetResult_Req, // txMsgType,
								 NULL, 0,                                       // no tx message
								 &g_NSDLoopTestDelay, sizeof(g_NSDLoopTestDelay), 
								 TDM_SERVICE_TIMEOUT);

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case MSG_READ_NSD_DEVELOPER_STATUS:
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send data, receive no data
			tdmServiceI_Request( e_TDMServiceAddr_NSD, 
								 e_TDMServiceMsgType_NSD_Read_Developer_Status_Req,
								 NULL, 0,
								 (unsigned long*)&g_NSD_Developer_Status, sizeof(g_NSD_Developer_Status),
								 TDM_SERVICE_TIMEOUT);
			
			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;
		}

		case READ_TEST_CONFIG_NSD:    
		{
			// remote call
			g_tdmServiceConfirmCalled = FALSE; // reset flag
			g_messageTypeDelayedCommand = frame->MessageType;
				 
			// send data, receive no data
			tdmServiceI_Request( e_TDMServiceAddr_NSD,                                      // destination address
								 e_TDMServiceMsgType_NSD_Read_TestsignalConfig_Req,                    // txMsgType,
								 NULL, 0,                                                  // no tx message
								 (unsigned long *) &g_NSD_TestSignalConfig,                        // receive buffer
								 sizeof(g_NSD_TestSignalConfig),                                 // size of receive buffer
								 TDM_SERVICE_TIMEOUT);

			result = e_ExecMsgStatus_Deferred;
			g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

			break;                                                
		}

		case RESET_P4LQ_PASSWORD:

			// set password to received (default) value 
			// (same as CHANGE_P4LQ_PASSWORD but no write permission is needed)

			MEMCPY(g_p_EpromLocalConfiguration->Password, 
			       pRxUD, 
				   sizeof(g_p_EpromLocalConfiguration->Password));

			eprom_storePermanentConfiguration();

			g_pCurrentCfg->system.remainingActiveTestConfigTime = 0; // de-activates time-bomb
	
			makeFrameHeader( NULL, 0); 
			break;

		///////////////////////////////////////////////////////////
		// Equalizer activities
		///////////////////////////////////////////////////////////

		case GET_DEVICE_STATE:

			makeFrameHeader(&g_p_StatusData->system.deviceState, sizeof(g_p_StatusData->system.deviceState));
			break;

		case GET_EQUALIZER_STATUS: {
			
			long chNum;
			long statusType;
			t_EqualizerStatus *pTemp;
			static t_EqualizerStatus *pEqStatus;
			
			blockSizeInLongs = *pRxUD++;
			addressOffset    = *pRxUD++;
			chNum			 = *pRxUD++;
			statusType		 = *pRxUD;
			
			if (addressOffset == 0) {	// workaround: pEqStatus is set only at the first read access

				switch (statusType)
				{
					case EQUALIZER_STATUS_NEW:   // status data of latest channel measurement 

						pEqStatus = equalizer_getStatusNewPtr(chNum);
						
						makeFrameHeader(((long*)pEqStatus) + addressOffset, blockSizeInLongs);
						
						break;

					case EQUALIZER_STATUS_ACTIVE: // status data of active equalizer

						pTemp = configuration_getPEqualizerStatus();  

						if ((chNum >= APLC_CH1) && (chNum <= APLC_CH3)) {

							pEqStatus = &pTemp[chNum];
							makeFrameHeader(((long*)pEqStatus) + addressOffset, blockSizeInLongs);
						}
						else { // invalid APLC channel number

							result = e_ExecMsgStatus_DataNotAvailable; // NACK: Requested data not available
							makeFrameHeader( NULL, 0);
						};

						break;

					// case EQUALIZER_STATUS_EMPTY is not (yet) supported

					default:
						result = e_ExecMsgStatus_DataNotAvailable; // NACK: Requested data not available
						makeFrameHeader( NULL, 0);

				} // end of switch(statusType)
			}
			else
			{
				makeFrameHeader(((long*)pEqStatus) + addressOffset, blockSizeInLongs);
			}

			break;      
		}

		case READ_EQUALIZER_CONFIG: {

			long chNum;

			chNum = *pRxUD;

			if ((chNum >= APLC_CH1) &&  (chNum <= APLC_CH3)) {

				makeFrameHeader((long*)&g_pCurrentCfg->APLC.equalizer[chNum], sizeof(t_EqualizerConfig));
			}
			else {
				
				result = e_ExecMsgStatus_DataNotAvailable; // NACK: Requested data not available
				makeFrameHeader( NULL, 0);
			};
			
			break;  
		}

#endif
		///////////////////////////////////////////////////////////			
		// FW Update activities
		// FW Update activities, used to download a new firmware, stored in the FlashEprom 
 		///////////////////////////////////////////////////////////	

		case END_OF_PROG_MESSAGE_P4LQ:

			makeFrameHeader( NULL, 0);

#ifndef EMERGENCY_FW

			monitor_blinkWarningLED(FALSE); // stop blinking of the warning LED
			
			eventRec_enable (TRUE);         // re-enable eventrecorder

			monitor_enableReset(TRUE);      // enable reset if CRC error is detected

			// do not turn on RF signal again
#endif
			break;

		///////////////////////////////////////////////////////////	
		/// RTC activities
 		///////////////////////////////////////////////////////////	
#ifndef EMERGENCY_FW
		case UPLOAD_RTC_DATA:
			makeFrameHeader( (long*)m_timeDatePtr, sizeof(t_TimeDate));
			break;

		case CONVERT_RTC_DATA_FOR_READ:
			// added for LAN alarm polling. This forces the conversion of internal time to HMI format
			// After that the LAN can read the time without write access.
			// The HMI uses message DOWNLOAD_RTC_DATA first to set the RTCmode to RTC_READ_TIME_AND_DATE
			// and therfore it needs write access first to read RTC.
			// This is a heritage of ETL500/MMI500
			m_timeDatePtr->RTCmode = RTC_READ_TIME_AND_DATE; // This forces the conversion of internal time to HMI format

			makeFrameHeader( NULL, 0);
			break;

 		///////////////////////////////////////////////////////////	
		/// EventRecoder
 		///////////////////////////////////////////////////////////	
 	    case READ_EVENT: 
		{
			static t_HMIEventData * pHmiEventData;
			
			// HMI supplies a pointer relative to the begin of the current event sector
			pHmiEventData = eventRec_readEvent((long*)(*pRxUD));

 			makeFrameHeader(pHmiEventData, sizeof(*pHmiEventData)); 
		}
		break;   
			
        case READ_EVENT_COUNTERS:   // Zähler im RAM lesen

			blockSizeInLongs      = *pRxUD++;
			addressOffset         = *pRxUD;

            makeFrameHeader((long*)(eventRec_readRAMCounters()) + addressOffset, blockSizeInLongs);
			break;  

		case READ_ROM_COUNTERS:     // Zähler im ROM lesen

			blockSizeInLongs      = *pRxUD++;
			addressOffset         = *pRxUD++;

            makeFrameHeader((long*)(eventRec_readROMCounters()) + addressOffset, blockSizeInLongs);
			break;	
#endif

		//////////////////////////////////////////////////////
		// password activities
 		///////////////////////////////////////////////////////////	
		case DOWNLOAD_P4LQ_PASSWORD: // verify password
		{
			// only used to login
			// password consists of 4 long.
			
			  // compare (emulate memcmp)

			int i = 0;
			g_WriteAccessAvailable = TRUE;

#ifndef EMERGENCY_FW
			while (i < sizeof(g_p_EpromLocalConfiguration->Password))
			{
				if (g_p_EpromLocalConfiguration->Password[i++] != *(pRxUD++))
				{
					g_WriteAccessAvailable = FALSE;
					break;
				}
			}
#endif
		    makeFrameHeader(&g_WriteAccessAvailable, sizeof(g_WriteAccessAvailable)); 
			
			break;
		}

		default:

			if (g_WriteAccessAvailable)
			{
				switch(frame->MessageType)
				{  
						
					//ab hier die Schreibbefehle (passwortgeschützt):

					///////////////////////////////////////////////////////////
					// Firmware download
					///////////////////////////////////////////////////////////

					case FW_UPDATE_MESSAGE_P4LQ:  

#ifdef EMERGENCY_FW
						makeFrameHeader( NULL, 0);
#else
						monitor_blinkWarningLED(TRUE); // turn on blinking of the warning LED

						eventRec_recordEvent(ALARM_EVENT_FIRMWARE_DOWNLOAD, 0, FALSE);

						eventRec_enable (FALSE);       // disable eventrecorder

						// turn off RF tx signal if not in debug mode
						// (in debug mode, the master DSP does not boot the slave DSPs. Thus the 
						//  front end would not reply to this TDM service message)
						if (isInDebugMode())
						{
							makeFrameHeader( NULL, 0); // Acknowledge if in debug mode
						}
						else
						{	// temporarily set tx RF level adjustment to zero (cuts RF signal)
							// remote call
							g_tdmServiceConfirmCalled = FALSE; // reset flag
							g_messageTypeDelayedCommand = frame->MessageType;
								 
							g_commissioningCfg.command             = COMM_CMD_ADJ_TX_RF_LEVEL;
							g_commissioningCfg.txRfLevelAdjustment = 0; 
							g_commissioningCfg.txTestToneLevel     = 0;

							// send data to FE DSP
							tdmServiceI_Request( e_TDMServiceAddr_FE,						// destination address
												 e_TDMServiceMsgType_CommissioningCfg_Send,	// txMsgType,
												 (unsigned long*)&g_commissioningCfg,		// transmit buffer
												 sizeof(g_commissioningCfg),				// size of transmit buffer
												 (unsigned long*)&g_commissionningRxBuffer,	// rx buffer
												 sizeof(g_commissionningRxBuffer),			// size of rx buffer
												 TDM_SERVICE_TIMEOUT); 

							result = e_ExecMsgStatus_Deferred;
							g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;
						}
#endif
						break;

					case ERASE_FLASH_MESSAGE_P4LQ: 
#ifndef EMERGENCY_FW						
						monitor_enableReset(FALSE);    // disable possibility of reset in case of CRC error	
#endif
						eprom_eraseFirmware();
						makeFrameHeader(&EraseMessageACK, sizeof(EraseMessageACK));
						break;

					case PRG_FLASH_MESSAGE_P4LQ:
						
						if(programData(pRxUD)) 
							makeFrameHeader(&EraseMessageACK, sizeof(EraseMessageACK));
						else
							makeFrameHeader(&EraseMessageNACK, sizeof(EraseMessageNACK));
						
						break;
					

					case MSG_ENABLE_EMERGENCY_FW_UPDATE:

						*g_firmware_startAddress_Ptr = SECTOR_SA0;	// Erease and programming from SECTOR_SA0
						*(long*)(IOFLAG) |= (FLG10);				// Enable write access of FLASH
#ifndef EMERGENCY_FW
						monitor_blinkWarningLED(TRUE);				// turn on blinking of the warning LED
#endif
						makeFrameHeader( NULL, 0);

						break;
#ifndef EMERGENCY_FW
				
					case CHANGE_P4LQ_PASSWORD:

						MEMCPY(g_p_EpromLocalConfiguration->Password, 
							   pRxUD, 
							   sizeof(g_p_EpromLocalConfiguration->Password));

						eprom_storePermanentConfiguration();

						g_pCurrentCfg->system.remainingActiveTestConfigTime = 0; // de-activates time-bomb

						makeFrameHeader( NULL, 0); 
						break;

					case DOWNLOAD_P4LQ_CONFIGURATION_DATA:
						
						// Backup ConfigurationCounter. 
						TempStorage = g_pCurrentCfg->system.equipmentInfo.configurationCounter;

						// get block size and offset from message
						blockSizeInLongs    	= *pRxUD++;
						addressOffset           = *pRxUD++;

						// calculate start address in t_P4LT_Config structure
						TempPtr1 = (long*)g_pNewCfg + addressOffset;	

						// prevent from overwriting past the t_P4LT_Config structure
						if((TempPtr1 + blockSizeInLongs) > ((long*)g_pNewCfg + sizeof(*g_pNewCfg)))
						{
							EndPtr = (long*)g_pNewCfg + sizeof(*g_pNewCfg);
						}
						else
						{
							EndPtr = TempPtr1 + blockSizeInLongs; // Endadresse der heruntergeladenen Daten
						}

						while(TempPtr1 < EndPtr) // copy structure
						{
							*TempPtr1++ = *pRxUD++;
						}

						makeFrameHeader( NULL, 0); // send reply
						
						// Restore ConfigurationCounter. 
						g_pNewCfg->system.equipmentInfo.configurationCounter = TempStorage;

						break;		

					case USE_TEMPORARY_CFG: 

						eventRec_recordEvent(ALARM_EVENT_CONFIGURATION_DOWNLOAD, 0, FALSE);

						// do reset, later
						g_messageTypeDelayedCommand = frame->MessageType;

						// increment configuration counter in new configuration:
						g_pNewCfg->system.equipmentInfo.configurationCounter = g_pCurrentCfg->system.equipmentInfo.configurationCounter + 1;

						eprom_storeTemporaryConfiguration();
						makeFrameHeader( NULL, 0);
						break;

					case SAVE_DATA_TO_EPROM: 

						eventRec_recordEvent(ALARM_EVENT_EPROM_SAVE, 0, FALSE);

						eprom_markTemporaryConfigurationAsPermanent();

						configuration_setConfigurationToPermanent();

						g_pCurrentCfg->system.remainingActiveTestConfigTime = 0; // de-activates time-bomb

						makeFrameHeader( NULL, 0);
						break;

					case SET_OPERATION_MODE:
					
						// remote call
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = frame->MessageType;
							 
						g_operationMode = *pRxUD;

						if (g_operationMode == OP_MODE_NORMAL) {

							// reset tx RF frequency
							tdmUpConvSettings_initSender(g_pCurrentCfg->system.RF.txFrequency, 
														 g_pCurrentCfg->system.RF.channelMode);
						};

						// send data to FE DSP
						tdmServiceI_Request( e_TDMServiceAddr_FE,						// destination address
											 e_TDMServiceMsgType_OperationMode_Send,	// txMsgType,
											 &g_operationMode, sizeof(g_operationMode),	// transmit buffer
											 NULL, 0,									// no rx message
											 TDM_SERVICE_TIMEOUT); 

						result = e_ExecMsgStatus_Deferred;
						g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

						break; 
#endif
					case RESET_BOARD: // performs a reset

#ifdef EMERGENCY_FW
						*(long*)(MSGR3) = 1000;  // needed for Emergency FW - no delayed messages supported by
												 // Emergency FW. *(long*)(MSGR3) = 1000; = delay counter ->
												 // will be decremented by EMERGENCY_Main.c main()
#else 
						// Delayed command, Ack must be transmitted before reset
						g_messageTypeDelayedCommand = frame->MessageType;

						eventRec_recordEvent(ALARM_EVENT_USER_RESET, 0, FALSE);
#endif
						makeFrameHeader( NULL, 0);
						break;
#ifndef EMERGENCY_FW


					///////////////////////////////////////////////////////////
					// Commissioning activities
					///////////////////////////////////////////////////////////

					case COMMISSIONING_CFG_SEND:
					{
						// remote call
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = frame->MessageType;
							 
						MEMCPY((unsigned long*)&g_commissioningCfg, pRxUD, sizeof(g_commissioningCfg));

						switch(g_commissioningCfg.command) {

							case COMM_CMD_ADJ_TX_RF_LEVEL:
							case COMM_CMD_ADJ_RX_RF_LEVEL:		
							case COMM_CMD_TUNE_TX_FILTER:
							case COMM_CMD_TUNE_RX_FILTER:
							case COMM_CMD_TUNE_HYBRID:
							case COMM_CMD_ACT_TX_ALARM_LEVEL:		

								// send data to UC DSP
								tdmUpConvSettings_initSender(g_commissioningCfg.txRfFrequency, RF_CHANNEL_MODE_ERECT);
								break;

							// default: do nothing
						};

						// send data to FE DSP
						tdmServiceI_Request( e_TDMServiceAddr_FE,						// destination address
											 e_TDMServiceMsgType_CommissioningCfg_Send,	// txMsgType,
											 (unsigned long*)&g_commissioningCfg,		// transmit buffer
											 sizeof(g_commissioningCfg),				// size of transmit buffer
											 (unsigned long*)&g_commissionningRxBuffer,	// rx buffer
											 sizeof(g_commissionningRxBuffer),			// size of rx buffer
											 TDM_SERVICE_TIMEOUT); 

						result = e_ExecMsgStatus_Deferred;
						g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

						break;
					}


					///////////////////////////////////////////////////////////
					// Equalizer activities
					///////////////////////////////////////////////////////////

					case START_MEASUREMENT:
						
						switch(*pRxUD) {

							case APLC_CH1: 
								g_p_StatusData->system.deviceState |= DS_MEASURE_RX_CH1;
								break;

							case APLC_CH2:
								g_p_StatusData->system.deviceState |= DS_MEASURE_RX_CH2;
								break;

							case APLC_CH3:
								g_p_StatusData->system.deviceState |= DS_MEASURE_RX_CH3;
								break;

							// default: do nothing
							
						};
						makeFrameHeader( NULL, 0);
						break;

					
					case SET_EQUALIZER_CONFIG: {

						long chNum;
						long flag;
						t_EqualizerStatus *pEqStatusActive, *pEqStatusNew;

						chNum = *pRxUD++;

						if ((chNum >= APLC_CH1) && (chNum <= APLC_CH3)) {

							g_pNewCfg->APLC.equalizer[chNum].equalizerSettings = *pRxUD++;
							g_pNewCfg->APLC.equalizer[chNum].onlyAmplitudeEqualizationActive = *pRxUD++;

							flag = *pRxUD;

							if (flag == EQUALIZER_STATUS_NEW) {

								pEqStatusActive = configuration_getPEqualizerStatus();
								pEqStatusNew = equalizer_getStatusNewPtr(chNum);
								MEMCPY((long*)&pEqStatusActive[chNum], (long*)pEqStatusNew, sizeof(t_EqualizerStatus));
								pEqStatusActive[chNum].flag = EQUALIZER_STATUS_ACTIVE;
							};

							// increment configuration counter in new configuration:
							g_pNewCfg->system.equipmentInfo.configurationCounter = g_pCurrentCfg->system.equipmentInfo.configurationCounter + 1;

							eprom_storePermanentConfiguration();

							result = e_ExecMsgStatus_Deferred;
							g_messageTypeDelayedCommand = RESET_BOARD;

						}
						else {
							
							result = e_ExecMsgStatus_DataNotAvailable;
						};

						makeFrameHeader(NULL, 0);

						break;
					}

					///////////////////////////////////////////////////////////
					// RTC activities
					///////////////////////////////////////////////////////////

					case DOWNLOAD_RTC_DATA:

						MEMCPY(m_timeDatePtr, 
							   pRxUD, 
							   __min(numberOfReceivedLongValues, sizeof(t_TimeDate)));
						
						//set the ALARM_EVENT_DATE_TIME_SET only if we write the time&date
						//DOWNLOAD_RTC_DATA is also used for read time&date; heritage of P4LQ; see comment in rtc.c
						if(m_timeDatePtr->RTCmode == RTC_WRITE_TIME_AND_DATE)
								eventRec_recordEvent(ALARM_EVENT_DATE_TIME_SET, 0, FALSE);

						makeFrameHeader( NULL, 0);
						break; 
						
					///////////////////////////////////////////////////////////
					// Event recorder activities
					///////////////////////////////////////////////////////////

					case RESET_EVENT_COUNTERS:    
						
						if (!eventRec_resetCounter((unsigned long)*pRxUD))
						{
							result = e_ExecMsgStatus_DataNotAvailable; // NACK: Requested data not available
						}

						makeFrameHeader( NULL, 0); 
						break;   

					///////////////////////////////////////////////////////////
					// Test switches
					///////////////////////////////////////////////////////////

					case DOWNLOAD_P4LQ_TEST_SWITCHES: 
					{
						// remote call
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = frame->MessageType;
							 
						MEMCPY(g_pTestPlcLinkCfg, pRxUD, sizeof(*g_pTestPlcLinkCfg));

						// start time-bomb
						g_pCurrentCfg->system.remainingActiveTestConfigTime = g_pTestPlcLinkCfg->remainingActiveTestConfigTime;

						// send data
						tdmServiceI_Request( e_TDMServiceAddr_FE,							// destination address
											 e_TDMServiceMsgType_SetTestPlcLinkCfg_Send,	// txMsgType,
											 (unsigned long *) g_pTestPlcLinkCfg,			// transmit buffer
											 sizeof(*g_pTestPlcLinkCfg),					// size of transmit buffer
											 NULL, 0,										// no rx message
											 TDM_SERVICE_TIMEOUT); 

						result = e_ExecMsgStatus_Deferred;
						g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

						break;                                                
					}

					///////////////////////////////////////////////////////////
					// TPE activities
					///////////////////////////////////////////////////////////

					case SET_NSD_LOCAL_LOOP:
					{
						// remote call
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = frame->MessageType;
						//
						g_NSDLocalLoopMode = *pRxUD;   // save local loop mode

						// send data, receive no data
						tdmServiceI_Request(e_TDMServiceAddr_NSD,							// destination address
											e_TDMServiceMsgType_NSD_SetLocalLoop_Req,		// txMsgType,
											&g_NSDLocalLoopMode, sizeof(g_NSDLocalLoopMode),
											NULL, 0,										// no rx message
											TDM_SERVICE_TIMEOUT);

						result = e_ExecMsgStatus_Deferred;
						g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

						break;                                                
					}

					case WRITE_TEST_CONFIG_NSD: 
					{
						// remote call
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = frame->MessageType;
							 
						MEMCPY(&g_NSD_TestSignalConfig, pRxUD, sizeof(g_NSD_TestSignalConfig)); // get and save parameters of request message

						// send no data, receive data
						tdmServiceI_Request( e_TDMServiceAddr_NSD,                                     // destination address
											 e_TDMServiceMsgType_NSD_Write_TestsignalConfig_Req,       // txMsgType,
											 (unsigned long *) &g_NSD_TestSignalConfig,                // transmit buffer
											 sizeof(g_NSD_TestSignalConfig),                           // size of transmit buffer
											 NULL, 0,                                                  // no rx message
											 TDM_SERVICE_TIMEOUT); 

						result = e_ExecMsgStatus_Deferred;
						g_delayedMessageTypesStatus = e_ExecMsgStatus_AwaitingResponse;

						break;                                                
					}

					///////////////////////////////////////////////////////////
					// Developer activities
					///////////////////////////////////////////////////////////

					case MSG_ERASE_EVENT_RECORDER: 
					{
						// Delayed command, Ack must be transmitted before reset
						g_messageTypeDelayedCommand = STATE_CLEAR_EVENT_RECORDER;

						makeFrameHeader( NULL, 0); // send acknowledge
						break;                                                
					}


#endif
					default:
							result = e_ExecMsgStatus_DataNotAvailable; // NACK: Requested data not available
							makeFrameHeader( NULL, 0); 
						break;

				} // switch(frame->MessageType)
			}
			else 
			{
				result = e_ExecMsgStatus_AccessDenied; // NACK: Password Protection or Requested data not available
				makeFrameHeader( NULL, 0); 
				break;

			} // if (g_writeAccessAvailable)

	}  // switch(frame->MessageType)
	return result;
}

// For critical commands which take away the complete calulation time or interrupt the EOC, these
// message types are transfered to g_messageTypeDelayedCommand when this function 
// waits until all Telegram buffers are empty and these commands are executed. 
// Called in 100 Hz cycle

// also used to time-out write access to P4LT


// For Debugging
extern unsigned long TestcountDSP; 


void SEGMENT_EXT_CODE msgHandler_executeDelayedMessageTypes(void)
{

#ifndef EMERGENCY_FW

	static t_NSK_DSP_Status temp_NSK_DSP_Status;

	if (g_messageTypeDelayedCommand)		// Are there any commands waiting to be executed?
	{
        // first, we process the pending TDM service messages
		switch (g_messageTypeDelayedCommand) 
		{
			case STATE_SEND_LOOPTEST:

				// ignore TDM service result because we can't react anyway
				g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
			    break;

			case GET_LOOPTEST_TIME:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSDLooptest_GetResult_Res == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
						makeFrameHeader( &g_NSDLoopTestDelay, sizeof(g_NSDLoopTestDelay)); 
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_READ_NSD_DEVELOPER_STATUS:
				if(g_tdmServiceConfirmCalled)
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSD_Read_Developer_Status_Resp == g_rxMessageType))
					{
						g_NSD_Developer_Status.l_Development_1 = TestcountDSP;
						
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
						makeFrameHeader( &g_NSD_Developer_Status, sizeof(g_NSD_Developer_Status)); 
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
				break;

			case READ_TEST_CONFIG_NSD:
				if(g_tdmServiceConfirmCalled)
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSD_Read_TestsignalConfig_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
						makeFrameHeader( &g_NSD_TestSignalConfig, sizeof(g_NSD_TestSignalConfig)); 
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
				break;

			case READ_IDLE_COUNTERS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_IdleCounter_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_idleCounters, sizeof(g_idleCounters));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_NSK_GET_SHORT_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSKShortStatusData_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_NSK_ShortStatus, sizeof(g_NSK_ShortStatus));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_NSK_GET_ADAPT_EQU_TF:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSKGetAdapEquTF_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_NSK_AdapEquTF, blockSizeInLongs); // address offset = 0
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_MUX_GET_VOICE_TRANSIT_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_MUXGetVoiceTransitStatus_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_MUX_VoiceTransitDevStatus, 
										sizeof(g_MUX_VoiceTransitDevStatus));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_MUX_SET_VOICE_TRANSIT_TEST_CFG:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType)        )
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_OFDM_GET_SHORT_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_OFDMShortStatusData_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_OFDMShortStatus, sizeof(g_OFDMShortStatus));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case MSG_OFDM_GET_LONG_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_OFDMLongStatusData_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
						makeFrameHeader(&g_OFDMLongStatusRes, blockSizeInLongs);
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;


			case UPLOAD_P4LT_STATUS_DATA_W4_FE_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_FE_StatusData_Resp == g_rxMessageType))
					{
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = UPLOAD_P4LT_STATUS_DATA_W4_NSK_STATUS;
							 
						// get NSK status from NSK DSP
						// send no data, receive data
						tdmServiceI_Request( e_TDMServiceAddr_NSK,                            // destination address
											 e_TDMServiceMsgType_StatusData_Req,              // txMsgType,
											 NULL, 0,
											 (unsigned long *)&temp_NSK_DSP_Status, 
											 sizeof(t_NSK_DSP_Status),                                         
											 TDM_SERVICE_TIMEOUT);
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
						g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
					}
				}
			    break;

			case UPLOAD_P4LT_STATUS_DATA_W4_NSK_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSK_StatusData_Resp == g_rxMessageType))
					{
						g_tdmServiceConfirmCalled = FALSE; // reset flag
						g_messageTypeDelayedCommand = UPLOAD_P4LT_STATUS_DATA_W4_NSD_STATUS;

						// copy temp NSK status to P4LT status (MUX_O4CV has to be copied separately because its not adjacent to NSK and MUX)
						// to									from					         number of longs
						MEMCPY(&g_p_StatusData->NSK,			&temp_NSK_DSP_Status.NSK,        sizeof(t_NSK_Status));
						MEMCPY(&g_p_StatusData->NSKEqualizer,	&temp_NSK_DSP_Status.equalizer,  sizeof(t_NSK_EqualizerStatus));
						MEMCPY(&g_p_StatusData->MUX,			&temp_NSK_DSP_Status.MUX,        sizeof(t_MUX_Status));
						MEMCPY(&g_p_StatusData->MUX_O4CV,		&temp_NSK_DSP_Status.MUX_O4CV,   sizeof(t_MUX_O4CV_Status));

						g_p_StatusData->system.muxTransitMissingRxData = temp_NSK_DSP_Status.MUX_VoiceTransit.missingRxData;
							 
						// get NSD status from NSD DSP
						// send no data, receive data
						tdmServiceI_Request( e_TDMServiceAddr_NSD,                            // destination address
											 e_TDMServiceMsgType_StatusData_Req,              // txMsgType,
											 NULL, 0,
											 (unsigned long *)&g_p_StatusData->ITS, 
											 sizeof(g_p_StatusData->ITS),                                         
											 TDM_SERVICE_TIMEOUT);
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
						g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
					}
				}
			    break;

			case UPLOAD_P4LT_STATUS_DATA_W4_NSD_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_NSD_StatusData_Resp == g_rxMessageType))
					{
						// check if OFDM is active
						if (0 != g_pCurrentCfg->OFDM.channelBW)
						{
							g_tdmServiceConfirmCalled = FALSE; // reset flag
							g_messageTypeDelayedCommand = UPLOAD_P4LT_STATUS_DATA_W4_OFDM_STATUS;
								 
							// get OFDM short status from OFDM DSP
							// send no data, receive data
							tdmServiceI_Request( e_TDMServiceAddr_OFDM,                                    // destination address
												 e_TDMServiceMsgType_OFDMShortStatusData_Req,              // txMsgType,
												 NULL, 0,
												 (unsigned long *)&g_p_StatusData->DPLC.MOD600, 
												 sizeof(g_p_StatusData->DPLC.MOD600),                                         
												 TDM_SERVICE_TIMEOUT + MAX_OFDM_SYMBOLDURATION_IN_TICKS);
						}
						else // no OFDM modem configured
						{
							g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
							makeFrameHeader(g_p_StatusData, blockSizeInLongs); // address offset = 0
							g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
						}
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
						g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
					}
				}
			    break;

			case UPLOAD_P4LT_STATUS_DATA_W4_OFDM_STATUS:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_OFDMShortStatusData_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;

						makeFrameHeader(g_p_StatusData, blockSizeInLongs); // address offset = 0
							 
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;
				
			case SAMPLE_DATA_MESSAGE_P4LQ:

				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
					    (e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader( NULL, 0);
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case READ_FLOAT_ARRAY_MESSAGE:

				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_FE_SignalBuffer_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_sampleBuffer, sizeof(g_sampleBuffer));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case CS_GET:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok                   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_CS_GetParameter_Res == g_rxMessageType)        )
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_ChannelSimGetSetParameter, sizeof(g_ChannelSimGetSetParameter));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}

					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case CS_SET:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType)        )
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;


			case UPLOAD_P4LQ_TEST_SWITCHES:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok					   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_GetTestPlcLinkCfg_Res == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(g_pTestPlcLinkCfg, sizeof(*g_pTestPlcLinkCfg));
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}

					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case DOWNLOAD_P4LQ_TEST_SWITCHES:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;
			
			case SET_NSD_LOCAL_LOOP:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case WRITE_TEST_CONFIG_NSD:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case SET_OPERATION_MODE:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 
					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case COMMISSIONING_CFG_SEND:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
						
						if (g_commissioningCfg.command == COMM_CMD_SAVE_TX_RF_LEVEL_ADJ) {

							g_pCurrentCfg->system.RF.txLevelAdjustment = g_commissioningCfg.txRfLevelAdjustment;
							g_pNewCfg->system.RF.txLevelAdjustment = g_commissioningCfg.txRfLevelAdjustment;
							
							// increment also current configurationCounter because board is not reset
							g_pCurrentCfg->system.equipmentInfo.configurationCounter++; 
							g_pNewCfg->system.equipmentInfo.configurationCounter = g_pCurrentCfg->system.equipmentInfo.configurationCounter;
							eprom_storePermanentConfiguration();
						};
						makeFrameHeader( NULL, 0); 
					}
					else if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
							 (e_TDMServiceMsgType_CommissioningGetRxTestToneLevel_Resp == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
	     				makeFrameHeader(&g_commissionningRxBuffer, sizeof(g_commissionningRxBuffer));

					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
						makeFrameHeader( NULL, 0); 
					}

					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

			case FW_UPDATE_MESSAGE_P4LQ:
				if (g_tdmServiceConfirmCalled) // check if request completed
				{
					if ((e_TDMServiceResult_Ok   == g_TDMServiceResult) &&
						(e_TDMServiceMsgType_Ack == g_rxMessageType))
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_Ok;
					}
					else
					{
						g_delayedMessageTypesStatus = e_ExecMsgStatus_DataNotAvailable;
					}

					makeFrameHeader( NULL, 0); 

					g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
				}
			    break;

		    // no pending TDM service messages: must be some other delayed command
			default: 
			{
				static BOOL bBuffersAreEmpty = FALSE;
				static int counter;	// Necessary to wait until the message is generated in the buffer

				if (!bBuffersAreEmpty)
				{
					// Wait 1 sec until telegramm is generated in buffer
					if (++counter > 100)  
					{	
						bBuffersAreEmpty = TRUE;
						counter = 0; // reset counter
					}
				}
				else	// Check if all buffers are empty and execute command
				{
					if (g_pReceiveBuffersEmpty())
					{
						// Wait 0.5 additional sec until telegramm is sent completely
						if (++counter > 50) 
						{	 
							switch (g_messageTypeDelayedCommand)
							{
								case SEND_LOOPTEST:
									// initiate TDM service request

									g_tdmServiceConfirmCalled = FALSE; // reset flag
										 
									// send data, receive no data
									tdmServiceI_Request( e_TDMServiceAddr_NSD,                        // destination address
														 e_TDMServiceMsgType_NSDLooptest_Trigger_Req, // txMsgType,
														 &g_LoopTestMode, sizeof(g_LoopTestMode),
														 NULL, 0,                                     // no rx message
														 TDM_SERVICE_TIMEOUT);
									
									g_messageTypeDelayedCommand = STATE_SEND_LOOPTEST;	// wait for TDM service response
									break;

								case STATE_CLEAR_EVENT_RECORDER:
									eventRec_clearAndReset(); // caller will not return
									break;

								case RESET_BOARD:
								case USE_TEMPORARY_CFG:

									resetP4LTandR1LA();	// reset P4LT & LANi
														// it dosen't matter if a LANi is present or not
									break;

								default: ASSERT(FALSE);
							}

							g_messageTypeDelayedCommand = MESSAGE_ID_NONE;
							counter = 0; // reset counter
							bBuffersAreEmpty = FALSE;
						}
					}
				} // if (!bBuffersAreEmpty)

			} // default
		} // switch (g_messageTypeDelayedCommand) 
	}

	// time-out write access to P4LT
	if (g_WriteAccessTimer > 0) 
	{
		if (--g_WriteAccessTimer == 0) 
		{
			g_WriteAccessAvailable = FALSE;
		}
	}
#endif
}

e_ExecMsgStatus msgHandler_lastDelayedMessageTypeStatus(void)
{
	return g_delayedMessageTypesStatus;
}

