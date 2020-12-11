/*-------------------------------------------------------------------------*
 * $Workfile	: binInOut.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The module binInOut handles the binary input/output signals 
 * 				  connected to general purpose pins of the DSP 
 * 				  (LEDs, M-wire, E-wire, local/transit, on/off hook, buzzer, ...). 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/binInOut.c $
 * 
 * 26    08-02-04 15:53 Chrobif
 * - Telefonie bug for SPH fixed
 * 
 * 25    08-01-29 18:36 Chrobif
 * + binInOut init source added
 * + binInOut_initTxEWireAndRxMWire() modified such that variables which
 * store part of the telephony system state are only initialised if
 * telephony state machines are initialized
 * 
 * 24    07-12-11 10:20 Chrobif
 * + Some critical sections protected
 * + N_CHANNELS_ON_BOARD used instead of number
 * 
 * 23    07-12-10 16:58 Chrobif
 * + Ringing E-/M-wire detection optimized.
 * 
 * 22    07-12-05 8:08 Chrobif
 * + Debug code removed.
 * 
 * 21    07-12-05 8:03 Chrobif
 * + binInOut_clearRingDetector() added.
 * + Bug fix in delayed E- and M-wire.
 * 
 * 20    07-11-28 10:46 Chrobif
 * + RINGING_PAX_TIMEOUT changed to 6.51s because counter starts after
 * negative edge.
 * + Ringing M-wire detection implemented
 * 
 * 19    07-11-27 16:20 Chrobif
 * + Ringing E-wire detection added
 * + bin_InOut_toggleTestOutPin() made "public"
 * + bin_InOut_toggleOutPin() added
 * + delayed E-wire function modified such that only negative edge is
 * delayed.
 * + delayed M-wire function modified such that only negative edge is
 * delayed.
 * + g_eWireSender.lastValidSample initialized with CLEAR for all
 * interface types on first call off binInOut_initTxEWireAndRxMWire().
 * During subsequent calls of this function g_eWireSender.lastValidSample
 * is not reinitialized.
 * + Last GPIO configured as output in series HW case
 * + Debug telephony infrastructure added.
 * 
 * 18    16.10.07 14:49 Chhamaa
 * 4-wire interface: e-wire output inverted
 * 
 * 17    07-10-11 13:00 Chalnot
 * New pins for ALARM_LED and BUZZER_N assigned.
 * 
 * 16    26.09.07 17:41 Chhamaa
 * Periodic insertion of m-wire frames added.
 * 
 * 15    07-09-24 9:45 Chrobif
 * Define PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION added.
 * 
 * 14    18.09.07 19:20 Chhamaa
 * E-wire interpolation added.
 * 
 * 13    07-09-13 15:00 Chrobif
 * + M-wire sampling commands introduced.
 * + binInOut_insertMWireFrame() added.
 * + Bug fix in binInOut_detectRingCurrent().
 * 
 * 12    12.09.07 14:48 Chhamaa
 * Output pin e_Tdm_Input_En added.
 * 
 * 11    07-09-07 15:07 Chalnot
 * Buzzer one freq functions added.
 * 
 * 10    07-08-31 11:35 Chrobif
 * + M-wire inverted for "Hot line" and "Remote subscriver" case.
 * + binInOut_getEWireDelayed extended for SPH
 * 
 * 9     22.08.07 14:55 Chhamaa
 * Function binInOut_initRingCurrentDetector() modified.
 * 
 * 8     21.08.07 18:03 Chhamaa
 * New functions added: binInOut_getEWireDelayed(),
 * binInOut_getMWireDelayed()
 * 
 * 7     20.08.07 19:02 Chhamaa
 * detectTransition() modified, test code added.
 * 
 * 6     17.08.07 17:33 Chhamaa
 * Bug fixed.
 * 
 * 5     16.08.07 19:17 Chhamaa
 * Ring current detection functions added.
 * 
 * 4     07-08-13 10:02 Chalnot
 * Buzzer functions added.
 * 
 * 3     8.08.07 14:38 Chhamaa
 * Functions implemented.
 * 
 * 2     07-07-20 16:36 Chrobif
 * + binInOut_enableMWireSampling() replaced by
 * binInOut_enableMWireForwarding()
 * + binInOut_disableMWireSampling() replaced by
 * binInOut_disableMWireForwarding()
 * 
 * 1     07-07-13 16:55 Chrobif
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

/* COMPILER SWITCHES *********************************************************/

//#define 	PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION
			// Define this if the firmware runs on prototype hardware without
			// hardware extension for pulse code dialing. This define prevents
			// the sampling of the pulse code dialing GPIOs because 
			// some of them are low, some high and others floating.

/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <assert.h>
#include <ccblkfn.h>
#include <cdefBF538.h>

// O4CV firmware
#include "binInOut.h"
#include "subframe.h"
#include "configuration.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define M_WIRE_DELAY			7		// 210 ms (7*30 ms)
#define E_WIRE_DELAY			7		// 210 ms (7*30 ms)
#define RINGING_PAX_TIMEOUT		217  	// 6.51 s (217*30ms) // We have learned that a 4-wire PAX may ring with the E-wire
															 // instead of hold it constantly set in case of a initialisation
															 // request. This is accounted for in the FXS state machine
															 // by waiting for the next ring before leaving the remote
															 // initialisation request state for the idle state in case
															 // of a remote subscriber configuration.

#define INSERT_MWIRE_FRAME		333	// 10s (333*30ms)

// m-wire subframe length
#define SUBFRAME1_LEN			11		// length of subframe 1
#define SUBFRAME12_LEN			21		// length of subframe 1 + subframe 2

// ring current detector
#define RCD_N_TRANSITIONS_MIN	5
#define RCD_DETECTION_TIME		1067	// 1 s (@ 1.067 kHz)
#define RCD_HOLD_TIME			6400	// 6 s (@ 1.067 kHz)

// ringing e- and m- wire detection
#define E_WIRE_PULSE_TONE_MIN	21	//  600ms + 30ms =  630ms (delayed counter = 0 means 30ms, hence add 30ms)
#define E_WIRE_PULSE_TONE_MAX	87	// 2580ms + 30ms = 2610ms (delayed counter = 0 means 30ms, hence add 30ms)
#define E_WIRE_SILENCE_MAX		205	// 6120ms + 30ms = 6150ms (delayed counter = 0 means 30ms, hence add 30ms)

// buzzer
#define BUZZER_TONE_LENGTH		700 	//(65.6 ms)
#define BUZZER_RING_PERIOD		10700 	//(1 s)

// indexes for transition detection
#define DTR_IX1		7
#define DTR_IX2		15
#define DTR_IX3		23
#define DTR_IX4		(BUFFER_SIZE_M_WIRE_SAMPLES - 1)	//31

// logical states of e- and m-wire
#define SET		1
#define CLEAR	0

// for testing
#define NUM_OF_TOGGLE_OUT_PIN_INSTANCES	4

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef enum
{

	e_Clear,
	e_RingTypeDetection,
	e_SetContinuous,
	e_SetRinging,
	e_SetWait,

} e_RingingWireState;

typedef enum
{
    
    e_Update,
    e_Delay,
    
} e_DelayState;

typedef struct
{
	bool started;
	bool buffer0Active;
	UINT16 sampleIx;
	
	UINT8 eWireServicePhone;
	bool chEnabled[N_CHANNELS_ON_BOARD];
	bool setEWire[N_CHANNELS_ON_BOARD];
	bool clearEWire[N_CHANNELS_ON_BOARD];
	bool invertEWire[N_CHANNELS_ON_BOARD];
	e_OutPinId outputPinId[N_CHANNELS_ON_BOARD];
	UINT8 lastValidSample[N_CHANNELS_ON_BOARD];
	UINT16 delayCounter[N_CHANNELS_ON_BOARD];
	e_DelayState delayedEWireState[N_CHANNELS_ON_BOARD];
	e_RingingWireState ringingEWireState[N_CHANNELS_ON_BOARD];
	UINT16 ringingEWireCounter[N_CHANNELS_ON_BOARD];
	t_mWireData *pChBuffer[N_CHANNELS_ON_BOARD];
	
} t_EWireSender;

typedef enum
{

	e_MWireStartSampling,
	e_MWireStopSampling,
	e_MWireNoCommand

} e_MWireSamplingCommands;

typedef struct
{
    bool started;
    bool buffer0Active;
    bool inSyncOld;
    UINT16 sampleIx;
    
    UINT8 mWireServicePhone;
	bool chEnabled[N_CHANNELS_ON_BOARD];
	bool setMWire[N_CHANNELS_ON_BOARD_PLUS_SPH];
	bool clearMWire[N_CHANNELS_ON_BOARD_PLUS_SPH];
	bool invertMWire[N_CHANNELS_ON_BOARD];
	bool forwardingDisabled[N_CHANNELS_ON_BOARD];
	bool insertMWireFrame[N_CHANNELS_ON_BOARD_PLUS_SPH];
	e_MWireSamplingCommands mWireSamplingCommands[N_CHANNELS_ON_BOARD];
	UINT8 lastSample[N_CHANNELS_ON_BOARD];
	e_DelayState delayedMWireState[N_CHANNELS_ON_BOARD];
	UINT16 delayCounter[N_CHANNELS_ON_BOARD];
	e_RingingWireState ringingMWireState[N_CHANNELS_ON_BOARD];
	UINT16 ringingMWireCounter[N_CHANNELS_ON_BOARD];
	UINT16 frameCounter[N_CHANNELS_ON_BOARD];
	e_InPinId inputPinId[N_CHANNELS_ON_BOARD];
	t_mWireData *pChBuffer[N_CHANNELS_ON_BOARD];

} t_MWireReceiver;


typedef enum {

	e_RcdIdle,
	e_DetectingRc,
	e_RcDetected

} e_RcdStates;


typedef struct
{
    e_RcdStates state;
    e_InPinId inputPinId;
    UINT8 lastSample;
    UINT16 timer;
    UINT16 transitionCounter;
    
} t_RingCurrentDetector;


typedef struct
{
	UINT16 counterTone; 
 	UINT16 counterRing;
 	UINT16 impulsLength;
 	UINT16 counterImpuls; 
 	UINT16 polarity; 
 	bool buzzerOn; 
 	UINT16 numberOfRings;
 	bool buzzerRing; 

} t_Buzzer;


