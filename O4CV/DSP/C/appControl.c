/*-------------------------------------------------------------------------*
 * $Workfile	: appControl.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The appControl module hosts the state machines for the
 * 				  2-wire and 4-wire channels and the service phone.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/appControl.c $
 * 
 * 18    9.04.08 15:46 Chhamaa
 * appControl_init(): if service phone not configured -> RDY LED off
 * 
 * 17    08-01-23 16:11 Chrobif
 * Some comments added.
 * 
 * 16    07-12-11 14:50 Chrobif
 * + FXO state machine: Power down/high impedance versus active state
 * modified.
 * 
 * 15    07-12-11 9:48 Chrobif
 * + Power down/high impedance mode implemented for FXO state machines
 * 
 * 14    07-12-05 8:03 Chrobif
 * + Call of binInOut_clearRingDetector() added in case of FXO.
 * 
 * 13    07-11-28 10:45 Chrobif
 * + Conditions for SPH line interruption modified such that ringing
 * 4-wire PAX on CH1 makes service phone unavailable.
 * 
 * 12    07-11-27 16:19 Chrobif
 * + Ringing E-wire detection added for FXS state machine, remote
 * subscriber case.
 * + Wait state for remote subscriber added.
 * + appControl_getSMState() added.
 * + binInOut_getEWireDelayed replaced by binInOut_getEWire twice in FXO
 * state machine.
 * + Sending of SPH signalling frame after SPH link recovery modified such
 * that signalling frame is only send if SPH is enabled.
 * 
 * 11    07-09-28 11:26 Chrobif
 * Unused variable removed.
 * 
 * 10    07-09-25 13:54 Chrobif
 * In all telephony state machines a call of app_setVoiceState() was added
 * if a transition to idle state is occurs.
 * 
 * 9     07-09-17 7:18 Chrobif
 * Bug fix RDY LED.
 * 
 * 8     14.09.07 16:06 Chhamaa
 * 4-wire state machine modified.
 * 
 * 7     07-09-13 14:59 Chrobif
 * + Prototype of appControl_activate() modified.
 * + t_TelephonyIFData made public for developer status upload.
 * + Initialization of round robin ringing added.
 * + Service phone LEDs implemented
 * + FXS state machine modified
 * + FXO state machine implemented
 * 
 * 6     07-08-27 13:41 Chrobif
 * Compiler Switch EMV_VERSION_ON_OFF_HOOK added.
 * 
 * 5     07-08-24 10:50 Chalnot
 * Round robin ringing functions implemented.
 * 
 * 4     23.08.07 17:40 Chhamaa
 * 4-wire state machine added.
 * 
 * 3     07-08-23 16:51 Chrobif
 * Software counter for 12 second interuption timeout added. Call
 * initiator added to state machine data struct. Activation of DuSlics
 * added. Parameter rxSubframes introduced in appControl_activate(). FXS
 * state machine implemented.
 * 
 * 2     07-07-20 16:31 Chrobif
 * + appControl_isServicePhoneActive() removed.
 * + isOffHook() added.
 * 
 * 1     07-07-13 16:55 Chrobif
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <assert.h>

// O4CV firmware
#include "appControl.h"
#include "O4CV_Types.h"
#include "O4CV_Const.h"
#include "configuration.h"
#include "DuSlic.h"
#include "binInOut.h"
#include "app.h"
#include "voiceData.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define LINE_INTERRUPTION_THRESHOLD	400	// 12 seconds
#define RING_BREAK_TIME 	27 		// 250ms
#define RING_RINGING_TIME 	107		// 1s
#define SPH_LED_OFF_TIME 	54 		// 0.5s
#define SPH_LED_PERIOD 		107		// 1s
#define RS_WAIT_TIME		33		// 1s (33 * 30ms)

/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef enum
{
    
    e_CH1SPH,
    e_CH2,
    e_CH3,
    e_CH4,
    
} e_RingChannelStates;

/* LOCAL VARIABLE DECLARATIONS ***********************************************/
static t_O4CVConfig *gs_pConfig;
static t_TelephonyIFData gs_telephonyIFData[N_CHANNELS_ON_BOARD_PLUS_SPH];
static t_InterfaceCfg gs_2WireCfg;
static t_InterfaceCfg gs_4WireCfg;
static e_2WireIfType  gs_2WireType[N_CHANNELS_ON_BOARD];
static bool gs_isSPhConfigured;
static UINT16 gs_receiverOutOfSyncCounter[N_CHANNELS_ON_BOARD_PLUS_SPH];
static t_RxFrameForDecoder* gs_pFramesForDecoder;
static bool gs_isRinging[N_CHANNELS_ON_BOARD_PLUS_SPH];
static bool gs_stopRinging[N_CHANNELS_ON_BOARD_PLUS_SPH];
static e_RingChannelStates ringChannelState;
static UINT16 gs_activateRingCounter;
static bool gs_isBreak;
static UINT16 gs_sPhLedCounter;
static bool gs_isSPhLedOn;

