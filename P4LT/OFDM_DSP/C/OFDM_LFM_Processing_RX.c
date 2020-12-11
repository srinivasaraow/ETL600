/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_LFM_Processing_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Processing of LFM signal
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_LFM_Processing_RX.c $
 * 
 * 7     5.04.06 15:28 Chhamaa
 * 
 * 7     17.03.06 14:41 Chhamaa
 * 
 * 5     7.09.05 11:22 Ats
 * Exchange from TLC
 * 
 * 3     15.02.05 10:02 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 2     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 5     22.12.04 9:30 Chchlee
 * 
 * 4     2.12.04 17:54 Chhamaa
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <trans.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/int_dec_filters/INTDEC_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

extern float K_NL_L[2*OFDM_N_MAX];


//========================================
void segment ( "seg_ext_pmco" ) OFDM_LFM_Processing_RX()
{
void fft(int, float *, float pm *, complex_float *);
void ifft(int, float *, float pm *, complex_float *);
void OFDM_2Tone_Correlation(int, int *);
void OFDM_NB_Interpolation();
float OFDM_Optim_NL_validateSTF();
void OFDM_Restart();

int i;
float a, max;
		
switch ( LFM_Proc_RX )
   	{
   		
   // Note: LFM_Size is 2*N for N=128,256 and 512
   //				and 4*N for N=64 due to doubled length of LFM and RX-Filter
    case 0:
	// Increment wait counter  
   	rx_lfm_in++;	
   	// Detection of LFM signal
	OFDM_2Tone_Correlation(NEG,&LFM_Proc_RX);
	Temp_Buf_ctr = 0;
		
   	break;
      	
   	case 1:
	// Save part of LFM signal in the buffer
   	vcopy_ED((float *)&Temp_Buf[Temp_Buf_ctr*NL],(float *)&Cmplx_Array[0],2*NL);
	// Increment block counter
   	Temp_Buf_ctr++;
   	// Check if a full LFM sequence has been stored
   	if (Temp_Buf_ctr*NL > LFM_Size)	LFM_Proc_RX++;
   	
   	break;
 		
   	case 2:

   	// Save 2nd part of LFM signal in the buffer
 //  	vcopy_ED((float *)&Temp_Buf[NL],(float *)&Cmplx_Array[0],2*N_L);

    // Store reference LFM signal in Cmplx_Array[N2]
 	vcopy_DE((float *)&Cmplx_Array[LFM_Size],(float *)&OFDM_LFM_2_RX[0],2*LFM_Size);
 	
 	// Store spectrum of reference LFM signal in Cmplx_Array[N2]
	fft(LFM_Size,Fl_Array_R,Fl_Array_I,&Cmplx_Array[LFM_Size]);
    vcombine_c_DDP(&Cmplx_Array[LFM_Size],Fl_Array_R,Fl_Array_I,LFM_Size); 
    
 	// Store input LFM signal in Cmplx_Array
 	vcopy_DE((float *)&Cmplx_Array[0],(float *)&Temp_Buf[0],2*LFM_Size);
 	
 	// Store spectrum of input LFM signal in Cmplx_Array
	fft(LFM_Size,Fl_Array_R,Fl_Array_I,Cmplx_Array);
    vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,LFM_Size); 
    
    // Complex transfer function of the channel in Cmplx_Array
    // Spectrum of input LFM signal/Spectrum of reference LFM signal
	vdivz_c_DDD(Cmplx_Array,Cmplx_Array,&Cmplx_Array[LFM_Size],LFM_Size);

	// Store result for Long Status
	vcopy_ED((float *)&Channel_TF[0],(float *)&Cmplx_Array[0],2*LFM_Size);

	// Interpolation of transfer function (as part of NB-Processing)
	if (NB_INTERPOLATION)
		{
		OFDM_NB_Interpolation();
		}
	
	
	// Channel gain for optimization
	vabs_sc_DD(Fl_Array_R,1,Cmplx_Array,LFM_2_N,N);
	vcopy_ED(K_RX,Fl_Array_R,N); 
	
	// Choice of reference Pilot
	if (K_RX[Pilot_ind1] < K_RX[Pilot_ind0])
		{// Switch Reference Pilot index to ensure that
		 // K_RX[Pilot_ind1] > K_RX[Pilot_ind0]
			i = Pilot_ind0;
   			Pilot_ind0 = Pilot_ind1;
   			Pilot_ind1 = i;
		}
	// Consider NB-Interferer in choice of reference pilot
   	if (Mask_NB[Pilot_ind1]==0.0)
   		{// if NB-Interferer on selected pilot
   		 // switch to other pilot
			i = Pilot_ind0;
   			Pilot_ind0 = Pilot_ind1;
   			Pilot_ind1 = i;
   		}
   	// Final Test	
   	if	( (Mask_NB[Pilot_ind1]==0.0) |
   		  (K_RX[Pilot_ind1]*Mask_NB[Pilot_ind1] < K_RX[Pilot_ind0]*Mask_NB[Pilot_ind0]) )
   		{
		Error.Count++;
       	Error.Number[Error.Count].Location = ERLOC_RX_PREAMB;
       	Error.Number[Error.Count].ID       = RX_REF_PILOT_WRN;
		}
	
	// Store complex conjugate transfer funktion in Temp_Buf
    vconjg_c_DD((complex_float *)&Fl_Array_R[0],Cmplx_Array,LFM_Size); 
	vcopy_ED((float *)&Temp_Buf[0],Fl_Array_R,2*LFM_Size);
    
	// Squared magnitude of channel transfer function in Fl_Array_R: |K(n)|^2
    vmul_c_DDD(&Cmplx_Array[LFM_Size],Cmplx_Array,(complex_float *)&Fl_Array_R[0],LFM_Size);
    vreal_c_DD(Fl_Array_R,&Cmplx_Array[LFM_Size],LFM_Size);
    
    // Save Squared magnitude of channel transfer function in Kabs2_RX for NL-Optimization
    vcopy_ED((float *)&Kabs2_RX[0],Fl_Array_R,LFM_Size); //|K(n)|^2
    // result is further used in label 0
    
    // Square root and decimation of squared magnitude of transfer function in &Fl_Array_R[N2] 
    //vsqrt_sf_DD(&Fl_Array_R[LFM_Size],1,Fl_Array_R,LFM_Size/L,L);
    vsqrt_sf_DD(&Fl_Array_R[LFM_Size],1,Fl_Array_R,LFM_Size/L_PROTO,L_PROTO);
  
	// Windowing of decimated transfer function in &Fl_Array_R[N2]
    //vmul_f_DDD(&Fl_Array_R[LFM_Size],Win_Synt,&Fl_Array_R[LFM_Size],L);
    vmul_f_DDD(&Fl_Array_R[LFM_Size],Win_Synt,&Fl_Array_R[LFM_Size],L_PROTO);

	// Desired impulse response of resulting channel in &Cmplx_Array[N2]
	vset_f_E(&Fl_Array_R[LFM_Size+L_PROTO],0.0f,L_PROTO);
    vcombine_c_DDD(&Cmplx_Array[LFM_Size],&Fl_Array_R[LFM_Size],&Fl_Array_R[LFM_Size+L_PROTO],L_PROTO); 
    ifft(L_PROTO,&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],&Cmplx_Array[LFM_Size]);
    vcombine_c_DDP(&Cmplx_Array[LFM_Size],&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],L_PROTO); 
	Cmplx_Array[LFM_Size+L_PROTO/2].re = 0.0f;
	Cmplx_Array[LFM_Size+L_PROTO/2].im = 0.0f;
    
	// Scaling of Desired impulse response in &Cmplx_Array[N2]
    vsmul_f_DD((float *)&Cmplx_Array[LFM_Size],1.0f/(float)L_PROTO,(float *)&Cmplx_Array[LFM_Size],2*L_PROTO);  
    
    // Zero filling for interpolation of desired transfer funktion of resulting channel in &Cmplx_Array[N2]
    vcopy_c_DD(&Cmplx_Array[2*LFM_Size-L_PROTO/2],&Cmplx_Array[LFM_Size+L_PROTO/2],L_PROTO/2);
	vset_f_E((float *)&Cmplx_Array[LFM_Size+L_PROTO/2],0.0f,2*(LFM_Size-L_PROTO));

    // Kz(n): Desired transfer function of resulting channel in &Fl_Array_R[N2] (real)
	fft(LFM_Size,&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],&Cmplx_Array[LFM_Size]);
    
    // label 0
	// Regularization in &Fl_Array_I[N2]: (|K(n)|^2 + RO)
	max=vmax_f_D(Fl_Array_R,LFM_Size);
    vsadd_f_PD(&Fl_Array_I[LFM_Size],(float)RO*max,Fl_Array_R,LFM_Size);
	
	// Desired transfer function of resulting channel devided by
	// regularized squared magnitude of channel transfer function in &Fl_Array_R[N2]
	// Kz(n)/(|K(n)|^2 + RO)
	vdivz_f_DDP(&Fl_Array_R[LFM_Size],&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],LFM_Size);
    // result Fl_Array_R[N2] is further used in "label 1" and "label 2"
	
    // label 1
    // Building of complex vector from desired transfer function of resulting channel in Cmplx_Array
    vset_f_P(&Fl_Array_I[LFM_Size],0.0f,LFM_Size);
    vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],LFM_Size); 
    // Multiplication with complex conjugate channel transfer function in Cmplx_Array
    vmul_c_DED(Cmplx_Array,Temp_Buf,Cmplx_Array,LFM_Size);
    // K*(n)*Kz(n)/(|K(n)|^2 + RO)
    // intermediate result in Cmplx_Array is further used in "label 3"
    
	// label 2
	// Resulting transfer function of the channel in Fl_Array_R
	vmul_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[LFM_Size],LFM_Size);
	
	// |K_L(n)| = Kz(n) * |K(n)|^2 / (|K(n)|^2 + RO)
	// |K_L(n)| [2N] -> K_NL_L[0], save for NL-Optimization: OFDM_Optim_NL_validateSTF()
