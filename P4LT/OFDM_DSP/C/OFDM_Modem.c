#include "../h/board.h"
#if BOARD==1
#define DRS7
#else
#undef DRS7
#endif
/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: OFDM_Modem.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Main Module
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/OFDM_Modem.c $
 * 
 * 30    29.08.08 15:34 Chhamaa
 * 
 * 26    07-10-26 13:39 Chchlee
 * SOFTWARE_VERSION_ID updated
 * 
 * 24    6.09.06 16:58 Chchlee
 * Bug 257 
 * 
 * 23    21.08.06 15:37 Chchlee
 * 
 * 21    5.04.06 15:28 Chhamaa
 * 
 * 21    17.03.06 14:41 Chhamaa
 * 
 * 19    30.09.05 10:55 Chalgyg
 * OFDM_DSP_V0_11_ur (12709051)
 * 
 * 12    2.05.05 12:40 Ats
 * Exchange with TLC : OFDM V0_09S
 * 
 * 8     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 7     7.02.05 14:39 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 6     28.01.05 14:48 Chchlee
 * V11_Clk removed
 * 
 * 5     28.01.05 14:44 Chchlee
 * #include "../h/v11interface_types.h" removed
 * 
 * 4     28.01.05 14:26 Chchlee
 * FPGA handling removed
 * 
 * 3     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 12    22.12.04 9:30 Chchlee
 * 
 * 11    2.12.04 17:54 Chhamaa
 * 
 * 9     30.09.04 9:13 Leeb02
 * 
 * 8     30.09.04 9:10 Leeb02
 * 
 * 
 * Initial version
 *       15.11.2003 DLK
 * Modified to use with universal TCM codec
 *       21.01.2004 vgp 
 * Modified to use TDM bus transfers
 *       02.03.04 vgp
 * Modified to use with P4LT
 *       24.06.04 vgp
 * V11 data I/O synchronization to beginning of OFDM symbol
 *       24.06.04 vgp
 *
 * TDM bus allocation scheme for 32.768 MHz internal TDM bus
 * Overall number of time slots - 96
 * Time slot numbering from 1 to 96
 * 
 *    Transmitted by OFDM DSP time slots
 * /////////////////////////////////////////
 *            OFDM DSP broadcast
 * ==========================================
 * Time Slot #11 : OFDM DSP Service Data (not yet implemented)
 *
 *         OFDM DSP -> FE - DSP
 * ==========================================
 * Time Slot #33 : OFDM Modem Tx Signal (I1) 
 * Time Slot #34 : OFDM Modem Tx Signal (Q1) 
 * Time Slot #35 : OFDM Modem Tx Signal (I2) 
 * Time Slot #36 : OFDM Modem Tx Signal (Q2) 
 * Time Slot #37 : OFDM Modem Tx Signal (I3) 
 * Time Slot #38 : OFDM Modem Tx Signal (Q3) 
 * Time Slot #39 : OFDM Modem Tx Signal (I4) 
 * Time Slot #40 : OFDM Modem Tx Signal (Q4) 
 * Time Slot #41 : OFDM Modem Tx Signal (I5) 
 * Time Slot #42 : OFDM Modem Tx Signal (Q5) 
 * Time Slot #43 : OFDM Modem Tx Signal (I6) 
 * Time Slot #44 : OFDM Modem Tx Signal (Q6) 
 *
 *
 *    Received by OFDM DSP time slots
 * /////////////////////////////////////////
 *            MASTER DSP broadcast
 * ==========================================
 * Time Slot #1  : Frame ID, Toggle Bit (just ignored)
 * Time Slot #2  : Master DSP service data (just ignored)
 *
 *           FE - DSP -> OFDM DSP 
 * ============================================
 * Time Slot #21 : OFDM Modem Rx Signal (I1) 
 * Time Slot #22 : OFDM Modem Rx Signal (Q1) 
 * Time Slot #23 : OFDM Modem Rx Signal (I2) 
 * Time Slot #24 : OFDM Modem Rx Signal (Q2) 
 * Time Slot #25 : OFDM Modem Rx Signal (I3) 
 * Time Slot #26 : OFDM Modem Rx Signal (Q3) 
 * Time Slot #27 : OFDM Modem Rx Signal (I4) 
 * Time Slot #28 : OFDM Modem Rx Signal (Q4) 
 * Time Slot #29 : OFDM Modem Rx Signal (I5) 
 * Time Slot #30 : OFDM Modem Rx Signal (Q5) 
 * Time Slot #31 : OFDM Modem Rx Signal (I6) 
 * Time Slot #32 : OFDM Modem Rx Signal (Q6) 
 *
 *****************************************************************************/
 
