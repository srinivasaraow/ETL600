/*---------------------------------------------------------------------------*
 * $Workfile: Init_UC_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H. P. Rytz PTUKT
 * Remarks      : 
 * Purpose      : Initialize DSP
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/UC_DSP/C/Init_UC_DSP.c $
 * 
 * 3     19.05.05 11:14 Chmibuh
 * dummy memoryCheck removed
 * 
 * 2     12.05.05 18:30 Chmibuh
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 6     26.08.04 16:48 Rytz01
 * TDM Bus off-set problem solved
 * 
 * 5     26.07.04 14:23 Maag01
 * 
 * 4     25.07.04 13:38 Leeb02
 * new idleCounter_init
 * 
 * 3     30.06.04 15:33 Maag01
 * 
 * 2     17.06.04 17:31 Rytz01
 * 
 * 1     17.06.04 16:43 Rytz01
 *
 * Initial Version
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "idleCounter.h"
#include "Timer.h"
#include "Constant.h"

extern volatile long IR_BlockCounter;

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
	// no SDRAM no Initialisation!!
	
	//Disable nested Interupts
	asm("bit clr mode1 NESTM;");    

	// configure IOFLAGS 
	asm("bit clr mode2  FLG1O|FLG2O|FLG3O;");							// set FLAG 1-3 as input, FLAG0 --> SPI-MS
																			// FLAG1 --> ID0 (pulled down)
																			// FLAG2 --> ID1 (pulled up)
																			// FLAG3 --> ID2 (pulled down)
	asm("bit set FLAGS FLG0;");												// Set FLAG0 high
	
	*(long*)(IOFLAG) |= FLG7O|FLG6O|FLG5O|FLG4O;							// set all Flags (4-7) as output
	*(long*)(IOFLAG) |= FLG7|FLG6|FLG5|FLG4;								// set all Flags (4-7) High
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

	// Mem Check, ect.


	InitSPORT02();
	InitSPORT13();
	Start_SPORT02();
	Start_SPORT13();

	IR_BlockCounter = 1;
	while (IR_BlockCounter)
	{

	}

// ************************************************************************************************************
// With this starting procedure (without waiting and without trick!!!) I had no lateral off-set in the internal 
// TDM Bus on UC!!!
// Befor: I had a Tx off-set off 1 TS. As I inserted more channels (debug words 0-3 in TS92 -TS94) for debuging
// the offset growed!!! (and I had more gray hairs)
// The solution is: Do not set *(long*)TX3A = 0; So, I used the StartChainedDMA_SPORT13Tx_Double() function 
// without this.
// ************************************************************************************************************


	// After a delay, start the Tx DMAs (keep order in DMA buffer)
/*	for (DelayCounter = 0; DelayCounter < 900; DelayCounter++)
	{
	}
*/	StartChainedDMA_SPORT13Tx_Double(); 
	StartChainedDMA_SPORT02Tx();

	// this DSP has 10 maincases and 10 subcases. The maincases run with 10.667kHz
	idleCounter_init(DSP_CYCLES_PER_MAINLOOP, N_MAINCASES_10K, N_SUBCASES_10K);

	init_timer();

}



