
/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Defs.h $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM modem main parameter definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /branches/Source MAS/P4LT/OFDM_DSP/H/OFDM_Defs.h $
 * 
 * 27    24.07.08 15:17 Chhamaa
 * 
 * 25    07-06-01 14:53 Chchlee
 * Initial dual pilot tone levels reduced by 3dB
 * 
 * 21    5.04.06 15:28 Chhamaa
 * 
 * 21    17.03.06 14:41 Chhamaa
 * 
 * 19    30.09.05 10:54 Chalgyg
 * OFDM_DSP_V0_11_ur (12709051)
 * 
 * 7     26.04.05 8:45 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 4     24.02.05 7:34 Chchlee
 * 
 * 3     1.02.05 13:36 Chalgyg
 * Version 0.09A
 * 
 * 5     2.12.04 18:22 Chhamaa
 * 
 * 4     30.09.04 8:42 Leeb02
 * 
 * 
 * Initial version
 *       15.02.2003 vgp
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * "CHD_RES" 
 *       06.04.04 vgp
 *****************************************************************************/

#ifndef OFDM_DEFS_H
#define OFDM_DEFS_H
 
#include "./tcm/tcm_defs.h"
//#include "./QAM4_FEC/C3212.h"
//#include "./QAM4_FEC/channel_data.h"


#define  ANALOG_LOOP	0
#define  DIGITAL_LOOP	1

#define  OFDM_N_MAX       512          // Max OFDM symbol length in complex samples
#define  OFDM_L_MAX       (OFDM_N_MAX/8)          // MAX cyclic extension in complex samples
#define  OFDM_L_MIN       8             // MIN cyclic extension in complex samples
#define  OFDM_NL_MAX      (OFDM_N_MAX+OFDM_L_MAX)          // MAX symbol lrngth in complex samples
#define  OFDM_NL2_MAX      (2*OFDM_NL_MAX)

#define OFDM_M_MAX		  3*OFDM_N_MAX/4  
#define INTERP_MAX		  8			// maximum interpolation coefficient

#define N_BASE 512		// start parameters for NL-Optimization
#define L_BASE  64

#define PILOT0_DEFAULT   N/4
#define PILOT1_DEFAULT 3*N/4
#define PILOT0_NL_OPT  3*N/8
#define PILOT1_NL_OPT  5*N/8
#define PILOT1_MIDDLE	 N/2

#define  OFDM_PILOTS_NUM  2

#define  DET_WIN_LEN      3

#define  OFDM_MIN_FLOAT   1.0e-6f

#define  OFDM_PI          3.14159265358979f
#define  OFDM_TWO_PI      (2.0f*OFDM_PI)

#define  OFDM_MASTER           1
#define  OFDM_SLAVE            0

#define  LEN_DF_EST_DATA  24

#define  N_OFDM_SYMBOLS_PER_SUBBLOCK 6
#define  DF_EST_SUBBLOCKS 3
//#define  LEN_DF_EST       DF_EST_SUBBLOCKS*(N_OFDM_SYMBOLS_PER_SUBBLOCK+3) //18
#define  LEN_DF_EST       DF_EST_SUBBLOCKS*(N_OFDM_SYMBOLS_PER_SUBBLOCK+1)+4 //18
#define  LEN_FFT_DF_EST   16

#define  N_EST_SYMBOLS    12


#define  K_WORD_L         12

#define  ON               1
#define  OFF              0

#define  FS_TX_RX         64000.0f    // TX/RX complex sampling frequency in Hz

#define  WAITING_SYMBOLS  4000

#define  DETECTION_THRESHOLD	0.85f

#define SHAPING_GAIN            3.8f

// Preamble_scheduler_TX controls the following states
typedef enum
{
TX_NL_Mode_Call	  		=0,
TX_2TONE_1			  	=1,  
TX_PAUSE		      	=2,
TX_2TONE_2			  	=3,
TX_LFM		      	  	=4,
TX_2TONE_3			  	=5,
TX_Preamble_IDLE	  	=6
} t_TX_Preamble_State;

