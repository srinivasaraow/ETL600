
/******************************************************************************
 *
 * SpeedDSP Master Include File
 *
 * DESCRIPTION: This file contains prototypes for all functions in the
 *              SpeedDSP library.
 *
 * AUTHOR:      LightSpray, Inc.
 * 
 *****************************************************************************/

/*---------------------------------------------------------------------------*
 * $Workfile: speeddsp.h $
 * Part of      : ETL600 / OFDM Modem
 * Language     : C
 * Modified by   : V. Potapov
 * Remarks      :  
 * Purpose      : SPEEDDSP Library Interface modified by TLC
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/speeddsp.h $
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 7     29.09.04 11:42 Maag01
 * 
 * 5     11.08.04 15:11 Leeb02
 * 
 * 3     5.07.04 16:26 Maag01
 * Type of vDelay changed to float (used in filtering routines)
 * 
 * 2     30.06.04 15:17 Leeb02
 * Initial Version
 *---------------------------------------------------------------------------*/


#ifndef __SPEEDDSP
#define __SPEEDDSP

/*
 * Complex arithmetic functions
 */
#include <macros.h>

// vgp
#ifndef __COMPLEX_DEFINED         
#ifndef __COMPLEX_H_DEFINED       
#ifndef __COMPLEX_FLOAT_DEFINED
#define __COMPLEX_H_DEFINED
#define __COMPLEX_DEFINED
typedef struct 
{
    float re;
    float im;
} complex_float;
#endif
#endif
#endif
//end of vgp

/*
 * FFT routines
 */
int fft_c_64( float dm         outR[],
              float pm         outI[],
              complex_float dm in1[] );
int fft_c_128( float dm         outR[],
               float pm         outI[],
               complex_float dm in1[] );
int fft_c_256( float dm         outR[],
               float pm         outI[],
               complex_float dm in1[] );
int fft_c_512( float dm         outR[],
               float pm         outI[],
               complex_float dm in1[] );
int fft_c_1024( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );
int fft_c_2048( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );
int fft_c_4096( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );
int fft_c_8192( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );

int ifft_c_64( float dm         outR[],
               float pm         outI[],
               complex_float dm in1[] );
int ifft_c_128( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );
int ifft_c_256( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );
int ifft_c_512( float dm         outR[],
                float pm         outI[],
                complex_float dm in1[] );
int ifft_c_1024( float dm         outR[],
                 float pm         outI[],
                 complex_float dm in1[] );
int ifft_c_2048( float dm         outR[],
                 float pm         outI[],
                 complex_float dm in1[] );
int ifft_c_4096( float dm         outR[],
                 float pm         outI[],
                 complex_float dm in1[] );
int ifft_c_8192( float dm         outR[],
                 float pm         outI[],
                 complex_float dm in1[] );

int fft_f_64( float dm outR[],
              float pm outI[],
              float dm in1[] );
int fft_f_128( float dm outR[],
               float pm outI[],
               float dm in1[] );
int fft_f_256( float dm outR[],
               float pm outI[],
               float dm in1[] );
int fft_f_512( float dm outR[],
               float pm outI[],
               float dm in1[] );
int fft_f_1024( float dm outR[],
                float pm outI[],
                float dm in1[] );
int fft_f_2048( float dm outR[],
                float pm outI[],
                float dm in1[] );
int fft_f_4096( float dm outR[],
                float pm outI[],
                float dm in1[] );
int fft_f_8192( float dm outR[],
                float pm outI[],
                float dm in1[] );
int fft_f_16384( float dm outR[],
                 float pm outI[],
                 float dm in1[] );

/*
 * Filtering routines
 */
float fir_f( float vIn,
             float pm vCoeffs[],
             int   nCoeffs,
             float vDelay[] );

void vfir_f( float vOut[], 
             float vIn[],
             int   nIn,
             float pm vCoeffs[],
             int   nCoeffs,
             float vDelay[] );

void vfir_c( complex_float vOut[], 
             complex_float vIn[],
             int           nIn,
             complex_float pm vCoeffs[],
             int           nCoeffs,
             float         vDelay[] );

void fir_f_init( float	vDelay[], 
             int      nCoeffs);

void viir_f( float    vOut[], 
             float    vIn[], 
             int      nIn, 
             float pm vCoeffs[], 
             int      nCoeffs,
             float    vDelay[] );

/*
 * Mathematical and Trigonometric routines
 */
float   acosq( float x );
float   asinq( float x );
float   atan2q( float x, float y );
float   atanq( float x );
float   cosq( float x );
float   expq( float x );
complex_float expq_c( complex_float x );
float   log10q( float x );
float   logq( float x );
void    polar_rect( float *x, float *y, float mag, float angle );
float   powq( float x, float y );
void    rect_polar( float *mag, float *angle, float x, float y );
float   sinq( float x );
float   sqrtq( float x );
float   tanq( float x );
float   sinhq( float x );
float   coshq( float x );

/*
 * Matrix routines
 */
void mattrans_f_DD( void *R, void *X, int rows, int cols );

int  matinv_f_DD( void *R, void *X, int rows );


/*
 * Sorting routines
 */
void bsort_f_D( float R[], int L );
void bsort_f_P( float R[], int L );

void hsort_f_D( float R[], int L );
void hsort_f_P( float R[], int L );

/*
 * Statistical routines
 */
void acorr_f_DD( float R[], float X[], int L, int LAGS );
void acorr_f_DP( float R[], float pm X[], int L, int LAGS );
void acorr_f_PD( float pm R[], float X[], int L, int LAGS );
void acorr_f_PP( float pm R[], float pm X[], int L, int LAGS );
void acorr_sf_DD( float R[], int i, float X[], int j, int L, int LAGS );
void acorr_sf_DP( float R[], int i, float pm X[], int j, int L, int LAGS );
void acorr_sf_PD( float pm R[], int i, float X[], int j, int L, int LAGS );
void acorr_sf_PP( float pm R[], int i, float pm X[], int j, int L, int LAGS );

float amean_f_D( float X[], int L );
float amean_f_P( float pm X[], int L );
float amean_sf_D( float X[], int i, int L );
float amean_sf_P( float pm X[], int j, int L );
int   amean_i_D( int X[], int L );
int   amean_i_P( int pm X[], int L );
int   amean_si_D( int X[], int i, int L );
int   amean_si_P( int pm X[], int j, int L );

void ccoh_f_DDD( float R[], float X[], float Y[], int L, int LAGS );
void ccoh_f_DDP( float R[], float X[], float pm Y[], int L, int LAGS );
void ccoh_f_DPD( float R[], float pm X[], float Y[], int L, int LAGS );
void ccoh_f_PDD( float pm R[], float X[], float Y[], int L, int LAGS );
void ccoh_f_PDP( float pm R[], float X[], float pm Y[], int L, int LAGS );
void ccoh_f_PPD( float pm R[], float pm X[], float Y[], int L, int LAGS );
void ccoh_sf_DDD( float R[], int i, float X[], int j, float Y[], int k, 
                   int L, int LAGS );
void ccoh_sf_DDP( float R[], int i, float X[], int j, float pm Y[], int k,
                   int L, int LAGS );
void ccoh_sf_DPD( float R[], int i, float pm X[], int j, float Y[], int k,
                   int L, int LAGS );
void ccoh_sf_PDD( float pm R[], int i, float X[], int j, float Y[], int k,
                   int L, int LAGS );
void ccoh_sf_PDP( float pm R[], int i, float X[], int j, float pm Y[], int k,
                   int L, int LAGS );
void ccoh_sf_PPD( float pm R[], int i, float pm X[], int j, float Y[], int k,
                   int L, int LAGS );

void ccorr_f_DDD( float R[], float X[], float Y[], int L, int LAGS );
void ccorr_f_DDP( float R[], float X[], float pm Y[], int L, int LAGS );
void ccorr_f_DPD( float R[], float pm X[], float Y[], int L, int LAGS );
void ccorr_f_PDD( float pm R[], float X[], float Y[], int L, int LAGS );
void ccorr_f_PDP( float pm R[], float X[], float pm Y[], int L, int LAGS );
void ccorr_f_PPD( float pm R[], float pm X[], float Y[], int L, int LAGS );
void ccorr_sf_DDD( float R[], int i, float X[], int j, float Y[], int k, 
                   int L, int LAGS );
void ccorr_sf_DDP( float R[], int i, float X[], int j, float pm Y[], int k,
                   int L, int LAGS );
void ccorr_sf_DPD( float R[], int i, float pm X[], int j, float Y[], int k,
                   int L, int LAGS );
void ccorr_sf_PDD( float pm R[], int i, float X[], int j, float Y[], int k,
                   int L, int LAGS );
void ccorr_sf_PDP( float pm R[], int i, float X[], int j, float pm Y[], int k,
                   int L, int LAGS );
void ccorr_sf_PPD( float pm R[], int i, float pm X[], int j, float Y[], int k,
                   int L, int LAGS );

float corr_f_DD( float X[], float Y[], int L );
float corr_f_DP( float X[], float pm Y[], int L );
float corr_f_PD( float pm X[], float Y[], int L );
float corr_f_PP( float pm X[], float pm Y[], int L );
float corr_sf_DD( float X[], int i, float Y[], int j, int L );
float corr_sf_DP( float X[], int i, float pm Y[], int j, int L );
float corr_sf_PD( float pm X[], int i, float Y[], int j, int L );
float corr_sf_PP( float pm X[], int i, float pm Y[], int j, int L );

float fact( int N );

void lsq_f_DD( float X[], float Y[], int L, float *SLOPE, float *INTERCEPT );
void lsq_f_DP( float X[], float pm Y[], int L, float *SLOPE, float *INTERCEPT );
void lsq_f_PD( float pm X[], float Y[], int L, float *SLOPE, float *INTERCEPT );
void lsq_f_PP( float pm X[], float pm Y[], int L, 
               float *SLOPE, float *INTERCEPT );
void lsq_sf_DD( float X[], int i, float Y[], int j, int L, 
                float *SLOPE, float *INTERCEPT );
void lsq_sf_DP( float X[], int i, float pm Y[], int j, int L, 
                float *SLOPE, float *INTERCEPT );
void lsq_sf_PD( float pm X[], int i, float Y[], int j, int L, 
                float *SLOPE, float *INTERCEPT );
void lsq_sf_PP( float pm X[], int i, float pm Y[], int j, int L, 
                float *SLOPE, float *INTERCEPT );

float meandev_f_D( float X[], int L );
float meandev_f_P( float pm X[], int L );
float meandev_sf_D( float X[], int i, int L );
float meandev_sf_P( float pm X[], int j, int L );
int   meandev_i_D( int X[], int L );
int   meandev_i_P( int pm X[], int L );
int   meandev_si_D( int X[], int i, int L );
int   meandev_si_P( int pm X[], int j, int L );

float random( void );
float rangauss( float mean, float stddev );

float range_f_D( float X[], int L );
float range_f_P( float pm X[], int L );
float range_sf_D( float X[], int i, int L );
float range_sf_P( float pm X[], int j, int L );
int   range_i_D( int X[], int L );
int   range_i_P( int pm X[], int L );
int   range_si_D( int X[], int i, int L );
int   range_si_P( int pm X[], int j, int L );

void ranseed( unsigned int seed );
float ranuniform( float mean, float stddev );

float rms_f_D( float X[], int L );
float rms_f_P( float pm X[], int L );
float rms_sf_D( float X[], int i, int L );
float rms_sf_P( float pm X[], int j, int L );
int   rms_i_D( int X[], int L );
int   rms_i_P( int pm X[], int L );
int   rms_si_D( int X[], int i, int L );
int   rms_si_P( int pm X[], int j, int L );

float stddev_f_D( float X[], int L );
float stddev_f_P( float pm X[], int L );
float stddev_sf_D( float X[], int i, int L );
float stddev_sf_P( float pm X[], int j, int L );
int   stddev_i_D( int X[], int L );
int   stddev_i_P( int pm X[], int L );
int   stddev_si_D( int X[], int i, int L );
int   stddev_si_P( int pm X[], int j, int L );

float var_f_D( float X[], int L );
float var_f_P( float pm X[], int L );
float var_sf_D( float X[], int i, int L );
float var_sf_P( float pm X[], int j, int L );
int var_i_D( int X[], int L );
int var_i_P( int pm X[], int L );
int var_si_D( int X[], int i, int L );
int var_si_P( int pm X[], int j, int L );

float wmean_f_DD( float X[], float W[], int L );
float wmean_f_DP( float X[], float pm W[], int L );
float wmean_f_PD( float pm X[], float W[], int L );
float wmean_f_PP( float pm X[], float pm W[], int L );
float wmean_sf_DD( float X[], int i, float W[], int j, int L );
float wmean_sf_DP( float X[], int i, float pm W[], int j, int L );
float wmean_sf_PD( float pm X[], int i, float W[], int j, int L );
float wmean_sf_PP( float pm X[], int i, float pm W[], int j, int L );
int wmean_i_DD( int X[], int W[], int L );
int wmean_i_DP( int X[], int pm W[], int L );
int wmean_i_PD( int pm X[], int W[], int L );
int wmean_i_PP( int pm X[], int pm W[], int L );
int wmean_si_DD( int X[], int i, int W[], int j, int L );
int wmean_si_DP( int X[], int i, int pm W[], int j, int L );
int wmean_si_PD( int pm X[], int i, int W[], int j, int L );
int wmean_si_PP( int pm X[], int i, int pm W[], int j, int L );

/*
 * Vector routines
 */
void vabs_f_DD( float R[], float X[], int L );
void vabs_f_DP( float R[], float pm X[], int L );
void vabs_f_PD( float pm R[], float X[], int L );
void vabs_f_PP( float pm R[], float pm X[], int L );
void vabs_sf_DD( float R[], int i, float X[], int j, int L );
void vabs_sf_DP( float R[], int i, float pm X[], int j, int L );
void vabs_sf_PD( float pm R[], int i, float X[], int j, int L );
void vabs_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vabs_i_DD( int R[], int X[], int L );
void vabs_i_DP( int R[], int pm X[], int L );
void vabs_i_PD( int pm R[], int X[], int L );
void vabs_i_PP( int pm R[], int pm X[], int L );
void vabs_si_DD( int R[], int i, int X[], int j, int L );
void vabs_si_DP( int R[], int i, int pm X[], int j, int L );
void vabs_si_PD( int pm R[], int i, int X[], int j, int L );
void vabs_si_PP( int pm R[], int i, int pm X[], int j, int L );

void vabs_c_DD( float R[], complex_float X[], int L );
void vabs_c_DP( float R[], complex_float pm X[], int L );
void vabs_c_PD( float pm R[], complex_float X[], int L );
void vabs_c_PP( float pm R[], complex_float pm X[], int L );
void vabs_sc_DD( float R[], int i, complex_float X[], int j, int L );
void vabs_sc_DP( float R[], int i, complex_float pm X[], int j, int L );
void vabs_sc_PD( float pm R[], int i, complex_float X[], int j, int L );
void vabs_sc_PP( float pm R[], int i, complex_float pm X[], int j, int L );

void vacos_f_DD( float R[], float X[], int L );
void vacos_f_DP( float R[], float pm X[], int L );
void vacos_f_PD( float pm R[], float X[], int L );
void vacos_f_PP( float pm R[], float pm X[], int L );
void vacos_sf_DD( float R[], int i, float X[], int j, int L );
void vacos_sf_DP( float R[], int i, float pm X[], int j, int L );
void vacos_sf_PD( float pm R[], int i, float X[], int j, int L );
void vacos_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vadb_f_DD( float R[], float X[], int L );
void vadb_f_DP( float R[], float pm X[], int L );
void vadb_f_PD( float pm R[], float X[], int L );
void vadb_f_PP( float pm R[], float pm X[], int L );
void vadb_sf_DD( float R[], int i, float X[], int j, int L );
void vadb_sf_DP( float R[], int i, float pm X[], int j, int L );
void vadb_sf_PD( float pm R[], int i, float X[], int j, int L );
void vadb_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vadd_f_DDD( float R[], float X[], float Y[], int L );
void vadd_f_DDP( float R[], float X[], float pm Y[], int L );
void vadd_f_PDP( float pm R[], float X[], float pm Y[], int L );
void vadd_sf_DDD( float R[], int i, float X[], int j, float Y[], int k, int L );
void vadd_sf_DDP( float R[], int i, float X[], int j, float pm Y[], int k,
                  int L );
void vadd_sf_PDP( float pm R[], int i, float X[], int j, float pm Y[], int k,
                  int L );

void vadd_i_DDD( int R[], int X[], int Y[], int L );
void vadd_i_DDP( int R[], int X[], int pm Y[], int L );
void vadd_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vadd_si_DDD( int R[], int i, int X[], int j, int Y[], int k, int L );
void vadd_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                  int L );
void vadd_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                  int L );

void vadd_c_DDD( complex_float R[], complex_float X[], complex_float Y[], 
                 int L );
void vadd_c_DDP( complex_float R[], complex_float X[], complex_float pm Y[], 
                 int L );
void vadd_c_PDP( complex_float pm R[], complex_float X[], complex_float pm Y[],
                 int L );
void vadd_sc_DDD( complex_float R[], int i, complex_float X[], int j, 
                  complex_float Y[], int k, int L );
void vadd_sc_DDP( complex_float R[], int i, complex_float X[], int j, 
                  complex_float pm Y[], int k, int L );
void vadd_sc_PDP( complex_float pm R[], int i, complex_float X[], int j, 
                  complex_float pm Y[], int k, int L );

void vaddadd_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vaddadd_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vaddadd_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vaddadd_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vaddadd_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], int L );
void vaddadd_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k,
                      float Z[], int m, int L );
void vaddadd_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vaddadd_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vaddadd_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vaddadd_sf_PDPP( float pm R[], int i, float X[], int j, 
                      float pm Y[], int k, float pm Z[], int m, int L );

void vaddadd_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vaddadd_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vaddadd_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vaddadd_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vaddadd_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], int L );
void vaddadd_si_DDDD( int R[], int i, int X[], int j, int Y[], int k,
                      int Z[], int m, int L );
void vaddadd_si_DDDP( int R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vaddadd_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vaddadd_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vaddadd_si_PDPP( int pm R[], int i, int X[], int j, 
                      int pm Y[], int k, int pm Z[], int m, int L );

void vaddadd_c_DDDD( complex_float R[], complex_float X[], 
                     complex_float Y[], complex_float Z[], int L );
void vaddadd_c_DDDP( complex_float R[], complex_float X[], 
                     complex_float Y[], complex_float pm Z[], int L );
void vaddadd_c_DDPP( complex_float R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vaddadd_c_PDDP( complex_float pm R[], complex_float X[], 
                     complex_float Y[], complex_float pm Z[], int L );
void vaddadd_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vaddadd_sc_DDDD( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float Z[], int m, 
                      int L );
void vaddadd_sc_DDDP( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float pm Z[], int m,
                      int L );
void vaddadd_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
                      int L );
void vaddadd_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
                      int L );
void vaddadd_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
                      int L );

void vaddmul_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vaddmul_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vaddmul_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vaddmul_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vaddmul_f_PDPD( float pm R[], float X[], float pm Y[], float Z[], int L );
void vaddmul_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vaddmul_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k, 
                      float Z[], int m, int L );
void vaddmul_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k, 
                      float pm Z[], int m, int L );
void vaddmul_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vaddmul_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vaddmul_sf_PDPD( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float Z[], int m, int L );
void vaddmul_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );
void vaddmul_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vaddmul_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vaddmul_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vaddmul_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vaddmul_i_PDPD( int pm R[], int X[], int pm Y[], int Z[], int L );
void vaddmul_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], int L );
void vaddmul_si_DDDD( int R[], int i, int X[], int j, int Y[], int k, int Z[], 
                      int m, int L );
void vaddmul_si_DDDP( int R[], int i, int X[], int j, int Y[], int k, 
                      int pm Z[], int m, int L );
void vaddmul_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k, 
                      int pm Z[], int m, int L );
void vaddmul_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k, 
                      int pm Z[], int m, int L );
void vaddmul_si_PDPD( int pm R[], int i, int X[], int j, int pm Y[], int k,
                      int Z[], int m, int L );
void vaddmul_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vaddmul_c( complex_float R[], complex_float X[], complex_float Y[], 
                complex_float Z[], int L );
void vaddmul_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vaddmul_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vaddmul_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vaddmul_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vaddmul_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vaddmul_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vaddmul_sc_DDDD( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float Z[], int m,
							 int L );
void vaddmul_sc_DDDP( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vaddmul_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vaddmul_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vaddmul_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vaddmul_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );

void vaddsub_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vaddsub_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vaddsub_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vaddsub_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vaddsub_f_PDPD( float pm R[], float X[], float pm Y[], float Z[], int L );
void vaddsub_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vaddsub_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k, 
                      float Z[], int m, int L );
void vaddsub_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k, 
                      float pm Z[], int m, int L );
void vaddsub_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vaddsub_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vaddsub_sf_PDPD( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float Z[], int m, int L );
void vaddsub_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );
void vaddsub_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vaddsub_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vaddsub_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vaddsub_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vaddsub_i_PDPD( int pm R[], int X[], int pm Y[], int Z[], int L );
void vaddsub_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], int L );
void vaddsub_si_DDDD( int R[], int i, int X[], int j, int Y[], int k, int Z[], 
                      int m, int L );
void vaddsub_si_DDDP( int R[], int i, int X[], int j, int Y[], int k, 
                      int pm Z[], int m, int L );
void vaddsub_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k, 
                      int pm Z[], int m, int L );
void vaddsub_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k, 
                      int pm Z[], int m, int L );
void vaddsub_si_PDPD( int pm R[], int i, int X[], int j, int pm Y[], int k,
                      int Z[], int m, int L );
void vaddsub_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vaddsub_c( complex_float R[], complex_float X[], complex_float Y[], 
                complex_float Z[], int L );
void vaddsub_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vaddsub_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vaddsub_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vaddsub_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vaddsub_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vaddsub_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vaddsub_sc_DDDD( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float Z[], int m,
							 int L );
void vaddsub_sc_DDDP( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vaddsub_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vaddsub_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vaddsub_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vaddsub_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );

void vand_i_DDD( int R[], int X[], int Y[], int L );
void vand_i_DDP( int R[], int X[], int pm Y[], int L );
void vand_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vand_si_DDD( int R[], int i, int X[], int j, int Y[], int k, int L );
void vand_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                  int L );
void vand_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                  int L );

void vasin_f_DD( float R[], float X[], int L );
void vasin_f_DP( float R[], float pm X[], int L );
void vasin_f_PD( float pm R[], float X[], int L );
void vasin_f_PP( float pm R[], float pm X[], int L );
void vasin_sf_DD( float R[], int i, float X[], int j, int L );
void vasin_sf_DP( float R[], int i, float pm X[], int j, int L );
void vasin_sf_PD( float pm R[], int i, float X[], int j, int L );
void vasin_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vatan_f_DD( float R[], float X[], int L );
void vatan_f_DP( float R[], float pm X[], int L );
void vatan_f_PD( float pm R[], float X[], int L );
void vatan_f_PP( float pm R[], float pm X[], int L );
void vatan_sf_DD( float R[], int i, float X[], int j, int L );
void vatan_sf_DP( float R[], int i, float pm X[], int j, int L );
void vatan_sf_PD( float pm R[], int i, float X[], int j, int L );
void vatan_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vbreverse_DD( void *R, void *X, int L );
void vbreverse_DP( void *R, void pm *X, int L );
void vbreverse_PD( void pm *R, void *X, int L );
void vbreverse_PP( void pm *R, void pm *X, int L );
void vbreverse_s_DD( void *R, int i, void *X, int j, int L );
void vbreverse_s_DP( void *R, int i, void pm *X, int j, int L );
void vbreverse_s_PD( void pm *R, int i, void *X, int j, int L );
void vbreverse_s_PP( void pm *R, int i, void pm *X, int j, int L );

void vbswap_DD( void *R, void *X, int L );
void vbswap_DP( void *R, void pm *X, int L );
void vbswap_PD( void pm *R, void *X, int L );
void vbswap_PP( void pm *R, void pm *X, int L );
void vbswap_s_DD( void *R, int i, void *X, int j, int L );
void vbswap_s_DP( void *R, int i, void pm *X, int j, int L );
void vbswap_s_PD( void pm *R, int i, void *X, int j, int L );
void vbswap_s_PP( void pm *R, int i, void pm *X, int j, int L );

void vclip_f_DD( float R[], float X[], float CLIPVAL, int L );
void vclip_f_DP( float R[], float pm X[], float CLIPVAL, int L );
void vclip_f_PD( float pm R[], float X[], float CLIPVAL, int L );
void vclip_f_PP( float pm R[], float pm X[], float CLIPVAL, int L );
void vclip_sf_DD( float R[], int i, float X[], int j, float CLIPVAL, int L );
void vclip_sf_DP( float R[], int i, float pm X[], int j, float CLIPVAL, int L );
void vclip_sf_PD( float pm R[], int i, float X[], int j, float CLIPVAL, int L );
void vclip_sf_PP( float pm R[], int i, float pm X[], int j, float CLIPVAL, 
                  int L );
void vclip_i_DD( int R[], int X[], int CLIPVAL, int L );
void vclip_i_DP( int R[], int pm X[], int CLIPVAL, int L );
void vclip_i_PD( int pm R[], int X[], int CLIPVAL, int L );
void vclip_i_PP( int pm R[], int pm X[], int CLIPVAL, int L );
void vclip_si_DD( int R[], int i, int X[], int j, int CLIPVAL, int L );
void vclip_si_DP( int R[], int i, int pm X[], int j, int CLIPVAL, int L );
void vclip_si_PD( int pm R[], int i, int X[], int j, int CLIPVAL, int L );
void vclip_si_PP( int pm R[], int i, int pm X[], int j, int CLIPVAL, int L );

void vcombine_c_DDD( complex_float R[], float RREAL[], float RIMAG[], int L );
void vcombine_c_DDP( complex_float R[], float RREAL[], float pm RIMAG[], 
                     int L );
void vcombine_c_DPD( complex_float R[], float pm RREAL[], float RIMAG[], 
                     int L );
void vcombine_c_DPP( complex_float R[], float pm RREAL[], float pm RIMAG[], 
                     int L );
void vcombine_c_PDD( complex_float pm R[], float RREAL[], float RIMAG[], 
                     int L );
void vcombine_c_PDP( complex_float pm R[], float RREAL[], float pm RIMAG[], 
                     int L );
void vcombine_c_PPD( complex_float pm R[], float pm RREAL[], float RIMAG[], 
                     int L );
void vcombine_c_PPP( complex_float pm R[], float pm RREAL[], float pm RIMAG[], 
                     int L );
void vcombine_sc_DDD( complex_float R[], int i, float RREAL[], int j, 
                      float RIMAG[], int k, int L );
void vcombine_sc_DDP( complex_float R[], int i, float RREAL[], int j, 
                      float pm RIMAG[], int k, int L );
void vcombine_sc_DPD( complex_float R[], int i, float pm RREAL[], int j,
                      float RIMAG[], int k, int L );
void vcombine_sc_DPP( complex_float R[], int i, float pm RREAL[], int j, 
                      float pm RIMAG[], int k, int L );
void vcombine_sc_PDD( complex_float pm R[], int i, float RREAL[], int j,
                      float RIMAG[], int k, int L );
void vcombine_sc_PDP( complex_float pm R[], int i, float RREAL[], int j,
                      float pm RIMAG[], int k, int L );
void vcombine_sc_PPD( complex_float pm R[], int i, float pm RREAL[], int j,
                      float RIMAG[], int k, int L );
void vcombine_sc_PPP( complex_float pm R[], int i, float pm RREAL[], int j,
                      float pm RIMAG[], int k, int L );

void vconj_c_DD( complex_float R[], complex_float X[], int L );
void vconj_c_DP( complex_float R[], complex_float pm X[], int L );
void vconj_c_PD( complex_float pm R[], complex_float X[], int L );
void vconj_c_PP( complex_float pm R[], complex_float pm X[], int L );
void vconj_sc_DD( complex_float R[], int i, complex_float X[], int j, int L );
void vconj_sc_DP( complex_float R[], int i, complex_float pm X[], int j, 
                  int L );
void vconj_sc_PD( complex_float pm R[], int i, complex_float X[], int j, 
                  int L );
void vconj_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j, 
                  int L );

void vcopy_DD( void *R, void *X, int L );
void vcopy_DP( void *R, void pm *X, int L );
void vcopy_PD( void pm *R, void *X, int L );
void vcopy_PP( void pm *R, void pm *X, int L );
void vcopy_s_DD( void *R, int i, void *X, int j, int L );
void vcopy_s_DP( void *R, int i, void pm *X, int j, int L );
void vcopy_s_PD( void pm *R, int i, void *X, int j, int L );
void vcopy_s_PP( void pm *R, int i, void pm *X, int j, int L );
void vcopy_c_DD( complex_float R[], complex_float X[], int L );
void vcopy_c_DP( complex_float R[], complex_float pm X[], int L );
void vcopy_c_PD( complex_float pm R[], complex_float X[], int L );
void vcopy_c_PP( complex_float pm R[], complex_float pm X[], int L );
void vcopy_sc_DD( complex_float R[], int i, complex_float X[], int j, int L );
void vcopy_sc_DP( complex_float R[], int i, complex_float pm X[], int j, 
                  int L );
void vcopy_sc_PD( complex_float pm R[], int i, complex_float X[], int j, 
                  int L );
void vcopy_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j, 
                  int L );

void vcos_f_DD( float R[], float X[], int L );
void vcos_f_DP( float R[], float pm X[], int L );
void vcos_f_PD( float pm R[], float X[], int L );
void vcos_f_PP( float pm R[], float pm X[], int L );
void vcos_sf_DD( float R[], int i, float X[], int j, int L );
void vcos_sf_DP( float R[], int i, float pm X[], int j, int L );
void vcos_sf_PD( float pm R[], int i, float X[], int j, int L );
void vcos_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vcosh_f_DD( float R[], float X[], int L );
void vcosh_f_DP( float R[], float pm X[], int L );
void vcosh_f_PD( float pm R[], float X[], int L );
void vcosh_f_PP( float pm R[], float pm X[], int L );
void vcosh_sf_DD( float R[], int i, float X[], int j, int L );
void vcosh_sf_DP( float R[], int i, float pm X[], int j, int L );
void vcosh_sf_PD( float pm R[], int i, float X[], int j, int L );
void vcosh_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vcube_f_DD( float R[], float X[], int L );
void vcube_f_DP( float R[], float pm X[], int L );
void vcube_f_PD( float pm R[], float X[], int L );
void vcube_f_PP( float pm R[], float pm X[], int L );
void vcube_sf_DD( float R[], int i, float X[], int j, int L );
void vcube_sf_DP( float R[], int i, float pm X[], int j, int L );
void vcube_sf_PD( float pm R[], int i, float X[], int j, int L );
void vcube_sf_PP( float pm R[], int i, float pm X[], int j, int L );
void vcube_i_DD( int R[], int X[], int L );
void vcube_i_DP( int R[], int pm X[], int L );
void vcube_i_PD( int pm R[], int X[], int L );
void vcube_i_PP( int pm R[], int pm X[], int L );
void vcube_si_DD( int R[], int i, int X[], int j, int L );
void vcube_si_DP( int R[], int i, int pm X[], int j, int L );
void vcube_si_PD( int pm R[], int i, int X[], int j, int L );
void vcube_si_PP( int pm R[], int i, int pm X[], int j, int L );
void vcube_c_DD( complex_float R[], complex_float X[], int L );
void vcube_c_DP( complex_float R[], complex_float pm X[], int L );
void vcube_c_PD( complex_float pm R[], complex_float X[], int L );
void vcube_c_PP( complex_float pm R[], complex_float pm X[], int L );
void vcube_sc_DD( complex_float R[], int i, complex_float X[], int j, int L );
void vcube_sc_DP( complex_float R[], int i, complex_float pm X[], int j,
                    int L );
void vcube_sc_PD( complex_float pm R[], int i, complex_float X[], int j,
                    int L );
void vcube_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j,
                    int L );

void vdba_f_DD( float R[], float X[], int L );
void vdba_f_DP( float R[], float pm X[], int L );
void vdba_f_PD( float pm R[], float X[], int L );
void vdba_f_PP( float pm R[], float pm X[], int L );
void vdba_sf_DD( float R[], int i, float X[], int j, int L );
void vdba_sf_DP( float R[], int i, float pm X[], int j, int L );
void vdba_sf_PD( float pm R[], int i, float X[], int j, int L );
void vdba_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vdbp_f_DD( float R[], float X[], int L );
void vdbp_f_DP( float R[], float pm X[], int L );
void vdbp_f_PD( float pm R[], float X[], int L );
void vdbp_f_PP( float pm R[], float pm X[], int L );
void vdbp_sf_DD( float R[], int i, float X[], int j, int L );
void vdbp_sf_DP( float R[], int i, float pm X[], int j, int L );
void vdbp_sf_PD( float pm R[], int i, float X[], int j, int L );
void vdbp_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vdiv_f_DDD( float R[], float X[], float Y[], int L );
void vdiv_f_DDP( float R[], float X[], float pm Y[], int L );
void vdiv_f_DPD( float R[], float pm X[], float Y[], int L );
void vdiv_f_PDP( float pm R[], float X[], float pm Y[], int L );
void vdiv_f_PPD( float pm R[], float pm X[], float Y[], int L );

void vdivq( float R[], float X[], float Y[], int L );
void vdivq_DDD( float R[], float X[], float Y[], int L );
void vdivq_DDP( float R[], float X[], float pm Y[], int L );
void vdivq_DPD( float R[], float pm X[], float Y[], int L );
void vdivq_PDP( float pm R[], float X[], float pm Y[], int L );
void vdivq_PPD( float pm R[], float pm X[], float Y[], int L );

void vdiv_c( complex_float R[], complex_float X[], complex_float Y[], int L );
void vdiv_c_DDD( complex_float R[], complex_float X[], complex_float Y[], int L );
void vdiv_c_DDP( complex_float R[], complex_float X[], complex_float pm Y[], int L );
void vdiv_c_DPD( complex_float R[], complex_float pm X[], complex_float Y[], int L );
void vdiv_c_PDP( complex_float pm R[], complex_float X[], complex_float pm Y[], int L );
void vdiv_c_PPD( complex_float pm R[], complex_float pm X[], complex_float Y[], int L );

void vdivq_c( complex_float R[], complex_float X[], complex_float Y[], int L );
void vdivq_c_DDD( complex_float R[], complex_float X[], complex_float Y[], int L );
void vdivq_c_DDP( complex_float R[], complex_float X[], complex_float pm Y[], int L );
void vdivq_c_DPD( complex_float R[], complex_float pm X[], complex_float Y[], int L );
void vdivq_c_PDP( complex_float pm R[], complex_float X[], complex_float pm Y[], int L );
void vdivq_c_PPD( complex_float pm R[], complex_float pm X[], complex_float Y[], int L );

float vdot_f_DD( float R[], float X[], int L );
float vdot_f_DP( float R[], float pm X[], int L );
float vdot_f_PD( float pm R[], float X[], int L );
float vdot_f_PP( float pm R[], float pm X[], int L );
float vdot_sf_DD( float R[], int i, float X[], int j, int L );
float vdot_sf_DP( float R[], int i, float pm X[], int j, int L );
float vdot_sf_PD( float pm R[], int i, float X[], int j, int L );
float vdot_sf_PP( float pm R[], int i, float pm X[], int j, int L );
int vdot_i_DD( int R[], int X[], int L );
int vdot_i_DP( int R[], int pm X[], int L );
int vdot_i_PD( int pm R[], int X[], int L );
int vdot_i_PP( int pm R[], int pm X[], int L );
int vdot_si_DD( int R[], int i, int X[], int j, int L );
int vdot_si_DP( int R[], int i, int pm X[], int j, int L );
int vdot_si_PD( int pm R[], int i, int X[], int j, int L );
int vdot_si_PP( int pm R[], int i, int pm X[], int j, int L );
complex_float vdot_c_DD( complex_float R[], complex_float X[], int L );
complex_float vdot_c_DP( complex_float R[], complex_float pm X[], int L );
complex_float vdot_c_PD( complex_float pm R[], complex_float X[], int L );
complex_float vdot_c_PP( complex_float pm R[], complex_float pm X[], int L );

complex_float vdot_c2_DD( float Xreal[], float Ximag[],
                          float Yreal[], float Yimag[], int L );

complex_float vdot_sc_DD( complex_float R[], int i, complex_float X[], int j,
                          int L );
complex_float vdot_sc_DP( complex_float R[], int i, complex_float pm X[], int j,
                          int L );
complex_float vdot_sc_PD( complex_float pm R[], int i, complex_float X[], int j,
                          int L );
complex_float vdot_sc_PP( complex_float pm R[], int i, complex_float pm X[], 
                          int j, int L );

int vequal_f_DD( float X[], float Y[], int L );
int vequal_f_DP( float X[], float pm Y[], int L );
int vequal_f_PP( float pm X[], float pm Y[], int L );
int vequal_sf_DD( float X[], int i, float Y[], int j, int L );
int vequal_sf_DP( float X[], int i, float pm Y[], int j, int L );
int vequal_sf_PP( float pm X[], int i, float pm Y[], int j, int L );

int vequal_i_DD( int X[], int Y[], int L );
int vequal_i_DP( int X[], int pm Y[], int L );
int vequal_i_PP( int pm X[], int pm Y[], int L );
int vequal_si_DD( int X[], int i, int Y[], int j, int L );
int vequal_si_DP( int X[], int i, int pm Y[], int j, int L );
int vequal_si_PP( int pm X[], int i, int pm Y[], int j, int L );

int vequal_c_DD( complex_float X[], complex_float Y[], int L );
int vequal_c_DP( complex_float X[], complex_float pm Y[], int L );
int vequal_c_PP( complex_float pm X[], complex_float pm Y[], int L );
int vequal_sc_DD( complex_float X[], int i, complex_float Y[], int j, int L );
int vequal_sc_DP( complex_float X[], int i, complex_float pm Y[], int j, 
                  int L );
int vequal_sc_PP( complex_float pm X[], int i, complex_float pm Y[], int j, 
                  int L );

void vexp_f_DD( float R[], float X[], int L );
void vexp_f_DP( float R[], float pm X[], int L );
void vexp_f_PD( float pm R[], float X[], int L );
void vexp_f_PP( float pm R[], float pm X[], int L );
void vexp_sf_DD( float R[], int i, float X[], int j, int L );
void vexp_sf_DP( float R[], int i, float pm X[], int j, int L );
void vexp_sf_PD( float pm R[], int i, float X[], int j, int L );
void vexp_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vexp_c( complex_float R[], complex_float X[], int L );
void vexp_c_DD( complex_float R[], complex_float X[], int L );
void vexp_c_DP( complex_float R[], complex_float pm X[], int L );
void vexp_c_PD( complex_float pm R[], complex_float X[], int L );
void vexp_c_PP( complex_float pm R[], complex_float pm X[], int L );

int vfirst_f_D( float X[], int L );
int vfirst_f_P( float pm X[], int L );
int vfirst_sf_D( float X[], int i, int L );
int vfirst_sf_P( float pm X[], int j, int L );
int vfirst_i_D( int X[], int L );
int vfirst_i_P( int pm X[], int L );
int vfirst_si_D( int X[], int i, int L );
int vfirst_si_P( int pm X[], int j, int L );

void vfix_DD( int R[], float X[], int S, int L );
void vfix_DP( int R[], float pm X[], int S, int L );
void vfix_PD( int pm R[], float X[], int S, int L );
void vfix_PP( int pm R[], float pm X[], int S, int L );
void vfix_s_DD( int R[], int i, float X[], int j, int S, int L );
void vfix_s_DP( int R[], int i, float pm X[], int j, int S, int L );
void vfix_s_PD( int pm R[], int i, float X[], int j, int S, int L );
void vfix_s_PP( int pm R[], int i, float pm X[], int j, int S, int L );