/* LOCAL MACRO DEFINITIONS ***************************************************/
#define IS_SERVICE_PHONE(pData)		(pData->channel == SPH)
#define IS_HOTLINE(pData)			(pData->afIFMode == AF_INTERFACE_HOTLINE)
#define IS_REMOTE_SUBSCRIBER(pData)	(pData->afIFMode == AF_INTERFACE_REMOTE_SUB)
#define IS_REMOTE_INITIATOR(pData)	(pData->initiator == e_Remote)
#define IS_LOCAL_INITIATOR( pData)	(pData->initiator == e_Local)

/* LOCAL FUNCTION DEFINITIONS ************************************************/
static void stateMachine2WireFXS(t_TelephonyIFData *pData);
static void stateMachine2WireFXO(t_TelephonyIFData *pData);
static void stateMachine4Wire   (t_TelephonyIFData *pData);
static void startRingSequence(UINT16 chNr);
static void stopRingSequence( UINT16 chNr);
static bool isEWireSet(t_TelephonyIFData *pData);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void appControl_init(void)
  Description:	appControl_init() initialises the module appControl.c
  				based on the configuration data downloaded by HMI600.
  				Specifically it initializes global variables and sets
  				the DuSlics into the appropriate state.
  Inputs:		None
  Outputs:		None
  Return:       None
  Globals: 		various
  Important:	appControl _init() needs to be called at the beginning
  				before calling appControl_activate().
*******************************************************************************/
void appControl_init(void)
{
	INT16 i;
	
	// Get pointer to O4CV configuration
	gs_pConfig = configuration_getPCurrentConfig();

	// Initialize telephony interface data structures
	for (i=0; i<N_CHANNELS_ON_BOARD; i++)
	{
		gs_telephonyIFData[i].state = e_Idle;
		gs_telephonyIFData[i].channel = i;
		gs_telephonyIFData[i].afIFMode = gs_pConfig->channel[i].afInterface.mode;
		gs_telephonyIFData[i].isInterrupted = false;
		gs_telephonyIFData[i].isInterruptionLong = false;
		gs_telephonyIFData[i].initiator = e_NoCall;
		gs_telephonyIFData[i].waitCounter = 0;
	}
	gs_telephonyIFData[SPH].state = e_Idle;
	gs_telephonyIFData[SPH].channel = SPH;
	gs_telephonyIFData[SPH].afIFMode = gs_pConfig->servicePhone.afInterface.mode;
	gs_telephonyIFData[SPH].isInterrupted = false;
	gs_telephonyIFData[SPH].isInterruptionLong = false;
	gs_telephonyIFData[SPH].initiator = e_NoCall;
	gs_telephonyIFData[SPH].waitCounter = 0;

	// Get 2-wire interface configuration
	configuration_get2wireCfg(&gs_2WireCfg);

	// Get 4-wire interface configuration
	configuration_get4wireCfg(&gs_4WireCfg);

	// Get 2-wire interface type
	for (i=0; i<N_CHANNELS_ON_BOARD; i++)
	{
		if (i<gs_2WireCfg.nIf)
			gs_2WireType[i] = configuration_get2WireIfType( gs_2WireCfg.ix[i] );
		else
			gs_2WireType[i] = e_Error;
	}

	// Get service phone configuration
	gs_isSPhConfigured = configuration_isServicePhoneActive();

	// Set the service phone ENA and RDY LED
	if (gs_isSPhConfigured)
		binInOut_setOutput(LED_ON , e_Sp_Enabled_Led);
	else
	{
		binInOut_setOutput(LED_OFF, e_Sp_Enabled_Led);
		binInOut_setOutput(LED_OFF, e_Sp_Free_Led);
	}	
		
	// Initialize receiver out of sync counters
	for (i=0; i<N_CHANNELS_ON_BOARD_PLUS_SPH; i++)
		gs_receiverOutOfSyncCounter[i] = 0;
		
	// Initialize round robin ringing variables
	for (i=0; i<N_CHANNELS_ON_BOARD_PLUS_SPH; i++)
	{
		gs_isRinging[i]   = false;
		gs_stopRinging[i] = false;
	}
	ringChannelState = e_CH1SPH;
	gs_activateRingCounter = 0;
	gs_isBreak = false;
	gs_sPhLedCounter = 0;
	gs_isSPhLedOn = false;

	// Set required DuSlics into appropriate state
	for (i=0; i<gs_2WireCfg.nIf; i++)
	{
		switch ( gs_2WireType[i] )
		{

			case e_FXS:
	    		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, gs_2WireCfg.ix[i]);
				break;

			case e_FXO:
				DuSlic_SpiWriteFIFO(SLIC_POWERDOWN, gs_2WireCfg.ix[i]);
				break;

			default:
				assert(false);
		}
	}
	if ( gs_isSPhConfigured )
		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, SPH);
		
	// Get pointer to frame for decoder data structure
	gs_pFramesForDecoder = app_getFrameForDecoderPtr();
}

