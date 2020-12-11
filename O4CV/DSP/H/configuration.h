/*-------------------------------------------------------------------------*
 * $Workfile	: configuration.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/H/configuration.h $
 * 
 * 17    12-03-28 10:56 Chalnot
 * configuration_isLecEnabled added.
 * 
 * 14    07-09-13 14:54 Chrobif
 * configuration_getFxsCfg() added.
 * 
 * 13    07-09-04 14:16 Chalnot
 * configuration_getBasicTimeSlotID() added.
 * 
 * 12    27.08.07 14:52 Chhamaa
 * New function prototype added: configuration_isChannelOn()
 * 
 * 11    07-08-23 16:48 Chrobif
 * configuration_is2wireConfigured() added.
 * 
 * 10    16.08.07 18:57 Chhamaa
 * New functions added: configuration_isMWireInverted(),
 * configuration_isEWireInverted()
 * 
 * 9     25.07.07 12:06 Chhamaa
 * configuration_getDataRate() added.
 * 
 * 8     07-07-17 7:29 Chrobif
 * Functions configuration_get2WireIfType() and
 * configuration_getAfInterfaceMode() added.
 * 
 * 7     9.07.07 15:58 Chhamaa
 * New functions added: configuration_isServicePhoneActive(),
 * configuration_is2wireInterface()
 * 
 * 6     29.06.07 17:54 Chhamaa
 * Functions configuration_get2wireCfg() and configuration_get4wireCfg()
 * added.
 * 
 * 5     07-06-11 12:57 Chrobif
 * O4CV_Const.h included.
 * 
 * 3     23.04.07 14:06 Chhamaa
 * Function configuration_getBoardNumber() added.
 * 
 * 2     5.04.07 11:26 Chrobif
 * Bug fix in VSS log area.
 *
 * 1     4.04.07 15:08 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"
#include "O4CV_Types.h"
#include "O4CV_Const.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {
    
    INT16 nIf;						// number of activated interfaces
    INT16 ix[N_CHANNELS_ON_BOARD];	// array of interface indexes
    
} t_InterfaceCfg;

typedef enum
{

	e_FXS,
	e_FXO,
	e_Error,

} e_2WireIfType;

/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/

extern void configuration_init(void);
extern t_O4CVConfig* configuration_getPCurrentConfig(void);
extern void configuration_set(t_O4CVConfig* srcConfig);
extern UINT16 configuration_getNumberOfChannels(void);
extern INT16 configuration_getBoardNumber(void);
extern UINT8 configuration_getBasicTimeSlotID(void);
extern UINT16 configuration_getDataRate(void);
extern bool configuration_isServicePhoneActive(void);
extern bool configuration_isChannelOn(UINT16 chNr);
extern bool configuration_is2wireInterface(UINT16 chNr);
extern bool configuration_isMWireInverted(UINT16 chNr);
extern bool configuration_isEWireInverted(UINT16 chNr);
extern bool configuration_isLecEnabled(UINT16 chNr);
extern  e_2WireIfType configuration_get2WireIfType(UINT16 chNr);
extern void configuration_get2wireCfg(t_InterfaceCfg *pIfCfg);
extern void configuration_getFxsCfg(  t_InterfaceCfg *pIfCfg);
extern bool configuration_is2wireConfigured(UINT16 chNr);
extern void configuration_get4wireCfg(t_InterfaceCfg *pIfCfg);
extern UINT8 configuration_getAfInterfaceMode(UINT16 chNr);
extern void configuration_getActiveChannels(UINT16 mod600DRIndex, bool activeChannels[N_CHANNELS_ON_BOARD]);


#endif
