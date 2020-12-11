/*-------------------------------------------------------------------------*
 * $Workfile: DL_2nd.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/DL_2nd.c $
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 6     29.07.04 6:42 Leeb02
 * 
 * 5     22.07.04 15:12 Leeb02
 * 
 * 4     20.07.04 13:07 Leeb02
 * 
 * 3     27.04.04 10:13 Leeb02
 * 
 * 2     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 14:06 Leeb02
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "diag.h"
#include "DL_2nd.h"
#include "msgHandler.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static e_FT12State            * g_pState;

static t_FT12TelegramBuffer ** g_pReceivedFrame;
static t_FT12TelegramBuffer ** g_pTransmitFrame;

/* LOCAL FUNCTIONS ***********************************************************/



/* IMPLEMENTATION ************************************************************/

void initDL_2nd(BOOL (* receiveBuffersEmpty)(void),
				t_FT12TelegramBuffer ** receivedFrame,
                t_FT12TelegramBuffer ** transmitFrame,
				e_FT12State           * pState)
{
	g_pReceivedFrame = receivedFrame;
	g_pTransmitFrame = transmitFrame;
	g_pState = pState;

	msgHandler_init(receiveBuffersEmpty);
}



//***************************************************************************************
//	void ApplicationLayer( )
//
// - generischer Application Layer
// - die Funktion void init_ApplicationLayer() muss zuerst ausgeführt worden sein (je nach Board
//   verschieden). Besonders die Adresse für NewFrame muss definiert sein, da die Funktion makeFrameHeader
//   sich auf diese globale Variable bezieht.
// - reagiert auf Funktioncode gemäss TN-NTC96-1538/C , Frame Format beim ETL500 Protokoll
// - ruft die Funktion executeMessage auf, die je nach Board, (P4LQ, O4LE, G4AI) verschieden ist
// - erstellt mit makeFrameUserData() die Frames unter Berücksichtigung der Rechenzeit.
// 
// Bemerkung:
// 
// 1) das Ziel solte sein die Globalen Variablen(pointer) g_pReceivedFrame, NewFrame
//			 und m_TxControlByte (@CL@ done) durch Parameterübergabe zuersetzen:
//			 	
//					- makeFrameHeader( &ACK, 0, NewFrame)
//					- get_ReceivedFrame(g_pReceivedFrame)
//
//					evt. neue Funktion get_NewFrame(NewFrame)
//					auch bei: executeMessage(g_pReceivedFrame)
//
//
//  2) Die Variable long  ACK wird benutzt um der Funktion makeFrameHeader einen Pointer uebergeben zu können.
//  Der Inhalt ist beliebig, da keine Daten gesendet werden.
//	ACK  = ACKNOWLEDGE; 			   
//  makeFrameHeader( &ACK, 0): only the frame will be send back, no data of ACK
//     @CL@ ACK replaced by NULL 
//***************************************************************************************

void ApplicationLayer(void)
{   
	switch (*g_pState)
	{
		static BOOL generateFrameHeader = FALSE;
		static unsigned long txControl;

		case e_FT12State_Forwarded:

			generateFrameHeader = TRUE;
			
			switch((*g_pReceivedFrame)->ControlField & MASK_4LSB)
			{
				case RXFUNCTION_SEND_CONFIRM:		
	 		   			txControl = TXFUNCTION_CONFIRM;			
						break;												// weiter mit dem Ausführen der Message Typen (executeMessage)
				case RXFUNCTION_REQUEST_RESPOND:							// Request of user data class 2 expected					
	 		   			txControl = TXFUNCTION_RESPOND_USER_DATA; 
						break;												// weiter mit dem Ausführen der Message Typen (executeMessage)
				case RXFUNCTION_RESPOND_EQ_ID:
	 		   			txControl = TXFUNCTION_EQUIPMENT_ID; 			
						msgHandler_equipmentFirmwareIdentification();		// Der Functioncodes ist ausgeführt.Es
						(*g_pReceivedFrame)->State = e_FT12State_Executed;
						*g_pState = e_FT12State_Executed;                   // werden KEINE Message Typen ausgeführt.
						return;					
				default:				
	 		   			txControl = TXFUNCTION_UNKNOWN;				        // Der Functioncodes ist unbekannt. Es
						makeFrameHeader(NULL, 0);							// werden KEINE Message Typen ausgeführt.
						(*g_pReceivedFrame)->State  = e_FT12State_Executed;
						*g_pState = e_FT12State_Executed;                   // werden KEINE Message Typen ausgeführt.
						return;        
			}
			
			switch (msgHandler_executeMessage(*g_pReceivedFrame))			
			{
				case e_ExecMsgStatus_Ok: 
					break;

				case e_ExecMsgStatus_DataNotAvailable:
					txControl = TXFUNCTION_DATA_NOT_AVAILABLE; 
					break;

				case e_ExecMsgStatus_AccessDenied:
					txControl = TXFUNCTION_ACCESS_DENIED; 
					break;

				case e_ExecMsgStatus_Deferred:            // deferred message execution
					*g_pState = e_FT12State_Deferred;
					return;

				default:
					ASSERT(FALSE);
			}

			(*g_pReceivedFrame)->State  = e_FT12State_Executed;	            // Der Application Layer das Telegramm ausgeführt, kann
			*g_pState = e_FT12State_Executed;			                    // jedoch noch keine neuen Telegramme bearbeiten. 
			break;

			
		case e_FT12State_Deferred:

			switch (msgHandler_lastDelayedMessageTypeStatus())			
			{
				case e_ExecMsgStatus_AwaitingResponse:                      // still waiting for response
					return;

				case e_ExecMsgStatus_Ok: 
					break;

				case e_ExecMsgStatus_DataNotAvailable:
					txControl = TXFUNCTION_DATA_NOT_AVAILABLE; 
					break;

				case e_ExecMsgStatus_AccessDenied:
					txControl = TXFUNCTION_ACCESS_DENIED; 
					break;

				default:
					ASSERT(FALSE);
			}
	
			(*g_pReceivedFrame)->State  = e_FT12State_Executed;	            // Der Application Layer das Telegramm ausgeführt, kann
			*g_pState = e_FT12State_Executed;			                    // jedoch noch keine neuen Telegramme bearbeiten. 
			break;

		case e_FT12State_Executed:

			if (generateFrameHeader)		// Führe die Funktion makeFrameHeaderComplet einmal aus.
			{								// Das Erzeugen des Frame Header wurde aus der der Funktion
											// executeMessage "herausgezogen", wobei in der 
											// neuen Funktion makeFrameHeader nur noch drei globale 
											// Variablen gesetzt werden. -> Rechenzeitgewinn ca. 170 Zyklen
				makeFrameHeaderComplete(txControl, 
					                    *g_pReceivedFrame, 
										*g_pTransmitFrame);
				generateFrameHeader = FALSE;	// Der Frame Header wurde generiert.
			}
			else 
			{
				if (makeFrameUserData())	// Bei jedem Aufruf wird das Frame um ein Word User Daten ergänzt.
				{
					(*g_pTransmitFrame)->State = e_FT12State_Full;				
					(*g_pReceivedFrame)->State  = e_FT12State_Empty;
					// toDL_secondaryStationState = FULL;// Receive Buffer kann wieder Telegramme empfangen 
					*g_pState = e_FT12State_Idle;		// toDL_secondaryStationState = FULL läuft nicht	
				}
			}
			break;

		case e_FT12State_Full: 
		case e_FT12State_Idle:
			break;

		default: ASSERT(FALSE);
	}
}                                              


