/*-------------------------------------------------------------------------*
 * $Workfile: NSK_DSP_Main.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : DSP main loop
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSK_DSP/C/NSK_DSP_Main.c $ 
 * 
 * 58    11-05-27 17:30 Chhamaa
 * R1LB reset pulse added.
 * 
 * 57    11-05-09 16:53 Chhamaa
 * Message handling for VoiceTransitStatus_Req and
 * VoiceTransitTestCfg_Send modified.
 * 
 * 56    11-04-19 13:43 Chhamaa
 * Test code added (transit register read).
 * 
 * 55    11-02-16 10:49 Chhamaa
 * Message handling for e_TDMServiceMsgType_MUXSetVoiceTransitTestCfg_Send
 * added.
 * 
 * 54    10-10-29 14:31 Chhamaa
 * Functions to read digital voice transit added.
 * 
 * 53    9.06.10 17:37 Chhamaa
 * Message handling for g_MUX_VoiceTransitDevStatus added.
 * 
 * 51    22.09.08 13:52 Chhamaa
 * 
 * 56    08-06-26 13:17 Chtognu
 * Firstloop with a startupdelay of 15 cycle before alarmindication
 * starts. 
 * 
 * 55    08-06-26 12:00 Chtognu
 * Firstloop with a startupdelay of 5 cycle before alarmindication starts.
 * 
 * 54    08-06-26 11:15 Chtognu
 * Firstloop with a startupdelay of 3 cycle again before alarmindication
 * (returned to old value). 
 * 
 * 53    08-06-26 9:52 Chtognu
 * Modem initialization spread to more cases to save computing time.
 * Firstloop with a startupdely of 1 cycle again before alarmindication. 
 * 
 * 52    08-06-05 9:42 Chsteic
 * '- changed firstloop with a startupdelay of 3 cycles
 * - bug fixed in nskDSPLoggerUpdate and nskDSPLoggerReset
 * 
 * 51    08-05-07 14:42 Chtognu
 * update functions moved to external memory, Rx Signal Alarm adapted
 * 
 * 50    08-05-02 15:44 Chsteic
 * - inserted nskDSPLoggerUpdate and nskDSPLoggerRest function to save
 * memory space
 * - removed unused status data logging
 * 
 * 49    08-04-25 15:51 Chsteic
 * '- changed signalQuality reset value to 0
 * 
 * 48    08-04-24 9:24 Chsteic
 * '- inserted counterDCDoffMax
 * - logging max signalQuality in NSK_ShortStatus
 * 
 * 47    08-04-11 13:58 Chsteic
 * '- logging of the DCDoff counter
 * - removed "'demodulate the same signal in modem 0 and modem2"
 * 
 * 46    08-04-08 14:10 Chsteic
 * -demodulate teh same signal in modem 0 and modem2,
 * nskRx[1] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[1]
 * -logging of Rdd
 * 
 * 45    08-04-03 9:43 Chtognu
 * txFormSymbolDAPSK() moved to external memory. Unused variables removed.
 * ad.eq: - new possibility added to seperate AST from ad.eq. Algorithmus
 * (DAPSK_AST_BEFORE_EQ) --> and seperated
 * - DAPSK_SAMPLE_STEP input for AST removed. 
 * - Update CMA back to 0.03
 * 
 * 44    08-03-20 14:29 Chsteic
 * '-added new parameters in NSK_ShortStatus array
 * 
 * 43    08-03-07 16:54 Chsteic
 * '- #define DAPSK_DETE_MIN_POWER_RCV        0.1
 * - test Flags added
 * - new algorithm
 * - variables renamed
 * - logging of new variables
 * - changed memory map
 * 
 * 41    08-03-05 11:11 Chsteic
 * ' -inserted logging of realTransmitSignalabsMax,pRcvMin
 *  -minor changes in EQUpdate_Flag logic
 * 
 * 40    08-03-04 10:31 Chsteic
 * '-inserted EQUpdate_Flag. For developing issues used.
 * -inserted NSK_ShortStatus_float and  NSK_ShortStatus_long array
 * - inserted SignalPowerRMax,SignalPowerIMax, normalization_factorMax for
 * the logger
 * 
 * 39    08-02-28 16:59 Chsteic
 * '-tau inserted
 * -changend Testvar, tau is logged now
 * 
 * 38    08-02-28 9:40 Chsteic
 * - Removed SNRLevelMin from NSK_DSP_STATUS
 * 
 * - Changend  static unsigned long TestVar to  static long TestVar
 * 
 * - Changend  static unsigned long TestVar to  static long TestVar in
 * NSK_Types.h
 * 
 * - added parameter t_NSK_ShortStatus  *NSK_ShortStatus in function
 * statusDataDAPSK()"
 * 
 * 37    08-02-25 9:54 Chtognu
 * Merged changes from ad.equ.branch:
 * ad. Equalizer functionality incl. Logger, scrambler, new filter,
 * removed preamble detection, alarm, processing time optimization, global
 * interrupt disabled during DAPSK fir function, FE path expantion
 * 
 * 36    24.07.07 10:53 Chhamaa
 * TDM_SERVICE_RX_BUF_SIZE increased to 120 in order to receive larger
 * configuration data structure.
 * 
 * 33    1.06.06 9:13 Chalgyg
 * Flag0 handling disabled, since it is not used anymore.
 * 
 * 31    29.03.06 11:36 Chmibuh
 * new function handle_R1LA_resetNSK()
 * toggle of FLAG1 (debug Flag) removed
 * 
 * 30    10.03.06 14:27 Chalgyg
 * Function NSK_FPGA_RD_OSR now directly writes to struct t_NSK_Alarm.
 * 
 * 28    26.10.05 13:11 Chtognu
 * comments added
 * 
 * 27    11.10.05 14:51 Chalgyg
 * MOD600 datarate change EMC issue corrected.
 * 
 * 26    11.10.05 14:25 Chtognu
 * debug code removed
 * 
 * 25    10.10.05 18:50 Chtognu
 *  getConfiguration() now SEGMENT_EXT_CODE
 * 
 * 24    26.09.05 18:04 Chtognu
 * DAPSK signal power bug fixed (oscillator agc)
 * 
 * 23    16.09.05 14:59 Chtognu
 * SNRcalculationFSK() moved to maincase
 * 
 * 22    12.08.05 9:00 Chtognu
 * bug 172 fixed initmodems() now using modem enable instead of port
 * enable 
 * 
 * 21    6.06.05 14:49 Chmibuh
 * putPiggyBackInfoToMaster(); added
 * 
 * 20    25.05.05 16:28 Chalgyg
 * - No MUX Alarms are produced, if FPGA is not loaded.
 * - Distribute calculation time for OFDM baudrate switching in 3 seperate
 * cycles.
 * 
 * 19    25.05.05 14:19 Chtognu
 * computing time tests added
 * 
 * 18    19.05.05 13:01 Chalgyg
 * Subprint_Error Alarm added.
 * 
 * 17    19.05.05 11:16 Chmibuh
 * memoryCheckRespondToMaster();  added
 * dummy memoryCheck removed
 * 
 * 16    18.05.05 15:56 Chmibuh
 * Memory Check enabled
 * 
 * 15    17.05.05 16:14 Chalgyg
 * - Alarm bugs removed.
 * 
 * 14    17.05.05 13:22 Chmibuh
 * Memory Check temporary disabled
 * 
 * 13    13.05.05 15:03 Chmibuh
 * MemoryCheck after Init_TDM_BufferPtr
 * 
 * 12    12.05.05 14:05 Chmibuh
 * crc check added
 * 
 * 11    12.05.05 11:53 Chalgyg
 * MUX600 alarms added.
 * 
 * 10    12.05.05 9:19 Chtognu
 * datasensitive mode ok, structure changed, alarm added
 * 
 * 9     29.03.05 8:10 Chtognu
 * functions optimized and fsk clockregenerator added 
 * 
 * 8     2.03.05 9:54 Chtognu
 * statusDataFSK() commented out
 * 
 * 7     17.02.05 13:59 Chtognu
 * merged Version with changes done in Prototype 1 till 16.02.05
 * 
 * 64    16.02.05 13:43 Chtognu (!!! PROTOTYPE1 !!!)
 * Structure changed, executeModems() and initModems() changed
 * 
 * 63    8.02.05 10:42 Chtognu (!!! PROTOTYPE1 !!!)
 * ArrayFloatCheck() added
 * 
 * 62    4.02.05 16:07 Chtognu (!!! PROTOTYPE1 !!!)
 * one working 9600Bd DAPSK Modem
 *
 * 6     16.02.05 10:40 Chalgyg
 * FPGA Interface changed to addressbus mode.
 * 
 * 5     15.02.05 10:10 Chbrbus
 * Changed interrupt routine - TDM buffer pointers all time valid in DMA
 * buffer range
 * 
 * 4     10.02.05 9:48 Chalgyg
 * FPGA OSR register read enabled.
 * 
 * 3     1.02.05 11:56 Chalgyg
 * NSK_FPGA_WR_RSIG() changed.
 * 
 * 2     27.01.05 16:30 Chalgyg
 * - OSR register read funtion removed, due to too much calculation time.
 * - Test struct removed.
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 61    14.01.05 10:55 Chalgyg
 * Alarm handling from FPGA loader implemented.
 * 
 * 60    12.01.05 13:39 Chalgyg
 * FPGA OSR register read function added.
 * 
 * 59    12.01.05 10:34 Chalgyg
 * FPGA initialisation moved to NSK_FPGA.c
 * 
 * 58    10.01.05 10:33 Chalgyg
 * OFDM bitrate watching implemented.
 * 
 * 57    7.01.05 14:57 Chtognu
 * functions for new FPGA (05-01-05)
 * 
 * 56    20.12.04 12:47 Chchlee
 * 
 * 55    17.12.04 13:14 Chhamaa
 * e_TDMServiceMsgType_StatusData_Req added
 * 
 * 54    17.11.04 20:55 Leeb02
 * g_niReceiveFlag bug fixed
 * 
 * 53    15.11.04 9:27 Leeb02
 * Function call in ASSERT() removed
 * 
 * 52    12.11.04 16:59 Maag01
 * Ack for configuration data is sent in getConfiguration()
 * 
 * 51    3.11.04 17:09 Maag01
 * case e_TDMServiceMsgType_Cfg_Send added
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include <signal.h>
#include <def21161.h>

asm("#include <def21161.h>");
#include "basetype.h"
#include "idleCounter.h"
#include "FPGAHandling.h"
#include "NSK_FPGA.h"
#include "Timer.h"
#include "Init_NSK_DSP.h"
#include "NSK_DSP_Main.h"
#include "constant.h" // not sure if i'm using it still -> check later 
#include "NSK_DSP_Types.h"
#include "NSK_DSP_Constant.h"
#include "FSK.h"
#include "DIAG.h"
#include "tdm_int_types.h"
#include "TDM_Service.h"
#include "DAPSK.h" // temporarily commented out by @CL@
#include <stdio.h> // printf
#include <math.h>   // sqrtf() for Carrierdetection


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define TDM_SERVICE_RX_BUF_SIZE		120
#define TDM_SERVICE_TX_BUF_SIZE		100
#define DAPSK_MODULATION_BOOTDELAY	150000  // original
#define DAPSK_STARTUP_DELAY			15
// test ProcessingPowerNoSubCases
//#define MAX_COUNTER_REALTIMEPART	7000	// 7000 --> max 75% processing load used in real part
/* LOCAL TYPE DECLARATIONS ***************************************************/

