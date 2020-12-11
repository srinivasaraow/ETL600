/*----------------------------------------------------------------------------*
 * $Workfile: Timer.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz
 * Remarks      : 
 * Purpose      : Timer 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/C/Timer.c $ 
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 4     16.06.04 17:17 Rytz01
 * 
 * 3     4.06.04 14:30 Rytz01
 * 
 * 2     3.06.04 15:48 Rytz01
 * 
 * 1     3.06.04 8:13 Rytz01
 * Initial version
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "Timer.h"
//asm("#include <Timer.h>");

/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

/*****************************************************************************
  Function:		init_timer(void)
  Description:	Initialize the timer counter 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	- TIMER0 is a looptimer which guarantees cyclically 
				  dataprocessing in absence of SPORT13 interrupt (RealTimeBus).
				- Necessary TIMER0 interrupt routine is in file interrupt.asm.

******************************************************************************/

//void segment("ext_code") init_timer(void)

void init_timer(void)
{
	asm("#define DSP_CYCLES		(9223)");// 9216 + 7 !! DSP_CYCLES_PER_MAINLOOP in 10.667 kHz main loop
	asm("bit clr imask TMZLI | TMZHI;");// Disable interrupt of timer		
	asm("bit clr mode2 TIMEN;");		// Disable timer

	asm("TCOUNT=DSP_CYCLES;");

	asm("TPERIOD=DSP_CYCLES;");
	asm("bit set imask TMZLI;");		// enable Timer Int low Priority

}





