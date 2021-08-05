/*-------------------------------------------------------------------------*
 * $Workfile: OFDM_Types.h $
 * Part of      : ETL600 / OFDM
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : OFDM global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/H/OFDM_Types.h $
 * 
 * 47    8.09.08 16:26 Chhamaa
 * e_OFDM_LongStatus_ID modified.
 * 
 * 46    8.09.08 11:25 Chsanoh
 * Bug fix: Status view/DATA COMMUNICATION STATUS/Current data rate and
 * Status view/DATA COMMUNICATION STATUS/O4CV channel status
 * 
 * 45    6.06.08 15:54 Chsanoh
 * 3rd MAS implementation phase
 * 
 * 44    6.05.08 15:55 Chhamaa
 * t_OFDM_Config: type of reserved changed to float
 * 
 * 38    14.09.05 14:05 Chrearp
 * 
 * 37    7.09.05 11:37 Chchlee
 * OFDM_CFG_MODE_4_8_kHz added
 * 
 * 36    26.04.05 8:45 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 34    16.03.05 10:17 Chchlee
 * OFDM_CFG_MODE_REDUCED_SPECTRUM removed again
 * 
 * 33    15.03.05 14:03 Chchlee
 * OFDM_CFG_MODE_REDUCED_SPECTRUM added
 * 
 * 32    9.03.05 13:34 Chhesch5
 * 
 * 31    3.01.05 15:09 Chhesch5
 * 
 * 30    3.01.05 15:07 Chhesch5
 * added define OFDM_CFG_MODE_ON_OFF
 * 
 * 29    20.12.04 13:55 Chhesch5
 * 
 * 28    17.12.04 12:46 Chchlee
 * Long status added
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef OFDM_TYPES_H
#define OFDM_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"constant.h" // for N_DATA_RATES


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

// OFDM configuration data

// BEGIN bitfield defines for OFDM_CFG_MODE bits (t_OFDM_Config.mode)

#define		OFDM_CFG_MODE_MASTER_BIT	     	BIT0	// 0 = slave, 1 = master
#define		OFDM_CFG_MODE_NB_ENABLE_BIT			BIT1	// enable narrowband interference detection
#define		OFDM_CFG_MODE_RS_ENABLE_BIT			BIT2	// enable Reed/Solomon


/* Testmode bits:
	  Bit 0            Bit 1
		0                0			--> normal mode
		1                0			--> test mode 1
		0                1			--> test mode 2
		1                1			--> test mode 3

  */
#define		OFDM_CFG_MODE_TESTMODE_SELECT_BIT0	BIT4	// Test mode selector bit 0
#define		OFDM_CFG_MODE_TESTMODE_SELECT_BIT1	BIT5	// Test mode selector bit 1
#define		OFDM_CFG_MODE_TESTMODE_SELECT_BITS	(OFDM_CFG_MODE_TESTMODE_SELECT_BIT0 | \
													OFDM_CFG_MODE_TESTMODE_SELECT_BIT1)	

#define		OFDM_CFG_MODE_DIN_EDGE_BIT			BIT7	// D-IN  valid on positive(1) / negative (0) edge
#define		OFDM_CFG_MODE_DOUT_EDGE_BIT			BIT8	// D-OUT valid on positive(1) / negative (0) edge

/* Interface type bits:
	  Bit 0             Bit 1
		0                0			--> V.11 mode
		1                0			--> G703.1
		0                1			--> future use
		1                1			--> future use

  */
#define		OFDM_CFG_MODE_IF_TYPE_BIT0		    BIT9	
#define		OFDM_CFG_MODE_IF_TYPE_BIT1		    BIT10	

/* Time-out bits:
	  Bit 0             Bit 1
		0                0			--> low time-out
		1                0			--> high time-out
		0                1			--> future use
		1                1			--> future use

  */
#define		OFDM_CFG_MODE_TMO_BIT0			    BIT11	
#define		OFDM_CFG_MODE_TMO_BIT1			    BIT12	


