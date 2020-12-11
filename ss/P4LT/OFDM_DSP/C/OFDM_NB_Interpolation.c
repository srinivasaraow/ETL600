/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_NB_Interpolation.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Interpolation of Channel Transfer Function
 *                 if Narrow band interference is detected
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT/OFDM_DSP/C/OFDM_NB_Interpolation.c $
 * 
 * 2     18.07.05 8:46 Chharyt
 * Exchange with TLC. By date 2005-07-15    Version: OFDM_DSP_V0_09U
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       25.03.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_NB_Interpolation()
	{
    	//Input : Cmplx_Array[] , Mask_NB[] , NB_count
		//Output: Cmplx_Array[] 
		
		//uses Fl_Array_R[0] - Fl_Array_R[2*N2]
			

	int nbi_i, nbi_k1, nbi_k2;
	float f_slope , f_intercept;
	float nb_phase_left, nb_phase_right;
	float nb_phase_sum, nb_phase_tmp;
	float nb_phase_re, nb_phase_im;

	
	if (NB_count>0)
		{
	/*********************************************************************
	**     Preparation of Magnitude and Phase							**
	*********************************************************************/
	
		// Split complex Transfer function in Magnitude and Phase Values
		// Fl_Array_R[ 0] : Magnitude Values
		// Fl_Array_R[N2] : Phase Values
		
		// Build Magnitude Values
//		vabs_c_DD(Fl_Array_R,&Cmplx_Array[0],N2);
		vabs_c_DD(Fl_Array_R,&Cmplx_Array[0],LFM_Size);
		
		// Build and unwrap phase
		nb_phase_re = Cmplx_Array[0].re;
		nb_phase_im = Cmplx_Array[0].im;
		nb_phase_left=atan2q(nb_phase_im,nb_phase_re);
//		Fl_Array_R[N2+0]=nb_phase_left; //Start_Value
		Fl_Array_R[LFM_Size+0]=nb_phase_left; //Start_Value
		nb_phase_sum=0.0f;
		
//		for (nbi_i=1; nbi_i<N2; nbi_i++)
		for (nbi_i=1; nbi_i<LFM_Size; nbi_i++)
			{
			nb_phase_re = Cmplx_Array[nbi_i].re;
		    nb_phase_im = Cmplx_Array[nbi_i].im;	
			nb_phase_right=atan2q(nb_phase_im,nb_phase_re);
			nb_phase_tmp = nb_phase_right - nb_phase_left;
			if ( fabsf(nb_phase_tmp) > OFDM_PI ) 
				{
				nb_phase_sum -= OFDM_TWO_PI * fsignf(1.0f , nb_phase_tmp);
				}
//			Fl_Array_R[N2+nbi_i] = nb_phase_sum + nb_phase_right;
			Fl_Array_R[LFM_Size+nbi_i] = nb_phase_sum + nb_phase_right;
			nb_phase_left = nb_phase_right;
			}
			
	/* only for debugging - start */
	//     Combine Magnitude and Phase again
//				for (nbi_i=0; nbi_i<N2; nbi_i++)
				for (nbi_i=0; nbi_i<LFM_Size; nbi_i++)
					{
//					Cmplx_Array[nbi_i].re = Fl_Array_R[nbi_i]*cosf(Fl_Array_R[N2+nbi_i]);
//					Cmplx_Array[nbi_i].im = Fl_Array_R[nbi_i]*sinf(Fl_Array_R[N2+nbi_i]);
					Cmplx_Array[nbi_i].re = Fl_Array_R[nbi_i]*cosf(Fl_Array_R[LFM_Size+nbi_i]);
					Cmplx_Array[nbi_i].im = Fl_Array_R[nbi_i]*sinf(Fl_Array_R[LFM_Size+nbi_i]);
					}
	/* only for debugging - end */	
			
			
	/*********************************************************************
	**     Interpolation of Magnitude and Phase							**
	*********************************************************************/
		pos_ptr=1;
			
		while ( pos_ptr < N )
			{
			// Search for Interpolation Start and Stop-Index
		 	// Increment pos_ptr until next Start-Index
		 	while ( (Mask_NB[pos_ptr] > 0.0f) && (pos_ptr<N)	) {pos_ptr++;}
		 	// Define Start_Index for Interpolation 
//		 	nbi_k1=2*pos_ptr-1;
		 	nbi_k1=LFM_2_N*pos_ptr-1;
		 	// Increment pos_ptr until next Stop-Index
		 	while ( (Mask_NB[pos_ptr] < 1.0f) && (pos_ptr<N)	) {pos_ptr++;}
		 	// Define Stop_Index for Interpolation 
//		 	nbi_k2=2*pos_ptr+1;
		 	nbi_k2=LFM_2_N*pos_ptr+1;
			// Check Values, if OK then do Interpolation
//			if ((nbi_k1<nbi_k2)&&(nbi_k2<N2))
			if ((nbi_k1<nbi_k2)&&(nbi_k2<LFM_Size))
				{
				//***********************************************
				//**      Interpolation of Magnitude
				f_slope = (Fl_Array_R[nbi_k2]-Fl_Array_R[nbi_k1]) / ((float)(nbi_k2-nbi_k1));
				f_intercept = Fl_Array_R[nbi_k1] - f_slope*(float)nbi_k1;
				for (nbi_i=nbi_k1+1; nbi_i<nbi_k2; nbi_i++)
					{
					Fl_Array_R[nbi_i] = f_slope * (float)nbi_i + f_intercept;
					}
				//***********************************************
				//**      Interpolation of Phase
//				f_slope = (Fl_Array_R[N2+nbi_k2]-Fl_Array_R[N2+nbi_k1]) / ((float)(nbi_k2-nbi_k1));
				f_slope = (Fl_Array_R[LFM_Size+nbi_k2]-Fl_Array_R[LFM_Size+nbi_k1]) / ((float)(nbi_k2-nbi_k1));
//				f_intercept = Fl_Array_R[N2+nbi_k1] - f_slope*(float)nbi_k1;
				f_intercept = Fl_Array_R[LFM_Size+nbi_k1] - f_slope*(float)nbi_k1;
				for (nbi_i=nbi_k1+1; nbi_i<nbi_k2; nbi_i++)
					{
//					Fl_Array_R[N2+nbi_i] = f_slope * (float)nbi_i + f_intercept;
					Fl_Array_R[LFM_Size+nbi_i] = f_slope * (float)nbi_i + f_intercept;
					}
				//***********************************************
				//**      Combine Magnitude and Phase
				for (nbi_i=nbi_k1+1; nbi_i<nbi_k2; nbi_i++)
					{
//					Cmplx_Array[nbi_i].re = Fl_Array_R[nbi_i]*cosf(Fl_Array_R[N2+nbi_i]);
//					Cmplx_Array[nbi_i].im = Fl_Array_R[nbi_i]*sinf(Fl_Array_R[N2+nbi_i]);
					Cmplx_Array[nbi_i].re = Fl_Array_R[nbi_i]*cosf(Fl_Array_R[LFM_Size+nbi_i]);
					Cmplx_Array[nbi_i].im = Fl_Array_R[nbi_i]*sinf(Fl_Array_R[LFM_Size+nbi_i]);
					}

				
				}
			}

		}
			

	
	}

