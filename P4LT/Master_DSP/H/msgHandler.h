/*-------------------------------------------------------------------------*
 * $Workfile: msgHandler.h $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: 
 * Remarks	: ETL500 Transmission protocols, TM-NKT98-1576/_ 
 * Purpose	: Definiert die Message Typen für die Baugruppe P4LQ. 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/Master_DSP/H/msgHandler.h $
 * 
 * 10    11-09-07 18:17 Chhamaa
 * msgHandler_getOperationMode() added.
 * 
 * 6     25.08.06 18:55 Chchlee
 * Function R1LAIsPlugged added
 * 
 * 2     17.02.05 18:36 Chhamaa
 * msgHandler_equipmentIsNotInNormalOpMode() added
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 4     29.07.04 6:46 Leeb02
 * 
 * 3     29.07.04 5:27 Leeb02
 * 
 * 2     26.04.04 9:02 Leeb02
 * 
 * 1     22.04.04 10:32 Leeb02
 * 
 * 3     13.04.04 7:31 Leeb02
 * 
 * 2     7.04.04 8:26 Leeb02
 * 
 * 1     5.04.04 14:16 Leeb02
 * 
 * 1     5.04.04 14:16 Leeb02
 * 
 *-------------------------------------------------------------------------*/


#ifndef	_msgHandler_H
#define	_msgHandler_H

/* INCLUDE FILES **************************************************************/

#include "FT12.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef enum 
{
	 e_ExecMsgStatus_Ok,
	 e_ExecMsgStatus_DataNotAvailable,
	 e_ExecMsgStatus_AccessDenied,
	 e_ExecMsgStatus_Deferred,
 	 e_ExecMsgStatus_AwaitingResponse

}
e_ExecMsgStatus;

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/
void msgHandler_init(BOOL (* )(void));
      
void msgHandler_equipmentFirmwareIdentification(void);
e_ExecMsgStatus msgHandler_executeMessage(t_FT12TelegramBuffer *frame);
e_ExecMsgStatus msgHandler_lastDelayedMessageTypeStatus(void);

unsigned long msgHandler_getOperationMode(void);
BOOL msgHandler_equipmentIsNotInNormalOpMode(void);

// to be executed periodically
void msgHandler_executeDelayedMessageTypes(void);

void R1LAIsPlugged(void); // sets the EQUIPMENT_CFG_R1LA_PLUGGED bit in the equipment SWID

#endif                

