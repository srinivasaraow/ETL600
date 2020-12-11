#include "../h/board.h"
#if BOARD==1
#define DRS7
#else
#undef DRS7
#endif
/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: QAM_scheduler_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : QAM Scheduler RX
 *
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/QAM_scheduler_RX.c $
 * 
 * 17    24.07.08 15:34 Chhamaa
 * 
 * 14    6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 13    21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 11    17.03.06 14:41 Chhamaa
 * 
 * 9     16.09.05 9:41 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 7     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 6     15.02.05 10:04 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 5     1.02.05 13:18 Chalgyg
 * Version 0.09A
 * 
 * 10    22.12.04 9:30 Chchlee
 * 
 * 9     2.12.04 17:54 Chhamaa 
 * 
 * 7     1.10.04 12:41 Leeb02
 *
 * REVISION HISTORY:
 * 		2.4.2004 PL new structure
 *            27.01.05 V11 variable length packing based on 
 *                     User_inf_bits_TX to minimize V11 overall delay, vgp
 *-----------------------------------------------------------------------------
 * 
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <trans.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/tdm/ofdm_tdm_externs.h"
#include "../h/v11/v11_externs.h"

extern int giEmergency_restart;

//========================================
void segment ( "seg_ext_pmco" ) QAM_scheduler_RX()
	{

	void Modulation_Init_QAM_M(int TX_RX);
	void Start_V11_Data_IO();
	int  Init_V11_Data_IO(int ,int);
	void OFDM_Restart();
	void V11_Init();

	int QAM4_bit_errors, V11_Baudcode;
	int i;
	
	if (Start_RX_QAM_Engine == 1 )
		{
			
			switch ( RX_QAM_State )
    			{      
    	   		case RX_QAM_PREFIX:
    	   				RX_OFDM_Engine_state = RX_Engine_QAM4;
			       		if (rx_state_finished)
    	   					{
    	   					// Prefix detected
	       					// go to next QAM-Processing State
	       					switch (Optim_NL_State)
       						  {
       							case Optim_NL_ON_Determination:
       								RX_QAM_State = RX_QAM_CH_DATA_NL;
       							break;
       							
       							case Optim_NL_ON_Selected:
       							case Optim_NL_OFF:
	       			 				RX_QAM_State = RX_QAM_CH_DATA;
	       						break;
       						  }	       			 		
	       					rx_state_finished = 0;
	       					Sync_State = Prefix_detected;
	  						rx_ch_data_counter = 0;
	
	       					//wait_timer = 0;
		   					}
       					else
       						{
       						// Prefix not detected
       						if (rx_prefix_counter > MAX_RX_PREFIX_CYCLES )
       							{//exit with ERROR_CODE ERROR_PREFIX
       							Error.Count++;
       							Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       							Error.Number[Error.Count].ID       = ERROR_PREFIX;
       							dbg_error[Error.Count]=ERROR_PREFIX;
       							giEmergency_restart = 1;
       							}
       						}
				break;
				
				case RX_QAM_CH_DATA_NL:
				case RX_QAM_CH_DATA:
						RX_OFDM_Engine_state = RX_Engine_QAM4;
			       		if (rx_state_finished)
	       					{
	       					Sync_State = ChannelData_received;
	  						// go to next QAM-Processing State
							RX_QAM_State = RX_QAM_CH_DATA_DECODE;
							// RX Engine deaktivieren, um Rechenzeit zu sparen
       						Pause_RX_QAM_Engine = ON;

	       			 		rx_state_finished = 0;
	       					//wait_timer = 0;
	       					}
       					else
    	   					{
	       					//timer
	       					}
				break;
				
				case RX_TX_NL_SELECTION:
					RX_OFDM_Engine_state = RX_Engine_QAM4;
					if (rx_state_finished)
       						{
	       					// go to next QAM-Processing State
	       			 		RX_QAM_State = RX_QAM_SUFFIX;
	       			 		rx_state_finished = 0;
	       			 		Sync_State = Optim_3_finished;
	       			 		// Start Suffix Transmission in local Transmitter
	       			 		Start_Suffix_Transmission = 1;
	       			 		// Reset Suffix Wait counter
	       			 		rx_suffix_counter = 0; 
    	   					}
				break;
				
				case RX_QAM_CH_DATA_DECODE:
					RX_OFDM_Engine_state = RX_Engine_QAM4;
					// RX Engine deaktivieren
			       		if (rx_state_finished)
	       					{
	       					if (Decoder_3212_Status==1)
       							{
       							// Error in QAM4 channel data transmission
       							Error.Count++;
       							Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       							Error.Number[Error.Count].ID       = ERROR_CH_DATA;
       							dbg_error[Error.Count]=ERROR_CH_DATA;
       							//debug: how many bit errors occured?  !!!!!!!!!! to delete
       							if(OFDM_Loop_Mode == DIGITAL_LOOP){
       								vxor_i_EEE((int *)BER_Buffer, (int *)&Channel_Data_TX,(int *)&Channel_Data_RX, sizeof(Channel_Data_TX));
				            		QAM4_bit_errors = vones_i_E((int *)BER_Buffer, sizeof(Channel_Data_TX) );
       								}
       							}
							// RX-Engine wieder aktivieren
							Pause_RX_QAM_Engine = OFF;

	  						// go to next QAM-Processing State
	  						switch (Optim_NL_State)
       						  {
       							case Optim_NL_ON_Determination:
       								RX_OFDM_Engine_state = RX_Engine_QAM4;
       								RX_QAM_State = RX_TX_NL_SELECTION;
       							break;
       							
       							case Optim_NL_ON_Selected:
       							case Optim_NL_OFF:
	       			 				RX_QAM_State = RX_TX_OPTIMIZATION_1;
	       			 				Optim1_RX = 0;
	       						break;
       						  }
	       			 		rx_state_finished = 0;
	       					//wait_timer = 0;
	       					}
       					else
    	   					{
	       					//timer
	       					}


				break;

				case RX_TX_OPTIMIZATION_1:
						RX_OFDM_Engine_state = RX_Engine_QAM4;
						
			       		if (rx_state_finished)
       						{
							// Fehler in Fehlerprotokoll eintragen
							if ( (Optim1_error_TX != 0) ||
       						 (Optim1_error_RX != 0) ||
       						 (V_11_clock_error != 0))
       						 {
       						 Error.Count++;
       						 Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       						 Error.Number[Error.Count].ID       = ERROR_OPTIM1;
       						 dbg_error[Error.Count]=ERROR_OPTIM1;
       						 }

	       					// go to next QAM-Processing State
	       			 		RX_QAM_State = RX_TX_OPTIMIZATION_2;
	       			 		Optim2_RX = 0;
	       			 		rx_state_finished = 0;
	       					Sync_State = Optim_1_finished;

	       			 		
	       			 		// skip next optimization parts
	       			 		/*RX_QAM_State = RX_QAM_SUFFIX;
	       			 		rx_state_finished = 0;
	       			 		// Start Suffix Transmission in local Transmitter
	       			 		Start_Suffix_Transmission = 1;
	       			 		// Reset Suffix Wait counter
	       			 		rx_suffix_counter = 0;
	       			 		 */
	       			 		}
       					
				break;
				
      			case RX_TX_OPTIMIZATION_2:
						RX_OFDM_Engine_state = RX_Engine_QAM4;
						if ( (Optim2_error_TX != 0) ||
       						 (Optim2_error_RX != 0) )       						 
       						 {
       						 Error.Count++;
       						 Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       						 Error.Number[Error.Count].ID       = ERROR_OPTIM2;
       						 dbg_error[Error.Count]=ERROR_OPTIM2;
       						 }

			       		if (rx_state_finished)
       						{
	       					// go to next QAM-Processing State
	       			 		RX_QAM_State = RX_TX_OPTIMIZATION_3;
	       			 		Optim3_RX = 0;
	       			 		rx_state_finished = 0;
	       					Sync_State = Optim_2_finished;
							}
       					
				break;
				
				case RX_TX_OPTIMIZATION_3:
						RX_OFDM_Engine_state = RX_Engine_QAM4;
						if ( (Optim3_error_TX < 0) ||
       						 (Optim3_error_RX < 0) )       						 
       						 {
       						 Error.Count++;
       						 Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       						 Error.Number[Error.Count].ID       = ERROR_OPTIM3;
       						 dbg_error[Error.Count]=ERROR_OPTIM3;
       						 giEmergency_restart = 1;
       						 }

			       		if (rx_state_finished)
       						{
	       					// go to next QAM-Processing State
	       			 		RX_QAM_State = RX_QAM_SUFFIX;
	       			 		rx_state_finished = 0;
	       			 		Sync_State = Optim_3_finished;
	       			 		// Start Suffix Transmission in local Transmitter
	       			 		Start_Suffix_Transmission = 1;
	       			 		// Reset Suffix Wait counter
	       			 		rx_suffix_counter = 0; 
    	   					}
       					
				break;
				
				case RX_QAM_SUFFIX:
						RX_OFDM_Engine_state = RX_Engine_QAM4;
			       		if (rx_state_finished)
       						{
       						// Suffix detected
       						Sync_State = Suffix_detected;
       						Suffix_is_detected = 1;
       						rx_state_finished = 0;
       						// go to next State
       						switch (Optim_NL_State)
       						  {
       							case Optim_NL_ON_Determination:
       								RX_OFDM_Engine_state = RX_Engine_QAM4;
       								RX_QAM_State = RX_WAIT_FOR_RESTART;
       							
       							break;
       							
       							case Optim_NL_ON_Selected:
       							case Optim_NL_OFF:
       								RX_QAM_State = RX_M_QAM_TRAINING;
       			 					RX_OFDM_Engine_state = RX_Engine_QAM_M_Training;
       								
       								// Reset counter for M-QAM Training symbols
       								n_blk_rx_M_QAM_train = 1;
			       					
									// Restore initial eqlzr from LFM-Estimation
		       						vcopy_DE((float *)&Eqlzr[0],(float *)&Eqlzr_backup[0],2*N);
			
    	   							// Prepare equalizer
   	   								vmul_ccf_DDD(Eqlzr,Eqlzr,C_RX,N);

       								// Prepare RX-TCM-Decoder for M-QAM      
	       							Modulation_Init_QAM_M(0);  // Argument "0" means Decoder Initialization
	       							Alpha_Pil=1.0;   // Reset Pre-Equalizer
									// For SNR worser than 35dB, set Pre-Equalizer Coefficient to usual value 0.2
									// (For small bin size Fbin<32Hz it was set to 0.5f in Modem_init_basic)
									if (SNR_mean_dB < 35.0f) Alpha_Pil_Destination = 0.2f;
			       					if (FN == 2000) Alpha_Pil_Destination = 0.4f; 
	    		   					Skp_cnt=0; //for debug purpose only
	       							//rx_suffix_counter = 0;
      								break;
       						  }
       						
	       					}
       					else
    	   					{
    	   					// Suffix not detected
       						if (rx_suffix_counter > MAX_RX_SUFFIX_CYCLES )
       							{// exit with ERROR_CODE ERROR_SUFFIX
       							Error.Count++;
       							Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       							Error.Number[Error.Count].ID       = ERROR_SUFFIX;
       							dbg_error[Error.Count]=ERROR_SUFFIX;
       							giEmergency_restart = 1;
       							}
       						}
				break;
				
				case RX_WAIT_FOR_RESTART:
					if ((Suffix_is_detected)&&(Suffix_is_transmitted))
						{
						if ((tx_wait_for_restart_ctr>4)
							&&(rx_wait_for_restart_ctr++>4))
						{		
						// Prepare Restart in new state	
						Optim_NL_State = Optim_NL_ON_Selected;
       					N=N_selected;
       					L=L_selected;
       					//N=128;
       					//L=16;
       					// Perform Restart with new N and L
       					OFDM_Restart();
						}
						}
				
				break;
				
				case RX_M_QAM_TRAINING:
						RX_OFDM_Engine_state = RX_Engine_QAM_M_Training;
						Sync_State = Training_running;
       						
						// Start RX-Filter Adaptation, if a sufficient number
						//		 of M-QAM-Training symbols has been received 
						if(n_blk_rx_M_QAM_train > 30)
							{
							Filter_RX_Adaptation = ON;
							}
								
			       		//if (++n_blk_rx_M_QAM_train > N_SYMBOLS_M_QAM_TRAINING)
						if (++n_blk_rx_M_QAM_train > M_QAM_Training_Symbols)							
       						{
	       						       					
	       					// go to next QAM-Processing State
	       			 		RX_QAM_State = RX_M_QAM_DATA;
	       			 		RX_OFDM_Engine_state = RX_Engine_QAM_M_Data;
	       			 		Sync_State = Training_finished;

	       					// Reset CRC_Errors_Sum for BLER Estimation
	       			 		CRC_Errors_Sum = 0;  
	       					Skp_cnt=0; //for debug purpose only, MSE log file
    	   					}
    	   					
				break;
				
				case RX_M_QAM_DATA:
						RX_OFDM_Engine_state = RX_Engine_QAM_M_Data;
					   if (n_blk_rx_data>5) {	
						if (V11_IsStarted == 0) {
							V11_Init();
							
#ifdef DRS7
							Init_V11_Data_IO(V11_SLEN,V11_Clock_Div_TX);
#else
							Init_V11_Data_IO(V11_Packing,0);
#endif
							V11_Start_IO = 1;

						
			    
						} //if (V11_IsStarted == 0) {
						} //if (n_blk_rx_data>30) {
					
				if (n_blk_rx_data > 500)
					{
					// clear sync Alarm	
					OFDM_Alarm.syncAlm    = 0 & 0x1;	
					syncAlm_restartcounter = 0;
					asm("nop;");
					asm("nop;");
					asm("nop;");
					}

				/*if(n_blk_rx_data > 1000)
							{
							Filter_RX_Adaptation = OFF;
							}
							*/
				break;
				
				
				default:      	  

      			//OFDM_Restart();

     		 	break;
              	}
		}
	}


