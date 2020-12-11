
/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600
 * FILE        : $Workfile: vgamma_off_dd.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : XORs input vector with gamma sequense
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vgamma_off_dd.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 1     22.06.04 14:25 Unp00611
 * 
 * 1     17/07/03 vgp
 * Initial version
 *       21.01.2004 vgp 
 * Modified to use with universal TCM codec
 *       20.09.2004 vgp 
 * save r2
 *****************************************************************************/
#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>

.global _vgamma_off_DD;
.global _vgamma_off_EE;
.global _vgamma_off_ED;
.global _vgamma_off_DE;
.global _vgamma_off_DD.end;
.global _vgamma_off_EE.end;
.global _vgamma_off_ED.end;
.global _vgamma_off_DE.end;

.extern _Descrambler_Ind;
.extern _Descrambler_Seed;	
.extern _Descramble_Data;
.extern _Descrambler_Length;	

.extern _OFDM_Num_32b_Words_RX;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vgamma_off_DD( int R[], int X[], int L );
*
* Operation : R = X + G;
*
*****************************************************/
_vgamma_off_DD:
_vgamma_off_EE:
_vgamma_off_ED:
_vgamma_off_DE:
      leaf_entry;

      puts = r2;               
      r0 = i2;  puts = r0;               /* Save registers */
      r0 = i3;  puts = r0;

      i2 = r4;                  /* Address of result */
      i3 = r8;                  /* Address of source */

      b4 = _Descramble_Data;    /* Address of gamma table */
      r0 = dm(_Descrambler_Length);
      l4 = r0;

      r0 = _Descramble_Data;    /* Address gamma table */
      r4 = dm(_Descrambler_Ind);
      r0 = r0 + r4;
      i4 = r0;                  /* Set address within gamma table */
      bit set MODE1 CBUFEN;       // Circular Buffer Enable 
      r2 = dm(_OFDM_Num_32b_Words_RX);     // length
      r0 = dm(i4,m6);
      r0 = dm(i4,m7);

      r0 = dm( i3, m6 );
      lcntr = r2, do gm_lp until lce;
         r1 = dm( i4, m6 );
         r1 = r1 xor r0, r0 = dm( i3, m6 );
gm_lp:   dm( i2, m6 ) = r1;

      bit clr MODE1 CBUFEN;       // Circular Buffer Disable 
      l4 = 0;
      r0 = i4;
      r4 = _Descramble_Data;
      r0 = r0 - r4;
      dm(_Descrambler_Ind) = r0;

      r2 = gets( 3 );
      i2 = gets( 2 );        /* Restore registers */
      i3 = gets( 1 );

      leaf_exit;
_vgamma_off_DD.end:
_vgamma_off_EE.end:
_vgamma_off_ED.end:
_vgamma_off_DE.end:

.endseg;
