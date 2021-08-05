//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      ofdm_sync_to_tdm.asm
//
// PROJECT:    P4LT system board of ETL600 on DRS7 board
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        24-FEB-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    Synchronize OFDM modem to tdm data line 
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    24.02.04  Initial Version, vgp
//
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TDM/ofdm_sync_to_tdm.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:57 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
 */	
#include <def21161.h>
#include <asm_sprt.h>

.global _OFDM_Sync_to_TDM;

.extern _Toggle_TDM_TX;
.extern _NL2Q;
.extern _n_rest_csamples;

.extern _out_first_bptr;
.extern _out_cur_bptr;
.extern _out_bptr;

.extern _in_first_bptr;
.extern _in_cur_bptr;
.extern _in_bptr;

.segment/pm 	seg_pmco;
	
_OFDM_Sync_to_TDM:
   leaf_entry;

lp1: r0 = dm(_Toggle_TDM_TX);
     r0 = pass r0;
     if eq jump lp1;

     // init TDM transmitter isr. pointers
     r0 = dm(_out_first_bptr);
     dm(_out_cur_bptr) = r0;        // init buffer pointer for writing      
     r1 = dm(_NL2Q);
     dm(_n_rest_csamples) = r1;     // set complex sample counter in rx buffer
     r1 = r1 + r1;
     r1 = r0 + r1;
     dm(_out_bptr) = r1;           // init buffer pointer for reading

     // init TDM receiver isr. pointers
     r4 = dm(_in_first_bptr);     
     dm(_in_bptr) = r4;            // init buffer pointer for reading
     dm(_in_cur_bptr) = r4;        // init buffer pointer for writing
               
lp2: r0 = dm(_Toggle_TDM_TX);
     r0 = pass r0;
     if ne jump lp2;
     
   leaf_exit;
_OFDM_Sync_to_TDM.end:

.endseg;

