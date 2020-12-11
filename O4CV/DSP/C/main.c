/*-------------------------------------------------------------------------*
 * $Workfile: main.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Main file of O4CV firmware
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/main.c $
 * 
 * 40    08-04-22 17:35 Chalnot
 * Watchdog activated in Emergency Fw.
 * 
 * 39    10.04.08 9:46 Chhamaa
 * Initialisation of 4-w interface delayed.
 * 
 * 38    08-02-19 13:42 Chalnot
 * + DuSlic PAX driving error fixed (again).
 * EXT_TDM_BUS_STOP_DELAY set back to 0.5s.
 * 10s delay added before DuSlic_config().
 * 
 * 37    08-02-15 10:49 Chrobif
 * + DuSlic PAX driving error fixed (again).
 *    Wait 15s between last chunk of config struct received and voltage
 *    measurement.
 * 
 * 36    08-01-29 18:30 Chrobif
 * binInOut init source added
 * 
 * 34    07-11-23 15:57 Chalnot
 * msgHandler_init() called if emergency firmware is running.
 * 
 * 33    07-10-26 16:37 Chalnot
 * Reduced licence check to max 5 tries.
 * 
 * 32    07-10-24 11:55 Chalnot
 * Check licence before ext tdm bus is started.
 * 
 * 31    07-10-19 11:51 Chalnot
 * Let Rdy LED blink if em fw is running.
 * 
 * 30    07-10-15 16:04 Chalnot
 * status_clearAllAlarmsAndWarnings() is called at the beginning of
 * startup().
 * 
 * 29    07-10-09 8:49 Chalnot
 * Repeat licence check also if the secret or the licence code is wrong.
 * 
 * 27    28.09.07 15:33 Chhamaa
 * Alarm O4CV_ALARM_DUSLIC_SYNC_FAILED cleared after starting 2-wire
 * interface.
 * 
 * 26    07-09-28 14:11 Chalnot
 * Licence check optimized.
 * 
 * 25    27.09.07 18:19 Chhamaa
 * Master and slave timing optimised.
 * 
 * 24    07-09-25 9:41 Chalnot
 * O4CV_ALARM_EM_FW_ACTIVE bit set if Em Fw is running.
 * 
 * 23    07-09-24 13:12 Chalnot
 * Em Fw with compiler switch EMERGENCY_FW implemented.
 * 
 * 22    17.09.07 17:45 Chhamaa
 * Restart implemented.
 * 
 * 21    07-09-13 15:01 Chrobif
 * Call of appControl_activate() added.
 * 
 * 20    07-09-06 14:59 Chalnot
 * New function watchdog_getHwResetFlag() used.
 * 
 * 19    07-09-06 8:41 Chalnot
 * Order of status_init and configuration_init switched.
 * 
 * 18    07-09-04 14:09 Chalnot
 * Watchdog time set to 10s at beginning of init. 
 * Slot mismatch and licence alarms implemented.
 * 
 * 17    07-08-31 12:40 Chrobif
 * + Call of extTdmBus_setReadDuSlicIntFlag() added.
 * + Call of appControl_init() added.
 * + Call of status_enableLeds() added.
 * 
 * 16    07-08-30 15:07 Chalnot
 * Memory check added with compiler switch MEMORYCHECK.
 * 
 * 15    28.08.07 14:23 Chhamaa
 * Comment added.
 * 
 * 14    27.08.07 18:25 Chhamaa
 * Compiler switch WATCHDOG and call of app_faxControl() added.
 * 
 * 13    07-08-23 16:49 Chrobif
 * 2-wire interface initialisation moved to DuSlic_config().
 * Random value determination for i-Button modified.
 * Activation of DuSlics moved to appControl_init().
 * 
 * 12    07-08-21 12:46 Chalnot
 * Set random value in initIButton function.
 * 
 * 11    16.08.07 19:21 Chhamaa
 * Call of binInOut_initRingCurrentDetector() added.
 * 
 * 10    14.08.07 16:31 Chhamaa
 * Buzzer call added after initialisation.
 * 
 * 9     07-08-14 14:42 Chalnot
 * Watchdog added.
 * 
 * 8     07-08-13 10:18 Chalnot
 * iButton licence check implemented.
 * 
 * 7     8.08.07 14:34 Chhamaa
 * Functions for e- and m-wire signal handling added.
 * 
 * 6     25.07.07 9:59 Chhamaa
 * gain_init() added.
 * 
 * 5     9.07.07 16:00 Chhamaa
 * Service phone interface activated.
 * 
 * 4     4.07.07 11:46 Chhamaa
 * Initialisation of 2- and 4-wire interface and basic scheduling added.
 * 
 * 3     29.05.07 13:30 Chrobif
 * Call of init function of modules for external TDM bus services added.
 * Generic call of sport0_initExtTdmBus() implemented.
 * 
 * 2     19.04.07 16:17 Chhamaa
 * sport functions renamed
 * 
 * 1     17.04.07 17:31 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/

/* COMPILER SWITCHES *******************************************************/

