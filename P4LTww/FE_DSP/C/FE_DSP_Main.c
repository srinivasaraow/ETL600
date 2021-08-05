/*---------------------------------------------------------------------------*
 * $Workfile: FE_DSP_Main.c $
 * Part of      : ETL600 / Front end DSP
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : FE DSP main file
 * Copyright    : ABB Switzerland Ltd., 2003
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/FE_DSP/C/FE_DSP_Main.c $
 * 
 * 70    12-03-07 11:49 Chhamaa
 * Tx alarm levle adjustment modified.
 * 
 * 69    12-02-24 17:51 Chhamaa
 * Commissioning of Tx alarm level modified for ETL600 R4.
 * 
 * 68    11-10-05 18:12 Chhamaa
 * RF loopback modified.
 * 
 * 67    11-09-07 18:15 Chhamaa
 * - Power reduction for RF loopback added.
 * - New commissioning mode added.
 * 
 * 66    11-03-23 15:47 Chhamaa
 * initAPLC(): handling for R4 power settings added.
 * 
 * 65    11-03-11 18:00 Chhamaa
 * Spectrum inversion in case of RF loopback added.
 * 
 * 64    11-01-26 13:21 Chhamaa
 * initAPLC(): dplcOnly calculation modified (bug 465 fixed).
 * 
 * 62    22.09.08 15:25 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 61    14.08.08 16:38 Chhamaa
 * OFDM tx signal peak amplitude adjusted.
 * 
 * 60    08-04-24 9:20 Chsteic
 * - switch off Tx signal of adaptive equalizer in case on pseudo noise
 * geneartion.
 * 
 * 59    17.12.07 14:48 Chchlee
 * 80W: MOD600 level -1dB
 * 
 * 58    07-08-28 13:12 Chchlee
 * Additional path for injection of NSK signals after allpass filter added
 * 
 * 56    14.11.06 13:47 Chpeplu
 * Function initAPLC() changed for custom mode
 * 
 * 54    27.06.06 11:03 Chhamaa
 * Code added to stop life sign in case of more than 50 filter resets (due
 * to floating point overflows) within 10s.
 * 
 * 52    5.04.06 14:48 Chhamaa
 * OFDM tx signal processing added
 * 
 * 51    4.04.06 18:28 Chmibuh
 * Added DMA supervision UC TDM bus: New functions 
 * IncrementDMA_ErrorCounter() and SetDMA_ErrorCounterZero(), tested Added
 * CheckUpconverterBusOnFE_DSP() and RestartUC_TDM()
 * 
 * 50    31.01.06 16:29 Chpeplu
 * Type t_ssbData1Ch changed, comment added
 * 
 * 48    28.10.05 15:42 Chhamaa
 * ASSERT replaced in txAfSignalProcessing(). Send no signal in case of
 * wrong number of channels.
 * 
 * 47    26.10.05 14:00 Chchlee
 * OP_MODE_SYSTEM_TEST removed
 * 
 * 46    5.10.05 13:55 Chhamaa
 * EOC debug code removed.
 * 
 * 45    4.10.05 17:15 Chchlee
 * prepared for moving getConfiguration to a file common for all DSPs
 * 
 * 44    16.09.05 14:31 Chhamaa
 * Floating point check added for rx signal from down converter
 * (rxSignalFloat) 
 * 
 * 43    16.09.05 11:34 Chhamaa
 * Float check for tx- and rx-signals modified
 * 
 * 42    15.09.05 18:26 Chhamaa
 * Float check for tx- and rx-signals added
 * 
 * 41    19.08.05 16:52 Chhamaa
 * segment ("seg_dmData") replaced with SEGMENT_DM_DATA2
 * 
 * 40    26.07.05 13:25 Chhamaa
 * More debug info added
 * 
 * 39    25.07.05 18:14 Chhamaa
 * g_FE_DSP_Status.APLC.rxTestToneLevel[APLC_CH1] used for debug info
 * 
 * 38    21.07.05 16:17 Chhamaa
 * Hybrid tuning / Rx RF level adj. bug fixed
 * 
 * 37    21.07.05 10:44 Chhamaa
 * OFDM Rx signal set to zero  if not OFDM_Enable
 * 
 * 36    20.07.05 10:06 Chhamaa
 * Tx signal scaling and OFDM PAR (new 10 dB) changed
 * 
 * 35    11.07.05 19:09 Chmibuh
 * g_FE_DSP_Alarm.rxRfOverflow added
 * 
 * 34    5.07.05 14:03 Chhamaa
 * tdmSyncAD6620() added
 * 
 * 33    28.06.05 11:56 Chchlee
 * Cleaned up, ASSERTs consolidated
 * 
 * 32    24.06.05 16:42 Chhamaa
 * TX_PEAK_AMPLITUDE modified
 * 
 * 31    20.06.05 17:33 Chhamaa
 * upDownConvTdmBus_FE_read/write functions moved close to while(1)
 * 
 * 30    8.06.05 11:59 Chhamaa
 * Pilot Tx signal insertion moved (after AP filter)
 * 
 * 29    2.06.05 13:29 Chbrbus
 * made new init function to reset AD6620 and ADS1605. Loading of AD6620
 * registers ans resetting is now in 2 functions. They messed up the
 * upconverter FS timing restrictions.
 * 
 * 28    23.05.05 11:42 Chchlee
 * some init functions moved to external RAM
 * 
 * 27    19.05.05 11:18 Chmibuh
 * memoryCheckRespondToMaster();  added
 * dummy memoryCheck removed
 * 
 * 26    13.05.05 15:03 Chmibuh
 * MemoryCheck after Init_TDM_BufferPtr
 * 
 * 25    12.05.05 18:33 Chmibuh
 * 
 * 24    11.05.05 18:48 Chmibuh
 * DoMemoryCheck(TRUE) removed temporary
 * 
 * 23    9.05.05 11:53 Chbrbus
 * MemoryCheck and Checksum patcher in batch file added - CAUTION: I had
 * to remove the -e switch
 * 
 * 22    14.04.05 18:34 Chhamaa
 * 
 * 21    12.04.05 15:53 Chhamaa
 * New AP filter functions added
 * 
 * 20    11.04.05 17:57 Chhamaa
 * 
 * 19    8.04.05 17:32 Chhamaa
 * Rx IF signal scaling added
 * 
 * 18    8.04.05 11:37 Chhamaa
 * 
 * 17    7.04.05 16:41 Chhamaa
 * 
 * 16    1.04.05 17:03 Chhamaa
 * 
 * 15    29.03.05 17:50 Chhamaa
 * 
 * 14    29.03.05 16:31 Chhamaa
 * 
 * 13    4.03.05 19:02 Chhamaa
 * Modifications to configure pilot weight by HMI
 * 
 * 12    2.03.05 13:43 Chhamaa
 * 
 * 11    25.02.05 18:16 Chhamaa
 * 
 * 10    24.02.05 18:52 Chhamaa
 * 
 * 9     21.02.05 17:03 Chhamaa
 * 
 * 8     18.02.05 19:25 Chhamaa
 * 
 * 7     18.02.05 11:32 Chchlee
 * temporarly commented out assert in operation mode
 * 
 * 6     17.02.05 18:45 Chhamaa
 * initCommissioningCfg() added
 * 
 * 5     16.02.05 18:08 Chhamaa
 * 
 * 130   16.02.05 17:24 Chhamaa
 * Msg e_TDMServiceMsgType_OperationMode_Send implemented
 * 
 * 129   14.02.05 18:39 Chhamaa
 * Message handling for test PLC link modified
 * 
 * 128   9.02.05 17:23 Chhamaa
 * Tx signal scaling modified
 * 
 * 127   8.02.05 11:49 Chhamaa
 * Test switch functions moved to testPlcLink.c
 * 
 * 126   2.02.05 18:15 Chhamaa
 * Define PN_LEVEL  1.4 added
 * 
 * 125   28.01.05 18:23 Chhamaa
 * equalizer added
 * 
 * 124   27.01.05 19:10 Chhamaa
 * TDM_SERVICE_RX_BUF_SIZE increased
 * 
 * 123   19.01.05 13:00 Chpeplu
 * new variable g_pilotEnableCh3 added and get pilot enable signal now
 * from BoostMgrOutput instead of BoostMgrInput
 * 
 * 122   14.01.05 17:27 Chpeplu
 * internal TDM-Bus:  snr to nsd added
 * 
 * 121   12.01.05 19:20 Chhamaa
 * 
 * 120   11.01.05 10:11 Chhamaa
 * 
 * 119   6.01.05 13:16 Chhamaa
 * 
 * 118   6.01.05 10:13 Chhamaa
 * Noise generator for channel measurement added
 * 
 * 117   5.01.05 15:01 Chhamaa
 * Test switches implemented
 * 
 * 116   4.01.05 18:18 Chhamaa
 * Test switch messages added
 * 
 * 115   4.01.05 11:49 Chhamaa
 * Floating point check for input TDM bus data added (ArrayFloatCheck()) 
 * 
 * 114   20.12.04 12:47 Chchlee
 * break added in Status Req
 * 
 * 113   17.12.04 17:50 Chhamaa
 * 
 * 112   16.12.04 17:05 Chhamaa
 * txAfSignalProcessing() created
 * 
 * 111   8.12.04 18:14 Chhamaa
 * AFC added
 * 
 * 110   1.12.04 12:06 Chhamaa
 * case e_TDMServiceMsgType_StatusData_Req added
 * 
 * 109   30.11.04 17:01 Chhamaa
 * TX_PEAK_AMPLITUDE_46dBm modified
 * 
 * 108   30.11.04 15:37 Chhamaa
 * 
 * 107   29.11.04 12:03 Maag01
 * FE_DSP_Alarm_updateAplcAlarms() added
 * 
 * 106   26.11.04 18:35 Maag01
 * 
 * 105   23.11.04 17:05 Maag01
 * 
 * 104   19.11.04 17:13 Maag01
 * initDataConverters() modified
 * 
 * 103   18.11.04 18:19 Maag01
 * 
 * 102   18.11.04 13:43 Maag01
 * 
 * 101   17.11.04 20:55 Leeb02
 * g_niReceiveFlag bug fixed
 * 
 * 100   17.11.04 16:24 Maag01
 * Sampling implemented
 * 
 * 99    15.11.04 8:33 Leeb02
 * function calls in ASSERT() removed
 * 
 * 98    12.11.04 16:46 Maag01
 * Ack for config data is sent in getConfiguration()
 * 
 * 97    10.11.04 11:35 Maag01
 * 
 * 96    9.11.04 10:22 Maag01
 * 
 * 95    4.11.04 19:27 Maag01
 * 
 * 94    4.11.04 11:46 Maag01
 * 
 * 93    2.11.04 10:36 Maag01
 * Case e_TDMServiceMsgType_Cfg_Send added in main loop message handler
 * 
 * 92    1.11.04 18:17 Maag01
 * 
 * 91    1.11.04 18:14 Maag01
 * AP filter moved from ssbDemodulator to ssbModulator
 * 
 * 90    29.10.04 17:07 Maag01
 * APLC_CHANNEL_MODE_A1_40I_A2_40I_A3_40I added
 * 
 * 89    27.10.04 14:40 Maag01
 * 
 * 88    12.10.04 7:37 Rytz01
 * #define DISABLE_OFDM and #define DISABLE_NSK inactiv
 * 
 * 87    8.10.04 16:08 Rytz01
 * #define DISABLE_OFDM and #define DISABLE_NSK
 * activ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * 
 * 86    8.10.04 14:00 Maag01
 * 
 * 85    6.10.04 17:48 Maag01
 * 
 * 84    6.10.04 11:49 Maag01
 * 
 * 83    5.10.04 19:31 Maag01
 * 
 * 82    5.10.04 9:50 Leeb02
 * #defines added for conditional disabling OFDM, NSD and NSK
 * 
 * 
 *---------------------------------------------------------------------------*/

// for testing

//#define DISABLE_OFDM
//#define DISABLE_NSD
//#define DISABLE_NSK


/* INCLUDE FILES *************************************************************/

asm("#include <def21161.h>");
#include <def21161.h>
#include <math.h>

#include <stdlib.h>
#include <math.h>
#include "FE_DSP_Main.h"
#include "DIAG.H"
#include "basetype.h"
#include "speeddsp.h"
#include "FE_DSP_Const.h"
#include "FE_DSP_Types.h"
#include "FE_DSP_Alarm.h"
#include "NSD_Types.h"
#include "idleCounter.h"
#include "idleCounter.h"
#include "Timer.h"
#include "tdm_int_types.h"
#include "upDownConvTdmBus.h"
#include "Init_FE_DSP.h"
#include "oscillator.h"
#include "sampling.h"
#include "commissioning.h"

#include "tdm_service.h"
#include "msg_P4LQ.h"
#include "tdmEocData.h"

// APLC
#include "Default.h"
#include "P4LT_Types.h"
#include "ssb.h"
#include "pilotMod.h"
#include "TestTone.h"
#include "testPlcLink.h"
#include "SignalGen.h"
#include "intDec.h"
#include "freqShift.h"
#include "afc.h"
#include "EQ_CALCU.H"
#include "equalizerFirFilter.h"
#include "apFilter.h"

