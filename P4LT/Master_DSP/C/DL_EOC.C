/*-------------------------------------------------------------------------*
 * $Workfile: DL_EOC.C $
 * Part of	: LCPLC
 * Language	: C
 * Created by: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/Master_DSP/C/DL_EOC.C $
 * 
 * 12    28.08.06 16:35 Chchlee
 * Some code moved to external RAM to free internal RAM
 * 
 * 11    28.08.06 16:33 Chchlee
 * Some code moved to external RAM to free internal RAM
 * 
 * 8     5.10.05 14:10 Chhamaa
 * EOC debug code removed.
 * 
 * 7     1.09.05 16:20 Chchlee
 * dlEOL... replaced by dlEOC...
 * 
 * 6     25.07.05 11:25 Chhamaa
 * Debug code added
 * 
 * 5     18.07.05 16:43 Chchlee
 * ASSERTs added in default branch of state machines
 * 
 * 4     1.07.05 9:39 Chchlee
 * Cleaned up
 * 
 * 3     30.06.05 8:14 Chchlee
 * cleaned up
 * 
 * 2     17.02.05 17:11 Chchlee
 * Improved monitoring concept for EOC
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 26    13.01.05 12:57 Chhamaa
 * deviceState[] changed  to  deviceState
 * 
 * 25    3.01.05 11:36 Chchlee
 * 
 * 24    30.12.04 12:18 Chchlee
 * DL_EOC_Monitor implemented for 3 PLC channels
 * 
 * 23    27.12.04 12:40 Chchlee
 * prepared for new monitoring concept
 * 
 * 22    17.12.04 17:21 Chhamaa
 * 
 * 21    16.12.04 14:17 Chchlee
 * transition to new status structure
 * 
 * 20    23.11.04 14:15 Maag01
 * 
 * 19    22.11.04 16:23 Leeb02
 * UpdateFastStatus updated for new P4LT_Status type
 * 
 * 18    15.11.04 8:04 Leeb02
 * Status object added
 * 
 * 17    12.11.04 13:28 Maag01
 * Some functions moved to ext. RAM. Optimizer (for speed) activated for
 * functions in internal RAM.
 * 
 * 16    11.11.04 16:07 Maag01
 * FAST_STATUS_TEL_CH2, FAST_STATUS_TEL_CH3 removed
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include <stddef.h>

#include "constant.h"
#include "APLC_Types.h"
#include "default.h"
#include "DL_EOC.h"
#include "diag.h"
#include "status.h"
#include "monitor.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

//Länge des Synchronisationstelegrammes (Pause,Daten,Pause,Pause,Daten)
#define	LENGTH_SYNC_TELEGR				5		
	
//	
//  nach 2 Minuten erfolgt ein EOC Alarm (bei 100 Hz sample des Case MonitoringEOC)
#define EOCALARMTIME	12000
	
#define	RUHEZUSTAND		0x07FF  
//  Ruhezustand auf der Leitung-> Signal=1
#define	STARTZEICHEN	0x06D0  
// 	Startbit=0|bit 0...7(Char 68H)|Parität=1|Stopbit=1
#define	STOPZEICHEN		0x06C2  
// 	Startbit=0|bit 0...7(Char 16H)|Parität=1|Stopbit=1  
#define CONTROLLZEICHEN 0x06c8  
// 	Startbit=0|bit 0...7(Char 64H)|Parität=1|Stopbit=1
// 	Steuerfeld: (MSB)DIR=0|PRM=1|FCB=1|FCV=0|functioncode(4Bit)=4 (LSB)
                               	// 	DIR: direction
                               	//	PRM: primary massage
                               	//	FCB: frame cout bit
                               	//	FCV: frame count bit valid
                               	// 	functioncode 
                               	//  -> Festlegung des Protokolles für das Pilotmodem siehe TM 1541/A 
#define SUMME_CONTROLLZ 0x0064	
//	Summe Byte Controllzeichen 
#define	SYNC_WORD		0x02AA  
//  Synchronisationswort (möglichst viele Wechsel 0->1->0)    


#define	MASKEBIT_1_8	(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8)

#define FIRST_BYTE						0
#define SECOND_BYTE						1
#define THIRD_BYTE						2
#define FOURTH_BYTE						3

#define	START_1		0x0007
#define	START_2		0x0008
#define	START_3		0x0009
#define SYNC_TELEG	0x000a
//
// Gemäss TM-NTC-96-1541/A müssen zwischen zwei Telegrammen 33 bit Ruhezustand(=1) gesendet werden.
// 33 bit Ruhezustand auf der Leitung bevor nächstes Telegramm gesendet wird
// 11 bit pro Datenwort -> 3 Datenworte für EOC
#define	NUMBER_OF_PAUSE	3
//
// Der TxAlarmDL_EOC wird nach einer vorgegebenen Zeit ausgelöst, wenn keine Daten mehr über den EOC
// gesendet werden können (txBufferCh1.full==1). Die Funktion ToDataLinkLayerEOC() muss im DSPB in 
// einem 100Hz CASE untergebracht werden (DL_EOC_TX_ALARMTIME =100 entspricht 1 Sekunde).
// setting aktuell: 120Sek -> 2Minuten gemäss Besprechung vom 98-09-01
#define	DL_EOC_TX_ALARMTIME		12000
//
//
// Der RxAlarmDL_EOC wird nach einer vorgegebenen Zeit ausgelöst, wenn keine Daten mehr vom den EOC
// empfangen werden können (rxBufferCh1.full==0). Die Funktion FromDataLinkLayerEOC() muss im DSPB in 
// einem 100Hz CASE untergebracht werden (IDLE_TIME_ON_RX_LINE_EOC =100 entspricht 1 Sekunde).
// setting aktuell: 120Sek -> 2Minuten gemäss Besprechung vom 98-09-01
#define	IDLE_TIME_ON_RX_LINE_EOC 12000	

//
// EOC Telegram types
//
#define	FAST_STATUS_TEL		0x0001

//
// control field data for EOC
//  msb8	7	6	5	4	3	2	1	0(LSB)
//  Res		PRM	FCB	FCV (--function code )
//
//

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef enum
{
	e_EOCTxState_SendFastStatus,
	e_EOCTxState_SendSync,
	e_EOCTxState_SendLinkData

} e_EOCTxState;


typedef enum
{
	e_EOCTxState_WaitingTx,
	e_EOCTxState_PreparingTx,
	e_EOCTxState_SendingDataTx

} e_EOCTxMakeState;


typedef struct 
{
	e_EOCTxMakeState	TxMakeState;
	long	            TxTimeNoSend;
	BOOL	            TxAlarmDL_EOC;
	e_EOCTxState   	    StateTx;
	long	            TxTelegramCounter;
	long	            TxTelegramCounterStatus;
	long	            RxWordFailure;
	long	            RxTelegramCounter;
	BOOL	            RxAlarmDL_EOC;
	long	            RxCRCFailure;
	long	            RxEndCharFailure;
	long	            Rx_Telegr_Failure;
	long	            RxAlarmDelayCounter;
	long	            RxTelegramTestCounter;
	long	            RxTelegramFailureCounter;
	long	            RxTelegramCounterStatus;
	long	            RxTelegramFailureCounterStatus;
} t_g_Status;

typedef struct 
{
	e_FT12State  	State;
	long	        CounterData;
	long	        Data[LENGTH_SYNC_TELEGR];
	long	        SendCounter;	      	   // for tests only

} t_SyncTelegramStatus;


//
//Teststruct for EOC
//

/*
typedef struct {
	long	data1;
	long	data2;
	long	data3;
	long	data4;
} TypeTestDataEOC;

*/
//
//
//
typedef struct {
	long	*ptrToTxDataEOC;
	long	*start_ptrEOC;
	long	*end_ptrEOC;
	long	m_checksumEOC;
	long	m_NrOfCharToBeTransmitEOC;
	long	m_TxByteCounterEOC;
	long	*m_TransmitCharacterPtrEOC;
} t_EOC_AuxTxData;

