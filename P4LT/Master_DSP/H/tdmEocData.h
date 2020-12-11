/*----------------------------------------------------------------------------*
 * $Workfile: tdmEocData.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/tdmEocData.h $
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 3     17.12.04 17:22 Chhamaa
 * 
 * 2     10.11.04 10:54 Maag01
 * 
 * 1     4.11.04 16:15 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef TDM_EOC_DATA_H
#define TDM_EOC_DATA_H


/* INCLUDE FILES **************************************************************/

#include "tdm_int_types.h"


/* GLOBAL TYPE DECLARATIONS ************************************************/

typedef struct {

  long data; 					// data word
  long full;             		// full/ empty flag
  long dataBreak;        		// data break flag (is set in case of pilot failure)
 
} EocDataBufferType;


/* PUBLIC FUNCTIONS ***********************************************************/

void tdmEocData_init(EocDataBufferType readBuffer[], EocDataBufferType writeBuffer[]);
void tdmEocData_transfer(t_EOC_data *tdmEocDataReadPtr, t_EOC_data *tdmEocDataWritePtr);


#endif
