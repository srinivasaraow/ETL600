/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Sim_Frequency_Offset.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Frequency Offset Simulation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Sim_Frequency_Offset.c $
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
void segment ( "seg_ext_pmco" ) Sim_Frequency_Offset()
	{
	float Exp_Gen(float, complex_float, complex_float *, float, int);

	// Test for frequency offset                      
    vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0],4*NL);

    Start_Phi_T = fmodf(Start_Phi_New_T,OFDM_TWO_PI);
	Start_Phi_New_T = Exp_Gen(Start_Phi_T,Rot_Fr_T,Compl_Exp,Delta_Phi_Symbol_T,2*NL);
	vmul_c_DDD(Cmplx_Array,Cmplx_Array,Compl_Exp,2*NL);

    vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],4*NL);
	}


