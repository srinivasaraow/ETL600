/*-------------------------------------------------------------------------*
 * $Workfile: SignalGen.c $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by: 
 * Remarks	:  
 * Purpose	:  Signal Generators for Systemtests, Commissioning and SSB
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/C/SignalGen.c $
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 4     6.10.04 17:49 Maag01
 * 
 * 3     2.08.04 14:35 Leeb02
 * N_2 removed
 * 
 * 2     7.05.04 15:08 Maag01
 * 
 * 1     30.04.04 14:56 Maag01
 * Initial version based on ETL500 Version 4.20 (signal_p.c)
 * 
 *-------------------------------------------------------------------------*/
/*-----------------------GLOBAL CONSTANTS----------------------------------*/

#include <math.h>
#include "basetype.h"  
#include "signalGen.h"

#ifndef CODECHECK
asm("#include <def21161.h>");
asm("#include <asm_sprt.h>");
#endif


/*-----------------------LOCAL CONSTANTS-----------------------------------*/

#define MAGIC_NUMBER_AMPL_KORR 			1.5
#define INITIAL_VAL_NOISE_GEN         	0x7fff  
#define MASK_13_LSB						0x1fff	
#define SIGN_BIT						0x2000
#define NR_OF_SAMPLE_NOISE_GEN			2048

#define N_13							13
#define N_14							14
#define N_15							15       


/*-----------------------IMPORTED VARIABLES--------------------------------*/

/*-----------------------STATIC VARIABLES----------------------------------*/

/*-----------------------GLOBAL FUNCTION DEFINITIONS-----------------------*/
 
/*-------------------------------------------------------------------------*/
/* void InitGenIQ(OscIQType osc,float f_transmit, float f_sample, float amplitude); */
/* INIT SIN GENGERATOR */  
/* Initialisiert  void GenIQ(OscIQType dummy, float *sinusPtr, float *cosinusPtr ) */
/*-------------------------------------------------------------------------*/

#pragma optimize_off 

void SEGMENT_EXT_CODE InitGenIQ(OscIQPtr osc,float f_transmit, float f_sample, float amplitude)
{
  /*
 * Calcualate new delta values (sin b, cos b)
 *
 * Additionstheoreme:
 *
 * sin (a+b) = sin a * cos b + cos a * sin b
 * cos (a+b) = cos a * cos b - sin a * sin b
 *
 * sampling frequence:           f_sample
 * frequence to be transmitted:  f_transmit
 *
 * f_transmit == 2*PI
 * b = 2*PI / ( f_sample / f_transmit)
 * sin_b   = sin (b)  !!! rad
 * cos_b   = cos (b)  !!! rad
 */

  float b;

	b = TWO_PI * f_transmit / f_sample ;
	osc->tps_tx_sin = 0.0;
	osc->tps_tx_cos = amplitude;
	osc->sin_b = sinf(b);
	osc->cos_b = cosf(b);
	osc->amplitude = amplitude;
	osc->f_sample = f_sample;
	osc->f_transmit = f_transmit;
}

#pragma optimize_as_cmd_line 

/*-------------------------------------------------------------------------*/
/* void GenIQ(OscIQPtr , float *, float * ) */
/*  SIN COS GENGERATOR  */
/*-------------------------------------------------------------------------*/

#pragma optimize_off 

