/*-------------------------------------------------------------------------*
 * $Workfile: TDM_Service.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/Master_DSP/H/TDM_Service.H $ 
 * 
 * 43    11-02-10 9:47 Chhamaa
 * e_TDMServiceMsgType_MUXSetVoiceTransitTestCfg_Send added.
 * 
 * 42    9.06.10 11:56 Chhamaa
 * e_TDMServiceMsgType modified.
 * 
 * 39    08-02-25 9:59 Chtognu
 * Merged changes from ad.equ.branch:
 * ad.Eq. Logger communication added
 * 
 * 34    5.10.05 17:49 Chchlee
 * Comments added
 * 
 * 33    28.06.05 11:57 Chchlee
 * Cleaned up, ASSERTs consolidated
 * 
 * 32    24.02.05 11:58 Chhamaa
 * e_TDMServiceMsgType_CommissioningGetRxTestToneLevel_Resp added
 * 
 * 31    16.02.05 10:31 Chhamaa
 * Msg e_TDMServiceMsgType_CommissioningCfg_Send added
 * 
 * 30    14.02.05 17:41 Chhamaa
 * Test PLC link messages renamed
 * 
 * 29    28.01.05 9:59 Chpeplu
 * 3 new messages (NSD_Read_TestsignalConfi, NSD_Write_TestsignalConfi*)
 * added in e_TDMServiceMsgType 
 * 
 * 28    23.12.04 11:05 Chchlee
 * Upload / Download of testswitches added
 * 
 * 27    20.12.04 15:38 Chpeplu
 * in struct e_TDMServiceMsgType members
 * e_TDMServiceMsgType_NSD_Read_Developer_Status_Req,
 * e_TDMServiceMsgType_NSD_Read_Developer_Status_Resp added
 * 
 * 26    18.11.04 15:06 Plueer01
 * e_TDMServiceMsgType_NSDLooptest_GetResult_Res added
 * 
 * 25    16.11.04 17:23 Leeb02
 * e_TDMServiceMsgType_CS_SetParameter_Send replaces
 * e_TDMServiceMsgType_CS_SetParameter_Req
 * 
 * 24    15.11.04 17:21 Maag01
 * e_TDMServiceMsgType_SamplingCfg_Send added
 * 
 * 23    12.11.04 8:48 Leeb02
 * e_TDMServiceMsgType_CS_SetParameter_Req added
 * 
 * 22    12.11.04 8:35 Leeb02
 * CS_Get/SetParameter_Req added
 * 
 * 21    9.11.04 7:55 Leeb02
 * Some new message types added for NSD
 * 
 * 20    29.09.04 9:37 Maag01
 * 
 * 18    23.09.04 11:12 Leeb02
 * OFDM Status message types added
 * 
 * 17    23.09.04 8:40 Leeb02
 * TDM Service Initiator can now receive messages which are <= reveice
 * buffer size
 * 
 *-------------------------------------------------------------------------*/

#ifndef TDM_SERVICE_H
#define TDM_SERVICE_H


/* INCLUDE FILES **************************************************************/

#include "BASETYPE.H"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/***************************************************************/
/******                                                   ******/
/******        P4LT TDM service message types             ******/
/******        for each DSP on the P4LT board             ******/
/******                                                   ******/
/****** Reference Doc no.: TM-PTUKT03-1637  11/04/2003    ******/
/******                    TM-PTUKT03-1637  12/05/2003    ******/ 
/******                                                   ******/
/***************************************************************/


