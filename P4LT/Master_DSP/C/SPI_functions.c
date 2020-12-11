/*-------------------------------------------------------------------------*
 * $Workfile: SPI_functions.c $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/Master_DSP/C/SPI_functions.c $
 * 
 * 13    4.04.06 18:30 Chmibuh
 * more reliable access of internal TDM bits
 * 
 * 12    17.03.06 10:51 Chmibuh
 * new state for SPI handling: SPI_MANAGER_GIVE_UP_BUSMASTERSHIP
 * 
 * 10    10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 9     16.07.05 17:45 Chmibuh
 * 
 * 8     30.06.05 9:39 Chchlee
 * cleaned up
 * 
 * 7     28.06.05 17:25 Chchlee
 * Walkthrough
 * 
 * 6     6.06.05 16:11 Chmibuh
 * 
 * 5     6.06.05 15:19 Chmibuh
 * getPiggyBackInfoFromNSK_DSP() added
 * 
 * 4     10.05.05 12:49 Chmibuh
 * 
 * 3     14.04.05 18:45 Chmibuh
 * 
 * 2     11.04.05 16:57 Chmibuh
 * Alarm monitoring fct. added
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 15    6.12.04 18:36 Chmibuh
 * Prepared for Emergency FW
 * 
 * 14    3.11.04 9:17 Buhl01
 * placeSPIRequest(SPI_MANAGER_GET_VOLTAGE_STATUS) added
 * 
 * 13    3.11.04 9:16 Buhl01
 * changed default UART mode to 8E1
 * 
 * 12    27.10.04 17:54 Buhl01
 * moved _spiRecvBuffer to segment seg_dma; DMA !
 * 
 * 11    27.10.04 14:38 Buhl01
 * uart_init added
 * 
 * 10    20.10.04 18:13 Buhl01
 * placeSPIRequest(SPI_MANAGER_GET_VOLTAGE_STATUS) commented out
 * 
 * 9     20.10.04 16:31 Buhl01
 * Moved fcts from SPI_functions.c to rtc.c
 * 
 * 8     18.10.04 19:47 Buhl01
 * placeSPIRequest(SPI_MANAGER_GET_VOLTAGE_STATUS);  commented out to
 * avoid microcontroller update on all P4LT !!!!
 * 
 * 7     18.10.04 18:01 Buhl01
 * 
 * 6     18.10.04 17:47 Buhl01
 * 
 * 5     21.09.04 18:22 Buhl01
 * 
 * 4     15.09.04 10:52 Buhl01
 * 
 * 3     14.09.04 18:32 Buhl01
 * 
 * 2     14.09.04 16:27 Buhl01
 * 
 * 1     1.09.04 14:34 Buhl01
 * initial
 *-------------------------------------------------------------------------*/

// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION
//
// If you add something, either if you write new code or comment-in previously commented-out 
// code fragments than bracket it with 
// #ifndef EMERGENCY_FW
//		"your code"
// #endif
//
// This prevents the emergency FW from needless code. The size of emergency FW is limited!!!
// This belongs to msgHandler.c, bridge.c, SPI_functions.c and Init_Master_DSP only.
//
// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION

#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "SPI_access.h"
#include "spi_dsp.h"
#include "rtc.h"
#include "dsp_micro_common.h"
#include "tdm_int_types.h"

extern unsigned long SEGMENT_DMA spiRecvBuffer[32];	// used figure instead of define, because definition in interrupt_routine uses figure too.


/*-----------------------GLOBAL CONSTANTS----------------------------------*/
/// State labels for SPI-Manager
#define SPI_MANAGER_GET_BYTE_UART				1
#define SPI_MANAGER_PUT_BYTE_UART				2
#define SPI_MANAGER_SET_UART_BAUDRATE			3
#define SPI_MANAGER_SET_TIME_ON_RTC				4
#define SPI_MANAGER_SET_RTC_MAGIC_WORD			5
#define SPI_MANAGER_GET_VOLTAGE_STATUS			6
#define SPI_MANAGER_SEND_DATA_DDS_GENERATOR		7
#define SPI_MANAGER_GIVE_UP_BUSMASTERSHIP		8

