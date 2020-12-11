/*-------------------------------------------------------------------------*
 * $Workfile: voiceData.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Public type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/O4CV/DSP/H/voiceData.h $
 * 
 * 10    22.09.08 14:34 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 9     08-06-16 14:13 Chrobif
 * Debugging of Problem 447 showed that voiceData_receive() is not robust
 * to corrupt data rate indices in basic time slot. This modification
 * improves this aspect.
 * 
 * 8     07-09-28 11:28 Chrobif
 * + Implementation of appAction unpack/interpolate/hold.
 * + Implementation of statistics gathering and bit error insertion (test
 * infrastructure)
 * + Implementation of receiver restart if MOD600 is out of sync or last
 * received subframe is older than 15ms
 * 
 * 7     07-07-20 16:14 Chrobif
 * Comment modified.
 * 
 * 6     07-07-20 15:53 Chrobif
 * inSync member in t_RxFrameForDecoder replaced by ready member
 * 
 * 5     27.06.07 17:51 Chhamaa
 * Global variable declarations added.
 * 
 * 4     07-06-22 11:20 Chrobif
 * voiceData_initRx() and voiceData_receive() added to API
 * 
 * 3     07-06-12 7:49 Chrobif
 * Interface voiceData_getRxFrames() modified.
 * 
 * 2     30.05.07 10:56 Chrobif
 * Typedef for t_RxFrameForDecoder and getter voiceData_getRxFrame added.
 * 
 * 1     2.05.07 10:21 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef VOICE_DATA_H
#define VOICE_DATA_H

/* INCLUDE FILES ***********************************************************/

#include "Basetype_O4CV.h"
#include "O4CV_Const.h"


/* PUBLIC CONSTANT DECLARATIONS ********************************************/

#define SUBFRAME_LEN	9		// length in bytes


/* PUBLIC TYPE DECLARATIONS ************************************************/

typedef UINT8 t_Subframe[SUBFRAME_LEN];

typedef enum {

	e_Unpack,
	e_Interpolate,
	e_Hold,
	e_Undefined,

} e_Action;

typedef struct {
    
    t_Subframe *pSubframe1; // Pointer to first subframe
    t_Subframe *pSubframe2; // Pointer to second subframe
    t_Subframe *pSubframe3; // Pointer to third subframe
    UINT16 iMin; // Frame type
    bool ready;  // True, if there is a frame ready for decoding. False, otherwise.
	e_Action appAction; // Action to be taken by the application
    
} t_RxFrameForDecoder;

typedef struct {

	UINT32 unpackCounter[N_CHANNELS_ON_BOARD];
	UINT32 interpolateCounter[N_CHANNELS_ON_BOARD];
	UINT32 holdCounter[N_CHANNELS_ON_BOARD];

} t_VoiceDataStatsRx;

/* GLOBAL VARIABLE DECLARATIONS ********************************************/

extern t_Subframe *g_pSubframeBufferTxCh[N_CHANNELS_ON_BOARD];

extern bool g_decoderDataReady;
extern bool g_receiverInSync;

/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

void voiceData_initTx(void);
void voiceData_startTx(void);
void voiceData_send(UINT16 *pTxTimeSlot);
void voiceData_initRx(void);
void voiceData_receive(UINT16 *pRxTimeSlot, bool cSValid);
void voiceData_getRxFrames(t_RxFrameForDecoder rxFrames[N_CHANNELS_ON_BOARD]);

#endif

