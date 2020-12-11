/*---------------------------------------------------------------------------*
 * $Workfile: Master_DSP_Main.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz
 * Remarks      :  
 * Purpose      : MAster_DSP main loop
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/Master_DSP/C/Master_DSP_Main.c $ 
 * 
 * 66    15-01-14 17:52 Chhamaa
 * Init value sent to up-converter DSP modified.
 * 
 * 65    12-10-03 10:51 Chhamaa
 * New APLC modes APLC_CHANNEL_MODE_A1_25I/E for 2.5 kHz TPS added.
 * 
 * 64    11-09-07 18:20 Chhamaa
 * Loopback activation for OP_MODE_COMMISSIONING_RF_TERMINATED added.
 * 
 * 63    10-11-24 16:16 Chhamaa
 * Activation for local RF loopback added.
 * 
 * 61    22.09.08 15:03 Chhamaa
 * Change merged from O4CV branch:
 * monitor_evaluateDataAlarmsAndWarnings() and monitor_evaluateNsdAlarms()
 * shifted from SUBCASE_7 to SUBCASE_6
 * 
 * 60    23.07.08 11:01 Chhamaa
 * FB/FF parameters added.
 * 
 * 59    15.05.08 15:28 Chhamaa
 * distributeConfiguration() modified: set leftPilotPresent in case of MOD
 * above speech.
 * 
 * 58    9.05.08 18:34 Chhamaa
 * Spectrum positions for new DPLC BW 2, 6 kHz added.
 * Test parameter for OFDM DSP added.
 * 
 * 56    23.07.07 16:53 Chtognu
 * memcopy of NSK adaptive Equalizer structs added
 * 
 * 55    22.03.07 10:27 Chhamaa
 * Load re-balancing due to low headroom in subtask 9 : function
 * monitor_evaluateTopLevelAlarms split into monitor_delayLowLevelAlarms
 * and  monitor_evaluateTopLevelAlarms
 * 
 * 54    17.01.07 17:49 Chhamaa
 * Function sendCurrentDataRateToO4CV() added.
 * 
 * 52    14.11.06 13:46 Chpeplu
 * Function distributeConfiguration() changed for custom mode
 * 
 * 51    18.09.06 16:31 Chhamaa
 * Manual alarm reset for AES added.
 * 
 * 50    28.08.06 16:41 Chchlee
 * detectR1LA() introduced to free internal RAM
 * 
 * 49    28.08.06 15:32 Chchlee
 * detectR1LA() introduced to free internal RAM
 * 
 * 48    25.08.06 9:13 Chchlee
 * Code to find out if R1LA is plugged added to subcase 4
 * 
 * 46    4.04.06 18:30 Chmibuh
 * more reliable access of internal TDM bits
 * 
 * 45    29.03.06 11:23 Chmibuh
 * new function handle_R1LA_reset
 * 
 * 44    17.03.06 10:50 Chmibuh
 * init_spiManager(internalTDM_Rx_Ptr, internalTDM_Tx_Ptr) instead of
 * init_spiManager(void)
 * 
 * 42    3.11.05 16:58 Chchlee
 * resetP4LT if we could not distribute the configuration
 * 
 * 41    5.10.05 17:47 Chchlee
 * Booting: fit into template
 * 
 * 40    4.10.05 17:14 Chchlee
 * iButton Alarm suppression added
 * 
 * 39    28.09.05 14:45 Chchlee
 * isInDebugMode() moved to IO_BUFFER.H, ASSERT() for distribute
 * configuration deactivated in debug mode
 * 
 * 38    27.09.05 7:14 Chchlee
 * distributeConfiguration() is called only if not in debug mode
 * 
 * 37    27.09.05 6:05 Chchlee
 * distributeConfiguration now reboots board on failure
 * 
 * 36    1.09.05 15:29 Chchlee
 * Comments added
 * 
 * 35    22.07.05 8:17 Chchlee
 * Bug 72 fixed be rearanging activation of monitor functions
 * 
 * 34    21.07.05 13:51 Chchlee
 * eventRec_EmptyStack call moved to resetP4LT
 * 
 * 33    15.07.05 18:39 Chmibuh
 * 
 * 32    13.07.05 8:44 Chchlee
 * equipmentAliveUpCounter removed
 * 
 * 31    11.07.05 19:42 Chmibuh
 * doSlowMuteBridge(); moved from SUBCASE_6 to SUBCASE_1 (load balance)
 * 
 * 30    30.06.05 8:14 Chchlee
 * cleaned up
 * 
*---------------------------------------------------------------------------*/

/*

	USE_LED_LIFESIGN compiler switch: if defined, the blinking LEDs are active, if not 
	                                  the LEDs are driven by the alarm monitoring

*/


/* INCLUDE FILES *************************************************************/

#include <signal.h>
#include <def21161.h>
asm("#include <def21161.h>");

#include "basetype.h"
#include "idleCounter.h"
#include "master_dsp_main.h"
#include "constant.h"
#include "Timer.h"
#include "Init_Master_DSP.h"
#include "IO_HW_BUFFER.H"
#include "ExternalTDM_BusBridge.h"
#include "monitor.h"
#include "PilotController.h"

#include "diag.h"
#include "bridge.h"
#include "dl_rs232.h"

