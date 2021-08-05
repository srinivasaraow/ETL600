/*-------------------------------------------------------------------------*
 * $Workfile: TDM_Service.C $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/FE_DSP/C/TDM_Service.C $ 
 * 
 * 3     11.10.05 16:43 Chchlee
 * t_AllDSP_input_tdm_block used for receive direction
 * 
 * 2     28.06.05 11:56 Chchlee
 * Cleaned up, ASSERTs consolidated
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 23    17.11.04 10:30 Leeb02
 * 
 * 22    23.09.04 8:39 Leeb02
 * TDM Service Initiator can now receive messages which are <= reveice
 * buffer size
 * 
 * 21    16.09.04 14:30 Leeb02
 * Extra functionality for OFDM Modem added
 * 
 * 20    13.08.04 9:01 Leeb02
 * external RAM handling moved to basetype.h
 * 
 * 19    12.08.04 18:40 Maag01
 * 
 * 18    12.08.04 13:37 Maag01
 * Modified to use without external RAM. NO_EXT_RAM has to be defined.
 * 
 * 17    3.08.04 17:01 Leeb02
 * Activate functions are now in external mem
 * 
 * 16    3.08.04 8:02 Leeb02
 * SEGMENT_EXT_CODE added to init functions
 * 
 * 15    28.07.04 16:20 Maag01
 * 
 * 14    28.07.04 8:34 Leeb02
 * initiator init modified
 * 
 * 13    28.07.04 8:19 Leeb02
 * Less parameters passed at runtime
 * 
 * 12    26.07.04 18:29 Leeb02
 * 
 * 11    23.07.04 13:47 Leeb02
 * 
 * 10    23.07.04 13:44 Leeb02
 * e_TDMServiceMsgType_None
 * 
 * 9     27.04.04 10:47 Leeb02
 * e_TDMServiceMsgType added
 * 
 * 8     19.04.04 11:36 Leeb02
 * harmonized with tdm_int_types.h
 * 
 * 7     7.04.04 11:22 Leeb02
 * Basetypes removed
 * 
 * 6     7.04.04 8:18 Leeb02
 * Checksum (simple addition) added. Reintroduced slot offsets
 * 
 * 5     04-03-17 8:07 Leeb02
 * Completely tested under MS C++
 * 
 * 4     04-03-15 7:40 Leeb02
 * Slot offsets removed
 * 
 * 3     04-03-10 7:33 Leeb02
 * First complete version. Without CRC, yet
 * 
 * 2     04-03-08 7:31 Leeb02
 * Initial version
 * 
 * 1     04-03-05 12:49 Leeb02
 * Initial working version
 *-------------------------------------------------------------------------*/

/*
remark:
  checksum is a simple addition of data values. Use CRC in future

  sicherheit: call/reply paarung bei timeout

*/


/* INCLUDE FILES **************************************************************/

#include <stddef.h>

#include "TDM_SERVICE.H"
#include "DIAG.H"
#include "tdm_int_types.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define TDM_SERVICE_START_CHAR		(unsigned long)0x68000068
#define TDM_SERVICE_END_CHAR		(unsigned long)0x16000016

// this should work on all compilers. Tested on VDSP and MS C++
#define TDM_SERVICE_HEADER_SIZE (sizeof(t_TDMServiceHeader) / sizeof(unsigned long))

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef enum
{
	e_TDMServiceState_INotInitialized,

	e_TDMServiceState_IIdle,

	e_TDMServiceState_IStartHeader,
	e_TDMServiceState_ISendingHeader,
	e_TDMServiceState_ISendingData,
	e_TDMServiceState_ISendingCRC,

	e_TDMServiceState_IAwaitReply,
	e_TDMServiceState_IReceivingHeader,
	e_TDMServiceState_IReceivingData,
	e_TDMServiceState_IReceivingCRC,
	e_TDMServiceState_IReceivingEndChar,
}
e_TDMServiceInitiatorStates;

