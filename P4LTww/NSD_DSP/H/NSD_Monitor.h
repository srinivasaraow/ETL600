/*---------------------------------------------------------------------------*
 * $Workfile: NSD_Monitor.h $
 * Part of      : ETL600 / 
 * Language     : C
 * Created by   : P.Plüer
 * Remarks      :  
 * Purpose      : NSD600 Monitor Functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/NSD_DSP/H/NSD_Monitor.h $
 * 
 * 9     28.08.07 9:09 Chtognu
 * Init_Monitor_Trip_Guard() initialization for AES mode changed for
 * correct Trip and Guard monitoring. Additional "Detector active check"
 * added for AES in Monitor_Trip_Guard()
 * 
 * 7     18.09.06 16:57 Chhamaa
 * 
 * 3     30.06.05 8:30 Chpeplu
 * Code walkthrough Pl
 * 
 * 2     1.03.05 11:56 Chpeplu
 * paramter for function SNR_Monitoring_Guard() changed
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 9     17.01.05 15:55 Chpeplu
 * Additional parameter in function call added void SEGMENT_EXT_CODE
 * SNR_Monitoring_Guard(long Rx_Command, long Rx_Guard, float SNR_from_FE,
 * unsigned long *NSD_Alarm, long Channel);
 * 
 * 8     11.01.05 11:41 Chpeplu
 * New functions added Init_RxCommandDurationMonit() and
 * RxCommandDurationMonit() added
 * 
 * 7     23.12.04 15:21 Chpeplu
 * functions void SEGMENT_EXT_CODE
 * Init_SNR_Monitoring_Guard(t_NSD_DSP_Config *g_NSD_Config) and 
 * void SEGMENT_EXT_CODE SNR_Monitoring_Guard(long Rx_Command, long
 * Rx_Guard, unsigned long *NSD_Alarm, long Channel) added
 * 
 * 6     7.12.04 11:51 Chpeplu
 * New function Update_Dev_Status_Guard() added
 * 
 * 5     3.12.04 13:23 Chpeplu
 * NSD_Alarm in status added
 * 
 * 4     1.12.04 17:27 Chpeplu
 * return parameter for function Unblocking() added, defines and types
 * moved to NSD_Monitor.c
 * 
 * 3     29.11.04 17:39 Plueer01
 * local defines moved to C-File
 * 
 * 2     17.11.04 16:49 Plueer01
 * second version 
 * 
 * 1     17.11.04 16:38 Plueer01
 * Initial Version
 * 
 *---------------------------------------------------------------------------*/

#ifndef NSD_MONITOR_H
#define NSD_MONITOR_H

/* INCLUDE FILES *************************************************************/

#include "NSD_DSP_Types.h"


/* PUBLIC TYPE DECLARATIONS **************************************************/

/* PUBLIC MACRO DEFINITIONS **************************************************/

/* PUBLIC FUNCTIONS **********************************************************/

void Init_NSDUnblocking(t_NSD_DSP_Config *g_NSD_Config);
long NSDUnblocking(long Channel);
//
void Init_Monitor_Trip_Guard(t_NSD_DSP_Config *g_NSD_Config);
void Monitor_Trip_Guard(long *ptr_Rx_Command, BOOL Rx_Guard, unsigned long *ptr_NSD_Alarm, long Channel, BOOL detectorOutputActive);
//
void Update_Dev_Status_Guard(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD);
//
void Init_SNR_Monitoring_Guard(t_NSD_DSP_Config *g_NSD_Config);
void SNR_Monitoring_Guard(long Rx_Command, float SNR_from_FE, t_NSD_Status *NSD_Status, long Channel);

void Init_RxCommandDurationMonit(t_NSD_DSP_Config *g_NSD_Config);
long RxCommandDurationMonit(long RxCommandsFromReceiver, unsigned long *ptr_Alarm, long Channel);

t_NSD_Power_Levels * getPtr_NSD_Power_Speech_Guard(void);


#endif


