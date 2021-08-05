/*-------------------------------------------------------------------------*
 * $Workfile: boost_manager.h $
 * Part of      : ETL600
 * Language     : C, obviously
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/H/boost_manager.h $
 * 
 * 3     14.09.05 8:18 Chchlee
 * Segment prefix for ADSP compiler removed
 * 
 * 2     6.07.05 14:03 Chchlee
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 5     6.09.04 9:06 Leeb02
 * New boost manager concept
 * 
 * 4     30.08.04 7:58 Leeb02
 * boostManager_activate: parameters removed
 * 
 * 3     23.08.04 15:43 Leeb02
 * 
 *-------------------------------------------------------------------------*/

#ifndef BOOST_MANAGER_H
#define BOOST_MANAGER_H

/* INCLUDE FILES **************************************************************/
#include "tdm_ext_types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/


/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/

void boostManager_init(t_TDM_HardTimingTelegramInformation * from_TDM,
				   	   t_TDM_HardTimingTelegramInformation * to_TDM);

void boostManager_activate(volatile long * internalTDM_Rx_Ptr,
						   volatile long * internalTDM_Tx_Ptr);

#endif
