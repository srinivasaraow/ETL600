/*-------------------------------------------------------------------------*
 * $Workfile: O4CV_cplbtab.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Cache Protection Lookaside Buffers table definitions
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/O4CV_cplbtab.c $
 * 
 * 2     7.05.07 17:23 Chhamaa
 * Cache settings modified for bank 3 in ext. memory.
 * 
 * 1     7.05.07 17:11 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/

/* MANAGED-BY-SYSTEM-BUILDER                                    */

/*
** CPLB table definitions generated on May 07, 2007 at 16:16:57.
**
** Copyright (C) 2000-2006 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the LDF Wizard. Changes to the LDF configuration should be made by 
** changing the appropriate options rather than editing this file. 
**
** Configuration:-
**     crt_doj:                                .\Debug\O4CV_basiccrt.doj
**     processor:                              ADSP-BF538
**     si_revision:                            automatic
**     cplb_init_cplb_ctrl:                    63
**     cplb_init_dcache_ctrl:                  dcache_wt
**     cplb_init_cplb_src_file:                C:\Data\ADSP\O4CV\FW\O4CV_cplbtab.c
**     cplb_init_cplb_obj_file:                .\Debug\O4CV_cplbtab.doj
**     using_cplusplus:                        false
**     mem_init:                               false
**     use_vdk:                                false
**     use_eh:                                 true
**     use_argv:                               false
**     running_from_internal_memory:           true
**     user_heap_src_file:                     C:\Data\ADSP\O4CV\FW\O4CV_heaptab.c
**     libraries_use_stdlib:                   true
**     libraries_use_fileio_libs:              false
**     libraries_use_ieeefp_emulation_libs:    false
**     libraries_use_eh_enabled_libs:          false
**     system_heap:                            SCRATCHPAD
**     system_heap_min_size:                   1K
**     system_stack:                           L1
**     system_stack_min_size:                  6K
**     use_sdram:                              true
**     use_sdram_size:                         16M
**     use_sdram_partitioned:                  custom
**
*/


#define CACHE_MEM_MODE CPLB_DDOCACHE_WT


#include <sys/platform.h>
#include <cplbtab.h>

#pragma section("cplb_data")

cplb_entry dcplbs_table[] = {

/*$VDSG<customisable-data-cplb-table>                           */
/* This code is preserved if the CPLB tables are re-generated.  */

   // L1 Data A & B, (set write-through bit to avoid 1st write exceptions)
   {0xFF800000, (PAGE_SIZE_4MB | CPLB_DNOCACHE  | CPLB_LOCK | CPLB_WT)}, 

   // Async Memory, all Banks
   {0x20000000, (PAGE_SIZE_4MB | CPLB_DNOCACHE  | CPLB_LOCK | CPLB_WT)}, 

   // 16 MB SDRAM memory space, 4x4MByte, Bank #3 not cached:
   {0x00000000, (PAGE_SIZE_4MB | CACHE_MEM_MODE | CPLB_LOCK | CPLB_WT)}, 
   {0x00400000, (PAGE_SIZE_4MB | CACHE_MEM_MODE | CPLB_LOCK | CPLB_WT)}, 
   {0x00800000, (PAGE_SIZE_4MB | CPLB_DNOCACHE  | CPLB_LOCK | CPLB_WT)}, 
   {0x00C00000, (PAGE_SIZE_4MB | CACHE_MEM_MODE | CPLB_LOCK | CPLB_WT)}, 
    
   // end of section - termination
   {0xffffffff, 0}, 
/*$VDSG<customisable-data-cplb-table>                           */

}; /* dcplbs_table */


cplb_entry icplbs_table[] = {

/*$VDSG<customisable-instr-cplb-table>                          */
/* This code is preserved if the CPLB tables are re-generated.  */

   // L1 Code
   {0xFFA00000, (PAGE_SIZE_1MB | CPLB_INOCACHE | CPLB_LOCK)}, 

   // Async Memory, all Banks  
   {0x20000000, (PAGE_SIZE_4MB | CPLB_INOCACHE | CPLB_LOCK)}, 

   // 16 MB SDRAM memory space, 4x4MByte:
   {0x00000000, (PAGE_SIZE_4MB | CPLB_IDOCACHE | CPLB_LOCK)},
   {0x00400000, (PAGE_SIZE_4MB | CPLB_IDOCACHE | CPLB_LOCK)},
   {0x00800000, (PAGE_SIZE_4MB | CPLB_DNOCACHE | CPLB_LOCK)},
   {0x00c00000, (PAGE_SIZE_4MB | CPLB_IDOCACHE | CPLB_LOCK)},

   // end of section - termination
   {0xffffffff, 0}, 
/*$VDSG<customisable-instr-cplb-table>                          */

}; /* icplbs_table */