// Config
static t_NSK_DSP_Config		g_NSK_DSP_Config;

// Status & alarm
static t_NSK_DSP_Status		g_NSK_DSP_Status;
static BOOL g_nskLinkAlarm[N_NSK_MODEMS];		
static BOOL g_nskDCDAlarm[N_NSK_MODEMS];
static BOOL g_nskJabberHalt[6];
static BOOL FPGA_Not_Loaded;
static BOOL Subprint_Error;

static t_MUX_VoiceTransitDevStatus	g_MUX_VoiceTransitDevStatus;
static t_MUX_VoiceTransitTestCfg	g_MUX_VoiceTransitTestCfg;

// FSK
static t_FSK_TxTiming           g_FSKTxTimingModem[N_NSK_MODEMS];       //
static t_FSK_Modulator          g_FSKModulatorModem[N_NSK_MODEMS];      // 
static t_FSK_Demodulator        g_FSKDemodulatorModem[N_NSK_MODEMS];    //
static t_FSK_Carrier            g_FSKCarrierDemodulator[N_NSK_MODEMS];  //
static t_FSK_Clockreg           g_FSKClockregenerator[N_NSK_MODEMS];    //

//DAPSK
static t_DAPSK_Modulator        g_DAPSKModulator[N_NSK_MODEMS];
static t_DAPSK_InputConversion 	g_DAPSKInputConversion[N_NSK_MODEMS];
static t_DAPSK_Demodulator      g_DAPSKDemodulator[N_NSK_MODEMS];
static t_DAPSK_OutputConversion	g_DAPSKDemodulatorConversion[N_NSK_MODEMS];
// DAPSK Test Data ------------------------------------------------------------------------------------------------------
static unsigned long startValueTestCounter;
static unsigned long idleValueTestCounter;
static unsigned long idleValueMINTestCounter;

static unsigned long startValueTestCounter1;
static unsigned long idleValueTestCounter1;
static unsigned long idleValueMINTestCounter1;

static unsigned long startValueTestCounter2;
static unsigned long idleValueTestCounter2;
static unsigned long idleValueMINTestCounter2;

static unsigned long startValueTestCounter3;
static unsigned long idleValueTestCounter3;
static unsigned long idleValueMINTestCounter3;

// DAPSK Test Data end ---------------------------------------------------------------------------------------------------

// TDM
static float			        *p_TxDataDSP; // looping for test reasons
static float					TDMModDataOut[N_NSK_MODEMS];
static float					TDMModDataOutMatrix[N_NSK_MODEMS][N_APLC_CHANNELS];
static float					TDMModDataOutMatrixNoAP[N_NSK_MODEMS][N_APLC_CHANNELS];
static float					*p_TDMDemodDataIn[N_NSK_MODEMS];

static int DelayDAPSKModulation;
// Configuration Flags
static BOOL NSKPortEnable[N_NSK_MODEMS]; // TRUE = Port is enable
static BOOL NSKMuxmode[N_NSK_MODEMS]; // TRUE = Channel in Mux Mode
static BOOL NSKmodemType[N_NSK_MODEMS]; // TRUE = DAPSK-Modulation, FALSE = FSK-Modulation or no modem
static BOOL RTSalwaysActive[N_NSK_MODEMS]; // TRUE = RTS always active
static BOOL NSKclockregeneratorOn[N_NSK_MODEMS]; // TRUE = Clockregenerator active

static BOOL channelDisconnectable[N_NSK_MODEMS]; // TRUE = Cannel disconnectable
static BOOL disconnectModem = FALSE; // control variable from TDM -> True = disconnect


static t_FPGA_NSK_PCR      g_FPGA_PCR;             ///< Port Configuration Register
static t_FPGA_NSK_RSIG     g_FPGA_RSIG;            ///< Modem CTS and DCD Values for FPGA (RSIG register)
static t_FPGA_NSK_RTS      g_FPGA_RTS;             ///< RTS read from FPGA

static t_FPGA_Registers	   g_FPGA_Addresses;	   ///< FPGA register address range
static t_FPGA_Registers	   g_FPGA_Registers;	   ///< FPGA register content

// for testing
//static t_FPGA_Registers  g_FPGA_RegistersTestRD; ///< FPGA register content read test
	

// Internal TDM bus buffer pointers

extern volatile long *internalTDM_Rx_Ptr;
extern volatile long *internalTDM_Tx_Ptr;
extern volatile long *externalTDM_Rx_Ptr;
extern volatile long *externalTDM_Tx_Ptr;

// TDM service variables
static unsigned long        g_niRxData[TDM_SERVICE_RX_BUF_SIZE];	///< receive buffer
static unsigned long		g_niTxData[TDM_SERVICE_TX_BUF_SIZE];	///< send buffer
static BOOL                 g_niReceiveFlag;
static e_TDMServiceMsgType	g_niRxMsgType;
static unsigned long	    g_niRxMsgSize;
// Alarm
// NSKalarm
static t_NSK_Alarm    g_NSK_DSP_Alarm = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// buffer for idle counters in case of TDM service request
static t_IdleCounters       g_idleCountersBuf;

static SEGMENT_EXT_DATA t_NSK_ShortStatus g_NSK_ShortStatus[N_NSK_MODEMS];

/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx and tx pointers
#define g_intTDM_RxStruct_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr)
#define g_intTDM_TxStruct_Ptr  ((t_NSK_output_tdm_block*)internalTDM_Tx_Ptr)


/* LOCAL VARIABLES ***********************************************************/

static t_OFDM_Alarm test;

/* LOCAL FUNCTIONS ***********************************************************/

static void SEGMENT_EXT_CODE writeStatusAndAlarm(int modemNo);
static void SEGMENT_EXT_CODE updateOscillator(int modemNo);

static void getConfiguration(t_NSK_DSP_Config *config);
static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize);
static void niConfirm(void);

static void SEGMENT_EXT_CODE initModems(t_NSK_DSP_Config g_NSK_DSP_Config, float TDMModDataOut[N_NSK_MODEMS],
										float TDMModDataOutMatrix[N_NSK_MODEMS][N_APLC_CHANNELS], 
										float TDMModDataOutMatrixNoAP[N_NSK_MODEMS][N_APLC_CHANNELS],
										float nskRx[N_APLC_CHANNELS],
										float nskRxNoEq[N_APLC_CHANNELS]);
static void executeModems(void);
// NSKalarm
static void SEGMENT_EXT_CODE nskDSPAlarmUpdate(t_NSK_Alarm* nskAlarm_Ptr,BOOL* nskLinkAlarm_Ptr, \
											   BOOL* nskDCDAlarm_Ptr, BOOL* nskJabberHalt_Ptr, \
											   BOOL FPGA_Not_Loaded,BOOL Subprint_Error);

void SEGMENT_EXT_CODE nskDSPLoggerUpdate(t_NSK_ShortStatus *pNSK_ShortStatus, long *processingOverload_Cntr);
void SEGMENT_EXT_CODE nskDSPLoggerReset(t_NSK_ShortStatus *pNSK_ShortStatus);

static void SEGMENT_EXT_CODE handle_R1LA_resetNSK(BOOL R1LA_reset);


/* testMatlab					
static t_Matlab_save SEGMENT_EXT_DATA	g_Matlab_save;
// testMatlab end */

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************/
/* MAIN                                                                      */
/*****************************************************************************/


