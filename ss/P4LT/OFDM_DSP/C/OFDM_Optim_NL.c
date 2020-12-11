/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Optim_NL.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Optimization of N and L
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Optim_NL.c $
 * 
 * 9     24.07.08 15:32 Chhamaa
 * 
 * 3     18.07.05 8:46 Chharyt
 * Exchange with TLC. By date 2005-07-15    Version: OFDM_DSP_V0_09U
 * 
 * 2     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 1     22.12.04 9:32 Chchlee
 * 
 * Initial version
 *       06.12.2004 PL
  *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/OFDM_cfg_types.h"

void  OFDM_Optim_NL_getSIR(int);
float OFDM_Optim_NL_validateSIR(int, int);
float OFDM_Optim_NL_getSTF(int);




int Ltest;
float FS_score;
float segment ( "seg_extn SHT_NOBITS" ) K_NL_L[2*OFDM_N_MAX];


//========================================
void segment ( "seg_ext_pmco" ) OFDM_Optim_NL_optim()
	{
	// Initialize end result
		N_selected=0;
		L_selected=0;
/*		
	// *********** L= 8 **************************************** 
	Ltest=8;
	// Calculate resulting impulse response after shortening to length Ltest
	OFDM_Optim_NL_getSIR(Ltest);
	// Result in Fl_Array_R and Fl_Array_I
	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N2);
	// store result in Imp_Resp_S08
	vcopy_ED((float *)&Imp_Resp_S08[0],(float *)&Cmplx_Array[0],2*N2);	
	// Validate resulting impulse response
	FS_Score_08 = OFDM_Optim_NL_validateSIR(N2,Ltest);
	FS_Score_08 = 10.0f*log10f(FS_Score_08);

	// *********** L= 16 **************************************** 
	Ltest=16;
	// Calculate resulting impulse response after shortening to length Ltest
	OFDM_Optim_NL_getSIR(Ltest);
	// Result in Fl_Array_R and Fl_Array_I
	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N2);
	// store result in Imp_Resp_S16
	vcopy_ED((float *)&Imp_Resp_S16[0],(float *)&Cmplx_Array[0],2*N2);	
	// Validate resulting impulse response
	FS_Score_16 = OFDM_Optim_NL_validateSIR(N2,Ltest);
	FS_Score_16 = 10.0f*log10f(FS_Score_16);

	// *********** L= 32 **************************************** 
	Ltest=32;
	// Calculate resulting impulse response after shortening to length Ltest
	OFDM_Optim_NL_getSIR(Ltest);
	// Result in Fl_Array_R and Fl_Array_I
	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N2);
	// store result in Imp_Resp_S32
	vcopy_ED((float *)&Imp_Resp_S32[0],(float *)&Cmplx_Array[0],2*N2);	
	// Validate resulting impulse response
	FS_Score_32 = OFDM_Optim_NL_validateSIR(N2,Ltest);
	FS_Score_32 = 10.0f*log10f(FS_Score_32);

	// *********** L= 64 **************************************** 
	Ltest=64;
	// Calculate resulting impulse response after shortening to length Ltest
	OFDM_Optim_NL_getSIR(Ltest);
	// Result in Fl_Array_R and Fl_Array_I
	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N2);
	// store result in Imp_Resp_S64
	vcopy_ED((float *)&Imp_Resp_S64[0],(float *)&Cmplx_Array[0],2*N2);	
	// Validate resulting impulse response
	FS_Score_64 = OFDM_Optim_NL_validateSIR(N2,Ltest);
	FS_Score_64 = 10.0f*log10f(FS_Score_64);
*/
	// *********** L=  8 **************************************** 
	Ltest=8;	
	FS_Score_08 = OFDM_Optim_NL_getSTF(Ltest);
	vcopy_EE((float *)&Imp_Resp_S08[0],(float *)&K_NL_L[0],LFM_Size); //PL

	// *********** L=  16 **************************************** 
	Ltest=16;	
	FS_Score_16 = OFDM_Optim_NL_getSTF(Ltest);
	vcopy_EE((float *)&Imp_Resp_S16[0],(float *)&K_NL_L[0],LFM_Size); //PL

	// *********** L= 32 **************************************** 
	Ltest=32;	
	FS_Score_32 = OFDM_Optim_NL_getSTF(Ltest);
	vcopy_EE((float *)&Imp_Resp_S32[0],(float *)&K_NL_L[0],LFM_Size); //PL

	// *********** L= 64 **************************************** 
	Ltest=64;	
	FS_Score_64 = OFDM_Optim_NL_getSTF(Ltest);
	vcopy_EE((float *)&Imp_Resp_S64[0],(float *)&K_NL_L[0],LFM_Size); //PL

	//FS_score -> L selection
	L_selected_RX = 64;
	
	     if (FS_Score_08 < 0.02f)   L_selected_RX = 8;
	else if (FS_Score_16 < 0.0021f) L_selected_RX = 16;
	else if (FS_Score_32 < 0.0021f) L_selected_RX = 32;
	else if (FS_Score_64 < 0.0021f) L_selected_RX = 64;
	
	// debug  --> N/L selection <--
	switch (Modem_Config.mode_reg.ORTH_Mode)
	{
		case ORTH_Mode64:
		    N_selected_RX = 64;
		    break;
		case ORTH_Mode128:
		    N_selected_RX = 128;
		    break;
		case ORTH_Mode256:
		    N_selected_RX = 256;
		    break;
		case ORTH_Mode512:
		    N_selected_RX = 512;
		    break;
		default:
		    N_selected_RX = 256;
		break;
	}

/*		
	switch (Modem_Config.mode_reg.GUARD_Int)
	{
		case GUARD_Int8:
		    L_selected_RX = 8;
		    break;
		case GUARD_Int16:
		    L_selected_RX = 16;
		    break;
		case GUARD_Int32:
		    L_selected_RX = 32;
		    break;
		case GUARD_Int64:
		    L_selected_RX = 64;
		    break;
		default:
		    L_selected_RX = 32;
		break; // exception return
	}
*/
	// end of debug  --> N/L selection <--

	
	}

