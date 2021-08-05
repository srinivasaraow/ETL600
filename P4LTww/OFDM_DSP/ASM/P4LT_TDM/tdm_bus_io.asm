#include "../../h/board.h"
#if BOARD==1
#define DRS7
#else
#undef DRS7
#endif

//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      dsp1_tdm_bus_io.asm
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        29-AUG-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    SPORT1 and SPORT3 configured in 3-wire TDM mode for master DSP on EZLite,
//             device driver routings 
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    29.08.03  Dr.Vladimir Potapov (vgp), Initial Version
//             31.08.03  Add verification of number of transmit and receive
//                       channels, vgp
//             04.09.03  pm section renaming is done to meet VisualDSP30 SP1 
//                       runtime model requirements, vgp
//             09.09.03  Master and Slave mode of SPORT02 operation is added, vgp
//             02.12.03  Revised version for DSP1 of DRS7 board
// HISTORY:    29.08.03  Dr.Vladimir Potapov (vgp), Initial Version
//             31.08.03  Add verification of number of transmit and receive
//                       channels, vgp
//             04.09.03  pm section renaming is done to meet VisualDSP30 SP1 
//                       runtime model requirements, vgp
//             09.09.03  Master and Slave mode of SPORT13 operation is added, vgp
//             02.12.03  Revised version for DSP2 of DRS7 board
//             30.01.04  Software correction of DRS7 board FPGA hardware anomaly in TDM bus 
//                       switch of data path transmission from DSP2 to DSP1 according to recommendation 
//                       as a result of ABB and TL Consulting GmbH meeting in Baden on January 30, 2004,  
//                       In data transmission direction from DSP1 to DSP2 DRS7 doesn't require additional 
//                       delay between Frame sync. signal and the first data bit in the frame,
//                       while in transmission direction from DSP2 to DSP1 FPGAs TDM switch requires 
//                       at least one additional CLKIN pulse which is specified by MFD1 parameter in 
//                       SPxyMCTL control register, 30.01.04, vgp
//             04.02.04  universal runtime configured tdm bus driver for Sport02 and Sport13, vgp
//                       
//
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/P4LT_TDM/tdm_bus_io.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 5     30.09.04 8:53 Leeb02
 * 
 * 4     30.09.04 8:27 Leeb02
 */	
#include <def21161.h>
#include <asm_sprt.h>

.global  _Init_TDM_Bus_IO;
.global  _Start_TDM_Bus_IO;
.global _Stop_TDM_Bus_IO;
.global _Get_TDM_Bus_Data_TX;
.global _Get_TDM_Bus_Data_RX;

.global _Flag_TDM02_Data_Received;
.global _Flag_TDM02_Data_Transmitted;
.global _Flag_TDM13_Data_Received;
.global _Flag_TDM13_Data_Transmitted;

/*---------------------------------------------------------------------------*/
.segment/dm    seg_dmda;

// define TDM port SPORT13
//

#define TDM13        // Sport13

#ifdef  DRS7
#define MFDx MFD1
#else
#define MFDx MFD0
#endif

// Early frame sync operating
// Definition of delay measured in CLKIN pulses between FS and the first data bit (FDB) in the first time slot 
// A value of zero for MFD causes the frame sync to be concurrent with the FDB. The maximum value
// allowed for MFD is 15. A new frame sync may occur before data from the last frame has been received, because
// blocks of data occur back to back (p. 10-59, ADSP21161N HWR).
//
// Restrictions on internal frame sync generation.
// Use a multichannel frame delay of at least one pulse when the DSP is generating frame syncs for the multichannel 
// system and the serial clock of the system is equal to CLKIN (the processor clock). If MFD is not set to at least
// one, the master DSP in a multiprocessing system does not recognize the first frame sync after multichannel operation 
// is enabled. All succeeding frame syncs are recognized normally (p. 10-59, ADSP21161N HWR).

/*****************************************************************************/
/*                                                                           */
/* TCB = "Transfer Control Block"                                            */
/*                                                                           */
/*   SPORTx TCB format:                                                      */
/*            ECx (length of destination buffer),                            */
/*            EMx (destination buffer step size),                            */
/*            EIx (destination buffer index (initialized to start address)), */
/*            GPx ("general purpose"),                                       */
/*            CPx ("Chain Point register"; points to last address (IIx) of   */
/*                                         next TCB to jump to               */
/*                                         upon completion of this TCB.),    */
/*            Cx  (length of source buffer),                                 */
/*            IMx (source buffer step size),                                 */
/*            IIx (source buffer index (initialized to start address))       */
/*                                                                           */
/*****************************************************************************/

//========== definitions ===================

