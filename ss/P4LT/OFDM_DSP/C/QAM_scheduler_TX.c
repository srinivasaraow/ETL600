/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: QAM_scheduler_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : QAM Scheduler TX
 *
 * $Log: /Source/P4LT/OFDM_DSP/C/QAM_scheduler_TX.c $
 * 
 * 2     16.09.05 9:41 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * REVISION HISTORY:
 * 		5.4.2004 PL new structure
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
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/tcm/TCM_Codec_externs.h"

//========================================
void segment ( "seg_ext_pmco" ) QAM_scheduler_TX()
	{
	int status;
	static int TX_Suffix_delay_ctr;
		
	void Modulation_Init_QAM_M(int TX_RX);
	void OFDM_Restart();
	
	if (Start_TX_QAM_Engine == 1 )
		{
			
			switch ( TX_QAM_State )
    			{      
    	   		case TX_QAM_DUMMY1:
    	   				TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
			       		if ( (txq_state_counter > N_SYMBOLS_QAM4_DUMMY) && (Start_CH_DATA_Transmission == 1) )
       					{
       							
       						// "Start_CH_DATA_Transmission" is set by local receiver in "Preamble_scheduler_RX"
       						//                              if receiver detected QAM4 symbols
       						// go to next QAM-Processing State
       			 			switch (Optim_NL_State)
    						{
    						// -----------------------------------------------------
    						case Optim_NL_ON_Determination: //Starttyp I
								TX_QAM_State = TX_QAM_NL_OPTIM;
    						break;
    						// -----------------------------------------------------
    						case Optim_NL_ON_Selected:		//Starttyp II
    						case Optim_NL_OFF:
    							TX_QAM_State = TX_QAM_CH_DATA_PREP;
       							
    						break;
    						}
    						txq_state_counter = 0;
       						tx_state_finished    = 0;
       					}
       					
				break;
				
				case TX_QAM_NL_OPTIM:
    	   				TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
			       		if ( tx_state_finished )
       						{
	       					// go to next QAM-Processing State
	       			 		TX_QAM_State         = TX_QAM_CH_DATA_NL_PREP;
	       			 		TX_OFDM_Engine_State = TX_Engine_QAM4_DATA;
	       					tx_state_finished    = 0;
	       					//wait_timer = 0;
	       					}
       					else
	       					{
	       					//timer
	       					}
				break;
				
				case TX_QAM_CH_DATA_NL_PREP:				
				case TX_QAM_CH_DATA_PREP:
    	   				TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
			       		if ( tx_state_finished )
       						{
	       					// go to next QAM-Processing State
	       			 		TX_QAM_State         = TX_QAM_PREFIX;
	       			 		TX_OFDM_Engine_State = TX_Engine_QAM4_DATA;
	       					tx_state_finished    = 0;
	       					//wait_timer = 0;
	       					}
       					else
	       					{
	       					//timer
	       					}
				break;
				
				case TX_QAM_PREFIX:
    	   				TX_OFDM_Engine_State = TX_Engine_QAM4_DATA;
			       		if ( tx_state_finished )
       						{
       						// go to next QAM-Processing State
       			 			TX_QAM_State = TX_QAM_CH_DATA;
       						tx_state_finished    = 0;
       						tx_ch_data_counter   = 1;
       						//wait_timer = 0;
       						}
       					else
       						{
       						//timer
       						}
				break;
				
				case TX_QAM_CH_DATA:
						TX_OFDM_Engine_State = TX_Engine_QAM4_DATA;
			       		if (++tx_ch_data_counter>N_Symb_Tx)
       						{
       						tx_ch_data_counter--;
       						// go to next QAM-Processing State
       			 			TX_QAM_State         = TX_QAM4_DUMMY2;
       			 			TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
       			 			// re-initialize In_PCM_Ptr
       			 			In_PCM_Ptr = &In_PCM_Buffer[0];
       						}
       					else
       						{
       						// stay in that state		
       						}
       				
				break;
				
				case TX_QAM4_DUMMY2:
						TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
			       		if (Start_Suffix_Transmission == 1)  // set in QAM_scheduler_RX if Optimization is finished
       						{
       						tx_state_finished = 0;
       						// go to next QAM-Processing State
       			 			TX_QAM_State 		 = TX_QAM_DELAY_SUFFIX;
       			 			TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
       			 			Start_Suffix_Transmission = 0;
       			 			TX_Suffix_delay_ctr = 0;
       						}
       					else
       						{
       						//timer
       						}
				break;
				
				case TX_QAM_DELAY_SUFFIX:
						TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
						if (TX_Suffix_delay_ctr++>5)
							{
							TX_Suffix_delay_ctr=0;
							tx_state_finished = 0;
							// go to next QAM-Processing State
							TX_QAM_State 		 = TX_QAM_SUFFIX;
       			 			TX_OFDM_Engine_State = TX_Engine_QAM4_DATA;       						
							}
      	
				case TX_QAM_SUFFIX:
						TX_OFDM_Engine_State = TX_Engine_QAM4_DATA;
			       		if (tx_state_finished)
       						{
							Suffix_is_transmitted = 1;
							tx_state_finished = 0;

       						// go to next QAM-Processing State
       						switch (Optim_NL_State)
       						  {
       							case Optim_NL_ON_Determination:
       								TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
       								TX_QAM_State = TX_WAIT_FOR_RESTART;
       							
       							break;
       							
       							case Optim_NL_ON_Selected:
       							case Optim_NL_OFF:
       			 					TX_QAM_State = TX_M_QAM_TRAINING;
       			 					TX_OFDM_Engine_State = TX_Engine_QAM_M_Training;
       								
       								// Reset counter for M-QAM-Training Symbols
       								n_blk_tx_M_QAM_train = 1;  
       						
       								// Activation of predictortion in TX engine
       								Predistortion_Active = ON;

       								// Prepare TX-TCM-Encoder for M-QAM
       								// debug section, this will be a result of Optimization
       								//Modulation_Type = QAM256;
       								//Modulation_Type = QAM4;
       								Modulation_Init_QAM_M(1);  // Argument "1" means Encoder Initialization
       						   }
       						}
       					else
       						{
       						//timer
       						}
				break;
				
				case TX_WAIT_FOR_RESTART:
						TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
						if ((Suffix_is_detected)&&(Suffix_is_transmitted))
							{
							if ((tx_wait_for_restart_ctr++>4)
								&&(rx_wait_for_restart_ctr>4))
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
				
				case TX_M_QAM_TRAINING:
						TX_OFDM_Engine_State = TX_Engine_QAM_M_Training;
						
			       		//if (++n_blk_tx_M_QAM_train > N_SYMBOLS_M_QAM_TRAINING)
						if (++n_blk_tx_M_QAM_train > M_QAM_Training_Symbols)							
       						{
       						n_blk_tx_M_QAM_train--;	
       						// go to next QAM-Processing State
       			 			TX_QAM_State = TX_M_QAM_DATA;
       			 			TX_OFDM_Engine_State = TX_Engine_QAM_M_Data;
       			 			
       			 			// initialize and start V11 interface
//                          status = Init_V11_Data_IO(32,V11_Clock_Div_TX, ?RX_Buffer, ?TX_Buffer, OFDM_Num_32b_Words_TX);
//                          if (status != 0) exit(0);
//                          Start_V11_Data_IO();
       						
       			 			//tx_state_finished = 0;
       			 			//n_blk_tx_M_QAM_train = 1;
       						}
       					else
       						{
       						//timer
       						}
       						
				break;
				
				case TX_M_QAM_DATA:
						TX_OFDM_Engine_State = TX_Engine_QAM_M_Data;
						
			    		// Transmission of Data	
				
				break;
				
				
				default:      	  

      			//OFDM_Restart();

     		 	break;
              	}
		}
	}


