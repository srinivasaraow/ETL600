/*-------------------------------------------------------------------------*
 * $Workfile: PMFilter.h $
 * Part of	: ETL600 / Front End DSP
 * Language	: C
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: coefficient declaration of pilot modem rx/tx filter 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/PMFilter.h $
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 2     23.11.04 17:28 Maag01
 * pilotFilterCoef8k removed
 * 
 * 1     30.04.04 15:11 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 *  
 *
 *-------------------------------------------------------------------------*/ 
 
#ifndef		_PMFILTER
#define		_PMFILTER 
 

// coefficients for iir filter fuction biiir() 
// tx/rx filter coefficients
// fs= 10666.67 Hz
  
#define NUM_OF_PF_SEC				  3             	// number of biquad sections
#define PMF_SARRAY_LENGTH       2*NUM_OF_PF_SEC	 		// pilot modem filter state array length
  
float pm pilotFilterCoef[4*NUM_OF_PF_SEC + 1]= {     	// filter s105220w

  -9.4832159e-001,  1.9463771e+000,  9.9999557e-001,  1.9999956e+000,
  -9.8738972e-001,  1.9824816e+000,  1.0000000e+000, -1.9923167e+000,
   0.0000000e+000,  9.6834211e-001,  0.0000000e+000,  1.0000044e+000,
   4.9155750e-006
};
 
  
#endif 