void vfloat_DD( float R[], int X[], int S, int L );
void vfloat_DP( float R[], int pm X[], int S, int L );
void vfloat_PD( float pm R[], int X[], int S, int L );
void vfloat_PP( float pm R[], int pm X[], int S, int L );
void vfloat_s_DD( float R[], int i, int X[], int j, int S, int L );
void vfloat_s_DP( float R[], int i, int pm X[], int j, int S, int L );
void vfloat_s_PD( float pm R[], int i, int X[], int j, int S, int L );
void vfloat_s_PP( float pm R[], int i, int pm X[], int j, int S, int L );

void vfloatmask_i_DD( float R[], unsigned int X[], unsigned int mask, int L );
void vfloatmask_i_DP( float R[], unsigned int pm X[], unsigned int mask, 
                      int L );
void vfloatmask_i_PD( float pm R[], unsigned int X[], unsigned int mask, 
                      int L );
void vfloatmask_i_PP( float pm R[], unsigned int pm X[], unsigned int mask, 
                      int L );
void vfloatmask_si_DD( float R[], int i, unsigned int X[], int j, 
                       unsigned int mask, int L );
void vfloatmask_si_DP( float R[], int i, unsigned int pm X[], int j, 
                       unsigned int mask, int L );
void vfloatmask_si_PD( float pm R[], int i, unsigned int X[], int j, 
                       unsigned int mask, int L );
void vfloatmask_si_PP( float pm R[], int i, unsigned int pm X[], int j, 
                       unsigned int mask, int L );

void vgather_f_DDD( float R[], float X[], int Y[], int L );
void vgather_f_DDP( float R[], float X[], int pm Y[], int L );
void vgather_f_DPD( float R[], float pm X[], int Y[], int L );
void vgather_f_PDP( float pm R[], float X[], int pm Y[], int L );
void vgather_sf_DDD( float R[], int i, float X[], int j, int Y[], int k, 
                     int L );
void vgather_sf_DDP( float R[], int i, float X[], int j, int pm Y[], int k,
                     int L );
void vgather_sf_DPD( float R[], int i, float pm X[], int j, int Y[], int k,
                     int L );
void vgather_sf_PDP( float pm R[], int i, float X[], int j, int pm Y[], int k,
                     int L );
void vgather_i_DDD( int R[], int X[], int Y[], int L );
void vgather_i_DDP( int R[], int X[], int pm Y[], int L );
void vgather_i_DPD( int R[], int pm X[], int Y[], int L );
void vgather_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vgather_si_DDD( int R[], int i, int X[], int j, int Y[], int k, 
                     int L );
void vgather_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                     int L );
void vgather_si_DPD( int R[], int i, int pm X[], int j, int Y[], int k,
                     int L );
void vgather_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                     int L );
void vgather_c_DDD( complex_float R[], complex_float X[], int Y[], int L );
void vgather_c_DDP( complex_float R[], complex_float X[], int pm Y[], int L );
void vgather_c_DPD( complex_float R[], complex_float pm X[], int Y[], int L );
void vgather_c_PDP( complex_float pm R[], complex_float X[], int pm Y[], 
                    int L );
void vgather_sc_DDD( complex_float R[], int i, complex_float X[], int j, 
                     int Y[], int k, int L );
void vgather_sc_DDP( complex_float R[], int i, complex_float X[], int j,
                     int pm Y[], int k, int L );
void vgather_sc_DPD( complex_float R[], int i, complex_float pm X[], int j, 
                     int Y[], int k, int L );
void vgather_sc_PDP( complex_float pm R[], int i, complex_float X[], int j, 
                     int pm Y[], int k, int L );

void vimag_c_DD( float R[], complex_float X[], int L );
void vimag_c_DP( float R[], complex_float pm X[], int L );
void vimag_c_PD( float pm R[], complex_float X[], int L );
void vimag_c_PP( float pm R[], complex_float pm X[], int L );
void vimag_sc_DD( float R[], int i, complex_float X[], int j, int L );
void vimag_sc_DP( float R[], int i, complex_float pm X[], int j, int L );
void vimag_sc_PD( float pm R[], int i, complex_float X[], int j, int L );
void vimag_sc_PP( float pm R[], int i, complex_float pm X[], int j, int L );

int vlast_f_D( float X[], int L );
int vlast_f_P( float pm X[], int L );
int vlast_sf_D( float X[], int i, int L );
int vlast_sf_P( float pm X[], int j, int L );
int vlast_i_D( int X[], int L );
int vlast_i_P( int pm X[], int L );
int vlast_si_D( int X[], int i, int L );
int vlast_si_P( int pm X[], int j, int L );

void vlimit_f_DD( float R[], float X[], float Min, float Max, int L );
void vlimit_f_DP( float R[], float pm X[], float Min, float Max, int L );
void vlimit_f_PD( float pm R[], float X[], float Min, float Max, int L );
void vlimit_f_PP( float pm R[], float pm X[], float Min, float Max, int L );
void vlimit_sf_DD( float R[], int i, float X[], int j, float Min, float Max, 
                   int L );
void vlimit_sf_DP( float R[], int i, float pm X[], int j, float Min, float Max,
                   int L );
void vlimit_sf_PD( float pm R[], int i, float X[], int j, float Min, float Max,
                   int L );
void vlimit_sf_PP( float pm R[], int i, float pm X[], int j, float Min, 
                   float Max, int L );
void vlimit_i_DD( int R[], int X[], int Min, int Max, int L );
void vlimit_i_DP( int R[], int pm X[], int Min, int Max, int L );
void vlimit_i_PD( int pm R[], int X[], int Min, int Max, int L );
void vlimit_i_PP( int pm R[], int pm X[], int Min, int Max, int L );
void vlimit_si_DD( int R[], int i, int X[], int j, int Min, int Max, int L );
void vlimit_si_DP( int R[], int i, int pm X[], int j, int Min, int Max, int L );
void vlimit_si_PD( int pm R[], int i, int X[], int j, int Min, int Max, int L );
void vlimit_si_PP( int pm R[], int i, int pm X[], int j, int Min, int Max, 
                   int L );
void vlimit_c_DD( complex_float R[], complex_float X[], complex_float Min, 
                  complex_float Max, int L );
void vlimit_c_DP( complex_float R[], complex_float pm X[], complex_float Min, 
                  complex_float Max, int L );
void vlimit_c_PD( complex_float pm R[], complex_float X[], complex_float Min, 
                  complex_float Max, int L );
void vlimit_c_PP( complex_float pm R[], complex_float pm X[], complex_float Min,
                  complex_float Max, int L );
void vlimit_sc_DD( complex_float R[], int i, complex_float X[], int j, 
                   complex_float Min, complex_float Max, int L );
void vlimit_sc_DP( complex_float R[], int i, complex_float pm X[], int j,
                   complex_float Min, complex_float Max, int L );
void vlimit_sc_PD( complex_float pm R[], int i, complex_float X[], int j,
                   complex_float Min, complex_float Max, int L );
void vlimit_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j,
                   complex_float Min, complex_float Max, int L );

void vlog_f_DD( float R[], float X[], int L );
void vlog_f_DP( float R[], float pm X[], int L );
void vlog_f_PD( float pm R[], float X[], int L );
void vlog_f_PP( float pm R[], float pm X[], int L );
void vlog_sf_DD( float R[], int i, float X[], int j, int L );
void vlog_sf_DP( float R[], int i, float pm X[], int j, int L );
void vlog_sf_PD( float pm R[], int i, float X[], int j, int L );
void vlog_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vlog10_f_DD( float R[], float X[], int L );
void vlog10_f_DP( float R[], float pm X[], int L );
void vlog10_f_PD( float pm R[], float X[], int L );
void vlog10_f_PP( float pm R[], float pm X[], int L );
void vlog10_sf_DD( float R[], int i, float X[], int j, int L );
void vlog10_sf_DP( float R[], int i, float pm X[], int j, int L );
void vlog10_sf_PD( float pm R[], int i, float X[], int j, int L );
void vlog10_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vlogb( int R[], float X[], int L );
void vlogb_DD( int R[], float X[], int L );
void vlogb_DP( int R[], float pm X[], int L );
void vlogb_PD( int pm R[], float X[], int L );
void vlogb_PP( int pm R[], float pm X[], int L );

void vmag2_f_DDD( float R[], float X[], float Y[], int L );
void vmag2_f_DDP( float R[], float X[], float pm Y[], int L );
void vmag2_f_PDD( float pm R[], float X[], float Y[], int L );
void vmag2_f_PDP( float pm R[], float X[], float pm Y[], int L );
void vmag2_sf_DDD( float R[], int i, float X[], int j, 
                   float Y[], int k, int L );
void vmag2_sf_DDP( float R[], int i, float X[], int j,
                   float pm Y[], int k, int L );
void vmag2_sf_PDD( float pm R[], int i, float X[], int j, 
                   float Y[], int k, int L );
void vmag2_sf_PDP( float pm R[], int i, float X[], int j, 
                   float pm Y[], int k, int L );
void vmag2_i_DDD( int R[], int X[], int Y[], int L );
void vmag2_i_DDP( int R[], int X[], int pm Y[], int L );
void vmag2_i_PDD( int pm R[], int X[], int Y[], int L );
void vmag2_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vmag2_si_DDD( int R[], int i, int X[], int j, 
                   int Y[], int k, int L );
void vmag2_si_DDP( int R[], int i, int X[], int j,
                   int pm Y[], int k, int L );
void vmag2_si_PDD( int pm R[], int i, int X[], int j, 
                   int Y[], int k, int L );
void vmag2_si_PDP( int pm R[], int i, int X[], int j, 
                   int pm Y[], int k, int L );
void vmag2_c_DD( float R[], complex_float X[], int L );
void vmag2_c_DP( float R[], complex_float pm X[], int L );
void vmag2_c_PD( float pm R[], complex_float X[], int L );
void vmag2_c_PP( float pm R[], complex_float pm X[], int L );
void vmag2_sc_DD( float R[], int i, complex_float X[], int j, int L );
void vmag2_sc_DP( float R[], int i, complex_float pm X[], int j, int L );
void vmag2_sc_PD( float pm R[], int i, complex_float X[], int j, int L );
void vmag2_sc_PP( float pm R[], int i, complex_float pm X[], int j, int L );

void vmant_f_DD( int R[], float X[], int L );
void vmant_f_DP( int R[], float pm X[], int L );
void vmant_f_PD( int pm R[], float X[], int L );
void vmant_f_PP( int pm R[], float pm X[], int L );
void vmant_sf_DD( int R[], int i, float X[], int j, int L );
void vmant_sf_DP( int R[], int i, float pm X[], int j, int L );
void vmant_sf_PD( int pm R[], int i, float X[], int j, int L );
void vmant_sf_PP( int pm R[], int i, float pm X[], int j, int L );

float vmax_f_D( float X[], int L );
float vmax_f_P( float pm X[], int L );
float vmax_sf_D( float X[], int i, int L );
float vmax_sf_P( float pm X[], int i, int L );
int vmax_i_D( int X[], int L );
int vmax_i_P( int pm X[], int L );
int vmax_si_D( int X[], int i, int L );
int vmax_si_P( int pm X[], int i, int L );

float vmaxabs_f_D( float X[], int L );
float vmaxabs_f_P( float pm X[], int L );
float vmaxabs_sf_D( float X[], int i, int L );
float vmaxabs_sf_P( float pm X[], int i, int L );
int vmaxabs_i_D( int X[], int L );
int vmaxabs_i_P( int pm X[], int L );
int vmaxabs_si_D( int X[], int i, int L );
int vmaxabs_si_P( int pm X[], int i, int L );

int vmaxindex_f_D( float X[], int L );
int vmaxindex_f_P( float pm X[], int L );
int vmaxindex_sf_D( float X[], int i, int L );
int vmaxindex_sf_P( float pm X[], int i, int L );
int vmaxindex_i_D( int X[], int L );
int vmaxindex_i_P( int pm X[], int L );
int vmaxindex_si_D( int X[], int i, int L );
int vmaxindex_si_P( int pm X[], int i, int L );

void vmerge( float R[], float X[], float Y[], int L1, int L2 );
void vmerge_DDD( float R[], float X[], float Y[], int L1, int L2 );
void vmerge_DDP( float R[], float X[], float pm Y[], int L1, int L2 );
void vmerge_DPD( float R[], float pm X[], float Y[], int L1, int L2 );
void vmerge_DPP( float R[], float pm X[], float pm Y[], int L1, int L2 );
void vmerge_PDD( float pm R[], float X[], float Y[], int L1, int L2 );
void vmerge_PDP( float pm R[], float X[], float pm Y[], int L1, int L2 );
void vmerge_PPD( float pm R[], float pm X[], float Y[], int L1, int L2 );
void vmerge_PPP( float pm R[], float pm X[], float pm Y[], int L1, int L2 );

float vmin_f_D( float X[], int L );
float vmin_f_P( float pm X[], int L );
int   vmin_i_D( int X[], int L );
int   vmin_i_P( int pm X[], int L );
float vmin_sf_D( float X[], int i, int L );
float vmin_sf_P( float pm X[], int i, int L );
int   vmin_si_D( int X[], int i, int L );
int   vmin_si_P( int pm X[], int i, int L );

