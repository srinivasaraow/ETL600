/*-------------------------------------------------------------------------*
 * $Workfile	: epromLow.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Low level eprom access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/C/epromLow.c $
 * 
 * 2     07-04-27 15:20 Chalnot
 * Replaced services.h with ccblkfn.h.
 * 
 * 1     07-04-17 10:37 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/             

/* INCLUDE FILES **************************************************************/

#include <ccblkfn.h>
#include "basetype.h"
#include "Basetype_O4CV.h"
#include "epromLow.h" 

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define	BOOT_FLASH_OFFSET	0x20000000		//external Address of parallel FLASH 

// Eprom Commands and codes for an AL032D90BFI04
#define COMMAND_ADDR_554	0x00000554
#define COMMAND_ADDR_2AA	0x000002AA
#define COMMAND_ADDR_AAA	0x00000AAA

#define COMMAND_BYTE_01		0x01
#define COMMAND_BYTE_10		0x10
#define COMMAND_BYTE_30		0x30
#define COMMAND_BYTE_55		0x55
#define COMMAND_BYTE_80		0x80
#define COMMAND_BYTE_88		0x88
#define COMMAND_BYTE_90		0x90
#define COMMAND_BYTE_98		0x98
#define COMMAND_BYTE_A0		0xA0
#define COMMAND_BYTE_AA		0xAA
#define COMMAND_BYTE_B0		0xB0
#define COMMAND_BYTE_F0		0xF0

#define STATUS_BYTE_80		0x00000080
#define STATUS_BYTE_20		0x00000020

// Other constants
#define BY_8_BITS						8
#define BY_16_BITS						16
#define BY_24_BITS						24

// Default values and miscellaneous definitions
#define FF					0x000000ff
#define BYTE1_OF_4			0x000000ff
#define BYTE2_OF_4			0x0000ff00
#define BYTE3_OF_4			0x00ff0000
#define BYTE4_OF_4			0xff000000
#define BYTES1_AND_2_OF_4	0x0000ffff 
#define FOUR_BYTES			4   

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static bool g_bEpromIsBusy;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:      epromLow_resetEprom()  
  Description: 	 Resets the Eprom	                    
  Inputs:      	 None                                                                          
  Outputs:    	 None                                                                                                                                         
  Return:      	 None       	
  Globals:       None
  Important:                                                          	
*******************************************************************************/
void epromLow_resetEprom(void)
{
    UINT32 uiSaveInts;
				
	uiSaveInts = cli();			// Global Interrupt disable
	
	*(UINT16*)(BOOT_FLASH_OFFSET) = COMMAND_BYTE_F0;
	asm("nop;");
	asm("nop;");
						
	sti(uiSaveInts);			// Global Interrupt enable
}


/*****************************************************************************
  Function:      epromLow_eraseSector()  
  Description: 	 Erases a sector of the Eprom	                    
  Inputs:      	 sectorAddress: An address in the sector which should be erased                                                                          
  Outputs:    	 None                                                                                                                                         
  Return:      	 TRUE if Eprom was not busy, else return FALSE        	
  Globals:       None
  Important:                                                          	
*******************************************************************************/
bool epromLow_eraseSector(UINT32 sectorAddress)	
{
	UINT32 uiSaveInts;
	UINT32 localSectorAddress = sectorAddress;
    
   	if (g_bEpromIsBusy)
		return (FALSE);

	uiSaveInts = cli();			// Global Interrupt disable
	
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA)	= COMMAND_BYTE_AA;
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_554)	= COMMAND_BYTE_55;
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA)	= COMMAND_BYTE_80;
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA)	= COMMAND_BYTE_AA;
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_554)	= COMMAND_BYTE_55;
	*(UINT16*)(BOOT_FLASH_OFFSET + localSectorAddress)		= COMMAND_BYTE_30;

	sti(uiSaveInts);			// Global Interrupt enable
	
	g_bEpromIsBusy = TRUE;

	return(TRUE);
}


/*****************************************************************************
  Function:      epromLow_eraseSectorStatus()  
  Description: 	 Checks the Status of the erase procedure	                    
  Inputs:      	 sectorAddress: An address in the sector which should be erased                                                                          
  Outputs:    	 None                                                                                                                                         
  Return:      	 Status of the erase procedure      	
  Globals:       None
  Important:                                                          	
*******************************************************************************/
e_eraseSectorStatus epromLow_eraseSectorStatus(UINT32 sectorAddress)  
{                                   
	UINT16 DQ5;
	UINT16 DQ7;
	UINT16 eraseEpromStatus;
	UINT32 localSectorAddress = sectorAddress;

	// read Status Register and check bits 7 and 5
	eraseEpromStatus =	*(UINT16*)(BOOT_FLASH_OFFSET + localSectorAddress); 
	
	DQ7 = eraseEpromStatus & STATUS_BYTE_80;	
	DQ5 = eraseEpromStatus & STATUS_BYTE_20;	
   
    if (DQ7 == 0) //  Test DQ7	, if one -> erase is finished
	{   
       	if (DQ5 == 0)	
		{ 
			return(e_eraseSectorStatus_AwaitErase);  
		}     // wait for erase to terminate 
       	else
       	{	// retry in case of ayncronus change from status to memory array
			eraseEpromStatus =	*(UINT16*)(BOOT_FLASH_OFFSET + localSectorAddress);  	 
			DQ7 = eraseEpromStatus & STATUS_BYTE_80;
			
			g_bEpromIsBusy = FALSE;

      		if (DQ7 == 0)
			{ 
				return(e_eraseSectorStatus_Timeout); 
			}                  
       		else 
			{
				return(e_eraseSectorStatus_Ok); 
			}  // if DQ7 changes to 1 in the same time, erase is ok
       	} 
    }	
    else  
	{	
		g_bEpromIsBusy = FALSE;
		return(e_eraseSectorStatus_Ok);  
	}
}                                


