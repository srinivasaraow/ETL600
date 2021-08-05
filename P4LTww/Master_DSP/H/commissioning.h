/*----------------------------------------------------------------------------*
 * $Workfile: commissioning.h $
 * Part of      : HMI600 / Master DSP
 * Language     : C
 * Created by   : Nohl Samuel
 * Remarks      :  
 * Purpose      : Definitions for commissioning functions
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/FE_DSP/H/commissioning.h $
 * 
 * 26    12-02-24 16:21 Chhamaa
 * COMM_TT_ATT_TX_ALARM added.
 * 
 * 25    11-09-07 18:15 Chhamaa
 * COMM_TT_LEVEL_PEP added.
 * 
 * 18    24.06.05 13:58 Chsanoh
 * 
 * 17    8.04.05 17:16 Chhamaa
 * COMM_TT_RX_SCALING_COEF set to 1.0
 * 
 * 16    7.04.05 14:29 Chsanoh
 * 
 * 15    5.04.05 17:44 Chhamaa
 * COMM_TT_LEVEL_ACT_TX_ALARM_LEVEL modified
 * 
 * 14    29.03.05 15:04 Chhamaa
 * COMM_TT_LEVEL_ACT_TX_ALARM_LEVEL modified
 * 
 * 13    29.03.05 14:21 Chhamaa
 * 
 * 12    24.03.05 16:32 Chhamaa
 * 
 * 11    11.03.05 15:21 Chsanoh
 * 
 * 10    25.02.05 9:56 Chhamaa
 * COMM_TT_RX_SCALING_COEF added
 * 
 * 9     24.02.05 11:11 Chsanoh
 * 
 * 8     23.02.05 11:42 Chsanoh
 * 
 * 7     21.02.05 17:35 Chsanoh
 * 
 * 6     18.02.05 15:41 Chsanoh
 * 
 * 5     18.02.05 9:42 Chsanoh
 * #define COMM_CMD_DEACTIVATE_TESTTONE added.
 * 
 * 4     17.02.05 18:41 Chhamaa
 * Defines added
 * 
 * 3     17.02.05 14:34 Chsanoh
 * 
 * 2     16.02.05 17:20 Chsanoh
 * 
 * 1     16.02.05 9:39 Chsanoh
 * Initial version
 *----------------------------------------------------------------------------*/

#ifndef	COMMISSIONING_H
#define	COMMISSIONING_H

/* INCLUDE FILES **************************************************************/


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// General defines
#define UL_TX_LEVEL_ADJUSTMENT_DB	(float)  3.0F	// UL = Upper Limit
#define LL_TX_LEVEL_ADJUSTMENT_DB	(float)-80.0F	// LL = Lower Limit
#define COMM_RF_FREQ_SHIFT			(float)3000		// Hz

// Defines for commissioning command modes
#define COMM_CMD_ADJ_TX_RF_LEVEL		(unsigned long)1
#define COMM_CMD_ADJ_RX_RF_LEVEL		(unsigned long)2
#define COMM_CMD_TUNE_TX_FILTER			(unsigned long)3
#define COMM_CMD_TUNE_RX_FILTER			(unsigned long)4
#define COMM_CMD_TUNE_HYBRID			(unsigned long)5
#define COMM_CMD_SAVE_TX_RF_LEVEL_ADJ	(unsigned long)6
#define COMM_CMD_DEACTIVATE_TESTTONE	(unsigned long)7
#define COMM_CMD_ACT_TX_ALARM_LEVEL		(unsigned long)8
#define COMM_CMD_GET_RX_TESTTONE_LEVEL	(unsigned long)9

// Commissioning settings
#define COMM_TT_FREQ_DEFAULT				1150.0F		// [Hz] default test tone frequency
#define COMM_TT_LEVEL_PEP					1.0			//   0dB relative to PEP
#define COMM_TT_LEVEL_ADJ_TX_RF_LEVEL		0.3162278F	// -10dB relative to PEP
#define COMM_TT_LEVEL_TUNE_FILTER			0.3162278F	// -10dB relative to PEP
#define COMM_TT_LEVEL_TUNE_HYBRID			0.3162278F	// -10dB relative to PEP
#define COMM_TT_LEVEL_ACT_TX_ALARM_LEVEL	0.0794328F	// -22dB relative to PEP (see "Levels and Boosting for ETL600")
#define COMM_TT_ATT_TX_ALARM				22			// [dB] below PEP (see "Levels and Boosting for ETL600")	
#define COMM_TT_RX_SCALING_COEF				1.0F		// without rx signal scaling on FE-DSP: 0.1306165F (1/7.656)   
														// scales the test tone amplitude to 1.0 (ADC full-scale)  

/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {

	unsigned long command;
	long txRfFrequency;			// value in Hz
	long rxRfFrequency;			// value in Hz
	float txRfLevelAdjustment;	// linear value
	float txTestToneLevel;		// linear value

} t_Commissioning;

#endif