/*****************************************************************************
  Function:		void appControl_activate(void)
  Description:	appControl_activate() executes the various state machines
  				running for the service phone and for the telephony channels.
  				Additionally the service phone RDY LED is serviced.
  				appControl_activate() needs to be called with a
  				repetition rate of 33.3 Hz.
  Inputs:		None
  Outputs:		None
  Return:       None
  Globals:		various
  Important:	-
*******************************************************************************/
void appControl_activate(void)
{
	UINT16 i;
	bool isSPHInterrupted;
	
	// Determine the line interruption flags for all channels plus service phone
	// Line interruption in the state machine layer can have multiple reasons:
	// (1) The MOD600 is down (!g_receiverInSync)
	// (2) FB/FF causes the a certain channel to be switched off (!gs_pFramesForDecoder[i].ready)
	// For the service phone there is an additional reason:
	// (3) CH1 is using channel 1 (gs_telephonyIFData[CH1].state != e_Idle)
	// Additionally there is a seperate condition for the service phone if the service phone is
	// multiplexed with a 4-wire interface. In this case one has to take into account, that
	// the 4-wire state machine toggles between idle and local init request if the PAX drives the
	// O4CV's M-wire with ringing pulses.

	if ( gs_telephonyIFData[CH1].afIFMode == AF_INTERFACE_4_WIRE )
	{
		isSPHInterrupted = !g_receiverInSync ||
						   !gs_pFramesForDecoder[CH1].ready ||
						   !(gs_telephonyIFData[CH1].state == e_Idle || gs_telephonyIFData[CH1].state == e_LocalInitRequest) ||
						   binInOut_isMWireRinging(CH1);
	}
	else
		isSPHInterrupted = !g_receiverInSync ||
						   !gs_pFramesForDecoder[CH1].ready ||
						   (gs_telephonyIFData[CH1].state != e_Idle);
		
	for (i=0; i<N_CHANNELS_ON_BOARD_PLUS_SPH; i++)
	{
	    // Set flag line interrupted
		if (i==SPH)
		{
		    // Send signalling frame for SPH if SPH is enabled and SPH link recovered.
			if (
					gs_isSPhConfigured &&
					(gs_telephonyIFData[i].isInterrupted && !isSPHInterrupted)
			   )
				binInOut_insertMWireFrame(SPH);
				
			gs_telephonyIFData[i].isInterrupted = isSPHInterrupted;
		}
		else
			gs_telephonyIFData[i].isInterrupted = !g_receiverInSync || !gs_pFramesForDecoder[i].ready;

		// Set flag line interrupted long
		if ( !gs_telephonyIFData[i].isInterrupted )
			gs_receiverOutOfSyncCounter[i] = 0;
		else if ( gs_receiverOutOfSyncCounter[i] < USHRT_MAX)
			gs_receiverOutOfSyncCounter[i]++;
		if ( gs_receiverOutOfSyncCounter[i] >= LINE_INTERRUPTION_THRESHOLD )
			gs_telephonyIFData[i].isInterruptionLong = true;
		else
			gs_telephonyIFData[i].isInterruptionLong = false;
	}
	
	// Service the service phone RDY LED
	if (gs_isSPhConfigured && !gs_isRinging[SPH])
	{
		if ( isSPHInterrupted )
		    binInOut_setOutput(LED_OFF,  e_Sp_Free_Led);
		else
			binInOut_setOutput(LED_ON, e_Sp_Free_Led);
	}

	// Execute 2-wire state machines
	for (i=0; i<gs_2WireCfg.nIf; i++)
	{
		switch (gs_2WireType[i])	
		{
			case e_FXS:
				stateMachine2WireFXS( &gs_telephonyIFData[ gs_2WireCfg.ix[i] ] );
				break;

			case e_FXO:
				stateMachine2WireFXO( &gs_telephonyIFData[ gs_2WireCfg.ix[i] ] );
				break;

			case e_Error:
			default:
				assert(false);
		}
	}
	
	// Execute 4-wire state machines
	for (i=0; i<gs_4WireCfg.nIf; i++)
	{
		stateMachine4Wire( &gs_telephonyIFData[ gs_4WireCfg.ix[i] ] ); 
	}

	// Execute service phone state machine
	if (gs_isSPhConfigured)
		stateMachine2WireFXS(&gs_telephonyIFData[SPH]);
}

