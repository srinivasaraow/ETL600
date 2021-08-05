#include "../h/board.h"
#if BOARD==1
#define DRS7
#else
#undef DRS7
#endif
/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_opt_decide_rate.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Compares optimization results of TX and RX
 *               and chooses data rate out of list
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/OFDM_opt_decide_rate.c $
 * 
 * 6     26.04.05 8:44 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 4     7.02.05 14:39 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 9     22.12.04 9:30 Chchlee
 * 
 * 8     2.12.04 17:54 Chhamaa
 * 
 * 6     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       22.04.2004 PL
 * Init of V11 clock dividers (for DSR7 board only, 29.06.04 vgp)
 *       11.05.04 vgp
 * Added error handler
 *	   Jan/Feb 05 PL
 ******************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//#include "../h/List_of_data_rates.h"
#include "../h/tcm/TCM_Codec_externs.h"

#include "../h/constant.h"

#include "../h/v11/v11_externs.h"

extern unsigned int Size_Of_Rate_List;
extern float    List_of_data_rates[];


//========================================

int segment ( "seg_ext_pmco" ) OFDM_opt_decide_rate(int *User_inf_bit_TX, int *User_inf_bit_RX, int Datarate_Mode)
// // Returns               0 - on success, otherwise returns -1
// //
// // Mode = 0:	
// // 			TX and RX independent
// //			Chooses the maximum possible data rate
// //  			!! Ignores data rate list !!
// //
// // Mode = 1:
// // 			TX and RX rate must be equal
// //			Takes the minimum of both as target value for both directions
// //  			!! Ignores data rate list !!
// //
// // Mode = 2:
// // 			TX and RX rate must be equal
// //			Takes a predefined fixed data rate as target value for both directions
// //  			!! Ignores data rate list !!
// //			!! A higher bit rate than the possible one is allowed here, which will result in bit errors
// //
// // Mode = 3: (Standard)
// // 			TX and RX rate must be equal
// //			Takes the minimum of both as intermediate value for both directions
// //  			Search in data rate list for highest possible data rate for both directions
// //


