/*-------------------------------------------------------------------------*
 * $Workfile: 161_spi.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Buhl, Michael PTUKT2
 * Remarks      :
 * Purpose      : Bootloader kernel for SPI 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/DSP_Boot_Loader/SPI_Boot/161_spi.asm $
 * 
 * 1     19.01.05 15:30 Chchlee
 * 
 * 1     11.07.04 18:28 Buhl01
 * initial
*-------------------------------------------------------------------------*/


/* ==============================================================================

		161_spi.asm

		Serial Peripheral Interface compatible port based boot loader for the
		ADSP21161 processor

		Copyright (c) 2002 Analog Devices Inc.  All rights reserved.

		version 0.1 6/21/01 - Zero initializations and external memory sections
		have not been tested.  Check for newest version of the SPI kernel at
		ftp.analog.com/pub/dsp/211xx/dev_tools/loader.

		version 0.2 8/1/01 - All sections of kernel have been tested.

		version 1.0 10/17/01 - R14 = 0xA001F81; was improperly initialized to R14 = 0xA801F81;
		in the previous version of the kernel.

		version 1.1 1/14/02 - changed some comments and fixed wait state init in SDRAM init section

		version 2.0 05.Sept.02  MSW
		    - Added initialization of all external memory types
		    - Added code to patch SPI IVT entry in final init
		    - Added final-init comments
		    - Added comments regarding tag info for each section

	    version 2.1 17.Oct.02 MSW
	        - Added comments regarding spi overflow errors possible during zero
	          initialization sections
	        - Added code to trap on spi overflow


	    version 2.2 20.DEC.2002 - M.Walsh
            -Changed #define SDRAM 1 to #define SDRAM 0

        version 2.3 17.JAN.2003 - M.WALSH
            -Updated FINAL_INIT comments

==============================================================================*/


#include <def21161.h>

.SECTION/PM	seg_ldr;

		NOP;NOP;NOP;NOP;		// Reserved interrupt

reset: 	nop;	// - Instruction specified here is never executed.  The core implicitly executes
                // an IDLE instruction here until 256 instructions are booted.  Once the kernel
                // is booted, execution resumes at 0x40040 (the SPI Rx ISR).  This address
                // (0x40005) and the next (0x40006) are also used as a temporary buffer to DMA-in
                // words from the SPI port

        nop;	// scratch memory2

//turn off SPI dma while kernel is executing
		ustat1=dm(SPICTL);
		bit clr ustat1 RDMAEN;	//clear DMA bit in SPICTL
		dm(SPICTL)=ustat1;

        call user_init;         // call subroutine at end of kernel in which users can insert
                                // initalization code, such as SDRAM, waitstates, etc.

        R5=2;                   // used to decrement tag by two during loading.

		L0=0; L1=0; L4=0;	    // Zero out L-registers so they	can be used without wrap
		L8=0; L12=0;L15=0;    	// ie. no circular buffering

		M5=0;M6=1;		        // Setup M-registers to use for various memory transfers
		M13=0; M14=1; M15=3;    // use to load count in read_dma_info

		I15 = 0x40006;			// scratch DMA destination address for 32 bit words;
		R14 = 0xA001F83;		// sets up and enables SPI port with DMA, this is also the default setting

		R11=DM(SYSCON);			// Read current SYSCON setting
		R12=PASS R11;			// Hold Initial SYSCON setting
		I12 = SYSCON;			// Address of SYSCON



// ==========================  READ_BOOT_INFO  =======================================
// Places TAG in R8, Word Count in R2, and Destination Address in R3
// -----------------------------------------------------------------------------------
read_boot_info:
        call read_dma_info;     // place two 48-bit words into 0x40004 and 0x40005
                                // equivalent to placing three 32-bit words into 0x40006,7,& 8
		R2=dm(0x40006);		//r2 passes section length
		R3=dm(0x40007);		//r3 passes destination address
		R8=dm(0x40008);     //r8 passes data-type tag
// -----------------------------------------------------------------------------------



