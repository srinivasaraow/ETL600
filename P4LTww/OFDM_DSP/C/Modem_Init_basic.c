/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Modem_Init_basic.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Initialize OFDM modem basic settings
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Modem_Init_basic.c $
 * 
 * 21    29.08.08 15:31 Chhamaa
 * 
 * 17    6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 16    21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 14    17.03.06 14:41 Chhamaa
 * 
 * 12    16.09.05 9:41 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 7     17.05.05 17:07 Chchlee
 * OFDM_Alarm.error --> OFDM_Alarm.configAlm
 * 
 * 6     26.04.05 8:42 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 5     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 4     24.02.05 7:27 Chchlee
 * 
 * 2     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 8     2.12.04 17:54 Chhamaa
 * 
 * 6     1.10.04 12:46 Leeb02
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * Split basic part into own file
 * introduced NB and optimization section
 * introduced state machine
 *		 06.04.04 PL
 * V11 integration
 *       11.05.04 vgp
 * Random Delta bit vector generation support for line operating
 *       12.05.04 vgp
 * SFC-Filter Initialization
 *		 22.06.04 PL
 * SFC to TDM integration
 *       26.06.04 vgp
 * New Bandwidth initialization
 *	   27.06.04 PL
 * Modified Frequency Offset init
 *		7.7.04 PL
*****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_Defs.h"
#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/util/util.h"
#include "../h/TDM/ofdm_tdm_externs.h"

#include "../h/int_dec_filters/INTDEC_externs.h"
#include "../h/int_dec_filters/INTDEC_filt_c.h"
#include "../h/tx_rx_filters/TXRX_FILT_externs.h"
#include "../h/v11/v11_externs.h"

extern int Delay_Measure_Flag;
extern int giEmergency_restart;
//float Xmax, a, b, c, d;

