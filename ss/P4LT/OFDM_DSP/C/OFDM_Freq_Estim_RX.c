/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Freq_Estim_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Primary estimation of frequency and clock offsets
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Freq_Estim_RX.c $
 * 
 * 11    24.07.08 15:31 Chhamaa
 * 
 * 6     5.04.06 15:28 Chhamaa
 * 
 * 6     17.03.06 14:41 Chhamaa
 * 
 * 4     30.09.05 10:55 Chalgyg
 * OFDM_DSP_V0_11_ur (12709051)
 * 
 * 1     19.01.05 15:40 Chchlee
 * 
 * 6     2.12.04 17:54 Chhamaa
 * 
 * 5     1.10.04 12:44 Leeb02
 * 
 * Initial version
 *       15.02.2003 DLK
 * Frequency compensation control variable in dependence of interpolation
 *       20.6.2004 PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <trans.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/dctcxo/ad5321.h"
#include "../h/dctcxo/ad5321_defs.h"
#include "../h/dctcxo/ad5321_externs.h"

extern float SFC_Interpolation;
extern float Alpha_CO;
extern float CO_delta;
unsigned int F_trend_ctr = 0;

//========================================

void segment ( "seg_ext_pmco" ) OFDM_Freq_Estim_RX()
	{
	void fft(int, float *, float pm *, complex_float *);
	void OFDM_Freq_LinReg_RX();
		
   	int i;
   	float CO_ppm;
	
	switch ( Freq_Est_RX )
       	{
       	case 0: // Clock Offset Estimation in initial,first SUBBLOCK of DF_EST_SUBBLOCKs

       	if(Df_block_cnt < (N_OFDM_SYMBOLS_PER_SUBBLOCK))
			{
			vcopy_ED((float *)&Df_Array[Df_block_cnt*NL],(float *)&Cmplx_Array[0],2*NL);

			Df_block_cnt++;
			rx_state_finished = 0;
			}
		else
			{
			Df_block_cnt = 0;
			Df_subblock_cnt = 1; // first Subblock for clock estimation
			
			Len_Lin_Regr=(int)((float)(NL*N_OFDM_SYMBOLS_PER_SUBBLOCK)/(float)LEN_FFT_DF_EST);
			
			OFDM_Freq_LinReg_RX(); // delivers Fr_Pil0 and Fr_Pil1
			
			// -------------------- two independent sources --------------------------		
			// Frequency offset = 2pi*Fr_Offset_Hz/F_Nyquist_Hz
			//Freq_Offset=((float)Tone_ind1*Fr_Pil0-
            //             (float)Tone_ind0*Fr_Pil1)/(float)(Tone_ind1-Tone_ind0);
			Freq_Offset=((float)Pilot_ind1*Fr_Pil0-
                         (float)Pilot_ind0*Fr_Pil1)/(float)(Pilot_ind1-Pilot_ind0);
            
		                 
            // Frequency offset in Hz
       		Df_Hz=Freq_Offset*(float)FN/OFDM_TWO_PI;
       			
			// Frequency Offset is not yet compensated, only measured
			Freq_Offset_Control_Flag = 0;
			Frequency_Control_enable = 0;   // in OFDM_FreqCtrl_RX.c
                         
                         // Clock offset = Clock_Offset_Hz/F_Nyquist_Hz
           	//Clock_Offset=(Fr_Pil0-Fr_Pil1)/(float)(Tone_ind0-Tone_ind1)
           	//             /OFDM_TWO_PI*(float)N;
           	Clock_Offset=(Fr_Pil0-Fr_Pil1)/(float)(Pilot_ind0-Pilot_ind1)
           	             /OFDM_TWO_PI*(float)N;
           
           	// systematic error 
           	// default value of AD5321 differs from AD5321_Convert_ppm2code(0 ppm)
           	// by this 0.751 ppm
//			Clock_Offset += 0.751e-6f;           	             
			Clock_Offset += 0.2233e-6f;       //1143(7ppm) 3010(-7ppm) => 2047(0.2233ppm)    	             
           	             
           	// Prepare DCTXCO control
     	    CO_ppm = Clock_Offset *(-1.0e6f);
        	AD5321_Control_Code = AD5321_Convert_ppm2code(CO_ppm); 
        
	     	AD5321_Control_Flag = 1;
        //  AD5321_Control_Flag = 0;             	             
        
        // Debug, save result into array
        	F_trend_ctr = 0;	
        	F_trend_DF[F_trend_ctr]   		= Df_Hz;
			F_trend_CO[F_trend_ctr]   		= Clock_Offset*1.0e6f;
			F_trend_CO_delta[F_trend_ctr] 	= Clock_Offset*1.0e6f;
			F_trend_CW[F_trend_ctr] 		= AD5321_Control_Code;
        	F_trend_ctr++;

        	
        	Freq_Est_RX++;
			rx_state_finished = 0;
			}
		break;

        case 1:
       	
   		// Empty case
        Freq_Est_RX++;
        rx_state_finished = 0;

   		break;

   		case 2:
       	
   		// Empty case
        Freq_Est_RX++;
        rx_state_finished = 0;

   		break;
   		
   		case 3: // Clock Offset Estimation in SUBBLOCK(s)
   		// this case is called (DF_EST_SUBBLOCKS-1)*(N_OFDM_SYMBOLS_PER_SUBBLOCK+1)+1 times
   		if (Df_subblock_cnt < DF_EST_SUBBLOCKS)
   		{
   		
   		if(Df_block_cnt < (N_OFDM_SYMBOLS_PER_SUBBLOCK))
			{
			vcopy_ED((float *)&Df_Array[Df_block_cnt*NL],(float *)&Cmplx_Array[0],2*NL);

			Df_block_cnt++;
			rx_state_finished = 0;
			}
		else
			{
			Df_block_cnt = 0;
			Df_subblock_cnt++;	// increment subblock counter
			
			Len_Lin_Regr=(int)((float)(NL*N_OFDM_SYMBOLS_PER_SUBBLOCK)/(float)LEN_FFT_DF_EST);
			
			OFDM_Freq_LinReg_RX(); // delivers Fr_Pil0 and Fr_Pil1
			
			// -------------------- two independent sources --------------------------		
			// Frequency offset = 2pi*Fr_Offset_Hz/F_Nyquist_Hz
			//Freq_Offset=((float)Tone_ind1*Fr_Pil0-
            //             (float)Tone_ind0*Fr_Pil1)/(float)(Tone_ind1-Tone_ind0);
			Freq_Offset=((float)Pilot_ind1*Fr_Pil0-
                         (float)Pilot_ind0*Fr_Pil1)/(float)(Pilot_ind1-Pilot_ind0);
             
            // Frequency offset in Hz
       		Df_Hz=Freq_Offset*(float)FN/OFDM_TWO_PI;
       		  
       		// on startup only clock is controlled
            // but with last estimation of Freq_Offset
            // Frequency Offset Compensation is activated
            //if (Df_subblock_cnt == DF_EST_SUBBLOCKS) Freq_Offset_Control_Flag = 1;
            if (Df_subblock_cnt == DF_EST_SUBBLOCKS) Frequency_Control_enable = 1;   // in OFDM_FreqCtrl_RX.c           
       		// Clock offset = Clock_Offset_Hz/F_Nyquist_Hz
       		CO_delta = (Fr_Pil0-Fr_Pil1)/(float)(Pilot_ind0-Pilot_ind1)/
                              OFDM_TWO_PI*(float)N;             
            Clock_Offset += (0.5f * CO_delta);
       
           	// Prepare DCTXCO control
     	    CO_ppm = Clock_Offset *(-1.0e6f);
        	AD5321_Control_Code = AD5321_Convert_ppm2code(CO_ppm); 
        
	     	AD5321_Control_Flag = 1;
        //  AD5321_Control_Flag = 0;             	             
        
        // Debug, save result into array
           	F_trend_DF[F_trend_ctr]   		= Df_Hz;
			F_trend_CO[F_trend_ctr]   		= Clock_Offset*1.0e6f;
			F_trend_CO_delta[F_trend_ctr] 	= CO_delta*1.0e6f ;
			F_trend_CW[F_trend_ctr] 		= AD5321_Control_Code;
        	F_trend_ctr++;

        	//Freq_Est_RX++;
			//rx_state_finished = 0;
			}
   		} // subblock counter
   		else
   		{
   		// all DF subblocks have been processed, go to next state
   		Freq_Est_RX++;
		rx_state_finished = 0;
   		}
		break;

/*		
        case 4:
       	
   		// Empty case
        Freq_Est_RX++;
        rx_state_finished = 0;

   		break;
*/
        case 4:
       	
   		// Last case , prepare end of state
	
		
       	Alpha_CO = 1.0f; // Coefficient used in fr_tracking for Clock_Offset Control
		Start_Ph_Pil0 = 0.0f;
		Start_Ph_Pil1 = 0.0f;
		Df_valid_result = 0; // to ignore first result of linear regression
       	
		// Reset inner State machine	
        Freq_Est_RX=0;
        
        //Debug PL Test: Wenn ow0 bzw ow1 auf ein hohes Vielfaches von 2PI gesetzt wird,
        //				dann funktioniert die Berechnung der linearen Regression nicht richtig
        //				Das Ergebnis ist quantisiert!! Wahrscheinlich, weil die tatsächliche Phase des 
        //				Pilots dann <<= ow0 bzw ow1
        //				Konsequenz: ow0 bzw. ow1 müssen parallelverschoben werden auf Null
        //				z.B. mit ow1 -= ow0; ow0=0.0f; nach jeder LR
        //ow0=12000.0f*OFDM_TWO_PI;
        //ow1=12250.0f*OFDM_TWO_PI;
        
        
        // Set Parent State Machine Flag
		rx_state_finished = 1;


   		break;

   		}
	}
		
