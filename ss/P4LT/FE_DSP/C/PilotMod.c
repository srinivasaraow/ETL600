                                                
/*-------------------------------------------------------------------------*
 * $Workfile: PilotMod.c $
 * Part of	: ETL600 / Front End DSP
 * Language	: C
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: Pilot modem 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/FE_DSP/C/PilotMod.c $
 * 
 * 9     5.10.05 13:58 Chhamaa
 * Test functions commented out.
 * 
 * 8     3.08.05 17:37 Chhamaa
 * Initialization of level alarm thresholds (guardConfig) modified 
 * 
 * 7     29.07.05 16:08 Chhamaa
 * LONG_PULSE set to 250
 * 
 * 6     28.07.05 12:02 Chhamaa
 * No guard alarm added in evaluateGuard()
 * 
 * 5     29.06.05 14:24 Chhamaa
 * Decrement txInitCounter only if > 0
 * 
 * 4     11.04.05 9:48 Chhamaa
 * RX_GAIN_MIN set to 1.0
 * 
 * 3     24.03.05 10:59 Chhamaa
 * 
 * 2     4.03.05 18:52 Chhamaa
 * Functions initPilotModemChx() modified to configure pilot weight.
 * Constants related to pilot weight removed.
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 16    6.01.05 13:37 Chhamaa
 * 
 * 15    17.12.04 15:58 Chhamaa
 * 
 * 14    16.12.04 16:16 Chhamaa
 * EocDataBufferType dataBreak replaced by PilotModDataType dataBreak in
 * pilotTransmitter(). Update of EocDataBufferType dataBreak in
 * txDataProcessing().
 * 
 * 13    1.12.04 17:24 Chhamaa
 * 
 * 12    30.11.04 15:32 Chhamaa
 * 
 * 11    26.11.04 15:18 Maag01
 * updatePilotStatus() modified
 * 
 * 10    23.11.04 17:06 Maag01
 * Constants added from pmconst.h, grecc.h
 * 
 * 9     4.11.04 19:27 Maag01
 * 
 * 8     4.11.04 10:20 Maag01
 * 
 * 7     3.11.04 16:48 Maag01
 * rx/tx buffer flags isFirstWord, overrun, outOfData removed
 * 
 * 6     7.10.04 18:00 Maag01
 * 
 * 5     5.08.04 15:35 Leeb02
 * 
 * 4     22.07.04 16:45 Maag01
 * GuardRec functions integrated
 * 
 * 3     14.07.04 12:18 Leeb02
 * Dependencies removed
 * 
 * 2     17.06.04 18:24 Maag01
 * Block length set to BLOCK_LENGTH_R_4K
 * 
 * 1     30.04.04 14:52 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 *  
 * 
 *
 *-------------------------------------------------------------------------*/

/*-----------------------INCLUDE FILES-------------------------------------*/

#include <math.h>
#include <stdlib.h>

#include "basetype.h"

#include "pilotMod.h"			// function prototypes
#include "pmfilter.h"			// coefficients for rx/tx filter; test tone filter coefficients		
#include "BiIIR_Filter.h"


/*-----------------------LOCAL CONSTANTS-----------------------------------*/

// General constants
#define FTWO					  2.0			// float two

// Demodulator Frequency PI-Controller Parameters
#define KI					    180.0/533.3 	// integral constant (division by loop sampling rate) 100.0/533
#define KP 						  4.0		    // proportional constant 3.0
#define KF			       10.0/1066.67*TWO_PI  // delta f  [Hz/V]
#define INTEGRATOR_MAX			  2.5	        // max. value of integrator state variable

// Amplitude Adjustment Constant
#define PILOT_AMP_AJUST_CONST     1.5

//Pilot Receiver Parameters 
#define SAMPLES_PER_REC_BIT		 10				// number of samples per received bit (before sampling, the data channel is decimated by 10)
#define SHORT_PULSE				 12    			// max. tolerated pulse width of 1 bit data pulses in [ms]  
#define SHORT_PULSE_MIN			  4   			// min. tolerated short pulse width of 1 bit data pulses in [ms]
#define LONG_PULSE				250   			// (old value: 300) min. length of framestarts in [ms]
#define INIT_LENGTH            1000             // init length for measureShort() function
#define DATA_BIT_SAMPLE_OFFSET	  5				// offset (after frame start) to adjust sample time of data bits 
#define DATA_DELAY_REC          500             // [s*10e-2] data delay after pulse on MWire 
#define RX_INIT_DELAY		    200 			// [s*10e-2] delay of state init/pilotFailure in function pilotReceiverController()
#define MIN_EDGE                  2             // min. number of edge to detect in measureShort() 

#define EWIRE_ACTIVE              1             // Active State of EWire (generateEWire)
#define EWIRE_INACTIVE			  0				// Inactive State of EWire (generateEWire)
#define EWIRE_HOLD_TIME        7000             // [ms] hold time of old E-Wire signal (receiverController)

//Pilot Transmitter Parameter
#define PILOT_PEAK_AMPLITUDE	  1.41421356	// sqrt(2.0)
#define SAMPLES_PER_BIT  		 20				// number of AF samples per bit
#define WORD_LENGTH				 11             // number of bits of tx data words
#define DATA_DELAY_TRAN  0.8*100*DATA_DELAY_REC // [s*10e-4] (number of AF samples) data delay after pulse on MWire 

#define MIN_PW_OF_MW             80             // min. pulse width on M-Wire
#define SYNC_DELAY			  11000             // [s*10e-4] data transmission and MWire signalling are interrupted, a sync frame is sent instead
#define INIT_WORD             0xFFFFFFFF        // init word for pilot transmitter to send at the beginning and during breaks
#define TX_INIT_DELAY          2000             // after pilot modem initialization no pilot is sent for TX_INIT_DELAY

// Gurad receiver constants
#define PILOT_OK_DELAY			  2     		//[s*10e-2] delay of signal pilotIsOk after pilot recovery to avoid oscillations
#define PILOT_FALSE_DELAY         2     		//[s*10e-2] delay of signal pilotIsOk after pilot failure to avoid oscillations
#define GUARD_IS_PRESENT_DELAY	  2     		//[2 ms] short delay of signal guardIsPresent after pilot recovery
#define SIG_OK_DELAY			100	    		//[s*10e-2] delay after pilot fail to detect a sudden attenuation (decision levels are frozen)
#define FAST_MUTE_PROLONGATION 2500				//[2 ms] prolongation of signal fastMute to avoid oscillation

#define GUARD_LEVEL_HYSTERESIS    1.41  		// Hysteresis of min. guard level (1.41 correponds to 3 dB)
#define GUARD_THRESHOLD_LOW		  0.5 			// Threshold of guard level guardIsPresent (0.5 -> -6 dB)
#define GUARD_THRESHOLD_HIGH      1.8 			// Upper guard level for signal fastMute to detect noise burst
#define NOISE_THRESHOLD			 35.0  		    // Noise-only detection level of guard receiver (35 -> 15.4 dB)
#define SNR_THRESHOLD_HIGH		 50.0  			// Noise-threshold high for SNR-alarm in guard receiver (50 -> 17 dB)
#define SNR_THRESHOLD_LOW		 35.0   		// Noise-threshold low for SNR-alarm in guard receiver (35 -> 15.4 dB)
#define NO_GUARD_LEVEL			  5.6234e-5		// -85 dB (level too low for guard receiver)

#define TH_SCALING				  0.5			// scaling factor for amplitude thresholds configured by HMI600

#define HYST_LEVEL_ALARM          2.0           // Hysteresis of pilot level alarm (2 -> 3 dB)
#define HYST_SNR_ALARM            2.0           // Hysteresis of snr alarm (2 -> 3 dB)

#define GL_COEF_1                 0.985         // guard level low pass filter constant  0.975
#define GL_COEF_2                 0.015         // guard level low pass filter constant

#define NL_COEF_1                 0.99          // noise level low pass filter constant
#define NL_COEF_2                 0.01          // noise level low pass filter constant

#define SNR_COEF_1                0.997         // snr low pass filter constant
#define SNR_COEF_2            	  0.003         // snr low pass filter constant

//AGC parameters
#define RX_GAIN_MIN				  1.0			// min. gain factor  0.0 dB
#define RX_GAIN_MAX               1.0e+4		// max. gain factor 80.0 dB

#define RXG_COEF_1                0.93          // rx gain low pass filter constant (first section)
#define RXG_COEF_2                0.07          // rx gain low pass filter constant (first section)

#define RXG_STATUS_COEF_1         0.997         // rx gain low pass filter constant (second section)
#define RXG_STATUS_COEF_2         0.003         // rx gain low pass filter constant (second section)


// for test functions
												// SP DATA DATA S
#define TEST_DATA_WORD0			0x0400			// 10 0000 0000 0
#define TEST_DATA_WORD1			0x05FE			// 10 1111 1111 0
#define TEST_DATA_WORD2			0x05E0			// 10 1111 0000 0
#define TEST_DATA_WORD3			0x0598			// 10 1100 1100 0
#define TEST_DATA_WORD4			0x0554			// 10 1010 1010 0
#define TEST_DATA_WORD5			0x0670			// 11 0011 1000 0
#define TEST_DATA_START			0x0568			// 11 0110 1000 0
#define TEST_DATA_INIT			0x07FF			// 11 1111 1111 1