typedef enum  // CAUTION: do not change sequence unless you harmonized it with OFDM modems implementation!
{
	e_TDMServiceMsgType_None,

	e_TDMServiceMsgType_Cfg_Send,

	e_TDMServiceMsgType_OperationMode_Send,

	e_TDMServiceMsgType_SamplingCfg_Send,

	e_TDMServiceMsgType_StatusData_Req,

	e_TDMServiceMsgType_OFDMShortStatusData_Req,
	e_TDMServiceMsgType_OFDMLongStatusData_Req,

	e_TDMServiceMsgType_SignalBuffer_Req,
	e_TDMServiceMsgType_IdleCounter_Req,

	e_TDMServiceMsgType_Ack,
	e_TDMServiceMsgType_Nack,

	e_TDMServiceMsgType_FE_StatusData_Resp,
	e_TDMServiceMsgType_UC_StatusData_Resp,
	e_TDMServiceMsgType_NSD_StatusData_Resp,
	e_TDMServiceMsgType_NSK_StatusData_Resp,

	e_TDMServiceMsgType_OFDMShortStatusData_Resp,
	e_TDMServiceMsgType_OFDMLongStatusData_Resp,

	e_TDMServiceMsgType_FE_SignalBuffer_Resp,

	e_TDMServiceMsgType_IdleCounter_Resp,

	e_TDMServiceMsgType_NSDLooptest_Trigger_Req,
	e_TDMServiceMsgType_NSDLooptest_GetResult_Req,
	e_TDMServiceMsgType_NSD_SetLocalLoop_Req,

	e_TDMServiceMsgType_CS_GetParameter_Req,  // response is _CS_GetParameter_Res or _Nack
	e_TDMServiceMsgType_CS_GetParameter_Res,

	e_TDMServiceMsgType_CS_SetParameter_Send,  // response is _Ack or _Nack

	e_TDMServiceMsgType_NSDLooptest_GetResult_Res,

	e_TDMServiceMsgType_NSD_Read_Developer_Status_Req,
	e_TDMServiceMsgType_NSD_Read_Developer_Status_Resp,

	e_TDMServiceMsgType_GetTestPlcLinkCfg_Req,  // response is _GetTestSwitch_Res or _Nack
	e_TDMServiceMsgType_GetTestPlcLinkCfg_Res,

	e_TDMServiceMsgType_SetTestPlcLinkCfg_Send,  // response is _Ack or _Nack

	e_TDMServiceMsgType_NSD_Read_TestsignalConfig_Req,
	e_TDMServiceMsgType_NSD_Read_TestsignalConfig_Resp,

	e_TDMServiceMsgType_NSD_Write_TestsignalConfig_Req,

	e_TDMServiceMsgType_CommissioningCfg_Send,
	e_TDMServiceMsgType_CommissioningGetRxTestToneLevel_Resp,

	e_TDMServiceMsgType_NSKShortStatusData_Req,
	e_TDMServiceMsgType_NSKShortStatusData_Resp,

	e_TDMServiceMsgType_NSKGetAdapEquTF_Req,		// NSK adaptive equalizer transfer function
	e_TDMServiceMsgType_NSKGetAdapEquTF_Resp,

	e_TDMServiceMsgType_MUXGetVoiceTransitStatus_Req,	// MUX voice transit developer status
	e_TDMServiceMsgType_MUXGetVoiceTransitStatus_Resp,

	e_TDMServiceMsgType_MUXSetVoiceTransitTestCfg_Send	// response is _Ack or _Nack

}
e_TDMServiceMsgType;


// Caution!!! Don't modify unless you know what yout doing :-)
// Initiator DSP (Master) must have lowest address
// All non-initiator DSP must follow. There must be no gaps.

typedef enum
{
	e_TDMServiceAddr_Invalid		= 0,

	e_TDMServiceAddr_First		    = 1,

	e_TDMServiceAddr_Master		    = 1,
	e_TDMServiceAddr_FirstNI        = 2,
	e_TDMServiceAddr_FE	            = 2,
	e_TDMServiceAddr_UC    		    = 3,
	e_TDMServiceAddr_OFDM	    	= 4,
	e_TDMServiceAddr_NSD			= 5,
	e_TDMServiceAddr_NSK			= 6,

	e_TDMServiceAddr_LastNI		    = 6,
	e_TDMServiceAddr_Last		    = 6

}
e_TDMServiceAddr;

