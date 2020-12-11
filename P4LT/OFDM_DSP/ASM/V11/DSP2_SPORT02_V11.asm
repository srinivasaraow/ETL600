#define USE_SPORT13
#undef USE_SPORT13
//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      dsp2_sport02_v11.asm
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        24-SEP-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    SPORT0 and SPORT2 of DSP2 on DRS7 board configured in V11 mode,
//             device driver routings 
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    24.09.03  Dr.Vladimir Potapov (vgp), Initial Version
//             01.12.03  revised version, vgp
//             09.05.04  interrupt driven I/O, vgp
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/V11/DSP2_SPORT02_V11.asm $
 * 
 * 2     7.02.05 14:51 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 3     30.09.04 9:05 Leeb02
 * 
 * 2     30.09.04 8:27 Leeb02
 */

#ifndef USE_SPORT13
#define SPCTL_RX SPCTL0
#define SPCTL_TX SPCTL2
#define SPMCTL   SP02MCTL
#define DIV_RX   DIV0
#define DIV_TX   DIV2
#define SPI_RX   SP0I
#define SPI_TX   SP2I
#define V11_RX   RX0A
#define V11_TX   TX2A
#else
#define SPCTL_RX SPCTL1
#define SPCTL_TX SPCTL3
#define SPMCTL   SP13MCTL
#define DIV_RX   DIV1
#define DIV_TX   DIV3
#define SPI_RX   SP1I
#define SPI_TX   SP3I
#define V11_RX   RX1A
#define V11_TX   TX3A
#endif
	
#include <def21161.h>
#include <asm_sprt.h>

.global _Init_V11_Data_IO;
.global _Start_V11_Data_IO;
.global _Stop_V11_Data_IO;
.global _Get_V11_Data;
.global _Put_V11_Data;

.global _Flag_V11_Data_Received;
.global _Flag_V11_Data_Transmitted;
/*---------------------------------------------------------------------------*/
.segment/dm    seg_dmda;
	

.var _Flag_V11_Data_Received;
.var _Flag_V11_Data_Transmitted;

.segment/pm 	seg_pmco;
	

/*=================================================================================================================*/
/*Input:                                                                                                           */   
/*   SLEN = r4  - number of bits in SPORT                                                                          */
/*                    (after entry will be SLEN=SLEN-1)                                                            */
/*                                                                                                                 */
/*  CLKDIV = r8 - Serial Clock Divisor, if equal to 0 - then external clock synchronization will be used           */
/*                  CLKDIV = F_core_clock/(2*F_serial_clock)                                                       */
/*                                                                                                                 */
/*Output:                                                                                                          */
/*         returns 0 if success                                                                                    */ 
/*         returns 1 if faild                                                                                      */ 
/*=================================================================================================================*/

_Init_V11_Data_IO:

    leaf_entry;

    r0 = i0;  puts = r0;
    puts = r1;
    puts = r2;

    /* disable interrupts */
    bit clr imask SPI_RX|SPI_TX;     // disable SPORT02 tx & rx interrupts 

    /* clear r0 */
    r0 = r0 - r0;
    dm(_Flag_V11_Data_Received) = r0;    // clear RX flag
    dm(_Flag_V11_Data_Transmitted) = r0; // clear TX flag
      
    /* clear SPCTL0, SPCTL2 and SP02MCTL registers */
    dm(SPCTL_RX) = r0;            // disable SPORT0
    dm(SPCTL_TX) = r0;            // disable SPORT2
    dm(SPMCTL) = r0;              // disable SPORT02 TDM
      
/* Verify input parameters          */

/*   3<=(SLEN = r4)<=32            */
    r4 = r4 - 1;                  // SLEN = SLEN-1
    r1 = 0xffffffe0;              // r1 = ~0x1f
    r1 = r1 and r4;
    if ne jump ret;               // error: SLEN > 32 or SLEN<1
    r1 = 2;
    r1 = r4 - r1;
    if lt jump ret;               // error: SLEN < 3
    r4 = lshift r4 by 4;          // shift number of bits in sport word length 
	                              // to correct position in SPCTLx register

/*   0<(CLKDIV = r8)<=65535  */
    r8 = pass r8;
    if eq r8 = r8+1;              // if CLKDIV == 0 then r8 = 1
    r1 = 0xffff0000;
    r1 = r1 and r8;
    if ne jump ret;               // error: CLKDIV > 65535


