/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Decoder_Vget_SPbits_DD.asm $
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
 * Short word addressing of d_h_for[] and d_old_d[]
 *       19.01.2004 vgp
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Decoder_Vget_SPbits_DD.asm $
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

#define _x_sign_d_ptr _wb_sig_ptr  // _wb_sig points temporary array of length 16*LEN_4D_MAX_RX
#define _y_sign_d_ptr _wb_st_ptr   // _wb_st points temporary array of length 16*LEN_4D_MAX_RX

.extern _x_sign_d_ptr;   // points array of length LEN_2D_MAX_RX
.extern _y_sign_d_ptr;   // points array of length LEN_2D_MAX_RX

	.section/dm seg_dmda;

.align 2;
 .var d_h_for[2] = 0, 0;
.align 2;
 .var d_old_d[2] = 0, 0;

    .endseg;


	.section/pm seg_pmco;
    .global _TCM_Decoder_Vget_SPbits_DD;

/*****************************************************
*
* void Vget_SPbit_DD( int R[], int d2alloc[], int L );
*
* Operation : 
*             
*
*****************************************************/

_TCM_Decoder_Vget_SPbits_DD:

    leaf_entry;

    r0 = i0; puts = r0;      
    r0 = i1; puts = r0;      
    r0 = i2; puts = r0;    /* Save registers */
    r0 = i3; puts = r0;
    r0 = i4; puts = r0;      
    r0 = i5; puts = r0;      
    r0 = m2; puts = r0;

    ustat1 = mode1;             // 22.01.04 vgp
    bit set mode1 SSE;
	
// clear array <d_h_for>
    i1 =  d_h_for;          // normal adressing
    dm(i1,m6) = m13;        // m13 must be 0
    dm(i1,m6) = m13;

    i4 = dm(_x_sign_d_ptr);   // address of X_Sign[]
    i3 = dm(_y_sign_d_ptr);   // address of Y_Sign[]
     
    i2 = r4;              /* Address of result R[] */
    i0 = r8;              /* address of d2alloc[] */
    r0 = r12;             /* Length */
    r0 = pass r0;
    if eq jump lend;

    i1 =  2*d_h_for;        // SW addressing
    i5 =  2*d_old_d;        // SW addressing
    m2 = -3;

    lcntr = r0, do (pc,lp1) until lce;
      r8 = dm(i0,m6);   // r8 = d2alloc[i]
      r8 = pass r8;
      if lt jump lp1 (db);
      modify(i3,m6);
      modify(i4,m6);
//    copy <d_h_for> in to <d_old_d>  
      modify(i3,m7);
      modify(i4,m7);
      r4 = dm(i1,m6);   //0
      dm(i5,m6) = r4;
      r0 = dm(i1,m6);   //1
      dm(i5,m6) = r0;
      r4 = dm(i1,m6);   //2
      dm(i5,m6) = r4;
      r4 = dm(i1,m2);   //3
      dm(i5,m2) = r4;
      r0 = r0 xor r4, r4 = dm(i4,m6);
      r0 = r0 xor r4, dm(i1,m6) = r4; 
      r4 = dm(2*d_old_d); // SW addressing
      r0 = r0 xor r4, dm(i1,m6) = r4;
      r4 = dm(i3,m6);
      r0 = r0 xor r4, dm(i1,m6) = r4;
      r4 = dm(2*d_old_d+2);// SW addressing
      dm(i1,m2) = r4;
      dm(i2,m6) = r0;
lp1:  nop;

lend:

    mode1 = ustat1;             // 22.01.04 vgp

    i0 = gets( 7 );
    i1 = gets( 6 );
    i2 = gets( 5 );        /* Restore registers */
    i3 = gets( 4 );
    i4 = gets( 3 );
    i5 = gets( 2 );
    m2 = gets( 1 );

    leaf_exit;
    
_TCM_Decoder_Vget_SPbits_DD.end:

.endseg;