/*-----------------------IMPORTED DATA-------------------------------------*/

	
/*-----------------------LOCAL DATA DEFINITIONS----------------------------*/

// tx, rx filter Ch1
float dm txFilterState1Ch1[PMF_SARRAY_LENGTH], txFilterState2Ch1[PMF_SARRAY_LENGTH];
float dm rxFilterState1Ch1[PMF_SARRAY_LENGTH], rxFilterState2Ch1[PMF_SARRAY_LENGTH];

// tx, rx filter Ch2 
float dm txFilterState1Ch2[PMF_SARRAY_LENGTH], txFilterState2Ch2[PMF_SARRAY_LENGTH];
float dm rxFilterState1Ch2[PMF_SARRAY_LENGTH], rxFilterState2Ch2[PMF_SARRAY_LENGTH];

// tx, rx filter Ch3 
float dm txFilterState1Ch3[PMF_SARRAY_LENGTH], txFilterState2Ch3[PMF_SARRAY_LENGTH];
float dm rxFilterState1Ch3[PMF_SARRAY_LENGTH], rxFilterState2Ch3[PMF_SARRAY_LENGTH];


/*------------------- LOCAL MACRO DEFINITIONS------------------------------*/

#define sqr(x) ((x)*(x))


/*------------------------IMPLEMENTATION-----------------------------------*/

#pragma optimize_off

/*-------------------------------------------------------------------------*/               
/* void pilotOscillatorInit(float oscFreq, float sampleFreq,      		   */
/*						    PilotOscDataType *osc)						   */ 
/*                                                                         */
/* Initializes osillator parameters 							           */
/*                                                                         */
/* Input: oscillator frequency, sample frequency, oscillator parameters    */
/* Output: initalized oscillator parameters		                           */
/*-------------------------------------------------------------------------*/  

void SEGMENT_EXT_CODE pilotOscillatorInit(float oscFreq, float sampleFreq, PilotOscDataType *osc)
{ 
  register float wo;
 
  // initialize oscillator parameters
  wo = oscFreq*TWO_PI/sampleFreq;
  
  osc->sampleFreq = sampleFreq;
  osc->oscFreq = oscFreq;
  osc->x1 = 0.0;
  osc->x2 = -1.0;
  osc->a = cosf(wo);
  osc->b = sinf(wo);  
}
   

/*-------------------------------------------------------------------------*/               
/* void channelDataInit(PilotDemodDataType *pilotDemod)					   */
/*                                                                         */
/* Initializes channel parameters 							               */
/*                                                                         */
/* Input:    															   */    
/* Output: initalized parameters: channel parameters    				   */
/*-------------------------------------------------------------------------*/

static void channelDataInit(PilotDemodDataType *pilotDemod)
{
  pilotDemod->dataChannelPtr = &pilotDemod->shiftedI;
  pilotDemod->sigChannelPtr = &pilotDemod->shiftedQ;
  pilotDemod->sgnData = 1.0;
  pilotDemod->sgnSig = 1.0;
}


/*-------------------------------------------------------------------------*/               
/* void initDemodFreqControl(PilotDemodDataType *pilotDemod)	 		   */
/*																		   */
/* Initializes the parameters used by the function demodFreqControl() 	   */
/*                                                                         */
/* Input: struct pilotDemod  						     			       */
/* Output: struct pilotDemod 											   */
/*-------------------------------------------------------------------------*/ 

static void initDemodFreqControl(PilotDemodDataType *pilotDemod)
{
  pilotDemod->x1 = 1.0;
  pilotDemod->x2 = 0.0;
  pilotDemod->stateI = 0.0;
  pilotDemod->uf = 0.0;
}   


/*-------------------------------------------------------------------------*/               
/* void initPilotModemCh1()									 			   */
/*																		   */
/* Initializes pilot modem functions of channel 1   				 	   */
/*                                                                         */
/* Input: 					 						     			       */
/* Output: 																   */
/*-------------------------------------------------------------------------*/ 

void SEGMENT_EXT_CODE initPilotModemCh1(t_PilotModemConfig *pilotConfig, 
										PilotModDataType *pilotModData,
										PilotDemodDataType *pilotDemodData,
										PilotRecDataType *pilotRecData,
										GuardConfigType *guardConfig,
										GuardDataType *guardStatus,
										agcDataType *agcData)
{
  float weightPilot;

  // set weightPilot
  weightPilot = pilotConfig->weight;

  // pilot modulator initialisation
  pilotOscillatorInit(pilotConfig->frequency, AF_SAMPLE_FREQ_4K, &pilotModData->osc);
  BiIIR_Filter_init(&pilotModData->filterData, pilotFilterCoef, txFilterState1Ch1, txFilterState2Ch1, NUM_OF_PF_SEC);
  pilotModData->txScalingCoef = weightPilot/PILOT_PEAK_AMPLITUDE;
  pilotModData->txInitCounter = TX_INIT_DELAY;
   
  // pilot demodulator initialisation
  channelDataInit(pilotDemodData);
  initDemodFreqControl(pilotDemodData);
  pilotOscillatorInit(pilotConfig->frequency, AF_SAMPLE_FREQ_4K, &pilotDemodData->osc);
  BiIIR_Filter_init(&pilotDemodData->filterData, pilotFilterCoef, rxFilterState1Ch1, rxFilterState2Ch1, NUM_OF_PF_SEC); 
  pilotDemodData->rxLevelAtt = 1.0/(weightPilot/PILOT_PEAK_AMPLITUDE);
  pilotDemodData->EWireOn = 0;
  pilotDemodData->EWire = 0;
  pilotDemodData->EWireOld = 0;
  pilotDemodData->channelsAreId = 0;
  pilotRecData->initCounter = RX_INIT_DELAY;
  pilotRecData->state = pilotFailure;

  // set guard configuration
  guardConfig->levelAlarmTh = pilotConfig->levelAlarmThresholdLin/sqr(PILOT_PEAK_AMPLITUDE);	 // power threshold
  guardConfig->snrAlarmTh = pilotConfig->snrAlarmThresholdLin;									 // power threshold	
  guardConfig->levelLowTh = pilotConfig->pilotLevelLowThreshold*TH_SCALING/PILOT_PEAK_AMPLITUDE; // amplitude threshold 
  guardConfig->noGuardLevelTh = NO_GUARD_LEVEL*TH_SCALING/PILOT_PEAK_AMPLITUDE;					 // amplitude threshold						

  // initialize guard status
  guardStatus->pilotIsOk = 0;

  // initialize AGC data
  agcData->ok = 0;
  agcData->rxGain = 0.0;

  // AGC evaluates I or Q-part of pilot signal (amplitude)
  agcData->rxLevelDesired = TH_SCALING*(weightPilot/PILOT_PEAK_AMPLITUDE);
  
}


