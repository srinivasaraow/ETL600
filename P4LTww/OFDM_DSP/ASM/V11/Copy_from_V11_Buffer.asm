/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Copy_from_V11_Buffer.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Copy V11 input bit buffer to output
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/V11/Copy_from_V11_Buffer.asm $
 * 
 * 4     15.02.05 10:00 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 3     7.02.05 14:50 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 1     1.10.04 11:44 Leeb02
 * 
 * Initial version
 *       28.09.2004 vgp
 * Protect access to V11 input buffer when SPORT isn't activated
 *       28.01.05, vgp 
 * Protect access to V11 input buffer before first occurence of interrupt
 *       31.01.05, vgp 
*****************************************************************************/
 
//#include "../../h/v11/v11_externs.h"
#include <def21161.h>
#include <asm_sprt.h>

#define SPCTL_RX SPCTL0

.extern _V11_RX_Bit_Ptr_Read;       // current V11 RX bit buffer pointer
.extern _V11_Bit_Buffer_RX;         // points first element of RX bit bufer 
.extern _V11_RX_Bit_Buffer_Length;  // number of elements in buffer
.extern _V11_RX_IsStarted;          // flag from V11 receive ISR

.global _Copy_from_V11_Buffer;

.segment/pm 	seg_pmco;
	

/*=================================================================================================================*/
/*Input:                                                                                                           */   
/*  In_ptr = r4  - input bit buffer address                                                                        */
/*                                                                                                                 */
/*  len    = r8  - number of elements in bit buffer                                                                */
/*                                                                                                                 */
/*Output:                                                                                                          */
/*         none                                                                                                    */ 
/*=================================================================================================================*/


//void segment ( "seg_ext_pmco" ) Copy_from_V11_Buffer(int *In_ptr, int len)
//void Copy_from_V11_Buffer(int *In_ptr, int len)
//{
//	int i;
//	for (i=0; i<len; i++) {
//	 *In_ptr++ = *V11_RX_Bit_Ptr_Read++;
//       if ( V11_RX_Bit_Ptr_Read == V11_RX_Buffer_Last ) V11_RX_Bit_Ptr_Read = &V11_Bit_Buffer_RX[0];
//	}
//}

_Copy_from_V11_Buffer:

    leaf_entry;
    
    puts = r1;
    r0 = i0; puts = r0;
    r0 = mode1; puts = r0;
    
    // test that V11 input SPORT is actived
    // if Sport isn't activated then exit 
    r0 = dm(SPCTL_RX);
    r1 = SPEN_A;
    r0 = r0 and r1;
    if eq jump ext;

    // test ISR flag    
    r0 = dm(_V11_RX_IsStarted);
    r0 = pass r0;
    if eq jump ext;

    bit set mode1 CBUFEN;
    
    b0 = _V11_Bit_Buffer_RX;
    i4 = r4;
    i0 = dm(_V11_RX_Bit_Ptr_Read);
    l0 = dm(_V11_RX_Bit_Buffer_Length);
    modify(i0,m6); modify(i0,m7);     // adjust circular pointers
    lcntr = r8, do cp_lp until lce;
       r0 = dm(i0,m6);
cp_lp: dm(i4,m6) = r0;

    dm(_V11_RX_Bit_Ptr_Read) = i0;
    l0 = m5;
    b0 = 0;

ext: 
    r1 = gets(3);
    i0 = gets(2);
    mode1 = gets(1);
    leaf_exit;
_Copy_from_V11_Buffer.end:
