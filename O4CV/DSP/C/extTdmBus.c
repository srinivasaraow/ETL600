/*-------------------------------------------------------------------------*
 * $Workfile	: extTdmBus.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : This module implements the external TDM bus services.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/extTdmBus.c $
 * 
 * 29    11-04-21 10:09 Chhamaa
 * O4CV_WARNING_MASK_LED applied to set O4CV warning bit on ext. TDM bus.
 * 
 * 27    22.09.08 14:33 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 26    08-06-16 13:49 Chrobif
 * Debugging of Problem 447 showed that voiceData_receive() is not robust
 * to corrupt data rate indices in basic time slot. This modification
 * improves this aspect.
 * 
 * 25    9.04.08 15:48 Chhamaa
 * Redundant tx toggle bit code removed.
 * 
 * 24    08-02-15 10:50 Chrobif
 * + extTdmBus_getReadDuSlicIntFlag() added.
 * 
 * 23    8.02.08 13:12 Chhamaa
 * External TDM bus monitoring modified.
 * 
 * 22    6.02.08 13:28 Chhamaa
 * Rx toggle bit monitoring modified.
 * 
 * 21    1.02.08 11:37 Chhamaa
 * + Defines for external TDM bus monitoring modified.
 * + Ext TDM bus alarm deacitvated (NO_TDM_BUS_ERROR_INTERVAL	set to 0)
 * 
 * 20    28.01.08 11:16 Chhamaa
 * NO_TDM_BUS_ERROR_INTERVAL set to 7s
 * 
 * 19    23.01.08 18:16 Chhamaa
 * Defines for external TDM bus monitoring modified.
 * 
 * 18    08-01-10 13:24 Chrobif
 * Some comments added.
 * 
 * 17    07-11-20 11:56 Chalnot
 * NO_TDM_BUS_ERROR_INTERVAL set to 10s instead 3s.
 * 
 * 16    27.09.07 16:15 Chhamaa
 * External TDM bus failure (restartFlag) does not trigger watchdog reset.
 * Restart handling in main.c.
 * 
 * 15    07-09-24 13:13 Chalnot
 * Em Fw with compiler switch EMERGENCY_FW implemented.
 * 
 * 14    07-09-04 16:37 Chalnot
 * Tdm bus error handled.
 * Warning and alarm bit set in basic timeslot.
 * extTdmBus_setRdyLedBlink() added.
 * 
 * 13    07-08-31 12:45 Chrobif
 * + readDuSlicInterrupts() implemented and call added in scheduler.
 * + Call of appControl_activateRingSequence() added in scheduer.
 * + Call of status_activate() added in scheduler.
 * + extTdmBus_setReadDuSlicIntFlag() implemented.
 * 
 * 12    07-08-30 15:12 Chalnot
 * Memory check added.
 * 
 * 11    28.08.07 14:14 Chhamaa
 * Compiler swtich BLINK_LED added.
 * 
 * 10    27.08.07 16:06 Chhamaa
 * Compiler swtich WATCHDOG added.
 * 
 * 9     16.08.07 19:42 Chhamaa
 * External TDM bus monitoring added.
 * 
 * 8     8.08.07 14:34 Chhamaa
 * Functions for e- and m-wire signal handling added.
 * 
 * 7     4.07.07 13:31 Chhamaa
 * Function call DuSlic_SpiFIFO() added.
 * 
 * 6     07-07-03 17:14 Chrobif
 * Cycle count infrastructure added.
 * 
 * 5     2.07.07 10:10 Chhamaa
 * calculateBasicTimeSlotChecksum() moved to extTdmBus_activate().
 * 
 * 4     29.05.07 16:29 Chrobif
 * Call to Init_getBasicTimeSlotID() added to determine physical slot id.
 * 
 * 3     29.05.07 9:29 Chrobif
 * Bug fix in transmitFT12Frame().
 * 
 * 2     25.05.07 16:37 Chrobif
 * Define renamed to FT12_FRAME_TRAILER_LENGTH.
 * 
 * 1     22.05.07 17:06 Chrobif
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/
 
/* COMPILER SWITCHES *********************************************************/

//#define WATCHDOG		// enables watchdog functions, set in release config
						// by VisualDSP IDE

//#define MEMORYCHECK	// enables Memory Check, set in release configuration
						// by VisualDSP IDE	
						
/* INCLUDE FILES *************************************************************/
// C Run-Time Library
#include <cycles.h>
#include <assert.h>

