/*-------------------------------------------------------------------------*
 * $Workfile: ExternalTDM_BusBridge.c $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :
 * Purpose      :
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/P4LT/Master_DSP/C/ExternalTDM_BusBridge.c $
 * 
 * 41    08-10-24 17:19 Chtognu
 * ALM_NSD_Trip_and_Guard and ALM_NSD_Loop_Test removed from PAX mute
 * criterion (doSlowMuteBridge()) other NSD Alarms do still cause the mute
 * criterion
 * 
 * 39    29.01.08 11:13 Chhamaa
 * APLC channel limiter added (FromO4LE_ExternalTDM_Bridge())
 * 
 * 38    29.11.07 16:57 Chhamaa
 * Extended time slots of O4CV board #4 moved to basic time slots 8 and 9.
 * 
 * 37    17.04.07 9:44 Chhamaa
 * Calculation g_O4CVExtended32TsOffsetTx added.
 * 
 * 36    1.03.07 17:31 Chhamaa
 * Bridging of O4CV subframe start flags added.
 * 
 * 35    17.01.07 17:46 Chhamaa
 * O4CV stuff added.
 * 
 * 33    18.09.06 16:28 Chhamaa
 * Function receiveBasicTimeSlot6() added.
 * 
 * 32    22.08.06 17:15 Chhamaa
 * doSlowMuteBridge() moved to external memory.
 * 
 * 28    26.10.05 9:27 Chchlee
 * Debug events removed
 * 
 * 27    24.10.05 16:28 Chchlee
 * temporary debug code removed
 * 
 * 26    18.10.05 9:44 Chchlee
 * E and M wire routing bug fixed
 * 
 * 25    3.10.05 14:55 Chchlee
 * 
 * 24    29.09.05 13:29 Chchlee
 * 
 * 23    26.09.05 17:39 Chchlee
 * intermediate version
 * 
 * 22    21.09.05 11:30 Chchlee
 * Intermediate version
 * 
 * 21    18.09.05 16:15 Chchlee
 * Cleaned up
 * 
 * 20    18.09.05 15:28 Chchlee
 * 
 * 19    15.09.05 13:53 Chchlee
 * Comments added
 * 
 * 18    8.09.05 16:35 Chchlee
 * BASIC_G4AI_FRAME_ID_BIT: new constant name
 * 
 * 17    18.07.05 16:00 Chhamaa
 * E_and_M_WireBridge(): M_WireInformationToInternalTDM_Bus initially set
 * to  {0,0,0,0} to prevent random signal on M-Wire
 * 
 * 16    15.07.05 11:50 Chchlee
 * All external TDM bus supervision logic deactivated until redesign
 * 
 * 15    6.07.05 13:50 Chchlee
 * 
 * 14    6.07.05 10:16 Chchlee
 * temporary code for debugging bug 88
 * 
 * 13    1.07.05 9:45 Chchlee
 * temporary version to catch bug
 * 
 * 12    30.06.05 8:14 Chchlee
 * cleaned up
 * 
 * 11    29.06.05 13:36 Chchlee
 * 
 * 10    29.06.05 13:05 Chchlee
 * cleaned up
 * 
 * 9     29.06.05 8:13 Chchlee
 * Cleaned up, bool removed
 * 
 * 8     28.06.05 13:53 Chchlee
 * ASSERTs consolidated
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include "basetype.h"
#include "diag.h"
#include <def21161.h>
asm("#include <def21161.h>");
#include <math.h>
#include "ExternalTDM_BusBridge.h"
#include "ExternalTDM_BusLookupTables.h"
#include "tdm_int_types.h"
#include "P4LT_Types.h"
#include "tdm_ext_types.h"
#include "FT12.h"
#include "configuration.h"
#include "boost_manager.h"
#include "TDM_Init.h"
#include "Monitor.h"
#include "O4CV_Const.h"

#include "limits.h"  // for ULONG_MAX


/* LOCAL TYPE DECLARATIONS ***************************************************/

// controls the soft timing struct for optimisation, accessed by the assembler funtion, DO NOT CHANGE
typedef struct 
{
	long	        SoftTimingBitsToSend[NO_TDM_BASIC_TIMESLOTS];	// [0]: Timeslot0 ;[1]: Timeslot1 ;[2]: Timeslot2 ;[3]: Timeslot3 ...
	long	        ActiveBasicTimeSlot;      // long offset: 10
	unsigned long	ReceivedSoftTimingBits;   // long offset: 11
} t_SoftTimingInformation;


typedef struct 
{
	unsigned long	O4LE_EM_WireMappingChannelInformation;
	BOOL	        EandM_WireIsActivated;
} t_EandM_WireMappingForBridge;


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

//****************************************************************************
//
// Monitoring defines for TDM Bus check, #repetitions until TDM Bus restart
//
//****************************************************************************

// Defines for TDM_BusStatusType, Number of repeated errors until restart of TDM Bus for P4LW
// These constants are for functions monitoring with 1 kHz
#define ALL_SLAVES_CHKSUM_ERROR_THRESHOLD		3000	         // 3s
#define SINGLE_SLAVE_CHKSUM_ERROR_THRESHOLD     2000             // 2s

#define TDM_REPEATED_TOTAL_COMMUNICATION_LOST_RETRIES_NEW_START		100

// Error status counter (bad fletcher checksum). Incremented in "ComputeRxBasicHardTiming" at 10.667 kHz
#define TDM_CHKSUM_ERROR_THRESHOLD								100  // 10ms

// Inhibit toggling of P4LW lifesign for 5s to allow for slave reset (clock 106 Hz)
#define LIFESIGN_SUPPRESSION_TIME	534						

// Runs wit real-time, after 0.5 s w/o SPORT02 interrupt, SPORT02 will be restarted  
#define EXT_TDM_DMA_MAX_FAIL_CNT			5000

#define BASIC_TIMESLOT_MASK (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9)
#define N_BASIC_TIMESLOTS	10	// number of basic time slots

// APLC channel limiter
#define APLC_CH_MAX_VAL		35.0

// O4CV defines
#define N_O4CV_EXTENDED32_TS			4	// number of 32-bit extended time slots (contains 2 16-bit TS)
#define O4CV_EXTENDED32_TS_OFFSET_RX	16	// first O4CV 32-bit extended time slot in receive DMA buffer
#define BASIC_TS_8_OFFSET				4	// offset of basic time slot #8				

/* GLOBAL VARIABLE DECLARATIONS ***********************************************/

// Variables and structs, used in ExternalTDM_BusLowLevel.asm 
t_TDM_HardTimingTelegramInformation HardTimingInformationFromTDM[NO_TDM_BASIC_TIMESLOTS];
t_TDM_HardTimingTelegramInformation HardTimingInformationToTDM  [NO_TDM_BASIC_TIMESLOTS];

// is also accessed by assembler code.
volatile t_SoftTimingInformation SoftTimingInformationStruct;

// External TDM Bus DMA buffer pointer - are changed in the interrupt routine
extern volatile long *externalTDM_Rx_Ptr;
extern volatile long *externalTDM_Tx_Ptr;
// Internal TDM Bus DMA buffer pointer - are changed in the interrupt routine
extern volatile long *internalTDM_Rx_Ptr;
extern volatile long *internalTDM_Tx_Ptr;

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static long g_O4LE_APLC_ChannelMapping[MAX_N_O4LE_BOARDS][N_O4LE_CHANNELS]; // supports maximum of MAX_N_O4LE_BOARDS in system, each with channel A & B 
static t_EandM_WireMappingForBridge g_O4LE_EM_WireMapping[N_APLC_CHANNELS];	// max. 3 APLC channels with own pilot signal