void main (void)
{

	// idle counters
	static unsigned long startValue;
	static unsigned long idleCounter;
	static unsigned long idleCounterMinIndex;

	static unsigned long OFDM_FBFF_state = 0;
	static unsigned long old_OFDM_FBFF_state = 0x7;
	static unsigned long OSR_Counter = 0;
	static e_Case_Baudrate_Switch Case_To_Do = Case_OFDM_Port;
	static e_Case_Baudrate_Switch Case_New = Case_OFDM_Port;

	// test ProcessingPowerNoSubCases
	//static unsigned long startValueRealtimePart;
	//static unsigned long CounterRealtimePart;
	
	// case counters
	static unsigned long	mainCaseCounter;
	static unsigned long	subCaseCounter;

	static float nskTx[N_APLC_CHANNELS];
	static float nskRx[N_APLC_CHANNELS];

	static float nskTxNoAP[N_APLC_CHANNELS];
	static float nskRxNoEq[N_APLC_CHANNELS];

	static BOOL	doR1LA_reset;

	static BOOL startUp = TRUE;
	static int startUpCounter = 0;


	ASSERT(sizeof(t_NSK_AdapEquTF)   <= sizeof(g_niTxData)); // makes sure that t_NSK_DSP_AdapEquTF fits service data tx buffer
	ASSERT(sizeof(t_NSK_ShortStatus) <= sizeof(g_niTxData)); // makes sure that t_NSK_DSP_ShortStatus fits service data tx buffer

	Init_DSP_Register();
	asm("nop;");			// Emulator: run up to here and reload *.dxe again (SDRAM settings)


	// init SPORT DMA pointers - acessing the DMA pointer is possible anytime - no more: if (internalTDM_Tx_Ptr) ....
	Init_InternalTDM_BufferPtr((unsigned long*) internalTDM_Rx_Ptr, (unsigned long*) internalTDM_Tx_Ptr);
	Init_ExternalTDM_BufferPtr((unsigned long*) externalTDM_Rx_Ptr, (unsigned long*) externalTDM_Tx_Ptr);

	MemoryCheckStartupSHARC(TRUE);

	// terminate R1LB reset (set flag4 to high), reset pulse is started in Init_DSP_Register()
	*(long*)(IOFLAG) |= FLG4;	

	memoryCheckRespondToMaster(); 
	putPiggyBackInfoToMaster();

	// Load FPGA
	FPGA_Not_Loaded = FPGAHandling_ConfigFPGA();

	// Reset parallel port timing for FPGA
	asm("ustat1 = 0;");      
	asm("bit set ustat1 RBWST7|RBWA0|EB3WS2|EB3A0|EB0WS0|EB0A0;");			
    asm("dm(WAIT) = ustat1;");

	initialize_DSP();
	RestartTimer10k();
	IR_TimerCounter = 1;

	// Configuration of the "HMI" Data
	getConfiguration(&g_NSK_DSP_Config);
  
	////////////////////////////////////////////////////////////////////
	// Initialisation of DSP Modems:                                  //
	initModems(g_NSK_DSP_Config, TDMModDataOut, TDMModDataOutMatrix, 
		       TDMModDataOutMatrixNoAP, nskRx, nskRxNoEq);  // INITIALISATION OF DSP MODEMS    //
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// Initialisation of FPGA:                                        //
	Subprint_Error = initFPGA(&g_NSK_DSP_Status,&g_NSK_DSP_Config,&g_FPGA_Addresses,&g_FPGA_Registers);    // INITIALISATION OF FPGA          //
	////////////////////////////////////////////////////////////////////

	// for testing
	//NSK_FPGA_RD_VoiceTransitReg(&g_FPGA_Addresses, &g_FPGA_RegistersTestRD);

	////////////////////////////////////////////////////////////////////
	// Initialisation of short status                                //
	////////////////////////////////////////////////////////////////////
	
	// assumes that g_DAPSKDemodulator is already initialized!
	{
		int modemNumber;

		for (modemNumber = NSK_MODEM1; modemNumber <= NSK_MODEM4; modemNumber++) 
		{
			g_NSK_ShortStatus[modemNumber].modemNumber = modemNumber;

				// 0: NSK600 not assigned to any APLC channel
				// 1: NSK600 assigned to APLC channel 1
				// 2: NSK600 assigned to APLC channel 2
				// 3: NSK600 assigned to APLC channel 3
			g_NSK_ShortStatus[modemNumber].aplcChannel = 1 + ((g_NSK_DSP_Config.modem[modemNumber].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS) >> 17);

			g_NSK_ShortStatus[modemNumber].datarate = g_DAPSKDemodulator[modemNumber].baud;

			g_NSK_ShortStatus[modemNumber].centerFrequency = g_NSK_DSP_Config.modem[modemNumber].centerFrequency & MASK_16LSB;

			g_NSK_ShortStatus[modemNumber].adEqLength = g_DAPSKDemodulator[modemNumber].adEqLength;		
		}
	}

	// main loop ------------------------------------------------------------------------------------ 
    while (1) {

		static unsigned long myLifesign;

		static unsigned long program_Counter;
		static long processingOverload_Cntr;

		startValue = count_start();

		// repetiton rate = 10.667 kHz
		while(IR_TimerCounter && IR_BlockCounter )
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		if (startUp)
		{
			startUpCounter = startUpCounter+1;
			if(startUpCounter > DAPSK_STARTUP_DELAY)
			{
				startUp = FALSE;
				program_Counter = IR_Counter;
				processingOverload_Cntr=0;
			}
		}
		else
		{
			program_Counter = program_Counter+1;
			if (program_Counter != IR_Counter)
			{
				processingOverload_Cntr++;
				program_Counter = IR_Counter;
				ASSERT(FALSE);
			}
		}
		
		idleCounter = count_end(startValue);
		idleCounter_update(idleCounterMinIndex, idleCounter);

		// test ProcessingPowerNoSubCases
		// Start Counter for Processing Time counting
		//startValueRealtimePart  = count_start();

		RestartTimer10k();
		IR_BlockCounter = 1;
		IR_TimerCounter = 1;

		// Write Alarm Bits to the internal TDM bus
		// NSKalarm
		g_intTDM_TxStruct_Ptr->alarm = g_NSK_DSP_Alarm;		
		// Write lifesign to the internal TDM bus
		g_intTDM_TxStruct_Ptr->alarm.lifesign = myLifesign;

		// Write nsk tx signal to the internal TDM bus
		g_intTDM_TxStruct_Ptr->nsk_tx[0] = nskTx[0];			// ch1
		g_intTDM_TxStruct_Ptr->nsk_tx[1] = nskTx[1];			// ch2
		g_intTDM_TxStruct_Ptr->nsk_tx[2] = nskTx[2];			// ch3
		g_intTDM_TxStruct_Ptr->nsk_tx_noAP[0] = nskTxNoAP[0]; 	// ch1
		g_intTDM_TxStruct_Ptr->nsk_tx_noAP[1] = nskTxNoAP[1]; 	// ch2
		g_intTDM_TxStruct_Ptr->nsk_tx_noAP[2] = nskTxNoAP[2]; 	// ch3
		
		// Write Debug Words to the internal TDM bus!!! (for tests)
		g_intTDM_TxStruct_Ptr->debug_word_0 = 0xAffe1234;  // not used at the moment
		g_intTDM_TxStruct_Ptr->debug_word_1 = 0x5555AAAA;  // not used at the moment
		g_intTDM_TxStruct_Ptr->debug_word_2 = 0xFF00FF00;  // not used at the moment

		// Read nsk rx signal from internal TDM bus
		nskRx[0] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[0];		// ch1
		nskRx[1] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[1];		// ch2
		nskRx[2] = g_intTDM_RxStruct_Ptr->APLC_RxSignalEqualized[2];		// ch3
		
		nskRxNoEq[0] = g_intTDM_RxStruct_Ptr->APLC_RxSignalNotEqualized[0];	// ch1
		nskRxNoEq[1] = g_intTDM_RxStruct_Ptr->APLC_RxSignalNotEqualized[1];	// ch2
		nskRxNoEq[2] = g_intTDM_RxStruct_Ptr->APLC_RxSignalNotEqualized[2];	// ch3

		ArrayFloatCheck(nskRx, 3);
		ArrayFloatCheck(nskRxNoEq, 3);


		doR1LA_reset = g_intTDM_RxStruct_Ptr->MasterDSPOutputFlags_TS65.reset_R1LA;
					
		/* testMatlab					
		///////////////////////////////////////
		//test:
				nskRx[0] = nskTx[0];
				nskRx[1] = nskTx[1];
				nskRx[2] = nskTx[2];

				nskRxNoEq[0] = nskTxNoAP[0];
				nskRxNoEq[1] = nskTxNoAP[1];
				nskRxNoEq[2] = nskTxNoAP[2];
		///////////////////////////////////////
		// end testMatlab */

		// TDM bus service
		tdmServiceActivateNonInitiator((unsigned long *)internalTDM_Rx_Ptr, (unsigned long *)internalTDM_Tx_Ptr);

		/////////////////////////////////////////////////////////////////////////////
		// Read "Modem Diconnect Flag" from internal TDM bus                       //
		disconnectModem = g_intTDM_RxStruct_Ptr->BoostMgrOutput_TS07.disconnect;   //
		// TEST:  disconnectModem = FALSE;
		/////////////////////////////////////////////////////////////////////////////
		// Read from FPGA:                                                         //
		NSK_FPGA_RD_TUDn(&g_FPGA_Registers,&g_FPGA_Addresses);	// FPGA read of the 4 TUDn register       //
		NSK_FPGA_RD_TSIG(&g_FPGA_RTS,&g_FPGA_Addresses,&g_FPGA_Registers);			// Getting RTS from FPGA                  //


		/////////////////////////////////////////////////////////////////////////////
		// Write to TDM struct (nskTX[]):                                          //
		nskTx[0] = (TDMModDataOutMatrix[0][0]*TDMModDataOut[0]) +(TDMModDataOutMatrix[1][0]*TDMModDataOut[1]) +(TDMModDataOutMatrix[2][0]*TDMModDataOut[2]) + (TDMModDataOutMatrix[3][0]*TDMModDataOut[3]);
		nskTx[1] = (TDMModDataOutMatrix[0][1]*TDMModDataOut[0]) +(TDMModDataOutMatrix[1][1]*TDMModDataOut[1]) +(TDMModDataOutMatrix[2][1]*TDMModDataOut[2]) + (TDMModDataOutMatrix[3][1]*TDMModDataOut[3]);	
		nskTx[2] = (TDMModDataOutMatrix[0][2]*TDMModDataOut[0]) +(TDMModDataOutMatrix[1][2]*TDMModDataOut[1]) +(TDMModDataOutMatrix[2][2]*TDMModDataOut[2]) + (TDMModDataOutMatrix[3][2]*TDMModDataOut[3]);

		nskTxNoAP[0] = (TDMModDataOutMatrixNoAP[0][0]*TDMModDataOut[0]) +(TDMModDataOutMatrixNoAP[1][0]*TDMModDataOut[1]) +(TDMModDataOutMatrixNoAP[2][0]*TDMModDataOut[2]) + (TDMModDataOutMatrixNoAP[3][0]*TDMModDataOut[3]);
		nskTxNoAP[1] = (TDMModDataOutMatrixNoAP[0][1]*TDMModDataOut[0]) +(TDMModDataOutMatrixNoAP[1][1]*TDMModDataOut[1]) +(TDMModDataOutMatrixNoAP[2][1]*TDMModDataOut[2]) + (TDMModDataOutMatrixNoAP[3][1]*TDMModDataOut[3]);	
		nskTxNoAP[2] = (TDMModDataOutMatrixNoAP[0][2]*TDMModDataOut[0]) +(TDMModDataOutMatrixNoAP[1][2]*TDMModDataOut[1]) +(TDMModDataOutMatrixNoAP[2][2]*TDMModDataOut[2]) + (TDMModDataOutMatrixNoAP[3][2]*TDMModDataOut[3]);  
		//
		/////////////////////////////////////////////////////////////////////////////

		// test maxProcessingPower
		/*
		nskTxNoAP[0] = g_DAPSKModulator[0].outPolyphase.re;
		nskTxNoAP[1] = g_DAPSKModulator[0].outPolyphase.im;
		*/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Write to FPGA:                                                                                      //
		NSK_FPGA_WR_RUDall(&g_FPGA_Registers,&g_FPGA_Addresses);          // write the four RX - Transmitdata  //
		NSK_FPGA_WR_RSIG(&g_FPGA_RSIG,&g_FPGA_Registers,&g_FPGA_Addresses);    // write the four CTS Flags			   //
		NSK_FPGA_WR_PCR(&g_NSK_DSP_Status,&g_FPGA_PCR,&g_FPGA_Addresses,&g_FPGA_Registers);	  // write the four DCD Flags			   //

		// test maxProcessingPower
		/*
		g_DAPSKDemodulator[0].test_outPolyphase.re = nskRxNoEq[0];
		g_DAPSKDemodulator[0].test_outPolyphase.im = nskRxNoEq[1];
		g_DAPSKDemodulator[1].test_outPolyphase.re = nskRxNoEq[0];
		g_DAPSKDemodulator[1].test_outPolyphase.im = nskRxNoEq[1];
		g_DAPSKDemodulator[2].test_outPolyphase.re = nskRxNoEq[0];
		g_DAPSKDemodulator[2].test_outPolyphase.im = nskRxNoEq[1];
		g_DAPSKDemodulator[3].test_outPolyphase.re = nskRxNoEq[0];
		g_DAPSKDemodulator[3].test_outPolyphase.im = nskRxNoEq[1];
		*/
		/////////////////////////////////////////////////////////////////////////////
		// MODEM Funktions:                                                        //
		executeModems();                                                           //
		/////////////////////////////////////////////////////////////////////////////

		// test maxProcessingPower
		/*
		if(g_DAPSKDemodulator[0].reception != g_DAPSKDemodulator[3].reception)
		{
			g_DAPSKDemodulator[0].reception = g_DAPSKDemodulator[0].reception;
		}

		if(g_DAPSKDemodulator[0].dapsk.re != g_DAPSKDemodulator[3].dapsk.re)
		{
			g_DAPSKDemodulator[0].dapsk.re = g_DAPSKDemodulator[0].dapsk.re;
		}
		*/		
		
		// test ProcessingPowerNoSubCases
		// End real time part processing time counting
		//CounterRealtimePart = count_end(startValueRealtimePart);
		//if (CounterRealtimePart < MAX_COUNTER_REALTIMEPART) // execute Cases only if enough processing time left
		//{			
				
		switch(mainCaseCounter)
		{
			// WARNING: don´t change number of cases
			// repetiton rate of case functions = 1.067 kHz

			case MAINCASE_0:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_0;

				updateOscillator(0);
/*
				if (!NSKmodemType[0]*NSKPortEnable[0])
				{// FSK
					// oscillator AGC for 1.Modem
					oscillator_AGC(&g_FSKModulatorModem[0].txOsc);
					oscillator_AGC(&g_FSKDemodulatorModem[0].rxOsc);
					// SNR Calculaton FSK:
					SNRcalculationFSK(&g_FSKCarrierDemodulator[0]);
				}
				else if (NSKmodemType[0]*NSKPortEnable[0])
				{//DAPSK
					// oscillator AGC for 1.Modem
					oscillator_AGC(&g_DAPSKModulator[0].txOsc);
					oscillator_AGC(&g_DAPSKDemodulator[0].rxOsc);
				}
*/
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_1:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_1;
				
				updateOscillator(1);
/*
				if (!NSKmodemType[1]*NSKPortEnable[1])
				{// FSK
					// oscillator AGC for 2.Modem
					oscillator_AGC(&g_FSKModulatorModem[1].txOsc);
					oscillator_AGC(&g_FSKDemodulatorModem[1].rxOsc);
					// SNR Calculaton FSK:
					SNRcalculationFSK(&g_FSKCarrierDemodulator[1]);
				}
				else if (NSKmodemType[1]*NSKPortEnable[1])
				{//DAPSK
					// oscillator AGC for 2.Modem
					oscillator_AGC(&g_DAPSKModulator[1].txOsc);
					oscillator_AGC(&g_DAPSKDemodulator[1].rxOsc);
				}
*/
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_2:

				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_2;

				updateOscillator(2);
/*
				if (!NSKmodemType[2]*NSKPortEnable[2])
				{// FSK
					// oscillator AGC for 3.Modem
					oscillator_AGC(&g_FSKModulatorModem[2].txOsc);
					oscillator_AGC(&g_FSKDemodulatorModem[2].rxOsc);
					// SNR Calculaton FSK:
					SNRcalculationFSK(&g_FSKCarrierDemodulator[2]);
				}
				else if (NSKmodemType[2]*NSKPortEnable[2])
				{//DAPSK
					// oscillator AGC for 3.Modem
					oscillator_AGC(&g_DAPSKModulator[2].txOsc);
					oscillator_AGC(&g_DAPSKDemodulator[2].rxOsc);
				}
*/
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_3:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_3;

				updateOscillator(3);
/*
				if (!NSKmodemType[3]*NSKPortEnable[3])
				{// FSK
					// oscillator AGC for 4.Modem
					oscillator_AGC(&g_FSKModulatorModem[3].txOsc);
					oscillator_AGC(&g_FSKDemodulatorModem[3].rxOsc);
					// SNR Calculaton FSK:
					SNRcalculationFSK(&g_FSKCarrierDemodulator[3]);
				}
				else if (NSKmodemType[3]*NSKPortEnable[3])
				{//DAPSK
					// oscillator AGC for 4.Modem
					oscillator_AGC(&g_DAPSKModulator[3].txOsc);
					oscillator_AGC(&g_DAPSKDemodulator[3].rxOsc);
				}
*/			
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_4:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_4;
				
				if (!FPGA_Not_Loaded) NSK_FPGA_RD_OSR(&g_NSK_DSP_Config,&g_NSK_DSP_Status,&g_NSK_DSP_Alarm,&g_nskJabberHalt[0],&g_FPGA_Addresses,&g_FPGA_Registers,OSR_Counter);
				if (OSR_Counter < (N_MUX_PORTS-1))	OSR_Counter++;
				else OSR_Counter = 0;
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_5:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_5;

				// Read Fallback / Fallforward State of OFDM Modem
				OFDM_FBFF_state = (g_intTDM_RxStruct_Ptr->OFDM_DSP_AlarmStatusData_TS16.currentDatarateIx);

				 // OFDM Modem State changed (fall-back, fall-forward or not in sync state
				if(old_OFDM_FBFF_state != OFDM_FBFF_state)
				{
					Case_To_Do = NSK_FPGA_change_port_bitrates(&g_NSK_DSP_Status,&g_NSK_DSP_Config,&OFDM_FBFF_state,&g_FPGA_Addresses,&g_FPGA_Registers,Case_OFDM_Port);
					old_OFDM_FBFF_state = OFDM_FBFF_state;
				}
				 // continuing until finished and returned to Case_OFDM_Port
				else if(Case_To_Do != Case_OFDM_Port)
				{
					Case_New = NSK_FPGA_change_port_bitrates(&g_NSK_DSP_Status,&g_NSK_DSP_Config,&OFDM_FBFF_state,&g_FPGA_Addresses,&g_FPGA_Registers,Case_To_Do);
					Case_To_Do = Case_New;
				}

				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_6:		
				
				DoMemoryCheck(TRUE);

				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_6;
								
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_7:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_7;
				
				handle_R1LA_resetNSK(doR1LA_reset);

				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_8:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_8;
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_9:
				switch(subCaseCounter) {

					// WARNING: don´t change number of cases
					// repetiton rate of case functions = 0.1067 kHz
					
					case SUBCASE_0:	
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_0;

						// write STATUS & ALARM DATA:
						writeStatusAndAlarm(0);
/*
						if (NSKmodemType[0]*NSKPortEnable[0])
						{// DAPSK:
							statusDataDAPSK(&g_DAPSKInputConversion[0], &g_DAPSKModulator[0], &g_DAPSKDemodulator[0],\
								&g_NSK_DSP_Status, g_nskLinkAlarm, g_nskDCDAlarm, &g_NSK_ShortStatus[0], RTSalwaysActive, 0);
						}
						else if (!NSKmodemType[0]*NSKPortEnable[0])
						{// FSK:
							statusDataFSK(&g_FSKTxTimingModem[0], &g_FSKCarrierDemodulator[0], &g_NSK_DSP_Status,\
										  g_nskLinkAlarm, g_nskDCDAlarm, RTSalwaysActive, 0);
						}
						else
						{
							g_nskLinkAlarm[0] = FALSE;
							g_nskDCDAlarm[0]  = FALSE; 

						}
*/
						subCaseCounter++;
						break;
						
					case SUBCASE_1:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_1;

						// write STATUS & ALARM DATA:
						writeStatusAndAlarm(1);
/*

						if (NSKmodemType[1]*NSKPortEnable[1])
						{// DAPSK:
							statusDataDAPSK(&g_DAPSKInputConversion[1], &g_DAPSKModulator[1], &g_DAPSKDemodulator[1],\
								&g_NSK_DSP_Status, g_nskLinkAlarm, g_nskDCDAlarm, &g_NSK_ShortStatus[1], RTSalwaysActive, 1);
						}
						else if (!NSKmodemType[1]*NSKPortEnable[1])
						{// FSK:
							statusDataFSK(&g_FSKTxTimingModem[1], &g_FSKCarrierDemodulator[1], &g_NSK_DSP_Status,\
										  g_nskLinkAlarm, g_nskDCDAlarm, RTSalwaysActive, 1);
						}
						else
						{
							g_nskLinkAlarm[1] = FALSE;
							g_nskDCDAlarm[1]  = FALSE; 

						}
*/
						subCaseCounter++;
						break;
						
					case SUBCASE_2:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_2;

						// write STATUS & ALARM DATA:
						writeStatusAndAlarm(2);
/*

						if (NSKmodemType[2]*NSKPortEnable[2])
						{// DAPSK:
							statusDataDAPSK(&g_DAPSKInputConversion[2], &g_DAPSKModulator[2], &g_DAPSKDemodulator[2],\
								&g_NSK_DSP_Status, g_nskLinkAlarm, g_nskDCDAlarm,&g_NSK_ShortStatus[2], RTSalwaysActive, 2);
						}
						else if (!NSKmodemType[2]*NSKPortEnable[2])
						{// FSK:
							statusDataFSK(&g_FSKTxTimingModem[2], &g_FSKCarrierDemodulator[2], &g_NSK_DSP_Status,\
										  g_nskLinkAlarm, g_nskDCDAlarm, RTSalwaysActive, 2);
						}
						else
						{
							g_nskLinkAlarm[2] = FALSE;
							g_nskDCDAlarm[2]  = FALSE; 
						}
*/
						subCaseCounter++;
						break;
						
					case SUBCASE_3:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_3;
						
						// write STATUS & ALARM DATA:

						writeStatusAndAlarm(3);
/*
						if (NSKmodemType[3]*NSKPortEnable[3])
						{// DAPSK:
							statusDataDAPSK(&g_DAPSKInputConversion[3], &g_DAPSKModulator[3], &g_DAPSKDemodulator[3],\
								&g_NSK_DSP_Status, g_nskLinkAlarm, g_nskDCDAlarm,&g_NSK_ShortStatus[3], RTSalwaysActive, 3);
						}
						else if (!NSKmodemType[3]*NSKPortEnable[3])
						{// FSK:
							statusDataFSK(&g_FSKTxTimingModem[3], &g_FSKCarrierDemodulator[3], &g_NSK_DSP_Status,\
										  g_nskLinkAlarm, g_nskDCDAlarm, RTSalwaysActive, 3);
						}
						else
						{
							g_nskLinkAlarm[3] = FALSE;
							g_nskDCDAlarm[3]  = FALSE; 
						}
*/
						subCaseCounter++;
						break;
						
					case SUBCASE_4:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_4;
						
						// set/clear alarm digital transit no sync
						NSK_FPGA_RD_GDTS(&g_FPGA_Addresses, &g_FPGA_Registers, &g_NSK_DSP_Alarm);
						
						subCaseCounter++;
						break;
						
					case SUBCASE_5:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_5;
						
						// set/clear alarm digital transit missing rx data, update transit status
						NSK_FPGA_RD_TND(&g_FPGA_Addresses, &g_FPGA_Registers, &g_NSK_DSP_Alarm, &g_NSK_DSP_Status);
											
						subCaseCounter++;
						break;
						
					case SUBCASE_6:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_6;

						subCaseCounter++;
						break;
			
					case SUBCASE_7:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_7;
						
						if (g_niReceiveFlag)
						{
							g_niReceiveFlag = FALSE;

							switch (g_niRxMsgType)
							{	
								e_TDMServiceResult tdmServiceResult;

								case e_TDMServiceMsgType_IdleCounter_Req:

// case_counter test reseting when uploading idle counter 
/*
									g_DAPSKDemodulator[0].counter_aquisition0 = 0;
									g_DAPSKDemodulator[0].counter_aquisition1 = 0;
									g_DAPSKDemodulator[0].counter_aquisition2 = 0;
									g_DAPSKDemodulator[0].counter_aquisition3 = 0;
									g_DAPSKDemodulator[0].counter_aquisition4 = 0;
									g_DAPSKDemodulator[0].counter_aquisition5 = 0;
									g_DAPSKDemodulator[0].counter_aquisition6 = 0;
									g_DAPSKDemodulator[0].counter_detect = 0;
									g_DAPSKDemodulator[0].counter_early = 0;
									g_DAPSKDemodulator[0].counter_late = 0;
									g_DAPSKDemodulator[0].counter_beforeearly = 0;
									g_DAPSKDemodulator[0].counter_afterlate = 0;
									g_DAPSKDemodulator[0].counter_adsymbol = 0;
*/
/*
									if((g_DAPSKDemodulator[0].stayDetect_Flag||
									   g_DAPSKDemodulator[1].stayDetect_Flag||
									   g_DAPSKDemodulator[2].stayDetect_Flag||
									   g_DAPSKDemodulator[3].stayDetect_Flag)==TRUE)
									{
										g_DAPSKDemodulator[0].stayDetect_Flag=FALSE;
										g_DAPSKDemodulator[1].stayDetect_Flag=FALSE;
										g_DAPSKDemodulator[2].stayDetect_Flag=FALSE;
										g_DAPSKDemodulator[3].stayDetect_Flag=FALSE;
										t_IdleCounters *temp;
										temp=idleCounter_read();
										temp->idleCounterMinArray[IDLE_COUNTER_MIN_SUBCASE_9]=99;
									}
									else
									{
										g_DAPSKDemodulator[0].stayDetect_Flag=TRUE;
										g_DAPSKDemodulator[1].stayDetect_Flag=TRUE;
										g_DAPSKDemodulator[2].stayDetect_Flag=TRUE;
										g_DAPSKDemodulator[3].stayDetect_Flag=TRUE;
									}	
*/
									MEMCPY(g_niTxData, idleCounter_read(), sizeof(t_IdleCounters));
									idleCounter_reset(); // reset idle counters

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(t_IdleCounters),
    															          e_TDMServiceMsgType_IdleCounter_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);

									break;

								case e_TDMServiceMsgType_StatusData_Req:

									MEMCPY(g_niTxData, &g_NSK_DSP_Status, sizeof(g_NSK_DSP_Status));
									
									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(g_NSK_DSP_Status),
    															          e_TDMServiceMsgType_NSK_StatusData_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_NSKShortStatusData_Req:
								{
									
									// g_niRxData[0] contains the NSK modem number
									t_NSK_ShortStatus *pNSK_ShortStatus = &g_NSK_ShortStatus[g_niRxData[0]];

									/*
									g_DAPSKDemodulator[g_niRxData[0]].errorCriterionMax =1;
									g_DAPSKDemodulator[g_niRxData[0]].InitAcqFlagCounter=2;
									g_DAPSKDemodulator[g_niRxData[0]].DCDoffFlagCounter =3;
									g_DAPSKDemodulator[g_niRxData[0]].tau				=4;

									g_DAPSKDemodulator[g_niRxData[0]].SignalPowerRMax	=5;
									g_DAPSKDemodulator[g_niRxData[0]].SignalPowerIMax	=6;
									g_DAPSKDemodulator[g_niRxData[0]].pRcvMin			=7;
									g_DAPSKModulator[g_niRxData[0]].realTransmitSignalabsMax=8	;
									g_DAPSKDemodulator[g_niRxData[0]].dapsk.re			=9;
									g_DAPSKDemodulator[g_niRxData[0]].dapsk.im			=10;
									g_DAPSKDemodulator[g_niRxData[0]].gain				=11;
									g_DAPSKDemodulator[g_niRxData[0]].tau0c				=12;
									g_DAPSKDemodulator[g_niRxData[0]].Rdd				=13;							

									processingOverload_Cntr								=14; // processing overload
									*/
									

									nskDSPLoggerUpdate(pNSK_ShortStatus,&processingOverload_Cntr);
									
									MEMCPY(g_niTxData, pNSK_ShortStatus, sizeof(t_NSK_ShortStatus));
								
									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(t_NSK_ShortStatus),
    															          e_TDMServiceMsgType_NSKShortStatusData_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);

									nskDSPLoggerReset(pNSK_ShortStatus);


									break;
								}
								case e_TDMServiceMsgType_NSKGetAdapEquTF_Req:

									// g_niRxData[0] contains the NSK modem number
									MEMCPY(g_niTxData, &g_DAPSKDemodulator[g_niRxData[0]].adEq_h, sizeof(t_NSK_AdapEquTF));


									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(t_NSK_AdapEquTF),
    															          e_TDMServiceMsgType_NSKGetAdapEquTF_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_MUXGetVoiceTransitStatus_Req:

									NSK_FPGA_RD_transitCounters(&g_FPGA_Addresses, &g_FPGA_Registers);
									g_MUX_VoiceTransitDevStatus.txFrameDiscardCounter	= g_FPGA_Registers.FDCT;
									g_MUX_VoiceTransitDevStatus.txFrameInsertCounter	= g_FPGA_Registers.FICT;
									g_MUX_VoiceTransitDevStatus.rxFrameDiscardCounter	= g_FPGA_Registers.FDCR;
									g_MUX_VoiceTransitDevStatus.rxFrameInsertCounter	= g_FPGA_Registers.FICR;

									// for testing
									//NSK_FPGA_RD_VoiceTransitReg(&g_FPGA_Addresses, &g_FPGA_RegistersTestRD);

									MEMCPY(g_niTxData, &g_MUX_VoiceTransitDevStatus, sizeof(g_MUX_VoiceTransitDevStatus));

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(g_MUX_VoiceTransitDevStatus),
    															          e_TDMServiceMsgType_MUXGetVoiceTransitStatus_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_MUXSetVoiceTransitTestCfg_Send:

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    															           e_TDMServiceMsgType_Ack); // txMsgType = acknowledge

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);

									MEMCPY(&g_MUX_VoiceTransitTestCfg, g_niRxData, sizeof(g_MUX_VoiceTransitTestCfg));
									
									NSK_FPGA_resetTransitCounters(&g_FPGA_Addresses, &g_FPGA_Registers);
									
									break;

								case e_TDMServiceMsgType_Cfg_Send:	// this case should not occur 

									// send TDM service reply
									tdmServiceResult = 
											tdmServiceNI_Reply (NULL, 0,                   // zero length reply message
    															e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;


								default:
									ASSERT(FALSE);
									
							}
						}

						subCaseCounter++;
						break;
						
					case SUBCASE_8:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_8;
					
						myLifesign++;
						if (myLifesign == 8) myLifesign = 0;

						// update APLC alarm bits
						nskDSPAlarmUpdate(&g_NSK_DSP_Alarm,&g_nskLinkAlarm[0], &g_nskDCDAlarm[0], &g_nskJabberHalt[0],FPGA_Not_Loaded,Subprint_Error);
					
						subCaseCounter++;
						break;
					
					case SUBCASE_9:
					{
						static BOOL bFlag = FALSE;


						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_9;
						
						if (!bFlag)
						{
							idleCounter_reset();
							bFlag = TRUE;
						}
							
						subCaseCounter = 0;
						break;
					}
					default:
						subCaseCounter = 0;
						break;
				}; // end switch(subCaseCounter)
					
				mainCaseCounter = 0;
				break;
    
			default:
				mainCaseCounter = 0;
				break;
		}; // end switch(mainCaseCounter)
		// test ProcessingPowerNoSubCases
		//} // end if
    }
}

