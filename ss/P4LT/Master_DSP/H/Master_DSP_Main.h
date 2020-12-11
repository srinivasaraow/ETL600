/*-------------------------------------------------------------------------*
 * $Workfile: Master_DSP_Main.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/Master_DSP_Main.h $ 
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 3     16.06.04 17:17 Rytz01
 * 
 * 2     27.04.04 11:52 Maag01
 * Idle counter defines and declarations moved to idleCounter.h
 * 
 * 1     27.04.04 10:09 Leeb02
*-------------------------------------------------------------------------*/

#ifndef MASTER_DSP_MAIN_H
#define MASTER_DSP_MAIN_H


/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
// case counter values
#define MAINCASE_0				0
#define MAINCASE_1				1
#define MAINCASE_2				2
#define MAINCASE_3				3
#define MAINCASE_4				4
#define MAINCASE_5				5
#define MAINCASE_6				6
#define MAINCASE_7				7
#define MAINCASE_8				8
#define MAINCASE_9				9

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


/* PUBLIC TYPE DECLARATIONS ***************************************************/
extern volatile long IR_TimerCounter;
extern volatile long IR_BlockCounter;

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/



#endif
