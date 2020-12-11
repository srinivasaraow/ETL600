/*-------------------------------------------------------------------------*
 * $Workfile: EQ_CALCU.C $
 * Part of      : LCPLC
 * Language     : C
 * Created by   : Bruno Busslinger NTC-1
 * Remarks      :  
 * Purpose      : Equalizer Calculation of sampled noise sequence
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/EQ_CALCU.C $ 
 * 
 * 1     16.02.05 13:42 Chhamaa
 * 
 * 6     2.02.05 9:36 Chhesch5
 * Defines replaced
 * 
 * 5     1.02.05 18:18 Chhamaa
 * Defines replaced
 * 
 * 4     27.01.05 13:46 Chhamaa
 * SEGMENT_EXT_CODE added for function running on P4LT
 * 
 * 3     24.01.05 17:02 Chhamaa
 * #include "eq_coeff.h" removed
 * 
 * 2     11.03.04 17:23 Nohl01
 * 
 * 1     10.03.04 12:11 Nohl01
 * Initial version
 * Based on MMI500 v. 3.20
 *-------------------------------------------------------------------------*/
/*-----------------------GLOBAL CONSTANTS----------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#ifndef VISUALC

#include <trans.h>
extern void PN_GeneratorSample(long *ShiftRegisterGenerator, float *BlockSampleValueArrPtr);

#endif

#include "basetype.h"
#include "equalizerTypes.h"
#include "constant.h"
#include "eq_calcu.h"

/*-----------------------TYPE AND STRUCTURE DEFINITIONS--------------------*/

#ifdef VISUALC


#define SWAP(a,b) tempr=(double)(a);(a)=(b);(b)=(float)tempr 
#define sqrtf(x) ((float) sqrt((double) (x)))
#define fabsf(x) ((float) fabs((double) (x)))
#define log10f(x) ((float) log10((double) (x)))

   float *cfft256(float real_in[], float imag_in[], float real_out[], float imag_out[])
	{                       
	//#define SWAP(a,b) tempr=(double)(a);(a)=(b);(b)=(float)tempr 	
	//#define PI  3.141592654
	    
		long NumberOfSamples = 256;
	 	float data[514];
	
	 	long i,ii,iii;
	    long nn,isign;      
	    long cntr; 
	    long n,mmax,m,j,istep;
	    double wtemp, wr, wpr,wpi,wi,theta;
	    double tempr, tempi;
	    float test;
	    
	    isign=1;         
	    nn= NumberOfSamples;
	    cntr=0;
	    
		//****************************************************************************
		// fill in the data as follows
		//****************************************************************************
		//  data[0] = real
		//  data[1] = imag.
		//  data[2] = real
		//  data[3] = imag.
		//    .......
		//  data[number_of_samples-2] = real
		//  data[number_of_samples-1] = imag.
		//****************************************************************************
		for(ii=0;ii<256L;ii++)
	    {
	    	data[(2*ii)+1] = real_in[ii];
	    	data[(2*ii)+2] = imag_in[ii];
	    	
	    	for( iii=0;iii<512;iii++)
	    	{ 
	    		test = data[iii];
	    	};
	    	
	    };
	    
	
		//****************************************************************************
		//***** fft() ****************************************************************
		// decrement data pointer to use the FFT function with array range [0...2*nn-1]
		//****************************************************************************
	
	    n=nn << 1;
	    j=1;
	    for(i=1;i<n;i+=2)
	    {
	    	if(j>i)
	     	{
	      		SWAP(data[j],data[i]);
	      		SWAP(data[j+1],data[i+1]);
	     	}
			m=n >>1;
			while(m>=2 && j > m)
			{
		 		j-=m;
		 		m >>=1;
			}
			j +=m;
		 }	 
		mmax=2;
		while(n > mmax)
		{
		 istep=2*mmax;
		 theta=6.28318530717959/(isign*mmax);
		 wtemp=sin(0.5*theta);
		 wpr = -2.0*wtemp*wtemp;
		 wpi=sin(theta);
		 wr=1.0;
		 wi=0.0;
		 for(m=1;m<mmax;m+=2)
		 {
		   for(i=m;i<=n;i+=istep)
		   {
		    j=i+mmax;
		    tempr=wr*data[j]-wi*data[j+1];
		    tempi=wr*data[j+1]+wi*data[j];
		    data[j]=data[i]-(float)tempr;
		    data[j+1]=data[i+1]-(float)tempi;
		    data[i] += (float)tempr;
		    data[i+1] += (float)tempi;
		   }
		   wr=(wtemp=wr)*wpr-wi*wpi+wr;
		   wi=wi*wpr+wtemp*wpi+wi;
		 }
		 mmax=istep;
		}
		//****************************************************************************
		// increment data pointer to initial value [0...2*nn-1]
		//  fft_data[0] = real
		//  fft_data[1] = imag.
		//  fft_data[2] = real
		//  fft_data[3] = imag.
		//    .......
		//  fft_data[number_of_samples-2] = real
		//  fft_data[number_of_samples-1] = imag.
		
		for(i=0;i<256;i++)
	    {
	    	real_out[i] =data[(2*i)+1];
	    	imag_out[i] = data[(2*i)+2];
	    };
	   
		return(real_out);                       /// used to be compatible with the shrac C-lib
	}


