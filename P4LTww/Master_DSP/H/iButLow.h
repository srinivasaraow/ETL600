/*-------------------------------------------------------------------------*
 * $Workfile: iButLow.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/iButLow.h $
 * 
 * 2     23.02.05 14:29 Chmibuh
 * unused functions removed
 * 
 * 1     23.02.05 14:01 Chmibuh
 *-------------------------------------------------------------------------*/

#ifndef IBUTLOW_H
#define IBUTLOW_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/


/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/

BOOL SEGMENT_EXT_CODE iButL_Reset(void);
void SEGMENT_EXT_CODE iButL_WriteByte(unsigned long data);
unsigned long SEGMENT_EXT_CODE iButL_ReadByte(void);
unsigned long SEGMENT_EXT_CODE iButL_TouchByte(unsigned long data);
void SEGMENT_EXT_CODE iButL_Block(unsigned long *data, unsigned long data_length);


#endif //#ifndef IBUTLOW_H
