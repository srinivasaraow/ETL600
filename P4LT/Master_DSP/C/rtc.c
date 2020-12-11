/*-------------------------------------------------------------------------*
 * $Workfile: rtc.c $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/rtc.c $
 * 
 * 6     10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 5     28.06.05 17:25 Chchlee
 * Walkthrough
 * 
 * 4     28.06.05 13:53 Chchlee
 * ASSERTs consolidated
 * 
 * 3     24.02.05 18:22 Chmibuh
 * Even if(checkRTCforUndervoltage()  read msec & sec from RTC and use it
 * as random number
 * 
 * 2     24.02.05 11:43 Chmibuh
 * g_IncorrectRTCDateAlarm added
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 7     10.12.04 12:38 Chchlee
 * rtc_getIntTimeBCDPtr enabled
 * 
 * 6     20.10.04 16:31 Buhl01
 * Moved fcts from SPI_functions.c to rtc.c
 * 
 * 5     18.10.04 17:46 Buhl01
 * optimizer enabled
 * 
 * 4     21.09.04 18:21 Buhl01
 * InternalRTC() removed is now asm
 * 
 * 3     15.09.04 10:52 Buhl01
 * 
 * 2     14.09.04 16:27 Buhl01
 * 
 * 1     5.09.04 18:09 Buhl01
 * initial
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "P4LT_Types.h"
#include "SPI_functions.h"
#include "SPI_access.h"
#include "spi_dsp.h"

#include "rtc.h"
#include "rtc_M41ST95.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define RTC_MAGIC_BYTE_1			0xC3
#define RTC_MAGIC_BYTE_2			0x3C
#define RTC_MAGIC_BYTE_3			0x93
#define RTC_MAGIC_BYTE_4			0x39
#define RTC_MAGIC_WORD_COMPLETE		0xC33C9339

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* GLOBAL VARIABLES ***********************************************************/
t_internalTime internalTime; // used in assembler code


/* LOCAL VARIABLES ***********************************************************/
static t_internalTime internalTimeBCD;

static t_TimeDate  	  m_timeDate;

static BOOL g_IncorrectRTCDateAlarm;

static unsigned long rtc_magic_word_buffer[4] = {RTC_MAGIC_BYTE_1, RTC_MAGIC_BYTE_2, RTC_MAGIC_BYTE_3, RTC_MAGIC_BYTE_4};

/* LOCAL FUNCTIONS ***********************************************************/
static unsigned long convertBCDtoLong(unsigned long BCD);
static unsigned long convertLongToBCD(unsigned long DEC);
static void writeBCDtoString(char* stringPtr, char BCDvalue);
static BOOL checkRTCforUndervoltage(void);     // check for invalid time
static void startRTC(void);
static void SEGMENT_EXT_CODE getCompleteTimeDateFromRTC(t_internalTime *destination_TimePtr);

/* IMPLEMENTATION ************************************************************/

static unsigned long convertBCDtoLong(unsigned long BCD)
{
	return ((10*(BCD & 0xF0)>>4) + (BCD & 0x0F));
}

static unsigned long convertLongToBCD(unsigned long DEC)
{
	unsigned long result;
	result = (DEC/10) << 4;
	DEC%=10;
	result |= DEC;
	return (result);
}

 
static void writeBCDtoString(char* stringPtr, char BCDvalue)
{                                                     
	*stringPtr++ = ((BCDvalue & 0xf0) >>4) + 0x30;
	*stringPtr++ = (BCDvalue & 0x0f) + 0x30;
}                                       


#pragma optimize_off 

static void SEGMENT_EXT_CODE startRTC(void)
{
	transferRTC_Byte(WRITE_RTC_REG_AlarmHour, 0x00);	// start the RTC - by setting the HT bit to 0, the RTC will 
														// update its clock registers with the current time.
}


