/*-------------------------------------------------------------------------*
 * $Workfile: testPlcLink.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Functions to set and get PLC link test config
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/FE_DSP/C/testPlcLink.c $ 
 * 
 * 9     11-03-11 17:58 Chhamaa
 * rfLoopbackOn added.
 * 
 * 2     21.02.05 16:45 Chhamaa
 * afcDisable removed
 * 
 * 1     16.02.05 17:51 Chhamaa
 * 
 * 1     8.02.05 11:09 Chhamaa
 * Initial version
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES *************************************************************/

#include "basetype.h"
#include "testPlcLink.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/


/* LOCAL VARIABLES ***********************************************************/


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		testPlcLink_getTestSwitches()	
  Description:	The test switch settings are read from tsStruct and returned
				as an unsigned long value.
  Inputs:		Pointer to test switch struct (*tsStruct)
  Outputs:		none
  Return:		test switches
  Globals:		none	
  Important:	-

******************************************************************************/

unsigned long SEGMENT_EXT_CODE testPlcLink_getTestSwitches(t_TestSwitches *tsStruct)
{
	unsigned long testSwitches = 0;

	
	if (tsStruct->agcDisable[APLC_CH1]) testSwitches |= TEST_SWITCH_DISABLE_AGC_CH1;
	if (tsStruct->agcDisable[APLC_CH2]) testSwitches |= TEST_SWITCH_DISABLE_AGC_CH2;
	if (tsStruct->agcDisable[APLC_CH3]) testSwitches |= TEST_SWITCH_DISABLE_AGC_CH3;

	if (tsStruct->pilotDisable[APLC_CH1]) testSwitches |= TEST_SWITCH_DISABLE_PILOT_CH1;
	if (tsStruct->pilotDisable[APLC_CH2]) testSwitches |= TEST_SWITCH_DISABLE_PILOT_CH2;
	if (tsStruct->pilotDisable[APLC_CH3]) testSwitches |= TEST_SWITCH_DISABLE_PILOT_CH3;

	if (tsStruct->testToneEnable[APLC_CH1]) testSwitches |= TEST_SWITCH_ENABLE_TESTTONE_CH1;
	if (tsStruct->testToneEnable[APLC_CH2]) testSwitches |= TEST_SWITCH_ENABLE_TESTTONE_CH2;
	if (tsStruct->testToneEnable[APLC_CH3]) testSwitches |= TEST_SWITCH_ENABLE_TESTTONE_CH3;

	if (tsStruct->rfLoopbackOn) testSwitches |= TEST_SWITCH_LOCAL_RF_LOOPBACK_ON;

	return(testSwitches);

}


/*****************************************************************************
  Function:		testPlcLink_setTestSwitches()	
  Description:	The test switch struct is set according to testSwitches.
  Inputs:		testSwitches, pointer to test switch struct (*tsStruct)
  Outputs:		tsStruct with new settings
  Return:		none
  Globals:		none	
  Important:	-

******************************************************************************/

void SEGMENT_EXT_CODE testPlcLink_setTestSwitches(unsigned long testSwitches, 
														 t_TestSwitches *tsStruct)
{

	// reset test switch struct
	memset(tsStruct, 0x0, sizeof(t_TestSwitches));

	// set test switch struct according testSwitches
	if (testSwitches & TEST_SWITCH_DISABLE_AGC_CH1) tsStruct->agcDisable[APLC_CH1] = TRUE;
	if (testSwitches & TEST_SWITCH_DISABLE_AGC_CH2) tsStruct->agcDisable[APLC_CH2] = TRUE;
	if (testSwitches & TEST_SWITCH_DISABLE_AGC_CH3) tsStruct->agcDisable[APLC_CH3] = TRUE;

	if (testSwitches & TEST_SWITCH_DISABLE_PILOT_CH1) tsStruct->pilotDisable[APLC_CH1] = TRUE;
	if (testSwitches & TEST_SWITCH_DISABLE_PILOT_CH2) tsStruct->pilotDisable[APLC_CH2] = TRUE;
	if (testSwitches & TEST_SWITCH_DISABLE_PILOT_CH3) tsStruct->pilotDisable[APLC_CH3] = TRUE;

	if (testSwitches & TEST_SWITCH_ENABLE_TESTTONE_CH1) tsStruct->testToneEnable[APLC_CH1] = TRUE;
	if (testSwitches & TEST_SWITCH_ENABLE_TESTTONE_CH2) tsStruct->testToneEnable[APLC_CH2] = TRUE;
	if (testSwitches & TEST_SWITCH_ENABLE_TESTTONE_CH3) tsStruct->testToneEnable[APLC_CH3] = TRUE;

	if (testSwitches & TEST_SWITCH_LOCAL_RF_LOOPBACK_ON) tsStruct->rfLoopbackOn = TRUE;
}

