/*-------------------------------------------------------------------------*
 * $Workfile: NSD_Transmitter.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : P.Plüer
 * Remarks      :  
 * Purpose      : Transmitter for NSD
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSD_DSP/C/NSD_Transmitter.c $ 
 * 
 * 31    12-10-09 15:58 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 30    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 26    28.11.06 10:12 Chpeplu
 * NSD600 Extended: if function nsdCustomTable_packEx() is not successfull
 * -> do not unse NSD channel
 * 
 * 25    27.11.06 10:55 Chpeplu
 * In function Init_Transmitter() the used function
 * CreateTransmittFreqTable() extended with additional call parameter,
 * function transmitter() changed for Delete-Cmd  functionality for NSD600
 * extended 
 * 
 * 24    24.11.06 13:33 Chpeplu
 * Mode NSD600 extended added, convert custom table wirh new functions
 * nsdCustomTable_packEx() and CreateTransmittFreqTable() 
 * !!!!!!-> Mode 3 with delete commands not yet supported correctly!!!!!!
 * 
 * 23    14.11.06 13:25 Chpeplu
 * Replace COM_T by TX_COM_T in function TxPowerReductionMonitoring(),
 * Replace COM_T by TX_COM_T in function Transmitter(), New define added
 * OFFSET_LOOPTEST, In function transmitter() new define OFFSET_LOOPTEST
 * used, Function Init_NSD_Transmitter() changed for extended mode,
 * Function Transmitter() mask for ALL_LP_COMMANDS changed
 * 
 * 22    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 22    20.07.06 17:18 Chhamaa
 * Return value for function Transmitter() added.
 * 
 * 19    2.03.06 14:00 Chtognu
 * HSD Power reduction changed, now always and using new variable
 * (TimedTxPowerReduction)
 * 
 * 18    8.02.06 13:27 Chpeplu
 * HSD Mode 2+1 removed, code for configurable integration time removed
 * 
 * 17    31.01.06 16:24 Chpeplu
 * Tx power Reduction implemented 
 * 
 * 15    30.06.05 8:34 Chpeplu
 * Code walkthrough Pl
 * 
 * 14    27.05.05 14:04 Chpeplu
 * New parameter Channel_active added and function transmitter() will only
 * procced if Channel_active = On 
 * 
 * 13    6.04.05 16:00 Chpeplu
 * Error corrected Scal_Trip for Ch2
 * 
 * 12    6.04.05 15:58 Chpeplu
 * 
 * 11    5.04.05 11:13 Chpeplu
 * new trip level set (Scal_Trip) equal to guard level in the init
 * function; mask for tx command changed to ALL_LP_COMMANDS in function
 * Transmitter()
 * 
 * 10    24.03.05 13:46 Chpeplu
 * Some changed made for variable guard level
 * 
 * 9     14.03.05 10:36 Chpeplu
 * Function Init_NSD_Transmitter() moved to extern and some changes for
 * custom table initialisation made
 * 
 * 8     11.03.05 16:23 Chpeplu
 * NSD-Transmitter uses now DefaultCostumTable
 * 
 * 7     10.03.05 17:53 Chpeplu
 * New setting for the tx frequency according to the custom table added
 * 
 * 6     9.03.05 16:19 Chpeplu
 * New function GenerateTxFreqTable() added
 * 
 * 5     9.03.05 14:05 Chpeplu
 * New include file NSD_CustomTableConverter.h added
 * 
 * 4     9.03.05 10:28 Chpeplu
 * New config used (NsdMode)
 * 
 * 3     8.03.05 17:31 Chpeplu
 * first changes for custom table added
 * 
 * 2     4.03.05 15:24 Chpeplu
 * Prepared for NewConfigNSD without custom table
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 18    19.10.04 12:01 Plueer01
 * Init NSD own guard for ch2 corrected!
 * 
 * 17    28.09.04 16:16 Plueer01
 * magic number 0x010 replaced by define COM_T
 * 
 * 16    27.09.04 17:18 Plueer01
 * Transmitter send frequency for Ch2 changed for HF- Frequency
 * 
 * 15    16.09.04 11:15 Plueer01
 * inintvalue for TxDataCh2.NSD_Pilot = 0; TxDataCh1.NSD_Pilot = 0; added
 * 
 * 14    8.09.04 14:23 Plueer01
 * void Init_NSD_Transmitter(t_NSD_DSP_Config *Config) -> config struct
 * changed
 * 
 * 13    20.08.04 16:31 Plueer01
 * init frequency for ch2 changed for local loop AF signal (Guard ,
 * A,B,AB)
 * 
 * 12    22.07.04 14:32 Plueer01
 * Use of Pilot signal in Ch2 without SSB shift
 * 
 * 11    20.07.04 16:14 Plueer01
 * Frequency shift for NSD commands and gaurd over SSB loop corrected
 * 
 * 10    20.07.04 13:21 Plueer01
 * Tx-Guard frequency define for Loop over SSB added
 * 
 * 9     19.07.04 16:13 Plueer01
 * HMI500 changes update -> struct member in nsd-config removed 
 * 
 * 8     16.07.04 9:32 Plueer01
 * function Init_NSD_Transmitter() corrected do not overwrite guard
 * frequency if 2 NSD's are configured
 * 
 * 7     9.07.04 9:44 Plueer01
 * New HMI600 defines used from NSD_Types.h
 * 
 * 6     8.07.04 17:10 Plueer01
 * NSD_Transmitter.c functions for two channel usable
 * 
 * 5     8.07.04 11:17 Plueer01
 * changes according new NSD-config struct 
 * 
 * 4     27.05.04 13:31 Plueer01
 * Some modifications added
 * 
 * 3     27.05.04 9:11 Plueer01
 * 
 * 2     25.05.04 16:07 Plueer01
 * Initial Version of NSD_Transmitter
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include <math.h>

#include "basetype.h"
#include "NSD_Transmitter.h"
#include "nsdCustomTable.H"
#include "NSD_CustomTableConverter.h"
#include "NSD_DSP_Types.h"
#include "NSD_Types.h"
#include "diag.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define			NUMBER_OF_TX_TONES_NSD			7
//Costum_Table for tests only
/*t_NSDCustomTable testTableTx = 
{	// command   A          B          C          D          E          F						Combination		Command
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
				{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
				{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			CD
				{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			ABCD
				{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			BCD
				{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			ACD
				{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			ABD
				{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			D
				{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			BD
				{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			AD
				{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			ABC
				{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
				{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			BC
				{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			AC
				{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
				{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
				{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
};
*/
// for tests only
float NSD_Freq_Table[NUMBER_OF_TX_TONES_NSD + 1] ={
	0,
	NSD_FREQ_1N,
	NSD_FREQ_2N,
	NSD_FREQ_3N,
	NSD_FREQ_4N,
	NSD_FREQ_5N,
	NSD_FREQ_6N,
	NSD_FREQ_7N
};
//
//
float NSD_Freq_Table_Normal[NUMBER_OF_TX_TONES_NSD + 1] ={
	0.0,
	((TWO_PI*(NSD_FREQ_1))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(NSD_FREQ_2))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(NSD_FREQ_3))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(NSD_FREQ_4))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(NSD_FREQ_5))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(NSD_FREQ_6))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(NSD_FREQ_7))/(1.0*(AF_SAMPLE_FREQ)))
};
//
// HSD transmit frequencies for tests only
float HSD_Freq_Table_Normal[NUMBER_OF_TX_TONES_NSD + 1] ={
	0.0,
	((TWO_PI*(HSD_FREQ_1))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_2))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_3))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_4))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_5))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_6))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_7))/(1.0*(AF_SAMPLE_FREQ)))
};

