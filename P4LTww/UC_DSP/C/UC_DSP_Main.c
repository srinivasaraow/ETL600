/*-------------------------------------------------------------------------*
 * $Workfile: UC_DSP_Main.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Up-Converter DSP main loop
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/UC_DSP/C/UC_DSP_Main.c $ 
 * 
 * 33    15-01-14 17:55 Chhamaa
 * Init value (frequency) for receiver of up-converter settings modified.
 * 
 * 27    4.04.06 18:34 Chmibuh
 * Added UC TDM bus DMA supervision. Restarts tested.
 * Added functions from SupervisionUC_Bus_UC_DSP.c
 * CheckUpconverterBusOnUC_DSP and RestartUC_TDM tested.
 * more reliable access of internal TDM bits
 * (MasterDSPOutputFlags_TS65.requestSPIbusAllowed) 
 * No signal processing if DAC is in reset state -> saving of calculation
 * time
 * 
 * 26    23.03.06 14:40 Chmibuh
 * clean up restartDAC()
 * 
 * 25    17.03.06 10:56 Chmibuh
 * SUBCASE_1 changed: Changed conditions for DAC reset
 * 
 * 23    4.10.05 7:58 Chchlee
 * SEGMENT_EXT_CODE added to getConfiguration()
 * 
 * 22    7.09.05 15:44 Chhamaa
 * dacInitFailure alarm added
 * 
 * 21    19.08.05 16:50 Chhamaa
 * segment ("seg_dmData") replaced with SEGMENT_DM_DATA2
 * 
 * 20    19.07.05 19:38 Chmibuh
 * g_UC_DSP_Alarm added for progMemCRCErr
 * 
 * 19    24.06.05 11:24 Chmibuh
 * re-enable memoryCheck 60 cycles should be available
 * 
 * 18    22.06.05 15:24 Chhamaa
 * While loop added (to wait for next interrupt) after receiving new RF
 * frequency in SUBCASE_7
 * 
 * 17    20.06.05 15:25 Chhamaa
 * static added to definition of unsigned long DAC_DMA_ErrorCounter
 * 
 * 16    17.06.05 15:47 Chhamaa
 * GET_CFG_SEND_ACK_CYCLES added
 * 
 * 15    16.06.05 15:53 Chhamaa
 * DAC_DMA_ERROR_COUNTER_MAX added
 * 
 * 14    16.06.05 15:20 Chbrbus
 * DMA DAC supervision alpha release (!!!!!!!!!!!!!!) included
 * 
 * 13    1.06.05 16:24 Chbrbus
 * Reset pulse width for glue logic DMA11 and DAC increased (factor 50).
 * OFDM modem is now starting up more stable. Memory Check temporary
 * disabled. In this combination no more OFDM startup anomalies.
 * 
 * 12    19.05.05 11:18 Chmibuh
 * memoryCheckRespondToMaster();  added
 * dummy memoryCheck removed
 * 
 * 11    12.05.05 18:30 Chmibuh
 * CRC check added
 * 
 * 10    11.05.05 18:49 Chmibuh
 * DoMemoryCheck(FALSE) removed temporay
 * 
 * 9     9.05.05 16:53 Chbrbus
 * MemoryCheck function added - memory check itself has still to be
 * implemented
 * 
 * 8     10.03.05 15:06 Chpeplu
 * Mystery delay START_UP_UC_DSP_WAIT_TIME for init AD9777 added, and
 * check TempRxDMA_BufferPtr != 0x0000 added -> UC_DSP works now with new
 * config
 * 
 * 7     16.02.05 16:51 Chhamaa
 * 
 * 6     14.02.05 16:34 Chbrbus
 * TDM buffer pointers all time valid in DMA buffer range
 * 
 * 5     11.02.05 14:44 Chbrbus
 * Added DMA buffer boundary check and SIMD mode off during interrupt
 * 
 * 4     10.02.05 16:09 Chbrbus
 * Reduced upconverter TDM bus DMA buffer - changed interrupt routine
 * 
 * 3     8.02.05 14:45 Chbrbus
 * UC DAC interrupt syncronized with SPORT13 interrupt - reduced DAC
 * buffers from 3 to 2 ! Cleaned Interrupt functions and EP1
 * initialisation.
 * 
 * 2     3.02.05 17:26 Chbrbus
 * Synchronized DAC interrupt with internal TDM bus interrupt -
 * supervision and reload of DAC not yet done
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 53    4.01.05 14:06 Chhamaa
 * Floating point check for input TDM bus data added (ArrayFloatCheck())
 * 
 * 52    30.11.04 16:51 Chhamaa
 * FLOAT_TO_FIX_SCALING_EXP set to 15
 * 
 * 51    17.11.04 20:55 Leeb02
 * g_niReceiveFlag bug fixed
 * 
 * 50    15.11.04 9:27 Leeb02
 * Function call in ASSERT() removed
 * 
 * 49    7.10.04 15:43 Maag01
 * 
 * 48    5.10.04 10:42 Maag01
 * 
 * 47    4.10.04 17:48 Maag01
 * 
 * 46    30.09.04 17:16 Maag01
 * 
 * 45    24.09.04 14:23 Leeb02
 * 
 * 44    9.09.04 16:56 Leeb02
 * 
 * 43    3.09.04 15:17 Rytz01
 * an other ominous leebsches memcpy replaced!!!
 * 
 * 42    1.09.04 14:25 Buhl01
 * reestablishSPI() added
 * 
 * 41    31.08.04 10:52 Maag01
 * 
 * 40    27.08.04 8:20 Rytz01
 * debug words now with pointer to struct (like lifesign) 
 * 
 * 39    26.08.04 16:48 Rytz01
 * lifesign now with struct and debug words sending
 * 
 * 38    25.08.04 11:36 Maag01
 * 
 * 37    24.08.04 18:55 Maag01
 * 
 * 36    24.08.04 18:51 Maag01
 * 
 * 35    18.08.04 18:39 Maag01
 * 
 * 34    18.08.04 14:19 Rytz01
 * var rxBufferPtr removed
 * 
 * 33    13.08.04 17:17 Rytz01
 * Garbage removed -->Check which buffer is valid and set the pointer to
 * the beginning --> now with lookup table
 * 
 * 32    12.08.04 18:40 Maag01
 * 
 * 31    11.08.04 18:23 Maag01
 * 
 * 30    11.08.04 16:59 Maag01
 * 
 * 29    26.07.04 14:23 Maag01
 * 
 * 28    19.07.04 17:33 Maag01
 * 
 * 27    6.07.04 13:45 Maag01
 * Interpolation filter for first stage replaced
 * 
 * 26    2.07.04 11:34 Bussli02
 * 
 * 25    29.06.04 19:29 Maag01
 * 
 * 24    24.06.04 14:05 Maag01
 * 
 * 23    22.06.04 15:07 Maag01
 * 
 * 22    22.06.04 14:04 Rytz01
 * 
 * 21    22.06.04 11:16 Maag01
 * 
 * 20    21.06.04 16:59 Maag01
 * 
 * 19    18.06.04 18:24 Maag01
 * 
 * 18    18.06.04 17:18 Rytz01
 * 
 * 17    18.06.04 15:47 Rytz01
 * 
 * 16    17.06.04 17:31 Rytz01
 * 
 * 15    7.06.04 19:21 Maag01
 * 
 * 14    4.06.04 16:34 Rytz01
 * 
 * 13    3.06.04 12:11 Maag01
 * 
 * 12    2.06.04 18:58 Maag01
 * 
 * 11    1.06.04 18:06 Maag01
 * 
 * 10    27.05.04 14:37 Maag01
 * 
 * 9     26.05.04 18:30 Maag01
 * 
 * 8     18.05.04 18:19 Maag01
 * 
 * 7     12.05.04 18:31 Maag01
 * 
 * 6     11.05.04 18:28 Maag01
 * 
 * 5     11.05.04 17:33 Maag01
 * 
 * 4     7.05.04 17:12 Maag01
 * 
 * 3     6.05.04 18:26 Maag01
 * 
 * 2     5.05.04 18:29 Maag01
 * 
 * 1     4.05.04 16:58 Maag01
 * Initial version
 * 
 *-------------------------------------------------------------------------*/


