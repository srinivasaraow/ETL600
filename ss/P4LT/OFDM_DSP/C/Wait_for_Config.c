/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Wait_for_Config.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Wait for users modem configuration (via TDM bus)
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/C/Wait_for_Config.c $
 * 
 * 12    24.07.08 15:35 Chhamaa
 * 
 * 7     5.04.06 15:28 Chhamaa
 * 
 * 6     17.03.06 14:41 Chhamaa
 * 
 * 5     13.09.05 13:23 Ats
 * Exchange from TLC
 * 
 * 3     24.02.05 7:26 Chchlee
 * 
 * 2     7.02.05 14:47 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 4     22.12.04 9:30 Chchlee
 * 
 * 3     21.10.04 11:08 Maag01
 * 
 * 2     30.09.04 8:33 Leeb02
 * 
 * Initial version
 *       13.09.2004 PL
 * Restart of modem
 *       21.09.2004 vgp
*****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../h/OFDM_cfg_types.h"
#include "../h/OFDM_Defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/util.h"
#include "../h/TDM_Service.H"
#include "../h/List_of_data_rates.h"

t_OFDM_Config segment ( "seg_extn_init" ) Modem_Ext_Config;

void segment ( "seg_ext_pmco" ) Wait_for_Modem_Configuration()
	{
	
     int i;		
     
     void  Set_Alarm();
     void  Clear_Alarm();
     int   Get_Configuration(t_OFDM_Config *config);
     int   Verify_Modem_Configuration(t_OFDM_Config *);
     void  OFDM_Sync_to_TDM();


if (External_Config == 0) {
	// debug : for internal config only
	if ( Restart_Counter == 0 ) {
#if 0 // if communication with master DSP
      while (Get_Configuration(&Modem_Ext_Config) == 1) Set_Alarm();
      Clear_Alarm();
#endif
	} else {
 	  Set_Alarm();
      OFDM_Sync_to_TDM();
	  Clear_Alarm();	      			
	}
}
// end of debug

switch (External_Config)
	{
// ====================================================================
	case 1: // External Config (from Master DSP)
	
	    if ( Restart_Counter == 0 ) {
          while (Get_Configuration(&Modem_Ext_Config) == 1) Set_Alarm();
          if ( Verify_Modem_Configuration(&Modem_Ext_Config) == 0) {
   		    Clear_Alarm();
   	        vcopy_EE((float *)&Modem_Config, (float *)&Modem_Ext_Config,sizeof(t_OFDM_Config));
          } else {
   		    Set_Alarm();
   		    Error.Count++;
       		Error.Number[Error.Count].Location = ERLOC_CONFIG;
       		Error.Number[Error.Count].ID       = WRNG_NL_BW_CNFG;
       		dbg_error[Error.Count]=WRNG_NL_BW_CNFG;	
            exit(1);
          }
	    } else {
 		  Set_Alarm();
          OFDM_Sync_to_TDM();
		  Clear_Alarm();	      	
	    }
	break;
// ====================================================================
	
	case 0: // Internal Config  (for Debug Pupose)
	/*-----------------------------------------*
	**---    (Internal )User Settings   	---*
	*-----------------------------------------*/

// Debug 2008
Modem_Config.phaseControlParam[0] = 100.0f; // Verstärkungsfaktor für Phasenregelung
Modem_Config.phaseControlParam[1] = 900.0f;
//Modem_Config.phaseControlParam[2] = 2000.0f // BW: 2000.0f für Bandbreite 2kHz, sonst 0
Modem_Config.phaseControlParam[2] = 0.0f; // BW: 2000.0f für Bandbreite 2kHz, sonst 0
Modem_Config.phaseControlParam[3] = 0.0f; // Frequenzverschiebung für 2kHz

	Modem_Config.mode_reg.MODEM_Mode = OFDM_MASTER;
//	Modem_Config.mode_reg.MODEM_Mode = OFDM_SLAVE;
	
	Modem_Config.mode_reg.TEST_Mode = Test3;
	
//	Modem_Config.channelBW = 32000; // bandwidth
//	Modem_Config.channelBW = 28000; // bandwidth
//	Modem_Config.channelBW = 24000; // bandwidth
//	Modem_Config.channelBW = 20000; // bandwidth
//	Modem_Config.channelBW = 16000; // bandwidth
//	Modem_Config.channelBW = 12000; // bandwidth
//	Modem_Config.channelBW = 8000; // bandwidth !
	Modem_Config.channelBW = 6000; // bandwidth !
//	Modem_Config.channelBW = 4000; // bandwidth
//	Modem_Config.channelBW = 2000; // bandwidth


	Modem_Config.mode_reg.BW4_8_Mode = std_FN;
//	Modem_Config.mode_reg.BW4_8_Mode = high_FN;

	Modem_Config.mode_reg.ORTH_Mode = ORTH_Mode512;
//	Modem_Config.mode_reg.ORTH_Mode = ORTH_Mode256;
//	Modem_Config.mode_reg.ORTH_Mode = ORTH_Mode128;
//	Modem_Config.mode_reg.ORTH_Mode = ORTH_Mode64;

//	Modem_Config.mode_reg.GUARD_Int = GUARD_Int64;
//	Modem_Config.mode_reg.GUARD_Int = GUARD_Int32;
	Modem_Config.mode_reg.GUARD_Int = GUARD_Int16;
//	Modem_Config.mode_reg.GUARD_Int = GUARD_Int8;


//	Modem_Config.mode_reg.RS_Mode = ON;
	Modem_Config.mode_reg.RS_Mode = OFF;
	
//	Modem_Config.mode_reg.NB_Detection = ON;
	Modem_Config.mode_reg.NB_Detection = OFF;
	
	Modem_Config.mode_reg.TO_Mode = TO_Low;
	
	Modem_Config.mode_reg.FB_FF_Mode = OFF;
	
	// Hz0=0, Hz4000=1, Hz8000=2, Hz12000=3 
	Modem_Config.spectrumPosition = Hz0; 
//	Modem_Config.spectrumPosition = Hz4000; 
//	Modem_Config.spectrumPosition = Hz8000;
//	Modem_Config.spectrumPosition = Hz12000; 

	// ETL-Pilot in DPLC band	
//	Modem_Config.leftPilotPresent = pilot_not_present;
	Modem_Config.leftPilotPresent = pilot_present;
	


	// ====================Data_Rate_Mode=======================
	// =========================================================
	Modem_Config.dataRate[0] = 40000;
	Modem_Config.dataRate[1] = 256000;
	Modem_Config.dataRate[2] = 128000;
	Modem_Config.dataRate[3] = 288000;
	Modem_Config.dataRate[4] =  96000;
	         

	// ****************  Fixed *********************
  	// Fixed Rate Mode: CMODE_DR0=0, CMODE_DR1=1, CMODE_DR2=2, CMODE_DR3=3, CMODE_DR4=4,
	Modem_Config.mode_reg.DRate_Mode = CMODE_DR0;	         
//	Modem_Config.mode_reg.DRate_Mode = CMODE_DR2;	         
	// **************** Optim -> List **(Standard)**
	// FBFF-Mode (automatic out of list): CMODE_FBFF=7
 	//Modem_Config.mode_reg.DRate_Mode = CMODE_FBFF;      
	
	/* Verify modem configuration */
     if ( Verify_Modem_Configuration(&Modem_Config) != 0) {
     	Error.Count++;
       	Error.Number[Error.Count].Location = ERLOC_CONFIG;
       	Error.Number[Error.Count].ID       = WRNG_NL_BW_CNFG;
       	dbg_error[Error.Count]=WRNG_NL_BW_CNFG;	
       	exit(1);
     }
	/*-----------------------------------------*
	**---  End of (internal) User Settings	---*
	*-----------------------------------------*/
	break;
// ====================================================================

	} //switch external/internal


	// ========Full List or Short List (6 Rates) ==============================
 	 #if 0  // ***Full List
 	 // float    List_of_data_rates[];
 	 // List_of_data_rates[i] from header file: #include "../h/List_of_data_rates.h"
 	 //											float    List_of_data_rates[];
 	 // size of list from unsigend int Size_Of_Rate_List;
 	 #else  // ***Small List from Modem_Config
		Size_Of_Rate_List = N_DATA_RATES;
	 // Copy User data rate list 0..4 to internal data rate list 0..4
		for (i=0;i<N_DATA_RATES;i++)
			{
			List_of_data_rates[i] = (float)Modem_Config.dataRate[i];
			}
		if ( (Size_Of_Rate_List & 1) == 1 )  // if odd number of elements
         		{							 // copy last element
         		List_of_data_rates[N_DATA_RATES]=List_of_data_rates[N_DATA_RATES-1];
         		Size_Of_Rate_List++;
         		}
 	 #endif

		
		
}


