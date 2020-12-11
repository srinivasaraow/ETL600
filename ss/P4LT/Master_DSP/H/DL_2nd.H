/*-------------------------------------------------------------------------*
 * $Workfile: DL_2nd.H $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/DL_2nd.H $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 3     29.07.04 4:50 Leeb02
 * 
 * 2     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 14:08 Leeb02
 *--------------------------------------------------------------------------*/


#ifndef DL_2ND_H
#define DL_2ND_H

#include "ft12.h"

void initDL_2nd(BOOL (* receiveBuffersEmpty)(void),
				t_FT12TelegramBuffer ** receivedFrame,
                t_FT12TelegramBuffer ** transmitFrame,
				e_FT12State           * pState);

void ApplicationLayer(void);


#endif