/* GLOBAL VARIABLE DECLARATIONS **********************************************/

t_mWireData *g_pEWireBuffer;
t_mWireData *g_pMWireBuffer;


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static t_mWireData g_eWireBuffer[2][N_CHANNELS_ON_BOARD];
static t_mWireData g_mWireBuffer[2][N_CHANNELS_ON_BOARD];

static t_EWireSender g_eWireSender;
static t_MWireReceiver g_mWireReceiver;

static t_RingCurrentDetector g_rcd[N_CHANNELS_ON_BOARD];

static t_Buzzer g_buzzer;
static t_Buzzer g_buzzerOneFreq;

/* LOCAL MACRO DEFINITIONS ***************************************************/

#define LAST_SAMPLE_LAST_M_WIRE_BUFFER(   i)	(  g_mWireReceiver.lastSample[i] )
#define LAST_SAMPLE_CURRENT_M_WIRE_BUFFER(i)	(  g_mWireReceiver.pChBuffer[i]->mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES-1] )
#define LAST_SAMPLE_LAST_E_WIRE_BUFFER(   i)	(  g_eWireSender.lastValidSample[i] )
#define LAST_SAMPLE_CURRENT_E_WIRE_BUFFER(i)	( (g_eWireSender.pChBuffer[i]->mWireDataReceived) ?							\
												  (g_eWireSender.pChBuffer[i]->mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES-1]) :	\
												  (g_eWireSender.lastValidSample[i]) )

/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void swapEWireBuffer(void);
static void swapMWireBuffer(void);
static bool detectTransition(UINT8 preVal, UINT8 *pBuf);
static bool detectPositiveEdge(UINT8 preVal, UINT8 *pBuf);
static bool detectNegativeEdge(UINT8 preVal, UINT8 *pBuf);
static UINT8 invertSample(UINT8 inVal);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void binInOut_initTxEWireAndRxMWire(t_O4CVConfig *pConfig, e_BinInOutInitSource initSource)
  Description: 	Initialization function for e- and m-wire signal handling.
  Inputs:		pConfig: pointer to configuration data
  				initSource: Indicates the reason for the call of
  							binInOut_initTxEWireAndRxMWire()
  Outputs:
  Return:       	
  Globals:		g_eWireSender, g_mWireReceiver
  Important:	This function has to be called before starting e- and m-wire 
  				functions.
  				g_eWireSender.lastValidSample is only initialized on the first
  				call of this function.
*******************************************************************************/

void binInOut_initTxEWireAndRxMWire(t_O4CVConfig *pConfig, e_BinInOutInitSource initSource)
{
    UINT16 i;
    e_OutPinId eWireOutputPinId[N_CHANNELS_ON_BOARD];
    e_OutPinId hookSwitchOutputPinId[N_CHANNELS_ON_BOARD];
    e_InPinId mWireInputPinId[N_CHANNELS_ON_BOARD];
    e_InPinId pulseCodeDialingInputPinId[N_CHANNELS_ON_BOARD];
    
    // init pin ID arrays
    eWireOutputPinId[CH1] = e_Buf_EW1;
    eWireOutputPinId[CH2] = e_Buf_EW2;
    eWireOutputPinId[CH3] = e_Buf_EW3;
    eWireOutputPinId[CH4] = e_Buf_EW4;
    
    hookSwitchOutputPinId[CH1] = e_Hook_Switch1;
    hookSwitchOutputPinId[CH2] = e_Hook_Switch2;
    hookSwitchOutputPinId[CH3] = e_Hook_Switch3;
    hookSwitchOutputPinId[CH4] = e_Hook_Switch4;
    
    mWireInputPinId[CH1] = e_MW1;
    mWireInputPinId[CH2] = e_MW2;
    mWireInputPinId[CH3] = e_MW3;
    mWireInputPinId[CH4] = e_MW4;
    
    pulseCodeDialingInputPinId[CH1] = e_Pulse_Code_Dialing1;
    pulseCodeDialingInputPinId[CH2] = e_Pulse_Code_Dialing2;
    pulseCodeDialingInputPinId[CH3] = e_Pulse_Code_Dialing3;
    pulseCodeDialingInputPinId[CH4] = e_Pulse_Code_Dialing4;
    
    // init e-wire sender and m-wire receiver
    g_eWireSender.started = false;
    g_mWireReceiver.started = false;
    
    g_eWireSender.sampleIx = 0;
    g_mWireReceiver.sampleIx = 0;
    
    g_eWireSender.buffer0Active = true;
    g_mWireReceiver.buffer0Active = true;
    
    g_pEWireBuffer = &g_eWireBuffer[1][0];
    g_pMWireBuffer = &g_mWireBuffer[1][0];

	g_mWireReceiver.inSyncOld = 0;
    
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
	{
		if ( configuration_isChannelOn(i) )
		{
			switch(configuration_getAfInterfaceMode(i))
			{
				case AF_INTERFACE_HOTLINE:

					switch (initSource)
					{
					    case e_HwStartUp:
					    case e_SwRestart:
					    default:
					    	// M-wire receiver
					    	g_mWireReceiver.chEnabled[i] = false;		// pulse code dialing is not supported
					    	break;
		    	
					    case e_TdmBusRestart:
					    case e_SlaveSync:
					    	// M-wire receiver
					    	// g_mWireReceiver.chEnabled[i]				// keep state
					    	break;
					}
					
					// E-wire sender
					g_eWireSender.chEnabled[i] = false;			// ring current instead of e-wire
					// g_eWireSender.outputPinId[i]				// not used
					// g_eWireSender.invertEWire[i]				// not used
					
					// M-wire receiver
					// g_mWireReceiver.inputPinId[i]			// not used
					// g_mWireReceiver.invertMWire[i]			// not used
					
					break;	

				case AF_INTERFACE_REMOTE_SUB:

					switch (initSource)
					{
					    case e_HwStartUp:
					    case e_SwRestart:
					    default:
					    	// M-wire receiver
					    	g_mWireReceiver.chEnabled[i] = false;		// receiver is used for pulse code dialling in Connected state
					    	break;
		    	
					    case e_TdmBusRestart:
					    case e_SlaveSync:
					    	// M-wire receiver
					    	// g_mWireReceiver.chEnabled[i]				// keep state
					    	break;
					}
					
					// E-wire sender
					g_eWireSender.chEnabled[i] = false;			// ring current instead of e-wire
					// g_eWireSender.outputPinId[i]				// not used
					// g_eWireSender.invertEWire[i]				// not used
					
					// M-wire receiver
					g_mWireReceiver.inputPinId[i] = pulseCodeDialingInputPinId[i];
					g_mWireReceiver.invertMWire[i] = true;		// Signal on GPIO is high if phone is On-Hook
					
					break;	
				
				case AF_INTERFACE_4_WIRE:
					
					switch (initSource)
					{
					    case e_HwStartUp:
					    case e_SwRestart:
					    default:
					    	// M-wire receiver
					    	g_mWireReceiver.chEnabled[i] = true;	// M-wire input is continuously sampled
					    	break;
		    	
					    case e_TdmBusRestart:
					    case e_SlaveSync:
					    	// M-wire receiver
					    	// g_mWireReceiver.chEnabled[i]			// keep state
					    	break;
					}
					
					// E-wire sender
					g_eWireSender.chEnabled[i]	 = true;
					g_eWireSender.outputPinId[i] = eWireOutputPinId[i];
					g_eWireSender.invertEWire[i] = !configuration_isEWireInverted(i);
					
					// M-wire receiver
					g_mWireReceiver.inputPinId[i]  = mWireInputPinId[i];	
					g_mWireReceiver.invertMWire[i] = configuration_isMWireInverted(i);
					
					break;
					
				case AF_INTERFACE_2_WIRE_PAX:
				
					switch (initSource)
					{
					    case e_HwStartUp:
					    case e_SwRestart:
					    default:
					    	// M-wire receiver
					    	g_mWireReceiver.chEnabled[i] = false;	// ring detect instead of m-wire
					    	break;
		    	
					    case e_TdmBusRestart:
					    case e_SlaveSync:
					    	// M-wire receiver
					    	// g_mWireReceiver.chEnabled[i]			// keep state
					    	break;
					}
					
					// E-wire sender
					g_eWireSender.chEnabled[i]		 = true;	// on-/off-hook emulation and pulse code dialling
					g_eWireSender.outputPinId[i] 	 = hookSwitchOutputPinId[i];
					g_eWireSender.invertEWire[i]	 = true;	// 1 is on On-Hook in terms of HW
					
					// M-wire receiver
					//g_mWireReceiver.inputPinId[i]			// not used
					//g_mWireReceiver.invertMWire[i]		// not used
					
					break;
					
				default:
				
					// E-wire sender
					g_eWireSender.chEnabled[i     ] = false;
					// g_eWireSender.outputPinId[i]			// not used
					// g_eWireSender.invertEWire[i]			// not used

					// M-wire receiver
					g_mWireReceiver.chEnabled[i     ] = false;
					// g_mWireReceiver.inputPinId[ i]		// not used
					// g_mWireReceiver.invertMWire[i]		// not used

					assert(false);
			}
		}
		else
		{
			g_eWireSender.chEnabled[     i] = false;
			// g_eWireSender.outputPinId[i]		// not used
			// g_eWireSender.invertEWire[i]		// not used
			
			g_mWireReceiver.chEnabled[     i] = false;
			// g_mWireReceiver.inputPinId[ i]	// not used
			// g_mWireReceiver.invertMWire[i]	// not used
		}
		
		// buffer pointer
		g_eWireSender.pChBuffer[  i] = &g_eWireBuffer[0][i];
		g_mWireReceiver.pChBuffer[i] = &g_mWireBuffer[0][i];
				
		// e-wire sender
		switch (initSource)
		{
		    case e_HwStartUp:
		    case e_SwRestart:
		    default:
		    	g_eWireSender.setEWire[i] = false;				// init
		    	g_eWireSender.clearEWire[i] = false;			// init
		    	g_eWireSender.lastValidSample[i] = CLEAR;		// init
				g_eWireSender.delayCounter[i] = 0;				// init
				g_eWireSender.delayedEWireState[i] = e_Update;	// init
				g_eWireSender.ringingEWireState[i] = e_Clear;	// init
				g_eWireSender.ringingEWireCounter[i] = 0;		// init
		    	break;
		    	
		    case e_TdmBusRestart:
		    case e_SlaveSync:
		    	// g_eWireSender.setEWire[i]			// keep state
		    	// g_eWireSender.clearEWire[i]			// keep state
		    	// g_eWireSender.lastValidSample[i] 	// keep state
		    	// g_eWireSender.delayCounter[i]		// keep state
		    	// g_eWireSender.delayedEWireState[i]	// keep state
		    	// g_eWireSender.ringingEWireState[i] 	// keep state
		    	// g_eWireSender.ringingEWireCounter[i]	// keep state
		    	break;
		}
		
		// m-wire receiver
		switch (initSource)
		{
		    case e_HwStartUp:
		    case e_SwRestart:
		    default:
				g_mWireReceiver.setMWire[i] = false;			// init
				g_mWireReceiver.clearMWire[i] = false;			// init
				g_mWireReceiver.forwardingDisabled[i] = false;	// init
				g_mWireReceiver.insertMWireFrame[i] = false;	// init
				g_mWireReceiver.mWireSamplingCommands[i] = e_MWireNoCommand; // init
				g_mWireReceiver.lastSample[i] = CLEAR;			// init
				g_mWireReceiver.delayedMWireState[i] = e_Update;// init
				g_mWireReceiver.delayCounter[i] = 0;			// init
				g_mWireReceiver.ringingMWireState[i] = e_Clear; // init
				g_mWireReceiver.ringingMWireCounter[i] = 0;		// init
		    	break;
		    	
		    case e_TdmBusRestart:
		    case e_SlaveSync:
				// g_mWireReceiver.setMWire[i]				// keep state
				// g_mWireReceiver.clearMWire[i]			// keep state
				// g_mWireReceiver.forwardingDisabled[i]	// keep state
				// g_mWireReceiver.insertMWireFrame[i]		// keep state
				// g_mWireReceiver.mWireSamplingCommands[i] // keep state
				// g_mWireReceiver.lastSample[i]			// keep state
				// g_mWireReceiver.delayedMWireState[i]		// keep state
				// g_mWireReceiver.delayCounter[i]			// keep state
				// g_mWireReceiver.ringingMWireState[i]		// keep state
				// g_mWireReceiver.ringingMWireCounter[i]	// keep state
		    	break;
		}
		g_mWireReceiver.frameCounter[i] = 0;	
	}
	
	// service phone
	switch (initSource)
	{
	    case e_HwStartUp:
	    case e_SwRestart:
	    default:
	    	g_eWireSender.eWireServicePhone = CLEAR;		// init
	    	g_mWireReceiver.mWireServicePhone = CLEAR;		// init
	    	g_mWireReceiver.insertMWireFrame[SPH] = false;	// init
	    	g_mWireReceiver.setMWire[SPH] = false;			// init
	    	g_mWireReceiver.clearMWire[SPH] = false;		// init
	    	break;
	    	
	    case e_TdmBusRestart:
	    case e_SlaveSync:
	    	// g_eWireSender.eWireServicePhone			// keep state
	    	// g_mWireReceiver.mWireServicePhone		// keep state
	    	// g_mWireReceiver.insertMWireFrame			// keep state
	    	// g_mWireReceiver.setMWire					// keep state
	    	// g_mWireReceiver.clearMWire				// keep state
	    	break;
	}
	
	if (configuration_isServicePhoneActive())
	{
		g_eWireBuffer[0][CH1].servicePhoneOn = true;
		g_eWireBuffer[1][CH1].servicePhoneOn = true;
		
		g_mWireBuffer[0][CH1].servicePhoneOn = true;
		g_mWireBuffer[1][CH1].servicePhoneOn = true;
	}
	else
	{
		g_eWireBuffer[0][CH1].servicePhoneOn = false;
		g_eWireBuffer[1][CH1].servicePhoneOn = false;
		
		g_mWireBuffer[0][CH1].servicePhoneOn = false;
		g_mWireBuffer[1][CH1].servicePhoneOn = false;	    
	}
	
	// for testing
	//g_eWireSender.outputPinId[CH1] = e_Test_Out;
	//g_mWireReceiver.inputPinId[CH1] = e_Test_In;	
	
}


