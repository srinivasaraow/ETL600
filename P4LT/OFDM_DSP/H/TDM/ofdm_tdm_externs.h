#ifndef OFDM_TDM_EXTERNS_H
#define OFDM_TDM_EXTERNS_H

#include <complex.h>
#include "ofdm_tdm_control_defs.h"
#include "../tdm_int_cfg.h"
#include "../tdm_int_types.h"

#include "../p4lt_tdm/p4lt_tdm_defs.h"
#include "../ofdm_defs.h"
#include "../ofdm_cfg_types.h"

// Flags
extern int IsModemStarted, Toggle_TDM_TX;

// OFDM Array of RX channel enable mask  bits for maximum allowed 128 channels
extern int TDM_RX_CHANNEL_MASK[4];
// OFDM Array of TX channel enable mask  bits for maximum allowed 128 channels
extern int TDM_TX_CHANNEL_MASK[4];

// TDM intermediate BUFFERS
extern complex_float ofdm_sig_tx_buffer[2*INTERP_MAX*OFDM_NL2_MAX+2*OFDM_IO_SIGNAL_CSAMPLES];
extern complex_float ofdm_sig_rx_buffer[2*INTERP_MAX*OFDM_NL2_MAX+2*OFDM_IO_SIGNAL_CSAMPLES];
extern t_OFDM_output_tdm_block  OFDM_Tx_tdm_buffer[2];   // FE Tx data buffers
extern t_OFDM_input_tdm_block   OFDM_Rx_tdm_buffer[2];   // FE Rx data buffers

extern complex_float *out_bptr, *out_first_bptr, *out_last_bptr, *out_cur_bptr;

extern complex_float *in_bptr, *in_first_bptr, *in_last_bptr, *in_cur_bptr;

extern int data_2tx_request, data_2tx_grant, data_2rx_request, data_2rx_grant;

extern t_OFDM_input_tdm_block  *OFDM_Rx_tdm_bptr;     // pointer to received TDM buffer
extern t_OFDM_output_tdm_block *OFDM_Tx_tdm_bptr;     // pointer to buffer to transmit via TDM

//extern int n_rest_csamples;

// TDM RX Control Channel buffer variables
extern int ControlRX_Service_Buffer[CONTROL_RX_SERVICE_BUFFER_LENGTH];
extern int ControlRX_Service_Write_Ind;
extern int ControlRX_Service_Read_Ind;
extern int *ControlRX_Service_Buffer_Ptr;
extern int ControlRX_Service_Buffer_Length;
extern int ControlRX_Service_Buffer_Overrun;

extern int ControlRX_Service_Message_Ready;
extern int ControlRX_Service_Busy_Flag;
extern int ControlRX_Service_Corrupted_Message;
extern int ControlRX_Service_Number_of_Messages;
extern int ControlRX_Service_Read_Is_Updated;
extern int ControlRX_Service_Number_of_Words;
extern int ControlRX_Service_Delivered_Words;
extern int ControlRX_Service_Secondary_Start;


// TDM TX Control Channel buffer variables
extern int ControlTX_Service_Buffer[CONTROL_TX_SERVICE_BUFFER_LENGTH];
extern int ControlTX_Service_Write_Ind;
extern int ControlTX_Service_Read_Ind;
extern int *ControlTX_Service_Buffer_Ptr;
extern int ControlTX_Service_Buffer_Length;
extern int ControlTX_Service_Buffer_Empty;

 
extern int ControlTX_Service_Write_Is_Updated;
extern int ControlTX_Service_Delivered_Words;
extern int ControlTX_Service_Number_of_Words;

extern int wait_ControlTX_ctr;
// TDM control Master DSP -> OFDM DSP Line
extern int mlen_rx;
extern int mes_crc;
extern unsigned int mes_rx[128];
extern t_TDMServiceHeader *Message_Header_Ptr;

// TDM control OFDM DSP -> Master DSP Line
extern unsigned int mes_tx[CONTROL_TX_SERVICE_BUFFER_LENGTH];

// TDM OFDM Alarm	(lifesign, datarateidx..)
extern t_OFDM_Alarm OFDM_Alarm;

extern float LongStatusDefaultAnswer[2*OFDM_N_MAX];


#endif

	
