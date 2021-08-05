/*-------------------------------------------------------------------------*
 * $Workfile: Configuration.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/Configuration.H $
 * 
 * 3     21.02.05 19:41 Chchlee
 * temporary configuration warning cleaned up
 * 
 * 2     16.02.05 13:44 Chhamaa
 * configuration_getPEqualizerStatus(void) added
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 6     15.11.04 7:54 Leeb02
 * Status getter removed
 * 
 * 5     10.08.04 16:30 Leeb02
 * 
 * 4     25.07.04 11:13 Leeb02
 * 
 * 3     23.07.04 8:07 Leeb02
 * 
 *-------------------------------------------------------------------------*/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* INCLUDE FILES **************************************************************/

#include "assert.h"
#include "P4LT_Types.h"
#include "epromHi.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/



/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/




/* PUBLIC FUNCTIONS ***********************************************************/




// must be called before any configuration data structures are touched!
void configuration_init(void);

t_P4LT_Config * configuration_getPCurrentConfig(void);
t_P4LT_Config * configuration_getPNewConfig(void);
t_EpromLocalConfiguration * configuration_getPEpromLocalConfiguration(void);
t_EqualizerStatus * configuration_getPEqualizerStatus(void);

BOOL configuration_isTemporary(void);
void configuration_setConfigurationToPermanent(void);

#endif