/*****************************************************************************
  Function:		binInOut_startRxMWire()
  Description: 	This function starts the sampling process on the M-wire 
  			   	input pins of all four speech interfaces. It it used to
			   	synchronise the sampling process to the codecs.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:	Call binInOut_initTxEWireAndRxMWire() before using this function.
*******************************************************************************/

void binInOut_startRxMWire(void)
{
	g_mWireReceiver.started = true;
}


/*****************************************************************************
  Function:		binInOut_startTxEWire()
  Description: 	This function starts the sending process on the E-wire
  			   	output pins of all four speech interfaces. It it used to
			   	synchronise the sending process to the codecs.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_eWireSender
  Important:	Call binInOut_initTxEWireAndRxMWire() before using this function.
*******************************************************************************/

void binInOut_startTxEWire(void)
{
	g_eWireSender.started = true;
}


/*****************************************************************************
  Function:		binInOut_startTxEWireAndRxMWire()
  Description: 	This function starts the sending and receiving process on E- 
  			   	and M-wire pins simultaneously.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_eWireSender, g_mWireReceiver
  Important:	Call binInOut_initTxEWireAndRxMWire() before using this function.
*******************************************************************************/

void binInOut_startTxEWireAndRxMWire(void)
{
    UINT32 saveInts;
	
    
    saveInts = cli();	// Global Interrupt disable
    
	g_eWireSender.started = true;
    g_mWireReceiver.started = true;
    
    sti(saveInts);		// Global Interrupt enable
}

/*****************************************************************************
  Function:		void binInOut_startMWireSampling(UINT16 chNr)
  Description: 	binInOut_startMWireSampling() starts the sampling of the M-wire
  				input pin.
  Inputs:		UINT16 chNr: channel nummber (0-3)
  Outputs:		None
  Return:       None	
  Globals:		g_mWireReceiver
  Important:	-
*******************************************************************************/

void binInOut_startMWireSampling(UINT16 chNr)
{
    UINT32 saveInts;

    saveInts = cli();	// Global Interrupt disable

	#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION
	#else
		g_mWireReceiver.mWireSamplingCommands[chNr] = e_MWireStartSampling;
	#endif

    sti(saveInts);		// Global Interrupt enable
}

/*****************************************************************************
  Function:		void binInOut_stopMWireSampling(UINT16 chNr)
  Description: 	binInOut_stopMWireSampling() stops the sampling of the M-wire
  				input pin.
  Inputs:		UINT16 chNr: channel nummber (0-3)
  Outputs:		None
  Return:       None	
  Globals:		g_mWireReceiver
  Important:	-
*******************************************************************************/

void binInOut_stopMWireSampling(UINT16 chNr)
{
    UINT32 saveInts;

    saveInts = cli();	// Global Interrupt disable

	#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION
	#else
		g_mWireReceiver.mWireSamplingCommands[chNr] = e_MWireStopSampling;
	#endif

    sti(saveInts);		// Global Interrupt enable
}

/*****************************************************************************
  Function:		binInOut_receiveMWire()
  Description: 	This function is continuously receiving M-wire samples of all
  			   	speech interfaces. The sampling rate is 1.067 kHz.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:	Call binInOut_initTxEWireAndRxMWire() before using this function.
*******************************************************************************/

