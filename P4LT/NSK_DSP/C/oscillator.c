/*-------------------------------------------------------------------------*
 * $Workfile: oscillator.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Oscillator functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/C/oscillator.c $ 
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 3     6.10.04 17:49 Maag01
 * 
 * 2     2.08.04 14:33 Leeb02
 * includes basetype.h instead of constant.h
 * 
 * 1     18.05.04 17:41 Maag01
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "speeddsp.h"
#include "basetype.h"

#include "oscillator.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		oscillator_init()	
  Description:	Initializes oscillator parameters. 
  			
  Inputs:		oscFreq: oscillator frequency
  				samplingFreq: sampling frequncy
  				*oscData: pointer to oscillator parameters
  Outputs:		oscData: initialized oscillator parameters
  Return:		None
  Globals:		None
  Important:	Has to be called befor using oscillator_osc() functions.

******************************************************************************/

#pragma optimize_off

void SEGMENT_EXT_CODE oscillator_init(float amplitude, float oscFreq, float samplingFreq, t_OscData *oscData)
{
	float dPhi;
	

	dPhi = oscFreq*TWO_PI/samplingFreq;
	
	oscData->phi = 0;
	oscData->cosPhi = amplitude;		// initial state real part
	oscData->sinPhi = 0.0;				// initial state imaginary part
	 
	oscData->deltaPhi = dPhi;
	oscData->cosDeltaPhi = cosf(dPhi);
	oscData->sinDeltaPhi = sinf(dPhi);
	
	oscData->amplitude = amplitude;
	oscData->ampCorrFactor = 1.0/(2.0*amplitude*amplitude);	// used for AGC
			
};

#pragma optimize_as_cmd_line


/*****************************************************************************
  Function:		oscillator_oscF()	
  Description:	Quadrature oscillator: generates cos and sin samples according 
  				to the initilized values.

  Inputs:		*oscData: pointer to oscillator parameters
  				blockLength: number of samples to be generated
  Outputs:		re: real part of the oscillation (cos)
  				im: imaginary part of the oscillation (sin)
  Return:		None
  Globals:		None
  Important:	oscillator_init() has to be called befor using oscillator_oscF() 
  				In order to avoid amplitude variation the automatic
  				gain control function oscillator_AGC() has to be called at 
  				regular intervals.

******************************************************************************/

void oscillator_oscF(float *re, float *im, t_OscData *oscData, long blockLength)
{
	register long i;
	register float cosPhiNew, cosPhi;
	register float sinPhiNew, sinPhi;
	register float cosDeltaPhi, sinDeltaPhi;
	
	
	cosDeltaPhi = oscData->cosDeltaPhi;
	sinDeltaPhi = oscData->sinDeltaPhi;
	
	cosPhi = oscData->cosPhi;
	sinPhi = oscData->sinPhi;
	
	for(i = 0; i < blockLength; i++) {	
	
		cosPhiNew = cosPhi*cosDeltaPhi - sinPhi*sinDeltaPhi;
		sinPhiNew = sinPhi*cosDeltaPhi + cosPhi*sinDeltaPhi;
		
		cosPhi= cosPhiNew;
		sinPhi= sinPhiNew;
		
		re[i] = cosPhiNew;
		im[i] = sinPhiNew;
	};
	
	oscData->cosPhi = cosPhi;
	oscData->sinPhi = sinPhi;
};


/*****************************************************************************
  Function:		oscillator_oscC()	
  Description:	Quadrature oscillator: generates cos and sin samples according 
  				to the initilized values.

  Inputs:		*oscData: pointer to oscillator parameters
  				blockLength: number of samples to be generated
  Outputs:		vOut: re: real part of the oscillation (cos)
  					  im: imaginary part of the oscillation (sin)
  Return:		None
  Globals:		None
  Important:	oscillator_init() has to be called befor using oscillator_oscC() 
  				In order to avoid amplitude variation the automatic
  				gain control function oscillator_AGC() has to be called at 
  				regular intervals.

******************************************************************************/

