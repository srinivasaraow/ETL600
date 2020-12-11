/*-------------------------------------------------------------------------*
 * $Workfile: ATMEL.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/H/ATMEL.H $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 3     14.09.04 16:27 Buhl01
 * 
 * 2     4.05.04 13:33 Leeb02
 * unsigned long -> unsigned char
 * 
 * 1     3.05.04 7:34 Leeb02
 *-------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////////////
///// Kopiere in SPI_functions.h
///////////////////////////////////////////////////////////////////////////////////////


#ifndef ATMEL_H
#define ATMEL_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/



/* PUBLIC TYPE DECLARATIONS ***************************************************/



/* PUBLIC MACRO DEFINITIONS ***************************************************/



/* PUBLIC FUNCTIONS ***********************************************************/

BOOL uart_init(unsigned long baudrate);

BOOL uart_getChar (unsigned char * rxChar);

BOOL uart_txQueueEmpty(void);

void uart_putChar(unsigned char txChar);

#endif