#define TCB_LENGTH 8
#define MAX_CHANNELS 128
#define ECx 0
#define EMx 1
#define EIx 2
#define GPx 3
#define CPx 4
#define Cx  5
#define IMx 6
#define IIx 7

#define TX2_inf_tcb1   tx2a_tdm_tcb
#define TX2_inf_tcb2   tx2a_tdm_tcb + TCB_LENGTH

#define RX0_inf_tcb1   rx0a_tdm_tcb
#define RX0_inf_tcb2   rx0a_tdm_tcb + TCB_LENGTH

#define TX3_inf_tcb1   tx3a_tdm_tcb
#define TX3_inf_tcb2   tx3a_tdm_tcb + TCB_LENGTH

#define RX1_inf_tcb1   rx1a_tdm_tcb
#define RX1_inf_tcb2   rx1a_tdm_tcb + TCB_LENGTH

//========== end of definitions ============


/* SPORT0 receive tcb */
.var    rx0a_tdm_tcb[2*TCB_LENGTH] = 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0;
/* SPORT2 transmit tcb */
.var    tx2a_tdm_tcb[2*TCB_LENGTH] = 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0;
/* SPORT0 receive tcb */
.var    rx1a_tdm_tcb[2*TCB_LENGTH] = 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0;
/* SPORT2 transmit tcb */
.var    tx3a_tdm_tcb[2*TCB_LENGTH] = 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0;


.var _Flag_TDM02_Data_Received;
.var _Flag_TDM02_Data_Transmitted;
.var _Flag_TDM13_Data_Received;
.var _Flag_TDM13_Data_Transmitted;

.segment/pm 	seg_pmco;
	
/*********************************************************************************************************************************/
/*                                                                                                                               */
/* SPORT0&2 Control and DMA Registers  Programming 							  	                                                 */
/* Multichannel Mode chained dma, early fs, act hi fs, fall edge, no pack                 	                                     */
/* Fcclk = 100 MHz                                                                                                               */
/* CLKDIV = Fcclk/(2*Fsclk) - 1;                                                                                                 */
/* for Fsclk = 2 MHz                                                                                                             */
/* CLKDIV = 100/2/2 - 1 = 24                                                                                                     */
/* int Init_TDM_Bus_IO(int NCH,int SLEN,int RXMASK[4],int TXMASK[4], int *InBuf, int *OutBuf, int IsMaster, int IsSPort13)       */
/*                                                                                                                               */
/*********************************************************************************************************************************/

/*=================================================================================================================*/
/*Input:                                                                                                           */   
/*    NCH = r4 - number of time slots in tdm frame, more or equal than 1, not more than 128                        */
/*   SLEN = r8 - number of bits in time slot, more or equal than 3, but not more than 32                           */
/*                    (after entry will be SLEN=SLEN-1)                                                            */
/*  *RXMASK = r12 points 4-element array of 32-bit unsigned integers,                                              */
/*                   RXMASK[0] - enables receive channels  31- 0,                                                  */
/*                   RXMASK[1] - enables receive channels  63-32,                                                  */
/*                   RXMASK[2] - enables receive channels  95-64,                                                  */
/*                   RXMASK[3] - enables receive channels 127-96,                                                  */
/*                   Each bit, 31-0 set (=1) in one of the for RXMASK[0..3] integers corresponds to an active      */
/*                   receive channel, when a bit within element activates a channel, the serial port receives the  */
/*                   word of SLEN bits in that channel position of serial data stream and stores 32-bit word into  */
/*                   RX buffer <InBuf> right-justify, zero-fill unused MSBs. Bit #0 set in one of RXMASK[1]        */ 
/*                   activates time slot 32, bit #31 activates time slot 63.                                       */
/*                   The function automatically calculates the desired number of active receive time slots, by     */
/*                   calculating number of bits equal to 1 withing RXMASK[0..3] array                              */
/*  *TXMASK = reads(1) points 4-element array of 32-bit unsigned integers,                                         */
/*                   TXMASK[0] - enables transmit channels  31- 0,                                                 */
/*                   TXMASK[1] - enables transmit channels  63-32,                                                 */
/*                   TXMASK[2] - enables transmit channels  95-64,                                                 */
/*                   TXMASK[3] - enables transmit channels 127-96,                                                 */
/*                   Each bit, 31-0 set (=1) in one of the for TXMASK[0..3] integers corresponds to an active      */
/*                   transmit channel, when a bit within element activates a channel, the serial port transmits    */ 
/*                   the word of SLEN bits in that channel position of serial data stream from SLEN least          */
/*                    significant bits of 32-bit word loaded from TX buffer <OutBuf>.                              */ 
/*                   Bit #0 set in one of TXMASK[1] activates time slot 32, bit #31 activates time slot 63.        */
/*                   The function automatically calculates the desired number of active transmit time slots, by    */
/*                   calculating number of bits equal to 1 withing TXMASK[0..3] array                              */
/*                                                                                                                 */
/*  *InBuf = reads(2) - points input buffer of length 2*(active channels to receive) of 32-bit words, but not more */
/*                  than 256 words                                                                                 */
/*                                                                                                                 */
/*  *OutBuf = reads(3) - points output buffer of length 2*(active channels to transmit) of 32-bit words, but not   */
/*                  more than 256 words                                                                            */
/*  IsMaster = reads(4) - Master/Slave flag, if set to zero then SPORT02 or Sport13 will be set                    */
/*                        to Slave mode,i.e. serial clock and frame clock will be configured as inputs,            */
/*                        if set to 1 (master)then serial clock is input, while frame clock is output              */
/*                                                                                                                 */
/*  IsSPort13 = reads(5) - if one then selects SPort13, if 0 - SPort02                                             */
/*                                                                                                                 */
/*Output:                                                                                                          */
/*         returns 0 if success                                                                                    */ 
/*         returns 1 if faild                                                                                      */ 
/*=================================================================================================================*/

