/*-------------------------------------------------------------------------*
 * $Workfile: PilotMod.h $
 * Part of	: ETL600 / Frond End DSP
 * Language	: C
 * Created by: H.-J. Maag 
 * Remarks	:  
 * Purpose	: constant and structure definition for pilotmod.c
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/FE_DSP/H/PilotMod.h $
 * 
 * 3     28.07.05 12:02 Chhamaa
 * noGuardAlarm added in GuardDataType
 * 
 * 2     4.03.05 18:49 Chhamaa
 * Constants related to pilot weight removed
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 17    17.12.04 18:11 Chhamaa
 * EocDataBufferType moved to tdmEocData.h
 * 
 * 16    16.12.04 16:08 Chhamaa
 * dataBreak flag added to PilotModDataType
 * 
 * 15    14.12.04 11:08 Chhamaa
 * 
 * 14    1.12.04 17:24 Chhamaa
 * 
 * 13    30.11.04 17:47 Chhamaa
 * 
 * 12    30.11.04 15:32 Chhamaa
 * 
 * 11    26.11.04 15:19 Maag01
 * 
 * 10    23.11.04 17:11 Maag01
 * Function prototypes and constants added from pilot.h, pmconst.h
 * 
 * 9     5.11.04 15:49 Maag01
 * 
 * 8     4.11.04 10:20 Maag01
 * 
 * 7     3.11.04 16:47 Maag01
 * rx/tx buffer flags isFirstWord, overrun, outOfData removed
 * 
 * 6     5.08.04 15:37 Leeb02
 * 
 * 5     22.07.04 16:51 Maag01
 * GuardRec types integrated
 * 
 * 4     14.07.04 12:19 Leeb02
 * Dependencies removed
 * 
 * 3     17.06.04 18:24 Maag01
 * BLOCK_LENGTH_R_4K
 * 
 * 2     7.05.04 13:50 Leeb02
 * Now compilable with Visual Studio
 * 
 * 1     30.04.04 15:09 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 *  
 *
 *-------------------------------------------------------------------------*/

#ifndef		_PILOTMOD
#define		_PILOTMOD


/* INCLUDE FILES ***********************************************************/

#include "APLC_Types.h"			// pilot config. and status types
#include "FE_DSP_Const.h"       // general constant declaration
#include "tdmEocData.h"			// EocDataBufferType
#include "BiIIR_Filter.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define MWIRE_DELAYLINE_LENGTH   30					// MWire delay line length


/* GLOBAL TYPE DECLARATIONS ************************************************/


typedef struct {

  float sampleFreq;     		// sample frequency 
  float oscFreq;        		// oscillator frequency 
  float a, b;           		// oscillator coefficients  
  float x1, x2;					// state variables
  
} PilotOscDataType; 


typedef struct {

  float data[BLOCK_LENGTH_R_4K];     // data samples
  float MWire[BLOCK_LENGTH_R_4K];    // MWire sample
  
} TxSampleBlockType; 


typedef struct {

  long sampleCount;             // number of samples already sent
  long bitPos;                  // position of bit being sent
  long txWord;                  // word being sent
  
} TxParameterType;


typedef struct {

  PilotOscDataType osc;				// oscillator parameter	
  FilterDataType filterData;  		// filter parameters 
  TxParameterType txParameter;  	// sample inforamation about the data word being sent
  TxSampleBlockType txSample;   	// samples of data word and M-Wire signal
  float txPilot[BLOCK_LENGTH_R_4K]; // samples of modulated pilot
  float txScalingCoef;				// scales the modulated signal to the configured weight
  float lastDataSample;         	// last data sample is hold in case of MWire signalling
  long txInitCounter;	        	// delay counter (set to TX_INIT_DELAY after initalization)
  long MWire;                   	// MWire sample
  long MWireDelayLine[MWIRE_DELAYLINE_LENGTH]; // M-Wire delay line
  long delayInd;					// position of delay line pointer
  long edgeDetected;            	// flag indicating edge on MWire
  long minPwCounter;            	// counter of MWire pulse width
  long MWirePWFilterIn;         	// pulse width filter parameter
  long MWirePWFilterOld;        	// pulse width filter parameter
  long MWirePWFilterOut; 			// pulse width filter parameter
  long syncCounter;					// sync. frame is sent after pilot failer
  long rts;							// ready to send 
  long rtsOld;     					// hold old value
  long outOfData;					// no data available in tx buffer
  long dataBreak;					// data cannot be sent (pilot failure)
  
} PilotModDataType;

  
typedef struct {

  PilotOscDataType osc;			// oscillator parameter
  FilterDataType filterData;    // filter parameters
  float *dataChannelPtr;		// pointer to data channel sample
  float *sigChannelPtr;			// pointer to telephone signalling channel sample
  float sgnData;           		// sign of data channel sample
  float sgnSig;           		// sign of telephone signalling channel sample
  float filteredI;          	// in phase sample after low pass filtering
  float filteredQ;          	// quadrature sample after low pass filtering
  float shiftedI;           	// in phase sample after phase shifter
  float shiftedQ;           	// quadrature sample after phase shifter
  float stateI;            		// integrator state used in function demodFreqControl
  float uf;				   		// frequency control voltage (calculated in function demodFreqControl)
  float x1;						// base band oscillator state variable
  float x2;						// base band oscillator state variable
  float	rxLevelAtt;				// attenuation of rx pilot level
  long  channelsAreId;      	// channels are indentified flag
  long	EWireOn;            	// E-Wire switch
  long  EWire;					// actual E-Wire output 
  long  EWireOld;           	// hold value of E-Wire signal
  long  EWireHoldCounter;       // in case of EWireOn= 0 EWireOld is held during EWireHoldCounter >0
  
}  PilotDemodDataType;   


