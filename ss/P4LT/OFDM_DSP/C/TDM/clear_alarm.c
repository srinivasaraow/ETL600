/*-------------------------------------------------------------------------*
 * $Workfile: clear_alarm.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/clear_alarm.c $
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 1     28.09.04 18:02 Maag01
 * Initial version
*/

void segment ( "seg_ext_pmco" ) Clear_Alarm()
{
   asm(".extern _Ofdm_DSP_ALARM;");
   asm("dm(_Ofdm_DSP_ALARM) = m5;");
}