// O4CV firmware
#include "extTdmBus.h"
#include "ft12.h"
#include "basetype.h"
#include "dll2ndStation.h"
#include "Basetype_O4CV.h"
#include "epromMed.h"
#include "Initialize.h"
#include "voiceData.h"
#include "DuSlic.h"
#include "binInOut.h"
#include "watchdog.h"
#include "memoryCheck.h"
#include "appControl.h"
#include "status.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define BASIC_TS_DST_BIT_MASK			BIT4
#define BASIC_TS_DST_BIT_POSITION		4
#define BASIC_TS_TOGGLE_BIT_MASK		BIT5
#define BASIC_TS_WARNING_BIT_MASK		BIT6	
#define BASIC_TS_ALARM_BIT_MASK			BIT7			
#define BASIC_TS_BTS_BITS_MASK			(BIT0 |BIT1 |BIT2 |BIT3)
#define BASIC_TS_CRC_BITS_MASK			(BIT12 | BIT13 | BIT14 | BIT15)

#define COUNTER_NO_TOGGLING_MAX			320		// 3s (@ 106.7 Hz)
#define CS_ERROR_RATE_EVAL_TIME			53333	// 5s (@ 10.67 kHz)

#define RX_TS_ID_MM_COUNTER_MAX			20000	// max. number of rx time slot ID mismatches
#define CS_ERROR_COUNTER_MAX			20000	// max. number of checksum errors
#define CS_ERROR_RATE_MAX				20000	// max. checksum error rate

#define TB_MONITOR_DISABLE_TH			100		// number checksum errors to disable toggle bit monitor
#define TB_MONITOR_DISABLE_HOLD_TIME	10667	// 10s (@1067 Hz)

#define LED_BLINK_INTERVAL				106		// 1s (@ 106.7 Hz)
#define DUSLIC_READ_INT_INTERVAL		3		// 0.28s (@ 106.7 Hz)

#define NO_TDM_BUS_ERROR_INTERVAL		0		// 0s (@ 1067 Hz) -> alarm deactivated

/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef enum 
{
	e_ft12RxStateIdling,
	e_ft12RxStateReceiving,
	e_ft12RxStateDecoding,
	e_ft12RxStateWaiting
} e_ft12RxState;

typedef enum 
{
	e_ft12TxStateIdling,
	e_ft12TxStateTransmitting
} e_ft12TxState;

typedef struct
{
    UINT16 rxTimeSlotIdMismatchCounter;
    UINT16 csErrorCounter;
    UINT16 csErrorRate;
    bool restartFlag;
    
} t_ExtTdmBusStatus;

typedef struct
{
    bool enabled;
    UINT16 lastToggleBit;
	UINT16 counterNoToggling;
	UINT16 holdCounter;

} t_ToggleBitMonitor;

typedef struct
{
	UINT16 timer;
    UINT16 errorCounter;
    
} t_CsErrorRate;


/* LOCAL VARIABLE DECLARATIONS ***********************************************/
static UINT16 gs_caseCounter;
static UINT16 gs_subCaseCounter;
static UINT16 gs_physicalSlotId;
static UINT16 gs_noErrorCounter;
#ifdef DO_CYCLE_COUNTS
	static cycle_stats_t gs_stats;
#endif

static t_ExtTdmBusStatus gs_extTdmBusStatus;
static t_O4CVStatus *gs_pStatus;
static t_ToggleBitMonitor gs_rxTbMonitor;
static t_CsErrorRate gs_csErrorRate;
static UINT16 gs_txToggleBit;
static bool gs_resetO4CV;
static bool gs_tdmBusRestartActive = false;
static bool gs_readDuSlicIntStarted;
static bool gs_rdyLedBlinking;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
static bool receiveBasicTS( UINT16   basicTSRx);
static void transmitBasicTS(UINT16 *pBasicTSTx);
static void receiveFT12Frame( UINT16  basicTS);
static void transmitFT12Frame(UINT16 *pBasicTS);
static UINT16 calculateBasicTimeSlotChecksum(UINT16 basicTSValue);

static void monitorRxToggleBit(UINT16 basicTsRx);
static void monitorExtTdmBusStatus(void);
static void calcCsErrorRate(bool crcValid);

static void blinkRdyLed(void);
static void readDuSlicInterrupts(void);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void extTdmBus_init(void)
  Description:	This function is used to initialise the external TDM bus services
  				module. It is called once at the beginning in the main function
  				before extTdmBus_activate() is called.
  Inputs:		None
  Outputs:		None
  Return:       None	
  Globals:		Various
  Important:	-
