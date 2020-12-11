/*-------------------------------------------------------------------------*
 * $Workfile	: subframe.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/subframe.c $
 * 
 * 6     07-11-27 16:21 Chrobif
 * + Debug telephony infrastructure added
 * 
 * 5     07-07-16 15:21 Chalnot
 * Set mWireDataReceived flag if mWire data received.
 * 
 * 4     07-07-10 12:51 Chalnot
 * Changed mWireBufferServicePhone from array to one value.
 * 
 * 3     4.07.07 13:48 Chhamaa
 * Input parameter data type of subframe_init() modified.
 * 
 * 2     4.07.07 13:27 Chhamaa
 * Constants  DATARATE_6300 and DATARATE_5300 removed.
 * 
 * 1     07-06-11 10:04 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES ***********************************************************/

#include "BASETYPE.H"
#include "Basetype_O4CV.h"
#include "O4CV_Const.h"
#include "subframe.h"
#include "binInOut.h"

/* LOCAL CONSTANT DECLARATIONS *********************************************/

// Signalling types
#define SIGNALLING_TYPE_EMPTY		0					//00
#define SIGNALLING_TYPE_DTMF		BIT0				//01	
#define SIGNALLING_TYPE_CPTONE		BIT1				//10	
#define SIGNALLING_TYPE_EANDM		BIT1 | BIT0			//11	

// E&M Codewords
#define EANDM_CODEWORD_0000			0x000
#define EANDM_CODEWORD_1000			0x8AB
#define EANDM_CODEWORD_0100			0x467
#define EANDM_CODEWORD_1100			0xCCC
#define EANDM_CODEWORD_0010			0x21F
#define EANDM_CODEWORD_1010			0xAB4
#define EANDM_CODEWORD_0110			0x678
#define EANDM_CODEWORD_1110			0xED3
#define EANDM_CODEWORD_0001			0x1FE
#define EANDM_CODEWORD_1001			0x955
#define EANDM_CODEWORD_0101			0x599
#define EANDM_CODEWORD_1101			0xD32
#define EANDM_CODEWORD_0011			0x3E1
#define EANDM_CODEWORD_1011			0xB4A
#define EANDM_CODEWORD_0111			0x786
#define EANDM_CODEWORD_1111			0xF2D		

/* LOCAL TYPE DECLARATIONS *************************************************/

/* GLOBAL VARIABLE DECLARATIONS ********************************************/

/* LOCAL VARIABLE DECLARATIONS *********************************************/

static UINT16 g_datarate = CH_VOICE_DR_5300; 
														
static const UINT16 g_eAndMCodewordTable [16] = {	EANDM_CODEWORD_0000,	// 0
													EANDM_CODEWORD_0001,	// 1	
													EANDM_CODEWORD_0010,	// 2
													EANDM_CODEWORD_0011,	// 3
													EANDM_CODEWORD_0100,	// 4
													EANDM_CODEWORD_0101,	// 5
													EANDM_CODEWORD_0110,	// 6
													EANDM_CODEWORD_0111,	// 7
													EANDM_CODEWORD_1000,	// 8
													EANDM_CODEWORD_1001,	// 9
													EANDM_CODEWORD_1010,	// 10
													EANDM_CODEWORD_1011,	// 11
													EANDM_CODEWORD_1100,	// 12
													EANDM_CODEWORD_1101,	// 13
													EANDM_CODEWORD_1110,	// 14
													EANDM_CODEWORD_1111};	// 15																								
													
static const UINT16 g_eAndMEncodeTable [32] = {0,1,1,2,3,4,4,5,6,7,7,0,1,2,2,3,4,
												5,5,6,7,0,0,1,2,3,3,4,5,6,6,7};
static const UINT16 g_eAndMDecodeTable0 [8] = {0,2,3,4,6,7,8,10};
static const UINT16 g_eAndMDecodeTable1 [8] = {11,12,14,15,16,18,19,20};
static const UINT16 g_eAndMDecodeTable2 [8] = {22,23,24,26,27,28,30,31};
							
/* LOCAL MACRO DEFINITIONS *************************************************/

/* LOCAL FUNCTION DEFINITIONS **********************************************/

void mWireEncode(UINT8* pMWireBuffer , UINT16* pCodeWord);
UINT16 mWireDecode(UINT8* pMWireBuffer , UINT16* pCodeWord);

/* IMPLEMENTATION **********************************************************/

/***************************************************************************
  Function:		subframe_init()		
  Description:	Sets the datarate.
  Inputs:		datarate: Datarate of the voicecoder (5300 or 6300kbps)		
  Outputs:		-
  Return:		-
  Globals:		g_datarate		
  Important: 	
                                          	
****************************************************************************/
void subframe_init(UINT16 datarate)
{
 	g_datarate = datarate;   
}


