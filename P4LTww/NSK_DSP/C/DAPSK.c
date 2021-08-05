/*-------------------------------------------------------------------------*
 * $Workfile: DAPSK.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : T.Gnus
 * Remarks      :  
 * Purpose      : DAPSK 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSK_DSP/C/DAPSK.c $ 
 * 
 * 46    12-03-23 15:03 Chhamaa
 * rxOutputConversionDAPSK(): exception handling modified.
 * 
 * 44    22.09.08 13:49 Chhamaa
 * 
 * 49    08-07-03 10:01 Chsteic
 * ' - moved idle case from INITACQUISITION3 to INITACQUISITION1
 *  - removed #pragma optimize_off from executeInitacquisiton- functions
 *    for processing load  reasons
 * 
 * 48    08-07-01 16:18 Chsteic
 * ' - minor changes in code for comparison with matlab. No running code
 * of RC5 changed.
 * 
 * 47    08-06-26 9:52 Chtognu
 * Modem initialization spread to more cases to save computing time.
 * Firstloop with a startupdely of 1 cycle again before alarmindication. 
 * 
 * 46    08-06-05 10:33 Chsteic
 * '- increased DAPSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT to
 *   avoid SNR alarm at  start-up
 * 
 * 45    08-06-05 9:42 Chsteic
 * - increased DAPSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT to
 *   avoid SNR alarm at  start-up
 * - DemodulatorData->dapsk=dapsk; for the static equalizer. Only needed 
 *   for channel data logging
 * 
 * 44    08-05-07 14:41 Chtognu
 * Max equalizer length changed (bug fixed), no Rx Signal Alarm adapted
 * (pickup time added), unused variables removed
 * 
 * 43    08-05-02 15:46 Chsteic
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
 * 42    08-04-25 15:50 Chsteic
 * '- changed rho of SNR calculation
 * - inserted DCDON hold  timer for AEQ
 * - removed defines
 * - removed some unused code
 * - limit gain factor to 5 (14dB)
 * 
 * 41    08-04-24 9:23 Chsteic
 * '- inserted counterDCDoffMax
 * - logging max signalQuality in NSK_ShortStatus
 * 
 * 40    08-04-11 13:56 Chsteic
 * '-changed jump in criteria from 0.2 to 0.1
 * - inserted DCDoff counter
 * - changed stepSizeTiming and stepSizeTimingPhase for adaptive EQ
 * - moved P_rcv behind gain
 * - removed Modem 0: DAPSK_AST_BEFORE_EQ=FALSE
 *                Modem 1:  DAPSK_AST_BEFORE_EQ=TRUE
 * - quick fix of NAN in signal quality
 * 
 * 39    08-04-08 14:10 Chsteic
 * inserted  long iexModem in void demodulatorDAPSK()
 * Modem 0: DAPSK_AST_BEFORE_EQ=FALSE
 * Modem 1:  DAPSK_AST_BEFORE_EQ=TRUE
 * 
 * 38    08-04-04 16:25 Chsteic
 * DAPSK_DETE_MIN_POWER_RCV   =     0.0
 * 
 * 37    08-04-03 10:43 Chtognu
 * DAPSK_SAMPLE_STEP input for AST removed
 * 
 * 36    08-04-03 9:43 Chtognu
 * txFormSymbolDAPSK() moved to external memory. Unused variables removed.
 * ad.eq: - new possibility added to seperate AST from ad.eq. Algorithmus
 * (DAPSK_AST_BEFORE_EQ) --> and seperated
 * - DAPSK_SAMPLE_STEP input for AST removed. 
 * - Update CMA back to 0.03
 * 
 * 35    08-03-28 10:31 Chtognu
 * bug fixed in new autonomous AGC for ad.equalizer
 * 
 * 34    08-03-27 8:44 Chtognu
 * new autonomous AGC for ad.equalizer with adaptive eq update conditions
 * drift-control removed
 * 
 * 33    08-03-20 14:27 Chsteic
 * '- inserted first version of a drift-control
 * - logging of non rounded tau
 * 
 * 32    08-03-18 14:42 Chtognu
 * Reduced step size of alpha_CMA (ad.Eq.)
 * 
 * 31    08-03-07 16:53 Chsteic
 * '- #define DAPSK_DETE_MIN_POWER_RCV        0.1
 * - test Flags added
 * - new algorithm
 * - variables renamed
 * - logging of new variables
 * - changed memory map
 * 
 * 29    08-03-06 16:59 Chsteic
 *  - inserted signal clipping
 * - changed EQUpdate_Flag functionality to no jumping back from detect
 * state inserted new ad EQ version of TvH. Functionality is uncommented
 * 
 * 28    08-03-05 11:10 Chsteic
 * ' -inserted logging of realTransmitSignalabsMax,pRcvMin
 *  -minor changes in EQUpdate_Flag logic
 * 
 * 27    08-03-04 10:31 Chsteic
 * '-inserted EQUpdate_Flag. For developing issues used.
 * -inserted NSK_ShortStatus_float and  NSK_ShortStatus_long array
 * - inserted SignalPowerRMax,SignalPowerIMax, normalization_factorMax for
 * the logger
 * 
 * 26    08-02-28 16:58 Chsteic
 * '-tau inserted
 * -changend Testvar, tau is logged now
 * 
 * 25    08-02-28 9:42 Chsteic
 * '- Removed SNRLevelMin from NSK_DSP_STATUS
 * - Changend  static unsigned long TestVar to  static long TestVar
 * - Changend  static unsigned long TestVar to  static long TestVar in
 * NSK_Types.h
 * - added parameter t_NSK_ShortStatus  *NSK_ShortStatus in function
 * statusDataDAPSK()
 * 
 * 24    08-02-25 9:54 Chtognu
 * Merged changes from ad.equ.branch:
 * ad. Equalizer functionality incl. Logger, scrambler, new filter,
 * removed preamble detection, alarm, processing time optimization, global
 * interrupt disabled during DAPSK fir function, FE path expantion
 * 
 * 19    28.10.05 8:52 Chchlee
 * ASSERT bug fix implemented
 * 
 * 18    26.10.05 14:59 Chtognu
 * order of fifo filling changed, number of idle bits after preamble
 * increased
 * 
 * 17    26.10.05 13:32 Chtognu
 * Idle bits after preamble increased, out commented code for
 * "parable"-acquisition added, some initial values added
 * 
 * 16    11.10.05 14:52 Chtognu
 * Ampl & Phase criteria now used for adaptive symbol timing and graymap
 * rotated (CHANGE 6 - 9). real and imag part of Signalpower used now to
 * leave acquisition case, lower level to fall back to acquisiton
 * 
 * 15    26.09.05 18:03 Chtognu
 * CTS always on working with idle bits (power criteria for leaving
 * acquisition)
 * 
 * 14    5.09.05 11:02 Chtognu
 * new end of communication sequenze, new DCD handling names
 * 
 * 13    29.08.05 10:19 Chtognu
 * threshold added for SNR alarm, delay for alarm after DCD active added.
 * After returning to acquisiton state, DCD stays active for a def. time
 * if SNR alarm is active (to prevent oscilation of DCD signal)
 * 
 * 12    22.08.05 8:38 Chtognu
 * stoppbits added at the end of transition
 * 
 * 11    15.08.05 16:04 Chtognu
 * Hardware Handshake now working (removed idletime reduction),
 * 3dB correction for SNR ALARM Level added
 * 
 * 10    16.06.05 17:34 Chtognu
 * #pragma vector_for added
 * SNR Alarm only when DCD active 
 * 
 * 9     8.06.05 13:16 Chtognu
 * #pragma optimize_as_cmd_line at the end of file removed
 * 
 * 8     3.06.05 11:18 Chtognu
 * V24PORT_CFG_MODE_PROTOCOL_BIT changed
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
 * 3     2.03.05 9:52 Chtognu
 * 
 * 2     17.02.05 14:00 Chtognu
 * merged Version with changes done in Prototype 1 till 16.02.05
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 14    7.01.05 14:58 Chtognu
 * Rx path working (Ringbuffer in Tx path not working) 
 * 
 * 13    9.12.04 16:39 Chtognu
 * 
 * 12    2.12.04 8:39 Chtognu
 * First Version with early late
 * 
 * 11    30.11.04 8:30 Gnus01
 * 
 * 10    23.11.04 13:18 Gnus01
 * Acquisition works
 * 
 * 9     9.11.04 16:18 Gnus01
 * 
 * 8     4.11.04 13:06 Gnus01
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/
 
 
/* INCLUDE FILES ***********************************************************/
#include <math.h> // to calculate sqrtf() 
#include <stdlib.h> // for div()
#include "basetype.h"
#include "DIAG.h"
#include "CONSTANT.H"
#include "NSK_DSP_Constant.h"
#include "speeddsp.h"
#include "ADPSK_FilterCoef.h"
#include "DAPSK.h"
asm("#include <def21161.h>");
/* testMatlab
#include "DAPSK_Data.h"
// end testMatlab */

//@CL@ the compiler reports a catastrophic error if ASSERT is defained away as in diag.h
#undef ASSERT
// for debugging
	 	#define ASSERT(condition) haltSystem(!(condition))

/* the error message was as follows:

  At end of source: : internal error: Uncaught exception Assertion failed raised
          at ../../../bril/optimiser/colour.c:2372 Please submit a bug report
          with this message, the command line used, type of machine and the
          output of the compiler when you add -ED -v to the command line.
          Please also send us the pre-processed file that is generated by the
          -ED option (the file generated is named <original_filename>.i)

*/


/* LOCAL CONSTANT DECLARATIONS *********************************************/
#define DAPSK_NO_SUPPORTED_BR           3  
#define DAPSK_DOWNSAMPLING              9    // Downsampling Factor from intermediate Freq. to DSP Freq. ( = DAPSK_OVERSAMPLING)
#define DAPSK_OVERSAMPLING              9    // Oversamgling Factor from DSP Freq. to intermediate Freq. ( = DAPSK_DOWNSAMPLING)

#define DAPSK_2_DELAYLINE               2    // 2 RX Delaylines specified (Matlab nr_delay_lines)
#define DAPSK_1_DELAYLINE               1    // 1 RX Delayline  specified (Matlab nr_delay_lines)

#define DAPSK_MIN_NO_DELAYLINES         1    // Minimum Number of RX Delaylines (Matlab nr_delay_lines)
#define DAPSK_ACQU_NO_SAMPLES           5    // Number of Samples of intermediate Frequency
#define DAPSK_ACQU_L_WINDOW            10    // acqusition sliding window length [symbols]

#define DAPSK_RHO_P_1                  0.0        // Filterweight of the old MEAN value (Matlab: abs(dpsk_old))    
#define DAPSK_RHO_P_2                  1.0 //(1 - DAPSK_RHO_P_1)  // Filterweight of the newest value (Matlab: abs(dpsk_old))

//#define DAPSK_RHO_E_1                  0.995 //0.97       // Filterweight of the old MEAN Error (Matlab: error)    
//#define DAPSK_RHO_E_2                  0.005      //(1 - DAPSK_RHO_E_1)  // Filterweight of the newest value (Matlab: err)
#define DAPSK_RHO_E_1                  0.9995 //0.97       // Filterweight of the old MEAN Error (Matlab: error)    
#define DAPSK_RHO_E_2                  0.0005      //(1 - DAPSK_RHO_E_1)  // Filterweight of the newest value (Matlab: err)

#define DAPSK_DEFAULT_INDEX            1//6     // Selected index in case of acquisition failed

#define DAPSK_ACQU_THR                  1.1    // Value of the detection threshold  
#define DAPSK_ACQU_NR_LEVEL             0.01   // "Noise" Signallevel for Acquisition 
#define DAPSK_ACQU_SR_NR_RATIO          4.0    // Ration between Signal and Nois Level for Acquisition
#define DAPSK_ACQU_MIN_SIGNAL           0.1    // "Data" Signallevel for leaving Acquisiton in case Acquisiton failed and static equalizer is choosen.
											   // -> CHANGE4

//#define DAPSK_DETE_MIN_POWER_RCV        0.32    // minimum "Data" Signalpower for not falling back to Acquisiton
#define DAPSK_DETE_MIN_POWER_RCV        0.1    // minimum "Data" Signalpower for not falling back to Acquisiton
#define DAPSK_DCDOFF_THRES              0.32
#define DAPSK_DCDON_HOLD              60 // [s] time constant used to not jump back to Acuisition before DAPSK_DCDOFF_CNT seconds stayed in Detect state.
										 // Needed to guarantee that the AEQ has enough time to adapt
#define DAPSK_DCDOFF_HOLD             5 // [s] time constant used to not jump back to Acuisition before SNR stayed DAPSK_DCDOFF_HOLD under the thresshold. Used to avoid jump back in case of a trip signal.

#define DAPSK_AD_EQ_AGC_TAU  		0.1		// time constant of power measurement in ad.eq. case
#define DAPSK_AD_EQ_MIN_P_AGC		0.005	// minimal power for updating gain

#define DAPSK_DETE_AGC_SMALL_NR		0.001   // ORIGINAL Multiplier for the adaptive gain control (AGC)

#define DAPSK_ADAP_TAU_START            0.05   // Startvalue for Adaptive Symbol Timing


#define DAPSK_EQUALIZER_INITIAL_TAP 5
#define DAPSK_ALPHA_CMA				0.03
#define DAPSK_ALPHA_NORMALIZED		DAPSK_ALPHA_CMA/DAPSK_AD_EQ_LENGTH

#define DAPSK_STANDARDISATION_FACTOR_9600    1.54
#define DAPSK_STANDARDISATION_FACTOR_4800    2.28
#define DAPSK_STANDARDISATION_FACTOR_2400    3.15

// Receiver switch(DemodulatorData->reception) cases:
#define ACQUISITION                     0
#define DETECT_SYMBOL                   1
#define CALC_EARLY_SYMB                 2
#define CALC_LATE_SYMB                  3
#define ADAPTIV_SYMB_TIME               4 
#define CALC_BEFORE_EARLY_SYMB          5
#define CALC_AFTER_LATE_SYMB            6
// reduce computintime
#define INITACQUISITION1                7
#define INITACQUISITION2                8
#define INITACQUISITION3                9
#define INITACQUISITION4                10
#define INITACQUISITION5                11
#define INITACQUISITION6                12

// end reduce computingtime

#define DAPSK_I_ACQU_INIT              -1    // initial value, cause "DAPSK_I_ACQU_INIT + 1" has to be zero -> delayline[0]

// Bitdelay after Preamble (minimum 4 Bit!)
// PL_change no Preamble #define BITDELAY_AFTER_PREAMBLE_9600    20 
// PL_change no Preamble #define BITDELAY_AFTER_PREAMBLE_4800    20
// PL_change no Preamble #define BITDELAY_AFTER_PREAMBLE_2400    20

// Input Conversion
#define DAPSK_ROW_IN_DATA               0    // TX FIFO Lines with Data information
#define DAPSK_ROW_IN_EOCh               1    // TX FIFO Lines with end of character information

// Mark_EndOfCharacter cases: ASYNCH MODE
#define DAPSK_STATE_EOCH_NOSYNC         0
#define DAPSK_STATE_EOCH_DATABITS       1
#define DAPSK_STATE_EOCH_STOPBITS       2

// Mark end of Telegram: FT1.2 / FT 3 MODE
#define DAPSK_MAX_NO_CONSECUTIVE_ONES  10   // FT1.2 / FT3 number of consecutive ones -> End of Telegram
#define DAPSK_STATE_READTELEGRAM_DATA   0
#define DAPSK_STATE_READTELEGRAM_PAUSE  1

// form Tx Symbol cases:
#define DAPSK_STATE_FORMSYMB_IDLE       0
#define DAPSK_STATE_FORMSYMB_BUSY       1

#define DAPSK_TX_SYMB_4STOPBIT 		   0xF // 4 stop bit 
#define DAPSK_TX_SYMB_3STOPBIT_FIRST   0xE // 3 stop bit
#define DAPSK_TX_SYMB_2STOPBIT_FIRST   0xC // 2 stop bit
#define DAPSK_TX_SYMB_1STOPBIT_FIRST   0x8 // 1 stop bit

#define DAPSK_TX_SYMB_3STOPBIT_LAST    0x7 // 3 stop bit
#define DAPSK_TX_SYMB_2STOPBIT_LAST    0x3 // 2 stop bit
#define DAPSK_TX_SYMB_1STOPBIT_LAST    0x1 // 1 stop bit

#define DAPSK_TX_SYMB_1STOPBIT_2PLACE  0x2 // 1 stop bit
#define DAPSK_TX_SYMB_1STOPBIT_3PLACE  0x4 // 1 stop bit
//#define DAPSK_TX_SYMB_1StopBit_4place  0x8 // 1 stop bit

// FPGA REGISTERMASK
#define DAPSK_FPGA_TXD_NO_BIT		   0x0000007F // no new bit available
#define DAPSK_FPGA_TXD_BIT_VALUE	   0x00000080 // Mask for new bit in TUD -> TXD

// Output Conversion
#define DAPSK_FPGA_UPPERQUANTITY       5.5
#define DAPSK_FPGA_LOWERQUANTITY       4.5
#define DAPSK_FPGA_RISE_600PPM         1.0006 // rise bit length for  600ppm
#define DAPSK_FPGA_REDUCE_600PPM       0.9994 // reduce bit length for  600ppm
// test no bitlength manipulation 
//#define DAPSK_FPGA_RISE_600PPM         1.0 // rise bit length for  600ppm
//#define DAPSK_FPGA_REDUCE_600PPM       1.0 // reduce bit length for  600ppm

#define DAPSK_FPGA_CLOCK_NO_TRANS      0xFF   // No Clock, No Transition
#define DAPSK_FPGA_DATA_NO_TRANS       0x7F   // No DATA  Transition
#define DAPSK_FPGA_DATA_MOVE_DATABIT   128    // Multiplier to move DATA Bit at right position

// STATUS
#define DAPSK_STATUS_SYNCALARM_LEVEL   0.01   // under 20dB
#define DAPSK_STATUS_SNRALARM_LEVEL    0.025   // under 16dB

// SNR Alarm Constants
#define DAPSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT 540  // To output link alarm -> DCD has to be aktive for a 
												   // minimal predefined time
												   // 53 -> ca. 0.5sec. (counting period 0.1067kHz)
#define DAPSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT	540	   // min "DCD off" time before output "NSK600 modem x no Rx signal detected"
												   // 540 -> ca. 5sec. (counting period 0.1067kHz)

#define DAPSK_SNR_LEVEL_CORRECTION_3DB      0.5012 //SNR correction because SNR calculation is 3dB too low
#define DAPSK_SNR_LEVEL_18_5DB_REAL_15_5DB  0.0316 // 18.5dB real dB value 15.5dB (DAPSK SNR calculation 3dB too low) 
#define DAPSK_SNR_LEVEL_21DB_REAL_18DB      0.016  // 21dB   real dB value 18dB (DAPSK SNR calculation 3dB too low) 
#define DAPSK_SNR_LEVEL_15DB_REAL_12DB      0.063   // 15dB   real dB value 12dB (DAPSK SNR calculation 3dB too low) 



// Receiver switch(ConversionData->stateSendDataToFPGA) cases:
#define IDLE_SENDDATATOFPGA            0
#define RECEIVE_SENDDATATOFPGA         1

// TX Data Delay Mask
#define DAPSK_TX_DATADELAY_MASK        0xFFFF

//_________________________________________________________________________________________
// DAPSK Symbol Map
// this is the used graymap[16] = {7,0,6,5,2,1,3,4,15,8,14,13,10,9,11,12};
// newGray CHANGE5
// this is the used graymap[16] = {5,6,4,3,0,7,1,2,13,14,12,11,8,15,9,10};

const float dph = TWO_PI/8;

//const float innerRadius = 0.697; // Kr = 1.765 original
//const float outerRadius = 1.2305; // Kr = 1.765 original
const float innerRadius = 6.970105860684660e-001; // changed SE more exact value
const float outerRadius = 1.230518688565311e+000; // changed SE more exact value

const float innerRadius_squ = 4.858237570915064e-001;  
const float outerRadius_squ = 1.514176242908493e+000; 

//#define DAPSK_DETE_AGC_CENTER_RADIUS    0.9637 // Kr = 1.7654 ORIGINAL Center Radius for adaptive gain control, lokated tween "innerRadius" and "outerRadius"

#define DAPSK_DETE_AGC_CENTER_RADIUS		9.637646373168887e-001 //	((innerRadius+outerRadius)/2)
#define DAPSK_DETE_AGC_CENTER_RADIUS_SQU    9.288422761425540e-001 //(DAPSK_DETE_AGC_CENTER_RADIUS*DAPSK_DETE_AGC_CENTER_RADIUS)

// adEq_change add config
/*
#define DAPSK_R2     (((outerRadius*outerRadius*outerRadius*outerRadius) + (innerRadius*innerRadius*innerRadius*innerRadius))/(outerRadius*outerRadius + innerRadius*innerRadius))
#define DAPSK_Rb0    ((innerRadius*innerRadius)*2/3)
#define DAPSK_Rb1    ((innerRadius*innerRadius) + ((outerRadius*outerRadius) - (innerRadius*innerRadius))/3)
#define DAPSK_Rb2    ((outerRadius*outerRadius) - ((outerRadius*outerRadius) - (innerRadius*innerRadius))/3)
#define DAPSK_Rb3    ((outerRadius*outerRadius)*4/3)     
*/        

const float DAPSK_R2 =1.264377208771494e+000;
const float DAPSK_Rb0=3.238825047276709e-001;
const float DAPSK_Rb1=8.286079190305020e-001;
const float DAPSK_Rb2=1.171392080969498e+000;
const float DAPSK_Rb3=2.018901657211325e+000;


const int mfIndexTable_9600[9][2]= {{7, 3}, 
									{8,	3},
									{0,	2},
									{1,	2},
									{2,	2},
									{3,	2},
									{4,	2},
									{5,	2},
									{6,	2}};       
const int mfIndexTable_4800[9][2]= {{5, 5},
									{6,	5},
									{7,	5},
									{8,	5},
									{0,	4},
									{1,	4},
									{2,	4},
									{3,	4},
									{4,	4}};       
const int mfIndexTable_2400[9][2]= {{1,	9},
									{2,	9},
									{3,	9},
									{4,	9},
									{5,	9},
									{6,	9},
									{7,	9},
									{8,	9},
									{0,	8}};       

      
// PL_change no Preamble const int txFirstSymbols[8] = {1, 0, 0, 1,  1, 1, 1, 1,}; // first symbols for ringbuffer

// calculated with Matlab used in Transmitter //
//      !! depending on graymap !!            //

// newGray CHANGE5
complex_float pm cp[16]           = {	 -7.071067811865477e-001,  -7.071067811865475e-001,
										  0.000000000000000e+000,  -1.000000000000000e+000,
										 -1.000000000000000e+000,   0.000000000000000e+000,
										 -7.071067811865475e-001,  +7.071067811865476e-001,
									      1.000000000000000e+000,	0.000000000000000e+000,
										  7.071067811865474e-001,  -7.071067811865477e-001,
									 	  7.071067811865476e-001,  +7.071067811865475e-001,
										  0.000000000000000e+000,  +1.000000000000000e+000,
										 -7.071067811865471e-001,  -7.071067811865479e-001,
										  0.000000000000000e+000,  -1.000000000000000e+000,
										 -1.000000000000000e+000,   0.000000000000000e+000,
										 -7.071067811865467e-001,  +7.071067811865484e-001,
										  1.000000000000000e+000,   0.000000000000000e+000,
										  7.071067811865466e-001,  -7.071067811865485e-001,
										  7.071067811865477e-001,  +7.071067811865474e-001,
										  0.000000000000000e+000,  +1.000000000000000e+000};
   
