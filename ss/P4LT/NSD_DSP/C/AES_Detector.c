/*-------------------------------------------------------------------------*
 * $Workfile: AES_Detector.c $
 * Part of      : ETL600 / NSD DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSD_DSP/C/AES_Detector.c $
 * 
 * 12    13-09-24 16:32 Chhamaa
 * AESDet.WindowLength decreased to original value in order to avoid
 * unwanted commands during commande sequences (issue 516).
 * Unfortunately, this solution is more susceptible to single tone
 * interferer.
 * 
 * 9     08-04-23 8:46 Chtognu
 * AESDet.WindowLength doubled and detector windows reset criterion
 * reduced to "guard active and lastDetector not active". Done to activate
 * the Com_MixUpBit criterion and to increase security against interferer
 * on trip tone frequencies. Trip level difference monitoring added but
 * not activated marked as "triplevelcheck"
 * 
 * 8     21.11.07 14:02 Chtognu
 * removed unused variable, comments added, "detector active"
 * signalisation added also for Trip detection, trip and guard check added
 * again (can not be done outside this function cause of guard sequence
 * inbetween trips) (checked in to AES branch: 5.09.07)
 * 
 * 7     28.08.07 9:08 Chtognu
 * AES: boolian added to AES_Detector_activate() for "detector active"
 * signalisation --> used for modified Trip and Guard Alarm. Trip and
 * Guard check removed.
 * 
 * 6     17.04.07 11:17 Chhamaa
 * Update from AES branch
 * 
 * 7     29.03.07 17:37 Chhamaa
 * Updating of SpeechPowerCh1[] for NSDUnblocking() added.
 * 
 * 6     22.02.07 14:18 Chhamaa
 * COS_OFFSET renamed: SINE_TAB_COS_OFFSET
 * 
 * 5     21.02.07 11:55 Chtognu
 * receiver modifications (bug 407) pointer exeded array length. new --> 
 * if(*PtrTempArgument >= SINE_TAB_LENGTH) instead 
 * if(*PtrTempArgument > SINE_TAB_LENGTH)
 * additional modifications 
 * 
 * 4     14.11.06 13:28 Chpeplu
 * Loop Test define (COM_T) changed in COMTABLE 1 and 2 for AES600 to
 * RX_COM_T
 * 
 * 3     5.09.06 16:08 Chhamaa
 * Wide band power calculation modified in function
 * AES_Detector_activate()
 * 
 * 2     29.08.06 18:08 Chhamaa
 * Code cleaned up.
 * 
 * 1     29.08.06 11:33 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
  
/* INCLUDE FILES *************************************************************/

#include "CONSTANT.H"
#include "basetype.h"
#include "diag.h"

#include "AES600.h"
#include "AES_Detector.h"
#include "AES_LP_FilterCoef.h"
#include "AES_SineTable.h"
#include "NSD_DSP_Types.h"
#include "NSD_Types.h"
#include "NSD_Monitor.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define NO_OF_TEV				3			// used only 2

#define RX_AF_BLOCK_LENGTH		2			// samples processed in one DSP cycle

// definese the delta argument for sin calculation with tabel 256
#define D_ARGFREQ1 (256*FREQU1*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ2 (256*FREQU2*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ3 (256*FREQU3*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ4 (256*FREQU4*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ5 (256*FREQU5*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ6 (256*FREQU6*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ7 (256*FREQU7*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ8 (256*FREQU8*2/(AF_SAMPLE_FREQ))
#define D_ARGFREQ9 (256*FREQU9*2/(AF_SAMPLE_FREQ))

// IF WIDEBANDPOWER < MINPOWER THEN Input for detector = 0.0
// Ref: Command A POWER1 = 0.7 == +1.2dB
// Threshold MINPOWER == -25dB  
#define MINPOWER	0.00168	
// Threshold MINPOWER == -35dB
#define MINPOWERSET	0.000168

// Max trip level difference of the two trips = 12dB
// triplevelcheck #define MAXTRIPLEVELDIF 15.8

// command prolongation to compensate evaluation time of coded commands
#define COM_PRE1	((29 + RX_WNDO1_SEC_LOW) + (2.0 *N1))
#define COM_PRE2	((48 + RX_WNDO2_SEC_LOW) + (2.0 *N2))
#define COM_PRE3	((60 + RX_WNDO3_SEC_LOW) + (2.0 *N3))

#define	COLUMN_WIDH 	10 