/*-------------------------------------------------------------------------*/               
/* void initPilotModemCh2(void)									 		   */
/*																		   */
/* Initializes pilot modem functions of channel 2	  				 	   */
/*                                                                         */
/* Input: 					 						     			       */
/* Output: 																   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE initPilotModemCh2(t_PilotModemConfig *pilotConfig, 
										PilotModDataType *pilotModData,
										PilotDemodDataType *pilotDemodData,
										PilotRecDataType *pilotRecData,
										GuardConfigType *guardConfig,
										GuardDataType *guardStatus,
										agcDataType *agcData)
{
  float weightPilot;

  // set weightPilot
  weightPilot = pilotConfig->weight;

  // pilot modulator initialisation
  pilotOscillatorInit(pilotConfig->frequency, AF_SAMPLE_FREQ_4K, &pilotModData->osc);
  BiIIR_Filter_init(&pilotModData->filterData, pilotFilterCoef, txFilterState1Ch2, txFilterState2Ch2, NUM_OF_PF_SEC);
  pilotModData->txScalingCoef = weightPilot/PILOT_PEAK_AMPLITUDE;
  pilotModData->txInitCounter = TX_INIT_DELAY;
   
  // pilot demodulator initialisation
  channelDataInit(pilotDemodData);
  initDemodFreqControl(pilotDemodData);
  pilotOscillatorInit(pilotConfig->frequency, AF_SAMPLE_FREQ_4K, &pilotDemodData->osc);
  BiIIR_Filter_init(&pilotDemodData->filterData, pilotFilterCoef, rxFilterState1Ch2, rxFilterState2Ch2, NUM_OF_PF_SEC); 
  pilotDemodData->rxLevelAtt = 1.0/(weightPilot/PILOT_PEAK_AMPLITUDE);
  pilotDemodData->EWireOn = 0;
  pilotDemodData->EWire = 0;
  pilotDemodData->EWireOld = 0;
  pilotDemodData->channelsAreId = 0;
  pilotRecData->initCounter = RX_INIT_DELAY;
  pilotRecData->state = pilotFailure;    
  
  // set guard configuration
  guardConfig->levelAlarmTh = pilotConfig->levelAlarmThresholdLin/sqr(PILOT_PEAK_AMPLITUDE);	 // power threshold
  guardConfig->snrAlarmTh = pilotConfig->snrAlarmThresholdLin;									 // power threshold	
  guardConfig->levelLowTh = pilotConfig->pilotLevelLowThreshold*TH_SCALING/PILOT_PEAK_AMPLITUDE; // amplitude threshold 
  guardConfig->noGuardLevelTh = NO_GUARD_LEVEL*TH_SCALING/PILOT_PEAK_AMPLITUDE;					 // amplitude threshold						

  // initialize guard status
  guardStatus->pilotIsOk = 0;

  // initialize AGC data
  agcData->ok = 0;
  agcData->rxGain = 0.0;

  // AGC evaluates I or Q-part of pilot signal (amplitude)
  agcData->rxLevelDesired = TH_SCALING*(weightPilot/PILOT_PEAK_AMPLITUDE);
  
}


/*-------------------------------------------------------------------------*/               
/* void initPilotModemCh3(void)									 		   */
/*																		   */
/* Initializes pilot modem functions of channel 3	  				 	   */
/*                                                                         */
/* Input: 					 						     			       */
/* Output: 																   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE initPilotModemCh3(t_PilotModemConfig *pilotConfig, 
										PilotModDataType *pilotModData,
										PilotDemodDataType *pilotDemodData,
										PilotRecDataType *pilotRecData,
										GuardConfigType *guardConfig,
										GuardDataType *guardStatus,
										agcDataType *agcData)  
{  
  float weightPilot;

  // set weightPilot
  weightPilot = pilotConfig->weight;

  // pilot modulator initialisation
  pilotOscillatorInit(pilotConfig->frequency, AF_SAMPLE_FREQ_4K, &pilotModData->osc);
  BiIIR_Filter_init(&pilotModData->filterData, pilotFilterCoef, txFilterState1Ch3, txFilterState2Ch3, NUM_OF_PF_SEC);
  pilotModData->txScalingCoef = weightPilot/PILOT_PEAK_AMPLITUDE;
  pilotModData->txInitCounter = TX_INIT_DELAY;
   
  // pilot demodulator initialisation
  channelDataInit(pilotDemodData);
  initDemodFreqControl(pilotDemodData);
  pilotOscillatorInit(pilotConfig->frequency, AF_SAMPLE_FREQ_4K, &pilotDemodData->osc);
  BiIIR_Filter_init(&pilotDemodData->filterData, pilotFilterCoef, rxFilterState1Ch3, rxFilterState2Ch3, NUM_OF_PF_SEC); 
  pilotDemodData->rxLevelAtt = 1.0/(weightPilot/PILOT_PEAK_AMPLITUDE);
  pilotDemodData->EWireOn = 0;
  pilotDemodData->EWire = 0;
  pilotDemodData->EWireOld = 0;
  pilotDemodData->channelsAreId = 0;
  pilotRecData->initCounter = RX_INIT_DELAY;
  pilotRecData->state = pilotFailure;  
  
  // set guard configuration
  guardConfig->levelAlarmTh = pilotConfig->levelAlarmThresholdLin/sqr(PILOT_PEAK_AMPLITUDE);	 // power threshold
  guardConfig->snrAlarmTh = pilotConfig->snrAlarmThresholdLin;									 // power threshold	
  guardConfig->levelLowTh = pilotConfig->pilotLevelLowThreshold*TH_SCALING/PILOT_PEAK_AMPLITUDE; // amplitude threshold 
  guardConfig->noGuardLevelTh = NO_GUARD_LEVEL*TH_SCALING/PILOT_PEAK_AMPLITUDE;					 // amplitude threshold						

  // initialize guard status
  guardStatus->pilotIsOk = 0;

  // initialize AGC data
  agcData->ok = 0;
  agcData->rxGain = 0.0;

  // AGC evaluates I or Q-part of pilot signal (amplitude)
  agcData->rxLevelDesired = TH_SCALING*(weightPilot/PILOT_PEAK_AMPLITUDE);
  
}


#pragma optimize_as_cmd_line

/*-------------------------------------------------------------------------*/               
/* void pilotAmpAdjust(PilotOscDataType *osc)							   */
/*																		   */
/* In order to garantee a constant oscillator amplitude internal states    */
/* have to be adjusted periodically.                                       */
/*                                                                         */
/* Input: oscillator parameters osc      							       */
/* Output: adjusted oscillator states 			                           */
/*-------------------------------------------------------------------------*/

void pilotAmpAdjust(PilotOscDataType *osc)
{
  register float h, v;    
  
  h = DIV_BY_TWO*(sqr(osc->x1*osc->b) + sqr(osc->x1*osc->a - osc->x2)); 
  v = PILOT_AMP_AJUST_CONST - h;       	// amplidtude adjustment factor 
  
  if ((h == 0.0) || (v <= 0.0)) {   // reset states to initial conditions 
    osc->x1 =  0.0;   
    osc->x2 = -1.0;
  }     
  else {                 			// state adjustment 
    osc->x1 = v*osc->x1;
    osc->x2 = v*osc->x2;
  }   
}  


/*-------------------------------------------------------------------------*/               
/* void pilotBbAmpAdjust(PilotDemodDataType *pilotDemod)				   */
/*																		   */
/* In order to garantee a constant oscillator amplitude internal states    */
/* have to be adjusted periodically. Especially the vco amplitude is 	   */
/* unstable because of changing frequency. This function is used to adjust */
/* the amplitude of the base band VCO in demodFreqControl().		  	   */
/*                                                                         */
/* Input: oscillator parameters osc      							       */
/* Output: adjusted oscillator states 			                           */
/*-------------------------------------------------------------------------*/

void pilotBbAmpAdjust(PilotDemodDataType *pilotDemod)
{
  register float v;
  
  v = sqrtf(sqr(pilotDemod->x1) + sqr(pilotDemod->x2)); // rsqrtf() doesn't work correctly !!

  if (v == 0.0) {
    pilotDemod->x1 = 1.0;           	// reset states to initial conditions
    pilotDemod->x2 = 0.0;  
  }
  else {								// state adjustment 
    v = 1.0/v; 
    pilotDemod->x1 = v*pilotDemod->x1;
    pilotDemod->x2 = v*pilotDemod->x2;  
  }
}                                          


/*-------------------------------------------------------------------------*/               
/* void phaseShift90(PilotDemodDataType *pilotDemod)			   		   */
/*																		   */
/* Performes a 90° phase shift of the demodulated channels.                */
/*                                                                         */
/* Input: channel pointers demodOutPtr and signs sgn                       */
/* Output: shifted channel pointers and signs                              */
/*-------------------------------------------------------------------------*/  

static void phaseShift90(PilotDemodDataType *pilotDemod)
{
  register float *savePtr;
  register float saveSgn;
  
  savePtr = pilotDemod->dataChannelPtr; 
  pilotDemod->dataChannelPtr = pilotDemod->sigChannelPtr; 
  pilotDemod->sigChannelPtr = savePtr;
  
  saveSgn = pilotDemod->sgnData;
  pilotDemod->sgnData = -pilotDemod->sgnSig;
  pilotDemod->sgnSig = saveSgn;
}
 
/*-------------------------------------------------------------------------*/               
/* void phaseShift180(PilotDemodDataType *pilotDemod)			   		   */
/*																		   */
/* Performes a 180° phase shift of the demodulated channels by sign        */
/* inversion                                                               */ 
/*                                                                         */
/* Input: channel signs sgn						                           */
/* Output: inverted signs sgn					                           */
/*-------------------------------------------------------------------------*/   

inline void phaseShift180(PilotDemodDataType *pilotDemod)                                             
{
  pilotDemod->sgnData = -pilotDemod->sgnData;
  pilotDemod->sgnSig = -pilotDemod->sgnSig;
}

 
/*-------------------------------------------------------------------------*/               
/* void detectFrameStart(long data, FrameStartDataType *frameStart) 	   */
/*																		   */
/* Detects a negativ transition in the signal data coresponding to a frame */
/* start                                                  				   */ 
/*                                                                         */
/* Input: actual sample data, struct frameStart                            */
/* Output: struct frameStart											   */
/*-------------------------------------------------------------------------*/  

inline void detectFrameStart(long data, FrameStartDataType *frameStart)
{
  if ((!data) & frameStart->oldVal) frameStart->isFrameStart = 1;
  frameStart->oldVal = data;
} 


/*-------------------------------------------------------------------------*/               
/* void initFrameStart(FrameStartDataType *frameStart)			 		   */
/*																		   */
/* Initializes the structure frameStart  								   */
/*                                                                         */
/* Input: struct frameStart 					                           */
/* Output: struct frameStart											   */
/*-------------------------------------------------------------------------*/ 

inline void initFrameStart(FrameStartDataType *frameStart)
{
  frameStart->oldVal = 0;
  frameStart->isFrameStart = 0;
}  
 

/*-------------------------------------------------------------------------*/               
/* void measureLong(long data, MeasureDataType *measure)			 	   */
/*																		   */
/* Measures the pulse width of the actual pulse (eiter negativ or positiv) */
/*                                                                         */
/* Input: binary signal data, struct measure						       */
/* Output: struct measure  												   */
/*-------------------------------------------------------------------------*/ 
 
static void measureLong(long data, MeasureDataType *measure)                          
{
  if (data & measure->posOldVal) {   // detect positive edges 
    measure->isPos = 1;
    measure->isNeg = 0;
    measure->negLength = 0;
  }
  else if ((!data) & measure->negOldVal) {  // detect negative edges 
    measure->isPos = 0;
    measure->isNeg = 1;
    measure->posLength = 0;
  }
  measure->posOldVal = !data;
  measure->negOldVal = data;
  
  if (measure->isPos) measure->posLength++;        // measure positive pulses 
  else if (measure->isNeg) measure->negLength++;   // measure negative pulses 
}; 
 