typedef enum
{
	e_TDMServiceResult_Ok,
	e_TDMServiceResult_Timeout,
	e_TDMServiceResult_Error,
	e_TDMServiceResult_ChecksumError
}
e_TDMServiceResult;

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/*********************************************************************************************************************
*
* FUNCTION TYPE t_TDMServiceIConfirm
*
* PARAMETERS
*     result	   [in]  result of request/response primitive. Possible values are:
*     Return Value       e_TDMServiceResult_Ok: success
*                        e_TDMServiceResult_Error: if 
*                                - received message too big
*                                - protocol violation
*                        e_TDMServiceResult_ChecksumError: checksum error
*                        e_TDMServiceResult_Timeout: reply timeout
*
*     rxMsgType    [in]  message type received in response. Only valid if result
*                        is e_TDMServiceResult_Ok.
*     rxMsgSize    [in]  actual received message size.
*     Return Value       none
*
* GLOBALS USED  none
*
* ABSTRACT If TDM service calls this function, the initiator may issue the next
*          request by calling tdmServiceI_Request.
*          It is legal to call tdmServiceI_Request in this function context.
*
* PRECONDITIONS
*         none
*
* NOTES
*
*
*********************************************************************************************************************/
typedef void (*t_TDMServiceIConfirm)(e_TDMServiceResult		result,
									 e_TDMServiceMsgType	rxMsgType,
									 unsigned long          rxMsgSize);  
                                     
/*********************************************************************************************************************
*
* FUNCTION TYPE t_TDMServiceNIReceive
*
* PARAMETERS
*     rxMsgType    [in]  message type received in request. 
*     rxMsgSize    [in]  actual received message size.
*     Return Value       none
*
* GLOBALS USED  none
*
* ABSTRACT On reception of a message TDM service calls this function function reporting
*          the actual received message size and the received message type.
*          The non-initiator shall process the request and prepare a reponse calling 
*          tdmServiceNI_Reply. 
*          It is legal to call tdmServiceNI_Reply in this function context.
*
* PRECONDITIONS
*         none
*
* NOTES
*
*
*********************************************************************************************************************/
typedef void (*t_TDMServiceNIReceive)(	e_TDMServiceMsgType rxMsgType,
										unsigned long       rxMsgSize);  

/*********************************************************************************************************************
*
* FUNCTION TYPE t_TDMServiceNIConfirm
*
* PARAMETERS
*     none
*     Return Value       none
*
* GLOBALS USED  none
*
* ABSTRACT If TDM service calls this function, the non-initiator may accept the next
*          message by calling tdmServiceNI_Receive.
*          It is legal to call tdmServiceNI_Receive in this function context.
*
* PRECONDITIONS
*         none
*
* NOTES
*
*
*********************************************************************************************************************/
typedef void (*t_TDMServiceNIConfirm)(void);  


/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/


//*********************************************************************************************************************
//*********************************************************************************************************************
// Interface for initiator
//*********************************************************************************************************************
//*********************************************************************************************************************

/*********************************************************************************************************************
*
* FUNCTION tdmServiceI_Init
*
* PARAMETERS
*     confirm      [in]  pointer to confirmation function that will be called on completion
*
* GLOBALS USED  none
*
* ABSTRACT Initiates the initiator services.
*
* PRECONDITIONS
*         none.
*
*
* NOTES
*
*
*********************************************************************************************************************/
void tdmServiceI_Init(t_TDMServiceIConfirm	confirm);


/*********************************************************************************************************************
*
* FUNCTION tdmServiceI_Request
*
* PARAMETERS
*     destAddr     [in]  destination address
*     txMsgType    [in]  message type to send
*     pTxData      [in]  pointer to user data to send
*     txDataSize   [in]  size of user data to send in unsigned long units
*     pRxData      [in]  pointer to user data to receive
*     rxDataSize   [in]  size of user data to receive in unsigned long units.
*                        The size must exactly fit the expected received data.
*     replyTimeout [in]  reply timeout in number of activations
*     Return Value       none
*
* GLOBALS USED  none
*
* ABSTRACT Initiates a request/response primitive at the initiator side.
*          Only one request/response primitive can be handles at a time. Therefore 
*          the confirmation function must be awaited before issuing the next request.
*          It is legal to initiate the next request in the confirmation function context.
*
* PRECONDITIONS
*         tdmServiceI_Init called.
*
*
* NOTES
*
*
*********************************************************************************************************************/
void tdmServiceI_Request(   e_TDMServiceAddr		destAddr, 
							e_TDMServiceMsgType		txMsgType,
							unsigned long *			pTxData,
							unsigned long			txDataSize,
							unsigned long *			pRxData,
							unsigned long			rxDataSize,
							unsigned long			replyTimeout);

