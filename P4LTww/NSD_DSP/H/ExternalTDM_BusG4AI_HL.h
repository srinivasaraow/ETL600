/*-------------------------------------------------------------------------*
 * $Workfile: ExternalTDM_BusG4AI_HL.h $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :
 * Purpose      :
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/NSD_DSP/H/ExternalTDM_BusG4AI_HL.h $
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 7     13.01.05 15:59 Chharyt
 * Garbage removed
 * 
 * 6     11.01.05 16:33 Chharyt
 * 
 * 5     11.01.05 14:09 Chharyt
 * new Saved_externalTDM_Tx_Ptr in function
 * 
 * 4     9.09.04 17:21 Rytz01
 * MapExternalTDMBusConfigToTimeslotsG4AI: parameter added
 * 
 * 3     24.08.04 13:00 Leeb02
 * MapExternalTDMBusConfigToTimeslotsG4AI: parameter added
 * 
 * 2     9.07.04 15:17 Bussli02
 * Communication with 2 G4AI, static words, tested  and working (N22 and
 * N28, AES FW)
 * 
 * 1     8.07.04 16:18 Bussli02
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef EXTERNALTDM_BUS_G4AI_HL_H
#define EXTERNALTDM_BUS_G4AI_HL_H

#include	"NSD_DSP_Types.h"

void ReceiveDataFromG4AI(long *DataFromG4AI);
void SendDataToG4AI(long *DataToG4AI);
void MapExternalTDMBusConfigToTimeslotsG4AI(unsigned long * tx_channel_mask_0, t_NSD_DSP_Config *Config);

#endif
