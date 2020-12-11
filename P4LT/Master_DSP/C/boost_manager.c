/*-------------------------------------------------------------------------*
 * $Workfile: boost_manager.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      : Corresponds to 
 *                TM-PTUKT04-1567DraftB Levels and boosting for ETL600.doc 
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/Master_DSP/C/boost_manager.c $ 
 * 
 * 15    12-10-03 10:35 Chhamaa
 * New TPE mode INT_TELPROT_MODE_HSD_25 added.
 * 
 * 11    14.11.06 13:46 Chpeplu
 * Function boostManager_init() changed for custom mode
 * 
 * 8     31.01.06 16:26 Chpeplu
 * new mode INT_TELPROT_MODE_HSD supported
 * 
 * 6     11.08.05 16:50 Chpeplu
 * Boost Bit O4LE_BOOST_A_BIT replaced with O4LE_BOOST_C_BIT and 
 * O4LE_BOOST_C_BIT replaced with O4LE_BOOST_A_BIT
 * 
 * 5     6.07.05 14:03 Chchlee
 * 
 * 4     6.07.05 13:49 Chchlee
 * 
 * 3     20.01.05 16:54 Chchlee
 * upgrade to TM-PTUKT04-1567DraftB Levels and boosting for ETL600.doc
 * 
 * 2     20.01.05 15:37 Chchlee
 * INT_TELPROT_MODE_AES now supported
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 24    8.10.04 9:11 Leeb02
 * APLC_CH_NONE definition used
 * 
 * 23    7.10.04 16:53 Leeb02
 * Bug fixed
 * 
 * 22    6.10.04 11:12 Leeb02
 * INT_TELPROT_MODE_AES not yet supported. Thus it will throw an
 * exception!
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "configuration.h"
#include "tdm_int_types.h"
#include "diag.h"
#include "boost_manager.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// index bits for external boost coder table
#define ANY_BOOST_REQ_BIT           BIT0 
#define TPE1_TEST_BIT               BIT1
#define TPE2_TEST_BIT               BIT2
#define TPE1_START_OR_RX_BIT        BIT3
#define TPE2_START_OR_RX_BIT        BIT4
#define BOOST_GRANT_EXT1_BIT        BIT5
#define BOOST_GRANT_EXT2_BIT        BIT6

#define EXT_BOOST_CODER_TABLE_SIZE  BIT7 // 7 input signals


// index bits for boost prority manager table 1
#define BOOST_REQ_INT1_BIT        BIT0
#define BOOST_REQ_INT2_BIT        BIT1
#define BOOST_REQ_EXT1_BIT        BIT2
#define BOOST_REQ_EXT2_BIT        BIT3
#define BOOST_REQ_EXT3_BIT        BIT4

#define BOOST_PRIO1_TABLE_SIZE  BIT5 // 5 input signals

// index bits for boost prority manager table 2
// uses index bit definitions of external boost coder table
#define BOOST_PRIO2_TABLE_SIZE  BIT5 // 5 input signals


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct
{
	unsigned long   reserved      : 28;
	unsigned long   any           :  1;
	unsigned long   ext1          :  1;
	unsigned long   ext2          :  1;
	unsigned long   ext3          :  1;
}
t_BoostPrio1Table;

typedef struct
{
	unsigned long   reserved      : 30;
	unsigned long   disconnect    :  1;
	unsigned long   disconnectOFDM:  1;
}
t_BoostPrio2Table;

/* LOCAL MACRO DEFINITIONS ***************************************************/

#define BOOST_INPUT  (((t_AllDSP_input_tdm_block *)internalTDM_Rx_Ptr)->BoostMgrInput_TS63)
#define BOOST_OUTPUT (((t_Master_output_tdm_block*)internalTDM_Tx_Ptr)->boostManagerOutput)

/* LOCAL VARIABLES ***********************************************************/

static unsigned long g_ITS_mode;
static BOOL          g_SpeechInAPLC1;
static BOOL          g_SpeechInAPLC2;

static SEGMENT_EXT_DATA unsigned long     g_extBoostCoderTable[MAX_N_O4LE_BOARDS][EXT_BOOST_CODER_TABLE_SIZE]; 

static SEGMENT_EXT_DATA t_BoostPrio1Table g_BoostPrio1Table[BOOST_PRIO1_TABLE_SIZE]; 
static SEGMENT_EXT_DATA t_BoostPrio2Table g_BoostPrio2Table[BOOST_PRIO2_TABLE_SIZE]; 