//#define WATCHDOG		// enables watchdog, set in release configuration
						// by VisualDSP IDE
						
//#define MEMORYCHECK	// enables Memory Check, set in release configuration
						// by VisualDSP IDE		
						
//#define EMERGENCY_FW	// enables emergency Firmware, set in release 
						// configuration by VisualDSP IDE									
								
/* INCLUDE FILES ***********************************************************/

#include <sysreg.h>
#include <ccblkfn.h>
#include <cdefBF538.h>
#include <assert.h>

#include "O4CV_Const.h"
#include "Initialize.h"
#include "sport0.h"
#include "sport1.h"
#include "sport2.h"
#include "extTdmBus.h"
#include "ft12.h"
#include "dll2ndStation.h"
#include "msgHandler.h"
#include "epromHigh.h"
#include "configuration.h"
#include "status.h"
#include "app.h"
#include "appControl.h"
#include "voiceData.h"
#include "DuSlic.h"
#include "gain.h"
#include "binInOut.h"
#include "iButHigh.h"
#include "licenceCheck.h"
#include "watchdog.h"
#include "memoryCheck.h"
#include "timer.h"
#include "epromLow.h"
#include "epromMed.h"


/* LOCAL CONSTANT DECLARATIONS *********************************************/

#define CYCLE_TIME				30000		// [us]
#define ENCODER_PROC_TIME		20000		// [us]
#define DECODER_PROC_TIME	 	7000		// [us]
#define DECODER_DELAY			(CYCLE_TIME - DECODER_PROC_TIME)

#define EXT_TDM_BUS_STOP_DELAY	500000		// [us]
#define DISCHARGE_DELAY			10000000	// [us]

#define WATCHDOG_REFRESH_TIME1	10000000	// [us]
#define WATCHDOG_REFRESH_TIME2	500000		// [us]

#define IBUTTON_WAIT_TIME		10000		// [us]
#define IBUTTON_MAX_COUNTER		5	

#define RANDOM_VALUE_SECTOR    	SECTOR_SA69
#define	BOOT_FLASH_OFFSET		0x20000000	

/* LOCAL TYPE DECLARATIONS *************************************************/

typedef enum {

    e_Startup,
    e_WaitingForNewCfg,
	e_StartingMaster,
	e_ProcessingMaster,
	e_WaitingForSyncSlave,
	e_SynchronisingSlave,
	e_ProcessingSlave

} e_SchedulingStates;

/* LOCAL VARIABLE DECLARATIONS *********************************************/

static t_O4CVConfig *g_pConfig;
static t_O4CVStatus *g_pStatus;
static e_SchedulingStates g_schedulingState;
static bool g_powerOn;
static UINT32 randomValue;

