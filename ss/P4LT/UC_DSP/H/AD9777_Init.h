/*----------------------------------------------------------------------------*
 * $Workfile: AD9777_Init.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT/UC_DSP/H/AD9777_Init.h $
 * 
 * 3     7.09.05 15:36 Chhamaa
 * AD9777_Init_ModeControlReg() modified
 * 
 * 2     16.02.05 16:39 Chhamaa
 * 
 * 4     9.02.05 17:02 Chhamaa
 * AD9777_Init_ModeControlReg() modified
 * 
 * 3     1.09.04 14:24 Buhl01
 * reestablishSPI() added
 * 
 * 2     21.06.04 16:59 Maag01
 * 
 * 1     27.05.04 14:36 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef AD9777_INIT_H
#define AD9777_INIT_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"


/* PUBLIC FUNCTIONS ***********************************************************/

void AD9777_Init_HW_Reset(void);
void reestablishSPI(void);
BOOL AD9777_Init_ModeControlReg(long rfPepMax);


#endif
