/*----------------------------------------------------------------------------*
 * $Workfile: upDownConvTdmBus.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/upDownConvTdmBus.h $
 * 
 * 2     10.02.05 16:09 Chbrbus
 * Reduced upconverter TDM bus DMA buffer - changed interrupt routine
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 3     26.10.04 18:58 Maag01
 * 
 * 2     7.10.04 15:43 Maag01
 * 
 * 1     22.07.04 18:28 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef UP_DOWN_CONV_TDM_BUS_H
#define UP_DOWN_CONV_TDM_BUS_H


/* INCLUDE FILES **************************************************************/

#include "speeddsp.h"


/* PUBLIC FUNCTIONS ***********************************************************/

void upDownConvTdmBus_FE_read(long vSamples[], volatile long *rxBufferPtr);
void upDownConvTdmBus_FE_write(complex_float vSamples[], volatile long *txBufferPtr);
void upDownConvTdmBus_UC_read(complex_float vSamples[], long *RxDMA_BufferArrayPtr);
void checkFloatVal(float val[], long nVal);


#endif