float vminabs_f_D( float X[], int L );
float vminabs_f_P( float pm X[], int L );
float vminabs_sf_D( float X[], int i, int L );
float vminabs_sf_P( float pm X[], int i, int L );
int vminabs_i_D( int X[], int L );
int vminabs_i_P( int pm X[], int L );
int vminabs_si_D( int X[], int i, int L );
int vminabs_si_P( int pm X[], int i, int L );

int vminindex_f_D( float X[], int L );
int vminindex_f_P( float pm X[], int L );
int vminindex_sf_D( float X[], int i, int L );
int vminindex_sf_P( float pm X[], int i, int L );
int vminindex_i_D( int X[], int L );
int vminindex_i_P( int pm X[], int L );
int vminindex_si_D( int X[], int i, int L );
int vminindex_si_P( int pm X[], int i, int L );

void vmul_f_DDD( float R[], float X[], float Y[], int L );
void vmul_f_DDP( float R[], float X[], float pm Y[], int L );
void vmul_f_PDP( float pm R[], float X[], float pm Y[], int L );
void vmul_sf_DDD( float R[], int i, float X[], int j, float Y[], int k, int L );
void vmul_sf_DDP( float R[], int i, float X[], int j, float pm Y[], int k, 
                  int L );
void vmul_sf_PDP( float pm R[], int i, float X[], int j, float pm Y[], int k,
                  int L );

void vmul_i_DDD( int R[], int X[], int Y[], int L );
void vmul_i_DDP( int R[], int X[], int pm Y[], int L );
void vmul_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vmul_si_DDD( int R[], int i, int X[], int j, int Y[], int k, int L );
void vmul_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k, 
                  int L );
void vmul_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                  int L );

void vmul_c_DDD( complex_float R[], complex_float X[], complex_float Y[], 
                 int L );
void vmul_c_DDP( complex_float R[], complex_float X[], complex_float pm Y[], 
                 int L );
void vmul_c_PDP( complex_float pm R[], complex_float X[], complex_float pm Y[],
                 int L );
void vmul_sc_DDD( complex_float R[], int i, complex_float X[], int j,
                  complex_float Y[], int k, int L );
void vmul_sc_DDP( complex_float R[], int i, complex_float X[], int j, 
                  complex_float pm Y[], int k, int L );
void vmul_sc_PDP( complex_float pm R[], int i, complex_float X[], int j,
                  complex_float pm Y[], int k, int L );

void vmuladd_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vmuladd_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vmuladd_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vmuladd_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vmuladd_f_PDPD( float pm R[], float X[], float pm Y[], float Z[], int L );
void vmuladd_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vmuladd_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k,
                      float Z[], int m, int L );
void vmuladd_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmuladd_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vmuladd_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmuladd_sf_PDPD( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k,float Z[], int m, int L );
void vmuladd_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );

void vmuladd_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vmuladd_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vmuladd_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vmuladd_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vmuladd_i_PDPD( int pm R[], int X[], int pm Y[], int Z[], int L );
void vmuladd_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], 
                     int L );
void vmuladd_si_DDDD( int R[], int i, int X[], int j, int Y[], int k,
                      int Z[], int m, int L );
void vmuladd_si_DDDP( int R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmuladd_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vmuladd_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmuladd_si_PDPD( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k,int Z[], int m, int L );
void vmuladd_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k, int pm Z[], int m, int L );

void vmuladd_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vmuladd_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vmuladd_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vmuladd_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vmuladd_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vmuladd_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vmuladd_sc_DDDD( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float Z[], int m,
							 int L );
void vmuladd_sc_DDDP( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmuladd_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmuladd_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmuladd_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vmuladd_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );

void vmulmul_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vmulmul_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vmulmul_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vmulmul_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vmulmul_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vmulmul_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k,
                      float Z[], int m, int L );
void vmulmul_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmulmul_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vmulmul_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmulmul_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );
void vmulmul_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vmulmul_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vmulmul_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vmulmul_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vmulmul_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], 
                     int L );
void vmulmul_si_DDDD( int R[], int i, int X[], int j, int Y[], int k,
                      int Z[], int m, int L );
void vmulmul_si_DDDP( int R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmulmul_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vmulmul_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmulmul_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k, int pm Z[], int m, int L );
void vmulmul_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vmulmul_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vmulmul_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vmulmul_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vmulmul_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vmulmul_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vmulmul_sc_DDDD( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float Z[], int m, 
							 int L );
void vmulmul_sc_DDDP( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulmul_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulmul_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulmul_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vmulmul_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );

void vmulsub_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vmulsub_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vmulsub_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vmulsub_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vmulsub_f_PDPD( float pm R[], float X[], float pm Y[], float Z[], 
                     int L );
void vmulsub_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vmulsub_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k,
                      float Z[], int m, int L );
void vmulsub_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmulsub_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vmulsub_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmulsub_sf_PDPD( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float Z[], int m, int L );
void vmulsub_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );
void vmulsub_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vmulsub_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vmulsub_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vmulsub_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vmulsub_i_PDPD( int pm R[], int X[], int pm Y[], int Z[], 
                     int L );
void vmulsub_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], 
                     int L );
void vmulsub_si_DDDD( int R[], int i, int X[], int j, int Y[], int k,
                      int Z[], int m, int L );
void vmulsub_si_DDDP( int R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmulsub_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vmulsub_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmulsub_si_PDPD( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k, int Z[], int m, int L );
void vmulsub_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k, int pm Z[], int m, int L );
void vmulsub_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vmulsub_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vmulsub_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vmulsub_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vmulsub_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vmulsub_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vmulsub_sc_DDDD( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float Z[], int m, 
							 int L );
void vmulsub_sc_DDDP( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulsub_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulsub_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulsub_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vmulsub_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );

void vmulsubneg_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vmulsubneg_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vmulsubneg_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vmulsubneg_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vmulsubneg_f_PDPD( float pm R[], float X[], float pm Y[], float Z[], 
                     int L );
void vmulsubneg_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vmulsubneg_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k,
                      float Z[], int m, int L );
void vmulsubneg_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmulsubneg_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vmulsubneg_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vmulsubneg_sf_PDPD( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float Z[], int m, int L );
void vmulsubneg_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );
void vmulsubneg_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vmulsubneg_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vmulsubneg_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vmulsubneg_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vmulsubneg_i_PDPD( int pm R[], int X[], int pm Y[], int Z[], 
                     int L );
void vmulsubneg_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], 
                     int L );
void vmulsubneg_si_DDDD( int R[], int i, int X[], int j, int Y[], int k,
                      int Z[], int m, int L );
void vmulsubneg_si_DDDP( int R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmulsubneg_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vmulsubneg_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vmulsubneg_si_PDPD( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k, int Z[], int m, int L );
void vmulsubneg_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], 
                      int k, int pm Z[], int m, int L );
void vmulsubneg_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vmulsubneg_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vmulsubneg_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vmulsubneg_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vmulsubneg_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vmulsubneg_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vmulsubneg_sc_DDDD( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float Z[], int m, 
							 int L );
void vmulsubneg_sc_DDDP( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulsubneg_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulsubneg_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vmulsubneg_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vmulsubneg_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );


void vneg_f_DD( float R[], float X[], int L );
void vneg_f_DP( float R[], float pm X[], int L );
void vneg_f_PD( float pm R[], float X[], int L );
void vneg_f_PP( float pm R[], float pm X[], int L );
void vneg_sf_DD( float R[], int i, float X[], int j, int L );
void vneg_sf_DP( float R[], int i, float pm X[], int j, int L );
void vneg_sf_PD( float pm R[], int i, float X[], int j, int L );
void vneg_sf_PP( float pm R[], int i, float pm X[], int j, int L );
void vneg_i_DD( int R[], int X[], int L );
void vneg_i_DP( int R[], int pm X[], int L );
void vneg_i_PD( int pm R[], int X[], int L );
void vneg_i_PP( int pm R[], int pm X[], int L );
void vneg_si_DD( int R[], int i, int X[], int j, int L );
void vneg_si_DP( int R[], int i, int pm X[], int j, int L );
void vneg_si_PD( int pm R[], int i, int X[], int j, int L );
void vneg_si_PP( int pm R[], int i, int pm X[], int j, int L );
void vneg_c_DD( complex_float R[], complex_float X[], int L );
void vneg_c_DP( complex_float R[], complex_float pm X[], int L );
void vneg_c_PD( complex_float pm R[], complex_float X[], int L );
void vneg_c_PP( complex_float pm R[], complex_float pm X[], int L );
void vneg_sc_DD( complex_float R[], int i, complex_float X[], int j, int L );
void vneg_sc_DP( complex_float R[], int i, complex_float pm X[], int j, int L );
void vneg_sc_PD( complex_float pm R[], int i, complex_float X[], int j, int L );
void vneg_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j,
                 int L );

int vnequal_f_DD( float X[], float Y[], int L );
int vnequal_f_DP( float X[], float pm Y[], int L );
#define vnequal_f_PD( x, y, z )   vnequal_f_DP( y, x, z )
int vnequal_f_PP( float pm X[], float pm Y[], int L );
int vnequal_sf_DD( float X[], int i, float Y[], int j, int L );
int vnequal_sf_DP( float X[], int i, float pm Y[], int j, int L );
#define vnequal_sf_PD( x, i, y, j, z )   vnequal_sf_DP( y, j, x, i, z )
int vnequal_sf_PP( float pm X[], int i, float pm Y[], int j, int L );
int vnequal_i_DD( int X[], int Y[], int L );
int vnequal_i_DP( int X[], int pm Y[], int L );
int vnequal_i_PP( int pm X[], int pm Y[], int L );
int vnequal_si_DD( int X[], int i, int Y[], int j, int L );
int vnequal_si_DP( int X[], int i, int pm Y[], int j, int L );
#define vnequal_si_PD( x, i, y, j, z )   vnequal_si_DP( y, j, x, i, z )
int vnequal_si_PP( int pm X[], int i, int pm Y[], int j, int L );
int vnequal_c_DD( complex_float X[], complex_float Y[], int L );
int vnequal_c_DP( complex_float X[], complex_float pm Y[], int L );
int vnequal_c_PP( complex_float pm X[], complex_float pm Y[], int L );
int vnequal_sc_DD( complex_float X[], int i, complex_float Y[], int j, int L );
int vnequal_sc_DP( complex_float X[], int i, complex_float pm Y[], int j, 
                   int L );
#define vnequal_sc_PD( x, i, y, j, l )  vnequal_sc_DP( y, j, x, i, l )
int vnequal_sc_PP( complex_float pm X[], int i, complex_float pm Y[], int j, 
                   int L );

void vor_i_DDD( int R[], int X[], int Y[], int L );
void vor_i_DDP( int R[], int X[], int pm Y[], int L );
void vor_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vor_si_DDD( int R[], int i, int X[], int j, int Y[], int k, int L );
void vor_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                  int L );
void vor_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                  int L );

void vpolrect_f( float X[], float Y[], float MAG[], float ANGLE[], int L );

void vpdb_f_DD( float R[], float X[], int L );
void vpdb_f_DP( float R[], float pm X[], int L );
void vpdb_f_PD( float pm R[], float X[], int L );
void vpdb_f_PP( float pm R[], float pm X[], int L );
void vpdb_sf_DD( float R[], int i, float X[], int j, int L );
void vpdb_sf_DP( float R[], int i, float pm X[], int j, int L );
void vpdb_sf_PD( float pm R[], int i, float X[], int j, int L );
void vpdb_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vspow_f_DD( float R[], float X[], float y, int L );
void vspow_f_DP( float R[], float pm X[], float y, int L );
void vspow_f_PD( float pm R[], float X[], float y, int L );
void vspow_f_PP( float pm R[], float pm X[], float y, int L );
void vspow_sf_DD( float R[], int i, float X[], int j, float y, int L );
void vspow_sf_DP( float R[], int i, float pm X[], int j, float y, int L );
void vspow_sf_PD( float pm R[], int i, float X[], int j, float y, int L );
void vspow_sf_PP( float pm R[], int i, float pm X[], int j, float y, int L );

void vramp_f_D( float R[], float Start, float Inc, int L );
void vramp_f_P( float pm R[], float Start, float Inc, int L );
void vramp_c_D( complex_float R[], complex_float Start, complex_float Inc, int L );
void vramp_c_P( complex_float pm R[], complex_float Start, complex_float Inc, int L );
void vramp_i_D( int R[], int Start, int Inc, int L );
void vramp_i_P( int pm R[], int Start, int Inc, int L );
void vramp_sf_D( float R[], int i, float Start, float Inc, int L );
void vramp_sf_P( float pm R[], int i, float Start, float Inc, int L );
void vramp_sc_D( complex_float R[], int i, complex_float Start, complex_float Inc, int L );
void vramp_sc_P( complex_float pm R[], int i, complex_float Start, complex_float Inc, int L );
void vramp_si_D( int [], int i, int Start, int Inc, int L );
void vramp_si_P( int pm R[], int i, int Start, int Inc, int L );

void vrangauss_f_D( float R[], float mean, float std_dev, int L );
void vrangauss_f_P( float pm R[], float mean, float std_dev, int L );

void vranuniform_f_D( float R[], float mean, float std_dev, int L );
void vranuniform_f_P( float pm R[], float mean, float std_dev, int L );

