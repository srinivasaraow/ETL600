/*-------------------------------------------------------------------------*
 * $Workfile: EPROMMED.H $
 * Part of	: ETL600
 * Language	: C
 * Created by	:  CLEMENS BUERGI 08.12.1997
 * Remarks	:      
 * Purpose	:      medium level eprom access
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/EPROMMED.H $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 4     16.08.04 7:51 Leeb02
 * Cleaned up
 * 
 * 3     30.07.04 15:01 Leeb02
 * 
 * 2     30.07.04 11:09 Leeb02
 * 
 * 1     21.07.04 12:19 Leeb02
 *-------------------------------------------------------------------------*/

#ifndef _EPROMMED
#define	_EPROMMED
 
/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
                                                
#define VOIDCRC 	0xCACA 
 

#define READ_DATA			1
#define SAVE_DATA			2 
#define CALC_AND_WRITE_CRC 	3
#define ERASE_BLOCK			4 


#define LENGHT_IN_BYTES  0xF0000000

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

// CAUTION: Data referenced to by "AddrFrom" and "AddrTo" must be STATIC 
//          because data is deferedly processed
void eprom_pushCommand(long option, long* AddrFrom, long* AddrTo, long Length);

BOOL eprom_commandsInStack(void);

void eprom_activate(void);  

void eprom_emptyStack(void); // must not be called during normal tasking operation!!!

//long GetEpromStatus(void);									

	
#endif




