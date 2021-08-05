/*-------------------------------------------------------------------------*
 * $Workfile: 161_prom_SDRAM_EMERG.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Buhl, Michael PTUKT2
 * Remarks      :
 * Purpose      : Boot Loader Kernel for PROM boot with SDRAM Settings
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/DSP_Boot_Loader/EMERGENCY_SDRAM/161_prom_SDRAM_EMERG.asm $
 * 
 * 4     20.10.05 18:52 Chmibuh
 * comment added
 * 
 * 3     19.04.05 18:33 Chmibuh
 * changed to fix start addresses
 * 
 * 2     20.01.05 17:35 Chmibuh
 * 
 * 1     19.01.05 15:30 Chchlee
 * 
 * 2     9.12.04 17:48 Chmibuh
 * Change logic of FLAG1
 * 
 * 1     7.12.04 18:06 Chmibuh
 * initial
 * 
 *-------------------------------------------------------------------------*/


/* -----------------------------------------------------------------------**

        161_prom.asm

        PROM based boot loader of the ADSP-21161 processor.

                This version supports initialization of "packed" instructions
                8, 16, 32 or 48-bit external memory.

        Copyright (c) 2001 Analog Devices Inc.  All rights reserved.
                6/06/01

        Modifications:
                11.FEB.02 - M.Walsh
                -Added conditional SDRAM initialization
                -Removed redundant initializations of registers
                -Added comments including tag information

                13.FEB.02 - M.Walsh
                -Added call to "user_init" subroutine
                -Relocated "read_prom_word" above EPB0I to add room for user init
                -Relocated "final_init" above EPB0I to add room for user init
                -Relocated "get_addr" above EPB0I to add room for user init
                -Relocated "read_boot_info" above EPB0I to add room for user init
                -Collected all the code for all eight external code init sections
                        into into this single source file 161_prom.asm, eliminating
                        the previous need for four kernel.DXE's
                -Optimized (for size) all four ext_code_init code sections
                        (code that processes tags 0x13, 0x14, 0x15, and 0x16)
                -Added a single ext_code_zero initialization routine that is used
                        to zero-out all four sections with ZERO_PM*_EXT tags.
                        External count is now derived from actual tag 0x17, 0X18,
                        0x19, and 0X1A.  IF TAGS EVER CHANGE, SO MUST THIS INIT-CODE .
                -Now handle PM16, DM16, PM32, and DM32 with single init routine
                -Now handle PM40, DM40, and PM48 with single init routine
                -Decrement tag value by 2 in some places to save instructions while
                        when checking for tag value.


                01.MAR.2002 - M.Walsh
                -Removed (unecessary) zeroing of EIEP0 and ECEP0 in loader_kernel_init
                        to make room for next optimization.
                -Added conditional debugging capability.  By #defining DEBUG, and then
                        rebuilding this kernel, the instruction "jump(pc,0);" is included
                        in user_init.  This will cause the kernel to be booted and
                        begin executing, but not complete.  If "Initial Reset on Startup"
                        is NOT enabled for your emulator session, you can attach
                        and debug the kernel.

                03.MAR.2002 - M.Walsh
                - Added code to final_init to clear IMASK after boot.

                04.MAR.2002 - M.Walsh
                -Added comments for last two modifications
                -Added explanation of final_init

                20.DEC.2002 - M.Walsh
                -Changed #define SDRAM 1 to #define SDRAM 0

                17.JAN.2003 - M.Walsh
                -Cleaned up a few comments

                10.MAR.2004 - Jing Li.
                -Changed "lcntr=4, do (pc, 2) until LCE;" to "lcntr=4, do pm8_init_ext_rot_end until LCE;".
                -Added label: "pm8_init_ext_rot_end"


IMPORTANT NOTES:
1) This kernel uses DMA Channel 10 to import data from the PROM.  The four
instructions that handle the associated interrupt MUST be located at 0x50 in
the interrupt vector table.

2) Unused interrupt vectors are used for additional loader kernel space,
due to the new TAGs added to the ADSP-21161.  This helps save space for
additional customization of the kernel by the user.  Out of the reset vector,
IMASK and LIRPTL are cleared to ensure that no other interrupts other than
the EP0I interrupt will be serviced
---------------------------------------------------------------------------*/

#include <def21161.h>
#define SDRAM 0

#define START_ADR_EMERGENCY		0x0080062A
#define START_ADR_NORMAL_FW		0x0081062A

