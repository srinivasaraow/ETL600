/*-------------------------------------------------------------------------*
 * $Workfile: TDM_Init.h $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/Master_DSP/H/TDM_Init.h $
 * 
 * 11    17.01.07 17:56 Chhamaa
 * MAX_WORDS_PER_DMA_AT_SPORT02_TX and TX_CHANNEL_MASK02 modified
 * 
 * 6     12.05.05 21:53 Chchlee
 * 
 * 5     12.05.05 21:34 Chchlee
 * global variables removed
 * 
 * 4     16.02.05 11:06 Chbrbus
 * Activated SPORT02 (External TDM Bus) supervision same  as in ETL500
 * 
 * 3     14.02.05 14:27 Chbrbus
 * Interrupt routine cleaned, preventing read and write in non DMA buffer
 * segments anytime
 * 
 * 2     10.02.05 16:10 Chbrbus
 * Reduced upconverter TDM bus DMA buffer - changed interrupt routine
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 63    17.01.05 13:11 Chharyt
 * go back to full buffers and not half of the size
 * 
 * 62    14.01.05 13:06 Chharyt
 * Code cleaned. 
 * 
 * 61    13.01.05 11:40 Chharyt
 * TX_CHANNEL_MASK02 changed again
 * 
 * 60    12.01.05 16:22 Chharyt
 * Some adjustments on NSD_DSP and Master_DSP external TDM Bus defines
 * MAX_WORDS_PER_DMA_AT_SPORT02_RX changed
 * MAX_WORDS_PER_DMA_AT_SPORT02_TX changed
 * RX_CHANNEL_MASK02 changed
 * TX_CHANNEL_MASK02 changed
 * 
 * 59    11.01.05 10:41 Chharyt
 * Old values recreated
 * 
 * 58    11.01.05 10:29 Chharyt
 * 
 * 57    10.01.05 17:02 Chharyt
 * 
 * 56    10.01.05 16:42 Chharyt
 * 
 * 55    10.01.05 16:30 Chharyt
 * 
 * 54    15.11.04 18:11 Leeb02
 * tdmInit_SetDSPId removed
 * 
 * 53    29.09.04 11:11 Maag01
 * 
 * 51    24.09.04 14:23 Leeb02
 * 
 * 50    24.09.04 13:26 Rytz01
 * Changes from version 49 undone. No good idea!
 * 
 * 49    24.09.04 12:14 Rytz01
 * SPORT02_16BIT_PACKING_ENABLED changed to value 2 because it's also the
 * factor and can be used directly
 * 
 * 48    24.09.04 9:09 Rytz01
 * NUMBER_OF_SPORT02_BUFFERS on NSD_DSP changed from two to four. 
 * 
 * 47    9.09.04 16:54 Leeb02
 * 
 * 46    9.09.04 15:14 Leeb02
 * DSP_ID removed
 * 
 * 45    9.09.04 8:26 Leeb02
 * preparing for removal of compiler switches
 * 
 * 44    26.08.04 10:20 Leeb02
 * 
 * 43    26.08.04 10:18 Leeb02
 * 
 * 42    26.08.04 8:53 Leeb02
 * tx channel mask override implemented
 * 
 * 41    24.08.04 16:59 Rytz01
 * const removed at tx_channel_mask02[4] for NSD_DSP
 * 
 * 40    24.08.04 16:49 Rytz01
 * const removed at tx_channel_mask02[4] for Master 
 * 
 * 39    13.08.04 16:59 Rytz01
 * DSP_ID inserted
 * 
 * 38    11.08.04 15:04 Leeb02
 * 
 * 36    8.07.04 15:35 Bussli02
 * External TDM bus: TS29, 30, 31 and 32 are driven now from TP DSP
 * (fixed), must be changed later from NSD config, Master DSP is driving
 * TS1-28
 * 
 * 35    29.06.04 13:54 Bussli02
 * 
 * 34    28.06.04 19:09 Bussli02
 * 
 * 33    28.06.04 16:27 Bussli02
 * TDM buffer order should now be normal
 * 
 * 32    23.06.04 16:48 Rytz01
 * 
 * 31    23.06.04 16:06 Rytz01
 * 
 * 30    22.06.04 14:50 Bussli02
 * Changed external TDM bus to MFD4 (according to ETL500)
 * 
 * 29    22.06.04 10:35 Rytz01
 * 
 * 28    16.06.04 9:32 Rytz01
 * 
 * 27    9.06.04 15:49 Rytz01
 * 
 * 26    4.06.04 16:33 Rytz01
 * 
 * 25    4.06.04 14:30 Rytz01
 * 
 * 24    4.06.04 9:14 Rytz01
 * AFE and UC TDM02 settings changed
 * 
 * 23    27.05.04 9:29 Rytz01
 * 
 * 22    19.05.04 14:46 Rytz01
 * 
 * 21    19.05.04 13:39 Rytz01
 * 
 * 20    19.05.04 10:42 Rytz01
 * 
 * 19    17.05.04 10:37 Rytz01
 * 
 * 18    12.05.04 10:54 Rytz01
 * All channels in RX direction are enabled for alpha1 test
 * selectiv enabled channels in TX direction
 * You can see what you send!
 * 
 * 17    12.05.04 10:39 Rytz01
 * 
 * 16    12.05.04 10:33 Rytz01
 * Common Defines for all DSP moved to the beginning
 * 
 * 15    12.05.04 9:55 Rytz01
 * All channels on for Alpha1 tests
 * 
 * 14    10.05.04 13:21 Rytz01
 * 
 * 13    10.05.04 9:35 Rytz01
 * 
 * 12    10.05.04 9:07 Rytz01
 * 
 * 11    10.05.04 9:00 Rytz01
 * 
 * 10    10.05.04 8:47 Rytz01
 * 
 * 9     10.05.04 8:42 Rytz01
 * 
 * 8     10.05.04 8:32 Rytz01
 * Multichannel Frame Delay adjusted once more
 * 
 * 7     10.05.04 8:15 Rytz01
 * Multichannel Frame Delay adjusted
 * 
 * 6     7.05.04 14:00 Rytz01
 * 
 * 5     7.05.04 9:53 Rytz01
 * 
 * 4     4.05.04 10:50 Rytz01
 * Prototyp functions added
 * 
 * 3     4.05.04 10:49 Rytz01
 * Defines added
 * 
 * 2     27.04.04 8:24 Rytz01
 * 
 * 1     7.04.04 13:49 Bussli02
 * Initial version - concept study
 * 
 *
 *-------------------------------------------------------------------------*/


