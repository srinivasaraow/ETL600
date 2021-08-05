/*----------------------------------------------------------------------------*
 * $Workfile: FE_DSP_Alarm.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT/FE_DSP/H/FE_DSP_Alarm.h $
 * 
 * 3     11.07.05 19:11 Chmibuh
 * FE_DSP_Alarm_rxRfOverflow() added
 * 
 * 2     18.02.05 14:59 Chhamaa
 * Parameter opMode added to FE_DSP_Alarm_updateAplcAlarms()
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 2     12.01.05 18:39 Chhamaa
 * 
 * 1     29.11.04 11:51 Maag01
 * Initial version
 *
 *----------------------------------------------------------------------------*/
 
#ifndef FE_DSP_ALARM_H
#define FE_DSP_ALARM_H


/* INCLUDE FILES **************************************************************/

#include "tdm_int_types.h"
#include "APLC_Types.h"
#include "FE_DSP_Types.h"
#include "FE_DSP_Alarm.h"


/* PUBLIC FUNCTIONS ***********************************************************/

void FE_DSP_Alarm_updateAplcAlarms(t_FE_Alarm *feAlarm, t_FrontEndDSP_Status *feStatus, 
								   unsigned long opMode, long nAplcCh);

BOOL SEGMENT_EXT_CODE FE_DSP_Alarm_rxRfOverflow(void);



#endif
