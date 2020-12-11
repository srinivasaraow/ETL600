/*-------------------------------------------------------------------------*
 * $Workfile: EPROMLOW.C $
 * Part of	: ETL600
 * Language	: C
 * Created by	: CLEMENS BUERGI    08.12.1997
 * Remarks	:     
 * Purpose	:     low level eprom access
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/EPROMLOW.C $
 * 
 * 3     11.04.05 18:32 Chchlee
 * Concurrent flash access bug fixed
 * 
 * 2     16.02.05 10:31 Chchlee
 * Upgrade to Ptty2
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 6     2.08.04 8:17 Leeb02
 * 
 * 5     30.07.04 11:04 Leeb02
 * 
 * 4     29.07.04 14:58 Leeb02
 * 
 * 3     29.07.04 11:39 Buhl01
 * adapt functions for P4LT HW
 * 
 * 2     21.07.04 12:47 Leeb02
 * clearer interface
 * 
 * 1     21.07.04 12:19 Leeb02
 *-------------------------------------------------------------------------*/
                  
             

#include <def21161.h>

#ifndef CODECHECK
	asm("#include <def21161.h>"); 
#endif

#include "EPROMLOW.H" 


#define	BOOT_FLASH_OFFSET	0x04000000		//external Address of parallel FLASH (/MS1)

// Eprom Commands and codes for an AM29LV320DB
#define COMMAND_ADDR_555	0x00000555
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


static BOOL g_bEpromIsBusy;

void resetEprom(void)
{
	unsigned long temp;

	asm("bit clr mode1 IRPTEN;");					// Global Interrupt disable
	*(long*)(BOOT_FLASH_OFFSET) = COMMAND_BYTE_F0;
	asm("nop;");
	asm("nop;");
	temp = *(long*)(BOOT_FLASH_OFFSET);				// Read for fun
	asm("bit set mode1 IRPTEN;");					// Global Interrupt enable
}



BOOL eraseBlock(long blockAddress)			// erase Sector!!! Block is a bloody old expression from P4LQ
{
	unsigned long sectorAdr;

	if (g_bEpromIsBusy)
		return FALSE;

	sectorAdr = (unsigned long)blockAddress;

	asm("bit clr mode1 IRPTEN;");					// Global Interrupt disable

	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA)	= COMMAND_BYTE_AA;
	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_555)	= COMMAND_BYTE_55;
	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA)	= COMMAND_BYTE_80;
	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA)	= COMMAND_BYTE_AA;
	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_555)	= COMMAND_BYTE_55;
	*(long*)(BOOT_FLASH_OFFSET + sectorAdr)			= COMMAND_BYTE_30;

	asm("bit set mode1 IRPTEN;");					// Global Interrupt enable

	g_bEpromIsBusy = TRUE;

	return TRUE;
}

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* long eraseBlockIsOK()									 				*/
/* Check state of erase process initiated by BlockErase() 					*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------                                                          
 * This subroutine determines whether the erasing procedure has been per-
 * formed properly. This is done by checking the Eprom state register.
 *                      
 * Input
 * -----
 *
 * Output
 * ------
 * Return Value is EPROM_ERROR to indicate that the erase process was not (yet?) ter-
 * minated properly. NO_EPROM_ERROR indicates the process has terminated without er-
 * rors.
 *
 * Change History
 * --------------
 * 25.07.97 Erste Version Clemens Bürgi
 *			Kopiert von eraseIsOK und umgeschrieben für irgendeinen Epromblock
 * 7.10.98	KORREKTUR: DQ7 nochmals checken nach einem Timeout: da as umschalten vom Statusregister
 *			zum Flash Memory asynchron (gleichzeitig!!!!) zum Lesezugriff erfolgen kann!!
 * 28.07.04 P4LT !  
 * 				*** This is a EPROM Type dependent routine ***
 */