#ifndef TDM_INIT_H
#define TDM_INIT_H


/* INCLUDE FILES **************************************************************/
#include "tdm_int_cfg.h"
#include "BASETYPE.H"
#include <def21161.h>

#include "TDM_Service.h"

/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// General: We just want one initialisation function for TDM Bus valid for all DSP's.
// Every DSP might run on differerent fractions or multiples of the 10.666 KHz internal
// system frequency. This makes the DMA Buffer and TCB definitions different for every
// DSP. We use macro definition when compiling this C-File:
//    -D MACRO_NAME
//
// MACRO_NAME:	FE_DSP
//				MASTER_DSP
//				NSD_DSP
//				NSK_DSP
//				OFDM_DSP
//				UC_DSP

// Common Defines for all DSP ************************************************************************
#define NUMBER_OF_TIMESLOTS_SPORT13				MAX_WORDS_PER_DMA_AT_SPORT13_RX
#define ALL_CHANNELS_ON							0xFFFFFFFF
#define P4LT_TDM_INT_TS_NR						96				// number of time slots in TDM frame

#define OFDM_IO_SIGNAL_CSAMPLES					6				// number of OFDM complex samples in TDM frame
//#define TP_TX_SIGNAL_CSAMPLES					3				// number of transmitted TP complex samples in TDM frame

#define				RX_CHANNEL_MASK13      {ALL_CHANNELS_ON,ALL_CHANNELS_ON,ALL_CHANNELS_ON,P4LT_ZERO};


