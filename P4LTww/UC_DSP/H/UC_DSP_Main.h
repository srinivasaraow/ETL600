/*---------------------------------------------------------------------------*
 * $Workfile: UC_DSP_Main.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz PTUKT
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/UC_DSP_Main.h $
 * 
 * 1     20.01.05 8:33 Chchlee
 * 
 * 4     1.09.04 13:59 Rytz01
 * unused var removed
 * 
 * 3     18.06.04 15:47 Rytz01
 * 
 * 2     17.06.04 17:31 Rytz01
 * 
 * 1     17.06.04 16:44 Rytz01
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef UC_DSP_Main_H
#define UC_DSP_Main_H

/* INCLUDE FILES *************************************************************/
/* PUBLIC TYPE DECLARATIONS **************************************************/
extern volatile long IR_TimerCounter;
extern volatile long IR_BlockCounter;
extern volatile long *externalTDM_Rx_Ptr;

/* PUBLIC CONSTANT DECLARATIONS **********************************************/
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

/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/

#endif

