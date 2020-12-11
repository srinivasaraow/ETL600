/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Channel_Simulator.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM Channel Simulator
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_Channel_Simulator.c $
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
void segment ( "seg_ext_pmco" ) OFDM_Channel_Simulator()
	{
	void  Sim_Frequency_Offset();
	void  Sim_NL_Lin_Dist();
	void  Sim_AWGN();
	void  Sim_NB();
	
	if(Sim_Attenuation_Flag == ON)
		{
		vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0],4*NL);
		vsmul_f_DD((float *)&Cmplx_Array[0],a_loss_base,(float *)&Cmplx_Array[0],4*NL);  
        vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],4*NL); 	  
		}

	if((Sim_Lin_Dist_Flag == ON) || (Sim_NL_Dist_Flag == ON))
		{	
		// Test for linear distortion                      
		Sim_NL_Lin_Dist();
		}

	if(Sim_Frequency_Offset_Flag == ON)
		{	
		// Test for frequency offset                      
		Sim_Frequency_Offset();
		}

	if(Sim_AWGN_Flag == ON)
		{	
		// Test for AWGN                      
		Sim_AWGN();
		}

	if(Sim_NB_Flag == ON)
		{	
		// Test for Narrow Band Interference                      
		Sim_NB();
		}
	}