// Memory Check
#include "MemoryCheck.h"
#include "SupervisionUC_bus_FE_DSP.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define S_VAL_ADJ_COEF				2.0		// SSB	Modulation reduces signal levels by factor 2.0
#define OFDM_PEAK_AMPLITUDE_40W		3.16228	// OFDM tx signal peak amplitude (PAR: 10 dB = 3.16228, 11 dB = 3.54813, 12 dB = 3.98107, 13 dB = 4.4668)
#define OFDM_PEAK_AMPLITUDE_40W_I	3.54813	// OFDM tx signal peak amplitude (PAR: 11 dB = 3.54813)
#define OFDM_PEAK_AMPLITUDE_80W		3.54813	// OFDM tx signal peak amplitude (PAR: 11 dB = 3.54813)
#define OFDM_PEAK_AMPLITUDE_80W_I	3.98107	// OFDM tx signal peak amplitude (PAR: 12 dB = 3.98107)
#define TX_PEAK_AMP_COEF_10W		0.48978	// -6.2 dB compared to DAC full scale (= 1.0), -3.2 dB = 0.6918
#define TX_PEAK_AMP_COEF_20W		0.48978	// -6.2 dB compared to DAC full scale (= 1.0), -3.2 dB = 0.6918
#define TX_PEAK_AMP_COEF_40W		0.54954	// -5.2 dB compared to DAC full scale (= 1.0)
#define TX_PEAK_AMP_COEF_80W		0.54954	// -5.2 dB compared to DAC full scale (= 1.0)
#define RX_IF_SCALING_COEF			0.12987	// = 1/7.7 scales ADC full scale signal to 1.0
#define PN_LEVEL					1.4		// pseude noise level used for channel measurement
#define PEP_RF_LOOPBACK				33		// [dBm] = 2 W (in case of RF loopback the output power has to be reduced)

#define TDM_SERVICE_RX_BUF_SIZE		300
#define TDM_SERVICE_TX_BUF_SIZE		100

#define GET_CFG_SEND_ACK_CYCLES		20

#define NAN							0xffffffff	// representation of NAN (Not-A-Number)

#define FILTER_RESET_COUNTER_MAX	50			// max. number of filter resets in case of overflow before board reset
#define TIME_COUNTER_MAX			10667		// = 10s time interval to count filter resets


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx and tx pointers
#define g_intTDM_RxStruct0_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr[0])
#define g_intTDM_RxStruct1_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr[1])

#define g_intTDM_TxStruct0_Ptr  ((t_FE_output_tdm_block*)internalTDM_Tx_Ptr[0])
#define g_intTDM_TxStruct1_Ptr  ((t_FE_output_tdm_block*)internalTDM_Tx_Ptr[1])


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

// Defined in Interrupt.asm as global
extern volatile long SPORT02InterruptCounter;

volatile long *rxBufferPtr;
volatile long *txBufferPtr;

static t_FrontEndDSP_Config g_FE_DSP_InitialConfig; // initial configuration received from Master DSP
static t_FrontEndDSP_Config g_FE_DSP_Config;		// configuration used during run time (channelMode is changed by initAPLC())
static t_FrontEndDSP_Status g_FE_DSP_Status;
static t_FE_Alarm g_FE_DSP_Alarm = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static t_TestPlcLinkData g_testPlcLinkConfig;
static t_TestSwitches g_testSwitches = {0,0,0,0,0,0,0,0,0,0};

static BOOL g_disconnect;
static BOOL g_inputCmdActive;

static unsigned long g_operationMode;

// RF parameters
static long	 g_rfChannelMode;
static float g_txPeakAmpCoef;
static float g_txSignalCoef;
static float g_txSignalCoefNormalOperation;
static float g_txSignalCoefLoopback;			// reduced output power in case of RF loopback
static float g_powerReductionCoef;
static float g_powerReductionCoefLoopback;

// OFDM signal scaling
static float g_OFDM_TxSignalCoef;

// APLC
static long g_numOfAplcChannels;
static float g_rxAFLevelAdjust[N_APLC_CHANNELS];

static GuardConfigType g_guradConfig[N_APLC_CHANNELS];

static agcDataType g_agcData[N_APLC_CHANNELS];
static BOOL g_afcEnable;

static t_AFC g_afc;

// sample arrays
static float g_txNsd[N_APLC_CHANNELS*BLOCK_LENGTH_C_4K];		// ch1, ch2, ch3 (complex)
static float g_txNsk[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];		// ch1, ch2, ch3 (from NSK)
static float g_txNsk_noAP[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];	// ch1, ch2, ch3 (from NSK, bypass allpass)

static float g_txAf[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];			// ch1, ch2, ch3
static float g_txAfSum[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];		// ch1, ch2, ch3

static float g_rxAfNe[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];		// ch1, ch2, ch3

static float g_rxAfNeScaled[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];	// ch1, ch2, ch3
static float g_rxAfEqScaled[N_APLC_CHANNELS*BLOCK_LENGTH_R_4K];	// ch1, ch2, ch3

// TPE pilot enable signals
static BOOL g_pilotEnableTPE[N_APLC_CHANNELS];

// Pseudo noise variables
static BOOL g_pnGeneratorEnable[N_APLC_CHANNELS];
static float g_pnLevel;

// SSB data
static t_ssbData1Ch g_ssbDataCh1;
static t_ssbData1Ch g_ssbDataCh3;

// pilot data
static PilotModDataType		g_pilotMod[N_APLC_CHANNELS];
static PilotDemodDataType	g_pilotDemod[N_APLC_CHANNELS];
static PilotRecDataType		g_pilotRec[N_APLC_CHANNELS];
static GuardDataType		g_guardStatus[N_APLC_CHANNELS];

// test tone data
static TestToneDataType g_testTone[N_APLC_CHANNELS];
static float			g_ttLevel[N_APLC_CHANNELS];
static float			g_ttLevelTxAlarmAdj;

// oscillator data for frequency shift
static t_OscData g_oscShiftTxAPLC;
static t_OscData g_oscShiftRxAPLC; 
static t_OscData g_oscShiftTxCh3; 
static t_OscData g_oscShiftRxCh3; 

// interpolation data
static t_IntPolyM2Data g_intPolyM2Data;
static t_IntPolyM2Data g_intPolyM2DataCh3; 

// decimation data
static t_DecM2Data g_decM2Data;
static t_DecM2Data g_decM2DataCh3;

// TDM service variables
static unsigned long        g_niRxData[TDM_SERVICE_RX_BUF_SIZE];	// receive buffer
static unsigned long		g_niTxData[TDM_SERVICE_TX_BUF_SIZE];	// send buffer
static BOOL                 g_niReceiveFlag;
static e_TDMServiceMsgType	g_niRxMsgType;
static unsigned long	    g_niRxMsgSize;

// for testing
//float inPhase[2];
//float quadrature[2];
//float weightAplcSignal;


/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void getConfiguration(t_FrontEndDSP_Config *config);
static void initAPLC(void);
static void initSSB(long nChannels);
static void initTxSignalPath(void);
static void initDataConverters(long rxFreq, long channelMode);
static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize);
static void niConfirm (void);
static void txAfSignalProcessing(void);
static float *getSamplingPtr(long signalPath, long signalSource);
static void initCommissioningMode(long operationMode);
static BOOL initCommissioningCfg(t_Commissioning *commissioningCfg);
static long calcRfLoopbackRxFreq(t_FrontEndDSP_Config *config);


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		getConfiguration()	
  Description:	Waits for the configuration sent by the master DSP
  Inputs:		config: pointer to configuration data structure 
  Outputs:		Received configuration data
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void SEGMENT_EXT_CODE getHMIConfiguration(void *config, unsigned long sizeOfConfig, 
												 e_TDMServiceAddr    TDMServiceAddr,
												 e_TDMServiceMsgType TDMServiceMsgType,
												 BOOL bIs5kHz);


static void SEGMENT_EXT_CODE getConfiguration(t_FrontEndDSP_Config *config)
{
	getHMIConfiguration(config, sizeof(*config), 
		                e_TDMServiceAddr_FE, e_TDMServiceMsgType_Cfg_Send, 
						TRUE); // 5.33kHz task
}

static void SEGMENT_EXT_CODE getHMIConfiguration(void *config, unsigned long sizeOfConfig, 
												 e_TDMServiceAddr    TDMServiceAddr,
												 e_TDMServiceMsgType TDMServiceMsgType,
												 BOOL bIs5kHz)
{
	long i;
	e_TDMServiceResult tdmServiceResult;


	tdmServiceNI_Init(TDMServiceAddr,  // initialize non-initiator TDM service
					  g_niRxData,
					  sizeof(g_niRxData) / sizeof(unsigned long),
					  &niReceive,
					  &niConfirm);

	niConfirm(); // enable TDM service receiver

	// wait for the configuration
	while(!g_niReceiveFlag) // the flag is set when the TDM service received a valid configuration
	{
		// repetiton rate = 5.333 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		IR_BlockCounter = bIs5kHz?2:1;

		if (bIs5kHz)
		{
			RestartTimer5k();
		}
		else
		{
			RestartTimer10k();
		}

		IR_TimerCounter = 1;

		if (bIs5kHz)
		{
			tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[0], (unsigned long *) internalTDM_Tx_Ptr[0]);
		}

		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[1], (unsigned long *) internalTDM_Tx_Ptr[1]);
	}

	g_niReceiveFlag = FALSE;

	// configuration received
	ASSERT(TDMServiceMsgType == g_niRxMsgType);	// check if tx message type is correct
	ASSERT(sizeOfConfig == g_niRxMsgSize);		// check if size is correct

	// send TDM service reply
	tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    				                       e_TDMServiceMsgType_Ack); // txMsgType = acknowledge

	ASSERT(tdmServiceResult == e_TDMServiceResult_Ok);

	MEMCPY (config, g_niRxData, sizeOfConfig); // copy received message into g_FE_DSP_Config

	// tdmServiceActivateNonInitiator() is called to send ack
	for(i = 0; i < GET_CFG_SEND_ACK_CYCLES; i++)
	{
		// repetiton rate = 5.333 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		IR_BlockCounter = bIs5kHz?2:1;

		if (bIs5kHz)
		{
			RestartTimer5k();
		}
		else
		{
			RestartTimer10k();
		}

		IR_TimerCounter = 1;
	
		if (bIs5kHz)
		{
			tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[0], (unsigned long *) internalTDM_Tx_Ptr[0]);
		}
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[1], (unsigned long *) internalTDM_Tx_Ptr[1]);
	}
}


static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize)
{
	g_niReceiveFlag = TRUE;
	g_niRxMsgType = rxMsgType;
	g_niRxMsgSize = rxMsgSize;
}


static void niConfirm (void)
{
	tdmServiceNI_Receive();
}


/*****************************************************************************
  Function:		initAPLC(void)	
  Description:	Initilizes all APLC functions
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		Pilot, testTone and SSB configuration data and parameters
  Important:	channelMode of struct g_FE_DSP_Config is modified in case of
				teleprotection placed in the DPLC band or DPLC only.

******************************************************************************/

