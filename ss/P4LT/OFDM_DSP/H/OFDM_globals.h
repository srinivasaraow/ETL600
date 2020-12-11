/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_globals.h $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM modem main parameter definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/H/OFDM_globals.h $
 * 
 * 17    24.07.08 15:19 Chhamaa
 * 
 * 12    5.04.06 15:28 Chhamaa
 * 
 * 12    17.03.06 14:41 Chhamaa
 * 
 * 10    16.09.05 9:42 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 5     26.04.05 8:45 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 4     7.03.05 8:43 Ats
 * Exchange with TLC
 * 
 * 3     15.02.05 10:06 Chalgyg
 * OFDM Modem Version 0.09G 
 * 
 * 2     28.01.05 11:48 Ats
 * Exchange TLC
 * 
 * 6     3.12.04 9:43 Chhamaa
 * 
 * 4     1.10.04 12:23 Leeb02
 * 
 * 
 * Initial version
 *       15.02.2003 vgp
 *****************************************************************************/

#include <complex.h>
#include "OFDM_Defs.h"
#include "OFDM_cfg_types.h"
#include "./tcm/TCM_Codec.h"

// *************** Segment pmArray_I ****************

float segment( "seg_pmArray_I" ) pm Fl_Array_I[4*OFDM_N_MAX];      // 4N = 2048 (for N = 512)

// *************** Segment Cmplx ****************

int segment( "seg_Cmplx" ) Cmplx[8*OFDM_N_MAX];			           // 8N = 4096 (for N = 512)

// *************** Segment dmArray_R ****************

float segment ( "seg_dmArray_R" ) Fl_Array_R[4*OFDM_NL_MAX+4*OFDM_N_MAX];       // 4(N+L) = 2304 (for N = 512)
																				// 4*N = 2048
																				// -------------
																				// 	4352    32bit words
// *************** Segment dmda ****************

asm(".align 2;");
complex_float Eqlzr[OFDM_N_MAX];          // 2N = 1024 (for N = 512)

asm(".align 2;");
float Win_Synt[OFDM_L_MAX];

asm(".align 2;");
float Ph_Pil_t0[LEN_DF_EST_DATA];

asm(".align 2;");
float Ph_Pil_t1[LEN_DF_EST_DATA];

int N;
int N2;
int N4;
int L;
int NL;
int NL2;
int NL2Q;
int N_L;
int N_pil;
int N_tone;
int Tone_ind0;
int Tone_ind1;
int Pilot_ind0;
int Pilot_ind1;
int Spilot_TX_0, Spilot_RX_0, Spilot_TX_1, Spilot_RX_1;

int L_PROTO;

int NL_SFC;

float T_OFDM_symbol;
int LifesignWatchDog;

int M_TX;
int M0_TX;

int M_RX;
int M0_RX;

int M_QAM4;
int M0_QAM4;

int BW;
int FN;

float Fbin;

complex_float *OFDM_Symbol_TX;

complex_float *OFDM_Symbol_RX;

complex_float *Cmplx_Array;

float Alpha_Pil;
float Alpha_Pil_Destination;

float Alpha_Channel;

float Norm_TX;

int  LFM_Index;

unsigned int LFM_Size;
unsigned int LFM_2_N, LFM_2_N2;

unsigned int Temp_Buf_ctr;

float Amp_Tone_TX;
complex_float tx2tone0_phase;
complex_float tx2tone1_phase;
complex_float tx2tone0_phaserot;
complex_float tx2tone1_phaserot;

float Amp_Pilot;

float Amp_Pilot_TX;
	
float Amp_Pilot_RX;

float Amp_LFM_TX;


float Scale_QAM_TX;

float Delta_TX;

float P_OFDM_TX;

float P_OFDM_Spectrum;

float P_QAM;

int OFDM_Loop_Mode;

float Input_Scale_RX;

float Output_Scale_TX;