void oscillator_oscC(complex_float *vOut, t_OscData *oscData, long blockLength)
{
	register long i;
	register float cosPhiNew, cosPhi;
	register float sinPhiNew, sinPhi;
	register float cosDeltaPhi, sinDeltaPhi;
	
	
	cosDeltaPhi = oscData->cosDeltaPhi;
	sinDeltaPhi = oscData->sinDeltaPhi;
	
	cosPhi = oscData->cosPhi;
	sinPhi = oscData->sinPhi;
	
	for(i = 0; i < blockLength; i++) {	
	
		cosPhiNew = cosPhi*cosDeltaPhi - sinPhi*sinDeltaPhi;
		sinPhiNew = sinPhi*cosDeltaPhi + cosPhi*sinDeltaPhi;
		
		cosPhi= cosPhiNew;
		sinPhi= sinPhiNew;
		
		vOut[i].re = cosPhiNew;
		vOut[i].im = sinPhiNew;
	};
	
	oscData->cosPhi = cosPhi;
	oscData->sinPhi = sinPhi;
	
};


/*****************************************************************************
  Function:		oscillator_AGC()	
  Description:	Automatic gain control function for oscillator_osc() functions.
  Inputs:		*oscData: pointer to oscillator parameters
  Outputs:		oscData: adjusted oscillator states
  Return:		None
  Globals:		None
  Important:	If osillator_osc() functions are used the automatic gain
  				control function oscillator_AGC() has to be called at regular 
  				intervals.

******************************************************************************/

void oscillator_AGC(t_OscData *oscData)
{
	register float h, v;
	
	
	h = oscData->cosPhi*oscData->cosPhi + oscData->sinPhi*oscData->sinPhi;
	
	if (h > 0.0) {
		
		v = 1.5 - oscData->ampCorrFactor*h; 
		
		if (v > 0.0) {
			
			// adjust oscillator states
			oscData->cosPhi = v*oscData->cosPhi;
			oscData->sinPhi = v*oscData->sinPhi;	
		}
		else {
			
			// reset oscillator states to initial conditions
			oscData->cosPhi = oscData->amplitude;
			oscData->sinPhi = 0.0;
		};
	}
	else {
		
		// reset oscillator states to initial conditions
		oscData->cosPhi = oscData->amplitude;
		oscData->sinPhi = 0.0;
	};
};



/*void osc_f(float *re, float *im, t_OscData *oscData, long blockLength)
{
	long i;
	float cosPhiNew, cosPhi;
	float sinPhiNew, sinPhi;
	float cosDeltaPhi, sinDeltaPhi;
	
	
	cosDeltaPhi = oscData->cosDeltaPhi;
	sinDeltaPhi = oscData->sinDeltaPhi;
	
	cosPhi = cosq(oscData->phi);
	sinPhi = sinq(oscData->phi);
	
	re[0] = cosPhi;
	im[0] = sinPhi;
	
	for(i = 1; i < blockLength; i++) {	
	
		cosPhiNew = cosPhi*cosDeltaPhi - sinPhi*sinDeltaPhi;
		sinPhiNew = sinPhi*cosDeltaPhi + cosPhi*sinDeltaPhi;
		
		cosPhi= cosPhiNew;
		sinPhi= sinPhiNew;
		
		re[i] = cosPhiNew;
		im[i] = sinPhiNew;
	};
	
	oscData->phi = fmodf(oscData->phi + oscData->deltaPhi*blockLength, TWO_PI);
}; */


/*void osc_c(complex_float *vOut, t_OscData *oscData, long blockLength)
{
	long i;
	float cosPhiNew, cosPhi;
	float sinPhiNew, sinPhi;
	float cosDeltaPhi, sinDeltaPhi;
	
	
	cosDeltaPhi = oscData->cosDeltaPhi;
	sinDeltaPhi = oscData->sinDeltaPhi;
	
	cosPhi = cosq(oscData->phi);
	sinPhi = sinq(oscData->phi);
	
	vOut[0].re = cosPhi;
	vOut[0].im = sinPhi;
	
	for(i = 1; i < blockLength; i++) {	
	
		cosPhiNew = cosPhi*cosDeltaPhi - sinPhi*sinDeltaPhi;
		sinPhiNew = sinPhi*cosDeltaPhi + cosPhi*sinDeltaPhi;
		
		cosPhi= cosPhiNew;
		sinPhi= sinPhiNew;
		
		vOut[i].re = cosPhiNew;
		vOut[i].im = sinPhiNew;
	};
	
	oscData->phi = fmodf(oscData->phi + oscData->deltaPhi*blockLength, TWO_PI);
};*/

	
	
	
	
