/*-------------------------------------------------------------------------*
 * $Workfile: bridge.c $
 * Part of	: ETl600
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/Master_DSP/C/bridge.c $
 * 
 * 15    9.10.06 15:38 Chchlee
 * Conditional code for debugging of FT1.2 bridge rules added
 * 
 * 16    28.08.06 15:50 Chchlee
 * Some code moved to external RAM to free internal RAM
 * 
 * 15    25.08.06 18:04 Chmibuh
 * Rule 2 expanded: if received from RS-232 then discard packet else
 * follow Rule 2
 * 
 * 14    21.08.06 17:16 Chmibuh
 * replace 65000 with FT12_MIN_HMI_ADDRESS (65001)
 * 
 * 13    16.08.06 17:58 Chmibuh
 * Change Rule 8: discard packet only if it is received from RS-232
 * 
 * 12    10.08.06 18:24 Chmibuh
 * changes of RULE 2, 4 and 5 implemented.
 * new RULE 8 added
 * 
 * 10    5.04.06 13:39 Chmibuh
 * RULE 7, see TN-PTUKT06-1537 implemented
 * Comments added
 * 
 * 9     3.03.06 13:18 Chmabur1
 * changed from LAN_Types.h to R1LA_Types.h
 * 
 * 8     27.02.06 14:07 Chmibuh
 * Bridge support for LAN600 added
 * 
 * 6     22.11.05 16:06 Chmibuh
 * Prepared for Emergency FW
 * 
 * 5     3.11.05 16:55 Chchlee
 * resetP4LT if initialization of DL_232 failed
 * 
 * 4     28.09.05 15:39 Chchlee
 * 
 * 3     27.09.05 16:12 Chchlee
 * 
 * 2     6.09.05 11:22 Chchlee
 * Comments added
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 21    30.12.04 15:13 Chchlee
 * adjusted for new field "systemSettings" in system configuration
 * 
 * 20    6.12.04 18:36 Chmibuh
 * Prepared for Emergency FW
 * 
 * 19    15.11.04 8:36 Leeb02
 * function call in ASSERT() removed
 * 
 * 18    18.08.04 17:33 Leeb02
 * now using TDM state
 * 
 * 17    2.08.04 9:34 Leeb02
 * Naming harmonized
 * 
 * 16    30.07.04 11:09 Leeb02
 * Configuration access added
 * 
 *-------------------------------------------------------------------------*/

// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION
//
// If you add something, either if you write new code or comment-in previously commented-out 
// code fragments than bracket it with 
// #ifndef EMERGENCY_FW
//		"your code"
// #endif
//
// This prevents the emergency FW from needless code. The size of emergency FW is limited!!!
// This belongs to msgHandler.c, bridge.c, SPI_functions.c and Init_Master_DSP only.
//
// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION

//#define DEBUG_FT12_BRIDGE_RULES

/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "diag.h"
#include "bridge.h"
#include "constant.h"
#include "DL_RS232.h"
#include "DL_EOC.h"
#include "DL_2nd.h"
#include "ExternalTDM_BusBridge.h"
#include "configuration.h"
#include "R1LA_Types.h"

#include "string.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#ifdef DEBUG_FT12_BRIDGE_RULES // For debugging of FT1.2 bridge rules
	#include "eventrec.h" 

	#define RECEIVED_FROM_RS232			0x00000232
	#define SEND_TO_RS232				2
	#define RECEIVED_FROM_EOC			0x00000E0C
	#define SEND_TO_EOC					4
#endif


#define P4LQ_LOCAL					0
#define TDM_CARD_SLOT_ID1_LOCAL		1
#define TDM_CARD_SLOT_ID2_LOCAL		2
#define TDM_CARD_SLOT_ID3_LOCAL		3
#define TDM_CARD_SLOT_ID4_LOCAL		4
#define TDM_CARD_SLOT_ID5_LOCAL		5
#define TDM_CARD_SLOT_ID6_LOCAL		6
#define TDM_CARD_SLOT_ID7_LOCAL		7
#define LAN_LOCAL					R1LA_SUBADDRESS		// 51 from LAN_Types.h

#define P4LQ_REMOTE					64
#define TDM_CARD_SLOT_ID1_REMOTE	65
#define TDM_CARD_SLOT_ID2_REMOTE	66
#define TDM_CARD_SLOT_ID3_REMOTE	67
#define TDM_CARD_SLOT_ID4_REMOTE	68
#define TDM_CARD_SLOT_ID5_REMOTE	69
#define TDM_CARD_SLOT_ID6_REMOTE	70
#define TDM_CARD_SLOT_ID7_REMOTE	71
#define LAN_REMOTE					(LAN_LOCAL + 64)	// 115

// For connect with remote equipment:
// The Bypass Addresse is the SubAddress + 64. This allows the Equipment
// to determine the source, if it is received by RS-232 or EOC. If received by RS-232
// the SubAddress is between P4LQ_REMOTE and LAN_REMOTE and the packets have to be forwarded via EOC. 
// The SubAddress has to be exchanged with the Bypass Address (+64). 
// If received by EOC the SubAddress is between P4LQ_BYPASS and LAN_BYPASS. The forwarding port depends on
// the type of board (e.g. for LAN600 forward to RS-232). Before forwarding the SubAddress has to be restored,
// -64 for P4LQ and TDM cards. For the LAN600 you have to substract 128 because the LAN600 is listening to 
// its SubAddress (51) only.
#define P4LQ_BYPASS					128
#define TDM_CARD_SLOT_ID1_BYPASS	129
#define TDM_CARD_SLOT_ID2_BYPASS	130
#define TDM_CARD_SLOT_ID3_BYPASS	131
#define TDM_CARD_SLOT_ID4_BYPASS	132
#define TDM_CARD_SLOT_ID5_BYPASS	133
#define TDM_CARD_SLOT_ID6_BYPASS	134
#define TDM_CARD_SLOT_ID7_BYPASS	135
#define LAN_BYPASS					(LAN_REMOTE + 64)	// 179


