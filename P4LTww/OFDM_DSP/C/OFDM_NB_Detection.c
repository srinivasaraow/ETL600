/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_NB_Detection.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Narrow band interference detection
 *
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_NB_Detection.c $ 
 * 
 * 2     26.04.05 8:43 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * REVISION HISTORY:
 *		24.3.2004 PL body implementation
 *      24.3.2004 PL trial of alternative algorithm
 *      22.09.04 Noise Threshold
 *-----------------------------------------------------------------------------
 * 
 * Initial version
 *       15.02.2004 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_NB_Detection()
	{
		
	// Input : Noise_Density_Sort[] , Noise_Ind[] , M0_RX , Norm_TX
	// Output: AWGN_Sigma3_Est , Inf_Array_NB[] , NB_count , Mask_NB[]

	// Parameters: NB_WIDTH_THRESHOLD , NB_FS_THRESHOLD
	
	
	int n_sum_start;
	int temp_int;
	int temp_flag;
	
	
	/*****************************************************************************
	**     Preparation of Noise Estimation (Scaling, Smoothing, Derivation)		**
	*****************************************************************************/
			//Input : Noise_Density_Sort[] , Noise_Ind
			//Output: nb_span_noise
			
			// Note: this part is always executed (independent of NB-Detection Result)
			
			
	nb_span_noise=0;
	
	if (NB_Detection_Flag == ON)
		{	
		// Copy Noise_Density_Sort to internal RAM		
    	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Noise_Density_Sort[0],N);
		
    	// find the middle of activ Noise values in this sorted field
		nb_i     = (Max_Index_RX-Min_Index_RX+1)/2;
		nb_scale =  1.0f/Noise_Density_Sort[nb_i];

		// and scale to this value
		vsmul_f_DD(&Fl_Array_R[0],nb_scale,&Fl_Array_R[0],N);
		
		// smoothing and Derivation
		Value_left =0.;
		Value_right=0.;

		for(nb_i=0;nb_i<M0_RX;nb_i++)
			{
			pos_ptr=M0_RX-nb_i;
			Value_right=vsum_f_E((float *)&Fl_Array_R[nb_i],pos_ptr);
			Value_right=Value_right/(float)pos_ptr;

			if (nb_i>0)
				{
				Norm_spec_NB[nb_i-1]=Value_right-Value_left;
				}
			Value_left=Value_right;
			}


		
		// Determination of NB-Span	to generate AWGN-Estimation
		while ((Norm_spec_NB[nb_span_noise] < NB_WIDTH_THRESHOLD) && (nb_span_noise < M0_RX))
			{
			nb_span_noise++;
			}			
		}
		
	/*****************************************************************************
	**     Noise Estimation (with respect to nb_span_noise)             		**
	*****************************************************************************/
			//Input : nb_span_noise, Noise_Density_Sort[] , M0_RX, Norm_TX
			//Output: AWGN_Sigma3_Est, NB_FS_Energy_tot
			
			// Note: this part is always executed (independent of NB-Detection Result)
			
		// Calculate Start_Index and Length of Estimation Window
		n_sum_start = nb_span_noise+4 - 1 ;     // Start_Index
		pos_ptr     = M0_RX - nb_span_noise -5; // Length
		
		// Check Length of Window 
		if (pos_ptr < 1)
			{
			n_sum_start=3;
			pos_ptr=M0_RX-5;
			// Generate Warning Message	
			}
		
		// Copy Noise_Density_Sort to internal RAM		
    	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Noise_Density_Sort[0],N);
		
		// Summation of Values inside of Estimation Window [Reference Value]
		Value_right=vsum_f_E((float *)&Fl_Array_R[n_sum_start],pos_ptr);
		Value_right=Value_right / (float)(pos_ptr);
		NB_FS_Energy_tot = Value_right; // Reference Value for FS
		
		// Secondary AWGN Estimation 
		AWGN_Sigma3_Est=sqrt(Value_right/2.0f)*Norm_TX;

				
	/*****************************************************************************
	**                  Fisher Statistics with Moving Window	[2. Variant]	**
	*****************************************************************************/
			// Input : Noise_Density_Sort[] , Noise_Ind[] , NB_FS_Energy_tot
			// Output: F_Stat_NB[] , Inf_Array_NB[] , NB_count , Mask_NB[]
			
	// Init Variables and Fields		
	NB_count=0; nb_width=0; nb_flag=0;
	vset_f_E( Mask_NB  ,1.0,N);
	vset_f_E( F_Stat_NB,0.0,N);
	
	// Copy Noise_Density_Sort to internal RAM		
   	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Noise_Density_Sort[0],N);

	
	// Note: Normalized Reference Energy is stored in NB_FS_Energy_tot
                      
	// Sliding F-Statistic loop
    for (nb_i=0;nb_i<N;nb_i++)
		{
		F_Stat_NB[nb_i] = Fl_Array_R[nb_i] / NB_FS_Energy_tot;
		
		if(NB_Detection_Flag == ON)
		{
		// Check FS Threshold
		if (F_Stat_NB[nb_i] > NB_FS_THRESHOLD ) // && &Fl_Array_R[i] > NB_SPEC_THRESHOLD
			{
			// Get Index of Interferer
			nb_flag=1;
			pos_ptr = Noise_Ind[nb_i];
			// Check absolute value of NB Interferer
			if (Noise_Density[pos_ptr] > NB_NOISE_THRESHOLD)
				{
				NB_count++;
				Mask_NB[pos_ptr]=0.0f;
				Inf_Array_NB[NB_count-1].Start		 = pos_ptr;
				Inf_Array_NB[NB_count-1].F_Stat_Max	 = F_Stat_NB[nb_i];
				}
			}
		else
			{
			nb_flag=0;	
			}
		} // NB_Detection_Flag

		} // For

	// Check if number of blocked channels is odd or even
	// if it is odd than block a neighbour channel to get an even number of
	// modulated channels
	if (NB_count>0)
		{
		// Check if number of blocked channels is odd	
		if (NB_count & 1 == 1)
			{
/* Version 1
			n_sum_start = Inf_Array_NB[0].Start + 1;
			// Check if righthand neighbour of first NB is inside active spectrum	
			if (n_sum_start <= Max_Index_RX)
				// Block righthand neighbour
				Mask_NB[n_sum_start]=0.0f; 
			else // Block lefthand neighbour 
				Mask_NB[n_sum_start-2] = 0.0f;
			// now number of blocked channels should be even
			NB_count++;		
*/
// Version 2
			// NB_count-1 is position in sorted Noise_spectrum of last blocked channel
			// Get Index of next channel in sorted Noise_spectrum 
			pos_ptr = Noise_Ind[NB_count];
			// Check if this channel is a neighbour to a blocked channel
			// if so, than block channel
			if ((Mask_NB[pos_ptr-1]==0.0f) || (Mask_NB[pos_ptr+1]==0.0f))
				Mask_NB[pos_ptr]=0.0f;
			else
			// if not, try to block a direct neighbour of the strongest NB interfer
			// To do so, we have to search first righthand neighbour, first lefthand neighbour
			// second righthand neighbour, second lefthand neighbour...
			// until we found for the first time a subchannel that isn't already blocked
				{
				temp_flag = 0; //Flag to exit while loop	
				temp_int = 1;  //Number of neighbour	
				while (temp_flag==0)
					{	
					// Try to block righthand neighbour
					// Check if righthand neighbour of first NB is inside active spectrum
					// and is not already blocked
					if (temp_flag == 0)
					{
					n_sum_start = Inf_Array_NB[0].Start + temp_int; //Index of righthand neighbour
					if ((n_sum_start <= Max_Index_RX)&&(Mask_NB[n_sum_start]!=0.0f))
						{
						Mask_NB[n_sum_start]=0.0f; // block channel
						temp_flag = 1;			   // flag for success
						NB_count++;	
						}
					}
					asm("nop;");
					asm("nop;");
					// Try to block lefthand neighbour
					if (temp_flag == 0)
					{
					n_sum_start = Inf_Array_NB[0].Start - temp_int; //Index of lefthand neighbour
					if ((n_sum_start >= Min_Index_RX)&&(Mask_NB[n_sum_start]!=0.0f))
						{
						Mask_NB[n_sum_start]=0.0f; 
						temp_flag = 1;
						NB_count++;	
						}
					}
					asm("nop;");
					asm("nop;");
					// If no sucess, go to next neighbour pair
					if (temp_flag == 0) temp_int++;
					// Try to catch error: If there is no sucess for half of spectrum
					// then for gods sake get out of here
					if (temp_int > N/2) temp_flag=1;
					}
				}// else
				
											
			}
		}
	
	}

