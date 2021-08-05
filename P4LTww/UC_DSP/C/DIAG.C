/*-------------------------------------------------------------------------*
 * $Workfile: DIAG.C $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/Master_DSP/C/DIAG.C $ 
 * 
 * 11    29.03.06 11:00 Chmibuh
 * two new funct. resetP4LTandR1LA() and resetR1LA()
 * 
 * 9     3.10.05 12:01 Chchlee
 * Final version
 * 
 * 8     28.09.05 15:42 Chchlee
 * nTimesToBlink increased to 100
 * 
 * 7     27.09.05 6:04 Chchlee
 * TOGGLE_LED moved to diag.h
 * 
 * 6     27.09.05 5:50 Chchlee
 * haltSystem/assert now resets board after blinking the LEDs for a while
 * 
 * 5     1.09.05 11:40 Chchlee
 * SEGMENT_EXT_CODE added to resetP4LT
 * 
 * 4     21.07.05 13:48 Chchlee
 * eventRec_EmptyStack added to resetP4LT()
 * 
 * 3     15.07.05 15:25 Chchlee
 * Comments added
 * 
 * 2     10.02.05 13:01 Chchlee
 * Turn off LEDs before reset
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 14    19.01.05 13:15 Chchlee
 * 
 * 13    15.11.04 18:42 Leeb02
 * resetP4LT() added in DIAG
 * 
 * 12    13.10.04 13:15 Leeb02
 * Counter added to "while(1)"-loop to avoid code removal by optimizer
 * 
 * 11    12.08.04 18:40 Maag01
 * 
 * 10    12.08.04 13:36 Maag01
 * Modified to use without external RAM. NO_EXT_RAM has to be defined.
 * 
 * 9     5.08.04 8:21 Leeb02
 * 
 * 8     4.08.04 17:48 Leeb02
 * 
 * 7     4.08.04 14:55 Leeb02
 * 
 * 6     21.07.04 14:43 Leeb02
 * 
 * 5     20.07.04 13:07 Leeb02
 * 
 * 4     20.04.04 16:00 Maag01
 * 
 * 3     7.04.04 11:22 Leeb02
 * Basetypes removed
 * 
 * 2     04-03-15 7:40 Leeb02
 * Intermediate version
 * 
 * 1     04-03-10 7:39 Leeb02
 * Initial version
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "stdlib.h"
#include "string.h"
#include "DIAG.H"

#ifdef MASTER_DSP
	#include "IO_HW_BUFFER.H"
	#include "EPROMMED.H"
#endif


/* LOCAL CONSTANT DECLARATIONS ***********************************************/


//#define DIAG_SECTOR_0  SECTOR_SA65
//#define DIAG_SECTOR_1  SECTOR_SA66

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/

void haltSystem(BOOL doHalt)
{
	if (doHalt)
	{

#ifdef MASTER_DSP

		// haltSystem on the master DSP: flash the 6 LEDs 30 times, then reset the board

		int nTimesToBlink = 100;

		while (nTimesToBlink--)
		{
			int i = 1000000; // blink rate

			TOGGLE_LED(HW_AL_LED_DSP|LINK_AL_LED|SYS_AL_LED|P4LT_AL_LED|WARNING_LED|READY_LED);

			*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer / serve watchdog

			while (i--)
			{ 
				volatile int x = 10; // to keep compiler from optimizing away this code
				x +=1;
			}
		}

		resetP4LT();

#else
		// haltSystem on the slave DSP: stop producing lifesign -> master will detect that and reset board

		while(1)	// do nothing on all slave DSPs, for Christ's sake
		{ 
			volatile int x = 10;  // to keep compiler from optimizing away this code
			x +=1;
		}

#endif

	}

}

#ifdef MASTER_DSP

void SEGMENT_EXT_CODE resetP4LT(void)
{
	volatile int x = 0;

	// turn off all LEDs
	IO_BUFFER_WRITE |= (HW_AL_LED_DSP|LINK_AL_LED|SYS_AL_LED|P4LT_AL_LED|WARNING_LED|READY_LED);
	*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer

	// empty stack
	eprom_emptyStack();

	// now we must make sure that the event is actually stored in the flash before resetting
	eventRec_EmptyStack();

	while(TRUE)
	{
		x++; // make sure this loop is not optimized away by the compiler
	}
	//let the watchdog do it's dirty job
}



void SEGMENT_EXT_CODE resetR1LA(void)
{

	*(long*)(MSGR4) = (RESET_R1LA_ONLY | RESET_FIRST_CALL);

}


void SEGMENT_EXT_CODE resetP4LTandR1LA(void)
{

	*(long*)(MSGR4) = (RESET_P4LT_AND_R1LA | RESET_FIRST_CALL);

}








#endif //#ifdef MASTER_DSP


/*
void SEGMENT_EXT_CODE diag_init (void)
{
}


void diagRecInsert (  unsigned long par0,
                      unsigned long par1, 
                      unsigned long action,
                      char *        file,
                      unsigned long lineNumber)
{


//	assert(0);

	typedef struct
	{
		struct 
		{
			unsigned int lineNumber: 16;
			unsigned int char0     : 5;
			unsigned int char1     : 5;
			unsigned int char2     : 5;
			unsigned int isHalt    : 1;
		}
		s_Word0;

		struct 
		{
			unsigned int char3     : 5;
			unsigned int char4     : 5;
			unsigned int char5     : 5;
			unsigned int unused1   : 1;

			unsigned int char6     : 5;
			unsigned int char7     : 5;
			unsigned int char8     : 5;
			unsigned int unused2   : 1;
		}
		s_Word1;

		struct 
		{
			unsigned int parameter0  : 16;
			unsigned int parameter1  : 16;
		}
		s_Word2;
	}
	t_DebugWords;
	
	t_DebugWords debugWords;

	char * p = &file[strlen(file) - 1]; // pointer to end of pathname


	while (*(--p) != '\\');  // find first backslash starting at the end of the pathname

	// now we have a pointer to the beginning of the filename

	memset(&debugWords, 0, sizeof(debugWords)); // zero out debugWords

	debugWords.s_Word0.char0 = toupper (*(++p)) - 'A';
	debugWords.s_Word0.char1 = toupper (*(++p)) - 'A';
	debugWords.s_Word0.char2 = toupper (*(++p)) - 'A';
	debugWords.s_Word1.char3 = toupper (*(++p)) - 'A';
	debugWords.s_Word1.char4 = toupper (*(++p)) - 'A';
	debugWords.s_Word1.char5 = toupper (*(++p)) - 'A';
	debugWords.s_Word1.char6 = toupper (*(++p)) - 'A';
	debugWords.s_Word1.char7 = toupper (*(++p)) - 'A';
	debugWords.s_Word1.char8 = toupper (*(++p)) - 'A';

	debugWords.s_Word0.lineNumber = lineNumber;
	debugWords.s_Word2.parameter0  = par0;
	debugWords.s_Word2.parameter1  = par1;

	debugWords.s_Word0.isHalt  = 1;

	{
		char test[9];

		test[0] = debugWords.s_Word0.char0 + 'A';
		test[1] = debugWords.s_Word0.char1 + 'A';
		test[2] = debugWords.s_Word0.char2 + 'A';
		test[3] = debugWords.s_Word1.char3 + 'A';
		test[4] = debugWords.s_Word1.char4 + 'A';
		test[5] = debugWords.s_Word1.char5 + 'A';
		test[6] = debugWords.s_Word1.char6 + 'A';
		test[7] = debugWords.s_Word1.char7 + 'A';
		test[8] = debugWords.s_Word1.char8 + 'A';

	   p=p;
	}


}
*/
