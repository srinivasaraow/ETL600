/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Transmitter.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM Transmitter
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Transmitter.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 2     17.03.06 14:41 Chhamaa
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
#include "../h/util/util.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Transmitter()
	{
	void  Preamble_scheduler_TX();
	void  Preamble_processing_TX();
	void  QAM_scheduler_TX();
	void  QAM_processing_TX();
    void  OFDM_Engine_TX();
    void  OFDM_Filter_TX();
    void  OFDM_SFC_TX();
    void  OFDM_Spectral_Shift_TX();

	// Start of transmitter for SLAVE modem
	// 		"Start_TX" is set in "OFDM_Detection_AGC.c" if 2Tone-Pilot is detected
	Num_Symbol_TX *= Start_TX;     
	
	// TX Preamble scheduler
   	Preamble_scheduler_TX();

   	// TX Preamble processing
    Preamble_processing_TX();
    
    // TX QAM scheduler
    QAM_scheduler_TX();
    
    // TX QAM processing
    QAM_processing_TX();
    
    // TX OFDM engine
    OFDM_Engine_TX();  
     
    //----------------
    
    // TX Filter
	OFDM_Filter_TX(); 

	//delivers: Cmplx_Array[0..Filter_TX_Interpolation*NL]
	
	/*
	// for digital loop with shortcut between TX-Filter Out and RX-Filter In
	if(OFDM_Loop_Mode == DIGITAL_LOOP)	
	{	
	vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],2*NL*Filter_TX_Interpolation);
	}
	else
	*/
		
	// TX Sampling Frequency conversion to 64kHz domain
	// and spectral shift
	OFDM_SFC_TX(); 
	
	
    // TX Symbol number
	Num_Symbol_TX++;     

	if(Num_Symbol_TX > 10000) Num_Symbol_TX--;     
	}


