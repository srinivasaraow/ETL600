/*-------------------------------------------------------------------------*
 * $Workfile: FSK.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : T.Gnus
 * Remarks      :  
 * Purpose      : FSK calculation and interpolation
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source MAS/P4LT/NSK_DSP/C/FSK.c $
 * 
 * 29    22.09.08 13:51 Chhamaa
 * 
 * 28    08-05-07 14:42 Chtognu
 * no Rx Signal Alarm added, mod and demodfunction moved to internal
 * memory
 * 
 * 27    08-03-07 16:54 Chsteic
 * '- #define DAPSK_DETE_MIN_POWER_RCV        0.1
 * - test Flags added
 * - new algorithm
 * - variables renamed
 * - logging of new variables
 * - changed memory map
 * 
 * 26    08-02-25 9:57 Chtognu
 * Merged changes from ad.equ.branch:
 * nskDCDAlarm (NSK modem x no Rx signal detected) added for DAPSK ad.eq
 * (will be ignored for FSK)
 * 
 * 21    12.10.05 9:56 Chchlee
 * private functions removed from interface
 * 
 * 20    11.10.05 14:26 Chtognu
 * assert() removed
 * 
 * 19    10.10.05 18:48 Chtognu
 * New 1200 above speech filters used
 * constants added
 * 
 * 18    20.09.05 17:15 Chtognu
 * minor changes in DCD detection, removed unused comment
 * 
 * 17    19.09.05 17:34 Chtognu
 * SNR Linkalarm changed
 * 
 * 16    16.09.05 15:00 Chtognu
 * Dataregeneration constant (alpha) changed, SNR Linkalarm terms added
 * 
 * 15    5.09.05 11:03 Chtognu
 * new definite outputsignal at the beginning of a transition
 * 
 * 14    29.08.05 10:01 Chtognu
 * SNR measurement stronger averaged, testfunctions removed
 * 
 * 13    22.08.05 8:50 Chtognu
 * new threshold DCDoff, old value cut the transition to early
 * resetvalues added
 * 
 * 12    12.08.05 9:05 Chtognu
 * bug 104 fixed -> SNR Level alarm appears only if the alarm is longer
 * then 4 sec active, no more biterrors after RTS active 
 * 
 * 11    3.08.05 17:32 Chtognu
 * new Filters for Bitrates from 200 to 1200bps
 * 
 * 10    29.06.05 16:23 Chtognu
 * 
 * 9     24.06.05 13:51 Chtognu
 * 
 * 8     8.06.05 13:12 Chtognu
 * #pragma optimize_as_cmd_line at the end of file removed
 * 
 * 7     1.06.05 15:55 Chtognu
 * 50 baud modem added
 * 
 * 6     27.05.05 17:03 Chtognu
 * FSK SNR Alarm level modified
 * 
 * 5     12.05.05 9:22 Chtognu
 * alarm added
 * 
 * 4     29.03.05 8:07 Chtognu
 * Status data changed
 * 
 * 3     2.03.05 9:51 Chtognu
 * Schüsslerfilter 62 added for 1200Baud above speech, tx path. all tx
 * filter 3.order
 * 
 * 2     17.02.05 14:00 Chtognu
 * merged Version with changes done in Prototype 1 till 16.02.05
 * 
 * 
 *-------------------------------------------------------------------------*/
 
 
/* INCLUDE FILES ***********************************************************/
#include <math.h> // to calculate cosf() and sinf() in modulatorFSK() 
                  // and for fabs() in demodulatorFSK()
#include "basetype.h"
#include "DIAG.h"
#include "FSK.h"
#include "FSK_FilterCoef.h"
#include "CONSTANT.H"
#include "NSK_DSP_Constant.h"


/* LOCAL CONSTANT DECLARATIONS *********************************************/
// LOCAL CONSTANT for SNR Calculation
#define CARRIER_MEAN_COEF_1		0.9995
#define CARRIER_MEAN_COEF_2     (1-CARRIER_MEAN_COEF_1)
#define NOISE_MEAN_COEF_1		0.9975
#define NOISE_MEAN_COEF_2		(1-NOISE_MEAN_COEF_1)
#define SNR_MEAN_COEF_1			0.9995
#define SNR_MEAN_COEF_2			(1-SNR_MEAN_COEF_1)

#define FSK_SNR_ALARMLEVEL_100_240_ON		6.5					//9.3//- 9.7dB		//2.0//-3.2dB
#define FSK_SNR_ALARMLEVEL_100_240_OFF      9.5					//12.6//-11.0dB		//3.2//-5.0dB

#define FSK_SNR_ALARMLEVEL_200_360_ON       7.0					//2.2//-3.4dB
#define FSK_SNR_ALARMLEVEL_200_360_OFF      9.6					//3.2//-5.0dB

#define FSK_SNR_ALARMLEVEL_200_480_ON       6.6					//2.0//-3.2dB
#define FSK_SNR_ALARMLEVEL_200_480_OFF      8.9					//3.2//-5.0dB

#define FSK_SNR_ALARMLEVEL_300_480_ON							6.4//2.4//-3.8dB // no longer in use
#define FSK_SNR_ALARMLEVEL_300_480_OFF							9.5//3.2//-5.0dB // no longer in use

#define FSK_SNR_ALARMLEVEL_600_960_ON       7.7					//2.4//-3.8dB
#define FSK_SNR_ALARMLEVEL_600_960_OFF      9.6					//3.2//-5.0dB

#define FSK_SNR_ALARMLEVEL_1200_2400_ON	    9.9					//13.2//-11.2dB		//2.7//-4.3dB
#define FSK_SNR_ALARMLEVEL_1200_2400_OFF   13.6					//17.8//-12.5dB		//3.2//-5.0dB

#define FSK_SNR_ALARMLEVEL_1200_1640_ON	   11.7					//2.4//-3.8dB
#define FSK_SNR_ALARMLEVEL_1200_1640_OFF   15.2					//3.2//-5.0dB

#define FSK_BITS_TO_WAIT_TX_START  4				// Nummer of bits to wait from RTS 'high' to CTS 'high'
													// choose from 3 - 4 bits
#define FSK_BITS_TO_WAIT_TX_END    1				// Nummer of bits to wait from RTS 'low' to TXD 'don't care'

#define FSK_BITS_TO_WAIT_RX        2                // Nummer of bits to wait from detectin a carrier and 
													// sending Data 
													// choose from 1 - 2 bits

#define FSK_DCD_ON_LEVEL  0.07					// threshold to detect the carrier is ON

#define FSK_DCD_OFF_LEVEL 0.035					// threshold to detect the carrier is OFF
												// not to high number
												// -> to early DCD off will cause bad SNR performance

#define FSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT   1067	// To output link alarm -> DCD has to be aktive for a 
													// minimal predefined time
													//  640 -> ca. 6sec. (counting period 0.1067kHz)
													//  853 -> ca. 8sec.
													// 1067 -> ca.10sec.
#define FSK_NO_SNR_MEAN_TIME    6400        // no mean value for SNR calculation for a 
											// minimal predefined time
											// 4260 -> ca. 4sec. (counting period 1.067kHz)
											// 6400 -> ca. 6sec.

#define FSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT  540	// min "DCD off" time before output no Rx signal alarm
												// 540 -> ca. 5sec. (counting period 0.1067kHz)

