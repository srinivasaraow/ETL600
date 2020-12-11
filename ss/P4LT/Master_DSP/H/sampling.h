/*----------------------------------------------------------------------------*
 * $Workfile: sampling.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/sampling.h $
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 4     18.11.04 13:21 Maag01
 * 
 * 3     17.11.04 13:48 Maag01
 * 
 * 2     15.11.04 16:59 Maag01
 * t_SamplingCfg added
 * 
 * 1     15.11.04 15:53 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef SAMPLING_H
#define SAMPLING_H


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define	SAMPLING_SIGNAL_BUF_SIZE		8192


/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {

	BOOL enabled;	// enable flag	
	float *ptr;		// pointer to 1. sample
	long index;		// index of sampleBuffer

} t_Sampling;


// sampling configuration
typedef struct {

	long signalPath;
	long signalSource;

} t_SamplingCfg;


// parameters for block read
typedef struct {

	long nSamples;		// number of samples to read
	long startIndex;	// index of first sample to read

} t_SamplingBufRdParam;



/* PUBLIC FUNCTIONS ***********************************************************/

void sampling_init(t_Sampling *sampling);
void sampling_enable(float *sourcePtr, t_Sampling *sampling);
void sampling_activate(t_Sampling *sampling);
float *sampling_getBufferPtr(void);


#endif
