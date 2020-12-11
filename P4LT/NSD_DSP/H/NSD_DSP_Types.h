/*-------------------------------------------------------------------------*
 * $Workfile: NSD_DSP_Types.h $
 * Part of      : ETL600 / NSD DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/H/NSD_DSP_Types.h $
 * 
 * 36    29.11.06 9:45 Chpeplu
 * FW_VERSION_NSD 0x00000051 changed
 * 
 * 35    23.11.06 13:01 Chpeplu
 * new defines for command and command combinations added
 * 
 * 34    14.11.06 13:27 Chpeplu
 * New define RX_COM_T and TX_COM_T added, Define FW_VERSION_NSD changed
 * 0x043 auf 0x050 
 * 
 * 33    18.08.06 13:34 Chhamaa
 * COM_T modified
 * 
 * 32    18.08.06 10:31 Chhamaa
 * AES defines moved to AES600.h
 * 
 * 31    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 31    7.07.06 18:50 Chhamaa
 * 
 * 28    28.02.06 13:40 Chpeplu
 * define FW_VERSION_NSD	0x00000043 changed
 * 
 * 27    8.02.06 13:26 Chpeplu
 * define FW_VERSION_NSD changed to 0x042
 * 
 * 26    31.01.06 16:23 Chpeplu
 * FW Version changed, new define F_0 added
 * 
 * 24    2.11.05 17:12 Chpeplu
 * define FW_VERSION_NSD (0x00000021) changed
 * 
 * 23    23.09.05 13:25 Chpeplu
 * define FW_VERSION_NSD (0x00000020) changed
 * 
 * 22    21.09.05 16:51 Chpeplu
 * define FW_VERSION_NSD (0x0000001f) changed!
 * 
 * 21    20.09.05 17:16 Chpeplu
 *  define FW_VERSION_NSD (0x0000001e) changed!
 * 
 * 20    29.08.05 16:54 Chpeplu
 * define FW_VERSION_NSD	0x0000001d changed
 * 
 * 19    9.08.05 17:41 Chpeplu
 * define FW_VERSION_NSD	0x0000001c changed
 * 
 * 18    30.06.05 9:20 Chpeplu
 * define FW_VERSION_NSD	0x0000001b changed
 * 
 * 17    30.06.05 8:31 Chpeplu
 * Code walkthrough Pl
 * 
 * 16    22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 15    15.06.05 14:50 Chpeplu
 * define FW_VERSION_NSD	0x0000001a changed
 * 
 * 14    27.05.05 14:27 Chpeplu
 * define FW_VERSION_NSD	0x00000019 changed
 * 
 * 13    23.05.05 17:04 Chpeplu
 * define FW_VERSION_NSD	0x00000018 changed
 * 
 * 12    22.04.05 13:30 Chpeplu
 * define FW_VERSION_NSD	0x00000017 changed
 * 
 * 11    14.04.05 12:40 Chpeplu
 * FW Version changed FW_VERSION_NSD 
 * 
 * 10    4.04.05 9:41 Chpeplu
 * define FW_VERSION_NSD changed 
 * 
 * 9     24.03.05 13:40 Chpeplu
 * New defines TRIP_WEIGHT_DEFAULT, PILOT_WEIGHT_DEFAULT added
 * 
 * 8     18.03.05 9:38 Chpeplu
 * FW-Version NSD changed 
 * 
 * 7     17.03.05 17:28 Chpeplu
 * define NUMBER_OF_ADAPTIVE_DETECTION set to 3
 * 
 * 6     14.03.05 12:14 Chpeplu
 * defines TDM_NSD_CMD_MASK_CH1 and TDM_NSD_CMD_MASK_CH2 removed FW
 * Version NSD changed
 * 
 * 5     14.03.05 10:34 Chpeplu
 * define COM_T changed from 0x0010 to 0x0040
 * 
 * 4     8.03.05 8:52 Chpeplu
 * Defines added NUMBER_OF_RX_DET_TONES and COM_A...COM_ABCD
 * 
 * 3     8.03.05 7:19 Chpeplu
 * Defines NUMBER_OF_APPLICATION_MODES and F_1 ... F_7 added
 * 
 * 2     1.03.05 11:57 Chpeplu
 *  new define  ONE_MSEC_MAINCASE added,  FW Version NSD600 changed to
 * 0.18
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 11    13.01.05 15:48 Chpeplu
 * FW_VERSION_NSD increased to 0x00000011
 * 
 * 10    10.01.05 8:48 Chpeplu
 * struct t_NSD_Power_Levels_Type changed
 * 
 * 9     21.12.04 15:55 Chpeplu
 * struct t_NSD_DSP_Status moved to P4LT_Types.h and renamed to
 * t_IntTeleprotStatus
 * 
 * 8     15.12.04 17:30 Chpeplu
 * defines for ON and OFF added
 * 
 * 7     3.12.04 13:23 Chpeplu
 * struct t_NSD_DSP_Status added
 * 
 * 6     2.12.04 13:28 Chpeplu
 * struct t_NSD_Power_Levels_Type extended by GuardPowerMean
 * 
 * 5     1.12.04 17:26 Chpeplu
 * struct t_NSD_Power_Levels_Type added -> used for Unblocking function
 * 
 * 4     25.08.04 7:30 Leeb02
 * t_TDM_BusConfig added
 * 
 * 3     24.08.04 15:39 Plueer01
 *   defines Command mask and start added
 * 
 * 2     23.08.04 17:04 Plueer01
 * 
 * 1     5.03.04 15:35 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef NSD_DSP_TYPES_H
#define NSD_DSP_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"P4LT_Types.h"

/* GLOBAL CONSTANT DECLARATIONS ********************************************/


