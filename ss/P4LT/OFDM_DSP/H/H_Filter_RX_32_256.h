/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: H_Filter_RX_32_256.h $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM modem RX filter impulse response for N=256 and BW=32 kHz 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       15.02.2003 vgp
 *****************************************************************************/

#include "../h/OFDM_Defs.h"

complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "../c/h256.dat"
};

	 