void binInOut_receiveMWire(void)
{
	UINT16 i;
	UINT8 inVal;
	bool linkRecovered;
	bool transitionDetected;
	bool negEdgeDetected;
	bool posEdgeDetected;
	
	if (g_mWireReceiver.started)
	{

		// check if sampling needs to be switched on or off
		if ( g_mWireReceiver.sampleIx == 0 )
		{
			for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
			{
				if ( g_mWireReceiver.mWireSamplingCommands[i] == e_MWireStartSampling )
				{
					g_mWireReceiver.mWireSamplingCommands[i] = e_MWireNoCommand; 
					g_mWireReceiver.chEnabled[i] = true;
				}
				else if ( g_mWireReceiver.mWireSamplingCommands[i] == e_MWireStopSampling )
				{
					g_mWireReceiver.mWireSamplingCommands[i] = e_MWireNoCommand; 
					g_mWireReceiver.chEnabled[i] = false;
				}
			}
		}

		// do sampling
		for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		{
			if (g_mWireReceiver.chEnabled[i])
			{
			    inVal = binInOut_getInput(g_mWireReceiver.inputPinId[i]);
			    
			    if (g_mWireReceiver.invertMWire[i])
			    {
			    	inVal = invertSample(inVal);   
			    }
			    
				g_mWireReceiver.pChBuffer[i]->mWireBuffer[g_mWireReceiver.sampleIx] = inVal;
			}
			else
			{
				// fill buffer in case the buffer has to be resend (line interruption, SPH signalisation in CH1)
				g_mWireReceiver.pChBuffer[i]->mWireBuffer[g_mWireReceiver.sampleIx] = g_mWireReceiver.lastSample[i];
			}
		}
		
		// Test output M-wire send
#ifdef DEBUG_TELEPHONY
		binInOut_setOutput(g_pMWireBuffer->mWireBuffer[g_mWireReceiver.sampleIx], e_Buf_EW1);
#endif
	
		g_mWireReceiver.sampleIx++;
	
		// check if receive buffer is full
		if (g_mWireReceiver.sampleIx >= BUFFER_SIZE_M_WIRE_SAMPLES)
		{
		    // for testing
		    //toggleTestOutPin();
		    
		    g_mWireReceiver.sampleIx = 0;
		    
		    // check if link has recoverd
		    if (g_receiverInSync && !g_mWireReceiver.inSyncOld)
		    {
		    	linkRecovered = true;
		    }
		    else
		    {
		        linkRecovered = false;
		    }
		    
		    // update inSyncOld
		    g_mWireReceiver.inSyncOld = g_receiverInSync;
		    
		    // check if m-wire of service phone has to be set or cleared
		    if (g_mWireReceiver.setMWire[SPH])
		    {
		        // clear flag
		        g_mWireReceiver.setMWire[SPH] = false;
		        
		        g_mWireReceiver.mWireServicePhone = 1;
		        g_mWireReceiver.insertMWireFrame[CH1] = true;
		    }
		    else if (g_mWireReceiver.clearMWire[SPH])
		    {
		        // clear flag
		        g_mWireReceiver.clearMWire[SPH] = false;
		        
		        g_mWireReceiver.mWireServicePhone = 0;
		        g_mWireReceiver.insertMWireFrame[CH1] = true;
		    }

			// check if signalling frame has to be send for service phone
			if (g_mWireReceiver.insertMWireFrame[SPH] == true)
			{
				g_mWireReceiver.insertMWireFrame[SPH] = false;
				g_mWireReceiver.insertMWireFrame[CH1] = true;
			}
	    
		    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
			{
			    // check if m-wire has to be set or cleared
			    if (g_mWireReceiver.setMWire[i])
			    {
			        // clear flag
			        g_mWireReceiver.setMWire[i] = false;
			        
			        memset(g_mWireReceiver.pChBuffer[i]->mWireBuffer, 1, BUFFER_SIZE_M_WIRE_SAMPLES);
			        g_mWireReceiver.insertMWireFrame[i] = true;
			    }
			    else if (g_mWireReceiver.clearMWire[i])
			    {
			        // clear flag
			        g_mWireReceiver.clearMWire[i] = false;
			        
			        memset(g_mWireReceiver.pChBuffer[i]->mWireBuffer, 0, BUFFER_SIZE_M_WIRE_SAMPLES);
			        g_mWireReceiver.insertMWireFrame[i] = true;
			    }
			    
			    // detect transitions on input signal
			    transitionDetected = detectTransition(g_mWireReceiver.lastSample[i], g_mWireReceiver.pChBuffer[i]->mWireBuffer);
			    
			    // handle delayed M-wire (only negative edge is delayed)
			    switch ( g_mWireReceiver.delayedMWireState[i] )
			    {
			        case e_Update:
			        	
			        	if ( detectNegativeEdge(g_mWireReceiver.lastSample[i], g_mWireReceiver.pChBuffer[i]->mWireBuffer) )
			        	{
			        	    g_mWireReceiver.delayCounter[i] = 0;
			        	    g_mWireReceiver.delayedMWireState[i] = e_Delay;
			        	}
			        	break;
			        
			        case e_Delay:
			        
			        	if ( detectNegativeEdge(g_mWireReceiver.lastSample[i], g_mWireReceiver.pChBuffer[i]->mWireBuffer) )
			        	{
			        	    g_mWireReceiver.delayCounter[i] = 0;
			        	}
			        	else if ( g_mWireReceiver.delayCounter[i] >= M_WIRE_DELAY )
			        	{
			        	    g_mWireReceiver.delayedMWireState[i] = e_Update;
			        	}
			        	else
			        	{
			        		g_mWireReceiver.delayCounter[i]++;
			        	}
			        	break;
			        	
			        default:
			        	assert(false);
			        	g_mWireReceiver.delayedMWireState[i] = e_Update;
			    }
			    
				// handle ringing M-wire
				switch ( g_mWireReceiver.ringingMWireState[i] )
				{
					case e_Clear:

						posEdgeDetected = !LAST_SAMPLE_LAST_M_WIRE_BUFFER(i) && LAST_SAMPLE_CURRENT_M_WIRE_BUFFER(i);

						if ( posEdgeDetected )
						{
							g_mWireReceiver.ringingMWireCounter[i] = 0;
							g_mWireReceiver.ringingMWireState[  i] = e_RingTypeDetection;
						}
						break;

					case e_RingTypeDetection:

						negEdgeDetected = LAST_SAMPLE_LAST_M_WIRE_BUFFER(i) && !LAST_SAMPLE_CURRENT_M_WIRE_BUFFER(i);

						if (
							 negEdgeDetected &&
							 g_mWireReceiver.ringingMWireCounter[i] < E_WIRE_PULSE_TONE_MIN
						   )
						{
							g_mWireReceiver.ringingMWireState[i] = e_Clear;
						}
						else if ( 
								  negEdgeDetected &&
								  g_mWireReceiver.ringingMWireCounter[i] <= E_WIRE_PULSE_TONE_MAX
								)
						{
							g_mWireReceiver.ringingMWireCounter[i] = 0;
							g_mWireReceiver.ringingMWireState[  i] = e_SetRinging;
						}
						else if ( g_mWireReceiver.ringingMWireCounter[i] > E_WIRE_PULSE_TONE_MAX )
						{
							g_mWireReceiver.ringingMWireState[i] = e_SetContinuous;
						}
						else
						{
							g_mWireReceiver.ringingMWireCounter[i]++;
						}

						break;

					case e_SetContinuous:

						negEdgeDetected = LAST_SAMPLE_LAST_M_WIRE_BUFFER(i) && !LAST_SAMPLE_CURRENT_M_WIRE_BUFFER(i);

						if ( negEdgeDetected )
						{
							g_mWireReceiver.ringingMWireState[i] = e_Clear;
						}

						break;

					case e_SetRinging:

						posEdgeDetected = !LAST_SAMPLE_LAST_M_WIRE_BUFFER(i) && LAST_SAMPLE_CURRENT_M_WIRE_BUFFER(i);
						
						if ( posEdgeDetected )
						{
							g_mWireReceiver.ringingMWireCounter[i] = 0;
							g_mWireReceiver.ringingMWireState[  i] = e_SetWait;
						}
						else if ( g_mWireReceiver.ringingMWireCounter[i] > E_WIRE_SILENCE_MAX )
						{
							g_mWireReceiver.ringingMWireState[i] = e_Clear;
						}
						else
						{
							g_mWireReceiver.ringingMWireCounter[i]++;
						}

						break;

					case e_SetWait:

						negEdgeDetected = LAST_SAMPLE_LAST_M_WIRE_BUFFER(i) && !LAST_SAMPLE_CURRENT_M_WIRE_BUFFER(i);

						if ( negEdgeDetected )
						{
							g_mWireReceiver.ringingMWireCounter[i] = 0;
							g_mWireReceiver.ringingMWireState[  i] = e_SetRinging;
						}
						else if ( g_mWireReceiver.ringingMWireCounter[i] > E_WIRE_PULSE_TONE_MAX )
						{
							g_mWireReceiver.ringingMWireState[i] = e_SetContinuous;
						}
						else
						{
							g_mWireReceiver.ringingMWireCounter[i]++;
						}

						break;

					default:
						assert(false);
						g_mWireReceiver.ringingMWireState[i] = e_Clear;
				}

			    // count frames since last inserted m-wire frame
			    g_mWireReceiver.frameCounter[i]++;	
			    	
			    // check if m-wire frame has to be sent
			    if ((transitionDetected || g_mWireReceiver.insertMWireFrame[i] || linkRecovered 
			    	 || (g_mWireReceiver.frameCounter[i] >= INSERT_MWIRE_FRAME))
			        && !g_mWireReceiver.forwardingDisabled[i])
			    {
			    	g_mWireReceiver.pChBuffer[i]->insertMWireFrame = true;
			    	g_mWireReceiver.pChBuffer[i]->mWireBufferServicePhone = g_mWireReceiver.mWireServicePhone;	// only needed in ch1
			    	g_mWireReceiver.frameCounter[i] = 0;
			    	g_mWireReceiver.insertMWireFrame[i] = false;
			    }
			    else
			    {
			    	g_mWireReceiver.pChBuffer[i]->insertMWireFrame = false;
			    }
				
			    // save last m-wire sample
			    g_mWireReceiver.lastSample[i] = g_mWireReceiver.pChBuffer[i]->mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES-1];
			}   
	    
		    swapMWireBuffer();		    
		}
	}
}


/*****************************************************************************
  Function:		binInOut_sendEWire()
  Description: 	This function is continuously sending E-wire samples to all
  			   	speech interfaces. The repetition rate is 1.067 kHz.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_eWireSender
  Important:	Call binInOut_initTxEWireAndRxMWire() before using this function.
*******************************************************************************/

