/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_externs.h $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM modem main parameter definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /branches/Source MAS/P4LT/OFDM_DSP/H/OFDM_externs.h $
 * 
 * 18    24.07.08 15:19 Chhamaa
 * 
 * 13    5.04.06 15:28 Chhamaa
 * 
 * 13    17.03.06 14:41 Chhamaa
 * 
 * 11    16.09.05 9:42 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 6     26.04.05 8:45 Chalgyg
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
 * 6     3.12.04 9:42 Chhamaa
 * 
 * 4     30.09.04 8:42 Leeb02
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

extern float pm Fl_Array_I[4*OFDM_N_MAX];      // 4N = 2048 (for N = 512)

// *************** Segment Cmplx ****************

extern int Cmplx[8*OFDM_N_MAX];			           // 8N = 4096 (for N = 512)

// *************** Segment dmArray_R ****************

extern float  Fl_Array_R[4*OFDM_NL_MAX+4*OFDM_N_MAX];       // 4(N+L) = 2304 (for N = 512)

// *************** Segment dmda ****************

extern complex_float Eqlzr[OFDM_N_MAX];          // 2N = 1024 (for N = 512)
                     
extern int QAM_Index[OFDM_N_MAX];

extern float Win_Synt[OFDM_L_MAX];

extern float Ph_Pil_t0[LEN_DF_EST_DATA];

extern float Ph_Pil_t1[LEN_DF_EST_DATA];

extern int N;
extern int N2;
extern int N4;
extern int L;
extern int NL;
extern int NL2;
extern int NL2Q;
extern int N_L;
extern int N_pil;
extern int N_tone;
extern int Tone_ind0;
extern int Tone_ind1;
extern int Pilot_ind0;
extern int Pilot_ind1;
extern int Spilot_TX_0, Spilot_RX_0, Spilot_TX_1, Spilot_RX_1;

extern int L_PROTO;

extern int NL_SFC;


extern float T_OFDM_symbol;
extern int LifesignWatchDog;

extern int M_TX;
extern int M0_TX;

extern int M_RX;
extern int M0_RX;

extern int M_QAM4;
extern int M0_QAM4;


extern int BW;
extern int FN;

extern float Fbin;

extern complex_float *OFDM_Symbol_TX;

extern float Alpha_Pil;
extern float Alpha_Pil_Destination;

extern complex_float *OFDM_Symbol_RX;

extern float Alpha_Channel;

extern complex_float *Cmplx_Array;

extern float Norm_TX;

extern int  LFM_Index;

extern unsigned int LFM_Size;
extern unsigned int LFM_2_N, LFM_2_N2;

extern unsigned int Temp_Buf_ctr;

extern float Amp_Tone_TX;
extern complex_float tx2tone0_phase;
extern complex_float tx2tone1_phase;
extern complex_float tx2tone0_phaserot;
extern complex_float tx2tone1_phaserot;

extern float Amp_Pilot;

extern float Amp_Pilot_TX;

extern float Amp_Pilot_RX;

extern float Amp_LFM_TX;


extern float Scale_QAM_TX;

extern float Delta_TX;

extern float P_OFDM_TX;

extern float P_OFDM_Spectrum;

extern float P_QAM;

extern int OFDM_Loop_Mode;

extern float Input_Scale_RX;

extern float Output_Scale_TX;

extern int Symbol_Synchro;
extern int Symbol_Synchro_FS;
extern int Symbol_Synchro_FS_rem;


extern float Digital_AGC;




extern float Energy, Energy_Pil0, Energy_Pil1, Power, Power_Pil, 
      FS_Det, Detection_Treshhold;

extern float Analog_Gain, Des_Val_Pow_Pil;

extern int Predistortion_Active;

extern unsigned int OFDM_Modem_DISABLE;

