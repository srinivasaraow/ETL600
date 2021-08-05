/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Freq_LinReg_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Linear regression for frequency offset calculation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log  $
 * 
 * 
 * Initial version
 *       01.12.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <trans.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

/*---------------------------------------
Input: 
	Df_Array[NL*LEN_DF_EST_SUBBLOCK]
	Len_Lin_Regr
Output:
	Fr_Pil0
	Fr_Pil1
Usage of Result: Calculation of Frequency and Clock Offset
	Example:
	Freq_Offset=((float)Pilot_ind1*Fr_Pil0-
                         (float)Pilot_ind0*Fr_Pil1)/(float)(Pilot_ind1-Pilot_ind0);
    Clock_Offset=(Fr_Pil0-Fr_Pil1)/(float)(Pilot_ind0-Pilot_ind1)
           	             /OFDM_TWO_PI*(float)N;
------------------------------------------*/           
		
//========================================

void segment ( "seg_ext_pmco" ) OFDM_Freq_LinReg_RX()
	{
	void fft(int, float *, float pm *, complex_float *);
		
   	int i;
   	
	for(i=0;i<Len_Lin_Regr;i++)
				{
				// Calculate spectrum of received 2 tone signal (LEN_FFT_DF_EST)  
				vcopy_DE((float *)&Cmplx_Array[0],(float *)&Df_Array[i*LEN_FFT_DF_EST],2*LEN_FFT_DF_EST);
      			fft(LEN_FFT_DF_EST,Fl_Array_R,Fl_Array_I,Cmplx_Array);   
   				// Save spectrum as complex
   				vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,LEN_FFT_DF_EST); 	  
			
   				// Calculation of the phase of both pilots
   				//Ph_Pil0[i]
   			    //	   =atan2f(Cmplx_Array[Tone_ind0*LEN_FFT_DF_EST/N].im,Cmplx_Array[Tone_ind0*LEN_FFT_DF_EST/N].re+0.00001f);
   				//Ph_Pil1[i]
   			    //	   =atan2f(Cmplx_Array[Tone_ind1*LEN_FFT_DF_EST/N].im,Cmplx_Array[Tone_ind1*LEN_FFT_DF_EST/N].re+0.00001f);
				Ph_Pil0[i]
   			    	   =atan2f(Cmplx_Array[Pilot_ind0*LEN_FFT_DF_EST/N].im,Cmplx_Array[Pilot_ind0*LEN_FFT_DF_EST/N].re+0.00001f);
   				Ph_Pil1[i]
   			    	   =atan2f(Cmplx_Array[Pilot_ind1*LEN_FFT_DF_EST/N].im,Cmplx_Array[Pilot_ind1*LEN_FFT_DF_EST/N].re+0.00001f);
				}

		// Pilot 0
		// Prepare initial consitions (for first execution)
		if (Start_Ph_Pil0 == 0.0f) // only in first execution
				{
				Start_Ph_Pil0 = Ph_Pil0[0];
				ow0 = 0.0f;
				}
		
		// Linear regression for pilot 0
      	if(fabsf(Ph_Pil0[0] - Start_Ph_Pil0) > OFDM_PI)
      		{
      		ow0 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil0[0] - Start_Ph_Pil0);
      		}

      		es1 = ow0 + Ph_Pil0[0];
   			ens1 = ow0 + Ph_Pil0[0];
			
   			for(i=1;i<Len_Lin_Regr;i++)
				{
      			if(fabsf(Ph_Pil0[i]-Ph_Pil0[i-1]) > OFDM_PI)
      				{
      				ow0 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil0[i]-Ph_Pil0[i-1]);
      				}
      			es1 += ow0+Ph_Pil0[i];
      			ens1 += (i+1)*(ow0+Ph_Pil0[i]);
      			}

			Fr_Pil0=(12.0f*ens1-6.0f*((float)Len_Lin_Regr+1.0f)*es1)/
   				(float)((Len_Lin_Regr+1)*(Len_Lin_Regr-1)*LEN_FFT_DF_EST*Len_Lin_Regr);

		
   		Start_Ph_Pil0 = Ph_Pil0[Len_Lin_Regr - 1];

   		// Pilot 1
		// Prepare initial consitions (for first execution)
		if (Start_Ph_Pil1 == 0.0f) // only in first execution
				{
				Start_Ph_Pil1 = Ph_Pil1[0];
				ow1 = 0.0f;
				}
		
		// Linear regression for pilot 1
      	if(fabsf(Ph_Pil1[0] - Start_Ph_Pil1) > OFDM_PI)
      		{
      		ow1 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil1[0] - Start_Ph_Pil1);
      		}

      		es1 = ow1 + Ph_Pil1[0];
   			ens1 = ow1 + Ph_Pil1[0];

			//ow1 = 0.0f;
			//es1 = Ph_Pil1[0];
   			//ens1= Ph_Pil1[0];
   
   			for(i=1;i<Len_Lin_Regr;i++)
				{
      			if(fabsf(Ph_Pil1[i]-Ph_Pil1[i-1]) > OFDM_PI)
      				{
      				ow1 -= OFDM_TWO_PI*fsignf(1.0f,Ph_Pil1[i]-Ph_Pil1[i-1]);
      				}
      			es1 += ow1 + Ph_Pil1[i];
      			ens1 += (i+1)*(ow1+Ph_Pil1[i]);
      			}

			Fr_Pil1=(12.0f*ens1-6.0f*((float)Len_Lin_Regr+1.0f)*es1)/
   				(float)((Len_Lin_Regr+1)*(Len_Lin_Regr-1)*LEN_FFT_DF_EST*Len_Lin_Regr);

   	
   		Start_Ph_Pil1 = Ph_Pil1[Len_Lin_Regr - 1];		

   		//	ow0 = 0.0f;
		//	ow1 = 0.0f;			
   	}
		
