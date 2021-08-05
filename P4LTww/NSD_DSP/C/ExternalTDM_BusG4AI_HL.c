/*-------------------------------------------------------------------------*
 * $Workfile: ExternalTDM_BusG4AI_HL.c $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :
 * Purpose      :
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/C/ExternalTDM_BusG4AI_HL.c $
 * 
 * 10    14.11.06 13:41 Chpeplu
 * Function MapExternalTDMBusConfigToTimeslotsG4AI() changed for custom
 * mode
 * 
 * 9     21.09.06 11:27 Chhamaa
 * AES modifications
 * 
 * 8     7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 8     18.07.06 18:41 Chhamaa
 * 
 * 4     6.07.05 13:48 Chchlee
 * 
 * 3     9.03.05 10:26 Chpeplu
 * New config used (UsedExtTimeSlots)
 * 
 * 2     3.03.05 14:21 Chpeplu
 * prepared for NewConfigNSD without custom table
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 34    13.01.05 16:35 Chharyt
 * Garbage removed
 * 
 * 33    11.01.05 16:33 Chharyt
 * function call SendDataToG4AI changed back
 * 
 * 32    11.01.05 14:10 Chharyt
 * function call SendDataToG4AI changed
 * 
 * 31    7.01.05 16:26 Chharyt
 * runs in first tests! (after bigger modifications) all garbage inside
 * 
 * 30    26.11.04 10:02 Chharyt
 * TDM_TO_G4AI_BIT_MASK adjusted/inserted otherwise no start, Guard,...
 * are possible.
 * 
 * 29    19.11.04 8:11 Chharyt
 * now synchron to master. All test variables now comments
 * 
 * 28    16.11.04 11:24 Chharyt
 * 
 * 27    12.11.04 8:54 Rytz01
 * Attention: Only preliminary version. Check in is necessary because of
 * login change and migration into AD
 * 
 * 26    5.10.04 15:07 Rytz01
 * only comments inserted
 * 
 * 25    5.10.04 14:16 Rytz01
 * div bugs removed. Checked in because it runs with different configs
 * (1-4 G4AI) but a lot of test code is still in!!!  :-))
 * 
 * 24    29.09.04 16:32 Rytz01
 * changed again!! Works at the moment with 1,2,3 and 4 G4AI with 2 O4LE
 * 
 * 23    29.09.04 13:52 Rytz01
 * Asserts removed! New test variables inserted for better tests 
 * 
 * 22    28.09.04 17:32 Rytz01
 * #include "DIAG.H" forgotten
 * 
 * 21    28.09.04 17:28 Rytz01
 * SendDataToG4AI and ReceiveDataFromG4AI changed. All theoretical cases
 * are programmed. Probably unused are protected by an assert!!
 * 
 * 20    28.09.04 11:51 Rytz01
 * SendDataToG4AI and ReceiveDataFromG4AI changed. Send and receive sync
 * when not synchronous. May more changes are necessary
 * 
 * 19    28.09.04 7:41 Rytz01
 * Fixed config garbage removed
 * 
 * 18    28.09.04 7:34 Rytz01
 * SendDataToG4AI and ReceiveDataFromG4AI changed. Offset corrected.
 * 
 * 17    21.09.04 17:32 Plueer01
 * function  SendDataToG4AI() data mapping changed CRC and Data and
 * Pointer
 * 
 * 16    21.09.04 13:18 Plueer01
 * In function SendDataToG4AI(long *DataToG4AI) sync with master DSP
 * changed
 * 
 * 15    21.09.04 10:22 Plueer01
 * Function MapExternalTDMBusConfigToTimeslotsG4AI changed for Tx Offset
 * 
 * 14    16.09.04 17:17 Plueer01
 * line G4AI_ExtendedTimeslotListTx.TimeslotNumber[((G4AI_ExtendedTimeslot
 * ListTx.nEntries-1)-boardIndex)] = boardIndex; corrected (  -1 added)
 * 
 * 13    16.09.04 7:43 Plueer01
 * Mapping of G4AI to NSD600 channels added
 * 
 * 12    9.09.04 18:32 Rytz01
 * G4ai external TimeSlot mapping improved bud not tested!!!!!
 * 
 * 11    30.08.04 17:07 Leeb02
 * 
 * 10    26.08.04 10:54 Plueer01
 * function MapExternalTDMBusConfigToTimeslotsG4AI() changed for correct
 * settings and function ReceivedDataFromG4AI corrected for false block
 * order
 * 
 * 9     25.08.04 15:39 Rytz01
 * 
 * 8     25.08.04 10:20 Rytz01
 * handling with comments inproved to get no warning
 * 
 * 7     25.08.04 10:15 Rytz01
 * #include "TDM_Init.h" inserted and instead of tx_channel_mask_0
 * tx_channel_mask02[0] direct used.
 * 
 * 6     24.08.04 13:02 Leeb02
 * MapExternalTDMBusConfigToTimeslotsG4AI: parameter added
 * 
 * 5     23.08.04 8:36 Leeb02
 * tx_channel_mask _0 enable inserted
 * 
 * 4     20.08.04 16:26 Plueer01
 * Function SendDataToG4AI() changed separation of ch1 and ch2 commands
 * added
 * 
 * 3     19.07.04 10:57 Plueer01
 * Use separate Inputword for N22 and N28 (DataFromG4AI)
 * 
 * 2     9.07.04 15:17 Bussli02
 * Communication with 2 G4AI, static words, tested  and working (N22 and
 * N28, AES FW)
 * 
 * 1     8.07.04 16:17 Bussli02
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include "basetype.h"
#include <def21161.h>
asm("#include <def21161.h>");
#include "ExternalTDM_BusG4AI_HL.h"
#include "CRC32_table.h"
#include "tdm_int_types.h"
#include "tdm_ext_types.h"

#include "nsd_dsp_types.h"


/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define BASIC_TIMESLOT_MASK (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8|BIT9)


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

// variables for TDM Bridge low level configuration
// During startup, those variables must be filled with data according to the config data 

static t_TDMTimeslotList G4AI_ExtendedTimeslotListRx;
static t_TDMTimeslotList G4AI_ExtendedTimeslotListTx;	// different bitmasks - different positions in DMA buffer

static long MapCh1ToExTimeSlot[NO_TDM_BASIC_TIMESLOTS];
static long MapCh2ToExTimeSlot[NO_TDM_BASIC_TIMESLOTS];

static long InputFromG4AI[NO_TDM_BASIC_TIMESLOTS];
static long OutputToG4AI[NO_TDM_BASIC_TIMESLOTS];

// for tests only
long TxCom1, TxCom2;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/
void MapRxCommandToG4AI(long *ptr_DataToG4AI);
void MapG4AI_InputToChannels(long *DataFromG4AI);

/* EXTERNAL VARIABLE DECLARATIONS ***********************************************/

