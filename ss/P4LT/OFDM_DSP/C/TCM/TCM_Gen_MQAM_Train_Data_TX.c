/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Gen_MQAM_Train_Data_TX.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Generates PCM data vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/TCM_Gen_MQAM_Train_Data_TX.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 4     1.10.04 12:34 Leeb02
 *
 * Initial version
 *       23.01.2001 vgp
 *****************************************************************************/
 
#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"

//========================================

void segment ( "seg_ext_pmco" ) TCM_Gen_MQAM_Train_Data_TX(unsigned int *OutPCM32, int len, int Mask)
{
	unsigned int Saved_Seed;

	unsigned int DRS7_setseed(unsigned int);
	unsigned int DRS7_getseed();
	void         DRS7_vrand_E(unsigned int *, int );			

	// save seed 
	Saved_Seed = DRS7_getseed();

	// load seed of previous block at the end of symbol
    DRS7_setseed(MQAM_Seed_TX);

    // generate vector
    DRS7_vrand_E(OutPCM32, len);    
    OutPCM32[len-1] &= Mask;          // Mask = PCM_32b_Mask_TX;

    // save PCM seed
    MQAM_Seed_TX =  DRS7_getseed();
    
    // restore seed
    DRS7_setseed(Saved_Seed);

}