typedef struct {
	long	start_message;	
	long	m_MessageStartDetected_EOC;
	long	m_CharCounter_EOC;
	long	m_NumberOfRecCharEOC;
	long	m_NumberOfUserDataEOC;
	long	*m_ReceiveLongPtrEOC;
	long	m_RxByteCounterEOC;
	long	m_IdleIntervalOnRxEOCCtr;
	long	m_RxChecksumEOC;
} t_EOC_AuxRxData;


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static t_FT12TelegramBuffer g_receiveBuffer[N_APLC_CHANNELS]; // 3 EOC aplcChannels

static t_FT12TelegramBuffer ** g_pTransmitBuffer; // pointer to pointer to EOC1 send buffer


static t_g_Status g_Status[N_APLC_CHANNELS];
static t_SyncTelegramStatus SyncTelegramStatus[N_APLC_CHANNELS];

static t_EOC_AuxTxData EOC_AuxData[N_APLC_CHANNELS]; 

static t_EOC_AuxRxData EOC_AuxRxData[N_APLC_CHANNELS];

static t_FT12TelegramBuffer FastStatusTxBufferEOC[N_APLC_CHANNELS];


static t_FastStatusTelegram * g_LocalFastStatusData;  // pointer to local fast status data in object "status"
static t_FastStatusTelegram * g_RemoteFastStatusData; // pointer to remote fast status data in object "status"
 								
static t_P4LT_Status        * g_p_StatusData;
static unsigned long        * g_pHighLevelAlarms;

//data for tests only
//static TypeTestDataEOC TestDataEOC;

static e_FT12State * g_pState;

static BOOL g_EOCDataWarning;

/* LOCAL FUNCTIONS ***********************************************************/

static void makeFrameHeaderEOC(long *MessageToEquipment, 
							   unsigned long txMsgSizeInLongs, 
							   unsigned long aplcChannel);

static BOOL makeFrameUserDataEOC(unsigned long aplcChannel);

static void UpdateFastStatus(unsigned long aplcChannel);

static long ConvertByteForEOC(unsigned long txCharacter);


/* IMPLEMENTATION ************************************************************/

#pragma optimize_off	// no optimization in ext ram

//
//*****************************************************************************
//*** InitDataLinkLayerEOC() ************************************************
//*****************************************************************************
// Function:
// Do the initialisation for the data link layer EOC tx/(rx)


void SEGMENT_EXT_CODE initDataLinkLayerEOC(
						  t_FT12TelegramBuffer ** receiveBuffer,  //[out]
		  				  t_FT12TelegramBuffer ** transmitBuffer, //[in]
						  e_FT12State * pState)                   //[in]
{
	unsigned int aplcChannel;

	g_pState = pState; // save pointer to EOC state

	*receiveBuffer    = &g_receiveBuffer[APLC_CH1]; // return pointer to Ch1 receive buffer
	g_pTransmitBuffer = transmitBuffer; // save pointer to pointer to tx buffer


	g_LocalFastStatusData  = status_getLocalFastStatus();
	g_RemoteFastStatusData = status_getRemoteFastStatus();

	g_p_StatusData         = status_getPStatus();
	g_pHighLevelAlarms     = monitor_get_pHighLevelAlarms();

	//
	// init setting von g_Status Ch1-3
	//

	for (aplcChannel = 0 ; aplcChannel < N_APLC_CHANNELS; aplcChannel++)
	{
    	g_receiveBuffer[aplcChannel].State   = e_FT12State_Empty;
		
		g_Status[aplcChannel].StateTx = e_EOCTxState_SendSync;
		
		g_Status[aplcChannel].TxMakeState = e_EOCTxState_WaitingTx;

		SyncTelegramStatus[aplcChannel].Data[0] = RUHEZUSTAND;
		SyncTelegramStatus[aplcChannel].Data[1] = SYNC_WORD;
		SyncTelegramStatus[aplcChannel].Data[2] = RUHEZUSTAND;
		SyncTelegramStatus[aplcChannel].Data[3] = RUHEZUSTAND;
		SyncTelegramStatus[aplcChannel].Data[4] = SYNC_WORD;
		SyncTelegramStatus[aplcChannel].State = e_FT12State_Full;

		FastStatusTxBufferEOC[aplcChannel].State  = e_FT12State_Empty;
	}

    g_LocalFastStatusData[APLC_CH1].fwCompatibilityIndex = FW_COMPATIBILITY_INDEX;

}


#pragma optimize_as_cmd_line

/******************************************************************************/
//
// void makeFrameHeader(long *MessageToEquipment, long txMsgSizeInLongs)
//
// - erzeugt ein neues Frame mit dem Header und dessen Prüfsumme
// - initialisiert die Funktion long makeFrameUserData() 
/******************************************************************************/