/*****************************************************************************/
/* Local Functions                                                           */
/*****************************************************************************/

/*****************************************************************************
  Function:		updateOscillator()	
  Description:	oscillator AGC update
  Inputs:		modem number 
 ******************************************************************************/
static void SEGMENT_EXT_CODE updateOscillator(int modemNo)
{
		if (!NSKmodemType[modemNo]*NSKPortEnable[modemNo])
		{// FSK
			oscillator_AGC(&g_FSKModulatorModem[modemNo].txOsc);
			oscillator_AGC(&g_FSKDemodulatorModem[modemNo].rxOsc);
			// SNR Calculaton FSK:
			SNRcalculationFSK(&g_FSKCarrierDemodulator[modemNo]);
		}
		else if (NSKmodemType[modemNo]*NSKPortEnable[modemNo])
		{//DAPSK
			oscillator_AGC(&g_DAPSKModulator[modemNo].txOsc);
			oscillator_AGC(&g_DAPSKDemodulator[modemNo].rxOsc);
		}
}


/*****************************************************************************
  Function:		writeStatusAndAlarm()	
  Description:	writes STATUS & ALARM DATA:
  Inputs:		modem number 
 ******************************************************************************/
static void SEGMENT_EXT_CODE writeStatusAndAlarm(int modemNo)
{


	// write STATUS & ALARM DATA:
	if (NSKmodemType[modemNo]*NSKPortEnable[modemNo])
	{// DAPSK:
			statusDataDAPSK(&g_DAPSKInputConversion[modemNo], &g_DAPSKModulator[modemNo], &g_DAPSKDemodulator[modemNo],\
						&g_NSK_DSP_Status, g_nskLinkAlarm, g_nskDCDAlarm, &g_NSK_ShortStatus[modemNo], RTSalwaysActive, modemNo);
	}
	else if (!NSKmodemType[modemNo]*NSKPortEnable[modemNo])
	{// FSK:
			statusDataFSK(&g_FSKTxTimingModem[modemNo], &g_FSKCarrierDemodulator[modemNo], &g_NSK_DSP_Status,\
					  g_nskLinkAlarm, g_nskDCDAlarm, RTSalwaysActive, modemNo);
	}
	else
	{
			g_nskLinkAlarm[modemNo] = FALSE;
			g_nskDCDAlarm[modemNo]  = FALSE; 
	}
}




