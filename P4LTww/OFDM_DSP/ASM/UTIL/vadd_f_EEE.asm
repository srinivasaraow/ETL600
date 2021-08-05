
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vadd_f_EEE.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : additions of two vectors
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * 1     01/03/04 vgp
 * Initial version
 * 2     20/09/04 vgp
 *  save r2
 * 3     09/11/04 vgp
 *  save r1
 *****************************************************************************/

#include <asm_sprt.h>


.global _vadd_f_EEE;
.global _vadd_f_EED;
.global _vadd_f_EDE;
.global _vadd_f_DEE;
.global _vadd_f_DED;
.global _vadd_f_DDE;
.global _vadd_f_EDD;


//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;
/*****************************************************
*
* void vadd_f_EEE( float R[], float X[], float Y[], int L );
* void vadd_f_EED( float R[], float X[], float Y[], int L );
* void vadd_f_EDE( float R[], float X[], float Y[], int L );
* void vadd_f_DEE( float R[], float X[], float Y[], int L );
* void vadd_f_DED( float R[], float X[], float Y[], int L );
* void vadd_f_DDE( float R[], float X[], float Y[], int L );
* void vadd_f_EDD( float R[], float X[], float Y[], int L );
*
* Operation : R = X + Y;
*
*****************************************************/
_vadd_f_EEE:
_vadd_f_EED:
_vadd_f_EDE:
_vadd_f_DEE:
_vadd_f_DED:
_vadd_f_DDE:
_vadd_f_EDD:

        leaf_entry;

        puts = r1;
		puts = r2;
        r0 = i2; puts = r0;              /* Save registers */
        r0 = i3; puts = r0;

        i2 = r4;               /* Address of R[] */
        i3 = r8;               /* Address of X[] */
        i4 = r12;              /* Address of Y[] */
        r2 = reads( 1 );       /* Get length */


        f0 = dm( i3, m6 );
        lcntr = r2, do add_lp until lce;
           f1 = dm( i4, m6 );
           f1 = f1 + f0, f0 = dm( i3, m6 );
add_lp:    dm( i2, m6 ) = f1;

        r1 = gets( 4 );
		r2 = gets( 3 );
        i2 = gets( 2 );        /* Restore registers */
        i3 = gets( 1 );

        exit;

_vadd_f_EEE.end:
_vadd_f_EED.end:
_vadd_f_EDE.end:
_vadd_f_DEE.end:
_vadd_f_DED.end:
_vadd_f_DDE.end:
_vadd_f_EDD.end:

.endseg;