/*-------------------------------------------------------------------------*/               
/* void measureShort(long data, MeasureShortDataType *measure)	 		   */
/*																		   */
/* Measures the pulse width of the actual pulse (eiter negativ or positiv) */
/* and stores the shortest length               	                       */ 
/*                                                                         */
/* Input: binary signal data, struct measure						       */
/* Output: struct measure  												   */
/*-------------------------------------------------------------------------*/

static void measureShort(long data, MeasureShortDataType *measure)
{
  if (data != measure->oldVal) {  // detect edge on data signal
    if ((measure->actualLength < measure->shortLength) && (measure->edgeCount>MIN_EDGE)) measure->shortLength = measure->actualLength;
    measure->actualLength = 0;
    measure->edgeCount++;
  };
  measure->oldVal = data;
  measure->actualLength++;
};  
 
 
/*-------------------------------------------------------------------------*/               
/* void initMeasure(MeasureDataType *measure)					 		   */
/*																		   */
/* Initializes the structure measure used in the function measureLong  	   */
/*                                                                         */
/* Input: struct measure											       */
/* Output: struct measure  												   */
/*-------------------------------------------------------------------------*/

inline void initMeasure(MeasureDataType *measure)                  
{
  measure->posLength = 0;
  measure->negLength = 0;
  measure->posOldVal = 0;
  measure->negOldVal = 0;
  measure->isPos = 0;
  measure->isNeg = 0;
};       
  

/*-------------------------------------------------------------------------*/               
/* void initMeasureShort(MeasureDataType *measure)				 		   */
/*																		   */
/* Initializes the structure measure used in the function measureShort 	   */
/*                                                                         */
/* Input: struct measure											       */
/* Output: struct measure  												   */
/*-------------------------------------------------------------------------*/
  
inline void initMeasureShort(MeasureShortDataType *measure)
{

  measure->oldVal = 0;
  measure->actualLength = INIT_LENGTH;
  measure->shortLength = INIT_LENGTH;
  measure->edgeCount = 0; 
  
};


/*-------------------------------------------------------------------------*/               
/* void initTxParameter(TxParameterType *param) 				 		   */
/*																		   */
/* Initializes the parameters used by the function txDataProcessing 	   */
/*                                                                         */
/* Input: struct param							     				       */
/* Output: struct param 												   */
/*-------------------------------------------------------------------------*/

inline void initTxParameter(TxParameterType *param)
{
  param->sampleCount = 0;
  param->bitPos = 0;
  param->txWord = INIT_WORD;
  
}


/*-------------------------------------------------------------------------*/               
/* void pilotTransmitter()										           */
/*																		   */
/* Sends data words (txBuffer) and MWire. If edges (either negativ or      */
/* positiv) are detected in the MWire signal data transmission is inter-   */ 
/* rupted immediately. In case of rtsPilotCh1 beeing zero the pilot signal */ 
/* has to be interrupted (e.g. because of protection commands).			   */
/* Data to be sent (txBuffer, MWire) is processed and sampled. 			   */
/* The generated samples are stored in txSample and serve as input signals */ 
/* of the pilotModulator. Output arrays of the structure txSample are of   */
/* length BLOCK_LENGTH.     											   */
/*                                                                         */
/* Input: data word txBuffer, signalling data MWire, rtsPilotCh1           */
/* Output: array of output sample							  			   */
/*-------------------------------------------------------------------------*/

void pilotTransmitter(PilotModDataType *pilotMod, float *outSample, int blockLength)
{
  // evaluate M-Wire signal and adjust pulse length (min pulse length = MIN_PW_OF_MW)
  if (pilotMod->minPwCounter > 0) {
    pilotMod->minPwCounter--;                   						// decrement puls width counter
    pilotMod->MWirePWFilterOut = pilotMod->MWirePWFilterOld;  			// take old output sample to garantee minimal puls width
  }
  else {
    pilotMod->MWirePWFilterIn = pilotMod->MWire; 						// sample new MWire input
    if (pilotMod->MWirePWFilterIn != pilotMod->MWirePWFilterOld) {  	// detect edge on MWire
      pilotMod->edgeDetected = DATA_DELAY_TRAN;  						// set delay of data delay after last pulse on MWire
      pilotMod->minPwCounter = MIN_PW_OF_MW;     						// set minimal puls width of MWire signal
    }  
    pilotMod->MWirePWFilterOld = pilotMod->MWirePWFilterIn;          	// store old value
    pilotMod->MWirePWFilterOut = pilotMod->MWirePWFilterIn;          	// new output sample
  }
  
  // detect pilot recovery and set syncCounter
  // sync frame is sent during this time
  // telephone signalling is interrupted
   
  if (pilotMod->rts == pilotMod->rtsOld) {
	  pilotMod->syncCounter = SYNC_DELAY;	// set syncCounter after pilot failure
  }
  else if (pilotMod->syncCounter > 0) {
	  pilotMod->syncCounter--;
  }
  else {
    // generate tx MWire samples		
	  if (pilotMod->MWireDelayLine[pilotMod->delayInd] == 0) { // get delayed M-Wire Sample
		  pilotMod->txSample.MWire[0] = -1.0;   
		  pilotMod->txSample.MWire[1] = -1.0;
	  }
	  else {
		  pilotMod->txSample.MWire[0] = 1.0;   
		  pilotMod->txSample.MWire[1] = 1.0;
	  }
  }
  
  pilotMod->rtsOld = !pilotMod->rts;
  
  // delay MWire samples
  pilotMod->MWireDelayLine[pilotMod->delayInd] = pilotMod->MWirePWFilterOut;  // input of M-Wire delay line
  pilotMod->delayInd++;                                   				// increment delay line index
  pilotMod->delayInd %= MWIRE_DELAYLINE_LENGTH;           				// delay line is a circular buffer
  
  // generate tx data samples                                                                         
  if ((pilotMod->edgeDetected >0) && !(pilotMod->syncCounter)) {    	// only M-Wire signal is transmitted

    pilotMod->edgeDetected--;                 							// decrement delay (delay of data after last pulse on MWire)
    
    pilotMod->dataBreak = TRUE;         									// set dataBreak flag in case of signalling on M-Wire
    initTxParameter(&pilotMod->txParameter);
            
    pilotMod->txSample.data[0] = pilotMod->lastDataSample;				// no data is sent
    pilotMod->txSample.data[1] = pilotMod->lastDataSample;    
  }
  else {
    pilotMod->dataBreak = FALSE; 										// clear data break flag
    pilotMod->lastDataSample = pilotMod->txSample.data[1];              // save last data sample
  }
  
  // modulate tx samples
  if (pilotMod->rts && !(pilotMod->txInitCounter)) {

	  pilotModulator(pilotMod, pilotMod->txSample.data, pilotMod->txSample.MWire, outSample, blockLength);

	  // scale ouput signal
	  outSample[0] *= pilotMod->txScalingCoef;
	  outSample[1] *= pilotMod->txScalingCoef;
  }
  else {
    pilotMod->dataBreak = TRUE;
    initTxParameter(&pilotMod->txParameter);
    outSample[0] = 0.0;
	outSample[1] = 0.0;
    pilotMod->edgeDetected = 0;
	if (pilotMod->txInitCounter) {
		pilotMod->txInitCounter--; 
	}
  }
}


/*-------------------------------------------------------------------------*/               
/* void txDataProcessing(EocDataBufferType *txBuffer,                	   */
/*						 PilotModDataType *pilotMod)              		   */
/*																		   */
/* Data words to be sent (txBuffer) are processed and sampled. If no data  */
/* is available dummy words are sent instead. The generated samples are    */ 
/* stored in txSample and serve as input signals of the pilotModulator.    */
/* Output arrays of the structure txSample are of length BLOCK_LENGTH.     */
/*                                                                         */
/* Input: data word txBuffer, parameters pilotMod       		           */
/* Output: array of output sample							  			   */
/*-------------------------------------------------------------------------*/

void txDataProcessing(EocDataBufferType *txBuffer, PilotModDataType *pilotMod)
{ 
  //static int ind;
  
  // process data word to be sent
  if (++pilotMod->txParameter.bitPos == WORD_LENGTH) {
  
    pilotMod->txParameter.bitPos = 0;

    if (txBuffer->full) {
    
      pilotMod->txParameter.txWord = txBuffer->data;
      txBuffer->full = FALSE;
      pilotMod->outOfData = FALSE;
      }    
    else { 
    
      pilotMod->txParameter.txWord = INIT_WORD;
      pilotMod->outOfData = TRUE;
    }  
  }

  // update data break flag
  txBuffer->dataBreak = pilotMod->dataBreak;
    
  pilotMod->txSample.data[0] = (float)((pilotMod->txParameter.txWord >> pilotMod->txParameter.bitPos) & BIT0);
  if (pilotMod->txSample.data[0] == 0.0) pilotMod->txSample.data[0] = -1.0;
  pilotMod->txSample.data[1] = pilotMod->txSample.data[0];

}


/*-------------------------------------------------------------------------*/               
/* void demodFreqControl(PilotDemodDataType *pilotDemod,                   */
/*						 GuardDataType *gStatus)						   */
/*																		   */
/* The frequency control voltage uf is updated according to the 		   */
/* demodulated in phase and quadrature component using PI-controller loop. */
/* Using a base band oscillator and phase shifter the frequncy and phase   */
/* of the demodulated signal are adjusted.								   */
/*																		   */
/* Input: PilotDemodDataType *pilotDemod, GuardDataType *gStatus 		   */
/* Output: adjusted in phase and quadrature samples  					   */
/*-------------------------------------------------------------------------*/