// for testing 
//#define GENERATE_TEST_SIGNAL
//#define DUAL_TONE			
#define TEST_SCALE_FACTOR		1.0
#define TT_AMPLITUDE1			1.0
#define TT_AMPLITUDE2			1.0
#define TT_FREQUENCY1			8000.0		// [Hz]
#define TT_FREQUENCY2		   	-8000.0		// [Hz]
#define TX_CARRIER_FREQUENCY    200000.0	// [Hz]


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
asm("#include <def21161.h>");

#ifndef _WIN32
	#include <def21161.h>
#endif

#include "basetype.h"
#include "speeddsp.h"

#include "constant.h"
#include "UC_DSP_Types.h"
#include "UC_DSP_Const.h"
#include "UC_DSP_Main.h"
#include "Timer.h"
#include "idleCounter.h"
#include "interpolation.h"
#include "intFiltCoef.h"
#include "oscillator.h"

#include "AD9777_dout.h"
#include "AD9777_utils.h"
#include "AD9777_Init.h"

#include "DIAG.H"
#include "tdm_int_types.h"
#include "tdm_service.h"
#include "tdmUpConvSettings.h"
#include "upDownConvTdmBus.h"

#include "MemoryCheck.h"
#include "SupervisionUC_Bus_UC_DSP.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define GET_CFG_SEND_ACK_CYCLES		20

