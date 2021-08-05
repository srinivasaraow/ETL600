/*-------------------------------------------------------------------------*
 * $Workfile: FSK.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : T.Gnus
 * Remarks      :  
 * Purpose      : FSK calculation and interpolation
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source MAS/P4LT/NSK_DSP/H/FSK.h $ 
 * 
 * 24    22.09.08 13:58 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 23    08-05-07 14:42 Chtognu
 * no Rx Signal Alarm added, mod and demodfunction moved to internal
 * memory
 * 
 * 22    08-03-07 16:55 Chsteic
 * '- #define DAPSK_DETE_MIN_POWER_RCV        0.1
 * - test Flags added
 * - new algorithm
 * - variables renamed
 * - logging of new variables
 * - changed memory map
 * 
 * 21    08-02-25 9:57 Chtognu
 * Merged changes from ad.equ.branch:
 * nskDCDAlarm (NSK modem x no Rx signal detected) added for DAPSK ad.eq
 * (will be ignored for FSK)
 * 
 * 16    12.10.05 9:56 Chchlee
 * private functions removed from interface
 * 
 * 15    11.10.05 14:27 Chtognu
 * unused variables removed
 * 
 * 14    10.10.05 18:48 Chtognu
 * New 1200 above speech filters used
 * 
 * 13    20.09.05 17:15 Chtognu
 * minor changes in DCD detection, removed unused comment
 * 
 * 12    16.09.05 15:00 Chtognu
 * Dataregeneration constant (alpha) changed, SNR Linkalarm terms added
 * 
 * 11    5.09.05 11:03 Chtognu
 * new definite outputsignal at the beginning of a transition
 * 
 * 10    29.08.05 10:02 Chtognu
 * SNR measurement stronger averaged, testfunctions removed
 * 
 * 9     12.08.05 9:05 Chtognu
 * bug 104 fixed -> SNR Level alarm appears only if the alarm is longer
 * then 4 sec active, no more biterrors after RTS active 
 * 
 * 8     24.06.05 13:50 Chtognu
 * 
 * 7     27.05.05 17:03 Chtognu
 * FSK SNR Alarm level modified
 * 
 * 6     21.05.05 12:08 Chtognu
 * clock recovery bug fixed
 * 
 * 5     12.05.05 9:22 Chtognu
 * alarm added
 * 
 * 4     29.03.05 8:08 Chtognu
 * Status data changed and functions moved to external memory
 * 
 * 3     2.03.05 9:51 Chtognu
 * Schüsslerfilter 62 added for 1200Baud above speech, tx path. all tx
 * filter 3.order
 * 
 * 2     17.02.05 14:00 Chtognu
 * merged Version with changes done in Prototype 1 till 16.02.05
 * 
 *-------------------------------------------------------------------------*/

#ifndef FSK_H
#define FSK_H

/* INCLUDE FILES **************************************************************/

#include "oscillator.h"
#include "NSK_DSP_Types.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define NUM_OF_BIQ_SEC_TX			3 // Numbers of Biquadrat sections on the TX side
#define NUM_OF_BIQ_SEC_RX			3 // Numbers of Biquadrat sections on the RX side

// Nummer of bits which were used to calculate the mean of the carrier
#define SAMPLES_FOR_MEAN_CARRIER     12


/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* t_FSK_TxFilter *************************************************/
typedef struct {
  long numOfSec;                       // number of biquad filter sections
  float pm *coefPtr;                      // pointer to filter coefficients
  float ziI[2*NUM_OF_BIQ_SEC_TX];      // start/return parameter I
  float ziQ[2*NUM_OF_BIQ_SEC_TX];      // start/return parameter Q

} t_FSK_TxFilter;
 
/* t_FSK_RxFilter *************************************************/
typedef struct {
  long numOfSec;                       // number of biquad filter sections
  float pm *coefPtr;                   // pointer to filter coefficients
  float ziI[2*NUM_OF_BIQ_SEC_RX];      // start/return parameter I
  float ziQ[2*NUM_OF_BIQ_SEC_RX];      // start/return parameter Q

} t_FSK_RxFilter;

