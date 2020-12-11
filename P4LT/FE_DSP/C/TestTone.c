/*-------------------------------------------------------------------------*
 * $Workfile: TestTone.c $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: Test tone generator and detector
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/C/TestTone.c $
 * 
 * 2     25.02.05 9:57 Chhamaa
 * Constants added from TTCONST.H
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 4     8.10.04 17:21 Maag01
 * 
 * 3     22.07.04 16:49 Maag01
 * New functions testToneOscillatorInit() and testToneAmpAdjust()
 * integrated (previously shared with pilotMod.c)
 * 
 * 2     14.07.04 12:18 Leeb02
 * Dependencies removed
 * 
 * 1     30.04.04 15:02 Maag01
 * Initial version based on ETL500 Version 4.20 (linkcont.c)
 * 
 * Initial Revision
 *-------------------------------------------------------------------------*/

/*-----------------------GLOBAL CONSTANTS----------------------------------*/

#include <math.h>
#include <stdlib.h>

#include "FE_DSP_Const.h"
#include "TestTone.h"         
#include "ttfilter.h"			// test tone filter coefficients


/*-----------------------LOCAL CONSTANTS-----------------------------------*/

#define TT_AMP_AJUST_CONST		  1.5		// amplitude adjustment constant

#define TT_ENV_SQR_SCALING_COEF	  4.0		// scaling of squared test tone envelope
#define TEST_TONE_LEVEL			  0.5		// level for test tone detector
#define TEST_TONE_DELAY			300			// [s*10e-2] delay of buzzer after test tone detection  
#define TT_COEF_1                 0.95      // test tone detector low pass filter constant 
#define TT_COEF_2                 0.05      // test tone detector low pass filter constant


/*------------------- LOCAL MACRO DEFINITIONS------------------------------*/

#define sqr(x) ((x)*(x))


/*-----------------------LOCAL DATA DEFINITIONS----------------------------*/

float dm ttFilterState1Ch1[TTF_SARRAY_LENGTH], ttFilterState2Ch1[TTF_SARRAY_LENGTH];
float dm ttFilterState1Ch2[TTF_SARRAY_LENGTH], ttFilterState2Ch2[TTF_SARRAY_LENGTH];
float dm ttFilterState1Ch3[TTF_SARRAY_LENGTH], ttFilterState2Ch3[TTF_SARRAY_LENGTH];


/*-----------------------IMPLEMENTATION------------------------------------*/


#pragma optimize_off 

/*-------------------------------------------------------------------------*/               
/* void testToneOscillatorInit(float oscFreq, float sampleFreq,      	   */
/*						    PilotOscDataType *osc)						   */ 
/*                                                                         */
/* Initializes osillator parameters 							           */
/*                                                                         */
/* Input: oscillator frequency, sample frequency, oscillator parameters    */
/* Output: initalized oscillator parameters		                           */
/*-------------------------------------------------------------------------*/  

void SEGMENT_EXT_CODE testToneOscillatorInit(float oscFreq, float sampleFreq, TestToneOscDataType *osc)
{ 
  register float wo;
 
  // initialize oscillator parameters
  wo = oscFreq*TWO_PI/sampleFreq;
  
  osc->sampleFreq = sampleFreq;
  osc->oscFreq = oscFreq;
  osc->x1 = 0.0;
  osc->x2 = -1.0;
  osc->a = cosf(wo);
  osc->b = sinf(wo);  
}