// NOTE: This PN Generator is only usen in MMI. On DSPB there is pn_gener.asm
//		 which is working exactly the same.
                                                                             
/*-------------------------------------------------------------------------*/
/*static void PN_GeneratorSample(long *ShiftRegisterGenerator, 			   */
/*									float *BlockSampleValueArrPtr)		   */
/*                                                                         */
/* *ShiftRegisterGenerator: Pointer to shift register of the PN generator  */
/* *BlockSampleValueArrPtr: pointer to the array of 2 float samples 	   */
/* 							representing 2 samples of pseudo noise		   */
/* Returns two float Sample in BlockSampleValueArr of the Pseudo Random    */
/* Noise Generator of length 256. The shift register is 				   */
/* ShiftRegisterGenerator. The output is NOISELEVEL or -NOISELEVEL         */
/*-------------------------------------------------------------------------*/

static void PN_GeneratorSample(long *ShiftRegisterGenerator, float *BlockSampleValueArrPtr)
{

	unsigned long buffer;

 
	BlockSampleValueArrPtr[0] = BlockSampleValueArrPtr[1] = NOISELEVEL;

	if ( (*ShiftRegisterGenerator & BITMASK_FIRST_POSITION) == 0 )
	{
		BlockSampleValueArrPtr[0] = -NOISELEVEL;
	}
	if ( (*ShiftRegisterGenerator & BITMASK_SECOND_POSITION) == 0 )
	{
		BlockSampleValueArrPtr[1] = -NOISELEVEL;
	}

	buffer = (((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_7)) ^ 
 			((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_3))) ^  
			(((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_2)) ^ 
			((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_1)));

	*ShiftRegisterGenerator = *ShiftRegisterGenerator >> SHIFT_BIT_1;
	*ShiftRegisterGenerator = (*ShiftRegisterGenerator & BITMASK_LAST_POSITION) | buffer;
	if (*ShiftRegisterGenerator == FIRST_STATE_SHIFT_REGISTER)	
	{
		// Insert one value more (Twice start value for 256 Samples)
		*ShiftRegisterGenerator = START_VALUE_SHIFT_REGISTER;
	}
	else
	{
		buffer = (((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_7)) ^ 
 			((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_3))) ^  
			(((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_2)) ^ 
			((BIT_MASK_SHIFT_REGISTER)&(*ShiftRegisterGenerator << SHIFT_BIT_1)));
		*ShiftRegisterGenerator = *ShiftRegisterGenerator >> SHIFT_BIT_1;
		*ShiftRegisterGenerator = (*ShiftRegisterGenerator & BITMASK_LAST_POSITION) | buffer;
	}
}


#endif


/*-------------------------------------------------------------------------*/
/*void SetArrayZero(float *ArrayPtr, long ArrayLength)					   */
/*                                                                         */
/* *ArrayPtr: Pointer to the first element of the array to set zero (float)*/
/* ArrayLength: length of this array									   */
/*																		   */
/* Sets all content of the array length 0				                   */
/*-------------------------------------------------------------------------*/

void SetFloatArrayZero(float *ArrayPtr, long ArrayLength)
	{
	long index;
	
	index = 0;
	while (index < ArrayLength)
		{
		*((float *) (ArrayPtr + index)) = FLOAT_ZERO_F;
		index++;
		}
	}


/*--------------------------------------------------------------------------*/
/* void CalculateTransferFunctionEqualizer(float *MeasureSamples,			*/
/*	float Pilotfrequenz, float *RealPartTransferFunctionEqPtr, 				*/
/*  float *ImagPartTransferFunctionEqPtr)  									*/	
/* 																			*/
/* Calculates the transfer function of an Equalizer with the measurement	*/
/* samples:																	*/
/*																			*/
/* Input:	*MeasureSamples:	Pointer to array of 256 Channel				*/
/*								measure samples (float)						*/
/*			Pilotfrequenz:		Actual Pilot frequency in [Hz] as float		*/
/*																			*/
/* Output: *RealPartTransferFunctionEqPtr:	Pointer to an array of 129 		*/
/*											samples representing the real	*/
/*											part of the equalizer transfer	*/
/*											function from 0..fs/2			*/ 
/*		   *ImagPartTransferFunctionEqPtr:	Pointer to an array of 129 		*/
/*											samples representing the 		*/
/*											imaginary part of the equalizer */
/*											transfer function from 0..fs/2	*/
/*--------------------------------------------------------------------------*/


