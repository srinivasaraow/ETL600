/*-------------------------------------------------------------------------*
 * $Workfile: DIAG.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/OFDM_DSP/H/DIAG.H $
 * 
 * 9     5.04.06 16:25 Chhamaa
 * Rollback
 * 
 * 7     29.03.06 11:24 Chmibuh
 * two new funct. resetP4LTandR1LA() and resetR1LA()
 * 
 * 5     1.11.05 15:54 Chchlee
 * DEBUG complier switch added
 * 
 * 4     28.10.05 8:16 Chchlee
 * Release version
 * 
 * 3     3.10.05 12:01 Chchlee
 * Final version
 * 
 * 2     27.09.05 6:03 Chchlee
 * TOGGLE_LED added
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 10    19.01.05 13:15 Chchlee
 * 
 * 9     15.11.04 18:42 Leeb02
 * resetP4LT() added in DIAG
 * 
 * 8     4.08.04 18:06 Leeb02
 * 
 * 7     4.08.04 17:49 Leeb02
 * 
 * 6     4.08.04 16:47 Leeb02
 * 
 * 5     4.08.04 16:12 Leeb02
 * USE_ASSERT added
 * 
 * 4     25.07.04 13:03 Leeb02
 * 
 * 3     25.07.04 12:53 Leeb02
 * 
 * 2     7.04.04 11:22 Leeb02
 * Basetypes removed
 * 
 * 1     04-03-10 7:38 Leeb02
 * Initial version
 *-------------------------------------------------------------------------*/

#ifndef DIAG_H
#define DIAG_H

/* INCLUDE FILES **************************************************************/

#include "assert.h"
#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/*
#define ACTION_CONTINUE           0
#define ACTION_HALT               1
*/
#define RESET_FIRST_CALL			0x80000000

#define RESET_NOTHING				0x00000000
#define	RESET_R1LA_ONLY				0x00000001  
#define RESET_P4LT_AND_R1LA			0x00000002


/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/

#ifdef _WIN32
	// on host
	#define ASSERT(condition) assert(condition)
#else
/*
	// on target
	#define DiagRecInsertHalt(condition) \
	   {if (condition) diagRecInsert ( (condition), 0, ACTION_HALT, __FILE__, __LINE__);}
*/
	#ifdef USE_ASSERT
		#define ASSERT(condition) assert(condition) // later... DiagRecInsertHalt(!condition)
	#else
		#ifdef DEBUG
	 		#define ASSERT(condition) haltSystem(!(condition))  // for debug version
		#else
	 		#define ASSERT(condition) ;                         // for release version
		#endif
	#endif
#endif

// toggles the LEDs given in "led"
#define TOGGLE_LED(led)\
	{\
		static BOOL fToggle;\
		if (fToggle = !fToggle)\
		{\
			IO_BUFFER_WRITE |= (led);\
		}\
		else\
		{\
			IO_BUFFER_WRITE &= ~(led);\
		}\
	}

/* PUBLIC FUNCTIONS ***********************************************************/

/*
void diag_init(void);

void diagRecInsert (  unsigned long par0,
                      unsigned long par1, 
                      unsigned long action,
                      char *        file,
                      unsigned long lineNumber);
*/

void haltSystem(BOOL doHalt);

void resetP4LT(void);
void resetR1LA(void);
void resetP4LTandR1LA(void);



#endif
