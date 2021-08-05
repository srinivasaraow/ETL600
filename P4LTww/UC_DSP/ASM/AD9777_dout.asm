/*-------------------------------------------------------------------------*
 * $Workfile: AD9777_dout.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Vladimir Potapov (vgp)
 * Remarks      :
 * Purpose      : AD9777 IQ_DATA output processing functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/UC_DSP/ASM/AD9777_dout.asm $
 * 
 * 5     4.04.06 18:36 Chmibuh
 *  _AD9777_Init_Data_Port: Flush external Port fifo added!!!
 * _AD9777_Stop_Data_Port is never used -> commented out
 * 
 * 4     17.03.06 10:56 Chmibuh
 * dm(_TempMode1_Timer) = mode1;
 *  bit clr mode1 PEYEN;   // Switch off SIMD                  
 * added for every function
 * 
 * 2     8.02.05 14:45 Chbrbus
 * UC DAC interrupt syncronized with SPORT13 interrupt - reduced DAC
 * buffers from 3 to 2 ! Cleaned Interrupt functions and EP1
 * initialisation.
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 7     3.06.04 13:37 Maag01
 * 
 * 6     3.06.04 12:06 Maag01
 * 
 * 5     2.06.04 18:58 Maag01
 * 
 * 4     1.06.04 18:04 Maag01
 * 
 * 3     26.05.04 18:31 Maag01
 * 
 * 2     26.05.04 16:38 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
	
#include <def21161.h>

#include <asm_sprt.h>

.global _AD9777_Init_Data_Port;
.global _AD9777_Start_Data_Port;
/*.global _AD9777_Stop_Data_Port;*/


/*---------------------------------------------------------------------------*/
.segment/dm    seg_dmda;
	
/*****************************************************************************/
/*                                                                           */
/* TCB = "Transfer Control Block"                                            */
/*                                                                           */
/*   TCB format:                                                             */
/*          0  ECx (length of destination buffer),                            */
/*          1  EMx (destination buffer step size),                            */
/*          2  EIx (destination buffer index (initialized to start address)), */
/*          3  GPx ("general purpose"),                                       */
/*          4  CPx ("Chain Point register"; points to last address (IIx) of   */
/*                                         next TCB to jump to               */
/*                                         upon completion of this TCB.),    */
/*          5  Cx  (length of source buffer),                                 */
/*          6  IMx (source buffer step size),                                 */
/*          7  IIx (source buffer index (initialized to start address))       */
/*                                                                           */
/*****************************************************************************/

.var			_TempMode1_Timer;

/* AD9777 transmit tcb */
.var    IQData_tcb[16] = 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0;	



#define ECx 0
#define EMx 1
#define EIx 2
#define GPx 3
#define CPx 4
#define Cx  5
#define IMx 6
#define IIx 7
#define IQDATA_TCB1 IQData_tcb
#define IQDATA_TCB2 (IQData_tcb+8)

.segment/pm 	seg_pmco;
	
/*******************************************************************************************************************/
/*                                                                                                                 */
/* int AD9777_Init_Data_Port(int *IQBuf1_ptr, int *IQBuf2_ptr, int Len)							                   */
/*                                                                                                                 */
/*  Buf1 - Buf2 - Buf3 - Buf1 ...                                                                                  */
/*                                                                                                                 */
/*******************************************************************************************************************/

/*=================================================================================================================*/
/*Input:                                                                                                           */   
/*                                                                                                                 */
/*  *IQBuf1_ptr =  r4 - points firts output buffer of length not less than Len of 32-bit packed IQData words       */
/*                                                                                                                 */
/*  *IQBuf2_ptr =  r8 - points second output buffer of length not less than Len of 32-bit packed IQData words      */
/*                                                                                                                 */
/*  Len			  r12 - Length of a buffer																	       */
/*                                                                                                                 */
/*                                                                                                                 */
/*  IQ data packing: IData in 16 LSB, QData in 16 MSB of 32-bit word                                               */
/*                                                                                                                 */
/*                                                                                                                 */
/*Output:                                                                                                          */
/*         returns 0                                                                                               */ 
/*                                                                                                                 */ 
/*=================================================================================================================*/

_AD9777_Init_Data_Port:

    leaf_entry;

	dm(_TempMode1_Timer) = mode1;
	bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)
        
    r0 = i0;  puts = r0;
    puts = r1;
    puts = r2;

     // clear interrupts from AD9777 DMA11 port
    bit clr imask EP1I;      // disable EP1 interrupts 

	nop;

     // reset AD9777 DMA11 port
    r0 = r0 - r0;
    dm(CPEP1)  = r0;         // stop DMA11 on external port 1
    dm(DMAC11) = r0;         // clear DMA11 control register of external port 1


	nop;
	nop;


	r0 = FLSH;				// set flush bit
	dm(DMAC11) = r0;		// Flush external Port FIFO
	// Due to the DAC reset this function will be called again and the flush of the FIFOs
	// guaranties a proper restart.


	nop;
	nop;


    // setup DMA control registers of EP1
	r0 = DEN | CHEN | TRAN | HSHAKE | PMODE4 | INT32 |MAXBL0; 
    dm(DMAC11) = r0;			// setup DMA control register of external port 1 in external handshake mode

