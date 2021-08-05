/*-------------------------------------------------------------------------*
 * $Workfile: Master_Message_Handler.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Patrick Langfeld, Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : TLC, ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/* $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/TDM/Master_Message_Handler.c $
 * 
 * 18    29.08.08 15:40 Chhamaa
 * 
 * 14    6.09.06 16:59 Chchlee
 * Bug 368
 * 
 * 13    23.08.06 14:26 Chchlee
 * Issue 368 corrected (error buffer is 211, not 201 bytes long)
 * 
 * 11    5.04.06 15:28 Chhamaa
 * 
 * 10    17.03.06 14:41 Chhamaa
 * 
 * 9     30.09.05 10:55 Chalgyg
 * OFDM_DSP_V0_11_ur (12709051)
 * 
 * 6     26.04.05 8:44 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 4     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 3     24.02.05 7:33 Chchlee
 * 
 * 2     28.01.05 11:49 Ats
 * Exchange TLC
 * 
 * 3     22.12.04 9:32 Chchlee
 * 
 * 1     15.10.04 pl,vgp
 * Initial version
*/
#include <stdlib.h>


#include "../../h/tdm/ofdm_tdm_externs.h"
#include "../../h/TCM/TCM_Codec_externs.h"
#include "../../h/OFDM_CFG_Types.h"
#include "../../h/ofdm_externs.h"
#include "../../h/util/util.h"
#include "../../h/TDM_Service.H"
	
t_OFDM_LongStatus LongStatus;
	float * data_ptr;
	float ** data_ptr_message;
	unsigned int max_length;

extern float idle_time_min;
extern unsigned int F_trend_ctr;
//extern float Clock_Ph_Offset_Delta;
//extern float Phase_trend[256];
//extern float Delta_PPM_Boost[256];
extern unsigned int Phase_trend_ctr;
extern int Phase_trend_ControlCode[256];
extern float dbg_EPSILON_P[256];
extern float dbg_EPSILON_D[256];


//extern unsigned int Ph_test_hop;
//extern unsigned int Ph_test_setrefphase;


extern int Phase_Code_Min_tot;
extern int Phase_Code_Max_tot;
extern int Phase_Code_Min_Map[168];
extern int Phase_Code_Max_Map[168];
extern unsigned int Phase_Code_minmax_mapctr;

extern unsigned int Num_Wait_LRs_halfhour;
//extern unsigned int Num_Wait_14LRs_onehour;

//extern float ofdmMaxSample;

//============================================

void segment ( "seg_ext_pmco" ) Check_for_Master_Message()
{
	void Process_Master_Message(t_TDMServiceHeader *);

	int message_size;
   t_TDMServiceHeader *Message_Header_Ptr;
   



    int Get_Message_from_Master(unsigned int *buffer);
    int Send_Message_to_Master(int *buffer, int len, e_TDMServiceMsgType mtype);
    
	int mes_crc;
	
	if ( (mlen_rx=Get_Message_from_Master(mes_rx)) != 0) 
	{

	Message_Header_Ptr = (t_TDMServiceHeader *)mes_rx;
	
	// Calculate checksum
    // In CRC: ADDR_FIELD+TYPE_FIELD+SIZE_FIELD+USER_DATA_FIELD, i.e 3+USER_DATA_SIZE
    mes_crc = vsum_i_E((int *)&Message_Header_Ptr->destAddr,Message_Header_Ptr->messageSize + 3);
	if (mes_crc == mes_rx[mlen_rx-2]) {
			   Process_Master_Message(Message_Header_Ptr);
			} else {
        	   // Bad CRC: reply NACK (e_TDMServiceMsgType_Nack with 0 length user packet)
         	   Send_Message_to_Master(NULL, 0, e_TDMServiceMsgType_Nack);
         	}
	}
}

