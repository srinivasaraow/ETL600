/*-------------------------------------------------------------------------*
 * $Workfile	: ft12.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The ft12 module allocates memory for two FT1.2 frames
 *				  (one rx and one tx) and provides functions to manipulate them.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/ft12.c $
 * 
 * 3     29.05.07 9:30 Chrobif
 * Bug fix in ft12_txMsgEncode().
 * 
 * 2     25.05.07 17:03 Chrobif
 * ft12_txSetFunctionCode() removed.
 * 
 * 1     25.05.07 16:39 Chrobif
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <string.h>

// O4CV firmware
#include "ft12.h"
#include "basetype.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define FT12_WORD_OFFSET_UD 4

/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef struct {
    UINT16 length;
    t_ft12PF ft12PF;
    INT32 checksum;
    e_ft12State state;
	UINT32 data[66];
} t_ft12Frame;

/* LOCAL VARIABLE DECLARATIONS ***********************************************/
static t_ft12Frame gs_rxFt12Frame;
static t_ft12Frame gs_txFt12Frame;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void ft12_init(void)
  Description:	ft12_init() initializes the ft12 module. It must be called
  				before any other function of the ft12 module is beeing called.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_init(void)
{
    memset(&gs_rxFt12Frame, 0, sizeof(gs_rxFt12Frame));
    gs_rxFt12Frame.state = e_ft12StateEmpty;
    
    memset(&gs_txFt12Frame, 0, sizeof(gs_txFt12Frame));
    gs_txFt12Frame.state = e_ft12StateEmpty;
    
    gs_rxFt12Frame.state = e_ft12StateEmpty;
    gs_txFt12Frame.state = e_ft12StateEmpty;
}

/*****************************************************************************
  Function:		e_ft12State ft12_rxGetState(void)
  Description:	ft12_rxGetState() is a getter which returns the state of the
  				rx FT1.2 frame.
  Inputs:
  Outputs:
  Return:       e_ft12State: State of the rx FT1.2 frame
  Globals:
  Important:                                      	
*******************************************************************************/
e_ft12State ft12_rxGetState(void)
{
    return  gs_rxFt12Frame.state;
}

/*****************************************************************************
  Function:		e_ft12State ft12_txGetState(void)
  Description:	ft12_txGetState() is a getter which returns the state of the
  				tx FT1.2 frame.
  Inputs:
  Outputs:
  Return:       e_ft12State: State of the tx FT1.2 frame
  Globals:
  Important:                                      	
*******************************************************************************/
e_ft12State ft12_txGetState(void)
{
    return  gs_txFt12Frame.state;
}