/*****************************************************************************
  Function:		getConfiguration()	
  Description:	Waits for the configuration sent by the master DSP
  Inputs:		config: pointer to configuration data structure 
  Outputs:		Received configuration data
  Return:		-
  Globals:		
  Important:	-
******************************************************************************/
static void SEGMENT_EXT_CODE getConfiguration(t_NSK_DSP_Config *config)
{
	long i;
	e_TDMServiceResult tdmServiceResult;

	tdmServiceNI_Init(e_TDMServiceAddr_NSK,  // initialize non-initiator TDM service
					  g_niRxData,
					  sizeof(g_niRxData) / sizeof(unsigned long),
					  &niReceive,
					  &niConfirm);

	niConfirm(); // enable TDM service receiver

	// wait for the configuration
	while(!g_niReceiveFlag) // the flag is set when the TDM service received a valid configuration
	{
		// repetiton rate = 10.667 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		IR_BlockCounter = 1;
		RestartTimer10k();
		IR_TimerCounter = 1;

		tdmServiceActivateNonInitiator((unsigned long *)internalTDM_Rx_Ptr, (unsigned long *)internalTDM_Tx_Ptr);
	}

	g_niReceiveFlag = FALSE;

	// configuration received
	ASSERT(e_TDMServiceMsgType_Cfg_Send == g_niRxMsgType); // check if tx message type is correct
	ASSERT(sizeof(*config)              == g_niRxMsgSize); // check if size is correct

	// send TDM service reply
	tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    				                       e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

	ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

	MEMCPY(config, g_niRxData, sizeof(*config)); // copy received mesage into config
	// tdmServiceActivateNonInitiator() is called to send ack
	for(i = 0; i < 20; i++)
	{
		// repetiton rate = 10.667 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		IR_BlockCounter = 1;
		RestartTimer10k();
		IR_TimerCounter = 1;

		tdmServiceActivateNonInitiator((unsigned long *)internalTDM_Rx_Ptr, (unsigned long *)internalTDM_Tx_Ptr);
	}
}