void SEGMENT_EXT_CODE CalculateTransferFunctionEqualizer(float *MeasureSamples, 
														 float Pilotfrequenz,
														 float *RealPartTransferFunctionEqPtr, 
														 float *ImagPartTransferFunctionEqPtr)
	{
	
	static long PN_GeneratorRegister = START_VALUE_SHIFT_REGISTER;
	long LowerPilotChannelFreqPoint;
	long UpperPilotChannelFreqPoint;
	long index;
	float nenner;
	float scaling_factor;
	float Frequenzbreite;
	float PN_BlockSamples[2];

	
	// set array memory dynamically

	float *RealNoiseSpectrumArrPtr;
	float *ImagNoiseSpectrumArrPtr;
	float *RealBackupArrPtr;
	float *ImagBackupArrPtr;
	RealBackupArrPtr = (float *) calloc(PN_ACCUMULATOR_LENGTH, sizeof(float));
	ImagBackupArrPtr = (float *) calloc(PN_ACCUMULATOR_LENGTH, sizeof(float));
	RealNoiseSpectrumArrPtr = (float*) calloc(((PN_ACCUMULATOR_LENGTH/ZWEI) + 1) ,sizeof(float));
	ImagNoiseSpectrumArrPtr = (float*) calloc(((PN_ACCUMULATOR_LENGTH/ZWEI) + 1) ,sizeof(float));
    
    // Main calculation
    
	for (index = 0;index < PN_ACCUMULATOR_LENGTH; index++)
		{
		// Calculate Noisesequenz, Noiselevel anpassen
		//RealBackupArrPtr[index] = PN_GeneratorSample();
		PN_GeneratorSample(&PN_GeneratorRegister, &PN_BlockSamples[0]);
		RealBackupArrPtr[index] = PN_BlockSamples[0];
		index++;										// DIRTY
		RealBackupArrPtr[index] = PN_BlockSamples[1];
	    }
	// Calculate Spectrum of Noise Sequence    
	cfft256(RealBackupArrPtr,ImagBackupArrPtr, RealBackupArrPtr, ImagBackupArrPtr);

	for (index = 0; index < (PN_ACCUMULATOR_LENGTH/ZWEI) + 1; index++)
		{
		RealNoiseSpectrumArrPtr[index] = ((float)MAX_NUM_OF_ACCUMULATIONS * RealBackupArrPtr[index]);
		ImagNoiseSpectrumArrPtr[index] = ((float)MAX_NUM_OF_ACCUMULATIONS * ImagBackupArrPtr[index]);
		ImagBackupArrPtr[index] =  FLOAT_ZERO_F;
		ImagBackupArrPtr[index + PN_ACCUMULATOR_LENGTH/ZWEI - 1] =  FLOAT_ZERO_F;
		}
	// Calculate Spectrum
	cfft256(MeasureSamples,ImagBackupArrPtr, RealBackupArrPtr, ImagBackupArrPtr);

    for (index = 0; index < (PN_ACCUMULATOR_LENGTH/ZWEI) + 1; index++)
    	{
    	nenner = (RealBackupArrPtr[index]*RealBackupArrPtr[index]) + 
    			 (ImagBackupArrPtr[index]*ImagBackupArrPtr[index]);
    	RealPartTransferFunctionEqPtr[index] = (RealNoiseSpectrumArrPtr[index] * RealBackupArrPtr[index] +
    											ImagNoiseSpectrumArrPtr[index] * ImagBackupArrPtr[index] )/ nenner;
    	ImagPartTransferFunctionEqPtr[index] = (ImagNoiseSpectrumArrPtr[index] * RealBackupArrPtr[index] -
    											RealNoiseSpectrumArrPtr[index] * ImagBackupArrPtr[index] )/ nenner;
    	}
    Frequenzbreite = (((float) PN_ACCUMULATOR_LENGTH) / ((float) AF_SAMPLE_FREQ)); 	// 1/(Frequenzauflösung FFT in [Hz])
    
    // LowerPilotChannelFreqPoint enthält Position der unteren Pilotkanalgrenze im EqualizerTransferFunctionArr
    LowerPilotChannelFreqPoint = (long) ((Pilotfrequenz - PILOT_CHANNEL_BANDWIDTH*FLOAT_ZERO_POINT_FIVE) * Frequenzbreite);
    // UpperPilotChannelFreqPoint enthält Position der oberen Pilotkanalgrenze im EqualizerTransferFunctionArr
    UpperPilotChannelFreqPoint = (long) (FLOAT_ONE + (Pilotfrequenz + PILOT_CHANNEL_BANDWIDTH*FLOAT_ZERO_POINT_FIVE) * 
    																									   Frequenzbreite);

	scaling_factor = FLOAT_TWO / ( sqrtf((RealPartTransferFunctionEqPtr[LowerPilotChannelFreqPoint] * 
										  RealPartTransferFunctionEqPtr[LowerPilotChannelFreqPoint]) + 
							     		(ImagPartTransferFunctionEqPtr[LowerPilotChannelFreqPoint] * 
							      		  ImagPartTransferFunctionEqPtr[LowerPilotChannelFreqPoint]) ) +
						 		   sqrtf((RealPartTransferFunctionEqPtr[UpperPilotChannelFreqPoint] * 
						 	  	 		  RealPartTransferFunctionEqPtr[UpperPilotChannelFreqPoint]) +
						    			(ImagPartTransferFunctionEqPtr[UpperPilotChannelFreqPoint] * 
						    			 ImagPartTransferFunctionEqPtr[UpperPilotChannelFreqPoint]) ) );								    

	// Scaling 

	for (index = 0;index < (PN_ACCUMULATOR_LENGTH/ZWEI) + 1; index++)
		{
		RealPartTransferFunctionEqPtr[index] = scaling_factor * RealPartTransferFunctionEqPtr[index];
		ImagPartTransferFunctionEqPtr[index] = scaling_factor * ImagPartTransferFunctionEqPtr[index];
		}

	// Deallocate dynamically allocated Memory
	
	free(ImagNoiseSpectrumArrPtr);
	free(RealNoiseSpectrumArrPtr);
	free(ImagBackupArrPtr);
	free(RealBackupArrPtr);
	}
	