//static t_TDMTimeslotList g_Unused_BasicTimeslotList;
static t_TDMTimeslotList g_O4LE_BasicTimeslotList;
static t_TDMTimeslotList g_O4LE_ExtendedTimeslotList;

static t_TDMTimeslotList g_O4CV_BasicTimeslotList;
//static t_TDMTimeslotList g_O4CV_ExtendedTimeslotList;

static t_TDMTimeslotList g_G4AI_BasicTimeslotList;

static long g_ConfiguredBasicTimeslotArr[NO_TDM_BASIC_TIMESLOTS];

static unsigned long g_O4CVBoardNumToBasicTSNum[N_O4CV_BOARDS];	
// basicTimeslot = g_O4CVBoardNumToBasicTSNum[boardNumber - 1]

static long g_O4CVExtended32TsOffsetTx;	// first O4CV 32-bit extended time slot in tx DMA buffer (depends on configuration)

// Supervision stuff
static t_AlarmsAndWarnings  * g_pAlarmsAndWarnings;

// SPORT supervision for external TDM bus
static struct 
{
	unsigned long DMA_Tx_NotRunning;
	unsigned long DMA_Rx_NotRunning;
	unsigned long RepeatedChecksumErrorsConfiguredTDM_Cards[NO_TDM_BASIC_TIMESLOTS];
	unsigned long RepeatedChecksumErrorsOverAllConfiguredCards;
	unsigned long RepeatedSlotID_ErrorsOverAllConfiguredCards;
	unsigned long TimeSlotNumberMapping[NO_TDM_BASIC_TIMESLOTS];
	unsigned long nConfiguredSlaveCards;
} g_TDM_BusStatus;

// During startup reset all slave cards after booting as in ETL500.
// According to Sharc datasheet of slave boards, the SPORT must be initialized AFTER frame sync and clock sync
// is available.Thus we reset the slave boards deliberately again after their startup
static struct
{
	enum
	{
		Idle, 
		ResetSingleSlaveCard, 
		ResetAllSlaveCards
	} 
	state;
	unsigned long slaveCardTimeSlotNumber;

} g_TDM_BusResetInformation = {ResetAllSlaveCards, 0};

static t_FT12TelegramBuffer ** g_pReceiveBuffer;
static t_FT12TelegramBuffer ** g_pTransmitBuffer; // pointer to pointer to TDM card send buffer
static e_FT12State          *  g_pState;


/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx and tx pointers
#define g_intTDM_RxStruct_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr)
#define g_intTDM_TxStruct_Ptr  ((t_Master_output_tdm_block*)internalTDM_Tx_Ptr)


/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void AddSoftTimingBitToExternalTDM(void);
static void ExtractSoftTimingInformationFromExternalTDM(void);
static void Init_ExternalTDMBusSupervision(void);
static void SystemResetHandlerForSlaves(void);

static unsigned long getO4CVBoardNumber(unsigned long boardInfo);


/* EXTERNAL FUNCTION DEFINITIONS ************************************************/

// Assembler functions from ExternalTDM_BusLowLevel.asm 
extern void ComputeRxBasicHardTiming(volatile long *ActiveTDM_DMA_Ptr);
extern void GenerateTxBasicHardTiming(volatile long *ActiveTDM_DMA_Ptr); 

///////////////////////////////////////////////////////////////////////////////
/////////////////////  TDM Initialisation routines  //////////////////////////


void SEGMENT_EXT_CODE intExtTDMBusBridge_initMapping(/*out*/unsigned long * tx_channel_mask_0)
{
	int basicTimeSlot, aplcChannel, ix;

	boostManager_init (HardTimingInformationFromTDM, 
		               HardTimingInformationToTDM);

	// Inital tx_channel_mask value: activate all 32 16-bit time slots.
	// The G4AI extended time slots, which are published by the NSD DSP will be de-activated later in this function
	
	// Deactivate all channels except the basic TS!!!!!
	*tx_channel_mask_0 = BASIC_TIMESLOT_MASK;
	t_P4LT_Config * p_P4LT_Config = configuration_getPCurrentConfig(); // get a ptr to the current configuration
	
	// get a pointer to the undelayed low level alarms for slow mute
	g_pAlarmsAndWarnings  = monitor_get_pAlarmsAndWarnings();

	g_O4LE_BasicTimeslotList.nEntries = g_O4LE_ExtendedTimeslotList.nEntries = 0;

	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		if ((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK) == O4LE_BOARD)
		{
			ASSERT(g_O4LE_BasicTimeslotList.nEntries <= MAX_N_O4LE_BOARDS); //make sure, configuration does not contain more than MAX_N_O4LE_BOARDS O4LE boards

	  		// O4LE channel A

			// note that in boardInfo the APLC channels are defined as follows
			//  Values: 0 = channel is not used							 
			//          1 = channel is mapped to APLC channel 1
			//          2 = channel is mapped to APLC channel 2
			//          3 = channel is mapped to APLC channel 3

			aplcChannel = ((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo >> 8) & MASK_4LSB) - 1;

			if (-1 == aplcChannel) // not configured
			{
				aplcChannel = APLC_CH_NONE;
			}
			
			g_O4LE_APLC_ChannelMapping[g_O4LE_BasicTimeslotList.nEntries][O4LE_CH_A] = aplcChannel;

			// O4LE channel B
			aplcChannel = ((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo >> 12) & MASK_4LSB) - 1;

			if (-1 == aplcChannel) // not configured
			{
				aplcChannel = APLC_CH_NONE;
			}
			
			g_O4LE_APLC_ChannelMapping[g_O4LE_BasicTimeslotList.nEntries][O4LE_CH_B] = aplcChannel;

			g_O4LE_BasicTimeslotList.TimeslotNumber[g_O4LE_BasicTimeslotList.nEntries] = basicTimeSlot;

			{ // find the current extended time slot number of this board
				
				unsigned int bitPosition;

    			// start searching for extended time slots after basic time slots 
				for (bitPosition = NO_TDM_BASIC_TIMESLOTS; bitPosition < NR_OF_BITS_IN_LONG; bitPosition++)
				{
					if ( p_P4LT_Config->system.TDM_Config[basicTimeSlot].bitMask & (BIT0 << bitPosition))
					{
						// bit is set -> get the slot number (as compressed 32-bit slot index)
						g_O4LE_ExtendedTimeslotList.TimeslotNumber[g_O4LE_BasicTimeslotList.nEntries] = bitPosition / 2;
						// activate the extended time slots for each used O4LE
						*tx_channel_mask_0 |= (p_P4LT_Config->system.TDM_Config[basicTimeSlot].bitMask & (~BASIC_TIMESLOT_MASK));
						break;
					}
				}
			}

			// one basic, one extended time slot per O4LE
			g_O4LE_BasicTimeslotList.nEntries++; g_O4LE_ExtendedTimeslotList.nEntries++;
		}
	}

	g_O4CV_BasicTimeslotList.nEntries = 0;
	g_G4AI_BasicTimeslotList.nEntries = 0;

	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		// find O4CV basic time slots
		if (((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo) & BOARD_INFO_KIND_OF_BOARD_MASK) == O4CV_BOARD)
		{
			unsigned long boardNumber;

			g_O4CV_BasicTimeslotList.TimeslotNumber[g_O4CV_BasicTimeslotList.nEntries++] = basicTimeSlot;
			
			boardNumber = getO4CVBoardNumber(p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo);
			if ((boardNumber >= 1) && (boardNumber <= N_O4CV_BOARDS))
			{
				g_O4CVBoardNumToBasicTSNum[boardNumber - 1] = basicTimeSlot;
			}
		}

		// find G4AI basic time slots
		if (((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo) & BOARD_INFO_KIND_OF_BOARD_MASK) == G4AI_BOARD)
		{
			g_G4AI_BasicTimeslotList.TimeslotNumber[g_G4AI_BasicTimeslotList.nEntries++] = basicTimeSlot;
		}
	}

	// calculate O4CV extended 32-bit time slot offset 
	g_O4CVExtended32TsOffsetTx = N_BASIC_TIMESLOTS/2 + 2*g_O4LE_BasicTimeslotList.nEntries;

	// Extract telephony information for E and M wire routing
	for (aplcChannel = APLC_CH1; aplcChannel <= APLC_CH3; aplcChannel++)
	{
		if (-1 != p_P4LT_Config->system.serviceData.TelephonySlotID[aplcChannel]) // is telephony configured
		{
			g_O4LE_EM_WireMapping[aplcChannel].EandM_WireIsActivated = TRUE;
			g_O4LE_EM_WireMapping[aplcChannel].O4LE_EM_WireMappingChannelInformation = p_P4LT_Config->system.serviceData.TelephonySlotID[aplcChannel]; 
		}
	}

	// Extract all configured basic time slot information for alarm monitoring purposes
	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		// if a board is plugged in
		if (p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK)
		{
			g_ConfiguredBasicTimeslotArr[basicTimeSlot] = TRUE;
		}
	}

	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		HardTimingInformationToTDM[basicTimeSlot].SlotID = basicTimeSlot;
	}

	// Init supervision stuff
	memset(       g_TDM_BusStatus.TimeSlotNumberMapping,  0, 
		   sizeof(g_TDM_BusStatus.TimeSlotNumberMapping));

	// initialize statix part of g_TDM_BusStatus structure
	g_TDM_BusStatus.nConfiguredSlaveCards = 0;

	// For all O4LEs do
	for (ix = 0; ix < g_O4LE_BasicTimeslotList.nEntries; ix++, g_TDM_BusStatus.nConfiguredSlaveCards++)
	{
		g_TDM_BusStatus.TimeSlotNumberMapping[g_TDM_BusStatus.nConfiguredSlaveCards] = g_O4LE_BasicTimeslotList.TimeslotNumber[ix];
	}

	// For all G4AIs do
	for (ix = 0; ix < g_G4AI_BasicTimeslotList.nEntries; ix++, g_TDM_BusStatus.nConfiguredSlaveCards++)
	{
		g_TDM_BusStatus.TimeSlotNumberMapping[g_TDM_BusStatus.nConfiguredSlaveCards] = g_G4AI_BasicTimeslotList.TimeslotNumber[ix];
	}

	// For all O4CVs do
	for (ix = 0; ix < g_O4CV_BasicTimeslotList.nEntries; ix++, g_TDM_BusStatus.nConfiguredSlaveCards++)
	{
		g_TDM_BusStatus.TimeSlotNumberMapping[g_TDM_BusStatus.nConfiguredSlaveCards] = g_O4CV_BasicTimeslotList.TimeslotNumber[ix];
	}


	Init_ExternalTDMBusSupervision();
}

