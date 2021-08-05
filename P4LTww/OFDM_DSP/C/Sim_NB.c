/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Sim_NB.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Narrow Band Interference Simulation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Sim_NB.c $
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
void segment ( "seg_ext_pmco" ) Sim_NB()
	{
	float Exp_Gen(float, complex_float, complex_float *, float, int);

    vcopy_DE((float *)&Cmplx_Array[0],(float *)&OFDM_Symbol_TX[0],4*NL); 	  

	// First NB interferer                      
	Start_Phi_NB0 = fmodf(Start_Phi_New_NB0,OFDM_TWO_PI);
	Start_Phi_New_NB0 = Exp_Gen(Start_Phi_NB0,Rot_NB0,Compl_Exp,Delta_Phi_NB0,2*NL);
    vsmul_f_DD((float *)&Compl_Exp[0],a_NB0,(float *)&Compl_Exp[0],4*NL);   
    vadd_f_DDD((float *)&Cmplx_Array[0],(float *)&Compl_Exp[0],(float *)&Cmplx_Array[0],4*NL);

	// Secund NB interferer                      
	Start_Phi_NB1 = fmodf(Start_Phi_New_NB1,OFDM_TWO_PI);
	Start_Phi_New_NB1 = Exp_Gen(Start_Phi_NB1,Rot_NB1,Compl_Exp,Delta_Phi_NB1,2*NL);
	vsmul_f_DD((float *)&Compl_Exp[0],a_NB1,(float *)&Compl_Exp[0],4*NL);   
    vadd_f_DDD((float *)&Cmplx_Array[0],(float *)&Compl_Exp[0],(float *)&Cmplx_Array[0],4*NL);

    vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[0],4*NL); 	  

    }


