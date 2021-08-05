/*-------------------------------------------------------------------------*
 * $Workfile	: O4CV_Const.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : O4CV constants used on various platforms (P4LW, O4CV, HMI)
 * Copyright    : ABB Switzerland Ltd., 2006
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/O4CV_Const.h $
 * 
 * 16    6.03.12 10:51 Chthbae
 * number of O4CV changed to 3 on small rack
 * 
 * 15    11-04-18 12:44 Chmazar
 * 
 * 12    07-09-12 14:17 Chrobif
 * Define O4CV_CONFIG_VERSION added.
 * 
 * 11    07-07-18 9:15 Chrobif
 * Defines O4CV_DATA_RATE_5300 and O4CV_DATA_RATE_6300 added.
 * 
 * 10    07-07-17 7:20 Chrobif
 * Define N_CHANNELS_ON_BOARD_PLUS_SPH added.
 * 
 * 9     07-06-04 11:44 Chvopog
 * #define SPH 5 added
 * 
 * 8     27.04.07 18:03 Chhamaa
 * Channel indexes added.
 * 
 * 7     5.04.07 8:51 Chrobif
 * SWID defines which are used on O4CV only moved to O4CV_ConstDsp.h
 * 
 * 6     4.04.07 14:57 Chhamaa
 * #define N_CHANNELS_ON_BOARD added.
 * 
 * 5     30.03.07 15:21 Chrobif
 * Defines for O4CV software identification (SWID) added.
 * 
 * 4     22.03.07 14:31 Chhamaa
 * Rollback to version 2
 * 
 * 2     1.03.07 17:26 Chhamaa
 * #define N_O4CV_BOARDS added.
 * 
 * 1     9.01.07 15:50 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef O4CV_CONST_H
#define O4CV_CONST_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// max. number of O4CV boards
#define N_O4CV_BOARDS				4 

// max. number of O4CV channels
#define N_MUX_CHANNELS				16
#define N_O4CV_CHANNELS				16
#define N_O4CV_CHANNELS_R4_4SLOTS	12

// number of channels on board
#define N_CHANNELS_ON_BOARD			4

// number of channels on board + service phone
#define N_CHANNELS_ON_BOARD_PLUS_SPH	(N_CHANNELS_ON_BOARD + 1)

// channel indexes
#define CH1							0
#define CH2							1
#define CH3							2
#define CH4							3
#define SPH							4

// defines for channel mode
#define CH_MODE_OFF					0
#define CH_MODE_VOICE				1
#define CH_MODE_AUDIO				2

// defines for channel data rate
#define CH_DR_OFF					0
#define CH_VOICE_DR_5300			1
#define CH_VOICE_DR_6300			2

// defines for data rate
#define O4CV_DATA_RATE_5300			5300
#define O4CV_DATA_RATE_6300			6300

// define for O4CV configuration structure version
#define O4CV_CONFIG_VERSION	0

#endif

