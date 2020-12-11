/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_QAM4_Data_Preparation.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Preparation of Channel Data for Transmission
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_QAM4_Data_Preparation.c $
 * 
 * 9     24.07.08 15:32 Chhamaa
 * 
 * 3     13.09.05 13:23 Ats
 * Exchange from TLC
 * 
 * 2     26.04.05 8:44 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       30.03.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/qam4_fec/c3212.h"

#define POWER_K_WORD1 ((1<<K_WORD_L)-1)
//========================================
void segment ( "seg_ext_pmco" ) OFDM_QAM4_Data_Preparation()
	{
		//register int i;
		//register unsigned int *i_ptr;
		 float           vmax,vmax2;
		 int i;
		//!! Changed Algorithm: At least one pilot must be unaffected
		//On other side modem transmitter must choose this pilot for scaling of
		//Predistortion
		
/*		// !! Old Algorithm :Protect Pilot positions
		// If NB interferer is detected on Pilot position
		// remove it from Mask_NB, because value of Pilot transfer function is
		// required in Optim1 to calculate Predistortion
		if (Mask_NB[Pilot_ind0] == 0.0f)
			{ // Warning: detected Interferer on Pilot
			  Mask_NB[Pilot_ind0] = 1.0f;
			  Error.Count++;
       	      Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       	      Error.Number[Error.Count].ID       = WRNG_NB_ON_PILOT;
			}
			
		if (Mask_NB[Pilot_ind1] == 0.0f)
			{ // Warning: detected Interferer on Pilot
			  Mask_NB[Pilot_ind1] = 1.0f;
			  Error.Count++;
       	      Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       	      Error.Number[Error.Count].ID       = WRNG_NB_ON_PILOT;
			}	  
*/			  
		// Integration of Narrow-Band Interferer Mask "Mask_NB"
		vcopy_DE((float *)&Fl_Array_R[N],(float *)&Mask_NB[0],N);
		vcopy_DE(Fl_Array_R,K_RX,N);
		vmul_f_DDD(Fl_Array_R,Fl_Array_R,(float *)&Fl_Array_R[N],N);
					
    	// Set borders to zero
		vset_f_E(Fl_Array_R,0.0f,Min_Index_RX);
		vset_f_E(&Fl_Array_R[Max_Index_RX+1],0.0f,N-Max_Index_RX-1);
		
		// Restore Pilot positions
		Fl_Array_R[Pilot_ind0] = K_RX[Pilot_ind0];
		Fl_Array_R[Pilot_ind1] = K_RX[Pilot_ind1];
		
 		
		// scale characteristic to K_WORD_L bits
        K_max_RX = vmax_f_D(Fl_Array_R, N);
        vmax2 = (float)POWER_K_WORD1/K_max_RX;
        vsmul_f_DD(Fl_Array_R,vmax2,Fl_Array_R,N);

				
		// Quantization to K_WORD_L bits 
		vfix_DD((int *)&Fl_Array_R[0],Fl_Array_R,0,N);
		
		
		
		// Write quantized values to K_Int_RX
		vcopy_ED((float *)&K_Int_RX[0],Fl_Array_R,N);
		// Write quantized values to Channel_Data filed for transmission
		vcopy_ED((float *)&Channel_Data_RX.K_int[0],Fl_Array_R,N);

		// Write quantized values back to local Channel_Data
		// !! maybe not required, if Optimization uses only struct
		// !! Channel_Data_RX and Channel_Data_TX 
		vfloat_DD(Fl_Array_R,(int *)&Fl_Array_R[0],-K_WORD_L,N);
		vsmul_f_DD(Fl_Array_R,K_max_RX,Fl_Array_R,N);
		vcopy_ED(K_RX,Fl_Array_R,N);
	
		// Prepare Struct Channel_Data_RX  
		Channel_Data_RX.AWGN_sigma = AWGN_Sigma3_Est;  // 32 bit Noise Power
		Channel_Data_RX.K_scale    = K_max_RX;    // 32 bit Scaling of channel transfer function
		
		// Prepare Struct Channel_Data_RX   Settings  !! 12 Bit Size
		   vset_i_E(Channel_Data_RX.Settings, 0, CHD_RES);
		   
		   Channel_Data_RX.Settings[0] =      1;            // Operating Mode   TBD
	       Channel_Data_RX.Settings[1] = OFDM_Modem_Type;   // Master_Slave
	       Channel_Data_RX.Settings[2] =      0;            // N-L-Optim-Mode   TBD
	       Channel_Data_RX.Settings[3] =      N; // N
	       Channel_Data_RX.Settings[4] =      L; // L
	    //   Channel_Data_RX.Settings[5] =   N_selected_RX & 0x0FFF; 	// 
	    //   Channel_Data_RX.Settings[6] =   L_selected_RX & 0x0FFF;	// 
	    // Channel_Data_RX.Settings[7] = 				    // List of Data Rates
	    // Channel_Data_RX.Settings[8] = 				    // List of Data Rates
	    // Channel_Data_RX.Settings[9] =  				    //    .....
	    
	    // Apply 12bit Mask to Channel_Data_XX.Settings
	    for (i=0; i<CHD_RES; i++) Channel_Data_RX.Settings[i] &= C3212_MASK;
	    
	    
	    
		
	}