// calculated with Matlab used in Receiver, with amplitude //
//  !! depending on graymap, innerRadius, outerRadius !!   //
/*

// old faster way -> removed for transparency reasons now using cp[] also in rx path

complex_float pm cp_Ampl[16]       = {    4.9285342648702346e-001, -4.9285342648702368e-001,
	                                      6.9699999999999995e-001,  0.0000000000000000e+000,
	                                      0.0000000000000000e+000, -6.9699999999999995e-001,
	                                     -4.9285342648702368e-001, -4.9285342648702357e-001,	  
	                                      0.0000000000000000e+000,  6.9699999999999995e-001,
	                                      4.9285342648702363e-001,  4.9285342648702357e-001,	
										 -4.9285342648702357e-001,  4.9285342648702363e-001,
										 -6.9699999999999995e-001,  0.0000000000000000e+000,
										  8.7009489425004549e-001, -8.7009489425004782e-001,	
										  1.2304999999999999e+000,  0.0000000000000000e+000,	 
										  0.0000000000000000e+000, -1.2304999999999999e+000,
										 -8.7009489425004616e-001, -8.7009489425004716e-001,
										  0.0000000000000000e+000,  1.2304999999999999e+000,
										  8.7009489425004682e-001,  8.7009489425004649e-001,
										 -8.7009489425004560e-001,  8.7009489425004771e-001,
										 -1.2304999999999999e+000,  0.0000000000000000e+000};

*/
// calculated with Matlab including preamble //
//   !! depending on preamblebits !!
///*
// Kr = 1.7654 (original) 
/*
complex_float zpfDelayline_9600[11] = {   4.9285342648702540e-001, 4.9285342648702180e-001,
			                              8.7009489425004372e-001,-8.7009489425004960e-001,
                						 -4.9285342648702507e-001,-4.9285342648702207e-001,
			                 			  8.7009489425004438e-001,-8.7009489425004893e-001,
                  						  4.9285342648702463e-001, 4.9285342648702252e-001,
                 						 -1.2304999999999999e+000, 3.2244840101879054e-015,
                						  1.2304999999999999e+000,-3.0737912215528238e-015,
                						 -4.9285342648702424e-001,-4.9285342648702291e-001,
	                					  0.0000000000000000e+000,-1.2304999999999999e+000,
	                					  0.0000000000000000e+000,-1.2304999999999999e+000,
	                					  0.0000000000000000e+000,-1.2304999999999999e+000};
*/
// newGray CHANGE5
// PL_change no Preamble complex_float zpfDelayline_9600[11] = {  4.928609119678238e-001,  +4.928609119678202e-001,	
// PL_change no Preamble										 8.701081090613060e-001,  -8.701081090613119e-001,	
// PL_change no Preamble										-4.928609119678236e-001,  -4.928609119678205e-001,	
// PL_change no Preamble										 8.701081090613066e-001,  -8.701081090613112e-001,	
// PL_change no Preamble										 4.928609119678231e-001,  +4.928609119678210e-001,	
// PL_change no Preamble										-1.230518688565311e+000,  +3.224532982946963e-015,	
// PL_change no Preamble										 1.230518688565311e+000,  -3.073837905622712e-015,	
// PL_change no Preamble										-4.928609119678227e-001,  -4.928609119678214e-001,	
// PL_change no Preamble										 0.000000000000000e+000,  -1.230500000000000e+000,	
// PL_change no Preamble										 0.000000000000000e+000,  -1.230500000000000e+000,	
// PL_change no Preamble										 0.000000000000000e+000,  -1.230500000000000e+000};

// Multiplicator used in Demodulator 
//complex_float demodAngle = {0.923879532511287,  - 0.38268343236509};
// newGray CHANGE5
complex_float demodAngle = {0.38268343236509,    +0.92387953251129};



// Array with values for the bit to decimal conversion used to dedicate the index of the sought cp_Ampl
int ValueArray[DAPSK_M] = { 8, 4, 2, 1};

// Decimal to Binary Table used for scramberl
const int DecTable[16][4] =	       {{0,0,0,0},
									{0,0,0,1},
									{0,0,1,0},
									{0,0,1,1},
									{0,1,0,0},
									{0,1,0,1},
									{0,1,1,0},
									{0,1,1,1},
									{1,0,0,0},
									{1,0,0,1},
									{1,0,1,0},
									{1,0,1,1},
									{1,1,0,0},
									{1,1,0,1},
									{1,1,1,0},
									{1,1,1,1}};

/* LOCAL TYPE DECLARATIONS *************************************************/
/* LOCAL MACRO DEFINITIONS *************************************************/
/* LOCAL VARIABLES *********************************************************/
/* IMPLEMENTATION **********************************************************/

///////////////////////////////////////////////////////////////////////////////////
// DAPSK MODULATOR PART                                                          //
///////////////////////////////////////////////////////////////////////////////////


/*! txtimingDAPSK 

  This function controls the RTS and CTS timing and the switch-off sequenze that
  starts after the RTS disappeares. The switch-off sequenze consists of the below-mentioned steps:
   
	1.	Waiting until delaytimeRTS expires. (This is an additional delay that may be chosen 
		by the User, see Transmit data delay in the HMI)

	2.	Write to the transmit FIFO additional "idle" bits. As soon they will be sent, they 
		are shifting the last data bits out of the TX filter delay line.

	3.	Controlling if the latest transmitted "idle" Symbol is located on the inner circle 
		of the Gray map. If not, add an additional "idle" Symbol. Its important that the last 
		transmitted Symbol is located on the inner circle to secure the receiver data detector 
		from wrong bit detection. If the last transmitted symbol would be located on the outer
		circle the power would be higher and the power breakdown could be interpreted as an 
		additional symbol on the inner gray map circle.
	
	\b Initfunction:

	No

*/
void RTSCTStimingDAPSK(t_DAPSK_Modulator *ModulatorData, t_DAPSK_InputConversion *InputData)
{
	int addbit;

	if(ModulatorData->fpgaRTS)
	{
		ModulatorData->modemRTS = ModulatorData->fpgaRTS;
		InputData->remainedDelaytimeRTS = InputData->delaytimeRTS;
		InputData->addStopbit = TRUE;
		InputData->addStopbit2 = FALSE;
	}
	else if (InputData->remainedDelaytimeRTS > 0)
	{
		InputData->remainedDelaytimeRTS = InputData->remainedDelaytimeRTS - 1;
	}	
	//else if added -> add stoppbits at the end of transition
	else if (InputData->addStopbit)
	{
		// add a defined number of bits (bits/4 = symbols)
		InputData->noOfBits = InputData->noOfBits + 12;
		InputData->i_newestBit = InputData->i_newestBit+12;
		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
		InputData->addStopbit = FALSE;
		InputData->addStopbit2 = TRUE;
	}
	else if (InputData->addStopbit2)
	{
		if (InputData->noOfBits<8)
		{
			addbit = (4-(InputData->noOfBits%4));                       // completing the number of symbols
			addbit = addbit + 4*(((InputData->noOfBits+addbit)%4)%2);   // even number of symbols

			addbit = addbit+ 4*(ModulatorData->stateMSB);               // adding a symbol depending on the last
																		// state of MSB -> make sure last transmitted symbol
																		// will be placed on the inner circle
			InputData->noOfBits = InputData->noOfBits +addbit;
			InputData->i_newestBit = InputData->i_newestBit+addbit;
			InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
			InputData->addStopbit2 = FALSE;
			ModulatorData->modemRTS = FALSE;
		}
		
	}
	else
	{
		ModulatorData->modemRTS = FALSE;
	}
	
	// RTS = CTS for DAPSK Modem
	ModulatorData->CTS = ModulatorData->fpgaRTS;
}

/* Initialize the Modulator *****************************************************/
#pragma optimize_off 
// adEq_change add config void SEGMENT_EXT_CODE InitModulatorDAPSK(t_DAPSK_Modulator *ModulatorData, t_ModemConfig *modemConfig, t_V24PortCfg *v24Config,long iModem)
void SEGMENT_EXT_CODE InitModulatorDAPSK(t_DAPSK_Modulator *ModulatorData, t_ModemConfig *modemConfig, \
										 t_V24PortCfg *v24Config, t_ModemEqualizerConfig *adEqConfig,long iModem)

{
	long splitBwTxDataDelay;
	int i_scrambler;

	ModulatorData->mode = modemConfig->mode;
	splitBwTxDataDelay = modemConfig->bw_txDataDelay;
	ModulatorData->baud = v24Config->datarate;
	ModulatorData->bandwidth = ((splitBwTxDataDelay >> 16)& 0xFFFF );
	
	ModulatorData->txfrequency = ((modemConfig->centerFrequency >> 16)& 0xFFFF );  

	ModulatorData->rSymb =  ModulatorData->baud / DAPSK_M; 
	ModulatorData->nss = DAPSK_F_INTERM/ModulatorData->rSymb; 

	ModulatorData->transmission = 0;

	ModulatorData->statePhi.re = 0.707107;
	ModulatorData->statePhi.im = 0.707107;
	ModulatorData->stateMSB = 0;
	ModulatorData->nssIndex = 0;

	if (ModulatorData->baud == 9600)
	{
		ModulatorData->plFilterCoef = &txPolyphase_9600[0][0];
		// Delayline filled up with Preamble
// PL_change no Preamble		MEMCPY(ModulatorData->zpfDelayline, zpfDelayline_9600, sizeof(zpfDelayline_9600));
		// add standardisation to outputgain
		ModulatorData->txOutputGain = DAPSK_STANDARDISATION_FACTOR_9600;
	}
	else if (ModulatorData->baud == 4800)
	{
		ModulatorData->plFilterCoef = &txPolyphase_4800[0][0];
		// Delayline filled up with Preamble
// PL_change no Preamble		MEMCPY(ModulatorData->zpfDelayline, zpfDelayline_9600, sizeof(zpfDelayline_9600));
		// add standardisation to outputgain
		ModulatorData->txOutputGain = DAPSK_STANDARDISATION_FACTOR_4800;
	}
	else if (ModulatorData->baud == 2400)
	{
		ModulatorData->plFilterCoef = &txPolyphase_2400[0][0];
		// Delayline filled up with Preamble
// PL_change no Preamble		MEMCPY(ModulatorData->zpfDelayline, zpfDelayline_9600, sizeof(zpfDelayline_9600));
		// add standardisation to outputgain
		ModulatorData->txOutputGain = DAPSK_STANDARDISATION_FACTOR_2400;
	}
	else
	{	ASSERT(FALSE); // wrong 
	}

	ModulatorData->outPolyphase.re = 0;
	ModulatorData->outPolyphase.im = 0;
	ModulatorData->realTransmitSignal = 0;
	ModulatorData->realTransmitSignalabsMax=0;

	// Initialising	of the UPMODULATION oscillator:
	oscillator_init(1.0, ModulatorData->txfrequency, AF_SAMPLE_FREQ, &ModulatorData->txOsc);

    // multiply weight to outputgain
	ModulatorData->txOutputGain = ModulatorData->txOutputGain * modemConfig->weight;


// adEq_change add config
//______________________________________________________________________________________

	if ((adEqConfig->mode && AD_EQ_MODE_MASK) != AD_EQ_MODE_OFF)
	{	
		ModulatorData->adEqOn = TRUE;
		ModulatorData->scramblerOn = TRUE;
		ModulatorData->difMSB = FALSE;
	}
	else
	{	
		ModulatorData->adEqOn = FALSE;
		ModulatorData->scramblerOn = FALSE;
		ModulatorData->difMSB = TRUE;
	}

	// scrambler
	for (i_scrambler = 0; i_scrambler <DAPSK_SCRAMBLER_LENGTH; i_scrambler++)
	{
		ModulatorData->adEqScramblerLine[i_scrambler]= 0;
	}
	ModulatorData->adEqScramblerLine[0]= 1;
	ModulatorData->adEqScramblerLine[1]= 1;

	ModulatorData->scInputPointer = 0;
	ModulatorData->scramblerXorP1 = 0;
	ModulatorData->scramblerXorP2 = 1;



//______________________________________________________________________________________
// adEq_change add config end

}
#pragma optimize_as_cmd_line 


/*! DAPSK Symbol for Modulation 

  This function is forming the compex value of the transmitted symbol. 
  Each symbol consists of 4 bits which defining one point on the Gray map.
  
  \b Initfunction:

  No
										 
*/
#pragma optimize_off 
void SEGMENT_EXT_CODE txFormSymbolDAPSK(t_DAPSK_Modulator *ModulatorData, t_DAPSK_InputConversion *InputData)
{
	int decData;
	int MSB;
	float amplitude;
	int testdummy;
	complex_float newstatePhi;

    //scrambler variables
	int i_scrambler;
	int scramblerNewBit;
	int scramblerReturn;

	decData = txReadSymbolDAPSK(InputData); // convert n bits to decimal


	// TX Scrambler function //
	if(ModulatorData->scramblerOn)
	{
		scramblerReturn = 0;
		for (i_scrambler = 0; i_scrambler < DAPSK_M; i_scrambler++)
		{
			scramblerNewBit = DecTable[decData][i_scrambler];
			scramblerNewBit = scramblerNewBit^ModulatorData->adEqScramblerLine[ModulatorData->scramblerXorP1];
			scramblerNewBit = scramblerNewBit^ModulatorData->adEqScramblerLine[ModulatorData->scramblerXorP2];
		
			// update Scrambler delayline
			ModulatorData->adEqScramblerLine[ModulatorData->scInputPointer] = scramblerNewBit;
			
			scramblerReturn = scramblerReturn + (scramblerNewBit*ValueArray[i_scrambler]);

			// update pointer
			ModulatorData->scInputPointer = ModulatorData->scInputPointer + 1;
			ModulatorData->scramblerXorP1 = ModulatorData->scramblerXorP1 + 1;
			ModulatorData->scramblerXorP2 = ModulatorData->scramblerXorP2 + 1;

			if(ModulatorData->scInputPointer >= DAPSK_SCRAMBLER_LENGTH)
			{  ModulatorData->scInputPointer = 0;
			}
			if(ModulatorData->scramblerXorP1 >= DAPSK_SCRAMBLER_LENGTH)
			{  ModulatorData->scramblerXorP1 = 0;
			}
			if(ModulatorData->scramblerXorP2 >= DAPSK_SCRAMBLER_LENGTH)
			{  ModulatorData->scramblerXorP2 = 0;
			}
		}
		decData = scramblerReturn;
	}


	newstatePhi.re =  (cp[decData].re*ModulatorData->statePhi.re) - (cp[decData].im*ModulatorData->statePhi.im);
	newstatePhi.im =  (cp[decData].re*ModulatorData->statePhi.im) + (cp[decData].im*ModulatorData->statePhi.re);
	ModulatorData->statePhi.re = newstatePhi.re;
	ModulatorData->statePhi.im = newstatePhi.im;


	MSB = (decData > 7); // if decimal data more than 7 -> MSB = 1


	if(ModulatorData->difMSB)
	{
		ModulatorData->stateMSB = ModulatorData->stateMSB ^ MSB; // differential modulation of amplitude 
	}
	else
	{
		ModulatorData->stateMSB = MSB;
	}

	if (ModulatorData->stateMSB == 1)
	{	ModulatorData->inPolyphase.re = ModulatorData->statePhi.re * outerRadius; // modulated signal (angle and amplitude)
		ModulatorData->inPolyphase.im = ModulatorData->statePhi.im * outerRadius; // modulated signal (angle and amplitude)
	}
	else
	{	ModulatorData->inPolyphase.re = ModulatorData->statePhi.re * innerRadius; // modulated signal (angle and amplitude)
		ModulatorData->inPolyphase.im = ModulatorData->statePhi.im * innerRadius; // modulated signal (angle and amplitude)
	}
}
#pragma optimize_as_cmd_line 


/*! DAPSK Modulation

  This function includes the signal processing part of the DAPSK modulation described 
  in [4] and the Matlab model [xx]. The function will only be called when sendDataFlag 
  is true. SendDataFlag is controlled by the function txInputConversionDAPSK().
  
  \b Initfunction:
  
  InitModulatorDAPSK(), called in initModems()

*/
void modulationDAPSK(t_DAPSK_Modulator *ModulatorData, t_DAPSK_InputConversion *InputData)
{
	int i_copy;
	int StayInWait;

	//complex_float  upmodComplex;
	float *p_polyFilter;
	float  polyFilter[DAPSK_L_Delay+1];

	pm static float  delayLineReal[DAPSK_L_Delay+1];
	pm static float  delayLineImag[DAPSK_L_Delay+1];

	if(ModulatorData->sendDataFlag)
	{
		if(ModulatorData->newSymb) // if new symbol to transmit
		{

			// if end of transmission, write zeros to delayline
			// else read complex symbol from buffer
			if (InputData->waitDelayLine)
			{
				StayInWait = InputData->waitDelayLineCount < (DAPSK_L_Delay-2); // reduced by 2 (empirical)

				InputData->waitDelayLine = StayInWait;
				InputData->waitDelayLineCount = InputData->waitDelayLineCount + 1;
				InputData->waitDelayLineCount = InputData->waitDelayLineCount * StayInWait;

				ModulatorData->inPolyphase.re = 0;
				ModulatorData->inPolyphase.im = 0;
			}
			else
			{	txFormSymbolDAPSK(ModulatorData, InputData); // modulate complex symbol including reading from TX-RINGBUFFER
			}

			// shift delayline
			for (i_copy = (DAPSK_L_Delay); i_copy>0; i_copy--)
			{ 
				ModulatorData->zpfDelayline[i_copy] = ModulatorData->zpfDelayline[i_copy-1];

			}
			// new symbol at begining
			ModulatorData->zpfDelayline[0] = ModulatorData->inPolyphase;
		}


		delayLineReal[0] = ModulatorData->zpfDelayline[0].re;
		delayLineImag[0] = ModulatorData->zpfDelayline[0].im;
		delayLineReal[1] = ModulatorData->zpfDelayline[1].re;
		delayLineImag[1] = ModulatorData->zpfDelayline[1].im;
		delayLineReal[2] = ModulatorData->zpfDelayline[2].re;
		delayLineImag[2] = ModulatorData->zpfDelayline[2].im;
		delayLineReal[3] = ModulatorData->zpfDelayline[3].re;
		delayLineImag[3] = ModulatorData->zpfDelayline[3].im;
		delayLineReal[4] = ModulatorData->zpfDelayline[4].re;
		delayLineImag[4] = ModulatorData->zpfDelayline[4].im;
		delayLineReal[5] = ModulatorData->zpfDelayline[5].re;
		delayLineImag[5] = ModulatorData->zpfDelayline[5].im;
		delayLineReal[6] = ModulatorData->zpfDelayline[6].re;
		delayLineImag[6] = ModulatorData->zpfDelayline[6].im;
		delayLineReal[7] = ModulatorData->zpfDelayline[7].re;
		delayLineImag[7] = ModulatorData->zpfDelayline[7].im;
		delayLineReal[8] = ModulatorData->zpfDelayline[8].re;
		delayLineImag[8] = ModulatorData->zpfDelayline[8].im;
		delayLineReal[9] = ModulatorData->zpfDelayline[9].re;
		delayLineImag[9] = ModulatorData->zpfDelayline[9].im;
		delayLineReal[10] = ModulatorData->zpfDelayline[10].re;
		delayLineImag[10] = ModulatorData->zpfDelayline[10].im;


		//polyphase pulseshape filter (pointer to right row)
		p_polyFilter = ModulatorData->plFilterCoef + (ModulatorData->nssIndex*(DAPSK_L_Delay+1));
		ModulatorData->outPolyphase.re = vdot_f_PD(delayLineReal, p_polyFilter, (DAPSK_L_Delay+1));
		ModulatorData->outPolyphase.im = vdot_f_PD(delayLineImag, p_polyFilter, (DAPSK_L_Delay+1));

		// Indexupdating
		ModulatorData->nssIndex = ModulatorData->nssIndex + DAPSK_OVERSAMPLING;
		ModulatorData->newSymb = (ModulatorData->nssIndex >= ModulatorData->nss); // if nssIndex more than nss -> newSymbol = 1
		ModulatorData->nssIndex = ModulatorData->nssIndex - (ModulatorData->newSymb * ModulatorData->nss);
	}
	// CHANGE: Datasensitive
	else
	{
		ModulatorData->stateMSB = 0; 
		ModulatorData->statePhi.re = 0.707107;
		ModulatorData->statePhi.im = 0.707107;
		ModulatorData->nssIndex = 0;
		ModulatorData->newSymb = 1;
	}
   // end CHANGE: Datasensitive

	// Upmodulation
	oscillator_oscC(&ModulatorData->outOsc, &ModulatorData->txOsc, 1);



	ModulatorData->realTransmitSignal = ModulatorData->sendDataFlag*2*((ModulatorData->outOsc.re * ModulatorData->outPolyphase.re) - (ModulatorData->outOsc.im * ModulatorData->outPolyphase.im));
	ModulatorData->realTransmitSignal = ModulatorData->realTransmitSignal * ModulatorData->txOutputGain; // multiply by standardisation Factor and weight

	if(ModulatorData->realTransmitSignalabsMax<fabs(ModulatorData->realTransmitSignal))
	{
		ModulatorData->realTransmitSignalabsMax=fabs(ModulatorData->realTransmitSignal);
	}

	// test: RMS of signal
	//ModulatorData->testquadrms = ModulatorData->testquadrms + (ModulatorData->realTransmitSignal * ModulatorData->realTransmitSignal); 
	//if (ModulatorData->testrmscounter < 10000)
	//{ 
	//	ModulatorData->testrmscounter = ModulatorData->testrmscounter + 1;
	//}
	//else 
	//{
	//	ModulatorData->testrms = sqrt(ModulatorData->testquadrms / 10000);
	//	ModulatorData->testrmscounter = 0;
	//	ModulatorData->testquadrms = 0.0;
	//}
	// Calculate maximum Value of real Transmited Signal
	// ModulatorData->maxReal = fmaxf(ModulatorData->realTransmitSignal, ModulatorData->maxReal);
	// ModulatorData->minReal = fminf(ModulatorData->realTransmitSignal, ModulatorData->minReal);
}