/*****************************************************************************
  Function:		static void appControl_activateRingSequence(void)
  Description:	appControl_activateRingSequence() checks which channels should 
  				ring and lets them ring during their timeslot. Each channel 
  				has a timeslot of 1s followed by a 0.25s break. The CH1 and 
  				the service phone share one timeslot because they can not be
  				active at the same time. If the service phone is ringing the 
  				buzzer rings too and the rdy led blinks.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		gs_isRinging, gs_activateRingCounter, gs_isBreak,
  				gs_sPhLedCounter, gs_isSPhLedOn  
  Important:	This function must be called every 107Hz.
*******************************************************************************/
void appControl_activateRingSequence(void)
{
    
   	if(gs_activateRingCounter <= 0)
   	{
   	    
   	    // Set counter to 1.25s
   	    gs_activateRingCounter = RING_RINGING_TIME + RING_BREAK_TIME;
   	    
   	    gs_isBreak = false;
   	    
   	    // Check which timeslot it is 
   	    switch (ringChannelState)
		{

			case e_CH1SPH:
				
				if(gs_isRinging[SPH])
				{
				 	DuSlic_SpiWriteFIFO(SLIC_RING, SPH);	// Start ringing  
				 	
					binInOut_buzzerRing(1);					// If service phone is ringing let also buzzer ring   
					
					gs_stopRinging[SPH] = true;
				}
				
				if(gs_isRinging[CH1])
				{
				 	DuSlic_SpiWriteFIFO(SLIC_RING, CH1); 	// Start ringing   
				 	
				 	gs_stopRinging[CH1] = true;
				}
				
				// Set next timeslot
				ringChannelState = e_CH2;
				break;	
   	    
			case e_CH2:
				
				if(gs_isRinging[CH2])
				{
				 	DuSlic_SpiWriteFIFO(SLIC_RING, CH2); 	// Start ringing   
				 	
				 	gs_stopRinging[CH2] = true;  
				}
				
				// Set next timeslot
				ringChannelState = e_CH3;
				break;	
				
			case e_CH3:
				
				if(gs_isRinging[CH3])
				{
				 	DuSlic_SpiWriteFIFO(SLIC_RING, CH3);  	// Start ringing 
				 	
				 	gs_stopRinging[CH3] = true;   
				}
				
				// Set next timeslot
				ringChannelState = e_CH4;
				break;	
				
			case e_CH4:
				
				if(gs_isRinging[CH4])
				{
				 	DuSlic_SpiWriteFIFO(SLIC_RING, CH4);	// Start ringing     
				 	
				 	gs_stopRinging[CH4] = true; 
				}
				
				// Set next timeslot
				ringChannelState = e_CH1SPH;
				break;	
				
			default:
				assert(false);
		}
   	    
   	}
   	// Stop ringing if it is break time  
   	else if((gs_activateRingCounter <= RING_BREAK_TIME) && (!gs_isBreak))
   	{
   	    if(gs_stopRinging[SPH])
   	    {
   	 		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, SPH);			// Stop ringing 
   	 		gs_stopRinging[SPH] = false;
   	    } 
   	    
   	    if(gs_stopRinging[CH1])
   	    {
   	 		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH1); 			// Stop ringing 
   	 		gs_stopRinging[CH1] = false;
   	    }
   	    
   	    if(gs_stopRinging[CH2])
   	    {
   	 		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH2);			// Stop ringing  
   	 		gs_stopRinging[CH2] = false;
   	    }
   	    
   	 	if(gs_stopRinging[CH3])
   	    {
   	 		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH3);			// Stop ringing  
   	 		gs_stopRinging[CH3] = false;
   	    }
   	    
   	    if(gs_stopRinging[CH4])
   	    {
   	 		DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH4);			// Stop ringing  
   	 		gs_stopRinging[CH4] = false;
   	    }
   	    
   	 	gs_isBreak = true;   
   	} 
 	
   	// Stop ringing when isRinging has changed to false 
   	if(!gs_isRinging[SPH] && gs_stopRinging[SPH])
   	{
   	 	DuSlic_SpiWriteFIFO(SLIC_ACTIVE, SPH);				// Stop ringing 
   	 	gs_stopRinging[SPH] = false;
   	} 
   	
   	if(!gs_isRinging[CH1] && gs_stopRinging[CH1])
   	{
   	 	DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH1);				// Stop ringing 
   	 	gs_stopRinging[CH1] = false;
   	} 
   	
   	if(!gs_isRinging[CH2] && gs_stopRinging[CH2])
   	{
   	 	DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH2);				// Stop ringing 
   	 	gs_stopRinging[CH2] = false;
   	} 
   	
   	if(!gs_isRinging[CH3] && gs_stopRinging[CH3])
   	{
   	 	DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH3);				// Stop ringing 
   	 	gs_stopRinging[CH3] = false;
   	} 
   	
   	if(!gs_isRinging[CH4] && gs_stopRinging[CH4])
   	{
   	 	DuSlic_SpiWriteFIFO(SLIC_ACTIVE, CH4);				// Stop ringing 
   	 	gs_stopRinging[CH4] = false;
   	} 
  	
  	gs_activateRingCounter--;
   	
   	// If service phone is ringing let rdy led blink
   	if(gs_isRinging[SPH])
   	{
   	 	if(gs_sPhLedCounter <= 0)
   	 	{
   	 	    gs_sPhLedCounter = SPH_LED_PERIOD;
   	 	    gs_isSPhLedOn = true;
   	 	    binInOut_setOutput(LED_ON, e_Sp_Free_Led);  
   	 	}   
   	 	
   	 	if((gs_sPhLedCounter <= SPH_LED_OFF_TIME) && gs_isSPhLedOn)
   	 	{
   	 	    binInOut_setOutput(LED_OFF, e_Sp_Free_Led); 
   	 	    gs_isSPhLedOn = false;
   	 	}
   	 	
   	 	gs_sPhLedCounter--;
   	}
   	else
   	{
   	 	gs_sPhLedCounter = 0;   
   	 	
   	 	if(gs_isSPhLedOn)
   	 	{
   	 	    binInOut_setOutput(LED_OFF, e_Sp_Free_Led); 
   	 	    gs_isSPhLedOn = false;
   	 	}
   	}
   	
}

