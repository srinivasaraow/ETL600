/*---------------------------------------------------------------------------*
 * $Workfile: freqShift.h $
 * Part of      : ETL600 / Front end DSP
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Frequency shift routines
 * Copyright    : ABB Switzerland Ltd., 2003
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/freqShift.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 4     2.12.04 14:59 Chhamaa
 * New function prototype added: getShiftFreqAPLC()
 * 
 * 3     8.07.04 19:24 Maag01
 * 
 * 2     17.06.04 16:11 Maag01
 * 
 * 1     30.04.04 15:03 Maag01
 * Initial version
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef _FREQ_SHIFT
#define _FREQ_SHIFT


/* INCLUDE FILES *************************************************************/

#include "oscillator.h"


/* PUBLIC FUNCTIONS **********************************************************/

float getShiftFreqAPLC(unsigned long channelMode);
void initFreqShiftAPLC(t_OscData *oscShiftTx, t_OscData *oscShiftRx, unsigned long channelMode);
void initFreqShiftCh3(t_OscData *oscShiftTx, t_OscData *oscShiftRx, unsigned long channelMode);
void freqShift(float *in, float *out, t_OscData *osc, int nSamples);

#endif