void GenIQ(OscIQPtr osc, float *sinusPtr, float *cosinusPtr )
{
/*
 * Calcualate new delta values (sin b, cos b)
 *
 * Additionstheoreme:
 *
 * sin (a+b) = sin a * cos b + cos a * sin b
 * cos (a+b) = cos a * cos b - sin a * sin b
 *
 *
*/
	asm("puts = r12;");               		/* cosinusPtr*/
	asm("puts = r8;");               		/* sinusPtr*/
	asm("puts = r4;");               		/* oscPtr */
    asm("     bit clr mode1 IRPTEN;		");
	asm("     bit set mode1 SRRFH|SRRFL;"); /* use secondary registers */
	asm("     nop;						");
	asm("     r2 = gets(1);				");	/*  use oscPtr in secondary registers */
	asm("     i4 = r2;");
	asm("     f1 = dm(0,i4);			");	/*  tps_tx_sin ,sin a    */
	asm("     f3 = dm(1,i4);			"); /*  tps_tx_cos, cos a   */
	asm("     f4 = dm(2,i4);			"); /*  sin_b          */
	asm("     f7 = dm(3,i4);			"); /*  cos_b          */
	asm("     f11= f1*f7;             	"); /* sin a * cos b */
	asm("     f15= f3*f4;           	"); /* cos a * sin b */
	asm("     f6 = f3*f7,f9 = f11+f15; 	"); /* cos a * cos b, ->sin(a+b)  */
	asm("     f11= f1*f4,f1 = f9;		");
	asm("     f3 = f6-f11;				");
	asm("     r2 = gets(2);				"); /* get sinusPtr */
	asm("     i4 = r2;      			");
	asm("     dm(i4,0) = f1;   			");	/* save sin(a+b) to sinusPtr */
	asm("     r2 = gets(3);				");	/* get cosinusPtr */
	asm("     i4 = r2;      			");                 
	asm("     dm(i4,0) = f3;         	");	/* save cos(a+b) to cosinusPtr */
	asm("     r2 = gets(1);        		");	/* get oscPtr */
	asm("     i4 = r2;             		");
	asm("     dm(0,i4) = f1;			"); /* new tps_tx_sin     */
	asm("     dm(1,i4) = f3;			"); /* new tps_tx_cos;    */
	asm("	  bit clr mode1 SRRFH|SRRFL;"); /* use primary registers */
    asm("     bit set mode1 IRPTEN;		");
    asm("     nop;");
}

#pragma optimize_as_cmd_line


void GenIQAmpCorr(OscIQPtr osc)
{
	float h;
	static float amplitude_old = 0, korr_faktor;

	if ( amplitude_old != osc->amplitude)
	 {
	    korr_faktor = 1/(2 * (osc->amplitude) * (osc->amplitude)); 
		amplitude_old = osc->amplitude;
	 }
	
	if (0 == (h = (osc->tps_tx_sin) * (osc->tps_tx_sin) + (osc->tps_tx_cos) * (osc->tps_tx_cos)))
	 {
		osc->tps_tx_sin = 0;
		osc->tps_tx_cos = osc->amplitude;
	 }
	else
	 {                   
		h = MAGIC_NUMBER_AMPL_KORR - korr_faktor * h;
		if (0. > h) h = 0.;
		osc->tps_tx_sin = h * osc->tps_tx_sin;
		osc->tps_tx_cos = h * osc->tps_tx_cos;
	 }
}


/*-------------------------------------------------------------------------*/
/* void noise_gen(long NumberOfNewValues, long *buffer_ptr) */
/*  NOISE GENERATOR */
/*-------------------------------------------------------------------------*/

/*long 	noise = INITIAL_VAL_NOISE_GEN;
long	NoiseCounter;
void noise_gen_float(float *buffer_ptr)
{
	long a,b,c;
    long v;

	if(NoiseCounter == 0) noise = INITIAL_VAL_NOISE_GEN; // only used once after noise gen. start
 	for(v = 0;v<N_15;v++)
 	{
	 	a = noise & (1L<<N_13);             // FIGURE 1/O.151 15 Bit
	 	b = (noise & (1L<<N_14))>>1;
	 	c = ((a^b)>>N_13) & 1;
		noise = (noise<<1) | c;          
	}
	if(noise & INITIAL_VAL_NOISE_GEN == 0) noise = INITIAL_VAL_NOISE_GEN; //if all bits zero: set all bits to 1
	if(noise & SIGN_BIT) *buffer_ptr = -(float)(noise & MASK_13_LSB);
	else 			   *buffer_ptr =  (float)(noise & MASK_13_LSB);
    NoiseCounter++;
	if(NoiseCounter >= NR_OF_SAMPLE_NOISE_GEN) NoiseCounter = 0; 
} */




