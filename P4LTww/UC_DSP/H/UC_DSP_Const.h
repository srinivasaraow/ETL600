/*-------------------------------------------------------------------------*
 * $Workfile: UC_DSP_Const.h $
 * Part of	: ETL600 / UC DSP
 * Language	: C
 * Created by: H.-J. Maag   
 * Remarks	:  
 * Purpose	: Constants definitions for UC DSP
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/UC_DSP_Const.h $
 * 
 * 1     20.01.05 8:33 Chchlee
 * 
 * 2     18.05.04 18:19 Maag01
 * 
 * 1     7.05.04 17:13 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef UC_DSP_CONST
#define UC_DSP_CONST

// signal processing constants
// An even number of samples must be used for interpolation functions
#define N_SAMPLES_IF1			6		// number of samples at intermediate sampling frequency 1 (64.0 kHz)
#define N_SAMPLES_IF2			24		// number of samples at intermediate sampling frequency 2 (256.0 kHz)
#define N_SAMPLES_IF3			96		// number of samples at intermediate sampling frequency 3 (1.024 MHz)
#define N_SAMPLES_RF			384 	// number of samples at output sampling frequency (4.096 MHz)
      
#endif
