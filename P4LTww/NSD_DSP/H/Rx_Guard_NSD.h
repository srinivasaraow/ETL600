/*----------------------------------------------------------------------------*
 * $Workfile: Rx_Guard_NSD.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : P. Plüer
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2004
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/H/Rx_Guard_NSD.h $
 * 
 * 11    12.09.06 14:34 Chhamaa
 * Function tangWindow_activate() added.
 * 
 * 10    8.09.06 16:09 Chhamaa
 * rxGuardSignaling_activate() added.
 * 
 * 9     15.08.06 9:26 Chhamaa
 * AES persistent command detector added
 * 
 * 5     20.09.05 17:15 Chpeplu
 * Additional parameter for function rx_guard_nsd() (NSD-Alarm) added
 * 
 * 4     22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 3     14.04.05 12:39 Chpeplu
 * Some comment added
 * 
 * 2     24.03.05 13:43 Chpeplu
 * Some defines removed and changed for variable guard level
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 6     3.12.04 13:22 Chpeplu
 * NSD Status data added
 * 
 * 5     2.12.04 13:29 Chpeplu
 * function long SEGMENT_EXT_CODE RxGuardLevelMonitoring(long Channel);
 * added
 * 
 * 4     1.12.04 17:29 Chpeplu
 * some defines moved to Rx_Guard_NSD.c and includes changed
 * 
 * 3     8.09.04 14:20 Plueer01
 * void init_rx_guard_nsd(t_NSD_DSP_Config *Config_NSD, long Channel); ->
 * config struct changed
 * 
 * 2     19.07.04 10:58 Plueer01
 * functions now usable for 2 channels
 * 
 * 1     21.06.04 9:21 Plueer01
 * inital version
 *----------------------------------------------------------------------------*/
 
#ifndef RX_GUARD_NSD_H
#define RX_GUARD_NSD_H


/* INCLUDE FILES **************************************************************/

#include "NSD_Types.h"
#include "NSD_DSP_Types.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/**************************************************************************/
/* DEFINES für  RX_GUARD_NSD.C ********************************************/
/**************************************************************************/
 // Bestimmt den Multiplikatior des Inputsignales
 // dieser Wert bestimmt die Geschwindigkeit des Guardmittelwertes
 // (MEAN_MAX-MEAN_MIN)/Aufruffrequenz = Zeit vom oberen bis unteren Rand
 #define GUARD_LEVEL 60000.0

 // Erwarteter Eingangspegel -6dB = 
 //#define INPUT_LEVEL (0.707106781*0.25) 

 #define HALF_SQRT_OF_TWO		0.707106781	
 // @Pl@ muss der Wert GUARD_LEVEL_SQRT evtl. noch angepast werden bei variablem Guard Pegel? -> Nein
 // Pegel = Inputlevel * Inputlevel *1/4
 // Guard_Level_SQRT = SQRT(GUARD_LEVEL * HALF_SQRT_OF_TWO * 4)
 #define GUARD_LEVEL_SQRT		411.0
 // Der Pegel soll +/- 3dB um den Nominalwert liegen
// #define MEAN_MIN				INPUT_LEVEL * GUARD_LEVEL * 0.5
// #define MEAN_MAX				INPUT_LEVEL * GUARD_LEVEL *	2

 // Nominal guard level
//#define NOMINAL_GUARD_LEVEL		INPUT_LEVEL * GUARD_LEVEL
//
/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/

void init_rx_guard_nsd(t_NSD_DSP_Config *Config_NSD);
BOOL rx_guard_nsd(float *RxSample, long Channel, long ActivRxCommand, unsigned long *ptr_NSD_Alarm);
void RxGuardLevelMonitoring(t_NSD_Status *ptr_NSD_Status, long Channel);
void RxGuardIFAmplCorr();

long AES_persCmd_detect(long rxCmdActive);

BOOL rxGuardSignaling_activate(BOOL input, long channel);
BOOL tangWindow_activate(BOOL rxGuardOn, long rxCmd);

#endif