/*--------------------------------------------------------------------------*/
/*void InterpolatePieceTransferFunction(long LowBoundary, long HighBoundary,*/
/*                                       float RealLowBoundaryValue,		*/ 
/*                                       float ImagLowBoundaryValue,		*/
/*										 float RealHighBoundaryValue,		*/
/*										 float ImagHighBoundaryValue,		*/
/*                                       float *RealTFEqPtr,				*/
/*										 float *ImagTFEqPtr,				*/
/*                                       float *AbsTFEqPtr)					*/
/*																			*/
/* Sets all samples between LowBoundary and HighBoundary in the arrays		*/
/* RealTFEqPtr and ImagTFEqPtr zero and interpolates these missing samples	*/
/* in AbsTFEqPtr as absolute values at the same array position.				*/
/*																			*/
/* Input:	LowBoundary:	Position in Array of the lower last valid sample*/
/*			HighBoundary:	Position in Array of the high first valid sample*/
/*			RealLowBoundaryValue:	Real part of value @ LowBoundary		*/
/*			ImagLowBoundaryValue:	Imaginary part of value @ LowBoundary	*/
/*			RealHighBoundaryValue:	Real part of value @ HighBoundary		*/
/*			ImagHighBoundaryValue:	Imaginary part of value @ HighBoundary	*/
/*																			*/
/* Input /																	*/
/* Output:	*RealTFEqPtr:	Pointer to an array of 129 samples representing */
/*							the real part of the equalizer transfer			*/
/*							function from 0..fs/2 with non valid samples	*/ 
/*		  	*ImagTFEqPtr:	Pointer to an array of 129 samples representing */
/*							the imaginary part of the equalizer transfer	*/
/*							function from 0..fs/2 with non valid samples	*/
/*			*AbsTFEqPtr:	Pointer to an array of 129 samples representing */
/*							the absolute value of the equalizer transfer	*/
/*							function from 0..fs/2 containing only the 		*/	
/*			                interpolated samples @ non valid parts			*/
/*--------------------------------------------------------------------------*/

static void SEGMENT_EXT_CODE InterpolatePieceTransferFunction(long InterpolationPhase, 
								long LowBoundary, long HighBoundary,
								float RealLowBoundaryValue, float ImagLowBoundaryValue, 
								float RealHighBoundaryValue, float ImagHighBoundaryValue,
								float *RealTFEqPtr, float *ImagTFEqPtr, float *AbsTFEqPtr)
{	
	long index;
	float absLowBoundaryValue;
	float absHighBoundaryValue;
	float delta;
	float correcture;
	
	absLowBoundaryValue = sqrtf( (RealLowBoundaryValue * RealLowBoundaryValue) + 
								 (ImagLowBoundaryValue * ImagLowBoundaryValue) );
	absHighBoundaryValue = sqrtf( (RealHighBoundaryValue * RealHighBoundaryValue) + 
								  (ImagHighBoundaryValue * ImagHighBoundaryValue) );
	delta = (absHighBoundaryValue - absLowBoundaryValue) / (HighBoundary - LowBoundary);

	for (index = (LowBoundary + 1); index < HighBoundary; index++)
	{
		AbsTFEqPtr[index] = absLowBoundaryValue + delta *(index - LowBoundary);
		if (InterpolationPhase)
		{
			correcture = AbsTFEqPtr[index] / (sqrtf((RealTFEqPtr[index] * RealTFEqPtr[index]) + (ImagTFEqPtr[index] * ImagTFEqPtr[index])));
			RealTFEqPtr[index] = RealTFEqPtr[index] * correcture;
			ImagTFEqPtr[index] = ImagTFEqPtr[index] * correcture;
			AbsTFEqPtr[index] = FLOAT_ZERO_F;
		}
		else
		{
			RealTFEqPtr[index] = FLOAT_ZERO_F;
			ImagTFEqPtr[index] = FLOAT_ZERO_F;
		}
	}
}