/*****************************************************************************
  Function:		niReceive()	
******************************************************************************/
static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize)
{
	g_niReceiveFlag = TRUE;
	g_niRxMsgType = rxMsgType;
	g_niRxMsgSize = rxMsgSize;
}

/*****************************************************************************
  Function:		niConfirm()	
******************************************************************************/
static void niConfirm (void)
{
	tdmServiceNI_Receive();
}

/*****************************************************************************
  Function:		initModems()	
  Description:	Initialize Modems and load the right configuration
				Called once on start-up.
  Inputs:		Pointer to configuration data structure


  Outputs:		Pointer to Modulator output array
				Pointer to Modulator output matrix (channel with	 allpass)
				Pointer to Modulator output matrix (channel without allpass)
				Pointer to Demodulator input array	(channel with    static equalizer)
				Pointer to Demodulator input array	(channel without static equalizer)
  Return:		-
  Globals:		
  Important:	-
******************************************************************************/



/*!
  \brief           Initialize Modems and load the right configuration

					Called once on start-up.

  \param[in]       g_NSK_DSP_Config    Pointer to configuration data structure


  \param[out]		TDMModDataOut			Pointer to Modulator output array
  \param[out] 		TDMModDataOutMatrix		Pointer to Modulator output matrix (channel with	 allpass)
  \param[out] 		TDMModDataOutMatrixNoAP	Pointer to Modulator output matrix (channel without allpass)
  \param[out] 		nskRx					Pointer to Demodulator input array	(channel with    static equalizer)
  \param[out] 		nskRxNoEq				Pointer to Demodulator input array	(channel without static equalizer)     

  \b Initfunction:

  No
*/