/*****************************************************************************
  Function:		e_TelephonyIFStates appControl_getSMState(UINT16 chNr)
  Description:	appControl_getSMState() retunrs the state of the telephony state
  				machine running for the channel specified by chNr.
  Inputs:		chNr: channel number
  Outputs:		-
  Return:   	e_TelephonyIFStates: state of telephony state machine	
  Globals:		gs_telephonyIFData
  Important:
*******************************************************************************/
e_TelephonyIFStates appControl_getSMState(UINT16 chNr)
{
    return gs_telephonyIFData[chNr].state;
}

/*****************************************************************************
  Function: static void stateMachine2WireFXS(t_TelephonyIFData *pData)
  Description:
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:
*******************************************************************************/
static void stateMachine2WireFXS(t_TelephonyIFData *pData)
{
	switch (pData->state)
	{

		case e_Idle:
		
			if ( DuSlic_isOffHook(pData->channel) && pData->isInterrupted )
			{
				app_activateBusyTone(pData->channel);
				
				binInOut_clearEWire(pData->channel);

				pData->state = e_LineNotAvailable;
			}

			else if ( DuSlic_isOffHook(pData->channel) )
			{
				if ( IS_SERVICE_PHONE(pData) || IS_HOTLINE(pData) )
				{
					app_activateRingingTone(pData->channel);
				}
				
				binInOut_setMWire(pData->channel);

				pData->initiator = e_Local;

				pData->state = e_LocalInitRequest;
			}

			else if ( isEWireSet(pData) )
			{
				startRingSequence(pData->channel);

				pData->initiator = e_Remote;

				pData->state = e_RemoteInitRequest;
			}
			
			break;

		case e_LocalInitRequest:
		
			if ( binInOut_getEWire(pData->channel) )
			{
				if ( IS_SERVICE_PHONE(pData) || IS_HOTLINE(pData) )
				{
					app_deactivateCpTone(pData->channel);
				}

				if ( IS_SERVICE_PHONE(pData) )
				{
					app_activateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_startMWireSampling(pData->channel);
				}

				pData->state = e_Connected;
			}

			else if ( !DuSlic_isOffHook(pData->channel) )
			{
				if ( IS_SERVICE_PHONE(pData) || IS_HOTLINE(pData) )
				{
					app_deactivateCpTone(pData->channel);
				}
				
				binInOut_clearMWire(pData->channel);

				app_setVoiceState(pData->channel);
				
				if ( IS_REMOTE_SUBSCRIBER(pData) )
					binInOut_clearRingingEWireDetector(pData->channel);

				pData->state = e_Idle;
			}

			else if ( pData->isInterrupted )
			{
				if ( IS_SERVICE_PHONE(pData) || IS_HOTLINE(pData) )
				{
					app_deactivateCpTone(pData->channel);
				}

				binInOut_clearMWire(pData->channel);
				
				app_activateBusyTone(pData->channel);

				binInOut_clearEWire(pData->channel);

				pData->state = e_LineNotAvailable;
			}

			break;

		case e_RemoteInitRequest:
			
			if ( DuSlic_isOffHook(pData->channel) )
			{
				stopRingSequence(pData->channel);

				binInOut_setMWire(pData->channel);

				if ( IS_SERVICE_PHONE(pData) )
				{
					app_activateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_startMWireSampling(pData->channel);
				}

				pData->state = e_Connected;
			}

			else if ( !isEWireSet(pData) )
			{
				stopRingSequence(pData->channel);

				app_setVoiceState(pData->channel);

				if ( IS_REMOTE_SUBSCRIBER(pData) )
					binInOut_clearRingingEWireDetector(pData->channel);

				pData->state = e_Idle;
			}

			else if ( pData->isInterrupted )
			{
				stopRingSequence(pData->channel);

				binInOut_clearEWire(pData->channel);

				app_setVoiceState(pData->channel);

				if ( IS_REMOTE_SUBSCRIBER(pData) )
					binInOut_clearRingingEWireDetector(pData->channel);

				pData->state = e_Idle;
			}

			break;

		case e_Connected:

			if ( !DuSlic_isOffHookDelayed(pData->channel) )
			{
				binInOut_clearMWire(pData->channel);
				
				if ( IS_SERVICE_PHONE(pData) )
				{
					app_deactivateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_stopMWireSampling(pData->channel);
				}

				pData->state = e_LocalTerminationRequest;
			}
			
			else if ( !isEWireSet(pData) )
			{
				if ( IS_SERVICE_PHONE(pData) )
				{
					app_deactivateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_stopMWireSampling(pData->channel);
				}

				pData->state = e_RemoteTerminationRequest;
			}

			else if ( pData->isInterrupted )
			{
				if ( IS_SERVICE_PHONE(pData) )
				{
					app_deactivateServicePhone();
				}
				
				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_stopMWireSampling(pData->channel);
				}

				pData->state = e_LineInterruption;
			}

			break;

		case e_LocalTerminationRequest:

			if ( !binInOut_getEWire(pData->channel) ) 
			{
				app_setVoiceState(pData->channel);

				if ( IS_REMOTE_SUBSCRIBER(pData) )
					binInOut_clearRingingEWireDetector(pData->channel);

				pData->state = e_Idle;
			}

			else if (
					  ( IS_SERVICE_PHONE(pData) || IS_HOTLINE(pData) || (IS_REMOTE_SUBSCRIBER(pData) && IS_REMOTE_INITIATOR(pData)) )
					  &&
					  DuSlic_isOffHook(pData->channel)
			   		)
			{
				binInOut_setMWire(pData->channel);

				if ( IS_SERVICE_PHONE(pData) )
				{
					app_activateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_startMWireSampling(pData->channel);
				}

				pData->state = e_Connected;
			}

			else if ( pData->isInterrupted )
			{
				binInOut_clearEWire(pData->channel);
			}

			break;

		case e_RemoteTerminationRequest:
			
			if ( !DuSlic_isOffHook(pData->channel) ) 
			{
				binInOut_clearMWire(pData->channel);

				app_setVoiceState(pData->channel);

				if ( IS_REMOTE_SUBSCRIBER(pData) )
					binInOut_clearRingingEWireDetector(pData->channel);

				pData->state = e_Idle;
			}

			else if ( 
					  ( IS_SERVICE_PHONE(pData) || IS_HOTLINE(pData) || (IS_REMOTE_SUBSCRIBER(pData) && IS_LOCAL_INITIATOR(pData)) ) 
					  &&	
					  binInOut_getEWire(pData->channel)
					) 
			{
				if ( IS_SERVICE_PHONE(pData) )
				{
					app_activateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_startMWireSampling(pData->channel);
				}

				pData->state = e_Connected;
			}

			else if ( pData->isInterrupted )
			{
				binInOut_clearMWire(pData->channel);

				app_activateBusyTone(pData->channel);

				binInOut_clearEWire(pData->channel);

				pData->state = e_LineNotAvailable;
			}

			break;

		case e_LineNotAvailable:
			
			if ( !DuSlic_isOffHook(pData->channel) )
			{
				app_deactivateCpTone(pData->channel);

				app_setVoiceState(pData->channel);

				if ( IS_REMOTE_SUBSCRIBER(pData) )
					binInOut_clearRingingEWireDetector(pData->channel);
				
				pData->state = e_Idle;
			}

			break;

		case e_LineInterruption:
		
			if ( !DuSlic_isOffHook(pData->channel) ) 
			{
				binInOut_clearMWire(pData->channel);

				binInOut_clearEWire(pData->channel);
				
				app_setVoiceState(pData->channel);
				
				

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					pData->waitCounter = 0;
					
					app_activateBusyTone(pData->channel);

					pData->state = e_Wait;

				}
				else
					pData->state = e_Idle;
			}

			else if ( !pData->isInterrupted )
			{
				if ( IS_SERVICE_PHONE(pData) )
				{
					app_activateServicePhone();
				}

				if ( IS_REMOTE_SUBSCRIBER(pData) )
				{
					binInOut_startMWireSampling(pData->channel);
				}

				pData->state = e_Connected;
			}

			else if ( pData->isInterruptionLong ) 
			{
				binInOut_clearMWire(pData->channel);
				
				app_activateBusyTone(pData->channel);

				binInOut_clearEWire(pData->channel);

				pData->state = e_LineNotAvailable;
			}

			break;
			
		case e_Wait:
				
			// Wait until one second passed after
			// permanent link recovery. This enables
			// the remote FXS state machine to
			// clear its M-wire.
			
			if ( !pData->isInterrupted )
			{
				pData->waitCounter++;
			
				if ( pData->waitCounter >= RS_WAIT_TIME )
				{
			    	binInOut_clearRingingEWireDetector(pData->channel);
					app_deactivateCpTone(pData->channel);
					pData->state = e_Idle;
				}
			}
			else
			    pData->waitCounter = 0;
		
			
			break;

		default:
			assert(false);
	}
}

