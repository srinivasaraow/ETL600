/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: QAM_processing_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : QAM Processing TX
 *
 * REVISION HISTORY:
 * 		5.4.2004 PL new structure
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source AES/P4LT/OFDM_DSP/C/QAM_processing_TX.c $
 * 
 * 8     6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 7     21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 4     7.09.05 11:22 Ats
 * Exchange from TLC
 * 
 * 3     7.02.05 14:42 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <trans.h>
#include "../h/TCM/tcm_defs.h"
#include "../h/rs_fec/rs6355_defs.h"
#include "../h/rs_fec/rs_externs.h"
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/rs_fec/RS_externs.h"
#include "../h/v11/v11_externs.h"
#include "../h/QAM4_FEC/vcode3212.h"


int RS_Encoder(int *TCM_Out, int *PCM_In, int *tmp);
void OFDM_Optim_NL_optim();
void Set_Service_Bit(int *OutBit, int position, int max_pos, int value);

int * stf_ptr_tx;
int * v11_ptr_tx;

int RS_status_tmp;
int Buffer_offset_tx;

extern int giEmergency_restart;


//========================================
void segment ( "seg_ext_pmco" ) QAM_processing_TX()
	{
            int RS_Buffer_Shift;
		
			//void  OFDM_Restart();
			void OFDM_QAM4_Data_Preparation();
			 int Encode_Channel_Data(unsigned int *TCM_Data_Tx, 
                                         int          *step_tx,
                                         Channel_Data_type *Ch_Data_Tx, 
                                         int n_tx, 
                                         int m_tx,
                                         unsigned int *tmp);
			void TCM_Gen_MQAM_Train_Data_TX(unsigned int *OutPCM32, int len, int Mask);
  			void  Scrambler(int *, int *);
  			void  TCM_Gen_PCM_Data(int *, int, int );
		    void  TCM_Gen_Delta_Bits(int *, int);
		    int  Get_Stuffing_Flag_TX();
    		void Copy_from_V11_Buffer(int *In_ptr, int len);
            void Adjust_V11_RX_Buffer();                						
			 
	if (Start_TX_QAM_Engine == 1 )
		{
			
			switch ( TX_QAM_State )
    			{ 
    			case TX_QAM_DUMMY1:
    				// State: send QAM4 arbitrary data
    				// here is nothing to do, because
    				// arbitrary bit stream is generated in 
    				// "OFDM_engine_TX" in State "TX_Engine_QAM4_DUMMY"
    				
    				// case is automatically left, if Receiver sets Flag
    				// "Start_CH_DATA_Transmission = 1" in "Preamble_scheduler_RX.c"
    				// but at least "N_SYMBOLS_QAM4_DUMMY" symbols are transmitted
    				// therefore this counter
    				txq_state_counter ++;
    				//  
    			break;
    			
    			case TX_QAM_NL_OPTIM:
    				// take estimation of channel Kabs2_RX[] and estimate required L
    				OFDM_Optim_NL_optim();
    				tx_state_finished=1;
    			break;
    			
    			case TX_QAM_CH_DATA_NL_PREP:
    				// preparation of Channel_Data_NL_RX
    				// to transfer result of NL-Optimization only
    				Channel_Data_NL_RX.N = N_selected_RX;
    				Channel_Data_NL_RX.L = L_selected_RX;
    				// reserved for future
    				Channel_Data_NL_RX.Settings[0]=1.0f;
    				Channel_Data_NL_RX.Settings[1]=2.0f;
    				Channel_Data_NL_RX.Settings[2]=3.0f;
    				Channel_Data_NL_RX.Settings[3]=4.0f;
    				
    				// Encode 32bit words. Input 6 words, output 6*8/3=16 words 
       				vencode3212_3x32bw((unsigned int *)&Word_Buf_CH_Data_TX, (unsigned int *)&Channel_Data_NL_RX, MAX_CHD_NL_NTX);
					
       				// 16x 32bits fit into one OFDM Symbol (for N=512,M=384,QAM4)
       				N_Symb_Tx = 1;	// one OFDM symbol to transmit
       				Step_Tx = 0; 	// no step-size, as there is only one symbol to transmit 
    				
       				// scramble
    				vxor_i_EEE(Word_Buf_CH_Data_TX, Word_Buf_CH_Data_TX,Word_Buf_CH_Scramble, N_Symb_Tx*OFDM_Num_32b_Words_TX);
				   
    				// initialize counter for channel data transmission
    				//    counts number of OFDM symbols, that are required to
    				//		transmit the content of struct "Channel_Data_RX"
    				tx_ch_data_counter = 1;
    				
    				tx_state_finished = 1;   			
    				// arbitrary bit stream is generated in 
    				// "OFDM_engine_TX" in State "TX_Engine_QAM4_DUMMY"
    			
    			break;
    			
    			case TX_QAM_CH_DATA_PREP:
    				// call preparation of channel data RX
    				// the data of the estimated channel will be transmitted to the other side
    				// Note: this data is called Channel_Data_RX
    				// packing and encoding
    				// ! exit state with "tx_state_finished = 1;"
    				
    				OFDM_QAM4_Data_Preparation();
    				
    				// call packing and coding
    				N_Symb_Tx = Encode_Channel_Data((unsigned int *)Word_Buf_CH_Data_TX, &Step_Tx, &Channel_Data_RX, N, M_TX, (unsigned int *)Fl_Array_R);
       				
    				// scramble
    				vxor_i_EEE(Word_Buf_CH_Data_TX, Word_Buf_CH_Data_TX,Word_Buf_CH_Scramble, N_Symb_Tx*OFDM_Num_32b_Words_TX);
				   
    				// initialize counter for channel data transmission
    				//    counts number of OFDM symbols, that are required to
    				//		transmit the content of struct "Channel_Data_RX"
    				tx_ch_data_counter = 1;
    				
    				tx_state_finished = 1;   			
    				// arbitrary bit stream is generated in 
    				// "OFDM_engine_TX" in State "TX_Engine_QAM4_DUMMY"
    			break;
    			
    	   		
    			case TX_QAM_PREFIX:
    	   			// generate QAM4 Prefix Bitstream
			       	// ! exit state with "tx_state_finished = 1;"
			       	vcopy_EE((float *)In_PCM_Ptr,(float *)Word_Buf_Prefix,OFDM_Num_32b_Words_TX);
			       	*(In_PCM_Ptr+OFDM_Num_32b_Words_TX-1) &= PCM_32b_Mask_TX;
			       	tx_state_finished = 1;
    			break;
				
				
				case TX_QAM_CH_DATA:
			       	// extract bit stream out of channel data bit field to transmit one OFDM symbol
			    	// ! exit state with "tx_state_finished = 1;" if all channel data is sent
					In_PCM_Ptr = &Word_Buf_CH_Data_TX[(tx_ch_data_counter-1)*Step_Tx];
					
			    break;
				
				
				case TX_QAM4_DUMMY2:
					// send QAM4 dummy data  
					// case is automatically left, if Receiver sets Flag
    				// "Start_Suffix_Transmission = 1" in "QAM_scheduler_RX.c"
    		  		
				break;
      	
				case TX_QAM_DELAY_SUFFIX:
					// nothing to do here, just wait
				break;
				
				case TX_QAM_SUFFIX:
			       	// send QAM4 Suffix	
					// ! exit state with "tx_state_finished = 1;"
					
					// generate QAM4 Suffix Bitstream
			       	// ! exit state with "tx_state_finished = 1;"
			       	vcopy_EE((float *)In_PCM_Ptr,(float *)Word_Buf_Suffix,OFDM_Num_32b_Words_TX);
			       	*(In_PCM_Ptr+OFDM_Num_32b_Words_TX-1) &= PCM_32b_Mask_TX;
			       	tx_state_finished = 1;
    			break;
				
				
				case TX_M_QAM_TRAINING:
					// send M_QAM Training Data
					TCM_Gen_MQAM_Train_Data_TX((unsigned int *)In_PCM_Ptr, OFDM_Num_32b_Words_TX,  PCM_32b_Mask_TX);
    	
				break;
				
				
				case TX_M_QAM_DATA:
			    	// send M-QAM User Data	
			    	// insert service bits
			    	// insert stuffing bits
			    	
			    	// Generate information bits internaly
					if ( PCM_Generator == ON ) {
						  TCM_Gen_PCM_Data(In_PCM_Ptr, OFDM_Num_32b_Words_TX,  PCM_32b_Mask_TX);
						  if ( RS_Coding_Flag ) { 
						     Bit_Buffer_TX_Ptr = (int *)(2*(unsigned int)&Cmplx_Array[0]);		
    				         Unpack_32Bit_To_1BitWord_DD(Bit_Buffer_TX_Ptr+LOG_GF64-CRC_LEN, 
								                         In_PCM_Ptr, OFDM_Num_32b_Words_TX);
        		             RS_status_tmp = RS_Encoder (Bit_Buffer_TX_Ptr+LOG_GF64-CRC_LEN, 
								                         Bit_Buffer_TX_Ptr+LOG_GF64-CRC_LEN,
														 (int *)Fl_Array_R);
							 Bit_Buffer_TX_Ptr[0] = Bit_Buffer_TX_Ptr[RS_TCM_Nbit_Tx];
        		             Pack_1Bit_To_32BitWord_DD(In_PCM_Ptr, Bit_Buffer_TX_Ptr,
        		                          32*OFDM_Num_32b_Words_TX);
   		                            
						  }
						}
					else
						{
							
						if (RS_Coding_Flag)	
						{
						Buffer_offset_tx = LOG_GF64-CRC_LEN; // adjust buffer to LOG_GF64-CRC_LEN=1 value
						Bit_Buffer_TX_Ptr = (int *)(2*(unsigned int)&Cmplx_Array[0]);		
						// Insert RS Dummy_Bits into Bit_Buffer
						if ( RS_dummy_bit_Tx > 0 )
							{
							TCM_Gen_Delta_Bits(Bit_Buffer_TX_Ptr+Buffer_offset_tx,RS_dummy_bit_Tx);
                        	Buffer_offset_tx += RS_dummy_bit_Tx;
							}
						}
						else
						{
						Buffer_offset_tx=0;
						Bit_Buffer_TX_Ptr = (int *)(2*(unsigned int)Bit_Buffer_TX_Ptr);		
						// -- V11 Interface delivers information bits	
						// Insert Delta_Bits into Bit_Buffer
						if ( Delta_bits_tx > 0 )
							{
							TCM_Gen_Delta_Bits(Bit_Buffer_TX_Ptr,Delta_bits_tx);
                        	Buffer_offset_tx += Delta_bits_tx;
							}
						}
						
                        // Append Service Bits into Bit_Buffer
						if (Number_of_Service_bits > 0)
							{
							TCM_Gen_Delta_Bits(Bit_Buffer_TX_Ptr+Buffer_offset_tx,Number_of_Service_bits);
                        	// prototyp: Set_Service_Bit(unsigned int *OutBit, int position, int max_pos, int value)
                        	Set_Service_Bit(Bit_Buffer_TX_Ptr+Buffer_offset_tx, SERVICE_BIT_POS_FFwd, Number_of_Service_bits, FFwd_Status_local);
                        	Buffer_offset_tx += Number_of_Service_bits;
							}
						
						// Align pointer to v11 bits
                        v11_ptr_tx = Bit_Buffer_TX_Ptr+Buffer_offset_tx;

                        // ==Append User Information Bits
                       if (User_inf_bits_TX > 0)
                       {
                    	if (Stuffing_symb_per_period ==0)
                	    	{ // *** No Stuffing required for this data rate
	                        // Append V11_User Bits to Bit_Buffer
    	                  	if (V11_RX_Buffer_IsAdjusted) 
	                        	Copy_from_V11_Buffer(v11_ptr_tx, User_inf_bits_TX); // TCM_optim_inf_bit_TX
							else  // append random bits
    	                    	TCM_Gen_Delta_Bits(v11_ptr_tx,User_inf_bits_TX);
    	                	
    	                    // for v11 debugging purpose
            	            //vcopy_EE((float *)Bit_Buffer_TX_Ptr, (float *)V11_xxx, OFDM_Num_Bits_TX-Delta_bits_tx); // TCM_optim_inf_bit_TX
                	    	}
                	    else
                	    	{ // *** Data Rate requires Bit Stuffing
                	    	// Get Stuffing Flag
                	    	switch (Get_Stuffing_Flag_TX())
                	    		{	
                	    		case 0:
                	    			// Append V11_User Bits [User_inf_bits_TX-1]
									if (V11_RX_Buffer_IsAdjusted)
									{
									/*	
									asm("nop;");
									asm("nop;");
									asm("nop;");
									*/
									Copy_from_V11_Buffer(v11_ptr_tx, User_inf_bits_TX-1); // TCM_optim_inf_bit_TX
									}
									else  // append random bits
                	    				TCM_Gen_Delta_Bits(v11_ptr_tx,User_inf_bits_TX-1);
                	    			
                	    			// Append Dummy bit and Stuffing Flag Bit
                	    			stf_ptr_tx = v11_ptr_tx + User_inf_bits_TX;
                	    			*(stf_ptr_tx-1)=0;
                	    			*stf_ptr_tx= 0;
                	    		break;
                	    		
                	    		case 1:
                	    			// Append V11_User Bits [User_inf_bits_TX]
									if (V11_RX_Buffer_IsAdjusted)
									{
									/*
									asm("nop;");
									asm("nop;");
									asm("nop;");
									*/
									Copy_from_V11_Buffer(v11_ptr_tx, User_inf_bits_TX); // TCM_optim_inf_bit_TX
									}
									else  // append random bits
	                	    			TCM_Gen_Delta_Bits(v11_ptr_tx,User_inf_bits_TX);
	                	    		
	                	    		// Append Stuffing Flag Bit
									stf_ptr_tx = v11_ptr_tx + User_inf_bits_TX;
									*stf_ptr_tx = 1;	
								break;
                	    		}
                	    	}

				        // Packing and opt. RS Encoder
				       if (RS_Coding_Flag)	
						{
//						Bit_Buffer_TX_Ptr = (int *)(2*(unsigned int)&Cmplx_Array[0]);		
        		        RS_status_tmp = RS_Encoder (Bit_Buffer_TX_Ptr+LOG_GF64-CRC_LEN,Bit_Buffer_TX_Ptr+LOG_GF64-CRC_LEN,(int *)Fl_Array_R);
                        Bit_Buffer_TX_Ptr[0] = Bit_Buffer_TX_Ptr[RS_TCM_Nbit_Tx]; 
        		        Pack_1Bit_To_32BitWord_DD(In_PCM_Ptr,Bit_Buffer_TX_Ptr,
        		                          32*OFDM_Num_32b_Words_TX);
						
						}
						else
						{
        		        Pack_1Bit_To_32BitWord_DD(In_PCM_Ptr,Bit_Buffer_TX_Ptr,
        		                          32*OFDM_Num_32b_Words_TX);
						}
						
                       } // if (User_inf_bits_TX > 0)
                       else giEmergency_restart = 1;
                       
			 		} //if ( PCM_Generator == ON ) {} else {
			
			 					 		
			 		// Data Scrambling
					if ( Scrambling == ON ) {
			   		   Scrambler(In_PCM_Ptr,In_PCM_Ptr); 
					}
	        
	   		
				break;
				
			
				default:      	  

      			//OFDM_Restart();

     		 	break;
              	}
		}
	}


