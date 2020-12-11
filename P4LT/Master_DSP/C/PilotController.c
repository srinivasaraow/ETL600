/*-------------------------------------------------------------------------*
 * $Workfile: PilotController.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : P.Plueer
 * Remarks      :  
 * Purpose      : Set Pilot enable and AGC blocking
 * Copyright    : ABB Switzerland Ltd., 2005
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/Master_DSP/C/PilotController.c $ 
 * 
 * 18    12-10-03 10:45 Chhamaa
 * New TPE mode INT_TELPROT_MODE_HSD_25 added.
 * 
 * 14    24.11.06 11:01 Chpeplu
 * Define CMD_LP_T_AES_RX_MASK changed for Cmd E and F
 * 
 * 13    14.11.06 13:37 Chpeplu
 * Define CMD_LP_T_AES_RX_MASK changed, In function Pilotcontroller()
 * define COM_T replaced by TX_COM_T, function Init_PilotController()
 * changed for custom mode
 * 
 * 12    13.09.06 13:11 Chhamaa
 * More AES modifications
 * 
 * 11    12.09.06 14:22 Chhamaa
 * AES modifications
 * 
 * 10    21.08.06 16:04 Chhamaa
 * AES case modified
 * 
 * 7     31.01.06 16:26 Chpeplu
 * new mode INT_TELPROT_MODE_HSD supported
 * 
 * 5     29.06.05 8:13 Chchlee
 * bool removed
 * 
 * 4     14.03.05 9:56 Chpeplu
 * define COMMAND_MASK_A_D_T changed to COMMAND_MASK_A_F_T
 * 
 * 3     9.03.05 11:00 Chpeplu
 * New config used (NSDMode and u_NSD_AES)
 * 
 * 2     21.01.05 7:47 Chpeplu
 * Command Maske for Tx and Rx Command added
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 3     19.01.05 13:00 Chpeplu
 * pilot enable and agc blocking signal generation implemented
 * 
 * 2     18.01.05 17:14 Chpeplu
 * First running version 
 * 
 * 1     18.01.05 14:53 Chpeplu
 * Inital version
 
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include "tdm_int_types.h"
#include "configuration.h"
#include "NSD_DSP_Types.h"
#include "AES600.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

//Rx Command maske  with loop test
// @Pl_ext
#define CMD_LP_T_AES_RX_MASK	(ALL_LP_AES_RX_COMMANDS | RX_COM_T | USED_CMD_E_RX | USED_CMD_F_RX)


/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {
	BOOL	ITS_In_Ch1;
	BOOL	ITS_In_Ch2;
	BOOL	ITS_In_Ch3;
	BOOL	ITS_Ch1_Own_Guard;
	BOOL	ITS_Ch2_Own_Guard;

} Type_PilotController;


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

t_AllDSP_input_tdm_block *g_intTDM_RxStruct_Ptr;
t_Master_output_tdm_block *g_intTDM_TxStruct_Ptr;
Type_PilotController t_PilotController;


/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		Init_PilotController()
  Description:	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	-

*******************************************************************************/