/*****************************************************************************
  Function: static void stateMachine2WireFXO(t_TelephonyIFData *pData)
  Description:
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:
*******************************************************************************/
static void stateMachine2WireFXO(t_TelephonyIFData *pData)
{
	switch (pData->state)
	{

		case e_Idle:
		
			if ( pData->isInterrupted )
			{
				binInOut_clearEWire(pData->channel);

				pData->state = e_LineNotAvailable;
			}

			else if ( binInOut_isRingCurrentOn(pData->channel) )
			{
				binInOut_setMWire(pData->channel);

				pData->state = e_LocalInitRequest;
			}

			else if ( binInOut_getEWire(pData->channel) )
			{
				DuSlic_SpiWriteFIFO(SLIC_ACTIVE, pData->channel);

				pData->state = e_RemoteInitRequest;
			}
			
			break;

		case e_LocalInitRequest:
			
			if ( binInOut_getEWire(pData->channel) )
			{
				DuSlic_SpiWriteFIFO(SLIC_ACTIVE, pData->channel);

				pData->state = e_Connected;
			}

			else if ( !binInOut_isRingCurrentOn(pData->channel) )
			{
				binInOut_clearMWire(pData->channel);

				app_setVoiceState(pData->channel);

				pData->state = e_Idle;
			}

			else if ( pData->isInterrupted )
			{
				binInOut_clearMWire(pData->channel);
				binInOut_clearEWire(pData->channel);

				pData->state = e_LineNotAvailable;
			}

			break;

		case e_RemoteInitRequest:

			binInOut_setMWire(pData->channel);

			pData->state = e_Connected;

			break;

		case e_Connected:
		
			if ( !binInOut_getEWireDelayed(pData->channel) ) 
			{
				pData->state = e_RemoteTerminationRequest;
			}

			else if ( pData->isInterrupted )
			{
				pData->state = e_LineInterruption;
			}

			break;

		case e_RemoteTerminationRequest:
			
			binInOut_clearMWire(pData->channel);

			DuSlic_SpiWriteFIFO(SLIC_POWERDOWN, pData->channel);

			app_setVoiceState(pData->channel);
			
			binInOut_clearRingDetector(pData->channel);

			pData->state = e_Idle;

			break;

		case e_LineNotAvailable:
			
			if ( !pData->isInterrupted )
			{
				app_setVoiceState(pData->channel);

				pData->state = e_Idle;
			}

			break;

		case e_LineInterruption:
			
			if ( !pData->isInterrupted )
			{
				pData->state = e_Connected;
			}

			else if ( pData->isInterruptionLong ) 
			{
				binInOut_clearMWire(pData->channel);
				binInOut_clearEWire(pData->channel);

				DuSlic_SpiWriteFIFO(SLIC_POWERDOWN, pData->channel);

				pData->state = e_LineNotAvailable;
			}

			break;

		default:
			assert(false);
	}
}