typedef struct {

  long oldVal;                	// last value
  long isFrameStart;          	// flag indicating a framestart
  
} FrameStartDataType;


typedef struct {

  long posLength;          		// max. length of positive pulse
  long negLength;           	// max. length of negative pulse
  long posOldVal;           	// last pos. sample
  long negOldVal;           	// last neg. sample
  long isPos;               	// flag indicating a pos. pulse
  long isNeg;               	// flag indicating a neg. pulse
  
} MeasureDataType; 


typedef struct {

  long oldVal;             		// last sample
  long actualLength;       		// length of actual pulse
  long shortLength;        		// lenth of shortest pulse
  long edgeCount;          		// number of detected edges
  
} MeasureShortDataType;


typedef struct {

  long sampleCount;             // number of samples already received
  long bitPos;                  // position of bit being received
  long word;                    // word being received
  
} DetectDataType;
   
   
// state definition of pilotReceiverController
enum PilotRecStateType  {pilotFailure,  identifyingChannels, receivingData,  waitingForFrameStart,
  			  			 receivingEWire};

typedef struct { 
                      
  MeasureShortDataType dataChannel;     // used to measure short pulses on the data channel
  MeasureShortDataType sigChannel;  	// used to measure short pulses on the signalling channel
  MeasureDataType dataChannelLong; 		// used to measure long pulses on the data channel
  MeasureDataType sigChannelLong; 		// used to measure long pulses on the signalling  channel
  FrameStartDataType frameStart;		// used to detect frame start
  
  // switches to activate processes in function pilot receiver
  long measureShortDataIsOn;
  long measureShortSigIsOn;
  long measureLongIsOn;
  long measureLongSigIsOn;
  long detectFrameStartIsOn;
  long detectDataIsOn;
  
  // variables used in function detectData
  long rxBufferOverrun;					// rx buffer not ready for new data
  DetectDataType ddParam;   			// parameters used in function detectData
  
  long isSigImp;               			// counter is set after sinalling was detected
  long initCounter;            			// counter to delay start up
  enum PilotRecStateType state;     	// actual stat of pilotReceiverController
   
} PilotRecDataType;


typedef struct {
	
	float levelAlarmTh;			// pilot level alarm threshold
	float snrAlarmTh;			// pilot SNR alarm threshold
	float levelLowTh;			// pilot level low threshold (HMI600: AGC blocking level)
	float noGuardLevelTh;		// pilot level is too low for receiver
	
} GuardConfigType;


