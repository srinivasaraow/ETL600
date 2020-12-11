/*-------------------------------------------------------------------------*
 * $Workfile: DL_EOC.H $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/H/DL_EOC.H $
 * 
 * 4     1.09.05 16:20 Chchlee
 * dlEOL... replaced by dlEOC...
 * 
 * 3     30.06.05 8:20 Chchlee
 * DL_EOC_Monitor now only surveils one channel
 * 
 * 2     17.02.05 17:11 Chchlee
 * Improved monitoring concept for EOC
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 11    30.12.04 11:40 Chchlee
 * DL_EOC_Monitor name modified
 * 
 * 10    17.12.04 17:23 Chhamaa
 * 
 * 9     11.11.04 10:25 Maag01
 * 
 * 8     4.11.04 11:07 Maag01
 * 
 * 7     5.08.04 15:38 Leeb02
 * 
 * 6     29.07.04 4:50 Leeb02
 * 
 * 5     20.07.04 13:05 Leeb02
 * 
 * 4     27.04.04 10:47 Leeb02
 * 
 * 3     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 14:08 Leeb02
 *--------------------------------------------------------------------------*/

#ifndef	_DL_EOC_H
#define	_DL_EOC_H


#include "ft12.h"
#include "tdmEocData.h"


void initDataLinkLayerEOC(t_FT12TelegramBuffer ** receiveBufferCh1,
		  				  t_FT12TelegramBuffer ** transmitBuffer,
				          e_FT12State           * pState);

 
void ToDataLinkLayerEOC  (EocDataBufferType * txBuffer,     // to activate in 0.1067 kHz task
						  BOOL                MWireActive, 
						  unsigned long       aplcChannel);

void FromDataLinkLayerEOC(EocDataBufferType * rxBuffer,     // to activate in 0.1067 kHz task
						  BOOL                EWireActive, 
						  unsigned long       aplcChannel);

void MakeTxFastStatusTelEOC(unsigned long aplcChannel);         // to activate in 0.1067 kHz task 
void HandleSendNoReply(     unsigned long aplcChannel);         // to activate in 0.1067 kHz task 
void DL_EOC_Monitor        (void);                              // to activate in 0.1067 kHz task 

BOOL dlEOC_DataTxWarning(void);
BOOL dlEOC_DataRxWarning(void);
BOOL dlEOC_DataWarning(void);


#endif
