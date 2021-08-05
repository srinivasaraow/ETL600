/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Scrambler.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Scrambler.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 *
 * Initial version
 *       18.02.2003 vgp
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 *****************************************************************************/
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/gamma.h"

void segment ( "seg_ext_pmco" ) Scrambler(int *In,int *Out)
{
   
   vgamma_on_DD( Out, In );
//   Out[OFDM_Num_32b_Words_TX-1]  &=  PCM_32b_Mask_TX;

}

