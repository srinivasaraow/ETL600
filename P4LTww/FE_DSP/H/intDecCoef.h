/*-------------------------------------------------------------------------*
 * $Workfile: intDecCoef.h $
 * Part of	: ETL600
 * Language	: C
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: Interpolation and decimation filter coefficients
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/FE_DSP/H/intDecCoef.h $
 * 
 * 3     19.08.05 16:54 Chhamaa
 * segment ("seg_pmData") replaced with SEGMENT_PM_DATA2
 * 
 * 2     16.02.05 18:17 Chhamaa
 * Comment added
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 3     20.10.04 10:00 Maag01
 * Int/dec filter coef changed and equalization filters added
 * 
 * 2     18.10.04 17:58 Maag01
 * 
 * 1     30.04.04 15:05 Maag01
 * Initial version
 * 
 *    
 * Initial Revision
 *-------------------------------------------------------------------------*/
 
#ifndef _INT_DEC_COEF
#define _INT_DEC_COEF
 
/*-----------------------GLOBAL CONSTANTS----------------------------------*/

// Matlab FDA-Tool used for FIR filter design (Signal Processing Toolbox V5.1)

// Interpolation and decimation low pass filter coefficients 
// First stage: M = 2

// Design method: equiripple
// Fs = 21.333 kHz
// Fpass = 4.0 kHz, Fstop = 6.266 kHz
// Apass = 0.1 dB, Astop = 82 dB

/*
#define N_FIR_M2_LP_COEF	32
float SEGMENT_PM_DATA2 pm firM2LpCoef[N_FIR_M2_LP_COEF] = {
	
	0.001045704703, 0.002624842571, 0.000585371803,-0.005686551798,-0.005735900253,
	0.00658750115,  0.01235013176,-0.007589161862, -0.02552072518, 0.002520335838,
	0.04424077272,  0.01119939797,  -0.0777810216, -0.05374199525,    0.172775358,
	0.4189261794,   0.4189261794,    0.172775358, -0.05374199525,  -0.0777810216,
	0.01119939797,  0.04424077272, 0.002520335838, -0.02552072518,-0.007589161862,
	0.01235013176,  0.00658750115,-0.005735900253,-0.005686551798, 0.000585371803,
	0.002624842571, 0.001045704703
};


// polyphase fir filters M = 2

#define N_FIR_M2_LP_POLY0_COEF 16

float SEGMENT_PM_DATA2 pm firM2LpPoly0Coef[N_FIR_M2_LP_POLY0_COEF] = {
	
	0.002624842571, -0.005686551798, 0.00658750115, -0.007589161862, 0.002520335838,
	0.01119939797, -0.05374199525, 0.4189261794, 0.172775358, -0.0777810216,
	0.04424077272, -0.02552072518, 0.01235013176, -0.005735900253, 0.000585371803,
    0.001045704703  
};

#define N_FIR_M2_LP_POLY1_COEF	16

float SEGMENT_PM_DATA2 pm firM2LpPoly1Coef[N_FIR_M2_LP_POLY1_COEF] = { 
	
	0.001045704703, 0.000585371803, -0.005735900253, 0.01235013176, -0.02552072518,
	0.04424077272, -0.0777810216, 0.172775358, 0.4189261794, -0.05374199525,
	0.01119939797, 0.002520335838, -0.007589161862, 0.00658750115, -0.005686551798,
	0.002624842571
}; */


// Optimized lowpass filter (non-flat passband)
// Design method: equiripple
// Fs = 21.333 kHz
// Fpass = 3.5 kHz, Fstop = 6.266kHz 
// Apass = 40.0 dB, Astop = 82.0 dB

#define N_FIR_M2_LP_COEF	12

float SEGMENT_PM_DATA2 pm firM2LpCoef[N_FIR_M2_LP_COEF] = {

	1.1212200e-003, 9.3688812e-003, 3.6386875e-002, 8.9742190e-002,
	1.5728785e-001, 2.0609299e-001, 2.0609299e-001, 1.5728785e-001,
	8.9742190e-002, 3.6386875e-002, 9.3688812e-003, 1.1212200e-003
};

// polyphase fir filters M = 2
#define N_FIR_M2_LP_POLY0_COEF	6

float SEGMENT_PM_DATA2 pm firM2LpPoly0Coef[N_FIR_M2_LP_POLY0_COEF] = {
	
	9.3688812e-003, 8.9742190e-002, 2.0609299e-001, 1.5728785e-001, 
	3.6386875e-002, 1.1212200e-003
};

