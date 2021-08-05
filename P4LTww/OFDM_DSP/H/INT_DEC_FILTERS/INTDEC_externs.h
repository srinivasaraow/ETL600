/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: INTDEC_externs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Extern variables for OFDM Interpolation/Decimation 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/H/INT_DEC_FILTERS/INTDEC_externs.h $
 * 
 * 8     24.07.08 15:21 Chhamaa
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:44 Chchlee
 * 
 * 3     1.10.04 12:19 Leeb02
 * 
 * 2     1.10.04 12:16 Leeb02
 * 
 * 
 * Initial version
 *       21.06.2004 PL
 *****************************************************************************/

#include "INTDEC_defs.h"
#include "../ofdm_defs.h"
#include <complex.h>

#ifndef INTDEC_EXTERNS_H
#define INTDEC_EXTERNS_H

/*****************************************************************
**					Interpolation/Decimation 					**
*****************************************************************/

extern complex_float *IntDec_Filt_Coeff; //-> Fl_Array_I[0]
extern complex_float *IntDec_Filt_Delay; //-> Fl_Array_R[12NL-8N]

extern float SFC_Interpolation;

extern complex_float Int2x12_filt_c_Delay[INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1];
extern complex_float Int2x12_Filt_Coeff_V1[INTFILT_2_LENGTH];
extern complex_float Int2x12_Filt_Coeff_V2[INTFILT_2_LENGTH];
extern complex_float Int2x12_Filt_Coeff_V3[INTFILT_2_LENGTH];

extern complex_float Dec2x12_filt_c_Delay[DECFILT_2_LENGTH+1];
extern complex_float Dec2x12_Filt_Coeff_V1[DECFILT_2_LENGTH];
extern complex_float Dec2x12_Filt_Coeff_V2[DECFILT_2_LENGTH];
extern complex_float Dec2x12_Filt_Coeff_V3[DECFILT_2_LENGTH];

extern complex_float Int3x24_filt_c_Delay[INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1];
extern complex_float Int3x24_Filt_Coeff_V1[INTFILT_3_LENGTH];
extern complex_float Int3x24_Filt_Coeff_V2[INTFILT_3_LENGTH];
extern complex_float Int3x24_Filt_Coeff_V3[INTFILT_3_LENGTH];
extern complex_float Int3x24_Filt_Coeff_V4[INTFILT_3_LENGTH];

extern complex_float Int4x24_filt_c_Delay[INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1];
extern complex_float Int4x24_filt_c_Delay1[INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1];

extern complex_float Int4x24_Filt_Coeff_V1[INTFILT_4_LENGTH];
extern complex_float Int4x24_Filt_Coeff_V2[INTFILT_4_LENGTH];
extern complex_float Int4x24_Filt_Coeff_V3[INTFILT_4_LENGTH];
extern complex_float Int4x24_Filt_Coeff_V4[INTFILT_4_LENGTH];

extern complex_float Dec4x24_filt_c_Delay[DECFILT_4_LENGTH+1];
extern complex_float Dec4x24_Filt_Coeff[DECFILT_4_LENGTH];
extern complex_float Dec4x24_Filt_Coeff_V3[DECFILT_4_LENGTH];
extern complex_float Dec4x24_Filt_Coeff_V4[DECFILT_4_LENGTH];

extern complex_float Int5x25_filt_c_Delay[INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1];
extern complex_float Int5x25_Filt_Coeff_V1[INTFILT_5_LENGTH];
extern complex_float Int5x25_Filt_Coeff_V2[INTFILT_5_LENGTH];

#endif
