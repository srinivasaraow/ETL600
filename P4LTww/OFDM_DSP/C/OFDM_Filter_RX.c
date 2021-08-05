/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Filter_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : RX Filter
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Filter_RX.c $
 * 
 * 5     5.04.06 15:28 Chhamaa
 * 
 * 5     17.03.06 14:41 Chhamaa
 * 
 * 3     13.09.05 13:23 Ats
 * Exchange from TLC
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * RX filter using SDRAM function
 *       02.03.04 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

#include "../h/tcm/TCM_Codec_externs.h"

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Filter_RX()
	{
	  float Exp_Gen(float, complex_float, complex_float *, float, int);
	  void  fft(int, float *, float pm *, complex_float *);
	  void  ifft(int, float *, float pm *, complex_float *);
	  
	  //float ph_rt;

//----------------------------------------------------------------
// For N==64 we use a RX_Filter of doubled length
// Length of Filter Impulse response is 2N-L+1
// Therefore we have to implement an Overlap_Save of at least (2N-L)
// in combination with the OFDM symbol of length N+L we need to apply
// an FFT of size (2N-L)+N+L=3N. We use 4N FFT.

	//if (N==64)
	if (use_long_RX_Filter_2NmL)
	{
	switch (Filter_RX_Decimation)
		{
		case 2:
		// If we have decimation factor 2, the size of Overlap_Save Buffer at the input 
		// must be doubled. So we use Overlap_Save Buffer Size 2*(2N-L)=4N-2L
		// The size of the input signal is 2(N+L)
	 	vset_f_E((float *)&Cmplx_Array[0],0.0f,2*8*N);
	
     	  // Store input samples at the tail of filter input buffer  [..6N]
		  vcopy_DE((float *)&Cmplx_Array[4*N-2*L],(float *)&OFDM_Symbol_RX[0],2*2*NL);                
	
	  	  // Receiver Overlap/Save input
	      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*(4*N-2*L));
	      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[2*NL],2*(4*N-2*L));
	      
    	  // Scaling and Initial normalization of input signal
      	  vsmul_f_DD((float *)&Cmplx_Array[0],Input_Scale_RX*Digital_AGC,(float *)&Cmplx_Array[0],2*8*N);   
	
	      // Decimation by 2 
	      fft(8*N, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
	      vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[6*N],&Fl_Array_I[6*N],N2); 
	      vcombine_c_DDP(&Cmplx_Array[N2],Fl_Array_R,Fl_Array_I,N2); 
	    break;
	      
	    case 4:
	    // If we have decimation factor 4, the size of Overlap_Save Buffer at the input 
		// must be multiplied by 4. So we use Overlap_Save Buffer Size 4*(2N-L)=8N-4L
		// The size of the input signal is 4(N+L)
	  	vset_f_E((float *)&Cmplx_Array[0],0.0f,2*16*N);

     	  // Store input samples at the tail of filter input buffer  [..12N]
		  vcopy_DE((float *)&Cmplx_Array[8*N-4*L],(float *)&OFDM_Symbol_RX[0],2*4*NL);                
	
	   	  // Receiver Overlap/Save input
	      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*(8*N-4*L));
	      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[4*NL],2*(8*N-4*L));
	      
    	  // Scaling and Initial normalization of input signal
      	  vsmul_f_DD((float *)&Cmplx_Array[0],Input_Scale_RX*Digital_AGC,(float *)&Cmplx_Array[0],2*16*N);   
	
	      // Decimation by 4 
	      fft(16*N, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
	      vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[14*N],&Fl_Array_I[14*N],N2); 
	      vcombine_c_DDP(&Cmplx_Array[N2],Fl_Array_R,Fl_Array_I,N2); 
	    break;
	    
	    case 8:
	    // If we have decimation factor 8, the size of Overlap_Save Buffer at the input 
		// must be multiplied by 8. So we use Overlap_Save Buffer Size 8*(2N-L)=16N-8L
		// The size of the input signal is 8(N+L)
	  	vset_f_E((float *)&Cmplx_Array[0],0.0f,2*32*N);

     	  // Store input samples at the tail of filter input buffer  [..24N]
		  vcopy_DE((float *)&Cmplx_Array[16*N-8*L],(float *)&OFDM_Symbol_RX[0],2*8*NL);                
	
	   	  // Receiver Overlap/Save input
	      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*(16*N-8*L));
	      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[8*NL],2*(16*N-8*L));
	      
    	  // Scaling and Initial normalization of input signal
      	  vsmul_f_DD((float *)&Cmplx_Array[0],Input_Scale_RX*Digital_AGC,(float *)&Cmplx_Array[0],2*32*N);   
	
	      // Decimation by 8  FFT-Length=32*N=32*64=2048=FFTmax
	      fft(32*N, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
	      vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[30*N],&Fl_Array_I[30*N],N2); 
	      vcombine_c_DDP(&Cmplx_Array[N2],Fl_Array_R,Fl_Array_I,N2); 
	    break;
		}
	
	  // -------- common part for N==64 ---------------------
    	
	  // Save input signal with complex conjugation for RX filter adaptation	 
	  if(Start_RX_QAM_Engine == 1)
	   	{
	    vconjg_c_DD((complex_float *)&Fl_Array_R[0],Cmplx_Array,4*N); 
		vcopy_ED((float *)&Temp_Buf[0],Fl_Array_R,2*4*N);
	   	}
	     	
	  // RX Filter
	  vmul_c_DDE(Cmplx_Array,Cmplx_Array,C_Filter_RX,4*N);
	
	  ifft(4*N, Fl_Array_R, Fl_Array_I, Cmplx_Array);  
	
	  // Extract symbol                                
