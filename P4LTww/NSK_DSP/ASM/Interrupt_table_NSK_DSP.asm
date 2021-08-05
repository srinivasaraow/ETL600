/*-------------------------------------------------------------------------*
 * $Workfile: Interrupt_table_NSK_DSP.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Hans Peter Rytz PTUKT
 * Remarks      :
 * Purpose      : Init routine for ADSP21161
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/NSK_DSP/ASM/Interrupt_table_NSK_DSP.asm $
 * 
 * 3     9.05.05 14:09 Chbrbus
 * Added DSP Checksum patcher
 * 
 * 2     15.02.05 10:10 Chbrbus
 * Changed interrupt routine - TDM buffer pointers all time valid in DMA
 * buffer range
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 1     22.07.04 16:08 Buhl01
 * Separate Table for each DSP
 *-------------------------------------------------------------------------*/


 #include <asm_sprt.h>
 #include <def21161.h>
 #include "../H/MagicNumbersForMemoryCheck.h"



 #define START_ADDR 0x40000			/* base address for an interrupt table in internal memory*/

.GLOBAL			___lib_prog_term;	//Termination address
.EXTERN			___lib_setup_c;

.EXTERN			_SPR0_int;
.EXTERN			_SPR1_int;
.EXTERN			_timer_int;
.EXTERN			_main;


// Variables for PM / DM Check: !!!!!!!!!!!!!!!!!!!!!!! DO NOT CHANGE POSITION
.SEGMENT/DM seg_dmda;

// For seg_rth checksum                
.var	        _CRC_Checksum_pm_rth = CRC_CHECKSUM_PM_RTH_DEFAULT;
.global			_CRC_Checksum_pm_rth;

.var	        _CRC_Checksum_pm_init = CRC_CHECKSUM_PM_INIT_DEFAULT;
.global			_CRC_Checksum_pm_init;
          
.var	        _CRC_Checksum_pm_intern = CRC_CHECKSUM_PM_INTERN_DEFAULT;
.global			_CRC_Checksum_pm_intern;      
 
.var            _CRC_Checksum_pm_extern = CRC_CHECKSUM_PM_EXTERN_DEFAULT;
.global         _CRC_Checksum_pm_extern;

// patch start and stop address for each segment to patch here
.var	        _PM_InternalSegment_rth_Start = PM_INTERNAL_SEGMENT_RTH_START_DEFAULT;
.global			_PM_InternalSegment_rth_Start; 
      
.var            _PM_InternalSegment_rth_Stop = PM_INTERNAL_SEGMENT_RTH_STOP_DEFAULT;
.global         _PM_InternalSegment_rth_Stop;

.var	        _PM_InternalSegment_init_Start = PM_INTERNAL_SEGMENT_INIT_START_DEFAULT;
.global			_PM_InternalSegment_init_Start; 
      
.var            _PM_InternalSegment_init_Stop = PM_INTERNAL_SEGMENT_INIT_STOP_DEFAULT;
.global         _PM_InternalSegment_init_Stop;

.var	        _PM_InternalSegmentStart = PM_INTERNAL_SEGMENT_START_DEFAULT;
.global			_PM_InternalSegmentStart; 
      
.var            _PM_InternalSegmentStop = PM_INTERNAL_SEGMENT_STOP_DEFAULT;
.global         _PM_InternalSegmentStop;

.var            _DM_InternalProgramStart = DM_PROGRAM_INTERNAL_START_DEFAULT;
.global         _DM_InternalProgramStart;

.var            _DM_InternalProgramStop = DM_PROGRAM_INTERNAL_STOP_DEFAULT;
.global         _DM_InternalProgramStop;

.var            _DM_InternalStart = DM_INTERNAL_START_DEFAULT;
.global         _DM_InternalStart;

.var            _DM_InternalStop = DM_INTERNAL_STOP_DEFAULT;
.global         _DM_InternalStop;

.var            _PM_ExternalSegmentStart = PM_EXTERNAL_SEGMENT_START_DEFAULT;
.global         _PM_ExternalSegmentStart;

.var            _PM_ExternalSegmentStop = PM_EXTERNAL_SEGMENT_STOP_DEFAULT;
.global         _PM_ExternalSegmentStop;

.var            _DM_ExternalStart = DM_EXTERNAL_START_DEFAULT;
.global         _DM_ExternalStart;

.var            _DM_ExternalStop = DM_EXTERNAL_STOP_DEFAULT;
.global         _DM_ExternalStop;

.endseg;




.SEGMENT/PM		seg_rth;


/* The loader begins with the interrupts up to and including the low 	*/
/* priority timer interrupt.											*/
				NOP;
				NOP;				
				NOP;				
				NOP;				
___lib_RSTI:	nop;
				JUMP ___lib_start;
				NOP;
				NOP;