/*----------------------------------------------------------------------------
* Blockierung der Bridge
* -----------------------
* 
* Ein nicht funktionierender EOC kann die Bridge blockieren. Um dies zu verhindern
* muss ein Mechanismus existieren. Dies wird relativ einfach mit einem Time Out Counter
* für jeden Port realisiert.
* Der langsamste Port (Kanal), die Frame-Länge und die Aufrufhäufigkeit
* der HandleXXPort Funktionen bestimmen die Zeit, nachdem die RECEIVE Buffer
* wieder neue Frames empfangen können /müssen. 
* 
* Herleitung der mittleren Uebertragungsrate im EOC.
* ----------------------------------------
*
* 10666Hz/100 (-> da das Pilot Modem im 106 Hz SUBCASE ist )-> max. 106.66 Bit/s
* 
* Es werden LINK Daten und Fast Status übertragen. Jedes zweite Frame
* ist ein Fast Status. Dadurch ergibt sich für die Link Daten eine minimale mittlere
* Uebertragungskapazität von 53.33Bit/s  (53.3 Baud), falls die beiden Frames gleich gross
* sind. 
* 
* max. Länge für ein Fast Status Frame
* -------------------------------------
* 
* Start/Länge/Stop/Checksum= 6 Byte
* Userdaten: 5 * 32 Bit	   = 40 Byte
* Header:				   = 11 Byte
* ---------------------------------
* tot. Byte					 57 Byte
* 
* ( evt. Zusätzliche User Daten für die Entzerrersteuerung)
*
* übermittelte Bits:	57* 11 Bit +33= 660 Bit
* max. mittlere Uebertragungskapazität (ohne Link Frame) -> 106.6 Bit/s
* min. mittlere Uebertragungskapazität ( mit max. Link Frame) -> 660*1/(660 +2904) *106.6 Bit/s = 19.8 Bit/s
* 
* 			
* max. Länge für ein Fast Status Frame
* -------------------------------------
* 
* übermittelte Bits: (255+6)[Byte]*11 Bit = 2871 Bit + 33 Bit = 2904 Bit
* min. Frame (keine Userdaten)			=	(11+6) * 11 Bit	= 187 Bit
*
* max. mittlere Uebertragungskapazität,  = 2904*1/(660 +2904) = 86.9 Bit/s
* min. mittlere Uebertragungskapazität,  = 187*1/(660 +187) *106.6 Bit/s = 23.53 Bit/s
*	
* max. Uebertragungszeit für ein Frame
* ------------------------------------
*
* 
* -> 2904 [Bit] / 86.9 [Bit/s] = 33.4 s 
* 
* Time Out 
* --------
*
* Die Funktion bridge_activate wird alle 187.4us aufgerufen und jeweils ein Port behandelt.
* Der Time Out Counter in der HandleXXXPort Funktion erhöht sich dadurch alle 749.6us um eins.
* 
* PORT_TIME_OUT > 33.4/749.6us = 44557
*  
* Ein Time Out von 60s entspricht ca. 80000.
-------------------------------------------------------------------------------*/

#define N_TDM_CARD_PORTS  2
#define N_P4LT_PORTS      2

 
// Bitmaske für die Ports, welche bei der Adressenbestimmung gesetzt werden.
// Jede Port bestimmt ein Bit, so dass auch mehrere Ports aktiv sein können.
#define NONE_mask		0
#define P4LQ_mask		BIT0
#define TDM_CARD_mask	BIT1
#define RS232_mask		BIT2
#define EOC_mask		BIT3
#define DISCARD_mask	BIT4	// unfortunately NONE_mask is used for a special case 
								// so we need an additional case for LAN600 -> P4LT has to discard
								// packets if DestinationAdress == g_myAddress | LOCAL && 
								// DestinationSubadress == LAN600. 

#define LOCAL			0

#define EOC_PORT_TIME_OUT  80000     // 60s    
#define TDM_PORT_TIME_OUT   1333	 // 1s
#define P4LQ_PORT_TIME_OUT  1333 
#define RS232_PORT_TIME_OUT 1333 

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {
	long	RxFrameCounterRS232;	
	long	RxBufferOverflowRS232;
	long	TxFrameCounterRS232;
	long	TxBufferOverflowRS232;

	long	RxFrameCounterEOC;	
	long	RxBufferOverflowEOC;
	long	TxFrameCounterEOC;
	long	TxBufferOverflowEOC;

	long	RxFrameCounterP4LQ;	
	long	RxBufferOverflowP4LQ;
	long	TxFrameCounterP4LQ;
	long	TxBufferOverflowP4LQ;

	long	RxFrameCounterTDM;	
	long	RxBufferOverflowTDM;
	long	TxFrameCounterTDM;
	long	TxBufferOverflowTDM;

} t_BridgeStatus;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

// RS232 port
static t_FT12TelegramBuffer * g_pReceiveBufferRS232;              // ptr to RS232 receive buffer. The buffer is owned by DL_232.C
static t_FT12TelegramBuffer * g_pToDL_RS232Frame;                 // ptr to RS232 transmit buffer
static e_FT12State toDL_RS232State;

// EOC port
static t_FT12TelegramBuffer * g_pReceiveBufferEOCCh1;             // ptr to EOC receive buffer. The buffer is owned by DL_EOC.C
static t_FT12TelegramBuffer * g_pToDL_EOCFrame; 	              // ptr to EOC transmit buffer
static e_FT12State toDL_EOCState; 

// TDM card port message handler
static t_FT12TelegramBuffer *g_pNewTDMFrame;					  // ptr to transmit frame for TDM message handler. Points to either g_receiveBufferTDM
static t_FT12TelegramBuffer g_receiveBufferTDM[N_TDM_CARD_PORTS]; // transmit buffers for TDM card port. 
static t_FT12TelegramBuffer * pToDL_TDMCardFrame;                 // ptr to received frame which is passeed to TDM message handler 
static e_FT12State toDL_TDMCardState;				

// local message handler 
static t_FT12TelegramBuffer * g_pNewSLAVEFrame;                   // ptr to transmit frame for local message handler. Points to either g_receiveBufferP4LQ
static t_FT12TelegramBuffer g_receiveBufferP4LQ[N_P4LT_PORTS];    // transmit buffers for local message handler. 
static t_FT12TelegramBuffer * g_pToDL_SLAVEFrame;                 // ptr to received frame which is passeed to TDM message handler 
static e_FT12State toDL_secondaryStationState;