static void SEGMENT_EXT_CODE makeFrameHeaderEOC(long * MessageToEquipment, 
											    unsigned long txMsgSizeInLongs,
											    unsigned long aplcChannel)
{
	unsigned int word;
	unsigned long txMsgSizeInBytes = txMsgSizeInLongs * NR_OF_BYTES_IN_LONG + LENGTH_HEADER;

	t_EOC_AuxTxData *p_EOC_AuxData = &EOC_AuxData[aplcChannel];
	t_FT12TelegramBuffer *p_FastStatusTxBufferEOC = &FastStatusTxBufferEOC[aplcChannel];

	if(txMsgSizeInBytes <= LENGTH_MAX) 
	{
		p_FastStatusTxBufferEOC->ControlField        = RXFUNCTION_SEND_NOREPLY;
		p_FastStatusTxBufferEOC->MessageType         = FAST_STATUS_TEL;
		p_FastStatusTxBufferEOC->DestinationSubadress= 0;		
		p_FastStatusTxBufferEOC->DestinationAdress   = 0;
		p_FastStatusTxBufferEOC->SourceAdress        = 0;
		p_FastStatusTxBufferEOC->HopCounter          = 0;
		p_FastStatusTxBufferEOC->SequenceNumber      = 0;

		p_EOC_AuxData->ptrToTxDataEOC	   = p_FastStatusTxBufferEOC->TelegrammDataArr;
		*p_EOC_AuxData->ptrToTxDataEOC++ = (txMsgSizeInBytes   <<SB24) | (txMsgSizeInBytes		  <<SB16)| (FT12_START_CHAR << SB8); 
		*p_EOC_AuxData->ptrToTxDataEOC++ = (p_FastStatusTxBufferEOC->MessageType   <<SB24) | (p_FastStatusTxBufferEOC->DestinationSubadress <<SB16)| (CONTROL_FIELD <<SB8) | FT12_START_CHAR;
		*p_EOC_AuxData->ptrToTxDataEOC++ = (p_FastStatusTxBufferEOC->SourceAdress  <<SB24) | (p_FastStatusTxBufferEOC->DestinationAdress	  <<SB8) | ((p_FastStatusTxBufferEOC->MessageType >>SB8)  & MASK_8LSB);
		*p_EOC_AuxData->ptrToTxDataEOC++ = (p_FastStatusTxBufferEOC->SequenceNumber<<SB16) | (p_FastStatusTxBufferEOC->HopCounter           <<SB8) | ((p_FastStatusTxBufferEOC->SourceAdress  >>SB8) & MASK_8LSB); 
		
		p_EOC_AuxData->start_ptrEOC = MessageToEquipment;
		p_EOC_AuxData->end_ptrEOC   = MessageToEquipment + txMsgSizeInLongs;	

		// berechne Prüfsumme des Headers 
		p_EOC_AuxData->m_checksumEOC = 0;
		p_EOC_AuxData->ptrToTxDataEOC	= p_FastStatusTxBufferEOC->TelegrammDataArr;
		p_EOC_AuxData->ptrToTxDataEOC++; // zeigt auf 2. Word

		for (word = 2; word <= 4; word++)
		{
			p_EOC_AuxData->m_checksumEOC +=  *p_EOC_AuxData->ptrToTxDataEOC         & MASK_8LSB;
			p_EOC_AuxData->m_checksumEOC += (*p_EOC_AuxData->ptrToTxDataEOC >> SB8 )& MASK_8LSB;
			p_EOC_AuxData->m_checksumEOC += (*p_EOC_AuxData->ptrToTxDataEOC >> SB16)& MASK_8LSB;
			p_EOC_AuxData->m_checksumEOC += (*p_EOC_AuxData->ptrToTxDataEOC >> SB24)& MASK_8LSB;
			p_EOC_AuxData->ptrToTxDataEOC++;
		}
		 // Die Prüfsumme beginnt erst mit dem Control Field Byte, darum muss
		 //	das Startzeichen vom 2. Word, das durch das "word-weise" berechnen 
		 //	fälschlicherweise dazu gekommen sind, abgezogen werden
		
		p_EOC_AuxData->m_checksumEOC -= FT12_START_CHAR; 
	}
}



/******************************************************************************/
//
// long makeFrameUserData()
//
// - ergänzt das Frame mit der Prüfsumme und den Daten
// - bei einem neuen Frame muss zuerst die Funktion makeFrameHeader ausgeführt werden.
//   Darin werden die Variablen initialisiert.
// - Der Return Wert TRUE zeigt das Ende der Frame - Zusammensetzung an
// -> keine Echtzeitanforderung, 
//	
// pro Aufruf wird, solange noch USER Daten übrig sind:
// -  ein "word" von den USER Daten in den Frame Buffer kopiert
// -  die aktuelle Prüfsumme mit diesem "word" berechnet 
// -  der Return Wert FALSE übergeben
// 
/******************************************************************************/


static BOOL SEGMENT_EXT_CODE makeFrameUserDataEOC(unsigned long aplcChannel)
{
	t_EOC_AuxTxData *p_EOC_AuxData = &EOC_AuxData[aplcChannel];	

	if (p_EOC_AuxData->start_ptrEOC < p_EOC_AuxData->end_ptrEOC)
	{
		*p_EOC_AuxData->ptrToTxDataEOC = *p_EOC_AuxData->start_ptrEOC++;				
		p_EOC_AuxData->m_checksumEOC += *p_EOC_AuxData->ptrToTxDataEOC & MASK_8LSB;		
		p_EOC_AuxData->m_checksumEOC += (*p_EOC_AuxData->ptrToTxDataEOC >> SB8 ) & MASK_8LSB;
		p_EOC_AuxData->m_checksumEOC += (*p_EOC_AuxData->ptrToTxDataEOC >> SB16) & MASK_8LSB;
		p_EOC_AuxData->m_checksumEOC += (*p_EOC_AuxData->ptrToTxDataEOC >> SB24) & MASK_8LSB;
		p_EOC_AuxData->ptrToTxDataEOC++;
		return FALSE;		
	}

	p_EOC_AuxData->m_checksumEOC &= MASK_8LSB;
	*p_EOC_AuxData->ptrToTxDataEOC = p_EOC_AuxData->m_checksumEOC |(FT12_END_CHAR << SB8);
	return TRUE;	
}


#pragma optimize_off	// no optimization in ext ram

//*****************************************************************************
//*** UpdateFastStatus() ******************************************************
//*****************************************************************************