// ********** State Machine TX ************
extern int Num_Symbol_TX;
extern t_TX_Preamble_State 		TX_Preamble_State;
extern int number_of_txp_cycles;
extern int txp_state_counter;
extern int Start_TX;
extern int Start_TX_QAM_Engine;
extern t_TX_QAM_State 			TX_QAM_State;
extern t_TX_OFDM_Engine_State 	TX_OFDM_Engine_State;
extern int txq_state_counter;
extern int tx_state_finished;
extern int Start_CH_DATA_Transmission;
extern int Start_Suffix_Transmission;
extern int Suffix_is_transmitted;
extern int Suffix_is_detected;
extern int tx_wait_for_restart_ctr;


// ********** State Machine RX ************
extern int rx_state_finished;
extern int rxq_state_counter;
extern t_RX_Preamble_State 		RX_Preamble_State;

extern int Start_RX_QAM_Engine;
extern int Pause_RX_QAM_Engine;
extern t_RX_OFDM_Engine_state 	RX_OFDM_Engine_state;
extern t_RX_QAM_State 			RX_QAM_State;


extern int Det_Agc_RX;
extern e_tone_detection_type tone_detected;
extern int pilotAlm_waitcounter;
extern int	Freq_Est_RX;
extern int	Noise_Est_RX;
extern int	LFM_Proc_RX;
extern int QAM4_Detect_RX;

extern int	Optim1_RX;
extern int	Optim2_RX;
extern int	Optim3_RX;


// ************ State Machine Exeption counters *********
// Preamble section
	// Noise Estimation
extern int rx_pilot_mode_ctr;
extern int RX_PilotMode_TimeOut;
extern int rx_noise_est_in;
extern int Noise_Est_Cnt;
extern int rx_noise_est_out;
extern int rx_lfm_in;
extern int rx_qam4_in;



// QAM section
extern int rx_prefix_counter;
extern int rx_suffix_counter;
extern int rx_wait_for_restart_ctr;

extern unsigned int n_blk_tx, n_blk_rx;
extern unsigned int n_blk_tx_data, n_blk_rx_data;
extern unsigned int n_blk_tx_QAM4, n_blk_rx_QAM4;
extern unsigned int n_blk_tx_M_QAM_train, n_blk_rx_M_QAM_train;
extern unsigned int M_QAM_Training_Symbols;





// ************ State Machine Debug *********
extern int Prefix_Suffix_TH;
extern int Prefix_thres;
extern int Suffix_thres;

extern int Software_Version_ID;


// ************* Error Handler **************
extern int dbg_error[];

extern t_Error_type Error;
extern int Error_count_shd;


extern int Df_block_cnt;
extern int Df_subblock_cnt;

extern int Df_valid_result;

extern unsigned int Freq_Offset_Control_Flag;
extern unsigned int Frequency_Control_enable;

extern float Freq_Offset, Clock_Offset, Clock_Phase_Offset, Df_Hz, Df_Hz_2;

extern float Freq_Offset_TX, Df_Hz_TX;
extern float Freq_Offset_RX, Df_Hz_RX;

extern float Start_Ph_Pil0, Start_Ph_Pil1;

extern float Full_Energy, Tone_Space_Energy, R_Corr;

extern complex_float *Compl_Exp;

extern int Is_Waiting;

extern int OFDM_Modem_Type;

extern unsigned int FB_FF_Flag;
extern unsigned int SFC_Freq_Comp;
extern float TimeOut_Time;

extern unsigned int Filter_TX_Interpolation;
extern unsigned int Filter_RX_Decimation;
extern unsigned int use_long_RX_Filter_2NmL;


extern int Max, Predd;

extern complex_float tx_filt_phase;
extern complex_float rx_filt_phase;
extern complex_float tx_filt_phaserot;
extern complex_float rx_filt_phaserot;


extern complex_float Pilot0_RX, Pilot1_RX, preeq, Preeq;
extern complex_float Pilot_diff,Pilot1_last_RX;


extern int CRC_Error;

extern float QAM_Remod_Energy, MSE, MSE_dB;
extern float QAM_Remod_Energy_Mean;

extern float delta_sigma, delta_sigma_mean;

extern float MSE_Mean, MSE_dB_Mean;

extern int CRC_Errors_Sum;

extern float Alpha_Filter;

extern complex_float A;