//***************************************************************************
//*** AES Code table for 12 Commands  ***************************************
//*************************************************************************** 
static long COMTAB_1[(NO_OF_TONES_AES + 1)*(NO_OF_TONES_AES +1)] = {

 	0,    0,          0,	  0,	  0,		  0,          0,		  0,          0,		0,	// uncoded
 	0,    0,      RX_COM_T,RX_AES_7,RX_AES_8,  RX_AES_9,  RX_AES_10,  RX_AES_11,  RX_AES_12,	0,  // F1
 	0,  RX_COM_T,     0,  RX_AES_1,RX_AES_2,  RX_AES_3,	  RX_AES_4,	  RX_AES_5,   RX_AES_6,		0,	// F2
 	0, RX_AES_7, RX_AES_1,    0,    COM_A,     COM_B,      COM_C,      COM_D,      COM_AB,		0,	// F3
 	0, RX_AES_8, RX_AES_2, COM_A,     0,       COM_AC,	   COM_AD,     COM_BC,	   COM_BD,		0,	// F4
 	0, RX_AES_9, RX_AES_3, COM_B,  COM_AC,        0,       COM_CD,     COM_ABC,    COM_ABD,		0,	// F5
 	0,RX_AES_10, RX_AES_4, COM_C,  COM_AD,	   COM_CD,        0,       COM_BCD,	   COM_ACD,		0,	// F6
 	0,RX_AES_11, RX_AES_5, COM_D,  COM_BC,     COM_ABC,    COM_BCD,       0,      COM_ABCD,		0,	// F7
 	0,RX_AES_12, RX_AES_6, COM_AB, COM_BD,	   COM_ABD,	   COM_ACD,   COM_ABCD,       0,		0,	// F8 
	0,	  0,          0,      0,	  0,		  0,	      0,		  0,		  0,		0	// F9
};                                   
//  -     F1          F2      F3      F4          F5          F6          F7          F8		F9  
// 

//***************************************************************************
//*** AES Code table for 20 Commands  ***************************************
//*************************************************************************** 
static long COMTAB_2[(NO_OF_TONES_AES + 1)*(NO_OF_TONES_AES +1)] = {

 	0,    0,          0,	  0,	  0,		  0,          0,		  0,          0,		0,		// uncoded
 	0,    0,      RX_COM_T,RX_AES_7,RX_AES_8,  RX_AES_9,  RX_AES_10,  RX_AES_11,  RX_AES_12,RX_AES_13,  // F1
 	0, RX_COM_T,      0,  RX_AES_1,RX_AES_2,  RX_AES_3,	  RX_AES_4,	  RX_AES_5,   RX_AES_6, RX_AES_14,	// F2
 	0, RX_AES_7, RX_AES_1,    0,    COM_A,     COM_B,      COM_C,      COM_D,      COM_AB,	RX_AES_15,	// F3
 	0, RX_AES_8, RX_AES_2, COM_A,     0,       COM_AC,	   COM_AD,     COM_BC,	   COM_BD,  RX_AES_16,	// F4
 	0, RX_AES_9, RX_AES_3, COM_B,  COM_AC,        0,       COM_CD,     COM_ABC,    COM_ABD, RX_AES_17,	// F5
 	0,RX_AES_10, RX_AES_4, COM_C,  COM_AD,	   COM_CD,        0,       COM_BCD,	   COM_ACD,	RX_AES_18,	// F6
 	0,RX_AES_11, RX_AES_5, COM_D,  COM_BC,     COM_ABC,    COM_BCD,       0,      COM_ABCD,	RX_AES_19,	// F7
 	0,RX_AES_12, RX_AES_6, COM_AB, COM_BD,	   COM_ABD,	   COM_ACD,   COM_ABCD,       0,	RX_AES_20,	// F8 
	0,RX_AES_13,RX_AES_14,RX_AES_15,RX_AES_16,RX_AES_17, RX_AES_18,	 RX_AES_19,	 RX_AES_20,		0		// F9
};                                   
//  -     F1          F2     F3      F4          F5          F6          F7           F8		F9  
// 


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {

	long	CommandEvalTime;
	long	TonesActive;
	long	DetFlag;
    long	WindowLength;
    long	WinCounter1;
    long	WinCounter2;
    long	Det_Win1;
    long	Det_Win2;
	long	Det_Last_Win1;
	long	Det_Last_Win2;
	long	Com_MixUpBit;
	long	Com_MixUpBitCounter;
    long	*PtrCOMTAB;
    long	HoldTimeCoded;
    long	HoldCounterPreset;
    long	HoldCounter;
    long	*Ptr_Com;
    long	ActiveCommand;
    float	WidePowerAndGuardCod;
    float	AlphaCoded;
    float	OneMinusAlphaCod;
    float	*PowerAdrCoded;

	float	*ptr_sumN1;
	float	*ptr_sumN2;
	float	*ptr_sumN3;
	float	Power_Verif[NO_OF_TEV];
	float	NCorrePower[NO_OF_TEV];
	float	NCorreDet;
	float	SinSum[NO_OF_TONES_AES];
	float	CosSum[NO_OF_TONES_AES];
	float	VerifSinCosSum[NO_OF_TONES_AES*2];

	float	*PtrSumSinCos[(NO_OF_TONES_AES) *2];
	float	*endptrSumSinCos[(NO_OF_TONES_AES) *2];
	float	*startptrSumSinCos[(NO_OF_TONES_AES)*2];

	float	DeltaArg[NO_OF_TONES_AES + 1];
	float	TempArgument[NO_OF_TONES_AES + 1];
	
	float	Sin_Value[NO_OF_TONES_AES + 1];
	float	Cos_Value[NO_OF_TONES_AES + 1]; 

	float	*ptrLpFilterState;
	float	*ptrLpCoef;

	t_NSD_Power_Levels *ptr_NSD_Power_Speech_Guard;
	
	// from NSDStatusData
	float	OutputWideBand[NO_OF_TEV];
	float	OutputDet[NO_OF_TONES_AES];	// current detector level
	float	TimeDet[NO_OF_TONES_AES];   // detector activ time in ms
	long	LastDetector[2];			// the last two active detectors

// triplevelcheck	float	tempDetectorPower;
// triplevelcheck	float	DetectorPowerWin1;
// triplevelcheck	float	DetectorPowerWin2;
	
} t_AES_DetectorData;


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static t_AES_DetectorData AESDet;