int Symbol_Synchro;
int Symbol_Synchro_FS;
int segment ( "seg_extn SHT_NOBITS" ) Symbol_Synchro_FS_rem;

float Digital_AGC;

float Energy, Energy_Pil0, Energy_Pil1, Power, Power_Pil, 
      FS_Det, Detection_Treshhold;

float Analog_Gain, Des_Val_Pow_Pil;

int Predistortion_Active;

unsigned int segment ( "seg_extn SHT_NOBITS" ) OFDM_Modem_DISABLE;
// ********** State Machine TX ************
int Num_Symbol_TX=0;
t_TX_Preamble_State 		TX_Preamble_State;
int number_of_txp_cycles;
int txp_state_counter;
int Start_TX;
int Start_TX_QAM_Engine;
t_TX_QAM_State				TX_QAM_State;
t_TX_OFDM_Engine_State 		TX_OFDM_Engine_State;
int txq_state_counter;
int tx_state_finished;
int Start_CH_DATA_Transmission;
int Start_Suffix_Transmission;
int Suffix_is_transmitted;
int Suffix_is_detected;
int tx_wait_for_restart_ctr =0;


// ********** State Machine RX ************
int rx_state_finished = 0;
int rxq_state_counter = 0;
t_RX_Preamble_State 		RX_Preamble_State = RX_DETECTION_MODE;

int Start_RX_QAM_Engine = 0;
int Pause_RX_QAM_Engine = 0;
t_RX_OFDM_Engine_state		RX_OFDM_Engine_state = RX_Engine_IDLE;
t_RX_QAM_State 				RX_QAM_State = RX_QAM_IDLE;


int Det_Agc_RX = 0;
e_tone_detection_type tone_detected;
int pilotAlm_waitcounter = 0;
int	Freq_Est_RX = 0;
int	Noise_Est_RX = 0;
int	LFM_Proc_RX = 0;
int QAM4_Detect_RX = 0;

int	Optim1_RX = 0;
int Optim2_RX = 0;
int Optim3_RX = 0;

// ************ State Machine Exeption counters *********
// Preamble section
int rx_pilot_mode_ctr=0;
int RX_PilotMode_TimeOut=0;
int rx_noise_est_in  = 0;
int Noise_Est_Cnt = 0;
int rx_noise_est_out = 0;
int rx_lfm_in        = 0;
int rx_qam4_in       = 0;



// QAM section
int rx_prefix_counter = 0;
int rx_suffix_counter = 0;
int rx_wait_for_restart_ctr =0;


unsigned int n_blk_tx, n_blk_rx;
unsigned int n_blk_tx_data, n_blk_rx_data;
unsigned int n_blk_tx_QAM4, n_blk_rx_QAM4;
unsigned int n_blk_tx_M_QAM_train, n_blk_rx_M_QAM_train;
unsigned int M_QAM_Training_Symbols;



// ************ State Machine Debug *********
//int segment ("seg_extn SHT_NOBITS") Prefix_thres =10000;
//int segment ("seg_extn SHT_NOBITS") Suffix_thres =10000;
int Prefix_Suffix_TH;
int Prefix_thres;
int Suffix_thres;

int segment ( "seg_extn SHT_NOBITS" ) Software_Version_ID;

// ************* Error Handler **************
int segment ( "seg_extn SHT_NOBITS" ) dbg_error[100];
//int segment ( "seg_extn_init" ) dbg_error[100];


t_Error_type segment ( "seg_extn SHT_NOBITS" )  Error;
int segment ("seg_extn SHT_NOBITS") Error_count_shd;

 
int Df_block_cnt;
int Df_subblock_cnt;

int Df_valid_result;

unsigned int Freq_Offset_Control_Flag;
unsigned int Frequency_Control_enable;

float Freq_Offset, Clock_Offset, Clock_Phase_Offset, Df_Hz, Df_Hz_2;

float Freq_Offset_TX, Df_Hz_TX;
float Freq_Offset_RX, Df_Hz_RX;