//**************************************************************** 
//****-------------------1DDMMYY-----------------------------***** 
#define	SOFTWARE_VERSION_ID  12008083   
//****------------------------------------------------------****** 
//**************************************************************** 

 
// Test Switches
//#define IDLE_TIME_MEASURE 
#undef IDLE_TIME_MEASURE

 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <cdef21161.h>
#include <def21161.h>
asm("#include <def21161.h>");

#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_defs.h"
#include "../h/OFDM_globals.h"
#include "../h/tcm/TCM_Codec_globals.h"
#include "../h/util/speeddsp.h"

#include "../h/tdm/ofdm_tdm_globals.h"
#include "../h/util/util.h"

#include "../h/util/adsp_init.h"
#include "../h/tdm/ofdm_tdm_io.h"
#include "../h/RS_FEC/RS_globals.h"
#include "../h/v11/v11_globals.h"

#include "../h/int_dec_filters/INTDEC_globals.h"
#include "../h/tx_rx_filters/TXRX_FILT_globals.h"
#include "../h/dctcxo/ad5321_defs.h"
#include "../h/dctcxo/ad5321_globals.h"
#include "../h/dctcxo/ad5321.h"

#include "../h/TDM_Service.h"

#ifdef IDLE_TIME_MEASURE 
	#include "../h/util/timer.h"  //PLtimer
#endif

int xxx; //PL has to ask VOWA for what purpose is xxx

extern unsigned int data_2tx_request;
extern unsigned int data_2rx_request;

// GLobal Variables for IDLE TIME MEASUREMENT
#ifdef IDLE_TIME_MEASURE 
	// dt is time in ms
	// dt (timer interval) is multiplied with number of counts of timer
	float dt = 1.0172526041667e-5f; // 1/(3*32768) kHz
	unsigned int t1, t2, t3;
	unsigned int time_flag;
	unsigned int t_counter;
#endif
	float idle_time, idle_time_min, tx_time, rx_time; // is used anyway (for Long Status)

#if 0
 //debug High Floats
 #define OFDM_MAX_FLOAT 5.9f
  float * ofdmSamplePtr;
  bool ofdmOverflow;
  unsigned int ofdmLimIndex;
  float ofdmMaxSample = 0;
  float ofdmAbsSample;	
#endif

//#undef DRS7_BOARD
//========================================================