/*****************************************************************************
  Function: static void stateMachine4Wire(t_TelephonyIFData *pData)
  Description:
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:
*******************************************************************************/
static void stateMachine4Wire(t_TelephonyIFData *pData)
{
        switch(pData->state)	
	{
	    case e_Idle:
	    
	    	if (pData->isInterrupted && binInOut_getMWire(pData->channel))
			{
			    binInOut_clearEWire(pData->channel);
			    binInOut_disableMWireForwarding(pData->channel);
			 
			    pData->state = e_LineNotAvailable;
			}
			else if (binInOut_getMWire(pData->channel))
			{
			    pData->state = e_LocalInitRequest;
			}
			else if (binInOut_getEWire(pData->channel))
			{
			    pData->state = e_RemoteInitRequest;   
			} 
	    
	    	break;
	    	
    	case e_LocalInitRequest:
    	
    		if (pData->isInterrupted)
			{
			    binInOut_clearEWire(pData->channel);
			    binInOut_disableMWireForwarding(pData->channel);
			 
			    pData->state = e_LineNotAvailable;
			}
			else if (!binInOut_getMWire(pData->channel))
			{
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;
			} 
			else if (binInOut_getEWire(pData->channel))
			{
			    pData->state = e_Connected;
			}  
    	
    		break;
    		
    	case e_RemoteInitRequest:
    	
    		if (pData->isInterrupted)
			{
			    binInOut_clearEWire(pData->channel);
			    
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;
			}
			else if (!binInOut_getEWire(pData->channel))
			{
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;
			}
			else if (binInOut_getMWire(pData->channel))
			{
			    pData->state = e_Connected;
			}      
			    
    		break;
    		
    	case  e_Connected:
    	
    		if (pData->isInterrupted)
			{
			    pData->state = e_LineInterruption;
			}
			else if (!binInOut_getMWireDelayed(pData->channel))
			{
			    pData->state = e_LocalTerminationRequest;
			}
			else if (!binInOut_getEWireDelayed(pData->channel))
			{
			    pData->state = e_RemoteTerminationRequest;
			}   
    	
    		break;
    		
    	case e_LocalTerminationRequest:
    	
    		if (pData->isInterrupted)
			{
			    binInOut_clearEWire(pData->channel);

				app_setVoiceState(pData->channel);
			    
			    pData->state = e_Idle;
			}
			else if (binInOut_getMWire(pData->channel))
			{
			    pData->state = e_Connected;
			}
			else if (!binInOut_getEWire(pData->channel))
			{
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;	   
			}
    	
    		break;
    		
    	case e_RemoteTerminationRequest:
    	
    		if (pData->isInterrupted)
			{
			    binInOut_clearEWire(pData->channel);
			    binInOut_disableMWireForwarding(pData->channel);
			 
			    pData->state = e_LineNotAvailable;
			}
			else if (binInOut_getEWire(pData->channel))
			{
			    pData->state = e_Connected;
			}
			else if (!binInOut_getMWire(pData->channel))	
			{
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;
			}    
    	
    		break;
    	
    	case e_LineNotAvailable:
    	
    		if (!binInOut_getMWire(pData->channel))	
			{
			    binInOut_enableMWireForwarding(pData->channel);
			    
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;
			}
    	
    		break;
    		
    	case e_LineInterruption:
    	
    		if (!pData->isInterrupted)
			{
			    pData->state = e_Connected;
			}
			else if (!binInOut_getMWire(pData->channel))	
			{
			    binInOut_clearEWire(pData->channel);
			    
				app_setVoiceState(pData->channel);

			    pData->state = e_Idle;
			}
			else if (pData->isInterruptionLong)
			{
				binInOut_clearEWire(pData->channel);
			    binInOut_disableMWireForwarding(pData->channel);
			 
			    pData->state = e_LineNotAvailable;
			} 
    	
    		break;
    		
    	default:
    	
			app_setVoiceState(pData->channel);
    		pData->state = e_Idle;
    		
	}
}

