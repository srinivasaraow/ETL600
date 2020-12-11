/*---------------------------------------------------------------------------*
 * $Workfile: NSK_DSP_Main.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz PTUKT
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/NSK_DSP/H/NSK_DSP_Main.h $
 * 
 * 6     08-02-25 9:57 Chtognu
 * Merged changes from ad.equ.branch:
 * Interrupt counter changes
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 2     18.06.04 9:18 Rytz01
 * 
 * 1     18.06.04 9:04 Rytz01
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef NSK_DSP_MAIN_H
#define NSK_DSP_MAIN_H

/* INCLUDE FILES *************************************************************/
/* PUBLIC TYPE DECLARATIONS **************************************************/
extern volatile long          IR_TimerCounter;
extern volatile long          IR_BlockCounter;
extern volatile unsigned long IR_Counter;
/* PUBLIC CONSTANT DECLARATIONS **********************************************/
/* PUBLIC MACRO DEFINITIONS **************************************************/
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

/* PUBLIC FUNCTIONS **********************************************************/

#endif

