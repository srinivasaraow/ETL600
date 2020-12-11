/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Restart.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Initialize OFDM modem
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_Restart.c $
 * 
 * 2     15.02.05 10:01 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 2     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 vgp
 *****************************************************************************/
 
#include <signal.h>
#include "../h/OFDM_externs.h"



asm(".extern ___lib_start;");
//asm(".extern _Restart_Counter;");
//========================================
void segment ( "seg_ext_pmco" ) OFDM_Restart()
	{
	asm("#include <def21161.h>");
    asm("dm(CP1A)=m5;");
    asm("dm(CP3A)=m5;");
    asm("dm(SPCTL0) = m5;");
    asm("dm(SPCTL2) = m5;");
    asm("bit clr imask SP1I | SP3I| SP0I| SP2I;");
    // increment restart counter
    Restart_Counter++;    
    
    asm("jump ___lib_start;");
	}