void binInOut_sendEWire(void)
{
	UINT16 i;
	UINT8 outVal;
	bool negEdgeDetected;
	bool posEdgeDetected;
	
	if (g_eWireSender.started)
	{
		for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		{
		    if (g_eWireSender.chEnabled[i])
		    {
	    		if (g_eWireSender.pChBuffer[i]->mWireDataReceived)
	    		{
	    			outVal = g_eWireSender.pChBuffer[i]->mWireBuffer[g_eWireSender.sampleIx];
	    		}
	    		else
	    		{
	    			outVal = g_eWireSender.lastValidSample[i];
	    		}
	    		
	    		if (g_eWireSender.invertEWire[i])
	    		{
	    			outVal = invertSample(outVal);
	    		}   
	    		
	    		// set output pin 
	    		binInOut_setOutput(outVal, g_eWireSender.outputPinId[i]);
		    }
		    else
		    {
		        // no output signal
		    }
		}
		
#ifdef DEBUG_TELEPHONY
		// Test output received E-wire
		if (g_eWireSender.pChBuffer[CH1]->mWireDataReceived)
		{
			outVal = g_eWireSender.pChBuffer[CH1]->mWireBuffer[g_eWireSender.sampleIx];
		}
		else
		{
			outVal = g_eWireSender.lastValidSample[CH1];
		}
		binInOut_setOutput(outVal, e_Buf_EW3);
#endif
		
		g_eWireSender.sampleIx++;
		
		// check if buffer is sent
		if (g_eWireSender.sampleIx >= BUFFER_SIZE_M_WIRE_SAMPLES)
		{
		    // for testing
		    //toggleTestOutPin();
		    
		    g_eWireSender.sampleIx = 0;
		    
		    if (g_eWireSender.pChBuffer[CH1]->mWireDataReceived)
    		{
    		    // update e-wire state of service phone
    			g_eWireSender.eWireServicePhone = g_eWireSender.pChBuffer[CH1]->mWireBufferServicePhone;
    			
    			// do not clear flag
    			
    			// for testing
    			/*if (g_eWireSender.eWireServicePhone)
    				binInOut_setOutput(1, e_Test_Out);
    			else
    				binInOut_setOutput(0, e_Test_Out); */
    		}
	    
		    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		    {
		        // Handle delayed E-wire (only negative edge is delayed)
	    		switch ( g_eWireSender.delayedEWireState[i] )
	    		{
	    		    case e_Update:

	    		    	if (
	    		    		 g_eWireSender.pChBuffer[i]->mWireDataReceived &&
	    		    		 detectNegativeEdge(g_eWireSender.lastValidSample[i], g_eWireSender.pChBuffer[i]->mWireBuffer)
	    		    	   )
	    		    	{
	    		    	    g_eWireSender.delayCounter[i] = 0;
	    		    	    g_eWireSender.delayedEWireState[i] = e_Delay;
	    		    	}
	    		    	break;
	    		    	
	    		    case e_Delay:

	    		    	if (
	    		    		 g_eWireSender.pChBuffer[i]->mWireDataReceived &&
	    		    		 detectNegativeEdge(g_eWireSender.lastValidSample[i], g_eWireSender.pChBuffer[i]->mWireBuffer)
	    		    	   )
	    		    	{
	    		    	    g_eWireSender.delayCounter[i] = 0;
	    		    	}
	    		    	else if (g_eWireSender.delayCounter[i] >= E_WIRE_DELAY)
	    		    	{
	    		    	    g_eWireSender.delayedEWireState[i] = e_Update;
	    		    	}
	    		    	else
	    		    	{
	    		    	    g_eWireSender.delayCounter[i]++;
	    		    	}
	    		    	break;
	    		    	
	    		    default:
	    		    	assert(false);
	    		    	g_eWireSender.delayedEWireState[i] = e_Update;
	    		}
	    		
				// Handle ringing E-wire
				switch ( g_eWireSender.ringingEWireState[i] )
				{
					case e_Clear:

						posEdgeDetected = !LAST_SAMPLE_LAST_E_WIRE_BUFFER(i) && LAST_SAMPLE_CURRENT_E_WIRE_BUFFER(i);

						if ( posEdgeDetected )
						{
							g_eWireSender.ringingEWireCounter[i] = 0;
							g_eWireSender.ringingEWireState[  i] = e_RingTypeDetection;
						}
						break;

					case e_RingTypeDetection:

						negEdgeDetected = LAST_SAMPLE_LAST_E_WIRE_BUFFER(i) && !LAST_SAMPLE_CURRENT_E_WIRE_BUFFER(i);

						if (
							 negEdgeDetected &&
							 g_eWireSender.ringingEWireCounter[i] < E_WIRE_PULSE_TONE_MIN
						   )
						{
							g_eWireSender.ringingEWireState[i] = e_Clear;
						}
						else if ( 
								  negEdgeDetected &&
								  g_eWireSender.ringingEWireCounter[i] <= E_WIRE_PULSE_TONE_MAX
								)
						{
							g_eWireSender.ringingEWireCounter[i] = 0;
							g_eWireSender.ringingEWireState[  i] = e_SetRinging;
						}
						else if ( g_eWireSender.ringingEWireCounter[i] > E_WIRE_PULSE_TONE_MAX )
						{
							g_eWireSender.ringingEWireState[i] = e_SetContinuous;
						}
						else
						{
							g_eWireSender.ringingEWireCounter[i]++;
						}

						break;

					case e_SetContinuous:

						negEdgeDetected = LAST_SAMPLE_LAST_E_WIRE_BUFFER(i) && !LAST_SAMPLE_CURRENT_E_WIRE_BUFFER(i);

						if ( negEdgeDetected )
						{
							g_eWireSender.ringingEWireState[i] = e_Clear;
						}

						break;

					case e_SetRinging:

						posEdgeDetected = !LAST_SAMPLE_LAST_E_WIRE_BUFFER(i) && LAST_SAMPLE_CURRENT_E_WIRE_BUFFER(i);
						
						if ( posEdgeDetected )
						{
							g_eWireSender.ringingEWireCounter[i] = 0;
							g_eWireSender.ringingEWireState[  i] = e_SetWait;
						}
						else if ( g_eWireSender.ringingEWireCounter[i] > E_WIRE_SILENCE_MAX )
						{
							g_eWireSender.ringingEWireState[i] = e_Clear;
						}
						else
						{
							g_eWireSender.ringingEWireCounter[i]++;
						}

						break;

					case e_SetWait:

						negEdgeDetected = LAST_SAMPLE_LAST_E_WIRE_BUFFER(i) && !LAST_SAMPLE_CURRENT_E_WIRE_BUFFER(i);

						if ( negEdgeDetected )
						{
							g_eWireSender.ringingEWireCounter[i] = 0;
							g_eWireSender.ringingEWireState[  i] = e_SetRinging;
						}
						else if ( g_eWireSender.ringingEWireCounter[i] > E_WIRE_PULSE_TONE_MAX )
						{
							g_eWireSender.ringingEWireState[i] = e_SetContinuous;
						}
						else
						{
							g_eWireSender.ringingEWireCounter[i]++;
						}

						break;

					default:
						assert(false);
						g_eWireSender.ringingEWireState[i] = e_Clear;
				}
	    		
				// Update last valid sample if new E-wire data was received.
		        if (g_eWireSender.pChBuffer[i]->mWireDataReceived)
	    			g_eWireSender.lastValidSample[i] = g_eWireSender.pChBuffer[i]->mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES-1];
	    		
	    		// clear flag
	    		g_eWireSender.pChBuffer[i]->mWireDataReceived = false;
		    }
		    
		    swapEWireBuffer();
		    
		    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		    {
		        // check if e-wire has to be set or cleared
		        if (g_eWireSender.setEWire[i])
	    		{
	    		    // clear flag
	    		    g_eWireSender.setEWire[i] = false;
	    		    
	    		    memset(g_eWireSender.pChBuffer[i]->mWireBuffer, 1, BUFFER_SIZE_M_WIRE_SAMPLES);
	    		    g_eWireSender.pChBuffer[i]->mWireDataReceived = true;
	    		}
	    		else if (g_eWireSender.clearEWire[i])
	    		{
	    		    // clear flag
	    		    g_eWireSender.clearEWire[i] = false;
	    		    
	    		    memset(g_eWireSender.pChBuffer[i]->mWireBuffer, 0, BUFFER_SIZE_M_WIRE_SAMPLES);
	    		    g_eWireSender.pChBuffer[i]->mWireDataReceived = true;
	    		}
	    		else if (g_eWireSender.pChBuffer[i]->mWireDataReceived)
	    		{
	    		    switch(g_eWireSender.pChBuffer[i]->freeze)
	    		    {
	    		        case 1:		// first subframe is not correct
	    		        	
	    		        	// replace samples of first subframe by last valid sample
	    		        	memset(g_eWireSender.pChBuffer[i]->mWireBuffer, g_eWireSender.lastValidSample[i], SUBFRAME1_LEN);
	    		        	break;
	    		        	
	    		        case 2:		// first and second subframe are not correct
	    		        	
	    		        	// replace samples of first and second subframe by last valid sample
	    		        	memset(g_eWireSender.pChBuffer[i]->mWireBuffer, g_eWireSender.lastValidSample[i], SUBFRAME12_LEN);
	    		        	break;
	    		        	
	    		        case 3:		// all subframes are not correct
	    		        
	    		        	// do not output received frame (last valid sample is used)
	    		        	g_eWireSender.pChBuffer[i]->mWireDataReceived = false;
	    		        	break;
	    		    }
	    		}
		    }
		}	        
	}
}


/*****************************************************************************
  Function: 	UINT8 binInOut_getEWire(UINT16 chNr)
  Description: 	Returns the e-wire state of the specified channel
  Inputs:		chNr: channel number (0-3, 4: service phone)	
  Outputs:
  Return: 		e-wire state      	
  Globals:		g_eWireSender, g_pEWireBuffer
  Important:
*******************************************************************************/

UINT8 binInOut_getEWire(UINT16 chNr)
{
    if (chNr == SPH)
    {
        return(g_eWireSender.eWireServicePhone);
    }
    else if (g_pEWireBuffer[chNr].mWireDataReceived)
    {
		return(g_pEWireBuffer[chNr].mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES-1]);
    }
    else
    {
        return(g_eWireSender.lastValidSample[chNr]);
    }
}


/*****************************************************************************
  Function:		binInOut_getEWireDelayed()
  Description:	Returns the delayed e-wire state of the specified channel.
  				Negative edges are delayed by approx. 200ms.
  Inputs:		chNr: channel number (0-3)
  Outputs:		-
  Return:       UINT8: delayed e-wire	
  Globals:		g_eWireSender
  Important:	-
*******************************************************************************/

UINT8 binInOut_getEWireDelayed(UINT16 chNr)
{
    assert(chNr < N_CHANNELS_ON_BOARD);
    
    if ( g_eWireSender.delayedEWireState[chNr] == e_Update )
    	return binInOut_getEWire(chNr);
    else
    	return true;
}

/*****************************************************************************
  Function:		UINT8 binInOut_isEWireRinging(UINT16 chNr)
  Description:	binInOut_isEWireRinging() returns the state of the ringing
  				E-wire detection.
  Inputs:		chNr: channel number (0-3)
  Outputs:		-
  Return:       UINT8: true, if ringing E-wire was detected
					   false, otherwise
  Globals:		g_eWireSender.ringingEWireState
  Important:	-
*******************************************************************************/
UINT8 binInOut_isEWireRinging(UINT16 chNr)
{
	assert(chNr < N_CHANNELS_ON_BOARD);

    switch ( g_eWireSender.ringingEWireState[chNr] )
    {
		case e_Clear:
			return false;

		case e_RingTypeDetection:
		case e_SetContinuous:
		case e_SetRinging:
		case e_SetWait:
			return true;

		default:
			assert(false);
			return false;
	}
}

/*****************************************************************************
  Function:		void binInOut_clearRingingEWireDetector(UINT16 chNr)
  Description:	binInOut_clearRingingEWireDetector() clears the state of the
  				ringing E-wire detector.
  Inputs:		chNr: channel number (0-3)
  Outputs:		-
  Return:       -
  Globals:		g_eWireSender.ringingEWireState
  Important:	-
*******************************************************************************/
void binInOut_clearRingingEWireDetector(UINT16 chNr)
{
    UINT32 saveInts;

	assert(chNr < N_CHANNELS_ON_BOARD);

    saveInts = cli();	// Global Interrupt disable

    g_eWireSender.ringingEWireState[chNr] = e_Clear;

    sti(saveInts);		// Global Interrupt enable
}