extern int perm_ind, tmp_ind;

extern float Af;

extern float Xmax, a, b, c, d;

extern int Min_Index_TX, Max_Index_TX;

extern int Min_Index_RX, Max_Index_RX;

//extern int Min_Index_RX_QAM4, Max_Index_RX_QAM4;

extern float Start_Phi, Start_Phi_New, Delta_Phi_Symbol;
extern float Start_Phi_New_FC;

extern float Start_Phi_TX, Start_Phi_New_TX, Delta_Phi_Symbol_TX;

extern complex_float Rot_Fr, Rot_Fr_TX;

extern complex_float Rot_Pilot0, Rot_Pilot1;

extern int Len_Lin_Regr;

extern float ow0, ow1, es1, ens1;

extern float Fr_Pil0, Fr_Pil1;

extern complex_float Filt0_RX, Filt1_RX;

extern int Start_Clk_Ph;

extern int Block_Error_Cnt;

//extern float Clock_Ph_Offset, Clock_Ph_Offset_Tmp, Ph_Clk0, Ph_Clk1;
extern float Ph_Clk0, Ph_Clk1;

extern float CO_0 , CO_1 , dt_rel0 , dt_rel1 , delta_f0_Hz , delta_f1_Hz;


extern float En_Pil, Ref_Level, Level;

extern float Amp_Pilot_TX_Ref;

extern int Level_Error_cnt;
extern int Level_Error_TimeOut;
extern int Level_Warning;

extern int FFwd_Status_local;
extern int FFwd_Status_distant;


extern int Scrambler_Ind;
extern int Scrambler_Seed;	
extern int Scrambler_Length;	

extern int Descrambler_Ind;
extern int Descrambler_Seed;
extern int Descrambler_Length;	

extern int FPGA_status;


extern float *Ph_Pil0;
extern float *Ph_Pil1;

extern int Filter_RX_Adaptation;

extern int Modulation_Type;

extern int Shaping_Flag; 

extern float AWGN_Sigma_Est;

extern int N_N_8;

// Test variables and arrays 

extern int Channel_Simulator_Flag;

extern int Sim_Frequency_Offset_Flag;

extern float Start_Phi_T, Start_Phi_New_T, Delta_Phi_Symbol_T;
extern complex_float Rot_Fr_T;
extern float Df_Hz_T, Freq_Offset_T;

extern int Sim_NL_Dist_Flag;

extern int Sim_Lin_Dist_Flag;

extern float A_Notch0, A_Notch1, F_Notch0, F_Notch1;

extern int Sim_AWGN_Flag;

extern float SNR;
extern float AWGN_sigma;

extern int	Sim_NB_Flag;

extern float F_NB0;   
extern float F_NB1;   
extern float A_NB0;   
extern float A_NB1;   
extern float Freq_NB0;
extern float Freq_NB1;
extern complex_float Rot_NB0;
extern complex_float Rot_NB1;
extern float Delta_Phi_NB0;
extern float Delta_Phi_NB1;
extern float Start_Phi_NB0;
extern float Start_Phi_New_NB0;
extern float Start_Phi_NB1;
extern float Start_Phi_New_NB1;
extern float a_NB0;
extern float a_NB1;

extern int Sim_Attenuation_Flag;
extern float A_loss_dB;
extern float a_loss_base;

// Meas variables

extern int Skp_cnt;
extern int Skp_cnt2;

extern float Symbol_Energy_TX;
extern float Symbol_Power_TX;
extern float Symbol_Power_TX_Mean;

extern float Symbol_Energy_RX;
extern float Symbol_Power_RX;
extern float Symbol_Power_RX_Mean;

extern float AWGN_Energy;
extern float AWGN_Power;
extern float AWGN_Power_Mean;

extern float SNR_Inp;
extern float SNR_Outp;

// *************** Segment extn ****************


extern float C_TX[OFDM_N_MAX];                      // N = 512   (for N = 512)
extern float C_RX[OFDM_N_MAX];                      // N = 512   (for N = 512)

extern complex_float Eqlzr_backup[OFDM_N_MAX];


