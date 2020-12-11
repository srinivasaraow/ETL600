/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Engine_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : TX Engine
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Engine_TX.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 2     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * Internal PCM generator & scrambling switches
 *       26.01.2004 vgp 
 * Dynamic temporary memory allocation for TCM encoding according to M_TX
 *       04.03.04 vgp
 * Redesign: New structure according to new state machine
 *       26.04.04 PL
 * V11 integration
 *       11.05.04 vgp
 * Random Delta bit vector generation for line operating
 *       12.05.04 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/v11/v11_externs.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Engine_TX()
	{
    int i;

    void  Trellis_Encoder(int *, complex_float *, int, int *);
    void Copy_from_V11_Buffer(int *In_ptr, int len);
    
    void  Scrambler(int *, int *);
    void  TCM_Gen_PCM_Data(int *, int, int );
    void  TCM_Gen_Dummy_Data(int *, int, int );
	void  Spectrum_QAM(complex_float *, complex_float *, int *, int, float);
    void  ifft(int, float *, float pm *, complex_float *);
	//void  TCM_Gen_Delta_Bits(int *, int);

	if (Start_TX_QAM_Engine == ON) 
		{ 
		
		switch ( TX_OFDM_Engine_State )
	   		{  
   			
	   		case TX_Engine_QAM4_DUMMY:
	   		
	   				// Generate arbitrary bitstream  !! Check Random generator !!
   					
   					TCM_Gen_Dummy_Data(In_PCM_Ptr, OFDM_Num_32b_Words_TX,  PCM_32b_Mask_TX);
    			
    				Bit_Buffer_TX_Ptr = (int *)&Cmplx_Array[N]+2*M_TX;
					
    				Unpack_32Bit_To_1BitWord_DD((int *)(2*(unsigned int)&Bit_Buffer_TX_Ptr[0]+CRC_LEN), 
   		                            In_PCM_Ptr, OFDM_Num_32b_Words_TX);
   		                            
   		            /*                
   		            if(n_blk_tx_QAM4==20)
   		            	{
   		            		asm("#include <def21161.h>");
			    	asm("dm(CP1A)=m5;");
    				asm("dm(CP3A)=m5;");
	    			asm("dm(SPCTL0) = m5;");
    				asm("dm(SPCTL2) = m5;");
		        	asm("nop;");
                    asm("nop;");
                    asm("nop;");
   		            	}
					*/
   		                            
   		            n_blk_tx_QAM4 ++;
   		            
   		           
   			break;
        
   			case TX_Engine_QAM4_DATA:
   			
   					// Bit stream is generated in "QAM_processing_TX.c"
   					// This bitstream contains rather "Prefix" or "Channel_Data"
      				Bit_Buffer_TX_Ptr = (int *)&Cmplx_Array[N]+2*M_TX;
					
    				Unpack_32Bit_To_1BitWord_DD((int *)(2*(unsigned int)&Bit_Buffer_TX_Ptr[0]+CRC_LEN), 
   		                            In_PCM_Ptr, OFDM_Num_32b_Words_TX);
   		            
   		            n_blk_tx_QAM4 ++;
       		break;

       		case TX_Engine_QAM_M_Training:
       				// Bit stream is generated in "QAM_processing_TX.c"
   					// This bitstream contains known random M-QAM-Training Data
       				Bit_Buffer_TX_Ptr = (int *)&Cmplx_Array[N]+2*M_TX;
		
					Unpack_32Bit_To_1BitWord_DD((int *)(2*(unsigned int)&Bit_Buffer_TX_Ptr[0]+CRC_LEN), 
	   		                            In_PCM_Ptr, OFDM_Num_32b_Words_TX);
       		
	
	   		        asm("nop;");                    
	   		        asm("nop;");                    
	   		        asm("nop;");
			break;

			case TX_Engine_QAM_M_Data:

			        // Data unpacking
	        		// ==============TCM Encoder SRAM Memory Requirements ===================
					// 1. Bit_Buffer_TX_Ptr must points array in INTERNAL SRAM BLOCK 1 type DM
					// of length BIT_BUFFER_MEMORY_LENGTH_TX/2 = 6908/2 = 3454 32 bit words 
					// for M_TX = 512
	 				// 2. TCM_Encoder_Tmp_Vec must point internal DM SRAM BLOCK 1 vector 
					// length 2*M_TX = 1024 for M_TX = 512
				
			   		Bit_Buffer_TX_Ptr = (int *)&Cmplx_Array[N]+2*M_TX;
		
					Unpack_32Bit_To_1BitWord_DD((int *)(2*(unsigned int)&Bit_Buffer_TX_Ptr[0]+CRC_LEN), 
	   		                            In_PCM_Ptr, OFDM_Num_32b_Words_TX);
	            
	 			                    
	   		        n_blk_tx_data ++;
       	
			break;
			
			}

		//-----------------------------------------------------------	
		//============== Common Transmitter Engine Parts ===============
				
		// Trellis Coder
		Trellis_Encoder(Bit_Buffer_TX_Ptr, Cmplx_Array, M_TX, (int *)&Cmplx_Array[N]);
			
	    // Build OFDM symbol in frequency domain
	
	    if (TX_OFDM_Engine_State == TX_Engine_QAM_M_Training) 
	    {			asm("nop;");                    
	   		        asm("nop;");                    
	   		        asm("nop;");
	    }
	    
	   	// Clear Cmplx_Array[N] 
	  	vset_f_D((float *)&Cmplx_Array[N],0.0f,N2);
	
	   	// Building of spectrum of OFDM signal
	   	// Place QAM modulated carriers in spectrum
		vcopy_DE((float *)&Fl_Array_R[0],(float *)&QAM_Index_TX[0],M_TX);
	   	Spectrum_QAM( &Cmplx_Array[N], Cmplx_Array, (int *)&Fl_Array_R[0], M_TX, Scale_QAM_TX);
	   		
	   	// Place pilot tones in spectrum
	   	Cmplx_Array[N+Pilot_ind0].re = Amp_Pilot_TX;
	   	Cmplx_Array[N+Pilot_ind1].re = Amp_Pilot_TX;      	
	
		// Predistortion 
   		if(Predistortion_Active == ON)
   			{
			vcopy_DE(Fl_Array_R,C_TX,N);
   			vmul_ccf_DDD(Cmplx_Array,&Cmplx_Array[N],Fl_Array_R,N);
   			}
   		else
			vcopy_c_DD(Cmplx_Array,&Cmplx_Array[N],N);	
		 
   		// Get OFDM symbol in time domain 
       	ifft(N, Fl_Array_R, Fl_Array_I, Cmplx_Array); 
			
       	// Cyclic extension: output in &Cmplx_Array[N2+N-L]
	    //vcombine_c_DDP(&Cmplx_Array[N2+N_L],&Fl_Array_R[N_L],&Fl_Array_I[N_L],L);      
	    vcombine_c_DDP(&Cmplx_Array[N2+N_L],&Fl_Array_R[N_L],&Fl_Array_I[N_L],L_PROTO);      
	    vcombine_c_DDP(&Cmplx_Array[N2+N],Fl_Array_R,Fl_Array_I,N);
	        


	}  // .. if (Start_TX_QAM_Engine) ..
	
}