/*****************************************************************************
  Function:		binInOut_setEWire()
  Description:	Sets the e-wire signal of the specified channel (1-4) to 1.
  Inputs:		chNr: channel number (0-3)
  Outputs:
  Return:       	
  Globals:		g_eWireSender
  Important:
*******************************************************************************/

void binInOut_setEWire(UINT16 chNr)
{
    g_eWireSender.setEWire[chNr] = true;
}


/*****************************************************************************
  Function:		binInOut_clearEWire()
  Description:	Sets the e-wire signal of the specified channel (1-4) to 0.
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_eWireSender
  Important:
*******************************************************************************/

void binInOut_clearEWire(UINT16 chNr)
{
    if (chNr == SPH)
    {
        g_eWireSender.eWireServicePhone = 0;
    }
    else
    {
    	g_eWireSender.clearEWire[chNr] = true;
    }
}


/*****************************************************************************
  Function:		binInOut_getMWire()
  Description:	Returns the m-wire state of the specified channel.	
  Inputs:		chNr: channel number (0-3)
  Outputs:
  Return:       	
  Globals:		g_pMWireBuffer
  Important:
*******************************************************************************/

UINT8 binInOut_getMWire(UINT16 chNr)
{
    return(g_pMWireBuffer[chNr].mWireBuffer[BUFFER_SIZE_M_WIRE_SAMPLES-1]);
}


/*****************************************************************************
  Function:		binInOut_getMWireDelayed()
  Description:	Returns the delayed m-wire state of the specified channel.
  				Negative edges are delayed by approx. 200ms.
  Inputs:		chNr: channel number (0-3)
  Outputs:		-
  Return:       UINT8: delayed m-wire	
  Globals:		g_mWireReceiver
  Important:
*******************************************************************************/

UINT8 binInOut_getMWireDelayed(UINT16 chNr)
{
    assert( chNr < N_CHANNELS_ON_BOARD );
    
    if ( g_mWireReceiver.delayedMWireState[chNr] == e_Update )
    	return binInOut_getMWire(chNr);
    else
    	return true;
}

/*****************************************************************************
  Function:		UINT8 binInOut_isMWireRinging(UINT16 chNr)
  Description:	binInOut_isMWireRinging() returns the state of the ringing
  				M-wire detection.
  Inputs:		chNr: channel number (0-3)
  Outputs:		-
  Return:       UINT8: true, if ringing M-wire was detected
					   false, otherwise
  Globals:		g_mWireReceiver.ringingMWireState
  Important:	-
*******************************************************************************/
UINT8 binInOut_isMWireRinging(UINT16 chNr)
{
	assert(chNr < N_CHANNELS_ON_BOARD);

    switch ( g_mWireReceiver.ringingMWireState[chNr] )
    {
		case e_Clear:
			return false;

		case e_RingTypeDetection:
		case e_SetContinuous:
		case e_SetRinging:
		case e_SetWait:
			return true;

		default:
			assert(false);
			return false;
	}
}

/*****************************************************************************
  Function:		void binInOut_clearRingingMWireDetector(UINT16 chNr)
  Description:	binInOut_clearRingingMWireDetector() clears the state of the
  				ringing M-wire detector.
  Inputs:		chNr: channel number (0-3)
  Outputs:		-
  Return:       -
  Globals:		g_mWireReceiver.ringingMWireState
  Important:	-
*******************************************************************************/
void binInOut_clearRingingMWireDetector(UINT16 chNr)
{
    UINT32 saveInts;

	assert(chNr < N_CHANNELS_ON_BOARD);

    saveInts = cli();	// Global Interrupt disable

    g_mWireReceiver.ringingMWireState[chNr] = e_Clear;

    sti(saveInts);		// Global Interrupt enable
}

/*****************************************************************************
  Function:		binInOut_setMWire()
  Description: 	Sets the m-wire signal of the specified channel (0-3) to 1.
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:
*******************************************************************************/

void binInOut_setMWire(UINT16 chNr)
{
    g_mWireReceiver.setMWire[chNr] = true;
}


/*****************************************************************************
  Function: 	binInOut_clearMWire(UINT16 chNr)
  Description: 	Sets the m-wire signal of the specified channel.
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:
*******************************************************************************/

void binInOut_clearMWire(UINT16 chNr)
{
    g_mWireReceiver.clearMWire[chNr] = true;
}

/*****************************************************************************
  Function: 	extern void binInOut_insertMWireFrame(UINT16 chNr)
  Description: 	binInOut_insertMWireFrame() inserts an M-wire signalling frame.
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:
*******************************************************************************/

extern void binInOut_insertMWireFrame(UINT16 chNr)
{
	g_mWireReceiver.insertMWireFrame[chNr] = true;
}

/*****************************************************************************
  Function: 	binInOut_enableMWireForwarding()
  Description:	Enables forwarding of m-wire signal over the link to the remote
  				equipment.
  Inputs:		chNr: channel number (0-3)
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:
*******************************************************************************/

void binInOut_enableMWireForwarding(UINT16 chNr)
{
	g_mWireReceiver.forwardingDisabled[chNr] = false;
	g_mWireReceiver.insertMWireFrame[chNr] = true;
}


/*****************************************************************************
  Function: 	binInOut_disableMWireForwarding()
  Description:	Disables forwarding of m-wire signal over the link to the remote
  				equipment.
  Inputs:		chNr: channel number (0-3)
  Outputs:
  Return:       	
  Globals:		g_mWireReceiver
  Important:
*******************************************************************************/

void binInOut_disableMWireForwarding(UINT16 chNr)
{
    g_mWireReceiver.forwardingDisabled[chNr] = true;
}


/*****************************************************************************
  Function: 	binInOut_initRingCurrentDetector()
  Description:	Initialises ring current detector data.
  Inputs:		
  Outputs:
  Return:       	
  Globals:		g_rcd		
  Important:	Call this function once before using binInOut_detectRingCurrent().
*******************************************************************************/

void binInOut_initRingCurrentDetector(void)
{
    UINT16 i;
    
    
    // init input pin ID
    g_rcd[CH1].inputPinId = e_Ring_Detect1;
	g_rcd[CH2].inputPinId = e_Ring_Detect2;
	g_rcd[CH3].inputPinId = e_Ring_Detect3;
	g_rcd[CH4].inputPinId = e_Ring_Detect4;
    
    // init ring current detector data struct
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
	{
	    g_rcd[i].state = e_RcdIdle;
    	g_rcd[i].lastSample = 0;
    	g_rcd[i].timer = 0;
    	g_rcd[i].transitionCounter = 0;
	}
}


/*****************************************************************************
  Function: 	binInOut_detectRingCurrent()
  Description:	Monitors the ring detect input of all four speech interfaces
  				and detects the ring current.
  Inputs:	
  Outputs:
  Return:       	
  Globals:		g_rcd		
  Important:
*******************************************************************************/

void binInOut_detectRingCurrent(void)
{
    UINT16 i;
    UINT8 newSample;
    
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
	{
		// get new sample
		newSample = binInOut_getInput(g_rcd[i].inputPinId);

		switch(g_rcd[i].state)
		{
		    case e_RcdIdle:
		     	
		    	if (newSample != g_rcd[i].lastSample)
		    	{
		    		g_rcd[i].transitionCounter++;
		    		g_rcd[i].timer = 0;
		    		g_rcd[i].state = e_DetectingRc;
		    	}
		    		 
		    	break;
		    	
		    case e_DetectingRc:
		    
		    	g_rcd[i].timer++;

		    	if (newSample != g_rcd[i].lastSample)
		    	{
		    		g_rcd[i].transitionCounter++;
		    	}
		    	
		    	if (g_rcd[i].transitionCounter >= RCD_N_TRANSITIONS_MIN)
		    	{
		    	    g_rcd[i].transitionCounter = 0;
		    		g_rcd[i].timer = 0;
		    		g_rcd[i].state = e_RcDetected;
		    	}
		    	else if (g_rcd[i].timer >= RCD_DETECTION_TIME)
		    	{
		    		g_rcd[i].transitionCounter = 0;
		    		g_rcd[i].timer = 0;
		    		g_rcd[i].state = e_RcdIdle;
				}
		    	
		    	break;
		    	
		    case e_RcDetected:
		    
		    	g_rcd[i].timer++;
		    	
		    	if (newSample != g_rcd[i].lastSample)
		    	{
		    		g_rcd[i].transitionCounter++;
		    	}
		    	
		    	if (g_rcd[i].transitionCounter >= RCD_N_TRANSITIONS_MIN)
		    	{
		    	    g_rcd[i].transitionCounter = 0;
		    		g_rcd[i].timer = 0;
		    	}
		    	else if (g_rcd[i].timer >= RCD_HOLD_TIME)
		    	{
		    		g_rcd[i].transitionCounter = 0;
		    		g_rcd[i].timer = 0;
		    		g_rcd[i].state = e_RcdIdle;
				}
		    	
		    	break;
		    	
		    default:
		    
		    	g_rcd[i].state = e_RcdIdle;
		}
	
		g_rcd[i].lastSample = newSample;	
		
	}	
}


/*****************************************************************************
  Function: 	binInOut_isRingCurrentOn()
  Description:	Returns the state of the ring current of the specified channel.
  Inputs:		chNr: channel number (0-3)	
  Outputs:
  Return:		true if ring current on, else false    	
  Globals:		g_rcd		
  Important:
*******************************************************************************/

bool binInOut_isRingCurrentOn(UINT16 chNr)
{
    if (g_rcd[chNr].state == e_RcDetected)
    {
    	return(true);
    }
    else
    {
        return(false);
    }
}

/*****************************************************************************
  Function: 	void binInOut_clearRingDetector(UINT16 chNr)
  Description:	binInOut_clearRingDetector() clears the ring detector for the
  				specified channel.
  Inputs:		chNr: channel number (0-3)	
  Outputs:		-
  Return:		-
  Globals:		g_rcd		
  Important:
*******************************************************************************/

void binInOut_clearRingDetector(UINT16 chNr)
{
    UINT32 saveInts;
      
    assert(chNr < N_CHANNELS_ON_BOARD);
    
    saveInts = cli();	// Global Interrupt disable
    
    g_rcd[chNr].transitionCounter = 0;
    g_rcd[chNr].timer = 0;
    g_rcd[chNr].state = e_RcdIdle;
    
    sti(saveInts);		// Global Interrupt enable
}