void SEGMENT_EXT_CODE initAPLC(void)
{
	float rfPepMax_dBm;
	float txSignalPeakAmplitude;
	float txSignalPeakAmplitudeRfLoopback;
	float ttLevelTxAlarmAdjDB;
	float exponent;
	float c;
	long i;
	BOOL dplcOnly;


	g_operationMode = OP_MODE_NORMAL;

	// use g_FE_DSP_InitialConfig to set dplcOnly because channelMode is modified in same cases
	dplcOnly = (g_FE_DSP_InitialConfig.channelMode == APLC_CHANNEL_MODE_NONE) && (g_FE_DSP_InitialConfig.DPLC_BW > 0);

	// In case of teleprotection placed in the DPLC band or DPLC only, the APLC channel mode
	// has to be modified to generate the APLC pilot signal.
	// The pilot frequency is set to 3840 Hz by HMI600. 
	// @Pl_ext
	if (((g_FE_DSP_Config.intTeleprotMode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_DPLC) || dplcOnly)
	{
		g_FE_DSP_Config.channelMode = APLC_CHANNEL_MODE_A1_40I;
	};
	// @Pl_ext
	if ((g_FE_DSP_Config.intTeleprotMode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC) {
	
		g_FE_DSP_Config.channelMode = APLC_CHANNEL_MODE_A1_40I_A2_40I;
	};

	// get number of APLC channels
	CNT_APLC_CHANNELS(g_FE_DSP_Config, g_numOfAplcChannels);

	// initialize SSB
	initSSB(g_numOfAplcChannels);

	// set test switches to default values
	g_testPlcLinkConfig.testSwitches = testPlcLink_getTestSwitches(&g_testSwitches);

	for (i = 0; i < N_APLC_CHANNELS; i++) {

		// set test tone config to default values
		g_testPlcLinkConfig.testTone[i].frequency = DEFAULT_TESTTONE_FREQUENCY_IN_HZ;
		g_testPlcLinkConfig.testTone[i].levelLin = DEFAULT_TESTTONE_LEVEL_LIN;
		g_testPlcLinkConfig.testTone[i].level_dB = DEFAULT_TESTTONE_LEVEL_IN_DB;

		// initialize test tone level
		g_ttLevel[i] = g_testPlcLinkConfig.testTone[i].levelLin;

		// initialize equalizer
		equalizerFirFilter_init(&g_FE_DSP_Config.equalizerFirCoef[i], i);

		// initialize AF level adjustment factor for APLC channnels
		g_rxAFLevelAdjust[i] = g_FE_DSP_Config.rxAFLevelAdjust[i];
		
	};

	// initialize AP filters
	apFilter_init(&g_FE_DSP_Config);
	
	// initialize pilot modems
	initPilotModemCh1(&g_FE_DSP_Config.pilot[APLC_CH1], &g_pilotMod[APLC_CH1], &g_pilotDemod[APLC_CH1], 
					  &g_pilotRec[APLC_CH1], &g_guradConfig[APLC_CH1], &g_guardStatus[APLC_CH1],
					  &g_agcData[APLC_CH1]);
			  
	initPilotModemCh2(&g_FE_DSP_Config.pilot[APLC_CH2], &g_pilotMod[APLC_CH2], &g_pilotDemod[APLC_CH2],
					  &g_pilotRec[APLC_CH2], &g_guradConfig[APLC_CH2], &g_guardStatus[APLC_CH2],
					  &g_agcData[APLC_CH2]);

			  
	initPilotModemCh3(&g_FE_DSP_Config.pilot[APLC_CH3], &g_pilotMod[APLC_CH3], &g_pilotDemod[APLC_CH3],
					  &g_pilotRec[APLC_CH3], &g_guradConfig[APLC_CH3], &g_guardStatus[APLC_CH3],
					  &g_agcData[APLC_CH3]);

	// initialize test tones
	initTestToneCh1(&g_testPlcLinkConfig.testTone[APLC_CH1], &g_testTone[APLC_CH1]);
	initTestToneCh2(&g_testPlcLinkConfig.testTone[APLC_CH2], &g_testTone[APLC_CH2]);
	initTestToneCh3(&g_testPlcLinkConfig.testTone[APLC_CH3], &g_testTone[APLC_CH3]);

	// initialize interpolation
	initIntPolyM2(&g_intPolyM2Data);
	initIntPolyM2Ch3(&g_intPolyM2DataCh3);
	initIntPolyM3();
	
	// initialize decimation
	initDecPolyM3();
	initDecM2(&g_decM2Data);
	initDecM2Ch3(&g_decM2DataCh3);
	
	// initialize frequency up/down shift
	initFreqShiftCh3(&g_oscShiftTxCh3, &g_oscShiftRxCh3, g_FE_DSP_Config.channelMode);
	initFreqShiftAPLC(&g_oscShiftTxAPLC, &g_oscShiftRxAPLC, g_FE_DSP_Config.channelMode);

	// initialize RF channnel mode
	g_rfChannelMode = g_FE_DSP_Config.RF.channelMode;

	// correct rfPepMax in case of HMI setting RF-PEP = 12.5 W
	if (g_FE_DSP_Config.rfPepMax == 12) c = 0.5;
	else c = 0;

	// calculate max. equipment power in dBm 
	rfPepMax_dBm = 10*log10((g_FE_DSP_Config.rfPepMax + c)/0.001);
	
	// calculate power reduction
	g_powerReductionCoef = pow(10.0, (g_FE_DSP_Config.RF.PEP_Value - rfPepMax_dBm)/20.0);  // PEP: [dBm]

	// calculate power reduction for RF loopback (output power has to be lower than in normal operation) 
	g_powerReductionCoefLoopback = pow(10.0, (PEP_RF_LOOPBACK - rfPepMax_dBm)/20.0);
	
	// get tx peak amplitude coef
	switch(g_FE_DSP_Config.rfPepMax) {

		case 10:
		case 12:
			
			g_txPeakAmpCoef = TX_PEAK_AMP_COEF_10W;
			break;

		case 20:
		case 25:

			g_txPeakAmpCoef = TX_PEAK_AMP_COEF_20W;
			break;

		case 40:
		case 50:

			g_txPeakAmpCoef = TX_PEAK_AMP_COEF_40W;
			break;

		case  80:
		case 100:

			g_txPeakAmpCoef = TX_PEAK_AMP_COEF_80W;
			break;

		default: 

			g_txPeakAmpCoef = TX_PEAK_AMP_COEF_10W;
	}

	// adjust peak amplitude according desired output power
	txSignalPeakAmplitude = g_powerReductionCoef*g_txPeakAmpCoef;
	txSignalPeakAmplitudeRfLoopback = g_powerReductionCoefLoopback*g_txPeakAmpCoef;

	// tx signal scaling factor for peak amplitude = 1.0
	g_txSignalCoef = g_FE_DSP_Config.RF.sumSValuesInv*S_VAL_ADJ_COEF;
	g_txSignalCoefLoopback = g_FE_DSP_Config.RF.sumSValuesInv*S_VAL_ADJ_COEF;

	// adjust tx signal scaling factor according to ouput power adjustment
	g_txSignalCoef *= g_FE_DSP_Config.RF.txLevelAdjustment*txSignalPeakAmplitude;
	g_txSignalCoefLoopback *= g_FE_DSP_Config.RF.txLevelAdjustment*txSignalPeakAmplitudeRfLoopback;

	// backup g_txSignalCoef
	g_txSignalCoefNormalOperation = g_txSignalCoef;

	// initialize OFDM scaling factor
	if (((g_FE_DSP_Config.rfPepMax == 80) || (g_FE_DSP_Config.rfPepMax == 100)) && dplcOnly)
	{
		g_OFDM_TxSignalCoef = g_FE_DSP_Config.weightOFDM_Signal/OFDM_PEAK_AMPLITUDE_80W_I/S_VAL_ADJ_COEF;
	}
	else if ((g_FE_DSP_Config.rfPepMax == 80) || (g_FE_DSP_Config.rfPepMax == 100))
	{
		g_OFDM_TxSignalCoef = g_FE_DSP_Config.weightOFDM_Signal/OFDM_PEAK_AMPLITUDE_80W/S_VAL_ADJ_COEF;
	}
	else if (((g_FE_DSP_Config.rfPepMax == 40) || (g_FE_DSP_Config.rfPepMax == 50)) && dplcOnly)
	{
		g_OFDM_TxSignalCoef = g_FE_DSP_Config.weightOFDM_Signal/OFDM_PEAK_AMPLITUDE_40W_I/S_VAL_ADJ_COEF;
	}
	else
	{
		g_OFDM_TxSignalCoef = g_FE_DSP_Config.weightOFDM_Signal/OFDM_PEAK_AMPLITUDE_40W/S_VAL_ADJ_COEF;
	}

	// set noise level for channel measurement
	g_pnLevel = PN_LEVEL;

	// initialize AFC
	g_afcEnable = g_FE_DSP_Config.AFC_Enabled;
	afc_init(&g_afc, g_FE_DSP_Config.channelMode);

	// calculate test tone level for Tx alarm level adj.
	// used only for ETL600 R4
	ttLevelTxAlarmAdjDB = COMM_TT_ATT_TX_ALARM - (g_FE_DSP_Config.RF.PEP_Value - PEP_RF_LOOPBACK);
	g_ttLevelTxAlarmAdj = pow(10.0, -ttLevelTxAlarmAdjDB/20.0);

}


/*****************************************************************************
  Function:		initSSB()	
  Description:	Initilizes SSB modulation based on the number of channels used.
  Inputs:		nChannels
  Outputs:		-
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void SEGMENT_EXT_CODE initSSB(long nChannels) {

	switch(nChannels)
	{
		case 0:

			// do nothing
			break;

		case 1:
			
			// 1 channel SSB ch1
			initSSB1Ch_Ch1(&g_FE_DSP_Config, &g_ssbDataCh1);		
			break;
							  
		case 2:
			
			// 2 channel SSB for ch1 and ch2
			initSSB2Ch(&g_FE_DSP_Config);
			break;
								
	 	case 3:
			
			// 2 channel SSB for ch1 and ch2, 1 channel SSB for ch3
			initSSB2Ch(&g_FE_DSP_Config);
			initSSB1Ch_Ch3(&g_FE_DSP_Config, &g_ssbDataCh3);
			break;
								
		default: ASSERT(FALSE);
	};
}


/*****************************************************************************
  Function:		initTxSignalPath()	
  Description:	Initializes tx signal path.
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void SEGMENT_EXT_CODE initTxSignalPath(void)
{
	// initialize AP filters
	apFilter_init(&g_FE_DSP_Config);

	// initialize SSB
	initSSB(g_numOfAplcChannels);

	// initialize interpolation
	initIntPolyM2(&g_intPolyM2Data);
	initIntPolyM2Ch3(&g_intPolyM2DataCh3);
	initIntPolyM3();

	// initialize OFDM filters
	txSignalProcessingOfdm_init(&g_FE_DSP_InitialConfig);
}


/*****************************************************************************
  Function:		nanCheck()	
  Description:	Checks if a float value is not-a-number.
  Inputs:		Float value
  Outputs:		True if value is not-a-number, else false.
  Return:		-
  Globals:		none
  Important:	-

******************************************************************************/

static BOOL nanCheck(float fVal)
{

	if (((long)fVal) == NAN) {

		return(TRUE);
	}
	else {

		return(FALSE);
	};
}


/*****************************************************************************
  Function:		resetDataConverters()	
  Description:	Resets ADC and down converter
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		none
  Important:	CAUTION: this fuctions only makes the reset pulses for the 
				AD6620 and the ADS1605. They had to be seperated because the 
				AD6620 had to be initialized immediately after the SPORT13 interrupt.

******************************************************************************/

void SEGMENT_EXT_CODE resetDataConverters(void)
{
	long tempWait = 0;

	// reset ADS1605 active low
	*(long*)(IOFLAG) &= ~(FLG5);
    for(tempWait = 200;tempWait;tempWait--)	
	{
		asm("nop;");
	}
	// now reset the AD6620
	AD6620_HW_Reset();
	*(long*)(IOFLAG) |= FLG5;

}


/*****************************************************************************
  Function:		initDataConverters()	
  Description:	Initilizes ADC and down converter
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		none
  Important:	CAUTION: This function could only be called immediately after an
				SPORT13 interrupt because the FS of the upconverter bus is
				generated by the AD6620. We reduced all the transmit / receive 
				buffers so there is a strict timing requirement between the FS
				SPORT13 and SPORT02. This has a great impact on commissionning functions.

******************************************************************************/
 
void SEGMENT_EXT_CODE initDataConverters(long rxFreq, long channelMode)
{
	long rxCenterFreq;

	// calculate rx center frequency
	if (channelMode == RF_CHANNEL_MODE_ERECT) {

		rxCenterFreq = rxFreq + 0.5*RF_BANDWIDTH;
	}
	else {
		
		rxCenterFreq = rxFreq - 0.5*RF_BANDWIDTH;
	};
	
	// initialize down converter
	AD6620_Init_Test(rxCenterFreq); 
    AD6620_Start();
}


/*****************************************************************************
  Function:		tdmSyncAD6620()	
  Description:	Sync procedure for AD6620 initialization
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		
  Important:	Call just before initDataConverters() is used

******************************************************************************/

void tdmSyncAD6620(void)
{
	IR_BlockCounter = 1;
	while(IR_BlockCounter)
		{
			// What do we do if internal TDM bus is not running ???
		}
	// Now wait a little bit shorter than the next interrupt (optimally one 10.666 kHz cycle - init time AD6620)
	asm("bit clr mode2 TIMEN;\
		 TCOUNT=9020;\
		 bit set mode2 TIMEN;"); 
	IR_TimerCounter = 1;
	while(IR_TimerCounter)
		{
		}
	// Now initialize AD6620 - so FS on FE Bus are synchronized with FS on internal TDM bus
}

	
/* MAIN **********************************************************************/


void main ( void )
{
	
	// idle counters
	static unsigned long startValue;
	static unsigned long idleCounter;
	static unsigned long idleCounterMinIndex;
	
	// case counters
	static unsigned long mainCaseCounter = 0;
	static unsigned long subCaseCounter4 = 0;

	// life sign enable
	static BOOL lifeSignEnable = TRUE;

	// transmit buffer for EOC data
	static EocDataBufferType txBufferEoc[N_APLC_CHANNELS];
	
	// receive buffer for EOC data
	static EocDataBufferType rxBufferEoc[N_APLC_CHANNELS];
	
	// AGC blocking
	static long agcBlocking = 0;

	// AFC, AGC disable flags for equalizer channel measurement
	static BOOL eqAfcDisable;
	static BOOL eqAgcDisable[N_APLC_CHANNELS];

	// gain factors
	static float gainRxAf[N_APLC_CHANNELS];
	
	// E-wire
	static long EWire[N_APLC_CHANNELS];

	// status of test tone detection
	static long testToneDetected[N_APLC_CHANNELS];

	// AFC enable
	static BOOL afcEnable = 1;

	// OFDM enable
	static BOOL OFDM_Enable;

	// constant
	const complex_float zeroC = {0.0, 0.0};
	
	// sample arrays
	static float txAfSSBMod[BLOCK_LENGTH_C_4K];		// ch1, ch2 (complex)
	static float txAfSSBModCh3[BLOCK_LENGTH_C_4K];	// ch3      (complex)

	static float rxAfSSBMod[BLOCK_LENGTH_C_4K];		// ch1, ch2 (complex)
	static float rxAfSSBModCh3[BLOCK_LENGTH_C_4K];	// ch3		(complex)

	static float rxAfEq[3*BLOCK_LENGTH_R_4K];		// ch1, ch2, ch3

	// seg_dmData used for even word alignment (use only even numbers of vector elements)
	static float SEGMENT_DM_DATA2 int2Out2Ch[BLOCK_LENGTH_C_INT2];
	static float SEGMENT_DM_DATA2 int2OutCh3[BLOCK_LENGTH_C_INT2];
	static float SEGMENT_DM_DATA2 freqShiftTxOutCh3[BLOCK_LENGTH_C_INT2];
	static float SEGMENT_DM_DATA2 freqShiftRxOutCh3[BLOCK_LENGTH_C_INT2];
	static float SEGMENT_DM_DATA2 int3In[BLOCK_LENGTH_C_INT2];
	
	static float SEGMENT_DM_DATA2 int6Out[BLOCK_LENGTH_C_IF];
	static float SEGMENT_DM_DATA2 dec6In[BLOCK_LENGTH_C_IF];
	static float SEGMENT_DM_DATA2 dec3Out[BLOCK_LENGTH_C_INT2];
	
	static float SEGMENT_DM_DATA2 freqShiftTxOut[BLOCK_LENGTH_C_IF];
	static float SEGMENT_DM_DATA2 freqShiftRxIn[BLOCK_LENGTH_C_IF];
	
	static complex_float SEGMENT_DM_DATA2 txSignal_IF[N_SAMPLES_IF_C];
	static complex_float SEGMENT_DM_DATA2 txSignalAPLC_IF[N_SAMPLES_IF_C];
	static complex_float SEGMENT_DM_DATA2 txSignalOFDM_IF[N_SAMPLES_IF_C];
	static complex_float SEGMENT_DM_DATA2 txSignalLpOFDM_IF[N_SAMPLES_IF_C];
	static complex_float SEGMENT_DM_DATA2 rxSignal_IF[N_SAMPLES_IF_C];
	static complex_float SEGMENT_DM_DATA2 zeroSignal[N_SAMPLES_IF_C/2];

	static long  SEGMENT_DM_DATA2 rxSignalFixed[BLOCK_LENGTH_C_IF];
	static float SEGMENT_DM_DATA2 rxSignalFloat[BLOCK_LENGTH_C_IF];

	// for UC TDM bus DMA supervision
	static volatile long *LastValidDMA_Rx_BufferPtr;
	
	// sampling parameters
	t_Sampling sampling;


	Init_DSP_Register();
	asm("nop;");			// Emulator: run up to here and reload *.dxe again (SDRAM settings)

	// init SPORT DMA pointers - acessing the DMA pointer is possible anytime - no more: if (internalTDM_Tx_Ptr) ....
	Init_InternalTDM_BufferPtr((unsigned long*) &internalTDM_Rx_Ptr[0], (unsigned long*) &internalTDM_Tx_Ptr[0]);
	Init_InternalTDM_BufferPtr((unsigned long*) &internalTDM_Rx_Ptr[1], (unsigned long*) &internalTDM_Tx_Ptr[1]);
	Init_ExternalTDM_BufferPtr((unsigned long*) &externalTDM_Rx_Ptr[0], (unsigned long*) &externalTDM_Tx_Ptr[0]);
	Init_ExternalTDM_BufferPtr((unsigned long*) &externalTDM_Rx_Ptr[1], (unsigned long*) &externalTDM_Tx_Ptr[1]);

	MemoryCheckStartupSHARC(TRUE);	// TRUE = ext. SDRAM support

	memoryCheckRespondToMaster();	

	initialize_DSP();
	RestartTimer5k();
	IR_TimerCounter = 1;

	// get configuraion from master DSP
	getConfiguration(&g_FE_DSP_InitialConfig);

	// initialize working copy of FE DSP configuration
	MEMCPY(&g_FE_DSP_Config, &g_FE_DSP_InitialConfig, sizeof(t_FrontEndDSP_Config));

	// initialize APLC functions
	initAPLC();

	// initialize processing of OFDM tx signal
	txSignalProcessingOfdm_init(&g_FE_DSP_InitialConfig);

	// initialize sampling
	sampling_init(&sampling);

	// initialize EOC data transfer
	tdmEocData_init(txBufferEoc, rxBufferEoc);

	// initialize zero signal vector
	vset_c_D(zeroSignal, zeroC, N_SAMPLES_IF_C/2);
	
	// initialize ADC and down converter
	// WARNING: DO NOT CHANGE ANYTHING WITHOUT KNOWING WHAT YOU ARE DOING !!!!!!!!!!!!!!!!!!!!!!!!!!!
	// This is important to syncronize FS on internal with UC TDM Bus - must be independent from running time
	// of previous init functions
	// Important to save transmission time and TDM buffer size between TDM busses  -  T0 time NSD

	// Wait for next Interrupt internal TDM Bus
	resetDataConverters();		// do a reset of AD6620 and ADS1605
	IR_BlockCounter = 1;
	while(IR_BlockCounter)
		{
			// What do we do if internal TDM bus is not running ???
		}
	// Now wait a little bit shorter than the next interrupt (optimally one 10.666 kHz cycle - init time AD6620)
	asm("bit clr mode2 TIMEN;\
		 TCOUNT=9020;\
		 bit set mode2 TIMEN;");
	IR_TimerCounter = 1;
	while(IR_TimerCounter)
		{
		}
	// Now initialize AD6620 - so FS on FE Bus are synchronized with FS on internal TDM bus
	initDataConverters(g_FE_DSP_Config.RF.rxFrequency, g_FE_DSP_Config.RF.channelMode);


    // main loop repetiton rate = 5.333 kHz
    while (1) {

		startValue = count_start();

		// repetiton rate = 5.333 kHz
		while(IR_BlockCounter && IR_TimerCounter)
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		idleCounter = count_end(startValue);
		idleCounter_update(idleCounterMinIndex, idleCounter);

		IR_BlockCounter = 2;
		RestartTimer5k();
		IR_TimerCounter = 1;

		// externalTDM_Rx_Ptr is the "interface" to the Downconverter to get the RX-samples over the SPORT0
		rxBufferPtr = externalTDM_Rx_Ptr;
		// externalTDM_Tx_Ptr is the "interface" to the UP_DSP to send the TX-samples over the SPORT2
		txBufferPtr = externalTDM_Tx_Ptr;


		// data exchange with up/down-converter
		//

		// get rx IF samples from down-converter TDM bus
		upDownConvTdmBus_FE_read(rxSignalFixed, rxBufferPtr);

		// write tx IF samples to up-converter TDM bus
		upDownConvTdmBus_FE_write(txSignal_IF, txBufferPtr);


		// read data from internal TDM bus
		//

		// get OFDM samples
		#ifndef DISABLE_OFDM
		vcopy_c_DD( txSignalOFDM_IF,    g_intTDM_RxStruct0_Ptr->OFDM_Modem_TxSignal, 6);
		vcopy_c_DD(&txSignalOFDM_IF[6], g_intTDM_RxStruct1_Ptr->OFDM_Modem_TxSignal, 6);

		ArrayFloatCheck((float*)txSignalOFDM_IF, 24);
		#endif

		// get boost manager data
		#ifdef DISABLE_NSD
		OFDM_Enable = TRUE;
		#else
		OFDM_Enable = !g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.disconnectOFDM &&
					  (g_operationMode == OP_MODE_NORMAL);
		#endif

		#ifndef DISABLE_NSD
		// get NSD signal Ch1
		g_txNsd[0] = g_intTDM_RxStruct0_Ptr->NSD_TxSignal[APLC_CH1].re;
		g_txNsd[1] = g_intTDM_RxStruct0_Ptr->NSD_TxSignal[APLC_CH1].im;
		g_txNsd[2] = g_intTDM_RxStruct1_Ptr->NSD_TxSignal[APLC_CH1].re;
		g_txNsd[3] = g_intTDM_RxStruct1_Ptr->NSD_TxSignal[APLC_CH1].im;

		// get NSD signal Ch2
		g_txNsd[4] = g_intTDM_RxStruct0_Ptr->NSD_TxSignal[APLC_CH2].re;
		g_txNsd[5] = g_intTDM_RxStruct0_Ptr->NSD_TxSignal[APLC_CH2].im;
		g_txNsd[6] = g_intTDM_RxStruct1_Ptr->NSD_TxSignal[APLC_CH2].re;
		g_txNsd[7] = g_intTDM_RxStruct1_Ptr->NSD_TxSignal[APLC_CH2].im;

		ArrayFloatCheck(g_txNsd, 8);
		#endif

		#ifndef DISABLE_NSK
		// get NSK signals which pass the allpass filter
		// get NSK signal Ch1
		g_txNsk[0] = g_intTDM_RxStruct0_Ptr->NSK600_ModemTxSignal[APLC_CH1];
		g_txNsk[1] = g_intTDM_RxStruct1_Ptr->NSK600_ModemTxSignal[APLC_CH1];

		// get NSK signal Ch2
		g_txNsk[2] = g_intTDM_RxStruct0_Ptr->NSK600_ModemTxSignal[APLC_CH2];
		g_txNsk[3] = g_intTDM_RxStruct1_Ptr->NSK600_ModemTxSignal[APLC_CH2];

		// get NSK signal Ch3
		g_txNsk[4] = g_intTDM_RxStruct0_Ptr->NSK600_ModemTxSignal[APLC_CH3];
		g_txNsk[5] = g_intTDM_RxStruct1_Ptr->NSK600_ModemTxSignal[APLC_CH3];

		ArrayFloatCheck(g_txNsk, 6);


		// get NSK signals which bypass the allpass filter
		// get APLC Ch1
		g_txNsk_noAP[0] = g_intTDM_RxStruct0_Ptr->NSK600_ModemTxSignal_noAP[APLC_CH1];
		g_txNsk_noAP[1] = g_intTDM_RxStruct1_Ptr->NSK600_ModemTxSignal_noAP[APLC_CH1];

		// get APLC Ch2
		g_txNsk_noAP[2] = g_intTDM_RxStruct0_Ptr->NSK600_ModemTxSignal_noAP[APLC_CH2];
		g_txNsk_noAP[3] = g_intTDM_RxStruct1_Ptr->NSK600_ModemTxSignal_noAP[APLC_CH2];

		// get APLC Ch3
		g_txNsk_noAP[4] = g_intTDM_RxStruct0_Ptr->NSK600_ModemTxSignal_noAP[APLC_CH3];
		g_txNsk_noAP[5] = g_intTDM_RxStruct1_Ptr->NSK600_ModemTxSignal_noAP[APLC_CH3];

		ArrayFloatCheck(g_txNsk_noAP, 6);

		#endif

		// get APLC Ch1
		g_txAf[0] = g_intTDM_RxStruct0_Ptr->APLC_TxSignalChannel[APLC_CH1];
		g_txAf[1] = g_intTDM_RxStruct1_Ptr->APLC_TxSignalChannel[APLC_CH1];

		// get APLC Ch2
		g_txAf[2] = g_intTDM_RxStruct0_Ptr->APLC_TxSignalChannel[APLC_CH2];
		g_txAf[3] = g_intTDM_RxStruct1_Ptr->APLC_TxSignalChannel[APLC_CH2];

		// get APLC Ch3
		g_txAf[4] = g_intTDM_RxStruct0_Ptr->APLC_TxSignalChannel[APLC_CH3];
		g_txAf[5] = g_intTDM_RxStruct1_Ptr->APLC_TxSignalChannel[APLC_CH3];

		ArrayFloatCheck(g_txAf, 6);

		// get M-Wire data (sampling rate 5.333 kHz)
		g_pilotMod[APLC_CH1].MWire = g_intTDM_RxStruct0_Ptr->M_WireSignallingAllChannels_TS05.channel1;
		g_pilotMod[APLC_CH2].MWire = g_intTDM_RxStruct0_Ptr->M_WireSignallingAllChannels_TS05.channel2;
		g_pilotMod[APLC_CH3].MWire = g_intTDM_RxStruct0_Ptr->M_WireSignallingAllChannels_TS05.channel3;

		// get pilot enable signals (sampling rate 5.333 kHz)
		#ifdef DISABLE_NSD
		g_pilotEnableTPE[APLC_CH1] = g_pilotEnableTPE[APLC_CH2] = g_pilotEnableTPE[APLC_CH3] = TRUE;
		#else
		g_pilotEnableTPE[APLC_CH1] = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.Pilot_Enable_Ch1;
		g_pilotEnableTPE[APLC_CH2] = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.Pilot_Enable_Ch2;
		g_pilotEnableTPE[APLC_CH3] = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.Pilot_Enable_Ch3;
		#endif

		// get TPS signals (sampling rate 5.333 kHz)
		#ifdef DISABLE_NSD
		g_disconnect = FALSE;
		g_inputCmdActive = FALSE;
		#else
		g_disconnect = g_intTDM_RxStruct0_Ptr->BoostMgrOutput_TS07.disconnect;
		g_inputCmdActive = g_intTDM_RxStruct0_Ptr->Channel1_InputCommandStatusData_TS12 ||
						   g_intTDM_RxStruct0_Ptr->Channel2_InputCommandStatusData_TS14;
		#endif

		// get noise generator enable flags (sampling rate 5.333 kHz)
		g_pnGeneratorEnable[APLC_CH1] = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.pnGenEnableCh1;
		g_pnGeneratorEnable[APLC_CH2] = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.pnGenEnableCh2;
		g_pnGeneratorEnable[APLC_CH3] = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.pnGenEnableCh3;

		// get AFC, AGC disable flags for equalizer channel measurement (sampling rate 5.333 kHz)
		eqAfcDisable = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.eqAfcDisable;
		eqAgcDisable[APLC_CH1] = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.eqAgcDisableCh1;
		eqAgcDisable[APLC_CH2] = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.eqAgcDisableCh2;
		eqAgcDisable[APLC_CH3] = g_intTDM_RxStruct0_Ptr->MasterDSPOutputFlags_TS65.eqAgcDisableCh3;
		

		// write data to internal TDM bus
		//

		// write OFDM samples to internal TDM bus

		if (OFDM_Enable) {

			vcopy_c_DD(g_intTDM_TxStruct0_Ptr->ofdm_rx, rxSignal_IF, N_SAMPLES_IF_C/2);
			vcopy_c_DD(g_intTDM_TxStruct1_Ptr->ofdm_rx, &rxSignal_IF[N_SAMPLES_IF_C/2], N_SAMPLES_IF_C/2);
		}
		else {
			
			vcopy_c_DD(g_intTDM_TxStruct0_Ptr->ofdm_rx, zeroSignal, N_SAMPLES_IF_C/2);
			vcopy_c_DD(g_intTDM_TxStruct1_Ptr->ofdm_rx, zeroSignal, N_SAMPLES_IF_C/2);
		};

		// write APLC signals (non-equalized) to internal TDM bus
		g_intTDM_TxStruct0_Ptr->aplc_rx_ne[0] = g_rxAfNeScaled[0];		// ch1
		g_intTDM_TxStruct1_Ptr->aplc_rx_ne[0] = g_rxAfNeScaled[1];		// ch1

		g_intTDM_TxStruct0_Ptr->aplc_rx_ne[1] = g_rxAfNeScaled[2];		// ch2
		g_intTDM_TxStruct1_Ptr->aplc_rx_ne[1] = g_rxAfNeScaled[3];		// ch2

		g_intTDM_TxStruct0_Ptr->aplc_rx_ne[2] = g_rxAfNeScaled[4];		// ch3
		g_intTDM_TxStruct1_Ptr->aplc_rx_ne[2] = g_rxAfNeScaled[5];		// ch3
		
		// write APLC signals (equalized) to internal TDM bus
		g_intTDM_TxStruct0_Ptr->aplc_rx_eq[0] = g_rxAfEqScaled[0];		// ch1
		g_intTDM_TxStruct1_Ptr->aplc_rx_eq[0] = g_rxAfEqScaled[1];		// ch1

		g_intTDM_TxStruct0_Ptr->aplc_rx_eq[1] = g_rxAfEqScaled[2];		// ch2
		g_intTDM_TxStruct1_Ptr->aplc_rx_eq[1] = g_rxAfEqScaled[3];		// ch2

		g_intTDM_TxStruct0_Ptr->aplc_rx_eq[2] = g_rxAfEqScaled[4];		// ch3
		g_intTDM_TxStruct1_Ptr->aplc_rx_eq[2] = g_rxAfEqScaled[5];		// ch3

		// write E-Wire data to TDM bus
		g_intTDM_TxStruct0_Ptr->ewire_rx.channel1 = EWire[APLC_CH1];	// ch1
		g_intTDM_TxStruct1_Ptr->ewire_rx.channel1 = EWire[APLC_CH1];	// ch1

		g_intTDM_TxStruct0_Ptr->ewire_rx.channel2 = EWire[APLC_CH2];	// ch2
		g_intTDM_TxStruct1_Ptr->ewire_rx.channel2 = EWire[APLC_CH2];	// ch2

		g_intTDM_TxStruct0_Ptr->ewire_rx.channel3 = EWire[APLC_CH3];	// ch3
		g_intTDM_TxStruct1_Ptr->ewire_rx.channel3 = EWire[APLC_CH3];	// ch3

		// write snr for NSD
		g_intTDM_TxStruct0_Ptr->SNR_aplc_rx[APLC_CH1] = g_intTDM_TxStruct1_Ptr->SNR_aplc_rx[APLC_CH1] = g_guardStatus[APLC_CH1].snr;
		g_intTDM_TxStruct0_Ptr->SNR_aplc_rx[APLC_CH2] = g_intTDM_TxStruct1_Ptr->SNR_aplc_rx[APLC_CH2] = g_guardStatus[APLC_CH2].snr;

		// write alarm bits
		g_intTDM_TxStruct0_Ptr->alarm = g_FE_DSP_Alarm;
		g_intTDM_TxStruct1_Ptr->alarm = g_FE_DSP_Alarm; 

		// write debug words
		g_intTDM_TxStruct0_Ptr->debug_word_0 = g_intTDM_TxStruct1_Ptr->debug_word_0 = 0xAffe1234;
		g_intTDM_TxStruct0_Ptr->debug_word_1 = g_intTDM_TxStruct1_Ptr->debug_word_1 = 0x5555AAAA;
		g_intTDM_TxStruct0_Ptr->debug_word_2 = g_intTDM_TxStruct1_Ptr->debug_word_2 = 0xFF00FF00;
		

		// TDM bus services
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[0], (unsigned long *) internalTDM_Tx_Ptr[0]);
		tdmServiceActivateNonInitiator((unsigned long *) internalTDM_Rx_Ptr[1], (unsigned long *) internalTDM_Tx_Ptr[1]);
		//asm("nop;"); asm("nop;"); asm("nop;"); asm("nop;");// 4 nop's and it runs!!!!!!!!!
		
		// EOC data transfer on internal TDM bus
		tdmEocData_transfer(&g_intTDM_RxStruct0_Ptr->EOC_TxData_Channel_1_2_3_TS06, &g_intTDM_TxStruct0_Ptr->eoc_rx);
		tdmEocData_transfer(&g_intTDM_RxStruct1_Ptr->EOC_TxData_Channel_1_2_3_TS06, &g_intTDM_TxStruct1_Ptr->eoc_rx);
		

		// Tx signal processing
		//
		
		txAfSignalProcessing();
				   
	    // SSB tx signal processing
		if (g_numOfAplcChannels == 1) {
			
			// 1 ch SSB modulation
			ssbModulator1Ch(g_txAfSum, g_txNsd, txAfSSBMod, &g_ssbDataCh1);	
		}	
		else if (g_numOfAplcChannels == 2) {
			
			// 2 ch SSB modulation
			ssbModulator2Ch(g_txAfSum, g_txNsd, txAfSSBMod);
		}
		else if (g_numOfAplcChannels == 3) {
			
			// 2 channel SSB for ch1 and ch2, 1 channel SSB for ch3
			ssbModulator2Ch(g_txAfSum, g_txNsd, txAfSSBMod);
			ssbModulator1Ch(&g_txAfSum[4], &g_txNsd[8], txAfSSBModCh3, &g_ssbDataCh3);
		}
		else {	// send no signal

			long i;

			for(i = 0; i < BLOCK_LENGTH_C_4K; i++) {
				
				txAfSSBMod[i]	 = 0.0;
				txAfSSBModCh3[i] = 0.0;
			};
		};

		if (g_operationMode != OP_MODE_NORMAL) {	// for commissioning ch3 not used

			long i;

			for(i = 0; i < BLOCK_LENGTH_C_4K; i++) {

				txAfSSBModCh3[i] = 0.0;
			};
		};
	
		
		// APLC tx signal up conversion
		//

		// interpolation M = 2
		intPolyM2(txAfSSBMod,    int2Out2Ch, &g_intPolyM2Data);
		intPolyM2(txAfSSBModCh3, int2OutCh3, &g_intPolyM2DataCh3);
		
		// shift channel 3
		freqShift(int2OutCh3, freqShiftTxOutCh3, &g_oscShiftTxCh3, N_SAMPLES_INT2_C);
		
		// add APLC signals
		vadd_f_DDD(int3In, int2Out2Ch, freqShiftTxOutCh3, BLOCK_LENGTH_C_INT2);
		
		// interpolation M = 3
		intPolyM3(int3In, int6Out);
		
		// shift APLC signal
		freqShift(int6Out, freqShiftTxOut, &g_oscShiftTxAPLC, N_SAMPLES_IF_C);
		
		// scale APLC signal for testing of internal loop
		//weightAplcSignal = 0.04;
		//vsmul_f_DD(freqShiftTxOut, weightAplcSignal, freqShiftTxOut, BLOCK_LENGTH_C_IF);
		
		// build complex APLC signal of real and imaginary part
		vcombine_c_DDD(txSignalAPLC_IF, freqShiftTxOut, &freqShiftTxOut[BLOCK_LENGTH_R_IF], N_SAMPLES_IF_C);

		// scale OFDM signal
		vsmul_f_DD((float*)txSignalOFDM_IF, g_OFDM_TxSignalCoef*((float)OFDM_Enable), (float*)txSignalOFDM_IF, BLOCK_LENGTH_C_IF);

		// lowpass filtering and frequency shift of OFDM tx signal
		txSignalProcessingOfdm_activate(txSignalOFDM_IF, txSignalLpOFDM_IF);

		// add APLC and OFDM signals
		vadd_c_DDD(txSignal_IF, txSignalAPLC_IF, txSignalLpOFDM_IF, N_SAMPLES_IF_C);
	
		// scale tx signal
		vsmul_f_DD((float*)txSignal_IF, g_txSignalCoef, (float*)txSignal_IF, BLOCK_LENGTH_C_IF);
		
		// check if channnel inverted
		if (g_rfChannelMode == RF_CHANNEL_MODE_INVERTED)
		{
			vconj_c_DD(txSignal_IF, txSignal_IF, N_SAMPLES_IF_C);
		}


		// APLC rx signal down conversion
		//
		
		// fixed point to floating point conversion
		vfloat_DD(rxSignalFloat, (int*)rxSignalFixed, -24, 24);

		ArrayFloatCheck(rxSignalFloat, 24);

		// scale input signal (affects APLC and OFDM)
		//vsmul_f_DD(rxSignalFloat, RX_IF_SCALING_COEF, rxSignalFloat, BLOCK_LENGTH_C_IF);
		
		// build complex signal
		vcombine_c_DDD(rxSignal_IF, rxSignalFloat, &rxSignalFloat[BLOCK_LENGTH_R_IF], N_SAMPLES_IF_C);
		
		// internal loop for testing
		//vcopy_c_DD(rxSignal_IF, txSignal_IF, N_SAMPLES_IF_C);

		// check channnel mode
		if ((g_rfChannelMode == RF_CHANNEL_MODE_INVERTED && !g_testSwitches.rfLoopbackOn) || 
			(g_rfChannelMode == RF_CHANNEL_MODE_ERECT && g_testSwitches.rfLoopbackOn))
		{
			vconj_c_DD(rxSignal_IF, rxSignal_IF, N_SAMPLES_IF_C);
		}
		
		// split complex rx signal into real and imaginary part
		vsplit_c_DDD(freqShiftRxIn, &freqShiftRxIn[BLOCK_LENGTH_R_IF], rxSignal_IF, N_SAMPLES_IF_C);

		// scale input signal (affects only APLC)
		vsmul_f_DD(freqShiftRxIn, RX_IF_SCALING_COEF, freqShiftRxIn, BLOCK_LENGTH_C_IF);

		// shift APLC signal
		freqShift(freqShiftRxIn, dec6In, &g_oscShiftRxAPLC, N_SAMPLES_IF_C);

		// decimation M = 3	
		decPolyM3(dec6In, dec3Out);		
		
		// shift channel 3
		freqShift(dec3Out, freqShiftRxOutCh3, &g_oscShiftRxCh3, N_SAMPLES_INT2_C);	
		
		// decimation M = 2
		decimateM2(dec3Out, rxAfSSBMod, &g_decM2Data);
		decimateM2(freqShiftRxOutCh3, rxAfSSBModCh3, &g_decM2DataCh3);
		
		// for testing
		//inPhase[0] = rxAfSSBMod[0];
		//inPhase[1] = rxAfSSBMod[2];
		//quadrature[0] = rxAfSSBMod[1];
		//quadrature[1] = rxAfSSBMod[3];
		
		//samplePilot(inPhase, 2);

		// loop SSB signal for testing
		//vcopy_DD(rxAfSSBMod, txAfSSBMod, 4);		// ch1, ch2
		//vcopy_DD(rxAfSSBModCh3, txAfSSBModCh3, 4);	// ch3
	
		
		// AF rx signal processing
		//
		
		// SSB rx signal processing
		if (g_numOfAplcChannels == 1) {
			
			ssbDemodulator1Ch(rxAfSSBMod, g_rxAfNe, &g_ssbDataCh1);
		}
		else if (g_numOfAplcChannels == 2) {
			
			ssbDemodulator2Ch(rxAfSSBMod, g_rxAfNe);
		}
		else if (g_numOfAplcChannels == 3) {
			
			ssbDemodulator2Ch(rxAfSSBMod, g_rxAfNe);
			ssbDemodulator1Ch(rxAfSSBModCh3, &g_rxAfNe[4], &g_ssbDataCh3);	
		};
		
		// pilot rx signal processing
		pilotDemodulator(&g_pilotDemod[APLC_CH1], &g_rxAfNe[0], BLOCK_LENGTH_R_4K);
		pilotDemodulator(&g_pilotDemod[APLC_CH2], &g_rxAfNe[2], BLOCK_LENGTH_R_4K);
		pilotDemodulator(&g_pilotDemod[APLC_CH3], &g_rxAfNe[4], BLOCK_LENGTH_R_4K);

		// equalizer filter
		equalizerFirFilter_activate(&g_rxAfNe[0], &rxAfEq[0], BLOCK_LENGTH_R_4K, APLC_CH1);
		equalizerFirFilter_activate(&g_rxAfNe[2], &rxAfEq[2], BLOCK_LENGTH_R_4K, APLC_CH2);
		equalizerFirFilter_activate(&g_rxAfNe[4], &rxAfEq[4], BLOCK_LENGTH_R_4K, APLC_CH3);
		
		// calculate gains of Rx AF signals
		gainRxAf[APLC_CH1] = g_agcData[APLC_CH1].rxGain*g_rxAFLevelAdjust[APLC_CH1];
		gainRxAf[APLC_CH2] = g_agcData[APLC_CH2].rxGain*g_rxAFLevelAdjust[APLC_CH2];
		gainRxAf[APLC_CH3] = g_agcData[APLC_CH3].rxGain*g_rxAFLevelAdjust[APLC_CH3];

		// scaling of non-equlized AF signals
		g_rxAfNeScaled[0] = g_rxAfNe[0]*gainRxAf[APLC_CH1];	// ch1
		g_rxAfNeScaled[1] = g_rxAfNe[1]*gainRxAf[APLC_CH1];	// ch1

		g_rxAfNeScaled[2] = g_rxAfNe[2]*gainRxAf[APLC_CH2];	// ch2
		g_rxAfNeScaled[3] = g_rxAfNe[3]*gainRxAf[APLC_CH2];	// ch2

		g_rxAfNeScaled[4] = g_rxAfNe[4]*gainRxAf[APLC_CH3];	// ch3
		g_rxAfNeScaled[5] = g_rxAfNe[5]*gainRxAf[APLC_CH3];	// ch3

		// scaling of equlized AF signals
		g_rxAfEqScaled[0] = rxAfEq[0]*gainRxAf[APLC_CH1];	// ch1
		g_rxAfEqScaled[1] = rxAfEq[1]*gainRxAf[APLC_CH1];	// ch1

		g_rxAfEqScaled[2] = rxAfEq[2]*gainRxAf[APLC_CH2];	// ch2
		g_rxAfEqScaled[3] = rxAfEq[3]*gainRxAf[APLC_CH2];	// ch2

		g_rxAfEqScaled[4] = rxAfEq[4]*gainRxAf[APLC_CH3];	// ch3
		g_rxAfEqScaled[5] = rxAfEq[5]*gainRxAf[APLC_CH3];	// ch3

		// activate sampling
		sampling_activate(&sampling);

		// UC TDM bus supervision - must be in real time
		if (LastValidDMA_Rx_BufferPtr == rxBufferPtr) IncrementDMA_ErrorCounter();
		else SetDMA_ErrorCounterZero();
		LastValidDMA_Rx_BufferPtr = rxBufferPtr;

		switch(mainCaseCounter)
		{
		 // WARNING: don´t change number of maincases
			case MAINCASE_0:
				
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_0;
				
				pilotReceiver(&rxBufferEoc[APLC_CH1], &g_pilotDemod[APLC_CH1], &g_pilotRec[APLC_CH1], &g_guardStatus[APLC_CH1]);
				pilotReceiver(&rxBufferEoc[APLC_CH2], &g_pilotDemod[APLC_CH2], &g_pilotRec[APLC_CH2], &g_guardStatus[APLC_CH2]);
				pilotReceiver(&rxBufferEoc[APLC_CH3], &g_pilotDemod[APLC_CH3], &g_pilotRec[APLC_CH3], &g_guardStatus[APLC_CH3]);

				mainCaseCounter++;
				break;
			
			case MAINCASE_1:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_1;
			
				// for testing
				//sampleData(&g_pilotDemod[APLC_CH1]);

				// Restart SPORT02 - UC TDM Bus if there is a problem
				if (RestartUC_TDM())		//returns true if the SPORT restarted - reload AD6620 registers
				{
					IR_BlockCounter = 1;
					while(IR_BlockCounter)
						{
							// What do we do if internal TDM bus is not running ???
						}
					// Now wait a little bit shorter than the next interrupt (optimally one 10.666 kHz cycle - init time AD6620)
					asm("bit clr mode2 TIMEN;\
						 TCOUNT=9020;\
						 bit set mode2 TIMEN;");
					IR_TimerCounter = 1;
					while(IR_TimerCounter)
						{
						}
					// Now initialize AD6620 - so FS on FE Bus are synchronized with FS on internal TDM bus
					initDataConverters(g_FE_DSP_Config.RF.rxFrequency, g_FE_DSP_Config.RF.channelMode);
				}
			
				EWire[APLC_CH1] = generateEWire(&g_pilotDemod[APLC_CH1]);
				EWire[APLC_CH2] = generateEWire(&g_pilotDemod[APLC_CH2]);
				EWire[APLC_CH3] = generateEWire(&g_pilotDemod[APLC_CH3]);
			
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_2: 
			{

				static long decMC2 = 0;


				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_2;
		
				if (decMC2) {
					
		  			decMC2 = !decMC2;
		  			
					demodFreqControl(&g_pilotDemod[APLC_CH1], &g_guardStatus[APLC_CH1]);	
					demodFreqControl(&g_pilotDemod[APLC_CH2], &g_guardStatus[APLC_CH2]);
					demodFreqControl(&g_pilotDemod[APLC_CH3], &g_guardStatus[APLC_CH3]);
					
					rxGuard(&g_guardStatus[APLC_CH1], &g_pilotDemod[APLC_CH1], &g_FE_DSP_Status.APLC.pilot[APLC_CH1]); 	
				}
		  		else {
		  			
		  			decMC2 = !decMC2;
		  			
			    	rxGuard(&g_guardStatus[APLC_CH2], &g_pilotDemod[APLC_CH2], &g_FE_DSP_Status.APLC.pilot[APLC_CH2]);
			    	rxGuard(&g_guardStatus[APLC_CH3], &g_pilotDemod[APLC_CH3], &g_FE_DSP_Status.APLC.pilot[APLC_CH3]);
		  		};
			
				mainCaseCounter++;                                                         
				break;
			}
			case MAINCASE_3:
			{
				static int timeCounter = 0;
				static int resetCounterTx = 0;
				static int resetCounterRx = 0;

			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_3;
				
				DoMemoryCheck(TRUE);		// TRUE = ext. SDRAM support

				// check if tx- and rx-signals are valid float values
				// tx signal
				if (nanCheck(txSignal_IF[0].re) || nanCheck(txSignal_IF[0].im)) {

					initTxSignalPath();

					resetCounterTx++;
					if (resetCounterTx > FILTER_RESET_COUNTER_MAX) {
						lifeSignEnable = FALSE;	// disable life sign -> board reset
					};
				};
				
				// rx pilot APLC ch1, ch2, ch3
				if (nanCheck(g_pilotDemod[APLC_CH1].filteredI) || 
					nanCheck(g_pilotDemod[APLC_CH1].filteredQ) ||
					nanCheck(g_pilotDemod[APLC_CH2].filteredI) || 
					nanCheck(g_pilotDemod[APLC_CH2].filteredQ) ||
					nanCheck(g_pilotDemod[APLC_CH3].filteredI) || 
					nanCheck(g_pilotDemod[APLC_CH3].filteredQ)) {

					initAPLC();

					resetCounterRx++;
					if (resetCounterRx > FILTER_RESET_COUNTER_MAX) {
						lifeSignEnable = FALSE;	// disable life sign -> board reset
					};
				};

				timeCounter++;
				if (timeCounter > TIME_COUNTER_MAX) {

					timeCounter = 0;
					resetCounterTx = 0;
					resetCounterRx = 0;
				};
		    	
				mainCaseCounter++;                                                         
				break;
			}
			case MAINCASE_4:
			
				switch(subCaseCounter4) {
					
					case SUBCASE_0: {

						static long decSC0 = 0;

					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_0;
						
						txDataProcessing(&txBufferEoc[APLC_CH1], &g_pilotMod[APLC_CH1]);
						txDataProcessing(&txBufferEoc[APLC_CH2], &g_pilotMod[APLC_CH2]);
						txDataProcessing(&txBufferEoc[APLC_CH3], &g_pilotMod[APLC_CH3]);
						
						testToneDetected[APLC_CH1] = detectTestTone(&g_testTone[APLC_CH1]);
						testToneDetected[APLC_CH2] = detectTestTone(&g_testTone[APLC_CH2]);
						testToneDetected[APLC_CH3] = detectTestTone(&g_testTone[APLC_CH3]);
				
						g_FE_DSP_Status.APLC.rxTestToneLevel[APLC_CH1] = g_testTone[APLC_CH1].rxLevel;
						g_FE_DSP_Status.APLC.rxTestToneLevel[APLC_CH2] = g_testTone[APLC_CH2].rxLevel;
						g_FE_DSP_Status.APLC.rxTestToneLevel[APLC_CH3] = g_testTone[APLC_CH3].rxLevel;
						
						if (decSC0) {
					
			  				decSC0 = !decSC0;
							
							pilotAmpAdjust(&g_pilotMod[APLC_CH1].osc);
							pilotAmpAdjust(&g_pilotMod[APLC_CH2].osc);
							pilotAmpAdjust(&g_pilotMod[APLC_CH3].osc);
							
							pilotAmpAdjust(&g_pilotDemod[APLC_CH1].osc);
							pilotAmpAdjust(&g_pilotDemod[APLC_CH2].osc);
							pilotAmpAdjust(&g_pilotDemod[APLC_CH3].osc);
						}
						else {
							
							decSC0 = !decSC0;
							
							pilotBbAmpAdjust(&g_pilotDemod[APLC_CH1]);
							pilotBbAmpAdjust(&g_pilotDemod[APLC_CH2]);
							pilotBbAmpAdjust(&g_pilotDemod[APLC_CH3]);
							
							testToneAmpAdjust(&g_testTone[APLC_CH1].osc);
							testToneAmpAdjust(&g_testTone[APLC_CH2].osc);
							testToneAmpAdjust(&g_testTone[APLC_CH3].osc);
						};
					
						subCaseCounter4++;
						break;
					}	
					case SUBCASE_1:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_1;
					
						agcBlocking = g_testSwitches.agcDisable[APLC_CH1] || eqAgcDisable[APLC_CH1];
						gainControl(&g_guardStatus[APLC_CH1], &g_agcData[APLC_CH1], agcBlocking);
						
						agcBlocking = g_testSwitches.agcDisable[APLC_CH2] || eqAgcDisable[APLC_CH2];
						gainControl(&g_guardStatus[APLC_CH2], &g_agcData[APLC_CH2], agcBlocking);

						agcBlocking = g_testSwitches.agcDisable[APLC_CH3] || eqAgcDisable[APLC_CH3];
						gainControl(&g_guardStatus[APLC_CH3], &g_agcData[APLC_CH3], agcBlocking);
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_2:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_2;
						// Supervision UC bus
						CheckUpconverterBusOnFE_DSP(rxBufferPtr);
						
						pilotReceiverController(&g_pilotDemod[APLC_CH1], &g_pilotRec[APLC_CH1], &g_guardStatus[APLC_CH1]);
						pilotReceiverController(&g_pilotDemod[APLC_CH2], &g_pilotRec[APLC_CH2], &g_guardStatus[APLC_CH2]);
						pilotReceiverController(&g_pilotDemod[APLC_CH3], &g_pilotRec[APLC_CH3], &g_guardStatus[APLC_CH3]);
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_3:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_3;
					
						evaluateGuard(&g_guardStatus[APLC_CH1], &g_guradConfig[APLC_CH1]);
					
						subCaseCounter4++;
						break;
						
					case SUBCASE_4:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_4;
					
						evaluateGuard(&g_guardStatus[APLC_CH2], &g_guradConfig[APLC_CH2]);
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_5:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_5;
					
						evaluateGuard(&g_guardStatus[APLC_CH3], &g_guradConfig[APLC_CH3]);
						
						subCaseCounter4++;
						break;
						
					case SUBCASE_6:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_6;
						
						if (g_niReceiveFlag)
						{
							g_niReceiveFlag = FALSE;

							switch (g_niRxMsgType)
							{	
								e_TDMServiceResult tdmServiceResult;

								case e_TDMServiceMsgType_IdleCounter_Req:

									MEMCPY(g_niTxData, idleCounter_read(), sizeof(t_IdleCounters));
									idleCounter_reset(); // reset idle counters

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(t_IdleCounters),
    															          e_TDMServiceMsgType_IdleCounter_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);

									break;

								case e_TDMServiceMsgType_SamplingCfg_Send: {

									static float *samplingPtr;
									static t_SamplingCfg samplingCfg;

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(NULL, 0, // zero length reply message
    																	  e_TDMServiceMsgType_Ack); // txMsgType = acknowledge

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);

									MEMCPY(&samplingCfg, g_niRxData, sizeof(samplingCfg)); // copy received message

									samplingPtr = getSamplingPtr(samplingCfg.signalPath, samplingCfg.signalSource);
									sampling_enable(samplingPtr, &sampling);
									break;
								}
								case e_TDMServiceMsgType_SignalBuffer_Req: {

									static float *readPtr;
									static t_SamplingBufRdParam samplingBufRdParam;

									MEMCPY(&samplingBufRdParam, g_niRxData, sizeof(samplingBufRdParam)); // copy received message
									
									// check index of last sample
									if ((samplingBufRdParam.startIndex + samplingBufRdParam.nSamples) <= SAMPLING_SIGNAL_BUF_SIZE) {

										readPtr = sampling_getBufferPtr() + samplingBufRdParam.startIndex;
										tdmServiceResult = tdmServiceNI_Reply((unsigned long *)readPtr, 
																			  samplingBufRdParam.nSamples,
    																		  e_TDMServiceMsgType_FE_SignalBuffer_Resp);	// txMsgType
									}
									else {	// send NACK

										tdmServiceResult = tdmServiceNI_Reply(NULL, 0,					 // zero length reply message
    															              e_TDMServiceMsgType_Nack); // txMsgType
									};

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);
									break;
								}
								case e_TDMServiceMsgType_StatusData_Req:

									MEMCPY(g_niTxData, &g_FE_DSP_Status, sizeof(g_FE_DSP_Status));
									
									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply(g_niTxData, 
																          sizeof(g_FE_DSP_Status),
    															          e_TDMServiceMsgType_FE_StatusData_Resp); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								case e_TDMServiceMsgType_GetTestPlcLinkCfg_Req: 

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply((unsigned long *)&g_testPlcLinkConfig, 
																          sizeof(g_testPlcLinkConfig),
    															          e_TDMServiceMsgType_GetTestPlcLinkCfg_Res); //txMsgType

									ASSERT(e_TDMServiceResult_Ok == tdmServiceResult);

									break;
								
								case e_TDMServiceMsgType_SetTestPlcLinkCfg_Send: {

									long i;
									long loopbackFreq;
									static BOOL loopbackOn = FALSE;

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    															           e_TDMServiceMsgType_Ack); // txMsgType = acknowledge

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									MEMCPY(&g_testPlcLinkConfig, g_niRxData, sizeof(g_testPlcLinkConfig));

									testPlcLink_setTestSwitches(g_testPlcLinkConfig.testSwitches, &g_testSwitches);
									
									for (i = 0; i < N_APLC_CHANNELS; i++) {
					
										g_ttLevel[i] = g_testPlcLinkConfig.testTone[i].levelLin;
										testToneOscillatorInit(g_testPlcLinkConfig.testTone[i].frequency,
															   AF_SAMPLE_FREQ_4K, 
															   &g_testTone[i].osc);
									}

									if (g_testSwitches.rfLoopbackOn && !loopbackOn)
									{
										loopbackOn = TRUE;
										g_txSignalCoef = g_txSignalCoefLoopback;
										
										loopbackFreq = calcRfLoopbackRxFreq(&g_FE_DSP_InitialConfig);
										tdmSyncAD6620();
										AD6620_Init_Test(loopbackFreq); 
										AD6620_Start();
									}
									else if (!g_testSwitches.rfLoopbackOn && loopbackOn)
									{
										loopbackOn = FALSE;
										g_txSignalCoef = g_txSignalCoefNormalOperation;

										tdmSyncAD6620();
										initDataConverters(g_FE_DSP_Config.RF.rxFrequency, 
														   g_FE_DSP_Config.RF.channelMode);
									}

									break;
								}
								case e_TDMServiceMsgType_OperationMode_Send:
								{
									e_TDMServiceMsgType	msgType	= e_TDMServiceMsgType_Ack;

									switch(g_niRxData[0]) {

										case OP_MODE_NORMAL: 
											
											initAPLC();
											 
											tdmSyncAD6620();
											initDataConverters(g_FE_DSP_Config.RF.rxFrequency, 
															   g_FE_DSP_Config.RF.channelMode);
											break;

										case OP_MODE_COMMISSIONING:
										case OP_MODE_COMMISSIONING_RF_TERMINATED:

											initCommissioningMode(g_niRxData[0]);
											break;

										default: 
											msgType	= e_TDMServiceMsgType_Nack; // unknown operation mode

									};

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,  // zero length reply message
    																	   msgType); 

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;
								}

								case e_TDMServiceMsgType_CommissioningCfg_Send: {

									static t_Commissioning commissioningCfg;
									static float rxTestToneLevel;

									MEMCPY(&commissioningCfg, g_niRxData, sizeof(commissioningCfg));

									if (commissioningCfg.command == COMM_CMD_GET_RX_TESTTONE_LEVEL) {
										
										rxTestToneLevel = g_testTone[APLC_CH1].rxLevel;

										// send TDM service reply
										tdmServiceResult = tdmServiceNI_Reply((unsigned long *)&rxTestToneLevel, 
																sizeof(rxTestToneLevel),
    															e_TDMServiceMsgType_CommissioningGetRxTestToneLevel_Resp);
									}
									else {

										BOOL bSuccess = initCommissioningCfg(&commissioningCfg);

										// send TDM service reply
										tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
																			   bSuccess ?                // txMsgType 
																			   e_TDMServiceMsgType_Ack:
										                                       e_TDMServiceMsgType_Nack); 

									};

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

									break;
								}

								case e_TDMServiceMsgType_Cfg_Send:	// this case should not occur (cfg distribution retry)

									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    															           e_TDMServiceMsgType_Ack); // txMsgType = acknowledge

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);
									break;

								default:
									// send TDM service reply
									tdmServiceResult = tdmServiceNI_Reply (NULL, 0,                  // zero length reply message
    															           e_TDMServiceMsgType_Nack);// txMsgType = no acknowledge

									ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

							}
						}

						subCaseCounter4++;
						break;
			
					case SUBCASE_7:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_7;

						// automatic frequency control functions
						afcEnable = g_afcEnable && !eqAfcDisable;
						afc_monitor(&g_afc, &g_guardStatus[APLC_CH1], &g_FE_DSP_Status.APLC, afcEnable);
						afc_activate(&g_afc, &g_guardStatus[APLC_CH1], &g_pilotDemod[APLC_CH1], &g_oscShiftRxAPLC);

						subCaseCounter4++;
						break;
						
					case SUBCASE_8: {

						static long counterSC8 = APLC_CH1;

					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_8;

						switch(counterSC8) {

							case APLC_CH1:

								updatePilotStatus(&g_guradConfig[APLC_CH1], &g_guardStatus[APLC_CH1],
												  &g_pilotMod[APLC_CH1], &g_pilotDemod[APLC_CH1],
												  &g_agcData[APLC_CH1], &g_FE_DSP_Status.APLC.pilot[APLC_CH1]);
								counterSC8++;
								break;

							case APLC_CH2:

								updatePilotStatus(&g_guradConfig[APLC_CH2], &g_guardStatus[APLC_CH2],
												  &g_pilotMod[APLC_CH2], &g_pilotDemod[APLC_CH2],
												  &g_agcData[APLC_CH2], &g_FE_DSP_Status.APLC.pilot[APLC_CH2]);
								counterSC8++;
								break;

							case APLC_CH3:

								updatePilotStatus(&g_guradConfig[APLC_CH3], &g_guardStatus[APLC_CH3], 
												  &g_pilotMod[APLC_CH3], &g_pilotDemod[APLC_CH3],
												  &g_agcData[APLC_CH3], &g_FE_DSP_Status.APLC.pilot[APLC_CH3]);
								counterSC8 = APLC_CH1;
								break;

							default: ASSERT(FALSE);
						};
					
						subCaseCounter4++;
						break;
					}
					case SUBCASE_9:
					{
						static BOOL bFlag = FALSE;
						static unsigned long myLifesign;


						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_9;

						if (!bFlag) // reset the idle counter the very first time
						{
							idleCounter_reset();
							bFlag = TRUE;
						}

						// update APLC alarm bits
						FE_DSP_Alarm_updateAplcAlarms(&g_FE_DSP_Alarm, &g_FE_DSP_Status, 
													  g_operationMode, g_numOfAplcChannels);
												
						if (lifeSignEnable == TRUE) {
							myLifesign++;
						};
						g_FE_DSP_Alarm.lifesign = myLifesign;
						g_FE_DSP_Alarm.progMemCRCErr = memoryCheckDSP_CRC_Alarm();
						g_FE_DSP_Alarm.rxRfOverflow = FE_DSP_Alarm_rxRfOverflow(); 
						
						subCaseCounter4 = 0;
						break;
					}
					default:
						subCaseCounter4 = 0;
						break;
				}; // end switch(subCaseCounter4)
					
				mainCaseCounter = 0;
                break;
                
		 	default:
				mainCaseCounter = 0;
				break;
		}; // end switch(mainCaseCounter)
		
    }

}