static BOOL SEGMENT_EXT_CODE checkRTCforUndervoltage(void)     // check for invalid time
{
	unsigned long readValue;
		
	readValue = 0;

	readValue |= ((transferRTC_Byte(READ_RTC_REG_USER_RAM_0, 0x00)) << 24);
	readValue |= ((transferRTC_Byte(READ_RTC_REG_USER_RAM_1, 0x00)) << 16);
	readValue |= ((transferRTC_Byte(READ_RTC_REG_USER_RAM_2, 0x00)) << 8);
	readValue |= transferRTC_Byte(READ_RTC_REG_USER_RAM_3, 0x00);

	
	if(readValue == RTC_MAGIC_WORD_COMPLETE)
	{
		transferRTC_Byte(WRITE_RTC_REG_USER_RAM_0, RTC_MAGIC_BYTE_1);  // write Magic words again to refresh sram in RTC
		transferRTC_Byte(WRITE_RTC_REG_USER_RAM_1, RTC_MAGIC_BYTE_2);
		transferRTC_Byte(WRITE_RTC_REG_USER_RAM_2, RTC_MAGIC_BYTE_3);
		transferRTC_Byte(WRITE_RTC_REG_USER_RAM_3, RTC_MAGIC_BYTE_4);
		return(FALSE);	// No Undervoltage - Time in RTC maybe oK
	}
	else
	{
		asm("nop;");
		return(TRUE);	// either this is a new P4LT or undervoltage condition occurred
	}
}


static void SEGMENT_EXT_CODE getCompleteTimeDateFromRTC(t_internalTime *destination_TimePtr)
{
	unsigned long DMA_is_running;
	t_internalTime *local_internalTimePtr;
	unsigned long *spiRecvPtr;

	local_internalTimePtr = destination_TimePtr;

	spiRecvPtr = spiFunctions_getSpiRecvBufferPtr();	// get Pointer on spiRecvBuffer
	
	readRTCmultipleByteDMA(READ_RTC_REG_Sec0, RTC_NoOF_BYTES_READ_TIME_DATE);

	do
	{
		asm("nop;");
		DMA_is_running = global_dmaCount;
		asm("nop;");
	}while(DMA_is_running);
	
	// get Time as BCD	 write all 8 Bits, incl. ST bit, CEB, CB, etc. see RTC M41ST95 register definition	
	spiRecvPtr++;		// ignore first byte in spiRecvBuffer, it's the received byte while sending RTC Command
	local_internalTimePtr->milisecond	= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[1]); 	
	local_internalTimePtr->second		= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[2]); 		
	local_internalTimePtr->minute		= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[3]);	
	local_internalTimePtr->hour			= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[4]);
	local_internalTimePtr->day_of_week	= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[5]);
	local_internalTimePtr->date			= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[6]);       
	local_internalTimePtr->month		= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[7]);  
	local_internalTimePtr->year			= (*(spiRecvPtr++) & 0xFF);	// (spiRecvBuffer[8]); 
}

void SEGMENT_EXT_CODE initInternalTime(void)
{
	unsigned long temp;
	// if we need the time of the last power off, than read RTC before the following RTC access!
	startRTC(); 
	
	g_IncorrectRTCDateAlarm = FALSE;
	
	if(checkRTCforUndervoltage())	// undervoltage condition detected - set default time & date 11:55:xx 09.12.2001
	{
		// Nevertheless read msec and sec from RTC - use it as random input for iButH_createChallenge -> iButton appl.
		temp = transferRTC_Byte(READ_RTC_REG_Sec0, 0x00);
		internalTime.milisecond = (convertBCDtoLong(temp))*10;
		temp = transferRTC_Byte(READ_RTC_REG_Sec1, 0x00);
		internalTime.second		= convertBCDtoLong(temp);

		internalTime.minute		= 55;
		internalTime.hour		= 11;
		internalTime.day_of_week= 1;
		internalTime.date		= 9;  
		internalTime.month		= 12; 
		internalTime.year		= 2001;  
		internalTime.togglebits = 0;
		
		g_IncorrectRTCDateAlarm = TRUE;  
	}
	else
	{
		getCompleteTimeDateFromRTC(&internalTimeBCD);  // get as BCD

		internalTime.milisecond = (convertBCDtoLong(internalTimeBCD.milisecond))*10;	// Byte   		
		internalTime.second		= convertBCDtoLong(internalTimeBCD.second & MASK_7LSB);	// MSB = ST bit 
		internalTime.minute		= convertBCDtoLong(internalTimeBCD.minute & MASK_7LSB);	// lowest 7 bits only
		internalTime.hour		= convertBCDtoLong(internalTimeBCD.hour & MASK_6LSB);	// lowest 6 bits only 
		internalTime.day_of_week= internalTimeBCD.day_of_week;							// ignored 
		internalTime.date		= convertBCDtoLong(internalTimeBCD.date & MASK_6LSB);	// lowest 6 bits only  
		internalTime.month		= convertBCDtoLong(internalTimeBCD.month & MASK_5LSB);	// lowest 5 bits only  
		internalTime.year		= (convertBCDtoLong(internalTimeBCD.year)) + 2000;		// Byte   
		internalTime.togglebits = 0;
	}

	m_timeDate.RTCmode		= 0;
	m_timeDate.timeValue	= 0;
}