void segment ( "seg_ext_pmco" ) Modem_Init_basic()
	{
    void  fft(int, float *, float pm *, complex_float *);
	void  TCM_MQAM_Train_Gen_Init();
	void  V11_Init();
    void  TCM_Delta_Gen_Init();
    void  OFDM_Fall_BackForward_INIT();
    void  OFDM_Filter_RX_Watchdog(float , int );
    void  Phase_Control_Init(void);
    void  Init_Delay_Restart(void);
    void  PreambleTX2Tone_Init(int index0, int index1);



    //void  Test_FIR_init();  // PL Test

	
    int   i;
	float *tx_filter_ptr;
	float *rx_filter_ptr;
	float m;
		
	Shaping_Flag = ON; // Shaping ON or OFF
		Shaping_Flag_tx = Shaping_Flag; 
		Shaping_Flag_rx = Shaping_Flag; 

	T_OFDM_symbol = (float)(N+L)/(float)FN;
	Fbin = (float)FN/(float)N;
	
	N2 = N*2;
	N4 = N*4;
	NL = N+L;
	NL2 = 2*NL;
	N_L = N-L;
	N_N_8 = N-N/8; // Length of IR of TX and RX Filters
	N_pil = 2;
	N_tone = 2;
	
	L_PROTO = L;
	if (L==7) L_PROTO=8;
	
	
	switch (Optim_NL_State)
	{
		case Optim_NL_OFF:
		 	// N and L was set by ORTH_Mode and GUARD_Int
		 	// default pilot position
		 	Tone_ind0  = PILOT0_DEFAULT;
			Tone_ind1  = PILOT1_DEFAULT;
			Pilot_ind0 = PILOT0_DEFAULT;
			Pilot_ind1 = PILOT1_DEFAULT;
			
			/*
			// spezielle Töne für den Start der EP (Master und Slave unterschiedlich)
			if (OFDM_Modem_Type==OFDM_MASTER) {
				Spilot_TX_0 = PILOT1_MIDDLE;
				Spilot_TX_1 = PILOT1_NL_OPT;
				Spilot_RX_0 = PILOT0_NL_OPT; // muss sein == Slave's Spilot_TX_0
				Spilot_RX_1 = PILOT1_MIDDLE; // muss sein == Slave's Spilot_TX_1
				
			}
			if (OFDM_Modem_Type==OFDM_SLAVE ) {
				Spilot_TX_0 = PILOT0_NL_OPT;
				Spilot_TX_1 = PILOT1_MIDDLE;
				Spilot_RX_0 = PILOT1_MIDDLE;// muss sein == Master's Spilot_TX_0
				Spilot_RX_1 = PILOT1_NL_OPT;// muss sein == Master's Spilot_TX_1
			}
			*/

			/*
			// spezielle Töne für den Start der EP (Master und Slave gleich)
			if (OFDM_Modem_Type==OFDM_MASTER) {
				Spilot_TX_0 = PILOT0_NL_OPT;
				Spilot_TX_1 = PILOT1_NL_OPT;
				Spilot_RX_0 = PILOT0_NL_OPT; // muss sein == Slave's Spilot_TX_0
				Spilot_RX_1 = PILOT1_NL_OPT; // muss sein == Slave's Spilot_TX_1
				
			}
			if (OFDM_Modem_Type==OFDM_SLAVE ) {
				Spilot_TX_0 = PILOT0_NL_OPT;
				Spilot_TX_1 = PILOT1_NL_OPT;
				Spilot_RX_0 = PILOT0_NL_OPT;// muss sein == Master's Spilot_TX_0
				Spilot_RX_1 = PILOT1_NL_OPT;// muss sein == Master's Spilot_TX_1
			}
			*/

			// default Töne für den Start der EP (Master und Slave gleich)
				Spilot_TX_0 = PILOT0_DEFAULT;
				Spilot_TX_1 = PILOT1_DEFAULT;
				Spilot_RX_0 = PILOT0_DEFAULT; 
				Spilot_RX_1 = PILOT1_DEFAULT; 
				
			
			
			// For very low number of active carriers Mmax
			// and high FN, put pilots closer together
			//obsolete
			/*
			if (BW4_8_Mode==high_FN)
			{
				if ( ((BW==3500)&(FN== 6000))
				   | ((BW==7500)&(FN==12000))
				   )
				{
				Tone_ind0  = PILOT0_NL_OPT;
				Tone_ind1  = PILOT1_NL_OPT;
				Pilot_ind0 = PILOT0_NL_OPT;
				Pilot_ind1 = PILOT1_NL_OPT;
				}
			}
			*/


		break;
		case Optim_NL_ON_Determination:
			// Optim-NL in determination state
			// special pilot position
			Tone_ind0  = PILOT0_NL_OPT;
			Tone_ind1  = PILOT1_NL_OPT;
			Pilot_ind0 = PILOT0_NL_OPT;
			Pilot_ind1 = PILOT1_NL_OPT;
			break;
		case Optim_NL_ON_Selected:
			// Optim-NL in NL-selected state
			// default pilot position
			Tone_ind0  = PILOT0_DEFAULT;
			Tone_ind1  = PILOT1_DEFAULT;
			Pilot_ind0 = PILOT0_DEFAULT;
			Pilot_ind1 = PILOT1_DEFAULT;
			break;
		
		default:
			Error.Count=999;
			exit(1);
			break;
	}
	
	
	half_log2 = 0.5f / log10f(2.0f);  // to generate 1/2*log2() in optimx()


    // Shared pointers initialization
    Cmplx_Array = (complex_float *)&Cmplx[0];
    OFDM_Symbol_TX = (complex_float *)&Tx_Array[0];
    OFDM_Symbol_RX=(complex_float *)&Rx_Array[0];

    Compl_Exp=(complex_float *)&Fl_Array_R[0];
    		
	Ph_Pil0=&Fl_Array_R[LEN_FFT_DF_EST];
//	Ph_Pil1=&Fl_Array_R[LEN_FFT_DF_EST+((NL*LEN_DF_EST)/LEN_FFT_DF_EST)];
	Ph_Pil1=&Fl_Array_R[LEN_FFT_DF_EST+((NL*N_OFDM_SYMBOLS_PER_SUBBLOCK)/LEN_FFT_DF_EST)];

	// Signal Scaling
	//=================================================================================================
		
	// Scaling Coefficient
	Norm_TX = sqrtf(2.0f); // sqrt(Interpolation Factor)!!!
	
	Delta_TX = 1.0f;
	
	P_OFDM_TX = 1.0f;
	
	// TWO Tones scaling
	Amp_Tone_TX = A_2TONE*sqrt((float)N/(float)OFDM_N_MAX);

	// TWO Tone phase init and phase rotation init (necessary for L==7 only)
//	PreambleTX2Tone_Init(Tone_ind0, Tone_ind1);
	PreambleTX2Tone_Init(Spilot_TX_0, Spilot_TX_1);
    /*
	tx2tone0_phase.re = 1.0f;
	tx2tone0_phase.im = 0.0f;
	tx2tone1_phase.re = 1.0f;
	tx2tone1_phase.im = 0.0f;
	
	tx2tone0_phaserot.re = cosf(OFDM_TWO_PI*(float)Tone_ind0*(float)L/(float)N);
	tx2tone0_phaserot.im = sinf(OFDM_TWO_PI*(float)Tone_ind0*(float)L/(float)N);
	tx2tone1_phaserot.re = cosf(OFDM_TWO_PI*(float)Tone_ind1*(float)L/(float)N);
	tx2tone1_phaserot.im = sinf(OFDM_TWO_PI*(float)Tone_ind1*(float)L/(float)N);
	*/
	
	// LFM scaling
	Amp_LFM_TX = A_LFM*sqrtf((float)N);//*4.0f;
	
	// Reset primary AGC setting
  	Analog_Gain = 1.0f;
  	
	//Des_Val_Pow_Pil = (A_2TONE*A_2TONE*OFDM_PILOTS_NUM)/(OFDM_PILOTS_NUM*DET_WIN_LEN)*
	//                  ((float)N/(float)OFDM_N_MAX);
	Des_Val_Pow_Pil = Amp_Tone_TX*Amp_Tone_TX/((float)DET_WIN_LEN);

	// Reset of digital AGC
    Digital_AGC = 1.0f;

    // Treshhold for Fisher statistics detection
    Detection_Treshhold = 40;

    // Init reference signals for 2 tone signal correlation
	Rot_Pilot0.re = cosf(OFDM_TWO_PI*(float)Pilot_ind0/(float)N);
	Rot_Pilot0.im = sinf(OFDM_TWO_PI*(float)Pilot_ind0/(float)N);

	Rot_Pilot1.re = cosf(OFDM_TWO_PI*(float)Pilot_ind1/(float)N);
	Rot_Pilot1.im = sinf(OFDM_TWO_PI*(float)Pilot_ind1/(float)N);

	// Init Ref Pilot Backup (used in OFDM_engine_RX)
	Pilot1_last_RX.re=0.0f;
	Pilot1_last_RX.im=0.0f;

	
	// Init SFC-Filter Interpolation-Functions
	//================================================================================================
	
	// Init SFC-Filter runtime memory sections
	IntDec_Filt_Coeff =(complex_float *)&Fl_Array_I[0]; //PL2106
		//IntDec_Filt_Delay =(complex_float *)&Fl_Array_R[12*OFDM_NL_MAX-8*OFDM_N_MAX]; //PL2106
	// Runtime SFC-Filter Delay Line is positioned at tail of Fl_Array_R[]
		// Size of Fl_Array_R: 4*OFDM_NL_MAX+4*OFDM_N_MAX = 4352 words
		// Maximum Size of SFC-Filter Delay Line: 2*(25+1) = 52 words
		// Start Adress is 4352-52 = 4300
	IntDec_Filt_Delay =(complex_float *)&Fl_Array_R[4300]; //PL 3.1.2006
		// debug test if pointers ok	
		//vcopy_PE((pm float *)IntDec_Filt_Coeff,(float *)&Int2x12_Filt_Coeff[0],2*INTFILT_2_LENGTH);
	
	// Init Default Value for TX-Filter Interpolation, RX-Filter Decimation
	Filter_TX_Interpolation = 2;
	Filter_RX_Decimation = 2;
	
	switch (FN)
	{
		case 2000: //nur für N<=128
			Filter_TX_Interpolation = 8;
			Filter_RX_Decimation = 8;
			SFC_Interpolation = 4.0f;
			
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    		Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
				
		break;
		case 4000:
			
			if (N>128) // for N=256 and N=512
			{
			Filter_TX_Interpolation = 2;
			Filter_RX_Decimation = 2;
			SFC_Interpolation = 8.0f;
			
			// Init interpolation filter in polyphase structure
    		Int2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
			// Init decimation filter
    		Dec2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    		Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
			}
			else // for N=64 and N=128
			{
			Filter_TX_Interpolation = 8;
			Filter_RX_Decimation = 8;
			SFC_Interpolation = 2.0f;
			
			// Init interpolation filter in polyphase structure
    		Int2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
			// Init decimation filter
    		Dec2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
			}
			
		break;
		
		case 4800:
			if (N<=128)  //only used for N<=128 up to now
			{
			Filter_TX_Interpolation=8;
			Filter_RX_Decimation = 8;
			SFC_Interpolation = 5.0f/3.0f;
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
						
			// Init interpolation filter in polyphase structure
    		Int5x25_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int5x25_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
			
			}
		
		break;
		
		
		case 6000:
			switch (N)
			{
			case 64:
			case 128:
			Filter_TX_Interpolation=8;
			Filter_RX_Decimation = 8;
			SFC_Interpolation = 4.0f/3.0f;
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
						
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    	//	Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
		//	vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
			break;
	
			case 256:
			Filter_TX_Interpolation=4;
			Filter_RX_Decimation = 4;
			SFC_Interpolation = 8.0f/3.0f;
			// Init interpolation filter in polyphase structure
    		Int2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
			// Init decimation filter
    		Dec2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
						
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    		Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
			break;
	
			case 512:
			Filter_TX_Interpolation = 2;
			Filter_RX_Decimation    = 2;
			SFC_Interpolation = 16.0f/3.0f;
			
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
						
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
			vcopy_ED((float *)&Int4x24_filt_c_Delay1[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    		Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
			
			break;	
			}
			
		break; //case 6000
		
		case 8000:
			SFC_Interpolation = 4.0f;
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    		Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
	
		break;

		case 9600:		 //BW 7.5 and BW 8.0 LowDelay
			if (N<=128)  //only used for N<=128 up to now
			{
			Filter_TX_Interpolation=4;
			Filter_RX_Decimation = 4;
			SFC_Interpolation = 5.0f/3.0f;
//			SFC_Interpolation = 5.0f;
//			NL_SFC = 90;
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
						
			// Init interpolation filter in polyphase structure
    		Int5x25_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int5x25_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
			
			}
		break;

		
		case 12000:
			SFC_Interpolation = 8.0f/3.0f;
			// Init interpolation filter in polyphase structure
    		Int2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
	
			// Init decimation filter
    		Dec2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
						
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init decimation filter
    		Dec4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_4_LENGTH+1));
	
		
		break;
		
		case 16000:
			SFC_Interpolation = 2.0f;
			// Init interpolation filter in polyphase structure
    		Int2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1));
			
			// Init decimation filter
    		Dec2x12_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Dec2x12_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(DECFILT_2_LENGTH+1));
	
		break;
		
		case 24000:
			SFC_Interpolation = 4.0f/3.0f;
			
			// Init interpolation filter in polyphase structure
    		Int4x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int4x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1));
		
			// Init interpolation filter in polyphase structure
    		Int3x24_filt_c_init(IntDec_Filt_Delay);
			// Save content of Delay Array to external Memory
			vcopy_ED((float *)&Int3x24_filt_c_Delay[0],(float *)IntDec_Filt_Delay,2*(INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1));
			
		
		break;
		
		case 32000:
			SFC_Interpolation = 1.0f;
		break;
	}





	// Init of transfer functions of RX and TX filters
	//================================================================================================
	use_long_RX_Filter_2NmL = 0; // init switch for long RX-Filter
								 // 0: length N-L,  1: length 2N-L
	switch (BW)
		{
		case 32000:
		case 24000:
		case 16000:
		case 12000:
//		case  8000:
//		case  6000:
//		case  4000:
		
		switch (N)
			{
    		case 64:
    			use_long_RX_Filter_2NmL=1;
			//for N=64, we have special version of RX-Filters
    		// separately for both cases L=8 and L=16
    		// these filters have doubled length (2*N-L+1)=(128-L+1)
    		// instead of (N-L+1)
    		if (L==8)
    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
    		if (L==16)
    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_16[0];
    		
			tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
    		break;
    		
			case 128:
			tx_filter_ptr = (float *)&H_fn4_bw4_128_tx[0];
			rx_filter_ptr = (float *)&H_fn4_bw4_128_rx[0];
			//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
			break;
    		
    		case 256:
			//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
			rx_filter_ptr = (float *)&H_fnxx_bwxx_256_rx[0];
			tx_filter_ptr = (float *)&H_fnxx_bwxx_256_tx[0];
    		break;
    		
    		case 512: // not allowed for FN = 6000 (which uses FFT-8N)
			rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
			tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
    		break;
    		
    		default:
    		
    		break;
			}
		break;
		
		case 2000:
			switch (N)
				{
    			case 64:
					use_long_RX_Filter_2NmL=0;
					tx_filter_ptr = (float *)&H_fn4_bw35_64_tx[0];
					rx_filter_ptr = (float *)&H_fn4_bw35_64_tx[0];
    			break;
    			case 128:
    				//tx_filter_ptr = (float *)&H_fn4_bw4_128_tx[0];
					//rx_filter_ptr = (float *)&H_fn4_bw4_128_rx[0];
					tx_filter_ptr = (float *)&H_fn4_bw35_128_tx[0];
					rx_filter_ptr = (float *)&H_fn4_bw35_128_rx[0];
					//rx_filter_ptr = (float *)&H_Filter_RX_28_128[0];
					//tx_filter_ptr = (float *)&H_Filter_TX_28_128[0];

    			break;
    			case 256:
    			case 512:
    			// not allowed
    			//while(1); // debug 2008
    			break;
				}
		break;
		
		case 4000:
			switch (BW4_8_Mode)
			{
			case std_FN: // FN= 4000
				switch (N)
				{
    			case 64: //not used anymore
						use_long_RX_Filter_2NmL=1;
    			//for N=64, we have special version of RX-Filters
    			// separately for both cases L=8 and L=16
    			// these filters have doubled length (2*N-L+1)=(128-L+1)
    			// instead of (N-L+1)
    			if (L==8)
	    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
	    		if (L==16)
	    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_16[0];
	    		
				tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
	    		break;
	    		
				case 128: // 4kHz HD FN=4000
				//H_fn4_bw4_128_tx (h_fn4_bw4_128_tx1.dat)
				//H_fn4_bw4_128_rx (h_fn4_bw4_128_tx1.dat)
				tx_filter_ptr = (float *)&H_fn4_bw4_128_tx[0];
				rx_filter_ptr = (float *)&H_fn4_bw4_128_rx[0];
				//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
				break;
	    		
	    		case 256:
				//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
				rx_filter_ptr = (float *)&H_fnxx_bwxx_256_rx[0];
				tx_filter_ptr = (float *)&H_fnxx_bwxx_256_tx[0];
	    		break;
	    		
	    		case 512:
				rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
	    		break;
	    		
	    		default:	    		
	    		break;
				}
			break;
			case high_FN: // FN= 6000  not used anymore
				switch (N)
				{
	    		case 64:
						use_long_RX_Filter_2NmL=1;
	    		//for N=64, we have special version of RX-Filters
	    		// separately for both cases L=8 and L=16
	    		// these filters have doubled length (2*N-L+1)=(128-L+1)
	    		// instead of (N-L+1)
	    		if (L==8)
	    		{
	    			//rx_filter_ptr = (float *)&H_Filter_RX_8_128_8[0];
	    			//rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
	    			rx_filter_ptr = (float *)&H_fn6_bw4_64_rx128[0];
				}
	    		    		
				//tx_filter_ptr = (float *)&H_Filter_TX_8_64[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];  //a real wide tx-filter
				//			tx_filter_ptr = (float *)&H_fn6_bw4_64_tx[0];
	    		break;
	    		
				case 128:
				rx_filter_ptr = (float *)&H_Filter_RX_8_128[0];
				tx_filter_ptr = (float *)&H_Filter_TX_8_128[0];
				break;
	    		
	    		case 256:
				rx_filter_ptr = (float *)&H_Filter_RX_8_256[0];
				tx_filter_ptr = (float *)&H_Filter_TX_8_256[0];
	    		break;
	    		
	    		// N=512 is not allowed for FN=6000
    			//case 512:
				//rx_filter_ptr = (float *)&H_Filter_RX_8_512[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_8_512[0];
	    		//break;
	    		
	    		default:	    		
	    		break;
				}
			break;
			}
			if (FN==4800)
			{
			switch (N)
				{
	    		case 64:  //---- used for BW 4.0kHz LD N=64 FN=4800 ---- short RX Filter
					if (0) //1 long Filter, 0 short Filter
					{
						use_long_RX_Filter_2NmL=1;
		    		//for N=64, we have special version of RX-Filters
		   			// separately for both cases L=8 and L=16
					// these filters have doubled length (2*N-L+1)=(128-L+1)
	    			// instead of (N-L+1)
	    			if (L==8) //N/L=64/16 not allowed for FN=4800
	    				rx_filter_ptr = (float *)&H_fn48_bw4_64_rx128[0];
					} else
					{
				use_long_RX_Filter_2NmL=0;
				rx_filter_ptr = (float *)&H_fn48_bw4_64_tx[0];
					}
				//tx_filter_ptr = (float *)&H_fn48_bw4_64_tx[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];  //a real wide tx-filter

	    		break;
	    		
				case 128: // not used
				//rx_filter_ptr = (float *)&H_fn48_bw35_128_tx[0];
				//tx_filter_ptr = (float *)&H_fn48_bw35_128_tx[0];
				break;
				}
			}
		break;
		
		case  3500:
		switch (BW4_8_Mode)
			{
			case std_FN: // FN= 4000
			switch (N)
				{
	    		case 64: // used for BW 3.5kHz LD N=64 FN=4000
				/*
						use_long_RX_Filter_2NmL=1;
	    		//for N=64, we have special version of RX-Filters
	    		// separately for both cases L=8 and L=16
	    		// these filters have doubled length (2*N-L+1)=(128-L+1)
	    		// instead of (N-L+1)
	    		if (L==8)
	    			rx_filter_ptr = (float *)&H_Filter_RX_4r_128_8[0];
	    		if (L==16)
	    			rx_filter_ptr = (float *)&H_Filter_RX_4r_128_16[0];
	    		
				tx_filter_ptr = (float *)&H_Filter_TX_4r_64[0];
				//rx_filter_ptr = (float *)&H_Filter_RX_32_64[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
	    		*/
				use_long_RX_Filter_2NmL=0;
				tx_filter_ptr = (float *)&H_fn4_bw35_64_tx[0];
				rx_filter_ptr = (float *)&H_fn4_bw35_64_tx[0];
				
	    		break;
	    		
				case 128: // used for BW 3.5kHz HD N=128 FN=4000
				tx_filter_ptr = (float *)&H_fn4_bw35_128_tx[0];
				rx_filter_ptr = (float *)&H_fn4_bw35_128_rx[0];
				//rx_filter_ptr = (float *)&H_Filter_RX_4r_128[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_4r_128[0];
				//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
				break;
				
				case 256: // used for BW 3.5kHz HD N=256 FN=4000
				//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
				rx_filter_ptr = (float *)&H_fnxx_bwxx_256_rx[0];
				tx_filter_ptr = (float *)&H_fnxx_bwxx_256_tx[0];
	    		
	    		break;
	    		
	    		case 512: // used for BW 3.5kHz HD N=512 FN=4000
				rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
	    		break;
				}
			break;
			case high_FN:  // FN= 6000 not used anymore
			switch (N)
				{
	    		case 64:
						use_long_RX_Filter_2NmL=1;
	    		//for N=64, we have special version of RX-Filters
	    		// separately for both cases L=8 and L=16
	    		// these filters have doubled length (2*N-L+1)=(128-L+1)
	    		// instead of (N-L+1)
	    		if (L==8)
//	    			rx_filter_ptr = (float *)&H_Filter_RX_8_128_8[0];
	    			//rx_filter_ptr = (float *)&H_Filter_RX_4r_fn6_128_8[0];
	    			rx_filter_ptr = (float *)&H_fn6_bw35_64_rx128[0];
	    		    		
				//tx_filter_ptr = (float *)&H_Filter_TX_8_64[0];
//				tx_filter_ptr = (float *)&H_Filter_TX_4r_fn6_64[0];
				tx_filter_ptr = (float *)&H_fn6_bw35_64_tx[0];
	    		break;
	    		
				case 128:
				rx_filter_ptr = (float *)&H_Filter_RX_8_128[0];
				tx_filter_ptr = (float *)&H_Filter_TX_8_128[0];
				break;
	    		
	    		case 256:
				rx_filter_ptr = (float *)&H_Filter_RX_8_256[0];
				tx_filter_ptr = (float *)&H_Filter_TX_8_256[0];
	    		break;
	    		
	    		// N=512 is not allowed for FN=6000
    			//case 512:		
				//rx_filter_ptr = (float *)&H_Filter_RX_8_512[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_8_512[0];
	    		//break;
	    		
	    		default:	    		
	    		break;
				}
			break;
			} //switch BW4_8_Mode
			if (FN==4800)            // not used anymore for BW 3.5
			{
			switch (N)
				{
	    		case 64:	//PL240106 for testing only, to remove later
							// TimeOut Switch is used to selected single or double RX-Filter
					if (0) // 1(1.5) long Filter, 0(0.3) short Filter
					{
							use_long_RX_Filter_2NmL=1;
	    			//for N=64, we have special version of RX-Filters
	    			// separately for both cases L=8 and L=16
	    			// these filters have doubled length (2*N-L+1)=(128-L+1)
	    			// instead of (N-L+1)
	    			if (L==8) //N/L=64/16 not allowed for FN=4800
		    			rx_filter_ptr = (float *)&H_fn48_bw35_64_rx128[0];
					} else
					{
							use_long_RX_Filter_2NmL=0;
							rx_filter_ptr = (float *)&H_fn48_bw35_64_tx[0];
					}
				tx_filter_ptr = (float *)&H_fn48_bw35_64_tx[0];
//				tx_filter_ptr = (float *)&H_fn48_bw4_64_tx[0];
	    		break;
	    		
				case 128:
				//rx_filter_ptr = (float *)&H_fn48_bw35_128_tx[0];
				//tx_filter_ptr = (float *)&H_fn48_bw35_128_tx[0];
				break;
				}
			}
		break;
		
		case  6000:
		switch (N)
			{
    		case 64:
    		/*
    		use_long_RX_Filter_2NmL=1;
			//for N=64, we have special version of RX-Filters
    		// separately for both cases L=8 and L=16
    		// these filters have doubled length (2*N-L+1)=(128-L+1)
    		// instead of (N-L+1)
    		if (L==8)
    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
    		if (L==16)
    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_16[0];
    		
			tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
			*/
			use_long_RX_Filter_2NmL=0;
			tx_filter_ptr = (float *)&H_fn6_bw6_64_tx[0];
			rx_filter_ptr = (float *)&H_fn6_bw6_64_rx[0];
    		break;
    		
			case 128:
			tx_filter_ptr = (float *)&H_fn6_bw6_128_tx[0];
			rx_filter_ptr = (float *)&H_fn6_bw6_128_rx[0];
			//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
			break;
    		
    		case 256:
			//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
			rx_filter_ptr = (float *)&H_fn6_bw6_256_rx[0];
			tx_filter_ptr = (float *)&H_fn6_bw6_256_tx[0];
    		break;
    		
    		case 512: 
			//rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
    		rx_filter_ptr = (float *)&H_fn6_bw6_512_rx[0];
			tx_filter_ptr = (float *)&H_fn6_bw6_512_tx[0];
    		break;
    		
    		default:
    		
    		break;
			}
		break;

		case 8000:
			switch (BW4_8_Mode)
			{
			case std_FN:  //used for HighDelay, possible are all N 128..512
			switch (N)	  // 	right now for N=64, high_FN is used			
				{
    			case 64: // not used
						use_long_RX_Filter_2NmL=1;
    			//for N=64, we have special version of RX-Filters
    			// separately for both cases L=8 and L=16
    			// these filters have doubled length (2*N-L+1)=(128-L+1)
    			// instead of (N-L+1)
    			if (L==8)
	    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
	    		if (L==16)
	    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_16[0];
	    		
				tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
	    		break;
	    		
				case 128: // BW 8kHz HD N=128 FN=8000
				tx_filter_ptr = (float *)&H_fn4_bw4_128_tx[0];
				rx_filter_ptr = (float *)&H_fn4_bw4_128_rx[0];
				//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
				break;
	    		
	    		case 256:
				//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
				rx_filter_ptr = (float *)&H_fnxx_bwxx_256_rx[0];
				tx_filter_ptr = (float *)&H_fnxx_bwxx_256_tx[0];
	    		
	    		break;
	    		
	    		case 512:
				rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
	    		break;
	    		
	    		default:	    		
	    		break;
				}
			break;
			case high_FN: // FN=12000 or FN=9600
				if (FN==12000) // not used anymore
				{
					switch (N) //usually N<=128 for high_FN/LowDelay
					{
	    			case 64:
							use_long_RX_Filter_2NmL=1;
	    			//for N=64, we have special version of RX-Filters
	    			// separately for both cases L=8 and L=16
	    			// these filters have doubled length (2*N-L+1)=(128-L+1)
	    			// instead of (N-L+1)
	    			if (L==8)
		    			rx_filter_ptr = (float *)&H_Filter_RX_8_128_8[0];
		    		    		
					tx_filter_ptr = (float *)&H_Filter_TX_8_64[0];
	    			break;
		    		
					case 128:
					rx_filter_ptr = (float *)&H_Filter_RX_8_128[0];
					tx_filter_ptr = (float *)&H_Filter_TX_8_128[0];
					break;
		    		
	    			case 256: //not used
					rx_filter_ptr = (float *)&H_Filter_RX_8_256[0];
					tx_filter_ptr = (float *)&H_Filter_TX_8_256[0];
	    			break;
		    		
    				case 512: //not used
					rx_filter_ptr = (float *)&H_Filter_RX_8_512[0];
					tx_filter_ptr = (float *)&H_Filter_TX_8_512[0];
	    			break;
		    		
	    			default:	    		
	    			break;
					}
				}
				else
				if (FN==9600)
				{
					switch (N) //usually N<=128 for high_FN/LowDelay
					{		   
					case 64: // not used anymore
							use_long_RX_Filter_2NmL=1;
	    			//for N=64, we have special version of RX-Filters
	    			// separately for both cases L=8 and L=16
	    			// these filters have doubled length (2*N-L+1)=(128-L+1)
	    			// instead of (N-L+1)
	    			if (L==8)
		    			rx_filter_ptr = (float *)&H_Filter_RX_8_128_8[0];
		    		    		
					tx_filter_ptr = (float *)&H_Filter_TX_8_64[0];
	    			break;
		    		
					case 128: // this is the important case !!!
							  // BW8 LowDelay/highFN ,FN=9600 N=128, L=7,16
					rx_filter_ptr = (float *)&H_fn96_bw8_128_rx[0];
					tx_filter_ptr = (float *)&H_fn96_bw8_128_tx[0];
					//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
					//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];

					break;
		    		
	    			case 256: // not used
					case 512:
					break;
		    		
	    			default:	    		
	    			break;
					}
				}
			break;
			}
		break;
		
		case  7500:
		switch (BW4_8_Mode)
			{
			case std_FN: //used for HighDelay, possible are all N 128..512
			switch (N)   // 	right now for N=64, high_FN is used
				{
	    		case 64: // not used
						use_long_RX_Filter_2NmL=1;
	    		//for N=64, we have special version of RX-Filters
	    		// separately for both cases L=8 and L=16
	    		// these filters have doubled length (2*N-L+1)=(128-L+1)
	    		// instead of (N-L+1)
	    		//rx_filter_ptr = (float *)&H_Filter_RX_32_64[0];  //replaced by next
				if (L==8)
	    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
	    		if (L==16)
	    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_16[0];
	    		
	    		//rx_filter_ptr = (float *)&H_Filter_RX_8r_64[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_8r_64[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
	    		
	    		break;
	    		
				case 128:
				//rx_filter_ptr = (float *)&H_Filter_RX_8r_128[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_8r_128[0];
				tx_filter_ptr = (float *)&H_fn4_bw4_128_tx[0];
				rx_filter_ptr = (float *)&H_fn4_bw4_128_rx[0];
				//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
				break;
				
				case 256: 
				//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
				//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
				rx_filter_ptr = (float *)&H_fnxx_bwxx_256_rx[0];
				tx_filter_ptr = (float *)&H_fnxx_bwxx_256_tx[0];
	    		
	    		break;
	    		
	    		case 512: // not optimized
				rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
				tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
	    		break;
				}
			break;
			case high_FN: // FN=12000 or FN=9600
				if (FN==12000) // not used
				{
					switch (N)
					{
	    			case 64:
							use_long_RX_Filter_2NmL=1;
	    			//for N=64, we have special version of RX-Filters
	    			// separately for both cases L=8 and L=16
	    			// these filters have doubled length (2*N-L+1)=(128-L+1)
	    			// instead of (N-L+1)
	    			if (L==8)
		    			rx_filter_ptr = (float *)&H_Filter_RX_8_128_8[0];
		    		    		
					tx_filter_ptr = (float *)&H_Filter_TX_8_64[0];
	    			break;
		    		
					case 128:
	//				rx_filter_ptr = (float *)&H_Filter_RX_8_128[0];
	//				tx_filter_ptr = (float *)&H_Filter_TX_8_128[0];
					rx_filter_ptr = (float *)&H_Filter_RX_8r_fn12_128[0];
					tx_filter_ptr = (float *)&H_Filter_TX_8r_fn12_128[0];
					break;
			    		
	    			case 256:
					rx_filter_ptr = (float *)&H_Filter_RX_8_256[0];
					tx_filter_ptr = (float *)&H_Filter_TX_8_256[0];
	    			break;
		    		
    				case 512:
					rx_filter_ptr = (float *)&H_Filter_RX_8_512[0];
					tx_filter_ptr = (float *)&H_Filter_TX_8_512[0];
	    			break;
		    		
	    			default:	    		
	    			break;
					}
				} //if FN==12000
				else
				if (FN==9600) //BW7.5kHz (highFN/LowDelay)
				{
				switch (N) //usually N<=128 for high_FN/LowDelay
					{		   
					case 64: // not used, N=128 instead
							use_long_RX_Filter_2NmL=1;
	    			//for N=64, we have special version of RX-Filters
	    			// separately for both cases L=8 and L=16
	    			// these filters have doubled length (2*N-L+1)=(128-L+1)
	    			// instead of (N-L+1)
	    			if (L==8)
		    			rx_filter_ptr = (float *)&H_Filter_RX_8_128_8[0];
		    		    		
					tx_filter_ptr = (float *)&H_Filter_TX_8_64[0];
	    			break;
		    		
					case 128: // this is the important case !!!
							  // BW7.5 LowDelay/highFN ,FN=9600 N=128, L=7,16
					rx_filter_ptr = (float *)&H_fn96_bw75_128_rx[0];
					tx_filter_ptr = (float *)&H_fn96_bw75_128_tx[0];
					//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
					//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];

					break;
		    		
	    			case 256: // not used
					case 512:
					break;
		    		
	    			default:	    		
	    			break;
					}
				}
			break; //case high_FN
			}
		break; //case BW7500
		
		case  11500:
		switch (N)
			{
    		case 64:
					use_long_RX_Filter_2NmL=1;
    		//for N=64, we have special version of RX-Filters
    		// separately for both cases L=8 and L=16
    		// these filters have doubled length (2*N-L+1)=(128-L+1)
    		// instead of (N-L+1)
    		//rx_filter_ptr = (float *)&H_Filter_RX_32_64[0];  //replaced by next
			if (L==8)
    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_8[0];
    		if (L==16)
    			rx_filter_ptr = (float *)&H_Filter_RX_32_128_16[0];
    		
    		tx_filter_ptr = (float *)&H_Filter_TX_32_64[0];
    		
    		break;
    		
			case 128:
			//rx_filter_ptr = (float *)&H_Filter_RX_32_128[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_128[0];
			tx_filter_ptr = (float *)&H_fn4_bw4_128_tx[0];
			rx_filter_ptr = (float *)&H_fn4_bw4_128_rx[0];
			break;
			
			case 256: 
			//rx_filter_ptr = (float *)&H_Filter_RX_32_256[0];
			//tx_filter_ptr = (float *)&H_Filter_TX_32_256[0];
			rx_filter_ptr = (float *)&H_fnxx_bwxx_256_rx[0];
			tx_filter_ptr = (float *)&H_fnxx_bwxx_256_tx[0];
	    		
    		break;
    		
    		case 512: 
			rx_filter_ptr = (float *)&H_Filter_RX_32_512[0];
			tx_filter_ptr = (float *)&H_Filter_TX_32_512[0];
    		break;
			}
		break;
		
		case 28000:
		
		switch (N)
			{
    		case 64:
					use_long_RX_Filter_2NmL=1;
    		//for N=64, we have special version of RX-Filters
    		// separately for both cases L=8 and L=16
    		// these filters have doubled length (2*N-L+1)=(128-L+1)
    		// instead of (N-L+1)
    		//rx_filter_ptr = (float *)&H_Filter_RX_28_64[0];
			if (L==8)
    			rx_filter_ptr = (float *)&H_Filter_RX_28_128_8[0];
    		if (L==16)
    			rx_filter_ptr = (float *)&H_Filter_RX_28_128_16[0];
    		tx_filter_ptr = (float *)&H_Filter_TX_28_64[0];
    		break;
    		
			case 128:
			rx_filter_ptr = (float *)&H_Filter_RX_28_128[0];
			tx_filter_ptr = (float *)&H_Filter_TX_28_128[0];
			break;
    		
    		case 256:
			rx_filter_ptr = (float *)&H_Filter_RX_28_256[0];
			tx_filter_ptr = (float *)&H_Filter_TX_28_256[0];
    		break;
    		
    		case 512:
			rx_filter_ptr = (float *)&H_Filter_RX_28_512[0];
			tx_filter_ptr = (float *)&H_Filter_TX_28_512[0];
    		break;
    		
    		default:
    		
    		break;
			}
		
		break;
		
		case 20000:
		
		switch (N)
			{
    		case 64:
					use_long_RX_Filter_2NmL=1;
    		//for N=64, we have special version of RX-Filters
    		// separately for both cases L=8 and L=16
    		// these filters have doubled length (2*N-L+1)=(128-L+1)
    		// instead of (N-L+1)
    		//rx_filter_ptr = (float *)&H_Filter_RX_20_64[0];
			if (L==8)
    			rx_filter_ptr = (float *)&H_Filter_RX_20_128_8[0];
    		if (L==16)
    			rx_filter_ptr = (float *)&H_Filter_RX_20_128_16[0];
    		
			tx_filter_ptr = (float *)&H_Filter_TX_20_64[0];
    		break;
    		
			case 128:
			rx_filter_ptr = (float *)&H_Filter_RX_20_128[0];
			tx_filter_ptr = (float *)&H_Filter_TX_20_128[0];
			break;
    		
    		case 256:
			rx_filter_ptr = (float *)&H_Filter_RX_20_256[0];
			tx_filter_ptr = (float *)&H_Filter_TX_20_256[0];
    		break;
    		
    		case 512:
			rx_filter_ptr = (float *)&H_Filter_RX_20_512[0];
			tx_filter_ptr = (float *)&H_Filter_TX_20_512[0];
    		break;
    		
    		default:
    		
    		break;
			}
		break;
		
		
		default:
		
		break;
		}
		
	// Transfer function of TX Filter in Filter_TX
	vset_f_D((float *)&Cmplx_Array[0],0.0f,2*N2);
	if(N < OFDM_N_MAX)
		{
		vcopy_DE((float *)&Cmplx_Array[0],tx_filter_ptr,2*(N_N_8+1));
		}
	else
		{
		vcopy_DE((float *)&Cmplx_Array[N_N_8/2-N_N_8/4],tx_filter_ptr,2*(N_N_8/2+1));
		}
	fft(N2,Fl_Array_R, Fl_Array_I, Cmplx_Array);
	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N2); 	  
	vabs_c_DD(Fl_Array_R,Cmplx_Array,N2);
	m=vmax_f_D(Fl_Array_R,N2);
	vsmul_f_DD((float *)&Cmplx_Array[0],1.0f/m,(float *)&Cmplx_Array[0],2*N2);
	vcopy_ED((float *)&Filter_TX[0],(float *)&Cmplx_Array[0],2*N2); 	  

	// LFM Signal Generation (LFM-Size must be known prior to RX-Filter-Design)
	if (use_long_RX_Filter_2NmL) LFM_Size=2*(2*64);  // for N=64 double LFM and RX-Filter-Length
	else						 LFM_Size=N2;		 // for N=512,256,128 normal LFM length

	LFM_2_N = LFM_Size/N;
	LFM_2_N2= LFM_Size/N2;
	
	a = 0.0f;
    b = (float)LFM_Size;
    c = Amp_LFM_TX/sqrtf(2.0f);
    for (i=0; i<LFM_Size; i++)
    	{
        d = (OFDM_PI*a*a)/b;
        OFDM_LFM_2_TX[i].re = cosf(d)*c;
        OFDM_LFM_2_TX[i].im = sinf(d)*c;      	
        OFDM_LFM_2_RX[i].re = cosf(d)*c/(float)N;
        OFDM_LFM_2_RX[i].im = sinf(d)*c/(float)N;      	
        a += 1.0f;
    	}
    	
	// Reset LFM generator
    LFM_Index = 0;
    

	// Transfer function of RX Filter in C_Filter_RX
	vset_f_D((float *)&Cmplx_Array[0],0.0f,2*LFM_Size);
	
	if (use_long_RX_Filter_2NmL)
	{ //for N=64 , except for FN=4800)
		vcopy_DE((float *)&Cmplx_Array[0],rx_filter_ptr,2*(128-L+1));
	}
	else
	{
	switch (N)
		{
		case 64:
		case 128:
		case 256:
			vcopy_DE((float *)&Cmplx_Array[0],rx_filter_ptr,2*(N_N_8+1));
		break;
		
		case 512:
			vcopy_DE((float *)&Cmplx_Array[N_N_8/2-N_N_8/4],rx_filter_ptr,2*(N_N_8/2+1));
		break;
		}
	}

	fft(LFM_Size,Fl_Array_R, Fl_Array_I, Cmplx_Array);
	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,LFM_Size); 	  
	vabs_c_DD(Fl_Array_R,Cmplx_Array,LFM_Size);
	m=vmax_f_D(Fl_Array_R,LFM_Size);
	vsmul_f_DD((float *)&Cmplx_Array[0],1.0f/m,(float *)&Cmplx_Array[0],2*LFM_Size);
	vcopy_ED((float *)&C_Filter_RX[0],(float *)&Cmplx_Array[0],2*LFM_Size); 	  

	// Squared transfer function of RX filter
	vabs_sc_DD(Fl_Array_R,1,Cmplx_Array,LFM_2_N,N);
    vmul_f_DDD(Fl_Array_R,Fl_Array_R,Fl_Array_R,N);
	vcopy_ED(Abs2_Filter_RX,Fl_Array_R,N); 	  
	
	
	// Init RX Filter Watchdog and Fallback Function
	OFDM_Filter_RX_Watchdog(0.0f, 1);
	vcopy_EE((float *)&C_Filter_RX_fallback[0],(float *)&C_Filter_RX[0],2*LFM_Size);
	//	Note: C_Filter_RX_fallback will be reinitialized in LFM_Processing

	// Clear complex_float Overlap_Save_RX[N4] array
	vset_f_E((float *)Overlap_Save_RX,0.0f,sizeof(Overlap_Save_RX));
	
		
	//==============================================================================================

	// Number of complex samples on 64kHz domain each OFDM symbol
	NL2Q = (int)((float)NL * SFC_Interpolation) * Filter_TX_Interpolation;
	
	
	// Signal Scaling cont.
	//==============================================================================================
		
	// TX output scaling (all FFT's and interpolation in TX-Filter)
	switch (Filter_TX_Interpolation)
	{
		case 2:  // Standard Case	
		Output_Scale_TX   =Norm_TX/sqrtf((float)N*(float)N2*(float)N4);
		break;
		
		case 4:				//sqrt(Filter_TX_Interpolation)/...FFTs...
		Output_Scale_TX   =2.0f/sqrtf((float)N*(float)N2*(float)N4*2.0f);
		break;
		
		case 8:				//sqrt(Filter_TX_Interpolation)/...FFTs...
		Output_Scale_TX   =2.0f*Norm_TX/sqrtf((float)N*(float)N2*(float)N4*4.0f);
		break;
	}
		
	// Scale for all RX FFT's
	//if (N==64)
	if (use_long_RX_Filter_2NmL)

		switch (Filter_RX_Decimation)
		{
		case 2:  // Standard Case N==64, Dec=2
				//				  RX-engine-FFT RX-Filt-IFFT RX-Filt-FFT dec2						
		Input_Scale_RX = 1.0f/sqrtf((float)N*(float)N4*(float)(N*8)*2.0f);
		break;

		case 4: // N==64, Dec=4
				//				  RX-engine-FFT RX-Filt-IFFT RX-Filt-FFT dec4						
		Input_Scale_RX = 1.0f/sqrtf((float)N*(float)N4*(float)(N*16)*4.0f);
		break;
		
		case 8: // N==64, Dec=8
				//				  RX-engine-FFT RX-Filt-IFFT RX-Filt-FFT dec8						
		Input_Scale_RX = 1.0f/sqrtf((float)N*(float)N4*(float)(N*32)*8.0f);
		break;
		}
	else //for normal RX-Filter length N-L
		switch (Filter_RX_Decimation)
		{
		case 2:  // Standard Case N>64, Dec=2
				//				  RX-engine-FFT RX-Filt-IFFT RX-Filt-FFT dec2
		Input_Scale_RX = 1.0f/sqrtf((float)N*(float)N2*(float)N4*2.0f);
						
		break;

		case 4: // N>64, Dec=4
				//				  RX-engine-FFT RX-Filt-IFFT RX-Filt-FFT dec4
		Input_Scale_RX = 1.0f/sqrtf((float)N*(float)N2*(float)(N*8)*4.0f);
 
		break;
		
		case 8: // N>64, Dec=8 (N<256!!)
				//				  RX-engine-FFT RX-Filt-IFFT RX-Filt-FFT dec8
		Input_Scale_RX = 1.0f/sqrtf((float)N*(float)N2*(float)(N*16)*8.0f);
 
		break;
		}

	// Phase Rotation in TX and RX Filter for L=7
	if (L==7)
	{
	tx_filt_phase.re=1.0f; //Start-Value
   	tx_filt_phase.im=0.0f;

   	//  tx_filt_phaserot.re = cosf(OFDM_PI*2.0f/(float)N2*(Shift_TX)*(float)L);
   	//  tx_filt_phaserot.im = sinf(OFDM_PI*2.0f/(float)N2*(Shift_TX)*(float)L);
	// with Shift_TX = N yields
   	  tx_filt_phaserot.re = cosf(OFDM_PI*2.0f/(float)N2*(float)N*(float)L);
   	  tx_filt_phaserot.im = sinf(OFDM_PI*2.0f/(float)N2*(float)N*(float)L);
   	// with L=7 yields
   	//  tx_filt_phaserot.re = -1.0f; //cosf(7PI)
   	//  tx_filt_phaserot.im =  0.0f; //sinf(7PI)
   	
	rx_filt_phase.re=1.0f; //Start-Value
   	rx_filt_phase.im=0.0f;
   	//Rot_RX.re = cosf(OFDM_PI*2.0f/(float)N2*(Shift_RX)*(float)L);
   	//Rot_RX.im =-sinf(OFDM_PI*2.0f/(float)N2*(Shift_RX)*(float)L);
	}
	