/* N (orthogonality interval) bits:
	  Bit 0   Bit 1    Bit 2
		0       0		 0    	--> 64
		1       0		 0	    --> 128
		0       1		 0 	    --> 256
		1       1		 0	    --> 512
		0       0		 1    	--> future use
		1       0		 1	    --> future use
		0       1		 1 	    --> future use
		1       1		 1	    --> future use
 
   N is only considered in test mode 1

  */
#define		N_LOOK_UP_VALUES				{64,128,256,512,99901,99902,99903,99904};

#define		OFDM_CFG_MODE_N_BIT0		    BIT13	
#define		OFDM_CFG_MODE_N_BIT1		    BIT14	
#define		OFDM_CFG_MODE_N_BIT2		    BIT15	
#define		OFDM_CFG_MODE_N_BITS	(OFDM_CFG_MODE_N_BIT0 | OFDM_CFG_MODE_N_BIT1 | \
									 OFDM_CFG_MODE_N_BIT2)	


/* L (guard interval) bits:
	  Bit 0   Bit 1    Bit 2
		0       0		 0    	--> 8
		1       0		 0	    --> 16
		0       1		 0 	    --> 32
		1       1		 0	    --> 64
		0       0		 1    	--> future use
		1       0		 1	    --> future use
		0       1		 1 	    --> future use
		1       1		 1	    --> future use
 
   L is only considered in test mode 1

  */
#define		L_LOOK_UP_VALUES				{8,16,32,64,99901,99902,99903,99904};

#define		OFDM_CFG_MODE_L_BIT0		    BIT16	
#define		OFDM_CFG_MODE_L_BIT1		    BIT17	
#define		OFDM_CFG_MODE_L_BIT2		    BIT18	
#define		OFDM_CFG_MODE_L_BITS	(OFDM_CFG_MODE_L_BIT0 | OFDM_CFG_MODE_L_BIT1 | \
									 OFDM_CFG_MODE_L_BIT2)	


// number of different orthogonality intervals (N)
#define		NUMBER_OF_N_MODES		8		
// number of different guard intervals (L)
#define		NUMBER_OF_L_MODES		8		




/* datarate bits:
	  Bit 0   Bit 1    Bit 2
		0       0		 0    	--> constant mode with datarate 0
		1       0		 0	    --> constant mode with datarate 1
		0       1		 0 	    --> constant mode with datarate 2
		1       1		 0	    --> constant mode with datarate 3
		0       0		 1    	--> constant mode with datarate 4
		1       0		 1	    --> invalid
		0       1		 1 	    --> invalid
		1       1		 1	    --> fallback/fallforward
 
  */

#define		OFDM_CFG_MODE_DATARATE_BIT0		BIT19	// enable fallback/fallforward
#define		OFDM_CFG_MODE_DATARATE_BIT1		BIT20	// enable fallback/fallforward
#define		OFDM_CFG_MODE_DATARATE_BIT2		BIT21	// enable fallback/fallforward
#define		OFDM_CFG_MODE_DATARATE_BITS		(OFDM_CFG_MODE_DATARATE_BIT0 | \
											 OFDM_CFG_MODE_DATARATE_BIT1 | OFDM_CFG_MODE_DATARATE_BIT2)	

// fallback/fallforward enabled
#define		OFDM_CFG_MODE_DATARATE_FALLBACK		7



/* MOD600 weight:
	  Bit 0             Bit 1
		0                0			--> low				(0)
		1                0			--> medium			(1)
		0                1			--> high			(2)
		1                1			--> proportional	(3)
*/

#define		OFDM_CFG_MODE_WEIGHT_BIT0		BIT22	// MOD600 weight
#define		OFDM_CFG_MODE_WEIGHT_BIT1		BIT23	// MOD600 weight
#define		OFDM_CFG_MODE_WEIGHT_BITS		(OFDM_CFG_MODE_WEIGHT_BIT0 | OFDM_CFG_MODE_WEIGHT_BIT1)	

#define		OFDM_CFG_MODE_ON_OFF_BIT		BIT24	// 0: MOD600 off, 1: MOD600 on disconnectable


/* 4 kHz, 8 kHz Signal Mode
0 -> standard signal duration
1-> reduced signal duration (reduced delay, higher Nyquist Frequency)
*/
#define     OFDM_CFG_MODE_4_8_kHz           BIT25

#define     OFDM_CFG_MODE_INIT				BIT31

