/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Preamble_processing_TX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : TX Preamble Processing
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Preamble_processing_TX.c $
 * 
 * 10    29.08.08 15:35 Chhamaa
 * 
 * 4     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 2     18.07.05 8:46 Chharyt
 * Exchange with TLC. By date 2005-07-15    Version: OFDM_DSP_V0_09U
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 5     22.12.04 9:30 Chchlee
 * 
 * 4     2.12.04 17:54 Chhamaa
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified structure
 *       05.04.2004 PL 
 *
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

#include "../h/tcm/TCM_Codec_externs.h"

int Delay_Measure_Flag=1;
int Delay_ctr=0;
//int EP_ctr =0;

int Preamble_Tone0, Preamble_Tone1; // wird initialisiert in void PreambleTX2Tone_Init(int index0, int index1) 
									// gemeinsam mit den Phasenrotations-Koeffizienten


//========================================
void segment ( "seg_ext_pmco" ) Preamble_processing_TX()
	{
    int i;
    complex_float pilot_temp;
	//int tempi;

    //void  Trellis_Encoder(int *, complex_float *, int, int *);
    
    //void  Scrambler(int *, int *);
    //void  TCM_Gen_PCM_Data(int *, int, int );
	//void  Spectrum_QAM(complex_float *, complex_float *, int *, int, float);
    void  ifft(int, float *, float pm *, complex_float *);

	switch ( TX_Preamble_State )
   		{ 
   		case TX_NL_Mode_Call:
   			if ((OFDM_Modem_Type == OFDM_MASTER) // Master allways sends Call
   				|| (TX_NL_Call == 1))			 // Slave only, if Call command was set
   											     // in Preamble_scheduler_RX.c		
   			{	
   			// Clear Cmplx_Array 
	  		vset_f_E((float *)&Cmplx_Array[0],0.0f,N2);

   			// Building of spectrum of 2 tone signal
	  		Cmplx_Array[Tone_ind0].re = Amp_Tone_TX*C_TX[Tone_ind0];
   			Cmplx_Array[Tone_ind1].re = Amp_Tone_TX*C_TX[Tone_ind1];
          
   			// Care for continuous phase by additional phase rotation for L=7
   			if (L==7)
	   			{
	   			// Cmplx_Array[Tone_ind0] = Amp_Tone_TX*C_TX[Tone_ind0] * tx2tone0_phase;
				Cmplx_Array[Tone_ind0].im = Cmplx_Array[Tone_ind0].re*tx2tone0_phase.im;
				Cmplx_Array[Tone_ind0].re = Cmplx_Array[Tone_ind0].re*tx2tone0_phase.re;


	   			// Cmplx_Array[Tone_ind1] = Amp_Tone_TX*C_TX[Tone_ind1] * tx2tone1_phase;
				Cmplx_Array[Tone_ind1].im = Cmplx_Array[Tone_ind1].re*tx2tone1_phase.im;
				Cmplx_Array[Tone_ind1].re = Cmplx_Array[Tone_ind1].re*tx2tone1_phase.re;

				// tx2tone0_phase = tx2tone0_phase * tx2tone0_phaserot;
   				pilot_temp.re     = tx2tone0_phase.re*tx2tone0_phaserot.re - tx2tone0_phase.im*tx2tone0_phaserot.im;
				tx2tone0_phase.im = tx2tone0_phase.re*tx2tone0_phaserot.im + tx2tone0_phase.im*tx2tone0_phaserot.re;
				tx2tone0_phase.re = pilot_temp.re;
					/*-------------------------- Phase Regeneration ----------------------*/
					pilot_temp.re=atan2q(tx2tone0_phase.im,tx2tone0_phase.re+0.000000001f);
					tx2tone0_phase.re=cosf(pilot_temp.re);
					tx2tone0_phase.im=sinf(pilot_temp.re);
					/*---------------------*/
   				
				// tx2tone1_phase = tx2tone1_phase * tx2tone1_phaserot;
   				pilot_temp.re     = tx2tone1_phase.re*tx2tone1_phaserot.re - tx2tone1_phase.im*tx2tone1_phaserot.im;
				tx2tone1_phase.im = tx2tone1_phase.re*tx2tone1_phaserot.im + tx2tone1_phase.im*tx2tone1_phaserot.re;
				tx2tone1_phase.re = pilot_temp.re;
					/*-------------------------- Phase Regeneration ----------------------*/
					pilot_temp.re=atan2q(tx2tone1_phase.im,tx2tone1_phase.re+0.000000001f);
					tx2tone1_phase.re=cosf(pilot_temp.re);
					tx2tone1_phase.im=sinf(pilot_temp.re);
					/*---------------------*/
   				} //L==7
   		          
			// Get OFDM symbol in time domain 
       		ifft(N, Fl_Array_R, Fl_Array_I, Cmplx_Array); 
			
       		// Cyclic extension: output in &Cmplx_Array[N2+N-L]
	    	//vcombine_c_DDP(&Cmplx_Array[N2+N_L],&Fl_Array_R[N_L],&Fl_Array_I[N_L],L);      
	    	vcombine_c_DDP(&Cmplx_Array[N2+N_L],&Fl_Array_R[N_L],&Fl_Array_I[N_L],L_PROTO);      
	    	vcombine_c_DDP(&Cmplx_Array[N2+N],Fl_Array_R,Fl_Array_I,N); 
	    	
   			}
   			TX_NL_Call = 0; //Reset Call Command
   			break;
 
   		case TX_2TONE_1:
   		case TX_2TONE_2:
        case TX_2TONE_3:

          	// Clear Cmplx_Array 
	  		vset_f_E((float *)&Cmplx_Array[0],0.0f,2*N4);
          
	       	 // Wait for input 2 tone signal in the slave modem
   			if((OFDM_Modem_Type == OFDM_SLAVE) && (Num_Symbol_TX == 0)) break;
        
   			// Increment Tx Preamble State Counter
   			txp_state_counter++;
   			
	   		// Clear Cmplx_Array 
	  		//vset_f_E((float *)&Cmplx_Array[0],0.0f,N2);

   			// Building of spectrum of 2 tone signal
   			Cmplx_Array[Preamble_Tone0].re = Amp_Tone_TX*C_TX[Preamble_Tone0];
   			Cmplx_Array[Preamble_Tone1].re = Amp_Tone_TX*C_TX[Preamble_Tone1];

   			// Care for continuous phase by additional phase rotation for L=7
   			if (L==7)
	   			{
	   			// Cmplx_Array[Tone_ind0] = Amp_Tone_TX*C_TX[Tone_ind0] * tx2tone0_phase;
				Cmplx_Array[Preamble_Tone0].im = Cmplx_Array[Preamble_Tone0].re*tx2tone0_phase.im;
				Cmplx_Array[Preamble_Tone0].re = Cmplx_Array[Preamble_Tone0].re*tx2tone0_phase.re;


	   			// Cmplx_Array[Tone_ind1] = Amp_Tone_TX*C_TX[Tone_ind1] * tx2tone1_phase;
				Cmplx_Array[Preamble_Tone1].im = Cmplx_Array[Preamble_Tone1].re*tx2tone1_phase.im;
				Cmplx_Array[Preamble_Tone1].re = Cmplx_Array[Preamble_Tone1].re*tx2tone1_phase.re;

				// tx2tone0_phase = tx2tone0_phase * tx2tone0_phaserot;
   				pilot_temp.re     = tx2tone0_phase.re*tx2tone0_phaserot.re - tx2tone0_phase.im*tx2tone0_phaserot.im;
				tx2tone0_phase.im = tx2tone0_phase.re*tx2tone0_phaserot.im + tx2tone0_phase.im*tx2tone0_phaserot.re;
				tx2tone0_phase.re = pilot_temp.re;
					/*-------------------------- Phase Regeneration ----------------------*/
					pilot_temp.re=atan2q(tx2tone0_phase.im,tx2tone0_phase.re+0.000000001f);
					tx2tone0_phase.re=cosf(pilot_temp.re);
					tx2tone0_phase.im=sinf(pilot_temp.re);
					/*---------------------*/
   				
				// tx2tone1_phase = tx2tone1_phase * tx2tone1_phaserot;
   				pilot_temp.re     = tx2tone1_phase.re*tx2tone1_phaserot.re - tx2tone1_phase.im*tx2tone1_phaserot.im;
				tx2tone1_phase.im = tx2tone1_phase.re*tx2tone1_phaserot.im + tx2tone1_phase.im*tx2tone1_phaserot.re;
				tx2tone1_phase.re = pilot_temp.re;
					/*-------------------------- Phase Regeneration ----------------------*/
					pilot_temp.re=atan2q(tx2tone1_phase.im,tx2tone1_phase.re+0.000000001f);
					tx2tone1_phase.re=cosf(pilot_temp.re);
					tx2tone1_phase.im=sinf(pilot_temp.re);
					/*---------------------*/
   				}
   		          
			// Get OFDM symbol in time domain 
       		ifft(N, Fl_Array_R, Fl_Array_I, Cmplx_Array); 
			
       		// Cyclic extension: output in &Cmplx_Array[N2+N-L]
	    	//vcombine_c_DDP(&Cmplx_Array[N2+N_L],&Fl_Array_R[N_L],&Fl_Array_I[N_L],L);      
	    	vcombine_c_DDP(&Cmplx_Array[N2+N_L],&Fl_Array_R[N_L],&Fl_Array_I[N_L],L_PROTO);      
	    	vcombine_c_DDP(&Cmplx_Array[N2+N],Fl_Array_R,Fl_Array_I,N);    
			

	 	break;
        
       	
   		case TX_PAUSE:
         
   			// Increment Tx Preamble State Counter
   			txp_state_counter++;
   			if (Delay_Measure_Flag==1) Delay_ctr++;
   			// Clear Cmplx_Array 
  			vset_f_E((float *)&Cmplx_Array[N2+N_L],0.0f,2*NL);

       	break;

       	
       	case TX_LFM:
			
       		// Increment Tx Preamble State Counter
   			txp_state_counter++;
   			
        	// LFM signal in time domain: output in &Cmplx_Array[N2+N-L]
       		for (i=0; i<NL; i++)
	       		{
	       		Cmplx_Array[N2+N_L+i].re = OFDM_LFM_2_TX[LFM_Index].re;
	       		Cmplx_Array[N2+N_L+i].im = OFDM_LFM_2_TX[LFM_Index].im;      	
//	       		if ( ++LFM_Index == N2 ) LFM_Index = 0;
	       		if ( ++LFM_Index == LFM_Size ) LFM_Index = 0;
	   			}      
	
		break;
	
			
		case TX_Preamble_IDLE:
      		// no operation
      		// Clear Cmplx_Array (clear output signal)
  			vset_f_E((float *)&Cmplx_Array[N2+N_L],0.0f,2*NL);
      		
		break;
		}
		
		// for debug: save EP
		/*
			tempi = EP_ctr*2*NL;
			if (tempi < (SKP_NUM-2*NL))
			{
			vcopy_ED(&MSE_trend_QAMD[tempi],(float *)&Cmplx_Array[N2+N_L],2*NL);
			EP_ctr++;
			}
			else
			{
		//	while(1);
			asm("nop;");	
			asm("nop;");	
			asm("nop;");	
			}
			*/
			
	}