//--------------- Reset State Machine ------------------------------ 
//=================================================================================================

   switch (Optim_NL_State)
	{
		case Optim_NL_OFF:
		 		TX_Preamble_State = TX_2TONE_1;
		 		RX_Preamble_State = RX_DETECTION_AGC;
			break;
		case Optim_NL_ON_Determination:
		case Optim_NL_ON_Selected:
				TX_Preamble_State = TX_NL_Mode_Call;
		 		RX_Preamble_State = RX_DETECTION_MODE;
		 		Pilot_Mode_detection = Pilot_not_detected;
			break;
		
		default:
			Error.Count=999;
			exit(1);
			break;
	}
	// Reset OFDM transmitter
	Num_Symbol_TX=0;
	TX_NL_Call = 0;
//TX_Preamble_State    =	TX_Preamble_IDLE;
	number_of_txp_cycles = N_SYMBOLS_TONE1;
	txp_state_counter    = 0;		   
	Start_TX 			 = 0;
//PL2106 start TX immediately	
//Start_TX 			 = 1;
	Start_TX_QAM_Engine  = 0;
	TX_QAM_State         = TX_QAM_DUMMY1;
	TX_OFDM_Engine_State = TX_Engine_QAM4_DUMMY;
	txq_state_counter	 = 0;
	tx_state_finished    = 0;
	Start_CH_DATA_Transmission = 0;
	Start_Suffix_Transmission  = 0;
	Suffix_is_transmitted 	= 0;
	Suffix_is_detected 		= 0;

	tx_ch_data_counter = 0;
	rx_ch_data_counter = 0;
	
	tx_wait_for_restart_ctr =0;
	rx_wait_for_restart_ctr =0;
	
	Predistortion_Active = OFF;
	
	// Reset OFDM receiver