/***************************************************************************
  Function:		subframe_pack()		
  Description:	Generates the 3 subframes with the data in the TFPayload
  Inputs:		pTFPayloadIn: Pointer to the TFPayload  
   				pMWireDataIn: Pointer to structure with m-wire data		
  Outputs:		pSubframeOut: Pointer to the 3 subframes 	
  Return:		-
  Globals:		-		
  Important: 	
                                          	
****************************************************************************/
void subframe_pack(	TFPayloadStruc *pTFPayloadIn, 
					t_mWireData *pMWireDataIn,
					t_Subframe *pSubframeOut)						
{
    UINT32 i = 0;
    UINT32 j = 0;
    
    UINT16 codeword[3];
    
    bool isSignallingFrame = false;		// Defines if E&M data will be stored to the payload

    if(pMWireDataIn->insertMWireFrame 			// Only insert an E&M frame if voice or fax 				
    	&& ((pTFPayloadIn->PayloadDataType == PAYLOAD_VOCODER)
    		| (pTFPayloadIn->PayloadDataType == PAYLOAD_FAX))) 	
    {
        // Store signal subframeheaders to subframes
     	pSubframeOut[0][0] = SUBFRAME_HEADER_SIGNAL_1; 
		pSubframeOut[1][0] = SUBFRAME_HEADER_SIGNAL_2; 
		pSubframeOut[2][0] = SUBFRAME_HEADER_SIGNAL_3; 
			
		for(j = 0; j <= 2; j++)
		{
		    // Store type E&M to the type section of the subframes
			pSubframeOut[j][0] |= ((SIGNALLING_TYPE_EANDM) << 3 | (SIGNALLING_TYPE_EANDM) << 1)  & 0x1E; 
			
			// Store suppressed frame type to the data section of the subframes
			if(pTFPayloadIn->PayloadDataType == PAYLOAD_VOCODER)
			{ 
			 	pSubframeOut[j][2] = BIT2; 
			 	pSubframeOut[j][4] = BIT2;     
			}
			else if(pTFPayloadIn->PayloadDataType == PAYLOAD_FAX)
			{
			 	pSubframeOut[j][2] = 0;    
			 	pSubframeOut[j][4] = 0;      
			}
		}
			
		isSignallingFrame = true;   
    }
    else
    {
	    switch (pTFPayloadIn->PayloadDataType)
		{
			case PAYLOAD_VOCODER: 	
				// Store voice subframeheaders to subframes
				pSubframeOut[0][0] = SUBFRAME_HEADER_VOICE_1; 
				pSubframeOut[1][0] = SUBFRAME_HEADER_VOICE_2; 
				pSubframeOut[2][0] = SUBFRAME_HEADER_VOICE_3; 
			
				if(pTFPayloadIn->PayloadDataBuf[0] & BIT6)
				{	
				    // Datarate = 5300kbps

				    // Store bytes 0-19 of the PayloadDataBuf to the subframes
					// Subframe 0
					pSubframeOut[0][0] |= (pTFPayloadIn->PayloadDataBuf[0] >> 1) & 0x1F;				// Bit4-Bit0
					for(i = 1; i <= 6; i++)
					{
					 	pSubframeOut[0][i] = (pTFPayloadIn->PayloadDataBuf[i-1] << 7);					
						pSubframeOut[0][i] |= (pTFPayloadIn->PayloadDataBuf[i] >> 1) & 0x7F;			// Bit6-Bit0	 	    	
					}
				
					// Subframe 1
					pSubframeOut[1][0] |= (pTFPayloadIn->PayloadDataBuf[6] << 4) & 0x10;				// Bit4
					pSubframeOut[1][0] |= (pTFPayloadIn->PayloadDataBuf[7] >> 4) & 0x0F;				// Bit3-Bit0
					for(i = 1; i <= 6; i++)
					{
					 	pSubframeOut[1][i] = (pTFPayloadIn->PayloadDataBuf[i+7-1] << 4);				
						pSubframeOut[1][i] |= (pTFPayloadIn->PayloadDataBuf[i+7] >> 4) & 0x0F;			// Bit3-Bit0	 	    	
					}
				
					// Subframe 2
					pSubframeOut[2][0] |= (pTFPayloadIn->PayloadDataBuf[13] << 1) & 0x1E;				// Bit4-Bit1
					pSubframeOut[2][0] |= (pTFPayloadIn->PayloadDataBuf[14] >> 7) & 0x01;				// Bit0
					for(i = 1; i <= 6; i++)
					{
					 	pSubframeOut[2][i] = (pTFPayloadIn->PayloadDataBuf[i+14-1] << 1);				
						pSubframeOut[2][i] |= (pTFPayloadIn->PayloadDataBuf[i+14] >> 7) & 0x01;			// Bit0	 	    	
					}	
					
				}	
				else
				{	
				    // Datarate = 6300kbps
	
				    // Store bytes 0-23 of the PayloadDataBuf to the subframes
					for(j = 0; j <= 2; j++)
					{
					    pSubframeOut[j][0] |= (pTFPayloadIn->PayloadDataBuf[8*j] >> 1) & 0x1F;			// Bit4-Bit0
						for(i = 1; i <= 8; i++)
						{
						    pSubframeOut[j][i] = (pTFPayloadIn->PayloadDataBuf[i+8*j-1] << 7);			
						
						    if(i+8*j <= 23)		// There are only 23 bytes in the PayloadDataBuf
							{
						    	pSubframeOut[j][i] |= (pTFPayloadIn->PayloadDataBuf[i+8*j] >> 1) & 0x7F;// Bit6-Bit0	 	    	
							}
						}	
					}
				}
			
				break;
					
			case PAYLOAD_FAX: 	
				// Store fax subframeheaders to subframes
				pSubframeOut[0][0] = SUBFRAME_HEADER_FAX_1; 
				pSubframeOut[1][0] = SUBFRAME_HEADER_FAX_2; 
				pSubframeOut[2][0] = SUBFRAME_HEADER_FAX_3; 
			
				// Store bytes 0-19 of the PayloadDataBuf to the subframes
				// Subframe 0
				pSubframeOut[0][0] |= (pTFPayloadIn->PayloadDataBuf[0] >> 1) & 0x1F;				// Bit4-Bit0
				for(i = 1; i <= 6; i++)
				{
				 	pSubframeOut[0][i] = (pTFPayloadIn->PayloadDataBuf[i-1] << 7);					
					pSubframeOut[0][i] |= (pTFPayloadIn->PayloadDataBuf[i] >> 1) & 0x7F;			// Bit6-Bit0	 	    	
				}
				
				// Subframe 1
				pSubframeOut[1][0] |= (pTFPayloadIn->PayloadDataBuf[6] << 4) & 0x10;				// Bit4
				pSubframeOut[1][0] |= (pTFPayloadIn->PayloadDataBuf[7] >> 4) & 0x0F;				// Bit3-Bit0
				for(i = 1; i <= 6; i++)
				{
				 	pSubframeOut[1][i] = (pTFPayloadIn->PayloadDataBuf[i+7-1] << 4);				
					pSubframeOut[1][i] |= (pTFPayloadIn->PayloadDataBuf[i+7] >> 4) & 0x0F;			// Bit3-Bit0	 	    	
				}
				
				// Subframe 2
				pSubframeOut[2][0] |= (pTFPayloadIn->PayloadDataBuf[13] << 1) & 0x1E;				// Bit4-Bit1
				pSubframeOut[2][0] |= (pTFPayloadIn->PayloadDataBuf[14] >> 7) & 0x01;				// Bit0
				for(i = 1; i <= 6; i++)
				{
				 	pSubframeOut[2][i] = (pTFPayloadIn->PayloadDataBuf[i+14-1] << 1);				
					pSubframeOut[2][i] |= (pTFPayloadIn->PayloadDataBuf[i+14] >> 7) & 0x01;			// Bit0	 	    	
				}	
				
				break;
			
			case PAYLOAD_NODATA: 	
				// Store signal subframeheaders to subframes
				pSubframeOut[0][0] = SUBFRAME_HEADER_SIGNAL_1; 
				pSubframeOut[1][0] = SUBFRAME_HEADER_SIGNAL_2; 
				pSubframeOut[2][0] = SUBFRAME_HEADER_SIGNAL_3; 
			
				for(j = 0; j <= 2; j++)
				{
				    // Store type EMPTY to the type section of the subframes
					pSubframeOut[j][0] |= (SIGNALLING_TYPE_EMPTY << 3 | SIGNALLING_TYPE_EMPTY << 1)  & 0x1E; 
					pSubframeOut[j][2] = 0;
				}
			
				isSignallingFrame = true;
				
				break;
			
			case PAYLOAD_DTMF: 	
				// Store signal subframeheaders to subframes
				pSubframeOut[0][0] = SUBFRAME_HEADER_SIGNAL_1; 
				pSubframeOut[1][0] = SUBFRAME_HEADER_SIGNAL_2; 
				pSubframeOut[2][0] = SUBFRAME_HEADER_SIGNAL_3; 
			
				for(j = 0; j <= 2; j++)
				{
				    // Store type DTMF to the type section of the subframes
					pSubframeOut[j][0] |= (SIGNALLING_TYPE_DTMF << 3 | SIGNALLING_TYPE_DTMF << 1) & 0x1E; 

					// Store byte 1 of the PayloadDataBuf to the subframes
					pSubframeOut[j][2] = pTFPayloadIn->PayloadDataBuf[1] >> 5; 
					pSubframeOut[j][3] = pTFPayloadIn->PayloadDataBuf[1] << 3;
					pSubframeOut[j][4] = pTFPayloadIn->PayloadDataBuf[1] >> 5; 
					pSubframeOut[j][5] = pTFPayloadIn->PayloadDataBuf[1] << 3;
				}
			
				isSignallingFrame = true;
				
				break;
			
			case PAYLOAD_CPTONE: 	
				// Store signal subframeheaders to subframes
				pSubframeOut[0][0] = SUBFRAME_HEADER_SIGNAL_1; 
				pSubframeOut[1][0] = SUBFRAME_HEADER_SIGNAL_2; 
				pSubframeOut[2][0] = SUBFRAME_HEADER_SIGNAL_3; 
			
				for(j = 0; j <= 2; j++)
				{
				    // Store type CPTONE to the type section of the subframes
					pSubframeOut[j][0] |= (SIGNALLING_TYPE_CPTONE << 3 | SIGNALLING_TYPE_CPTONE << 1) & 0x1E; 

					// Store bytes 1-2 of the PayloadDataBuf to the subframes
					pSubframeOut[j][2] = pTFPayloadIn->PayloadDataBuf[1] >> 5; 
					pSubframeOut[j][3] = pTFPayloadIn->PayloadDataBuf[1] << 3;
					pSubframeOut[j][3] |= (pTFPayloadIn->PayloadDataBuf[2] >> 5) & 0x07; 
					pSubframeOut[j][4] = pTFPayloadIn->PayloadDataBuf[2] << 3;
					
					pSubframeOut[j][4] |= (pTFPayloadIn->PayloadDataBuf[1] >> 5) & 0x07;  
					pSubframeOut[j][5] = pTFPayloadIn->PayloadDataBuf[1] << 3;
					pSubframeOut[j][5] |= (pTFPayloadIn->PayloadDataBuf[2] >> 5) & 0x07;; 
					pSubframeOut[j][6] = pTFPayloadIn->PayloadDataBuf[2] << 3;
			
				}
			
				isSignallingFrame = true;
				
				break;			
														
			default: 	
						
				break;	
				
		} // End switch (pTFPayloadIn->PayloadDataType)

    } // End else of if(insertMWireFrame)
    
    	
	// Add E&M if it is a signalling frame
	if(isSignallingFrame)
	{
#ifdef DEBUG_TELEPHONY
		binInOut_toggleOutPin(e_Buf_EW2, TOGGLE_OUT_PIN_INSTANCE_2);
#endif
	    
	    // E&M from normal phone
	 	mWireEncode(pMWireDataIn->mWireBuffer , &codeword[0]); 
	 	 
	 	// Store codewords to the E&M section of the subframes
	 	for(j = 0; j <= 2; j++)
		{
		    pSubframeOut[j][0] |= (codeword[j] >> 11) & 0x01;
			pSubframeOut[j][1] = codeword[j] >> 3;
			pSubframeOut[j][2] |= (codeword[j] << 5) & 0xE0; 
		}	
		
		// E&M from service phone
		if(pMWireDataIn->servicePhoneOn)
		{
		 	for(j = 0; j <= 2; j++)
			{
			    // Set servicephone m-wire bits in the subframes like the last m-wire sample
				if(pMWireDataIn->mWireBufferServicePhone)	
				{
					pSubframeOut[j][2] |= BIT4;
					pSubframeOut[j][2] |= BIT3;
				}
				else
				{
				  	pSubframeOut[j][2] &= ~BIT4;
					pSubframeOut[j][2] &= ~BIT3;
				}
			}	
		}
	} // End if(isSignallingFrame) 
} // End subframe_pack