#define MASK_TRANS_BITS_TUDX	0x00000030 // to mask bit 4-5 (Transition) of the Tx User Data (TUDx) 
#define MASK_TRANS_00_TUDX		0x00000000 // No Transition occured, currend level = 0 (bit 4-5 of TUDx)
#define MASK_TRANS_01_TUDX		0x00000010 // Transition  0 -> 1 occured (bit 4-5 of TUDx)
#define MASK_TRANS_10_TUDX		0x00000020 // Transition  1 -> 0 occured (bit 4-5 of TUDx)
#define MASK_TRANS_11_TUDX		0x00000030 // No Transition occured, currend level = 1 (bit 4-5 of TUDx)


#define MASK_CUR_STATE_TUDX		0x00000080 // to mask bit 7 (current state) of the Tx User Data (TUDx)

#define SHIFT_POS_BITS_TUDX		3          // to shift the Position of the Tx User Data (TUDx)
#define MASK_POS_BITS_TUDX		0x0000000F // to mask bit 0-3 of the Tx User Data (TUDx)
#define MASK_NO_TRANS_TUDX		0x0000000C // lowest value for "NO Transition" in this periode

// No Rx Data output to the FPGA (RUDn)
#define NO_TXDATA_OUT_ZERO      0x0000007F
#define NO_TRANSITION_OUT_ONE   0x000000FF
#define TRANSITION_TO_ONE       0x00000080


// LOCAL CONSTANT for Clockregenerator
#define FSK_CLOCKREG_STATE_INACTIVE          0    // inactive state of clockregenerator
#define FSK_CLOCKREG_STATE_ACTIVE            1    // active state of clockregenerator

// Alpha changed after first systemtest (jitter with data regenerator was to high)
#define FSK_CLOCKREG_ALPHA			         0.02 // end weight of the flywheel
//#define FSK_CLOCKREG_ALPHA			         0.05 // end weight of the flywheel

#define FSK_CLOCKREG_MAX_NO_BIT_COUNTER	  1000    // The upper limit for bitCounter should be chosen >> 1/ALPHA 
#define FSK_CLOCKREG_MAX_NO_EQUAL_BITS		11    // Maximum number of equal bits in active state
#define FSK_FPGA_DATA_POSITION_MASK        127    // Mask to seperate the transition
#define FSK_FPGA_DATA_BITVALUE_MASK        128    // Mask to seperate the bitvalue

/* LOCAL TYPE DECLARATIONS *************************************************/
/* LOCAL MACRO DEFINITIONS *************************************************/
/* LOCAL VARIABLES *********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

static void SEGMENT_EXT_CODE InitTxFilterFSK(t_FSK_TxFilter *filterData, float pm *coef, int numOfSec);
static void SEGMENT_EXT_CODE InitRxFilterFSK(t_FSK_RxFilter *filterData, float pm *coef, int numOfSec);

static void SEGMENT_EXT_CODE InitTxTimingFSK(t_FSK_Modulator *modulatorData, t_FSK_TxTiming *TxTimingData);
static void SEGMENT_EXT_CODE InitCarrierDetectionFSK(t_FSK_Demodulator *demodulatorData, t_FSK_Carrier *CarrierData);
static void SEGMENT_EXT_CODE InitClockregeneratorFSK(t_FSK_Clockreg *ClockregData, t_FSK_Demodulator *DemodulatorData);

/* IMPLEMENTATION **********************************************************/

/* t_FSK_TxFilter **********************************************************/
#pragma optimize_off 
static void SEGMENT_EXT_CODE InitTxFilterFSK(t_FSK_TxFilter *filterData, float pm *coef, int numOfSec)
{
  register long i;
  
  filterData->numOfSec = numOfSec;
  filterData->coefPtr = coef;
   
  // initialize filter state arrays 	
  for (i = 0; i < (2*filterData->numOfSec); i++) // @CL@ could be speeded up by using MEMCPY
  {                                          
	filterData->ziI[i] = 0.0;
	filterData->ziQ[i] = 0.0;
  }
}
#pragma optimize_as_cmd_line 

/* t_FSK_RxFilter **********************************************************/
#pragma optimize_off 
static void SEGMENT_EXT_CODE  InitRxFilterFSK(t_FSK_RxFilter *filterData, float pm *coef, int numOfSec)
{
  register long i;
  
  filterData->numOfSec = numOfSec;
  filterData->coefPtr = coef;
   
  // initialize filter state arrays 	
  for (i = 0; i < (2*NUM_OF_BIQ_SEC_RX); i++)   // @CL@ could be speeded up by using MEMCPY
  {                                          
	filterData->ziI[i] = 0.0;
	filterData->ziQ[i] = 0.0;
  }
}
#pragma optimize_as_cmd_line 

/* Initialisation of TX Timing RTS and CTS *******************************************/
#pragma optimize_off 
static void SEGMENT_EXT_CODE InitTxTimingFSK(t_FSK_Modulator *modulatorData, t_FSK_TxTiming *txTimingData)
{
	txTimingData->sendData = FALSE; // reset Flag
	txTimingData->sendCarrier = FALSE; // reset Flag
	txTimingData->CTS = FALSE; // reset Flag
	txTimingData->RTS = FALSE; // reset Flag

	txTimingData->waitendStart = floor(FSK_BITS_TO_WAIT_TX_START * (AF_SAMPLE_FREQ / modulatorData->baud));
	txTimingData->counterStart = 0; // initialvalue of the delay counter

	txTimingData->waitendEnd = floor(FSK_BITS_TO_WAIT_TX_END * (AF_SAMPLE_FREQ / modulatorData->baud));
	txTimingData->counterEnd = 0; // initialvalue of the delay counter
}
#pragma optimize_as_cmd_line 

/*! TX Timing RTS and CTS
*  Generates the hand - shake signals and the correct timing of the transmitter. 
*  After detecting data or receiving the RTS signal the FSK carrier will be switched-on. 
*  After a definite time the CTS will be send to the RTU. Also the switching-off mechanism 
*  will be generated here. For more details see [3] and [6].
*
*  \b Initfunction:
*  
*	InitTxTimingFSK(), called in InitModulatorFSK()
*/
#pragma optimize_off 
void SEGMENT_EXT_CODE txTimingFSK(t_FSK_Modulator *modulatorData, t_FSK_TxTiming *txTimingData)
{
	if (txTimingData->RTS)
	{
		if(txTimingData->CTS) // normal transmit function
		{
			txTimingData->sendCarrier = TRUE;
			txTimingData->sendData = TRUE;
			txTimingData->counterEnd = 0;
		}
		else  // -> (!txTimingData->CTS) start sequence: switch on carrier and wait
		{
			txTimingData->sendCarrier = TRUE;
			txTimingData->sendData = FALSE;
			if (txTimingData->counterStart > txTimingData->waitendStart)
			{
				txTimingData->counterStart = 0;
				txTimingData->CTS = TRUE;
			}
			else
			{
				txTimingData->counterStart = txTimingData->counterStart + 1;
			}
		}
	}
	else // -> (!txTimingData->RTS)
	{
		if(txTimingData->CTS) // stop sequence: wait before switching off
		{
			txTimingData->sendCarrier = TRUE;
			txTimingData->sendData = TRUE;
			if (txTimingData->counterEnd > txTimingData->waitendEnd)
			{
				txTimingData->counterEnd = 0;
				txTimingData->CTS = FALSE;
			}
			else
			{
				txTimingData->counterEnd = txTimingData->counterEnd + 1;
			}
		}
		else // -> (!txTimingData->CTS) no transmission
		{
			txTimingData->sendCarrier = FALSE;
			txTimingData->sendData = FALSE;
		}
	}
}
#pragma optimize_as_cmd_line  

/* FSK_Modulator Initalisation **********************************************************/
#pragma optimize_off 
void SEGMENT_EXT_CODE InitModulatorFSK(t_FSK_Modulator *modulatorData, t_ModemConfig *modemConfig, t_V24PortCfg *v24Config, t_FSK_TxTiming *txTimingData)
{	
	long splitBwTxDataDelay,
	     splitCenterFrequency;

	modulatorData->mode = modemConfig->mode;
	splitBwTxDataDelay = modemConfig->bw_txDataDelay;
	modulatorData->baud = v24Config->datarate;
	modulatorData->bandwidth = ((splitBwTxDataDelay >> 16)& 0xFFFF );
	splitCenterFrequency = modemConfig->centerFrequency;
	modulatorData->txfrequency = ((splitCenterFrequency >> 16)& 0xFFFF );
	
	modulatorData->oldState = NO_TRANSITION_OUT_ONE; // imaginary Start conditions of the "old" inputdata

	modulatorData->modulatorPhi = 0.0; // Start conditions of the FSK modulator Phi 
	
	// Initialising	of the UPMODULATION oscillator:
	oscillator_init(1.0, modulatorData->txfrequency, AF_SAMPLE_FREQ, &modulatorData->txOsc);
	
	//assignment of the shiftfrequency and txfilter 
	if ((modulatorData->baud == 100)&&(modulatorData->bandwidth == 240))
	{   modulatorData->shiftfrequency = F_SHIFT_100_240;
	    InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_100_240_10666, NUM_OF_BIQ_SEC_TX); 	// Initialising	of the Tx Filter
		modulatorData->standardisationFactor = 1.026;  // standardisation factor (tx signal = 1)
//		modulatorData->standardisationFactor = 1.0177;  // standardisation factor (tx signal = 1)

	}
	else if ((modulatorData->baud == 50)&&(modulatorData->bandwidth == 120))
	{   modulatorData->shiftfrequency = F_SHIFT_50_120;
	    InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_50_120_10666, NUM_OF_BIQ_SEC_TX); 	// Initialising	of the Tx Filter
		modulatorData->standardisationFactor = 1.026;  // standardisation factor (tx signal = 1)
	}
	else if ((modulatorData->baud == 200)&&(modulatorData->bandwidth == 360))
	{   modulatorData->shiftfrequency = F_SHIFT_200_360;
        InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_200_360_10666, NUM_OF_BIQ_SEC_TX);	// Initialising	of the Tx Filter
		modulatorData->standardisationFactor = 1.048;  // standardisation factor (tx signal = 1)
//		modulatorData->standardisationFactor = 1.024;  // standardisation factor (tx signal = 1)

	}
	else if ((modulatorData->baud == 200)&&(modulatorData->bandwidth == 480))
	{   modulatorData->shiftfrequency = F_SHIFT_200_480;
        InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_200_480_10666, NUM_OF_BIQ_SEC_TX);	// Initialising	of the Tx Filter
		modulatorData->standardisationFactor = 1.055;  // standardisation factor (tx signal = 1)
	}
	else if ((modulatorData->baud == 300)&&(modulatorData->bandwidth == 480))
	{   modulatorData->shiftfrequency = F_SHIFT_300_480;
        InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_300_480_10666, NUM_OF_BIQ_SEC_TX);	// Initialising	of the Tx Filter
		modulatorData->standardisationFactor = 1.029;  // standardisation factor (tx signal = 1)
	}
	else if ((modulatorData->baud == 600)&&(modulatorData->bandwidth == 960))
	{   modulatorData->shiftfrequency = F_SHIFT_600_960;
        InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_600_960_10666, NUM_OF_BIQ_SEC_TX);	// Initialising	of the Tx Filter
//		modulatorData->standardisationFactor = 1.03;  // standardisation factor (tx signal = 1)
		modulatorData->standardisationFactor = 1.05;  // standardisation factor (tx signal = 1)

	}
    else if ((modulatorData->baud == 1200)&&(modulatorData->bandwidth == 2400))
	{   modulatorData->shiftfrequency = F_SHIFT_1200_2400;
        InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_1200_2400_10666, NUM_OF_BIQ_SEC_TX);	// Initialising	of the Tx Filter
		modulatorData->standardisationFactor = 0.944;  // standardisation factor (tx signal = 1)
	}
	else if ((modulatorData->baud == 1200)&&(modulatorData->bandwidth == 1640))
	{   modulatorData->shiftfrequency = F_SHIFT_1200_1640;
		InitTxFilterFSK(&modulatorData->txFilter, txFilterCoef_1200_1640_10666, NUM_OF_BIQ_SEC_TX);	// Initialising	of the Tx Filter
		// new tx filter: Schuessler10:62.20
		modulatorData->standardisationFactor = 1.0277;  // standardisation factor (tx signal = 1)
		// old NSK filter
		//	modulatorData->standardisationFactor = 0.926;  // standardisation factor (tx signal = 1)	
	}
	else
	{  ASSERT(FALSE);
	}

	// weight of transmitted signal:
	modulatorData->weight = (modemConfig->weight * modulatorData->standardisationFactor); // (weight of signal from HMI)*(standardisation factor)

	
	modulatorData->modulatorPhiDelta = (TWO_PI * (modulatorData->shiftfrequency))/AF_SAMPLE_FREQ;
	
	modulatorData->fskModulatedI = 0.0;     // initialisation of the FSK modulated I Channel
	modulatorData->fskModulatedQ = 0.0;     // initialisation of the FSK modulated Q Channel
	modulatorData->fskFilteredI = 0.0;      // initialisation of the lowpass filtered I channel 
	modulatorData->fskFilteredQ = 0.0;      // initialisation of the lowpass filtered Q channel
	modulatorData->fskUpmodulatedI = 0.0;   // initialisation of the upmodulated I channel 
	modulatorData->fskUpmodulatedQ = 0.0;   // initialisation of the upmodulated Q channel
	modulatorData->fskTx = 0.0;             // initialisation of the transmitting (Tx) Signal

	// InitTxTimingFSK() has to be at the end of this function to make sure 
	// that the demodulator is initialized already
	InitTxTimingFSK(modulatorData, txTimingData);

//	modulatorData->testquadrms = 0.0;
//	modulatorData->testrms = 0.0;
//	modulatorData->testrmscounter = 0;

//	modulatorData->maxfskTx = 0.0;
}
#pragma optimize_as_cmd_line 
  
