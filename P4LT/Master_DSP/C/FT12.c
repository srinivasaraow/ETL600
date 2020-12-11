/*-------------------------------------------------------------------------*
 * $Workfile: FT12.c $
 * Part of	: ETL600 
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/Master_DSP/C/FT12.c $
 * 
 * 6     25.08.06 19:12 Chchlee
 * new fct. SetSubAddressLAN600Bypass()
 *  * ClearAddressBypassLAN600() renamed to ChangeDestiAddressLAN600
 * 
 * 5     10.08.06 18:17 Chmibuh
 * new fct. SetSubAddressLAN600Bypass()
 * ClearAddressBypassLAN600() renamed to ChangeDestiAddressLAN600 and
 * remove manipulation of DestinationSubAdress
 * 
 * 3     27.02.06 14:14 Chmibuh
 * Bridge support for LAN600 added
 * ClearAddressBypassLAN600()
 * SetSubAddressLAN600()
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 6     13.08.04 8:54 Leeb02
 * 
 * 5     4.08.04 14:55 Leeb02
 * 
 * 4     25.07.04 11:44 Leeb02
 * makeFrameHeader: argument type changed to "void *"
 * 
 * 3     27.04.04 10:13 Leeb02
 * 
 * 2     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 17:57 Leeb02
 * 
 * 1     5.04.04 14:14 Leeb02
 * 
 * 1     5.04.04 14:07 Leeb02
  * Initial Revision
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "FT12.h"


/************************************************************************************
* Implementierung des Frames														*	
* TM-NKT98-1576/_																	*
*																					*
*		|	Byte																	*
*----------------------------------------------------------------------------------	*
* Word	| 4					| 3					| 2				| 1					*
*----------------------------------------------------------------------------------	*
* 1		| L					|	L				| Start 68H		| 0					*
*----------------------------------------------------------------------------------	*
* 2		| Message Type (LSB)| Dest. Subaddr		| Control Field	| Start 68H			*
*----------------------------------------------------------------------------------	*
* 3		| Source Addr (LSB)	| Dest. Addr(MSB)	| Dest.Addr(LSB)| Message Type (MSB)*
*----------------------------------------------------------------------------------	*
* 4		| Sequence Nr(MSB)	| Sequence Nr(LSB)	| HOP Counter	| Source Addr. (MSB)*
*----------------------------------------------------------------------------------	*
* 5		| UD (Bit 31.24)	| UD (Bit 23.16)	| UD (Bit 15.8)	| UD (Bit 7.0)		*
*----------------------------------------------------------------------------------	*
* 6		| UD (Bit 31.24)	| UD (Bit 23.16)	| UD (Bit 15.8)	| UD (Bit 7.0)		*
*----------------------------------------------------------------------------------	*
*		|					|					|				|					*
*		|					|					|				|					*
*----------------------------------------------------------------------------------	*
* 65	| UD (Bit 31.24)	| UD (Bit 23.16)	| UD (Bit 15.8)	| UD (Bit 7.0)		*
*----------------------------------------------------------------------------------	*
* 66	| 0					| 0					| End 16H		| Checksum			*
*																					*
************************************************************************************/


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define HOP_MAX			13		// Max 6 Kabelverbindungen, nach Absprache Benninger + Morf
							
#define MASK_1724LSB					0xff00ffff
#define MASK_0916LSB					0xffff00ff

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/

static long          *g_pTxData;

static const long    *g_pStart; 
static const long    *g_pEnd;
static long           g_checksum;
static long           g_NCharToTransmit;


/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/


/******************************************************************************/
//
// void makeFrameHeader(long *MessageToEquipment, long length_of_txmsg)
//
// - erzeugt ein neues Frame mit dem Header und dessen Prüfsumme
// - initialisiert die Funktion long makeFrameUserData() 
/******************************************************************************/

void makeFrameHeader(const void * MessageToEquipment, unsigned long sizeInLongs)
			
{	// initialisiere die globalen Variablen, die in den Funktionen	makeFrameHeaderComplet

	g_NCharToTransmit	= sizeInLongs * NR_OF_BYTES_IN_LONG + LENGTH_HEADER;	
	g_pStart = MessageToEquipment;

	g_pEnd	 = (long*)MessageToEquipment + sizeInLongs;	    
}