*******************************************************************************/
void extTdmBus_init(void)
{
	gs_caseCounter = 0;
	gs_subCaseCounter = 0;
	gs_physicalSlotId = (UINT16) Init_getBasicTimeSlotID();
	gs_readDuSlicIntStarted = false;
	gs_rdyLedBlinking = false;
	gs_noErrorCounter = NO_TDM_BUS_ERROR_INTERVAL; 
	
	// Initialize status
	gs_extTdmBusStatus.rxTimeSlotIdMismatchCounter = 0;
    gs_extTdmBusStatus.csErrorCounter = 0;
    gs_extTdmBusStatus.csErrorRate = 0;
    gs_extTdmBusStatus.restartFlag = false;
    
    // Initialize checksum error rate calculation
    gs_csErrorRate.timer = 0;
    gs_csErrorRate.errorCounter = 0;
    
    // Initialize rx toggle bit monitor
    if (!gs_tdmBusRestartActive)
    {
	    gs_rxTbMonitor.enabled = true;
	    gs_rxTbMonitor.lastToggleBit = 0;
		gs_rxTbMonitor.counterNoToggling = 0;
		gs_rxTbMonitor.holdCounter = 0;
    }
    else
    {
        gs_tdmBusRestartActive = false;
    }
	
    // Get O4CV Status
    gs_pStatus = status_getPCurrentStatus();
    
	// Initialize the cycle statistics infrastructure
	CYCLES_INIT(gs_stats);
}

/*****************************************************************************
  Function:		void extTdmBus_activate(UINT16 *pExtTdmRxBuffer, UINT16 *pExtTdmTxBuffer)
  Description:	This function performs the scheduling of the external TDM bus
  				services. It is periodically called in the interrupt service
  				routine sport_irqExtTDMBus().
  				Initially the repetition rate will be 10.67 kHz.
  				In case of a reduced repetition rate this function has to be modified.
  Inputs:		pExtTdmRxBuffer: Pointer to the received timeslots
  Outputs:		pExtTdmTxBuffer: Pointer to the timeslots to be transmitted
  Return:       None	
  Globals:		Various
  Important:    -                            	
*******************************************************************************/
void extTdmBus_activate(UINT16 *pExtTdmRxBuffer, UINT16 *pExtTdmTxBuffer)
{	// runs @ 10.7 kHz

	bool cSValid;
	
	// Start cycle count
	CYCLES_START(gs_stats);
	
	// init tx time slot
	*pExtTdmTxBuffer = 0;
	
	////////////////////////////
    // Service basic timeslot //
    ////////////////////////////
	cSValid = receiveBasicTS(*pExtTdmRxBuffer);
	transmitBasicTS(pExtTdmTxBuffer);

#ifndef EMERGENCY_FW
	
	////////////////////////////
    // Service extended timeslots //
    ////////////////////////////
	voiceData_send(pExtTdmTxBuffer);
	voiceData_receive(pExtTdmRxBuffer, cSValid);
	
#endif
	
	// set checksum
	*pExtTdmTxBuffer |= calculateBasicTimeSlotChecksum(*pExtTdmTxBuffer) & BASIC_TS_CRC_BITS_MASK;
	
	// buzzer driver
	binInOut_sampleToBuzzer();

	///////////////////
	// Run scheduler //
	///////////////////
    switch (gs_caseCounter)
    {  
	    case 0:
	    	// runs @ 1.07 kHz
	    	dll2ndStation_activate();
	    	gs_caseCounter = 1;
	    	break;
    	
	    case 1:
	    	// runs @ 1.07 kHz
	    	epromMed_activate();
	    	gs_caseCounter = 2;
	    	break;
    	
	    case 2:
	    	// runs @ 1.07 kHz
	    	//---poll SPI FIFO and execute commands from FIFO. Let it run in main			
			DuSlic_SpiFIFO();
	    	gs_caseCounter = 3;
	    	break;
    	
	    case 3:
	    	// runs @ 1.07 kHz
    	
	    	// send e-wire samples
	    	binInOut_sendEWire();
    	
	    	// receive m-wire samples
	    	binInOut_receiveMWire();
    	
	    	gs_caseCounter = 4;
	    	break;
    	
	    case 4:
	    	// runs @ 1.07 kHz
	    	
	    	monitorExtTdmBusStatus();
	    	
	    	gs_caseCounter = 5;
	    	break;
    	
	    case 5:
	    	// runs @ 1.07 kHz
	    	
	    	binInOut_detectRingCurrent();
	    	
	    	gs_caseCounter = 6;
	    	break;
    	
	    case 6:
	    	// runs @ 1.07 kHz
	    	
#ifdef MEMORYCHECK	
	    	
	    	memoryCheck_doMemoryCheck();
#endif	    	
	    	gs_caseCounter = 7;
	    	break;
    	
	    case 7:
	    	// runs @ 1.07 kHz
	    	gs_caseCounter = 8;
	    	break;
    	
	    case 8:
	    	// runs @ 1.07 kHz
	    	gs_caseCounter = 9;
	    	break;
    	
	    case 9:
	    	// runs @ 1.07 kHz
	    	
	    	switch (gs_subCaseCounter)
		    { 
		        case 0:		// runs @ 107 Hz
			    	
		        	// monitor rx toggle bit
		        	monitorRxToggleBit(*pExtTdmRxBuffer);
		        	
		        	// generate tx toggle bit
		        	gs_txToggleBit ^= BASIC_TS_TOGGLE_BIT_MASK;	
		        
			    	gs_subCaseCounter = 1;
			    	break;
			    	
			    case 1:		// runs @ 107 Hz

#ifdef WATCHDOG	    			
			    	// serve watchdog
			    	watchdog_refresh();
#endif
			    	gs_subCaseCounter = 2;
			    	break;
			    	
				case 2:		// runs @ 107 Hz
				
					if (gs_resetO4CV)
			    	{
#ifdef WATCHDOG
			    	    watchdog_hwReset();
#endif
			    	}
	    			
			    	gs_subCaseCounter = 3;
			    	break;
			    	
				case 3:		// runs @ 107 Hz
				
					blinkRdyLed();
					gs_subCaseCounter = 4;
					break;

				case 4:		// runs @ 107 Hz
				    readDuSlicInterrupts();
					gs_subCaseCounter = 5;
					break;

				case 5:		// runs @ 107 Hz
					appControl_activateRingSequence();
					gs_subCaseCounter = 6;
					break;
					
				case 6:		// runs @ 107 Hz
	    			status_activate();	
			    	gs_subCaseCounter = 7;
			    	break;
			    	
			    case 7:		// runs @ 107 Hz
	    			
			    	gs_subCaseCounter = 8;
			    	break;
			    	
			    case 8:		// runs @ 107 Hz
	    			
			    	gs_subCaseCounter = 9;
			    	break;
			    
			    case 9:		// runs @ 107 Hz
	    			
			    	gs_subCaseCounter = 0;
			    	break;
			    
			    default:
			    
			    	gs_subCaseCounter = 0;	
		    }
	    	
	    	gs_caseCounter = 0;
	    	break;
    	
	    default:
	    	gs_caseCounter = 0;
		}
	
	// Stop cycle count
	CYCLES_STOP(gs_stats);
}