static void SEGMENT_EXT_CODE initModems(t_NSK_DSP_Config g_NSK_DSP_Config,								
										float TDMModDataOut[N_NSK_MODEMS],								
										float TDMModDataOutMatrix[N_NSK_MODEMS][N_APLC_CHANNELS],		
										float TDMModDataOutMatrixNoAP[N_NSK_MODEMS][N_APLC_CHANNELS],	
										float nskRx[N_APLC_CHANNELS],									
										float nskRxNoEq[N_APLC_CHANNELS])								
{

	long iModem; // control variable for Modem initialisation
	float nskNoRx = 0.0;

	
	for (iModem=0; iModem<N_NSK_MODEMS; iModem++)
	{
		TDMModDataOut[iModem] = 0.0; // reset output array

		if ((g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS) > 0)
		// NSK600 modem assigned to port
		{
			if ((g_NSK_DSP_Config.muxCfg.ports.V24[iModem].control & V24PORT_CFG_CONTROL_PORT_MODES) == V24PORT_CFG_CONTROL_PORT_MODES)
			{// -> Mux mode
				NSKMuxmode[iModem] = TRUE;
			}
			else
			{// -> NSK mode
				NSKMuxmode[iModem] = FALSE;
							
				if ((g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_OFF_ON_PRIORITY_BITS) > 0)
				{ // -> NSK modem is on (disconnectable or non-disconnectable)
					NSKPortEnable[iModem] = TRUE;

					// set pointer to right channel allocation: 1: ch1, 2: ch2, 3: ch3
					// if the adaptive equalizer is active choose the "no Allpass" path
				
					if ((g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS) == NSK_CFG_MODE_CH_ALLOCATION_BITS)
					// Modem in Channel 3
					{	
						if((g_NSK_DSP_Config.equalizer[iModem].mode & AD_EQ_MODE_MASK) == AD_EQ_MODE_OFF)
						//  --> no adaptive equalizer used -> use the allpass path
						{
							TDMModDataOutMatrix[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][2] = 1.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][2] = 0.0; // fill-up send (Tx) matrix

							p_TDMDemodDataIn[iModem] = &nskRx[2];  // RX pointer to Channel 3 "allpass path"
						}
						else
						// --> adaptive equalizer used fill in "no allpass" path
						{
							TDMModDataOutMatrix[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][2] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][2] = 1.0; // fill-up send (Tx) matrix

							p_TDMDemodDataIn[iModem] = &nskRxNoEq[2];  // RX pointer to Channel 3 "allpass path"
						}
					}
					else if ((g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS) == NSK_CFG_MODE_CH_ALLOCATION_BIT1)
					// Modem in Channel 2
					{ 	
						if((g_NSK_DSP_Config.equalizer[iModem].mode & AD_EQ_MODE_MASK) == AD_EQ_MODE_OFF)
						//  --> no adaptive equalizer used -> use the allpass path
						{
							TDMModDataOutMatrix[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][1] = 1.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][2] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][2] = 0.0; // fill-up send (Tx) matrix

							p_TDMDemodDataIn[iModem] = &nskRx[1];  // RX pointer to Channel 2  "allpass path"

						}
						else
						// --> adaptive equalizer used fill in "no allpass" path
						{
							TDMModDataOutMatrix[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][2] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][1] = 1.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][2] = 0.0; // fill-up send (Tx) matrix

							p_TDMDemodDataIn[iModem] = &nskRxNoEq[1];  // RX pointer to Channel 3 "allpass path"
						}
					}
					else if ((g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS) == NSK_CFG_MODE_CH_ALLOCATION_BIT0)
					// Modem in Channel 1
					{	

						if((g_NSK_DSP_Config.equalizer[iModem].mode & AD_EQ_MODE_MASK) == AD_EQ_MODE_OFF)
						//  --> no adaptive equalizer used -> use the allpass path
						{
							TDMModDataOutMatrix[iModem][0] = 1.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][2] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][2] = 0.0; // fill-up send (Tx) matrix

							p_TDMDemodDataIn[iModem] = &nskRx[0];  // RX pointer to Channel 1  "allpass path"

						}						
						else
						// --> adaptive equalizer used fill in "no allpass" path
						{
							TDMModDataOutMatrix[iModem][0] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrix[iModem][2] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][0] = 1.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][1] = 0.0; // fill-up send (Tx) matrix
							TDMModDataOutMatrixNoAP[iModem][2] = 0.0; // fill-up send (Tx) matrix

							p_TDMDemodDataIn[iModem] = &nskRxNoEq[0];  // RX pointer to Channel 3 "allpass path"
						}
					}
					else
					{
						TDMModDataOutMatrix[iModem][0] = 0.0; // fill-up send (Tx) matrix
						TDMModDataOutMatrix[iModem][1] = 0.0; // fill-up send (Tx) matrix
						TDMModDataOutMatrix[iModem][2] = 0.0; // fill-up send (Tx) matrix
						TDMModDataOutMatrixNoAP[iModem][0] = 0.0; // fill-up send (Tx) matrix
						TDMModDataOutMatrixNoAP[iModem][1] = 0.0; // fill-up send (Tx) matrix
						TDMModDataOutMatrixNoAP[iModem][2] = 0.0; // fill-up send (Tx) matrix
						p_TDMDemodDataIn[iModem] = &nskNoRx;
					}
					//___________________________________________________________________
					//
					// Channel disconnectable?
					if ((g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_OFF_ON_PRIORITY_BITS) == NSK_CFG_MODE_OFF_ON_PRIORITY_BIT0)
					{ 	channelDisconnectable[iModem] = TRUE;
					}
					else
					{	channelDisconnectable[iModem] = FALSE;
					}				
					//___________________________________________________________________
					//
					// RTS "always active"->none or RTS/CTS/DCD, Data Sensitive?
					//
					if ((g_NSK_DSP_Config.muxCfg.ports.V24[iModem].control & V24PORT_CFG_CONTROL_PORT_MODES) == V24PORT_CFG_CONTROL_PORT_MODE_0)
					// case DAPSK Modem --> transmit always = RTS always active
					{
						RTSalwaysActive[iModem] = TRUE;
					}
					else
					{
					//  --> FSK case: do normal check
						if (((g_NSK_DSP_Config.muxCfg.ports.V24[iModem].control & V24PORT_CFG_CONTROL_HW_HANDSHAKING_MASK) == V24PORT_CFG_CONTROL_HW_HANDSHAKING_0) || \
						((g_NSK_DSP_Config.muxCfg.ports.V24[iModem].control & V24PORT_CFG_CONTROL_HW_HANDSHAKING_MASK) == V24PORT_CFG_CONTROL_HW_HANDSHAKING_1))
						{	
							RTSalwaysActive[iModem] = FALSE;
						}
						else
						{	
							RTSalwaysActive[iModem] = TRUE;
						}
					}

					//___________________________________________________________________
					//
					// Clockregenerator active? 
					if (g_NSK_DSP_Config.modem[iModem].mode & NSK_CFG_MODE_RX_CLK_RECOVERY_BITS)
					// if Clockregenerator = active, RTS = True
					{	
						NSKclockregeneratorOn[iModem] = TRUE;
					}
					else
					{
						NSKclockregeneratorOn[iModem] = FALSE;
					}				
					//___________________________________________________________________
					//
					// DAPSK or FSK?
					if ((g_NSK_DSP_Config.muxCfg.ports.V24[iModem].control & V24PORT_CFG_CONTROL_PORT_MODES) == V24PORT_CFG_CONTROL_PORT_MODE_0)
					// -> DAPSK mode
					{  
						NSKmodemType[iModem] = TRUE;
						InitModulatorDAPSK(&g_DAPSKModulator[iModem], &g_NSK_DSP_Config.modem[iModem], \
									&g_NSK_DSP_Config.muxCfg.ports.V24[iModem],&g_NSK_DSP_Config.equalizer[iModem], iModem);
						InitTxInputConversionDAPSK(&g_NSK_DSP_Config.modem[iModem], &g_DAPSKInputConversion[iModem],\
							                       &g_NSK_DSP_Config.muxCfg.ports.V24[iModem]);
						InitDemodulatorDAPSK(&g_DAPSKDemodulator[iModem], &g_NSK_DSP_Config.modem[iModem], \
									         &g_NSK_DSP_Config.muxCfg.ports.V24[iModem], &g_DAPSKDemodulatorConversion[iModem], \
											 &g_NSK_DSP_Config.equalizer[iModem], iModem);
					}
					else 
					// -> FSK mode
					{
						NSKmodemType[iModem] = FALSE;
						InitModulatorFSK(&g_FSKModulatorModem[iModem], &g_NSK_DSP_Config.modem[iModem],\
							             &g_NSK_DSP_Config.muxCfg.ports.V24[iModem] ,&g_FSKTxTimingModem[iModem]);
					    InitDemodulatorFSK(&g_FSKDemodulatorModem[iModem], &g_NSK_DSP_Config.modem[iModem], \
							               &g_NSK_DSP_Config.muxCfg.ports.V24[iModem], &g_FSKCarrierDemodulator[iModem], \
										   &g_FSKClockregenerator[iModem]);
					}

				}
				else
				{ // -> NSK modem is off
					NSKPortEnable[iModem] = FALSE;
				}
			}
		}
		else
		{// V24 port is disable
			NSKMuxmode[iModem] = FALSE;
			NSKPortEnable[iModem] = FALSE;
			NSKclockregeneratorOn[iModem] = FALSE;
			RTSalwaysActive[iModem] = FALSE;
			channelDisconnectable[iModem] = TRUE;
		}
	}
}


/*****************************************************************************
  Function:		executeModems()	
  Description:	This is the realtime function in which the transmitter and receiver is proceeded
				called with 10.66kHz
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		
  Important:	-
******************************************************************************/

/*!
  \brief           Realtime Signal processing
				
  This is the realtime function in which the transmitter and receiver is proceeded called with 10.66kHz

  \b Initfunction:

  initModems(), called in main()

*/
	
static void executeModems(void)
{
	long iexModem; // control variable for FPGA initialisation

	for (iexModem=0; iexModem<N_NSK_MODEMS; iexModem++)
	{
		// Checking Modem Configuration (Activ?,FSK/ADPSK?):
		if (NSKPortEnable[iexModem])
    	// -> PORT is ENABLED:
		{
			if (NSKMuxmode[iexModem])
			// -> Mux mode
			{ // ??? used ???
			}
			else
			{	// DAPSK or FSK?
				if (NSKmodemType[iexModem])
				// -> DAPSK mode
				{  
					if (DelayDAPSKModulation > DAPSK_MODULATION_BOOTDELAY) // do not send data'till AGC is ready (test)
					{
					
						// Modulation -----------------------------------------
						// if "RTSalwaysActive" = True -> fpgaRTS = 1 
						g_DAPSKModulator[iexModem].fpgaRTS = (g_FPGA_RTS.TXD_RTS[iexModem] || RTSalwaysActive[iexModem]);

						// RTS / CTS:
						RTSCTStimingDAPSK(&g_DAPSKModulator[iexModem], &g_DAPSKInputConversion[iexModem]);

						// Data from FPGA:
						txInputConversionDAPSK(&g_FPGA_Registers.TUD[iexModem], &g_DAPSKInputConversion[iexModem], &g_DAPSKModulator[iexModem]); 

						modulationDAPSK(&g_DAPSKModulator[iexModem], &g_DAPSKInputConversion[iexModem]);
			
						// Channel disconnectable and disconnected? True -> no Signal else transmit signal
						//write to TDM output struct
						TDMModDataOut[iexModem] = g_DAPSKModulator[iexModem].realTransmitSignal * !(channelDisconnectable[iexModem] && disconnectModem);
					}
					else
					{
						DelayDAPSKModulation = DelayDAPSKModulation+1;
					}


					// Demodulation ---------------------------------------
					/* testMatlab
					if (g_Matlab_save.n_cntr>=L_Matlab_save)
					{
						g_Matlab_save.n_cntr=0;
						g_Matlab_save.N_cntr++;
					}
					if (iexModem==0)
					{
						g_Matlab_save.save_flag=1;
					}
					// end testMatlab */


					demodulatorDAPSK(p_TDMDemodDataIn[iexModem], &g_DAPSKDemodulator[iexModem], &g_DAPSKDemodulatorConversion[iexModem]);
                    // uncomment this definition when using testMatlab
    				//demodulatorDAPSK(p_TDMDemodDataIn[iexModem], &g_DAPSKDemodulator[iexModem], &g_DAPSKDemodulatorConversion[iexModem],&g_Matlab_save);
					/* testMatlab
					if (iexModem==0)
					{
						g_Matlab_save.save_flag=0;
						g_Matlab_save.n_cntr++;						
					}	
					// end testMatlab */

					// Carrierdetection if ok -> send data else send high
					// from FSK: CarrierDetectionFSK(&g_FSKDemodulatorModem[iexModem], &g_FSKCarrierDemodulator[iexModem]);
						
					// write to FPGA structs ------------------------------
					g_FPGA_Registers.RUD[iexModem] = g_DAPSKDemodulator[iexModem].fpgaRUD; // set RX transmitdata variable in FPGA struct 
					g_FPGA_PCR.POE[iexModem] = g_DAPSKDemodulator[iexModem].DCD; // set DCD variable in FPGA struct
					g_FPGA_RSIG.TXD_CTS[iexModem] = g_DAPSKModulator[iexModem].CTS;  // set CTS variable in FPGA struct	

				}
				else 
				// -> FSK mode
				{
					// Modulation -----------------------------------------
		   			// if "RTSalwaysActive" = True -> fpgaRTS = 1 
					g_FSKTxTimingModem[iexModem].RTS = (g_FPGA_RTS.TXD_RTS[iexModem] || RTSalwaysActive[iexModem]);
									
					txTimingFSK(&g_FSKModulatorModem[iexModem],&g_FSKTxTimingModem[iexModem]); // Timing for Modulation
					// Data from FPGA:
					modulatorFSK(&g_FPGA_Registers.TUD[iexModem], &g_FSKModulatorModem[iexModem],&g_FSKTxTimingModem[iexModem]); // Modulation
				   	    
					// Channel disconnectable and disconnected? True -> no Signal else transmit signal
					//write to TDM output struct
					TDMModDataOut[iexModem] = g_FSKModulatorModem[iexModem].fskTx * !(channelDisconnectable[iexModem] && disconnectModem);

					// ----------------------------------------------------
			//		p_TxDataDSP = &g_FSKModulatorModem[iexModem].fskTx; // looping for testreasons only
// testFSK
//					p_TDMDemodDataIn[iexModem] = &g_FSKModulatorModem[iexModem].fskTx; // looping for testreasons only
// end testFSK
					// ----------------------------------------------------
		
					// Demodulation ---------------------------------------
					demodulatorFSK(p_TDMDemodDataIn[iexModem], &g_FSKDemodulatorModem[iexModem] \
							/* testFSK	, &g_FSKModulatorModem[iexModem] */ );  
					// Carrierdetection if ok -> send data else send high
					CarrierDetectionFSK(&g_FSKDemodulatorModem[iexModem], &g_FSKCarrierDemodulator[iexModem]);

					// Clockregenerator -----------------------------------
					clockregeneratorFSK(&g_FSKClockregenerator[iexModem], &g_FSKDemodulatorModem[iexModem], \
										&g_FSKCarrierDemodulator[iexModem], &NSKclockregeneratorOn[iexModem]);
					//g_FSKDemodulatorModem[iexModem].fpgaRUD =  g_FSKDemodulatorModem[iexModem].demodRUD;
		
					// write to FPGA structs ------------------------------
					g_FPGA_Registers.RUD[iexModem] = g_FSKDemodulatorModem[iexModem].fpgaRUD; // set RX transmitdata variable in FPGA struct 
					g_FPGA_PCR.POE[iexModem] = g_FSKCarrierDemodulator[iexModem].fskDCD; // set DCD variable in FPGA struct
					g_FPGA_RSIG.TXD_CTS[iexModem] = g_FSKTxTimingModem[iexModem].CTS;  // set CTS variable in FPGA struct	
				}
			}
		}
	}
}