/* Initialize the txInputConversion **********************************************/
#pragma optimize_off 
void SEGMENT_EXT_CODE InitTxInputConversionDAPSK(t_ModemConfig *modemConfig, t_DAPSK_InputConversion *InputData , t_V24PortCfg *v24Config)
{

	int i_NO_fifo;
	int i_L_fifo;
	int i_preamble;

	// Initialising	of the RTS DELAY & calculation of the NUMBER OF DELAYED DATABITS
	InputData->delaytimeRTS = (int)((AF_SAMPLE_FREQ/1000) * (modemConfig->bw_txDataDelay & DAPSK_TX_DATADELAY_MASK));

	InputData->remainedDelaytimeRTS = InputData->delaytimeRTS;

	InputData->noDelayedDatabits = (int)((v24Config->datarate * InputData->delaytimeRTS) / AF_SAMPLE_FREQ);
	if (InputData->noDelayedDatabits >= DAPSK_L_IN_FIFO)
	{
		ASSERT(FALSE); // wrong 
	}

	////////////////////////////////////////////////////////////////
	// set fifobuffer data array to idle and endofchararray to zero
	////////////////////////////////////////////////////////////////
	for (i_L_fifo = 0; i_L_fifo < DAPSK_L_IN_FIFO; i_L_fifo++)
	{
		InputData->txDataFIFO[0][i_L_fifo] = 1;
	}
	for (i_L_fifo = 0; i_L_fifo < DAPSK_L_IN_FIFO; i_L_fifo++)
	{
		InputData->txDataFIFO[1][i_L_fifo] = 0;
	}


	/////////////////////////////////////////////////////
	// set fifo pointer to first value
	/////////////////////////////////////////////////////

	InputData->noOfBits = 0;
	InputData->i_oldestBit = 0;
	InputData->i_newestBit = InputData->i_oldestBit;

	
	/////////////////////////////////////////////////////
	// get number of info bits in char (if asynchronous protocol used)
	/////////////////////////////////////////////////////
	if (((v24Config->control && V24PORT_CFG_MODE_PROTOCOL_BIT_0) != V24PORT_CFG_MODE_PROTOCOL_BIT_0) \
		&&((v24Config->control && V24PORT_CFG_MODE_PROTOCOL_BIT_1) != V24PORT_CFG_MODE_PROTOCOL_BIT_1))
	{
		InputData->asynchronousProtocol = TRUE;
		InputData->stateMarkEndOfChar = DAPSK_STATE_EOCH_NOSYNC;

		InputData->noOfInfoBits = (	(1 * ((v24Config->control & V24PORT_CFG_MODE_DATA_BITS_BIT0) == V24PORT_CFG_MODE_DATA_BITS_BIT0)) + \
									(2 * ((v24Config->control & V24PORT_CFG_MODE_DATA_BITS_BIT1) == V24PORT_CFG_MODE_DATA_BITS_BIT1)) + \
									(4 * ((v24Config->control & V24PORT_CFG_MODE_DATA_BITS_BIT2) == V24PORT_CFG_MODE_DATA_BITS_BIT2)) + \
									(8 * ((v24Config->control & V24PORT_CFG_MODE_DATA_BITS_BIT3) == V24PORT_CFG_MODE_DATA_BITS_BIT3)) + \
								    (1 * ((v24Config->control & V24PORT_CFG_MODE_PARITY_BITS)!=0)) );
	}
	else
	{
		InputData->asynchronousProtocol = FALSE;
		InputData->noOfInfoBits = 0;  // reset value in FT1.2 / FT3 mode
	}

	/////////////////////////////////////////////////////
	// set state for FormSymbol
	/////////////////////////////////////////////////////
	InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

	/////////////////////////////////////////////////////
	// Load Preamble into FIFO
	/////////////////////////////////////////////////////
// PL_change no Preamble	for (i_preamble = 0; i_preamble<sizeof(txFirstSymbols); i_preamble++)
// PL_change no Preamble	{
// PL_change no Preamble		InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txFirstSymbols[i_preamble];
// PL_change no Preamble		InputData->noOfBits = InputData->noOfBits +1;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit+1;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
// PL_change no Preamble	}

	// "safety-idle" bits for the case the acquisiton will be left with succesful correlation within 
	// the second part of the preamble (important for "datasensitive" and handshake mode)
// PL_change no Preamble	if (v24Config->datarate == 9600)
// PL_change no Preamble	{
// PL_change no Preamble		InputData->noOfDealyBitsAfterPreamble = BITDELAY_AFTER_PREAMBLE_9600;
// PL_change no Preamble		InputData->noOfBits =InputData->noOfBits + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
// PL_change no Preamble	}
// PL_change no Preamble	else if (v24Config->datarate == 4800)
// PL_change no Preamble	{
// PL_change no Preamble		InputData->noOfDealyBitsAfterPreamble = BITDELAY_AFTER_PREAMBLE_4800;
// PL_change no Preamble		InputData->noOfBits =InputData->noOfBits + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
// PL_change no Preamble	}
// PL_change no Preamble	else if(v24Config->datarate == 2400)
// PL_change no Preamble	{
// PL_change no Preamble		InputData->noOfDealyBitsAfterPreamble = BITDELAY_AFTER_PREAMBLE_2400;
// PL_change no Preamble		InputData->noOfBits =InputData->noOfBits + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
// PL_change no Preamble	}
// PL_change no Preamble	else
// PL_change no Preamble	{
// PL_change no Preamble	}

	/////////////////////////////////////////////////////
	// Load DATATRANSMIT WAIT into FIFO
	/////////////////////////////////////////////////////
	InputData->noOfBits = InputData->noOfBits + InputData->noDelayedDatabits;
	InputData->i_newestBit = InputData->i_newestBit + InputData->noDelayedDatabits;
	InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;

	/////////////////////////////////////////////////////
	// reset Buffer over/underflow for HMI Statuscounter 
	/////////////////////////////////////////////////////
	InputData->bufferOverflowCounter = 0;
	InputData->bufferUnderflowCounter = 0;
}
#pragma optimize_as_cmd_line 

/*! Tx Input Conversion 

  To transform input data. Two functions will be executed:
	-	If a RTS signal is pending: checking if a valid bit is detected. If so, the async to sync conversion described in [9] is processed. The data is written to the TX input FIFO.
	-	After the RTS disappears the switch-off sequence consists of the below-mentioned steps is executed:
		-#	waiting until the TX input buffer is empty
		-#	waiting until the delay line is empty, see also the function "RTSCTStimingDAPSK". Important flag: waitDelayLine.
		-#	Reset of all needed variables and preparing for next data transmission:
			-	Loading part of the preamble into the delayline
			-	Reset FIFO pointer
			-	Loading rest of preamble into the TX FIFO
			-	Loading additional "idle" bits into TX FIFO
			-	Loading data transmit bits into TX FIFO
		-#	waiting for next RTS

  \b Initfunction:

  InitTxInputConversionDAPSK(), called in initModems()

*/
#pragma optimize_off 
void SEGMENT_EXT_CODE txInputConversionDAPSK(long *FPGAtxd,  t_DAPSK_InputConversion *InputData, t_DAPSK_Modulator *ModulatorData) 
// check if bit valid, fifo, mark endofchar
{
	int txDataBit;
	int i_preamble;

	if (ModulatorData->modemRTS)
	{
		ModulatorData->sendDataFlag = TRUE;
		// CHANGE: Datasensitive
		InputData->waitDelayLineEnd = 0;
		InputData->waitDelayLine    = 0;
		InputData->waitDelayLineCount = 0;
		// end CHANGE: Datasensitive

		if ((*FPGAtxd == 0x7F) || (*FPGAtxd == 0xFF))
		{
			// do nothing if no bit available
		}
		else
		{
			// Check DATA Bit Value
			if (*FPGAtxd >= DAPSK_FPGA_TXD_BIT_VALUE) // TRUE: a "1" was dedected
			{ txDataBit = 1;}
			else
			{ txDataBit = 0;}

			if (InputData->asynchronousProtocol)
			/////////////////////////////////////////////////////
			// ASYNCHRONUS MODE -> Mark_EndOfCharacter
			/////////////////////////////////////////////////////
			{
				/////////////////////////////////////////////////////
				// Send_data_to_FPGA
				/////////////////////////////////////////////////////
				switch (InputData->stateMarkEndOfChar)
				{
					/////////////////////////////////////////////////////	
					case DAPSK_STATE_EOCH_NOSYNC:
					/////////////////////////////////////////////////////	
						if (txDataBit == 0)
						{
							InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
							InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_newestBit] = 0;
							InputData->characterBitCounter = InputData->noOfInfoBits;
							InputData->stateMarkEndOfChar  = DAPSK_STATE_EOCH_DATABITS;
							txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
						}

					break;

					/////////////////////////////////////////////////////		
					case DAPSK_STATE_EOCH_DATABITS:
					/////////////////////////////////////////////////////
						InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
						if(InputData->characterBitCounter == 0)
						{
							if(txDataBit == 0)
							{
								InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_newestBit] = 0;
								InputData->stateMarkEndOfChar = DAPSK_STATE_EOCH_NOSYNC;
							}
							else
							{
								InputData->firstStopBit = TRUE;
								InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_newestBit] = 1;
								InputData->stateMarkEndOfChar = DAPSK_STATE_EOCH_STOPBITS;
							}
						}
						else
						{
							InputData->characterBitCounter = (InputData->characterBitCounter - 1);
							InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_newestBit] = 0;
						}
						txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
					break;
					/////////////////////////////////////////////////////		
					case DAPSK_STATE_EOCH_STOPBITS:
					/////////////////////////////////////////////////////		
						if(txDataBit == 0)
						{
							InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
							InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_newestBit] = 0;
							InputData->characterBitCounter = InputData->noOfInfoBits;
							InputData->stateMarkEndOfChar = DAPSK_STATE_EOCH_DATABITS;
							txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
						}
						else if (InputData->firstStopBit)
						{
							InputData->firstStopBit = FALSE;
							InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
							InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_newestBit] = 1;
							txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
						}
						else
						{
							InputData->stateMarkEndOfChar = DAPSK_STATE_EOCH_NOSYNC;
						}
				
					break;	

					/////////////////////////////////////////////////////
					default:
					/////////////////////////////////////////////////////
						 ASSERT(FALSE); // wrong	

					break;
				}; // end switch(InputData->stateMarkEndOfChar)

			}
			else
			/////////////////////////////////////////////////////
			// FT1.2/FT3 MODE -> Mark_EndOfFrame
			/////////////////////////////////////////////////////
			{
				/////////////////////////////////////////////////////
				// Read Telegram
				/////////////////////////////////////////////////////
				switch (InputData->stateReadTelegram)
				{

					/////////////////////////////////////////////////////	
					case DAPSK_STATE_READTELEGRAM_DATA:
					/////////////////////////////////////////////////////
						if (ModulatorData->modemRTS == 0)
						{
							InputData->stateReadTelegram = DAPSK_STATE_READTELEGRAM_PAUSE;
						}
						else if (txDataBit == 0)
						{						
							InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
							InputData->characterBitCounter = DAPSK_MAX_NO_CONSECUTIVE_ONES;
							txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
						}
						else if (InputData->characterBitCounter == 0)
						{
							InputData->stateReadTelegram = DAPSK_STATE_READTELEGRAM_PAUSE;
						}
						else
						{
							InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
							InputData->characterBitCounter = (InputData->characterBitCounter - 1);
							txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
						}

					break;

					/////////////////////////////////////////////////////		
					case DAPSK_STATE_READTELEGRAM_PAUSE:
					/////////////////////////////////////////////////////
						if((txDataBit == 0)&&(ModulatorData->modemRTS == 1))
						{
							InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txDataBit; // TxDATA_Fifo.in = data_bit_fm_FPGA
							InputData->characterBitCounter = DAPSK_MAX_NO_CONSECUTIVE_ONES;
							InputData->stateReadTelegram = DAPSK_STATE_READTELEGRAM_DATA;
							txUpdatePointerWriteFifo(InputData); // Update Pointer at the end
						}
					
					break;

					/////////////////////////////////////////////////////
					default:
					/////////////////////////////////////////////////////
						 ASSERT(FALSE); // wrong 
					break;
				}; // end switch(InputData->stateReadTelegram)

				// -> TX symbol required is removed!
			}
		}
	}
	else if (InputData->noOfBits)
	// -> if there are bits left in the buffer
	{ // do nothing

	}
	// CHANGE: Datasensitive
	else if ((InputData->waitDelayLine == 0) && (InputData->waitDelayLineEnd == 0))
	{
		InputData->waitDelayLine = 1;
	}
	else if (InputData->waitDelayLine)
	{
		InputData->waitDelayLineEnd = 1;
	}
   // end CHANGE: Datasensitive
	else if (ModulatorData->sendDataFlag)
	// -> Reset Modulator (preparing for next transmission)
	{	
	
		// reset Flag (do this case only once)
		ModulatorData->sendDataFlag = FALSE;

		//Load Preamble into Filter and Buffer:
		// Delayline filled up with Preamble
// PL_change no Preamble		MEMCPY(ModulatorData->zpfDelayline, zpfDelayline_9600, sizeof(zpfDelayline_9600));

		// set fifo pointer to first value
		InputData->i_newestBit = 0;
		InputData->i_oldestBit = InputData->i_newestBit;
		InputData->noOfBits = 0;
	
		// set state for FormSymbol
		InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

		// Load Preamble into FIFO
// PL_change no Preamble		for (i_preamble = 0; i_preamble<sizeof(txFirstSymbols); i_preamble++)
// PL_change no Preamble		{
// PL_change no Preamble			InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_newestBit] = txFirstSymbols[i_preamble];
// PL_change no Preamble			InputData->noOfBits = InputData->noOfBits +1;
// PL_change no Preamble			InputData->i_newestBit = InputData->i_newestBit+1;
// PL_change no Preamble			InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
// PL_change no Preamble		}

		// "safety-idle" bits for the case the acquisiton will be left with succesful correlation within 
		// the second part of the preamble (important for "datasensitive" and handshake mode
// PL_change no Preamble		InputData->noOfBits =InputData->noOfBits + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit + InputData->noOfDealyBitsAfterPreamble;
// PL_change no Preamble		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;

		// Load DATATRANSMIT DELAY into FIFO
		InputData->noOfBits = InputData->noOfBits + InputData->noDelayedDatabits;
		InputData->i_newestBit = InputData->i_newestBit + InputData->noDelayedDatabits;
		InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
		
	}
	else 
	// -> wait for RTS (all bits send & modulator prepared for next transmission)
	{	// do nothing
	}
}
#pragma optimize_as_cmd_line 

/* Tx Update pointer after writing Bit to FIFO ****************************/
//#pragma optimize_off 
void /*SEGMENT_EXT_CODE*/ txUpdatePointerWriteFifo(t_DAPSK_InputConversion *InputData)
{
	InputData->i_newestBit = InputData->i_newestBit+1;
	InputData->i_newestBit = InputData->i_newestBit % DAPSK_L_IN_FIFO;
	InputData->noOfBits = InputData->noOfBits +1;
	if (InputData->noOfBits > DAPSK_L_IN_FIFO)
	{
		// Bufferoverflow 
		// update counter for HMI Status
		InputData->bufferOverflowCounter = InputData->bufferOverflowCounter +1;

		// Resest Buffervariables
		InputData->i_newestBit = 0;
		InputData->i_oldestBit = InputData->i_newestBit;
		InputData->noOfBits = 0;
	}
}
//#pragma optimize_as_cmd_line 

/* Tx Update pointer after reading Bit from FIFO ****************************/
void txUpdatePointerReadFifo(t_DAPSK_InputConversion *InputData)
{
	// reset value of Buffer to "idle bits" (!!! important for  datadelay!!!)
	InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit] = 1;
	InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit] = 0;
	
	// update pointer
	InputData->i_oldestBit = InputData->i_oldestBit + 1;
//	InputData->i_oldestBit = InputData->i_oldestBit % DAPSK_L_IN_FIFO;
	if(InputData->i_oldestBit >= DAPSK_L_IN_FIFO)
	{ 	InputData->i_oldestBit = 0; 
	}

	InputData->noOfBits = InputData->noOfBits - 1;
	if(InputData->noOfBits < 0)
	{
		// Bufferunderflow
		// update counter for HMI Status
		InputData->bufferUnderflowCounter = InputData->bufferUnderflowCounter +1;
		
		// Resest Buffervariables
		InputData->i_newestBit = 0;
		InputData->i_oldestBit = InputData->i_newestBit;
		InputData->noOfBits = 0;
	
	}
}

