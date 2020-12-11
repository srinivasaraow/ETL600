/*-------------------------------------------------------------------------*
 * $Workfile: FT12.h $
 * Part of	:	LCPLC 
 * Language	:	C
 * Created by:  Bruno Busslinger NKT1 
 * Remarks	:	Global ETL500 System Telegramm Defines , TM-NKT98-1576/_                     
 * Purpose	: 
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*    
 * $Log: /branches/Source AES/P4LT/Master_DSP/H/FT12.h $                                  
 * 
 * 22    25.08.06 19:12 Chchlee
 * new fct. SetSubAddressLAN600Bypass()
 *  * ClearAddressBypassLAN600() renamed to ChangeDestiAddressLAN600
 * 
 * 21    22.08.06 10:04 Chchlee
 * FT12_MAX_HMI_ADDRESS changed to 65534
 * 
 * 20    10.08.06 18:12 Chmibuh
 * new fct. SetSubAddressLAN600Bypass()
 * ClearAddressBypassLAN600() renamed to ChangeDestiAddressLAN600
 * 
 * 18    15.03.06 10:49 Chmabur1
 * 
 * 16    15.03.06 9:57 Chmabur1
 * added defines for PRM and FCV and RES
 * 
 * 15    27.02.06 14:15 Chmibuh
 * 
 * 14    20.02.06 11:51 Chmabur1
 * added defines for hmi address range
 * 
 * 12    1.09.05 15:45 Chchlee
 * IDLE_TIME_ON_RX_LINE doubled
 * 
 * 11    13.08.04 8:51 Leeb02
 * 
 * 10    29.07.04 5:27 Leeb02
 * 
 * 9     29.07.04 4:50 Leeb02
 * 
 * 8     28.07.04 8:04 Leeb02
 * RXFUNCTION_SEND_NOREPLY added
 * 
 * 7     25.07.04 11:44 Leeb02
 * makeFrameHeader: argument type changed to "void *"
 * 
 * 6     22.07.04 13:30 Leeb02
 * Bug in "MESSAGE_START" fixed
 * 
 * 5     28.04.04 19:12 Leeb02
 *-------------------------------------------------------------------------*/
#ifndef	_FT12
#define	_FT12

/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define FT12_START_CHAR						0x68
#define FT12_END_CHAR						0x16


#define MASK_MESSAGE_START  			0xff0000ff
#define MESSAGE_START					(FT12_START_CHAR<<SB24 | FT12_START_CHAR)

#define CRC_AND_END_CHAR				2

#define LENGTH_HEADER	11   // of FT1.2
#define FRAME_OVERHEAD   6
#define LENGTH_MAX		255

// The value of idle time isn´t critical. IEC870 defines a minimum time of 33 "Ruhezustandsbits"
// Defined is a value of 20 * 11bit (20 * 1 word). The function, which is incrementing the idle counter
// is called in realtime process (every 182us).    
// 7.5.97: IDLE_TIME_ON_RX_LINE changed form 20 to 200 because of modem delay between words > some ms. 
// now the delay is 20.8ms to be on the save side!

// In ETL600 this time is changed to 400 because FromDataLinkLayerRS232 is called with 10.667kHz instead of
// 5.33kHz
#define IDLE_TIME_ON_RX_LINE	400


// muss bei eins anfangen, entspricht der Reihenfolge in Table Frame FT1.2 TM-NKT98-1576/_
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

// Control Bits:
// ATTENTION: PRM and FCV are interchanged due to a bug in MMI500 (and therefore
// also in HMI600). The initerchanged implementation ist tested and works,
// therefore we leave it this way...
// See TM-NKT98-1576 and HMI600/Transmit.cpp
#define RES								0x80
#define PRM								0x10
#define FCB								0x20
#define	FCV								0x40

// Function codes for secondary station, gemaess IEC870
#define TXFUNCTION_CONFIRM				0
#define TXFUNCTION_UNKNOWN				1
#define TXFUNCTION_ACCESS_DENIED		6
#define TXFUNCTION_RESPOND_USER_DATA	8
#define TXFUNCTION_DATA_NOT_AVAILABLE	9
#define TXFUNCTION_EQUIPMENT_ID			13

// Function codes from primary station, gemaess IEC870
#define RXFUNCTION_RESET_FCB			1
#define RXFUNCTION_SEND_CONFIRM			3
#define RXFUNCTION_SEND_NOREPLY         4
#define RXFUNCTION_REQUEST_RESPOND		11
#define RXFUNCTION_RESPOND_EQ_ID		14
                    

// HMI600 address range:
#define	FT12_MIN_HMI_ADDRESS 		65001
#define	FT12_MAX_HMI_ADDRESS 		65534



/* PUBLIC TYPE DECLARATIONS ***************************************************/




// - Steuern die Zustände der Data Link Layer ( RS232, EOC, TDM )
// - Sowohl auf dem P4LT und den Slaves (G4AI, O4LE) 
typedef enum 
{
	e_FT12State_Empty,
	e_FT12State_Receiving,
	e_FT12State_Full,
	e_FT12State_Forwarded,
	e_FT12State_Executed,
	e_FT12State_Discarded,
	e_FT12State_Idle,
	e_FT12State_Sending,
	e_FT12State_Transmitted,
	e_FT12State_Deferred
} e_FT12State;


// Data struct for telegram buffer in the ETL500 system 
typedef struct { 
	long      	    ControlField;
	long	        DestinationSubadress;
	long	        MessageType;
	long	        DestinationAdress;
	long	        SourceAdress;
	long	        HopCounter;
	long	        SequenceNumber;
	e_FT12State 	State;
	long	        TelegrammDataArr[66]; // FT1.2 telegram
} t_FT12TelegramBuffer;

/* PUBLIC MACRO DEFINITIONS ***************************************************/

#define FT12_SIZEOF(frame) ((((frame[0]) >>SB24) & MASK_8LSB)- LENGTH_HEADER)	


/* PUBLIC FUNCTIONS ***********************************************************/

void makeFrameHeader(const void * MessageToEquipment, unsigned long sizeInLongs);
BOOL makeFrameUserData(void);
 
void makeFrameHeaderComplete(       unsigned long         txControl,
							 const  t_FT12TelegramBuffer *pReceivedFrame,
							        t_FT12TelegramBuffer *pNewFrame);

void  ChangeHopCounter(t_FT12TelegramBuffer *);
void  ClearAddressBypass(t_FT12TelegramBuffer *);
void  SetAddressBypass(t_FT12TelegramBuffer *);

/////////// New for LAN600 support 
void  ChangeDestinAddressLAN600(t_FT12TelegramBuffer *Frame);
void  SetSubAddressLAN600(t_FT12TelegramBuffer *Frame);
void  SetSubAddressLAN600Bypass(t_FT12TelegramBuffer *Frame);


#endif     