static float dm LP_2kHz_FilterState1[LPF_SARRAY_LENGTH];

static float SEGMENT_EXT_DATA PowerArray_N1[N1], PowerArray_N2[N2], PowerArray_N3[N3];

static float SEGMENT_EXT_DATA SumSinF1[N3], SumCosF1[N3], SumSinF2[N3], SumCosF2[N3], SumSinF3[N3], SumCosF3[N3];
static float SEGMENT_EXT_DATA SumSinF4[N3], SumCosF4[N3], SumSinF5[N3], SumCosF5[N3], SumSinF6[N3], SumCosF6[N3];
static float SEGMENT_EXT_DATA SumSinF7[N3], SumCosF7[N3], SumSinF8[N3], SumCosF8[N3], SumSinF9[N3], SumCosF9[N3];

static long	Failure_Counter, Counter_set;		// for test only


/* LOCAL MACRO DEFINITIONS ***************************************************/

#define SQUARE(x) ((x)*(x))


/* IMPLEMENTATION ************************************************************/
		
/*****************************************************************************
  Function:		void AES_Detector_init()	
  Description:	Initialization of AES command detector data.
  Inputs:		pointer to NSD DSP configuration data		
  Outputs:		-		
  Return:		-		
  Globals:		struct AESDet
  Important:	This function has to be called once before using AES_Detector_activate()

*******************************************************************************/

