
/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: vcopy_in_buf.asm $
 * COPYRIGHT   : TL consulting GmbH
 * DESCRIPTION : Copies received via tdm bus complex signal into buffer
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /branches/Source MAS/P4LT/OFDM_DSP/ASM/TDM/vcopy_in_buf.asm $
 * 
 * 8     24.07.08 15:13 Chhamaa
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *  
 * 1     10/07/03 vgp
 * Initial version
 *****************************************************************************/

//#include "ofdm_defs.h"

#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif


.global _vcopy_in_buf;

.extern _in_bptr;
.extern _in_first_bptr;
.extern _in_last_bptr;
.extern RX_Semaphore;
.extern _data_2rx_request;
.extern n_rx_csamples; 
.extern _data_2rx_grant;

		.segment/dm   seg_dmda;
.var xxx=0;

		.segment/pm   seg_pmco;

_vcopy_in_buf:
        leaf_entry;

        r0 = i2; puts = r0;
        r0 = i4; puts = r0;

/*wt2:  // check flag in main loop  , PL      
        r0 = dm(_data_2rx_request);
        r0 = pass r0;
        if eq jump wt2;
*/
        i2 = r4;               /* Address of complex_float X[] */
        bit set mode1 CBUFEN;        

        r8 = r8 + r8;          /* number of real samples */ 
        
        b4 = dm(_in_first_bptr); /* prepare ring buffer */
        i4 = dm(_in_bptr);
        r0 = dm(_in_first_bptr);
        r4 = dm(_in_last_bptr);
        r4 = r4 - r0;
        r0 = r4 + 1;
        r0 = r0 + 1;
        l4 = r0;

        lcntr = r8, do cp_lp1 until lce;
           r0 = dm( i4, m6 );	/* copy samples from ring buffer to OFDM engine */
cp_lp1:     dm( i2, m6 ) = r0;

        bit clr IMASK SP1I;
wt:
        r0 = dm(RX_Semaphore); /* wait until no interrupt routine is executed */
        r0 = pass r0;
        if ne jump wt;
        
        dm(_in_bptr) = i4;		/* update ring buffer pointer */
        r8 = lshift r8 by -1;   // number of complex samples
        r0 = dm(n_rx_csamples); /* reduce number of unprocessed samples by number of samples ..*/
        r0 = r0 - r8;			/* which have been copied to OFDM engine */
        dm(n_rx_csamples) = r0;
        dm(_data_2rx_request) = m5;
        dm(_data_2rx_grant) = m6; 
        
        // debug2008: Phase Control  Clear Flag2
        bit clr FLAGS FLG2;	
	    // end of debug2008
        
        bit set IMASK SP1I;
        
        bit clr mode1 CBUFEN;
        r0 = m6;
        b4 = 0;
        l4 = 0;
        
nret:
        i2 = gets( 2 );
        i4 = gets( 1 );

        leaf_exit;

_vcopy_in_buf.end:

	.endseg;