//void segment ( "seg_ext_pmco" ) main(void)
void main(void)
	{
	void  Wait_for_Modem_Configuration();	
    void  Modem_Init_basic();    
    void  Modulation_Init_QAM4();
    void  Modulation_Init_QAM_M(int);
    void  OFDM_Transmitter();
    void  OFDM_Receiver();
	void  OFDM_Channel_Simulator();
	void  OFDM_Channel_Simulator_Init();
	void  Error_handler();
	void  Start_V11_Data_IO();
    void  Stop_V11_Data_IO();
	void  Adjust_V11_TX_Buffer();    
	void  Adjust_V11_RX_Buffer();    
    void  OFDM_TDM_Stop();
	void  OFDM_TDM_Adjust();
    void  Init_Control_Services();
    void  Set_Alarm();   
    void  AD5321_Wait();
    void  AD5321_Set_Reg(unsigned, unsigned);     
	void  Check_for_Master_Message();
	
//===================================================
//
//          Hardware initialization
//===================================================    
#ifdef DRS7
	void  Init_DSP2_DRS7();
	
    Init_DSP2_DRS7();
#else	
    Init_OFDM_DSP();
    // Reset parallel port timing for FPGA
    asm("#include <def21161.h>");
    asm("r0 = RBWST7|RBWA0|EB3WS4|EB3A0|EB0WS0|EB0A0;");
    asm("dm(WAIT)=r0;");
#endif         

//======= Init frequency control circuit with 1.65V
    AD5321_Init();
    
//=======  Stop all data I/O
	Stop_V11_Data_IO();	



//===================================================
//
//          End of hardware initialization
//
//===================================================    

Software_Version_ID = SOFTWARE_VERSION_ID;


//===================================================
//====               Test loop settings          ====
//===================================================

//	OFDM_Loop_Mode = DIGITAL_LOOP;
	OFDM_Loop_Mode = ANALOG_LOOP;
	
	// --- External or Internal Configuration:
	//       1:external(via TDM), 0:internal(debug) ---
	External_Config=ON;

	// --- Channel Simulator Setting ---
	Channel_Simulator_Flag = OFF;

    // --- Scrambling ------------------
    Scrambling = ON;  

    // ----------- BER meter -----------
//    BER_Meter = ON;
    BER_Meter = OFF;
    
    // -------- PCM generator ----------
//    PCM_Generator = ON;  
    PCM_Generator = OFF;  

//===================================================
//====        End of Test loop settings          ====
//===================================================	


//===================================================
//===================================================
//====       Start TDM bus operating             ====
//===================================================

// Initialize Lifesign WatchDog (watching main loop operation)
// this Lifesign Counter is decremented by 1 in TDM-TRM-ISR
// if it reaches zero, then LifeSignupdate is stopped.
// It must be refreshed in every main cycle	
	LifesignWatchDog = 5000;
	
	
	xxx=0;

   	if( OFDM_Loop_Mode == ANALOG_LOOP )
   		{
   		IsModemStarted = OFF;
   		Set_Alarm();
     	OFDM_TDM_Init_Start();
   		IsModemStarted = OFF;	
        //======= initialize control servises
        Init_Control_Services();
		}
    else
    	{
       	External_Config=OFF;
    	}


//===================================================
//====       Get Modem Configuration             ====
//===================================================
	Wait_for_Modem_Configuration();
	
//    V11_Clk = V11_Baudrate_256kbps;
//    V11Interface_SetBaudrate(V11_Clk);
    
	
	if(OFDM_Loop_Mode == DIGITAL_LOOP)
		{	// in digital loop there is no FE-DSP frequency shift
			Df_Hz_TX = Df_Hz_RX;
		}
//===================================================
//====       Modem initialization Part I         ====
//===================================================	    
	Modem_Init_basic();


	
//======= IDLE TIMER ================================
	/*
	#ifdef IDLE_TIME_MEASURE 
	    if ( OFDM_Modem_Type==OFDM_MASTER ) 
	      {
	 	    AD5321_Wait();
	 	     Timer_Stop();
	      }
	#endif
	*/
	idle_time_min = 1000.0f; // used anyway for LongStatus
	#ifdef IDLE_TIME_MEASURE 
	t_counter=0;
	#endif
//===================================================
//====               Test loop preparation       ====
//===================================================
	if(OFDM_Loop_Mode == ANALOG_LOOP)
		{
		Channel_Simulator_Flag = OFF;
		}
	
   	if(OFDM_Loop_Mode == DIGITAL_LOOP)
		{
		PCM_Generator = ON;
		BER_Meter = ON;
		}
		
	if(Channel_Simulator_Flag == ON)
		{
		OFDM_Channel_Simulator_Init();
		}

		
		
//===================================================
//====       Modem initialization Part II        ====
//===================================================
	// Modulation Initialization
	Modulation_Init_QAM4();
	
	// Start TDM bus operation			
   	if( OFDM_Loop_Mode == ANALOG_LOOP )
   		{
	    OFDM_TDM_Adjust();
     	OFDM_Sync_to_TDM();
     	IsModemStarted = ON;
   		}
   		
   // If Modem is disabled then
   // terminate in endless loop
   // Flag is set in "Verify_Modem_Config.c"			
   if (OFDM_Modem_DISABLE)
   		{
   		while(1);
   		}

		
//======================================================
//!!!!!!!!!       MAIN LOOP						!!!!!!!!
//======================================================
 	while (1)
		{
		// Refresh LifesignWatchDog
		// [LifesignWatchDog is decremented by one every
		// 100 TDM-Interrupts (100/10.666khz)
		// If there is no Refresh for 54*(100/10.666khz) ~ 0.5s
		// Lifesign Update will stop in ofdm_tdm_trm_isr ]
			
		LifesignWatchDog = 54;	
			
		
	//=============== DIGITAL_LOOP =====================================
   		if(OFDM_Loop_Mode == DIGITAL_LOOP)
   			{
			//  Transmitter                        
			OFDM_Transmitter();

	   		n_blk_tx++;

	   		// TX Signal Power
	        if(TX_QAM_State == TX_M_QAM_DATA)
				{
	        	Symbol_Energy_TX = vsummag2_c_E(OFDM_Symbol_TX,2*NL);
	        	Symbol_Power_TX=Symbol_Energy_TX/(float)(2*NL); 
	        	if (Symbol_Power_TX_Mean==0.0f)
		        	{
		        	Symbol_Power_TX_Mean = Symbol_Power_TX;
		        	}
		        else
		        	{
		            Symbol_Power_TX_Mean=Symbol_Power_TX_Mean +
	        						 0.01f*(Symbol_Power_TX-Symbol_Power_TX_Mean); 
					}
	        	}
			
	        // Channel Simulator
			if(Channel_Simulator_Flag == ON)
				{
				OFDM_Channel_Simulator();
				}
	   	    
		// Digital loop
		//PL2106 moved to OFDM_Receiver(), due to memory problems when using 
			//		 vcopy_s_EE
		// vcopy_EE((float *)&OFDM_Symbol_RX[0],(float *)&OFDM_Symbol_TX[0],4*NL);   			

			// Receiver                        

			// RX Signal Power
	   		if(Start_RX_QAM_Engine == 1)
				{
		        Symbol_Energy_RX = vsummag2_c_E(OFDM_Symbol_RX,2*NL);
		        Symbol_Power_RX=Symbol_Energy_RX/(float)(2*NL);
		        if (Symbol_Power_RX_Mean==0.0f)
		        	{
		        	Symbol_Power_RX_Mean = Symbol_Power_RX;
		        	}
		        else
		        	{
		            Symbol_Power_RX_Mean=Symbol_Power_RX_Mean +
	        						 0.01f*(Symbol_Power_RX-Symbol_Power_RX_Mean); 
					}
				}

	        // Measured SNR values  
			if((Channel_Simulator_Flag == ON) && (Sim_AWGN_Flag == ON)) 
				{
				// Channel Input
				SNR_Inp=10.0f*log10(Symbol_Power_TX_Mean/AWGN_Power_Mean);

				// Channel Output (Receiver Input)
				SNR_Outp=10.0f*log10(Symbol_Power_RX_Mean/AWGN_Power_Mean);
				}
	   	    
	        OFDM_Receiver();
	        
	        // Error Handler
           	Error_handler();

	        n_blk_rx++;
			}
	
			
	//=============== ANALOG LOOP =====================================
	//=============== ANALOG LOOP =====================================
	//=============== ANALOG LOOP =====================================

	if ( OFDM_Loop_Mode == ANALOG_LOOP )
		{
		// -------- Start of V11 Interface ---------------------	
			  if ( V11_Start_IO == 1) {
				Start_V11_Data_IO();
				V11_IsStarted = 1;				
				V11_Start_IO = 0;
				Sync_State = V11_sync;
				Selected_data_rate_IDX      = Selected_data_rate_IDX_prepared;
				OFDM_Alarm.currentDatarateIx= Selected_data_rate_IDX & 0x7;
				}
		// -------- Frequency Control --------------------------	        
		if (OFDM_Modem_Type==OFDM_SLAVE)
			{	     	
 			if (AD5321_Control_Flag == 1)
              	{
              	AD5321_Wait();
            	AD5321_Set_Reg(AD5321_NORMAL_MODE, AD5321_Control_Code);    
    	      	AD5321_Control_Flag = 0;
              	}
			} 
		// debug line
		// AD5321_Set_Reg(AD5321_NORMAL_MODE, VOSC165VCODE);    
	
			

		// ---------- Symbol Time Measurement (Verification of Timer Operation) --
		/*
		#ifdef IDLE_TIME_MEASURE 
 			if (OFDM_Modem_Type==OFDM_MASTER && n_blk_rx_data >= 1)
 				{ 
 				Timer_Start(0xffffffff);         	
 				t2 = Get_Timer();   //PLtimer
 				time_flag = 1;
 				}
		#endif
		*/
		
		 
		// ========= Transmitter =====================
		if (data_2tx_request==1)
		{
		// ---------- Processing Time Measurement Start ------------------------------
		#ifdef IDLE_TIME_MEASURE 
		// Start Timer
			if (OFDM_Modem_Type==OFDM_MASTER)
				{
				// if (Start_TX == 1 && n_blk_rx_data >= 1)
 				if (Start_TX == 1 && Num_Symbol_TX >= 1)
	 				{ 
	 				Timer_Start(0xffffffff);         	
	 				t2 = Get_Timer();   //PLtimer
	 				time_flag = 1;
	 				}
				}
		#endif
		// ---------- Processing Time Measurement (end)--------------------------

			// -------- Adjust V11 pointers (only once) ------------------------
			if (V11_RX_IsStarted == 1)	  // is set in first call of V11 (SPORT 0) ISR
				  Adjust_V11_RX_Buffer(); // is only executed once and sets Flag V11_RX_Buffer_IsAdjusted =1
			
			// Call Transmitter (generate samples)
		   	OFDM_Transmitter();
		   	
		   	//  Copy samples to TDM buffer
		   	// !!! Do not change NL2Q !!!, 
            // which is defined in Modem_Init_Basic.c
			#if 0 // Test Maximum Hold und Begrenzung
				ofdmSamplePtr = (float*)OFDM_Symbol_TX;
				ofdmOverflow = FALSE;
				for (ofdmLimIndex = 0; ofdmLimIndex < 2*NL2Q; ofdmLimIndex++)
				{
					ofdmAbsSample = fabs(*ofdmSamplePtr);
            		if (ofdmAbsSample > ofdmMaxSample) ofdmMaxSample = ofdmAbsSample;
	
					if (*ofdmSamplePtr >  OFDM_MAX_FLOAT) *ofdmSamplePtr =  OFDM_MAX_FLOAT;
					if (*ofdmSamplePtr < -OFDM_MAX_FLOAT) *ofdmSamplePtr = -OFDM_MAX_FLOAT;
	
            		ofdmSamplePtr++;	
				}
	            
				if (ofdmMaxSample >= OFDM_MAX_FLOAT) {
				asm(" nop;");	
				asm(" nop;");	
				asm(" nop;");	
				}
			#endif  
				
		   	vcopy_out_buf(OFDM_Symbol_TX,NL2Q);
		   	n_blk_tx++;

		// ---------- Processing Time Measurement Stop ------------------------------
		#ifdef IDLE_TIME_MEASURE 
			if (OFDM_Modem_Type==OFDM_MASTER && time_flag==1)
				{		   	
				t1 = Get_Timer();	 
				t3 = t2-t1;
				tx_time = dt*(float)(t3); // its tx processing time
				// Save Time Measurement and Modem State into external Array (Size 2500)
				if (t_counter<sizeof(Time_trend)-5)
					{
					Time_trend[t_counter+0]=1111111111.0f; // Label for TX
					Time_trend[t_counter+1]=Num_Symbol_TX;
					Time_trend[t_counter+2]=tx_time;
					Time_trend[t_counter+3]=TX_Preamble_State;
					Time_trend[t_counter+4]=TX_QAM_State;
					t_counter += 5;
					}
				// Save minimum idle time
				//if (idle_time < idle_time_min) idle_time_min=idle_time;
				Timer_Stop();
				time_flag = 0;
				}
		#endif
		// ---------- IDLE Time Measurement (end) ----------------------------	
		}
		
		
		// ========= Receiver ========================
		if (data_2rx_request==1)
		{
		// ---------- Processing Time Measurement Start ------------------------------
		#ifdef IDLE_TIME_MEASURE 
		// Start Timer
			if (OFDM_Modem_Type==OFDM_MASTER)
				{
				// if (Start_TX == 1 && n_blk_rx_data >= 1)
 				if (Start_TX == 1 && Num_Symbol_TX >= 1)
	 				{ 
	 				Timer_Start(0xffffffff);         	
	 				t2 = Get_Timer();   //PLtimer
	 				time_flag = 1;
	 				}
				}
		#endif
		// ---------- Processing Time Measurement (end)--------------------------

			// -------- Adjust V11 pointers (only once) ------------------------
	    	if (V11_TX_IsStarted == 1)    // is set in first call of V11 (SPORT 2) ISR
				  Adjust_V11_TX_Buffer(); // is only executed once and sets Flag V11_TX_Buffer_IsAdjusted =1

			if (Symbol_Synchro_FS>0)	  // calculated in OFDM_LFM_Processing.c
				{
				// Get reduced number of samples from TDM buffer
				vcopy_in_buf(OFDM_Symbol_RX,Symbol_Synchro_FS);
				Symbol_Synchro_FS = 0;
				}
				else
				{
				// Get reduced number of samples from TDM buffer
				vcopy_in_buf(OFDM_Symbol_RX,NL2Q);
				}
					
			// Call Receiver (process samples)
			OFDM_Receiver();
			
			// Call Error Handler
			Error_handler();
			
			// Call Master-DSP Message Handler
			Check_for_Master_Message();
			
			n_blk_rx++;

				// ---------- Processing Time Measurement Stop ------------------------------
		#ifdef IDLE_TIME_MEASURE 
			if (OFDM_Modem_Type==OFDM_MASTER && time_flag==1)
				{		   	
				t1 = Get_Timer();	 
				t3 = t2-t1;
				rx_time = dt*(float)(t3); // its rx processing time
				// Save Time Measurement and Modem State into external Array (Size 2500)
				if (t_counter<sizeof(Time_trend)-5)
					{
					idle_time = T_OFDM_symbol*1000.0f - (tx_time + rx_time);
					Time_trend[t_counter+0]=idle_time; // idle time instead of 11111111
					Time_trend[t_counter+1]=Num_Symbol_TX;
					Time_trend[t_counter+2]=rx_time;
					Time_trend[t_counter+3]=RX_Preamble_State;
					Time_trend[t_counter+4]=RX_QAM_State;
					t_counter += 5;
					}
				// Save minimum idle time
				if (idle_time < idle_time_min) idle_time_min=idle_time;
				
				Timer_Stop();
				time_flag = 0;
				}
		#endif
		// ---------- IDLE Time Measurement (end) ----------------------------	
		}
		
		   			   			   		   	
/*
	asm("#include <def21161.h>");
    asm("dm(CP1A)=m5;");
    asm("dm(CP3A)=m5;");
    asm("dm(SPCTL0) = m5;");
    asm("dm(SPCTL2) = m5;");    
    asm("nop;");
asm("nop;");
asm("nop;");
}
*/
/*

		// ---------- IDLE Time Measurement ------------------------------
		#ifdef IDLE_TIME_MEASURE 
			if (OFDM_Modem_Type==OFDM_MASTER)
				{
				// if (Start_TX == 1 && n_blk_rx_data >= 1)
 				if (Start_TX == 1 && Num_Symbol_TX >= 1)
	 				{ 
	 				Timer_Start(0xffffffff);         	
	 				t2 = Get_Timer();   //PLtimer
	 				time_flag = 1;
	 				}
				}
		#endif
		// ---------- IDLE Time Measurement (end)--------------------------

			
		// ---------- IDLE Time Measurement ------------------------------
		#ifdef IDLE_TIME_MEASURE 
			if (OFDM_Modem_Type==OFDM_MASTER && time_flag==1)
				{		   	
				t1 = Get_Timer();	 
				t3 = t2-t1;
				idle_time = dt*(float)(t3);
				// Save IDLE Time and Modem State into external Array (Size 2500)
				if (Num_Symbol_TX<500)
					{
					Time_trend[5*(Num_Symbol_TX-1)+0]=idle_time;
					Time_trend[5*(Num_Symbol_TX-1)+1]=TX_Preamble_State;
					Time_trend[5*(Num_Symbol_TX-1)+2]=TX_QAM_State;
					Time_trend[5*(Num_Symbol_TX-1)+3]=RX_Preamble_State;
					Time_trend[5*(Num_Symbol_TX-1)+4]=RX_QAM_State;
					}
				// Save minimum idle time
				if (idle_time < idle_time_min) idle_time_min=idle_time;
				Timer_Stop();
				}
		#endif
		// ---------- IDLE Time Measurement (end) ----------------------------

*/
		
	        }               		 
		}
	}


