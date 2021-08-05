/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Noise_Estimation.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Esimation of spectral density and power of additive noise
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Noise_Estimation.c $
 * 
 * 7     24.07.08 15:31 Chhamaa
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 5     22.12.04 9:30 Chchlee
 * 
 * 4     2.12.04 17:54 Chhamaa
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 	Wait for Noise estimation until
 *  own transmitter sends mute signal
 *		28.10.04 PL
 *
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

unsigned int debug_wait_ctr=0;
extern int Delay_Measure_Flag;

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Noise_Estimation()
	{
	void  fft(int, float *, float pm *, complex_float *);
	void OFDM_2Tone_Correlation(int, int *);
	//void OFDM_2Tone_Detection(int, int *);
	void OFDM_2Tone_Detection(int , int *, int , int);

	void OFDM_NB_Detection();
	void OFDM_AWGN_NB_Estimation();
	
	int tmp_pilot;
	
	switch ( Noise_Est_RX )
   		{
    	case 0:	//PT min=2
		
   		// Increment wait counter  
   		rx_noise_est_in++;
   		// Detection of pause
   		//OFDM_2Tone_Correlation(NEG,&Noise_Est_RX);
   		//OFDM_2Tone_Detection(NEG, &Noise_Est_RX, Tone_ind0, Tone_ind1);
   		OFDM_2Tone_Detection(NEG, &Noise_Est_RX, Spilot_RX_0, Spilot_RX_1);
		rx_state_finished = 0;
		
   		break;
   		
   		case 1: //PT=4
		
   		// Increment wait counter  
   		rx_noise_est_in++;
   		Delay_Measure_Flag=0;
   		// Wait until own Transmitter sends Pause
   		if ((TX_Preamble_State == TX_PAUSE)
   				&& (txp_state_counter>3))  { Noise_Est_RX++;}
   				else debug_wait_ctr++;

		rx_state_finished = 0;
		
   		break;
   		

   		case 2:
   		
		// AWGN Estimation   PT= N_EST_SYMBOLS+1 =13
		OFDM_AWGN_NB_Estimation();
		rx_state_finished = 0;

   		break;

   		case 3:

   		// NB Estimation	PT=1
   		OFDM_NB_Detection();
   		rx_state_finished = 0;
   		
   		Noise_Est_RX++;

   		break;

   		case 4:			//	PT=wait(5)
   		// Increment wait counter  
   		rx_noise_est_out++;
		// Detection of 2-tone signal
		//OFDM_2Tone_Correlation(POS,&Noise_Est_RX);
		OFDM_2Tone_Detection(POS, &Noise_Est_RX, Tone_ind0, Tone_ind1);

		rx_state_finished = 0;
		
   		break;
      	
   		case 5:
   		// Reset Time Out counters
   		//rx_noise_est_in =0;
   		//rx_noise_est_out=0;
   		
   		// Check if at least one pilot is unaffected
   		if ((Mask_NB[Pilot_ind0]==0.0) && (Mask_NB[Pilot_ind1]==0.0))
   		{// both pilots are affected
   		 // choose pilot 1 and delete Mask to prevent deadlock in far end receiver
   		 	Mask_NB[Pilot_ind1]=1.0;
   		 	Error.Count++;
       		Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       		Error.Number[Error.Count].ID       = RX_REF_PILOT_WRN;
   		} 
   		// Consider NB-Interferer in choice of reference pilot
   		if (Mask_NB[Pilot_ind1]==0.0)
   		{// if NB-Interferer on selected pilot
   		 // switch to other pilot
			tmp_pilot = Pilot_ind0;
   			Pilot_ind0 = Pilot_ind1;
   			Pilot_ind1 = tmp_pilot;
   		}
   	
   		Noise_Est_RX++;
   		rx_state_finished = 0;
   		
   		
   		break;

   		case 6:
   		
   		// Empty case
   		Noise_Est_RX++;
   		rx_state_finished = 0;
   		
   		break;

   		case 7:
		
   		// Reset inner State machine
        Noise_Est_RX = 0;
        // Set Parent State Machine Flag
      	rx_state_finished = 1;

		break;
		}
	}
