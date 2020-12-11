/*----------------------------------------------------------------------------*
 * $Workfile: tdmUpConvSettings.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/tdmUpConvSettings.h $
 * 
 * 1     16.02.05 14:40 Chhamaa
 * 
 * 1     11.02.05 18:37 Chhamaa
 * Initial version
 *
 *----------------------------------------------------------------------------*/
 
#ifndef TDM_UP_CONV_SETTINGS_H
#define TDM_UP_CONV_SETTINGS_H


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "tdm_int_types.h"


/* PUBLIC FUNCTIONS ***********************************************************/

void tdmUpConvSettings_initSender(long frequency, long channelMode);
void tdmUpConvSettings_initReceiver(long frequency, long channelMode);
void tdmUpConvSettings_send(t_UpConvSettings *pTdmUpConvSettingsTx);
BOOL tdmUpConvSettings_receive(t_UpConvSettings *pTdmUpConvSettingsRx);
t_UpConvSettings *tdmUpConvSettings_getNewSettingsPtr(void);


#endif