//	  vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[2*N-L],&Fl_Array_I[2*N-L],NL); 
	  vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[2*N-L],&Fl_Array_I[2*N-L],N+L_PROTO); 
			
	}
	else  // it means, not(use_long_RX_Filter_2NmL)
	//----------------------------------------------------------------
	// For N>64 we use a RX_Filter of standard length 
	// Length of Filter Impulse response is N-L+1
	// Therefore we have to implement an Overlap_Save of at least (N-L)
	// in combination with the OFDM symbol of length N+L we need to apply
	// an FFT of size (N-L)+N+L=2N. We use 2N FFT.

	{
	
	switch (Filter_RX_Decimation)
	{
	case 2:
		// If we have decimation factor 2, the size of Overlap_Save Buffer at the input 
		// must be doubled. So we use Overlap_Save Buffer Size 2*(N-L)=2N-2L
		// The size of the input signal is 2(N+L)
      
     // Store input samples at the tail of filter input buffer  
	  vcopy_DE((float *)&Cmplx_Array[2*N_L],(float *)&OFDM_Symbol_RX[0],2*2*NL);                
     
      // Receiver Overlap/Save input, Store saved samples at the head of filter input buffer
      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*2*N_L);
      // update saved samples with actual tail of input samples
      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[2*NL],2*2*N_L);
