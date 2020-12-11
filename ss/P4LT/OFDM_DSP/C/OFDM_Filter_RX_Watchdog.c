/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Filter_RX_Watchdog.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : "Watchdog" for RX Filter Adaptation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT/OFDM_DSP/C/OFDM_Filter_RX_Watchdog.c $
 * 
 * 2     18.07.05 8:46 Chharyt
 * Exchange with TLC. By date 2005-07-15    Version: OFDM_DSP_V0_09U
 * 
 * 1     15.02.05 8:42 Chalgyg
 * 
 * Initial version
 *       10.02.2005 PL
  *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/util.h"



#define WINDOW_SIZE 1000
float MSE_window_average;
// debug
int segment ( "seg_extn SHT_NOBITS" ) RX_Filter_adapt_improved,RX_Filter_adapt_degraded;

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Filter_RX_Watchdog(float MSE, int INIT)
	{
	static int symbol_counter;
	static float MSE_sum;
	static float MSE_average_last;
	
	if (INIT)
	{
	// Initialization
	symbol_counter = 0;
	MSE_sum=0.0f;
	MSE_average_last=0.0f;
	RX_Filter_adapt_improved=0; // for debug only
	RX_Filter_adapt_degraded=0;	// for debug only
	}
	else
	{
	// Main Operation
	MSE_sum += MSE;
	symbol_counter++;
	// check for window size
	if ((symbol_counter % WINDOW_SIZE) == 0)
	{
	// every WINDOW_SIZE symbols do the following
	symbol_counter=0;
	// calculate Quality criterion
	MSE_window_average = MSE_sum / (float) WINDOW_SIZE;	
	MSE_sum = 0.0f;
	// handle quality result
	if (MSE_window_average < MSE_average_last)
		{
		// quality improved by Filter adaptation
		// store new minimum
		MSE_average_last = 	MSE_window_average;
		// replace Filter Fallback Function
//		vcopy_EE((float *)&C_Filter_RX_fallback[0],(float *)&C_Filter_RX[0],2*N2);
		vcopy_EE((float *)&C_Filter_RX_fallback[0],(float *)&C_Filter_RX[0],2*LFM_Size);
		RX_Filter_adapt_improved++;
		}
		else
		{
		// quality degraded by RX-Filter adaptation
		// replace Filter by stored Fallback-Filter
//		vcopy_EE((float *)&C_Filter_RX[0],(float *)&C_Filter_RX_fallback[0],2*N2);
		vcopy_EE((float *)&C_Filter_RX[0],(float *)&C_Filter_RX_fallback[0],2*LFM_Size);
		RX_Filter_adapt_degraded++;	
		}
	}
	
	
	
	// replace Filter Function if Quality improved
	
		
	}
		
	
	
	}

