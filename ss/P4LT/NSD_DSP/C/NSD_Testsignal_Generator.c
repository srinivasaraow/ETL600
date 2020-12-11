/*----------------------------------------------------------------------------*
 * $Workfile: NSD_Testsignal_Generator.c $
 * Part of:		NSD550 (ETL500)
 * Language:	C
 * Created by:	P.Plueer
 * Remarks:		
 * Purpose:		
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/NSD_DSP/C/NSD_Testsignal_Generator.c $ *
 * 
 * 1     1.03.05 8:58 Chpeplu
 * Inital version prototyp 2
 * 
 * 4     1.02.05 15:56 Chpeplu
 * Test signal generator changed to produce I- and Q signal
 * 
 * 3     28.01.05 14:34 Chpeplu
 * function getNSDTestsignal_Gen_Konfig() changed
 * 
 * 2     28.01.05 8:15 Chpeplu
 * first running version of NSD Testsignal
 * 
 * 1     27.01.05 15:06 Chpeplu
 * Inital version not running
 * 
 *    Rev 1.6   24 Mar 1998 15:32:00   plueer
 * KeyCounter-DTMF und- MFC corrected
 * 
 *    Rev 1.5   23 Mar 1998 15:09:16   plueer
 * Correction of Alien test signal
 * 
 *    Rev 1.4   20 Mar 1998 15:33:48   plueer
 * konfig changed via mmi
 * 
 *    Rev 1.3   17 Mar 1998 13:37:06   plueer
 * Alien sweep added fix settings
 * 
 *    Rev 1.2   16 Mar 1998 11:17:54   plueer
 * NSD Testsignal MFC working
 * 
 *    Rev 1.1   12 Mar 1998 16:41:58   plueer
 * first running version with DTMF
 * 
 *    Rev 1.0   12 Mar 1998 10:55:34   plueer
 * inital version
------------------------------------------------------------------------------*/
#include "NSD_TEST.H"
#include <math.h>
#include "NSD_Types.h"
#include "CONSTANT.H"
//#include "NSD550.h"
 // data initialisation--------------------------------------------------------
 NSDTestDataConfig ConfigNSDTest= {
 								NO_TEST,	    	//No test selected
 								0.5,                // level DTMF
 								70,					// send time DTMF
 								70,					//break time DTMF
 								{0,1,2,3,4,5,6,7,8,8,10,11,12,13,14,15},	// send key DTMF
 								0.5,				// level MFC
 								100,				// send time MFC
 								100,				// break time MFC
 								{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},		// send kex MFC
 								FORWARD,			// MFC direction forward
 								0.5,				// level ALIEN
 								300.0,				// start freq.
 								2400.0,				// stop freq
 								(long)(100*(ONE_SECOND/ONE_MILESEC)),				// sweep time
 								DIR_UP_AND_DOWN,    // sweep direction up and down
 								1                   // change flag = true
 								};
 NSDTestDataStatus StatusNSDTest;
 // imported data -------------------------------------------------------------
 // local constantes ----------------------------------------------------------
#define	TX_FREQ_SHIFT_IN_HF1	2150
 
 // ---define frequencies for DTMF --------------------------------------------
 static float 	FREQ_DTMF_F1[ROW]= 
 { 1209.0, 1336.0, 1477.0, 1633.0};
 static float	FREQ_DTMF_F2[COLUMN]=
 { 697.0, 770.0, 852.0, 941.0};
 static float MFC_Freq_Tab[(NUMBER_OF_KEY - 1) * 2];
 //
 //