extern complex_float Filter_TX[2*OFDM_N_MAX];
extern complex_float C_Filter_RX[2*OFDM_N_MAX];
extern complex_float C_Filter_RX_fallback[2*OFDM_N_MAX];
extern complex_float Imp_Filter_RX[2*OFDM_N_MAX];


//extern int Tx_Array[8*OFDM_NL_MAX];
//extern int Rx_Array[8*OFDM_NL_MAX];
extern int Tx_Array[8*OFDM_NL_MAX*INTERP_MAX];
extern int Rx_Array[8*OFDM_NL_MAX*INTERP_MAX];

extern complex_float Interp_Buf[8*OFDM_NL_MAX];



extern int QAM_Index_TX[OFDM_N_MAX];
extern int QAM_Index_RX[OFDM_N_MAX];

extern complex_float Temp_Buf[2*OFDM_N_MAX];
extern complex_float OFDM_LFM_2_TX[2*OFDM_N_MAX];
extern complex_float OFDM_LFM_2_RX[2*OFDM_N_MAX];

extern int  dsp_ID;

extern complex_float Df_Array[OFDM_NL_MAX*N_OFDM_SYMBOLS_PER_SUBBLOCK];

extern complex_float Overlap_Save_TX[OFDM_N_MAX-OFDM_L_MIN];
extern complex_float Overlap_Save_RX[2*(OFDM_N_MAX-OFDM_L_MIN)];
extern complex_float Symbol_Buf[8*OFDM_NL_MAX*INTERP_MAX];

//extern complex_float OFDM_TX_Filter_Out[8*OFDM_NL_MAX];
//extern complex_float OFDM_RX_Filter_In[8*OFDM_NL_MAX];

extern complex_float Adapt_Buf[OFDM_N_MAX];

extern float MSE_trend_QAM4[150]; 
extern float MSE_trend_QAMT[1000];
extern float MSE_trend_QAMD[SKP_NUM];

extern float Time_trend[2500];


extern float F_trend_DF[1000];
extern float F_trend_CO[1000];
extern float F_trend_CO_delta[1000];
extern int F_trend_CW[1000];



extern complex_float Complex_Exp_T[2*OFDM_NL_MAX]; 

extern float Noise_Density[OFDM_N_MAX];

extern complex_float Sim_Overlap_Save[2*(OFDM_N_MAX-OFDM_L_MIN)];
extern complex_float Sim_Tr_Funct[4*OFDM_N_MAX];
extern float Sim_Tr_Funct_Mag[4*OFDM_N_MAX];

extern float Abs2_Filter_RX[OFDM_N_MAX];
extern float K_RX[OFDM_N_MAX];
extern float K_TX[OFDM_N_MAX];
extern int   K_Int_RX[OFDM_N_MAX];
extern float Kabs2_RX[2*OFDM_N_MAX];
extern float K_NL_w[2*OFDM_N_MAX];



extern float Win_TF_RX[OFDM_N_MAX];
extern float Win_NLopt_LFMSize[2*OFDM_N_MAX];

extern complex_float Channel_TF[2*OFDM_N_MAX];



/*****************************************************************
**					Optim Section 					**
*****************************************************************/

extern float K_Abs_TX[OFDM_N_MAX];
extern float K_Abs_RX[OFDM_N_MAX];
extern float K_Sort_TX[OFDM_N_MAX];
extern float K_Sort_RX[OFDM_N_MAX];
extern float K_Sort_2_TX[OFDM_N_MAX];
extern float K_Sort_2_RX[OFDM_N_MAX];
extern float F_TX[OFDM_N_MAX];
extern float F_RX[OFDM_N_MAX];
extern float Psi_TX[OFDM_N_MAX];
extern float Psi_RX[OFDM_N_MAX];
extern int   Index_K_TX[OFDM_N_MAX];
extern int   Index_K_RX[OFDM_N_MAX];
extern int   Bits_in_SC_TX[OFDM_N_MAX];
extern int   Bits_in_SC_RX[OFDM_N_MAX];
extern int   D2_alloc_TX[OFDM_N_MAX];
extern int   D2_alloc_RX[OFDM_N_MAX];