/*****************************************************************************
  Function:		binInOut_setOutput()
  Description: 	Sets the specified output pin to 0 or 1.
  Inputs:		val: output value 
  				pinId: pin identification
  Outputs:
  Return:       	
  Globals:
  Important:
*******************************************************************************/

void binInOut_setOutput(UINT8 val, e_OutPinId pinId)
{
  	if(val == 0)
  	{
  	    switch(pinId)
  	    {
  	      	case e_Buf_EW1: 	

				*pPORTDIO_CLEAR = PF8;
				break;	
				
			case e_Buf_EW2: 	

				*pPORTDIO_CLEAR = PF9;
				break;	
				
			case e_Buf_EW3: 	

				*pPORTDIO_CLEAR = PF10;
				break;	
				
			case e_Buf_EW4: 	

				*pPORTDIO_CLEAR = PF11;
				break;
				
			case e_Hook_Switch1:
			
				*pPORTDIO_CLEAR = PF4;
				break;
			
			case e_Hook_Switch2:
			
				*pPORTDIO_CLEAR = PF5;
				break;
				
			case e_Hook_Switch3:
			
				*pPORTDIO_CLEAR = PF6;
				break;
				
			case e_Hook_Switch4:
			
				*pPORTDIO_CLEAR = PF7;
				break;
	
			case e_Buzzer_P: 	

				*pPORTCIO_CLEAR = PF0;
				break;	
				
			case e_Buzzer_N: 	

#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION 

				*pPORTCIO_CLEAR = PF1;
#else
				*pPORTCIO_CLEAR = PF9;			
#endif
				break;						
			
			case e_Alarm_Led: 	

#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION 

				*pPORTCIO_CLEAR = PF4;

#else
				*pUART0_LCR |= PF6;			
#endif
				break;
				
			case e_Warning_Led: 	

				*pPORTCIO_CLEAR = PF5;
				break;	
				
			case e_Ready_Led: 	

				*pPORTCIO_CLEAR = PF6;
				break;	
				
			case e_Sp_Enabled_Led: 	

				*pPORTCIO_CLEAR = PF7;
				break;	
				
			case e_Sp_Free_Led: 	

				*pPORTCIO_CLEAR = PF8;
				break;
				
			case e_Tdm_Input_En:
			
				*pPORTEIO_CLEAR = PF9;			
				break;
				
			case e_Test_Out: 	

#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION 
				*pUART0_LCR |= PF6;
#else
				*pPORTFIO_CLEAR = PF0;
#endif
				break;
												
			default: 	
	
				break;	  
  	    }
  	}
  	
  	else
  	{
  	 	switch(pinId)
  	    {
  	      	case e_Buf_EW1: 	

				*pPORTDIO_SET = PF8;
				break;	
				
			case e_Buf_EW2: 	

				*pPORTDIO_SET = PF9;
				break;	
				
			case e_Buf_EW3: 	

				*pPORTDIO_SET = PF10;
				break;	
				
			case e_Buf_EW4: 	

				*pPORTDIO_SET = PF11;
				break;
				
			case e_Hook_Switch1:
			
				*pPORTDIO_SET = PF4;
				break;
			
			case e_Hook_Switch2:
			
				*pPORTDIO_SET = PF5;
				break;
				
			case e_Hook_Switch3:
			
				*pPORTDIO_SET = PF6;
				break;
				
			case e_Hook_Switch4:
			
				*pPORTDIO_SET = PF7;
				break;
				
			case e_Buzzer_P: 	

				*pPORTCIO_SET = PF0;
				break;	
				
			case e_Buzzer_N: 	

#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION 

				*pPORTCIO_SET = PF1;
#else
				*pPORTCIO_SET = PF9;			
#endif
				break;						
			
			case e_Alarm_Led: 	

#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION 

				*pPORTCIO_SET = PF4;

#else
				*pUART0_LCR &= ~PF6;		
#endif
				break;
				
			case e_Warning_Led: 	

				*pPORTCIO_SET = PF5;
				break;	
				
			case e_Ready_Led: 	

				*pPORTCIO_SET = PF6;
				break;	
				
			case e_Sp_Enabled_Led: 	

				*pPORTCIO_SET = PF7;
				break;	
				
			case e_Sp_Free_Led: 	

				*pPORTCIO_SET = PF8;
				break;
				
			case e_Tdm_Input_En:
			
				*pPORTEIO_SET = PF9;			
				break;	
				
			case e_Test_Out: 
				
#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION 
				*pUART0_LCR &= ~PF6;
#else
				*pPORTFIO_SET = PF0;
#endif
				break;
												
			default: 	
						
				break;	  
  	    }	   
  	}  
}


/*****************************************************************************
  Function:		binInOut_getInput()
  Description: 	Returns the state of the specified input pin (0 or 1).
  Inputs:		pinId: pin identification
  Outputs:
  Return:		input state (0 or 1)       	
  Globals:
  Important:
*******************************************************************************/

UINT8 binInOut_getInput(e_InPinId pinId)
{
    UINT16 tempValue = 0;
    
    switch(pinId)
    {
  	    case e_Pulse_Code_Dialing1: 	

			tempValue = *pPORTDIO & PF12;
			break;	
			
  	    case e_Pulse_Code_Dialing2: 	

			tempValue = *pPORTDIO & PF13;
			break;	
			
		case e_Pulse_Code_Dialing3: 	

			tempValue = *pPORTFIO & PF15;
			break;	
			
		case e_Pulse_Code_Dialing4: 	

			tempValue = *pPORTEIO & PF15;
			break;			
				
		case e_Ring_Detect1: 	

			tempValue = *pPORTFIO & PF7;
			break;
			
		case e_Ring_Detect2: 	

			tempValue = *pPORTFIO & PF8;
			break;
			
		case e_Ring_Detect3: 	

			tempValue = *pPORTFIO & PF9;
			break;
			
		case e_Ring_Detect4: 	

			tempValue = *pPORTFIO & PF10;
			break;			
			
		case e_MW1: 	

			tempValue = *pPORTFIO & PF11;
			break;
			
		case e_MW2: 	

			tempValue = *pPORTFIO & PF12;
			break;
			
		case e_MW3: 	

			tempValue = *pPORTFIO & PF13;
			break;
			
		case e_MW4: 	

			tempValue = *pPORTFIO & PF14;
			break;	

#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION			
		case e_Test_In: 	

			tempValue = *pPORTFIO & PF0;
			break;
#endif	
							
		default: 	
						
			break;	  
  	}
  	
  	// Return 0 or 1
  	if(tempValue == 0)
  	{
		return 0;
  	}
  	else
  	{
  	    return 1;
  	}    
  	
}

/*****************************************************************************
  Function:		binInOut_buzzerOn()
  Description: 	Turns the buzzer on.
  Inputs:		
  Outputs:
  Return:		  	
  Globals:		g_buzzer
  Important:
*******************************************************************************/

void binInOut_buzzerOn(void)
{
    // Initialisation 
    g_buzzer.counterTone = 0; 
 	g_buzzer.counterRing = BUZZER_RING_PERIOD;
 	g_buzzer.impulsLength = 0;
 	g_buzzer.counterImpuls = 0; 
 	g_buzzer.polarity = 0; 
 	g_buzzer.numberOfRings = 0;
 	g_buzzer.buzzerRing = false;
 	
	// Set buzzer on
 	g_buzzer.buzzerOn = true;	

}

/*****************************************************************************
  Function:		binInOut_buzzerOff()
  Description: 	Turns the buzzer off.
  Inputs:		
  Outputs:
  Return:		  	
  Globals:		g_buzzer
  Important:
*******************************************************************************/

void binInOut_buzzerOff(void)
{
    // Set buzzer off
 	g_buzzer.buzzerOn = false;	   
}

/*****************************************************************************
  Function:		binInOut_buzzerRing()
  Description: 	Makes the buzzer ring for certain times.
  Inputs:		numberOfRings: How many times the buzzer should ring.
  Outputs:
  Return:		  	
  Globals:		g_buzzer
  Important:
*******************************************************************************/

void binInOut_buzzerRing(UINT16 numberOfRings)
{
    // Initialisation 
    g_buzzer.counterTone = 0; 
 	g_buzzer.counterRing = BUZZER_RING_PERIOD;
 	g_buzzer.impulsLength = 0;
 	g_buzzer.counterImpuls = 0; 
 	g_buzzer.polarity = 0; 
 	g_buzzer.buzzerOn = false; 
 	
 	// Set the number of rings
    g_buzzer.numberOfRings = numberOfRings;
    g_buzzer.buzzerRing = true;
}

/*****************************************************************************
  Function:		binInOut_sampleToBuzzer()
  Description: 	Creates the correct waveform for the buzzer. Switches the 
  				polarity with the right frequency.
  Inputs:		 
  Outputs:
  Return:		  	
  Globals:		g_buzzer
  Important:	Has to be called periodically with 10.7khz
*******************************************************************************/

void binInOut_sampleToBuzzer(void)
{
    // If buzzerRing is true turn buzzer on
    if(g_buzzer.buzzerRing)
    {
        if(g_buzzer.numberOfRings > 0)
        {
            g_buzzer.buzzerOn = true; 
        }
        else
        {
      		g_buzzer.buzzerOn = false; 
            g_buzzer.buzzerRing = false;
        } 
    }
    
    if(g_buzzer.buzzerOn)
    {
		// Ring for half of the period
		if(g_buzzer.counterRing >= BUZZER_RING_PERIOD / 2)
		{
			// Check if tone frequency should change
			if (g_buzzer.counterTone == 0)
			{
				g_buzzer.counterTone = BUZZER_TONE_LENGTH;
			
				switch (g_buzzer.impulsLength)
				{
					case 6:
						g_buzzer.impulsLength = 3;
						break;
					case 5:
						g_buzzer.impulsLength = 4;
						break;
					case 4:
						g_buzzer.impulsLength = 2;
						break;
					case 3:
						g_buzzer.impulsLength = 5;
						break;
					default:
						g_buzzer.impulsLength = 6;
						break;
				}
			}
			g_buzzer.counterTone--;
	
			// Check if polarity should change
			if (g_buzzer.counterImpuls == 0)
			{
				if (g_buzzer.polarity)
				{
				    binInOut_setOutput(0, e_Buzzer_P);
					binInOut_setOutput(1, e_Buzzer_N);
					g_buzzer.polarity = 0;
				}
				else
				{
				    binInOut_setOutput(1, e_Buzzer_P);
					binInOut_setOutput(0, e_Buzzer_N);
					g_buzzer.polarity = 1;
				}
				g_buzzer.counterImpuls = g_buzzer.impulsLength;

			}
		
			g_buzzer.counterImpuls--;
		
		}// End if(ringCounter >= BUZZER_RING_PERIOD / 2)
		else
		{
		 	// Do not ring for the other half of the period   
		}
	
		// Check if ring period is finished
		if(g_buzzer.counterRing == 0)
		{
		 	g_buzzer.counterRing = BUZZER_RING_PERIOD;	 
		 	
		 	// Decrement number of rings if ringing is active
			if(g_buzzer.buzzerRing)
		    {
		        g_buzzer.numberOfRings--;  
		    }
		}
		else
		{
			g_buzzer.counterRing--;
		}
		
    }// End if(isBuzzerOn)

}


