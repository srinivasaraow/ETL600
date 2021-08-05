/*-------------------------------------------------------------------------*
 * $Workfile: IrigB.c $
 * Part of	: ETl600 
 * Language	: C
 * Created by	: M. Buhl
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/IrigB.c $
 * 
 * 10    10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 9     28.06.05 17:25 Chchlee
 * Walkthrough
 * 
 * 8     17.05.05 18:10 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 7     30.03.05 12:03 Chmibuh
 * irigB_checkRTC_Date() is now called by syncRTC() instead of main() ->
 * easier error handling
 * 
 * 6     17.02.05 16:41 Chchlee
 * Monitoring concept changed
 * 
 * 5     15.02.05 12:07 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem)  & code cosmetic
 * 
 * 4     4.02.05 14:47 Chchlee
 * SW quality
 * 
 * 3     3.02.05 20:18 Chmibuh
 * Comment added
 * 
 * 2     2.02.05 17:33 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem)
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 6     30.12.04 15:13 Chchlee
 * adjusted for new field "systemSettings" in system configuration
 * 
 * 5     30.12.04 11:08 Chchlee
 * 
 * 4     30.12.04 10:14 Chchlee
 * prepared for monitoring
 * 
 * 3     3.11.04 9:06 Buhl01
 * syncRTC fractional_ms = 22;
 * 
 * 2     27.10.04 14:38 Buhl01
 * 
 * 1     15.10.04 16:11 Buhl01
 * initial
 *-------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
// This is historical code. Nearly identical objects are operating for ETL500 and NSD570.
// This is the second porting of the source files. The style is ugly 
// (e.g. different functions controls the statemachine for decoding Irig bits)
// but works. 
// Some cosmetically changes were wished -> the style remains ugly.  
//-----------------------------------------------------------------------------


/* INCLUDE FILES **************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "basetype.h"
#include "IrigB.h"
#include "P4LT_Types.h"
#include "rtc.h"
#include "diag.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define PIND				2	// position indicator 


#define WAIT_ONE_MAIN_CYCL	1			// number of 10.666 kHz cycles ETL600 93.75 us
#define WAIT1ms				10			// 0.9375 us
#define WAIT2_5ms			26			// 2.4375 us
#define WAIT3ms				32			// 3.0 ms
#define WAIT5_5ms			58			// 5.4375 ms
#define WAIT480ms			5120		// 480.0 ms
#define WAIT10s				53333		// 9.9999375 s
#define WAIT60s				640000		// 60.0 s
#define WAIT70s				746666		// 69.9999375 s

#define NOT_COUNTING		10

#define GPS_SECONDS			0
#define GPS_MINUTES			1
#define GPS_HOURS			2
#define GPS_DAY				3
#define GPS_MONTH			4


// Number of repeated Wrong RTC errors
#define THRESHOLD_SET_ALARM_WRONG_RTC_DATE	10

#define CHECK_RTC_DATE_INTERVAL				120  // SUBCASE_1 = 0.106 KHz = 120 * 9.375 ms = 1.125 sec

/* LOCAL TYPE DECLARATIONS ***************************************************/
static enum 
{
	DecodeBitState_Init,
	DecodeBitState_Sync,
	DecodeBitState_WaitForRisingEdge,
	DecodeBitState_Wait3ms,
	DecodeBitState_Wait6ms
}
e_DecodeBitState = DecodeBitState_Init;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/
static long g_TrackingCount = WAIT480ms;
static long g_TimeoutCount  = WAIT10s;
static long g_Irig_time[5];
static BOOL g_IrigB_IsEnabled;

static BOOL g_IncorrectRTCDateAlarm;
static BOOL g_SignalLostAlarm;

static t_internalTime * g_pInternal_IrigTime;

/* LOCAL FUNCTIONS ***********************************************************/
static void decodeBit(void);
static void decodeByte(long newBit);
static void decodeFrame(long byte, long bits);
static void syncRTC (void);


/* EXTERNAL VARIABLES ***********************************************************/
extern volatile long fractional_ms; // defined in Interrupt_routines_Master_DSP.asm - needed for synchronization of IRIG and intrnal time

//unsigned long testL_count = 0;
//SEGMENT_EXT_DATA unsigned long testarryL[20000];


/* IMPLEMENTATION ************************************************************/

//---------------void irigB_init(t_P4LT_Config *config)------------------------
// Init Irig-B decoder
// g_IrigB_IsEnabled = TRUE if enabled in config
// g_pInternal_IrigTime get Pointer to internal time for update
// g_pAlarmsAndWarnings get Pointer to Alarms&Warning struct to set or clear Irig-Alarms
//-----------------------------------------------------------------------------
void SEGMENT_EXT_CODE irigB_init(t_P4LT_Config *config)
{
	g_IrigB_IsEnabled = config->system.systemSettings & SYSTEM_SETTINGS_ENABLE_IRIG_B;

	g_pInternal_IrigTime = rtc_getIntTimePtr();
}
 

#define IRIG_FLAG_MASK					0x00000002  // Flag 5

//-------------------void decodeBit(void)--------------------------------------
// Statemachine for decoding Irig-Bits
// some States will be changed by other functions like decodeFrame() and 
// irigB_activate().
// If g_IrigB_IsEnabled = TRUE, decodeBit() is called by irigB_activate().
// decodeBit() calls syncRTC() and decodeByte()
//-----------------------------------------------------------------------------
static void SEGMENT_EXT_CODE decodeBit(void)
{
	static long irigBbit;

	// for Prototype 2 HW use #define P4LT_PROTOTYPE2HW - same as in spi_dsp.h & Init_Master_DSP.c
#ifdef P4LW_PTTY2																
	// Check Flag 0 (as input)
	asm("r0=0;");
	asm("IF FLAG0_IN r0=r0+1;");	
	asm("%0=r0;":"=d" (irigBbit));		 
#else										
	// Check Flag 5 (as input)
	irigBbit = ((*(long*)IOFLAG) & (IRIG_FLAG_MASK)); 
#endif

	switch (e_DecodeBitState) 
	{
		case DecodeBitState_Init:						// initial state, wait for IRIG-B low 	
			g_TrackingCount=WAIT1ms;
			if (irigBbit == 0) e_DecodeBitState=DecodeBitState_WaitForRisingEdge;
			break;

		case DecodeBitState_WaitForRisingEdge:			// IRIG-B is low, wait for rising edge 
			if (irigBbit)								// rising edge
			{
				e_DecodeBitState = DecodeBitState_Wait3ms;	
				g_TrackingCount = WAIT3ms;
			}
			else 
				g_TrackingCount=WAIT1ms;
			break;

		case DecodeBitState_Wait3ms:					// check IRIG-B 3ms after rising edge 
			if (irigBbit == 0)							// falling edge
			{											// if low, bit is 0
				e_DecodeBitState = DecodeBitState_Init;	// initial state 8.5ms after rising edge 
				g_TrackingCount = WAIT5_5ms;
				decodeByte(0) ;
			}
			else 
			{
				e_DecodeBitState = DecodeBitState_Wait6ms;
				g_TrackingCount=WAIT3ms;
			}
			break;

		case DecodeBitState_Wait6ms:					// check IRIG-B 6ms after rising edge 
			if (irigBbit == 0)							// falling edge
			{
				e_DecodeBitState = DecodeBitState_Init;	// if low, bit is 1 else bit is PIND (position indicator) 
				g_TrackingCount = WAIT2_5ms;				// initial state 8.5ms after rising edge 
				decodeByte(1);
			}
			else 
			{
				e_DecodeBitState = DecodeBitState_Init;
				g_TrackingCount = WAIT2_5ms;				// initial state 8.5ms after rising edge
				decodeByte(PIND);
			}
			break;

		case DecodeBitState_Sync:						// wait for rising edge with high accuracy 
			if (irigBbit)								// rising edge
			{
				syncRTC();								//  synchronize RTC
				e_DecodeBitState = DecodeBitState_Init;
				g_TrackingCount = WAIT480ms;				// set next GPS tracking time & timeout
				g_TimeoutCount = WAIT10s;
			}
			else 
				g_TrackingCount=WAIT_ONE_MAIN_CYCL;
			break;

		default:
			ASSERT(FALSE);
	}
}

//--------------------------void decodeByte(long newBit)-----------------------
// Decode Irig-B bytes
// Is called by decodeBit() and calls decodeFrame()
//-----------------------------------------------------------------------------
static void decodeByte(long newBit)
{	
	static long bitCount=NOT_COUNTING;
	static long byte=0;

	if (newBit != PIND) {								// shift new bit into byte (bit 8)
		byte = ((byte >> 1) & 0x000000FF) | (newBit * 0x00000100);
		if (bitCount != NOT_COUNTING) bitCount++;
	}
	else {
		decodeFrame(byte,bitCount);
		bitCount=0;
	}
}

//--------------------void decodeFrame(long byte, long bits)-------------------
// Decode Irig-B frames
// Is called by decodeByte()
// Sets state e_DecodeBitState=DecodeBitState_Sync of statemachine for decoding 
// Irig-Bits at decodeBits() 
//-----------------------------------------------------------------------------
static void decodeFrame(long byte, long bits)
{
	static long byteCount = NOT_COUNTING;
	
	if (bits == 0) byteCount = 0;						// reference marker => start frame 
	else if ((bits == 8) && (byteCount == 0)) {			// decode seconds, 8 bit BCD 
		g_Irig_time[GPS_SECONDS] = ((byte >> 1) & 0x0F) | ((byte >> 2) & 0x70); 
		byteCount++;
	}
	else if (bits == 9) {
		if (byteCount == 0) byteCount = NOT_COUNTING;   // seconds must be 8 bit! 
		else if ((byteCount == 1) | (byteCount == 2)) {	// decode min & hrs 9 bit BCD 
			g_Irig_time[byteCount] = (byte & 0x0F) | ((byte >> 1) & 0xF0);
			byteCount++;
		}
		else if (byteCount == 3) {						// decode days 9 bit BCD to int 
			g_Irig_time[GPS_DAY] = (byte & 0x0F) + 10*((byte >> 5) & 0x0F);
			byteCount++;
		}
		else if (byteCount == 4) {						// decode days 3 bit BCD to int  
			g_Irig_time[GPS_DAY] +=  100*(byte & 0x03);
			byteCount = NOT_COUNTING;
			e_DecodeBitState=DecodeBitState_Sync;		// synchronise RTC on next rising edge which 	
		}												// occurs at decoded time plus 500 ms (see IRIG-B spec) 
	}
	else byteCount = NOT_COUNTING;	// not a valid frame format
}

//-----------------------void irigB_activate(void)-----------------------------
// Waiting loop for Irig-B decoding e.g. g_TrackingCount = WAIT480ms. Used by  
// statemachine for decoding Irig-Bits at decodeBits().
// Sets alarm ALM_IRIG_signal_lost if g_TimeoutCount = 0 -> no valid Irig-B signal 
// was decoded and clears alarm ALM_Incorrect_RTC_date, because if we don't know 
// the Irig date we can't decide if date is correct or not. See irigB_checkRTC_Date().
// Is called by main() every 93.75 uSec
//-----------------------------------------------------------------------------
void irigB_activate(void)
{

	if (g_IrigB_IsEnabled)
	{
		if (--g_TimeoutCount == 0) {				// No IRIG signal detected during the time
			e_DecodeBitState=DecodeBitState_Init;	// specified by g_TimeoutCount.
			g_TrackingCount = WAIT60s;				// Look for IRIG signal after 1 min.
			g_TimeoutCount = WAIT70s;				// Stop looking if not found in 10 sec.

			g_SignalLostAlarm       = TRUE;
			g_IncorrectRTCDateAlarm = FALSE;
		}	
		
		if (--g_TrackingCount == 0) decodeBit();		// check GPS signal
	}
}


static long BCDtoLong(long BCDvalue)
{
	return (10*((BCDvalue & 0xf0) >> 4) + (BCDvalue & 0x0f));
} 



//-----------------------void syncRTC (void)-----------------------------------
// Synchronize the internal time if a valid Irig-B frame was decoded. Irig-B frame 
// do not include a complete date (only No. of days beginning with 01.Jan of each 
// year) therefore do not set the date.  
// Is called by statemachine for decoding Irig-Bits
// Clears Alarm ALM_IRIG_signal_lost
//-----------------------------------------------------------------------------
static void syncRTC (void)
{
/*	static unsigned long testCount = 189;

	if(--testCount == 0)
	{
		asm("nop;");
	}

	if(testL_count < 20000)
	{
		testarryL[testL_count++] = fractional_ms;
		testarryL[testL_count++] = g_pInternal_IrigTime->milisecond;
	}
*/

	fractional_ms = 22;						 // synchronize the internal time

	g_pInternal_IrigTime->milisecond	= 500;   // decoding of IRIG Time finished after start + 500 ms; see Irig Spec
	
	g_pInternal_IrigTime->second		= BCDtoLong(g_Irig_time[GPS_SECONDS]);	
	g_pInternal_IrigTime->minute		= BCDtoLong(g_Irig_time[GPS_MINUTES]);	
	g_pInternal_IrigTime->hour	  	    = BCDtoLong(g_Irig_time[GPS_HOURS]);	

	g_SignalLostAlarm = FALSE;

	irigB_checkRTC_Date();
}


//----------------------void irigB_checkRTC_Date(void)-------------------------
// Irig-B frame do not include a complete date (only No. of days beginning with 
// 01.Jan of each year). 
// The year must be set manually by HMI600.
// Reads the date from external RTC, calculates the No. of days of the actual year 
// and compares it with No. of days from decoded Irig-B frame.
// If not equal, set Alarm ALM_Incorrect_RTC_date after HoldCounter = 0;
// else clear alarm ALM_Incorrect_RTC_date.
// Is called by syncRTC every second. Checks the dates every 120 loops (dateCheckCount) 
// = 120 sec
//-----------------------------------------------------------------------------
void SEGMENT_EXT_CODE irigB_checkRTC_Date(void)
{
	static long dateCheckCount = (CHECK_RTC_DATE_INTERVAL / 10);
    static long *day;
                 // number of days at the end of each month 
	static long daysnoleap[13]={0,31,59,90,120,151,181,212,243,273,304,334};
    static long daysleap[13]={0,31,60,91,121,152,182,213,244,274,305,335};

	if (--dateCheckCount == 0) 
	{
		dateCheckCount = CHECK_RTC_DATE_INTERVAL; 
		
		if ((g_pInternal_IrigTime->year) & 0x03)	//check if leapyear
		{
			day = daysnoleap;	
		}
		else 
		{
			day = daysleap;
		}

		
		if (g_Irig_time[GPS_DAY] != (*(day + (g_pInternal_IrigTime->month) - 1) + (g_pInternal_IrigTime->date))) 
		{
			g_IncorrectRTCDateAlarm = TRUE;
		}
		else
		{
			g_IncorrectRTCDateAlarm = FALSE;
		}

	}
}


BOOL irigB_incorrectRTCDateAlarm(void)
{
	return g_IncorrectRTCDateAlarm;
}

BOOL irigB_signalLostAlarm(void)
{
	return g_SignalLostAlarm;
}

