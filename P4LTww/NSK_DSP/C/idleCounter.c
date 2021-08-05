/*-------------------------------------------------------------------------*
 * $Workfile: idleCounter.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Idle counter functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/C/idleCounter.c $ 
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 7     21.09.04 13:59 Maag01
 * IDLE_COUNTER_MIN_INIT_VALUE set to 20000
 * 
 * 6     26.07.04 14:22 Maag01
 * 
 * 5     26.07.04 13:56 Maag01
 * 
 * 4     25.07.04 11:45 Leeb02
 * reset method added
 * 
 * 3     25.07.04 11:14 Leeb02
 * Scaling added
 * 
 * 2     27.04.04 14:20 Maag01
 * 
 * 1     27.04.04 11:54 Maag01
 * Initial Version
 * 
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES **************************************************************/

#include <stdlib.h>

#include "basetype.h"
#include "idleCounter.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define IDLE_COUNTER_MIN_INIT_VALUE		20000	// initial value for min. idle counters


/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/

static t_IdleCounters g_idleCounters;

/* LOCAL FUNCTIONS ***********************************************************/
/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		idleCounter_read()
  Description:	returns a pointer to the g_idleCounters.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		g_idleCounters
  Important:	-

*******************************************************************************/
t_IdleCounters * idleCounter_read(void)
{
	return &g_idleCounters;
}


/*****************************************************************************
  Function:		idleCounter_init()
  Description:	Initializes g_idleCounters.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		g_idleCounters
  Important:	Call before using upDateIdleCounterArray().

*******************************************************************************/
void idleCounter_init(unsigned long maxCycles, unsigned long numOfMaincases,
									   unsigned long numOfSubcases)
{
	idleCounter_reset();
	
	g_idleCounters.maxCycles = maxCycles;
	g_idleCounters.numOfMaincases = numOfMaincases;
	g_idleCounters.numOfSubcases = numOfSubcases;
	g_idleCounters.minInitValue = IDLE_COUNTER_MIN_INIT_VALUE;
}

/*****************************************************************************
  Function:		idleCounter_reset()
  Description:	Resets g_idleCounters.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		g_idleCounters
  Important:	Call before using upDateIdleCounterArray().

*******************************************************************************/
void idleCounter_reset(void)
{
	int i;
	
	for (i = 0; i < N_IDLE_COUNTERS; i++) {
		
		g_idleCounters.idleCounterMinArray[i] = IDLE_COUNTER_MIN_INIT_VALUE;
	};
}

/*****************************************************************************
  Function:		idleCounter_update()
  Description:	Updates g_idleCounters if idleCounter is smaller than
  				previous values.
  Inputs:		index: Index to acces the correct idle counter. 
  				idleCounter: idle counter value
  Outputs:		None
  Return:		None
  Globals:		g_idleCounters
  Important:	

*******************************************************************************/

void idleCounter_update(unsigned long index, unsigned long idleCounter)
{
	if (idleCounter < g_idleCounters.idleCounterMinArray[index]) {
		
		g_idleCounters.idleCounterMinArray[index] = idleCounter;
	};
	
	if (idleCounter < g_idleCounters.idleCounterMinArray[IDLE_COUNTER_MIN]) {
		
		g_idleCounters.idleCounterMinArray[IDLE_COUNTER_MIN] = idleCounter;
	};
}