static BOOL ForwardingBufferRS232Blocked;		// do not count down hop counter if forwarding function is still busy
static BOOL ForwardingBufferEOCBlocked;

// pointer auf diese struktur muss später über eine Funktion zugänglich gemacht werden.
// "später" heisst effektiv "nie", denn wir sind jetzt bei ETL600!
static t_BridgeStatus g_BridgeStatus;

static unsigned long g_myAddress;

static BOOL g_isEOCEnabled;

/* LOCAL FUNCTIONS ***********************************************************/

static long checkAddress(BOOL RS232_Active, t_FT12TelegramBuffer *);
static void HandleP4LQPort();
static void HandleRS232Port();
static void HandleEOCPort();
static void HandleTDMCardPort();
static BOOL receiveBuffersEmpty(void);

/* IMPLEMENTATION ************************************************************/


/*******************************************************************************
*	Die Routing Information P4LQ_UP und TDM_CARD_UP ermöglichen der Bridge 
*	das Antworttelegramm von der Sekundärstation (P4LQ oder einer TDM Karte ) 
*	an den Port zu schicken, von welchem die Anfrage kam. Die Ports der Bridge
*	auf der Sekundärstation werden dadurch nicht "geflooded". Dies würde 
*	geschehen, wenn diese Bridge nur die Destination Address überprüfen würde 
*	und keine Adresslisten für das Forwarding unterhält. 
*	z.B. Ein Zugriff mit dem PC (Primärstation) direkt an der COM1 Schnittstelle 
*	des P4LQ auf die lokale Station (Sekundärstation würde eine Antwort über die 
*	Ports RS232 und EOC erzeugen.
* 
*	Durch die Routing Information müss jedoch kein Fluten (flooding) unterstützt 
*	werden, welches unnötig das Netz verstopft und speziell über den langsamen 
*	EOC Port. Die Bridge mit den zwei extern Ports RS232 und EOC braucht das Fluten
*	gar nicht. Ein Telegramm wird entweder rausgefiltert oder an den anderen Port 
*	weitergeleitet.
*	 
*  
********************************************************************************/

void SEGMENT_EXT_CODE bridge_init(void)
{
	BOOL bSuccess; 

#ifdef EMERGENCY_FW	
	bSuccess = init_DL_RS232(0,							 // This leads to default Bd-Rate of 57600 Bd
		                     &g_pReceiveBufferRS232,     // get ptr to RS232 receive buffer
					         &g_pToDL_RS232Frame,        // pass ptr to RS232 transmit 
					         &toDL_RS232State);

	if (!bSuccess)
	{
		*(long*)(MSGR3) = 1000;	// try again be rebooting P4LT
	}

#else
	g_isEOCEnabled = configuration_getPCurrentConfig()->system.systemSettings & SYSTEM_SETTINGS_ENABLE_EOC;
	g_myAddress    = configuration_getPCurrentConfig()->system.equipmentInfo.address;	

	bSuccess = init_DL_RS232(configuration_getPCurrentConfig()->system.HMIBaudrate, 
		                     &g_pReceiveBufferRS232,     // get ptr to RS232 receive buffer
					         &g_pToDL_RS232Frame,        // pass ptr to RS232 transmit 
					         &toDL_RS232State);


	if (!bSuccess)
	{
		resetP4LT(); // try again be rebooting P4LT
		// does not return from resetP4LT
	}
#endif

	initDL_2nd(receiveBuffersEmpty,
		       &g_pToDL_SLAVEFrame,      // pass ptr to ptr to received frame (i.e. frame to be processed by DL_2nd)
			   &g_pNewSLAVEFrame,        // pass ptr to ptr to be transmitted frame (i.e. frame to be filled by DL_2nd)
		       &toDL_secondaryStationState);

#ifndef EMERGENCY_FW	
	initDataLinkLayerEOC(&g_pReceiveBufferEOCCh1, // get ptr to EOC receive buffer
		                 &g_pToDL_EOCFrame,
						 &toDL_EOCState);

    intExtTDMBusBridge_init(&pToDL_TDMCardFrame,     // pass ptr to received frame (frame which is sent to the local TDM card)
		                    &g_pNewTDMFrame,         // pass pass ptr to ptr to be transmitted frame (i.e. frame to be filled by TDM card)
						    &toDL_TDMCardState);     // pass ptr to TDM card state
#endif

   	// Pointer haben definierte Adressen, und sicher nie 0 !	
	g_pToDL_RS232Frame	= g_pReceiveBufferRS232;
	g_pToDL_EOCFrame	= g_pReceiveBufferRS232; 	
	g_pToDL_SLAVEFrame	= g_pReceiveBufferRS232;

	toDL_secondaryStationState  = e_FT12State_Idle;
	toDL_TDMCardState           = e_FT12State_Idle;
	toDL_RS232State             = e_FT12State_Idle;
	toDL_EOCState               = e_FT12State_Idle;

	g_pReceiveBufferRS232->State    = e_FT12State_Empty;
	g_receiveBufferP4LQ[0].State	= e_FT12State_Empty;
	g_receiveBufferP4LQ[1].State	= e_FT12State_Empty;

	g_receiveBufferTDM[0].State	= e_FT12State_Empty;
	g_receiveBufferTDM[1].State	= e_FT12State_Empty;

	// note: g_pReceiveBufferEOCCh1->State is initialized in DL_EOC.c

	g_pNewSLAVEFrame   = &g_receiveBufferP4LQ[0];
    g_pNewTDMFrame     = &g_receiveBufferTDM[0];
}