typedef enum
{
	e_TDMServiceState_NINotInitialized,

	e_TDMServiceState_NIIdle,
	e_TDMServiceState_NIAwaitRequest,

	e_TDMServiceState_NIReceivingHeader,
	e_TDMServiceState_NIReceivingData,
	e_TDMServiceState_NIReceivingCRC,
	e_TDMServiceState_NIReceivingEndChar,

	e_TDMServiceState_NIAwaitReply,
	e_TDMServiceState_NIStartHeader,
	e_TDMServiceState_NISendingHeader,
	e_TDMServiceState_NISendingData,
	e_TDMServiceState_NISendingCRC,
}
e_TDMServiceNonInitiatorStates;

typedef struct
{
	unsigned long				startChar;
	e_TDMServiceAddr	        destAddr;
	e_TDMServiceMsgType			messageType;
	unsigned long				messageSize;
}
t_TDMServiceHeader;

	

/* LOCAL MACRO DEFINITIONS ***************************************************/

#ifdef _WIN32

	#define iSendWord(word, buf) (ItoNI = word);
	#define iReadWord(buf) (NItoI)

	#define niSendWord(word, buf)  (NItoI = word);
	#define niReadWord(buf) (ItoNI)

#else

	#define iSendWord(word, buf)\
        (buf[offsetof(t_Master_output_tdm_block, service_data) / sizeof(unsigned long)] = word)

	#define iReadWord(buf)\
            (buf[g_I_RxSlotOffsetTable[g_I_TxHeader.destAddr-e_TDMServiceAddr_FirstNI]])

	#define niSendWord(word, buf) (buf[g_NI_TxSlotOffset] = word)
	#define niReadWord(buf)       (buf[g_NI_RxSlotOffset])

#endif

#define UpdateChecksum(checksum, word) (checksum += word);



/* LOCAL VARIABLES ***********************************************************/

#ifdef _WIN32
static 	unsigned long ItoNI = 0; 
static 	unsigned long NItoI = 0;
#endif

// initiator
static e_TDMServiceInitiatorStates		g_InitiatorState = e_TDMServiceState_INotInitialized;
static t_TDMServiceIConfirm				g_IConfirm;

static e_TDMServiceMsgType				g_I_TxMsgType;
static unsigned long *					g_I_pTxData;
static unsigned long *					g_I_pRxData;
static unsigned long					g_I_MaxRxDataSize;
static t_TDMServiceHeader				g_I_TxHeader;

static unsigned long					g_ReplyTimeout;

// one slot offset for each non-initiator
const unsigned long g_I_RxSlotOffsetTable [e_TDMServiceAddr_LastNI - e_TDMServiceAddr_FirstNI + 1] = 
{
	offsetof (t_AllDSP_input_tdm_block, FE_DSP_ServiceData_TS09)   / sizeof(unsigned long),
	offsetof (t_AllDSP_input_tdm_block, UpConverter_DSP_ServiceData_TS21)   / sizeof(unsigned long),
	offsetof (t_AllDSP_input_tdm_block, OFDM_DSP_ServiceData_TS17) / sizeof(unsigned long),
	offsetof (t_AllDSP_input_tdm_block, NSD_DSP_ServiceData_TS11)  / sizeof(unsigned long),
	offsetof (t_AllDSP_input_tdm_block, NSK600_DSP_ServiceData_TS19)  / sizeof(unsigned long)
};


// non-initiator
static e_TDMServiceNonInitiatorStates	g_NonInitiatorState = e_TDMServiceState_NINotInitialized;

static t_TDMServiceNIReceive			g_NIReceive;
static t_TDMServiceNIConfirm			g_NIConfirm;

static e_TDMServiceMsgType				g_NI_TxMsgType;
static unsigned long *					g_NI_pTxData; 
static unsigned long *					g_NI_pRxData; 
static unsigned long					g_NI_MaxRxDataSize;
static t_TDMServiceHeader				g_NI_TxHeader;

static e_TDMServiceAddr					g_MyAddr;

