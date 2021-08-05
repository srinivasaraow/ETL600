/*-------------------------------------------------------------------------*
 * $Workfile: iButHigh.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 1-Wire Interface for iButton
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/C/iButHigh.c $ 
 * 
 * 10    16.11.05 13:52 Chmibuh
 * Cosmetic
 * 
 * 9     4.11.05 19:25 Chchlee
 * Key externalized
 * 
 * 8     13.06.05 20:34 Chmibuh
 * hide the PW, use official PW
 * 
 * 7     3.06.05 10:32 Chmibuh
 * iButH_getLicenceSerieNum() added
 * 
 * 6     17.05.05 18:10 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 5     10.04.05 13:37 Chmibuh
 * iButH_getLicenceCode() added
 * 
 * 4     25.02.05 17:50 Chmibuh
 * 
 * 3     24.02.05 18:20 Chmibuh
 * iButH_licenceErrorAlarm added
 * 
 * 2     23.02.05 14:29 Chmibuh
 * unused functions removed
 * 
 * 1     23.02.05 13:56 Chmibuh
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES *************************************************************/
#include "basetype.h"
#include "iButMed.h"
#include "iButAlarm.h"
#include "rtc.h"		// Needed for pointer to internal Time  
						// -> use msec & sec as random input for iButH_createChallenge

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// How many tries to access the iButton before continue
#define HOW_MANY_ACCESS_TRIES		0x03

#define DS1961S_FAMILY_CODE      	0x33

#define DATA_MEMORY_PAGE_0			0x00
#define DATA_MEMORY_PAGE_1			0x01
#define DATA_MEMORY_PAGE_2			0x02
#define DATA_MEMORY_PAGE_3			0x03

#include "iButPW.h"

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
	unsigned long bindData[32];
	// input value from Master DSP to create a random challenge
	unsigned long *input_rand_chlgPtr;
	// a long to hold various alarm conditions, see iButAlarm.h
	unsigned long alarm_iButton;
	// indicates that the master authentication secret was
	// padded to match the secret of a DS1961S
	BOOL ds1961Scompatible;

} SHACopr;

// struct SHAUser
//   Holds all information pertinent to SHA user tokens.
//   Maintains state between routines for verifying the
//   user's authentication response, the account signature,
//   and updating the account data.
typedef struct
{
	// address of the device, serial number
	unsigned long devAN[8];
	// page the user's account file is stored on
	unsigned long accountPageNumber;
	// MAC from Read Authenticated Page command
	unsigned long responseMAC[20];
	// 32-byte account file
	unsigned long accountFile[32];

} SHAUser;

/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/
	SHACopr copr;
	SHAUser user;

	static SEGMENT_EXT_DATA unsigned long  auth_secret[32] = {HIDE_SECRET_0, 
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
															HIDE_SECRET_31};

	static unsigned long  wspc_secret[8];

	static BOOL g_LicenceErrorAlarm;


/* LOCAL FUNCTIONS ***********************************************************/
BOOL SEGMENT_EXT_CODE iButH_VerifyAuthResponse(SHACopr* copr, SHAUser* user, unsigned long* chlg, BOOL doBind);
BOOL SEGMENT_EXT_CODE iButH_CreateDataSignature(SHACopr* copr, unsigned long* secret, unsigned long* data,
                               unsigned long* scratchpad, unsigned long* signature,
                               BOOL readSignature);


/* IMPLEMENTATION ************************************************************/



//-------------------------------------------------------------------------
// Answers a random challenge by performing an authenticated read of the
// user's account information.
//
// 'user'      - Structure for holding user token information.
// 'chlg'      - 3-byte buffer of challenge data.
//
// Return: the value of the write cycle counter for the account page.
//         or -1 if there is an error
//
BOOL SEGMENT_EXT_CODE iButH_AnswerChallenge(SHAUser* user, unsigned long* chlg)
{
	unsigned long i;
	unsigned long addr = user->accountPageNumber << 5;
		
	user->accountFile[20] = ((*chlg++) & 0xFF);
	user->accountFile[21] = ((*chlg++) & 0xFF);
	user->accountFile[22] = ((*chlg) & 0xFF);
			
	if(!(iButM_WriteScratchpad(addr, &user->accountFile[16])))
	{
		return(FALSE);	
	}
	
	iButM_prepareTransfer();
		
	if(!(iButM_ReadAuthPage(user->accountPageNumber, user->accountFile, user->responseMAC)))
	{
		return(FALSE);			
	}
                              
    return(TRUE);
}