.SECTION/PM seg_ldr;
                NOP;NOP;NOP;NOP;                        // reserved (emulator) interrupt
reset:          IDLE;                                   // Implicit IDLE instruction at 0x40004
                JUMP loader_kernel_init (DB);           // 1st instruction executed after reset
                IMASK = 0;                              // mask all interrupts, since we use unused vectors for kernel instructions to save space
                LIRPTL = 0;

loader_kernel_init:
                CALL user_init;         // before executing the kernel, execute user's system initialization code

                // Zero out L-registers so I-registers can be used without wrap
                L0=0; L4=0;             //DAG1
                L8=0;                   //DAG2

                //Init M-registers
                M5=0; M13=0;            //DAG1
                M6=1; M14=1;            //DAG2


                R8 = 2;                 // Used to decrement tag by two in test_dm16_zero
                R14 = 0x421;            // Used in read_prom_word to config and begin DMAC10
                R15 = 6;
                I4 = 0x40004 ;          // DMA destination address 0x40004 used as local scratch

                R12=DM(SYSCON);         // R12 = hold original SYSCON setting
                R11=BSET R12 BY 1;      // R11 ~ to set BSO bit for reading ROM
                R10=BCLR R12 BY 1;      // R10 ~ to clear BSO bit for ext write
                DM(SYSCON)=R10;         // Clear BSO bit for writing RAM

                DM(IMEP0)=M13;          // EPB0 Internal Modify = 0
                DM(EMEP0)=M14;          // EPB0 External Modify = 1

                BIT SET IMASK 0x8000;   // Enable EP0 interrupt
                BIT SET MODE1 0x1800;   // Enable interrupts and nesting


// ==========================  GET_ADDR  =============================================
// Test flag 9 to find boot address - emergency or normal application
// ---------------------------------------------------------------------------
get_addr: 		R0=1;
				ustat1 = DM(IOFLAG);	// read flag reg				
				R3 = START_ADR_EMERGENCY;	// preset addresses	
				R2 = START_ADR_NORMAL_FW;	// preset addresses
				bit tst ustat1 FLG9;	// test flag
				if tf R3 = R2;			// if flag is high use address of R2 = normal appl.
                DM(EIEP0)=R3;           
// ========================= END GET_ADDR  ===========================================


// ==========================  READ_BOOT_INFO  =======================================
// Places TAG in R0, Internal Count in R2, and Destination Address in R3
// -----------------------------------------------------------------------------------
read_boot_info: CALL read_prom_word;    // read first tag and count
                R0=PASS R2;             // r0 holds tag
                CALL read_prom_word;
                jump Start_Load;
// ========================= END READ_BOOT_INFO =======================================


// ==========================  read_prom_word  =======================================
// read_prom_word is a callable subroutine that DMA's in one 48-bit word from PROM.
// It places the MS 32-bits in R3 and the LS 16 (right justified) in R2.
// For example, given the 48-bit word 0x112233445566,
//        R2 holds 0x00005566
//        R3 holds 0x11223344
// -----------------------------------------------------------------------------------
read_prom_word: R13=DM(SYSCON);         // Save old value of SYSCON
                DM(SYSCON)=R11;         // Set BSO bit for ROM read
                DM(IIEP0)=I4;           // 0x40004 = DMA destination address
                DM(CEP0)=M14;           // 1 = DMA internal count
                DM(ECEP0)=R15;          // 1 = DMA external count
                DM(DMAC10)=R14;         // 0x421 Start DMA
                IDLE;                   // Wait for DMA to complete
                PX=PM(0x40004);         // Read word from scratch
                DM(SYSCON)=R13;         // Reset SYSCON to previous
                R2=PX1;                 // Copy PX values into DREGS
              RTS (DB);
                    R2=LSHIFT R2 by -16;    // Downshift 16 bits to adjust for the extension of PX for HH
read_prom_word.end: R2=PASS R2, R3=PX2;     // Test the length
// ======================== END read_prom_word =======================================





//============================= EXT_ZERO_CODE =============================
// This portion of code is used to initialize external memory with zero's
// r2 holds number of external #NOP's to be initialized
// r0 holds (tag - 22)
//      If  pm8, r0 = 1, multiply r2 by 8      (tag: 23 0X0017 ZERO PM8 EXT)
//      If pm16, r0 = 2, multiply r2 by 4      (tag: 24 0X0018 ZERO PM16 EXT
//      If pm32, r0 = 3, multiply r2 by 2      (tag: 25 0X0019 ZERO PM32 EXT
//      If pm48, r0 = 4, multiply r2 by 1      (tag: 26 0X001A ZERO PM48 EXT
//------------------------------------------------------------
ext_zero_code: 	r0=r5-r0, I0=R3;	// r5=4, r0=tag, I0=ext address
				r2=lshift r2 by r0;	// r2 = number addresses per NOP
                LCNTR=R2, DO ext_zero_code.end UNTIL LCE;