/***************************************************************************
  Function:		subframe_unpack()		
  Description:	Generates the TFPayload and the M-Wire data with the RxFrame 	
  Inputs:		pRxFrame: Pointer to a structure which includes 3 pointers to 
  				the subframes and their type and sync. 
  				pMWireDataOut->servicePhoneOn: Shows if the service phone
  				is enabled 				
  Outputs:		pTFPayloadOut: Pointer to the TFPayload
  				pMWireDataOut: Pointer to structure with M-Wire data		
  Return:		-
  Globals:		-		
  Important: 	
                                          	
****************************************************************************/
void subframe_unpack(	t_RxFrameForDecoder* pRxFrame,	
						TFPayloadStruc* pTFPayloadOut, 
						t_mWireData *pMWireDataOut)						
{						
	UINT32 i = 0;
	UINT32 j = 0;
	
    UINT16 signallingFrameType = 0;
    UINT16 signallingFramePayload[5];
    UINT16 numberOfOnes = 0;
    
    UINT16 codeword[3];
    
	pMWireDataOut->mWireDataReceived = false;

    // Switch type					
	switch (pRxFrame->iMin)
	{		
		case 1: // Voice
    	
			// Set PayloadDataType to voice
			pTFPayloadOut->PayloadDataType = PAYLOAD_VOCODER;	
		
			if(g_datarate == CH_VOICE_DR_6300)
			{
			 	// Datarate = 6300kbps
				
			 	// Restore bytes 0-23 of the PayloadDataBuf from the subframes
			 	// Fill Payload Byte 0
				pTFPayloadOut->PayloadDataBuf[0] = BIT7;												// Bit 7-6
				pTFPayloadOut->PayloadDataBuf[0] |= (pRxFrame->pSubframe1[0][0] << 1) & 0x3E;			// Bit 5-1
				pTFPayloadOut->PayloadDataBuf[0] |= (pRxFrame->pSubframe1[0][1] >> 7) & 0x01;			// Bit 0	
				for(i = 1; i <= 7; i++)
				{
				    // Fill Payload Byte 1-7
				    pTFPayloadOut->PayloadDataBuf[i] = (pRxFrame->pSubframe1[0][i] << 1);				// Bit 7-1
					pTFPayloadOut->PayloadDataBuf[i] |= (pRxFrame->pSubframe1[0][i+1] >> 7) & 0x01;		// Bit 0		 	    	
				}	
				pTFPayloadOut->PayloadDataBuf[8] = (pRxFrame->pSubframe1[0][8] << 1);					// Bit 7-6
					
				// Fill Payload Byte 8																			
				pTFPayloadOut->PayloadDataBuf[8] |= (pRxFrame->pSubframe2[0][0] << 1) & 0x3E;			// Bit 5-1
				pTFPayloadOut->PayloadDataBuf[8] |= (pRxFrame->pSubframe2[0][1] >> 7) & 0x01;			// Bit 0	
				for(i = 1; i <= 7; i++)
				{
				    // Fill Payload Byte 9-15
				    pTFPayloadOut->PayloadDataBuf[i+8] = (pRxFrame->pSubframe2[0][i] << 1);				// Bit 7-1
					pTFPayloadOut->PayloadDataBuf[i+8] |= (pRxFrame->pSubframe2[0][i+1] >> 7) & 0x01;	// Bit 0		 	    	
				}	
				pTFPayloadOut->PayloadDataBuf[16] = (pRxFrame->pSubframe2[0][8] << 1);					// Bit 7-6
				
				// Fill Payload Byte 16																				
				pTFPayloadOut->PayloadDataBuf[16] |= (pRxFrame->pSubframe3[0][0] << 1) & 0x3E;			// Bit 5-1
				pTFPayloadOut->PayloadDataBuf[16] |= (pRxFrame->pSubframe3[0][1] >> 7) & 0x01;			// Bit 0	
				for(i = 1; i <= 7; i++)
				{
				    // Fill Payload Byte 17-23
				    pTFPayloadOut->PayloadDataBuf[i+16] = (pRxFrame->pSubframe3[0][i] << 1);			// Bit 7-1
					pTFPayloadOut->PayloadDataBuf[i+16] |= (pRxFrame->pSubframe3[0][i+1] >> 7) & 0x01;	// Bit 0		 	    	
				}	
			}
			else
			{
				// Datarate = 5300kbps
				
				// Restore bytes 0-19 of the PayloadDataBuf from the subframes
				// Fill Payload Byte 0
				pTFPayloadOut->PayloadDataBuf[0] = BIT7 | BIT6;											// Bit 7-6	
				pTFPayloadOut->PayloadDataBuf[0] |= (pRxFrame->pSubframe1[0][0] << 1) & 0x3E;			// Bit 5-1
				pTFPayloadOut->PayloadDataBuf[0] |= (pRxFrame->pSubframe1[0][1] >> 7) & 0x01;			// Bit 0	
			
				// Fill Payload Byte 1-5
				for(i = 1; i <= 5; i++)
				{
				    pTFPayloadOut->PayloadDataBuf[i] = (pRxFrame->pSubframe1[0][i] << 1);				// Bit 7-1
					pTFPayloadOut->PayloadDataBuf[i] |= (pRxFrame->pSubframe1[0][i+1] >> 7) & 0x01;		// Bit 0		 	    	
				}
				
				// Fill Payload Byte 6
				pTFPayloadOut->PayloadDataBuf[6] = (pRxFrame->pSubframe1[0][6] << 1);					// Bit 7-1
				pTFPayloadOut->PayloadDataBuf[6] |= (pRxFrame->pSubframe2[0][0] >> 4) & 0x01;			// Bit 0
			
				// Fill Payload Byte 7-12		
				for(i = 1; i <= 6; i++)
				{
				    pTFPayloadOut->PayloadDataBuf[i+6] = (pRxFrame->pSubframe2[0][i-1] << 4);			// Bit 7-4
					pTFPayloadOut->PayloadDataBuf[i+6] |= (pRxFrame->pSubframe2[0][i] >> 4) & 0x0F;		// Bit 3-0	 	    	
				}

				// Fill Payload Byte 13
				pTFPayloadOut->PayloadDataBuf[13] = (pRxFrame->pSubframe2[0][6] << 4);					// Bit 7-4
				pTFPayloadOut->PayloadDataBuf[13] |= (pRxFrame->pSubframe3[0][0] >> 1) & 0x0F;			// Bit 3-0		
				
				// Fill Payload Byte 14-19
				for(i = 1; i <= 6; i++)		
				{
				    pTFPayloadOut->PayloadDataBuf[i+13] = (pRxFrame->pSubframe3[0][i-1] << 7);			// Bit 7	
					pTFPayloadOut->PayloadDataBuf[i+13] |= (pRxFrame->pSubframe3[0][i] >> 1) & 0x7F;	// Bit 6-0	 	    	
				}
			}    
			
			break;	
    
    
		case 2: // Fax 
    
			// Set PayloadDataType to voice
			pTFPayloadOut->PayloadDataType = PAYLOAD_FAX;	
		
			// Restore bytes 0-19 of the PayloadDataBuf from the subframes
			// Fill Payload Byte 0
			pTFPayloadOut->PayloadDataBuf[0] = BIT6;												// Bit 7-6	
			pTFPayloadOut->PayloadDataBuf[0] |= (pRxFrame->pSubframe1[0][0] << 1) & 0x3E;			// Bit 5-1
			pTFPayloadOut->PayloadDataBuf[0] |= (pRxFrame->pSubframe1[0][1] >> 7) & 0x01;			// Bit 0	
			
			// Fill Payload Byte 1-5
			for(i = 1; i <= 5; i++)
			{
			    pTFPayloadOut->PayloadDataBuf[i] = (pRxFrame->pSubframe1[0][i] << 1);				// Bit 7-1
				pTFPayloadOut->PayloadDataBuf[i] |= (pRxFrame->pSubframe1[0][i+1] >> 7) & 0x01;		// Bit 0		 	    	
			}
				
			// Fill Payload Byte 6
			pTFPayloadOut->PayloadDataBuf[6] = (pRxFrame->pSubframe1[0][6] << 1);					// Bit 7-1
			pTFPayloadOut->PayloadDataBuf[6] |= (pRxFrame->pSubframe2[0][0] >> 4) & 0x01;			// Bit 0
			
			// Fill Payload Byte 7-12		
			for(i = 1; i <= 6; i++)
			{
			    pTFPayloadOut->PayloadDataBuf[i+6] = (pRxFrame->pSubframe2[0][i-1] << 4);			// Bit 7-4
				pTFPayloadOut->PayloadDataBuf[i+6] |= (pRxFrame->pSubframe2[0][i] >> 4) & 0x0F;		// Bit 3-0	 	    	
			}

			// Fill Payload Byte 13
			pTFPayloadOut->PayloadDataBuf[13] = (pRxFrame->pSubframe2[0][6] << 4);					// Bit 7-4
			pTFPayloadOut->PayloadDataBuf[13] |= (pRxFrame->pSubframe3[0][0] >> 1) & 0x0F;			// Bit 3-0		
				
			// Fill Payload Byte 14-19
			for(i = 1; i <= 6; i++)		
			{
			    pTFPayloadOut->PayloadDataBuf[i+13] = (pRxFrame->pSubframe3[0][i-1] << 7);			// Bit 7	
				pTFPayloadOut->PayloadDataBuf[i+13] |= (pRxFrame->pSubframe3[0][i] >> 1) & 0x7F;	// Bit 6-0	 	    	
			}
		
			break;	
			

    	case 3: // Signalling
    		    		 
#ifdef DEBUG_TELEPHONY
			binInOut_toggleOutPin(e_Buf_EW4, TOGGLE_OUT_PIN_INSTANCE_4);
#endif
			
			// Set mWireDataReceived flag    		  
    		pMWireDataOut->mWireDataReceived = true;

    		// Restore the signallingFrameType from the type section with a majority rule
    		// Bit1 of the signallingFrameType
    		numberOfOnes = 0;
    		
   			if(pRxFrame->pSubframe1[0][0] & BIT4)
			{
	    		numberOfOnes++;
			}	
		   	if(pRxFrame->pSubframe2[0][0] & BIT4)
			{
	    		numberOfOnes++;
			}	  
		   	if(pRxFrame->pSubframe3[0][0] & BIT4)
			{
	    		numberOfOnes++;
			}  
		   	if(pRxFrame->pSubframe1[0][0] & BIT2)
			{
	    		numberOfOnes++;
			}		 	
		   	if(pRxFrame->pSubframe2[0][0] & BIT2)
			{
	    		numberOfOnes++;
			}
			
			if(numberOfOnes >= 3)
			{
			    signallingFrameType = BIT1;
			}
			else
			{
			    signallingFrameType = 0;
			}
			
			// Bit0 of the signallingFrameType
			numberOfOnes = 0;
			
   			if(pRxFrame->pSubframe1[0][0] & BIT3)
			{
	    		numberOfOnes++;
			}
		   	if(pRxFrame->pSubframe2[0][0] & BIT3)
			{
	    		numberOfOnes++;
			}
		   	if(pRxFrame->pSubframe3[0][0] & BIT3)
			{
	    		numberOfOnes++;
			}
		   	if(pRxFrame->pSubframe1[0][0] & BIT1)
			{
	    		numberOfOnes++;
			}
		 	if(pRxFrame->pSubframe2[0][0] & BIT1)
			{
	    		numberOfOnes++;
			}
		 	
			if(numberOfOnes >= 3)
			{
			    signallingFrameType |= BIT0;
			}
			else
			{
			    signallingFrameType &= ~BIT0;
			}
					
			// Decode the M-Wire codewords and fill the M-Wire sample buffer
			codeword[0] = (pRxFrame->pSubframe1[0][0] << 11) & 0xFFF;
			codeword[0] |= (pRxFrame->pSubframe1[0][1] << 3) & 0x7F8;
			codeword[0] |= (pRxFrame->pSubframe1[0][2] >> 5) & 0x007; 
				
			codeword[1] = (pRxFrame->pSubframe2[0][0] << 11) & 0xFFF;
			codeword[1] |= (pRxFrame->pSubframe2[0][1] << 3) & 0x7F8;
			codeword[1] |= (pRxFrame->pSubframe2[0][2] >> 5) & 0x007; 
			
			codeword[2] = (pRxFrame->pSubframe3[0][0] << 11) & 0xFFF;
			codeword[2] |= (pRxFrame->pSubframe3[0][1] << 3) & 0x7F8;
			codeword[2] |= (pRxFrame->pSubframe3[0][2] >> 5) & 0x007; 
			
			pMWireDataOut->freeze = mWireDecode(pMWireDataOut->mWireBuffer, &codeword[0]);
			
			// If service phone is enabled read the service phone M-Wire
			if(pMWireDataOut->servicePhoneOn)
			{
			    // Check the M-Wire state with a majority rule
			    numberOfOnes = 0;
   				   				
			    if(pRxFrame->pSubframe1[0][2] & BIT4)
				{
		    		numberOfOnes++;
				}
			 	if(pRxFrame->pSubframe2[0][2] & BIT4)
				{
		    		numberOfOnes++;
				}
			 	if(pRxFrame->pSubframe3[0][2] & BIT4)
				{
		    		numberOfOnes++;
				}
			 	if(pRxFrame->pSubframe1[0][2] & BIT3)
				{
		    		numberOfOnes++;
				}
			 	if(pRxFrame->pSubframe2[0][2] & BIT3)
				{
		    		numberOfOnes++;
				}
				
				if(numberOfOnes >= 3)
				{
					pMWireDataOut->mWireBufferServicePhone = 1;
				}
				else
				{
				    pMWireDataOut->mWireBufferServicePhone = 0;
				}
			} // End if(pMWireDataOut->servicePhoneOn)
			
			
			// Restore the PayloadDataBuf depending on the signallingFrameType
			switch (signallingFrameType)
			{		
				case SIGNALLING_TYPE_EMPTY: 
					// Set PayloadDataType to NODATA
					pTFPayloadOut->PayloadDataType = PAYLOAD_NODATA;
					pTFPayloadOut->PayloadDataBuf[0] = 0;
			
					break;
					
			
				case SIGNALLING_TYPE_DTMF: 
					// Set PayloadDataType to DTMF
					pTFPayloadOut->PayloadDataType = PAYLOAD_DTMF;
					pTFPayloadOut->PayloadDataBuf[0] = BIT0;
					pTFPayloadOut->PayloadDataBuf[1] = 0;
					
					// Restore byte 1 of the PayloadDataBuf from the data section of the subframes
					// Save 5 Payloads in a local buffer
					signallingFramePayload[0] = (pRxFrame->pSubframe1[0][2] << 5) & 0xFF; 
					signallingFramePayload[0] |= (pRxFrame->pSubframe1[0][3] >> 3) & 0x1F; 
					signallingFramePayload[1] = (pRxFrame->pSubframe2[0][2] << 5) & 0xFF; 
					signallingFramePayload[1] |= (pRxFrame->pSubframe2[0][3] >> 3) & 0x1F; 
					signallingFramePayload[2] = (pRxFrame->pSubframe3[0][2] << 5) & 0xFF; 
					signallingFramePayload[2] |= (pRxFrame->pSubframe3[0][3] >> 3) & 0x1F; 
					signallingFramePayload[3] = (pRxFrame->pSubframe1[0][4] << 5) & 0xFF; 
					signallingFramePayload[3] |= (pRxFrame->pSubframe1[0][5] >> 3) & 0x1F; 
					signallingFramePayload[4] = (pRxFrame->pSubframe2[0][4] << 5) & 0xFF; 
					signallingFramePayload[4] |= (pRxFrame->pSubframe2[0][5] >> 3) & 0x1F; 

					// Check the majority of every bit and store it to the PayloadDataBuf
					for(i = 0; i <= 7; i++)		 
					{
					    numberOfOnes = 0;
					    
						for(j = 0; j <= 4; j++)
						{
						    if(signallingFramePayload[j] & (1 << i))	
							{
								numberOfOnes++;
							}
						}	
						
						if(numberOfOnes >= 3)
						{
							pTFPayloadOut->PayloadDataBuf[1] |= (1 << i);
						}
						else
						{
						    // Payload is already 0
						}   
					}
			
					break;	
				
					
				case SIGNALLING_TYPE_CPTONE: 
					
					pTFPayloadOut->PayloadDataType = PAYLOAD_CPTONE;
					pTFPayloadOut->PayloadDataBuf[0] = BIT1;
					pTFPayloadOut->PayloadDataBuf[1] = 0;
					pTFPayloadOut->PayloadDataBuf[2] = 0;
					
					// Restore bytes 1-2 of the PayloadDataBuf from the data section of the subframes
					// Save 5 Payloads in a local buffer
					signallingFramePayload[0] = pRxFrame->pSubframe1[0][2] << 13; 
					signallingFramePayload[0] |= (pRxFrame->pSubframe1[0][3] << 5) & 0x1FE0; 
					signallingFramePayload[0] |= (pRxFrame->pSubframe1[0][4] >> 3) & 0x001F; 
					signallingFramePayload[1] = pRxFrame->pSubframe2[0][2] << 13; 
					signallingFramePayload[1] |= (pRxFrame->pSubframe2[0][3] << 5) & 0x1FE0; 
					signallingFramePayload[1] |= (pRxFrame->pSubframe2[0][4] >> 3) & 0x001F;
					signallingFramePayload[2] = pRxFrame->pSubframe3[0][2] << 13; 
					signallingFramePayload[2] |= (pRxFrame->pSubframe3[0][3] << 5) & 0x1FE0; 
					signallingFramePayload[2] |= (pRxFrame->pSubframe3[0][4] >> 3) & 0x001F;
					signallingFramePayload[3] = pRxFrame->pSubframe1[0][4] << 13;   
					signallingFramePayload[3] |= (pRxFrame->pSubframe1[0][5] << 5) & 0x1FE0;
					signallingFramePayload[3] |= (pRxFrame->pSubframe1[0][6] >> 3) & 0x001F;
					signallingFramePayload[4] = pRxFrame->pSubframe2[0][4] << 13;   
					signallingFramePayload[4] |= (pRxFrame->pSubframe2[0][5] << 5) & 0x1FE0;
					signallingFramePayload[4] |= (pRxFrame->pSubframe2[0][6] >> 3) & 0x001F;
					
					// Check the majority of every bit and store it to the PayloadDataBuf
					for(i = 0; i <= 15; i++)		 
					{
					    numberOfOnes = 0;
						for(j = 0; j <= 4; j++)
						{
						    if(signallingFramePayload[j] & (1 << i))	
							{
								numberOfOnes++;
							}
						}
							
						if(numberOfOnes >= 3)
						{
						    if(i <= 7)
						    {
								pTFPayloadOut->PayloadDataBuf[2] |= (1 << i);
						    }
						    else
						    {
						      	pTFPayloadOut->PayloadDataBuf[1] |= (1 << (i-8));  
						    }
						}
						else
						{
						    // Payload is already 0
						}   
					}
			
					break;	
				

				case SIGNALLING_TYPE_EANDM: 
					
					// Restore the suppressed frame type from the data section with a majority rule
					numberOfOnes = 0;

					if(pRxFrame->pSubframe1[0][2] & BIT2)
					{
			    		numberOfOnes++;
					}
				 	if(pRxFrame->pSubframe2[0][2] & BIT2)
					{
			    		numberOfOnes++;
					}
				 	if(pRxFrame->pSubframe3[0][2] & BIT2)
					{
			    		numberOfOnes++;
					}
				 	if(pRxFrame->pSubframe1[0][4] & BIT2)
					{
			    		numberOfOnes++;
					}
				 	if(pRxFrame->pSubframe2[0][4] & BIT2)
					{
			    		numberOfOnes++;
					}
				 	
					if(numberOfOnes >= 3)
					{
					    // Suppressed frame was a voice frame
						pTFPayloadOut->PayloadDataType = PAYLOAD_BADVOICE;
						for(i = 0; i <= 23; i++) 
					    {
					      	pTFPayloadOut->PayloadDataBuf[i] = 0;  
					    }
					    pTFPayloadOut->PayloadDataBuf[0] |= BIT7;
					    
					    if(g_datarate == CH_VOICE_DR_5300)
					    {
					     	pTFPayloadOut->PayloadDataBuf[0] |= BIT6;	   
					    }
					    
					}
					else
					{
					    // Suppressed frame was a fax frame
					    pTFPayloadOut->PayloadDataType = PAYLOAD_FAX;
					    for(i = 0; i <= 19; i++) 
					    {
					      	pTFPayloadOut->PayloadDataBuf[i] = 0;  
					    }
					    pTFPayloadOut->PayloadDataBuf[0] |= BIT6;
					}   
					    
					break;			
					
				default: 	
						
					break;
					
			} // End switch (signallingFrameType)		
					
			break;	
				
		default: 	
						
			break;		
    
	} // End switch (pRxFrame->iMin)
	
} // End subframe_unpack


