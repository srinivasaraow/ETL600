/*-------------------------------------------------------------------------*
 * $Workfile: EQ_COEFF.H $
 * Part of	: ETL600 / P4LT FW & HMI600
 * Language	: C
 * Created by	: B. Busslinger NTC-1
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/HMI600/H/EQ_COEFF.H $
 * 
 * 2     18.05.04 11:48 Schnyd01
 * 
 * 1     10.03.04 12:14 Nohl01
 * Initial version
 * Based on MMI500 v. 3.20
 *-------------------------------------------------------------------------*/
/*-----------------------GLOBAL CONSTANTS----------------------------------*/
#ifndef 	_EQ_COEFF
#define 	_EQ_COEFF


// Filter Koeffizienten für Standard Equalizer
// Insert coefficients here from Matlab
// Add F (for float) MMI
// Matlab File: eq_stand.m
// Coefficients: Generated stand_eq.dat through eq_stand.m
// Calculate new CRC for Standard EQ in pn_gener.c read with emulator on DSPB LastCRC Value with ETL setting Standard
// equalizer in decimal format and insert this calculated CRC in entzerr.h in define CRC_STANDARD_EQUALIZER
// First enable this variable LastCRC in function CheckCRC_Equalizer (pn_gener.c) and after calculation disable variable.
// Change EQUALIZER_STANDARD_LENGTH in entzerr.h
// NOTE: LENGTH CHANGE WILL AFFECT MMI !!!!!! 

// With O4LE, no equalisation is used, so first coefficient is 1.0 and all others are 0

float StandardEntzerrerKoeff[EQUALIZER_STANDARD_LENGTH]=
	{  1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F
	};

#define GROUPDELAY_ENTZERRER_STANDARD	0L


#endif //_EQ_COEFF
