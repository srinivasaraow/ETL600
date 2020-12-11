/*-------------------------------------------------------------------------*
 * $Workfile: O4CV_heaptab.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : User heap source file
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/O4CV_heaptab.c $
 * 
 * 2     7.05.07 17:10 Chhamaa
 * System heap modified.
 * 
 * 1     16.04.07 16:44 Chhamaa
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

/* MANAGED-BY-SYSTEM-BUILDER                                    */

/*
** User heap source file generated on May 07, 2007 at 16:16:57.
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


extern "asm" int ldf_heap_space;
extern "asm" int ldf_heap_length;


struct heap_table_t
{
  void          *base;
  unsigned long  length;
  long int       userid;
};

#pragma file_attr("libData=HeapTable")
#pragma section("constdata")
struct heap_table_t heap_table[2] = 
{


  { &ldf_heap_space, (int) &ldf_heap_length, 0 },


  { 0, 0, 0 }
};