void SEGMENT_EXT_CODE AES_Detector_init(t_NSD_DSP_Config *config)
{ 
	int	i,j;
	float *tempptr;


	// for tests only
	Failure_Counter  = 0;
	Counter_set = 0;

	// set pointer to filter state vector
	AESDet.ptrLpFilterState = LP_2kHz_FilterState1;

	// init LP-Filter
	for (i = 0; i < LPF_SARRAY_LENGTH; i++)
	{
		// reset filter state values
		AESDet.ptrLpFilterState[i] = 0.0;
	}

	AESDet.OutputWideBand[0] = 0.0;		// Reset power
	AESDet.OutputWideBand[1] = 0.0;
	AESDet.OutputWideBand[2] = 0.0;
     
	// Reset Detector outputs
	for (i = 0; i < NO_OF_TONES_AES; i++) 	
	{
		AESDet.OutputDet[i] = 0.0;		// reset detector Outputs
	}

	// Reset PowerArrays for N1, N2 and N3 
	AESDet.ptr_sumN1 = PowerArray_N1;
	for (i = 0; i < N1; i++)
	{
		PowerArray_N1[i] = 0.0;			// Reset PowerArray N1
	}

	AESDet.ptr_sumN2 = PowerArray_N2;
	for (i = 0; i < N2; i++)
	{
		PowerArray_N2[i] = 0.0;        	// Reset PowerArray N2
	}

	AESDet.ptr_sumN3 = PowerArray_N3;
	for (i = 0; i < N3; i++)
	{
		PowerArray_N3[i] = 0.0;       	// Reset PowerArray N3
	}

	//	Init TempArgument to zero  
	for (i = 0; i < NO_OF_TONES_AES; i++)
	{
		AESDet.TempArgument[i] = 0;
	}

	//  set delta argument to for commands and guard
	AESDet.DeltaArg[0] = D_ARGFREQ1;
	AESDet.DeltaArg[1] = D_ARGFREQ2;
	AESDet.DeltaArg[2] = D_ARGFREQ3;
	AESDet.DeltaArg[3] = D_ARGFREQ4;
	AESDet.DeltaArg[4] = D_ARGFREQ5;
	AESDet.DeltaArg[5] = D_ARGFREQ6;
	AESDet.DeltaArg[6] = D_ARGFREQ7;
	AESDet.DeltaArg[7] = D_ARGFREQ8;
	AESDet.DeltaArg[8] = D_ARGFREQ9;

	AESDet.PtrSumSinCos[0] = SumSinF1;
	AESDet.PtrSumSinCos[1] = SumCosF1;
	AESDet.PtrSumSinCos[2] = SumSinF2;
	AESDet.PtrSumSinCos[3] = SumCosF2;
	AESDet.PtrSumSinCos[4] = SumSinF3;
	AESDet.PtrSumSinCos[5] = SumCosF3;
	AESDet.PtrSumSinCos[6] = SumSinF4;
	AESDet.PtrSumSinCos[7] = SumCosF4;
	AESDet.PtrSumSinCos[8] = SumSinF5;
	AESDet.PtrSumSinCos[9] = SumCosF5;
	AESDet.PtrSumSinCos[10] = SumSinF6;
	AESDet.PtrSumSinCos[11] = SumCosF6;
	AESDet.PtrSumSinCos[12] = SumSinF7;
	AESDet.PtrSumSinCos[13] = SumCosF7;
	AESDet.PtrSumSinCos[14] = SumSinF8;
	AESDet.PtrSumSinCos[15] = SumCosF8;
	AESDet.PtrSumSinCos[16] = SumSinF9;
	AESDet.PtrSumSinCos[17] = SumCosF9;

	AESDet.startptrSumSinCos[0] = SumSinF1;
	AESDet.startptrSumSinCos[1] = SumCosF1;
	AESDet.startptrSumSinCos[2] = SumSinF2;
	AESDet.startptrSumSinCos[3] = SumCosF2;
	AESDet.startptrSumSinCos[4] = SumSinF3;
	AESDet.startptrSumSinCos[5] = SumCosF3;
	AESDet.startptrSumSinCos[6] = SumSinF4;
	AESDet.startptrSumSinCos[7] = SumCosF4;
	AESDet.startptrSumSinCos[8] = SumSinF5;
	AESDet.startptrSumSinCos[9] = SumCosF5;
	AESDet.startptrSumSinCos[10] = SumSinF6;
	AESDet.startptrSumSinCos[11] = SumCosF6;
	AESDet.startptrSumSinCos[12] = SumSinF7;
	AESDet.startptrSumSinCos[13] = SumCosF7;
	AESDet.startptrSumSinCos[14] = SumSinF8;
	AESDet.startptrSumSinCos[15] = SumCosF8;
	AESDet.startptrSumSinCos[16] = SumSinF9;
	AESDet.startptrSumSinCos[17] = SumCosF9;

	// Init operation mode
	if ((config->AES.AES600_Mode & AES600_AES_MODE) == 0)
	{
		// 12 AES commands
		AESDet.PtrCOMTAB = COMTAB_1;
		AESDet.ptrLpCoef = LP2k0HzCoef;		// LP-Filter Coef for 2.0kHz LP-Filter 
		AESDet.TonesActive = NO_OF_TONES_AES - 1;
	}
	else
	{
		// 20 AES commands
		AESDet.PtrCOMTAB = COMTAB_2;
		AESDet.ptrLpCoef = LP2k2HzCoef;		// LP-Filter Coef for 2.2kHz LP-Filter
		AESDet.TonesActive = NO_OF_TONES_AES;
	};

	// set Alpha-Values and Integration times
	if ((config->AES.AES600_Mode & AES600_T0) == 0)	// T01
	{
		AESDet.HoldTimeCoded = COM_PRE1;
		AESDet.CommandEvalTime = N1;
		AESDet.PowerAdrCoded = &AESDet.OutputWideBand[0];

		if ((config->AES.AES600_Mode & AES600_SEC_DEP_CMD) == 0)	// security low
		{
			AESDet.WindowLength = RX_WNDO1_SEC_LOW;
			AESDet.AlphaCoded = ALPHA_N1_LOW;
			AESDet.OneMinusAlphaCod = 1.0 - ALPHA_N1_LOW;
		}
		else	// security high
		{
			AESDet.WindowLength = RX_WNDO1_SEC_HIGH;
			AESDet.AlphaCoded = ALPHA_N1_HIGH;
			AESDet.OneMinusAlphaCod = 1.0 - ALPHA_N1_HIGH;
		}
	}
	else	// T02
	{
		AESDet.HoldTimeCoded = COM_PRE2;
		AESDet.CommandEvalTime = N2;
		AESDet.PowerAdrCoded = &AESDet.OutputWideBand[1];

		if ((config->AES.AES600_Mode & AES600_SEC_DEP_CMD) == 0)	// security low
		{
			AESDet.WindowLength = RX_WNDO2_SEC_LOW;
			AESDet.AlphaCoded = ALPHA_N2_LOW;
			AESDet.OneMinusAlphaCod = 1.0 - ALPHA_N2_LOW;
		}
		else	// security high
		{
			AESDet.WindowLength = RX_WNDO2_SEC_HIGH;
			AESDet.AlphaCoded = ALPHA_N2_HIGH;
			AESDet.OneMinusAlphaCod = 1.0 - ALPHA_N2_HIGH;
		}
	}


	// set the endpointer to the sinus summe and the Cosinus summe for all command frequencies
	AESDet.endptrSumSinCos[0] = SumSinF1 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[1] = SumCosF1 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[2] = SumSinF2 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[3] = SumCosF2 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[4] = SumSinF3 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[5] = SumCosF3 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[6] = SumSinF4 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[7] = SumCosF4 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[8] = SumSinF5 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[9] = SumCosF5 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[10] = SumSinF6 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[11] = SumCosF6 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[12] = SumSinF7 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[13] = SumCosF7 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[14] = SumSinF8 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[15] = SumCosF8 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[16] = SumSinF9 + AESDet.CommandEvalTime - 1;
	AESDet.endptrSumSinCos[17] = SumCosF9 + AESDet.CommandEvalTime - 1;

	// Reset sinus and cosinus summe arrays for all command frequencies
	for (i = 0; i < (2*(NO_OF_TONES_AES)); i++)
	{
		tempptr = AESDet.startptrSumSinCos[i];

		for (j = 0; j < N3; j++)
		{
			*tempptr++ = 0.0; 
		}
	}

	// Reset sinus and cosinus summe for all command frequencies
	for (i = 0; i < NO_OF_TONES_AES;i++)
	{
		AESDet.SinSum[i] = 0.0;
		AESDet.CosSum[i] = 0.0;
	}

	// Reset verify summe for cos-path and sin-path for all command frequencies
	for (i = 0; i < NO_OF_TONES_AES*2; i++)
	{
		AESDet.VerifSinCosSum[i] = 0.0;
	}

	// set power correction factor   
	AESDet.NCorrePower[0] = 1.0 / N1;
	AESDet.NCorrePower[1] = 1.0 / N2;
	AESDet.NCorrePower[2] = 1.0 / N3;

	// set correction factor for command frequency correlators 
	AESDet.NCorreDet = 1.0 / AESDet.CommandEvalTime;   // for all frequencies in common

	// Reset Detector activ times
	for (i=0;i<NO_OF_TONES_AES;i++)
	{
		AESDet.TimeDet[i] = 0.0;
	}

	// init struct AESDet
	AESDet.DetFlag = 0;
	AESDet.HoldCounter = 0;
	AESDet.WinCounter1 = 0;
	AESDet.WinCounter2 = 0;
	AESDet.Det_Win1 = 0;
	AESDet.Det_Win2 = 0;
	AESDet.Det_Last_Win1 = 0;
	AESDet.Det_Last_Win2 = 0;
	AESDet.Com_MixUpBit = 0;
	AESDet.Com_MixUpBitCounter = 0;
	AESDet.ActiveCommand = 0;
	
	// Reset Verification sum of Power N1, N2, N3
	AESDet.Power_Verif[0] = 0.0;
	AESDet.Power_Verif[1] = 0.0;
	AESDet.Power_Verif[2] = 0.0; 

	AESDet.ptr_NSD_Power_Speech_Guard = getPtr_NSD_Power_Speech_Guard();
}
 