void vreal_c_DD( float R[], complex_float X[], int L );
void vreal_c_DP( float R[], complex_float pm X[], int L );
void vreal_c_PD( float pm R[], complex_float X[], int L );
void vreal_c_PP( float pm R[], complex_float pm X[], int L );
void vreal_sc_DD( float R[], int i, complex_float X[], int j, int L );
void vreal_sc_DP( float R[], int i, complex_float pm X[], int j, int L );
void vreal_sc_PD( float pm R[], int i, complex_float X[], int j, int L );
void vreal_sc_PP( float pm R[], int i, complex_float pm X[], int j, int L );

void vrectpol_f( float MAG[], float ANGLE[], float X[], float Y[], int L );

void vreverse_f_DD( float R[], float X[], int L );
void vreverse_f_DP( float R[], float pm X[], int L );
void vreverse_f_PD( float pm R[], float X[], int L );
void vreverse_f_PP( float pm R[], float pm X[], int L );
void vreverse_sf_DD( float R[], int i, float X[], int j, int L );
void vreverse_sf_DP( float R[], int i, float pm X[], int j, int L );
void vreverse_sf_PD( float pm R[], int i, float X[], int j, int L );
void vreverse_sf_PP( float pm R[], int i, float pm X[], int j, int L );
void vreverse_i_DD( int R[], int X[], int L );
void vreverse_i_DP( int R[], int pm X[], int L );
void vreverse_i_PD( int pm R[], int X[], int L );
void vreverse_i_PP( int pm R[], int pm X[], int L );
void vreverse_si_DD( int R[], int i, int X[], int j, int L );
void vreverse_si_DP( int R[], int i, int pm X[], int j, int L );
void vreverse_si_PD( int pm R[], int i, int X[], int j, int L );
void vreverse_si_PP( int pm R[], int i, int pm X[], int j, int L );
void vreverse_c_DD( complex_float R[], complex_float X[], int L );
void vreverse_c_DP( complex_float R[], complex_float pm X[], int L );
void vreverse_c_PD( complex_float pm R[], complex_float X[], int L );
void vreverse_c_PP( complex_float pm R[], complex_float pm X[], int L );
void vreverse_sc_DD( complex_float R[], int i, complex_float X[], int j, 
                     int L );
void vreverse_sc_DP( complex_float R[], int i, complex_float pm X[], int j,
                     int L );
void vreverse_sc_PD( complex_float pm R[], int i, complex_float X[], int j,
                     int L );
void vreverse_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j,
                     int L );

void vround_f_DD( float R[], float X[], int L );
void vround_f_DP( float R[], float pm X[], int L );
void vround_f_PD( float pm R[], float X[], int L );
void vround_f_PP( float pm R[], float pm X[], int L );
void vround_sf_PP( float pm R[], int i, float pm X[], int j, int L );
void vround_sf_DD( float R[], int i, float X[], int j, int L );
void vround_sf_DP( float R[], int i, float pm X[], int j, int L );
void vround_sf_PD( float pm R[], int i, float X[], int j, int L );

void vsadd_f_DD( float R[], float S, float X[], int L );
void vsadd_f_DP( float R[], float S, float pm X[], int L );
void vsadd_f_PD( float pm R[], float S, float X[], int L );
void vsadd_f_PP( float pm R[], float S, float pm X[], int L );
void vsadd_sf_DD( float R[], int i, float S, float X[], int j, int L );
void vsadd_sf_DP( float R[], int i, float S, float pm X[], int j, int L );
void vsadd_sf_PD( float pm R[], int i, float S, float X[], int j, int L );
void vsadd_sf_PP( float pm R[], int i, float S, float pm X[], int j, int L );
void vsadd_i_DD( int R[], int S, int X[], int L );
void vsadd_i_DP( int R[], int S, int pm X[], int L );
void vsadd_i_PD( int pm R[], int S, int X[], int L );
void vsadd_i_PP( int pm R[], int S, int pm X[], int L );
void vsadd_si_DD( int R[], int i, int S, int X[], int j, int L );
void vsadd_si_DP( int R[], int i, int S, int pm X[], int j, int L );
void vsadd_si_PD( int pm R[], int i, int S, int X[], int j, int L );
void vsadd_si_PP( int pm R[], int i, int S, int pm X[], int j, int L );
void vsadd_c_DD( complex_float R[], complex_float S, complex_float X[], int L );
void vsadd_c_DP( complex_float R[], complex_float S, complex_float pm X[], 
                 int L );
void vsadd_c_PD( complex_float pm R[], complex_float S, complex_float X[], 
                 int L );
void vsadd_c_PP( complex_float pm R[], complex_float S, complex_float pm X[], 
                 int L );
void vsadd_sc_DD( complex_float R[], int i, complex_float S, complex_float X[],
                  int j, int L );
void vsadd_sc_DP( complex_float R[], int i, complex_float S, 
                  complex_float pm X[], int j, int L );
void vsadd_sc_PD( complex_float pm R[], int i, complex_float S, 
                  complex_float X[], int j, int L );
void vsadd_sc_PP( complex_float pm R[], int i, complex_float S, 
                  complex_float pm X[], int j, int L );

void vsaddmul_f_DD( float R[], float S1, float X[], float S2, int L );
void vsaddmul_f_DP( float R[], float S1, float pm X[], float S2, int L );
void vsaddmul_f_PD( float pm R[], float S1, float X[], float S2, int L );
void vsaddmul_f_PP( float pm R[], float S1, float pm X[], float S2, int L );
void vsaddmul_sf_DD( float R[], int i, float S1, float X[], int j, float S2, 
                     int L );
void vsaddmul_sf_DP( float R[], int i, float S1, float pm X[], int j, float S2,
                     int L );
void vsaddmul_sf_PD( float pm R[], int i, float S1, float X[], int j, float S2,
                     int L );
void vsaddmul_sf_PP( float pm R[], int i, float S1, float pm X[], int j, 
                     float S2, int L );
void vsaddmul_i_DD( int R[], int S1, int X[], int S2, int L );
void vsaddmul_i_DP( int R[], int S1, int pm X[], int S2, int L );
void vsaddmul_i_PD( int pm R[], int S1, int X[], int S2, int L );
void vsaddmul_i_PP( int pm R[], int S1, int pm X[], int S2, int L );
void vsaddmul_si_DD( int R[], int i, int S1, int X[], int j, int S2, int L );
void vsaddmul_si_DP( int R[], int i, int S1, int pm X[], int j, int S2, int L );
void vsaddmul_si_PD( int pm R[], int i, int S1, int X[], int j, int S2, int L );
void vsaddmul_si_PP( int pm R[], int i, int S1, int pm X[], int j, int S2, 
                     int L );
void vsaddmul_c_DD( complex_float R[], complex_float S1, complex_float X[], 
                    complex_float S2, int L );
void vsaddmul_c_DP( complex_float R[], complex_float S1, complex_float pm X[],
                    complex_float S2, int L );
void vsaddmul_c_PD( complex_float pm R[], complex_float S1, complex_float X[], 
                    complex_float S2, int L );
void vsaddmul_c_PP( complex_float pm R[], complex_float S1, 
                    complex_float pm X[], complex_float S2, int L );
void vsaddmul_sc_DD( complex_float R[], int i, complex_float S1, 
                     complex_float X[], int j, complex_float S2, int L );
void vsaddmul_sc_DP( complex_float R[], int i, complex_float S1, 
                     complex_float pm X[], int j, complex_float S2, int L );
void vsaddmul_sc_PD( complex_float pm R[], int i, complex_float S1, 
                     complex_float X[], int j, complex_float S2, int L );
void vsaddmul_sc_PP( complex_float pm R[], int i, complex_float S1, 
                     complex_float pm X[], int j, complex_float S2, int L );


void vscale_f_DD( float R[], float X[], int S, int L );
void vscale_f_DP( float R[], float pm X[], int S, int L );
void vscale_f_PD( float pm R[], float X[], int S, int L );
void vscale_f_PP( float pm R[], float pm X[], int S, int L );
void vscale_sf_DD( float R[], int i, float X[], int j, int S, int L );
void vscale_sf_DP( float R[], int i, float pm X[], int j, int S, int L );
void vscale_sf_PD( float pm R[], int i, float X[], int j, int S, int L );
void vscale_sf_PP( float pm R[], int i, float pm X[], int j, int S, int L );

void vscatter_f_DDD( float R[], float X[], int Y[], int L );
void vscatter_f_DDP( float R[], float X[], int pm Y[], int L );
void vscatter_f_DPD( float R[], float pm X[], int Y[], int L );
void vscatter_f_PDP( float pm R[], float X[], int pm Y[], int L );
void vscatter_sf_DDD( float R[], int i, float X[], int j, int Y[], int k, 
                     int L );
void vscatter_sf_DDP( float R[], int i, float X[], int j, int pm Y[], int k,
                     int L );
void vscatter_sf_DPD( float R[], int i, float pm X[], int j, int Y[], int k,
                     int L );
void vscatter_sf_PDP( float pm R[], int i, float X[], int j, int pm Y[], int k,
                     int L );
void vscatter_i_DDD( int R[], int X[], int Y[], int L );
void vscatter_i_DDP( int R[], int X[], int pm Y[], int L );
void vscatter_i_DPD( int R[], int pm X[], int Y[], int L );
void vscatter_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vscatter_si_DDD( int R[], int i, int X[], int j, int Y[], int k, 
                     int L );
void vscatter_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                     int L );
void vscatter_si_DPD( int R[], int i, int pm X[], int j, int Y[], int k,
                     int L );
void vscatter_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                     int L );
void vscatter_c_DDD( complex_float R[], complex_float X[], int Y[], int L );
void vscatter_c_DDP( complex_float R[], complex_float X[], int pm Y[], int L );
void vscatter_c_DPD( complex_float R[], complex_float pm X[], int Y[], int L );
void vscatter_c_PDP( complex_float pm R[], complex_float X[], int pm Y[], 
                    int L );
void vscatter_sc_DDD( complex_float R[], int i, complex_float X[], int j, 
                     int Y[], int k, int L );
void vscatter_sc_DDP( complex_float R[], int i, complex_float X[], int j,
                     int pm Y[], int k, int L );
void vscatter_sc_DPD( complex_float R[], int i, complex_float pm X[], int j, 
                     int Y[], int k, int L );
void vscatter_sc_PDP( complex_float pm R[], int i, complex_float X[], int j, 
                     int pm Y[], int k, int L );

void  vset_f_D( float R[], float S, int L );
void  vset_f_P( float pm R[], float S, int L );
void  vset_sf_D( float R[], int i, float S, int L );
void  vset_sf_P( float pm R[], int i, float S, int L );

void  vset_i_D( int R[], int S, int L );
void  vset_i_P( int pm R[], int S, int L );
void  vset_si_D( int R[], int i, int S, int L );
void  vset_si_P( int pm R[], int i, int S, int L );

void  vset_c_D( complex_float R[], complex_float S, int L );
void  vset_c_P( complex_float pm R[], complex_float S, int L );
void  vset_sc_D( complex_float R[], int i, complex_float S, int L );
void  vset_sc_P( complex_float pm R[], int i, complex_float S, int L );

void vshrink_f_DD( float R[], float X[], int L );
void vshrink_f_DP( float R[], float pm X[], int L );
void vshrink_f_PD( float pm R[], float X[], int L );
void vshrink_f_PP( float pm R[], float pm X[], int L );
void vshrink_sf_DD( float R[], int i, float X[], int j, int L );
void vshrink_sf_DP( float R[], int i, float pm X[], int j, int L );
void vshrink_sf_PD( float pm R[], int i, float X[], int j, int L );
void vshrink_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vshrink_i_DD( int R[], int X[], int L );
void vshrink_i_DP( int R[], int pm X[], int L );
void vshrink_i_PD( int pm R[], int X[], int L );
void vshrink_i_PP( int pm R[], int pm X[], int L );
void vshrink_si_DD( int R[], int i, int X[], int j, int L );
void vshrink_si_DP( int R[], int i, int pm X[], int j, int L );
void vshrink_si_PD( int pm R[], int i, int X[], int j, int L );
void vshrink_si_PP( int pm R[], int i, int pm X[], int j, int L );

void vsin_f_DD( float R[], float X[], int L );
void vsin_f_DP( float R[], float pm X[], int L );
void vsin_f_PD( float pm R[], float X[], int L );
void vsin_f_PP( float pm R[], float pm X[], int L );
void vsin_sf_DD( float R[], int i, float X[], int j, int L );
void vsin_sf_DP( float R[], int i, float pm X[], int j, int L );
void vsin_sf_PD( float pm R[], int i, float X[], int j, int L );
void vsin_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vsinh_f_DD( float R[], float X[], int L );
void vsinh_f_DP( float R[], float pm X[], int L );
void vsinh_f_PD( float pm R[], float X[], int L );
void vsinh_f_PP( float pm R[], float pm X[], int L );
void vsinh_sf_DD( float R[], int i, float X[], int j, int L );
void vsinh_sf_DP( float R[], int i, float pm X[], int j, int L );
void vsinh_sf_PD( float pm R[], int i, float X[], int j, int L );
void vsinh_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vsmul_f_DD( float R[], float S, float X[], int L );
void vsmul_f_DP( float R[], float S, float pm X[], int L );
void vsmul_f_PD( float pm R[], float S, float X[], int L );
void vsmul_f_PP( float pm R[], float S, float pm X[], int L );
void vsmul_sf_DD( float R[], int i, float S, float X[], int j, int L );
void vsmul_sf_DP( float R[], int i, float S, float pm X[], int j, int L );
void vsmul_sf_PD( float pm R[], int i, float S, float X[], int j, int L );
void vsmul_sf_PP( float pm R[], int i, float S, float pm X[], int j, int L );
void vsmul_i_DD( int R[], int S, int X[], int L );
void vsmul_i_DP( int R[], int S, int pm X[], int L );
void vsmul_i_PD( int pm R[], int S, int X[], int L );
void vsmul_i_PP( int pm R[], int S, int pm X[], int L );
void vsmul_si_DD( int R[], int i, int S, int X[], int j, int L );
void vsmul_si_DP( int R[], int i, int S, int pm X[], int j, int L );
void vsmul_si_PD( int pm R[], int i, int S, int X[], int j, int L );
void vsmul_si_PP( int pm R[], int i, int S, int pm X[], int j, int L );
void vsmul_c_DD( complex_float R[], complex_float S, complex_float X[], int L );
void vsmul_c_DP( complex_float R[], complex_float S, complex_float pm X[], 
                 int L );
