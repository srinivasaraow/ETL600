
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vmul_f_EEE.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Multiplies two vectors
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vmul_f_EEE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     10/07/03 vgp
 * Initial version
 * 1     20/09/04 vgp
 * save r2
 *****************************************************************************/

#include <asm_sprt.h>


.global _vmul_f_EEE;
.global _vmul_f_EED;
.global _vmul_f_EDE;
.global _vmul_f_DEE;
.global _vmul_f_DED;
.global _vmul_f_DDE;
.global _vmul_f_EDD;

.global _vmul_f_EEE.end;
.global _vmul_f_EED.end;
.global _vmul_f_EDE.end;
.global _vmul_f_DEE.end;
.global _vmul_f_DED.end;
.global _vmul_f_DDE.end;
.global _vmul_f_EDD.end;

		.segment/pm   seg_pmco;
/*****************************************************
*
* void vmul_f_EEE( float R[], float X[], float Y[], int L );
* void vmul_f_EED( float R[], float X[], float Y[], int L );
* void vmul_f_EDE( float R[], float X[], float Y[], int L );
* void vmul_f_DEE( float R[], float X[], float Y[], int L );
* void vmul_f_DED( float R[], float X[], float Y[], int L );
* void vmul_f_DDE( float R[], float X[], float Y[], int L );
* void vmul_f_EDD( float R[], float X[], float Y[], int L );
*
* Operation : R = X * Y;
*
*****************************************************/
_vmul_f_EEE:
_vmul_f_EED:
_vmul_f_EDE:
_vmul_f_DEE:
_vmul_f_DED:
_vmul_f_DDE:
_vmul_f_EDD:

        leaf_entry;

        puts = r2;
        r0 = i2; puts = r0;              /* Save registers */
        r0 = i3; puts = r0;

        i2 = r4;               /* Address of R[] */
        i3 = r8;               /* Address of X[] */
        i4 = r12;              /* Address of Y[] */
        r2 = reads( 1 );       /* Get length */


        f0 = dm( i3, m6 );
        lcntr = r2, do mul_lp until lce;
           f1 = dm( i4, m6 );
           f1 = f1 * f0, f0 = dm( i3, m6 );
mul_lp:    dm( i2, m6 ) = f1;

        r2 = gets( 3 );
        i2 = gets( 2 );        /* Restore registers */
        i3 = gets( 1 );

        exit;

_vmul_f_EEE.end:
_vmul_f_EED.end:
_vmul_f_EDE.end:
_vmul_f_DEE.end:
_vmul_f_DED.end:
_vmul_f_DDE.end:
_vmul_f_EDD.end:

.endseg;