/*--------------------------------------------------------------------------*/
/* void InterpolationEqualizer(float *RealPartTransferFunctionEqPtr,		*/
/*			                   float *ImagPartTransferFunctionEqPtr,		*/
/*                             float *AbsTransferFunctionEqInterpolatedPtr,	*/
/*							   float PilotFrequenz)							*/
/*																			*/
/* Sets all invalid samples from the equalizer transfer function 0.			*/
/* Interpolates the invalid samples from the known ones and stores			*/ 
/* them in AbsTransferFunctionEqInterpolatedPtr (only absolute values).		*/
/* Following parts of the equalizer transfer function are invalid:			*/
/* Pilot channel (Bandwidth 240 Hz), LowFrequency, High End, boundaries		*/
/* defined in entzerr.h														*/
/*																			*/ 
/* Input/																	*/
/* Output:	*RealPartTransferFunctionEqPtr: Pointer to an array of 129      */
/*											samples representing the real   */
/*											part of the equalizer transfer	*/
/*											function from 0..fs/2 with non	*/
/*											valid samples					*/
/*			*ImagPartTransferFunctionEqPtr:	Pointer to an array of 129      */
/*											samples representing the imag   */
/*											part of the equalizer transfer	*/
/*											function from 0..fs/2 with non	*/
/*											valid samples					*/
/*			PilotFrequenz: Actual pilot frequency in Hz as float			*/
/*																			*/
/* Output:	*AbsTransferFunctionEqInterpolatedPtr:	Pointer to an array of  */
/*													129 samples representing*/
/*													the absolute value of 	*/
/*													the equalizer transfer	*/
/*													function from 0..fs/2 	*/
/*													containing only the 	*/
/*			               							interpolated samples @ 	*/
/*													non valid parts			*/
/*--------------------------------------------------------------------------*/
 
 
void SEGMENT_EXT_CODE InterpolationEqualizer(float *RealPartTransferFunctionEqPtr, 
											 float *ImagPartTransferFunctionEqPtr,
											 float *AbsTransferFunctionEqInterpolatedPtr, 
											 float  Pilotfrequenz)
	{
	long LowCutoffFrequencyArrPos;
	long HighCutoffFrequencyArrPos;
	long LowPilotChannelBoundaryArrPos;
	long HighPilotChannelBoundaryArrPos;
	float Frequenzbreite;
	float BeginValue;

	Frequenzbreite = ((float) PN_ACCUMULATOR_LENGTH) / ((float) AF_SAMPLE_FREQ); 	// 1/(Frequenzauflösung FFT in [Hz])
	LowCutoffFrequencyArrPos = (long) ((LOW_FREQUENCY_CUTOFF * Frequenzbreite) + 1);
	HighCutoffFrequencyArrPos = (long) (HIGH_FREQUENCY_CUTOFF * Frequenzbreite) - 1 ;
	LowPilotChannelBoundaryArrPos = (long) ((Pilotfrequenz - PILOT_CHANNEL_BANDWIDTH*FLOAT_ZERO_POINT_FIVE) * 
																							Frequenzbreite);
    HighPilotChannelBoundaryArrPos = (long) (FLOAT_ONE + (Pilotfrequenz + 
    										PILOT_CHANNEL_BANDWIDTH*FLOAT_ZERO_POINT_FIVE) * Frequenzbreite);
	SetFloatArrayZero((float *) AbsTransferFunctionEqInterpolatedPtr, (PN_ACCUMULATOR_LENGTH/ZWEI) + 1);
	BeginValue = sqrtf((RealPartTransferFunctionEqPtr[LowCutoffFrequencyArrPos] * 
						RealPartTransferFunctionEqPtr[LowCutoffFrequencyArrPos]) +
						(ImagPartTransferFunctionEqPtr[LowCutoffFrequencyArrPos] * 
						 ImagPartTransferFunctionEqPtr[LowCutoffFrequencyArrPos]) );
	if (BeginValue < 0) BeginValue = FLOAT_ZERO_F;
	
	InterpolatePieceTransferFunction(0, 0, LowCutoffFrequencyArrPos, 
									BeginValue,
									0, 
									RealPartTransferFunctionEqPtr[LowCutoffFrequencyArrPos + 1],
									ImagPartTransferFunctionEqPtr[LowCutoffFrequencyArrPos + 1],
									RealPartTransferFunctionEqPtr, ImagPartTransferFunctionEqPtr,
									AbsTransferFunctionEqInterpolatedPtr);
	AbsTransferFunctionEqInterpolatedPtr[0] = BeginValue; 
	RealPartTransferFunctionEqPtr[0] = FLOAT_ZERO_F;
	ImagPartTransferFunctionEqPtr[0] = FLOAT_ZERO_F;
									
	InterpolatePieceTransferFunction(0, HighCutoffFrequencyArrPos, PN_ACCUMULATOR_LENGTH/ZWEI, 
									RealPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos],
									ImagPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos],
									RealPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos],
									ImagPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos],
									RealPartTransferFunctionEqPtr, ImagPartTransferFunctionEqPtr,
									AbsTransferFunctionEqInterpolatedPtr);
	RealPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = FLOAT_ZERO_F;
	ImagPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = FLOAT_ZERO_F;
	AbsTransferFunctionEqInterpolatedPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = sqrtf((RealPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos] * 
																						RealPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos]) + 
																				(ImagPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos] *
																				 ImagPartTransferFunctionEqPtr[HighCutoffFrequencyArrPos]) );
	
	InterpolatePieceTransferFunction(1, LowPilotChannelBoundaryArrPos, HighPilotChannelBoundaryArrPos, 
									RealPartTransferFunctionEqPtr[LowPilotChannelBoundaryArrPos],
									ImagPartTransferFunctionEqPtr[LowPilotChannelBoundaryArrPos],
									RealPartTransferFunctionEqPtr[HighPilotChannelBoundaryArrPos],
									ImagPartTransferFunctionEqPtr[HighPilotChannelBoundaryArrPos],
									RealPartTransferFunctionEqPtr, ImagPartTransferFunctionEqPtr,
									AbsTransferFunctionEqInterpolatedPtr);
	}    

