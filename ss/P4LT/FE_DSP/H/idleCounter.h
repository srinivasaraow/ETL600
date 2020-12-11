/*----------------------------------------------------------------------------*
 * $Workfile: idleCounter.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT/UC_DSP/H/idleCounter.h $
 * 
 * 8     29.09.04 11:00 Maag01
 * 
 * 6     11.08.04 15:02 Leeb02
 * 
 * 4     26.07.04 13:56 Maag01
 * 
 * 3     25.07.04 11:45 Leeb02
 * reset method added
 * 
 * 2     25.07.04 11:14 Leeb02
 * Scaling added
 * 
 * 1     27.04.04 11:55 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef IDLE_COUNTER_H
#define IDLE_COUNTER_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define N_IDLE_COUNTERS					21

// idle counter array index defines
#define IDLE_COUNTER_MIN				0		
#define IDLE_COUNTER_MIN_MAINCASE_0		1		
#define IDLE_COUNTER_MIN_MAINCASE_1		2		
#define IDLE_COUNTER_MIN_MAINCASE_2		3	
#define IDLE_COUNTER_MIN_MAINCASE_3		4		
#define IDLE_COUNTER_MIN_MAINCASE_4		5
#define IDLE_COUNTER_MIN_MAINCASE_5		6		
#define IDLE_COUNTER_MIN_MAINCASE_6		7		
#define IDLE_COUNTER_MIN_MAINCASE_7		8	
#define IDLE_COUNTER_MIN_MAINCASE_8		9		
#define IDLE_COUNTER_MIN_MAINCASE_9		10
#define IDLE_COUNTER_MIN_SUBCASE_0		11
#define IDLE_COUNTER_MIN_SUBCASE_1		12	
#define IDLE_COUNTER_MIN_SUBCASE_2		13
#define IDLE_COUNTER_MIN_SUBCASE_3		14
#define IDLE_COUNTER_MIN_SUBCASE_4		15
#define IDLE_COUNTER_MIN_SUBCASE_5		16
#define IDLE_COUNTER_MIN_SUBCASE_6		17
#define IDLE_COUNTER_MIN_SUBCASE_7		18
#define IDLE_COUNTER_MIN_SUBCASE_8		19
#define IDLE_COUNTER_MIN_SUBCASE_9		20

// number of maincases, subcases in 5 and 10 kHz mainloop
#define N_MAINCASES_5K					5
#define N_MAINCASES_10K					10

#define N_SUBCASES_5K					10
#define N_SUBCASES_10K					10


/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct
{
	unsigned long idleCounterMinArray[N_IDLE_COUNTERS];
	unsigned long maxCycles;
	unsigned long numOfMaincases;
	unsigned long numOfSubcases;
	unsigned long minInitValue;
}
t_IdleCounters;


/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/

void idleCounter_init(unsigned long maxCycles, unsigned long numOfMaincases,
					  unsigned long numOfSubcases);
void idleCounter_reset(void);
void idleCounter_update(unsigned long index, unsigned long idleCounter);
t_IdleCounters * idleCounter_read(void);

#endif
