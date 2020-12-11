/*-------------------------------------------------------------------------*
 * $Workfile	: epromHigh.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : High level eprom access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/epromHigh.c $
 * 
 * 4     07-09-12 14:49 Chalnot
 * The functions epromHigh_eraseEFW()  and epromHigh_programEFWData()
 * added for Em Fw Upload.
 * 
 * 3     07-08-30 13:16 Chalnot
 * Emergency Fw update function added.
 * 
 * 2     07-04-19 9:45 Chalnot
 * Modified function headers.
 * 
 * 1     07-04-17 10:37 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/   
 
/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "Basetype_O4CV.h"
#include "O4CV_Types.h"
#include "epromHigh.h" 
#include "epromMed.h" 
#include "epromLow.h" 
#include "crc.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// External address of flash 
#define	BOOT_FLASH_OFFSET	0x20000000		

// Configuration sector
#define CFG_SECTOR        SECTOR_SA55
#define CRC_OFFSET        0xFFFE  

// Firmware sector
#define FIRMWARE_START_ADDRESS    SECTOR_SA39
#define FIRMWARE_END_ADDRESS      SECTOR_SA55

// Emergency Firmware sector
#define EMERGENCY_FIRMWARE_START_ADDRESS    SECTOR_SA8
#define EMERGENCY_FIRMWARE_END_ADDRESS      SECTOR_SA23

/* LOCAL TYPE DECLARATIONS ***************************************************/
 
/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:      epromHigh_init()  
  Description: 	 Initializes the Eprom.               
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       None
  Important:     Must not be called at runtime, because it's using epromMed_emptyStack.                                                     	
*******************************************************************************/ 
void epromHigh_init(void)
{
   	epromLow_resetEprom();
	
	epromMed_emptyStack(); 
}


/*****************************************************************************
  Function:      epromHigh_storeConfiguration()  
  Description: 	 Stores the configuration of the O4CV in the eprom.
  				 First the config sector is erased, then the config is saved 
  				 and finally the crc is calculated and stored at the end of 
  				 the config sector.
				 This function uses the stack.
  Inputs:      	 pO4CVConfig: Pointer to the config struct of the O4CV			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       None
  Important:                                                       	
*******************************************************************************/ 
void epromHigh_storeConfiguration(UINT16* pO4CVConfig)
{
    
    // Erases the config sector
    epromMed_pushCommand(ERASE_SECTOR, 0, (UINT16*) CFG_SECTOR , 0); 
	
    // Writes the config to the config sector
	epromMed_pushCommand(SAVE_DATA,
							(pO4CVConfig),  			// from ram address
							(UINT16*)CFG_SECTOR,		// to flash address
							sizeof(t_O4CVConfig));    	// number of bytes
	
	// Calculates the crc of the config and writes it at the end of the sector
	epromMed_pushCommand(CALC_AND_WRITE_CRC, 
						(UINT16*)(CFG_SECTOR), 				// cfg start address
						(UINT16*)(CFG_SECTOR + CRC_OFFSET), // crc address  
			        	sizeof(t_O4CVConfig));           	// number of bytes
			       			           
}
  

/*****************************************************************************
  Function:      epromHigh_readConfiguration()  
  Description: 	 Reads the config of the O4CV in the eprom.   
  				 It calculates the crc and compares it with the one in the eprom.           
  Inputs:      	 pO4CVConfig: Pointer to the config struct of the O4CV			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if the config was read successfully and the crc was
  				 correct, else return FALSE    	
  Globals:       None
  Important:     Must not be called during normal tasking operation!!!	                                                  	
*******************************************************************************/ 
bool epromHigh_readConfiguration(UINT16* pO4CVConfig)
{
  
	UINT32 length = sizeof(t_O4CVConfig);		// In bytes
	UINT16* AddrFrom = (UINT16*)(CFG_SECTOR);
	UINT16 value = 0;
	UINT32 byteCounter = 0;
	UINT16 calcCRC = 0xCACA; 
    	
	// Read the config word after word and calculate the crc
	while (byteCounter < length)
	{
	    epromLow_readWordFromEprom(AddrFrom, &value);
	    *pO4CVConfig = value;
	   	calcCRC = crc16(calcCRC, (value & MASK_8LSB));
		calcCRC = crc16(calcCRC, ((value >> 8) & MASK_8LSB));
							
		AddrFrom++;
		pO4CVConfig++;
		byteCounter += 2;	// +2 because word 
	}
	
	// Check if the calculated CRC match with the CRC in the Eprom
	epromLow_readWordFromEprom((UINT16*)(CFG_SECTOR + CRC_OFFSET), &value);
	
	if (value == calcCRC)
	{
	    return(TRUE);
	}
	else
	{
	    return(FALSE);
	}
	
}


