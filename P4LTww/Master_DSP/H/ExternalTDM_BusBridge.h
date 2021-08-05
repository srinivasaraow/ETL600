/*-------------------------------------------------------------------------*
 * $Workfile: ExternalTDM_BusBridge.h $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :
 * Purpose      :
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/Master_DSP/H/ExternalTDM_BusBridge.h $
 * 
 * 15    17.01.07 17:51 Chhamaa
 * Function sendCurrentDataRateToO4CV() added.
 * 
 * 13    18.09.06 16:29 Chhamaa
 * Function receiveBasicTimeSlot6() added.
 * 
 * 9     27.09.05 16:12 Chchlee
 * 
 * 8     18.09.05 15:44 Chchlee
 * t_TDM_BusStatus moved to body
 * 
 * 7     16.09.05 14:11 Chchlee
 * Cleaned up
 * 
 * 6     6.07.05 14:03 Chchlee
 * 
 * 5     6.07.05 13:48 Chchlee
 * 
 * 4     30.06.05 7:40 Chchlee
 * Quality
 * 
 * 3     17.05.05 10:03 Chbrbus
 * Supervision rewritten
 * 
 * 2     16.02.05 11:06 Chbrbus
 * Activated SPORT02 (External TDM Bus) supervision same  as in ETL500
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 12    6.01.05 16:01 Chharyt
 * void ExternalTDM_HandlerTx(unsigned long SyncBitForNSD_DSP);
 * 
 * 11    3.01.05 16:32 Chchlee
 * extTDMBusBridge_getPHardTimingTelegramInfoFrom/ToTDM added
 * 
 * 10    7.09.04 7:25 Leeb02
 * 
 * 9     6.09.04 11:54 Leeb02
 * 
 * 8     24.08.04 12:38 Leeb02
 * 
 * 7     18.08.04 17:32 Leeb02
 * state parameter added in init
 * 
 * 6     29.07.04 9:10 Leeb02
 * 
 * 5     26.07.04 15:51 Leeb02
 * 
 * 4     8.07.04 14:59 Bussli02
 * External TDM Bus from Master DSP working (basic time slots tested)
 * 
 * 3     28.06.04 16:27 Bussli02
 * TDM buffer order should now be normal
 * 
 * 2     19.05.04 17:16 Bussli02
 * 
 * 1     13.05.04 10:07 Bussli02
 * 
 *
 *
 *-------------------------------------------------------------------------*/

#ifndef EXTERNALTDM_BUS_BRIDGE_H
#define EXTERNALTDM_BUS_BRIDGE_H


/* INCLUDE FILES **************************************************************/

#include  "FT12.h"
#include  "P4LT_Types.h"
#include  "tdm_ext_types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void  intExtTDMBusBridge_init(t_FT12TelegramBuffer ** receiveBuffer,     //[in]
		  				      t_FT12TelegramBuffer ** transmitBuffer,    //[in]
						      e_FT12State          *  pState);           //[in]

void intExtTDMBusBridge_initMapping(unsigned long * tx_channel_mask_0);  //[out]

t_TDM_HardTimingTelegramInformation * extTDMBusBridge_getPHardTimingTelegramInfoFromTDM(void);
t_TDM_HardTimingTelegramInformation * extTDMBusBridge_getPHardTimingTelegramInfoToTDM(void);

void ExternalTDM_HandlerTx(unsigned long SyncBitForNSD_DSP);            // to be called in 10kHz task
void ExternalTDM_HandlerRx(void);                                       // to be called in 10kHz task

void E_and_M_WireBridge(void);                                          // to be called in 1kHz task
void doSlowMuteBridge(void);                                            // to be called in 100Hz task

void sendCurrentDataRateToO4CV(void);									// to be called in 100Hz task

void Check_SPORT_TDM_Bus(void);                                         // to be called in 10kHz task
void CheckExternalTDM_Bus(void);                                        // to be called in 1kHz task
void CheckAndRestartExternalTDM_Bus(t_P4LT_Status *P4LT_SystemStatus);  // to be called in 1kHz task

void receiveBasicTimeSlot6(unsigned long *basicTimeSlot);


#endif
