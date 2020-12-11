/*-------------------------------------------------------------------------*
 * $Workfile: EPROMMED.C $
 * Part of	: ETL600
 * Language	: C
 * Created by	: CLEMENS BUERGI 08.12.1997
 * Remarks	:             
 * Purpose	:     medium level eprom access
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/EPROMMED.C $
 * 
 * 15    13.10.05 19:17 Chchlee
 * debug code removed
 * 
 * 14    3.10.05 18:39 Chmibuh
 * #ifndef EMERGENCY_FW added to a compilable emergency FW
 * 
 * 13    27.09.05 6:04 Chchlee
 * Syntax error removed
 * 
 * 12    27.09.05 5:47 Chchlee
 * eprom_emptyStack time-out increased to 15s
 * 
 * 11    19.09.05 11:46 Chchlee
 * break statement added to function eprom_emptyStack()
 * 
 * 10    1.09.05 11:35 Chchlee
 * debug alarm event renamed to ALARM_EVENT_DEBUG1 
 * 
 * 9     25.07.05 14:49 Chchlee
 * New testcode for bug 129: Now, an event is generated
 * 
 * 8     18.07.05 10:51 Chchlee
 * 
 * 7     15.07.05 16:56 Chchlee
 * Temporary version for debugging bug 129
 * 
 * 6     30.06.05 7:45 Chchlee
 * ASSERT on ring buffer overflow removed
 * 
 * 5     28.06.05 13:53 Chchlee
 * ASSERTs consolidated
 * 
 * 4     11.04.05 18:32 Chchlee
 * Concurrent flash access bug fixed
 * 
 * 3     4.04.05 7:53 Chchlee
 * Command stack now implemented with ringbuffer to avoid task time
 * overflow. 
 * Temporary dirty fix of flash access problem
 * 
 * 2     16.02.05 10:29 Chchlee
 * Upgrade to Ptty2
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 14    25.08.04 15:14 Leeb02
 * reset properly handled
 * 
 * 13    16.08.04 9:57 Leeb02
 * 
 * 12    16.08.04 7:37 Leeb02
 * 
 * 11    6.08.04 17:23 Leeb02
 * 
 * 10    6.08.04 13:51 Leeb02
 * 
 * 9     3.08.04 12:54 Leeb02
 * Queue size increased again
 * 
 * 8     2.08.04 9:09 Leeb02
 * COMMAND_STACK_SIZE increased
 * 
 * 7     2.08.04 8:17 Leeb02
 * 
 * 6     30.07.04 15:03 Leeb02
 * 
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES **************************************************************/
  
#include "EPROMMED.H"
#include "EPROMLOW.H"  

#include "diag.h"
#include "crc.h"
#include "IO_HW_BUFFER.H" // for watchdog

#include "eventrec.h"  // for debugging
#include "constant.h"  // for debugging


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define COMMAND_STACK_SIZE		500 // queue size

#define CASE_0         0
#define CASE_1         1
#define CASE_2         2
#define CASE_3         3
#define CASE_4         4    

/* LOCAL TYPE DECLARATIONS ***************************************************/

// This typedef defines an command stack for passing commands and parameters to eprom_activate().
typedef struct 
{
	long Command;
	long *AddrFrom;
	long *AddrTo;
	long Length;
} 
t_CommandStackElement;

/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/
static SEGMENT_EXT_DATA t_CommandStackElement g_commandStack[COMMAND_STACK_SIZE];	// Eprom Option stack   

static unsigned long    g_readPointer, g_writePointer;

static BOOL g_epromIsBusy;							

/* LOCAL FUNCTIONS ***********************************************************/

static void eprom_pullCommand(void);
static void SaveData(void);
static void CalcAndWriteCRC(void);
static void BlockErase(void);

/* IMPLEMENTATION ************************************************************/

BOOL eprom_isBusy(void)
{
	return g_epromIsBusy;
}

void eprom_setBusyState(BOOL isBusy)
{
	g_epromIsBusy = isBusy;
}


void eprom_emptyStack(void) // must not be called during normal tasking operation!!!
{
#ifndef EMERGENCY_FW
	unsigned long startValue = count_start();
#endif
	while(eprom_commandsInStack())
	{
		IO_BUFFER_READ = *(long*)IO_BUFFER_DSP; //serve watchdog
		
		eprom_activate(); 

#ifndef EMERGENCY_FW
		if (count_end(startValue) > (15UL * DSP_CLK_INT_Hz)) // wait at most 15 seconds
			// NOTE: max value here is MAXLONG, i.e. 43.69s
		{
			//eventRec_recordEvent(ALARM_EVENT_DEBUG1, g_commandStack[g_readPointer].Command, FALSE); // debug code
			break;
		}
#endif
	}
}


/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* CommandStack	Handling													*/
/* push / pull from command stack											*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * Write a new command onto the stack. A ring buffer is used.
 *
 * Input
 * -----       
 * Command to be written onto the stack
 *
 * Output
 * ------
 * none
 *
 * Calls to other functions:
 * ------------------------         
 * none             
 *
*/

void eprom_pushCommand(long option, long* AddrFrom, long* AddrTo, long Length)	        
{   
	// Push option if there is space left on the stack
	if (g_readPointer != (g_writePointer + 1) % COMMAND_STACK_SIZE)
	{   
		g_commandStack[g_writePointer].Command  = option;
		g_commandStack[g_writePointer].AddrFrom = AddrFrom;
		g_commandStack[g_writePointer].AddrTo   = AddrTo;
		g_commandStack[g_writePointer].Length   = Length; 

		g_writePointer = (g_writePointer + 1) % COMMAND_STACK_SIZE;
	}	
	//else discard commands
}


/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* CommandStack	Handling													*/
/* push / pull from command stack											*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * Remove one command.
 *
 * Input
 * -----       
 * none
 *
 * Output
 * ------
 * none
 *
 * Calls to other functions:
 * ------------------------         
 * none       
 *
*/
static void eprom_pullCommand(void)
{	
    // Do nothing if stack is already empty
	if (g_writePointer != g_readPointer)
	{
		g_readPointer = (g_readPointer + 1) % COMMAND_STACK_SIZE;
	}
}				


     
/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* BOOL eprom_commandsInStack()												*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * This routine can be used to check if the stack is empty or not.
 *
 * Input
 * -----       
 * none
 *
 * Output
 * ------
 * TRUE is stack is not empty.
 *
 * Calls to other functions:
 * ------------------------         
 * none
*/     

BOOL eprom_commandsInStack(void)	        
{
	return(g_writePointer != g_readPointer);	
}
 
 

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* saveData() 														*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * writes length longs, from AddrFrom to epromaddress AddrTo
 * Input
 * -----
 * Is taken directly from the command stack
 * Output
 * ------
 * Error Code		None
 *
 */

static void SaveData(void)
{   
	static long *AddrTo,*AddrFrom;
	static long Length;
	static char char_to_be_saved, tempChar;	
    static long WriteCase = CASE_0; 
	static BOOL	bWriting; 
	static long ByteCounter, WriteCounter = 0;	
    static BOOL bSuccess;
    static e_EpromWriteStatus epromWriteStatus;                    
	
	switch(WriteCase)
	{                                               
		// Case 0: initialization
		case CASE_0: 

	        AddrFrom = g_commandStack[g_readPointer].AddrFrom;
            AddrTo   = g_commandStack[g_readPointer].AddrTo;
            Length   = g_commandStack[g_readPointer].Length; 

			if(Length & LENGHT_IN_BYTES) // check if legth was given in bytes or longs
			{ 
				Length &= (~LENGHT_IN_BYTES); 
			}
			else 
			{ 
				Length *= NR_OF_BYTES_IN_LONG;
			}
            
			WriteCounter = 0;  
            ByteCounter = NR_OF_BYTES_IN_LONG;
            bWriting = TRUE;
            tempChar = *AddrFrom++;
			char_to_be_saved = tempChar & MASK_8LSB;

            WriteCase = CASE_1;   
            // There's intentionally no break here !   
			
		case CASE_1:

			if (bWriting)
            {
				if(writeByteToEprom(char_to_be_saved, 
					                AddrTo, 
					                FALSE)) // non-blocking write
				{
					bWriting = FALSE;
				}
				// else retry because eprom was busy
    		}
    		else
    		{           
				epromWriteStatus = writeByteToEpromStatus(char_to_be_saved, AddrTo);
    			// Check if writing has terminated, if so, allow next char to be written.
    			if (e_EpromWriteStatus_Ok == epromWriteStatus)
    			{
					WriteCounter++;
	            	if (--ByteCounter)
					{   
						tempChar = tempChar >> 8;
						char_to_be_saved = tempChar & MASK_8LSB;
						
					}
					else   
					{ 
						tempChar = *AddrFrom++; 
						char_to_be_saved = tempChar & MASK_8LSB;
						
						ByteCounter = NR_OF_BYTES_IN_LONG;
					}

					if (WriteCounter >= Length) 
					{
						eprom_pullCommand();
						g_epromIsBusy = FALSE;
						WriteCase = CASE_0;
					}

					AddrTo++;   
					bWriting = TRUE;
   				}	      
    			else	// If writing is not finished, check timeout condition.
    			{
		            if (epromWriteStatus == e_EpromWriteStatus_AwaitWrite) 
					{ 
						epromWriteStatus = e_EpromWriteStatus_Ok;
					}  // no error
					else 
					{
	            		eprom_pullCommand();

						WriteCase = CASE_0; 

						resetEprom(); 
						g_epromIsBusy = FALSE;
    	       		}	
            	}	
    		}	
			break;   
			
		default:
			ASSERT(FALSE);
	}           
}                                       


/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* void BlockErase()		 												*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * handles ERASE_BLOCK case 
 *
 * Input
 * -----
 *
 * Output                                                     
 * ------
 * EPROM_ERROR if an error occurred (timeout), else NO_EPROM_ERROR.
 *
 * Change History
 * --------------
 *
 * Calls to other functions
 * ------------------------
 *
 */
 
static void BlockErase(void)
{
	static long EraseCase = CASE_0;
	static long Addr;
    static e_EraseBlockStatus EraseReturnValue = e_EraseBlockStatus_Ok; 

	switch (EraseCase)
	{                                                                          
		// Case 0 does the initialization part.
		case CASE_0:	
			
			Addr = (long)g_commandStack[g_readPointer].AddrTo;
            if (eraseBlock(Addr))   // retry if eprom was busy
			{
				EraseCase = CASE_1;   
			}
            break;

        case CASE_1: 

            EraseReturnValue = eraseBlockStatus(Addr);

        	if (EraseReturnValue == e_EraseBlockStatus_Ok)
        	{  
        		eprom_pullCommand();
        		g_epromIsBusy = FALSE;
        		EraseCase = CASE_0;
        	}
        	else 
        	{ 
        		if(EraseReturnValue == e_EraseBlockStatus_AwaitErase) 
				{ 
					EraseReturnValue = e_EraseBlockStatus_Ok; 
				}  // WAIT_FOR_ERASE ist kein Fehler
        		else 
        		{  // Timeout
        			eprom_pullCommand();
					resetEprom(); 
        			g_epromIsBusy = FALSE;
        			EraseCase = CASE_0;
        		}
        	}
        	break;

		default:	
			ASSERT(FALSE);

	}	
}                                           



/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* void CalcAndWriteCRC()	 												*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * handles CALC_AND_WRITE_CRC case 
 *
 * Input
 * -----
 *
 * Output                                                     
 * ------
 * EPROM_ERROR if an error occurred (timeout), else NO_EPROM_ERROR.
 *
 * Change History
 * --------------
 *
 * Calls to other functions
 * ------------------------
 *
 */
 
static void CalcAndWriteCRC(void)
{
	static unsigned long Case = CASE_0; 
    static unsigned long calcCRC; 
    static long          *AddrFrom, *AddrTo, Length; 
    static unsigned long Counter;
    
	switch (Case)
	{                                                                          
		case CASE_0:

			AddrFrom = g_commandStack[g_readPointer].AddrFrom;
            AddrTo   = g_commandStack[g_readPointer].AddrTo;
            Length   = g_commandStack[g_readPointer].Length;    

            Counter = 0;
		    calcCRC = VOIDCRC;
			
			Case = CASE_1;	
            break;

        case CASE_1:  
			{
				unsigned long value;

				if (readByteFromEprom(AddrFrom, (long*)&value))
				{
					calcCRC = crc16(calcCRC, value);
					
					AddrFrom++;

					if (!(++Counter < Length)) 
					{ 
						Case = CASE_2; 
					}
				}
				// else try again
			}
			break;	 

		case CASE_2:

			eprom_pullCommand(); 
			eprom_pushCommand(SAVE_DATA, (long*)&calcCRC, AddrTo, 1);
			g_epromIsBusy = FALSE;
			Case = CASE_0;
			break;	

		default:	
			ASSERT(FALSE);
	}
}                   

                                    
/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* long MainEprom()															*/
/* Main function															*/
/*--------------------------------------------------------------------------*/
/* 
 * Description
 * -----------
 * This function executes the function on the Command Stack. It is the main
 * function of eprommed.c.
 *
 * Input
 * -----
 * Command stack must contain a valid option. Commands are defined in eprommed.h
 *
 * Output
 * ------
 * none
 *
 * Calls to other functions:
 * ------------------------
*/												

void eprom_activate(void)
{
	if (eprom_commandsInStack()) 
	{ 
		g_epromIsBusy = TRUE; // is set to false again in respective command function

		switch (g_commandStack[g_readPointer].Command)
		{
    		case SAVE_DATA:                     
				SaveData();
				break;

			case CALC_AND_WRITE_CRC: 
				CalcAndWriteCRC();
				break;  

			case ERASE_BLOCK:
				BlockErase();
				break;

    		default: // unknown command
				ASSERT(FALSE);
		}
	}
}