_Init_TDM_Bus_IO:

    leaf_entry;

    r0 = i0;  puts = r0;
    puts = r1;
    puts = r2;
    puts = r3;

    r0 = 1;                          // set err ret
    r1 = reads(5);                   // get IsSport13 flag
	r2 = lshift r1 by -1;            // test for 0/1
	if not sz jump ret;
	r0 = r0 - r0;                    // clear r0
	r1 = pass r1;
	if ne jump init13;               // jump if Sport13 is selected

    bit clr imask SP0I|SP2I;          // disable SPORT0 & SPORT2 interrupts 

    dm(_Flag_TDM02_Data_Received) = r0;    // clear RX flag of Sport 02
    dm(_Flag_TDM02_Data_Transmitted) = r0; // clear TX flag of Sport 02
      
    /* clear SPCTL_RX, SPCTL_TX and SPxxMCTL registers */
    dm(SPCTL0) = r0;                      // disable SPORT02 RX 
    dm(SPCTL2) = r0;                      // disable SPORT02 TX
    dm(SP02MCTL) = r0;                    // disable SPORT02 TDM mode

    /* disable DMA */
    dm(CP0A) = r0;              // disable DMA on SPORT0_RX 
    dm(CP2A) = r0;              // disable DMA on SPORT2_TX 
      
    /* clear SPORT0_RX & SPORT2_TX receive & transmit multichannel companding enable registers */
    /*                      MR0CC0-MR0CC3 and MT2CC0-MT2CC3  */
    dm(MR0CCS0) = r0;           // no companding on SPORT0 receive channels   31- 0 
    dm(MR0CCS1) = r0;           // no companding on SPORT0 receive channels   63-32 
    dm(MR0CCS2) = r0;           // no companding on SPORT0 receive channels   95-64 
    dm(MR0CCS3) = r0;           // no companding on SPORT0 receive channels  127-96 	
    dm(MT2CCS0) = r0;           // no companding on SPORT2 transmit channels  31- 0 
    dm(MT2CCS1) = r0;           // no companding on SPORT2 transmit channels  63-32 
    dm(MT2CCS2) = r0;           // no companding on SPORT2 transmit channels  95-64 
    dm(MT2CCS3) = r0;           // no companding on SPORT2 transmit channels 127-96 

	dm(DIV0) = r0;
	dm(DIV2) = r0;

    jump  ver_input;

init13:
    bit clr imask SP1I|SP3I;          // disable SPORT1 & SPORT3 interrupts 

    dm(_Flag_TDM13_Data_Received) = r0;    // clear RX flag of Sport 13
    dm(_Flag_TDM13_Data_Transmitted) = r0; // clear TX flag of Sport 13
      
    /* clear SPCTL1_RX, SPCTL3_TX and SP13MCTL registers */
    dm(SPCTL1) = r0;                      // disable SPORT13 RX 
    dm(SPCTL3) = r0;                      // disable SPORT13 TX
    dm(SP13MCTL) = r0;                    // disable SPORT13 TDM mode

    /* disable DMA */
    dm(CP1A) = r0;              // disable DMA on SPORT1_RX 
    dm(CP3A) = r0;              // disable DMA on SPORT3_TX 
      
    /* clear SPORT1_RX & SPORT3_TX receive & transmit multichannel companding enable registers */
    /*                      MR1CC0-MR1CC3 and MT3CC0-MT3CC3  */
    dm(MR1CCS0) = r0;           // no companding on SPORT1 receive channels   31- 0 
    dm(MR1CCS1) = r0;           // no companding on SPORT1 receive channels   63-32 
    dm(MR1CCS2) = r0;           // no companding on SPORT1 receive channels   95-64 
    dm(MR1CCS3) = r0;           // no companding on SPORT1 receive channels  127-96 	
    dm(MT3CCS0) = r0;           // no companding on SPORT3 transmit channels  31- 0 
    dm(MT3CCS1) = r0;           // no companding on SPORT3 transmit channels  63-32 
    dm(MT3CCS2) = r0;           // no companding on SPORT3 transmit channels  95-64 
    dm(MT3CCS3) = r0;           // no companding on SPORT3 transmit channels 127-96 

	dm(DIV1) = r0;
	dm(DIV3) = r0;

