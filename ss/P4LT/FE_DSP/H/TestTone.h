/*-------------------------------------------------------------------------*
 * $Workfile: TestTone.h $
 * Part of	: ETL600
 * Language	: C
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/TestTone.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 2     22.07.04 16:50 Maag01
 * New function testToneAmpAdjust() integrated (previously shared with
 * pilotMod.c)
 * 
 * 1     30.04.04 15:21 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 *    
 * Initial Revision
 *-------------------------------------------------------------------------*/

#include "APLC_Types.h" 
#include "BiIIR_Filter.h"

#ifndef _TEST_TONE_H
#define _TEST_TONE_H
 

/*-----------------------TYPE AND STRUCTURE DEFINITIONS--------------------*/

typedef struct {

  float sampleFreq;     		// sample frequency 
  float oscFreq;        		// oscillator frequency 
  float a, b;           		// oscillator coefficients  
  float x1, x2;					// state variables
  
} TestToneOscDataType;


typedef struct {
  
  TestToneOscDataType osc;      // oscillator parameters
  FilterDataType filterData;  	// filter parameters
  
  float demodSample[2];       	// demodulated sample (I & Q part)
  float rxLevel;         		// low pass filtered receive level
  long  delayCounter;         	// delay of buzzer activation
  
} TestToneDataType; 


/*-----------------------GLOBAL FUNCTION DEFINITIONS-----------------------*/

void initTestToneCh1(t_TestToneConfig *ttConfig, TestToneDataType *ttData);
void initTestToneCh2(t_TestToneConfig *ttConfig, TestToneDataType *ttData);
void initTestToneCh3(t_TestToneConfig *ttConfig, TestToneDataType *ttData);
void testToneAmpAdjust(TestToneOscDataType *osc);
long detectTestTone(TestToneDataType *testTone);


#endif