/***************************************************************************
  Function:		mWireEncode()		
  Description:	Creates 3 codewords out of 32 M-Wire samples. The 32 samples
  				describe a time interval of 30ms/32=0.9375ms, however the 
  				codewords describe a time interval of 10ms/8=1.25ms. There 
  				are 16 different codewords 2(pos and neg egde)*8(number of 
  				intervals). A codeword has 12 bit, so the minimum Hamming
  				distance is 6. 
  Inputs:		pMWireBuffer: Pointer to 32 M-Wire samples 		
  Outputs:		pCodeword: Pointer to 3 codewords, which each defines the time
  				of a change between 10ms and the edge. 
  Return:		-
  Globals:		-		
  Important: 	
                                          	
****************************************************************************/
void mWireEncode(UINT8 *pMWireBuffer , UINT16 *pCodeword)
{
  	UINT32 i = 0;  
    
  	// First 10ms
  	pCodeword[0] = 0xFFFF;
  	for(i = 1; i <= 10;i++)
  	{
  	    // Check if m-wire changes in the bytes 0-10 of the MWireBuffer
  	 	if(pMWireBuffer[i] != pMWireBuffer[i-1])
  	 	{   
  	 	    // Check if m-wire changed to 1 or 0
  	 	    if(pMWireBuffer[i] & 0x01)
  	 	    {	
  	 	    	pCodeword[0] = g_eAndMCodewordTable[g_eAndMEncodeTable[i]+8];		// M-wire changed to 1
  	 	    }
  	 	    else
  	 	    {
  	 	     	pCodeword[0] = g_eAndMCodewordTable[g_eAndMEncodeTable[i]]; 		// M-wire changed to 0  
  	 	    }
  	 	}
  	}
  	// Check if M-wire has not changed 
  	if(pCodeword[0] == 0xFFFF)
  	{
  	 	// Check if m-wire is 1 or 0	    
   		if(pMWireBuffer[0] & 0x01)
  	 	{	
  	     	pCodeword[0] = g_eAndMCodewordTable[8];			// M-wire is 1
  	    }
  	    else
  	    {
  	     	pCodeword[0] = g_eAndMCodewordTable[0];  		// M-wire is 0 
   	    }  	 	   
  	}
  	
  	
  	// Second 10ms
  	pCodeword[1] = 0xFFFF;
  	for(i = 12; i <= 20;i++)
  	{
  	    // Check if m-wire changes in the bytes 11-20 of the MWireBuffer
  	 	if(pMWireBuffer[i] != pMWireBuffer[i-1])
  	 	{   
  	 	    // Check if m-wire changed to 1 or 0
  	 	    if(pMWireBuffer[i] & 0x01)
  	 	    {	
  	 	    	pCodeword[1] = g_eAndMCodewordTable[g_eAndMEncodeTable[i]+8];		// M-wire changed to 1
  	 	    }
  	 	    else
  	 	    {
  	 	     	pCodeword[1] = g_eAndMCodewordTable[g_eAndMEncodeTable[i]]; 		// M-wire changed to 0  
  	 	    }
  	 	}
  	}
  	// Check if M-wire has not changed 
  	if(pCodeword[1] == 0xFFFF)
  	{
  	 	// Check if m-wire is 1 or 0	    
   		if(pMWireBuffer[11] & 0x01)
  	 	{	
  	     	pCodeword[1] = g_eAndMCodewordTable[8];			// M-wire is 1
  	    }
  	    else
  	    {
  	     	pCodeword[1] = g_eAndMCodewordTable[0];  		// M-wire is 0 
   	    }  	 	   
  	}
  	
  	
  	
  	// Third 10ms
  	pCodeword[2] = 0xFFFF;
  	for(i = 22; i <= 31;i++)
  	{
  	    // Check if m-wire changes in the bytes 21-31 of the MWireBuffer
  	 	if(pMWireBuffer[i] != pMWireBuffer[i-1])
  	 	{   
  	 	    // Check if m-wire changed to 1 or 0
  	 	    if(pMWireBuffer[i] & 0x01)
  	 	    {	
  	 	    	pCodeword[2] = g_eAndMCodewordTable[g_eAndMEncodeTable[i]+8];		// M-wire changed to 1
  	 	    }
  	 	    else
  	 	    {
  	 	     	pCodeword[2] = g_eAndMCodewordTable[g_eAndMEncodeTable[i]]; 		// M-wire changed to 0  
  	 	    }
  	 	}
  	}
  	// Check if M-wire has not changed 
  	if(pCodeword[2] == 0xFFFF)
  	{
  	 	// Check if m-wire is 1 or 0	    
   		if(pMWireBuffer[21] & 0x01)
  	 	{	
  	     	pCodeword[2] = g_eAndMCodewordTable[8];			// M-wire is 1
  	    }
  	    else
  	    {
  	     	pCodeword[2] = g_eAndMCodewordTable[0];  		// M-wire is 0 
   	    }  	 	   
  	}
}