void demodFreqControl(PilotDemodDataType *pilotDemod, GuardDataType *gStatus)
{
  register float dPhi, dw, in, quad, quadI, quadQ, dif, mult;
  register float bbSine, bbCosine;
  
  in = pilotDemod->shiftedI*gStatus->rxGain*pilotDemod->rxLevelAtt;
  quad = pilotDemod->shiftedQ*gStatus->rxGain*pilotDemod->rxLevelAtt;
  
  // phase detector
  quadI = sqr(in);
  quadQ = sqr(quad);
  
  dif = quadQ - quadI;
  mult = in*quad;
  
  dPhi = mult*dif;
                   
  // PI-Controller
  pilotDemod->stateI = pilotDemod->stateI + KI*dPhi;
  pilotDemod->uf = KP*dPhi + pilotDemod->stateI;    
    
  if (pilotDemod->stateI > INTEGRATOR_MAX) initDemodFreqControl(pilotDemod);
  else if (pilotDemod->stateI < -INTEGRATOR_MAX) initDemodFreqControl(pilotDemod);
  
  // basis band VCO 
  dw = KF*pilotDemod->uf;       
  
  // state update of basis band VCO
  bbCosine = pilotDemod->x1 - dw*pilotDemod->x2;
  bbSine = pilotDemod->x2 + dw*pilotDemod->x1;
  
  pilotDemod->x1 = bbCosine;
  pilotDemod->x2 = bbSine;
 
}

 	
/*-------------------------------------------------------------------------*/               
/* void detectDataWord(long input, PilotRecDataType *pilotRec,             */ 
/*                     EocDataBufferType *rxBuffer) 					   */
/*																		   */
/* Detects data bits by sampling the demodualted data channel and stores   */
/* eleven of them as one word in the receive buffer rxBuffer.			   */
/*                                                                         */
/* Input: data channel input, parameters pilotRec						   */
/* Output: receive buffer rxBufffer							  			   */
/*-------------------------------------------------------------------------*/

static void detectDataWord(long input, PilotRecDataType *pilotRec, EocDataBufferType *rxBuffer)
{ 
  if (++pilotRec->ddParam.sampleCount == SAMPLES_PER_REC_BIT) {
  
    pilotRec->ddParam.sampleCount = 0;
    pilotRec->ddParam.word |= ((input) << pilotRec->ddParam.bitPos++);	// data word is regenerated by joining received data bits
    
    if (pilotRec->ddParam.bitPos == WORD_LENGTH) {            			// data word is received
     
      rxBuffer->data = pilotRec->ddParam.word;
      if (rxBuffer->full) pilotRec->rxBufferOverrun = 1;
      rxBuffer->full = 1;

      pilotRec->ddParam.word = 0;
      pilotRec->ddParam.bitPos = 0;
                                
    }
  } 
} 


/*-------------------------------------------------------------------------*/               
/* void initDetectData(DetectDataType *detectDataParameter)		 		   */
/*																		   */
/* Initializes the parameters used by the function detectDataWord   	   */
/*                                                                         */
/* Input: struct detectDataParameters  	   							       */
/* Output: struct detectDataParameters									   */
/*-------------------------------------------------------------------------*/

inline void initDetectData(DetectDataType *detectDataParameter)
{
  detectDataParameter->sampleCount = DATA_BIT_SAMPLE_OFFSET;  // offset to adjust sample time
  detectDataParameter->bitPos = 0;
  detectDataParameter->word = 0; 
}                    

            
/*-------------------------------------------------------------------------*/               
/* void pilotReceiver(EocDataBufferType *rxBuffer, PilotDemodDataType 	   */
/*     *pilotDemod, PilotRecDataType *pilotRec, GuardDataType *gStatus)    */
/*																		   */
/* Controls the processes needed for channel identification, monitoring and*/
/* data detection. The switch siganls are generated in the the function    */
/* pilotReceiverController() using a state machine.                        */
/*                                                                         */
/* Input: channel pointers of demodulated quadrature and inphase component */
/*		  process switch signals PilotRecDataType *pilotRec  	           */
/*		  guard status GuardDataType *gStatus							   */
/* Output: received data word rxBuffer  								   */
/*-------------------------------------------------------------------------*/

void pilotReceiver(EocDataBufferType *rxBuffer, PilotDemodDataType *pilotDemod, PilotRecDataType *pilotRec, GuardDataType *gStatus)
{
  register float xi, xq; 
  register long sampleI, sampleQ;
    
  // select channel  
  xi = pilotDemod->sgnData*(*pilotDemod->dataChannelPtr);
  xq = pilotDemod->sgnSig*(*pilotDemod->sigChannelPtr); 
  
  // binarisation 
  if (xi > 0.0) sampleI = 1;
  else sampleI = 0.0;
    
  if (xq > 0.0) sampleQ = 1.0;
  else sampleQ = 0.0;
 
  // test function switch 
  if (pilotRec->measureShortDataIsOn) measureShort(sampleI, &pilotRec->dataChannel);
  if (pilotRec->measureShortSigIsOn)  measureShort(sampleQ, &pilotRec->sigChannel);
  if (pilotRec->measureLongIsOn)	  measureLong(sampleI, &pilotRec->dataChannelLong);
  if (pilotRec->measureLongSigIsOn)   measureLong(sampleQ, &pilotRec->sigChannelLong);
  if (pilotRec->detectFrameStartIsOn) detectFrameStart(sampleI, &pilotRec->frameStart);
    
  // data detection is activated asynchronuosly just after a framestart was detected
  if (pilotRec->frameStart.isFrameStart && (!pilotRec->detectDataIsOn)) {
    initDetectData(&pilotRec->ddParam);  
    pilotRec->detectDataIsOn = 1;
  } 
  
  // detect data bits and regenerate data words  
  if (pilotRec->detectDataIsOn) detectDataWord(sampleI, pilotRec, rxBuffer);    
  
  // EWire Switch used in function generateEWire()
  pilotDemod->EWireOn = pilotDemod->channelsAreId && gStatus->guardIsPresent && !gStatus->fastMute && !gStatus->snrAlarm;// && (!EWireOffCounter); // && gStatus->pilotIsOk 
                               
}
  
 
/*-------------------------------------------------------------------------*/               
/* long generateEWire(PilotDemodDataType *pilotDemod)	                   */
/*																		   */
/* Generates the output signal EWire depending on the switch EWireOn. If   */
/* EWireOn= 1 the demodulated MWire signal is converted to a binary signal */
/* (EWire) an returned. Else zero or the last valid value is returned.     */
/*                                                                         */
/* Input: pointer to demodulated MWire signal demodPtr[1]			       */
/* Output: binary signal EWire											   */
/*-------------------------------------------------------------------------*/

long generateEWire(PilotDemodDataType *pilotDemod)  // NF sample time
{ 

  // phase shifter
  pilotDemod->shiftedI = pilotDemod->filteredI*pilotDemod->x1 + pilotDemod->filteredQ*pilotDemod->x2;
  pilotDemod->shiftedQ = pilotDemod->filteredQ*pilotDemod->x1 - pilotDemod->filteredI*pilotDemod->x2;
  
  // evaluate E-Wire signal
  if (pilotDemod->sgnSig*(*pilotDemod->sigChannelPtr) > 0.0) pilotDemod->EWire = EWIRE_ACTIVE;
  else pilotDemod->EWire = EWIRE_INACTIVE;
  
  // E-Wire switch
  if (pilotDemod->EWireOn) {
    pilotDemod->EWireOld = pilotDemod->EWire;        	// save old value
    pilotDemod->EWireHoldCounter = EWIRE_HOLD_TIME;
  }      
  else if (pilotDemod->EWireHoldCounter > 0) {
    pilotDemod->EWire = pilotDemod->EWireOld;       	// return old value 
    pilotDemod->EWireHoldCounter--;
  }
  else pilotDemod->EWire = EWIRE_INACTIVE;
  
  return(pilotDemod->EWire); 
  
}


#pragma optimize_off