typedef struct {

  long  guardIsPresent;         // actual guard signal level is higher than the mean*GUARD_THRESHOLD_LOW
  long  oldGuardIsPresent;      // last value of guardIsPresent flag
  long  timerGuardIsPresent;    // timer used to delay guardIsPresent after pilot failure 
  long  pilotIsOk;              // guard signal (pilot) quality (snr, level) allows data reception 
  long  fastMute;               // actual guard signal level exceeds the mean value by a specified factor GUARD_THRESHOLD_HIGH
  float rxGain;                 // rx gain factor
  long  guardLevelAlarm;        // received guard level is low (used for AGC and AFC blocking)
  long  noGuardAlarm;			// received guard level is too low for receiver (pilot failure)
  long  snrAlarm;               // snr of guard signal
  long  lpValid;                // actual low pass filter values (mean and noise level) are valid
  long  thValid;                // frozen values (mean, noise) are valid
  float guardDecisionLevelLow;  // lower level to generate guardIsPresent
  float guardDecisionLevelHigh;	// upper level to generate fastMute
  float inNoiseLevel;           // noise level of in phase component of guard signal
  float quadNoiseLevel;         // noise level of quadrature component of guard signal
  float inNoiseHold;            // frozen noise level of in phase component of guard signal
  float quadNoiseHold;          // frozen noise level of quadrature component of guard signal
  long  pilotCount1;            // counter to delay pilotIsOk signal (avoids oscillations) after pilot failure
  long  pilotCount2;            // counter to delay pilotIsOk signal (avoids oscillations) after pilot recovery
  long  sigOkCounter;           // counter to delay sigIsOk
  long  sigOkDelay;             // delay after sudden line attenuation
  float inMag;                  // absolute value of in phase sample
  float quadMag;                // absolute value of quadrature sample
  float meanIHold;              // frozen value of mean value of in phase component
  float meanQHold;              // frozen value of mean value of quadrature component
  float meanHold;               // frozen mean value
  float meanI;                  // actual mean value of in phase component
  float meanQ;                  // actual mean value of quadrature component
  float mean;                   // actual valid mean value 
  float rxLevel;				// receive level
  float noiseLevel;				// receive noise level
  float snr;                    // snr value of guard signal
  
} GuardDataType;


typedef struct {
 
  long ok;						// flag indicating normal operation
  float rxGain;					// receive gain factor
  float rxLevelDesired;			// desired pilot rx level		
  
} agcDataType;


// data structur for sending test data (used in function sendTestData())
typedef struct {
	
	int idleCount;
	int wordCount;
	int arrayCount;
	int frameStart;
	
} TxTestDataParamType;


/* PUBLIC FUNCTIONS ********************************************************/
 
void initPilotModemCh1(t_PilotModemConfig *pilotConfig, PilotModDataType *pilotModData,
					   PilotDemodDataType *pilotDemodData, PilotRecDataType *pilotRecData,
					   GuardConfigType *guardConfig, GuardDataType *guardStatus, agcDataType *agcData);
void initPilotModemCh2(t_PilotModemConfig *pilotConfig, PilotModDataType *pilotModData,
					   PilotDemodDataType *pilotDemodData, PilotRecDataType *pilotRecData,
					   GuardConfigType *guardConfig, GuardDataType *guardStatus, agcDataType *agcData);
void initPilotModemCh3(t_PilotModemConfig *pilotConfig, PilotModDataType *pilotModData,
					   PilotDemodDataType *pilotDemodData, PilotRecDataType *pilotRecData,
					   GuardConfigType *guardConfig, GuardDataType *guardStatus, agcDataType *agcData);
				   
void txDataProcessing(EocDataBufferType *txBuffer, PilotModDataType *pilotMod);
void pilotTransmitter(PilotModDataType *pilotMod, float *outSample, int blockLength);
void pilotReceiver(EocDataBufferType *rxBuffer, PilotDemodDataType *pilot, PilotRecDataType *pilotRecCh1, GuardDataType *gStatus);
void pilotReceiverController(PilotDemodDataType *pilot, PilotRecDataType *pilotRecCh1, GuardDataType *gStatus);
long generateEWire(PilotDemodDataType *pilot);
void demodFreqControl(PilotDemodDataType *pilot, GuardDataType *gStatus);
void pilotAmpAdjust(PilotOscDataType *osc);
void pilotBbAmpAdjust(PilotDemodDataType *pilot);

void rxGuard(GuardDataType *gStatus, PilotDemodDataType *pilot, t_PilotModemStatus *pilotStatus);
void evaluateGuard(GuardDataType *gStatus, GuardConfigType *gConfig);
void gainControl(GuardDataType *gStatus, agcDataType *agcData, long blockAGC);
void updatePilotStatus(GuardConfigType *gConfig, GuardDataType *gStatus, PilotModDataType *pilotMod, 
					   PilotDemodDataType *pilotDemod, agcDataType *agcData, t_PilotModemStatus *pilotStatus);

// Assembly function prototypes 
void pilotModulator(PilotModDataType *pilotModCh1, float *data, float *MWire, float *outSample, int blockLength);
void pilotDemodulator(PilotDemodDataType *pilotDemod, float *inSample, int blockLength);

#endif
