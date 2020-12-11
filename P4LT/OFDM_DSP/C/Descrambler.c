/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600 
 * FILE        : $Workfile: Descrambler.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 *
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Descrambler.c $ 
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       18.02.2003 vgp
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 *****************************************************************************/
 
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/util/gamma.h"

void segment ( "seg_ext_pmco" ) Descrambler(int *In,int *Out)
{
   
   vgamma_off_DD( Out, In );
   Out[OFDM_Num_32b_Words_RX-1]  &=  PCM_32b_Mask_RX;

}

