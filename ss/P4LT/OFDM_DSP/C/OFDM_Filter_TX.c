/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Filter_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : TX Filter
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Filter_TX.c $
 * 
 * 9     24.07.08 15:30 Chhamaa
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
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * TX filter using SDRAM function
 *       02.03.04 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Filter_TX()
{
	void fft(int, float *, float pm *, complex_float *);
	void ifft(int, float *, float pm *, complex_float *);
 	
	float ph_rt;

	// Input in &Cmplx_Array[N2+N-L],

	// Overlap - Save procedure: input in &Cmplx_Array[N2+N-L],
	// 	output in &Cmplx_Array[N2]
    vcopy_DE((float *)&Cmplx_Array[N2],(float *)&Overlap_Save_TX[0],2*N_L);
    vcopy_ED((float *)&Overlap_Save_TX[0],(float *)&Cmplx_Array[N2+NL],2*N_L);

    // TX scaling
    vsmul_f_DD((float *)&Cmplx_Array[N2],Output_Scale_TX,(float *)&Cmplx_Array[N2],N4);   

    // Input FFT-2N
    fft(N2,Fl_Array_R,Fl_Array_I,&Cmplx_Array[N2]);   
    vcombine_c_DDP(&Cmplx_Array[N],Fl_Array_R,Fl_Array_I,N2);      
	// result in Cmplx_Array[N..3N-1]
    
    switch (Filter_TX_Interpolation)
	{
	case 2:
    // filter TX and interpolation by 2
    vmul_c_DDE(Cmplx_Array,&Cmplx_Array[N2],&Filter_TX[N],N);
    vmul_c_DDE(&Cmplx_Array[3*N],&Cmplx_Array[N],Filter_TX,N);

    if (L==7) //phase rotation e.g. for L=7, tx_filt_phaserot.re=-1
    {
    	
    vsmul_c_DD( Cmplx_Array     ,tx_filt_phase, Cmplx_Array     ,N);
    vsmul_c_DD(&Cmplx_Array[3*N],tx_filt_phase,&Cmplx_Array[3*N],N);
    
    //tx_filt_phase.re = tx_filt_phase.re * tx_filt_phaserot.re;
		// tx_filt_phase.re = -1.0f * tx_filt_phase.re; //switches from -1, 1 ,-1 ...
    //Phase rotation is simple in our special case, but here is the usual procedure
	ph_rt           =tx_filt_phase.re*tx_filt_phaserot.re - tx_filt_phase.im*tx_filt_phaserot.im;
	tx_filt_phase.im=tx_filt_phase.re*tx_filt_phaserot.im + tx_filt_phase.im*tx_filt_phaserot.re;
	tx_filt_phase.re=ph_rt;
	//-------------------------- tx_filt_phase Regeneration ----------------------

	ph_rt=atan2q(tx_filt_phase.im,tx_filt_phase.re+0.000000001f);
	tx_filt_phase.re=cosf(ph_rt);
	tx_filt_phase.im=sinf(ph_rt);
	//---------------------*/
    }
    
    // Zero filling and output IFFT-4N
    vset_f_D((float *)&Cmplx_Array[N],0.0f,N4);
	ifft(N4,Fl_Array_R,Fl_Array_I,Cmplx_Array);  
              
    // Extract OFDM TX symbol   
   	vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[2*N_L],&Fl_Array_I[2*N_L],2*NL);
   
	break;
	
	case 4:
	// filter TX and interpolation by 4
    vmul_c_DDE(Cmplx_Array,&Cmplx_Array[N2],&Filter_TX[N],N);
    vmul_c_DDE(&Cmplx_Array[7*N],&Cmplx_Array[N],Filter_TX,N);
    
    if (L==7) //phase rotation e.g. for N=128 L=7 with FN=9600
    {
    	
    vsmul_c_DD( Cmplx_Array     ,tx_filt_phase, Cmplx_Array     ,N);
    vsmul_c_DD(&Cmplx_Array[7*N],tx_filt_phase,&Cmplx_Array[7*N],N);
    
    //tx_filt_phase.re = tx_filt_phase.re * tx_filt_phaserot.re;
		// tx_filt_phase.re = -1.0f * tx_filt_phase.re; //switches from -1, 1 ,-1 ...
    //Phase rotation is simple in our special case, but here is the usual procedure
	ph_rt           =tx_filt_phase.re*tx_filt_phaserot.re - tx_filt_phase.im*tx_filt_phaserot.im;
	tx_filt_phase.im=tx_filt_phase.re*tx_filt_phaserot.im + tx_filt_phase.im*tx_filt_phaserot.re;
	tx_filt_phase.re=ph_rt;
	//-------------------------- tx_filt_phase Regeneration ----------------------

	ph_rt=atan2q(tx_filt_phase.im,tx_filt_phase.re+0.000000001f);
	tx_filt_phase.re=cosf(ph_rt);
	tx_filt_phase.im=sinf(ph_rt);
	//---------------------*/
    }

    // Zero filling and output IFFT-8N
    vset_f_D((float *)&Cmplx_Array[N],0.0f,2*6*N);
	ifft(2*N4,Fl_Array_R,Fl_Array_I,Cmplx_Array);  
              
    // Extract OFDM TX symbol   
   	vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[4*N_L],&Fl_Array_I[4*N_L],4*NL);
   
	break;
	
	case 8:  // !! Nmax = 128 (IFFT-16N=16*128=2048=FFTmax)
	// filter TX and interpolation by 8
    vmul_c_DDE(Cmplx_Array,&Cmplx_Array[N2],&Filter_TX[N],N);
    vmul_c_DDE(&Cmplx_Array[15*N],&Cmplx_Array[N],Filter_TX,N);

	if (L==7) //phase rotation e.g. for N=128 L=7 with FN=6000
    {
    	
    vsmul_c_DD( Cmplx_Array     ,tx_filt_phase, Cmplx_Array     ,N);
    vsmul_c_DD(&Cmplx_Array[15*N],tx_filt_phase,&Cmplx_Array[15*N],N);
    
    //tx_filt_phase.re = tx_filt_phase.re * tx_filt_phaserot.re;
		// tx_filt_phase.re = -1.0f * tx_filt_phase.re; //switches from -1, 1 ,-1 ...
    //Phase rotation is simple in our special case, but here is the usual procedure
	ph_rt           =tx_filt_phase.re*tx_filt_phaserot.re - tx_filt_phase.im*tx_filt_phaserot.im;
	tx_filt_phase.im=tx_filt_phase.re*tx_filt_phaserot.im + tx_filt_phase.im*tx_filt_phaserot.re;
	tx_filt_phase.re=ph_rt;
	//-------------------------- tx_filt_phase Regeneration ----------------------

	ph_rt=atan2q(tx_filt_phase.im,tx_filt_phase.re+0.000000001f);
	tx_filt_phase.re=cosf(ph_rt);
	tx_filt_phase.im=sinf(ph_rt);
	//---------------------*/
    }

    // Zero filling and output IFFT-16N
    vset_f_D((float *)&Cmplx_Array[N],0.0f,2*14*N);
	ifft(4*N4,Fl_Array_R,Fl_Array_I,Cmplx_Array);  
              
    // Extract OFDM TX symbol   
   	vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[8*N_L],&Fl_Array_I[8*N_L],8*NL);
   
	break;
	}
   	
	// Output in Cmplx_Array[0] length 2NL (for Interpolation by 2)
	//							length 4NL (for Interpolation by 4)
	//							length 8NL (for Interpolation by 8)

   	
}


