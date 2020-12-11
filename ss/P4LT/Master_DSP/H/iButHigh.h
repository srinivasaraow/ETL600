/*-------------------------------------------------------------------------*
 * $Workfile: iButHigh.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/iButHigh.h $
 * 
 * 4     3.06.05 10:33 Chmibuh
 * iButH_getLicenceSerieNum() added
 * 
 * 3     10.04.05 13:38 Chmibuh
 * iButH_getLicenceCode() added
 * 
 * 2     24.02.05 18:22 Chmibuh
 * 
 * 1     23.02.05 14:00 Chmibuh
 *-------------------------------------------------------------------------*/

#ifndef IBUTHIGH_H
#define IBUTHIGH_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/
BOOL SEGMENT_EXT_CODE iButH_is_iButton_valid(void);
BOOL SEGMENT_EXT_CODE iButH_init_iButton(void);
unsigned long SEGMENT_EXT_CODE iButH_getLicenceCode(void);
BOOL SEGMENT_EXT_CODE iButH_licenceErrorAlarm(void);
void SEGMENT_EXT_CODE iButH_getLicenceSerieNum(t_P4LT_Status *p_StatusData);



#endif //#ifndef IBUTHIGH_H