void SEGMENT_EXT_CODE Init_PilotController(void)
{
	t_P4LT_Config * p_P4LT_Config = configuration_getPCurrentConfig();
	// @Pl_ext
	switch((p_P4LT_Config->ITS.mode & INT_TPE_MODE_MASK))
	{
		case INT_TELPROT_MODE_NONE:
			t_PilotController.ITS_In_Ch1 = FALSE;
			t_PilotController.ITS_In_Ch2 = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			//
			break;
		case INT_TELPROT_MODE_AES:
			t_PilotController.ITS_In_Ch1 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.AES.AES600_Mode  & AES600_NSD_ETL_GUARD) == AES600_NSD_ETL_GUARD) t_PilotController.ITS_Ch1_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch1_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch2 = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			//
			break;
		case INT_TELPROT_MODE_NSD1_APLC1:
			t_PilotController.ITS_In_Ch1 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch1_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch1_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch2 = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			break;
		case INT_TELPROT_MODE_NSD1_DPLC:
			t_PilotController.ITS_In_Ch1 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch1_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch1_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch2 = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:
			t_PilotController.ITS_In_Ch1 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch1_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch1_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch2 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[1].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch2_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch2_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			break;
		case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:
			t_PilotController.ITS_In_Ch1 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch1_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch1_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch2 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[1].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch2_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch2_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			//
			break;
		case INT_TELPROT_MODE_HSD:
		case INT_TELPROT_MODE_HSD_25:
			t_PilotController.ITS_In_Ch1 = TRUE;
			if((p_P4LT_Config->ITS.u_NSD_AES.NSD[0].NsdMode & NSD_MODE_OWN_GUARD_USED) == NSD_MODE_OWN_GUARD_USED) t_PilotController.ITS_Ch1_Own_Guard = TRUE;
			else t_PilotController.ITS_Ch1_Own_Guard = FALSE;
			t_PilotController.ITS_In_Ch2 = FALSE;
			t_PilotController.ITS_In_Ch3 = FALSE;
			break;

		default:
			// unknowed mode of NSD or AES
			
			//
			break;
	}
}


/*****************************************************************************
  Function:		PilotController()
  Description:	
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	

*******************************************************************************/

