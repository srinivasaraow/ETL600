/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Tracking_Engine_RX.c $
 * COPYRIGHT   : 2005 TL Consulting GmbH
 * DESCRIPTION : Main module for Frequency and clock offsets tracking
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Tracking_Engine_RX.c $
 * 
 * 1     5.04.06 15:34 Chhamaa
 * 
 * 2     17.03.06 14:41 Chhamaa
 * 
 * 
 * Initial version
 *       08.11.05 PL
 *****************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
//#include "../h/util/util.h"

    

//========================================
void OFDM_Tracking_Engine_RX()
	{
	void  Fr_Tracking();
    void  Pilots_Preprocessing();
	void  fft(int, float *, float pm *, complex_float *);


	if(Start_RX_QAM_Engine == 1)
		{

		//%============== Perpare Signal to Extract Pilots ===============
    		
    	//% OFDM received symbol extraction in &Cmplx_Array[N2]
        //M:RX_Signal_Engine(1:N) = RX_Signal_Filter_out(L+1:NL);
		vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

		//% Received OFDM spectrum in Cmplx_Array
        //M:RX_Signal_Engine(1:N) = 1/sqrt(N)*fft(RX_Signal_Engine(1:N),N);
        fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   
      	vcombine_c_DDP(&Cmplx_Array[N2],Fl_Array_R,Fl_Array_I,N); 	  

        
		//%============== Process Pilots and tracking  ===============

        //% Pilot tones preprocessing and level error estimation
        //	operates on Cmplx_Array[N2:N2+N-1]
		Pilots_Preprocessing(); 
      	
     	//% Frequency and clock offset tracking
      	Fr_Tracking();
     	
		} //  if(Start_RX_QAM_Engine == 1)
 }


