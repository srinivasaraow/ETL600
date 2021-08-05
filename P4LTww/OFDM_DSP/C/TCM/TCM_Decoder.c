/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Decoder.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/TCM_Decoder.c $
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
 *       16.01.2004 vgp
 * Input/output parameter revision revision
 *       17.01.2004 vgp
 * Short word storage and addressing for RX and TX bit buffers
 * IN INTERNAL MEMORY (DOESN'T WORK WITH EXTERNAL MEMORY)
 *       18.01.2004 vgp
 * Short word storage and addressing for d2alloc_tx[] d2alloc_rx[] buffers
 *       18.01.2004 vgp
 * Execution from SDRAM
 *       06.04.04 vgp
*****************************************************************************/
#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_codec_externs.h"

int segment ( "seg_ext_pmco" ) Trellis_Decoder(complex_float * In_QAM_vector, float *ETF_vector, int * InfBits_Ptr, complex_float * Remod_QAM_vector, int len2d, int *TCM_decoder_tmp) 
{
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//!!!!	TCM_decoder_tmp points free memory of 9*LEN_4D_MAX_RX words 
//!!!!  TCM_decoder_tmp[] array must reside INTERNAL SRAM memory
//!!!!  InfBits_Ptr must points internal SRAM memory
//!!!!  d2alloc_rx[] array must reside INTERNAL SRAM memory
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int *d2alloc_ptr;
    	
    extern complex_float *Sig_RX_Ptr;           // pointer of received QAM modulated vector
    extern complex_float *Sig_RX_Remod_Ptr;     // pointer of decoder's QAM remodulated vector
    extern float         *ETF_Ptr;
    extern int           *RX_Code_Bits_Ptr;     // points information bits to be decoded by 3/4 trellis code
    extern int           *RX_Gray_Bits_Ptr;     // points information bits to be decoded by gray code
    extern int           *RX_Shaping_Bits_Ptr;  // points information bits to be decoded by shaping code
    extern int           OFDM_Shaping_Flag_RX;     // shaping flag: 1 - on, 0 - off
    extern int           OFDM_Num_Bits_RX;         // number of inf. bits in TCM scheme
    extern int           OFDM_Code_Bits_RX;        // number of inf. bits in TCM scheme to be encoded by 3/4 trellis code
    extern int           OFDM_Gray_Bits_RX;        // number of inf. bits in TCM scheme to be encoded by Gray code

    void TCM_Trellis_34_ViterbiDecoder(int *,int, int *);                      // Metrics calculation & TCM 3/4 Viterby decoder
    void TCM_Decoder_Extract_Gr_Cd_Bits(int *,int);     // Extractor of 3/4 trelis decoded and gray decoded information bits
    void TCM_Shaping_Decoder(int *, int);               // Extractor of shaping decoded information bits
    int  TCM_CRC5_Decoder();                            // CRC calculator
    void TCM_Decoder_Remodulate(int *, int);            // Decoder's QAM vector remodulator
    	
    Sig_RX_Ptr = In_QAM_vector;                 // pointer of the received QAM vector
    Sig_RX_Remod_Ptr = Remod_QAM_vector;        // pointer of the remodulated by decoder QAM vector
    ETF_Ptr = ETF_vector;                       // pointer of estimated transfer channel function
	
    RX_Code_Bits_Ptr = (int *)(2*(unsigned int)InfBits_Ptr);
    RX_Gray_Bits_Ptr = RX_Code_Bits_Ptr+CRC_LEN+OFDM_Code_Bits_RX;
    RX_Shaping_Bits_Ptr = RX_Gray_Bits_Ptr+OFDM_Gray_Bits_RX;

    d2alloc_ptr = (int *)(2*(unsigned int)d2alloc_rx);  // enable SW addressing mode
	
    TCM_Trellis_34_ViterbiDecoder(d2alloc_ptr,len2d,TCM_decoder_tmp);
    TCM_Decoder_Extract_Gr_Cd_Bits(d2alloc_ptr,len2d);  
    TCM_Shaping_Decoder(d2alloc_ptr, len2d); 
    TCM_Decoder_Remodulate(d2alloc_ptr, len2d);             

    return TCM_CRC5_Decoder();     
}