/* deposit DMA11 internal addresses */
    dm(IQDATA_TCB1+IIx) = r4;   // store internal address of FPGA IData channel regiter of the first TCB
    dm(IQDATA_TCB1+GPx) = r8;  // store internal address of FPGA IData channel regiter of the first TCB
    dm(IQDATA_TCB2+IIx) = r8;   // store internal address of FPGA IData channel regiter of the second TCB
    dm(IQDATA_TCB2+GPx) = r4;   // store internal address of FPGA IData channel regiter of the second TCB
        
/* deposit DMA word counters */
    dm(IQDATA_TCB1+Cx)  = r12;   // store number of IData words to transmit from internal memory of first receive TCB
    dm(IQDATA_TCB2+Cx)  = r12;   // store number of IData words to transmit from internal memory of second receive TCB
    dm(IQDATA_TCB1+ECx) = r12;   // store number of IData words to receive by IQData register of first receive TCB
    dm(IQDATA_TCB2+ECx) = r12;   // store number of IData words to receive by IQData register of second receive TCB

/* deposit dummy address of external IQData register */
    r4 = r4-r4;                 // dummy address of IQData register
    dm(IQDATA_TCB1+EIx) = r4;   // store external dummy address of IQData channel register of the first TCB
    dm(IQDATA_TCB2+EIx) = r4;   // store external dummy address of IQData channel register of the second TCB
        
/* deposit DMA internal address modifiers eq 1 */
    dm(IQDATA_TCB1+IMx) = m6;   // store internal address modifier IQData channel regiter of the first TCB
    dm(IQDATA_TCB2+IMx) = m6;   // store internal address modifier IQData channel regiter of the second TCB

/* deposit DMA external address modifiers eq 0 */
    dm(IQDATA_TCB1+EMx) = m5;   // store external address modifier IQData channel regiter of the first TCB
    dm(IQDATA_TCB2+EMx) = m5;   // store external address modifier IQData channel regiter of the second TCB

/* deposit DMA chain point addresses */
    r0 = 0x0003FFFF;            // cpx register mask 

    r1 = IQDATA_TCB2+IIx;
    r1 = r0 and r1;
    r1 = bset r1 BY 18;         // set the pci bit 
    dm(IQDATA_TCB1+CPx) = r1;   // store chain pointer of IQData channel of the second TCB 

    r1 = IQDATA_TCB1+IIx;
    r1 = r0 and r1;
    r1 = bset r1 BY 18;         // set the pci bit 
    dm(IQDATA_TCB2+CPx) = r1;   // store chain pointer of IQData channel of the first TCB 
    
        
    r0 = r0 - r0;   // normal return
				
ret:
    i0 = gets(3);
    r1 = gets(2);
    r2 = gets(1);

	mode1 = dm(_TempMode1_Timer);	// use primary registers and restore mode1 register

    leaf_exit;

_AD9777_Init_Data_Port.end:

/*----------------------------------------------------------------------------------*/
/*              Start IQData channel on external port #1                            */
/*----------------------------------------------------------------------------------*/

_AD9777_Start_Data_Port:
    leaf_entry;

	dm(_TempMode1_Timer) = mode1;
	bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)

    puts = r0;

    bit set imask EP1I;         // enable EP1 interrupts 

    r0 = dm(IQDATA_TCB2+CPx);   // get IData DMA transmit block chain pointer
    dm(CPEP1) = r0;             // start chained DMA on external port # 1 	
    
    r0 = gets(1);

	mode1 = dm(_TempMode1_Timer);	// use primary registers and restore mode1 register

    leaf_exit;

_AD9777_Start_Data_Port.end:

/*----------------------------------------------------------------------------------*/
/*              Disable DMA operation on external ports #1                          */
/*----------------------------------------------------------------------------------*/
/* Function is unused: Removed to save PM memory
_AD9777_Stop_Data_Port:
    leaf_entry;

	dm(_TempMode1_Timer) = mode1;
	bit clr mode1 PEYEN;			// Switch off SIMD (speeddsp library uses it)


    bit clr imask EP1I;       // disable EP1 interrupts 

	nop;

    dm(CPEP1)  = m5;          // disable DMA on EP1 channel
    dm(DMAC11) = m5;          // clear DMA control register of external port 1 

	mode1 = dm(_TempMode1_Timer);	// use primary registers and restore mode1 register

    leaf_exit;

_AD9777_Stop_Data_Port.end:
*/






