/*-------------------------------------------------------------------------*
 * $Workfile: NSK_DSP_Constant.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : T.Gnus
 * Remarks      :  
 * Purpose      : NSK Constants
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/NSK_DSP/H/NSK_DSP_Constant.h $ 
 * 
 * 4     3.08.05 17:32 Chtognu
 * new Filters for Bitrates from 200 to 1200bps
 * 
 * 3     1.06.05 15:55 Chtognu
 * 50 baud modem added
 * 
 * Initial Version
 *-------------------------------------------------------------------------*/
 
#ifndef NSK_DSP_CONSTANT_H
#define NSK_DSP_CONSTANT_H

/* INCLUDE FILES ***********************************************************/

/* GLOBAL CONSTANT DECLARATIONS ********************************************/

//Shift frequencies from center frequency in Hz (depending on baudrate and bandwidth):
#define F_SHIFT_50_120         30
#define F_SHIFT_100_240        60
#define F_SHIFT_200_360        90
#define F_SHIFT_200_480       120
#define F_SHIFT_300_480       120
#define F_SHIFT_600_960       240
#define F_SHIFT_1200_2400     400
#define F_SHIFT_1200_1640     400

// FSK TX  FPGA OVERSAMPLÌNG FACTOR
#define FSK_TX_OVERSAMPLING    12 // gnus original
//#define FSK_TX_OVERSAMPLING    96 // gnus original

// FSK DSP RX INTERPOLATION RATIO
#define FSK_RX_INTERP_RATIO              96.0 // gnus original
//#define FSK_RX_INTERP_RATIO            12.0  // gnus test
#define RECIPROCAL_FSK_RX_INTERP_RATIO   1/FSK_RX_INTERP_RATIO

// FPGA Interface
#define FPGA_SAMPLING_RATE       1024000 // FPGA output sampling rate in Hz
#define FPGA_MAIN_LOOP_INTERVAL  96     // Main loop interval is FPGA sampling rate (FPGA_SAMPLING_RATE)
									 	      // of 1.024MHz divided by the main loop repetition rate of 10.667kHz


//FPGA Masks
#define FPGA_RUDN_NO_TRANS    127
#define FPGA_RUDN_NEW_STATE   128

//FPGA Bit rate Mask
#define BRT_Baud_19200      0x00000001
#define BRT_Baud_9600       0x00000002
#define BRT_Baud_4800       0x00000004
#define BRT_Baud_2400       0x00000008
#define BRT_Baud_1200       0x00000010
#define BRT_Baud_600        0x00000020
#define BRT_Baud_300        0x00000040
#define BRT_Baud_200        0x00000060
#define BRT_Baud_100        0x000000C0

// DAPSK general parameters
#define DAPSK_F_INTERM      96000 // intermediate Frequency
#define DAPSK_M             4     // Number of bits per Symbol


/* GLOBAL TYPE DECLARATIONS ************************************************/


#endif
