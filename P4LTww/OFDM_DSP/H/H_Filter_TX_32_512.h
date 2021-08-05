/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: H_Filter_TX_32_512.h $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : OFDM modem TX filter impulse response for N=512 and BW=32 kHz
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       15.02.2003 vgp
 *****************************************************************************/

#include "../h/OFDM_Defs.h"

complex_float segment ( "seg_extn_init" ) H_Filter_TX_32_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "../c/h512.dat"
};

	  