void segment ( "seg_ext_pmco" ) Process_Master_Message(t_TDMServiceHeader *Message_Header_Ptr)
{
	void Send_ShortStatus();
	void Send_LongStatus();
	int message_size;
	
	message_size = Message_Header_Ptr->messageSize;

	switch(Message_Header_Ptr->messageType)
	{
	case e_TDMServiceMsgType_OFDMShortStatusData_Req:
		if ( message_size != 0 ) exit(0); // only for debug
		Send_ShortStatus();
	break;

	case e_TDMServiceMsgType_OFDMLongStatusData_Req:
		//if ( message_size != 0 ) exit(0); // only for debug
		Send_LongStatus(Message_Header_Ptr);
	break;
	
	default:
		exit(0);
		break;
	}


}

void segment ( "seg_ext_pmco" ) Send_ShortStatus()
{
	int Send_Message_to_Master(int *buffer, int len, e_TDMServiceMsgType mtype);
	unsigned int temp_ds;
/*
	typedef struct {
	unsigned int       Reserved:    10;
	unsigned int	   BLER:	3; //0 for 0, 1 for >10E-6, >10E-5, >10E-4, >10E-3, >10E-2, 6 for >10E-1, 7 for unknown
	unsigned int	   SelectedRate:	3; // 0-4(rate0-4) , 7(unknown)
	unsigned int	   MaxRate:	10; // maximum data rate 0-1023kbit/s
	t_GUARD_Int        GUARD_Int:    3; //L
	t_ORTH_Mode        ORTH_Mode:    3; //N
} t_mode0;

typedef struct {
	unsigned int 	 SNR: 		8;
	unsigned int 	 SYNC: 		8;
	unsigned int 	 DeltaSigma:	8;
	unsigned int 	 EPSM: 		8;
} t_mode1;

typedef struct {
	t_mode0       mode0;
	t_mode1       mode1;
} t_OFDM_ShortStatus;
*/
	t_OFDM_ShortStatus ShortStatus;
	switch (L)
	{
		case 7:
			ShortStatus.mode0.GUARD_Int		= GUARD_Int8; // short status doesn't tell the truth
		break;
		case 8:
			ShortStatus.mode0.GUARD_Int		= GUARD_Int8;
		break;
		case 16:
			ShortStatus.mode0.GUARD_Int		= GUARD_Int16;
		break;
		case 32:
			ShortStatus.mode0.GUARD_Int		= GUARD_Int32;
		break;
		case 64:
			ShortStatus.mode0.GUARD_Int		= GUARD_Int64;
		break;
		default:
			ShortStatus.mode0.GUARD_Int		= GUARD_IntXXX;
		break;
	}
	
	switch (N)
	{
		case 64:
			ShortStatus.mode0.ORTH_Mode		= ORTH_Mode64;
		break;
		case 128:
			if (BW4_8_Mode == high_FN)					   // this cond.is only valid for N=128 if BW=7.5 or 8.0 Low Delay (FN=9.6)
			ShortStatus.mode0.ORTH_Mode		= ORTH_Mode64; // this case was configured with N=64 in HMI and customer likes this answer more
			else											// We don't want to confuse customer so short status doesn't tell the truth here,
			ShortStatus.mode0.ORTH_Mode		= ORTH_Mode128; // this is the truth
		break;
		case 256:
			ShortStatus.mode0.ORTH_Mode		= ORTH_Mode256;
		break;
		case 512:
			ShortStatus.mode0.ORTH_Mode		= ORTH_Mode512;
		break;
		default:
			ShortStatus.mode0.ORTH_Mode		= ORTH_ModeXXX;
		break;
	}
	
	if (Data_rate_max_RX < Data_rate_max_TX)
		ShortStatus.mode0.MaxRate		= ((unsigned int) (Data_rate_max_RX / 1000.0f)) & 0x3FF; 
	else
		ShortStatus.mode0.MaxRate		= ((unsigned int) (Data_rate_max_TX  / 1000.0f)) & 0x3FF; 
	 
	ShortStatus.mode0.SelectedRate	= Selected_data_rate_IDX & 0x7;
			

	if (	( RX_OFDM_Engine_state == RX_Engine_QAM_M_Data)
    	 || ( RX_OFDM_Engine_state == RX_Engine_QAM4)
    		)
    {
	ShortStatus.mode0.BLER			= 0;

	if (BLER_Est > 1.0E-6f) ShortStatus.mode0.BLER			= 1;
	if (BLER_Est > 1.0E-5f) ShortStatus.mode0.BLER			= 2;
	if (BLER_Est > 1.0E-4f) ShortStatus.mode0.BLER			= 3;
	if (BLER_Est > 1.0E-3f) ShortStatus.mode0.BLER			= 4;
	if (BLER_Est > 1.0E-2f) ShortStatus.mode0.BLER			= 5;
	if (BLER_Est > 1.0E-1f) ShortStatus.mode0.BLER			= 6;
    } else  ShortStatus.mode0.BLER			= 7; // unknown


	
	ShortStatus.mode1.EPSM = (unsigned int)(-MSE_dB);
	temp_ds = (unsigned int) delta_sigma;
	if (temp_ds > 0xFF) temp_ds = 0xFF;
	ShortStatus.mode1.DeltaSigma = temp_ds & 0xFF;

		
	ShortStatus.mode1.SYNC = Sync_State & 0xFF;
	
	ShortStatus.mode1.SNR =  ((unsigned int) SNR_mean_dB) & 0xFF;



	Send_Message_to_Master((int *)&ShortStatus, 2, e_TDMServiceMsgType_OFDMShortStatusData_Resp);

}

