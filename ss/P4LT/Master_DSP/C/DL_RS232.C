/*-------------------------------------------------------------------------*
 * $Workfile: DL_RS232.C $
 * Part of	: ETl500 R2/3
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/DL_RS232.C $
 * 
 * 2     7.09.05 10:02 Chalgyg
 * Bug #186 removed.
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 17    14.09.04 16:50 Buhl01
 * #include "SPI_functions.h" added
 * 
 * 16    29.07.04 15:17 Leeb02
 * 
 * 15    29.07.04 14:58 Leeb02
 * 
 * 14    29.07.04 11:48 Buhl01
 * testBuffer removed to enable compilation without errors
 * 
 * 13    29.07.04 10:05 Leeb02
 * 
 * 12    29.07.04 6:42 Leeb02
 * 
 * 11    29.07.04 4:51 Leeb02
 * 
 * 10    26.07.04 8:50 Leeb02
 * 
 * 9     22.07.04 15:14 Leeb02
 * 
 * 8     22.07.04 10:24 Buhl01
 * remove comment out of  uart_getChar(), uart_txQueueEmpty(),
 * uart_putChar()
 * 
 * 7     21.07.04 10:53 Leeb02
 * warning removed
 * 
 * 6     20.07.04 13:07 Leeb02
 * 
 * 5     4.05.04 13:33 Leeb02
 * unsigned long -> unsigned char for uart characters
 * 
 * 4     3.05.04 7:33 Leeb02
 * Now ready to communicate with UART
 * 
 * 3     28.04.04 19:13 Leeb02
 * pre-cleaned by CL for Buh
 * 
 * 2     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 14:06 Leeb02
 *-------------------------------------------------------------------------*/
/*-----------------------GLOBAL CONSTANTS----------------------------------*/

/* INCLUDE FILES **************************************************************/

#include "DL_RS232.H"

#include "constant.h"
#include "ft12.h"
#include "SPI_functions.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define FIRST_BYTE						0
#define SECOND_BYTE						1
#define THIRD_BYTE						2
#define FOURTH_BYTE						3

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static t_FT12TelegramBuffer g_receiveBuffer;     // the receive buffer is owned by this module
static t_FT12TelegramBuffer **g_pTransmitBuffer; // a pointer to the pointer to the transmit buffer

static e_FT12State *g_pState; // a pointer to the state

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/

//*************************************************************
//
//  Member Function:
//     long init_uart(long baudrate_9600);
//
//  Purpose:
//      Initialize the UART 
//
//  Parameters: 
//		
//
//	Return:     nothing
//
//	Notes:
//		
//
//
//*************************************************************
BOOL SEGMENT_EXT_CODE init_DL_RS232(   unsigned long           baudrate,       // in 
									   t_FT12TelegramBuffer ** receiveBuffer,  // out
									   t_FT12TelegramBuffer ** transmitBuffer, // in
									   e_FT12State *           pState)         // in
{

	g_pState = pState; // save pointer to state

	*receiveBuffer    = &g_receiveBuffer; // return pointer to receive buffer
	g_pTransmitBuffer = transmitBuffer;   // save pointer which points to the current 
	                                      // transmit buffer
	return uart_init(baudrate);
}



/*
*  schreibt nur in den Empfangsbuffer( g_receiveBuffer), falls das g_receiveBuffer.Flag nicht gesetzt ist.
*	( muss noch implementiert werden )
*  Achtung die Anzahl User Data Bytes müssen durch 4 Teilbar sein, sonst wird der 
*  Prüfsummen- und END_CHAR Test nicht richtig ausgeführt.
*
*/  