void segment ( "seg_ext_pmco" ) OFDM_Optim_NL_select()
	{
	// Extract NL-Optimization results from other side
	N_selected_TX = Channel_Data_NL_TX.N; 	// 
	L_selected_TX = Channel_Data_NL_TX.L;


	if ((Optim_NL_State == Optim_NL_ON_Determination) &&
		(N_selected+L_selected == 0) &&
		(L_selected_TX >0)&&(L_selected_RX >0))
		{
		if (L_selected_TX > L_selected_RX)
			{
			N_selected = N_selected_TX;
			L_selected = L_selected_TX;
			}
		else
			{
			N_selected = N_selected_RX;
			L_selected = L_selected_RX;
			}
		}
	}

//----------------------------------------------------------------
//--	OFDM_Optim_NL_getSIR()	(SIR shorted impulse response)	--
//--	Calculation of resulting impluse response   			--
//--	after "shortening" to length L_test						--
//--	Input Field: Kabs2_RX[] of length N2					--
//--					squared magnitude. of channel transf fct--	
//--	Input: Ltest -  length of desired shortening			--
//--	Output Field: Fl_Array_R,Fl_Array_I						--
//--					resulting channel impulse response re,im--
//----------------------------------------------------------------
void segment ( "seg_ext_pmco" ) OFDM_Optim_NL_getSIR(int Ltest)
	{
	void fft(int, float *, float pm *, complex_float *);
	void ifft(int, float *, float pm *, complex_float *);
	float max;
	int i;
	
		
    // Restore Squared magnitude of channel transfer function Kabs2_RX
    vcopy_DE(Fl_Array_R,(float *)&Kabs2_RX[0],N2); //PL
 
	// Square root and decimation of squared magnitude of transfer function in &Fl_Array_R[N2] 
    vsqrt_sf_DD(&Fl_Array_R[N2],1,Fl_Array_R,N2/Ltest,Ltest);
  
    // Window syntesis
    for (i=0; i<Ltest; i++)
    	{
        Fl_Array_R[i] = 0.5f*(1.0f-cosf(OFDM_TWO_PI/(float)Ltest*(float)i));
    	}
    	
	// Windowing of decimated transfer function in &Fl_Array_R[N2]
    vmul_f_DDD(&Fl_Array_R[N2],Fl_Array_R,&Fl_Array_R[N2],Ltest);

	// Desired impulse response of resulting channel in &Cmplx_Array[N2]
	vset_f_E(&Fl_Array_R[N2+Ltest],0.0f,Ltest);
    vcombine_c_DDD(&Cmplx_Array[N2],&Fl_Array_R[N2],&Fl_Array_R[N2+Ltest],Ltest); 
    ifft(Ltest,&Fl_Array_R[N2],&Fl_Array_I[N2],&Cmplx_Array[N2]);
    vcombine_c_DDP(&Cmplx_Array[N2],&Fl_Array_R[N2],&Fl_Array_I[N2],Ltest); 
	Cmplx_Array[N2+Ltest/2].re = 0.0f;
	Cmplx_Array[N2+Ltest/2].im = 0.0f;
    
	// Scaling of Desired impulse response in &Cmplx_Array[N2]
    vsmul_f_DD((float *)&Cmplx_Array[N2],1.0f/(float)Ltest,(float *)&Cmplx_Array[N2],2*Ltest);  
    
    // Zero filling for interpolation of desired transfer funktion of resulting channel in &Cmplx_Array[N2]
    vcopy_c_DD(&Cmplx_Array[N4-Ltest/2],&Cmplx_Array[N2+Ltest/2],Ltest/2);
	vset_f_E((float *)&Cmplx_Array[N2+Ltest/2],0.0f,2*(N2-Ltest));

    // Desired transfer function of resulting channel in &Fl_Array_R[N2] (real)
	fft(N2,&Fl_Array_R[N2],&Fl_Array_I[N2],&Cmplx_Array[N2]);
    
    // Regularization in &Fl_Array_I[N2]
	vcopy_DE(Fl_Array_R,(float *)&Kabs2_RX[0],N2); //PL
    max=vmax_f_D(Fl_Array_R,N2);
    vsadd_f_PD(&Fl_Array_I[N2],(float)RO*max,Fl_Array_R,N2);
	
	// Desired transfer function of resulting channel devided by
	// regularized squared magnitude of channel transfer function in &Fl_Array_R[N2]
	vdivz_f_DDP(&Fl_Array_R[N2],&Fl_Array_R[N2],&Fl_Array_I[N2],N2);
    
	//...//
	
	// Resulting transfer funqtion of the channel in Fl_Array_R
	vmul_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[N2],N2);
	
	// Building of complex vector from resulting transfer function of the channel in &Cmplx_Array[N2]
	vset_f_P(Fl_Array_I,0.0f,N2);
	vcombine_c_DDP(&Cmplx_Array[N2],Fl_Array_R,Fl_Array_I,N2);
	
    /*
	// Rotation coefficients for equalizer, i = 0,...,N, in Eqlzr 
	//	Fl_Array_R[i]=cosq(OFDM_TWO_PI*(float)(L/2)*(float)i/(float)N);
	//	Fl_Array_I[i]=-sinq(OFDM_TWO_PI*(float)(L/2)*(float)i/(float)N);
	a = OFDM_TWO_PI*(float)(L/2)/(float)N;
	vramp_f_D(Fl_Array_R,0.0f,a,N);
	vramp_f_P(Fl_Array_I,0.0f,-a,N);
	vcos_f_DD(Fl_Array_R,Fl_Array_R,N);
	vsin_f_PP(Fl_Array_I,Fl_Array_I,N);
	vcombine_c_DDP(Eqlzr,Fl_Array_R,Fl_Array_I,N);      

	// Coefficients of equalizer
	vmul_sc_DDD(Eqlzr,1,&Cmplx_Array[N2],2,Eqlzr,1,N);
	
	vcopy_DE(Fl_Array_R,C_RX,N);
	vmul_ccf_DDD(Eqlzr,Eqlzr,Fl_Array_R,N);

	// Scaling of equalizer coefficients to 1st pilot tone
	A.re = Eqlzr[Pilot_ind1].re;
	A.im = Eqlzr[Pilot_ind1].im;
	Af = A.re*A.re + A.im*A.im;
	A.re = A.re/Af;
	A.im = -A.im/Af;
	vsmul_c_DD(Eqlzr,A,Eqlzr,N); 
	
	// Backup of Eqlzr result for M-QAM
	// Eqlzr will be changed during QAM-4 Eqlzr adaptation
	// at the end of QAM-4 and start of M-QAM the initial content
	// Eqlzr_backup will be restored (in QAM_scheduler_RX)
	vcopy_ED((float *)&Eqlzr_backup[0],(float *)&Eqlzr[0],2*N);
	
	// Test for resulting impulse response
	vcopy_c_DD(&Cmplx_Array[N2],Eqlzr,N);
	ifft(N,Fl_Array_R,Fl_Array_I,&Cmplx_Array[N2]);
	*/
	
	// Test for resulting impulse response
	ifft(N2,Fl_Array_R,Fl_Array_I,&Cmplx_Array[N2]);
		
		
	}
	
	