/* Verify input parameters          */
ver_input:
    r0 = 1;                       // set err ret
/*   1<=(NCH = r4)<=128             */
    r4 = r4 - 1;                  // NCH = NC-1
    r1 = 0xffffff80;              // r1 = ~0x7f
    r1 = r1 and r4;
    if ne jump ret;               // NCH > 128 or NCH<1

/*    3<=(SLEN = r8)<=32            */
    r8 = r8 - 1;                  // SLEN = SLEN-1
    r1 = 0xffffffe0;              // r1 = ~0x1f
    r1 = r1 and r8;
    if ne jump ret;               // SLEN > 32 or SLEN<1
    r1 = 2;
    r1 = r8 - r1;
    if lt jump ret;               // SLEN < 3

/*   read Master/Slave flag into r3 */
    r3 = reads(4);                // r3 = 0/1
	r2 = lshift r3 by -1;
	if not sz jump ret;
    
	r3 = pass r3;                 // if slave skip 
	if eq jump set_spmctl;
/* deposit sport 0 or 1 frame sync divide registers */
    r1 = r4*r8(uui);              // r1 = (NCH-1)(SLEN-1)
    r1 = r1 + r4;                 // r1 = NCH*SLEN -SLEN
    r1 = r1 + r8;                 // r1 = NCH*SLEN-1
    r1 = lshift r1 by 16;         // frame sync counter of internal FS generator
	r0 = reads(5);                // get SPORT13/SPORT02 flag
	r0 = pass r0;
	if eq jump set_div0;
    dm(DIV1) = r1;                // set FS DIV register of SPORT13 
	jump set_spmctl;
set_div0:
    dm(DIV0) = r1;                // set FS DIV register of SPORT02

    /* SPORT_RX and SPORT_TX operated in "multichannel" mode. */
    /* Deposit SPORT_RX and SPORT_TX Miscellaneous Control Bits Registers */
set_spmctl:
    r1 = lshift r4 by 5;          // r1 = (NCH-1)<<5
    r2 = MFDx;                    // if internal synch. MFD >= MFD1 !!!
    r1 = r1 or r2;
    r0 = pass r0;                 // test SPORT13/SPORT02 flag
	if eq jump set_spmctl02;
	dm(SP13MCTL) = r1;            // set MCRL register of SPORT13
    jump set_spctl;
set_spmctl02:
	dm(SP02MCTL) = r1;            // set MCRL register of SPORT02

    /* deposit SPORT_RX and SPORT_TX control registers */
set_spctl:
    r8 = lshift r8 by 4;          // shift left 4 bits sport word length 
	                              // to correct position in SPCTLx register
    r1 = SCHEN_A | SDEN_A;
    r1 = r1 or r8;                // add SLEN to control register, r1 = spctl of TX channel
    r2 = SCHEN_A | SDEN_A | IRFS; // mask for clock generation
	r2 = r2 or r8;                // r2 = spctl for RX channel in Master mode
    r3 = pass r3;                 // test Master/Slave flag
	if eq r2 = pass r1;           // copy spctl TX mask into RX
	r0 = pass r0;                 // test SPORT13/SPORT02 flag 
	if eq jump set_spctl02;
	dm(SPCTL1) = r2;              // Set SPORT1 RX control register
	dm(SPCTL3) = r1;              // Set SPORT3 TX control register
	jump set_mrmtcs;
set_spctl02:
	dm(SPCTL0) = r2;              // Set SPORT0 RX control register
	dm(SPCTL2) = r1;              // Set SPORT2 TX control register



    /* SPORT_RX & SPORT_TX receive and transmit multichannel word enable registers */
    // *RXMASK = r12                 */
    // *TXMASK = reads(1)            */
	// r1 and r2 destroided by Weight32() function
	// r12 contains number of active RX channels