/*---------------------------------------------------------------------------
* void bridge_activate 
* Verwaltet die Receive Port und füllt die Transmit Ports mit Daten.
* Bezeichnungen für Empfangen und Senden sind aus der Sicht der Bridge gewählt.
* z.B. Die Bridge empfängt das ganze Paket vom RS232 - Port (g_pReceiveBufferRS232)
* und "sendet" es an den P4LQ - Port. (ReceivedFrame für den P4LQ). (-> Store and forward).

------------------------------------------------------------------------------*/
void SEGMENT_EXT_CODE bridge_activate(void)
{ 
	// Zustände der Bridge fuer die Antworttelegramme der Sekundarstationen,
    // welche in P4LQ_UP und TDM_CARD_UP gespeichert sind
	static enum 
	{
		P4LQ, 
		TDM_CARD, 
		RS232,
		EOC    
	} state = RS232;

	switch (state)
	{
		case RS232:		HandleRS232Port(); 
						state = P4LQ;				
						break;

		case P4LQ:		HandleP4LQPort();

#ifdef EMERGENCY_FW	// process only P4LQ and RS232 in case of emergency firmware
						state = RS232; 
						break;
#else
						state = TDM_CARD;
						break;

		case TDM_CARD:	HandleTDMCardPort();
						state = EOC;
						break;

		case EOC:		HandleEOCPort();
						state = RS232;
						break;
#endif
	}			
}	

/*---------------------------------------------------------------------------
* void HandleRS232Port
*	
* Der Zustand des Antworttelegramms g_pReceiveBufferRS232.State
* wird in den folgenden Funktionen geändert.
*  
* ReceiveBuffers232.State	:
*1. Fall FORWARDED:			EMPTY->FULL->	FORWARDED->	SENDING->TRANSMITTED->	EMPTY
*2. Fall Frame für P4LQ:								EXECUTED------------->	EMPTY		
*															
*																		
*--------------------------------------------------------------------------------
*	InitControlBridge		:	x	|		|			|		|		|
*	FromDataLinkLayerRS232	:		|	x	|			|		|		|
*	HandleRS232Port()		:		|		|	x		|		|		|	x
*	ToDataLinkLayerEOC()	:		|		|			|	x	|   x	|
*		oder						|		|			|		|		|
*   AddSoftTimingBit()		:		|		|			|	x	|   x	|
*		oder				:		|		|			|		|
*	ApplicationLayer		:		|		|			|	x--------------- x

------------------------------------------------------------------------------*/

static void SEGMENT_EXT_CODE HandleRS232Port()
{
	static long TimeOutCntrRS232;
	static long TimeOutRS232     = P4LQ_PORT_TIME_OUT;

	static BOOL EOC_ForwardingActive;

	if (TimeOutCntrRS232 > TimeOutRS232) 
	{
		g_pReceiveBufferRS232->State = e_FT12State_Transmitted;
	}

	switch (g_pReceiveBufferRS232->State)
	{
		case e_FT12State_Empty:		break;
			
		case e_FT12State_Full:		// received a telegram from RS232
		{

			#ifdef DEBUG_FT12_BRIDGE_RULES // For debugging of FT1.2 bridge rules
			
				if(((g_pReceiveBufferRS232->MessageType & 0xFFFF) != 0x0158) &&
				   ((g_pReceiveBufferRS232->MessageType & 0xFFFF) != 0x0107) &&
				   ((g_pReceiveBufferRS232->MessageType & 0xFFFF) != 0x0108) &&
				   ((g_pReceiveBufferRS232->MessageType & 0xFFFF) != 0x0109) &&
				   ((g_pReceiveBufferRS232->MessageType & 0xFFFF) != 0x010A) &&
				   ((g_pReceiveBufferRS232->MessageType & 0xFFFF) != 0x0133)
					)
				{
				
					eventRec_recordEvent(ALARM_EVENT_DEBUG4, RECEIVED_FROM_RS232, FALSE); 
					eventRec_recordEvent(ALARM_EVENT_DEBUG5, ((g_pReceiveBufferRS232->SourceAdress <<16) | (g_pReceiveBufferRS232->DestinationAdress)), FALSE); 
					eventRec_recordEvent(ALARM_EVENT_DEBUG5, ((g_pReceiveBufferRS232->DestinationSubadress <<16) | (g_pReceiveBufferRS232->SequenceNumber)), FALSE); 
				}

			#endif

			long ForwardedPort = checkAddress(TRUE, // RS232 active 
											  g_pReceiveBufferRS232);
			TimeOutCntrRS232++;
			g_BridgeStatus.RxFrameCounterRS232++;
			
			if(DISCARD_mask == ForwardedPort)
			{
				g_pReceiveBufferRS232->State = e_FT12State_Transmitted; // No forwarding over EOC, its for LAN600
				break;
			}

			if (NONE_mask == ForwardedPort)
			{
				if (g_isEOCEnabled && (g_pReceiveBufferRS232->HopCounter > 0))
				{
					ForwardedPort = EOC_mask;
				}
				else 
				{	
					g_pReceiveBufferRS232->State = e_FT12State_Transmitted; // No forwarding over EOC, alternative route available
				}
			}	
			
			if (ForwardedPort & P4LQ_mask)   // forward to local message handler...
			{						
				if (toDL_secondaryStationState == e_FT12State_Idle) // ... if idle
				{									
					g_BridgeStatus.TxFrameCounterP4LQ++;

					toDL_secondaryStationState = e_FT12State_Forwarded;
					g_pToDL_SLAVEFrame = g_pReceiveBufferRS232;
					
					g_pReceiveBufferRS232->State = e_FT12State_Forwarded;
					g_pNewSLAVEFrame = &g_receiveBufferP4LQ[0];    // response telegram will be in this buffer

					TimeOutRS232 = P4LQ_PORT_TIME_OUT;	
				}
				return;
			}


			if (ForwardedPort & TDM_CARD_mask)	
			{
				if ((toDL_TDMCardState == e_FT12State_Idle) && (toDL_RS232State == e_FT12State_Idle))	// lokal TDM card
				{	
					g_BridgeStatus.TxFrameCounterTDM++;

					toDL_TDMCardState  = e_FT12State_Forwarded;

					pToDL_TDMCardFrame = g_pReceiveBufferRS232;

					g_pReceiveBufferRS232->State = e_FT12State_Forwarded;
					
					g_pNewTDMFrame = &g_receiveBufferTDM[0]; // use buffer 0 for RS232

					TimeOutRS232 = TDM_PORT_TIME_OUT;
				}
				return;
			}
			
									
			if (ForwardedPort & EOC_mask)			// forwarded to EOC Port 
			{
				
				if (toDL_EOCState == e_FT12State_Idle) 
				{
					g_BridgeStatus.TxFrameCounterEOC++;
					toDL_EOCState = e_FT12State_Forwarded;

					g_pToDL_EOCFrame = g_pReceiveBufferRS232;
					
					g_pReceiveBufferRS232->State = e_FT12State_Forwarded;
					
					TimeOutRS232 = EOC_PORT_TIME_OUT;
					EOC_ForwardingActive = TRUE;
					ForwardingBufferEOCBlocked = FALSE;
				}
				else 
				{
					ForwardingBufferEOCBlocked = TRUE;
				}
				return;
			}
			break;
		}
		
	    case e_FT12State_Forwarded:	
		case e_FT12State_Executed:	
		case e_FT12State_Sending:	
						TimeOutCntrRS232++;
						break;
		
		case e_FT12State_Transmitted: 
			            g_pReceiveBufferRS232->State = e_FT12State_Empty;

						// Quick'n dirty bug removal blocking EOC forwarding
						if (EOC_ForwardingActive)
						{
							EOC_ForwardingActive = FALSE;
							toDL_EOCState = e_FT12State_Idle;
						}

						TimeOutCntrRS232 = 0;
						break;
	}
}