#include "configuration.h"
#include "status.h"
#include "tdm_service.h"

#include "tdmEocData.h"
#include "tdmUpConvSettings.h"

#include "msgHandler.h"

#include "eprommed.h"
#include "tdm_int_types.h"

#include "Booting.h"

#include "FE_DSP_Types.h"
#include "UC_DSP_Types.h"
#include "OFDM_DSP_Types.h"
#include "NSK_DSP_Types.h"
#include "NSD_DSP_Types.h"

#include "SPI_functions.h"
#include "rtc.h"
#include "IrigB.h"

#include "AD9833_Generator.h"

#include "boost_manager.h"

#include "edgeDetection.h"

#include "Equalizer.h"

#include "MemoryCheck.h"

#include "iButHigh.h"
#include "licenceCheck.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// number of subcases per minute on this DSP
#define NR_SUBCASES_PER_MINUTE  (60 * ((DSP_CLK_INT_Hz/DSP_CYCLES_PER_MAINLOOP) / \
                                (N_MAINCASES_10K * N_SUBCASES_10K)))

#define N_RETRIES_FOR_CFG_DISTRIBUTION 20

#define E_WIRE_ACTIVE_PROLONGATION	30000	//[s*1e-3] 30s
#define M_WIRE_ACTIVE_PROLONGATION	30000	//[s*1e-3] 30s


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx and tx pointers
#define g_intTDM_RxStruct_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr)
#define g_intTDM_TxStruct_Ptr  ((t_Master_output_tdm_block*)internalTDM_Tx_Ptr)

/* LOCAL VARIABLES ***********************************************************/

static t_P4LT_Config  * g_pCfg;
static t_P4LT_Status  * g_pStatusData;

static t_TestPlcLinkData    * g_pTestPlcLinkCfg;

static BOOL                 g_tdmServiceConfirmCalled;
static e_TDMServiceResult   g_TDMServiceResult;
static e_TDMServiceMsgType  g_rxMessageType;

static unsigned long        g_nAPLCChannels;

static BOOL                 g_licenseIsOk;

/* LOCAL FUNCTIONS ***********************************************************/

static BOOL distributeConfiguration(t_P4LT_Config  * cfg_P4LT);

static BOOL sendConfiguration(e_TDMServiceAddr   destinationAddress, 
							  void             * configuration,  
							  unsigned long      configurationSizeInLongs);

static void iConfirm (e_TDMServiceResult  result, 
					  e_TDMServiceMsgType messageType,
					  unsigned long       rxMsgSize);

static BOOL handle_R1LA_reset(void);
static void detectR1LA(void);


/* MAIN **********************************************************************/

extern volatile long *externalTDM_Tx_Ptr;
extern volatile long *externalTDM_Rx_Ptr;
extern volatile long *internalTDM_Tx_Ptr;
extern volatile long *internalTDM_Rx_Ptr;

/* IMPLEMENTATION ************************************************************/

//	unsigned long year,result;