/* Illegal Input condition*/
___lib_IICDI:	RTI;
				NOP;
				NOP;
				NOP;
/* Vector for status stack/loop stack overflow or PC stack full:*/
___lib_SOVFI:	RTI;
				NOP;
				NOP;
				NOP;
/* Vector for high priority timer interrupt:*/
___lib_TMZHI:	RTI;
				NOP;
				NOP;
				NOP;
/* Multiprocessor Vector Interrupt:*/
___lib_VIRPTI:	RTI;
				NOP;
				NOP;
				NOP;
/* IRQ2 asserted:*/
___lib_IRQ2I:	RTI;
				NOP;
				NOP;
				NOP;
/* IRQ1 asserted:*/
___lib_IRQ1I:	RTI;
				NOP;
				NOP;
				NOP;
/* IRQ0 asserted:*/
___lib_IRQ0I:	RTI;	
				NOP;
				NOP;
				NOP;
/* Reserved */	
				NOP;
				NOP;
				NOP;
				NOP;
/* SPORT0 DMA:*/
___lib_SP0I:	call _SPR0_int;	//Rx Interrupt from ext. TDM (SPORT02)
				RTI;
				NOP;
				NOP;
/* SPORT1 DMA:*/
___lib_SP1I:	call _SPR1_int;	//Rx Interrupt from int. TDM (SPORT13)
				RTI;
				NOP;
				NOP;
/* SPORT2 DMA:*/
___lib_SP2I:	RTI;
                NOP;
				NOP;
				NOP;
/* SPORT3 DMA:*/
___lib_SP3I:	RTI;
                NOP;
				NOP;
				NOP;
/* Link Buffer 0 DMA Interrupt*/
___lib_LP0I:	RTI;
                NOP;
				NOP;
				NOP;
/* Link Buffer 1 DMA Interrupt */
___lib_LP1I:	RTI;
                NOP;
				NOP;
				NOP;
/* SPI Receive DMA Interrupt:*/
___lib_SPIRI:	RTI;
				NOP;
				NOP;
				NOP;
/* SPI Transmit DMA Interrupt:*/
___lib_SPITI:	RTI;
				NOP;
				NOP;
				NOP;
/* Reserved */	
				RTI;
				NOP;
				NOP;
				NOP;
/* Reserved */	
				RTI;
				NOP;
				NOP;
				NOP;
/* DMA Channel 10 - Ext. Port Buffer 0:*/
___lib_EP0I:	RTI;
				NOP;
				NOP;
				NOP;
/* DMA Channel 11 - Ext. Port Buffer 1:*/
___lib_EP1I:	RTI;
				NOP;
				NOP;
				NOP;
/* DMA Channel 12 - Ext. Port Buffer 2:*/
___lib_EP2I:	RTI;
				NOP;
				NOP;
				NOP;
/* DMA Channel 13 - Ext. Port Buffer 3:*/
___lib_EP3I:	RTI;
				NOP;
				NOP;
				NOP;
/* Link Port Service Request:*/
___lib_LSRQI:	RTI;
				NOP;
				NOP;
				NOP;
/* Circular Buffer 7 overflow:*/
___lib_CB7I:	RTI;
				NOP;
				NOP;
				NOP;
/* Circular Buffer 15 overflow:*/
___lib_CB15I:	RTI;
				NOP;
				NOP;
				NOP;
/* Timer=0 (low priority option):*/
___lib_TMZLI:	call _timer_int;
				RTI;
				NOP;
				NOP;
/* Fixed-Point overflow exception:*/
___lib_FIXI:	RTI;
				NOP;
				NOP;
				NOP;
/* Floating-Point overflow exception:*/
___lib_FLTOI:	RTI;
				NOP;
				NOP;
				NOP;
/* Floating-Point underflow exception:*/
___lib_FLTUI:	RTI;
				NOP;
				NOP;
				NOP;
/* Floating-Point invalid exception:*/
___lib_FLTII:	RTI;
				NOP;
				NOP;
				NOP;
/* User software interrupt 0:*/
___lib_SFT0I:	RTI;
				NOP;
				NOP;
				NOP;
/* User software interrupt 1:*/
___lib_SFT1I:	RTI;
				NOP;
				NOP;
				NOP;
/* User software interrupt 2:*/
___lib_SFT2I:	RTI;
				NOP;
				NOP;
				NOP;
/* User software interrupt 3:*/
___lib_SFT3I:	RTI;
				NOP;
				NOP;
				NOP;
/* Reserved */	RTI;
				NOP;
				NOP;
				NOP;

				
// Init from Library:
___lib_start:	call ___lib_setup_c;
				jump _main;

___lib_prog_term:
				pm(__done_execution)=pc;
				idle;
				jump ___lib_prog_term;
___lib_prog_term.end:				

__done_execution:
				nop;
				nop;
				jump __done_execution;

.endseg;