/*--------------------------------------------------------------------------*/
/* long CalculateBalancePoint(float *Stossantwort);							*/
/*																			*/
/* Returns the Position of balance point in the array Stossantwort of 256	*/
/* samples																	*/
/*																			*/
/* Input:	*Stossantwort:	Pointer to an array of length 256 containing	*/
/*							the step response as float values				*/
/*--------------------------------------------------------------------------*/


static long SEGMENT_EXT_CODE CalculateBalancePoint(float *StossantwortPtr)
{	
	float summe;
	float lastsumme;
	long position;
	long index;
	long ReturnPosition;
	long ArrayPos1;
	long ArrayPos2;
	
	lastsumme = BIG_FLOAT_NUMBER;
	ReturnPosition = 0L;
	for (position = 0; position < PN_ACCUMULATOR_LENGTH + 1; position++)
	{
		summe = FLOAT_ZERO_F;			// Neue Berechnung, zuerst initialisieren
		for (index = 1; index < PN_ACCUMULATOR_LENGTH/ZWEI; index++)
		{
			ArrayPos1 = (long) ((position + index + PN_ACCUMULATOR_LENGTH) % PN_ACCUMULATOR_LENGTH);
			ArrayPos2 = (long) ((position - index + PN_ACCUMULATOR_LENGTH) % PN_ACCUMULATOR_LENGTH);
			summe = summe + index * fabsf(StossantwortPtr[ArrayPos1]) + index * fabsf(StossantwortPtr[ArrayPos2]);
		}
		if (lastsumme >= summe)
		{
			lastsumme = summe;
			ReturnPosition = (long) (position % PN_ACCUMULATOR_LENGTH);  // calculate first position twice
		}                                                               // initialisation no comparison
	}
	return ReturnPosition;
} 
	
	

/*----------------------------------------------------------------------------------*/
/* void CalculateStepResponseEqualizer(float *RealPartTransferFunctionEqPtr,		*/ 
/*	                                   float *ImagPartTransferFunctionEqPtr,		*/
/*                                     float *AbsTransferFunctionEqInterpolatedPtr,	*/
/*                                     float *StepResponseEqualizerPtr,				*/
/*                                     long *SchwerpunktPtr)						*/
/*                                                                  				*/
/* Calculates the step response and the balance point of the equalizer of length	*/
/*                                                                  				*/
/* 256.                                                                				*/
/* Input:	*RealPartTransferFunctionEqPtr:	Pointer to an array of 129 samples		*/
/*											representing the real part of the		*/
/*											equalizer transfer function from 0..fs/2*/ 
/*			*ImagPartTransferFunctionEqPtr:	Pointer to an array of 129 samples		*/
/*											representing the imag part of the		*/
/*											equalizer transfer function from 0..fs/2*/
/*			*AbsTransferFunctionEqInterpolatedPtr:	Pointer to an array of 129		*/
/*													samples representing the		*/
/*													absolute part of the 			*/
/*													interpolated equalizer transfer	*/
/*													function from 0..fs/2			*/			
/* Output:	*SchwerpunktPtr:	Pointer to Position of the balance point of step	*/
/*								response (is a long)								*/
/* 			*StepResponseEqualizerPtr:	Pointer to an array of length 256 containing*/
/*										the step response as float values			*/
/*----------------------------------------------------------------------------------*/
 
void SEGMENT_EXT_CODE CalculateStepResponseEqualizer(float *RealPartTransferFunctionEqPtr, 
													 float *ImagPartTransferFunctionEqPtr,
													 float *AbsTransferFunctionEqInterpolatedPtr, 
													 float *StepResponseEqualizerPtr,
													 long  *SchwerpunktPtr, 
													 long   ETL_GroupDelayEqualisationEnabled)
{	
	long index;
	long index1;
	long SchwerpunktInterpolated;
	long ArrayPos1;
	long ArrayPos2;
	long delta;
	float Summe;
	float LastSumme;


	// Allocate Arrays dynamically

	float *RealInputBackupArrPtr;
	float *ImagInputBackupArrPtr;
	RealInputBackupArrPtr = (float *) calloc(PN_ACCUMULATOR_LENGTH, sizeof(float));
	ImagInputBackupArrPtr = (float *) calloc(PN_ACCUMULATOR_LENGTH, sizeof(float));
	
	// Bestimme Stossantwort gültige Werte Equalizer, je nachdem, ob mit oder ohne Laufzeitentzerrung

	if (ETL_GroupDelayEqualisationEnabled)	// no groupdelay equalisation selected
	{
		RealInputBackupArrPtr[0] =  sqrtf( (RealPartTransferFunctionEqPtr[0] * RealPartTransferFunctionEqPtr[0]) +
											(ImagPartTransferFunctionEqPtr[0] * ImagPartTransferFunctionEqPtr[0]) ) +
									AbsTransferFunctionEqInterpolatedPtr[0];
		ImagInputBackupArrPtr[0] = 0.0F;
		RealInputBackupArrPtr[PN_ACCUMULATOR_LENGTH/ZWEI] =  
									sqrtf( (RealPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI] * 
									RealPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI]) +
									(ImagPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI] * 
									ImagPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI]) ) +
									AbsTransferFunctionEqInterpolatedPtr[PN_ACCUMULATOR_LENGTH/ZWEI];
		ImagInputBackupArrPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = 0.0F;
		for (index = 1; index < PN_ACCUMULATOR_LENGTH/ZWEI; index++)
		{
			// Real erweitern und Imaginärteil nullsetzen
			RealInputBackupArrPtr[index] = sqrtf( (RealPartTransferFunctionEqPtr[index] * RealPartTransferFunctionEqPtr[index]) +
													(ImagPartTransferFunctionEqPtr[index] * ImagPartTransferFunctionEqPtr[index])) +
													AbsTransferFunctionEqInterpolatedPtr[index];
			ImagInputBackupArrPtr[index] = 0.0F;
			RealInputBackupArrPtr[PN_ACCUMULATOR_LENGTH - index] = RealInputBackupArrPtr[index];
			ImagInputBackupArrPtr[PN_ACCUMULATOR_LENGTH - index] = 0.0F;

		}
	}
	else
	{
		RealInputBackupArrPtr[0] = RealPartTransferFunctionEqPtr[0];
		ImagInputBackupArrPtr[0] = ImagPartTransferFunctionEqPtr[0];
		RealInputBackupArrPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = 
									RealPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI];
		ImagInputBackupArrPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = 
									ImagPartTransferFunctionEqPtr[PN_ACCUMULATOR_LENGTH/ZWEI];
		for (index = 1; index < PN_ACCUMULATOR_LENGTH/ZWEI; index++)
		{
			// Konjugiert komplex erweitern, falls Groupdelay Entzerrung eingeschaltet
			RealInputBackupArrPtr[index] = RealPartTransferFunctionEqPtr[index];
			ImagInputBackupArrPtr[index] = ImagPartTransferFunctionEqPtr[index];
			RealInputBackupArrPtr[PN_ACCUMULATOR_LENGTH - index] = 
									RealPartTransferFunctionEqPtr[index];
			ImagInputBackupArrPtr[PN_ACCUMULATOR_LENGTH - index] = 
									- ImagPartTransferFunctionEqPtr[index];
		}
	}
		
	// Reverse Fourier Transformation by swapping real and imaginary part of input and output:

	  
	cfft256(ImagInputBackupArrPtr, RealInputBackupArrPtr, ImagInputBackupArrPtr, StepResponseEqualizerPtr);
    
   
	// Bestimme Stossantwort interpolierter Frequenzgang Equalizer, nur nötig für group delay equalisation