/*****************************************************************************
  Function:		static void startRingSequence(UINT16 chNr)
  Description:	startRingSequence() starts the periodic ring sequence on the specified
  				channel. A ring sequence lets the connected phone ring for
				1 second and mute for 4 seconds.
				In case of channel SPH, the buzzer and the RDY LED is
				also used in the ring sequence. The buzzer buzzes for 300ms
				each time the connected phone rings where as the RDY LED
				continuously repeats the sequence 0.5 second on and
				0.5 second off.
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:		None
  Return:		None
  Globals:		gs_isRinging
  Important:	-
*******************************************************************************/
static void startRingSequence(UINT16 chNr)
{
    // If chNr is the service phone check first if ch1 is ringing
    if(gs_isRinging[CH1] && (chNr == SPH))
    {
        // Do not ring the service phone
    	gs_isRinging[SPH] = false;
    }
    // If chNr is the ch1 check if service phone is ringing
    else if(gs_isRinging[SPH] && (chNr == CH1))
    {
        // Stop service phone and let ch1 ring
     	gs_isRinging[SPH] = false;   
     	gs_isRinging[CH1] = true;   
    }
    else
    {
      	gs_isRinging[chNr] = true;   
    }
}

/*****************************************************************************
  Function:		static void stopRingSequence(UINT16 chNr)
  Description:	stopRingSequence() stops the ring sequence on the specified
  				channel. In case of channel SPH, the buzzer and the RDY LED is
				stopped as well.
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:		None
  Return:		None
  Globals:		gs_isRinging
  Important:	-
*******************************************************************************/
static void stopRingSequence(UINT16 chNr)
{
    gs_isRinging[chNr] = false;  
}

/*****************************************************************************
  Function:		static bool isEWireSet(t_TelephonyIFData *pData)
  Description:	isEWireSet() calls binInOut_isEWireRinging()
  				if the telephony mode is remote subscriber and 
				otherwise binInOut_getEWire() to get the E-wire
				state.
  Inputs:		t_TelephonyIFData *pData: Pointer to state machine's static data
  Outputs:		None
  Return:		bool: true, if E-wire is set
  					  false, otherwise
  Globals:		-
  Important:	-
*******************************************************************************/
static bool isEWireSet(t_TelephonyIFData *pData)
{
    if ( IS_REMOTE_SUBSCRIBER(pData) )
        return binInOut_isEWireRinging(pData->channel);
    else
        return binInOut_getEWire(pData->channel);
}