extern float SNR_bin_TX[OFDM_N_MAX];
extern float SNR_bin_RX[OFDM_N_MAX];
extern int Bits_bin_TX[OFDM_N_MAX];
extern int Bits_bin_RX[OFDM_N_MAX];
extern float SNR_mean_dB;

//extern float List_of_data_rates[N_DATA_RATES-1];


extern int RS_Coding_Flag;
extern int opti;
extern float half_log2;

extern float Delta_Opt_TX,  Delta_Opt_RX;
extern float AWGN_Sigma_Est_TX,AWGN_Sigma_Est_RX;
extern unsigned int   M_Opt_TX, M_Opt_RX;
extern unsigned int   M_Opt_max_TX, M_Opt_max_RX;

extern float K_max_TX,K_max_RX;
extern float K_Prob_Err;
extern float Delta_6_rx, Delta_6_tx;

extern int TCM_optim_inf_bit_TX, TCM_optim_inf_bit_RX;
extern int RS_optim_code_bit_TX, RS_optim_code_bit_RX;
extern int  RS_optim_inf_bit_TX,  RS_optim_inf_bit_RX;

extern int Optim_inf_bit_TX, Optim_inf_bit_RX;

extern float R_target_TX , R_target_RX;

extern int Optim1_inf_bit_TX, Optim1_inf_bit_RX;
extern int Optim1_M_TX, Optim1_M_RX;
extern float Optim1_rate_TX, Optim1_rate_RX;
extern int Optim1_error_TX, Optim1_error_RX;

extern unsigned int Selected_data_rate_IDX;
extern unsigned int Selected_data_rate_IDX_prepared;

extern int Data_Rate_Mode;
extern int Number_of_Service_bits;

extern int        User_inf_bits_TX,        User_inf_bits_RX;
extern int     Optim_sr_inf_bit_TX,     Optim_sr_inf_bit_RX;
extern int  RS_optim_sr_inf_bit_TX,  RS_optim_sr_inf_bit_RX;
extern int RS_optim_sr_code_bit_TX, RS_optim_sr_code_bit_RX;
extern int TCM_optim_sr_inf_bit_TX, TCM_optim_sr_inf_bit_RX;

extern float Data_rate_max_TX, Data_rate_max_RX;
extern float Selected_data_rate_TX, Selected_data_rate_RX;
extern int Higher_rate_available, Lower_rate_available;

extern int V_11_clock_error;

extern int Optim2_inf_bit_TX, Optim2_inf_bit_RX;
extern int Optim2_M_TX, Optim2_M_RX;
extern float Optim2_rate_TX, Optim2_rate_RX;
extern int Optim2_error_TX, Optim2_error_RX;

extern int Optim3_inf_bit_TX, Optim3_inf_bit_RX;
extern float Optim3_rate_TX, Optim3_rate_RX;
extern int Optim3_error_TX, Optim3_error_RX;



extern float R_Temp[OFDM_N_MAX];

extern int ngr16384 ;		
extern int ngr4096	;
extern int ngr1024	;
extern int ngr256 	;
extern int ngr64  	;
extern int ngr16  	;
extern int ngr4   	;

extern int M_Opt2_max_TX, M_Opt2_max_RX;
extern int Delta_bits_tx, Delta_bits_rx;

// Bit Stuffing
extern int Stuffing_Period;
extern int	Stuffing_symb_per_period;
extern int Stuffing_Flag_Bit;
extern float Check_rate;

extern float N_inf_bit_soll, N_inf_bit_ist;
//float Check_rate;
extern int N_bit_dn, N_bit_up;
extern int diff5i, diff3i;
extern int diff4i, diff2i;
extern float diff_bit, diff5, diff3;
extern float diff4, diff2;
extern int N_bit_in_period;


/************ Noise and MSE ****************************************/
extern float Noise_Density_Sort[OFDM_N_MAX];
extern int Noise_Ind[OFDM_N_MAX];