ext_zero_code.end:      DM(I0,M6)=M13;          //m13=0
                JUMP read_boot_info; //fetch header for next block
//===========================================================================



// ============================= FINAL_INIT =========================================
// This code is the last piece run in the kernel.  It uses a DMA to initialize the
// 256 instructions that reside in the interrupt vector table.
// -----------------------------------------------------------------------------------
final_init: R11=BSET R11 BY 9;      // Set IMDW to 1 for inst write
            DM(SYSCON)=R11;		    // Setup to read PROM, inst wrt

            I12 = SYSCON;           // point to SYSCON
           	R9=0xb16b0000;          // Load opcode for instruction "PM(0,I8)=PX"
            I8=0x40050;             // I8 = 1st address of EPOI in IVT
            R9=PASS R9, R11=R12;    // Clear AZ, restore power-up value of SYSCON into R11

    		FLUSH CACHE;            // flush kernel's instructions from cache

    //----------Initialize final DMA-------------
            DM(IMEP0)=M14;          // Set to increment internal ptr
                r0=0x40000;
            DM(IIEP0)=r0;           // 0x40000 setup DMA to load over ldr
                R1=0x100;
			DM(CEP0)=R1;          	// Load internal count
			    R4=R1*R15 (SSI);    // Compute external count r2=#instr's, r15=6
			DM(ECEP0)=R4;  			// Write external count
    //--------------------------------------------

            do reset until EQ;      // set 0x40004 as bottom-of-loop, top-of-loop set to current PC
    		PCSTK=0x40004;		    // change top-of-loop to 0x40004 (to execute from 0x40004 on loop wrap)


            DM(DMAC10)=R14;         // Kick-off DMA transfer
            JUMP 0x40004 (DB);      // Jump to start - 0x40004 pushed on PC stack - RTI; in ISR pops this value.
                IDLE;               // wait for final DMA to complete, before vectoring to ISR @ 0x40050
final_init.end: IMASK=0;            // clear IMASK on way to x40050

// When this final DMA completes, IDLE in the delayed branch completes, IMASK is cleared,
// and then the DSP executes the instruction at 0x40050. The loader automatically places an RTI
// at that address (the user-intended instruction for 0x40050 is saved in the PX register). Executing
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

nop;nop;nop;nop;



// ============================== EPO_ISR  =================================================
// DMA Channel 10 (EPB0) Complete. This code MUST resolve to 0x50 in the interrupt vector table.
// If any code is added or removed prior to this, the kernel will not function.
ep0i_at_0x50:   R2=DM(DMAC10);          // Get DMAC Control setting
                RTI (DB);
                R6=r6-r6;
                DM(DMAC10)=R6;          //  zeroed between uses.
// ============================= END  EP0_ISR  =========================================



// =============================  USER_INIT   ========================================
// If any registers need initialization before the kernel begins importing the application,
// they should be done so here.  Typically, a user would need to initialize SDRAM settings,
// but they may also modify WAIT, SYSCON, SPICTL, or LCTL to increase the speed of the boot
// process.
//
// This subroutine may include NO MORE THAN 67 instructions (in addition to the RTS). As
// modifications are made, "kernel.map" shows the updated number of remaining free locations
// under "Memory-Words-Unused".
// -----------------------------------------------------------------------------------
user_init:

        // ------------------ DEBUG BOOTING USING ICE? ----------------
        // Include this "trap" instruction by using the -D switch on the commandline when building
        // (for example: 	easm21k.exe -21161 -o 161_prom.doj 161_prom.asm -DDEBUG)
        //
        // Attach emulator/debugger and hit run.  While running, reset board to trigger the PROM boot.
        // Now, when you hit halt with the emulator, you'll be at the following instruction. Change
        // the instruction to a NOP; and you'll be able to single-step through the kernel.
        #ifdef DEBUG
        kdebug:         jump (pc,0);    // do not completely execute kernel - allows ICE to break in.
        #endif
        // (Note: to make debugging the kernel more simple, you can load the LABEL's using the
        // following TCL command in the IDDE console:
        //
        //                  dspload "C:\\<path>\\161_prom.DXE" -symbols
        //
        // --------------------------------------------------

//#if SDRAM
                r0=0x01C00000;      // WORKS (clear MSx waitstate and mode)
                dm(WAIT)=r0;
                r0=0x05F7;          // refresh rate
                dm(SDRDIV)=r0;
                r0=0x0231C25A;      // SDRAM running @ Core-clock
                dm(SDCTL)=r0;
//#endif


                RTS;                // return from user_init to loader_kernel_init
// ============================  END USER_INIT  =====================================



// =============================  START LOAD   ========================================
// The following instructions analyze the TAG for each data section and initialize
// accordingly.  READ_BOOT_INFO places the TAG in R0, the Word Count in R2,
// and the Destination Address in R3.
//
// Notes:
// - Several tags are initialized with common code to save space.
// - These instructions must execute sequentially because the tag value
// is decremented after each false test.
// -----------------------------------------------------------------------------------
Start_Load:


//--------------- 0 0x0000 FINAL INIT ---------------
                R0=PASS R0;
                IF EQ JUMP (PC, final_init);    // if tag = zero, execute final init
// ---------------------------------------------------


//--------------- 1 0x0001 ZERO DM16 ---------------
//--------------- 2 0x0002 ZERO DM32 ---------------
test_dm_zero:   R0=R0-R8;   //R8=2
                IF GT JUMP (PC, test_dm40_zero); //jump unless tag == 2 or tag==1
dm_zero:        I0=R3;  // i0 holds SW or NW address accordingly
                LCNTR=R2, DO dm_zero_loop UNTIL LCE;  //R2 holds number of SWs or NWs
dm_zero_loop:   DM(I0,M6)=M13;
                JUMP read_boot_info;
// ---------------------------------------------------

//--------------- 3 0x0003 ZERO DM40 ---------------
test_dm40_zero: R0=R0-1;
                IF NE JUMP (PC, test_dm_init);
dm40_zero:      PX=0;
                I0=R3;
                LCNTR=R2, DO dm40_zero.end UNTIL LCE;
dm40_zero.end:  DM(I0,M6)=PX;
                JUMP read_boot_info;    //fetch header for next block
// -------------------------------------------------------


//--------------- 4 0x0004 INIT DM16 ---------------
//--------------- 5 0x0005 INIT DM32 ---------------
test_dm_init:   R0=R0-r8;   // r8 = 2
                IF GT JUMP (PC, test_dm40_init);
dm_init:        I0=R3;
                LCNTR=R2, DO dm_init.end UNTIL LCE;
                CALL read_prom_word(DB);
                NOP; NOP;  //call cannot be in last 3 inst. of a loop
dm_init.end:    DM(I0,M6)=R3;
                JUMP read_boot_info;    //fetch header for next block
// -------------------------------------------------------


//--------------- 6 0x0006 INIT DM40 ---------------
test_dm40_init: R0=R0-1;
                IF NE JUMP (PC, test_pm_zero);
dm40_init:      I0=R3;
                LCNTR=R2, DO dm40_init.end UNTIL LCE;
                CALL read_prom_word (DB);
                NOP; //call cannot be in last 3 inst. of a loop
                NOP;
dm40_init.end:  DM(I0,M6)=PX;
                JUMP read_boot_info;    //fetch header for next block
// -------------------------------------------------------



//--------------- 7 0x0007 ZERO PM16 ---------------
//--------------- 8 0x0008 ZERO PM32 ---------------
//--------------- 9 0x0009 ZERO PM40 ---------------
//--------------- 10 0x000A ZERO PM48 ---------------
test_pm_zero:   R0=R0-R8;  //r8==2
                IF LE JUMP (PC, dm_zero);     // jump if tag==7 or tag==8
                R0=R0-R8;  //r8==2
                IF LE JUMP (PC, dm40_zero);     // jump if tag==9 or tag==10


//--------------- 11 0x000B INIT PM16 ---------------
//--------------- 12 0x000C INIT PM32 ---------------
//--------------- 13 0x000D INIT PM40 ---------------
//--------------- 14 0x000E INIT PM48 ---------------
test_pm_init:   R0=R0-R8;  //r8==2
                IF LE JUMP (PC, dm_init);     //jump if tag==11 or 12
                R0=R0-R8;
                IF LE JUMP (PC, dm40_init);   //jump if tag==13 or 14
