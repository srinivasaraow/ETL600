/*-------------------------------------------------------------------------*
 * $Workfile	: dll2ndStation.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The dll2ndStation module implements a Data Link Layer Secondary
 *				  Station according to Nt61538c ETL500 Transmission protocols.doc.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/dll2ndStation.c $
 * 
 * 2     08-01-10 11:35 Chrobif
 * Some comments added.
 * 
 * 1     25.05.07 17:12 Chrobif
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <string.h>

// O4CV firmware
#include "dll2ndStation.h"
#include "ft12.h"
#include "basetype.h"
#include "msgHandler.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef enum 
{
	e_idling,
	e_rxMsgExecuted,
	e_copyingUD
} e_State;

/* LOCAL VARIABLE DECLARATIONS ***********************************************/
static e_State gs_state;

static t_ft12PF *gs_pRxMsgPF;
static UINT32   *gs_pRxMsgUD;
static t_ft12PF *gs_pTxMsgPF;
static UINT32   *gs_pTxMsgUD;

static UINT32   *gs_pUD;
static UINT32   *gs_pUDEnd;

static UINT16 gs_txMsgFunctionCode;
static UINT16 gs_txMsgLengthBytes;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
void executeRxMsg(void);
void setupTxMsgHeader(void);
BOOL copyUDWord(void);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void dll2ndStation_init(void)
  Description:	dll2ndStation_init() initializes the dll2ndStation module.
  Inputs:
  Outputs:
  Return:       	
  Globals:		gs_state, gs_pRxMsgPF, gs_pRxMsgUD, gs_pTxMsgPF, gs_pTxMsgUD
  				gs_pUD, gs_pUDEnd, gs_txMsgFunctionCode, gs_txMsgLengthBytes
  Important:	dll2ndStation_init() must be called before any other function
  				of the dll2ndStation module is called.
*******************************************************************************/
void dll2ndStation_init(void)
{
    gs_state = e_idling;
    
    gs_pRxMsgPF = ft12_rxGetPtrPF();
    gs_pRxMsgUD = ft12_rxGetPtrUD();
    gs_pTxMsgPF = ft12_txGetPtrPF();
    gs_pTxMsgUD = NULL;
    
    gs_pUD    = NULL;
	gs_pUDEnd = NULL;
	
	gs_txMsgFunctionCode = 0;
	gs_txMsgLengthBytes = 0;
}

/*****************************************************************************
  Function:		void dll2ndStation_activate(void)
  Description:	dll2ndStation_activate() implements a state machine which enables
  				the secondary station to receive and transmit FT1.2 messages.
  				dll2ndStation_activate() must be called periodically.
  Inputs:
  Outputs:
  Return:       	
  Globals:		gs_state
  Important:
*******************************************************************************/
void dll2ndStation_activate(void)
{
    switch (gs_state)
    {
    
    case e_idling:
    	if ( ft12_rxGetState() == e_ft12StateFull )
    	{
    	    executeRxMsg();
    		gs_state = e_rxMsgExecuted;
    	}
    	break;
    	
    case e_rxMsgExecuted:
    	setupTxMsgHeader();
    	ft12_txResetChecksum();
    	gs_state = e_copyingUD;
        break;
        
    case e_copyingUD:
    	if ( copyUDWord() )
    	{
    	    ft12_txMsgEncode();
    	    ft12_txSetState(e_ft12StateFull);
    		gs_state = e_idling;
    	}
    	break;
    	
    default:
        gs_state = e_idling;
    }
}

/*****************************************************************************
  Function:		void dll2ndStation_setupTxUDPtrs(UINT32 *pTxUD, UINT32 nTxUDWords)
  Description:	dll2ndStation_setupTxUDPtrs() sets up pointers in the dll2ndStation
  				module to point to the payload	of a FT1.2 TX message which is located
  				in the memory allocated by the caller. In subsequent calls to
  				dll2ndStation_activate() this payload is then copied to the memory
  				of the FT1.2 message.
  Inputs:		pTxUD:		Pointer to the tx user data
				nTxUDWords:	Number of 32 bit words user data to send.
  							The number of data to be send must be a multiple
  							of 4 bytes.
  Outputs:
  Return:       	
  Globals:		gs_txMsgLengthBytes, gs_pUD, gs_pUDEnd
  Important:                                      	
*******************************************************************************/
void dll2ndStation_setupTxUDPtrs(UINT32 *pTxUD, UINT32 nTxUDWords)
{
	gs_txMsgLengthBytes	= nTxUDWords * NR_OF_BYTES_IN_LONG + FT12_HEADER_LENGTH;	
	gs_pUD    = pTxUD;
	gs_pUDEnd = pTxUD + nTxUDWords;	
}

