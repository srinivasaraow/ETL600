/******************************************************************************
 *
 * PROJECT     : DRS7  
 * FILE        : $Workfile: util.h $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec parameter definitions
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       15.02.2003 vgp
 *****************************************************************************/

#ifndef  UTIL_H
#define  UTIL_H

#include <complex.h>

void DRS7_setseed(unsigned int);
unsigned int DRS7_getseed();
int DRS7_rand();
void DRS7_vrand_D(int R[], int LEN);   
void DRS7_vrand_E(int R[], int LEN);   

void  vgauss_f_D(float *, float , float , int);

int Pack_1Bit_To_32BitWord_DD(int R[], int X[], int LEN_OF_X);  
int Pack_1Bit_To_32BitWord_EE(int R[], int X[], int LEN_OF_X);  
int Pack_1Bit_To_32BitWord_ED(int R[], int X[], int LEN_OF_X);  
int Pack_1Bit_To_32BitWord_DE(int R[], int X[], int LEN_OF_X);  

void Unpack_32Bit_To_1BitWord_DD(int R[], int X[],  int LEN_OF_X);  
void Unpack_32Bit_To_1BitWord_EE(int R[], int X[],  int LEN_OF_X);  
void Unpack_32Bit_To_1BitWord_ED(int R[], int X[],  int LEN_OF_X);  
void Unpack_32Bit_To_1BitWord_DE(int R[], int X[],  int LEN_OF_X);  

void vabs_uc_DDP(float R[], float X[], float pm Y[], int LEN);

void vadd_f_EEE( float R[], float X[], float Y[], int L );
void vadd_f_EED( float R[], float X[], float Y[], int L );
void vadd_f_EDE( float R[], float X[], float Y[], int L );
void vadd_f_DEE( float R[], float X[], float Y[], int L );
void vadd_f_DED( float R[], float X[], float Y[], int L );
void vadd_f_DDE( float R[], float X[], float Y[], int L );
void vadd_f_EDD( float R[], float X[], float Y[], int L );

