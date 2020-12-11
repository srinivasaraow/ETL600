/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Engine_RX.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : RX Engine
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Engine_RX.c $
 * 
 * 17    24.07.08 15:29 Chhamaa
 * 
 * 12    5.04.06 15:28 Chhamaa
 * 
 * 12    17.03.06 14:41 Chhamaa
 * 
 * 10    7.09.05 11:22 Ats
 * Exchange from TLC
 * 
 * 7     26.04.05 8:43 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 5     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 4     24.02.05 7:26 Chchlee
 * 
 * 2     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 7     2.12.04 17:54 Chhamaa
 * 
 * 5     1.10.04 12:45 Leeb02
 * 
 * Initial version
 *       15.02.2003 DLK
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * BER meter & scrambling switches
 *       26.01.2004 vgp 
 * Correct switching of BER meter
 *       26.01.2004 vgp
 * Modified spectrum density calculation
 *       02.03.04 vgp
 * Dynamic temporary memory allocation for TCM decoding according to M_RX
 *       04.03.04 vgp
 * V11 integration
 *       11.05.04 vgp
 * Combining common parts (EQualizer and RX-Filter Adaptaion)
 *		 12.05.04 PL
 * Introduced structure for new state machine
 * 		19.5.04 PL
*****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"
#include "../h/util/util.h"
#include "../h/tcm/TCM_Codec_externs.h"

float segment ( "seg_extn SHT_NOBITS" ) debug_QAM4[500];
unsigned int segment ( "seg_extn_init" ) debug_ptr=0;

