/******************************************************************************
 *
 * PROJECT     : DRS7  
 * FILE        : $Workfile: TCM_Encoder.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/TCM_Encoder.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Memory usage reduction
 *       17.01.2004 vgp
 * Short word storage and addressing for RX and TX bit buffers
 * IN INTERNAL MEMORY (DOESN'T WORK WITH EXTERNAL MEMORY)
 *       18.01.2004 vgp
 * Short word storage and addressing for d2alloc_tx[] d2alloc_rx[] buffers
 *       18.01.2004 vgp
 * Execution from SDRAM
 *       06.04.04 vgp
 * TCM_CRC5_Encoder(), TCM_Shaping_Encoder() calling parameters modification
 *       06.09.04 vgp
 *****************************************************************************/

#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

void segment ( "seg_ext_pmco" ) Trellis_Encoder(int * InfBits_Ptr, complex_float * QAM_vector, int len2d, int *TCM_encoder_tmp)
{
    int *d2alloc_ptr;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//!!!!	TCM_encoder_tmp points free memory of 2*LEN_2D_MAX_RX words 
//!!!!  TCM_decoder_tmp[] array must reside INTERNAL SRAM memory
//!!!!  InfBits_Ptr must points internal SRAM memory
//!!!!  d2alloc_tx[] array must reside INTERNAL SRAM memory
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
    extern int           *TX_Code_Bits_Ptr;     // points information bits to be encoded by 3/4 trellis code
    extern int           *TX_Gray_Bits_Ptr;     // points information bits to be encoded by gray code
    extern int           *TX_Shaping_Bits_Ptr;  // points information bits to be encoded by shaping code
    extern complex_float *Sig_TX_Ptr;           // pointer of QAM modulated vector
    extern int           OFDM_Shaping_Flag_TX;     // shaping flag: 1 - on, 0 - off
    extern int           OFDM_Num_Bits_TX;         // number of inf. bits in TCM scheme
    extern int           OFDM_Code_Bits_TX;        // number of inf. bits in TCM scheme to be encoded by 3/4 trellis code
    extern int           OFDM_Gray_Bits_TX;        // number of inf. bits in TCM scheme to be encoded by Gray code
    extern int           OFDM_Shaping_Bits_TX;     // number of inf. bits in TCM scheme to be encoded by Shaping code

    void TCM_CRC5_Encoder(int );      // CRC5 calculation
    void TCM_Trellis_34_Encoder(int, int *);      // Trellis 3/4 encoding
    void TCM_Gray_Encoder(int *, int);    // Gray encoding 
    void TCM_Shaping_Encoder(int *, int, int, int); // Shaping encoding and QAM signal mapping

        
    // deposit encoder's pointers with required values
    TX_Code_Bits_Ptr = (int *)(2*(unsigned int)InfBits_Ptr);
    TX_Gray_Bits_Ptr = TX_Code_Bits_Ptr+CRC_LEN+OFDM_Code_Bits_TX;
    TX_Shaping_Bits_Ptr = TX_Gray_Bits_Ptr+OFDM_Gray_Bits_TX;
    Sig_TX_Ptr = QAM_vector;

    d2alloc_ptr = (int *)(2*(unsigned int)d2alloc_tx);  // enable SW addressing mode
                	
    TCM_CRC5_Encoder(OFDM_Num_Bits_TX);        // CRC5 calculation
    TCM_Trellis_34_Encoder(len2d, TCM_encoder_tmp);      // Trellis 3/4 encoding
    TCM_Gray_Encoder(d2alloc_ptr, len2d);    // Gray encoding 
    TCM_Shaping_Encoder(d2alloc_ptr, len2d, OFDM_Shaping_Flag_TX, OFDM_Shaping_Bits_TX); // Shaping encoding and QAM signal mapping
}

