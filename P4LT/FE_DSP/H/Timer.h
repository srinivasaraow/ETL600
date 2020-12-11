/*---------------------------------------------------------------------------*
 * $Workfile: Timer.h $
 * Part of      : ETL600 / OFDM Modem
 * Language     : C
 * Created by   : H.P.Rytz
 * Remarks      :  
 * Purpose      : Generate timer based interrupt in absence of SPORT13
				  interrupt
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/Timer.h $
 * 
 * 1     19.01.05 15:51 Chchlee
 * 
 * 6     16.06.04 17:39 Rytz01
 * 
 * 5     4.06.04 14:30 Rytz01
 * 
 * 4     3.06.04 17:35 Rytz01
 * 
 * 3     3.06.04 8:52 Rytz01
 * Initial version
 *---------------------------------------------------------------------------*/

#ifndef TIMER_H
#define TIMER_H

/* INCLUDE FILES *************************************************************/
#include "Constant.h"
/* PUBLIC TYPE DECLARATIONS **************************************************/
/* PUBLIC CONSTANT DECLARATIONS **********************************************/
//#define DSP_CYCLES			10000		// for Tests only
//#define DSP_CYCLES		(DSP_CYCLES_PER_MAINLOOP+10) 	
/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/
#define RestartTimer10k() asm("bit clr mode2 TIMEN;\
							 TCOUNT=9223;\
							 bit set mode2 TIMEN;") // (9216 + 7) DSP_CYCLES_PER_MAINLOOP in 10.667 kHz main loop

#define RestartTimer5k() asm("bit clr mode2 TIMEN;\
							 TCOUNT=18439;\
							 bit set mode2 TIMEN;") // (18432 + 7) DSP_CYCLES_PER_MAINLOOP in 5.333 kHz main loop

							 
void init_timer(void);

#endif


