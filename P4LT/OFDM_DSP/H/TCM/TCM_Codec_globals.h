/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: TCM_Codec_globals.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec's global variables definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       15.02.2003 vgp
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * BER tester
 *      23.01.2004 vgp
 * Delta bits generation support
 *      12.05.2004 vgp
 *****************************************************************************/

#include "../../h/tcm/TCM_Codec.h"
#include <complex.h>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//                   Short Word (16 bit) addressing 
//!!!!  Bit_Buffer_TX[] array must reside in INTERNAL SRAM memory
//!!!!  Bit_Buffer_RX[] array must reside in INTERNAL SRAM memory
//!!!!  d2alloc_tx[] array must reside in INTERNAL SRAM memory
//!!!!  d2alloc_rx[] array must reside in INTERNAL SRAM memory
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	

// *************** Segment dmda ****************

// =============== TCM encoder (transmitter) globals
volatile int   *TCM_Encoder_Tmp_Vec;   // pointer to the encoder temporary vector of length 2*LEN_2D_MAX_TX

int   Shaping_Flag_tx;	        // possible values: 0 - shaping OFF, 1 - Shaping ON
int   d2alloc_tx[(LEN_2D_MAX_TX+1)/2];   // SW addressing, QAM Modulation vector QAM_order[i]/2-2, possible values of elements: QPSK - -1, 16QAM - 0, 64-QAM - 1, 256QAM - 2, ...16384QAM -5

int   OFDM_Shaping_Flag_TX;        // 1 - shaping encoder ON, 0 -shaping enoder off
int   OFDM_Num_Bits_TX;     // overall TCM number of information bits per OFDM symbol
int   OFDM_Code_Bits_TX;    // number of information bits to be encoded by 4/4 trellis encoder 
int   OFDM_Gray_Bits_TX;    // number of informtation bits to be encoded by Gray code
int   OFDM_Shaping_Bits_TX; // number of information bits to be encoded by shaping encoder
float OFDM_QAM_Energy_TX;
int   OFDM_Num_32b_Words_TX;  // overall number of 32bit inf. words in TCM frame, for given qam order and shaping status

int *Bit_Buffer_TX_Ptr;      // points TX bit buffer
int *TX_Code_Bits_Ptr;    // points first element in Bit_Buffer_TX[] to be encoded by 3/4 trellis code
int *TX_Gray_Bits_Ptr;    // points first element in Bit_Buffer_TX[] to be encoded by Gray code    
int *TX_Shaping_Bits_Ptr; // points first element in Bit_Buffer_TX[] to be encoded by shaping code

complex_float *Sig_TX_Ptr;

// =============== TCM decoder (receiver ) globals
int   *TCM_Decoder_Tmp_Vec;   // pointer to the encoder temporary vector of length 9*LEN_4D_MAX_RX

int   Shaping_Flag_rx;	        // possible values: 0 - shaping OFF, 1 - Shaping ON
int   d2alloc_rx[(LEN_2D_MAX_RX+1)/2];   //SW addressing, QAM Modulation vector QAM_order[i]/2-2, possible values of elements: QPSK - -1, 16QAM - 0, 64-QAM - 1, 256QAM - 2, ...16384QAM -5

int   OFDM_Shaping_Flag_RX;        // 1 - shaping encoder ON, 0 -shaping enoder off
int   OFDM_Num_Bits_RX;     // overall TCM number of information bits per OFDM symbol
int   OFDM_Code_Bits_RX;    // number of information bits to be encoded by 4/4 trellis encoder 
int   OFDM_Gray_Bits_RX;    // number of informtation bits to be encoded by Gray code
int   OFDM_Shaping_Bits_RX; // number of information bits to be encoded by shaping encoder
float OFDM_QAM_Energy_RX;
int   OFDM_Num_32b_Words_RX;  // overall number of 32bit inf. words in TCM frame, for given qam order and shaping status

int *Bit_Buffer_RX_Ptr;
int *RX_Code_Bits_Ptr;
int *RX_Gray_Bits_Ptr;
int *RX_Shaping_Bits_Ptr;

complex_float *Sig_RX_Ptr;
complex_float *Sig_RX_Remod_Ptr;
float         *ETF_Ptr;
float         *ETF;

//=============== PCM data & BER tester =============

int *In_PCM_Ptr;
int *Out_PCM_Ptr;

int PCM_32b_Mask_RX;
int PCM_32b_Mask_TX;

int           PCM_Generator;
unsigned int  PCM_Seed;
unsigned int  Dummy_Seed;
unsigned int  DeltaBits_Seed;
unsigned int  MQAM_Seed_TX;
unsigned int  MQAM_Seed_RX;


int segment ( "seg_extn SHT_NOBITS" ) In_PCM_Buffer[PCM_BUFFER_LENGTH_TX];
int segment ( "seg_extn SHT_NOBITS" ) Out_PCM_Buffer[PCM_BUFFER_LENGTH_RX];

int          BER_Meter;
unsigned int BER_Seed;

unsigned int Bit_Errors_Sum;
unsigned int Block_Errors_Sum;
unsigned int BER_Block_Counter;
float        Block_Error_rate;
float        BER;
float        BLER;
float        BLER_Est;

unsigned int segment ( "seg_extn SHT_NOBITS" ) BER_Buffer[PCM_BUFFER_LENGTH_RX];

//=============== Scrambler/Descrambler data =============
int Scrambling;
int segment ( "seg_extn SHT_NOBITS" ) Scramble_Data[MAX_SCRAMBLE_TX];
int segment ( "seg_extn SHT_NOBITS" ) Descramble_Data[MAX_SCRAMBLE_RX];

// *************** Segment extn ****************

// ============= Bit Allocation for QAM-M ==================
// =============== TCM encoder (transmitter) globals
int segment ( "seg_extn SHT_NOBITS" ) QAM_vec_tx[LEN_2D_MAX_TX];

// =============== TCM decoder (receiver) globals
int segment ( "seg_extn SHT_NOBITS" ) QAM_vec_rx[LEN_2D_MAX_RX];


// ============= Bit Allocation for QAM-4 ==================
// =============== TCM encoder (transmitter) globals
int segment ( "seg_extn SHT_NOBITS" ) QAM4_vec_tx[LEN_2D_MAX_TX];

// =============== TCM decoder (receiver) globals
int segment ( "seg_extn SHT_NOBITS" ) QAM4_vec_rx[LEN_2D_MAX_RX];

// filled by tcm_bitcalc.c function
int   segment ( "seg_extn SHT_NOBITS" ) TCM_Code_Bits, TCM_Gray_Bits, TCM_Shaping_Bits, TCM_Num_Inf_Bits, TCM_Shaping_Flag;  
float segment ( "seg_extn SHT_NOBITS" ) TCM_QAM_Energy;

//                           Modulation QAM  order      0      2      4      6     8      10       12      14
float segment ( "seg_extn_init" ) Shaping_gain[] = {0.000f,0.000f,3.735f,3.900f,3.960f, 3.960f, 3.960f, 3.960f};