/*! Tx form symbol 

	This function reads symbols from the TX FIFO. If necessary some of the 4 bits which 
	are forming the symbol are not data bits but "idle" bits. For detailed algorithmus 
	description see [9].
	
  \b Initfunction:
  
  No

*/
#pragma optimize_off 
int SEGMENT_EXT_CODE txReadSymbolDAPSK(t_DAPSK_InputConversion *InputData) 
{
	int ReturnSymbol;
	int EoCh2, EoCh3, EoCh4;
	int Data2, Data3, Data4;

	if (InputData->asynchronousProtocol)
		/////////////////////////////////////////////////////
		// ASYNCHRONUS MODE -> Form_TxSymbol_Asynchronous
		/////////////////////////////////////////////////////
		{
			switch (InputData->stateFormSymbol)
			{
			/////////////////////////////////////////////////////	
			case DAPSK_STATE_FORMSYMB_IDLE:
			/////////////////////////////////////////////////////	
		
				switch (InputData->noOfBits)
				{
					/////////////////////////////////////////////////////
					case 0:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////	
					
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;

					break;

					/////////////////////////////////////////////////////		
					case 1:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
				
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;
						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;

					break;
					/////////////////////////////////////////////////////		
					case 2:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = (DAPSK_TX_SYMB_3STOPBIT_FIRST + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);
						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;

					break;
					/////////////////////////////////////////////////////		
					case 3:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = (DAPSK_TX_SYMB_2STOPBIT_FIRST + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
						ReturnSymbol = (ReturnSymbol + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;

					break;
					/////////////////////////////////////////////////////		
					case 4:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = (DAPSK_TX_SYMB_1STOPBIT_FIRST + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
					
						ReturnSymbol = (ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = (ReturnSymbol + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;

					break;
					/////////////////////////////////////////////////////
					default:
					/////////////////////////////////////////////////////
						ReturnSymbol = (8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);
				
						ReturnSymbol = (ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
					
						ReturnSymbol = (ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = (ReturnSymbol + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;

					break;
				}; // end switch(InputData->noOfBits)

			break;

			/////////////////////////////////////////////////////		
			case DAPSK_STATE_FORMSYMB_BUSY:
			/////////////////////////////////////////////////////
				switch (InputData->noOfBits)
				{
					/////////////////////////////////////////////////////
					case 0:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////	
			
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;


						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

					break;

					/////////////////////////////////////////////////////		
					case 1:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
				
						ReturnSymbol = (DAPSK_TX_SYMB_3STOPBIT_LAST + (8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
					
						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

					break;
					/////////////////////////////////////////////////////		
					case 2:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
					
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						EoCh2 = InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit];

						txUpdatePointerReadFifo(InputData);

						if (EoCh2 || InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit])
						{
							ReturnSymbol = ReturnSymbol + DAPSK_TX_SYMB_3STOPBIT_LAST;
						}
						else
						{
							ReturnSymbol = ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]) + DAPSK_TX_SYMB_2STOPBIT_LAST;
							txUpdatePointerReadFifo(InputData);

							InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;
						}


					break;
					/////////////////////////////////////////////////////		
					case 3:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
					
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						EoCh3 = InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);

						Data2 = InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						EoCh2 = InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);


						if(EoCh3)
						{
							ReturnSymbol = ReturnSymbol + DAPSK_TX_SYMB_1STOPBIT_3PLACE + DAPSK_TX_SYMB_1STOPBIT_2PLACE + Data2;
						}
						else if (InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit] || EoCh2)
						{
							ReturnSymbol = ReturnSymbol + (4 * Data2) + DAPSK_TX_SYMB_2STOPBIT_LAST;
						}
						else
						{
							ReturnSymbol = ReturnSymbol + (4 * Data2) + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit] + DAPSK_TX_SYMB_1STOPBIT_LAST;
							txUpdatePointerReadFifo(InputData);
							InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;
						}
				
					break;
					/////////////////////////////////////////////////////		
					case 4:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						EoCh4 = InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);

						Data3 = InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						EoCh3 = InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);

						Data2 = InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						EoCh2 = InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);

						if(EoCh4)
						{
							ReturnSymbol = ReturnSymbol + DAPSK_TX_SYMB_1STOPBIT_3PLACE + (2 * Data3) + Data2;
						}
						else if(EoCh3)
						{
							ReturnSymbol = ReturnSymbol + (4 * Data3) + DAPSK_TX_SYMB_1STOPBIT_2PLACE + Data2;
						}
						else if(EoCh2 || InputData->txDataFIFO[DAPSK_ROW_IN_EOCh][InputData->i_oldestBit])
						{
							ReturnSymbol = ReturnSymbol + (4 * Data3) + (2 * Data2) + DAPSK_TX_SYMB_1STOPBIT_LAST;
						}
						else
						{
							ReturnSymbol = ReturnSymbol + (4 * Data3) + (2 * Data2) + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
							txUpdatePointerReadFifo(InputData);
						}
				
					break;
					/////////////////////////////////////////////////////
					default:
					/////////////////////////////////////////////////////
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = ReturnSymbol + (InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

					break;
				}; // end switch(InputData->noOfBits)
		
			break;

			/////////////////////////////////////////////////////
			default:
			/////////////////////////////////////////////////////
				 ASSERT(FALSE); // wrong 
			break;
			}; // end switch(InputData->stateFormSymbol)
		}
		else
		/////////////////////////////////////////////////////
		// SYNCHRONUS MODE -> Form_TxSymbol_FT1.2FT3
		/////////////////////////////////////////////////////
		{
			switch (InputData->stateFormSymbol)
			{
			/////////////////////////////////////////////////////	
			case DAPSK_STATE_FORMSYMB_IDLE:
			/////////////////////////////////////////////////////	
		
				switch (InputData->noOfBits)
				{
					/////////////////////////////////////////////////////
					case 0:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////	
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;
					break;
					/////////////////////////////////////////////////////		
					case 1:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;
					break;
					/////////////////////////////////////////////////////		
					case 2:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;
					break;
					/////////////////////////////////////////////////////		
					case 3:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = (DAPSK_TX_SYMB_3STOPBIT_FIRST + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;
					break;
					/////////////////////////////////////////////////////		
					case 4:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = (DAPSK_TX_SYMB_2STOPBIT_FIRST + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
						ReturnSymbol = (ReturnSymbol + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;
					break;
					/////////////////////////////////////////////////////		
					case 5:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
						ReturnSymbol = (DAPSK_TX_SYMB_1STOPBIT_FIRST + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
					
						ReturnSymbol = (ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = (ReturnSymbol + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;
					break;
					/////////////////////////////////////////////////////
					default:
					/////////////////////////////////////////////////////
						ReturnSymbol = (8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);
				
						ReturnSymbol = (ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
					
						ReturnSymbol = (ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = (ReturnSymbol + InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_BUSY;

					break;
				}; // end switch(InputData->noOfBits)

			break;

			/////////////////////////////////////////////////////		
			case DAPSK_STATE_FORMSYMB_BUSY:
			/////////////////////////////////////////////////////
				switch (InputData->noOfBits)
				{
					/////////////////////////////////////////////////////
					case 0:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////	
			
						ReturnSymbol = DAPSK_TX_SYMB_4STOPBIT;
						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

					break;

					/////////////////////////////////////////////////////		
					case 1:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
				
						ReturnSymbol = (DAPSK_TX_SYMB_3STOPBIT_LAST + (8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]));
						txUpdatePointerReadFifo(InputData);
					
						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

					break;
					/////////////////////////////////////////////////////		
					case 2:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
					
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);
						ReturnSymbol = ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]) + DAPSK_TX_SYMB_2STOPBIT_LAST;
						txUpdatePointerReadFifo(InputData);

						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;

					break;
					/////////////////////////////////////////////////////		
					case 3:  // Number of Bits in FIFO
					/////////////////////////////////////////////////////
					
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);
						ReturnSymbol = ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);
						ReturnSymbol = ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit])+ DAPSK_TX_SYMB_1STOPBIT_LAST;
						txUpdatePointerReadFifo(InputData);
						
						InputData->stateFormSymbol = DAPSK_STATE_FORMSYMB_IDLE;
					
					break;
					/////////////////////////////////////////////////////
					default:
					/////////////////////////////////////////////////////
						ReturnSymbol = 8*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit];
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = ReturnSymbol + (4*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = ReturnSymbol + (2*InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

						ReturnSymbol = ReturnSymbol + (InputData->txDataFIFO[DAPSK_ROW_IN_DATA][InputData->i_oldestBit]);
						txUpdatePointerReadFifo(InputData);

					break;
				}; // end switch(InputData->noOfBits)
		
			break;

			/////////////////////////////////////////////////////
			default:
			/////////////////////////////////////////////////////
				 ASSERT(FALSE); // wrong 
			break;
			}; // end switch(InputData->stateFormSymbol)

		}

	return ReturnSymbol;
}
#pragma optimize_as_cmd_line 

///////////////////////////////////////////////////////////////////////////////////
// DAPSK DEMODULATOR PART                                                        //
///////////////////////////////////////////////////////////////////////////////////

/* Initialize the Demodulator *****************************************************/
#pragma optimize_off 
// adEq_change add config void SEGMENT_EXT_CODE InitDemodulatorDAPSK(t_DAPSK_Demodulator *DemodulatorData, t_ModemConfig *modemConfig, t_V24PortCfg *v24Config, t_DAPSK_OutputConversion *ConversionData, long iModem)
void SEGMENT_EXT_CODE InitDemodulatorDAPSK(t_DAPSK_Demodulator *DemodulatorData, t_ModemConfig *modemConfig,\
										   t_V24PortCfg *v24Config, t_DAPSK_OutputConversion *ConversionData,\
										   t_ModemEqualizerConfig *adEqConfig, long iModem)

{
	// control variables:
	int i_preamble; int i_numerator; int i_nodelayline; int i_delayelement; int i_delayline;int i_scrambler;
	long splitBwTxDataDelay;

	
	DemodulatorData->mode = modemConfig->mode;
	splitBwTxDataDelay = modemConfig->bw_txDataDelay;
	DemodulatorData->baud = v24Config->datarate;

	DemodulatorData->bandwidth = ((splitBwTxDataDelay >> 16)& 0xFFFF );
	DemodulatorData->rxfrequency = (modemConfig->centerFrequency & 0xFFFF );  

	DemodulatorData->rSymb =  DemodulatorData->baud / DAPSK_M; 

	DemodulatorData->nss = DAPSK_F_INTERM/DemodulatorData->rSymb; 

	DemodulatorData->reception = INITACQUISITION1;

/* testMatlab
	DemodulatorData->reception = DETECT_SYMBOL;
// end testMatlab */

	DemodulatorData->gain = 1.0;
	DemodulatorData->stateMSB = 0;

	DemodulatorData->rohNoise  = expq(-1/(float)(DAPSK_ACQU_L_WINDOW * DAPSK_ACQU_SAMPLES_PER_SYMBOL));
	DemodulatorData->rohSignal = expq(-1/(float)(DAPSK_L_PREAMBLE * DAPSK_ACQU_SAMPLES_PER_SYMBOL));
	DemodulatorData->NoisePower = 0.0;
	DemodulatorData->SignalPowerR = 0.0;
	DemodulatorData->SignalPowerI = 0.0;

//	DemodulatorData->EQUpdate_Flag=TRUE;
//	DemodulatorData->stayDetect_Flag=FALSE;
	
// PL_change no Preamble	DemodulatorData->index_acq = 0;

	DemodulatorData->index_mf = 0; // reset index of polyphase matched filter (orig)

	if (DemodulatorData->baud == 9600)
	{	
		// set pointer to the right polyphase matched filter arrays
		DemodulatorData->pmfEarlyLateShift = 5;
        DemodulatorData->pmf0FilterCoef = &rxPolyphase_9600[0][DemodulatorData->pmfEarlyLateShift];

		DemodulatorData->timingSymb = 1;
		DemodulatorData->noOfDelaylines = DAPSK_2_DELAYLINE;
// PL_change machedFilter L=7 		DemodulatorData->lengthMachedFilter = 48; // L=10
		DemodulatorData->lengthMachedFilter = 38;	// L=7

		// add standardisation factor to inputgain
		DemodulatorData->rxInputGain = 1.0/DAPSK_STANDARDISATION_FACTOR_9600;
        
// adEq_change add config:
		DemodulatorData->mfIndexTable =   &mfIndexTable_9600[0][0];
	}
	else if (DemodulatorData->baud == 4800)
	{	
		// set pointer to the right polyphase matched filter arrays
		DemodulatorData->pmfEarlyLateShift = 10;
        DemodulatorData->pmf0FilterCoef = &rxPolyphase_4800[0][DemodulatorData->pmfEarlyLateShift];

		DemodulatorData->timingSymb = 2;
		DemodulatorData->noOfDelaylines = DAPSK_1_DELAYLINE;
// PL_change machedFilter L=7 		DemodulatorData->lengthMachedFilter = 94; // L=10
		DemodulatorData->lengthMachedFilter = 74;   // L=7

		// add standardisation factor to inputgain
		DemodulatorData->rxInputGain = 1.0/DAPSK_STANDARDISATION_FACTOR_4800;

// adEq_change add config:
   		DemodulatorData->mfIndexTable =  &mfIndexTable_4800[0][0];
	}
	else if (DemodulatorData->baud == 2400)
	{	
		// set pointer to the right polyphase matched filter arrays
		DemodulatorData->pmfEarlyLateShift = 14;
        DemodulatorData->pmf0FilterCoef = &rxPolyphase_2400[0][DemodulatorData->pmfEarlyLateShift];

        DemodulatorData->timingSymb = 4;
		DemodulatorData->noOfDelaylines = DAPSK_1_DELAYLINE;
// PL_change machedFilter L=7 		DemodulatorData->lengthMachedFilter = 182; // L=10
		DemodulatorData->lengthMachedFilter = 140;   // L=7

		// add standardisation factor to inputgain
		DemodulatorData->rxInputGain = 1.0/DAPSK_STANDARDISATION_FACTOR_2400;
        
// adEq_change add config:
   		DemodulatorData->mfIndexTable =   &mfIndexTable_2400[0][0];
	}
	else
	{	
		ASSERT(FALSE); // wrong 
	}
	DemodulatorData->timingSymb2 = 2 * DemodulatorData->timingSymb;

// PL_change no Preamble	DemodulatorData->ppreamble = &rxPreamble[0];

	DemodulatorData->symbTimingResolution = DAPSK_ACQU_NO_SAMPLES*(DemodulatorData->timingSymb);
	DemodulatorData->symbPerFilterline = (DemodulatorData->nss/DemodulatorData->symbTimingResolution);


	// Initialising	of the DOWNMODULATION oscillator:
	oscillator_init(1.0, ((-1)*DemodulatorData->rxfrequency), AF_SAMPLE_FREQ, &DemodulatorData->rxOsc);

	// Initialising of the used FIR Filter, take care to initialize All Delaylines
	for (i_delayline = 0; i_delayline <(DemodulatorData->noOfDelaylines); i_delayline++)
	{
		fir_f_init(&(DemodulatorData->pmfDelaylineReal[i_delayline][0]), (DemodulatorData->lengthMachedFilter));
		fir_f_init(&(DemodulatorData->pmfDelaylineImag[i_delayline][0]), (DemodulatorData->lengthMachedFilter));
	}

// PL_change no Preamble	for (i_delayline = 0; i_delayline <DAPSK_ACQU_SAMPLES_PER_SYMBOL; i_delayline++)
// PL_change no Preamble	{
// PL_change no Preamble		fir_f_init(&(DemodulatorData->zpFir[i_delayline][0]), DAPSK_ACQU_SAMPLES_PER_SYMBOL);
// PL_change no Preamble	}

	// init index of the circular buffer in acquisition state:
// PL_change no Preamble	DemodulatorData->iNewZyCorrelation = 0;
	DemodulatorData->iNewZyReal = 0;
	DemodulatorData->iEndZyReal = 0;

	InitRxOutputConversionDAPSK(DemodulatorData, ConversionData);

	// Handshake lines
	DemodulatorData->DCD = FALSE;

	// STATUS Flags:
	DemodulatorData->acquisitonStatus = FALSE;

	// multiply weight to inputgain
	DemodulatorData->rxInputGain = DemodulatorData->rxInputGain *(1.0/modemConfig->weight);

    DemodulatorData->error = 0.001;
	DemodulatorData->dapskAbs = 1.0;
	DemodulatorData->countSignalOn = 0;

	DemodulatorData->InitAcqFlagCounter=0;
	DemodulatorData->DCDoffFlagCounter=0;
	DemodulatorData->errorCriterion=0;
	DemodulatorData->errorCriterionMax=0;
	DemodulatorData->SignalPowerRMax=0;
	DemodulatorData->SignalPowerIMax=0;
//	DemodulatorData->normalization_factorMax=0;
	DemodulatorData->pRcvMin=1.0E+38;
//	DemodulatorData->counterDCDoff=0;
//	DemodulatorData->counterDCDoff_counter=0;
//	DemodulatorData->counterDCDoffMax=0;

// adEq_change add config
//______________________________________________________________________________________

	if ((adEqConfig->mode && AD_EQ_MODE_MASK) != AD_EQ_MODE_OFF)
	{	
		DemodulatorData->adEqOn = TRUE;
		DemodulatorData->difMSB = FALSE; // DAPSK_16DAPSK_ABS
		DemodulatorData->scramblerOn = TRUE;// FALSE;//TRUE;
		DemodulatorData->stepSizeTimingPhase = 0;
		DemodulatorData->stepSizeTimingAmp = 0.02;
	}
	else
	{	
		DemodulatorData->adEqOn = FALSE;
		DemodulatorData->difMSB =  TRUE;  // DAPSK_16DAPSK
		DemodulatorData->scramblerOn = FALSE;
		DemodulatorData->stepSizeTimingPhase = 0.04;
		DemodulatorData->stepSizeTimingAmp = 0.08;
	}

	DemodulatorData->adEqLength = DAPSK_AD_EQ_LENGTH;

	DemodulatorData->adEq_h[(DAPSK_EQUALIZER_INITIAL_TAP-1)].re=1;

	DemodulatorData->adEqInput2i=DemodulatorData->adEqLength;
	DemodulatorData->adEqInput2i_m=DemodulatorData->adEqLength;
	DemodulatorData->adEqInput2i_p=DemodulatorData->adEqLength;
	

	DemodulatorData->offsetRealU =	&((DemodulatorData->pmfDelaylineReal[1][DemodulatorData->lengthMachedFilter+1]))- \
									&((DemodulatorData->pmfDelaylineReal[0][DemodulatorData->lengthMachedFilter+1]));
	DemodulatorData->offsetImagU =	&((DemodulatorData->pmfDelaylineImag[1][DemodulatorData->lengthMachedFilter+1]))- \
									&((DemodulatorData->pmfDelaylineImag[0][DemodulatorData->lengthMachedFilter+1]));


	// Adaptive equalizer AGC
	DemodulatorData->rhoAGC = expq((float)(-1/(DAPSK_AD_EQ_AGC_TAU * DemodulatorData->rSymb)));
	DemodulatorData->P_AGC = 1;

	// descrambler
	for (i_scrambler = 0; i_scrambler <DAPSK_SCRAMBLER_LENGTH; i_scrambler++)
	{
		DemodulatorData->adEqScramblerLine[i_scrambler]= 0;
	}
	DemodulatorData->adEqScramblerLine[0]= 1;
	DemodulatorData->adEqScramblerLine[1]= 1;

	DemodulatorData->scInputPointer = 0;
	DemodulatorData->scramblerXorP1 = 0;
	DemodulatorData->scramblerXorP2 = 1;




//______________________________________________________________________________________
// adEq_change add config end

}
#pragma optimize_as_cmd_line 

/* Signum funtion like in MATLAB sign() ****************************/
float signum(float inFloat)
{	
	if (inFloat>0.0)
	{	
		return 1.0;
	}
	else if(inFloat<0.0)
	{	
		return -1.0;
	}
	else
	{
		return 0.0;
	}
}

/* FIR function with disabled interrupt ****************************/
float fir_f_NoInterrupt( float vIn, float pm vCoeffs[], int   nCoeffs, float vDelay[] )
{
	float returnvalue;

	asm("bit clr mode1 IRPTEN;");
	asm("nop;");				
	returnvalue = fir_f(vIn, vCoeffs,nCoeffs,vDelay);
	asm("nop;");
	asm("bit set mode1 IRPTEN;");

	return returnvalue;
	
}
/*! DAPSK Demodulator 

  This function includes the signal processing part of the DAPSK demodulation 
  described in [4] and the Matlab model [xx]. 

  \b Initfunction:

  InitDemodulatorDAPSK(), called in initModems()

  \image html     Detailed_state_diagram_of_the_DAPSK_receiver.jpg
                  describtion of the picture 

*/

void demodulatorDAPSK(float *RxDataIn, t_DAPSK_Demodulator *DemodulatorData , \
					  t_DAPSK_OutputConversion *ConversionData)
// uncomment this definition when using testMatlab
                      
/* testMatlab
void demodulatorDAPSK(float *RxDataIn, t_DAPSK_Demodulator *DemodulatorData , \
					  t_DAPSK_OutputConversion *ConversionData,t_Matlab_save *Matlab_save)
// end testMatlab */

{
	complex_float downmodData;

	float RxDataNormalized;
	
	///////////////////////////////////////////////////////////////////////////////
	// variable used in state AQUISITION
	BOOL calc_acq; 
    static int i_acq;
	pm static float *p_pmfx;
	int xi,p;
	unsigned long **u;
	float pmf0Filter[DAPSK_L_MATCHED_FILTER_MAX];
	pm static float delayLineReal[DAPSK_L_MATCHED_FILTER_MAX + DAPSK_MAX_NO_EXTENDED_ZEROS];
	pm static float delayLineImag[DAPSK_L_MATCHED_FILTER_MAX + DAPSK_MAX_NO_EXTENDED_ZEROS];
	complex_float outPolyphase;
	complex_float dummyOut;
	register float yr;
	register float yi;
// PL_change no Preamble	pm static float delayLineCorr[DAPSK_L_PREAMBLE];
// PL_change no Preamble	register float yc;
// PL_change no Preamble	register float tacqi;
// PL_change no Preamble	register int tauii;
// PL_change no Preamble	register float tacq;
	//register float index_symb0; // CHANGE 10
	register int index_symb0; 
	register int iDapskOld;
	int SignalOn;
	///////////////////////////////////////////////////////////////////////////////
	// variable used in state DETECT SYMBOL
	BOOL symb; 
	complex_float dapsk;
	complex_float dapsk2;
	complex_float dapsk2_m;
	complex_float dapsk2_p;
	int i,k;
	int dapskMsb;
	complex_float dapskRC;
	int isymbol;
	BOOL addValueToSymbol;
	int iBitNo;
	int i_init;
// PL_change no Preamble	int i_init2;
	///////////////////////////////////////////////////////////////////////////////
	// variable used in states CALC_EARLY_SYMB, CALC_LATE_SYMB
	int index_symb_pre;
	int index_symb_pre2;
	BOOL delay_pre;
	BOOL delay_pre2;
	int index_symb_post;
	int index_symb_post2;
	BOOL delay_post;
	BOOL delay_post2;
	///////////////////////////////////////////////////////////////////////////////
	// variable used in states ADAPTIV_SYMB_TIME
	int i_delayline;
	float rel_ms;
	float rel_sp;
	float rel_dp;
	float rel_dm;
	float rel_mmm;
	float rel_ppp;
	float rel_dpp;
	float rel_dmm;
	float deltaTau1;
	float deltaTau2;
	BOOL updateAdEqualizer = TRUE;
	///////////////////////////////////////////////////////////////////////////////
	// newEarlyLate CHANGE6 / CHANGE7 / CHANGE8 / CHANGE9
	BOOL rel_squ_ms;
	BOOL rel_squ_sp; 
	BOOL rel_squ_dp;
	BOOL rel_squ_dm;

	BOOL rel_squ_mmm;
	BOOL rel_squ_ppp;
	BOOL rel_squ_dpp;
	BOOL rel_squ_dmm;
	BOOL sample_step;
	BOOL phase_cond;
	BOOL amp_cond;

	float deltaTau3;
	float deltaTau4;
	float deltaTau_phase_sum;
	float deltaTau_amp_sum;
///////////////////////////////////////////////////////////////////////
	// variable used in state ADAPTIV_SYMB_TIME
	int z[4];
	int	  iEndZyReal_noise;//CHANGE 11

	///////////////////////////////////////////////////////////////////////////////
	// variable used for error calculation
	complex_float err;
	static float reciprocaldapskAbsOld;
	///////////////////////////////////////////////////////////////////////////////
// case_counter test
/*
static unsigned long startValueTestCounter1;
static unsigned long idleValueTestCounter1;
startValueTestCounter1  = count_start();
*/
	complex_float tempVariable;

    complex_float adEqInput_temp[DAPSK_AD_EQ_LENGTH_MAX];
    static complex_float pm adEqInput_conj[DAPSK_AD_EQ_LENGTH_MAX];

	//scrambler variables
	int i_scrambler;
	int scramblerNewBit;
// PL_change no Preamble	int i_taTEST;
	/////////////////////////////////////////////////////////////////////////////////////////
	// CLEAR BIT FOR FPGA OUTPUT CONVERSION (Set in DETECT SYMBOL)                         //
	DemodulatorData->RxSymbolReady = FALSE;
    // End CLEAR BIT FOR FPGA OUTPUT                                                       //
    /////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// reverse Gainmanipulation                                                  //
	RxDataNormalized = *RxDataIn * DemodulatorData->rxInputGain; 

	///////////////////////////////////////////////////////////////////////////////
	// Downmodulation                                                            //
	oscillator_oscC(&DemodulatorData->outOsc, &DemodulatorData->rxOsc, 1);
	downmodData.re = DemodulatorData->outOsc.re * RxDataNormalized;
	downmodData.im = DemodulatorData->outOsc.im * RxDataNormalized;
// test maxProcessingPower
/*	
	downmodData.re = DemodulatorData->test_outPolyphase.re;
	downmodData.im = DemodulatorData->test_outPolyphase.im;
*/
/* testMatlab
	if ( Matlab_save->save_flag==1 && DemodulatorData->reception!=INITACQUISITION1)
	{
		if (DemodulatorData->baud==600*4)
		{
			downmodData.re=rxInSignal600[Matlab_save->n_cntr][0]+.1;
			downmodData.im=rxInSignal600[Matlab_save->n_cntr][1]+.1;			
		}
		if (DemodulatorData->baud==1200*4)
		{
			downmodData.re=rxInSignal1200[Matlab_save->n_cntr][0]+.1;
			downmodData.im=rxInSignal1200[Matlab_save->n_cntr][1]+.1;			
		}
		if (DemodulatorData->baud==2400*4)
		{
			downmodData.re=rxInSignal2400[Matlab_save->n_cntr][0]+.1;
			downmodData.im=rxInSignal2400[Matlab_save->n_cntr][1]+.1;			
		}
		Matlab_save->downmodData_re[Matlab_save->n_cntr]=downmodData.re;
		Matlab_save->downmodData_im[Matlab_save->n_cntr]=downmodData.im;
	}
// end testMatlab */
		
	switch (DemodulatorData->reception)
	{
	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case ACQUISITION:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		i_acq = DAPSK_I_ACQU_INIT;
		
		calc_acq = (DemodulatorData->index_mf < DAPSK_DOWNSAMPLING);
		
		// wrongfor2400: calc_acq = TRUE;
		
		while (calc_acq)
		{
			i_acq = i_acq +1;
			
		//////////////////////////////////////////////////////////////////////////////////////////
		// Mached Filter                                                                        //
			// pointer to right filter coefficientsline		
			p_pmfx = DemodulatorData->pmf0FilterCoef+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * DemodulatorData->index_mf);

			outPolyphase.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[i_acq][0]));
			outPolyphase.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[i_acq][0]));

			// shift and store polyphase filter outputs (reused in signal acquisition)
			// Start circular buffer for z_y0
			DemodulatorData->iNewZ_y0  = DemodulatorData->iNewZ_y0 + 1;
//			DemodulatorData->iNewZ_y0  = DemodulatorData->iNewZ_y0  % (DAPSK_ACQU_SAMPLES_PER_SYMBOL + 1);
			if(DemodulatorData->iNewZ_y0  >= (DAPSK_ACQU_SAMPLES_PER_SYMBOL + 1))
			{	DemodulatorData->iNewZ_y0  = 0;
			}
			
			DemodulatorData->iEndZ_y0 = DemodulatorData->iNewZ_y0 + 1;
//			DemodulatorData->iEndZ_y0 = DemodulatorData->iEndZ_y0 % (DAPSK_ACQU_SAMPLES_PER_SYMBOL + 1);
			if (DemodulatorData->iEndZ_y0 >=(DAPSK_ACQU_SAMPLES_PER_SYMBOL + 1))
			{	DemodulatorData->iEndZ_y0 = 0;
			}

			DemodulatorData->z_y0[DemodulatorData->iNewZ_y0] = outPolyphase;
			// End circular buffer for z_y0

			// yr: real part of differential symbol received
			yr = (outPolyphase.im * DemodulatorData->z_y0[DemodulatorData->iEndZ_y0].re) - (outPolyphase.re * DemodulatorData->z_y0[DemodulatorData->iEndZ_y0].im);
			// yi: imag part of differential symbol received
			yi = (outPolyphase.re * DemodulatorData->z_y0[DemodulatorData->iEndZ_y0].re) + (outPolyphase.im * DemodulatorData->z_y0[DemodulatorData->iEndZ_y0].im);

		// End Mached Filter                                                                  //
        ////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////
		// correlation with known preamble, realized as filtering with time-inversed preamble  //
// PL_change no Preamble			yc = fir_f(yr, DemodulatorData->ppreamble, 8, &(DemodulatorData->zpFir[DemodulatorData->index_acq][0]));
		// End correlation                                                                    //
        ////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////
		// Update index and Acquisiton arrays                                                  //
// PL_change no Preamble			DemodulatorData->index_acq = DemodulatorData->index_acq + 1;
// PL_change no Preamble			DemodulatorData->index_acq = DemodulatorData->index_acq % (DemodulatorData->symbPerFilterline);

			// Update array with correlated Values of n Symbols
			// Start circular buffer for yc
// PL_change no Preamble			DemodulatorData->iNewZyCorrelation = DemodulatorData->iNewZyCorrelation+1;
// PL_change no Preamble			DemodulatorData->iNewZyCorrelation = DemodulatorData->iNewZyCorrelation % (DAPSK_ACQU_NO_CORR_SYMBOL*DAPSK_ACQU_SAMPLES_PER_SYMBOL);

// PL_change no Preamble			DemodulatorData->zyCorrelation[DemodulatorData->iNewZyCorrelation] = yc;
			// End circular buffer for yc

			// Update array with real part of differential symbol received
			// Start circular buffer for yr
			DemodulatorData->iNewZyReal = DemodulatorData->iNewZyReal + 1;