/*-------------------------------------------------------------------------*/               
/* void pilotReceiverController(PilotDemodDataType *pilotDemod,            */ 
/*                  PilotRecDataType *pilotRec, GuardDataType *gStatus)    */
/*																		   */
/* Calculates the next state of the pilot receiver controller. The switch  */
/* signals needed to activate processes in the function pilotReceiver()    */
/* are generated depending on the actual state.  				           */
/*                                                                         */
/* Input: PilotDemodDataType *pilot, GuardDataType *gStatus     	       */
/* Output: switch signals PilotRecDataType *pilotRec					   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE pilotReceiverController(PilotDemodDataType *pilotDemod, PilotRecDataType *pilotRec, GuardDataType *gStatus)
{
  
  // calculate next state
  
  // STATE PILOT FAILURE
  //  initial state after power up and pilot failure
  //  wait until initCouner = 0 and pilotIsOk = 1
  
  if (pilotRec->state == pilotFailure) {
    
    if (pilotRec->initCounter > 0) {
      pilotRec->initCounter--;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = pilotFailure;
    }  
    else if (gStatus->pilotIsOk) {
      initMeasureShort(&pilotRec->dataChannel);
      initMeasureShort(&pilotRec->sigChannel);
      initMeasure(&pilotRec->dataChannelLong); 
      initMeasure(&pilotRec->sigChannelLong);
      initFrameStart(&pilotRec->frameStart);
      pilotRec->detectFrameStartIsOn = 0;
      pilotRec->detectDataIsOn = 0;
      pilotRec->measureLongSigIsOn = 0;
      pilotRec->measureShortDataIsOn = 1;
      pilotRec->measureShortSigIsOn = 1;
      pilotRec->state = identifyingChannels;
    }
    else pilotRec->state = pilotFailure;
    
  }
  
  
  // STATE IDENTIFYING CHANNELS
  //  identifies data and signalling channel based on pulse length measurements
  //  short pulses are characteristic for the data channel (10 ms)
  //  min. pulse width on the signalling channel is 16,6 ms (60 baud)
  //  if short pulses are detected on the siganlling channel a phase shift of 90 deg is performed
    
  else if (pilotRec->state == identifyingChannels) {
  
    if (pilotRec->dataChannel.shortLength < SHORT_PULSE_MIN) {  // short pulse, probably caused by noise
      initMeasureShort(&pilotRec->dataChannel); 
      pilotRec->state = identifyingChannels;
    }
    else if (pilotRec->dataChannel.shortLength < SHORT_PULSE) {
      initMeasureShort(&pilotRec->dataChannel);
      pilotRec->measureShortDataIsOn = 0;
      pilotRec->measureLongIsOn = 1;
      pilotRec->measureLongSigIsOn = 1;
      pilotRec->state = receivingData;
    }
    else if (pilotRec->sigChannel.shortLength < SHORT_PULSE) {
      phaseShift90(pilotDemod);
      initMeasureShort(&pilotRec->dataChannel);
      initMeasureShort(&pilotRec->sigChannel);
      pilotRec->state = identifyingChannels;
    }
    else if (!gStatus->pilotIsOk) { 
      pilotRec->measureShortDataIsOn = 0;
      pilotRec->measureShortSigIsOn = 0;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = pilotFailure; 
    }
    else pilotRec->state = identifyingChannels;
    
  }
  
  
  // STATE RECEIVING DATA
  //  detects breaks between data frames on the data channel
  //  wrong sign of data is corrected by a 180 deg phase shift
  //  in case of signalling data detection is interrupted
    
  else if (pilotRec->state == receivingData) {
  
    if (pilotRec->dataChannelLong.posLength > LONG_PULSE) {
      initMeasure(&pilotRec->dataChannelLong);
      pilotRec->measureLongIsOn = 0;
      pilotRec->detectDataIsOn = 0;
      pilotRec->detectFrameStartIsOn = 1;
      pilotDemod->channelsAreId = 1;
      pilotRec->state = waitingForFrameStart;
    }
    else if (pilotRec->dataChannelLong.negLength > LONG_PULSE) {
      phaseShift180(pilotDemod);
      initMeasure(&pilotRec->dataChannelLong);
      initMeasure(&pilotRec->sigChannelLong);
      initMeasureShort(&pilotRec->sigChannel);
      pilotRec->measureLongIsOn = 0;
      pilotRec->detectDataIsOn = 0;
      pilotRec->detectFrameStartIsOn = 1;
      pilotDemod->channelsAreId = 1;
      pilotRec->state = waitingForFrameStart; 
    }
    else if (pilotRec->sigChannel.shortLength < SHORT_PULSE) {
      initMeasureShort(&pilotRec->sigChannel);
      pilotRec->measureLongIsOn = 0;
      pilotRec->measureLongSigIsOn = 0;
      pilotRec->detectDataIsOn = 0; 
      pilotRec->measureShortDataIsOn = 1;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = identifyingChannels; 
    } 
    else if (((pilotRec->sigChannelLong.posLength > SHORT_PULSE) ||
    		  (pilotRec->sigChannelLong.negLength > SHORT_PULSE)) && pilotDemod->channelsAreId) {
      initMeasure(&pilotRec->sigChannelLong);
      pilotRec->isSigImp = DATA_DELAY_REC;
      pilotRec->detectDataIsOn = 0;
      pilotRec->measureLongIsOn = 0;
      pilotRec->state = receivingEWire;
    }
    else if ((pilotRec->sigChannelLong.posLength > SHORT_PULSE) ||
    		 (pilotRec->sigChannelLong.negLength > SHORT_PULSE)){
      initMeasure(&pilotRec->sigChannelLong);
      initMeasure(&pilotRec->dataChannelLong);
      pilotRec->detectDataIsOn = 0;
      pilotRec->measureLongIsOn = 1;
      pilotRec->state = receivingData;
    }
    else if (!gStatus->pilotIsOk) { 
      pilotRec->measureShortSigIsOn = 0;
      pilotRec->measureLongIsOn = 0;
      pilotRec->detectDataIsOn = 0;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = pilotFailure; 
    }
    else pilotRec->state = receivingData;
    
  }
  
  
  // STATE WAITING FOR FRAMESTART
  //  detects the beginning of a new data frame and activates data detection
   
  else if (pilotRec->state == waitingForFrameStart) { 
  
    if (pilotRec->frameStart.isFrameStart) {
      initFrameStart(&pilotRec->frameStart);
      pilotRec->detectFrameStartIsOn = 0;
      pilotRec->measureLongIsOn = 1;
      pilotRec->state = receivingData;
    }
    else if (pilotRec->sigChannel.shortLength < SHORT_PULSE) {
      initMeasureShort(&pilotRec->sigChannel);
      pilotRec->detectFrameStartIsOn = 0;
      pilotRec->measureShortDataIsOn = 1;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = identifyingChannels;
    } 
    else if ((pilotRec->sigChannelLong.posLength > SHORT_PULSE) ||
    		 (pilotRec->sigChannelLong.negLength > SHORT_PULSE)) {
      initMeasure(&pilotRec->sigChannelLong);
      pilotRec->isSigImp = DATA_DELAY_REC;
      pilotRec->detectFrameStartIsOn = 0;
      pilotRec->state = receivingEWire;
    } 
    else if (!gStatus->pilotIsOk) {
      pilotRec->detectFrameStartIsOn = 0;
      pilotRec->measureShortSigIsOn = 0;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = pilotFailure;
    }
    else pilotRec->state = waitingForFrameStart;
     
  }
  
  
  // STATE RECEIVING E-WIRE
  //  data detection is interrupted because of telephone signalling
  //  state is held for DATA_DELAY_REC after last signalling pulse
    
  else if (pilotRec->state == receivingEWire) { 
  
    if (pilotRec->isSigImp > 0) pilotRec->isSigImp--;
    
    if (!gStatus->pilotIsOk) {
      pilotRec->measureLongSigIsOn = 0;
      pilotRec->measureShortSigIsOn = 0;
      pilotDemod->channelsAreId = 0;
      pilotRec->state = pilotFailure;
    }
    else if (pilotRec->sigChannel.shortLength < SHORT_PULSE) {
      pilotDemod->channelsAreId = 0;
      pilotRec->initCounter = 0;
      pilotRec->state = pilotFailure;  
    }    
    else if (pilotRec->sigChannelLong.posLength > SHORT_PULSE) {
      initMeasure(&pilotRec->sigChannelLong);
      pilotRec->isSigImp = DATA_DELAY_REC;
      pilotRec->state = receivingEWire; 
    }
    else if (pilotRec->sigChannelLong.negLength > SHORT_PULSE) {
      initMeasure(&pilotRec->sigChannelLong);
      pilotRec->isSigImp = DATA_DELAY_REC;
      pilotRec->state = receivingEWire; 
    }    
    else if (!pilotRec->isSigImp) {
      initMeasure(&pilotRec->dataChannelLong);
      initMeasure(&pilotRec->sigChannelLong);
      pilotRec->measureLongIsOn = 1;
      pilotRec->state = receivingData;
    }
    else pilotRec->state = receivingEWire; 
    
  }
 
  // set initial state  
  else { 
  
    pilotRec->state = pilotFailure;
    pilotDemod->channelsAreId = 0;
    
  }                                      
}


/*-------------------------------------------------------------------------*/               
/* void rxGuard(GuardDataType *gStatus, PilotDemodDataType *pilot,         */
/*				PCStatusData *PCStatus)									   */
/*																		   */
/* Receives the demodulated samples of the pilot signal, caluculates the   */
/* mean and variance of both I & Q component and generates the  		   */
/* signals gStatus.guardIsPresent and gStatus.fastMute                	   */
/*                                                                         */
/* Input: structure pilot (demodulated samples)							   */
/* Output: guard status gStatus, PCStatus					  			   */
/*-------------------------------------------------------------------------*/
                        