static void SEGMENT_EXT_CODE UpdateFastStatus(unsigned long aplcChannel)
{
	g_LocalFastStatusData[aplcChannel].deviceState = g_p_StatusData->system.deviceState;
	
	if (APLC_CH1 == aplcChannel)
	{
		g_LocalFastStatusData[aplcChannel].highLevelAlarms = *g_pHighLevelAlarms;
	}

	// do nothing in APLC_CH2/3

/*
	if (APLC_CH1 == aplcChannel)
	{
		g_LocalFastStatusData[aplcChannel].FW_Compatibility_Index = FW_COMPATIBILITY_INDEX;	
		//g_LocalFastStatusData[aplcChannel].dummy1 = 1;
		//g_LocalFastStatusData[aplcChannel].dummy2 = 1;
		// for testing
		g_LocalFastStatusData[aplcChannel].dummy1 = 0x11111111;
		g_LocalFastStatusData[aplcChannel].dummy2 = 0x11111111;
		g_LocalFastStatusData[aplcChannel].Alarm_Status = 0x22222222;
        g_LocalFastStatusData[aplcChannel].Alarm_Output = 0x22222222;
        g_LocalFastStatusData[aplcChannel].AktuellerGeraeteStatus = 0x33333333;	
	}

	if (APLC_CH2 == aplcChannel)
	{
		g_LocalFastStatusData[aplcChannel].FW_Compatibility_Index = FW_COMPATIBILITY_INDEX;	
		//g_LocalFastStatusData[aplcChannel].dummy1 = 1;
		//g_LocalFastStatusData[aplcChannel].dummy2 = 1;
		// for testing
		g_LocalFastStatusData[aplcChannel].dummy1 = 0x44444444;
		g_LocalFastStatusData[aplcChannel].dummy2 = 0x55555555;
		g_LocalFastStatusData[aplcChannel].Alarm_Status = 0x66666666;
        g_LocalFastStatusData[aplcChannel].Alarm_Output = 0x77777777;
        g_LocalFastStatusData[aplcChannel].AktuellerGeraeteStatus = 0x88888888;	
	}

		if (APLC_CH3 == aplcChannel)
	{
		g_LocalFastStatusData[aplcChannel].FW_Compatibility_Index = FW_COMPATIBILITY_INDEX;	
		//g_LocalFastStatusData[aplcChannel].dummy1 = 1;
		//g_LocalFastStatusData[aplcChannel].dummy2 = 1;
		// for testing
		g_LocalFastStatusData[aplcChannel].dummy1 = 0x99999999;
		g_LocalFastStatusData[aplcChannel].dummy2 = 0xaaaaaaaa;
		g_LocalFastStatusData[aplcChannel].Alarm_Status = 0xbbbbbbbb;
        g_LocalFastStatusData[aplcChannel].Alarm_Output = 0xcccccccc;
        g_LocalFastStatusData[aplcChannel].AktuellerGeraeteStatus = 0xdddddddd;	
	}

*/
}


#pragma optimize_as_cmd_line

//*****************************************************************************
//*** MakeTxFastStatusTelEOC() ************************************************
//*****************************************************************************
// function: 
// Update data for fast status telegramm to EOC 

// To be called with f_TDM / 100 = 0.10667kHz

void SEGMENT_EXT_CODE MakeTxFastStatusTelEOC(unsigned long aplcChannel)
{
	// *****************************************
	// set the pointers for selected aplcChannel 
	// *****************************************
	t_g_Status           * p_g_Status              = &g_Status[aplcChannel];
	t_FT12TelegramBuffer * p_FastStatusTxBufferEOC = &FastStatusTxBufferEOC[aplcChannel];
	t_FastStatusTelegram * p_FastStatusData        = &g_LocalFastStatusData[aplcChannel];

	//
	if (e_EOCTxState_SendFastStatus == p_g_Status->StateTx)
	{
		switch(p_g_Status->TxMakeState)
		{
			case e_EOCTxState_WaitingTx:

				UpdateFastStatus(aplcChannel);
				if(p_FastStatusTxBufferEOC->State == e_FT12State_Empty)
				{
					makeFrameHeaderEOC((long*)p_FastStatusData, 
							           sizeof(t_FastStatusTelegram),
									   aplcChannel);
					p_g_Status->TxMakeState = e_EOCTxState_PreparingTx;
				}
				break;

			case e_EOCTxState_PreparingTx:

				if(makeFrameUserDataEOC(aplcChannel))
				{
					p_FastStatusTxBufferEOC->State = e_FT12State_Full; 
					p_g_Status->TxMakeState = e_EOCTxState_SendingDataTx;
				}
				break;
			
			case e_EOCTxState_SendingDataTx:

				if (p_FastStatusTxBufferEOC->State == e_FT12State_Empty)
				{
					p_g_Status->TxMakeState = e_EOCTxState_WaitingTx;
				}
				break;
			
			default:
				ASSERT(FALSE);
		}
	}
}

  
//
//
//*****************************************************************************
//*** ConvertByteForEOC()  ***************************************************
//*****************************************************************************
// Function:
// Convert the input value (byte value) to the format defined by TM-NTC-  
// 96-1541/A (Startbit,LSB ... MSB,Parity(even),Stopbit).
//
//
static long SEGMENT_EXT_CODE ConvertByteForEOC(unsigned long txCharacter)
{
	#define	MASKE_PARITY_CLEAR				0x400
	#define	MASKE_PARITY_SET				0x600    

	// Tabelle zum auslesen der Parität eines 4 Bit Wertes->2^4=16
	static const long TabParity_0[] = // SIZE_TAB1_PARITY
						{0x0,0x1,0x1,0x0,
						 0x1,0x1,0x0,0x1,
						 0x1,0x0,0x0,0x1,
						 0x0,0x1,0x1,0x0};

	// Das Byte wird in 2 4 bit Werte aufgeteilt und von beiden 4 bit Werten die Parität bestimmt
	// anhand der Tabelle TabParity. Die beiden Werte werden zusammengezählt und die 
	// Summe wird als Offset in der Taballe ParityTab1 verwendet. Dies ergibt Werte 0...2 ->
	// Grösse der Tabelle =3
	static const long  ParityTab_1[] = // SIZE_PARITY_TAB
		                 {MASKE_PARITY_CLEAR,
						  MASKE_PARITY_SET,
						  MASKE_PARITY_CLEAR};

	const long * ptr_parity_tab  = &TabParity_0[0]; // load adress of parity tab 1
	const long * ptr_parity_tab1 = &ParityTab_1[0]; // load adress of parity tab 2

	unsigned long stemp1 = txCharacter & MASK_4LSB;					   // maske bit 0...3     
	unsigned long stemp2 = (txCharacter>>4) & MASK_4LSB;               // maske bit 4...7

	stemp1 = *(ptr_parity_tab + stemp1);						// parity of bit 0...3
	stemp2 = *(ptr_parity_tab + stemp2);						// parity of bit 4...7
	stemp2 = *(ptr_parity_tab1 + stemp1 + stemp2);				// parity of first byte
	stemp1 = ((txCharacter << 1) & MASKEBIT_1_8) | stemp2;	// Add parity-, start- and stop- bit 

	return(stemp1);											// return converted value
}