// ========================== SPI Overflow Detect ====================================
// Monitor the SPI Status register to ensure that no overflow has ocurred.  The RBSY
// (receiver busy) bit in SPI is set to 1 when the SPI port receive buffer is full and
// another word is clocked into the shift register.  This may happen if the core is
// performing a zero-initialization section, and the SPI-master device continues to
// send data. RBSY is a sticky bit, so it will stay set until the spi port is
// disabled.
//
// If an overflow is detected the kernel will trap in an infinite loop so that user
// intervention can determine the problem.  If this problem occurs, the SPI-master
// device must be configured to pause while the DSP performs the zero initializations.
// This can be done in software by looking at the .LDR section headers, or in hardware
// by configuring the kernel to set a DSP flag pin while it is performing zero-
// initializations. More information can be found in an EE note on the ADI website
// titled "21161 SPI Booting".  http://www.analog.com/dsp
// -----------------------------------------------------------------------------------
        USTAT1=DM(SPISTAT);
        bit tst USTAT1 RBSY;
        if tf JUMP (pc,0);
        jump start_loader;
// -----------------------------------------------------------------------------------




// ============================= FINAL_INIT =========================================
// This code is the very last piece run in the kernel.  It uses a DMA to initialize the
// 256 instructions that reside in the interrupt vector table.
// -----------------------------------------------------------------------------------
final_init:	R9=0xb16b0000;          /* Load instruction PM(0,I8)=PX; 	*/

			R11=BSET R11 BY 9;      /* Set IMDW to 1 for inst write  	*/
			DM(SYSCON)=R11;         /* Setup for instruction write     	*/

			I4=0x40004;            /* Point to 0x040004 for self modifing code*/

            PX=pm(0x40004);         /* Load instruction to be patched into 0x40040*/
			I8=0x40040;            /* Point to DMA8 vector to patch 	*/

			R9=pass R9, R11=R12;	/* Clear AZ, hold initial SYSCON 	*/
			DO reset UNTIL EQ;      /* set top of loop value to 0x40004 (see comments below)*/
			PCSTK=0x40004;			/* Set top of PC-stack, so RTI after final DMA goes to 0x40004;*/

            //setup DMA parameters
			            DM(IMSRX)=M6;            /* Set to increment internal ptr 	*/
            R1=0x40000; DM(IISRX)=R1;             /* Setup DMA to load over ldr   	*/
			R2=0x180;   DM(CSRX)=R2;				/* Load internal count				*/

			bit clr LIRPTL SPIRI;	    // clear pending SPIRX interrupt latch 	*/
			bit set IMASK LPISUMI;		// enable LPISUMI interrupt
			bit set LIRPTL SPIRMSK;		// enable SPI receive interrupt
            bit set mode1 IRPTEN;       // enable global interrupts

            FLUSH CACHE;			/* Flush cache for kernel overwrite	*/

            DM(SPICTL)=R14;			/* Start DMA transfer				*/

            JUMP 0x40004 (DB);      /* Jump to start                	*/
				IDLE;					/* After IDLE, patch then start		*/
                IMASK=0;                // clear IMASK on way to 0x40004

// When this final DMA completes, IDLE in the delayed branch completes, IMASK is cleared,
// and then the DSP executes the instruction at 0x40040. The loader automatically places an RTI
// at that address (the user-intended instruction for 0x40040 is saved in the PX register). Executing
// the RTI pops the PC-stack which now holds 0x40004.
//
// The loader places the following instruction at 0x40005:
//
//          R0=R0-R0, DM(I4,M5)=R9, PM(I12,M13)=R11;   //opcode: x39732D802000
//
// Executing this instruction performs the three things:
// 1) It sets the AN flag ("until EQ" now evaluates as true)
// 2) It restores the power-up value of SYSCON (held in R11).
// 3) It overwrites itself with the instruction PM(0,I8)=PX; (opcode for which is held in R9)
//
// Because the termination condition (EQ) is achieved within the last three instructions
// of the loop, the DSP jumps to the top-of-loop address one last time.  We manually changed this
// top-of-loop address (PCSTK) to x40004, so, to conclude the kernel, the DSP executes the instruction
// at 0x40004 again.  The new instruction there resets the DMA10 interrupt vector (x40050) to it's
// user-intended final value. At this point, the kernel is finished, and execution continues at
// 0x40005 as if nothing happened!
//
// ========================== END FINAL_INIT ==============================




// ==========================================================================
// ========================== SPI Recieve DMA ISR ===========================
// This code must resolve to 0x40 in the interrupt vector table. If any code
// is added or removed prior to this without padding with "NOP's", then this
// kernel will not function.
            nop;nop;nop;nop;// added nop's to resolve RTI to correct address
            nop;nop;
x40040:     nop; RTI;
// ========================== END SPI Recieve DMA ISR ======================