/*****************************************************************************
  Function:		void executeRxMsg(void)
  Description:	executeRxMsg() executes a received message by calling the
  				appropriate message handler.
  Inputs:
  Outputs:
  Return:       	
  Globals:		gs_pRxMsgPF, gs_txMsgFunctionCode, gs_pRxMsgUD, gs_pTxMsgUD
  Important:                                      	
*******************************************************************************/
void executeRxMsg(void)
{
		switch (gs_pRxMsgPF->ControlField & MASK_4LSB)
		{
			case RXFUNCTION_SEND_CONFIRM:
	 		   		gs_txMsgFunctionCode = TXFUNCTION_CONFIRM;
	 		   		msgHandler_executeMsg(gs_pRxMsgPF->MessageType, gs_pRxMsgUD);
					break;
			case RXFUNCTION_REQUEST_RESPOND:
	 		   		gs_txMsgFunctionCode = TXFUNCTION_RESPOND_USER_DATA;
	 		   		msgHandler_executeMsg(gs_pRxMsgPF->MessageType, gs_pRxMsgUD);
					break;
			case RXFUNCTION_RESPOND_EQ_ID:
	 		   		gs_txMsgFunctionCode = TXFUNCTION_EQUIPMENT_ID;
					msgHandler_respondEqId();
					break;
			default:
	 		   		gs_txMsgFunctionCode = TXFUNCTION_UNKNOWN;
					dll2ndStation_setupTxUDPtrs(NULL, 0);
		}
		
		ft12_rxSetState(e_ft12StateExecuted);
		
		gs_pTxMsgUD = ft12_txGetPtrUD();
}

/*****************************************************************************
  Function:		void setupTxMsgHeader(void)
  Description:	setupTxMsgHeader() sets up the FT1.2 header of the frame to be
  				send.
  Inputs:
  Outputs:
  Return:       	
  Globals:		gs_txMsgLengthBytes, gs_pTxMsgPF, gs_txMsgFunctionCode, gs_pRxMsgPF
  Important:                                      	
*******************************************************************************/
void setupTxMsgHeader(void)
{
	if ( gs_txMsgLengthBytes <= FT12_LENGTH_MAX )
	{
		ft12_txSetLength(gs_txMsgLengthBytes);
	    
		gs_pTxMsgPF->ControlField         = gs_txMsgFunctionCode;
		gs_pTxMsgPF->DestinationSubadress = gs_pRxMsgPF->DestinationSubadress;		
		gs_pTxMsgPF->MessageType          = gs_pRxMsgPF->MessageType;
		gs_pTxMsgPF->DestinationAdress    = gs_pRxMsgPF->SourceAdress;
		gs_pTxMsgPF->SourceAdress         = gs_pRxMsgPF->DestinationAdress;
		gs_pTxMsgPF->HopCounter           = FT12_HOP_MAX;
		gs_pTxMsgPF->SequenceNumber       = gs_pRxMsgPF->SequenceNumber;

	}
}

/*****************************************************************************
  Function:		BOOL copyUDWord(void)
  Description:	copyUDWord() copies 4 byte of user data from the chunk of memory
  				specified by a call of dll2ndStation_setupTxUDPtrs() to the
				data array of the ft12.c module.
  Inputs:
  Outputs:
  Return:       True, if the whole chunk has been copied. False, otherwise.	
  Globals:		gs_pUD, gs_pUDEnd, gs_pTxMsgUD
  Important:                                      	
*******************************************************************************/
BOOL copyUDWord(void)
{
    INT32 checksum = 0;
    BOOL rv = FALSE;
    
	if (gs_pUD < gs_pUDEnd)
	{
		*gs_pTxMsgUD = *gs_pUD++;				
		checksum +=  *gs_pTxMsgUD & MASK_8LSB;		
		checksum += (*gs_pTxMsgUD >> SB8 ) & MASK_8LSB;
		checksum += (*gs_pTxMsgUD >> SB16) & MASK_8LSB;
		checksum += (*gs_pTxMsgUD >> SB24) & MASK_8LSB;
		ft12_txAccChecksum(checksum);
		gs_pTxMsgUD++;
	}
	else
		rv = TRUE;
		
	return rv;
}
