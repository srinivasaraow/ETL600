/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Sim_AWGN.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : AWGN Simulation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Sim_AWGN.c $
 * 
 * 1     19.01.05 15:41 Chchlee
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
void segment ( "seg_ext_pmco" ) Sim_AWGN()
	{
	
	// AWGN generation in Cmplx_Array
	vgauss_f_D((float *)&Cmplx_Array[0],0.0f,AWGN_sigma,4*NL);
	vcopy_DE(Fl_Array_R,(float *)&OFDM_Symbol_TX[0],4*NL);	
	vadd_f_DDD(Fl_Array_R,Fl_Array_R,(float *)&Cmplx_Array[0],4*NL);	
	vcopy_ED((float *)&OFDM_Symbol_TX[0],Fl_Array_R,4*NL);	
	
    AWGN_Energy = vsummag2_c_D(Cmplx_Array,2*NL);
	AWGN_Power=AWGN_Energy/(float)(2*NL); 
	AWGN_Power_Mean=AWGN_Power_Mean + 0.01f*(AWGN_Power-AWGN_Power_Mean);
	}