/*****************************************************************************
  Function:		txAfSignalProcessing()	
  Description:	Generates pilot, test tone and multipexes all APLC AF signals.
				
  Inputs:		None
  Outputs:		None
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

void txAfSignalProcessing(void)
{
	static long i;

	// sample arrays
	static float txPilot[3*BLOCK_LENGTH_R_4K];		// ch1, ch2, ch3
	static float txTestTone[3*BLOCK_LENGTH_R_4K];	// ch1, ch2, ch3
	static float txAfSum[3*BLOCK_LENGTH_R_4K];		// ch1, ch2, ch3
	static float txPn[BLOCK_LENGTH_R_4K];			// used for 1 ch

	// noise generator shift registers
	static long shiftRegPnGen[N_APLC_CHANNELS] = {START_VALUE_SHIFT_REGISTER,
												  START_VALUE_SHIFT_REGISTER,
												  START_VALUE_SHIFT_REGISTER};
	// noise generator enable
	static BOOL pnGenOn;

	// test tone enables
	static BOOL testToneEnable[N_APLC_CHANNELS];

	switch(g_operationMode) {

		case OP_MODE_NORMAL:

			g_pilotMod[APLC_CH1].rts = g_pilotEnableTPE[APLC_CH1] && !g_testSwitches.pilotDisable[APLC_CH1];
			g_pilotMod[APLC_CH2].rts = g_pilotEnableTPE[APLC_CH2] && !g_testSwitches.pilotDisable[APLC_CH2];
			g_pilotMod[APLC_CH3].rts = g_pilotEnableTPE[APLC_CH3] && !g_testSwitches.pilotDisable[APLC_CH3];

			// pilot tx signal generation
			pilotTransmitter(&g_pilotMod[APLC_CH1], &txPilot[0], BLOCK_LENGTH_R_4K); 
			pilotTransmitter(&g_pilotMod[APLC_CH2], &txPilot[2], BLOCK_LENGTH_R_4K);
			pilotTransmitter(&g_pilotMod[APLC_CH3], &txPilot[4], BLOCK_LENGTH_R_4K); 

			// test tone processing
			ttone(&g_testTone[APLC_CH1], &g_rxAfEqScaled[0], &txTestTone[0], BLOCK_LENGTH_R_4K); 
			ttone(&g_testTone[APLC_CH2], &g_rxAfEqScaled[2], &txTestTone[2], BLOCK_LENGTH_R_4K);
			ttone(&g_testTone[APLC_CH3], &g_rxAfEqScaled[4], &txTestTone[4], BLOCK_LENGTH_R_4K); 
			
			// test tone can only be activated if no TPS command is transmitted
			testToneEnable[APLC_CH1] = g_testSwitches.testToneEnable[APLC_CH1] && !g_disconnect;
			testToneEnable[APLC_CH2] = g_testSwitches.testToneEnable[APLC_CH2] && !g_disconnect;
			testToneEnable[APLC_CH3] = g_testSwitches.testToneEnable[APLC_CH3] && !g_disconnect;

			txTestTone[0] *= g_ttLevel[APLC_CH1]*((float)testToneEnable[APLC_CH1]);
			txTestTone[1] *= g_ttLevel[APLC_CH1]*((float)testToneEnable[APLC_CH1]);

			txTestTone[2] *= g_ttLevel[APLC_CH2]*((float)testToneEnable[APLC_CH2]);
			txTestTone[3] *= g_ttLevel[APLC_CH2]*((float)testToneEnable[APLC_CH2]);

			txTestTone[4] *= g_ttLevel[APLC_CH3]*((float)testToneEnable[APLC_CH3]);
			txTestTone[5] *= g_ttLevel[APLC_CH3]*((float)testToneEnable[APLC_CH3]);

			// AF signal multiplexer
			txAfSignalMux(g_txAf, g_txNsk, txTestTone, txAfSum);

			// check if noise generator has to be activated
			pnGenOn = g_pnGeneratorEnable[APLC_CH1] && !g_inputCmdActive;

			if (pnGenOn) {

				// call noise generator
				PN_GeneratorSample(&shiftRegPnGen[APLC_CH1], &txPn[0]);

				txAfSum[0] = txPn[0]*g_pnLevel;
				txAfSum[1] = txPn[1]*g_pnLevel;
				g_txNsk_noAP[0]=0;
				g_txNsk_noAP[1]=0;
			};

			pnGenOn = g_pnGeneratorEnable[APLC_CH2] && !g_inputCmdActive;

			if (pnGenOn) {

				// call noise generator
				PN_GeneratorSample(&shiftRegPnGen[APLC_CH2], &txPn[0]);

				txAfSum[2] = txPn[0]*g_pnLevel;
				txAfSum[3] = txPn[1]*g_pnLevel;
				g_txNsk_noAP[2]=0;
				g_txNsk_noAP[3]=0;
			};

			pnGenOn = g_pnGeneratorEnable[APLC_CH3] && !g_inputCmdActive;

			if (pnGenOn) {

				// call noise generator
				PN_GeneratorSample(&shiftRegPnGen[APLC_CH3], &txPn[0]);

				txAfSum[4] = txPn[0]*g_pnLevel;
				txAfSum[5] = txPn[1]*g_pnLevel;
				g_txNsk_noAP[4]=0;
				g_txNsk_noAP[5]=0;
			}

			// ap filters
			apFilter_activate(&txAfSum[0], &txAfSum[0], BLOCK_LENGTH_R_4K, APLC_CH1);
			apFilter_activate(&txAfSum[2], &txAfSum[2], BLOCK_LENGTH_R_4K, APLC_CH2);
			apFilter_activate(&txAfSum[4], &txAfSum[4], BLOCK_LENGTH_R_4K, APLC_CH3);

			// add pilot signal (after ap filters to reduce delay for teleprotection)
			for (i = 0; i < N_APLC_CHANNELS*BLOCK_LENGTH_R_4K; i++) {

				g_txAfSum[i] = txAfSum[i] + txPilot[i] + g_txNsk_noAP[i];
			};

			break;

		case OP_MODE_COMMISSIONING:
		case OP_MODE_COMMISSIONING_RF_TERMINATED:

			// test tone processing
			g_rxAfNe[0] *= COMM_TT_RX_SCALING_COEF;
			g_rxAfNe[1] *= COMM_TT_RX_SCALING_COEF;

			ttone(&g_testTone[APLC_CH1], &g_rxAfNe[0], &txTestTone[0], BLOCK_LENGTH_R_4K); 

			g_txAfSum[0] = g_ttLevel[APLC_CH1]*txTestTone[0];
			g_txAfSum[1] = g_ttLevel[APLC_CH1]*txTestTone[1];

			// reset NSD samples
			for(i = 0; i < BLOCK_LENGTH_C_4K; i++) {

				g_txNsd[i] = 0.0;
			};

			break;

		default:

			// send no signal
			for (i = 0; i < N_APLC_CHANNELS*BLOCK_LENGTH_R_4K; i++) {

				g_txAfSum[i] = 0.0;
			};
	}

}


/*****************************************************************************
  Function:		getSamplingPtr()	
  Description:	Based on the inputs signalPath and signalSource the pointer to
				the corresponding signal array is returned.
  Inputs:		signalPath, signalSource
  Outputs:		none
  Return:		Pointer to signal array
  Globals:		
  Important:	-

******************************************************************************/

static float SEGMENT_EXT_CODE *getSamplingPtr(long signalPath, long signalSource)
{

	switch(signalPath) {
	
		case SAMPLING_SIGNAL_PATH_APLC_1_TX:		// tx AF signal ch 1

			switch(signalSource) {

				case 0:	return &g_txAfSum[0];		// internal and external signals
				case 1:	return &g_txAf[0];			// external signals (from O4LEs)
				default: &g_txAfSum[0];
			}
			break;

		case SAMPLING_SIGNAL_PATH_APLC_2_TX:		// tx AF signal ch 2

			switch(signalSource) {

				case 0:	return &g_txAfSum[2];		// internal and external signals	
				case 1: return &g_txAf[2];			// external signals (from O4LEs)	
				default: &g_txAfSum[2];
			}
			break;

		case SAMPLING_SIGNAL_PATH_APLC_1_RX:		// rx AF signal ch 1

			switch(signalSource) {
			
				case 0:	return &g_rxAfNeScaled[0];	// non equlized signals
				case 1:	return &g_rxAfEqScaled[0];	// equalized signals
				default: &g_rxAfNeScaled[0];
			}
			break;

		case SAMPLING_SIGNAL_PATH_APLC_2_RX:		// rx AF signal ch 2

			switch(signalSource) {

				case 0:	return &g_rxAfNeScaled[2];	// non equlized signals
				case 1:	return &g_rxAfEqScaled[2];	// equalized signals
				default: &g_rxAfNeScaled[2];
			}
			break;

		case SAMPLING_SIGNAL_PATH_APLC_3_TX:		// tx AF signal ch 3

			switch(signalSource) {
			
				case 0:	return &g_txAfSum[4];		// internal and external signals
				case 1:	return &g_txAf[4];			// external signals (from O4LEs)
				default: &g_txAfSum[4];
			}
			break;

		case SAMPLING_SIGNAL_PATH_APLC_3_RX:		// rx AF signal ch 3

			switch(signalSource) {

				case 0:	return &g_rxAfNeScaled[4];	// non equlized signals
				case 1:	return &g_rxAfEqScaled[4];	// equalized signals
				default: &g_rxAfNeScaled[4];
			}
			break;

		default: &g_txAfSum[0];
	}

	return &g_txAfSum[0];	// this line should not be reached

}