/*****************************************************************************
  Function:		bool extTdmBus_getRestartFlag(void)
  Description:	Returns the restart flag of external TDM bus status.
  Inputs:		None
  Outputs:		None
  Return:		restart flag
  Globals:		gs_extTdmBusStatus
  Important:	-
*******************************************************************************/
bool extTdmBus_getRestartFlag(void)
{
	return(gs_extTdmBusStatus.restartFlag);    
}

/*****************************************************************************
  Function:		void extTdmBus_setReadDuSlicIntFlag(bool flag)
  Description:	extTdmBus_setReadDuSlicIntFlag() can be used to start or stop
  				the periodic read of DuSlic interrupts over SPI.
  Inputs:		flag: True, interrupts are read. False, interrupts are not read.
  Outputs:		None
  Return:		None
  Globals:		gs_readDuSlicIntStarted
  Important:	-
*******************************************************************************/
void extTdmBus_setReadDuSlicIntFlag(bool flag)
{
    gs_readDuSlicIntStarted = flag;
}

/*****************************************************************************
  Function:		bool extTdmBus_getReadDuSlicIntFlag(void)
  Description:	extTdmBus_getReadDuSlicIntFlag() can be used to get the state of
  				the periodic read of DuSlic interrupts over SPI mechanism.
  Inputs:		None
  Outputs:		None
  Return:		True, if DuSlic interrupts are periodically read.
  				False, otherwise.
  Globals:		gs_readDuSlicIntStarted
  Important:	-
*******************************************************************************/
bool extTdmBus_getReadDuSlicIntFlag(void)
{
	return	gs_readDuSlicIntStarted;
}

/*****************************************************************************
  Function:		void extTdmBus_setRdyLedBlink(bool state)
  Description:	extTdmBus_setRdyLedBlink() enables/disables the blinking of the 
  				rdy led
  Inputs:		state: True, rdy led blinks. False, rdy led does not blink.
  Outputs:		None
  Return:		None
  Globals:		gs_rdyLedBlinking
  Important:	-
*******************************************************************************/
void extTdmBus_setRdyLedBlinking(bool state)
{
    gs_rdyLedBlinking = state;
}