/***************************************************************************
  Function:		mWireDecode()		
  Description:	Creates 32 M-Wire samples out of 3 codewords. The 32 samples
  				describe a time interval of 30ms/32=0.9375ms, however the 
  				codewords describe a time interval of 10ms/8=1.25ms. So some
  				intervals can have a different value than the original value. 
  				If the smallest Hamming distance is >= 3 the value of the 
  				M-wire will be freezed. As this function does not know the 
  				value before the first codeword it returns a value between 0-3, 
  				which means freeze for 0ms-30ms.
  Inputs:		pCodeword: Pointer to 3 codewords, which each defines the time
  				of a change between 10ms and the edge 
  Outputs:		pMWireBuffer: Pointer to 32 M-Wire samples 		
  Return:		freeze: Shows how long the M-Wire state must be freezed in
  				10ms intervals, because the Hamming distance of the 
  				codewords was >=3. 
  Globals:		-		
  Important: 	
                                          	
****************************************************************************/
UINT16 mWireDecode(UINT8 *pMWireBuffer , UINT16 *pCodeWord)
{
    UINT16 hDist;
    UINT16 hDistMin;
    UINT16 toggleSlot8;			// Shows in which one of the 8 slots the value has changed
    UINT16 toggleSlot32;		// Shows in which one of the 32 slots the value has changed
    
    UINT16 freeze = 0x00;
    
    UINT32 i;
    
    // First 10ms
    
    // Search codeword with the smallest hamming distance to codeword 0
    hDistMin = 0xf;
	for(i = 0;i <= 15;i++)
	{
	    // The asm function must be volatile otherwise it will not work correctly after the optimization
    	asm volatile( "    	\
      	%1 = %1 ^ %2; 		\
      	%0 = ones %1;   " : "=l" (hDist) : "d" (pCodeWord[0]) , "d" (g_eAndMCodewordTable[i]) );
		
      	if(hDist <= hDistMin)
		{
	    	hDistMin = hDist;
	    	toggleSlot8 = i;
		}
	}
	
	// If the min Hamming distance is <= 3 freeze the M-Wire 
	if(hDistMin >= 3)
	{
	    // freeze first 10ms
	 	freeze++;			   
	}
	else
	{
	 	// If M-Wire changed from 1 to 0
	    if(toggleSlot8 <= 7)
	 	{   
	 	    // Restore the mWireBuffer 
	    	toggleSlot32 = g_eAndMDecodeTable0[toggleSlot8];
	    	for(i = toggleSlot32;i <= 10;i++)
			{
	    		pMWireBuffer[i] = 0;			    
			}
			if(toggleSlot32 != 0)
			{
				for(i = 0;i <= toggleSlot32 - 1;i++)
				{
		    		pMWireBuffer[i] = 1;			    
				}   
			}
	 	}
	 	// If M-Wire changed from 0 to 1
	 	else
	 	{ 
	 	    // Restore the mWireBuffer 
	 	 	toggleSlot32 = g_eAndMDecodeTable0[toggleSlot8 - 8];
	    	for(i = toggleSlot32;i <= 10;i++)
			{
	    		pMWireBuffer[i] = 1;			    
			}
			if(toggleSlot32 != 0)	
			{
				for(i = 0;i <= toggleSlot32 - 1;i++)
				{
		    		pMWireBuffer[i] = 0;			    
				}   
			}
	 	}
	} // End else of if(hDistMin >= 3)
    
	
	
	// Second 10ms
    
    // Search codeword with the smallest hamming distance to codeword 1
    hDistMin = 0xf;
	for(i = 0;i <= 15;i++)
	{
	    // The asm function must be volatile otherwise it will not work correctly after the optimization
    	asm volatile( "    	\
      	%1 = %1 ^ %2; 		\
      	%0 = ones %1;   " : "=l" (hDist) : "d" (pCodeWord[1]) , "d" (g_eAndMCodewordTable[i]) );
		
      	if(hDist <= hDistMin)
		{
	    	hDistMin = hDist;
	    	toggleSlot8 = i;
		}
	}
	
	// If the min Hamming distance is <= 3 freeze the M-Wire
	if(hDistMin >= 3)
	{	
	    // If first 10ms were freezed
	    if(freeze !=0)
	    {
	        // freeze second 10ms
	     	freeze++;			   
	    }
	    else
	    {
	   		// Fill the mWireBuffer with the last value from the first 10ms 
	 		for(i = 11;i <= 20;i++)
			{
	    		pMWireBuffer[i] = pMWireBuffer[10];			    
			}
	    }	   
	}
	else
	{
	 	// If M-Wire changed from 1 to 0
	    if(toggleSlot8 <= 7)
	 	{   
	 	    // Restore the mWireBuffer 
	    	toggleSlot32 = g_eAndMDecodeTable1[toggleSlot8];
	    	for(i = toggleSlot32;i <= 20;i++)
			{
	    		pMWireBuffer[i] = 0;			    
			}
			if(toggleSlot32 != 0)	
			{
				for(i = 11;i <= toggleSlot32 - 1;i++)
				{
			    	pMWireBuffer[i] = 1;			    
				}
			}
	 	}
	 	// If M-Wire changed from 0 to 1
	 	else
	 	{ 
	 	    // Restore the mWireBuffer 
	 	 	toggleSlot32 = g_eAndMDecodeTable1[toggleSlot8 - 8];
	    	for(i = toggleSlot32;i <= 20;i++)
			{
	    		pMWireBuffer[i] = 1;			    
			}
			if(toggleSlot32 != 0)	
			{
				for(i = 11;i <= toggleSlot32 - 1;i++)
				{
			    	pMWireBuffer[i] = 0;			    
				}
			}
	 	}
	} // End else of if(hDistMin >= 3)
    
    
	// Third 10ms
    
    // Search codeword with the smallest hamming distance to codeword 3
    hDistMin = 0xf;
	for(i = 0;i <= 15;i++)
	{
	    // The asm function must be volatile otherwise it will not work correctly after the optimization
    	asm volatile( "   	\
      	%1 = %1 ^ %2; 		\
      	%0 = ones %1;   " : "=l" (hDist) : "d" (pCodeWord[2]) , "d" (g_eAndMCodewordTable[i]) );
		
      	if(hDist <= hDistMin)
		{
	    	hDistMin = hDist;
	    	toggleSlot8 = i;
		}
	}
	
	// If the min Hamming distance is <= 3 freeze the M-Wire
	if(hDistMin >= 3)
	{
	    // If first and second 10ms were freezed
	    if(freeze >= 2)
	    {
	        // freeze third 10ms
	     	freeze++;			   
	    }
	    else
	    {
	   		// Fill the mWireBuffer with the last value from the second 10ms 
	 		for(i = 21;i <= 31;i++)
			{
	    		pMWireBuffer[i] = pMWireBuffer[20];			    
			}	
	    }
	}
	else
	{
	 	// If M-Wire changed from 1 to 0
	    if(toggleSlot8 <= 7)
	 	{   
	 	    // Restore the mWireBuffer 
	    	toggleSlot32 = g_eAndMDecodeTable2[toggleSlot8];
	    	pMWireBuffer[21] = pMWireBuffer[20];	// M-Wire does not change between 20 and 21
	    	for(i = toggleSlot32;i <= 31;i++)
			{
	    		pMWireBuffer[i] = 0;			    
			}
			if(toggleSlot32 != 0)	
			{
				for(i = 22;i <= toggleSlot32 - 1;i++)
				{
			    	pMWireBuffer[i] = 1;			    
				}
			}
	 	}
	 	// If M-Wire changed from 0 to 1
	 	else
	 	{ 
	 	    // Restore the mWireBuffer 
	 	 	toggleSlot32 = g_eAndMDecodeTable2[toggleSlot8 - 8];
	 	 	pMWireBuffer[21] = pMWireBuffer[20];	// M-Wire does not change between 20 and 21
	    	for(i = toggleSlot32;i <= 31;i++)
			{
	    		pMWireBuffer[i] = 1;			    
			}
			if(toggleSlot32 != 0)	
			{
				for(i = 22;i <= toggleSlot32 - 1;i++)
				{
			    	pMWireBuffer[i] = 0;			    
				}   
			}
	 	}
	} // End else of if(hDistMin >= 3)
    
 	return(freeze);   
}
