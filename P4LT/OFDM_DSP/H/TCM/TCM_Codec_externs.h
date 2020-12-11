/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: TCM_Codec_externs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec's global variables 
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
extern int   *TCM_Encoder_Tmp_Vec;   // pointer to the encoder temporary vector of length 2*LEN_2D_MAX_TX

extern int   Shaping_Flag_tx;	        // possible values: 0 - shaping OFF, 1 - Shaping ON
extern int   d2alloc_tx[(LEN_2D_MAX_TX+1)/2];   // SW addressing, QAM Modulation vector QAM_order[i]/2-2, possible values of elements: QPSK - -1, 16QAM - 0, 64-QAM - 1, 256QAM - 2, ...16384QAM -5

extern int   OFDM_Shaping_Flag_TX;        // 1 - shaping encoder ON, 0 -shaping enoder off
extern int   OFDM_Num_Bits_TX;     // overall TCM number of information bits per OFDM symbol
extern int   OFDM_Code_Bits_TX;    // number of information bits to be encoded by 4/4 trellis encoder 
extern int   OFDM_Gray_Bits_TX;    // number of informtation bits to be encoded by Gray code
extern int   OFDM_Shaping_Bits_TX; // number of information bits to be encoded by shaping encoder
extern float OFDM_QAM_Energy_TX;
extern int   OFDM_Num_32b_Words_TX;  // overall number of 32bit inf. words in TCM frame, for given qam order and shaping status

extern int *Bit_Buffer_TX_Ptr;      // points TX bit buffer
extern int *TX_Code_Bits_Ptr;    // points first element in Bit_Buffer_TX[] to be encoded by 3/4 trellis code
extern int *TX_Gray_Bits_Ptr;    // points first element in Bit_Buffer_TX[] to be encoded by Gray code    
extern int *TX_Shaping_Bits_Ptr; // points first element in Bit_Buffer_TX[] to be encoded by shaping code

extern complex_float *Sig_TX_Ptr;

// =============== TCM codec globals RX
extern int   *TCM_Decoder_Tmp_Vec;   // pointer to the encoder temporary vector of length 9*LEN_4D_MAX_RX

extern int   Shaping_Flag_rx;	        // possible values: 0 - shaping OFF, 1 - Shaping ON
extern int   d2alloc_rx[(LEN_2D_MAX_RX+1)/2];   //SW addressing, QAM Modulation vector QAM_order[i]/2-2, possible values of elements: QPSK - -1, 16QAM - 0, 64-QAM - 1, 256QAM - 2, ...16384QAM -5

extern int   OFDM_Shaping_Flag_RX;        // 1 - shaping encoder ON, 0 -shaping enoder off
extern int   OFDM_Num_Bits_RX;     // overall TCM number of information bits per OFDM symbol
extern int   OFDM_Code_Bits_RX;    // number of information bits to be encoded by 4/4 trellis encoder 
extern int   OFDM_Gray_Bits_RX;    // number of informtation bits to be encoded by Gray code
extern int   OFDM_Shaping_Bits_RX; // number of information bits to be encoded by shaping encoder
extern float OFDM_QAM_Energy_RX;
extern int   OFDM_Num_32b_Words_RX;  // overall number of 32bit inf. words in TCM frame, for given qam order and shaping status

extern int *Bit_Buffer_RX_Ptr;
extern int *RX_Code_Bits_Ptr;
extern int *RX_Gray_Bits_Ptr;
extern int *RX_Shaping_Bits_Ptr;

extern complex_float *Sig_RX_Ptr;
extern complex_float *Sig_RX_Remod_Ptr;
extern float         *ETF_Ptr;
extern float         *ETF;

//=============== PCM data =============

extern int *In_PCM_Ptr;
extern int *Out_PCM_Ptr;

extern int PCM_32b_Mask_RX;
extern int PCM_32b_Mask_TX;

extern int           PCM_Generator;
extern unsigned int  PCM_Seed;
extern unsigned int  Dummy_Seed;
extern unsigned int  DeltaBits_Seed;
extern unsigned int  MQAM_Seed_TX;
extern unsigned int  MQAM_Seed_RX;

extern int In_PCM_Buffer[PCM_BUFFER_LENGTH_TX];
extern int Out_PCM_Buffer[PCM_BUFFER_LENGTH_RX];

extern int          BER_Meter;
extern unsigned int BER_Seed;

extern unsigned int Bit_Errors_Sum;
extern unsigned int Block_Errors_Sum;
extern unsigned int BER_Block_Counter;
extern float        Block_Error_rate;
extern float        BER;
extern float        BLER;
extern float        BLER_Est;

extern unsigned int BER_Buffer[PCM_BUFFER_LENGTH_RX];

//=============== Scrambler/Descrambler data =============
extern int Scrambling;
extern int Scramble_Data[MAX_SCRAMBLE_TX];
extern int Descramble_Data[MAX_SCRAMBLE_RX];

// *************** Segment extn ****************

// ============= Bit Allocation for QAM-M ==================
// =============== TCM encoder (transmitter) globals
extern int QAM_vec_tx[LEN_2D_MAX_TX];

// =============== TCM decoder (receiver) globals
extern int QAM_vec_rx[LEN_2D_MAX_RX];

// ============= Bit Allocation for QAM-4 ==================
// =============== TCM encoder (transmitter) globals
extern int QAM4_vec_tx[LEN_2D_MAX_TX];

// =============== TCM decoder (receiver) globals
extern int QAM4_vec_rx[LEN_2D_MAX_RX];

// filled by tcm_bitcalc.c function
extern int   TCM_Code_Bits, TCM_Gray_Bits, TCM_Shaping_Bits, TCM_Num_Inf_Bits, TCM_Shaping_Flag;  
extern float TCM_QAM_Energy;

float Shaping_gain[];

