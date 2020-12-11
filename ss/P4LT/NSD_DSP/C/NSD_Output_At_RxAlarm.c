/*-------------------------------------------------------------------------*
 * $Workfile: NSD_Output_At_RxAlarm.c $
 * Part of	: NSD600
 * Language	: C
 * Created by	:  P.Plueer   
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSD_DSP/C/NSD_Output_At_RxAlarm.c $
 * 
 * 21    13-09-24 16:48 Chhamaa
 * AES Mode: NSD_TRIP_AND_GUARD_ALARM added to Maske_Rx_Alarm in order to
 * enable blocking of outputs in case of single tone interferer at
 * commmand frequencies.
 * 
 * 20    12-09-20 10:24 Chhamaa
 * Modifications for 2.5 kHz TPS mode.
 * 
 * 16    14.11.06 13:41 Chpeplu
 * Function init_Output_At_RxAlarm() changed for custom mode, Function
 * Set_Output_At_RxAlarm() changed for up to 6 LP commands, Function
 * Freeze_CommandOutput() changed for up to 6 LP commands
 * 
 * 15    20.09.06 14:43 Chhamaa
 * Rx_Alarm_AND_Mask changed for AES
 * 
 * 14    7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 14    7.07.06 18:50 Chhamaa
 * 
 * 11    31.01.06 16:24 Chpeplu
 * Init function extended with new HSD mode
 * 
 * 9     23.09.05 13:27 Chpeplu
 * Hold time Freeze increased to 100ms and function Freeze_CommandOutput()
 * changed -> no event "Output set to predefined state"
 * 
 * 8     21.09.05 16:52 Chpeplu
 * Error (Bug 235) corrected in function Set_Output_At_RxAlarm()
 * 
 * 7     20.09.05 17:18 Chpeplu
 * Function Freeze_CommandOutput() new implemented and some other changes
 * 
 * 6     30.06.05 8:34 Chpeplu
 * Code walkthrough Pl
 * 
 * 5     22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 4     22.06.05 10:38 Chchlee
 * Code walkthrough
 * 
 * 3     15.06.05 14:38 Chpeplu
 * Start_up_Delay added to set command outputs in case of link alarm
 * 
 * 2     14.03.05 11:22 Chpeplu
 * Some changed made for up to 6 LP commands 
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 5     21.12.04 15:52 Chpeplu
 * struct t_NSD_DSP_Status changed to t_IntTeleprotStatus
 * 
 * 4     20.12.04 10:20 Chpeplu
 * Function Save_StateWithoutRxAlarm() changed, LastActivCommand will only
 * be saved if ChangeOutput is not active!
 * 
 * 3     16.12.04 10:34 Chpeplu
 * new function Save_StateWithoutRxAlarm() added
 * 
 * 2     15.12.04 17:32 Chpeplu
 * Second Version runs
 * 
 * 1     15.12.04 11:02 Chpeplu
 * Inital Version 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include "basetype.h"
#include "NSD_DSP_Types.h"
#include "NSD_Types.h"
#include "NSD_Output_At_RxAlarm.h"
#include "diag.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// subcase is 106.66Hz
#define	CONV_SEC_SUBCASE	106.66
#define START_UP_DELAY_SEC	10
// special defines for freeze functionality
// in ms
#define HOLD_TIME_FREEZE	100
// mask to set commands or reset commmands
#define	RESET_NO_COM	0xffffffff
#define	OR_NO_CHANGE	0x00000000

/* LOCAL TYPE DECLARATIONS ***************************************************/
// Type for State_Hold_Delay
typedef enum 
{
	e_Idle_RX_Alarm,
	e_Delay_RX_Alarm,
	e_Hold_RX_Alarm
} e_State_Hold_Delay;
//
typedef struct {
	long				State_At_Alarm; // activ configuration setting (freezed, not influenzed, guard state, ..)
	long				Maske_Rx_Alarm;
	unsigned long		*ptrRxAlarm;
	long				Set_Command_Mask_RxAlarm;
	long				Reset_Command_Mask_RxAlarm;
	long				Set_Hold_time;
	long				Set_Pickup_time;
	e_State_Hold_Delay	State_Hold_Delay; // use enum
	long				Delay_time;
	long				Hold_time;
	BOOL				ChangeOutput;
	long				LastCommandWithoutRxAlarm;
	unsigned long		Rx_Alarm_OR_Mask;
	unsigned long		Rx_Alarm_AND_Mask;
	BOOL				Channel_activ;
	unsigned long		Start_up_Delay;
} t_Output_At_Alarm;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/

