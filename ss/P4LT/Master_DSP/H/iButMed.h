/*-------------------------------------------------------------------------*
 * $Workfile: iButMed.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/iButMed.h $
 * 
 * 2     23.02.05 14:29 Chmibuh
 * unused functions removed
 * 
 * 1     23.02.05 14:02 Chmibuh
 *-------------------------------------------------------------------------*/

#ifndef IBUTMED_H
#define IBUTMED_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/


/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/
BOOL SEGMENT_EXT_CODE iButM_ReadScratchpad(unsigned long* address, unsigned long* es, unsigned long* data);
BOOL SEGMENT_EXT_CODE iButM_WriteScratchpad(unsigned long address, unsigned long *data);
BOOL SEGMENT_EXT_CODE iButM_ReadAuthPage(unsigned long pagenum, unsigned long* data, unsigned long* sign);
void SEGMENT_EXT_CODE iButM_ComputeSHA(unsigned long* MT, long* hash);
BOOL SEGMENT_EXT_CODE iButM_ReadSerialNum(unsigned long* destinBuf);
BOOL SEGMENT_EXT_CODE iButM_Detect_iButton(void);
BOOL SEGMENT_EXT_CODE iButM_prepareTransfer(void);


#endif //#ifndef IBUTMED_H