//			DemodulatorData->iNewZyReal = DemodulatorData->iNewZyReal % ((DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1);
			if(DemodulatorData->iNewZyReal >= ((DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1))
			{	DemodulatorData->iNewZyReal =0;
			}
						
			DemodulatorData->iEndZyReal = DemodulatorData->iNewZyReal + 1;
//			DemodulatorData->iEndZyReal = DemodulatorData->iEndZyReal % ((DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1);
			if(DemodulatorData->iEndZyReal >= ((DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1))
			{	DemodulatorData->iEndZyReal = 0;
			}
			
			DemodulatorData->zyReal[DemodulatorData->iNewZyReal] = yr;
			// End circular buffer for yr

		// End Update index and Acquisiton arrays                                             //
        ////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////
		// Noise / Signal Power (smoothed)                                                     //
			//iEndZyReal_noise = (DemodulatorData->iEndZyReal+1) % ((DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1); // CHANGE 11
			iEndZyReal_noise = DemodulatorData->iEndZyReal;

			DemodulatorData->NoisePower = (DemodulatorData->rohNoise*DemodulatorData->NoisePower)+ \
										((1-DemodulatorData->rohNoise)*DemodulatorData->zyReal[iEndZyReal_noise]*DemodulatorData->zyReal[iEndZyReal_noise]);
//			DemodulatorData->NoisePower = (DemodulatorData->rohNoise*DemodulatorData->NoisePower)+ \
//										((1-DemodulatorData->rohNoise)*DemodulatorData->zyReal[DemodulatorData->iEndZyReal]*DemodulatorData->zyReal[DemodulatorData->iEndZyReal]);

			DemodulatorData->SignalPowerR = (DemodulatorData->rohSignal*DemodulatorData->SignalPowerR)+((1-DemodulatorData->rohSignal)*yr*yr);
			DemodulatorData->SignalPowerI = (DemodulatorData->rohSignal*DemodulatorData->SignalPowerI)+((1-DemodulatorData->rohSignal)*yi*yi);

			if((DemodulatorData->SignalPowerR+DemodulatorData->SignalPowerI)>(DemodulatorData->SignalPowerRMax+DemodulatorData->SignalPowerIMax))
			{
				DemodulatorData->SignalPowerRMax=DemodulatorData->SignalPowerR;
				DemodulatorData->SignalPowerIMax=DemodulatorData->SignalPowerI;
			}

	    // End Noise / Signal Power (smoothed)                                                 //
        /////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////
		// Acquisition Check and index ajustment                                               //


			// update counter if Signal Power is enough to leave aquisiton
			SignalOn = ((DemodulatorData->SignalPowerR+DemodulatorData->SignalPowerI) > DAPSK_ACQU_MIN_SIGNAL); // change4
			// SignalOn = ((DemodulatorData->SignalPowerR > DAPSK_ACQU_MIN_SIGNAL)||(DemodulatorData->SignalPowerI > DAPSK_ACQU_MIN_SIGNAL));
			DemodulatorData->countSignalOn = DemodulatorData->countSignalOn + SignalOn;
			DemodulatorData->countSignalOn = DemodulatorData->countSignalOn * SignalOn;

			// update DCD inactive counter, if the DCDflag is set (DCD was active) and the signalpower
			// was low without a break increase conter
			DemodulatorData->DCDoffcount = DemodulatorData->DCDoffcount + (!SignalOn * DemodulatorData->DCDflag);
			DemodulatorData->DCDoffcount = DemodulatorData->DCDoffcount * !SignalOn;

// PL_change no Preamble			if ((DemodulatorData->NoisePower > DAPSK_ACQU_NR_LEVEL) && ((DemodulatorData->SignalPowerR/DemodulatorData->NoisePower) > DAPSK_ACQU_SR_NR_RATIO))
// PL_change no Preamble			{
// PL_change no Preamble				tacqi = 0;
// PL_change no Preamble				for (i_taTEST = 0; i_taTEST < 16; i_taTEST++)
// PL_change no Preamble				{
// PL_change no Preamble					if (DemodulatorData->zyCorrelation[i_taTEST]>tacqi)
// PL_change no Preamble					{
// PL_change no Preamble						tacqi = DemodulatorData->zyCorrelation[i_taTEST];
// PL_change no Preamble						tauii = i_taTEST;
// PL_change no Preamble						
// PL_change no Preamble					}
// PL_change no Preamble				}
// PL_change no Preamble				tauii =  DemodulatorData->iNewZyCorrelation + ((DAPSK_ACQU_NO_CORR_SYMBOL*DAPSK_ACQU_SAMPLES_PER_SYMBOL) - tauii);
// PL_change no Preamble				tauii = tauii % (DAPSK_ACQU_NO_CORR_SYMBOL*DAPSK_ACQU_SAMPLES_PER_SYMBOL);
// PL_change no Preamble				tacq = (tacqi*tacqi)/DemodulatorData->SignalPowerR;
					// instead of sqrt:
					//	tacq = tacqi / sqrtf(DemodulatorData->SignalPowerR);

// PL_change no Preamble				if (tacq > (DAPSK_ACQU_THR*DAPSK_ACQU_THR))
// PL_change no Preamble				{
// PL_change no Preamble					//index_symb0 = fmodf((dtauii + (float)tauii),(float)DemodulatorData->symbPerFilterline); // CHANGE10
// PL_change no Preamble					index_symb0 = (tauii) % (DemodulatorData->symbPerFilterline); // removeCHANGE 10
// PL_change no Preamble
// PL_change no Preamble					DemodulatorData->reception = DETECT_SYMBOL;
// PL_change no Preamble					// Status Flag: Acquisiton successful
// PL_change no Preamble					DemodulatorData->acquisitonStatus = TRUE;

// PL_change no Preamble				}


// PL_change no Preamble			}
// PL_change no Preamble			else if (DemodulatorData->countSignalOn > (DAPSK_L_PREAMBLE*DemodulatorData->symbPerFilterline)) // in case acquisition failed
			if (DemodulatorData->countSignalOn > (10*DAPSK_L_PREAMBLE*DemodulatorData->symbPerFilterline)) // in case acquisition failed
			{
				index_symb0 = DAPSK_DEFAULT_INDEX;
				DemodulatorData->reception = DETECT_SYMBOL;
				
				// Status Flag: Acquisiton NOT successful
				DemodulatorData->acquisitonStatus = FALSE;
			}
			else
			{
				// criteria to set DCD inactiv : low signalpower over a defined timeperiod
				if ((DemodulatorData->DCDoffcount > (DemodulatorData->DCDholdOnTime*(DAPSK_L_PREAMBLE*DemodulatorData->symbPerFilterline))))
				{
					if(DemodulatorData->DCD==TRUE)
					{
						DemodulatorData->DCDoffFlagCounter++;
					}

					DemodulatorData->DCD = FALSE;

					DemodulatorData->DCDflag = FALSE;
				}
			}
			
	    // End Acquisition Check and index ajustment                                           //
        /////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////
		// Update polyphase matched filter index for next output and calc_acq(exit while or not)//

			DemodulatorData->index_mf = (DemodulatorData->index_mf + DemodulatorData->symbTimingResolution);
			calc_acq = (DemodulatorData->index_mf < DAPSK_DOWNSAMPLING);
			
	    // End index updating                                                                  //
        /////////////////////////////////////////////////////////////////////////////////////////
		} // end of while

		if (DemodulatorData->reception == DETECT_SYMBOL)
		{
			DemodulatorData->index_symb = index_symb0 * DemodulatorData->symbTimingResolution;
			iDapskOld = ((DemodulatorData->iNewZ_y0 + (DAPSK_ACQU_SAMPLES_PER_SYMBOL + 1)) - index_symb0) % (DAPSK_ACQU_SAMPLES_PER_SYMBOL + 1);

			DemodulatorData->dapskOld = DemodulatorData->z_y0[iDapskOld];

			DemodulatorData->dapsk_MFout_Old = DemodulatorData->z_y0[iDapskOld]; // DAPSK_AST_BEFORE_EQ

			///////////////////////////////////////////////////////
			// initialization for Adaptive Symbol Timing

			// CHANGE 2:
			DemodulatorData->pRcv = sqrtf((DemodulatorData->dapskOld.re * DemodulatorData->dapskOld.re) + (DemodulatorData->dapskOld.im * DemodulatorData->dapskOld.im));
			
			if(DemodulatorData->pRcv<DemodulatorData->pRcvMin)
			{
				DemodulatorData->pRcvMin=DemodulatorData->pRcv;
			}
			
			// CHANGE: Datasensitive
			DemodulatorData->dapskMsbOld = (DemodulatorData->pRcv > DAPSK_DETE_AGC_CENTER_RADIUS);
			// end CHANGE: Datasensitive

			DemodulatorData->dapskMsb_AST_Old = (DemodulatorData->pRcv > DAPSK_DETE_AGC_CENTER_RADIUS); // DAPSK_AST_BEFORE_EQ

			DemodulatorData->corrIndex = 0;

			DemodulatorData->index_mf = (DemodulatorData->index_mf - DemodulatorData->symbTimingResolution);
			DemodulatorData->index_symb = DemodulatorData->index_symb + ((DAPSK_DOWNSAMPLING-1) - DemodulatorData->index_mf);

			DemodulatorData->changeIndex = 0; 

			DemodulatorData->DCDoncount=0;
			DemodulatorData->StayDetectCounter=0;

		}
		else
		{
			DemodulatorData->index_mf = DemodulatorData->index_mf - DAPSK_DOWNSAMPLING;
		}


		while (i_acq < (DemodulatorData->noOfDelaylines - 1))
		{
			i_acq = i_acq+1;
			dummyOut.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[i_acq][0]));
			dummyOut.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[i_acq][0]));
		}
		
// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition0)
		{	DemodulatorData->counter_aquisition0 = idleValueTestCounter1;
		}
*/
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////		
	case DETECT_SYMBOL:
	///////////////////////////////////////////////////////////////////////////////////////////////////		

		DemodulatorData->index_symb = (DemodulatorData->index_symb + DAPSK_OVERSAMPLING);
	
		symb = (DemodulatorData->index_symb >= DemodulatorData->nss); // TRUE: a symbol is to be calculatec
		DemodulatorData->index_symb = DemodulatorData->index_symb - (symb*DemodulatorData->nss); 
		
		if (symb)
		{  
			if (DemodulatorData->adEqOn)
			{
				// SAMPLE symbol 2 %%
				
				xi=DAPSK_ACQU_SAMPLES_PER_SYMBOL-DemodulatorData->index_symb;
				p_pmfx = DemodulatorData->pmf0FilterCoef -  *(DemodulatorData->mfIndexTable + (2*xi)+1)			
					  +((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift)*(*(DemodulatorData->mfIndexTable + (2*xi))));

				if (DemodulatorData->noOfDelaylines==2)// 9600 bps
				{
					dapsk2.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[1][0]));
				    dapsk2.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[1][0]));
				}
				else if (DemodulatorData->noOfDelaylines==1)// 4800bps /2400 bps
				{
					// second pmfDelayline is used as dummy.
                    // second mfDelayline is not used in 4800 /2400 bps
                    // Attention %% SAMPLE symbol 2 % must be proceeded
                    // bevor %% SAMPLE symbol 1 % because of updated
                    // delayline.

					vcopy_DD(&(DemodulatorData->pmfDelaylineReal[1][0]), &(DemodulatorData->pmfDelaylineReal[0][0]), (DAPSK_L_MATCHED_FILTER_MAX+DAPSK_MAX_NO_EXTENDED_ZEROS));
					vcopy_DD(&(DemodulatorData->pmfDelaylineImag[1][0]), &(DemodulatorData->pmfDelaylineImag[0][0]), (DAPSK_L_MATCHED_FILTER_MAX+DAPSK_MAX_NO_EXTENDED_ZEROS));
					// ~200 cycles together @2400bps

					u=(unsigned long **)&((DemodulatorData->pmfDelaylineReal[1][DemodulatorData->lengthMachedFilter+1]));
					*u = *u + DemodulatorData->offsetRealU;


					u=(unsigned long **)&((DemodulatorData->pmfDelaylineImag[1][DemodulatorData->lengthMachedFilter+1]));
					*u = *u + DemodulatorData->offsetImagU;

					// This position in the delay line is the pointer to the next position where the new value has to be inserted.
					dapsk2.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[1][0]));
				    dapsk2.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[1][0]));
					// ~240 cycles @2400bps together
				}

				DemodulatorData->index_symb_ad=DemodulatorData->index_symb;
			}

            // SAMPLE symbol 1 %%
			p_pmfx = DemodulatorData->pmf0FilterCoef+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) 
				* (DAPSK_ACQU_SAMPLES_PER_SYMBOL - DemodulatorData->index_symb));

			dapsk.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[0][0]));
		    dapsk.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[0][0]));

			//DemodulatorData->pRcv =sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im));
			//DemodulatorData->pRcv = (DAPSK_RHO_P_1*DemodulatorData->pRcv) + (DAPSK_RHO_P_2*(sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im))));

			// ~240 cycles @2400bps together
/* testMatlab
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->symb_nr++;
				Matlab_save->symb_nr_f=Matlab_save->symb_nr;

				Matlab_save->state[Matlab_save->symb_nr-1]=1;
				Matlab_save->index_symb[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-DemodulatorData->index_symb;
				Matlab_save->pmf[Matlab_save->symb_nr-1]=*p_pmfx;
				Matlab_save->dapsk1_re[Matlab_save->symb_nr-1]=dapsk.re;
				Matlab_save->dapsk1_im[Matlab_save->symb_nr-1]=dapsk.im;
			}
// end testMatlab */

			if (DemodulatorData->adEqOn)
			{
				// AGC ad.Eq.
				// P_AGC = (1-rhoAGC)*real(dpsk*dpsk') + rhoAGC*P_AGC;
				DemodulatorData->P_AGC = (1-DemodulatorData->rhoAGC)*(dapsk.re*dapsk.re+dapsk.im*dapsk.im) + \
										 (DemodulatorData->rhoAGC*DemodulatorData->P_AGC);

				// P_AGC = (1-rhoAGC)*real((dpsk+dpsk2)*(dpsk+dpsk2)')/3.1153 + rhoAGC*P_AGC; 
				// DemodulatorData->P_AGC = 


				if(DemodulatorData->P_AGC > DAPSK_AD_EQ_MIN_P_AGC)
				{
					DemodulatorData->gain=1/sqrtf(DemodulatorData->P_AGC);
					//if(DemodulatorData->gain<0.2) // -14dB
					//{
					//	DemodulatorData->gain==0.2;
					//}
					if(DemodulatorData->gain>5) // +14dB
					{
						DemodulatorData->gain==5;
					}
				}

				DemodulatorData->oldgain = DemodulatorData->gain;

				dapsk.re = dapsk.re * DemodulatorData->oldgain;
				dapsk.im = dapsk.im * DemodulatorData->oldgain;
				dapsk2.re = dapsk2.re * DemodulatorData->oldgain;
				dapsk2.im = dapsk2.im * DemodulatorData->oldgain;

				DemodulatorData->dapsk_MFout.re = dapsk.re; // DAPSK_AST_BEFORE_EQ 
				DemodulatorData->dapsk_MFout.im = dapsk.im; // DAPSK_AST_BEFORE_EQ 
				
				DemodulatorData->pRcv =sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im));
		

				//% Adaptive equalizer% 
				// zwei werte werden in die delayline geschoben fir funktion nur einmal aufrufen
				// insert values in doublebuffer

				if(DemodulatorData->adEqInput2i<0)
				{
					DemodulatorData->adEqInput2i=DemodulatorData->adEqLength-1;
				}
				
				DemodulatorData->adEqInput2[DemodulatorData->adEqInput2i]=dapsk2; 
				DemodulatorData->adEqInput2[DemodulatorData->adEqInput2i+DemodulatorData->adEqLength]=dapsk2; 
				DemodulatorData->adEqInput2i--; 

				if(DemodulatorData->adEqInput2i<0)
				{
					DemodulatorData->adEqInput2i=DemodulatorData->adEqLength-1;
				}
				
				DemodulatorData->adEqInput2[DemodulatorData->adEqInput2i]=dapsk; 
				DemodulatorData->adEqInput2[DemodulatorData->adEqInput2i+DemodulatorData->adEqLength]=dapsk; 
				DemodulatorData->adEqInput2i--; 


				// adaptive filter output calculation
				dapsk = vdot_c_DD( &DemodulatorData->adEqInput2[DemodulatorData->adEqInput2i+1], DemodulatorData->adEq_h, DemodulatorData->adEqLength);

				DemodulatorData->dapsk=dapsk;
				if(DemodulatorData->dapskAbs == 0)
				{
					reciprocaldapskAbsOld = 1.0;
				}
				else
				{
					reciprocaldapskAbsOld = 1.0/DemodulatorData->dapskAbs;
				}
				DemodulatorData->dapskAbs = sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im)); // CHANGE2
				
			}
			else
			{
				if(DemodulatorData->dapskAbs == 0)
				{
					reciprocaldapskAbsOld = 1.0;
				}
				else
				{
					reciprocaldapskAbsOld = 1.0/DemodulatorData->dapskAbs;
				}
				DemodulatorData->dapskAbs = sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im)); // CHANGE2

				dapsk.re = dapsk.re * DemodulatorData->gain;
				dapsk.im = dapsk.im * DemodulatorData->gain;
				DemodulatorData->dapsk=dapsk;
				DemodulatorData->oldgain = DemodulatorData->gain;// CHANGE7

				DemodulatorData->pRcv =sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im));
 			}
/* testMatlab
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->gain[Matlab_save->symb_nr-1]=DemodulatorData->gain;
			}
// end testMatlab */


/* testMatlab
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->dapsk2_re[Matlab_save->symb_nr-1]=dapsk2.re;
				Matlab_save->dapsk2_im[Matlab_save->symb_nr-1]=dapsk2.im;
				Matlab_save->dapsk_re[Matlab_save->symb_nr-1]=dapsk.re;
				Matlab_save->dapsk_im[Matlab_save->symb_nr-1]=dapsk.im;

			}
// end testMatlab */


			if(DemodulatorData->pRcv<DemodulatorData->pRcvMin)
			{
				DemodulatorData->pRcvMin=DemodulatorData->pRcv;
			}

			DemodulatorData->dapsk_squ = (dapsk.re * dapsk.re) + (dapsk.im * dapsk.im); // CHANGE6 (including gain)


			DemodulatorData->dapskR.re = (dapsk.re * DemodulatorData->dapskOld.re) - (dapsk.im * ((-1) * DemodulatorData->dapskOld.im));
			DemodulatorData->dapskR.im = (dapsk.re * ((-1) * DemodulatorData->dapskOld.im)) + (dapsk.im * DemodulatorData->dapskOld.re);
			
			
			if (DemodulatorData->adEqOn)
			{
				dapskMsb = ((DemodulatorData->dapskAbs) > DAPSK_DETE_AGC_CENTER_RADIUS);			
			}
			else
			{
				dapskMsb = ((DemodulatorData->gain * DemodulatorData->dapskAbs) > DAPSK_DETE_AGC_CENTER_RADIUS);            // True: MSB was detected // CHANGE2			
			}

			DemodulatorData->dapsk_d_squ  = ((dapskMsb)*outerRadius*outerRadius) + ((!dapskMsb)*innerRadius*innerRadius); // CHANGE6

			if (!DemodulatorData->adEqOn)
			{
				DemodulatorData->gain = DemodulatorData->gain + (DAPSK_DETE_AGC_SMALL_NR*(DAPSK_DETE_AGC_CENTER_RADIUS - (DemodulatorData->gain*DemodulatorData->dapskAbs))); // CHANGE2
			}

			DemodulatorData->JB[0] = dapskMsb;
			if (DemodulatorData->difMSB)
			{
				DemodulatorData->IB[0] = dapskMsb ^ DemodulatorData->dapskMsbOld;
				DemodulatorData->dapskMsbOld = dapskMsb;
			}
			else
			{
				DemodulatorData->IB[0] = dapskMsb;
			}
			
			dapskRC.re = (DemodulatorData->dapskR.re * demodAngle.re) - (DemodulatorData->dapskR.im * demodAngle.im);
			dapskRC.im = (DemodulatorData->dapskR.re * demodAngle.im) + (DemodulatorData->dapskR.im * demodAngle.re);

			DemodulatorData->IB[1] = DemodulatorData->JB[1] = (dapskRC.im > 0.0);                     // True: second bit is "1"
			DemodulatorData->IB[2] = DemodulatorData->JB[2] = (dapskRC.re < 0.0);                     // True: third  bit is "1"
			DemodulatorData->IB[3] = DemodulatorData->JB[3] = (fabs(dapskRC.im) < fabs(dapskRC.re));  // True: fourth bit is "1"

  
			// Descrambler function
			if(DemodulatorData->scramblerOn)
			{
				for (i_scrambler = 0; i_scrambler < DAPSK_M; i_scrambler++)
				{
					scramblerNewBit = DemodulatorData->IB[i_scrambler];
					scramblerNewBit = scramblerNewBit^DemodulatorData->adEqScramblerLine[DemodulatorData->scramblerXorP1];
					scramblerNewBit = scramblerNewBit^DemodulatorData->adEqScramblerLine[DemodulatorData->scramblerXorP2];
		
					// update Scrambler delayline
					DemodulatorData->adEqScramblerLine[DemodulatorData->scInputPointer] = DemodulatorData->IB[i_scrambler];
					DemodulatorData->IB[i_scrambler] = scramblerNewBit;

					// update pointer
					DemodulatorData->scInputPointer = DemodulatorData->scInputPointer + 1;
					DemodulatorData->scramblerXorP1 = DemodulatorData->scramblerXorP1 + 1;
					DemodulatorData->scramblerXorP2 = DemodulatorData->scramblerXorP2 + 1;

					if (DemodulatorData->scInputPointer >= DAPSK_SCRAMBLER_LENGTH)
					{	DemodulatorData->scInputPointer = 0;
					}
					if (DemodulatorData->scramblerXorP1 >= DAPSK_SCRAMBLER_LENGTH)
					{	DemodulatorData->scramblerXorP1 = 0;
					}
					if (DemodulatorData->scramblerXorP2 >= DAPSK_SCRAMBLER_LENGTH)
					{	DemodulatorData->scramblerXorP2 = 0;
					}
				}
			}


		/////////////////////////////////////////////////////////////////////////////////////////
		// SET BIT FOR FPGA OUTPUT CONVERSION (Reset at the beginning of this function)        //
			DemodulatorData->RxSymbolReady = TRUE;
	    // End SET BIT FOR FPGA OUTPUT                                                         //
        /////////////////////////////////////////////////////////////////////////////////////////

			// find right Index of symbol
			isymbol = 0;
			#pragma vector_for 
			for (iBitNo= 0; iBitNo<DAPSK_M ; iBitNo++)
			{
				addValueToSymbol = (DemodulatorData->JB[iBitNo] > 0);
				isymbol = isymbol + (addValueToSymbol * ValueArray[iBitNo]);
			}
			
			//DemodulatorData->dapskRD = cp_Ampl[isymbol]; // old faster way -> removed for transparency reasons
			if(DemodulatorData->JB[0])
			{	DemodulatorData->dapskRD.re = cp[isymbol].re * outerRadius;
				DemodulatorData->dapskRD.im = cp[isymbol].im * outerRadius;
			}
			else
			{	DemodulatorData->dapskRD.re = cp[isymbol].re * innerRadius;
				DemodulatorData->dapskRD.im = cp[isymbol].im * innerRadius;
			}

/* testMatlab
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->IB[Matlab_save->symb_nr-1]=1*DemodulatorData->IB[0]+
													 2*DemodulatorData->IB[1]+
													 4*DemodulatorData->IB[2]+
													 8*DemodulatorData->IB[3];
				Matlab_save->dapskR_re[Matlab_save->symb_nr-1]=DemodulatorData->dapskR.re;
				Matlab_save->dapskR_im[Matlab_save->symb_nr-1]=DemodulatorData->dapskR.im;
				Matlab_save->dapskRD_re[Matlab_save->symb_nr-1]=DemodulatorData->dapskRD.re;
				Matlab_save->dapskRD_im[Matlab_save->symb_nr-1]=DemodulatorData->dapskRD.im;
			}
// end testMatlab */
			err.re =  (reciprocaldapskAbsOld * \
				       ((DemodulatorData->dapskOld.re * DemodulatorData->dapskRD.re) - (DemodulatorData->dapskOld.im * DemodulatorData->dapskRD.im))) - dapsk.re;
			err.im =  (reciprocaldapskAbsOld * \
				      ((DemodulatorData->dapskOld.re * DemodulatorData->dapskRD.im)  + (DemodulatorData->dapskOld.im * DemodulatorData->dapskRD.re))) - dapsk.im;

			DemodulatorData->error = (DAPSK_RHO_E_1*DemodulatorData->error) + (DAPSK_RHO_E_2*((err.re * err.re) - ((-1) * err.im * err.im)));


			BOOL GOTOINITACQUISITION;
			GOTOINITACQUISITION=FALSE;

			if(DemodulatorData->adEqOn) // adaptive
			{
				if(DemodulatorData->DCDoncount<DAPSK_DCDON_HOLD*DemodulatorData->rSymb)
				{
					DemodulatorData->DCDoncount++;	
				}
				else 
				{
					BOOL AEQ_DCDOFF_Counter;

					AEQ_DCDOFF_Counter =(DemodulatorData->error>DAPSK_SNR_LEVEL_15DB_REAL_12DB);
								
					DemodulatorData->StayDetectCounter = (DemodulatorData->StayDetectCounter+AEQ_DCDOFF_Counter);
					DemodulatorData->StayDetectCounter = (DemodulatorData->StayDetectCounter*AEQ_DCDOFF_Counter);

					GOTOINITACQUISITION = (DemodulatorData->StayDetectCounter >= (DAPSK_DCDOFF_HOLD*DemodulatorData->rSymb));
				}

			}
			else //static
			{
				GOTOINITACQUISITION=(DemodulatorData->pRcv<DAPSK_DETE_MIN_POWER_RCV);
			}
			
			
			if(GOTOINITACQUISITION )
			{
				//////////////////////////////////////////////
				// overwrite last FPGA outputsequence,because of 
				// insufficient Power
				DemodulatorData->IB[0] = 1;
				DemodulatorData->IB[1] = 1;
				DemodulatorData->IB[2] = 1;
				DemodulatorData->IB[3] = 1;

				//////////////////////////////////////////////
				// go to Aquisition init
				// reduce computingtime
				DemodulatorData->reception = INITACQUISITION1;
				//////////////////////////////////////////////
/* testMatlab
				DemodulatorData->reception = DETECT_SYMBOL;
// end testMatlab */
			}
			else
			{
				//////////////////////////////////////////////
				// Handshake line: DCD High
				DemodulatorData->DCD = TRUE;
				//////////////////////////////////////////////
				// go to next state
				DemodulatorData->reception = CALC_EARLY_SYMB;
				//////////////////////////////////////////////
			}
/* testMatlab
				DemodulatorData->reception = CALC_EARLY_SYMB;
// end testMatlab */
			DemodulatorData->dapskOld = dapsk;
					
		}
		else
		{
			p_pmfx = DemodulatorData->pmf0FilterCoef;
			dummyOut.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[0][0]));
		    dummyOut.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[0][0]));
			
			if ((DemodulatorData->noOfDelaylines == DAPSK_2_DELAYLINE)&& (DemodulatorData->adEqOn))
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef;
				dummyOut.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[1][0]));
				dummyOut.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[1][0]));
			}
/* testMatlab
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=1.01;
				Matlab_save->pmf[Matlab_save->n_cntr]=*p_pmfx;
			}
// end testMatlab */

		}

		if ((DemodulatorData->noOfDelaylines == DAPSK_2_DELAYLINE) && (!DemodulatorData->adEqOn))
		{
			p_pmfx = DemodulatorData->pmf0FilterCoef;
			dummyOut.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[1][0]));
			dummyOut.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[1][0]));
		}

// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_detect)
		{	DemodulatorData->counter_detect = idleValueTestCounter1;
		}
*/
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case CALC_EARLY_SYMB:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

			///////////////////////////////////////////////
			// calculate "early" symbol
   			///////////////////////////////////////////////
			index_symb_pre = DemodulatorData->index_symb + DemodulatorData->timingSymb;
			delay_pre = (index_symb_pre >= DAPSK_DOWNSAMPLING);
			index_symb_pre = index_symb_pre - (delay_pre*DAPSK_DOWNSAMPLING);
			// calculate mached filter
			if (delay_pre)
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 2 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_pre));

				DemodulatorData->dapsk_m.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_m.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));