//------------------------------------------------------------


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
dm64_init:      I0=R3;
                LCNTR=R2, DO dm64_init.end UNTIL LCE;
                CALL read_prom_word;    // read 32 lsbs of 64-bit word
                R10=PX2;                // save 32 lsbs of 64-bit word
                CALL read_prom_word;    // read 32 msbs of 64-bit word
                PX1=R10;                // restore 32 lsbs to PX1
                nop;                    // Call cannot be in last 3 instructions of end
dm64_init.end:  DM(I0,M6)=PX (lw);      // write all 64-bits to longword address location
                JUMP read_boot_info;    //fetch header for next block
//------------------------------------------------------------


//--------------- 17 0x0011 ZERO PM64 ---------------
//--------------- 18 0x0012 INIT PM64 ---------------
test_pm64_zero: R0=R0-1;
                IF EQ JUMP (PC, dm64_zero);
test_pm64_init: R0=R0-1;
                IF EQ JUMP (PC, dm64_init);
//------------------------------------------------------------



//----------------------------------------------------------------------------------
// "INIT PMx EXT" load "packed" instructions into external memory.  External memory
// starts at address 0x200000 on the 21161. The format for this packing can be found
// in the Chapter 5 of the 21161 Hardware Reference.  Zero-init code sections (many
// consecutive NOP's) are initialized using the EXT_ZERO_CODE subroutine above.
//----------------------------------------------------------------------------------

//--------------- 19 0x0013 INIT PM8 EXT ---------------
test_pm8_initext:       R0=R0-1;
                        IF NE JUMP test_pm16_initext;
pm8_init_ext_code:      I0=R3;
                        LCNTR=R2, DO pm8_init_external.end UNTIL LCE;
                        CALL read_prom_word;    //r3= bits 47-16, r2= bits 15-0
                        lcntr=4, do pm8_init_ext_rot_end until LCE;
                        r3=rot r3 by 8;
pm8_init_ext_rot_end:   dm(i0,m0)= r3;  // write out 4 MSB's
                        r4= LSHIFT R2 by -8;
                        dm(i0,m0) = r4; // write out 8-15
pm8_init_external.end:  dm(i0,3) = r2;  // write out 7-0, leaving 2 uninitialized locations
                        JUMP read_boot_info;    //fetch header for next block

//--------------- 20 0X0014 INIT PM16 EXT ---------------
test_pm16_initext:      R0=R0-1;
                        IF NE JUMP test_pm32_initext;
pm16_init_ext_code:     I0=R3;
                        LCNTR=R2, DO pm16_init_external_loop UNTIL LCE;
                        CALL read_prom_word;
                        R5 = LSHIFT R3 BY -16;  //r3= bits 47-16, r2= bits 15-0
                        DM(I0,M6)=R5;
                        DM(I0,M6)=R3;   // "unpack" instruction opcode bits 31:16
pm16_init_external_loop:DM(I0,2)=R2;    // "unpack" instruction opcode bits 15:0, skip one location
                        JUMP read_boot_info;

//--------------- 21 0X0015 INIT PM32 EXT ---------------
test_pm32_initext:      R0=R0-r8;       // r8 = 2
                        IF EQ JUMP pm48_init_ext_code;
                        IF GT JUMP ext_zero_code(DB);   // jump if one of last 4 tags (all ext_zero_code)
                            r5=4;                       // used in ext_zero_code to determine addr's per nops
pm32_init_ext_code:     I0=R3;
                        LCNTR=R2, DO pm32_init_external_loop UNTIL LCE;
                        CALL read_prom_word;
                        DM(I0,M6)=R3;
                        DM(I0,M6)=R2;
pm32_init_external_loop:nop; // call can't be in last 3 inst. of loop
                        JUMP read_boot_info;

//--------------- 22 0X0016 INIT PM48 EXT ---------------
pm48_init_ext_code:     r0 = IPACK0;
                        r0 = r12 OR R0, I8=R3; // disable linkports&enable lower 16 lsbs
                        dm(SYSCON)=r0;
                        LCNTR=R2, DO pm48_init_external.end UNTIL LCE;
                        CALL read_prom_word;
                        R0=PX1;
                        PX1=PX2;
                        PX2=R0;
pm48_init_external.end: PM(I8,M14)=PX;
                        JUMP read_boot_info;
//=============================================================================
//========================= END 161_prom.ASM ===================================
