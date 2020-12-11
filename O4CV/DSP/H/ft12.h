/*-------------------------------------------------------------------------*
 * $Workfile	: ft12.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The ft12 module allocates memory for two FT1.2 frames
 *				  (one rx and one tx) and provides functions to manipulate them.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/ft12.h $
 * 
 * 2     25.05.07 17:03 Chrobif
 * ft12_txSetFunctionCode() removed.
 * 
 * 1     25.05.07 16:40 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
/************************************************************************************
* Documentation:																	*
* The FT1.2 protocol and its frame format is described in							*
* Nt61538c ETL500 Transmission protocols.doc										*
* The table below describes the memory layout in the FT1.2 frame data buffer.		*	
* Byte 0 in word 0 is not used in order to align the first byte of user data.		*
*																					*
*		|	Byte																	*
*----------------------------------------------------------------------------------	*
* Word	| 3					| 2					| 1				| 0					*
*----------------------------------------------------------------------------------	*
* 0		| L					|	L				| Start 68H		| 0					*
*----------------------------------------------------------------------------------	*
* 1		| Message Type (LSB)| Dest. Subaddr		| Control Field	| Start 68H			*
*----------------------------------------------------------------------------------	*
* 2		| Source Addr (LSB)	| Dest. Addr(MSB)	| Dest.Addr(LSB)| Message Type (MSB)*
*----------------------------------------------------------------------------------	*
* 3		| Sequence Nr(MSB)	| Sequence Nr(LSB)	| HOP Counter	| Source Addr. (MSB)*
*----------------------------------------------------------------------------------	*
* 4		| UD (Bit 31.24)	| UD (Bit 23.16)	| UD (Bit 15.8)	| UD (Bit 7.0)		*
*----------------------------------------------------------------------------------	*
* 5		| UD (Bit 31.24)	| UD (Bit 23.16)	| UD (Bit 15.8)	| UD (Bit 7.0)		*
*----------------------------------------------------------------------------------	*
*		|					|					|				|					*
*		|					|					|				|					*
*----------------------------------------------------------------------------------	*
* N - 1	| UD (Bit 31.24)	| UD (Bit 23.16)	| UD (Bit 15.8)	| UD (Bit 7.0)		*
*----------------------------------------------------------------------------------	*
* N		| 0					| 0					| End 16H		| Checksum			*
*																					*
* N is between 4 and 65																*
************************************************************************************/

#ifndef FT12_H
#define FT12_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
#define FT12_START_CHAR						0x68
#define FT12_END_CHAR						0x16

#define FT12_START_FLAG_MASK	0xff0000ff
#define FT12_START_FLAG			(FT12_START_CHAR<<SB24 | FT12_START_CHAR)

#define FT12_END_FLAG_MASK		0x0000ff00
#define FT12_END_FLAG			(FT12_END_CHAR<<SB8)

#define FT12_CHECKSUM_MASK		0x000000ff

#define FT12_HEADER_LENGTH       11
#define FT12_FRAME_HEADER_LENGTH  4
#define FT12_FRAME_TRAILER_LENGTH 2
#define FT12_FRAME_OVERHEAD (FT12_FRAME_HEADER_LENGTH + FT12_FRAME_TRAILER_LENGTH)
#define FT12_LENGTH_MAX		255

#define FT12_HOP_MAX 13

// FT1.2 protocol fields 
#define CONTROL_FIELD	1
#define	DEST_SUB_ADR	2
#define	MESS_TYP_LSB	3
#define	MESS_TYP_MSB	4
#define	DEST_ADR_LSB	5
#define DEST_ADR_MSB	6
#define	SOUR_ADR_LSB	7
#define SOUR_ADR_MSB	8
#define HOP_COUNTER		9
#define SEQUENCE_NR_LSB	10
#define SEQUENCE_NR_MSB	11

// FT1.2 function codes for messages form primary station
#define RXFUNCTION_RESET_FCB			1
#define RXFUNCTION_SEND_CONFIRM			3
#define RXFUNCTION_REQUEST_RESPOND		11
#define RXFUNCTION_RESPOND_EQ_ID		14

// FT1.2 function codes for messages form secondary station
#define TXFUNCTION_CONFIRM				0
#define TXFUNCTION_UNKNOWN				1
#define TXFUNCTION_ACCESS_DENIED		6
#define TXFUNCTION_RESPOND_USER_DATA	8
#define TXFUNCTION_DATA_NOT_AVAILABLE	9
#define TXFUNCTION_EQUIPMENT_ID			13
                    
/* PUBLIC TYPE DECLARATIONS ***************************************************/
typedef enum 
{
	e_ft12StateEmpty,
	e_ft12StateFull,
	e_ft12StateExecuted,
	e_ft12StateTransmitted
} e_ft12State;

typedef struct {
    UINT16 ControlField;
    UINT16 DestinationSubadress;
    UINT16 MessageType;
    UINT16 DestinationAdress;
    UINT16 SourceAdress;
    UINT16 HopCounter;
    UINT16 SequenceNumber;
} t_ft12PF;

/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
extern void ft12_init(void);

extern e_ft12State ft12_rxGetState(void);
extern e_ft12State ft12_txGetState(void);

extern void ft12_rxSetState(e_ft12State state);
extern void ft12_txSetState(e_ft12State state);

extern t_ft12PF* ft12_rxGetPtrPF(void);
extern t_ft12PF* ft12_txGetPtrPF(void);

extern UINT32* ft12_rxGetPtrData(void);
extern UINT32* ft12_txGetPtrData(void);

extern UINT32* ft12_rxGetPtrUD(void);
extern UINT32* ft12_txGetPtrUD(void);

extern void ft12_rxMsgDecode(void);
extern void ft12_txMsgEncode(void);

extern void ft12_txSetLength(UINT16 nBytes);

extern void ft12_txResetChecksum(void);
extern void ft12_txAccChecksum(INT32 checksum);

#endif