#ifdef MASTER_DSP	// Runs with 10.666 KHz
	#define RTB_FRAMES_PER_MAINLOOP				1
	// SPORT13 , internal TDM 3 wire bus
	#define NUMBER_OF_SPORT13_BUFFERS			2
	#define MAX_WORDS_PER_DMA_AT_SPORT13_RX		P4LT_TDM_INT_TS_NR
	#define MAX_WORDS_PER_DMA_AT_SPORT13_TX		P4LT_TDM_INT_TS_NR
	#define GENERATE_FS1						IRFS
	#define FS_DIVIDER_SPORT13					0x0BFF0000		// FSDIV = fSCLK/fSFS - 1= 3071 (FS generated only in Master DSP)
	#define MULTICHANNEL_FRAME_DELAY13			MFD0			// 0x0
	// selectively enabled tx channels
	#define				TX_CHANNEL_MASK13      {MASTER_DSP_TX_TDM_MASK1,MASTER_DSP_TX_TDM_MASK2,MASTER_DSP_TX_TDM_MASK3,MASTER_DSP_TX_TDM_MASK4};

	// SPORT02 , external TDM 4 wire bus
	#define NUMBER_OF_SPORT02_BUFFERS			2
	#define NUMBER_OF_TIMESLOTS_SPORT02			44
	#define SPORT02_16BIT_PACKING_ENABLED		1
	#define MAX_WORDS_PER_DMA_AT_SPORT02_RX		44			// Receive all TS 0-44 (see mask)		--> better interrupt timing
	#define MAX_WORDS_PER_DMA_AT_SPORT02_TX		40			// O4CV TS: 32-39	// 38, Send TS 0-31 and 6 more (see mask)	--> better interrupt timing
	#define GENERATE_FS0						IRFS
	#define FS_DIVIDER_SPORT02					0x02CF0000		// FSDIV = fSCLK/fSFS - 1= 719 (FS generated only in Master DSP)
	#define MULTICHANNEL_FRAME_DELAY02			MFD4			// 0x4
	#define SPORT02_REGISTER_SPCLT0             (GENERATE_FS0 | SDEN_A | SCHEN_A | SLEN16 | PACK | SENDN);
	#define SPORT02_REGISTER_SPCLT2             (LTDV | SDEN_A | SCHEN_A | SLEN16 | PACK | SENDN);
	#define				RX_CHANNEL_MASK02        {ALL_CHANNELS_ON,0x00000FFF,P4LT_ZERO,P4LT_ZERO};	// channels 0-43 on. channel 44 off --> 16bit packing?!?
    #define             TX_CHANNEL_MASK02        {0x0fffffff,0x000000FF,P4LT_ZERO,P4LT_ZERO};		// 0x000000FF: O4CV time slots 32-39 // 0x0000003F: channels 32-37 on!!! --> better interrupt timing

#endif