/*****************************************************************************
  Function:		void receiveBasicTS(UINT16 basicTSRx)
  Description:	receiveBasicTS() processes the bits in the received basic timeslot.
  Inputs:		basicTSRx: Received basic time slot
  Outputs:		None
  Return:		None
  Globals:		gs_extTdmBusStatus, gs_physicalSlotId
  Important:	-
*******************************************************************************/
bool receiveBasicTS(UINT16 basicTSRx)
{
	UINT16 calculatedCS = 0;
	UINT16 receivedCS   = 0;
	UINT16 receivedSlotId = 0;
	bool cSValid     = true;
	bool slotIdValid = true;

	// Check basic timeslot checksum
    calculatedCS = calculateBasicTimeSlotChecksum(basicTSRx);
    receivedCS   = basicTSRx & BASIC_TS_CRC_BITS_MASK;
    if ( calculatedCS != receivedCS )
    {
    	cSValid = false;
    	gs_extTdmBusStatus.csErrorCounter++;
    }
    else
    {
		gs_extTdmBusStatus.csErrorCounter = 0;
    }
    
    calcCsErrorRate(cSValid);
    
    // Check basic time slot id == physical slot id
    receivedSlotId = basicTSRx & BASIC_TS_BTS_BITS_MASK;
    if ( gs_physicalSlotId != receivedSlotId )
    {
    	slotIdValid = false;
    	gs_extTdmBusStatus.rxTimeSlotIdMismatchCounter++;
    }
    else
    {
        gs_extTdmBusStatus.rxTimeSlotIdMismatchCounter = 0;
    }
    
    if ( slotIdValid && cSValid )
    {
        // Extract Data Soft Timing (DST) bit
        receiveFT12Frame(basicTSRx);
    }
    
    return cSValid;
}

/*****************************************************************************
  Function:		void transmitBasicTS(UINT16 *pBasicTSTx)
  Description:	transmitBasicTS() sets the bits in the basic timeslot to be transmitted.
  Inputs:		None
  Outputs:		pBasicTSTx: Pointer to the basic timeslot to be transmitted
  Return:		None
  Globals:		gs_physicalSlotId, gs_txToggleBit, gs_pStatus
  Important:	-
*******************************************************************************/
void transmitBasicTS(UINT16 *pBasicTSTx)
{
    // Set basic timeslot id (BTS)
    *pBasicTSTx |= gs_physicalSlotId & BASIC_TS_BTS_BITS_MASK;
    
    // Set data soft timing bit (DST)
    transmitFT12Frame(pBasicTSTx);
    
    // Set toggle bit
    *pBasicTSTx |= gs_txToggleBit;
    
    // Set warning bit
    if ((gs_pStatus->warnings & O4CV_WARNING_MASK_LED) != O4CV_WARNING_NO_WARNING)
	{
	 	*pBasicTSTx |= BASIC_TS_WARNING_BIT_MASK;   
	}
 
    // Set HW alarm bit
    if (gs_pStatus->alarms != O4CV_ALARM_NO_ALARM)	
	{
    	*pBasicTSTx |= BASIC_TS_ALARM_BIT_MASK;
	}
}