/*--  
      // Symbol synchronization  
	  vcopy_DE((float *)&Cmplx_Array[2*N_L],(float *)&Symbol_Buf[0],2*(2*NL-Symbol_Synchro));                
      if(Symbol_Synchro > 0)
      	vcopy_DE((float *)&Cmplx_Array[N4-Symbol_Synchro],(float *)&OFDM_Symbol_RX[0],2*Symbol_Synchro);                

      // Save input OFDM symbol in buffer Symbol_Buf
      vcopy_EE((float *)&Symbol_Buf[0],(float *)&OFDM_Symbol_RX[Symbol_Synchro],2*(2*NL-Symbol_Synchro));                
   	  // Receiver Overlap/Save input
      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*2*N_L);
      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[2*NL],2*2*N_L);

      
*/      
      // Scaling and Initial normalization of input signal
      vsmul_f_DD((float *)&Cmplx_Array[0],Input_Scale_RX*Digital_AGC,(float *)&Cmplx_Array[0],8*N);   

      // Decimation by 2 
      fft(N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
      vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[3*N],&Fl_Array_I[3*N],N); 
      vcombine_c_DDP(&Cmplx_Array[N],Fl_Array_R,Fl_Array_I,N); 
                
      break;
     
       
    case 4:
   		// If we have decimation factor 4, the size of Overlap_Save Buffer at the input 
		// must be multiplied by 4. So we use Overlap_Save Buffer Size 4*(N-L)=4N-4L
		// The size of the input signal is 4(N+L)

      // Store input samples at the tail of filter input buffer  [..12N]
	  vcopy_DE((float *)&Cmplx_Array[4*N_L],(float *)&OFDM_Symbol_RX[0],2*4*NL);                

   	  // Receiver Overlap/Save input
      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*4*N_L);
      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[4*NL],2*4*N_L);
      
      // Scaling and Initial normalization of input signal
      vsmul_f_DD((float *)&Cmplx_Array[0],Input_Scale_RX*Digital_AGC,(float *)&Cmplx_Array[0],2*8*N);   

      // FFT 8N cmplx and Decimation by 4 
      fft(2*N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
      vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[7*N],&Fl_Array_I[7*N],N); 
      vcombine_c_DDP(&Cmplx_Array[N],Fl_Array_R,Fl_Array_I,N); 
      
      break;
      
    case 8:
   		// If we have decimation factor 8, the size of Overlap_Save Buffer at the input 
		// must be multiplied by 8. So we use Overlap_Save Buffer Size 8*(N-L)=8N-8L
		// The size of the input signal is 8(N+L)
		// FFT size is 16N, [[OS 8N-8L][SIG 8N+8L]]
        // !!! N=512,256 not allowed, due to FFT of length 16N (16*128=2048=max_FFT)

      // Store input samples at the tail of filter input buffer  [..16N]
	  vcopy_DE((float *)&Cmplx_Array[8*N_L],(float *)&OFDM_Symbol_RX[0],2*8*NL);                

   	  // Receiver Overlap/Save input
      vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_RX[0],2*8*N_L);
      vcopy_ED((float *)&Overlap_Save_RX[0],(float *)&Cmplx_Array[8*NL],2*8*N_L);
      
      // Scaling and Initial normalization of input signal
      vsmul_f_DD((float *)&Cmplx_Array[0],Input_Scale_RX*Digital_AGC,(float *)&Cmplx_Array[0],2*16*N);   

      // FFT 16N cmplx and Decimation by 8 
      fft(4*N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
      vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[15*N],&Fl_Array_I[15*N],N); 
      vcombine_c_DDP(&Cmplx_Array[N],Fl_Array_R,Fl_Array_I,N); 
      
      break;
	}
	// -------- common part for N>64 ---------------------
     
	// Phase rotation e.g. for N=128 L=7 with FN=9600 
	if(L==7)
	{
	vsmul_c_DD(Cmplx_Array,rx_filt_phase,Cmplx_Array,N2);

	//rx_filt_phase.re = rx_filt_phase.re * rx_filt_phaserot.re;
    rx_filt_phase.re = -1.0f * rx_filt_phase.re; //switches from -1, 1 ,-1 ...
    /* //Phase rotation is simple in our special case, but here is the usual procedure
	ph_rt           =rx_filt_phase.re*rx_filt_phaserot.re - rx_filt_phase.im*rx_filt_phaserot.im;
	rx_filt_phase.im=rx_filt_phase.re*rx_filt_phaserot.im + rx_filt_phase.im*rx_filt_phaserot.re;
	rx_filt_phase.re=ph_rt;
	//-------------------------- rx_filt_phase Regeneration ----------------------

	ph_rt=atan2q(rx_filt_phase.im,rx_filt_phase.re+0.000000001f);
	rx_filt_phase.re=cosf(ph_rt);
	rx_filt_phase.im=sinf(ph_rt);
	//---------------------*/

	}
    // Save input signal with complex conjugation for RX filter adaptation	 
    if(Start_RX_QAM_Engine == 1)
    	{
	    vconjg_c_DD((complex_float *)&Fl_Array_R[0],Cmplx_Array,N2); 
		vcopy_ED((float *)&Temp_Buf[0],Fl_Array_R,2*N2);
      	}
      	
    // RX Filter
    vmul_c_DDE(Cmplx_Array,Cmplx_Array,C_Filter_RX,N2);

    ifft(N2, Fl_Array_R, Fl_Array_I, Cmplx_Array);  

    // Extract symbol                                
//    vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[N_L],&Fl_Array_I[N_L],NL); 
    vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[N_L],&Fl_Array_I[N_L],N+L_PROTO); 
            
  
	} // else (N>64)
	 
 }

      	
