/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Spectrum_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : RX Spectrum estimation
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_Spectrum_RX.c $
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     2.12.04 17:54 Chhamaa
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified spectrum dencity calculation
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
void segment ( "seg_ext_pmco" ) OFDM_Spectrum_RX()
	{
	  void  fft(int, float *, float pm *, complex_float *);
	  void  ifft(int, float *, float pm *, complex_float *);
		
	  float Amax; 
	  static int spec_ctr=0; 

      // Calculate spectral dencity of signal at input of RX filter in digital loop
// if((Start_RX_QAM_Engine == 1) && (OFDM_Loop_Mode == DIGITAL_LOOP))
	if(Start_RX_QAM_Engine == 1)
		{
/*			
				asm("#include <def21161.h>");
    asm("dm(CP1A)=m5;");
    asm("dm(CP3A)=m5;");
    asm("dm(SPCTL0) = m5;");
    asm("dm(SPCTL2) = m5;");    
    asm("nop;");
asm("nop;");
asm("nop;");
*/
    	vcopy_DE((float *)&Cmplx_Array[2*N_L],(float *)&OFDM_Symbol_RX[0],4*NL);

        // Spectrum analyzer Overlap/Save 
        vcopy_DE((float *)&Cmplx_Array[0],(float *)&Overlap_Save_Spectrum_RX[0],4*N_L);
        vcopy_ED((float *)&Overlap_Save_Spectrum_RX[0],(float *)&Cmplx_Array[2*NL],4*N_L);
      
    	// Windowing in time domain
	   	// Harris window for spectrum estimation
    	// gharris(Fl_Array_R,N4);	// done in modem_init_basic	

    	vcopy_DE(Fl_Array_R,Harris_Window,N4);
   		vmul_ccf_DDD(Cmplx_Array,Cmplx_Array,Fl_Array_R,N4);
    	
    	// FFT-2N: input and output in Cmplx_Array 
    	fft(N4,Fl_Array_R,Fl_Array_I,Cmplx_Array);   
		if (spec_ctr++ > 10)
			{
			spec_ctr--;
			
			// Calculate spectral density of RX signal after RX Filter
	   		vmag2_uc_DDP(Fl_Array_R,Fl_Array_R,Fl_Array_I,N4);
	        vcopy_PE(Fl_Array_I,Mag2_Inp,N4);
	        
	        vadd_f_DDP(Fl_Array_R,Fl_Array_R,Fl_Array_I,N4);
	   		vcopy_ED(Mag2_Inp,Fl_Array_R,N4);
		    Amax = vmax_f_D(Fl_Array_R,N4);
		    if (Amax > 0.0f)
		    	{
	   			vsmul_f_DD(Fl_Array_R,1.0f/Amax,Fl_Array_R,N4);
		    	}
	   		vadb_f_DD(Fl_Array_R,Fl_Array_R,N4);
	   		vcopy_ED(Mag2l_Inp,Fl_Array_R,N4);
	
			}
			
   		if(n_blk_rx_M_QAM_train>80)
   			{
   			asm("nop;");
			asm("nop;");
			asm("nop;");
   			}

   	}
     }

      	
