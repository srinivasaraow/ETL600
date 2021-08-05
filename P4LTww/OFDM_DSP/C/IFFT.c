/******************************************************************************
 *
 * PROJECT     : Test of common FFT  
 * FILE        : $Workfile: IFFT.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : IFFT for N=8,...,2048
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/IFFT.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       24.12.2003 DLK
 * Modified for use with universal ifft_c_dpd.asm routings by VGP
 *       21.12.2004 VGP
 *****************************************************************************/
 
#include <complex.h>
#include "../h/fft/fft_c_dpd.h"

void segment ( "seg_ext_pmco" ) ifft(int NFFT, float *Array_R, float pm *Array_I, complex_float *Cmplx)
	{
	switch ( NFFT )
    	{      
       	case 8:
       	// IFFT-8	
    	ifft8_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 16:
       	// IFFT-16	
    	ifft16_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 32:
       	// IFFT-32	
    	ifft32_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 64:
       	// IFFT-64	
    	ifft64_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 128:
       	// IFFT-128	
    	ifft128_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 256:
       	// IFFT-256	
    	ifft256_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 512:
       	// IFFT-512	
    	ifft512_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 1024:
       	// IFFT-1024	
    	ifft1024_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
       	case 2048:
       	// IFFT-2048	
    	ifft2048_c_DPD(Array_R, Array_I, Cmplx);   
		
		break;
      	
      	default:      	  

      	break;
      	}
	}