//--------------------------------------------------------------------
//--	OFDM_Optim_NL_validateSIR()	(SIR shorted impulse response)	--
//--	Validation of resulting shorted impluse response   			--
//--	with Fisher Statistiks										--
//--	Input Fields: Fl_Array_R, Fl_Array_I 						--	
//--						- real,imaginary part of resulting IR	--
//--	Input: 	NN 	  - length of prepared input fields				--
//--			Ltest - length of shorted IR (intervall for testing)--
//--	Output: float FS score										--
//--------------------------------------------------------------------
float segment ( "seg_ext_pmco" ) OFDM_Optim_NL_validateSIR(int NN, int Ltest)
	{
	float Asum, Bsum;
	float FS;
	
	if (NN  > 2*OFDM_N_MAX)	return -1.0f;
	if (Ltest > OFDM_L_MAX) return -1.0f;
	
	// squared magnitude of shorted impulse response
	vmag2_uc_DDP(&Fl_Array_R[NN],Fl_Array_R,Fl_Array_I,NN);
	
	// calculate total sum of squares
	Asum = vsum_f_D(&Fl_Array_R[NN],NN);
	
	// calculate sum outside length Ltest
	Bsum = vsum_f_D(&Fl_Array_R[NN+Ltest/2],NN-Ltest);
	
	// calculate weigthed sum inside length Ltest
	Asum = Asum - Bsum;
	/*
	Asum =  vsum(&Fl_Array_R[N2],Ltest/2);
	Asum += vsum(&Fl_Array_R[N2+N2-Ltest/2],Ltest/2);
	Asum = Asum / Ltest;
	*/
	
	// weighted sums
	Asum = Asum / (float)Ltest;
	Bsum = Bsum / (float)(NN-Ltest);
	
	// Apply Fisher Statistiks
	FS = Asum / Bsum;
	
	// return result
	return FS;
	}
	
	
	