/*-------------------------------------------------------------------------*/               
/* void initTestToneCh1(void) 							 	   			   */
/*																		   */
/* Initializes the test tone for channel 1.							 	   */
/*                                                                         */
/* Input: 										     				       */
/* Output: 																   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE initTestToneCh1(t_TestToneConfig *ttConfig, TestToneDataType *ttData)
{
   // test tone initialisation
   testToneOscillatorInit(ttConfig->frequency, AF_SAMPLE_FREQ_4K, &ttData->osc);
   BiIIR_Filter_init(&ttData->filterData, testToneCoef, ttFilterState1Ch1, ttFilterState2Ch1, NUM_OF_TTF_SEC);
   
};


/*-------------------------------------------------------------------------*/               
/* void initTestToneCh2(void) 							 	   			   */
/*																		   */
/* Initializes the test tone for channel 2.							 	   */
/*                                                                         */
/* Input: 										     				       */
/* Output: 																   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE initTestToneCh2(t_TestToneConfig *ttConfig, TestToneDataType *ttData)
{
   // test tone initialisation
   testToneOscillatorInit(ttConfig->frequency, AF_SAMPLE_FREQ_4K, &ttData->osc);
   BiIIR_Filter_init(&ttData->filterData, testToneCoef, ttFilterState1Ch2, ttFilterState2Ch2, NUM_OF_TTF_SEC);
   
};

/*-------------------------------------------------------------------------*/               
/* void initTestToneCh3(void) 							 	   			   */
/*																		   */
/* Initializes the test tone for channel 3.							 	   */
/*                                                                         */
/* Input: 										     				       */
/* Output: 																   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE initTestToneCh3(t_TestToneConfig *ttConfig, TestToneDataType *ttData)
{
   // test tone initialisation
   testToneOscillatorInit(ttConfig->frequency, AF_SAMPLE_FREQ_4K, &ttData->osc);
   BiIIR_Filter_init(&ttData->filterData, testToneCoef, ttFilterState1Ch3, ttFilterState2Ch3, NUM_OF_TTF_SEC);
   
};


#pragma optimize_as_cmd_line

/*-------------------------------------------------------------------------*/               
/* void testToneAmpAdjust(PilotOscDataType *osc)						   */
/*																		   */
/* In order to garantee a constant oscillator amplitude internal states    */
/* have to be adjusted periodically.                                       */
/*                                                                         */
/* Input: oscillator parameters osc      							       */
/* Output: adjusted oscillator states 			                           */
/*-------------------------------------------------------------------------*/

void testToneAmpAdjust(TestToneOscDataType *osc)
{
  register float h, v;    
  
  h = DIV_BY_TWO*(sqr(osc->x1*osc->b) + sqr(osc->x1*osc->a - osc->x2)); 
  v = TT_AMP_AJUST_CONST - h;       // amplidtude adjustment factor 
  
  if ((h == 0.0) || (v <= 0.0)) {   // reset states to initial conditions 
    osc->x1 =  0.0;   
    osc->x2 = -1.0;
  }     
  else {                 			// state adjustment 
    osc->x1 = v*osc->x1;
    osc->x2 = v*osc->x2;
  }   
}


/*-------------------------------------------------------------------------*/               
/* long detectTestTone(TestToneDemodDataType *testTone)				       */
/*                                                                         */
/* Calculates the received test tone level. If the low pass filtered test  */
/* tone level exceeds the predefined TEST_TONE_LEVEL the output is         */
/* activated after the TEST_TONE_DELAY.									   */
/*																		   */
/* Input: base band samples demodSample (I & Q part) calculated by         */
/*        the function testTone()                                          */
/* Output: returns true in case of test tone present else returns false    */
/*-------------------------------------------------------------------------*/
 
long detectTestTone(TestToneDataType *testTone)
{
  register float rxEnvSqr;

  // compute squared envelope of rx test tone  
  rxEnvSqr = sqr(testTone->demodSample[0]) + sqr(testTone->demodSample[1]);
  rxEnvSqr *= TT_ENV_SQR_SCALING_COEF;

  // low pass filtering  of rxEnvSqr
  testTone->rxLevel = TT_COEF_1*testTone->rxLevel + TT_COEF_2*rxEnvSqr; 
  
  // compare rx level with threshold
  if (testTone->rxLevel < TEST_TONE_LEVEL) testTone->delayCounter = TEST_TONE_DELAY;
  else if (testTone->delayCounter > 0) testTone->delayCounter--;
  
  // return test tone detected
  return(!testTone->delayCounter);
}