/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->pmf[Matlab_save->n_cntr]=*p_pmfx;
				}
// end testMatlab */
			}
			else
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 1 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_pre));

				DemodulatorData->dapsk_m.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_m.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));

/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->pmf[Matlab_save->n_cntr]=*p_pmfx;
				}
// end testMatlab */
			}
			if (!DemodulatorData->adEqOn)
			{
				DemodulatorData->dapsk_m.re = DemodulatorData->dapsk_m.re * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_m.im = DemodulatorData->dapsk_m.im * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_m_squ = ((DemodulatorData->dapsk_m.re*DemodulatorData->dapsk_m.re)+(DemodulatorData->dapsk_m.im*DemodulatorData->dapsk_m.im));// CHANGE6

				DemodulatorData->dapskR_m.re = (DemodulatorData->dapsk_m.re * DemodulatorData->dapskOld_m.re) - (DemodulatorData->dapsk_m.im * ((-1) * DemodulatorData->dapskOld_m.im));
				DemodulatorData->dapskR_m.im = (DemodulatorData->dapsk_m.re * ((-1) * DemodulatorData->dapskOld_m.im)) + (DemodulatorData->dapsk_m.im * DemodulatorData->dapskOld_m.re);

				DemodulatorData->dapskOld_m = DemodulatorData->dapsk_m;
			}

			if ((DemodulatorData->rSymb==2400) && (!DemodulatorData->adEqOn))
			{
			/////////////////////////////////////////////////////////////////////
			// calculate "before-early" symbol (when 2 DELAYLINES are maintained)
			/////////////////////////////////////////////////////////////////////
				index_symb_pre2 = DemodulatorData->index_symb + DemodulatorData->timingSymb2;
				delay_pre2 = (index_symb_pre2 >= DAPSK_DOWNSAMPLING);
				index_symb_pre2 = index_symb_pre2 - (delay_pre2*DAPSK_DOWNSAMPLING);
				// calculate mached filter
				if (delay_pre2)
				{
					p_pmfx = DemodulatorData->pmf0FilterCoef- ( 2 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_pre2));
					DemodulatorData->dapsk_mm.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[1][0]));
					DemodulatorData->dapsk_mm.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[1][0]));
				}
				else
				{
					p_pmfx = DemodulatorData->pmf0FilterCoef- ( 1 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_pre2));

					DemodulatorData->dapsk_mm.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[1][0]));
					DemodulatorData->dapsk_mm.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[1][0]));
				}

				DemodulatorData->dapsk_mm.re = DemodulatorData->dapsk_mm.re * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_mm.im = DemodulatorData->dapsk_mm.im * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_mm_squ = ((DemodulatorData->dapsk_mm.re*DemodulatorData->dapsk_mm.re)+(DemodulatorData->dapsk_mm.im*DemodulatorData->dapsk_mm.im)); // CHANGE6
			
				DemodulatorData->dapskR_mm.re = (DemodulatorData->dapsk_mm.re * DemodulatorData->dapskOld_mm.re) - (DemodulatorData->dapsk_mm.im * ((-1) * DemodulatorData->dapskOld_mm.im));
				DemodulatorData->dapskR_mm.im = (DemodulatorData->dapsk_mm.re * ((-1) * DemodulatorData->dapskOld_mm.im)) + (DemodulatorData->dapsk_mm.im * DemodulatorData->dapskOld_mm.re);

				DemodulatorData->dapskOld_mm = DemodulatorData->dapsk_mm;
/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->index_symb_pre2[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_pre2;
					Matlab_save->dapsk_mm_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_mm.re;
					Matlab_save->dapsk_mm_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_mm.im;
				}			
// end testMatlab */
			}
			else if ((DemodulatorData->rSymb == 2400) && (DemodulatorData->adEqOn))
			{
				//%% SAMPLE early 2 for 9600bps %%
				xi=DAPSK_ACQU_SAMPLES_PER_SYMBOL-DemodulatorData->index_symb_ad-DemodulatorData->timingSymb;
				p=(xi<0);
				xi=xi+(DAPSK_ACQU_SAMPLES_PER_SYMBOL+1)*p;
				p_pmfx = DemodulatorData->pmf0FilterCoef -  *(DemodulatorData->mfIndexTable + (2*xi)+1)-p-1			
					  +((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift)*(*(DemodulatorData->mfIndexTable + (2*xi))));

				dapsk2_m.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[1][0]));
				dapsk2_m.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[1][0]));

				// AGC ad.Eq.
				DemodulatorData->dapsk_m.re = DemodulatorData->dapsk_m.re * DemodulatorData->oldgain;
				DemodulatorData->dapsk_m.im = DemodulatorData->dapsk_m.im * DemodulatorData->oldgain;
				dapsk2_m.re = dapsk2_m.re * DemodulatorData->oldgain;
				dapsk2_m.im = dapsk2_m.im * DemodulatorData->oldgain;

				DemodulatorData->dapsk_MFout_m.re = DemodulatorData->dapsk_m.re; // DAPSK_AST_BEFORE_EQ
				DemodulatorData->dapsk_MFout_m.im = DemodulatorData->dapsk_m.im; // DAPSK_AST_BEFORE_EQ

/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->dapsk2_m_re[Matlab_save->symb_nr-1]=dapsk2_m.re;
					Matlab_save->dapsk2_m_im[Matlab_save->symb_nr-1]=dapsk2_m.im;
				}			
// end testMatlab */
				
				//% Adaptive equalizer% 
				
				// insert values in doublebuffer
				if(DemodulatorData->adEqInput2i_m<0)
				{
					DemodulatorData->adEqInput2i_m=DemodulatorData->adEqLength-1;
				}
				
				DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m]=dapsk2_m; 
				DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m+DemodulatorData->adEqLength]=dapsk2_m; 
				DemodulatorData->adEqInput2i_m--; 

				if(DemodulatorData->adEqInput2i_m<0)
				{
					DemodulatorData->adEqInput2i_m=DemodulatorData->adEqLength-1;
				}
				
				dapsk2_m=DemodulatorData->dapsk_m;				
				DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m]=dapsk2_m; 
				DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m+DemodulatorData->adEqLength]=dapsk2_m; 
				DemodulatorData->adEqInput2i_m--; 

				// adaptive filter output calculation				
				DemodulatorData->dapsk_m = vdot_c_DD( &DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m+1], DemodulatorData->adEq_h, DemodulatorData->adEqLength);

				DemodulatorData->dapsk_m_squ = ((DemodulatorData->dapsk_m.re*DemodulatorData->dapsk_m.re)+(DemodulatorData->dapsk_m.im*DemodulatorData->dapsk_m.im));// CHANGE6

				DemodulatorData->dapskR_m.re = (DemodulatorData->dapsk_m.re * DemodulatorData->dapskOld_m.re) - (DemodulatorData->dapsk_m.im * ((-1) * DemodulatorData->dapskOld_m.im));
				DemodulatorData->dapskR_m.im = (DemodulatorData->dapsk_m.re * ((-1) * DemodulatorData->dapskOld_m.im)) + (DemodulatorData->dapsk_m.im * DemodulatorData->dapskOld_m.re);			

				DemodulatorData->dapskOld_m = DemodulatorData->dapsk_m;

			}
			else
			{
				// do nothing
			}
/* testMatlab		
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=2;
				Matlab_save->index_symb_pre[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_pre;
				Matlab_save->dapsk_m_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_m.re;
				Matlab_save->dapsk_m_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_m.im;
			}
// end testMatlab */
			///////////////////////////////////////////////
			// index update
			DemodulatorData->index_symb = (DemodulatorData->index_symb + DAPSK_OVERSAMPLING);

			///////////////////////////////////////////////
			// go to next state
			DemodulatorData->reception = CALC_LATE_SYMB;
			///////////////////////////////////////////////

			///////////////////////////////////////////////
			// ad Equalizer errorCriterion	
			DemodulatorData->errorCriterion = (DemodulatorData->NL.re * DemodulatorData->NL.re) + (DemodulatorData->NL.im * DemodulatorData->NL.im);
			if(DemodulatorData->errorCriterion>DemodulatorData->errorCriterionMax)
			{
				DemodulatorData->errorCriterionMax=DemodulatorData->errorCriterion;
			}
			///////////////////////////////////////////////

// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_early)
		{	DemodulatorData->counter_early = idleValueTestCounter1;
		}
*/	
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case CALC_LATE_SYMB:
	///////////////////////////////////////////////////////////////////////////////////////////////////	
		
			///////////////////////////////////////////////
			// calculate "late" symbol
			index_symb_post = DemodulatorData->index_symb - (DemodulatorData->timingSymb + DAPSK_DOWNSAMPLING);
			delay_post = (index_symb_post < 0);
			index_symb_post = index_symb_post + (delay_post*DAPSK_DOWNSAMPLING);
			///////////////////////////////////////////////
			// calculate mached filter
			if (delay_post)
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 1 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_post));
				DemodulatorData->dapsk_p.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_p.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));
			}
			else
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 2 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_post));
				DemodulatorData->dapsk_p.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_p.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));
			}

			if (!DemodulatorData->adEqOn)
			{
				DemodulatorData->dapsk_p.re = DemodulatorData->dapsk_p.re * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_p.im = DemodulatorData->dapsk_p.im * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_p_squ = ((DemodulatorData->dapsk_p.re*DemodulatorData->dapsk_p.re)+(DemodulatorData->dapsk_p.im*DemodulatorData->dapsk_p.im)); // CHANGE6

				DemodulatorData->dapskR_p.re = (DemodulatorData->dapsk_p.re * DemodulatorData->dapskOld_p.re) - (DemodulatorData->dapsk_p.im * ((-1) * DemodulatorData->dapskOld_p.im));
				DemodulatorData->dapskR_p.im = (DemodulatorData->dapsk_p.re * ((-1) * DemodulatorData->dapskOld_p.im)) + (DemodulatorData->dapsk_p.im * DemodulatorData->dapskOld_p.re);

				DemodulatorData->dapskOld_p = DemodulatorData->dapsk_p;
			}
			
/* testMatlab		
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=3;
				Matlab_save->index_symb_post[Matlab_save->n_cntr]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_post;
				Matlab_save->dapsk_p_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_p.re;
				Matlab_save->dapsk_p_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_p.im;
			}
// end testMatlab */

			//if (DemodulatorData->noOfDelaylines == DAPSK_2_DELAYLINE)
			if ((DemodulatorData->rSymb == 2400) && (!DemodulatorData->adEqOn))
			{
			///////////////////////////////////////////////////////////////////
			// calculate "after-late" symbol (when 2 DELAYLINES are maintained)
			///////////////////////////////////////////////////////////////////
				index_symb_post2 = DemodulatorData->index_symb - (DemodulatorData->timingSymb2 + DAPSK_DOWNSAMPLING);
				delay_post2 = (index_symb_post2 < 0);
				index_symb_post2 = index_symb_post2 + (delay_post2*DAPSK_DOWNSAMPLING);
				///////////////////////////////////////////////
				// calculate mached filter
				if (delay_post2)
				{
					p_pmfx = DemodulatorData->pmf0FilterCoef- ( 1 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_post2));
					DemodulatorData->dapsk_pp.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[1][0]));
					DemodulatorData->dapsk_pp.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[1][0]));
				}
				else
				{
					p_pmfx = DemodulatorData->pmf0FilterCoef- ( 2 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_post2));

					DemodulatorData->dapsk_pp.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[1][0]));
					DemodulatorData->dapsk_pp.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[1][0]));
				}

				DemodulatorData->dapsk_pp.re = DemodulatorData->dapsk_pp.re * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_pp.im = DemodulatorData->dapsk_pp.im * DemodulatorData->oldgain; // CHANGE7
				DemodulatorData->dapsk_pp_squ = ((DemodulatorData->dapsk_pp.re*DemodulatorData->dapsk_pp.re)+(DemodulatorData->dapsk_pp.im*DemodulatorData->dapsk_pp.im));// CHANGE6

				DemodulatorData->dapskR_pp.re = (DemodulatorData->dapsk_pp.re * DemodulatorData->dapskOld_pp.re) - (DemodulatorData->dapsk_pp.im * ((-1) * DemodulatorData->dapskOld_pp.im));
				DemodulatorData->dapskR_pp.im = (DemodulatorData->dapsk_pp.re * ((-1) * DemodulatorData->dapskOld_pp.im)) + (DemodulatorData->dapsk_pp.im * DemodulatorData->dapskOld_pp.re);

				DemodulatorData->dapskOld_pp = DemodulatorData->dapsk_pp;
				
/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->index_symb_post2[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_post2;
					Matlab_save->dapsk_pp_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_pp.re;
					Matlab_save->dapsk_pp_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_pp.im;
				}			
// end testMatlab */	
				

				///////////////////////////////////////////////
				// go to next state
				DemodulatorData->reception = ADAPTIV_SYMB_TIME;
				///////////////////////////////////////////////
			}
			else if ((DemodulatorData->rSymb == 2400) && (DemodulatorData->adEqOn))
			{
				//%% SAMPLE late 2 %%
				xi=DAPSK_ACQU_SAMPLES_PER_SYMBOL-DemodulatorData->index_symb_ad+DemodulatorData->timingSymb;
				p=(xi>DAPSK_ACQU_SAMPLES_PER_SYMBOL);
				xi=xi-(DAPSK_ACQU_SAMPLES_PER_SYMBOL+1)*p;
				p_pmfx = DemodulatorData->pmf0FilterCoef -  *(DemodulatorData->mfIndexTable + (2*xi)+1)+p-2			
					  +((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift)*(*(DemodulatorData->mfIndexTable + (2*xi))));

				dapsk2_p.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[1][0]));
				dapsk2_p.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[1][0]));

				// AGC ad.Eq.
				DemodulatorData->dapsk_p.re = DemodulatorData->dapsk_p.re * DemodulatorData->oldgain;
				DemodulatorData->dapsk_p.im = DemodulatorData->dapsk_p.im * DemodulatorData->oldgain;
				dapsk2_p.re = dapsk2_p.re * DemodulatorData->oldgain;
				dapsk2_p.im = dapsk2_p.im * DemodulatorData->oldgain;

				DemodulatorData->dapsk_MFout_p.re = DemodulatorData->dapsk_p.re; // DAPSK_AST_BEFORE_EQ
				DemodulatorData->dapsk_MFout_p.im = DemodulatorData->dapsk_p.im; // DAPSK_AST_BEFORE_EQ

/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->index_symb_post2[Matlab_save->symb_nr-1]=xi;
					Matlab_save->dapsk2_p_re[Matlab_save->symb_nr-1]=dapsk2_p.re;
					Matlab_save->dapsk2_p_im[Matlab_save->symb_nr-1]=dapsk2_p.im;
				}			
// end testMatlab */

				//% Adaptive equalizer% 
				
				// insert values in doublebuffer
				if(DemodulatorData->adEqInput2i_p<0)
				{
					DemodulatorData->adEqInput2i_p=DemodulatorData->adEqLength-1;
				}
				
				DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p]=dapsk2_p; 
				DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p+DemodulatorData->adEqLength]=dapsk2_p; 
				DemodulatorData->adEqInput2i_p--; 

				if(DemodulatorData->adEqInput2i_p<0)
				{
					DemodulatorData->adEqInput2i_p=DemodulatorData->adEqLength-1;
				}
				
				DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p]=DemodulatorData->dapsk_p; 
				DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p+DemodulatorData->adEqLength]=DemodulatorData->dapsk_p; 
				DemodulatorData->adEqInput2i_p--; 

				// adaptive filter output calculation				
				DemodulatorData->dapsk_p = vdot_c_DD( &DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p+1], DemodulatorData->adEq_h, DemodulatorData->adEqLength);

				DemodulatorData->dapsk_p_squ = ((DemodulatorData->dapsk_p.re*DemodulatorData->dapsk_p.re)+(DemodulatorData->dapsk_p.im*DemodulatorData->dapsk_p.im)); // CHANGE6

				DemodulatorData->dapskR_p.re = (DemodulatorData->dapsk_p.re * DemodulatorData->dapskOld_p.re) - (DemodulatorData->dapsk_p.im * ((-1) * DemodulatorData->dapskOld_p.im));
				DemodulatorData->dapskR_p.im = (DemodulatorData->dapsk_p.re * ((-1) * DemodulatorData->dapskOld_p.im)) + (DemodulatorData->dapsk_p.im * DemodulatorData->dapskOld_p.re);

				DemodulatorData->dapskOld_p = DemodulatorData->dapsk_p;

				///////////////////////////////////////////////
				// go to next state
				DemodulatorData->reception = ADAPTIV_SYMB_TIME;
				///////////////////////////////////////////////
			}
			else
			{
				///////////////////////////////////////////////
				// go to next state
				DemodulatorData->reception = CALC_BEFORE_EARLY_SYMB;
				///////////////////////////////////////////////
			}
/* testMatlab		
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=3;
				Matlab_save->index_symb_post[Matlab_save->n_cntr]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_post;
				Matlab_save->dapsk_p_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_p.re;
				Matlab_save->dapsk_p_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_p.im;
			}
// end testMatlab */

			///////////////////////////////////////////////
			// index update
			DemodulatorData->index_symb = (DemodulatorData->index_symb + DAPSK_OVERSAMPLING);
// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_late)
		{	DemodulatorData->counter_late = idleValueTestCounter1;
		}
*/
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case CALC_BEFORE_EARLY_SYMB:
	///////////////////////////////////////////////////////////////////////////////////////////////////	
		if (!DemodulatorData->adEqOn)
		{

			/////////////////////////////////////////////////////////////////////
			// calculate "before-early" symbol (when 1 DELAYLINE is maintained)
			/////////////////////////////////////////////////////////////////////
			index_symb_pre2 = DemodulatorData->index_symb + DemodulatorData->timingSymb2 - (2*DAPSK_DOWNSAMPLING);
			delay_pre2 = (index_symb_pre2 >= DAPSK_DOWNSAMPLING);
			index_symb_pre2 = index_symb_pre2 - (delay_pre2*DAPSK_DOWNSAMPLING);
			// calculate mached filter
			if (delay_pre2)
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 4 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_pre2));
				DemodulatorData->dapsk_mm.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_mm.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));
			}
			else
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 3 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_pre2));
				DemodulatorData->dapsk_mm.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_mm.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));
			}
		

			DemodulatorData->dapsk_mm.re = DemodulatorData->dapsk_mm.re * DemodulatorData->oldgain; // CHANGE7
			DemodulatorData->dapsk_mm.im = DemodulatorData->dapsk_mm.im * DemodulatorData->oldgain; // CHANGE7
			DemodulatorData->dapsk_mm_squ = ((DemodulatorData->dapsk_mm.re*DemodulatorData->dapsk_mm.re)+(DemodulatorData->dapsk_mm.im*DemodulatorData->dapsk_mm.im)); // CHANGE6
			
			DemodulatorData->dapskR_mm.re = (DemodulatorData->dapsk_mm.re * DemodulatorData->dapskOld_mm.re) - (DemodulatorData->dapsk_mm.im * ((-1) * DemodulatorData->dapskOld_mm.im));
			DemodulatorData->dapskR_mm.im = (DemodulatorData->dapsk_mm.re * ((-1) * DemodulatorData->dapskOld_mm.im)) + (DemodulatorData->dapsk_mm.im * DemodulatorData->dapskOld_mm.re);

			DemodulatorData->dapskOld_mm = DemodulatorData->dapsk_mm;
		
/* testMatlab		
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=4;
				Matlab_save->index_symb_pre2[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_pre2;
				Matlab_save->dapsk_mm_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_mm.re;
				Matlab_save->dapsk_mm_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_mm.im;
			}			
// end testMatlab */
		
		}
		else
		{
			//%% SAMPLE early 2 for 4800bps and 2400bps %%
			xi=DAPSK_ACQU_SAMPLES_PER_SYMBOL-DemodulatorData->index_symb_ad-DemodulatorData->timingSymb;
			p=(xi<0);
			xi=xi+(DAPSK_ACQU_SAMPLES_PER_SYMBOL+1)*p;
			p_pmfx = DemodulatorData->pmf0FilterCoef -  *(DemodulatorData->mfIndexTable + (2*xi)+1)-p-1-2		
					  +((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift)*(*(DemodulatorData->mfIndexTable + (2*xi))));

			dapsk2_m.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[0][0]));
			dapsk2_m.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[0][0]));
			

			// AGC ad.Eq.
			DemodulatorData->dapsk_m.re = DemodulatorData->dapsk_m.re * DemodulatorData->oldgain;
			DemodulatorData->dapsk_m.im = DemodulatorData->dapsk_m.im * DemodulatorData->oldgain;
			dapsk2_m.re = dapsk2_m.re * DemodulatorData->oldgain;
			dapsk2_m.im = dapsk2_m.im * DemodulatorData->oldgain;

			DemodulatorData->dapsk_MFout_m.re = DemodulatorData->dapsk_m.re; // DAPSK_AST_BEFORE_EQ
			DemodulatorData->dapsk_MFout_m.im = DemodulatorData->dapsk_m.im; // DAPSK_AST_BEFORE_EQ

/* testMatlab		
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->dapsk2_m_re[Matlab_save->symb_nr-1]=dapsk2_m.re;
				Matlab_save->dapsk2_m_im[Matlab_save->symb_nr-1]=dapsk2_m.im;
			}			
// end testMatlab */
			
			//% Adaptive equalizer% 
				
			// insert values in doublebuffer
			if(DemodulatorData->adEqInput2i_m<0)
			{
				DemodulatorData->adEqInput2i_m=DemodulatorData->adEqLength-1;
			}
				
			DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m]=dapsk2_m; 
			DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m+DemodulatorData->adEqLength]=dapsk2_m; 
			DemodulatorData->adEqInput2i_m--; 

			if(DemodulatorData->adEqInput2i_m<0)
			{
				DemodulatorData->adEqInput2i_m=DemodulatorData->adEqLength-1;
			}
				
			DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m]=DemodulatorData->dapsk_m; 
			DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m+DemodulatorData->adEqLength]=DemodulatorData->dapsk_m; 
			DemodulatorData->adEqInput2i_m--; 

			// adaptive filter output calculation
			DemodulatorData->dapsk_m = vdot_c_DD( &DemodulatorData->adEqInput2_m[DemodulatorData->adEqInput2i_m+1], DemodulatorData->adEq_h, DemodulatorData->adEqLength);
				
			DemodulatorData->dapsk_m_squ = ((DemodulatorData->dapsk_m.re*DemodulatorData->dapsk_m.re)+(DemodulatorData->dapsk_m.im*DemodulatorData->dapsk_m.im));// CHANGE6

			DemodulatorData->dapskR_m.re = (DemodulatorData->dapsk_m.re * DemodulatorData->dapskOld_m.re) - (DemodulatorData->dapsk_m.im * ((-1) * DemodulatorData->dapskOld_m.im));
			DemodulatorData->dapskR_m.im = (DemodulatorData->dapsk_m.re * ((-1) * DemodulatorData->dapskOld_m.im)) + (DemodulatorData->dapsk_m.im * DemodulatorData->dapskOld_m.re);			

			DemodulatorData->dapskOld_m = DemodulatorData->dapsk_m;

/* testMatlab		
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=2;
				Matlab_save->index_symb_pre[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_pre;
				Matlab_save->dapsk_m_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_m.re;
				Matlab_save->dapsk_m_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_m.im;
			}
// end testMatlab */

		}

		///////////////////////////////////////////////
		// index update
		DemodulatorData->index_symb = (DemodulatorData->index_symb + DAPSK_OVERSAMPLING);

		///////////////////////////////////////////////
		// go to next state
		DemodulatorData->reception = CALC_AFTER_LATE_SYMB;
		///////////////////////////////////////////////
// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_beforeearly)
		{	DemodulatorData->counter_beforeearly = idleValueTestCounter1;
		}
