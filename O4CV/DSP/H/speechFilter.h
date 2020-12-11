/*-------------------------------------------------------------------------*
 * $Workfile	: speechFilter.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Speech filter for the telephony signals
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/speechFilter.h $
 * 
 * 1     07-07-18 16:17 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef SPEECHFILTER_H
#define SPEECHFILTER_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"
#include "O4CV_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define speechFilter_Rx		0
#define speechFilter_Tx 	1

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void speechFilter_init(t_O4CVConfig* pConfig);
void speechFilter_apply(INT16* bufferIn, INT16* bufferOut, UINT16 channel, INT16 rxTx);

#endif