float Start_Ph_Pil0, Start_Ph_Pil1;

float Full_Energy, Tone_Space_Energy, R_Corr;

complex_float *Compl_Exp;

int Is_Waiting;

int OFDM_Modem_Type;

unsigned int FB_FF_Flag =0;
unsigned int SFC_Freq_Comp=0;
float TimeOut_Time=0.0f;

unsigned int Filter_TX_Interpolation;
unsigned int Filter_RX_Decimation;
unsigned int use_long_RX_Filter_2NmL;

int Max, Predd;

complex_float tx_filt_phase;
complex_float rx_filt_phase;
complex_float tx_filt_phaserot;
complex_float rx_filt_phaserot;

complex_float Pilot0_RX, Pilot1_RX, preeq, Preeq;
complex_float Pilot_diff,Pilot1_last_RX;

int CRC_Error;

float QAM_Remod_Energy, MSE=0.0f, MSE_dB=0.0f;
float QAM_Remod_Energy_Mean;

float delta_sigma, delta_sigma_mean;

float MSE_Mean, MSE_dB_Mean;

int CRC_Errors_Sum;

float Alpha_Filter;

complex_float A;

int perm_ind, tmp_ind;

float Af;

float Xmax, a, b, c, d;

int Min_Index_TX, Max_Index_TX;

int Min_Index_RX, Max_Index_RX;

//int Min_Index_RX_QAM4, Max_Index_RX_QAM4;

float Start_Phi, Start_Phi_New, Delta_Phi_Symbol;
float Start_Phi_New_FC;

float Start_Phi_TX, Start_Phi_New_TX, Delta_Phi_Symbol_TX;

complex_float Rot_Fr, Rot_Fr_TX;

complex_float Rot_Pilot0, Rot_Pilot1;

int Len_Lin_Regr;

float ow0, ow1, es1, ens1;

float Fr_Pil0, Fr_Pil1;

complex_float Filt0_RX, Filt1_RX;

int Start_Clk_Ph;

int Block_Error_Cnt;

//float Clock_Ph_Offset, Clock_Ph_Offset_Tmp, Ph_Clk0, Ph_Clk1;
float Ph_Clk0, Ph_Clk1;

float CO_0 , CO_1 , dt_rel0 , dt_rel1 , delta_f0_Hz , delta_f1_Hz;

float En_Pil, Ref_Level, Level;

float Amp_Pilot_TX_Ref;

int Level_Error_cnt;
int Level_Error_TimeOut;
int Level_Warning;

int FFwd_Status_local;
int FFwd_Status_distant;

int Scrambler_Ind;
int Scrambler_Seed;	
int Scrambler_Length;	
int Descrambler_Ind;
int Descrambler_Seed;
int Descrambler_Length;	

int FPGA_status;


float *Ph_Pil0;
float *Ph_Pil1;

int Filter_RX_Adaptation;

int Modulation_Type;

int Shaping_Flag;

float AWGN_Sigma_Est;

int N_N_8;

// Test variables and arrays 

int segment ( "seg_extn SHT_NOBITS" )Channel_Simulator_Flag;

int segment ( "seg_extn SHT_NOBITS" )Sim_Frequency_Offset_Flag;

float segment ( "seg_extn SHT_NOBITS" )Start_Phi_T, Start_Phi_New_T, Delta_Phi_Symbol_T;
complex_float segment ( "seg_extn SHT_NOBITS" )Rot_Fr_T;
float segment ( "seg_extn SHT_NOBITS" )Df_Hz_T, Freq_Offset_T;

int segment ( "seg_extn SHT_NOBITS" )Sim_NL_Dist_Flag;

int segment ( "seg_extn SHT_NOBITS" )Sim_Lin_Dist_Flag;

float segment ( "seg_extn SHT_NOBITS" )A_Notch0, A_Notch1, F_Notch0, F_Notch1;
int segment ( "seg_extn SHT_NOBITS" )Sim_AWGN_Flag;