/*****************************************************************************
  Function:		nskDSPAlarmUpdate()		
  Description:	Updates the APLC alarm bits.
  Inputs:		Pointer to NSK DSP alarm bits: nskAlarm 
				Pointer to NSK DSP Link Alarm Array: nskLinkAlarm
  Outputs:		NSK DSP alarm bits
  Return:		None
  Globals:		None
  Important:	-
*******************************************************************************/
void SEGMENT_EXT_CODE nskDSPAlarmUpdate(t_NSK_Alarm* nskAlarm_Ptr,BOOL* nskLinkAlarm_Ptr, \
										BOOL* nskDCDAlarm_Ptr, BOOL* nskJabberHalt_Ptr, \
										BOOL FPGA_Not_Loaded,BOOL Subprint_Error)
{
	nskAlarm_Ptr->LinkAlm1			= *nskLinkAlarm_Ptr;
	nskAlarm_Ptr->LinkAlm2			= *(nskLinkAlarm_Ptr+1);
	nskAlarm_Ptr->LinkAlm3			= *(nskLinkAlarm_Ptr+2);
	nskAlarm_Ptr->LinkAlm4			= *(nskLinkAlarm_Ptr+3);

	nskAlarm_Ptr->DCDAlm1			= *nskDCDAlarm_Ptr;
	nskAlarm_Ptr->DCDAlm2			= *(nskDCDAlarm_Ptr+1);
	nskAlarm_Ptr->DCDAlm3			= *(nskDCDAlarm_Ptr+2);
	nskAlarm_Ptr->DCDAlm4			= *(nskDCDAlarm_Ptr+3);

	nskAlarm_Ptr->JabberTmo1		= *nskJabberHalt_Ptr;
	nskAlarm_Ptr->JabberTmo2		= *(nskJabberHalt_Ptr+1);
	nskAlarm_Ptr->JabberTmo3		= *(nskJabberHalt_Ptr+2);
	nskAlarm_Ptr->JabberTmo4		= *(nskJabberHalt_Ptr+3);
	nskAlarm_Ptr->JabberTmo5		= *(nskJabberHalt_Ptr+4);
	nskAlarm_Ptr->JabberTmo6		= *(nskJabberHalt_Ptr+5);
	nskAlarm_Ptr->SubprintAlarm		= Subprint_Error;
	nskAlarm_Ptr->FPGALoadFailure	= FPGA_Not_Loaded;
	nskAlarm_Ptr->progMemCRCErr		= memoryCheckDSP_CRC_Alarm();
}


/*****************************************************************************
  Function:		handle_R1LA_resetNSK()		
  Description:	resets R1LA if Master DSP requests it. Test for positive edge of reset bit
				and count it twice to be sure that the bit is stable. 
				Generates reset pulse of 9,375 ms on Flag4 (low active)
  Inputs:		doR1LA_reset = g_intTDM_RxStruct_Ptr->MasterDSPOutputFlags_TS65.reset_R1LA
  Outputs:		
  Return:		None
  Globals:		None
  Important:	 
*******************************************************************************/
#define RESET_R1LA_PULSE_WIDTH		10		// 10 * MAIN_CASE_7 = 9,375 ms


static void SEGMENT_EXT_CODE handle_R1LA_resetNSK(BOOL R1LA_reset)
{
	static unsigned long count_R1LA_reset_Bit;
	static BOOL	R1LA_reset_Bit_pos_edge;
	static BOOL	is_R1LA_reset_Bit_pos_edge_detected;


	if((R1LA_reset) && (!R1LA_reset_Bit_pos_edge))
	{	// Detect 0 to 1 transition 
		is_R1LA_reset_Bit_pos_edge_detected = TRUE;
	}
	else
	{
		if((is_R1LA_reset_Bit_pos_edge_detected) && (R1LA_reset))
		{	// positive edge & reset_R1LA still active (second test for active reset bit)
			count_R1LA_reset_Bit++;
			
			// Reset R1LA
			*(long*)(IOFLAG) &= ~(FLG4);
			
			if(count_R1LA_reset_Bit > RESET_R1LA_PULSE_WIDTH)
			{	// Release Reset
				*(long*)(IOFLAG) |= (FLG4);

				is_R1LA_reset_Bit_pos_edge_detected = FALSE; // be prepared for the next reset
			}			
		}
		else
		{
			count_R1LA_reset_Bit = 0;
			*(long*)(IOFLAG) |= (FLG4);
			is_R1LA_reset_Bit_pos_edge_detected = FALSE;
		}		
	}

	R1LA_reset_Bit_pos_edge = R1LA_reset;
}


void SEGMENT_EXT_CODE nskDSPLoggerUpdate(t_NSK_ShortStatus *pNSK_ShortStatus ,long *processingOverload_Cntr)
{

	// fill the dynamic process values

	pNSK_ShortStatus->adaptEquError = g_DAPSKDemodulator[g_niRxData[0]].errorCriterionMax;
	pNSK_ShortStatus->InitAcqFlagCounter=g_DAPSKDemodulator[g_niRxData[0]].InitAcqFlagCounter;
	pNSK_ShortStatus->DCDoffFlagCounter=g_DAPSKDemodulator[g_niRxData[0]].DCDoffFlagCounter;
	pNSK_ShortStatus->TestVar=g_DAPSKDemodulator[g_niRxData[0]].tau;

	pNSK_ShortStatus->NSK_ShortStatus_float[0]=g_DAPSKDemodulator[g_niRxData[0]].SignalPowerRMax;
	pNSK_ShortStatus->NSK_ShortStatus_float[1]=g_DAPSKDemodulator[g_niRxData[0]].SignalPowerIMax;
	//pNSK_ShortStatus->NSK_ShortStatus_float[2]=
	pNSK_ShortStatus->NSK_ShortStatus_float[3]=g_DAPSKDemodulator[g_niRxData[0]].pRcvMin;
	pNSK_ShortStatus->NSK_ShortStatus_float[4]=g_DAPSKModulator[g_niRxData[0]].realTransmitSignalabsMax	;
	pNSK_ShortStatus->NSK_ShortStatus_float[5]=g_DAPSKDemodulator[g_niRxData[0]].dapsk.re;
	pNSK_ShortStatus->NSK_ShortStatus_float[6]=g_DAPSKDemodulator[g_niRxData[0]].dapsk.im;
	pNSK_ShortStatus->NSK_ShortStatus_float[7]=g_DAPSKDemodulator[g_niRxData[0]].gain;
	pNSK_ShortStatus->NSK_ShortStatus_float[8]=g_DAPSKDemodulator[g_niRxData[0]].tau0c;
	pNSK_ShortStatus->NSK_ShortStatus_float[9]=g_DAPSKDemodulator[g_niRxData[0]].Rdd;


	pNSK_ShortStatus->NSK_ShortStatus_long[0]=*processingOverload_Cntr; // processing overload

}
void SEGMENT_EXT_CODE nskDSPLoggerReset(t_NSK_ShortStatus *pNSK_ShortStatus)
{

	pNSK_ShortStatus->signalQuality=0; // big value
	g_DAPSKDemodulator[g_niRxData[0]].errorCriterionMax=0;
	g_DAPSKDemodulator[g_niRxData[0]].InitAcqFlagCounter=0;
	g_DAPSKDemodulator[g_niRxData[0]].DCDoffFlagCounter=0;
	g_DAPSKDemodulator[g_niRxData[0]].SignalPowerRMax=0;
	g_DAPSKDemodulator[g_niRxData[0]].SignalPowerIMax=0;
	g_DAPSKDemodulator[g_niRxData[0]].pRcvMin=1.0E+38;
	g_DAPSKModulator[g_niRxData[0]].realTransmitSignalabsMax=0;
	//OverloadCounter is not set back !!!

}
/* End of File **************************************************************/