void SEGMENT_EXT_CODE PilotController(volatile long * internalTDM_Rx_Ptr, volatile long * internalTDM_Tx_Ptr)
{

	long	Rx_Cmd_Ch1, Rx_Cmd_Ch2;
	long	Tx_Cmd_Ch1, Tx_Cmd_Ch2;
	BOOL	Disconnect;
	BOOL	boostRequestCh1;
	BOOL	boostRequestCh2;


	// get actual internal tdm pointer for rx and tx 
	g_intTDM_RxStruct_Ptr = (t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr;
	g_intTDM_TxStruct_Ptr = (t_Master_output_tdm_block*)internalTDM_Tx_Ptr;

	// get actual nsd rx commands for ch1 and ch2
	Rx_Cmd_Ch1 = g_intTDM_RxStruct_Ptr->Channel1_OutputCommandStatusData_TS13 & CMD_LP_T_AES_RX_MASK;
	Rx_Cmd_Ch2 = g_intTDM_RxStruct_Ptr->Channel2_OutputCommandStatusData_TS15 & CMD_LP_T_AES_RX_MASK;

	//get actual nsd tx commands for ch1 and ch2
	Tx_Cmd_Ch1 = g_intTDM_RxStruct_Ptr->Channel1_InputCommandStatusData_TS12;
	Tx_Cmd_Ch2 = g_intTDM_RxStruct_Ptr->Channel2_InputCommandStatusData_TS14;

	boostRequestCh1 = g_intTDM_RxStruct_Ptr->BoostMgrInput_TS63.TPE1_BoostReq;
	boostRequestCh2 = g_intTDM_RxStruct_Ptr->BoostMgrInput_TS63.TPE2_BoostReq;

	// get inverse actual disconnect signal from BoostManager
	if(g_intTDM_TxStruct_Ptr->boostManagerOutput.disconnect == TRUE) Disconnect = FALSE;
	else Disconnect = TRUE;
	//
	// **********************************************************************************
	// *** Generate pilot enabel signal for Ch1 - Ch3  **********************************
	// **********************************************************************************
	//
	// *******************************************************************
	// *** Pilot enable for Ch1 ******************************************
	// *******************************************************************
	//
	if(t_PilotController.ITS_In_Ch1 == FALSE)
	{
		// no nsd in ch1 -> use disconnect signal for pilot enable 
		g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch1 = Disconnect; // pilot switch == disconnect
	}
	else
	{
		// nsd is configured in ch1
		if(t_PilotController.ITS_Ch1_Own_Guard == FALSE)
		{
			// nsd uses Etl pilot
			// @Pl_ext
			if(boostRequestCh1 || (Tx_Cmd_Ch1 & TX_COM_T)) // boost request or loop test command
			{
				g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch1 = FALSE;	// switch off etl pilot
			}
			else
			{
				g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch1 = TRUE;	// switch on etl pilot
			}
		}
		else
		{
			// nsd uses nsd own guard
			g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch1 = TRUE;						// do not switch off etl pilot
		}
	}
	//
	// *******************************************************************
	// *** Pilot enable for Ch2 ******************************************
	// *******************************************************************
	//
	if(t_PilotController.ITS_In_Ch2 == FALSE)
	{
		// no nsd in ch2 -> use disconnect signal for pilot enable 
		g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch2 = Disconnect; // pilot switch == disconnect
	}
	else
	{
		// nsd is configured in ch2
		if(t_PilotController.ITS_Ch2_Own_Guard == FALSE)
		{
			// nsd uses Etl pilot
			// @Pl_ext
			if(boostRequestCh2 || (Tx_Cmd_Ch2 & TX_COM_T)) // boost request or loop test command
			{
				g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch2 = FALSE;	// switch off etl pilot
			}
			else 
			{
				g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch2 = TRUE;	// switch on etl pilot
			}
		}
		else
		{
			// nsd uses nsd own guard
			g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch2 = TRUE;						// do not switch off etl pilot
		}
	}
	//
	// *******************************************************************
	// *** Pilot enable for Ch3 ******************************************
	// *******************************************************************
	//
	g_intTDM_TxStruct_Ptr->boostManagerOutput.Pilot_Enable_Ch3 = Disconnect; // pilot switch == disconnect
	//
	// **********************************************************************************
	// *** Generate agc-blocking signal for Ch1 - Ch3  **********************************
	// **********************************************************************************
	//
	// *******************************************************************
	// *** agc blocking for Ch1 ******************************************
	// *******************************************************************
	//
	if(t_PilotController.ITS_In_Ch1 == FALSE)
	{
		// no ITS in ch1 -> no ITS used in any channel at all ->agc blocking not set 
		g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch1 = FALSE; // agc blocking not set
	}
	else
	{
		// ITS in ch1 active
		if(t_PilotController.ITS_Ch1_Own_Guard == FALSE)
		{
			// nsd uses Etl pilot
			if(Rx_Cmd_Ch1 == 0) g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch1 = FALSE;		// agc blocking not set
			else g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch1 = TRUE;						// rx-command active agc blocking set
		}
		else
		{
			// nsd in ch1 uses own pilot signal -> do not block agc
			g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch1 = FALSE;	
		}
	}
	// *******************************************************************
	// *** agc blocking for Ch2 ******************************************
	// *******************************************************************
	//
	if(t_PilotController.ITS_In_Ch2 == FALSE)
	{
		// no ITS in ch2 ->   if rx cmd in ch1 is active -> do blocking agc
		if(Rx_Cmd_Ch1 == 0) g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch2 = FALSE;		// agc blocking not set
		else g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch2 = TRUE;	
	}
	else
	{
		// ITS in ch2 active
		if(t_PilotController.ITS_Ch2_Own_Guard == FALSE)
		{
			// nsd in ch2 uses Etl pilot
			if(Rx_Cmd_Ch2 == 0) g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch2 = FALSE;		// agc blocking not set
			else g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch2 = TRUE;						// rx-command active agc blocking set
		}
		else
		{
			// nsd in ch2 uses own pilot signal -> do not block agc
			g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch2 = FALSE;	
		}
	}
	// *******************************************************************
	// *** agc blocking for Ch3 ******************************************
	// *******************************************************************
	//
	if((Rx_Cmd_Ch1 == 0) && (Rx_Cmd_Ch2 == 0)) g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch3 = FALSE;
	else g_intTDM_TxStruct_Ptr->boostManagerOutput.AGC_Blocking_Ch3 = TRUE;		// rx cmd in ch1 or ch2 is activ -> set agc blocking in ch3
	//
}