/* t_FSK_TxTiming *************************************************/
typedef struct {
	long sendData;                     // Timing Flag for DSP Funktion (= 1 modulate data, else dont send data)
	long sendCarrier;                  // Timing Flag for DSP Funktion (= 1 modulate carrier, else dont generate carrier)
	BOOL RTS;                          // Timing Flag from FPGA (has to be updatet by seperate function)
	long CTS;                          // Timing Flag for FPGA (has to be send to FPGA by seperate function)
	int waitendStart;                  // terminal value to wait until output data
	int counterStart;                  // current value of the delay counter
	int waitendEnd;                    // terminal value to wait until output data
	int counterEnd;                    // current value of the delay counter

} t_FSK_TxTiming;
  
/* t_FSK_Modulator *************************************************/
typedef struct {
	unsigned long mode;
	long baud;
	long bandwidth;
	long txfrequency;
	float weight;
	unsigned int shiftfrequency;       // Frequency shift [Hz]
	float oldState;					   // last(old) State of the received TXD data
	float modulatorPhi;                // Phi of the FSK modulation incl. jitter reduction
    float modulatorPhiDelta;           // currend value of the FSK modulator oscillator
    float fskModulatedI;               // FSK modulated I Channel
	float fskModulatedQ;               // FSK modulated Q Channel
	float fskFilteredI;                // in phase sample after low pass filtering
    float fskFilteredQ;                // quadrature sample after low pass filtering
    float fskUpmodulatedI;             // FSK upmodulated I Channel
    float fskUpmodulatedQ;             // FSK upmodulated Q Channel
    float fskTx;                       // FSK transmitting Signal (Channel I + Q)
    t_FSK_TxFilter  txFilter;          // step of the FSK modulation oscillator
	t_OscData       txOsc;             // Oscillator for upmodulation

	float standardisationFactor;       // Factor to normalise tx signal on "1"
	
	// Test variables:
	//	float testadjustment;
	//	float testrms;
	//	float testquadrms;
	//  long testrmscounter;
	//	float maxfskTx;

} t_FSK_Modulator;
  
/* t_FSK_Demodulator *************************************************/
typedef struct {
	
    unsigned long mode;
	long baud;
	long bandwidth;
	long rxfrequency;
	unsigned int shiftfrequency;       // Frequency shift [Hz]
    
	float fskFilteredI;                // FSK currend filtered I Data, used for demodulation
	float fskFilteredQ;                // FSK currend filtered Q Data, used for demodulation
	float fskFilteredOldI;             // FSK last filtered I Data, used for demodulation
	float fskFilteredOldQ;             // FSK last filtered Q Data, used for demodulation
	float fskDemodulatedSignal;        // FSK currend demodulated Signal
	float fskDemodulatedOldSignal;     // FSK last demodulated Signal, used for hysteresis function

	float fskDemodulatedFilteredSignal;
	float fskDemodulatedOldFilteredSignal;

	float butterFilter[4];
	float pm *buttercoefPtr;


	BOOL fskRxOut;                     // FSK Rx currend Outputsignal
 	BOOL fskOldRxOut;                  // FSK Rx last Outputsignal
	long  fpgaRUD;                     // FPGA RUDx Outputsignal
	long  demodRUD;                    // Demodulator RUDx Outputsignal
	t_FSK_RxFilter  rxFilter;          // 
	t_OscData       rxOsc;             // Oscillator for downmodulation
	float weight;
	float inverseweight;

	float fskDownmodulatedI;
	float fskDownmodulatedQ;

	// TEST variables:	
	//	float testatan;

} t_FSK_Demodulator;