//PL2106 RX alway idle	
//RX_Preamble_State = RX_Preamble_IDLE;
	rxq_state_counter    = 0;
	rx_state_finished    = 0;
	// Disable start of RX OFDM engine         
  	Start_RX_QAM_Engine  = 0;
	Pause_RX_QAM_Engine	 = 0;
  	// Reset of RX OFDM engine
    RX_OFDM_Engine_state = RX_Engine_IDLE;
    RX_QAM_State = RX_QAM_IDLE;

	Sync_State = Mode_Detection;
  	

	// Reset processing of preamble	
  	Det_Agc_RX = 0;
  	tone_detected = unknown_state;
 	pilotAlm_waitcounter = 0;
	Freq_Est_RX = 0;
	Noise_Est_RX = 0;
	LFM_Proc_RX = 0;
	QAM4_Detect_RX = 0;
	Optim1_RX = 0;
	Optim2_RX = 0;
	Optim3_RX = 0;
	
	// ************ State Machine Debug *********
	Prefix_Suffix_TH = (5*N)/64;
    Prefix_thres = 10000;
    Suffix_thres = 10000;

    // ******** Error Struct Init *********************
    if ( Restart_Counter == 0 ) {
    vset_f_E((float *)&Error,0.0f,sizeof(Error));
    Error_count_shd = Error.Count;
    }
 
    giEmergency_restart = 0;
    
    Init_Delay_Restart();
    
	// Reset of waiting cycle
    Is_Waiting = WAITING_SYMBOLS;
    
    // Init block counters 
	n_blk_tx = 0;
	n_blk_rx = 0;
	
	n_blk_tx_data = 0;
	n_blk_rx_data = 0;
	
	n_blk_tx_QAM4 = 0;
	n_blk_rx_QAM4 = 0;
	
	n_blk_tx_M_QAM_train = 0;
	n_blk_rx_M_QAM_train = 0;

	// If size of bin more than 20Hz, do standard length of Training sequence
	// If size of bin less than 20Hz, do longer Training sequence
	// (small bin size is more affected by residual frequency offset)
	M_QAM_Training_Symbols=N_SYMBOLS_M_QAM_TRAINING;
	if (Fbin < 20.0f) M_QAM_Training_Symbols=2*N_SYMBOLS_M_QAM_TRAINING;
	
	// MQAM Random Generator Init
	TCM_MQAM_Train_Gen_Init();
	
	// Counter for Time Out Errors
	rx_pilot_mode_ctr=0;
	rx_noise_est_in  =0;
	rx_noise_est_out =0;
	rx_lfm_in        =0;
	rx_qam4_in       =0;
   
   //--------------- End of State Machine Reset ------------------------------ 
	
	

    
	// Frequency Control, Measurement and Shift
	//=================================================================================================
    
    // Complex sample shift for block synchronization
	Symbol_Synchro = 0;
	Symbol_Synchro_FS = 0;

	// Reset frequency and clock offset estimation
	Df_block_cnt = 0;
	
	Df_valid_result = 0;
	
	Freq_Offset_Control_Flag = 0;
	Frequency_Control_enable = 0;

	Freq_Offset = 0.0f; // in part of Nyquist frequenz
	
	Clock_Offset = 0.0f; // in part of Nyquist frequenz
	
	Start_Clk_Ph = 0;
	
	//Clock_Ph_Offset = 0.0f;
	
	Df_Hz   = 0.0f;       // in Hz
	Df_Hz_2 = 0.0f;

	// Frequency Shift in SFC-Filter TX and RX  
	//Df_Hz_TX = 00.0f;   // defined in Verify_Modem_Config
	//Df_Hz_RX = 00.0f;   // defined in Verify_Modem_Config
	//				depending on APLC and ETL-Pilot configuration
	
	Start_Phi_TX = 0.0f;
	Start_Phi_New_TX = 0.0f;

	// Initialize Rotation Coefficients (used in OFDM_SFC_TX)
	Freq_Offset_TX = Df_Hz_TX*OFDM_TWO_PI/FS_TX_RX;
			Rot_Fr_TX.re = cosf(Freq_Offset_TX);
			Rot_Fr_TX.im = sinf(Freq_Offset_TX);
	// Initialize Restauration of Phase Increment
	// Delta_Phi_Symbol_TX is the phase increment after one processed block
	// and depends on block length
	// In the cases FN=6000,12000,24000 the block length for one call to Exp_Gen
	// in OFDM_SFC_TX is 4/3*NL,
	// in the case FN==4800 block length is 5/3*NL
	// in the case FN==9600 block length is 10/3*NL
	//, for all other cases it is 2NL
	switch (FN)
	{
		case  6000:
		case 12000:
		case 24000:			
			Delta_Phi_Symbol_TX = Freq_Offset_TX*(4.0f*(float)NL/3.0f);
		break;
		
		case 4800:
		case 9600:
			Delta_Phi_Symbol_TX = Freq_Offset_TX*(5.0f*(float)NL/3.0f);
		break;
		
	//	case 9600:
	//		Delta_Phi_Symbol_TX = Freq_Offset_TX*(10.0f*(float)NL/3.0f);
	//	break;
		
		default: 
			Delta_Phi_Symbol_TX = Freq_Offset_TX*(float)(2*NL);
		break;
	}
	
	// RX Frequency Shift in SFC-Filter_RX  
	// Initialize Rotation Coefficients (used in OFDM_SFC_RX)
	Freq_Offset_RX = Df_Hz_RX*OFDM_TWO_PI/FS_TX_RX;
   	// next variables are initialized in OFDM_SFC_RX directly, because
   	// they depend on implementation (processing blocklength for Delta_Phi_Symbol)
	//Rot_Fr.re =  cosf(Freq_Offset);
	//Rot_Fr.im = -sinf(Freq_Offset);
	//Delta_Phi_Symbol = -Freq_Offset*(float)(2*NL);

	// Reset frequency offset compensation 
	Start_Phi		 = 0.0f;
	Start_Phi_New	 = 0.0f;
	Start_Phi_New_FC = 0.0f; // OFDM_FreqCtrl_RX.c

	//Freq_Offset = Freq_Offset_TX;
	Freq_Offset = 0.0f;

   	Rot_Fr.re =  0.0f;
	Rot_Fr.im =  0.0f;
	
	Delta_Phi_Symbol = 0.0f;

	ow0 = 0.0f;
	ow1 = 0.0f;
			
	Start_Ph_Pil0 = 0.0f;
	Start_Ph_Pil1 = 0.0f;
	
	// PHASECONTROL 2008
	Phase_Control_Init();
    
	// Initialization of some Arrays, Fields and Window Functions
	//=================================================================================================

	// Init of predistortion
    vset_f_E(C_TX,1.0f,N);
    vset_f_E(C_RX,1.0f,N);
	
	// Window for RX filter syntesis
    for (i=0; i<L_PROTO; i++)
    	{
        Fl_Array_R[i] = 0.5f*(1.0f-cosf(OFDM_TWO_PI/(float)L_PROTO*(float)i));
    	}
	vcopy_DD(Win_Synt,Fl_Array_R,L_PROTO);
    
	// Window for NL-Optimization [2N]
    for (i=0; i<LFM_Size; i++)
    	{
        Fl_Array_R[i] = 0.5f*(1.0f-cosf(OFDM_TWO_PI/(float)LFM_Size*(float)i));
    	}
    vcopy_ED(Win_NLopt_LFMSize,Fl_Array_R,LFM_Size);
    
    // Window for RX transfer function evaluation
    for (i=0; i<N; i++)
    	{
        Fl_Array_R[i] = 0.5f*(1.0f-cosf(OFDM_TWO_PI/(float)N*(float)i));
    	}
	vcopy_ED(Win_TF_RX,Fl_Array_R,N);

    // Init noise index array 
	vramp_i_D((int *)&Fl_Array_R[0],0,1,N);
	vcopy_ED((float *)&Noise_Ind[0],(float *)&Fl_Array_R[0],N);

	// Init channel gain index array 
	vramp_i_D((int *)&Fl_Array_R[0],0,1,N);
	vcopy_ED((float *)&Index_K_TX[0],(float *)&Fl_Array_R[0],N);
	vcopy_ED((float *)&Index_K_RX[0],(float *)&Fl_Array_R[0],N);
	

	// Initialization of some Coefficients
	//=================================================================================================

	// Init Predd, defines position of maximum of impulse response in RX filter synthesis
    Predd = (N-L_PROTO)/4;
	//    Predd = N_L/3;
	//    Predd = N_L/2;
    
    if (use_long_RX_Filter_2NmL)	
    	Predd = (2*N-L_PROTO)/4; //due to filter length 2N-L
    	//Predd = (2*N-L)/2; //due to filter length 2N-L
       
    // Coefficient for preequalizer adaptation
    Alpha_Pil = 1.0f;				// start and actual value
 	Alpha_Pil_Destination = 0.2f;	// destination value
	// For small bin size <32Hz, the preequalizer is faster
	if (Fbin < 32.0f) Alpha_Pil_Destination = 0.5f;
	if ((FN<=6000)&&(N==64)) Alpha_Pil_Destination = 0.36f;

    // Coefficient for equalizer adaptation
    Alpha_Channel = 0.05f;	
 //   Alpha_Channel = 0.2f;	
	
    // Coefficient for RX Filter adaptation
