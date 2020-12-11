/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_AWGN_NB_Estimation.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Esimation of spectral density and power of AWGN and narrow band
 *               interference
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_AWGN_NB_Estimation.c $
 * 
 * 2     26.04.05 8:43 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     2.12.04 17:54 Chhamaa
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Function revision
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
void segment ( "seg_ext_pmco" ) OFDM_AWGN_NB_Estimation()
	{
	void  fft(int, float *, float pm *, complex_float *);
	void  vdivz_f_EEE(float * ,float * ,float * ,int);

	if(Noise_Est_Cnt < N_EST_SYMBOLS)
		{  	
    	// OFDM received symbol extraction in &Cmplx_Array[N2]
      	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

		// Received OFDM spectrum in Cmplx_Array
      	fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   

		// Noise Spectral Density
		vmag2_uc_DDP( Fl_Array_R, Fl_Array_R, Fl_Array_I, N );
      	vadd_f_EDE(Noise_Density,Fl_Array_R,Noise_Density,N);

      	Noise_Est_Cnt++; 
      	//if (Noise_Est_Cnt==1)
      	//{
      	//asm("nop;");
      	//asm("nop;");
      	//asm("nop;");
      	//}
		}
	else
		{
		// Mean Noise Spectral Density
        vsmul_f_EE(Noise_Density,1.0f/(float)N_EST_SYMBOLS,Noise_Density,N);
        
		vdivz_f_EEE(&Noise_Density[Min_Index_RX],&Noise_Density[Min_Index_RX],
		           &Abs2_Filter_RX[Min_Index_RX],Max_Index_RX-Min_Index_RX+1);
		
		vset_f_E(Noise_Density,0.0f,Min_Index_RX);
		vset_f_E(&Noise_Density[Max_Index_RX+1],0.0f,N-Max_Index_RX-1);
		
		// Sorted noise density
		vsortii_f_DDDD( Noise_Density_Sort, Noise_Ind, Noise_Density, Noise_Ind, N );

		// Primary Estimation of AWGN Power
		AWGN_Sigma_Est = vsum_f_E(Noise_Density_Sort,
		                           (Max_Index_RX-Min_Index_RX+1))/
		                           (float)(Max_Index_RX-Min_Index_RX+1);

		AWGN_Sigma_Est_RX=sqrt(AWGN_Sigma_Est/2.0f)*Norm_TX;
        		
		Noise_Est_RX++;
		}
	}