/******************************************************************************/
//
// void makeFrameHeaderComplete(unsigned long txControl)
//
// - erzeugt ein neues Frame mit dem Header und dessen Prüfsumme
// - initialisiert die Funktion long makeFrameUserData() 
/******************************************************************************/

void makeFrameHeaderComplete(unsigned long               txControl,
							 const t_FT12TelegramBuffer *pReceivedFrame,
							       t_FT12TelegramBuffer *pNewFrame)
{
	if(g_NCharToTransmit <= LENGTH_MAX) 
	{
  	    long word;
	
		pNewFrame-> ControlField        = txControl;
		pNewFrame-> DestinationSubadress= pReceivedFrame-> DestinationSubadress;		
		pNewFrame-> MessageType         = pReceivedFrame-> MessageType;
		pNewFrame-> DestinationAdress   = pReceivedFrame-> SourceAdress;
		pNewFrame-> SourceAdress        = pReceivedFrame-> DestinationAdress;
		pNewFrame-> HopCounter          = HOP_MAX;
		pNewFrame-> SequenceNumber      = pReceivedFrame->SequenceNumber;

		g_pTxData	 = pNewFrame->TelegrammDataArr;
		*g_pTxData++ = (g_NCharToTransmit         <<SB24) | (g_NCharToTransmit                <<SB16)| (FT12_START_CHAR               <<SB8  | 0x0); 
		*g_pTxData++ = (pNewFrame-> MessageType   <<SB24) | (pNewFrame-> DestinationSubadress <<SB16)| (txControl                     <<SB8) | FT12_START_CHAR;
		*g_pTxData++ = (pNewFrame-> SourceAdress  <<SB24)                                            | (pNewFrame-> DestinationAdress <<SB8) | ((pNewFrame-> MessageType >>SB8) & MASK_8LSB);
		*g_pTxData++ =                                      (pNewFrame-> SequenceNumber       <<SB16)| (pNewFrame-> HopCounter        <<SB8) | ((pNewFrame->SourceAdress >>SB8) & MASK_8LSB); 

		/* berechne Prüfsumme des Headers */
		g_checksum = 0;
		g_pTxData	= pNewFrame->TelegrammDataArr;
		g_pTxData++; // zeigt auf 2. Word

		for(word = 2; word <= 4; word++)
		{
			g_checksum +=  *g_pTxData         & MASK_8LSB;
			g_checksum += (*g_pTxData >> SB8 )& MASK_8LSB;
			g_checksum += (*g_pTxData >> SB16)& MASK_8LSB;
			g_checksum += (*g_pTxData >> SB24)& MASK_8LSB;
			g_pTxData++;
		}
		 /* Die Prüfsumme beginnt erst mit dem Control Field Byte, darum müssen
			das Startzeichen vom 2. Word, die durch das "word-weise" Berechnen 
			fälschlicherweise dazu gekommen sind, abgezogen werden
		*/
		g_checksum -= FT12_START_CHAR; 
	}
}

/******************************************************************************/
//
// BOOL makeFrameUserData()
//
// - ergänzt das Frame mit der Prüfsumme und den Daten
// - bei einem neuen Frame muss zuerst die Funktion makeFrameHeader ausgeführt werden.
//   Darin werden die Variablen initialisiert.
// - Der Return Wert 1 zeigt das Ende der Frame - Zusammensetzung an
// -> keine Echtzeitanforderung, 
//	
// pro Aufruf wird, solange noch USER Daten übrig sind:
// -  ein "word" von den USER Daten in den Frame Buffer kopiert
// -  die aktuelle Prüfsumme mit diesem "word" berechnet 
// -  der Return Wert 0 übergeben
// 
/******************************************************************************/

BOOL makeFrameUserData(void)
{
	if (g_pStart < g_pEnd)
	{
		*g_pTxData = *g_pStart++;		         // copy one user data word		
		g_checksum += *g_pTxData & MASK_8LSB;		 // update checksum
		g_checksum += (*g_pTxData >> SB8 ) & MASK_8LSB;
		g_checksum += (*g_pTxData >> SB16) & MASK_8LSB;
		g_checksum += (*g_pTxData >> SB24) & MASK_8LSB;
		g_pTxData++;
		return(FALSE);		
	}

	g_checksum &= MASK_8LSB;
	*g_pTxData = g_checksum |(FT12_END_CHAR << SB8);
	return(TRUE);	
}



