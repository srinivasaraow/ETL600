/*-------------------------------------------------------------------------*
 * $Workfile: ofdm_tdm_stop.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/ofdm_tdm_stop.c $
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
*/
#include <signal.h>


void segment ( "seg_ext_pmco" ) OFDM_TDM_Stop()
{

    asm("#include <def21161.h>");
    asm("dm(CP1A)=m5;");
    asm("dm(CP3A)=m5;");
    asm("dm(SPCTL1) = m5;");
    asm("dm(SPCTL3) = m5;");

    asm("bit clr imask SP1I | SP3I;");

}
