/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Descrambler_Init.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Descrambler_Init.c $ * 
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       18.02.2003 vgp
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * Independent TX/RX paths
 *       23.01.04 vgp
 *****************************************************************************/
 
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/ofdm_externs.h"
#include "../h/util/util.h"

int segment ( "seg_ext_pmco" ) Descrambler_Init()
{

    Descrambler_Ind = RX_DELAY_SYMB*OFDM_Num_32b_Words_RX;  // 2, because do not ask why life is so good
    Descrambler_Seed = 876545;
    Descrambler_Length = MAX_SCRAMBLE_RX;
    DRS7_setseed(Descrambler_Seed);
    DRS7_vrand_E(Descramble_Data, Descrambler_Length );            

    return 0;
}

