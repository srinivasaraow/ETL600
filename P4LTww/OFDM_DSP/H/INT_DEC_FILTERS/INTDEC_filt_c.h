/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: INTDEC_filt_c.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Definitions for Interpolation/Decimation Filters 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/H/INT_DEC_FILTERS/INTDEC_filt_c.h $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:44 Chchlee
 * 
 * 3     1.10.04 12:19 Leeb02
 * 
 * 
 * 
 * Initial version
 *       21.06.2004 PL
 *****************************************************************************/

#ifndef INTDEC_FILT_C_H
#define INTDEC_FILT_C_H

#include <complex.h>

void Int2x12_filt_c_init(complex_float *Delay_Line);

void Int2x12_filt_c( complex_float  vOut[], complex_float    vIn[],
                    int               nIn, complex_float pm *vCoeffs,
                    complex_float  *vDelay); 


void Dec2x12_filt_c_init(complex_float *Delay_Line);

void Dec2x12_filt_c( complex_float  vOut[], complex_float    vIn[],
                    int               nIn, complex_float pm *vCoeffs,
                    complex_float  *vDelay); 



                    
void Int4x24_filt_c_init(complex_float *Delay_Line);

void Int4x24_filt_c( complex_float  vOut[], complex_float    vIn[],
                    int               nIn, complex_float pm *vCoeffs,
                    complex_float  *vDelay); 

void Dec4x24_filt_c_init(complex_float *Delay_Line);

void Dec4x24_filt_c( complex_float  vOut[], complex_float    vIn[],
                    int               nIn, complex_float pm *vCoeffs,
                    complex_float  *vDelay); 


void Int3x24_filt_c_init(complex_float *Delay_Line);

void Int3x24_filt_c( complex_float  vOut[], complex_float    vIn[],
                    int               nIn, complex_float pm *vCoeffs,
                    complex_float  *vDelay); 


void Int5x25_filt_c_init(complex_float *Delay_Line);

void Int5x25_filt_c( complex_float  vOut[], complex_float    vIn[],
                    int               nIn, complex_float pm *vCoeffs,
                    complex_float  *vDelay); 



#endif

