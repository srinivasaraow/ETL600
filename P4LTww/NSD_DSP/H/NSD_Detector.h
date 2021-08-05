/*-------------------------------------------------------------------------*
 * $Workfile: NSD_Detector.h $
 * Part of	:	NSD570
 * Language	:	C
 * Created by:	P.Plueer 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/NSD_DSP/H/NSD_Detector.h $
 * 
 * 10    08-02-25 10:17 Chtognu
 * Merged changes from ad.equ.branch:
 * NSD trip and guard alarm boolian added for RUENE (set flag whene enough
 * power in largest/second detector comparing to wide band power used
 * additionaly for the alarm "trip and guard")
 * 
 * 8     8.11.06 14:33 Chtognu
 * Bug 392 fixed, new input value for CompensationOfEvaluationTime() to
 * reset prolongation after the guard is detected. Changes active for NSD
 * TPE (2 and 4KHz).
 * 
 * 4     30.06.05 8:32 Chpeplu
 * Code walkthrough Pl
 * 
 * 3     22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 2     8.03.05 8:51 Chpeplu
 * Define NUMBER_OF_RX_DET_TONES moved to NSD_DSP_Types.h
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 12    12.01.05 17:18 Chpeplu
 * Functions Init_CompensationOfEvaluationTime() and
 * CompensationOfEvaluationTime() added
 * 
 * 11    7.12.04 11:52 Chpeplu
 * New function Update_Dev_Status_Det() added
 * 
 * 10    2.12.04 8:05 Chpeplu
 * Defines and struct definition moved to NSD_Detector.c
 * 
 * 9     8.09.04 14:22 Plueer01
 * void init_Detector(t_NSD_DSP_Config *g_NSD_Config); -> config struct
 * changed
 * 
 * 8     22.07.04 14:55 Plueer01
 * struct "type_NSD_Detector" added
 * 
 * 7     4.06.04 16:55 Plueer01
 * Some defines added for NSD600 frequency table
 * 
 * 6     1.06.04 16:37 Plueer01
 * define NUMBER_OF_PATH_CORRELATOR added
 * 
 * 5     28.05.04 16:45 Plueer01
 * Some defines added
 * 
 * 4     28.05.04 10:43 Plueer01
 * Some defines changed
 * 
 * 3     27.05.04 14:41 Plueer01
 * make it compilable
 * 
 * 2     27.05.04 13:58 Plueer01
 * 
 * 1     27.05.04 13:49 Plueer01
 * Inital version
 * 
 *-------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////////
// NSD570 defines
#ifndef NSD_DETECTOR_H
#define NSD_DETECTOR_H
//
/* INCLUDE FILES ***********************************************************/
#include	"NSD_DSP_Types.h"
#include	"NSD_Types.h"
//
//
#define	NUMBER_OF_RX_SAMPLE				2

//
// *** public functions for NSD detector ***************
//
void init_Detector(t_NSD_DSP_Config *g_NSD_Config);
long Detector_NSD(float *Rx_LP_Input, long Channel, BOOL *detectorOutputActive); // change_gn NSD trip and guard alarm boolian added
void Update_Dev_Status_Det(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD);

void Init_CompensationOfEvaluationTime(t_NSD_DSP_Config *g_NSD_Config);
long CompensationOfEvaluationTime(long RxCommand, long Channel, BOOL StopCompensation);

void Get_Ptr_Det_Amp_Correction(float **ptr_Sin_Ch1, float **ptr_Cos_Ch1, float **ptr_Sin_Ch2, float **ptr_Cos_Ch2);
void Get_Ptr_RXIF_Amp_Correction(float **ptr_Rx_Sin_Ch1, float **ptr_Rx_Cos_Ch1, float **ptr_Rx_Sin_Ch2, float **ptr_Rx_Cos_Ch2);
#endif
