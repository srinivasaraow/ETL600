/*-------------------------------------------------------------------------*
 * $Workfile: OFDM_CFG_Types.h $
 * Part of      : ETL600 / OFDM
 * Language     : C
 * Created by   : vgp
 * Remarks      :  
 * Purpose      : OFDM global type declarations
 * Copyright    : 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/H/OFDM_CFG_Types.h $
 * 
 * 18    24.07.08 15:17 Chhamaa
 * 
 * 13    5.04.06 15:28 Chhamaa
 * 
 * 12    17.03.06 14:41 Chhamaa
 * 
 * 11    16.09.05 9:42 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 8     26.04.05 8:45 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 5     7.03.05 8:43 Ats
 * Exchange with TLC
 * 
 * 4     24.02.05 7:34 Chchlee
 * 
 * 2     28.01.05 11:48 Ats
 * Exchange TLC
 * 
 * 7     22.12.04 9:35 Chchlee
 * 
 * 6     21.10.04 14:12 Maag01
 * 
 * 5     21.10.04 11:15 Maag01
 *
 *-------------------------------------------------------------------------*/

#ifndef OFDM_CFG_TYPES_H
#define OFDM_CFG_TYPES_H

#ifdef __ECC__
// C definitions

#define N_DATA_RATES			5		// number of data rates
#define N_PH_CTRL_PARAM			4
#define TDM_SERVICE_START_CHAR	(unsigned long)0x68000068
#define TDM_SERVICE_END_CHAR		(unsigned long)0x16000016

#else

// Assembler difinitions
#define TDM_SERVICE_START_CHAR	0x68000068
#define TDM_SERVICE_END_CHAR		0x16000016
#define e_TDMServiceAddr_OFDM       4
#endif

#ifdef __ECC__
// C definitions

#include "../h/tdm_service.h"


// -- OFDM CONFIGURATION -----------------------------

typedef enum { GUARD_Int8=0, GUARD_Int16=1, GUARD_Int32=2, GUARD_Int64=3, GUARD_Int7=4, GUARD_IntXXX=7} t_GUARD_Int;
typedef enum { ORTH_Mode64=0, ORTH_Mode128=1, ORTH_Mode256=2, ORTH_Mode512=3 , ORTH_ModeXXX=7} t_ORTH_Mode;
typedef enum { TO_Low=0, TO_High=1} t_TO_Mode;
typedef enum { V11=0, G703=1 } t_INT_Type;
typedef enum { Test_Disable=0, Test1=1, Test2=2, Test3=3 } t_TEST_Mode;
typedef enum { Hz0=0, Hz4000=1, Hz8000=2, Hz12000=3 } t_spectrumPosition;
//typedef enum { BW4000=0, BW8000=1, BW12000=2, BW16000=3, BW20000=4, BW24000=5, BW28000=6, BW32000=7} t_channelBW;
typedef enum { CMODE_DR0=0, CMODE_DR1=1, CMODE_DR2=2, CMODE_DR3=3, CMODE_DR4=4, CMODE_FBFF=7} t_DRate_Mode;
typedef unsigned int t_MODEM_Mode;
typedef unsigned int t_NB_Det_Mode;
typedef unsigned int t_RS_Mode;
typedef unsigned int t_FBFF_Mode; // obsolete
typedef unsigned int t_SYNC_Mode; //obsolete
typedef enum { Normal_BW=0 , Reduced_BW=1} t_SPEC_Mode; //obsolete
typedef unsigned int t_Edge_Mode;
typedef enum { pilot_not_present=0, pilot_present=1 } t_ETLPilot_Mode;
typedef enum { std_FN = 0, high_FN = 1 } t_BW4_8_Mode;

typedef enum {
	Mode_Detection	    = 0,
	Tone2_Searching	 	= 1,
	Tone2_detected		= 2,
	NoiseEst_finished   = 3,
	FreqEst_finished	= 4,
	LFM_finished		= 5,
	QAM4_detected		= 6,
	Prefix_detected		= 7,
	ChannelData_received= 8,
	Optim_1_finished	= 9,
	Optim_2_finished	=10,
	Optim_3_finished	=11,
	Suffix_detected		=12,
	Training_running	=13,
	Training_finished	=14,
	V11_sync			=15
} t_Sync_State;

typedef t_Edge_Mode t_DIN_Mode;
typedef t_Edge_Mode t_DOUT_Mode;