//-------------------------------------------------------------------------
// Creates a 3-Byte random challenge
//
// 'input_chlg'	- "random" value from Master DSP, msec & seconds of RTC
//				  
// 'chlg'	    - destination pointer - puts 3 Bytes
//
void SEGMENT_EXT_CODE iButH_createChallenge(unsigned long *input_chlgPtr, unsigned long* chlg)
{
	unsigned long temp;

	temp = (*(input_chlgPtr++));		//  mseconds
	temp |= ((*(input_chlgPtr)) << 11);	//	mseconds max. 1000dec = 10 Bits -> shift seconds 11 bits and OR

	// shift input a little around and xor it.
	chlg[0] = (((temp & 0x00000FF0) >> 4) & 0xFF);
	chlg[1] = ((temp >> 5) & 0xFF);
	chlg[2] = (((temp >> 7) ^ (chlg[1])) & 0xFF); 
}


//-------------------------------------------------------------------------
// Verifies a user token as a valid member of the system.  First, a random
// challenge is generated.  Then, the user must answer that challenge.
// The user's response signature is then verified against the proper
// response..
//
// 'copr'      - Structure for holding coprocessor information.
// 'user'      - Structure for holding user token information.
// 'doBind'    - if true, the user's unique secret is recreated on the
//               coprocessor.  If this function is called multiple times,
//               it is acceptable to skip the bind for all calls after
//               the first on the same user token.
//
// Return: If TRUE, user was verified.
//         If FALSE, an error occurred or user verification failed.
//
BOOL SEGMENT_EXT_CODE iButH_VerifyUser(SHACopr* copr, SHAUser* user, BOOL doBind)
{
   unsigned long chlg[3];

   // Create Challenge  
   iButH_createChallenge(copr->input_rand_chlgPtr, chlg);
      
   if(!(iButH_AnswerChallenge(user, chlg)))
   {
		return(FALSE);   	
   }
   
   if(!(iButH_VerifyAuthResponse(copr, user, chlg, doBind)))
   {
		return(FALSE);
   }

   return(TRUE);
}