static unsigned long                    g_NI_RxSlotOffset;
static unsigned long                    g_NI_TxSlotOffset;

static unsigned long *                  g_NI_pTdmTxBuf;
static unsigned long                    g_NI_TdmTxBufSize;


/* LOCAL FUNCTIONS ***********************************************************/


/* IMPLEMENTATION ************************************************************/

void SEGMENT_EXT_CODE tdmServiceI_Init(t_TDMServiceIConfirm	confirm)
{
	g_InitiatorState              = e_TDMServiceState_IIdle;

	g_I_TxHeader.startChar        = TDM_SERVICE_START_CHAR;

	g_IConfirm					  = confirm;

	ASSERT (confirm); //make sure, call back is valid otherwise we jump to nirvana
}


void SEGMENT_EXT_CODE tdmServiceNI_Init(e_TDMServiceAddr            myAddr,
										unsigned long *				pRxData,
										unsigned long 				maxRxDataSize,
										t_TDMServiceNIReceive	    receive,
										t_TDMServiceNIConfirm	    confirm)
{
	g_NI_pRxData				= pRxData;
	g_NI_MaxRxDataSize			= maxRxDataSize;
	g_NIReceive					= receive;
	g_NIConfirm					= confirm;
	
	ASSERT (receive && confirm); //make sure, call backs are valid otherwise we jump to nirvana

	g_NonInitiatorState      = e_TDMServiceState_NIIdle;

	g_NI_TxHeader.startChar  = TDM_SERVICE_START_CHAR;

	
	// calculate rx slot offset
	g_NI_RxSlotOffset = offsetof (t_AllDSP_input_tdm_block,  MasterDSPServiceData_TS01) / sizeof(unsigned long);

	// calculate tx slot offsets
	switch (g_MyAddr = myAddr)
	{
		case e_TDMServiceAddr_FE : 
			g_NI_TxSlotOffset = offsetof (t_FE_output_tdm_block, service_data) / sizeof(unsigned long);
			break;
		case e_TDMServiceAddr_UC :
			g_NI_TxSlotOffset = offsetof (t_UC_output_tdm_block, service_data) / sizeof(unsigned long);
			break;
		//case e_TDMServiceAddr_OFDM : // not supported
		//	break;
		case e_TDMServiceAddr_NSD : 
			g_NI_TxSlotOffset = offsetof (t_NSD_output_tdm_block, service_data) / sizeof(unsigned long);
			break;
		case e_TDMServiceAddr_NSK : 
			g_NI_TxSlotOffset = offsetof (t_NSK_output_tdm_block, service_data) / sizeof(unsigned long);
			break;

		default: ASSERT (FALSE);
	}


}

void tdmServiceI_Request(   e_TDMServiceAddr		destAddr, 
							e_TDMServiceMsgType		txMsgType,
							unsigned long *			pTxData,
							unsigned long			txDataSize,
							unsigned long *			pRxData,
							unsigned long			rxDataSize,
							unsigned long			replyTimeout)
{
	ASSERT (e_TDMServiceState_IIdle == g_InitiatorState);

	g_I_TxHeader.destAddr         = destAddr;
	g_I_TxHeader.messageType      = txMsgType;
	g_I_TxHeader.messageSize      = txDataSize;

	g_I_pTxData					  = pTxData;
	g_I_pRxData					  = pRxData;
	g_I_MaxRxDataSize				  = rxDataSize;

	g_ReplyTimeout				  = replyTimeout;
	
	g_InitiatorState = e_TDMServiceState_IStartHeader;
}

void tdmServiceNI_Receive(void)
{
	ASSERT (e_TDMServiceState_NIIdle == g_NonInitiatorState);

	g_NonInitiatorState = e_TDMServiceState_NIAwaitRequest;
}


void SEGMENT_EXT_CODE tdmServiceNI_SetTDMTxBuf(unsigned long * pTDMTxBuf, 
											   unsigned long TDMTxBufSize)
{
	// save buffer pointer and maximum size
	g_NI_pTdmTxBuf     = pTDMTxBuf;
	g_NI_TdmTxBufSize  = TDMTxBufSize;
}

