/*-------------------------------------------------------------------------*
 * $Workfile: txSignalProcessingOfdm.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/FE_DSP/H/txSignalProcessingOfdm.h $ 
 * 
 * 1     5.04.06 14:19 Chhamaa
 * 
 * 1     7.02.06 9:55 Chhamaa
 * Initial version.
 * 
 * 
 *-------------------------------------------------------------------------*/

#ifndef TX_SIGNAL_PROCESSING_OFDM_H
#define TX_SIGNAL_PROCESSING_OFDM_H


/* INCLUDE FILES **************************************************************/

#include "speeddsp.h"
#include "FE_DSP_Types.h"

/* PUBLIC TYPE DECLARATIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/

void txSignalProcessingOfdm_init(t_FrontEndDSP_Config *config);
void txSignalProcessingOfdm_activate(complex_float vIn[], complex_float vOut[]);


#endif