#define  N_SYMBOLS_TONE1 	  11 // PT=6
#define  N_SYMBOLS_PAUSE  	  (N_EST_SYMBOLS+13) //PT=20
#define  N_SYMBOLS_TONE2  	  (LEN_DF_EST+8) //PT=18
#define  N_SYMBOLS_LFM    	   6 //!this value is doubled in
								 //Preamble_scheduler_TX if N==64
								 //due to doubled length of LFM sequence
#define  N_SYMBOLS_TONE3   	   8 //PT=3
#define  N_SYMBOLS_QAM4_DUMMY 20
#define  N_SYMBOLS_M_QAM_TRAINING		50  //100
#define  N_SYMBOLS_M_QAM_TRAINING_LONG  200


// QAM_scheduler_TX controls the following states
// TX_OFDM_Engine_state 's 
typedef enum
{
TX_Engine_QAM4_DUMMY 		=0,  
TX_Engine_QAM4_DATA  		=1,  
TX_Engine_QAM_M_Training	=2,
TX_Engine_QAM_M_Data		=3 
} t_TX_OFDM_Engine_State;

//TX_QAM_State 's
typedef enum
{
TX_QAM_DUMMY1		  	=0,
TX_QAM_CH_DATA_NL_PREP 	=1,
TX_QAM_CH_DATA_PREP  	=2,
TX_QAM_NL_OPTIM	  		=3,
TX_QAM_PREFIX		  	=4,
TX_QAM_CH_DATA       	=5,
TX_QAM4_DUMMY2       	=6,
TX_QAM_DELAY_SUFFIX  	=7,
TX_QAM_SUFFIX        	=8,
TX_M_QAM_TRAINING    	=9,
TX_M_QAM_DATA        	=10,
TX_WAIT_FOR_RESTART  	=11
} t_TX_QAM_State;

// Preamble_scheduler_RX controls the following states
// RX_Preamble_State 's
typedef enum
{
RX_DETECTION_MODE	  =0,	
RX_DETECTION_AGC	  =1, 
RX_NOISE_EST	      =2,  
RX_FREQUENCY_EST	  =3, 
RX_LFM_PROC	      	  =4,  
RX_QAM4_DETECT        =5,
RX_Preamble_IDLE      =6
} t_RX_Preamble_State;

// QAM_scheduler_RX controls the following states
// RX_OFDM_Engine_state 's
typedef enum
{
RX_Engine_IDLE		  		=0,
RX_Engine_QAM4		  		=1,
RX_Engine_QAM_M_Training	=2,
RX_Engine_QAM_M_Data		=3
} t_RX_OFDM_Engine_state;

// RX_QAM_State 's
typedef enum
{
RX_QAM_IDLE		  			=0,
RX_QAM_PREFIX		  		=1,
RX_QAM_CH_DATA_NL	  		=2,
RX_QAM_CH_DATA		  		=3,
RX_QAM_CH_DATA_DECODE		=4,
RX_TX_NL_SELECTION			=5,
RX_TX_OPTIMIZATION_1  		=6,
RX_TX_OPTIMIZATION_2  		=7,
RX_TX_OPTIMIZATION_3  		=8,
RX_QAM_SUFFIX		  		=9,
RX_M_QAM_TRAINING		  	=10,
RX_M_QAM_DATA		  		=11,
RX_WAIT_FOR_RESTART  		=12
} t_RX_QAM_State;

#define OFDM_MAX_NBIT_QAM4_TX  ((3*OFDM_M_MAX)/2 - CRC_LEN - TERM_BITS)
#define OFDM_MAX_NBIT_QAM4_RX  ((3*OFDM_M_MAX)/2 - CRC_LEN - TERM_BITS)
#define OFDM_NUM_32BW_TX       ((OFDM_MAX_NBIT_QAM4_TX>>5)+1)
#define OFDM_NUM_32BW_RX       ((OFDM_MAX_NBIT_QAM4_RX>>5)+1)

// RX_PREAMB Counter Tresholds
#define RX_PILOTMODE_TIMEOUT_TIME  10.0f 	//if no pilot is detected in this time (seconds)
											//->restart in Starttyp I	
#define MAX_RX_MUTE_START		   N_SYMBOLS_TONE1 + 10
#define MAX_RX_MUTE_STOP  		   N_EST_SYMBOLS + 10
#define MAX_RX_LFM_WAIT			   N_SYMBOLS_TONE2 + 3            
#define MAX_RX_QAM4_WAIT		   N_SYMBOLS_TONE3 + 2*N_SYMBOLS_LFM           

