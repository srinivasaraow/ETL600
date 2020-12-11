#define P4LT
//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      adsp_init.asm
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

#include <def21161.h>
#include <asm_sprt.h>

#include "../../h/p4lt/Constant.h"

.global _Init_OFDM_DSP_P4LT;

.segment/pm seg_pmco;

//---------------------------------------------------------------------------
_Init_OFDM_DSP_P4LT:

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
/*        
        ustat1=dm(WAIT);
		bit clr ustat1 0xffffffe0;	// clear MS0 waitstate and mode
		dm(WAIT)=ustat1;
*/
		// Configure WAIT register for FPGA loading
        r0 = RBWST7|RBWA0|EB3WS1|EB3S1|EB0WS0|EB0A0;
        dm(WAIT)=r0;
				

//		ustat1=0x1000;			    // refresh rate
        ustat1=0x5F7;
		dm(SDRDIV)=ustat1;
		
		ustat1 = dm(SYSCON);        // enable 48 bit SDRAM word
		bit clr ustat1 IPACK0 | IPACK1;
		bit set ustat1 IPACK0;
		dm(SYSCON) = ustat1;

        // Configure SDRAM controller
		bit clr ustat1 0xffffffff;
//		bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS3|SDCL2;
        bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS5|DSDCK1|SDCL2;		
		dm(SDCTL)=ustat1;

	    // Config FLAG0...FLAG3 as inputs for SPI interface
	    bit clr MODE2 FLG0O | FLG1O | FLG2O | FLG3O; 
		
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
	    	    
		// Enable interrupts
//        bit set MODE1 IRPTEN | ALUSAT | TRUNCATE | NESTM;  // configure mode1 register
        bit set MODE1 IRPTEN | NESTM;  // configure mode1 register
        
		// Configure WAIT register
//        r0=0x01C80000;	            // 4 Waitstates in asynchronous mode
//        dm(WAIT)=r0;
                
		leaf_exit;
_Init_OFDM_DSP_P4LT.end:

.endseg;

