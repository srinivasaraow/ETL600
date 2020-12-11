/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vxor_i_EEE.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH.
 * DESCRIPTION : 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vxor_i_EEE.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 1     10/07/03 vgp
 * Initial version
 * 2     19/09/04 vgp
 * save r2
 *****************************************************************************/

#include <asm_sprt.h>


.global _vxor_i_EEE;
.global _vxor_i_EED;
.global _vxor_i_EDE;
.global _vxor_i_DEE;
.global _vxor_i_DED;
.global _vxor_i_DDE;
.global _vxor_i_EDD;


		.segment/pm   seg_pmco;

/*****************************************************
*
* void vxor_i_EEE( int R[], int X[], int Y[], int L );
*
* Operation : r[i] = x[i] xor y[i], i = 0..L-1
*
*
*****************************************************/
_vxor_i_EEE:
_vxor_i_EED:
_vxor_i_EDE:
_vxor_i_DEE:
_vxor_i_DED:
_vxor_i_DDE:
_vxor_i_EDD:

      leaf_entry;
      puts = r2;
      r0 = i1; puts = r0;  /* Save registers */
      r0 = i2; puts = r0;

      i4 = r4;             /* Address of R[] */
      i1 = r8;             /* Address of X[] */
      i2 = r12;            /* Address of Y[] */
      r2 = reads( 1 );     /* Number of elements */

      r2 = pass r2;
      if le jump vxor_ext;
      r2 = r2 - 1, r8  = dm(i1,m6);  /* x[0] */
      r12 = dm(i2,m6 );  /* y[0] */
      if eq jump v1xor;
      lcntr = r2, do xor_lp until lce;
	     r4 = r8 xor r12, r8 = dm(i1,m6);
		 r12 = dm(i2,m6);
xor_lp:  dm(i4,m6) = r4;
v1xor:
      r4 = r8 xor r12;
	  dm(i4,m6) = r4;
	  
vxor_ext:      
	  r2 = gets(3);
      i1 = gets(2);       /* Restore registers */
      i2 = gets(1);

      leaf_exit;

_vxor_i_EEE.end:
_vxor_i_EED.end:
_vxor_i_EDE.end:
_vxor_i_DEE.end:
_vxor_i_DED.end:
_vxor_i_DDE.end:
_vxor_i_EDD.end:

.endseg;
