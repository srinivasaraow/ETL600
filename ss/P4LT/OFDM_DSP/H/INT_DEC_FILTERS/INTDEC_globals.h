/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: INTDEC_globals.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Global variables for OFDM Interpolation/Decimation 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       21.06.2004 PL
 *****************************************************************************/

#include "INTDEC_defs.h"
#include "../ofdm_defs.h"
#include <complex.h>

#ifndef INTDEC_GLOBALS_H
#define INTDEC_GLOBALS_H

/*****************************************************************
**					Interpolation/Decimation 					**
*****************************************************************/

complex_float *IntDec_Filt_Coeff; //-> Fl_Array_I[0]
complex_float *IntDec_Filt_Delay; //-> Fl_Array_R[12NL-8N]

float SFC_Interpolation;
/*****************************************************************
**					Interpolation  x2			**
*****************************************************************/

asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Int2x12_filt_c_Delay[INTFILT_2_LENGTH/INTFILT_2_POLYNUMBER+1];

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Int2x12_Filt_Coeff_V1[INTFILT_2_LENGTH] = {
#include "h2x12_0.dat"    
#include "h2x12_1.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Int2x12_Filt_Coeff_V2[INTFILT_2_LENGTH] = {
#include "SFC_FN4_8_2_ce2_a.dat"    
#include "SFC_FN4_8_2_ce2_b.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Int2x12_Filt_Coeff_V3[INTFILT_2_LENGTH] = {
#include "SFC_FN16_2_2_ce1_a.dat"    
#include "SFC_FN16_2_2_ce1_b.dat"
};

/*****************************************************************
**					Decimation  /2			**
*****************************************************************/

asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Dec2x12_filt_c_Delay[DECFILT_2_LENGTH+1];

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Dec2x12_Filt_Coeff_V1[DECFILT_2_LENGTH] = {
#include "dec2x12.dat"    
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Dec2x12_Filt_Coeff_V2[DECFILT_2_LENGTH] = {
#include "SFC_FN4_8_2_ce2_dec.dat"    
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Dec2x12_Filt_Coeff_V3[DECFILT_2_LENGTH] = {
#include "SFC_FN16_2_2_ce1_dec.dat"    
};

/*****************************************************************
**					Interpolation  x3			**
*****************************************************************/

asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Int3x24_filt_c_Delay[INTFILT_3_LENGTH/INTFILT_3_POLYNUMBER+1];

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int3x24_Filt_Coeff_V1[INTFILT_3_LENGTH] = {
#include "h3x24_0.dat"    
#include "h3x24_1.dat"
#include "h3x24_2.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int3x24_Filt_Coeff_V2[INTFILT_3_LENGTH] = {
#include "SFC_FN6_8_3_ce2_a.dat"    
#include "SFC_FN6_8_3_ce2_b.dat"
#include "SFC_FN6_8_3_ce2_c.dat"
};

asm(".align 2;"); // for FN=4800 FFTx8 SFC_RX *3/5,and for FN=9600 FFTx4 SFC_RX *3/5
complex_float segment ( "seg_extn_init" ) Int3x24_Filt_Coeff_V3[INTFILT_3_LENGTH] = {
#include "SFC_FN48_8_3_ce2_a.dat"    
#include "SFC_FN48_8_3_ce2_b.dat"
#include "SFC_FN48_8_3_ce2_c.dat"
};

asm(".align 2;"); // for FN=24000 FFTx2 SFC_RX *3/4
complex_float segment ( "seg_extn_init" ) Int3x24_Filt_Coeff_V4[INTFILT_3_LENGTH] = {
#include "SFC_FN24_2_3_ce1_a.dat"    
#include "SFC_FN24_2_3_ce1_b.dat"
#include "SFC_FN24_2_3_ce1_c.dat"
};

/*****************************************************************
**					Interpolation  x4			**
*****************************************************************/

asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Int4x24_filt_c_Delay[INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1];
asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Int4x24_filt_c_Delay1[INTFILT_4_LENGTH/INTFILT_4_POLYNUMBER+1];

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int4x24_Filt_Coeff_V1[INTFILT_4_LENGTH] = {
#include "h4x24_0.dat"    
#include "h4x24_1.dat"
#include "h4x24_2.dat"
#include "h4x24_3.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int4x24_Filt_Coeff_V2[INTFILT_4_LENGTH] = {
#include "SFC_FN6_8_4_ce2_a.dat"    
#include "SFC_FN6_8_4_ce2_b.dat"
#include "SFC_FN6_8_4_ce2_c.dat"
#include "SFC_FN6_8_4_ce2_d.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int4x24_Filt_Coeff_V3[INTFILT_4_LENGTH] = {
#include "SFC_FN8_2_4_ce1_a.dat"    
#include "SFC_FN8_2_4_ce1_b.dat"
#include "SFC_FN8_2_4_ce1_c.dat"
#include "SFC_FN8_2_4_ce1_d.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int4x24_Filt_Coeff_V4[INTFILT_4_LENGTH] = {
#include "SFC_FN2_8_4_ce1_a.dat"    
#include "SFC_FN2_8_4_ce1_b.dat"
#include "SFC_FN2_8_4_ce1_c.dat"
#include "SFC_FN2_8_4_ce1_d.dat"
};

/*****************************************************************
**					Decimation  /4			**
*****************************************************************/


asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Dec4x24_filt_c_Delay[DECFILT_4_LENGTH+1];

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Dec4x24_Filt_Coeff[DECFILT_4_LENGTH] = {
#include "dec4x24.dat"    
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Dec4x24_Filt_Coeff_V3[DECFILT_4_LENGTH] = {
#include "SFC_FN8_2_4_ce1_dec.dat"    
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" )  Dec4x24_Filt_Coeff_V4[DECFILT_4_LENGTH] = {
#include "SFC_FN2_8_4_ce1_dec.dat"    
};

/*****************************************************************
**					Interpolation  x5			**
*****************************************************************/

asm(".align 2;");
complex_float segment ( "seg_extn SHT_NOBITS" ) Int5x25_filt_c_Delay[INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1];

asm(".align 2;");

complex_float segment ( "seg_extn_init" ) Int5x25_Filt_Coeff_V1[INTFILT_5_LENGTH] = {
#include "SFC_FN48_8_5_ce2_a.dat"    
#include "SFC_FN48_8_5_ce2_b.dat"
#include "SFC_FN48_8_5_ce2_c.dat"
#include "SFC_FN48_8_5_ce2_d.dat"
#include "SFC_FN48_8_5_ce2_e.dat"
};

asm(".align 2;");
complex_float segment ( "seg_extn_init" ) Int5x25_Filt_Coeff_V2[INTFILT_5_LENGTH] = {
#include "SFC_FN96_4_5_ce1_a.dat"    
#include "SFC_FN96_4_5_ce1_b.dat"
#include "SFC_FN96_4_5_ce1_c.dat"
#include "SFC_FN96_4_5_ce1_d.dat"
#include "SFC_FN96_4_5_ce1_e.dat"
};


#endif