set_mrmtcs:									
	r0 = pass r0;                 // test SPORT13/SPORT02 flag 
	if eq jump set_mrmtcs02;

//=====================================================================
//================              Sport13          ======================
//=====================================================================
/*  calculate number of RX Sport0 channels */

    i0 = r12;                      // i0 points RXMASK[0..3]
    r12 = r12 -r12, r0 = dm(i0,m6);
    dm(MR1CS0) = r0;               // enable receive channels  31- 0 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MR1CS1) = r0;               // enable receive channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MR1CS2) = r0;               // enable receive channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m5);
    dm(MR1CS3) = r0;               // enable receive channels  63-32 
    call Weight32;
    r12 = r12 + r1;                // r12 = number of active receive channels
    r0 = 1;                        // set err ret
    r1 = r12-1;                   
    r1 = r1-r4;
    if gt jump ret;                 // error return if number of activeted receive channels more than NCH
    dm(RX1_inf_tcb1 + Cx) = r12;    // store number of rx channels in Cx field of first receive TCB
    dm(RX1_inf_tcb2 + Cx) = r12;    // store number of rx channels in Cx field of second receive TCB

/*   *InBuf = reads(2)    InBuf[2][N_RX]        */
    // N_RX = r12 contains number of active RX channels
    r0 = reads(2);
    dm(RX1_inf_tcb1 + IIx) = r0;	   // deposit IIx field of first receive TCB
    dm(RX1_inf_tcb2 + GPx) = r0;	   // deposit GPx field of second receive TCB
    r0 = r0 + r12;
    dm(RX1_inf_tcb1 + GPx) = r0;	   // deposit GPx field of first TCB
    dm(RX1_inf_tcb2 + IIx) = r0;	   // deposit IIx field of second TCB
    
/*  calculate number of TX Sport2 channels */
    i0 = reads(1);                 // i0 points TXMASK[0..3]
    r12 = r12 -r12, r0 = dm(i0,m6);
    dm(MT3CS0) = r0;               // enable transmit channels  31- 0 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MT3CS1) = r0;               // enable transmit channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MT3CS2) = r0;               // enable transmit channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m5);
    dm(MT3CS3) = r0;               // enable transmit channels  63-32 
    call Weight32;
    r12 = r12 + r1;                // r12 = number of active transmit channels
    r0 = 1;                        // set err ret
    r1 = r12-1;                   
    r1 = r1-r4;
    if gt jump ret(db);            // error return if number of activeted transmit channels more than NCH
    dm(TX3_inf_tcb1 + Cx) = r12;   // store number of tx channels in Cx field of first transmit TCB
    dm(TX3_inf_tcb2 + Cx) = r12;   // store number of tx channels in Cx field of second transmit TCB	

/*   *OutBuf = reads(3)   OutBuf[2][N_TX]      */
    // N_TX = r12 contains number of active TX channels
    r0 = reads(3);
    dm(TX3_inf_tcb1 + IIx) = r0;	// deposit IIx field of first TCB 
    dm(TX3_inf_tcb2 + GPx) = r0;    // deposit GPx field of second TCB
    r0 = r0 + r12;
    dm(TX3_inf_tcb1 + GPx) = r0;    // deposit GPx field of first TCB
    dm(TX3_inf_tcb2 + IIx) = r0;    // deposit IIx field of second TCB
	
	/* deposit index modifier registers in TCBs */
    r0 = 1;
    dm(RX1_inf_tcb1 + IMx) = r0;	//  deposit IM field of first receive TCB
    dm(RX1_inf_tcb2 + IMx) = r0;    //  deposit IM field of second receive TCB
    dm(TX3_inf_tcb1 + IMx) = r0;	//  deposit IM field of first transmit TCB
    dm(TX3_inf_tcb2 + IMx) = r0;    //  deposit IM field of second transmit TCB	
	
    r1 = 0x0003FFFF;                // cpx register mask 
	/* SPORT_TX dma control setup and go  */
    r0 = TX3_inf_tcb2 + IIx;	    // get DMA chaining internal mem pointer containing tx_buf address
    r0 = r1 and r0;                 // mask the pointer 
                                    //(Address will be contained in lower 18 bits (bits 17-0);
                                    // Upper 13 bits will be zeroed (bits 19-31);
                                    // Bit 19 is PCI bit ("Program-Controlled Interrupts") 
    r0 = bset r0 BY 18;             // set the pci bit 
    dm(TX3_inf_tcb1 + CPx) = r0;    // write DMA transmit block chain pointer to first transmit TCB 
    r0 = TX3_inf_tcb1 + IIx;        // get DMA chaining internal mem pointer containing tx_buf address 
    r0 = r1 and r0;                 // mask the pointer 
    r0 = bset r0 BY 18;             // set the pci bit 
    dm(TX3_inf_tcb2  + CPx) = r0;   // write DMA transmit block chain pointer to second transmit TCB  
    dm(CP3A) = r0;                  // transmit block chain pointer, initiate TX DMA transfers 

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/* - Note: Tshift2 & TXxA will be automatically loaded with the first 2 values in the  - */
	/* - Tx buffer. The Tx buffer pointer ( IIx ) will increment by 2x the modify value    - */
	/* - ( IIx ).                                                			       	       - */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
	/* SPORT_RX dma control setup and go */
    r0 = RX1_inf_tcb2 + IIx;
    r0 = r1 and r0;                // mask the pointer 
    r0 = bset r0 BY 18;            // set the pci bit 
    dm(RX1_inf_tcb1 + CPx) = r0;   // write DMA receive block chain pointer to TCB buffer
    r0 = RX1_inf_tcb1 + IIx;
    r0 = r1 and r0;                // mask the pointer 
    r0 = bset r0 BY 18;            // set the pci bit 
    dm(RX1_inf_tcb2  + CPx) = r0;  // write DMA receive block chain pointer to TCB buffer
    dm(CP1A) = r0;                 // receive block chain pointer, initiate RX DMA transfers
    jump  nret;                       