float segment ( "seg_extn SHT_NOBITS" )SNR;
float segment ( "seg_extn SHT_NOBITS" )AWGN_sigma;

int	segment ( "seg_extn SHT_NOBITS" )Sim_NB_Flag;

float segment ( "seg_extn SHT_NOBITS" )F_NB0;   
float segment ( "seg_extn SHT_NOBITS" )F_NB1;   
float segment ( "seg_extn SHT_NOBITS" )A_NB0;   
float segment ( "seg_extn SHT_NOBITS" )A_NB1;   
float segment ( "seg_extn SHT_NOBITS" )Freq_NB0;
float segment ( "seg_extn SHT_NOBITS" )Freq_NB1;
complex_float segment ( "seg_extn SHT_NOBITS" )Rot_NB0;
complex_float segment ( "seg_extn SHT_NOBITS" )Rot_NB1;
float segment ( "seg_extn SHT_NOBITS" )Delta_Phi_NB0;
float segment ( "seg_extn SHT_NOBITS" )Delta_Phi_NB1;
float segment ( "seg_extn SHT_NOBITS" )Start_Phi_NB0;
float segment ( "seg_extn SHT_NOBITS" )Start_Phi_New_NB0;
float segment ( "seg_extn SHT_NOBITS" )Start_Phi_NB1;
float segment ( "seg_extn SHT_NOBITS" )Start_Phi_New_NB1;
float segment ( "seg_extn SHT_NOBITS" )a_NB0;
float segment ( "seg_extn SHT_NOBITS" )a_NB1;

int segment ( "seg_extn SHT_NOBITS" )Sim_Attenuation_Flag;
float segment ( "seg_extn SHT_NOBITS" )A_loss_dB;
float segment ( "seg_extn SHT_NOBITS" )a_loss_base;
// Meas variables

int Skp_cnt;
int Skp_cnt2=0;

float Symbol_Energy_TX;
float Symbol_Power_TX;
float Symbol_Power_TX_Mean;

float Symbol_Energy_RX;
float Symbol_Power_RX;
float Symbol_Power_RX_Mean;

float AWGN_Energy;
float AWGN_Power;
float AWGN_Power_Mean;

float segment ( "seg_extn_init" )  SNR_Inp = 0.0f;
float segment ( "seg_extn_init" )  SNR_Outp = 0.0f;

// *************** Segment extn ****************
asm(".align 2;");
float segment ( "seg_extn SHT_NOBITS" ) C_TX[OFDM_N_MAX];                      // N = 512   (for N = 512)
float segment ( "seg_extn SHT_NOBITS" ) C_RX[OFDM_N_MAX];                      // N = 512   (for N = 512)

complex_float segment ( "seg_extn SHT_NOBITS" ) Eqlzr_backup[OFDM_N_MAX];

complex_float segment ( "seg_extn SHT_NOBITS" ) Filter_TX[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) C_Filter_RX[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) C_Filter_RX_fallback[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) Imp_Filter_RX[2*OFDM_N_MAX];

int segment ( "seg_extn SHT_NOBITS" ) QAM_Index_TX[OFDM_N_MAX];
int segment ( "seg_extn SHT_NOBITS" ) QAM_Index_RX[OFDM_N_MAX];

complex_float segment( "seg_extn SHT_NOBITS" ) Temp_Buf[2*OFDM_N_MAX];
//int segment( "seg_extn SHT_NOBITS" ) Tx_Array[8*OFDM_NL_MAX];
//int segment( "seg_extn SHT_NOBITS" ) Rx_Array[8*OFDM_NL_MAX];
int segment( "seg_extn SHT_NOBITS" ) Tx_Array[8*OFDM_NL_MAX*INTERP_MAX];
int segment( "seg_extn SHT_NOBITS" ) Rx_Array[8*OFDM_NL_MAX*INTERP_MAX];
complex_float segment( "seg_extn SHT_NOBITS" ) Interp_Buf[8*OFDM_NL_MAX];


