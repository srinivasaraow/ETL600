/*-------------------------------------------------------------------------*
 * $Workfile	: appControl.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The appControl module hosts the state machines for the
 * 				  2-wire and 4-wire channels and the service phone.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/appControl.h $
 * 
 * 6     07-11-27 16:23 Chrobif
 * + Wait state for remote subscriber added.
 * + appControl_getSMState() added.
 * 
 * 5     07-09-13 14:57 Chrobif
 * + Prototype of appControl_activate() modified.
 * + t_TelephonyIFData made public for developer status upload.
 * 
 * 4     07-08-24 10:49 Chalnot
 * Round robin ringing function added.
 * 
 * 3     07-08-23 16:50 Chrobif
 * Software counter for 12 second interuption timeout added. Call
 * initiator added to state machine data struct. Activation of DuSlics
 * added. Parameter rxSubframes introduced in appControl_activate(). FXS
 * state machine implemented.
 * 
 * 2     07-07-20 16:29 Chrobif
 * appControl_isServicePhoneActive() removed.
 * 
 * 1     07-07-13 16:56 Chrobif
 * Initial version
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef APP_CONTROL_H
#define APP_CONTROL_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"
#include "voiceData.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
/* PUBLIC TYPE DECLARATIONS ***************************************************/
typedef enum
{

	e_NoCall,
	e_Local,
	e_Remote,

} e_CallInitiator;

typedef enum
{
    
    e_Idle,
    e_LocalInitRequest,
    e_RemoteInitRequest,
    e_Connected,
    e_LocalTerminationRequest,
    e_RemoteTerminationRequest,
    e_LineNotAvailable,
    e_LineInterruption,
    e_Wait,
    
} e_TelephonyIFStates;

typedef struct
{
    
    e_TelephonyIFStates state;
    UINT8 channel;
    UINT8 afIFMode;
	bool isInterrupted;
	bool isInterruptionLong;
	e_CallInitiator initiator;
	UINT16 waitCounter;
    
} t_TelephonyIFData;

/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
extern void appControl_init(void);
extern void appControl_activate(void);
extern void appControl_activateRingSequence(void);
extern e_TelephonyIFStates appControl_getSMState(UINT16 chNr);

#endif

