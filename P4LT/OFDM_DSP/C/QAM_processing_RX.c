/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: QAM_processing_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : QAM Processing RX
 *
 * REVISION HISTORY:
 * 		2.4.2004 PL new structure
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/QAM_processing_RX.c $
 * 
 * 14    25.07.08 13:54 Chhamaa
 * 
 * 7     30.09.05 10:55 Chalgyg
 * OFDM_DSP_V0_11_ur (12709051)
 * 
 * 3     7.02.05 14:41 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 6     22.12.04 9:30 Chchlee
 * 
 * 5     2.12.04 17:54 Chhamaa
 * 
 * 4     28.09.04 17:45 Maag01
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
#include "../h/QAM4_FEC/vcode3212.h"
#include "../h/v11/v11_externs.h"


int * stf_ptr_rx; //Pointer to Stuffing Bits
int * v11_ptr_rx; //Pointer to v11 Bits
int * svc_ptr_rx; //Pointer to Service Bits
int Stuffing_Flag_RX_c;

int RSdec_status_tmp;
int Buffer_offset_rx;


int  RS_Decoder(int *PCM_Out, int *TCM_In, int *tmp);



//========================================
void segment ( "seg_ext_pmco" ) QAM_processing_RX()
	{
		
			void  OFDM_Restart();
			void  OFDM_Optim1();
			void  OFDM_Optim2();
			void  OFDM_Optim3();
			void OFDM_Optim_NL_select();
			//int   Decode_Channel_Data(Channel_Data_type *Ch_Data_Rx, 
            //                             unsigned int *TCM_Data_Rx, 
            //                             int n_rx, 
            //                             int m_rx,
            //                             unsigned int *tmp);
            int Decode_Channel_Data(Channel_Data_type *Ch_Data_Rx, 
                        unsigned int *TCM_Data_Rx, 
                        int n_rx, 
                        int m_rx,
                        int *Decoding_Result);
            void Init_Decode_Channel_Data(void);

             void  Copy_to_V11_Buffer(int *In_ptr, int len);
             void  Descrambler(int *, int *);
             unsigned int TCM_BER_Tester(unsigned int *, int, int, unsigned int *);
             int   Get_Service_Bit(int *InBit, int position, int max_pos);
             int   Get_Stuffing_Flag_RX();
             void  Adjust_V11_TX_Buffer();


			int QAM4_bit_errors;
			
		   int Stuffing_Flag_RX_b;
	static int stuffing_error_ctr;
	
			 
	if (Start_RX_QAM_Engine == 1 )
		{
			
			switch ( RX_QAM_State )
    			{      
    	   		case RX_QAM_PREFIX:
    	   			// call Prefix Detection
    	   			// Input is received bit field
    	   			// ! exit state with "rx_state_finished=1" if Prefix is detected
    	   			
    	   			// Modify unused bits (masked out by ~PCM_32b_Mask_RX) in last 32bit Word of received data
    	   			//             with known last 32bit Word of saved Prefix
					// set unused bits at tail to zero
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) &= PCM_32b_Mask_RX;
					// replace unused bits by tail of Prefix
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) += (*(Word_Buf_Prefix+OFDM_Num_32b_Words_RX-1)&(~PCM_32b_Mask_RX));

    	   			// Compare received Word with saved Prefix bitwise
    	   			vxor_i_EEE((int *)BER_Buffer, Word_Buf_Prefix,Out_PCM_Ptr, OFDM_Num_32b_Words_RX);
				    QAM4_bit_errors = vones_i_E((int *)BER_Buffer, OFDM_Num_32b_Words_RX );
					
				    // Debug: Save minimum value
				    if (QAM4_bit_errors < Prefix_thres ) Prefix_thres = QAM4_bit_errors;

    	   			// check received Words for Prefix
			       	if (QAM4_bit_errors < Prefix_Suffix_TH)  // Prefix_Suffix_TH=5*N/64
			       		{
			       		// Prefix is detected
			       		rx_state_finished=1;
			       		Decoder_3212_Status=0;	
			       		}
			       	else
			       		{
			       		// Prefix not detected, increment wait counter
			       		rx_prefix_counter++;
			       		}
    	   			// debug section
    	   			// if (debug_state_cnt++ == 20) {rx_state_finished=1; debug_state_cnt=0;}
    	   		
    	   			n_blk_rx_QAM4++;	
				break;
				
				case RX_QAM_CH_DATA_NL:
					// save received bit stream into vector until all channel data bits are received (here only once)
			       	vcopy_EE((float *)&Word_Buf_CH_Data_RX[rx_ch_data_counter*Step_Tx],(float *)Out_PCM_Ptr,OFDM_Num_32b_Words_RX);
			       	rx_ch_data_counter++;
			       	
			       	if (rx_ch_data_counter == N_Symb_Tx) // here N_Symb_Tx should be "1"
			       		{
			       		// unscramble
    				    vxor_i_EEE(Word_Buf_CH_Data_RX, Word_Buf_CH_Data_RX,Word_Buf_CH_Scramble, N_Symb_Tx*OFDM_Num_32b_Words_RX);
   				    	// Decode 32bit words. Input 16 words, output 16/8*3=6 words 
      					Decoder_3212_Status = vdecode3212_3x32bw((unsigned int *)&Channel_Data_NL_TX, (unsigned int *)Word_Buf_CH_Data_RX, MAX_CHD_NL_ENCNTX);

 						rx_state_finished=1;	
			       		}
			       	n_blk_rx_QAM4++;
				break;
				
				case RX_QAM_CH_DATA:
			       	// save received bit stream into vector until all channel data bits are received
			       	vcopy_EE((float *)&Word_Buf_CH_Data_RX[rx_ch_data_counter*Step_Tx],(float *)Out_PCM_Ptr,OFDM_Num_32b_Words_RX);
			       	
			       	rx_ch_data_counter++;
			       	
			       	if (rx_ch_data_counter == N_Symb_Tx)
			       		{
			       		// now all channel data bits are received
			       		// unscramble
    				    vxor_i_EEE(Word_Buf_CH_Data_RX, Word_Buf_CH_Data_RX,Word_Buf_CH_Scramble, N_Symb_Tx*OFDM_Num_32b_Words_RX);
				   		// Init Decoding State Machine
    				    Init_Decode_Channel_Data();
    				    rx_state_finished=1;
			       		}
			       	n_blk_rx_QAM4++;
				break;
				
				case RX_TX_NL_SELECTION:
					OFDM_Optim_NL_select();
					rx_state_finished=1;
				break;
				
				case RX_QAM_CH_DATA_DECODE:
 				   		// decode data into struct "Channel_Data_TX"
//				   		Decoder_3212_Status = Decode_Channel_Data(&Channel_Data_TX, (unsigned int *)Word_Buf_CH_Data_RX, N, M_RX, (unsigned int *)Fl_Array_R); 
//						rx_state_finished=1;
				   		rx_state_finished = Decode_Channel_Data(&Channel_Data_TX, (unsigned int *)Word_Buf_CH_Data_RX, N, M_RX, &Decoder_3212_Status); 
						
						n_blk_rx_QAM4++;
				break;

				case RX_TX_OPTIMIZATION_1:
					// call Optimization State Machine for Step 1
					OFDM_Optim1();
					n_blk_rx_QAM4++;
				break;
      	
				case RX_TX_OPTIMIZATION_2:
					// Test if Suffix is detected before end of optimization
					// Modify unused bits (masked out by ~PCM_32b_Mask_RX) in last 32bit Word of received data
    	   			//             with known last 32bit Word of saved Suffix
					// set unused bits at tail to zero
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) &= PCM_32b_Mask_RX;
					// replace unused bits by tail of Prefix
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) += (*(Word_Buf_Suffix+OFDM_Num_32b_Words_RX-1)&(~PCM_32b_Mask_RX));
	
    	   			// Compare received Word with saved Suffix bitwise
    	   			vxor_i_EEE((int *)BER_Buffer, Word_Buf_Suffix,Out_PCM_Ptr, OFDM_Num_32b_Words_RX);
				    QAM4_bit_errors = vones_i_E((int *)BER_Buffer, OFDM_Num_32b_Words_RX );
					
    	   			// check received Words for Suffix
			       	if (QAM4_bit_errors < Prefix_Suffix_TH)  
			       		{
			       		// Suffix is detected
			       			Error.Count++;
       						Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       						Error.Number[Error.Count].ID       = ERROR_SUFFIX_DURING_OPTIM;
       						dbg_error[Error.Count]=ERROR_SUFFIX_DURING_OPTIM;
			       		}
					// call Optimization State Machine for Step 2
					OFDM_Optim2();
					n_blk_rx_QAM4++;
				break;
				
				case RX_TX_OPTIMIZATION_3:
					// Test if Suffix is detected before end of optimization
					// Modify unused bits (masked out by ~PCM_32b_Mask_RX) in last 32bit Word of received data
    	   			//             with known last 32bit Word of saved Suffix
					// set unused bits at tail to zero
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) &= PCM_32b_Mask_RX;
					// replace unused bits by tail of Prefix
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) += (*(Word_Buf_Suffix+OFDM_Num_32b_Words_RX-1)&(~PCM_32b_Mask_RX));
	
    	   			// Compare received Word with saved Suffix bitwise
    	   			vxor_i_EEE((int *)BER_Buffer, Word_Buf_Suffix,Out_PCM_Ptr, OFDM_Num_32b_Words_RX);
				    QAM4_bit_errors = vones_i_E((int *)BER_Buffer, OFDM_Num_32b_Words_RX );
					
    	   			// check received Words for Suffix
			       	if (QAM4_bit_errors < Prefix_Suffix_TH)  
			       		{
			       		// Suffix is detected
			       			Error.Count++;
       						Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       						Error.Number[Error.Count].ID       = ERROR_SUFFIX_DURING_OPTIM;
       						dbg_error[Error.Count]=ERROR_SUFFIX_DURING_OPTIM;
			       		}
					// call Optimization State Machine for Step 3
					OFDM_Optim3();
					n_blk_rx_QAM4++;
				break;
				
				case RX_QAM_SUFFIX:
			       	// Suffix Detection
			       	// Input is Pointer to received bit field "Out_PCM_Ptr"
			       	// Output is flag "rx_state_finished"
    	   			// exits state with "rx_state_finished=1" if Prefix is detected
			       	
    	   			// Modify unused bits (masked out by ~PCM_32b_Mask_RX) in last 32bit Word of received data
    	   			//             with known last 32bit Word of saved Suffix
					// set unused bits at tail to zero
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) &= PCM_32b_Mask_RX;
					// replace unused bits by tail of Prefix
					*(Out_PCM_Ptr+OFDM_Num_32b_Words_RX-1) += (*(Word_Buf_Suffix+OFDM_Num_32b_Words_RX-1)&(~PCM_32b_Mask_RX));

    	   			// Compare received Word with saved Suffix bitwise
    	   			vxor_i_EEE((int *)BER_Buffer, Word_Buf_Suffix,Out_PCM_Ptr, OFDM_Num_32b_Words_RX);
				    QAM4_bit_errors = vones_i_E((int *)BER_Buffer, OFDM_Num_32b_Words_RX );
					
    	   			// check received Words for Suffix
			       	if (QAM4_bit_errors < Prefix_Suffix_TH)  // Prefix_Suffix_TH=5*N/64
			       		{
			       		// Suffix is detected
			       		rx_state_finished=1;	
			       		}
			       	else
			       		{
			       		// Prefix not detected, increment wait counter
			       		rx_suffix_counter++;
			       		}
    	   			
    	   			n_blk_rx_QAM4++;
				break;
				
				case RX_WAIT_FOR_RESTART:
					// nothing to do here, just waiting
				break;
				
				case RX_M_QAM_TRAINING:
					// what is done here?, maybe BER calculation 
			       	//n_blk_rx_M_QAM_train++;
				break;
				
				case RX_M_QAM_DATA:
			    // Reception of Data, Push received bits to user interface	
				// extract service bits
				
				Buffer_offset_rx=0;
				
				// Data descrambling
				if ( Scrambling == ON )
				{
		  			Descrambler(Out_PCM_Ptr,Out_PCM_Ptr);
		  			if (RS_Coding_Flag)
		  				{
						Bit_Buffer_RX_Ptr = (int *)(2*(unsigned int)&Cmplx_Array[0]);		
						Unpack_32Bit_To_1BitWord_DD(Bit_Buffer_RX_Ptr, Out_PCM_Ptr, 
						                            OFDM_Num_32b_Words_RX);
						//adjust buffer
                        Bit_Buffer_TX_Ptr[RS_TCM_Nbit_Rx] = Bit_Buffer_TX_Ptr[0]; 

		  				// RS Decoder
		  				RSdec_status_tmp = RS_Decoder(Bit_Buffer_RX_Ptr+LOG_GF64-CRC_LEN, Bit_Buffer_RX_Ptr+LOG_GF64-CRC_LEN, (int *)&Fl_Array_R[0]);
		  				// debug, to remove
		  				//if (n_blk_rx_data<100) MSE_trend_QAM4[n_blk_rx_data]=(float)RSdec_status_tmp;
		  			
		  				Buffer_offset_rx = RS_dummy_bit_Rx+LOG_GF64-CRC_LEN;

		  				// align pointer to Service bits
						svc_ptr_rx = Bit_Buffer_RX_Ptr + Buffer_offset_rx;
						Buffer_offset_rx += Number_of_Service_bits;

						// align pointer to v11 bits
						v11_ptr_rx = Bit_Buffer_RX_Ptr + Buffer_offset_rx;
						}
					else
						{
						Bit_Buffer_RX_Ptr = (int *)(2*(unsigned int)Bit_Buffer_RX_Ptr);		
						Unpack_32Bit_To_1BitWord_DD(Bit_Buffer_RX_Ptr, Out_PCM_Ptr, 
						                            OFDM_Num_32b_Words_RX);

						Buffer_offset_rx = Delta_bits_rx;
		  			
						// align pointer to Service bits
						svc_ptr_rx = Bit_Buffer_RX_Ptr + Buffer_offset_rx;
						Buffer_offset_rx += Number_of_Service_bits;

                    	// align pointer to v11 bits
	   		        	v11_ptr_rx = Bit_Buffer_RX_Ptr + Buffer_offset_rx;
						}
                } else {    		      	
					Bit_Buffer_RX_Ptr = (int *)(2*(unsigned int)Bit_Buffer_RX_Ptr);		
                    
                    Buffer_offset_rx = CRC_LEN+Delta_bits_rx;
                    
                    // align pointer to Service bits
					svc_ptr_rx = Bit_Buffer_RX_Ptr + Buffer_offset_rx;
					Buffer_offset_rx += Number_of_Service_bits;
					
					// align pointer to v11 bits
	   		        v11_ptr_rx = Bit_Buffer_RX_Ptr + Buffer_offset_rx;
              	}
              	
              	// Extract Service Bits
				if (Number_of_Service_bits > 0)
					{
					FFwd_Status_distant = Get_Service_Bit(svc_ptr_rx, SERVICE_BIT_POS_FFwd, Number_of_Service_bits);						
					}
 
              	
              	// Extract V11 User Bits
                if (Stuffing_symb_per_period ==0)
               		{ // No Stuffing required for this data rate
	                  // Extract V11 User Bits
		      		if (V11_TX_Buffer_IsAdjusted) 
                		 Copy_to_V11_Buffer(v11_ptr_rx, User_inf_bits_RX); // TCM_optim_inf_bit_RX
             
					}
                else
                   	{ // *** Data Rate requires Bit Stuffing
                   	// Get Stuffing Flag by Counter
                   	Stuffing_Flag_RX_c = Get_Stuffing_Flag_RX();
                   	
                   	
                   	// **debug section to verify stuffing operation
                   	if ((STUFFING_ERROR_CONTROL) && (CRC_Error == 0) && (Level_Warning==0))
                   		{
                   		// Initialization of Counter	
                   		if (n_blk_rx_data < 2) stuffing_error_ctr=0;
                   		
                   		// Get Stuffing Flag by received Bit field
                   		// align pointer in bit field and extract Flag
                		stf_ptr_rx = v11_ptr_rx + User_inf_bits_RX;
						Stuffing_Flag_RX_b = * stf_ptr_rx;
                   	
                   		// Check for Identity
                   		if (Stuffing_Flag_RX_c == Stuffing_Flag_RX_b)
	                		{
	                		// if no error then decrement stuffing_error_ctr if positiv	
	                	    stuffing_error_ctr-=2;
	                	    if (stuffing_error_ctr<0) stuffing_error_ctr=0; 
			  				}
                		else
                			{
                			// if error then increment stuffing_error_ctr
                			//   this is not necessarily a stuffing_error, maybe only bit error
                			stuffing_error_ctr+=4;
                			if (stuffing_error_ctr > STUFFING_ERROR_THRES)
                				{ // now there are to many stuffing errors, report that to error_handler
                				Error.Count++;
                				Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
                				Error.Number[Error.Count].ID	   = ERROR_STUFFING;
                				}
                			}
                			
                   		}
                	// **end of debug section to verify stuffing operation  		
                	
					switch (Stuffing_Flag_RX_c)
                		{
                		case 0:
                			// Extract V11 User Bits (ignore last bit, length-1)
                	   		if (V11_TX_Buffer_IsAdjusted)
								Copy_to_V11_Buffer(v11_ptr_rx, User_inf_bits_RX-1); // TCM_optim_inf_bit_RX
                	   	break;
                	   
                	   	case 1:
                	   		// Extract V11 User Bits (including last bit, full length)
                			if (V11_TX_Buffer_IsAdjusted)
								Copy_to_V11_Buffer(v11_ptr_rx, User_inf_bits_RX); // TCM_optim_inf_bit_RX
                		break;
                		}
               		}
   			
   				// BER Meter
				if ( BER_Meter == ON )
					{
        			TCM_BER_Tester((unsigned int *)Out_PCM_Ptr, OFDM_Num_32b_Words_RX,
                    		        PCM_32b_Mask_RX, BER_Buffer);
					}
					
				n_blk_rx_data ++;

				break;
				
				case RX_QAM_IDLE:
				// no operation
				break;
				
				
				default:      	  

      			OFDM_Restart();

     		 	break;
              	}
		}
	}


