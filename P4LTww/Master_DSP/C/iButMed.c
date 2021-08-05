/*-------------------------------------------------------------------------*
 * $Workfile: iButMed.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 1-Wire Interface for iButton
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/C/iButMed.c $ 
 * 
 * 3     16.11.05 13:47 Chmibuh
 * Cosmetic
 * 
 * 2     23.02.05 14:29 Chmibuh
 * unused functions removed
 * 
 * 1     23.02.05 13:58 Chmibuh
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES *************************************************************/
#include <def21161.h>
#include "basetype.h"
#include "iButLow.h"
#include "crc.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define NoOF_BYTES_CHECK_ROM_CRC8		7		//1 byte family code + 6 bytes (48 bit) serial number

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

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/
void SEGMENT_EXT_CODE iButM_ComputeSHA(unsigned long* MT, long* hash);
void SEGMENT_EXT_CODE iButM_HashToMAC(long* hash, unsigned long* MAC);

/* IMPLEMENTATION ************************************************************/


//compute the CRC8 or DOW-CRC of inData using 0 as inital value for the CRC. Needed for content of iButton ROM only
static unsigned long SEGMENT_EXT_CODE iButM_ComputeCRC8(unsigned long inData, unsigned long crc_word)
{
    unsigned long bitsLeft;
    unsigned long temp;

    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--)
    {
        temp = ((crc_word ^ inData) & 0x01);
        if (temp == 0)
        {
            crc_word >>= 1;
        }
        else
        {
            crc_word ^= 0x18;
            crc_word >>= 1;
            crc_word |= 0x80;
        }
        inData >>= 1;
    }
    return (crc_word);    
}

// Check the CRC 8 of Read ROM command 
// return TRUE if CRC is OK, else return FALSE
BOOL SEGMENT_EXT_CODE iButM_CheckRomCRC(unsigned long *romBytePtr)
{
    unsigned long i;
    unsigned long crc8 = 0;
    
    for (i = 0; i < NoOF_BYTES_CHECK_ROM_CRC8; i++)
    {
        crc8 = iButM_ComputeCRC8(*romBytePtr, crc8);
        romBytePtr++;
    }
    if (crc8 == (*romBytePtr))
    {
        return (TRUE);
    }
    return (FALSE);
}


BOOL SEGMENT_EXT_CODE iButM_prepareTransfer(void)
{
	BOOL returnVal;
	
	returnVal = iButL_Reset();	
	iButL_WriteByte(ROM_CMD_SKIP_ROM);	
	return(returnVal);	
}


//----------------------------------------------------------------------
// Read the scratchpad with CRC16 verification for DS1961S.
//
// 'address'     - pointer to address that is read from scratchpad
// 'es'          - pointer to offset byte read from scratchpad
// 'data'        - pointer to data buffer read from scratchpad
//
// Return: TRUE - scratch read, address, es, and data returned
//         FALSE - error reading scratch, device not present
//
//
BOOL SEGMENT_EXT_CODE iButM_ReadScratchpad(unsigned long* address, unsigned long* es, unsigned long* data)
{
	unsigned long send_block[32];
	unsigned long send_cnt, i;
	unsigned long lastcrc16;

	send_cnt=0;

	iButM_prepareTransfer();

	// read scratchpad command
	send_block[send_cnt++] = CMD_READ_SCRATCHPAD;

	// now add the read bytes for data bytes and crc16
	for(i=0; i<13;i++)		// 13 = 2B Adr, 1B E/S, 8B data and 2B CRC 
	{
		send_block[send_cnt++] = 0xFF;   	
	}
	
	// now send the block
	iButL_Block(send_block, send_cnt);     
	
	
	// copy data to return buffer
	*address = (send_block[2] << 8) | send_block[1];
	*es = send_block[3];
   
	// calculate CRC16 of result
	lastcrc16 = 0;
   	for(i=0; i < send_cnt;i++)
	{
		lastcrc16 = crc16(lastcrc16,(send_block[i]) & 0x000000ff);		
	} 

	// verify CRC16 is correct
	if(lastcrc16==0xB001)
	{ 
		// copy data to return buffers
	    for(i= 0;i < 8; i++)
	    {
	    	data[i] = send_block[4 + i];
	    }   // success
		return TRUE;
	}
   
	return FALSE;   
}

