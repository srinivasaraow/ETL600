/*-------------------------------------------------------------------------*
 * $Workfile	: epromMed.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Med level eprom access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/epromMed.c $
 * 
 * 3     07-12-13 14:28 Chalnot
 * Local function prototype corrected.
 * 
 * 2     07-10-08 15:18 Chalnot
 * Refresh watchdog during epromMed_emptyStack().
 * 
 * 1     07-04-17 10:38 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/    
 
/* INCLUDE FILES **************************************************************/
  
#include "basetype.h"
#include "Basetype_O4CV.h"
#include "epromMed.h"
#include "epromLow.h"  
#include "crc.h"
#include "watchdog.h"

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
	UINT16 command;
	UINT16 *addrFrom;
	UINT16 *addrTo;
	UINT32 length;
} 
t_CommandStackElement;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static t_CommandStackElement g_commandStack[COMMAND_STACK_SIZE];	// Eprom Option stack   
static UINT32 g_readPointer; 
static UINT32 g_writePointer;
static bool g_epromIsBusy;							

/* LOCAL FUNCTIONS ***********************************************************/

static void pullCommand(void);
static void saveData(void);
static void calcAndWriteCRC(void);
static void eraseSector(void);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:      eprommed_emptyStack()  
  Description: 	 Executes all commands in the stack                    
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       None
  Important:     Must not be called during normal tasking operation!!!	                                                      	
*******************************************************************************/ 
void epromMed_emptyStack(void) // 
{
    while(epromMed_commandsInStack())
	{
	    watchdog_refresh();
		epromMed_activate(); 
	}
}


/*****************************************************************************
  Function:      epromMed_pushCommand()  
  Description: 	 Stores a command in the stack. A ring buffer is used.                    
  Inputs:      	 option: The command
  				 addrFrom: The source address
  				 addrTo: The destination address
  				 length: The length of the data			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       None
  Important:                                                        	
*******************************************************************************/ 
void epromMed_pushCommand(UINT16 option, UINT16* addrFrom, UINT16* addrTo, UINT32 length)	        
{   
	// Push option if there is space left on the stack
	if (g_readPointer != (g_writePointer + 1) % COMMAND_STACK_SIZE)
	{   
		g_commandStack[g_writePointer].command  = option;
		g_commandStack[g_writePointer].addrFrom = addrFrom;
		g_commandStack[g_writePointer].addrTo   = addrTo;
		g_commandStack[g_writePointer].length   = length; 

		g_writePointer = (g_writePointer + 1) % COMMAND_STACK_SIZE;
	}	
	//else discard commands
}


/*****************************************************************************
  Function:      pullCommand()  
  Description: 	 Removes the oldest command                  
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       None
  Important:                                                          	
*******************************************************************************/ 
static void pullCommand(void)
{	
    // Do nothing if stack is already empty
	if (g_writePointer != g_readPointer)
	{
		g_readPointer = (g_readPointer + 1) % COMMAND_STACK_SIZE;
	}
}				


/*****************************************************************************
  Function:      epromMed_commandsInStack()  
  Description: 	 Checks if there are any commands in the stack                
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if there are commands in the stack, else return FALSE    	
  Globals:       None
  Important:                                                          	
*******************************************************************************/ 
bool epromMed_commandsInStack(void)	        
{
	return(g_writePointer != g_readPointer);	
}

 
/*****************************************************************************
  Function:      saveData()  
  Description: 	 Handles the SAVE_DATA command 
  				 It is called by epromMed_activate               
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       g_commandStack: Contains the commands with all parameters
  Important:     It must be called multiple times to completely write some data                                                     	
*******************************************************************************/
static void saveData(void)
{   
	static UINT16* addrTo;
	static UINT16* addrFrom;
	static UINT32 length;
	static UINT16 data;
	
    static UINT16 writeCase = CASE_0; 
	static bool	bWriting; 

	static UINT32 byteCounter = 0;	
    static bool bSuccess;
    static e_EpromWriteStatus epromWriteStatus;                    
	
	switch(writeCase)
	{                                               
		// Case 0: initialization
		case CASE_0: 

	        addrFrom = (UINT16 *) g_commandStack[g_readPointer].addrFrom;
            addrTo   = (UINT16 *) g_commandStack[g_readPointer].addrTo;
            length   = g_commandStack[g_readPointer].length; 
			byteCounter = 0;  
            bWriting = TRUE;
                  
         	// Check if data is in eprom or not	
			if (addrFrom < (UINT16*)EPROM_SIZE)
			{	
				epromLow_readWordFromEprom (addrFrom, &data);
			}
			else
			{
			 	data = *addrFrom; 	   
			}
			addrFrom++; 
			           
            writeCase = CASE_1;   
            // There's intentionally no break here !   
			
		case CASE_1:

			if (bWriting)
            {
				if(epromLow_writeWordToEprom(data, 
					                addrTo, 
					                FALSE)) // non-blocking write
				{
					bWriting = FALSE;
				}
				// else retry because eprom was busy
    		}
    		else
    		{           
				epromWriteStatus = epromLow_writeWordToEpromStatus(data, addrTo);
    			// Check if writing has terminated, if so, allow next char to be written.
    			if (e_EpromWriteStatus_Ok == epromWriteStatus)
    			{
					byteCounter += 2;
					
					// Check if data is in eprom or not	
					if (addrFrom < (UINT16*)EPROM_SIZE)
					{
						epromLow_readWordFromEprom (addrFrom, &data);
					}
					else
					{
					 	data = *addrFrom; 	   
					}
					addrFrom++; 

					if (byteCounter >= length) 
					{
						pullCommand();
						g_epromIsBusy = FALSE;
						writeCase = CASE_0;
					}

					addrTo++;   
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
	            		pullCommand();

						writeCase = CASE_0; 

						epromLow_resetEprom(); 
						g_epromIsBusy = FALSE;
    	       		}	
            	}	
    		}	
			break;   
			
		default:
		
			break; 
	}           
}                                       
 

/*****************************************************************************
  Function:      eraseSector()  
  Description: 	 Handles the ERASE_SECTOR command 
  				 It is called by epromMed_activate               
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       g_commandStack: Contains the commands with all parameters
  Important:     It must be called multiple times to completely erase a sector                                                     	
*******************************************************************************/
static void eraseSector(void)
{
	static UINT16 eraseCase = CASE_0;
	static UINT32 addr;
    static e_eraseSectorStatus eraseReturnValue = e_eraseSectorStatus_Ok; 

	switch (eraseCase)
	{                                                                          
		// Case 0 does the initialization part.
		case CASE_0:	
			
			addr = (UINT32)g_commandStack[g_readPointer].addrTo;
            if (epromLow_eraseSector(addr))   // retry if eprom was busy
			{
				eraseCase = CASE_1;   
			}
            break;

        case CASE_1: 

            eraseReturnValue = epromLow_eraseSectorStatus(addr);

        	if (eraseReturnValue == e_eraseSectorStatus_Ok)
        	{  
        		pullCommand();
        		g_epromIsBusy = FALSE;
        		eraseCase = CASE_0;
        	}
        	else 
        	{ 
        		if(eraseReturnValue == e_eraseSectorStatus_AwaitErase) 
				{ 
					eraseReturnValue = e_eraseSectorStatus_Ok; 
				}  // WAIT_FOR_ERASE ist kein Fehler
        		else 
        		{  // Timeout
        			pullCommand();
					epromLow_resetEprom(); 
        			g_epromIsBusy = FALSE;
        			eraseCase = CASE_0;
        		}
        	}
        	break;

		default:	

			break; 
	}	
}                                           


/*****************************************************************************
  Function:      calcAndWriteCRC()  
  Description: 	 Handles the CALC_AND_WRITE_CRC command 
  				 It is called by epromMed_activate               
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       g_commandStack: Contains the commands with all parameters
  Important:     It must be called multiple times to calculate und write the crc                                                     	
*******************************************************************************/  
static void calcAndWriteCRC(void)
{
	static UINT16 calcCase = CASE_0; 
    static UINT16 calcCRC; 
    static UINT16* addrFrom;
    static UINT16* addrTo;
    static UINT16 length; 
    static UINT32 counter;
    
	switch (calcCase)
	{                                                                          
		case CASE_0:

			addrFrom = (UINT16*) g_commandStack[g_readPointer].addrFrom;
            addrTo   = (UINT16*) g_commandStack[g_readPointer].addrTo;
            length   = (UINT16) g_commandStack[g_readPointer].length;    

            counter = 0;
		    calcCRC = VOIDCRC;
			
			calcCase = CASE_1;	
            break;

        case CASE_1:  
			{
				UINT16 value;

				if (epromLow_readWordFromEprom(addrFrom, &value))
				{
					calcCRC = crc16(calcCRC, (value & MASK_8LSB));
					calcCRC = crc16(calcCRC, ((value >> 8) & MASK_8LSB));
					
					addrFrom++;
					counter += 2;

					if (!(counter < length)) 
					{ 
						calcCase = CASE_2; 
					}
				}
				// else try again
			}
			break;	 

		case CASE_2:

			pullCommand(); 
			epromMed_pushCommand(SAVE_DATA, &calcCRC, addrTo, 1);
			g_epromIsBusy = FALSE;
			calcCase = CASE_0;
			break;	

		default:	
		
			break; 
	}
}                   
                                    									

/*****************************************************************************
  Function:      epromMed_activate()  
  Description: 	 Reads the commands in the stack and calls the corresponding
 				 function               
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       g_commandStack: Contains the commands with all parameters
  Important:     It is normally called periodically                                                  	
*******************************************************************************/ 
void epromMed_activate(void)
{
	if (epromMed_commandsInStack()) 
	{ 
		g_epromIsBusy = TRUE; // is set to false again in respective command function

		switch (g_commandStack[g_readPointer].command)
		{
    		case SAVE_DATA:                     
				saveData();
				break;

			case CALC_AND_WRITE_CRC: 
				calcAndWriteCRC();
				break;  

			case ERASE_SECTOR:
				eraseSector();
				break;

    		default: // unknown command
    		
				break; 
		}
	}
}


