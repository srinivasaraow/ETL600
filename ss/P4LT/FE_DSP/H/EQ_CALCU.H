/*-------------------------------------------------------------------------*
 * $Workfile: EQ_CALCU.H $
 * Part of	: ETL600 / P4LT FW & HMI600
 * Language	: C
 * Created by	: Bruno Busslinger, NKT1
 * Remarks	:  
 * Purpose	: clearer program code
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * $Log: /Source/HMI600/H/EQ_CALCU.H $
 * 
 * 2     18.05.04 11:48 Schnyd01
 * Initial version 
 * Based on MMI500 v. 3.20
 *-------------------------------------------------------------------------*/ 
///////////////////////////////////////////////////////////////////////////////////
#ifndef 	_EQ_CALCU
#define 	_EQ_CALCU

/*-----------------------LOCAL #define DEFINITIONS----------------------------*/
#define START_VALUE_SHIFT_REGISTER		0xffffffffL 
#define BIT_MASK_SHIFT_REGISTER			0x00000080L
#define BITMASK_FIRST_POSITION			0x00000001L
#define BITMASK_SECOND_POSITION			0x00000002L
#define FIRST_STATE_SHIFT_REGISTER		0x000000ffL
#define BITMASK_LAST_POSITION			0x0000007fL
#define SHIFT_BIT_1						1L
#define SHIFT_BIT_2						2L
#define SHIFT_BIT_3						3L
#define SHIFT_BIT_7						7L

#define ZWEI							2                                                                  






#define QUALITY_OUTPUT_FACTOR			10
#define MINIMUM_INTERPOLATE_DIFFERENCE	3

#ifndef CODECHECK
#define VOLTAGE_DB_FACTOR				20.0F
#define FLOAT_ONE						1.0F
#define FLOAT_ZERO_POINT_FIVE			0.5F
#define FLOAT_ZERO_F                    0.0F
#define FLOAT_TWO						2.0F
#define LONG_ONE_THOUSAND				1000L
#define ONE_THOUSAND					1000
#define TEN_THOUSAND					10000
#define BIG_FLOAT_NUMBER				999999999.99F
#endif


// Function Prototypes
void SetFloatArrayZero(float *ArrayPtr, long ArrayLength);
void CalculateTransferFunctionEqualizer(float *MeasureSamples, float Pilotfrequenz,
										float *RealPartTransferFunctionEqPtr, 
										float *ImagPartTransferFunctionEqPtr);
void InterpolationEqualizer(float *RealPartTransferFunctionEqPtr, float *ImagPartTransferFunctionEqPtr,
							float *AbsTransferFunctionEqInterpolatedPtr, float Pilotfrequenz);
void CalculateStepResponseEqualizer(float *RealPartTransferFunctionEqPtr, float *ImagPartTransferFunctionEqPtr,
									float *AbsTransferFunctionEqInterpolatedPtr, float *StepResponseEqualizerPtr,
									long *SchwerpunktPtr, long ETL_GroupDelayEqualisationEnabled);
#endif // 	_EQ_CALCU
									
