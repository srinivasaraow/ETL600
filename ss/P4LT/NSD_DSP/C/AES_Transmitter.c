/*-------------------------------------------------------------------------*
 * $Workfile: AES_Transmitter.c $
 * Part of      : ETL600 / NSD DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/P4LT/NSD_DSP/C/AES_Transmitter.c $
 * 
 * 14    13.11.08 13:57 Chhamaa
 * Configuration for number of command cycles added.
 * 
 * 13    13.10.08 15:02 Chhamaa
 * - tx signal filtering modified
 * - original sequence of command frequencies of LP cmds (A,B,C,D)
 * re-established
 * 
 * 12    7.10.08 18:08 Chhamaa
 * Sequence of command frequencies of LP cmds (A,B,C,D) changed (lower
 * freq. f3 is sent first) to avoid unwanted commands in case of state
 * transmission of cmd A.
 * 
 * 11    25.08.08 13:14 Chhamaa
 * Tx filter added.
 * 
 * 8     14.11.06 13:24 Chpeplu
 * Function AES_Transmitter_activate() COM_T replaced by TX_COM_T
 * 
 * 7     20.09.06 12:01 Chhamaa
 * AES_Transmitter_localCommandLoop() added.
 * 
 * 6     30.08.06 11:21 Chhamaa
 * 
 * 5     29.08.06 11:35 Chhamaa
 * Defines for AES command frequencies moved to AES600.h.
 * 
 * 4     21.08.06 18:19 Chhamaa
 * Unused code removed.
 * 
 * 3     21.08.06 17:53 Chhamaa
 * Persistent command changed.
 * 
 * 2     10.08.06 18:07 Chhamaa
 * 
 * 1     7.08.06 12:49 Chchlee
 * 
 * 2     21.07.06 17:50 Chhamaa
 * 
 * 1     21.07.06 14:22 Chhamaa
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include <stdlib.h>
#include <math.h>

#include "basetype.h"
#include "AES600.h"
#include "AES_Transmitter.h"
#include "NSD_Transmitter.h"
#include "NSD_DSP_Types.h"
#include "NSD_Types.h"
#include "tdm_ext_types.h"
#include "diag.h"

asm("#include <def21161.h>");
asm("#include <asm_sprt.h>");


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// Define for Trips
#define F1					1
#define F2					2
#define F3					3
#define F4					4
#define F5					5
#define F6					6
#define F7					7
#define F8					8
#define F9					9	

// defines for the Commands to each Command there are 2 Tones with uncoded Commands they are equal!
#define COMMAND_T			( 16* F2 ) + ( F1 ) 		
#define COMMAND_A			( 16* F4 ) + ( F3 )
#define COMMAND_B			( 16* F5 ) + ( F3 )
#define COMMAND_C			( 16* F6 ) + ( F3 )
#define COMMAND_D			( 16* F7 ) + ( F3 )
#define COMMAND_AB			( 16* F8 ) + ( F3 )
#define COMMAND_AC			( 16* F5 ) + ( F4 )
#define COMMAND_AD			( 16* F6 ) + ( F4 )
#define COMMAND_BC			( 16* F7 ) + ( F4 )
#define COMMAND_BD			( 16* F8 ) + ( F4 )
#define COMMAND_CD			( 16* F6 ) + ( F5 )
#define COMMAND_ABC			( 16* F7 ) + ( F5 ) 
#define COMMAND_ABD			( 16* F8 ) + ( F5 )
#define COMMAND_ACD			( 16* F8 ) + ( F6 )
#define COMMAND_BCD			( 16* F7 ) + ( F6 )
#define COMMAND_ABCD		( 16* F8 ) + ( F7 )
#define AES_CMD_1			( 16* F3 ) + ( F2 )
#define AES_CMD_2			( 16* F4 ) + ( F2 )
#define AES_CMD_3			( 16* F5 ) + ( F2 )
#define AES_CMD_4			( 16* F6 ) + ( F2 )
#define AES_CMD_5			( 16* F7 ) + ( F2 )
#define AES_CMD_6			( 16* F8 ) + ( F2 )
#define AES_CMD_7			( 16* F3 ) + ( F1 )
#define AES_CMD_8			( 16* F4 ) + ( F1 )
#define AES_CMD_9			( 16* F5 ) + ( F1 )
#define AES_CMD_10			( 16* F6 ) + ( F1 )
#define AES_CMD_11			( 16* F7 ) + ( F1 )
#define AES_CMD_12			( 16* F8 ) + ( F1 )
#define AES_CMD_13			( 16* F9 ) + ( F1 )
#define AES_CMD_14			( 16* F9 ) + ( F2 )
#define AES_CMD_15			( 16* F3 ) + ( F9 )
#define AES_CMD_16			( 16* F4 ) + ( F9 )
#define AES_CMD_17			( 16* F5 ) + ( F9 )
#define AES_CMD_18			( 16* F6 ) + ( F9 )
#define AES_CMD_19			( 16* F7 ) + ( F9 )
#define AES_CMD_20			( 16* F8 ) + ( F9 )


#define TWO_PI_FS				(TWO_PI/AF_SAMPLE_FREQ)

#define AES_CMD_SHIFT_TO_ZERO	(32-NO_OF_AES_COMMANDS)	
#define AES_CMD_OFFSET			((1<<NO_OF_LP_COMMANDS)-1)
#define GET_AES_COMMANDS		((1<<NO_OF_AES_COMMANDS)-1)
#define GET_LP_COMMANDS			((1<<NO_OF_LP_COMMANDS)-1)

#define LOOP_TEST_OFFSET		((sizeof(commandToTone))-1)
#define	SEND_GUARD				0
#define	TEST_TONE_LEVEL			2.0
#define TX_LEVEL_ADJ_COEF		0.5		// coef to adujust tx signal level

// number of command cycles for signaling modes "normal" and "fast"
#define	N_CMD_CYCLES_NORMAL		3
#define N_CMD_CYCLES_FAST		2

// defines for oscillator
#define MAGIC_NUMBER_AMPL_KORR 	1.5
#define COS_OFFSET				11

// used to shift tx signal to SSB band
#define SSB_FREQ_SHIFT			2150	// [Hz]

// used to shift tx signal to base band
#define FREQ_SHIFT_1000_HZ		1000	// [Hz] signal BW 2.0 kHz (used in case of 12 AES cmd)
#define FREQ_SHIFT_1100_HZ		1100	// [Hz] signal BW 2.2 kHz (used in case of 20 AES cmd)

// tx filter constants
#define N_TX_LP_SEC				2

// tx lp filter coef: ellip(4,0.2,20,2*950/fs)
static float pm txLpFilterCoef950[4*N_TX_LP_SEC + 1] = {

	-4.9966857e-001, 1.2960575e+000, 1.0000000e+000,-6.4541604e-001,
	-8.9778158e-001, 1.5742022e+000, 1.0000000e+000,-1.5412650e+000,
	 1.0361301e-001
};

// tx lp filter coef: ellip(4,0.2,15,2*1120/fs)
static float pm txLpFilterCoef1120[4*N_TX_LP_SEC + 1] = {	

	-4.4733792e-001, 1.1330806e+000, 1.0000000e+000,-5.7753336e-001,
	-9.1272454e-001, 1.4734783e+000, 1.0000000e+000,-1.4561625e+000,
	 1.7437419e-001
};

static const char commandToTone[] = {
	
	0,
	COMMAND_A,
	COMMAND_B,
	COMMAND_AB,
	COMMAND_C,
	COMMAND_AC,
	COMMAND_BC,
	COMMAND_ABC,
	COMMAND_D,
	COMMAND_AD,
	COMMAND_BD,
	COMMAND_ABD,
	COMMAND_CD,
	COMMAND_ACD,
	COMMAND_BCD,
	COMMAND_ABCD,
	AES_CMD_20,
	AES_CMD_19,
	AES_CMD_18,
	AES_CMD_17,
	AES_CMD_16,
	AES_CMD_15,
	AES_CMD_14,
	AES_CMD_13,
	AES_CMD_12,
	AES_CMD_11,
	AES_CMD_10,
	AES_CMD_9,
	AES_CMD_8,
	AES_CMD_7,
	AES_CMD_6,
	AES_CMD_5,
	AES_CMD_4,
	AES_CMD_3,
	AES_CMD_2,
	AES_CMD_1,
	COMMAND_T
};


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {

	float tps_tx_sin;
	float tps_tx_cos;            
	float sin_b;
	float cos_b;
	float amplitude;
	float f_sample;
	float f_transmit;

} t_OscIQType;


typedef struct {

	float nsdGuardOn;
	float toneToSinCosTable[(2*NO_OF_TONES_AES)+4];
	float sinGuardContOff;
	float cosGuardContOff;
	float cmdBoostLevel;
	long maskPersCmd;
	long switchToneInit;
	long doNotChangeTonesInit;
	long pauseCounterInit;
	long lastCmd;
	long nCmdCycles;

	t_OscIQType oscTxSignal;
	t_OscIQType oscFreqShiftSSB;

	float pm *pCoef;
	float dm filterStateI[2*N_TX_LP_SEC];
	float dm filterStateQ[2*N_TX_LP_SEC];

} t_AES_TransmitData;

 
/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static t_AES_TransmitData AES_txData;

// Pointer to Line Protection Tx
static BOOL	(*sendLpCmds)(float*, float*, long);


/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void initCommand2ToneTable(t_NSD_DSP_Config *config);
static void selectLpTx(long signalingTx);
static BOOL sendLpCmdSignaling(float *txSignalI, float *txSignalQ, long command);
static BOOL sendLpCmdState(float *txSignalI, float *txSignalQ, long command);
static long selectAESCmd(long* command, long lastCommand);
static BOOL sendAesCmd(float *txSignalI, float *txSignalQ, long command, BOOL sending);
static long sendTones(float *txSignalI, float *txSignalQ, long command, float boost);

static void initGenIQ(t_OscIQType *osc,float delta_sin, float delta_cos, float amplitude);
static void genIQ(t_OscIQType *osc, float *sinusPtr, float *cosinusPtr);
static void genIQAmpCorr(t_OscIQType *osc);


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		AES_Transmitter_init()		
  Description:	Initializes AES transmitter
  Inputs:		pointer to NSD DSP configuration data		 
  Outputs:		-	
  Return:		-
  Globals:		struct AES_txData
  Important:	Call this function once before using AES_Transmitter_activate().

******************************************************************************/