// External TDM Bus DMA buffer pointer - are changed in the interrupt routine
// Runs with 5.333 kHz - so every DMA buffer is doubled
extern volatile long *externalTDM_Rx_Ptr[2];
extern volatile long *externalTDM_Tx_Ptr[2];
// Internal TDM Bus DMA buffer pointer - are changed in the interrupt routine
extern volatile long *internalTDM_Rx_Ptr[2];
extern volatile long *internalTDM_Tx_Ptr[2];

/* EXTERNAL FUNCTION DECLARATIONS ***********************************************/

// Assembler functions from ExternalTDM_BusLowLevel.asm 
extern long CalculateCRC32(long Value);		

///////////////////////////////////////////////////////////////////////////////
/////////////////////  TDM Initialisation routines  //////////////////////////

void SEGMENT_EXT_CODE MapExternalTDMBusConfigToTimeslotsG4AI(unsigned long *tx_channel_mask_0, t_NSD_DSP_Config *Config)
{
	int basicTimeSlot;
	unsigned long boardIndex = 0;
	long usedExtTimeSlotsCh1;
	long usedExtTimeSlotsCh2;


	// Inital tx_channel_mask value: all slots disabled.
	// The G4AI extended time slots, which are published by the NSD DSP will be activated later in this function
	*tx_channel_mask_0 = 0x00000000;

	G4AI_ExtendedTimeslotListRx.nEntries = 0;
	G4AI_ExtendedTimeslotListTx.nEntries = 0;

	// To generate the "G4AI_ExtendedTimeslotListTx.TimeslotNumber[x]" I must know how many G4AI are used.
	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		if (((Config->TDM_Config[basicTimeSlot].boardInfo) & BOARD_INFO_KIND_OF_BOARD_MASK) == G4AI_BOARD)
		{
			G4AI_ExtendedTimeslotListTx.nEntries++;
			G4AI_ExtendedTimeslotListRx.nEntries++;
		}
	}
	// @Pl_ext
	if ((Config->mode & INT_TPE_MODE_MASK)== INT_TELPROT_MODE_AES)
	{
		usedExtTimeSlotsCh1 = Config->AES.UsedExtTimeSlots;
		usedExtTimeSlotsCh2 = 0;
	}
	else
	{
		usedExtTimeSlotsCh1 = Config->NSD[0].UsedExtTimeSlots;
		usedExtTimeSlotsCh2 = Config->NSD[1].UsedExtTimeSlots;
	};

	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		if (((Config->TDM_Config[basicTimeSlot].boardInfo) & BOARD_INFO_KIND_OF_BOARD_MASK) == G4AI_BOARD)
		{			
			{ // find the current extended time slot number of this board
				unsigned int bitPosition;

				// start searching for extended time slots after basic time slots (0-9) 
				for (bitPosition = NO_TDM_BASIC_TIMESLOTS; bitPosition < NR_OF_BITS_IN_LONG; bitPosition++)
				{
					if ( Config->TDM_Config[basicTimeSlot].bitMask & (BIT0 << bitPosition))
					{
						// bit is set -> get the slot number (as compressed 32-bit slot index)
						// there is one 32-bit slot per G4AI
						//G4AI_ExtendedTimeslotListTx.TimeslotNumber[((G4AI_ExtendedTimeslotListTx.nEntries-1)-boardIndex)] = boardIndex;
						G4AI_ExtendedTimeslotListTx.TimeslotNumber[boardIndex] = G4AI_ExtendedTimeslotListTx.nEntries -(16-(bitPosition/2));
						G4AI_ExtendedTimeslotListRx.TimeslotNumber[boardIndex] = bitPosition / 2;
						//  Ch1 settings
						if(((usedExtTimeSlotsCh1>>basicTimeSlot) & BIT0) == BIT0)
						{
							// @Pl_ext
							if ((Config->mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES)
							{
								MapCh1ToExTimeSlot[boardIndex] = 0xFFFFFFFF;	// all time slots enabled
							}
							else
							{
								MapCh1ToExTimeSlot[boardIndex] = TDM_TO_G4AI_BIT_MASK;
							}
						}
						else
						{
								MapCh1ToExTimeSlot[boardIndex] = 0;
						}
						// Ch2 settings
						if(((usedExtTimeSlotsCh2>>basicTimeSlot) & BIT0) == BIT0)
						{
							MapCh2ToExTimeSlot[boardIndex] = TDM_TO_G4AI_BIT_MASK;
						}
						else
						{
								MapCh2ToExTimeSlot[boardIndex] = 0;
						}
						break;
					}
				}
			}
			// update the tx_channel_mask_0: activate the extended time slots
			*tx_channel_mask_0 |= (Config->TDM_Config[basicTimeSlot].bitMask & (~BASIC_TIMESLOT_MASK));
			boardIndex++;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////// Extended time slot routines //////////////////////////

void SendDataToG4AI(long *DataToG4AI)
{
	static long *Offset;
	static long TempIndex = 0;
	static long CRC32[NO_TDM_BASIC_TIMESLOTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static long DataToSendExtendedTimeSlot = 0;

	MapRxCommandToG4AI(DataToG4AI);
	
	TempIndex = 0;
	while (TempIndex < G4AI_ExtendedTimeslotListTx.nEntries)
	{
		// Data will be sampled now
		DataToSendExtendedTimeSlot = OutputToG4AI[TempIndex];
		// Calculate checksum to TP commands send to all G4AIs
		CRC32[TempIndex] = CalculateCRC32(OutputToG4AI[TempIndex]);

		Offset = (long*) (G4AI_ExtendedTimeslotListTx.TimeslotNumber[TempIndex] + externalTDM_Tx_Ptr[0]);
		*(Offset) = DataToSendExtendedTimeSlot;
		Offset = (long*) (G4AI_ExtendedTimeslotListTx.TimeslotNumber[TempIndex] + externalTDM_Tx_Ptr[1]);
		*(Offset) = CRC32[TempIndex];
		TempIndex++;
	}
}

void ReceiveDataFromG4AI(long *DataFromG4AI)
{
	long *Offset;
	static long TempIndex = 0;
	int i;
	static volatile long ReceivedCommandWord[NO_TDM_BASIC_TIMESLOTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static volatile long Received_CRC32 = 0;
	static long OffsetInDMA_Buffer = 0;

	// Testing only
	static long TPS_G4AI_ErrorCounter = 0;
	static long TPS_G4AI_BlockCounter = 0;

	// Set back all previous received commands 
	for(i=0;i<NO_TDM_BASIC_TIMESLOTS;i++)
	{
		InputFromG4AI[i] = 0;
	}

	TempIndex = 0;
	while (TempIndex < G4AI_ExtendedTimeslotListRx.nEntries)
	{
		// G4AI listens to the sync bit sent by the Master DSP in basic time slots.
		// G4AI sends extended time slot according to the received frame ID bit
		// Undocumented feature: Sync of incoming packets are also done by the counter
		// from the internal TDM bus
		OffsetInDMA_Buffer = G4AI_ExtendedTimeslotListRx.TimeslotNumber[TempIndex];
		// Testing only
		TPS_G4AI_BlockCounter++;

		// Received commands from G4AI in block order
		ReceivedCommandWord[TempIndex] = *(externalTDM_Rx_Ptr[0] + OffsetInDMA_Buffer);
		Received_CRC32 = *(externalTDM_Rx_Ptr[1] + OffsetInDMA_Buffer);
		
		// Check if CRC32 is correct
		if ((Received_CRC32 == CalculateCRC32(ReceivedCommandWord[TempIndex])))
		{
			InputFromG4AI[TempIndex] = ReceivedCommandWord[TempIndex];
		}
		else 
		{
			TPS_G4AI_ErrorCounter++;
		}
		TempIndex++;
	}

	MapG4AI_InputToChannels(DataFromG4AI);
}

void MapG4AI_InputToChannels(long *DataFromG4AI)
{
	int i;
	long TxCom_Ch1 = 0;
	long TxCom_Ch2 = 0;
	// reset Tx Command Ch1 and Ch2
	//*DataFromG4AI++ = 0;
	//*DataFromG4AI-- = 0;
	//
	for (i=0; i< G4AI_ExtendedTimeslotListRx.nEntries; i++)
	{
		TxCom_Ch1 |= ((InputFromG4AI[i])&MapCh1ToExTimeSlot[i]);
		TxCom_Ch2 |= ((InputFromG4AI[i])&MapCh2ToExTimeSlot[i]);
	}
	*DataFromG4AI++ = TxCom_Ch1;
	*DataFromG4AI = TxCom_Ch2;
	// for tests only
	TxCom1 = TxCom_Ch1;
	TxCom2 = TxCom_Ch2; 
}

void MapRxCommandToG4AI(long *ptr_DataToG4AI)
{
	int i;

	long RxComCh1, RxComCh2;

	RxComCh1 = *ptr_DataToG4AI++;
	RxComCh2 = *ptr_DataToG4AI; 

	for (i=0; i< G4AI_ExtendedTimeslotListTx.nEntries; i++)
	{
		OutputToG4AI[i] = 0;	// reset send data	
	}


	for (i=0; i< G4AI_ExtendedTimeslotListTx.nEntries; i++)
	{
		OutputToG4AI[i] |=  (((RxComCh1) & MapCh1ToExTimeSlot[i]) | ((RxComCh2) & MapCh2ToExTimeSlot[i]));
	}
}