//=====================================================================
//================              Sport02          ======================
//=====================================================================
/*  calculate number of RX Sport0 channels */
set_mrmtcs02:
    i0 = r12;                      // i0 points RXMASK[0..3]
    r12 = r12 -r12, r0 = dm(i0,m6);
    dm(MR0CS0) = r0;               // enable receive channels  31- 0 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MR0CS1) = r0;               // enable receive channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MR0CS2) = r0;               // enable receive channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m5);
    dm(MR0CS3) = r0;               // enable receive channels  63-32 
    call Weight32;
    r12 = r12 + r1;                // r12 = number of active receive channels
    r0 = 1;                        // set err ret
    r1 = r12-1;                   
    r1 = r1-r4;
    if gt jump ret;                // error return if number of activeted receive channels more than NCH
    dm(RX0_inf_tcb1 + Cx) = r12;   // store number of rx channels in Cx field of first receive TCB
    dm(RX0_inf_tcb2 + Cx) = r12;   // store number of rx channels in Cx field of second receive TCB

/*   *InBuf = reads(2)    InBuf[2][N_RX]        */
    // N_RX = r12 contains number of active RX channels
    r0 = reads(2);
    dm(RX0_inf_tcb1 + IIx) = r0;	   // deposit IIx field of first receive TCB
    dm(RX0_inf_tcb2 + GPx) = r0;	   // deposit GPx field of second receive TCB
    r0 = r0 + r12;
    dm(RX0_inf_tcb1 + GPx) = r0;	   // deposit GPx field of first TCB
    dm(RX0_inf_tcb2 + IIx) = r0;	   // deposit IIx field of second TCB
    
/*  calculate number of TX Sport2 channels */
    i0 = reads(1);                 // i0 points TXMASK[0..3]
    r12 = r12 -r12, r0 = dm(i0,m6);
    dm(MT2CS0) = r0;               // enable transmit channels  31- 0 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MT2CS1) = r0;               // enable transmit channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m6);
    dm(MT2CS2) = r0;               // enable transmit channels  63-32 
    call Weight32;
    r12 = r12 + r1, r0 = dm(i0,m5);
    dm(MT2CS3) = r0;               // enable transmit channels  63-32 
    call Weight32;
    r12 = r12 + r1;                // r12 = number of active transmit channels
    r0 = 1;                        // set err ret
    r1 = r12-1;                   
    r1 = r1-r4;
    if gt jump ret(db);            // error return if number of activeted transmit channels more than NCH
    dm(TX2_inf_tcb1 + Cx) = r12;   // store number of tx channels in Cx field of first transmit TCB
    dm(TX2_inf_tcb2 + Cx) = r12;   // store number of tx channels in Cx field of second transmit TCB	