void vsmul_c_PD( complex_float pm R[], complex_float S, complex_float X[], 
                 int L );
void vsmul_c_PP( complex_float pm R[], complex_float S, complex_float pm X[], 
                 int L );
void vsmul_sc_DD( complex_float R[], int i, complex_float S, complex_float X[],
                  int j, int L );
void vsmul_sc_DP( complex_float R[], int i, complex_float S, 
                  complex_float pm X[], int j, int L );
void vsmul_sc_PD( complex_float pm R[], int i, complex_float S, 
                  complex_float X[], int j, int L );
void vsmul_sc_PP( complex_float pm R[], int i, complex_float S, 
                  complex_float pm X[], int j, int L );

void vsmuladd_f_DD( float R[], float S1, float X[], float S2, int L );
void vsmuladd_f_DP( float R[], float S1, float pm X[], float S2, int L );
void vsmuladd_f_PD( float pm R[], float S1, float X[], float S2, int L );
void vsmuladd_f_PP( float pm R[], float S1, float pm X[], float S2, int L );
void vsmuladd_sf_DD( float R[], int i, float S1, float X[], int j, float S2, 
                     int L );
void vsmuladd_sf_DP( float R[], int i, float S1, float pm X[], int j, float S2,
                     int L );
void vsmuladd_sf_PD( float pm R[], int i, float S1, float X[], int j, float S2,
                     int L );
void vsmuladd_sf_PP( float pm R[], int i, float S1, float pm X[], int j, 
                     float S2, int L );
void vsmuladd_i_DD( int R[], int S1, int X[], int S2, int L );
void vsmuladd_i_DP( int R[], int S1, int pm X[], int S2, int L );
void vsmuladd_i_PD( int pm R[], int S1, int X[], int S2, int L );
void vsmuladd_i_PP( int pm R[], int S1, int pm X[], int S2, int L );
void vsmuladd_si_DD( int R[], int i, int S1, int X[], int j, int S2, int L );
void vsmuladd_si_DP( int R[], int i, int S1, int pm X[], int j, int S2, int L );
void vsmuladd_si_PD( int pm R[], int i, int S1, int X[], int j, int S2, int L );
void vsmuladd_si_PP( int pm R[], int i, int S1, int pm X[], int j, int S2, 
                     int L );
void vsmuladd_c_DD( complex_float R[], complex_float S1, complex_float X[], 
                    complex_float S2, int L );
void vsmuladd_c_DP( complex_float R[], complex_float S1, complex_float pm X[],
                    complex_float S2, int L );
void vsmuladd_c_PD( complex_float pm R[], complex_float S1, complex_float X[], 
                    complex_float S2, int L );
void vsmuladd_c_PP( complex_float pm R[], complex_float S1, 
                    complex_float pm X[], complex_float S2, int L );
void vsmuladd_sc_DD( complex_float R[], int i, complex_float S1, 
                     complex_float X[], int j, complex_float S2, int L );
void vsmuladd_sc_DP( complex_float R[], int i, complex_float S1, 
                     complex_float pm X[], int j, complex_float S2, int L );
void vsmuladd_sc_PD( complex_float pm R[], int i, complex_float S1, 
                     complex_float X[], int j, complex_float S2, int L );
void vsmuladd_sc_PP( complex_float pm R[], int i, complex_float S1, 
                     complex_float pm X[], int j, complex_float S2, int L );

void vsmulvadd_f_DDD( float R[], float s, float X[], float Y[], int L );
void vsmulvadd_f_DDP( float R[], float s, float X[], float pm Y[], int L );
void vsmulvadd_f_DPD( float R[], float s, float pm X[], float Y[], int L );
void vsmulvadd_f_PDP( float pm R[], float s, float X[], float pm Y[], int L );
void vsmulvadd_f_PPD( float pm R[], float s, float pm X[], float Y[], int L );
void vsmulvadd_sf_DDD( float R[], int i, float s, float X[], int j, float Y[], 
                       int k, int L );
void vsmulvadd_sf_DDP( float R[], int i, float s, float X[], int j, 
                       float pm Y[], int k, int L );
void vsmulvadd_sf_DPD( float R[], int i, float s, float pm X[], int j, 
                       float Y[], int k, int L );
void vsmulvadd_sf_PDP( float pm R[], int i, float s, float X[], int j, 
                       float pm Y[], int k, int L );
void vsmulvadd_sf_PPD( float pm R[], int i, float s, float pm X[], int j,
                       float Y[], int k, int L );

void vsmulvsub_f_DDD( float R[], float s, float X[], float Y[], int L );
void vsmulvsub_f_DDP( float R[], float s, float X[], float pm Y[], int L );
void vsmulvsub_f_DPD( float R[], float s, float pm X[], float Y[], int L );
void vsmulvsub_f_PDP( float pm R[], float s, float X[], float pm Y[], int L );
void vsmulvsub_f_PPD( float pm R[], float s, float pm X[], float Y[], int L );
void vsmulvsub_sf_DDD( float R[], int i, float s, float X[], int j, float Y[], 
                       int k, int L );
void vsmulvsub_sf_DDP( float R[], int i, float s, float X[], int j, 
                       float pm Y[], int k, int L );
void vsmulvsub_sf_DPD( float R[], int i, float s, float pm X[], int j, 
                       float Y[], int k, int L );
void vsmulvsub_sf_PDP( float pm R[], int i, float s, float X[], int j, 
                       float pm Y[], int k, int L );
void vsmulvsub_sf_PPD( float pm R[], int i, float s, float pm X[], int j,
                       float Y[], int k, int L );

void vsmul_c( complex_float R[], complex_float S, complex_float X[], int L );
void vsmul_c_DD( complex_float R[], complex_float S, complex_float X[], int L );
void vsmul_c_DP( complex_float R[], complex_float S, complex_float pm X[], int L );
void vsmul_c_PD( complex_float pm R[], complex_float S, complex_float X[], int L );
void vsmul_c_PP( complex_float pm R[], complex_float S, complex_float pm X[], int L );

void vsplit_c_DDD( float RREAL[], float RIMAG[], complex_float X[], int L );
void vsplit_c_DDP( float RREAL[], float RIMAG[], complex_float pm X[], int L );
void vsplit_c_DPD( float RREAL[], float pm RIMAG[], complex_float X[], int L );
void vsplit_c_DPP( float RREAL[], float pm RIMAG[], complex_float pm X[], int L );
void vsplit_c_PDD( float pm RREAL[], float RIMAG[], complex_float X[], int L );
void vsplit_c_PDP( float pm RREAL[], float RIMAG[], complex_float pm X[], int L );
void vsplit_c_PPD( float pm RREAL[], float pm RIMAG[], complex_float X[], int L );
void vsplit_c_PPP( float pm RREAL[], float pm RIMAG[], complex_float pm X[], int L );
void vsplit_sc_DDD( float RREAL[], int i, float RIMAG[], int j, complex_float X[], int k, int L );
void vsplit_sc_DDP( float RREAL[], int i, float RIMAG[], int j, complex_float pm X[], int k, int L );
void vsplit_sc_DPD( float RREAL[], int i, float pm RIMAG[], int j, complex_float X[], int k, int L );
void vsplit_sc_DPP( float RREAL[], int i, float pm RIMAG[], int j, complex_float pm X[], int k, int L );
void vsplit_sc_PDD( float pm RREAL[], int i, float RIMAG[], int j, complex_float X[], int k, int L );
void vsplit_sc_PDP( float pm RREAL[], int i, float RIMAG[], int j, complex_float pm X[], int k, int L );
void vsplit_sc_PPD( float pm RREAL[], int i, float pm RIMAG[], int j, complex_float X[], int k, int L );
void vsplit_sc_PPP( float pm RREAL[], int i, float pm RIMAG[], int j, complex_float pm X[], int k, int L );

void vsqrt_f_DD( float R[], float X[], int L );
void vsqrt_f_DP( float R[], float pm X[], int L );
void vsqrt_f_PD( float pm R[], float X[], int L );
void vsqrt_f_PP( float pm R[], float pm X[], int L );
void vsqrt_sf_DD( float R[], int i, float X[], int j, int L );
void vsqrt_sf_DP( float R[], int i, float pm X[], int j, int L );
void vsqrt_sf_PD( float pm R[], int i, float X[], int j, int L );
void vsqrt_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vsquare_f_DD( float R[], float X[], int L );
void vsquare_f_DP( float R[], float pm X[], int L );
void vsquare_f_PD( float pm R[], float X[], int L );
void vsquare_f_PP( float pm R[], float pm X[], int L );
void vsquare_sf_DD( float R[], int i, float X[], int j, int L );
void vsquare_sf_DP( float R[], int i, float pm X[], int j, int L );
void vsquare_sf_PD( float pm R[], int i, float X[], int j, int L );
void vsquare_sf_PP( float pm R[], int i, float pm X[], int j, int L );
void vsquare_i_DD( int R[], int X[], int L );
void vsquare_i_DP( int R[], int pm X[], int L );
void vsquare_i_PD( int pm R[], int X[], int L );
void vsquare_i_PP( int pm R[], int pm X[], int L );
void vsquare_si_DD( int R[], int i, int X[], int j, int L );
void vsquare_si_DP( int R[], int i, int pm X[], int j, int L );
void vsquare_si_PD( int pm R[], int i, int X[], int j, int L );
void vsquare_si_PP( int pm R[], int i, int pm X[], int j, int L );
void vsquare_c_DD( complex_float R[], complex_float X[], int L );
void vsquare_c_DP( complex_float R[], complex_float pm X[], int L );
void vsquare_c_PD( complex_float pm R[], complex_float X[], int L );
void vsquare_c_PP( complex_float pm R[], complex_float pm X[], int L );
void vsquare_sc_DD( complex_float R[], int i, complex_float X[], int j, int L );
void vsquare_sc_DP( complex_float R[], int i, complex_float pm X[], int j,
                    int L );
void vsquare_sc_PD( complex_float pm R[], int i, complex_float X[], int j,
                    int L );
void vsquare_sc_PP( complex_float pm R[], int i, complex_float pm X[], int j,
                    int L );

void vsub_f_DDD( float R[], float X[], float Y[], int L );
void vsub_f_DDP( float R[], float X[], float pm Y[], int L );
void vsub_f_PDP( float pm R[], float X[], float pm Y[], int L );
void vsub_sf_DDD( float R[], int i, float X[], int j, float Y[], int k, int L );
void vsub_sf_DDP( float R[], int i, float X[], int j, float pm Y[], int k,
                  int L );
void vsub_sf_PDP( float pm R[], int i, float X[], int j, float pm Y[], int k,
                  int L );

void vsub_i_DDD( int R[], int X[], int Y[], int L );
void vsub_i_DDP( int R[], int X[], int pm Y[], int L );
void vsub_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vsub_si_DDD( int R[], int i, int X[], int j, int Y[], int k, int L );
void vsub_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                  int L );
void vsub_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                  int L );

void vsub_c_DDD( complex_float R[], complex_float X[], complex_float Y[], 
                 int L );
void vsub_c_DDP( complex_float R[], complex_float X[], complex_float pm Y[], 
                 int L );
void vsub_c_PDP( complex_float pm R[], complex_float X[], complex_float pm Y[],
                 int L );
void vsub_sc_DDD( complex_float R[], int i, complex_float X[], int j, 
                  complex_float Y[], int k, int L );
void vsub_sc_DDP( complex_float R[], int i, complex_float X[], int j, 
                  complex_float pm Y[], int k, int L );
void vsub_sc_PDP( complex_float pm R[], int i, complex_float X[], int j, 
                  complex_float pm Y[], int k, int L );

void vsubsub_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vsubsub_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vsubsub_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vsubsub_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vsubsub_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], int L );
void vsubsub_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k,
                      float Z[], int m, int L );
void vsubsub_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vsubsub_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vsubsub_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vsubsub_sf_PDPP( float pm R[], int i, float X[], int j, 
                      float pm Y[], int k, float pm Z[], int m, int L );

void vsubsub_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vsubsub_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vsubsub_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vsubsub_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vsubsub_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], int L );
void vsubsub_si_DDDD( int R[], int i, int X[], int j, int Y[], int k,
                      int Z[], int m, int L );
void vsubsub_si_DDDP( int R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vsubsub_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vsubsub_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k,
                      int pm Z[], int m, int L );
void vsubsub_si_PDPP( int pm R[], int i, int X[], int j, 
                      int pm Y[], int k, int pm Z[], int m, int L );

void vsubsub_c_DDDD( complex_float R[], complex_float X[], 
                     complex_float Y[], complex_float Z[], int L );