//    Alpha_Filter = 0.000005f/(float)N2;
    Alpha_Filter = 0.000001f;
    
	// Adaptation start flag
	Filter_RX_Adaptation = OFF;
    
    // Reset CRC counter
    CRC_Errors_Sum = 0;
	
	// Init restart counter
	Block_Error_Cnt = 10000000;	
	
	// Init Restart parameters
	Block_Error_Cnt = MAX_CRC_ERROR_BURST;
	Level_Error_cnt = 0;
	
	// Init FallBack / FallForward
	OFDM_Fall_BackForward_INIT();
	
	Level_Error_TimeOut= (int)(TimeOut_Time/T_OFDM_symbol);
	if (Level_Error_TimeOut<2) Level_Error_TimeOut=2;

	RX_PilotMode_TimeOut= (int)(RX_PILOTMODE_TIMEOUT_TIME/T_OFDM_symbol);
	if (RX_PilotMode_TimeOut<2) RX_PilotMode_TimeOut=2;

	Level_Warning   = 0;
	//Ref_Level = (OFDM_PILOTS_NUM*Amp_Pilot_TX_Ref*Amp_Pilot_TX_Ref); // moved to Modulation init
    
	// Init Noise Estimation Parameters
	Noise_Est_Cnt = 0;
	vset_f_E(Noise_Density,0.0f,sizeof(Noise_Density));
	
	AWGN_Sigma_Est=0.0f;
	AWGN_Sigma_Est_TX=0.0f;
	AWGN_Sigma_Est_RX=0.0f;
	AWGN_Sigma3_Est=0.0f;

	// Clear Channel_Data_TX and Channel_Data_RX
	vset_f_E((float *)&Channel_Data_TX,0.0f,sizeof(Channel_Data_TX));
	vset_f_E((float *)&Channel_Data_RX,0.0f,sizeof(Channel_Data_RX));
	
	
	// Clear TX/RX arrays
	vset_f_E((float *)&Overlap_Save_TX[0],0.0f,sizeof(Overlap_Save_TX));
	vset_f_E((float *)&Symbol_Buf[0],0.0f,sizeof(Symbol_Buf));
	vset_f_E((float *)&Tx_Array[0],0.0f,sizeof(Tx_Array));
	vset_f_E((float *)&Rx_Array[0],0.0f,sizeof(Rx_Array));
	vset_f_E((float *)&Interp_Buf[0],0.0f,sizeof(Interp_Buf));
	
	// Clear Cmplx_Array (    Cmplx_Array = (complex_float *)&Cmplx[0]; )
	vset_f_E((float *)&Cmplx[0],0.0f,sizeof(Cmplx));

    // Init test parameters	
	Skp_cnt = 0;
	Skp_cnt2 = 0;
   	vset_f_E(MSE_trend_QAM4,0.0f,sizeof(MSE_trend_QAM4));
    vset_f_E(MSE_trend_QAMT,0.0f,sizeof(MSE_trend_QAMT));
    vset_f_E(MSE_trend_QAMD,0.0f,sizeof(MSE_trend_QAMD));
    
    vset_f_E(F_trend_DF,0.0f,sizeof(F_trend_DF));
    vset_f_E(F_trend_CO,0.0f,sizeof(F_trend_CO));
    vset_f_E(F_trend_CO_delta,0.0f,sizeof(F_trend_CO_delta));
    vset_f_E((float *)F_trend_CW,0.0f,sizeof(F_trend_CO));
    
	vset_f_E(Mag2_Inp,0.0f,sizeof(Mag2_Inp));
	vset_f_E(Mag2l_Inp,0.0f,sizeof(Mag2l_Inp));
    vset_f_E((float *)&Overlap_Save_Spectrum_RX[0],0.0f,sizeof(Overlap_Save_Spectrum_RX));

    vset_f_E((float *)&Channel_TF[0],0.0f,sizeof(Channel_TF));
	vset_f_E((float *)&Imp_Filter_RX[0],0.0f,sizeof(Imp_Filter_RX));
	
	
	
	MSE_Mean = 0.0f;
	MSE_dB = 0.0f;
	MSE_dB_Mean = 0.0f;
	delta_sigma = 0.0f;
	delta_sigma_mean = 0.0f;
	BLER_Est = 0.0f;
	SNR_mean_dB = 0.0f;

	Symbol_Energy_TX=0.0f;
	Symbol_Power_TX=0.0f;
	Symbol_Power_TX_Mean=0.0f;

	Symbol_Energy_RX=0.0f;
	Symbol_Power_RX=0.0f;
	Symbol_Power_RX_Mean=0.0f;
	
	vset_f_E(Remod_Spectral_Density,0.0f,sizeof(Remod_Spectral_Density));
	vset_f_E(MSE_Spectral_Density,0.0f,sizeof(MSE_Spectral_Density));
	vset_f_E(MSE_Spectral_Density_Sort,0.0f,sizeof(MSE_Spectral_Density_Sort));
	vset_f_E(SNR_TCM_Sort,0.0f,sizeof(SNR_TCM_Sort));
	vset_f_E(SNR_TCM,0.0f,sizeof(SNR_TCM));
	vset_f_E(Delta_Sigma_Array,0.0f,sizeof(Delta_Sigma_Array));
	SNR_TCM_Switch=0; // Switch to activate/deactivate Measurement block in OFDM_engine_RX
	RX_FILT_ADAPT_Switch = 1; // Switch RX-Filter Adaptation ON in OFDM_engine_RX,
								// Switch can be modified by Long Status !
	gharris(Harris_Window,N4); // Harris Window for spectral estimation in OFDM_Spectrum_RX

	Mean_Coeff = 0.001f;

	// Init optimization parameters
	K_Prob_Err = 5.0f;
	M_Opt_max_TX=0;
	M_Opt_max_RX=0;
	Data_rate_max_RX = 0.0f;
    Data_rate_max_TX = 0.0f;
    User_inf_bits_TX = 0;
    User_inf_bits_RX = 0;
    vset_f_E((float *)&Bits_in_SC_TX[0],0.0f,sizeof(Bits_in_SC_TX));
    vset_f_E((float *)&Bits_in_SC_RX[0],0.0f,sizeof(Bits_in_SC_RX));