// 4-wire interface buffer pointers
extern INT16 *g_pTxBuffer4wIf;
extern INT16 *g_pRxBuffer4wIf;


/* LOCAL MACRO DEFINITIONS *************************************************/

/* LOCAL FUNCTION DEFINITIONS **********************************************/

static void startup(void);
static void waitForNewCfg(void);
static void startMaster(void);
static void processMaster(void);
static void waitForSyncSlave(void);
static void syncSlave(void);
static void processSlave(void);

static void initAndStartExtTdmBus(e_BinInOutInitSource initSource);
static void stopAllSports(void);


/* IMPLEMENTATION **********************************************************/

/***************************************************************************
  Function:		main()	  
  Description:	Initialization of HW, start and scheduling of application	                    
  Inputs:		-                                                                            
  Outputs:		-                                                                                                                                      
  Return:		-      	
  Globals:		
  Important: 
                                          	
****************************************************************************/

void main(void)
{
    
	//////////////////////
	// initialization part
	//////////////////////
	
	// init system configuration register
	sysreg_write(reg_SYSCFG, 0x0002);	// enable 64-bit, free-running cycle counter
	
	// init DSP in/output pins
	Init_flags();

	// init PLL
	Init_PLL();	
	
	// init EPROM	
	epromHigh_init();
	
#ifdef EMERGENCY_FW	
	
	// Deactivate watchdog 
	watchdog_deactivate();
	watchdog_setTime(WATCHDOG_REFRESH_TIME2);
	watchdog_activate();
	
	// Init status and set em fw active alarm
	status_init();
	status_enableLeds();
	status_setAlarm(O4CV_ALARM_EM_FW_ACTIVE); 
	
	// init msgHandler module
	msgHandler_init();
	
	// init and start external TDM bus services
	initAndStartExtTdmBus(e_HwStartUp);
	
	// Go in endless loop 
	while(true){}   
		
#else
	
	// set initial state
	g_schedulingState = e_Startup;
	g_powerOn = true;
	
	
	////////////
	// main loop
	////////////
	
	while(1)
	{
	    
	    switch(g_schedulingState)
	    {
	        case e_Startup:
	        
	        	startup();
	        	break;
	        	
	        case e_WaitingForNewCfg:
	        
	        	waitForNewCfg();
	        	break;
	        	
	        case e_StartingMaster:
	        
	        	startMaster();
	        	break;
	        	
	        case e_ProcessingMaster:
	        
	        	processMaster();
	        	break;
	        	
	        case e_WaitingForSyncSlave:
	        
	        	waitForSyncSlave();
	        	break;
	        	
	        case e_SynchronisingSlave:
	
	        	syncSlave();
	        	break;
	        
	        case e_ProcessingSlave:
	      	
				processSlave();	        
	        	break;
	        	
	        default:
	        
				assert(false);
	    }
	    	
	}
	
#endif
		
}


/***************************************************************************
  Function:		startup(void)		
  Description:	Initialisation of hardware and firmware modules.
  Inputs:		
  Outputs:			
  Return:				
  Globals:				
  Important:	
  
****************************************************************************/

