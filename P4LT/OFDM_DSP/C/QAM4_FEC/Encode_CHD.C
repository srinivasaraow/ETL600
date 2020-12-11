/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: Encode_CHD.C $
 * COPYRIGHT   : 2004 TL Consulting GmbH.
 * DESCRIPTION : Encode channel data QAM4 frame
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/QAM4_FEC/Encode_CHD.C $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 1     1.04.04 vgp
 *       Initial version
 * 2     Execution from SDRAM
 *       06.04.04 vgp
 *****************************************************************************/
#include "../../h/qam4_fec/c3212.h"
#include "../../h/ofdm_defs.h"
#include "../../h/tcm/tcm_defs.h"
#include "../../h/qam4_fec/channel_data.h"
#include "../../h/ofdm_externs.h"

/*===================================*/
int segment ( "seg_ext_pmco" ) Encode_Channel_Data(unsigned int *TCM_Data_Tx, 
                        int          *n_symb_step_tx,
                        Channel_Data_type *Ch_Data_Tx, 
                        int n_tx, 
                        int m_tx,
                        unsigned int *C3212_Data_Tx)
{
   int            i, j, rem;
   unsigned int   *ui_ptr, *tmp_ui_ptr, ui, iw, ibit32_mask, obit32_mask, bit12_mask;
   int            n_c3212_words_tx, n_symb_tx, n_dummy_bits_tx;
   int OFDM_Nbit_QAM4_Tx, OFDM_Num_32b_Words_Tx, Word_32b_Mask_Tx;
   	
   void           CRC32_Encoder(Channel_Data_type *Ch_Data, int n_k_tx);
   unsigned int   Encoder_3212(unsigned int);
   
   // save pointer of temporary array
   tmp_ui_ptr = C3212_Data_Tx;
   
   // Calculate CRC32 word
   CRC32_Encoder(Ch_Data_Tx,n_tx);
   	
   // Calculate OFDM flat modulation QPSK data transfer charactestics
   OFDM_Nbit_QAM4_Tx = (3*m_tx)/2 - CRC_LEN - TERM_BITS;   
   OFDM_Num_32b_Words_Tx = OFDM_Nbit_QAM4_Tx >> 5; 
   Word_32b_Mask_Tx = 0xffffffff << (32 - ( OFDM_Nbit_QAM4_Tx & 0x1f ));
   if ( Word_32b_Mask_Tx == 0 ) Word_32b_Mask_Tx = 0xffffffff;
   else OFDM_Num_32b_Words_Tx++;
   
   //==== Channel Data Encoding by (32.12) code
   ui_ptr = (unsigned int *)Ch_Data_Tx;
   n_c3212_words_tx = n_tx + sizeof(Channel_Data_RX.Settings) + (C3212_N*CHD_32BW_TX)/C3212_K;
   n_symb_tx = (C3212_N*n_c3212_words_tx)/OFDM_Nbit_QAM4_Tx;
   rem = (C3212_N*n_c3212_words_tx)%OFDM_Nbit_QAM4_Tx;
   if ( rem != 0 ) n_symb_tx++;

   // Encode 32-bit data words
   bit12_mask = 1<<(C3212_K-1);
   iw = 0;
   for (i=0; i<CHD_32BW_TX; i++) {
   	 ui = *ui_ptr++;
   	 ibit32_mask = 0x80000000;
   	 while ( ibit32_mask ) {
   	   if ( (ibit32_mask & ui) != 0 ) iw |= bit12_mask;
   	   ibit32_mask >>= 1;
   	   bit12_mask >>= 1;
   	   if ( bit12_mask == 0 ) { 
   	     *C3212_Data_Tx++ = Encoder_3212(iw);
   	     bit12_mask = 1<<(C3212_K-1);
   	     iw = 0;
   	   }
   	 }
   }
   
   // Encode reserved and channel data fields
   for (i=0; i<n_tx + sizeof(Channel_Data_RX.Settings); i++) {
   	 iw = *ui_ptr++ & C3212_MASK;
   	 *C3212_Data_Tx++ = Encoder_3212(iw);
   }

   // restore pointer of temporary array
   C3212_Data_Tx = tmp_ui_ptr;
      
   // Pack data into output OFDM QAM4 buffer
   obit32_mask = 0x80000000;
   iw = 0;
   j = 0;   // output OFDM symbol TCM 32bit word counter
   for (i=0; i<n_c3212_words_tx; i++) {
   	 ui = *C3212_Data_Tx++;
     ibit32_mask = 0x80000000;
   	 while ( ibit32_mask ) {
   	   if ( (ibit32_mask & ui) != 0 ) iw |= obit32_mask;
   	   obit32_mask >>= 1;       
       if ( obit32_mask == 0) {
         if ( ++j == OFDM_Num_32b_Words_Tx ) j = 0;
         *TCM_Data_Tx++ = iw;
         iw = 0;
         obit32_mask = 0x80000000;       	
       } else {           
         if ( j == OFDM_Num_32b_Words_Tx-1 ) {
           if ( (Word_32b_Mask_Tx & obit32_mask) == 0) {
             j = 0;
       	     *TCM_Data_Tx++ = iw;
       	     iw = 0;
             obit32_mask = 0x80000000;       	 
       	   }
         }
       }
 	   ibit32_mask >>= 1;
     } 
   }
   if ( rem != 0 ) *TCM_Data_Tx = iw;
      
   *n_symb_step_tx = OFDM_Num_32b_Words_Tx;
   return n_symb_tx;	
}

