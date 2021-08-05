/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Verify_Modem_Config.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Verify configuration modem settings
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Verify_Modem_Config.c $
 * 
 * 20    22.09.08 15:49 Chhamaa
 *  Frequency shift for MOD BW 2 kHz modified.
 * 
 * 19    29.08.08 15:38 Chhamaa
 * Frequency shift for MOD BW 2 kHz modified.
 * 
 * 14    21.07.06 10:54 Chhamaa
 * Comment see below.
 * 
 * 14    21.06.06 13:56 Chtognu
 * Frequency shift for BW 3.5 kHz set to 272 Hz.
 * 
 * 13    13.04.06 11:42 Chhamaa
 * Frequency shift for BW 11.5 kHz set to 188 Hz.
 * 
 * 12    6.04.06 14:04 Chhamaa
 * Frequency shift for BW 3.5 kHz set to 210 Hz.
 * 
 * 11    17.03.06 14:41 Chhamaa
 * 
 * 9     16.09.05 9:42 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
 * 
 * 5     26.04.05 8:44 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 3     24.02.05 7:28 Chchlee
 * 
 * 2     28.01.05 11:50 Ats
 * Exchange TLC
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     2.12.04 17:54 Chhamaa
 * 
 * 1     28.09.04 17:58 Maag01
 * Initial version
 * 
 * Initial version
 *       21.09.2004 vgp
 * Added external Config
 *       24.09.04 PL
*****************************************************************************/

#include <stdlib.h>
#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_externs.h"