float HSD_Freq_Table_Normal2k[NUMBER_OF_TX_TONES_NSD + 1] ={
	0.0,
	((TWO_PI*(HSD_FREQ_12))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_22))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_32))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_42))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_52))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_62))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_FREQ_72))/(1.0*(AF_SAMPLE_FREQ)))
};

float HSD_25_Freq_Table_Normal[NUMBER_OF_TX_TONES_NSD + 1] ={
	0.0,
	((TWO_PI*(HSD_25_FREQ_1))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_25_FREQ_2))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_25_FREQ_3))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_25_FREQ_4))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_25_FREQ_5))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_25_FREQ_6))/(1.0*(AF_SAMPLE_FREQ))),
	((TWO_PI*(HSD_25_FREQ_7))/(1.0*(AF_SAMPLE_FREQ)))
};


// pilot level -6dB factor (transmitter is implemented as 2 * two tone sine for guard and 2 * two tone sine for trip
// amplitude for each tone is halfe of max amplitude!
//
#define NUMBER_OF_TONE_PER_CMD		2
//
// The NSD600 signals were feed in after the SSB, so the TPE-signals must be reduced by factor 2 again!
#define	SSB_LEVEL_REDUCTION			2
// @Pl_ext
// The offset for the loop test is the last two adresses of the table TransmitSin
#define OFFSET_LOOPTEST				(MAX_COMMAND_COMBINATION*NUMBER_OF_TONE_PER_CMD)
//
/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef struct 
{   
	float TransmitSin[(MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD];
	float TransmitCos[(MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD];
	float TxSin_new[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	float TxCos_new[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	float TxSin_0[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	float TxCos_0[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
//
	float Scal_Trip;								// scale factor including actual boost value
	float Scal_Guard;								// scale factor for NSD own pilot
	float Scal_Test;								// scale factor for Loop test signal
	BOOL  NSD_Pilot;								// TRUE = NSD own pilot ;  FALSE = ETL_Pilot
	BOOL  Channel_active;							// ON if channel is activ OFF if channel is not configured
// used parameter for HSD600
	BOOL  HSD_Type;									// true -> HSD is configured / false -> NSD or AES is configured
	float Scal_Tx_power_reduction;					// Scalling in case of tx power reduction activ
	long  Tx_Power_Red_Counter;						// counter for time monitoring 
	BOOL  Power_Reduction_activ;					// the power reduction is activ for transmitter
	long  Max_Tx_Time_Power_red;					// time limit to set the tx power reduction
	// @Pl_ext
	BOOL  Mode_delete_Cmd;							// used for custom mode only if delete cmd is enabled		
} t_Transmit_Data;
//
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/
static t_Transmit_Data TxDataCh1, TxDataCh2;
SEGMENT_EXT_DATA t_Custom2CommandTable Custom2CommandTable;
SEGMENT_EXT_DATA unsigned long TxPriorityTable[MAX_COMMAND_COMBINATION +1];

SEGMENT_EXT_DATA unsigned long SendFreq_Temp[(MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD];
SEGMENT_EXT_DATA float Normal_SendFreq[(MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD];				// for tests only

SEGMENT_EXT_DATA t_NSDCustomTable UnpackedCustomTable;
static SEGMENT_EXT_DATA t_NSDExtCustomTable localCustomTable;
static SEGMENT_EXT_DATA unsigned long OverWriteCmd_withGuard_Table[(MAX_COMMAND_COMBINATION+1)];
//
/* LOCAL FUNCTIONS ***********************************************************/
/* IMPLEMENTATION ************************************************************/
//
/*****************************************************************************
  Function:		Init_TxPower_Reduction()
  Description:	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	-

*******************************************************************************/
void SEGMENT_EXT_CODE Init_TxPower_Reduction(t_NSD_DSP_Config *Config)
{
	int i = 0;
	//
	TxDataCh1.Power_Reduction_activ = FALSE;		// init power reduction activ to off!
	TxDataCh1.Tx_Power_Red_Counter = 0;				// init counter time to 0

	
	TxDataCh1.Max_Tx_Time_Power_red = (long)(Config->NSD[0].TimedTxPowerReduction * ONE_SECOND);

	// init scale factor for reduction power
	TxDataCh1.Scal_Tx_power_reduction = TxDataCh1.Scal_Test;	// set scale factor to loop test level
	//
}
//
/*****************************************************************************
  Function:		Init_NSD_Transmitter()
  Description:	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	-

*******************************************************************************/
void SEGMENT_EXT_CODE Init_NSD_Transmitter(t_NSD_DSP_Config *Config)
{
	int		Counter, Temp;
	float	f_temp;
	float	shift_freq, hsd_freq_12, hsd_freq_22, hsd_freq_32, hsd_freq_42, hsd_freq_52, hsd_freq_62, hsd_freq_72; 
	long	l_N_HSD;
	// @Pl_ext
	BOOL success;
	long TPE_Basic_Mode = 0;
	long TPE1_ext_Mode = 0;
	long TPE2_ext_Mode = 0;
	//
	// @Pl_ext
	TPE_Basic_Mode = (Config->mode & INT_TPE_MODE_MASK);
	TPE1_ext_Mode = (Config->mode & INT_TPE1_MODE_EXT_MASK);
	TPE2_ext_Mode = (Config->mode & INT_TPE2_MODE_EXT_MASK);

	// reset Custom2CommandTable
	for(Counter=0;Counter < NSD_CUSTOM_TABLE_SIZE; Counter++)
	{
		Custom2CommandTable[Counter] = 0;	
	}

	// reset TxPriorityTable
	for(Counter = 0; Counter< MAX_COMMAND_COMBINATION +1; Counter++)
	{
		TxPriorityTable[Counter] = 0;	
	}
	// reset send frequency table
	for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
	{
		SendFreq_Temp[Counter] = 0;	
		TxDataCh1.TransmitSin[Counter] = 0.0;
		TxDataCh1.TransmitCos[Counter] = 0.0;
		TxDataCh2.TransmitSin[Counter] = 0.0;
		TxDataCh2.TransmitCos[Counter] = 0.0;
	}
	// @Pl_ext
	// switch off Mode_delete_Cmd
	TxDataCh1.Mode_delete_Cmd = FALSE;
	TxDataCh2.Mode_delete_Cmd = FALSE;
	//
	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			TxDataCh1.Channel_active = OFF;
			TxDataCh2.Channel_active = OFF;
			TxDataCh1.HSD_Type = FALSE;
			TxDataCh2.HSD_Type = FALSE;
			break;
		case INT_TELPROT_MODE_AES:
			// AES is not supported in this version and will be init separately
			TxDataCh1.Channel_active = OFF;
			TxDataCh2.Channel_active = OFF;
			TxDataCh1.HSD_Type = FALSE;
			TxDataCh2.HSD_Type = FALSE;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
			//
			if(TPE1_ext_Mode == 0)
			{
				// -> no custom mode for ch1 configured
				//
				// ****************************************************************
				// *************** init command frequencies for Ch1 ***************
				// ****************************************************************
				//
				nsdCustomTable_pack (FALSE, UnpackedCustomTable, Config->NSD[0].customTable);
				GenerateTxPriorityTable(UnpackedCustomTable, Custom2CommandTable, TxPriorityTable);
				GenerateTxFreqTable(Custom2CommandTable, TxPriorityTable, SendFreq_Temp);
				//
				// write Frequency to Tx Array transmitter
				//
				for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
				{
					Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
					TxDataCh1.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					TxDataCh1.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
				}
				// @Pl_ext
				TxDataCh1.Channel_active = ON;
			}
			else
			{
				// Custom mode activ do init with extended custome table!
				//
				// -> to do @Pl_ext
				// -> unpack custom table
				success = nsdCustomTable_packEx(FALSE,  &localCustomTable, Config->NSD[0].customTable);
				if(success)
				{
					CreateTransmittFreqTable(SendFreq_Temp, OverWriteCmd_withGuard_Table, &localCustomTable);
					// set Mode_delete_Cmd according to custom table entry
					if(localCustomTable.nrOfGuardCommands != 0) TxDataCh1.Mode_delete_Cmd = TRUE;
					//
					// write Frequency to Tx Array transmitter
					//
					for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
					{
						Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
						TxDataCh1.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
						TxDataCh1.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					}
					TxDataCh1.Channel_active = ON;
				}
				else
				{
					// unpack custom Table not success full!!
					TxDataCh1.Channel_active = OFF;
				}
			}
			//
			// *************************************************************
			// *** init nsd own guard frequency Ch1   **********************
			// *************************************************************
			//
			if((Config->NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				TxDataCh1.NSD_Pilot = TRUE;
				//	f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency))/(1.0*(AF_SAMPLE_FREQ))); // local loop without SSB
				f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency - TX_FREQ_SHIFT_IN_HF))/(1.0*(AF_SAMPLE_FREQ))); // loop over SSB
			}
			else
			{
				TxDataCh1.NSD_Pilot = FALSE;
				f_temp = 0.0;
			}
			TxDataCh1.TransmitSin[0] = sinf(f_temp);
			TxDataCh1.TransmitSin[1] = sinf(f_temp);
			TxDataCh1.TransmitCos[0] = cosf(f_temp);
			TxDataCh1.TransmitCos[1] = cosf(f_temp);
			//
			// *************************************************************
			// *** set boost and scal factor for transmitter ***************
			// *************************************************************
			// Level settings for Ch1
			TxDataCh1.Scal_Guard = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
			TxDataCh1.Scal_Trip = TxDataCh1.Scal_Guard * Config->NSD[0].BoostLevel;
			if(Config->NSD[0].GuardLevel > TRIP_WEIGHT_DEFAULT)
			{
				TxDataCh1.Scal_Test = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = ((Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel);
			}
			else
			{
				// use default weight (1.0) for test and trip signal
				TxDataCh1.Scal_Test = (TRIP_WEIGHT_DEFAULT/ (NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = (TRIP_WEIGHT_DEFAULT/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel;
			}
			//
			TxDataCh2.Channel_active = OFF;
			TxDataCh1.HSD_Type = FALSE;
			TxDataCh2.HSD_Type = FALSE;
			break;

		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			//
			if(TPE1_ext_Mode == 0)
			{
				// -> no custom mode for ch1 configured
				//
				// ****************************************************************
				// *************** init command frequencies for Ch1 ***************
				// ****************************************************************
				//
				nsdCustomTable_pack (FALSE, UnpackedCustomTable, Config->NSD[0].customTable);
				GenerateTxPriorityTable(UnpackedCustomTable, Custom2CommandTable, TxPriorityTable);
				GenerateTxFreqTable(Custom2CommandTable, TxPriorityTable, SendFreq_Temp);
				//
				// write Frequency to Tx Array transmitter
				//
				for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
				{
					Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
					TxDataCh1.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					TxDataCh1.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
				}
				TxDataCh1.Channel_active = ON;
			}
			else
			{
				//
				// Custom mode activ do init with extended custom table!
				//
				// -> to do @Pl_ext
				// -> unpack custom table
				//
				// !!!!!!!!!! Mode not supported by HMI600 yet !!!!!!!!!!!!
				//
				success = nsdCustomTable_packEx(FALSE,  &localCustomTable, Config->NSD[0].customTable);
				if(success)
				{
					CreateTransmittFreqTable(SendFreq_Temp, OverWriteCmd_withGuard_Table, &localCustomTable);
					//
					// set Mode_delete_Cmd according to custom table entry
					if(localCustomTable.nrOfGuardCommands != 0) TxDataCh1.Mode_delete_Cmd = TRUE;
					//
					// write Frequency to Tx Array transmitter
					//
					for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
					{
						Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
						TxDataCh1.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
						TxDataCh1.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					}
					TxDataCh1.Channel_active = ON;
				}
				else
				{
					// unpack custom Table not success full!!
					TxDataCh1.Channel_active = OFF;
				}
			}
			//
			if(TPE2_ext_Mode == 0)
			{
				//
				// -> no custom mode for ch2 configured
				//
				// reset Custom2CommandTable
				for(Counter=0;Counter < NSD_CUSTOM_TABLE_SIZE; Counter++)
				{
					Custom2CommandTable[Counter] = 0;	
				}

				// reset TxPriorityTable
				for(Counter = 0; Counter< MAX_COMMAND_COMBINATION +1; Counter++)
				{
					TxPriorityTable[Counter] = 0;	
				}
				// reset send frequency table
				for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
				{
					SendFreq_Temp[Counter] = 0;	
				}
				//
				nsdCustomTable_pack (FALSE, UnpackedCustomTable, Config->NSD[1].customTable);
				GenerateTxPriorityTable(UnpackedCustomTable, Custom2CommandTable, TxPriorityTable);
				GenerateTxFreqTable(Custom2CommandTable, TxPriorityTable, SendFreq_Temp);
				// write Frequency to Tx Array transmitter
				for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
				{
					Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
					TxDataCh2.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					TxDataCh2.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
				}
				TxDataCh2.Channel_active = ON;
			}
			else
			{
				//
				// Custom mode activ do init with extended custome table!
				//
				// -> to do @Pl_ext
				//
				// !!!!!!!!!! Mode not supported by HMI600 yet !!!!!!!!!!!!
				//
				success = nsdCustomTable_packEx(FALSE,  &localCustomTable, Config->NSD[1].customTable);
				if(success)
				{
					CreateTransmittFreqTable(SendFreq_Temp, OverWriteCmd_withGuard_Table, &localCustomTable);
					//
					// set Mode_delete_Cmd according to custom table entry
					if(localCustomTable.nrOfGuardCommands != 0) TxDataCh2.Mode_delete_Cmd = TRUE;
					//
					// write Frequency to Tx Array transmitter
					//
					for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
					{
						Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
						TxDataCh2.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
						TxDataCh2.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					}
					TxDataCh2.Channel_active = ON;
				}
				else
				{
					// unpack custom Table not success full!!
					TxDataCh2.Channel_active = OFF;
				}
			}
			//
			// *************************************************************
			// *** init nsd own guard frequency Ch1   **********************
			// *************************************************************
			//
			if((Config->NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				TxDataCh1.NSD_Pilot = TRUE;
				//f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency))/(1.0*(AF_SAMPLE_FREQ))); // local loop without SSB
				f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency - TX_FREQ_SHIFT_IN_HF))/(1.0*(AF_SAMPLE_FREQ))); // loop over SSB
			}
			else
			{
				TxDataCh1.NSD_Pilot = FALSE;
				f_temp = 0.0;
			}
			TxDataCh1.TransmitSin[0] = sinf(f_temp);
			TxDataCh1.TransmitSin[1] = sinf(f_temp);
			TxDataCh1.TransmitCos[0] = cosf(f_temp);
			TxDataCh1.TransmitCos[1] = cosf(f_temp);
			//
			// *************************************************************
			// *** init nsd own guard frequency Ch2   **********************
			// *************************************************************
			if((Config->NSD[1].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				TxDataCh2.NSD_Pilot = TRUE;
				//	f_temp = ((TWO_PI*(Config->NSD[1].PilotFrequency))/(1.0*(AF_SAMPLE_FREQ))); // NSD loop without SSB
				f_temp = ((TWO_PI*(Config->NSD[1].PilotFrequency - TX_FREQ_SHIFT_IN_HF))/(1.0*(AF_SAMPLE_FREQ))); // Loop over SSB
			}
			else
			{
				TxDataCh2.NSD_Pilot = FALSE;
				f_temp = 0.0;
			}
			TxDataCh2.TransmitSin[0] = sinf(f_temp);
			TxDataCh2.TransmitSin[1] = sinf(f_temp);
			TxDataCh2.TransmitCos[0] = cosf(f_temp);
			TxDataCh2.TransmitCos[1] = cosf(f_temp);
			//
			// *************************************************************
			// *** set boost and scal factor for transmitter ***************
			// *************************************************************
			// Level settings for Ch1
			TxDataCh1.Scal_Guard = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
			TxDataCh1.Scal_Trip = TxDataCh1.Scal_Guard * Config->NSD[0].BoostLevel;
			if(Config->NSD[0].GuardLevel > TRIP_WEIGHT_DEFAULT)
			{
				TxDataCh1.Scal_Test = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = ((Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel);
			}
			else
			{
				// use default weight (1.0) for test and trip signal
				TxDataCh1.Scal_Test = (TRIP_WEIGHT_DEFAULT/ (NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = (TRIP_WEIGHT_DEFAULT/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel;
			}
			//
			// Level settings for Ch2
			//
			TxDataCh2.Scal_Guard = (Config->NSD[1].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
			TxDataCh2.Scal_Trip = TxDataCh2.Scal_Guard * Config->NSD[1].BoostLevel;
			if(Config->NSD[1].GuardLevel > TRIP_WEIGHT_DEFAULT)
			{
				TxDataCh2.Scal_Test = (Config->NSD[1].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh2.Scal_Trip = ((Config->NSD[1].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[1].BoostLevel);
			}
			else
			{
				// use default weight (1.0) for test and trip signal
				TxDataCh2.Scal_Test = (TRIP_WEIGHT_DEFAULT/ (NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh2.Scal_Trip = (TRIP_WEIGHT_DEFAULT/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[1].BoostLevel;
			}
			//
			TxDataCh1.HSD_Type = FALSE;
			TxDataCh2.HSD_Type = FALSE;
			break;
		case INT_TELPROT_MODE_HSD:
			//
			if(TPE1_ext_Mode == 0)
			{
				// -> no custom mode for ch1 configured
				//
				// ****************************************************************
				// *************** init command frequencies for Ch1 ***************
				// ****************************************************************
				//
				nsdCustomTable_pack (FALSE, UnpackedCustomTable, Config->NSD[0].customTable);
				GenerateTxPriorityTable(UnpackedCustomTable, Custom2CommandTable, TxPriorityTable);
				GenerateTxFreqTable(Custom2CommandTable, TxPriorityTable, SendFreq_Temp);
				//
				// write Frequency to Tx Array transmitter
				//
				for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
				{
					Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
					TxDataCh1.TransmitSin[Counter]	= sinf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					TxDataCh1.TransmitCos[Counter]	= cosf(NSD_Freq_Table_Normal[SendFreq_Temp[Counter]]);
				}
				TxDataCh1.Channel_active = ON;
				TxDataCh2.Channel_active = OFF;
				TxDataCh1.HSD_Type = TRUE;
				TxDataCh2.HSD_Type = FALSE;
			}
			else
			{
				// Custom mode activ do init with extended custome table!
				//
				// !!!!!!!!!!!! -> Mode not supported !!!!!!!!!!!!!!!!!!!!
				// 
				TxDataCh1.Channel_active = OFF;
				TxDataCh2.Channel_active = OFF;
				TxDataCh1.HSD_Type = FALSE;
				TxDataCh2.HSD_Type = FALSE;
			}
			//
			// *************************************************************
			// *** init nsd own guard frequency Ch1   **********************
			// *************************************************************
			//
			if((Config->NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				TxDataCh1.NSD_Pilot = TRUE;
				//	f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency))/(1.0*(AF_SAMPLE_FREQ))); // local loop without SSB
				f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency - TX_FREQ_SHIFT_IN_HF))/(1.0*(AF_SAMPLE_FREQ))); // loop over SSB
			}
			else
			{
				TxDataCh1.NSD_Pilot = FALSE;
				f_temp = 0.0;
			}
			TxDataCh1.TransmitSin[0] = sinf(f_temp);
			TxDataCh1.TransmitSin[1] = sinf(f_temp);
			TxDataCh1.TransmitCos[0] = cosf(f_temp);
			TxDataCh1.TransmitCos[1] = cosf(f_temp);
			//
			// *************************************************************
			// *** set boost and scal factor for transmitter ***************
			// *************************************************************
			// Level settings for Ch1
			TxDataCh1.Scal_Guard = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
			TxDataCh1.Scal_Trip = TxDataCh1.Scal_Guard * Config->NSD[0].BoostLevel;
			if(Config->NSD[0].GuardLevel > TRIP_WEIGHT_DEFAULT)
			{
				TxDataCh1.Scal_Test = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = ((Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel);
			}
			else
			{
				// use default weight (1.0) for test and trip signal
				TxDataCh1.Scal_Test = (TRIP_WEIGHT_DEFAULT/ (NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = (TRIP_WEIGHT_DEFAULT/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel;
			}
			//
			//
			//	new for selectable N
			//	
			l_N_HSD = INTEGR_TIME_DETECTOR_HSD;
			shift_freq = ((1.0*AF_SAMPLE_FREQ)/(2*l_N_HSD));
			//
			// HSD600 Mode 3+1
			//
			// set freq. Table for selected N and mode 3+1
			//
			hsd_freq_12 = ((HSD_CENTRE_FREQ - 2*shift_freq)- TX_FREQ_SHIFT_IN_HF_HSD);
			hsd_freq_22	= ((HSD_CENTRE_FREQ - 1*shift_freq)- TX_FREQ_SHIFT_IN_HF_HSD);
			hsd_freq_32	= ((HSD_CENTRE_FREQ - 0*shift_freq) - TX_FREQ_SHIFT_IN_HF_HSD);
			hsd_freq_42	= ((HSD_CENTRE_FREQ + 1*shift_freq)- TX_FREQ_SHIFT_IN_HF_HSD);
			hsd_freq_52	= ((HSD_CENTRE_FREQ + 2*shift_freq)- TX_FREQ_SHIFT_IN_HF_HSD);
			hsd_freq_62	= 0.0;
			hsd_freq_72	= 0.0;
			//
			HSD_Freq_Table_Normal2k[1] = ((TWO_PI*(hsd_freq_12))/(1.0*(AF_SAMPLE_FREQ)));
			HSD_Freq_Table_Normal2k[2] = ((TWO_PI*(hsd_freq_22))/(1.0*(AF_SAMPLE_FREQ)));
			HSD_Freq_Table_Normal2k[3] = ((TWO_PI*(hsd_freq_32))/(1.0*(AF_SAMPLE_FREQ)));
			HSD_Freq_Table_Normal2k[4] = ((TWO_PI*(hsd_freq_42))/(1.0*(AF_SAMPLE_FREQ)));
			HSD_Freq_Table_Normal2k[5] = ((TWO_PI*(hsd_freq_52))/(1.0*(AF_SAMPLE_FREQ)));
			HSD_Freq_Table_Normal2k[6] = ((TWO_PI*(hsd_freq_62))/(1.0*(AF_SAMPLE_FREQ)));
			HSD_Freq_Table_Normal2k[7] = ((TWO_PI*(hsd_freq_72))/(1.0*(AF_SAMPLE_FREQ)));
			//
			// write Frequency to Tx Array transmitter
			for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
			{
				TxDataCh1.TransmitSin[Counter]	= sinf(HSD_Freq_Table_Normal2k[SendFreq_Temp[Counter]]);
				TxDataCh1.TransmitCos[Counter]	= cosf(HSD_Freq_Table_Normal2k[SendFreq_Temp[Counter]]);
			}
			//
			Init_TxPower_Reduction(Config);
			break;

		case INT_TELPROT_MODE_HSD_25:
			//
			if(TPE1_ext_Mode == 0)
			{
				// -> no custom mode for ch1 configured
				//
				// ****************************************************************
				// *************** init command frequencies for Ch1 ***************
				// ****************************************************************
				//
				nsdCustomTable_pack (FALSE, UnpackedCustomTable, Config->NSD[0].customTable);
				GenerateTxPriorityTable(UnpackedCustomTable, Custom2CommandTable, TxPriorityTable);
				GenerateTxFreqTable(Custom2CommandTable, TxPriorityTable, SendFreq_Temp);
				//
				// write Frequency to Tx Array transmitter
				//
				for(Counter = 0; Counter < (MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD; Counter++)
				{
					Normal_SendFreq[Counter]		= NSD_Freq_Table[SendFreq_Temp[Counter]];	// for tests only
					TxDataCh1.TransmitSin[Counter]	= sinf(HSD_25_Freq_Table_Normal[SendFreq_Temp[Counter]]);
					TxDataCh1.TransmitCos[Counter]	= cosf(HSD_25_Freq_Table_Normal[SendFreq_Temp[Counter]]);
				}
				
				TxDataCh1.Channel_active = ON;
				TxDataCh2.Channel_active = OFF;
				TxDataCh1.HSD_Type = TRUE;
				TxDataCh2.HSD_Type = FALSE;
			}
			else
			{
				// Custom mode activ do init with extended custome table!
				//
				// !!!!!!!!!!!! -> Mode not supported !!!!!!!!!!!!!!!!!!!!
				// 
				TxDataCh1.Channel_active = OFF;
				TxDataCh2.Channel_active = OFF;
				TxDataCh1.HSD_Type = FALSE;
				TxDataCh2.HSD_Type = FALSE;
			}

			//
			// *************************************************************
			// *** init nsd own guard frequency Ch1   **********************
			// *************************************************************
			//
			if((Config->NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED)
			{
				TxDataCh1.NSD_Pilot = TRUE;
				//	f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency))/(1.0*(AF_SAMPLE_FREQ))); // local loop without SSB
				f_temp = ((TWO_PI*(Config->NSD[0].PilotFrequency - HSD_25_TX_FREQ_SHIFT_IN_HF))/(1.0*(AF_SAMPLE_FREQ))); // loop over SSB
			}
			else
			{
				TxDataCh1.NSD_Pilot = FALSE;
				f_temp = 0.0;
			}

			TxDataCh1.TransmitSin[0] = sinf(f_temp);
			TxDataCh1.TransmitSin[1] = sinf(f_temp);
			TxDataCh1.TransmitCos[0] = cosf(f_temp);
			TxDataCh1.TransmitCos[1] = cosf(f_temp);

			//
			// *************************************************************
			// *** set boost and scal factor for transmitter ***************
			// *************************************************************
			// Level settings for Ch1
			TxDataCh1.Scal_Guard = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
			TxDataCh1.Scal_Trip = TxDataCh1.Scal_Guard * Config->NSD[0].BoostLevel;
			if(Config->NSD[0].GuardLevel > TRIP_WEIGHT_DEFAULT)
			{
				TxDataCh1.Scal_Test = (Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = ((Config->NSD[0].GuardLevel/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel);
			}
			else
			{
				// use default weight (1.0) for test and trip signal
				TxDataCh1.Scal_Test = (TRIP_WEIGHT_DEFAULT/ (NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION));
				//TxDataCh1.Scal_Trip = (TRIP_WEIGHT_DEFAULT/(NUMBER_OF_TONE_PER_CMD * SSB_LEVEL_REDUCTION)) * Config->NSD[0].BoostLevel;
			}
			
			Init_TxPower_Reduction(Config);
			
			break;

		default:
			ASSERT(FALSE);
	}
	
	//
	// *************************************************************
	// *** set up start values for transmitter *********************
	// *************************************************************
	//
	TxDataCh1.TxSin_0[0] = 0.0;	// start value
	TxDataCh1.TxSin_0[1] = 0.0;
	TxDataCh1.TxSin_new[0] = 0.0;
	TxDataCh1.TxSin_new[1] = 0.0;
	TxDataCh1.TxCos_0[0] = 1.0;	// start value
	TxDataCh1.TxCos_0[1] = 1.0;
	TxDataCh1.TxCos_new[0] = 0.0;
	TxDataCh1.TxCos_new[1] = 0.0;
	//
	TxDataCh2.TxSin_0[0] = 0.0;	// start value
	TxDataCh2.TxSin_0[1] = 0.0;
	TxDataCh2.TxSin_new[0] = 0.0;
	TxDataCh2.TxSin_new[1] = 0.0;
	TxDataCh2.TxCos_0[0] = 1.0;	// start value
	TxDataCh2.TxCos_0[1] = 1.0;
	TxDataCh2.TxCos_new[0] = 0.0;
	TxDataCh2.TxCos_new[1] = 0.0;

}
//
/*****************************************************************************
  Function:		TxPowerReductionMonitoring(t_Transmit_Data *ptr_TxData, long Command_Input)
  Description:	This function set the parameter  Power_Reduction_activ to TRUE if the command is activ
				longer then the configured time and the monitoring is switched on for this command.
  Inputs:		Activ tx commands
  Outputs:		
  Return:		None
  Globals:		
  Important:	This function monitors all command with tx power red. monitoring on in common.
*******************************************************************************/
//
void TxPowerReductionMonitoring(t_Transmit_Data *ptr_TxData, long Command_Input)
{
	int i=0;
	//
	ptr_TxData->Power_Reduction_activ = FALSE;		// set power reduction off for activ TPE
	//
	if(ptr_TxData->HSD_Type)
	{
		//
		// HSD is configured
		//
		if((Command_Input & ~TX_COM_T) > 0) // // do no reduction if Testcommand active
		{
			//
			// command is on
			//
			ptr_TxData->Tx_Power_Red_Counter = ptr_TxData->Tx_Power_Red_Counter +1;										// increment timer 
			
			if(ptr_TxData->Tx_Power_Red_Counter >= ptr_TxData->Max_Tx_Time_Power_red)
			{
				//
				// monitoring time over -> set power reduction activ
				//
				ptr_TxData->Tx_Power_Red_Counter = ptr_TxData->Max_Tx_Time_Power_red;	// freeze counter
				ptr_TxData->Power_Reduction_activ = TRUE;								// set Power reduction activ
			}
			else
			{
				// time for power reduction monitoring not over -> do nothing
			}
		}
		else
		{
			ptr_TxData->Tx_Power_Red_Counter = 0;		// reset counter 	
		}
	}
	else
	{
		// HSD is not configured -> do nothing
	}

}
//
/*****************************************************************************
  Function:		Transmitter(float *I_Signal, float *Q_Signal, long Command_Input, long Channel)
  Description:	Do the generation of the tx signal for tx trip, Loop test or NSD own guard
  Inputs:		
  Outputs:		I- and Q-Signal for fs=10.666kHz
  Return:		None
  Globals:		
  Important:	
*******************************************************************************/
//
BOOL Transmitter(float *I_Signal, float *Q_Signal, long Command_Input, long Channel)
{
	int i;
	float Scale;
	long l_temp_t = 0;
	long tempreg;
	BOOL sending = FALSE;
	// 
	float *ptr_fSin, *ptr_fCos;
	t_Transmit_Data *ptr_TxData;

	//
	if(Channel == TPE1)
	{
		ptr_TxData = &TxDataCh1;
		ptr_fSin = TxDataCh1.TransmitSin;
		ptr_fCos = TxDataCh1.TransmitCos;
	}
	else
	{
		if(Channel == TPE2)
		{
			ptr_TxData = &TxDataCh2;
			ptr_fSin = TxDataCh2.TransmitSin;
			ptr_fCos = TxDataCh2.TransmitCos;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

 if(ptr_TxData->Channel_active == ON)
 {
	// ********************************************************************************************
	// *** TPE channel is on **********************************************************************
	// ********************************************************************************************
	//
	TxPowerReductionMonitoring(ptr_TxData, Command_Input);	// do power reduction for HSD only
	//
	// *********************************************************
	// *** Overwrite Command_Input in case of custom mode with delete command activ
	// *********************************************************
	//
	if((ptr_TxData->Mode_delete_Cmd)&&((Command_Input&ALL_LP_COMMANDS_A_F) != 0))
	{
		// check if delete cmd is activ
		// !!!!!!!!!!!!!!!!!!!!!!!
		// @Pl_ext -> to do
		if(OverWriteCmd_withGuard_Table[Command_Input] == SEND_GUARD)
		{
			// reset cmd 
			Command_Input = 0;
		}
		else
		{
			// do not reset cmd
		}
	}
	else
	{
		// no cmd active or Mode_delete_cmd is not configured -> do no reset tx cmd
	}
	//
	if (((Command_Input&(ALL_LP_COMMANDS_A_F|TX_COM_T)) == 0)&& (ptr_TxData->NSD_Pilot == FALSE))
	{
		// 
		// no command and no looptest activ and ETL-Pilot on
		//
		//
		for (i= 0; i<NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE;i++)
		{
			*I_Signal++ = 0.0;	// do not send any signal
			*Q_Signal++ = 0.0;
		}
	}
	else
	{
		// 
		// NSD own guard is activ or command or looptest is activ!
		// 
		// convert RTB-Input to commands
		// @Pl_ext
		l_temp_t = (Command_Input & ALL_LP_COMMANDS_A_F);	// mask command A&B&C&D&E&F
		//
		if(l_temp_t == 0x0000) 
		{
			// ************************************************
			// *** no tx command active ***********************
			// ************************************************
			//
			Scale = ptr_TxData->Scal_Guard;				// set scale factor for looptest and guard -6dB

			if((Command_Input&TX_COM_T) == TX_COM_T) 
			{
				// ********************************************
				// *** send loop test *************************
				// ********************************************
				//
				// @Pl_ext
				l_temp_t |= MAX_COMMAND_COMBINATION;		// add looptest offset if no command is active
				//l_temp_t |= COM_T;						// add looptest if no command is active
				Scale = ptr_TxData->Scal_Test;
			}
			else
			{
				// ********************************************
				// *** send NSD own guard signal **************
				// ********************************************
				//
				//TempArg[1] = TempArg[0]; // set both arguments equal after a dual tone trip was sent otherwise
										 // the guard signal can be extinguish due to phase shift of 180° of 
										 // TempArg[0] to TempArg[1]
				ptr_TxData->TxSin_0[1] = ptr_TxData->TxSin_0[0];
				ptr_TxData->TxCos_0[1] = ptr_TxData->TxCos_0[0];
			}
			sending = FALSE;
		}
		else
		{
			// ************************************************
			// *** tx trip signal is active send command ******
			// ************************************************
			//
			Scale = ptr_TxData->Scal_Trip;	// set scale factor for trip signal
			//
			if((ptr_TxData->HSD_Type)&&(ptr_TxData->Power_Reduction_activ))
			{	
				// HSD is configured 
				// power reduction activ -> set reduction scale factor for tx command
				Scale = ptr_TxData->Scal_Tx_power_reduction;
			}
			else
			{
				// HSD is not configured OR
				// Power reduction not active
			}
			sending = TRUE;
		//
		}
		//
		// ******************************************************************************
		// *** generate tx signal *******************************************************
		// ******************************************************************************
		//
		Command_Input = l_temp_t;
		//
		for (i= 0; i<NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE;i++)
			{
				ptr_TxData->TxSin_new[0] = ptr_TxData->TxSin_0[0] * ptr_fCos[2*Command_Input] + ptr_TxData->TxCos_0[0] * ptr_fSin[2*Command_Input];
				ptr_TxData->TxCos_new[0] = ptr_TxData->TxCos_0[0] * ptr_fCos[2*Command_Input] - ptr_TxData->TxSin_0[0] * ptr_fSin[2*Command_Input];
				//
				ptr_TxData->TxSin_new[1] = ptr_TxData->TxSin_0[1] * ptr_fCos[2*Command_Input+1] + ptr_TxData->TxCos_0[1] * ptr_fSin[2*Command_Input+1];
				ptr_TxData->TxCos_new[1] = ptr_TxData->TxCos_0[1] * ptr_fCos[2*Command_Input+1] - ptr_TxData->TxSin_0[1] * ptr_fSin[2*Command_Input+1];
			
				
				*Q_Signal++ = ((ptr_TxData->TxSin_new[0] + ptr_TxData->TxSin_new[1])*Scale);	// I-signal
				*I_Signal++ = ((ptr_TxData->TxCos_new[0] + ptr_TxData->TxCos_new[1])*Scale);	// Q-signal

				ptr_TxData->TxSin_0[0] = ptr_TxData->TxSin_new[0];
				ptr_TxData->TxCos_0[0] = ptr_TxData->TxCos_new[0];
				ptr_TxData->TxSin_0[1] = ptr_TxData->TxSin_new[1];
				ptr_TxData->TxCos_0[1] = ptr_TxData->TxCos_new[1];
			 }
	}
 }
 else
 {
   // TPE channel is off -> do nothing
	sending = FALSE;
 }
 return(sending);
}
//
//
//
volatile void AmpCorr(float *ptrSin_00, float *ptrCos_00, int Number)
{
	int i;
	float	f_sin, f_cos;

	for (i=0;i<Number;i++)
	{
		f_sin = *ptrSin_00;
		f_cos = *ptrCos_00;
		// correction algorithmus : Si' = Si*(3/2 - (Si^2 + Ci^2)/(2*A^2))
		//							Ci' = Ci*(3/2 - (Si^2 + Ci^2)/(2*A^2))
		*ptrSin_00++ = f_sin*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
		*ptrCos_00++ = f_cos*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
	}
}

volatile void AmpCorrTx(int Channel)
{
	int i;
	float	f_sin, f_cos;
	float *ptrSin_00, *ptrCos_00;

	if(Channel == TPE1)
	{
		ptrSin_00 = &TxDataCh1.TxSin_0[0];
		ptrCos_00 = &TxDataCh1.TxCos_0[0];
	}
	else
	{
		if(Channel == TPE2)
		{
			ptrSin_00 = &TxDataCh2.TxSin_0[0];
			ptrCos_00 = &TxDataCh2.TxCos_0[0];
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	for (i=0;i<NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE;i++)
	{
		f_sin = *ptrSin_00;
		f_cos = *ptrCos_00;
		// correction algorithmus : Si' = Si*(3/2 - (Si^2 + Ci^2)/(2*A^2))
		//							Ci' = Ci*(3/2 - (Si^2 + Ci^2)/(2*A^2))
		*ptrSin_00++ = f_sin*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
		*ptrCos_00++ = f_cos*(1.5 - ((f_sin*f_sin + f_cos*f_cos)*0.5));
	}
}