// RX_QAM Counter Tresholds
#define MAX_RX_PREFIX_CYCLES       N_SYMBOLS_QAM4_DUMMY + 10
#define MAX_RX_SUFFIX_CYCLES	   60



#define  RO			  	  0.01f  

#define  MAX_CRC_ERROR_BURST	150 //16 PL
//#define  MAX_LEVEL_ERROR_BURST	16

#define  L_INT_FILT		  25
#define INTERPOLATION_FACTOR	4

#define  QAM4		      4
#define  QAM16		      16
#define  QAM64		      64
#define  QAM256		      256
#define  QAM1024		  1024
#define  QAM4096		  4096
#define  QAM16384		  16384

#define  MAXBITS_IN_SC    14

#define SKP_NUM 10000

#define MAX_INOUT_DATA_BUF   224 // for 16384qam without shaping; 512*14/32
#define MAX_SCRAMBLE         100*MAX_INOUT_DATA_BUF // length of scramble data

#define  POS			  0  
#define  NEG			  1  

#define  A_2TONE          22.627f // 32.0f / SQRT(2) = 32.0f - 3dB
#define  A_LFM             4.0f


/*****************************************************************
**					Optimization			 					**
*****************************************************************/
// Schwellen in optim1()
#define TH_OPTIM1_PMIN   0.5f  //Mindestleistung, die erreicht sein muss
									// damit Teilkanal verwendet wird P_i>TH_OPTIM1_PMIN
#define TH_OPTIM1_DISCR  0.2f  // Schwelle fuer Quantisierung von kontinuierlichem R_i auf
								// diskrete Anzahl an Bits
								// 0.0f bedeutet immer abrunden (sehr kondervativ)
								// 0.5f bedeutet normales runden (im Grenzfall gefaehrlich)
								// 0.2f Kompromiss (zu testen) 									
// Schwellen in optim2()
#define TH_OPTIM2_QAM4  1.3f	// Legt fest ab welchem MindestWert (Anzahl an Inf-Bits pro Teilkanal)
								// bei Diskretisierung ein Teilkanal mit QAM4 moduliert wird
								// bei QAM4 sind pro Traegerpaar 3 Inf-Bit, d.h. pro Kanal 1.5 Inf-Bit
								
/*****************************************************************
**					Narrowband Noise Detection 					**
*****************************************************************/
#define  NB_WINDSIZE			1
#define  NB_FS_THRESHOLD        20.0f  // NB is detected if NB_level > Noise_Level + 13dB(20.0f)
#define  NB_WIDTH_THRESHOLD		-0.05f
#define  NB_NOISE_THRESHOLD     0.00003f //NB level must exceed this THRED to be detected (-45dB)
#define  NB_INTERPOLATION       ON

typedef struct    // sollte in ein File OFDM_data_types.h ausgelagert werden
{
	int Start;
	int Width;
	float F_Stat_Max;
} Inf_Array_NB_type;


/*****************************************************************
**					Channel Data Transmission 					**
*****************************************************************/
//-- normal mode NL selected
#define CHD_RES 20
typedef struct    // sollte evtl. in ein File OFDM_data_types.h ausgelagert werden
{
	int   CRC;					//32 bit	
	float AWGN_sigma;   		//32 bit
	float K_scale;      		//32 bit
	int   Settings[CHD_RES]; 	//20*12 bit
	int K_int[OFDM_N_MAX];   	//OFDM_N_MAX*12bit
								//+===============
} Channel_Data_type;			// 540 * 12 bit


//#define MAX_C3212_BUF (sizeof(Channel_Data_TX.K_int)+sizeof(Channel_Data_TX.Settings)+(C3212_N*CHD_32BW_TX)/C3212_K)
//#define MAX_QAM4_BUF        (((C3212_N*MAX_C3212_BUF_TX)/OFDM_MAX_NBIT_QAM4_TX + 1)*OFDM_NUM_32BW_TX)
#define MAX_C3212_BUF   540		// Channel_Date RS-Input-Size  = 540 * 12 bit words [sizeof(Channel_Data_XX)] 	
								// Channel_Data RS-Output-Size = 540 * 32 bit words = 17280bit
								// N_bits_per_OFDM_QAM4_Symbol_min = 3/2 * M_512_min -9 = 495bit = 16 *32 bit words
								// 17280 channel data bits / 495bit_per_OFDM_QAM4_Symbol = 35 OFDM Symbols for channel data  
