/*-------------------------------------------------------------------------*
 * $Workfile: benchmark.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : 
 * Remarks      :
 * Purpose      : Functions to count DSP cycles
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/ASM/benchmark.asm $
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 2     9.11.04 11:05 Maag01
 * Initial version
 * 
 * 
 *-------------------------------------------------------------------------*/


/*****************************************************/
/*                                                   */
/*  int Clock_start = count_start()                  */
/*  int Clock_count = count_end(int Clock_start)     */
/*                                                   */
/*****************************************************/

#include "def21161.h"
#include "asm_sprt.h"

.segment/pm seg_pmco;
.global _count_start;
.global _count_end;

_count_start: /* call this to start cycle count */
r1=mode1;
bit clr mode1 IRPTEN;
r0=emuclk;
mode1=r1;
exit;
_count_start.end:

_count_end: /* call this to end cycle count */ 
r2=mode1;
bit clr mode1 IRPTEN;
r0=emuclk;
r0=r0-r4;
r1=14;     /* fudge factor to compensate for overhead */
r0=r0-r1;  
mode1=r2;
exit;
_count_end.end:

.endseg;
