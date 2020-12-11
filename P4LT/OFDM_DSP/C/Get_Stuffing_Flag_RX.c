/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: Get_Stuffing_Flag_RX.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Calculates Stuffing Scheme
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Get_Stuffing_Flag_RX.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       18.05.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>

#include "../h/OFDM_externs.h"
#include "../h/OFDM_Defs.h"



int segment ( "seg_ext_pmco" ) Get_Stuffing_Flag_RX()
// // Input: n_blk_rx_data
// // Output:
// // Returns               0 - no stuffing applied in this block
// //						1 - Stuffing applied in this block
{

 static int St_period_counter_RX;	
		int Stuffing_Flag;

		
	// Initialize Period Counter
	if (n_blk_rx_data == 0) St_period_counter_RX=1;

	if (St_period_counter_RX <= Stuffing_symb_per_period)
		Stuffing_Flag=1;
	else
		Stuffing_Flag=0;
	
	// Increment or Reset Period Counter
	if (++St_period_counter_RX > Stuffing_Period ) St_period_counter_RX=1;
	
	return Stuffing_Flag;
}

