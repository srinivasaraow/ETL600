/*--------------------------------------------------------------------------
 * $Workfile: modulation.h $
 * Part of		: ETL600 / Up-converter DSP
 * Language		: C
 * Created by	: 
 * Remarks		:  
 * Purpose		: Modulation function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/modulation.h $ 
 * 
 * 1     20.01.05 8:33 Chchlee
 * 
 * 2     7.05.04 15:09 Maag01
 * 
 * 1     6.05.04 18:59 Maag01
 * Initial version
 * 
 *-------------------------------------------------------------------------*/


#ifndef MODULATION_H
#define MODULATION_H


/* INCLUDE FILES ***********************************************************/

#include "speeddsp.h"
#include "SignalGen.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define MOD_OSC_AMPLITUDE	1.0

/* PUBLIC FUNCTIONS ********************************************************/

void modulation_init(float modFreq, OscIQPtr oscModTx);
void quadModulator(float vOut[], complex_float vIn[], OscIQType *osc);


#endif