static t_Output_At_Alarm Output_At_RxAlarmCh1, Output_At_RxAlarmCh2;	

/* LOCAL FUNCTIONS ***********************************************************/
/* IMPLEMENTATION ************************************************************/
//
/*****************************************************************************
  Function:		init_Output_At_RxAlarm
  Description:	set all parameters used 
  Inputs:		pointer to NSD config struct, pointer to NSD status struct
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	-

******************************************************************************/
//
 void SEGMENT_EXT_CODE init_Output_At_RxAlarm(t_NSD_DSP_Config *g_NSD_Config, t_IntTeleprotStatus	*g_NSD_DSP_Status)
{
int	i;
long	l_temp_1;
//
	// @Pl_ext
	long TPE_Basic_Mode = 0;
	// 
	TPE_Basic_Mode = (g_NSD_Config->mode & INT_TPE_MODE_MASK);

switch(TPE_Basic_Mode)
	{
		case INT_TELPROT_MODE_NONE:
			//
			Output_At_RxAlarmCh1.Channel_activ = OFF;
			Output_At_RxAlarmCh2.Channel_activ = OFF;
			break;
		case INT_TELPROT_MODE_AES:
			//
			Output_At_RxAlarmCh1.State_At_Alarm = g_NSD_Config->AES.OutputsAtRxAlarm;
			Output_At_RxAlarmCh1.Set_Pickup_time = CONV_SEC_SUBCASE * g_NSD_Config->AES.OutputsAtRxAlarmPickupTime;
			Output_At_RxAlarmCh1.Set_Hold_time = CONV_SEC_SUBCASE * g_NSD_Config->AES.OutputsAtRxAlarmHoldTime;
			Output_At_RxAlarmCh1.ptrRxAlarm = &(g_NSD_DSP_Status->NSD_Status[0].NSD_Alarms);
			Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_TRIP_AND_GUARD_ALARM);
			Output_At_RxAlarmCh1.State_Hold_Delay = e_Idle_RX_Alarm;
			Output_At_RxAlarmCh1.ChangeOutput = FALSE;
			Output_At_RxAlarmCh1.LastCommandWithoutRxAlarm = 0;
			Output_At_RxAlarmCh1.Start_up_Delay = START_UP_DELAY_SEC*CONV_SEC_SUBCASE;
			//
			switch(Output_At_RxAlarmCh1.State_At_Alarm)
			{
				case OUTPUT_AT_RX_ALARM_UNAFFECTED:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_AES_RX_COMMANDS);
					break;
				case OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE:
					// set mask to reset all commands to guard state in case of Rx-alarm
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = (~ALL_LP_AES_RX_COMMANDS);
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_AES_RX_COMMANDS);
					break;
				default:
					ASSERT(FALSE);
			} 

			Output_At_RxAlarmCh1.Channel_activ = ON;
			Output_At_RxAlarmCh2.Channel_activ = OFF;
			break;

		case INT_TELPROT_MODE_NSD1_APLC1:
		case INT_TELPROT_MODE_NSD1_DPLC:
		case INT_TELPROT_MODE_HSD_25:
			Output_At_RxAlarmCh1.State_At_Alarm = g_NSD_Config->NSD[0].OutputsAtRxAlarm;
			Output_At_RxAlarmCh1.Set_Pickup_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[0].OutputsAtRxAlarmPickupTime;
			Output_At_RxAlarmCh1.Set_Hold_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[0].OutputsAtRxAlarmHoldTime;
			Output_At_RxAlarmCh1.ptrRxAlarm = &(g_NSD_DSP_Status->NSD_Status[0].NSD_Alarms);
			Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM);
			Output_At_RxAlarmCh1.State_Hold_Delay = e_Idle_RX_Alarm;
			Output_At_RxAlarmCh1.ChangeOutput = FALSE;
			Output_At_RxAlarmCh1.LastCommandWithoutRxAlarm = 0;
			Output_At_RxAlarmCh1.Start_up_Delay = START_UP_DELAY_SEC*CONV_SEC_SUBCASE;
			//
			switch(Output_At_RxAlarmCh1.State_At_Alarm)
			{
				case OUTPUT_AT_RX_ALARM_UNAFFECTED:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	//
					// @Pl_ext
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);	
					break;
				case OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE:
					// set mask to reset all commands to guard state in case of Rx-alarm
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);
					// @Pl_ext
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);
					break;
				case OUTPUT_AT_RX_ALARM_FROZEN:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					// @Pl_ext
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);	
					// over write  maske Rx Alarms for freeze including "no trip and no guard"
					Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_NO_TRIP_AND_GUARD_ALARM);
					Output_At_RxAlarmCh1.Set_Pickup_time = 0; // no pick up time for freeze (CONV_SEC_SUBCASE * PICK_UP_TIME_FREEZE;)
					Output_At_RxAlarmCh1.Set_Hold_time = HOLD_TIME_FREEZE * (1.0/CYCLE_TIME);
					break;
				case OUTPUT_AT_RX_ALARM_DIRECT_TO_GUARD_OTHERS_TRIPPED:
					l_temp_1 = 0x0001;
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = 0;
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = 0;
					for (i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
					{
						if(((g_NSD_Config->NSD[0].CommandApplication[i] == DIRECT)||(g_NSD_Config->NSD[0].CommandApplication[i] == COMMAND_OFF)))
						{
							Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm |=	l_temp_1<<i;
						}
						else
						{
							Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm |= l_temp_1<<i;

						}
					}
					break;
				default:
					ASSERT(FALSE);
			} 
			Output_At_RxAlarmCh1.Channel_activ = ON;
			Output_At_RxAlarmCh2.Channel_activ = OFF;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			// Ch1 settings
			Output_At_RxAlarmCh1.State_At_Alarm = g_NSD_Config->NSD[0].OutputsAtRxAlarm;
			Output_At_RxAlarmCh1.Set_Pickup_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[0].OutputsAtRxAlarmPickupTime;
			Output_At_RxAlarmCh1.Set_Hold_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[0].OutputsAtRxAlarmHoldTime;
			Output_At_RxAlarmCh1.ptrRxAlarm = &(g_NSD_DSP_Status->NSD_Status[0].NSD_Alarms);
			Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM);
			Output_At_RxAlarmCh1.State_Hold_Delay = e_Idle_RX_Alarm;
			Output_At_RxAlarmCh1.ChangeOutput = FALSE;
			Output_At_RxAlarmCh1.LastCommandWithoutRxAlarm = 0;
			Output_At_RxAlarmCh1.Start_up_Delay = START_UP_DELAY_SEC*CONV_SEC_SUBCASE;
			//
			switch(Output_At_RxAlarmCh1.State_At_Alarm)
			{
				case OUTPUT_AT_RX_ALARM_UNAFFECTED:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					// @Pl_ext
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);		// set Bits 0 ... 9 
					break;
				case OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE:
					// set mask to reset all commands to guard state in case of Rx-alarm
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);
					// @Pl_ext
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);
					break;
				case OUTPUT_AT_RX_ALARM_FROZEN:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					// @Pl_ext
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);	
					//
					Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_NO_TRIP_AND_GUARD_ALARM);
					Output_At_RxAlarmCh1.Set_Pickup_time = 0; // no pick up time for freeze (CONV_SEC_SUBCASE * PICK_UP_TIME_FREEZE;)
					Output_At_RxAlarmCh1.Set_Hold_time =  HOLD_TIME_FREEZE * (1.0/CYCLE_TIME);;
					break;
				case OUTPUT_AT_RX_ALARM_DIRECT_TO_GUARD_OTHERS_TRIPPED:
					l_temp_1 = 0x0001;
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = 0;
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = 0;
					for (i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
					{
						if(((g_NSD_Config->NSD[0].CommandApplication[i] == DIRECT)||(g_NSD_Config->NSD[0].CommandApplication[i] == COMMAND_OFF)))
						{
							Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm |=	l_temp_1<<i;
						}
						else
						{
							Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm |= l_temp_1<<i;

						}
					}
					break;
				default:
					ASSERT(FALSE);
			} 
			Output_At_RxAlarmCh1.Channel_activ = ON;
			//
			// Ch2 settings
			//
			Output_At_RxAlarmCh2.State_At_Alarm = g_NSD_Config->NSD[1].OutputsAtRxAlarm;
			Output_At_RxAlarmCh2.Set_Pickup_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[1].OutputsAtRxAlarmPickupTime;
			Output_At_RxAlarmCh2.Set_Hold_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[1].OutputsAtRxAlarmHoldTime;
			Output_At_RxAlarmCh2.ptrRxAlarm = &(g_NSD_DSP_Status->NSD_Status[1].NSD_Alarms);
			Output_At_RxAlarmCh2.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM);
			Output_At_RxAlarmCh2.State_Hold_Delay = e_Idle_RX_Alarm;
			Output_At_RxAlarmCh2.ChangeOutput = FALSE;
			Output_At_RxAlarmCh2.LastCommandWithoutRxAlarm = 0;
			Output_At_RxAlarmCh2.Start_up_Delay = START_UP_DELAY_SEC*CONV_SEC_SUBCASE;
			//
			switch(Output_At_RxAlarmCh2.State_At_Alarm)
			{
				case OUTPUT_AT_RX_ALARM_UNAFFECTED:
					Output_At_RxAlarmCh2.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					// @Pl_ext
					Output_At_RxAlarmCh2.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);		// set Bits 0 ... 9 
					break;
				case OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE:
					// set mask to reset all commands to guard state in case of Rx-alarm
					// @Pl_ext
					Output_At_RxAlarmCh2.Reset_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);
					Output_At_RxAlarmCh2.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);
					break;
				case OUTPUT_AT_RX_ALARM_FROZEN:
					Output_At_RxAlarmCh2.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					// @Pl_ext
					Output_At_RxAlarmCh2.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS_A_F);	
					//
					Output_At_RxAlarmCh2.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_NO_TRIP_AND_GUARD_ALARM);
					Output_At_RxAlarmCh2.Set_Pickup_time = 0; // no pick up time for freeze (CONV_SEC_SUBCASE * PICK_UP_TIME_FREEZE;)
					Output_At_RxAlarmCh2.Set_Hold_time =  HOLD_TIME_FREEZE * (1.0/CYCLE_TIME);;
					break;
				case OUTPUT_AT_RX_ALARM_DIRECT_TO_GUARD_OTHERS_TRIPPED:
					l_temp_1 = 0x0001;
					Output_At_RxAlarmCh2.Set_Command_Mask_RxAlarm = 0;
					Output_At_RxAlarmCh2.Reset_Command_Mask_RxAlarm = 0;
					for (i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
					{
						if(((g_NSD_Config->NSD[1].CommandApplication[i] == DIRECT)||(g_NSD_Config->NSD[1].CommandApplication[i] == COMMAND_OFF)))
						{
							Output_At_RxAlarmCh2.Reset_Command_Mask_RxAlarm |=	l_temp_1<<i;
						}
						else
						{
							Output_At_RxAlarmCh2.Set_Command_Mask_RxAlarm |= l_temp_1<<i;

						}
					}
					break;
				default:
					ASSERT(FALSE);
			}
			Output_At_RxAlarmCh2.Channel_activ = ON;
			break;
		case INT_TELPROT_MODE_HSD:
			// init HSD mode
			Output_At_RxAlarmCh1.State_At_Alarm = g_NSD_Config->NSD[0].OutputsAtRxAlarm;
			Output_At_RxAlarmCh1.Set_Pickup_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[0].OutputsAtRxAlarmPickupTime;
			Output_At_RxAlarmCh1.Set_Hold_time = CONV_SEC_SUBCASE * g_NSD_Config->NSD[0].OutputsAtRxAlarmHoldTime;
			Output_At_RxAlarmCh1.ptrRxAlarm = &(g_NSD_DSP_Status->NSD_Status[0].NSD_Alarms);
			Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM);
			Output_At_RxAlarmCh1.State_Hold_Delay = e_Idle_RX_Alarm;
			Output_At_RxAlarmCh1.ChangeOutput = FALSE;
			Output_At_RxAlarmCh1.LastCommandWithoutRxAlarm = 0;
			Output_At_RxAlarmCh1.Start_up_Delay = START_UP_DELAY_SEC*CONV_SEC_SUBCASE;
			//
			switch(Output_At_RxAlarmCh1.State_At_Alarm)
			{
				case OUTPUT_AT_RX_ALARM_UNAFFECTED:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS);	
					break;
				case OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE:
					// set mask to reset all commands to guard state in case of Rx-alarm
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS);
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS);
					break;
				case OUTPUT_AT_RX_ALARM_FROZEN:
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = RESET_NO_COM;	// 
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = (~ALL_LP_COMMANDS);	
					// over write  maske Rx Alarms for freeze including "no trip and no guard"
					Output_At_RxAlarmCh1.Maske_Rx_Alarm = (NSD_SNR_ALARM|NSD_RX_GUARD_LOW_LEVEL_ALARM|NSD_RX_GUARD_HIGH_LEVEL_ALARM|NSD_NO_TRIP_AND_GUARD_ALARM);
					Output_At_RxAlarmCh1.Set_Pickup_time = 0; // no pick up time for freeze (CONV_SEC_SUBCASE * PICK_UP_TIME_FREEZE;)
					Output_At_RxAlarmCh1.Set_Hold_time = HOLD_TIME_FREEZE * (1.0/CYCLE_TIME);
					break;
				case OUTPUT_AT_RX_ALARM_DIRECT_TO_GUARD_OTHERS_TRIPPED:
					l_temp_1 = 0x0001;
					Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm = 0;
					Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm = 0;
					for (i=0;i<MAX_NUMBER_LP_COMMANDS;i++)
					{
						if(((g_NSD_Config->NSD[0].CommandApplication[i] == DIRECT)||(g_NSD_Config->NSD[0].CommandApplication[i] == COMMAND_OFF)))
						{
							Output_At_RxAlarmCh1.Reset_Command_Mask_RxAlarm |=	l_temp_1<<i;
						}
						else
						{
							Output_At_RxAlarmCh1.Set_Command_Mask_RxAlarm |= l_temp_1<<i;

						}
					}
					break;
				default:
					ASSERT(FALSE);
			} 
			Output_At_RxAlarmCh1.Channel_activ = ON;
			Output_At_RxAlarmCh2.Channel_activ = OFF;
			break;
		default:
			ASSERT(FALSE);
	}
	
	
	

}
/*****************************************************************************
  Function:		Set_Output_At_RxAlarm()
  Description:	- Decrement the system start up counter
				- Set the OR and AND maske to set the output in case of link alarm
				  to there predefined state!
				- Set/reset the alarm "output set to predefined state"
  Inputs:		-
  Outputs:		- Rx_Alarm_OR_Mask
				- Rx_Alarm_AND_Mask
  Return:		-
  Globals:		-
  Important:	Function must call in a 106Hz case of main programm
				For freeze this function is not used!
				The state of the paramter ChangeOutput is set in the function Delay_Hold_RxAlarm!

******************************************************************************/
//
 void SEGMENT_EXT_CODE Set_Output_At_RxAlarm(unsigned long *Rx_Alarm_OR_Mask, unsigned long *Rx_Alarm_AND_Mask, long Channel)
{

	t_Output_At_Alarm *ptr_Output_At_RxAlarm;

	// ***********************************************************************
	// *** Check channel *****************************************************
	// ***********************************************************************
	if(Channel == TPE1)
	{
		ptr_Output_At_RxAlarm = &Output_At_RxAlarmCh1;
	}
	else
	{
		if(Channel == TPE2)
		{
			ptr_Output_At_RxAlarm = &Output_At_RxAlarmCh2;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	//
	if(ptr_Output_At_RxAlarm->Channel_activ == ON)
	{
		// *******************************************************************
		// *** channel is on *************************************************
		// *******************************************************************
		if(ptr_Output_At_RxAlarm->Start_up_Delay == 0)
		{
			//
			// start up delay over
			//
			if(ptr_Output_At_RxAlarm->State_At_Alarm != OUTPUT_AT_RX_ALARM_FROZEN)
			{
				//
				// freeze is not configured
				// 
				if(ptr_Output_At_RxAlarm->ChangeOutput)
				{
					// ***************************************************************
					// *** alarm is activ and pick up time is over set output to defined state
					// ***************************************************************
					switch(ptr_Output_At_RxAlarm->State_At_Alarm)
					{
						case OUTPUT_AT_RX_ALARM_UNAFFECTED:
							ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = OR_NO_CHANGE;		// 0x00000000
							ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = RESET_NO_COM;	// 0xffffffff
							break;
						case OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE:
							ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = OR_NO_CHANGE;		// 0x00000000
							// @Pl_ext
							ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = (~(ALL_LP_AES_RX_COMMANDS|ALL_LP_COMMANDS_A_F));
							*(ptr_Output_At_RxAlarm->ptrRxAlarm) |=NSD_OUTPUT_SET_ALARM;
							break;
						case OUTPUT_AT_RX_ALARM_FROZEN:
							//ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm;
							//ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = (~ALL_LP_COMMANDS);
							//*(ptr_Output_At_RxAlarm->ptrRxAlarm) |=NSD_OUTPUT_SET_ALARM;
							break;
						case OUTPUT_AT_RX_ALARM_DIRECT_TO_GUARD_OTHERS_TRIPPED:
							ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->Set_Command_Mask_RxAlarm;			// set permissive and blocking commands on
							ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = (~ptr_Output_At_RxAlarm->Reset_Command_Mask_RxAlarm);	// reset direct commands 
							*(ptr_Output_At_RxAlarm->ptrRxAlarm) |=NSD_OUTPUT_SET_ALARM;
							break;
						default:
							ASSERT(FALSE);
					} // end switch
				}
				else
				{
					// *************************************************************
					// *** no alarm activ or pick up time not over ****************
					// *************************************************************
					// set AND and OR Maske for no influence to the command output
					ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = OR_NO_CHANGE;		// 0x00000000
					ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = RESET_NO_COM;	// 0xffffffff
					*(ptr_Output_At_RxAlarm->ptrRxAlarm) &=(~NSD_OUTPUT_SET_ALARM);
				}
				// update AND_Mask and OR_Mask for main
				*Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask;
				*Rx_Alarm_AND_Mask = ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask;
			}
			else
			{
				// freeze is configured -> do nothing 
			}
		}
		else
		{
			// start up delay not over 
			ptr_Output_At_RxAlarm->Start_up_Delay -=1;	// decrement counter
		}
	}
	else
	{
		// channel is off do nothing
	}
}
//
/*****************************************************************************
  Function:		Delay_Hold_RxAlarm()
  Description:	-Main function of this function is the pick up and hold time state machine,
				 which is used to set the command outputs to a predefined state in case of 
				 nsd link alarm
				- set/reset the pararmeter ChangeOutput
  Inputs:		
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	Function must call in a 106Hz case of main programm
				This function Delay_Hold_RxAlarm() is designed to use in a 
				10ms case of the dsp_main_NSD600 programm. It is used to 
				influence the Rx Commands during a receiver alarm!
				Changing of the rx-alarms do not influence the delay and 
				hold time i.e. the change from SNR to Rx-level alarm will not
				start the delay time again of the rx-alarm to change the 
				commands output during a rx alarm! 
				Not used for freeze!!
******************************************************************************/
//
 void SEGMENT_EXT_CODE Delay_Hold_RxAlarm(long Channel)
{

	t_Output_At_Alarm *ptr_Output_At_RxAlarm;

	// ***********************************************************************
	// *** Check channel *****************************************************
	// ***********************************************************************
	if(Channel == TPE1)
	{
		ptr_Output_At_RxAlarm = &Output_At_RxAlarmCh1;
	}
	else
	{
		if(Channel == TPE2)
		{
			ptr_Output_At_RxAlarm = &Output_At_RxAlarmCh2;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	//
	if(ptr_Output_At_RxAlarm->Channel_activ == ON)
	{
		// *******************************************************************
		// *** channel is on *************************************************
		// *******************************************************************
		if(ptr_Output_At_RxAlarm->Start_up_Delay == 0)
		{
			//
			// system start up delay over
			//
			if(ptr_Output_At_RxAlarm->State_At_Alarm != OUTPUT_AT_RX_ALARM_FROZEN)
			{
				//
				// freeze is not configured -> use pick up and hold time state machine
				//
				switch(ptr_Output_At_RxAlarm->State_Hold_Delay)
					{
					case e_Idle_RX_Alarm:
						if((*(ptr_Output_At_RxAlarm->ptrRxAlarm) & ptr_Output_At_RxAlarm->Maske_Rx_Alarm) != 0)
						{
							ptr_Output_At_RxAlarm->State_Hold_Delay = e_Delay_RX_Alarm;
							ptr_Output_At_RxAlarm->Delay_time = ptr_Output_At_RxAlarm->Set_Pickup_time;
						}
						else
						{
							
						}
						ptr_Output_At_RxAlarm->ChangeOutput = FALSE;
						break;
					case e_Delay_RX_Alarm:
						if((*(ptr_Output_At_RxAlarm->ptrRxAlarm) & ptr_Output_At_RxAlarm->Maske_Rx_Alarm) == 0)
						{
							ptr_Output_At_RxAlarm->State_Hold_Delay = e_Idle_RX_Alarm;
							ptr_Output_At_RxAlarm->ChangeOutput = FALSE;
						}
						else
						{
							ptr_Output_At_RxAlarm->Delay_time -=1;
							if(ptr_Output_At_RxAlarm->Delay_time <= 0)
							{
								ptr_Output_At_RxAlarm->ChangeOutput = TRUE;
								ptr_Output_At_RxAlarm->State_Hold_Delay = e_Hold_RX_Alarm;
								ptr_Output_At_RxAlarm->Hold_time = ptr_Output_At_RxAlarm->Set_Hold_time;
							}
							else
							{

							}
						}
						break;
					case e_Hold_RX_Alarm:
						if((*(ptr_Output_At_RxAlarm->ptrRxAlarm) & ptr_Output_At_RxAlarm->Maske_Rx_Alarm) == 0)
						{
							ptr_Output_At_RxAlarm->Hold_time -=1;
							if(ptr_Output_At_RxAlarm->Hold_time <= 0)
							{
								ptr_Output_At_RxAlarm->State_Hold_Delay = e_Idle_RX_Alarm;	
								ptr_Output_At_RxAlarm->ChangeOutput = FALSE;
							}
							else
							{

							}
						}
						else
						{
							ptr_Output_At_RxAlarm->Hold_time = ptr_Output_At_RxAlarm->Set_Hold_time;
						}
						break;
					default:
						ASSERT(FALSE);
				}
			}
			else
			{
				// freeze is configured -> do not use pick up and hold time state machine
			}
		}
		else
		{
			// start up delay not over
			// -> decrement counter will be done in function Set_Output_At_RxAlarm()
		}
	}
	else
	{
		// channel is off do nothing
	}
}
//
/*****************************************************************************
  Function:		Freeze_CommandOutput()
  Description:	Save the last state Rx command or guard without Rx alarm
				Trip or guard must be active and ChangeOutput must not be active 
				to save the last state. 
				The alarm "output set to predefined state" should be suppresed for freeze!
				Only a rx command will be freeze and hold for the defined time HOLD_TIME_FREEZE.
  Inputs:		Rx_Command_Ch1/2, RxGuardStateCh1/2, 
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	Function must call in the real time part of the main programm

******************************************************************************/
//     
void Freeze_CommandOutput(long RxCommand, BOOL GuardState, unsigned long *Rx_Alarm_OR_Mask, unsigned long*Rx_Alarm_AND_Mask, long Channel)
{

	t_Output_At_Alarm *ptr_Output_At_RxAlarm;
	BOOL Freeze = FALSE;

	// ***********************************************************************
	// *** Check channel *****************************************************
	// ***********************************************************************
	if(Channel == TPE1)
	{
		ptr_Output_At_RxAlarm = &Output_At_RxAlarmCh1;
	}
	else
	{
		if(Channel == TPE2)
		{
			ptr_Output_At_RxAlarm = &Output_At_RxAlarmCh2;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	//
	if(ptr_Output_At_RxAlarm->Channel_activ == ON)
	{
		// *******************************************************************
		// *** channel is on *************************************************
		// *******************************************************************
		//
		if((ptr_Output_At_RxAlarm->Start_up_Delay == 0) && (ptr_Output_At_RxAlarm->State_At_Alarm == OUTPUT_AT_RX_ALARM_FROZEN))
		{
			// ***************************************************************
			// *** system start up delay over and configuration freeze is set*
			// ***************************************************************
			//
			if(RxCommand != 0)
			{
				Freeze = FALSE;
				// -> update last state for freezing 
				if(ptr_Output_At_RxAlarm->ChangeOutput == FALSE)
				{
					ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm = RxCommand;
				}
				else
				{
					// freeze hold is still on do not save new state
				}
			}
			else
			{
				// no trip activ
				if((*(ptr_Output_At_RxAlarm->ptrRxAlarm) & NSD_TRIP_AND_GUARD_ALARM) == 0)
				{
					// trip and guard alarm is not activ
					if(GuardState)
					{
						// guard is activ
						Freeze = FALSE;
						if(ptr_Output_At_RxAlarm->ChangeOutput == FALSE)
						{
							ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm = 0;
							//
						}
						else
						{
							// freeze hold is still on do not save new state
						}
					}
					else
					{
						// no guard and no trip and no "trip and guard" activ
						// -> activate freeze 
						//ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm;
						//ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = (~ALL_LP_COMMANDS);
						//*(ptr_Output_At_RxAlarm->ptrRxAlarm) |=NSD_OUTPUT_SET_ALARM;
						Freeze = TRUE;

					}
				}
				else
				{
					// trip and guard alarm is activ 
					//  -> activate freeze
					//ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm;
					//ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = (~ALL_LP_COMMANDS);
					//*(ptr_Output_At_RxAlarm->ptrRxAlarm) |=NSD_OUTPUT_SET_ALARM;
					Freeze = TRUE;
				}
			}
			//
			//state machine to hold freezed command outputs
			// 
			switch(ptr_Output_At_RxAlarm->State_Hold_Delay)
			{
					case e_Idle_RX_Alarm:
						if(Freeze == TRUE)
						{
							//  -> activate freeze
							ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm;
							// @Pl_ext
							ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = (~ALL_LP_COMMANDS_A_F);
							// next line set as comment ->specifcation according hermann spiess from 23.9.2005
							// -> no event "output set to predefined state" if freeze is activ!!!!
							//*(ptr_Output_At_RxAlarm->ptrRxAlarm) |=NSD_OUTPUT_SET_ALARM;
							ptr_Output_At_RxAlarm->ChangeOutput = TRUE;
							ptr_Output_At_RxAlarm->State_Hold_Delay = e_Hold_RX_Alarm;
							ptr_Output_At_RxAlarm->Hold_time = ptr_Output_At_RxAlarm->Set_Hold_time;
						}
						else
						{
							// set AND and OR Maske for no influence to the command output
							ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = OR_NO_CHANGE;		// 0x00000000
							ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = RESET_NO_COM;	// 0xffffffff
							*(ptr_Output_At_RxAlarm->ptrRxAlarm) &=(~NSD_OUTPUT_SET_ALARM);
						}
						break;
					case e_Hold_RX_Alarm:
						if(Freeze == FALSE)
						{
							ptr_Output_At_RxAlarm->Hold_time -=1;
							if(ptr_Output_At_RxAlarm->Hold_time <= 0)
							{
								// set AND and OR Maske for no influence to the command output
								ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = OR_NO_CHANGE;		// 0x00000000
								ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = RESET_NO_COM;	// 0xffffffff
								*(ptr_Output_At_RxAlarm->ptrRxAlarm) &=(~NSD_OUTPUT_SET_ALARM);
								ptr_Output_At_RxAlarm->ChangeOutput = FALSE;
								ptr_Output_At_RxAlarm->State_Hold_Delay = e_Idle_RX_Alarm; 

							}
							else
							{
								// freeze hold time not over 
								if(ptr_Output_At_RxAlarm->LastCommandWithoutRxAlarm == 0)
								{
									//
									// last alarm free condition was guard -> do not freeze
									//
									// set AND and OR Maske for no influence to the command output
									ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask = OR_NO_CHANGE;		// 0x00000000
									ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask = RESET_NO_COM;	// 0xffffffff
									*(ptr_Output_At_RxAlarm->ptrRxAlarm) &=(~NSD_OUTPUT_SET_ALARM);
									ptr_Output_At_RxAlarm->ChangeOutput = FALSE;
									ptr_Output_At_RxAlarm->State_Hold_Delay = e_Idle_RX_Alarm;
								}
								else
								{

								}
							}
						}
						else
						{
							// freeze condition still activ set delay new
							ptr_Output_At_RxAlarm->Hold_time = ptr_Output_At_RxAlarm->Set_Hold_time;
						}

						break;
					default:
						ASSERT(FALSE);
			}
						
			// update AND_Mask and OR_Mask for main
			*Rx_Alarm_OR_Mask = ptr_Output_At_RxAlarm->Rx_Alarm_OR_Mask;
			*Rx_Alarm_AND_Mask = ptr_Output_At_RxAlarm->Rx_Alarm_AND_Mask;
			//
		}
		else
		{
			// start up delay not over or freeze is not configured
			//
			// -> decrement start up counter will be done in function Set_Output_At_RxAlarm()
		}
	}
	else
	{
		// channel is off do nothing	
	}

}