static long              g_O4LE_APLC_ChannelMapping[MAX_N_O4LE_BOARDS][N_O4LE_CHANNELS]; // supports maximum of 3 O4LEs in system, each with channel A & B 
static t_TDMTimeslotList g_O4LE_BasicTimeslotList;

static t_TDM_HardTimingTelegramInformation * g_to_TDM;
static t_TDM_HardTimingTelegramInformation * g_from_TDM;

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/


void SEGMENT_EXT_CODE boostManager_init(t_TDM_HardTimingTelegramInformation * from_TDM,
										t_TDM_HardTimingTelegramInformation * to_TDM)
{
	int ix, basicTimeSlot, boardIndex;

	
	g_to_TDM   = to_TDM;
	g_from_TDM = from_TDM;

	t_P4LT_Config * p_P4LT_Config = configuration_getPCurrentConfig();

	// extract some information for fast access in "boostManager_activate"
	// @Pl_ext
	g_ITS_mode = (p_P4LT_Config->ITS.mode & INT_TPE_MODE_MASK); // teleprotection mode

	// AES mode is equivalent to INT_TELPROT_MODE_NSD1_APLC1 for the boost manager
	if (INT_TELPROT_MODE_AES == g_ITS_mode)
	{
		g_ITS_mode = INT_TELPROT_MODE_NSD1_APLC1;
	}
	// HSD mode is equivalent to INT_TELPROT_MODE_NSD1_APLC1 for the boost manager
	if (INT_TELPROT_MODE_HSD == g_ITS_mode)
	{
		g_ITS_mode = INT_TELPROT_MODE_NSD1_APLC1;
	}
	// HSD 2.5 kHz mode is equivalent to INT_TELPROT_MODE_NSD1_APLC1 for the boost manager
	if (INT_TELPROT_MODE_HSD_25 == g_ITS_mode)
	{
		g_ITS_mode = INT_TELPROT_MODE_NSD1_APLC1;
	}

	g_SpeechInAPLC1 = TELEPHONY_MASK & p_P4LT_Config->system.serviceData.telephonyTeleprotMode[APLC_CH1];
	g_SpeechInAPLC2 = TELEPHONY_MASK & p_P4LT_Config->system.serviceData.telephonyTeleprotMode[APLC_CH2];

	g_O4LE_BasicTimeslotList.nEntries = 0;

	// extract cross connect information from P4LT configuration:
	// fill "O4LE_APLC_ChannelMapping" and "O4LE_BasicTimeslotList"
	for (basicTimeSlot = 0; basicTimeSlot < NO_TDM_BASIC_TIMESLOTS; basicTimeSlot++)
	{
		if ((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo & BOARD_INFO_KIND_OF_BOARD_MASK) == O4LE_BOARD)
		{
			int aplcChannel;
			ASSERT(g_O4LE_BasicTimeslotList.nEntries <= MAX_N_O4LE_BOARDS); //make sure, configuration does not contain more than MAX_N_O4LE_BOARDS O4LE boards

	  		// O4LE channel A
			aplcChannel = ((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo >> 8) & MASK_4LSB) - 1;

			if (-1 == aplcChannel) // not configured-> mapping to APLC channel 1 does no harm
			{
				aplcChannel = APLC_CH_NONE;
			}
			
			g_O4LE_APLC_ChannelMapping[g_O4LE_BasicTimeslotList.nEntries][O4LE_CH_A] = aplcChannel;

			// O4LE channel B
			aplcChannel = ((p_P4LT_Config->system.TDM_Config[basicTimeSlot].boardInfo >> 12) & MASK_4LSB) - 1;

			if (-1 == aplcChannel) // not configured-> mapping to APLC channel 1 does no harm
			{
				aplcChannel = APLC_CH_NONE;
			}
			
			g_O4LE_APLC_ChannelMapping[g_O4LE_BasicTimeslotList.nEntries][O4LE_CH_B] = aplcChannel;

			g_O4LE_BasicTimeslotList.TimeslotNumber[g_O4LE_BasicTimeslotList.nEntries] = basicTimeSlot;

			g_O4LE_BasicTimeslotList.nEntries++;
		}
	}

	

	#define NOT !  // for better readability
	#define AND && 
	#define OR  || 
		

	#define EXT1_1        (ix & BOOST_GRANT_EXT1_BIT)
	#define EXT2_1        (ix & BOOST_GRANT_EXT2_BIT)
	#define ANY__1        (ix & ANY_BOOST_REQ_BIT)
	#define TST1_1        (ix & TPE1_TEST_BIT)
	#define TST2_1        (ix & TPE2_TEST_BIT)
	#define SOR1_1        (ix & TPE1_START_OR_RX_BIT)
	#define SOR2_1        (ix & TPE2_START_OR_RX_BIT)

	#define EXT1_0        (NOT(ix & BOOST_GRANT_EXT1_BIT))
	#define EXT2_0        (NOT(ix & BOOST_GRANT_EXT2_BIT))
	#define ANY__0        (NOT(ix & ANY_BOOST_REQ_BIT))
	#define TST1_0        (NOT(ix & TPE1_TEST_BIT))
	#define TST2_0        (NOT(ix & TPE2_TEST_BIT))
	#define SOR1_0        (NOT(ix & TPE1_START_OR_RX_BIT))
	#define SOR2_0        (NOT(ix & TPE2_START_OR_RX_BIT))

	#define EXT1_X        (TRUE)
	#define EXT2_X        (TRUE)
	#define ANY__X        (TRUE)
	#define TST1_X        (TRUE)
	#define TST2_X        (TRUE)
	#define SOR1_X        (TRUE)
	#define SOR2_X        (TRUE)


	// O4LE_BasicTimeslotList: index   is O4LE board number
	//	                       element is basic time slot of O4LE
	for (boardIndex = 0; boardIndex < g_O4LE_BasicTimeslotList.nEntries; boardIndex++)
	{
		// TelephonySlotID[APLC_CH1] contains the basic time slot for telephony of each APLC channel
		// TelephonySlotID[N_APLC_CHANNELS]; // slot 0...9 , no telephony: <0 (-1)

		BOOL bThisO4LEHasTelephonyInAPLCCh1 = 
			  (p_P4LT_Config->system.serviceData.TelephonySlotID[APLC_CH1] == g_O4LE_BasicTimeslotList.TimeslotNumber[boardIndex]);

		BOOL bThisO4LEHasTelephonyInAPLCCh2 = 
			  (p_P4LT_Config->system.serviceData.TelephonySlotID[APLC_CH2] == g_O4LE_BasicTimeslotList.TimeslotNumber[boardIndex]);


		// build external boost coder table
		for (ix = 0; ix < EXT_BOOST_CODER_TABLE_SIZE; ix++)
		{
          
			if ((INT_TELPROT_MODE_NONE      == g_ITS_mode) OR
			    (INT_TELPROT_MODE_NSD1_DPLC == g_ITS_mode)    )
			{
				// case 1, table 14
				
				// boost C = always FALSE
				// boost B
				if (
					     (EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
				    OR
					     (EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if (
					     (EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}
			else if (
				      g_SpeechInAPLC1 AND bThisO4LEHasTelephonyInAPLCCh1 AND // speech in APLC channel 1 and on this O4LE
					 (
					  ( INT_TELPROT_MODE_NSD1_APLC1            == g_ITS_mode) OR  // no TPE2          (2a)
					  ( INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC  == g_ITS_mode) OR  // TPE2 in DPLC     (2a)
					  ((INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND g_SpeechInAPLC2)  //(4a)
					 )
					)
			{
				// cases 2a, 4a, table 15
				
				// boost C 
				if (
					     (EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if (
					     (EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
				    OR
					     (EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if (
					     (EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
				    OR
					     (EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X)
				    OR
					     (EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_0 AND TST2_X AND SOR1_1 AND SOR2_X)
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}
			else if (
					 NOT g_SpeechInAPLC1 AND
					 (
					  (INT_TELPROT_MODE_NSD1_APLC1           == g_ITS_mode) OR
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC == g_ITS_mode)    
					 )
					)
				// case 3, table 17
			{
				// boost C 
				if (
					     (EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
				    OR
					     (EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X)
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if (
					     (EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
				    OR
					     (EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if (
					     (EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) 
                   )					
  				     g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}
			else if (
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND
					   g_SpeechInAPLC1                                       AND
					   g_SpeechInAPLC2                                       AND
					   bThisO4LEHasTelephonyInAPLCCh2
					)
				// case 4b, table 18
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_X AND TST2_1 AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_X AND TST2_0 AND SOR1_X AND SOR2_1)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}
			else if (
				     (    // 2b
				           g_SpeechInAPLC1 AND                  // speech in APLC channel 1 
			           NOT bThisO4LEHasTelephonyInAPLCCh1  AND  // no speech in APLC channel 1 on this O4LE 
					  (
					   ( INT_TELPROT_MODE_NSD1_APLC1            == g_ITS_mode) OR  // no TPE2 
					   ( INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC  == g_ITS_mode)     // TPE2 in DPLC
					  )

					 ) 
					 OR // 4c
					 (     g_SpeechInAPLC1 AND g_SpeechInAPLC2 AND // speech in APLC channel 1 & 2
					   NOT bThisO4LEHasTelephonyInAPLCCh1      AND // no speech in APLC channel 1 on this O4LE  
			           NOT bThisO4LEHasTelephonyInAPLCCh2      AND // no speech in APLC channel 2 on this O4LE 
					   (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2  == g_ITS_mode)     
					 )
					)

			// case 2b, 4c, table 16
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;					
			}

			else if (
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND
						   g_SpeechInAPLC1                                   AND
					   NOT g_SpeechInAPLC2                                   AND
                           bThisO4LEHasTelephonyInAPLCCh1
					)
			// case 5a, table 19
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_X AND TST2_1 AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_1 AND TST2_0 AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_0 AND TST2_0 AND SOR1_1 AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}		
			else if (
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND
						   g_SpeechInAPLC1                                   AND
					   NOT g_SpeechInAPLC2                                   AND
                       NOT bThisO4LEHasTelephonyInAPLCCh1
					)
			// case 5b, table 20
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_X AND TST2_1 AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}		
			else if (
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND
					   NOT g_SpeechInAPLC1                                   AND
					       g_SpeechInAPLC2                                   AND
                           bThisO4LEHasTelephonyInAPLCCh2
					)
			// case 6a, table 21
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_0 AND TST2_1 AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_0 AND TST2_0 AND SOR1_X AND SOR2_1)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}		
			else if (
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND
					   NOT g_SpeechInAPLC1                                   AND
					       g_SpeechInAPLC2                                   AND
                       NOT bThisO4LEHasTelephonyInAPLCCh2
					)
			// case 6b, table 22
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}		
			else if (
					  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2 == g_ITS_mode) AND
					   NOT g_SpeechInAPLC1                                   AND
					   NOT g_SpeechInAPLC2
					)
			// case 7, table 23
			{
				// boost C
				if	(
						(EXT1_0 AND EXT2_0 AND ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_0 AND ANY__0 AND TST1_0 AND TST2_1 AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_C_BIT;

				// boost B
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					OR
						(EXT1_0 AND EXT2_1 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_B_BIT;

				// boost A
				if	(
						(EXT1_1 AND EXT2_0 AND ANY__X AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
					)
					g_extBoostCoderTable[boardIndex][ix] |= O4LE_BOOST_A_BIT;
			}
			else ASSERT(FALSE); 
		}
	}

	#undef EXT1_1 
	#undef EXT2_1 
	#undef ANY__1 
	#undef TST1_1 
	#undef TST2_1 
	#undef SOR1_1 
	#undef SOR2_1 

	#undef EXT1_0 
	#undef EXT2_0 
	#undef ANY__0 
	#undef TST1_0 
	#undef TST2_0 
	#undef SOR1_0 
	#undef SOR2_0 

	#undef EXT1_X 
	#undef EXT2_X 
	#undef ANY__X 
	#undef TST1_X 
	#undef TST2_X 
	#undef SOR1_X 
	#undef SOR2_X 

 
    // build boost priority manager table 1

	#define INT1_1        (ix & BOOST_REQ_INT1_BIT)
	#define INT2_1        (ix & BOOST_REQ_INT2_BIT)
	#define EXT1_1        (ix & BOOST_REQ_EXT1_BIT)
	#define EXT2_1        (ix & BOOST_REQ_EXT2_BIT)
	#define EXT3_1        (ix & BOOST_REQ_EXT3_BIT)

	#define INT1_0        (NOT(ix & BOOST_REQ_INT1_BIT))
	#define INT2_0        (NOT(ix & BOOST_REQ_INT2_BIT))
	#define EXT1_0        (NOT(ix & BOOST_REQ_EXT1_BIT))
	#define EXT2_0        (NOT(ix & BOOST_REQ_EXT2_BIT))
	#define EXT3_0        (NOT(ix & BOOST_REQ_EXT3_BIT))

	#define INT1_X        (TRUE)
	#define INT2_X        (TRUE)
	#define EXT1_X        (TRUE)
	#define EXT2_X        (TRUE)
	#define EXT3_X        (TRUE)


	// table 3
	for (ix = 0; ix < BOOST_PRIO1_TABLE_SIZE; ix++)
	{
		if (INT1_0 AND INT2_0 AND EXT1_1 AND EXT2_X AND EXT3_X)
				g_BoostPrio1Table[ix].ext1 = TRUE;
			
		if (INT1_0 AND INT2_0 AND EXT1_0 AND EXT2_1 AND EXT3_X)
				g_BoostPrio1Table[ix].ext2 = TRUE;

		if (INT1_0 AND INT2_0 AND EXT1_0 AND EXT2_0 AND EXT3_1)
				g_BoostPrio1Table[ix].ext3 = TRUE;

		if (INT1_1 OR INT2_1 OR EXT1_1 OR EXT2_1 OR EXT3_1)
				g_BoostPrio1Table[ix].any = TRUE;
	}

	#undef INT1_1  
	#undef INT2_1  
	#undef EXT1_1  
	#undef EXT2_1  
	#undef EXT3_1  

	#undef INT1_0  
	#undef INT2_0  
	#undef EXT1_0  
	#undef EXT2_0  
	#undef EXT3_0  

	#undef INT1_X  
	#undef INT2_X  
	#undef EXT1_X  
	#undef EXT2_X  
	#undef EXT3_X  


    // build boost priority manager table 2

	#define ANY__1        (ix & ANY_BOOST_REQ_BIT)
	#define TST1_1        (ix & TPE1_TEST_BIT)
	#define TST2_1        (ix & TPE2_TEST_BIT)
	#define SOR1_1        (ix & TPE1_START_OR_RX_BIT)
	#define SOR2_1        (ix & TPE2_START_OR_RX_BIT)

	#define ANY__0        (NOT(ix & ANY_BOOST_REQ_BIT))
	#define TST1_0        (NOT(ix & TPE1_TEST_BIT))
	#define TST2_0        (NOT(ix & TPE2_TEST_BIT))
	#define SOR1_0        (NOT(ix & TPE1_START_OR_RX_BIT))
	#define SOR2_0        (NOT(ix & TPE2_START_OR_RX_BIT))

	#define ANY__X        (TRUE)
	#define TST1_X        (TRUE)
	#define TST2_X        (TRUE)
	#define SOR1_X        (TRUE)
	#define SOR2_X        (TRUE)



	for (ix = 0; ix < BOOST_PRIO2_TABLE_SIZE; ix++)
	{
		// generate disconnectOFDM signal

		// table 4: TPE1 in the DPLC channel
		if (INT_TELPROT_MODE_NSD1_DPLC == g_ITS_mode)
		{
			if ((ANY__1 AND TST1_X AND TST2_X) OR
			    (ANY__0 AND TST1_1 AND TST2_X)    )
				g_BoostPrio2Table[ix].disconnectOFDM = TRUE;
 	    }

		//table 5: TPE2 in the DPLC channel
		else if (INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC == g_ITS_mode)
		{
			if ((ANY__1 AND TST1_X AND TST2_X) OR
			    (ANY__0 AND TST1_X AND TST2_1)    )
				g_BoostPrio2Table[ix].disconnectOFDM = TRUE;
 	    }

		//table 6: no TPE in the DPLC channel
		else 
		{
			if (ANY__1 AND TST1_X AND TST2_X)
				g_BoostPrio2Table[ix].disconnectOFDM = TRUE;
 	    }

		// generate disconnect signal
			
		// table 9: cases 4,6,8 in table 7:
		// if TPE1 is located in APLC1 and no speech
		if(  !g_SpeechInAPLC1                                        &&  // no speech in APLC channel 1
			( (INT_TELPROT_MODE_NSD1_APLC1             == g_ITS_mode)||    // if TPE1 is located in APLC1
			  (INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC   == g_ITS_mode)||
			 ((INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2  == g_ITS_mode) && g_SpeechInAPLC2 )  )   )
		{
			if ((ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) OR
				(ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X)    )
				g_BoostPrio2Table[ix].disconnect = TRUE;
		}

		// table 10: case 9 in table 7:
		// if TPE1 is located in APLC1 and    speech and
		//    TPE2 is located in APLC2 and no speech 
		else if (     g_SpeechInAPLC1                                         &&  // speech in APLC channel 1,
			 NOT g_SpeechInAPLC2                                         &&  // no speech in APLC channel 2
			(INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2  == g_ITS_mode)         )

		{
			if ((ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) OR
				(ANY__0 AND TST1_X AND TST2_1 AND SOR1_X AND SOR2_X)    )
				g_BoostPrio2Table[ix].disconnect = TRUE;
		}

		// table 11, case 10 in table 7
		// if TPE1 is located in APLC1 and no speech and
		//    TPE2 is located in APLC2 and no speech 
		else if ( NOT g_SpeechInAPLC1                                         &&  // no speech in APLC channel 1,
			 NOT g_SpeechInAPLC2                                         &&  // no speech in APLC channel 2
			(INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2  == g_ITS_mode)         )

		{
			if ((ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X) OR
				(ANY__0 AND TST1_1 AND TST2_X AND SOR1_X AND SOR2_X) OR   
				(ANY__0 AND TST1_0 AND TST2_1 AND SOR1_X AND SOR2_X)    )
				g_BoostPrio2Table[ix].disconnect = TRUE;
		}
		else
		{
		// table 8, case 1,2,3,5,7 in table 7
			if (ANY__1 AND TST1_X AND TST2_X AND SOR1_X AND SOR2_X)
				g_BoostPrio2Table[ix].disconnect = TRUE;
		}
	}

	#undef ANY__1 
	#undef TST1_1 
	#undef TST2_1 
	#undef SOR1_1 
	#undef SOR2_1 

	#undef ANY__0 
	#undef TST1_0 
	#undef TST2_0 
	#undef SOR1_0 
	#undef SOR2_0 

	#undef ANY__X 
	#undef TST1_X 
	#undef TST2_X 
	#undef SOR1_X 
	#undef SOR2_X 

 
	#undef NOT
	#undef AND
	#undef OR


}

void SEGMENT_EXT_CODE boostManager_activate(volatile long * internalTDM_Rx_Ptr,
											volatile long * internalTDM_Tx_Ptr)
{
	BOOL anyBoostRequest;

	BOOL boostReq_Ext1 = FALSE;
	BOOL boostReq_Ext2 = FALSE;
	BOOL boostReq_Ext3 = FALSE;
	
	// boost grant per APLC channel
	BOOL boostGrant_Ext1, boostGrant_Ext2, boostGrant_Ext3;

	int boardIndex, aplcChannel;

	t_BoostPrio1Table boostPrio1TableEntry;
	t_BoostPrio2Table boostPrio2TableEntry;

	// External Boost Request Cross Connect
	// get external boost requests for APLC channels 1..3
	for (boardIndex = 0; boardIndex < g_O4LE_BasicTimeslotList.nEntries; boardIndex++)
	{
		BOOL boost_O4LE_ChA = g_from_TDM[g_O4LE_BasicTimeslotList.TimeslotNumber[boardIndex]].Additional_7Bit & BOOSTREQ_CH1_SET;
		BOOL boost_O4LE_ChB = g_from_TDM[g_O4LE_BasicTimeslotList.TimeslotNumber[boardIndex]].Additional_7Bit & BOOSTREQ_CH2_SET;

		boostReq_Ext1 |=
			(((g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_A] == APLC_CH1) && boost_O4LE_ChA) ||  
			 ((g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_B] == APLC_CH1) && boost_O4LE_ChB)   );

		boostReq_Ext2 |=
			(((g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_A] == APLC_CH2) && boost_O4LE_ChA) ||  
			 ((g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_B] == APLC_CH2) && boost_O4LE_ChB)   );

	    boostReq_Ext3 |=
			(((g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_A] == APLC_CH3) && boost_O4LE_ChA) || 
			 ((g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_B] == APLC_CH3) && boost_O4LE_ChB)   );
	}

	// Boost Priority Manager
	// part 1
	// 5 Inputs: Boost TPE1, Boost TPE2, Boost ext. TPE1, Boost ext. TPE2, Boost ext. TPE3,
	// 4 Outputs: Boost Grant ext. TPS1, Boost Grant ext. TPS2, Boost Grant ext. TPS3 und Boost of any int. or ext. TPE


	boostPrio1TableEntry = g_BoostPrio1Table   [BOOST_REQ_INT1_BIT * BOOST_INPUT.TPE1_BoostReq    +
												BOOST_REQ_INT2_BIT * BOOST_INPUT.TPE2_BoostReq    +
												BOOST_REQ_EXT1_BIT * boostReq_Ext1                +
												BOOST_REQ_EXT2_BIT * boostReq_Ext2                +
												BOOST_REQ_EXT3_BIT * boostReq_Ext3                  ];

	anyBoostRequest = boostPrio1TableEntry.any;
	boostGrant_Ext1 = boostPrio1TableEntry.ext1;
	boostGrant_Ext2 = boostPrio1TableEntry.ext2;
	boostGrant_Ext3 = boostPrio1TableEntry.ext3;

	// Boost Priority Manager
	// part 2
	// 5 Inputs: Boost of any int. or ext. TPE, Test TPE1, Test TPE2, Start or RxCmd TPE1, Start or RxCmd TPE2,
    // 2 Outputs: Disconnect MOD600, Disconnect.

	boostPrio2TableEntry = g_BoostPrio2Table   [ANY_BOOST_REQ_BIT    * anyBoostRequest               +
												TPE1_TEST_BIT        * BOOST_INPUT.TPE1_Test         +
												TPE2_TEST_BIT        * BOOST_INPUT.TPE2_Test         +
												TPE1_START_OR_RX_BIT * BOOST_INPUT.TPE1_Start_or_Rx  +
												TPE2_START_OR_RX_BIT * BOOST_INPUT.TPE2_Start_or_Rx    ];

	BOOST_OUTPUT.disconnect     = boostPrio2TableEntry.disconnect;
	BOOST_OUTPUT.disconnectOFDM = boostPrio2TableEntry.disconnectOFDM;


	for (boardIndex = 0; boardIndex < g_O4LE_BasicTimeslotList.nEntries; boardIndex++)
	{
		// External boost coder grant cross connect
		// input: boostGrant_Ext1..3
		// output: boost boostGrant_Ext1..2 for each O4LE


		BOOL boostGrant_Ext_O4LE_CH_A = 
			 (boostGrant_Ext1 && (g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_A] == APLC_CH1)) ||
			 (boostGrant_Ext2 && (g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_A] == APLC_CH2)) ||
			 (boostGrant_Ext3 && (g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_A] == APLC_CH3));

		BOOL boostGrant_Ext_O4LE_CH_B = 
			 (boostGrant_Ext1 && (g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_B] == APLC_CH1)) ||
			 (boostGrant_Ext2 && (g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_B] == APLC_CH2)) ||
			 (boostGrant_Ext3 && (g_O4LE_APLC_ChannelMapping[boardIndex][O4LE_CH_B] == APLC_CH3));

		// External boost coder for O4LE
		// input: boostGrant_Ext_O4LE[][]
		// output: external TDM bus
		
		// clear the boost bits
		g_to_TDM[g_O4LE_BasicTimeslotList.TimeslotNumber[boardIndex]].Additional_7Bit &= ~(O4LE_BOOST_C_BIT | 
			                                                                               O4LE_BOOST_B_BIT | 
																					       O4LE_BOOST_A_BIT  );
		// set the boost bits according to g_extBoostCoderTable
		g_to_TDM[g_O4LE_BasicTimeslotList.TimeslotNumber[boardIndex]].Additional_7Bit |= 

		     g_extBoostCoderTable [boardIndex] [BOOST_GRANT_EXT1_BIT * boostGrant_Ext_O4LE_CH_A      +
												BOOST_GRANT_EXT2_BIT * boostGrant_Ext_O4LE_CH_B      +
												ANY_BOOST_REQ_BIT    * anyBoostRequest               +
												TPE1_TEST_BIT        * BOOST_INPUT.TPE1_Test         +
												TPE2_TEST_BIT        * BOOST_INPUT.TPE2_Test         +
												TPE1_START_OR_RX_BIT * BOOST_INPUT.TPE1_Start_or_Rx  +
												TPE2_START_OR_RX_BIT * BOOST_INPUT.TPE2_Start_or_Rx    ];
	}
}
