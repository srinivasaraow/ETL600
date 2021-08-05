/*---------------------------------------------------------------------------*
 * $Workfile: Init_FE_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H. P. Rytz PTUKT
 * Remarks      : 
 * Purpose      : Initialize functions of FE_DSP
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/C/Init_FE_DSP.c $
 * 
 * 3     19.05.05 11:15 Chmibuh
 * dummy memoryCheck removed
 * 
 * 2     24.01.05 16:11 Chbrbus
 * Changed FLAG10 as input: ADC Overload 
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 15    30.08.04 17:38 Rytz01
 * 
 * 14    30.08.04 10:39 Rytz01
 * TDM Bus off-set problem solved
 * 
 * 13    19.08.04 17:04 Buhl01
 * shortened dummy mem check
 * 
 * 12    12.08.04 16:18 Buhl01
 * 
 * 11    26.07.04 17:25 Maag01
 * 
 * 10    26.07.04 13:56 Maag01
 * 
 * 9     25.07.04 13:24 Leeb02
 * idleCounter_init: new parameters
 * 
 * 8     30.06.04 18:31 Maag01
 * 
 * 7     29.06.04 16:43 Maag01
 * 
 * 6     29.06.04 13:49 Bussli02
 * 
 * 5     29.06.04 11:17 Bussli02
 * Changed to new init internal TDM bus, Rx buffer ok, Tx buffer aligned,
 * no bus conflicts detected on FE DSP
 * 
 * 4     23.06.04 16:03 Rytz01
 * 
 * 3     17.06.04 16:27 Rytz01
 * 
 * 2     17.06.04 14:58 Rytz01
 * 
 * 1     17.06.04 14:03 Rytz01
 *
 * Initial Version
 *---------------------------------------------------------------------------*/
 
 
/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "idleCounter.h"
#include "Timer.h"
#include "Constant.h"

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
	asm("bit set ustat1 EB2WS3|EB2A0|EB0WS0|EB0A0;");
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
	asm("bit clr mode2  FLG0O|FLG1O|FLG2O|FLG3O;");							// set FLAG 0-3 as input, FLAG0 --> not used (TP)
																			// FLAG1 --> ID0 (pulled up)
																			// FLAG2 --> ID1 (pulled down)
																			// FLAG3 --> ID2 (pulled up)
	
	*(long*)(IOFLAG) |= FLG11O|FLG6O|FLG5O|FLG4O;					// set  Flags (4-6, 11) as outputs
	*(long*)(IOFLAG) |= FLG11|FLG9|FLG8|FLG7|FLG4;					// set  Flags (4 and 7-9, 11) High
																	// Flag 10 is used as input for P2: ADC overload

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
	long not_synchronized = 1;
	long DelayCounter;

	// Add Mem Check, ect.

	asm("bit set mode1 IRPTEN;");

// Do not change anything below this line *********************************************************************
//
// With this starting procedure (without waiting and without trick!!!) I had no lateral off-set in the internal 
// TDM Bus on NSD!!! (30-08-2004 HPR)
	InitSPORT02();
	InitSPORT13();
	Start_SPORT02();
	Start_SPORT13();

	IR_BlockCounter = 2;
	while (IR_BlockCounter)
	{
	}
	StartChainedDMA_SPORT13Tx_Double();
	StartChainedDMA_SPORT02Tx();
// Do not change anything above this Line *********************************************************************

	// this DSP has 5 maincases and 10 subcases. The mainloop runs with 5.3kHz
	idleCounter_init(DSP_CYCLES_PER_MAINLOOP_5K, N_MAINCASES_5K, N_SUBCASES_5K); 

	init_timer();

	// Wait at least 2 cycles to ensure there are no TDM null pointer
	IR_BlockCounter = 3;
	while (IR_BlockCounter)
	{

	}
}