//int segment ( "seg_extn SHT_NOBITS" ) Bit_Buf_Ext_TX[32*MAX_INOUT_DATA_BUF];
//int segment ( "seg_extn SHT_NOBITS" ) Bit_Buf_Ext_RX[32*MAX_INOUT_DATA_BUF];
complex_float segment ( "seg_extn SHT_NOBITS" ) OFDM_LFM_2_TX[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) OFDM_LFM_2_RX[2*OFDM_N_MAX];
int segment ( "seg_extn SHT_NOBITS" ) dsp_ID;
complex_float segment ( "seg_extn SHT_NOBITS" ) Df_Array[OFDM_NL_MAX*LEN_DF_EST];

complex_float segment ( "seg_extn SHT_NOBITS" ) Overlap_Save_TX[OFDM_N_MAX-OFDM_L_MIN];
complex_float segment ( "seg_extn SHT_NOBITS" ) Overlap_Save_RX[4*(OFDM_N_MAX-OFDM_L_MIN)];
complex_float segment ( "seg_extn SHT_NOBITS" ) Symbol_Buf[8*OFDM_NL_MAX*INTERP_MAX];

//complex_float segment ( "seg_extn SHT_NOBITS" ) OFDM_TX_Filter_Out[8*OFDM_NL_MAX];
//complex_float segment ( "seg_extn SHT_NOBITS" ) OFDM_RX_Filter_In[8*OFDM_NL_MAX];

complex_float segment ( "seg_extn SHT_NOBITS" ) Adapt_Buf[OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) MSE_trend_QAM4[150];
float segment ( "seg_extn SHT_NOBITS" ) MSE_trend_QAMT[1000];
float segment ( "seg_extn SHT_NOBITS" ) MSE_trend_QAMD[SKP_NUM];

float segment ( "seg_extn SHT_NOBITS" ) Time_trend[2500];


float segment ( "seg_extn SHT_NOBITS" ) F_trend_DF[1000];
float segment ( "seg_extn SHT_NOBITS" ) F_trend_CO[1000];
float segment ( "seg_extn SHT_NOBITS" ) F_trend_CO_delta[1000];
int   segment ( "seg_extn SHT_NOBITS" ) F_trend_CW[1000];


complex_float segment ( "seg_extn SHT_NOBITS" ) Complex_Exp_T[2*OFDM_NL_MAX]; 

float segment ( "seg_extn SHT_NOBITS" ) Noise_Density[OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) Sim_Tr_Funct_Mag[4*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) Sim_Overlap_Save[2*(OFDM_N_MAX-OFDM_L_MIN)];
complex_float segment ( "seg_extn SHT_NOBITS" ) Sim_Tr_Funct[4*OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) Abs2_Filter_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_TX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) K_Int_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Kabs2_RX[2*OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_NL_w[2*OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) Win_TF_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Win_NLopt_LFMSize[2*OFDM_N_MAX];

complex_float segment ( "seg_extn SHT_NOBITS" ) Channel_TF[2*OFDM_N_MAX];

/*****************************************************************
**					Optim Section 					**
*****************************************************************/

float segment ( "seg_extn SHT_NOBITS" ) K_Abs_TX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_Abs_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_Sort_TX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_Sort_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_Sort_2_TX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) K_Sort_2_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) F_TX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) F_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Psi_TX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Psi_RX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) Index_K_TX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) Index_K_RX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) Bits_in_SC_TX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) Bits_in_SC_RX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) D2_alloc_TX[OFDM_N_MAX];
int   segment ( "seg_extn SHT_NOBITS" ) D2_alloc_RX[OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) SNR_bin_TX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) SNR_bin_RX[OFDM_N_MAX];
int segment ( "seg_extn SHT_NOBITS" ) Bits_bin_TX[OFDM_N_MAX];
int segment ( "seg_extn SHT_NOBITS" ) Bits_bin_RX[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) SNR_mean_dB;

//float segment ( "seg_extn SHT_NOBITS" )List_of_data_rates[N_DATA_RATES-1];


int RS_Coding_Flag;
int opti;
float half_log2;
float Delta_Opt_TX,  Delta_Opt_RX;
float AWGN_Sigma_Est_TX,AWGN_Sigma_Est_RX;
unsigned int   M_Opt_TX, M_Opt_RX;
unsigned int   M_Opt_max_TX, M_Opt_max_RX;
float K_max_TX,K_max_RX;
float K_Prob_Err;
float Delta_6_rx, Delta_6_tx;


int TCM_optim_inf_bit_TX;
int TCM_optim_inf_bit_RX;
int RS_optim_code_bit_TX;
int RS_optim_code_bit_RX;
int RS_optim_inf_bit_TX;
int RS_optim_inf_bit_RX;

int Optim_inf_bit_TX;
int Optim_inf_bit_RX;

int segment ( "seg_extn SHT_NOBITS" ) Optim1_inf_bit_TX;
int segment ( "seg_extn SHT_NOBITS" ) Optim1_inf_bit_RX;
int segment ( "seg_extn SHT_NOBITS" ) Optim1_M_TX;
int segment ( "seg_extn SHT_NOBITS" ) Optim1_M_RX;
float segment ( "seg_extn SHT_NOBITS" ) Optim1_rate_TX;
float segment ( "seg_extn SHT_NOBITS" ) Optim1_rate_RX;
int Optim1_error_TX;
int Optim1_error_RX;

int segment ( "seg_extn SHT_NOBITS" ) Data_Rate_Mode;
int segment ( "seg_extn SHT_NOBITS" ) Number_of_Service_bits;
unsigned int segment ( "seg_extn SHT_NOBITS" ) Selected_data_rate_IDX;
unsigned int segment ( "seg_extn SHT_NOBITS" ) Selected_data_rate_IDX_prepared;
int User_inf_bits_TX;
int User_inf_bits_RX;

int Optim_sr_inf_bit_TX;
int Optim_sr_inf_bit_RX;

int RS_optim_sr_inf_bit_TX;
int RS_optim_sr_inf_bit_RX;
int RS_optim_sr_code_bit_TX;
int RS_optim_sr_code_bit_RX;
int TCM_optim_sr_inf_bit_TX;
int TCM_optim_sr_inf_bit_RX;

float segment ( "seg_extn SHT_NOBITS" ) Data_rate_max_TX;
float segment ( "seg_extn SHT_NOBITS" ) Data_rate_max_RX;
float segment ( "seg_extn SHT_NOBITS" ) Selected_data_rate_TX, Selected_data_rate_RX;
int segment ( "seg_extn SHT_NOBITS" ) Higher_rate_available;
int segment ( "seg_extn SHT_NOBITS" ) Lower_rate_available;
 

int V_11_clock_error;

int segment ( "seg_extn SHT_NOBITS" ) Optim2_inf_bit_TX;
int segment ( "seg_extn SHT_NOBITS" ) Optim2_inf_bit_RX;
int segment ( "seg_extn SHT_NOBITS" ) Optim2_M_TX;
int segment ( "seg_extn SHT_NOBITS" ) Optim2_M_RX;
float segment ( "seg_extn SHT_NOBITS" ) Optim2_rate_TX;
float segment ( "seg_extn SHT_NOBITS" ) Optim2_rate_RX;
int Optim2_error_TX;
int Optim2_error_RX;

int Optim3_inf_bit_TX;
int Optim3_inf_bit_RX;
float segment ( "seg_extn SHT_NOBITS" ) Optim3_rate_TX;
float segment ( "seg_extn SHT_NOBITS" ) Optim3_rate_RX;
int Optim3_error_TX;
int Optim3_error_RX;

