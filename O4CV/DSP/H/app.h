/*-------------------------------------------------------------------------*
 * $Workfile: app.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Public type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/app.h $
 * 
 * 6     21.09.07 16:57 Chhamaa
 * Function prototype app_setVoiceState() added.
 * 
 * 5     07-09-13 14:53 Chrobif
 * app_getFrameForDecoderPtr() added.
 * 
 * 4     27.08.07 18:23 Chhamaa
 * New functions added: app_faxControl(), app_getStatusPtr()
 * 
 * 3     13.08.07 11:01 Chhamaa
 * Function prototypes added:
 * app_activateRingingTone(UINT16 chNr);
 * void app_activateBusyTone(UINT16 chNr);
 * void app_deactivateCpTone(UINT16 chNr);
 * 
 * 2     12.07.07 10:39 Chhamaa
 * New functions added: app_activateServicePhone(),
 * app_deactivateServicePhone(void), app_getPtrsTFDataMem(void)
 * 
 * 1     4.07.07 11:48 Chhamaa
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
#ifndef APP_H
#define APP_H

/* INCLUDE FILES ***********************************************************/

#include "Basetype_O4CV.h"
#include "voiceData.h"

/* PUBLIC CONSTANT DECLARATIONS ********************************************/

/* PUBLIC TYPE DECLARATIONS ************************************************/

typedef struct
{
    bool initFailure[N_CHANNELS_ON_BOARD_PLUS_SPH];
    bool runtimeFailure[N_CHANNELS_ON_BOARD_PLUS_SPH];
    
} t_AppStatus;


/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

void app_init(t_O4CVConfig *pConfig);

void app_encode(void);
void app_decode(void);

void app_faxControl(void);

void app_setVoiceState(UINT16 chNr);
void app_activateServicePhone(void);
void app_deactivateServicePhone(void);

void app_activateRingingTone(UINT16 chNr);
void app_activateBusyTone(UINT16 chNr);
void app_deactivateCpTone(UINT16 chNr);


UINT16 **app_getTFDataMemPtrs(void);
t_AppStatus *app_getStatusPtr(void);
t_RxFrameForDecoder* app_getFrameForDecoderPtr(void);

#endif