// =============================  START LOAD   ========================================
// The following instructions analyze the TAG for each data section and initialize
// accordingly.  READ_BOOT_INFO places the TAG in R8, Word Count in R2, and Destination
// Address in R3.
//
// Notes:
// - Several tags are initialized with common code to save space.
// - These instructions must execute sequentially because the tag value
// is decremented after each false test.
// -----------------------------------------------------------------------------------
start_loader:
                R0=PASS R8;
                IF EQ jump final_init;  // jump if tag = 0

//--------------- 1 0x0001 ZERO DM16 ---------------
//--------------- 2 0x0002 ZERO DM32 ---------------
test_dm_zero:	R0=R0-R5;   	        // R5=2
			    IF GT JUMP (PC, test_dm40_zero);
dm_zero:		R0=R0-R0, I0=R3;        //clears r0 and sets I0 to R3 which is the address information
    			LCNTR=R2, DO dm_zero.end UNTIL LCE;
dm_zero.end:		DM(I0,M6)=R0; 	    // writes zeros to dm memory; M6=1
    			JUMP read_boot_info;
// ---------------------------------------------------


//--------------- 3 0x0003 ZERO DM40 ---------------
test_dm40_zero: R0=R0-1;	// checks for tag zero dm40
                IF NE JUMP (PC, test_dm_init);
dm40_zero:		PX1=0;
    			PX2=0;	// clears px
	    		R0=R0-R0, I0=R3;
		    	LCNTR=R2, DO dm40_zero.end UNTIL LCE;
dm40_zero.end:		DM(I0,M6)=PX;	// writes px out to correct memory locations
    			JUMP read_boot_info;
// -------------------------------------------------------


//--------------- 4 0x0004 INIT DM16 ---------------
//--------------- 5 0x0005 INIT DM32 ---------------
test_dm_init:   R0=R0-r5;	// checks for tag init dm16
			    IF GT JUMP (PC, test_dm40_init);
dm_init:        B0=R3;
    			LCNTR=R2, DO dm16_init.end UNTIL LCE;
	        	CALL read_SPI_word;
				DM(I0,M6)=R8;
				NOP;
dm16_init.end:	NOP; //a call can't be in the last 3 instructions
    			JUMP read_boot_info;
// -------------------------------------------------------


//--------------- 6 0x0006 INIT DM40 ---------------
test_dm40_init: R0=R0-1;	// checks for tag init dm40
    			IF NE JUMP (PC, test_pm_zero);
dm40_init:		B0=R3;
	    		LCNTR=R2, DO dm40_init.end UNTIL LCE;
					CALL read_dma_info;
			        PX=dm(0x40004);
			        dm(i0,M6)=PX;
			        PX=dm(0x40005);
dm40_init.end:		dm(i0,M6)=PX;
    			JUMP read_boot_info;
// -------------------------------------------------------


//--------------- 7 0x0007 ZERO PM16 ---------------
//--------------- 8 0x0008 ZERO PM32 ---------------
//--------------- 9 0x0009 ZERO PM40 ---------------
//--------------- 10 0x000A ZERO PM48 ---------------
test_pm_zero:   R0=R0-R5;	// R5=2, checks for tag zero pm16, zero pm32
                    IF LE JUMP (PC, dm_zero);
                R0=R0-R5;	// R5=2, checks for tag zero pm40, zero 48
        			IF LE JUMP (PC, dm40_zero);
// -------------------------------------------------------


//--------------- 11 0x000B INIT PM16 ---------------
//--------------- 12 0x000C INIT PM32 ---------------
//--------------- 13 0x000D INIT PM40 ---------------
//--------------- 14 0x000E INIT PM48 ---------------
test_pm_init:   R0=R0-R5;	// R5=2, checks for tag init pm16, init pm32
        		    IF LE JUMP (PC, dm_init);
                R0=R0-R5;	// R5=2, checks for tag init pm40, init pm48
        			IF LE JUMP (PC, dm40_init);
// -------------------------------------------------------


//--------------- 15 0x000F ZERO DM64 ---------------
test_dm64_zero: R0=R0-1;
                IF NE JUMP (PC, test_dm64_init);
dm64_zero:      R2=R2+R2;               //double count and initialize in NW memory
                R3 = LSHIFT R3 by 1;    //double longword address to NW memory
                jump dm_zero;
//------------------------------------------------------------


