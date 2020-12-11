/*-------------------------------------------------------------------------*
 * $Workfile	: sport2.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : V. Pogorilyy
 * Remarks      :  
 * Purpose      : 4 Wire interface driver
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/sport2.h $
 * 
 * 5     10.04.08 9:42 Chhamaa
 * Unused prototype functions removed.
 * 
 * 4     24.07.07 18:50 Chhamaa
 * #include "O4CV_Const.h" added.
 * 
 * 3     07-07-03 10:04 Chvopog
 * Start, Stop  Rx & TX added
 * 
 * 2     11.05.07 13:01 Chhamaa
 * Defines renamed
 * 
 * 1     07-05-09 15:13 Chvopog
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef SPORT2_H
#define SPORT2_H

/* INCLUDE FILES **************************************************************/

#include "O4CV_Const.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
											
#define			BUFFER_SIZE_4W_IF	1920	// collect 1920 samples per buffer 
											// before irq is rised (30ms)
																																
/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef enum   {RX_GAIN_0DB = 0,  RX_GAIN_6DB,  RX_GAIN_12DB,  RX_GAIN_18DB, 
                RX_GAIN_20DB,     RX_GAIN_26DB, RX_GAIN_32DB,  RX_GAIN_38DB
               } t_RxGain;
                
typedef enum   {TX_GAIN_p6DB = 0, TX_GAIN_p3DB, TX_GAIN_0DB,   TX_GAIN_m3DB, 
                TX_GAIN_m6DB,     TX_GAIN_m9DB, TX_GAIN_m12DB, TX_GAIN_m15DB
               } t_TxGain;

typedef struct {
	t_RxGain	rx_gain[N_CHANNELS_ON_BOARD];
	t_TxGain	tx_gain[N_CHANNELS_ON_BOARD];	
} t_4w_Gains;


/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void sport2_init4wireInterface(t_4w_Gains*);
bool sport2_4wireDataAvailable(void);
void sport2_4wireDataLoopback(void);
void sport2_start4wireInterface(void);
void sport2_stop4wireInterface(void);

//--------------------------------------------------------------------------//
// Global variables															//
//--------------------------------------------------------------------------/

//INT16 *g_pRxBuffer4wIf, *g_pTxBuffer4wIf;   
    	
#endif