//	vcopy_ED((float *)&K_NL_L[0],Fl_Array_R,N2); // check for case N=64 
	vcopy_ED((float *)&K_NL_L[0],Fl_Array_R,LFM_Size); // !!

	
	// Building of complex vector from resulting transfer function of the channel in &Cmplx_Array[N2]
	vset_f_P(Fl_Array_I,0.0f,LFM_Size);
	vcombine_c_DDP(&Cmplx_Array[LFM_Size],Fl_Array_R,Fl_Array_I,LFM_Size);
	
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
	vmul_sc_DDD(Eqlzr,1,&Cmplx_Array[LFM_Size],LFM_2_N,Eqlzr,1,N);
	
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
	vcopy_c_DD(&Cmplx_Array[LFM_Size],Eqlzr,N);
	ifft(N,Fl_Array_R,Fl_Array_I,&Cmplx_Array[LFM_Size]);

/*	// Alternative calculation of channel transfer function for optimization
	vabs_c_DD(Fl_Array_R,&Cmplx_Array[N2],N);

	vcopy_DE(&Fl_Array_R[N],Win_TF_RX,N);
	vdivz_f_DDD(Fl_Array_R,Fl_Array_R,&Fl_Array_R[N],N);
	
	//K_max_RX=vmax_f_D(Fl_Array_R,N);   //PL: hier keine Skalierung mit K_max_RX mehr
    // Scale 0.25 ?????!!!!				 //    jetzt alles in QAM4_Data_Preparation	
	//vsmul_f_DD(Fl_Array_R,(float)(0.25f/(double)K_max_RX),Fl_Array_R,N);
	vsmul_f_DD(Fl_Array_R,0.25f,Fl_Array_R,N);  
	vcopy_ED(K_RX,Fl_Array_R,N); 
*/	
	//label 3
	// Multiplication with default transfer funktion of RX filter in Cmplx_Array
    vcopy_DE((float *)&Fl_Array_R[0],(float *)&C_Filter_RX[0],2*LFM_Size); 	  
    vmul_c_DDD(Cmplx_Array,Cmplx_Array,(complex_float *)&Fl_Array_R[0],LFM_Size);

	// Impulse response of RX filter
	ifft(LFM_Size,Fl_Array_R,Fl_Array_I,Cmplx_Array);
	vcombine_c_DDP(&Cmplx_Array[LFM_Size],Fl_Array_R,Fl_Array_I,LFM_Size);
	
	// Magnitude of impulse response of RX filter
	vabs_c_DD(&Fl_Array_R[LFM_Size],&Cmplx_Array[LFM_Size],LFM_Size);

	// Index of maximum element of Fl_Array_R
	Max = vmaxindex_f_D(&Fl_Array_R[LFM_Size],LFM_Size);
	
	// **** Symbol synchronization ****
	if (use_long_RX_Filter_2NmL)
		Symbol_Synchro = -2*L-L/2-Predd-Max+N;// for N=64 (RX imp resp is N samples longer)
	else
		Symbol_Synchro = -2*L-L/2-Predd-Max; // standard
    	
	if(Symbol_Synchro < 0)	Symbol_Synchro += 4*NL;
	Symbol_Synchro=(Symbol_Synchro%NL); //modulo operation
	
	// Symbol_Synchro is related to sampling frequency FN
	// However the block synchronisation is executed in 64kHz domain.
	// Therfore the value of Symbol_Synchro has to be interpolated to 64kHz
	// 
	// Number of samples at FS=64kHz sampling domain
	// -> Symbol_Synchro_FS = (int)((float)(Symbol_Synchro * Filter_TX_Interpolation) * SFC_Interpolation);

	// For FN=6,12 and 24 there is a divider of 3 in Interpolation factor
	// As a result, it might occur, that Symbol_Synchro_FS is not integer
	// The solution is, to modify the value Predd in these special cases
	if ( (FN==12000)|(FN==24000)|(FN==6000)|(FN==4800)|(FN==9600) )
		{
		// Modify Predd	
		Predd +=(Symbol_Synchro%3);
		// Recalculation of Symbol synchronization
    	if (use_long_RX_Filter_2NmL)
			Symbol_Synchro = -2*L-L/2-Predd-Max+N;// for N=64 (RX imp resp is N samples longer)
		else
			Symbol_Synchro = -2*L-L/2-Predd-Max; // standard
   	    	
		if(Symbol_Synchro < 0)	Symbol_Synchro += 4*NL;
		Symbol_Synchro=(Symbol_Synchro%NL); //modulo operation
		}

	// Number of samples at FS=64kHz sampling domain	
	Symbol_Synchro_FS = (int)((float)(Symbol_Synchro * Filter_TX_Interpolation) * SFC_Interpolation);
	Symbol_Synchro_FS_rem = Symbol_Synchro_FS; // for debug only, to remind the result

	// Number of samples at 2*FN
	   // Symbol_Synchro=Symbol_Synchro*2;
	// Number of samples at Filter_TX_Interpolation*FN (output filter)
	Symbol_Synchro=Symbol_Synchro*Filter_TX_Interpolation;

	// **** RX-Filter ****
	// Index for cyclic permutation of impulse response of RX filter
	tmp_ind = (Max - (LFM_Size/2-L-Predd));
	
	if(tmp_ind < 0) tmp_ind += LFM_Size;
	
	perm_ind = tmp_ind % LFM_Size;
		
	// Cyclic permutation of impulse response of RX filter 
	vset_f_D(&Fl_Array_R[LFM_Size],0.0f,LFM_Size);
	vset_f_P(&Fl_Array_I[LFM_Size],0.0f,LFM_Size);
	for (i=0;i<LFM_Size/2-L;i++)
		{
		if(perm_ind == LFM_Size) perm_ind=0;
		Fl_Array_R[i+LFM_Size]=Fl_Array_R[perm_ind];
		Fl_Array_I[i+LFM_Size]=Fl_Array_I[perm_ind];
		perm_ind++;
		}
	vcombine_c_DDP(Cmplx_Array,&Fl_Array_R[LFM_Size],&Fl_Array_I[LFM_Size],LFM_Size);

	// Scaling of impulse response of RX Filter
	b = 1.0f/(float)LFM_Size;
	vsmul_f_DD((float *)Cmplx_Array,b,(float *)Cmplx_Array,2*(LFM_Size/2-L));

	// Transfer function of RX Filter
    vset_f_E((float *)&Cmplx_Array[LFM_Size/2-L],0.0f,2*(LFM_Size/2+L));
	vcopy_ED((float *)&Imp_Filter_RX[0],(float *)&Cmplx_Array[0],2*LFM_Size);
	// New transfer function of RX Filter in &Cmplx_Array[N2]
	fft(LFM_Size,Fl_Array_R, Fl_Array_I, Cmplx_Array);
    vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,LFM_Size); 	  
    vcopy_ED((float *)&C_Filter_RX[0],(float *)&Cmplx_Array[0],2*LFM_Size); 	  
    vcopy_ED((float *)&C_Filter_RX_fallback[0],(float *)&Cmplx_Array[0],2*LFM_Size); 	  

	// Squared transfer function of RX filter
	vabs_sc_DD(Fl_Array_R,1,Cmplx_Array,LFM_2_N,N);
    vmul_f_DDD(Fl_Array_R,Fl_Array_R,Fl_Array_R,N);
	vcopy_ED(Abs2_Filter_RX,Fl_Array_R,N); 	  
	
    
   	LFM_Proc_RX++;
   	
   	break;

   	case 3:
   	// Reset inner State machine	
    LFM_Proc_RX=0;
        
    // Set Parent State Machine Flag
	rx_state_finished = 1;	
	
	/*
	// Debug for NL Optimization
	// after 2nd restart make Reset in state Optim_NL_Determination
	if ( Restart_Counter == 2 ) {
	Optim_NL_State = Optim_NL_ON_Determination;
	// Start with N and L base values for NL-Optimization
			N = N_BASE;	
			L = L_BASE;
	OFDM_Restart();
	}
   	*/
   	// NL-Optimization
   	// validate selected L, if not sufficient, prepare restart
   	// next function uses K_NL_L[] and Kabs2_RX[]
   	FS_Score_L = OFDM_Optim_NL_validateSTF();
   	
   	if (Optim_NL_State == Optim_NL_ON_Selected)
   		{
   		// if value of FS_Score_L exceeds FS_SCORE_L_LIMIT, then L was not sufficient
   		// now how to set FS_SCORE_L_LIMIT??
		FS_SCORE_L_LIMIT = 0.02f; // later this should be a define
   			
   		if (FS_Score_L > FS_SCORE_L_LIMIT)
   			{
   			// restart with Starttyp I
    		Optim_NL_State =  Optim_NL_ON_Determination;
    		// Start with N and L base values for NL-Optimization
			N = N_BASE;	
			L = L_BASE;
    		OFDM_Restart();    
   			}
   		}
   	break;
   	}
}


