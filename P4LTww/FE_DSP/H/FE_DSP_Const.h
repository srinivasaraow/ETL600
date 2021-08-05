/*-------------------------------------------------------------------------*
 * $Workfile: FE_DSP_Const.h $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by	:     
 * Remarks	:  
 * Purpose	: Constants definitions for FE DSP
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/FE_DSP_Const.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 6     22.07.04 16:52 Maag01
 * 
 * 5     18.06.04 13:38 Maag01
 * 
 * 4     17.06.04 18:26 Maag01
 * 
 * 3     7.05.04 15:17 Maag01
 * 
 * 2     7.05.04 15:16 Maag01
 * 
 * 1     30.04.04 15:03 Maag01
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

#ifndef _FE_DSP_CONST
#define _FE_DSP_CONST


// general contstants
#define DIV_BY_TWO                0.5               // substitution of division by multiplication

// sampling rates
#define AF_SAMPLE_FREQ_4K		10666.66666666667	// AF sampling frequency 4 kHz channel
#define INT_SAMPLE_FREQ			64000.00000000000	// intermediate sampling frequency

// number of samples
#define N_SAMPLES_INT2_C			4				// number of complex samples @ 21.33 kHz (main loop: 5.33 kHz)
#define N_SAMPLES_IF_C				12				// number of complex samples @ 64 kHz (main loop: 5.33 kHz)

// block length
#define BLOCK_LENGTH_R_4K			2       		// block length of real AF samples (4kHz Version)
#define BLOCK_LENGTH_C_4K			4       		// block length of complex AF samples (4kHz Version)

#define BLOCK_LENGTH_R_INT2			4				// block length of real samples @ 21.33 kHz
#define BLOCK_LENGTH_C_INT2			8				// block length of complex samples @ 21.33 kHz

#define BLOCK_LENGTH_R_IF			12				// block length of real samples @ intermediate sample frequency
#define BLOCK_LENGTH_C_IF			24				// block length of complex samples @ intermediate sample frequency

// limiter
#define LIMIT_FACTOR			1.2*0.5				//Reserve: 1.2; SSB-Faktor:0.5

// Switch
#define ON  1
#define OFF 0

   
#endif
