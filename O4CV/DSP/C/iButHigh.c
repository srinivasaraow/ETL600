/*-------------------------------------------------------------------------*
 * $Workfile	: iButHigh.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : High level iButton access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/iButHigh.c $
 * 
 * 4     07-08-21 9:19 Chalnot
 * Set random value in init function.
 * 
 * 3     07-08-09 11:39 Chalnot
 * O4CV_Const.h included.
 * 
 * 2     07-04-27 11:11 Chalnot
 * Defined alarms intern.
 * 
 * 1     07-04-17 10:01 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include "basetype.h"
#include "Basetype_O4CV.h"
#include "O4CV_Const.h"
#include "iButHigh.h"
#include "iButMed.h"
#include "iButPW.h"
#include "DuSlic.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// How many tries to access the iButton before continue
#define HOW_MANY_ACCESS_TRIES		0x03

#define DS1961S_FAMILY_CODE      	0x33

#define DATA_MEMORY_PAGE_0			0x00
#define DATA_MEMORY_PAGE_1			0x01
#define DATA_MEMORY_PAGE_2			0x02
#define DATA_MEMORY_PAGE_3			0x03

#define iBUT_AL_NO_DEVICE_DETECTED			0x01
#define iBUT_AL_INVALID_CRC_SERIAL_NO		0x02
#define iBUT_AL_WRONG_DEVICE				0x04
#define iBUT_AL_iBUTTON_IS_INVALID			0x08

/* LOCAL TYPE DECLARATIONS ***************************************************/

// ********************************************************************** //
// SHA Device Structures
// ********************************************************************** //

// struct SHACopr
//   Holds all information pertinent to SHA coprocessors, as
//   well as all of the system parameters for this account
//   transaction system.
typedef struct
{
	// Binding information for producing unique secrets
	UINT32 bindData[32];
	
	// a long to hold various alarm conditions, see iButAlarm.h
	UINT32 alarm_iButton;
	// indicates that the master authentication secret was
	// padded to match the secret of a DS1961S
	bool ds1961Scompatible;

} SHACopr;

// struct SHAUser
//   Holds all information pertinent to SHA user tokens.
//   Maintains state between routines for verifying the
//   user's authentication response, the account signature,
//   and updating the account data.
typedef struct
{
	// address of the device, serial number
	UINT32 devAN[8];
	// page the user's account file is stored on
	UINT32 accountPageNumber;
	// MAC from Read Authenticated Page command
	UINT32 responseMAC[20];
	// 32-byte account file
	UINT32 accountFile[32];

} SHAUser;

/* LOCAL VARIABLES ***********************************************************/

	static SHACopr g_copr;
	static SHAUser g_user;
	
	static UINT32 g_randomValue;

	static UINT32 g_authSecret[32] = {	HIDE_SECRET_0, 
										HIDE_SECRET_1,
										HIDE_SECRET_2,
										HIDE_SECRET_3,
										HIDE_SECRET_4,
										HIDE_SECRET_5,
										HIDE_SECRET_6,
										HIDE_SECRET_7,
										HIDE_SECRET_8,
										HIDE_SECRET_9,
										HIDE_SECRET_10,
										HIDE_SECRET_11,
										HIDE_SECRET_12,
										HIDE_SECRET_13,
										HIDE_SECRET_14,
										HIDE_SECRET_15,
										HIDE_SECRET_16,
										HIDE_SECRET_17,
										HIDE_SECRET_18,
										HIDE_SECRET_19,
										HIDE_SECRET_20,
										HIDE_SECRET_21,
										HIDE_SECRET_22,
										HIDE_SECRET_23,
										HIDE_SECRET_24,
										HIDE_SECRET_25,
										HIDE_SECRET_26,
										HIDE_SECRET_27,
										HIDE_SECRET_28,
										HIDE_SECRET_29,
										HIDE_SECRET_30,
										HIDE_SECRET_31	};

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

static bool answerChallenge(UINT8* chlg);
static void createChallenge(UINT8* chlg);
static bool verifyUser(bool doBind);
static bool verifyAuthResponse(UINT8* chlg, bool doBind);
static bool createDataSignature(UINT32* secret, UINT32* data, UINT32* scratchpad, 
								UINT32* signature, bool readSignature);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:      answerChallenge()  
  Description: 	 Answers a random challenge by performing an authenticated read 
  				 of the user's account information.       
  Inputs:      	 chlg: 3-byte buffer of challenge data.			        				 	                                                                       
  Outputs:		 None			                                                                                                                                    
  Return:      	 TRUE if no error has occured, else return FALSE
  Globals:       g_user
  Important:                                                    	
