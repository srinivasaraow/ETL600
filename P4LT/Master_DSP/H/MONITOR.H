/*-------------------------------------------------------------------------*
 * $Workfile: MONITOR.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*
 * $Log: /branches/Source O4CV/P4LT/Master_DSP/H/MONITOR.H $
 * 
 * 11    19.01.07 15:30 Chhamaa
 *  Load re-balancing
 * 
 * 9     18.09.06 16:30 Chhamaa
 * Function monitor_detectResetG4AI() added.
 * 
 * 5     22.07.05 8:15 Chchlee
 * monitor_enableReset renamed
 * 
 * 4     22.07.05 8:04 Chchlee
 * Comments added
 * 
 * 3     13.06.05 7:28 Chchlee
 * serve system out removed
 * 
 * 2     3.06.05 10:34 Chmibuh
 * monitor_Reset_enable() added
 * 
 * 1     19.01.05 15:50 Chchlee
 *-------------------------------------------------------------------------*/

#ifndef		_MONITOR
#define		_MONITOR         


/* INCLUDE FILES **************************************************************/
#include "P4LT_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void monitor_init(void); // constructor

void monitor_recordTxCmdEvents(void);						// activate in 5.33 kHz task
void monitor_recordRxCmdEvents(void);						// activate in 5.33 kHz task

void monitor_generateAlarmEvents(void);						// activate in 0.1067 kHz task 
void monitor_evaluateDataAlarmsAndWarnings(void);			// activate in 0.1067 kHz task 
void monitor_evaluatePlcIfAlarmsAndWarnings(void);			// activate in 0.1067 kHz task 
void monitor_evaluatePlcLinkAlarms(void);					// activate in 0.1067 kHz task 
void monitor_evaluatePlcHwAlarms(void);						// activate in 0.1067 kHz task 
void monitor_evaluateNsdAlarms(void);						// activate in 0.1067 kHz task 
void monitor_evaluateLifesigns(void);						// activate in 0.1067 kHz task 
void monitor_checkTDMBus(void);								// activate in 0.1067 kHz task 
void monitor_ifCRCerror_Reset(void);						// activate in 0.1067 kHz task
void monitor_detectResetG4AI(unsigned long basicTimeSlot);	// activate in 0.1067 kHz task

/* !!!Important!!!:
   These functions must be called after all other cyclically
   activated monitor functions, first "monitor_delayLowLevelAlarms", second
   "monitor_evaluateTopLevelAlarms"
   Reason: if an alarmDelay with "bSkipInitialPickupTime = TRUE" is created,
   the respective alarm function (e.g. monitor_evaluatePlcLinkAlarms) must 
   have been processed at least once. Else, alarmDelay will switch back to state AlarmIdle when
   activated the first time.
*/
void monitor_delayLowLevelAlarms (void);					// activate in 0.1067 kHz task 
void monitor_evaluateTopLevelAlarms (void);					// activate in 0.1067 kHz task 

void monitor_enableReset(BOOL isEnabled);

t_AlarmsAndWarnings * monitor_get_pAlarmsAndWarnings(void);
unsigned long       * monitor_get_pHighLevelAlarms(void);


#endif
