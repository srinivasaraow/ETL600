/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Preamble_scheduler_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : RX Preamble Scheduler
 *
 * REVISION HISTORY:
 * 		2.4.2004 PL new structure
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Preamble_scheduler_RX.c $
 * 
 * 10    24.07.08 15:34 Chhamaa
 * 
 * 7     6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 6     21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 3     15.02.05 10:02 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 2     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 5     2.12.04 17:54 Chhamaa
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

#include "../h/OFDM_defs.h"
#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/tcm/TCM_Codec_externs.h"

extern int giEmergency_restart;
 
//========================================
void segment ( "seg_ext_pmco" ) Preamble_scheduler_RX()
	{
	void OFDM_Restart();
	
	switch ( RX_Preamble_State )
    	{
    	// ----------- This case is for NL-Optimization only --------------------	
    	case RX_DETECTION_MODE:
    		Sync_State = Mode_Detection;
    		switch (Optim_NL_State)
    			{
    				// -----------------------------------------------------
    				case Optim_NL_ON_Determination: //Starttyp I
    					if (Pilot_Mode_detection == Pilot_Mode_I_detected)
    						{ // detected Starttyp I of far end modem
    						  // I am also in Starttyp I, so lets start rumbling
    						  // go to next Preamble State
    						 RX_Preamble_State = RX_DETECTION_AGC;
    						 TX_Preamble_State = TX_2TONE_1;
    						 rx_state_finished = 0;   
    						 rx_pilot_mode_ctr = 0;	  						
    						}
    						
    					if (Pilot_Mode_detection == Pilot_Mode_II_detected)
    						{ // detected Starttyp II of far end modem
    						  // I want to tell the other side ,that I am in Starttyp I
    						  // therfore I will answer with my Starttyp I Pilots, even if I am Slave
    						  // (I will pass a command to my local transmitter) 
    						  TX_NL_Call=1;	 
    						}
    				break;
    				// -----------------------------------------------------
    				case Optim_NL_ON_Selected:		//Starttyp II
    					if (Pilot_Mode_detection == Pilot_Mode_I_detected)
    						{ // detected Starttyp I of far end modem
    						  // restart with Starttyp I
    						 Optim_NL_State =  Optim_NL_ON_Determination;
    						 // Start with N and L base values for NL-Optimization
							 N = N_BASE;	
							 L = L_BASE;
    						 rx_pilot_mode_ctr = 0;	
    						 OFDM_Restart();    						
    						}
    					
    					if (Pilot_Mode_detection == Pilot_Mode_II_detected)
    						{ // detected Starttyp II of far end modem
    						  // I am also in Starttyp II, so lets start rumbling
    						  // go to next Preamble State
    						 RX_Preamble_State = RX_DETECTION_AGC;
    						 TX_Preamble_State = TX_2TONE_1;
    						 rx_state_finished = 0;   
    						 rx_pilot_mode_ctr = 0;						
    						}
    							
    					if (Pilot_Mode_detection == Pilot_not_detected)
    						{ // detected no pilot, I will wait some time and then
    						  // after time out I will restart with Starttyp I
    						 if(rx_pilot_mode_ctr++ > RX_PilotMode_TimeOut)
    						 	{
    						 	Optim_NL_State =  Optim_NL_ON_Determination;
    						 	// Start with N and L base values for NL-Optimization
								N = N_BASE;	
								L = L_BASE;
    						 	rx_pilot_mode_ctr = 0;	
    						 	OFDM_Restart();
    						 	}
    						}
    				break;
    				// -----------------------------------------------------
    				case Optim_NL_OFF:
    					// Error: if no NL-Optimization
    					// the modem should never be in this state
    					Error.Count=990;
    					exit(1);
    				break;
    			}
    			
    		
    	break;
    		      
       	case RX_DETECTION_AGC:
       		Sync_State = Tone2_Searching;
       		if (rx_state_finished)
       			{
       			// go to next Preamble State
       			RX_Preamble_State = RX_NOISE_EST;
       			rx_state_finished = 0;
       			Sync_State = Tone2_detected;
       			}
       			else
       			{
       				//timer
       		
       			}
			
		break;
      	
        case RX_NOISE_EST:
    	     if (rx_state_finished)
       			{
       			// go to next Preamble State
       			RX_Preamble_State = RX_FREQUENCY_EST;
       			rx_state_finished = 0;
       			//wait_timer = 0;
       			Sync_State = NoiseEst_finished;
       			}
       			else
       			{
       				//timer
       			// Check detection of Pause
       			if( rx_noise_est_in > MAX_RX_MUTE_START)
       				{//exit with ERROR_CODE RX_MUTE_START_ERROR
       				Error.Count++;
       				Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       				Error.Number[Error.Count].ID       = RX_MUTE_START_ERROR;
       				dbg_error[Error.Count]=RX_MUTE_START_ERROR;
       				}
       			// Check detection of Pause
       			if( rx_noise_est_out > MAX_RX_MUTE_STOP)
       				{//exit with ERROR_CODE RX_MUTE_START_ERROR
       				Error.Count++;
       				Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       				Error.Number[Error.Count].ID       = RX_MUTE_STOP_ERROR;
       				dbg_error[Error.Count]=RX_MUTE_STOP_ERROR;
       				}
       			}
   
		break;

      	case RX_FREQUENCY_EST:
			 if (rx_state_finished)
       			{
       			// go to next Preamble State
       			RX_Preamble_State = RX_LFM_PROC;
       			rx_state_finished = 0;
       			//wait_timer = 0;
       			Sync_State = FreqEst_finished;
       			}
       			else
       			{
       				//timer
       			}

		break;
		
		case RX_LFM_PROC:
	     	if (rx_state_finished)
       			{
       			// go to next Preamble State
       			RX_Preamble_State = RX_QAM4_DETECT;
       			rx_state_finished = 0;
    			Sync_State = LFM_finished;
       			}
       			else
       			{
       			// Check Time for detection of LFM
       			if( rx_lfm_in > MAX_RX_LFM_WAIT)
       				{//exit with ERROR_CODE RX_LFM_WAIT_ERROR
       				Error.Count++;
       				Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       				Error.Number[Error.Count].ID       = RX_LFM_WAIT_ERROR;
       				dbg_error[Error.Count]=RX_LFM_WAIT_ERROR;
       				giEmergency_restart = 1;
       				}
       			}

		break;
		
      	case RX_QAM4_DETECT:
      		 if (rx_state_finished)
       			{
       			// go to next Preamble State
       			RX_Preamble_State = RX_Preamble_IDLE;
       			rx_state_finished = 0;
	   			Sync_State = QAM4_detected;
       			rx_qam4_in = 0;
       			
       			// Start QAM Scheduler RX
       			Start_RX_QAM_Engine  = ON;
				Pause_RX_QAM_Engine  = OFF;
       			RX_QAM_State 		 = RX_QAM_PREFIX;
       			RX_OFDM_Engine_state = RX_Engine_QAM4;
       			rx_prefix_counter    = 0;
       			
       			// Start Channel Data Transmission in QAM_scheduler_TX.c
       			Start_CH_DATA_Transmission = 1;
       			
       			// jump direct to M-QAM  (debug)
       			//RX_QAM_State 		 = RX_M_QAM_DATA;
       			//RX_OFDM_Engine_state = RX_Engine_QAM_M_Data;
       			
       			}
       			else
       			{
       			// Check Time for detection of QAM4
       			if( rx_qam4_in++ > MAX_RX_QAM4_WAIT)
       				{//exit with ERROR_CODE RX_LFM_WAIT_ERROR
       				Error.Count++;
       				Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       				Error.Number[Error.Count].ID       = RX_QAM4_WAIT_ERROR;
       				dbg_error[Error.Count]=RX_QAM4_WAIT_ERROR;
       				}
       				
       			}
      	
      	

		break;
      	
      	case RX_Preamble_IDLE:
      		// no operation
      		
		break;
      	
      	default:      	  

      	//OFDM_Restart();

      	break;
      	}
	}


