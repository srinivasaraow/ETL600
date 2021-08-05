/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_MQAM_Train_Gen_Init.c $Log: $
 *
 * Initial version
 *       23.01.2001 vgp
 * Execution from SDRAM
 *       06.04.04 vgp
 *****************************************************************************/
 
#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

//========================================

void segment ( "seg_ext_pmco" ) TCM_MQAM_Train_Gen_Init()
{
   MQAM_Seed_TX = INF_MQAM_SEED;
   MQAM_Seed_RX = INF_MQAM_SEED;
}