#define N_FIR_M2_LP_POLY1_COEF	6

float SEGMENT_PM_DATA2 pm firM2LpPoly1Coef[N_FIR_M2_LP_POLY1_COEF] = { 
	
	1.1212200e-003, 3.6386875e-002, 1.5728785e-001, 2.0609299e-001,
	8.9742190e-002, 9.3688812e-003
};


// IIR equalizer for FIR low pass filter

#define EQ_M2_SEC		3	// number of sections
#define EQ_M2_STATE		6

static float pm eqIntDecM2Coef[2*EQ_M2_STATE + 1] = { 

	 5.7851828e-001, -2.7013563e-001, -3.1660086e-001, -2.0123114e-001,
	-3.6274097e-001, -1.1670254e+000,  1.2017509e-001, -3.3375236e-001,
	-6.6577424e-001,  1.4169816e+000,  6.7520992e-001, -1.4313170e+000,
	 4.7068471e+000
};

// Interpolation and decimation low pass filter coefficients
// Second Stage: M = 3

// Design method: equiripple
// Fs = 64.0 kHz
// Fpass = 4.0 kHz, Fstop = 16.933 kHz
// Apass = 0.1 dB, Astop = 85 dB

/*#define N_FIR_M3_LP_COEF	16

float SEGMENT_PM_DATA2 pm firM3LpCoef[N_FIR_M3_LP_COEF] = {
	
	-0.001862795092,-0.009187138639, -0.02116808482, -0.02382557467,  0.00760261761,
     0.08660310507,   0.1909796149,   0.2666514814,   0.2666514814,   0.1909796149,
     0.08660310507,  0.00760261761, -0.02382557467, -0.02116808482,-0.009187138639,
	-0.001862795092
};


// polyphase fir filters M = 3

#define N_FIR_M3_LP_POLY0_COEF	6

float SEGMENT_PM_DATA2 pm firM3LpPoly0Coef[N_FIR_M3_LP_POLY0_COEF] = {
		
	-0.001862795092, -0.02382557467, 0.1909796149, 0.1909796149, -0.02382557467,
	-0.001862795092
};


#define N_FIR_M3_LP_POLY1_COEF	6

float SEGMENT_PM_DATA2 pm firM3LpPoly1Coef[N_FIR_M3_LP_POLY1_COEF] = {

    0.0, -0.02116808482, 0.08660310507, 0.2666514814, 0.00760261761, -0.009187138639
};


#define N_FIR_M3_LP_POLY2_COEF	6

float SEGMENT_PM_DATA2 pm firM3LpPoly2Coef[N_FIR_M3_LP_POLY2_COEF] = {
	
	0.0, -0.009187138639, 0.00760261761, 0.2666514814, 0.08660310507, -0.02116808482
};*/


// Interpolation and decimation low pass filter coefficients
// Second Stage: M = 3

// Design method: equiripple
// Fs = 64.0 kHz
// Fpass = 8.0 kHz, Fstop = 12.933 kHz
// Apass = 0.1 dB, Astop = 85 dB

