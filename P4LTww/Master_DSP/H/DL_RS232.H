/*-------------------------------------------------------------------------*
 * $Workfile: DL_RS232.H $
 * Part of	:	ETL500 R2/3
 * Language	:	C
 * Created by:   
 * Remarks	:	                    
 * Purpose	: 
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*    
 * $Log: /Source/P4LT/Master_DSP/H/DL_RS232.H $                                  
 * 
 * 2     1.09.05 15:39 Chchlee
 * comments edded
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 5     29.07.04 4:50 Leeb02
 * 
 * 4     20.07.04 13:05 Leeb02
 * 
 * 3     13.04.04 7:30 Leeb02
 * 
 * 1     5.04.04 14:08 Leeb02
 *-------------------------------------------------------------------------*/
#ifndef	_DL_RS232
#define	_DL_RS232


#include "basetype.h"
#include "ft12.h"

BOOL init_DL_RS232(unsigned long           baudrate, 
				   t_FT12TelegramBuffer ** receiveBuffer,
				   t_FT12TelegramBuffer ** transmitBuffer,
				   e_FT12State          *  pState);

void ToDataLinkLayerRS232(void);    // must be called in 5.33kHz   task
void FromDataLinkLayerRS232(void);  // must be called in 10.667kHz task




#endif