/*---------------------------------------------------------------------------
* void HandleEOCPort()
*	
* Der Zustand des Antworttelegramms g_pReceiveBufferEOCCh1.State
* wird in den folgenden Funktionen geändert.
*  
* g_pReceiveBufferEOCCh1.State	:
*1. Fall FORWARDED:			EMPTY->FULL->	FORWARDED->	SENDING->TRANSMITTED->	EMPTY
*2. Fall Frame für P4LQ:								EXECUTED------------->	EMPTY		
*															
*																		
*-------------------------------------------------------------------------------------
*	InitControlBridge		:	x	|		|			|		|			|
*	FromDataLinkLayerEOC	:		|	x	|			|		|			|
*	HandleEOCPort()			:		|		|	x		|		|			|	x
*	ToDataLinkLayerRS232()	:		|		|			|	x	|   x		|
*		oder						|		|			|		|			|
*   AddSoftTimingBit()		:		|		|			|	x	|   x		|
*		oder				:		|		|			|		|
*	ApplicationLayer		:		|		|			|	x---------------	x

------------------------------------------------------------------------------*/

static void SEGMENT_EXT_CODE HandleEOCPort()
{
 	static long TimeOutCntrEOC  = 0;
	static long TimeOutEOC      = P4LQ_PORT_TIME_OUT;

	if (TimeOutCntrEOC > TimeOutEOC) 
	{
		g_pReceiveBufferEOCCh1->State = e_FT12State_Transmitted;
	}

	switch (g_pReceiveBufferEOCCh1->State)
	{
		case e_FT12State_Empty:		break;

		case e_FT12State_Full:		
		{
			long ForwardedPort = checkAddress(FALSE, // no RS232
											  g_pReceiveBufferEOCCh1);

			#ifdef DEBUG_FT12_BRIDGE_RULES // For debugging of FT1.2 bridge rules
			
				if(((g_pReceiveBufferEOCCh1->MessageType & 0xFFFF) != 0x0158) &&
				   ((g_pReceiveBufferEOCCh1->MessageType & 0xFFFF) != 0x0107) &&
				   ((g_pReceiveBufferEOCCh1->MessageType & 0xFFFF) != 0x0108) &&
				   ((g_pReceiveBufferEOCCh1->MessageType & 0xFFFF) != 0x0109) &&
				   ((g_pReceiveBufferEOCCh1->MessageType & 0xFFFF) != 0x010A) &&
				   ((g_pReceiveBufferEOCCh1->MessageType & 0xFFFF) != 0x0133)
					)
				{
					eventRec_recordEvent(ALARM_EVENT_DEBUG4, RECEIVED_FROM_EOC, FALSE); 
					eventRec_recordEvent(ALARM_EVENT_DEBUG5, ((g_pReceiveBufferEOCCh1->SourceAdress <<16) | (g_pReceiveBufferEOCCh1->DestinationAdress)), FALSE); 
					eventRec_recordEvent(ALARM_EVENT_DEBUG5, ((g_pReceiveBufferEOCCh1->DestinationSubadress <<16) | (g_pReceiveBufferEOCCh1->SequenceNumber)), FALSE); 
				}

			#endif

			TimeOutCntrEOC++;
			g_BridgeStatus.RxFrameCounterEOC++;

			if(DISCARD_mask == ForwardedPort)
			{
				g_pReceiveBufferEOCCh1->State = e_FT12State_Transmitted; // No forwarding over RS232, its for LAN600
				break;
			}

			if (NONE_mask == ForwardedPort)
			{
				if (g_isEOCEnabled)
				{
					ForwardedPort = RS232_mask;
				}
				else 
				{
					g_pReceiveBufferEOCCh1->State = e_FT12State_Transmitted; // No forwarding over RS232, alternative route available
					break;
				}
			}

			if (ForwardedPort & P4LQ_mask)   // forward to local message handler
			{
				if ((toDL_secondaryStationState == e_FT12State_Idle) && (toDL_EOCState == e_FT12State_Idle))
				{									
					g_BridgeStatus.TxFrameCounterP4LQ++;

					toDL_secondaryStationState = e_FT12State_Forwarded;
					
					g_pToDL_SLAVEFrame = g_pReceiveBufferEOCCh1;   							
					
					g_pReceiveBufferEOCCh1->State = e_FT12State_Forwarded;
					
					g_pNewSLAVEFrame = &g_receiveBufferP4LQ[1]; // use buffer 1 for EOC
					
					TimeOutEOC = P4LQ_PORT_TIME_OUT;
				}
				return;						
			}

			if (ForwardedPort & TDM_CARD_mask)	// forward to local TDM card
			{
				if ((toDL_TDMCardState == e_FT12State_Idle) && (toDL_EOCState == e_FT12State_Idle))		// local TDM card
				{	
					g_BridgeStatus.TxFrameCounterTDM++;

					toDL_TDMCardState = e_FT12State_Forwarded;

					pToDL_TDMCardFrame = g_pReceiveBufferEOCCh1;

					g_pReceiveBufferEOCCh1->State = e_FT12State_Forwarded;

					g_pNewTDMFrame = &g_receiveBufferTDM[1]; // use buffer 1 for EOC

					TimeOutEOC = TDM_PORT_TIME_OUT;
				}
				return;
			}
			
									
			if (ForwardedPort & RS232_mask)			// forward to RS-232 Port 
			{
				
				if (toDL_RS232State == e_FT12State_Idle) 
				{
					g_BridgeStatus.TxFrameCounterRS232++;
					toDL_RS232State = e_FT12State_Forwarded;
					g_pToDL_RS232Frame = g_pReceiveBufferEOCCh1;
					g_pReceiveBufferEOCCh1->State = e_FT12State_Forwarded;
					TimeOutEOC = RS232_PORT_TIME_OUT;
					ForwardingBufferRS232Blocked = FALSE;
					return;
				}
				else 
				{
					ForwardingBufferRS232Blocked = TRUE;
					return;
				}
			}
		}						
						break;

		case e_FT12State_Forwarded:		
		case e_FT12State_Executed:	
		case e_FT12State_Sending:	
			            TimeOutCntrEOC++;
						break;

		case e_FT12State_Transmitted: 
			            g_pReceiveBufferEOCCh1->State = e_FT12State_Empty;
						TimeOutCntrEOC = 0;
						break;
	
	}

}


