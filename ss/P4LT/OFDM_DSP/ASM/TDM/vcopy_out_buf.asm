
/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: vcopy_out_buf.asm $
 * COPYRIGHT   : TL consulting GmbH
 * DESCRIPTION : Copies generated complex vector for output via tdm bus
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /branches/Source MAS/P4LT/OFDM_DSP/ASM/TDM/vcopy_out_buf.asm $
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


.global _vcopy_out_buf;

.extern _out_bptr;
.extern _NL2;
.extern _out_first_bptr;
.extern _out_last_bptr;
.extern _n_rest_csamples;
.extern _data_2tx_request;
.extern _data_2tx_grant;
.extern TX_Semaphore;


		.segment/pm   seg_pmco;

_vcopy_out_buf:
        leaf_entry;

        r0 = i2; puts = r0;
        r0 = i4; puts = r0;

/* // Now this flag is checked in main program , PL       
wt3:                        
        r0 = dm(_data_2tx_request); // clear request
        r0 = pass r0;
        if eq jump wt3;
*/        
        r0 = r0 - r0, i2 = r4;      /* Address of complex_float X[] */
        bit set mode1 CBUFEN;
         
        r8 = r8 + r8;               /* number of real samples */ 

        b4 = dm(_out_first_bptr);	/* prepare ring buffer*/
        i4 = dm(_out_bptr);
        r0 = dm(_out_first_bptr);
        r4 = dm(_out_last_bptr);
        r4 = r4 - r0;
        r0 = r4 + 1;
        r0 = r0 + 1;
        l4 = r0;
        
        lcntr = r8, do cp_lp until lce;
           r0 = dm( i2, m6 );		/* copy OFDM samples to ring buffer*/
cp_lp:     dm( i4, m6 ) = r0;
              

        bit clr IMASK SP3I;
        nop;
        nop;
wt1:    r0 = dm(TX_Semaphore);		/* wait until there is no interrupt in progress */
        r0 = pass r0;
        if ne jump wt1;
        
        dm(_out_bptr) = i4;			/* update pointer in ring buffer */
        r8 = lshift r8 by -1;
        r0 = dm(_n_rest_csamples);  /* update counter of unprocessed samples*/
        r0 = r8 + r0;
        dm(_n_rest_csamples) = r0;
        dm(_data_2tx_request) = m5;
        dm(_data_2tx_grant) = m6;
        
         // debug2008: Phase Control  Clear Flag1
        bit clr FLAGS FLG1;	
	    // end of debug2008
	    
        bit set IMASK SP3I;
        
        bit clr mode1 CBUFEN; 
        r0 = m6;
        l4 = 0;
        b4 = 0;
        
nret:        
        i2 = gets( 2 );        /* Restore registers */
        i4 = gets( 1 );

        leaf_exit;
        
_vcopy_out_buf.end:

	.endseg;
	
