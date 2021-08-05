/*----------------------------------------------------------------------------*
 * $Workfile: testPlcLink.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/FE_DSP/H/testPlcLink.h $
 * 
 * 9     11-03-11 17:57 Chhamaa
 * rfLoopbackOn added.
 * 
 * 2     21.02.05 16:44 Chhamaa
 * afcDisable removed from struct t_TestSwitches
 * 
 * 1     16.02.05 17:51 Chhamaa
 * 
 * 1     8.02.05 11:10 Chhamaa
 * Initial version
 *
 *----------------------------------------------------------------------------*/
 
#ifndef TEST_PLC_LINK_H
#define TEST_PLC_LINK_H


/* INCLUDE FILES **************************************************************/

#include "P4LT_Types.h"


/* GLOBAL TYPE DECLARATIONS ***************************************************/

typedef struct {

	BOOL agcDisable[N_APLC_CHANNELS];
	BOOL pilotDisable[N_APLC_CHANNELS];
	BOOL testToneEnable[N_APLC_CHANNELS];
	BOOL rfLoopbackOn;

} t_TestSwitches;


/* PUBLIC FUNCTIONS ***********************************************************/

unsigned long testPlcLink_getTestSwitches(t_TestSwitches *tsStruct);
void testPlcLink_setTestSwitches(unsigned long testSwitches, t_TestSwitches *tsStruct);

#endif