//--------------- 16 0x0010 INIT DM64 ---------------
test_dm64_init: R0=R0-1;
				IF NE JUMP (PC, test_pm64_zero);
dm64_init:		I0=R3;
				LCNTR=R2, DO dm64_init.end UNTIL LCE;
					CALL read_SPI_word;		/* read 32 lsbs of 64-bit word	*/
					PX2=R8;
					CALL read_SPI_word;		/* read 32 msbs of 64-bit word	*/
					PX1=R8;
					nop;
					nop;						/* Call cannot be in last 3 instructions of loop	*/
dm64_init.end:		DM(I0,M6)=PX (lw);			/* write all 64-bits to longword address location	*/
				JUMP read_boot_info;
//------------------------------------------------------------


//--------------- 17 0x0011 ZERO PM64 ---------------
//--------------- 18 0x0012 INIT PM64 ---------------
test_pm64_zero: R0=R0-1;
                IF EQ JUMP (PC, dm64_zero);
test_pm64_init: R0=R0-1;
                IF EQ JUMP (PC, dm64_init);
//------------------------------------------------------------



//=================================================================================
// "INIT PMx EXT" load "packed" instructions into external memory.  External memory
// starts at address 0x200000 on the 21161. The format for this packing can be found
// in the Chapter 5 of the 21161 Hardware Reference.  Zero-init code sections (many
// consecutive NOP's) are initialized using the EXT_ZERO_CODE subroutine above.
//----------------------------------------------------------------------------------

//--------------- 19 0x0013 INIT PM8 EXT ---------------
test_pm8_init_ext:
                R0=R0-1; //checks for init pm8 ext tag
                    IF NE JUMP (PC, test_pm16_init_ext);

        		B0=R3;
		        R2=LSHIFT R2 by -1; //r2 was # 32-bit words -> two inits per loop, so half R2
            	LCNTR=R2, DO pm8_init.end UNTIL LCE;

           		CALL read_SPI_word; // r8 = B1 B2 B3 B4
                    lcntr=4, do loop8 until lce;
            		r8=rot r8 by 8; // rotate the 32 word left by 8 bits
loop8:	    	    DM(I0,M6)=R8;   // to write out MSB (B1) first

                CALL read_SPI_word; // r8 = B0 B0 B5 B6
                    DM(1,I0)=R8;        // write out B6 to address n+1
                    R8=rot R8 by -8;
pm8_init.end:		DM(I0,4)=R8;        // write out B5 to address N, and then leave two
                                        // blank addresses to comply w/ 8-bit packing convention.
                JUMP read_boot_info;
//------------------------------------------------------------


//--------------- 20 0X0014 INIT PM16 EXT ---------------
test_pm16_init_ext:
                R0=R0-1; //checks for init pm16 ext tag
                IF NE JUMP (PC, test_pm32_init_ext);
            	B0=R3;
            	R2=LSHIFT R2 by -1;
            	LCNTR=R2, DO pm16_init.end UNTIL LCE;
            		CALL read_SPI_word; // r8 = B1 B2 B3 B4
            		DM(1,I0)=R8;        // write out b3, b4
            		R8=LSHIFT R8 by -16;
            		DM(I0,2)=R8;        // write out b1, b2
            		CALL read_SPI_word; // r8 = B0 B0 B5 B6
            		DM(I0,2)=R8;       // write out B5, b6, leave 1 black address for 16-bit packing
            		nop;
pm16_init.end:		nop; // call can't be in last 3 instructions of loop
            	JUMP read_boot_info;
//------------------------------------------------------------


//--------------- 21 0X0015 INIT PM32 EXT ---------------
test_pm32_init_ext:
                R0=R0-1;
				IF NE JUMP (PC, test_pm48_init_ext);
        		B0=R3;
        		LCNTR=R2, DO pm32_init.end UNTIL LCE;
            		CALL read_SPI_word;// r8 = B1 B2 B3 B4, then R8 = B0 B0 B5 B6
            		DM(I0,M6)=R8;
                  	nop;
pm32_init.end:      nop; // a call can't be in the last 3 instructions
            	JUMP read_boot_info;
//------------------------------------------------------------


