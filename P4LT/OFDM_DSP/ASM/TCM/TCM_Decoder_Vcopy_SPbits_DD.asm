/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Decoder_Vcopy_SPbits_DD.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Extracts shaping encoded information bits 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Input/output parameter revision revision
 *       17.01.2004 vgp
 *       i6 DAG REGISTER DOEN'T WORK WITH EXTERNAL MEMORY
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 * Modification for universal TCM codec
 *       22.01.2004 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Decoder_Vcopy_SPbits_DD.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 3     30.09.04 8:56 Leeb02
 * 
 * 2     30.09.04 8:27 Leeb02
 */
#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

.global _TCM_Decoder_Vcopy_SPbits_DD;

#define _x_sign_d_ptr _wb_sig_ptr  // _wb_sig points temporary array of length 16*LEN_4D_MAX_RX
#define _y_sign_d_ptr _wb_st_ptr   // _wb_st points temporary array of length 16*LEN_4D_MAX_RX

.extern _x_sign_d_ptr;   // points array of length LEN_2D_MAX_RX
.extern _y_sign_d_ptr;   // points array of length LEN_2D_MAX_RX

	.section/pm seg_pmco;

/*****************************************************
*
* void Vcopy_SPbit_DD( int R[], int d2alloc[], int L );
*
* Operation : R   = X;  R    = Y
*              2i    i   2i+1   i     i=0..L-1
*
*****************************************************/
_TCM_Decoder_Vcopy_SPbits_DD:

    leaf_entry;

    r0 = i1; puts = r0;
    r0 = i2; puts = r0;    /* Save registers */
    r0 = i3; puts = r0;
    r0 = i4; puts = r0;      

    ustat1 = mode1;             // 22.01.04 vgp
    bit set mode1 SSE;

    i4 = dm(_x_sign_d_ptr);   // address of X_Sign[]
    i3 = dm(_y_sign_d_ptr);   // address of Y_Sign[]
    
    i2 = r4;              /* Address of result R[] */
    i1 = r8;              /* address of d2alloc[] */
    r0 = r12;             /* Length */
    r0 = pass r0;
    if eq jump lend;

    r12 = dm(i1,m6);     /* r12 = d2alloc[i] */
    lcntr = r0, do (pc,lp1) until lce;
       r8 = dm( i3, m6 );
       r12 = pass r12, r4 = dm( i4, m6 );
       if lt jump lp1;
         dm( i2, m6 ) = r4;
         dm( i2, m6 ) = r8;
lp1:   r12 = dm(i1,m6);

lend:
    mode1 = ustat1;             // 22.01.04 vgp

    i1 = gets( 4 );
    i2 = gets( 3 );        /* Restore registers */
    i3 = gets( 2 );
    i4 = gets( 1 );


    leaf_exit;
_TCM_Decoder_Vcopy_SPbits_DD.end:
.endseg;

