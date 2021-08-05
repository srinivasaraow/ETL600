/*----------------------------------------------------------------------------*
 * $Workfile: Equalizer.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/Equalizer.h $
 * 
 * 2     16.02.05 13:50 Chhamaa
 * 
 * 9     27.01.05 19:19 Chhamaa
 * 
 * 8     20.01.05 18:28 Chhamaa
 * 
 * 7     18.01.05 18:55 Chhamaa
 * 
 * 6     13.01.05 18:48 Chhamaa
 * 
 * 5     13.01.05 17:05 Chhamaa
 * 
 * 4     11.01.05 18:16 Chhamaa
 * 
 * 3     11.01.05 17:39 Chhamaa
 * 
 * 2     10.01.05 16:58 Chhamaa
 * 
 * 1     10.01.05 11:14 Chhamaa
 * Initial version based on ETL500 version 4.20
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef EQUALIZER_H
#define EQUALIZER_H


/* INCLUDE FILES **************************************************************/

#include "equalizerTypes.h"
#include "equalizerFirFilter.h"


/* PUBLIC FUNCTIONS ***********************************************************/

void equalizer_init(long channel);
void equalizer_tdmDataTransfer(void);
void equalizer_stateMachinePnController(long Channel);
void equalizer_measureChannel(long channel);
void equalizer_stateMachineAccController(long channel);

t_EqualizerStatus *equalizer_getStatusNewPtr(long channel);
t_EqualizerFirCoef *equalizer_getFirCoefPtr(long channel);

#endif