/*****************************************************************************
  Function:      epromLow_readWordFromEprom()  
  Description: 	 Reads a word from the given address in the Eprom 	                    
  Inputs:      	 pSource: Pointer to the word which should be read    				 	                                                                       
  Outputs:    	 pDestination: Pointer to the destination where the word should
  							   be saved                                                                                                                                        
  Return:      	 TRUE if Eprom was not busy, else return FALSE      	
  Globals:       None
  Important:                                                          	
*******************************************************************************/
bool epromLow_readWordFromEprom (UINT16* pSource, UINT16* pDestination)
{
    UINT16* pLocalSource       = pSource;	
	UINT16* pLocalDestination  = pDestination;
	
	if (g_bEpromIsBusy)
	{
		return(FALSE);
	}
	*pLocalDestination = (*(UINT16*)(BOOT_FLASH_OFFSET + (UINT32)pLocalSource));

	return(TRUE);

}
 
 
/*****************************************************************************
  Function:      epromLow_writeWordToEprom()  
  Description: 	 Writes a word to the given address	in the Eprom                    
  Inputs:      	 wordValue: The word which should be written in the eprom
  				 pDestination: Pointer to the destination where the word should
  							   be saved 
  				 isBlockingWrite: Defines if the function should wait for the 
  				 				  end of the write procedure 			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if Eprom was not busy, else return FALSE        	
  Globals:       None
  Important:     It is better to set isBlockingWrite FALSE and to call the
  				 function epromLow_writeWordToEpromStatus	                                                      	
*******************************************************************************/ 
bool epromLow_writeWordToEprom(UINT16 wordValue, UINT16* pDestination, bool isBlockingWrite)  // works!
{
	UINT16* pLocalDestination = pDestination;;
	UINT16 DQ5;
	UINT16 DQ7;                                                                        
	UINT16 EpromStatus;

	if (g_bEpromIsBusy)
	{
		return(FALSE);
	}
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA) = COMMAND_BYTE_AA;
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_554) = COMMAND_BYTE_55;
	*(UINT16*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA) = COMMAND_BYTE_A0;
	*(UINT16*)(BOOT_FLASH_OFFSET + (UINT32)pLocalDestination) = wordValue;

    if (isBlockingWrite)
    {
		do
		{
			EpromStatus = (*(UINT16*)(BOOT_FLASH_OFFSET + (UINT32)pLocalDestination));			// read DQ7 + DQ5 
			DQ7 = EpromStatus & STATUS_BYTE_80;
			DQ5 = EpromStatus & STATUS_BYTE_20;
				
		// loop until DQ7==data||DQ5==1	
		} 
		while(!((DQ7 == (wordValue & STATUS_BYTE_80)) || DQ5));	 

		// DQ7!=data: Failed, retry
		if(DQ7 != (wordValue & STATUS_BYTE_80)) 
		{
			DQ7 = (((*(UINT16*)(BOOT_FLASH_OFFSET + (UINT32)pLocalDestination))) & STATUS_BYTE_80);
		}
	
		// DQ7!=data: Failed, return with error
		if(DQ7 != (wordValue & STATUS_BYTE_80))
		{
			return(FALSE);	// time-out	 
		}

	}
	else
	{
		g_bEpromIsBusy = TRUE;
	}
	return(TRUE);  
}


/*****************************************************************************
  Function:      epromLow_writeWordToEpromStatus()  
  Description: 	 Checks the status of the write procedure                     
  Inputs:      	 wordValue: The word which should be written in the eprom
  				 pDestination: Pointer to the destination where the word should
  							   be saved 			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 Status of the write procedure     	
  Globals:       None
  Important:                                                          	
*******************************************************************************/ 
 e_EpromWriteStatus epromLow_writeWordToEpromStatus(UINT16 wordValue, UINT16* pDestination)
{
	UINT16* pLocalDestination = pDestination;
	UINT16 DQ5;
	UINT16 DQ7;                                                                         
	UINT16 EpromStatus = 0;

	// Get status register and mask out DQ7 and DQ5
	EpromStatus = (*(UINT16*)(BOOT_FLASH_OFFSET + (UINT32)pLocalDestination));		
	DQ7 = EpromStatus & STATUS_BYTE_80;
	DQ5 = EpromStatus & STATUS_BYTE_20;
    
    if (DQ7 != (wordValue & STATUS_BYTE_80)) //  Test DQ7, if the same as written -> write has finished
	{   
       	if (DQ5 == 0)	
		{ 
			return(e_EpromWriteStatus_AwaitWrite);  
		}     // wait for erase to terminate 
       	else
       	{	// retry in case of ayncronus change from status to memory array
			EpromStatus = (*(UINT16*)(BOOT_FLASH_OFFSET + (UINT32)pLocalDestination));		
			DQ7 = EpromStatus & STATUS_BYTE_80;

			g_bEpromIsBusy = FALSE;

   			if (DQ7 != (wordValue & STATUS_BYTE_80)) 
			{ 
				return(e_EpromWriteStatus_Timeout); 
			}    // else time out error
       		else 
			{ 	
				return(e_EpromWriteStatus_Ok); 
			}  // if DQ7 changes to 1 in the same time, erase is ok
       	} 
    }	
    else  
	{	
		g_bEpromIsBusy = FALSE;
		return(e_EpromWriteStatus_Ok);  
	}
}