static void startup()
{
    t_4w_Gains gainSettings4wIf;
    UINT16 iButtonInitCounter;
    e_BinInOutInitSource initSource;
    
    // Clear all alarms and warnings except O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE
    status_clearAllAlarmsAndWarnings();
    
    // Deactivate watchdog 
	watchdog_deactivate();

	// Set watchdog time to 10s and only activate it if WATCHDOG is defined
#ifdef WATCHDOG		
	watchdog_setTime(WATCHDOG_REFRESH_TIME1);
	watchdog_activate();
#endif
	
	// Determine binInOut init source
	if (g_powerOn)
		initSource = e_HwStartUp;
	else
		initSource = e_SwRestart;
		
	if (g_powerOn)	// initial start up: perform memory test and get configuraion from flash
    {
        // reset flag
        g_powerOn = false;
        
#ifdef MEMORYCHECK		
		// memory check
		memoryCheck_startupMemoryCheck();
	
		if(memoryCheck_getCrcAlarm())
		{
		 	// Make hardreset after 1s
		 	watchdog_hwReset();
		}
		else
		{
		    // Set bootTries to 0
		 	memoryCheck_resetBootTries();  
		}
#endif	

		// init configuration and status modules
		status_init();
		g_pStatus = status_getPCurrentStatus();
		configuration_init();
		g_pConfig = configuration_getPCurrentConfig();
    }
	
	// Set slot mismatch alarm if the slot id of the configuration does not match the physical slot id
	if(Init_getBasicTimeSlotID() != configuration_getBasicTimeSlotID())
	{
	 	status_setAlarm(O4CV_ALARM_SLOT_MISMATCH);   
	}
	
	// init msgHandler module
	msgHandler_init();
	
	// Test licence of the iButton
	iButtonInitCounter = 0;
	randomValue = *((UINT32*)(RANDOM_VALUE_SECTOR + BOOT_FLASH_OFFSET));		// read random value from last startup
	while (iButtonInitCounter < IBUTTON_MAX_COUNTER)
	{
		if(iButH_initIButton(randomValue))										// check if there is a responding iButton	
		{
			if(iButH_isIButtonValid())											// check if the iButton has the right secret	
			{
			    // Write licence code of the ibutton in the equipment struct
			    msgHandler_setLicenseCode(iButH_getLicenceCode());
		    
			    status_clearAlarm(O4CV_ALARM_INVALID_LICENSE);
			    status_clearAlarm(O4CV_ALARM_INSUFFICIENT_LICENSE);   
			    
			    if(iButH_getLicenceCode() != LICENCE_DEF_O4CV_2CHANNELS &&		// check if the iButton is for O4CV
			    	iButH_getLicenceCode() != LICENCE_DEF_O4CV_4CHANNELS)
			    {
			        // not O4CV ibutton  
			    	status_setAlarm(O4CV_ALARM_INVALID_LICENSE); 	    
			   	}
				else if(!licenceCheck_verifyLicence()) 
				{
				 	// insufficient ibutton 
				 	status_setAlarm(O4CV_ALARM_INSUFFICIENT_LICENSE);   
				}
				else
				{
				    // licence is ok exit while loop
					iButtonInitCounter = IBUTTON_MAX_COUNTER;
				}
			}
			else
			{
			 	// invalid ibutton   
			 	status_setAlarm(O4CV_ALARM_INVALID_LICENSE);   
			}
			
		}
		else
		{
		 	// no ibutton detected 
		 	status_setAlarm(O4CV_ALARM_INVALID_LICENSE);   
		 	
		}
		
		if (iButtonInitCounter < IBUTTON_MAX_COUNTER)
		{
			iButtonInitCounter++;
			timer_init(IBUTTON_WAIT_TIME, e_Timer1);
			timer_wait(e_Timer1);
		}
		
	}
	
	// init application
	app_init(g_pConfig);

	// init and start external TDM bus services
	initAndStartExtTdmBus(initSource);

	// set gain coefs
	gain_init(g_pConfig, &gainSettings4wIf);
	
	// wait until capacitors in the telephones are discharged
	timer_init(DISCHARGE_DELAY, e_Timer1);
	timer_wait(e_Timer1);
	
	// init 4-wire interface					
	sport2_init4wireInterface(&gainSettings4wIf);
	
	// start 4-wire interface (workaround)
	sport2_start4wireInterface();
		
	// init 2-wire interface
	// Note: Periodic read of DuSlic interrupts must be started after
	//		 DuSlic is configured.
	sport1_init2wireInterface();
	randomValue = DuSlic_config();
	extTdmBus_setReadDuSlicIntFlag(true);
	
	// store random value to flash for next licence check
	epromMed_pushCommand(ERASE_SECTOR, (UINT16*) &randomValue, (UINT16*) RANDOM_VALUE_SECTOR, sizeof(randomValue));	 
    epromMed_pushCommand(SAVE_DATA, (UINT16*) &randomValue, (UINT16*) RANDOM_VALUE_SECTOR, sizeof(randomValue));	
	
	// init appControl module
	appControl_init();
	
#ifdef WATCHDOG	
	// If hw reset flag is false then set watchdog time to 500ms
	if(!watchdog_getHwResetFlag())
	{
		watchdog_deactivate();
		watchdog_setTime(WATCHDOG_REFRESH_TIME2);
		watchdog_activate();
	}
#endif

	// Enable alarm and status LEDs
	status_enableLeds();

	// Go in endless loop if ibutton is invalid or insufficient
	if(g_pStatus->alarms == O4CV_ALARM_INVALID_LICENSE 
		|| g_pStatus->alarms == O4CV_ALARM_INSUFFICIENT_LICENSE) 
	{
	 	g_schedulingState = e_WaitingForNewCfg;  
	}
	else if (g_pConfig->settings & OP_MODE_BIT)
	{
	    g_schedulingState = e_StartingMaster;
	}
	else
	{
	    g_schedulingState = e_SynchronisingSlave;
	}
	
	// stop 4-wire interface (workaround)
	sport2_stop4wireInterface();
	
	// generate buzzer sequence
	binInOut_buzzerRing(1);

	// reset flag
	g_decoderDataReady = false;
		
}