/*********************************************************************************************************************
*
* FUNCTION tdmServiceActivateInitiator
*
* PARAMETERS
*     pRxBuf       [in]  pointer to current TDM rx buffer
*     pTxBuf       [in]  pointer to current TDM tx buffer
*
* GLOBALS USED  none
*
* ABSTRACT This function implements the actual state machine of the initiator.
*          This function must be called in every TDM bus cycle.
*          The current TDM rx buffer and tx buffer is passed.                    
*
* PRECONDITIONS
*         tdmServiceI_Init called.
*
*
*
*********************************************************************************************************************/
void tdmServiceActivateInitiator    (unsigned long * pRxBuf, unsigned long * pTxBuf);

//*********************************************************************************************************************
//*********************************************************************************************************************
// Interface for non-initiator
//*********************************************************************************************************************
//*********************************************************************************************************************

/*********************************************************************************************************************
*
* FUNCTION tdmServiceNI_Init
*
* PARAMETERS
*     pRxData       [in]  pointer to user data to receive
*     MaxRxDataSize [in]  maximum accepted size of user data to receive in unsigned long units.
*     receive       [in]  pointer to function that will be called on reception of a request
*     confirm       [in]  pointer to function that will be called on completion of the reply
*     Return Value        none
*
* GLOBALS USED  none
*
* ABSTRACT Initiates the non-initiator services.
*
* PRECONDITIONS
*         none
*
*
* NOTES
*
*
*********************************************************************************************************************/
void tdmServiceNI_Init( e_TDMServiceAddr            myAddr,
						unsigned long *				pRxData,
						unsigned long 				maxRxDataSize,
						t_TDMServiceNIReceive	    receive,
						t_TDMServiceNIConfirm	    confirm);


/*********************************************************************************************************************
*
* FUNCTION tdmServiceNI_Receive
*
* PARAMETERS    none
*
* GLOBALS USED  none
*
* ABSTRACT The non-initiator calls this fuction if it's ready to receive a request.
*          On a reception of a message TDM service calls the receive function reporting
*          the actual received message size and the received message type.
*          The non-initiator shall process the request and prepare a reponse calling 
*          tdmServiceNI_Reply.
*
* PRECONDITIONS
*         tdmServiceNI_Init called.
*
*
* NOTES
*
*
*********************************************************************************************************************/
void tdmServiceNI_Receive(void);

/*********************************************************************************************************************
*
* FUNCTION tdmServiceNI_Reply
*
* PARAMETERS
*     pTxData       [in]  pointer to user data to send in the response
*     txDataSize    [in]  size of user data to send in unsigned long units.
*     txMsgType     [in]  message type to send
*     Return Value       e_TDMServiceResult_Ok on success
*                        e_TDMServiceResult_Error if 
*                                - this function is called in an invalid state
*                                 (TDM service not initialized or not waiting for a reply)
*
* GLOBALS USED  none
*
* ABSTRACT The non-initiator calls this fuction to send the reply to an pending request.
*          TDM service idicated send completion by called the confirm function.
*          Note that the TDM service will wait forever for the tdmServiceNI_Reply. Only
*          the initiator provides a timeout.                    
*
* PRECONDITIONS
*         tdmServiceNI_Init called.
*
*
* NOTES
*
*
*********************************************************************************************************************/
e_TDMServiceResult tdmServiceNI_Reply  (
					unsigned long *				pTxData,
					unsigned long				txDataSize,
					e_TDMServiceMsgType			txMsgType);



/*********************************************************************************************************************
*
* FUNCTION tdmServiceActivateNonInitiator
*
* PARAMETERS
*     pRxBuf       [in]  pointer to current TDM rx buffer
*     pTxBuf       [in]  pointer to current TDM tx buffer
*
* GLOBALS USED  none
*
* ABSTRACT This function implements the actual state machine of the non-initiator.
*          This function must be called in every TDM bus cycle.
*          The current TDM rx buffer and tx buffer is passed.                    
*
* PRECONDITIONS
*         tdmServiceNI_Init called.
*
*
*
*********************************************************************************************************************/
void tdmServiceActivateNonInitiator (unsigned long * pRxBuf, unsigned long * pTxBuf);

#endif