//	if (ETL_GroupDelayEqualisationEnabled == EQUALIZER_WITH_GROUP_DELAY_EQUALISATION)
	if (ETL_GroupDelayEqualisationEnabled)
	{
		RealInputBackupArrPtr[0] = AbsTransferFunctionEqInterpolatedPtr[0];
		RealInputBackupArrPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = 
									AbsTransferFunctionEqInterpolatedPtr[PN_ACCUMULATOR_LENGTH/ZWEI];

		ImagInputBackupArrPtr[0] = FLOAT_ZERO_F;
		ImagInputBackupArrPtr[PN_ACCUMULATOR_LENGTH/ZWEI] = FLOAT_ZERO_F;

		for (index = 1; index < PN_ACCUMULATOR_LENGTH/ZWEI; index++)
			{
			// Konjugiert komplex erweitern
			RealInputBackupArrPtr[index] = AbsTransferFunctionEqInterpolatedPtr[index];
			RealInputBackupArrPtr[PN_ACCUMULATOR_LENGTH - index] = 
								AbsTransferFunctionEqInterpolatedPtr[index];
			ImagInputBackupArrPtr[index] = FLOAT_ZERO_F;
			ImagInputBackupArrPtr[index + PN_ACCUMULATOR_LENGTH/ZWEI] = FLOAT_ZERO_F;
			}
		
		// Reverse Fourier Transformation by swapping real and imaginary part of input and output:
	
		
		cfft256(ImagInputBackupArrPtr, RealInputBackupArrPtr, ImagInputBackupArrPtr, RealInputBackupArrPtr);
	}

	// Bestimme Schwerpunkte der beiden Stossantworten

	*SchwerpunktPtr = CalculateBalancePoint(StepResponseEqualizerPtr);

	if (ETL_GroupDelayEqualisationEnabled == EQUALIZER_WITH_GROUP_DELAY_EQUALISATION)
	{
		SchwerpunktInterpolated = CalculateBalancePoint(RealInputBackupArrPtr);


		// Shift 2nd sequence until energy is maximized in main lope
    
    
	    Summe = FLOAT_ZERO_F;
		delta = 0;
		LastSumme = BIG_FLOAT_NUMBER;
		for (index = 0; index < EQUALIZER_FINE_SHIFT; index++)
    	{
			for (index1 = 0; index1 < (PN_ACCUMULATOR_LENGTH - EQUALIZER_SHORT_LENGTH); index1++)
    		{
    			ArrayPos1 = (long) ((*SchwerpunktPtr + EQUALIZER_SHORT_LENGTH/ZWEI + index1) % PN_ACCUMULATOR_LENGTH);
				ArrayPos2 = (long) ((SchwerpunktInterpolated + index + index1 + EQUALIZER_SHORT_LENGTH/ZWEI - EQUALIZER_FINE_SHIFT/ZWEI) % PN_ACCUMULATOR_LENGTH);
				Summe = fabsf(StepResponseEqualizerPtr[ArrayPos1]);
				Summe = Summe + fabsf(RealInputBackupArrPtr[ArrayPos2]);
			}
			if (LastSumme > Summe)
	    	{
		    	delta = index;
		    	LastSumme = Summe;
		    }
		    Summe = FLOAT_ZERO_F;  
		}
	
		for (index = 0; index < PN_ACCUMULATOR_LENGTH; index++)
		{
			ArrayPos1 = (long) ((*SchwerpunktPtr + index) % PN_ACCUMULATOR_LENGTH);
			ArrayPos2 = (long) ((SchwerpunktInterpolated + index) % PN_ACCUMULATOR_LENGTH);
		 // Bug ifft Routine Library, Faktor 1/N nicht normiert -- Alle Werte entsprechend normieren
			StepResponseEqualizerPtr[ArrayPos1] = (FLOAT_ONE/PN_ACCUMULATOR_LENGTH) * 
															(StepResponseEqualizerPtr[ArrayPos1] + 
															RealInputBackupArrPtr[ArrayPos2]);
		}
	}
	else
	{
		for (index = 0; index < PN_ACCUMULATOR_LENGTH; index++)
		{
		// Bug ifft Routine Library, Faktor 1/N nicht normiert -- Alle Werte entsprechend normieren
			StepResponseEqualizerPtr[index] = (FLOAT_ONE/PN_ACCUMULATOR_LENGTH) * 
															StepResponseEqualizerPtr[index];
		}
	}
		
	// Deallocate dynamically allocated memory
	
	free(ImagInputBackupArrPtr);
	free(RealInputBackupArrPtr);
}
	
	
	

		
	