#define SPI_DUMMY_DATA							0x55

#define BITMASK_UNDERVOLT_P12V8					BIT4    
#define BITMASK_UNDERVOLT_P5V0					BIT2    
#define BITMASK_UNDERVOLT_N12V8					BIT0   
#define BITMASK_UNDERVOLT_P3V3					BIT8
#define BITMASK_UNDERVOLT_P1V8					BIT10    
#define BITMASK_OVERVOLT_P12V8					BIT5    
#define BITMASK_OVERVOLT_P5V0					BIT3
#define BITMASK_OVERVOLT_N12V8					BIT1    
#define BITMASK_OVERVOLT_P3V3					BIT9
#define BITMASK_OVERVOLT_P1V8					BIT11

/*-----------------------GLOBAL Static DATA DEFINITIONS---------------------------*/
///////// for spiManager ////////////////////////
static BOOL isSPIRequest_pending = FALSE;
static BOOL isSPIRequest_executing = FALSE;
static unsigned long SPI_OpCode = 0;
static unsigned long g_voltageStatus = 0;
/////////////////////////////////////////////////
static t_internalTime *internalTimePtr;
static unsigned long  *dataForDDS_Ptr;
static unsigned long  dataForDDS_length;

static unsigned long g_piggyBackInfo;


BOOL placeSPIRequest(unsigned long opCode) 
{
	unsigned long DMA_is_running;

	DMA_is_running = global_dmaCount;

	if(DMA_is_running)					// an other dma transfer is running
	{
		return (FALSE);
	}
	else
	{
		if (isSPIRequest_pending) 
		{
			// no success, already pending request
			return !isSPIRequest_pending;		
		} 
		else 
		{
			// success, insert request as pending 
			SPI_OpCode = opCode;										
			return isSPIRequest_pending = TRUE;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// U A R T functions //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////// for change UART settings - DSP sends following sequence
//  INIT_SET_UART_BAUDRATE					0x1F
//  CONFIRM_SET_UART_BAUDRATE				0x2F
//  Uart Settings							0xXX
//  INIT_SET_UART_BAUDRATE					0x1F
//  CONFIRM_SET_UART_BAUDRATE				0x2F
//  Uart Settings							0xXX
//  RETURN_RECV_UART_SETTINGS				0x3F  now the microcontroller will put the received settings in SPI transfer reg - read it with next access
//  dummy value								0x55  DSP reads the previously sent UART settings and compare them
//  The DSP will transfer this sequence until it reads the same UART settings as sent or max. 3 times (sendTrials = 3).

#define UART_ACCESS_DELAY				10000

BOOL SEGMENT_EXT_CODE uart_init(unsigned long baudrate)
{
	unsigned long local_baudrate, tempCount, sendtwice, sendTrials, recveivedBd;
	unsigned long send_buffer[3] = {INIT_SET_UART_BAUDRATE, CONFIRM_SET_UART_BAUDRATE, 0x00};

	local_baudrate = baudrate;

	send_buffer[2] = UART_MODE_8E1;			// change Mode here if it will be necessary see dsp_micro_common.h for valid defines

	switch(local_baudrate)					// see dsp_micro_common.h for additional valid defines
	{
		case CASE_2400_BD:
			send_buffer[2] |= UART_2400_BD;		// Test
			break;
		case CASE_9600_BD:
			send_buffer[2] |= UART_9600_BD;		// supported by HMI
			break;
		case CASE_19200_BD:
			send_buffer[2] |= UART_19200_BD;	// supported by HMI
			break;
		case CASE_57600_BD:
			send_buffer[2] |= UART_57600_BD;	// supported by HMI
			break;
		case CASE_115200_BD:
			send_buffer[2] |= UART_115200_BD;	// Test	
			break;
		default:
			send_buffer[2] |= UART_57600_BD;
			break;
	}

	set_uC_TaskMode();
	sendTrials = 3;
	do
	{
		for(tempCount = UART_ACCESS_DELAY; tempCount; tempCount--)
		{
			asm("nop;");
		}

		for(sendtwice = 2; sendtwice; sendtwice--)
		{
			sendSPIbyte(send_buffer[0], CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
			for(tempCount = UART_ACCESS_DELAY; tempCount; tempCount--)
			{
				asm("nop;");
			}
			sendSPIbyte(send_buffer[1], CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
			for(tempCount = UART_ACCESS_DELAY; tempCount; tempCount--)
			{
				asm("nop;");
			}
			sendSPIbyte(send_buffer[2], CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);
			for(tempCount = UART_ACCESS_DELAY; tempCount; tempCount--)
			{
				asm("nop;");
			}
		}
		sendSPIbyte(RETURN_RECV_UART_SETTINGS, CS_SPI_MIC, SPI_SETTINGS_FOR_MIC);	// send something else than send_buffer[2] to avoid to read own data
		for(tempCount = UART_ACCESS_DELAY; tempCount; tempCount--)
		{
			asm("nop;");
		}
	}while((((readSPIword(CS_SPI_MIC, SPI_SETTINGS_FOR_MIC)) & 0xFF) != send_buffer[2]) && (--sendTrials > 0));


	for(tempCount = UART_ACCESS_DELAY; tempCount; tempCount--)
	{
		asm("nop;");
	}
	set_uC_UartMode();
	
	if(0 == sendTrials) // did not work after 3 retries
	{
		return (FALSE);
	}	
	return (TRUE);
}

// This function musst be called twice to get one char. If chars are existing the frist call initializes the request 
// to get char from UART (Microcontroller) and the second call returns this char.
BOOL uart_getChar (unsigned char * rxChar)
{
	static BOOL getByteUARTisInitiated = 0;
	unsigned long DMA_is_running;

	DMA_is_running = global_dmaCount;

	if(DMA_is_running)					// an other dma transfer is running or our transfer is not ready
	{
		return (FALSE);
	}
	else if(getByteUARTisInitiated)		 
	{
		getByteUARTisInitiated = FALSE;
		*rxChar = spiRecvBuffer[0];
		return (TRUE);
	}
	else if (uartRequest)
	{
#ifndef EMERGENCY_FW	
		if(placeSPIRequest(SPI_MANAGER_GET_BYTE_UART))
#endif
		{		
			asm("bit clr IMASK IRQ0I;");	// disable new int
			uartRequest = 0;
			asm("bit set IMASK IRQ0I;");	// enable uart int
#ifdef EMERGENCY_FW	
			transferByteMIC_DMA(uC_UART_MODE, SPI_DUMMY_DATA);
#endif
			getByteUARTisInitiated = TRUE;
		}   // return FALSE in every case
	}
	return (FALSE);
}


BOOL uart_txQueueEmpty(void)			// simulate a filled TX Queue if an other DMA is running
{	
#ifndef EMERGENCY_FW	
	return (placeSPIRequest(SPI_MANAGER_PUT_BYTE_UART));
#endif

#ifdef EMERGENCY_FW						// if Emergency FW always return TRUE 
	return (TRUE);
#endif
}

//uart_txQueueEmpty() must be called before - this ensures access over SPI
void uart_putChar(unsigned char txChar)
{
	transferByteMIC_DMA(uC_UART_MODE, (unsigned char)txChar);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// DDS Functions /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL requestSendDataToDDS(unsigned long *sendDataPtr, unsigned long dataLength)
{	
	dataForDDS_Ptr = sendDataPtr;
	dataForDDS_length = dataLength;

	return (placeSPIRequest(SPI_MANAGER_SEND_DATA_DDS_GENERATOR));
}


void sendDataToDDSgenerator(void)
{
	writeDDSmultipleByteDMA(dataForDDS_Ptr, dataForDDS_length);  
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// S P I  M A N A G E R //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


// State labels for get Voltage Status from Microcontroller
// The pause states are necessary because a 93.75uSes cycle is to fast for the Microcontroller (the voltage measurements should not be interrupted)
#define FIRST_PAUSE_VOLTAGE_STATUS				1
#define SECOND_PAUSE_VOLTAGE_STATUS				2
#define INIT_READ_BASE_VOLTAGE_STATUS			3
#define READ_BASE_VOLTAGE_STATUS				4
#define INIT_READ_GENERATED_VOLTAGE_STATUS		5
#define READ_GENERATED_VOLTAGE_STATUS			6

#define VOLTAGE_READ_CYCLE						2000	// * 93.75 uSec
#define VOLTAGE_READ_CYCLE_SLOW_DOWN			10000	// * 93,75 uSec; if UART transfer -> reduce voltage read cycle 



void SEGMENT_EXT_CODE init_spiManager(void)
{
	isSPIRequest_pending = FALSE;
	isSPIRequest_executing = FALSE;
	SPI_OpCode = 0;

	internalTimePtr = rtc_getIntTimePtr();

	dataForDDS_Ptr = NULL;
	dataForDDS_length = 0;
}

unsigned long TestcountDSP = 0; 


BOOL SEGMENT_EXT_CODE spiManager(BOOL isSPIbus_requested)
{
	static unsigned long voltageStatusProgress = 0;
	static unsigned long voltageStatus = 0;
	static unsigned long voltageReadCounter = VOLTAGE_READ_CYCLE_SLOW_DOWN;
	static unsigned long togglebits = 0;
	static BOOL requestSPIbus_posEdge = FALSE;
	static BOOL isSPIbus_released = FALSE;
	unsigned long tempSPI_OpCode; 
	unsigned long DMA_is_running, temp;

	tempSPI_OpCode = SPI_OpCode;
	DMA_is_running = global_dmaCount;
	
	if (isSPIRequest_pending) 
	{
		if(!isSPIRequest_executing)		// start a new transfer
		{
			switch(tempSPI_OpCode)		// which transfer is desired 
			{
			case SPI_MANAGER_GET_BYTE_UART:
				transferByteMIC_DMA(uC_UART_MODE, SPI_DUMMY_DATA);// initialize DMA transfer
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				voltageReadCounter = VOLTAGE_READ_CYCLE_SLOW_DOWN;	
				break;
			case SPI_MANAGER_PUT_BYTE_UART:
				// special: direct access from ToDataLinkLayerRS232() to uart_putChar(); 
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				voltageReadCounter = VOLTAGE_READ_CYCLE_SLOW_DOWN;	
				break;
			case SPI_MANAGER_SET_TIME_ON_RTC:
				setTimeDateOnRTC();
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				break;
			case SPI_MANAGER_SET_RTC_MAGIC_WORD:
				setMagicWordsOnRTC();
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				break;
			case SPI_MANAGER_GET_VOLTAGE_STATUS:
				transferByteMIC_DMA(uC_TASK_MODE, SEND_DATA_INIT_READ_VOLTAGE);	// initialize DMA transfer 
				voltageStatusProgress = FIRST_PAUSE_VOLTAGE_STATUS;
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				break;
			case SPI_MANAGER_SEND_DATA_DDS_GENERATOR:
				sendDataToDDSgenerator();
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				break;
			case SPI_MANAGER_GIVE_UP_BUSMASTERSHIP:
				reestablishSPI_DMA();
				isSPIRequest_pending = isSPIRequest_executing = TRUE;
				voltageReadCounter = 2 * (VOLTAGE_READ_CYCLE_SLOW_DOWN);
				break;

			default:
				isSPIRequest_pending = isSPIRequest_executing = FALSE;
				break;
			} // end switch
		}			
		else							// transfer was initiated
		{
			if(!DMA_is_running)			// transfer is finished
			{				
				switch(tempSPI_OpCode) 
				{
				case SPI_MANAGER_GET_BYTE_UART:
					isSPIRequest_pending = isSPIRequest_executing = FALSE;
					break;

				case SPI_MANAGER_PUT_BYTE_UART:
					isSPIRequest_pending = isSPIRequest_executing = FALSE;
					break;

				case SPI_MANAGER_SET_TIME_ON_RTC:
					isSPIRequest_pending = isSPIRequest_executing = FALSE;
					break;
				case SPI_MANAGER_SET_RTC_MAGIC_WORD:		
					isSPIRequest_pending = isSPIRequest_executing = FALSE;
					break;
				case SPI_MANAGER_GET_VOLTAGE_STATUS:		//special case again for uP read, because of stupide SPI implementation (ATmega as Slave)
					switch(voltageStatusProgress)
					{
					case FIRST_PAUSE_VOLTAGE_STATUS:
						voltageStatusProgress++;
						isSPIRequest_pending = isSPIRequest_executing = TRUE;
						break;
					case SECOND_PAUSE_VOLTAGE_STATUS:					
						voltageStatusProgress++;						
						isSPIRequest_pending = isSPIRequest_executing = TRUE;
						break;
					case INIT_READ_BASE_VOLTAGE_STATUS:
						transferByteMIC_DMA(uC_TASK_MODE, SEND_DATA_READ_BASE_VOLTAGE);			// initialize DMA transfer  
						voltageStatusProgress++;
						isSPIRequest_pending = isSPIRequest_executing = TRUE;
						break;
					case READ_BASE_VOLTAGE_STATUS:    // and init read generated voltage
						temp = spiRecvBuffer[0];
						if(temp & 0x80)
						{
							voltageStatus = spiRecvBuffer[0]; 
						}
						voltageStatusProgress++;
						isSPIRequest_pending = isSPIRequest_executing = TRUE;
						break;
					case INIT_READ_GENERATED_VOLTAGE_STATUS:
						transferByteMIC_DMA(uC_TASK_MODE, SEND_DATA_READ_GENERATED_VOLTAGE);	// initialize DMA transfer 
						voltageStatusProgress++;
						isSPIRequest_pending = isSPIRequest_executing = TRUE;
						break;
					case READ_GENERATED_VOLTAGE_STATUS:
						temp = spiRecvBuffer[0];
						if(temp & 0x80)
						{
							voltageStatus |= (spiRecvBuffer[0] << 8);   
						}
						voltageStatusProgress = 0;
						isSPIRequest_pending = isSPIRequest_executing = FALSE;
						// copy voltageStatus to check Alarms 					
						g_voltageStatus = voltageStatus;
						break;
					default:
						voltageStatusProgress = 0;
						isSPIRequest_pending = isSPIRequest_executing = FALSE;
						break;
					} // end switch(voltageStatusProgress)
					
					break;
				
				case SPI_MANAGER_SEND_DATA_DDS_GENERATOR:
					isSPIRequest_pending = isSPIRequest_executing = FALSE;
					break;
				case SPI_MANAGER_GIVE_UP_BUSMASTERSHIP:	
					
					isSPIbus_released = TRUE;					// give up SPI Bus Mastership

					isSPIRequest_pending = isSPIRequest_executing = TRUE;   // SPI request is still pending and executing
					
					if(!(isSPIbus_requested))
					{	// UC DSP is finished
						isSPIRequest_pending = isSPIRequest_executing = FALSE;
						
						isSPIbus_released = FALSE;

						voltageReadCounter = VOLTAGE_READ_CYCLE;
						TestcountDSP++;
					}
					else if(--voltageReadCounter == 0) // Emergency exit after ~ 2 sec. Reuse of voltageReadCounter
					{
						isSPIRequest_pending = isSPIRequest_executing = FALSE;
						isSPIbus_released = FALSE;

						voltageReadCounter = VOLTAGE_READ_CYCLE;						
					}
					break;

				default:
					isSPIRequest_pending = isSPIRequest_executing = FALSE;
					break;
				} // end switch(tempSPI_OpCode)
				
			}
			//else do nothing - transfer is not ready
		}
	}
	else // no request is pending
	{			
			// detect positive edge of requestSPIbus
		if((isSPIbus_requested) && (!requestSPIbus_posEdge))
		{
			placeSPIRequest(SPI_MANAGER_GIVE_UP_BUSMASTERSHIP); // ignore return val, because here is isSPIRequest_pending = FALSE
		}				
		else if(((internalTimePtr->togglebits) & TOGGLE_MINUTE) != (togglebits & TOGGLE_MINUTE))	// Minute has toggled - write time to RTC
		{	//Test togglebits for TOGGLE_MINUTE 
			placeSPIRequest(SPI_MANAGER_SET_TIME_ON_RTC); // ignore return val, because here is isSPIRequest_pending = FALSE
		}
		else if((internalTimePtr->togglebits) & SET_RTC_MAGIC_WORD)
		{	//test togglebits for SET_RTC_MAGIC_WORD 
			placeSPIRequest(SPI_MANAGER_SET_RTC_MAGIC_WORD); // ignore return val, because here is isSPIRequest_pending = FALSE
			internalTimePtr->togglebits &= ~(SET_RTC_MAGIC_WORD);
		}
		else //decrement voltageReadCounter
		{
			if(--voltageReadCounter == 0)
			{
				placeSPIRequest(SPI_MANAGER_GET_VOLTAGE_STATUS);// ignore return val, because here is isSPIRequest_pending = FALSE
				voltageReadCounter = VOLTAGE_READ_CYCLE;	// 
			}
		}
		togglebits = internalTimePtr->togglebits;

		requestSPIbus_posEdge = isSPIbus_requested;
	}
	return(isSPIbus_released);
}




unsigned long SEGMENT_EXT_CODE * spiFunctions_getSpiRecvBufferPtr(void)
{
	return &spiRecvBuffer[0];
}

void SEGMENT_EXT_CODE getPiggyBackInfoFromNSK_DSP(void)
{
	unsigned long tempWait;

	for(tempWait = 120;tempWait;tempWait--)
	{
		asm("nop;");
		asm("nop;");
	}
	
	g_piggyBackInfo = readSPIword(CS_SPI_DSP_NSK, SPI_SETTINGS_FOR_DSP);
}


unsigned long SEGMENT_EXT_CODE getPiggyBackInfo(void)
{
	return(g_piggyBackInfo);
}

BOOL SEGMENT_EXT_CODE spiFuncM12V_UndervAlarm(void)
{
	return (g_voltageStatus & BITMASK_UNDERVOLT_N12V8);
}
BOOL SEGMENT_EXT_CODE spiFuncM12V_OvervAlarm(void)
{
	return (g_voltageStatus & BITMASK_OVERVOLT_N12V8);
}
BOOL SEGMENT_EXT_CODE spiFuncP12V_UndervAlarm(void)
{
	return (g_voltageStatus & BITMASK_UNDERVOLT_P12V8);
}
BOOL SEGMENT_EXT_CODE spiFuncP12V_OvervAlarm(void)
{
	return (g_voltageStatus & BITMASK_OVERVOLT_P12V8);
}
BOOL SEGMENT_EXT_CODE spiFuncP5V_UndervAlarm(void)
{
	return (g_voltageStatus & BITMASK_UNDERVOLT_P5V0);
}
BOOL SEGMENT_EXT_CODE spiFuncP5V_OvervAlarm(void)
{
	return (g_voltageStatus & BITMASK_OVERVOLT_P5V0);
}
BOOL SEGMENT_EXT_CODE spiFuncP3_3V_UndervAlarm(void)
{
	return (g_voltageStatus & BITMASK_UNDERVOLT_P3V3);
}
BOOL SEGMENT_EXT_CODE spiFuncP3_3V_OvervAlarm(void)
{
	return (g_voltageStatus & BITMASK_OVERVOLT_P3V3);
}
BOOL SEGMENT_EXT_CODE spiFuncP1_8V_UndervAlarm(void)
{
	return (g_voltageStatus & BITMASK_UNDERVOLT_P1V8);
}
BOOL SEGMENT_EXT_CODE spiFuncP1_8V_OvervAlarm(void)
{
	return (g_voltageStatus & BITMASK_OVERVOLT_P1V8);
}



























