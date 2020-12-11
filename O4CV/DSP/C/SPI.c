
/*-------------------------------------------------------------------------*
 * $Workfile	: SPI.C
 * Part of      : ETL600 / O4CV 
 * Language     : C
 * Created by   : Vladimir Pogorilyy
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/SPI.c $
 * 
 * 1     07-06-04 11:47 Chvopog
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/

 /* INCLUDE FILES *************************************************************/
#include "SPI.h"
#include "sport1.h"
#include <cdefBF538.h>
#include <ccblkfn.h>
#include <sysreg.h>
/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLE DECLARATIONS ***********************************************/
extern volatile unsigned char *DataPtr;
extern volatile int dataLength;
volatile bool flag_SPIsendfinisched=0;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/


/*****************************************************************************
  Function:      SPI_config(void) 
  Description: 	 The function configures SPI interface. 
  Inputs:      	 None
  Outputs:    	 None                                                                                                                                         
  Return:      	 Nono
  Globals:       None 
  Important:     None                                      	
*******************************************************************************/

void SPI_config(void){
	// Generate 488 kHz from 125 MHz	
	*pSPI0_BAUD = 0x80;			
	// configure spi port
	// SPI DMA write, 8-bit data, MSB first, SPI Master,
	*pSPI0_CTL = SIZE_SPI_8|MSTR|CPOL|CPHA|EMISO|GM|SZ|TIMOD_INT_TX;
}

/*****************************************************************************
  Function:      SPI_bytesSend(unsigned short writeByte)
  Description: 	 The function sends a 8-bit byte over SPI bus
  Inputs:      	 *Buf: pointer to input array; lenwriteByte: number of bytes to be sent
  Outputs:    	 *Buf: pointer to output array. Contains read data.                                                                                                                                       
  Return:      	 Nono
  Globals:       None 
  Important:     The function sends the first byte & enable SPI bus. All other bytes will be sent 
  				 over Interrupt routine.                                     	
*******************************************************************************/


void SPI_bytesSend(unsigned char *Buf, int len)
{
    unsigned char i;
    DataPtr = Buf;
    dataLength = len;
 
    *pSPI_STAT = TXCOL | RBSY | MODF | TXE; // reset errors
   	//enable spi
	*pSPI0_CTL = (*pSPI_CTL | SPE );

    *pSPI_TDBR = *DataPtr++;
    i = *pSPI_RDBR;    // first data dummy read

}

/*****************************************************************************
  Function:      SPI_bytesRecieve(unsigned short writeByte)
  Description: 	 The function recieve a 8-bit byte over SPI bus
  Inputs:      	 *Buf: pointer to input array; lenwriteByte: number of bytes to be sent
  Outputs:    	 *Buf: pointer to output array. Contains read data.                                                                                                                                       
  Return:      	 None
  Globals:       None 
  Important:     The function sends the first byte & enable SPI bus. All other bytes will be sent 
  				 over Interrupt routine.                                     	
*******************************************************************************/

void SPI_bytesRecieve(unsigned char *Buf, int len)
{
    unsigned char i;
    DataPtr = Buf;
    dataLength = len;
 
    *pSPI_STAT = TXCOL | RBSY | MODF | TXE; // reset errors
   	//enable spi
	*pSPI0_CTL = (*pSPI_CTL | SPE );

    *pSPI_TDBR = *DataPtr++;
    i = *pSPI_RDBR;    // first data dummy read

}


/*****************************************************************************
  Function:		EX_INTERRUPT_HANDLER(ISR_SPI)
  Description:	Interrupt handler SPI
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

EX_INTERRUPT_HANDLER(ISR_SPI)
{
  unsigned short stat;

  ssync();
    
    dataLength--;
    if (dataLength>=1) {    
     
      while ((*pSPI_STAT&TXS) != 0 ) ;
      *pSPI_TDBR = *DataPtr;
    }
    
    if (dataLength>0) {  

      *(DataPtr-1) = *pSPI_RDBR;
      DataPtr++;
    } else {
       int i; 
       //Disable ALL Slaves
		*pSPI_FLG = 0xFF00;	
		// disable spi
       *pSPI_CTL &= ~SPE;                // stop SPI
       i = *pSPI_RDBR;               // clear dummy read buffer              
       flag_SPIsendfinisched=0;		// set SPI bus free
    }
 
     ssync(); 
}

