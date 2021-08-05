/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_PCM_Gen_Init.c $Log: $
 *
 * Initial version
 *       23.01.2001 vgp
 * Execution from SDRAM
 *       06.04.04 vgp
 *****************************************************************************/
 
#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

//========================================

void segment ( "seg_ext_pmco" ) TCM_PCM_Gen_Init()
{
   PCM_Seed = INF_PCM_SEED;
}