void SEGMENT_EXT_CODE rxGuard(GuardDataType *gStatus, PilotDemodDataType *pilot, t_PilotModemStatus *pilotStatus)
{
  register long lowLevelValid, highLevelValid;
  register float varI, varQ;

  // get absolute value of new sample 
  gStatus->inMag =  fabsf(pilot->shiftedI);
  gStatus->quadMag  = fabsf(pilot->shiftedQ);

  if (gStatus->thValid) {                  // get old values if threshold valid
    gStatus->meanI = gStatus->meanIHold;
    gStatus->meanQ = gStatus->meanQHold;
    gStatus->inNoiseLevel = gStatus->inNoiseHold;
    gStatus->quadNoiseLevel = gStatus->quadNoiseHold;
  }
  
  // calculate mean of I and Q component  
  gStatus->meanI = GL_COEF_1*gStatus->meanI + GL_COEF_2*gStatus->inMag;   
  gStatus->meanQ = GL_COEF_1*gStatus->meanQ + GL_COEF_2*gStatus->quadMag; 
  
  // calculate variance of I and Q component  
  varI = gStatus->meanI- gStatus->inMag;
  varQ = gStatus->meanQ- gStatus->quadMag;
  
  gStatus->inNoiseLevel =  SNR_COEF_1*gStatus->inNoiseLevel + SNR_COEF_2*sqr(varI);
  gStatus->quadNoiseLevel =  SNR_COEF_1*gStatus->quadNoiseLevel + SNR_COEF_2*sqr(varQ); 
    
  if (gStatus->lpValid) {                 // save new values if low pass valid
    gStatus->meanIHold = gStatus->meanI;
    gStatus->meanQHold = gStatus->meanQ;
    gStatus->inNoiseHold = gStatus->inNoiseLevel;
    gStatus->quadNoiseHold = gStatus->quadNoiseLevel;
  }

  // in case of a pilot failure both channnels (in-phase, quadrature) are interrupted 
  lowLevelValid = ((gStatus->inMag < gStatus->guardDecisionLevelLow) && (gStatus->quadMag < gStatus->guardDecisionLevelLow));
  highLevelValid = ((gStatus->inMag > gStatus->guardDecisionLevelHigh) || (gStatus->quadMag > gStatus->guardDecisionLevelHigh));

  // generate guardIsPresent
  if (lowLevelValid) gStatus->timerGuardIsPresent = GUARD_IS_PRESENT_DELAY;
  else if (gStatus->timerGuardIsPresent > 0) gStatus->timerGuardIsPresent--;
  
  gStatus->guardIsPresent = !gStatus->timerGuardIsPresent;
  
  // generate fastMute prolongation 
  if (highLevelValid) gStatus->fastMute = FAST_MUTE_PROLONGATION;
  else if (gStatus->fastMute > 0) gStatus->fastMute--;
  
  // update pilotStatus FAST_MUTE
  if (gStatus->fastMute) pilotStatus->general |=  FAST_MUTE_SET;
  else pilotStatus->general &= FAST_MUTE_CLEAR;
  
}



/*-------------------------------------------------------------------------*/
/* void evaluateGuard()													   */
/*																		   */
/* Monitors the received guard signal and generates alarms stored in the   */
/* structre guardStatus:		   										   */
/* 								   										   */
/* - guardIsPresent: received guard level is sufficient                    */
/* - pilotIsOk: guard level and SNR allow data receiving                   */
/* - guardLevelAlarm: received guard level is to low                       */
/* - snrAlarm: SNR is to low                                               */
/*                                                                         */
/* Input: samples of demodulated pilot signal gStatus->inMag,              */
/* 		  gStatus->quadMag							   					   */
/* Output: guard status gStatus								  			   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE evaluateGuard(GuardDataType *gStatus, GuardConfigType *gConfig) 
{  
  register long temp;
  register float levelThreshold;
   
  // calculate guard decision levels and generate alarms
  register float  maxLow, maxLowHold;
       
  // calculate  noise level and snr
  register float snr;
    
  // guard control process
  register long guardIsPresent, oldGuardIsPresent;
      
     
  // calculate guard decision levels and generate alarms
  
  // -Hold variables are frozen during a sudden guard level decreas (either pilot fail or line attenuation
  maxLowHold = (gStatus->meanIHold > gStatus->meanQHold) ? gStatus->meanIHold:gStatus->meanQHold;
  gStatus->guardDecisionLevelLow = GUARD_THRESHOLD_LOW*maxLowHold; 
    
  // variables used for calculation of guardDecisionLevelHigh are continuously updated
  maxLow = (gStatus->meanI > gStatus->meanQ) ? gStatus->meanI:gStatus->meanQ;
  gStatus->guardDecisionLevelHigh = GUARD_THRESHOLD_HIGH*maxLow;

  levelThreshold = gConfig->levelLowTh;

  // guard level alarm with hysteresis used for AGC and AFC blocking
  if ((temp = (maxLow < levelThreshold)) == (maxLow < levelThreshold*GUARD_LEVEL_HYSTERESIS)) {
    gStatus->guardLevelAlarm = temp;
  }

  levelThreshold = gConfig->noGuardLevelTh;

  // no guard alarm used to reset pilotIsOk
  if ((temp = (maxLow < levelThreshold)) == (maxLow < levelThreshold*GUARD_LEVEL_HYSTERESIS)) {
    gStatus->noGuardAlarm = temp;
  }
    
  // calculate rxLevel
  gStatus->mean = (gStatus->meanI > gStatus->meanQ) ? gStatus->meanI:gStatus->meanQ;
  gStatus->meanHold = (gStatus->meanIHold > gStatus->meanQHold) ? gStatus->meanIHold:gStatus->meanQHold;
    
  gStatus->rxLevel = sqr(gStatus->mean);    

   
  // calculate  noise level and snr
    
  gStatus->noiseLevel = (gStatus->inNoiseHold < gStatus->quadNoiseHold) ? gStatus->inNoiseHold:gStatus->quadNoiseHold;

  // calculate snr
  if (gStatus->noiseLevel > 0.0) {
    snr = gStatus->rxLevel/gStatus->noiseLevel;
    gStatus->snr = SNR_COEF_1*gStatus->snr + SNR_COEF_2*snr;
  }
    
  // snr alarm with hysteresis 
  if ((temp = (SNR_THRESHOLD_HIGH > gStatus->snr)) == (SNR_THRESHOLD_LOW > gStatus->snr)) {
    gStatus->snrAlarm = temp;
  } 
 

  // guard control process

  guardIsPresent = gStatus->guardIsPresent;
  oldGuardIsPresent = gStatus->oldGuardIsPresent;
  
  if (guardIsPresent) gStatus->sigOkDelay = SIG_OK_DELAY; 
  else if (gStatus->sigOkDelay > 0) gStatus->sigOkDelay--;

  // pilotIsOk delay to avoid oscillations 
  if (guardIsPresent) gStatus->pilotCount1 = PILOT_FALSE_DELAY;
  else if (gStatus->pilotCount1 > 0) gStatus->pilotCount1--;
  
  if (!guardIsPresent) gStatus->pilotCount2 = PILOT_OK_DELAY;  
  else if (gStatus->pilotCount2 > 0) gStatus->pilotCount2--;
  
  // set pilotIsOk
  if (!gStatus->pilotCount1 || gStatus->snrAlarm || gStatus->noGuardAlarm) gStatus->pilotIsOk = 0;
  if (!gStatus->pilotCount2 && !gStatus->snrAlarm && !gStatus->noGuardAlarm) gStatus->pilotIsOk = 1;
 
  // guard state machine: guard estimates may be unfreezed in case of guardIsPresent = true 
  gStatus->lpValid = ((guardIsPresent && oldGuardIsPresent) || !(guardIsPresent || oldGuardIsPresent || gStatus->sigOkDelay));
  gStatus->thValid = (guardIsPresent && !oldGuardIsPresent);  
  
  // save old value 
  gStatus->oldGuardIsPresent = gStatus->guardIsPresent;
  
}


/*-------------------------------------------------------------------------*/               
/*void gainControl(GuardDataType *gStatus, agcDataType *agcData, 		   */
/*				   long blockAGC)                                          */
/*																		   */
/* Calculates the receiver gain to keep the rx level constant by 		   */
/* evaluating the pilot signal rx level.								   */
/*                                                                         */
/* Input: guard status, agcData, blockAGC   							   */
/* Output: rx gain														   */
/*-------------------------------------------------------------------------*/

void SEGMENT_EXT_CODE gainControl(GuardDataType *gStatus, agcDataType *agcData, long blockAGC)
{
	register float rxGain;
	
	// calculate rxGain
    if (gStatus->meanHold > 0.0) rxGain = agcData->rxLevelDesired/gStatus->meanHold;
    else rxGain = RX_GAIN_MIN;
    
    // error handling
    if (rxGain > RX_GAIN_MAX) {
      rxGain = RX_GAIN_MAX;
      agcData->ok = 0;
    }   
    else if (rxGain < RX_GAIN_MIN) {
      rxGain = RX_GAIN_MIN;
      agcData->ok = 0;
    }  
    else agcData->ok = 1; 
    
    // low pass filtering of rxGain (first stage) used in demodFreqControl()
    gStatus->rxGain = RXG_COEF_1*gStatus->rxGain + RXG_COEF_2*rxGain;
    
    if (gStatus->pilotIsOk && !gStatus->fastMute && !gStatus->guardLevelAlarm && !blockAGC) {
      // low pass filtering of rxGain (second stage)  
      agcData->rxGain = RXG_STATUS_COEF_1*agcData->rxGain + RXG_STATUS_COEF_2*gStatus->rxGain;
    }
    else agcData->ok = 0;    
}  		
                            

/*-------------------------------------------------------------------------*/               
/* void updatePilotStatus()												   */
/*                                                                         */
/* Updates state variable pilotStatus.		                               */
/*																		   */
/* Input: status variables of pilot functions 							   */
/* Output: pilotStatus												       */
/*-------------------------------------------------------------------------*/  
        
void SEGMENT_EXT_CODE updatePilotStatus(GuardConfigType *gConfig, GuardDataType *gStatus, PilotModDataType *pilotMod,
										PilotDemodDataType *pilotDemod, agcDataType *agcData, t_PilotModemStatus *pilotStatus)
{
  register BOOL temp;
  register float levelAlarmThreshold;
  

  // update status values
  pilotStatus->rxLevel = FTWO*gStatus->rxLevel;          // rx pilot level of pilot channel (I or Q-part of pilot signal contains only half of the signal power)
  pilotStatus->noiseLevel = FTWO*gStatus->noiseLevel;    // noise level of pilot channel
  pilotStatus->snr = gStatus->snr;
  pilotStatus->rxGain = agcData->rxGain;
  
  // update status flags
  // FAST_MUTE is set in rxGuard()
  // SLOW_MUTE is set in Monitor.c (Master DSP)
  // 
  if (pilotMod->rts) pilotStatus->general |= RTS_PILOT_SET; 
  else pilotStatus->general &= RTS_PILOT_CLEAR;

  if (gStatus->guardIsPresent) pilotStatus->general |= DCD_PILOT_SET;
  else pilotStatus->general &= DCD_PILOT_CLEAR;

  if(pilotDemod->channelsAreId) pilotStatus->general |= CHANNELSAREID_SET;
  else pilotStatus->general &= CHANNELSAREID_CLEAR;

  if (pilotMod->outOfData) pilotStatus->general |= OUTOFDATA_SET; 
  else pilotStatus->general &= OUTOFDATA_CLEAR;

  if (pilotMod->edgeDetected) pilotStatus->general |= REC_MW_SET;
  else pilotStatus->general &= REC_MW_CLEAR;

  if ((temp = (gStatus->snr < gConfig->snrAlarmTh)) ==
       (gStatus->snr < gConfig->snrAlarmTh*HYST_SNR_ALARM)) {
  
	if (temp) {

	  pilotStatus->general |=  SNR_ALARM_SET;
	}
    else {
		
	  pilotStatus->general &= SNR_ALARM_CLEAR;
	}
  }
  
  if (gStatus->pilotIsOk) pilotStatus->general |= RX_PILOT_OK_SET;
  else pilotStatus->general &= RX_PILOT_OK_CLEAR;

  levelAlarmThreshold = gConfig->levelAlarmTh;
  
  if ((temp = (gStatus->rxLevel < levelAlarmThreshold)) ==
      (gStatus->rxLevel < levelAlarmThreshold*HYST_LEVEL_ALARM)) {
      
    if (temp) {
		  
	    pilotStatus->general |=  PILOT_LEVEL_ALARM_SET;
	  }
	  else {
		  
	    pilotStatus->general &= PILOT_LEVEL_ALARM_CLEAR;
	  }
  }

  if (!agcData->ok) pilotStatus->general |= AGC_FAIL_SET;
  else pilotStatus->general &= AGC_FAIL_CLEAR;

}



/*------------------------TEST FUNCTIONS-----------------------------------*/

/*-------------------------------------------------------------------------*/
/* void sampleData(PilotDemodDataType *pilot)						       */
/*																		   */
/* Samples demodualted pilot signal (I and Q component) if activated in    */
/* the main loop (fs = 1 kHz). Used for debugging only.		 		   	   */
/*                                                                         */
/* Input: structure pilot (demodulated samples)							   */
/* Output: 													  			   */
/*-------------------------------------------------------------------------*/

/*void SEGMENT_EXT_CODE sampleData(PilotDemodDataType *pilot)
{

#define A_SIZE 512
#define SCALE  1.0

static long aCount;
static float SEGMENT_EXT_DATA inArray[A_SIZE];
static float SEGMENT_EXT_DATA quadArray[A_SIZE];
static float SEGMENT_EXT_DATA tempArray[A_SIZE];  
  
   
	if (aCount == A_SIZE) aCount = 0;
	inArray[aCount] = pilot->sgnData*(*pilot->dataChannelPtr)*SCALE;  
	quadArray[aCount] = pilot->sgnSig*(*pilot->sigChannelPtr)*SCALE;
	tempArray[aCount] = pilot->uf; 
	aCount++;   
}
*/

/*-------------------------------------------------------------------------*/
/* void samplePilot(float *txPilot, int blockLength)				       */
/*																		   */
/* Samples modualted pilot signal if activated in the main loop 		   */
/* (fs = 10 kHz). Used for debugging only.								   */
/*                                                                         */
/* Input: ptr to sample array (txPilot)									   */
/*		  number of samples stored in the array (blockLength)			   */
/* Output: 													  			   */
/*-------------------------------------------------------------------------*/

/*void SEGMENT_EXT_CODE samplePilot(float *txPilot, int blockLength)
{

#define PILOT_BUFFER_SIZE	4096

int i;
static int sampleCount;
static float SEGMENT_EXT_DATA pilotArray[PILOT_BUFFER_SIZE];

	for (i = 0; i < blockLength; i++) {
	
		pilotArray[sampleCount + i] = *txPilot;
		txPilot++;
	};
	
	sampleCount += blockLength;
	if (sampleCount >= (PILOT_BUFFER_SIZE - blockLength)) sampleCount = 0;

};
*/

//#pragma optimize_off

/*-------------------------------------------------------------------------*/               
/* void sendTestData()													   */
/*																		   */
/* Sends a sequence of test data words.									   */
/* Used for testing only.												   */ 
/*                                                                         */
/* Input: -											       		           */
/* Output: new test data word (txBuffer)							  	   */
/*-------------------------------------------------------------------------*/

/*void SEGMENT_EXT_CODE sendTestData(EocDataBufferType *txBuffer, TxTestDataParamType *param)
{

	static long testDataArray[12] = {TEST_DATA_WORD3, TEST_DATA_WORD4, TEST_DATA_WORD5,
									 TEST_DATA_WORD3, TEST_DATA_WORD4, TEST_DATA_WORD5,
									 TEST_DATA_WORD3, TEST_DATA_WORD4, TEST_DATA_WORD5,
									 TEST_DATA_WORD3, TEST_DATA_WORD4, TEST_DATA_WORD5};
									 
//	static long testDataArray[12] = {TEST_DATA_INIT, TEST_DATA_INIT, TEST_DATA_INIT,
//									 TEST_DATA_INIT, TEST_DATA_INIT, TEST_DATA_INIT,
//									 TEST_DATA_INIT, TEST_DATA_INIT, TEST_DATA_INIT,
//									 TEST_DATA_INIT, TEST_DATA_INIT, TEST_DATA_INIT};
//									 
//	static int idleCount = 0, wordCount = 0, arrayCount = 0;
	

	if (txBuffer->full == 0) {

		if (param->arrayCount >= 10) {
			
			// send 4 init. words after sending 10 times the test array
			txBuffer->data = TEST_DATA_INIT;
			txBuffer->full = 1;
			param->idleCount++;
			if (param->idleCount >= 4) {
				param->idleCount = 0;
				param->arrayCount = 0;
				param->frameStart = 1;
			}
		}
		else if (param->frameStart == 1) {
			
			txBuffer->data = TEST_DATA_START;
			txBuffer->full = 1;
			param->frameStart = 0;
		}
		else {
			
			txBuffer->data = testDataArray[param->wordCount];
			txBuffer->full = 1;
			param->wordCount++;
			if (param->wordCount >= 12) {
				param->arrayCount++;
				param->wordCount = 0;
			}
		}
	}
}


void SEGMENT_EXT_CODE initTxTestDataParam(TxTestDataParamType *param)
{
	
	param->idleCount = 0;
	param->wordCount = 0;
	param->arrayCount = 0;
	param->frameStart = 1;
}
*/

/*-------------------------------------------------------------------------*/               
/* void receiveTestData()						             			   */
/*																		   */
/* Receives test data words.											   */
/* Used for testing only.												   */ 
/*                                                                         */
/* Input: -											       		           */
/* Output: new test data word (txBuffer)							  	   */
/*-------------------------------------------------------------------------*/

/*void SEGMENT_EXT_CODE receiveTestData(EocDataBufferType *rxBuffer)
{
#define RCV_TEST_ARRAY_SIZE		128

	static long receiveTestDataArray[RCV_TEST_ARRAY_SIZE];
	static int wordCounter = 0;
	static int numOfReceivedFrames = 0, numOfReceiveErrors = 0;
	static int numOfInitWord = 0, numOfTestWordS =0, numOfTestWord3 = 0, numOfTestWord4 = 0, numOfTestWord5 = 0;
	
		
	if (rxBuffer->full == 1) {
		
		if (rxBuffer->data == TEST_DATA_START) {
			
			wordCounter = 0;
			numOfReceivedFrames++;
		};
		
		// copy received data word to the receive array
		if (wordCounter >= RCV_TEST_ARRAY_SIZE) wordCounter = 0;
		receiveTestDataArray[wordCounter] = rxBuffer->data;
		rxBuffer->full = 0;
		
		// update receive statistics
		switch(receiveTestDataArray[wordCounter]) {
			
			case TEST_DATA_START: numOfTestWordS++; break;
			case TEST_DATA_WORD3: numOfTestWord3++; break;
			case TEST_DATA_WORD4: numOfTestWord4++; break;
			case TEST_DATA_WORD5: numOfTestWord5++; break;
			case TEST_DATA_INIT:  numOfInitWord++;  break;
			
			default: numOfReceiveErrors++;
		};
		
		wordCounter++;
	};
};
*/
