/*-------------------------------------------------------------------------*
 * $Workfile: AES_LP_FilterCoef.h $
 * Part of      : ETL600 / NSD DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Coefficient declaration of different LP filters
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/H/AES_LP_FilterCoef.h $
 * 
 * 2     25.08.06 15:01 Chhamaa
 * Sine table moved to AES_SineTable.h
 * 
 * 1     15.08.06 9:29 Chhamaa
 * Initial version based on AES550
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef AES_LP_FILTER_COEF_H
#define AES_LP_FILTER_COEF_H


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
 
/* coefficients for iir filter fuction biiir() 
filter coefficients
fs= 10666 */

#define NUM_OF_SEC_LP2k		2					// number of biquad sections            		
#define LPF_SARRAY_LENGTH	2*NUM_OF_SEC_LP2k	// 2kHz LP-filter state array length	 	 	
 	
//
// 2.0kHz TP-Filter
// 
float pm LP2k0HzCoef[4*NUM_OF_SEC_LP2k + 1] = {     	// filter elip_fi.m 2.0kHz

	-2.4437986e-001,  7.1880016e-001,  1.0000000e+000,  1.5741499e+000,
	-7.2946701e-001,  5.3819522e-001,  1.0000000e+000,  4.8362918e-001,
	 6.8927087e-002
};

//
// 2.0kHz TP-Filter 8kHz Variante
// 
float pm LP2kHz8Coef[4*NUM_OF_SEC_LP2k + 1] = {     	// filter elip_fi.m 2.0kHz

	-5.0240516e-001,  1.3452778e-000,  1.0000000e+000,  5.3469502e-001,
	-8.2192721e-001,  1.4659444e-000,  1.0000000e+000,  -9.9049917e-001,
	 2.1362318e-002
};

//
// 2.133kHz LP-Filter
//
float pm LP2k1HzCoef[4*NUM_OF_SEC_LP2k + 1] = {     	// filter LP2_1k55.m  2.133kHz

	-2.2270404e-001,  6.3494203e-001,  1.0000000e+000,  1.6336724e+000,
	-7.2409037e-001,  4.0328900e-001,  1.0000000e+000,  6.3746991e-001,
	 7.9159992e-002
};

//
// 2.2kHz LP-Filter for AES550
//
float pm LP2k2HzCoef[4*NUM_OF_SEC_LP2k + 1] = {     	// filter LP2k2Hz.m  2.2kHz 

	-2.1280216e-001,  5.9248640e-001,  1.0000000e+000,  1.6602160e+000,
	-7.2198122e-001,  3.3503821e-001,  1.0000000e+000,  7.1058248e-001,
	 8.4742660e-002
};

//
// 2.34kHz LP-Filter 
//
float pm LP2k3HzCoef[4*NUM_OF_SEC_LP2k + 1] = {     	// filter LP2_3k55.m  2.346kHz

	-1.9356779e-001,  4.9936144e-001,  1.0000000e+000,  1.7113050e+000,
	-7.1875771e-001,  1.8605859e-001,  1.0000000e+000,  8.5998399e-001,
	 9.7961459e-002
};

//
// 2.56kHz LP-Filter
//
float pm LP2k5HzCoef[4*NUM_OF_SEC_LP2k + 1] = {     	// filter LP2_5k55.m  2.560kHz

	-1.7118760e-001,  3.6097384e-001,  1.0000000e+000,  1.7724431e+000,
	-7.1737162e-001, -3.1728665e-002,  1.0000000e+000,  1.0555293e+000,
	 1.2014485e-001
};



#endif