#ifdef NSD_DSP		// Runs with 5.333 KHz
	#define RTB_FRAMES_PER_MAINLOOP				2
	// SPORT13 , internal TDM 3 wire bus
	#define NUMBER_OF_SPORT13_BUFFERS			4
	#define MAX_WORDS_PER_DMA_AT_SPORT13_RX		P4LT_TDM_INT_TS_NR
	#define MAX_WORDS_PER_DMA_AT_SPORT13_TX		P4LT_TDM_INT_TS_NR
	#define GENERATE_FS1						0
	#define FS_DIVIDER_SPORT13					0				// 0=external
	#define MULTICHANNEL_FRAME_DELAY13			MFD0			// 0x0
	// selectively enabled tx channels
	#define				TX_CHANNEL_MASK13      {NSD_DSP_TX_TDM_MASK1,NSD_DSP_TX_TDM_MASK2,NSD_DSP_TX_TDM_MASK3,NSD_DSP_TX_TDM_MASK4};

	// SPORT02 , external TDM 4 wire bus
	#define NUMBER_OF_SPORT02_BUFFERS			4
	#define NUMBER_OF_TIMESLOTS_SPORT02			44
	#define SPORT02_16BIT_PACKING_ENABLED		1
	#define MAX_WORDS_PER_DMA_AT_SPORT02_RX		44			// Receive all TS 0-44 (see mask)		--> better interrupt timing
	#define MAX_WORDS_PER_DMA_AT_SPORT02_TX		38			// Send TS 0-31 and 6 more (see mask)	--> better interrupt timing
	#define GENERATE_FS0						0
	#define FS_DIVIDER_SPORT02					0				// 0=external
	#define MULTICHANNEL_FRAME_DELAY02			MFD4			// 0x4
	#define SPORT02_REGISTER_SPCLT0             (SDEN_A | SCHEN_A | SLEN16 | PACK | SENDN);
	#define SPORT02_REGISTER_SPCLT2             (LTDV | SDEN_A | SCHEN_A | SLEN16 | PACK | SENDN);
	#define				RX_CHANNEL_MASK02        {ALL_CHANNELS_ON,0x00000FFF,P4LT_ZERO,P4LT_ZERO};	// channels 0-43 on. channel 44 off --> 16bit packing?!?
    #define             TX_CHANNEL_MASK02        {0xf0000000,0x00000FC0,P4LT_ZERO,P4LT_ZERO};		// channels 38-43 on!!! --> better interrupt timing

#endif

#ifdef FE_DSP		// Runs with 5.333 KHz
	#define RTB_FRAMES_PER_MAINLOOP				2
	// SPORT13 , internal TDM 3 wire bus	
	#define NUMBER_OF_SPORT13_BUFFERS			4
	#define MAX_WORDS_PER_DMA_AT_SPORT13_RX		P4LT_TDM_INT_TS_NR
	#define MAX_WORDS_PER_DMA_AT_SPORT13_TX		P4LT_TDM_INT_TS_NR
	#define GENERATE_FS1						0
	#define FS_DIVIDER_SPORT13					0				// 0=external
	#define MULTICHANNEL_FRAME_DELAY13			MFD0			// 0x0
	// selectiv enabled tx channels
	#define				TX_CHANNEL_MASK13      {FE_DSP_TX_TDM_MASK1,FE_DSP_TX_TDM_MASK2,FE_DSP_TX_TDM_MASK3,FE_DSP_TX_TDM_MASK4};

	// SPORT02 , external TDM 4 wire bus
	#define NUMBER_OF_SPORT02_BUFFERS			2*12			// 12 I/Q values with 12*5.333 KHz incoming
	#define MAX_WORDS_PER_DMA_AT_SPORT02_RX		8				// Is This enough? 1 for the I sample, one for the Q sample
	#define MAX_WORDS_PER_DMA_AT_SPORT02_TX		8
	#define GENERATE_FS0						0
	#define FS_DIVIDER_SPORT02					0				// 0=external
	#define NUMBER_OF_TIMESLOTS_SPORT02			MAX_WORDS_PER_DMA_AT_SPORT02_RX
	#define MULTICHANNEL_FRAME_DELAY02			MFD1			// 0x1
	#define SPORT02_16BIT_PACKING_ENABLED		FALSE
	#define SPORT02_REGISTER_SPCLT0             (SDEN_A | SCHEN_A | SLEN32);
	#define SPORT02_REGISTER_SPCLT2             (LTDV | SDEN_A | SCHEN_A | SLEN32);

	#define				RX_CHANNEL_MASK02        {0x000000FF,P4LT_ZERO,P4LT_ZERO,P4LT_ZERO};
    #define             TX_CHANNEL_MASK02        RX_CHANNEL_MASK02

#endif

