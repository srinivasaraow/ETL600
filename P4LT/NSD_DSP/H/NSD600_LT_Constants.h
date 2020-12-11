/*-------------------------------------------------------------------------*
 * $Workfile: NSD600_LT_Constants.h $
 * Part of	:   NSD600
 * Language	:   C
 * Created by:  Alexander Gygax (NSD570) / Peter Plüer (NSD600)
 * Remarks	:   Version 1.9 from NSD570 analog used for NSD600
 * Purpose	:   Constants for Loop Test
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/NSD_DSP/H/NSD600_LT_Constants.h $
 * 
 * 4     22.06.05 12:08 Chchlee
 * Code walkthrough
 * 
 * 3     2.06.05 9:43 Chpeplu
 * New define MANUAL_LT_FAILED_EVENT added
 * 
 * 2     23.05.05 17:03 Chpeplu
 * defines for LT_Events added, parameter for functions LT_Repeater() and
 * Determine_LT_Cases() changed
 * 
 * 1     19.01.05 15:34 Chchlee
 * 
 * 8     11.01.05 10:31 Chpeplu
 * defines and struct definitions moved to file LT_FunC_NSD600.c
 * 
 * 7     7.12.04 11:53 Chpeplu
 * function Determine_LT_Cases() additional  parameter added
 * 
 * 6     19.11.04 7:37 Plueer01
 * Add function ServeManualLoopTest_byHMI for manaul loop test by HMI
 * 
 * 5     2.11.04 14:32 Plueer01
 * Some additional defines added
 * 
 * 4     27.10.04 15:40 Plueer01
 * Some defines for loop test changed
 * 
 * 3     26.10.04 17:37 Plueer01
 * Some defines added
 * 
 * 2     22.10.04 16:47 Plueer01
 * back up changes it does not work yet
 *-------------------------------------------------------------------------*/


#ifndef NSD600_LT_CONSTANTS
#define NSD600_LT_CONSTANTS

/* INCLUDE FILES ***********************************************************/
#include "NSD_Types.h"

/***************************************************************************/
/*-------------------------- Other Constants ------------------------------*/
/***************************************************************************/

/* PUBLIC CONSTANT DECLARATIONS **********************************************/
#define	CYCLIC_LT_SENT_EVENT	0x00000001
#define MANUAL_LT_SENT_EVENT	0x00000002
#define	LT_REFLECTED_EVENT		0x00000004
#define	LT_FAILED_EVENT			0x00000008
#define	MANUAL_LT_FAILED_EVENT	0x00000010
//
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
void  Init_Cyclic_LT (t_NSD_DSP_Config *g_NSD_Config);
void  LT_Handler(long *ptr_TxCommand, long RxCommand, long Channel);
void  LT_Repeater(long *ptr_TxCommand, unsigned long *ptr_LT_Event, long RxCommand, long Channel);
//void  Determine_LT_Cases(long TxCommand, long RxCommand, long Channel);

// description of parameters
// *ptr_LT_Duration = 0 means manual looptest failed?
void  ServeManualLoopTest_byHMI(unsigned long *ptr_LoopTestMode, unsigned long *ptr_LT_Duration);

void  Determine_LT_Cases(long TxCommand, long RxCommand,t_NSD_Status *ptr_NSD_Status, unsigned long *ptr_LT_Event, long Channel);
void  Update_Dev_Status_LT(t_NSD_DevelopperStatus_Type *ptr_Develop_Status_NSD);

#endif

