/*---------------------------------------------------------------------------*
 * $Workfile: FE_DSP_Main.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz PTUKT
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/FE_DSP_Main.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 8     17.11.04 14:43 Leeb02
 * MAINCASE_5 added 
 * 
 * 7     1.09.04 14:00 Rytz01
 * Unused vars rermoved
 * 
 * 6     5.07.04 15:58 Maag01
 * 
 * 5     23.06.04 16:30 Rytz01
 * 
 * 4     18.06.04 12:11 Rytz01
 * 
 * 3     17.06.04 16:00 Rytz01
 * 
 * 2     17.06.04 14:58 Rytz01
 * 
 * 1     17.06.04 13:45 Rytz01
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef FE_DSP_MAIN_H
#define FE_DSP_MAIN_H

/* INCLUDE FILES *************************************************************/
/* PUBLIC TYPE DECLARATIONS **************************************************/
extern volatile long IR_TimerCounter;
extern volatile long IR_BlockCounter;
extern volatile long *externalTDM_Tx_Ptr;
extern volatile long *externalTDM_Rx_Ptr;
extern volatile long *internalTDM_Tx_Ptr[2];
extern volatile long *internalTDM_Rx_Ptr[2];

/* PUBLIC CONSTANT DECLARATIONS **********************************************/
// case counter values
#define MAINCASE_0				0
#define MAINCASE_1				1    
#define MAINCASE_2				2
#define MAINCASE_3				3
#define MAINCASE_4				4

#define MAINCASE_5				5	// only used by channel simulator

#define SUBCASE_0				0
#define SUBCASE_1				1
#define SUBCASE_2				2
#define SUBCASE_3				3
#define SUBCASE_4				4
#define SUBCASE_5				5
#define SUBCASE_6				6
#define SUBCASE_7				7
#define SUBCASE_8				8
#define SUBCASE_9				9

/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/



#endif

