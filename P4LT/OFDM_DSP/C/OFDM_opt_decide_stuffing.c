/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_opt_decide_stuffing.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Evaluates Stuffing scheme in dependence of data rate
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_opt_decide_stuffing.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 2     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       17.05.2004 PL
 ******************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

//========================================

int OFDM_opt_decide_stuffing()
// // Returns               0 - on success, otherwise returns -1
// //
{
	
// uses globals: float Selected_data_rate_TX , Selected_data_rate_RX;
//				 float T_OFDM_symbol
//	Output:	int Stuffing_Period
//			int	Stuffing_symb_per_period
//			float Check_rate  (for checking of result)
//					
/*
float N_inf_bit_soll, N_inf_bit_ist;
//float Check_rate;
int N_bit_dn, N_bit_up;
int diff5i, diff3i;
float diff_bit, diff5, diff3;
int N_bit_in_period;
*/

N_inf_bit_soll = T_OFDM_symbol * Selected_data_rate_TX;

N_bit_dn = (int)(N_inf_bit_soll+0.01f);		// next lower integer
N_bit_up = (int)(N_inf_bit_soll+0.9f);	// next higher integer
//N_bit_dn = floorf(N_inf_bit_soll);		// next lower integer
//N_bit_up = ceilf(N_inf_bit_soll);	// next higher integer


if (N_bit_dn == N_bit_up)
	{// No Stuffing required
	Stuffing_Period = 1;
	Stuffing_symb_per_period = 0;
	}
	else
	{// Stuffing required: Decide Period
	diff_bit = N_inf_bit_soll - (float)(N_bit_dn);
	// Period 5: diff_bit is element of [0.2 ; 0.4 ; 0.6 ; 0.8]
	// Period 4: diff_bit is element of [0.25 ; 0.75]
	// Period 3: diff_bit is element of [0.33 ; 0.66]
	// Period 2: diff_bit is element of [0.5]
	

	diff5= 5.0f * diff_bit;	// if P=5 then the result is element of [1.0 ; 2.0 ; 3.0 ; 4.0] 
	diff4= 4.0f * diff_bit; // if P=4 then the result is element of [1.0 ; (2.0) ; 3.0]
	diff3= 3.0f * diff_bit;	// if P=3 then the result is element of [1.0 ; 2.0]
	diff2= 2.0f * diff_bit;	// if P=2 then the result is element of [1.0]
	
	
	diff5i = (int)(diff5+0.5f);
	diff4i = (int)(diff4+0.5f);
	diff3i = (int)(diff3+0.5f);
	diff2i = (int)(diff2+0.5f);

	// if diff5 is integer
	if (fabsf(diff5-(float)diff5i) < 0.01f)
		{
		// Period is 5
		Stuffing_Period=5;
		Stuffing_symb_per_period=diff5i;	
		}
	
	// if diff4 is integer
	if (fabsf(diff4-(float)diff4i) < 0.01f)
		{
		// Period is 4
		Stuffing_Period=4;
		Stuffing_symb_per_period=diff4i;	
		}	
	// if diff3 is integer
	if (fabsf(diff3-(float)diff3i) < 0.01f)
		{
		// Period is 3
		Stuffing_Period=3;
		Stuffing_symb_per_period=diff3i;	
		}
	// if diff2 is integer
	if (fabsf(diff2-(float)diff2i) < 0.01f)
		{
		// Period is 2
		Stuffing_Period=2;
		Stuffing_symb_per_period=diff2i;	
		}
	}
	
// Check result
N_bit_in_period = Stuffing_symb_per_period * N_bit_up + (Stuffing_Period - Stuffing_symb_per_period) * N_bit_dn;
N_inf_bit_ist = (float)N_bit_in_period / (float)Stuffing_Period;
Check_rate = N_inf_bit_ist / T_OFDM_symbol;

if (fabsf(Check_rate - Selected_data_rate_TX)>0.1f) 
	{
		asm("nop;");
		asm("nop;");
		asm("nop;");
		return -1;
	}


return 0;		
}