*******************************************************************************/ 
static bool answerChallenge(UINT8* chlg)
{
	UINT32 i;
	UINT32 addr = g_user.accountPageNumber << 5;
		
	g_user.accountFile[20] = *chlg;
	chlg++;
	g_user.accountFile[21] = *chlg;
	chlg++;
	g_user.accountFile[22] = *chlg;
			
	if(!(iButM_writeScratchpad(addr, &g_user.accountFile[16])))
	{
		return(FALSE);	
	}
	
	iButM_prepareTransfer();
		
	if(!(iButM_readAuthPage(g_user.accountPageNumber, g_user.accountFile, g_user.responseMAC)))
	{
		return(FALSE);			
	}
                              
    return(TRUE);
}


/*****************************************************************************
  Function:      createChallenge()  
  Description: 	 Creates a 3-Byte random challenge with the duSlic       
  Inputs:      	 None			        				 	                                                                       
  Outputs:		 chlg: 3-byte buffer for challenge data.			                                                                                                                                    
  Return:      	 TRUE if no error has occured, else return FALSE
  Globals:       g_randomValue
  Important:     DuSlic must be activated                                                	
*******************************************************************************/ 
static void createChallenge(UINT8* chlg)
{
	UINT32 temp = 0;

	temp = g_randomValue;
	chlg[0] = (temp & 0xFF);
	temp >>= 8; 
	chlg[1] = (temp & 0xFF);
	temp >>= 8; 
	chlg[2] = (temp & 0xFF);
		
}