//========================================
void segment ( "seg_ext_pmco" ) OFDM_Engine_RX()
	{
	register int i;
	float a;

//complex_float Pilot_diff,Pilot1_last_RX //global
float A2_Pilot_diff,A2_Pilot_last;
			    
    void  Fr_Tracking();
	void  Extract_QAM(complex_float * ,complex_float * ,int * ,int);
    int   Trellis_Decoder(complex_float * In_QAM_vector, float *ETF_vector, int * InfBits_Ptr, complex_float * Remod_QAM_vector, int, int *);
    void  Descrambler(int *, int *);
    float Exp_Gen(float, complex_float, complex_float *, float, int);
    void  Pilots_Preprocessing();
	void  Spectrum_QAM(complex_float *, complex_float *, int *, int, float);
	void  OFDM_Restart(); 
	void  fft(int, float *, float pm *, complex_float *);
	void  ifft(int, float *, float pm *, complex_float *);
    unsigned int   TCM_BER_Tester(unsigned int *, int, int, unsigned int *);
    void  Trellis_Encoder_Ref(int *, complex_float *, int, int *);
    void  TCM_Gen_MQAM_Train_Data_RX(unsigned int *OutPCM32, int len, int Mask);
//	void  vdivz_c_DDD(complex_float * ,complex_float * ,complex_float * ,int);
//	void  vdivz_f_DDD(float * ,float * ,float * ,int);
	void  OFDM_Fall_BackForward();
    void  Copy_to_V11_Buffer(int *In_ptr, int len);
    void  OFDM_Filter_RX_Watchdog(float , int );



    
    if((Start_RX_QAM_Engine == ON)&&(Pause_RX_QAM_Engine == OFF))
    	{
    	//-----------------------------------------------------------	
		//============== Common Receiver Engine Parts ===============
    		
    	// OFDM received symbol extraction in &Cmplx_Array[N2]
      	vcopy_c_DD(&Cmplx_Array[N2],&Cmplx_Array[L],N);          

		// Received OFDM spectrum in Cmplx_Array
      	fft(N, Fl_Array_R, Fl_Array_I, &Cmplx_Array[N2]);   
      	vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N); 	  

		// Extract Reference Pilot
		Pilot1_RX.re = Cmplx_Array[Pilot_ind1].re;	// Reference Pilot
		Pilot1_RX.im = Cmplx_Array[Pilot_ind1].im;	// Reference Pilot
	/*
        // Pilot tones preprocessing and level error estimation
        Pilots_Preprocessing();
      	
     	// Frequency and clock offset tracking
     	Fr_Tracking();
	*/
		// preeq =   Pilot_tx_reference divided by actual received (reference) Pilot
		Af = Pilot1_RX.re*Pilot1_RX.re + Pilot1_RX.im*Pilot1_RX.im;
      	preeq.re =  Amp_Pilot_RX*Pilot1_RX.re/Af;
      	preeq.im = -Amp_Pilot_RX*Pilot1_RX.im/Af;
      	
 	    // For debug: Hard decission
        //if(Level_Warning == 1) Alpha_Pil=1.0f;
     	
		// Preequalizer: complex scaling to ref pilot tone
		if(Level_Warning==0)
		{
		//Check ref Pilot deviation compared to last received OFDM symbol
		// (Soft decission)
		Pilot_diff.re =  Pilot1_RX.re - Pilot1_last_RX.re;
		Pilot_diff.im =  Pilot1_RX.im - Pilot1_last_RX.im;
		A2_Pilot_diff =  Pilot_diff.re*Pilot_diff.re + Pilot_diff.im*Pilot_diff.im;
		A2_Pilot_last =  Pilot1_last_RX.re*Pilot1_last_RX.re + Pilot1_last_RX.im*Pilot1_last_RX.im;
				// idee: A2_Pilot_last aus Af ableiten. Spart oberen Rechenschritt PL
			
		// Update last Pilot memory
		Pilot1_last_RX.re = Pilot1_RX.re;
		Pilot1_last_RX.im = Pilot1_RX.im;

		// Check if deviation exceeds threshold
		if (A2_Pilot_diff > 0.8f*A2_Pilot_last)
			{
			// reset Kalman Filter coefficient
			Alpha_Pil=1.0f;
			}
     		// idee: A2_Pilot_last=Af; //dann muss A2_Pilot_last aber global sein
		
		// Filtering of preequalizer
      	Preeq.re = Preeq.re + Alpha_Pil*(preeq.re - Preeq.re);      	
		Preeq.im = Preeq.im + Alpha_Pil*(preeq.im - Preeq.im); 
		
		// Update of Preequalizer Filter Coefficient
		Alpha_Pil *= 0.8122524f;
//      	if(Alpha_Pil < 0.2f) Alpha_Pil = 0.2f; 
		// PL !!
      	if(Alpha_Pil < Alpha_Pil_Destination) Alpha_Pil = Alpha_Pil_Destination; 
  //    Alpha_Pil=1.0f;
		}
		vsmul_c_DD(Cmplx_Array,Preeq,Cmplx_Array,N);
     	
      	// Save signal for equalizer adaptation in &Adapt_Buf[0]
      	vcopy_ED((float *)&Adapt_Buf[0],(float *)&Cmplx_Array[0],N2);
      	
		// Main equalizer
		vdivz_c_DDD(&Cmplx_Array[N+Min_Index_RX],&Cmplx_Array[Min_Index_RX],&Eqlzr[Min_Index_RX],Max_Index_RX-Min_Index_RX+1); 

        // Save signal for RX filter adaptation in (complex_float *)&Fl_Array_I[0]
      	vcopy_c_PD((complex_float pm *)&Fl_Array_I[0],&Cmplx_Array[N],N);

      	//============== End of Common Receiver Engine Parts ===============
    	//-----------------------------------------------------------	

    	switch ( RX_OFDM_Engine_state )
    		{
    		case RX_Engine_IDLE:
    		
    		break;
    		
    		case RX_Engine_QAM4:
    			// Decode QAM4 Signal, deliver bit stream to QAM_processing_RX, that follows after this function
    			// Consider QAM4 Settings: QAM_Index_RX, M_RX
    			// No Adaptation of Equalizer and RX-Filter
    			// Calculation of MSE and CRC_Errors_Sum, BLER_Est
    			/*
    			//debug, save QAM4 on left, middle, right carrier
    			vcopy_ED(&debug_QAM4[debug_ptr],(float *)&Cmplx_Array[N+N/8+2],2); //left
    			debug_ptr+=2;
    			vcopy_ED(&debug_QAM4[debug_ptr],(float *)&Cmplx_Array[N+N/2],2); //middle
    			debug_ptr+=2;
    			vcopy_ED(&debug_QAM4[debug_ptr],(float *)&Cmplx_Array[N+7*N/8-2],2); //right
    			debug_ptr+=2;
    			if (debug_ptr>500) debug_ptr=0;
    			*/
    			
    			// Extract QAM modulated carriers (output in Cmplx_Array)
				vcopy_DE((float *)&Fl_Array_R[0],(float *)&QAM_Index_RX[0],M_RX);
      			Extract_QAM(Cmplx_Array,&Cmplx_Array[N],(int *)&Fl_Array_R[0], M_RX);
		                
        		// Initialze squared estimation of channel transfer function in Fl_Array_R	 
        		ETF = (float *)&Cmplx_Array[N2];
		        
		        vset_f_E(ETF,1.0f,M_RX);
		        
		        // Trellis Decoder (input in Cmplx_Array, remodulated signal in &Cmplx_Array[N]),
		      	// ETF in (int *)&Fl_Array_R[8*N-M], output bits in (int *)&Fl_Array_R[0]
		
        	    // ==============TCM Decoder SRAM Memory Requirements ===================
				// 1. Bit_Buffer_RX_Ptr must points array in INTERNAL SRAM BLOCK 1 type DM
				// of length BIT_BUFFER_MEMORY_LENGTH_RX/2 = 6908/2 = 3454 32 bit words for M_RX = 512
		 		// 2. TCM_Decoder_Tmp_Vec must point internal DM SRAM BLOCK 1 vector 
				// length 9*(M_RX/2) = max 2304 words for M_RX = 512
		      	// !!!!!!!!!!!!!!!!!!!!!Max M_RX = 484!!!
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	

		        TCM_Decoder_Tmp_Vec = (int *)&Cmplx_Array[N2+N/2];
	
    		    Bit_Buffer_RX_Ptr = TCM_Decoder_Tmp_Vec + 9*(M_RX/2);
	      	
    		    CRC_Error = Trellis_Decoder(Cmplx_Array, ETF, Bit_Buffer_RX_Ptr, &Cmplx_Array[N],
      	                            M_RX, TCM_Decoder_Tmp_Vec);

			    if((CRC_Error == 1) && (Level_Warning==0))
					{
					// CRC error counter
					CRC_Errors_Sum += CRC_Error;  
					}
		
				BLER_Est = (float)CRC_Errors_Sum/(float)(n_blk_rx_QAM4+1);
		
				// Packing
        		Pack_1Bit_To_32BitWord_DD(Out_PCM_Ptr,
		                                  (int *)(2*(unsigned int)&Bit_Buffer_RX_Ptr[0]+CRC_LEN),
        		                          32*OFDM_Num_32b_Words_RX);
	
		        // Energy of remodulated signal
		        QAM_Remod_Energy = vsummag2_c_D(&Cmplx_Array[N],M_RX)/(float)M_RX;
		        // MSE estimation
		        vsub_c_DDD(Cmplx_Array,Cmplx_Array,&Cmplx_Array[N],M_RX);

        		MSE = vsummag2_c_D(Cmplx_Array,M_RX)/(float)M_RX; 
			
    		    MSE_Mean = MSE_Mean + 0.1f*(MSE - MSE_Mean);
        
		        MSE_dB = 10.0f*log10f(MSE/QAM_Remod_Energy);      	
		        MSE_dB_Mean = 10.0f*log10f(MSE_Mean/QAM_Remod_Energy);      	
      	
		        //if(Skp_cnt < SKP_NUM)	MSE_trend_QAM4[Skp_cnt] = MSE_dB;
		//		else {Skp_cnt = 0; Skp[Skp_cnt++] = MSE_dB;}

    			//n_blk_rx_QAM4++;
    			
    			if(n_blk_rx_QAM4 < SKP_NUM)	MSE_trend_QAM4[n_blk_rx_QAM4] = MSE_dB;
		
 /*   			
    			if(n_blk_rx_QAM4==24)
   			{
   				asm("#include <def21161.h>");
    asm("dm(CP1A)=m5;");
    asm("dm(CP3A)=m5;");
    asm("dm(SPCTL0) = m5;");
    asm("dm(SPCTL2) = m5;");    
    asm("nop;");
asm("nop;");
asm("nop;");
   			asm("nop;");
			asm("nop;");
			asm("nop;");
   			}
    	*/		
    		break;
    		
    		
    		case RX_Engine_QAM_M_Training:
    		
    			//----- Received QAM-Symbols --------------------------------------//
				// Extract QAM modulated carriers (output in Cmplx_Array)
				vcopy_DE((float *)&Fl_Array_R[0],(float *)&QAM_Index_RX[0],M_RX);
      			Extract_QAM(Cmplx_Array,&Cmplx_Array[N],(int *)&Fl_Array_R[0], M_RX);
		       
      			
    			//----- Reference QAM-Symbols --------------------------------------//
    		    // Generate Training Bit Sequence (Reference Signal) in Bit_Buffer_RX_Ptr
				TCM_Gen_MQAM_Train_Data_RX((unsigned int *)Out_PCM_Ptr, OFDM_Num_32b_Words_RX,  PCM_32b_Mask_RX);
				
				Bit_Buffer_RX_Ptr = TCM_Decoder_Tmp_Vec + 9*(M_RX/2);
				
				Unpack_32Bit_To_1BitWord_DD((int *)(2*(unsigned int)&Bit_Buffer_RX_Ptr[0]+CRC_LEN), 
	   		                            Out_PCM_Ptr, OFDM_Num_32b_Words_RX);
	   		                            
	   	   		// Trellis Coder of Bits in Bit_Buffer_RX_Ptr into Cmplx_Array[N] 
	   	   		
	   	   		// !!!!!!!!!1 Error !!!! Trellis_encoder has wrong settings
				Trellis_Encoder_Ref(Bit_Buffer_RX_Ptr, &Cmplx_Array[N], M_RX, TCM_Decoder_Tmp_Vec);

				// temporary work around: copy received signal to refernece signal
				//vcopy_DD(&Cmplx_Array[N],Cmplx_Array,2*N);
       	        // Energy of remodulated signal (reference signal)
		        QAM_Remod_Energy = vsummag2_c_D(&Cmplx_Array[N],M_RX)/(float)M_RX;
		        QAM_Remod_Energy_Mean = QAM_Remod_Energy; //during training without averaging
		        
		        //---------- MSE estimation ----------------------------------------//
		        vsub_c_DDD(Cmplx_Array,Cmplx_Array,&Cmplx_Array[N],M_RX);

        		MSE = vsummag2_c_D(Cmplx_Array,M_RX)/(float)M_RX; 
			
    		    MSE_Mean = MSE_Mean + 0.1f*(MSE - MSE_Mean);
        
		        MSE_dB = 10.0f*log10f(MSE/QAM_Remod_Energy);      	
		        MSE_dB_Mean = 10.0f*log10f(MSE_Mean/QAM_Remod_Energy); 
		        
		        //if(Skp_cnt < SKP_NUM)	MSE_trend_QAMT[Skp_cnt++] = MSE_dB;
		        if(n_blk_rx_M_QAM_train < 1000)	MSE_trend_QAMT[n_blk_rx_M_QAM_train] = MSE_dB;
 
    			//---------- Adaptation of Equalizer and RX-Filter -------------------//
    			//	is done in common part
    			CRC_Error =0;
    		
    		break;
    		
    		
    		case RX_Engine_QAM_M_Data:
    			// Decode QAM-M Data
    			// Pass result to digital interface
    			// Adaptation of Equalizer and RX-Filter
    			
    			// Extract QAM modulated carriers (output in Cmplx_Array)
				vcopy_DE((float *)&Fl_Array_R[0],(float *)&QAM_Index_RX[0],M_RX);
      			Extract_QAM(Cmplx_Array,&Cmplx_Array[N],(int *)&Fl_Array_R[0], M_RX);
		                
        		// Initialze squared estimation of channel transfer function in Fl_Array_R	 
        		ETF = (float *)&Cmplx_Array[N2];
		        
		        vset_f_D(ETF,1.0f,M_RX);
                    
		      	// Trellis Decoder (input in Cmplx_Array, remodulated signal in &Cmplx_Array[N]),
		      	// ETF in (int *)&Fl_Array_R[8*N-M], output bits in (int *)&Fl_Array_R[0]
		
        	    // ==============TCM Decoder SRAM Memory Requirements ===================
				// 1. Bit_Buffer_RX_Ptr must points array in INTERNAL SRAM BLOCK 1 type DM
				// of length BIT_BUFFER_MEMORY_LENGTH_RX/2 = 6908/2 = 3454 32 bit words for M_RX = 512
		 		// 2. TCM_Decoder_Tmp_Vec must point internal DM SRAM BLOCK 1 vector 
				// length 9*(M_RX/2) = max 2304 words for M_RX = 512
		      	// !!!!!!!!!!!!!!!!!!!!!Max M_RX = 484!!!
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	

		        TCM_Decoder_Tmp_Vec = (int *)&Cmplx_Array[N2+N/2];
	
    		    Bit_Buffer_RX_Ptr = TCM_Decoder_Tmp_Vec + 9*(M_RX/2);
	      	
		        CRC_Error = Trellis_Decoder(Cmplx_Array, ETF, Bit_Buffer_RX_Ptr, &Cmplx_Array[N],
      	                            M_RX, TCM_Decoder_Tmp_Vec);

			    if((CRC_Error == 1) && (Level_Warning==0))
					{
					// CRC error counter
					CRC_Errors_Sum += CRC_Error;  
					}
		
				BLER_Est = (float)CRC_Errors_Sum/(float)(n_blk_rx_data+1);
		

				// Restart if block error burst >= MAX_CRC_ERROR_BURST
				if((CRC_Error == 1) && (Level_Warning==0))
   					{
   					Block_Error_Cnt--;
	    			// Restart after too long error burst
	    			if(Block_Error_Cnt < 0) 
	    				{
	    				Error.Count++;
       					Error.Number[Error.Count].Location = ERLOC_RX_M_QAM_DATA;
       					Error.Number[Error.Count].ID       = ERROR_CRC;
       							
	    					//OFDM_Restart();   // to move to Error Handler function
	    				}
   					}
				else
					{
					Block_Error_Cnt = MAX_CRC_ERROR_BURST;
					}
						

				// Packing
        		Pack_1Bit_To_32BitWord_DD(Out_PCM_Ptr,
		                                  (int *)(2*(unsigned int)&Bit_Buffer_RX_Ptr[0]+CRC_LEN),
        		                          32*OFDM_Num_32b_Words_RX);
	
							
		        // Energy of remodulated signal
		        QAM_Remod_Energy = vsummag2_c_D(&Cmplx_Array[N],M_RX)/(float)M_RX;
		        QAM_Remod_Energy_Mean = QAM_Remod_Energy_Mean + 0.1f*(QAM_Remod_Energy - QAM_Remod_Energy_Mean);
		        
		        // MSE estimation
		        vsub_c_DDD(Cmplx_Array,Cmplx_Array,&Cmplx_Array[N],M_RX);

				// Spectral density of MSE
				vmag2_c_DD(Fl_Array_R,Cmplx_Array,M_RX);
				vcopy_DE(&Fl_Array_R[N],MSE_Spectral_Density_Sort,M_RX);
				vkalman_f_DDD(Fl_Array_R,&Fl_Array_R[N],Fl_Array_R,Mean_Coeff,M_RX);
				vcopy_ED(MSE_Spectral_Density_Sort,Fl_Array_R,M_RX);

        		// MSE cont.
				MSE = vsummag2_c_D(Cmplx_Array,M_RX)/(float)M_RX; 
        		MSE_Mean = MSE_Mean + 0.1f*(MSE - MSE_Mean);
        
		        MSE_dB = 10.0f*log10f(MSE/QAM_Remod_Energy);      	
		        MSE_dB_Mean = 10.0f*log10f(MSE_Mean/QAM_Remod_Energy);      	
      	
		        // Delta_Sigma estimation (to compare with sqrtf(K_Prob_Err))
		        delta_sigma      = Delta_TX / sqrtf(MSE);
		        delta_sigma_mean = delta_sigma_mean + 0.1f*(delta_sigma - delta_sigma_mean);
		        
		        if(Skp_cnt < SKP_NUM)	MSE_trend_QAMD[Skp_cnt++] = MSE_dB;
		        //if(n_blk_rx_data < SKP_NUM)	MSE_trend_QAMD[n_blk_rx_data] = MSE_dB;
				else {Skp_cnt = 0; Skp_cnt2++; MSE_trend_QAMD[Skp_cnt++] = MSE_dB;}
		
				// Check Fallback-FallForward Restart Criterions
				 if((Level_Warning==0)&&(n_blk_rx_data>200)) //only if signal is available
				 	{
				 	OFDM_Fall_BackForward();	
				 	}
				//
				
				
				// ------------- Measurement Block ---------------------------------
		    	// measurement of local SNR only if activated by Long Status Command
		    	// of in Digital Loop		   		
		   		if ((SNR_TCM_Switch)||(OFDM_Loop_Mode == DIGITAL_LOOP)) 
		   			{
		   				
					// Spectral density of remodulated signal
					vmag2_c_DD(Fl_Array_R,&Cmplx_Array[N],M_RX);			
					vcopy_DE(&Fl_Array_R[N],Remod_Spectral_Density,M_RX);
				    vkalman_f_DDD(Fl_Array_R,&Fl_Array_R[N],Fl_Array_R,Mean_Coeff,M_RX);
					vcopy_ED(Remod_Spectral_Density,Fl_Array_R,M_RX);
		
					// Delta_Sigma_Array
					vcopy_DE(Fl_Array_R,MSE_Spectral_Density_Sort,M_RX);
					svdivz_f_DD(&Fl_Array_R[N],1.0f,Fl_Array_R,M_RX);
					vsqrt_f_DD(&Fl_Array_R[N],&Fl_Array_R[N],M_RX);
					vcopy_ED(Delta_Sigma_Array,&Fl_Array_R[N],M_RX);
/*
					// Spectral distribution of SNR
					vcopy_DE(&Fl_Array_R[N],Remod_Spectral_Density,M_RX);
					vdivz_f_DDD(Fl_Array_R,&Fl_Array_R[N],Fl_Array_R,M_RX);		
   					vadb_f_DD(Fl_Array_R,Fl_Array_R,M_RX);
					vcopy_ED(SNR_TCM_Sort,Fl_Array_R,M_RX);

					vcopy_DE(Fl_Array_R,SNR_TCM_Sort,N);
					vcopy_DE((float *)&Cmplx_Array[0],(float *)&QAM_Index_RX[0],N);
					vscatter_f_DDD(&Fl_Array_R[N],Fl_Array_R,(int *)&Cmplx_Array[0],N);
					vcopy_ED(SNR_TCM,&Fl_Array_R[N],N);
*/					
					// MSE_Spectral_Density_Sort / QAM_Remod_Energy
					vset_f_D(&Fl_Array_R[M_RX],0.0f,N-M_RX);
					vcopy_DE(Fl_Array_R,MSE_Spectral_Density_Sort,M_RX);
    				vsmul_f_DD(&Fl_Array_R[N],1.0f/QAM_Remod_Energy_Mean,Fl_Array_R,M_RX);  
					vadb_f_DD(Fl_Array_R,&Fl_Array_R[N],M_RX);
					vcopy_DE((float *)&Cmplx_Array[0],(float *)&QAM_Index_RX[0],N);
					vscatter_f_DDD(&Fl_Array_R[N],Fl_Array_R,(int *)&Cmplx_Array[0],N);
					vcopy_ED(SNR_TCM,&Fl_Array_R[N],N);
					
					}
				   		    
					
    		break;
    		
    		
    		default:      	  

      			OFDM_Restart();

      		break;	
    		}
    	
    		
    		
	  if (
	  		( RX_OFDM_Engine_state == RX_Engine_QAM_M_Training)
    	 ||	( RX_OFDM_Engine_state == RX_Engine_QAM_M_Data)
    	 || ( RX_OFDM_Engine_state == RX_Engine_QAM4)
    		)
    	{
      //-------------------------------------------------------------	
	  //============== Common Receiver Engine Parts =================
	  //									== for M-QAM-States only ==
   		

    	// Rebuild OFDM symbol in frequency domain in Cmplx_Array
        
       	// clear OFDM spectrum
   		vset_f_D((float *)&Cmplx_Array[0],0.0f,N2);
		vset_f_D((float *)&Cmplx_Array[3*N],0.0f,N2);
		
		// Rebuilding of spectrum of OFDM signal (x_remodulated)
		// Place QAM remodulated carriers in spectrum
		vcopy_DE((float *)&Fl_Array_R[0],(float *)&QAM_Index_RX[0],M_RX);
		Spectrum_QAM(Cmplx_Array, &Cmplx_Array[N],(int *)&Fl_Array_R[0], M_RX, 1.0f);

   		// Place pilot tones in spectrum
//   		Cmplx_Array[Pilot_ind0].re = Amp_Pilot_TX;
//   		Cmplx_Array[Pilot_ind1].re = Amp_Pilot_TX;      //PL140705	
   		Cmplx_Array[Pilot_ind0].re = Amp_Pilot;
   		Cmplx_Array[Pilot_ind1].re = Amp_Pilot;      	

		// Received unequalized signal y in &Cmplx_Array[N2] 
	   	vcopy_DE((float *)&Cmplx_Array[N2],(float *)&Adapt_Buf[0],N2);
		
	   	// Transfer function of the channel by K=y/x_remodulated
	   	// only in used part of the spectrum from Min_index to Max_index
		vdivz_c_DDD(&Cmplx_Array[3*N+Min_Index_RX],&Cmplx_Array[N2+Min_Index_RX],&Cmplx_Array[Min_Index_RX],Max_Index_RX-Min_Index_RX+1); 
			
		// Adaptation of equalizer and RX filter
		// -------------------------------------------------------------------
		//				only if no CRC error occured
		//				Note: during M-QAM-Training, CRC-Error is set to zero
		if((CRC_Error == 0) && (Level_Warning==0))
			{
			// -- Adaptation of equalizer 
			// ---------------------------------------------
			// Error signal in &Cmplx_Array[3*N]
			vkalman_c_DDD(Eqlzr,Eqlzr,&Cmplx_Array[3*N],Alpha_Channel,N);
				
			if((Filter_RX_Adaptation == ON)&&(RX_FILT_ADAPT_Switch==1))
				{
				// -- Adaptation of RX filter
				// ---------------------------------------------
				// Error signal multiplied with eqlzr in &Cmplx_Array[N2]
	            vset_f_E((float *)&Cmplx_Array[N2],0.0f,2*Min_Index_RX);
	            vset_f_E((float *)&Cmplx_Array[N2+Max_Index_RX+1],0.0f,2*(N-Max_Index_RX-1));
				vsubmul_c_DDPD(&Cmplx_Array[N2+Min_Index_RX],&Cmplx_Array[Min_Index_RX],(complex_float pm *)&Fl_Array_I[2*Min_Index_RX],&Eqlzr[Min_Index_RX],M0_RX);
				// next lines are replaced by upper command
					// Restore signal for RX filter adaptation from (complex_float *)&Fl_Array_I[0]
					// to &Cmplx_Array[N2]
      				//vcopy_c_DP(&Cmplx_Array[N2],(complex_float pm *)&Fl_Array_I[0],N);
					// Error signal in &Cmplx_Array[N2]
					//vsub_c_DDD(&Cmplx_Array[N2],Cmplx_Array,&Cmplx_Array[N2],N);
					// Error signal multilied with eqlzr in &Cmplx_Array[N2]
					//vmul_c_DDD(&Cmplx_Array[N2+Min_Index_RX],&Cmplx_Array[N2+Min_Index_RX],&Eqlzr[Min_Index_RX],M0_RX);
				
				// Multiplication with inv. preequalizer in Cmplx_Array
     			Af = Preeq.re*Preeq.re + Preeq.im*Preeq.im;
      			preeq.re =  Preeq.re/Af;
      			preeq.im = -Preeq.im/Af;
				vsmul_c_DD(Cmplx_Array,preeq,&Cmplx_Array[N2],N);

				if (use_long_RX_Filter_2NmL)
				{
				// Return to time domain in &Cmplx_Array[N]
      			ifft(N, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
      			vcombine_c_DDP(&Cmplx_Array[N2],Fl_Array_R,Fl_Array_I,N); 
      			// Multiplication with complex conjugate Frequency Control Array
      			vmul_c_DDE(&Cmplx_Array[N2],&Cmplx_Array[N2],&Complex_Exp_T[L],N);      			
     			// Set 1st part of vector to zero (0-2N)
				vset_f_D((float *)Cmplx_Array,0.0f,2*N2);
				// Set last part of vector to zero (3N-4N)
				vset_f_D((float *)&Cmplx_Array[3*N],0.0f,2*N);
				// Fft
				fft(N4,Fl_Array_R, Fl_Array_I, Cmplx_Array);
      			vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N4); 	  
				// Multiplication with complex conjugate input spectrum
				vmul_c_DDE(Cmplx_Array,Cmplx_Array,Temp_Buf,N4);
				// Multiplication with adaptation coefficient and
				//    addition with actual impulse response of RX filter
				vfsmulvadd_c_DDD(C_Filter_RX,Alpha_Filter,Cmplx_Array,C_Filter_RX,N4);
				// next lines are replaced by upper command vfsmulvadd_c_DDD
				 // vsmul_f_DD((float *)&Cmplx_Array[0],Alpha_Filter,(float *)&Cmplx_Array[0],N4);
				 // Addition with actual impulse response of RX filter
				 //vcopy_DE((float *)&Fl_Array_R[0],(float *)&C_Filter_RX[0],2*N2); 	  
				 //vadd_c_DDD(Cmplx_Array,Cmplx_Array,(complex_float *)&Fl_Array_R[0],N2);
    			 //vcopy_ED((float *)&C_Filter_RX[0],(float *)&Cmplx_Array[0],2*N2); 
//				asm("nop;");
//    			asm("nop;");
//    			asm("nop;");
    			}
				else
				{
				// Return to time domain in &Cmplx_Array[N]
      			ifft(N, Fl_Array_R, Fl_Array_I, Cmplx_Array);   
      			vcombine_c_DDP(&Cmplx_Array[N],Fl_Array_R,Fl_Array_I,N);
      			// Multiplication with complex conjugate Frequency Control Array
      			vmul_c_DDE(&Cmplx_Array[N],&Cmplx_Array[N],&Complex_Exp_T[L],N);      			
 				// Set 1st part of vector to zero
				vset_f_D((float *)Cmplx_Array,0.0f,N2);
				// Fft
				fft(N2,Fl_Array_R, Fl_Array_I, Cmplx_Array);
      			vcombine_c_DDP(Cmplx_Array,Fl_Array_R,Fl_Array_I,N2); 	  
				// Multiplication with complex conjugate input spectrum
				vmul_c_DDE(Cmplx_Array,Cmplx_Array,Temp_Buf,N2);
				// Multiplication with adaptation coefficient and
				//    addition with actual impulse response of RX filter
				vfsmulvadd_c_DDD(C_Filter_RX,Alpha_Filter,Cmplx_Array,C_Filter_RX,N2);
				// next lines are replaced by upper command vfsmulvadd_c_DDD
				 // vsmul_f_DD((float *)&Cmplx_Array[0],Alpha_Filter,(float *)&Cmplx_Array[0],N4);
				 // Addition with actual impulse response of RX filter
				 //vcopy_DE((float *)&Fl_Array_R[0],(float *)&C_Filter_RX[0],2*N2); 	  
				 //vadd_c_DDD(Cmplx_Array,Cmplx_Array,(complex_float *)&Fl_Array_R[0],N2);
    			 //vcopy_ED((float *)&C_Filter_RX[0],(float *)&Cmplx_Array[0],2*N2); 
					
				}
				
    			// Call Watchdog for RX-Filter Adaptation 
    			OFDM_Filter_RX_Watchdog(MSE_dB ,0);

    			 
    			}  // if Filter ON
    		} // if CRC_Error==0
    	 } // if M-QAM-State
		
		} // if (Start_RX_QAM_Engine == ON)
		
   	}