//----------------------------------------------------------------
//--	OFDM_Optim_NL_getSTF()	(STF shorted transfer function)	--
//--	Calculation of resulting transfer function   			--
//--	after "shortening" to length L_test		
//--	and calculation of mean square error to estimated
//--	transfer function
//--	Input Field: Kabs2_RX[] of length N2					--
//--					squared magnitude. of channel transf fct--	
//--	Input: Ltest -  length of desired shortening			--
//--	Output Field: --
//----------------------------------------------------------------
float segment ( "seg_ext_pmco" ) OFDM_Optim_NL_getSTF(int Ltest)
	{
	void fft(int, float *, float pm *, complex_float *);
	void ifft(int, float *, float pm *, complex_float *);
	float max;
	float 	f_scale;
	int 	n_pos;
	float   F_NL, F_NL_Z, F_NL_N;
	
	// Restore Squared magnitude of channel transfer function Kabs2_RX
    vcopy_DE(Fl_Array_R,(float *)&Kabs2_RX[0],LFM_Size); //PL
 
	// Square root of squared magnitude of transfer function in &Fl_Array_R[LFM_Size] 
    vsqrt_f_DD(&Fl_Array_R[LFM_Size],Fl_Array_R,LFM_Size);
      
    // Restore Window of Length 2N
    vcopy_DE(Fl_Array_R,(float *)&Win_NLopt_LFMSize[0],LFM_Size); //PL

    
	// Windowing transfer function -> Fl_Array_R
    vmul_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);

	// |Kw(n)| [2N] -> Fl_Array_R[0] -> K_NL_w[0]
	vcopy_ED((float *)&K_NL_w[0],Fl_Array_R,LFM_Size); //PL

	// Get Scaling Value
	n_pos = LFM_Size / 4;
	f_scale =  Fl_Array_R[n_pos];
	
	// Decimation to length Ltest
	vcopy_s_EE(&Fl_Array_R[LFM_Size],1,Fl_Array_R,LFM_Size/Ltest,Ltest);
	
	// Desired impulse response of resulting channel in &Cmplx_Array[LFM_Size]
	vset_f_E(&Fl_Array_R[LFM_Size+Ltest],0.0f,Ltest);
    vcombine_c_DDD(&Cmplx_Array[LFM_Size],&Fl_Array_R[LFM_Size],&Fl_Array_R[LFM_Size+Ltest],Ltest); 
    ifft(Ltest,&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],&Cmplx_Array[LFM_Size]);
    vcombine_c_DDP(&Cmplx_Array[LFM_Size],&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],Ltest); 
	Cmplx_Array[LFM_Size+Ltest/2].re = 0.0f;
	Cmplx_Array[LFM_Size+Ltest/2].im = 0.0f;
    
	// Scaling of Desired impulse response in &Cmplx_Array[LFM_Size]
    vsmul_f_DD((float *)&Cmplx_Array[LFM_Size],1.0f/(float)Ltest,(float *)&Cmplx_Array[LFM_Size],2*Ltest);  
    
    // Zero filling for interpolation of desired transfer funktion of resulting channel in &Cmplx_Array[LFM_Size]
    vcopy_c_DD(&Cmplx_Array[2*LFM_Size-Ltest/2],&Cmplx_Array[LFM_Size+Ltest/2],Ltest/2);
	vset_f_E((float *)&Cmplx_Array[LFM_Size+Ltest/2],0.0f,2*(LFM_Size-Ltest));

    // Desired transfer function of resulting channel in &Fl_Array_R[LFM_Size] (real)
	fft(LFM_Size,&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],&Cmplx_Array[LFM_Size]);
    
    // Regularization in &Fl_Array_I[LFM_Size]
	vcopy_DE(Fl_Array_R,(float *)&Kabs2_RX[0],LFM_Size); //PL
    max=vmax_f_D(Fl_Array_R,LFM_Size);
    vsadd_f_PD(&Fl_Array_I[LFM_Size],(float)RO*max,Fl_Array_R,LFM_Size);
	
	// Desired transfer function of resulting channel devided by
	// regularized squared magnitude of channel transfer function in &Fl_Array_R[LFM_Size]
	vdivz_f_DDP(&Fl_Array_R[LFM_Size],&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],LFM_Size);
    
	//...//
	
	// Resulting transfer function of the channel in Fl_Array_R
	vmul_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);
	
	 /*
	// Building of complex vector from resulting transfer function of the channel in &Cmplx_Array[LFM_Size]
	vset_f_P(Fl_Array_I,0.0f,LFM_Size);
	vcombine_c_DDP(&Cmplx_Array[LFM_Size],Fl_Array_R,Fl_Array_I,LFM_Size);
		
	// Test for resulting impulse response
	ifft(LFM_Size,Fl_Array_R,Fl_Array_I,&Cmplx_Array[LFM_Size]);
	*/	
	
	//--abs?  |K_L(n)| -> Fl_Array_R//
	
    
    // f_scale = Kw(2N/4) / K_L(2N/4); 
    f_scale = f_scale / Fl_Array_R[n_pos];
    
   	// Scaling of STF to |Kw(pilot)|
    vsmul_f_DD(Fl_Array_R,f_scale,Fl_Array_R,LFM_Size); 

   	// |K_L(n)| [2N] -> K_NL_L[0]
	vcopy_ED((float *)&K_NL_L[0],Fl_Array_R,LFM_Size); //PL

    // Restore |Kw(n)| [LFM_Size] -> &Fl_Array_R[LFM_Size]
    vcopy_DE(&Fl_Array_R[LFM_Size],(float *)&K_NL_w[0],LFM_Size); //PL

    // Build  f_scale*|K_L(n)|-|Kw(n)|    -> Fl_Array_R
    vsub_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);
    
	// Build  f_scale*|K_L(n)|-|Kw(n)|    -> Fl_Array_R
    //vsmulvsub_f_DDD(Fl_Array_R,f_scale,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);
    
    // Build SUM ((f_scale*|K_L(n)|-|Kw(n)|)^2)
    F_NL_Z = vsummag2_f_D(Fl_Array_R,LFM_Size);
    
        // Build SUM ((|Kw(n)|)^2)
    F_NL_N = vsummag2_f_D(&Fl_Array_R[LFM_Size],LFM_Size);
    
    F_NL = F_NL_Z / F_NL_N;
    
    return F_NL;

	}
	
	