/* deposit sport 0 & 2 or 1 & 3 clock sync divide registers */
    r8 = r8 - 1;                  // CLKDIV--;
    r4 = lshift r4 by 12;
    r8 = r8 or r4;
    dm(DIV_TX) = r8;
    dm(DIV_RX) = r8;
    r4 = lshift r4 by -12;
    r1 = 0xffff;
    r8 = r8 and r1;
    if ne jump int_synch;
//r1 = 0x1f00bf;
//dm(DIV_TX) = r1;
//r1 = ICLK | DDIR | DITFS;         // external synchronization, TX channel
    r1 = DDIR | DITFS;         // external synchronization, TX channel
	                           // data independent FS, 
    r0 = IFS;                  // ??IFS?? internal synchronization, RX channel
    jump dep_creg;
    
int_synch:        
    r1 = ICLK | DDIR | DITFS;  // internal synchronization, TX channel
	                           // data independent FS, 
    r0 = ICLK | IFS;           // ??IFS?? internal synchronization, RX channel

/* deposit sport0 and sport2 control registers */
dep_creg:
    r0 = r0 or r4;             // add required SLEN to control word
    r1 = r1 or r4;             // add required SLEN to control word
    dm(SPCTL_RX) = r0;		   // sport0 RX control register
    dm(SPCTL_TX) = r1;		   // sport2 TX control register

    r0 = r0 - r0;   // normal return
				
ret:
    i0 = gets(3);
    r1 = gets(2);
    r2 = gets(1);
    leaf_exit;

_Init_V11_Data_IO.end:

/*----------------------------------------------------------------------------------*/
/*              Start SPORT0 and SPORT2 in V11 primary A channel mode               */
/*----------------------------------------------------------------------------------*/

_Start_V11_Data_IO:
    leaf_entry;


//    bit set imask SPI_RX|SPI_TX;  // enable SPORT02 tx & rx interrupts 
	r4 = SPEN_A;
    r0 = dm(SPCTL_RX);
	r0 = r0 or r4;
	dm(SPCTL_RX) = r0;
	r0 = r0 - r0;
    r0 = dm(SPCTL_TX);
	r0 = r0 or r4;
	dm(SPCTL_TX) = r0;
	nop;
	nop;
	dm(V11_TX) = r0;
    bit set imask SPI_RX|SPI_TX;  // enable SPORT02 tx & rx interrupts 
    
    leaf_exit;

_Start_V11_Data_IO.end:

/*----------------------------------------------------------------------------------*/
/*              Disables SPORT0 and SPORT2  TDM mode of operating                   */
/*----------------------------------------------------------------------------------*/

_Stop_V11_Data_IO:
    leaf_entry;

    bit clr imask SPI_RX|SPI_TX;     // disable SPORT02 or SPORT13 tx & rx interrupts 

    r0 = r0 - r0;
    dm(SPCTL_RX) = r0;               // disable Sport0  
    dm(SPCTL_TX) = r0;               // disable Sport2  

    leaf_exit;

_Stop_V11_Data_IO.end:

/*----------------------------------------------------------------------------------*/
/*              Returns -1 if SPORT0 is not received V11 data yet                   */
/*              Returns V11 data                                                    */
/*----------------------------------------------------------------------------------*/

_Get_V11_Data:

    leaf_entry;

    r0 = dm(_Flag_V11_Data_Received);
    r0 = pass r0;
    if ne jump get_data;
	r0 = r0 - 1;
	leaf_exit;

get_data:
//    r0 = dm(V11_RX);
    // clear flag
    dm(_Flag_V11_Data_Received) = m5;
    r0 = r0 - r0;

    leaf_exit;
    
_Get_V11_Data.end:

/*----------------------------------------------------------------------------------*/
/*              Returns -1 if SPORT2 is not finished transmit operation             */
/*              Returns 0 and sends data                                            */
/*----------------------------------------------------------------------------------*/

_Put_V11_Data:

    leaf_entry;

    r0 = dm(_Flag_V11_Data_Transmitted);
    r0 = pass r0;
    if ne jump put_data;
    r0 = r0 - 1;
	leaf_exit;

put_data:
    dm(V11_TX) = r4;
    // clear flag
    dm(_Flag_V11_Data_Transmitted) = m5;
    r0 = r0 - r0;

    leaf_exit;
    
_Put_V11_Data.end:

