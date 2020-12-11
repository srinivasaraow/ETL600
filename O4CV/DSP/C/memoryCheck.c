/*-------------------------------------------------------------------------*
 * $Workfile	: MemoryCheck.c $
 * Part of		: ETL600 / O4CV
 * Language		: C
 * Created by	: Allen Notter (AN)
 * Remarks		:  
 * Purpose		: Program and constant data memory check 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/memoryCheck.c $
 * 
 * 2     07-09-04 16:40 Chalnot
 * Memory crc error bit set in status.
 * 
 * 1     07-08-30 10:48 Chalnot
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "Basetype_O4CV.h"
#include "crc.h"
#include "epromMed.h"
#include "status.h"

#include <cdefBF538.h>

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define CHECK_CASE_L1_CODE				0
#define CHECK_CASE_L1_DATA_CONST		1
#define CHECK_CASE_SDRAM0_BANK0			2
#define CALCULATE_OVERALL_CRC			3
#define COMPARE_OVERALL_CRC				4

#define PATCH_DATA_ADDRESS	0x2F0000		// Address in flash
#define BOOT_TRIES_ADDRESS	0x3F0000		// Address in flash	
#define FLASH_OFFSET		0x20000000

/* LOCAL TYPE DECLARATIONS ***************************************************/

// structure crc values for status  
typedef struct{
	UINT32	crc_L1_Code;
	UINT32	crc_L1_Data_Const;
	UINT32	crc_Sdram0_Bank0;
}t_crcStatus;

// structure for register read
typedef struct{ 
	UINT32  low;
  	UINT32  high;	  
}t_regData;

// structure for crc and start/stop addresses
typedef struct{ 
	UINT32 crc_sum;
	UINT32 l1_CodeStart;
	UINT32 l1_CodeStop;
	UINT32 l1_Data_ConstStart;
	UINT32 l1_Data_ConstStop;
	UINT32 sdram0_Bank0Start;
	UINT32 sdram0_Bank0Stop;
}t_patchData;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static UINT8 g_memData[8];
static t_crcStatus g_crcStatus;
static t_patchData g_patchData;
static bool g_memoryCheckAlarm;

/* LOCAL FUNCTIONS ***********************************************************/

static void get8BytesFromInternalPM(UINT32 address, t_regData *regData);
static void checkWholeMemorySegment(UINT32 startAddress, UINT32 stopAddress, UINT32* pActual_CRC16, bool isInternalPM);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		get8BytesFromInternalPM()
  Description:	Reads 8 bytes from the internal program memory, which can not 
  				be accessed directly, but via the data test registers  
  Inputs:		address: Address to read the 8 bytes
  Outputs:		regData: Pointer to regData structure
  Return:       None
  Globals:		None
  Important:
*******************************************************************************/
static void get8BytesFromInternalPM(UINT32 address, t_regData* regData)
{
	UINT32 commandValue = 0x01000004; 
	
	
	// Address bit 15
	if(address & BIT15)
	{
	 	commandValue |= BIT23;   
	}
	else
	{
		commandValue &= ~BIT23; 
	}
	
	// Address bit 14
	if(address & BIT14)
	{
	 	commandValue |= BIT14;   
	}
	else
	{
		commandValue &= ~BIT14; 
	}
	
	// Address bit 13
	if(address & BIT13)
	{
	 	commandValue |= BIT17;   
	}
	else
	{
		commandValue &= ~BIT17; 
	}
	
	// Address bit 12
	if(address & BIT12)
	{
	 	commandValue |= BIT16;   
	}
	else
	{
		commandValue &= ~BIT16; 
	}
	
	// Address bit 11
	if(address & BIT11)
	{
	 	commandValue |= BIT26;   
	}
	else
	{
		commandValue &= ~BIT26; 
	}
	
	// Address bit 3-10
	commandValue |= address & 0x07F8;

	// Set address in the dtest command register	
	*pDTEST_COMMAND = commandValue;
	csync();
	
	// Read 8 bytes from the dtest data registers
	regData->low = *pDTEST_DATA0;
	regData->high = *pDTEST_DATA1;

}    


/*****************************************************************************
  Function:		checkWholeMemorySegment()
  Description:	Calculates the crc over a whole segment. If the segment is
  				the L1 code segment the get8BytesFromInternalPM function
  				is used.
  Inputs:		startAddress: Startaddress of the segment
  				stopAddress: Stopaddress of the segment
  				isInternalPM: Defines if it is the L1 code segment
  Outputs:		pActualCRC16: Pointer to the actual crc value
  Return:       None
  Globals:		g_memData
  Important:	If there are any breakpoints in the code the crc sum will be
  				wrong! The debugger always sets a breakpoint at the beginning 
  				of the main!
*******************************************************************************/
static void checkWholeMemorySegment(UINT32 startAddress, UINT32 stopAddress, UINT32* pActualCRC16, bool isInternalPM)
{
	UINT32 	tempCRC = 0;
	UINT32 	numberOfBytesLeft = 0;
	UINT32 	actualAddress = 0;		// In 8 Byte steps		
	UINT16 	byteNumber = 0;			// Byte number 0 to 7 
	
	
	// Calculate the number of bytes
	numberOfBytesLeft = (stopAddress - startAddress); 
	
	// Calculate the address without the 3 lsb
	actualAddress = startAddress & 0xFFFFFFF8;
	
	// Check which of the 8 bytes is the first byte
	byteNumber = startAddress & 0x7; 
	
	// Get first 8 bytes
	if(isInternalPM)
	{
		get8BytesFromInternalPM(actualAddress, (t_regData *) &g_memData[0]);
	}
	else
	{
		memcpy(&g_memData[0], (UINT8*) actualAddress, 8);  
	}
	
	while(numberOfBytesLeft > 0)
	{
	    // Calculate crc
	    tempCRC = crc16(tempCRC, g_memData[byteNumber]);
		numberOfBytesLeft--;
	    
		// If it was the last byte of the 8 bytes, the next 8 bytes will be loaded  
		if(byteNumber >=7)
		{
		    actualAddress += 8;
		    byteNumber = 0;  
		    
		    // Get next 8 bytes
		    if(isInternalPM)
			{
				get8BytesFromInternalPM(actualAddress, (t_regData *) &g_memData[0]);
			}
			else
			{
				memcpy(&g_memData[0], (UINT8*) actualAddress, 8);  
			}
		}
		else
		{
		    byteNumber++;
		}
	}
	
	*pActualCRC16 = tempCRC;
	
}


/*****************************************************************************
  Function:		memoryCheck_initializeMemoryCheck()
  Description:	Loads the patch data from the flash into g_patchData
  Inputs:		None
  Outputs:		None
  Return:       None
  Globals:		g_patchData
  Important:	
*******************************************************************************/
void memoryCheck_initializeMemoryCheck(void)
{
	memcpy(&g_patchData, (UINT8*) (PATCH_DATA_ADDRESS + FLASH_OFFSET), sizeof(t_patchData));  
	
	g_memoryCheckAlarm = false;
}


/*****************************************************************************
  Function:		memoryCheck_startupMemoryCheck()
  Description:	Checks all segments and sets the alarm if the crc is not equal
  				with the one stored in the flash
  Inputs:		None
  Outputs:		None
  Return:       None
  Globals:		g_patchData, g_crcStatus, g_memoryCheckAlarm
  Important:	
*******************************************************************************/
void memoryCheck_startupMemoryCheck(void)
{
    UINT32 tempCRC = 0;
    
    
	memoryCheck_initializeMemoryCheck();
	
	// Calculate crc of L1 code segment
	checkWholeMemorySegment(g_patchData.l1_CodeStart, 
		g_patchData.l1_CodeStop, &g_crcStatus.crc_L1_Code, true); 

	// Calculate crc of L1 const data segment
	checkWholeMemorySegment(g_patchData.l1_Data_ConstStart, 
		g_patchData.l1_Data_ConstStop, &g_crcStatus.crc_L1_Data_Const, false); 

	// Calculate crc of sdram0 bank0 segment
	checkWholeMemorySegment(g_patchData.sdram0_Bank0Start, 
		g_patchData.sdram0_Bank0Stop, &g_crcStatus.crc_Sdram0_Bank0, false);

	// Calculate overall crc sum	
	tempCRC = crc16(tempCRC, g_crcStatus.crc_L1_Code);
	tempCRC = crc16(tempCRC, g_crcStatus.crc_L1_Data_Const);
	tempCRC = crc16(tempCRC, g_crcStatus.crc_Sdram0_Bank0);

	// Set alarm if crcs are not equal
	g_memoryCheckAlarm = false; 
	if(tempCRC != g_patchData.crc_sum)
	{
	 	g_memoryCheckAlarm = true;  
	}

}


/*****************************************************************************
  Function:		memoryCheck_doMemoryCheck()
  Description:	Checks all segments and sets the alarm if the crc is not equal
  				with the one stored in the flash. It makes the same as the
  				function memoryCheck_startupMemoryCheck but it can be used 
  				during runtime.	For a whole memory check this function must 
  				be called approx. 1.125 * (number of bytes to check) times
  Inputs:		None
  Outputs:		None
  Return:       None
  Globals:		g_patchData, g_crcStatus, g_memoryCheckAlarm
  Important:	
*******************************************************************************/
void memoryCheck_doMemoryCheck(void)
{
	static bool isCheckStarted = false;
	static bool getNext8Bytes = false;
	static UINT8* actualAddress;
	static UINT16 byteNumber = 0;
	static UINT16 tempCRC = 0;
	static UINT16 checkWhichSegment = CHECK_CASE_L1_CODE;
	static UINT16 overallCRCSegment = CHECK_CASE_L1_CODE;

	
	switch(checkWhichSegment)
	{
		case CHECK_CASE_L1_CODE:

			if (!isCheckStarted)
			{
			    // Prepare for crc calculation of this segment
			    actualAddress = (UINT8*) (g_patchData.l1_CodeStart & 0xFFFFFFF8);
				byteNumber = g_patchData.l1_CodeStart & 0x7; 
				tempCRC = 0;
				isCheckStarted = true;
				getNext8Bytes = true;
			}
			else	// Check is started
			{
			    // Check if address is still in this segment
				if ((UINT32) (actualAddress + byteNumber) < g_patchData.l1_CodeStop)
				{
					if(getNext8Bytes)
				    {
				        // Copy next 8 bytes from the memory to the g_memData array
				      	get8BytesFromInternalPM((UINT32) actualAddress, (t_regData *) &g_memData[0]); 
			        	getNext8Bytes = false;
				    }
				    else
				    { 
						// Calculate crc with every byte
						tempCRC = crc16(tempCRC, g_memData[byteNumber]);

						// If it was the last byte of the 8 bytes, read next 8 bytes 
						if(byteNumber >=7)
						{
							actualAddress += 8;
							byteNumber = 0;
							getNext8Bytes = true;   
						}
						else
						{
						    byteNumber++;
						}
					}
				}
				else	// CRC calculation of this segment is finished
				{
				    // Prepare for next segment
				  	g_crcStatus.crc_L1_Code = tempCRC;
					isCheckStarted = false;
					checkWhichSegment = CHECK_CASE_L1_DATA_CONST;  
				}	
			}
			break;
		
		case CHECK_CASE_L1_DATA_CONST:

			if (!isCheckStarted)
			{
			    // Prepare for crc calculation of this segment
			    actualAddress = (UINT8*) (g_patchData.l1_Data_ConstStart & 0xFFFFFFF8);
				byteNumber = g_patchData.l1_Data_ConstStart & 0x7; 
				tempCRC = 0;
				isCheckStarted = true;
				getNext8Bytes = true;
			}
			else	// Check is started
			{
			    // Check if address is still in this segment
				if ((UINT32) (actualAddress + byteNumber) < g_patchData.l1_Data_ConstStop)
				{
					if(getNext8Bytes)
				    {
				        // Copy next 8 bytes from the memory to the g_memData array
				      	memcpy(&g_memData[0], actualAddress, 8);  
			        	getNext8Bytes = false;
				    }
				    else
				    { 
						// Calculate crc with every byte
						tempCRC = crc16(tempCRC, g_memData[byteNumber]);
						
						// If it was the last byte of the 8 bytes, read next 8 bytes 
						if(byteNumber >=7)
						{
							actualAddress += 8;
							byteNumber = 0;
							getNext8Bytes = true;   
						}
						else
						{
						    byteNumber++;
						}
					}
				}
				else	// CRC calculation of this segment is finished
				{
				    // Prepare for next segment
				  	g_crcStatus.crc_L1_Data_Const = tempCRC;
					isCheckStarted = false;
					checkWhichSegment = CHECK_CASE_SDRAM0_BANK0;  
				}	
			}
			break;

		case CHECK_CASE_SDRAM0_BANK0:

			if (!isCheckStarted)
			{
			    // Prepare for crc calculation of this segment
			    actualAddress = (UINT8*) (g_patchData.sdram0_Bank0Start & 0xFFFFFFF8);
				byteNumber = g_patchData.sdram0_Bank0Start & 0x7; 
				tempCRC = 0;
				isCheckStarted = true;
				getNext8Bytes = true;
			}
			else	// Check is started
			{
			    // Check if address is still in this segment
				if ((UINT32) (actualAddress + byteNumber) < g_patchData.sdram0_Bank0Stop)
				{
					if(getNext8Bytes)
				    {
				        // Copy next 8 bytes from the memory to the g_memData array
				      	memcpy(&g_memData[0], actualAddress, 8);  
			        	getNext8Bytes = false;
				    }
				    else
				    { 
						// Calculate crc with every byte
						tempCRC = crc16(tempCRC, g_memData[byteNumber]);
						
						// If it was the last byte of the 8 bytes, read next 8 bytes 
						if(byteNumber >=7)
						{
							actualAddress += 8;
							byteNumber = 0;
							getNext8Bytes = true;   
						}
						else
						{
						    byteNumber++;
						}
					}
				}
				else	// CRC calculation of this segment is finished
				{
				    // Prepare for overall crc calculation
				  	g_crcStatus.crc_Sdram0_Bank0 = tempCRC;
					isCheckStarted = false;
					checkWhichSegment = CALCULATE_OVERALL_CRC;  
				}	
			}
			break;
	
		case CALCULATE_OVERALL_CRC:	
	
			switch(overallCRCSegment)
			{
			 	case CHECK_CASE_L1_CODE:
			 	
			 		tempCRC = 0;
			 		tempCRC = crc16(tempCRC, g_crcStatus.crc_L1_Code);
			 		overallCRCSegment = CHECK_CASE_L1_DATA_CONST;	
			 		break;   
			 		
			 	case CHECK_CASE_L1_DATA_CONST:
			 	
			 		tempCRC = crc16(tempCRC, g_crcStatus.crc_L1_Data_Const);
			 		overallCRCSegment = CHECK_CASE_SDRAM0_BANK0;	
			 		break;  
			 		
			 	case CHECK_CASE_SDRAM0_BANK0:
			 	
			 		tempCRC = crc16(tempCRC, g_crcStatus.crc_Sdram0_Bank0);
			 		overallCRCSegment = COMPARE_OVERALL_CRC;	
			 		break;  
			 		
			 	case COMPARE_OVERALL_CRC:
			 	
			 		if(tempCRC == g_patchData.crc_sum)
			 		{
			 		 	// Memory is still correct
			 		 	g_memoryCheckAlarm = false;
			 		 	status_clearAlarm(O4CV_ALARM_MEMORY_CRC_ERROR);
			 		}
			 		else
			 		{
			 		  	// Memory is corrupt
			 		  	g_memoryCheckAlarm = true;
			 		  	status_setAlarm(O4CV_ALARM_MEMORY_CRC_ERROR);
			 		}
			 		   
			 		// Initialize for next check
			 		overallCRCSegment = CHECK_CASE_L1_CODE;	
			 		checkWhichSegment = CHECK_CASE_L1_CODE;
			 		isCheckStarted = false;
			 		break;  	
			 		
			 	default:
					break;
					
			}// end switch(overallCRCSegment)	
	
			break;
			
		default:
			break;
			
	}// end switch(checkWhichSegment)

}


/*****************************************************************************
  Function:		memoryCheck_getCrcAlarm()
  Description:	Returns the alarm state
  Inputs:		None
  Outputs:		None
  Return:       Alarm state
  Globals:		g_memoryCheckAlarm
  Important:	
*******************************************************************************/
bool memoryCheck_getCrcAlarm(void)
{
	if(g_memoryCheckAlarm) 
	{
		return (true);
	}
	else
	{
		return (false);
	}
}


/*****************************************************************************
  Function:		memoryCheck_resetBootTries()
  Description:	Sets the variable bootTries to 0 in the flash
  Inputs:		None
  Outputs:		None
  Return:       None
  Globals:		None
  Important:	This function should not be called during runtime
*******************************************************************************/
void memoryCheck_resetBootTries(void)
{
    UINT16 bootTries = 0;
    UINT16* pBootTries = (UINT16*) (BOOT_TRIES_ADDRESS);
    
    
    // Push erase and write commands
    epromMed_pushCommand(ERASE_SECTOR, &bootTries, pBootTries, 2);	 
    
    epromMed_pushCommand(SAVE_DATA, &bootTries, pBootTries, 2);	
    
    // Wait until commands are finished
    epromMed_emptyStack(); 
    
}