/*   *OutBuf = reads(3)   OutBuf[2][N_TX]      */
    // N_TX = r12 contains number of active TX channels
    r0 = reads(3);
    dm(TX2_inf_tcb1 + IIx) = r0;   // deposit IIx field of first TCB 
    dm(TX2_inf_tcb2 + GPx) = r0;   // deposit GPx field of second TCB
    r0 = r0 + r12;
    dm(TX2_inf_tcb1 + GPx) = r0;   // deposit GPx field of first TCB
    dm(TX2_inf_tcb2 + IIx) = r0;   // deposit IIx field of second TCB
	
	/* deposit index modifier registers in TCBs */
    r0 = 1;
    dm(RX0_inf_tcb1 + IMx) = r0;   //  deposit IM field of first receive TCB
    dm(RX0_inf_tcb2 + IMx) = r0;   //  deposit IM field of second receive TCB
    dm(TX2_inf_tcb1 + IMx) = r0;   //  deposit IM field of first transmit TCB
    dm(TX2_inf_tcb2 + IMx) = r0;   //  deposit IM field of second transmit TCB	
	
    r1 = 0x0003FFFF;               // cpx register mask 
	/* SPORT_TX dma control setup and go  */
    r0 = TX2_inf_tcb2 + IIx;	   // get DMA chaining internal mem pointer containing tx_buf address
    r0 = r1 and r0;                // mask the pointer 
                                   //(Address will be contained in lower 18 bits (bits 17-0);
                                   // Upper 13 bits will be zeroed (bits 19-31);
                                   // Bit 19 is PCI bit ("Program-Controlled Interrupts") 
    r0 = bset r0 BY 18;            // set the pci bit 
    dm(TX2_inf_tcb1 + CPx) = r0;   // write DMA transmit block chain pointer to first transmit TCB 
    r0 = TX2_inf_tcb1 + IIx;       // get DMA chaining internal mem pointer containing tx_buf address 
    r0 = r1 and r0;                // mask the pointer 
    r0 = bset r0 BY 18;            // set the pci bit 
    dm(TX2_inf_tcb2  + CPx) = r0;  // write DMA transmit block chain pointer to second transmit TCB  
    dm(CP2A) = r0;                 // transmit block chain pointer, initiate TX DMA transfers 

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/* - Note: Tshift2 & TXxA will be automatically loaded with the first 2 values in the  - */
	/* - Tx buffer. The Tx buffer pointer ( IIx ) will increment by 2x the modify value    - */
	/* - ( IIx ).                                                			       	       - */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
	/* SPORT_RX dma control setup and go */
    r0 = RX0_inf_tcb2 + IIx;
    r0 = r1 and r0;                // mask the pointer 
    r0 = bset r0 BY 18;            // set the pci bit 
    dm(RX0_inf_tcb1 + CPx) = r0;   // write DMA receive block chain pointer to TCB buffer
    r0 = RX0_inf_tcb1 + IIx;
    r0 = r1 and r0;                // mask the pointer 
    r0 = bset r0 BY 18;            // set the pci bit 
    dm(RX0_inf_tcb2  + CPx) = r0;  // write DMA receive block chain pointer to TCB buffer
    dm(CP0A) = r0;                 // receive block chain pointer, initiate RX DMA transfers

nret:
    r0 = r0 - r0;   // normal return
				
ret:
    i0 = gets(4);
    r1 = gets(3);
    r2 = gets(2);
    r3 = gets(1);
    leaf_exit;

_Init_TDM_Bus_IO.end:

/*----------------------------------------------------------------------------------*/
/*              Starts SPORT02 or SPORT13 in TDM primary A channel mode             */
/*----------------------------------------------------------------------------------*/
//function prototype:
//                   void Start_TDM_Bus_IO(int IsSport13);
/////////////////////////////////////////////////////////////////////////////////////

_Start_TDM_Bus_IO:
    leaf_entry;

    r4 = pass r4;
	if eq jump start02;

    bit set imask SP1I|SP3I;      // enable SPORT1 and SPORT3 interrupts 

    ustat2 = dm(SP13MCTL);
    bit set ustat2 MCE;
    dm(SP13MCTL) = ustat2;        // start SPORT13 in TDM mode of operating

    leaf_exit;

start02:
    bit set imask SP0I|SP2I;      // enable SPORT0 and SPORT2 interrupts 

    ustat2 = dm(SP02MCTL);
    bit set ustat2 MCE;
    dm(SP02MCTL) = ustat2;        // start SPORT02 in TDM mode of operating

    leaf_exit;

_Start_TDM_Bus_IO.end:

/*----------------------------------------------------------------------------------*/
/*              Disables SPORT02 or SPORT13  TDM mode of operating                  */
/*----------------------------------------------------------------------------------*/
//function prototype:
//                   void Stop_TDM_Bus_IO(int IsSport13);
/////////////////////////////////////////////////////////////////////////////////////

