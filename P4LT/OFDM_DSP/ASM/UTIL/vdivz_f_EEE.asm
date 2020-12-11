
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vdivz_f_EEE.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : division of two vectors
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vdivz_f_EEE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     01/03/04 vgp
 * Initial version
 * 2     08.05.04 vgp
 *   r0 = r0 - r0 instead f0 = f0 - f0 which can produce NAN
 * 3     20.09.04 vgp
 *  save r2
 *****************************************************************************/

#include <asm_sprt.h>


.global _vdivz_f_EEE;
.global _vdivz_f_EED;
.global _vdivz_f_EDE;
.global _vdivz_f_DEE;
.global _vdivz_f_DED;
.global _vdivz_f_DDE;
.global _vdivz_f_EDD;


//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;
/*****************************************************
*
* void vdivz_f_EEE( float R[], float X[], float Y[], int L );
* void vdivz_f_EED( float R[], float X[], float Y[], int L );
* void vdivz_f_EDE( float R[], float X[], float Y[], int L );
* void vdivz_f_DEE( float R[], float X[], float Y[], int L );
* void vdivz_f_DED( float R[], float X[], float Y[], int L );
* void vdivz_f_DDE( float R[], float X[], float Y[], int L );
* void vdivz_f_EDD( float R[], float X[], float Y[], int L );
*
* Operation : R[i] = X[i] / Y[i], i=0..L-1
*
*****************************************************/
_vdivz_f_EEE:
_vdivz_f_EED:
_vdivz_f_EDE:
_vdivz_f_DEE:
_vdivz_f_DED:
_vdivz_f_DDE:
_vdivz_f_EDD:

        leaf_entry;

        r0 = i2; puts = r0;              /* Save registers */
        r0 = i3; puts = r0;
        puts = r2;
        puts = r7;
        puts = r11;

        i2 = r4;               /* Address of R[] */
        i3 = r8;               /* Address of X[] */
        i4 = r12;              /* Address of Y[] */
        r2 = reads( 1 );       /* Get length */

        f11 = 2.0;
        f7 = dm(i3,m6);
        r0 = r0 - r0, f12 = dm(i4,m6);
        lcntr = r2, do div_lp until lce;
           f12 = pass f12;
           if ne f0 = recips f12;
           f12 = f0*f12;
           f7 = f0*f7, f0 = f11-f12;
           f12 = f0*f12;
           f7 = f0*f7, f0 = f11-f12;
           f12 = f0*f12;
           f7 = f0*f7, f0 = f11-f12, f12 = dm(i4,m6);
           f0 = f0*f7, f7 = dm(i3,m6);           
div_lp:    r0 = r0 - r0, dm(i2,m6) = f0;

        i2 = gets( 5 );        /* Restore registers */
        i3 = gets( 4 );
        r2 = gets( 3 );
        r7 = gets( 2 );
        r11 = gets( 1 );


        exit;

_vdivz_f_EEE.end:
_vdivz_f_EED.end:
_vdivz_f_EDE.end:
_vdivz_f_DEE.end:
_vdivz_f_DED.end:
_vdivz_f_DDE.end:
_vdivz_f_EDD.end:

.endseg;