//--------------- 22 0X0016 INIT PM48 EXT ---------------
test_pm48_init_ext:
                R0=R0-1;	//checks for init pm48 ext tag
                IF NE JUMP (PC, ext_zero_code);

                // configure External port for 48-bit writes (disable linkports)
    			ustat1=dm(SYSCON);
        	    bit set ustat1 IPACK0;
    			dm(SYSCON)=ustat1;

                I8=R3;
            	lcntr=R2, do pm48_init.end until lce;
            		CALL read_dma_info; // get 2 instructions (three 32-bit words)
                    I6=0x40004;
        		   	LCNTR=0x2, do loop1 until lce;
        				PX=DM(I6,M6); // read word from internal scratch locations
        				R0=PX1;	        // swap px registers for external write
        				PX1=PX2;        // (see pag 5-13 of the 161 HW-Ref Rev3.0)
        				PX2=R0;
loop1:	                PM(I8,M14)=px;	//write word to external memory
pm48_init.end:      nop; 	 	//nested loops can't terminate on the same instruction
                jump read_boot_info;
//===========================================================================



//============================= EXT_ZERO_CODE =============================
// This portion of code is used to initialize external memory with zero's
//  r0 holds (tag - 22)
//  r2 holds number of external #NOP's to be initialized
//
//  calculate how many external addresses are used by each NOP based on the tag:
//
//      If  pm8 (tag: 23 0X0017 ZERO PM8  EXT), r0 = 1, so multiply count by 8   (lshift r2 by 3)
//      If pm16 (tag: 24 0X0018 ZERO PM16 EXT), r0 = 2, so multiply count by 4   (lshift r2 by 2)
//      If pm32 (tag: 25 0X0019 ZERO PM32 EXT), r0 = 3, so multiply count by 2   (lshift r2 by 1)
//      If pm48 (tag: 26 0X001A ZERO PM48 EXT), r0 = 4, so multiply count by 1   (lshift r2 by 0)
//------------------------------------------------------------
ext_zero_code: 	r6=4;
                r0=r6-r0, I0=R3;	// calculate amount to shift by based on (4 - tag)
				r2=lshift r2 by r0;	// r2 = number addresses per NOP
                LCNTR=R2, DO ext_zero_code.end UNTIL LCE;
ext_zero_code.end:      DM(I0,M6)=M13;          //m13=0
                JUMP read_boot_info;  //fetch header for next block
//===========================================================================



//============================= read_dma_info =============================
// This section performs a length 3, 32bit dma into scratch location
// that can be read out as 2 40 or 48 bit words
//------------------------------------------------------------
read_dma_info:
		DM(IMSRX)=M14;            	/* setup up modify to 1*/
		DM(CSRX)=M15;				/* Setup DMA count to be 3 ~ 3 32 bit words = 2 48 bit words  */
		jump more;

//============================= read_SPI_word =============================
//	This section reads ONE 32-bit spi word into a scratch location (0x40006)
//	and then also into register R8
//------------------------------------------------------------
read_SPI_word:
		DM(IMSRX)=M14;            	/* setup up modify to 1*/
		DM(CSRX)=M14;				/* Setup DMA count to be 1  */

more:   DM(IISRX)=I15;          	/* Setup DMA destination address; I15 should be 0x40006*/
    	ustat1=dm(SPICTL);
    	bit set ustat1 RDMAEN;
    	dm(SPICTL)=ustat1;


SPI_DMA_not_done_yet:               // Test SPI completion
		R1 = DMA8ST;
		R0 = DM(DMASTAT);
		R0 = R0 AND R1;
		IF NE jump SPI_DMA_not_done_yet;
    	ustat1=dm(SPICTL);
	    bit clr ustat1 RDMAEN;
    	dm(SPICTL)=ustat1;
        r8=dm(0x40006);
        rts;
//===========================================================================




// =============================  USER_INIT   ========================================
// If any registers need initialization before the kernel begins importing the application,
// they should be done so here.  Typically, a user would need to initialize SDRAM settings,
// but they may also modify WAIT, SYSCON, SPICTL, or LCTL to increase the speed of the boot
// process.
//
// This subroutine may include NO MORE THAN 48 instructions (in addition to the RTS). As
// modifications are made, "kernel.map" shows the updated number of remaining free locations
// under "Memory-Words-Unused".
// -----------------------------------------------------------------------------------
user_init:


    			ustat1 = 0xCC33DD44;	// for Test use	
				dm(MSGR6)= ustat1;

//          ~~~~~INSERT SYSTEM INITIALIZATION CODE HERE ~~~~



                RTS;                // return from user_init to loader_kernel_init
// ============================  END USER_INIT  =====================================