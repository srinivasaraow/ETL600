/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Optim3.c $
 * COPYRIGHT   : 2003/2004 TL Consulting GmbH
 * DESCRIPTION : Discrete optimization (bit shift) 
 *                to achieve target number of inf bits 							
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source AES/P4LT/OFDM_DSP/C/OFDM_Optim3.c $
 * 
 * 6     6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 5     21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 6     2.12.04 17:54 Chhamaa
 * 
 * 4     28.09.04 17:45 Maag01
 * 	20.07.04 PL implemented check for delta_bits_xx<0
 *				corrected error for very small data rates in algorithm
 * Initial version
 *       05.05.2004 PL (transfer in own function)
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
#include "../h/rs_fec/RS_externs.h"


//========================================
void segment ( "seg_ext_pmco" ) OFDM_Optim3()
	{
// **********************************************************************************
//        3. TCM-Optimization to required number of information bits				*							*
//                      - discrete bit shift algorithm 								*
//                         to achieve required number of information bits			*
//						  															*													*
// **********************************************************************************
				
	// Input: 	TCM_optim_inf_bit_TX, TCM_optim_inf_bit_RX (Sollwert)
	//			K_Sort_2_TX,K_Sort_2_RX
	//			Psi_TX , Psi_RX          
    //       
	//
	// Output: 	M_Opt_TX, M_Opt_RX
	//			Bits_in_SC_TX , Bits_in_SC_RX
	
	int TCM_bitcalc(int *, int, int *, float *, int *, int,int);
	

	
	int TCM_Error;
	int i,l_count;
	
	int not_enough, still_to_much;
	float tmpd, temp_SNR;
	
	int *Fl_Array_Int;
	
	
	switch ( Optim3_RX )
   		{
	   	case 0:
		// TX
		
		// copy Bit allocation into internal memory for processing and define an int pointer
		vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_TX[0],N);
		Fl_Array_Int=(int *)&Fl_Array_R[0];
		
		if (M_Opt_TX>0)
			{
 		 	// Calculate maximum number of TCM information bits and ngr..
// 	    	Optim3_error_TX	 =	TCM_bitcalc(&D2_alloc_TX[0], Shaping_Flag_tx,&Bits_in_SC_TX[0], C_TX, Index_K_TX, M_Opt_TX,0);
 	    	Optim3_error_TX	 =	TCM_bitcalc(&D2_alloc_TX[0], Shaping_Flag_tx,Fl_Array_Int, C_TX, Index_K_TX, M_Opt_TX,0);
			}
			else
			{
				ngr4=0;
				TCM_Num_Inf_Bits= - CRC_LEN - TERM_BITS;
				Optim3_error_TX=0;
			}
 	    	// Sollwert: TCM_optim_sr_inf_bit_TX
			// Istwert:  TCM_Num_Inf_Bits
 			Delta_bits_tx = TCM_Num_Inf_Bits - TCM_optim_sr_inf_bit_TX;
 			
			
 			// debug check
 			if ( (ngr4 & 1) == 1 ){Error.Count++;} //ngr4 muss gerade sein
 			not_enough=0;
 			still_to_much=0;
 			
 			if (Delta_bits_tx < 0)
 				{ // there are not enough information bits
 				not_enough=1;
 				if (not_enough)  // QAM4 Shift
 					{
 					for (opti=ngr4;opti<M_Opt2_max_TX;opti+=2)
 						{
 						//Bits_in_SC_TX[opti  ]+=2;
            			//Bits_in_SC_TX[opti+1]+=2;
            			Fl_Array_Int[opti  ]+=2;
            			Fl_Array_Int[opti+1]+=2;
            			M_Opt_TX += 2;
            			ngr4 += 2;
            			Delta_bits_tx += 3;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}
 					
 				if (not_enough)  // QAM16 Shift
 					{
 					for (opti=ngr16;opti<ngr4;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_tx += 1;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}	
 				
 				if (not_enough)  // QAM64 Shift
 					{
 					for (opti=ngr64;opti<ngr16;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_tx += 2;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}		
 					
 				if (not_enough)  // QAM256 Shift
 					{
 					for (opti=ngr256;opti<ngr64;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_tx+=2;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}	
 				
 				if (not_enough)  // QAM1024 Shift
 					{
 					for (opti=ngr1024;opti<ngr256;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_tx+=2;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}		
 				
 				if (not_enough)  // QAM4096 Shift
 					{
 					for (opti=ngr4096;opti<ngr1024;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_tx+=2;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}	
 				
 				if (not_enough)  // QAM16384 Shift
 					{
 					for (opti=ngr16384;opti<ngr4096;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_tx+=2;
           				 if (Delta_bits_tx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}		
 						
 				} // if(delta_bits <..)
 				
 			// Calculate maximum number of TCM information bits and ngr..
 	    	Optim3_error_TX	 =	TCM_bitcalc(&D2_alloc_TX[0], Shaping_Flag_tx,&Fl_Array_Int[0], C_TX, Index_K_TX, M_Opt_TX,0);
			Optim3_inf_bit_TX = TCM_Num_Inf_Bits;
 	    	//Optim3_rate_TX = (float)TCM_Num_Inf_Bits / T_OFDM_symbol;  
 	    	// Sollwert: TCM_optim_sr_inf_bit_TX
			// Istwert:  TCM_Num_Inf_Bits
 			Delta_bits_tx = TCM_Num_Inf_Bits - TCM_optim_sr_inf_bit_TX;
 			
 	 				
 			if (Delta_bits_tx > 0)
 				{ // there are to many information bits
 				still_to_much=1;
 				
 				if (still_to_much)  //QAM16384
 					{
 					for (opti=ngr16384-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_tx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_tx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM4096
 					{
 					for (opti=ngr4096-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_tx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_tx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM1024
 					{
 					for (opti=ngr1024-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_tx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_tx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM256
 					{
 					for (opti=ngr256-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_tx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_tx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 				
 				if (still_to_much)  //QAM64
 					{
 					for (opti=ngr64-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_tx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_tx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 						
 				if (still_to_much)  //QAM16
 					{
 					for (opti=ngr16-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 1 ?
 						if(Delta_bits_tx<1)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_tx--;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<1)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM4
 					{
 					for (opti=ngr4-1;opti>=0;opti-=2)
 						{
 						// is it possible to reduce by 3 ?
 						if(Delta_bits_tx<3)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti  ]-=2;
 							Fl_Array_Int[opti-1]-=2;
 							Delta_bits_tx-=3;
 							M_Opt_TX -= 2;
 							// test Delta_bits_tx again
 							if(Delta_bits_tx<3)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}	
 				
 				} // if(delta_bits >..)	
 				
 			// Calculate maximum number of TCM information bits and ngr..
 	    	Optim3_error_TX	 =	TCM_bitcalc(&D2_alloc_TX[0], Shaping_Flag_tx,&Fl_Array_Int[0], C_TX, Index_K_TX, M_Opt_TX,0);
			Optim3_inf_bit_TX = TCM_Num_Inf_Bits;
 	    	//Optim3_rate_TX = (float)TCM_Num_Inf_Bits / T_OFDM_symbol;  
 	    	// Sollwert: TCM_optim_sr_inf_bit_TX
			// Istwert:  TCM_Num_Inf_Bits
 			Delta_bits_tx = TCM_Num_Inf_Bits - TCM_optim_sr_inf_bit_TX;
 			
 			// Save result into external memory (result is in Fl_Array_Int, which points to Fl_Array_R[0])
 			vcopy_ED((float *)&Bits_in_SC_TX[0],Fl_Array_R,N);
		
 			
      		Optim3_RX++;
		
 		break;	
		
 		
 		case 1:
		// RX
		
			// copy Bit allocation into internal memory for processing and define an int pointer
			vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_RX[0],N);
			Fl_Array_Int=(int *)&Fl_Array_R[0];
		
 			
 			if (M_Opt_RX>0)
 			{
 			// Calculate maximum number of TCM information bits and ngr..
 	    	//Optim3_error_RX	 =	TCM_bitcalc(&D2_alloc_RX[0], Shaping_Flag_rx,&Bits_in_SC_RX[0], C_RX, Index_K_RX, M_Opt_RX);
 	    	Optim3_error_RX	 =	TCM_bitcalc(&D2_alloc_RX[0], Shaping_Flag_rx,Fl_Array_Int, C_RX, Index_K_RX, M_Opt_RX,0);
 			}
 			else
 				{
				ngr4=0;
				TCM_Num_Inf_Bits= - CRC_LEN - TERM_BITS;
				}
 	    	// Sollwert: TCM_optim_sr_inf_bit_RX
			// Istwert:  TCM_Num_Inf_Bits
 			Delta_bits_rx = TCM_Num_Inf_Bits - TCM_optim_sr_inf_bit_RX;
			

 			// debug check
 			if ( (ngr4 & 1) == 1 ){Error.Count++;} //ngr4 muss gerade sein
 			not_enough=0;
 			still_to_much=0;
 			
 			if (Delta_bits_rx < 0)
 				{ // there are not enough information bits
 				not_enough=1;
 				if (not_enough)  // QAM4 Shift
 					{
 					for (opti=ngr4;opti<M_Opt2_max_RX;opti+=2)
 						{
 						//Bits_in_SC_RX[opti  ]+=2;
            			//Bits_in_SC_RX[opti+1]+=2;
            			Fl_Array_Int[opti  ]+=2;
            			Fl_Array_Int[opti+1]+=2;
            			M_Opt_RX += 2;
            			ngr4 += 2;
            			Delta_bits_rx += 3;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}
 					
 				if (not_enough)  // QAM16 Shift
 					{
 					for (opti=ngr16;opti<ngr4;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_rx += 1;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}	
 				
 				if (not_enough)  // QAM64 Shift
 					{
 					for (opti=ngr64;opti<ngr16;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_rx += 2;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}		
 					
 				if (not_enough)  // QAM256 Shift
 					{
 					for (opti=ngr256;opti<ngr64;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_rx += 2;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}	
 				
 				if (not_enough)  // QAM1024 Shift
 					{
 					for (opti=ngr1024;opti<ngr256;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_rx+=2;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}		
 				
 				if (not_enough)  // QAM4096 Shift
 					{
 					for (opti=ngr4096;opti<ngr1024;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_rx+=2;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}	
 				
 				if (not_enough)  // QAM16384 Shift
 					{
 					for (opti=ngr16384;opti<ngr4096;opti++)
 						{
 						Fl_Array_Int[opti]+=2;
            			Delta_bits_rx+=2;
           				 if (Delta_bits_rx >= 0)
            			   {
           				   not_enough=0;
           				   break;
           				   }

 						}
 					}		
 						
 				} // if(delta_bits <..)
 				
 			// Calculate maximum number of TCM information bits and ngr..
 	    	Optim3_error_RX	 =	TCM_bitcalc(&D2_alloc_RX[0], Shaping_Flag_rx,&Fl_Array_Int[0], C_RX, Index_K_RX, M_Opt_RX,0);
			Optim3_inf_bit_RX = TCM_Num_Inf_Bits;
 	    	//Optim3_rate_RX = (float)TCM_Num_Inf_Bits / T_OFDM_symbol;  
 	    	// Sollwert: TCM_optim_sr_inf_bit_RX
			// Istwert:  TCM_Num_Inf_Bits
 			Delta_bits_rx = TCM_Num_Inf_Bits - TCM_optim_sr_inf_bit_RX;
 			
 			
 				
 			if (Delta_bits_rx > 0)
 				{ // there are to many information bits
 				still_to_much=1;
 				
 				if (still_to_much)  //QAM16384
 					{
 					for (opti=ngr16384-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_rx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_rx-=2;
 							// test Delta_bits_rx again
 							if(Delta_bits_rx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM4096
 					{
 					for (opti=ngr4096-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_rx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_rx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_rx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM1024
 					{
 					for (opti=ngr1024-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_rx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_rx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_rx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM256
 					{
 					for (opti=ngr256-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_rx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_rx-=2;
 							// test Delta_bits_tx again
 							if(Delta_bits_rx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 				
 				if (still_to_much)  //QAM64
 					{
 					for (opti=ngr64-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 2 ?
 						if(Delta_bits_rx<2)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_rx-=2;
 							// test Delta_bits_rx again
 							if(Delta_bits_rx<2)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 						
 				if (still_to_much)  //QAM16
 					{
 					for (opti=ngr16-1;opti>=0;opti--)
 						{
 						// is it possible to reduce by 1 ?
 						if(Delta_bits_rx<1)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti]-=2;
 							Delta_bits_rx--;
 							// test Delta_bits_rx again
 							if(Delta_bits_rx<1)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}
 					
 				if (still_to_much)  //QAM4
 					{
 					for (opti=ngr4-1;opti>=0;opti-=2)
 						{
 						// is it possible to reduce by 3 ?
 						if(Delta_bits_rx<3)
 							{
 							// no, it isn't
 							still_to_much=0;
 							break;
 							}
 						else
 							{
 							// yes, I can reduce
 							Fl_Array_Int[opti  ]-=2;
 							Fl_Array_Int[opti-1]-=2;
 							Delta_bits_rx-=3;
 							M_Opt_RX-=2;
 							// test Delta_bits_rx again
 							if(Delta_bits_rx<3)
 								{
 								still_to_much=0;
 								break;
 								}
 							}
 						}
 					}	
 				
 				} // if(delta_bits >..)	
 				
 			// Calculate maximum number of TCM information bits and ngr..
 	    	Optim3_error_RX	 =	TCM_bitcalc(&D2_alloc_RX[0], Shaping_Flag_rx,&Fl_Array_Int[0], C_RX, Index_K_RX, M_Opt_RX,0);
			Optim3_inf_bit_RX = TCM_Num_Inf_Bits;
 	    	//Optim3_rate_RX = (float)TCM_Num_Inf_Bits / T_OFDM_symbol;
 	    	// Sollwert: TCM_optim_sr_inf_bit_RX
			// Istwert:  TCM_Num_Inf_Bits
 			Delta_bits_rx = TCM_Num_Inf_Bits - TCM_optim_sr_inf_bit_RX;
 			
 			// Save result into external memory (result is in Fl_Array_Int, which points to Fl_Array_R[0])
 			vcopy_ED((float *)&Bits_in_SC_RX[0],Fl_Array_R,N);
		
 			
      	Optim3_RX++;

		
		break;
		
		case 2:
		
		// Calculation of RS redundancy to check Optimization result
		if (RS_Coding_Flag) 
			{
			RS_redund_bits_Tx = RS_optim_sr_code_bit_TX - RS_optim_sr_inf_bit_TX;
			RS_redund_bits_Rx = RS_optim_sr_code_bit_RX - RS_optim_sr_inf_bit_RX;
			}
			else
			{ RS_redund_bits_Tx = 0; RS_redund_bits_Rx = 0; }

		
		// check results
		// Note: Bit Field contains
		// | Delta_bits | Number_of_Service_bits | User_inf_bits (including stuffing bit) | Stuffing_Flag |
		// |                 <-  Optim3_inf_bit_XX   ->											 |
		
		if ((Optim3_inf_bit_TX != Delta_bits_tx + RS_redund_bits_Tx + Number_of_Service_bits + User_inf_bits_TX + Stuffing_Flag_Bit )
			| (Delta_bits_tx<0))
			{
			Optim3_error_TX = -2; // Report Error in Bit Table	
			}
		
		if ((Optim3_inf_bit_RX != Delta_bits_rx + RS_redund_bits_Rx + Number_of_Service_bits + User_inf_bits_RX + Stuffing_Flag_Bit )
		 	| (Delta_bits_rx<0))	
			{
			Optim3_error_RX = -2; // Report Error in Bit Table	
			}

		if ((User_inf_bits_RX<5)|(User_inf_bits_TX<5))	
			{
			Optim3_error_RX = -3; // Report Error in Bit Table	
			}

	
		// ##################################################################
		// Summerize Results of Optimization for Status Data
		// ##################################################################
	
		// **** signal power to noise power (SNR) in each bin
			tmpd=1.0f/(AWGN_Sigma_Est_RX*AWGN_Sigma_Est_RX);  // =1/sigma^2
			SNR_mean_dB = 0.0f;
			l_count = 0;
			for(i=0;i<N;i++)
				{
				opti=Index_K_RX[i];
				if (K_Sort_2_RX[i] > 0.0f)
					{
					temp_SNR = 10.0f*log10f(K_Sort_2_RX[i]*tmpd);
					l_count ++;
					SNR_mean_dB += temp_SNR;	
					}
				else temp_SNR = 0.0f;
 
				SNR_bin_RX[opti] = temp_SNR; //= k^2 / sigma^2
				}
			SNR_mean_dB = SNR_mean_dB / (float) l_count;
			
			tmpd=1.0f/(AWGN_Sigma_Est_TX*AWGN_Sigma_Est_TX);  // =1/sigma^2
			for(i=0;i<N;i++)
				{
				opti=Index_K_TX[i];
				SNR_bin_TX[opti] = 10.0f*log10f(K_Sort_2_TX[i]*tmpd); //= k^2 / sigma^2
				}
				
				
 		// **** Bit Distribution in physical order
      	 	vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_TX[0],N);
      		vcopy_DE(&Fl_Array_R[N],(float *)&Index_K_TX[0],N);
			vscatter_i_DDD((int *)&Fl_Array_R[N2],(int *)&Fl_Array_R[0],(int *)&Fl_Array_R[N],N);
			vcopy_ED((float *)&Bits_bin_TX[0],&Fl_Array_R[N2],N);
      		
			vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_RX[0],N);
      		vcopy_DE(&Fl_Array_R[N],(float *)&Index_K_RX[0],N);
			vscatter_i_DDD((int *)&Fl_Array_R[N2],(int *)&Fl_Array_R[0],(int *)&Fl_Array_R[N],N);
			vcopy_ED((float *)&Bits_bin_RX[0],&Fl_Array_R[N2],N);
      		
		// ##################################################################
			
			
 		
		Optim3_RX = 0;
		
		rx_state_finished=1;
		
		break;
		}
	}