/*****************************************************************************
  Function:		initCommissioningMode()	
  Description:	Initializes the commissioning mode.
  Inputs:		
  Outputs:		
  Return:		
  Globals:		
  Important:	

******************************************************************************/

static void initCommissioningMode(long operationMode) 
{
	t_FrontEndDSP_Config tempCfg;

	
	// set operation mode
	g_operationMode = operationMode;

	// disable AFC
	g_afcEnable = FALSE;

	// initialize 1 ch SSB without AP filters
	g_numOfAplcChannels = 1;
	tempCfg.apFilterFlags = 0;
	tempCfg.channelMode = APLC_CHANNEL_MODE_A1_40I;
	initSSB1Ch_Ch1(&tempCfg, &g_ssbDataCh1);

	// initialize frequency up/down shift
	initFreqShiftAPLC(&g_oscShiftTxAPLC, &g_oscShiftRxAPLC, tempCfg.channelMode);

	// initialize RF channnel mode
	g_rfChannelMode = RF_CHANNEL_MODE_ERECT;

	// calculate tx signal scaling
	if (g_operationMode == OP_MODE_COMMISSIONING_RF_TERMINATED)
	{
		// reduced output power
		g_txSignalCoef = S_VAL_ADJ_COEF*g_txPeakAmpCoef;
		g_txSignalCoef *= g_powerReductionCoefLoopback*g_FE_DSP_Config.RF.txLevelAdjustment;

	}
	else 
	{
		g_txSignalCoef  = S_VAL_ADJ_COEF*g_txPeakAmpCoef;
		g_txSignalCoef *= g_powerReductionCoef*g_FE_DSP_Config.RF.txLevelAdjustment;
	}
		
	// initialize test tone 
	g_ttLevel[APLC_CH1] = 0.0;
	testToneOscillatorInit(COMM_TT_FREQ_DEFAULT, AF_SAMPLE_FREQ_4K, &g_testTone[APLC_CH1].osc);

}


