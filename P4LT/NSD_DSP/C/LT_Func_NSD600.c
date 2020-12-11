/*-------------------------------------------------------------------------*
 * $Workfile: LT_Func_NSD600.c $
 * Part of	:	NSD600
 * Language	:	C
 * Created by:	Alexander Gygax(NSD570) / Peter Plüer (NSD600)
 * Remarks	:   Version 1.28 from NSD570 analog used for NSD600
 * Purpose	:	Functions for Loop Test Handling
 *-------------------------------------------------------------------------*/                                
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSD_DSP/C/LT_Func_NSD600.c $ 
 * 
 * 25    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 21    14.11.06 13:36 Chpeplu
 * Used define COM_T replaced by RX_COM_T oder TX_COM_T, Function
 * Init_Cyclic_LT() changed for custom mode, Function LT_Repeater()
 * changed for custom mode
 * 
 * 20    20.09.06 16:25 Chhamaa
 * 
 * 19    1.09.06 12:51 Chhamaa
 * ALL_LP_AES_TX_COMMANDS used in LT_Repeater()
 * 
 * 18    21.08.06 17:28 Chhamaa
 * In func. ServeManualLoopTest_byHMI() loop test for AES enabled.
 * 
 * 17    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 17    7.07.06 18:50 Chhamaa
 * 
 * 14    31.01.06 16:25 Chpeplu
 * new mode INT_TELPROT_MODE_HSD supported
 * 
 * 12    28.10.05 8:11 Chchlee
 * some ASSERTs removed
 * 
 * 11    30.06.05 8:35 Chpeplu
 * Code walkthrough Pl
 * 
 * 10    22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 9     2.06.05 9:46 Chpeplu
 * New event MANUAL_LT_FAILED_EVENT added 
 * 
 * 8     1.06.05 16:38 Chpeplu
 * Define CORRECTION_FAC_COUNTER changed
 * 
 * 7     27.05.05 14:09 Chpeplu
 * LT start time for first cyclic loop test set to 10 minutes after start
 * up an littel delay for Ch2 first loop test added
 * 
 * 6     23.05.05 17:01 Chpeplu
 * LT-Events added
 * 
 * 5     14.03.05 13:28 Chpeplu
 * Changes made for up to six LP commands
 * 
 * 4     9.03.05 10:26 Chpeplu
 * New config used (NsdMode)
 * 
 * 3     4.03.05 15:23 Chpeplu
 * Prepared for NewConfigNSD without custom table
 * 
 * 2     1.03.05 12:03 Chpeplu
 * Cyclic loop test time set to hours, manual loop test failed or cancel
 * parameter correct set for HMI600 -> first loop test set to 1minute
 * instead of 10 minutes for tests
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 8     11.01.05 10:30 Chpeplu
 * New function added Update_Dev_Status_LT(); Channel_Active added in
 * structs Loop_TestCh1/2; Some comment added;
 * 
 * 7     7.12.04 11:55 Chpeplu
 * In function Determine_LT_Cases() update of NSD Status data (Loop test
 * duration, Alarms, Warnings) added
 * 
 * 6     19.11.04 7:37 Plueer01
 * Add function ServeManualLoopTest_byHMI for manal loop test by HMI
 * 
 * 5     2.11.04 14:30 Plueer01
 * Some modifications made ( additional defines added instead of
 * constatns,...)
 * 
 * 4     27.10.04 15:40 Plueer01
 * Some changes done for Loop test limits
 * 
 * 3     26.10.04 17:39 Plueer01
 * All functions adapeted for ETL600 and first tests -> don't work correct
 * 
 * 2     22.10.04 16:47 Plueer01
 * back up changes it does not work yet
 * 
 * 1     22.10.04 11:23 Plueer01
 *-------------------------------------------------------------------------*
 */      
/* INCLUDE FILES *************************************************************/    
#include "NSD_DSP_Types.h"
#include "NSD_Types.h"
#include "NSD_DSP_Main.h"
#include "NSD600_LT_Constants.h"
#include "CONSTANT.h"
#include "NSD_Transmitter.h"
#include "P4LT_Types.h"
#include "AES600.h"
#include "diag.h"
//
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/***************************************************************************/
/*------------------- Constants for 100ms state machine -------------------*/
/***************************************************************************/

#define WAIT_AFTER_STARTUP			0
#define LT_IDLE						1
#define MANUAL_LT_RUNNING			2
#define	CYCLIC_LT_RUNNING			3
#define	WAIT_AFTER_FAILED_CYCLIC_LT	4
#define	LED_PASSED_LT				5
#define	LED_FAILED_LT				6

/***************************************************************************/
/*---------------------- Constant for Rx/TxCommand ------------------------*/
/***************************************************************************/

#define CYCLIC_LT_1H				1
#define	CYCLIC_LT_3H				3
#define	CYCLIC_LT_6H				6
#define	CYCLIC_LT_12H				12
#define	CYCLIC_LT_24H				24

// Common_State bit fields for LT handler
#define LOOP_TEST_BUTTON	BIT0
#define LT_ACTIVATED		BIT1
#define LT_PASSED			BIT2
#define LT_STOPPED			BIT3

// states for State_LT_Repeater
#define REPEAT_IDLE_STATE	BIT0
#define	REPEAT_ACTIVE_STATE	BIT1
//
// LT alarms
#define LOOP_TEST_ERROR		0x00000001
// states for Manual_LT_State
#define M_LT_RUNNING		0x0000000b
#define M_LT_END			0x0000000c
#define M_LT_IDLE			0x0000000d


#define SECONDS_PER_HOUR			3600

#define	MAINCASE_TIME				(1000/1066.667)