//-------------------------------------------------------------------------
// Verifies the authentication response of a user token without a
// hardware coprocessor.
//
// 'copr'      - Structure for holding coprocessor information.
// 'user'      - Structure for holding user token information.
// 'chlg'      - 3-byte buffer of challenge data.
// 'doBind'    - if true, the user's unique secret is recreated on the
//               coprocessor.  If this function is called multiple times,
//               it is acceptable to skip the bind for all calls after
//               the first on the same user token.
//
// Return: If TRUE, the user's authentication response matched exactly the
//            signature generated by the coprocessor.
//         If FALSE, an error occurred or the signature did not match.
//
BOOL SEGMENT_EXT_CODE iButH_VerifyAuthResponse(SHACopr* copr, SHAUser* user, unsigned long* chlg, BOOL doBind)
{
	unsigned long scratchpad[32];
	unsigned long fullBindCode[15];
	unsigned long i;
	unsigned long temp_buf[32];
	// M-X control byte
	unsigned long MXP = 0x00;
	unsigned long dummyBuffer[64];
	long dummyRet[5];

	dummyBuffer[59] = (((auth_secret[6] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[29] = (((auth_secret[1] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[44] = (((auth_secret[0] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[13] = (((auth_secret[7] & 0x0FF00000) >> 20) & 0xFF); 

	//   memset(scratchpad, 0x00, 32);
	for(i=0; i < 32; i++)
	{
		scratchpad[i] = 0x00;
	}
	dummyBuffer[20] = (((auth_secret[18] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[30] = (((auth_secret[11] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[33] = (((auth_secret[27] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[15] = (((auth_secret[9] & 0x003FC000) >> 14) & 0xFF);
	dummyBuffer[49] = (((auth_secret[22] & 0x00FF0000) >> 16) & 0xFF); 
	
	//   memset(fullBindCode, 0x0FF, 15);
	for(i=0; i < 15; i++)
	{
		fullBindCode[i] = 0xFF;
	}
	dummyBuffer[6] = (((auth_secret[31] & 0x00FF0000) >> 16) & 0xFF); 
	dummyBuffer[10] = (((auth_secret[6] & 0x00000FF0) >> 4) & 0xFF);
	dummyBuffer[36] = (((auth_secret[21] & 0x001FE000) >> 13) & 0xFF);
	  
	// Leave bindCode FF for DS1961S
	// Use AuthenticateHost command
	MXP = 0x40;
	// the user doesn't have a write cycle counter
	//memset(&scratchpad[8], 0x0FF, 4);
	scratchpad[8] = 0x0FF;
	dummyBuffer[3] = (((auth_secret[8] & 0x00000FF0) >> 4) & 0xFF); 
	scratchpad[9] = 0x0FF;
	dummyBuffer[39] = (auth_secret[9] & 0xFF);	
	dummyBuffer[54] = (((auth_secret[17] & 0x0000FF00) >> 8) & 0xFF);  
	scratchpad[10] = 0x0FF;
	scratchpad[11] = 0x0FF;
	dummyBuffer[5] = (auth_secret[17] & 0xFF);
	dummyBuffer[32] = (((auth_secret[2] & 0x003FC000) >> 14) & 0xFF);
      
	// the pagenumber
	fullBindCode[4] = user->accountPageNumber;
	dummyBuffer[60] = (auth_secret[7] & 0xFF);
	dummyBuffer[56] = (((auth_secret[27] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[25] = (((auth_secret[31] & 0x0FF00000) >> 20) & 0xFF); 
	// and 7 bytes of the address of current device
	//memcpy(&fullBindCode[5], user->devAN, 7);
	for(i=0; i < 7; i++)
	{
		fullBindCode[5 + i] = user->devAN[i];  	
	}
	dummyBuffer[63] = (((auth_secret[0] & 0x00FF0000) >> 16) & 0xFF); 
	dummyBuffer[52] = (((auth_secret[24] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[1] = (((auth_secret[8] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[42] = (((auth_secret[2] & 0x0007F800) >> 11) & 0xFF);
	
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
	dummyBuffer[21] = (((auth_secret[29] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[47] = (((auth_secret[16] & 0x003FC000) >> 14) & 0xFF);
	dummyBuffer[11] = (((auth_secret[18] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[19] = (((auth_secret[26] & 0x0FF00000) >> 20) & 0xFF); 
		
	// Fix the M-X control bits
	scratchpad[12] = ((scratchpad[12]&0x3F)|(MXP&0xC0));

	dummyBuffer[0] = (((auth_secret[25] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[58] = (((auth_secret[5] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[14] = (((auth_secret[3] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[35] = (((auth_secret[9] & 0x00FF0000) >> 16) & 0xFF); 
	//memset(temp_buf, 0x00, 32);
	for(i=0; i < 32; i++)
	{
		temp_buf[i] = 0x00;
	}
	dummyBuffer[4] = (((auth_secret[5] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[16] = (auth_secret[14] & 0xFF);
	dummyBuffer[53] = (((auth_secret[13] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[50] = (((auth_secret[4] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[18] = (((auth_secret[15] & 0x0007F800) >> 11) & 0xFF);
	
	//memcpy(&temp_buf[8],fullBindCode,15);
	for(i=0; i < 15; i++)
	{
		temp_buf[8 + i] = fullBindCode[i];
	}
	
	
	dummyBuffer[8] = (((auth_secret[28] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[12] = (((auth_secret[2] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[17] = (((auth_secret[25] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[23] = (((auth_secret[17] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[2] = (((auth_secret[16] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[46] = (((auth_secret[1] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[26] = (((auth_secret[26] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[22] = (((auth_secret[6] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[27] = (((auth_secret[12] & 0x00FF0000) >> 16) & 0xFF); 
	dummyBuffer[7] = (((auth_secret[21] & 0x000FF000) >> 12) & 0xFF);  
	dummyBuffer[41] = (((auth_secret[8] & 0x0FF00000) >> 20) & 0xFF); 
	dummyBuffer[28] = (((auth_secret[29] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[9] = (((auth_secret[0] & 0x0000FF00) >> 8) & 0xFF);
	dummyBuffer[34] = (((auth_secret[4] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[37] = (((auth_secret[6] & 0x0000FF00) >> 8) & 0xFF);
	dummyBuffer[57] = (((auth_secret[8] & 0x0007F800) >> 11) & 0xFF);
	dummyBuffer[24] = (auth_secret[2] & 0xFF);
	dummyBuffer[38] = (((auth_secret[19] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[40] = (((auth_secret[18] & 0x001FE000) >> 13) & 0xFF);
	dummyBuffer[45] = (auth_secret[6] & 0xFF);
	dummyBuffer[31] = (((auth_secret[3] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[48] = (((auth_secret[7] & 0xFF000000) >> 24) & 0xFF); 
	dummyBuffer[43] = (((auth_secret[30] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[51] = (((auth_secret[1] & 0x0000FF00) >> 8) & 0xFF);  
	dummyBuffer[62] = (((auth_secret[17] & 0x000FF000) >> 12) & 0xFF); 
	dummyBuffer[55] = (((auth_secret[3] & 0x003FC000) >> 14) & 0xFF);
	dummyBuffer[61] = (((auth_secret[24] & 0x0007F800) >> 11) & 0xFF);
		
	iButM_ComputeSHA(dummyBuffer, dummyRet);

		
	// recreate the signature and verify
	if(!(iButH_CreateDataSignature(copr, (unsigned long *)dummyRet, user->accountFile, scratchpad, user->responseMAC, FALSE)))
	{
		return(FALSE);	
	}
	
	return(TRUE);
}



//-------------------------------------------------------------------------
// Creates a data signature for the given data buffer without a hardware
// coprocessor.
//
// 'copr'          - Structure for holding coprocessor information.
// 'data'          - data written to the data page to sign
// 'scratchpad'    - data written to the scratchpad to sign
// 'signature'     - data buffer which is either holding the signature that
//                   must match exactly what is generated on the coprocessor
//                   -or- will hold the resulting signature created by the
//                   coprocessor.
// 'readSignature' - implies whether or not the signature buffer
//                   receives the contents of the scratchpad or is used to
//                   match the contents of the scratchpad.  If true,
//                   scratchpad contents are read into the signature buffer.
//
// Return: If TRUE, the user's authentication response matched exactly the
//            signature generated by the coprocessor or the signature was
//            successfully copied into the return buffer.
//         If FALSE, an error occurred or the signature did not match.
//
BOOL SEGMENT_EXT_CODE iButH_CreateDataSignature(SHACopr* copr, unsigned long* secret, unsigned long* data,
                               unsigned long* scratchpad, unsigned long* signature,
                               BOOL readSignature)
{
   long i,j,offset;
   long hash[5], temp;
   unsigned long digestBuff[64];

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

   iButM_ComputeSHA(digestBuff, hash);

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



//----------------------------------------------------------------------
// initialize the DS1961 SHA iButton
//
// init struct user & copr
// tries to detect PRESENCE PULSE of iButton
// if true, reads serial number & CRC
// if CRC true, checks family code
// set various alarm bits
//
// Return:	TRUE  - iButton was detected, CRC of serial no. is OK, family code = 0x33
// 			FALSE - an error was detected, see copr.alarm_iButton
//
BOOL SEGMENT_EXT_CODE iButH_init_iButton(void)
{
	unsigned long i, how_many_tries = 0;	// = HOW_MANY_ACCESS_TRIES;
	volatile BOOL d_success, r_success;
		
	copr.alarm_iButton = 0x00000000;
	copr.ds1961Scompatible = TRUE;
	
	copr.input_rand_chlgPtr = (unsigned long*)rtc_getIntTimePtr();
	
	user.accountPageNumber = DATA_MEMORY_PAGE_0;

	for(i=0; i < sizeof(user.accountFile); i++)
	{
		user.accountFile[i] = 0x00000000;
	}
	
	g_LicenceErrorAlarm = FALSE;
	
	// Try to detect an iButton
	do
	{
		d_success = iButM_Detect_iButton();
		r_success = FALSE;
		asm("nop;");
		if(d_success)
		{
			r_success = iButM_ReadSerialNum(&user.devAN[0]);
		}
		asm("nop;");		
	}while( (++how_many_tries < HOW_MANY_ACCESS_TRIES) && (!(r_success)) );	
	
	if(!d_success)		// No device deteted
	{
		copr.alarm_iButton |= iBUT_AL_NO_DEVICE_DETECTED;
	}
	
	if(!r_success)		// CRC of serial number is invalid
	{
		copr.alarm_iButton |= iBUT_AL_INVALID_CRC_SERIAL_NO;
	}
	
	if(user.devAN[0] != DS1961S_FAMILY_CODE)
	{
		copr.alarm_iButton |= iBUT_AL_WRONG_DEVICE;
	}

	if(copr.alarm_iButton)	// any error detected?
	{
		g_LicenceErrorAlarm = TRUE;
		return(FALSE);
	}
		
	return(TRUE);
}

//----------------------------------------------------------------------
// Check the iButton for validity - challenge/response 
//
// Return:	TRUE  - iButton is valid - now read the licence data (which fct. is enabled)
//			FALSE - iButton is invalid - do not read the data
//
BOOL SEGMENT_EXT_CODE iButH_is_iButton_valid(void)
{
	unsigned long i;
	
	if(iButH_VerifyUser(&copr, &user, FALSE))
	{
		return(TRUE);	
	}
	else
	{
		copr.alarm_iButton |= iBUT_AL_iBUTTON_IS_INVALID;
		g_LicenceErrorAlarm = TRUE;
		//be sure that accountFile is filled with 0x00 if iButton is invalid
		for(i=0; i < sizeof(user.accountFile); i++)
		{
			user.accountFile[i] = 0x00000000;
		}
		return(FALSE);
	}
}


//----------------------------------------------------------------------
// Reads the first four bytes of user.accountFile[] and combine them to a unsigned long
//
// Return:	unsigned long licenceCode
//			
//
unsigned long SEGMENT_EXT_CODE iButH_getLicenceCode(void)
{
	unsigned long returnVal;


  #ifdef P4LW_PTTY2		
	returnVal = 0x00010401;		//enable all
  #else
	returnVal =  ((user.accountFile[0] & MASK_8LSB) << SB24);
	returnVal |= ((user.accountFile[1] & MASK_8LSB) << SB16);
	returnVal |= ((user.accountFile[2] & MASK_8LSB) << SB8);
	returnVal |= (user.accountFile[3] & MASK_8LSB);
  #endif

	return(returnVal);
}


void SEGMENT_EXT_CODE iButH_getLicenceSerieNum(t_P4LT_Status *p_StatusData)
{
// Complete information including device family and CRC
	p_StatusData->system.serieNumber_iButton[0] = ((user.devAN[0] & MASK_8LSB) << SB24);	//device family
	p_StatusData->system.serieNumber_iButton[0] |= ((user.devAN[1] & MASK_8LSB) << SB16); 
	p_StatusData->system.serieNumber_iButton[0] |= ((user.devAN[2] & MASK_8LSB) << SB8); 
	p_StatusData->system.serieNumber_iButton[0] |= (user.devAN[3] & MASK_8LSB); 

	p_StatusData->system.serieNumber_iButton[1] = ((user.devAN[4] & MASK_8LSB) << SB24); 
	p_StatusData->system.serieNumber_iButton[1] |= ((user.devAN[5] & MASK_8LSB) << SB16); 
	p_StatusData->system.serieNumber_iButton[1] |= ((user.devAN[6] & MASK_8LSB) << SB8); 
	p_StatusData->system.serieNumber_iButton[1] |= (user.devAN[7] & MASK_8LSB);				//CRC
}



BOOL SEGMENT_EXT_CODE iButH_licenceErrorAlarm(void)
{
	return g_LicenceErrorAlarm;
}
