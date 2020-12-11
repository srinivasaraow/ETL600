/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Preamble_processing_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : RX Preamble Processing
 *
 * REVISION HISTORY:
 *		2.4.2004 PL new structure
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Preamble_processing_RX.c $
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <trans.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"

#include "../h/tcm/TCM_Codec_externs.h"

//========================================
void segment ( "seg_ext_pmco" ) Preamble_processing_RX()
	{
	void OFDM_Detection_Pilot_Mode();
	void OFDM_Detection_AGC();
	void OFDM_Freq_Estim_RX();
	void OFDM_Noise_Estimation();
	void OFDM_LFM_Processing_RX();
	void OFDM_QAM4_Detection();
	void OFDM_Restart();
		
	switch ( RX_Preamble_State )
    	{   
    		
    	case RX_DETECTION_MODE:
    	   
    	// detection of NL_Optimization Starttyp by different Pilot schemes
    	OFDM_Detection_Pilot_Mode();
    	
    	break;
       	
    	case RX_DETECTION_AGC:
		
       	// 2 tone signal detection and primary analog and digital AGC setting	
       	OFDM_Detection_AGC();
		
		break;
      	
        case RX_NOISE_EST:
    	
      	// Esimation of spectral density and power of noise
		OFDM_Noise_Estimation();

		break;
		
		case RX_FREQUENCY_EST:
		
        // Primary frequency and clock offset canceller
        OFDM_Freq_Estim_RX();

		break;
      	
		case RX_LFM_PROC:

      	// LFM Processing 
      	
      	OFDM_LFM_Processing_RX(); //PT=5

		break;
		
      	case RX_QAM4_DETECT:
      	
      	// QAM4 Detection
		OFDM_QAM4_Detection();
		
		break;
      	
      	case RX_Preamble_IDLE:
      		// no operation
      	break;
		
		default:      	  

      	OFDM_Restart();

      	break;
      	}
	}