t_TDM_HardTimingTelegramInformation SEGMENT_EXT_CODE * extTDMBusBridge_getPHardTimingTelegramInfoFromTDM(void)
{
	return HardTimingInformationFromTDM;
}

t_TDM_HardTimingTelegramInformation SEGMENT_EXT_CODE * extTDMBusBridge_getPHardTimingTelegramInfoToTDM(void)
{
	return HardTimingInformationToTDM;
}


void SEGMENT_EXT_CODE intExtTDMBusBridge_init(
						  t_FT12TelegramBuffer ** receiveBuffer,    //[in]
		  				  t_FT12TelegramBuffer ** transmitBuffer,   //[in]
						  e_FT12State          *  pState)           //[in]
{
	g_pReceiveBuffer  = receiveBuffer;  // telegram to be forwarded
	g_pTransmitBuffer = transmitBuffer; // save pointer to pointer to tx buffer
    g_pState          = pState;
}


static unsigned long SEGMENT_EXT_CODE getO4CVBoardNumber(unsigned long boardInfo)
{
	unsigned long boardNumber;

	boardNumber = (boardInfo & BOARD_INFO_O4CV_BOARD_NUMBER_MASK);
	boardNumber = boardNumber >> GET_SET_LSB(BOARD_INFO_O4CV_BOARD_NUMBER_MASK);
	
	return(boardNumber);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////// Extended time slot routines //////////////////////////

static void ToO4LE_ExternalTDM_Bridge(void)
{
	int   ix;
	float APLC_RxSamples[N_APLC_CHANNELS];

	APLC_RxSamples[APLC_CH1] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[APLC_CH1];
	APLC_RxSamples[APLC_CH2] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[APLC_CH2];
	APLC_RxSamples[APLC_CH3] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[APLC_CH3];

	// Do a floatCheck on all samples received from FE on internal TDM bus
	ArrayFloatCheck(APLC_RxSamples, sizeof(APLC_RxSamples));

	for (ix = 0; ix < g_O4LE_ExtendedTimeslotList.nEntries; ix++)
	{
		long * Offset = (long*) (g_O4LE_ExtendedTimeslotList.TimeslotNumber[ix] + externalTDM_Tx_Ptr);

		int ChannelMapping = g_O4LE_APLC_ChannelMapping[ix][O4LE_CH_A];
		
		if (ChannelMapping == APLC_CH_NONE) 
		{
			*((float*) (Offset)) = 0.0f;
		}
		else
		{  
			*((float*) (Offset)) = APLC_RxSamples[ChannelMapping];	        // Ch A on O4LE
		}
		
		ChannelMapping = g_O4LE_APLC_ChannelMapping[ix][O4LE_CH_B];
		
		if (ChannelMapping == APLC_CH_NONE)
		{
			*((float*) (Offset + 1)) = 0.0f;
		}
		else
		{    
			*((float*) (Offset + 1)) = APLC_RxSamples[ChannelMapping];		 // Ch B on O4LE
		}
	}
}


static void FromO4LE_ExternalTDM_Bridge(void)
{
	float MuxedAPLC_TxSamples[N_APLC_CHANNELS] = {0.0f, 0.0f, 0.0f};
	int ix;

	for (ix = 0; ix < g_O4LE_ExtendedTimeslotList.nEntries; ix++)
	{
		// Extract an check channel 1 data
		long * Offset = (long*) (g_O4LE_ExtendedTimeslotList.TimeslotNumber[ix] + externalTDM_Rx_Ptr);

		float ReceivedTempFloat = *((float*) (Offset));
		int ChannelMapping = g_O4LE_APLC_ChannelMapping[ix][O4LE_CH_A];

		if (ChannelMapping != APLC_CH_NONE) 
		{
			MuxedAPLC_TxSamples[ChannelMapping] += ReceivedTempFloat;
		}
		
		ReceivedTempFloat = *((float*) (Offset + 1));
		ChannelMapping = g_O4LE_APLC_ChannelMapping[ix][O4LE_CH_B];
		
		if (ChannelMapping != APLC_CH_NONE) 
		{
			MuxedAPLC_TxSamples[ChannelMapping] += ReceivedTempFloat;
		}
	}

	// Do a floatCheck on all samples received from O4LEs on external TDM bus
	ArrayFloatCheck(MuxedAPLC_TxSamples, sizeof(MuxedAPLC_TxSamples));

	// APLC channel limiter
	if (fabsf(MuxedAPLC_TxSamples[APLC_CH1]) > APLC_CH_MAX_VAL)
	{
		MuxedAPLC_TxSamples[APLC_CH1] = 0.0;
	}

	if (fabsf(MuxedAPLC_TxSamples[APLC_CH2]) > APLC_CH_MAX_VAL)
	{
		MuxedAPLC_TxSamples[APLC_CH2] = 0.0;
	}

	if (fabsf(MuxedAPLC_TxSamples[APLC_CH3]) > APLC_CH_MAX_VAL)
	{
		MuxedAPLC_TxSamples[APLC_CH3] = 0.0;
	}

	// Write samples out on internal TDM bus
	g_intTDM_TxStruct_Ptr->aplc_tx[APLC_CH1] = MuxedAPLC_TxSamples[APLC_CH1];	// APLC Channel 1
	g_intTDM_TxStruct_Ptr->aplc_tx[APLC_CH2] = MuxedAPLC_TxSamples[APLC_CH2];	// APLC Channel 2
	g_intTDM_TxStruct_Ptr->aplc_tx[APLC_CH3] = MuxedAPLC_TxSamples[APLC_CH3];	// APLC Channel 3

}

static void ToO4CV_ExternalTDM_Bridge(void)
{
	unsigned long *pExtendedTS;
	unsigned long *pInternalTS;
	int ix;


	pExtendedTS = (unsigned long*)(externalTDM_Tx_Ptr + g_O4CVExtended32TsOffsetTx);
	pInternalTS = &g_intTDM_RxStruct_Ptr->O4CV_RxDataCh1_4_TS70;

	// copy data from internal to external TDM bus
	for(ix = 0; ix < g_O4CV_BasicTimeslotList.nEntries; ix++)
	{
		*(pExtendedTS++) = *(pInternalTS++);
	}

	// workaround: basic time slots 8 and 9 are used for O4CV channels 13-16
	*(externalTDM_Tx_Ptr + BASIC_TS_8_OFFSET) = g_intTDM_RxStruct_Ptr->O4CV_RxDataCh13_16_TS73;

}

static void setO4CVSubframeStartFlags(void)
{
	static int ix;
	static unsigned long bitMask;

	// set bit mask for subframe start flag of O4CV board 1
	bitMask = BIT0;

	// set O4CV subframe start bits on basic time slot
	for (ix = 0; ix < g_O4CV_BasicTimeslotList.nEntries; ix++) 
	{
		if (g_intTDM_RxStruct_Ptr->O4CV_RxSubframeStartFlags_TS69 & bitMask)
		{
			HardTimingInformationToTDM[g_O4CVBoardNumToBasicTSNum[ix]].Additional_7Bit |= BASIC_O4CV_SUBFRAME_START_BIT;
		}
		else
		{
			HardTimingInformationToTDM[g_O4CVBoardNumToBasicTSNum[ix]].Additional_7Bit &= (~BASIC_O4CV_SUBFRAME_START_BIT);
		}

		bitMask = bitMask << 1;		// shift mask to next board
	}
}

static void FromO4CV_ExternalTDM_Bridge(void)
{
	unsigned long *pExtendedTS;
	unsigned long *pInternalTS;
	int ix;


	pExtendedTS = (unsigned long*)(externalTDM_Rx_Ptr + O4CV_EXTENDED32_TS_OFFSET_RX);
	pInternalTS = &g_intTDM_TxStruct_Ptr->O4CV_TxDataCh1_4;

	// copy data from external to internal TDM bus
	for(ix = 0; ix < g_O4CV_BasicTimeslotList.nEntries; ix++)
	{
		*(pInternalTS++) = *(pExtendedTS++);
	}

	// workaround: basic time slots 8 and 9 are used for O4CV channels 13-16
	g_intTDM_TxStruct_Ptr->O4CV_TxDataCh13_16 = *(externalTDM_Rx_Ptr + BASIC_TS_8_OFFSET);

}

static void getO4CVSubframeStartFlags(void)
{
	static int ix;
	static unsigned long bitMask;

	// set bit mask for subframe start flag of O4CV board 1
	bitMask = BIT0;

	// get O4CV subframe start bits from basic time slot and write it to internal TDM bus
	for (ix = 0; ix < g_O4CV_BasicTimeslotList.nEntries; ix++) 
	{
		if (HardTimingInformationFromTDM[g_O4CVBoardNumToBasicTSNum[ix]].Additional_7Bit & BASIC_O4CV_SUBFRAME_START_BIT)
		{
			g_intTDM_TxStruct_Ptr->O4CV_TxSubframeStartFlags |= bitMask;
		}
		else
		{
			g_intTDM_TxStruct_Ptr->O4CV_TxSubframeStartFlags &= (~bitMask);
		}

		bitMask = bitMask << 1;		// shift mask to next board
	}
}

void sendCurrentDataRateToO4CV(void)
{
	unsigned long currentDatarateIx;
	int ix;


	// get current data rate index from OFDM DSP
	currentDatarateIx = g_intTDM_RxStruct_Ptr->OFDM_DSP_AlarmStatusData_TS16.currentDatarateIx;
	currentDatarateIx <<= 1;

	// send current data rate index to all O4CVs
	for(ix = 0; ix < g_O4CV_BasicTimeslotList.nEntries; ix++)
	{	
		// clear old value
		HardTimingInformationToTDM[g_O4CV_BasicTimeslotList.TimeslotNumber[ix]].Additional_7Bit &= ~(CURRENT_DATA_RATE_IX_MASK);
		// set new value
		HardTimingInformationToTDM[g_O4CV_BasicTimeslotList.TimeslotNumber[ix]].Additional_7Bit	|= 	currentDatarateIx;		
	}
}

static void AddSoftTimingBitToExternalTDM(void)
{
	static unsigned long WordsToBeTransmitted;
	static unsigned long TransmittedBits;
	static unsigned long TransmittedWords;

	static BOOL softTransmitterIsBusy;

	static unsigned long SoftTimingArrayOffset;

    // State machine for sending soft timing information to any TDM card

	if (!softTransmitterIsBusy)
	{
		// Soft transmitter is idle. Check if something to do...
		if (e_FT12State_Forwarded == *g_pState)				        	// New telegramm ready to transmit
		{                                 
			(*g_pReceiveBuffer)->State = *g_pState = e_FT12State_Sending;
			softTransmitterIsBusy = TRUE;

			WordsToBeTransmitted = 
				((((*g_pReceiveBuffer)->TelegrammDataArr[0] >> 16) & MASK_8LSB) / NR_OF_BYTES_IN_LONG) + 3;  // Calculate complete telegramm length as 32 Bit words
			
			TransmittedBits = TransmittedWords = 0;  // Set counters back
			
			// Adressed to local equipment
			if ((*g_pReceiveBuffer)->DestinationSubadress < 11)
			{
				// Set basic time slot from Destination Subadress
				SoftTimingInformationStruct.ActiveBasicTimeSlot = (*g_pReceiveBuffer)->DestinationSubadress - 1;

				if (SoftTimingInformationStruct.ActiveBasicTimeSlot < 0) 
					SoftTimingInformationStruct.ActiveBasicTimeSlot = 0;
			}
			// Request came from remote equipment accessing local equipment
			else
			{
				if (((*g_pReceiveBuffer)->DestinationSubadress > 64) &&
				    ((*g_pReceiveBuffer)->DestinationSubadress < 75)	)
					{
						SoftTimingInformationStruct.ActiveBasicTimeSlot = (*g_pReceiveBuffer)->DestinationSubadress - 65;
					}
			}
			SoftTimingArrayOffset = SoftTimingInformationStruct.ActiveBasicTimeSlot;
		}
	}
	else
	{
		if (TransmittedWords < WordsToBeTransmitted)
		{	
			// Prepare bit to be transmitted
			SoftTimingInformationStruct.SoftTimingBitsToSend[SoftTimingArrayOffset] = 
				((*g_pReceiveBuffer)->TelegrammDataArr[TransmittedWords] >> TransmittedBits) & BIT0;

			if (++TransmittedBits >= NR_OF_BITS_IN_LONG)	// Word complete transmitted ?
			{
				TransmittedBits = 0; // Increment word counter and set back bit counter
				TransmittedWords++;
			}
		}
		else
		{
			(*g_pReceiveBuffer)->State = e_FT12State_Empty;
			*g_pState                  = e_FT12State_Transmitted; 
			softTransmitterIsBusy = FALSE;
		}
	}
}


void ExtractSoftTimingInformationFromExternalTDM(void)
{
	static enum 
	{
		e_SoftReceiver_Idle,
		e_SoftReceiver_Receiving,
		e_SoftReceiver_CopyHeaderToStruct,
		e_SoftReceiver_AwaitTimeout

	}
	state = e_SoftReceiver_Idle;

	// Local variables for soft timing reception and internal state machine
	static volatile long	TelegrammRX_Checksum;
	static unsigned long	ActualNumberOfReceivedSoftTimingBits;
	static unsigned long	ActualNumberOfReceivedSoftTimingWords;
	static unsigned long	ReceivedTelegrammLengthFromHeader;
	static unsigned long	TimeoutCounterRx;
	
	switch (state)
	{
		case e_SoftReceiver_Idle:		// No telegramm reception is active

			if ((SoftTimingInformationStruct.ReceivedSoftTimingBits & MASK_MESSAGE_START) == MESSAGE_START)		// Telegramm header received
			{
				ReceivedTelegrammLengthFromHeader = (SoftTimingInformationStruct.ReceivedSoftTimingBits >> 8) & MASK_8LSB;

				if (ReceivedTelegrammLengthFromHeader == ((SoftTimingInformationStruct.ReceivedSoftTimingBits >> 16) & MASK_8LSB))
				{

					(*g_pTransmitBuffer)->TelegrammDataArr[0] = SoftTimingInformationStruct.ReceivedSoftTimingBits << 8;
					
					ActualNumberOfReceivedSoftTimingBits = 8;	// Received first valid word;
					
					ActualNumberOfReceivedSoftTimingWords = 1;	// First word received
					
					TelegrammRX_Checksum = -104;
					
					ReceivedTelegrammLengthFromHeader = ((ReceivedTelegrammLengthFromHeader >> 2) & MASK_8LSB) + 2;

					state = e_SoftReceiver_Receiving;
				}
				else		// Wrong length in telegramm header
				{
					TimeoutCounterRx = 0;

					state = e_SoftReceiver_AwaitTimeout;
				}
			}
			break;

		case e_SoftReceiver_Receiving:

			if (++ActualNumberOfReceivedSoftTimingBits >= NR_OF_BITS_IN_LONG)		// One word more received?
			{
				(*g_pTransmitBuffer)->TelegrammDataArr[ActualNumberOfReceivedSoftTimingWords] = 
															SoftTimingInformationStruct.ReceivedSoftTimingBits;

				if (ActualNumberOfReceivedSoftTimingWords >= ReceivedTelegrammLengthFromHeader)
				{
					if ( ((SoftTimingInformationStruct.ReceivedSoftTimingBits & 0x0000ff00) == 0x00001600)  && 
						 ((SoftTimingInformationStruct.ReceivedSoftTimingBits & MASK_8LSB) == (TelegrammRX_Checksum & MASK_8LSB)) )
					{
						state = e_SoftReceiver_CopyHeaderToStruct;
					}
					else
					{
						TimeoutCounterRx = 0;
						state = e_SoftReceiver_AwaitTimeout;
					}
				}
				else
				{
					ActualNumberOfReceivedSoftTimingBits = 0;

					(*g_pTransmitBuffer)->TelegrammDataArr[ActualNumberOfReceivedSoftTimingWords] = 
														SoftTimingInformationStruct.ReceivedSoftTimingBits;
					
					ActualNumberOfReceivedSoftTimingWords++;
					
					TelegrammRX_Checksum +=  (SoftTimingInformationStruct.ReceivedSoftTimingBits        & MASK_8LSB) +
											((SoftTimingInformationStruct.ReceivedSoftTimingBits >> 8)  & MASK_8LSB) +
											((SoftTimingInformationStruct.ReceivedSoftTimingBits >> 16) & MASK_8LSB) +
											((SoftTimingInformationStruct.ReceivedSoftTimingBits >> 24) & MASK_8LSB);
				}
			}
			break;

		case e_SoftReceiver_CopyHeaderToStruct:

			
			TimeoutCounterRx = 0;
			
			(*g_pTransmitBuffer)->ControlField = ((*g_pTransmitBuffer)->TelegrammDataArr[1] >> 8) & MASK_8LSB;
			(*g_pTransmitBuffer)->DestinationSubadress = ((*g_pTransmitBuffer)->TelegrammDataArr[1] >> 16) & MASK_8LSB;
			(*g_pTransmitBuffer)->MessageType = (((*g_pTransmitBuffer)->TelegrammDataArr[1] >> 24) & MASK_8LSB) |
										        (((*g_pTransmitBuffer)->TelegrammDataArr[2] << 8) & 0x0000ff00);
			(*g_pTransmitBuffer)->DestinationAdress = ((*g_pTransmitBuffer)->TelegrammDataArr[2] >> 8) & MASK_16LSB;
			(*g_pTransmitBuffer)->SourceAdress = (((*g_pTransmitBuffer)->TelegrammDataArr[2] >> 24) & MASK_8LSB)|
									         	 (((*g_pTransmitBuffer)->TelegrammDataArr[3] << 8) & 0x0000ff00);
			(*g_pTransmitBuffer)->HopCounter = ((*g_pTransmitBuffer)->TelegrammDataArr[3] >> 8) & MASK_8LSB;
			(*g_pTransmitBuffer)->SequenceNumber = ((*g_pTransmitBuffer)->TelegrammDataArr[3] >> 16) & MASK_16LSB;
			
			(*g_pTransmitBuffer)->State = e_FT12State_Full;

			state = e_SoftReceiver_AwaitTimeout;
			
			break;

		case e_SoftReceiver_AwaitTimeout:

			if (++TimeoutCounterRx > 10)
			{
				state = e_SoftReceiver_Idle;
			}
			break;

		default:
			ASSERT(FALSE); // invalid state
	} 
}

// WARNING !!!!!   WARNING !!!!!  WARNING !!!!!  WARNING !!!!!  WARNING !!!!!  WARNING !!!!!  WARNING !!!!!
//
// Problem: Data communication through the internal TDM Bus might be non deterministic if a funtion is not 
//          writing in real time into the TDM Tx buffer.
//
// The E/M Wire funtion is a non real time funtion. Routing of the E/M wire signal is usually handled in a subcase
// of the main() function, eg every 10th time. 
// Due to the nature of the TDM Tx buffers with the chained DMA mode, writing every 10th time into the TX buffer writes 
// always to the same address, the other chained DMA buffer is never written into.
// That means the receiving DSP gets different values alternating the old and the new value (flickering information).
// If the receiving DSP is also getting the information out in a subcase, following possibilities might be observed:
// a) Data communication is working, no anomalies
// b) Data communication is never working 
// c) Data communication is working with several seconds of delay
// d) Stopping and starting with the emulator might change behaviour a to c
//
// possible workaround: 
// A new struct containing all non real time variables to be send over the internal TDM bus. All sub case funtions 
// using the internal TDM bus for communication must use this new struct. Another real time function is copying this
// struct information to the DMA Tx buffer in the main case.


void E_and_M_WireBridge(void)
{
	//E-wire is read from internal TDM bus and written to external TDM bus
	//M-wire is read from external TDM bus and written to internal TDM bus

	t_AllDSP_input_tdm_block  *InternalTDM_ReceiveBufferPtr;
	t_Master_output_tdm_block *InternalTDM_TransmitBufferPtr;

	t_E_wire_data E_WireInformationFromInternalTDM_Bus;
	t_M_wire_data M_WireInformationToInternalTDM_Bus = {0,0,0,0};

	// Get pointers
	if (internalTDM_Rx_Ptr) // make sure pointer is valid
	{
		InternalTDM_ReceiveBufferPtr  = (t_AllDSP_input_tdm_block*)  internalTDM_Rx_Ptr; 
		InternalTDM_TransmitBufferPtr = (t_Master_output_tdm_block*) internalTDM_Tx_Ptr;

		E_WireInformationFromInternalTDM_Bus = InternalTDM_ReceiveBufferPtr->E_WireSignallingAllChannels_TS28;

		// Do channel APLC 1
		if (g_O4LE_EM_WireMapping[APLC_CH1].EandM_WireIsActivated)
		{
			if (E_WireInformationFromInternalTDM_Bus.channel1)
				 HardTimingInformationToTDM[g_O4LE_EM_WireMapping[APLC_CH1].O4LE_EM_WireMappingChannelInformation].Additional_7Bit |=  BASIC_O4LE_E_OR_M_WIRE_BIT;
			else HardTimingInformationToTDM[g_O4LE_EM_WireMapping[APLC_CH1].O4LE_EM_WireMappingChannelInformation].Additional_7Bit &= ~BASIC_O4LE_E_OR_M_WIRE_BIT;

			if ((HardTimingInformationFromTDM[g_O4LE_EM_WireMapping[APLC_CH1].O4LE_EM_WireMappingChannelInformation].Additional_7Bit) & BASIC_O4LE_E_OR_M_WIRE_BIT)
				M_WireInformationToInternalTDM_Bus.channel1 = TRUE;
			else
				M_WireInformationToInternalTDM_Bus.channel1 = FALSE;
		}

		// Do channel APLC 2
		if (g_O4LE_EM_WireMapping[APLC_CH2].EandM_WireIsActivated)
		{
			if (E_WireInformationFromInternalTDM_Bus.channel2)
				 HardTimingInformationToTDM[g_O4LE_EM_WireMapping[APLC_CH2].O4LE_EM_WireMappingChannelInformation].Additional_7Bit |=  BASIC_O4LE_E_OR_M_WIRE_BIT;
			else HardTimingInformationToTDM[g_O4LE_EM_WireMapping[APLC_CH2].O4LE_EM_WireMappingChannelInformation].Additional_7Bit &= ~BASIC_O4LE_E_OR_M_WIRE_BIT;
 
			if ((HardTimingInformationFromTDM[g_O4LE_EM_WireMapping[APLC_CH2].O4LE_EM_WireMappingChannelInformation].Additional_7Bit) & BASIC_O4LE_E_OR_M_WIRE_BIT)
				M_WireInformationToInternalTDM_Bus.channel2 = TRUE;
			else
				M_WireInformationToInternalTDM_Bus.channel2 = FALSE;
		}

		// Do channel APLC 3
		if (g_O4LE_EM_WireMapping[APLC_CH3].EandM_WireIsActivated)
		{
			if (E_WireInformationFromInternalTDM_Bus.channel3)
				  HardTimingInformationToTDM[g_O4LE_EM_WireMapping[APLC_CH3].O4LE_EM_WireMappingChannelInformation].Additional_7Bit |=  BASIC_O4LE_E_OR_M_WIRE_BIT;
			else  HardTimingInformationToTDM[g_O4LE_EM_WireMapping[APLC_CH3].O4LE_EM_WireMappingChannelInformation].Additional_7Bit &= ~BASIC_O4LE_E_OR_M_WIRE_BIT;
			
			if ((HardTimingInformationFromTDM[g_O4LE_EM_WireMapping[APLC_CH3].O4LE_EM_WireMappingChannelInformation].Additional_7Bit) & BASIC_O4LE_E_OR_M_WIRE_BIT)
				M_WireInformationToInternalTDM_Bus.channel3 = TRUE;
			else
				M_WireInformationToInternalTDM_Bus.channel3 = FALSE;
		}

		// Writing to the DMA Tx buffer in non real time causes problems
		// Workaround: call the same funtion in an odd and in an even subcase
		InternalTDM_TransmitBufferPtr->mwire_tx = M_WireInformationToInternalTDM_Bus;
	}
}


/// Pax-Blocking: blockiert den Telefonieausgang
/// bei Linkalarm, nicht Normalbetrieb, 
/// Neu: Wenn die Dummyload gesteckt ist, wird SlowMute nicht mehr aktiviert,
///      da sonst Widerspruch zu Commissioning Instruction 10-17-99, LK
//////////////////////////////////////////////////////////////	

void SEGMENT_EXT_CODE doSlowMuteBridge(void) 
{
	int O4LE_Counter;

	BOOL slowMuteInformation[N_APLC_CHANNELS];

	// build slowMuteInformation
	slowMuteInformation[APLC_CH1] = 
		   (g_pAlarmsAndWarnings->plcIfAlarmsAndWarnings & WARN_Not_in_normal_operational_mode) |
		   (g_pAlarmsAndWarnings->plcLinkAlarms          & (ALM_Slow_Mute_Ch1     |
		                                                    ALM_Rx_pilot_fail_Ch1 |
												            ALM_Pilot_level_Ch1   |
												            ALM_AGC_fail_Ch1      |
	                                                        ALM_AFC_fail            )           |
		   (g_pAlarmsAndWarnings->tpe1AlarmsAndWarnings  & (ALM_NSD_Rx_Low_Level         | 
															ALM_NSD_Rx_High_Level        |
															ALM_NSD_Rx_SNR               | 
															ALM_NSD_No_Trip_and_No_Guard ))         );

									
	slowMuteInformation[APLC_CH2] = 
		   (g_pAlarmsAndWarnings->plcIfAlarmsAndWarnings & WARN_Not_in_normal_operational_mode) |
		   (g_pAlarmsAndWarnings->plcLinkAlarms          & (ALM_Slow_Mute_Ch2     |
		                                                    ALM_Rx_pilot_fail_Ch2 |
												            ALM_Pilot_level_Ch2   |
												            ALM_AGC_fail_Ch2        )           |
		   (g_pAlarmsAndWarnings->tpe2AlarmsAndWarnings  & (ALM_NSD_Rx_Low_Level         | 
															ALM_NSD_Rx_High_Level        |
															ALM_NSD_Rx_SNR               | 
															ALM_NSD_No_Trip_and_No_Guard ))         );

	slowMuteInformation[APLC_CH3] = 
		   (g_pAlarmsAndWarnings->plcIfAlarmsAndWarnings & WARN_Not_in_normal_operational_mode) |
		   (g_pAlarmsAndWarnings->plcLinkAlarms          & (ALM_Slow_Mute_Ch3     |
		                                                    ALM_Rx_pilot_fail_Ch3 |
												            ALM_Pilot_level_Ch3   |
												            ALM_AGC_fail_Ch3        )            );

	
	for (O4LE_Counter = 0; O4LE_Counter < g_O4LE_BasicTimeslotList.nEntries; O4LE_Counter++)
	{
		unsigned long activeBasicTimeSlot = g_O4LE_BasicTimeslotList.TimeslotNumber[O4LE_Counter];

		// O4LE cross connect
		unsigned long aplcChannel = (g_O4LE_APLC_ChannelMapping[O4LE_Counter][O4LE_CH_A]);

		SET_BITS((aplcChannel != APLC_CH_NONE) && (slowMuteInformation[aplcChannel]),
			     HardTimingInformationToTDM[activeBasicTimeSlot].Additional_7Bit,
				 SET_SLOW_MUTE_CH1_ON_TDM);






		aplcChannel = (g_O4LE_APLC_ChannelMapping[O4LE_Counter][O4LE_CH_B]);
		
		SET_BITS((aplcChannel != APLC_CH_NONE) && (slowMuteInformation[aplcChannel]),
			     HardTimingInformationToTDM[activeBasicTimeSlot].Additional_7Bit,
				 SET_SLOW_MUTE_CH2_ON_TDM);
	}
}

/***************************************************************************
 * TDM Handler TX
 ***************************************************************************/

void ExternalTDM_HandlerTx(unsigned long SyncBitForNSD_DSP) // is executed with 10kHz
{
	static int ix;
	static long CounterSlow;

	for (ix = 0; ix < g_G4AI_BasicTimeslotList.nEntries; ix++)
	{
		if (SyncBitForNSD_DSP & BIT0)
		{
			HardTimingInformationToTDM[g_G4AI_BasicTimeslotList.TimeslotNumber[ix]].Additional_7Bit |= BASIC_G4AI_FRAME_ID_BIT;
		}
		else
		{
			HardTimingInformationToTDM[g_G4AI_BasicTimeslotList.TimeslotNumber[ix]].Additional_7Bit &= (~BASIC_G4AI_FRAME_ID_BIT);
		}
	}

	// Resethandler must be called with 100Hz cycle, not faster
	if (++CounterSlow >= 100)
	{
		CounterSlow = 0;
		// reset stuff for testing only
		SystemResetHandlerForSlaves();
	}

	setO4CVSubframeStartFlags(); // on basic time slot	

	if (externalTDM_Tx_Ptr)	// Check here if the pointer is valid, saves calculation time
	{
		GenerateTxBasicHardTiming(externalTDM_Tx_Ptr);

		// Send O4LE Data to TDM Bus
		ToO4LE_ExternalTDM_Bridge();
		
		// Send O4CV Data to TDM Bus
		ToO4CV_ExternalTDM_Bridge();

		// This fuction must be called after SendHardTimingO4LE() and SendHardTimingG4AI() !!!!!!!!!!!!!!
		AddSoftTimingBitToExternalTDM();
	}
	// Extract soft timing Information from all TDM Cards
	ExtractSoftTimingInformationFromExternalTDM();
}


/***************************************************************************
 * TDM Handler RX
 ***************************************************************************/

void ExternalTDM_HandlerRx(void)
{
	if (externalTDM_Rx_Ptr) // Check here if the pointer is valid, saves calculation time
	{
		ComputeRxBasicHardTiming(externalTDM_Rx_Ptr);// Serve all 7 Basic time slots

		getO4CVSubframeStartFlags();	// from basic time slot		

		FromO4LE_ExternalTDM_Bridge();	// Process O4LE Data and Mux CH1 and CH2 from different configured O4LE

		FromO4CV_ExternalTDM_Bridge();	// receive data from O4CV and send it to the mux on internal TDM bus

		//G4AI stuff is moved to TP DSP
	}
}



// Monitoring functions TDM Bus ---------------------------------------------------------------------------

static void SEGMENT_EXT_CODE Init_ExternalTDMBusSupervision()
{
	g_TDM_BusStatus.DMA_Rx_NotRunning = 0;
	g_TDM_BusStatus.DMA_Tx_NotRunning = 0;

	g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards = 0;
	g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards  = 0;

	memset(       g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards,  0, 
		   sizeof(g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards));
}

// Realtime functions
// Must be called with 10.666 KHz in the master main() loop, checks if the DMA is still working
// Please note that starting/stopping with the Emulator also increments this counter, so the 
// decision about restarting the SPORT has to be set somehow slow
void Check_SPORT_TDM_Bus(void)
{
	static long *LastRx_DMA_BufferPtr;
	static long *LastTx_DMA_BufferPtr;

	if (externalTDM_Rx_Ptr == LastRx_DMA_BufferPtr) g_TDM_BusStatus.DMA_Rx_NotRunning++;
	if (externalTDM_Tx_Ptr == LastTx_DMA_BufferPtr) g_TDM_BusStatus.DMA_Tx_NotRunning++;

	LastRx_DMA_BufferPtr = (long*) externalTDM_Rx_Ptr;
	LastTx_DMA_BufferPtr = (long*) externalTDM_Tx_Ptr;
}


// Monitoring functions with low priority.

// output of this function.
	//  g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards[]

	//  g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards
	//  g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards

// Those error counters are processed in CheckAndRestartExternalTDM_Bus()

void SEGMENT_EXT_CODE CheckExternalTDM_Bus(void) // executed with 1kHz 
{
	int ix;
	BOOL allConfiguredSlaveCardsChecksum_Failed = TRUE; // initially TRUE. Set to FALSE if at least one card reports "ok".
	BOOL allConfiguredSlaveCardsSlotID_Failed   = TRUE;

	// This part checks all !!CONFIGURED!! TDM SLAVE cards.

	// For all slave cards do
	//  increment or reset g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards if invalid fletcher checksum of slave card 

	for (ix = 0; ix < g_TDM_BusStatus.nConfiguredSlaveCards; ix++)
	{
		int basicTimeSlotNumber = g_TDM_BusStatus.TimeSlotNumberMapping[ix];

		// Check ErrorStatus (Fletcher checksum. chksumErrorCnt is incremented and reset by asm-code
		if (HardTimingInformationFromTDM[basicTimeSlotNumber].chksumErrorCnt > TDM_CHKSUM_ERROR_THRESHOLD) 
		{
			g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards[ix]++;
		}
		else 
		{
			g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards[ix] = 0;
			allConfiguredSlaveCardsChecksum_Failed = FALSE;
		}

		// Check Slot ID
		if ((HardTimingInformationFromTDM[basicTimeSlotNumber].SlotID == 
			 HardTimingInformationToTDM[basicTimeSlotNumber].SlotID))
		{
			allConfiguredSlaveCardsSlotID_Failed = FALSE;
		}
	}

	// Check for repeated Checksum and Slot ID Errors:
	//   increment or reset g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards
	//   increment or reset g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards

	if (allConfiguredSlaveCardsChecksum_Failed)
	{
		if (g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards < ULONG_MAX)
		{
			g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards++;
		}
	}
	else
	{
		g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards = 0;
	}

	if (allConfiguredSlaveCardsSlotID_Failed) 
	{
		if (g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards < ULONG_MAX)
		{
			g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards++;
		}
	}
	else
	{
		g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards = 0;
	}
}

// CAUTION: In case of a restart of external TDM bus, it will take more than a Master DSP Main() cycle !
// Timing of reinitialisation is also important - see timing interrupt desaster
void SEGMENT_EXT_CODE CheckAndRestartExternalTDM_Bus(t_P4LT_Status *P4LT_SystemStatus) // executed with 1kHz
{
	static unsigned long numberOfFailedAllSlaveResets;
	
	static enum 
	{
		Command_Idle, 
		Command_Reset
	}
	resetCommand = Command_Reset; // reset TDM slave card after starup by suppressing lifesign


	// Exit, if there is no slave TDM card configured
	if (g_TDM_BusStatus.nConfiguredSlaveCards == 0)
		return; 

	// There are no more SPORT02 Rx or Tx interrupts - Reinitialize SPORT02
	// This has really high priority - has to be done everytime
	if ((g_TDM_BusStatus.DMA_Rx_NotRunning > EXT_TDM_DMA_MAX_FAIL_CNT) ||
		(g_TDM_BusStatus.DMA_Tx_NotRunning > EXT_TDM_DMA_MAX_FAIL_CNT) ||
		(numberOfFailedAllSlaveResets > TDM_REPEATED_TOTAL_COMMUNICATION_LOST_RETRIES_NEW_START))
	{
      /*{ // {} temporary code for debugging @CL@

			unsigned long reason = 0;

			if (g_TDM_BusStatus.DMA_Rx_NotRunning > EXT_TDM_DMA_MAX_FAIL_CNT)
			{
				reason |= BIT0;
			}
			
			if (g_TDM_BusStatus.DMA_Tx_NotRunning > EXT_TDM_DMA_MAX_FAIL_CNT)
			{
				reason |= BIT1;
			}
			
			if (numberOfFailedAllSlaveResets > TDM_REPEATED_TOTAL_COMMUNICATION_LOST_RETRIES_NEW_START)
			{
				reason |= BIT2;
			}
			
			eventRec_recordEvent(ALARM_EVENT_DEBUG4, reason, FALSE);
		}*/

		Restart_SPORT02();
		
		Init_ExternalTDMBusSupervision();

		P4LT_SystemStatus->system.numberOfTDM_BusRestarts++;
		numberOfFailedAllSlaveResets = 0;  // reset counter
	}

	// Reset command state machine
	switch (resetCommand)
	{
		case Command_Reset: // There is a ongoing reset on a/all slave boards -> temporarily inhibit supervision
		{
			if (Idle == g_TDM_BusResetInformation.state)
			{
				Init_ExternalTDMBusSupervision();
				resetCommand = Command_Idle; // reset command state machine to idle
			}
		}
		break;

		case Command_Idle:	// no reset is in progress, continue with supervision
		{
			static unsigned long currentSlaveCard;

			// First check if all cards together should be reset:
			// There are TDM Slave cards configured, but no communication was possible for a long amount of time
			// The condition is "all cards report wrong slot ID **and** all cards report wrong checksum"

			if ((g_TDM_BusStatus.RepeatedSlotID_ErrorsOverAllConfiguredCards  > ALL_SLAVES_CHKSUM_ERROR_THRESHOLD) &&
				(g_TDM_BusStatus.RepeatedChecksumErrorsOverAllConfiguredCards > ALL_SLAVES_CHKSUM_ERROR_THRESHOLD))
			{
				// It does make sense to reset ALL slave cards in this case - it doesn't depend on the number of configured O4LE/G4AI
				resetCommand = Command_Reset;
				g_TDM_BusResetInformation.state = ResetAllSlaveCards;
				numberOfFailedAllSlaveResets++;

				//eventRec_recordEvent(ALARM_EVENT_DEBUG5, numberOfFailedAllSlaveResets, FALSE); // record for debugging
			}

			// Now just check every slave card for individual reset
			if (g_TDM_BusStatus.RepeatedChecksumErrorsConfiguredTDM_Cards[currentSlaveCard] > SINGLE_SLAVE_CHKSUM_ERROR_THRESHOLD)
			{
				if (Idle == g_TDM_BusResetInformation.state) // check if no "reset all" command was just issued
				{
					resetCommand = Command_Reset;
					g_TDM_BusResetInformation.state = ResetSingleSlaveCard;
					g_TDM_BusResetInformation.slaveCardTimeSlotNumber = g_TDM_BusStatus.TimeSlotNumberMapping[currentSlaveCard];
					
					//eventRec_recordEvent(ALARM_EVENT_DEBUG3, g_TDM_BusResetInformation.slaveCardTimeSlotNumber, FALSE);  // record for debugging
				}
				// else do nothing, wait until the pending reset is over
			}

			// switch next basic time slot
			if (++currentSlaveCard >= g_TDM_BusStatus.nConfiguredSlaveCards) 
			{
				currentSlaveCard = 0;
			}
		}
		break;

		default: ASSERT(FALSE);
	} // end of switch()
}


// For all time slots, a toggle bit in the Additional7_Bits of the basic time slot
// is toggled with low frequency. If there is toggeling detected during a specified amount of
// time on the slave, a reset would be activated with blocking the watchdog
// is executed with 100Hz

static void SEGMENT_EXT_CODE SystemResetHandlerForSlaves(void)
{
	// Reset all slave cards on external TDM bus, as during ETL600 startup sequence
	switch(g_TDM_BusResetInformation.state)	
	{
		static long lifeSignSuppressionCounter;
		static long currentTimeSlotNumberTx;

		case Idle:
			
			HardTimingInformationToTDM[currentTimeSlotNumberTx].Additional_7Bit ^= BIT0;

			if (++currentTimeSlotNumberTx >= NO_TDM_BASIC_TIMESLOTS) // next slot
			{
				currentTimeSlotNumberTx = 0;
			}
			break;

		case ResetSingleSlaveCard:	// Reset of a specific basic time slot

			lifeSignSuppressionCounter++;

			// toggle all slave board lifesign but the one to be reset
			if (g_TDM_BusResetInformation.slaveCardTimeSlotNumber != currentTimeSlotNumberTx)
			{
				// Toggle now
				HardTimingInformationToTDM[currentTimeSlotNumberTx].Additional_7Bit ^= BIT0;
			}
			else
			{
				if (lifeSignSuppressionCounter > LIFESIGN_SUPPRESSION_TIME)
				{
					g_TDM_BusResetInformation.state = Idle;
					lifeSignSuppressionCounter = 0;
				}
			}

			if (++currentTimeSlotNumberTx >= NO_TDM_BASIC_TIMESLOTS) // next slot
			{
				currentTimeSlotNumberTx = 0;
			}
			break;

		case ResetAllSlaveCards: // supress toggling of all lifesigns

			if (++lifeSignSuppressionCounter > LIFESIGN_SUPPRESSION_TIME)
			{
				g_TDM_BusResetInformation.state = Idle;
				lifeSignSuppressionCounter = 0;
			}
			break;

		default: ASSERT(FALSE);
	}
}


void receiveBasicTimeSlot6(unsigned long *basicTimeSlot)
{
	*basicTimeSlot = (*(externalTDM_Rx_Ptr + 3)) & 0x0000FFFF;
}





