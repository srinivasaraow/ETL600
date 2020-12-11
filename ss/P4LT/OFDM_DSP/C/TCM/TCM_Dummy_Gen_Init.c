/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Dummy_Gen_Init.c $Log: $
 *
 * Initial version
 *       23.01.2001 vgp
 * Execution from SDRAM
 *       06.04.04 vgp
 *****************************************************************************/
 
#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

//========================================

void segment ( "seg_ext_pmco" ) TCM_Dummy_Gen_Init()
{
   Dummy_Seed = INF_DUMMY_SEED;
}