/* t_FSK_Carrier *************************************************/
typedef struct {

	int currentCarrierPosition;           // Control variable to fill the fskCarrier[]-Array
	float currentCarrier;                 // Mean of the latest Carrier values
//	float DCDon;                          // threshold to detect the carrier is ON
//	float DCDoff;                         // threshold to detect the carrier is OFF
	long fskDCD;                          // DCD signal to transmit to FPGA
//	int bitsToWait;                       // Nummer of bits to wait from detectin a carrier and 
//	                                      // sending Data
	int waitend;                          // terminal value to wait until output data
	int delay;                            // current value of the delay counter
	
	int carrierValid;					// factor for saving carriervalue

	int i_carrierValid;
	float latestCarriers[SAMPLES_FOR_MEAN_CARRIER];
	int oversamplingPerBitForMeancarrier;

	// SNR
	float fskSNRAlarmlevelON;
	float fskSNRAlarmlevelOFF;
	float meanCarrierPower;
	float sqrtNoisePower;
	float noisePower2;
	float meanNoisePower2;
	float meanCarrierPower2;

	float SNR;
	float meanSNR;
	BOOL SNRalarm;
	float reciprocalNumCarrierMean;
	BOOL SNRhysteresysFlag;

	int DCDminOnCount;
	int noSNRmeanCount;

	int SNRalarmOffcount;
	BOOL fskSNRalarmCountFlag;

	int noDataPickupCount;

	// test to ajust the threshold for the SNR link alarm
	//	float meanSNRmin;
	//	float meanSNRmax;
	//	float meanSNRmin_i;
	//	float meanSNRmax_i; 
	//	int alarmlevelcount;

} t_FSK_Carrier;

/* t_FSK_Clockreg *************************************************/
typedef struct {
	int clockregState;
	
	float nominalBitLenght;
	float halfNominalBitLenght;
	float timeToNextSampling;
	float deviation;
	
	int noOfTransitions;
	float noOfEqualBits;
	float pieceOfEqualBit;
	int bitCounter;

} t_FSK_Clockreg;

/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/

void SEGMENT_EXT_CODE InitModulatorFSK(t_FSK_Modulator *modulatorData, t_ModemConfig *modemConfig, \
											t_V24PortCfg *v24Config,  t_FSK_TxTiming *txTimingData);
void SEGMENT_EXT_CODE InitDemodulatorFSK(t_FSK_Demodulator *demodulatorData, t_ModemConfig *modemConfig,\
											  t_V24PortCfg *v24Config, t_FSK_Carrier *CarrierData,\
											  t_FSK_Clockreg *ClockregData);

void modulatorFSK(long *TxDataFPGA, t_FSK_Modulator *modulatorData, t_FSK_TxTiming *txTimingData);
void demodulatorFSK(float *RxDataIn, t_FSK_Demodulator *demodulatorData \
										  /* testFSK ,t_FSK_Modulator *modulatorData */ ); 

void SEGMENT_EXT_CODE txTimingFSK(t_FSK_Modulator *modulatorData, t_FSK_TxTiming *TxTimingData);

void SEGMENT_EXT_CODE CarrierDetectionFSK(t_FSK_Demodulator *demodulatorData, t_FSK_Carrier *CarrierData);
void SEGMENT_EXT_CODE SNRcalculationFSK(t_FSK_Carrier *CarrierData);

void SEGMENT_EXT_CODE InitClockregeneratorFSK(t_FSK_Clockreg *ClockregData, t_FSK_Demodulator *DemodulatorData);
void SEGMENT_EXT_CODE clockregeneratorFSK(t_FSK_Clockreg *ClockregData, t_FSK_Demodulator *DemodulatorData, \
										  t_FSK_Carrier *CarrierData, BOOL *ClockregActiv);

void SEGMENT_EXT_CODE statusDataFSK(t_FSK_TxTiming *txTimingData, t_FSK_Carrier *CarrierData, t_NSK_DSP_Status *StatusData, \
									BOOL *nskLinkAlarm, BOOL *nskDCDAlarm, BOOL *nskRTSalwaysActive, int ModemNo);
#endif
/* End of File ***********************************************************/
