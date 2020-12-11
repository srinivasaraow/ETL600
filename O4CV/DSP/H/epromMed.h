/*-------------------------------------------------------------------------*
 * $Workfile	: epromMed.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Med level eprom access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/epromMed.h $
 * 
 * 1     07-04-17 10:38 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/ 
 
#ifndef _EPROMMED
#define	_EPROMMED
 
/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
                                                
#define VOIDCRC 	0xCACA 
 
#define SAVE_DATA			1 
#define CALC_AND_WRITE_CRC 	2
#define ERASE_SECTOR		3 

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void epromMed_pushCommand(UINT16 option, UINT16* addrFrom, UINT16* addrTo, UINT32 length);
bool epromMed_commandsInStack(void);
void epromMed_activate(void);  
void epromMed_emptyStack(void); 	
									

#endif





