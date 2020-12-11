#include "../../h/board.h"
#if BOARD==1
#define DRS7
#else
#undef DRS7
#endif
//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      Init_OFDM_DSP.asm
//
// PROJECT:    P4LT system board of ETL600 on DRS7 board
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        29-AUG-2003
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
// HISTORY:    29.08.03 Dr.Vladimir Potapov (vgp), Initial Version
//             04.09.03 pm section renaming is done to meet VisualDSP30 SP1 
//                      runtime model requirements, vgp
//             10.10.03 DRS7 board FPGA support 
//             09.02.04 FPGA interrupt support, vgp
//             16.02.04 Mask out all interrupts from FPGA during FPGA init, vgp
//             04.05.04 Reliable initialization of ADSP-21161N-EZlite controller,vgp
//---------------------------------------------------------------------------
/* $Log: /branches/Source MAS/P4LT/OFDM_DSP/ASM/UTIL/Init_OFDM_DSP.asm $
 * 
 * 7     24.07.08 15:14 Chhamaa
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 6     30.09.04 10:05 Leeb02
 * 
 * 5     30.09.04 9:00 Leeb02
 * 
 * 4     30.09.04 8:27 Leeb02
 */

#include <def21161.h>
#include <asm_sprt.h>

#ifndef DRS7

#include "../../h/Constant.h"

.global _Init_OFDM_DSP;

.segment/pm seg_pmco;

//---------------------------------------------------------------------------
_Init_OFDM_DSP:

        leaf_entry;

        // disable interrupts
        bit clr MODE1 IRPTEN | NESTM;                                    // disable interrupts
        bit clr IMASK SP0I | SP2I | SP1I | SP3I;       
        
        // clear control SPORT registers 
        dm(CP1A)= m5;
        dm(CP3A)= m5;
        dm(SP13MCTL) = m5;
        dm(SPCTL1) = m5;
        dm(SPCTL3) = m5;
        dm(SPCTL0) = m5;
        dm(SPCTL2) = m5;

        // Configure WAIT register for FPGA loading
        r0 = RBWST7|RBWA0|EB3WS1|EB3S1|EB0WS0|EB0A0;
        dm(WAIT)=r0;
				

        ustat1=0x5F7;
		dm(SDRDIV)=ustat1;
		
		ustat1 = dm(SYSCON);        // enable 48 bit SDRAM word
		bit clr ustat1 IPACK0 | IPACK1;
		bit set ustat1 IPACK0;
		dm(SYSCON) = ustat1;

        // Configure SDRAM controller
		bit clr ustat1 0xffffffff;
        bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS5|DSDCK1|SDCL2;		
		dm(SDCTL)=ustat1;

	    // Config FLAG0...FLAG3 as inputs for SPI interface
//	    bit clr MODE2 FLG0O | FLG1O | FLG2O | FLG3O; 
// debug 2008 Phase Control, Flag1 und Flag2 als Output
bit clr MODE2 FLG0O | FLG3O; 
bit set MODE2 FLG1O | FLG2O; 
		
	    // configure FLAG[9..4] as output
	    r0 = FLG9O|FLG8O|FLG7O|FLG6O|FLG5O|FLG4O;
	    dm(IOFLAG) = r0;
	    nop;

	    r4 = dm(IOFLAG);
	    r0 = FLG9|FLG8|FLG6;
	    r4 = r4 or r0;
	    dm(IOFLAG) = r4;
	    nop;
	    
	    r4 = dm(IOFLAG);
	    r0 = ~(FLG7|FLG5|FLG4); 
	    r4 = r4 and r0;         // Mask to "Low" DS4000 Clock, DS4000 Data and enable DS4000 clock select line
	    dm(IOFLAG) = r4;
	    	    
		// Init mode1 register
        bit set MODE1 IRPTEN | NESTM | RND32 | CBUFEN;  // configure mode1 register
                        
		leaf_exit;
_Init_OFDM_DSP.end:

#else

#include "../../../analog/rdnfpga.h"
#include "../../../analog/rdnad6620.h"

.global _Init_DSP2_DRS7;
.segment/pm seg_pmco;

//---------------------------------------------------------------------------
_Init_DSP2_DRS7:

        leaf_entry;

        bit clr MODE1 IRPTEN | NESTM;                                    // disable interrupts
        bit clr IMASK IRQ0I | LPISUMI | EP1I | EP2I | SP1I | SP3I;       // disable IRQ0 for FPGA and link port interrupts
        bit clr LIRPTL LP0MSK | LP1MSK;                                  // disable interrupts from link ports 1 & 2
        
        ustat1=dm(WAIT);
		bit clr ustat1 0xffffffe0;	/*clear MS0 waitstate and mode*/
		dm(WAIT)=ustat1;

		ustat1=0x1000;			    /*refresh rate*/
		dm(SDRDIV)=ustat1;
		
		ustat1 = dm(SYSCON);
		bit clr ustat1 IPACK0 | IPACK1;
		dm(SYSCON) = ustat1;

	    bit clr ustat1 0xffffffff;
		// SDCTL = 0x02014231;
		// 1/1 CCLK, no SDRAM buffering option, 2 SDRAM banks
		// SDRAM mapped to bank 0 only, no self-refresh, page size 256 words
		// SDRAM powerup mode is prechrg, 8 CRB refs, and then mode reg set cmd
		// tRCD = 2 cycles, tRP=2 cycles, tRAS=3 cycles, SDCL=1 cycle                     
		// SDCLK0, SDCLK1, RAS, CAS and SDCLKE activated                          
		bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS3|SDCL2;
		dm(SDCTL)=ustat1;

		// Init mode1 register
        bit set MODE1 IRPTEN | NESTM | RND32 | CBUFEN;  // configure mode1 register

        ustat1 = 0x200000;
        dm(LCTL) = ustat1;                      // assigng LBUF0 to LP0 and LBUF1 to LP1
        
        bit set LIRPTL LP0MSK | LP1MSK;       // enable interrupts from link ports 1 & 2
        bit set IMASK  LPISUMI;        // enable IRQ0 for FPGA and link port interrupts

		leaf_exit;
_Init_DSP2_DRS7.end:

#endif

.endseg;