#define INT64_SCALE_FACTOR			(64.0*0.25)
#define RF_OSC_AMPLITUDE			1.0
#define FLOAT_TO_FIX_SCALING_EXP	15

// START_UP_UC_DSP_WAIT_TIME values between 20 and 2500 will work with actual FW from VSS-archiv (2005_03_10)
// START_UP_UC_DSP_WAIT_TIME = 3000 will not work with the actual FW from VSS-archiv (2005-03-10) 
// START_UP_UC_DSP_WAIT_TIME = 10 will not work with the actual FW from VSS-archiv (2005-03-10)
// config: 4 G4AI, 1 O4LE, MOD600 = off, NSK = off, 2 APLC Channels, 2 TPS active testet with emulator
#define	START_UP_UC_DSP_WAIT_TIME	1000

#define DAC_DMA_ERROR_COUNTER_MAX	20		// DAC DMA supervision


/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx and tx pointers
#define g_intTDM_RxStruct_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr)
#define g_intTDM_TxStruct_Ptr  ((t_UC_output_tdm_block*)internalTDM_Tx_Ptr)


/* LOCAL VARIABLES ***********************************************************/

// Defined in Interrupt.asm as global
extern volatile long IR_BlockCounter;

// Internal TDM buffer pointers
extern volatile long *internalTDM_Rx_Ptr;
extern volatile long *internalTDM_Tx_Ptr;
extern volatile long *externalTDM_Rx_Ptr;
extern volatile long *externalTDM_Tx_Ptr;
extern volatile long *DAC_Tx_Ptr;
extern long SPORT02_RxBufferLookupTable[];

//UC TDM bus DMA supervision - low processing load required - not good code practice style!!
volatile unsigned long UC_Bus_DMA_ErrorCounter = 0;

static t_UpConverterDSP_Config	g_UC_DSP_Config;
static float g_scaleTxSignalIF1_In;

// TDM service variables
static unsigned long        g_niRxData[100]; // receive buffer
static BOOL                 g_niReceiveFlag;
static e_TDMServiceMsgType	g_niRxMsgType;
static unsigned long	    g_niRxMsgSize;

// Delay lines for interpolation filters
static complex_float g_interp4Delay1[L_INT_FILT1/4+1];
static complex_float g_interp4Delay2[L_INT_FILT2/4+1];
static complex_float g_interp4Delay3[L_INT_FILT3/4+1];

// RF oscillator data
static t_OscData g_rfOscData;

// Buffer for RF DAC Tx samples
static int SEGMENT_DMA g_DAC_TxBuffer1[N_SAMPLES_RF];
static int SEGMENT_DMA g_DAC_TxBuffer2[N_SAMPLES_RF];

// For interrupt routine, prevents writing in non DMA buffer adresses
const long *EP1_FirstDMA_BufferAdress = (long*) &g_DAC_TxBuffer1[0];
const long *EP1_LastDMA_BufferAdress = (long*) &g_DAC_TxBuffer2[0];

static t_UC_Alarm g_UC_DSP_Alarm = {0,0,0,0};

// Test signal generation
t_OscData testOsc1Data;
t_OscData testOsc2Data;

complex_float SEGMENT_DM_DATA2 testOsc1Out[N_SAMPLES_IF1];
complex_float SEGMENT_DM_DATA2 testOsc2Out[N_SAMPLES_IF1];


/* LOCAL FUNCTIONS ***********************************************************/

