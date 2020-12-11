/******************************************************************************
 *
 * PROJECT     : Test of common FFT  
 * FILE        : $Workfile: FFT.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : FFT for N=8,...,2048
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/FFT.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       24.12.2003 DLK
 * Modified for use with universal fft_c_dpd.asm function by VGP
 *       21.12.2004 VGP
 *****************************************************************************/
 
#include <complex.h>
#include "../h/fft/fft_c_dpd.h"


void segment ( "seg_ext_pmco" ) fft(int NFFT, float *Array_R, float pm *Array_I, complex_float *Cmplx)
	{
	switch ( NFFT )
    	{      
       	case 8:
       	// FFT-8	
        fft8_c_DPD(Array_R, Array_I, Cmplx);		
		break;
      	
       	case 16:
       	// FFT-16	
        fft16_c_DPD(Array_R, Array_I, Cmplx);		
		
		break;
      	
       	case 32:
       	// FFT-32	
        fft32_c_DPD(Array_R, Array_I, Cmplx);		
		
		break;
      	
       	case 64:
       	// FFT-64	
    	fft64_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 128:
       	// FFT-128	
    	fft128_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 256:
       	// FFT-256	
    	fft256_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 512:
       	// FFT-512	
    	fft512_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 1024:
       	// FFT-1024	
    	fft1024_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 2048:
       	// FFT-2048	
    	fft2048_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
      	default:      	  

      	break;
      	}

	}