#define SUBCASE_100MS_CORRECTION	100
//
#define CORRECTION_FAC_COUNTER		(10666.667/10000)
//
#define THREE_SECONDS_100MS_CASE	(30*CORRECTION_FAC_COUNTER)
#define ONE_MINUTE_100MS_CASE		(600*CORRECTION_FAC_COUNTER)
#define FIVE_MINUTES_100MS_CASE		(3000*CORRECTION_FAC_COUNTER)
//
//START_FIRST_LT if cyclic loop test is enable in xy minutes
// !!!!!!!!!!-> should be set to 10 Minutes according to TRS after start up -> to @Pl@ !!!!!!!!!!!!!!!!!!
#define START_FIRST_LT				10
//start up delay first loop test in channel 2 after channel 1 
// otherwise both loop test are send simultaneous
// start up time ch2 = START_FIRST_LT * START_UP_DELAY_CH2
#define	START_UP_DELAY_CH2			1.02
// block time after a loop test was send for the loop test repeater ca 3 sec
#define	LT_REPEATER_BLOCK_TIME		(3000*CORRECTION_FAC_COUNTER)
//
// T0 time for NSD600 commands in ms -> must be measured 
#define T0_NSD600_MEASURED			12
#define T01_AES600_MEASURED			16
#define T02_AES600_MEASURED			26
//
#define T0_NSD600_MAINCASE_TIME		(T0_NSD600_MEASURED*CORRECTION_FAC_COUNTER)
// T0 = 12ms -> Send time loop test = 3 * T0 = 3*12ms = 36ms
#define T01_AES600_MAINCASE_TIME		(T01_AES600_MEASURED*CORRECTION_FAC_COUNTER)
#define T02_AES600_MAINCASE_TIME		(T02_AES600_MEASURED*CORRECTION_FAC_COUNTER)

#define	SEND_TIME_LOOPTEST			3
// Loop test window max time = LOOPTEST_WINDOW * T0 = 10 * T0 = 120ms
#define LOOPTEST_WINDOW				10
//
// minimum looptest time for successfull loop test 
// actual value -> local loop will be possible without alarm if t(looptest) < 0.5*T0
#define MINIMUM_LOOPTESTTIME		0.5
//
/***************************************************************************/
/*-------------------- Constants for Relay Interface ----------------------*/
/***************************************************************************/
// Input output delay G4AI 723us
#define	RELAY_INOUT_DELAY			(723*CORRECTION_FAC_COUNTER)
//
/* LOCAL TYPE DECLARATIONS ***************************************************/
// 
// *************************************************************************
// define structs
// *************************************************************************
typedef struct {
	BOOL Channel_Active;			// on or off switch depending on configuration
	long Cyclic_LT_Time;			// cyclic looptest time for subcase  0.010666kHz
	long LT_Tev_max;				// 
	long LT_Tev_min;				// minimum time for Loop test successfull
	long State_100ms_LT;			// states for 100ms task in function Determine_LT_Cases()
	long State_Timer_LT;			// Timer for 100ms case used in function Determine_LT_Cases()
	long Start_up_Time;				// use that not both NSD do the cyclic loop test simultaneous

	long LT_Fast_Counter;			// timer for Loop test transmission time in us
	long LoopTest_Duration;			// time for HMI600 in us
	long Common_State;				// bit wise 
	long Counter_LT_TX_RX;			// for tests only Tx and Rx counters

	long State_LT_Repeater;			// State for the loop test repeater
	long Send_time_LT;				//
	long LT_Wait_Counter;			// waite counter for LT repeater and Loop test sender 
	int Number_Of_Tries;
	long LT_Alarms;					// Alarm for HMI600

	long Manual_LT_State;			// state for manual loop test started by HMI600

} t_Loop_Test;

//
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
/* PUBLIC VARIABLE DECLARATION ***********************************************/
//
// used variable for NSD600
t_Loop_Test Loop_TestCh1, Loop_TestCh2;