extern float Remod_Spectral_Density[OFDM_N_MAX];

extern float MSE_Spectral_Density_Sort[OFDM_N_MAX];
extern float MSE_Spectral_Density[OFDM_N_MAX];

extern float SNR_TCM_Sort[OFDM_N_MAX];
extern float SNR_TCM[OFDM_N_MAX];

extern int SNR_TCM_Switch;
extern int RX_FILT_ADAPT_Switch;


extern float Harris_Window[4*OFDM_N_MAX];

extern float Mean_Coeff;

extern complex_float Overlap_Save_Spectrum_RX[OFDM_N_MAX-OFDM_L_MIN];
extern complex_float Ext_Cmplx_Array[4*OFDM_N_MAX];
extern float Mag2_Inp[4*OFDM_N_MAX];
extern float Mag2l_Inp[4*OFDM_N_MAX];
extern float Delta_Sigma_Array[OFDM_N_MAX];


/*****************************************************************
**					Narrowband Noise Detection 					**
*****************************************************************/
extern unsigned int NB_Detection_Flag;
extern float F_Stat_NB[OFDM_N_MAX];
extern float Mask_NB[OFDM_N_MAX];
extern float Norm_spec_NB[OFDM_N_MAX];
extern Inf_Array_NB_type Inf_Array_NB[OFDM_N_MAX];
extern float NB_FS_Energy_tot;
extern float NB_FS_Energy_wind;
extern int   NB_count;

extern int nb_i, pos_ptr;
	
extern float nb_scale;
extern float Fmax;
extern int nb_width, nb_flag;
extern float Value_left , Value_right;
extern int nb_span_noise, nb_span_mask;
extern float AWGN_Sigma3_Est;

/*****************************************************************
**					QAM4 Transmission Channel Data 					**
*****************************************************************/

extern Channel_Data_type Channel_Data_RX;
extern Channel_Data_type Channel_Data_TX;
extern Channel_Data_type_NL Channel_Data_NL_RX;
extern Channel_Data_type_NL Channel_Data_NL_TX;



extern int Word_Buf_Prefix[OFDM_NUM_32BW_TX];
extern int Word_Buf_Suffix[OFDM_NUM_32BW_TX];

extern int Word_Buf_CH_Scramble[MAX_QAM4_BUF];

extern int Word_Buf_CH_Data_TX[MAX_QAM4_BUF];
extern int Word_Buf_CH_Data_RX[MAX_QAM4_BUF];

extern int Step_Tx;
extern int N_Symb_Tx;
extern int tx_ch_data_counter;
extern int rx_ch_data_counter;

extern int Decoder_3212_Status;


/*****************************************************************
**					Modem Configuration Parameters				**
*****************************************************************/
extern int External_Config;
extern t_OFDM_Config Modem_Config;
extern t_BW4_8_Mode  BW4_8_Mode;


/*****************************************************************
**					Modem Sync Status				**
*****************************************************************/
 
extern t_Sync_State Sync_State;



/*****************************************************************
**					Modem RESTART counter        				**
*****************************************************************/
extern int Restart_Counter;
extern int syncAlm_restartcounter;



/*****************************************************************
**					NL-Opimization		        				**
*****************************************************************/
extern e_Optim_NL_State_Type Optim_NL_State;
extern e_Pilot_Mode_detection_type Pilot_Mode_detection;
extern unsigned int N_selected_TX, N_selected_RX;
extern unsigned int L_selected_TX, L_selected_RX;
extern unsigned int N_selected, L_selected;
extern unsigned int TX_NL_Call;

extern complex_float Imp_Resp_S08[2*OFDM_N_MAX];
extern complex_float Imp_Resp_S16[2*OFDM_N_MAX];
extern complex_float Imp_Resp_S32[2*OFDM_N_MAX];
extern complex_float Imp_Resp_S64[2*OFDM_N_MAX];
extern float FS_Score_08,FS_Score_16,FS_Score_32,FS_Score_64;
extern float FS_Score_L;
extern float FS_SCORE_L_LIMIT; //later this should be a #define




