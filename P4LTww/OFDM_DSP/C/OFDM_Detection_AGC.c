/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Detection_AGC.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : 2 tone signal detection and primary analog and digital AGC setting	
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Detection_AGC.c $
 * 
 * 10    24.07.08 15:29 Chhamaa
 * 
 * 5     5.04.06 15:28 Chhamaa
 * 
 * 5     17.03.06 14:41 Chhamaa
 * 
 * 2     15.02.05 10:01 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Case 0: "Power" calculation correction
 *       02.03.04 vgp
 * SDRAM memory functions
 *       02.03.04 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//========================================
		void segment ( "seg_ext_pmco" ) OFDM_Detection_AGC()
			{
        	void fft(int, float *, float pm *, complex_float *);
		
       		// Primary AGC setting
       		switch ( Det_Agc_RX )
            	{
            	case 0:
      			// OFDM received symbol extraction
			   	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

	  			// Calculate spectrum of received signal  
      			fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   

    			// Energy of the signal
      			Energy = vsummag2_uc_DP( Fl_Array_R, Fl_Array_I, N );
      			
      			   				
    			// Energy near Pilot 0 (Window = 3 bin)
    			Energy_Pil0 = vsummag2_uc_DP( &Fl_Array_R[Spilot_RX_0-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Spilot_RX_0-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			// Energy near Pilot 1 (Window = 3 bin)
    			Energy_Pil1 = vsummag2_uc_DP( &Fl_Array_R[Spilot_RX_1-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Spilot_RX_1-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
        		
				if (Energy > 0)
				{    			                              
    			                              
    				// Energy otside of windows
    				Energy -= (Energy_Pil0 + Energy_Pil1);
	    			
	    			// Power otside of windows
	    			Power = Energy/(float)(N-OFDM_PILOTS_NUM*DET_WIN_LEN); 
	    			
	    			// Power of the pilots
	    			Power_Pil = (Energy_Pil0 + Energy_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN); // 02.03.04 vgp
	    			
	    			// Fisher statistic for detection
	    			FS_Det = Power_Pil/(Power+5.0E-7f);
	    			
	    			// Decision about detection
	    			if (FS_Det > Detection_Treshhold)
	    				{
	    				// Signal is detected!
	    				// Start of transmitting in SLAVE modem
	    				tone_detected  = tone_detected_state;
	      				rx_state_finished = 0;
	    				Start_TX = 1;
	    				Det_Agc_RX++;
	    				}
					else
    					{
	    				// Signal not detected
	    				tone_detected  = tone_not_detected_state;
	    				}
				}
				else
				{
				// No Energy then No Signal
    			tone_detected  = tone_not_detected_state;
    			}
    	       	
    			break;
    			
            	case 1:

                // Set analog AGC

               	// OFDM received symbol extraction
			   	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

	  			// Calculate spectrum of received signal  
      			fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   
      			
    			// Energy near Pilot 0 (Window = 3 bin)
    			Energy_Pil0 = vsummag2_uc_DP( &Fl_Array_R[Spilot_RX_0-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Spilot_RX_0-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			                              
    			// Energy near Pilot 1 (Window = 3 bin)
    			Energy_Pil1 = vsummag2_uc_DP( &Fl_Array_R[Spilot_RX_1-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Spilot_RX_1-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			// Power of the pilots
    			Power_Pil = (Energy_Pil0 + Energy_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN);

    			Analog_Gain=Analog_Gain * sqrtf(Des_Val_Pow_Pil/Power_Pil);

    			Det_Agc_RX++;

               	break;
               	
            	case 2:
				// Set digital AGC

               	// OFDM received symbol extraction
			   	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

	  			// Calculate spectrum of received signal  
      			fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   
      			
    			// Energy near Pilot 0 (Window = 3 bin)
    			Energy_Pil0 = vsummag2_uc_DP( &Fl_Array_R[Spilot_RX_0-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Spilot_RX_0-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			                              
    			// Energy near Pilot 1 (Window = 3 bin)
    			Energy_Pil1 = vsummag2_uc_DP( &Fl_Array_R[Spilot_RX_1-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Spilot_RX_1-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			// Power of the pilots
    			Power_Pil = (Energy_Pil0 + Energy_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN);

    			Digital_AGC = sqrtf(Des_Val_Pow_Pil/Power_Pil);
    			
                Det_Agc_RX++;

                break;

		        case 3:
       	
        		// Decide Referenz Pilot
        	#if 0	
        		// OFDM received symbol extraction
			   	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

	  			// Calculate spectrum of received signal  
      			fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   
      			
    			// Energy near Pilot 0 (Window = 3 bin)
    			Energy_Pil0 = vsummag2_uc_DP( &Fl_Array_R[Tone_ind0-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Tone_ind0-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			                              
    			// Energy near Pilot 1 (Window = 3 bin)
    			Energy_Pil1 = vsummag2_uc_DP( &Fl_Array_R[Tone_ind1-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[Tone_ind1-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			                           
				// Compare Energies
				if (Energy_Pil0 > Energy_Pil1)
					{
					// Setze Pilot_ind1 (Referenz-Pilot) auf Pilot Position 0
					Pilot_ind1 = Tone_ind0;
					Pilot_ind0 = Tone_ind1;
					}
					else
					{
					// Setze Pilot_ind1 (Referenz-Pilot) auf Pilot Position 1
					Pilot_ind1 = Tone_ind1;
					Pilot_ind0 = Tone_ind0;
					}
        	#else	
        			Pilot_ind1 = Tone_ind1;
					Pilot_ind0 = Tone_ind0;
			#endif	
        		
                Det_Agc_RX++;

        		break;

        		case 4:
       	
        		// Reset inner State machine
                Det_Agc_RX = 0;
                // Set Parent State Machine Flag
      			rx_state_finished = 1;

        		break;
            	}
			}