/***************************************************************************
  Function:		waitForNewCfg(void)		
  Description:	This functions checks if new configuration data has been 
  				downloaded. In this case a system startup is initiated.
  Inputs:		
  Outputs:			
  Return:				
  Globals:				
  Important:	
  
****************************************************************************/

static void waitForNewCfg()
{
    if (msgHandler_getNewCfgReceivedFlag())
	{
		// Set DuSlics configured as FXS into power down / high impedance state
		DuSlic_DeactivateFxsIf();

	    // wait for ack to be sent to HMI
	    timer_init(EXT_TDM_BUS_STOP_DELAY, e_Timer1);
		timer_wait(e_Timer1);
		
		// stop external TDM bus
		sport0_stopExtTdmBus();
		
		// set next state
		g_schedulingState = e_Startup;
	}
}


/***************************************************************************
  Function:		startMaster(void)		
  Description:	Starts serial ports to send and receive data in master
  				operation mode.
  Inputs:		
  Outputs:			
  Return:				
  Globals:				
  Important:	
  
****************************************************************************/

static void startMaster(void)
{
    
	// start sending e-wire and receiving m-wire signals
	binInOut_startTxEWireAndRxMWire();
			           
	// start 4-wire interface
	sport2_start4wireInterface();
	
	// start 2-wire interface
	sport1_startRx2wireInterface();
	sport1_startTx2wireInterface();
	
	status_clearAlarm(O4CV_ALARM_DUSLIC_SYNC_FAILED);
	
	// encoder processing time
	timer_init(ENCODER_PROC_TIME, e_Timer1);
	timer_wait(e_Timer1);

	// start sending subframes
	voiceData_startTx();
	
	// set next state
	g_schedulingState = e_ProcessingMaster;
}


/***************************************************************************
  Function:		processMaster(void)		
  Description:	Encoding and decoding of pcm/voice data in master operation 
  				mode.
  Inputs:		
  Outputs:			
  Return:				
  Globals:		
  Important:	
  
****************************************************************************/