e_TDMServiceResult tdmServiceNI_Reply  (unsigned long *		pTxData,
										unsigned long		txDataSize,
										e_TDMServiceMsgType	txMsgType)
{
	if (e_TDMServiceState_NIAwaitReply != g_NonInitiatorState)
	{	
		return e_TDMServiceResult_Error;
	}

	g_NI_pTxData					= pTxData;
	g_NI_TxHeader.messageSize		= txDataSize; 
	g_NI_TxHeader.messageType		= txMsgType;

	g_NI_TxHeader.destAddr          = e_TDMServiceAddr_Master;
	
	g_NonInitiatorState             = e_TDMServiceState_NIStartHeader;

	return e_TDMServiceResult_Ok;
}


void SEGMENT_EXT_CODE tdmServiceActivateInitiator (unsigned long * pRxBuf, unsigned long * pTxBuf)
{
	static unsigned long		wordIndex;
	static unsigned long		tmoCounter;
	static t_TDMServiceHeader	g_RxHeader; // the field "startChar" is never used
	static unsigned long		checksum;

	switch (g_InitiatorState)
	{
		case e_TDMServiceState_IIdle:	// do nothing
			break;

		case e_TDMServiceState_IStartHeader:	
		{
			checksum = 0;
			iSendWord(((unsigned long*)&g_I_TxHeader)[wordIndex = 0], pTxBuf);
			g_InitiatorState = e_TDMServiceState_ISendingHeader;
		}	
		break;

		case e_TDMServiceState_ISendingHeader:	
		{
			if (TDM_SERVICE_HEADER_SIZE == ++wordIndex) 
			{	// complete header sent
				if (0 == g_I_TxHeader.messageSize) 
				{  	// no data to send --> send CRC
					iSendWord(checksum, pTxBuf);
					g_InitiatorState = e_TDMServiceState_ISendingCRC;			
				}				
				else
				{	// send user data
					unsigned long word = g_I_pTxData[wordIndex = 0];
					UpdateChecksum(checksum, word);
					iSendWord(word, pTxBuf);
					g_InitiatorState = e_TDMServiceState_ISendingData;			
				}
			}
			else
			{	// send next word in header
				unsigned long word = ((unsigned long*)&g_I_TxHeader)[wordIndex];
				UpdateChecksum(checksum, word);
				iSendWord(word, pTxBuf);
			}
		}	
		break;

		case e_TDMServiceState_ISendingData:	
		{
			if (++wordIndex == g_I_TxHeader.messageSize) // all data bytes sent. Send CRC
			{
				// send CRC
				iSendWord(checksum, pTxBuf);
				g_InitiatorState = e_TDMServiceState_ISendingCRC;			
			}
			else
			{	
				unsigned long word = g_I_pTxData[wordIndex];
				UpdateChecksum(checksum, word);
				iSendWord(word, pTxBuf);       // send next data word
			}
		}	
		break;

		case e_TDMServiceState_ISendingCRC:	
		{
			iSendWord(TDM_SERVICE_END_CHAR, pTxBuf);
			tmoCounter = 0;	// init timer
			g_InitiatorState = e_TDMServiceState_IAwaitReply;			
		}	
		break;

		case e_TDMServiceState_IAwaitReply:	
		{
			if (TDM_SERVICE_START_CHAR == iReadWord(pRxBuf))
			{
				checksum = 0;
				wordIndex = 1; // start char already read, hence skip it
				g_InitiatorState = e_TDMServiceState_IReceivingHeader;
			}
			else
			{	
				if (++tmoCounter >= g_ReplyTimeout)
				{
					g_InitiatorState = e_TDMServiceState_IIdle;
					g_IConfirm(e_TDMServiceResult_Timeout, e_TDMServiceMsgType_None, 0);  // report failure
				}
			}
		}	
		break;

		case e_TDMServiceState_IReceivingHeader:	
		{
			unsigned long word = iReadWord(pRxBuf); // read next header word
			UpdateChecksum (checksum, word);
			((unsigned long*)&g_RxHeader)[wordIndex] = word; 

			if (TDM_SERVICE_HEADER_SIZE == ++wordIndex)
			{	// complete header received. Check it.

				    // illegal destination address
				if ((e_TDMServiceAddr_Master != g_RxHeader.destAddr)||
				    // or size of reponse too big
				    (g_RxHeader.messageSize > g_I_MaxRxDataSize))
				{	
					g_InitiatorState = e_TDMServiceState_IIdle;
					g_IConfirm(e_TDMServiceResult_Error, e_TDMServiceMsgType_None, 0);  // report failure
				}
				else
				{
					if (0 == g_RxHeader.messageSize)
					{	
						// we expect no data words --> wait for checksum
						g_InitiatorState = e_TDMServiceState_IReceivingCRC;
					}
					else 
					{	
						wordIndex = 0;	// start receiving data
						g_InitiatorState = e_TDMServiceState_IReceivingData;
					}
				}
			}
		}	
		break;

		case e_TDMServiceState_IReceivingData:	
		{
			unsigned long word = iReadWord(pRxBuf); // read next data word
			UpdateChecksum(checksum, word);
			g_I_pRxData[wordIndex] = word;

			if (++wordIndex == g_RxHeader.messageSize)
			{	// complete data received. Wait for CRC.
				g_InitiatorState = e_TDMServiceState_IReceivingCRC;
			}
		}	
		break;

		case e_TDMServiceState_IReceivingCRC:	
		{
			unsigned long receivedChecksum = iReadWord(pRxBuf); // read CRC

			// check CRC
			if (receivedChecksum == checksum)
			{
				g_InitiatorState = e_TDMServiceState_IReceivingEndChar;
			}
			else
			{
				g_InitiatorState = e_TDMServiceState_IIdle;
				g_IConfirm(e_TDMServiceResult_ChecksumError, e_TDMServiceMsgType_None, 0);  // report failure
			}
		}	
		break;

		case e_TDMServiceState_IReceivingEndChar:	
		{
			g_InitiatorState = e_TDMServiceState_IIdle;

			// read and verify end char
			if (TDM_SERVICE_END_CHAR == iReadWord(pRxBuf))
			{
				g_IConfirm(	e_TDMServiceResult_Ok,    g_RxHeader.messageType, g_RxHeader.messageSize); 
			}
			else
			{
				g_IConfirm(	e_TDMServiceResult_Error, e_TDMServiceMsgType_None, 0); 
			}
		}	
		break;

		default: 
			ASSERT(FALSE);
	}
}