/*---------------------------------------------------------------------------
* void HandleP4LQPort
*
* Der Zustand des Antworttelegramms g_receiveBufferP4LQ[i].State
* wird in den folgenden Funktionen geändert.
*  
* g_receiveBufferP4LQ[i].State	:	  EMPTY->FULL->	FORWARDED->	SENDING->	TRANSMITTED->EMPTY
*-------------------------------------------------------------------------------------------
*	InitControlBridge		:		x	|		|			|			|			|
*	ApplicationLayer		:			|	x	|			|			|			|
*	HandleP4LQPort			:			|		|	x		|			|			|	x
*	ToDataLinkLayerRS232	:			|		|			|	x		|	x		|
*	ToDataLinkEOC:			:			|		|			|	x		|	x		|

*
*
* Der Zustand der Sekundärstation  toDL_secondaryStationState, welche
* das Antworttelegramms erstellt, wird in den folgenden Funktionen geändert.
*  
* toDL_secondaryStationState :	 IDLE	->FORWARDED -> EXECUTED	->	 FULL -> IDLE
*--------------------------------------------------------------------------------------------
*	InitControlBridge		:		x	|			|			|			|
*	HandleRS232Port()		:			|	x		|			|			|
*	HandleEOCPort()			:			|	x		|			|			|
*	ApplicationLayer		:			|			|	x		|	x		|
*	HandleP4LQPort			:			|			|			|			|	x
*
*
------------------------------------------------------------------------------*/

static void SEGMENT_EXT_CODE HandleP4LQPort()
{
	int i;

	for (i = 0; i < N_P4LT_PORTS; i++)
	{
		static long TimeOutCntrP4LQ[N_P4LT_PORTS] = { 0,                   0};
		static long TimeOutP4LQ[N_P4LT_PORTS]     = { RS232_PORT_TIME_OUT, EOC_PORT_TIME_OUT };
		static BOOL EOC_ForwardingActive          = FALSE;

		if (TimeOutCntrP4LQ[i] > TimeOutP4LQ[i]) g_receiveBufferP4LQ[i].State = e_FT12State_Transmitted;

		switch (g_receiveBufferP4LQ[i].State)
		{
			case e_FT12State_Empty:		break;

			case e_FT12State_Full:		
				            TimeOutCntrP4LQ[i]++;
							g_BridgeStatus.RxFrameCounterP4LQ++;

							if (( i == 0 ) && (toDL_RS232State == e_FT12State_Idle))
							{ 										
								g_BridgeStatus.TxFrameCounterRS232++;
								toDL_RS232State = e_FT12State_Forwarded;
								g_pToDL_RS232Frame = &g_receiveBufferP4LQ[i];										
								g_receiveBufferP4LQ[i].State = e_FT12State_Forwarded;										
							}
							

							if (( i == 1) && (toDL_EOCState == e_FT12State_Idle))
							{ 										
								g_BridgeStatus.TxFrameCounterEOC++;
								toDL_EOCState = e_FT12State_Forwarded;
								g_pToDL_EOCFrame = &g_receiveBufferP4LQ[i];										
								g_receiveBufferP4LQ[i].State = e_FT12State_Forwarded;
								EOC_ForwardingActive = TRUE;
											
							}
							break;

			case e_FT12State_Forwarded:	
			case e_FT12State_Executed:	
			case e_FT12State_Sending:	
				            TimeOutCntrP4LQ[i]++;
							break;

			case e_FT12State_Transmitted:
				           // toDL_secondaryStationState = IDLE; // d.h. die Sekundärstation (P4LQ)
																// darf erst nach dem Uebermitteln des
																// Frames wieder ein neues generieren.
	
							g_receiveBufferP4LQ[i].State = e_FT12State_Empty;
							TimeOutCntrP4LQ[i] = 0;

							// Quick'n dirty bug removal blocking EOC forwarding
							if ((EOC_ForwardingActive) && (i == 1))
							{
								EOC_ForwardingActive = FALSE;
								toDL_EOCState = e_FT12State_Idle;
							}
							break;
		}
	}
}


/*---------------------------------------------------------------------------
* void HandleTDMCardPort
*
* Der Zustand des Antworttelegramms g_pNewTDMFrame->State
* wird in den folgenden Funktionen geändert.
*  
* g_pNewTDMFrame->State	:	  EMPTY->FULL->		FORWARDED->	EXECUTED->	EMPTY
*--------------------------------------------------------------------------------
*	InitControlBridge		:		x	|			|			|			|
*	FromDataLinkLayerRS232	:			|	x		|			|			|
*	HandleRS232Port()		:			|			|	x		|			|
*	ApplicationLayer:		:			|			|			|	x		|	x

* Der Zustand der Sekundärstation  toDL_TDMCardState, welche
* das Antworttelegramms erstellt, wird in den folgenden Funktionen geändert.
*  
* toDL_secondaryStationState :	 IDLE	->FORWARDED->EXECUTED	->	FULL->	 IDLE
*--------------------------------------------------------------------------------------------
*	InitControlBridge		:		x	|			|			|			|
*	HandleRS232Port()		:			|	x		|			|			|
*	HandleEOCPort()			:			|	x		|			|			|
*	ApplicationLayer		:			|			|	x		|	x		|
*	HandleTDMCardPort		:			|			|			|			|	x
*
------------------------------------------------------------------------------*/