e_EraseBlockStatus eraseBlockStatus(long blockAddress)  // works!
{                                   
	long DQ5, DQ7;
	unsigned long sectorAdr, eraseEpromStatus;
	sectorAdr = (unsigned long)blockAddress;

	// read Status Register and check bits 7 and 5
	eraseEpromStatus =	*(long*)(BOOT_FLASH_OFFSET + sectorAdr); 
	
	DQ7 = eraseEpromStatus & STATUS_BYTE_80;	
	DQ5 = eraseEpromStatus & STATUS_BYTE_20;	
   
    if (DQ7 == 0) //  Test DQ7	, if one -> erase is finished
	{   
       	if (DQ5 == 0)	
		{ 
			return(e_EraseBlockStatus_AwaitErase);  
		}     // wait for erase to terminate 
       	else
       	{	// retry in case of ayncronus change from status to memory array
			eraseEpromStatus =	*(long*)(BOOT_FLASH_OFFSET + sectorAdr);  	 
			DQ7 = eraseEpromStatus & STATUS_BYTE_80;
			
			g_bEpromIsBusy = FALSE;

      		if (DQ7 == 0)
			{ 
				return(e_EraseBlockStatus_Timeout); 
			}                  
       		else 
			{
				return(e_EraseBlockStatus_Ok); 
			}  // if DQ7 changes to 1 in the same time, erase is ok
       	} 
    }	
    else  
	{	
		g_bEpromIsBusy = FALSE;
		return(e_EraseBlockStatus_Ok);  
	}
}                                

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* long readLongFromEprom(long *valuePtr);	 											*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * This routine packs 4 consecutive 8bit values, to the first one of which
 * valuePtr points, into an integer. This conversion is often needed to read
 * values stored in the Eprom, because physically, Eprom is 8 bits wide.  
 *
 * Input
 * -----
 * Pointer to a 8 bit value in Eprom
 *
 * Output
 * ------
 * 32 bit integer value corresponding to 4 consecutive 8bit bytes in Eprom, 
 * starting at valuePtr.                                  
 *
 * Warning
 * -------
 * This routine makes use of the fact that an integer is 32 bits and
 * occupies one address in RAM. This may restrict portability.
 *                                                          
 * Change History
 * --------------
 * 17.10.96		PGW	Header added, defined inline 
 * 05.11.96		PGW	Including DMA access... ok.
 * 28.07.04		P4LT !  Without DMA
 *
 *				*** This is an Eprom Accessing Routine ***
*/

BOOL readLongFromEprom (long *pSource, long * pDestination)
{
	long *pLocalSource = pSource;

	if (g_bEpromIsBusy)
		return FALSE;

	*pDestination =	  ((*(long*)(BOOT_FLASH_OFFSET + (long)pLocalSource++)) & 0xFF);
	*pDestination |= (((*(long*)(BOOT_FLASH_OFFSET + (long)pLocalSource++)) & 0xFF) << BY_8_BITS);
	*pDestination |= (((*(long*)(BOOT_FLASH_OFFSET + (long)pLocalSource++)) & 0xFF) << BY_16_BITS);
	*pDestination |= (((*(long*)(BOOT_FLASH_OFFSET + (long)pLocalSource))   & 0xFF) << BY_24_BITS);

	return TRUE;
}

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* long readByteFromEprom(long *valuePtr);	 										*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------
 * This routine reads one 8bit byte from Flash Memory at the given location.
 *
 * Input
 * -----
 * Pointer to a 8 bit value in Eprom
 *
 * Output
 * ------
 * 8 bit integer value corresponding to 1 8bit byte in Eprom at valuePtr.      
 *                                                        
 * Change History
 * --------------
 * 17.10.96		PGW		Created                         
 * 05.11.96		PGW		Temporarily test with dma channel 7
 * 08.11.96		PGW		Use channel 7 definitely and interrupt mode, not polling.
 * 28.07.04		P4LT !  Without DMA
 *
 *				*** This is an Eprom Accessing Routine ***
*/
BOOL readByteFromEprom (long *pSource, long * pDestination)
{
	if (g_bEpromIsBusy)
		return FALSE;
	
	*pDestination = (*(long*)(BOOT_FLASH_OFFSET + (long)pSource))  & MASK_8LSB;

	return TRUE;

}


BOOL readBytesFromEprom(long *pSource, long * pDestination, long nrBytes, long increment)
{
	long *pLocalSource       = pSource;	// speed up with local ptr, 
	long *pLocalDestination  = pDestination;
	long byteCount;
	
	if (g_bEpromIsBusy)
		return FALSE;

	for(byteCount = nrBytes; byteCount; --byteCount)
	{
		*(pLocalDestination++) = ((*(long*)(BOOT_FLASH_OFFSET + (long)pLocalSource))  & MASK_8LSB);
		pLocalSource += increment;
	}

	return TRUE;
}

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* long writeByteToEprom()													*/
/* Write one byte to Eprom, return EPROM_ERROR if not OK, else NO_EPROM_ERROR */
/*--------------------------------------------------------------------------*/
/* Description
 * -----------                                                          
 * This subroutine saves one 8 bit value (char) to the Eprom. 
 *
 * Input
 * -----
 * char_to_be_saved			The 8bit value to be written to the Eprom
 * *AddressInEprom			Address in Eprom where the 8bit value is to be stored.
 * wait						1: wait till write terminates, 0: don't wait
 *							  see writeIsOK() for more information.
 *               
 * Output
 * ------
 * return value EPROM_ERROR means an error has occurred, NO_EPROM_ERROR means function was per
 * formed OK. If wait is 0, return will always be NO_EPROM_ERROR.                                                                                 
 *
 * Note
 * ----
 * The mechanism to determine the end of the write operation seems tricky to me, 
 * because if data == 0, then DQ7 can be equal to data before the operation ter-
 * minates!             
 *
 * Note
 * ----
 * Adapt so that only a writing address and a sector base address is passed! Then
 * this routine can be used for programData as well!
 *
 * Change History
 * --------------
 * 1.10.96		PGW		Created.                                                                
 * 8.10.96		PGW		Adapted to allow SGS Eproms 
 * 9.10.96		PGW		Adapted to allow usage by programData()                             
 * 17.10.96		PGW		Adapted for encapsulated Eprom Access 
 * 06.11.96		PGW		Added parameter WAIT 
 * 28.07.04		P4LT !  Without DMA
 *
 * 				*** This is a EPROM Type dependent routine ***
 */  