//----------------------------------------------------------------------
// Write the scratchpad with CRC16 verification for DS1961S.  There must
// be eight bytes worth of data in the buffer.
//
// 'address'   - address to write data to
// 'data'      - data to write
//
// Return: TRUE - write to scratch verified
//         FALSE - error writing scratch, device not present, or HIDE
//                 flag is in incorrect state for address being written.
//
BOOL SEGMENT_EXT_CODE iButM_WriteScratchpad(unsigned long address, unsigned long *data)
{
	unsigned long send_block[32];
	unsigned long send_cnt, i;
	unsigned long lastcrc16;

	send_cnt=0;

	iButM_prepareTransfer();

	lastcrc16 = 0;
	// write scratchpad command
	send_block[send_cnt] = CMD_WRITE_SCRATCHPAD;   
	lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	// address 1
	send_block[send_cnt] = (address & 0xFF);
	lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	// address 2
	send_block[send_cnt] = ((address >> 8) & 0xFF);
	lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	// data
	for (i = 0; i < 8; i++)
	{
	  send_block[send_cnt] = data[i];
	  lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	
	}
	// CRC16 (for read)
	send_block[send_cnt++] = 0xFF;
	send_block[send_cnt++] = 0xFF;
	
	// now send the block
	iButL_Block(send_block, send_cnt);     
	   
	// perform CRC16 of last 2 byte in packet
	for (i = send_cnt - 2; i < send_cnt; i++)
	{
		lastcrc16 = crc16(lastcrc16,(send_block[i]) & 0x000000ff);
	}
	  	
	if(lastcrc16==0xB001)
	{
	   // success
	   return TRUE; 	
	}
	 
	return FALSE;
}



//----------------------------------------------------------------------
// Read Authenticated Page for DS1961S.
//
// 'pagenum'     - page number to do a read authenticate
// 'data'        - buffer to read into from page
// 'sign'        - buffer for storing resulting sha computation
//
// Return: 		TRUE if CRC ok
//         		FALSE if CRC is invalid
//
BOOL SEGMENT_EXT_CODE iButM_ReadAuthPage(unsigned long pagenum, unsigned long* data, unsigned long* sign)
{
	unsigned long send_block[55];
	unsigned long send_cnt, i;
	unsigned long lastcrc16;
   	unsigned long address = pagenum << 5;
   	
	send_cnt=0;
   	
	iButM_prepareTransfer();
	
	//seed the crc
	lastcrc16 = 0;
	
	// create the send block
	// Read Authenticated Page command
	send_block[send_cnt] = CMD_READ_AUTH_PAGE;
	lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	
	// TA1
	send_block[send_cnt] = (address & 0xFF);
	lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	
	// TA2
	send_block[send_cnt] = ((address >> 8) & 0xFF);
	lastcrc16 = crc16(lastcrc16,(send_block[send_cnt++]) & 0x000000ff);
	
	// now add the read bytes for data bytes, 0xFF byte, and crc16 (1B E/S, 32B data, 2B CRC)
	for(i=0; i<35;i++)		
	{
		send_block[send_cnt++] = 0xFF;   	
	}
   
	// now send the block
	iButL_Block(send_block, send_cnt);     
	
	// check the CRC
	for (i = 3; i < send_cnt; i++)
	{
	  lastcrc16 = crc16(lastcrc16,(send_block[i]) & 0x000000ff);
	}
	
	// verify CRC16 is correct  
	if(!(lastcrc16==0xB001))   // CRC Error!!!
	{  	
		return (FALSE);
	}
	
	// transfer results
    for(i= 0;i < 32; i++)
    {
    	data[i] = send_block[(send_cnt-35) + i];		
    }   // success
   
   
	// now wait for the MAC computation.
	// should provide strong pull-up here.
	// msDelay(2);
   	i = 24000;								
   	do
	{
		asm("nop;");
	}while(i--);


	// read the MAC
	send_cnt = 0;
	for(i=0; i<23; i++)		
	{
		send_block[send_cnt++] = 0xFF;   	
	}
   	iButL_Block(send_block, send_cnt);     
   	
	// check CRC of the MAC
	//seed the crc
	lastcrc16 = 0;
	
	for(i = 0; i < 22; i++)
	{
	  lastcrc16 = crc16(lastcrc16,(send_block[i]) & 0x000000ff);
	}
	
	// verify CRC of the MAC is correct
	if(!(lastcrc16==0xB001))   // CRC Error!!!
	{  	
		return (FALSE);
	}
	 
	// check verification
	if(!(((send_block[22] & 0xF0) == 0x50) || ((send_block[22] & 0xF0) == 0xA0)))
	{
		return (FALSE);   	
	}
   
	// transfer MAC into buffer
    for(i= 0;i < 20; i++)
    {
    	sign[i] = send_block[i];		
    }   // success
   
	return (TRUE); // All CRCs and checks were successful
}


//constants used in SHA computation
static const long KTN[4] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };

// calculation used for the SHA MAC
static long NLF (long B, long C, long D, int n)
{
   if(n<20)
      return ((B&C)|((~B)&D));
   else if(n<40)
      return (B^C^D);
   else if(n<60)
      return ((B&C)|(B&D)|(C&D));
   else
      return (B^C^D);
}

//----------------------------------------------------------------------
// computes a SHA given the 64 byte MT digest buffer.  The resulting 5
// long values are stored in the given long array, hash.
//
// Note: This algorithm is the SHA-1 algorithm as specified in the
// datasheet for the DS1961S, where the last step of the official
// FIPS-180 SHA routine is omitted (which only involves the addition of
// constant values).
//
// 'MT'        - buffer containing the message digest
// 'hash'      - result buffer
//
void SEGMENT_EXT_CODE iButM_ComputeSHA(unsigned long* MT, long* hash)
{
   unsigned long MTword[80];
   unsigned long i;
   long ShftTmp;
   long Temp;

   for(i=0;i<16;i++)
   {
      MTword[i] = ((MT[i*4]&0x00FF) << 24) | ((MT[i*4+1]&0x00FF) << 16) |
                  ((MT[i*4+2]&0x00FF) << 8) | (MT[i*4+3]&0x00FF);
   }

   for(;i<80;i++)
   {
      ShftTmp = MTword[i-3] ^ MTword[i-8] ^ MTword[i-14] ^ MTword[i-16];
      MTword[i] = ((ShftTmp << 1) & 0xFFFFFFFE) |
                  ((ShftTmp >> 31) & 0x00000001);
   }

   hash[0] = 0x67452301;
   hash[1] = 0xEFCDAB89;
   hash[2] = 0x98BADCFE;
   hash[3] = 0x10325476;
   hash[4] = 0xC3D2E1F0;

   for(i=0;i<80;i++)
   {
      ShftTmp = ((hash[0] << 5) & 0xFFFFFFE0) | ((hash[0] >> 27) & 0x0000001F);
      Temp = NLF(hash[1],hash[2],hash[3],i) + hash[4]
               + KTN[i/20] + MTword[i] + ShftTmp;
      hash[4] = hash[3];
      hash[3] = hash[2];
      hash[2] = ((hash[1] << 30) & 0xC0000000) | ((hash[1] >> 2) & 0x3FFFFFFF);
      hash[1] = hash[0];
      hash[0] = Temp;
   }
}

//----------------------------------------------------------------------
// Converts the 5 long numbers that represent the result of a SHA
// computation into the 20 bytes (with proper byte ordering) that the
// SHA iButton's expect.
//
// 'hash'      - result of SHA calculation
// 'MAC'       - 20-byte, LSB-first message authentication code for SHA
//                iButtons.
//
void SEGMENT_EXT_CODE iButM_HashToMAC(long* hash, unsigned long* MAC)
{
   long temp;
   long i, j, offset;

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
         MAC[i+offset] = temp & 0xFF;
         temp >>= 8;
      }
   }
}



//----------------------------------------------------------------------
// Execute READ ROM CMD
// Reads family code, 6 bytes serial number and CRC
// Checks CRC of received data
//
// 'destinBuf'	- destination buffer for serial number
//
// Return:		TRUE  - if CRC is OK 
//				FALSE - if CRC is invalid
//
BOOL SEGMENT_EXT_CODE iButM_ReadSerialNum(unsigned long* destinBuf)
{
	unsigned long i, tempBuf[8];
	
	iButL_WriteByte(ROM_CMD_READ_ROM);						// Read ROM command
	
	tempBuf[0] = iButL_ReadByte();		// family code must be 0x33
	tempBuf[1] = iButL_ReadByte();		// serie No
	tempBuf[2] = iButL_ReadByte();		// serie No
	tempBuf[3] = iButL_ReadByte();		// serie No
	tempBuf[4] = iButL_ReadByte();		// serie No
	tempBuf[5] = iButL_ReadByte();		// serie No
	tempBuf[6] = iButL_ReadByte();		// serie No
	tempBuf[7] = iButL_ReadByte();		// CRC
	
	if(iButM_CheckRomCRC(tempBuf))		// CRC is OK
	{
		for(i=0; i < 8;i++)
		{
			*(destinBuf++) = tempBuf[i];		
		}
		return(TRUE);
	}
	return(FALSE);	
}

//----------------------------------------------------------------------
// Execute Reset impulse on iButton (1-wire interface)
//
// Return: 	TRUE  - Presence Pulse was detected
//			FALSE - No P. Pulse was detected
BOOL SEGMENT_EXT_CODE iButM_Detect_iButton(void)
{
	return(iButL_Reset());		
}