//----------------------------------------------------------------
//--	OFDM_Optim_NL_validateSTF()	(STF shorted transfer function)	--
//--	Validation of L selection  during starttype II 			--
//--			
//--	calculation of mean square error to estimated
//--	transfer function
//--	Input Field: Kabs2_RX[] of length LFM_Size					--
//--					squared magnitude. of channel transf fct--	
//--				 K_NL_L[] of length LFM_Size						--
//--					magnitude of shorted channel transf fct --
//--	Input: 			--
//--	Output Field: --
//----------------------------------------------------------------
float segment ( "seg_ext_pmco" ) OFDM_Optim_NL_validateSTF()
	{
	float 	f_scale;
	int 	n_pos;
	float   F_NL, F_NL_Z, F_NL_N;
	
	// -- step 1: restore and prepare Kabs2_RX[]
	 
	// Restore Squared magnitude of channel transfer function Kabs2_RX
    vcopy_DE(Fl_Array_R,(float *)&Kabs2_RX[0],LFM_Size); //PL
 
	// Square root of squared magnitude of transfer function in &Fl_Array_R[LFM_Size] 
    vsqrt_f_DD(&Fl_Array_R[LFM_Size],Fl_Array_R,LFM_Size);
      
    // Restore Window of Length 2N
    vcopy_DE(Fl_Array_R,(float *)&Win_NLopt_LFMSize[0],LFM_Size); //PL

    
	// Windowing transfer function -> Fl_Array_R[LFM_Size]
    vmul_f_DDD(&Fl_Array_R[LFM_Size],Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);

	  // |Kw(n)| [2N] -> Fl_Array_R[0] -> K_NL_w[0], save result (debug)
	  vcopy_ED((float *)&K_NL_w[0],&Fl_Array_R[LFM_Size],LFM_Size); 

	// Get Scaling Value (Value on first pilot position)
	n_pos = LFM_Size / 4;
	f_scale =  Fl_Array_R[LFM_Size+n_pos];

	
	// -- step 2: restore and prepare K_NL_L[]

	// restore K_NL_L	
	vcopy_DE(&Fl_Array_R[0],(float *)&K_NL_L[0],LFM_Size);
	
	
	// f_scale = Kw(2N/4) / K_L(2N/4); 
    f_scale = f_scale / Fl_Array_R[n_pos];
    
   	// Scaling of STF to |Kw(pilot)|
    vsmul_f_DD(Fl_Array_R,f_scale,Fl_Array_R,LFM_Size); 

      // |K_L(n)| [2N] -> K_NL_L[0], save scaled result (debug)
	  vcopy_ED((float *)&K_NL_L[0],Fl_Array_R,LFM_Size); 

   
    // Build  f_scale*|K_L(n)|-|Kw(n)|    -> Fl_Array_R
    vsub_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);
    
	// Build  f_scale*|K_L(n)|-|Kw(n)|    -> Fl_Array_R
    //vsmulvsub_f_DDD(Fl_Array_R,f_scale,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);
    
    // Build SUM ((f_scale*|K_L(n)|-|Kw(n)|)^2)
    F_NL_Z = vsummag2_f_D(Fl_Array_R,LFM_Size);
    
        // Build SUM ((|Kw(n)|)^2)
    F_NL_N = vsummag2_f_D(&Fl_Array_R[LFM_Size],LFM_Size);
    
    F_NL = F_NL_Z / F_NL_N;
    
	return F_NL;
	
			
	}