void SEGMENT_EXT_CODE AES_Transmitter_init(t_NSD_DSP_Config *config)
{
	float dSin, dCos;
	float shiftFreq;
	int i;


	if(config->AES.AES600_Mode & AES600_PERSIST_ON_OFF)
	{
		AES_txData.maskPersCmd = EXTENDED1_G4AI_TRIP_P_BIT;
	}
	else
	{
		AES_txData.maskPersCmd = 0;
	};


	if(config->AES.AES600_Mode & AES600_SIGNALING_MODE)
	{
		AES_txData.nCmdCycles = N_CMD_CYCLES_FAST;
	}
	else
	{
		AES_txData.nCmdCycles = N_CMD_CYCLES_NORMAL;
	};

	initCommand2ToneTable(config);

	// Select the Line Protection TX: Signaling or State Transmission mode
	selectLpTx(config->AES.AES600_Mode & AES600_LP_MODE);

	AES_txData.nsdGuardOn	 = (config->AES.AES600_Mode & AES600_NSD_ETL_GUARD) ? 1 : 0; 
	AES_txData.cmdBoostLevel =  config->AES.BoostLevel; 
	initGenIQ(&AES_txData.oscTxSignal, AES_txData.toneToSinCosTable[0], AES_txData.toneToSinCosTable[COS_OFFSET], config->AES.GuardLevel);

	if ((config->AES.AES600_Mode & AES600_T0) == 0)	// T01
	{
		AES_txData.switchToneInit = N1*(1.3);
		AES_txData.doNotChangeTonesInit = ((N1*0.9)+1);	// 1.3 TO - lowest alpha T0 = 1.3 T0 -0.5 T0 = 0.8 T0

	    // set the pause length between 2 combinations
		if(config->AES.AES600_Mode & AES600_SEC_DEP_CMD)	// increased security
		{
			AES_txData.pauseCounterInit = RX_WNDO1_SEC_HIGH;
		}
		else	// icreased dependability
		{
			AES_txData.pauseCounterInit = RX_WNDO1_SEC_LOW;
		};
	}
	else // T02
	{
		AES_txData.switchToneInit = N2*(1.3); 
		AES_txData.doNotChangeTonesInit = ((N2*0.9)+1);

		// set the pause length between 2 combinations
		if(config->AES.AES600_Mode & AES600_SEC_DEP_CMD)	// increased security			
		{
			AES_txData.pauseCounterInit = RX_WNDO2_SEC_HIGH;
		}
		else	// icreased dependability
		{
			AES_txData.pauseCounterInit = RX_WNDO2_SEC_LOW;
		};
	};

	AES_txData.lastCmd = -1;

	// init oscillator for frequency shift and tx filter
	if ((config->AES.AES600_Mode & AES600_AES_MODE) == 0)
	{
		// 12 AES commands
		shiftFreq = -(SSB_FREQ_SHIFT - FREQ_SHIFT_1000_HZ);
		AES_txData.pCoef = txLpFilterCoef950;
	}
	else
	{
		// 20 AES commands
		shiftFreq = -(SSB_FREQ_SHIFT - FREQ_SHIFT_1100_HZ);
		AES_txData.pCoef = txLpFilterCoef1120;
	}

	// init oscillator
	dSin = sin(TWO_PI_FS*shiftFreq);
	dCos = cos(TWO_PI_FS*shiftFreq);
	initGenIQ(&AES_txData.oscFreqShiftSSB, dSin, dCos, 1.0);

	// init tx filter states
	for (i = 0; i < (2*N_TX_LP_SEC); i++) {
		
		AES_txData.filterStateI[i] = 0;
		AES_txData.filterStateQ[i] = 0;
	}

}