void SEGMENT_EXT_CODE FromDataLinkLayerRS232(void)
{
    static BOOL m_MessageStartDetected = FALSE;

	static long *m_ReceiveLongPtr;
	static unsigned long  m_RxByteCounter;
	static unsigned long  m_RxChecksum;

	static unsigned long m_IdleIntervalOnRxLineCtr;
	static unsigned long m_CharCounter, m_NumberOfRecChar, m_NumberOfUserData;

	unsigned char        receivedCharacter;

	if(uart_getChar(&receivedCharacter) && (g_receiveBuffer.State == e_FT12State_Empty))
	{
		m_IdleIntervalOnRxLineCtr = 0; // reset idle time between two successive bytes

		if(!m_MessageStartDetected) 
	    {
			static unsigned long start_message;

			// detect start m_Message >>| start | L | L | start |<< 
		    // start == FT12_START_CHAR
		    // length field, range: 0...255, this value is transmitted twice for higher security

			start_message = (start_message << SB8) | receivedCharacter;

		    if((start_message & MASK_MESSAGE_START) == MESSAGE_START)
			{
				m_NumberOfUserData   = (start_message >> SB8) & MASK_8LSB;  // number of user data characters 

	            if(((start_message >> SB16) & MASK_8LSB) != m_NumberOfUserData)	
				{
					m_MessageStartDetected = FALSE; // compare the two L characters
				}
				else  
				{
					m_MessageStartDetected = TRUE;
					m_NumberOfRecChar = m_NumberOfUserData + CRC_AND_END_CHAR;// add CRC and End character.

					m_CharCounter = 0;
					m_RxChecksum = 0;
					m_RxByteCounter = FALSE;
					// NEU: Kopiere 0|0|S|L in 1. Word und L|S in 2. Word MSB des Empfangsbuffers 
					// 0 = LSB , L= MSB
					// Programcode....

					m_ReceiveLongPtr    = g_receiveBuffer.TelegrammDataArr;	// 1. Word
					*m_ReceiveLongPtr++ = ((FT12_START_CHAR << SB8) | (m_NumberOfUserData << SB16)| (m_NumberOfUserData << SB24)) & 0xffffff00;							// 0S00 			
					*m_ReceiveLongPtr   = FT12_START_CHAR;	// 2. Word -> 1 Byte
				}	
			}
		}
		else
		{
			// >>| start | L | L | start |<< characters detected
			if(++m_CharCounter <= m_NumberOfRecChar) 
			{
				if(m_CharCounter <= m_NumberOfUserData) 
				{
					m_RxChecksum += receivedCharacter;
				}
			
				if(m_CharCounter <= LENGTH_HEADER) 
				{
					// Scan header
					switch (m_CharCounter) 
					{				
						case CONTROL_FIELD:
									*m_ReceiveLongPtr |=  receivedCharacter << SB8;
									g_receiveBuffer.ControlField=receivedCharacter;
									break;

						case DEST_SUB_ADR:		
									*m_ReceiveLongPtr |=  receivedCharacter << SB16; 
									g_receiveBuffer.DestinationSubadress=receivedCharacter;
									break;

						case MESS_TYP_LSB:
									*m_ReceiveLongPtr |=  receivedCharacter << SB24;
									g_receiveBuffer.MessageType=receivedCharacter;
									m_ReceiveLongPtr++;								// Pointer auf 3. Word
									break;

						case MESS_TYP_MSB:
									*m_ReceiveLongPtr =   0x00000000;
									*m_ReceiveLongPtr =  receivedCharacter;
									g_receiveBuffer.MessageType |= receivedCharacter << SB8;
									break;

						case DEST_ADR_LSB:
									*m_ReceiveLongPtr |=  receivedCharacter << SB8;
									g_receiveBuffer.DestinationAdress = receivedCharacter;
									break;

						case DEST_ADR_MSB:
									*m_ReceiveLongPtr |=  receivedCharacter << SB16;
									g_receiveBuffer.DestinationAdress |= receivedCharacter << SB8;							
									break;

						case SOUR_ADR_LSB:
									*m_ReceiveLongPtr |=  receivedCharacter << SB24;
									g_receiveBuffer.SourceAdress = receivedCharacter;
									m_ReceiveLongPtr++;			// Pointer auf 4. Word
									break;

						case SOUR_ADR_MSB:
									*m_ReceiveLongPtr =   0x00000000;
									*m_ReceiveLongPtr =  receivedCharacter;
									g_receiveBuffer.SourceAdress|=receivedCharacter << SB8;
									break;

						case HOP_COUNTER:
									*m_ReceiveLongPtr |=  receivedCharacter << SB8;
									g_receiveBuffer.HopCounter = receivedCharacter;
									break;
									
						case SEQUENCE_NR_LSB:								
									*m_ReceiveLongPtr |=  receivedCharacter << SB16;
									g_receiveBuffer.SequenceNumber = receivedCharacter;
									break;

						case SEQUENCE_NR_MSB:
									*m_ReceiveLongPtr |=  receivedCharacter << SB24;
									g_receiveBuffer.SequenceNumber |=receivedCharacter << SB8;
									m_ReceiveLongPtr++;			// Pointer auf 5. Word
									break;

						default:    m_ReceiveLongPtr   = g_receiveBuffer.TelegrammDataArr;
									*m_ReceiveLongPtr  = 0x0068FFFF;
									break;
					}	// end switch
				} // if  length
				else 
				{   // Scan Application User Data,  32Bit (1 Word), and Checksum and End Char
		      		switch (m_RxByteCounter)
		      		{
		      			 case FIRST_BYTE:
								*m_ReceiveLongPtr  =  0x00000000;			// Reset buffer
		      	 				*m_ReceiveLongPtr  =  receivedCharacter;
		      	 				m_RxByteCounter++;
		      	 				break;
		      			 case SECOND_BYTE:
		      	 				*m_ReceiveLongPtr |=  receivedCharacter << SB8;
		      	 				m_RxByteCounter++;
		      	 				break;
		      			 case THIRD_BYTE:
		      	 				*m_ReceiveLongPtr |=  receivedCharacter << SB16;
		      	 				m_RxByteCounter++;
		      	 				break;
		      			 case FOURTH_BYTE:
		      	 				*m_ReceiveLongPtr |=  receivedCharacter << SB24;
		      	 				m_RxByteCounter = 0;
	          					m_ReceiveLongPtr++;
		      	 				break;
						 default:
		      	 				*m_ReceiveLongPtr = 0;
		      	 				m_RxByteCounter = 0;
	          				
					} // switch

				} // else  length
			
			} // if (m_CharCounter <= m_NumberOfRecChar)
		
		    if(m_CharCounter == m_NumberOfRecChar)				
			{ 	// all characters received
				//Is the calculated CRC Sum equal to the received value in the RxFrame
				if((*m_ReceiveLongPtr & MASK_8LSB) != (m_RxChecksum & MASK_8LSB))	
				{
					return; 
				}		
				//	3.	Check the FT12_END_CHAR
				if((((*m_ReceiveLongPtr)>>SB8) & MASK_8LSB) != FT12_END_CHAR)	
				{ 
					return;
				}

				m_MessageStartDetected = FALSE;
				g_receiveBuffer.State = e_FT12State_Full; // Frame ohne Fehler empfangen
			} // end Block alle Oktette empfangen
		} // if(!m_MessageStartDetected)  
	}
	else
	{
		// detect idle interval (if no characters receive)
		if(++m_IdleIntervalOnRxLineCtr > IDLE_TIME_ON_RX_LINE)
		{
			 m_MessageStartDetected = FALSE;
			 m_IdleIntervalOnRxLineCtr = 0;
		}
	}
}