#ifdef OFDM_DSP		// Runs with 10.666 KHz
	#define RTB_FRAMES_PER_MAINLOOP				1
	// SPORT13 , internal TDM 3 wire bus
	#define NUMBER_OF_SPORT13_BUFFERS			2
	#define MAX_WORDS_PER_DMA_AT_SPORT13_RX		P4LT_TDM_INT_TS_NR
	#define MAX_WORDS_PER_DMA_AT_SPORT13_TX		P4LT_TDM_INT_TS_NR
	#define GENERATE_FS1						0
	#define FS_DIVIDER_SPORT13					0				// 0=external
	#define MULTICHANNEL_FRAME_DELAY13			MFD0			// 0x0
	// selectiv enabled tx channels
	#define				TX_CHANNEL_MASK13      {OFDM_DSP_TX_TDM_MASK1,OFDM_DSP_TX_TDM_MASK2,OFDM_DSP_TX_TDM_MASK3,OFDM_DSP_TX_TDM_MASK4};

	// SPORT02 , external TDM 4 wire bus
	// Is not yet used for communication with the FPGA, dummy define
	#define NUMBER_OF_SPORT02_BUFFERS			2		
	#define MAX_WORDS_PER_DMA_AT_SPORT02_RX		1		
	#define MAX_WORDS_PER_DMA_AT_SPORT02_TX		1
	#define GENERATE_FS0						0
	#define FS_DIVIDER_SPORT02					0				// 0=external
	#define NUMBER_OF_TIMESLOTS_SPORT02			MAX_WORDS_PER_DMA_AT_SPORT02_RX
	#define MULTICHANNEL_FRAME_DELAY02			MFD0			// 0x0
	#define SPORT02_16BIT_PACKING_ENABLED		FALSE
	#define SPORT02_REGISTER_SPCLT0             (SDEN_A | SCHEN_A | SLEN32);
	#define SPORT02_REGISTER_SPCLT2             (LTDV | SDEN_A | SCHEN_A | SLEN32);

	#define				RX_CHANNEL_MASK02        {ALL_CHANNELS_ON,P4LT_ZERO,P4LT_ZERO,P4LT_ZERO};
    #define             TX_CHANNEL_MASK02        RX_CHANNEL_MASK02

#endif

#ifdef NSK_DSP		// Runs with 10.666 KHz
	#define RTB_FRAMES_PER_MAINLOOP				1
	// SPORT13 , internal TDM 3 wire bus
	#define NUMBER_OF_SPORT13_BUFFERS			2
	#define MAX_WORDS_PER_DMA_AT_SPORT13_RX		P4LT_TDM_INT_TS_NR
	#define MAX_WORDS_PER_DMA_AT_SPORT13_TX		P4LT_TDM_INT_TS_NR
	#define GENERATE_FS1						0
	#define FS_DIVIDER_SPORT13					0				// 0=external
	#define MULTICHANNEL_FRAME_DELAY13			MFD0			// 0x0
	// selectiv enabled tx channels
	#define				TX_CHANNEL_MASK13      {NSK_DSP_TX_TDM_MASK1,NSK_DSP_TX_TDM_MASK2,NSK_DSP_TX_TDM_MASK3,NSK_DSP_TX_TDM_MASK4};

	// SPORT02 , external TDM 4 wire bus
	// Is not yet used for communication with the FPGA, dummy define
	#define NUMBER_OF_SPORT02_BUFFERS			2		
	#define MAX_WORDS_PER_DMA_AT_SPORT02_RX		1		
	#define MAX_WORDS_PER_DMA_AT_SPORT02_TX		1
	#define GENERATE_FS0						0
	#define FS_DIVIDER_SPORT02					0				// 0=external
	#define NUMBER_OF_TIMESLOTS_SPORT02			MAX_WORDS_PER_DMA_AT_SPORT02_RX
	#define MULTICHANNEL_FRAME_DELAY02			MFD0			// 0x0
	#define SPORT02_16BIT_PACKING_ENABLED		FALSE
	#define SPORT02_REGISTER_SPCLT0             (SDEN_A | SCHEN_A | SLEN32);
	#define SPORT02_REGISTER_SPCLT2             (LTDV | SDEN_A | SCHEN_A | SLEN32);

	#define				RX_CHANNEL_MASK02        {ALL_CHANNELS_ON,P4LT_ZERO,P4LT_ZERO,P4LT_ZERO};
    #define             TX_CHANNEL_MASK02        RX_CHANNEL_MASK02

