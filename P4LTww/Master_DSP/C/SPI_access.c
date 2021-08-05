/*-------------------------------------------------------------------------*
 * $Workfile: SPI_access.c $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/Master_DSP/C/SPI_access.c $
 * 
 * 6     17.03.06 10:51 Chmibuh
 * new function reestablishSPI_DMA
 * 
 * 4     20.10.05 17:24 Chmibuh
 * cleaned up
 * 
 * 3     17.05.05 18:10 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 2     15.02.05 12:51 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem)
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 9     27.10.04 17:54 Buhl01
 * moved _spiSendBuffer to segment seg_dma; DMA !
 * 
 * 8     20.10.04 16:31 Buhl01
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
 * 2     21.07.04 19:10 Buhl01
 * 
 * 1     21.07.04 13:16 Buhl01
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "spi_dsp.h"
#include "SPI_access.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

spiCtr_struct spiCtr;      
unsigned long SEGMENT_DMA spiSendBuffer[32];		// must be in internal mem, because it's used for DMA transfers


/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/


	// for Prototype 2 HW use #define P4LW_PTTY2 - same as in spi_dsp.h, SPI_access.h & IrigB.c

#ifdef P4LW_PTTY2


//***************************************************************************************
//	void reestablishSPI(void)
//
//	workaround for DSP anomaly 47.
//  If Master DSP was SPI master it drives MOSI, MISO & CLK pins active even if the transfer was finished.
//	Result: no other SPI master can access the spi bus properly.
//	After reestablishSPI() all SPI pins are open drain (OPD).
//****************************************************************************************
void reestablishSPI(void)
{
	unsigned long spiSET;

	spiSET = MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT | OPD;   // the settings are not important, except OPD - it's a must!!!
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long *)SPITX = 0xFFFF;
	*(long *)SPICTL = spiSET;					// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))		// wait
	{
		asm("nop;");
	}
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
}



void sendSPIbyte(unsigned long dataByte, unsigned long whichSlave, unsigned long spi_settings)
{
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long*)(IOFLAG) &= whichSlave;				// select Slave (uP) P4LT

	*(long *)SPITX = dataByte;
	*(long *)SPICTL = spi_settings;				// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	*(long*)(IOFLAG) |= DESELECT_SPI_SLAVE;		// deselect Slave (uP) P4LT		
}

unsigned long readSPIword(unsigned long whichSlave, unsigned long spi_settings)
{
	unsigned long returnDataWord;

	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long*)(IOFLAG) &= whichSlave;				// select Slave (uP) P4LT
	*(long *)SPITX = 0xFF001155;				// load dummy value in transmit reg 0xFF001155 is used as MagicWord for UC DSP
	*(long *)SPICTL = spi_settings;				// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	returnDataWord =  *(long *)SPIRX;			// get data from recveive reg
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	*(long*)(IOFLAG) |= DESELECT_SPI_SLAVE;		// deselect Slave (uP) P4LT		
	return(returnDataWord);
}


void transmitSPI_DMA(void)
{
	spiCtr_struct *local_spiCtrPtr;

	local_spiCtrPtr = &spiCtr;
	
	*(long *)SPICTL = 0x00000000;	   					// clear SPI Reg

	*(long *)IISTX = (unsigned long) spiSendBuffer;		// it's always the same buffer
	*(long *)IMSTX = 1;
	*(long *)CSTX  = local_spiCtrPtr->dmaCount;
	
	global_dmaCount = local_spiCtrPtr->dmaCount;        
	spiRecvindex = 0;	

	asm("bit set LIRPTL SPIRMSK;");
	asm("bit set IMASK LPISUMI;");
	*(long*)(IOFLAG) &= local_spiCtrPtr->whichSlave;
	asm("nop;");
	*(long *)SPICTL = local_spiCtrPtr->spi_settings;	//start transfer
	asm("nop;");
}


void SEGMENT_EXT_CODE reestablishSPI_DMA(void)
{
	unsigned long temp;
	spiCtr_struct *local_spiCtrPtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  MS | WL8 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | OPD | SPRINT | TDMAEN;   // the settings are not important, except OPD - it's a must!!! 
	local_spiCtrPtr->whichSlave = 0;                // Select no slave
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = 1;					// Send one dummy byte

	*local_spiCtrPtr->dataPtr++ = 0xA1;             // dummy send data 

	transmitSPI_DMA();               // when dma finished received data are stored in spiRecvBuffer[] 
}

//bidirectional function: transfer single byte with DMA
//distinguish between data to/from UART (uC_UART_MODE) and task data for Microcontroller (uC_TASK_MODE) e.g. get voltage status
//write to Microcontroller:  put data to write 
//read from Microcontroller: put dummy data and read data from spiRecvBuffer[]
void transferByteMIC_DMA(BOOL mode, unsigned long dataByte)
{
	unsigned long temp;
	spiCtr_struct *local_spiCtrPtr;

	if(mode)
	{
		set_uC_UartMode();
	}
	else
	{
		set_uC_TaskMode();
	}

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_UART_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_MIC;
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = DMA_LENGTH_MIC_UART;   

	*local_spiCtrPtr->dataPtr++ = dataByte; // send data 

	transmitSPI_DMA();               // when dma finished received data are stored in spiRecvBuffer[]              
}

void readRTCmultipleByteDMA(unsigned long startByteAddress, unsigned long how_manyBytes)
{
	spiCtr_struct *local_spiCtrPtr;
	local_spiCtrPtr = &spiCtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_RTC_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_RTC;						
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = (how_manyBytes + 1); // one transfer for the AddressByte   

	*local_spiCtrPtr->dataPtr++ = startByteAddress; // send data 

	transmitSPI_DMA();               // when dma finished received data are stored in spiRecvBuffer[]  
}

void writeRTCmultipleByteDMA(unsigned long startByteAddress, unsigned long how_manyBytes, unsigned long *sendDataPtr)
{
	spiCtr_struct *local_spiCtrPtr;
	unsigned long *local_sendDataPtr, i;
	
	local_spiCtrPtr = &spiCtr;

	local_sendDataPtr = sendDataPtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_RTC_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_RTC;						
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = (how_manyBytes + 1); // one transfer for the AddressByte   

	*local_spiCtrPtr->dataPtr++ = startByteAddress; // send data 
	for(i=how_manyBytes;i;--i)
	{
		*local_spiCtrPtr->dataPtr++ = *local_sendDataPtr++;
	}

	transmitSPI_DMA();               
}


void writeDDSmultipleByteDMA(unsigned long *sendDataPtr, unsigned long how_manyBytes)
{
	spiCtr_struct *local_spiCtrPtr;
	unsigned long *local_sendDataPtr, i;
	
	local_spiCtrPtr = &spiCtr;

	local_sendDataPtr = sendDataPtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_DDS_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_DDS;						
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = how_manyBytes;

	for(i=how_manyBytes;i;--i)
	{
		*local_spiCtrPtr->dataPtr++ = *local_sendDataPtr++;
	}

	transmitSPI_DMA();               
}


//bidirectional function: non DMA
//write to RTC:  use a register WRITE_ADR, put data to write and ignore return value 
//read from RTC: use a register READ_ADR, put dummy data and use return value
unsigned long transferRTC_Byte(volatile unsigned long byteAddress, volatile unsigned long data)
{
	unsigned long returnDataByte;

	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long*)(IOFLAG) &= CS_SPI_RTC;				// select Slave (RTC) P4LT
	*(long *)SPITX = byteAddress;				// load Adr 
	*(long *)SPITX = data;						// load dummy value in transmit reg
	*(long *)SPICTL = SPI_SETTINGS_FOR_RTC;		// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	returnDataByte =  *(long *)SPIRX;			// get data from recveive reg - discard this byte

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	returnDataByte =  *(long *)SPIRX;			// get data from recveive reg
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	*(long*)(IOFLAG) |= DESELECT_SPI_SLAVE;		// deselect Slave (uP) P4LT	
	return(returnDataByte);
}




#else //#ifdef P4LW_PTTY2


//***************************************************************************************
//	void reestablishSPI(void)
//
//	workaround for DSP anomaly 47.
//  If Master DSP was SPI master it drives MOSI, MISO & CLK pins active even if the transfer was finished.
//	Result: no other SPI master can access the spi bus properly.
//	After reestablishSPI() all SPI pins are open drain (OPD).
//****************************************************************************************
void reestablishSPI(void)
{
	unsigned long spiSET;

	spiSET = MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT | OPD;   // the settings are not important, except OPD - it's a must!!!
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long *)SPITX = 0xFFFF;
	*(long *)SPICTL = spiSET;					// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))		// wait
	{
		asm("nop;");
	}
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
}



void sendSPIbyte(unsigned long dataByte, unsigned long whichSlave, unsigned long spi_settings)
{
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg

	*(long *)SPITX = dataByte;
	*(long *)SPICTL = (spi_settings | whichSlave);	// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
}

unsigned long readSPIword(unsigned long whichSlave, unsigned long spi_settings)
{
	unsigned long returnDataWord;

	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long *)SPITX = 0xFF001155;				// load dummy value in transmit reg 0xFF001155 is used as MagicWord for UC DSP
	*(long *)SPICTL = (spi_settings | whichSlave);				// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	returnDataWord =  *(long *)SPIRX;			// get data from recveive reg
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	return(returnDataWord);
}


void transmitSPI_DMA(void)
{
	spiCtr_struct *local_spiCtrPtr;

	local_spiCtrPtr = &spiCtr;
	
	*(long *)SPICTL = 0x00000000;	   					// clear SPI Reg

	*(long *)IISTX = (unsigned long) spiSendBuffer;		// it's always the same buffer
	*(long *)IMSTX = 1;
	*(long *)CSTX  = local_spiCtrPtr->dmaCount;
	
	global_dmaCount = local_spiCtrPtr->dmaCount;        
	spiRecvindex = 0;	

	asm("bit set LIRPTL SPIRMSK;");
	asm("bit set IMASK LPISUMI;");
	asm("nop;");
	*(long *)SPICTL = ((local_spiCtrPtr->spi_settings) | (local_spiCtrPtr->whichSlave));	//start transfer
	asm("nop;");
}


void reestablishSPI_DMA(void)
{
	unsigned long temp;
	spiCtr_struct *local_spiCtrPtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  MS | WL8 | BAUDR2 | SPIEN | NSMLS | SENDZ | GM | SPTINT | OPD | SPRINT | TDMAEN;   // the settings are not important, except OPD - it's a must!!! 
	local_spiCtrPtr->whichSlave = 0;                // Select no slave
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = 1;					// Send one dummy byte

	*local_spiCtrPtr->dataPtr++ = 0xA1;             // dummy send data 

	transmitSPI_DMA();               // when dma finished received data are stored in spiRecvBuffer[] 
}



//bidirectional function: transfer single byte with DMA
//distinguish between data to/from UART (uC_UART_MODE) and task data for Microcontroller (uC_TASK_MODE) e.g. get voltage status
//write to Microcontroller:  put data to write 
//read from Microcontroller: put dummy data and read data from spiRecvBuffer[]
void transferByteMIC_DMA(BOOL mode, unsigned long dataByte)
{
	unsigned long temp;
	spiCtr_struct *local_spiCtrPtr;

	if(mode)
	{
		set_uC_UartMode();
	}
	else
	{
		set_uC_TaskMode();
	}

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_UART_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_MIC;
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = DMA_LENGTH_MIC_UART;   

	*local_spiCtrPtr->dataPtr++ = dataByte; // send data 

	transmitSPI_DMA();               // when dma finished received data are stored in spiRecvBuffer[]              
}

void readRTCmultipleByteDMA(unsigned long startByteAddress, unsigned long how_manyBytes)
{
	spiCtr_struct *local_spiCtrPtr;
	local_spiCtrPtr = &spiCtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_RTC_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_RTC;						
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = (how_manyBytes + 1); // one transfer for the AddressByte   

	*local_spiCtrPtr->dataPtr++ = startByteAddress; // send data 

	transmitSPI_DMA();               // when dma finished received data are stored in spiRecvBuffer[]  
}

void writeRTCmultipleByteDMA(unsigned long startByteAddress, unsigned long how_manyBytes, unsigned long *sendDataPtr)
{
	spiCtr_struct *local_spiCtrPtr;
	unsigned long *local_sendDataPtr, i;
	
	local_spiCtrPtr = &spiCtr;

	local_sendDataPtr = sendDataPtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_RTC_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_RTC;						
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = (how_manyBytes + 1); // one transfer for the AddressByte   

	*local_spiCtrPtr->dataPtr++ = startByteAddress; // send data 
	for(i=how_manyBytes;i;--i)
	{
		*local_spiCtrPtr->dataPtr++ = *local_sendDataPtr++;
	}

	transmitSPI_DMA();               
}


void writeDDSmultipleByteDMA(unsigned long *sendDataPtr, unsigned long how_manyBytes)
{
	spiCtr_struct *local_spiCtrPtr;
	unsigned long *local_sendDataPtr, i;
	
	local_spiCtrPtr = &spiCtr;

	local_sendDataPtr = sendDataPtr;

	local_spiCtrPtr = &spiCtr;
	local_spiCtrPtr->spi_settings =  SPI_SETTINGS_FOR_DDS_VIA_DMA; 
	local_spiCtrPtr->whichSlave = CS_SPI_DDS;						
	local_spiCtrPtr->dataPtr = &spiSendBuffer[0];
	local_spiCtrPtr->dmaCount = how_manyBytes;

	for(i=how_manyBytes;i;--i)
	{
		*local_spiCtrPtr->dataPtr++ = *local_sendDataPtr++;
	}

	transmitSPI_DMA();               
}


//bidirectional function: non DMA
//write to RTC:  use a register WRITE_ADR, put data to write and ignore return value 
//read from RTC: use a register READ_ADR, put dummy data and use return value
unsigned long transferRTC_Byte(volatile unsigned long byteAddress, volatile unsigned long data)
{
	unsigned long returnDataByte;

	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long *)SPITX = byteAddress;				// load Adr 
	*(long *)SPITX = data;						// load dummy value in transmit reg
	*(long *)SPICTL = (SPI_SETTINGS_FOR_RTC | CS_SPI_RTC);		// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	returnDataByte =  *(long *)SPIRX;			// get data from recveive reg - discard this byte

	while(!((*(long *)SPISTAT & (RXS0))))
	{
		asm("nop;");
	}
	returnDataByte =  *(long *)SPIRX;			// get data from recveive reg
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
	return(returnDataByte);
}

#endif //#ifdef P4LW_PTTY2


