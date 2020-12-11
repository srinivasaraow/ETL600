#ifndef OFDM_TDM_GLOBALS_H
#define OFDM_TDM_GLOBALS_H

#include <complex.h>
#include "ofdm_tdm_control_defs.h"
#include "../../h/tdm_int_cfg.h"
#include "../../h/tdm_int_types.h"

#include "../p4lt_tdm/p4lt_tdm_defs.h"
#include "../ofdm_defs.h"

#undef ALIGN2
#ifndef ALIGN2
#define ALIGN2 asm(".align 2;")
#endif

// Flags
int IsModemStarted;

// OFDM Array of RX channel enable mask  bits for maximum allowed 128 channels
int TDM_RX_CHANNEL_MASK[4] = {
     OFDM_DSP_RX_TDM_MASK1, OFDM_DSP_RX_TDM_MASK2, OFDM_DSP_RX_TDM_MASK3, OFDM_DSP_RX_TDM_MASK4	
   };

// OFDM Array of TX channel enable mask  bits for maximum allowed 128 channels
int TDM_TX_CHANNEL_MASK[4] = {   
     OFDM_DSP_TX_TDM_MASK1, OFDM_DSP_TX_TDM_MASK2, OFDM_DSP_TX_TDM_MASK3, OFDM_DSP_TX_TDM_MASK4 
   };

// TDM intermediate BUFFERS
ALIGN2;
complex_float segment ( "seg_extn SHT_NOBITS" ) ofdm_sig_tx_buffer[2*INTERP_MAX*OFDM_NL2_MAX+2*OFDM_IO_SIGNAL_CSAMPLES];
ALIGN2;
complex_float segment ( "seg_extn SHT_NOBITS" ) ofdm_sig_rx_buffer[2*INTERP_MAX*OFDM_NL2_MAX+2*OFDM_IO_SIGNAL_CSAMPLES];
ALIGN2;
t_OFDM_output_tdm_block  OFDM_Tx_tdm_buffer[2];   // FE Tx data buffers
ALIGN2;
t_OFDM_input_tdm_block   OFDM_Rx_tdm_buffer[2];   // FE Rx data buffers


complex_float  *out_bptr = &ofdm_sig_tx_buffer[0],
               *out_first_bptr = &ofdm_sig_tx_buffer[0],
               *out_last_bptr = &ofdm_sig_tx_buffer[2*INTERP_MAX*OFDM_NL2_MAX+2*OFDM_IO_SIGNAL_CSAMPLES-1],
               *out_cur_bptr;
complex_float  *in_bptr = &ofdm_sig_rx_buffer[0], 
               *in_first_bptr = &ofdm_sig_rx_buffer[0],
               *in_last_bptr = &ofdm_sig_rx_buffer[2*INTERP_MAX*OFDM_NL2_MAX+2*OFDM_IO_SIGNAL_CSAMPLES-1],
               *in_cur_bptr;


t_OFDM_input_tdm_block  *OFDM_Rx_tdm_bptr;     // pointer to received TDM buffer
t_OFDM_output_tdm_block *OFDM_Tx_tdm_bptr;     // pointer to buffer to transmit via TDM

//int n_rest_csamples;


// TDM RX Control Channel buffer variables
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Buffer[CONTROL_RX_SERVICE_BUFFER_LENGTH];
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Write_Ind;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Read_Ind;
int segment ( "seg_extn SHT_NOBITS" ) *ControlRX_Service_Buffer_Ptr;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Buffer_Length;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Buffer_Overrun;

int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Message_Ready;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Busy_Flag;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Corrupted_Message;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Number_of_Messages;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Read_Is_Updated;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Number_of_Words;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Delivered_Words;
int segment ( "seg_extn SHT_NOBITS" ) ControlRX_Service_Secondary_Start;


// TDM TX Control Channel buffer variables
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Buffer[CONTROL_TX_SERVICE_BUFFER_LENGTH];
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Write_Ind;
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Read_Ind;
int segment ( "seg_extn SHT_NOBITS" ) *ControlTX_Service_Buffer_Ptr;
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Buffer_Length;
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Buffer_Empty;

int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Write_Is_Updated;
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Delivered_Words;
int segment ( "seg_extn SHT_NOBITS" ) ControlTX_Service_Number_of_Words;

int segment ( "seg_extn SHT_NOBITS" ) wait_ControlTX_ctr;

// TDM control Master DSP -> OFDM DSP Line
int segment ( "seg_extn SHT_NOBITS" ) mlen_rx;
int segment ( "seg_extn SHT_NOBITS" ) mes_crc;
unsigned int segment ( "seg_extn SHT_NOBITS" ) mes_rx[128];
t_TDMServiceHeader *Message_Header_Ptr;

// TDM control OFDM DSP -> Master DSP Line
unsigned int segment ( "seg_extn SHT_NOBITS" ) mes_tx[CONTROL_TX_SERVICE_BUFFER_LENGTH];

// TDM OFDM Alarm	(lifesign, datarateidx..)
t_OFDM_Alarm OFDM_Alarm;

float segment ( "seg_extn SHT_NOBITS" ) LongStatusDefaultAnswer[2*OFDM_N_MAX];
#endif

	