/*****************************************************************************
  Function:		ServeManualLoopTest_byHMI(unsigned long *ptr_LoopTestMode, unsigned long *ptr_LT_Duration)
  Description:	 
  Inputs:		unsigned long *ptr_LoopTestMode (input from HMI 1-> start LT-TPE1; 2-> start LT-TPE2) 
  Outputs:		unsigned long *ptr_LT_Duration
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
// ***********************************************************************
// *** check Manual Loop test set by HMI  ********************************
// ***********************************************************************
void SEGMENT_EXT_CODE ServeManualLoopTest_byHMI(unsigned long *ptr_LoopTestMode, unsigned long *ptr_LT_Duration)
{
	if ((*ptr_LoopTestMode == LOOP_TEST_NSD_TPE1) || (*ptr_LoopTestMode == LOOP_TEST_AES_TPE1))
	{
		if(Loop_TestCh1.Channel_Active == ON)
		{
			// loop test started for Ch1 by HMI600
			switch(Loop_TestCh1.Manual_LT_State)
			{
			case M_LT_IDLE:
				Loop_TestCh1.Manual_LT_State = M_LT_RUNNING;
				Loop_TestCh1.Common_State |= LOOP_TEST_BUTTON;
				*ptr_LT_Duration = 0; // reset loop test time
				//
				break;
			case M_LT_RUNNING:
				// do nothing for for Loop test end set by function Determine_LT_Cases()
				break;
			case M_LT_END:
				*ptr_LT_Duration = Loop_TestCh1.LoopTest_Duration;
				*ptr_LoopTestMode = 0;
				Loop_TestCh1.Manual_LT_State = M_LT_IDLE;
				break;
			default:
				ASSERT(FALSE);
			}
		}
		else
		{
			// channel 1 is switched off -> do nothing
		}
	}
	else
	{
		if(*ptr_LoopTestMode == LOOP_TEST_NSD_TPE2)
		{
			if(Loop_TestCh2.Channel_Active == ON)
			{
				switch(Loop_TestCh2.Manual_LT_State)
				{
				case M_LT_IDLE:
					Loop_TestCh2.Manual_LT_State = M_LT_RUNNING;
					Loop_TestCh2.Common_State |= LOOP_TEST_BUTTON;
					*ptr_LT_Duration = 0; // reset loop test time
				//
					break;
				case M_LT_RUNNING:
					// do nothing for for Loop test end set by function Determine_LT_Cases()
					break;
				case M_LT_END:
					*ptr_LT_Duration = Loop_TestCh2.LoopTest_Duration;
					*ptr_LoopTestMode = 0;
					Loop_TestCh2.Manual_LT_State = M_LT_IDLE;
					break;
				default:
					ASSERT(FALSE);
				}
			}
			else
			{
				// channel 2 is switched off -> do nothing
			}
		}
		else
		{
			// no loop test start by hmi
		}
	}
}

/*****************************************************************************
  Function:		void Determine_LT_Cases(long TxCommand, long RxCommand,t_NSD_Status *ptr_NSD_Status, unsigned long *ptr_LT_Event, long Channel)
  Description:	 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
void SEGMENT_EXT_CODE Determine_LT_Cases(long TxCommand, long RxCommand,t_NSD_Status *ptr_NSD_Status, unsigned long *ptr_LT_Event, long Channel)
{
	long temp_counter = 0;
	t_Loop_Test *ptr_Loop_Test;
	//
	// ************************************************************************
	// *** Select active channel for LT         *******************************
	// ************************************************************************
	ptr_Loop_Test = (Channel == TPE1) ? &Loop_TestCh1: &Loop_TestCh2;

	if(ptr_Loop_Test->Channel_Active == ON)
	{
		// **********************************************************
		// *** Channel is active ************************************
		// **********************************************************
		switch(ptr_Loop_Test->State_100ms_LT)	{	
		case WAIT_AFTER_STARTUP:
			if(!(ptr_Loop_Test->Common_State & LOOP_TEST_BUTTON))
			{
				// Manual Loop test (initiated from HMI600) is not active
				if(ptr_Loop_Test->Cyclic_LT_Time) 
				{								
					// Cyclic Loop Test is enabled
					if(ptr_Loop_Test->State_Timer_LT < ptr_Loop_Test->Start_up_Time) 
					{					
						// Wait for x minutes
						ptr_Loop_Test->State_Timer_LT++;
					}
					else 
					{
						ptr_Loop_Test->State_Timer_LT = 0;
						ptr_Loop_Test->State_100ms_LT = CYCLIC_LT_RUNNING;
						ptr_Loop_Test->LT_Wait_Counter = 1;
					}
				}
				else 
				{
					// cyclic looptest is off do not wait after start up go to idle state direct
					ptr_Loop_Test->State_100ms_LT = LT_IDLE;
				}
			}
			else 
			{
					// Manual LT is activated via HMI600
					// -> start manual loop test
					ptr_Loop_Test->State_Timer_LT = 0;
					ptr_Loop_Test->State_100ms_LT = MANUAL_LT_RUNNING;
					ptr_Loop_Test->Common_State &= ~LOOP_TEST_BUTTON;
					ptr_Loop_Test->LT_Wait_Counter = 1;
			}
			break;
		case LT_IDLE:
			if(!(ptr_Loop_Test->Common_State & LOOP_TEST_BUTTON))
			{
				// ******************************************************
				// *** no manual loop test activated via HMI600 *********
				// ******************************************************
				if(ptr_Loop_Test->Cyclic_LT_Time) 
				{
					// ***********************************
					// *** cyclic loop test is enable ****
					// ***********************************
					if(ptr_Loop_Test->State_Timer_LT < ptr_Loop_Test->Cyclic_LT_Time) 
					{	
						// *******************************
						// *** Wait for X hours **********
						// *******************************
						ptr_Loop_Test->State_Timer_LT++;
					}
					else 
					{
						// *******************************
						// *** start cyclic loop test ****
						// *******************************
						ptr_Loop_Test->State_Timer_LT = 0;
						ptr_Loop_Test->State_100ms_LT = CYCLIC_LT_RUNNING;
						ptr_Loop_Test->LT_Wait_Counter = 1;
					}
				}
			}
			else 
			{
				// ******************************************************
				// *** Manual LT is activated via HMI600 ****************
				// ******************************************************
				// -> start manual loop test
				ptr_Loop_Test->State_Timer_LT = 0;
				ptr_Loop_Test->State_100ms_LT = MANUAL_LT_RUNNING;
				ptr_Loop_Test->Common_State &= ~LOOP_TEST_BUTTON;
				ptr_Loop_Test->LT_Wait_Counter = 1;
			}
			break;
		case MANUAL_LT_RUNNING:	
			if ((ptr_Loop_Test->Common_State & (LT_ACTIVATED | LT_STOPPED)) == 0) 
			{
				// ******************************************************
				// *** no loop test is running now **********************
				// ******************************************************
				if((TxCommand == 0)&&(RxCommand == 0))
				{
					// **************************************************
					// *** no tx or rx command active -> start looptest *
					// **************************************************
					ptr_Loop_Test->Common_State |= LT_ACTIVATED;		// Entering function for the 1st time. Start-up looptest().
					*ptr_LT_Event |= MANUAL_LT_SENT_EVENT;
					temp_counter = ((ptr_Loop_Test->Counter_LT_TX_RX + 0x10000) & 0xffff0000);
					ptr_Loop_Test->Counter_LT_TX_RX = (ptr_Loop_Test->Counter_LT_TX_RX &0x0000ffff) | temp_counter;
				}
				else
				{
					// **************************************************
					// *** tx or rx command active -> do not start looptest
					// **************************************************
					ptr_Loop_Test->State_100ms_LT = LED_FAILED_LT; // no count of looptest failed only led settings 
					ptr_Loop_Test->LoopTest_Duration = LT_CANCEL;
					ptr_NSD_Status->ActualTransmissionTime = LT_CANCEL;
					ptr_Loop_Test->Manual_LT_State = M_LT_END;
					ptr_Loop_Test->Common_State &= ~LT_PASSED;
				}
			}
			else if ((ptr_Loop_Test->Common_State & (LT_ACTIVATED | LT_STOPPED)) == (LT_ACTIVATED | LT_STOPPED)) 
			{ 
				// ******************************************************
				// *** looptest has finished.  **************************
				// ******************************************************
				ptr_Loop_Test->Manual_LT_State = M_LT_END;
				ptr_Loop_Test->Common_State &= ~(LT_ACTIVATED | LT_STOPPED);
				if (ptr_Loop_Test->Common_State & LT_PASSED) 
				{	
					// ***********************************
					// *** Loop Test has passed. *********
					// ***********************************
					ptr_Loop_Test->State_100ms_LT = LED_PASSED_LT;
					ptr_Loop_Test->LT_Alarms &= ~LOOP_TEST_ERROR;
					ptr_Loop_Test->Number_Of_Tries = 0;
					ptr_NSD_Status->NSD_Alarms &= ~NSD_LOOP_TEST_WARNING;
					ptr_NSD_Status->NSD_Alarms &= ~NSD_LOOP_TEST_ALARM;
					ptr_NSD_Status->ActualTransmissionTime = ptr_Loop_Test->LoopTest_Duration;
				}
				else 
				{	
					// ***********************************
					// *** Loop Test has failed. *********
					// ***********************************
					// event loop test failed
					*ptr_LT_Event |= MANUAL_LT_FAILED_EVENT;

					if (ptr_Loop_Test->Number_Of_Tries < 2) 
					{
						ptr_Loop_Test->State_100ms_LT = LED_FAILED_LT;
						ptr_Loop_Test->Number_Of_Tries++;
						ptr_NSD_Status->NSD_Alarms|= NSD_LOOP_TEST_WARNING;
						ptr_NSD_Status->ActualTransmissionTime = LT_FAIL;
					}
					else 
					{										
						// *****************************************
						// *** Loop Test has failed for the 3rd time.
						// *****************************************
						ptr_Loop_Test->LT_Alarms |= LOOP_TEST_ERROR;
						ptr_NSD_Status->NSD_Alarms &=~NSD_LOOP_TEST_WARNING;
						ptr_NSD_Status->NSD_Alarms|= NSD_LOOP_TEST_ALARM;
						ptr_NSD_Status->ActualTransmissionTime = LT_FAIL;
						ptr_Loop_Test->State_100ms_LT = LED_FAILED_LT;
					}
				}
			}
			break;
		case CYCLIC_LT_RUNNING:	
			if ((ptr_Loop_Test->Common_State & (LT_ACTIVATED | LT_STOPPED)) == 0) 
			{
				// ******************************************************
				// *** no cyclic looptest is running now ****************
				// ******************************************************
				if(((TxCommand) == 0)&&(RxCommand == 0))
				{
					// *********************************************
					// *** no tx or rx command active -> start cyclic looptest
					// **********************************************
					ptr_Loop_Test->Common_State |= LT_ACTIVATED;		// Entering function for the 1st time. Start-up looptest().
					*ptr_LT_Event |= CYCLIC_LT_SENT_EVENT;
					temp_counter = ((ptr_Loop_Test->Counter_LT_TX_RX + 0x10000) & 0xffff0000);
					ptr_Loop_Test->Counter_LT_TX_RX = (ptr_Loop_Test->Counter_LT_TX_RX &0x0000ffff) | temp_counter;
				}
				else
				{
					// **********************************************
					// *** tx or rx command active -> do not start cyclic looptest -> wait one minute
					// **********************************************
					ptr_Loop_Test->State_100ms_LT = LT_IDLE;
					ptr_Loop_Test->State_Timer_LT = ptr_Loop_Test->Cyclic_LT_Time - ONE_MINUTE_100MS_CASE; // set start time in one minute counter counts up!!!
				}
			}
			else if ((ptr_Loop_Test->Common_State & (LT_ACTIVATED | LT_STOPPED)) == (LT_ACTIVATED | LT_STOPPED)) 
			{ 
				// ***************************************
				// *** cyclic looptest has finished. *****
				// ***************************************
				ptr_Loop_Test->Common_State &= ~(LT_ACTIVATED | LT_STOPPED);
				if (ptr_Loop_Test->Common_State & LT_PASSED) 
				{	
					// ***********************************
					// *** cyclic Loop Test has passed. **
					// ***********************************
					ptr_Loop_Test->State_100ms_LT = LT_IDLE;
					ptr_Loop_Test->LT_Alarms &= ~LOOP_TEST_ERROR;
					ptr_Loop_Test->Number_Of_Tries = 0;
					ptr_NSD_Status->NSD_Alarms &= ~NSD_LOOP_TEST_WARNING;
					ptr_NSD_Status->NSD_Alarms &= ~NSD_LOOP_TEST_ALARM;
					ptr_NSD_Status->ActualTransmissionTime = ptr_Loop_Test->LoopTest_Duration;
				}
				else 
				{											
					// ***********************************
					// *** Cyclic Loop Test has failed. **
					// ***********************************
					// LT Event failed 
					*ptr_LT_Event |= LT_FAILED_EVENT;
					//
					if (ptr_Loop_Test->Number_Of_Tries < 2) 
					{
						ptr_Loop_Test->State_100ms_LT = WAIT_AFTER_FAILED_CYCLIC_LT;
						ptr_Loop_Test->Number_Of_Tries++;
						ptr_NSD_Status->NSD_Alarms |= NSD_LOOP_TEST_WARNING;
						ptr_NSD_Status->ActualTransmissionTime = LT_FAIL;
					}
					else 
					{	
						// cyclic Loop Test has failed for the 3rd time.
						ptr_Loop_Test->LT_Alarms |= LOOP_TEST_ERROR;
						ptr_NSD_Status->NSD_Alarms &= ~NSD_LOOP_TEST_WARNING;
						ptr_NSD_Status->NSD_Alarms |= NSD_LOOP_TEST_ALARM;
						ptr_NSD_Status->ActualTransmissionTime = LT_FAIL;
						ptr_Loop_Test->State_100ms_LT = WAIT_AFTER_FAILED_CYCLIC_LT;	// new loop test repeat time in case of error 5 minutes
					}
				}
			}
			break;
		case WAIT_AFTER_FAILED_CYCLIC_LT:
			if(!(ptr_Loop_Test->Common_State & LOOP_TEST_BUTTON))
			{
				// ******************************************************
				// *** Manual loop test is not activated ****************
				// ******************************************************
				if(ptr_Loop_Test->State_Timer_LT < FIVE_MINUTES_100MS_CASE) 
				{					
					// ***********************************
					// *** Wait for 5 minutes ************
					// ***********************************
					ptr_Loop_Test->State_Timer_LT++;
				}
				else 
				{
					// ***********************************
					// *** start cyclic loop test after failure
					// ***********************************
					ptr_Loop_Test->State_Timer_LT = 0;
					ptr_Loop_Test->State_100ms_LT = CYCLIC_LT_RUNNING;
				}
			}
			else 
			{
				// ******************************************************
				// *** start manual loop test initiated by HMI600   *****
				// ******************************************************
				ptr_Loop_Test->State_Timer_LT = 0;
				ptr_Loop_Test->State_100ms_LT = MANUAL_LT_RUNNING;
				ptr_Loop_Test->Common_State &= ~LOOP_TEST_BUTTON;
			}
			break;
		case LED_PASSED_LT:	
			// ***********************************************************
			// *** Manual loop test initiated by HMI600 passed ***********
			// ***********************************************************
			if(ptr_Loop_Test->State_Timer_LT < THREE_SECONDS_100MS_CASE) 
			{							
				// ***************************************
				// *** Wait 3 seconds before next loop test is possible
				// ***************************************
				ptr_Loop_Test->State_Timer_LT++;
			}
			else 
			{
				// ***************************************
				// *** Wait time 3 sec is over -> enable manual loop test again
				// ***************************************
				ptr_Loop_Test->State_Timer_LT = 0;
				ptr_Loop_Test->State_100ms_LT = LT_IDLE;
			}
			break;
		case LED_FAILED_LT:	
			// ***********************************************************
			// *** Manual loop test initiated by HMI600 failed ***********
			// ***********************************************************
			
			if(ptr_Loop_Test->State_Timer_LT < THREE_SECONDS_100MS_CASE) 
			{						
				// ***************************************
				// *** Wait 3 seconds before next loop test is possible
				// ***************************************
				ptr_Loop_Test->State_Timer_LT++;
			}
			else 
			{
				// ***************************************
				// *** Wait time 3 sec is over -> enable manual loop test again
				// ***************************************	
				ptr_Loop_Test->State_100ms_LT = LT_IDLE;
				ptr_Loop_Test->State_Timer_LT = 0;
			}
			break;
		default:
			ASSERT(FALSE);
		}
	}
	else
	{
		// Channel is switched off -> do nothing
	}
}

/*****************************************************************************
  Function:		void LT_Handler(long *ptr_TxCommand, long RxCommand, long Channel)
  Description:	 
  Inputs:		long RxCommand, long Channel
  Outputs:		long *ptr_TxCommand
  Return:		None
  Globals:		
  Important:	Overwrite the send command word to NSD transmitter 
				if a loop test should be sent

******************************************************************************/
void SEGMENT_EXT_CODE LT_Handler(long *ptr_TxCommand, long RxCommand, long Channel)
{
	long temp_counter = 0;
	t_Loop_Test *ptr_Loop_Test;

	//
	// ************************************************************************
	// *** Select active channel for LT Handler *******************************
	// ************************************************************************
	ptr_Loop_Test = (Channel == TPE1) ? &Loop_TestCh1: &Loop_TestCh2;

	if(ptr_Loop_Test->Channel_Active == ON)
	{
		if ((ptr_Loop_Test->Common_State & (LT_ACTIVATED | LT_STOPPED)) == LT_ACTIVATED)
		{


				// ************************************************************************
				// *** Check looptest window max time 10*T0********************************
				// ************************************************************************
				//
				if (ptr_Loop_Test->LT_Fast_Counter <= (LOOPTEST_WINDOW * ptr_Loop_Test->LT_Tev_max)) 
				{
					ptr_Loop_Test->LT_Fast_Counter++;
					// **********************************************************
					// *** Check loop test send time ****************************
					// **********************************************************
					if (ptr_Loop_Test->LT_Fast_Counter <= (SEND_TIME_LOOPTEST * ptr_Loop_Test->LT_Tev_max)) 
					{			
						// Send LT during 3 * T0
						// @Pl_ext
						*ptr_TxCommand |= TX_COM_T;
					}
					else
					{
						// loop test send time > send time loop test -> stop sending loop test
						// @Pl_ext
						*ptr_TxCommand &= ~TX_COM_T;
					}
					// @Pl_ext
					if (RxCommand == RX_COM_T) 
					{
						// **********************************************
						// *** Loop test received ***********************
						// **********************************************
						if (ptr_Loop_Test->LT_Fast_Counter >= (ptr_Loop_Test->LT_Tev_min)) 
						{
							// ******************************************
							// ***  Loop test time within minim. limit **
							// ******************************************
							// @Pl_ext
							*ptr_TxCommand &= ~TX_COM_T;
							ptr_Loop_Test->LoopTest_Duration = ((ptr_Loop_Test->LT_Fast_Counter * 1000 * MAINCASE_TIME) / 2) + RELAY_INOUT_DELAY;	// Time in [us].
							ptr_Loop_Test->LT_Fast_Counter = 0;
							ptr_Loop_Test->Common_State |= (LT_STOPPED | LT_PASSED);
							temp_counter = ((ptr_Loop_Test->Counter_LT_TX_RX + 0x1) & 0x0000ffff);
		 					ptr_Loop_Test->Counter_LT_TX_RX = (ptr_Loop_Test->Counter_LT_TX_RX &0xffff0000) | temp_counter;
						}
						else 
						{
							// ******************************************
							// *** loop test too fast not within limit **
							// ******************************************
							// @Pl_ext
							*ptr_TxCommand &= ~TX_COM_T;
							ptr_Loop_Test->LoopTest_Duration = ((ptr_Loop_Test->LT_Fast_Counter * 1000 * MAINCASE_TIME) / 2) + RELAY_INOUT_DELAY;	// Time in [us].
							ptr_Loop_Test->LT_Fast_Counter = 0;
							ptr_Loop_Test->Common_State |= LT_STOPPED;
							ptr_Loop_Test->Common_State &= ~LT_PASSED;
							temp_counter = ((ptr_Loop_Test->Counter_LT_TX_RX + 0x1) & 0x0000ffff);
							ptr_Loop_Test->Counter_LT_TX_RX = (ptr_Loop_Test->Counter_LT_TX_RX &0xffff0000) | temp_counter;
						}
					}
					else
					{
						// **********************************************
						// *** Loop test not received *******************
						// **********************************************
						// -> do nothing wait 
					}
				}
				else 
				{
					// **************************************************
					// *** Loop test not received within max limit 10*T0*
					// **************************************************
					ptr_Loop_Test->LoopTest_Duration = LT_FAIL;
					ptr_Loop_Test->LT_Fast_Counter = 0;
					ptr_Loop_Test->Common_State |= LT_STOPPED;
					ptr_Loop_Test->Common_State &= ~LT_PASSED;
				}
		}
		else
		{
			// Do not execute LT Handler function  no action pending
		}
	}
	else
	{
		// Channel is switched off -> do nothing
	}
}

