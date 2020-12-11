/*-------------------------------------------------------------------------*
 * $Workfile: AES_Transmitter.h $
 * Part of      : ETL600 / NSD DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/H/AES_Transmitter.h $
 * 
 * 2     20.09.06 12:00 Chhamaa
 * AES_Transmitter_localCommandLoop() added.
 * 
 * 1     7.08.06 12:49 Chchlee
 * 
 * 1     21.07.06 14:23 Chhamaa
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef AES_Transmitter_H
#define AES_Transmitter_H

/* INCLUDE FILES **************************************************************/

#include "NSD_DSP_Types.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/


/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/

void AES_Transmitter_init(t_NSD_DSP_Config *config);
BOOL AES_Transmitter_activate(float *txSignalI, float *txSignalQ, long command);

long AES_Transmitter_localCommandLoop(long cmd);

#endif
