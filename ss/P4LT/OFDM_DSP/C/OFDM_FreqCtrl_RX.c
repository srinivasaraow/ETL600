/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_FreqCtrl_RX.c $
 * COPYRIGHT   : 2005 TL Consulting GmbH
 * DESCRIPTION : Frequency offset correction in FN domain
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_FreqCtrl_RX.c $
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
#include "../h/util/util.h"

    

//========================================
void OFDM_FreqCtrl_RX()
{
	//% Input: Cmplx_Array[0:N+L-1],	signal
	//%       Frequency_Control_enable, activation flag
	//%       Freq_Offset,				Estimation of Frequency Offset
	//%       Start_Phi_New_FC,				Start phase for Exp_Gen

	float Exp_Gen(float, complex_float, complex_float *, float, int);
		
	float tmp_rot; //for rotation coefficient
	

	if (Frequency_Control_enable == ON)
	{
		// Frequency Offset to compensate
		tmp_rot = Freq_Offset;
		// Rotation coefficient
		Rot_Fr.re = cosf(tmp_rot);
		Rot_Fr.im = -sinf(tmp_rot);
		// Phase increment for one OFDM symbol
		Delta_Phi_Symbol = -(tmp_rot)*(float)NL;
		
		// Frequency offset compensation (in place)
	   	Start_Phi = fmodf(Start_Phi_New_FC,OFDM_TWO_PI);
	    Start_Phi_New_FC = Exp_Gen(Start_Phi,Rot_Fr,Compl_Exp,Delta_Phi_Symbol,NL);
	    vmul_c_DDD(Cmplx_Array,Cmplx_Array,Compl_Exp,NL);
	    
	    // Store Rotation coefficients for RX-Filter-Adaptation
	    vconjg_c_DD(&Compl_Exp[NL],Compl_Exp,NL); 
	    vcopy_ED((float *)&Complex_Exp_T,(float *)&Compl_Exp[NL],2*NL);

	}
	//% Output: Cmplx_Array[0:N+L-1]

}