static void SEGMENT_EXT_CODE HandleTDMCardPort()
{
	static long TimeOutCntrTDMresponse;
	
	static long TimeOutCntrTDM       [N_TDM_CARD_PORTS];
	static long TimeOutTDM           [N_TDM_CARD_PORTS] = { RS232_PORT_TIME_OUT, EOC_PORT_TIME_OUT};
	static long toDL_TDMCardStateIdle[N_TDM_CARD_PORTS];

	int i;

	for (i = 0; i < N_TDM_CARD_PORTS; i++)
	{	
		long * tempTDM;

		if ( TimeOutCntrTDM[i] > TimeOutTDM[i] ) 
		{
			g_receiveBufferTDM[i].State = e_FT12State_Transmitted;
		}

		switch (g_receiveBufferTDM[i].State)
		{
			case e_FT12State_Empty:		

				if (toDL_TDMCardState != e_FT12State_Idle) 
				{
					TimeOutCntrTDMresponse++;                       // Ueberprüfe Kommunikation mit der TDM Card 
				}

				if ( TimeOutCntrTDMresponse > TDM_PORT_TIME_OUT )	// keine Antwort von der TDM -
				{												    // Baugruppe, darum Kommunikation
					toDL_TDMCardState = e_FT12State_Idle;		    // vom P4LT zur TDM Card wiederzu-
					TimeOutCntrTDMresponse = 0;						// lassen.
				}
				
				break;
							
			case e_FT12State_Full:	
				
				if (0 == toDL_TDMCardStateIdle[i])		   // Der Zustand toDL_TDMCardState wird nur
				{										   // einmal zurückgesetzt. Ein Frame, welches
					TimeOutCntrTDMresponse = 0;			   // in diesem Case aufs Senden warten muss, stört somit
					toDL_TDMCardState = e_FT12State_Idle;  // die Kommunikation über den andern Port nicht.
					toDL_TDMCardStateIdle[i] = 1;		   // i= 0->  RS232 i= 1 -> EOC. 
				}
				
				
				TimeOutCntrTDM[i]++;

				g_BridgeStatus.RxFrameCounterTDM++;
			
				if ((0 == i ) && (e_FT12State_Idle == toDL_RS232State))
				{ 										
					g_BridgeStatus.TxFrameCounterRS232++;
					toDL_RS232State = e_FT12State_Forwarded; 
					g_pToDL_RS232Frame = &g_receiveBufferTDM[i];										
					g_receiveBufferTDM[i].State  = e_FT12State_Forwarded;
			
					// g_pToDL_RS232Frame->State = e_FT12State_Forwarded;
				}
					
				if ((1 == i) && (e_FT12State_Idle == toDL_EOCState))
				{ 																	
					g_BridgeStatus.TxFrameCounterEOC++;
					toDL_EOCState = e_FT12State_Forwarded; 
					g_pToDL_EOCFrame = &g_receiveBufferTDM[i];										
					g_receiveBufferTDM[i].State  = e_FT12State_Forwarded;
				
				// g_pToDL_EOCFrame->State = FORWARDED;
				}							
				
				break;

			case e_FT12State_Executed:	
			case e_FT12State_Forwarded:				
			case e_FT12State_Sending:	
				TimeOutCntrTDM[i]++;
				break;

			case e_FT12State_Transmitted:	
				g_receiveBufferTDM[i].State = e_FT12State_Empty;
				TimeOutCntrTDM[i] = 0;
				toDL_TDMCardStateIdle[i] = 0;  
			
				break;
		}
	}
}


/*---------------------------------------------------------------------------
* long checkAddress(long RS232_Active, t_FT12TelegramBuffer *ReceiveBuffer)
* 
* - bestimmt die Ports, an welche das Packet weitergeleitet wird.
* - jedes Bit entspricht einem Port (z.B.P4LQ_mask ) welche OR verknüpft werden
* - entspricht Control Bridge 0.5-s1- STD0001.DAT
* - unterstützt eine statische Bridge, die einzelnen Ports RS232, EOC, NET können 
*   einzeln konfiguriert werden:
*
*		- RS232 oder EOC abschalten, falls loops auftreten und keine Netzwerkkarte am NET Port angehängt ist.
*
*		- RS232 und EOC abschalten, falls eine Netzwerkkarte am NET Port angehängt ist.
*
* - könnte zu einer learning Bridge erweitert werden -> forwarding database 
*
* P4LQ-P4LQ Kommunikation Adress == LOCAL und P4LQ
------------------------------------------------------------------------------*/

