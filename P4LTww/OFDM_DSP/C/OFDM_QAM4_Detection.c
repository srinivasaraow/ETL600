/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_QAM4_Detection.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Preliminary optimization
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_QAM4_Detection.c $
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_QAM4_Detection()
	{
	void OFDM_2Tone_Correlation(int, int *);


	switch ( QAM4_Detect_RX )
   		{
	   	case 0:
		
   		// Detection of 2 tone signal
		OFDM_2Tone_Correlation(POS,&QAM4_Detect_RX);

    	break;

    	case 1:
		
      	// Detection of QAM signal
		OFDM_2Tone_Correlation(NEG,&QAM4_Detect_RX);
		
	   	break;
    	
    	case 2:
    	
    	// Reset inner State machine	
        QAM4_Detect_RX=0;
        
        // Set Parent State Machine Flag
		rx_state_finished = 1;
		
   		break;
		}
	}