//float TCM_optim_QAM_Energy_TX;
//float TCM_optim_QAM_Energy_RX;
//float TCM_optim_rate_TX;
//float TCM_optim_rate_RX;
float R_target_TX , R_target_RX;





float segment ( "seg_extn SHT_NOBITS" ) R_Temp[OFDM_N_MAX];

int ngr16384;		// borders in SCK, used in optim->bitshift
int ngr4096	;
int ngr1024	;
int ngr256 	;
int ngr64  	;
int ngr16  	;
int ngr4   	;

int M_Opt2_max_TX, M_Opt2_max_RX;
int Delta_bits_tx, Delta_bits_rx;

// Bit Stuffing
int segment ( "seg_extn SHT_NOBITS" ) Stuffing_Period;
int	segment ( "seg_extn SHT_NOBITS" ) Stuffing_symb_per_period;
int Stuffing_Flag_Bit;
float segment ( "seg_extn SHT_NOBITS" ) Check_rate;

// debug sec
float segment ( "seg_extn SHT_NOBITS" ) N_inf_bit_soll, N_inf_bit_ist;
//float Check_rate;
int   segment ( "seg_extn SHT_NOBITS" ) N_bit_dn, N_bit_up;
int   segment ( "seg_extn SHT_NOBITS" ) diff5i, diff3i;
int   segment ( "seg_extn SHT_NOBITS" ) diff4i, diff2i;
float segment ( "seg_extn SHT_NOBITS" ) diff_bit, diff5, diff3;
float segment ( "seg_extn SHT_NOBITS" ) diff4, diff2;
int	  segment ( "seg_extn SHT_NOBITS" ) N_bit_in_period;
//debug sec

/************ Noise and MSE ****************************************/
float segment ( "seg_extn SHT_NOBITS" ) Noise_Density_Sort[OFDM_N_MAX];
int segment ( "seg_extn SHT_NOBITS" ) Noise_Ind[OFDM_N_MAX];


float segment ( "seg_extn SHT_NOBITS" ) Remod_Spectral_Density[OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) MSE_Spectral_Density_Sort[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) MSE_Spectral_Density[OFDM_N_MAX];

float segment ( "seg_extn SHT_NOBITS" ) SNR_TCM_Sort[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) SNR_TCM[OFDM_N_MAX];

int segment ( "seg_extn SHT_NOBITS" ) SNR_TCM_Switch;
int segment ( "seg_extn SHT_NOBITS" ) RX_FILT_ADAPT_Switch;

float segment ( "seg_extn SHT_NOBITS" ) Harris_Window[4*OFDM_N_MAX];

float Mean_Coeff;