/*****************************************************************************
  Function:		initCommissioningCfg()	
  Description:	Initializes the commissioning configuration.
  Inputs:		Pointer to commissioning configuration
  Outputs:		
  Return:		
  Globals:		
  Important:	

******************************************************************************/

static BOOL initCommissioningCfg(t_Commissioning *commissioningCfg)
{
	BOOL bSuccess = TRUE;

	switch(commissioningCfg->command) {

		case COMM_CMD_ADJ_TX_RF_LEVEL:

			// set test tone level
			g_ttLevel[APLC_CH1] = commissioningCfg->txTestToneLevel;

			// calculate tx signal scaling
			g_txSignalCoef = S_VAL_ADJ_COEF*g_txPeakAmpCoef;
			g_txSignalCoef *= g_powerReductionCoef*commissioningCfg->txRfLevelAdjustment;

			break;

		case COMM_CMD_ACT_TX_ALARM_LEVEL:
			
			if (g_operationMode == OP_MODE_COMMISSIONING_RF_TERMINATED)	 // ETL600 R4
			{
				g_ttLevel[APLC_CH1] = g_ttLevelTxAlarmAdj;
			}
			else // ETL600 R3
			{	
				// set test tone level 
				g_ttLevel[APLC_CH1] = commissioningCfg->txTestToneLevel;
			}

			break;

		case COMM_CMD_ADJ_RX_RF_LEVEL:

			// set test tone level 
			g_ttLevel[APLC_CH1] = commissioningCfg->txTestToneLevel;

			// set rx frequency
			tdmSyncAD6620();
			initDataConverters(commissioningCfg->rxRfFrequency, RF_CHANNEL_MODE_ERECT);

			break;

		case COMM_CMD_TUNE_TX_FILTER:

			// tx filter tuning ETL600 R4 (currently not used by HMI)
			if (g_operationMode == OP_MODE_COMMISSIONING_RF_TERMINATED)
			{
				// set test tone level
				g_ttLevel[APLC_CH1] = COMM_TT_LEVEL_PEP;

				// set rx frequency for filter check
				tdmSyncAD6620();
				initDataConverters(commissioningCfg->rxRfFrequency, RF_CHANNEL_MODE_ERECT);
			}
			else // ETL600 R3
			{
				// set test tone level
				g_ttLevel[APLC_CH1] = COMM_TT_LEVEL_TUNE_FILTER;
			}

			break;

		case COMM_CMD_TUNE_RX_FILTER:

			// set test tone level 
			g_ttLevel[APLC_CH1] = COMM_TT_LEVEL_TUNE_FILTER;

			break;

		case COMM_CMD_TUNE_HYBRID:

			// set test tone level 
			g_ttLevel[APLC_CH1] = COMM_TT_LEVEL_TUNE_HYBRID;

			// set rx frequency
			tdmSyncAD6620();
			initDataConverters(commissioningCfg->rxRfFrequency, RF_CHANNEL_MODE_ERECT);

			break;

		case COMM_CMD_DEACTIVATE_TESTTONE:

			// reset test tone level
			g_ttLevel[APLC_CH1] = 0.0;

			break;

		case COMM_CMD_SAVE_TX_RF_LEVEL_ADJ:

			g_FE_DSP_Config.RF.txLevelAdjustment = commissioningCfg->txRfLevelAdjustment;

			break;


		default: bSuccess = FALSE; // invalid command (could stem from TDM bus error)
	}

	return bSuccess;
}