void segment ( "seg_ext_pmco" ) Send_LongStatus(t_TDMServiceHeader *Message_Header_Ptr2)
{
	int Send_Message_to_Master(int *buffer, int len, e_TDMServiceMsgType mtype);

//	float * data_ptr;
//	float ** data_ptr_message;
//	unsigned int max_length;
	int m;
	int type_i_f;
	int * i_ptr;
	
/*
//----
// LongStatus_IDs are defined in OFDM_CFG_Types.h
	typedef struct
	{
		e_OFDM_LongStatus_ID 	ID;
		unsigned int 			datalength;
		unsigned int			offset;
		unsigned int			step;
		float 		 			data[MAX_LENGTH_LONG_STATUS];
	}
	t_OFDM_LongStatus;

*/
	
	
	// put pointer (to pointer) to received message
	// and increment to point on first data element of message
	data_ptr_message 	= (float **)(&Message_Header_Ptr2);
	*data_ptr_message += sizeof(t_TDMServiceHeader);
	
	// Copy content of message to LongStatus.header
	// Read ID     from message	-> LongStatus.header.ID
	// Read length from message -> LongStatus.header.datalength
	// Read offset from message -> LongStatus.header.offset
	// Read step   from message -> LongStatus.header.step
	vcopy_EE((float *)&LongStatus.header.ID, *data_ptr_message,sizeof(t_OFDM_LongStatusHeader));

	// Copy content of requested data to -> LongStatus.data[]
	// Prepare pointer to requested data according to LongStatus_ID of data 
	type_i_f=0;
	switch (LongStatus.header.ID)
	{
		case e_LongStID_Software_Version_ID:
			type_i_f=1;
			i_ptr=(int *)&Software_Version_ID;
			max_length = 1;
		break;
		case e_LongStID_MSE_trend_QAM4:
			data_ptr=(float *)&MSE_trend_QAM4[0];
			max_length = 150;
		break;
		case e_LongStID_MSE_trend_QAMT		:
			data_ptr=(float *)&MSE_trend_QAMT[0];
			max_length = 1000;
		break;
		case e_LongStID_MSE_trend_QAMD		:
			data_ptr=(float *)&MSE_trend_QAMD[0];
			max_length = 10000;
		break;
		case e_LongStID_Noise_Density:
			data_ptr=(float *)&Noise_Density[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Noise_Density_Sort:
			data_ptr=(float *)&Noise_Density_Sort[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Eqlzr:
			data_ptr=(float *)&Eqlzr[0];
			max_length = 2*OFDM_N_MAX;
		break;
		case e_LongStID_Eqlzr_backup:
			data_ptr=(float *)&Eqlzr_backup[0];
			max_length = 2*OFDM_N_MAX;
		break;
		case e_LongStID_Error_Count:
			type_i_f=1;
			i_ptr=(int *)&Error.Count;
			max_length = sizeof(t_Error_type);
		break;
		case e_LongStID_Error_Last:
			type_i_f=1;
			i_ptr=(int *)&Error.Number[Error.Count].ID;
			max_length = 1;
		break;
		case e_LongStID_QAM_Index_TX:
			type_i_f=1;
			i_ptr=(int *)&QAM_Index_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_QAM_Index_RX:
			type_i_f=1;
			i_ptr=(int *)&QAM_Index_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Abs2_Filter_RX:
			data_ptr=(float *)&Abs2_Filter_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Bits_in_SC_TX:
			type_i_f=1;
			i_ptr=(int *)&Bits_in_SC_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Bits_in_SC_RX:
			type_i_f=1;
			i_ptr=(int *)&Bits_in_SC_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_SNR_bin_TX:
			data_ptr=(float *)&SNR_bin_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_SNR_bin_RX:
			data_ptr=(float *)&SNR_bin_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Bits_bin_TX:
			type_i_f=1;
			i_ptr=(int *)&Bits_bin_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Bits_bin_RX:
			type_i_f=1;
			i_ptr=(int *)&Bits_bin_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Mag2l_Inp:
			data_ptr=(float *)&Mag2l_Inp[0];
			max_length = 4*OFDM_N_MAX;
		break;
		case e_LongStID_Mask_NB:
			data_ptr=(float *)&Mask_NB[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_F_Stat_NB:
			data_ptr=(float *)&F_Stat_NB[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_Channel_TF:
			data_ptr=(float *)&Channel_TF[0];
			max_length = 4*OFDM_N_MAX;
		break;
		case e_LongStID_K_TX:
			data_ptr=(float *)&K_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_K_RX:
			data_ptr=(float *)&K_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_K_Abs_TX:
			data_ptr=(float *)&K_Abs_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_K_Abs_RX:
			data_ptr=(float *)&K_Abs_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_K_Sort_2_TX:
			data_ptr=(float *)&K_Sort_2_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_K_Sort_2_RX:
			data_ptr=(float *)&K_Sort_2_RX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_C_TX:
			data_ptr=(float *)&C_TX[0];
			max_length = OFDM_N_MAX;
		break;
		case e_LongStID_C_RX:
			data_ptr=(float *)&C_RX[0];
			max_length = OFDM_N_MAX;
		break;

		case e_LongStID_BLER_EST:
			data_ptr=(float *)&BLER_Est;
			max_length = 1;
		break;
		case e_LongStID_MSE_dB_Mean:
			data_ptr=(float *)&MSE_dB_Mean;
			max_length = 1;
		break;
		case e_LongStID_CRC_Errors_Sum:
			type_i_f=1;
			i_ptr=(int *)&CRC_Errors_Sum;
			max_length = 1;
		break;
		case e_LongStID_M_Opt_TX:
			type_i_f=1;
			i_ptr=(int *)&M_Opt_TX;
			max_length = 1;
		break;
		case e_LongStID_M_Opt_RX:
			type_i_f=1;
			i_ptr=(int *)&M_Opt_RX;
			max_length = 1;
		break;
		case e_LongStID_Analog_Gain:
			data_ptr=(float *)&Analog_Gain;
			max_length = 1;
		break;
		case e_LongStID_Digital_AGC:
			data_ptr=(float *)&Digital_AGC;
			max_length = 1;
		break;
		case e_LongStID_Df_Hz:
			data_ptr=(float *)&Df_Hz;
			max_length = 1;
		break;
		case e_LongStID_NB_count:
			type_i_f=1;
			i_ptr=(int *)&NB_count;
			max_length = 1;
		break;
		case e_LongStID_AWGN_Sigma3_Est:
			data_ptr=(float *)&AWGN_Sigma3_Est;
			max_length = 1;
		break;
		case e_LongStID_AWGN_Sigma_Est_TX:
			data_ptr=(float *)&AWGN_Sigma_Est_TX;
			max_length = 1;
		break;
		case e_LongStID_AWGN_Sigma_Est_RX:
			data_ptr=(float *)&AWGN_Sigma_Est_RX;
			max_length = 1;
		break;
		case e_LongStID_Data_rate_max_TX:
			data_ptr=(float *)&Data_rate_max_TX;
			max_length = 1;
		break;
		case e_LongStID_Data_rate_max_RX:
			data_ptr=(float *)&Data_rate_max_RX;
			max_length = 1;
		break;
		case e_LongStID_Selected_data_rate_TX:
			data_ptr=(float *)&Selected_data_rate_TX;
			max_length = 1;
		break;
		case e_LongStID_Selected_data_rate_RX:
			data_ptr=(float *)&Selected_data_rate_RX;
			max_length = 1;
		break;
	
		case e_LongStID_FS_Score_L:	
			data_ptr=(float *)&FS_Score_L;
			max_length = 1;
		break;
	
		case e_LongStID_FS_SCORE_L_LIMIT:
			data_ptr=(float *)&FS_SCORE_L_LIMIT;
			max_length = 1;
		break;
		
		case e_LongStID_SNR_TCM:
			data_ptr=(float *)&SNR_TCM[0];
			max_length = OFDM_N_MAX;
		break;
		
		// Switch to activate / deactivate Measurement Routines in OFDM_engine_RX
		case e_LongStID_SNR_TCM_SWITCH:
			SNR_TCM_Switch ^= 1; // Switch from 0 to 1, resp. from 1 to 0
			if (SNR_TCM_Switch==0) vset_f_E(SNR_TCM,0.0f,sizeof(SNR_TCM));
			// return new state
			type_i_f=1;
			i_ptr=(int *)&SNR_TCM_Switch;
			max_length = 1;
		 break;
		 
		case e_LongStID_RX_FILT_ADAPT_SWITCH:
			RX_FILT_ADAPT_Switch ^= 1; // Switch from 0 to 1, resp. from 1 to 0
			// return new state
			type_i_f=1;
			i_ptr=(int *)&RX_FILT_ADAPT_Switch;
			max_length = 1;
		 break;
	
		case e_LongStID_Time_trend: 		//IDLE TIMER Array
			data_ptr=(float *)&Time_trend[0];
			max_length = 2500;
		break;
		
		case e_LongStID_idle_time_min: 		//IDLE TIMER Array
			data_ptr=(float *)&idle_time_min;
			max_length = 1;
		break;
		
		case e_LongStID_MSE_Spectral_Density:
			data_ptr=(float *)&MSE_Spectral_Density_Sort[0];
			max_length = OFDM_N_MAX;
		break;
		//-----------------------------------------------------------
		case e_LongStID_F_trend_CO: //10
			data_ptr=(float *)&F_trend_CO[0];
			max_length = sizeof(F_trend_CO);
		break;
		case e_LongStID_F_trend_CO_delta: //11
			data_ptr=(float *)&F_trend_CO_delta[0];
			max_length = sizeof(F_trend_CO_delta);
		break;
		case e_LongStID_F_trend_DF: //56
			data_ptr=(float *)&F_trend_DF[0];
			max_length = sizeof(F_trend_DF);
		break;
		case e_Long_StID_F_trend_CW: //64
 			//F_trend_CW[F_trend_ctr++] = AD5321_Control_Code;	
			type_i_f=1;
			i_ptr=(int *)&F_trend_CW[0];
			max_length = sizeof(F_trend_CW);
		break;	
		case e_Long_StID_F_trend_ctr: //58
			type_i_f=1;
			i_ptr=(int *)&F_trend_ctr;
			max_length = 1;
		break;
		case e_Long_StID_F_trend_rst: //57
			vset_f_E(F_trend_CO,0.0f,sizeof(F_trend_CO));
			vset_f_E(F_trend_CO_delta,0.0f,sizeof(F_trend_CO_delta));
			vset_f_E(F_trend_DF,0.0f,sizeof(F_trend_DF));
			vset_i_E(F_trend_CW,0,sizeof(F_trend_CW));
			F_trend_ctr=0;
			type_i_f=1;
			i_ptr=(int *)&F_trend_ctr;
			max_length = 1;
		break;
		case e_Long_StID_Clock_Ph_Offset_Delta: //60
			data_ptr=(float *)&dbg_EPSILON_P[0];
			max_length = 256;
		break;
		
		case e_Long_StID_Phase_trend_ctr: //61
			type_i_f=1;
			i_ptr=(int *)&Phase_trend_ctr;
			max_length = 1;
		 break;	
		
		case e_Long_StID_Phase_trend_rst: //62
//			vset_f_E(Phase_trend,0.0f,sizeof(Phase_trend));
			Phase_trend_ctr=0;
			type_i_f=1;
			i_ptr=(int *)&Phase_trend_ctr;
			max_length = 1;
		break;	
		
		case e_Long_StID_Phase_trend: //63
			data_ptr=(float *)&dbg_EPSILON_D;
			max_length = 256;
		break;	
		
		
		
		case e_Long_StID_Phase_trend_ControlCode: //65
            //Phase_trend_ControlCode[Phase_trend_ctr]=Phase_Control_Code_Delta;
			type_i_f=1;
			i_ptr=(int *)&Phase_trend_ControlCode[0];
			max_length = sizeof(Phase_trend_ControlCode);
		break;	

		case e_Long_StID_Phaseminmaxmapctr: //   = 70,
			type_i_f=1;
			i_ptr=(int *)&Phase_Code_minmax_mapctr;
			max_length = 1;
		break;
		case e_Long_StID_Phasemintot: //			= 71,
			type_i_f=1;
			i_ptr=(int *)&Phase_Code_Min_tot;
			max_length = 1;
		break;
		case e_Long_StID_Phasemaxtot://			= 72,
			type_i_f=1;
			i_ptr=(int *)&Phase_Code_Max_tot;
			max_length = 1;
		break;
		case e_Long_StID_Phaseminmap://			= 73,
			type_i_f=1;
			i_ptr=(int *)&Phase_Code_Min_Map[0];
			max_length = sizeof(Phase_Code_Min_Map);
		break;
		case e_Long_StID_Phasemaxmap://			= 74,
			type_i_f=1;
			i_ptr=(int *)&Phase_Code_Max_Map[0];
			max_length = sizeof(Phase_Code_Max_Map);
		break;
		/*
		case e_Long_StID_Ph_test_hop_switch://	=75
			Ph_test_hop ^= 1; //switch from 0 to 1 and from 1 to 0
			type_i_f=1;
			i_ptr=(int *)&Ph_test_hop;
			max_length = 1;
		break;
		
		case e_Long_StID_Ph_test_LRlen_inc://	=76
			Modem_Config.phaseControlParam[1] += 1.0f;
			data_ptr=(float *)&Modem_Config.phaseControlParam[0];
			max_length = N_PH_CTRL_PARAM;
		break;
		case e_Long_StID_Ph_test_LRlen_dec://	=77
			if (Modem_Config.phaseControlParam[1] >= 2.0f)
				Modem_Config.phaseControlParam[1] -= 1.0f;
			data_ptr=(float *)&Modem_Config.phaseControlParam[0];
			max_length = N_PH_CTRL_PARAM;
		break;
	
		case e_Long_StID_Ph_test_HUB_inc://	=78
			Modem_Config.phaseControlParam[0] += 5.0f;
			data_ptr=(float *)&Modem_Config.phaseControlParam[0];
			max_length = N_PH_CTRL_PARAM;
		break;
		case e_Long_StID_Ph_test_HUB_dec://	=79
			Modem_Config.phaseControlParam[0] -= 5.0f;
			data_ptr=(float *)&Modem_Config.phaseControlParam[0];
			max_length = N_PH_CTRL_PARAM;
		break;
		
		
		case e_Long_StID_Ph_test_setRefPhase: // 80
			Ph_test_setrefphase = 1;
			type_i_f=1;
			i_ptr=(int *)&Ph_test_setrefphase;
			max_length = 1;
		break;
		
		case e_Long_StID_Ph_test_HUB_invert: //	= 81,
			Modem_Config.phaseControlParam[0] *= -1.0f;
			data_ptr=(float *)&Modem_Config.phaseControlParam[0];
			max_length = N_PH_CTRL_PARAM;
		break;
		*/
		case e_Long_StID_phaseControlParam:	// = 82
			data_ptr=(float *)&Modem_Config.phaseControlParam[0];
			max_length = N_PH_CTRL_PARAM;
		break;
		
		case e_Long_StID_Num_Wait_LRs_halfhour://	= 83,
			type_i_f=1;
			i_ptr=(int *)&Num_Wait_LRs_halfhour;
			max_length = 1;
		break;
		/*
		case e_Long_StID_ofdmMaxSample: // =90
			data_ptr=(float *)&ofdmMaxSample;
			max_length = 1;
		break;

		case e_Long_StID_ofdmMaxSampleReset:// =91
			ofdmMaxSample = 0.0f;
			data_ptr=(float *)&ofdmMaxSample;
			max_length = 1;
		
		break;
		*/
		case e_Long_StID_ofdmRestartCounter://	= 92,
			type_i_f=1;
			i_ptr=(int *)&Restart_Counter;
			max_length = 1;
		break;

		
		case e_LongStID_reserved6:
		default:
			data_ptr=(float *)&LongStatusDefaultAnswer[0];
			max_length = 1024;
		break;	
		
			
	}
	
	// Check length and step information for valid values
	if (LongStatus.header.step 		 < 1) LongStatus.header.step       = 1;
	if (LongStatus.header.datalength < 1) LongStatus.header.datalength = 1;
	
	// Check if size of requested data set is smaller than real size (max_length) of data set 
	if (max_length < (LongStatus.header.offset + (LongStatus.header.datalength-1)*LongStatus.header.step+1) )
		{  // Error in Length
			asm("nop;");	
			asm("nop;");
			asm("nop;");
			type_i_f=0;
			data_ptr=(float *)&LongStatusDefaultAnswer[0];
			LongStatus.header.step = 1;
			LongStatus.header.datalength = 10;
			LongStatus.header.offset = 0;
			
		}

	// Prepare pointer and copy
	if (type_i_f == 1) // integer or float
	{
	// copy int values to LongStatus.data[]
	i_ptr += LongStatus.header.offset; 
	for (m=0;m<LongStatus.header.datalength;m++)
		{
		LongStatus.data[m] = (float) (*i_ptr);
		i_ptr += LongStatus.header.step;		
		}
	}
	// copy float values to LongStatus.data[]
	else
	{
	data_ptr+=LongStatus.header.offset; 	
	vcopy_s_EE( (float *)&LongStatus.data[0], 1, data_ptr,LongStatus.header.step , LongStatus.header.datalength );	
	}
	// Send LongStatus
	Send_Message_to_Master((int *)&LongStatus, LongStatus.header.datalength+sizeof(t_OFDM_LongStatusHeader), e_TDMServiceMsgType_OFDMLongStatusData_Resp);

	
}