void vsubsub_c_DDDP( complex_float R[], complex_float X[], 
                     complex_float Y[], complex_float pm Z[], int L );
void vsubsub_c_DDPP( complex_float R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vsubsub_c_PDDP( complex_float pm R[], complex_float X[], 
                     complex_float Y[], complex_float pm Z[], int L );
void vsubsub_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vsubsub_sc_DDDD( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float Z[], int m, 
                      int L );
void vsubsub_sc_DDDP( complex_float R[], int i, complex_float X[], int j, 
                      complex_float Y[], int k, complex_float pm Z[], int m,
                      int L );
void vsubsub_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
                      int L );
void vsubsub_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
                      int L );
void vsubsub_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
                      int L );

void vsubmul_f_DDDD( float R[], float X[], float Y[], float Z[], int L );
void vsubmul_f_DDDP( float R[], float X[], float Y[], float pm Z[], int L );
void vsubmul_f_DDPP( float R[], float X[], float pm Y[], float pm Z[], int L );
void vsubmul_f_PDDP( float pm R[], float X[], float Y[], float pm Z[], int L );
void vsubmul_f_PDPD( float pm R[], float X[], float pm Y[], float Z[], int L );
void vsubmul_f_PDPP( float pm R[], float X[], float pm Y[], float pm Z[], 
                     int L );
void vsubmul_sf_DDDD( float R[], int i, float X[], int j, float Y[], int k, 
                      float Z[], int m, int L );
void vsubmul_sf_DDDP( float R[], int i, float X[], int j, float Y[], int k, 
                      float pm Z[], int m, int L );
void vsubmul_sf_DDPP( float R[], int i, float X[], int j, float pm Y[], int k,
                      float pm Z[], int m, int L );
void vsubmul_sf_PDDP( float pm R[], int i, float X[], int j, float Y[], int k,
                      float pm Z[], int m, int L );
void vsubmul_sf_PDPD( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float Z[], int m, int L );
void vsubmul_sf_PDPP( float pm R[], int i, float X[], int j, float pm Y[], 
                      int k, float pm Z[], int m, int L );
void vsubmul_i_DDDD( int R[], int X[], int Y[], int Z[], int L );
void vsubmul_i_DDDP( int R[], int X[], int Y[], int pm Z[], int L );
void vsubmul_i_DDPP( int R[], int X[], int pm Y[], int pm Z[], int L );
void vsubmul_i_PDDP( int pm R[], int X[], int Y[], int pm Z[], int L );
void vsubmul_i_PDPD( int pm R[], int X[], int pm Y[], int Z[], int L );
void vsubmul_i_PDPP( int pm R[], int X[], int pm Y[], int pm Z[], int L );
void vsubmul_si_DDDD( int R[], int i, int X[], int j, int Y[], int k, int Z[], 
                      int m, int L );
void vsubmul_si_DDDP( int R[], int i, int X[], int j, int Y[], int k, 
                      int pm Z[], int m, int L );
void vsubmul_si_DDPP( int R[], int i, int X[], int j, int pm Y[], int k, 
                      int pm Z[], int m, int L );
void vsubmul_si_PDDP( int pm R[], int i, int X[], int j, int Y[], int k, 
                      int pm Z[], int m, int L );
void vsubmul_si_PDPD( int pm R[], int i, int X[], int j, int pm Y[], int k,
                      int Z[], int m, int L );
void vsubmul_si_PDPP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                      int pm Z[], int m, int L );
void vsubmul_c( complex_float R[], complex_float X[], complex_float Y[], 
                complex_float Z[], int L );
void vsubmul_c_DDDD( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float Z[], int L );
void vsubmul_c_DDDP( complex_float R[], complex_float X[], complex_float Y[], 
                     complex_float pm Z[], int L );
void vsubmul_c_DDPP( complex_float R[], complex_float X[], complex_float pm Y[],
                     complex_float pm Z[], int L );
void vsubmul_c_PDDP( complex_float pm R[], complex_float X[], complex_float Y[],
                     complex_float pm Z[], int L );
void vsubmul_c_PDPD( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float Z[], int L );
void vsubmul_c_PDPP( complex_float pm R[], complex_float X[], 
                     complex_float pm Y[], complex_float pm Z[], int L );
void vsubmul_sc_DDDD( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float Z[], int m,
							 int L );
void vsubmul_sc_DDDP( complex_float R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vsubmul_sc_DDPP( complex_float R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );
void vsubmul_sc_PDDP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float Y[], int k, complex_float pm Z[], int m,
							 int L );
void vsubmul_sc_PDPD( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float Z[], int m,
							 int L );
void vsubmul_sc_PDPP( complex_float pm R[], int i, complex_float X[], int j,
                      complex_float pm Y[], int k, complex_float pm Z[], int m,
							 int L );

float vsum_f_D( float X[], int L );
float vsum_f_P( float pm X[], int L );
float vsum_sf_D( float X[], int i, int L );
float vsum_sf_P( float pm X[], int i, int L );

int vsum_i_D( int X[], int L );
int vsum_i_P( int pm X[], int L );
int vsum_si_D( int X[], int i, int L );
int vsum_si_P( int pm X[], int i, int L );

complex_float vsum_c_D( complex_float X[], int L );
complex_float vsum_c_P( complex_float pm X[], int L );
complex_float vsum_sc_D( complex_float X[], int i, int L );
complex_float vsum_sc_P( complex_float pm X[], int i, int L );

float vsummag2_f_D( float X[], int L );
float vsummag2_f_P( float pm X[], int L );
float vsummag2_sf_D( float X[], int i, int L );
float vsummag2_sf_P( float pm X[], int i, int L );
int vsummag2_i_D( int X[], int L );
int vsummag2_i_P( int pm X[], int L );
int vsummag2_si_D( int X[], int i, int L );
int vsummag2_si_P( int pm X[], int i, int L );
float vsummag2_c_D( complex_float X[], int L );
float vsummag2_c_P( complex_float pm X[], int L );
float vsummag2_sc_D( complex_float X[], int i, int L );
float vsummag2_sc_P( complex_float pm X[], int i, int L );

void vswap_f_DD( float X[], float Y[], int L );
void vswap_f_DP( float X[], float pm Y[], int L );
void vswap_f_PD( float pm X[], float Y[], int L );
void vswap_f_PP( float pm X[], float pm Y[], int L );
void vswap_sf_DD( float X[], int i, float Y[], int j, int L );
void vswap_sf_DP( float X[], int i, float pm Y[], int j, int L );
void vswap_sf_PD( float pm X[], int i, float Y[], int j, int L );
void vswap_sf_PP( float pm X[], int i, float pm Y[], int j, int L );
void vswap_i_DD( int X[], int Y[], int L );
void vswap_i_DP( int X[], int pm Y[], int L );
void vswap_i_PD( int pm X[], int Y[], int L );
void vswap_i_PP( int pm X[], int pm Y[], int L );
void vswap_si_DD( int X[], int i, int Y[], int j, int L );
void vswap_si_DP( int X[], int i, int pm Y[], int j, int L );
void vswap_si_PD( int pm X[], int i, int Y[], int j, int L );
void vswap_si_PP( int pm X[], int i, int pm Y[], int j, int L );
void vswap_c_DD( complex_float dm X[], complex_float dm Y[], int L );
void vswap_c_DP( complex_float dm X[], complex_float pm Y[], int L );
void vswap_c_PD( complex_float pm X[], complex_float dm Y[], int L );
void vswap_c_PP( complex_float pm X[], complex_float pm Y[], int L );
void vswap_sc_DD( complex_float dm X[], int i, complex_float dm Y[], int j,
                  int L );
void vswap_sc_DP( complex_float dm X[], int i, complex_float pm Y[], int j, 
                  int L );
void vswap_sc_PD( complex_float pm X[], int i, complex_float dm Y[], int j,
                  int L );
void vswap_sc_PP( complex_float pm X[], int i, complex_float pm Y[], int j,
                  int L );

void vtan_f_DD( float R[], float X[], int L );
void vtan_f_DP( float R[], float pm X[], int L );
void vtan_f_PD( float pm R[], float X[], int L );
void vtan_f_PP( float pm R[], float pm X[], int L );
void vtan_sf_DD( float R[], int i, float X[], int j, int L );
void vtan_sf_DP( float R[], int i, float pm X[], int j, int L );
void vtan_sf_PD( float pm R[], int i, float X[], int j, int L );
void vtan_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vtanh_f_DD( float R[], float X[], int L );
void vtanh_f_DP( float R[], float pm X[], int L );
void vtanh_f_PD( float pm R[], float X[], int L );
void vtanh_f_PP( float pm R[], float pm X[], int L );
void vtanh_sf_DD( float R[], int i, float X[], int j, int L );
void vtanh_sf_DP( float R[], int i, float pm X[], int j, int L );
void vtanh_sf_PD( float pm R[], int i, float X[], int j, int L );
void vtanh_sf_PP( float pm R[], int i, float pm X[], int j, int L );

void vwswap_DD( void *R, void *X, int L );
void vwswap_DP( void *R, void pm *X, int L );
void vwswap_PD( void pm *R, void *X, int L );
void vwswap_PP( void pm *R, void pm *X, int L );
void vwswap_s_DD( void *R, int i, void *X, int j, int L );
void vwswap_s_DP( void *R, int i, void pm *X, int j, int L );
void vwswap_s_PD( void pm *R, int i, void *X, int j, int L );
void vwswap_s_PP( void pm *R, int i, void pm *X, int j, int L );

void vxor_i_DDD( int R[], int X[], int Y[], int L );
void vxor_i_DDP( int R[], int X[], int pm Y[], int L );
void vxor_i_PDP( int pm R[], int X[], int pm Y[], int L );
void vxor_si_DDD( int R[], int i, int X[], int j, int Y[], int k, int L );
void vxor_si_DDP( int R[], int i, int X[], int j, int pm Y[], int k,
                  int L );
void vxor_si_PDP( int pm R[], int i, int X[], int j, int pm Y[], int k,
                  int L );

int vpcompare_sf_DD( float X[], int i, float Y[], int j, int precision, int L );
int vpcompare_sf_DP( float X[], int i, float pm Y[], int j, int precision, 
                     int L );
int vpcompare_sf_PD( float pm X[], int i, float Y[], int j, int precision, 
                     int L );
int vpcompare_sf_PP( float pm X[], int i, float pm Y[], int j, int precision, 
                     int L );
int vpcompare_si_DD( int X[], int i, int Y[], int j, int precision, int L );
int vpcompare_si_DP( int X[], int i, int pm Y[], int j, int precision, 
                     int L );
int vpcompare_si_PD( int pm X[], int i, int Y[], int j, int precision, 
                     int L );
int vpcompare_si_PP( int pm X[], int i, int pm Y[], int j, int precision, 
                     int L );
int vpcompare_sc_DD( complex_float X[], int i, complex_float Y[], int j, 
                     int precision, int L );
int vpcompare_sc_DP( complex_float X[], int i, complex_float pm Y[], int j, 
                     int precision, int L );
int vpcompare_sc_PD( complex_float pm X[], int i, complex_float Y[], int j, 
                     int precision, int L );
int vpcompare_sc_PP( complex_float pm X[], int i, complex_float pm Y[], int j,
                     int precision, int L );

void vwswap( void *R, void *X, int L );
void vwswap_DD( void *R, void *X, int L );
void vwswap_DP( void *R, void pm *X, int L );
void vwswap_PD( void pm *R, void *X, int L );
void vwswap_PP( void pm *R, void pm *X, int L );

/*
 * Windowing routines
 */
void gbartlett( float R[], int L );
void gblackman( float R[], int L );
void ghamming( float R[], int L );
void ghanning( float R[], int L );
void gharris( float R[], int L );

void window_f( float R[], float X[], float pm W[], int L );
void window_sf( float R[], int sr, float X[], int sx, float pm W[], int L );
void window_c( complex_float    R[], 
               complex_float    X[], 
               complex_float pm W[], 
               int              L );

/*
 * Utility routines
 */
#define TRACE_BUFFER_SIZE      256


typedef enum trace_typ
{
   TRACE_CIRCULAR,
   TRACE_FIXED
} TRACE_TYPE;

#define CAT_FFT        1
#define CAT_FILTER     2
#define CAT_MATH       3
#define CAT_MATRIX     4
#define CAT_SORT       5
#define CAT_STAT       6
#define CAT_UTIL       7
#define CAT_VECTOR     8
#define CAT_WINDOW     9

void hex_print( char *str, int value );
void trace_init( TRACE_TYPE tr_type );
void trace_add( unsigned int value );
void *trace_get( void );

void         start_timer( void );
unsigned int stop_timer( void );
#define START_TIMER  start_timer
#define STOP_TIMER   stop_timer

void get_version_id( unsigned long *version );
void get_version_string( char **version );
void get_version_date( char **version_date_str );
void get_version_copyright( char **version_date_str );
unsigned long get_major_version_id( void );
unsigned long get_minor_version_id( void );
unsigned long get_patch_level( void );

void log_create( int argc, char *argv[] );
void log_open( int argc, char *argv[] );
void log_close( void );
void log_write( char *str );
void log_header( char *str );
void log_write_test( char *str );
/* void log_write_test2( char *sFuncName, char *sLibName ); */
/* void log_write_status( int status ); */
/* void log_passed( int elapsed_time ); */
/* void log_tests_finished( void ); */
void log_done( int elapsed_time, int code_size, int category, char *comments );
void log_failed( int status );

#endif

