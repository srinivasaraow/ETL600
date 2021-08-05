//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      timer.asm
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        1-NOV-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    ADSP initialization 
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    01.11.03  Vladimir Potapov (vgp), Initial Version
//
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/timer.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 4     22.12.04 9:21 Chchlee
 * 
 * 3     21.10.04 10:56 Maag01
 * 
 * 2     30.09.04 8:27 Leeb02
*/

#include <def21161.h>
#include <asm_sprt.h>

.segment/pm seg_pmco;

.global	_Timer_Start;
.global	_Timer_Stop;
.global        _Get_Timer;

//=======================================================
_Timer_Start:
   leaf_entry;
       
      TCOUNT = r4;
      TPERIOD = r4;
      bit set MODE2 TIMEN;

   leaf_exit;
_Timer_Start.end:
//=======================================================
_Timer_Stop:
   leaf_entry;

   bit clr MODE2 TIMEN;

   leaf_exit;
_Timer_Stop.end:
//=======================================================
_Get_Timer:
   leaf_entry;

   r0 = TCOUNT;
   leaf_exit;
_Get_Timer.end:



