/*-------------------------------------------------------------------------*
 * $Workfile: Equalizer.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Equalizer functions
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/C/Equalizer.c $ 
 * 
 * 3     27.05.05 10:15 Chchlee
 * UART_NOT_OPERATIONAL and Limiter active alarm removed
 * 
 * 2     16.02.05 13:49 Chhamaa
 * 
 * 18    4.02.05 16:18 Chhamaa
 * 
 * 17    4.02.05 9:53 Chhamaa
 * 
 * 16    2.02.05 17:21 Chhamaa
 * 
 * 15    1.02.05 18:19 Chhamaa
 * 
 * 14    1.02.05 10:46 Chhamaa
 * 
 * 13    27.01.05 19:18 Chhamaa
 * 
 * 12    24.01.05 17:04 Chhamaa
 * 
 * 11    21.01.05 19:21 Chhamaa
 * 
 * 10    20.01.05 18:27 Chhamaa
 * 
 * 9     18.01.05 18:55 Chhamaa
 * 
 * 8     14.01.05 19:21 Chhamaa
 * 
 * 7     13.01.05 18:45 Chhamaa
 * 
 * 6     13.01.05 17:05 Chhamaa
 * 
 * 5     13.01.05 12:58 Chhamaa
 * deviceState[] changed  to  deviceState
 * 
 * 4     11.01.05 18:16 Chhamaa
 * 
 * 3     11.01.05 17:38 Chhamaa
 * 
 * 2     10.01.05 16:58 Chhamaa
 * 
 * 1     10.01.05 11:13 Chhamaa
 * Initial version basd on ETL500 version 4.20
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "tdm_int_types.h"
#include "diag.h"
#include "P4LT_Types.h"
#include "Configuration.h"
#include "Status.h"
#include "Event_Defs.h"
#include "Monitor.h"
#include "Equalizer.h"
#include "EQ_CALCU.H"


/* LOCAL CONSTANT DECLARATIONS ************************************************/

// States channel measurement (t_ChMeasurementStatus.stateAccumulation)
#define CHANNEL_ACCUMULATION_IDLE			0
#define CHANNEL_ACCUMULATION_CLEAR_ARRAY	1
#define CHANNEL_ACCUMULATION_MEASURE		2
#define CHANNEL_ACCUMULATION_ERROR			3

// States control accumulation (t_ChMeasurementStatus.stateAccControl)
#define MEASUREMENT_IDLE					0
#define MEASUREMENT_PN_WAITING				1
#define MEASUREMENT_RUNNING					2
#define MEASUREMENT_END						3

// States pn generation (t_PnControllerStatus.statePn)
#define PN_IDLE								0
#define PN_RUNNING							1
#define PN_RUNNING_HOT						2
#define PN_END								3 

// Timeout for state machine state timeout (#sec * 106.6 + 10)
#define TIMER_VALUE							9604	// Timeout 1.5 Min 3 telegramm error
#define EQUALIZER_OFFSET_REMOTE_COMMAND		4
#define TIMER_ZERO_VALUE					10
#define MEASUREMENT_NOT_VALID				0.0

#define HW_ALARMS_APLC		(ALM_Master_Life_Sign | ALM_FE_Life_Sign | ALM_UC_Life_Sign|\
							 ALM_Master_Programm_Memory_CRC | ALM_FE_Programm_Memory_CRC |\
							 ALM_UC_Programm_Memory_CRC | ALM_M12V_Underv | ALM_M12V_Overv |\
							 ALM_P12V_Underv | ALM_P12V_Overv | ALM_P5V_Underv | ALM_P5V_Overv |\
							 ALM_P3_3V_Underv | ALM_P3_3V_Overv | ALM_P1_8V_Underv |\
							 ALM_P1_8V_Overv | ALM_License |\
							 ALM_Plut_out_3_RF_Hybrid)

#define HW_ALARMS_TX		(ALM_Tx_RF)					  
#define HW_ALARMS_RX 		(ALM_RF_Rx_overflow | ALM_Plut_out_4_Rx_Filter)
#define HW_ALARMS_EQ_TX		(HW_ALARMS_APLC | HW_ALARMS_TX)
#define HW_ALARMS_EQ_RX		(HW_ALARMS_APLC | HW_ALARMS_RX)

#define LINK_ALARMS_EQ_CH1	(ALM_Slow_Mute_Ch1 | ALM_Rx_pilot_fail_Ch1 | ALM_Pilot_level_Ch1 |\
							 ALM_AGC_fail_Ch1 | ALM_AFC_fail)

#define LINK_ALARMS_EQ_CH2	(ALM_Slow_Mute_Ch2 | ALM_Rx_pilot_fail_Ch2 | ALM_Pilot_level_Ch2 |\
							 ALM_AGC_fail_Ch2 | ALM_AFC_fail)

#define LINK_ALARMS_EQ_CH3	(ALM_Slow_Mute_Ch3 | ALM_Rx_pilot_fail_Ch3 | ALM_Pilot_level_Ch3 |\
							 ALM_AGC_fail_Ch3 | ALM_AFC_fail)

#define RX_PILOT_ALARMS_EQ_CH1	(ALM_Slow_Mute_Ch1 | ALM_Rx_pilot_fail_Ch1 | ALM_Pilot_level_Ch1)
#define RX_PILOT_ALARMS_EQ_CH2	(ALM_Slow_Mute_Ch2 | ALM_Rx_pilot_fail_Ch2 | ALM_Pilot_level_Ch2)
#define RX_PILOT_ALARMS_EQ_CH3	(ALM_Slow_Mute_Ch3 | ALM_Rx_pilot_fail_Ch3 | ALM_Pilot_level_Ch3)	


/* LOCAL MACRO DEFINITIONS ***************************************************/

// cast internal TDM bus rx and tx pointers
#define g_intTDM_RxStruct_Ptr  ((t_AllDSP_input_tdm_block*)internalTDM_Rx_Ptr)
#define g_intTDM_TxStruct_Ptr  ((t_Master_output_tdm_block*)internalTDM_Tx_Ptr)


/* LOCAL TYPE DECLARATIONS ****************************************************/

// Pseudo noise controller status struct

typedef struct {

	long statePn;
	long timer;

} t_PnControllerStatus;	


// Definition of status variables used for channel measurement

typedef struct {

	// variables used in equalizer_measureChannel()
	long stateAccumulation;
	long indexAccumulation;
	long numberOfAccumulations;
	long clearAccumulationArrayIndex;
	long pnAccumulationRunning;

	// variables used in equalizer_stateMachineAccController()
	long stateAccControl;
	long timerAccStateMachine;

} t_ChMeasurementStatus;


typedef struct {

	BOOL afcDisable;
	BOOL agcDisable[N_APLC_CHANNELS];

} t_OutputFlags;


/* LOCAL VARIABLES ************************************************************/

extern volatile long *internalTDM_Tx_Ptr;
extern volatile long *internalTDM_Rx_Ptr;

static t_EqualizerStatus SEGMENT_EXT_DATA g_equalizerStatusNew[N_APLC_CHANNELS];
static t_EqualizerFirCoef SEGMENT_EXT_DATA g_equalizerFirCoef[N_APLC_CHANNELS];

static t_PnControllerStatus g_pnControllerStatus[N_APLC_CHANNELS];
static t_ChMeasurementStatus g_chMeasurementStatus[N_APLC_CHANNELS];

static t_OutputFlags g_outputFlags;

static BOOL g_inputCmdActive;

static t_AlarmsAndWarnings *g_pAlarms;
static t_P4LT_Status *g_pP4LT_Status;
static t_FastStatusTelegram *g_pFastStatusRemote;
static t_EqualizerStatus *g_pEqualizerStatusActive;
static t_P4LT_Config *g_pP4LT_Config;


/* LOCAL FUNCTIONS ************************************************************/

static void initPtrs();
static void initPNController(long channel);
static void initMeasureChannel(long channel);
static void setDeviceState(long channel);
static void calculateCoefs(long channel);
static void setFirCoefs(float *eqCoef, long balancePoint, long eqLength, 
						t_EqualizerFirCoef *eqFirCoef);


/* IMPLEMENTATION *************************************************************/

/******************************************************************************
  Function:		equalizer_init()			
  Description:	Initializes equalizer data structures.
  Inputs:		Channel number
  Outputs:		
  Return:		
  Globals:		
  Important:	Call before using equalizer functions

*******************************************************************************/

void SEGMENT_EXT_CODE equalizer_init(long channel)
{

	if (channel == APLC_CH1) {

		initPtrs();	// pointers have to be initialized only once for all equalizers
	};		

	// check if channel valid
	if ((channel >= APLC_CH1) && (channel <= APLC_CH3)) {

		setDeviceState(channel);
		initPNController(channel);
		initMeasureChannel(channel);
		calculateCoefs(channel);
	};
}


/******************************************************************************
  Function:		initPtrs()			
  Description:	Initializes file global pointers
  Inputs:		Channel number
  Outputs:		
  Return:		
  Globals:		
  Important:	Call before using equalizer functions

*******************************************************************************/

static void SEGMENT_EXT_CODE initPtrs(void)
{
	g_pP4LT_Status = status_getPStatus();
	g_pAlarms = monitor_get_pAlarmsAndWarnings();
	g_pFastStatusRemote = status_getRemoteFastStatus();
	g_pEqualizerStatusActive = configuration_getPEqualizerStatus();
	g_pP4LT_Config = configuration_getPCurrentConfig();
}


/******************************************************************************
  Function:		setDeviceState()			
  Description:	Set DS_MEASURING_DATA_AVAILABLE_CHx bit in deviceState
  Inputs:		Channel number
  Outputs:		
  Return:		
  Globals:		
  Important:	

*******************************************************************************/

static void SEGMENT_EXT_CODE setDeviceState(long channel)
{
	
	if (g_pEqualizerStatusActive[channel].flag == EQUALIZER_STATUS_ACTIVE) {

		switch(channel) {

			case APLC_CH1:

				g_pP4LT_Status->system.deviceState |= DS_MEASURING_DATA_AVAILABLE_CH1;
				break;

			case APLC_CH2:

				g_pP4LT_Status->system.deviceState |= DS_MEASURING_DATA_AVAILABLE_CH2;
				break;

			case APLC_CH3:

				g_pP4LT_Status->system.deviceState |= DS_MEASURING_DATA_AVAILABLE_CH3;
				break;

			default: ASSERT(FALSE);
		};
	};
};


/******************************************************************************
  Function:		initPNController()			
  Description:	Initializes data used for pseudo noise generation
  Inputs:		Channel number
  Outputs:		
  Return:		
  Globals:		
  Important:	Call before using StateMachinePNController()

*******************************************************************************/

static void SEGMENT_EXT_CODE initPNController(long channel)
{
	g_pnControllerStatus[channel].statePn = PN_IDLE;
	g_pnControllerStatus[channel].timer = (long)TIMER_VALUE;
}


/******************************************************************************
  Function:		initMeasureChannel()		
  Description:	Initializes data used for channel measurement 
				(functions: measureChannel() and StateMachineAccController())
  Inputs:		Channel number		
  Outputs:				
  Return:			
  Globals:		
  Important:	Call before using measureChannel(), StateMachineAccController()

*******************************************************************************/

static void SEGMENT_EXT_CODE initMeasureChannel(long channel)
{
	g_chMeasurementStatus[channel].stateAccumulation = CHANNEL_ACCUMULATION_IDLE;
	g_chMeasurementStatus[channel].indexAccumulation = 0;
	g_chMeasurementStatus[channel].numberOfAccumulations = 0;
	g_chMeasurementStatus[channel].clearAccumulationArrayIndex = 0;

	g_chMeasurementStatus[channel].stateAccControl = MEASUREMENT_IDLE;
	g_chMeasurementStatus[channel].timerAccStateMachine = (long)TIMER_VALUE;
}


/******************************************************************************
  Function:		calculateCoefs()			
  Description:	Calculates the equalizer FIR filter coefficients
  Inputs:		channel	
  Outputs:		
  Return:		
  Globals:		
  Important:	

*******************************************************************************/

static void SEGMENT_EXT_CODE calculateCoefs(long channel)
{
	float tfRe[EQUALIZER_TF_LENGTH];	// real part of equalizer transfer function
    float tfIm[EQUALIZER_TF_LENGTH];	// imaginary part of equalizer transfer function
    float tfInt[EQUALIZER_TF_LENGTH];	// interpolated transfer function
    float eqCoef[PN_ACCUMULATOR_LENGTH];
	float *pPnAccu;
    long balancePoint;
    
	long ampEqOnlyFlag;
	long eqLength;
	float pilotFrequency;


	// get configuration data
	switch(g_pP4LT_Config->APLC.equalizer[channel].equalizerSettings) {

		case EQUALIZER_SETTINGS_NONE:

			eqLength = 0;
			break;

		case EQUALIZER_SETTINGS_SHORT:

			eqLength = EQUALIZER_SHORT_LENGTH;
			break;

		case EQUALIZER_SETTINGS_MEDIUM:

			eqLength = EQUALIZER_MIDDLE_LENGTH;
			break;

		case EQUALIZER_SETTINGS_LONG:

			eqLength = EQUALIZER_LONG_LENGTH;
			break;

		default: eqLength = 0; // no equalizer
	};

	ampEqOnlyFlag = g_pP4LT_Config->APLC.equalizer[channel].onlyAmplitudeEqualizationActive;
	pilotFrequency = g_pP4LT_Config->APLC.pilot[channel].frequency;

	pPnAccu = g_pEqualizerStatusActive[channel].pnAccu;

	if (g_pEqualizerStatusActive[channel].flag == EQUALIZER_STATUS_ACTIVE) {

		// calculate equalizer coefficients
		CalculateTransferFunctionEqualizer(pPnAccu, pilotFrequency, tfRe, tfIm);
		InterpolationEqualizer(tfRe, tfIm, tfInt, pilotFrequency);
		CalculateStepResponseEqualizer(tfRe, tfIm, tfInt, eqCoef, &balancePoint, ampEqOnlyFlag);
	}
	else {

		eqLength = 0;
		balancePoint = 0;
	};

	setFirCoefs(eqCoef, balancePoint, eqLength, &g_equalizerFirCoef[channel]);
};


/******************************************************************************
  Function:		setFirCoefs()			
  Description:	A subset of FIR coefficients is copied from eqCoef to eqFirCoef.	
  Inputs:		pointers to eqCoef and eqFirCoef, balancePoint, eqLength	
  Outputs:		FIR coefficient vector eqFirCoef[]
  Return:		
  Globals:		
  Important:	

*******************************************************************************/

void SEGMENT_EXT_CODE setFirCoefs(float *eqCoef, long balancePoint, long eqLength, 
								  t_EqualizerFirCoef *eqFirCoef)
{
	long i;
	static long eqCoefStartIndex;
	static long eqCoefIndex;
	static long eqFirCoefIndex;


	eqCoefStartIndex = (balancePoint + PN_ACCUMULATOR_LENGTH - (eqLength / 2))
						% PN_ACCUMULATOR_LENGTH;

	for(i = 0; i < eqLength; i++) {

		eqFirCoefIndex = EQUALIZER_MAX_LENGTH - 1 - i;	// fir filter coefs have to be set in reverse order
		eqCoefIndex = (eqCoefStartIndex + i) % PN_ACCUMULATOR_LENGTH;	
		
		((float*)eqFirCoef)[eqFirCoefIndex] = eqCoef[eqCoefIndex];
		
	};

	// set unused fir filter coefs to zero
	for(i = 0; i < (EQUALIZER_MAX_LENGTH - eqLength); i++) {

		((float*)eqFirCoef)[i] = 0.0;
	};

	if (eqLength == 0){		// no equalizer selected

		((float*)eqFirCoef)[EQUALIZER_MAX_LENGTH - 1] = 1.0;	// only one coef to copy input to output
	};
};


/******************************************************************************
  Function:		equalizer_getStatusNewPtr()			
  Description:	Returns the pointer to the specified equalizer status data.
  Inputs:		channel, type	
  Outputs:		
  Return:		Pointer to equalizer status data
  Globals:		
  Important:	

*******************************************************************************/

t_EqualizerStatus SEGMENT_EXT_CODE *equalizer_getStatusNewPtr(long channel)
{
	t_EqualizerStatus *pStatus;


	// check if channel valid
	if ((channel >= APLC_CH1) && (channel <= APLC_CH3)) {

			pStatus = &g_equalizerStatusNew[channel];		// data of latest channel measurement
	}
	else {	// invalid channel
	
		pStatus = &g_equalizerStatusNew[APLC_CH1];
	};

	return(pStatus);
}


/******************************************************************************
  Function:		equalizer_getFirCoefPtr()			
  Description:	Returns the pointer to the specified equalizer fir filter coefs.
  Inputs:		channel	
  Outputs:		
  Return:		Pointer to equalizer fir filter coef array		
  Globals:		
  Important:	

*******************************************************************************/

t_EqualizerFirCoef SEGMENT_EXT_CODE *equalizer_getFirCoefPtr(long channel)
{

	// check if channel valid
	if ((channel >= APLC_CH1) && (channel <= APLC_CH3)) {

		return(&g_equalizerFirCoef[channel]);
	}
	else {

		return(&g_equalizerFirCoef[APLC_CH1]);
	};
}


/******************************************************************************
  Function:		void equalizer_tdmDataTransfer()			
  Description:	Reads/writes equalizer data from/to the internal TDM bus.
  Inputs:		
  Outputs:		
  Return:		
  Globals:		
  Important:	Call in every main loop cycle (10.67 kHz) .

*******************************************************************************/

void equalizer_tdmDataTransfer(void)
{
	static unsigned long deviceState;


	// read data from TDM bus
	g_inputCmdActive = g_intTDM_RxStruct_Ptr->Channel1_InputCommandStatusData_TS12 ||
					   g_intTDM_RxStruct_Ptr->Channel2_InputCommandStatusData_TS14;

	// for testing
	//g_inputCmdActive = FALSE;

	deviceState = g_pP4LT_Status->system.deviceState;

	// write data to TDM bus
	g_intTDM_TxStruct_Ptr->flags.pnGenEnableCh1 = deviceState & DS_MEASURE_TX_CH1;
	g_intTDM_TxStruct_Ptr->flags.pnGenEnableCh2 = (deviceState & DS_MEASURE_TX_CH2)>>1;
	g_intTDM_TxStruct_Ptr->flags.pnGenEnableCh3 = (deviceState & DS_MEASURE_TX_CH3)>>2;

	g_intTDM_TxStruct_Ptr->flags.eqAfcDisable = g_outputFlags.afcDisable;
	g_intTDM_TxStruct_Ptr->flags.eqAgcDisableCh1 = g_outputFlags.agcDisable[APLC_CH1];
	g_intTDM_TxStruct_Ptr->flags.eqAgcDisableCh2 = g_outputFlags.agcDisable[APLC_CH2];
	g_intTDM_TxStruct_Ptr->flags.eqAgcDisableCh3 = g_outputFlags.agcDisable[APLC_CH3];

}


/******************************************************************************
  Function:		equalizer_stateMachinePnController()			
  Description:	Executes the state machines Pseudo Noise Controller and changes		   
				in the Idle state. Controls complete generation
				of pseudo noise of corresponding channel.
  Inputs:		Channel
  Outputs:		
  Return:		
  Globals:		
  Important:	Call initPNController() before using.

*******************************************************************************/

void SEGMENT_EXT_CODE equalizer_stateMachinePnController(long channel)
{
	long timerStatus = 0;
	long composedEvents;
	unsigned long bitMaskLinkAlarms;
	unsigned long bitMaskMeasureTxCh;
	unsigned long bitMaskMeasureRxCh;
	unsigned long *pDeviceStateLocal;
	unsigned long deviceStateRemote;

	static t_PnControllerStatus *pPnControllerStatus;

	
	switch(channel) {

		case APLC_CH1:

			bitMaskLinkAlarms = LINK_ALARMS_EQ_CH1;
			bitMaskMeasureTxCh = DS_MEASURE_TX_CH1;
			bitMaskMeasureRxCh = DS_MEASURE_RX_CH1;
			pPnControllerStatus = &g_pnControllerStatus[APLC_CH1];
			break;

		case APLC_CH2:
		
			bitMaskLinkAlarms = LINK_ALARMS_EQ_CH2;
			bitMaskMeasureTxCh = DS_MEASURE_TX_CH2;
			bitMaskMeasureRxCh = DS_MEASURE_RX_CH2;
			pPnControllerStatus = &g_pnControllerStatus[APLC_CH2];
			break;

		case APLC_CH3:
			
			bitMaskLinkAlarms = LINK_ALARMS_EQ_CH3;
			bitMaskMeasureTxCh = DS_MEASURE_TX_CH3;
			bitMaskMeasureRxCh = DS_MEASURE_RX_CH3;
			pPnControllerStatus = &g_pnControllerStatus[APLC_CH3];
			break;

		default: ASSERT(FALSE);
	};
		
	pDeviceStateLocal = &g_pP4LT_Status->system.deviceState;
	deviceStateRemote = g_pFastStatusRemote->deviceState;

	switch (pPnControllerStatus->statePn)
		{
		
		case PN_IDLE:

			// The other side wants to measure the channel caracteristics
			if ((deviceStateRemote & bitMaskMeasureRxCh) != 0)
				{
				// Normal operation mode: Remote command to switch on PN
				pPnControllerStatus->statePn = PN_RUNNING_HOT;
				pPnControllerStatus->timer = (long) TIMER_VALUE; // ResetStateMachineTimer
				*pDeviceStateLocal |= bitMaskMeasureTxCh;
				}
			break;
					
		case PN_RUNNING_HOT:
		

			pPnControllerStatus->timer--; // Decrement Timer
		    if (pPnControllerStatus->timer < TIMER_ZERO_VALUE) // Check Timer for Timeout
		    	{
		    	timerStatus = 1;
		    	}
			composedEvents = timerStatus || g_inputCmdActive ||
			// 				(g_pAlarms->plcLinkAlarms & bitMaskLinkAlarms) ||  // do not use if measuring shall be possible with only one equipment
							(g_pAlarms->plcHwAlarms & HW_ALARMS_EQ_TX);
			if (composedEvents)
				{
				// Timeout occurred, back to end state
				pPnControllerStatus->statePn = PN_END;
				*pDeviceStateLocal &= ~bitMaskMeasureTxCh;
				}


			else if ((deviceStateRemote & bitMaskMeasureRxCh) == 0)
				{
				// Remote interrupt signal, Normal operation or failure, switch off PN
				pPnControllerStatus->statePn = PN_IDLE;
				pPnControllerStatus->timer = (long) TIMER_VALUE; // ResetStateMachineTimer
				*pDeviceStateLocal &= ~bitMaskMeasureTxCh;
				}
			break;

		case PN_END:

			if ((deviceStateRemote & bitMaskMeasureRxCh) == 0)
			{
				pPnControllerStatus->statePn = PN_IDLE;
				pPnControllerStatus->timer = (long) TIMER_VALUE; // ResetStateMachineTimer
			}
			break;
				
		default: 

			// Error handling
			initPNController(channel);
			*pDeviceStateLocal &= ~bitMaskMeasureTxCh;
			break;
	}

}


/******************************************************************************
  Function:		void equalizer_measureChannel()			
  Description:	Routine to initialize and accumulate RxAfRaw Samples of noise
				during channel measurement.
  Inputs:		Channel number
  Outputs:		
  Return:		
  Globals:		
  Important:	Call initMeasureChannel() before using.

*******************************************************************************/

void equalizer_measureChannel(long channel)
{
	static float rxAfSample;
	static t_EqualizerStatus *pEqualizerStatus;
	static t_ChMeasurementStatus *pChMeasurementStatus;

	
	pChMeasurementStatus = &g_chMeasurementStatus[channel];
	pEqualizerStatus = &g_equalizerStatusNew[channel];

	switch(pChMeasurementStatus->stateAccumulation)
		{
	    
	    case CHANNEL_ACCUMULATION_IDLE:
		
			if (pChMeasurementStatus->pnAccumulationRunning == TRUE)
				{
				pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_CLEAR_ARRAY;
				pChMeasurementStatus->clearAccumulationArrayIndex = 0;	// Don't forget this
				pEqualizerStatus->flag = EQUALIZER_STATUS_NEW;			// Set flag for MMI new measurement data
				}
			break;
			
		case CHANNEL_ACCUMULATION_CLEAR_ARRAY:

			if (pChMeasurementStatus->clearAccumulationArrayIndex < PN_ACCUMULATOR_LENGTH)
				{
				pEqualizerStatus->pnAccu[pChMeasurementStatus->clearAccumulationArrayIndex] = 0.0;
				pChMeasurementStatus->clearAccumulationArrayIndex++;
				}
			else 
				{
				pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_MEASURE;
				pChMeasurementStatus->indexAccumulation = 0;
				pChMeasurementStatus->numberOfAccumulations = 0;
				}
			if (pChMeasurementStatus->pnAccumulationRunning == FALSE) 
									pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_ERROR;
			break;
			
			
		case CHANNEL_ACCUMULATION_MEASURE:

			rxAfSample = g_intTDM_RxStruct_Ptr->APLC_RxSignalNotEqualized[channel];
			pEqualizerStatus->pnAccu[pChMeasurementStatus->indexAccumulation] = 
        		    	pEqualizerStatus->pnAccu[pChMeasurementStatus->indexAccumulation] + rxAfSample;
        	pChMeasurementStatus->indexAccumulation++;

//			Not used: only one sample has to be accumulated at 10.67 kHz @HM@
/*        	pEqualizerStatus->pnAccu[pChMeasurementStatus->indexAccumulation] = 
        		    	pEqualizerStatus->pnAccu[pChMeasurementStatus->indexAccumulation] + RxAfRaw[1];
        	pChMeasurementStatus->indexAccumulation++;
*/
        	if (pChMeasurementStatus->pnAccumulationRunning == FALSE) 
        		pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_ERROR;
        	if (pChMeasurementStatus->indexAccumulation >= PN_ACCUMULATOR_LENGTH)
				{
				pChMeasurementStatus->indexAccumulation = 0;
				pChMeasurementStatus->numberOfAccumulations++;
				}
			if (pChMeasurementStatus->numberOfAccumulations >= MAX_NUM_OF_ACCUMULATIONS)
				{
				pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_IDLE;
				pChMeasurementStatus->clearAccumulationArrayIndex =  0;
				pChMeasurementStatus->numberOfAccumulations =  0;
				pChMeasurementStatus->indexAccumulation = 0;
				pChMeasurementStatus->pnAccumulationRunning = FALSE; // Feedback on state machine
				}
			break;
			
		case CHANNEL_ACCUMULATION_ERROR:

			pEqualizerStatus->pnAccu[0] = 0.0;	// Flag Measurement invalid
			pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_IDLE;
			pEqualizerStatus->flag = EQUALIZER_STATUS_EMPTY;	// ETL500: EQUALIZER_STATUS_NEW;
			break;
						
		default:

			pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_IDLE;
			break;
	}
}


/******************************************************************************
  Function:		void equalizer_stateMachineAccController()			
  Description:	Executes the state machine Noise Accumulation Controller.

  Inputs:		Channel number
  Outputs:		
  Return:		
  Globals:		
  Important:	Call initMeasureChannel() before using.

*******************************************************************************/

void SEGMENT_EXT_CODE equalizer_stateMachineAccController(long channel)
{
	long timerStatus = 0;
	long composedEvents;
	unsigned long bitMaskLinkAlarms;
	unsigned long bitMaskPilotNotOk;
	unsigned long bitMaskMeasureTxCh;
	unsigned long bitMaskMeasureRxCh;
	unsigned long bitMaskHwAlarms;
	unsigned long *pDeviceStateLocal;
	unsigned long deviceStateRemote;

	static t_ChMeasurementStatus *pChMeasurementStatus;

	
	switch(channel) {
		
		case APLC_CH1:
		
			bitMaskLinkAlarms = LINK_ALARMS_EQ_CH1;
			bitMaskPilotNotOk = RX_PILOT_ALARMS_EQ_CH1;
			bitMaskMeasureTxCh = DS_MEASURE_TX_CH1;
			bitMaskMeasureRxCh = DS_MEASURE_RX_CH1;
			pChMeasurementStatus = &g_chMeasurementStatus[APLC_CH1];
			break;

		case APLC_CH2:
		
			bitMaskLinkAlarms = LINK_ALARMS_EQ_CH2;
			bitMaskPilotNotOk = RX_PILOT_ALARMS_EQ_CH2;
			bitMaskMeasureTxCh = DS_MEASURE_TX_CH2;
			bitMaskMeasureRxCh = DS_MEASURE_RX_CH2;
			pChMeasurementStatus = &g_chMeasurementStatus[APLC_CH2];
			break;
		
		case APLC_CH3:

			bitMaskLinkAlarms = LINK_ALARMS_EQ_CH3;
			bitMaskPilotNotOk = RX_PILOT_ALARMS_EQ_CH3;
			bitMaskMeasureTxCh = DS_MEASURE_TX_CH3;
			bitMaskMeasureRxCh = DS_MEASURE_RX_CH3;
			pChMeasurementStatus = &g_chMeasurementStatus[APLC_CH3];
			break;

		default: ASSERT(FALSE);
	};

	bitMaskHwAlarms = HW_ALARMS_EQ_RX;
	pDeviceStateLocal = &g_pP4LT_Status->system.deviceState;
	deviceStateRemote = g_pFastStatusRemote->deviceState;
	
	switch (pChMeasurementStatus->stateAccControl)
	{
		case MEASUREMENT_IDLE:

			if ((*pDeviceStateLocal & bitMaskMeasureRxCh) != 0)
				{
				// Normal operation: Request remote ETL to switch on PN
				pChMeasurementStatus->stateAccControl = MEASUREMENT_PN_WAITING;
				}
			break;
				
		case MEASUREMENT_PN_WAITING:
                                                  
		    pChMeasurementStatus->timerAccStateMachine--; // Decrement Timer
		    if (pChMeasurementStatus->timerAccStateMachine < TIMER_ZERO_VALUE) // Check Timer for Timeout
		    	{
		    	timerStatus = 1;
		    	}
			composedEvents = timerStatus || 
							(g_pAlarms->plcLinkAlarms & bitMaskLinkAlarms) || 
							(g_pAlarms->plcHwAlarms & bitMaskHwAlarms);
			
			if (composedEvents)
				{
				// Timeout occurred, Set error measurement, go to ende state
		        pChMeasurementStatus->stateAccControl = MEASUREMENT_END;
				pChMeasurementStatus->pnAccumulationRunning = FALSE;
				*pDeviceStateLocal &= ~bitMaskMeasureRxCh;
				// Signalize error in measurement, set first element zero, Signal in equalizer_measureChannel()
				pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_ERROR;	// Goto error state and set first element 0
				}			
			else if (deviceStateRemote & bitMaskMeasureTxCh)
				{
				// Normal operation: PN running remote, begin accumulation
				pChMeasurementStatus->pnAccumulationRunning = TRUE;
				g_outputFlags.afcDisable = TRUE;
				g_outputFlags.agcDisable[channel] = TRUE;
				pChMeasurementStatus->stateAccControl = MEASUREMENT_RUNNING;
				pChMeasurementStatus->timerAccStateMachine = (long) TIMER_VALUE; // ResetStateMachineTimer
				}
			break;
					
		case MEASUREMENT_RUNNING:

		    pChMeasurementStatus->timerAccStateMachine--; // Decrement Timer
		    if (pChMeasurementStatus->timerAccStateMachine < TIMER_ZERO_VALUE) // Check Timer for Timeout
		    	{
		    	timerStatus = 1;
		    	}
		    composedEvents = timerStatus || (g_pAlarms->plcLinkAlarms & bitMaskPilotNotOk);
			if (timerStatus)	// Timeout occured, go back
				{
				// Timeout occured: Go to ende state, error set
				pChMeasurementStatus->stateAccControl = MEASUREMENT_END;
				pChMeasurementStatus->pnAccumulationRunning = FALSE;
				*pDeviceStateLocal &= ~bitMaskMeasureRxCh;
				// Signalize error in measurement, set first element zero, Signal in equalizer_measureChannel()
				pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_ERROR;	// Goto error state and set first element 0
				g_outputFlags.afcDisable = FALSE;
	        	g_outputFlags.agcDisable[channel] = FALSE;
				}	
			else if ((deviceStateRemote & bitMaskMeasureTxCh) == 0)
				{
				// Remote interrupt occurred: Measurement error
				*pDeviceStateLocal &= ~bitMaskMeasureRxCh;
				pChMeasurementStatus->pnAccumulationRunning = FALSE;
				pChMeasurementStatus->stateAccControl = MEASUREMENT_IDLE;
				pChMeasurementStatus->timerAccStateMachine = (long) TIMER_VALUE; // ResetStateMachineTimer
				// Signalize error in measurement, set first element zero, Signal in equalizer_measureChannel()
				pChMeasurementStatus->stateAccumulation = CHANNEL_ACCUMULATION_ERROR;	// Goto error state and set first element 0
				g_outputFlags.afcDisable = FALSE;
	        	g_outputFlags.agcDisable[channel] = FALSE;
				}
			else if (pChMeasurementStatus->pnAccumulationRunning == FALSE)
				{				
				if ((deviceStateRemote & bitMaskMeasureTxCh) != 0)
					{
					// Normal operation: Measurement finished
					*pDeviceStateLocal &= ~bitMaskMeasureRxCh;
					pChMeasurementStatus->stateAccControl = MEASUREMENT_END;
					g_outputFlags.afcDisable = FALSE;
					g_outputFlags.agcDisable[channel] = FALSE;
					}
				}
			break;
				
		case MEASUREMENT_END:
		
			if ((deviceStateRemote & bitMaskMeasureTxCh) == 0)
				{
				// Awaiting acknowledge from remote ETL: Go back to idle state
				pChMeasurementStatus->stateAccControl = MEASUREMENT_IDLE;
				pChMeasurementStatus->timerAccStateMachine = (long) TIMER_VALUE; // ResetStateMachineTimer
				}
			break;
				
		default: 

			*pDeviceStateLocal &= ~bitMaskMeasureRxCh;
			pChMeasurementStatus->stateAccControl = MEASUREMENT_IDLE;
			pChMeasurementStatus->timerAccStateMachine = (long) TIMER_VALUE; // ResetStateMachineTimer

		}

}


