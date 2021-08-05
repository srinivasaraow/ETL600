/*----------------------------------------------------------------------------*
 * $Workfile: afc.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/afc.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 1     8.12.04 17:51 Chhamaa
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/

#ifndef FREQUENCY_SYNC_H
#define FREQUENCY_SYNC_H


/* INCLUDE FILES **************************************************************/

#include "APLC_Types.h"
#include "PilotMod.h"
#include "oscillator.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {
 
	float freqOffset;          	// link frequency offset
	float oscFreq;				// oscillator frequency to be modified
	float ki;                   // integral constant
	float kp;                   // proportional constant 
	float ferr;					// frequency error
	float stateI;				// integrator state
	float dfLpState;			// low pass filter state for frequency detector output
	float inOld;				// differentiator state
	float quadOld;				// differentiator state
	float sgn;					// sign of frequency offset (depends on channel mode)
	long  initCounter;			// is set at initialisation to garantee start up also at bad channel conditions
	long  waitSyncCounter;     	// counter after start up or sync. failure
	long  fastTrackingCounter;	// counter for trackin duration
	long  on;					// synchronisation is on
  
} t_AFC; 


/* PUBLIC FUNCTIONS ***********************************************************/

void afc_init(t_AFC *afc, unsigned long channelMode);
void afc_activate(t_AFC *afc, GuardDataType *gStatus, PilotDemodDataType *pilotDemod,
				  t_OscData *osc);
void afc_monitor(t_AFC *afc, GuardDataType *gStatus, t_APLC_Status *aplc, BOOL afcEnabled);

#endif