/*****************************************************************************
  Function:		calcRfLoopbackRxFreq()	
  Description:	Calculates the Rx RF frequency used for local RF loopback.
  Inputs:		
  Outputs:		
  Return:		
  Globals:		
  Important:	

******************************************************************************/
static long SEGMENT_EXT_CODE calcRfLoopbackRxFreq(t_FrontEndDSP_Config *config)
{
	long nominalBW;
	long aplcBW;
	long rxCenterFreq;
	t_APLC_Config aplcConfig;
	

	aplcConfig.channelMode = config->channelMode;
	GET_APLC_BANDWIDTH(aplcConfig, aplcBW);

	nominalBW = aplcBW + config->DPLC_BW;

	if (config->RF.channelMode == RF_CHANNEL_MODE_ERECT)
	{
		rxCenterFreq = config->RF.rxFrequency + nominalBW - 0.5*RF_BANDWIDTH;
	}
	else
	{
		rxCenterFreq = config->RF.rxFrequency - nominalBW + 0.5*RF_BANDWIDTH;
	}

	return rxCenterFreq;
}


/* TEST FUNCTIONS ************************************************************/

/*static void SEGMENT_EXT_CODE setFE_DSP_Config(t_FrontEndDSP_Config *config)
{
	 
	config->pilot[APLC_CH1].frequency = 3840.0; //3120.0; 
	config->pilot[APLC_CH2].frequency = 3840.0; //2280.0;
	config->pilot[APLC_CH3].frequency = 3840.0; //2280.0;
	
	config->testTone[APLC_CH1].frequency = 450.0;
	config->testTone[APLC_CH2].frequency = 1150.0;
	config->testTone[APLC_CH3].frequency = 1950.0;
	
	config->channelMode = APLC_CHANNEL_MODE_A1_40I_A2_40E_A3_40E; //APLC_CHANNEL_MODE_NONE; //APLC_CHANNEL_MODE_A1_40I; //APLC_CHANNEL_MODE_A1_40I; //APLC_CHANNEL_MODE_A1_40I_A2_40E; //
}*/


/*void getRxSamples(long vRx[], volatile long *rxBufferPtr, long offset, long nTimeSlots, long blockLength)
{
	long i;

	for (i = 0; i < blockLength; i++) {
	
		vRx[i] = (*(rxBufferPtr + offset)) >> 10;
		rxBufferPtr += nTimeSlots;
	};
} */


/*void sampleRxSignal_I(volatile long *sampleIn, long offset, long nTimeSlots, long blockLength)
{

#define RX_I_BUFFER_SIZE	1024

	long i;
	static long sampleCount;
	static long rxSignal_I[RX_I_BUFFER_SIZE];
	static long rxBufferI[12];
	

	for (i = 0; i < blockLength; i++) {
	
		rxBufferI[i] = (*(sampleIn + offset));
		rxSignal_I[sampleCount + i] = (*(sampleIn + offset)) >> 10;
		sampleIn += nTimeSlots;
	};
	
	sampleCount += blockLength;
	if (sampleCount > (RX_I_BUFFER_SIZE - blockLength)) sampleCount = 0;

}; */


/*
void sampleRxSignal_Q(volatile long *sampleIn, long offset, long nTimeSlots, long blockLength)
{

#define RX_Q_BUFFER_SIZE	1024

	long i;
	static int sampleCount;
	static long rxSignal_Q[RX_Q_BUFFER_SIZE];
	

	for (i = 0; i < blockLength; i++) {
	
		rxSignal_Q[sampleCount + i] = (*(sampleIn + offset)) >> 10;
		sampleIn += nTimeSlots;
	};
	
	sampleCount += blockLength;
	if (sampleCount > (RX_Q_BUFFER_SIZE - blockLength)) sampleCount = 0;

};
*/

