/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Receiver.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM Receiver
 *
 * REVISION HISTORY:
 * 		2.4.2004	PL new program structure
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Receiver.c $
 * 
 * 4     5.04.06 15:28 Chhamaa
 * 
 * 4     17.03.06 14:41 Chhamaa
 * 
 * 2     13.09.05 13:23 Ats
 * Exchange from TLC
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 5     2.12.04 17:54 Chhamaa
 * 
 * 4     28.09.04 17:45 Maag01
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
#include "../h/int_dec_filters/INTDEC_externs.h"

extern int EP_ctr;
//========================================
void segment ( "seg_ext_pmco" ) OFDM_Receiver()
	{
    void  OFDM_Filter_RX();
	void  OFDM_Tracking_Engine_RX();
	void  OFDM_FreqCtrl_RX();
	void  Preamble_scheduler_RX();
	void  Preamble_processing_RX();
	void  QAM_scheduler_RX();
	void  QAM_processing_RX();
    void  OFDM_Engine_RX();
    void  OFDM_Spectrum_RX();
    void  OFDM_Spectral_Shift_RX();
	void  OFDM_SFC_RX(); 

	
//PL2106 debug for digital loop
if(OFDM_Loop_Mode == DIGITAL_LOOP)	
	{	
	/*
	// for decimation without filter
	vset_f_E((float *)&OFDM_Symbol_RX[0],0.0f,4*NL);			
    vcopy_s_EE( (float *)&OFDM_Symbol_RX[0], 2, (float *)&OFDM_Symbol_TX[0], 2*(int)SFC_Interpolation, 2*NL );
	vcopy_s_EE( (((float *)&OFDM_Symbol_RX[0])+1), 2, (((float *)&OFDM_Symbol_TX[0])+1), 2*(int)SFC_Interpolation, 2*NL );
	*/
	
	/*
	// for digital shortcut after SFC-Filters (64kHz-domain)
	// for copy of interpolated block to receiver without blocksync
	//	vcopy_EE((float *)&OFDM_Symbol_RX[0],(float *)&OFDM_Symbol_TX[0],2*NL2Q);   			
	*/
	
	// for digital shortcut after SFC-Filters (64kHz-domain)
	// for copy from TX to Rx via Symbol Buf with Symbol synchronization  
	  vcopy_EE((float *)&OFDM_Symbol_RX[0],(float *)&Symbol_Buf[0],2*(NL2Q-Symbol_Synchro_FS));                
      if(Symbol_Synchro_FS > 0)
      	vcopy_EE((float *)&OFDM_Symbol_RX[NL2Q-Symbol_Synchro_FS],(float *)&OFDM_Symbol_TX[0],2*Symbol_Synchro_FS);                

      // Save input OFDM symbol in buffer Symbol_Buf
      vcopy_EE((float *)&Symbol_Buf[0],(float *)&OFDM_Symbol_TX[Symbol_Synchro_FS],2*(NL2Q-Symbol_Synchro_FS));                
   	
	/*
   	// for digital shortcut after TX/RX-Filters (Filter_TX_Interpolation*FN-domain)
	// for copy from TX to Rx via Symbol Buf with Symbol synchronization  
	  vcopy_EE((float *)&OFDM_Symbol_RX[0],(float *)&Symbol_Buf[0],2*(NL*Filter_TX_Interpolation-Symbol_Synchro));                
      if(Symbol_Synchro > 0)
      	vcopy_EE((float *)&OFDM_Symbol_RX[NL*Filter_TX_Interpolation-Symbol_Synchro],(float *)&OFDM_Symbol_TX[0],2*Symbol_Synchro);                

      // Save input OFDM symbol in buffer Symbol_Buf
      vcopy_EE((float *)&Symbol_Buf[0],(float *)&OFDM_Symbol_TX[Symbol_Synchro],2*(NL*Filter_TX_Interpolation-Symbol_Synchro));                
    */  
	}
    
		
    // RX Spectrum Estimation in 64 kHz domain
    // OFDM_Spectrum_RX();
	
    // RX Sampling Frequency conversion, spectral shift and 
	//		frequency offset compensation
	OFDM_SFC_RX(); //to comment, if digital loop with shortcut after TX/RX-Filters
	
	
	if(OFDM_Loop_Mode == DIGITAL_LOOP)	
	{
	// RX Spectrum Estimation in 2*FN domain (input block of 2*NL complex samples)
    OFDM_Spectrum_RX();
	}
	
	// overlap/save filtering, freq. compensation, extraction
    OFDM_Filter_RX();

	// Estimation of Clock and Frequency Offset during Data transmission,
    // Clock Control
    OFDM_Tracking_Engine_RX();
    
    // Compensation of residual Frequency Offset
    OFDM_FreqCtrl_RX();

	
	// Preambel scheduler
    Preamble_scheduler_RX();
    
    // RX preamble processing
    Preamble_processing_RX();
    
    // RX QAM scheduler
    QAM_scheduler_RX();
    
    // RX OFDM engine
    OFDM_Engine_RX();  
     
    // RX QAM processing
    QAM_processing_RX();

       	
    }


