/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Modulation_Init_QAM_M_v2.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Initialize M-QAM Modulation Parameters
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source AES/P4LT/OFDM_DSP/C/Modulation_Init_QAM_M_v2.c $
 * 
 * 5     6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 4     21.08.06 15:26 Chchlee
 * Bug 257 fixed
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 5     22.12.04 9:30 Chchlee
 * 
 * 4     2.12.04 17:54 Chhamaa
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * Split QAM-M initialization into own function
 *		 06.04.2004 PL
 * Error Report for TCM Initialization
 *		 28.10.2004 PL
*****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_Defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/tcm/TCM_Codec_externs.h"
#include "../h/util/util.h"

int RS_Encoder_Init(int );
int RS_Decoder_Init(int );

extern int giEmergency_restart;


void segment ( "seg_ext_pmco" ) Modulation_Init_QAM_M(int TX_RX)
	{
		// TX_RX==1: Initialization of Encoder (TX-Path)
		// TX_RX==0: Initialization of Decoder (RX-Path)
		
    int   TCM_Init_MQAM_Encoder(int *);
    int   TCM_Init_MQAM_Decoder(int *);
	
    int   i;
    float Q , QAM_max;
    int Q_max;
	
    switch (TX_RX)
    	{
    case 1: // TX
    
    	// **************************************************************
    	//      process (copy) all Optimization results to working arrays
    	//
    	// M_Opt_TX  		->  M_TX, M0_TX
    	// Bits_in_SC_TX[] 	->  Encoder Initialization (d2alloc_tx)
    	// Index_K_TX 		->  QAM_Index_TX, Min_Index_TX, Max_Index_TX
    	// C_TX				->  for Power calculation and Scaling
    	// **************************************************************

    	
    	M_TX = M_Opt_TX;
    		
    	// Number of channels during QAM4 transmission
		M0_TX = M_TX + N_pil;
		
		// Init QAM TX index array   Index_K_TX
		vcopy_DE(Fl_Array_R,(float *)Index_K_TX,N);
		
		Min_Index_TX = vmin_i_D((int *)&Fl_Array_R[0],M_TX);
		Max_Index_TX = vmax_i_D((int *)&Fl_Array_R[0],M_TX);
	    vcopy_ED((float *)&QAM_Index_TX[0],(float *)&Fl_Array_R[0],N);

	
	    //============== RS configuration parameters     ====================
		if (RS_Coding_Flag)
			{
			RS_Encoder_Init(RS_optim_sr_code_bit_TX);
			}
	    
	    //============== TCM configuration parameters     ====================
		// Init trellis Encoder
    	if (TCM_Init_MQAM_Encoder((int *)Fl_Array_R) == -1 )
    		{// fatal error in TCM Initialization
    		Error.Count++;
       		Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       		Error.Number[Error.Count].ID       = ERROR_TCM_INIT;
       		giEmergency_restart = 1;
       		asm("nop;");
    		asm("nop;");
    		asm("nop;");
    		}
    		//  Note: comprises "OFDM_QAM_Energy_TX" calculation:
            // intermediate results:TCM_QAM_Energy in TCM_bitcalc and
            // 		OFDM_QAM_Energy_TX= TCM_QAM_Energy in TCM_Init_Encoder     
		
    	// Find maximum modulation order
    	Q_max 	=  vmax_i_E(Bits_in_SC_TX,M_TX);
    	QAM_max =  powf(2.0f , (float)Q_max);
    	
    	// Power Calculation                
		Xmax      = sqrt(2.0f)*(Delta_TX/2.0f)*(sqrtf(QAM_max) - 1.0f);   
		Amp_Pilot = 1.0f*Xmax;
		
				
		P_OFDM_Spectrum = (OFDM_QAM_Energy_TX +
							C_TX[Pilot_ind0]*C_TX[Pilot_ind0]* Amp_Pilot*Amp_Pilot +
							C_TX[Pilot_ind1]*C_TX[Pilot_ind1]* Amp_Pilot*Amp_Pilot)/
		                  (float)N;

		// QAM scaling
		Scale_QAM_TX=sqrtf(P_OFDM_TX/P_OFDM_Spectrum);
		
		// Pilots scaling
		Amp_Pilot_TX =Amp_Pilot*Scale_QAM_TX;
	
	
		
		
	
    break;
    
    case 0: // RX
    
   	    // **************************************************************
    	//      process (copy) all Optimization results to working arrays
    	//
    	// M_Opt_RX  		->  M_RX, M0_RX
    	// Bits_in_SC_RX[] 	->  Decoder Initialization (d2alloc_rx)
    	// Index_K_RX 		->  QAM_Index_RX, Min_Index_RX, Max_Index_RX
    	// C_RX				->  for Power calculation and Scaling
    	// **************************************************************
    	
    	M_RX = M_Opt_RX;

		// Number of channels during QAM4 transmission
		M0_RX = M_RX + N_pil;
		
		// Init QAM RX index array   Index_K_RX
		vcopy_DE(Fl_Array_R,(float *)Index_K_RX,N);
		Min_Index_RX = vmin_i_D((int *)&Fl_Array_R[0],M_RX);
		Max_Index_RX = vmax_i_D((int *)&Fl_Array_R[0],M_RX);
	    vcopy_ED((float *)&QAM_Index_RX[0],(float *)&Fl_Array_R[0],N);
	    
	    //============== RS configuration parameters     ====================
		if (RS_Coding_Flag)
			{
			RS_Decoder_Init(RS_optim_sr_code_bit_RX);
			}
	
		//============== TCM configuration parameters     ===================
       	// Init trellis Decoder
       	if (TCM_Init_MQAM_Decoder((int *)Fl_Array_R) == -1 )
    		{// fatal error in TCM Initialization
    		Error.Count++;
       		Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       		Error.Number[Error.Count].ID       = ERROR_TCM_INIT;
       		giEmergency_restart = 1;
       		asm("nop;");
    		asm("nop;");
    		asm("nop;");
    		}
    		//  Note: comprises "OFDM_QAM_Energy_RX" calculation:
            // intermediate results:TCM_QAM_Energy in TCM_bitcalc and
            // 		OFDM_QAM_Energy_RX= TCM_QAM_Energy in TCM_Init_Encoder     

		//Init trellis Encoder for Reference signal in M-QAM training
		
            
    	//============== Power processing and scaling of SCK and Pilots ========

    	// Find maximum modulation order
    	Q_max 	=  vmax_i_E(Bits_in_SC_RX,M_RX);
    	QAM_max =  powf(2.0f ,(float)Q_max);
    	
    	// Power Calculation                
		Xmax      = sqrt(2.0f)*(Delta_TX/2.0f)*(sqrtf(QAM_max) - 1.0f);   
		Amp_Pilot = 1.0f*Xmax;
	
		
		P_OFDM_Spectrum = (OFDM_QAM_Energy_RX +
							C_RX[Pilot_ind0]*C_RX[Pilot_ind0]* Amp_Pilot*Amp_Pilot +
							C_RX[Pilot_ind1]*C_RX[Pilot_ind1]* Amp_Pilot*Amp_Pilot)/
		                  (float)N;
	
	
			

		// Pilots scaling
		Amp_Pilot_RX = Amp_Pilot;
		
		Amp_Pilot_TX_Ref = Amp_Pilot*sqrtf(P_OFDM_TX/P_OFDM_Spectrum);
		Ref_Level = (OFDM_PILOTS_NUM*Amp_Pilot_TX_Ref*Amp_Pilot_TX_Ref);

		Preeq.re = 1.0f;
		Preeq.im = 0.0f;
		Alpha_Pil = 1.0f;
		
		
		
    break;
    
    
    	}
     	

   	}