/******************************************************************************/
//
// void  SetAddressBypass(t_FT12TelegramBuffer *);
// 
// - addiert 64 zur Subadresse im Frame mit Anpassung der Prüfsumme
// - aus der Länge des Frame kann die Position der Prüfsumme berechnet werden
// 
/******************************************************************************/

void  SetAddressBypass(t_FT12TelegramBuffer *Frame)
{

	long checksumIndex;			// Anzahl Word im Telegramm -> die Checksumme ist im letzen Word !
	long checksum;

	Frame-> DestinationSubadress += 64;
	Frame-> TelegrammDataArr[1] &=MASK_1724LSB;							// lösche Bit 17-24
	Frame-> TelegrammDataArr[1] |= (Frame-> DestinationSubadress &  MASK_8LSB) <<SB16;
	checksumIndex = 4 + (FT12_SIZEOF(Frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG);		// (Len-LENGTH_HEADER		
	checksum = (Frame->TelegrammDataArr[checksumIndex]) & MASK_8LSB;
	checksum += 64;
	checksum &= MASK_8LSB;
	Frame->TelegrammDataArr[checksumIndex] = checksum |(FT12_END_CHAR << SB8);
}


/******************************************************************************/
//
// void  ClearAddressBypass(t_FT12TelegramBuffer *);
//
//
// - subtrahiert 64 von der Subadresse mit Anpassung der Prüfsumme
// - aus der Länge des Frame kann die Position der Prüfsumme berechnet werden
// - Achtung: was passiert, wenn das Resultat negativ wird ? 
/******************************************************************************/

void  ClearAddressBypass(t_FT12TelegramBuffer *Frame)
{

	long checksumIndex;			// Anzahl Word im Telegramm -> die Checksumme ist im letzen Word !
	long checksum;

	Frame-> DestinationSubadress -= 64;
	Frame-> TelegrammDataArr[1] &=MASK_1724LSB; // lösche Bit 17-24
	Frame-> TelegrammDataArr[1] |= Frame-> DestinationSubadress <<SB16;
	checksumIndex = 4 + (FT12_SIZEOF(Frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG);		// (Len-LENGTH_HEADER		
	checksum = ((Frame->TelegrammDataArr[checksumIndex]) & MASK_8LSB) + 0x100;	// 0x100 Uebertrag, damit es keine 
																			// negativen Werte gibt. beim Addieren
																			// wird der Uebertrag immer ausmaskiert !
	checksum -= 64;		
	checksum &= MASK_8LSB;
	Frame->TelegrammDataArr[checksumIndex]=checksum |(FT12_END_CHAR << SB8);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Special version for LAN600
//////////////////////////////////////////////////////////////////////////////////////////////////
// set DestinationAdress to g_myAddress
// Adapt the TelegrammDataArr and checksum only.
// Frame-> DestinationAdress = g_myAddress is already done
// in checkAddress(), because g_myAddress is unknown here.

void SEGMENT_EXT_CODE ChangeDestinAddressLAN600(t_FT12TelegramBuffer *Frame)
{

	long checksumIndex;			// Anzahl Word im Telegramm -> die Checksumme ist im letzen Word !
	long checksum;

	Frame-> TelegrammDataArr[1] &=MASK_1724LSB;							// Mask 0xff00ffff; clear DestinationSubadress 
	Frame-> TelegrammDataArr[1] |= Frame-> DestinationSubadress <<SB16; // Fill in new DestinationSubadress

	Frame-> TelegrammDataArr[2] &= (MASK_1724LSB & MASK_0916LSB);		// Mask 0xff0000ff; clear DestinationAdress
	Frame-> TelegrammDataArr[2] |= ((Frame-> DestinationAdress) << SB8);// Fill in new DestinationAdress

	checksumIndex = 4 + (FT12_SIZEOF(Frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG);		// (Len-LENGTH_HEADER		
	checksum = ((Frame->TelegrammDataArr[checksumIndex]) & MASK_8LSB) + 0x100;	// 0x100 Uebertrag, damit es keine 
																			// negativen Werte gibt. beim Addieren
																			// wird der Uebertrag immer ausmaskiert !
	// Adapt checksum with the two manipulations 
	checksum += (Frame->DestinationAdress);	// DestinationAdress - prev. Adr was 0 (local/remote)

	checksum &= MASK_8LSB;
	Frame->TelegrammDataArr[checksumIndex]=checksum |(FT12_END_CHAR << SB8);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Special version for LAN600
//////////////////////////////////////////////////////////////////////////////////////////////////
// Frame-> DestinationSubadress = LAN_LOCAL is already done in checkAddress(), 
// because LAN_LOCAL is unknown here.

void SEGMENT_EXT_CODE SetSubAddressLAN600(t_FT12TelegramBuffer *Frame)
{

	long checksumIndex;			// Anzahl Word im Telegramm -> die Checksumme ist im letzen Word !
	long checksum;

	Frame-> TelegrammDataArr[1] &=MASK_1724LSB;							// Mask 0xff00ffff; clear DestinationSubadress 
	Frame-> TelegrammDataArr[1] |= Frame-> DestinationSubadress <<SB16; // Fill in new DestinationSubadress


	checksumIndex = 4 + (FT12_SIZEOF(Frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG);		// (Len-LENGTH_HEADER		
	checksum = ((Frame->TelegrammDataArr[checksumIndex]) & MASK_8LSB);	
	
	// Adapt checksum  
	checksum += (Frame->DestinationSubadress);	// DestinationSubAdress - prev. Sub Adr was 0, now it is LAN_LOCAL

	checksum &= MASK_8LSB;
	Frame->TelegrammDataArr[checksumIndex]=checksum |(FT12_END_CHAR << SB8);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Special version for LAN600
//////////////////////////////////////////////////////////////////////////////////////////////////
// Frame-> DestinationSubadress = LAN_BYPASS is already done in checkAddress(), 
// because LAN_BYPASS is unknown here.

void SEGMENT_EXT_CODE SetSubAddressLAN600Bypass(t_FT12TelegramBuffer *Frame)
{

	long checksumIndex;			// Anzahl Word im Telegramm -> die Checksumme ist im letzen Word !
	long checksum;

	Frame-> TelegrammDataArr[1] &=MASK_1724LSB;							// Mask 0xff00ffff; clear DestinationSubadress 
	Frame-> TelegrammDataArr[1] |= Frame-> DestinationSubadress <<SB16; // Fill in new DestinationSubadress


	checksumIndex = 4 + (FT12_SIZEOF(Frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG);		// (Len-LENGTH_HEADER		
	checksum = ((Frame->TelegrammDataArr[checksumIndex]) & MASK_8LSB);	
	
	// Adapt checksum  
	checksum += 128;			// DestinationSubAdress - prev. Sub Adr was LAN_LOCAL, now it is LAN_BYPASS

	checksum &= MASK_8LSB;
	Frame->TelegrammDataArr[checksumIndex]=checksum |(FT12_END_CHAR << SB8);
}




/******************************************************************************/
// void ChangeHopCounter(t_FT12TelegramBuffer *Frame)
//
//
/******************************************************************************/

void ChangeHopCounter(t_FT12TelegramBuffer *Frame)
{

	long checksumIndex;			// Anzahl Word im Telegramm -> die Checksumme ist im letzen Word !
	long checksum;

	if (Frame->HopCounter > 0)	// do not make HopCounter negative, Bugs ...
	{
		Frame->HopCounter--;
		Frame->TelegrammDataArr[3] &=MASK_0916LSB;						// lösche Bit 09-16
		Frame->TelegrammDataArr[3] |= Frame->HopCounter <<SB8;

		// get checksum
		checksumIndex = 4 + (FT12_SIZEOF(Frame->TelegrammDataArr) / NR_OF_BYTES_IN_LONG);		// (Len-LENGTH_HEADER		
		checksum = ((Frame->TelegrammDataArr[checksumIndex]) & MASK_8LSB) + 0x100;	// 0x100 Uebertrag, damit es keine 
																				// negativen Werte gibt. beim Addieren
																				// wird der Uebertrag immer ausmaskiert !
		checksum = (checksum - 1) & MASK_8LSB;
		Frame->TelegrammDataArr[checksumIndex] = checksum |(FT12_END_CHAR << SB8);
	}
}
