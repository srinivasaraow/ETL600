/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Channel_Simulator_Init.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM Channel Simulator Initialization
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_Channel_Simulator_Init.c $
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     28.09.04 17:45 Maag01
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
void segment ( "seg_ext_pmco" ) OFDM_Channel_Simulator_Init()
	{
    void  fft(int, float *, float pm *, complex_float *);
	void  ifft(int, float *, float pm *, complex_float *);
 	
	float A0, A1, a0, a1, phi0, phi1, Amax;
	int i;	
	
	//************ Nonlinear distortion test*********** 
	Sim_NL_Dist_Flag = OFF;
	//************ Linear distortion test*********** 
	Sim_Lin_Dist_Flag = OFF;
	//************ Base Attenuation *********** 
	Sim_Attenuation_Flag = OFF;
	A_loss_dB = 12.0f;	
    //************ Frequency offset test*********** 
    Sim_Frequency_Offset_Flag = OFF;
	//**************** AWGN ***********************
	Sim_AWGN_Flag = OFF;
	//************ Narrow band interference*********** 
	Sim_NB_Flag = OFF;
	
	//************ Linear distortion test*********** 

	if(Sim_Lin_Dist_Flag == OFF)
		{
		// Depth of notches in dB
		A_Notch0 = 0.0f;
		A_Notch1 = 0.0f;

		// Frequency of notches Hz
		F_Notch0 = 0.0f;
		F_Notch1 = 0.0f;

		vset_f_E(Sim_Tr_Funct_Mag,1.0f,N4); 	  
		}
	else
		{
			
		// Depth of notches in dB
		A_Notch0 = 20.0f;
		A_Notch1 = 0.0f;

		// Frequency of notches Hz
		F_Notch0 = 15000.0f;
		F_Notch1 = 48000.0f;
	
		// Reset overlap/save buffer array
		vset_f_E((float *)Sim_Overlap_Save,0.0f,sizeof(Sim_Overlap_Save));

		a0=powf(10.0f,0.05f*A_Notch0);
		a1=powf(10.0f,0.05f*A_Notch1);
	
		A0=(a0-1.0f)/(a0+1.0f);
		A1=(a1-1.0f)/(a1+1.0f);

		phi0=OFDM_TWO_PI*(F_Notch0/FS_TX_RX-0.5f);
		phi1=OFDM_TWO_PI*(F_Notch1/FS_TX_RX-0.5f);
	
    	// Calculation of transfer function of first notch
		vset_f_E((float *)&Cmplx_Array[0],0.0f,8*N);

    	Cmplx_Array[0].re = 1.0f/sqrtf(1.0f+A0*A0);
		Cmplx_Array[0].im = 0.0f;

		Cmplx_Array[1].re = A0/sqrtf(1.0f+A0*A0)*cosf(phi0);
		Cmplx_Array[1].im = A0/sqrtf(1.0f+A0*A0)*sinf(phi0);
			
		fft(N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
    	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N4); 
 
    	// Store transfer function of first notch
    	vcopy_ED((float *)&Sim_Tr_Funct[0],(float *)&Cmplx_Array[0],2*N4); 	  

    	// Calculation of transfer function of secund notch
		vset_f_E((float *)&Cmplx_Array[0],0.0f,8*N);

    	Cmplx_Array[0].re = 1.0f/sqrtf(1.0f+A1*A1);
		Cmplx_Array[0].im = 0.0f;

		Cmplx_Array[1].re = A1/sqrtf(1.0f+A1*A1)*cosf(phi1);
		Cmplx_Array[1].im = A1/sqrtf(1.0f+A1*A1)*sinf(phi1);
			
		fft(N4, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
    	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N4); 
 
    	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Sim_Tr_Funct[0],2*N2); 	  
    	vmul_c_DDD(Cmplx_Array,Cmplx_Array,(complex_float *)&Fl_Array_R[0],N2);
    	vcopy_DE((float *)&Fl_Array_R[0],(float *)&Sim_Tr_Funct[N2],2*N2); 	  
    	vmul_c_DDD(&Cmplx_Array[N2],&Cmplx_Array[N2],(complex_float *)&Fl_Array_R[0],N2);

    	// Store transfer function of channel
    	vcopy_ED((float *)&Sim_Tr_Funct[0],(float *)&Cmplx_Array[0],2*N4); 	  

    	// Store magnitude of channel transfer function
    	vabs_c_DD(Fl_Array_R,Cmplx_Array,N4); 	  
		Amax = vmax_f_D(Fl_Array_R,N4);
    	vsmul_f_DD(Fl_Array_R,1.0f/Amax,Fl_Array_R,N4);
    	vadb_f_DD(Fl_Array_R,Fl_Array_R,N4);
    	vsmul_f_DD(Fl_Array_R,2.0f,Fl_Array_R,N4);
    	vcopy_ED(Sim_Tr_Funct_Mag,Fl_Array_R,N4);
    		  
		}
		
    //************ Base Attenuation Loss ***********
    
    if (Sim_Attenuation_Flag == ON)
    	{
    	a_loss_base = powf(10.0f,-0.05f*A_loss_dB);
    	}
    	else
    	{
    	a_loss_base = 1.0f;
    	}
    	
    //************ Frequency offset test*********** 

	if(Sim_Frequency_Offset_Flag == OFF)
		{
		// Frequency offset in Hz 
		Df_Hz_T = 0.0f;   
		}
	else
		{		
		// Frequency offset in Hz 
		Df_Hz_T = 10.0f;   

   		Freq_Offset_T = Df_Hz_T*OFDM_TWO_PI/FS_TX_RX;

    	Rot_Fr_T.re = cosf(Freq_Offset_T);
		Rot_Fr_T.im = sinf(Freq_Offset_T);
			
		Delta_Phi_Symbol_T = Freq_Offset_T*(float)(2*NL);

		Start_Phi_T = 0.0f;
		Start_Phi_New_T = 0.0f;
		}
	//**************** AWGN ***********************

	if(Sim_AWGN_Flag == OFF)
		{
		// SNR in dB
		SNR=100.0f; 
		}
	else
		{
		// SNR in dB
		SNR=34.0f; 
	
		AWGN_sigma = sqrtf(powf(10.0f,-0.1f*SNR)/2.0f);
	
		ranseed(5792641);
	
		AWGN_Power_Mean = 0.0f;
		}
	//************ Narrow band interference*********** 

	if(Sim_NB_Flag == OFF)
		{
		// Amplitude of first NB interferer [dB]
		A_NB0 = -100.0f;   

		// Amplitude of secund NB interferer [dB]
		A_NB1 = -100.0f;   

		// Frequency of first NB interferer [Hz]
		F_NB0 = 0.0f;   

		// Frequency of secund NB interferer [Hz]
		F_NB1 = 0.0f;   

		}
	else
		{

		// Amplitude of first NB interferer [dB]
		A_NB0 = -35.0f;   

		// Amplitude of secund NB interferer [dB]
		A_NB1 = -100.0f;   

		// Frequency of first NB interferer [Hz]
		F_NB0 = -12000.0f;   

		// Frequency of secund NB interferer [Hz]
		F_NB1 = 005.0f;   

		Freq_NB0 = F_NB0*(OFDM_TWO_PI/FS_TX_RX);
		Freq_NB1 = F_NB1*(OFDM_TWO_PI/FS_TX_RX);

    	Rot_NB0.re = cosf(Freq_NB0);
		Rot_NB0.im = sinf(Freq_NB0);
			
    	Rot_NB1.re = cosf(Freq_NB1);
		Rot_NB1.im = sinf(Freq_NB1);

		Delta_Phi_NB0 = Freq_NB0*(float)(2*NL);
		Delta_Phi_NB1 = Freq_NB1*(float)(2*NL);

		Start_Phi_NB0 = 0.0f;
		Start_Phi_New_NB0 = 0.0f;

		Start_Phi_NB1 = 0.0f;
		Start_Phi_New_NB1 = 0.0f;

		a_NB0 = powf(10.0f,0.05f*A_NB0);
		a_NB1 = powf(10.0f,0.05f*A_NB1);

		}
	}