/*****************************************************************************
  Function:		AES_Detector_activate()	
  Description:	Detection of AES commands
  Inputs:		pointer to Rx AF sample buffer, rxGuard: TRUE if guard present	
  Outputs:		-		
  Return:		acitive commands		
  Globals:		struct AESDet
  Important:	The function AES_Detector_init() has to be called once before 
				using AES_Detector_activate().

*******************************************************************************/
// 
unsigned long AES_Detector_activate(float *rxAfSignal, BOOL rxGuard, BOOL *detectorOutputActive)
{
	register float *PtrCos_Value;
	register float *PtrSin_Value;
	register float *PtrTempArgument;

	// static used for better performance
	static int ixSin;
	static int ixCos;
	static float rxAfSignalLP[RX_AF_BLOCK_LENGTH];
	static float PowerN;
	static float Oldvalue;
	static float NewValue;
	static float *PtrSinSum;
	static float *PtrCosSum;
	static float *PtrDeltaArg;
	static float *PtrOutputDet;
	
// triplevelcheck	BOOL tripLevelOk;
	
	// LP filtering of Rx signal
	BiIIR(rxAfSignal, rxAfSignalLP, AESDet.ptrLpCoef, AESDet.ptrLpFilterState, NUM_OF_SEC_LP2k, RX_AF_BLOCK_LENGTH);

	//******************************************************************
	//*** update Power N1, N2, N3 **************************************
	//******************************************************************
	
	PowerN = rxAfSignalLP[1] * rxAfSignalLP[1];

	//Calculate PowerN1
	Oldvalue = *AESDet.ptr_sumN1;
	NewValue = PowerN * AESDet.NCorrePower[0];

	*AESDet.ptr_sumN1++ = NewValue;
	AESDet.OutputWideBand[0] = AESDet.OutputWideBand[0] + NewValue - Oldvalue;
	AESDet.Power_Verif[0] += NewValue;

	if (AESDet.ptr_sumN1 > PowerArray_N1 + N1 -1) 
	{
		AESDet.ptr_sumN1 = PowerArray_N1; 
		// for tests only count failure of verif. sum of power N1
		if (AESDet.OutputWideBand[0] != AESDet.Power_Verif[0]) Failure_Counter +=1; 
		AESDet.OutputWideBand[0] = AESDet.Power_Verif[0];
		AESDet.Power_Verif[0] = 0.0;
		Counter_set +=1; 
	}

	// Calculate PowerN2
	Oldvalue = *AESDet.ptr_sumN2;
	NewValue = PowerN * AESDet.NCorrePower[1];

	*AESDet.ptr_sumN2++ = NewValue;
	AESDet.OutputWideBand[1] = AESDet.OutputWideBand[1] + NewValue - Oldvalue;
	AESDet.Power_Verif[1] += NewValue;

	if (AESDet.ptr_sumN2 > PowerArray_N2 + N2 -1) 
	{
		AESDet.ptr_sumN2 = PowerArray_N2;
		AESDet.OutputWideBand[1] = AESDet.Power_Verif[1]; 
		AESDet.Power_Verif[1] = 0.0;
	}

	// Calculate PowerN3
	Oldvalue = *AESDet.ptr_sumN3;
	NewValue = PowerN * AESDet.NCorrePower[2];

	*AESDet.ptr_sumN3++ = NewValue;
	AESDet.OutputWideBand[2] = AESDet.OutputWideBand[2] + NewValue - Oldvalue;
	AESDet.Power_Verif[2] += NewValue;

	if (AESDet.ptr_sumN3 > (PowerArray_N3 + N3 - 1)) 
	{
	
		AESDet.ptr_sumN3 = PowerArray_N3;
		AESDet.OutputWideBand[2] = AESDet.Power_Verif[2]; 
		AESDet.Power_Verif[2] = 0.0;
	
	}

	// Check minimum power
	// if power < -35dB set power -35dB
	// for tests the next three lines are to set for comment
	//
	if (AESDet.OutputWideBand[0] < MINPOWERSET)  AESDet.OutputWideBand[0] = MINPOWERSET; 
	if (AESDet.OutputWideBand[1] < MINPOWERSET)  AESDet.OutputWideBand[1] = MINPOWERSET;
	if (AESDet.OutputWideBand[2] < MINPOWERSET)  AESDet.OutputWideBand[2] = MINPOWERSET;

	//******************************************************************
	//*** Calculate Cos- and Sin- value for  F1...F9  ******************
	//******************************************************************
	//
	// init Pointers 
	PtrSin_Value = AESDet.Sin_Value;
	PtrCos_Value = AESDet.Cos_Value;
	PtrDeltaArg = AESDet.DeltaArg; 
	PtrTempArgument = AESDet.TempArgument; 

	// Calculate sin and cos for all frequencies
	do {
		static int temp;

  		// Calculate Sin 
  		*PtrTempArgument = (*PtrTempArgument + *PtrDeltaArg++);
  		if(*PtrTempArgument >= SINE_TAB_LENGTH)
		{
			*PtrTempArgument -= SINE_TAB_LENGTH;
		}
  		*PtrSin_Value++ = *(sintab + (int)(*PtrTempArgument));

  		// Calculate Cos
  		temp = (((int)(*PtrTempArgument++ + SINE_TAB_COS_OFFSET)) & MASKE_SINTABLE);
  		*PtrCos_Value++ = *(sintab + temp);

	 } while (PtrTempArgument < &AESDet.TempArgument[NO_OF_TONES_AES]);

	// check widebandpower < -25dB -> set correlator input rxAfSignalLP[1]= 0.0
	if (AESDet.OutputWideBand[2] < MINPOWER) rxAfSignalLP[1] = 0.0; // set input for detector to zero if Power(tev3) < -25dB

	AESDet.WidePowerAndGuardCod =  *AESDet.PowerAdrCoded * AESDet.AlphaCoded;    // Power * Alpha for coded commands

	//*****************************************************************
	//*** Calcualte the Correlation for trip frequencies **************
	//*****************************************************************
	//
	//set the pointers
	PtrSin_Value = AESDet.Sin_Value;
	PtrCos_Value = AESDet.Cos_Value;
	PtrSinSum = AESDet.SinSum;
	PtrCosSum = AESDet.CosSum;
	PtrOutputDet = AESDet.OutputDet;

	ixSin = 0;
	ixCos = 1;
	AESDet.DetFlag = 0;
	AESDet.LastDetector[1] = AESDet.LastDetector[0]; //save old detector
	AESDet.LastDetector[0] = 0;

	// do the correlation for all trip frequencies
	do {
		// Calculate Correlator sinus part
		Oldvalue = *AESDet.PtrSumSinCos[ixSin];								// save old value
		NewValue = *PtrSin_Value * rxAfSignalLP[1] * AESDet.NCorreDet;		// calculate new value
		PtrSin_Value++;														// inc pointer to sin value

	  	*AESDet.PtrSumSinCos[ixSin] = NewValue;								// write new value to sin delay line
		AESDet.PtrSumSinCos[ixSin]++;										// inc sin delay line pointer

		AESDet.VerifSinCosSum[ixSin] += NewValue;							// update sin verification summe
  		*PtrSinSum = *PtrSinSum - Oldvalue + NewValue;						// update sin sum

		
  		if (AESDet.endptrSumSinCos[ixSin] < AESDet.PtrSumSinCos[ixSin]) 
		{
			AESDet.PtrSumSinCos[ixSin] = AESDet.startptrSumSinCos[ixSin];
			*PtrSinSum = AESDet.VerifSinCosSum[ixSin];						// take verification summe to SinSum
			AESDet.VerifSinCosSum[ixSin] = 0.0;								// reset verification summme Sin part

		}

		// Calculate Correlator cosinus part
		Oldvalue = *AESDet.PtrSumSinCos[ixCos];								// save old value
		NewValue = *PtrCos_Value * rxAfSignalLP[1] * AESDet.NCorreDet;		// calculate new value
		PtrCos_Value++;														// inc pointer to cos value

  		*AESDet.PtrSumSinCos[ixCos] = NewValue;								// write new value to cos delay line
		AESDet.PtrSumSinCos[ixCos]++;										// inc cos delay line pointer

		AESDet.VerifSinCosSum[ixCos] += NewValue;							// update cos verification summe cos part	 
  		*PtrCosSum = *PtrCosSum - Oldvalue + NewValue;						// update cos sum

  		if (AESDet.endptrSumSinCos[ixCos] < AESDet.PtrSumSinCos[ixCos]) 
		{
			AESDet.PtrSumSinCos[ixCos] = AESDet.startptrSumSinCos[ixCos];
			*PtrCosSum = AESDet.VerifSinCosSum[ixCos];						// take verification summe to CosSum
			AESDet.VerifSinCosSum[ixCos] = 0.0;								// reset verification summme cos-part
		}

		// Calculate Detector Output
  		*PtrOutputDet = (SQUARE(*PtrCosSum) + SQUARE(*PtrSinSum)) * 2.0;
		PtrCosSum++;
		PtrSinSum++;
		
// triplevelcheck		AESDet.tempDetectorPower = *PtrOutputDet;
  		//Decision current detector active
		Oldvalue = *PtrOutputDet++ - AESDet.WidePowerAndGuardCod;
		if (Oldvalue > 0.0) 
		{
    		AESDet.LastDetector[0] = (ixSin >> 1) + 1; 
    		AESDet.DetFlag = AESDet.DetFlag + 1;
    		AESDet.TimeDet[ixSin >> 1] += CYCLE_TIME;  // update activ detector time
		}
		else
		{
		}
  		
		ixSin += 2;
		ixCos += 2;

	} while (ixSin < (AESDet.TonesActive*2));

	// trip and guard alarm (for same behavior as AES550)
	// set / reset flag when command detector active
	*detectorOutputActive = FALSE;
	if(AESDet.LastDetector[0] != 0) // during coded FSK commands the LastDetector[0] can be 0!! 
									// --> only for single tone interference detection!
	{
		// set flag whene DetectorActive  used in AES600 for " Trip and Guard" alarm
		*detectorOutputActive = TRUE;
	}


	// New reset window if guard only is present and wide power is lower then unblocking threshold low
//	if((rxGuard == TRUE) && (AESDet.LastDetector[0] == 0) && (*AESDet.PowerAdrCoded < UN_THERS_LOW))
//	if((rxGuard == TRUE))
	if((rxGuard == TRUE) && (AESDet.LastDetector[0] == 0))
	{
		AESDet.Det_Win2 = 0;      // close window 2 -> reset activ detector in window 2 
		AESDet.Det_Last_Win2 = 0;
		AESDet.Det_Win1 = 0;      // close window 1 -> reset activ detector in window 1
		AESDet.Det_Last_Win1 = 0;	
// triplevelcheck		AESDet.DetectorPowerWin1 = 0;
// triplevelcheck		AESDet.DetectorPowerWin2 = 0;
	}
		
	// check if more than one detector is active
	if (AESDet.DetFlag > 1)  
	{
		AESDet.LastDetector[0] = 0;  // if more then one detector is active reset active detector
	}

	//*****************************************************************
	//***   Command evaluation       **********************************
	//*****************************************************************
	//gemäss NSD50 CODET.ASM -> Beschreibung HENF468563.TXT 
	//
	if ((AESDet.LastDetector[0] != AESDet.LastDetector[1]) && (AESDet.LastDetector[0] != 0))
	{
		if (AESDet.WinCounter1 > AESDet.WinCounter2)		// neu, nicht im HENF468563.TXT enthalten               
    	{													
    	  	if (AESDet.LastDetector[0] != AESDet.Det_Win1) 
    	  	{
    	  	  	AESDet.Det_Last_Win2 = AESDet.Det_Win2;		// neu gegen Befehlsverwechslung
				AESDet.WinCounter2 = AESDet.WindowLength;
    	  	  	AESDet.Det_Win2 = AESDet.LastDetector[0];

// triplevelcheck				AESDet.DetectorPowerWin2 = AESDet.tempDetectorPower; //gnnew
				//
				// neu gegen Befehlsverwechslung Window 2
				//
				if(AESDet.Det_Last_Win2 == 0)				// neu gegen Befehlsverwechslung
				{
					AESDet.Com_MixUpBit = 0;
				}
				else
				{
					//if((AESDet.Det_Win2 == AESDet.Det_Last_Win2) && (AESDet.Det_Win1 == AESDet.Det_Last_Win1)) {}
					if(AESDet.Det_Win2 == AESDet.Det_Last_Win2) AESDet.Com_MixUpBit = 0;
					else 
					{
						AESDet.Com_MixUpBit = 1;
						AESDet.Com_MixUpBitCounter += 1;
					}
				}
				//
				// Ende Befehlsverwechslung Window 2
				// 
    	   	}
    	   	else
    	  	{
    	  	}
    	 }
    	 else
    	 {				// else WinCounter1>WinCounter2
    	  	if (AESDet.LastDetector[0] != AESDet.Det_Win2)       // neu, nicht im HENF468563.TXT enthalten
    	  	{                                                      //
    	  	   	AESDet.Det_Last_Win1 = AESDet.Det_Win1;
				AESDet.WinCounter1 = AESDet.WindowLength;
    	  	   	AESDet.Det_Win1 = AESDet.LastDetector[0];

// triplevelcheck				AESDet.DetectorPowerWin1 = AESDet.tempDetectorPower; //gnnew
				//
				// neu gegen Befehlsverwechslung Window 1
				//
				if (AESDet.Det_Last_Win1 == 0)
				{
					AESDet.Com_MixUpBit = 0;
				}
				else
				{
					//if((AESDet.Det_Win2 == AESDet.Det_Last_Win2) && (AESDet.Det_Win1 == AESDet.Det_Last_Win1)){}
					if(AESDet.Det_Win1 == AESDet.Det_Last_Win1) AESDet.Com_MixUpBit = 0;
					else
					{
						AESDet.Com_MixUpBit = 1;
						AESDet.Com_MixUpBitCounter += 1;
					}
				}

    	  	}
    	  	else
    	  	{
    	  	}
		}	
	}
	else 
	{
	}

	// Window monitoring
	//
	// window 1
	if (AESDet.WinCounter1 > 0) 
	{
   		AESDet.WinCounter1 -= 1;   // window 1 still activ
	}
	else
	{
   		AESDet.Det_Win1 = 0;      // close window 1 -> reset activ detector in window 1
		AESDet.Det_Last_Win1 = 0;
// triplevelcheck		AESDet.DetectorPowerWin1 = 0;
// triplevelcheck		AESDet.DetectorPowerWin2 = 0;
	}
	// window 2
	if (AESDet.WinCounter2 > 0) 
	{
   		AESDet.WinCounter2 -= 1;   // window 2 still activ
	}
	else
	{
   		AESDet.Det_Win2 = 0;      // close window 2 -> reset activ detector in window 2 
		AESDet.Det_Last_Win2 = 0;
// triplevelcheck		AESDet.DetectorPowerWin1 = 0;
// triplevelcheck		AESDet.DetectorPowerWin2 = 0;
	}
	//****************************************************************
	//*** Set hold time for coded commands ***************************
	//**************************************************************** 

	AESDet.Ptr_Com = AESDet.PtrCOMTAB + AESDet.Det_Win1 + COLUMN_WIDH * AESDet.Det_Win2;
	AESDet.HoldCounterPreset = AESDet.HoldTimeCoded;	// set hold time coded command

// triplevelcheck	if (*AESDet.Ptr_Com != 0)
// triplevelcheck	{
// triplevelcheck		if (AESDet.DetectorPowerWin1>AESDet.DetectorPowerWin2) // LastdetectorPower[0] higer
// triplevelcheck		{
// triplevelcheck			if(AESDet.DetectorPowerWin1 < (MAXTRIPLEVELDIF*AESDet.DetectorPowerWin2))
// triplevelcheck			{
// triplevelcheck				tripLevelOk = TRUE; //ok
// triplevelcheck			}
// triplevelcheck			else
// triplevelcheck			{
// triplevelcheck				tripLevelOk = FALSE; // don't accept Trip
// triplevelcheck			}
// triplevelcheck		}
// triplevelcheck		else
// triplevelcheck		{ 
// triplevelcheck			if(AESDet.DetectorPowerWin2 < (MAXTRIPLEVELDIF*AESDet.DetectorPowerWin1))
// triplevelcheck			{
// triplevelcheck				tripLevelOk = TRUE; //ok
// triplevelcheck			}
// triplevelcheck			else
// triplevelcheck			{
// triplevelcheck				tripLevelOk = FALSE; // don't accept Trip
// triplevelcheck			}
// triplevelcheck		}
// triplevelcheck	}

	//****************************************************************
	//*** Check if true command is activ     *************************
	//****************************************************************	  
	if (*AESDet.Ptr_Com == 0) // command is not active
	{

		// command not activ -> hold command if holdtime is not zero
		if (AESDet.HoldCounter > 0)
		 {  
			
			// set flag whene Command Active  used in AES600 for " Trip and Guard" alarm
			*detectorOutputActive = TRUE;

     		//hold last command while hold time not over
     		AESDet.HoldCounter--;      // hold last activ command
		 }
		else
		 {  
			//hold time ist over reset command
     		AESDet.ActiveCommand = 0;	// Reset active command
		 }
	}
	else // command is active
	{
		// set flag when Command Active  used in AES600 for " Trip and Guard" alarm
		*detectorOutputActive = TRUE;
		AESDet.HoldCounter = AESDet.HoldCounterPreset;	// set hold counter	for flag or command prolongation
														// needed cause of guard sequence between commands
	
		 //****************************************************************
		 //*** test if no Rx Guard is active to set the activ commands ****
		 //****************************************************************
// triplevelcheck		if ((rxGuard == FALSE) && (AESDet.Com_MixUpBit == 0) && (tripLevelOk == TRUE))                  
		if ((rxGuard == FALSE) && (AESDet.Com_MixUpBit == 0))                  
   		{
   	 		// correct command is active
   	 		AESDet.ActiveCommand = *AESDet.Ptr_Com;  		// set activ command 
   		}

	}

	// ************************************************************************
	// *** update power level Speechband for monitoring functions   ***********
	// ************************************************************************

	// update for Ch1 only
	AESDet.ptr_NSD_Power_Speech_Guard->SpeechPowerCh1[0] = AESDet.OutputWideBand[0];
	AESDet.ptr_NSD_Power_Speech_Guard->SpeechPowerCh1[1] = AESDet.OutputWideBand[1];
	AESDet.ptr_NSD_Power_Speech_Guard->SpeechPowerCh1[2] = AESDet.OutputWideBand[2];
	

	return(AESDet.ActiveCommand);	
}