#pragma optimize_as_cmd_line 

// We have an internal RTC counting in decimal format on the Master DSP. And we have an external RTC chip counting in BCD.
// The DSP clock is much more precise than the ext. RTC.
// The ext. RTC chip gets an update periodically (about every minute) but we translate the decimal format to BCD continuously 
// to get constant computing time. 
// If the HMI wants to read the time, first it sends the message "DOWNLOAD_RTC_DATA" with RTCmode RTC_READ_TIME_AND_DATE and 
// no valid time information. This is a heritage of P4LQ. We use this as trigger to convert the decimal time to 
// HMI format (timeStringPacked) on demand. With a second message "UPLOAD_RTC_DATA" the HMI gets the requested time.
// If the HMI wants to set the time it sends the the message "DOWNLOAD_RTC_DATA" with RTCmode RTC_WRITE_TIME_AND_DATE and 
// the valid time information. With this time information we adapt the internal decimal time and force an update of the ext. RTC.
// We have to set the RTC Magic Words additionally. They are used during power up to detect a RTC undervoltage condition = no valid
// time in ext. RTC.

void convert_time_formats(void)
{
	char asciiBuffer[2];
	t_internalTime      *local_internalTimeBCDPtr;
	t_internalTime      *local_internalTimePtr;
	t_TimeDate       	*local_timeDatePtr;

	local_internalTimePtr    = &internalTime;
	local_internalTimeBCDPtr = &internalTimeBCD;
	local_timeDatePtr		 = &m_timeDate;	
	
	// use the MASKs (e.g. MASK_6LSB) in order to not to change the RTC bits like ST, CEB, CB, etc. 
	local_internalTimeBCDPtr->milisecond	= 0;			// clear milisec
	local_internalTimeBCDPtr->second		&= ~(MASK_7LSB);		
	local_internalTimeBCDPtr->second		|= (convertLongToBCD(local_internalTimePtr->second) & MASK_7LSB);		
	local_internalTimeBCDPtr->minute		&= ~(MASK_7LSB);
	local_internalTimeBCDPtr->minute		|= (convertLongToBCD(local_internalTimePtr->minute) & MASK_7LSB);	
	local_internalTimeBCDPtr->hour			&= ~(MASK_6LSB); 
	local_internalTimeBCDPtr->hour			|= (convertLongToBCD(local_internalTimePtr->hour) & MASK_6LSB); 
	local_internalTimeBCDPtr->day_of_week	|= local_internalTimePtr->day_of_week;  // unchanged
	local_internalTimeBCDPtr->date			&= ~(MASK_6LSB); 
	local_internalTimeBCDPtr->date			|= (convertLongToBCD(local_internalTimePtr->date) & MASK_6LSB); 
	local_internalTimeBCDPtr->month			&= ~(MASK_5LSB); 
	local_internalTimeBCDPtr->month			|= (convertLongToBCD(local_internalTimePtr->month) & MASK_5LSB); 
	local_internalTimeBCDPtr->year			&= ~(MASK_8LSB); 
	local_internalTimeBCDPtr->year			|= (convertLongToBCD((local_internalTimePtr->year) - 2000) & MASK_8LSB); 


	if(local_timeDatePtr->RTCmode == RTC_READ_TIME_AND_DATE) // HMI needs time & date - convert it 
	{	
		local_timeDatePtr->timeStringPacked[0] = 0;                                        
		local_timeDatePtr->timeStringPacked[1] = 0;       
                                     
		writeBCDtoString(asciiBuffer, local_internalTimeBCDPtr->milisecond);
		local_timeDatePtr->timeStringPacked[0] += asciiBuffer[1] + 256*asciiBuffer[0];

		writeBCDtoString(asciiBuffer, (local_internalTimeBCDPtr->second & MASK_7LSB));
		local_timeDatePtr->timeStringPacked[0] += ((65536*asciiBuffer[1]) + (16777216*asciiBuffer[0]));

		writeBCDtoString(asciiBuffer, (local_internalTimeBCDPtr->minute & MASK_7LSB));
		local_timeDatePtr->timeStringPacked[1] += ((asciiBuffer[1]) + (256*asciiBuffer[0]));

		writeBCDtoString(asciiBuffer, (local_internalTimeBCDPtr->hour & MASK_6LSB));
		local_timeDatePtr->timeStringPacked[1] += ((65536*asciiBuffer[1]) + (16777216*asciiBuffer[0]));


		local_timeDatePtr->dateStringPacked[0] = 0;                                        
		local_timeDatePtr->dateStringPacked[1] = 0;                                        
		
		writeBCDtoString(asciiBuffer, (local_internalTimeBCDPtr->date & MASK_6LSB));
		local_timeDatePtr->dateStringPacked[0] = (asciiBuffer[1] + (256*asciiBuffer[0]));

		writeBCDtoString(asciiBuffer, (local_internalTimeBCDPtr->month  & MASK_5LSB));
		local_timeDatePtr->dateStringPacked[0] += ((65536*asciiBuffer[1]) + (16777216*asciiBuffer[0]));

		writeBCDtoString(asciiBuffer, local_internalTimeBCDPtr->year);
		local_timeDatePtr->dateStringPacked[1] += (asciiBuffer[1] + (256*asciiBuffer[0]));
		
		local_timeDatePtr->RTCmode = 0;
	}
	else if(local_timeDatePtr->RTCmode == RTC_WRITE_TIME_AND_DATE) // HMI sent time & date - adapt internal time
	{
		local_internalTimePtr->milisecond	= 0;			// clear milisec
		local_internalTimePtr->second		= ((((local_timeDatePtr->timeStringPacked[0] & 0xFF000000) >> 24) - 48) * 10) + (((local_timeDatePtr->timeStringPacked[0] & 0x00FF0000) >> 16) - 48);		
		local_internalTimePtr->minute		= ((((local_timeDatePtr->timeStringPacked[1] & 0x0000FF00) >> 8) - 48) * 10) + ((local_timeDatePtr->timeStringPacked[1] & 0x000000FF) - 48);		 
		local_internalTimePtr->hour			= ((((local_timeDatePtr->timeStringPacked[1] & 0xFF000000) >> 24) - 48) * 10) + (((local_timeDatePtr->timeStringPacked[1] & 0x00FF0000) >> 16) - 48);		 
        //local_internalTimePtr->day_of_week	= 1;			// is not used
		local_internalTimePtr->date			= ((((local_timeDatePtr->dateStringPacked[0] & 0x0000FF00) >> 8) - 48) * 10) + ((local_timeDatePtr->dateStringPacked[0] & 0x000000FF) - 48);		 
		local_internalTimePtr->month		= ((((local_timeDatePtr->dateStringPacked[0] & 0xFF000000) >> 24) - 48) * 10) + (((local_timeDatePtr->dateStringPacked[0] & 0x00FF0000) >> 16) - 48);		
		local_internalTimePtr->year			= ((((local_timeDatePtr->dateStringPacked[1] & 0x0000FF00) >> 8) - 48) * 10) + ((local_timeDatePtr->dateStringPacked[1] & 0x000000FF) - 48) + 2000;		 
		
		local_internalTimePtr->togglebits   ^= TOGGLE_MINUTE;       // force an RTC update
		local_internalTimePtr->togglebits	|= (SET_RTC_MAGIC_WORD);		

		local_timeDatePtr->RTCmode = 0;

		g_IncorrectRTCDateAlarm = FALSE;	// Clear Alarm
	}
}

#pragma optimize_off 



void setTimeDateOnRTC(void)
{
	writeRTCmultipleByteDMA(WRITE_RTC_REG_Sec0, RTC_NoOF_BYTES_WRITE_TIME_DATE, (unsigned long*)&internalTimeBCD);
}


void setMagicWordsOnRTC(void)
{
	writeRTCmultipleByteDMA(WRITE_RTC_REG_USER_RAM_0, RTC_NoOF_BYTES_SET_MAGIC_WORDS, (unsigned long*)rtc_magic_word_buffer);
}


t_internalTime SEGMENT_EXT_CODE * rtc_getIntTimePtr(void)
{
	return &internalTime;
}

t_internalTime SEGMENT_EXT_CODE * rtc_getIntTimeBCDPtr(void)
{
	return &internalTimeBCD;
}

t_TimeDate SEGMENT_EXT_CODE * rtc_getTimeDatePtr(void)
{
	return &m_timeDate;
}

BOOL SEGMENT_EXT_CODE rtc_incorrectRTCDateAlarm(void)
{
	return g_IncorrectRTCDateAlarm;
}
