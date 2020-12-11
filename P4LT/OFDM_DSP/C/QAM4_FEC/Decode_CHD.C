/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: Decode_CHD.C $
 * COPYRIGHT   : 2004 TL Consulting GmbH.
 * DESCRIPTION : Decode channel data QAM4 frame
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/QAM4_FEC/Decode_CHD.C $
 * 
 * 7     25.07.08 13:56 Chhamaa
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


unsigned int segment ( "seg_extn SHT_NOBITS" ) RS_Decoder_3212Array[OFDM_N_MAX+20+8];
unsigned int DecodeCD_State = 0;


/*===================================*/
//int segment ( "seg_ext_pmco" ) Decode_Channel_Data(Channel_Data_type *Ch_Data_Rx, 
int Decode_Channel_Data(Channel_Data_type *Ch_Data_Rx, 
                        unsigned int *TCM_Data_Rx, 
                        int n_rx, 
                        int m_rx,
                        int *Decoding_Result)
{
	
   int            i, j, rem;
   //unsigned int   *ui_ptr, *tmp_ui_ptr;
   unsigned int   ui, iw, rw, ibit32_mask, obit32_mask, bit12_mask;
   int            Is_Error, Is_CRC_Error, n_symb_rx, n_symb_step_rx;
   //int  Is_Decoder_Error;
   int            n_c3212_words_rx, Decoder3212_status;
   int            OFDM_Nbit_QAM4_Rx, OFDM_Num_32b_Words_Rx, Word_32b_Mask_Rx;
   
   int Decoding_finished;
               	
   int   CRC32_Decoder(Channel_Data_type *Ch_Data, int n_k_rx);
   int   Decoder_3212(unsigned int *R, unsigned int X );
   
   static unsigned int *ui_ptr;
   static unsigned int *C3212_Data_Rx;
   static int  Is_Decoder_Error;

   switch (DecodeCD_State)
   {
   	
   	case 0:
   	   // Zielfeld für RS-Eingangsdaten
   	   C3212_Data_Rx = RS_Decoder_3212Array;
   	   // RS_Decoder_3212Array auf Null setzen nicht unbedingt nötig, da es vollständig überschreiben wird
   	    
   	   	OFDM_Nbit_QAM4_Rx = (3*m_rx)/2 - CRC_LEN - TERM_BITS;   
   		OFDM_Num_32b_Words_Rx = OFDM_Nbit_QAM4_Rx >> 5; 
   		Word_32b_Mask_Rx = 0xffffffff << (32 - ( OFDM_Nbit_QAM4_Rx & 0x1f ));
   		if ( Word_32b_Mask_Rx == 0 ) Word_32b_Mask_Rx = 0xffffffff;
   		else OFDM_Num_32b_Words_Rx++;
		
   		n_c3212_words_rx = n_rx + sizeof(Channel_Data_TX.Settings) + (C3212_N*CHD_32BW_TX)/C3212_K;
   		n_symb_rx = (C3212_N*n_c3212_words_rx)/OFDM_Nbit_QAM4_Rx;
   		if ( (C3212_N*n_c3212_words_rx)%OFDM_Nbit_QAM4_Rx != 0 ) n_symb_rx++;
   		n_symb_step_rx = OFDM_Num_32b_Words_Rx;
		
   		/* Pack received 32bit TCM data to 32bit c(32,12) data*/
   		obit32_mask = 0x80000000;
   		iw = 0;
   		j = 0;
   		for (i=0; i<n_symb_rx*OFDM_Num_32b_Words_Rx; i++) {
   	 		ui = *TCM_Data_Rx++;
     		ibit32_mask = 0x80000000;
     		if ( j == OFDM_Num_32b_Words_Rx ) j = 0;
   	 		while ( ibit32_mask ) {
   	   		if ( j == OFDM_Num_32b_Words_Rx-1 ) {
         		if ( (Word_32b_Mask_Rx & ibit32_mask) == 0) {
           		ibit32_mask = 0;
           		if (obit32_mask == 0 ) {
             		*C3212_Data_Rx++ = iw;
             		iw = 0;
             		obit32_mask = 0x80000000;
           		}
         		} else {	
   	       		if ( (ibit32_mask & ui) != 0 ) iw |= obit32_mask;
   	       		obit32_mask >>= 1;
   	       		if ( obit32_mask == 0 ) {
             		*C3212_Data_Rx++ = iw;
             		iw = 0;
             		obit32_mask = 0x80000000;
   	       		}
         		}
       		} else {
   	     		if ( (ibit32_mask & ui) != 0 ) iw |= obit32_mask;
   	     		obit32_mask >>= 1;
   	     		if ( obit32_mask == 0 ) {
           		*C3212_Data_Rx++ = iw;
           		iw = 0;
           		obit32_mask = 0x80000000;
   	     		}
       		}
 	   		ibit32_mask >>= 1; 	   
     		} 
     		j++;
   		}
	// jetzt stehen in C3212_Data_Rx bzw. RS_Decoder_3212Array die 32 bit RS-Wörter aus denen
	// jetzt durch Dekodierung 32->12 12 bit Inf-Wörter entstehen. 
   
		DecodeCD_State++;
		Decoding_finished = 0;
		*Decoding_Result = 0;
		
   	break;
   	
   	case 1:
   		// Pointer wieder auf Anfang der RS_Decoder Eingangsdaten setzen 	
   	   	C3212_Data_Rx = RS_Decoder_3212Array;
   	   	
   	   	Is_Decoder_Error = 0;
   	   	// Pointer auf Ziel setzen (Channel_Data-Struktur) 
   		ui_ptr = (unsigned int *)Ch_Data_Rx;
   		
   		// Decode & Pack data into output channel data structure
   		// Decode & Pack 32-bit data words (3 Words) 8*12 -> 3*32
   		ui = 0;
   		obit32_mask = 0x80000000;
   		for (i=0; i<(C3212_N*CHD_32BW_RX)/C3212_K; i++) {
     		bit12_mask = 1<<(C3212_K-1);
     		rw = *C3212_Data_Rx++;
     		Decoder3212_status = Decoder_3212(&iw,rw);
     		if (Decoder3212_status != 0 ) Is_Decoder_Error = 1; 
  	 		while ( bit12_mask ) {
   	   		if ( (bit12_mask & iw) != 0 ) ui |= obit32_mask;
   	   		obit32_mask >>= 1;
   	   		if ( obit32_mask == 0 ) { 
   	     		*ui_ptr++ = ui;
   	     		obit32_mask = 0x80000000;
   	     		ui = 0;
   	   		}
   	   		bit12_mask >>= 1;
   	 		}
   		}
   		// jetzt sind erste 8 RS Wörter decodiert in erste 3 32b_words der Kanaldaten

		// Pack unsigned reserved fields (20 * 12bit = 20 32b_words)
   		for (i=0; i<sizeof(Channel_Data_TX.Settings); i++) {
   			Decoder3212_status = Decoder_3212(ui_ptr++,*C3212_Data_Rx++);
    		if (Decoder3212_status != 0 ) Is_Decoder_Error = 1; 
   		}
   		// jetzt sind 20 RS Wörter decodiert in 20*12 bit der Settings
   		
   		DecodeCD_State++;
		Decoding_finished = 0;
		*Decoding_Result = 0;
		
   	break;
   	
   	case 2:
   		// ! Die Pointer ui_ptr und C3212_Data_Rx zeigen bereits auf richtige Stelle
   		// 	aus vorherigem Durchgang
   		
   		// Pack signed channel data fields (64, 128,256 oder 512)
   		for (i=0; i<n_rx; i++) {
   			Decoder3212_status = Decoder_3212(ui_ptr,*C3212_Data_Rx++);
		//   	if ( (*ui_ptr & (1<<(C3212_K-1))) != 0 ) *ui_ptr |= ~C3212_MASK; 
   			ui_ptr++;
    		if (Decoder3212_status != 0 ) Is_Decoder_Error = 1;  
   		}
      	 
 		// Calculate CRC
   		Is_CRC_Error = CRC32_Decoder(Ch_Data_Rx,n_rx);
   		if ( Is_CRC_Error == 0 ) Is_Decoder_Error = 0; 
		   
   		
   		DecodeCD_State = 0; // Reset
   		Decoding_finished = 1;
   		
   		*Decoding_Result = Is_CRC_Error | Is_Decoder_Error;
   		
   	break;
   	
   }
   
   return Decoding_finished;
   
   
  
}


void Init_Decode_Channel_Data(void) {
	// Reset State Machine
	DecodeCD_State = 0;
	
}