/*****************************************************************************
  Function:      epromHigh_programData()  
  Description: 	 Writes a firmware to the eprom  
  				 During a firmware update, this routine is called a number of 
  				 times with 20H bytes to write. If an error is returned, 
  				 this will be indicated on HMI. 
				 This function uses the stack.
  Inputs:      	 length: Length of the data package
  				 offset: Offset of the data package
  				 pData: Pointer to the data			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if there is enough space in the firmware sector, 
  				 else return FALSE    	
  Globals:       None
  Important:   	 pData must be an even address!!!                                                     	
*******************************************************************************/  
bool epromHigh_programData(UINT32 length, UINT32 offset, UINT16* pData)
{
	
    UINT16* addressInEprom = (UINT16*)(FIRMWARE_START_ADDRESS + offset);  
  	  
// Range plausibility check : Make sure that no WRITE operation can accidentally
// overwrite the config sector. Unfortunately, by this time, the Eprom will 
// already have been erased, making damage inevitable. But like this, FW update
// will at least terminate immediately if address is not plausible.

	if ((UINT32)addressInEprom + length > FIRMWARE_END_ADDRESS)
	{
	   	return(FALSE);			    	
	}   
    
	epromMed_pushCommand(SAVE_DATA, pData, addressInEprom , length); 
								
    return(TRUE);
}


/*****************************************************************************
  Function:      epromHigh_programEFWData()  
  Description: 	 Writes an emergency firmware to the eprom  
  				 During a firmware update, this routine is called a number of 
  				 times with 20H bytes to write. If an error is returned, 
  				 this will be indicated on HMI. 
				 This function uses the stack.
  Inputs:      	 length: Length of the data package
  				 offset: Offset of the data package
  				 pData: Pointer to the data			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if there is enough space in the firmware sector, 
  				 else return FALSE    	
  Globals:       None
  Important:   	 pData must be an even address!!!                                                     	
*******************************************************************************/  
bool epromHigh_programEFWData(UINT32 length, UINT32 offset, UINT16* pData)
{
	
    UINT16* addressInEprom = (UINT16*)(EMERGENCY_FIRMWARE_START_ADDRESS + offset);  
  	  
// Range plausibility check : Make sure that no WRITE operation can accidentally
// overwrite the config sector. Unfortunately, by this time, the Eprom will 
// already have been erased, making damage inevitable. But like this, FW update
// will at least terminate immediately if address is not plausible.

	if ((UINT32)addressInEprom + length > EMERGENCY_FIRMWARE_END_ADDRESS)
	{
	   	return(FALSE);			    	
	}   
    
	epromMed_pushCommand(SAVE_DATA, pData, addressInEprom , length); 
								
    return(TRUE);
}


/*****************************************************************************
  Function:      epromHigh_eraseFirmware()  
  Description: 	 Erases the firmware sector of the eprom.
				 This function uses the stack.
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None 	
  Globals:       None
  Important:                                                       	
*******************************************************************************/ 
void epromHigh_eraseFirmware(void)
{
    UINT32 sector = FIRMWARE_START_ADDRESS;
    
    // Sector 0 to 7 are 8kB
    while (sector < SECTOR_SA8)
	{
		epromMed_pushCommand(ERASE_SECTOR,0,(UINT16*)sector,0);   
		sector += SECTOR_SIZE_8K;
	}
	
	// Sector 8 to 70 are 64kB
    while (sector < FIRMWARE_END_ADDRESS)
	{
		epromMed_pushCommand(ERASE_SECTOR,0,(UINT16*)sector,0);   
		sector += SECTOR_SIZE_64K;
	}
}    


/*****************************************************************************
  Function:      epromHigh_eraseEFW()  
  Description: 	 Erases the emergency firmware sector of the eprom.
				 This function uses the stack.
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None 	
  Globals:       None
  Important:                                                       	
*******************************************************************************/ 
void epromHigh_eraseEFW(void)
{
    UINT32 sector = EMERGENCY_FIRMWARE_START_ADDRESS;
    
    // Sector 0 to 7 are 8kB
    while (sector < SECTOR_SA8)
	{
		epromMed_pushCommand(ERASE_SECTOR,0,(UINT16*)sector,0);   
		sector += SECTOR_SIZE_8K;
	}
	
	// Sector 8 to 70 are 64kB
    while (sector < EMERGENCY_FIRMWARE_END_ADDRESS)
	{
		epromMed_pushCommand(ERASE_SECTOR,0,(UINT16*)sector,0);   
		sector += SECTOR_SIZE_64K;
	}
}  