typedef struct {
	unsigned int	   Reserved2:    6;	   // [Bit 31:26] reserved
	t_BW4_8_Mode	   BW4_8_Mode:   1;    // [Bit 25] 0- std_FN , 1-high_FN
	unsigned int       Reserved:     3;	   // [Bit 24:22] reserved for Master DSP	
	t_DRate_Mode       DRate_Mode:   3;    // [Bit 21:19] 
	t_GUARD_Int        GUARD_Int:    3;    // [Bit 18:16] 0 - 8, 4 - 16, 2- 32, 6 - 64, 1,5,3,7 - reserved
	t_ORTH_Mode        ORTH_Mode:    3;    // [Bit 13:15] 0 - 64, 4 - 128, 2- 256, 6 - 512, 1,5,3,7 - reserved
	t_TO_Mode          TO_Mode:      2;    // [Bit 11:12] 0 - low time-out, 2 - high time-out, 3,4 - reserved
	t_INT_Type         INT_Type:     2;    // [Bit  9:10] 0 - V.11 mode, 2 - G.703, 1,3 - reserved
	t_DOUT_Mode        DOUT_Mode:    1;    // [Bit  8]    1 - valid on positive, 0 - negative edge
	t_DIN_Mode         DIN_Mode:     1;    // [Bit  7]    1 - valid on positive,  0 - negative edge
	t_SYNC_Mode        SYNC_Mode:    1;    // obsolete[Bit  6] 
	t_TEST_Mode        TEST_Mode:    2;    // [Bit  4:5]  0 - disable test mode, 1 - test mode 1, 2 - test mode 2, 3 - test mode 3
	t_FBFF_Mode        FB_FF_Mode:   1;    // obsolete[Bit  3]    1 - enable fallback/fallforward mode, 0 - disable fallback/fallforward mode
	t_RS_Mode          RS_Mode:      1;    // [Bit  2]    1 - enable Reed-Solomon, 0 - disable Reed-Solomon
	t_NB_Det_Mode      NB_Detection: 1;    // [Bit  1]    1 - enable narrowband interference detection, 0 - don't enable narrowband interference detection
	t_MODEM_Mode       MODEM_Mode:   1;    // [Bit  0]    1 - Master, 0 - Slave
} t_OFDM_Mode_Reg;

typedef struct {
	t_OFDM_Mode_Reg    mode_reg;				// bitfield structure: OFDM_CFG_MODE bits, see above							
	int				   channelBW;				// channel bandwidth [Hz]
	int			       dataRate[N_DATA_RATES];	// [bps], [0]: constant mode, 
										        // [1]-[5]:fall back /fall forward mode
	t_spectrumPosition spectrumPosition;		// 0 = 0Hz, 1 = 4000Hz, 2 = 8000Hz, 3 = 12000Hz
	t_ETLPilot_Mode	   leftPilotPresent;		// pilot_not_present=0, pilot_present=1
	unsigned long	   burstErrorWindowSize;	// fall back parameter
	unsigned long	   fallBackWindowSize;		// fall back parameter
	float			   burstErrorProbLimit;		// fall back parameter
	float			   fallBackProbLimit;		// fall back parameter
	float			   fallForwardSigmaLimit;	// fall forward parameter
	float			   phaseControlParam[N_PH_CTRL_PARAM];

} t_OFDM_Config;

// -- OFDM NL Optimization -----------------------------
typedef enum {
	Optim_NL_OFF,
	Optim_NL_ON_Determination,
	Optim_NL_ON_Selected,
}
e_Optim_NL_State_Type;

typedef enum
{
	Pilot_Mode_I_detected,
	Pilot_Mode_II_detected,
	Pilot_not_detected
}
e_Pilot_Mode_detection_type;

// -- Two Tone Detection State
typedef enum
{
	unknown_state,
	tone_detected_state,
	tone_not_detected_state
}
e_tone_detection_type;

// -- OFDM SHORT STATUS -----------------------------
typedef struct {
	unsigned int       reservedB:    9;
	unsigned int	   BLER:	3; //0 for 0, 1 for >10E-6, >10E-5, >10E-4, >10E-3, >10E-2, 6 for >10E-1, 7 for unknown
	unsigned int	   reservedA: 1;	
	unsigned int	   SelectedRate:	3; // 0-4(rate0-4) , 7(unknown)
	unsigned int	   MaxRate:	10; // maximum data rate 0-1023kbit/s
	t_GUARD_Int        GUARD_Int:    3; //L
	t_ORTH_Mode        ORTH_Mode:    3; //N
} t_mode0;

typedef struct {
	unsigned int 	 SNR: 		8;
	unsigned int 	 SYNC: 		8;
	unsigned int 	 DeltaSigma:	8;
	unsigned int 	 EPSM: 		8;
} t_mode1;

typedef struct {
	t_mode0       mode0;
	t_mode1       mode1;
} t_OFDM_ShortStatus;


// -- OFDM LONG STATUS -----------------------------
#define MAX_LENGTH_LONG_STATUS 1024