/*****************************************************************************
  Function:		AES_Transmitter_activate()		
  Description:	Transmits coded line protection and AES commands.
  Inputs:		txSignalI, txSignalQ: pointers to complex tx signal samples
				command: tx command word		 
  Outputs:		complex tx signal samples 	
  Return:		TRUE if command is beeing sent, else FALSE
  Globals:		struct AES_txData
  Important:	Call AES_Transmitter_init() once before using this function.

******************************************************************************/

BOOL AES_Transmitter_activate(float *txSignalI, float *txSignalQ, long command)
{
	static BOOL sending = FALSE;
	static float txI[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	static float txQ[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	static float sin[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	static float cos[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	static int i;


	if(command & AES_txData.maskPersCmd)	// persistent cmd on
	{
		AES_txData.toneToSinCosTable[0] = AES_txData.toneToSinCosTable[10];
		AES_txData.toneToSinCosTable[11] = AES_txData.toneToSinCosTable[21];

	}
	else	// persistent cmd off
	{
		AES_txData.toneToSinCosTable[0]  = AES_txData.sinGuardContOff;
		AES_txData.toneToSinCosTable[11] = AES_txData.cosGuardContOff;
	}

	// Send LP command
	sending = sendLpCmds(txI, txQ, command & GET_LP_COMMANDS);
	// Send AES command
	sending = sendAesCmd(txI, txQ, (command >> AES_CMD_SHIFT_TO_ZERO) & GET_AES_COMMANDS, sending);

	if(!sending)
	{
		if(command & TX_COM_T)
		{
			// Send Looptest
			sendTones(txI, txQ, LOOP_TEST_OFFSET, TEST_TONE_LEVEL);	// with NSD_Guard
		}
		else
		{
			// Send Guard
			sendTones(txI, txQ, SEND_GUARD, AES_txData.nsdGuardOn);
		}
	}

	// shift base band signal to SSB band  
	genIQ(&AES_txData.oscFreqShiftSSB, &sin[0], &cos[0]); 
	genIQ(&AES_txData.oscFreqShiftSSB, &sin[1], &cos[1]);
	genIQAmpCorr(&AES_txData.oscFreqShiftSSB);

	txSignalQ[0] = txI[0]*sin[0] + txQ[0]*cos[0];
	txSignalI[0] = txI[0]*cos[0] - txQ[0]*sin[0];
	txSignalQ[1] = txI[1]*sin[1] + txQ[1]*cos[1];
	txSignalI[1] = txI[1]*cos[1] - txQ[1]*sin[1];

	return(sending);

}


/*****************************************************************************
  Function:		AES_Transmitter_localCommandLoop()		
  Description:	Converts a tx cmd word to a rx cmd word
  Inputs:		cmd: tx command word		 
  Outputs:		-	
  Return:		rx command word
  Globals:		-
  Important:	-

******************************************************************************/

long SEGMENT_EXT_CODE AES_Transmitter_localCommandLoop(long cmd)
{
	unsigned long AesCmd = 0;

	// Get most inportant AES command
	AesCmd = (cmd>>AES_CMD_SHIFT_TO_ZERO)& GET_AES_COMMANDS;
	asm("%0 = EXP %1;" : "=d" (AesCmd) : "d" (AesCmd));
	AesCmd += 31;

	// Convert form G4AI->P4LQ to P4LQ->G4AI  
	// Here is the conversion:
	// 20 -> 0x0800		== 1* 0x0800
	// 19 -> 0x01000	== 2* 0x0800
	// 18 -> 0x01000	== 3* 0x0800
	if(AesCmd)
	{
		AesCmd = ((21-AesCmd) * RX_AES_1);
	}
	else
	{
		AesCmd = 0;
	}

	AesCmd |= (cmd & (ALL_LP_COMMANDS | EXTENDED1_G4AI_TRIP_P_BIT)); 
	
	if(AesCmd)
	{
		AesCmd |= (EXTENDED1_G4AI_AES_TX_CMD_ACK_SUM | EXTENDED1_G4AI_AES_RX_CMD_ACK_SUM); 
		return AesCmd;
	}
	else
	{
		return 0;
	}

}


static void SEGMENT_EXT_CODE initCommand2ToneTable(t_NSD_DSP_Config *config)
{
	long guardShift;
	long txFreqShift;

	
	if (config->AES.AES600_Mode & AES600_PERSIST_ON_OFF)
	{
		// persistent command is switched on -> do guard shift
		guardShift = GUARD_FREQ_SHIFT;
	}
	else
	{
		guardShift = 0;
	};

	// set txFreqShift used to shift tx signal to base band
	if ((config->AES.AES600_Mode & AES600_AES_MODE) == 0)
	{
		// 12 AES commands
		txFreqShift = FREQ_SHIFT_1000_HZ;
	}
	else
	{
		// 20 AES commands
		txFreqShift = FREQ_SHIFT_1100_HZ;
	}

	AES_txData.sinGuardContOff = sin(TWO_PI_FS * -(txFreqShift - config->AES.PilotFrequency+guardShift));
	AES_txData.cosGuardContOff = cos(TWO_PI_FS * -(txFreqShift - config->AES.PilotFrequency+guardShift));
	
	// NSD pilot sin
	AES_txData.toneToSinCosTable[0] = sin(TWO_PI_FS * -(txFreqShift - config->AES.PilotFrequency+guardShift));

	// Detektor 1-8 sin
	AES_txData.toneToSinCosTable[1] = sin(TWO_PI_FS * -(txFreqShift - FREQU1));
	AES_txData.toneToSinCosTable[2] = sin(TWO_PI_FS * -(txFreqShift - FREQU2));
	AES_txData.toneToSinCosTable[3] = sin(TWO_PI_FS * -(txFreqShift - FREQU3));

	AES_txData.toneToSinCosTable[4] = sin(TWO_PI_FS * -(txFreqShift - FREQU4));
	AES_txData.toneToSinCosTable[5] = sin(TWO_PI_FS * -(txFreqShift - FREQU5));
	AES_txData.toneToSinCosTable[6] = sin(TWO_PI_FS * -(txFreqShift - FREQU6));

	AES_txData.toneToSinCosTable[7] = sin(TWO_PI_FS * -(txFreqShift - FREQU7));
	AES_txData.toneToSinCosTable[8] = sin(TWO_PI_FS * -(txFreqShift - FREQU8));
	AES_txData.toneToSinCosTable[9] = sin(TWO_PI_FS * -(txFreqShift - FREQU9));

	AES_txData.toneToSinCosTable[10] = sin(TWO_PI_FS * -(txFreqShift - config->AES.PilotFrequency-guardShift));

	// NSD pilot cos
	AES_txData.toneToSinCosTable[11] = cos(TWO_PI_FS * -(txFreqShift - config->AES.PilotFrequency+guardShift));
	
	AES_txData.toneToSinCosTable[12] = cos(TWO_PI_FS * -(txFreqShift - FREQU1));
	AES_txData.toneToSinCosTable[13] = cos(TWO_PI_FS * -(txFreqShift - FREQU2));
	AES_txData.toneToSinCosTable[14] = cos(TWO_PI_FS * -(txFreqShift - FREQU3));

	AES_txData.toneToSinCosTable[15] = cos(TWO_PI_FS * -(txFreqShift - FREQU4));
	AES_txData.toneToSinCosTable[16] = cos(TWO_PI_FS * -(txFreqShift - FREQU5));
	AES_txData.toneToSinCosTable[17] = cos(TWO_PI_FS * -(txFreqShift - FREQU6));

	AES_txData.toneToSinCosTable[18] = cos(TWO_PI_FS * -(txFreqShift - FREQU7));
	AES_txData.toneToSinCosTable[19] = cos(TWO_PI_FS * -(txFreqShift - FREQU8));
	AES_txData.toneToSinCosTable[20] = cos(TWO_PI_FS * -(txFreqShift - FREQU9));

	AES_txData.toneToSinCosTable[21] = cos(TWO_PI_FS * -(txFreqShift - config->AES.PilotFrequency-guardShift));
}


static void SEGMENT_EXT_CODE selectLpTx(long signalingTx)
{
	if(signalingTx)
	{
		sendLpCmds = sendLpCmdSignaling;
	}
	else
	{
		sendLpCmds = sendLpCmdState;
	}
}


static BOOL sendLpCmdSignaling(float *txSignalI, float *txSignalQ, long command)
{
	static long ToneCyle		= 0;
	static long LastCommand		= 0;
	static long CommandBuffer	= 0;

	// Detect positive slope
	// This if is necessary because if ToneCycle is >3 and a new command
	// Appears we may loose this command
	if(command & (command ^ LastCommand))
	{	// Update CommandBuffer and Start new ToneCycle
		CommandBuffer |= (command & (command ^ LastCommand));
		ToneCyle = 0;
	}

	LastCommand = command;
	
	if(CommandBuffer)
	{
		if(ToneCyle < AES_txData.nCmdCycles)
		{
			ToneCyle = sendTones(txSignalI, txSignalQ, CommandBuffer, AES_txData.cmdBoostLevel);
			return(TRUE);
		}
		else
		{	// Commands are sent now
			// Reset Buffer and Tone cycle
			CommandBuffer = 0;
			ToneCyle = 0;
			return(FALSE);
		}
	
	}
	else
	{
		return(FALSE);
	}
}


static BOOL sendLpCmdState(float *txSignalI, float *txSignalQ, long command)
{
	if(command)			
	{
		sendTones(txSignalI, txSignalQ, command & GET_LP_COMMANDS, AES_txData.cmdBoostLevel);
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}


/****************************************************/
/*	This function clears the old AES command		*/
/*	and selects the new one.						*/
/*  The parameters are transfered:					*/
/*  r0 = selectAESCmd( r4, r8)						*/	
/*	Command the contains all pending AES commands	*/
/*	coded in a bitmap (0 to 0x0FFFFF)				*/
/*	lastCommand contains the number of the last		*/
/*	command e.g. 0 - 20								*/
/*	The return value will be the number of the next */
/*	AES command	(0 to 20)							*/
/*													*/
/*	Created: 1.2.00 Si								*/
/*													*/
/****************************************************/
// return values are in reverse order:	AES cmd 1  -> return value: 20
//										AES cmd 4  -> return value: 17
//										AES cmd 20 -> return value:  1
					
static long selectAESCmd(long* command, long lastCommand)
{
		
	asm("		i4 = r4;			");		// Copy pointer to command to a indexregister
	asm("		r4 = dm(i4,0);		");		// Load R4 with command
	asm("		r8 = r8 -1;			");		// decrement lastCommand
	asm("		r4 = BCLR r4 BY R8;	");		// Clear the last command in the command word
											// note: if r8 is not between 0 and 31 no bits 
											// are cleard (vy handy!!)
	asm("		dm(i4,0) = r4;		");		// store the command
	asm("		r4 = EXP r4;		");		// Get the leading bit (or better: the tows complent
											// is stored in the shf8 field
	asm("		r8 = 31;			");		// add the offset to make is a positive value
	asm("		r0 = r4 + r8;		");		// the value is returned in r0


/*	//The way we do this in the PC
	(*command) &= (~(1<<(lastCommand-1)));
	if(*command)
		return ((long) (log10(*command)/log10(2))) +1;
	else
		return 0;
*/
}


static BOOL sendAesCmd(float *txSignalI, float *txSignalQ, long command, BOOL sending)
{
	static long ToneCyle = 4;
	static long ActualCommand = 0;
	static long CommandBuffer = 0;
	static long LastCommand = 0;

	// Get the AES command
		// Detect positive slope
	CommandBuffer |= (command & (command ^ LastCommand));
	LastCommand = command;
	
	if(sending)
	{
		ToneCyle = 4;			// start form the beginning
		ActualCommand = 0;		// make shure that we dont clear a command
		return(sending);
	}
	else
	{
		if(!CommandBuffer)
		{
			return(sending);
		}
		else
		{
			if(ToneCyle < AES_txData.nCmdCycles)
			{
				ToneCyle = sendTones(txSignalI, txSignalQ, ActualCommand+AES_CMD_OFFSET, AES_txData.cmdBoostLevel);
				return(TRUE);
			}
			else
			{
				// clear the last command and select an new one
				ActualCommand = selectAESCmd(&CommandBuffer, ActualCommand);
				if(ActualCommand)
				{
					ToneCyle = sendTones(txSignalI, txSignalQ, ActualCommand+AES_CMD_OFFSET, AES_txData.cmdBoostLevel);
					return(TRUE);
				}
				else
				{
					return(FALSE);
				}
			}
		}
	}
}


static long sendTones(float *txSignalI, float *txSignalQ, long command, float boost)
{
	static float sin[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	static float cos[NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE];
	static float Last_boost = 0.5;
	static long	SwitchToneCounter = 0;
	static long	ToneCyleNumber = 0;
	static long ToneCombination = 0;
	static long PauseCounter = -1;
	static long i;


	if(AES_txData.lastCmd != command)
	{

		if(((AES_txData.lastCmd == 0) &&			// detect short Guard transmissions
			(ToneCyleNumber == 0) &&
			(PauseCounter<1)) ||
			// detect change from command x to command y
		   ((AES_txData.lastCmd != 0) &&
			(command !=	0) &&
			!((ToneCyleNumber == 0)	&& (SwitchToneCounter > AES_txData.doNotChangeTonesInit)) &&
			(PauseCounter<1)))

		{
			PauseCounter = AES_txData.pauseCounterInit;
		}
		SwitchToneCounter = AES_txData.switchToneInit;
		ToneCyleNumber = 0;
	}

	AES_txData.lastCmd = command;
	Last_boost = boost;

	if(PauseCounter>0)
	{
		PauseCounter--;
		//Send guard with -6dBm0
		AES_txData.oscTxSignal.sin_b = AES_txData.sinGuardContOff;		//send guard signal between commands ToneToSinCosTable[0];
		AES_txData.oscTxSignal.cos_b = AES_txData.cosGuardContOff;		//send guard signal between commands ToneToSinCosTable[COS_OFFSET];
		Last_boost = 1;		// 1 == -6dBm0
	}
	else
	{
		// send one or another command
		ToneCombination = commandToTone[AES_txData.lastCmd];
		if(--SwitchToneCounter<0)
		{
				AES_txData.oscTxSignal.sin_b = AES_txData.toneToSinCosTable[(ToneCombination&0x0F)];
				AES_txData.oscTxSignal.cos_b = AES_txData.toneToSinCosTable[((ToneCombination&0x0F)+COS_OFFSET)];
		}
		else
		{
				AES_txData.oscTxSignal.sin_b = AES_txData.toneToSinCosTable[(ToneCombination&0xF0)>>4];
				AES_txData.oscTxSignal.cos_b = AES_txData.toneToSinCosTable[(((ToneCombination&0xF0)>>4)+COS_OFFSET)];
		}

		// Handle underflow of SwitchToneCounter
		if(SwitchToneCounter <= -AES_txData.switchToneInit)
		{
			SwitchToneCounter = AES_txData.switchToneInit;
			ToneCyleNumber++;
		}
	}

	genIQAmpCorr(&AES_txData.oscTxSignal);  
	genIQ(&AES_txData.oscTxSignal,&sin[0],&cos[0]); 
	genIQ(&AES_txData.oscTxSignal,&sin[1],&cos[1]);

	// tx filter if command is sent
	if ((PauseCounter < 1) && (AES_txData.lastCmd > 0))
	{
		BiIIR(sin, sin, AES_txData.pCoef, AES_txData.filterStateI, N_TX_LP_SEC, NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE);
		BiIIR(cos, cos, AES_txData.pCoef, AES_txData.filterStateQ, N_TX_LP_SEC, NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE);
	}
	else	// guard is sent: no tx filter
	{
		// reset filter states
		for (i = 0; i < (2*N_TX_LP_SEC); i++) {
		
			AES_txData.filterStateI[i] = 0;
			AES_txData.filterStateQ[i] = 0;
		}
	}

	txSignalQ[0] = Last_boost*TX_LEVEL_ADJ_COEF*sin[0];
	txSignalI[0] = Last_boost*TX_LEVEL_ADJ_COEF*cos[0];
	txSignalQ[1] = Last_boost*TX_LEVEL_ADJ_COEF*sin[1];
	txSignalI[1] = Last_boost*TX_LEVEL_ADJ_COEF*cos[1];

	return ToneCyleNumber;
}


static void SEGMENT_EXT_CODE initGenIQ(t_OscIQType *osc,float delta_sin, float delta_cos, float amplitude)
{
	osc->tps_tx_sin = 0.0;
	osc->tps_tx_cos = amplitude;
	osc->sin_b = delta_sin;
	osc->cos_b = delta_cos;
	osc->amplitude = amplitude;
}


static void genIQ(t_OscIQType *osc, float *sinusPtr, float *cosinusPtr )
{
	asm("puts = r12;");               		/* cosinusPtr*/
	asm("puts = r8;");               		/* sinusPtr*/
	asm("puts = r4;");               		/* oscPtr */
    asm("     bit clr mode1 IRPTEN;		");
	asm("     bit set mode1 SRRFH|SRRFL;"); /* use secondary registers */
	asm("     nop;						");
	asm("     r2 = gets(1);				");	/*  use oscPtr in secondary registers */
	asm("     i4 = r2;");
	asm("     f1 = dm(0,i4);			");	/*  tps_tx_sin ,sin a    */
	asm("     f3 = dm(1,i4);			"); /*  tps_tx_cos, cos a   */
	asm("     f4 = dm(2,i4);			"); /*  sin_b          */
	asm("     f7 = dm(3,i4);			"); /*  cos_b          */
	asm("     f11= f1*f7;             	"); /* sin a * cos b */
	asm("     f15= f3*f4;           	"); /* cos a * sin b */
	asm("     f6 = f3*f7,f9 = f11+f15; 	"); /* cos a * cos b, ->sin(a+b)  */
	asm("     f11= f1*f4,f1 = f9;		");
	asm("     f3 = f6-f11;				");
	asm("     r2 = gets(2);				"); /* get sinusPtr */
	asm("     i4 = r2;      			");
	asm("     dm(i4,0) = f1;   			");	/* save sin(a+b) to sinusPtr */
	asm("     r2 = gets(3);				");	/* get cosinusPtr */
	asm("     i4 = r2;      			");                 
	asm("     dm(i4,0) = f3;         	");	/* save cos(a+b) to cosinusPtr */
	asm("     r2 = gets(1);        		");	/* get oscPtr */
	asm("     i4 = r2;             		");
	asm("     dm(0,i4) = f1;			"); /* new tps_tx_sin     */
	asm("     dm(1,i4) = f3;			"); /* new tps_tx_cos;    */
	asm("	  bit clr mode1 SRRFH|SRRFL;"); /* use primary registers */
    asm("     bit set mode1 IRPTEN;		");
    asm("     nop;");
}


static void genIQAmpCorr(t_OscIQType *osc)
{
	float h;
	static float amplitude_old = 0, korr_faktor;

	if ( amplitude_old != osc->amplitude)
	 {
	    korr_faktor = 1/(N_2 * (osc->amplitude) * (osc->amplitude)); 
		amplitude_old = osc->amplitude;
	 }
	// dieser IF koennet eingespart werden
	if (0 == (h = (osc->tps_tx_sin) * (osc->tps_tx_sin) + (osc->tps_tx_cos) * (osc->tps_tx_cos)))
	 {
		osc->tps_tx_sin = 0;
		osc->tps_tx_cos = osc->amplitude;
	 }
	else
	 {                   
		h = MAGIC_NUMBER_AMPL_KORR - korr_faktor * h;
		if (0. > h) h = 0.;
		osc->tps_tx_sin = h * osc->tps_tx_sin;
		osc->tps_tx_cos = h * osc->tps_tx_cos;
	 }
}