//	TCM_optim_rate_TX=0.0f;
//	TCM_optim_rate_RX=0.0f;

	// Init NL-Optimization Parameters
	FS_Score_L=0.0f;

	// init delta bit vector generator
	TCM_Delta_Gen_Init();
	// init V11 buffers and pointers
	V11_Init();
	V11_Start_IO = 0;
	Selected_data_rate_IDX=7; //unknown

	OFDM_Alarm.lifesign   = 0 & 0x7;
	OFDM_Alarm.currentDatarateIx=Selected_data_rate_IDX & 0x7;
	OFDM_Alarm.configAlm  = 0 & 0x1;
	OFDM_Alarm.pilotAlm   = 0 & 0x1;
	OFDM_Alarm.ofdm_syncstate = Sync_State & 0xFF; //PL16.05.2006
	//OFDM_Alarm.syncAlm    = 0 & 0x1; // this Alarm Flag has memory, so maybe 
										// we should leave it as it is
	
 
	
	// debug: init default answer of Long Status
	vramp_f_D((float *)&Fl_Array_R[0],0.0f,1.0f,2*OFDM_N_MAX);
	vcopy_ED((float *)&LongStatusDefaultAnswer[0],(float *)&Fl_Array_R[0],2*OFDM_N_MAX);

	
	Delay_Measure_Flag=1;
	}

//=============================================================================================
void PreambleTX2Tone_Init(int index0, int index1) {
	
	extern int Preamble_Tone0, Preamble_Tone1;
	
	Preamble_Tone0 = index0;
	Preamble_Tone1 = index1;
		
	// TWO Tone phase init and phase rotation init (necessary for L==7 only)
	tx2tone0_phase.re = 1.0f;
	tx2tone0_phase.im = 0.0f;
	tx2tone1_phase.re = 1.0f;
	tx2tone1_phase.im = 0.0f;
	
	tx2tone0_phaserot.re = cosf(OFDM_TWO_PI*(float)index0*(float)L/(float)N);
	tx2tone0_phaserot.im = sinf(OFDM_TWO_PI*(float)index0*(float)L/(float)N);
	tx2tone1_phaserot.re = cosf(OFDM_TWO_PI*(float)index1*(float)L/(float)N);
	tx2tone1_phaserot.im = sinf(OFDM_TWO_PI*(float)index1*(float)L/(float)N);
	
	}


