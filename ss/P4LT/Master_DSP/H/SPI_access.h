/*-------------------------------------------------------------------------*
 * $Workfile: SPI_access.h $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/Master_DSP/H/SPI_access.h $
 * 
 * 5     17.03.06 10:51 Chmibuh
 * reestablishSPI_DMA
 * 
 * 3     17.05.05 18:11 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 2     15.02.05 12:08 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem) 
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 7     18.10.04 17:47 Buhl01
 * 
 * 6     15.09.04 10:52 Buhl01
 * 
 * 5     14.09.04 16:27 Buhl01
 * 
 * 4     18.08.04 11:20 Buhl01
 * 
 * 3     4.08.04 17:21 Buhl01
 * 
 * 2     22.07.04 10:39 Buhl01
 * 
 * 1     21.07.04 13:21 Buhl01
 *-------------------------------------------------------------------------*/

#ifndef SPI_ACCESS_H
#define SPI_ACCESS_H

#include "basetype.h"


#define DMA_LENGTH_MIC_UART					1


	// for Prototype 2 HW use #define P4LW_PTTY2 - same as in spi_dsp.h & IrigB.c

#ifdef P4LW_PTTY2
	#define set_uC_UartMode()						asm("bit set FLAGS  FLG2;")
	#define set_uC_TaskMode()						asm("bit clr FLAGS  FLG2;") 
#else
	#define set_uC_UartMode()						*(long*)(IOFLAG) |= FLG4
	#define set_uC_TaskMode()						*(long*)(IOFLAG) &= ~(FLG4) 
#endif

	#define uC_UART_MODE							TRUE
	#define uC_TASK_MODE							FALSE


///// global variables to communicate with interrupt routines ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
extern volatile long global_dmaCount;
extern volatile long spiRecvindex;
extern volatile long uartRequest;


typedef struct{ 
unsigned long spi_settings;
unsigned long whichSlave;  
unsigned long *dataPtr; 			
unsigned long dmaCount;                                     
}spiCtr_struct;



void reestablishSPI(void);
void sendSPIbyte(unsigned long dataByte, unsigned long whichSlave, unsigned long spi_settings);

void transmitSPI_DMA(void);
void transferByteMIC_DMA(BOOL mode, unsigned long dataByte);

unsigned long readSPIword(unsigned long whichSlave, unsigned long spi_settings);

unsigned long transferRTC_Byte(volatile unsigned long byteAddress, volatile unsigned long data);
void readRTCmultipleByteDMA(unsigned long startByteAddress, unsigned long how_manyBytes);
void writeRTCmultipleByteDMA(unsigned long startByteAddress, unsigned long how_manyBytes, unsigned long *sendDataPtr);

void writeDDSmultipleByteDMA(unsigned long *sendDataPtr, unsigned long how_manyBytes);

void reestablishSPI_DMA(void);



#endif  // SPI_ACCESS_H