/* GLOBAL TYPE DECLARATIONS ************************************************/
//
#define		TDM_START_BIT_NSD		0x00000080
//
#define		NUMBER_OF_ADAPTIVE_DETECTION	3
// Firmware version controlling of NSD
#define		FW_VERSION_NSD			0x00000051
// common used defines
#define		OFF		FALSE
#define		ON		TRUE
//
#define		ONE_MSEC_MAINCASE		(AF_SAMPLE_FREQ/10000.0)
//
// Frequencies for code table
//
#define F_0								0
#define	F_1								1
#define	F_2								2
#define F_3								3
#define F_4								4
#define F_5								5
#define	F_6								6
#define F_7								7
//
// blocking, permissive, direct
#define	NUMBER_OF_APPLICATION_MODES		3
//
//
// Command defines		
#define	COM_A							0x0001
#define	COM_B							0x0002
#define	COM_C							0x0004
#define	COM_D							0x0008
#define COM_E							0x0010
#define COM_F							0x0020
#define	COM_T							0x0020
#define	COM_AB							(COM_A|COM_B)
#define	COM_AC							(COM_A|COM_C)
#define COM_BC							(COM_B|COM_C)
#define COM_ABC							(COM_A|COM_B|COM_C)
#define COM_AD							(COM_A|COM_D)
#define	COM_BD							(COM_B|COM_D)
#define COM_CD							(COM_C|COM_D)
#define COM_ABD							(COM_A|COM_B|COM_D)
#define COM_ACD							(COM_A|COM_C|COM_D)
#define COM_BCD							(COM_B|COM_C|COM_D)
#define COM_ABCD						(COM_A|COM_B|COM_C|COM_D)
#define COM_AE							(COM_A|COM_E)
#define COM_BE							(COM_B|COM_E)
#define COM_ABE							(COM_A|COM_B|COM_E)
#define COM_CE							(COM_C|COM_E)
#define COM_ACE							(COM_A|COM_C|COM_E)
#define COM_BCE							(COM_B|COM_C|COM_E)
#define COM_ABCE						(COM_A|COM_B|COM_C|COM_E)
#define	COM_DE							(COM_D|COM_E)
#define COM_ADE							(COM_A|COM_D|COM_E)
#define COM_BDE							(COM_B|COM_D|COM_E)
#define COM_ABDE						(COM_A|COM_B|COM_D|COM_E)
#define COM_CDE							(COM_C|COM_D|COM_E)
#define COM_ACDE						(COM_A|COM_C|COM_D|COM_E)
#define COM_BCDE						(COM_B|COM_C|COM_D|COM_E)
#define COM_ABCDE						(COM_A|COM_B|COM_C|COM_D|COM_E)
//
#define COM_AF							(COM_A|COM_F)
#define COM_BF							(COM_B|COM_F)
#define COM_ABF							(COM_A|COM_B|COM_F)
#define COM_CF							(COM_C|COM_F)
#define COM_ACF							(COM_A|COM_C|COM_F)
#define COM_BCF							(COM_B|COM_C|COM_F)
#define COM_ABCF						(COM_A|COM_B|COM_C|COM_F)
#define	COM_DF							(COM_D|COM_F)
#define COM_ADF							(COM_A|COM_D|COM_F)
#define COM_BDF							(COM_B|COM_D|COM_F)
#define COM_ABDF						(COM_A|COM_B|COM_D|COM_F)
#define COM_CDF							(COM_C|COM_D|COM_F)
#define COM_ACDF						(COM_A|COM_C|COM_D|COM_F)
#define COM_BCDF						(COM_B|COM_C|COM_D|COM_F)
#define COM_ABCDF						(COM_A|COM_B|COM_C|COM_D|COM_F)
//
#define COM_EF							(COM_E|COM_F)
#define COM_AEF							(COM_A|COM_E|COM_F)
#define COM_BEF							(COM_B|COM_E|COM_F)
#define COM_ABEF						(COM_A|COM_B|COM_E|COM_F)
#define COM_CEF							(COM_C|COM_E|COM_F)
#define COM_ACEF						(COM_A|COM_C|COM_E|COM_F)
#define COM_BCEF						(COM_B|COM_C|COM_E|COM_F)
#define COM_ABCEF						(COM_A|COM_B|COM_C|COM_E|COM_F)
#define	COM_DEF							(COM_D|COM_E|COM_F)
#define COM_ADEF						(COM_A|COM_D|COM_E|COM_F)
#define COM_BDEF						(COM_B|COM_D|COM_E|COM_F)
#define COM_ABDEF						(COM_A|COM_B|COM_D|COM_E|COM_F)
#define COM_CDEF						(COM_C|COM_D|COM_E|COM_F)
#define COM_ACDEF						(COM_A|COM_C|COM_D|COM_E|COM_F)
#define COM_BCDEF						(COM_B|COM_C|COM_D|COM_E|COM_F)
#define COM_ABCDEF						(COM_A|COM_B|COM_C|COM_D|COM_E|COM_F)
// @ Pl_ext
#define RX_COM_T						0x00010000
#define TX_COM_T						0x00000100

//
// number of detector tones to do the correlation 
#define	NUMBER_OF_RX_DET_TONES			4

#define	PILOT_WEIGHT_DEFAULT			0.5
// trip level 0dB
#define	TRIP_WEIGHT_DEFAULT				1.0
//
//
typedef struct {

	long				mode;					// constants INT_TELPROT_MODE_...
	t_NSD_Config		NSD[NUM_OF_NSD_INST];	// NSD600-specific configuration data
	t_AES_Config		AES;					// AES600-specific configuration data

	t_TDM_BusConfig		TDM_Config[NO_TDM_BASIC_TIMESLOTS];


} t_NSD_DSP_Config;
//
typedef struct {
	float	SpeechPowerCh1[NUMBER_OF_ADAPTIVE_DETECTION];
	float	SpeechPowerCh2[NUMBER_OF_ADAPTIVE_DETECTION];
	float	GuardPower[NUM_OF_NSD_INST+1];
	float	GuardPowerMean[NUM_OF_NSD_INST+1];
	float	RxGuardI[NUM_OF_NSD_INST+1];
	float	RxGuardQ[NUM_OF_NSD_INST+1];
} t_NSD_Power_Levels;
//


#endif
