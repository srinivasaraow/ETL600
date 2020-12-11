/*-------------------------------------------------------------------------*
 * $Workfile	: dll2ndStation.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The dll2ndStation module implements a Data Link Layer Secondary
 *				  Station according to Nt61538c ETL500 Transmission protocols.doc.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/dll2ndStation.h $
 * 
 * 1     25.05.07 17:11 Chrobif
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef DLL_2ND_STATION_H
#define DLL_2ND_STATION_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
extern void dll2ndStation_init(void);
extern void dll2ndStation_activate(void);
extern void dll2ndStation_setupTxUDPtrs(UINT32 *pTxUD, UINT32 nTxUDWords);

#endif
