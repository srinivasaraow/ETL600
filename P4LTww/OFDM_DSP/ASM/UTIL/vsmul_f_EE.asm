
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vsmul_f_EE.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Multiplies vector by scalar
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsmul_f_EE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     10/07/03 vgp
 * Initial version
 *****************************************************************************/

#include <asm_sprt.h>


.global _vsmul_f_EE;
.global _vsmul_f_ED;
.global _vsmul_f_DE;


		.segment/pm   seg_pmco;
/*****************************************************
*
* void vsmul_f_EE( float R[], float sc, float Y[], int L );
* void vsmul_f_ED( float R[], float sc, float Y[], int L );
* void vsmul_f_DE( float R[], float sc, float Y[], int L );
*
* Operation : R[i] = sc * Y[i], i=0..L-1;
*
*****************************************************/
_vsmul_f_EE:
_vsmul_f_ED:
_vsmul_f_DE:

        leaf_entry;

        r0 = i2; puts = r0;              /* Save registers */
        puts = r2;

        i2 = r4;               /* Address of R[] */
        i4 = r12;              /* Address of X[] */
        r2 = reads( 1 );       /* Get length */
        r2 = r2 - 1;


        f4 = dm( i4, m6 );
        lcntr = r2, do mul_lp until lce;
           f12 = f4 * f8, f4 = dm( i4, m6 );
mul_lp:    dm( i2, m6 ) = f12;
        f12 = f4 * f8;
        dm( i2, m6 ) = f12;

        i2 = gets( 2 );        /* Restore registers */
        r2 = gets( 1 );

        exit;

_vsmul_f_EE.end:
_vsmul_f_ED.end:
_vsmul_f_DE.end:

.endseg;