*/		
		break;


	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case CALC_AFTER_LATE_SYMB:
	///////////////////////////////////////////////////////////////////////////////////////////////////	
		if (!DemodulatorData->adEqOn)
		{
			///////////////////////////////////////////////////////////////////
			// calculate "after-late" symbol (when 2 DELAYLINES are maintained)
			///////////////////////////////////////////////////////////////////
			index_symb_post2 = DemodulatorData->index_symb - (DemodulatorData->timingSymb2 + (3*DAPSK_DOWNSAMPLING));
			delay_post2 = (index_symb_post2 < 0);
			index_symb_post2 = index_symb_post2 + (delay_post2*DAPSK_DOWNSAMPLING);
			///////////////////////////////////////////////
			// calculate mached filter
			if (delay_post2)
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 3 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_post2));
				DemodulatorData->dapsk_pp.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_pp.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));
			}
			else
			{
				p_pmfx = DemodulatorData->pmf0FilterCoef- ( 4 )+((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift) * (DAPSK_ACQU_SAMPLES_PER_SYMBOL - index_symb_post2));
				DemodulatorData->dapsk_pp.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineReal[0][0]));
				DemodulatorData->dapsk_pp.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, (DemodulatorData->lengthMachedFilter), &(DemodulatorData->pmfDelaylineImag[0][0]));
			}


			DemodulatorData->dapsk_pp.re = DemodulatorData->dapsk_pp.re * DemodulatorData->oldgain; // CHANGE7
			DemodulatorData->dapsk_pp.im = DemodulatorData->dapsk_pp.im * DemodulatorData->oldgain; // CHANGE7
			DemodulatorData->dapsk_pp_squ = ((DemodulatorData->dapsk_pp.re*DemodulatorData->dapsk_pp.re)+(DemodulatorData->dapsk_pp.im*DemodulatorData->dapsk_pp.im));// CHANGE6

			DemodulatorData->dapskR_pp.re = (DemodulatorData->dapsk_pp.re * DemodulatorData->dapskOld_pp.re) - (DemodulatorData->dapsk_pp.im * ((-1) * DemodulatorData->dapskOld_pp.im));
			DemodulatorData->dapskR_pp.im = (DemodulatorData->dapsk_pp.re * ((-1) * DemodulatorData->dapskOld_pp.im)) + (DemodulatorData->dapsk_pp.im * DemodulatorData->dapskOld_pp.re);

			DemodulatorData->dapskOld_pp = DemodulatorData->dapsk_pp;

/* testMatlab		
		if ( Matlab_save->save_flag==1)
		{
			Matlab_save->state[Matlab_save->n_cntr]=5;
			Matlab_save->index_symb_post2[Matlab_save->symb_nr-1]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_post2;
			Matlab_save->dapsk_pp_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_pp.re;
			Matlab_save->dapsk_pp_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_pp.im;
		}			
// end testMatlab */

		}
		else 
		{
			//%% SAMPLE late 2 %%
			xi=DAPSK_ACQU_SAMPLES_PER_SYMBOL-DemodulatorData->index_symb_ad+DemodulatorData->timingSymb;
			p=(xi>DAPSK_ACQU_SAMPLES_PER_SYMBOL);
			xi=xi-(DAPSK_ACQU_SAMPLES_PER_SYMBOL+1)*p;
			p_pmfx = DemodulatorData->pmf0FilterCoef -  *(DemodulatorData->mfIndexTable + (2*xi)+1)+p-2-2			
				  +((DemodulatorData->lengthMachedFilter + DemodulatorData->pmfEarlyLateShift)*(*(DemodulatorData->mfIndexTable + (2*xi))));

			dapsk2_p.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[0][0]));
			dapsk2_p.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[0][0]));

			// AGC ad.Eq.
			DemodulatorData->dapsk_p.re = DemodulatorData->dapsk_p.re * DemodulatorData->oldgain;
			DemodulatorData->dapsk_p.im = DemodulatorData->dapsk_p.im * DemodulatorData->oldgain;
			dapsk2_p.re = dapsk2_p.re * DemodulatorData->oldgain;
			dapsk2_p.im = dapsk2_p.im * DemodulatorData->oldgain;

			DemodulatorData->dapsk_MFout_p.re = DemodulatorData->dapsk_p.re; // DAPSK_AST_BEFORE_EQ
			DemodulatorData->dapsk_MFout_p.im = DemodulatorData->dapsk_p.im; // DAPSK_AST_BEFORE_EQ

/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->index_symb_post2[Matlab_save->symb_nr-1]=xi;
					Matlab_save->dapsk2_p_re[Matlab_save->symb_nr-1]=dapsk2_p.re;
					Matlab_save->dapsk2_p_im[Matlab_save->symb_nr-1]=dapsk2_p.im;
				}			
// end testMatlab */

			//% Adaptive equalizer% 
			// insert values in doublebuffer				
			if(DemodulatorData->adEqInput2i_p<0)
			{
				DemodulatorData->adEqInput2i_p=DemodulatorData->adEqLength-1;
			}
				
			DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p]=dapsk2_p; 
			DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p+DemodulatorData->adEqLength]=dapsk2_p; 
			DemodulatorData->adEqInput2i_p--; 

			if(DemodulatorData->adEqInput2i_p<0)
			{
				DemodulatorData->adEqInput2i_p=DemodulatorData->adEqLength-1;
			}
				
			DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p]=DemodulatorData->dapsk_p; 
			DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p+DemodulatorData->adEqLength]=DemodulatorData->dapsk_p; 
			DemodulatorData->adEqInput2i_p--; 

			// adaptive filter output calculation				
			DemodulatorData->dapsk_p = vdot_c_DD( &DemodulatorData->adEqInput2_p[DemodulatorData->adEqInput2i_p+1], DemodulatorData->adEq_h, DemodulatorData->adEqLength);

			DemodulatorData->dapsk_p_squ = ((DemodulatorData->dapsk_p.re*DemodulatorData->dapsk_p.re)+(DemodulatorData->dapsk_p.im*DemodulatorData->dapsk_p.im)); // CHANGE6

			DemodulatorData->dapskR_p.re = (DemodulatorData->dapsk_p.re * DemodulatorData->dapskOld_p.re) - (DemodulatorData->dapsk_p.im * ((-1) * DemodulatorData->dapskOld_p.im));
			DemodulatorData->dapskR_p.im = (DemodulatorData->dapsk_p.re * ((-1) * DemodulatorData->dapskOld_p.im)) + (DemodulatorData->dapsk_p.im * DemodulatorData->dapskOld_p.re);

			DemodulatorData->dapskOld_p = DemodulatorData->dapsk_p;

/* testMatlab		
				if ( Matlab_save->save_flag==1)
				{
					Matlab_save->state[Matlab_save->n_cntr]=3;
					Matlab_save->index_symb_post[Matlab_save->n_cntr]=DAPSK_ACQU_SAMPLES_PER_SYMBOL-index_symb_post;
					Matlab_save->dapsk_p_re[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_p.re;
					Matlab_save->dapsk_p_im[Matlab_save->symb_nr-1]=DemodulatorData->dapsk_p.im;
				}
// end testMatlab */

		}		

		///////////////////////////////////////////////
		// index update
		DemodulatorData->index_symb = (DemodulatorData->index_symb + DAPSK_OVERSAMPLING);

		///////////////////////////////////////////////
		// go to next state
		DemodulatorData->reception = ADAPTIV_SYMB_TIME;
		///////////////////////////////////////////////
// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_afterlate)
		{	DemodulatorData->counter_afterlate = idleValueTestCounter1;
		}
*/
		break;
		
	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case ADAPTIV_SYMB_TIME:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		// Update adaptive filter
        if (DemodulatorData->adEqOn)
        {
            if ((DemodulatorData->dapsk_squ > DAPSK_Rb0) && (DemodulatorData->dapsk_squ < DAPSK_Rb1) )
            {
                DemodulatorData->Rdd=innerRadius_squ;
				updateAdEqualizer = TRUE;
            }
            else if((DemodulatorData->dapsk_squ > DAPSK_Rb2) && (DemodulatorData->dapsk_squ < DAPSK_Rb3))
            {
                DemodulatorData->Rdd=outerRadius_squ;
				updateAdEqualizer = TRUE;
            }
            else if((DemodulatorData->dapsk_squ > DAPSK_Rb1) && (DemodulatorData->dapsk_squ < DAPSK_Rb2))
			{
                DemodulatorData->Rdd=DAPSK_R2;
				updateAdEqualizer = TRUE;
			}
            else
            {
                DemodulatorData->Rdd=DAPSK_R2;
				updateAdEqualizer = FALSE;
            }

			
            DemodulatorData->NL.re = (DemodulatorData->dapsk.re*(DemodulatorData->dapsk_squ - DemodulatorData->Rdd));
            DemodulatorData->NL.im = (DemodulatorData->dapsk.im*(DemodulatorData->dapsk_squ - DemodulatorData->Rdd));

			vconj_c_PD(adEqInput_conj, &DemodulatorData->adEqInput2[DemodulatorData->adEqInput2i+1], DemodulatorData->adEqLength);

			tempVariable.re = DAPSK_ALPHA_NORMALIZED*DemodulatorData->NL.re;
			tempVariable.im = DAPSK_ALPHA_NORMALIZED*DemodulatorData->NL.im;

			vsmul_c_DP(adEqInput_temp,tempVariable, adEqInput_conj, DemodulatorData->adEqLength);
			if (updateAdEqualizer)
			{			
				vsub_c_DDD(DemodulatorData->adEq_h, DemodulatorData->adEq_h, adEqInput_temp, DemodulatorData->adEqLength);
			}
			
/* testMatlab	
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->Rdd[Matlab_save->symb_nr-1]=DemodulatorData->Rdd;
				Matlab_save->alpha_normalized[Matlab_save->symb_nr-1]=DAPSK_ALPHA_NORMALIZED;
				Matlab_save->NL_re[Matlab_save->symb_nr-1]=DemodulatorData->NL.re;
				Matlab_save->NL_im[Matlab_save->symb_nr-1]=DemodulatorData->NL.im;
			}			
// end testMatlab */

		}
		if (DemodulatorData->adEqOn)
		{
			//if (DAPSK_AST_BEFORE_EQ)// DAPSK_AST_BEFORE_EQ //using the if case --> switching between nested and cascaded algorithm
			//{ 
				float dapskAbs_MFout; // --> dapskAbs
			

				dapsk = DemodulatorData->dapsk_MFout;
				DemodulatorData->dapsk_p = DemodulatorData->dapsk_MFout_p;
				DemodulatorData->dapsk_m = DemodulatorData->dapsk_MFout_m;

				dapskAbs_MFout = sqrtf((dapsk.re * dapsk.re) + (dapsk.im * dapsk.im));

				DemodulatorData->dapskR.re = (dapsk.re * DemodulatorData->dapsk_MFout_Old.re) - (dapsk.im * ((-1) * DemodulatorData->dapsk_MFout_Old.im));
				DemodulatorData->dapskR.im = (dapsk.re * ((-1) * DemodulatorData->dapsk_MFout_Old.im)) + (dapsk.im * DemodulatorData->dapsk_MFout_Old.re);

				dapskMsb = (dapskAbs_MFout > DAPSK_DETE_AGC_CENTER_RADIUS);        // True: MSB was detected

				DemodulatorData->JB[0] = dapskMsb;
				if (DemodulatorData->difMSB)
				{
					DemodulatorData->IB[0] = dapskMsb ^ DemodulatorData->dapskMsbOld;
					DemodulatorData->dapskMsbOld = dapskMsb;
				}
				else
				{
					DemodulatorData->IB[0] = dapskMsb;
				}

					
				dapskRC.re = (DemodulatorData->dapskR.re * demodAngle.re) - (DemodulatorData->dapskR.im * demodAngle.im);
				dapskRC.im = (DemodulatorData->dapskR.re * demodAngle.im) + (DemodulatorData->dapskR.im * demodAngle.re);

				DemodulatorData->IB[1] = DemodulatorData->JB[1] = (dapskRC.im > 0.0);                     // True: second bit is "1"
				DemodulatorData->IB[2] = DemodulatorData->JB[2] = (dapskRC.re < 0.0);                     // True: third  bit is "1"
				DemodulatorData->IB[3] = DemodulatorData->JB[3] = (fabs(dapskRC.im) < fabs(dapskRC.re));  // True: fourth bit is "1"


				// find right Index of symbol
				isymbol = 0;
				#pragma vector_for 
				for (iBitNo= 0; iBitNo<DAPSK_M ; iBitNo++)
				{
					addValueToSymbol = (DemodulatorData->JB[iBitNo] > 0);
					isymbol = isymbol + (addValueToSymbol * ValueArray[iBitNo]);
				}
					
				if(DemodulatorData->JB[0])
				{	DemodulatorData->dapskRD.re = cp[isymbol].re * outerRadius;
					DemodulatorData->dapskRD.im = cp[isymbol].im * outerRadius;
				}
				else
				{	DemodulatorData->dapskRD.re = cp[isymbol].re * innerRadius;
					DemodulatorData->dapskRD.im = cp[isymbol].im * innerRadius;
				}

				DemodulatorData->dapsk_MFout_Old = dapsk;

				DemodulatorData->dapskR_m.re = (DemodulatorData->dapsk_m.re * DemodulatorData->dapsk_MFout_Old_m.re) - (DemodulatorData->dapsk_m.im * ((-1) * DemodulatorData->dapsk_MFout_Old_m.im));
				DemodulatorData->dapskR_m.im = (DemodulatorData->dapsk_m.re * ((-1) * DemodulatorData->dapsk_MFout_Old_m.im)) + (DemodulatorData->dapsk_m.im * DemodulatorData->dapsk_MFout_Old_m.re);			
				DemodulatorData->dapsk_MFout_Old_m = DemodulatorData->dapsk_m;

				DemodulatorData->dapskR_p.re = (DemodulatorData->dapsk_p.re * DemodulatorData->dapsk_MFout_Old_p.re) - (DemodulatorData->dapsk_p.im * ((-1) * DemodulatorData->dapsk_MFout_Old_p.im));
				DemodulatorData->dapskR_p.im = (DemodulatorData->dapsk_p.re * ((-1) * DemodulatorData->dapsk_MFout_Old_p.im)) + (DemodulatorData->dapsk_p.im * DemodulatorData->dapsk_MFout_Old_p.re);
				DemodulatorData->dapsk_MFout_Old_p = DemodulatorData->dapsk_p;

				DemodulatorData->dapsk_squ = (dapsk.re * dapsk.re) + (dapsk.im * dapsk.im); // (including gain)
				DemodulatorData->dapsk_m_squ = ((DemodulatorData->dapsk_m.re*DemodulatorData->dapsk_m.re)+(DemodulatorData->dapsk_m.im*DemodulatorData->dapsk_m.im));
				DemodulatorData->dapsk_d_squ  = ((dapskMsb)*outerRadius*outerRadius) + ((!dapskMsb)*innerRadius*innerRadius);
				DemodulatorData->dapsk_p_squ = ((DemodulatorData->dapsk_p.re*DemodulatorData->dapsk_p.re)+(DemodulatorData->dapsk_p.im*DemodulatorData->dapsk_p.im)); // CHANGE6

			//}


			// set dpsk_xx and dpsk_x to same value
			// In this way the AST remains  the sam for static and
			// adaptive equalizer.
			DemodulatorData->dapsk_pp=DemodulatorData->dapsk_p;
			DemodulatorData->dapskOld_pp=DemodulatorData->dapskOld_p;
			DemodulatorData->dapskR_pp=DemodulatorData->dapskR_p;

			DemodulatorData->dapsk_pp_squ=DemodulatorData->dapsk_p_squ;

			DemodulatorData->dapsk_mm=DemodulatorData->dapsk_m;
			DemodulatorData->dapskOld_mm=DemodulatorData->dapskOld_m;
			DemodulatorData->dapskR_mm=DemodulatorData->dapskR_m;

			DemodulatorData->dapsk_mm_squ=DemodulatorData->dapsk_m_squ;
		}

			///////////////////////////////////////////////
			// update delayline
			p_pmfx = DemodulatorData->pmf0FilterCoef;

			for (i_delayline = 0; i_delayline <(DemodulatorData->noOfDelaylines); i_delayline++)
			{
				dummyOut.re = fir_f_NoInterrupt(downmodData.re, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineReal[i_delayline][0]));
				dummyOut.im = fir_f_NoInterrupt(downmodData.im, p_pmfx, DemodulatorData->lengthMachedFilter, &(DemodulatorData->pmfDelaylineImag[i_delayline][0]));
			}
/* testMatlab	
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->state[Matlab_save->n_cntr]=6;
				//Matlab_save->index_symb[Matlab_save->n_cntr]=-1;
			}			
// end testMatlab */
			rel_squ_ms  = (DemodulatorData->dapsk_m_squ  < DemodulatorData->dapsk_squ);    // CHANGE6
			rel_squ_sp  = (DemodulatorData->dapsk_squ    < DemodulatorData->dapsk_p_squ);  // CHANGE6
			rel_squ_dp  = (DemodulatorData->dapsk_d_squ  < DemodulatorData->dapsk_p_squ);  // CHANGE6
			rel_squ_dm  = (DemodulatorData->dapsk_d_squ  < DemodulatorData->dapsk_m_squ);  // CHANGE6

			rel_squ_mmm = (DemodulatorData->dapsk_mm_squ < DemodulatorData->dapsk_m_squ);  // CHANGE6
			rel_squ_ppp = (DemodulatorData->dapsk_pp_squ < DemodulatorData->dapsk_p_squ);  // CHANGE6
			rel_squ_dpp = (DemodulatorData->dapsk_d_squ  < DemodulatorData->dapsk_pp_squ); // CHANGE6
			rel_squ_dmm = (DemodulatorData->dapsk_d_squ  < DemodulatorData->dapsk_mm_squ); // CHANGE6


			if(DemodulatorData->adEqOn)
			{
				sample_step = 0;
			}
			else
			{
				sample_step = (((DemodulatorData->dapsk_mm_squ<DAPSK_DETE_AGC_CENTER_RADIUS_SQU)&&(DAPSK_DETE_AGC_CENTER_RADIUS_SQU<DemodulatorData->dapsk_pp_squ))|| \
					          ((DemodulatorData->dapsk_mm_squ>DAPSK_DETE_AGC_CENTER_RADIUS_SQU)&&(DAPSK_DETE_AGC_CENTER_RADIUS_SQU>DemodulatorData->dapsk_pp_squ))); // CHANGE8
			}

			///////////////////////////////////////////////
			// calculate flags
			rel_ms  = signum((DemodulatorData->dapskR_m.re * ((-1) * DemodulatorData->dapskR.im)) + (DemodulatorData->dapskR_m.im * DemodulatorData->dapskR.re));
			rel_sp  = signum((DemodulatorData->dapskR.re * ((-1) * DemodulatorData->dapskR_p.im)) + (DemodulatorData->dapskR.im * DemodulatorData->dapskR_p.re));
			rel_dp  = signum((DemodulatorData->dapskRD.re * ((-1) * DemodulatorData->dapskR_p.im)) + (DemodulatorData->dapskRD.im * DemodulatorData->dapskR_p.re));
			rel_dm  = signum((DemodulatorData->dapskRD.re * ((-1) * DemodulatorData->dapskR_m.im)) + (DemodulatorData->dapskRD.im * DemodulatorData->dapskR_m.re));

			rel_mmm = signum((DemodulatorData->dapskR_mm.re * ((-1) * DemodulatorData->dapskR_m.im)) + (DemodulatorData->dapskR_mm.im * DemodulatorData->dapskR_m.re));
			rel_ppp = signum((DemodulatorData->dapskR_pp.re * ((-1) * DemodulatorData->dapskR_p.im)) + (DemodulatorData->dapskR_pp.im * DemodulatorData->dapskR_p.re));
			rel_dpp = signum((DemodulatorData->dapskRD.re * ((-1) * DemodulatorData->dapskR_pp.im)) + (DemodulatorData->dapskRD.im * DemodulatorData->dapskR_pp.re));
			rel_dmm = signum((DemodulatorData->dapskRD.re * ((-1) * DemodulatorData->dapskR_mm.im)) + (DemodulatorData->dapskRD.im * DemodulatorData->dapskR_mm.re));

			///////////////////////////////////////////////
			// update of symbol timing
			phase_cond = ((rel_ms == rel_sp)&&(rel_dp == rel_dm)); 
			deltaTau1 = ((rel_dp != rel_sp) -  (rel_dm == rel_ms)); 
            deltaTau2 = (((rel_dpp == rel_ppp)&&(rel_ppp != rel_sp)) - ((rel_dmm == rel_mmm)&&(rel_mmm == rel_ms)))*(!DemodulatorData->changeIndex);
			deltaTau_phase_sum = phase_cond* (DemodulatorData->stepSizeTimingPhase*(deltaTau1+deltaTau2));

			amp_cond = ((rel_squ_ms == rel_squ_sp )&&(rel_squ_dp == rel_squ_dm));
			deltaTau3 = ((rel_squ_dp != rel_squ_sp) -  (rel_squ_dm == rel_squ_ms));
			deltaTau4 = (((rel_squ_dpp == rel_squ_ppp)&&(rel_squ_ppp != rel_squ_sp)) - ((rel_squ_dmm == rel_squ_mmm)&&(rel_squ_mmm == rel_squ_ms)))*(!DemodulatorData->changeIndex);
			deltaTau_amp_sum = amp_cond* (DemodulatorData->stepSizeTimingAmp*(deltaTau3+deltaTau4));


//no deltaTAU_EQ			float absinittab, absinittab1,absinittab_1;
//no deltaTAU_EQ			int deltatau_EQ;
//no deltaTAU_EQ			absinittab=  DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-1].re*DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-1].re
//no deltaTAU_EQ						+DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-1].im*DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-1].im;
//no deltaTAU_EQ			absinittab1= DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP].re*DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP].re
//no deltaTAU_EQ					    +DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP].im*DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP].im;
//no deltaTAU_EQ			absinittab_1=DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-2].re*DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-2].re
//no deltaTAU_EQ						+DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-2].im*DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-2].im;
//no deltaTAU_EQ
//no deltaTAU_EQ			deltatau_EQ=(absinittab1>absinittab)-(absinittab_1>=absinittab);
//no deltaTAU_EQ			DemodulatorData->tau0 = DemodulatorData->tau0 + (deltaTau_phase_sum + deltaTau_amp_sum + sample_step+ DAPSK_tap_track_step*deltatau_EQ);
//no deltaTAU_EQ:

			DemodulatorData->tau0 = DemodulatorData->tau0 + (deltaTau_phase_sum + deltaTau_amp_sum + sample_step);
			
			DemodulatorData->corrIndex = floorf(DemodulatorData->tau0 + 0.5); // uncomment to switch of the AST
			DemodulatorData->tau0 = DemodulatorData->tau0 - DemodulatorData->corrIndex; // uncomment to switch of the AST
			DemodulatorData->tau = DemodulatorData->tau + DemodulatorData->corrIndex*DemodulatorData->timingSymb;
			DemodulatorData->tau0c=DemodulatorData->tau0c+DemodulatorData->tau0;
/* testMatlab	
			if ( Matlab_save->save_flag==1)
			{
				Matlab_save->corrIndex[Matlab_save->symb_nr-1]=DemodulatorData->corrIndex;
				Matlab_save->tau0[Matlab_save->symb_nr-1]	  =DemodulatorData->tau0;
			}			
// end testMatlab */
			DemodulatorData->changeIndex = DemodulatorData->corrIndex * DemodulatorData->corrIndex;
			DemodulatorData->index_symb = DemodulatorData->index_symb - (DemodulatorData->corrIndex * DemodulatorData->timingSymb);

	
			if (DemodulatorData->corrIndex > 0.0)
			{
				DemodulatorData->dapskOld_m = DemodulatorData->dapskOld;
                DemodulatorData->dapskOld   = DemodulatorData->dapskOld_p;
                DemodulatorData->dapskOld_p = DemodulatorData->dapskOld_pp;

				DemodulatorData->dapsk_MFout_Old_m = DemodulatorData->dapsk_MFout_Old; // DAPSK_AST_BEFORE_EQ
                DemodulatorData->dapsk_MFout_Old   = DemodulatorData->dapsk_MFout_Old_p; // DAPSK_AST_BEFORE_EQ
                DemodulatorData->dapsk_MFout_Old_p = DemodulatorData->dapsk_MFout_Old_p; // DAPSK_AST_BEFORE_EQ
			}
			else if (DemodulatorData->corrIndex < 0.0)
			{
				DemodulatorData->dapskOld_p = DemodulatorData->dapskOld;
				DemodulatorData->dapskOld   = DemodulatorData->dapskOld_m;
				DemodulatorData->dapskOld_m = DemodulatorData->dapskOld_mm;

				DemodulatorData->dapsk_MFout_Old_p = DemodulatorData->dapsk_MFout_Old; // DAPSK_AST_BEFORE_EQ
				DemodulatorData->dapsk_MFout_Old   = DemodulatorData->dapsk_MFout_Old_m; // DAPSK_AST_BEFORE_EQ
				DemodulatorData->dapsk_MFout_Old_m = DemodulatorData->dapsk_MFout_Old_m; // DAPSK_AST_BEFORE_EQ
			}
			else
			{
			}
		
		///////////////////////////////////////////////
		// index update
		DemodulatorData->index_symb = (DemodulatorData->index_symb + DAPSK_OVERSAMPLING);

		///////////////////////////////////////////////
		// go to next state
		DemodulatorData->reception = DETECT_SYMBOL;
		///////////////////////////////////////////////
// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_adsymbol)
		{	DemodulatorData->counter_adsymbol = idleValueTestCounter1;
		}
*/
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case INITACQUISITION1:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		// idle case to compansate calculation power

		//////////////////////////////////////////////
		// go back to acquisition
		DemodulatorData->reception = INITACQUISITION2;
/* testMatlab
		DemodulatorData->reception = DETECT_SYMBOL;
// end testMatlab */
		//////////////////////////////////////////////
// case_counter test
/*
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition1)
		{	DemodulatorData->counter_aquisition1 = idleValueTestCounter1;
		}
*/
		DemodulatorData->InitAcqFlagCounter++;
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case INITACQUISITION2:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		executeInitacquisiton2(DemodulatorData);

		//////////////////////////////////////////////
		// go back to acquisition
		DemodulatorData->reception = INITACQUISITION3;
		//////////////////////////////////////////////
// case_counter test
/*		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition2)
		{	DemodulatorData->counter_aquisition2 = idleValueTestCounter1;
		}
*/		
		break;


	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case INITACQUISITION3:
	///////////////////////////////////////////////////////////////////////////////////////////////////	


		executeInitacquisiton3(DemodulatorData);

		//////////////////////////////////////////////
		// go back to acquisition
		DemodulatorData->reception = INITACQUISITION4;
		//////////////////////////////////////////////
// case_counter test
/*		
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition3)
		{	DemodulatorData->counter_aquisition3 = idleValueTestCounter1;
		}
*/		
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case INITACQUISITION4:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		executeInitacquisiton4(DemodulatorData);

		//////////////////////////////////////////////
		// go back to acquisition
		DemodulatorData->reception = INITACQUISITION5;
		//////////////////////////////////////////////
// case_counter test
/*		
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition4)
		{	DemodulatorData->counter_aquisition4 = idleValueTestCounter1;
		}
*/		
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case INITACQUISITION5:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		executeInitacquisiton5(DemodulatorData);

		//////////////////////////////////////////////
		// go back to acquisition
		DemodulatorData->reception = INITACQUISITION6;
		//////////////////////////////////////////////
// case_counter test
/*		
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition5)
		{	DemodulatorData->counter_aquisition5 = idleValueTestCounter1;
		}
*/		
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case INITACQUISITION6:
	///////////////////////////////////////////////////////////////////////////////////////////////////	

		executeInitacquisiton6(DemodulatorData);

		//////////////////////////////////////////////
		// go back to acquisition
		DemodulatorData->reception = ACQUISITION;
		//////////////////////////////////////////////
// case_counter test
/*		
		idleValueTestCounter1 = count_end(startValueTestCounter1);
		if (idleValueTestCounter1 > DemodulatorData->counter_aquisition6)
		{	DemodulatorData->counter_aquisition6 = idleValueTestCounter1;
		}
*/		
		break;

	default:
		
		DemodulatorData->reception = 0;
		break;
	}; // end switch(DemodulatorData->reception)


	/////////////////////////////////////////////////////////////////////////////////////////
	// FPGA OUTPUT CONVERSION                                                              //
	DemodulatorData->fpgaRUD =  rxOutputConversionDAPSK(DemodulatorData, ConversionData);
    // End FPGA OUTPUT CONVERSION                                                          //
    /////////////////////////////////////////////////////////////////////////////////////////

}

/* Initialization DAPSK ACQUISITION 2 case ***************/
//#pragma optimize_off 
void SEGMENT_EXT_CODE executeInitacquisiton2(t_DAPSK_Demodulator *DemodulatorData)
{
	int i_init;
//	complex_float initvalue;
//	initvalue.re = 0.0;
//	initvalue.im = 0.0;

	//////////////////////////////////////////////
	// initialize acquisition variables
	// CHANGE: Datasensitive
	DemodulatorData->error = 0.001;
	DemodulatorData->dapskAbs = 1.0;
	DemodulatorData->gain = 1.0;
	// end CHANGE: Datasensitive

	// RESET ACQUISITION VARIABLES
// PL_change no Preamble		DemodulatorData->index_acq = 0;

	DemodulatorData->index_mf = 0; // NEW added index reset

	DemodulatorData->NoisePower = 0.0;
	DemodulatorData->SignalPowerR = 0.0;
	DemodulatorData->SignalPowerI = 0.0;

// PL_change no Preamble		for (i_init = 0; i_init < DAPSK_ACQU_SAMPLES_PER_SYMBOL; i_init++)
// PL_change no Preamble		{
// PL_change no Preamble			#pragma vector_for 
// PL_change no Preamble			for (i_init2 = 0; i_init2 < (10); i_init2++)
// PL_change no Preamble			{
// PL_change no Preamble				DemodulatorData->zpFir[i_init][i_init2]= 0.0;
// PL_change no Preamble			}
// PL_change no Preamble			fir_f_init(&(DemodulatorData->zpFir[i_init][0]), 8);
// PL_change no Preamble		}
		

		
	for (i_init = 0; i_init < (DAPSK_ACQU_SAMPLES_PER_SYMBOL+1); i_init++)
	{
		DemodulatorData->z_y0[i_init].re = 0.0;
		DemodulatorData->z_y0[i_init].im = 0.0;
	}
//	vset_c_D(DemodulatorData->z_y0, initvalue , DAPSK_ACQU_SAMPLES_PER_SYMBOL+1);
		
// PL_change no Preamble		for (i_init = 0; i_init < (DAPSK_ACQU_NO_CORR_SYMBOL*DAPSK_ACQU_SAMPLES_PER_SYMBOL); i_init++)
// PL_change no Preamble		{	DemodulatorData->zyCorrelation[i_init] = 0.0;
// PL_change no Preamble		}
				
	for (i_init = 0; i_init < ((DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1); i_init++)
	{	DemodulatorData->zyReal[i_init] = 0.0;
	}
//	vset_f_D(DemodulatorData->zyReal, 0.0 , (DAPSK_L_PREAMBLE*DAPSK_ACQU_SAMPLES_PER_SYMBOL)+1);


	DemodulatorData->countSignalOn = 0;
	//////////////////////////////////////////////
	// Handshake line: set DCD Flag -> DCD inactive in acquisitionstate
	DemodulatorData->DCDflag = TRUE;	

	DemodulatorData->DCDholdOnTime = 4*DemodulatorData->DCDholdOnCriteria; // factor 4 -> empirical found


// added from case ACQUISITION
	DemodulatorData->dapskOld_m.re  = 1.0;
	DemodulatorData->dapskOld_m.im  = 0.0;
			
	DemodulatorData->dapskOld_mm.re = 1.0;
	DemodulatorData->dapskOld_mm.im = 0.0;

	DemodulatorData->dapskOld_p.re = 1.0;
	DemodulatorData->dapskOld_p.im = 0.0;
			
	DemodulatorData->dapskOld_pp.re = 1.0;
	DemodulatorData->dapskOld_pp.im = 0.0;

	DemodulatorData->tau0 = DAPSK_ADAP_TAU_START;
	DemodulatorData->tau0c = DAPSK_ADAP_TAU_START;
	DemodulatorData->tau = DAPSK_ADAP_TAU_START;

// Adaptive equalizer
	DemodulatorData->P_AGC = 1;

// DAPSK_AST_BEFORE_EQ
	DemodulatorData->dapsk_MFout_Old_m.re  = 1.0;
	DemodulatorData->dapsk_MFout_Old_m.im  = 0.0;
			
	DemodulatorData->dapsk_MFout_Old_p.re = 1.0;
	DemodulatorData->dapsk_MFout_Old_p.im = 0.0;

}
//#pragma optimize_as_cmd_line 


/* Initialization DAPSK ACQUISITION 3 case ***************/
//#pragma optimize_off 
void SEGMENT_EXT_CODE executeInitacquisiton3(t_DAPSK_Demodulator *DemodulatorData)
{
	int i_init;

	// reset adaptive equalizer coefficients
	for (i_init = 0; i_init < (DAPSK_AD_EQ_LENGTH_MAX); i_init++)
	{	
		DemodulatorData->adEq_h[i_init].re=0;
		DemodulatorData->adEq_h[i_init].im=0;
	}
	DemodulatorData->adEq_h[DAPSK_EQUALIZER_INITIAL_TAP-1].re=1;

	// restet delay line od adaptive equalizer
/*	for (i_init = 0; i_init < (2*DAPSK_AD_EQ_LENGTH_MAX); i_init++)
	{	
		DemodulatorData->adEqInput2[i_init].re=0;
		DemodulatorData->adEqInput2[i_init].im=0;
		DemodulatorData->adEqInput2_p[i_init].re=0;
		DemodulatorData->adEqInput2_p[i_init].im=0;
		DemodulatorData->adEqInput2_m[i_init].re=0;
		DemodulatorData->adEqInput2_m[i_init].im=0;
	}
*/
	DemodulatorData->adEqInput2i=DemodulatorData->adEqLength;
	DemodulatorData->adEqInput2i_m=DemodulatorData->adEqLength;
	DemodulatorData->adEqInput2i_p=DemodulatorData->adEqLength;

}
//#pragma optimize_as_cmd_line 

/* Initialization DAPSK ACQUISITION 4 case ***************/
//#pragma optimize_off 
void SEGMENT_EXT_CODE executeInitacquisiton4(t_DAPSK_Demodulator *DemodulatorData)
{
	int i_init;
	// restet delay line of adaptive equalizer
	for (i_init = 0; i_init < (2*DAPSK_AD_EQ_LENGTH_MAX); i_init++)
	{	
		DemodulatorData->adEqInput2[i_init].re=0;
		DemodulatorData->adEqInput2[i_init].im=0;
	}
}
//#pragma optimize_as_cmd_line 


/* Initialization DAPSK ACQUISITION 5 case ***************/
//#pragma optimize_off 
void SEGMENT_EXT_CODE executeInitacquisiton5(t_DAPSK_Demodulator *DemodulatorData)
{
	int i_init;
	// restet delay line of adaptive equalizer
	for (i_init = 0; i_init < (2*DAPSK_AD_EQ_LENGTH_MAX); i_init++)
	{	
		DemodulatorData->adEqInput2_p[i_init].re=0;
		DemodulatorData->adEqInput2_p[i_init].im=0;
	}
}
//#pragma optimize_as_cmd_line 

/* Initialization DAPSK ACQUISITION 6 case ***************/
//#pragma optimize_off 
void SEGMENT_EXT_CODE executeInitacquisiton6(t_DAPSK_Demodulator *DemodulatorData)
{
	int i_init;
	// restet delay line of adaptive equalizer
	for (i_init = 0; i_init < (2*DAPSK_AD_EQ_LENGTH_MAX); i_init++)
	{	
		DemodulatorData->adEqInput2_m[i_init].re=0;
		DemodulatorData->adEqInput2_m[i_init].im=0;
	}
}
//#pragma optimize_as_cmd_line 

/* Initialization DAPSK FPGA OUTPUT CONVERSION ***************/
#pragma optimize_off 
void SEGMENT_EXT_CODE InitRxOutputConversionDAPSK(t_DAPSK_Demodulator *DemodulatorData, t_DAPSK_OutputConversion *ConversionData)
{
	
	ConversionData->i_newestBit = 0;
	ConversionData->i_oldestBit = ConversionData->i_newestBit;
	ConversionData->noOfBits = 0;

	ConversionData->currentBitLeftTime = 0.0;
	ConversionData->rxTimeStock = 0.0;

	ConversionData->nominalBitLength = (float)FPGA_SAMPLING_RATE / DemodulatorData->baud;
	//  For an FPGA sampling rate of 1.024MHz, Nominal_Bit_Length is 106.667 for 9600bps,
    //  213.333 for 4800bps, 426.667 for 2400bps nominal data rate 

	ConversionData->upperQuantity = ConversionData->nominalBitLength * DAPSK_FPGA_UPPERQUANTITY;
	ConversionData->lowerQuantity = ConversionData->nominalBitLength * DAPSK_FPGA_LOWERQUANTITY;

	ConversionData->stateSendDataToFPGA = 0;
	ConversionData->currentBitSendToFPGA = 1;

}
#pragma optimize_as_cmd_line 

/*! DAPSK FPGA OUTPUT CONVERSION 

   Responsible for the modem output conversion. 
   For detailed describtion see [9] (Send_Data_To_FPGA part).

  \b Initfunction:
  InitRxOutputConversionDAPSK(), called in InitDemodulatorDAPSK()

*/
//#pragma optimize_off 
long rxOutputConversionDAPSK(t_DAPSK_Demodulator *DemodulatorData,\
							 t_DAPSK_OutputConversion *ConversionData)
{
	int i_BitNo;
	long FPGA_Data;

	/////////////////////////////////////////////////////
	// Get Symbol from Demodulator
	/////////////////////////////////////////////////////
	if(DemodulatorData->RxSymbolReady) // Rx_Symbol_Available
	{
		/////////////////////////////////////////////////////
		// Put_Rx_Symbol_in_RxData_Fifo
		/////////////////////////////////////////////////////
		for (i_BitNo= 0; i_BitNo<DAPSK_M ; i_BitNo++)
		{
			ConversionData->rxDataFIFO[ConversionData->i_newestBit] = DemodulatorData->IB[i_BitNo];
			ConversionData->noOfBits = ConversionData->noOfBits +1;
			ConversionData->i_newestBit = ConversionData->i_newestBit+1;
//			ConversionData->i_newestBit = ConversionData->i_newestBit % DAPSK_L_OUT_FIFO;
			if(ConversionData->i_newestBit >= DAPSK_L_OUT_FIFO)
			{
				ConversionData->i_newestBit = 0;
			}

			if (ConversionData->noOfBits > DAPSK_L_OUT_FIFO)
			{
				// exception handling instead of ASSERT(FALSE) to avoid DSP crash during Rx RF level adjustment
				InitRxOutputConversionDAPSK(DemodulatorData, ConversionData);
			}

		}
		/////////////////////////////////////////////////////
		// Calculate_Actual_Bit_Length
		/////////////////////////////////////////////////////
		ConversionData->rxTimeStock = (ConversionData->nominalBitLength * ConversionData->noOfBits) + ConversionData->currentBitLeftTime;
		if (ConversionData->rxTimeStock > ConversionData->upperQuantity)
		{
			ConversionData->actualBitLength = DAPSK_FPGA_REDUCE_600PPM * ConversionData->nominalBitLength;
		}
		else if (ConversionData->rxTimeStock < ConversionData->lowerQuantity)
		{
			ConversionData->actualBitLength = DAPSK_FPGA_RISE_600PPM * ConversionData->nominalBitLength;
		}
		else
		{
			ConversionData->actualBitLength = ConversionData->nominalBitLength;
		}

	}


	/////////////////////////////////////////////////////
	// Send_data_to_FPGA
	/////////////////////////////////////////////////////
	switch (ConversionData->stateSendDataToFPGA)
	{
	/////////////////////////////////////////////////////	
	case IDLE_SENDDATATOFPGA:
	/////////////////////////////////////////////////////	
	
		ConversionData->currentBitSendToFPGA = 1;      // 1= Stopbit polarity 
		
		// Wait one "idle bit" before transfer of received data to the FPGA starts: 
		ConversionData->currentBitLeftTime = ConversionData->nominalBitLength;
	
		FPGA_Data = DAPSK_FPGA_CLOCK_NO_TRANS;  // Data = 1, No Clock Transition

		if (ConversionData->noOfBits > 0)     // Data waits in the fifo 
		{
			ConversionData->stateSendDataToFPGA = RECEIVE_SENDDATATOFPGA; 
		}
		
		break;

	/////////////////////////////////////////////////////		
	case RECEIVE_SENDDATATOFPGA:
	/////////////////////////////////////////////////////		
	
		if (ConversionData->currentBitLeftTime >= FPGA_MAIN_LOOP_INTERVAL)
		{
		// Continue current bit
			FPGA_Data = (ConversionData->currentBitSendToFPGA * DAPSK_FPGA_DATA_MOVE_DATABIT) + DAPSK_FPGA_DATA_NO_TRANS;
		}
		else
		{
			if (ConversionData->noOfBits > 0)
			// Start with next bit
			{
				ConversionData->currentBitSendToFPGA =  ConversionData->rxDataFIFO[ConversionData->i_oldestBit];
				ConversionData->rxDataFIFO[ConversionData->i_oldestBit] = 1;
				ConversionData->i_oldestBit = ConversionData->i_oldestBit+1;
//				ConversionData->i_oldestBit = ConversionData->i_oldestBit % DAPSK_L_OUT_FIFO;
				if(ConversionData->i_oldestBit >=DAPSK_L_OUT_FIFO)
				{	ConversionData->i_oldestBit = 0;
				}

				ConversionData->noOfBits = ConversionData->noOfBits - 1;

				FPGA_Data = (ConversionData->currentBitSendToFPGA * DAPSK_FPGA_DATA_MOVE_DATABIT) + (int)ConversionData->currentBitLeftTime;
				
				ConversionData->currentBitLeftTime = ConversionData->currentBitLeftTime + ConversionData->actualBitLength;

			}
			else
			// Fifo is empty
			{
			    FPGA_Data = DAPSK_FPGA_CLOCK_NO_TRANS;  // Data = 1, No Clock Transition
				ConversionData->stateSendDataToFPGA = IDLE_SENDDATATOFPGA; 
			}
		}
		ConversionData->currentBitLeftTime = ConversionData->currentBitLeftTime - FPGA_MAIN_LOOP_INTERVAL;

		break;

	default:
		
		ConversionData->stateSendDataToFPGA = IDLE_SENDDATATOFPGA;
		break;
	}; // end switch(ConversionData->stateSendDataToFPGA)
	
	return FPGA_Data; 

}
//#pragma optimize_as_cmd_line 


/*! DAPSK Status Data collecting 
  
  To write DAPSK status and alarm information into the right structure.

  \b Initfunction:
  No
*/
#pragma optimize_off
void SEGMENT_EXT_CODE statusDataDAPSK(t_DAPSK_InputConversion *InputConvData, t_DAPSK_Modulator *ModulatorData,\
					 t_DAPSK_Demodulator *DemodulatorData, t_NSK_DSP_Status *StatusData, BOOL *nskLinkAlarm, \
					 BOOL *nskDCDAlarm,t_NSK_ShortStatus  *NSK_ShortStatus, BOOL *nskRTSalwaysActive, int ModemNo)
{
	float iErrorCriterion;
// Modulator part------------------------------------------------------------	

	// write FPGA RTS 
	if (ModulatorData->fpgaRTS)
	{	StatusData->NSK.statusFlags[ModemNo] |=  NSK_RTS_FPGA_IN;
	}
	else
	{	StatusData->NSK.statusFlags[ModemNo] &= ~NSK_RTS_FPGA_IN;
	}
	// write Modem RTS 
	if (ModulatorData->modemRTS)
	{	StatusData->NSK.statusFlags[ModemNo] |=  NSK_RTS_MODEM_IN;
	}
	else
	{	StatusData->NSK.statusFlags[ModemNo] &= ~NSK_RTS_MODEM_IN; 
	}
	// write CTS 
	if (ModulatorData->CTS)
	{	StatusData->NSK.statusFlags[ModemNo] |=  NSK_CTS;
	}
	else
	{	StatusData->NSK.statusFlags[ModemNo] &= ~NSK_CTS; 
	}

	// write Buffer over/underflow -> no Buffer Alarm in FSK mode
	StatusData->NSK.counterBufferOverflow[ModemNo] = InputConvData->bufferOverflowCounter;
	StatusData->NSK.counterBufferUnderflow[ModemNo] = InputConvData->bufferUnderflowCounter;


// Demodulator part------------------------------------------------------------	

	// update Link Alarm counter -> do not output link alarm until DCD is active for a definite time 
	DemodulatorData->DCDminOnCount = DemodulatorData->DCDminOnCount + DemodulatorData->DCD;
	DemodulatorData->DCDminOnCount = DemodulatorData->DCDminOnCount * DemodulatorData->DCD;


	// write DCD alarm in case DCD off for more than FSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT
	// reset ALARM------------------------------------------------------------	
	nskDCDAlarm[ModemNo] = FALSE; // reset DCD ALARM (NSK600 modem x no Rx signal detected)
	if(nskRTSalwaysActive[ModemNo])// only in case of "CTS always on"
	{
		DemodulatorData->noDataPickupCount = DemodulatorData->noDataPickupCount+(!DemodulatorData->DCD);
		DemodulatorData->noDataPickupCount = DemodulatorData->noDataPickupCount*(!DemodulatorData->DCD);
		if(DemodulatorData->noDataPickupCount > DAPSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT) // pickup time exceeded
		{
			nskDCDAlarm[ModemNo] = TRUE;
			DemodulatorData->noDataPickupCount = DAPSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT;
		}
	}


	// write DCD and SNR
	if (DemodulatorData->DCD)
	{	
		StatusData->NSK.statusFlags[ModemNo] |=  NSK_DCD;
		StatusData->NSK.SNRLevel[ModemNo] = DAPSK_SNR_LEVEL_CORRECTION_3DB*DemodulatorData->error; //  correction of 3 dB!! see documentation
		// logging of min SNR / max error
		if(StatusData->NSK.SNRLevel[ModemNo]>NSK_ShortStatus->signalQuality)
		{
			NSK_ShortStatus->signalQuality=StatusData->NSK.SNRLevel[ModemNo];
		}

		if (DemodulatorData->error > DAPSK_SNR_LEVEL_18_5DB_REAL_15_5DB)  // worse than 18.5dB
		{
			DemodulatorData->hysteresysFlag = TRUE;
			DemodulatorData->LinkalarmFlag = TRUE;
		}
		if(DemodulatorData->hysteresysFlag)
		{
			if(DemodulatorData->error < DAPSK_SNR_LEVEL_21DB_REAL_18DB) // better than 21dB and LinkalarmFlag set
			{
				DemodulatorData->LinkalarmFlag = FALSE;
				DemodulatorData->hysteresysFlag = FALSE;
			}
		}
		else
		{
			DemodulatorData->LinkalarmFlag = FALSE;
			DemodulatorData->hysteresysFlag = FALSE;
		}

		// To output link alarm -> DCD has to be aktive for a minimal predefined time
		if (DemodulatorData->DCDminOnCount > DAPSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT)
		{
			nskLinkAlarm[ModemNo] = DemodulatorData->LinkalarmFlag;
			DemodulatorData->DCDminOnCount = DAPSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT;

		}
		else
		{
			nskLinkAlarm[ModemNo] = FALSE;
		}
		DemodulatorData->DCDholdOnCriteria = nskLinkAlarm[ModemNo];
	}
	else
	{	StatusData->NSK.statusFlags[ModemNo] &= ~NSK_DCD;
		StatusData->NSK.SNRLevel[ModemNo] = 1;
		nskLinkAlarm[ModemNo] = FALSE;
	}

	// Preamble detected Flag
	if(DemodulatorData->acquisitonStatus*DemodulatorData->DCD)
	{	StatusData->NSK.statusFlags[ModemNo] |=  NSK_ACQUISITION;
	}
	else
	{	StatusData->NSK.statusFlags[ModemNo] &= ~NSK_ACQUISITION;
	}
	
	// write sync status
	if ((DAPSK_SNR_LEVEL_CORRECTION_3DB*DemodulatorData->error < DAPSK_STATUS_SYNCALARM_LEVEL) && (DemodulatorData->DCD))
	{
		StatusData->NSK.statusFlags[ModemNo] &= ~NSK_NOT_SYNC;
	}
	else
	{	
		StatusData->NSK.statusFlags[ModemNo] |= NSK_NOT_SYNC;
	}


// ad Equalizer errorCriterion

	StatusData->equalizer.errorCriterion[ModemNo]=DemodulatorData->errorCriterion;
}

//#pragma optimize_as_cmd_line

/* End of File **************************************************************/



