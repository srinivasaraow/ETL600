/*-------------------------------------------------------------------------*
 * $Workfile: bridge.h $
 * Part of	: ETl500 R2/3
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/H/bridge.h $
 * 
 * 2     1.09.05 15:37 Chchlee
 * comments added
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 7     29.07.04 7:05 Leeb02
 * 
 * 6     29.07.04 6:55 Leeb02
 * 
 * 5     19.07.04 8:51 Leeb02
 * "controlBridge" now public.
 * 
 * 4     27.04.04 10:47 Leeb02
 * 
 * 3     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 14:08 Leeb02
 *-------------------------------------------------------------------------*/
#ifndef		_BRIDGE
#define		_BRIDGE

#include "basetype.h"


void bridge_init(void);
void bridge_activate(void); // must be called in 5.33kHz task

#endif
