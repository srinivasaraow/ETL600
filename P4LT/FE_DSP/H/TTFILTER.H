/*-------------------------------------------------------------------------*
 * $Workfile: TTFILTER.H $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: coefficient declaration for testtone rx filter 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/TTFILTER.H $
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 1     30.04.04 15:23 Maag01
 * Initial version based on ETL500 Version 4.20
 *  
 *  
 *
 *-------------------------------------------------------------------------*/ 
#ifndef		_TTFILTER
#define		_TTFILTER

/* test tone filter 
   coefficients for iir filter fuction biiir()
   fs= 10666, butter(2,20/5333), fg= 20 Hz*/
     
  #define NUM_OF_TTF_SEC		 1    					// number of biquad sections
  #define TTF_SARRAY_LENGTH      2*NUM_OF_TTF_SEC	 	// test tone rx filter state array length
                                      
  float pm testToneCoef[4*NUM_OF_TTF_SEC + 1]= {
  
  -9.8347619e-001,  1.9833385e+000,  1.0000000e+000,  2.0000000e+000,
   3.4415061e-005
   };
   
#endif 
 
