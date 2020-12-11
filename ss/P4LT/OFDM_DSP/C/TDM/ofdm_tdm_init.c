/*-------------------------------------------------------------------------*
 * $Workfile: ofdm_tdm_init.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : TL Consulting GmbH, 2004
 *-------------------------------------------------------------------------*/
/*
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/ofdm_tdm_init.c $
 * 
 * 2     26.04.05 8:44 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 4     28.09.04 17:45 Maag01
*/
#include <stdlib.h>

#include "../../h/p4lt_tdm/tdm_bus_io.h"
#include "../../h/p4lt_tdm/p4lt_tdm_defs.h"
#include "../../h/util/speeddsp.h"
#include "../../h/util/util.h"

#include "../../h/tdm/ofdm_tdm_externs.h"
#include "../../h/ofdm_externs.h"

#define OFF 0
#define ON  1

void segment ( "seg_ext_pmco" ) OFDM_TDM_Init_Start()
{
 
	int status;
 
    void OFDM_TDM_Stop();

// stop TDM operation    
    OFDM_TDM_Stop();
    
// setup SPORT 1 & 3 in 96 TDM mode of operating
    status = Init_TDM_Bus_IO(P4LT_TDM_NCH_MAX, P4LT_TDM_SLEN, 
                             TDM_RX_CHANNEL_MASK, TDM_TX_CHANNEL_MASK,  
                             (int *)&OFDM_Rx_tdm_buffer[0], (int *)&OFDM_Tx_tdm_buffer[0], TDM_MASTER_OFF, TDM_ISSPORT13_ON);
				 
    if (status != 0) exit(0);

    Toggle_TDM_TX = 0;

    wait_ControlTX_ctr = 0;
   
    IsModemStarted = OFF;
    Start_TDM_Bus_IO(TDM_ISSPORT13_ON);   	
   
}