static long SEGMENT_EXT_CODE checkAddress(BOOL RS232_Active, t_FT12TelegramBuffer *ReceiveBuffer)
{
	long port;	
	BOOL BypassAddressField = FALSE;		
	
	if (ReceiveBuffer->DestinationSubadress >= 128) 
	{
		BypassAddressField = TRUE;
	}

	if  ((ReceiveBuffer->DestinationAdress == LOCAL)       || // telegram for me?
		 (ReceiveBuffer->DestinationAdress == g_myAddress) || 
		 (BypassAddressField))
	{
		switch (ReceiveBuffer->DestinationSubadress)
		{
			//------------------------- Local Equipment Address -------------------------------------------
			case P4LQ_LOCAL:							// RULE 6, see TN-PTUKT06-1537
				if(ReceiveBuffer->SourceAdress < FT12_MIN_HMI_ADDRESS) // Source is LAN and not HMI; This packet is a response of LAN
				{
					// RULE 8, see TN-PTUKT06-1537 
					if(ReceiveBuffer->ControlField == TXFUNCTION_RESPOND_USER_DATA) // Respond Packet of ETL500
					{
						if(RS232_Active)			// received from RS-232
						{
							return(DISCARD_mask);	// Discard these packets;
						}
						else
						{
							return(NONE_mask);		// forward to RS-232
						}						
					}
					
					ReceiveBuffer->DestinationSubadress = LAN_LOCAL;// LAN will now accept the response
					SetSubAddressLAN600(ReceiveBuffer);
				}
				return(P4LQ_mask);	// forwarded to P4LQ
			
			case TDM_CARD_SLOT_ID1_LOCAL:						// forwarded to TDM-CARD										
			case TDM_CARD_SLOT_ID2_LOCAL:										
			case TDM_CARD_SLOT_ID3_LOCAL:										
			case TDM_CARD_SLOT_ID4_LOCAL:	
			case TDM_CARD_SLOT_ID5_LOCAL:	
			case TDM_CARD_SLOT_ID6_LOCAL:	
			case TDM_CARD_SLOT_ID7_LOCAL:	return(TDM_CARD_mask);
			case LAN_LOCAL:					
				if(RS232_Active)			// received from RS-232
				{
					return(DISCARD_mask);	// Discard these packets; LAN is connected to RS-232
											// RULE 3, see TN-PTUKT06-1537
				}
				else						// received from EOC
				{
					
					ReceiveBuffer->DestinationSubadress = LAN_BYPASS; 
					SetSubAddressLAN600Bypass(ReceiveBuffer);
					return(NONE_mask);		// forward to RS-232
				}							// RULE 4, see TN-PTUKT06-1537

			//-------------------------- Remote Equipment Address -----------------------------------------
			// SetAddressBypass will increase SubAddress by 64. This allows the remote Equipment
			// to determine the source, if it is received by RS-232 or EOC. If received by RS-232
			// the SubAddress is between P4LQ_REMOTE and LAN_REMOTE and the packets have to be forwarded via EOC. 
			// Otherwise the Addresses are + 64 each.
		
			case P4LQ_REMOTE:
			case TDM_CARD_SLOT_ID1_REMOTE:														
			case TDM_CARD_SLOT_ID2_REMOTE:										
			case TDM_CARD_SLOT_ID3_REMOTE:										
			case TDM_CARD_SLOT_ID4_REMOTE:	
			case TDM_CARD_SLOT_ID5_REMOTE:	
			case TDM_CARD_SLOT_ID6_REMOTE:	
			case TDM_CARD_SLOT_ID7_REMOTE:	
			case LAN_REMOTE:				SetAddressBypass(ReceiveBuffer);// RULE 1, see TN-PTUKT06-1537
											return(EOC_mask);// forwarded to EOC

			//-------------------------- Bypass Equipment Address -----------------------------------------
			// Packets with these Bypass SubAddresses are forwarded by the remote Equipment via EOC. Now we have
			// to restore the original SubAddress (- 64). This is done by ClearAddressBypass. The forwarding port
			// depends on the board type. 

			case P4LQ_BYPASS:				ClearAddressBypass(ReceiveBuffer);// Clear Bypass // damit das Antwort telegramm an den 
																						// an den PC zurückgelangt, der auch eine Adresse hat
											return(P4LQ_mask);

			case TDM_CARD_SLOT_ID1_BYPASS:														
			case TDM_CARD_SLOT_ID2_BYPASS:										
			case TDM_CARD_SLOT_ID3_BYPASS:										
			case TDM_CARD_SLOT_ID4_BYPASS:	
			case TDM_CARD_SLOT_ID5_BYPASS:	
			case TDM_CARD_SLOT_ID6_BYPASS:	
			case TDM_CARD_SLOT_ID7_BYPASS:	ClearAddressBypass(ReceiveBuffer);// Clear Bypass
											return(TDM_CARD_mask);	// forwarded to TDM Card

											
			case LAN_BYPASS:				// RULE 2, see TN-PTUKT06-1537
											if(RS232_Active)			// received from RS-232
											{
												return(DISCARD_mask);	// Discard these packets; LAN is connected to RS-232
																		// 
											}
											else						// received from EOC
											{
												ReceiveBuffer->DestinationAdress = g_myAddress;
												ChangeDestinAddressLAN600(ReceiveBuffer);// Do not clear Bypass, change Destination Address instead
												return(RS232_mask);		// forward to RS232 (LAN)
											}
			default:				 	
				return(NONE_mask);
		}
	}
	else // telegram not for me -> broadcast
	{
		port = NONE_mask;

		// This filters respond packets from LAN to HMI -> discard them, otherwise they will be forwarded via EOC
		// RULE 5, see TN-PTUKT06-1537
		if ((ReceiveBuffer->DestinationSubadress == LAN_LOCAL) && ((ReceiveBuffer->SourceAdress == LOCAL) || (ReceiveBuffer->SourceAdress == g_myAddress)) )
		{
			port = DISCARD_mask;
		}
		else if(ReceiveBuffer->SourceAdress == ReceiveBuffer->DestinationAdress) // RULE 7, see TN-PTUKT06-1537
		{
			port = DISCARD_mask;   
		}

		// Bug EOC disabled, answer must go to RS232 
		// This workaround from the past disturbs the communication to a remote LAN
		// Therefore the exception (ReceiveBuffer->DestinationSubadress != LAN_REMOTE) was added
		if ((ReceiveBuffer->DestinationSubadress >= P4LQ_REMOTE) && (ReceiveBuffer->DestinationSubadress != LAN_REMOTE)) 
		{
			port = RS232_mask;
		}

		if ((!RS232_Active) && (!ForwardingBufferRS232Blocked))
		{
			ChangeHopCounter(ReceiveBuffer);
		}

		if ((RS232_Active) && (!ForwardingBufferEOCBlocked))
		{
			ChangeHopCounter(ReceiveBuffer);
		}

		return(port);
	}
	
}

static BOOL SEGMENT_EXT_CODE receiveBuffersEmpty(void)
{
	return (g_receiveBufferP4LQ[0].State == e_FT12State_Empty) && 
		   (g_receiveBufferP4LQ[1].State == e_FT12State_Empty);
}





