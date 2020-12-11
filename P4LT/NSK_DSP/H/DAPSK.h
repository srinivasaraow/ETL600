/*-------------------------------------------------------------------------*
 * $Workfile: DAPSK.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : T.Gnus
 * Remarks      :  
 * Purpose      : DAPSK
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source MAS/P4LT/NSK_DSP/H/DAPSK.h $ 
 * 
 * 34    22.09.08 13:57 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 37    08-07-03 10:01 Chsteic
 * ' - moved idle case from INITACQUISITION3 to INITACQUISITION1
 *  - removed #pragma optimize_off from executeInitacquisiton- functions
 *    for processing load  reasons
 * 
 * 36    08-07-01 16:18 Chsteic
 * ' - minor changes in code for comparison with matlab. No running code
 * of RC5 changed.
 * 
 * 35    08-06-26 9:52 Chtognu
 * Modem initialization spread to more cases to save computing time.
 * Firstloop with a startupdely of 1 cycle again before alarmindication. 
 * 
 * 34    08-05-07 14:42 Chtognu
 * Max equalizer length changed (bug fixed), no Rx Signal Alarm adapted
 * (pickup time added), unused variables removed
 * 
 * 33    08-05-02 15:47 Chsteic
 * '-DAPSK_DETE_MIN_POWER_RCV   = 0.1
 * - removed old DCDoff code
 * - inserted now DCD off criteria
 * - inserted DCDoff delay off 5s in chase of SNR<15dB
 * - in commented code of 4800/2400bps modems
 * - inserted reset of AEQ coefficients, AEQ delay lines, AEQ delay line
 * index
 * - inserted new state INITACQUISITION2, used for a correct startup
 * - remoded unused variables
 * 
 * 32    08-04-25 15:50 Chsteic
 * '- changed rho of SNR calculation
 * - inserted DCDON hold  timer for AEQ
 * - removed defines
 * - removed some unused code
 * - limit gain factor to 5 (14dB)
 * 
 * 31    08-04-24 9:24 Chsteic
 * '- inserted counterDCDoffMax
 * - logging max signalQuality in NSK_ShortStatus
 * 
 * 30    08-04-11 13:56 Chsteic
 * '-changed jump in criteria from 0.2 to 0.1
 * - inserted DCDoff counter
 * - changed stepSizeTiming and stepSizeTimingPhase for adaptive EQ
 * - moved P_rcv behind gain
 * - removed Modem 0: DAPSK_AST_BEFORE_EQ=FALSE
 *                Modem 1:  DAPSK_AST_BEFORE_EQ=TRUE
 * - quick fix of NAN in signal quality
 * 
 * 29    08-04-08 14:09 Chsteic
 * inserted  long iexModem in void demodulatorDAPSK()
 * Modem 0: DAPSK_AST_BEFORE_EQ=FALSE
 * Modem 1:  DAPSK_AST_BEFORE_EQ=TRUE
 * 
 * 28    08-04-03 9:43 Chtognu
 * txFormSymbolDAPSK() moved to external memory. Unused variables removed.
 * ad.eq: - new possibility added to seperate AST from ad.eq. Algorithmus
 * (DAPSK_AST_BEFORE_EQ) --> and seperated
 * - DAPSK_SAMPLE_STEP input for AST removed. 
 * - Update CMA back to 0.03
 * 
 * 27    08-03-27 8:44 Chtognu
 * new autonomous AGC for ad.equalizer with adaptive eq update conditions
 * drift-control removed
 * 
 * 26    08-03-20 14:48 Chsteic
 * '- inserted first version of a drift-control
 * - logging of non rounded tau
 * 
 * 25    08-03-07 16:55 Chsteic
 * '- #define DAPSK_DETE_MIN_POWER_RCV        0.1
 * - test Flags added
 * - new algorithm
 * - variables renamed
 * - logging of new variables
 * - changed memory map
 * 
 * 23    08-03-05 10:56 Chsteic
 * ' -inserted logging of realTransmitSignalabsMax,pRcvMin
 *  -minor changes in EQUpdate_Flag logic
 * 
 * 22    08-03-04 10:32 Chsteic
 * '-inserted EQUpdate_Flag. For developing issues used.
 * -inserted NSK_ShortStatus_float and  NSK_ShortStatus_long array
 * - inserted SignalPowerRMax,SignalPowerIMax, normalization_factorMax for
 * the logger
 * 
 * 21    08-02-28 16:58 Chsteic
 * '-tau inserted
 * -changend Testvar, tau is logged now
 * 
 * 20    08-02-28 9:39 Chsteic
 * - added parameter t_NSK_ShortStatus  *NSK_ShortStatus in function
 * statusDataDAPSK()
 * 
 * 19    08-02-25 9:55 Chtognu
 * Merged changes from ad.equ.branch:
 * ad. Equalizer functionality incl. Logger, scrambler, new filter,
 * removed preamble detection, alarm, processing time optimization, global
 * interrupt disabled during DAPSK fir function, FE path expantion
 * 
 * 14    26.10.05 14:57 Chtognu
 * Tx FIFO extended
 * 
 * 13    26.10.05 13:32 Chtognu
 * Idle bits after preamble increased, out commented code for
 * "parable"-acquisition added, some initial values added
 * 
 * 12    11.10.05 14:52 Chtognu
 * Ampl & Phase criteria now used for adaptive symbol timing and graymap
 * rotated (CHANGE 6 - 9). real and imag part of Signalpower used now to
 * leave acquisition case, lower level to fall back to acquisiton
 * 
 * 11    26.09.05 18:03 Chtognu
 * CTS always on working with idle bits (power criteria for leaving
 * acquisition)
 * 
 * 10    5.09.05 11:02 Chtognu
 * new end of communication sequenze, new DCD handling names
 * 
 * 9     29.08.05 10:19 Chtognu
 * threshold added for SNR alarm, delay for alarm after DCD active added.
 * After returning to acquisiton state, DCD stays active for a def. time
 * if SNR alarm is active (to prevent oscilation of DCD signal)
 * 
 * 8     22.08.05 8:39 Chtognu
 * stopbits added at the end of transition
 * 
 * 7     27.05.05 17:11 Chtognu
 * Modulator: some computing time optimization done
 * Demodulator: new case added (initacquisition)
 * 
 * 6     12.05.05 14:04 Chmibuh
 * 
 * 5     12.05.05 9:19 Chtognu
 * datasensitive mode ok, structure changed, alarm added
 * 
 * 4     29.03.05 8:06 Chtognu
 * Signal error calculation added
 * 
 * 3     2.03.05 9:54 Chtognu
 * 
 * 2     17.02.05 14:00 Chtognu
 * merged Version with changes done in Prototype 1 till 16.02.05
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 3     7.01.05 14:59 Chtognu
 * Rx path working (Ringbuffer in Tx path not working)
 * 
 * 2     9.12.04 16:39 Chtognu
 * 
 * 1     2.12.04 13:36 Chtognu
 * 
 * 2     2.12.04 13:28 Chtognu
 * 
 * 13    2.12.04 13:11 Chtognu
 * 
 * 12    2.12.04 11:45 Chtognu
 * 
 * 11    2.12.04 8:38 Chtognu
 * 
 *-------------------------------------------------------------------------*/