/*****************************************************************************
  Function:      verifyUser()  
  Description: 	 Verifies a user token as a valid member of the system.  
  				 First, a random challenge is generated.  Then, the user must 
  				 answer that challenge. The users response signature is then 
  				 verified against the proper response.      
  Inputs:      	 doBind: If true, the user's unique secret is recreated on the
                 coprocessor.  If this function is called multiple times,
                 it is acceptable to skip the bind for all calls after
                 the first on the same user token.			        				 	                                                                       
  Outputs:		 None			                                                                                                                                    
  Return:      	 TRUE, user was verified.
         		 FALSE, an error occurred or user verification failed.
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
static bool verifyUser(bool doBind)
{
   UINT8 chlg[3];
 
   createChallenge(chlg);
      
   if(!(answerChallenge(chlg)))
   {
		return(FALSE);   	
   }
   
   if(!(verifyAuthResponse(chlg, doBind)))
   {
		return(FALSE);
   }

   return(TRUE);
}


/*****************************************************************************
  Function:      verifyAuthResponse()  
  Description: 	 Verifies the authentication response of a user token without a
				 hardware coprocessor.     
  Inputs:      	 chlg: 3-byte buffer for challenge data.
  				 doBind: If true, the user's unique secret is recreated on the
                 coprocessor.  If this function is called multiple times,
                 it is acceptable to skip the bind for all calls after
                 the first on the same user token.			        				 	                                                                       
  Outputs:		 None			                                                                                                                                    
  Return:      	 TRUE, the user's authentication response matched exactly the
            	 signature generated by the coprocessor.
         		 FALSE, an error occurred or the signature did not match.
  Globals:       g_authSecret, g_user
  Important:                                                    	
*******************************************************************************/ 
static bool verifyAuthResponse(UINT8* chlg, bool doBind)
{
	UINT32 scratchpad[32];
	UINT32 fullBindCode[15];
	UINT32 i;
	UINT32 temp_buf[32];
	// M-X control byte
	UINT32 MXP = 0x00;
	UINT32 dummyBuffer[64];
	INT32 dummyRet[5];

	dummyBuffer[59] = (((g_authSecret[6] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[29] = (((g_authSecret[1] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[44] = (((g_authSecret[0] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[13] = (((g_authSecret[7] & 0x0FF00000) >> 20) & 0xFF); 

	//   memset(scratchpad, 0x00, 32);
	for(i=0; i < 32; i++)
	{
		scratchpad[i] = 0x00;
	}
	dummyBuffer[20] = (((g_authSecret[18] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[30] = (((g_authSecret[11] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[33] = (((g_authSecret[27] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[15] = (((g_authSecret[9] & 0x003FC000) >> 14) & 0xFF);
	dummyBuffer[49] = (((g_authSecret[22] & 0x00FF0000) >> 16) & 0xFF); 
	
	//   memset(fullBindCode, 0x0FF, 15);
	for(i=0; i < 15; i++)
	{
		fullBindCode[i] = 0xFF;
	}
	dummyBuffer[6] = (((g_authSecret[31] & 0x00FF0000) >> 16) & 0xFF); 
	dummyBuffer[10] = (((g_authSecret[6] & 0x00000FF0) >> 4) & 0xFF);
	dummyBuffer[36] = (((g_authSecret[21] & 0x001FE000) >> 13) & 0xFF);
	  
	// Leave bindCode FF for DS1961S
	// Use AuthenticateHost command
	MXP = 0x40;
	// the user doesn't have a write cycle counter
	//memset(&scratchpad[8], 0x0FF, 4);
	scratchpad[8] = 0x0FF;
	dummyBuffer[3] = (((g_authSecret[8] & 0x00000FF0) >> 4) & 0xFF); 
	scratchpad[9] = 0x0FF;
	dummyBuffer[39] = (g_authSecret[9] & 0xFF);	
	dummyBuffer[54] = (((g_authSecret[17] & 0x0000FF00) >> 8) & 0xFF);  
	scratchpad[10] = 0x0FF;
	scratchpad[11] = 0x0FF;
	dummyBuffer[5] = (g_authSecret[17] & 0xFF);
	dummyBuffer[32] = (((g_authSecret[2] & 0x003FC000) >> 14) & 0xFF);
      
	// the pagenumber
	fullBindCode[4] = g_user.accountPageNumber;
	dummyBuffer[60] = (g_authSecret[7] & 0xFF);
	dummyBuffer[56] = (((g_authSecret[27] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[25] = (((g_authSecret[31] & 0x0FF00000) >> 20) & 0xFF); 
	// and 7 bytes of the address of current device
	//memcpy(&fullBindCode[5], user->devAN, 7);
	for(i=0; i < 7; i++)
	{
		fullBindCode[5 + i] = g_user.devAN[i];  	
	}
	dummyBuffer[63] = (((g_authSecret[0] & 0x00FF0000) >> 16) & 0xFF); 
	dummyBuffer[52] = (((g_authSecret[24] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[1] = (((g_authSecret[8] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[42] = (((g_authSecret[2] & 0x0007F800) >> 11) & 0xFF);
	
	// get the user address and page num from fullBindCode
	//   memcpy(&scratchpad[12], &fullBindCode[4], 8);
	for(i=0; i < 8; i++)
	{
		scratchpad[12 + i] = fullBindCode[4 + i];
	}
	 
	// set the same challenge bytes
	//memcpy(&scratchpad[20], chlg, 3);
	
	scratchpad[20] = chlg[0];
	scratchpad[21] = chlg[1];
	scratchpad[22] = chlg[2];
	dummyBuffer[21] = (((g_authSecret[29] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[47] = (((g_authSecret[16] & 0x003FC000) >> 14) & 0xFF);
	dummyBuffer[11] = (((g_authSecret[18] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[19] = (((g_authSecret[26] & 0x0FF00000) >> 20) & 0xFF); 
		
	// Fix the M-X control bits
	scratchpad[12] = ((scratchpad[12]&0x3F)|(MXP&0xC0));

	dummyBuffer[0] = (((g_authSecret[25] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[58] = (((g_authSecret[5] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[14] = (((g_authSecret[3] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[35] = (((g_authSecret[9] & 0x00FF0000) >> 16) & 0xFF); 
	//memset(temp_buf, 0x00, 32);
	for(i=0; i < 32; i++)
	{
		temp_buf[i] = 0x00;
	}
	dummyBuffer[4] = (((g_authSecret[5] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[16] = (g_authSecret[14] & 0xFF);
	dummyBuffer[53] = (((g_authSecret[13] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[50] = (((g_authSecret[4] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[18] = (((g_authSecret[15] & 0x0007F800) >> 11) & 0xFF);
	
	//memcpy(&temp_buf[8],fullBindCode,15);
	for(i=0; i < 15; i++)
	{
		temp_buf[8 + i] = fullBindCode[i];
	}	
	
	dummyBuffer[8] = (((g_authSecret[28] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[12] = (((g_authSecret[2] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[17] = (((g_authSecret[25] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[23] = (((g_authSecret[17] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[2] = (((g_authSecret[16] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[46] = (((g_authSecret[1] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[26] = (((g_authSecret[26] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[22] = (((g_authSecret[6] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[27] = (((g_authSecret[12] & 0x00FF0000) >> 16) & 0xFF); 
	dummyBuffer[7] = (((g_authSecret[21] & 0x000FF000) >> 12) & 0xFF);  
	dummyBuffer[41] = (((g_authSecret[8] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[28] = (((g_authSecret[29] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[9] = (((g_authSecret[0] & 0x0000FF00) >> 8) & 0xFF);
	dummyBuffer[34] = (((g_authSecret[4] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[37] = (((g_authSecret[6] & 0x0000FF00) >> 8) & 0xFF);
	dummyBuffer[57] = (((g_authSecret[8] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[24] = (g_authSecret[2] & 0xFF);
	dummyBuffer[38] = (((g_authSecret[19] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[40] = (((g_authSecret[18] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[45] = (g_authSecret[6] & 0xFF);
	dummyBuffer[31] = (((g_authSecret[3] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[48] = (((g_authSecret[7] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[43] = (((g_authSecret[30] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[51] = (((g_authSecret[1] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[62] = (((g_authSecret[17] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[55] = (((g_authSecret[3] & 0x003FC000) >> 14) & 0xFF);
	dummyBuffer[61] = (((g_authSecret[24] & 0x0007F800) >> 11) & 0xFF);
		
	iButM_computeSHA(dummyBuffer, dummyRet);
		
	// recreate the signature and verify
	if(!(createDataSignature((UINT32 *)dummyRet, g_user.accountFile, scratchpad, g_user.responseMAC, FALSE)))
	{
		return(FALSE);	
	}
	
	return(TRUE);
}


/*****************************************************************************
  Function:      createDataSignature()  
  Description: 	 Creates a data signature for the given data buffer without a 
  				 hardware coprocessor.    
  Inputs:      	 secret: Buffer of the secret
  		         data: The data written to the data page to sign
  		         scratchpad: The data written to the scratchpad to sign 
  		         that must match exactly what is generated on the coprocessor
  		         -or- will hold the resulting signature created by the coprocessor.
  				 readSignature: Implies whether or not the signature buffer
                 receives the contents of the scratchpad or is used to
                 match the contents of the scratchpad. If true, scratchpad 
                 contents are read into the signature buffer. 			        				 	                                                                       
  Outputs:		 signature: Data buffer which is either holding the signature 			                                                                                                                                    
  Return:      	 TRUE, the user's authentication response matched exactly the
            	 signature generated by the coprocessor or the signature was
               	 successfully copied into the return buffer.
         		 FALSE, an error occurred or the signature did not match.
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
static bool createDataSignature(UINT32* secret, UINT32* data,
                               UINT32* scratchpad, UINT32* signature,
                               bool readSignature)
{
   UINT32 i;
   INT32 j;
   INT32 offset;
   INT32 hash[5]; 
   INT32 temp;
   UINT32 digestBuff[64];

   //Set up the 64 byte buffer for computing the digest.
 
   digestBuff[1] = (((secret[4] & 0x0007F800) >> 11) & 0xFF);
   
   //memcpy(&digestBuff[4],data,32);
   for(i=0; i < 32;i++)
   {
   		digestBuff[i + 4] = data[i];
   }
   digestBuff[2] = (((secret[2] & 0xFF000000) >> 24) & 0xFF); 
   digestBuff[50] = (((secret[4] & 0x00000FF0) >> 4) & 0xFF);  
   
   //memcpy(&digestBuff[36],&scratchpad[8],12);
   for(i=0; i < 12;i++)
   {
   		digestBuff[i + 36] = scratchpad[i + 8];
   }  
   digestBuff[0] = (((secret[2] & 0x001FE000) >> 13) & 0xFF);
   
   digestBuff[48] = (((secret[3] & 0x0FF00000) >> 20) & 0xFF);
   digestBuff[51] = (secret[1] & 0xFF);
    
   //memcpy(&digestBuff[52],&scratchpad[20],3);
   digestBuff[52] = scratchpad[20];
   digestBuff[53] = scratchpad[21];
   digestBuff[3] = (((secret[0] & 0x001FE000) >> 13) & 0xFF); 
   digestBuff[54] = scratchpad[22];
  
   //digest buffer padding
   digestBuff[55] = 0x80;
   
   //memset(&digestBuff[56], 0x00, 6);
   digestBuff[56] = 0x00;
   digestBuff[57] = 0x00;
   digestBuff[49] = (((secret[0] & 0x0007F800) >> 11) & 0xFF);
   digestBuff[58] = 0x00;
   digestBuff[59] = 0x00;
   digestBuff[60] = 0x00;
   digestBuff[61] = 0x00;
     
   digestBuff[62] = 0x01;
   digestBuff[63] = 0xB8;

   iButM_computeSHA(digestBuff, hash);

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
         if(readSignature)
         {
            //reading the signature into a buffer
            signature[i+offset] = (temp & 0x000000FF);
         }
         else
         {
            //trying to match the signature;
            if(((signature[i+offset]) & 0x000000FF) != (temp & 0x000000FF))
            {
            	asm("nop;");	// can be removed
            	return(FALSE);
            }
         }
         temp >>= 8;
      }
   }

   return(TRUE);
}


/*****************************************************************************
  Function:      iButH_initIButton(UINT32 randomValue) 
  Description: 	 Initializes the DS1961 SHA iButton
  				 Init struct user & copr, tries to detect PRESENCE PULSE of iButton
				 if true, reads serial number & CRC, if CRC true, checks family code,
				 set various alarm bits       
  Inputs:      	 randomValue: Contains 3 byte random values
  Outputs:		 None			                                                                                                                                    
  Return:      	 TRUE if iButton was detected, CRC of serial no. is OK, 
  				 family code = 0x33
				 FALSE if an error was detected, see copr.alarm_iButton
  Globals:       g_copr, g_user
  Important:                                                    	
*******************************************************************************/ 
bool iButH_initIButton(UINT32 randomValue)
{
	UINT32 i;
	UINT32 howManyTries = 0;	// = HOW_MANY_ACCESS_TRIES;
	volatile bool dSuccess;
	volatile bool rSuccess;
		
	g_copr.alarm_iButton = 0x00000000;
	g_copr.ds1961Scompatible = TRUE;
	
	g_randomValue = randomValue;
	
	g_user.accountPageNumber = DATA_MEMORY_PAGE_0;

	for(i=0; i < (sizeof(g_user.accountFile) / sizeof(UINT32)) ; i++)	//sizeof returns number of bytes!!
	{
		g_user.accountFile[i] = 0x00000000;
	}
	
	// Try to detect an iButton
	do
	{
		dSuccess = iButM_detectIButton();
		rSuccess = FALSE;
		asm("nop;");
		if(dSuccess)
		{
			rSuccess = iButM_readSerialNum(&g_user.devAN[0]);
		}
		asm("nop;");		
	}while( (++howManyTries < HOW_MANY_ACCESS_TRIES) && (!(rSuccess)) );	
	
	if(!dSuccess)		// No device deteted
	{
		g_copr.alarm_iButton |= iBUT_AL_NO_DEVICE_DETECTED;
	}
	
	if(!rSuccess)		// CRC of serial number is invalid
	{
		g_copr.alarm_iButton |= iBUT_AL_INVALID_CRC_SERIAL_NO;
	}
	
	if(g_user.devAN[0] != DS1961S_FAMILY_CODE)
	{
		g_copr.alarm_iButton |= iBUT_AL_WRONG_DEVICE;
	}

	if(g_copr.alarm_iButton)	// any error detected?
	{
		return(FALSE);
	}
		
	return(TRUE);
}


/*****************************************************************************
  Function:      iButH_isIButtonValid()  
  Description: 	 Checks the iButton for validity - challenge/response  			      
  Inputs:      	 None
  Outputs:		 None			                                                                                                                                    
  Return:      	 TRUE if iButton is valid, now the licence can be read
  				 FALSE if iButton is invalid, do not read licence
  Globals:       g_copr, g_user
  Important:                                                    	
*******************************************************************************/ 
bool iButH_isIButtonValid(void)
{
	UINT32 i;
	
	if(verifyUser(FALSE))
	{
		return(TRUE);	
	}
	else
	{
		g_copr.alarm_iButton |= iBUT_AL_iBUTTON_IS_INVALID;
		
		//be sure that accountFile is filled with 0x00 if iButton is invalid
		for(i=0; i < (sizeof(g_user.accountFile) / sizeof(UINT32)) ; i++)	//sizeof returns number of bytes!!
		{
			g_user.accountFile[i] = 0x00000000;
		}
		return(FALSE);
	}
}


/*****************************************************************************
  Function:      iButH_getLicenceCode()  
  Description: 	 Reads the first four bytes of user.accountFile[] and combines 
  				 them to an UINT32  			      
  Inputs:      	 None
  Outputs:		 None			                                                                                                                                    
  Return:      	 The licence code
  Globals:       g_user
  Important:                                                    	
*******************************************************************************/ 
UINT32 iButH_getLicenceCode(void)
{
	UINT32 returnVal;

	returnVal =  ((g_user.accountFile[0] & MASK_8LSB) << SB24);
	returnVal |= ((g_user.accountFile[1] & MASK_8LSB) << SB16);
	returnVal |= ((g_user.accountFile[2] & MASK_8LSB) << SB8);
	returnVal |= (g_user.accountFile[3] & MASK_8LSB);

	return(returnVal);
}