void vadd_sf_EEE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
void vadd_sf_EED( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
void vadd_sf_EDE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
void vadd_sf_DEE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
void vadd_sf_DED( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
void vadd_sf_DDE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
void vadd_sf_EDD( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );

void vmag2_uc_DDP(float R[], float X[], float pm Y[], int LEN);
void vmag2_c_DD(float R[], complex_float X[], int LEN);
void vmag2_c_DE(float R[], complex_float X[], int LEN);
void vmag2_c_ED(float R[], complex_float X[], int LEN);

void vconjg_c_DD(complex_float R[], complex_float X[], int LEN); 

void vcopy_EE( float R[], float X[], int LEN);
void vcopy_DE( float R[], float X[], int LEN);
void vcopy_ED( float R[], float X[], int LEN);

void vcopy_s_EE( float R[], int stride_R, float X[], int stride_X, int N);
void vcopy_s_ED( float R[], int stride_R, float X[], int stride_X, int N);
void vcopy_s_DE( float R[], int stride_R, float X[], int stride_X, int N);

void vfix_c_DD( int R[], complex_float X[], int exp_scale, int L );

void vfloat_DE( float R[], int X[], int s, int LEN);

void vsmul_f_EE( float R[], float sc, float Y[], int L );
void vsmul_f_ED( float R[], float sc, float Y[], int L );
void vsmul_f_DE( float R[], float sc, float Y[], int L );

void vmul_c_EEE( complex_float R[], complex_float X[], complex_float Y[], int LEN);
void vmul_c_EED( complex_float R[], complex_float X[], complex_float Y[], int LEN);
void vmul_c_EDE( complex_float R[], complex_float X[], complex_float Y[], int LEN);
void vmul_c_DEE( complex_float R[], complex_float X[], complex_float Y[], int LEN);
void vmul_c_DED( complex_float R[], complex_float X[], complex_float Y[], int LEN);
void vmul_c_DDE( complex_float R[], complex_float X[], complex_float Y[], int LEN);
void vmul_c_EDD( complex_float R[], complex_float X[], complex_float Y[], int LEN);

void vmul_f_EEE( float R[], float X[], float Y[], int LEN );
void vmul_f_EED( float R[], float X[], float Y[], int LEN );
void vmul_f_EDE( float R[], float X[], float Y[], int LEN );
void vmul_f_DEE( float R[], float X[], float Y[], int LEN );
void vmul_f_DED( float R[], float X[], float Y[], int LEN );
void vmul_f_DDE( float R[], float X[], float Y[], int LEN );
void vmul_f_EDD( float R[], float X[], float Y[], int LEN );

void vdiv_f_EEE( float R[], float X[], float Y[], int L );
void vdiv_f_EED( float R[], float X[], float Y[], int L );
void vdiv_f_EDE( float R[], float X[], float Y[], int L );
void vdiv_f_DEE( float R[], float X[], float Y[], int L );
void vdiv_f_DED( float R[], float X[], float Y[], int L );
void vdiv_f_DDE( float R[], float X[], float Y[], int L );
void vdiv_f_EDD( float R[], float X[], float Y[], int L );

int vones_i_E( int X[], int L );
int vones_i_D( int X[], int L );

void vset_f_E( float R[], float s, int LEN);
void vset_i_E( int R[], int s, int LEN);

void vset_sf_E( float R[], int stride, float scalar, int LEN);
void vset_si_E( int R[], int stride, int scalar, int LEN);

float vsummag2_c_E( complex_float X[], int L );

void vwin_c_DDD( complex_float R[], complex_float X[], float W[], int LEN );
void vmul_ccf_DDD( complex_float R[], complex_float X[], float Y[], int LEN );

void vxor_i_EEE( int R[], int X[], int Y[], int LEN );
void vxor_i_EED( int R[], int X[], int Y[], int LEN );
void vxor_i_EDE( int R[], int X[], int Y[], int LEN );
void vxor_i_DEE( int R[], int X[], int Y[], int LEN );
void vxor_i_DED( int R[], int X[], int Y[], int LEN );
void vxor_i_DDE( int R[], int X[], int Y[], int LEN );
void vxor_i_EDD( int R[], int X[], int Y[], int LEN );

float vsummag2_uc_DP( float X[], float pm Y[], int LEN );

void vsort_f_DD(float R[], float X[], int L );
void vsort_f_DE(float R[], float X[], int L );
void vsort_f_ED(float R[], float X[], int L );
void vsort_f_EE(float R[], float X[], int L );

void vsorti_f_DDD(float R[], int IndX[], float X[], int L );
void vsorti_f_DDE(float R[], int IndX[], float X[], int L );
void vsorti_f_DED(float R[], int IndX[], float X[], int L );
void vsorti_f_DEE(float R[], int IndX[], float X[], int L );
void vsorti_f_EDD(float R[], int IndX[], float X[], int L );
void vsorti_f_EDE(float R[], int IndX[], float X[], int L );
void vsorti_f_EED(float R[], int IndX[], float X[], int L );
void vsorti_f_EEE(float R[], int IndX[], float X[], int L );

void vsortii_f_DDDD(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DDDE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DDED(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DDEE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DEDD(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DEDE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DEED(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_DEEE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EDDD(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EDDE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EDED(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EDEE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EEDD(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EEDE(float R[], int IndX[], float X[], int XI[], int L );
void vsortii_f_EEEE(float R[], int IndX[], float X[], int XI[], int L );

void vcopy_PE( float pm R[], float X[], int L );
void vcopy_EP( float R[], float pm X[], int L );

float vsum_f_E( float X[], int L );
int vsum_i_E( int X[], int L );

void vfloat_cs_DD( complex_float R[],int X[], int complex_stride, int exp_scale, int L );

float vmin_f_E( float X[], int L );
float vmax_f_E( float X[], int L );
int   vmin_i_E( int X[], int L );
int   vmax_i_E( int X[], int L );

void vdivz_c_DDD(complex_float R[], complex_float X[], complex_float Y[], int N);
void vdivz_f_DDP(float R[], float X[], float pm Y[], int N);
void vdivz_f_DDD(float R[], float X[], float Y[], int N);
void vdivz_f_EEE( float R[], float X[], float Y[], int L );
void vdivz_f_EED( float R[], float X[], float Y[], int L );
void vdivz_f_EDE( float R[], float X[], float Y[], int L );
void vdivz_f_DEE( float R[], float X[], float Y[], int L );
void vdivz_f_DED( float R[], float X[], float Y[], int L );
void vdivz_f_DDE( float R[], float X[], float Y[], int L );
void vdivz_f_EDD( float R[], float X[], float Y[], int L );

void svdivz_f_DD( float R[], float   s, float X[], int L );


void vkalman_c_DDD( complex_float R[], complex_float X[], complex_float Y[], float alpha, int L );
void vkalman_f_DDD( float R[], float X[], float Y[], float alpha, int L );

void vfsmulvadd_c_DDD( complex_float R[], float fs, complex_float X[], complex_float Y[], int L );
void vsubmul_c_DDPD( complex_float R[], complex_float X[], complex_float pm Y[], complex_float Z[], int L );
//void vsubmul_c_DDDD( complex_float R[], complex_float X[], complex_float pm Y[], complex_float Z[], int L );

#endif