#ifndef DAPSK_H
#define DAPSK_H

/* INCLUDE FILES **************************************************************/

#include "oscillator.h"
#include "NSK_DSP_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
// DAPSK Modem
#define DAPSK_L_Delay                 10    // length of TX Delayline
#define DAPSK_L_PREAMBLE               6    // length of RX Preamble
#define DAPSK_MAX_NO_DELAYLINES        2    // Maximum Number of RX Delaylines (Matlab nr_delay_lines)

#define DAPSK_ACQU_SAMPLES_PER_SYMBOL  8    // Acquisition Sambles per Symbol 
                               //  -> (Nss/(DAPSK_ACQU_NO_SAMPLES*symbTimingResolution))
// PL_change no Preamble #define DAPSK_ACQU_NO_CORR_SYMBOL      2    // Number of Symbols for Correlation Array

// #define DAPSK_L_MATCHED_FILTER_MAX   182    //Maximal length of polyphase matched
#define DAPSK_L_MATCHED_FILTER_MAX   140    //Maximal length of polyphase matched filter
   // -> Calculation: ((DAPSK_L_Delay*(DAPSK_F_INTERM/(Baud/DAPSK_M))/DAPSK_DOWNSAMPLING) + 1) + (DAPSK_SUM_EARLY_LATE_POINTS/DAPSK_MAX_NO_DELAYLINES)