// **************************************************************************
// *** Comment to Loop test repeater ****************************************
// 
// 
//
//
/*****************************************************************************
  Function:		LT_Repeater(long *ptr_TxCommand, unsigned long *ptr_LT_Event, long RxCommand, long Channel)
  Description:	 
  Inputs:		long RxCommand, long Channel
  Outputs:		long *ptr_TxCommand
  Return:		None
  Globals:		
  Important:	Die Funktion LT_Repeater() wird ^NSD_DSP_Main im 1ms Case aufgerufen!
				Der Looptest Repeater kann nach einem gesendeten Looptest (Manuel, Zyklisch oder mit HMI)
				oder nach einem repeateten Looptest erst nach der Zeit von LT_REPEATER_BLOCK_TIME (3.0sec) 
				wieder ausgeführt werden. Während der Blockierzeit werden auch keine ankommenden 
				Looptests gezählt sowie im Eventrecorder nicht aufgezeichnet! 
******************************************************************************/
void SEGMENT_EXT_CODE LT_Repeater(long *ptr_TxCommand, unsigned long *ptr_LT_Event, long RxCommand, long Channel)
{
	t_Loop_Test *ptr_Loop_Test;
	// ************************************************************************
	// *** Select active channel for LT Repeater ******************************
	// ************************************************************************
	ptr_Loop_Test = (Channel == TPE1) ? &Loop_TestCh1: &Loop_TestCh2;

	if(ptr_Loop_Test->Channel_Active == ON)
	{
		// ****************************************************
		// *** Check loop test block counter ******************
		// ****************************************************
		//
		if (ptr_Loop_Test->LT_Wait_Counter < (LT_REPEATER_BLOCK_TIME))	ptr_Loop_Test->LT_Wait_Counter++;
		//
		switch(ptr_Loop_Test->State_LT_Repeater)
		{
		case REPEAT_IDLE_STATE:
			// @Pl_ext
			if (RxCommand == RX_COM_T)
			{
				if ((!(ptr_Loop_Test->Common_State & LT_ACTIVATED)) && ((ptr_Loop_Test->LT_Wait_Counter == 0) || (ptr_Loop_Test->LT_Wait_Counter >= (LT_REPEATER_BLOCK_TIME))))
				{
					//user_Events = LT_REMOTE;
					*ptr_LT_Event |= LT_REFLECTED_EVENT;
					// 
					if((*ptr_TxCommand) != 0) // not "no command"
					{
						ptr_Loop_Test->LT_Wait_Counter = 1; // set wait counter 1 
					}
					else
					{
						ptr_Loop_Test->State_LT_Repeater = REPEAT_ACTIVE_STATE; // set state repeat loop test
						// @Pl_ext
						*ptr_TxCommand |= TX_COM_T;
						ptr_Loop_Test->Send_time_LT = 0;
						ptr_Loop_Test->LT_Wait_Counter = 0;
					}
				}
				else
				{
				//	
				}
			}
			else
			{
				if (!(ptr_Loop_Test->Common_State & LT_ACTIVATED)) 
				{
					// @Pl_ext
					*ptr_TxCommand &= ~TX_COM_T;
				//	
				}
			}
			break;
		case REPEAT_ACTIVE_STATE:
			// @Pl_ext
			if((*ptr_TxCommand & (ALL_LP_AES_TX_COMMANDS | ALL_LP_COMMANDS_A_F) ) != 0)	// ALL_LP_AES_TX_COMMANDS used because COM_T could is active
			{
				// can not repeat looptest because tx commmand is active
				// @Pl_ext
				*ptr_TxCommand &= ~TX_COM_T;
				ptr_Loop_Test->Send_time_LT = 1+ (SEND_TIME_LOOPTEST * ptr_Loop_Test->LT_Tev_max);
			}
			else
			{
				// repeat looptest no command is active
				if (ptr_Loop_Test->Send_time_LT > (SEND_TIME_LOOPTEST * ptr_Loop_Test->LT_Tev_max))
				{
					// do not send loop test longer 
					ptr_Loop_Test->State_LT_Repeater = REPEAT_IDLE_STATE;
					// @Pl_ext
					*ptr_TxCommand &= ~TX_COM_T;
				}
				else
				{
					// send loop test
					ptr_Loop_Test->Send_time_LT++;
					// @Pl_ext
					*ptr_TxCommand |= TX_COM_T;
				}
			}
			break;
		default:
			ptr_Loop_Test->State_LT_Repeater = REPEAT_IDLE_STATE;
			break;
		}
	}
	else
	{
		// channel is switched off -> do nothing
	}
}
/*****************************************************************************
  Function:		Init_Cyclic_LT (t_NSD_DSP_Config *g_NSD_Config)
  Description:	Init the parameter in the structs Loop_TestCh1 and Loop_TestCh2
				according to the configuration settings
  Inputs:		t_NSD_DSP_Config *g_NSD_Config
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
void SEGMENT_EXT_CODE Init_Cyclic_LT (t_NSD_DSP_Config *g_NSD_Config)
{
	// @Pl_ext
	long TPE_Basic_Mode = 0;
	// 
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);
	//
	switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			Loop_TestCh1.Cyclic_LT_Time = 0;
			Loop_TestCh2.Cyclic_LT_Time = 0;
			Loop_TestCh1.Channel_Active = OFF;
			Loop_TestCh2.Channel_Active = OFF;
			break;
		case INT_TELPROT_MODE_AES:
			// init Ch1 data 
			Loop_TestCh1.Channel_Active = ON;
			//
			if((g_NSD_Config->AES.AES600_Mode & AES600_T0) == AES600_T0)
			{
				Loop_TestCh1.LT_Tev_max = T02_AES600_MAINCASE_TIME;
				Loop_TestCh1.LT_Tev_min = (T02_AES600_MAINCASE_TIME * MINIMUM_LOOPTESTTIME);
			}
			else
			{
				Loop_TestCh1.LT_Tev_max = T01_AES600_MAINCASE_TIME;
				Loop_TestCh1.LT_Tev_min = (T01_AES600_MAINCASE_TIME * MINIMUM_LOOPTESTTIME);
			}
			//
			if((g_NSD_Config->AES.AES600_Mode & AES600_LOOPTEST_ON_OFF) == AES600_LOOPTEST_ON_OFF)
			{
					
				Loop_TestCh1.Cyclic_LT_Time = (long)((AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * SECONDS_PER_HOUR * g_NSD_Config->AES.TimeBetweenCL_Test);
				// for tests only
				// Loop_TestCh1.Cyclic_LT_Time = (AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * 60; // every 60 sec 
				//
				Loop_TestCh1.Start_up_Time = ONE_MINUTE_100MS_CASE * START_FIRST_LT;
			}
			else
			{
				// cyclic loop test ch 1 is off
				Loop_TestCh1.Cyclic_LT_Time = 0;
			}
			//
			Loop_TestCh2.Channel_Active = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			//
			// one channel 
			//
			Loop_TestCh1.Channel_Active = ON;
			//
			Loop_TestCh1.LT_Tev_max = T0_NSD600_MAINCASE_TIME;
			Loop_TestCh1.LT_Tev_min = (T0_NSD600_MAINCASE_TIME * MINIMUM_LOOPTESTTIME);
			//
			if((g_NSD_Config->NSD[0].NsdMode & NSD_MODE_CYCLIC_LOOPTEST_ON) == NSD_MODE_CYCLIC_LOOPTEST_ON)
			{
					
				Loop_TestCh1.Cyclic_LT_Time = (long)((AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * SECONDS_PER_HOUR * g_NSD_Config->NSD[0].TimeBetweenCL_Test);
				// for tests only
				// Loop_TestCh1.Cyclic_LT_Time = (AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * 60; // every 60 sec 
				//
				Loop_TestCh1.Start_up_Time = ONE_MINUTE_100MS_CASE * START_FIRST_LT;
			}
			else
			{
				// cyclic loop test ch 1 is off
				Loop_TestCh1.Cyclic_LT_Time = 0;
			}
			//
			Loop_TestCh2.Channel_Active = OFF;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// ***************************************************
			// *** do init for Ch1
			// ***************************************************
			//
			Loop_TestCh1.Channel_Active = ON;
			//
			Loop_TestCh1.LT_Tev_max = T0_NSD600_MAINCASE_TIME;
			Loop_TestCh1.LT_Tev_min = (T0_NSD600_MAINCASE_TIME * MINIMUM_LOOPTESTTIME);
			//
			if((g_NSD_Config->NSD[0].NsdMode & NSD_MODE_CYCLIC_LOOPTEST_ON) == NSD_MODE_CYCLIC_LOOPTEST_ON)
			{
				Loop_TestCh1.Cyclic_LT_Time = (long)((AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * SECONDS_PER_HOUR * g_NSD_Config->NSD[0].TimeBetweenCL_Test);
				// for tests
				//Loop_TestCh1.Cyclic_LT_Time = (AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * 60; // loop test every 60 sec
				//
				Loop_TestCh1.Start_up_Time = ONE_MINUTE_100MS_CASE * START_FIRST_LT;
			}
			else
			{
				// cyclic loop test ch 1 is off
				Loop_TestCh1.Cyclic_LT_Time = 0;
			}
			//
			// ***************************************************
			// *** do init for Ch2
			// ***************************************************
			//
			Loop_TestCh2.Channel_Active = ON;
			//
			Loop_TestCh2.LT_Tev_max = T0_NSD600_MAINCASE_TIME;
			Loop_TestCh2.LT_Tev_min = (T0_NSD600_MAINCASE_TIME * MINIMUM_LOOPTESTTIME);
			//
			if((g_NSD_Config->NSD[1].NsdMode & NSD_MODE_CYCLIC_LOOPTEST_ON) == NSD_MODE_CYCLIC_LOOPTEST_ON)
			{
				Loop_TestCh2.Cyclic_LT_Time = (long)((AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * SECONDS_PER_HOUR * g_NSD_Config->NSD[1].TimeBetweenCL_Test);
				// for tests only
				//Loop_TestCh2.Cyclic_LT_Time = (AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * 60; // loop test every 60 sec 
				//
				Loop_TestCh2.Start_up_Time = ONE_MINUTE_100MS_CASE * START_FIRST_LT * START_UP_DELAY_CH2;
			}
			else
			{
				// cyclic loop test ch 2 is off
				Loop_TestCh2.Cyclic_LT_Time = 0;
			}
			//
			break;
		case INT_TELPROT_MODE_HSD:
			//
			// one channel HSD is on
			//
			Loop_TestCh1.Channel_Active = ON;
			//
			Loop_TestCh1.LT_Tev_max = T0_NSD600_MAINCASE_TIME;
			Loop_TestCh1.LT_Tev_min = (T0_NSD600_MAINCASE_TIME * MINIMUM_LOOPTESTTIME);
			//
			if((g_NSD_Config->NSD[0].NsdMode & NSD_MODE_CYCLIC_LOOPTEST_ON) == NSD_MODE_CYCLIC_LOOPTEST_ON)
			{
					
				Loop_TestCh1.Cyclic_LT_Time = (long)((AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * SECONDS_PER_HOUR * g_NSD_Config->NSD[0].TimeBetweenCL_Test);
				// for tests only
				// Loop_TestCh1.Cyclic_LT_Time = (AF_SAMPLE_FREQ/(2*NUMBER_OF_MAINCASE*SUBCASE_100MS_CORRECTION)) * 60; // every 60 sec 
				//
				Loop_TestCh1.Start_up_Time = ONE_MINUTE_100MS_CASE * START_FIRST_LT;
			}
			else
			{
				// cyclic loop test ch 1 is off
				Loop_TestCh1.Cyclic_LT_Time = 0;
			}
			//
			Loop_TestCh2.Channel_Active = OFF;
			//
			break;
		default:
			ASSERT(FALSE);
	}

	// do init all variable for Looptest ch1
			Loop_TestCh1.State_100ms_LT = WAIT_AFTER_STARTUP;
			Loop_TestCh1.State_Timer_LT = 0;
			Loop_TestCh1.LT_Fast_Counter = 0;
			Loop_TestCh1.Common_State = 0;
			Loop_TestCh1.State_LT_Repeater = REPEAT_IDLE_STATE;
			Loop_TestCh1.Send_time_LT = 0;
			Loop_TestCh1.LT_Wait_Counter = 0;
			Loop_TestCh1.Number_Of_Tries = 0;
			Loop_TestCh1.Manual_LT_State = M_LT_IDLE;
			Loop_TestCh1.LoopTest_Duration = 0;
			Loop_TestCh1.Counter_LT_TX_RX = 0;
	//
	// do init all variable for Looptest ch2
			Loop_TestCh2.State_100ms_LT = WAIT_AFTER_STARTUP;
			Loop_TestCh2.State_Timer_LT = 0;
			Loop_TestCh2.LT_Fast_Counter = 0;
			Loop_TestCh2.Common_State = 0;
			Loop_TestCh2.State_LT_Repeater = REPEAT_IDLE_STATE;
			Loop_TestCh2.Send_time_LT = 0;
			Loop_TestCh2.LT_Wait_Counter = 0;
			Loop_TestCh2.Number_Of_Tries = 0;
			Loop_TestCh2.Manual_LT_State = M_LT_IDLE;
			Loop_TestCh2.LoopTest_Duration = 0;
			Loop_TestCh2.Counter_LT_TX_RX = 0;

}
/*****************************************************************************
  Function:		void Update_Dev_Status_LT(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD)
  Description:	For test update developer status Loop test counter Tx und Rx for both channels 
  Inputs:		t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

******************************************************************************/
void SEGMENT_EXT_CODE Update_Dev_Status_LT(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD)
{
	// For tests only
	// update loop test counter in developer status
	//
	ptr_Develop_Status_NSD->l_Development_3 = ((Loop_TestCh1.Counter_LT_TX_RX & 0xffff0000)>>16);	// update loop test tx counter Ch1
	ptr_Develop_Status_NSD->l_Development_4 = (Loop_TestCh1.Counter_LT_TX_RX & 0x0000ffff);			// update loop test rx counter Ch1

	ptr_Develop_Status_NSD->l_Development_5 = ((Loop_TestCh2.Counter_LT_TX_RX & 0xffff0000)>>16);	// update loop test tx counter Ch2
	ptr_Develop_Status_NSD->l_Development_6 = (Loop_TestCh2.Counter_LT_TX_RX & 0x0000ffff);			// update loop test rx counter Ch2 

}