#endif

#ifdef UC_DSP		// Runs with 10.666 KHz
 	#define RTB_FRAMES_PER_MAINLOOP				1
	// SPORT13 , internal TDM 3 wire bus
	#define NUMBER_OF_SPORT13_BUFFERS			2
	#define MAX_WORDS_PER_DMA_AT_SPORT13_RX		P4LT_TDM_INT_TS_NR
	#define MAX_WORDS_PER_DMA_AT_SPORT13_TX		P4LT_TDM_INT_TS_NR
	#define GENERATE_FS1						0
	#define FS_DIVIDER_SPORT13					0				// 0=external
	#define MULTICHANNEL_FRAME_DELAY13			MFD0			// 0x0
	// selectiv enabled tx channels
	#define				TX_CHANNEL_MASK13      {UC_DSP_TX_TDM_MASK1,UC_DSP_TX_TDM_MASK2,UC_DSP_TX_TDM_MASK3,UC_DSP_TX_TDM_MASK4};
	
	// SPORT02 , external TDM 4 wire bus
	#define NUMBER_OF_SPORT02_BUFFERS			2*6				// 6 I/Q values with 6*10.666 KHz incoming
	#define MAX_WORDS_PER_DMA_AT_SPORT02_RX		8				// Is This enough? 1 for the I sample, one for the Q sample
	#define MAX_WORDS_PER_DMA_AT_SPORT02_TX		8
	#define GENERATE_FS0						0
	#define FS_DIVIDER_SPORT02					0				// 0=external
	#define NUMBER_OF_TIMESLOTS_SPORT02			MAX_WORDS_PER_DMA_AT_SPORT02_RX
	#define MULTICHANNEL_FRAME_DELAY02			MFD1			// 0x1
	#define SPORT02_16BIT_PACKING_ENABLED		FALSE
	#define SPORT02_REGISTER_SPCLT0             (SDEN_A | SCHEN_A | SLEN32);
	#define SPORT02_REGISTER_SPCLT2             (LTDV | SDEN_A | SCHEN_A | SLEN32);

	#define				RX_CHANNEL_MASK02        {0x000000FF,P4LT_ZERO,P4LT_ZERO,P4LT_ZERO};
	#define             TX_CHANNEL_MASK02        RX_CHANNEL_MASK02
	
#endif


/*****************************************************************************/
/*                                                                           */
/* TCB = "Transfer Control Block"                                            */
/*                                                                           */
/*   SPORTx TCB format:                                                      */
/*            ECx (length of destination buffer),                            */
/*            EMx (destination buffer step size),                            */
/*            EIx (destination buffer index (initialized to start address)), */
/*            GPx ("general purpose"),                                       */
/*            CPx ("Chain Point register"; points to last address (IIx) of   */
/*                                         next TCB to jump to               */
/*                                         upon completion of this TCB.),    */
/*            Cx  (length of source buffer),                                 */
/*            IMx (source buffer step size),                                 */
/*            IIx (source buffer index (initialized to start address))       */
/*                                                                           */
/*****************************************************************************/
	
/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/

void SPORT02_DMA_init(void);
void SPORT13_DMA_init(void);

void Start_SPORT02(void);
void Start_SPORT13(void);

void StartChainedDMA_SPORT02Tx(void);
void StartChainedDMA_SPORT13Tx(void);
void StartChainedDMA_SPORT13Tx_Double(void); 

void SEGMENT_EXT_CODE tdmInit_OverrideTxChannelMask02_0 (unsigned long channelMask);
void Restart_SPORT02(void);

void SEGMENT_EXT_CODE Init_InternalTDM_BufferPtr(unsigned long *RxDMA_BufferPtr, unsigned long *TxDMA_BufferPtr);
void SEGMENT_EXT_CODE Init_ExternalTDM_BufferPtr(unsigned long *RxDMA_BufferPtr, unsigned long *TxDMA_BufferPtr);

#endif

