/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Optim1.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Preliminary optimization
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Optim1.c $
 * 
 * 12    24.07.08 15:32 Chhamaa
 * 
 * 9     6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 8     21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 6     17.03.06 14:41 Chhamaa
 * 
 * 4     2.05.05 12:40 Ats
 * Exchange with TLC : OFDM V0_09S
 * 
 * 3     24.02.05 7:26 Chchlee
 * 
 * 2     1.02.05 13:14 Chalgyg
 * Version 0.09A
 * 
 * 4     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * SDRAM memory functions
 *       02.03.04 vgp
 * Re-Design of Optimization algorithms
 *       20.04.04 PL 	
 * Implemented Predistortion calculation
 *  	 03.05.04 PL	
 * Resolved timing error
 *		 16.05.04 PL
 * Implemented RS Coding consideration in bitrate
 *		 28.07.04 PL
 * Threshhold correction for RS Coding 
 *		 15.02.05 vgp
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

int RS_Encoder_Init(int );
int RS_Decoder_Init(int );
int RS_Encoder_Init_cb(int );
int RS_Decoder_Init_cb(int );

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Optim1()
	{
// **********************************************************************************
//        1. TCM-Optimization 														*
//                      - maximize Data Rate at given Power constraints				*
//						  															*													*
// **********************************************************************************

	int TCM_bitcalc(int *, int, int *, float *, int *, int, int);
	int OFDM_opt_decide_rate(int *User_inf_bit_TX, int *User_inf_bit_RX, int Datarate_Mode);

/* 	static 	float segment ( "seg_extn SHT_NOBITS" )q_help, P_help;
	static	float segment ( "seg_extn SHT_NOBITS" ) qqq;
	static 	int   segment ( "seg_extn SHT_NOBITS" )qqi;
*/
	static 	float q_help, P_help;
	static	float qqq;
	static 	int   qqi;


	int i, ref_pilot;
	int M , M_is_even;
	float Temp, a_rx, b_tx;
	
	float P_i;
	

	//int *ptr;
	
	//debug
	//float Sig_o, tmpd;
	
	switch ( Optim1_RX )
   		{
	   	case 0:
	   	
	   	// **************************************************************************
      	//         Check Master Slave Settings		OFDM_MASTER  OFDM_SLAVE         1
      	// **************************************************************************
		// OFDM_Modem_type on other side is stored in "Channel_Data_TX.Settings[1]"
		// OFDM_Modem_type on local side is stored in OFDM_Modem_Type
      	// if both are configured as Master then report Error
      	if ((Channel_Data_TX.Settings[1]==OFDM_MASTER)&&(OFDM_Modem_Type==OFDM_MASTER))
      		{
      			Error.Count++;
       			Error.Number[Error.Count].Location = ERLOC_CONFIG;
       			Error.Number[Error.Count].ID       = WRNG_MASTER_MASTER_CNFG;
       			//dbg_error[Error.Count]=WRNG_MASTER_MASTER_CNFG;	
      		}

		Optim1_error_RX = 0;
		Optim1_error_TX = 0;
      	// **************************************************************************
      	//         Extract required values out of Channel_Data Field				*
      	//                           RX      										*
      	// **************************************************************************
		
		// Fetch quantized values of Channel_Data field to internal memory
		vcopy_DE(Fl_Array_R,(float *)&Channel_Data_RX.K_int[0],N);

		// Convert quantized values to float
		vfloat_DD(Fl_Array_R,(int *)&Fl_Array_R[0],-K_WORD_L,N);
		
		// Fetch Skaling Value from Channel_Data_field
		Temp = Channel_Data_RX.K_scale;
		
		// Skale Channel Data
		vsmul_f_DD(Fl_Array_R,Temp,Fl_Array_R,N);
		
		// Unsorted magnitudes of channel transfer function (RX part)
		vcopy_ED(K_Abs_RX,Fl_Array_R,N);
		
		// Set Pilots to zero (for sorted fields in optimization)
		Fl_Array_R[Pilot_ind0]=0.0f;  
		Fl_Array_R[Pilot_ind1]=0.0f;  

		// Sorting of gain values
		vcopy_DE(&Fl_Array_R[N],(float *)Index_K_RX,N);

		vsortii_f_DDDD( &Fl_Array_R[N2], (int *)&Fl_Array_R[N], Fl_Array_R, (int *)&Fl_Array_R[N], N );
		
		// Squared value of channel gain
		vmul_f_DDD(&Fl_Array_R[N2+N],&Fl_Array_R[N2],&Fl_Array_R[N2],N);
		
		vcopy_ED((float *)Index_K_RX,&Fl_Array_R[N],N);
		vcopy_ED(K_Sort_2_RX,&Fl_Array_R[N2+N],N);

		// Fetch noise estimation Value from Channel_Data_field
		AWGN_Sigma_Est_RX = Channel_Data_RX.AWGN_sigma;
		if (AWGN_Sigma_Est_RX < 1.0e-10f){AWGN_Sigma_Est_RX=1.0e-10f;}

		
		Optim1_RX++;

		break;
      	
      	case 1:
		// **************************************************************************
      	//         Extract required values out of Channel_Data Field				*
      	//                           TX      										*
      	// **************************************************************************
		
		// Fetch quantized values of Channel_Data field to internal memory
		vcopy_DE(Fl_Array_R,(float *)&Channel_Data_TX.K_int[0],N);

		// Convert quantized values to float
		vfloat_DD(Fl_Array_R,(int *)&Fl_Array_R[0],-K_WORD_L,N);
		
		// Fetch Skaling Value from Channel_Data_field
		Temp = Channel_Data_TX.K_scale;
		
		// Skale Channel Data
		vsmul_f_DD(Fl_Array_R,Temp,Fl_Array_R,N);
		
		// Unsorted magnitudes of channel transfer function (TX part)
		vcopy_ED(K_Abs_TX,Fl_Array_R,N);

		// Set Pilots to zero (for sorted fields in optimization)
		Fl_Array_R[Pilot_ind0]=0.0f;  
		Fl_Array_R[Pilot_ind1]=0.0f;  
		
		// Sorting of gain values
		vcopy_DE(&Fl_Array_R[N],(float *)Index_K_TX,N);

		vsortii_f_DDDD( &Fl_Array_R[N2], (int *)&Fl_Array_R[N], Fl_Array_R, (int *)&Fl_Array_R[N], N );
		
		// Squared value of channel gain
		vmul_f_DDD(&Fl_Array_R[N2+N],&Fl_Array_R[N2],&Fl_Array_R[N2],N);
		
		vcopy_ED((float *)Index_K_TX,&Fl_Array_R[N],N);
		vcopy_ED(K_Sort_2_TX,&Fl_Array_R[N2+N],N);

		// Fetch noise estimation Value from Channel_Data_field
		AWGN_Sigma_Est_TX = Channel_Data_TX.AWGN_sigma;
		if (AWGN_Sigma_Est_TX < 1.0e-10f){AWGN_Sigma_Est_TX=1.0e-10f;}
		
		Optim1_RX++;

		break;
      	
      	case 2:	


		// **************************************************************************
      	//         Prepare predistortion for 										*
      	//                           TX and RX 										*
      	//                     Predistortion values are scaled to					*
      	// 						local Reference Pilot Pilot_ind1 RX-side			*
      	//						far end Reference Pilot ref_pilot for local TX-side	*
      	// **************************************************************************
    	
      	// Decide Reference Pilot (local TX side) 
		if (K_Abs_TX[Pilot_ind1] > K_Abs_TX[Pilot_ind0])
			{ ref_pilot = Pilot_ind1;}
			else
			{ ref_pilot = Pilot_ind0;}
		b_tx = K_Abs_TX[ref_pilot];
		
		a_rx = K_Abs_RX[Pilot_ind1];
		vcopy_DE(Fl_Array_R,K_Abs_TX,N);
		vcopy_DE(&Fl_Array_R[N],K_Abs_RX,N);
		

		for(i=0;i<N;i++)
    		{
    		if(Fl_Array_R[i] == 0.0f)
    			C_TX[i] = 1.0f;
    		else
    			C_TX[i] = b_tx/Fl_Array_R[i]*C_TX[i];
    	   							
    		if(Fl_Array_R[N+i] == 0.0f)
    			C_RX[i] = 1.0f;
    		else
    			C_RX[i] = a_rx/Fl_Array_R[N+i];
    			}
    			  					      
		// **************************************************************************
      	//         Preparation of some additional usefull Arrays out of K(i)		*
      	//                      TX  and   RX 										*
      	// **************************************************************************
		
      	// Cumulative sum of inverse squared values of channel gain
		M_Opt_max_RX =1;
		M_Opt_max_TX =1;
		
		// F(i)
		vcopy_DE(Fl_Array_R,K_Sort_2_TX,N);
		vcopy_DE(&Fl_Array_R[N],K_Sort_2_RX,N);
		Temp = 1.0f/(float)N;

		Fl_Array_R[N2]=Temp/Fl_Array_R[0];
		Fl_Array_R[N2+N]=Temp/Fl_Array_R[N];
		for(i=1;i<N;i++)
			{
			// F_TX
			if (Fl_Array_R[i] > 0.0f )
				{
				Fl_Array_R[N2+i]=Fl_Array_R[N2+i-1]+Temp/Fl_Array_R[i];
				M_Opt_max_TX++;
				}
			// F_RX
			if (Fl_Array_R[N+i] > 0.0f )
				{
				Fl_Array_R[N2+N+i]=Fl_Array_R[N2+N+i-1]+Temp/Fl_Array_R[N+i];
				M_Opt_max_RX++;
				}
			}

		vcopy_ED(F_TX,&Fl_Array_R[N2],N);
		vcopy_ED(F_RX,&Fl_Array_R[N2+N],N);

		// Psi(i)
		Fl_Array_R[N2]=half_log2 * log10f(Fl_Array_R[0]);
		Fl_Array_R[N2+N]=half_log2 * log10f(Fl_Array_R[N]);
		for(i=1;i<N;i++)
			{
			if (i<M_Opt_max_TX)
				{
				Fl_Array_R[N2+i]=((float)i*Fl_Array_R[N2+i-1]+half_log2*log10f(Fl_Array_R[i]))/(float)(i+1);
				}
			if (i<M_Opt_max_RX)
				{
				Fl_Array_R[N2+N+i]=((float)i*Fl_Array_R[N2+N+i-1]+half_log2*log10f(Fl_Array_R[N+i]))/(float)(i+1);
				}
			}
		
		vcopy_ED(Psi_TX,&Fl_Array_R[N2],N);
		vcopy_ED(Psi_RX,&Fl_Array_R[N2+N],N);

		
		Optim1_RX++;

		break;
      	
      	case 3:	

      	// ******************************************************************************
      	//         Define sensitivity of Optimization									*
      	//                                  ( results in Bit error probability)			*
      	// ******************************************************************************
      		// Definition: delta^2 = K_Prob_Err * sigma^2
      		// delta/sigma = 4.0
			if (RS_Coding_Flag) {
      		   K_Prob_Err = (5.0f*5.0f)*powf(10.0f,-0.1f*SHAPING_GAIN); 
			} else {
      		   K_Prob_Err = (5.0f*5.0f)*powf(10.0f,-0.1f*SHAPING_GAIN); 
			}
      		// substitution: delta_6 = delta^2 / 6
      		Delta_6_rx    = K_Prob_Err*AWGN_Sigma_Est_RX*AWGN_Sigma_Est_RX/6.0f;
      		Delta_6_tx    = K_Prob_Err*AWGN_Sigma_Est_TX*AWGN_Sigma_Est_TX/6.0f;
      		
      		
      	// ******************************************************************************
      	//         Search for M_Opt, that is maximum number of usefull subchannels		*
      	//                                  (depends on measured channel K(i), sigma^2	*
      	//										and given maximum error probability)	*
      	// ******************************************************************************
	
      	// - TX -----------------------------------------------------------
			M_Opt_TX = M_Opt_max_TX;
			
			// calculate Power in subchannel i (P_i)
			// as long as P_i >0 increase number of subchannels (M_Opt)
			
			vcopy_DE(Fl_Array_R,K_Sort_2_TX,N);
			vcopy_DE(&Fl_Array_R[N],F_TX,N);
			
			for(i=0;i<M_Opt_max_TX;i++)
				{
				P_i = (float)N/((float)(i+1)) * (P_OFDM_TX + Delta_6_tx*Fl_Array_R[N+i]) - Delta_6_tx/Fl_Array_R[i];
				if(P_i < TH_OPTIM1_PMIN)  //0.0 replaced by 0.5, that is minimum power for QAM4
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
   		   			
      	// - RX -----------------------------------------------------------
			M_Opt_RX = M_Opt_max_RX;
			
			// calculate Power in subchannel i (P_i)
			// as long as P_i >0 increase number of subchannels (M_Opt)
			
			vcopy_DE(Fl_Array_R,K_Sort_2_RX,N);
			vcopy_DE(&Fl_Array_R[N],F_RX,N);
			
			for(i=0;i<M_Opt_max_RX;i++)
				{
				P_i = (float)N/((float)(i+1)) * (P_OFDM_TX + Delta_6_rx*Fl_Array_R[N+i]) - Delta_6_rx/Fl_Array_R[i];
				if(P_i < TH_OPTIM1_PMIN)  //0.0 replaced by 0.5, that is minimum power for QAM4
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
   		   			
   		Optim1_RX++;

		break;
      	
      	case 4:	
       	// **********************************************************************************
      	//         Calculate Bit Distribution												*
      	//					(Maximization of data rate under restriction of limited power)	*
        //				TX   and   RX														*
      	// **********************************************************************************
  		   	
      		// **********************************************************************************			
      		// *     Theorie and Formulas:
      		// *     With known number of useful subchannels M, we can calculate
      		// *     optimum Power distribution to subchannels i
      		// *     P(i) = N/M*[P_OFDM_TX + sigma^2/Kprob*F(M)] - sigma^2/Kprob *1/|ki|^2 	(eq.1)
      		// *
      		// *     The first term is independent of index i, so it can be substituted by
      		// *     P_i_sum = N/M*[P_OFDM_TX + sigma^2/Kprob*F(M)]
      		// * 
      		// *     The number of bits in SC i is derived from
      		// *     Q(i) = log2[ 1 + Kprob/sigma^2*P(i)*|ki|^2]							(eq.2)
      		// *            must be a even number, therefore we first calculate float
      		// * 			Q_2(i) = 1/2*log2[...] and then produce integer by
      		// *            Q(i) = 2* ( int[ Q_2(i) +0.5] ) 	
      		// *
      		// *     Now we insert P(i) (eq.1) into calculation of Q(i) (eq.2) and combine all
      		// *            terms, that are independet of index i in variabel "P_help" which is
      		// *			calculated prior to loop execution
      		// *     Note: delta_6 = sigma^2 / Kprob
        	// **********************************************************************************

     	// - TX -----------------------------------------------------------
    		
         // delete Bit Allocation	
         vset_f_E((float *)&Bits_in_SC_TX,0.0f,sizeof(Bits_in_SC_TX));
		 Optim1_error_TX = 1000;
       	 M_is_even = 0;

       	 P_help = ((float)N/(float)M_Opt_TX)/Delta_6_tx*(P_OFDM_TX + Delta_6_tx*F_TX[M_Opt_TX-1]);
    	 M=0;
      		
      		for (opti=0;opti<M_Opt_TX;opti++)
      				{
      				q_help = K_Sort_2_TX[opti]*P_help;
//					Bits_in_SC_TX[opti] = 2*((int)(half_log2*log10f(q_help)+TH_OPTIM1_DISCR));  // Kompromiss       				
      				qqq =  log10f(q_help);
					qqq = qqq * half_log2;
					qqq = qqq + TH_OPTIM1_DISCR;
					qqi = 2* (int) qqq;
				
      				if (qqi > MAXBITS_IN_SC)
         				{
         				qqi=MAXBITS_IN_SC;
         				}
         			if (qqi > 0) M++;
         				
         			Bits_in_SC_TX[opti]=qqi;		
         			}
			
      		// Force remaining values to zero
   			for (opti=M_Opt_TX;opti<M_Opt_max_TX;opti++)
      				{
      				Bits_in_SC_TX[opti]=0;
      				}	
 
      		// Check if number of active carriers M is even
           	if ( (M & 1) == 1 ) 
         		{
         		// if not, reduce number of channels by one
         		M--;
         		Bits_in_SC_TX[M]=0;
         		}
       		else
       			{
            	M_is_even=1;   // so it's ok, terminate "while" loop
       			}
            
      	
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
      		
      	// Calculate maximum number of TCM information bits and maximum data rate
      	//     and calculate power of signal constellation (predistortion is considered)
   		vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_TX[0],N);
   		vcopy_DE(&Fl_Array_R[N],(float *)&Index_K_TX[0],N);

   		Optim1_error_TX=TCM_bitcalc((int *)&Fl_Array_R[N2],Shaping_Flag_tx,(int *)Fl_Array_R,C_TX,(int *)&Fl_Array_R[N],M_Opt_TX,0);
		
   		// if no error, store results
   		if (Optim1_error_TX==0)
   			{
   			TCM_optim_inf_bit_TX = TCM_Num_Inf_Bits;
			if (M_Opt_TX>0)
 				{
				vcopy_ED((float *)&D2_alloc_TX[0],&Fl_Array_R[N2],M_Opt_TX);
 				}
 		 	// Save intermediate results of TCM_bitcalc in Optim1_...TX global variables	
 	    	Optim1_inf_bit_TX    = TCM_Num_Inf_Bits;
	    	Optim1_rate_TX		 = (float)TCM_Num_Inf_Bits / T_OFDM_symbol;
	    	Optim1_M_TX = M_Opt_TX; 
   			} else
   			{
   			TCM_optim_inf_bit_TX = 0;
			Optim1_inf_bit_TX    = 0;
	    	Optim1_rate_TX		 = 0.0f;
	    	Optim1_M_TX = 0; 
   			}
   				
		Optim1_RX++;

		break;
      	
      	case 5:		
	// - RX -----------------------------------------------------------
    	
	
         // delete Bit Allocation	
         vset_f_E((float *)&Bits_in_SC_RX,0.0f,sizeof(Bits_in_SC_RX));
		 Optim1_error_RX = 1000;
       	 M_is_even = 0;

       	 P_help = ((float)N/(float)M_Opt_RX)/Delta_6_rx*(P_OFDM_TX + Delta_6_rx*F_RX[M_Opt_RX-1]);
      	 M=0;
      		
      		for (opti=0;opti<M_Opt_RX;opti++)
      				{
      				q_help = K_Sort_2_RX[opti]*P_help;
//					Bits_in_SC_RX[opti] = 2*((int)(half_log2*log10f(q_help)+TH_OPTIM1_DISCR));  // Kompromiss       				
      				qqq =  log10f(q_help);
					qqq = qqq * half_log2;
					qqq = qqq + TH_OPTIM1_DISCR;
					qqi = 2* (int) qqq;
				
      				if (qqi > MAXBITS_IN_SC)
         				{
         				qqi=MAXBITS_IN_SC;
         				}
         			if (qqi > 0) M++;
         				
         			Bits_in_SC_RX[opti]=qqi;
       				}
			
      		// Force remaining values to zero
      		
   			for (opti=M_Opt_RX;opti<M_Opt_max_RX;opti++)
      				{
      				Bits_in_SC_RX[opti]=0;
      				}	
 
      		// Check if number of active carriers M is even
           	if ( (M & 1) == 1 ) 
         		{
         		// if not, reduce number of channels by one
         		M--;
         		Bits_in_SC_RX[M]=0;
         		}
       		else
       			{
            	M_is_even=1;   // so it's ok, terminate "while" loop
       			}
            
      	
 		 // result	
 		 M_Opt_RX = M;
      		
      	      		
      	// Calculate maximum number of TCM information bits and maximum data rate
      	//     and calculate power of signal constellation (predistortion is considered)
   		vcopy_DE(Fl_Array_R,(float *)&Bits_in_SC_RX[0],N);
   		vcopy_DE(&Fl_Array_R[N],(float *)&Index_K_RX[0],N);

   		Optim1_error_RX=TCM_bitcalc((int *)&Fl_Array_R[N2],Shaping_Flag_tx,(int *)Fl_Array_R,C_RX,(int *)&Fl_Array_R[N],M_Opt_RX,0);
		
   		// if no error, store results
   		if (Optim1_error_RX==0)
   			{
   			TCM_optim_inf_bit_RX = TCM_Num_Inf_Bits;
			if (M_Opt_RX>0)
 				{
				vcopy_ED((float *)&D2_alloc_RX[0],&Fl_Array_R[N2],M_Opt_RX);
 				}
 		 	// Save intermediate results of TCM_bitcalc in Optim1_...RX global variables	
 	    	Optim1_inf_bit_RX    = TCM_Num_Inf_Bits;
	    	Optim1_rate_RX		 = (float)TCM_Num_Inf_Bits / T_OFDM_symbol;
	    	Optim1_M_RX = M_Opt_RX; 
   			} else
   			{
   			TCM_optim_inf_bit_RX = 0;
			Optim1_inf_bit_RX    = 0;
	    	Optim1_rate_RX		 = 0.0f;
	    	Optim1_M_RX = 0; 
   			}
		  
		Optim1_RX++;
		
 		break;
		
 		case 6:
 		
 		// ** If no error occured in previous optimization, then the number of infomration bits
 		//    TCM_optim_inf_bit_TX/RX is further reduced by RS-coding redundancy (optional), 
 		//    and Service Bits (optional) and a Stuffing bit. This yields the maximum possible
 		//    net bitrate on V.11 interface (Optim_inf_bit_RX is number of remaining inf bits).
 		// ** If an error occured Optim_inf_bit_TX/RX is set to zero. Then OFDM_opt_decide_rate() will
 		//    choose minimum data rate out of list and recalculate Optim_inf_bit_TX/RX 
 		if (Optim1_error_TX==0 && Optim1_error_RX==0)
 		{
 		
 		// **********************************************************************************
      	//         If RS coding then calculate number of information bits if RS code is applied
      	//                      - Calculate RS code parameters								*
      	//						- Calculate RS_optim_rate_XX 								*
        //																					*
      	// **********************************************************************************
 		
// RS_Coding_ON,     RS_optim_code_bit_TRX,  RS_optim_inf_bit_TRX, 	
      	
      	if (RS_Coding_Flag)
      		{
      		// TX path
      		// Number of RS code bits is equal to TCM inf bits		
      		RS_optim_code_bit_TX = TCM_optim_inf_bit_TX;
      		// Calculate RS inf bits   (result is in RS_global variabel RS_PCM_Nbit_Tx)
      		RS_Encoder_Init(RS_optim_code_bit_TX);
      		RS_optim_inf_bit_TX	= RS_PCM_Nbit_Tx;
      		
      		// RX path
      		// Number of RS code bits is equal to TCM inf bits	
      		RS_optim_code_bit_RX = TCM_optim_inf_bit_RX;
      		// Calculate RS inf bits   (result is in RS_global variabel RS_PCM_Nbit_Tx)
      		RS_Decoder_Init(RS_optim_code_bit_RX);
      		RS_optim_inf_bit_RX	= RS_PCM_Nbit_Rx;
      		
      		Optim_inf_bit_TX = RS_optim_inf_bit_TX;
      		Optim_inf_bit_RX = RS_optim_inf_bit_RX;
      		}
      	else // without RS Coding
      		{
      		Optim_inf_bit_TX = TCM_optim_inf_bit_TX;
      		Optim_inf_bit_RX = TCM_optim_inf_bit_RX;	
      		}
     // debug only, to remove 	
      //		Optim_inf_bit_TX = TCM_optim_inf_bit_TX;
      //		Optim_inf_bit_RX = TCM_optim_inf_bit_RX;
      		
      	// **********************************************************************************
      	//         If Service Channel is active, then										*
      	//                      - substract number of Service Bits per OFDM Symbol			*
        //																					*
      	// **********************************************************************************
		Optim_inf_bit_TX = Optim_inf_bit_TX - Number_of_Service_bits - 1; //-1 for Stuffing Flag
		Optim_inf_bit_RX = Optim_inf_bit_RX - Number_of_Service_bits - 1; //-1 for Stuffing Flag
		
 		} // end of if (Optim1_error_TX==0 && Optim1_error_RX==0)
 		else
 		// ** If an error occured Optim_inf_bit_TX/RX is set to zero. Then OFDM_opt_decide_rate() will
 		//    choose minimum data rate out of list and recalculate Optim_inf_bit_TX/RX 
 		{
 		Optim_inf_bit_TX=0;
 		Optim_inf_bit_RX=0;	
 		}

 		
      	// **********************************************************************************
      	//         Calculate remaining data rate (at user interface) for TX and RX			*
      	//               - Compare Results of TX and RX										*
      	//				 - Decide data rate. Take minimum value, if rate has to be identical in both directions*
        //				 - Calculate number of required user information bits (RS if active)																	*
      	// **********************************************************************************
		V_11_clock_error = OFDM_opt_decide_rate(&Optim_inf_bit_TX, &Optim_inf_bit_RX, Data_Rate_Mode);
	 	
		if (V_11_clock_error <0)
			{
			// An error occured in rate decission
			if (V_11_clock_error == -1)
			{
				Error.Count++;
       			Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       			Error.Number[Error.Count].ID       = ERROR_RATE_OUT_OF_RANGE;
       			dbg_error[Error.Count]=ERROR_RATE_OUT_OF_RANGE;				
			}
			if (V_11_clock_error == -2)
			{
				Error.Count++;
       			Error.Number[Error.Count].Location = ERLOC_RX_QAM4;
       			Error.Number[Error.Count].ID       = ERROR_STUFFING_SETTING;
       			dbg_error[Error.Count]=ERROR_STUFFING_SETTING;				
			}
			}
		
		//   These are the target number of information bits to achieve selected data rate
		//    they will be used in V11_Init to initialize the V11 buffer sizes and
		//    ..TX will be used in QAM_processing_TX to generate the target number of training bits
		//    or to fetch the target number of data bits from V11 buffer
		//    ..RX will be used in QAM_processing_RX to copy the data bits to V11 buffer
			User_inf_bits_TX = Optim_inf_bit_TX;
			User_inf_bits_RX = Optim_inf_bit_RX;

		// Also save this results in Optim_sr.. global variables for further optimization processing
		//  sr means s_elected r_ate
			Optim_sr_inf_bit_TX= Optim_inf_bit_TX;
			Optim_sr_inf_bit_RX= Optim_inf_bit_RX;
		// To this result the number of redundancy bits are added in the following
		//  redundancy of 1) stuffing, 2) Service Bits, 3) RS coding
		// this yields the total number of required TCM information bits
		// TCM_optim_sr_inf_bit_TX resp. TCM_optim_sr_inf_bit_RX for further optimization processing in optim2()	
		
	// debug to remove PL !!		
	//Stuffing_Period = 1;
	//Stuffing_symb_per_period = 0;
		
		// Consider stuffing bits, if stuffing is applied		
		if (Stuffing_symb_per_period > 0)
			{// add one bit as Stuffing Flag to required number of inf bits
			Stuffing_Flag_Bit = 1;
			Optim_sr_inf_bit_TX +=Stuffing_Flag_Bit;
			Optim_sr_inf_bit_RX +=Stuffing_Flag_Bit;
			}
		else
			{ // no stuffing bit required
			Stuffing_Flag_Bit=0;
			}
			
			
		
 		// **********************************************************************************
      	//         If Service Channel is active, then										*
      	//                      - add number of Service Bits per OFDM Symbol				*
        //						  to number of required information bits					*													*
      	// **********************************************************************************
		if (Number_of_Service_bits > 0)
			{
			Optim_sr_inf_bit_TX +=Number_of_Service_bits;
			Optim_sr_inf_bit_RX +=Number_of_Service_bits;
			}
			
      	// **********************************************************************************
      	//         If RS coding then calculate number of code bits of RS 
      	//                      - Calculate RS code parameters								*
      	//						- Calculate RS_optim_codebits_XX 							*
        //						-  this will be number of required TCM information bits																					*
      	// **********************************************************************************
 		
		if (RS_Coding_Flag)
      		{
      		// TX path
      		// Number of RS inf bits to achieve selected data rate		
      		RS_optim_sr_inf_bit_TX = Optim_sr_inf_bit_TX;
      		// Calculate RS Code bits   
      		RS_Encoder_Init_cb(RS_optim_sr_inf_bit_TX);
      		RS_optim_sr_code_bit_TX	= RS_TCM_Nbit_Tx; 
      		
      		// RX path
      		// Number of RS inf bits to achieve selected data rate		
      		RS_optim_sr_inf_bit_RX = Optim_sr_inf_bit_RX;
      		// Calculate RS Code bits   
      		RS_Decoder_Init_cb(RS_optim_sr_inf_bit_RX);
      		RS_optim_sr_code_bit_RX	= RS_TCM_Nbit_Rx; 
      		
      		
      		TCM_optim_sr_inf_bit_TX = RS_optim_sr_code_bit_TX;
      		TCM_optim_sr_inf_bit_RX = RS_optim_sr_code_bit_RX;
      		}
      	else // without RS Coding
      		{
      		TCM_optim_sr_inf_bit_TX = Optim_sr_inf_bit_TX;
      		TCM_optim_sr_inf_bit_RX = Optim_sr_inf_bit_RX;
      		}

      	Optim1_RX = 0;
		
		rx_state_finished=1;
		
 		break;	
		
		}
	}

