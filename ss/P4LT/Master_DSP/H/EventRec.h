/*-------------------------------------------------------------------------*
 * $Workfile: EventRec.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/H/EventRec.h $
 * 
 * 5     28.06.05 13:53 Chchlee
 * ASSERTs consolidated
 * 
 * 4     13.06.05 17:23 Chchlee
 * 
 * 3     9.06.05 19:51 Chchlee
 * eventRec_blockRotationIsOngoing added
 * 
 * 2     16.02.05 10:35 Chchlee
 * Upgrade to Ptty2
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 6     4.01.05 15:58 Chchlee
 * eventRec_readRAMCounters now returns unsigned long *
 * 
 * 5     15.12.04 10:06 Chchlee
 * 
 * 4     10.12.04 8:58 Chchlee
 * 
 * 3     29.11.04 8:40 Leeb02
 * 
 * 2     26.11.04 18:19 Leeb02
 * 
 * 1     26.11.04 7:32 Leeb02

 *-------------------------------------------------------------------------*/
#ifndef EVENTREC_H
#define EVENTREC_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "event_defs.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/



/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/



/* PUBLIC FUNCTIONS ***********************************************************/

void eventRec_init(void);  

void eventRec_activate(void);                      // to activate with 1000Hz
void eventRec_activateBlockRotationHandler(void);  // to activate with 1000Hz
void eventRec_writeEventsToEprom(void);            // to activate with 5000Hz

void eventRec_recordEvent(unsigned long category, 
						  unsigned long data, 
						  BOOL          doCount);

BOOL eventRec_resetCounter(unsigned long counterNumber);

t_HMIEventData * eventRec_readEvent (long *p);

t_EventCounter * eventRec_readROMCounters(void);
unsigned long  * eventRec_readRAMCounters(void);

// can be used by one client only to 
// 1. disable event recorder
// 2. re-enable eventrecorder
void eventRec_enable(BOOL bEnable);

// for alarm monitoring
BOOL eventRec_isEnabled(void);
BOOL eventRec_blockRotationIsOngoing(void);

void eventRec_clearAndReset(void);

// may only be called before reset 
void eventRec_EmptyStack(void);

#endif