/*****************************************************************************
  Function:		void ft12_rxSetState(e_ft12State state)
  Description:	ft12_rxSetState() is a setter to set the state of the
  				rx FT1.2 frame.
  Inputs:		e_ft12State state: State to set
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_rxSetState(e_ft12State state)
{
    gs_rxFt12Frame.state = state;
}

/*****************************************************************************
  Function:		void ft12_txSetState(e_ft12State state)
  Description:	ft12_txSetState() is a setter to set the state of the
  				tx FT1.2 frame.
  Inputs:		e_ft12State state: State to set
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_txSetState(e_ft12State state)
{
    gs_txFt12Frame.state = state;
}

/*****************************************************************************
  Function:		t_ft12PF* ft12_rxGetPtrPF(void)
  Description:	ft12_rxGetPtrPF is a getter which returns a pointer to the
  				protocol field struct of the rx FT1.2 frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
t_ft12PF* ft12_rxGetPtrPF(void)
{
    return &gs_rxFt12Frame.ft12PF;
}

/*****************************************************************************
  Function:		t_ft12PF* ft12_txGetPtrPF(void)
  Description:	ft12_txGetPtrPF() is a getter which returns a pointer to the
  				protocol field struct of the tx FT1.2 frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
t_ft12PF* ft12_txGetPtrPF(void)
{
    return &gs_txFt12Frame.ft12PF;
}

/*****************************************************************************
  Function:		UINT32* ft12_rxGetPtrData(void)
  Description:	ft12_rxGetPtrData() is a getter which returns a pointer to the
  				data array of the rx FT1.2 frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
UINT32* ft12_rxGetPtrData(void)
{
    return gs_rxFt12Frame.data;
}

/*****************************************************************************
  Function:		UINT32* ft12_txGetPtrData(void)
  Description:	ft12_txGetPtrData() is a getter which returns a pointer to the
  				data array of the tx FT1.2 frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
UINT32* ft12_txGetPtrData(void)
{
    return gs_txFt12Frame.data;
}

/*****************************************************************************
  Function:		UINT32* ft12_rxGetPtrUD(void)
  Description:	ft12_rxGetPtrUD() is a getter which returns a pointer to the
  				first word of user data within the data array of the rx
  				FT1.2 frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
UINT32* ft12_rxGetPtrUD(void)
{
    return &gs_rxFt12Frame.data[FT12_WORD_OFFSET_UD];
}

/*****************************************************************************
  Function:		UINT32* ft12_txGetPtrUD(void)
  Description:	ft12_txGetPtrUD() is a getter which returns a pointer to the
  				first word of user data within the data array of the tx
  				FT1.2 frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
UINT32* ft12_txGetPtrUD(void)
{
    return &gs_txFt12Frame.data[FT12_WORD_OFFSET_UD];
}

/*****************************************************************************
  Function:		void ft12_rxMsgDecode(void)
  Description:	ft12_rxMsgDecode() decodes the FT1.2 rx frame. In other words
  				it extracts the protocol fields from the data array and stores
  				it in the protocol field structure.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_rxMsgDecode(void)
{
	gs_rxFt12Frame.ft12PF.ControlField         = ( gs_rxFt12Frame.data[1] >> 8)  & MASK_8LSB;
	gs_rxFt12Frame.ft12PF.DestinationSubadress = ( gs_rxFt12Frame.data[1] >> 16) & MASK_8LSB;
	gs_rxFt12Frame.ft12PF.MessageType          = ((gs_rxFt12Frame.data[1] >> 24) & MASK_8LSB) |
										         ((gs_rxFt12Frame.data[2] << 8)  & 0x0000ff00);
	gs_rxFt12Frame.ft12PF.DestinationAdress    = ( gs_rxFt12Frame.data[2] >> 8)  & 0x0000ffff;
	gs_rxFt12Frame.ft12PF.SourceAdress         = ((gs_rxFt12Frame.data[2] >> 24) & MASK_8LSB) |
										         ((gs_rxFt12Frame.data[3] << 8)  & 0x0000ff00);
	gs_rxFt12Frame.ft12PF.HopCounter           = ( gs_rxFt12Frame.data[3] >> 8)  & MASK_8LSB;
	gs_rxFt12Frame.ft12PF.SequenceNumber       = ( gs_rxFt12Frame.data[3] >> 16) & 0x0000ffff;
}

/*****************************************************************************
  Function:		void ft12_txMsgEncode(void)
  Description:	ft12_txMsgEncode() encodes the FT1.2 tx frame. In other words
  				it writes the protocol fields in the protocol field structure
  				into the data array.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_txMsgEncode(void)
{
    UINT32   *pData = gs_txFt12Frame.data;
    t_ft12PF *pft12PF   = &gs_txFt12Frame.ft12PF;
    UINT16 word;
    UINT16 idxLastWord;
    
	*pData++ = (gs_txFt12Frame.length  <<SB24) | (gs_txFt12Frame.length		    <<SB16)| 0x6800; 
	*pData++ = (pft12PF->MessageType   <<SB24) | (pft12PF->DestinationSubadress <<SB16)| ( pft12PF->ControlField <<SB8) | 0x68;
	*pData++ = (pft12PF->SourceAdress  <<SB24) | (pft12PF->DestinationAdress	<<SB8) | ((pft12PF->MessageType       >>SB8) & MASK_8LSB);
	*pData++ = (pft12PF->SequenceNumber<<SB16) | (pft12PF->HopCounter           <<SB8) | ((pft12PF->SourceAdress      >>SB8) & MASK_8LSB); 

	// Continue the calculation of the checksum by accumulating the
	// the bytes of the header of the protocol
	gs_txFt12Frame.checksum -= 0x68; // Substract one start character because we will add one in the for loop
	pData = &gs_txFt12Frame.data[1];
	for(word=1;word<=3;word++)
	{
		gs_txFt12Frame.checksum +=  *pData & MASK_8LSB;
		gs_txFt12Frame.checksum += (*pData >> SB8 )& MASK_8LSB;
		gs_txFt12Frame.checksum += (*pData >> SB16)& MASK_8LSB;
		gs_txFt12Frame.checksum += (*pData >> SB24)& MASK_8LSB;
		pData++;
	}	
	gs_txFt12Frame.checksum &= MASK_8LSB;
	
	idxLastWord = FT12_WORD_OFFSET_UD + (gs_txFt12Frame.length - FT12_HEADER_LENGTH) / NR_OF_BYTES_IN_LONG;
	gs_txFt12Frame.data[idxLastWord] = gs_txFt12Frame.checksum | (FT12_END_CHAR << SB8);
}

/*****************************************************************************
  Function:		void ft12_txSetLength(UINT16 nBytes)
  Description:	ft12_txSetLength() is a setter to set the length of the tx FT1.2
  				frame.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_txSetLength(UINT16 nBytes)
{
    gs_txFt12Frame.length = nBytes;
}

/*****************************************************************************
  Function:		void ft12_txResetChecksum(void)
  Description:	ft12_txResetChecksum() can be called to reset the accumulator
  				of the tx FT1.2 frame checksum.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_txResetChecksum(void)
{
    gs_txFt12Frame.checksum = 0;
}

/*****************************************************************************
  Function:		void ft12_txAccChecksum(INT32 checksum)
  Description:	ft12_txAccChecksum() can be called to add a value to the tx FT1.2
  				frame checksum accumulator.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:                                      	
*******************************************************************************/
void ft12_txAccChecksum(INT32 checksum)
{
    gs_txFt12Frame.checksum += checksum;
}
