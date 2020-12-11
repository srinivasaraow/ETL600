/*-------------------------------------------------------------------------*
 * $Workfile: AD9777_Init.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : AD9777 (RF DAC) mode control
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/UC_DSP/C/AD9777_Init.c $ 
 * 
 * 16    11-03-23 15:41 Chhamaa
 * RF_PEP_MAX defines for R4 added.
 * 
 * 10    17.03.06 10:52 Chmibuh
 * AD9777_Init_HW_Reset() changed
 * 
 * 8     7.09.05 15:36 Chhamaa
 * Verification of mode control registers added in function
 * AD9777_Init_ModeControlReg()
 * 
 * 7     20.07.05 10:14 Chhamaa
 * IDAC_COARSE/FINE modified for reduced OFDM PAR of 10 dB
 * 
 * 6     24.06.05 16:01 Chhamaa
 * AD9777_IDAC_COARSE and AD9777_IDAC_FINE settings modified
 * 
 * 5     1.06.05 16:24 Chbrbus
 * Reset pulse width for glue logic DMA11 and DAC increased (factor 50).
 * OFDM modem is now starting up more stable. Memory Check temporary
 * disabled. In this combination no more OFDM startup anomalies.
 * 
 * 4     10.03.05 11:10 Chbrbus
 * Commentary added
 * 
 * 3     25.02.05 14:56 Chbrbus
 * Changed DAC current source register for P1LA 40W
 * 
 * 2     16.02.05 16:38 Chhamaa
 * 
 * 7     9.02.05 17:01 Chhamaa
 * AD9777_Init_ModeControlReg() modified
 * 
 * 6     1.09.04 14:25 Buhl01
 * reestablishSPI() added
 * 
 * 5     2.07.04 11:35 Bussli02
 * 
 * 4     21.06.04 16:59 Maag01
 * 
 * 3     15.06.04 17:56 Bussli02
 * reduced DAC current source to 11 mA max: reduces intermod in 2 tone
 * tests and uses now all 16 bit resolution for HPA 46 dBm output with a
 * small reserve without modification of the RF Tx output circuitry
 * 
 * 2     3.06.04 12:06 Maag01
 * 
 * 1     27.05.04 14:36 Maag01
 * Initial version
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include <stdlib.h>
#include <def21161.h>

#include "CONSTANT.H"
#include "AD9777_Init.h"
#include "AD9777_utils.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// number of mode control registers
#define AD9777_N_REG		14

// Mode control register addresses
#define AD9777_REG0_ADDR	0x00
#define AD9777_REG1_ADDR	0x01
#define AD9777_REG2_ADDR	0x02
#define AD9777_REG3_ADDR	0x03
#define AD9777_REG4_ADDR	0x04
#define AD9777_REG5_ADDR	0x05
#define AD9777_REG6_ADDR	0x06
#define AD9777_REG7_ADDR	0x07
#define AD9777_REG8_ADDR	0x08
#define AD9777_REG9_ADDR	0x09
#define AD9777_REG10_ADDR	0x0a
#define AD9777_REG11_ADDR	0x0b
#define AD9777_REG12_ADDR	0x0c
#define AD9777_REG13_ADDR	0x0d

// Mode control register values
// Default values are not written but checked if correctly set
#define AD9777_REG0_VALUE	0x00	// default value
#define AD9777_REG1_VALUE	0xc5	// Bit7-Bit0: 1100 0101
#define AD9777_REG2_VALUE   0x20	// Bit7-Bit0: 0010 0000
#define AD9777_REG3_VALUE	0x00	// default value
#define AD9777_REG4_VALUE	0x00	// default value
#define AD9777_REG5_VALUE	0x00	// IDAC fine gain adjustment, value depends on power setting
#define AD9777_REG6_VALUE   0x00	// IDAC coarse gain adjustment, value depends on power setting
#define AD9777_REG7_VALUE	0x00	// default value
#define AD9777_REG8_VALUE	0x00	// default value
#define AD9777_REG9_VALUE	0x00	// default value
#define AD9777_REG10_VALUE	0x0f	// default value
#define AD9777_REG11_VALUE	0x00	// default value
#define AD9777_REG12_VALUE	0x00	// default value
#define AD9777_REG13_VALUE	0x00	// default value


// IDAC gain values
// Measurements for determining the IDAC register values for Prototype 2:

// 1: Full scale (means scaling factor 1.0 in upconverter) sine wave fout = 500 kHz, P1LA connected 40W
//    Due to upconverter filters, clipping will occur when samples of absolute value 1.00 will be received over the 
//    upconverter TDM bus, no clipping will occur when absolute values =<0.98 will be received on the upconverter
//    TDM bus

// Measurement full scale DAC with max absolute value 0.98
// IDAC_FINE_GAIN = 0x00;
// --------------------------------------------------------------------------------------------------
// | IDAC COARSE | High impedance measurement X3201 | High impedance measurement Backplane TXRF A-B |
// --------------------------------------------------------------------------------------------------
// | 0x0F        | 4.84 Vpp                         | 4.32 Vpp                                      | measured
// --------------------------------------------------------------------------------------------------
// | 0x0E        | 4.54 Vpp                         | 4.06 Vpp                                      | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x0D        | 4.24 Vpp                         | 3.80 Vpp                                      | measured
// --------------------------------------------------------------------------------------------------
// | 0x0C        | 3.96 Vpp                         | 3.54 Vpp                                      | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x0B        | 3.68 Vpp                         | 3.28 Vpp                                      | measured
// --------------------------------------------------------------------------------------------------
// | 0x0A        | 3.38 Vpp                         | 3.02 Vpp                                      | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x09        | 3.08 Vpp                         | 2.76 Vpp                                      | measured
// --------------------------------------------------------------------------------------------------
// | 0x08        | 2.78 Vpp                         | 2.50 Vpp                                      | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x07        | 2.48 Vpp                         | 2.24 Vpp                                      | measured
// --------------------------------------------------------------------------------------------------
// | 0x06        | 2.16 Vpp                         | 1.916 Vpp                                     | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x05        | 1.84 Vpp                         | 1.592 Vpp                                     | measured
// --------------------------------------------------------------------------------------------------
// | 0x04        | 1.524 Vpp                        | 1.364 Vpp                                     | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x03        | 1.208 Vpp                        | 1.136 Vpp                                     | measured
// --------------------------------------------------------------------------------------------------
// | 0x02        | 0.906 Vpp                        | 0.866 Vpp                                     | interpolated
// --------------------------------------------------------------------------------------------------
// | 0x01        | 0.604 Vpp                        | 0.596 Vpp                                     | measured
// --------------------------------------------------------------------------------------------------
// | 0x00        | 0.3025 Vpp                       | 0.27 Vpp                                      | interpolated
// --------------------------------------------------------------------------------------------------

// According to HF front end team, for getting 40 W out of the PA, we need 2.8 Vpp on the backpane TXRF-A 
// and TXRF-B for 40 W output power.
// The measurement shows an impedance of 390 Ohm seen on the backplane between TXRF-A and TXRF-B. According
// to data sheet P1LA the input impedance is 150 Ohm!!!! And the 80 W combination actual measurements shows 
// a level of 3.76 Vpp for 72W output power.
// In order to have enough output power from the P4LT we increased the possible max output value for 0 Series.
//
// ----------------------------------------------------------------------------------------------------------
//
// Following formula has been extracted for the calculation of the Tx output level measured 
// at X3201 with high impedance:
// A: Anti Aliasing Filter gain (LC-filter frequency response ignored, more or less flat up to 500 kHz, max. derivation 0.7 dB)
//    Prototype 1: A = 9.32
//    Prototype 2: A = 5.6  (A[Diff-Single Ended]*A[AD8099])     ! LC-filters ignored, depending from the impedance P4LT is seeing @ TxRF-OUT!
//    0 Series:    A = 5.6  (A[Diff-Single Ended]*A[AD8099])     ! LC-filters ignored, depending from the impedance P4LT is seeing @ TxRF-OUT!
//
//
// R(current): Resistor hanging between Iouta1 and Iouta2 on P4LT
//             This is (2 * 47R0) || (100R + Resistor(L3201 + L3202)), ignoring LC-filter response
//    Prototype 1: R(current) = 50 Ohm
//    Prototype 2: R(current) = 49.5 Ohm
//    0-Series:    R(current) = 49.5 Ohm
//
// R(FSADJ): Resistor for setting the current source, resistor R3118 on P4LT
//    Prototype 1: R(FSADJ) = 2.2 KOhm
//    Prototype 2: R(FSADJ) = 2.2 KOhm
//    0-Series:    R(FSADJ) = 1970 Ohm
//
//
// Vref: Reference voltage on AD9777, using internal reference according to data sheet
//    Prototype 1: Vref = 1.2 V nominal (tolerance 1.14 V...1.26 V)
//    Prototype 2: Vref = 1.2 V nominal (tolerance 1.14 V...1.26 V)
//    0-Series   : Vref = 1.2 V nominal (tolerance 1.14 V...1.26 V)
//
// UC_Value: Scalar absolute value send to the Upconverter DSP, is a Float value
//           For calculating maximum output level @ X3201 (High impedance measurement), set this to 1.0 Float
//           Is usually +-1.0 for full scale DAC
// 
//
//							                   Vref    (                         IDAC_FINE )
// Vout[X3201] = UC_Value * A * R(current) * -------- (  2 * (IDAC_COARSE + 1) - ---------  )    (in Vpeak-peak !!)
//							                 R(FSADJ)  (                            64     )
//
// CAUTION: P4LT has an additional 47 Ohm serie resistor and a LC lowpass filter before the TX signal leaves P4LT.
//          Depending from the PA impendance, we will have other TX-RF levels on the backplane as measured @ X3201.
//
//
//     Level measured @ X3201 (high impedance measuring)
//                 |
//                 |        ------ 
//     ---------------------|	 |-----------
//                          ------           |							
//                    47 Ohm + Lowpass       |
//                                          ---
//											| |  PA input impedance
//											| |
//											| |  Other level measured over this impedance than @ X3201
//                                          ---
//                                           |
//                                           |
//                                           |
//     ---------------------------------------
//
//                      P4LT          |   Backplane with connected PA


// DSP test signal used to measure output signal @ X3201:
// TT_AMPLITUDE1 = 0.98, TT_FREQUENCY1 = 4000, TX_CARRIER_FREQUENCY = 464000.0 (RF freq. = 484 kHz)
// Full scale output power is approx. 3 dB higher than nominal power

#define AD9777_IDAC_COARSE_GAIN_10W		0x08	// 2.99 Vpp @ X3201
#define AD9777_IDAC_COARSE_GAIN_20W		0x0C	// 4.21 Vpp @ X3201
#define AD9777_IDAC_COARSE_GAIN_40W		0x0F	// 5.30 Vpp @ X3201
#define AD9777_IDAC_COARSE_GAIN_80W		0x0F	// 5.30 Vpp @ X3201

#define AD9777_IDAC_FINE_GAIN_10W		0x00
#define AD9777_IDAC_FINE_GAIN_20W		0x33
#define AD9777_IDAC_FINE_GAIN_40W		0x00
#define AD9777_IDAC_FINE_GAIN_80W		0x00


/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL VARIABLES ***********************************************************/