void SEGMENT_EXT_CODE tdmServiceActivateNonInitiator (unsigned long * pRxBuf, unsigned long * pTxBuf)
{
	static unsigned long		wordIndex, checksum;
	static t_TDMServiceHeader	g_RxHeader; // the field "startChar" is never used

	switch (g_NonInitiatorState)
	{
		case e_TDMServiceState_NIIdle:	// do nothing
			break;

		case e_TDMServiceState_NIAwaitRequest:
			if (TDM_SERVICE_START_CHAR == niReadWord(pRxBuf))
			{
				checksum = 0;
				wordIndex = 1; // start char already read, hence skip it
				g_NonInitiatorState = e_TDMServiceState_NIReceivingHeader;
			}
			break;

		case e_TDMServiceState_NIReceivingHeader:	
		{
			unsigned long word = niReadWord(pRxBuf); // read next header word
			UpdateChecksum (checksum, word);
			((unsigned long*)&g_RxHeader)[wordIndex] = word; 

			if (TDM_SERVICE_HEADER_SIZE == ++wordIndex)
			{	// complete header received. Check it
				if ((g_RxHeader.destAddr != g_MyAddr)               || 
					(g_RxHeader.messageSize > g_NI_MaxRxDataSize)   )
				{	// the message is not addressed to me or too big
					g_NonInitiatorState = e_TDMServiceState_NIAwaitRequest;
				}	
				else
				{
					if (0 == g_RxHeader.messageSize)
					{	// no data to receive --> receive CRC
						g_NonInitiatorState = e_TDMServiceState_NIReceivingCRC;
					}
					else
					{	// receive data
						wordIndex = 0;
						g_NonInitiatorState = e_TDMServiceState_NIReceivingData;
					}
				}
			}
		}	
		break;

		case e_TDMServiceState_NIReceivingData:	
		{
			unsigned long word = niReadWord(pRxBuf); // read next data word
			UpdateChecksum(checksum, word);
			g_NI_pRxData[wordIndex] = word;

			if (++wordIndex == g_RxHeader.messageSize)
			{	// complete data received. Receive CRC
				g_NonInitiatorState = e_TDMServiceState_NIReceivingCRC;
			}
		}	
		break;

		case e_TDMServiceState_NIReceivingCRC:	
		{
			unsigned long receivedChecksum = niReadWord(pRxBuf); // read CRC

			// check CRC
			if (receivedChecksum == checksum)
			{
				g_NonInitiatorState = e_TDMServiceState_NIReceivingEndChar;
			}
			else
			{	// error. Go back to idle
				g_NonInitiatorState = e_TDMServiceState_NIAwaitRequest;
			}
		}	
		break;

		case e_TDMServiceState_NIReceivingEndChar:	
		{
			// read and verify end Char
			if (TDM_SERVICE_END_CHAR == niReadWord(pRxBuf)) 
			{
				g_NonInitiatorState = e_TDMServiceState_NIAwaitReply;
				g_NIReceive( g_RxHeader.messageType,
							 g_RxHeader.messageSize);
			}
			else
			{	// error. Go back to idle
				g_NonInitiatorState = e_TDMServiceState_NIAwaitRequest;
			}
		}	
		break;

		case e_TDMServiceState_NIAwaitReply:	
			break;

		case e_TDMServiceState_NIStartHeader:	
		{
			checksum = 0;
			niSendWord(((unsigned long*)&g_NI_TxHeader)[wordIndex = 0], pTxBuf);
			g_NonInitiatorState = e_TDMServiceState_NISendingHeader;
		}	
		break;

		case e_TDMServiceState_NISendingHeader:	
		{
			if (TDM_SERVICE_HEADER_SIZE == ++wordIndex) 
			{	// complete header sent 
				if (0 == g_NI_TxHeader.messageSize)
				{   // no data to send --> send CRC
					niSendWord(checksum, pTxBuf);
					g_NonInitiatorState = e_TDMServiceState_NISendingCRC;			
				}				
				else
				{	// send user data
					unsigned long word = g_NI_pTxData[wordIndex = 0];
					UpdateChecksum(checksum, word);
					niSendWord(word, pTxBuf);
					g_NonInitiatorState = e_TDMServiceState_NISendingData;			
				}
			}
			else
			{	// send next word in header
				unsigned long word = ((unsigned long*)&g_NI_TxHeader)[wordIndex];
				UpdateChecksum(checksum, word);
				niSendWord(word, pTxBuf);
			}
		}	
		break;

		case e_TDMServiceState_NISendingData:	
		{
			if (++wordIndex == g_NI_TxHeader.messageSize) // all data bytes sent.
			{	// send CRC
				niSendWord(checksum, pTxBuf);
				g_NonInitiatorState = e_TDMServiceState_NISendingCRC;			
			}
			else
			{	// send next word of data
				unsigned long word = g_NI_pTxData[wordIndex];
				UpdateChecksum(checksum, word);
				niSendWord(word, pTxBuf);       
			}
		}	
		break;

		case e_TDMServiceState_NISendingCRC:	
		{
			niSendWord(TDM_SERVICE_END_CHAR, pTxBuf);
			g_NonInitiatorState = e_TDMServiceState_NIIdle;			
			g_NIConfirm();
		}	
		break;

		default: 
			ASSERT(FALSE);
	}
}