typedef enum
{
	e_LongStID_MSE_trend_QAM4		= 0,
	e_LongStID_MSE_trend_QAMT		= 1,
	e_LongStID_MSE_trend_QAMD		= 2,
	e_LongStID_Noise_Density		= 3,
	e_LongStID_Noise_Density_Sort	= 4,
	e_LongStID_Eqlzr				= 5,
	e_LongStID_Error_Count			= 6,
	e_LongStID_Error_Last			= 7,
	e_LongStID_QAM_Index_TX			= 8,
	e_LongStID_QAM_Index_RX			= 9,
	e_LongStID_F_trend_CO			= 10,
	e_LongStID_F_trend_CO_delta		= 11,
	e_LongStID_Abs2_Filter_RX		= 12,
	e_LongStID_Bits_in_SC_TX		= 13,
	e_LongStID_Bits_in_SC_RX		= 14,
	e_LongStID_SNR_bin_TX			= 15,
	e_LongStID_SNR_bin_RX			= 16,
	e_LongStID_Bits_bin_TX			= 17,
	e_LongStID_Bits_bin_RX			= 18,
	e_LongStID_Mag2l_Inp			= 19,
	e_LongStID_Mask_NB				= 20,
	e_LongStID_F_Stat_NB			= 21,
	e_LongStID_Channel_TF			= 22,
	e_LongStID_K_TX					= 23,
	e_LongStID_K_RX					= 24,
	e_LongStID_K_Abs_TX				= 25,
	e_LongStID_K_Abs_RX				= 26,
	e_LongStID_K_Sort_2_TX			= 27,
	e_LongStID_K_Sort_2_RX			= 28,
	e_LongStID_C_TX					= 29,
	e_LongStID_C_RX					= 30,

	e_LongStID_BLER_EST				= 31,
	e_LongStID_MSE_dB_Mean			= 32,
	e_LongStID_CRC_Errors_Sum		= 33,
	e_LongStID_M_Opt_TX				= 34,
	e_LongStID_M_Opt_RX				= 35,
	e_LongStID_Analog_Gain			= 36,
	e_LongStID_Digital_AGC			= 37,
	e_LongStID_Df_Hz				= 38,
	e_LongStID_NB_count				= 39,
	e_LongStID_AWGN_Sigma3_Est		= 40,
	e_LongStID_AWGN_Sigma_Est_TX	= 41,
	e_LongStID_AWGN_Sigma_Est_RX	= 42,
	e_LongStID_Data_rate_max_TX		= 43,
	e_LongStID_Data_rate_max_RX		= 44,
	e_LongStID_Selected_data_rate_TX= 45,
	e_LongStID_Selected_data_rate_RX= 46,
	
	e_LongStID_FS_Score_L			= 47,
	e_LongStID_FS_SCORE_L_LIMIT		= 48,
	e_LongStID_SNR_TCM				= 49, // local SNR distribution
	e_LongStID_SNR_TCM_SWITCH		= 50, // Switch to activate measurement
	e_LongStID_RX_FILT_ADAPT_SWITCH = 51, // Switch to deactivate RX-Filter Adaptation
	e_LongStID_Time_trend			= 52, // Processing Time Measurement Array
	e_LongStID_idle_time_min		= 53, // Min IDLE TIME
	e_LongStID_Eqlzr_backup			= 54, // Initial Eqlzr Setting
	e_LongStID_MSE_Spectral_Density	= 55, // MSE spectral density during data transm.
	e_LongStID_F_trend_DF			= 56,
	e_Long_StID_F_trend_rst			= 57,
	e_Long_StID_F_trend_ctr			= 58,
	e_LongStID_reserved6			= 59,
	
	e_Long_StID_Clock_Ph_Offset_Delta = 60,
	e_Long_StID_Phase_trend_ctr		= 61,
	e_Long_StID_Phase_trend_rst		= 62,
	e_Long_StID_Phase_trend			= 63,
	e_Long_StID_F_trend_CW			= 64,
	e_Long_StID_Phase_trend_ControlCode = 65,
	
	e_Long_StID_Phaseminmaxmapctr   = 70,
	e_Long_StID_Phasemintot			= 71,
	e_Long_StID_Phasemaxtot			= 72,
	e_Long_StID_Phaseminmap			= 73,
	e_Long_StID_Phasemaxmap			= 74,
	
	e_Long_StID_Ph_test_hop_switch	= 75,
	e_Long_StID_Ph_test_LRlen_inc	= 76,
	e_Long_StID_Ph_test_LRlen_dec	= 77,
	e_Long_StID_Ph_test_HUB_inc		= 78,
	e_Long_StID_Ph_test_HUB_dec		= 79,	
	
	e_Long_StID_Ph_test_setRefPhase	= 80,
	e_Long_StID_Ph_test_HUB_invert	= 81,
	
	
	e_Long_StID_phaseControlParam 	  = 82,
	e_Long_StID_Num_Wait_LRs_halfhour = 83,
	
	e_Long_StID_ofdmMaxSample		= 90,
	e_Long_StID_ofdmMaxSampleReset	= 91,
	e_Long_StID_ofdmRestartCounter	= 92,

	e_LongStID_Software_Version_ID  = 100,

}
e_OFDM_LongStatus_ID;

typedef struct
	{
	e_OFDM_LongStatus_ID 	ID;
	unsigned long 			datalength;
	unsigned long			offset;
	unsigned long			step;
} t_OFDM_LongStatusHeader;

typedef struct
{
	t_OFDM_LongStatusHeader	header;
	float 		 			data[MAX_LENGTH_LONG_STATUS];
} t_OFDM_LongStatus;


// -- TDM Service -----------------------------
typedef struct
{
	unsigned long				startChar;
	e_TDMServiceAddr	        destAddr;
	e_TDMServiceMsgType			messageType;
	unsigned long				messageSize;
}
t_TDMServiceHeader;

#endif // ifdef __ECC__

#endif