BOOL writeByteToEprom(long byteValue, long *pDestination, BOOL isBlockingWrite)  // works!
{
	long *pLocalDestination;
	long DQ5, DQ7;                                                                        
	long EpromStatus;

	if (g_bEpromIsBusy)
		return FALSE;

	pLocalDestination = pDestination;

	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA) = COMMAND_BYTE_AA;
	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_555) = COMMAND_BYTE_55;
	*(long*)(BOOT_FLASH_OFFSET + COMMAND_ADDR_AAA) = COMMAND_BYTE_A0;
	*(long*)(BOOT_FLASH_OFFSET + (long)pLocalDestination) = byteValue;

    if (isBlockingWrite)
    {
		do
		{
			EpromStatus = (*(long*)(BOOT_FLASH_OFFSET + (long)pLocalDestination));			// read DQ7 + DQ5 
			DQ7 = EpromStatus & STATUS_BYTE_80;
			DQ5 = EpromStatus & STATUS_BYTE_20;
				
		// loop until DQ7==data||DQ5==1	
		} 
		while(!((DQ7 == (byteValue & STATUS_BYTE_80)) || DQ5));	 

		// DQ7!=data: Failed, retry
		if(DQ7 != (byteValue & STATUS_BYTE_80)) 
		{
			DQ7 = (((*(long*)(BOOT_FLASH_OFFSET + (long)pLocalDestination))) & STATUS_BYTE_80);
		}
	
		// DQ7!=data: Failed, return with error
		if(DQ7 != (byteValue & STATUS_BYTE_80))
		{
			return(FALSE);	// time-out	 
		}

	}
	else
		g_bEpromIsBusy = TRUE;

	return (TRUE);  
}

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* long writeByteToEpromIsOK()															*/
/* Write one byte to Eprom, return EPROM_ERROR if not OK, else NO_EPROM_ERROR 			*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------                                                          
 * This routine runs in conjunction with writeCharToEprom() in its new version
 * with WAIT set to 0.
 *
 * Input
 * -----
 * char_to_be_saved			The 8bit value to be written to the Eprom
 * *AddressInEprom			Address in Eprom where the 8bit value is to be stored.
 *               
 * Output
 * ------
 * WIT_FOR_WRITE    		Writing not yet finished
 * NO_EPROM_ERROR			Writing finished
 *
 * Change History
 * --------------
 * 06.11.96		PGW		Created.                                                                
 * 12.03.98		CB		changed error recognition                                                                        
 * 7.10.98		KORREKTUR: DQ7 nochmals checken nach einem Timeout: da as umschalten vom Statusregister
 *				zum Flash Memory asynchron (gleichzeitig!!!!) zum Lesezugriff erfolgen kann!!
 * 28.07.04		P4LT !  Without DMA
 */  
e_EpromWriteStatus writeByteToEpromStatus(long byteValue, long *pDestination)  // works!
{
	long *pLocalDestination;

	long DQ5, DQ7;                                                                        
	long EpromStatus = 0;

	pLocalDestination = pDestination;

	// Get status register and mask out DQ7 and DQ5
	EpromStatus = (*(long*)(BOOT_FLASH_OFFSET + (long)pLocalDestination));		
	DQ7 = EpromStatus & STATUS_BYTE_80;
	DQ5 = EpromStatus & STATUS_BYTE_20;
    
    if (DQ7 != (byteValue & STATUS_BYTE_80)) //  Test DQ7, if the same as written -> write has finished
	{   
       	if (DQ5 == 0)	
		{ 
			return(e_EpromWriteStatus_AwaitWrite);  
		}     // wait for erase to terminate 
       	else
       	{	// retry in case of ayncronus change from status to memory array
			EpromStatus = (*(long*)(BOOT_FLASH_OFFSET + (long)pLocalDestination));		
			DQ7 = EpromStatus & STATUS_BYTE_80;

			g_bEpromIsBusy = FALSE;

   			if (DQ7 != (byteValue & STATUS_BYTE_80)) 
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


