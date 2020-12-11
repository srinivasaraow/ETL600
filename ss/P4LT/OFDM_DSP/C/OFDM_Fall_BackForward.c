/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Fall_BackForward.c $
 * COPYRIGHT   : 2005 TL Consulting GmbH
 * DESCRIPTION : Algorithm for FallBack and FallForward
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Fall_BackForward.c $
 * 
 * 7     24.07.08 15:30 Chhamaa
 * 
 * 1     28.01.05 11:51 Ats
 * Exchange TLC (New Files)
 * 
 * Initial version
 *       11.01.2005 PL
  *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//#define FALLFORWARD_SIGMA_LIMIT 15


//int Ltest;
//float FS_score;
//float segment ( "seg_extn SHT_NOBITS" ) K_NL_L[2*OFDM_N_MAX];
typedef struct {
	int   error_cnt;
	int   block_cnt;
	int   Window_Size;
	float prob_limit;
	int   error_cnt_limit;
	float estim_error_prob;
	int   error_flag;
} fallb_error_type;

fallb_error_type Error_fallb;
fallb_error_type Error_abbr;

//----------------------------------------------------------------
//--	OFDM_Fall_BackForward()									--
//--	is estimating the block error probability.				--
//--	If a limit is exceeded an error_flag is set				--
//--    Two cases are treated: 1) Error Burst 2)Fallback		--
//--	Input Field: 	
//--	Input: CRC_Error , delta_sigma_mean						--
//--	Output : ERROR.count, ERROR.number.., 					--
//--			 Error_fallb.estim_error_prob 					--
//----------------------------------------------------------------
void segment ( "seg_ext_pmco" ) OFDM_Fall_BackForward()
	{
	float error_prob_tmp;
// Restart Criterion for Error-Burst
//----------------------------------
		
	Error_abbr.block_cnt++;
	Error_abbr.error_cnt += CRC_Error;
	// Evaluate Window
	if (Error_abbr.block_cnt >= Error_abbr.Window_Size)
		{
		// Check if error limit is exceeded	
		if (Error_abbr.error_cnt >= Error_abbr.error_cnt_limit)
			{
			Error_abbr.error_flag = 1;
			}
		// Reset Window
		Error_abbr.error_cnt = 0;
		Error_abbr.block_cnt = 0;
		}
		
	// Check Error Flag 
	if ( Error_abbr.error_flag == 1)
		{
		// Prepare "Abbruch" Restart
		// (in this version: Restart without memory!)
		// (full list of data rates is available for next start)
		// (AMX500 was with memory. Which scheme is better?)
		Error.Count++;
       	Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       	Error.Number[Error.Count].ID       = ERROR_ABBRUCH;
       		dbg_error[Error.Count]		   = ERROR_ABBRUCH;
			
		}

		
// Restart Criterion for FallBack
//----------------------------------
		
	Error_fallb.block_cnt++;
	Error_fallb.error_cnt += CRC_Error;
	// Evaluate Window
	if (Error_fallb.block_cnt >= Error_fallb.Window_Size)
		{
		// Check if error limit is exceeded	
		if (Error_fallb.error_cnt >= Error_fallb.error_cnt_limit)
			{
			Error_fallb.error_flag = 1;
			}
		
		// Estimate Error Probability
		error_prob_tmp = (float)Error_fallb.error_cnt / (float)Error_fallb.block_cnt;
		Error_fallb.estim_error_prob = Error_fallb.estim_error_prob + 0.3f*
										(error_prob_tmp - Error_fallb.estim_error_prob);
		
		// Reset Window
		Error_fallb.error_cnt = 0;
		Error_fallb.block_cnt = 0;		
		}
	
	// Check Error Flag if FallBackForward is activated
	if ( (FB_FF_Flag == ON) && (Error_fallb.error_flag == 1) )
		{
		// Prepare FallBack Restart
		// (in this version: Restart without memory!)
		// (full list of data rates is available for next start)
		// (AMX500 was with memory. Which scheme is better?)
		Error.Count++;
       	Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       	Error.Number[Error.Count].ID       = ERROR_FALLBACK;
       		dbg_error[Error.Count]		   = ERROR_FALLBACK;
			
		}

// Restart Criterion for FallForward
//----------------------------------
		//if ( (FB_FF_Flag == ON) && (delta_sigma_mean > FALLFORWARD_SIGMA_LIMIT) 
	if ( (FB_FF_Flag == ON) && (delta_sigma_mean > Modem_Config.fallForwardSigmaLimit) 
		  && (n_blk_rx_data>2000) && (Higher_rate_available==1))
		{
		FFwd_Status_local=1;	
		}
		else FFwd_Status_local=0;
	
	if ((FFwd_Status_local==1) && (FFwd_Status_distant==1))
		{
		// Prepare FallForward Restart
		// (in this version: Restart without memory!)
		// (full list of data rates is available for next start)
		// (AMX500 was with memory. Which scheme is better?)
		Error.Count++;
       	Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       	Error.Number[Error.Count].ID       = ERROR_FALLFORWARD;
       		dbg_error[Error.Count]		   = ERROR_FALLFORWARD;
			
		}



	}


void segment ( "seg_ext_pmco" ) OFDM_Fall_BackForward_INIT()
	{
		// Initialisation of of Restart-Criterion for Error-Burst
		//Error_abbr.Window_Size = 31;
		Error_abbr.Window_Size = Modem_Config.burstErrorWindowSize;
		
		//Error_abbr.prob_limit  = 0.8f;
		Error_abbr.prob_limit  = Modem_Config.burstErrorProbLimit;
		
		Error_abbr.error_cnt_limit = (int)(Error_abbr.prob_limit * (float)Error_abbr.Window_Size);
		
		Error_abbr.error_cnt = 0;
		Error_abbr.block_cnt = 0;
		Error_abbr.error_flag= 0;
		
		// Initialisation of Restart-Criterion for Fallback
			//Error_fallb.Window_Size = 1000;
		Error_fallb.Window_Size = Modem_Config.fallBackWindowSize;
		
			//Error_fallb.prob_limit  = 0.1f;
		Error_fallb.prob_limit  = Modem_Config.fallBackProbLimit;
		
		Error_fallb.error_cnt_limit = (int)(Error_fallb.prob_limit * (float)Error_fallb.Window_Size);

		Error_fallb.estim_error_prob = 0.0f;
		
		Error_fallb.error_cnt = 0;
		Error_fallb.block_cnt = 0;
		Error_fallb.error_flag= 0;
		
		// Initialisation of Restart-Criterion for FallForward
		FFwd_Status_local=0;
		FFwd_Status_distant=0;


		
		
		
		

		
	}