// END bitfield defines for OFDM_CFG_MODE bits (t_OFDM_Config.mode)




// OFDM status data

// mode0 field
       
//N
#define		OFDM_STATUS_MODE0_N_BIT0		             BIT0	
#define		OFDM_STATUS_MODE0_N_BIT1		             BIT1	
#define		OFDM_STATUS_MODE0_N_BIT2		             BIT2	
#define		OFDM_STATUS_MODE0_N_BITS	                 (OFDM_STATUS_MODE0_N_BIT0 | OFDM_STATUS_MODE0_N_BIT1 | \
									                      OFDM_STATUS_MODE0_N_BIT2)	
//L
#define		OFDM_STATUS_MODE0_L_BIT0		             BIT3	
#define		OFDM_STATUS_MODE0_L_BIT1		             BIT4	
#define		OFDM_STATUS_MODE0_L_BIT2		             BIT5	
#define		OFDM_STATUS_MODE0_L_BITS	                 (OFDM_STATUS_MODE0_L_BIT0 | OFDM_STATUS_MODE0_L_BIT1 | \
									                      OFDM_STATUS_MODE0_L_BIT2)	
// max. datarate in kbit/s (0..1023)
#define	    OFDM_STATUS_MODE0_MAXRATE_MASK               (BIT6 |BIT7 |BIT8 |BIT9| BIT10|\
                                                          BIT11|BIT12|BIT13|BIT14|BIT15)
// selected data rate: 0-4 or 7 for "no rate selected, yet"
#define 	OFDM_STATUS_MODE0_SELECTED_RATE_MASK	     (BIT16|BIT17|BIT18)
#define		OFDM_STATUS_MODE0_SELECTED_RATE_NONE		 7

// block error rate: 0 for 0, 1 for >10E-6, >10E-5, >10E-4, >10E-3, >10E-2, 6 for >10E-1, 7 for unknown
#define	    OFDM_STATUS_MODE0_BLER_MASK	                 (BIT20|BIT21|BIT22)


// mode1 field

#define	    OFDM_STATUS_MODE1_EPSM_MASK	                  MASK_8LSB
#define	    OFDM_STATUS_MODE1_DELTA_SIGMA_MASK	         (MASK_8LSB << 8)
#define	    OFDM_STATUS_MODE1_SYNC_MASK	                 (MASK_8LSB << 16)
#define 	OFDM_STATUS_MODE1_SNR_MASK	                 (MASK_8LSB << 24)


#define MAX_LENGTH_OFDM_LONG_STATUS 1024 //maximale Größe des vom OFDM-DSP versendeten Datensatzes

typedef enum		 		 // IDs der OFDM Long-Status Datensätze, wird noch weiter ergänzt
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

	e_LongStID_Software_Version_ID  = 100
	
}
e_OFDM_LongStatus_ID;

/* GLOBAL TYPE DECLARATIONS ************************************************/

// OFDM configuration data

typedef struct {
	unsigned long   mode;						// bitfield: OFDM_CFG_MODE bits, see above							
	long			channelBW;					// channel bandwidth [Hz]
	long			dataRate[N_DATA_RATES];	    // [0]-[4]:aggregate datarates
	float           weight;					    // weight of OFDM signal

	// reserved for futur extensions
	float		fReserved1;
	float		fReserved2;
	float		fReserved3;
	float		fReserved4;		
} t_OFDM_Config;


// OFDM status data

typedef struct {

	unsigned long mode0; // N, L, max rate,index to selected data rate, epsm, bler, delta sigma
	unsigned long mode1; // sync mask,  snr

} t_OFDM_ShortStatus;



typedef struct 
{ 
	e_OFDM_LongStatus_ID                  id; 
	unsigned long                         datalength; 
	unsigned long                         offset; 
	unsigned long                         step; 
} t_OFDM_LongStatusHeader; 



typedef struct 
{ 
	t_OFDM_LongStatusHeader          header; 
	float                            data[MAX_LENGTH_OFDM_LONG_STATUS]; 
} t_OFDM_LongStatus;


typedef struct {

	t_OFDM_ShortStatus shortStatus;
} t_OFDM_Status;




#endif
