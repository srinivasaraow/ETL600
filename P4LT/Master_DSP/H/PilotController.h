                                                                             /*-------------------------------------------------------------------------*
 * $Workfile: PilotController.h $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by	: P.Plueer
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/PilotController.h $
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 2     18.01.05 17:12 Chpeplu
 * first running version
 * 
 * 1     18.01.05 14:54 Chpeplu
 * Initial Version
 *
 *-------------------------------------------------------------------------*/
 
#ifndef PILOT_CONTROLLER_H
#define PILOT_CONTROLLER_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void SEGMENT_EXT_CODE Init_PilotController(void);
void SEGMENT_EXT_CODE PilotController(volatile long * internalTDM_Rx_Ptr, volatile long * internalTDM_Tx_Ptr);

#endif