#pragma optimize_off	// no optimization in ext ram

//*****************************************************************************
//*** ToDataLinkLayerEOC()   **************************************************
//*****************************************************************************
//
//

// To be called with f_TDM / 100 = 0.10667kHz

void SEGMENT_EXT_CODE ToDataLinkLayerEOC(EocDataBufferType *txBuffer, BOOL MWireActive, unsigned long aplcChannel)
{       
	register BOOL bEOCTxQueueEmpty = FALSE;

	//
	// set the pointers to the selected aplcChannel
	//
	t_g_Status           *p_g_Status              = &g_Status[aplcChannel];
	t_EOC_AuxTxData      *p_EOC_AuxData           = &EOC_AuxData[aplcChannel];
	t_SyncTelegramStatus *p_SyncTelegramStatus    = &SyncTelegramStatus[aplcChannel];
	t_FT12TelegramBuffer *p_FastStatusTxBufferEOC = &FastStatusTxBufferEOC[aplcChannel];


	switch (p_g_Status->StateTx)
	{
		case e_EOCTxState_SendLinkData:
			// *****************************************************************
			// skip this state for ch2/3 (no link data were sent over ch2/3) 
			// *****************************************************************
			if(APLC_CH1 == aplcChannel)
			{
				//
				// check if all eoc link data were sent
				//
				if 	((0 == p_EOC_AuxData->m_NrOfCharToBeTransmitEOC) && (e_FT12State_Sending == *g_pState)) 
				{
					*g_pState = e_FT12State_Idle;					//
					(*g_pTransmitBuffer)->State = e_FT12State_Transmitted;			// Packet im Transmitt-Buffer wurde vollständig,gesendet				
					p_g_Status->TxTelegramCounter++;	            // for tests only
					p_g_Status->StateTx = e_EOCTxState_SendFastStatus;	    // switch to send fast status data
				}
				//
				// check if new data for eoc link are ready to send
				//
				if(e_FT12State_Forwarded == *g_pState) // Packet im Sendebuffer
				{ 
					*g_pState = e_FT12State_Sending;
					(*g_pTransmitBuffer)->State = e_FT12State_Sending;
					p_EOC_AuxData->m_NrOfCharToBeTransmitEOC = (((*g_pTransmitBuffer)->TelegrammDataArr[0]>>SB16) & MASK_8LSB)+ FRAME_OVERHEAD + NUMBER_OF_PAUSE;		// L +2
					p_EOC_AuxData->m_TransmitCharacterPtrEOC = (*g_pTransmitBuffer)->TelegrammDataArr;
					p_EOC_AuxData->m_TxByteCounterEOC = START_1;		// starte mit 33 Ruhezustandsbits
				}
				//
				// check if no link data were ready to send -> switch to fast status data send 
				//
				if(e_FT12State_Idle == *g_pState)
				{
					p_g_Status->StateTx = e_EOCTxState_SendFastStatus;	// switch to send fast status data			
				}
				//
				// check if eoc aplcChannel data break is activ while link data are sending
				//
				if (txBuffer->dataBreak)
				{
					*g_pState = e_FT12State_Forwarded;		// try again after sync
					(*g_pTransmitBuffer)->State = e_FT12State_Forwarded;
					p_g_Status->StateTx = e_EOCTxState_SendSync;
					p_SyncTelegramStatus->State = e_FT12State_Full;
				}
			}
			break;

		case e_EOCTxState_SendSync:
			//
			// check if all sync data for EOC were sent
			//
			if 	((0 == p_EOC_AuxData->m_NrOfCharToBeTransmitEOC) && (p_SyncTelegramStatus->State == e_FT12State_Sending)) 
			{
				p_SyncTelegramStatus->State = e_FT12State_Empty;	// Packet im Sync-Transmit-Buffer wurde vollständig gesendet
				p_SyncTelegramStatus->SendCounter++;
				p_g_Status->StateTx = e_EOCTxState_SendFastStatus;	// switch to send fast status data
				//toDL_EOCState = IDLE;	// Bug, interferes with bridge
			}
			//
			// check if new sync data for EOC are ready to send
			// 
			if (p_SyncTelegramStatus->State == e_FT12State_Full)
			{
				p_SyncTelegramStatus->State = e_FT12State_Sending;
				p_SyncTelegramStatus->CounterData = 0;
				p_EOC_AuxData->m_NrOfCharToBeTransmitEOC = LENGTH_SYNC_TELEGR;
				p_EOC_AuxData->m_TxByteCounterEOC = SYNC_TELEG;
			}
			break;

		case e_EOCTxState_SendFastStatus:
			//
			// check if all eoc fast status data were sent
			//
			if 	((p_EOC_AuxData->m_NrOfCharToBeTransmitEOC == 0) && (p_FastStatusTxBufferEOC->State == e_FT12State_Sending)) 
			{
				p_FastStatusTxBufferEOC->State = e_FT12State_Empty;			// Packet im Transmit-Buffer wurde vollständig,gesendet				
				p_g_Status->TxTelegramCounterStatus++;	// for tests only

				if(APLC_CH1 == aplcChannel) 
				{
					p_g_Status->StateTx = e_EOCTxState_SendLinkData;	// switch to send link data only for aplcChannel 1
				}
			}
			//
			// check if fast status telegram is ready to send
			//
			if( p_FastStatusTxBufferEOC->State == e_FT12State_Full) // Packet im Sendebuffer
			{ 
				p_FastStatusTxBufferEOC->State = e_FT12State_Sending;
				p_EOC_AuxData->m_NrOfCharToBeTransmitEOC = ((p_FastStatusTxBufferEOC->TelegrammDataArr[0]>>SB16) & MASK_8LSB)+ FRAME_OVERHEAD + NUMBER_OF_PAUSE;		// L +2
				p_EOC_AuxData->m_TransmitCharacterPtrEOC =  p_FastStatusTxBufferEOC->TelegrammDataArr;
				p_EOC_AuxData->m_TxByteCounterEOC = START_1;		// starte mit 33 Ruhezustandsbits
			}
			//
			// check if eoc aplcChannel data break is activ while fast status data are sending
			//
			if (txBuffer->dataBreak)
			{
				p_FastStatusTxBufferEOC->State  = e_FT12State_Empty;	
				p_g_Status->StateTx = e_EOCTxState_SendSync;
				p_SyncTelegramStatus->State = e_FT12State_Full;
				p_g_Status->TxMakeState = e_EOCTxState_WaitingTx;
			}
			break;

		default:
			ASSERT(FALSE);
	} // end switch (g_Status.StateTx)


    if(p_EOC_AuxData->m_NrOfCharToBeTransmitEOC)
	{
		//
		// test if TxQueue (EOC) is empty and waiting for a character
		//
		if(!txBuffer->full) 
		{	// ready to send next character
			bEOCTxQueueEmpty = TRUE;
			p_g_Status->TxTimeNoSend = 0;
		}
		else
		{	// not ready to send next character
			if(MWireActive)
			{
				// MWire active do not set EOC send Warning
			}
			else
			{
				// M-Wire not activ -> set Warning
				p_g_Status->TxTimeNoSend++;
				bEOCTxQueueEmpty = FALSE;

				p_g_Status->TxAlarmDL_EOC = (p_g_Status->TxTimeNoSend > DL_EOC_TX_ALARMTIME);	// set alarm no data send 
			}
		}

		if(bEOCTxQueueEmpty)
	    {                     
	      	p_EOC_AuxData->m_NrOfCharToBeTransmitEOC--;
			
	      	switch (p_EOC_AuxData->m_TxByteCounterEOC)
	      	{
  	            register unsigned long txCharacter;

				case FIRST_BYTE:
					txCharacter = *p_EOC_AuxData->m_TransmitCharacterPtrEOC  & MASK_8LSB;
					txBuffer->data = ConvertByteForEOC(txCharacter);
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TxByteCounterEOC++;
					break;
				case SECOND_BYTE:
					txCharacter = (*p_EOC_AuxData->m_TransmitCharacterPtrEOC >> SB8)  & MASK_8LSB;
					txBuffer->data = ConvertByteForEOC(txCharacter);
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TxByteCounterEOC++;
					break;
				case THIRD_BYTE:
					txCharacter = (*p_EOC_AuxData->m_TransmitCharacterPtrEOC >> SB16)  & MASK_8LSB;
					txBuffer->data = ConvertByteForEOC(txCharacter);
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TxByteCounterEOC++;
					break;
				case FOURTH_BYTE:
					txCharacter = (*p_EOC_AuxData->m_TransmitCharacterPtrEOC >> SB24)  & MASK_8LSB;
					txBuffer->data = ConvertByteForEOC(txCharacter);
					p_EOC_AuxData->m_TxByteCounterEOC = FIRST_BYTE;
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TransmitCharacterPtrEOC++;
					break;
				case START_1:
					txBuffer->data = RUHEZUSTAND;
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TxByteCounterEOC = START_2;
					break;
				case START_2:
					txBuffer->data = RUHEZUSTAND;
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TxByteCounterEOC = START_3;
					break;
				case START_3:
					txBuffer->data = RUHEZUSTAND;
					txBuffer->full = TRUE;
					p_EOC_AuxData->m_TxByteCounterEOC = SECOND_BYTE;
					break;
				case SYNC_TELEG:
					txBuffer->data = p_SyncTelegramStatus->Data[p_SyncTelegramStatus->CounterData];
					txBuffer->full = TRUE;
					p_SyncTelegramStatus->CounterData++;
					break;
				default:
					ASSERT(FALSE);
			}		// end switch
		} // end if (noCharacterInTxQueue)    
	} // end if (p_EOC_AuxData->m_NrOfCharToBeTransmitEOC)                                   
} // end function void ToDataLinkLayerEOC(void)



