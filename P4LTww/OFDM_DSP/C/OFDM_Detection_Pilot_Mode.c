/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Detection_Pilot_Mode.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : 2 tone signal detection and Start Mode selection	
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Detection_Pilot_Mode.c $
 * 
 * 4     5.04.06 15:28 Chhamaa
 * 
 * 4     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 1     22.12.04 9:30 Chchlee
 *
 * Initial version
 *       07.12.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"


float FS_DetI,FS_DetII;

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Detection_Pilot_Mode()
		{
       	void fft(int, float *, float pm *, complex_float *);
		float Energy_TypeII_Pil0,Energy_TypeII_Pil1;
		float Energy_TypeI_Pil0,Energy_TypeI_Pil1;
		float Energy_TypeII,Energy_TypeI;
		
       	
      			// OFDM received symbol extraction
			   	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

	  			// Calculate spectrum of received signal  
      			fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   

    			// Energy of the signal
      			Energy = vsummag2_uc_DP( Fl_Array_R, Fl_Array_I, N );
      			
      			Pilot_Mode_detection = Pilot_not_detected;
      			
      			if (Energy >0)
      			{
      			// -- Starttyp II -- check for PILOT0_DEFAULT  , PILOT1_DEFAULT
      			//-------------------------------------------------------- 				
      			// Energy near Pilot 0 (Window = 3 bin)
    			Energy_TypeII_Pil0 = vsummag2_uc_DP( &Fl_Array_R[PILOT0_DEFAULT-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[PILOT0_DEFAULT-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			// Energy near Pilot 1 (Window = 3 bin)
    			Energy_TypeII_Pil1 = vsummag2_uc_DP( &Fl_Array_R[PILOT1_DEFAULT-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[PILOT1_DEFAULT-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			// Energy outside of windows
    			Energy_TypeII = Energy -(Energy_TypeII_Pil0 + Energy_TypeII_Pil1);
      			
    			// -- Starttyp I -- check for PILOT0_NL_OPT  , PILOT1_NL_OPT
      			//-------------------------------------------------------- 								
    			// Energy near Pilot 0 (Window = 3 bin)
    			Energy_TypeI_Pil0 = vsummag2_uc_DP( &Fl_Array_R[PILOT0_NL_OPT-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[PILOT0_NL_OPT-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);
    			// Energy near Pilot 1 (Window = 3 bin)
    			Energy_TypeI_Pil1 = vsummag2_uc_DP( &Fl_Array_R[PILOT1_NL_OPT-((DET_WIN_LEN-1)/2)], 
    			                              &Fl_Array_I[PILOT1_NL_OPT-((DET_WIN_LEN-1)/2)],
    			                              DET_WIN_LEN);		                              
        		// Energy outside of windows
    			Energy_TypeI = Energy -(Energy_TypeI_Pil0 + Energy_TypeI_Pil1);
    			//-------------------------------------------------------- 								

    		
    			// -- Starttyp II -- FS check for PILOT0_DEFAULT  , PILOT1_DEFAULT
      			//-------------------------------------------------------- 		
    			// Power outside of windows
    			Power = Energy_TypeII/(float)(N-OFDM_PILOTS_NUM*DET_WIN_LEN); 
    			
    			// Power of the pilots
    			Power_Pil = (Energy_TypeII_Pil0 + Energy_TypeII_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN); // 02.03.04 vgp
    			
    			// Fisher statistic for detection
    			FS_DetII = Power_Pil/(Power+5.0E-7f);
    			
    			// Decision about detection
    			if (FS_DetII > Detection_Treshhold)
    				{
    				// Pilot Scheme Mode I is detected!
    				Pilot_Mode_detection = Pilot_Mode_II_detected;
    				}
    				
    			// -- Starttyp I -- FS check for PILOT0_NL_OPT  , PILOT1_NL_OPT
      			//-------------------------------------------------------- 		
    			// Power outside of windows
    			Power = Energy_TypeI/(float)(N-OFDM_PILOTS_NUM*DET_WIN_LEN); 
    			
    			// Power of the pilots
    			Power_Pil = (Energy_TypeI_Pil0 + Energy_TypeI_Pil1)/(OFDM_PILOTS_NUM*DET_WIN_LEN); // 02.03.04 vgp
    			
    			// Fisher statistic for detection
    			FS_DetI = Power_Pil/(Power+5.0E-7f);
    			
    			// Decision about detection
    			if (FS_DetI > Detection_Treshhold)
    				{
    				// Pilot Scheme Mode II is detected!
    				Pilot_Mode_detection = Pilot_Mode_I_detected;
    				}
    						
    			// -- Combined check
      			//-------------------------------------------------------- 		
    			if ((FS_DetII > Detection_Treshhold)&&(FS_DetI > Detection_Treshhold))
    	       		{
    	       		Error.Count=999;
    	       		exit(1);	
    	       		}
    	       		
      			}
   		}


