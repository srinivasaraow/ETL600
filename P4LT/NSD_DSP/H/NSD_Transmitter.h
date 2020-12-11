/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSD_DSP/H/NSD_Transmitter.h $
 * 
 * 18    12-10-09 16:02 Chhamaa
 * HSD 2.5 kHz frequencies added.
 * 
 * 14    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 14    20.07.06 17:18 Chhamaa
 * Return value for function Transmitter() added.
 * 
 * 11    31.01.06 16:22 Chpeplu
 * New centre frequency for HSD and new shift frequency in HF for HSD set,
 * defines for frequencies set to fix integration time only
 * 
 * 9     22.04.05 13:30 Chpeplu
 * #define	NSD_CENTRE_FREQ 1083 changed
 * 
 * 8     14.04.05 12:39 Chpeplu
 * define NSD_CENTRE_FREQ		1340 changed
 * 
 * 7     5.04.05 11:13 Chpeplu
 * new frequency for NSD set NSD_CENTRE_FREQ = 1083Hz (old 1079Hz)
 * 
 * 6     4.04.05 9:42 Chpeplu
 * define NSD_CENTRE_FREQ changed from 1150Hz to 1079Hz
 * 
 * 5     24.03.05 13:42 Chpeplu
 * Some Defines removed and struct define removed 
 * 
 * 4     14.03.05 10:35 Chpeplu
 * Function Init_NSD_Transmitter() moved to extern and some changes for
 * custom table initialisation made 
 * 
 * 3     10.03.05 17:51 Chpeplu
 * Array TransmitSin[] and TransmitCos[] increased
 * 
 * 2     8.03.05 8:50 Chpeplu
 * defines COM_A ... COM_ABCD moved to NSD_DSP_Types.h
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 12    28.09.04 16:15 Plueer01
 * redefinition of COM_T removed
 * 
 * 11    28.09.04 15:36 Plueer01
 * SCALE_PILOT and SCALE_TRIP define changed
 * 
 * 10    8.09.04 14:21 Plueer01
 * void Init_NSD_Transmitter(t_NSD_DSP_Config *Config); -> config struct
 * changed
 * 
 * 9     20.08.04 16:30 Plueer01
 * new defines NSD_FREQ_1N- NSD_FREQ_7N added for transmitter wihout shift
 * for SSB
 * 
 * 8     20.07.04 16:14 Plueer01
 * Frequency shift for NSD commands and guard over SSB loop corrected
 * 
 * 7     20.07.04 13:20 Plueer01
 * Frequency defines for loop over SSB added
 * 
 * 6     8.07.04 17:10 Plueer01
 * NSD_Transmitter.c functions for two channel usable
 * 
 * 5     28.05.04 16:45 Plueer01
 * Some defines changed
 * 
 * 4     27.05.04 13:31 Plueer01
 * declaration of functions added
 * 
 * 3     27.05.04 9:11 Plueer01
 * 
 * 2     25.05.04 16:08 Plueer01
 * Initial version of NSD_Transmitter.h
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef NSD_TRANSMITTER_H
#define NSD_TRANSMITTER_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"NSD_Types.h"
#include	"NSD_DSP_Types.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/


/* GLOBAL TYPE DECLARATIONS ************************************************/

//
// NSD600 frequencies
//
// Integration length N1 sample for fs=10.667kHz
#define	NSD_INTEGR_TIME_TX	(2*INTEGR_TIME_DETECTOR)
// Shift frequnecy depending of the Integration time
#define SHIFT_FREQ_1		((1.0*AF_SAMPLE_FREQ)/NSD_INTEGR_TIME_TX)
//
#define TX_FREQ_SHIFT_IN_HF	2150
//NSD-centre frequency
#define	NSD_CENTRE_FREQ		1083
//
// HSD600 frequencies 
//
// Integration length N1 sample for fs=10.667kHz
#define	HSD_INTEGR_TIME_TX	(2*INTEGR_TIME_DETECTOR_HSD)
// Shift frequnecy depending of the Integration time
#define SHIFT_FREQ_1HSD		((1.0*AF_SAMPLE_FREQ)/HSD_INTEGR_TIME_TX)
//
#define TX_FREQ_SHIFT_IN_HF_HSD 1150
//
// HSD centre frequency for receiver
//#define	HSD_CENTRE_FREQ		841
//#define	HSD_CENTRE_FREQ		1083
//#define HSD_CENTRE_FREQ		950
//#define HSD_CENTRE_FREQ		886
#define	HSD_CENTRE_FREQ			950