void SEGMENT_EXT_CODE ToDataLinkLayerRS232(void)
{       
	static long  m_TxByteCounter;
 	static long  m_NrOfCharToBeTransmitted;
	static long *m_TransmitCharacterPtr;
	
	if( e_FT12State_Forwarded == *g_pState) // Packet ist bereit zum SENDEN
	{ 
		(*g_pTransmitBuffer)->State = e_FT12State_Sending;

		*g_pState = e_FT12State_Sending;
		m_NrOfCharToBeTransmitted = (((*g_pTransmitBuffer)->TelegrammDataArr[0]>>SB16) & MASK_8LSB)+ FRAME_OVERHEAD; // L +2

		m_TransmitCharacterPtr = (*g_pTransmitBuffer)->TelegrammDataArr;
		m_TxByteCounter = SECOND_BYTE; // ab dem zweiten Byte senden
		*m_TransmitCharacterPtr = *m_TransmitCharacterPtr & 0xffffff00;		// if there is someting wrong in the buffer
	}

    if(m_NrOfCharToBeTransmitted > 0)
	{
		//test if TxQueue (Uart) is empty and waiting for a character.
		if(uart_txQueueEmpty())
	    {               
			register unsigned long charToBeTransmitted;

	      	m_NrOfCharToBeTransmitted--;
			
	      	switch (m_TxByteCounter)
	      	{
				case FIRST_BYTE:
					charToBeTransmitted = *m_TransmitCharacterPtr;
					m_TxByteCounter++;
					break;
				case SECOND_BYTE:
					charToBeTransmitted = *m_TransmitCharacterPtr >> SB8;
					m_TxByteCounter++;
					break;
				case THIRD_BYTE:
					charToBeTransmitted = *m_TransmitCharacterPtr >> SB16;
					m_TxByteCounter++;
					break;
				case FOURTH_BYTE:
					charToBeTransmitted = *m_TransmitCharacterPtr >> SB24;
					m_TxByteCounter = FIRST_BYTE;
					m_TransmitCharacterPtr++;
					break;
			}

//			testBuffer[testBufferPtr++] = charToBeTransmitted & MASK_8LSB;
//			if (testBufferPtr >= BUFSIZ) testBufferPtr = 0;

			uart_putChar((unsigned char) (charToBeTransmitted & MASK_8LSB));

			if 	(m_NrOfCharToBeTransmitted == 0) 
			{
				(*g_pTransmitBuffer)->State = e_FT12State_Transmitted;	
				*g_pState = e_FT12State_Idle;	                    // Packet im Receive-Buffer wurde vollständig gesendet									     
			}
		}    
	}                                   
}