/* FSK_Demodulator ********************************************************/
#pragma optimize_off 
void SEGMENT_EXT_CODE InitDemodulatorFSK(t_FSK_Demodulator *demodulatorData, t_ModemConfig *modemConfig, \
											    t_V24PortCfg *v24Config, t_FSK_Carrier *CarrierData, \
												t_FSK_Clockreg *ClockregData)
{
    long splitBwTxDataDelay,
		 splitCenterFrequency;
	
	demodulatorData->mode = modemConfig->mode;
	splitBwTxDataDelay = modemConfig->bw_txDataDelay;
	demodulatorData->baud = v24Config->datarate;;
	demodulatorData->bandwidth = ((splitBwTxDataDelay >> 16)& 0xFFFF );
	splitCenterFrequency = modemConfig->centerFrequency;
	demodulatorData->rxfrequency = (splitCenterFrequency & 0xFFFF );
	
	demodulatorData->fskFilteredI = 0.0;
	demodulatorData->fskFilteredQ = 0.0;
	demodulatorData->fskFilteredOldI = 0.0;
	demodulatorData->fskFilteredOldQ = 0.0;
	
	demodulatorData->fskDemodulatedSignal = 0.0;
	demodulatorData->fskDemodulatedFilteredSignal = 0.0;

    demodulatorData->fskRxOut = TRUE;    
	demodulatorData->fskOldRxOut = TRUE; 
	
	// Initialising	of the UPMODULATION oscillator:
	oscillator_init(1.0, demodulatorData->rxfrequency, AF_SAMPLE_FREQ, &demodulatorData->rxOsc);
	
	//assignment of the shiftfrequency  
	if ((demodulatorData->baud == 100)&&(demodulatorData->bandwidth == 240))
	{   demodulatorData->shiftfrequency = F_SHIFT_100_240;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_100_240_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
		demodulatorData->buttercoefPtr =  buttercoef_300;
//		demodulatorData->buttercoefPtr =  buttercoef_500;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_100_240_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_100_240_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 6;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
	else if ((demodulatorData->baud == 50)&&(demodulatorData->bandwidth == 120))
	{   
		demodulatorData->shiftfrequency = F_SHIFT_50_120;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_50_120_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
		demodulatorData->buttercoefPtr =  buttercoef_300;
//		demodulatorData->buttercoefPtr =  buttercoef_500;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_100_240_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_100_240_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 6;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}	
	else if ((demodulatorData->baud == 200)&&(demodulatorData->bandwidth == 360))
	{   demodulatorData->shiftfrequency = F_SHIFT_200_360;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_200_360_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
		demodulatorData->buttercoefPtr =  buttercoef_300;
//		demodulatorData->buttercoefPtr =  buttercoef_500;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_200_360_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_200_360_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 6;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
	else if ((demodulatorData->baud == 200)&&(demodulatorData->bandwidth == 480))
	{   demodulatorData->shiftfrequency = F_SHIFT_200_480;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_200_480_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
//		demodulatorData->buttercoefPtr =  buttercoef_300;
		demodulatorData->buttercoefPtr =  buttercoef_500;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_200_480_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_200_480_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 6;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
	else if ((demodulatorData->baud == 300)&&(demodulatorData->bandwidth == 480))
	{   demodulatorData->shiftfrequency = F_SHIFT_300_480;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_300_480_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
//		demodulatorData->buttercoefPtr =  buttercoef_300;
		demodulatorData->buttercoefPtr =  buttercoef_500;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_300_480_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_300_480_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 4;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
	else if ((demodulatorData->baud == 600)&&(demodulatorData->bandwidth == 960))
	{   demodulatorData->shiftfrequency = F_SHIFT_600_960;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_600_960_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
		//demodulatorData->buttercoefPtr =  buttercoef_1000;
		demodulatorData->buttercoefPtr =  buttercoef_500;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_600_960_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_600_960_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 4;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
    else if ((demodulatorData->baud == 1200)&&(demodulatorData->bandwidth == 2400))
	{   demodulatorData->shiftfrequency = F_SHIFT_1200_2400;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_1200_2400_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
		demodulatorData->buttercoefPtr =  buttercoef_1000;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_1200_2400_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_1200_2400_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 6;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
	else if ((demodulatorData->baud == 1200)&&(demodulatorData->bandwidth == 1640))
	{   demodulatorData->shiftfrequency = F_SHIFT_1200_1640;
	    InitRxFilterFSK(&demodulatorData->rxFilter, rxFilterCoef_1200_1640_10666, NUM_OF_BIQ_SEC_RX);	// Initialising	of the Rx Filter
//		demodulatorData->buttercoefPtr =  buttercoef_800;
//		demodulatorData->buttercoefPtr =  buttercoef_1333;
		demodulatorData->buttercoefPtr =  buttercoef_1000;
//		demodulatorData->buttercoefPtr =  buttercoef_1000_2;
//		demodulatorData->buttercoefPtr =  buttercoef_1000_3;
		CarrierData->fskSNRAlarmlevelON  = FSK_SNR_ALARMLEVEL_1200_1640_ON;
		CarrierData->fskSNRAlarmlevelOFF = FSK_SNR_ALARMLEVEL_1200_1640_OFF;

		CarrierData->oversamplingPerBitForMeancarrier = 3;	// the no of bits for the mean value depends also on
															// SAMPLES_FOR_MEAN_CARRIER
															// SAMPLES_FOR_MEAN_CARRIER/oversamplingPerBitForMeancarrier = Number of Bits!
	}
	else
	{   ASSERT(FALSE);
	}
	
	// InitCarrierDetectionFSK() has to be at the end of this function to make sure 
	// that the demodulator is initialized already
	demodulatorData->weight = modemConfig->weight; // !!!! DO NOT INCLUDE THE  standardisationFactor !!!
	demodulatorData->inverseweight =  1.0 /modemConfig->weight;

	InitCarrierDetectionFSK(demodulatorData, CarrierData);

	InitClockregeneratorFSK(ClockregData, demodulatorData);

}
#pragma optimize_as_cmd_line 
  
/*! FSK Modulator 

	Responsible for the signal processing of the FSK transmitter, including transforming 
	of the oversampled datasignal, filtering and upmodulation. 
	For more detailed description see [5] and the Matlab model [xx].

	\b Initfunction:

	InitModulatorFSK(), called in initModems()
*/

void modulatorFSK(long *TxDataFPGA, t_FSK_Modulator *modulatorData, t_FSK_TxTiming *txTimingData)
// später wird ein pointer auf die eingelesene Datenstruktur übergeben
{	
	register float fskPosition;
	register float fskCurrentState;
	register float fskAdjustment;
	float sinUpmod;
	float cosUpmod;
	
	if (txTimingData->sendData) // TxTiming: send DATA!
	{
		fskPosition = (*TxDataFPGA >> SHIFT_POS_BITS_TUDX) & MASK_POS_BITS_TUDX;
		fskPosition = 11-fskPosition;		
		fskCurrentState = (*TxDataFPGA & MASK_CUR_STATE_TUDX);

		if(modulatorData->oldState == fskCurrentState)
		// old = new -> no transition
		{
			if(fskCurrentState == MASK_CUR_STATE_TUDX)
			// -> old state = 1, currentstate = 1
			{
				modulatorData->modulatorPhi = (modulatorData->modulatorPhi) + (modulatorData->modulatorPhiDelta);
			}
			else
			// -> old state = 0, current state = 0
			{
				modulatorData->modulatorPhi = (modulatorData->modulatorPhi) - (modulatorData->modulatorPhiDelta);
			}
			//modulatorData->testadjustment = 0;


		}
		else
		// a transition is in this period
		{

			if(fskCurrentState == MASK_CUR_STATE_TUDX)
			// -> old state = 0, currentstate = 1
			{
				fskAdjustment = ((modulatorData->modulatorPhiDelta) * (fskPosition/FSK_TX_OVERSAMPLING)) - ((modulatorData->modulatorPhiDelta) * ((FSK_TX_OVERSAMPLING-fskPosition)/FSK_TX_OVERSAMPLING));
				modulatorData->modulatorPhi = (modulatorData->modulatorPhi) + fskAdjustment;
			}
			else
			// -> old state = 1, current state = 0
			{
				fskAdjustment = ((modulatorData->modulatorPhiDelta) * ((FSK_TX_OVERSAMPLING - fskPosition)/FSK_TX_OVERSAMPLING)) - ((modulatorData->modulatorPhiDelta) *(fskPosition/FSK_TX_OVERSAMPLING));
				modulatorData->modulatorPhi = (modulatorData->modulatorPhi) + fskAdjustment;
			}
			//modulatorData->testadjustment = fskAdjustment;
			
			//old state = currentstate
			modulatorData->oldState = fskCurrentState;
		}
	}
	else  // Tx timing: Don't send Data! old state = 0, current state = 0
	{
		modulatorData->oldState = NO_TRANSITION_OUT_ONE; // imaginary Start conditions of the "old" inputdata
		modulatorData->modulatorPhi = (modulatorData->modulatorPhi) + (modulatorData->modulatorPhiDelta);
	}

	// oszilator control and if necessary reset of phi
	modulatorData->modulatorPhi = fmodf(modulatorData->modulatorPhi, TWO_PI);

	// FSK modulated Channels
    modulatorData->fskModulatedI =  cosf(modulatorData->modulatorPhi);
    modulatorData->fskModulatedQ =  sinf(modulatorData->modulatorPhi);  

    // Lowpass filtered Channels
    BiIIR(&modulatorData->fskModulatedI, &modulatorData->fskFilteredI, 
           modulatorData->txFilter.coefPtr, &modulatorData->txFilter.ziI,
           NUM_OF_BIQ_SEC_TX, 1);
    BiIIR(&modulatorData->fskModulatedQ, &modulatorData->fskFilteredQ, 
           modulatorData->txFilter.coefPtr, &modulatorData->txFilter.ziQ,
           NUM_OF_BIQ_SEC_TX, 1);

	if (txTimingData->sendCarrier) // TxTiming: Send CARRIER!
	{
		// Upmodulated Channels
        oscillator_oscF(&cosUpmod, &sinUpmod, &modulatorData->txOsc, 1);
        modulatorData->fskUpmodulatedI = (modulatorData->fskFilteredI) * cosUpmod;
        modulatorData->fskUpmodulatedQ = (modulatorData->fskFilteredQ) * sinUpmod;
     
		// test: RMS of signal
		//modulatorData->testquadrms = modulatorData->testquadrms + ((modulatorData->fskUpmodulatedI + modulatorData->fskUpmodulatedQ)*(modulatorData->fskUpmodulatedI + modulatorData->fskUpmodulatedQ)); 
		//if (modulatorData->testrmscounter < 10000)
		//{ 
		//	modulatorData->testrmscounter = modulatorData->testrmscounter + 1;
		//}
		//else 
		//{
		//	modulatorData->testrms = sqrt(modulatorData->testquadrms / 10000);
		//	modulatorData->testrmscounter = 0;
		//	modulatorData->testquadrms = 0.0;
		//}

        // Creating outputsignal of sending device
		modulatorData->fskTx = modulatorData->weight * (modulatorData->fskUpmodulatedI + modulatorData->fskUpmodulatedQ);
// Test standardisationFactor
//		modulatorData->fskTx =  modulatorData->standardisationFactor * (modulatorData->fskUpmodulatedI + modulatorData->fskUpmodulatedQ);
	}
	else  // TxTiming: Don't send Carrier!
	{
		modulatorData->fskFilteredI = 0.0;      // initialisation of the lowpass filtered I channel 
	    modulatorData->fskFilteredQ = 0.0;      // initialisation of the lowpass filtered Q channel
	    modulatorData->fskUpmodulatedI = 0.0;   // initialisation of the upmodulated I channel 
	    modulatorData->fskUpmodulatedQ = 0.0;   // initialisation of the upmodulated Q channel
	    modulatorData->fskTx = 0.0;             // initialisation of the transmitting (Tx) Signal
	}
// Test standardisationFactor
//		if (modulatorData->maxfskTx < modulatorData->fskTx)
//		{
//			modulatorData->maxfskTx = modulatorData->fskTx;
//		}

}

/*! FSK Demodulator 

	Responsible for the signal processing of the FSK receiver, 
	including downmodulation, filtering and interpolation.
	For more detailed description see [5] and the Matlab model [xx].
	
	\b Initfunction:

	InitDemodulatorFSK(), called in initModems()
*/
void demodulatorFSK(float *RxDataIn, t_FSK_Demodulator *demodulatorData \
									/* testFSK	, t_FSK_Modulator *modulatorData */)
{	
	float cosDownmod;
	float sinDownmod;
	
	register float deltaAmplitude;
	register float transPosition;
	
		
	// Downmodulation to basis band
	oscillator_oscF(&cosDownmod, &sinDownmod, &demodulatorData->rxOsc, 1);
    demodulatorData->fskDownmodulatedI = (*RxDataIn * cosDownmod) * demodulatorData->inverseweight; // demodulatorData->weight;
	demodulatorData->fskDownmodulatedQ = (*RxDataIn * sinDownmod) * demodulatorData->inverseweight; // demodulatorData->weight;

// testFSK nowtesting!! 
// !! remove the Modulator struct from the demodulator function !! 
//demodulatorData->fskDownmodulatedI = modulatorData->fskFilteredI;
//demodulatorData->fskDownmodulatedQ = modulatorData->fskFilteredQ;
// end testFSK	

	// lowpass Filter recived I and Q Data
	demodulatorData->fskFilteredOldI = demodulatorData->fskFilteredI;
    demodulatorData->fskFilteredOldQ = demodulatorData->fskFilteredQ;
    
	BiIIR(&demodulatorData->fskDownmodulatedI, &demodulatorData->fskFilteredI, demodulatorData->rxFilter.coefPtr, 
          &demodulatorData->rxFilter.ziI, NUM_OF_BIQ_SEC_RX, 1);
          
    BiIIR(&demodulatorData->fskDownmodulatedQ, &demodulatorData->fskFilteredQ, demodulatorData->rxFilter.coefPtr, 
          &demodulatorData->rxFilter.ziQ,NUM_OF_BIQ_SEC_RX, 1);

// testFSK 
// !! remove the Modulator struct from the demodulator function !! 
//	demodulatorData->fskFilteredI = modulatorData->fskModulatedI;
//	demodulatorData->fskFilteredQ = modulatorData->fskModulatedQ;
// end testFSK
	

    // Demodulation of the I and Q Channel
	demodulatorData->fskDemodulatedOldSignal = demodulatorData->fskDemodulatedSignal;
	demodulatorData->fskDemodulatedSignal = (demodulatorData->fskFilteredOldI * demodulatorData->fskFilteredQ) 
                           + ((-1) * demodulatorData->fskFilteredOldQ * demodulatorData->fskFilteredI);

// calculating of the arctan-> same result	
//	demodulatorData->testatan = atan(((demodulatorData->fskFilteredOldI * demodulatorData->fskFilteredQ)-(demodulatorData->fskFilteredOldQ *  demodulatorData->fskFilteredI))/((demodulatorData->fskFilteredOldI * demodulatorData->fskFilteredI )+(demodulatorData->fskFilteredOldQ * demodulatorData->fskFilteredQ)));
//	demodulatorData->fskDemodulatedSignal = demodulatorData->testatan;
//	if (( demodulatorData->testatan > -100) | (demodulatorData->testatan < 100))
//	{
//		demodulatorData->fskDemodulatedSignal = demodulatorData->testatan;
//	}
//	else 
//	{
//		demodulatorData->fskDemodulatedSignal = 0;
//	}

	// butterworth filter:
	demodulatorData->fskDemodulatedOldFilteredSignal = demodulatorData->fskDemodulatedFilteredSignal;
	 
	BiIIR(&demodulatorData->fskDemodulatedSignal, &demodulatorData->fskDemodulatedFilteredSignal, demodulatorData->buttercoefPtr, 
          &demodulatorData->butterFilter, 1, 1);
	 
	 demodulatorData->fskOldRxOut = demodulatorData->fskRxOut;// Replace Old with "New"

	 if (demodulatorData->fskDemodulatedFilteredSignal > 0) // new: any startcriteria that are set to zero gets a FALSE 
		// if (demodulatorData->fskDemodulatedFilteredSignal >= 0)//old
	 {
    	demodulatorData->fskRxOut = TRUE;
     }
	 else 
	 {
		 demodulatorData->fskRxOut = FALSE;
	 }

	 
    // liear interpolation
    if (demodulatorData->fskOldRxOut == demodulatorData->fskRxOut)
    // no transition:
    {	
		demodulatorData->demodRUD = (demodulatorData->fskRxOut * FPGA_RUDN_NEW_STATE) + (FPGA_RUDN_NO_TRANS);
    }
    else
    // there is a transition:
    {
    	deltaAmplitude = (fabs(demodulatorData->fskDemodulatedOldFilteredSignal) + fabs(demodulatorData->fskDemodulatedFilteredSignal)) * RECIPROCAL_FSK_RX_INTERP_RATIO;
		transPosition = floorf((fabs(demodulatorData->fskDemodulatedOldFilteredSignal))/deltaAmplitude);

// testFSK
		demodulatorData->demodRUD = (demodulatorData->fskRxOut * FPGA_RUDN_NEW_STATE) + (transPosition);
//		demodulatorData->demodRUD = (demodulatorData->fskRxOut * FPGA_RUDN_NEW_STATE); // test
// end testFSK
    }
}

/* Initialisation of Carrier average calculation ***************************/
#pragma optimize_off 
static void SEGMENT_EXT_CODE InitCarrierDetectionFSK(t_FSK_Demodulator *demodulatorData, t_FSK_Carrier *CarrierData)
// before running this programm the InitDemodulatorFSK() should be executed
{   
	int i;

	CarrierData->currentCarrierPosition = 0;
	CarrierData->currentCarrier = 0.0;
//	CarrierData->DCDoff = 0.035; // not to high number
								 // -> to early DCD off will cause bad SNR performance
//	CarrierData->DCDon = 0.07;
	CarrierData->fskDCD = 0;

//    CarrierData->bitsToWait = 2; // choose from 1 - 2 bits
    CarrierData->waitend = floor(FSK_BITS_TO_WAIT_RX * (AF_SAMPLE_FREQ / demodulatorData->baud));
	CarrierData->delay = 0; // initialvalue of the delay counter

	for (i=0; i < SAMPLES_FOR_MEAN_CARRIER; i++)
	{
		CarrierData->latestCarriers[i] = 0.0;
	}

//	CarrierData->carrierValid = 1200 / demodulatorData->baud;
	CarrierData->carrierValid = AF_SAMPLE_FREQ / (demodulatorData->baud * CarrierData->oversamplingPerBitForMeancarrier); // integer number (dsp samples per bit)
	CarrierData->i_carrierValid = 0;

	CarrierData->SNRalarm = FALSE;
	CarrierData->meanCarrierPower = 0.0;
	CarrierData->meanNoisePower2 = 0.0;
	CarrierData->meanSNR = 0.0;
	CarrierData->reciprocalNumCarrierMean = (float)(1.0/SAMPLES_FOR_MEAN_CARRIER);

// new end

}
#pragma optimize_as_cmd_line 

/*! Average calculation of Carrier 

	For modem signal detection. At first I^2 + Q^2 will be averaged depending on two variables. 
	The number of bits for calculating the averaged is defined 
	by SAMPLES_FOR_MEAN_CARRIER / oversamplingPerBitForMeancarrier. 
	After the average determination the carrier is detected by comparing with a on and off level. 
	For enabling or disabeling the DCD signal additional delays are important see [6]. 
	The receive data signal will only be transmitted to the output when DCD on.

	\b Initfunction:

	InitCarrierDetectionFSK(), called in InitDemodulatorFSK()
*/
#pragma optimize_off
void SEGMENT_EXT_CODE CarrierDetectionFSK(t_FSK_Demodulator *demodulatorData, t_FSK_Carrier *CarrierData )
{	
	register int i;

	CarrierData->i_carrierValid = CarrierData->i_carrierValid+1;
	if (CarrierData->i_carrierValid >= CarrierData->carrierValid)
	{
		CarrierData->i_carrierValid = 0;

		CarrierData->currentCarrierPosition = CarrierData->currentCarrierPosition + 1;
		
		CarrierData->currentCarrierPosition = CarrierData->currentCarrierPosition % SAMPLES_FOR_MEAN_CARRIER;

		CarrierData->latestCarriers[CarrierData->currentCarrierPosition] = ((demodulatorData->fskFilteredI * demodulatorData->fskFilteredI) \
		   	               + (demodulatorData->fskFilteredQ * demodulatorData->fskFilteredQ)); 

		CarrierData->currentCarrier = 0;
		for (i=0; i < SAMPLES_FOR_MEAN_CARRIER; i++)
		{
			CarrierData->currentCarrier = CarrierData->currentCarrier + CarrierData->latestCarriers[i];
   		}
		CarrierData->currentCarrier = CarrierData->currentCarrier * CarrierData->reciprocalNumCarrierMean;
	}

	if (CarrierData->currentCarrier > FSK_DCD_OFF_LEVEL)
	{
		if (!CarrierData->fskDCD)
		{
			demodulatorData->demodRUD = TRANSITION_TO_ONE;

			if (CarrierData->currentCarrier > FSK_DCD_ON_LEVEL)
			{
				demodulatorData->demodRUD = NO_TRANSITION_OUT_ONE;

				if (CarrierData->delay > CarrierData->waitend)
				{
					CarrierData->fskDCD = 1;
					CarrierData->delay = 0;
					demodulatorData->fskRxOut = TRUE;    
					demodulatorData->fskOldRxOut = TRUE; 
				}
				else
				{
					CarrierData->delay = CarrierData->delay + 1;
				}
			}
		}
	}
	else
	{
		// turning off sequenz send "stop bit"
		demodulatorData->demodRUD = NO_TRANSITION_OUT_ONE;
				// old removed:
				//		if ((demodulatorData->demodRUD & 0x00000080)  == 0x00000080)
				//		{
				//			demodulatorData->demodRUD = 0x00000000;
				//		}
				//		else if (demodulatorData->demodRUD == 0x00000000)
				//		{
				//			demodulatorData->demodRUD = NO_TXDATA_OUT_ZERO;
				//		}
		
		CarrierData->fskDCD = 0;
		CarrierData->delay = 0;
	}
}
#pragma optimize_as_cmd_line 
/* SNR calculation of Carrier *******************************************/
#pragma optimize_off
void SEGMENT_EXT_CODE SNRcalculationFSK(t_FSK_Carrier *CarrierData)
{
//	float currentNoise;
	// SNR:
	CarrierData->meanCarrierPower = CarrierData->meanCarrierPower *CARRIER_MEAN_COEF_1 + \
		                            CarrierData->currentCarrier*CARRIER_MEAN_COEF_2;

	CarrierData->sqrtNoisePower =sqrtf(CarrierData->meanCarrierPower) - sqrtf(CarrierData->latestCarriers[CarrierData->currentCarrierPosition]);
	CarrierData->noisePower2 = CarrierData->sqrtNoisePower * CarrierData->sqrtNoisePower * CarrierData->sqrtNoisePower * CarrierData->sqrtNoisePower;

	CarrierData->meanNoisePower2 = CarrierData->meanNoisePower2*NOISE_MEAN_COEF_1 + CarrierData->noisePower2*NOISE_MEAN_COEF_2;

	CarrierData->meanCarrierPower2 = CarrierData->meanCarrierPower*CarrierData->meanCarrierPower;

	if(CarrierData->meanNoisePower2)
	{
		CarrierData->SNR = sqrt(CarrierData->meanCarrierPower2 / CarrierData->meanNoisePower2);

		
		if (CarrierData->noSNRmeanCount<FSK_NO_SNR_MEAN_TIME)
		{
			//dont build a mean value but take the current value
			CarrierData->meanSNR = CarrierData->SNR;
			CarrierData->noSNRmeanCount = CarrierData->noSNRmeanCount + 1;
		}
		else
		{
			// build a mean value
			CarrierData->meanSNR = CarrierData->meanSNR*SNR_MEAN_COEF_1 + CarrierData->SNR*SNR_MEAN_COEF_2;
		}

// Test for alarm levels:
// 
/*
		if (CarrierData->alarmlevelcount< 192060) // 3min.
		{
			if (CarrierData->meanSNR < CarrierData->meanSNRmin_i)
			{
				CarrierData->meanSNRmin_i = CarrierData->meanSNR;
			}
			if (CarrierData->meanSNR > CarrierData->meanSNRmax_i)
			{
				CarrierData->meanSNRmax_i = CarrierData->meanSNR; 
			}

			CarrierData->alarmlevelcount = CarrierData->alarmlevelcount+1;
		}
		else
		{
			CarrierData->meanSNRmin = CarrierData->meanSNRmin_i;
			CarrierData->meanSNRmax = CarrierData->meanSNRmax_i; 
			
			CarrierData->meanSNRmin_i = 60;
			CarrierData->meanSNRmax_i = 0; 

			CarrierData->alarmlevelcount = 0;
		}
// */
// END Test for alarm levels

	}
}
#pragma optimize_as_cmd_line

/* FSK Clockregenerator *************************************************/
#pragma optimize_off
static void SEGMENT_EXT_CODE InitClockregeneratorFSK(t_FSK_Clockreg *ClockregData, t_FSK_Demodulator *DemodulatorData)
{
	ClockregData->nominalBitLenght = (float)FPGA_SAMPLING_RATE / DemodulatorData->baud;
	//  For an FPGA sampling rate of 1.024MHz, Nominal_Bit_Length are:
	//    50 Baud = 20480.0,  100 Baud = 10240.0
	//   200 Baud =  5120.0,  600 Baud =  1706.667
	//  1200 Baud =   853.333
	ClockregData->halfNominalBitLenght = ClockregData->nominalBitLenght/2;

	ClockregData->clockregState = FSK_CLOCKREG_STATE_INACTIVE;


	ClockregData->pieceOfEqualBit = DemodulatorData->baud/AF_SAMPLE_FREQ;

}
#pragma optimize_as_cmd_line

/*! FSK Clockregenerator 

   This function is used for jitter reduction. It is implemented as described in [11]. 
   The regenerator is only  appropriate for the nominal data rate.
   
   \b Initfunction:

   InitClockregeneratorFSK(), called in InitDemodulatorFSK()

*/
#pragma optimize_off
void SEGMENT_EXT_CODE clockregeneratorFSK(t_FSK_Clockreg *ClockregData, t_FSK_Demodulator *DemodulatorData, \
										  t_FSK_Carrier *CarrierData, BOOL *ClockregActiv)
{
	float origPos;
	int regData;
	int regPos;
	float timeCorrection;

	origPos = (DemodulatorData->demodRUD & FSK_FPGA_DATA_POSITION_MASK);
	
	switch (ClockregData->clockregState)
	{
	///////////////////////////////////////////////////////////////////////////////////////////////////	
	case FSK_CLOCKREG_STATE_INACTIVE:
	///////////////////////////////////////////////////////////////////////////////////////////////////	
		regData = (DemodulatorData->demodRUD & FSK_FPGA_DATA_BITVALUE_MASK);
		if((origPos != FSK_FPGA_DATA_POSITION_MASK) && (CarrierData->fskDCD)) // Transition and valid
		{
			regData = ((DemodulatorData->demodRUD + FSK_FPGA_DATA_BITVALUE_MASK) & FSK_FPGA_DATA_BITVALUE_MASK); 
			          // Value of DataBit befor transition
			ClockregData->timeToNextSampling = (float)ClockregData->halfNominalBitLenght + origPos;
			ClockregData->deviation = 0.0;
			ClockregData->noOfTransitions = 0.0;
			ClockregData->noOfEqualBits = ClockregData->pieceOfEqualBit;
			ClockregData->bitCounter = 0;
			ClockregData->clockregState = FSK_CLOCKREG_STATE_ACTIVE;
		}
		
		DemodulatorData->fpgaRUD =  ( *ClockregActiv * (regData + FSK_FPGA_DATA_POSITION_MASK)) + \
			                        (!*ClockregActiv * DemodulatorData->demodRUD);
/*		
		if (*ClockregActiv)
		{
			DemodulatorData->fpgaRUD = regData + FSK_FPGA_DATA_POSITION_MASK;
		}
		else
		{
			DemodulatorData->fpgaRUD = DemodulatorData->demodRUD;
		}
*/		

		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////		
	case FSK_CLOCKREG_STATE_ACTIVE:
	///////////////////////////////////////////////////////////////////////////////////////////////////
		regData = 0;
		ClockregData->timeToNextSampling = ClockregData->timeToNextSampling - FPGA_MAIN_LOOP_INTERVAL;

		// processing of the data signal transition:
		if(origPos != FSK_FPGA_DATA_POSITION_MASK)
		{
			ClockregData->deviation = ClockregData->deviation + ClockregData->halfNominalBitLenght - \
				                      ClockregData->timeToNextSampling + origPos;
			ClockregData->noOfTransitions = ClockregData->noOfTransitions + 1;
			ClockregData->noOfEqualBits = ClockregData->pieceOfEqualBit;
		}
		else
		{
			ClockregData->noOfEqualBits = ClockregData->noOfEqualBits + ClockregData->pieceOfEqualBit;
		}

		// Check if the Active state has to be left
		if ((!CarrierData->fskDCD) ||(ClockregData->noOfEqualBits>FSK_CLOCKREG_MAX_NO_EQUAL_BITS))
		{
			ClockregData->clockregState = FSK_CLOCKREG_STATE_INACTIVE;
		}

		// Sampling of data and timing correction
		regPos = FSK_FPGA_DATA_POSITION_MASK;
		if((ClockregData->timeToNextSampling - FPGA_MAIN_LOOP_INTERVAL) < 0)
		{
			// Sample data: 
			if((ClockregData->timeToNextSampling > origPos)||(origPos == FSK_FPGA_DATA_POSITION_MASK))
			{
				// Data after transition or no transition
				regData = ((DemodulatorData->demodRUD & FSK_FPGA_DATA_BITVALUE_MASK) & FSK_FPGA_DATA_BITVALUE_MASK);
			}
			else
			{
				regData = (DemodulatorData->demodRUD + FSK_FPGA_DATA_BITVALUE_MASK);
			}

			regPos = (int)ClockregData->timeToNextSampling;

			// Adjust timing:
			ClockregData->bitCounter = min((ClockregData->bitCounter+1),FSK_CLOCKREG_MAX_NO_BIT_COUNTER);
			if(ClockregData->noOfTransitions)
			{
				timeCorrection = (FSK_CLOCKREG_ALPHA + (1/ClockregData->bitCounter)) * ClockregData->deviation / ClockregData->noOfTransitions;
			}
			else
			{
				timeCorrection = 0.0;
			}
			ClockregData->timeToNextSampling = ClockregData->timeToNextSampling + timeCorrection + ClockregData->nominalBitLenght;
			ClockregData->deviation = 0.0;
			ClockregData->noOfTransitions = 0;			
		}
		
		DemodulatorData->fpgaRUD =  ( *ClockregActiv * (regData + regPos)) + \
			                        (!*ClockregActiv * DemodulatorData->demodRUD);
		
/*		if (*ClockregActiv)
		{
			DemodulatorData->fpgaRUD =  regData + regPos;
		}
		else
		{
			DemodulatorData->fpgaRUD = DemodulatorData->demodRUD;
		}
*/		
		break;

	///////////////////////////////////////////////////////////////////////////////////////////////////	
	default:
	///////////////////////////////////////////////////////////////////////////////////////////////////	
	
		break;

	}; // end switch (ClockregData->clockregState)
}
#pragma optimize_as_cmd_line
/*! Writing STATUS DATA for HMI 
   To write FSK status and alarm information into the right structure.

   \b Initfunction:

	No

*/
#pragma optimize_off
void SEGMENT_EXT_CODE statusDataFSK(t_FSK_TxTiming *txTimingData, t_FSK_Carrier *CarrierData, t_NSK_DSP_Status *StatusData,\
									BOOL *nskLinkAlarm, BOOL *nskDCDAlarm, BOOL *nskRTSalwaysActive, int ModemNo)
{

// Modulator part------------------------------------------------------------	

	// write RTS (FPGA = MODEM -> in FSK mode)
	if (txTimingData->RTS)
	{	
		StatusData->NSK.statusFlags[ModemNo] |=  NSK_RTS_FPGA_IN;
		StatusData->NSK.statusFlags[ModemNo] |=  NSK_RTS_MODEM_IN;
	}
	else
	{
		StatusData->NSK.statusFlags[ModemNo] &= ~NSK_RTS_FPGA_IN;
		StatusData->NSK.statusFlags[ModemNo] &= ~NSK_RTS_MODEM_IN;
	}

	// write CTS 
	if (txTimingData->CTS)
	{	StatusData->NSK.statusFlags[ModemNo] |=  NSK_CTS;
	}
	else
	{	StatusData->NSK.statusFlags[ModemNo] &= ~NSK_CTS; 
	}

	// write Buffer over/underflow -> no Buffer Alarm in FSK mode
	StatusData->NSK.counterBufferOverflow[ModemNo] = 0;
	StatusData->NSK.counterBufferUnderflow[ModemNo] = 0;


// Demodulator part------------------------------------------------------------	

	// write DCD alarm in case DCD off for more than FSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT
	// reset ALARM------------------------------------------------------------	
	nskDCDAlarm[ModemNo] = FALSE; // reset DCD ALARM (NSK600 modem x no Rx signal detected)
	if(nskRTSalwaysActive[ModemNo])// only in case of "CTS always on"
	{
		CarrierData->noDataPickupCount = CarrierData->noDataPickupCount+(!CarrierData->fskDCD);
		CarrierData->noDataPickupCount = CarrierData->noDataPickupCount*(!CarrierData->fskDCD);
		if(CarrierData->noDataPickupCount>FSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT) // pickup time exceeded
		{
			nskDCDAlarm[ModemNo] = TRUE;
			CarrierData->noDataPickupCount=FSK_MIN_DCDOFF_FOR_NORXSIGNAL_OUT;
		}
	}

	// update Link Alarm counter -> do not output link alarm until DCD is active for a definite time 
	CarrierData->DCDminOnCount = CarrierData->DCDminOnCount + CarrierData->fskSNRalarmCountFlag;
	CarrierData->DCDminOnCount = CarrierData->DCDminOnCount * CarrierData->fskSNRalarmCountFlag;

	// write SNR calculation into SNRalarm
	if (CarrierData->meanSNR < CarrierData->fskSNRAlarmlevelON)
	{ // SNR not ok
		CarrierData->SNRalarm = TRUE;
		CarrierData->SNRhysteresysFlag = TRUE;
	}
	else if(CarrierData->SNRhysteresysFlag)
	{
		if (CarrierData->meanSNR > CarrierData->fskSNRAlarmlevelOFF)
		{ // SNR ok
			CarrierData->SNRalarm = FALSE;
			CarrierData->SNRhysteresysFlag = FALSE;
		}
	}
	else
	{ // SNR ok
		CarrierData->SNRalarm = FALSE;
		CarrierData->SNRhysteresysFlag = FALSE;
	}

	// don't write sync alarm -> no Sync Alarm in FSK mode
	// write DCD
	if (CarrierData->fskDCD)
	{
		CarrierData->fskSNRalarmCountFlag = TRUE;
		CarrierData->SNRalarmOffcount = 0;
		StatusData->NSK.statusFlags[ModemNo] |=  NSK_DCD;
		// To output SNR link alarm -> DCD has to be aktive for a minimal predefined time
		if (CarrierData->DCDminOnCount > FSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT)
		{
			CarrierData->DCDminOnCount = FSK_MIN_DCDON_TIME_FOR_LINKALARM_OUT;

			// write real SNR alarm -> Link Alarm
			nskLinkAlarm[ModemNo] = CarrierData->SNRalarm; 
		}
		else
		{	
			nskLinkAlarm[ModemNo] = FALSE;
		}
	}
	else if (CarrierData->fskSNRalarmCountFlag * nskLinkAlarm[ModemNo]) 
	// delay nskLinkAlarm OFF! in case DCD was on and SNR Linkalarm ON  
	{
		// delay SNR Linkalarm OFF if bad SNR -> reason DCD could toggle because of noise
		CarrierData->SNRalarmOffcount = CarrierData->SNRalarmOffcount + 1;
		if (CarrierData->SNRalarmOffcount> 53 ) // delay DCD OFF for 0.5sec.
		{
			nskLinkAlarm[ModemNo] = FALSE;
			CarrierData->SNRalarmOffcount = 0;
			CarrierData->fskSNRalarmCountFlag = FALSE;
		}
	}
	else
	{
		CarrierData->fskSNRalarmCountFlag = FALSE;
		CarrierData->noSNRmeanCount = 0;
		StatusData->NSK.statusFlags[ModemNo] &= ~NSK_DCD;
		// reset SNR link alarm
		nskLinkAlarm[ModemNo] = FALSE;
	}

    // write SNR
	StatusData->NSK.SNRLevel[ModemNo] = CarrierData->meanSNR;
	//*SNRLevel = CarrierData->meanSNR;
}
//#pragma optimize_as_cmd_line

/* End of File **************************************************************/