#define MAX_QAM4_BUF    566  	// 35 OFDM Symbols * 16 32bit_words_per_OFDM_QAM4_Symbol = 560 
								// 6 words oversized to be on safe side
//-- NL-Optim Mode
#define MAX_CHD_NL_NTX		6	//maximum number of 32bit words at input of RS3212 encoder,
								//that are transmitted during one OFDM symbol,
								// if N=512(M=384,QAM4) in NL-Optim-Mode
#define MAX_CHD_NL_ENCNTX  (8*MAX_CHD_NL_NTX/3) 
								//maximum number of 32bit words at output of RS3212 encoder,
								//that are transmitted during one OFDM symbol,
								// if N=512(M=384,QAM4) in NL-Optim-Mode
								// must be less than OFDM_Num_32b_Words_TX							
typedef struct    // sollte evtl. in ein File OFDM_data_types.h ausgelagert werden
{
	int 	N;			//1: 32 bit	
	int 	L;   		//2: 32 bit
	float	Settings[MAX_CHD_NL_NTX-2];
	
} Channel_Data_type_NL;
								
/*****************************************************************
**					Bit Stuffing				 					**
*****************************************************************/
#define STUFFING_ERROR_CONTROL OFF
//#define STUFFING_ERROR_CONTROL ON
#define STUFFING_ERROR_THRES   20


/*****************************************************************
**					Service Bits				 					**
*****************************************************************/
// Position of Service Bits
// if you do changes here, remember to adjust maximum number of Service Bits
//			in Verify_Modem_Configuration()
#define SERVICE_BIT_POS_FFwd 0 
#define SERVICE_BIT_POS_rsvd 1

/*****************************************************************
**					Error Codes				 					**
*****************************************************************/
typedef enum
{
NO_ERROR					=0,	
// ERROR Codes   TX: 1XX  (Warning: 3xx) 	
//				 RX: 2XX  (Warning: 4xx)

// Location Configuration
ERLOC_CONFIG		    	=10,
// IDs
WRNG_MASTER_MASTER_CNFG		=101,
WRNG_NL_BW_CNFG				=102,

// Location RX_PREAMB
ERLOC_RX_PREAMB		    	=20,
// IDs
RX_MUTE_START_ERROR			=201,
RX_MUTE_STOP_ERROR			=202,
RX_LFM_WAIT_ERROR			=203,
RX_REF_PILOT_WRN			=403,
RX_QAM4_WAIT_ERROR			=204,
RX_GNRL_2T_WAIT_ERROR		=205,

// Location RX_QAM4
ERLOC_RX_QAM4     			=21,
// IDs
ERROR_PREFIX				=211,
ERROR_CH_DATA				=212,
ERROR_OPTIM1				=213,
ERROR_OPTIM2				=214,
ERROR_OPTIM3				=215,
ERROR_SUFFIX				=216,
ERROR_RATE_OUT_OF_RANGE		=217,
ERROR_STUFFING_SETTING		=218,
ERROR_SUFFIX_DURING_OPTIM	=219,
WRNG_NB_ON_PILOT			=413,

// Location M_QAM
ERLOC_RX_M_QAM_DATA			=22,
//IDs
ERROR_CRC					=221,
ERROR_LEVEL					=222,
ERROR_STUFFING				=223,
ERROR_TCM_INIT				=224,
ERROR_ABBRUCH				=225,
ERROR_FALLBACK				=226,
ERROR_FALLFORWARD			=227
	
} t_Single_Error_ID;

typedef struct
{
	t_Single_Error_ID ID;
	int Location;
} t_Single_Error_type;

#define ERROR_LOG_SIZE 110

typedef struct    // sollte evtl. in ein File OFDM_data_types.h ausgelagert werden
{
	int   				Count;			 //Total Number of occured Errors	
	t_Single_Error_type   Number[ERROR_LOG_SIZE];     //Properties of singel Error
	
} t_Error_type;


#endif
