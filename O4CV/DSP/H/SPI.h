/*-------------------------------------------------------------------------*
 * $Workfile	: SPI.H  $
 * Part of 	    : ETL600 / O4CV
 * Language     : C
 * Created by   : Vladimir Pogorilyy
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/SPI.h $
 * 
 * 1     07-06-04 11:46 Chvopog
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/

 
#ifndef  __SPI_DEFINED
#define  __SPI_DEFINED

/* INCLUDE FILES **************************************************************/

#include <sys\exception.h>

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define SIZE_SPI_8 			0x0000
// SPI transfer mode
#define TIMOD_DMA_TX 		0x0003
#define TIMOD_INT_TX 		0x0000
// SPI receive mode
#define TIMOD_DMA_RX 		0x0002
#define TIMOD_INT_RX 		0x0001

/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/
void SPI_config(void);
void SPI_bytesSend(unsigned char *Buf, int len);
void SPI_bytesRecieve(unsigned char *Buf, int len);

EX_INTERRUPT_HANDLER(ISR_SPI);

#endif 