_Stop_TDM_Bus_IO:
    leaf_entry;

    r0 = r0 - r0;
    r4 = pass r4;
	if eq jump stop02;

    dm(SP13MCTL) = r0;	         // disable TDM on SPORT13  
    dm(CP1A)  = r0;              // disable DMA on SPORT1 
    dm(CP3A)  = r0;              // disable DMA on SPORT3 
    dm(SPCTL1) = r0;             // disable SPORT1  
    dm(SPCTL3) = r0;             // disable SPORT3 

    leaf_exit;

stop02:
    dm(SP02MCTL) = r0;	         // disable TDM on SPORT02  
    dm(CP0A)  = r0;              // disable DMA on SPORT0 
    dm(CP2A)  = r0;              // disable DMA on SPORT2 
    dm(SPCTL0) = r0;             // disable SPORT0  
    dm(SPCTL2) = r0;             // disable SPORT2 

    leaf_exit;

_Stop_TDM_Bus_IO.end:

/*----------------------------------------------------------------------------------*/
/*              Returns 0 if SPORT01 or SPORT13 is not received yet                 */
/*              Returns receive buffer pointer if data is received, clears flag     */
/*----------------------------------------------------------------------------------*/
//function prototype:
//                   int *Get_TDM_Bus_Data_RX(int IsSport13);
/////////////////////////////////////////////////////////////////////////////////////

_Get_TDM_Bus_Data_RX:

    leaf_entry;

    r4 = pass r4;
    if eq jump get02_rx;

    r0 = dm(_Flag_TDM13_Data_Received);
    r0 = pass r0;
    if eq jump zend;

    // clear flag
    dm(_Flag_TDM13_Data_Received) = m5;

    r0 = dm(GP1A);
    r0 = bset r0 BY 18;            // set the pci bit 
	jump zend;

get02_rx:
    r0 = dm(_Flag_TDM02_Data_Received);
    r0 = pass r0;
    if eq jump zend;

    // clear flag
    dm(_Flag_TDM02_Data_Received) = m5;

    r0 = dm(GP0A);
    r0 = bset r0 BY 18;            // set the pci bit 

zend:
    leaf_exit;
    
_Get_TDM_Bus_Data_RX.end:

/*----------------------------------------------------------------------------------*/
/*              Returns 0 if SPORT02 or SPORT13 is not finished transmit operation  */
/*              Returns transmit buffer pointer free to be filled                   */
/*----------------------------------------------------------------------------------*/
//function prototype:
//                   int *Get_TDM_Bus_Data_TX(int IsSport13);
/////////////////////////////////////////////////////////////////////////////////////

_Get_TDM_Bus_Data_TX:

    leaf_entry;

    r4 = pass r4;
    if eq jump get02_tx;

    r0 = dm(_Flag_TDM13_Data_Transmitted);
    r0 = pass r0;
    if eq jump zend1;

    // clear flag
    dm(_Flag_TDM13_Data_Transmitted) = m5;

    r0 = dm(GP3A);
    r0 = bset r0 BY 18;            // set the pci bit 
    jump zend1;

get02_tx:
    r0 = dm(_Flag_TDM02_Data_Transmitted);
    r0 = pass r0;
    if eq jump zend1;

    // clear flag
    dm(_Flag_TDM02_Data_Transmitted) = m5;

    r0 = dm(GP2A);
    r0 = bset r0 BY 18;            // set the pci bit 

zend1:
    leaf_exit;
    
_Get_TDM_Bus_Data_TX.end:

/*----------------------------------------------------------------------------------*/
/* Internal assembler function                                                      */
/* 	            returns weight of r0 32-bit field in r1                             */
/* on input                                                                         */
/*   r0 - 32 bit field                                                              */
/* on output:                                                                       */
/*   r1 - returns number of ones in input 32 bit field                              */
/*   r0 = 0                                                                         */
/*   r2 = 0xffffffff = -1                                                           */
/*                                                                                  */
/* r0, r1 and r2 are destroied                                                      */
/*----------------------------------------------------------------------------------*/

Weight32:
   r0 = pass r0;
   if eq rts (db);
    r1 = r1 - r1;
    r2 = r0 - 1;
   // save interrupt mask register
   ustat1 = IMASK;
   // disable FIXI interrupt
   bit clr IMASK FIXI;
   // save mode1 register
   ustat2 = MODE1;
   // clear saturation mode
   bit clr MODE1 ALUSAT;
   nop;                 // wait one cycle for mode1
next_it:
   r0 = r0 and r2;
   if ne jump next_it (db);
   r1 = r1+1;
   r2 = r0-1;
   // restore mode1 register
   MODE1 = ustat2;
   bit clr STKYx AOS;
   bit clr IRPTL FIXI;
   // restore interrupt mask register
   IMASK = ustat1;
   rts;
Weight32.end:

