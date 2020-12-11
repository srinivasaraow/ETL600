/*-------------------------------------------------------------------------*
 * $Workfile: NSD_Output_At_RxAlarm.h $
 * Part of	: NSD570
 * Language	: C
 * Created by	: P. Plueer
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/NSD_DSP/H/NSD_Output_At_RxAlarm.h $
 * 
 * 4     20.09.05 17:15 Chpeplu
 * New function name Freeze_CommandOutput() added
 * 
 * 3     30.06.05 8:30 Chpeplu
 * Code walkthrough Pl
 * 
 * 2     22.06.05 10:38 Chchlee
 * Code walkthrough
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 4     21.12.04 15:53 Chpeplu
 * struct t_NSD_DSP_Status changed to t_IntTeleprotStatus
 * 
 * 3     16.12.04 10:35 Chpeplu
 * New function Save_StateWithoutRxAlarm() added
 * 
 * 2     15.12.04 17:32 Chpeplu
 * Second Version runs
 * 
 * 1     15.12.04 11:03 Chpeplu
 * Inital Version
 * 
 *-------------------------------------------------------------------------*/

#ifndef Output_At_Alarm_
#define Output_Al_Alarm_

void init_Output_At_RxAlarm(t_NSD_DSP_Config *g_NSD_Config, t_IntTeleprotStatus	*g_NSD_DSP_Status);
void Set_Output_At_RxAlarm(unsigned long *Rx_Alarm_OR_Mask, unsigned long*Rx_Alarm_AND_Mask, long Channel);
void Delay_Hold_RxAlarm(long Channel);

void Freeze_CommandOutput(long RxCommand, BOOL GuardState, unsigned long *Rx_Alarm_OR_Mask, unsigned long*Rx_Alarm_AND_Mask, long Channel);

#endif     
