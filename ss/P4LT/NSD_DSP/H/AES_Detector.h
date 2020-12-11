/*-------------------------------------------------------------------------*
 * $Workfile: AES_Detector.h $
 * Part of      : ETL600 / NSD DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/NSD_DSP/H/AES_Detector.h $
 * 
 * 3     28.08.07 9:08 Chtognu
 * AES: boolian added to AES_Detector_activate() for "detector active"
 * signalisation --> used for modified Trip and Guard Alarm. Trip and
 * Guard check removed.
 * 
 * 1     29.08.06 11:38 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef AES_DETECTOR_H
#define AES_DETECTOR_H


/* INCLUDE FILES **************************************************************/

#include "NSD_DSP_Types.h"


/* PUBLIC FUNCTIONS ***********************************************************/

void AES_Detector_init(t_NSD_DSP_Config *config);
unsigned long AES_Detector_activate(float *rxAfSignal, BOOL rxGuard, BOOL *detectorOutputActive);


#endif
