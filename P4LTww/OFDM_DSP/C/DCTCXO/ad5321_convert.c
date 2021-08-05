//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      ad5321_convert.c
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        23-11-2004
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    AD5321 ppm to frequency control word converter
//             
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/DCTCXO/ad5321_convert.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 1     3.12.04 11:33 Chhamaa
 * 
 * 1     23.11.04 vpotapov
 * 
 */	
//---------------------------------------------------------------------------

#include "../../h/dctcxo/ad5321_defs.h"

int AD5321_Convert_ppm2code(float ppm)
//int  segment ( "seg_ext_pmco" ) AD5321_Convert_ppm2code(float ppm)
{
    int   i, code, max_code;
        float step, uppm, qbnd;
    
    if (ppm > AD5321_MAX_PPM ) ppm = AD5321_MAX_PPM;
    if (ppm < AD5321_MIN_PPM ) ppm = AD5321_MIN_PPM;

    //convert to unsigned float
	uppm = ppm - AD5321_MIN_PPM;
	if ( uppm < 0.0f ) uppm = 0.0f;

	//  Define symmetrical ppm quantizer stepsize	
	step = (AD5321_MAX_PPM - AD5321_MIN_PPM) / (float)(AD5321_CODE_RANGE - 1);
	
	//  Search quantizer boundaries
	qbnd = 0.5f * step;
	max_code = AD5321_CODE_RANGE - 1;
	for (code = 0; code < AD5321_CODE_RANGE - 1; code++ ) {
		if (uppm < qbnd) break;
		else qbnd += step;
	}

    return AD5321_MIN_CODE+code;
}

