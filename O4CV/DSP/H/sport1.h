/*-------------------------------------------------------------------------*
 * $Workfile	: sport1.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : V. Pogorilyy
 * Remarks      :  
 * Purpose      : 2 Wire interface driver
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/sport1.h $
 * 
 * 3     08-02-06 7:49 Chrobif
 * + sport1_isTxRunning() added.
 * 
 * 2     07-06-15 13:16 Chvopog
 * Number of time slots changed
 * 
 * 1     07-06-04 11:46 Chvopog
 * Initial version
 * 
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef SPORT1_H
#define SPORT1_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// SPORT0 word length
#define SLEN_32	0x001f

// DMA flow mode
#define FLOW_1	0x1000
#define SLEN_16	0x000f
#define RDTYPE_SIGN	0x0006

// SPORT1 bits
#define ITFS	0x0200
#define IRFS	0x0200

// length of 30ms buffers in #samples (240)
#define	BUFFER_SIZE_2W_IF	    		240
#define N_CHANNELS_2W					5		// there should be a global define 
#define SLIC_TDM_TIME_SLOT_MASK_RX		0x0000001F
#define SLIC_TDM_TIME_SLOT_MASK_TX		0x0000001F


/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/
void sport1_init2wireInterface(void);
void sport1_startRx2wireInterface(void);
void sport1_startTx2wireInterface(void);
void sport1_stopRx2wireInterface(void);
void sport1_stopTx2wireInterface(void);
bool sport1_isTxRunning(void);
bool sport1_2wireDataAvailable(void);
void sport1_2wireDataLoopback(void);
//--------------------------------------------------------------------------//
// Global variables															//
//--------------------------------------------------------------------------/

//INT16 *g_pRxBuffer2wIf,*g_pTxBuffer2wIf; 
    	
#endif


