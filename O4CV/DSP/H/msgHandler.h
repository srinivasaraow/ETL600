/*-------------------------------------------------------------------------*
 * $Workfile	: msgHandler.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The msgHandler module executes the messages from the HMI600
 *				  and sets up the responses.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/msgHandler.h $
 * 
 * 2     17.09.07 13:51 Chhamaa
 * Function prototype msgHandler_getNewCfgReceivedFlag() added.
 * 
 * 1     25.05.07 17:15 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"
#include "ft12.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
extern void msgHandler_init(void);
extern void msgHandler_setSWIDEqState(INT32 state);
extern void msgHandler_setLicenseCode(INT32 licenseCode);
extern void msgHandler_executeMsg(UINT16 msgType, UINT32* PtrToRxUD);
extern void msgHandler_respondEqId(void);
extern bool msgHandler_getNewCfgReceivedFlag(void);

#endif