//
// HSD600 2.5 kHz frequencies
//		
// Shift frequnecy depending of the Integration time
#define HSD_25_SHIFT_FREQ_1			SHIFT_FREQ_1		// same as for NSD600
//
#define HSD_25_TX_FREQ_SHIFT_IN_HF	1400				// same as for SSB 2.5 kHz
// Centre frequency
#define	HSD_25_CENTRE_FREQ			NSD_CENTRE_FREQ		// same as for NSD600	


//
// Frequency for Loop NSD Signal without SSB 
//
//                                                    	
#define	NSD_FREQ_1N	(NSD_CENTRE_FREQ - 3*SHIFT_FREQ_1)
#define	NSD_FREQ_2N	(NSD_CENTRE_FREQ - 2*SHIFT_FREQ_1)
#define	NSD_FREQ_3N	(NSD_CENTRE_FREQ - 1*SHIFT_FREQ_1)
#define	NSD_FREQ_4N	NSD_CENTRE_FREQ
#define	NSD_FREQ_5N	(NSD_CENTRE_FREQ + 1*SHIFT_FREQ_1)
#define	NSD_FREQ_6N	(NSD_CENTRE_FREQ + 2*SHIFT_FREQ_1)
#define	NSD_FREQ_7N	(NSD_CENTRE_FREQ + 3*SHIFT_FREQ_1)
//
// Frequency for Loop HSD Signal without SSB 
//
//                                                    	
#define	HSD_FREQ_1N	(HSD_CENTRE_FREQ - 2*SHIFT_FREQ_1HSD)
#define	HSD_FREQ_2N	(HSD_CENTRE_FREQ - 1*SHIFT_FREQ_1HSD)
#define	HSD_FREQ_3N	(HSD_CENTRE_FREQ - 0*SHIFT_FREQ_1HSD)
#define	HSD_FREQ_4N	(HSD_CENTRE_FREQ + 1*SHIFT_FREQ_1HSD)
#define	HSD_FREQ_5N	(HSD_CENTRE_FREQ + 2*SHIFT_FREQ_1HSD)
#define	HSD_FREQ_6N	0.0
#define	HSD_FREQ_7N	0.0

