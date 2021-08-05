
/******************************************************************************
 *
 * PROJECT     : 
 * FILE        : $Workfile: vsubmul_c_DDPD.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Adds two vectors
 * OPERATION   : R = (X-Y) .* Z
 * PROTOTYPE   : void vsubmul_c_DDPD( complex_float    R[],
 *                                    complex_float    X[],
 *                                    complex_float pm Y[],
 *                                    complex_float    Z[],
 *                                    int              N );
 * ASSUMPTIONS :
 *               All arrays must start on even address boundaries
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsubmul_c_DDPD.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *
 * 1     06.05.04 vgp
 * Initial version
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif
#ifdef __ADSP21262__
#include <def21262.h>
#endif

#include <asm_sprt.h>

	.segment /pm seg_pmco;

.global _vsubmul_c_DDPD;

_vsubmul_c_DDPD:
      leaf_entry;

      puts = r10;
      puts = r11;
      r0   = m4;  puts = r0;
      r0   = m8;  puts = r0;
      r0   = i2;  puts = r0;
      r0   = i3;  puts = r0;

      i2  = r4;                    // address of R[] 
      i3  = r8;                    // address of X[] 
      i8  = r12;                   // address of Y[] 
      i4  = reads(1);              // address of Z[] 
      r12 = reads(2);              // N
	  m8 = 2;
	  f11 = -1.0;
	  s11 =  1.0;

      bit set mode1 PEYEN;         // Enter SIMD mode 
      m4  = 2;

      f4 = dm(i3,m4), f8 = pm(i8,m8);     // f4 = x[0].re, s4 = x[0].im, f8 = y[0].re s8 = y[0].im  
      lcntr = r12, do mul_lp until lce;
         f4 = f4-f8, f12 = dm(i4,m4);     // f4=x[i].re - y[i].re, s4 = x[i].im - y[i].im, f12 = y.re[i], s12 = y.im[i] 
         f0 = f4*f12;                     // f0=(x[i].re - y[i].re)*z[i].re, s0=(x[i].im - y[i].im)*z[i].im
		 f4 <-> s4;                       // f4=x[i].im - y[i].im, s4 = x[i].re - y[i].re
		 f10 = f4*f12;                    // f10=(x[i].im - y[i].im)*z[i].re, s10=(x[i].re - y[i].re)*z[i].im
		 f10 <-> s0;                      // f10=(x[i].im - y[i].im)*z[i].im, s0=(x[i].im - y[i].im)*z[i].re 
         f10 = f10*f11, f4 = dm(i3,m4);   // f10=-(x[i].im - y[i].im)*z[i].im, s10=(x[i].re - y[i].re)*z[i].im
		 f0 = f0 + f10, f8 = pm(i8,m8);   // f0 = (x[i].re - y[i].re)*z[i].re - (x[i].im - y[i].im)*z[i].im;
		                                  // s0 = (x[i].im - y[i].im)*z[i].im + s10=(x[i].re - y[i].re)*z[i].im
mul_lp:  dm(i2, m4) = f0;                 // Store result in r[i], i++
      
	  bit clr mode1 PEYEN;               // Enter SISD mode 
      nop;

      r10 = gets( 6 );
      r11 = gets( 5 );
      m4  = gets( 4 );
      m4  = gets( 3 );
      i2  = gets( 2 );
      i3  = gets( 1 );

      leaf_exit;
_vsubmul_c_DDPD.end:

.endseg;
