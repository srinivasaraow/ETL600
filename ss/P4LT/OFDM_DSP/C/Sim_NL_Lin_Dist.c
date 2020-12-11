/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Sim_NL_Lin_Dist.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Nonlinear and Linear Distortion Simulation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Sim_NL_Lin_Dist.c $
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
void segment ( "seg_ext_pmco" ) Sim_NL_Lin_Dist()
	{
    void  fft(int, float *, float pm *, complex_float *);
	void  ifft(int, float *, float pm *, complex_float *);
    
	if(Sim_Lin_Dist_Flag == ON) 
		{	
		// Input Overlap/Save 
    	vcopy_DE((float *)&Cmplx_Array[0],(float *)&Sim_Overlap_Save[0],4*N_L);
    	vcopy_DE((float *)&Cmplx_Array[2*N_L],(float *)&OFDM_Symbol_TX[0],4*NL);
    	vcopy_EE((float *)&Sim_Overlap_Save[0],(float *)&OFDM_Symbol_TX[4*L],4*N_L);
      
    	// Input FFT 
    	fft(N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
    	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N4); 
 
    	// Scaling 
    	vsmul_f_DD((float *)&Cmplx_Array[0],1.0f/(float)N4,(float *)&Cmplx_Array[0],8*N);   

    	// Linear Distortion
    	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Sim_Tr_Funct[0],2*N2); 	  
    	vmul_c_DDD(Cmplx_Array,Cmplx_Array,(complex_float *)&Fl_Array_R[0],N2);
    	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Sim_Tr_Funct[N2],2*N2); 	  
    	vmul_c_DDD(&Cmplx_Array[N2],&Cmplx_Array[N2],(complex_float *)&Fl_Array_R[0],N2);

    	// Output IFFT 
    	ifft(N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
    	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N4); 
 
		// Output Overlap/Save 
    	vcopy_ED((float *)&OFDM_Symbol_TX[0],(float *)&Cmplx_Array[2*N_L],4*NL);
		}
    }