//Frequency for NSD over SSB
//
#define	NSD_FREQ_1	((NSD_CENTRE_FREQ - 3*SHIFT_FREQ_1)- TX_FREQ_SHIFT_IN_HF)
#define	NSD_FREQ_2	((NSD_CENTRE_FREQ - 2*SHIFT_FREQ_1)- TX_FREQ_SHIFT_IN_HF)
#define	NSD_FREQ_3	((NSD_CENTRE_FREQ - 1*SHIFT_FREQ_1) - TX_FREQ_SHIFT_IN_HF)
#define	NSD_FREQ_4	(NSD_CENTRE_FREQ - TX_FREQ_SHIFT_IN_HF)
#define	NSD_FREQ_5	((NSD_CENTRE_FREQ + 1*SHIFT_FREQ_1) - TX_FREQ_SHIFT_IN_HF)
#define	NSD_FREQ_6	((NSD_CENTRE_FREQ + 2*SHIFT_FREQ_1) - TX_FREQ_SHIFT_IN_HF)
#define	NSD_FREQ_7	((NSD_CENTRE_FREQ + 3*SHIFT_FREQ_1) - TX_FREQ_SHIFT_IN_HF)
//
// HSD frequencies over SSB 4kHz
//
#define	HSD_FREQ_1	((HSD_CENTRE_FREQ - 2*SHIFT_FREQ_1HSD)- TX_FREQ_SHIFT_IN_HF)
#define	HSD_FREQ_2	((HSD_CENTRE_FREQ - 1*SHIFT_FREQ_1HSD)- TX_FREQ_SHIFT_IN_HF)
#define	HSD_FREQ_3	((HSD_CENTRE_FREQ - 0*SHIFT_FREQ_1HSD) - TX_FREQ_SHIFT_IN_HF)
#define	HSD_FREQ_4	((HSD_CENTRE_FREQ + 1*SHIFT_FREQ_1HSD) - TX_FREQ_SHIFT_IN_HF)
#define	HSD_FREQ_5	((HSD_CENTRE_FREQ + 2*SHIFT_FREQ_1HSD) - TX_FREQ_SHIFT_IN_HF)
#define	HSD_FREQ_6	0.0
#define	HSD_FREQ_7	0.0
//
// HSD frequencies over SSB 2kHz
//
#define	HSD_FREQ_12	((HSD_CENTRE_FREQ - 2*SHIFT_FREQ_1HSD)- TX_FREQ_SHIFT_IN_HF_HSD)
#define	HSD_FREQ_22	((HSD_CENTRE_FREQ - 1*SHIFT_FREQ_1HSD)- TX_FREQ_SHIFT_IN_HF_HSD)
#define	HSD_FREQ_32	((HSD_CENTRE_FREQ - 0*SHIFT_FREQ_1HSD) - TX_FREQ_SHIFT_IN_HF_HSD)
#define	HSD_FREQ_42	((HSD_CENTRE_FREQ + 1*SHIFT_FREQ_1HSD) - TX_FREQ_SHIFT_IN_HF_HSD)
#define	HSD_FREQ_52	((HSD_CENTRE_FREQ + 2*SHIFT_FREQ_1HSD) - TX_FREQ_SHIFT_IN_HF_HSD)
#define	HSD_FREQ_62	0.0
#define	HSD_FREQ_72	0.0
//
// HSD 2.5 kHz frequencies over SSB 2.5 kHz
//
#define	HSD_25_FREQ_1	((HSD_25_CENTRE_FREQ - 3*HSD_25_SHIFT_FREQ_1) - HSD_25_TX_FREQ_SHIFT_IN_HF)
#define	HSD_25_FREQ_2	((HSD_25_CENTRE_FREQ - 2*HSD_25_SHIFT_FREQ_1) - HSD_25_TX_FREQ_SHIFT_IN_HF)
#define	HSD_25_FREQ_3	((HSD_25_CENTRE_FREQ - 1*HSD_25_SHIFT_FREQ_1) - HSD_25_TX_FREQ_SHIFT_IN_HF)
#define	HSD_25_FREQ_4	 (HSD_25_CENTRE_FREQ - HSD_25_TX_FREQ_SHIFT_IN_HF)
#define	HSD_25_FREQ_5	((HSD_25_CENTRE_FREQ + 1*HSD_25_SHIFT_FREQ_1) - HSD_25_TX_FREQ_SHIFT_IN_HF)
#define	HSD_25_FREQ_6	((HSD_25_CENTRE_FREQ + 2*HSD_25_SHIFT_FREQ_1) - HSD_25_TX_FREQ_SHIFT_IN_HF)
#define	HSD_25_FREQ_7	((HSD_25_CENTRE_FREQ + 3*HSD_25_SHIFT_FREQ_1) - HSD_25_TX_FREQ_SHIFT_IN_HF)

//
// max command combinations 
//#define	MAX_COM_COMBINATION		0x00000010
//
// fs for tx = 10.6667kHz and mainloop fs= 5.333kHz
#define NUMBER_OF_TX_SAMPLES_PROCESSED_PER_CYCLE	2
//
// Public functions for NSD **************************
BOOL Transmitter(float *I_Signal, float *Q_Signal, long Command_Input, long Channel);
void SEGMENT_EXT_CODE Init_NSD_Transmitter(t_NSD_DSP_Config *Config);
volatile void AmpCorr(float *ptrSin_00, float *ptrCos_00, int Number);
volatile void AmpCorrTx(int Channel);

	
#endif
