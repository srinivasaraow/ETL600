/*-------------------------------------------------------------------------*
 * $Workfile: DSP_Main_5k.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : DSP main loop
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/NSK_DSP/C/DSP_Main_5k.c $ 
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 4     5.05.04 17:09 Maag01
 * 
 * 3     5.05.04 17:06 Maag01
 * 
 * 2     7.04.04 11:42 Leeb02
 * 
 * 1     31.03.04 11:30 Maag01
 * Initial Version
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include "basetype.h"
#include "idleCounter.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// case counter values
#define MAINCASE_0				0
#define MAINCASE_1				1    
#define MAINCASE_2				2
#define MAINCASE_3				3
#define MAINCASE_4				4

#define SUBCASE_0				0
#define SUBCASE_1				1
#define SUBCASE_2				2
#define SUBCASE_3				3
#define SUBCASE_4				4
#define SUBCASE_5				5
#define SUBCASE_6				6
#define SUBCASE_7				7
#define SUBCASE_8				8
#define SUBCASE_9				9


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/


/* MAIN **********************************************************************/

void main (void)
{
	static unsigned long	mainCaseCounter;
	static unsigned long	subCaseCounter4;
	static unsigned long	idleCounterMinIndex;
	static unsigned long	idleCounter;
	

	idleCounter_init();

	 
    // main loop 
    while (1) {

		// repetiton rate = 5.333 kHz


		switch(mainCaseCounter)
		{
			// WARNING: don´t change number of cases
			// repetiton rate of case functions = 1.067 kHz

			case MAINCASE_0:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_0;

				// repetiton rate = 1.067 kHz		
					  
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_1:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_1;
				
			
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_2:

				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_2;
					
			
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_3:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_3;
				
				
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_4:
			
				switch(subCaseCounter4) {

					// WARNING: don´t change number of cases
					// repetiton rate of case functions = 0.1067 kHz
					
					case SUBCASE_0:	
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_0;
						
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_1:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_1;
					
					
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_2:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_2;
					
						
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_3:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_3;
						
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_4:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_4;
						
						
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_5:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_5;
						
						
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_6:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_6;
						
						

						subCaseCounter4++;
						break;
			
					case SUBCASE_7:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_7;
						
						

						subCaseCounter4++;
						break;
						
					case SUBCASE_8:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_8;
					
						
					
						subCaseCounter4++;
						break;
					
					case SUBCASE_9:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_9;
						
						
							
						subCaseCounter4 = 0;
						break;
						
					default:
						subCaseCounter4 = 0;
						break;
				}; // end switch(subCaseCounter4)
					
				mainCaseCounter = 0;
				break;
    
			default:
				mainCaseCounter = 0;
				break;
		}; // end switch(mainCaseCounter)

		idleCounter_update(idleCounterMinIndex, idleCounter);
    }

}


