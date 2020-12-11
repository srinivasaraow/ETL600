/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vones_i_E.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH.
 * DESCRIPTION : count ones in integer array (weigth of bit vector)
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vones_i_E.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 1     23/01/04 vgp
 * Initial version
 * 2     20/09/04 vgp
 * delete save r2
 *****************************************************************************/

#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif
#ifdef __ADSP21262__
#include <def21262.h>
#endif

.global _vones_i_E;
.global _vones_i_D;


		.segment/pm   seg_pmco;

/*****************************************************
*
* int vones_i_E( int X[], int L );
*
* Operation : returns sum(X[i][j] == 1), i=0...L; j=31:0
*                         i - word index, j - bit index
*
*
*****************************************************/
_vones_i_E:
_vones_i_D:

      leaf_entry;

      r8 = pass r8;
      if le jump vones_ext;
      
      puts = r1;
      r0 = i4; puts = r0;  /* Save registers */

      // save interrupt mask register
      ustat1 = IMASK;
      // disable FIXI interrupt
      bit clr IMASK FIXI;
      // save mode1 register
      ustat2 = MODE1;
      // clear saturation mode
      bit clr MODE1 ALUSAT;


      i4 = r4;             /* Address of X[] */
                                   /* r8  = L */
      r0 = r0 - r0;
      lcntr = r8, do wh_lp until lce;
	     r1 = r1 - r1, r4 = dm(i4,m6);
         r4 = pass r4;
         if eq jump wh_lp;
           r8 = r4 - 1;
next_w32:
           r4 = r4 and r8;
           if ne jump next_w32 (db);
           r1 = r1 + 1;
           r8 = r4 - 1;
wh_lp:   r0  = r0 + r1;

      // restore mode1 register
      MODE1 = ustat2;
      bit clr STKYx AOS;
      bit clr IRPTL FIXI;
      // restore interrupt mask register
      IMASK = ustat1;

      r1 = gets(2);
      i4 = gets(1);
      
vones_ext:
      leaf_exit;

_vones_i_E.end:
_vones_i_D.end:

.endseg;