//*****************************************************************************
//*** FromDataLinkLayerEOC()   ************************************************
//*****************************************************************************
//
//

// To be called with f_TDM / 100 = 0.10667kHz

void SEGMENT_EXT_CODE FromDataLinkLayerEOC(EocDataBufferType *rxBuffer, BOOL EWireActive, unsigned long aplcChannel)
{
	register BOOL bCharacterInRxQueue = FALSE;
	register unsigned long receivedCharacter = 0;


	t_EOC_AuxRxData      *p_EOC_AuxRxData      = &EOC_AuxRxData[aplcChannel];
	t_g_Status           *p_g_Status           = &g_Status[aplcChannel];
	t_FT12TelegramBuffer *p_ReceiveBufferEOC   = &g_receiveBuffer[aplcChannel];


	// test if correct character is waiting in RxQueue
	if(rxBuffer->full)
	{
		receivedCharacter = (rxBuffer->data>>1) & MASK_8LSB;
		
		// check received char for parity-, start- and stop- bit
		if(ConvertByteForEOC(receivedCharacter) == rxBuffer->data) 
		{
			bCharacterInRxQueue = TRUE;								// received char correct
		}
		else
		{
			bCharacterInRxQueue = FALSE;	
			p_g_Status->RxWordFailure++;						// recieved char failed
		}

		rxBuffer->full = FALSE;
	}

	if(bCharacterInRxQueue && (e_FT12State_Empty == p_ReceiveBufferEOC->State ))
	{
		p_EOC_AuxRxData->m_IdleIntervalOnRxEOCCtr = 0; // reset idle time between two successive bytes
		if(!p_EOC_AuxRxData->m_MessageStartDetected_EOC)
	    {
			// detect start m_Message >>| start | L | L | start |<< 
		    // start == 0x68
		    // length field range: 0...255, this value is transmitted twice for higher security
		    p_EOC_AuxRxData->start_message = p_EOC_AuxRxData->start_message << SB8 | receivedCharacter;
		    
			if((p_EOC_AuxRxData->start_message & MASK_MESSAGE_START) == MESSAGE_START)
			{
				p_EOC_AuxRxData->m_NumberOfUserDataEOC = (p_EOC_AuxRxData->start_message >> SB8) & MASK_8LSB;  // number of user data characters 
	            
				if(((p_EOC_AuxRxData->start_message >> SB16) & MASK_8LSB) != p_EOC_AuxRxData->m_NumberOfUserDataEOC)	
				{
					p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE; // compare the two L characters
				}
				else  
				{
					// Header correctly received
					p_EOC_AuxRxData->m_MessageStartDetected_EOC = TRUE;
					p_EOC_AuxRxData->m_NumberOfRecCharEOC = p_EOC_AuxRxData->m_NumberOfUserDataEOC + CRC_AND_END_CHAR;
					p_EOC_AuxRxData->m_CharCounter_EOC = 0;
					p_EOC_AuxRxData->m_RxChecksumEOC = 0;
					p_EOC_AuxRxData->m_RxByteCounterEOC = FALSE;
					p_g_Status->RxWordFailure = 0;
					// NEU: Kopiere 0|S|L|L in 1. Word und S in 2. Word MSB des Empfangsbuffers 
					// 0 = LSB , L= MSB
					// Programcode....

					p_EOC_AuxRxData->m_ReceiveLongPtrEOC    = p_ReceiveBufferEOC->TelegrammDataArr;	// 1. Word
					*p_EOC_AuxRxData->m_ReceiveLongPtrEOC++ = ((FT12_START_CHAR<<SB8) | (p_EOC_AuxRxData->m_NumberOfUserDataEOC << SB16)| (p_EOC_AuxRxData->m_NumberOfUserDataEOC << SB24)) & 0xffffff00; 			
					*p_EOC_AuxRxData->m_ReceiveLongPtrEOC   = FT12_START_CHAR;						// 2. Word -> 2 Byte		
					p_EOC_AuxRxData->start_message = 0;
				}	
			}
		}
		else
		{
			// >>| start | L | L | start |<< characters detected
			if(++p_EOC_AuxRxData->m_CharCounter_EOC <= p_EOC_AuxRxData->m_NumberOfRecCharEOC) 
			{
				if(p_EOC_AuxRxData->m_CharCounter_EOC <= p_EOC_AuxRxData->m_NumberOfUserDataEOC) 
				{
					p_EOC_AuxRxData->m_RxChecksumEOC += receivedCharacter;
				}

				if(p_EOC_AuxRxData->m_CharCounter_EOC <= LENGTH_HEADER) 
				{
				    // Scan header
					switch (p_EOC_AuxRxData->m_CharCounter_EOC) 
					{
						case CONTROL_FIELD:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB8;
									p_ReceiveBufferEOC->ControlField=receivedCharacter;
									break;

						case DEST_SUB_ADR:		
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB16; 
									p_ReceiveBufferEOC->DestinationSubadress=receivedCharacter;
									break;

						case MESS_TYP_LSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB24;
									p_ReceiveBufferEOC->MessageType=receivedCharacter;
									p_EOC_AuxRxData->m_ReceiveLongPtrEOC++;			// Pointer auf 3. Word
									break;

						case MESS_TYP_MSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC =  0x00000000;
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC =  receivedCharacter;
									p_ReceiveBufferEOC->MessageType |= receivedCharacter << SB8;
									break;

						case DEST_ADR_LSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB8;
									p_ReceiveBufferEOC->DestinationAdress = receivedCharacter;
									break;

						case DEST_ADR_MSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB16;
									p_ReceiveBufferEOC->DestinationAdress |= receivedCharacter << SB8;
									break;

						case SOUR_ADR_LSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB24;
									p_ReceiveBufferEOC->SourceAdress = receivedCharacter;
									p_EOC_AuxRxData->m_ReceiveLongPtrEOC++;			// Pointer auf 4. Word
									break;

						case SOUR_ADR_MSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC  =  receivedCharacter;
									p_ReceiveBufferEOC->SourceAdress|=receivedCharacter << SB8;
									break;

						case HOP_COUNTER:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB8;
									p_ReceiveBufferEOC->HopCounter = receivedCharacter;
									break;
									
						case SEQUENCE_NR_LSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB16;
									p_ReceiveBufferEOC->SequenceNumber = receivedCharacter;
									break;

						case SEQUENCE_NR_MSB:
									*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB24;
									p_ReceiveBufferEOC->SequenceNumber |=receivedCharacter << SB8;
									p_EOC_AuxRxData->m_ReceiveLongPtrEOC++;			// Pointer auf 5. Word
									break;

						default:    
							ASSERT(FALSE);

					}	// end switch
				} // if  length
				else 
				{   // Scan Application User Data,  32Bit (1 Word), and Checksum and End Char
		      		switch (p_EOC_AuxRxData->m_RxByteCounterEOC)
		      		{
		      		 case FIRST_BYTE:
		      	 			*p_EOC_AuxRxData->m_ReceiveLongPtrEOC  =  receivedCharacter;
		      	 			p_EOC_AuxRxData->m_RxByteCounterEOC++;
		      	 			break;
		      		 case SECOND_BYTE:
		      	 			*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB8;
		      	 			p_EOC_AuxRxData->m_RxByteCounterEOC++;
		      	 			break;
		      		 case THIRD_BYTE:
		      	 			*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB16;
		      	 			p_EOC_AuxRxData->m_RxByteCounterEOC++;
		      	 			break;
		      		 case FOURTH_BYTE:
		      	 			*p_EOC_AuxRxData->m_ReceiveLongPtrEOC |=  receivedCharacter << SB24;
		      	 			p_EOC_AuxRxData->m_RxByteCounterEOC = 0;
	          				p_EOC_AuxRxData->m_ReceiveLongPtrEOC++;
		      	 			break;
					 default:
							ASSERT(FALSE);
	          				
					} // switch
				} // else  length
			} // if (m_CharCounter_EOC <= m_NumberOfRecCharEOC)

		    if(p_EOC_AuxRxData->m_CharCounter_EOC == p_EOC_AuxRxData->m_NumberOfRecCharEOC)				
			{ 	// all characters received
				//*******************************************************
				//*** check received telegramm **************************
				//*******************************************************
				p_g_Status->RxTelegramTestCounter++;	// for tests only
				//	1.	Was there any byte partiy error, framing error or overrun error in the EOC_Modem while receiving 
				//		the Message ?
				if(p_g_Status->RxWordFailure > 0) 
				{
		   			// Framing or Parity error detected by EOC (any byte in the message).
					p_g_Status->RxWordFailure = 0;				// Reset RxWordFailure Counter
					p_g_Status->RxTelegramFailureCounter++;	// for tests only
					p_g_Status->Rx_Telegr_Failure = TRUE;
					p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE;
					return;		   				 			 	 								
				} 

				//	2.	Is the calculated CRC Sum equal to the received value in the RxFrame
				if((*p_EOC_AuxRxData->m_ReceiveLongPtrEOC & MASK_8LSB) != (p_EOC_AuxRxData->m_RxChecksumEOC & MASK_8LSB))
				{
					p_g_Status->RxCRCFailure++;				// for tests only
					p_g_Status->Rx_Telegr_Failure = TRUE;
					p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE;
					return; 
				}		
				//	3.	Check the end char 0x16
				if((((*p_EOC_AuxRxData->m_ReceiveLongPtrEOC)>>SB8) & MASK_8LSB) != FT12_END_CHAR)	
				{ 
					p_g_Status->RxEndCharFailure++;			// for tests only
					p_g_Status->Rx_Telegr_Failure = TRUE;
					p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE;
					return; 
				}
				//***************************************************
				//*** correct telegramm recieved ********************
				//***************************************************
				p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE;
				p_g_Status->Rx_Telegr_Failure = FALSE;

				//  check if send no reply telegram is received
				if((p_ReceiveBufferEOC->ControlField == CONTROL_FIELD)&&
														// telegram type	
				   (p_ReceiveBufferEOC->MessageType == FAST_STATUS_TEL))
				{
					p_g_Status->RxTelegramCounterStatus++;	// for tests only
					p_ReceiveBufferEOC->State = e_FT12State_Idle;
				}
				else
				{
					p_ReceiveBufferEOC->State = e_FT12State_Full;				// Frame ohne Fehler empfangen
					p_g_Status->RxTelegramCounter++;			// for tests only
				}
			} // end Block alle Oktette empfangen
		} // end else correct header received 
	}
	else
	{	// else no char in rx queue or word failure or receive buffer not empty
		// detect idle interval (if no characters receive)
		if(EWireActive)
		{
			p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE; //e-Wire active reset received telegramm
		}
		else
		{
			p_EOC_AuxRxData->m_IdleIntervalOnRxEOCCtr++;
		}
	}

	// Check if rxAlarm EOC no data received during a time of IDLE_TIME_ON_RX_LINE_EOC is to set
	if(p_EOC_AuxRxData->m_IdleIntervalOnRxEOCCtr > IDLE_TIME_ON_RX_LINE_EOC)
	{
		p_EOC_AuxRxData->m_MessageStartDetected_EOC = FALSE;
		p_EOC_AuxRxData->m_IdleIntervalOnRxEOCCtr = IDLE_TIME_ON_RX_LINE_EOC;
		p_g_Status->RxAlarmDL_EOC = TRUE;
	}
	else
	{
		p_g_Status->RxAlarmDL_EOC = FALSE;
	}
}


