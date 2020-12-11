/*-------------------------------------------------------------------------*
 * $Workfile	: iButMed.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Med level iButton access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/iButMed.c $
 * 
 * 2     07-08-08 16:34 Chalnot
 * Delay with timer module.
 * 
 * 1     07-04-17 10:01 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include "basetype.h"
#include "Basetype_O4CV.h"
#include "iButMed.h"
#include "iButLow.h"
#include "crc.h"
#include "timer.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

//constants used in SHA computation
static const INT32 KTN[4] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };

#define NoOF_BYTES_CHECK_ROM_CRC8	7	//1 byte family code + 6 bytes (48 bit) serial number

// ********************************************************************** //
// DS1961S SHA iButton commands 
// ********************************************************************** //
#define CMD_READ_MEMORY          	0xF0	
#define CMD_WRITE_SCRATCHPAD     	0x0F	
#define CMD_READ_SCRATCHPAD      	0xAA	
#define CMD_COPY_SCRATCHPAD      	0x55	
#define CMD_READ_AUTH_PAGE       	0xA5	
#define CMD_LOAD_FIRST_SECRET     	0x5A	
#define CMD_COMPUTE_NEXT_SECRET   	0x33	
#define CMD_REFRESH_SCRATCHPAD    	0xA3	

#define ROM_CMD_READ_ROM			0x33	
#define ROM_CMD_MATCH_ROM			0x55	
#define ROM_CMD_SEARCH_ROM			0xF0	
#define ROM_CMD_SKIP_ROM           	0xCC	
#define ROM_CMD_RESUME           	0xA5	
#define ROM_CMD_OvD_SKIP_ROM		0x3C
#define ROM_CMD_OvD_MATCH_ROM		0x69

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

static UINT32 computeCRC8(UINT32 inData, UINT32 crc);
static bool checkRomCRC(UINT32 *romBytePtr);
static INT32 nlf (INT32 b, INT32 c, INT32 d, INT32 n);
static void hashToMAC(INT32* hash, UINT32* MAC);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:      computeCRC8()  
  Description: 	 compute the CRC8 or DOW-CRC of inData. Needed for content of 
  				 the iButton ROM only         
  Inputs:      	 inData: One byte data 
  				 crc: Initial value of the crc 			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 The calculated crc  	
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
static UINT32 computeCRC8(UINT32 inData, UINT32 crc)
{
    UINT32 bitsLeft;
    UINT32 temp;

    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--)
    {
        temp = ((crc ^ inData) & 0x01);
        if (temp == 0)
        {
            crc >>= 1;
        }
        else
        {
            crc ^= 0x18;
            crc >>= 1;
            crc |= 0x80;
        }
        inData >>= 1;
    }
    return(crc);    
}


/*****************************************************************************
  Function:      checkRomCRC()  
  Description: 	 Checks the crc8 of the Read ROM command         
  Inputs:      	 romBytePtr: Pointer to the Byte with the ROM command			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if crc is OK, else return FALSE 	
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
static bool checkRomCRC(UINT32 *romBytePtr)
{
    UINT32 i;
    UINT32 crc8 = 0;
    
    for (i = 0; i < NoOF_BYTES_CHECK_ROM_CRC8; i++)
    {
        crc8 = computeCRC8(*romBytePtr, crc8);
        romBytePtr++;
    }
    if (crc8 == (*romBytePtr))
    {
        return(TRUE);
    }
    return(FALSE);
}


/*****************************************************************************
  Function:      nlf()  
  Description: 	 Makes a calculation used for the SHA MAC    
  Inputs:      	 b:	Second value of the hash
  				 c: Third value of the hash
  				 d: Fourth value of the hash
  				 n:	Shows how many times that this function has been called 
  				 during this sha calculation		        				 	                                                                       
  Outputs:    	 none                                                                                                                             
  Return:      	 The result of the calculation
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
static INT32 nlf(INT32 b, INT32 c, INT32 d, INT32 n)
{
   if(n<20)
      return ((b&c)|((~b)&d));
   else if(n<40)
      return (b^c^d);
   else if(n<60)
      return ((b&c)|(b&d)|(c&d));
   else
      return (b^c^d);
}


/*****************************************************************************
  Function:      hashToMAC()  
  Description: 	 Converts the 5 long numbers that represent the result of a SHA
 				 computation into the 20 bytes (with proper byte ordering) that 
 				 the SHA iButton is expecting.    
  Inputs:      	 hash: Buffer for the result of the sha calculation  			        				 	                                                                       
  Outputs:    	 mac: 20-byte, LSB-first message authentication code for SHA
                 iButtons.                                                                                                                              
  Return:      	 None
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
static void hashToMAC(INT32* hash, UINT32* mac)
{
   UINT32 temp;
   UINT32 i;
   INT32 j;
   INT32 offset;

   //iButtons use LSB first, so we have to turn
   //the result around a little bit.  Instead of
   //result A-B-C-D-E, our result is E-D-C-B-A,
   //where each letter represents four bytes of
   //the result.
   for(j=4; j>=0; j--)
   {
      temp = hash[j];
      offset = (4-j)*4;
      for(i=0; i<4; i++)
      {
         mac[i+offset] = temp & 0xFF;
         temp >>= 8;
      }
   }
}


/*****************************************************************************
  Function:      iButM_prepareTransfer()  
  Description: 	 Resets the ibutton and and writes the skip command          
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if reset was successful, else return FALSE 		
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButM_prepareTransfer(void)
{
	bool returnVal;
	
	returnVal = iButL_reset();	
	iButL_writeByte(ROM_CMD_SKIP_ROM);	
	return(returnVal);	
}


/*****************************************************************************
  Function:      iButM_readScratchpad()  
  Description: 	 Reads the scratchpad with CRC16 verification for DS1961S         
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 pAddress: Pointer to address that is read from scratchpad  
  				 pOffset: Pointer to offset byte read from scratchpad
  				 data: Buffer for the read from scratchpad                                                                                                                                    
  Return:      	 TRUE if scratch read, address, es, and data returned
  				 FALSE if error reading scratch, device not present	
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButM_readScratchpad(UINT32* pAddress, UINT32* pOffset, UINT32* data)
{
	UINT32 sendBlock[32];
	UINT32 sendCnt = 0;
	UINT32 i;
	UINT32 lastcrc16;

	iButM_prepareTransfer();

	// read scratchpad command
	sendBlock[sendCnt++] = CMD_READ_SCRATCHPAD;

	// now add the read bytes for data bytes and crc16
	for(i=0; i<13;i++)		// 13 = 2B Adr, 1B E/S, 8B data and 2B CRC 
	{
		sendBlock[sendCnt++] = 0xFF;   	
	}
	
	// now send the block
	iButL_touchBlock(sendBlock, sendCnt);     
	
	
	// copy data to return buffer
	*pAddress = (sendBlock[2] << 8) | sendBlock[1];
	*pOffset = sendBlock[3];
   
	// calculate CRC16 of result
	lastcrc16 = 0;
   	for(i=0; i < sendCnt;i++)
	{
		lastcrc16 = crc16(lastcrc16,(sendBlock[i]) & 0x000000ff);		
	} 

	// verify CRC16 is correct
	if(lastcrc16==0xB001)
	{ 
		// copy data to return buffers
	    for(i= 0;i < 8; i++)
	    {
	    	data[i] = sendBlock[4 + i];
	    }   // success
		return(TRUE);
	}
   
	return(FALSE);   
}


/*****************************************************************************
  Function:      iButM_writeScratchpad()  
  Description: 	 Write the scratchpad with CRC16 verification for DS1961S  
  				 There must be eight bytes worth of data in the buffer        
  Inputs:      	 data: Data buffer			        				 	                                                                       
   			  	 address: Address where data should be written
  Outputs:		 None			                                                                                                                                    
  Return:      	 TRUE if write to scratch verified
  				 FALSE if error writing scratch, device not present, or HIDE
                 flag is in incorrect state for address being written.
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButM_writeScratchpad(UINT32 address, UINT32* data)
{
	UINT32 sendBlock[32];
	UINT32 sendCnt = 0;
	UINT32 i;
	UINT32 lastcrc16;

	iButM_prepareTransfer();

	lastcrc16 = 0;
	// write scratchpad command
	sendBlock[sendCnt] = CMD_WRITE_SCRATCHPAD;   
	lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	// address 1
	sendBlock[sendCnt] = (address & 0xFF);
	lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	// address 2
	sendBlock[sendCnt] = ((address >> 8) & 0xFF);
	lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	// data
	for (i = 0; i < 8; i++)
	{
	  sendBlock[sendCnt] = data[i];
	  lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	
	}
	// CRC16 (for read)
	sendBlock[sendCnt++] = 0xFF;
	sendBlock[sendCnt++] = 0xFF;
	
	// now send the block
	iButL_touchBlock(sendBlock, sendCnt);     
	   
	// perform CRC16 of last 2 byte in packet
	for (i = sendCnt - 2; i < sendCnt; i++)
	{
		lastcrc16 = crc16(lastcrc16,(sendBlock[i]) & 0x000000ff);
	}
	  	
	if(lastcrc16==0xB001)
	{
	   // success
	   return(TRUE);	
	}
	 
	return(FALSE);
}


/*****************************************************************************
  Function:      iButM_readAuthPage()  
  Description: 	 Read Authenticated Page for DS1961S        
  Inputs:      	 pagenum: Page number to do a read authenticate 			        				 	                                                                       
  Outputs:    	 data: Buffer to read into from page   
  				 sign: Buffer for storing the result of sha computation                                                                                                                               
  Return:      	 TRUE if crc is ok, else return FALSE 
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButM_readAuthPage(UINT32 pagenum, UINT32* data, UINT32* sign)
{
	UINT32 sendBlock[55];
	UINT32 sendCnt = 0;
	UINT32 i;
	UINT32 lastcrc16;
   	UINT32 address = pagenum << 5;
   		
	iButM_prepareTransfer();
	
	//seed the crc
	lastcrc16 = 0;
	
	// create the send block
	// Read Authenticated Page command
	sendBlock[sendCnt] = CMD_READ_AUTH_PAGE;
	lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	
	// TA1
	sendBlock[sendCnt] = (address & 0xFF);
	lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	
	// TA2
	sendBlock[sendCnt] = ((address >> 8) & 0xFF);
	lastcrc16 = crc16(lastcrc16,(sendBlock[sendCnt++]) & 0x000000ff);
	
	// now add the read bytes for data bytes, 0xFF byte, and crc16 (1B E/S, 32B data, 2B CRC)
	for(i=0; i<35;i++)		
	{
		sendBlock[sendCnt++] = 0xFF;   	
	}
   
	// now send the block
	iButL_touchBlock(sendBlock, sendCnt);     
	
	// check the CRC
	for (i = 3; i < sendCnt; i++)
	{
	  lastcrc16 = crc16(lastcrc16,(sendBlock[i]) & 0x000000ff);
	}
	
	// verify CRC16 is correct  
	if(!(lastcrc16==0xB001))   // CRC Error!!!
	{  	
		return (FALSE);
	}
	
	// transfer results
    for(i= 0;i < 32; i++)
    {
    	data[i] = sendBlock[(sendCnt-35) + i];		
    }   // success
   
   
	// now wait for the MAC computation.
	// should provide strong pull-up here.
	
	// 2ms delay
	timer_init(2000, e_Timer1);
	timer_wait(e_Timer1);

	// read the MAC
	sendCnt = 0;
	for(i=0; i<23; i++)		
	{
		sendBlock[sendCnt++] = 0xFF;   	
	}
   	iButL_touchBlock(sendBlock, sendCnt);     
   	
	// check CRC of the MAC
	//seed the crc
	lastcrc16 = 0;
	
	for(i = 0; i < 22; i++)
	{
	  lastcrc16 = crc16(lastcrc16,(sendBlock[i]) & 0x000000ff);
	}
	
	// verify CRC of the MAC is correct
	if(!(lastcrc16==0xB001))   // CRC Error!!!
	{  	
		return (FALSE);
	}
	 
	// check verification
	if(!(((sendBlock[22] & 0xF0) == 0x50) || ((sendBlock[22] & 0xF0) == 0xA0)))
	{
		return (FALSE);   	
	}
   
	// transfer MAC into buffer
    for(i= 0;i < 20; i++)
    {
    	sign[i] = sendBlock[i];		
    }   // success
   
	return(TRUE); // All CRCs and checks were successful
}


/*****************************************************************************
  Function:      iButM_computeSHA()  
  Description: 	 Computes a SHA with the given 64 byte MT digest buffer. The 
  				 resulting 5 long values are stored in the long array (hash).
 				 Note: This algorithm is the SHA-1 algorithm as specified in the
 				 datasheet for the DS1961S, where the last step of the official
 				 FIPS-180 SHA routine is omitted (which only involves the addition of
 				 constant values).     
  Inputs:      	 mt: Buffer containing the message digest 			        				 	                                                                       
  Outputs:    	 hash: Buffer for the result                                                                                                                               
  Return:      	 None
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
void iButM_computeSHA(UINT32* mt, INT32* hash)
{
   
   UINT32 mtWord[80];
   UINT32 i;
   INT32 shiftTemp;
   INT32 temp;

   for(i=0;i<16;i++)
   {
      mtWord[i] = ((mt[i*4]&0x00FF) << 24) | ((mt[i*4+1]&0x00FF) << 16) |
                  ((mt[i*4+2]&0x00FF) << 8) | (mt[i*4+3]&0x00FF);
   }

   for(;i<80;i++)
   {
      shiftTemp = mtWord[i-3] ^ mtWord[i-8] ^ mtWord[i-14] ^ mtWord[i-16];
      mtWord[i] = ((shiftTemp << 1) & 0xFFFFFFFE) |
                  ((shiftTemp >> 31) & 0x00000001);
   }

   hash[0] = 0x67452301;
   hash[1] = 0xEFCDAB89;
   hash[2] = 0x98BADCFE;
   hash[3] = 0x10325476;
   hash[4] = 0xC3D2E1F0;

   for(i=0;i<80;i++)
   {
      shiftTemp = ((hash[0] << 5) & 0xFFFFFFE0) | ((hash[0] >> 27) & 0x0000001F);
      temp = nlf(hash[1],hash[2],hash[3],i) + hash[4]
               + KTN[i/20] + mtWord[i] + shiftTemp;
      hash[4] = hash[3];
      hash[3] = hash[2];
      hash[2] = ((hash[1] << 30) & 0xC0000000) | ((hash[1] >> 2) & 0x3FFFFFFF);
      hash[1] = hash[0];
      hash[0] = temp;
   }
}


/*****************************************************************************
  Function:      iButM_readSerialNum()  
  Description: 	 Reads the family code, 6 bytes serial number and CRC   
  Inputs:      	 None  			        				 	                                                                       
  Outputs:    	 destinBuf: Destination buffer for serial number                                                                                                                              
  Return:      	 TRUE if crc is OK, else return FALSE 	
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButM_readSerialNum(UINT32* destinBuf)
{
	UINT32 i;
	UINT32 tempBuf[8];
	
	iButL_writeByte(ROM_CMD_READ_ROM);						// Read ROM command
	
	tempBuf[0] = iButL_readByte();		// family code must be 0x33
	tempBuf[1] = iButL_readByte();		// serie No
	tempBuf[2] = iButL_readByte();		// serie No
	tempBuf[3] = iButL_readByte();		// serie No
	tempBuf[4] = iButL_readByte();		// serie No
	tempBuf[5] = iButL_readByte();		// serie No
	tempBuf[6] = iButL_readByte();		// serie No
	tempBuf[7] = iButL_readByte();		// CRC
	
	if(checkRomCRC(tempBuf))		// CRC is OK
	{
		for(i=0; i < 8;i++)
		{
			*(destinBuf++) = tempBuf[i];		
		}
		return(TRUE);
	}
	return(FALSE);	
}


/*****************************************************************************
  Function:      iButM_detectIButton()  
  Description: 	 Executes a reset impulse on iButton (1-wire interface)  
  Inputs:      	 None  			        				 	                                                                       
  Outputs:    	 None                                                                                                                            
  Return:      	 TRUE if presence pulse was detected, else return FALSE 	
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButM_detectIButton(void)
{
	return(iButL_reset());		
}





