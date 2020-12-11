/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Exp_Gen.c $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Comlex exponent generator for frequency offset tracking
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/Exp_Gen.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       15.02.2003 DLK
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "../h/OFDM_defs.h"
#include "../h/OFDM_externs.h"
#include "../h/util/speeddsp.h"

//========================================
float Exp_Gen(float start_ph, complex_float rotph,
              complex_float *foc, float stepphsymbol,
              int length)
	{
	int i;
	float start_ph_new;
	float rrotph, irotph, rfoc, ifoc, rrr, iii;
	
	rrotph = rotph.re;
	irotph = rotph.im;

	rfoc = cosq(start_ph);
	ifoc = sinq(start_ph);
	
	foc[0].re = rfoc;
	foc[0].im = ifoc;
	
	for(i = 1; i < length; i++)	
		{
		rrr = rfoc*rrotph - ifoc*irotph;
		iii = rfoc*irotph + ifoc*rrotph;
		foc[i].re = rrr;
		foc[i].im = iii;
		rfoc=rrr;
		ifoc=iii;
		}

	start_ph_new = start_ph + stepphsymbol;
	
	return start_ph_new; 
	}