//*****************************************************************************
//*** HandleSendNoReply() *****************************************************
//*****************************************************************************
// Diese Funktion kopiert die korrekt empfangenen Daten im Empfangsbuffer des EOC's
// in den Struct RTFastStatusDataCh1
//

// To be called with f_TDM / 100 = 0.10667kHz

void SEGMENT_EXT_CODE HandleSendNoReply(unsigned long aplcChannel)
{
	if (g_receiveBuffer[aplcChannel].State == e_FT12State_Idle)
	{
		if( FAST_STATUS_TEL == g_receiveBuffer[aplcChannel].MessageType)
		{
			long *EndPtr_EOC; 
			long NumberOfReceivedLongValues_EOC;

			long * TempPtr1_EOC = (long*)&g_RemoteFastStatusData[aplcChannel];
			long *PtrToRxUD_EOC = (long *)(&g_receiveBuffer[aplcChannel]);

			PtrToRxUD_EOC += offsetof(t_FT12TelegramBuffer, TelegrammDataArr);
			PtrToRxUD_EOC += 4;		// Pointer To User Data (5. Word)
			NumberOfReceivedLongValues_EOC =  (((g_receiveBuffer[aplcChannel].TelegrammDataArr[0] >>SB24) & MASK_8LSB)- LENGTH_HEADER)/NR_OF_BYTES_IN_LONG;	

	        if(NumberOfReceivedLongValues_EOC < sizeof(g_RemoteFastStatusData[aplcChannel]))
			{
	            EndPtr_EOC = PtrToRxUD_EOC + NumberOfReceivedLongValues_EOC;
			}
			else
			{
				EndPtr_EOC = PtrToRxUD_EOC + sizeof(g_RemoteFastStatusData[aplcChannel]);
			}

			while(PtrToRxUD_EOC < EndPtr_EOC) 
			{
				*TempPtr1_EOC++ = *PtrToRxUD_EOC++;
			}

			g_receiveBuffer[aplcChannel].State = e_FT12State_Empty;	
		}
	}
}