static void processMaster(void)
{
    
    if (sport1_2wireDataAvailable())
	{
	    // init decoding delay
	    timer_init(DECODER_DELAY, e_Timer2);
	    
		app_encode();
		app_faxControl();
		appControl_activate();
		
		// wait for decoding
		timer_wait(e_Timer2);
		
		app_decode();
	}
	
	if (msgHandler_getNewCfgReceivedFlag())
	{
		// Set DuSlics configured as FXS into power down / high impedance state
		DuSlic_DeactivateFxsIf();

	    // wait for ack to be sent to HMI
	    timer_init(EXT_TDM_BUS_STOP_DELAY, e_Timer1);
		timer_wait(e_Timer1);
		
		stopAllSports();
		
		// set next state
		g_schedulingState = e_Startup;
	}
	
	if (extTdmBus_getRestartFlag())
	{
	    stopAllSports();

		initAndStartExtTdmBus(e_TdmBusRestart);
		
		// start DuSlic polling
		extTdmBus_setReadDuSlicIntFlag(true);
		
		// set next state
		g_schedulingState = e_StartingMaster;
	}

}


/***************************************************************************
  Function:		waitForSyncSlave(void)		
  Description:	Waits for voice data from mux.
  Inputs:		
  Outputs:			
  Return:				
  Globals:			
  Important:	
  
****************************************************************************/

static void waitForSyncSlave(void)
{
    if (sport1_2wireDataAvailable())
	{
		app_decode();
		app_faxControl();
		appControl_activate();
	}
	
	if (g_decoderDataReady)
	{
		// stop 2-wire interface
	    sport1_stopRx2wireInterface();
		sport1_stopTx2wireInterface();
	    
	    // stop 4-wire interface
	    sport2_stop4wireInterface();

		// stop sending voice data to mux
		voiceData_initTx();
		
		// stop sending/receiving e-/m-wire samples
		binInOut_initTxEWireAndRxMWire(g_pConfig, e_SlaveSync); 	    
	
		// set next state
		g_schedulingState = e_SynchronisingSlave;
		
		// reset flag
		g_decoderDataReady = false;
	}
	
	if (msgHandler_getNewCfgReceivedFlag())
	{
		// Set DuSlics configured as FXS into power down / high impedance state
		DuSlic_DeactivateFxsIf();

	    // wait for ack to be sent to HMI
	    timer_init(EXT_TDM_BUS_STOP_DELAY, e_Timer1);
		timer_wait(e_Timer1);
		
		stopAllSports();
		
		// set next state
		g_schedulingState = e_Startup;
	}
	
	if (extTdmBus_getRestartFlag())
	{
	    stopAllSports();

		initAndStartExtTdmBus(e_TdmBusRestart);
		
		// start DuSlic polling
		extTdmBus_setReadDuSlicIntFlag(true);
		
		// start sending e-wire and receiving m-wire samples
		binInOut_startTxEWireAndRxMWire();
	
        // start 4-wire interface
		sport2_start4wireInterface();
	
		// start 2-wire interface
		sport1_startRx2wireInterface();
		sport1_startTx2wireInterface();
		
		// set next state
		g_schedulingState = e_WaitingForSyncSlave;

	}
		
}


/***************************************************************************
  Function:		syncSlave(void)		
  Description:	Synchronises signal processing in slave operatin mode.
  Inputs:		
  Outputs:			
  Return:				
  Globals:			
  Important:	
  
****************************************************************************/