int segment ( "seg_ext_pmco" ) Verify_Modem_Configuration(t_OFDM_Config  *Config)
{
	int NL_CHECK_passed;
	
	// ------ Modem Type Setting --OFDM_MASTER/OFDM_SLAVE-----
	OFDM_Modem_Type = Config->mode_reg.MODEM_Mode; // if 1 - Master, 0 - Slave
	
	OFDM_Modem_DISABLE = 0;
	
	// do not use HMI information for std_FN or high_FN
	//BW4_8_Mode = Config ->mode_reg.BW4_8_Mode; // 0-std_FN , 1-high_FN
	
	BW4_8_Mode = std_FN; // 0-std_FN default setting
//	BW4_8_Mode = high_FN; // 1-high_FN for BW7.5,BW8.0 LowDelay
	
	switch (Config->mode_reg.ORTH_Mode)
	{
		case ORTH_Mode64:
		    N = 64;
		    break;
		case ORTH_Mode128:
		    N = 128;
		    break;
		case ORTH_Mode256:
		    N = 256;
		    break;
		case ORTH_Mode512:
		    N = 512;
		    break;
		default:
		    return 2;
	}
		
	switch (Config->mode_reg.GUARD_Int)
	{
		case GUARD_Int8:
		    L = 8;
		    break;
		case GUARD_Int16:
		    L = 16;
		    break;
		case GUARD_Int32:
		    L = 32;
		    break;
		case GUARD_Int64:
		    L = 64;
		    break;
		default:
		    return 3; // exception return
	}
	
//N=128;
//L=8;
//Config->channelBW = 4000;
//Config->dataRate[0] = 9600;
		
	// Nyquist frequency in Hz
	switch (Config->channelBW)
	{
		case 32000:
		    BW = 32000;
		    FN = 32000;
		    break;
		case 28000:
		    BW = 28000;
		    FN = 32000;
		    break;
		case 24000:
		    BW = 24000;
		    FN = 24000;
		    break;
		case 20000:
		    BW = 20000;
		    FN = 24000;
		    break;
		case 16000:
		    BW = 16000;
		    FN = 16000;
		    break;
		case 12000:
		    BW = 12000;
		    FN = 12000;
		    break;
		case 8000:
		    BW = 8000; //or BW=7500
			/*
		    switch (BW4_8_Mode)
		    { case std_FN:  FN = 8000;  break;
		      case high_FN:
		      		//FN = 12000;
		      		FN=9600;  //for LowDelay N=128,L=7,16
		      	break;
		    }
			*/
			//  N=64 is not used anymore for BW8kHz.
			// if N=64 is configured in HMI, it means LowDelay Version with N=128 but FN=9600 !!!
			if (N==64)	// configure Low Delay Version and replace N=64
				{
				N  = 128;
				if (L==8) L=7;  // 64/8  -> 128/7
								// 64/16 -> 128/16
								// 64/32 -> 128/32 not configurable by HMI
				BW4_8_Mode = high_FN;
				FN = 9600;
				}
			else
			{
				BW4_8_Mode = std_FN;
				FN=8000; //High Delay Configuration for N>=128
			}

		    break;
		case 6000: // MAS2008
			BW=6000;
			FN=6000;
			if ((N==128) && (L==8)) L=7; // 128/8 is replaced by 128/7
			break;    
		case 4000:
		    BW = 4000;
			/*
		    switch (BW4_8_Mode)
		    { case std_FN:  FN = 4000; break;
		      case high_FN:
				  //FN = 6000;
				  FN = 4800;  // for N=64,L=8
				break;
		    }
			*/
			if (Config->leftPilotPresent == pilot_present)
			{ // BW =3500:---------------------
			  // always use stdFN: FN=4000; for all N
				BW4_8_Mode = std_FN;
				FN = 4000;  //PL TEST 09.03.06
			}
			else
			{ // BW =4000:---------------------
			  // use high_FN, FN=4800 for N=64
			  // use std_FN , FN=4000 for N>=128
			  if (N==64)
				{
				BW4_8_Mode = high_FN;
				FN = 4800;  //PL TEST 09.03.06
				}
			  else
				{
				BW4_8_Mode = std_FN;
				FN = 4000;  //PL TEST 09.03.06
				}
			}
		    break;
	    case 2000: // MAS2008
			BW=2000;
			FN=2000;
			// Auswahl der N,L Kombis beschränken
			//N=128;
			//L=8;
			break;    
		case 0:
			OFDM_Modem_DISABLE=1;
			BW = 32000;	// dummy initialization
		    FN = 32000;	// modem will terminate in OFDM_Modem.c
			break;
		
		default:
		    return 1; // exception return
	}
	
	
	
	switch (Config->spectrumPosition)
	{
	// f_corner_ist = 64kHz - BW/2
	// f_corner_soll= 48kHz+APLC
	// f_shift = f_corner_soll - f_corner_ist = 48+APLC-64+BW/2
		
		case Hz0: 		// no APLC
		    Df_Hz_RX = -16000.0f + 0.5f*(float)BW;
		    break; 
		case Hz4000:	// APLC  4kHz
		    Df_Hz_RX = -16000.0f + 0.5f*(float)BW + 4000.0f;
		    break;
		case Hz8000:	// APLC  8kHz
		    Df_Hz_RX = -16000.0f + 0.5f*(float)BW + 8000.0f;
		    break;
		case Hz12000:	// APLC 12kHz
		    Df_Hz_RX = -16000.0f + 0.5f*(float)BW +12000.0f;
		    break;
		default:
		    return 8; // exception return
	}
	
	//PL 2008 2kHz und 6kHz, zusätzlich nach oben schieben
	if (BW == 2000) {
		Df_Hz_RX += 2000.0f; // 2000Hz nach oben schieben, da above 2kHz (2+4, 2+8) Speech
		}
	if (BW == 6000) {
		Df_Hz_RX += 2000.0f; // 2000Hz nach oben schieben, da above 2kHz (2+4, 2+8) Speech
		}
		//!!if (Config->spectrumPosition == Hz12000 ) ERROR !!
		// Diese Verschiebung ist für BW2000 nicht spezifiziert, HMI sollte das managen

	
	Df_Hz_TX =0.0f; // for FE-Version with Filter and Mixer
					//	when mixing of OFDM signal is done in FE-DSP
	
//	Df_Hz_TX = Df_Hz_RX; // for old FE-Version without mixer 
	
	
	switch (Config->leftPilotPresent)
	{
		case pilot_not_present:
		// No ETL-Pilot in DPLC band
		break;
		
		case pilot_present:
		 // Treat special case: ETL-Pilot in DPLC Band
		    // reduced bandwidth, additional frequency shift
		
		 if (BW==2000) 
				{
				// Verschiebung des Spektrums um shift_OFDM
				// vgl. FE-DSP:
				//		FREQ_OFFSET_BW_1700	= 3309
				//		SSB_OFFSET			=  150
				// FE_DSP: shift = -16000 + (   aplc_BW       -2000 )	+ FREQ_OFFSET_BW_1890 - SSB_OFFSET
				//				 = -16000 + ((2000;6000;10000)-2000))	+       3259	      -  150
				//				 = -16000 +	(     0;4000;8000       )	+ 2000 +BW/2   +259   -  150
				//	OFDM:		 = -16000 + (     0;4000;8000       )	+ 2000 +BW/2  + shift_OFDM
				//	=> shift_OFDM = 309 - 150 = 159
				Df_Hz_RX+= 159.0f;
				}
		 if (BW==4000) 
		    	{
		    	BW=3500;
		    	// Spectrum is moved towards upper frequencies a little bit
		    	// away from ETL600-Pilot. The additional frequency shift
		    	// depends on N (resp. TX Filter selectivity)
		    	switch (BW4_8_Mode)
				{
				case std_FN:
		    		switch (N)
			    		{
			    		case 64: // used for Low Delay Version BW3.5 N=64 FN=4000
			    			//Df_Hz_TX+= 190.0f;
							Df_Hz_RX+= 272.0f;
			    		break;
			    			
			    		case 128: // used for High Delay Version BW3.5 N=128 FN=4000
							//Df_Hz_TX+= 210.0f;
							Df_Hz_RX+= 272.0f;
			    		break;
			    		
			    		case 256: // used for High Delay Version BW3.5 N=256 FN=4000
			    			//Df_Hz_TX+= 200.0f;
			    			Df_Hz_RX+= 272.0f;	
			    		break;
			    		
			    		case 512: // used for High Delay Version BW3.5 N=512 FN=4000
			    			//Df_Hz_TX+= 200.0f;
			    			Df_Hz_RX+= 272.0f;
			    		break;

				   		default:
				   		break;
		    			}
		    	break;
		    	case high_FN: //not used anymore
			    	switch (N)
			    		{
			    		case 64:	
//			    			Df_Hz_TX+= 260.0f;
//			    			Df_Hz_RX+= 260.0f;
			    			Df_Hz_RX+= 272.0f;
							//Df_Hz_RX+= 365.0f;
			    		break;
			    			
			    		case 128:
			    			Df_Hz_TX+= 210.0f;
			    			Df_Hz_RX+= 210.0f;
			    		break;
			    		
			    		case 256: // not optimized, maybe not used for 4KHz
			    			Df_Hz_TX+= 200.0f;
			    			Df_Hz_RX+= 200.0f;
			    		break;
			    		
			    		case 512: // not optimized, maybe not used for 4KHz
			    			Df_Hz_TX+= 200.0f;
			    			Df_Hz_RX+= 200.0f;
			    		break;
			    		default:
			    		break;
			    		}
			    break;		
		    	}
		    	}
		    if (BW==6000)
				{
				// Verschiebung des Spektrums um shift_OFDM
				// vgl. FE-DSP:
				//		FREQ_OFFSET_BW_5050	=  5357
				//		FREQ_OFFSET_BW_5308	=  5427 (+40 +60)
				//		SSB_OFFSET			=  150
				// FE_DSP: shift = -16000 + (   aplc_BW       -2000 )	+ FREQ_OFFSET_BW_5050 - SSB_OFFSET
				//				 = -16000 + ((2000;6000;10000)-2000))	+       5427	      -  150
				//				 = -16000 +	(     0;4000;8000       )	+ 2000 +BW/2   +427   -  150
				//	OFDM:		 = -16000 + (     0;4000;8000       )	+ 2000 +BW/2  + shift_OFDM
				//	=> shift_OFDM = 357 - 150 = 207
				//	=> shift_OFDM = 427 - 150 = 277 (+40=317, +60=337)
				//Df_Hz_RX+= 207.0f;
				Df_Hz_RX+= 317.0f;
				
				// Feinjustierung in Abhängigkeit von N
				switch (N)
		    		{
		    		case 64:	
		    			break;
		    			
		    		case 128:
		    			Df_Hz_TX-= 46.0f;
						Df_Hz_RX-= 46.0f;
		    		break;
		    		
		    		case 256: 
		    			Df_Hz_TX-= 66.0f;
						Df_Hz_RX-= 66.0f;
		    		break;
		    		
		    		case 512: 
		    			Df_Hz_TX-= 60.0f;
						Df_Hz_RX-= 60.0f;
		    		break;
		    		default:
		    		break;
		    		}
				}
		    if (BW==8000) 
		    	{
		    	BW=7500;
		    	// Spectrum is moved towards upper frequencies a little bit
		    	// away from ETL600-Pilot. The additional frequency shift
		    	// depends on N (resp. TX Filter selectivity)
		    	switch (BW4_8_Mode)
				{
				case std_FN:
			    	switch (N)
			    		{
			    		case 64: //not used	
			    			Df_Hz_TX+= 190.0f;
			    			Df_Hz_RX+= 190.0f;
			    		break;
			    			
			    		case 128: // used for High Delay Version BW7.5 N=128 FN=8000
//			    			Df_Hz_TX+= 210.0f;
//			    			Df_Hz_RX+= 210.0f;
			    			Df_Hz_RX+= 278.0f;
			    		break;
			    		
			    		case 256: // used for High Delay Version BW7.5 N=256 FN=8000
//			    			Df_Hz_TX+= 200.0f;
//			    			Df_Hz_RX+= 200.0f;
			    			Df_Hz_RX+= 278.0f;
			    		break;
			    		
			    		case 512: // used for High Delay Version BW7.5 N=512 FN=8000
//			    			Df_Hz_TX+= 200.0f;
//			    			Df_Hz_RX+= 200.0f;
			    			Df_Hz_RX+= 278.0f;
			    		break;
			    		default:
			    		break;
			    		}
			    break;
			    case high_FN:
			    	switch (N)
			    		{
			    		case 64:	
//			    			Df_Hz_TX+= 374.0f;
//			    			Df_Hz_RX+= 374.0f;
			    			Df_Hz_RX+= 278.0f; //not used. LowDelay uses also N=128
			    		break;
			    			
			    		case 128: // used for Low Delay Version BW7.5 N=128 FN=9600
			    			//Df_Hz_TX+= 250.0f; 
			    			Df_Hz_RX+= 278.0f; // for BW7.5 LowDelay N=128, L=7,16
			    		break;
			    		
			    		case 256: // not optimized, maybe not used for 8KHz
			    			Df_Hz_TX+= 200.0f;
			    			Df_Hz_RX+= 200.0f;
			    		break;
			    		
			    		case 512: // not optimized, maybe not used for 8KHz
			    			Df_Hz_TX+= 190.0f;
			    			Df_Hz_RX+= 190.0f;
			    		break;
			    		default:
			    		break;
			    		}
			    break;		
		    	}
		    	}
		    	
			if (BW==12000) 
		    	{
		    	BW=11500;
		    	// Spectrum is moved towards upper frequencies a little bit
		    	// away from ETL600-Pilot. The additional frequency shift
		    	// depends on N (resp. TX Filter selectivity)
		    	switch (N)
		    		{
		    		case 64:	
		    			Df_Hz_TX+= 188.0f;	//100.0f;
		    			Df_Hz_RX+= 188.0f;	//100.0f;
		    		break;
		    			
		    		case 128:
						Df_Hz_TX+= 188.0f;
		    			Df_Hz_RX+= 188.0f;
		    		break;
		    		
		    		case 256:
						Df_Hz_TX+= 188.0f;
		    			Df_Hz_RX+= 188.0f;	
		    		break;
		    		
		    		case 512:
						Df_Hz_TX+= 188.0f;
		    			Df_Hz_RX+= 188.0f;
		    		break;

		    		default:
		    		break;
		    		}
		    	}
			if (BW==16000)
				{
				// Spectrum is moved towards upper frequencies a little bit
		    	// away from ETL600-Pilot. The additional frequency shift
		    	// depends on N (resp. TX Filter selectivity)
				// BW 15500
				Df_Hz_TX+= 100.0f;
		    	Df_Hz_RX+= 100.0f;
				}
			if (BW==20000) 
		    	{
		    	// Spectrum is moved towards upper frequencies a little bit
		    	// away from ETL600-Pilot. The additional frequency shift
		    	// depends on N (resp. TX Filter selectivity)
		    	switch (N)
		    		{
		    		case 64:	
		    			Df_Hz_TX+= 400.0f;
		    			Df_Hz_RX+= 400.0f;
		    		break;
		    			
		    		case 128:
						Df_Hz_TX+= 400.0f;
						Df_Hz_RX+= 400.0f;
		    		break;
		    		
		    		case 256: 
		    			
		    		break;
		    		
		    		case 512: 
		    			
		    		break;
		    		default:
		    		break;
		    		}
		    	}
			if (BW==28000) 
		    	{
		    	// Spectrum is moved towards upper frequencies a little bit
		    	// away from ETL600-Pilot. The additional frequency shift
		    	// depends on N (resp. TX Filter selectivity)
		    	switch (N)
		    		{
		    		case 64:	
		    			Df_Hz_TX+= 500.0f;
		    			Df_Hz_RX+= 500.0f;
		    		break;
		    			
		    		case 128:
						Df_Hz_TX+= 500.0f;
						Df_Hz_RX+= 500.0f;
		    		break;
		    		
		    		case 256: 
		    			
		    		break;
		    		
		    		case 512: 
		    			
		    		break;
		    		default:
		    		break;
		    		}
		    	}
		break;
	}
	
// debug: Einstellung der Frequenzlage über HMI-Parameter
		if (Config->phaseControlParam[2] == 2000.0f) Df_Hz_RX = Config->phaseControlParam[3];
		if (Config->phaseControlParam[2] == 6000.0f) Df_Hz_RX = Config->phaseControlParam[3];
	
	
	switch (Config->mode_reg.TEST_Mode) //Test_Disable=0, Test1=1, Test2=2, Test3=3
	{
		case Test_Disable:  // NL Optimization=ON, SFC-Freq-Komp=OFF
			Optim_NL_State =  Optim_NL_ON_Determination;
			SFC_Freq_Comp  =  OFF;
		break;
		
		case Test1:			// NL Optimization=OFF, SFC-Freq-Komp=OFF
			Optim_NL_State =  Optim_NL_OFF;
			SFC_Freq_Comp  =  OFF;
		break;
		
		case Test2:
			Optim_NL_State =  Optim_NL_ON_Determination;
			SFC_Freq_Comp  =  ON;
		break;
		
		case Test3:
			Optim_NL_State =  Optim_NL_OFF;
			SFC_Freq_Comp  =  ON;
		break;
	}
	
	switch (Config->mode_reg.FB_FF_Mode) //unsigend int
	{
		case OFF:
			FB_FF_Flag = OFF;
		break;
		
		case ON:
			FB_FF_Flag = ON;
		break;
	}

// debug Activate NL-Optimization
//--------------------------------	
//Optim_NL_State =  Optim_NL_ON_Determination;
//Optim_NL_State =  Optim_NL_ON_Selected;
//Optim_NL_State =  Optim_NL_OFF;

	switch (Optim_NL_State)
	{
		case Optim_NL_OFF:
		 	// N and L was set by ORTH_Mode and GUARD_Int
		break;
		case Optim_NL_ON_Determination:
			// Start with N and L base values for NL-Optimization
			N = N_BASE;	
			L = L_BASE;
			break;
		case Optim_NL_ON_Selected:
			// Error, during initialization, modem should not come to this state
			return 8;
		
		default:
			return 8;
			
	}
	
		 
//	N = 256;  // N = 64, 128, 256, 512
//	L = 32;   // L = 8, 16, 32, 64. L/N <= 1/8

	// Check N,L Setting for allowed combinations in bandwidths with restrictions
	if ( (FN==12000)|(FN==24000)|(FN==9600)|(FN==6000)|(FN==4800) )
		{
		NL_CHECK_passed = 0;
		
		if (N==512)
					{ if ((L==64)|(L==16)) NL_CHECK_passed = 1; }
					
		if (N==256)
					{ if ((L==32)|(L==8))  NL_CHECK_passed = 1; }
		
		if (N==128)
					{ if ((L==16)|(L==7))  NL_CHECK_passed = 1; }
		
		if (N== 64)
					{ if  (L== 8)          NL_CHECK_passed = 1; }
					
		if (NL_CHECK_passed == 0) 
			{ // here to insert error message or warning
				return 4;  // exception return
			}
										
		}
		
		
	switch (Config->mode_reg.RS_Mode)
	{
		case ON:
			RS_Coding_Flag = 1;
			break;
		case OFF:
			RS_Coding_Flag = 0;
			break;
		default:
			// exception: error on input
		    return 5;
	}
	
	switch (Config->mode_reg.NB_Detection)
	{
		case ON:
			NB_Detection_Flag = 1;
			break;
		case OFF:
			NB_Detection_Flag = 0;
			break;
		default:
			// exception: error on input
		    return 6;
	}
	
	switch (Config->mode_reg.TO_Mode) //TO_Low=0, TO_High=1
	{
		case TO_Low:
			TimeOut_Time=0.3f;
		break;
		
		case TO_High:
			TimeOut_Time=1.5f;
		break;
		
		default: 
			TimeOut_Time= 4.0f;
		break;
	}
	
	FB_FF_Flag = OFF;
	switch (Config->mode_reg.DRate_Mode)
	{ //CMODE_DR0=0, CMODE_DR1=1, CMODE_DR2=2, CMODE_DR3=3, CMODE_DR4=4, CMODE_FBFF=7
		case CMODE_DR0:
			Data_Rate_Mode = 2;  //Fixed data rate
			Selected_data_rate_TX = (float) Config->dataRate[0];
			Selected_data_rate_RX = Selected_data_rate_TX;
			Selected_data_rate_IDX_prepared = 0;
			break;
		case CMODE_DR1:
			Data_Rate_Mode = 2;  //Fixed data rate
			Selected_data_rate_TX = (float) Config->dataRate[1];
			Selected_data_rate_RX = Selected_data_rate_TX;
			Selected_data_rate_IDX_prepared = 1;
			break;
		case CMODE_DR2:
			Data_Rate_Mode = 2;  //Fixed data rate
			Selected_data_rate_TX = (float) Config->dataRate[2];
			Selected_data_rate_RX = Selected_data_rate_TX;
			Selected_data_rate_IDX_prepared = 2;
			break;
		case CMODE_DR3:
			Data_Rate_Mode = 2;  //Fixed data rate
			Selected_data_rate_TX = (float) Config->dataRate[3];
			Selected_data_rate_RX = Selected_data_rate_TX;
			Selected_data_rate_IDX_prepared = 3;
			break;
		case CMODE_DR4:
			Data_Rate_Mode = 2;  //Fixed data rate
			Selected_data_rate_TX = (float) Config->dataRate[4];
			Selected_data_rate_RX = Selected_data_rate_TX;
			Selected_data_rate_IDX_prepared = 4;
			break; 
		case CMODE_FBFF:
			Data_Rate_Mode = 3;  //Full Optimization, Data rate out of List
			Selected_data_rate_TX = 0.0f; // Do not change 
			Selected_data_rate_RX = 0.0f;
			Selected_data_rate_IDX_prepared = 7;
			FB_FF_Flag = ON;
				break;
		default:
			// exception: error on input
		    return 7;
	}
	
	Number_of_Service_bits = 0; // Number of Service Bits per OFDM Symbol
	if (FB_FF_Flag)	Number_of_Service_bits++; // add one Service Bit for FallForward-Indicator
	
	
	return 0; // normal return, i.e. configuration has passed
		
}

