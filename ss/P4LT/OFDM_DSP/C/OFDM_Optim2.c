/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Optim2.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Secondary optimization for minimum power
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/OFDM_Optim2.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       23.04.2004 PL
 *
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"

#include "../h/tcm/TCM_Codec_externs.h"




//========================================
void segment ( "seg_ext_pmco" ) OFDM_Optim2()
	{
// **********************************************************************************
//        2. TCM-Optimization to required number of information bits				*							*
//                      - minimize Power at given number of information bits		*
//						  															*													*
// **********************************************************************************
				
	// Input: 	TCM_optim_inf_bit_TX, TCM_optim_inf_bit_RX (Sollwert)
	//			K_Sort_2_TX,K_Sort_2_RX
	//			Psi_TX , Psi_RX          
    //       
	//
	// Output: 	M_Opt_TX, M_Opt_RX
	//			Bits_in_SC_TX , Bits_in_SC_RX
	
	int TCM_bitcalc(int *, int, int *, float *, int *, int, int);
	
	// int opti;
	int i;
	int M;
	int TCM_Error;
	int M_is_even;
	
	
	
	float R_i , R_i_2;
	float R_i_base;
	float dbg_R_i_sum, dbg_Q_i_sum, dbg_N_R_target , dbg_N_bit; // debug
	

	
	
	switch ( Optim2_RX )
   		{
	   	case 0:
		
		
      		
      	// ******************************************************************************
      	//         Define target rate value for Optimization									*
      	//                                  ( adapt result of first optimization)	*
      	// equation (2.36)
      	// ******************************************************************************
    	//R_target_TX = 0.5f/((float)N) * (float)(Optim_sr_inf_bit_TX + CRC_LEN + TERM_BITS);
   		//R_target_RX = 0.5f/((float)N) * (float)(Optim_sr_inf_bit_RX + CRC_LEN + TERM_BITS);
		R_target_TX = 0.5f/((float)N) * (float)(TCM_optim_sr_inf_bit_TX + CRC_LEN + TERM_BITS);
   		R_target_RX = 0.5f/((float)N) * (float)(TCM_optim_sr_inf_bit_RX + CRC_LEN + TERM_BITS);

      	// ******************************************************************************
      	//         Search for M_Opt, that is maximum number of usefull subchannels		*
      	//                                  (depends on measured channel K(i), sigma^2	*
      	//					and given maximum error probability)						*
      	// ******************************************************************************
	
      	// - TX -----------------------------------------------------------
			M_Opt_TX = M_Opt_max_TX;
			
			// calculate R in subchannel i (R_i)
			// as long as R_i >0 increase number of subchannels (M_Opt)
			// see equation (2.30)
			
			for(i=0;i<M_Opt_max_TX;i++)
				{
				R_i = (float)N/((float)(i+1)) * R_target_TX - Psi_TX[i] + half_log2* log10f(K_Sort_2_TX[i]);
				if(R_i < 0.0f) //0.0 replaced by 1.0: at least 1 information bit is required
					{
					M_Opt_TX = i - 1;
					break;
					}
				}
			//  Limit M_Opt to allowed maximum number of subchannels
   			if (M_Opt_TX > M_Opt_max_TX)
      				{
      				M_Opt_TX = M_Opt_max_TX;
   		   			}
   		    
   		   	// M_Opt must be even
   		   	if ( (M_Opt_TX & 1) == 1 ) 
         			{
         			M_Opt_TX-=1;
         			}
         		
   		   	// save maximum number of usefull channels for bordershift algorithm
   		   	M_Opt2_max_TX=M_Opt_TX;
   		   	
   		// **********************************************************************************
      	//         Calculate Bit Distribution												*
      	//			(Maximization of data rate under restriction of limited power)			*
        //				TX																	*
      	// **********************************************************************************
  		   	
      		// **********************************************************************************			
      		// *     Theorie and Formulas:
      		// *     With known number of useful subchannels M, we can calculate
      		// *     optimum Rate distribution to subchannels i
      		// *     R(i) = N/M*R_target - PSI(M) + 1/2log2(|ki|^2) 	(eq.2.31)
      		// *
      		// *     The first term is independent of index i, so it can be substituted by
      		// *     R_i_base = N/M*R_target - PSI(M)
      		// * 
      		// *     Even integer numbers of bits/subchannel (code bits) are derived from
      		// *     the continuous result R(i) (information bits) by a table, 
      		// *     that considers the signal code construction (!table valid for Shaping ON only!)
      		// *     
      	   	// **********************************************************************************

        
  	
     	// - TX -----------------------------------------------------------
    		
         // delete Bit Allocation	
         vset_f_E((float *)&Bits_in_SC_TX,0.0f,sizeof(Bits_in_SC_TX));
	
       	// M_is_even = 0;
       	// M_trial_counter_tx = 0;
	//	 while (!M_is_even) // repeat until result of M is even
	//		{
      		R_i_base = ((float)N/(float)M_Opt_TX)*R_target_TX - Psi_TX[M_Opt_TX-1];
      		M=0;
      		dbg_R_i_sum = 0.0f; // for debug only
      		
      		for (opti=0;opti<M_Opt_TX;opti++)
      				{
      				R_i = R_i_base + half_log2*log10f(K_Sort_2_TX[opti]);
      				R_i_2 = 2.0f*R_i;
      				
      				R_Temp[opti] = R_i_2; // for debug only
      				
      				dbg_R_i_sum += R_i; // for debug only
      				
      				if (R_i_2 <= TH_OPTIM2_QAM4)
      					{Bits_in_SC_TX[opti] = 0;}
      				if (( TH_OPTIM2_QAM4 < R_i_2)&(R_i_2 <= 2.0f))
      					{Bits_in_SC_TX[opti] = 2;}
      				if (( 2.0f < R_i_2)&(R_i_2 <= 3.5f))
      					{Bits_in_SC_TX[opti] = 4;}
      				if (( 3.5f < R_i_2)&(R_i_2 <= 5.5f))
      					{Bits_in_SC_TX[opti] = 6;}
      				if (( 5.5f < R_i_2)&(R_i_2 <= 7.5f))
      					{Bits_in_SC_TX[opti] = 8;}
      				if (( 7.5f < R_i_2)&(R_i_2 <= 9.5f))
      					{Bits_in_SC_TX[opti] = 10;}
      				if (( 9.5f < R_i_2)&(R_i_2 <= 11.5f))
      					{Bits_in_SC_TX[opti] = 12;}
      				if ((11.5f < R_i_2))
      					{Bits_in_SC_TX[opti] = 14;}
      				
      				if (Bits_in_SC_TX[opti] > MAXBITS_IN_SC)
         				{
         				Bits_in_SC_TX[opti]=MAXBITS_IN_SC;
         				}
         			if (Bits_in_SC_TX[opti] > 0) M++;
       				}
			
      		// Force remaining values to zero
   			for (opti=M_Opt_TX;opti<M_Opt_max_TX;opti++)
      				{
      				Bits_in_SC_TX[opti]=0;
      			}	
      		
      		// Check result (debug)
      		if(OFDM_Loop_Mode == DIGITAL_LOOP)
			{
      		 dbg_Q_i_sum = 2.0f* dbg_R_i_sum; // summe der informations bits
      		 // next value should be equal to dbg_R_i_sum
      		 dbg_N_R_target = (float)N*R_target_TX; 
      		 // next value should be equal to TCM_optim_inf_bit_TX
      		 dbg_N_bit = dbg_R_i_sum *2.0f - (float)CRC_LEN - (float)TERM_BITS ;
			}
			
      		// Check if number of active carriers M is even (!reconsider!)
           	if ( (M & 1) == 1 ) 
         		{
         		
         		//M_trial_counter_tx++; 	// oops, M was odd	
         		//M_Opt_TX--;     	// so decrease and repeat optimization
         		M_is_even = 0;
         		Bits_in_SC_TX[M]=2;
         		M+=1;
         		}
       		else
       			{
            	M_is_even=1;   // so it's ok, terminate "while" loop
       			}
            
 		 //} // End of while (!M_is_even) 	  
      	
 		 // Result	
 		 M_Opt_TX = M;
 		 
      		
      	 // Debug: Display Result  !! result RX is erased here
 		 // *********** For debug only , sorting to physical order for display
      	if(OFDM_Loop_Mode == DIGITAL_LOOP)
			{
				
 		 	// !!!!!!!! to remove !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      		vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_TX[0],N);
      		vcopy_DE(&Fl_Array_R[N],(float *)&Index_K_TX[0],N);
			vscatter_i_DDD((int *)&Fl_Array_R[N2],(int *)&Fl_Array_R[0],(int *)&Fl_Array_R[N],N);
			vcopy_ED((float *)&Bits_in_SC_RX[0],&Fl_Array_R[N2],N);
      		// ************ End of debug section		
			
      		}
			
      	
		if (M_Opt_TX > 0) 	
      		{	
			// Calculate maximum number of TCM information bits and maximum data rate
 	    	Optim2_error_TX	 =	TCM_bitcalc(&D2_alloc_TX[0], Shaping_Flag_tx,&Bits_in_SC_TX[0], C_TX, Index_K_TX, M_Opt_TX,0);
      		} else {
      		Optim2_error_TX = 0;
      		TCM_Num_Inf_Bits = 0;
      		}
      		
 	    // Save intermediate results of TCM_bitcalc in Optim2...TX global variables	
			Optim2_inf_bit_TX	= TCM_Num_Inf_Bits;
			Optim2_rate_TX 		= (float)TCM_Num_Inf_Bits / T_OFDM_symbol;  
			Optim2_M_TX = M;

		Optim2_RX++;
		
 		break;
		
		
 		case 1:
   		// ******************************************************************************
		// ******************************************************************************
		// ******************************************************************************
		// ******************************************************************************
	 

			
			
		// ******************************************************************************
      	//         Search for M_Opt, that is maximum number of usefull subchannels		*
      	//                                  (depends on measured channel K(i), sigma^2	*
      	//					and given maximum error probability)						*
      	// ******************************************************************************
  	
   		   	
   		   	
      	// - RX -----------------------------------------------------------
			M_Opt_RX = M_Opt_max_RX;
			
			// calculate R in subchannel i (R_i)
			// as long as R_i >0 increase number of subchannels (M_Opt)
			// see equation (2.30)
			
			for(i=0;i<M_Opt_max_RX;i++)
				{
				R_i = (float)N/((float)(i+1)) * R_target_RX - Psi_RX[i] + half_log2* log10f(K_Sort_2_RX[i]);
				if(R_i < 0.0f)
					{
					M_Opt_RX = i - 1;
					break;
					}
				}
			//  Limit M_Opt to allowed maximum number of subchannels
   			if (M_Opt_RX > M_Opt_max_RX)
      				{
      				M_Opt_RX = M_Opt_max_RX;
   		   			}
   		   	
   		   	// M_Opt must be even
   		   	if ( (M_Opt_RX & 1) == 1 ) 
         			{
         			M_Opt_RX-=1;
         			}
   		   			
   		   	// save maximum number of usefull channels for bordershift algorithm
   		   	M_Opt2_max_RX=M_Opt_RX;
   		   	
   		// **********************************************************************************
      	//         Calculate Bit Distribution												*
      	//			(Maximization of data rate under restriction of limited power)			*
        //				RX																	*
      	// **********************************************************************************
 
   		   			
	// - RX -----------------------------------------------------------
	 	
		// delete Bit Allocation	
        vset_f_E((float *)&Bits_in_SC_RX,0.0f,sizeof(Bits_in_SC_RX));
	
       	// M_is_even = 0;
       	// M_trial_counter_tx = 0;
	//	 while (!M_is_even) // repeat until result of M is even
	//		{
      		R_i_base = ((float)N/(float)M_Opt_RX)*R_target_RX - Psi_RX[M_Opt_RX-1];
      		M=0;
      		dbg_R_i_sum = 0.0f; // for debug only
      		
      		for (opti=0;opti<M_Opt_RX;opti++)
      				{
      				R_i = R_i_base + half_log2*log10f(K_Sort_2_RX[opti]);
      				R_i_2 = 2.0f*R_i;
      				
      				R_Temp[opti] = R_i_2; // for debug only
      				
      				dbg_R_i_sum += R_i; // for debug only
      				
      				if (R_i_2 <= TH_OPTIM2_QAM4)
      					{Bits_in_SC_RX[opti] = 0;}
      				if (( TH_OPTIM2_QAM4 < R_i_2)&(R_i_2 <= 2.0f))
      					{Bits_in_SC_RX[opti] = 2;}
      				if (( 2.0f < R_i_2)&(R_i_2 <= 3.5f))
      					{Bits_in_SC_RX[opti] = 4;}
      				if (( 3.5f < R_i_2)&(R_i_2 <= 5.5f))
      					{Bits_in_SC_RX[opti] = 6;}
      				if (( 5.5f < R_i_2)&(R_i_2 <= 7.5f))
      					{Bits_in_SC_RX[opti] = 8;}
      				if (( 7.5f < R_i_2)&(R_i_2 <= 9.5f))
      					{Bits_in_SC_RX[opti] = 10;}
      				if (( 9.5f < R_i_2)&(R_i_2 <= 11.5f))
      					{Bits_in_SC_RX[opti] = 12;}
      				if ((11.5f < R_i_2))
      					{Bits_in_SC_RX[opti] = 14;}
      				
      				if (Bits_in_SC_RX[opti] > MAXBITS_IN_SC)
         				{
         				Bits_in_SC_RX[opti]=MAXBITS_IN_SC;
         				}
         			if (Bits_in_SC_RX[opti] > 0) M++;
       				}
			
      		// Force remaining values to zero
   			for (opti=M_Opt_RX;opti<M_Opt_max_RX;opti++)
      				{
      				Bits_in_SC_RX[opti]=0;
      			}	
      		
      		// Check result (debug)
      		if(OFDM_Loop_Mode == DIGITAL_LOOP)
			{
      		 dbg_Q_i_sum = 2.0f* dbg_R_i_sum; // summe der informations bits
      		 // next value should be equal to dbg_R_i_sum
      		 dbg_N_R_target = (float)N*R_target_RX; 
      		 // next value should be equal to TCM_optim_inf_bit_TX
      		 dbg_N_bit = dbg_R_i_sum *2.0f - (float)CRC_LEN - (float)TERM_BITS ;
			}
			
      		// Check if number of active carriers M is even (!reconsider!)
           	if ( (M & 1) == 1 ) 
         		{
         		
         		//M_trial_counter_rx++; 	// oops, M was odd	
         		//M_Opt_RX--;     	// so decrease and repeat optimization
         		M_is_even = 0;
         		Bits_in_SC_RX[M]=2;
         		M+=1;
         		}
       		else
       			{
            	M_is_even=1;   // so it's ok, terminate "while" loop
       			}
            
 		 //} // End of while (!M_is_even) 	  
      	
 		 // Result	
 		 M_Opt_RX = M;
 		 
      		
      	if (M_Opt_RX > 0) 	
      		{	
			// Calculate maximum number of TCM information bits and maximum data rate
 	    	Optim2_error_RX	 =	TCM_bitcalc(&D2_alloc_RX[0], Shaping_Flag_rx,&Bits_in_SC_RX[0], C_RX, Index_K_RX, M_Opt_RX,0);
      		} else {
 	    	Optim2_error_RX = 0;
      		TCM_Num_Inf_Bits = 0;
      		}
      		
 	    // Save intermediate results of TCM_bitcalc in Optim2...RX global variables	
 	  		Optim2_inf_bit_RX	= TCM_Num_Inf_Bits;
			Optim2_rate_RX 		= (float)TCM_Num_Inf_Bits / T_OFDM_symbol; 
			Optim2_M_RX = M; 
 
    		  
		Optim2_RX++;
		
 		break;
		
		
 		case 2:
 		
 		// prepared for splitting in analog loop
 		// this case might be deleted, if not needed
 		
 		Optim2_RX = 0;
		asm("nop;");
		rx_state_finished=1;
		
		break;
		}
	}

