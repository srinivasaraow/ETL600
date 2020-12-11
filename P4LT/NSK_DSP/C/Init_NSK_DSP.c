/*---------------------------------------------------------------------------*
 * $Workfile: Init_NSK_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : A. Gygax
 * Remarks      : 
 * Purpose      : Initialisation functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSK_DSP/C/Init_NSK_DSP.c $ 
 * 
 * 13    11-05-27 17:30 Chhamaa
 * R1LB reset pulse added.
 * 
 * 6     13.07.05 8:55 Chchlee
 * SB3 -> 3
 * 
 * 5     13.07.05 8:13 Chchlee
 * PIGGYBACK_HW_ID_MASK modified
 * 
 * 4     1.07.05 10:23 Chmibuh
 * 
 * 3     6.06.05 14:49 Chmibuh
 * putPiggyBackInfoToMaster() added
 * 
 * 2     19.05.05 11:14 Chmibuh
 * dummy memoryCheck removed
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 10    30.08.04 9:46 Rytz01
 * TDM Bus off-set problem solved
 * 
 * 9     20.08.04 15:49 Buhl01
 * shortened dummy mem check again
 * 
 * 8     19.08.04 17:04 Buhl01
 * shortened dummy mem check
 * 
 * 7     18.08.04 9:15 Gygax02
 * Set Flag 0 as output
 * 
 * 6     11.08.04 11:54 Buhl01
 * dummy memory check added for boot control
 * 
 * 5     10.08.04 16:06 Maag01
 * 
 * 4     26.07.04 16:28 Maag01
 * 
 * 3     25.07.04 13:45 Leeb02
 * adjusted for new idleCounter_init
 * 
 * 2     18.06.04 9:19 Rytz01
 * 
 * 1     9.06.04 12:59 Gygax02
 * 
 *---------------------------------------------------------------------------*/


/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "idleCounter.h"
#include "Timer.h"
#include "Constant.h"
#include "FPGAHandling.h"
#include "Piggyback.h"

extern volatile long IR_BlockCounter;;

/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

/*****************************************************************************
  Function:		void Init_DSP_Register(void)
  Description:	Initialize SDRAM's and Registers.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	incl. setting of all waitstates
******************************************************************************/
void Init_DSP_Register(void)
{
    asm("ustat1 = 0;");      
	asm("bit set ustat1 RBWST7|RBWA0|EB3WS1|EB3S1|EB0WS0|EB0A0;");			// For Init-FPGA only. After Init-FPGA it must be changed.
    asm("dm(WAIT) = ustat1;");
    asm("ustat1 = 0x05F7;");          // refresh rate P4LT Settings
    asm("dm(SDRDIV) = ustat1;");
	asm("ustat1 = 0;");
	asm("bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS5|DSDCK1|SDCL2;");
	asm("bit clr ustat1 SDBUF|SDEM3|SDEM2|SDEM1|SDPM|DSDCTL;");
//	asm("ustat1=0x0231C25A;");		// Settings for P4LT 
    asm("dm(SDCTL)=ustat1;");	
	// configure External port for 48-bit writes (disable linkports)
	asm("ustat1 = dm(SYSCON);");
	asm("bit set ustat1 IPACK0 | EBPR10;");
	asm("dm(SYSCON) = ustat1;");
	
	//Disable nested Interupts
	asm("bit clr mode1 NESTM;");    

	// configure IOFLAGS 
	asm("bit clr mode2 FLG1O|FLG2O|FLG3O;");								// set FLAG 1-3 as input,
																			// FLAG1 --> ID0 (pulled up)
																			// FLAG2 --> ID1 (pulled up)
																			// FLAG3 --> ID2 (pulled down)

	asm("bit set mode2 FLG0O;");											// FLAG0 --> goes to I/O FPGA (Pin R4)

	*(long*)(IOFLAG) |= FLG9O|FLG8O|FLG4O;									// set Flags (4-9) as output
	*(long*)(IOFLAG) |= FLG9|FLG8;											// Set FPGA Reset and FPGA Program line
	*(long*)(IOFLAG) &= ~FLG4;												// Start R1LB reset (flag4 = low), reset pulse is terminated in NSK_DSP_Main.c 
}


/*****************************************************************************
  Function:		void initialize_DSP(void)
  Description:	Initialize DSP's.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	
******************************************************************************/
void initialize_DSP(void)
{
	long DelayCounter;

	// Add HW-ID, Mem Chech, ect.

// Do not change anything below this line *********************************************************************
//
// With this starting procedure (without waiting and without trick!!!) I had no lateral off-set in the internal 
// TDM Bus on NSK!!!
// Befor: I had a Tx off-set off 1 TS. As I inserted more channels (debug words 0-3 in TS92 -TS94) for debuging
// the offset growed!!! (and I had more gray hairs)
// The solution is: Do not set *(long*)TX3A = 0; So, I used the StartChainedDMA_SPORT13Tx_Double() function 
// without this trick. (30-08-2004 HPR)

	InitSPORT02();
	InitSPORT13();
	Start_SPORT02();
	Start_SPORT13();
	
	IR_BlockCounter = 1;
	while (IR_BlockCounter)
	{
	}
	StartChainedDMA_SPORT13Tx_Double();
	StartChainedDMA_SPORT02Tx();
// Do not change anything above this Line *********************************************************************




	// this DSP has 10 maincases and 10 subcases. The mainloop runs with 10.667kHz
	idleCounter_init(DSP_CYCLES_PER_MAINLOOP, N_MAINCASES_10K, N_SUBCASES_10K);
	
	init_timer();

}



void SEGMENT_EXT_CODE putPiggyBackInfoToMaster(void)
{
	unsigned long piggybackInfoForMaster;

	piggybackInfoForMaster = ReadSubprintID();
	
	piggybackInfoForMaster = ((piggybackInfoForMaster << 3) & PIGGYBACK_HW_ID_MASK);
	
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	asm("nop;");
	asm("bit set mode1 IRPTEN;");				// enable global Interrupt
	asm("nop;");

	*(long *)SPICTL = WL32 | BAUDR2 | SPIEN | NSMLS | SPTINT | SPRINT | SENDZ;
	
	*(long *)SPITX = piggybackInfoForMaster;
	
	while(!((*(long *)SPISTAT & (RXS0))))		// wait for data shifted in by Master DSP
	{
		asm("nop;");
	}    
	asm("bit clr mode1 IRPTEN;");				// disable global Interrupt
}