/*****************************************************************************
  Function:		binInOut_buzzerRingOneFreq()
  Description: 	Makes the buzzer ring with a certain freq and duration
  Inputs:		freq: Frequency with that the buzzer rings, see public constants
  				duration: How long the buzzer should ring in ms
  Outputs:
  Return:		  	
  Globals:		g_buzzer
  Important:
*******************************************************************************/

void binInOut_buzzerRingOneFreq(UINT16 freq, UINT16 duration)
{
    // Initialisation 
    g_buzzerOneFreq.counterTone = 0; 
 	g_buzzerOneFreq.impulsLength = 0;
 	g_buzzerOneFreq.counterImpuls = 0; 
 	g_buzzerOneFreq.polarity = 0; 
 	g_buzzerOneFreq.buzzerOn = false; 
    g_buzzerOneFreq.numberOfRings = 0;
    
    g_buzzerOneFreq.impulsLength = freq;
    g_buzzerOneFreq.counterRing = duration * 10;
    g_buzzerOneFreq.buzzerRing = true;
}

/*****************************************************************************
  Function:		binInOut_sampleToBuzzerOneFreq()
  Description: 	Creates the correct waveform for the buzzer. Overrides the 
  				normal ringing.
  Inputs:		 
  Outputs:
  Return:		  	
  Globals:		g_buzzerOneFreq
  Important:	Has to be called periodically with 10.7khz
*******************************************************************************/

void binInOut_sampleToBuzzerOneFreq(void)
{
    // If buzzerRing is true turn buzzer on
    if(g_buzzerOneFreq.buzzerRing)
    {
        g_buzzerOneFreq.buzzerOn = true; 
        
        // Disable normal ringing
        g_buzzer.numberOfRings = 0;
    	g_buzzer.buzzerRing = false;
    	g_buzzer.buzzerOn = false;
    }
    
   if(g_buzzerOneFreq.buzzerOn)
    {

		// Check if polarity should change
		if (g_buzzerOneFreq.counterImpuls == 0)
		{
			if (g_buzzerOneFreq.polarity)
			{
			    binInOut_setOutput(0, e_Buzzer_P);
				binInOut_setOutput(1, e_Buzzer_N);
				g_buzzerOneFreq.polarity = 0;
			}
			else
			{
			    binInOut_setOutput(1, e_Buzzer_P);
				binInOut_setOutput(0, e_Buzzer_N);
				g_buzzerOneFreq.polarity = 1;
			}
			g_buzzerOneFreq.counterImpuls = g_buzzerOneFreq.impulsLength;

		}
		
		g_buzzerOneFreq.counterImpuls--;
		
		// Check if is finished
		if(g_buzzerOneFreq.counterRing == 0)
		{
		    g_buzzerOneFreq.buzzerOn = false; 
		    g_buzzerOneFreq.buzzerRing = false; 
		}
		else
		{
			g_buzzerOneFreq.counterRing--;
		}
		
    }// End if(isBuzzerOn)

}

/*****************************************************************************
  Function:		void binInOut_toggleTestOutPin(void)
  Description: 	binInOut_toggleTestOutPin() toggles the test output.
  Inputs:		- 
  Outputs:		-
  Return:		-  	
  Globals:		-
  Important:	-
*******************************************************************************/
void binInOut_toggleTestOutPin(void)
{
    static bool testFlag = false;
    
    
    if (testFlag)
    {
        testFlag = false;
        binInOut_setOutput(0, e_Test_Out);
    }
    else
    {
        testFlag = true;
        binInOut_setOutput(1, e_Test_Out);
    }
}

/*****************************************************************************
  Function:		void binInOut_toggleOutPin(e_OutPinId pinId, UINT8 instance)
  Description:	binInOut_toggleOutPin() toggles the output pin specified by
  				pinId.
  Inputs:		e_OutPinId pinId: output pin to toggle 
  				UINT8 instance  : instance of this function
  Outputs:		-
  Return:		-  	
  Globals:		-
  Important:	Use a different instance of this function of each output pin
*******************************************************************************/
void binInOut_toggleOutPin(e_OutPinId pinId, UINT8 instance)
{
    static bool testFlag[NUM_OF_TOGGLE_OUT_PIN_INSTANCES] = {false};
    
    assert( instance < NUM_OF_TOGGLE_OUT_PIN_INSTANCES );
    
    if ( testFlag[instance] )
    {
        testFlag[instance] = false;
        binInOut_setOutput(0, pinId);
    }
    else
    {
        testFlag[instance] = true;
        binInOut_setOutput(1, pinId);
    }
}

/* LOCAL FUNCTIONS ***********************************************************/

/*****************************************************************************
  Function:		swapEWireBuffer()
  Description: 	Swaps the e-wire buffer pointers.
  Inputs:		
  Outputs:
  Return:		       	
  Globals:		g_eWireSender, g_pEWireBuffer, g_eWireBuffer
  Important:
*******************************************************************************/

static void swapEWireBuffer(void)
{
	UINT16 i;
    
   
    // toggle flag
    g_eWireSender.buffer0Active = !g_eWireSender.buffer0Active;
    
    // swap pointers
    if (g_eWireSender.buffer0Active)
    {
        // update global buffer pointer
        g_pEWireBuffer = &g_eWireBuffer[1][0];
        
        // update channel buffer pointers
		for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		{
			g_eWireSender.pChBuffer[i] = &g_eWireBuffer[0][i];
		}
    }
    else
    {
        // update global buffer pointer
        g_pEWireBuffer = &g_eWireBuffer[0][0];
        
        // update channel buffer pointers
		for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		{
			g_eWireSender.pChBuffer[i] = &g_eWireBuffer[1][i];
		}
    }  
}


/*****************************************************************************
  Function:		swapMWireBuffer()
  Description: 	Swaps the m-wire buffer pointers.
  Inputs:		
  Outputs:
  Return:		       	
  Globals:		g_mWireReceiver, g_pMWireBuffer, g_mWireBuffer
  Important:
*******************************************************************************/

static void swapMWireBuffer(void)
{
    UINT16 i;
    
   
    // toggle flag
    g_mWireReceiver.buffer0Active = !g_mWireReceiver.buffer0Active;
    
    // swap pointers
    if (g_mWireReceiver.buffer0Active)
    {
        // update global buffer pointer
        g_pMWireBuffer = &g_mWireBuffer[1][0];
        
        // update channel buffer pointers
		for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		{
			g_mWireReceiver.pChBuffer[i] = &g_mWireBuffer[0][i];
		}
    }
    else
    {
        // update global buffer pointer
        g_pMWireBuffer = &g_mWireBuffer[0][0];
        
        // update channel buffer pointers
		for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
		{
			g_mWireReceiver.pChBuffer[i] = &g_mWireBuffer[1][i];
		}
    }
}


/*****************************************************************************
  Function:		detectTransition()
  Description: 	Detects positive or negative transitions in the specified
  				buffer.The sample values are assumed to be either 0 or 1.
  Inputs:		preVal: last sample of the previous buffer
  				pBuf: buffer pointer		
  Outputs:
  Return:		True if transition detected, else false.	       	
  Globals:		
  Important:
*******************************************************************************/

static bool detectTransition(UINT8 preVal, UINT8 *pBuf)
{
    if (preVal != pBuf[DTR_IX1])
    {
        return(true);
    }
    else if (pBuf[DTR_IX1] != pBuf[DTR_IX2])
    {
        return(true);
    }
    else if (pBuf[DTR_IX2] != pBuf[DTR_IX3])
    {
        return(true);
    }
    else if (pBuf[DTR_IX3] != pBuf[DTR_IX4])
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

/*****************************************************************************
  Function:		static bool detectPositiveEdge(UINT8 preVal, UINT8 *pBuf)
  Description: 	detectPositiveEdge() detects a positive edge in the specified
  				buffer.
  Inputs:		preVal: last sample of the previous buffer
  				pBuf: buffer pointer		
  Outputs:
  Return:		True if positive edge is detected.
  Globals:		
  Important:
*******************************************************************************/
static bool detectPositiveEdge(UINT8 preVal, UINT8 *pBuf)
{
	UINT8 previous, current;
	UINT32 i;

	previous = preVal;

	for (i=0; i<BUFFER_SIZE_M_WIRE_SAMPLES; i++)
	{
		current = pBuf[i];
		if ( previous == 0 && current == 1)
			return true;
		previous = current;
	}

	return false;
}

/*****************************************************************************
  Function:		static bool detectNegativeEdge(UINT8 preVal, UINT8 *pBuf)
  Description: 	detectNegativeEdge() detects a negative edge in the specified
  				buffer.
  Inputs:		preVal: last sample of the previous buffer
  				pBuf: buffer pointer		
  Outputs:
  Return:		True if negative edge is detected.
  Globals:		
  Important:
*******************************************************************************/
static bool detectNegativeEdge(UINT8 preVal, UINT8 *pBuf)
{
	UINT8 previous, current;
	UINT32 i;

	previous = preVal;

	for (i=0; i<BUFFER_SIZE_M_WIRE_SAMPLES; i++)
	{
		current = pBuf[i];
		if ( previous == 1 && current == 0)
			return true;
		previous = current;
	}

	return false;
}

static UINT8 invertSample(UINT8 inVal)
{
    if (inVal > 0)
    {
        return(0);
    }
    else
    {
        return(1);
    }
}

