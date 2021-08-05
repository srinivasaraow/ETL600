/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Delta_Gen_Init.c $Log: $
 *
 * Initial version
 *       12.05.2004 vgp
 *****************************************************************************/
 
#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

//========================================

void segment ( "seg_ext_pmco" ) TCM_Delta_Gen_Init()
{
   DeltaBits_Seed = INF_DELTA_SEED;
}

