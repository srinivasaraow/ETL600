/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: Error_handler.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Evaluates internal error codes
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 *
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Error_handler.c $
 * 
 * 14    24.07.08 15:25 Chhamaa
 * 
 * 11    6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 10    21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 7     5.07.05 11:13 Chalgyg
 * OFDM_DSP_V0_09U
 * 
 * 5     24.02.05 7:27 Chchlee
 * 
 * 3     1.02.05 13:09 Chalgyg
 * Version 0.09A
 * 
 * 6     22.12.04 9:30 Chchlee
 * 
 * 5     1.10.04 11:56 Leeb02
 * Initial version
 *       09.06.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>

#include "../h/OFDM_externs.h"
#include "../h/OFDM_Defs.h"
#include "../h/tdm/ofdm_tdm_externs.h"

// Anzahl der OFDM-Symbole, um die der Restart bei SyncAlarm verzögert wird
#define DELAY_RESTART_SYMBOLS 40

int Delay_Restart_Ctr = 0;
//

int giEmergency_restart;

void  OFDM_Restart();

void segment ( "seg_ext_pmco" ) Error_handler()
// // 
{
	int do_restart;
	
	//PL16.05.2006
	OFDM_Alarm.ofdm_syncstate = Sync_State & 0xFF;
	
	do_restart=0;
	
	// Handler for OFDM_Alarm.pilotAlm
	// "tone_detected" is set in "OFDM_Detection_AGC.c" !!
	//				and initialized in "Modem_init_basic.c" to unknown state 
	switch (tone_detected)
		{
		case unknown_state:
				// no Alarm in unknown state (during startup)
	       		OFDM_Alarm.pilotAlm = 0 & 0x1;
		break;
		
		case tone_detected_state:
			// clear alarm
			OFDM_Alarm.pilotAlm = 0 & 0x1;
			//		pilotAlm_waitcounter = 0;
		break;
		
		case tone_not_detected_state:
			//if (pilotAlm_waitcounter++ > 20)
       		//		{
       				// set Alarm Flag
       				OFDM_Alarm.pilotAlm = 1 & 0x1;
       		//		pilotAlm_waitcounter = 21;
       		//		}
		break;
		}
	
	// General Error handler
		
	if (Error.Count != Error_count_shd)
		{
		// new error occurred
		Error_count_shd = Error.Count;
		
		switch (Error.Number[Error.Count].ID)
			{
			case RX_MUTE_START_ERROR:
			case RX_MUTE_STOP_ERROR:
			case RX_LFM_WAIT_ERROR:
				do_restart=1;
			break;
			case RX_REF_PILOT_WRN:
			break;
			
			case RX_QAM4_WAIT_ERROR:
			case RX_GNRL_2T_WAIT_ERROR:
				do_restart=1;
			break;
			
			case ERROR_PREFIX:
				do_restart=1;
			break;
			case ERROR_CH_DATA:
				do_restart=1;
			break;
			case ERROR_OPTIM1:
			break;
			case ERROR_STUFFING_SETTING:
			case ERROR_RATE_OUT_OF_RANGE:
			// to be done: inform Master DSP TBD
				do_restart=1;
			break;
			case WRNG_MASTER_MASTER_CNFG:
				OFDM_Alarm.configAlm  = 1 & 0x1;
			break;
			 
			case ERROR_SUFFIX_DURING_OPTIM:
			break; 
			
			case ERROR_OPTIM2:
			break;
			case ERROR_OPTIM3:
				do_restart=1;
			break;
			case ERROR_SUFFIX:
				do_restart=1;
			break;
			
			case ERROR_CRC:
			case ERROR_LEVEL:
				// Special case at the end of State Optim_NL_ON_Determination:
				// Following "if" command is for the case that one modem does Restart
				// and as a result other modem gets LEVEL or CRC Error.
				// If this situation occurs during wait for Restart in other Startmode
				// this case is catched here and the othe start mode is set here
				if ((Optim_NL_State == Optim_NL_ON_Determination)
					&& (RX_QAM_State == RX_WAIT_FOR_RESTART)
					&& (TX_QAM_State == TX_WAIT_FOR_RESTART)
					&& (Suffix_is_detected)&&(Suffix_is_transmitted))
					{
						// Prepare Restart in new state	
						Optim_NL_State = Optim_NL_ON_Selected;
       					N=N_selected;
       					L=L_selected;
       					//N=128;
       					//L=16;
					}
					
				do_restart=1;
			break;
			
			case ERROR_STUFFING:
			break;
			case ERROR_TCM_INIT:
				do_restart=1;
			break;
			
			
			case ERROR_ABBRUCH:
			case ERROR_FALLBACK:
			case ERROR_FALLFORWARD:
				do_restart=1;
			break;
			}
		
		}
	// Wrap around
	if (Error.Count > ERROR_LOG_SIZE-10) //wrap around of buffer 
		{
		Error.Count =0;
		Error_count_shd =0;
		}
		
	if (do_restart | giEmergency_restart)
		{
		// -- handler for OFDM_Alarm.syncAlm
		// Note: OFDM_Alarm.syncAlm is set in here but reset in
		//							QAM_scheduler_RX()	!!
		// increment restart counter for sync Alarm
    	syncAlm_restartcounter++;
    	// if more than 10 Restarts occured without Synchronization
    	// (condition for synchronization:
    	//		number of received OFDM M-QAM Datasymbols more than 500)
    	// (condition for synchronization is checked in "QAM_scheduler_RX.c")
    	// then set sync Alarm
    	if (syncAlm_restartcounter > 10)
	    	{
	    	// set sync Alarm	
			OFDM_Alarm.syncAlm    = 1 & 0x1;
			
			// set Flag for Delayed Restart
			//Delay_Restart_Flg = 1;
			
			// Initialize Transmitter for Mute-Signal
			Start_TX_QAM_Engine = OFF;
			TX_Preamble_State = TX_Preamble_IDLE; //Mute
			// Initialize Receiver for IDLE-State
			Start_RX_QAM_Engine = OFF;
			RX_Preamble_State = RX_Preamble_IDLE;
			Frequency_Control_enable = OFF;
			
			giEmergency_restart = 1;
			// note: from now on the programm will return to this position with every OFDM_Symbol, because
			// giEmergency_restart is kept on 1
			
			// Increment Counter for Delayed Restart
			if (Delay_Restart_Ctr++ > DELAY_RESTART_SYMBOLS) {
				// delay Restart
				//Delay_Restart_Flg = 0;
	    		Delay_Restart_Ctr = 0;
	    		giEmergency_restart = 0;
	    		syncAlm_restartcounter = 0;
	    		// Restart will be executed immediately 
	    		// further below, because now syncAlm_restartcounter <= 10
	    		// OFDM_Restart(); is therefore not required here	
				}
			
			
			
			}
	    //--
	    
	    // restart immediately	
	    if (syncAlm_restartcounter <= 10)
	    	OFDM_Restart();
	    	
	    	
	    	
		}
	
}

//-------------------------------
void Init_Delay_Restart(void) {
	Delay_Restart_Ctr = 0;
}