static void niReceive(e_TDMServiceMsgType rxMsgType, unsigned long rxMsgSize);
static void niConfirm(void);
static void getConfiguration(t_UpConverterDSP_Config *config);
static void configUC_DSP(t_UpConverterDSP_Config *config);
static void setRfFrequency(long frequency, long channelMode);


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		configUC_DSP()	
  Description:	Configures UC DSP functions
  Inputs:		config: pointer to configuration data structure 
  Outputs:		-
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void configUC_DSP(t_UpConverterDSP_Config *config)
{

#ifdef GENERATE_TEST_SIGNAL

	// initialize scaling factor
	g_scaleTxSignalIF1_In = INT64_SCALE_FACTOR*TEST_SCALE_FACTOR;
	
	// set RF parameters
	config->RF.channelMode = RF_CHANNEL_MODE_ERECT;
	config->RF.txFrequency = TX_CARRIER_FREQUENCY;	// [Hz]

	// initialize test tone 1
	oscillator_init(TT_AMPLITUDE1, TT_FREQUENCY1, 64000.0, &testOsc1Data);

#ifdef DUAL_TONE

	// initialize test tone 2
	oscillator_init(TT_AMPLITUDE2, TT_FREQUENCY2, 64000.0, &testOsc2Data);

#endif

#else

	// initialize scaling factor
	g_scaleTxSignalIF1_In = INT64_SCALE_FACTOR;

#endif

	// initialize receiver for up-converter settings (used in commissioning mode)
	tdmUpConvSettings_initReceiver(config->RF.txFrequency, config->RF.channelMode);

	// initialize interpolation filters
	Interp4_init_filt_c(g_interp4Delay1, g_interp4Delay2, g_interp4Delay3);

	setRfFrequency(config->RF.txFrequency, config->RF.channelMode);

}


/*****************************************************************************
  Function:		restartDAC(BOOL isSPIbus_free)	
  Description:	Once this state-machine is called it handles the SPI bus mastership, 
				resets the DAC and sets the registers of the DAC. Finally it calls 
				reestablishSPI();
  Inputs:		isSPIbus_free: = g_intTDM_RxStruct_Ptr->MasterDSPOutputFlags_TS65.requestSPIbusAllowed
  Outputs:		-
  Return:		FALSE if restart of DAC is finished, else TRUE
  Globals:		
  Important:	called by SUBCASE_1 1.067 KHz (0.9375 ms)

******************************************************************************/

static BOOL restartDAC(BOOL isSPIbus_free)
{
	static unsigned long waitfor;
	unsigned long returnVal;
	// state fields
	static enum 
	{
		restartDAC_start,
		restartDAC_waitForSPI, 
		restartDAC_resetDAC,
		restartDAC_initRegisters,
		restartDAC_resetDMA,
		restartDAC_releaseSPI,
		restartDAC_final
	} state = restartDAC_start;

	returnVal = TRUE;

		switch (state)
		{   
			case restartDAC_start:
				g_UC_DSP_Alarm.requestSPIbus = TRUE; // send request to get SPI Bus Mastership from Master DSP
				state = restartDAC_waitForSPI;
				waitfor = 0;
				break;
			case restartDAC_waitForSPI:
				waitfor++;
				if(isSPIbus_free)
				{	//SPI bus is free
					state = restartDAC_resetDAC;
					
					*(long*)(IOFLAG) |= FLG6;		// Reset DAC - do not use "AD9777_Init_HW_Reset" here
													// because this will generate a short reset pulse. 
													// But this short pulse will not work here.
				}
				else //SPI bus is busy
				{
					state = restartDAC_waitForSPI;
					if(waitfor > 100) // after 1 (0.9375) seconds forget it!
					{
						g_UC_DSP_Alarm.requestSPIbus = FALSE;
						state = restartDAC_start;
						returnVal = FALSE;
					}
				}
				break;
			case restartDAC_resetDAC:
				AD9777_Init_HW_Reset();				// Release DAC reset; reset is already done in case restartDAC_waitForSPI				
				
				// Init DAC registers
				g_UC_DSP_Alarm.dacInitFailure = AD9777_Init_ModeControlReg(g_UC_DSP_Config.rfPepMax);
				state = restartDAC_initRegisters;
				break;
			case restartDAC_initRegisters:
				// do nothing, add an other 9.375ms pause. Do not remove it!!!
				asm("nop;");
				state = restartDAC_resetDMA;
				break;
			case restartDAC_resetDMA:				
				IR_BlockCounter = 1;
				while(IR_BlockCounter)				// wait for next interrupt of SPORT0
				{
					
				}

				IR_BlockCounter = 1;

				// initialize DMA buffers for RF DAC
				AD9777_Init_Data_Port(g_DAC_TxBuffer1, g_DAC_TxBuffer2, N_SAMPLES_RF);
				AD9777_Start_Data_Port();

				state = restartDAC_releaseSPI;			
				break;
			case restartDAC_releaseSPI:
				reestablishSPI();
				state = restartDAC_final;
				break;
			case restartDAC_final:
				g_UC_DSP_Alarm.requestSPIbus = FALSE;	// inform Master DSP that the DAC reset is finished
				state = restartDAC_start;
				returnVal = FALSE;
				break;
			default:
				g_UC_DSP_Alarm.requestSPIbus = FALSE;
				state = restartDAC_start;
				returnVal = FALSE;
				break;
		}

		return(returnVal);
}