complex_float segment ( "seg_extn SHT_NOBITS" ) Overlap_Save_Spectrum_RX[OFDM_N_MAX-OFDM_L_MIN];
complex_float segment ( "seg_extn SHT_NOBITS" ) Ext_Cmplx_Array[4*OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Mag2_Inp[4*OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Mag2l_Inp[4*OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Delta_Sigma_Array[OFDM_N_MAX];


/*****************************************************************
**					Narrowband Noise Detection 					**
*****************************************************************/

unsigned int NB_Detection_Flag;
float segment ( "seg_extn SHT_NOBITS" ) F_Stat_NB[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Mask_NB[OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) Norm_spec_NB[OFDM_N_MAX];
Inf_Array_NB_type segment ( "seg_extn SHT_NOBITS" ) Inf_Array_NB[OFDM_N_MAX];
segment ( "seg_extn SHT_NOBITS" ) float NB_FS_Energy_tot;
segment ( "seg_extn SHT_NOBITS" ) float NB_FS_Energy_wind;
int NB_count;

int nb_i, pos_ptr;
	
float nb_scale;
segment ( "seg_extn SHT_NOBITS" ) float Fmax;
int nb_width, nb_flag;
float Value_left, Value_right;
int nb_span_noise, nb_span_mask;
segment ( "seg_extn SHT_NOBITS" ) float AWGN_Sigma3_Est; 

/*****************************************************************
**					QAM4 Transmission Channel Data 					**
*****************************************************************/

Channel_Data_type segment ( "seg_extn SHT_NOBITS" ) Channel_Data_RX;
Channel_Data_type segment ( "seg_extn SHT_NOBITS" ) Channel_Data_TX;
Channel_Data_type_NL segment ( "seg_extn SHT_NOBITS" ) Channel_Data_NL_RX;
Channel_Data_type_NL segment ( "seg_extn SHT_NOBITS" ) Channel_Data_NL_TX;

int segment ( "seg_extn_init" ) Word_Buf_Prefix[OFDM_NUM_32BW_TX]={
#include "Prefix.h"
};
int segment ( "seg_extn_init" ) Word_Buf_Suffix[OFDM_NUM_32BW_TX]={
#include "Suffix.h"
};

int segment ( "seg_extn_init" ) Word_Buf_CH_Scramble[MAX_QAM4_BUF]={
#include "Channel_Random_Data.h"
};

int segment ( "seg_extn SHT_NOBITS" ) Word_Buf_CH_Data_TX[MAX_QAM4_BUF];
int segment ( "seg_extn SHT_NOBITS" ) Word_Buf_CH_Data_RX[MAX_QAM4_BUF];

int segment ( "seg_extn SHT_NOBITS" ) Step_Tx;
int segment ( "seg_extn SHT_NOBITS" ) N_Symb_Tx;
int segment ( "seg_extn SHT_NOBITS" ) tx_ch_data_counter;
int segment ( "seg_extn SHT_NOBITS" ) rx_ch_data_counter;

int segment ( "seg_extn SHT_NOBITS" ) Decoder_3212_Status;


/*****************************************************************
**					Modem Configuration Parameters				**
*****************************************************************/

int External_Config;

t_OFDM_Config segment ( "seg_extn_init" ) Modem_Config;
t_BW4_8_Mode  segment ( "seg_extn_init" ) BW4_8_Mode;

/*****************************************************************
**					Modem Sync Status				**
*****************************************************************/
 
t_Sync_State  segment ( "seg_extn SHT_NOBITS" ) Sync_State;

/*****************************************************************
**					Modem RESTART counter        				**
*****************************************************************/
int segment ( "seg_extn_init" ) Restart_Counter = 0;
int segment ( "seg_extn_init" ) syncAlm_restartcounter = 0;



/*****************************************************************
**					NL-Opimization		        				**
*****************************************************************/
e_Optim_NL_State_Type segment ( "seg_extn SHT_NOBITS" ) Optim_NL_State;

e_Pilot_Mode_detection_type segment ( "seg_extn SHT_NOBITS" ) Pilot_Mode_detection;

unsigned int segment ( "seg_extn SHT_NOBITS" ) N_selected_TX, N_selected_RX;
unsigned int segment ( "seg_extn SHT_NOBITS" ) L_selected_TX, L_selected_RX;
unsigned int segment ( "seg_extn SHT_NOBITS" ) N_selected, L_selected;
unsigned int segment ( "seg_extn SHT_NOBITS" ) TX_NL_Call;

complex_float segment ( "seg_extn SHT_NOBITS" ) Imp_Resp_S08[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) Imp_Resp_S16[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) Imp_Resp_S32[2*OFDM_N_MAX];
complex_float segment ( "seg_extn SHT_NOBITS" ) Imp_Resp_S64[2*OFDM_N_MAX];
float segment ( "seg_extn SHT_NOBITS" ) FS_Score_08,FS_Score_16,FS_Score_32,FS_Score_64;
float segment ( "seg_extn SHT_NOBITS" ) FS_Score_L;
float segment ( "seg_extn SHT_NOBITS" ) FS_SCORE_L_LIMIT; //later this should be a #define



