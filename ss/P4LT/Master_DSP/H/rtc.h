/*-------------------------------------------------------------------------*
 * $Workfile: rtc.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/H/rtc.h $
 * 
 * 3     10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 2     24.02.05 11:44 Chmibuh
 * g_IncorrectRTCDateAlarm added
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 6     10.12.04 12:38 Chchlee
 * rtc_getIntTimeBCDPtr enabled
 * 
 * 5     20.10.04 16:30 Buhl01
 * 
 * 4     18.10.04 17:47 Buhl01
 * 
 * 3     21.09.04 18:17 Buhl01
 * day_of_week added
 * 
 * 2     14.09.04 16:27 Buhl01
 * 
 * 1     7.09.04 13:01 Buhl01
 * initial
 *-------------------------------------------------------------------------*/

#ifndef RTC_H
#define RTC_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "P4LT_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define TOGGLE_MINUTE		0x00000001
#define TOGGLE_HOUR			0x00000002
#define SET_RTC_MAGIC_WORD	0x00000004

#define RTC_NoOF_BYTES_READ_TIME_DATE		8
#define RTC_NoOF_BYTES_WRITE_TIME_DATE		8
#define RTC_NoOF_BYTES_SET_MAGIC_WORDS		4

/* PUBLIC TYPE DECLARATIONS ***************************************************/
typedef struct{ 
	unsigned long milisecond;
	unsigned long second;   			//enter the current time, date, month, and year
	unsigned long minute;
	unsigned long hour; 
	unsigned long day_of_week;			//ignored but needed for rtc and allows the use of pointer++
	unsigned long date;       
	unsigned long month;
	unsigned long year; 
	unsigned long togglebits;
}t_internalTime;



/* PUBLIC MACRO DEFINITIONS ***************************************************/



/* PUBLIC FUNCTIONS ***********************************************************/
void initInternalTime(void);
void convert_time_formats(void);

t_internalTime * rtc_getIntTimePtr(void);
t_internalTime * rtc_getIntTimeBCDPtr(void);
t_TimeDate     * rtc_getTimeDatePtr(void);
void             setTimeDateOnRTC(void);

void setMagicWordsOnRTC(void);

BOOL rtc_incorrectRTCDateAlarm(void);

#endif //#ifndef RTC_H
