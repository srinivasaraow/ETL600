/*-------------------------------------------------------------------------*
 * $Workfile: SPI_functions.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/Master_DSP/H/SPI_functions.h $
 * 
 * 6     4.04.06 18:31 Chmibuh
 * new function call of spiManager
 * 
 * 5     17.03.06 10:51 Chmibuh
 * 
 * 3     6.06.05 15:19 Chmibuh
 * getPiggyBackInfo() added
 * 
 * 2     11.04.05 16:57 Chmibuh
 * Alarm monitoring fct. added
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 6     20.10.04 16:30 Buhl01
 * 
 * 5     18.10.04 17:48 Buhl01
 * 
 * 4     15.09.04 10:52 Buhl01
 * 
 * 3     14.09.04 18:24 Buhl01
 * 
 * 2     14.09.04 16:27 Buhl01
 * 
 * 1     1.09.04 14:38 Buhl01
 * initial
 *-------------------------------------------------------------------------*/

#ifndef SPI_FUNCTIONS_H
#define SPI_FUNCTIONS_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "rtc.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/



/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/



/* PUBLIC FUNCTIONS ***********************************************************/
BOOL uart_init(unsigned long baudrate);

BOOL uart_getChar (unsigned char * rxChar);

BOOL uart_txQueueEmpty(void);

void uart_putChar(unsigned char txChar);

void SEGMENT_EXT_CODE init_spiManager(void);

BOOL placeSPIRequest(unsigned long opCode);

BOOL spiManager(BOOL isSPIbus_requested);

BOOL requestSendDataToDDS(unsigned long *sendDataPtr, unsigned long dataLength);

unsigned long SEGMENT_EXT_CODE * spiFunctions_getSpiRecvBufferPtr(void);

unsigned long getPiggyBackInfo(void);

BOOL SEGMENT_EXT_CODE spiFuncM12V_UndervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncM12V_OvervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP12V_UndervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP12V_OvervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP5V_UndervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP5V_OvervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP3_3V_UndervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP3_3V_OvervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP1_8V_UndervAlarm(void);
BOOL SEGMENT_EXT_CODE spiFuncP1_8V_OvervAlarm(void);
#endif
