
/******************************************************************************
 *
 * PROJECT     : 
 * FILE        : $Workfile: vfsmulvadd_c_DDD.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Adds two vectors
 * OPERATION   : R = fs*X + Y
 * PROTOTYPE   : void vfsmulvadd_c_DDD( complex_float R[],
 *                                      float         fs,
 *                                      complex_float X[],
 *                                      complex_float Y[],
 *                                      int           N );
 * ASSUMPTIONS :
 *               All arrays must start on even address boundaries, N is even
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vfsmulvadd_c_DDD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
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

.global _vfsmulvadd_c_DDD;

_vfsmulvadd_c_DDD:
      leaf_entry;

      r0   = m4;  puts = r0;
      r0   = i2;  puts = r0;
      r0   = i3;  puts = r0;

      i2  = r4;                    // address R[] 
      s8 = f8;                     // s8 = f8 = fs
      i3  = r12;                   // address of X[] 
      i4  = reads(1);              // address of Y[] 
      r12 = reads(2);              // N

      bit set mode1 PEYEN;         // Enter SIMD mode 
      m4  = 2;

      f4 = dm(i3,m4);                     // read x[0].re & x[0].im 
      lcntr = r12, do add_lp until lce;
         f4 = f4*f8, f12 = dm(i4,m4);     // f4=fs*x[i].re, s4 = fs*x[i].im, f12 = y.re[i], s12 = y.im[i] 
         f12 = f4 + f12, f4 = dm(i3,m4);  // f12 = fs*x[i]+y[i], f4 = x[i+1].re, s4 = x[i+1].im
add_lp:  dm(i2, m4) = f12;                // Store result in r[i], i++
      
	bit clr mode1 PEYEN;         // Enter SISD mode 
      nop;

      m4  = gets( 3 );
      i2  = gets( 2 );
      i3  = gets( 1 );

      leaf_exit;
_vfsmulvadd_c_DDD.end:

.endseg;
