/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Pilots_Preprocessing.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Preprocessing of pilots for frequency tracking and level error estimation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Pilots_Preprocessing.c $
 * 
 * 10    24.07.08 15:33 Chhamaa
 * 
 * 5     5.04.06 15:28 Chhamaa
 * 
 * 5     17.03.06 14:41 Chhamaa
 * 
 * 3     18.07.05 8:46 Chharyt
 * Exchange with TLC. By date 2005-07-15    Version: OFDM_DSP_V0_09U
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       07.12.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"

//========================================
void Pilots_Preprocessing()
	{
	// Input Field is Cmplx_Array[N2:N2+N-1]

	float pil0r, pil0i, filt0r, filt0i;
    float pil1r, pil1i, filt1r, filt1i;
    float A2filt0, A2filt1;
    float A2_Pil0, A2_Pil1;
	float xxx0, yyy0;
	float xxx1, yyy1;
    
   	// Extract pilot tones
    Pilot0_RX.re = Cmplx_Array[N2+Pilot_ind0].re;
    Pilot0_RX.im = Cmplx_Array[N2+Pilot_ind0].im;
    Pilot1_RX.re = Cmplx_Array[N2+Pilot_ind1].re;	// Reference Pilot
    Pilot1_RX.im = Cmplx_Array[N2+Pilot_ind1].im;	// Reference Pilot

	// Extraction of pilot tones
	pil0r = Pilot0_RX.re;
	pil0i = Pilot0_RX.im;

	pil1r = Pilot1_RX.re;
	pil1i = Pilot1_RX.im;
		
	// Extraction of RX filter complex conjugated coefficients
	// on pilot tones
	filt0r = C_Filter_RX[LFM_2_N*Pilot_ind0].re;
	filt0i = -C_Filter_RX[LFM_2_N*Pilot_ind0].im;

	filt1r = C_Filter_RX[LFM_2_N*Pilot_ind1].re;
	filt1i = -C_Filter_RX[LFM_2_N*Pilot_ind1].im;

	A2filt0 = filt0r*filt0r + filt0i*filt0i;
	A2filt1 = filt1r*filt1r + filt1i*filt1i;
		 
	// Multiplication of 1st pilot with complex conjugated
	// coefficient of RX filter 
	xxx0 = pil0r*filt0r - pil0i*filt0i; 
	yyy0 = pil0r*filt0i + pil0i*filt0r; 

	xxx0 /= A2filt0;
	yyy0 /= A2filt0;
		
	// Energy of 1st pilot
	A2_Pil0 = xxx0*xxx0 + yyy0*yyy0;
		
	// Multiplication of 2nd pilot with complex conjugated
	// coefficient of RX filter 
	xxx1 = pil1r*filt1r - pil1i*filt1i; 
	yyy1 = pil1r*filt1i + pil1i*filt1r; 

	xxx1 /= A2filt1;
	yyy1 /= A2filt1;
		
	// Energy of 2nd pilot
	A2_Pil1 = xxx1*xxx1 + yyy1*yyy1;

	// Summary energy of pilots
	En_Pil = A2_Pil0 + A2_Pil1;
	
	Level = En_Pil/Ref_Level;
	
	if(Level > 4.0f)
		{
		Level_Error_cnt += 1;
		Level_Warning = 1;
		}
	else
		{
		if(Level < 0.01f)
			{
			Level_Error_cnt += 1;
			Level_Warning = 1;
			}
		else
			{
			Level_Error_cnt = 0;
			Level_Warning = 0;
			}
		}
		
	// Stop and Restart Linear Regression, if Pilot Warning
	if (Level_Warning == 1)
			{
			Df_block_cnt=0;
		 	Start_Ph_Pil0 = 0.0f;
			Start_Ph_Pil1 = 0.0f;
			Df_valid_result = 0; 
			}

	// Restart if level error burst >= MAX_LEVEL_ERROR_BURST
	if(Level_Error_cnt > Level_Error_TimeOut)
   		{
   		Error.Count++;
       	Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       	Error.Number[Error.Count].ID       = ERROR_LEVEL;
	   	//OFDM_Restart(); 
		}

	if(Df_block_cnt < LEN_DF_EST_DATA) 
		{
		// Phase of 1st pilot
//   		Ph_Pil_t0[Df_block_cnt] = atan2q(yyy0,xxx0);
   		Ph_Pil_t0[Df_block_cnt] = atan2(yyy0,xxx0);
   		
		// Phase of 2nd pilot
//   		Ph_Pil_t1[Df_block_cnt] = atan2q(yyy1,xxx1);  	// Reference Pilot
   		Ph_Pil_t1[Df_block_cnt] = atan2(yyy1,xxx1);  	// Reference Pilot
		
   		Df_block_cnt++;
		}

	}


