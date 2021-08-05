/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Preamble_scheduler_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : TX Preamble Scheduler
 *
 * REVISION HISTORY:
 * 		5.4.2004 PL new structure
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Preamble_scheduler_TX.c $
 * 
 * 9     24.07.08 15:34 Chhamaa
 * 
 * 4     5.04.06 15:28 Chhamaa
 * 
 * 4     17.03.06 14:41 Chhamaa
 * 
 * 2     18.07.05 8:46 Chharyt
 * Exchange with TLC. By date 2005-07-15    Version: OFDM_DSP_V0_09U
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 5     22.12.04 9:30 Chchlee
 * 
 * 4     1.10.04 12:42 Leeb02
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
void segment ( "seg_ext_pmco" ) Preamble_scheduler_TX()
	{
	
    void  PreambleTX2Tone_Init(int index0, int index1);

	switch ( TX_Preamble_State )
    	{
    	case TX_NL_Mode_Call:
    		// Master will send NL_Mode_Call
    		// (2-Tone with pilot scheme according to Optim_NL_State)
    		//
    		// Slave will send NL_Mode_Call only as a reply. if he is in 
    		// 	Optim_NL_State=Optim_NL_ON_Determination (Starttyp I)
    		//  and received a pilot scheme of Starttyp II
    		// 
    		// This case will be left under control of Preamble_scheduler_RX.c
    	
    	break;
    		
    	case TX_2TONE_1:
       	
       	 	if ( Start_TX == 1 )  	// this Flag ist set by receiver
       	 		{					//  in "OFDM_Detection_AGC.c"
       	 		if (txp_state_counter == number_of_txp_cycles)
       				{
       				// go to next TX Preamble State
       				TX_Preamble_State    = TX_PAUSE;
       				number_of_txp_cycles = N_SYMBOLS_PAUSE;
       				txp_state_counter = 0;
       				break;
       				}	
       	 		}
       	 	else
       	 		{
       	 		txp_state_counter = 0;
       	 		Num_Symbol_TX = 0;
       	 		}
       		
			
		break;
      	
		
        case TX_PAUSE:
			
       	 		if (txp_state_counter == number_of_txp_cycles)
       				{
       				// Neue Initialisierung der Pilotton-Positionen
       				PreambleTX2Tone_Init(Tone_ind0, Tone_ind1);
	
       				// go to next TX Preamble State
       				TX_Preamble_State    = TX_2TONE_2;
       				number_of_txp_cycles = N_SYMBOLS_TONE2;
       				txp_state_counter = 0;
       				break;
       				}	
        	
        break;
      	
        
    	case TX_2TONE_2:
    	
    	    	if (txp_state_counter == number_of_txp_cycles)
       				{
       				// go to next TX Preamble State
       				TX_Preamble_State    = TX_LFM;
       				// double number of OFDM symbols
       				// for N=64, due to doubled LFM sequence
       				if (use_long_RX_Filter_2NmL) number_of_txp_cycles = 2*N_SYMBOLS_LFM;
       				else						 number_of_txp_cycles = N_SYMBOLS_LFM;
       				txp_state_counter = 0;
       				break;
       				}	
 
		break;

		
      	case TX_LFM:
	     	
    	    	if (txp_state_counter == number_of_txp_cycles)
       				{
       				// go to next TX Preamble State
       				TX_Preamble_State    = TX_2TONE_3;
       				number_of_txp_cycles = N_SYMBOLS_TONE3;
       				txp_state_counter = 0;
       				break;
       				}	
 

		break;
		
      	case TX_2TONE_3:
      	
      			if (txp_state_counter == number_of_txp_cycles)
       				{
       				// go to next TX Preamble State
       				TX_Preamble_State    = TX_Preamble_IDLE;
       				txp_state_counter = 0;
       				
       				Start_TX_QAM_Engine = 1;
       				TX_QAM_State = TX_QAM_DUMMY1;
       				TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
       				
       				// debug: jump directly to M-QAM
       				//TX_QAM_State = TX_M_QAM_DATA;
       				//TX_OFDM_Engine_State = TX_Engine_QAM_M_Data;
       				
       				
       				break;
       				}	
 
		break;
      	
		
		
      	case TX_Preamble_IDLE:
      		// no operation
      		
		break;
      	
      	default:      	  

      	//OFDM_Restart();

      	break;
      	}
	}

	
	