/*****************************************************************************
  Function:		void receiveFT12Frame(UINT16 basicTS)
  Description:	receiveFT12Frame() receives one data soft timing bit (DST) per
  				call which it uses to assemble a FT1.2 frame.
  Inputs:		basicTS: Received basic time slot
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	-
*******************************************************************************/
void receiveFT12Frame(UINT16 basicTS)
{
    static UINT32  s_rxDSTBitBuffer = 0;
	static UINT16  s_lengthFromHeader = 0;
	static UINT16  s_numOfRxBits = 0;
	static UINT16  s_numOfRxWords = 0;
	static INT32   s_ft12Checksum = 0;
	static UINT16  s_timeoutCounter = 0;
	static UINT32 *s_pRxData = NULL;
    static e_ft12RxState s_state = e_ft12RxStateIdling;
		
	// Shift the current DST bit into the DST bit buffer from the right
    s_rxDSTBitBuffer = ( ((UINT32) (basicTS & BASIC_TS_DST_BIT_MASK)) << 27 ) |
    				   ( s_rxDSTBitBuffer >> 1 );
    
	switch (s_state)
	{

	// In the idle state the receiver checks the DST bit buffer for a message start
	// flag [0x68 L1 L2 0x68]. If a start flag was received and L1 = L2 a transition
	// to the receiving state takes place. If L1 != L2 a transition to the waiting
	// state occurs.
	case e_ft12RxStateIdling:
		if ( (s_rxDSTBitBuffer & FT12_START_FLAG_MASK) == FT12_START_FLAG )
		{
			s_lengthFromHeader = (s_rxDSTBitBuffer >> 8) & MASK_8LSB;
			if (s_lengthFromHeader == ((s_rxDSTBitBuffer >> 16) & MASK_8LSB))
			{
				s_state = e_ft12RxStateReceiving;
				
				s_pRxData = ft12_rxGetPtrData();
				
				*s_pRxData++ = s_rxDSTBitBuffer << 8;
				s_numOfRxWords = 1;
				
				// Reuse the first start character for the second word (See memory layout
				// in ft12.h
				s_rxDSTBitBuffer = s_rxDSTBitBuffer << 24;
				s_numOfRxBits = 8;
				
				// The checksum is calculated over the bytes from the control field to the
				// last byte of the user data. Substract FT12_START_CHAR now because it will
				// be added later on due to the memory layout (See memory layout in ft12.h)
				s_ft12Checksum = -FT12_START_CHAR;
				
				s_lengthFromHeader = (s_lengthFromHeader / NR_OF_BYTES_IN_LONG) + FT12_FRAME_TRAILER_LENGTH;
			}
			else
			{
				s_state = e_ft12RxStateWaiting;
				s_timeoutCounter = 0;
			}
		}
		break;

	// In the receiving state bit for bit is received. If 32 bit are received the word is written
	// to the data array of the rx frame in the ft12 module and the FT1.2 checksum is computed on the
	// fly. When all words have been received it is checked whether the calculated checksum
	// matches the checksum in the FT1.2 frame and if the the end character is found.
	// If one of these checks fails the FT1.2 frame is dropped and the state machine makes a transition
	// to the wait state. Otherwise a trasition to the decoding state takes place.
	case e_ft12RxStateReceiving:
		s_numOfRxBits++;
		if (s_numOfRxBits >= 32)
		{
			if (s_numOfRxWords >= s_lengthFromHeader)
			{
				*s_pRxData = s_rxDSTBitBuffer;
				if ( ((s_rxDSTBitBuffer & FT12_END_FLAG_MASK) == FT12_END_FLAG)  && 
					 ((s_rxDSTBitBuffer & FT12_CHECKSUM_MASK) == (s_ft12Checksum & MASK_8LSB))
				   )
				{							
					s_state = e_ft12RxStateDecoding;
				}
				else
				{
					s_state = e_ft12RxStateWaiting;
					s_timeoutCounter = 0;
				}
			}
			else
			{
				s_numOfRxBits = 0;
				s_numOfRxWords++;
				*s_pRxData++ = s_rxDSTBitBuffer;
				s_ft12Checksum = s_ft12Checksum +  (s_rxDSTBitBuffer & MASK_8LSB) +
										       	  ((s_rxDSTBitBuffer >> 8) & MASK_8LSB) +
										          ((s_rxDSTBitBuffer >> 16) & MASK_8LSB) +
										          ((s_rxDSTBitBuffer >> 24) & MASK_8LSB);
			}
		}
		break;

	// In the decoding state the FT1.2 frame is decoded which means that the various fields of the
	// protocol header are extracted. On completion the state of the FT1.2 rx frame is set to full
	// which triggers the secondary station to process the frame. The state machine makes a transition
	// to the waiting state.
	case e_ft12RxStateDecoding:
		ft12_rxMsgDecode();
		ft12_rxSetState(e_ft12StateFull);
		s_state = e_ft12RxStateWaiting;
		s_timeoutCounter = 0;
		break;
			
	// The state machine remains 25 times in the waiting state. Then it makes a transition back to the
	// idling state. The number 25 comes from the implementation of the O4LE. It is not obvious why
	// it is 25 and not another number.
	case e_ft12RxStateWaiting:
		s_timeoutCounter++;
		if (s_timeoutCounter > 25)
		{
			s_state = e_ft12RxStateIdling;
		}
		break;

	// This should never happen. However, if it does the state machine makes a transition to the
	// waiting state.
	default:
		s_state = e_ft12RxStateWaiting;
		s_timeoutCounter = 0;
		break;
	}	
}

/*****************************************************************************
  Function:		void transmitFT12Frame(UINT16 *pBasicTS)
  Description:	transmitFT12Frame() transmits a FT1.2 frame. It writes one 
  				data soft timing bit (DTS) to the basic timeslot per call.
  Inputs:		None
  Outputs:		pBasicTS: Pointer to the basic timeslot to be transmitted
  Return:		None
  Globals:		None
  Important:	-
*******************************************************************************/
void transmitFT12Frame(UINT16 *pBasicTS)
{
	static UINT16 s_numOfTxBits = 0;
	static UINT16 s_bytesTransmitted = 0;
	static UINT16 s_bytesToBeTransmitted = 0;
	static UINT32 s_dataToBeTransmitted = 0;
	static UINT32 *s_pTxData = NULL;
	static e_ft12TxState s_state = e_ft12TxStateIdling;

	switch (s_state)
	{

	// The transmitter state machine idles as long as the state of the tx frame is not full. If
	// it becomes full the state machine makes a transition to the transmitting state.
	case e_ft12TxStateIdling:
		if (ft12_txGetState() == e_ft12StateFull)
		{
			s_pTxData = ft12_txGetPtrData();
			
			s_bytesToBeTransmitted = (((*s_pTxData >> SB16) & MASK_8LSB)) + FT12_FRAME_OVERHEAD;
			
			// The lowest 8 bits are dumped. See memory layout in ft12.h
			s_dataToBeTransmitted = *s_pTxData >> SB8;
			s_numOfTxBits = 8;
			s_bytesTransmitted = 0;
			
			s_state = e_ft12TxStateTransmitting;
		}
		break;
		
	// In the transmitting state bit for bit is transmitted in the basic time slot. If the FT1.2
	// frame is completely transmitted the state of the tx frame is set to transmitted and
	// a transition to the idling state takes place.
	case e_ft12TxStateTransmitting:
		if (s_bytesTransmitted < s_bytesToBeTransmitted)
		{	
			*pBasicTS |= (UINT16)(s_dataToBeTransmitted & BIT0) << BASIC_TS_DST_BIT_POSITION;
			s_dataToBeTransmitted >>= 1;
			s_numOfTxBits++;
			if (!(s_numOfTxBits % NR_OF_BITS_IN_BYTE)) s_bytesTransmitted++;
			if (s_numOfTxBits >= NR_OF_BITS_IN_LONG)
			{
					s_numOfTxBits = 0;
					s_pTxData++;
					s_dataToBeTransmitted = *s_pTxData;
			}
		}
		else
		{
			ft12_txSetState(e_ft12StateTransmitted);
			s_state = e_ft12TxStateIdling;
		}
		break;

	// This should never happen. However, if it does the state machine makes a transition to the
	// idling state.
	default:
		ft12_txSetState(e_ft12StateEmpty);
		s_state = e_ft12TxStateIdling;
	}
}

/*****************************************************************************
  Function:		UINT16 calculateBasicTimeSlotChecksum(UINT16 basicTSValue)
  Description:	calculateBasicTimeSlotChecksu() calculates the 4 bit checksum of
  				a basic timeslot according to the following formula:
					input[0:3] + input[4:7] + input[8:11]
					(with input = BasicTimeSlotValue)		
  Inputs:		basicTSValue: Basic time slot over with to compute the checksum
  Outputs:		None
  Return:		Returns the 4-bit checksum of the basic time slot as bits 
				bits 12 ... 15 of the return value
  Globals:		None
  Important:	-
*******************************************************************************/
UINT16 calculateBasicTimeSlotChecksum(UINT16 basicTSValue)
{
    UINT16 CheckSum;

    CheckSum = 1;

    CheckSum = CheckSum +  basicTSValue;
    CheckSum = CheckSum + (basicTSValue >> 4);
    CheckSum = CheckSum + (basicTSValue >> 8);

    CheckSum = CheckSum & 0x000F;

    CheckSum = CheckSum << 12;

    return(CheckSum);

}


/*****************************************************************************
  Function:		static void monitorRxToggleBit(UINT16 basicTsRx)
  Description:	Monitors the toggle bit in the basic time slot sent by P4LW.
  				If toggling is stopped for longer than 3s the O4CV card is reset. 		
  Inputs:		basicTsRx: value of the basic rx time slot
  Outputs:		None
  Return:		None
  Globals:		gs_resetO4CV
  Important:	This function hast to be called at 106.67 Hz repetition rate.
*******************************************************************************/

static void monitorRxToggleBit(UINT16 basicTsRx)
{
    
	if (gs_rxTbMonitor.enabled)
	{
		// check if toggle bit has changed
		if (gs_rxTbMonitor.lastToggleBit == (basicTsRx & BASIC_TS_TOGGLE_BIT_MASK))
		{
			gs_rxTbMonitor.counterNoToggling++;
			if (gs_rxTbMonitor.counterNoToggling > COUNTER_NO_TOGGLING_MAX)
			{
				gs_resetO4CV = true;
			}
		}
		else
		{
			gs_rxTbMonitor.counterNoToggling = 0;
		}
	}
	else
	{
	    gs_rxTbMonitor.counterNoToggling = 0;
	}
	
	// update last rx toggle bit
	gs_rxTbMonitor.lastToggleBit = basicTsRx & BASIC_TS_TOGGLE_BIT_MASK;
	
}


/*****************************************************************************
  Function:		static void monitorExtTdmBusStatus(void)
  Description:	Monitors the external TDM Bus status. In case of excessive
  				errors the restart flag is set.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		gs_extTdmBusStatus
  Important:	-
*******************************************************************************/

static void monitorExtTdmBusStatus(void)
{

    // Set the tdm bus alarm if an error has occured for certain times
	if ((gs_extTdmBusStatus.rxTimeSlotIdMismatchCounter > RX_TS_ID_MM_COUNTER_MAX) ||
    	(gs_extTdmBusStatus.csErrorCounter > CS_ERROR_COUNTER_MAX) ||
    	(gs_extTdmBusStatus.csErrorRate > CS_ERROR_RATE_MAX))
	{
	    gs_extTdmBusStatus.restartFlag = true; 
	    gs_pStatus->alarms |= O4CV_ALARM_TDM_BUS_ERROR;
	    gs_noErrorCounter = NO_TDM_BUS_ERROR_INTERVAL;
	    
	    gs_tdmBusRestartActive = true;
	    gs_rxTbMonitor.enabled = false;
	    gs_rxTbMonitor.holdCounter = TB_MONITOR_DISABLE_HOLD_TIME;
	    
	}
	
	// If no ext tdm bus error occured for NO_TDM_BUS_ERROR_INTERVAL then the alarm gets cleared
	if(gs_noErrorCounter > 0)
	{
	    gs_noErrorCounter--;
	}
	else
	{
	 	gs_pStatus->alarms &= ~O4CV_ALARM_TDM_BUS_ERROR;   
	}
	
	// check if rx toggle bit monitor has to be disabled
	if (gs_extTdmBusStatus.csErrorCounter > TB_MONITOR_DISABLE_TH)
	{
	    gs_rxTbMonitor.enabled = false;
	    gs_rxTbMonitor.holdCounter = TB_MONITOR_DISABLE_HOLD_TIME;
	}
	else if (gs_rxTbMonitor.holdCounter > 0)
	{
		gs_rxTbMonitor.holdCounter--;
	}
	else
	{    
		gs_rxTbMonitor.enabled = true;
	}
}


/*****************************************************************************
  Function:		static void calcCsErrorRate(bool crcValid)
  Description:	Counts the checksum errors during a predefined time interval
  				(CS_ERROR_RATE_EVAL_TIME) and updates the external TDM bus 
  				status accordingly. 
  Inputs:		crcValid: state of the CRC check
  Outputs:		None
  Return:		None
  Globals:		gs_extTdmBusStatus
  Important:	-
*******************************************************************************/

static void calcCsErrorRate(bool crcValid)
{
    
    if (!crcValid)
    {
    	gs_csErrorRate.errorCounter++;
    }
    
    gs_csErrorRate.timer++;
    
    if (gs_csErrorRate.timer >= CS_ERROR_RATE_EVAL_TIME)
    {
        gs_extTdmBusStatus.csErrorRate = gs_csErrorRate.errorCounter;
        gs_csErrorRate.errorCounter = 0;
        gs_csErrorRate.timer = 0;
    }
}


/*****************************************************************************
  Function:		static void blinkRdyLed(void)
  Description:	Blinks the ready LED. 
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		gs_rdyLedBlinking	
  Important:	-
*******************************************************************************/

static void blinkRdyLed(void)
{
    static UINT16 timer = 0;
    static bool ledOn = true;
    
				
	if(gs_rdyLedBlinking)
    {
	    timer++;
    
	    if (timer >= LED_BLINK_INTERVAL)
	    {
	        timer = 0;
        
	        if (ledOn)
	        {
	            ledOn = false;
	            binInOut_setOutput(0, e_Ready_Led);
	        }
	        else
	        {
	            ledOn = true;
	            binInOut_setOutput(1, e_Ready_Led);
	        }
	    }
    }
}

/*****************************************************************************
  Function:		static void readDuSlicInterrupts(void)
  Description:	readDuSlicInterrupts() writes a command to the SPI FIFO which
  				reads the interrupt register in the DuSlic.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	gs_readDuSlicIntStarted
*******************************************************************************/

static void readDuSlicInterrupts(void)
{
	static UINT16 timer = 0;
	
	if (gs_readDuSlicIntStarted)
	{
	    timer++;
	    
	    if (timer >= DUSLIC_READ_INT_INTERVAL)
	    {
	        DuSlic_readAllInt();
	        timer = 0;
	    }
	}
}
    