{
	
 int inf_bits_TX, inf_bits_RX , inf_bits_target, Num_of_data_rates;
 int i;
 float data_rate_max, data_rate_selected , list_value;
 float Min_data_rate, Max_data_rate;
 int Stuffing_Error;
 
 int OFDM_opt_decide_stuffing();

// uses globals: float Selected_data_rate_TX , Selected_data_rate_RX;
//				 float Fixed_data_rate_TX    , Fixed_data_rate_RX; (only in Mode 2)
// 				 float List_of_data_rates[]

// Evaluate List of data rates
//Num_of_data_rates = sizeof(List_of_data_rates);
Num_of_data_rates = Size_Of_Rate_List;  //PL replace later
										// introduced for debug, to use full data rate list
										// instead of ABBs short list of 5 rates

Min_data_rate     = vmin_f_E(List_of_data_rates,Num_of_data_rates); 
Max_data_rate     = vmax_f_E(List_of_data_rates,Num_of_data_rates); 

// Calculate maximum data rates for TX and RX direction
inf_bits_TX = *User_inf_bit_TX;  
inf_bits_RX = *User_inf_bit_RX;
Data_rate_max_TX = floorf( (float)inf_bits_TX / T_OFDM_symbol);
Data_rate_max_RX = floorf( (float)inf_bits_RX / T_OFDM_symbol);

switch (Datarate_Mode)
	{
	case 0:
	
	break;
	
	case 1:
	
	break;
	
	case 2:
		// fetch possible maximum values of User information bits per T_OFDM_symbol
		inf_bits_TX = *User_inf_bit_TX;  
		inf_bits_RX = *User_inf_bit_RX;
	
		// Select minimum value as intermediate target value for both directions
		inf_bits_target = min(inf_bits_TX,inf_bits_RX);
	
		// Calculate resulting maximum possible speed for both directions
		data_rate_max = floorf( (float)inf_bits_target / T_OFDM_symbol);
		
		// Check for minimum data rate
		if ((data_rate_max < Selected_data_rate_TX)
			||(data_rate_max < Selected_data_rate_RX))
			{
			// Minimum data rate can't be achieved
			// no error, force modem to this data rate
			// return -1;
			}
		// Fixed data rate, predefined in Selected_data_rate_TX and Selected_data_rate_RX
	
		*User_inf_bit_TX = (int)(Selected_data_rate_TX * T_OFDM_symbol + 0.9f) ;
		*User_inf_bit_RX = (int)(Selected_data_rate_RX * T_OFDM_symbol + 0.9f) ;
		
	
	break;
	
	case 3:
	
		// fetch possible maximum values of User information bits per T_OFDM_symbol
		inf_bits_TX = *User_inf_bit_TX;  
		inf_bits_RX = *User_inf_bit_RX;
	
		// Select minimum value as intermediate target value for both directions
		inf_bits_target = min(inf_bits_TX,inf_bits_RX);
	
		// Calculate resulting maximum possible speed for both directions
		data_rate_max = floorf( (float)inf_bits_target / T_OFDM_symbol);
		
		// Check for minimum data rate
		if (data_rate_max < Min_data_rate)
			{
			// Error: Minimum data rate can't be achieved
			//return -1;  //nevertheless use it
			}
		 
		// Search into list of data rates for next lower (or equal) data rate
		data_rate_selected = Min_data_rate;
		for(i=0;  i<N_DATA_RATES; i++)
			{
			list_value = List_of_data_rates[i];
			if ( (list_value <= data_rate_max) &
				 (list_value >  data_rate_selected)	)
				 {
				 data_rate_selected = list_value;
				 Selected_data_rate_IDX_prepared = i;
				 }
			else
			if ( list_value == data_rate_selected	)
				 Selected_data_rate_IDX_prepared = i;
			}
		// Calculate number of required information bits for selected data rate
			//Note: next line with ceilf() returns wrong result unfortunately			
			// inf_bits_target = (int) ceilf(data_rate_selected * T_OFDM_symbol ) ;
		inf_bits_target = (int)((data_rate_selected/1000.0f) * (T_OFDM_symbol*1000.0f) + 0.9f) ;
	
		*User_inf_bit_TX = inf_bits_target;
		*User_inf_bit_RX = inf_bits_target;
		Selected_data_rate_TX = data_rate_selected;
		Selected_data_rate_RX = data_rate_selected;
		
		// Get Lower/Higher_rate_available
		// which is required in OFDM_Fall_BackForward()
		if (data_rate_selected > Min_data_rate) 
				Lower_rate_available = 1;
		else
				Lower_rate_available = 0;
			
		if (data_rate_selected < Max_data_rate) 
				Higher_rate_available = 1;
		else
				Higher_rate_available = 0;
		
		
		
	break;
	}
	
	
	// Calculate Bit Stuffing Scheme
	if (OFDM_opt_decide_stuffing()!=0) return -2;
	
	
	// Calculate V11 interface serial clock frequency devisor 
//		V11_Clock_Div_TX = CORE_CLOCK_KHZ;

#ifdef DRS7
	V11_Clock_Div_TX = (DSP_CLK_INT_Hz/2)/(int)Selected_data_rate_TX;
	if ( V11_Clock_Div_TX*Selected_data_rate_TX != DSP_CLK_INT_Hz/2 ) {
		V11_Clock_Div_TX = -1;
	    return -3;
	}
	
	V11_Clock_Div_RX = (DSP_CLK_INT_Hz/2)/(int)Selected_data_rate_RX;
	if ( V11_Clock_Div_RX*Selected_data_rate_RX != DSP_CLK_INT_Hz/2 ) {
		V11_Clock_Div_RX = -1;			
		return -3;
	}	
#endif

return 0;		
}