static void syncSlave(void)
{
    
    if (g_decoderDataReady)
    {
        // decoder processing time
        timer_init(DECODER_PROC_TIME, e_Timer1);
		timer_wait(e_Timer1);
        
        // start sending e-wire and receiving m-wire samples
		binInOut_startTxEWireAndRxMWire();
	
        // start 4-wire interface
		sport2_start4wireInterface();
	
		// start 2-wire interface
		sport1_startRx2wireInterface();
		sport1_startTx2wireInterface();
		
		status_clearAlarm(O4CV_ALARM_DUSLIC_SYNC_FAILED);
	
		// encoder processing time
        timer_init(ENCODER_PROC_TIME, e_Timer1);
		timer_wait(e_Timer1);
		
		// start sending subframes
		voiceData_startTx();
		
		// set next state
		g_schedulingState = e_ProcessingSlave;
		
		// reset flag
        g_decoderDataReady = false; 
    }
    
    if (!g_receiverInSync)
	{
	    // start sending e-wire and receiving m-wire samples
		binInOut_startTxEWireAndRxMWire();
	
        // start 4-wire interface
		sport2_start4wireInterface();
	
		// start 2-wire interface
		sport1_startRx2wireInterface();
		sport1_startTx2wireInterface();
		
		status_clearAlarm(O4CV_ALARM_DUSLIC_SYNC_FAILED);
		
		// set next state
		g_schedulingState = e_WaitingForSyncSlave;
	}
    
}


/***************************************************************************
  Function:		processSlave(void)		
  Description:	Encoding and decoding of pcm/voice data in slave operation
  				mode.
  Inputs:		
  Outputs:			
  Return:				
  Globals:			
  Important:	
  
****************************************************************************/

static void processSlave(void)
{
    
    if (g_decoderDataReady)
	{
		app_decode();
	}
	
	if (sport1_2wireDataAvailable())
	{
		app_encode();
		app_faxControl();
		appControl_activate();
	}
	
	if (!g_receiverInSync)
	{
		// set next state
		g_schedulingState = e_WaitingForSyncSlave; 
	}
	
	if (msgHandler_getNewCfgReceivedFlag())
	{
		// Set DuSlics configured as FXS into power down / high impedance state
		DuSlic_DeactivateFxsIf();

	    // wait for ack to be sent to HMI
	    timer_init(EXT_TDM_BUS_STOP_DELAY, e_Timer1);
		timer_wait(e_Timer1);
		
		stopAllSports();
		
		// set next state
		g_schedulingState = e_Startup;
	}
	
	if (extTdmBus_getRestartFlag())
	{
	    stopAllSports();

		initAndStartExtTdmBus(e_TdmBusRestart);
		
		// start DuSlic polling	
		extTdmBus_setReadDuSlicIntFlag(true);
		
		// set next state
		g_schedulingState = e_SynchronisingSlave;
		
		// reset flag
		g_decoderDataReady = false;
	}
	
}



static void initAndStartExtTdmBus(e_BinInOutInitSource initSource)
{
    INT16 boardNumber;

    // init external TDM bus services modules
	dll2ndStation_init();
	ft12_init();
	extTdmBus_init();
	
#ifndef EMERGENCY_FW

	voiceData_initTx();
	voiceData_initRx();
	
	// init binInOut.c functions
	binInOut_initTxEWireAndRxMWire(g_pConfig, initSource);
	binInOut_initRingCurrentDetector();

#endif
		
	// set board number
	if (Init_getBasicTimeSlotID() == configuration_getBasicTimeSlotID())
	{
	 	boardNumber = configuration_getBoardNumber();   
	}
	else
	{
		boardNumber = 0;	// disables extended time slots
	} 
	
	// init external TDM bus
	sport0_initExtTdmBus(Init_getBasicTimeSlotID(), boardNumber);				

	// start external TDM bus
	sport0_startExtTdmBus();
}


static void stopAllSports(void)
{
    // stop 2-wire interface
    sport1_stopRx2wireInterface();
	sport1_stopTx2wireInterface();
	
	// stop 4-wire interface
	sport2_stop4wireInterface();
	
	// reset output buffers to avoid noise on the connected phones
	memset(g_pRxBuffer4wIf, 0, N_CHANNELS_ON_BOARD*BUFFER_SIZE_4W_IF*2); 	// size in #bytes
  	memset(g_pTxBuffer4wIf, 0, N_CHANNELS_ON_BOARD*BUFFER_SIZE_4W_IF*2);	// size in #bytes
	
	// stop external TDM bus
    sport0_stopExtTdmBus();
}