/* IMPLEMENTATION ************************************************************/

void AD9777_Init_HW_Reset(void)
{
	long tempWait = 0;

	// reset AD9777 active high
	*(long*)(IOFLAG) |= FLG6;
    for(tempWait = 200;tempWait;tempWait--)	
	{
		asm("nop;");
	}
	*(long*)(IOFLAG) &= ~(FLG6);
}


BOOL AD9777_Init_ModeControlReg(long rfPepMax)
{
	long i;
	int writeStatus;
	BOOL initFailure = 0;
	int regWrite[AD9777_N_REG];
	int regRead[AD9777_N_REG];

	
	// init register values
	regWrite[AD9777_REG0_ADDR] = AD9777_REG0_VALUE;
	regWrite[AD9777_REG1_ADDR] = AD9777_REG1_VALUE;
	regWrite[AD9777_REG2_ADDR] = AD9777_REG2_VALUE;
	regWrite[AD9777_REG3_ADDR] = AD9777_REG3_VALUE;
	regWrite[AD9777_REG4_ADDR] = AD9777_REG4_VALUE;
	regWrite[AD9777_REG5_ADDR] = AD9777_REG5_VALUE;
	regWrite[AD9777_REG6_ADDR] = AD9777_REG6_VALUE;
	regWrite[AD9777_REG7_ADDR] = AD9777_REG7_VALUE;
	regWrite[AD9777_REG8_ADDR] = AD9777_REG8_VALUE;
	regWrite[AD9777_REG9_ADDR] = AD9777_REG9_VALUE;
	regWrite[AD9777_REG10_ADDR] = AD9777_REG10_VALUE;
	regWrite[AD9777_REG11_ADDR] = AD9777_REG11_VALUE;
	regWrite[AD9777_REG12_ADDR] = AD9777_REG12_VALUE;
	regWrite[AD9777_REG13_ADDR] = AD9777_REG13_VALUE;

	switch(rfPepMax) {

		case RF_PEP_MAX_10W:
		case RF_PEP_MAX_12_5W:

			regWrite[AD9777_REG5_ADDR] = AD9777_IDAC_FINE_GAIN_10W;
			regWrite[AD9777_REG6_ADDR] = AD9777_IDAC_COARSE_GAIN_10W;
			break;

		case RF_PEP_MAX_20W:
		case RF_PEP_MAX_25W:

			regWrite[AD9777_REG5_ADDR] = AD9777_IDAC_FINE_GAIN_20W;
			regWrite[AD9777_REG6_ADDR] = AD9777_IDAC_COARSE_GAIN_20W;
			break;

		case RF_PEP_MAX_40W:
		case RF_PEP_MAX_50W:

			regWrite[AD9777_REG5_ADDR] = AD9777_IDAC_FINE_GAIN_40W;
			regWrite[AD9777_REG6_ADDR] = AD9777_IDAC_COARSE_GAIN_40W;
			break;

		case RF_PEP_MAX_80W:
		case RF_PEP_MAX_100W:

			regWrite[AD9777_REG5_ADDR] = AD9777_IDAC_FINE_GAIN_80W;
			regWrite[AD9777_REG6_ADDR] = AD9777_IDAC_COARSE_GAIN_80W;
			break;

		default:

			regWrite[AD9777_REG5_ADDR] = AD9777_IDAC_FINE_GAIN_10W;
			regWrite[AD9777_REG6_ADDR] = AD9777_IDAC_COARSE_GAIN_10W;
			break;
	};


	// write register 1
	writeStatus = AD9777_Write(AD9777_REG1_ADDR, regWrite[AD9777_REG1_ADDR]);

	// write register 2
	writeStatus = AD9777_Write(AD9777_REG2_ADDR, regWrite[AD9777_REG2_ADDR]);

	// write register 5
	writeStatus = AD9777_Write(AD9777_REG5_ADDR, regWrite[AD9777_REG5_ADDR]);

	// write register 6
	writeStatus = AD9777_Write(AD9777_REG6_ADDR, regWrite[AD9777_REG6_ADDR]);

	// verify mode control registers settings
	for(i = 0; i < AD9777_N_REG; i++) {

		regRead[i] = AD9777_Read(i);
		if (regRead[i] != regWrite[i]) {
			
			initFailure = TRUE;
		};
	};

	return(initFailure);

};
	

//***************************************************************************************
//	void reestablishSPI(void)
//
//	workaround for DSP anomaly 47.
//  If DSP was SPI master it drives MOSI, MISO & CLK pins active even if the transfer was finished.
//	Result: no other SPI master can access the spi bus properly.
//	After reestablishSPI() all SPI pins are open drain (OPD).
//****************************************************************************************
void reestablishSPI(void)
{
	unsigned long spiSET;

	spiSET = MS | WL8 | BAUDR5 | SPIEN | NSMLS | SENDZ | GM | SPTINT | OPD;   // the settings are not important, except OPD - it's a must!!!
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	*(long *)SPITX = 0xFFFF;
	*(long *)SPICTL = spiSET;					// start transfer

	while(!((*(long *)SPISTAT & (RXS0))))		// wait
	{
		asm("nop;");
	}
	*(long *)SPICTL = 0x00000000;	   			// stop transfer
}

	
	
