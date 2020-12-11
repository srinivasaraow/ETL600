/*-------------------------------------------------------------------------*
 * $Workfile	: epromHigh.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : High level eprom access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/epromHigh.h $
 * 
 * 3     07-09-12 14:50 Chalnot
 * The functions epromHigh_eraseEFW() and epromHigh_programEFWData() added
 * for Em Fw Upload.
 * 
 * 2     07-08-30 13:17 Chalnot
 * Emergency Fw update function added.
 * 
 * 1     07-04-17 10:38 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/  
 
#ifndef _EPROMHIGH
#define _EPROMHIGH
 
/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void epromHigh_init(void);
bool epromHigh_readConfiguration(UINT16* pO4CVConfig);
void epromHigh_storeConfiguration(UINT16* pO4CVConfig);
bool epromHigh_programData(UINT32 length, UINT32 offset, UINT16* pData);
bool epromHigh_programEFWData(UINT32 length, UINT32 offset, UINT16* pData);
void epromHigh_eraseFirmware(void);
void epromHigh_eraseEFW(void);


#endif // _EPROMHIGH  





