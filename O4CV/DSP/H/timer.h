/*-------------------------------------------------------------------------*
 * $Workfile	: timer.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Time delays and periodic interrupts
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/timer.h $
 * 
 * 1     07-07-20 16:25 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef TIMER_H
#define TIMER_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"
#include "O4CV_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

typedef enum {

	e_Timer1,
	e_Timer2,
	e_Timer3,

} e_TimerId;

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void timer_init(UINT32 microSeconds, e_TimerId timerId);
void timer_wait(e_TimerId timerId);
void timer_initInterruptTimer(UINT32 microSeconds);
void timer_startInterruptTimer(void);
void timer_stopInterruptTimer(void);
bool timer_getInterruptFlag(void);

#endif

