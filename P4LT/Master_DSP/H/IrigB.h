/*-------------------------------------------------------------------------*
 * $Workfile: IrigB.h $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/IrigB.h $
 * 
 * 3     17.02.05 16:41 Chchlee
 * Monitoring concept changed
 * 
 * 2     15.02.05 12:52 Chmibuh
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 3     30.12.04 10:14 Chchlee
 * Interface cleaned up
 * 
 * 2     27.10.04 14:36 Buhl01
 * Adapt to P4LT
 * 
 * 1     15.10.04 16:12 Buhl01
 * initial
 *-------------------------------------------------------------------------*/


#ifndef IRIGB_H
#define IRIGB_H

/* INCLUDE FILES **************************************************************/

#include "P4LT_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/



/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/



/* PUBLIC FUNCTIONS ***********************************************************/
void irigB_init(t_P4LT_Config *config);

void irigB_activate(void);      // to activated in 10.667 kHz task

void irigB_checkRTC_Date(void); // to activated in 0.1067 kHz task 

BOOL irigB_incorrectRTCDateAlarm(void);
BOOL irigB_signalLostAlarm(void);


#endif //IRIGB_H