NSDTestDataConfig *	getNSDTestsignal_Gen_Konfig()
 {
	 return &ConfigNSDTest;
 }
 //****************************************************************************
 //*initNSD_Test() ************************************************************
 //**************************************************************************** 
 void SEGMENT_EXT_CODE initNSD_Testsignal(void)
 {
  //int	i_temp;
  // init config data no used after proper installation with MMI and modification in UART.C on DSPB 
  //
  // init kind of NSD550 test 
  //ConfigNSDTest.TestNumber = ALIEN;
  // reset change flag
  ConfigNSDTest.ChangeFlag = 0;
  //
  // init DTMF settings fix
  //ConfigNSDTest.LevelDTMF = 0.5;
  //ConfigNSDTest.SendTimeDTMF = 70;          			// in ms
  //ConfigNSDTest.BreakTimeDTMF = 70;                     // in ms
  //for(i_temp = 0; i_temp < NUMBER_OF_KEY; i_temp++)
  //{
  //	ConfigNSDTest.KeySelectDTMF[i_temp] = i_temp;
  //}
  //
  // init MFC settings fix
  //ConfigNSDTest.LevelMFC = 0.5;
  //ConfigNSDTest.MFC_Direction = FORWARD;				// set forward direction of signalling
  //ConfigNSDTest.SendTimeMFC = 100;						// in ms
  //ConfigNSDTest.BreakTimeMFC = 100;						// in ms
  
  // init Alien settings fix 
  //ConfigNSDTest.LevelAlien = 0.5; 
  //ConfigNSDTest.StartFreq = 300;                        // Startfreq. in Hz
  //ConfigNSDTest.StopFreq = 2400;                        // Stopfreq. in Hz
  //ConfigNSDTest.SweepTime = 100;						// time in sec
  //ConfigNSDTest.SweepDirection = DIR_UP_AND_DOWN;		// direction down
  //
  // init common status data for NSD Tests
  StatusNSDTest.TempArgF1[0] = 0.0;
  StatusNSDTest.TempArgF1[1] = 0.0;
  StatusNSDTest.TempArgF2[0] = 0.0; 
  StatusNSDTest.TempArgF2[1] = 0.0;
  StatusNSDTest.SignalF1_Q[0] = 0.0;
  StatusNSDTest.SignalF1_Q[1] = 0.0; 
  StatusNSDTest.SignalF2_Q[0] = 0.0;
  StatusNSDTest.SignalF2_Q[1] = 0.0;
  StatusNSDTest.SumSignal_Q[0] = 0.0;  
  StatusNSDTest.SumSignal_Q[1] = 0.0;
  //
  StatusNSDTest.SignalF1_I[0] = 0.0;
  StatusNSDTest.SignalF1_I[1] = 0.0; 
  StatusNSDTest.SignalF2_I[0] = 0.0;
  StatusNSDTest.SignalF2_I[1] = 0.0;
  StatusNSDTest.SumSignal_I[0] = 0.0;  
  StatusNSDTest.SumSignal_I[1] = 0.0;
  //
  // init DTMF Status
  StatusNSDTest.KeyCounterDTMF = 0;
  StatusNSDTest.StatusDTMF = START_UP;
  //
  // init MFC Status 
  StatusNSDTest.StatusMFC = START_UP_MFC;
  StatusNSDTest.KeyCounterMFC = 0;
  //for(i_temp = 0; i_temp < NUMBER_OF_KEY - 1; i_temp++)
  //{
  //	ConfigNSDTest.KeySelectMFC[i_temp] = i_temp;
  //} 
  // init MFC_freq table
  if(ConfigNSDTest.MFC_Direction == FORWARD)
   {
    // init MFC frequencies for FORWARD
    MFC_Freq_Tab[0] = F_MFC_F1-TX_FREQ_SHIFT_IN_HF1;     // Ziffer 1 f1
    MFC_Freq_Tab[1] = F_MFC_F2-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 1 f2 
    MFC_Freq_Tab[2] = F_MFC_F1-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 2 f1
    MFC_Freq_Tab[3] = F_MFC_F3-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 2 f2
    MFC_Freq_Tab[4] = F_MFC_F2-TX_FREQ_SHIFT_IN_HF1;     // Ziffer 3 f1
    MFC_Freq_Tab[5] = F_MFC_F3-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 3 f2
    
    MFC_Freq_Tab[6] = F_MFC_F1-TX_FREQ_SHIFT_IN_HF1;     // Ziffer 4 f1
    MFC_Freq_Tab[7] = F_MFC_F4-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 4 f2 
    MFC_Freq_Tab[8] = F_MFC_F2-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 5 f1
    MFC_Freq_Tab[9] = F_MFC_F4-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 5 f2
    MFC_Freq_Tab[10] = F_MFC_F3-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 6 f1
    MFC_Freq_Tab[11] = F_MFC_F4-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 6 f2 
    
    MFC_Freq_Tab[12] = F_MFC_F1-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 7 f1
    MFC_Freq_Tab[13] = F_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 7 f2 
    MFC_Freq_Tab[14] = F_MFC_F2-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 8 f1
    MFC_Freq_Tab[15] = F_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 8 f2
    MFC_Freq_Tab[16] = F_MFC_F3-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 9 f1
    MFC_Freq_Tab[17] = F_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 9 f2
    
    MFC_Freq_Tab[18] = F_MFC_F4-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 10 f1
    MFC_Freq_Tab[19] = F_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 10 f2 
    MFC_Freq_Tab[20] = F_MFC_F1-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 11 f1
    MFC_Freq_Tab[21] = F_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 11 f2
    MFC_Freq_Tab[22] = F_MFC_F2-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 12 f1
    MFC_Freq_Tab[23] = F_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 12 f2
    
    MFC_Freq_Tab[24] = F_MFC_F3-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 13 f1
    MFC_Freq_Tab[25] = F_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 13 f2 
    MFC_Freq_Tab[26] = F_MFC_F4-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 14 f1
    MFC_Freq_Tab[27] = F_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 14 f2
    MFC_Freq_Tab[28] = F_MFC_F5-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 15 f1
    MFC_Freq_Tab[29] = F_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 15 f2
   }
  else
   {
   	// init MFC frequencies for BACKWARD
    MFC_Freq_Tab[0] = B_MFC_F1-TX_FREQ_SHIFT_IN_HF1;     // Ziffer 1 f1
    MFC_Freq_Tab[1] = B_MFC_F2-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 1 f2 
    MFC_Freq_Tab[2] = B_MFC_F1-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 2 f1
    MFC_Freq_Tab[3] = B_MFC_F3-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 2 f2
    MFC_Freq_Tab[4] = B_MFC_F2-TX_FREQ_SHIFT_IN_HF1;     // Ziffer 3 f1
    MFC_Freq_Tab[5] = B_MFC_F3-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 3 f2
    
    MFC_Freq_Tab[6] = B_MFC_F1-TX_FREQ_SHIFT_IN_HF1;     // Ziffer 4 f1
    MFC_Freq_Tab[7] = B_MFC_F4-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 4 f2 
    MFC_Freq_Tab[8] = B_MFC_F2-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 5 f1
    MFC_Freq_Tab[9] = B_MFC_F4-TX_FREQ_SHIFT_IN_HF1;		// Ziffer 5 f2
    MFC_Freq_Tab[10] = B_MFC_F3-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 6 f1
    MFC_Freq_Tab[11] = B_MFC_F4-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 6 f2 
    
    MFC_Freq_Tab[12] = B_MFC_F1-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 7 f1
    MFC_Freq_Tab[13] = B_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 7 f2 
    MFC_Freq_Tab[14] = B_MFC_F2-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 8 f1
    MFC_Freq_Tab[15] = B_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 8 f2
    MFC_Freq_Tab[16] = B_MFC_F3-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 9 f1
    MFC_Freq_Tab[17] = B_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 9 f2
    
    MFC_Freq_Tab[18] = B_MFC_F4-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 10 f1
    MFC_Freq_Tab[19] = B_MFC_F5-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 10 f2 
    MFC_Freq_Tab[20] = B_MFC_F1-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 11 f1
    MFC_Freq_Tab[21] = B_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 11 f2
    MFC_Freq_Tab[22] = B_MFC_F2-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 12 f1
    MFC_Freq_Tab[23] = B_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 12 f2
    
    MFC_Freq_Tab[24] = B_MFC_F3-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 13 f1
    MFC_Freq_Tab[25] = B_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 13 f2 
    MFC_Freq_Tab[26] = B_MFC_F4-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 14 f1
    MFC_Freq_Tab[27] = B_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 14 f2
    MFC_Freq_Tab[28] = B_MFC_F5-TX_FREQ_SHIFT_IN_HF1;    // Ziffer 15 f1
    MFC_Freq_Tab[29] = B_MFC_F6-TX_FREQ_SHIFT_IN_HF1;	// Ziffer 15 f2
   }
 // init Alien Status 
 StatusNSDTest.SweepTimeCounterSet = ConfigNSDTest.SweepTime;
 StatusNSDTest.SweepTimeCounter = StatusNSDTest.SweepTimeCounterSet;
 StatusNSDTest.DeltaArgF1 = StatusNSDTest.SweepTempFreq  * 2 * PI_F /AF_SAMPLE_FREQ;
 switch(ConfigNSDTest.SweepDirection)
  {
   	case DIR_UP:
   		StatusNSDTest.Sign_Delta_Sweep = 1;
   		StatusNSDTest.SweepTempFreq = ConfigNSDTest.StartFreq;
   		break;
   	case DIR_DOWN:
   		StatusNSDTest.Sign_Delta_Sweep = -1;
   		StatusNSDTest.SweepTempFreq = ConfigNSDTest.StopFreq;
   		break;
   	case DIR_UP_AND_DOWN:
   		StatusNSDTest.Sign_Delta_Sweep = 1;
   		StatusNSDTest.SweepTempFreq = ConfigNSDTest.StartFreq;
   		break;
   	default:
   		break;
  } 
 StatusNSDTest.SweepDeltaFreq = StatusNSDTest.Sign_Delta_Sweep * (ConfigNSDTest.StopFreq - ConfigNSDTest.StartFreq) / StatusNSDTest.SweepTimeCounterSet;
 StatusNSDTest.StatusAlien = START_UP_ALIEN;

}
 
 
 //****************************************************************************
 //*NSD600_Tests() ************************************************************
 //**************************************************************************** 
 void SEGMENT_EXT_CODE NSD600_Testsignal(void)
 { 
  if(ConfigNSDTest.ChangeFlag == 1)
   {
   	initNSD_Testsignal();
   }
  else
   {
  	switch(ConfigNSDTest.TestNumber)
  	 {
  	 	case NO_TEST:
  	 		break;
  	 	case DTMF_TEST:
  	    	SM_DTMF();
  	 		break;
  	 	case MFC_TEST:
  	   	 	SM_MFC();
  	 		break;
  	 	case ALIEN:
  			SM_Alien();
  			break;
  		default:  
  	    	ConfigNSDTest.TestNumber = NO_TEST;
	  		break;
	 }
  }
}
 
 //****************************************************************************
 // *** SM_DTMF() *************************************************************
 // ***************************************************************************
 void SEGMENT_EXT_CODE SM_DTMF(void)
 {
 int temp1, row1, col1; 
 float	*ptrF1, *ptrF2;
   switch(StatusNSDTest.StatusDTMF)
   	{
   		case START_UP:
   			StatusNSDTest.SendTimeCounter = ConfigNSDTest.SendTimeDTMF;
   			StatusNSDTest.BreakTimeCounter = ConfigNSDTest.BreakTimeDTMF;
   			StatusNSDTest.StatusDTMF = SEND_KEY;
   			temp1 = ConfigNSDTest.KeySelectDTMF[StatusNSDTest.KeyCounterDTMF];
   			col1 = (int)(temp1/ROW);
   			row1 = temp1 - col1*ROW;
   			ptrF1 = FREQ_DTMF_F1;
   			ptrF1 += row1;
   			ptrF2 = FREQ_DTMF_F2;
   			ptrF2 += col1;
   			StatusNSDTest.DeltaArgF1 = (*ptrF1-TX_FREQ_SHIFT_IN_HF1) * 2 * PI_F /AF_SAMPLE_FREQ;
   			StatusNSDTest.DeltaArgF2 = (*ptrF2-TX_FREQ_SHIFT_IN_HF1) * 2 * PI_F /AF_SAMPLE_FREQ; 
   			StatusNSDTest.SendLevelF1 = ConfigNSDTest.LevelDTMF * DTMF_LEVEL_F1;
   			StatusNSDTest.SendLevelF2 = ConfigNSDTest.LevelDTMF * DTMF_LEVEL_F2;
   			break;
   		case SEND_KEY:
   			if(StatusNSDTest.SendTimeCounter >0) 
   			 {
   			  	StatusNSDTest.SendTimeCounter -=1;
   			  	if(StatusNSDTest.TempArgF1[0] > 10*PI_F)
   			  	 {
   			  	  	StatusNSDTest.TempArgF1[0] = StatusNSDTest.TempArgF1[0] - 10*PI_F;
   			  	  	StatusNSDTest.TempArgF1[1] = StatusNSDTest.TempArgF1[1] - 10*PI_F;
   			  	 }
   			  	else
   			  	 { 
   			  	   	if(StatusNSDTest.TempArgF2[0] > 10*PI_F)
   			  	   	 {
   			  	   	  	StatusNSDTest.TempArgF2[0] = StatusNSDTest.TempArgF2[0] - 10*PI_F;
   			  	  		StatusNSDTest.TempArgF2[1] = StatusNSDTest.TempArgF2[1] - 10*PI_F;
   			  	   	 }
   			  	   	else
   			  	   	 {
   			  	   	 }
   			  	 }
   			 }
   			else
   			 {
   			  	StatusNSDTest.StatusDTMF = BREAK_S;
   			 }
   			break;
   		case BREAK_S:
   			if(StatusNSDTest.BreakTimeCounter > 0)
   			 {
   			  	StatusNSDTest.BreakTimeCounter -=1;
   			 }
   			else
   			 {
   			 	StatusNSDTest.StatusDTMF = GET_NEXT_KEY;
   			 	StatusNSDTest.TempArgF1[0] = 0.0;
   			 	StatusNSDTest.TempArgF1[1] = 0.0;
  				StatusNSDTest.TempArgF2[0] = 0.0;
  				StatusNSDTest.TempArgF2[1] = 0.0;	
   			 } 
   			break;
   		case GET_NEXT_KEY:
   			StatusNSDTest.SendTimeCounter = ConfigNSDTest.SendTimeDTMF;
   			StatusNSDTest.BreakTimeCounter = ConfigNSDTest.BreakTimeDTMF;
   			if(StatusNSDTest.KeyCounterDTMF < NUMBER_OF_KEY - 1) 
   			 {
   			  	StatusNSDTest.KeyCounterDTMF +=1;
   			 }
   			else
   			 {
   			  	StatusNSDTest.KeyCounterDTMF = 0;
   			 }
   			temp1 = ConfigNSDTest.KeySelectDTMF[StatusNSDTest.KeyCounterDTMF];
   			col1 = (int)(temp1/ROW);
   			row1 = temp1 - col1*ROW;
   			ptrF1 = FREQ_DTMF_F1;
   			ptrF1 += row1;
   			ptrF2 = FREQ_DTMF_F2;
   			ptrF2 += col1;
   			StatusNSDTest.DeltaArgF1 = (*ptrF1-TX_FREQ_SHIFT_IN_HF1) * 2 * PI_F /AF_SAMPLE_FREQ;
   			StatusNSDTest.DeltaArgF2 = (*ptrF2-TX_FREQ_SHIFT_IN_HF1) * 2 * PI_F /AF_SAMPLE_FREQ;
   			StatusNSDTest.StatusDTMF = SEND_KEY;
   			break;
   		default:
   		    StatusNSDTest.StatusDTMF = START_UP;
   			break;
   
   	}
 }
 // ***************************************************************************
 // *** SM_MFC() **************************************************************
 // ***************************************************************************
 void SEGMENT_EXT_CODE SM_MFC(void)
 {
 int temp1;
  switch(StatusNSDTest.StatusMFC)
   {
    	case START_UP_MFC:
    		StatusNSDTest.SendTimeCounter = ConfigNSDTest.SendTimeMFC;
   			StatusNSDTest.BreakTimeCounter = ConfigNSDTest.BreakTimeMFC;
   			StatusNSDTest.StatusMFC = SEND_MFC;
   			temp1 = ConfigNSDTest.KeySelectMFC[StatusNSDTest.KeyCounterMFC];
   			StatusNSDTest.DeltaArgF1 = MFC_Freq_Tab[(2*temp1)] * 2 * PI_F /AF_SAMPLE_FREQ;
   			StatusNSDTest.DeltaArgF2 = MFC_Freq_Tab[(2*temp1)+1] * 2 * PI_F /AF_SAMPLE_FREQ;
    		StatusNSDTest.SendLevelF1 = ConfigNSDTest.LevelMFC;
    		StatusNSDTest.SendLevelF2 = ConfigNSDTest.LevelMFC;
    		break;
    	case SEND_MFC:
    		if(StatusNSDTest.SendTimeCounter >0) 
   			 {
   			  	StatusNSDTest.SendTimeCounter -=1;
   			  	if(StatusNSDTest.TempArgF1[0] > 10*PI_F)
   			  	 {
   			  	  	StatusNSDTest.TempArgF1[0] = StatusNSDTest.TempArgF1[0] - 10*PI_F;
   			  	  	StatusNSDTest.TempArgF1[1] = StatusNSDTest.TempArgF1[1] - 10*PI_F;
   			  	 }
   			  	else
   			  	 { 
   			  	   	if(StatusNSDTest.TempArgF2[0] > 10*PI_F)
   			  	   	 {
   			  	   	  	StatusNSDTest.TempArgF2[0] = StatusNSDTest.TempArgF2[0] - 10*PI_F;
   			  	  		StatusNSDTest.TempArgF2[1] = StatusNSDTest.TempArgF2[1] - 10*PI_F;
   			  	   	 }
   			  	   	else
   			  	   	 {
   			  	   	 }
   			  	 }
   			 }
   			else
   			 {
   			  	StatusNSDTest.StatusMFC = BREAK_MFC;
   			 }
    		break;
    	case BREAK_MFC:
    		if(StatusNSDTest.BreakTimeCounter > 0)
   			 {
   			  	StatusNSDTest.BreakTimeCounter -=1;
   			 }
   			else
   			 {
   			 	StatusNSDTest.StatusMFC = GET_NEXT_MFC;
   			 	StatusNSDTest.TempArgF1[0] = 0.0;
   			 	StatusNSDTest.TempArgF1[1] = 0.0;
  				StatusNSDTest.TempArgF2[0] = 0.0;
  				StatusNSDTest.TempArgF2[1] = 0.0;	
   			 } 
    		break; 
    	case GET_NEXT_MFC:
    		StatusNSDTest.SendTimeCounter = ConfigNSDTest.SendTimeMFC;
   			StatusNSDTest.BreakTimeCounter = ConfigNSDTest.BreakTimeMFC;
   			StatusNSDTest.StatusMFC = SEND_MFC;
   			if(StatusNSDTest.KeyCounterMFC < (NUMBER_OF_KEY - 2)) 
   			 {
   			  	StatusNSDTest.KeyCounterMFC +=1;
   			 }
   			else
   			 {
   			  	StatusNSDTest.KeyCounterMFC = 0;
   			 }
   			temp1 = ConfigNSDTest.KeySelectMFC[StatusNSDTest.KeyCounterMFC];
   			StatusNSDTest.DeltaArgF1 = MFC_Freq_Tab[(2*temp1)] * 2 * PI_F /AF_SAMPLE_FREQ;
   			StatusNSDTest.DeltaArgF2 = MFC_Freq_Tab[(2*temp1)+1] * 2 * PI_F /AF_SAMPLE_FREQ;
    		break;
    	default:
    		StatusNSDTest.StatusMFC = START_UP_MFC;
    		break;
    
   }
 
 }
 // ***************************************************************************
 // *** SM_Alien() ************************************************************
 // ***************************************************************************
 void SEGMENT_EXT_CODE SM_Alien(void)
 {
  switch(StatusNSDTest.StatusAlien)
  {
   	case START_UP_ALIEN:
   		StatusNSDTest.SweepTimeCounterSet = ConfigNSDTest.SweepTime;
 		StatusNSDTest.SweepTimeCounter = StatusNSDTest.SweepTimeCounterSet;
 		StatusNSDTest.DeltaArgF1 = (StatusNSDTest.SweepTempFreq-TX_FREQ_SHIFT_IN_HF1)  * 2 * PI_F /AF_SAMPLE_FREQ;
 		StatusNSDTest.SendLevelF1 = ConfigNSDTest.LevelAlien;
 		StatusNSDTest.SendLevelF2 = ConfigNSDTest.LevelAlien;
 		switch(ConfigNSDTest.SweepDirection)
  		{
   			case DIR_UP:
   				StatusNSDTest.Sign_Delta_Sweep = 1;
   				StatusNSDTest.SweepTempFreq = ConfigNSDTest.StartFreq;
   				break;
   			case DIR_DOWN:
   				StatusNSDTest.Sign_Delta_Sweep = (-1);
   				StatusNSDTest.SweepTempFreq = ConfigNSDTest.StopFreq;
   				break;
   			case DIR_UP_AND_DOWN:
   				StatusNSDTest.Sign_Delta_Sweep = 1;
   				StatusNSDTest.SweepTempFreq = ConfigNSDTest.StartFreq;
   				break;
   			default:
   				break;
  		}
  		StatusNSDTest.SweepDeltaFreq = StatusNSDTest.Sign_Delta_Sweep * (ConfigNSDTest.StopFreq - ConfigNSDTest.StartFreq) / StatusNSDTest.SweepTimeCounterSet;  
 		StatusNSDTest.StatusAlien = SEND_ALIEN;
   		break;
   	case SEND_ALIEN:
   		if(StatusNSDTest.SweepTimeCounter > 0)
   		 {
   		  	StatusNSDTest.SweepTimeCounter -=1;
   		  	StatusNSDTest.SweepTempFreq = StatusNSDTest.SweepTempFreq + StatusNSDTest.SweepDeltaFreq;
   		  	StatusNSDTest.DeltaArgF1 = (StatusNSDTest.SweepTempFreq-TX_FREQ_SHIFT_IN_HF1)  * 2 * PI_F /AF_SAMPLE_FREQ;
   		  	if(StatusNSDTest.TempArgF1[0] > 10*PI_F)
   			 {
   			  	  	StatusNSDTest.TempArgF1[0] = StatusNSDTest.TempArgF1[0] - 10*PI_F;
   			  	  	StatusNSDTest.TempArgF1[1] = StatusNSDTest.TempArgF1[1] - 10*PI_F;
   			 }
   			else
   			 {
   			 }	
   		 }
   		else
   		 {
   		 	StatusNSDTest.StatusAlien = RESTART_SWEEP;
   		 }
   		break;
   	case RESTART_SWEEP:
   		StatusNSDTest.SweepTimeCounter = StatusNSDTest.SweepTimeCounterSet;
 		StatusNSDTest.DeltaArgF1 = (StatusNSDTest.SweepTempFreq-TX_FREQ_SHIFT_IN_HF1)  * 2 * PI_F /AF_SAMPLE_FREQ; 
 		StatusNSDTest.TempArgF1[0] = 0.0;
  		StatusNSDTest.TempArgF1[1] = 0.0;
  		switch(ConfigNSDTest.SweepDirection)
  		{
   			case DIR_UP:
   				StatusNSDTest.Sign_Delta_Sweep = 1;
   				StatusNSDTest.SweepTempFreq = ConfigNSDTest.StartFreq;
   				break;
   			case DIR_DOWN:
   				StatusNSDTest.Sign_Delta_Sweep = -1;
   				StatusNSDTest.SweepTempFreq = ConfigNSDTest.StopFreq;
   				break;
   			case DIR_UP_AND_DOWN:
   				StatusNSDTest.Sign_Delta_Sweep *= (-1);
   				break;
   			default:
   				break;
  		}
  		StatusNSDTest.SweepDeltaFreq = StatusNSDTest.Sign_Delta_Sweep * (ConfigNSDTest.StopFreq - ConfigNSDTest.StartFreq) / StatusNSDTest.SweepTimeCounterSet; 
 		StatusNSDTest.StatusAlien = SEND_ALIEN;
   		break;
   	default:
   		StatusNSDTest.StatusAlien = START_UP_ALIEN;
   		break;
  }	
 }
 // ***************************************************************************
 // *** Sig_Gen() *************************************************************
 // ***************************************************************************
 void SEGMENT_EXT_CODE Testsignal_Gen(float *Tx_Test_Signal_Q, float *Tx_Test_Signal_I )
 {
  if((StatusNSDTest.StatusDTMF == SEND_KEY) | (StatusNSDTest.StatusMFC == SEND_MFC))
   {
    	StatusNSDTest.TempArgF1[0] = StatusNSDTest.DeltaArgF1 + StatusNSDTest.TempArgF1[1];
    	StatusNSDTest.TempArgF1[1] = StatusNSDTest.DeltaArgF1 + StatusNSDTest.TempArgF1[0];
    	 
    	StatusNSDTest.SignalF1_Q[0] = sinf(StatusNSDTest.TempArgF1[0])*StatusNSDTest.SendLevelF1;
    	StatusNSDTest.SignalF1_Q[1] = sinf(StatusNSDTest.TempArgF1[1])*StatusNSDTest.SendLevelF1; 
    	
		StatusNSDTest.SignalF1_I[0] = cosf(StatusNSDTest.TempArgF1[0])*StatusNSDTest.SendLevelF1;
    	StatusNSDTest.SignalF1_I[1] = cosf(StatusNSDTest.TempArgF1[1])*StatusNSDTest.SendLevelF1; 
    	

    	StatusNSDTest.TempArgF2[0] = StatusNSDTest.DeltaArgF2 + StatusNSDTest.TempArgF2[1];
    	StatusNSDTest.TempArgF2[1] = StatusNSDTest.DeltaArgF2 + StatusNSDTest.TempArgF2[0];
    	 
    	StatusNSDTest.SignalF2_Q[0] = sinf(StatusNSDTest.TempArgF2[0])*StatusNSDTest.SendLevelF2;
    	StatusNSDTest.SignalF2_Q[1] = sinf(StatusNSDTest.TempArgF2[1])*StatusNSDTest.SendLevelF2; 
    	
		StatusNSDTest.SignalF2_I[0] = cosf(StatusNSDTest.TempArgF2[0])*StatusNSDTest.SendLevelF2;
    	StatusNSDTest.SignalF2_I[1] = cosf(StatusNSDTest.TempArgF2[1])*StatusNSDTest.SendLevelF2; 
    	

    	StatusNSDTest.SumSignal_Q[0] = StatusNSDTest.SignalF2_Q[0] + StatusNSDTest.SignalF1_Q[0];
    	StatusNSDTest.SumSignal_Q[1] = StatusNSDTest.SignalF2_Q[1] + StatusNSDTest.SignalF1_Q[1];

		StatusNSDTest.SumSignal_I[0] = StatusNSDTest.SignalF2_I[0] + StatusNSDTest.SignalF1_I[0];
    	StatusNSDTest.SumSignal_I[1] = StatusNSDTest.SignalF2_I[1] + StatusNSDTest.SignalF1_I[1];
   }
  else
   {
    	if(StatusNSDTest.StatusAlien == SEND_ALIEN)
    	 {
    	  	StatusNSDTest.TempArgF1[0] = StatusNSDTest.DeltaArgF1 + StatusNSDTest.TempArgF1[1];
    		StatusNSDTest.TempArgF1[1] = StatusNSDTest.DeltaArgF1 + StatusNSDTest.TempArgF1[0];
    	 
    		StatusNSDTest.SumSignal_Q[0] = sinf(StatusNSDTest.TempArgF1[0])*StatusNSDTest.SendLevelF1;
    		StatusNSDTest.SumSignal_Q[1] = sinf(StatusNSDTest.TempArgF1[1])*StatusNSDTest.SendLevelF1;

			StatusNSDTest.SumSignal_I[0] = cosf(StatusNSDTest.TempArgF1[0])*StatusNSDTest.SendLevelF1;
    		StatusNSDTest.SumSignal_I[1] = cosf(StatusNSDTest.TempArgF1[1])*StatusNSDTest.SendLevelF1;
    	 }
    	else
    	 {
    	 
    		StatusNSDTest.SignalF1_Q[0] = 0.0;
    		StatusNSDTest.SignalF1_Q[1] = 0.0;
    		StatusNSDTest.SignalF2_Q[0] = 0.0;
    		StatusNSDTest.SignalF2_Q[1] = 0.0;
    		StatusNSDTest.SumSignal_Q[0] = 0.0;
    		StatusNSDTest.SumSignal_Q[1] = 0.0;

			StatusNSDTest.SignalF1_I[0] = 0.0;
    		StatusNSDTest.SignalF1_I[1] = 0.0;
    		StatusNSDTest.SignalF2_I[0] = 0.0;
    		StatusNSDTest.SignalF2_I[1] = 0.0;
    		StatusNSDTest.SumSignal_I[0] = 0.0;
    		StatusNSDTest.SumSignal_I[1] = 0.0;
    	 }
   }
	*Tx_Test_Signal_Q++ = StatusNSDTest.SumSignal_Q[0];
	*Tx_Test_Signal_Q = StatusNSDTest.SumSignal_Q[1];
	//
	*Tx_Test_Signal_I++ = StatusNSDTest.SumSignal_I[0];
	*Tx_Test_Signal_I = StatusNSDTest.SumSignal_I[1];
 }