void main (void)
{
	BOOL bSuccess;

	// receive buffer for EOC data
	static EocDataBufferType rxBufferEoc[N_APLC_CHANNELS];
	
	// transmit buffer for EOC data
	static EocDataBufferType txBufferEoc[N_APLC_CHANNELS];

	// variables for edgeDetection on e- and m-wire
	static t_EdgeDetection	edgeDetParamEWire[N_APLC_CHANNELS];
	static t_EdgeDetection	edgeDetParamMWire[N_APLC_CHANNELS];

	static BOOL EWireActive[N_APLC_CHANNELS];
	static BOOL MWireActive[N_APLC_CHANNELS];

	// for synchronizing NSD and Master DSP for extended time slots, communication with G4AI in system
	static unsigned long g_syncCounterForNSD_DSP=0;

	static BOOL doR1LA_reset;
	static BOOL isSPIbus_requested;
	static BOOL isSPIbus_released;

	static unsigned long basicTimeSlot6;


	Init_DSP_Register();	// this must be the first function call!
	asm("nop;");			// Emulator: run up to here and reload *.dxe again (SDRAM settings)

	// init SPORT DMA pointers - acessing the DMA pointer is possible anytime - no more: if (internalTDM_Tx_Ptr) ....
	Init_InternalTDM_BufferPtr((unsigned long*) internalTDM_Rx_Ptr, (unsigned long*) internalTDM_Tx_Ptr);
	Init_ExternalTDM_BufferPtr((unsigned long*) externalTDM_Rx_Ptr, (unsigned long*) externalTDM_Tx_Ptr);

	MemoryCheckStartupSHARC(TRUE);		// TRUE = SDRAM support

	configuration_init(); // construct the singleton "configuration"
	status_init();        // construct the singleton "status"
	
	booting_bootAllDSPs();

	initInternalTime();

#ifndef P4LW_PTTY2		
	if(iButH_init_iButton())	// use iButton not for Prototype 2; use it for 0-Serie onward
	{
		iButH_is_iButton_valid();
	}
#endif

	initialize_DSP();

	RestartTimer10k();
	IR_TimerCounter = 1;
	
	reestablishSPI();		// workaround for DSP anomaly 47

	g_pCfg = configuration_getPCurrentConfig();

	CNT_APLC_CHANNELS (g_pCfg->APLC, g_nAPLCChannels); // get the number of active APLC channels

	g_pStatusData = status_getPStatus();

	g_pTestPlcLinkCfg = status_getPTestSwitches();

	// calculation of equalizer coefficients has to be done before distributeConfiguration()
	equalizer_init(APLC_CH1);
	equalizer_init(APLC_CH2);
	equalizer_init(APLC_CH3);

    g_licenseIsOk = licence_match_config(iButH_getLicenceCode(), licence_read_config(g_pCfg));


	// Caution: "distributeConfiguration()" must be called BEFORE "bridge_init()", 
	//          because TDM serivce is re-initialized (new call-backs)
	bSuccess = distributeConfiguration(g_pCfg); 

	// Reboot if we could no distribute the configuration. In debug mode we don't care	
	if (!bSuccess && !isInDebugMode())
	{                
		resetP4LT(); 
		// does not return from resetP4LT
	}

	bridge_init();

	initAD9833_DDS(g_pCfg);

	irigB_init(g_pCfg);

	init_spiManager();

	Init_PilotController();

	// initialize data transfer on internal TDM bus for all three EOC channels
	tdmEocData_init(rxBufferEoc, txBufferEoc);

	// initialize up-converter settings to send on internal TDM
	tdmUpConvSettings_initSender(g_pCfg->system.RF.txFrequency, g_pCfg->system.RF.channelMode);

	// initialize edge detection on E-wire
	edgeDetection_init(&edgeDetParamEWire[APLC_CH1], E_WIRE_ACTIVE_PROLONGATION);
	edgeDetection_init(&edgeDetParamEWire[APLC_CH2], E_WIRE_ACTIVE_PROLONGATION);
	edgeDetection_init(&edgeDetParamEWire[APLC_CH3], E_WIRE_ACTIVE_PROLONGATION);

	// initialize edge detection on M-wire
	edgeDetection_init(&edgeDetParamMWire[APLC_CH1], M_WIRE_ACTIVE_PROLONGATION);
	edgeDetection_init(&edgeDetParamMWire[APLC_CH2], M_WIRE_ACTIVE_PROLONGATION);
	edgeDetection_init(&edgeDetParamMWire[APLC_CH3], M_WIRE_ACTIVE_PROLONGATION);

	monitor_init();  // initialize monitoring

	eventRec_init(); // initialize event recorder

	if (!g_licenseIsOk) // if no valid iButton, turn off event recorder after 
	{                   // storing ALM_License PLC hardware alarm
		eventRec_recordEvent(ALARM_EVENT_PLC_HW_ALM, ALM_License|ALM_Event_Recorder_disabled, FALSE);		
		eventRec_EmptyStack();
		eventRec_enable(FALSE);         // disable eventrecorder forever
	}

    // main loop 
    while (1) 
	{
		static unsigned long	idleCounterMinIndex;
		static unsigned long	idleCounter;

		static unsigned long	mainCaseCounter;

		unsigned long startValue = count_start();

		// repetiton rate = 10.667 kHz
		while(IR_TimerCounter && IR_BlockCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		idleCounter = count_end(startValue);
		idleCounter_update(idleCounterMinIndex, idleCounter);
		
		// Counter for NSD_DSP Sync bit
		g_syncCounterForNSD_DSP++;
		g_intTDM_TxStruct_Ptr->NSD_Sync = g_syncCounterForNSD_DSP;

		g_intTDM_TxStruct_Ptr->flags.reset_R1LA = doR1LA_reset;

		RestartTimer10k();
		IR_BlockCounter = 1;
		IR_TimerCounter = 1;

		ExternalTDM_HandlerRx();
				
		ExternalTDM_HandlerTx((g_syncCounterForNSD_DSP & BIT0));

		tdmServiceActivateInitiator ((unsigned long *) internalTDM_Rx_Ptr, (unsigned long *) internalTDM_Tx_Ptr);
		boostManager_activate       (internalTDM_Rx_Ptr, internalTDM_Tx_Ptr);

		PilotController(internalTDM_Rx_Ptr, internalTDM_Tx_Ptr);

		// EOC data transfer on internal TDM bus
		tdmEocData_transfer(&g_intTDM_RxStruct_Ptr->EOC_RxData_Channel_1_2_3_TS29, &g_intTDM_TxStruct_Ptr->eoc_tx);

		// send up-converter settings to UC DSP
		tdmUpConvSettings_send(&g_intTDM_TxStruct_Ptr->upConvSettings);

		// get basic time slot number 6 to detect reset of G4AI at rightmost position
		receiveBasicTimeSlot6(&basicTimeSlot6);

		g_intTDM_TxStruct_Ptr->flags.requestSPIbusAllowed = isSPIbus_released;

		isSPIbus_requested = g_intTDM_RxStruct_Ptr->UpConverter_DSP_AlarmStatusData_TS20.requestSPIbus;

		equalizer_tdmDataTransfer();

		equalizer_measureChannel(APLC_CH1);
		equalizer_measureChannel(APLC_CH2);
		equalizer_measureChannel(APLC_CH3);

		{
			// make a 5.33kHz task
			static BOOL toggle = FALSE;

			if(toggle = !toggle)
			{
				// serve the HMI port
				ToDataLinkLayerRS232();           

				// serve the FT1.2 bridge
				bridge_activate(); 

				eventRec_writeEventsToEprom();
			}
			else
			{
				monitor_recordTxCmdEvents();
                monitor_recordRxCmdEvents();
			}
		}

		FromDataLinkLayerRS232();
		
		irigB_activate();

		convert_time_formats();
		
		isSPIbus_released = spiManager(isSPIbus_requested);

		// Real time function to supervise external TDM bus
		Check_SPORT_TDM_Bus();

		switch(mainCaseCounter)
		{
			// WARNING: don´t change number of cases
			// repetiton rate of case functions = 1.067 kHz

			case MAINCASE_0:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_0;

				// repetiton rate = 1.067 kHz	
				
				// ***************************************
				// ***         read IO-Buffer          ***
				// ***************************************
				IO_BUFFER_READ = *(long*)IO_BUFFER_DSP;

				doR1LA_reset = handle_R1LA_reset();	// Do not move to subcase without adapting RESET_DELAY_COUNTER

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_1:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_1;

				// ***************************************
				// ***       write to IO-Buffer        ***
				// ***************************************
				*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE; // write output buffer
				CheckExternalTDM_Bus();
				
				DoMemoryCheck(TRUE);		// TRUE = SDRAM support
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_2:

				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_2;
					
				eprom_activate(); // ran with 300Hz in ETL500
			
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_3:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_3;
				
				E_and_M_WireBridge();

				// Restarts external TDM Bus if no slave card is plugged - new in ETL600 a valid case
				CheckAndRestartExternalTDM_Bus(g_pStatusData);

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_4:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_4;

				E_and_M_WireBridge();// called 2 times: workaround for writing all DMA Tx buffers
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_5:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_5;

				EWireActive[APLC_CH1] = edgeDetection_activate(g_intTDM_RxStruct_Ptr->E_WireSignallingAllChannels_TS28.channel1, 
															   &edgeDetParamEWire[APLC_CH1]);
				EWireActive[APLC_CH2] = edgeDetection_activate(g_intTDM_RxStruct_Ptr->E_WireSignallingAllChannels_TS28.channel2, 
															   &edgeDetParamEWire[APLC_CH2]);
				EWireActive[APLC_CH3] = edgeDetection_activate(g_intTDM_RxStruct_Ptr->E_WireSignallingAllChannels_TS28.channel3, 
															   &edgeDetParamEWire[APLC_CH3]);
				
				MWireActive[APLC_CH1] = edgeDetection_activate(g_intTDM_TxStruct_Ptr->mwire_tx.channel1, 
															   &edgeDetParamMWire[APLC_CH1]);
				MWireActive[APLC_CH2] = edgeDetection_activate(g_intTDM_TxStruct_Ptr->mwire_tx.channel2, 
															   &edgeDetParamMWire[APLC_CH2]);
				MWireActive[APLC_CH3] = edgeDetection_activate(g_intTDM_TxStruct_Ptr->mwire_tx.channel3, 
															   &edgeDetParamMWire[APLC_CH3]);
				
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_6:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_6;
				
				eventRec_activate();
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_7:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_7;
				
				eventRec_activateBlockRotationHandler();
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_8:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_8;
				
		        // Moving ApplicationLayer to this Maincase speeds up FW Download for more than 30 %!			
				ApplicationLayer(); 				
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_9:
			{
				static unsigned long	subCaseCounter;
			
				switch(subCaseCounter) {

					// WARNING: don´t change number of cases
					// repetiton rate of case functions = 0.1067 kHz
					
					case SUBCASE_0:	
					{
						static BOOL bFlag = FALSE;

						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_0;

						if (!bFlag)
						{
							idleCounter_reset();
							bFlag = TRUE;
						}

						monitor_evaluatePlcHwAlarms();

						equalizer_stateMachinePnController(APLC_CH1);
						equalizer_stateMachineAccController(APLC_CH1);

						equalizer_stateMachinePnController(APLC_CH2);
						equalizer_stateMachineAccController(APLC_CH2);

						equalizer_stateMachinePnController(APLC_CH3);
						equalizer_stateMachineAccController(APLC_CH3);
						
						subCaseCounter++;
						break;
					}	
					case SUBCASE_1:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_1;
					
						monitor_generateAlarmEvents();
						monitor_detectResetG4AI(basicTimeSlot6);

						doSlowMuteBridge();

						sendCurrentDataRateToO4CV();

						subCaseCounter++;
						break;
						
					case SUBCASE_2:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_2;
						
						MakeTxFastStatusTelEOC(APLC_CH1);
						MakeTxFastStatusTelEOC(APLC_CH2);
						MakeTxFastStatusTelEOC(APLC_CH3);

#ifndef USE_LED_LIFESIGN
						// inhibit P4LT reset if LEDs are used for lifesign display
						// This mode is used for the channel simulator
						monitor_evaluateLifesigns();
#endif
						subCaseCounter++;
						break;
						
					case SUBCASE_3:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_3;
						
						ToDataLinkLayerEOC(&txBufferEoc[APLC_CH1], MWireActive[APLC_CH1], APLC_CH1);
						ToDataLinkLayerEOC(&txBufferEoc[APLC_CH2], MWireActive[APLC_CH2], APLC_CH2);
						ToDataLinkLayerEOC(&txBufferEoc[APLC_CH3], MWireActive[APLC_CH3], APLC_CH3);
					

						subCaseCounter++;
						break;
						
					case SUBCASE_4:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_4;

						FromDataLinkLayerEOC(&rxBufferEoc[APLC_CH1], EWireActive[APLC_CH1], APLC_CH1);
						FromDataLinkLayerEOC(&rxBufferEoc[APLC_CH2], EWireActive[APLC_CH2], APLC_CH2);
						FromDataLinkLayerEOC(&rxBufferEoc[APLC_CH3], EWireActive[APLC_CH3], APLC_CH3);
						
						detectR1LA();

						subCaseCounter++;
						break;
						
					case SUBCASE_5:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_5;

						HandleSendNoReply(APLC_CH1);
						HandleSendNoReply(APLC_CH2);
						HandleSendNoReply(APLC_CH3);

						DL_EOC_Monitor();

						// Dummy load handling
						if ((IO_BUFFER_READ & TXRF_INPUT) && (g_pCfg->system.equipmentType == EQ_TYPE_ETL600_R3) ||
							(g_pTestPlcLinkCfg->testSwitches & TEST_SWITCH_LOCAL_RF_LOOPBACK_ON) ||
							(msgHandler_getOperationMode() == OP_MODE_COMMISSIONING_RF_TERMINATED))
						{
							ActivateAD9833();
						}
						else
						{
							DeActivateAD9833();
						}
						
						subCaseCounter++;
						break;
						
					case SUBCASE_6:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_6;

						monitor_evaluatePlcLinkAlarms();

						// 2x as fast as in ETL500
						msgHandler_executeDelayedMessageTypes();

						monitor_ifCRCerror_Reset();
						monitor_evaluateDataAlarmsAndWarnings();
						monitor_evaluateNsdAlarms();

						subCaseCounter++;
						break;
			
					case SUBCASE_7:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_7;

						monitor_checkTDMBus();
						
						subCaseCounter++;
						break;
						
					case SUBCASE_8:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_8;
	
						monitor_evaluatePlcIfAlarmsAndWarnings();
						
						{   // reset if test configuration counter expires
							static unsigned long subcasesPerMinuteCounter;

							// if value > remainingActiveTestConfigTime the time-bomb is enabled 
							if(g_pCfg->system.remainingActiveTestConfigTime > 0)
							{
								if (++subcasesPerMinuteCounter >= NR_SUBCASES_PER_MINUTE)
								{
									if(--g_pCfg->system.remainingActiveTestConfigTime > 0)
									{
										subcasesPerMinuteCounter = 0;
									}
									else // time-bomb expired --> reset board
									{
										eventRec_recordEvent(ALARM_EVENT_PREVIOUS_CONFIGURATION,0,FALSE);  
									
										resetP4LTandR1LA();		// The system is now running a few cycles before 
																// reset will occur
									}
								}
							}

						}

						monitor_delayLowLevelAlarms();

						subCaseCounter++;
						break;
					
					case SUBCASE_9:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_9;
						
#ifdef USE_LED_LIFESIGN // lifesign handling for test or channel simulator
						
						{
							static int ix;
							static unsigned long lifesignFE, lifesignUC, lifesignNSD, lifesignNSK, lifesignOFDM;
							       unsigned long lifesign;
							
							// toggle LED 
							if (ix++ == 8)
							{
								TOGGLE_LED(P4LT_AL_LED); // my own lifesign
								ix = 0;
							}

							lifesign = g_intTDM_RxStruct_Ptr->FE_DSP_AlarmStatusData_TS08.lifesign;
							
							if (lifesign != lifesignFE)
							{	// life sign has been updated since last time
								lifesignFE = lifesign; // remember new lifesign
								if (0==lifesign) TOGGLE_LED(READY_LED);
							}

							lifesign = g_intTDM_RxStruct_Ptr->UpConverter_DSP_AlarmStatusData_TS20.lifesign;
							
							if (lifesign != lifesignUC)
							{	// life sign has been updated since last time
								lifesignUC = lifesign; // remember new lifesign
								if (0==lifesign) TOGGLE_LED(SYS_AL_LED);
							}

							lifesign = g_intTDM_RxStruct_Ptr->NSD_DSP_AlarmStatusData_TS10.lifesign;
							
							if (lifesign != lifesignNSD)
							{	// life sign has been updated since last time
								lifesignNSD = lifesign; // remember new lifesign
								if (0==lifesign) TOGGLE_LED(WARNING_LED);
							}

							lifesign = g_intTDM_RxStruct_Ptr->NSK600_DSP_AlarmStatusData_TS18.lifesign;
							
							if (lifesign != lifesignNSK)
							{	// life sign has been updated since last time
								lifesignNSK = lifesign; // remember new lifesign
								if (0==lifesign) TOGGLE_LED(LINK_AL_LED);
							}

							lifesign = g_intTDM_RxStruct_Ptr->OFDM_DSP_AlarmStatusData_TS16.lifesign;
							
							if (lifesign != lifesignOFDM)
							{	// life sign has been updated since last time
								lifesignOFDM = lifesign; // remember new lifesign
								if (0==lifesign) TOGGLE_LED(HW_AL_LED_DSP);
							}
						}
#else
						// !!!Important!!!:
						// This function must be called after all other cyclically
						// activated monitor functions
						monitor_evaluateTopLevelAlarms(); // monitoring drives the LEDs
#endif
						subCaseCounter = 0;
						break;
						
					default:
						subCaseCounter = 0;
						break;
				}; // end switch(subCaseCounter)
					
				mainCaseCounter = 0;
				break;
			}
			default:
				mainCaseCounter = 0;
				break;
		}; // end switch(mainCaseCounter)
   }
}

static void iConfirm (e_TDMServiceResult    result, 
					  e_TDMServiceMsgType   messageType,
					  unsigned long         rxMsgSize)
{
	g_rxMessageType              = messageType;
	g_TDMServiceResult           = result;
	g_tdmServiceConfirmCalled    = TRUE; // set flag
	// ignore rxMsgSize
}


// send configuration to one particular DSP
static BOOL SEGMENT_EXT_CODE sendConfiguration(e_TDMServiceAddr destinationAddress,        // where to send
											   void           * configuration,			   // ptr to configuration to send
											   unsigned long    configurationSizeInLongs)  // size of configuration to send 
{
	int retries = -1; // retry counter

	g_tdmServiceConfirmCalled = FALSE; // reset flag

	// for debugging: give up after 50 retries (FE might not running)
	while (!g_tdmServiceConfirmCalled && (retries < N_RETRIES_FOR_CFG_DISTRIBUTION))
	{
		retries++;

		g_tdmServiceConfirmCalled = FALSE; // reset flag

		// send no data, receive data
		tdmServiceI_Request(  destinationAddress,                       // destination address
							  e_TDMServiceMsgType_Cfg_Send,             // txMsgType,
							  configuration, configurationSizeInLongs,  // tx message
							  NULL, 0,                                  // receive buffer (no reply expected)
							  TDM_SERVICE_TIMEOUT); 

		while (!g_tdmServiceConfirmCalled) // wait for confirm
		{
			// repetiton rate = 10.667 kHz
			while(IR_TimerCounter && IR_BlockCounter)
			{
				// wait until Timer is ringing or all Blocks has received.
				// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
			}

			RestartTimer10k();
			IR_BlockCounter = 1;
			IR_TimerCounter = 1;

			tdmServiceActivateInitiator ((unsigned long *) internalTDM_Rx_Ptr, (unsigned long *) internalTDM_Tx_Ptr);
		}

		if ((e_TDMServiceResult_Ok   != g_TDMServiceResult) ||
            (e_TDMServiceMsgType_Ack != g_rxMessageType))   // check if the correct reply was received
		{
			//force retry on time-out
			g_tdmServiceConfirmCalled = FALSE; // reset flag
		}
	}

	return (retries < N_RETRIES_FOR_CFG_DISTRIBUTION); // give up after N_RETRIES_FOR_CFG_DISTRIBUTION retries
}

// distribute configuration to FE, UC, NSD, NSK and OFDM DSP
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  Caveat:
//		Do not change the sequence of the DSPs unless you know what you are doing:
//      The UC DSP will use the SPI interface to initialize the DAC as soon as it has received the configuration.
//		After finishing with the configuration distribution, the master will regain control over the SPI!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
static BOOL SEGMENT_EXT_CODE distributeConfiguration(t_P4LT_Config  * pP4LTcfg)
{
	BOOL bSuccess = TRUE;

	tdmServiceI_Init(iConfirm);

	// ****** FE DSP ****************************
	{
		t_FrontEndDSP_Config    cfg;
		t_EqualizerFirCoef		*pEqFirCoef;


		pEqFirCoef = equalizer_getFirCoefPtr(APLC_CH1);

		// the marshalling yard
		// to					         from					         number of longs
		MEMCPY(&cfg.pilot,              &pP4LTcfg->APLC.pilot,           sizeof(cfg.pilot));
		MEMCPY(&cfg.equalizer,          &pP4LTcfg->APLC.equalizer,       sizeof(cfg.equalizer));
		MEMCPY(&cfg.equalizerFirCoef,    pEqFirCoef,					   sizeof(cfg.equalizerFirCoef));
		MEMCPY(&cfg.RF,                 &pP4LTcfg->system.RF,            sizeof(cfg.RF));
		MEMCPY(&cfg.rxAFLevelAdjust,    &pP4LTcfg->APLC.rxAFLevelAdjust, sizeof(cfg.rxAFLevelAdjust));
		        cfg.channelMode       =  pP4LTcfg->APLC.channelMode;
		        cfg.apFilterFlags     =  pP4LTcfg->APLC.apFilterFlags;
				cfg.intTeleprotMode   =  pP4LTcfg->ITS.mode;
				cfg.weightOFDM_Signal =  pP4LTcfg->OFDM.weight;
				cfg.DPLC_BW			  =  pP4LTcfg->OFDM.channelBW;
				cfg.rfPepMax		  =  pP4LTcfg->system.serviceData.rfPEP;

		cfg.AFC_Enabled = (pP4LTcfg->system.systemSettings & SYSTEM_SETTINGS_ENABLE_AFC) ? TRUE : FALSE;

		////////////////////////////////////////////////////////////////////////////////////
		////// Check if the Licence Code from iButton matches the configuration 
		////// If Licence is insufficient turn off the sender (cfg.RF.txLevelAdjustment = 0)
		if(!g_licenseIsOk)
		{
			cfg.RF.txLevelAdjustment = 0.0;
		}
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////

		if (!sendConfiguration (e_TDMServiceAddr_FE, &cfg, sizeof(cfg)))
		{
			bSuccess = FALSE;
		}
	}

	// ****** UC DSP ****************************
	{
		t_UpConverterDSP_Config    cfg;

		// to					     from					          number of longs
		MEMCPY(&cfg.RF,             &pP4LTcfg->system.RF,             sizeof(cfg.RF));
				cfg.rfPepMax	  =  pP4LTcfg->system.serviceData.rfPEP;

		if (!sendConfiguration (e_TDMServiceAddr_UC, &cfg, sizeof(cfg)))
		{
			bSuccess = FALSE;
		}
	}

	// ****** NSD DSP ****************************
	{
		t_NSD_DSP_Config    cfg;

		// to					from					      number of longs
		        cfg.mode =       pP4LTcfg->ITS.mode;
		MEMCPY(&cfg.NSD,        &pP4LTcfg->ITS.u_NSD_AES.NSD,           sizeof(cfg.NSD));
		MEMCPY(&cfg.AES,        &pP4LTcfg->ITS.u_NSD_AES.AES,           sizeof(cfg.AES));
		MEMCPY(&cfg.TDM_Config, &pP4LTcfg->system.TDM_Config, sizeof(cfg.TDM_Config));

		if (!sendConfiguration (e_TDMServiceAddr_NSD, &cfg, sizeof(cfg)))
		{
			bSuccess = FALSE;
		}
	}

	// ****** NSK DSP ****************************
	{
		t_NSK_DSP_Config    cfg;

		// to					from					      number of longs
		MEMCPY(&cfg.modem,      &pP4LTcfg->NSK.modem,       sizeof(cfg.modem));
		MEMCPY(&cfg.equalizer,  &pP4LTcfg->NSKEqualizerCfg, sizeof(cfg.equalizer));
		MEMCPY(&cfg.muxCfg,     &pP4LTcfg->muxCfg,			sizeof(cfg.muxCfg));
		MEMCPY(&cfg.ofdmCfg,    &pP4LTcfg->OFDM,			sizeof(cfg.ofdmCfg));

		if (!sendConfiguration (e_TDMServiceAddr_NSK, &cfg, sizeof(cfg)))
		{
			bSuccess = FALSE;
		}
	}

	// ****** OFDM DSP ****************************
	{
		t_OFDM_DSP_Config    cfg;

		// to					from					      number of longs
				cfg.mode      = pP4LTcfg->OFDM.mode;
				cfg.channelBW = pP4LTcfg->OFDM.channelBW;
		MEMCPY(cfg.dataRate,   &pP4LTcfg->OFDM.dataRate,     sizeof(cfg.dataRate));

		switch(pP4LTcfg->APLC.channelMode)
		{	 
			case APLC_CHANNEL_MODE_NONE:
			case APLC_CHANNEL_MODE_A1_20E_MAS:		// MOD above speech
			
				cfg.spectrumPosition = OFDM_SPECTRUM_POS_0HZ;
				break;
			
			case APLC_CHANNEL_MODE_A1_20E:
			case APLC_CHANNEL_MODE_A1_20I:
			case APLC_CHANNEL_MODE_A1_25E:
			case APLC_CHANNEL_MODE_A1_25I:
			case APLC_CHANNEL_MODE_A1_40E:
			case APLC_CHANNEL_MODE_A1_40I:
			case APLC_CHANNEL_MODE_A1_40I_A2_20E:	// MOD above speech

				cfg.spectrumPosition = OFDM_SPECTRUM_POS_4000HZ;

				break;

			case APLC_CHANNEL_MODE_A1_40I_A2_40E:
			case APLC_CHANNEL_MODE_A1_40I_A2_40I:
			case APLC_CHANNEL_MODE_A1_32I_A2_24E_A3_24E:
			case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E:	// MOD above speech

				cfg.spectrumPosition = OFDM_SPECTRUM_POS_8000HZ;
				break;
			
			case APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E:
			case APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I:

				cfg.spectrumPosition = OFDM_SPECTRUM_POS_12000HZ;
				break;

			default: 
				cfg.spectrumPosition = OFDM_SPECTRUM_POS_0HZ; // does no harm
				ASSERT(FALSE);
		};
		
		// @Pl_ext
		if ((APLC_CHANNEL_MODE_NONE                 == pP4LTcfg->APLC.channelMode)  ||
			(APLC_CHANNEL_MODE_A1_20E_MAS           == pP4LTcfg->APLC.channelMode)  ||
			(APLC_CHANNEL_MODE_A1_40I_A2_20E        == pP4LTcfg->APLC.channelMode)  ||
			(APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_20E == pP4LTcfg->APLC.channelMode)  ||
            (INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC  == (pP4LTcfg->ITS.mode & INT_TPE_MODE_MASK)))
		{
			cfg.leftPilotPresent = TRUE; // keep some space free for pilot
		}
		else
		{
			cfg.leftPilotPresent = FALSE; 
		}

		// set fall back/forward parameters
		cfg.burstErrorWindowSize  = BURST_ERROR_WINDOW_SIZE;
		cfg.fallBackWindowSize	  = FALL_BACK_WINDOW_SIZE;
		cfg.burstErrorProbLimit   = BURST_ERROR_PROB_LIMIT;
		cfg.fallBackProbLimit     = FALL_BACK_PROB_LIMIT;
		cfg.fallForwardSigmaLimit = FALL_FORWARD_SIGMA_LIMIT;

		// for testing
		cfg.phaseControlParam[0] = pP4LTcfg->OFDM.fReserved1;
		cfg.phaseControlParam[1] = pP4LTcfg->OFDM.fReserved2;
		cfg.phaseControlParam[2] = pP4LTcfg->OFDM.fReserved3;
		cfg.phaseControlParam[3] = pP4LTcfg->OFDM.fReserved4;

		if (!sendConfiguration (e_TDMServiceAddr_OFDM, &cfg, sizeof(cfg)))
		{
			bSuccess = FALSE;
		}
	}

	return bSuccess;
}



//***************************************************************************************
//	void handle_R1LA_reset(void)
//
//	Handles reset of R1LA (LANi)
//  The reset of R1LA will be done by NSK DSP. Therefore the NSK DSP has be informed about the reset.
//  This is done by TDM bus. (g_intTDM_TxStruct_Ptr->flags.reset_R1LA = TRUE)
//  The function distinguish between reset R1LA or R1LA & P4LT.
//  If reset R1LA only we need a reset pulse. (Clear bit after RESET_DELAY_COUNTER expired) otherwise the 
//  NSK DSP will reset R1LA permanently.
//  If reset R1LA & P4LT the Master DSP has to run for a few cycles (RESET_DELAY_COUNTER) 
//  before it resets itself. Otherwise no transfer via TDM bus will occur.
//
//	Return: TRUE if reset should be active else return FALSE
//****************************************************************************************
#define RESET_DELAY_COUNTER		100		// handle_R1LA_reset is called in MAINCASE_0


static BOOL SEGMENT_EXT_CODE handle_R1LA_reset(void)
{
	unsigned long tempMSGR4;
	static unsigned long reset_delay_ctr = 0;

	tempMSGR4 = *(long*)(MSGR4);
	asm("nop;");
	asm("nop;");

	if(tempMSGR4)	// any of the resets is set; do not waste calculation time if none of the resets are active
	{
		if(tempMSGR4 & RESET_FIRST_CALL)
		{
			*(long*)(MSGR4) &= ~(RESET_FIRST_CALL);			// clear RESET_FIRST_CALL
			reset_delay_ctr = RESET_DELAY_COUNTER;			// set reset delay counter
			return(TRUE);									// this will force an R1LA reset via NSK DSP
		}
		else
		{
			if(--reset_delay_ctr == 0)						// reset delay counter expired?
			{
				*(long*)(MSGR4) = 0;						// be prepared for the next reset

				if(tempMSGR4 == RESET_R1LA_ONLY)
				{
					return(FALSE);	// clear bit; if we reset R1LA only than we need a reset pulse
				}
				else if(tempMSGR4 == RESET_P4LT_AND_R1LA)
				{
					resetP4LT();
					return(TRUE);		// keep R1LA reset bit active
				}
			}
		}
		return(TRUE);		// keep R1LA reset bit active
	}
	return(FALSE);			// no R1LA reset
}




static void SEGMENT_EXT_CODE detectR1LA(void)
{   
/* Find out if R1LA plugged
   The R1LA is plugged if the bit "nskAlarms.LANNotBooted" from the NSK-DSP is 0 at startup.
   Later it will toggle to provide a lifesign.
   The state machine below makes sure that the NSK-DSP and the internal TDM bus
   is up and running. We consider it up and running when the NSK DSP lifesign 
   has chnaged twice. */

	static BOOL bDone; // execute only one

	if (!bDone)
	{
		static enum 
		{ 
			STATE_INITIAL,					// startup state
			STATE_FIRST_LIFESIGN_CHANGE, 
			STATE_SECOND_LIFESIGN_CHANGE
		} 
		state = STATE_INITIAL;

		static unsigned int previousLifesign;

		t_NSK_Alarm nskAlarms  = g_intTDM_RxStruct_Ptr->NSK600_DSP_AlarmStatusData_TS18;

		switch (state)
		{
			case STATE_INITIAL:
				previousLifesign = nskAlarms.lifesign; // remember lifesign
				state = STATE_FIRST_LIFESIGN_CHANGE;
				break;

			case STATE_FIRST_LIFESIGN_CHANGE:
				if (previousLifesign != nskAlarms.lifesign) // switch to next state if lifesign has changed
				{
					previousLifesign = nskAlarms.lifesign;
					state = STATE_SECOND_LIFESIGN_CHANGE;
				}
				break;

			case STATE_SECOND_LIFESIGN_CHANGE:
				if (previousLifesign != nskAlarms.lifesign)  // Done if lifesign has changed again
				{
					previousLifesign = nskAlarms.lifesign;

					if (!nskAlarms.LANNotBooted)	// now we are sure that NSK DSP is running
					{
						R1LAIsPlugged(); // set EQUIPMENT_CFG_R1LA_PLUGGED bit in the equipment SWID 
					}
					bDone = TRUE; // we are done. Never execute this code again.
				}
				break;

			default:
				ASSERT(FALSE);
		}
	}
}
