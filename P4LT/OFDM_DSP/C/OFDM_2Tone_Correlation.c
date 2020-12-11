/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_2Tone_Correlation.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Correlation of input signal with 2 tone reference signal
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_2Tone_Correlation.c $
 * 
 * 9     24.07.08 15:28 Chhamaa
 * 
 * 4     5.04.06 15:28 Chhamaa
 * 
 * 4     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *		 12.07.2004 PL checked for Energy
 *       15.07.2004 PL removed OFDM_restart / replaced by Error handler
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_2Tone_Correlation(int State_Corr, int *State_Cnt)
	{
		float Exp_Gen(float start_ph, complex_float rotph,
              		  complex_float *foc, float stepphsymbol,
              		  int length);
        //void OFDM_Restart();
      		  
		complex_float b;
		float startph;

		// 1st pilot reference signal in Compl_Exp
		startph = Exp_Gen(0.0f,Rot_Pilot0,Compl_Exp,0.0f,NL);
       	
      	// Energy of projection on 1st tone signal subspace
       	b = vdot_c_DD(Cmplx_Array,Compl_Exp,NL);
       	Tone_Space_Energy = b.re*b.re+b.im*b.im;

	  	// 2nd pilot reference signal in Compl_Exp
		startph = Exp_Gen(0.0f,Rot_Pilot1,Compl_Exp,0.0f,NL);
       	
      	// Energy of projection on 2nd tone signal subspace
       	b = vdot_c_DD(Cmplx_Array,Compl_Exp,NL);
       	Tone_Space_Energy += b.re*b.re+b.im*b.im;

       	// Full energy of RX signal
      	Full_Energy = vsummag2_c_D(Cmplx_Array,NL);
	   	
      	

      	// Decision for the next state
		if ( State_Corr == NEG )
			{
			if (Full_Energy > 0) {
				// Correlation
      			R_Corr = Tone_Space_Energy/Full_Energy/(float)NL;
				// Test for Corr threshold
	  			if(R_Corr > DETECTION_THRESHOLD)
		  			{	  	
	    			// LFM or QAM signal is not found
	    			Is_Waiting--;	    	    
	    			// Restart after too long waiting for LFM or QAM signal
	    			if(Is_Waiting < 0) 
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       					Error.Number[Error.Count].ID       = RX_GNRL_2T_WAIT_ERROR;
	    				} 
	    			}
				else
					{
					(*State_Cnt)++;
					// Restore waiting counter
   	    			Is_Waiting = WAITING_SYMBOLS;
					}
				}
			else
				{// no energy, no 2-tone
				(*State_Cnt)++;
				// Restore waiting counter
   	    		Is_Waiting = WAITING_SYMBOLS;
				}
			}
			
		if ( State_Corr == POS )
			{
			if (Full_Energy > 0) {
				// Correlation
      			R_Corr = Tone_Space_Energy/Full_Energy/(float)NL;
				// Test for Corr threshold
	  			if(R_Corr > DETECTION_THRESHOLD)
		  			{	  	
	    			// 2-tone is detected
 					(*State_Cnt)++;
   	    			Is_Waiting = WAITING_SYMBOLS;
	  				}
				else
					{
	    			Is_Waiting--;	    	    
	    			// Restart after too long waiting for 2-tone-signal
	    			if(Is_Waiting < 0)
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       					Error.Number[Error.Count].ID       = RX_GNRL_2T_WAIT_ERROR;
	    				}  
					}
				}
			else
				{// no energy, no 2-tone
				Is_Waiting--;	    	    
	    		// Restart after too long waiting for 2-tone-signal
	    		if(Is_Waiting < 0)
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       					Error.Number[Error.Count].ID       = RX_GNRL_2T_WAIT_ERROR;
	    				} 
				}

			}
   		}

void segment ( "seg_ext_pmco" ) OFDM_2Tone_Detection(int State_Corr, int *State_Cnt, int index_tone0, int index_tone1)
	{

	void fft(int, float *, float pm *, complex_float *);
		
	// OFDM received symbol extraction
   	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

	// Calculate spectrum of received signal  
	fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   

	// Energy of the signal
    Energy = vsummag2_uc_DP( Fl_Array_R, Fl_Array_I, N );
      			
      			   				
    // Energy near Pilot 0 (Window = 3 bin)
    Energy_Pil0 = vsummag2_uc_DP( &Fl_Array_R[index_tone0-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[index_tone0-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    // Energy near Pilot 1 (Window = 3 bin)
    Energy_Pil1 = vsummag2_uc_DP( &Fl_Array_R[index_tone1-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[index_tone1-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
        		
    			
    if ( State_Corr == NEG )
		{
		if (Energy > 0) {
				// Energy otside of windows
    			Energy -= (Energy_Pil0 + Energy_Pil1);
    			
    			// Power otside of windows
    			Power = Energy/(float)(N-OFDM_PILOTS_NUM*DET_WIN_LEN); 
    			
    			// Power of the pilots
    			Power_Pil = (Energy_Pil0 + Energy_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN); // 02.03.04 vgp
    			
    			// Fisher statistic for detection
    			FS_Det = Power_Pil/(Power+5.0E-7f);
    			
				// Test for Corr threshold
	  			if(FS_Det > Detection_Treshhold)
		  			{	  	
	    			// 2 Tone still detected
	    			Is_Waiting--;	    	    
	    			// Restart after too long waiting for LFM or QAM signal
	    			if(Is_Waiting < 0) 
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       					Error.Number[Error.Count].ID       = RX_GNRL_2T_WAIT_ERROR;
	    				} 
	    			}
				else
					{
					(*State_Cnt)++;
					// Restore waiting counter
   	    			Is_Waiting = WAITING_SYMBOLS;
					}
				}
			else
				{// no energy, no 2-tone
				(*State_Cnt)++;
				// Restore waiting counter
   	    		Is_Waiting = WAITING_SYMBOLS;
				}
			}
    			
	if ( State_Corr == POS )
		{
		if (Energy > 0) {
				// Energy otside of windows
    			Energy -= (Energy_Pil0 + Energy_Pil1);
    			
    			// Power otside of windows
    			Power = Energy/(float)(N-OFDM_PILOTS_NUM*DET_WIN_LEN); 
    			
    			// Power of the pilots
    			Power_Pil = (Energy_Pil0 + Energy_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN); // 02.03.04 vgp
    			
    			// Fisher statistic for detection
    			FS_Det = Power_Pil/(Power+5.0E-7);
    			
				// Test for Corr threshold
	  			if(FS_Det > Detection_Treshhold)
		  			{	  	
	    			// 2-tone is detected
 					(*State_Cnt)++;
   	    			Is_Waiting = WAITING_SYMBOLS;
	  				}
				else
					{
	    			Is_Waiting--;	    	    
	    			// Restart after too long waiting for 2-tone-signal
	    			if(Is_Waiting < 0)
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       					Error.Number[Error.Count].ID       = RX_GNRL_2T_WAIT_ERROR;
	    				}  
					}
				}
			else
				{// no energy, no 2-tone
				Is_Waiting--;	    	    
	    		// Restart after too long waiting for 2-tone-signal
	    		if(Is_Waiting < 0)
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       					Error.Number[Error.Count].ID       = RX_GNRL_2T_WAIT_ERROR;
	    				} 
				}

			}
   	
	}