/*****************************************************************************
  Function:		setRfFrequency()	
  Description:	Calculates the RF center frequency and initializes the RF
				oscillater.
  Inputs:		frequency and channelMode
  Outputs:		-
  Return:		-
  Globals:		
  Important:	-

******************************************************************************/

static void setRfFrequency(long frequency, long channelMode)
{
	long txCenterFreq;


	// calculate tx center frequency
	if (channelMode == RF_CHANNEL_MODE_ERECT) {

		txCenterFreq = frequency + 0.5*RF_BANDWIDTH;
	}
	else {
		
		txCenterFreq = frequency - 0.5*RF_BANDWIDTH;
	};
	
	// initialize RF oscillator
	oscillator_init(RF_OSC_AMPLITUDE, txCenterFreq, (float)RF_SAMPLE_FREQ, &g_rfOscData);
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

static void SEGMENT_EXT_CODE getConfiguration(t_UpConverterDSP_Config *config)
{
	long i;
	e_TDMServiceResult tdmServiceResult;


	tdmServiceNI_Init(e_TDMServiceAddr_UC,  // initialize non-initiator TDM service
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
	ASSERT(sizeof(t_UpConverterDSP_Config) == g_niRxMsgSize); // check if size is correct

	// send TDM service reply
	tdmServiceResult = tdmServiceNI_Reply (NULL, 0, // zero length reply message
    						               e_TDMServiceMsgType_Ack); // txMsgType = acknowledge,

	ASSERT( e_TDMServiceResult_Ok == tdmServiceResult);

	MEMCPY (config, g_niRxData, sizeof(*config)); // copy received mesage into g_UC_DSP_Config

	// tdmServiceActivateNonInitiator() is called to send ack
	for(i = 0; i < GET_CFG_SEND_ACK_CYCLES; i++)
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


/* MAIN **********************************************************************/


void main (void)
{
	static long i;
	static unsigned long mainCaseCounter;
	static unsigned long subCaseCounter;
	
	// idle counters
	static unsigned long startValue;
	static unsigned long idleCounter;
	static unsigned long idleCounterMinIndex;

	static BOOL newSettingsReceived;

	static long *TempRxDMA_BufferPtr;

	// for correct alignment the arrays in the seg_dmData must have even length
	static complex_float SEGMENT_DM_DATA2 txSignalIF1_In[N_SAMPLES_IF1];
	static complex_float SEGMENT_DM_DATA2 txSignalIF2[N_SAMPLES_IF2];
	static complex_float SEGMENT_DM_DATA2 txSignalIF3[N_SAMPLES_IF3];
	static complex_float SEGMENT_DM_DATA2 txSignalRF[N_SAMPLES_RF];
	
	static float SEGMENT_DM_DATA2 inPhase[N_SAMPLES_RF];
	static float SEGMENT_DM_DATA2 quad[N_SAMPLES_RF];
	static float SEGMENT_DM_DATA2 vRe[N_SAMPLES_RF];
	static float SEGMENT_DM_DATA2 vIm[N_SAMPLES_RF];
	static float SEGMENT_DM_DATA2 temp[N_SAMPLES_RF];
	static float SEGMENT_DM_DATA2 txSignalReal[N_SAMPLES_RF];
	static unsigned int SEGMENT_DM_DATA2 txSignalOut[N_SAMPLES_RF];

	// DAC DMA supervision
	static volatile long *OldDAC_DMA_Ptr;
	static unsigned long DAC_DMA_ErrorCounter = 0;
	static long *OldUC_Bus_Rx_DMA_Ptr;
	static BOOL isDACinProcess = FALSE;
	static BOOL isSPIbus_free = FALSE;	

	Init_DSP_Register();
	asm("nop;");

	// init SPORT DMA pointers - acessing the DMA pointer is possible anytime - no more: if (internalTDM_Tx_Ptr) ....
	Init_InternalTDM_BufferPtr((unsigned long*) internalTDM_Rx_Ptr, (unsigned long*) internalTDM_Tx_Ptr);
	Init_ExternalTDM_BufferPtr((unsigned long*) externalTDM_Rx_Ptr, (unsigned long*) externalTDM_Tx_Ptr);
	DAC_Tx_Ptr = (long*) &g_DAC_TxBuffer1[0];

	MemoryCheckStartupSHARC(FALSE);		// FALSE = no ext. SDRAM

	memoryCheckRespondToMaster();

	initialize_DSP();
	RestartTimer10k();
	IR_TimerCounter = 1;

	// initialize DSP configuration
	getConfiguration(&g_UC_DSP_Config);

	configUC_DSP(&g_UC_DSP_Config);

	for (i=0; i<START_UP_UC_DSP_WAIT_TIME; i++)
	{
		asm("nop;");
		asm("nop;");
		asm("nop;");
	}

	// perform HW reset of AD9777
	AD9777_Init_HW_Reset();

	// initialize mode control registers of RF DAC
	g_UC_DSP_Alarm.dacInitFailure = AD9777_Init_ModeControlReg(g_UC_DSP_Config.rfPepMax);

	// workaround for DSP anomaly 47 
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Do not add any SPI access to AD9777 after this line !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	reestablishSPI();

	// initialize DMA buffers for RF DAC
	//AD9777_Init_Data_Port(g_DAC_TxBuffer1, g_DAC_TxBuffer2, g_DAC_TxBuffer3, N_SAMPLES_RF);

	// WARNING: DO NOT CHANGE ANYTHING WITHOUT KNOWING WHAT YOU ARE DOING !!!!!!!!!!!!!!!!!!!!!!!!!!!
	// This is important to syncronize FS on internal TDM Bus with DAC Interrupt - must be independent from 
	// running time of previous init functions
	// Important to save transmission time and TDM buffer size between TDM busses and DAC -  T0 time NSD

	// Wait for next Interrupt internal TDM Bus
	// SPORT13 Interrupt must be enabled or else it will hang indefinately here

	IR_BlockCounter = 1;
	while(IR_BlockCounter)
		{
			// What do we do if internal TDM bus is not running ???
		}

	IR_BlockCounter = 1;


	// NOTE: DAC interrupt routine is just finished 1.1 us after FS on SPORT13 occurs - independent from the
	// use of an emulator
	// END syncronisation of interrupts
	
	// initialize DMA buffers for RF DAC
	AD9777_Init_Data_Port(g_DAC_TxBuffer1, g_DAC_TxBuffer2, N_SAMPLES_RF);
	AD9777_Start_Data_Port();
	
	asm("bit set mode1 IRPTEN;");
	
    // main loop 
    while (1) {

		static unsigned long myLifesign;


		startValue = count_start();

		// repetiton rate = 10.667 kHz
		while(IR_TimerCounter && IR_BlockCounter )
		{
			// wait until Timer is ringing or all Blocks has received.
			// IR_BlockCounter is decremented by Interrupt routine of SPORT0;
		}

		idleCounter = count_end(startValue);
		idleCounter_update(idleCounterMinIndex, idleCounter);

		RestartTimer10k();
		IR_BlockCounter = 1;
		IR_TimerCounter = 1;

		// receive up-converter settings from Maser DSP on internal TDM bus
		newSettingsReceived = tdmUpConvSettings_receive(&g_intTDM_RxStruct_Ptr->UpConvSettings_TS68);
		
		// write alarm bits to internal TDM bus
		g_intTDM_TxStruct_Ptr->alarm = g_UC_DSP_Alarm;

		// Generate Debug Words and write them to the internal TDM bus!!!
		//g_intTDM_TxStruct_Ptr->debug_word_0 = 0xAffe1234;
		//g_intTDM_TxStruct_Ptr->debug_word_1 = 0x5555AAAA;
		//g_intTDM_TxStruct_Ptr->debug_word_2 = 0xFF00FF00;


#ifdef GENERATE_TEST_SIGNAL

		// test oscillator 1
		oscillator_oscC(testOsc1Out, &testOsc1Data, N_SAMPLES_IF1);
		oscillator_AGC(&testOsc1Data);
		
#ifdef DUAL_TONE
		
		// test oscillator 2
		oscillator_oscC(testOsc2Out, &testOsc2Data, N_SAMPLES_IF1);
		oscillator_AGC(&testOsc2Data);
		
#endif

		// add test signals
		vadd_c_DDD(txSignalIF1_In, testOsc1Out, testOsc2Out, N_SAMPLES_IF1);

#else

		// get samples from FE DSP
		// !!! CAUTION !!! !!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!
		// see WARNING in upDownConvTdmBus.c
		// NEVER EVER CHANGE the following 2 lines without understanding the concept
		TempRxDMA_BufferPtr = (long*) externalTDM_Rx_Ptr;
		if(TempRxDMA_BufferPtr != 0x0000000)
		{
 			upDownConvTdmBus_UC_read(txSignalIF1_In, TempRxDMA_BufferPtr);
		}

		ArrayFloatCheck((float*)txSignalIF1_In, 12);

#endif
		if(!(isDACinProcess))	// No DAC reset; save calculation time while DAC is in reset state
		{
			// scale signal
			vsmul_f_DD((float*)txSignalIF1_In, g_scaleTxSignalIF1_In, (float*)txSignalIF1_In, 2*N_SAMPLES_IF1);
 			
			// signal interpolation        	
			Int4x44_filt_c(txSignalIF2, txSignalIF1_In, N_SAMPLES_IF1, interp4FiltCoeff1, g_interp4Delay1);
			Interp4_filt_c_L16(txSignalIF3, txSignalIF2, N_SAMPLES_IF2, interp4FiltCoeff2, g_interp4Delay2);
			Interp4_filt_c_L16(txSignalRF, txSignalIF3, N_SAMPLES_IF3, interp4FiltCoeff3, g_interp4Delay3);

			// get real and imaginary part of the interpolated signal
    		vsplit_c_DDD(inPhase, quad, txSignalRF, N_SAMPLES_RF);
    		
    		// RF oscillator
    		oscillator_oscF(vRe, vIm, &g_rfOscData, N_SAMPLES_RF);
    		oscillator_AGC(&g_rfOscData);
    		
    		// quadrature modulation:
    		// txSignalReal = inPhase*cos - quad*sin;
			vmul_f_DDD(temp, quad, vIm, N_SAMPLES_RF);
			vmulsub_f_DDDD(txSignalReal, inPhase, vRe, temp, N_SAMPLES_RF);
			

			// conversion to integer 
			vfix_DD((int*) DAC_Tx_Ptr, txSignalReal, FLOAT_TO_FIX_SCALING_EXP, N_SAMPLES_RF);
		}

		isSPIbus_free = g_intTDM_RxStruct_Ptr->MasterDSPOutputFlags_TS65.requestSPIbusAllowed;		

		// DAC DMA supervision
		if (DAC_Tx_Ptr == OldDAC_DMA_Ptr) DAC_DMA_ErrorCounter++;
		else DAC_DMA_ErrorCounter = 0;
		OldDAC_DMA_Ptr = DAC_Tx_Ptr;

		// UC bus DMA supervision
		if (TempRxDMA_BufferPtr == OldUC_Bus_Rx_DMA_Ptr) UC_Bus_DMA_ErrorCounter++;
		else UC_Bus_DMA_ErrorCounter = 0;
		OldUC_Bus_Rx_DMA_Ptr = TempRxDMA_BufferPtr;

		switch(mainCaseCounter)
		{
			// WARNING: don´t change number of cases
			// repetiton rate of case functions = 10.667 kHz

			case MAINCASE_0:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_0;
				
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_1:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_1;
			
				// BB: I had to disable the MemoryCheck because it interfered with the Tx signal.
				// I still don't now if it is a problem of lack of idle cycles or interference witt DMA.
				// There must be a few samples affected.

				// BUH: previously there were no idle cycles left if MemoryCheck was enabled. Now, about 60 
				// cycles should be available with changed MemoryCheck!
				
				DoMemoryCheck(FALSE);		// FALSE means there is no SDRAM to check
			
				mainCaseCounter++;                                                         
				break;

			case MAINCASE_2:

				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_2;
				// Supervision UC TDM bus
				CheckUpconverterBusOnUC_DSP(TempRxDMA_BufferPtr);					

				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_3:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_3;			
				// restart UC TDM bus if in trouble
				RestartUC_TDM();				
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_4:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_4;
				
				
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_5:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_5;
				
				
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_6:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_6;
				
				
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_7:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_7;
				
				
				
				mainCaseCounter++;                                                         
				break;
				
			case MAINCASE_8:
			
				idleCounterMinIndex = IDLE_COUNTER_MIN_MAINCASE_8;
				
				
				
				mainCaseCounter++;                                                         
				break;
			
			case MAINCASE_9:

				switch(subCaseCounter) {

					// WARNING: don´t change number of cases
					// repetiton rate of case functions = 1.067 kHz
					
					case SUBCASE_0:	
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_0;
						
						if(memoryCheckDSP_CRC_Alarm())
						{
							g_UC_DSP_Alarm.progMemCRCErr = TRUE;
						}
						else
						{
							g_UC_DSP_Alarm.progMemCRCErr = FALSE;
						}
					
						subCaseCounter++;
						break;
						
					case SUBCASE_1:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_1;
					
						// Restart DAC DMA 
						if ((DAC_DMA_ErrorCounter > DAC_DMA_ERROR_COUNTER_MAX) || (isDACinProcess))	
						{
							isDACinProcess = restartDAC(isSPIbus_free);
							DAC_DMA_ErrorCounter = 0;
						}
			
						subCaseCounter++;
						break;
						
					case SUBCASE_2:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_2;
					
						
					
						subCaseCounter++;
						break;
						
					case SUBCASE_3:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_3;
						
					
						subCaseCounter++;
						break;
						
					case SUBCASE_4:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_4;
						
						
						
						subCaseCounter++;
						break;
						
					case SUBCASE_5:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_5;
						
						
						
						subCaseCounter++;
						break;
						
					case SUBCASE_6:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_6;
						
						

						subCaseCounter++;
						break;
			
					case SUBCASE_7:
						
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_7;
						
						if (newSettingsReceived) {

							static t_UpConvSettings *pNewSettings;

							pNewSettings = tdmUpConvSettings_getNewSettingsPtr();
							setRfFrequency(pNewSettings->frequency, pNewSettings->channelMode);
							tdmUpConvSettings_initReceiver(pNewSettings->frequency, pNewSettings->channelMode);

							// wait for next interrupt
							IR_BlockCounter = 1;
							while(IR_BlockCounter)
								{
									// What do we do if internal TDM bus is not running ???
								}

							IR_BlockCounter = 1;
						};
						
						subCaseCounter++;
						break;
						
					case SUBCASE_8:
					
						idleCounterMinIndex = IDLE_COUNTER_MIN_SUBCASE_8;
					
						myLifesign++;
						if (myLifesign == 8) myLifesign = 0;

						g_UC_DSP_Alarm.lifesign = myLifesign;
					
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
		
    }

}



/* TEST FUNCTIONS ************************************************************/

/*
void setUC_DSP_Config(t_UpConverterDSP_Config *config)
{
	config->RF.channelMode = RF_CHANNEL_MODE_ERECT; // RF_CHANNEL_MODE_INVERTED
	config->RF.txFrequency = TX_CARRIER_FREQUENCY;	// [Hz]
	config->RF.txLevelAdjustment = 1.0;

}; */


/*
void sampleSignalIF(float *sampleIn, int blockLength)
{

#define BUFFER_IF_SIZE	1024

	int i;
	static int sampleCount;
	static float signalIF[BUFFER_IF_SIZE];
	

	for (i = 0; i < blockLength; i++) {
	
		signalIF[sampleCount + i] = *sampleIn;
		sampleIn++;
	};
	
	sampleCount += blockLength;
	if (sampleCount > (BUFFER_IF_SIZE - blockLength)) sampleCount = 0;

};
*/


// test function HPR
/*
void sampleRxSignal_from_SPORT02(volatile long *sampleIn, long offset, long nTimeSlots, long blockLength)
{

#define RX_Q_BUFFER_SIZE	1024

	long i;
	static int sampleCount;
	static long rxSignal_from_SPORT02[RX_Q_BUFFER_SIZE];
	

	for (i = 0; i < blockLength; i++) {
	
		rxSignal_from_SPORT02[sampleCount + i] = (*(sampleIn + offset));
		sampleIn += nTimeSlots;
	};
	
	sampleCount += blockLength;
	if (sampleCount > (RX_Q_BUFFER_SIZE - blockLength)) sampleCount = 0;

};
*/

