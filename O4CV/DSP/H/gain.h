/*-------------------------------------------------------------------------*
 * $Workfile	: gain.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Gain of the telephony signals
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/gain.h $
 * 
 * 1     07-07-18 15:21 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef GAIN_H
#define GAIN_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"
#include "O4CV_Types.h"
#include "sport2.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define gain_Rx		0
#define gain_Tx 	1

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void gain_init(t_O4CVConfig* pConfig, t_4w_Gains* fw_gains);
void gain_apply(INT16* buffer, UINT16 channel, UINT16 rxTx);

#endif

