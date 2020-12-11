
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vadd_sf_EEE.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : additions of two vectors with stride
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * 1     09/11/04 vgp
 * Initial version
 *****************************************************************************/

#include <asm_sprt.h>


.global _vadd_sf_EEE;
.global _vadd_sf_EED;
.global _vadd_sf_EDE;
.global _vadd_sf_DEE;
.global _vadd_sf_DED;
.global _vadd_sf_DDE;
.global _vadd_sf_EDD;


//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;
/*****************************************************
*
* void vadd_sf_EEE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
* void vadd_sf_EED( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
* void vadd_sf_EDE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
* void vadd_sf_DEE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
* void vadd_sf_DED( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
* void vadd_sf_DDE( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
* void vadd_sf_EDD( float R[], float X[], int Stride_X, float Y[], int Stride_Y, int L );
*
* Operation : R = X + Y;
*
*****************************************************/
_vadd_sf_EEE:
_vadd_sf_EED:
_vadd_sf_EDE:
_vadd_sf_DEE:
_vadd_sf_DED:
_vadd_sf_DDE:
_vadd_sf_EDD:

        leaf_entry;

        puts = r1;
		puts = r2;
        r0 = i2; puts = r0;              /* Save registers */
        r0 = i3; puts = r0;
		r0 = m3; puts = r0;
		r0 = m4; puts = r0;

        i2 = r4;               /* Address of R[] */
        i3 = r8;               /* Address of X[] */
        m3 = r12;              /* Stride  of X[] */
		i4 = reads( 1 );       /* Address of Y[] */
        m4 = reads( 2 );       /* Stride  of Y[] */ 
        r2 = reads( 3 );       /* Get length */


        f0 = dm( i3, m3 );
        lcntr = r2, do add_lp until lce;
           f1 = dm( i4, m4 );
           f1 = f1 + f0, f0 = dm( i3, m3 );
add_lp:    dm( i2, m6 ) = f1;

        r1 = gets( 6 );
		r2 = gets( 5 );
        i2 = gets( 4 );        /* Restore registers */
        i3 = gets( 3 );
		m3 = gets( 2 );
		m4 = gets( 1 );

        exit;

_vadd_sf_EEE.end:
_vadd_sf_EED.end:
_vadd_sf_EDE.end:
_vadd_sf_DEE.end:
_vadd_sf_DED.end:
_vadd_sf_DDE.end:
_vadd_sf_EDD.end:

.endseg;