#define DAPSK_MAX_NO_EXTENDED_ZEROS    14    // Maximum Number of Zeros at the end of the Filtercoef (optimization: using only one coef array)

// Input Conversion
#define DAPSK_L_IN_FIFO                340  // length of TX FIFO from FPGA (for additional delay and preamble)
#define DAPSK_NO_IN_FIFO               2    // Number of TX FIFO Lines (Data and EoCh Flag = 2)

// Output Conversion
#define DAPSK_L_OUT_FIFO               9    // length of RX demodulated bit FIFO for FPGA conversion


// adEq_change add config
#define DAPSK_SCRAMBLER_LENGTH         15    // 

/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* t_DAPSK_InputConversion ********************************************/
typedef struct {
	int i_newestBit;
	int i_oldestBit;
	int noOfBits;
	int noOfDealyBitsAfterPreamble;
	int txDataFIFO[DAPSK_NO_IN_FIFO][DAPSK_L_IN_FIFO];
	BOOL firstStopBit;
	int noOfInfoBits;
	BOOL asynchronousProtocol;
	int stateMarkEndOfChar;
	int characterBitCounter;

	int stateReadTelegram;

	// used in formSymbol
	int stateFormSymbol;
	
	// Handshake lines
	int delaytimeRTS;
	int remainedDelaytimeRTS;
	int noDelayedDatabits;

	// Statusflag Counter for Bufferoverflow
	int bufferOverflowCounter;
	int bufferUnderflowCounter;
	
	int waitDelayLine;
	int waitDelayLineCount;
	int waitDelayLineEnd;

	BOOL addStopbit;
	BOOL addStopbit2;
	
} t_DAPSK_InputConversion;

/* t_DAPSK_Modulator *************************************************/
typedef struct {
	int sendDataFlag;							// Modem only sends Data if Flag = 1
	unsigned long mode;
	long baud;
	long bandwidth;
	long txfrequency;
	long rSymb;                                   // Symbolrate 
	long nss;                                     // Number of Filterlines (length of symbol)
    long nssIndex;                                // index of Nss
	BOOL newSymb;                                 // indicating a new Sybol to encode (every DAPSK_M bits)
	long transmission;                            // State of transmission
	complex_float statePhi;                       // State of Phi from last modulationcycle
	int stateMSB;                                 // State of MSB from last modulationcycle 
	float *plFilterCoef;                          // Pointer to polyphase filter coefficients 
	complex_float inPolyphase;                    // encoded complex signal passed to polyphase filter
	complex_float zpfDelayline[DAPSK_L_Delay+1];  // Current values of the polyphase filter Delayline (array)
	complex_float outPolyphase;                   // complex modulated signal from polyphase filter
	float realTransmitSignal;                     // real signal from Modulater going to TDM bus
	float realTransmitSignalabsMax;               
	complex_float outOsc;                         // Pointer to complex oscillator outputdata
	t_OscData     txOsc;                          // Oscillator for upmodulation

	// Handshake lines
	BOOL fpgaRTS; // comming from FPGA
	BOOL modemRTS; // delayed RTS (if necessary)
	BOOL CTS;

	float txOutputGain; // Standardisation Factor Peaksignal = 1 AND weight

	// Maximal transmitted Signal
	//float maxReal;
	//float minReal;

//___________________________________________________________________________________________
// adEq_change add config
	BOOL adEqOn;					// adap.Equalizer on/off flag
	BOOL scramblerOn;                 // Scrambler on/off flag
	BOOL difMSB;					// True = toggel MSB

	int adEqScramblerLine[DAPSK_SCRAMBLER_LENGTH]; // scrambler delay line transmitter
	int scramblerXorP1;
	int scramblerXorP2;
	int scInputPointer;
	
} t_DAPSK_Modulator;