//*****************************************************************************
//***  MonitorEOC()  **********************************************************
//*****************************************************************************
// Damit die Delayzeiten für den EOC-alarm korrekt sind, muss diese Funktion in 
// einem 100Hz-Case aufgerufen werden.
//
// Diese Funktion überwacht, ob daten über den EOC gesednet werden können(TxAlarmDL_EOC).
// Falls während einer Zeit (DL_EOC_TX_ALARMTIME = 2000) von 20 Sekunden keine
// Daten über den EOC gesendet werden können, wird der Alarm NO_EOC_DATA_SEND_CH1
// gesetzt. Dies ist ein HW-alarm, da dies nur bei dauernder Signallisierung 
// (M-Wire) über den EOC erfolgen kann.
// -> HW-Alarm
//
// Dies Funktion überwacht ob Daten über den EOC übertragen werden (RxAlarmDL_EOC). 
// Falls während der Zeit (IDLE_TIME_ON_RX_LINE_EOC =2000) von 20 Sekunden keine
// Daten über den EOC empfangen wurden, wird der Alarm  NO_EOC_DATA_REC_CH1 gesetzt.
// -> Linklarm
//
// Bei Empfang eines Telegrams wird geprüft, ob Startbit, Stopbit, Paritybit, CRC und
// Endcharakter korrekt empfangen wurden. Falls ein Fehler festgestellt wurde, wird nach 
// einer Delayzeit (EOCALARMTIME) von 2 Minuten der Alarm EOC_ALARM_CH1 gesetzt.
// -> Linkalarm
// 
// To be called with f_TDM / 100 = 0.10667kHz

void SEGMENT_EXT_CODE DL_EOC_Monitor(void)
{
	//
	// Check if CRC, parity, startbit, stopbit or end char failure
	//
	if((g_Status[APLC_CH1].Rx_Telegr_Failure) || 
	   (g_Status[APLC_CH1].RxWordFailure > 0)    )
	{
		if(g_Status[APLC_CH1].RxAlarmDelayCounter > EOCALARMTIME)
		{
			g_EOCDataWarning = TRUE;	
		}
		else
		{
			g_Status[APLC_CH1].RxAlarmDelayCounter++;	
		}
	}
	else
	{	// no rx alarm eoc (parity,start,stop,end char) pending 
		g_EOCDataWarning = FALSE;	
		g_Status[APLC_CH1].RxAlarmDelayCounter = 0;
	}
}

BOOL SEGMENT_EXT_CODE dlEOC_DataTxWarning(void)
{
	return g_Status[APLC_CH1].TxAlarmDL_EOC;
}

BOOL SEGMENT_EXT_CODE dlEOC_DataRxWarning(void)
{
	return g_Status[APLC_CH1].RxAlarmDL_EOC;
}

BOOL SEGMENT_EXT_CODE dlEOC_DataWarning(void)
{
	return g_EOCDataWarning;
}