/*
#define N_FIR_M3_LP_COEF	46

float SEGMENT_PM_DATA2 pm firM3LpCoef[N_FIR_M3_LP_COEF] = {
	
	 1.9480820e-004,  3.0956470e-004, -1.5898261e-004, -1.6115641e-003, -3.4090578e-003,
	-3.6771712e-003, -7.3910172e-004,  4.4991520e-003,  7.7500152e-003,  4.2778734e-003,
	-5.7668606e-003, -1.4731925e-002, -1.2162617e-002,  4.8863150e-003,  2.4886847e-002,
	 2.7450130e-002,  9.9493251e-004, -4.0850719e-002, -6.0818641e-002, -2.2014963e-002,
	 8.0913744e-002,  2.0873479e-001,  2.9700249e-001,  2.9700249e-001,  2.0873479e-001,
	 8.0913744e-002, -2.2014963e-002, -6.0818641e-002, -4.0850719e-002,  9.9493251e-004,
	 2.7450130e-002,  2.4886847e-002,  4.8863150e-003, -1.2162617e-002, -1.4731925e-002,
	-5.7668606e-003,  4.2778734e-003,  7.7500152e-003,  4.4991520e-003, -7.3910172e-004,
	-3.6771712e-003, -3.4090578e-003, -1.6115641e-003, -1.5898261e-004,  3.0956470e-004,
	 1.9480820e-004
};


// polyphase fir filters M = 3

#define N_FIR_M3_LP_POLY0_COEF	16

float SEGMENT_PM_DATA2 pm firM3LpPoly0Coef[N_FIR_M3_LP_POLY0_COEF] = {
		
	1.9480820e-004, -1.6115641e-003, -7.3910172e-004,  4.2778734e-003, -1.2162617e-002,
	2.7450130e-002, -6.0818641e-002,  2.0873479e-001,  2.0873479e-001, -6.0818641e-002,
	2.7450130e-002, -1.2162617e-002,  4.2778734e-003, -7.3910172e-004, -1.6115641e-003,
	1.9480820e-004	
};


#define N_FIR_M3_LP_POLY1_COEF	16

float SEGMENT_PM_DATA2 pm firM3LpPoly1Coef[N_FIR_M3_LP_POLY1_COEF] = {

	0.0000000e+000, -1.5898261e-004, -3.6771712e-003,  7.7500152e-003, -1.4731925e-002,
	2.4886847e-002, -4.0850719e-002,  8.0913744e-002,  2.9700249e-001, -2.2014963e-002,
	9.9493251e-004,  4.8863150e-003, -5.7668606e-003,  4.4991520e-003, -3.4090578e-003,
	3.0956470e-004	   
};


#define N_FIR_M3_LP_POLY2_COEF	16

float SEGMENT_PM_DATA2 pm firM3LpPoly2Coef[N_FIR_M3_LP_POLY2_COEF] = {
	
	0.0000000e+000,  3.0956470e-004, -3.4090578e-003,  4.4991520e-003, -5.7668606e-003,
	4.8863150e-003,  9.9493251e-004, -2.2014963e-002,  2.9700249e-001,  8.0913744e-002,
	-4.0850719e-002,  2.4886847e-002, -1.4731925e-002,  7.7500152e-003, -3.6771712e-003,
	-1.5898261e-004	
}; */


// Optimized lowpass filter (non-flat passband)
// Design method: equiripple
// Fs = 64.0 kHz
// Fpass = 8.0 kHz, Fstop = 12.933 kHz
// Apass = 80.0 dB, Astop = 75.0 dB
 
#define N_FIR_M3_LP_COEF	18	

float SEGMENT_PM_DATA2 pm firM3LpCoef[N_FIR_M3_LP_COEF] = {
	
	-3.8720750e-005, 7.8502181e-004, 4.5801119e-003, 1.4732329e-002,
	 3.4373566e-002, 6.3936365e-002, 9.9184493e-002, 1.3149169e-001,
	 1.5095515e-001, 1.5095515e-001, 1.3149169e-001, 9.9184493e-002,  
	 6.3936365e-002, 3.4373566e-002, 1.4732329e-002, 4.5801119e-003,  
	 7.8502181e-004,-3.8720750e-005
};


// polyphase fir filters M = 3

#define N_FIR_M3_LP_POLY0_COEF	6	

float SEGMENT_PM_DATA2 pm firM3LpPoly0Coef[N_FIR_M3_LP_POLY0_COEF] = {
		
	4.5801119e-003, 6.3936365e-002, 1.5095515e-001, 9.9184493e-002,
	1.4732329e-002,-3.8720750e-005	 
};


#define N_FIR_M3_LP_POLY1_COEF	6	

float SEGMENT_PM_DATA2 pm firM3LpPoly1Coef[N_FIR_M3_LP_POLY1_COEF] = {

	7.8502181e-004, 3.4373566e-002, 1.3149169e-001, 1.3149169e-001,
	3.4373566e-002, 7.8502181e-004	
};


#define N_FIR_M3_LP_POLY2_COEF	6	

float SEGMENT_PM_DATA2 pm firM3LpPoly2Coef[N_FIR_M3_LP_POLY2_COEF] = {
	
	-3.8720750e-005, 1.4732329e-002, 9.9184493e-002, 1.5095515e-001,
	 6.3936365e-002, 4.5801119e-003	
};


// IIR equalizer for FIR low pass filter

#define EQ_M3_SEC		2	// number of sections
#define EQ_M3_STATE		4

static float pm eqIntDecM3Coef[2*EQ_M3_STATE + 1] = { 

	 1.5951610e-001,-2.5781708e-001,-3.0371103e-001, 4.1865742e-001,
	-6.5299129e-001,-1.6202815e+000, 6.2269354e-002,-1.9322425e-001,
	 3.7102850e+000
};



#endif