/* t_DAPSK_Demodulator *************************************************/
typedef struct {
	unsigned long mode;
	long baud;
	long bandwidth;
	long rxfrequency;
	long rSymb;                          // Symbolrate (Matlab Rsymb)
	long nss;                            // Number of Filterlines (length of symbol)
	int reception;						 // State in Demodulation process (Matlab reception)
	
	float rxInputGain;					 // 1/weight AND 1/standardisationFactor 	

	float gain;                          // adjustable gain of received signal (Matlab gain)
	float oldgain;
	int stateMSB;                        // State of MSB from last modulationcycle (Matlab MSB_old)
	
	// acquisition / preamble:
	BOOL acquisitonStatus;  // TRUE = Acquisiton left with preamble correlation
// PL_change no Preamble	pm float *ppreamble;   // Pointer to Rx preamble used for correlation (based on rxPreambleBits)

	float rohNoise;						 // time constant for smoothing filter 
	float rohSignal;                     // time constant for smoothing filter 
	float NoisePower;
	float SignalPowerR;
	float SignalPowerI;
	float SignalPowerRMax;
	float SignalPowerIMax;
	int iNewZ_y0;
	int iEndZ_y0;
	complex_float z_y0[DAPSK_ACQU_SAMPLES_PER_SYMBOL+1]; // Array with polyphase filter outputs reused in signal acquisition

// PL_change no Preamble	int   iNewZyCorrelation;				// index of correlated Values - Array (new value)
// PL_change no Preamble	float zyCorrelation[DAPSK_ACQU_NO_CORR_SYMBOL*DAPSK_ACQU_SAMPLES_PER_SYMBOL]; // Array with correlated Values of 2 Symbols
	
	int	  iEndZyReal;
	int   iNewZyReal;
	float zyReal[(DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1]; // Array with real part of differential symbol received
	
	int symbTimingResolution;            // resolution of the symboltiming
	long symbPerFilterline;				 // Symbols per Filterline (nss/symbTimingResolution)

	//polyphase filter / delayline:
	int noOfDelaylines;					// Number of used Delaylines
// PL_change no Preamble #pragma align 2
// PL_change no Preamble	float zpFir[DAPSK_ACQU_SAMPLES_PER_SYMBOL][8+2];// numerator coefficient vector for FIR "correlation"
#pragma align 2
	float zpFir[DAPSK_ACQU_SAMPLES_PER_SYMBOL][8+2];// numerator coefficient vector for FIR "correlation"
#pragma align 2
	float pmfDelaylineReal[DAPSK_MAX_NO_DELAYLINES][DAPSK_L_MATCHED_FILTER_MAX+DAPSK_MAX_NO_EXTENDED_ZEROS]; //delayline (array of max possible length)  
#pragma align 2
	float pmfDelaylineImag[DAPSK_MAX_NO_DELAYLINES][DAPSK_L_MATCHED_FILTER_MAX+DAPSK_MAX_NO_EXTENDED_ZEROS]; //delayline (array of max possible length)  
	
	int lengthMachedFilter;              // length of the used Matched Filter
	pm float *pmf0FilterCoef;            // pointer to polyphase matched filter extended by 2 zeros per filter  

	int pmfEarlyLateShift;               // shift for the add. coefficients of the early late calc. (cause using only one coef.array) 

	int index_symb;                      // indicates the time index of the task cycle within a symbol
    int index_mf;                        // Index of polyphase matched filter
// PL_change no Preamble    int index_acq;                       // Index of delay line used for correlation in aquisition

	// Adaptive Symbol Timing (early/late)
	int timingSymb;						 // Adaptive Symbol timing depending on Symbolrate (Matlab dT)
	int timingSymb2;					 // twice Adaptive Symbol timing depending on Symbolrate (Matlab dT2)

	complex_float outOsc;                // Pointer to complex oscillator outputdata
	t_OscData       rxOsc;               // Oscillator for upmodulation
	
	// DETECT Symbol
	complex_float dapskOld;
	int dapskMsbOld;
	int IB[DAPSK_M];                     // array with the demodulated bits (Matlab IB) -> output FPGA
	int JB[DAPSK_M];                     // array with the demodulated bits (Matlab JB) 
										 // -> for errormeasurement and early late (amplitude independent)
	complex_float dapsk;
	complex_float dapskR;
	complex_float dapskRD;
	
	// Adaptiv Symbol Timing
	complex_float dapsk_m;
	complex_float dapsk_mm;
	complex_float dapskOld_m;
	complex_float dapskOld_mm;
	complex_float dapskR_m;
	complex_float dapskR_mm;

	complex_float dapsk_p;
	complex_float dapsk_pp;
	complex_float dapskR_p;
	complex_float dapskR_pp;
	complex_float dapskOld_p;
	complex_float dapskOld_pp;

	int changeIndex;

	float tau0;
	float tau0c; // cummulated Tau0
	float tau;
	float corrIndex;
//	BOOL  stayDetect_Flag;


	// newEarlyLate CHANGE6
	float dapsk_squ;
	float dapsk_m_squ;
	float dapsk_mm_squ;
	float dapsk_p_squ;
	float dapsk_pp_squ;
	float dapsk_d_squ;

//	int dapsk_d_MSB;
	
	// FPGA Output conversion
	BOOL RxSymbolReady;

	long  fpgaRUD;                     // FPGA RUDx Outputsignal

	int countSignalOn;
	float error;
	float pRcv;
	float pRcvMin;
	float dapskAbs;
	
	///////////////////////////////////////////////////////////////////////////
	// Handshake / Alarms
	BOOL DCD;
	BOOL hysteresysFlag;
	BOOL DCDflag;
	int DCDoffcount;
	int StayDetectCounter;
	int DCDoncount; // Counter used to not jump back to Acuisition before end of time 
	BOOL LinkalarmFlag;
	int DCDholdOnTime;
	BOOL DCDholdOnCriteria;
	int DCDminOnCount;
	int noDataPickupCount;
	///////////////////////////////////////////////////////////////////////////
    
//__________________________________________________________________________________
// adEq_change add config
 
	/////////////////////////////////////////////////////////////////////////
    // DAPSK_AST_BEFORE_EQ
	
	int dapskMsb_AST_Old; // --> dapskMsbOld
	
	complex_float dapsk_MFout; // --> dapsk;
	complex_float dapsk_MFout_Old; // --> dapskOld

	complex_float dapsk_MFout_m; // -->  dapsk_m
	complex_float dapsk_MFout_Old_m; // -->  dapskOld_m

	complex_float dapsk_MFout_p; // --> dapsk_p
	complex_float dapsk_MFout_Old_p; // --> dapskOld_p

	/////////////////////////////////////////////////////////////////////////
    // Adaptive Equalizer
    
    float stepSizeTimingPhase;// step-size for adaptive symbol timing in Rx 
    float stepSizeTimingAmp;  // step-size for adaptive symbol timing in Rx

	BOOL adEqOn;              // True = ad. Equalizer is used
	BOOL scramblerOn;		      // Flag to switch on or off of the scrambler
	BOOL difMSB;					// True = toggel MSB

    int   adEqLength;

    const int *mfIndexTable;  // pointer to matched filter index table. with it it's possible to get the index of symbol 2, early 2 or late 2 for the adaptive equalizer.
	                          // The first colums corespond to the mf_index. column two i the required delay in samples at 10.66kHz.
    complex_float adEq_h[DAPSK_AD_EQ_LENGTH_MAX]; // filter coefficients of adaptive equalizer

    complex_float adEqInput2[2*DAPSK_AD_EQ_LENGTH_MAX];
	int adEqInput2i;
    complex_float adEqInput2_p[2*DAPSK_AD_EQ_LENGTH_MAX];
	int adEqInput2i_p;
    complex_float adEqInput2_m[2*DAPSK_AD_EQ_LENGTH_MAX];
	int adEqInput2i_m;

	int index_symb_ad;
    
	int adEqScramblerLine[DAPSK_SCRAMBLER_LENGTH]; // scrambler delay line transmitter
	int scramblerXorP1;
	int scramblerXorP2;
	int scInputPointer;

	complex_float NL;
	float Rdd;

	float rhoAGC;
	float P_AGC;

	int offsetRealU;
	int offsetImagU;
	
	float errorCriterion;
	float errorCriterionMax;
	unsigned long	InitAcqFlagCounter;
	unsigned long	DCDoffFlagCounter;

// case_counter test
/*
	int counter_aquisition0;
	int counter_aquisition1;
	int counter_aquisition2;
	int counter_aquisition3;
	int counter_aquisition4;
	int counter_aquisition5;
	int counter_aquisition6;
	int counter_detect;
	int counter_early;
	int counter_late;
	int counter_beforeearly;
	int counter_afterlate;
	int counter_adsymbol;
*/
    ////////////////////////////////////////////////////////////////////////
	// test maxProcessingPower
	/*
    complex_float test_outPolyphase;
	*/

} t_DAPSK_Demodulator;

/* t_DAPSK_OutputConversion *************************************************/
typedef struct {

	int i_newestBit;
	int i_oldestBit;
	int noOfBits;
	int rxDataFIFO[DAPSK_L_OUT_FIFO];

	
	float currentBitLeftTime;
	float rxTimeStock;

	float nominalBitLength;
	float actualBitLength;
	float upperQuantity;
	float lowerQuantity;

	int currentBitSendToFPGA;
	int stateSendDataToFPGA;
	
} t_DAPSK_OutputConversion;

 /* testMatlab
#define L_Matlab_save               1000    // length of saverd values
// end testMatlab */

// t_DAPSK_OutputConversion *************************************************/
/*  testMatlab
typedef  struct {
	int n_cntr;
	float N_cntr;
	float save_flag;
	int symb_nr;
	float symb_nr_f;
	float downmodData_re [L_Matlab_save]; // 1
	float downmodData_im [L_Matlab_save];
	float dapsk1_re		 [L_Matlab_save];
	float dapsk1_im	     [L_Matlab_save];
	float state			 [L_Matlab_save];
	float index_symb	 [L_Matlab_save];
	float pmf			 [L_Matlab_save];	
	float dapsk_re		 [L_Matlab_save];
	float dapsk_im	     [L_Matlab_save];
	float gain   	     [L_Matlab_save]; // 10
	float dapskRD_re	 [L_Matlab_save];
	float dapskRD_im     [L_Matlab_save];
	float IB		     [L_Matlab_save];
	float dapsk_m_re	 [L_Matlab_save];
	float dapsk_m_im	 [L_Matlab_save];
	float dapsk_p_re	 [L_Matlab_save];
	float dapsk_p_im	 [L_Matlab_save];
	float dapsk_mm_re	 [L_Matlab_save];
	float dapsk_mm_im	 [L_Matlab_save];
	float dapsk_pp_re	 [L_Matlab_save]; // 20
	float dapsk_pp_im	 [L_Matlab_save];
	float dapskR_re		 [L_Matlab_save];
	float dapskR_im		 [L_Matlab_save];
	float index_symb_pre  [L_Matlab_save];
	float index_symb_pre2  [L_Matlab_save];
	float index_symb_post [L_Matlab_save];
	float index_symb_post2[L_Matlab_save];
	float corrIndex		 [L_Matlab_save];
	float tau0			 [L_Matlab_save]; // 29
	float dapsk2_re		 [L_Matlab_save];
	float dapsk2_im		 [L_Matlab_save];
	float dapsk2_m_re	 [L_Matlab_save];
	float dapsk2_m_im	 [L_Matlab_save]; //33
	float dapsk2_p_re	 [L_Matlab_save];
	float dapsk2_p_im	 [L_Matlab_save]; //35
	float Rdd			 [L_Matlab_save];
	float alpha_normalized [L_Matlab_save];
	float NL_re			 [L_Matlab_save];
	float NL_im			 [L_Matlab_save]; //39
} t_Matlab_save;

// end testMatlab */
  
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
//_____________________________________________________________________________
// Modulator:
//

// TX Timing
void RTSCTStimingDAPSK(t_DAPSK_Modulator *ModulatorData, t_DAPSK_InputConversion *InputData);
// adEq_change add config  void SEGMENT_EXT_CODE InitModulatorDAPSK(t_DAPSK_Modulator *ModulatorData, t_ModemConfig *modemConfig, t_V24PortCfg *v24Config, t_ModemEqualizerConfig *adEqConfig, long iModem); 
void SEGMENT_EXT_CODE InitModulatorDAPSK(t_DAPSK_Modulator *ModulatorData, t_ModemConfig *modemConfig, \
										 t_V24PortCfg *v24Config, t_ModemEqualizerConfig *adEqConfig, long iModem); 
void modulationDAPSK(t_DAPSK_Modulator *ModulatorData, t_DAPSK_InputConversion *InputData);

// TX FPGA Interface
void SEGMENT_EXT_CODE InitTxInputConversionDAPSK(t_ModemConfig *modemConfig, t_DAPSK_InputConversion *InputData,\
												 t_V24PortCfg *v24Config);
void SEGMENT_EXT_CODE txInputConversionDAPSK(long *FPGAtxd, t_DAPSK_InputConversion *InputData, t_DAPSK_Modulator *ModulatorData); // check if bit valid, fifo, mark endofchar
void /*SEGMENT_EXT_CODE*/ txUpdatePointerWriteFifo(t_DAPSK_InputConversion *InputData);
void txUpdatePointerReadFifo(t_DAPSK_InputConversion *InputData);
int  SEGMENT_EXT_CODE txReadSymbolDAPSK(t_DAPSK_InputConversion *InputData);
void SEGMENT_EXT_CODE txFormSymbolDAPSK(t_DAPSK_Modulator *ModulatorData, t_DAPSK_InputConversion *InputData);


//_____________________________________________________________________________
// Demodulator:
//
// adEq_change add config  void SEGMENT_EXT_CODE InitDemodulatorDAPSK(t_DAPSK_Demodulator *DeodulatorData, t_ModemConfig *modemConfig, t_V24PortCfg *v24Config, t_DAPSK_OutputConversion *ConversionData, long iModem); 
void SEGMENT_EXT_CODE InitDemodulatorDAPSK(t_DAPSK_Demodulator *DeodulatorData, t_ModemConfig *modemConfig, \
										   t_V24PortCfg *v24Config, t_DAPSK_OutputConversion *ConversionData,\
										   t_ModemEqualizerConfig *adEqConfig, long iModem); 
float signum(float inFloat);

float fir_f_NoInterrupt( float vIn, float pm vCoeffs[], int   nCoeffs, float vDelay[] );

void demodulatorDAPSK(float *RxDataIn, t_DAPSK_Demodulator *DemodulatorData,\
					  t_DAPSK_OutputConversion *ConversionData);
// uncomment these definition when using testMatlab

/* testMatlab
void demodulatorDAPSK(float *RxDataIn, t_DAPSK_Demodulator *DemodulatorData,\
					  t_DAPSK_OutputConversion *ConversionData,t_Matlab_save *Matlab_save);
// end testMatlab */

void SEGMENT_EXT_CODE executeInitacquisiton2(t_DAPSK_Demodulator *DemodulatorData);

void SEGMENT_EXT_CODE executeInitacquisiton3(t_DAPSK_Demodulator *DemodulatorData);

void SEGMENT_EXT_CODE executeInitacquisiton4(t_DAPSK_Demodulator *DemodulatorData);

void SEGMENT_EXT_CODE executeInitacquisiton5(t_DAPSK_Demodulator *DemodulatorData);

void SEGMENT_EXT_CODE executeInitacquisiton6(t_DAPSK_Demodulator *DemodulatorData);

// RX FPGA Interface
void SEGMENT_EXT_CODE InitRxOutputConversionDAPSK(t_DAPSK_Demodulator *DemodulatorData, \
												  t_DAPSK_OutputConversion *ConversionData);
long rxOutputConversionDAPSK(t_DAPSK_Demodulator *DemodulatorData,\
							 t_DAPSK_OutputConversion *ConversionData);


//_____________________________________________________________________________
// Status:
//
void SEGMENT_EXT_CODE statusDataDAPSK(t_DAPSK_InputConversion *InputConvData, t_DAPSK_Modulator *ModulatorData,\
						t_DAPSK_Demodulator *DemodulatorData, t_NSK_DSP_Status *StatusData, BOOL *nskLinkAlarm, \
						BOOL *nskDCDAlarm,t_NSK_ShortStatus  *NSK_ShortStatus, BOOL *nskRTSalwaysActive, int ModemNo);
#endif
/* End of File ***********************************************************/
