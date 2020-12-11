/*-------------------------------------------------------------------------*
 * Workfile		: DuSlic.c
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : V. Pogorilyy
 * Remarks      :  
 * Purpose      : DuSlic configuration&implementation
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/DuSlic.c $
 * 
 * 28    08-02-15 10:51 Chrobif
 * + DuSlic_DeactivateFxsIf() added.
 * 
 * 27    08-02-15 9:09 Chrobif
 * FXO and FXS coefficients modified. They were rolled back to label 1.10
 * because new coefficients caused problems in line echo canceler (echo,
 * idle tone and beeping).
 * 
 * 26    08-02-08 15:53 Chrobif
 * + DuSlic_config modified. Line card switche is closed for 100ms in
 * order to discharge the coupling capacitors. Voltarte is present, only
 * if all measurements are above threshhold. This should avoid the false
 * PAX driving error.
 * 
 * 25    08-02-05 9:22 Chrobif
 * + FXS coefficents modified due to HW modification necessary to fix the
 * ring detect problem.
 * 
 * 24    08-02-01 13:57 Chrobif
 * FXO coefficients modified due to HW modification for fixing ring
 * detection problem.
 * 
 * 23    10.12.07 11:58 Chhamaa
 * Functions startLEC() and stopLEC() defined as static.
 * 
 * 22    7.12.07 9:32 Chhamaa
 * Function DuSlic_modePowerdown() modified: power down high impedance
 * mode set.
 * 
 * 21    07-11-27 16:20 Chrobif
 * + Bug fix in delayed Off-hook detection.
 * 
 * 20    07-11-06 9:22 Chrobif
 * Automatic power down disabled for all 2-wire interfaces.
 * 
 * 19    07-09-25 17:31 Chrobif
 * Temporary disable of prohibition of line card switch closing removed.
 * 
 * 18    07-09-25 11:36 Chalnot
 * bug fixed: Index by COP table upload 
 * 
 * 17    07-09-25 8:11 Chvopog
 * Integration function is switched off bei voltage measurements
 * 
 * 16    07-09-21 16:57 Chrobif
 * Temporarily disable prohibition of line card switch closing. This
 * change will be undo if problem with voltage measurement is solved.
 * 
 * 15    07-09-21 16:46 Chrobif
 * Rollback (Point of call of DuSlic_startEDSP())
 * 
 * 13    07-09-13 14:57 Chrobif
 * + DuSlic_isOffHookDelayed() added.
 * + Implementation of voltage measurement and random value generation in
 * DuSlic_config().
 * 
 * 12    07-09-05 8:30 Chvopog
 * Register 0x1C changed
 * 
 * 11    07-09-04 10:06 Chvopog
 * - LEC switch ON/OFF added
 * - Voltage measurements changed
 * 
 * 10    07-08-24 10:54 Chvopog
 * #include "binInOut.h" added.
 * 
 * 9     07-08-23 16:50 Chrobif
 * DuSlic_writeCOP() input parameter mode, impedanceInd and ringerFreq
 * introduced.
 * DuSlic_verifyCOP() input parameter selectChannel removed and memcmp()
 * used instead of for loop.
 * DuSlic_startRingTripMeasurement() input parameter selectChannel
 * intoduced.
 * Entire DuSlic configuration moved to DuSlic_config(). Furthermore
 * DuSlic configuration was made dependenc on configuration structure.
 * DuSlic gain set to 6dB. Return value randomValue introduced.
 * 
 * 8     07-08-08 13:50 Chalnot
 * Led functions deleted. Delay with timer module.
 * 
 * 7     07-07-06 13:01 Chvopog
 * FXO/FXS Tables changed. Gains +0 dB, +6 dB, +9 dB added
 * 
 * 6     07-07-02 13:53 Chvopog
 * LOCAL CONSTANT DECLARATIONS ADDED
 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include "assert.h"
#include "string.h"

#include "DuSlic.h"
#include "SPI.h"
#include "sport1.h"
#include "O4CV_Const.h"
#include "Basetype_O4CV.h"
#include "Timer.h"
#include "configuration.h"
#include "status.h"
#include "binInOut.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define DUSLIC_SIZE_COP_TABLE			160
#define DUSLIC_BYTE_SIZE_COP_TABLE_ROW	8
#define DUSLIC_COP_ROW_INDEX_RINGER_FREQ_COEF	14 // zero based
#define DUSLIC_COP_BYTE_OFFSET_RINGER_FREQ_COEF	(DUSLIC_COP_ROW_INDEX_RINGER_FREQ_COEF * \
		                                         DUSLIC_BYTE_SIZE_COP_TABLE_ROW)
#define	DUSLIC_READ_COP_BUFLN		    166
#define	DUSLIC_WRITE_COP_BUFLN			163
#define	DUSLIC_READ_POP_BUFLN		    45
#define	DUSLIC_WRITE_POP_BUFLN			42
#define	DUSLIC_BUF_LENGTH				9
#define	DUSLIC_CHANNEL					5
#define	DUSLIC_NUM_INT					4
#define	SPI_STACK_LENGTH				32
#define	SPI_STACK_WIDE					2
#define	SPI_INT_MASK					10

#define AF_INTERFACE_2_WIRE_PAX_VOLTAGE 10

#define IMPEDANCE_600_OHM	1
#define NUM_OF_DUSLIC_TABLE_WRITE_TRIALS	3
#define RANDOM_VALUE_IN_CASE_OF_ERROR 0
#define OFF_HOOK_DELAY_THRESHOLD	7	// 210ms

// Ringer frequency defines
#define RINGER_FREQ_16_HZ				16
#define RINGER_FREQ_20_HZ				20
#define RINGER_FREQ_25_HZ				25
#define RINGER_FREQ_50_HZ				50
#define RINGER_FREQ_60_HZ				60
#define RINGER_FREQ_16_HZ_COEF {0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03}
#define RINGER_FREQ_20_HZ_COEF {0xAB,0x0A,0x23,0x24,0x34,0x24,0x24,0x03} 
#define RINGER_FREQ_25_HZ_COEF {0xAB,0x3A,0x1B,0xC0,0x33,0x24,0xAC,0x02}
#define RINGER_FREQ_50_HZ_COEF {0xAB,0xDA,0x0A,0xC0,0x52,0x23,0xAC,0x01}
#define RINGER_FREQ_60_HZ_COEF {0xAB,0xDA,0x0B,0xD3,0x32,0x23,0xE0,0x01}
const unsigned char ringerFreqCoef16Hz[DUSLIC_BYTE_SIZE_COP_TABLE_ROW] = RINGER_FREQ_16_HZ_COEF;
const unsigned char ringerFreqCoef20Hz[DUSLIC_BYTE_SIZE_COP_TABLE_ROW] = RINGER_FREQ_20_HZ_COEF;
const unsigned char ringerFreqCoef25Hz[DUSLIC_BYTE_SIZE_COP_TABLE_ROW] = RINGER_FREQ_25_HZ_COEF;
const unsigned char ringerFreqCoef50Hz[DUSLIC_BYTE_SIZE_COP_TABLE_ROW] = RINGER_FREQ_50_HZ_COEF;
const unsigned char ringerFreqCoef60Hz[DUSLIC_BYTE_SIZE_COP_TABLE_ROW] = RINGER_FREQ_60_HZ_COEF;

////////////////////////////////////
// COP coefficient tables for FXS //
////////////////////////////////////

// Active state 5VDC!!! OK. Imp Austria  600 Ohm, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz, 
const unsigned char COP_VoltageMeasurements_FXS[160] =
{     
	0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x5B,0x58,0x20,0x2A,0x23,0x02, // TH-Filter part 2 
	0xA2,0x33,0xBA,0x42,0xA5,0xF9,0x88,0x00, // TH-Filter part 3 
	0x08,0x20,0xCB,0x32,0x46,0xEC,0xA4,0x07, // FRR-Filter       
	0x08,0xB0,0xF9,0xA3,0x35,0xFA,0xC4,0x06, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2              
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX              
	0x3D,0x2F,0xE3,0xA4,0x6D,0xDA,0xA2,0x00, // IM-Filter part 1 
	0x10,0x00,0x09,0x08,0x00,0x7D,0x02,0x00, // IM-Filter part 2 
	0xAB,0xDA,0x0B,0xD3,0x32,0x23,0xE0,0x01, // Ringing                    	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator  			
	0x2E,0xAE,0xCA,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels           	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters       	                      
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved
};

// OK. Imp Austria  600 Ohm, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance1_FXS[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x5B,0x58,0x20,0x2A,0x23,0x02, // TH-Filter part 2 
	0x22,0x34,0xBA,0x42,0xA5,0xF9,0x88,0x00, // TH-Filter part 3 
	0x08,0xA0,0xCA,0xA3,0xB5,0xEA,0x2D,0x07, // FRR-Filter       
	0x08,0x20,0xFF,0xCA,0x26,0xF3,0x5A,0x07, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2    
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0x3D,0x2F,0xE3,0xA4,0x6D,0xDA,0xA2,0x00, // IM-Filter part 1 
	0x10,0x00,0x09,0x08,0x00,0x7D,0x02,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. Imp FRG 220Ohm+820Ohm+115nF, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance2_FXS[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x52,0x58,0xD0,0x2D,0xA2,0x01, // TH-Filter part 2 
	0xA4,0x2E,0xB2,0x44,0x24,0xE3,0x88,0x00, // TH-Filter part 3 
	0x08,0x20,0xBC,0x9C,0x27,0xF2,0x2B,0x07, // FRR-Filter       
	0x08,0x30,0x2A,0x73,0x25,0x79,0xB2,0x07, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2    
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0xCB,0xA4,0x4E,0xF9,0xB4,0x4D,0xA2,0x00, // IM-Filter part 1 
	0x2C,0x02,0x29,0x8D,0x02,0x9A,0x02,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator 
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. Imp British Telecom 30Ohm+1000Ohm+220nF, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance3_FXS[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x5E,0xF8,0xB0,0x2A,0xAB,0x01, // TH-Filter part 2 
	0x8F,0xB6,0xAC,0xAA,0xA3,0xCB,0x88,0x00, // TH-Filter part 3 
	0x08,0xB0,0xB3,0x3B,0x2C,0xF4,0xAB,0x07, // FRR-Filter       
	0x08,0x50,0x2D,0xB2,0xC3,0x52,0x2B,0x05, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2    
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0xCC,0xB4,0x42,0xAB,0x23,0x3D,0xA2,0x00, // IM-Filter part 1 
	0x50,0x03,0x29,0x8B,0x0C,0xA6,0x01,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator 	
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// Almost OK. NTT. 600Ohm 1.uF, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz
// only this imp. made with manual Filter optimisation ! 3dB corner = 30Hz
const unsigned char COP_Impedance4_FXS[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0xE3,0xE8,0x20,0x3B,0xAC,0x01, // TH-Filter part 2 
	0x3B,0xA4,0xAA,0x3A,0x23,0xBA,0x88,0x00, // TH-Filter part 3 
	0x08,0x20,0xCB,0x33,0xDE,0xD3,0xAA,0x0E, // FRR-Filter       
	0x08,0x70,0x28,0x32,0x75,0x28,0x2A,0x06, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2   
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0x3B,0x2B,0xBC,0x4B,0x3E,0x32,0xA2,0x00, // IM-Filter part 1 
	0xAF,0x07,0xA9,0x93,0x09,0xC4,0x05,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// Almost  OK!!! USA. 900Ohm 1.16uF, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance5_FXS[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0xFA,0xC8,0xB0,0x33,0xA3,0x01, // TH-Filter part 2 
	0x8C,0x40,0xB2,0xB2,0xD3,0xCB,0x88,0x00, // TH-Filter part 3 
	0x08,0xB0,0xEA,0x93,0x3F,0xE2,0x92,0x0F, // FRR-Filter       
	0x08,0x30,0xE4,0xA4,0xA5,0xFE,0xAC,0x06, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2   
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0x24,0xAD,0xD2,0xD3,0xB7,0x3A,0xA2,0x00, // IM-Filter part 1 
	0xA0,0x07,0xC9,0x8C,0x06,0x8C,0x09,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. ETSI 270Ohm+750Ohm+150nF, gains +6dB, FXS,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance6_FXS[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x52,0xE8,0xA0,0x2B,0xA2,0x01, // TH-Filter part 2 
	0x8F,0x34,0xAA,0x6F,0x24,0xE2,0x88,0x00, // TH-Filter part 3 
	0x08,0x30,0xB7,0x9B,0x4F,0xF2,0x2A,0x07, // FRR-Filter       
	0x08,0xC0,0x2A,0xA5,0x24,0x7F,0x2A,0x07, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2   
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0xB3,0x24,0x4C,0xD2,0xB4,0x4E,0xA2,0x00, // IM-Filter part 1 
	0x53,0x02,0xB9,0x8C,0x0C,0x5A,0x02,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator 
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

////////////////////////////////////
// COP coefficient tables for FXO //
////////////////////////////////////

// OK. Imp Austria  600 Ohm, gains +6dB, FXO,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance1_FXO[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x7D,0x78,0xA0,0x32,0xA3,0x01, // TH-Filter part 2 
	0x23,0xB4,0xB5,0x32,0xA4,0xDC,0x88,0x00, // TH-Filter part 3 
	0x08,0xB0,0xC4,0xA3,0xA5,0xEA,0x22,0x07, // FRR-Filter       
	0x08,0xA0,0xEA,0xAB,0x26,0xF3,0x4D,0x07, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2    
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0x86,0x20,0xDB,0xD2,0x4D,0xDB,0xA2,0x00, // IM-Filter part 1 
	0x10,0x00,0x99,0x87,0x0A,0x18,0x04,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. Imp FRG 220Ohm+820Ohm+115nF, gains +6dB, FXO,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance2_FXO[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x5B,0x68,0xA0,0x2B,0xA4,0x01, // TH-Filter part 2 
	0xA2,0xDE,0xAA,0xA3,0x43,0xD4,0x88,0x00, // TH-Filter part 3 
	0x08,0xC0,0xBC,0x92,0x47,0xF2,0x2B,0x07, // FRR-Filter       
	0x08,0x20,0x32,0xC2,0xF5,0x28,0xB3,0x07, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2  
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0x52,0x24,0x44,0x3A,0xA4,0x43,0xA2,0x00, // IM-Filter part 1 
	0x2C,0x02,0x99,0x8B,0x0C,0x67,0x03,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. Imp British Telecom 30Ohm+1000Ohm+220nF, gains +6dB, FXO,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance3_FXO[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x62,0x58,0x30,0x2A,0xAA,0x01, // TH-Filter part 2 
	0x92,0xAF,0xA5,0xCA,0x73,0xC3,0x88,0x00, // TH-Filter part 3 
	0x08,0x30,0xB3,0x4A,0xBC,0xF5,0xB3,0x07, // FRR-Filter       
	0x08,0xB0,0x2D,0xBB,0x23,0x52,0x23,0x05, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2   
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0xC6,0xB4,0x42,0xAD,0x23,0x3C,0xA2,0x00, // IM-Filter part 1 
	0x50,0x03,0x59,0x8B,0x0E,0xBB,0x01,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator 	
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// Almost OK. NTT. 600Ohm 1.uF, gains +6dB, FXO,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance4_FXO[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0xDC,0xD8,0xA0,0x4E,0xAA,0x01, // TH-Filter part 2 
	0x2A,0xA4,0xAD,0xA2,0xD2,0xBB,0x88,0x00, // TH-Filter part 3 
	0x08,0x70,0x58,0xBA,0x4D,0xDB,0xAB,0x0E, // FRR-Filter       
	0x08,0x20,0x5A,0xA2,0x54,0x79,0xAA,0x05, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2   
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0x22,0xB6,0x5A,0x33,0x45,0x4B,0xA2,0x00, // IM-Filter part 1 
	0xA0,0x07,0x49,0x8E,0x06,0x15,0x02,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. USA. 900Ohm 1.16uF, gains +6dB, FXO,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance5_FXO[DUSLIC_SIZE_COP_TABLE] =
{ 
    0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0xEA,0xE8,0xB0,0x33,0xAC,0x01, // TH-Filter part 2 
	0xAD,0xC4,0xAA,0x42,0xB3,0xC2,0x88,0x00, // TH-Filter part 3 
	0x08,0x40,0xCC,0xEB,0x67,0xE2,0x92,0x07, // FRR-Filter       
	0x08,0xF0,0x7B,0xA3,0x35,0xF9,0xCB,0x06, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2   
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0xBA,0x2C,0xC5,0x3B,0x2D,0x55,0xA2,0x00, // IM-Filter part 1 
	0xA0,0x07,0x29,0x8E,0x0C,0xFC,0x0A,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

// OK. ETSI 270Ohm+750Ohm+150nF, gains +6dB, FXO,  Ring 45Vrsm 20V Ofset, 16Hz
const unsigned char COP_Impedance6_FXO[DUSLIC_SIZE_COP_TABLE] =
{ 
	0x00,0x08,0x08,0x81,0x00,0x80,0x00,0x08, // TH-Filter part 1 
	0x81,0x00,0x5B,0xC8,0x20,0x2B,0x92,0x00, // TH-Filter part 2 
	0x8F,0xA5,0xAA,0xA2,0xA3,0xD2,0x88,0x00, // TH-Filter part 3 
    0x08,0x20,0xB6,0xBC,0x2F,0xF3,0x2A,0x07, // FRR-Filter       
	0x08,0x30,0x2A,0xA2,0xF4,0x7B,0x2A,0x07, // FRX-Filter       
	0xCA,0xDD,0x75,0xE6,0x00,0x00,0x00,0x00, // AR-Filter    
    0x7B,0xCC,0xC1,0x1C,0x00,0x00,0x00,0x00, // AX-Filter  6 dB  
	0x08,0x40,0x22,0x05,0xB3,0x01,0x00,0x00, // TG1              
	0x09,0x40,0x22,0x05,0x08,0x00,0x00,0x00, // TG2    
	0x96,0x38,0x29,0x96,0xC9,0x2B,0x8B,0x00, // LPR-Filter       
	0x96,0x38,0x29,0xF5,0xFA,0x2B,0x8B,0x00, // LPX-Filter       
	0x90,0x40,0x34,0xDB,0x0E,0xA2,0x2A,0x00, // TTX  
	0xD3,0xA4,0x45,0x42,0xE4,0x43,0xA2,0x00, // IM-Filter part 1 
	0x53,0x02,0x99,0x8B,0x0D,0x17,0x03,0x00, // IM-Filter part 2 
	0xAB,0xEA,0x23,0x70,0x44,0x24,0x63,0x03, // Ringing  16Hz 	 	
	0x00,0xC0,0x6D,0xAE,0xB2,0x48,0x08,0x00, // Ramp Generator
	0xC4,0xC9,0x9A,0xEB,0x1A,0xD2,0x22,0x1A, // DC-Parameter     				
	0x7B,0xF3,0x1A,0x7B,0xB3,0x47,0x7B,0x01, // Hook Levels	
	0x3F,0xC3,0x2E,0x2A,0x84,0x90,0x00,0x09, // LP-Filters 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // Reserved		
};

///////////////////////////
// POP coefficient table //
///////////////////////////

//with and only Line Echo Canceler
const unsigned char POP_coef[39]={ 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    		// DTMF 
	0x20,0x30,0x13,0x07,0x60,0x43,0x60,0x00,0x00,		  		// Line Echo Canceller	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,			  		// Caller ID
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 // UTD	
};

/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef enum
{
    
    e_Update,
    e_Delay,
        
} e_DelayState;

typedef struct
{

	UINT16 delayCounter;
	bool isOffHookPrevious;
	bool isOffHookDelayed;
	e_DelayState delayState;

} t_OffHookDelay;

/* LOCAL VARIABLE DECLARATIONS ***********************************************/
extern volatile int dataLength;
extern volatile bool flag_SPIsendfinisched;
unsigned char interupt_2WireArray[DUSLIC_CHANNEL][DUSLIC_NUM_INT]; 					
unsigned char spiCommandTable[SPI_STACK_LENGTH][SPI_STACK_WIDE];  
volatile unsigned char spiCurrentPointer=0;
bool flag_read2wireInterrupt=0;
unsigned char selectIntChannel=0; 					
unsigned char buf[DUSLIC_BUF_LENGTH];
unsigned char read_COPcoef[DUSLIC_READ_COP_BUFLN];
unsigned char write_COPcoef[DUSLIC_WRITE_COP_BUFLN];
unsigned char send_COPcoef [DUSLIC_WRITE_COP_BUFLN];
unsigned char write_POPcoef[DUSLIC_WRITE_POP_BUFLN];
unsigned char read_POPcoef[DUSLIC_READ_POP_BUFLN];
unsigned char read_Int[SPI_INT_MASK];
static t_OffHookDelay gs_offHookDelay[N_CHANNELS_ON_BOARD];
bool errorbitSOP0=0,errorbitSOP1=0,errorbitSOP2=0,errorbitSOP3=0,errorbitSOP4=0;	
bool errorbitCOP0=0,errorbitCOP1=0,errorbitCOP2=0,errorbitCOP3=0,errorbitCOP4=0;	
bool errorbitPOP0=0,errorbitPOP1=0,errorbitPOP2=0,errorbitPOP3=0,errorbitPOP4=0;
	
/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/
static void startLEC(unsigned short selectChannel);
static void stopLEC(unsigned short selectChannel);

/*****************************************************************************
  Function:      UINT32 DuSlic_config()
  Description: 	 DuSlic_config() configures all four channels and the service
  				 phone. Additionally DuSlic_config() generates a radom value
				 by measuring the voltage on a DuSlic. This random value
				 is used by the i-Button module.
  Inputs:      	 none
  Outputs:       none
  Return:      	 UINT32: random value
  Globals:       none
  Important:     The function configurates all 5 channels
  				 The function do not use in time sensetive applications		 
				 sport1_init2wireInterface() must be called before DuSlic_config
*******************************************************************************/
UINT32 DuSlic_config(void)
{
	t_O4CVConfig *pConfig = NULL;
	t_AfInterfaceCfg *pAFInterfaceCfg = NULL;
	t_InterfaceCfg fxsCfg;
	e_2WireIfType ifType;
	bool is2WireConfigured;
	bool writeTableFailed = false;
	bool randomOnly = false;
	bool voltageOnLine[N_CHANNELS_ON_BOARD] = {false, false, false, false};
	UINT16 i;
	UINT16 duslicTableWriteTrials; 
	UINT32 randomValue = RANDOM_VALUE_IN_CASE_OF_ERROR;

	///////////////////////////////////////////
	// Initialize Off-Hook delayed variables //
	///////////////////////////////////////////
	for (i=0; i<N_CHANNELS_ON_BOARD; i++)
	{
		gs_offHookDelay[i].delayCounter = 0;
		gs_offHookDelay[i].isOffHookPrevious = false;
		gs_offHookDelay[i].isOffHookDelayed  = false;
		gs_offHookDelay[i].delayState = e_Update;
	}

	////////////////////////////////////////////////
	// Get a pointer to the current configuration //
	////////////////////////////////////////////////
	pConfig = configuration_getPCurrentConfig();
	
	//////////////////
	// Start SPORT1 //
	//////////////////
	sport1_startRx2wireInterface();
	sport1_startTx2wireInterface();

	////////////////////////////////////////////////////
	// Do voltage measurement on all FXS channels and //
	// generate a random value.						  //
	////////////////////////////////////////////////////
	
	// Reset DuSlics
	DuSlic_hardwarereset();	// 2 sec DELAY by reset!

	// Dermine which channel are configured as FXS.
	configuration_getFxsCfg(&fxsCfg);
	if (fxsCfg.nIf == 0)
	{
		// No FXS channel. Generate random value only
		randomOnly = true;
		fxsCfg.nIf = 1;
		fxsCfg.ix[0] = 0;
	}

	// Configure DuSlics of all FXS channels
	for (i=0 ; i < fxsCfg.nIf; i++)
	{
		// Set on hook. It is VERY important that it is ON HOOK! 
		// We measure the voltage over 15VDC. In OFF Hook state the voltage can drop below 15VDC.
		// So we will not detect connected PAX & this could cause failture of the system!
		DuSlic_OffHook(fxsCfg.ix[i], OFF);

		// Write COP tables
		duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
		while (duslicTableWriteTrials && !writeTableFailed )
		{
			DuSlic_writeCOP(fxsCfg.ix[i], AF_INTERFACE_2_WIRE_PAX_VOLTAGE, IMPEDANCE_600_OHM, RINGER_FREQ_16_HZ);
			while (dataLength>0);
			DuSlic_readCOP(fxsCfg.ix[i]);
			while (dataLength>0);
			if ( DuSlic_verifyCOP() )
			{
				duslicTableWriteTrials--;
				if (!duslicTableWriteTrials)
					writeTableFailed = true;
			}
			else
				break;
		}

		// Write POP tables
		duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
		while (duslicTableWriteTrials && !writeTableFailed )
		{
			DuSlic_writePOP(fxsCfg.ix[i], 0);
			while (dataLength>0);
			DuSlic_readPOP(fxsCfg.ix[i]);
			while (dataLength>0);
			if ( DuSlic_verifyPOP(fxsCfg.ix[i]) )
			{
				duslicTableWriteTrials--;
				if (!duslicTableWriteTrials)
					writeTableFailed = true;
			}
			else
				break;
		}
		
		// Write SOP table
		duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
		while (duslicTableWriteTrials && !writeTableFailed )
		{
			DuSlic_writeALLRegistersSOP(fxsCfg.ix[i]);
			if ( DuSlic_verifyALLRegistersSOP(fxsCfg.ix[i]) )
			{
				duslicTableWriteTrials--;
				if (!duslicTableWriteTrials)
					writeTableFailed = true;
			}
			else
				break;
		}

		// SPI table write alarming
		if (writeTableFailed ) 
		{
			status_setAlarm(O4CV_ALARM_DUSLIC_CONFIGURATION_FAILED);
			return randomValue;
		}
	}

	// Shortly close line card switch for each FXS channel to discharge coupling capacitors
	for (i=0 ; i < fxsCfg.nIf; i++)
	{
		DuSlic_LinecardSwitchSelect(fxsCfg.ix[i], ON);
		while (dataLength>0);
	}
	timer_init(100000, e_Timer1);
	timer_wait(e_Timer1);
	for (i=0 ; i < fxsCfg.nIf; i++)
	{
	    DuSlic_LinecardSwitchSelect(fxsCfg.ix[i], OFF);
		while (dataLength>0);
	}

	// Measure voltage on all FXS channels and generate random value from CH1
	for (i=0 ; i < fxsCfg.nIf; i++)
	{
		DuSlic_modeActive(fxsCfg.ix[i]);
		while (dataLength>0);

		if (!randomOnly)
		{
			voltageOnLine[fxsCfg.ix[i]] = DuSlic_VoltageMeasurement(fxsCfg.ix[i]);
		}
		
		if ( i == 0 )
			randomValue = DuSlic_getRandom_long(fxsCfg.ix[i]);
	}

	///////////////////////////////////////////////////////////////
	// Reset the DuSlics. They will awake in high impedance mode //
	///////////////////////////////////////////////////////////////
	DuSlic_hardwarereset();	// 2 sec DELAY by reset!

	/////////////////////////////////
	// Configure all four channels //
	/////////////////////////////////
	
	// All DuSlics are configured, whether a channel is a configured 2 wire interface or not.
	// If a channel is configured as 2-wire interface the corresponding DuSlic is configured
	// according to the O4CV configuration struct. Otherwise the corresponding DuSlic is configured
	// with a default configuration (mode=2 wire pax, impedance=600Ohm, ringer frequency=16Hz).
	// The explicit configuration of the DuSlic is done to set it in a defined state.
	for (i=0; i<N_CHANNELS_ON_BOARD; i++)
	{
		// Determine if this channel has an enabled 2-wire interface
		is2WireConfigured = configuration_is2wireConfigured(i);

		// Set on hook
   		DuSlic_OffHook(i, OFF);
		
		// Write COP tables
		duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
		while (duslicTableWriteTrials && !writeTableFailed )
		{
			if (is2WireConfigured)
			{
				pAFInterfaceCfg = &pConfig->channel[i].afInterface;
				DuSlic_writeCOP(i, pAFInterfaceCfg->mode, pAFInterfaceCfg->impedanceInd, pAFInterfaceCfg->ringerFreq);
			}
			else
				DuSlic_writeCOP(i, AF_INTERFACE_2_WIRE_PAX, IMPEDANCE_600_OHM, RINGER_FREQ_16_HZ);
			while (dataLength>0);
			DuSlic_readCOP(i);
			while (dataLength>0);
			if ( DuSlic_verifyCOP() )
			{
				duslicTableWriteTrials--;
				if (!duslicTableWriteTrials)
					writeTableFailed = true;
			}
			else
				break;
		}

		// Write POP tables
		duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
		while (duslicTableWriteTrials && !writeTableFailed )
		{
			DuSlic_writePOP(i, 0);
			while (dataLength>0);
			DuSlic_readPOP(i);
			while (dataLength>0);
			if ( DuSlic_verifyPOP(i) )
			{
				duslicTableWriteTrials--;
				if (!duslicTableWriteTrials)
					writeTableFailed = true;
			}
			else
				break;
		}
		
		// Write SOP table
		duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
		while (duslicTableWriteTrials && !writeTableFailed )
		{
			DuSlic_writeALLRegistersSOP(i);
			if ( DuSlic_verifyALLRegistersSOP(i) )
			{
				duslicTableWriteTrials--;
				if (!duslicTableWriteTrials)
					writeTableFailed = true;
			}
			else
				break;
		}

		// SPI table write alarming
		if (writeTableFailed ) 
		{
			status_setAlarm(O4CV_ALARM_DUSLIC_CONFIGURATION_FAILED);
			return randomValue;
		}

		// Close line card switch if 2-wire channel is FXS and no PAX is driving
		// the line.
		if (is2WireConfigured)
		{
			ifType = configuration_get2WireIfType(i);
			switch (ifType)
			{
				case e_FXS:
					if ( !voltageOnLine[i] )
					{
						DuSlic_LinecardSwitchSelect(i, ON);
						while (dataLength>0);
						DuSlic_startRingTripMeasurement(i);
					}
					else
					{
						status_setAlarm(O4CV_ALARM_PAX_DRIVES_FXS_INTERFACE);
					}
					break;

				case e_FXO:
					DuSlic_LinecardSwitchSelect(i, OFF);
					while (dataLength>0);
					break;

				default:
					assert(false);
			}
		}
		else
		{
			DuSlic_LinecardSwitchSelect(i, OFF);
			while (dataLength>0);
		}

		// Start the DSP on the DuSlic
		DuSlic_startEDSP(i);
	}

	/////////////////////////////
	// Configure service phone //
	/////////////////////////////
		
	// The DuSlic used for the service phone is configured, whether the service phone
	// is enabled or not. If the service phone is enabled, the DuSlic is configured
	// according to the O4CV configuration struct. Otherwise, the DuSlic is configured
	// with a default configuration (mode=hot line, impedance=600Ohm, ringer frequency=16Hz).
	// The explicit configuration of the DuSlic is done to set it in a defined state.
	
	// Write COP table
	duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
	while (duslicTableWriteTrials && !writeTableFailed )
	{
		if ( configuration_isServicePhoneActive() )
		{
			pAFInterfaceCfg = &pConfig->servicePhone.afInterface;
			DuSlic_writeCOP(SPH, AF_INTERFACE_HOTLINE, pAFInterfaceCfg->impedanceInd, pAFInterfaceCfg->ringerFreq);
		}
		else
			DuSlic_writeCOP(SPH, AF_INTERFACE_HOTLINE, IMPEDANCE_600_OHM, RINGER_FREQ_16_HZ);
		while (dataLength>0);
		DuSlic_readCOP(SPH);
		while (dataLength>0);
		if ( DuSlic_verifyCOP() )
		{
			duslicTableWriteTrials--;
			if (!duslicTableWriteTrials)
				writeTableFailed = true;
		}
		else
			break;
	}
	
	// Write POP table
	duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
	while (duslicTableWriteTrials && !writeTableFailed )
	{
		DuSlic_writePOP(SPH,0);
		while (dataLength>0);
		DuSlic_readPOP(SPH);
		while (dataLength>0);
		if ( DuSlic_verifyPOP(SPH) )
		{
			duslicTableWriteTrials--;
			if (!duslicTableWriteTrials)
				writeTableFailed = true;
		}
		else
			break;
	}

	// Write SOP table
	duslicTableWriteTrials = NUM_OF_DUSLIC_TABLE_WRITE_TRIALS;
	while (duslicTableWriteTrials && !writeTableFailed )
	{
		DuSlic_writeALLRegistersSOP(SPH);
		if ( DuSlic_verifyALLRegistersSOP(SPH) )
		{
			duslicTableWriteTrials--;
			if (!duslicTableWriteTrials)
				writeTableFailed = true;
		}
		else
			break;
	}
	
	// SPI table write alarming
	if (writeTableFailed ) 
	{
		status_setAlarm(O4CV_ALARM_DUSLIC_CONFIGURATION_FAILED);
		return randomValue;
	}

	// Start ring trip measurement
	DuSlic_startRingTripMeasurement(SPH);

	// Start the DSP on the DuSlic
	DuSlic_startEDSP(SPH);
	
	/////////////////
	// Stop SPORT1 //
	/////////////////
	sport1_stopRx2wireInterface();
	sport1_stopTx2wireInterface();

    return randomValue;
}

void DuSlic_DeactivateFxsIf(void)
{
	UINT16 i;
	t_InterfaceCfg twoWireCfg;
	e_2WireIfType  twoWireType[N_CHANNELS_ON_BOARD];


	// Get 2-wire interface configuration
	configuration_get2wireCfg(&twoWireCfg);

	// Get 2-wire interface type
	for (i=0; i<N_CHANNELS_ON_BOARD; i++)
	{
		if (i<twoWireCfg.nIf)
			twoWireType[i] = configuration_get2WireIfType( twoWireCfg.ix[i] );
		else
			twoWireType[i] = e_Error;
	}

	// Set DuSlics configured as FXS into power down / high impedance state
	for (i=0; i<twoWireCfg.nIf; i++)
	{
		switch ( twoWireType[i] )
		{

			case e_FXS:
	    		DuSlic_SpiWriteFIFO(SLIC_POWERDOWN, twoWireCfg.ix[i]);
				break;

			case e_FXO:
				break;

			default:
				assert(false);
		}
	}
}

/*****************************************************************************
  Function:      DuSlic_SpiFIFO
  Description: 	 The function executes tasks from FIFO
  Inputs:      	 none
  Outputs:       none
  Return:      	 None
  Globals:       The function executes tasks from FIFO
  Important:     The function executes tasks from FIFO
  				 The function works in background  				 
*******************************************************************************/

void DuSlic_SpiFIFO(void){
unsigned int i=0, command,channel;
    if ((flag_SPIsendfinisched==0) && (spiCommandTable[0][0]!=0)){        
				
		command = spiCommandTable[0][0];
		channel = spiCommandTable[0][1];
				
		for (i=0;i<=30;i++){
			spiCommandTable[i][0] = spiCommandTable[i+1][0];
			spiCommandTable[i][1] = spiCommandTable[i+1][1];
			spiCommandTable[i+1][0] = 0;
			spiCommandTable[i+1][1] = 0;			
		}				
    	spiCurrentPointer--;	
    	
		switch (command)
	    {
			case 1 :
			{
			    flag_SPIsendfinisched=1;
			    DuSlic_modePowerdown(channel);
				break;
			}
			case 2 :
			{
			    flag_SPIsendfinisched=1;
				DuSlic_modeActive(channel);
				break;
			}
			case 3 :
			{			    
			    flag_SPIsendfinisched=1;
		    	DuSlic_modeActiveRing(channel);
				break;
			}
			case 4 :			
			{
			    flag_SPIsendfinisched=1;
			    DuSlic_readIntChannel(channel);
				break;
			}
			case 5 :			
			{
			    interupt_2WireArray[channel][0]=read_Int[3];
			    interupt_2WireArray[channel][1]=read_Int[4];
		        interupt_2WireArray[channel][2]=read_Int[5];
		        interupt_2WireArray[channel][3]=read_Int[6];				  	
			    flag_SPIsendfinisched=0;
				break;
			}								
			case 6 :			
			{
			    flag_SPIsendfinisched=1;
			    startLEC(channel);				
				break;
			}
			case 7 :			
			{
			    flag_SPIsendfinisched=1;			    
				stopLEC(channel);
				break;
			}
	    }	    			  
    }
}

/*****************************************************************************
  Function:      DuSlic_SpiWriteFIFO
  Description: 	 The function adds to SPI FIFO stack the tasks to do on all channel
  Inputs:      	 sendCommand: #define	SLICPOWERDOWN		0x01  
							  #define	SLICACTIVE			0x02
							  #define	SLICRING			0x03
							  #define	SLICINTERRUPT		0x04
							  #define	READINTERRUPT		0x05
  				 selectChannel
  Outputs:       True or False. False if the FIFO is full
  Return:      	 None
  Globals:       spiCurrentPointer - shows current position in FIFO
  Important:     The function adds to SPI FIFO stack task to  on all channel
  				 Use the function  in time sencetive application.
*******************************************************************************/

bool DuSlic_SpiWriteFIFO(unsigned short sendCommand,unsigned char selectChannel){     
    if (spiCurrentPointer<30){
		spiCommandTable[spiCurrentPointer][0] =  sendCommand;
		spiCommandTable[spiCurrentPointer][1] =  selectChannel;    
    	spiCurrentPointer++;    	
    	return true;
    }     
    else{
        return false;
    }
}
/*****************************************************************************
  Function:      DuSlic_readAllInt
  Description: 	 The function adds to SPI FIFO stack tasks to read all int on all channel
  Inputs:      	 None
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     The function adds to SPI FIFO stack tasks to read all int on all channel
  				 Use the function  in time sencetive application.
  				 Start the function with 30ms timestamps
*******************************************************************************/

void DuSlic_readAllInt(void)
{
	UINT16 i;
	bool isOffHookNow;
	
	// Handle delayed off-hook detection (only edges from off to on-hook are delayed)
	for (i=0; i<N_CHANNELS_ON_BOARD; i++)
	{
		isOffHookNow = DuSlic_isOffHook(i);

		switch ( gs_offHookDelay[i].delayState )
		{
		    case e_Update:

		    	if ( !isOffHookNow && gs_offHookDelay[i].isOffHookPrevious )
		    	{
		    	    gs_offHookDelay[i].isOffHookDelayed = true;
		    	    gs_offHookDelay[i].delayCounter = 0;
		    	    gs_offHookDelay[i].delayState = e_Delay;
		    	}
		    	else
			    	gs_offHookDelay[i].isOffHookDelayed = isOffHookNow;

		    	break;
		    	
		    case e_Delay:

		    	if ( !isOffHookNow && gs_offHookDelay[i].isOffHookPrevious )
					gs_offHookDelay[i].delayCounter = 0;
				else
		    		gs_offHookDelay[i].delayCounter++;
		    	
		    	if ( gs_offHookDelay[i].delayCounter >= OFF_HOOK_DELAY_THRESHOLD )
		    	    gs_offHookDelay[i].delayState = e_Update;
		    	break;
		    	
		    default:
		    	assert(false);
		    	gs_offHookDelay[i].delayState = e_Update;
		}

		// Update state
		gs_offHookDelay[i].isOffHookPrevious = isOffHookNow;
	}
    
	// Insert new interrupt read command to SPI FIFO
	DuSlic_SpiWriteFIFO(SLIC_INTERRUPT,CH1);   		    		    		  		                	
	DuSlic_SpiWriteFIFO(SLIC_READ_INTERRUPT,CH1);
	DuSlic_SpiWriteFIFO(SLIC_INTERRUPT,CH2);   		    		    		  		                	
	DuSlic_SpiWriteFIFO(SLIC_READ_INTERRUPT,CH2);
	DuSlic_SpiWriteFIFO(SLIC_INTERRUPT,CH3);   
	DuSlic_SpiWriteFIFO(SLIC_READ_INTERRUPT,CH3);
	DuSlic_SpiWriteFIFO(SLIC_INTERRUPT,CH4);
	DuSlic_SpiWriteFIFO(SLIC_READ_INTERRUPT,CH4);
	DuSlic_SpiWriteFIFO(SLIC_INTERRUPT,SPH); 
	DuSlic_SpiWriteFIFO(SLIC_READ_INTERRUPT,SPH);	
}	 

/*****************************************************************************
  Function:      DuSlic_readIntAllChannels
  Description: 	 The function reads interrupts on selected 2 wire Channels
  Inputs:      	  selectIntChannel - select the channel
  Outputs:       None
  Return:      	 None
  Globals:       Fills out Interrupt 2D array 
  Important:     THe function needs 0.750ms time to execute. Tte function works in beckground
  				 Use the function  in time sencetive application.
  				 Start the function with 30ms timestamps
*******************************************************************************/

void DuSlic_readIntChannel(unsigned short selectIntChannel){	 
    
    	switch (selectIntChannel)
	    {
			case 0 :
			{			    
			    DuSlic_readInt(CH1);			    				  					
				break;
			}
			case 1 :
			{			    
			    DuSlic_readInt(CH2);			    				  					
				break;
			}
			case 2 :
			{			    
			    DuSlic_readInt(CH3);			    				  					
				break;
			}		
			case 3 :
			{			    
			    DuSlic_readInt(CH4);			    				  					
				break;
			}
			case 4 :
			{			    
			    DuSlic_readInt(SPH);			    				  					
				break;
			}
	    }
} 	

	    		    		  	
/*****************************************************************************
  Function:      DuSlic_writeALLRegistersSOP
  Description: 	 The function writes all SOP register to selected channel
  Inputs:      	 selectChannel -  number of Channel,
  Outputs:       None
  Return:      	 None
  Globals:       None
  Important:     The function writes all SOP register to selected channel
  				 Uses "while (dataLength>0);" Use it only by Init!
  				 
  				The First byte description: 											//
				Bit 7  6   5 4 3     2 1 0												//
				    RD OP ADR[2:0] CMD[2:0]	

					RD:	0 Write data to chip;1 - Read data from chip;
					OP:	1 The CMD field acts as the SOP, COP, or POP command;
					ADR[2:0]: 000-Channel A,001-Channel B;
					CMD[2:0]: 000-Soft reset of the chip(reset all configuration registers, CRAM data is not affected)
							  001-Soft reset for the specified channel A or B in ADR field
							  010-Resychronization of the PCM interface
							  100-SOP command (Status Operation;programming, and monitoring of all status-relevant data)
							  101-COP command (Coefficient Operation;programming, and monitoring of all coefficients in the CRAM).
							  110-POP command (Signal Processing Operation Programming).
						  

				The second command byte specifies a particular SOP, COP, or POP command,
*******************************************************************************/


void DuSlic_writeALLRegistersSOP(unsigned char selectChannel){
    
     switch (selectChannel)
    {
		case 0 :
		{
		    //-------For Channel AF1
			/*    
			 05H PCMC1 PCM Configuration Register 1
				DBL-CLK X-SLOPE R-SLOPE NO-DRIVE-0 SHIFT PCMO[2:0]
			Bit    7        6       5       4        3    2 1 0
			value: 0		0		0		0		 0    0 0 0   
			*/
			    DuSlic_writeSOP(0,0x05,0x00);
			    while (dataLength>0);
			/*
			06H XCR Extended Configuration Register
				EDSPEN ASYNCH-R 0  0 0 0 0 0
			Bit    7     6      5  4 3 2 1 0
				   0     0      0  0 0 0 0 0
			*/
			    DuSlic_writeSOP(0,0x06,0x00);
				while (dataLength>0);
			/*
			11H  Mask Register 
				Ready_M    HOOK    GNDK    GNKP     ICON VTRLIM OTEMP  Sync-Fail
			Bit    7        6       5       4        3    2 	1   	0
			value: 1		0		1		1		 0    0 	0       0
			*/
			    DuSlic_writeSOP(0,0x11,0xFF);//war B0
				while (dataLength>0);
			/*
			12H I/O Control Register 1
				  IO-INEN     IO-M
			Bit    7 6 5 4    3 2 1 0
			value: 1 1 0 0    1 1 1 1
					IO3/4 input, Int Mask off
			*/
			    DuSlic_writeSOP(0,0x12,0x0F);
				while (dataLength>0);
    
			/*
			13H I/O Control Register 2
				  IO-OEN       IO-DD
			Bit    7 6 5 4    3 2 1 0
			value: 0 0 0 0    0 0 0 0    
			*/  
				DuSlic_writeSOP(0,0x13,0x31); //DISABLE output IO1, Line card switch OFF
			  	while (dataLength>0);
			/*
			14H I/O Control Register 3
				    DUP        DUP-IO
			Bit    7 6 5 4    3 2 1 0
			value: 0 1 0 1    0 0 1 1
				   10ms delay  16ms delay 
			*/
				DuSlic_writeSOP(0,0x14,0x94);
				while (dataLength>0);
			/*
				15H  Basic Configuration Register 1
				  HIR      HIT    Sleep    RevPOL  ACTR  ACTL  SEL-SLIC
			Bit    7        6       5       4        3    2 	1  	0
			value: 0		0		1		0		 0    0 	0   0
							Sleep mode enable 				 SLIC-E/-E2 selected
			*/
				DuSlic_writeSOP(0,0x15,0x00);
				while (dataLength>0);
			/*
			16H  Basic Configuration Register 2
				  REXT-EN SOFT-DIS TTX-dis TTX-12K HIM-AN AC-XGAIN UTDX PDOT
			Bit    7        6       5       4        3    2 		1  	 0
			value: 0		0		1		0		 0    0 		0    0
																		Autopowerdown on!!!
			*/
				DuSlic_writeSOP(0,0x16,0x21);
				while (dataLength>0);
			/*
			17H  Basic Configuration Register 3
				  MU-LAW  LIN   PCM16K   PCMX-EN CONFX-EN CONF-EN  LPRX-CP CRAM-EN
			Bit    7        6       5       4        3    2 		1  	   0
			value: 0		1		0		1		 0    0 		1      1
					     linmode   8KHz	 SV to PCM							CRAM CRAM EN
			*/
				DuSlic_writeSOP(0,0x17,0x53);
				while (dataLength>0);
			/*
			18H  Basic Configuration Register 4
				  TH-DIS  IM-DIS AX-DIS AR-DAS   FRX-Dis FRR-DIS HPX-DIS HPR_DIS
			Bit    7        6       5       4        3    2 		1  	   0
			value: 0		0		0		0		 0    0 		0      0
				   All Filters enable
			*/
				DuSlic_writeSOP(0,0x18,0x00);
				while (dataLength>0);
			/*
			19H  Basic Configuration Register 5
				  EN     UTDX-EN  CIS-AUTO CIS-EN LEC-OUT LEC-EN  DTMF-SRC DTMF-EN
			Bit    7        6       5       4        3    2 		1  	     0
			value: 0		0		0		0		 1    1 		0        0
			*/
				DuSlic_writeSOP(0,0x19,0x0C); //0C, 05
				while (dataLength>0);

			/*	
			1AH  DTMF Sender Configuration Register 
				    DG-KEY  COR8 PTG TG2-EN TG1-EN
			Bit    7 6 5 4    3    2 	1      0
			value: 0 0 0 0    0    0 	0      0	
			*/
				DuSlic_writeSOP(0,0x1A,0x00);
				while (dataLength>0);
	
			/*
			1CH  Level Mettering Configuration Register 1

			Bit    7 6 5 4    3    2 	1      0
			value: 0 0 0 0    0    0 	0      0
			*/	
				//DuSlic_writeSOP(0,0x1C,0x26);
				DuSlic_writeSOP(0,0x1C,0x00);
				while (dataLength>0);
			/*
			1DH  Level Mettering Configuration Register 2

			Bit    7 6 5 4    3    2 	1      0
			value: 0 0 0 0    0    0 	0      0
			*/
				//DuSlic_writeSOP(0,0x1D,0x0A);
				DuSlic_writeSOP(0,0x1D,0x00);
				while (dataLength>0);
			/*
			1EH  Level Mettering Configuration Register 3

			Bit    7 6 5 4    3    2 	1      0
			value: 0 0 0 0    0    0 	0      0
			*/
				DuSlic_writeSOP(0,0x1E,0x00);
				while (dataLength>0);

			/*
			1FH  Offset register1

			Bit    7 6 5 4    3    2 	1      0
			value: 0 0 0 0    0    0 	0      0
			*/
				DuSlic_writeSOP(0,0x1F,0x00);
				while (dataLength>0);
				
			/*
			20H  Offset register1

			Bit    7 6 5 4    3    2 	1      0
			value: 0 0 0 0    0    0 	0      0
			*/
				DuSlic_writeSOP(0,0x20,0x00);
				while (dataLength>0);
	
					
			/*	
			21H  PCM Recieve Register 1
				   R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 0&1
			*/
				DuSlic_writeSOP(0,0x21,0x00);
				while (dataLength>0);
			/*
			22H  PCM Recieve Register 2
				   R4-HW  RS4_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
			*/
				DuSlic_writeSOP(0,0x22,0x00);
				while (dataLength>0);
			/*
			23H  PCM Recieve Register 3
				   R4-HW  RS4_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
			*/
				DuSlic_writeSOP(0,0x23,0x00);
				while (dataLength>0);
			/*
			24H  PCM Recieve Register 4
				   R4-HW  RS4_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
			*/
				DuSlic_writeSOP(0,0x24,0x00);
				while (dataLength>0);
			/*
			25H  PCM Transive Register 1
				   R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 0&1
			*/
				DuSlic_writeSOP(0,0x25,0x00);
				while (dataLength>0);
			/*
			26H  PCM Transive Register 2
				   R4-HW  RS4_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
			*/
				DuSlic_writeSOP(0,0x26,0x00);
				while (dataLength>0);
			/*
			27H  PCM Transive Register 3
				   R4-HW  RS4_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
			*/
				DuSlic_writeSOP(0,0x27,0x00);
				while (dataLength>0);
			/*
			28H  PCM Transive Register 4
				   R4-HW  RS4_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0	   	   
			*/	
				DuSlic_writeSOP(0,0x28,0x00);
				while (dataLength>0);
			
			//Test registers	
			DuSlic_writeSOP(0,0x29,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(0,0x2A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(0,0x2B,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(0,0x2C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(0,0x2D,0x00);
			while (dataLength>0);
				
   			break;
		}
		case 1 :
		{
		    //----For Channel AF2
		    DuSlic_writeSOP(1,0x05,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(1,0x06,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(1,0x11,0xFF); //war B0
		    while (dataLength>0);;
		    DuSlic_writeSOP(1,0x12,0x0F);
		    while (dataLength>0);
			DuSlic_writeSOP(1,0x13,0x32);//DISABLE output IO2, Line card switch OFF
			while (dataLength>0);
			DuSlic_writeSOP(1,0x14,0x94);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x15,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x16,0x21);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x17,0x53);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x18,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x19,0x0C);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x1A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x1C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x1D,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x1E,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x1F,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x20,0x00);
			while (dataLength>0);
	
			/*	
			21H  PCM Recieve Register 1
				   R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 2&3
			*/
			DuSlic_writeSOP(1,0x21,0x02);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x22,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x23,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x24,0x00);
			while (dataLength>0);
			/*
			25H  PCM Transive Register 1
				   R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 2&3
			*/
			DuSlic_writeSOP(1,0x25,0x02);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x26,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x27,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x28,0x00);
			while (dataLength>0);
				
		    DuSlic_writeSOP(1,0x29,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x2A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x2B,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x2C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(1,0x2D,0x00);
			while (dataLength>0);
				
   			break;
		}
		case 2 :
		{
			//----For Channel AF3
		    DuSlic_writeSOP(2,0x05,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(2,0x06,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(2,0x11,0xFF);//war B0
		    while (dataLength>0);
		    DuSlic_writeSOP(2,0x12,0x0F);
		    while (dataLength>0);
			DuSlic_writeSOP(2,0x13,0x31); //DISABLE output IO1, Line card switch OFF
			while (dataLength>0);
			DuSlic_writeSOP(2,0x14,0x94);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x15,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x16,0x21);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x17,0x53);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x18,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x19,0x0C);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x1A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x1C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x1D,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x1E,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x1F,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x20,0x00);
			while (dataLength>0);
	
			/*
			21H  PCM Recieve Register 1
			   	R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 4&5
			*/	
			DuSlic_writeSOP(2,0x21,0x04);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x22,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x23,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x24,0x00);
			while (dataLength>0);

			/*
			25H  PCM Transive Register 1
			   	R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 4&5
			*/
			DuSlic_writeSOP(2,0x25,0x04);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x26,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x27,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x28,0x00);
			while (dataLength>0);
			
			DuSlic_writeSOP(2,0x29,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x2A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x2B,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x2C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(2,0x2D,0x00);
			while (dataLength>0);
				
   			break;
		}
		case 3 :
		{		    
	    	//----For Channel AF4
		    DuSlic_writeSOP(3,0x05,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(3,0x06,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(3,0x11,0xFF);//war B0
		    while (dataLength>0);
		    DuSlic_writeSOP(3,0x12,0x0F);
		    while (dataLength>0);
			DuSlic_writeSOP(3,0x13,0x32);//DISABLE output IO2, Line card switch OFF
			while (dataLength>0);
			DuSlic_writeSOP(3,0x14,0x94);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x15,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x16,0x21);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x17,0x53);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x18,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x19,0x0C);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x1A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x1C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x1D,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x1E,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x1F,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x20,0x00);
			while (dataLength>0);
	
			/*	
			21H  PCM Recieve Register 1
			   	R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 6&7
			*/
			DuSlic_writeSOP(3,0x21,0x06);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x22,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x23,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x24,0x00);
			while (dataLength>0);
			/*
			25H  PCM Transive Register 1
			   	R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 6&7
			*/
			DuSlic_writeSOP(3,0x25,0x06);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x26,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x27,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x28,0x00);
			while (dataLength>0);
			
			DuSlic_writeSOP(3,0x29,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x2A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x2B,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x2C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(3,0x2D,0x00);
			while (dataLength>0);
				
   			break;
		}
		case 4 :
		{
		    //----For Channel AF5
		    DuSlic_writeSOP(4,0x05,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(4,0x06,0x00);
		    while (dataLength>0);
		    DuSlic_writeSOP(4,0x11,0xFF);//war B0
		    while (dataLength>0);
		    DuSlic_writeSOP(4,0x12,0x0F);//warCF
		    while (dataLength>0);
			DuSlic_writeSOP(4,0x13,0x20);//war 00 
			while (dataLength>0);
			DuSlic_writeSOP(4,0x14,0x94);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x15,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x16,0x21);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x17,0x53);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x18,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x19,0x0C);//war 0C, 05
			while (dataLength>0);
			DuSlic_writeSOP(4,0x1A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x1C,0x00);//war22
			while (dataLength>0);
			DuSlic_writeSOP(4,0x1D,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x1E,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x1F,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x20,0x00);
			while (dataLength>0);
			
			/*	
			21H  PCM Recieve Register 1
			   	R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 8&9
			*/
			DuSlic_writeSOP(4,0x21,0x08);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x22,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x23,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x24,0x00);
			while (dataLength>0);

			/*
			25H  PCM Transive Register 1
			   	R1-HW  RS1_TS
			Bit    7      6 5 4 3 2 1 0
			value: 0      0 0 0 0 0 0 0
				PCM A    Time Slot 8&9
			*/
			DuSlic_writeSOP(4,0x25,0x08);	
			while (dataLength>0);
			DuSlic_writeSOP(4,0x26,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x27,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x28,0x00);
			while (dataLength>0);
			
			DuSlic_writeSOP(4,0x29,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x2A,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x2B,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x2C,0x00);
			while (dataLength>0);
			DuSlic_writeSOP(4,0x2D,0x00);
			while (dataLength>0);
				
   			break;
		}
    }		
}

/*****************************************************************************
  Function:      DuSlic_verifyALLRegistersSOP
  Description: 	 The function verifies ALL SOP register on selected Channel
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       None
  Return:      	 bit_error: 0-ok; 1- verify error
  Globals:       buf[5] array is used as data transmition array. 
  Important:     The function verifies ALL SOP register on selected Channel
  				 Uses "while (dataLength>0);" Use it only by Init!               
*******************************************************************************/

bool DuSlic_verifyALLRegistersSOP(unsigned char selectChannel){    
    
bool bit_error=0;

   switch (selectChannel){
		case 0 :{
		    		    
		    DuSlic_readSOP(0,0x05);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				    
		    DuSlic_readSOP(0,0x06);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
		    DuSlic_readSOP(0,0x11);
		    while (dataLength>0);
			if (buf[3]!=0xFF) bit_error=1;
	
		    DuSlic_readSOP(0,0x12);
		    while (dataLength>0); 
			if (buf[3]!=0x0F) bit_error=1;
	
			DuSlic_readSOP(0,0x13);
		    while (dataLength>0);
			if (buf[3]!=0x31) bit_error=1;
	
			DuSlic_readSOP(0,0x14);
		    while (dataLength>0);
			if (buf[3]!=0x94) bit_error=1;
				
			DuSlic_readSOP(0,0x15);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x16);
		    while (dataLength>0);
			if (buf[3]!=0x21) bit_error=1;
				
			DuSlic_readSOP(0,0x17);
		    while (dataLength>0);
			if (buf[3]!=0x53) bit_error=1;
				
			DuSlic_readSOP(0,0x18);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(0,0x19);
		    while (dataLength>0);
			if (buf[3]!=0x0C) bit_error=1;
				
			DuSlic_readSOP(0,0x1A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x1C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		
			DuSlic_readSOP(0,0x1D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x1E);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x1F);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x20);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x21);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x22);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(0,0x23);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(0,0x24);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x25);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x26);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x27);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(0,0x28);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(0,0x29);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(0,0x2A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;

			DuSlic_readSOP(0,0x2B);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
								
			DuSlic_readSOP(0,0x2C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
			DuSlic_readSOP(0,0x2D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
		    break;
		}
		case 1 :{
		    		    
		    DuSlic_readSOP(1,0x05);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				    
		    DuSlic_readSOP(1,0x06);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
		    DuSlic_readSOP(1,0x11);
		    while (dataLength>0);
			if (buf[3]!=0xFF) bit_error=1;
			    
		    DuSlic_readSOP(1,0x12);
		    while (dataLength>0); 
			if (buf[3]!=0x0F) bit_error=1;
				
			DuSlic_readSOP(1,0x13);
		    while (dataLength>0);
			if (buf[3]!=0x32) bit_error=1;
				
			DuSlic_readSOP(1,0x14);
		    while (dataLength>0);
			if (buf[3]!=0x94) bit_error=1;
				
			DuSlic_readSOP(1,0x15);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x16);
		    while (dataLength>0);
			if (buf[3]!=0x21) bit_error=1;
				
			DuSlic_readSOP(1,0x17);
		    while (dataLength>0);
			if (buf[3]!=0x53) bit_error=1;
				
			DuSlic_readSOP(1,0x18);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(1,0x19);
		    while (dataLength>0);
			if (buf[3]!=0x0C) bit_error=1;
			
				
			DuSlic_readSOP(1,0x1A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x1C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x1D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x1E);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x1F);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x20);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		
			DuSlic_readSOP(1,0x21);
		    while (dataLength>0);
			if (buf[3]!=0x02) bit_error=1;
				
			DuSlic_readSOP(1,0x22);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x23);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x24);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x25);
		    while (dataLength>0);
			if (buf[3]!=0x02) bit_error=1;
				
			DuSlic_readSOP(1,0x26);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x27);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(1,0x28);
		    while (dataLength>0); 
			if (buf[3]!=0x00) bit_error=1;	
						
			DuSlic_readSOP(1,0x29);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(1,0x2A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;

			DuSlic_readSOP(1,0x2B);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
								
			DuSlic_readSOP(1,0x2C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
			DuSlic_readSOP(1,0x2D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		    break;
		}
		case 2 :{
		    		    
		    DuSlic_readSOP(2,0x05);
		    while (dataLength>0); 
			if (buf[3]!=0x00) bit_error=1;
			    
		    DuSlic_readSOP(2,0x06);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
		    DuSlic_readSOP(2,0x11);
		    while (dataLength>0);
			if (buf[3]!=0xFF) bit_error=1;
			    
		    DuSlic_readSOP(2,0x12);
		    while (dataLength>0);
			if (buf[3]!=0x0F) bit_error=1;
				
			DuSlic_readSOP(2,0x13);
		    while (dataLength>0);
			if (buf[3]!=0x31) bit_error=1;
				
			DuSlic_readSOP(2,0x14);
		    while (dataLength>0);
			if (buf[3]!=0x94) bit_error=1;
				
			DuSlic_readSOP(2,0x15);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x16);
		    while (dataLength>0);
			if (buf[3]!=0x21) bit_error=1;
				
			DuSlic_readSOP(2,0x17);
		    while (dataLength>0);
			if (buf[3]!=0x53) bit_error=1;
				
			DuSlic_readSOP(2,0x18);
		    while (dataLength>0); 
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(2,0x19);
		    while (dataLength>0); 
			if (buf[3]!=0x0C) bit_error=1;
			
				
			DuSlic_readSOP(2,0x1A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x1C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x1D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x1E);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x1F);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x20);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x21);
		    while (dataLength>0);
			if (buf[3]!=0x04) bit_error=1;
				
			DuSlic_readSOP(2,0x22);
		    while (dataLength>0); 
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x23);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x24);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x25);
		    while (dataLength>0);
			if (buf[3]!=0x04) bit_error=1;
				
			DuSlic_readSOP(2,0x26);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x27);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(2,0x28);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		    			
			DuSlic_readSOP(2,0x29);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(2,0x2A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;

			DuSlic_readSOP(2,0x2B);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
								
			DuSlic_readSOP(2,0x2C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
			DuSlic_readSOP(2,0x2D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		    break;
		}
		case 3 :{
			    
		    DuSlic_readSOP(3,0x05);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
		    DuSlic_readSOP(3,0x06);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
		    DuSlic_readSOP(3,0x11);
		    while (dataLength>0);
			if (buf[3]!=0xFF) bit_error=1;
			    
		    DuSlic_readSOP(3,0x12);
		    while (dataLength>0);
			if (buf[3]!=0x0F) bit_error=1;
				
			DuSlic_readSOP(3,0x13);
		    while (dataLength>0);
			if (buf[3]!=0x32) bit_error=1;
				
			DuSlic_readSOP(3,0x14);
		    while (dataLength>0);
			if (buf[3]!=0x94) bit_error=1;
				
			DuSlic_readSOP(3,0x15);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x16);
		    while (dataLength>0);
			if (buf[3]!=0x21) bit_error=1;
				
			DuSlic_readSOP(3,0x17);
		    while (dataLength>0);
			if (buf[3]!=0x53) bit_error=1;
				
			DuSlic_readSOP(3,0x18);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(3,0x19);
		    while (dataLength>0);
			if (buf[3]!=0x0C) bit_error=1;			
				
			DuSlic_readSOP(3,0x1A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(3,0x1C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x1D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x1E);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x1F);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x20);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(3,0x21);
		    while (dataLength>0);
			if (buf[3]!=0x06) bit_error=1;
				
			DuSlic_readSOP(3,0x22);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x23);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x24);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x25);
		    while (dataLength>0);
			if (buf[3]!=0x06) bit_error=1;
				
			DuSlic_readSOP(3,0x26);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x27);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(3,0x28);
		    while (dataLength>0); 
			if (buf[3]!=0x00) bit_error=1;
		    
			
			DuSlic_readSOP(3,0x29);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(3,0x2A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;

			DuSlic_readSOP(3,0x2B);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
								
			DuSlic_readSOP(3,0x2C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
			DuSlic_readSOP(3,0x2D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		    break;
		}
		case 4 :{
		    
		    DuSlic_readSOP(4,0x05);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
		    DuSlic_readSOP(4,0x06);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
		    DuSlic_readSOP(4,0x11);
		    while (dataLength>0);
			if (buf[3]!=0xFF) bit_error=1;
				
		    DuSlic_readSOP(4,0x12);
		    while (dataLength>0);
			if (buf[3]!=0x0F) bit_error=1;
				
			DuSlic_readSOP(4,0x13);
		    while (dataLength>0);
			if (buf[3]!=0x20) bit_error=1;
				
			DuSlic_readSOP(4,0x14);
		    while (dataLength>0);
			if (buf[3]!=0x94) bit_error=1;
				
			DuSlic_readSOP(4,0x15);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
				
			DuSlic_readSOP(4,0x16);
		    while (dataLength>0); 
			if (buf[3]!=0x21) bit_error=1;
				
			DuSlic_readSOP(4,0x17);
		    while (dataLength>0);
			if (buf[3]!=0x53) bit_error=1;
				
			DuSlic_readSOP(4,0x18);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x19);
		    while (dataLength>0);
			if (buf[3]!=0x0C) bit_error=1;
				
			DuSlic_readSOP(4,0x1A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x1C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x1D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x1E);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x1F);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x20);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x21);
		    while (dataLength>0);
			if (buf[3]!=0x08) bit_error=1;
	
			DuSlic_readSOP(4,0x22);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x23);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x24);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x25);
		    while (dataLength>0);
			if (buf[3]!=0x08) bit_error=1;
		
			DuSlic_readSOP(4,0x26);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x27);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
	
			DuSlic_readSOP(4,0x28);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;	
			
			DuSlic_readSOP(4,0x29);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			
			DuSlic_readSOP(4,0x2A);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;

			DuSlic_readSOP(4,0x2B);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
								
			DuSlic_readSOP(4,0x2C);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
			    
			DuSlic_readSOP(4,0x2D);
		    while (dataLength>0);
			if (buf[3]!=0x00) bit_error=1;
		    break;
		}
    }
	
	return bit_error;
	
}

/*****************************************************************************
  Function:      DuSlic_writeSOP
  Description: 	 The function writes to selected SOP reagister on selected Channel
  Inputs:      	 selectChannel -  number of Channel, registerAdress - wished SOP register to be writter; writeByte - byte to be writter
  Outputs:       None
  Return:      	 None
  Globals:       buf[] array is used as data transmition array. 
  Important:     The function writes to selected SOP reagister on selected Channel
  				 Use it in time sencetive aplications. Polle like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/
				    
void DuSlic_writeSOP(unsigned short selectChannel, unsigned short registerAdress, unsigned short writeByte){
    
	SPI_config();
    
    buf[1]=registerAdress;
    buf[2]=writeByte;	
    			    
    switch (selectChannel)
    {
		case 0 :
		{
		  	// slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x44;			    
			SPI_bytesSend(buf, 3);
   			break;
		}
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x4C;
			SPI_bytesSend(buf, 3);
			break;
		}
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x44;
			SPI_bytesSend(buf, 3);
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x4C;
			SPI_bytesSend(buf, 3);
			break;
		}
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			buf[0]=0x44;
			SPI_bytesSend(buf, 3);
			break;
		}
	} 
}

/*****************************************************************************
  Function:      DuSlic_readSOP
  Description: 	 The function reada from selected SOP reagister on selected Channel
  Inputs:      	 selectChannel -  number of Channel, registerAdress - wished SOP register
  Outputs:       None
  Return:      	 None
  Globals:       buf[] array is used as data transmition array. 
  Important:     The function reada from selected SOP reagister of selected Channel
  				 Use it in time sencetive aplications. Polle like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/


void DuSlic_readSOP(unsigned short selectChannel,unsigned short registerAdress){

SPI_config();

buf[1]=registerAdress;
buf[2]=0x11;//dummy byte
buf[3]=0x11;//dummy byte
buf[4]=0x11;//dummy byte

    switch (selectChannel){
		case 0 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0xC4;
			SPI_bytesRecieve(buf, 5);
			break;
		}
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0xCC;
			SPI_bytesRecieve(buf, 5);
			break;
		}
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0xC4;
			SPI_bytesRecieve(buf, 5);
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0xCC;
			SPI_bytesRecieve(buf, 5);
			break;
		}		
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			buf[0]=0xC4;
    		SPI_bytesRecieve(buf, 5);
			break;
		}
	}	
}

/*****************************************************************************
  Function:      DuSlic_writeCOP
  Description: 	 The function writes all COP coeff to selected 
  Inputs:      	 selectChannel -  number of Channel, writeMask - Mask with selected Ring freq, inpedance.
  Outputs:       None
  Return:      	 None
  Globals:       write_COPcoef[] array is used as data transmition array. 
  Important:     The function writes all COP coeff to selected 
  				 Use it in time sencetive aplications. Polle like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/
void DuSlic_writeCOP(unsigned short selectChannel, unsigned char mode, unsigned char impedanceInd, unsigned char ringerFreq)
{
	unsigned short i=2;  
	const unsigned char *pStart;

	// Initialize SPI port
	SPI_config();

	// Setup first command byte
	switch (selectChannel)
	{
		case CH1: write_COPcoef[0] = 0x45; break;
		case CH2: write_COPcoef[0] = 0x4D; break;
		case CH3: write_COPcoef[0] = 0x45; break;
		case CH4: write_COPcoef[0] = 0x4D; break;
		case SPH: write_COPcoef[0] = 0x45; break;
	}

	// Setup second command byte
	write_COPcoef[1] = 0x00; 
	
	// Setup data
	switch (mode)
	{
		case AF_INTERFACE_HOTLINE:
		case AF_INTERFACE_REMOTE_SUB:
			switch (impedanceInd)
			{
				case 1:	pStart = COP_Impedance1_FXS; break;
				case 2:	pStart = COP_Impedance2_FXS; break;
				case 3:	pStart = COP_Impedance3_FXS; break;
				case 4:	pStart = COP_Impedance4_FXS; break;
				case 5:	pStart = COP_Impedance5_FXS; break;
				case 6:	pStart = COP_Impedance6_FXS; break;
				default: assert(false);
			} // switch (impedanceInd)
			break;

		case AF_INTERFACE_2_WIRE_PAX:
			switch (impedanceInd)
			{
				case 1:	pStart = COP_Impedance1_FXO; break;
				case 2:	pStart = COP_Impedance2_FXO; break;
				case 3:	pStart = COP_Impedance3_FXO; break;
				case 4: pStart = COP_Impedance4_FXO; break;
				case 5:	pStart = COP_Impedance5_FXO; break;
				case 6:	pStart = COP_Impedance6_FXO; break;
				default: assert(false);
			} // switch (impedanceInd)
			break;
		case AF_INTERFACE_2_WIRE_PAX_VOLTAGE:
			switch (impedanceInd)
			{
				case 1:	pStart = COP_VoltageMeasurements_FXS; break;
				default: assert(false);
			} // switch (impedanceInd)
			break;

		default:
			assert(false);
			break;
	}
	memcpy(&write_COPcoef[2], pStart, DUSLIC_SIZE_COP_TABLE);
	switch (ringerFreq)
	{
		case RINGER_FREQ_16_HZ: pStart = ringerFreqCoef16Hz; break;
		case RINGER_FREQ_20_HZ: pStart = ringerFreqCoef20Hz; break;
		case RINGER_FREQ_25_HZ: pStart = ringerFreqCoef25Hz; break;
		case RINGER_FREQ_50_HZ: pStart = ringerFreqCoef50Hz; break;
		case RINGER_FREQ_60_HZ: pStart = ringerFreqCoef60Hz; break;
		default: assert(false);
	}
	memcpy(&write_COPcoef[DUSLIC_COP_BYTE_OFFSET_RINGER_FREQ_COEF+2], pStart, DUSLIC_BYTE_SIZE_COP_TABLE_ROW);

	///////////////////////////////////////
	// Copy coefficents into send buffer //
	///////////////////////////////////////
	
	// We need a second buffer because SPI_bytesSend() overwrites the
	// the buffer it is sending from and we need the send buffer for
	// comparison.
	memcpy(send_COPcoef, write_COPcoef, sizeof(send_COPcoef));
	
	//////////////////////////////
	// Set chip select and send //
	//////////////////////////////
	switch (selectChannel)
	{
		case CH1:
			// slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
		    SPI_bytesSend(send_COPcoef,(DUSLIC_WRITE_COP_BUFLN-1));
		    break;
		
		case CH2:
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
		    SPI_bytesSend(send_COPcoef,(DUSLIC_WRITE_COP_BUFLN-1));
			break;
		
		case CH3:
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
		    SPI_bytesSend(send_COPcoef,(DUSLIC_WRITE_COP_BUFLN-1));
		    break;
		
		case CH4:
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
		    SPI_bytesSend(send_COPcoef,(DUSLIC_WRITE_COP_BUFLN-1));
		    break;
		
		case SPH:
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);
			*pSPI_FLG = (*pSPI_FLG & 0xF708);
		    SPI_bytesSend(send_COPcoef,(DUSLIC_WRITE_COP_BUFLN-1));
		    break;
	} // switch (selectChannel) 		
}

/*****************************************************************************
  Function:      DuSlic_readCOP
  Description: 	 The function reads All COP reagister on selected Channel
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       read_COPcoe¨[]- output array with COP coeffs.
  Return:      	 None
  Globals:       read_COPcoef[] array is used as data transmition/reception array. 
  Important:     The function reads All COP reagister on selected Channel
  				 Use it in time sencetive aplications. Polle like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

void DuSlic_readCOP(unsigned short selectChannel){
    
unsigned int i=2;

	SPI_config();
	read_COPcoef[1]=0x00;
	for (i=2;i<=(DUSLIC_READ_COP_BUFLN-2);i++){
	    read_COPcoef[i]=0x11;//fill array with dummy bytes
	}
		
    switch (selectChannel){
		case 0 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			read_COPcoef[0]=0xC5;
			SPI_bytesRecieve(read_COPcoef,(DUSLIC_READ_COP_BUFLN-1));
			break;
		}
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			read_COPcoef[0]=0xCD;			
			SPI_bytesRecieve(read_COPcoef, (DUSLIC_READ_COP_BUFLN-1));
			break;
		}
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			read_COPcoef[0]=0xC5;
			SPI_bytesRecieve(read_COPcoef, (DUSLIC_READ_COP_BUFLN-1));
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			read_COPcoef[0]=0xCD;
			SPI_bytesRecieve(read_COPcoef,(DUSLIC_READ_COP_BUFLN-1));
			break;
		}		
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			read_COPcoef[0]=0xC5;
    		SPI_bytesRecieve(read_COPcoef,(DUSLIC_READ_COP_BUFLN-1));
			break;
		}
	}	
}

/*****************************************************************************
  Function:      bool DuSlic_verifyCOP(void)
  Description: 	 The function verifies COP register
  Inputs:      	 None
  Outputs:       None
  Return:      	 1 or 0: 0-ok, 1- verify error
  Globals:       read_COPcoef[],write_COPcoef[]
  Important:     Use it in time sencetive aplications. 
*******************************************************************************/
bool DuSlic_verifyCOP(void)
{
	bool error = false;
		
	if (memcmp(&read_COPcoef[3], &write_COPcoef[2], DUSLIC_SIZE_COP_TABLE) != 0)
		error = true;

	return error;
}

/*****************************************************************************
  Function:      DuSlic_writePOP
  Description: 	 The function writes Pop Register of DuSlic
  Inputs:      	 selectChannel -  number of Channel, registerAdress - wished SOP register to be writter; writeByte - byte to be writter
  Outputs:       None
  Return:      	 None
  Globals:       None 
  Important:     The function writes Write Pop Register of DuSlic
  				 Use it in time sencetive aplications. Polle like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/
/*
POP OPERATION
Bit    7  6  5 4 3    2 1 0
Byte 1 RD 1  ADR[2:0] 1 1 0
Byte 2 OFFSET[7:0]

*/
void DuSlic_writePOP(unsigned short selectChannel, unsigned short *writeMask){
	
unsigned int i=0;

   SPI_config();
   write_POPcoef[1] = 0x30; 
   for (i=0;i<39;i++){
       write_POPcoef[i+2]=POP_coef[i];
   }
   		    
       switch (selectChannel){
			case 0 :{
			    // slave select enable
				*pSPI_FLG = (*pSPI_FLG | 0xFF02);
				*pSPI_FLG = (*pSPI_FLG & 0xFD02);
				write_POPcoef[0] = 0x46;
			    SPI_bytesSend(write_POPcoef,(DUSLIC_WRITE_POP_BUFLN-1));
			    break;
			}
			case 1 :{
			    // slave select enable
				*pSPI_FLG = (*pSPI_FLG | 0xFF02);
				*pSPI_FLG = (*pSPI_FLG & 0xFD02);
				write_POPcoef[0] = 0x4E;
			    SPI_bytesSend(write_POPcoef,(DUSLIC_WRITE_POP_BUFLN-1));
				break;
			}
			case 2 :{
			    // slave select enable
				*pSPI_FLG = (*pSPI_FLG | 0xFF04);
				*pSPI_FLG = (*pSPI_FLG & 0xFB04);
				write_POPcoef[0] = 0x46;
			    SPI_bytesSend(write_POPcoef,(DUSLIC_WRITE_POP_BUFLN-1));
				break;
			}
			case 3 :{
			    // slave select enable
				*pSPI_FLG = (*pSPI_FLG | 0xFF04);
				*pSPI_FLG = (*pSPI_FLG & 0xFB04);
				write_POPcoef[0] = 0x4E;
			    SPI_bytesSend(write_POPcoef,(DUSLIC_WRITE_POP_BUFLN-1));
				break;
			}
			case 4 :{
			    // slave select enable
				*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
				*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
				write_POPcoef[0] = 0x46;
			    SPI_bytesSend(write_POPcoef,(DUSLIC_WRITE_POP_BUFLN-1));
				break;
			}
		}  	
}


/*****************************************************************************
  Function:      DuSlic_readPOP
  Description: 	 The function reads All POP reagister on selected Channel
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       read_POPcoef[]- output array with POP coeffs.
  Return:      	 None
  Globals:       read_POPcoef[] array is used as data transmition/reception array. 
  Important:     The function reads All POP reagister on selected Channel
  				 Use it in time sencetive aplications. Polle like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

void DuSlic_readPOP(unsigned short selectChannel){
    
unsigned int i=2;

	SPI_config();
	read_POPcoef[1]=0x30;
	for (i=2;i<=(DUSLIC_READ_POP_BUFLN-2);i++){
	    read_POPcoef[i]=0x11;//fill array with dummy bytes
	}
		
    switch (selectChannel){
		case 0 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			read_POPcoef[0]=0xC6;
			SPI_bytesRecieve(read_POPcoef,(DUSLIC_READ_POP_BUFLN-1));
			break;
		}
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			read_POPcoef[0]=0xCE;			
			SPI_bytesRecieve(read_POPcoef,(DUSLIC_READ_POP_BUFLN-1));
			break;
		}
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			read_POPcoef[0]=0xC6;
			SPI_bytesRecieve(read_POPcoef,(DUSLIC_READ_POP_BUFLN-1));
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			read_POPcoef[0]=0xCE;
			SPI_bytesRecieve(read_POPcoef,(DUSLIC_READ_POP_BUFLN-1));
			break;
		}		
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			read_POPcoef[0]=0xC6;
    		SPI_bytesRecieve(read_POPcoef,(DUSLIC_READ_POP_BUFLN-1));
			break;
		}
	}	
}


/*****************************************************************************
  Function:      DuSlic_verifyPOP
  Description: 	 The function verifies POP register on selected channel
  Inputs:      	 selectChannel -  number of Channel, 
  Outputs:       None
  Return:      	 1 or 0: 0-ok, 1- verify error
  Globals:       read_POPcoef[],POP_coef[] - buffers with data
  Important:     The function verifies POP register on selected channel
  				 Use it in time sensetive aplications. 
*******************************************************************************/

bool DuSlic_verifyPOP(unsigned short selectChannel){

bool error=0;
unsigned int i=0;
		
    switch (selectChannel){
		case 0 :{
			for (i=0;i<38;i++){
	    		if (read_POPcoef[i+3]!=POP_coef[i]) error=1;
			}			    
			break;
		}
		case 1 :{
		    for (i=0;i<38;i++){
	    		if (read_POPcoef[i+3]!=POP_coef[i]) error=1;
			}			    			
			break;
		}
		case 2 :{
		    for (i=0;i<38;i++){
	    		if (read_POPcoef[i+3]!=POP_coef[i]) error=1;
			}			    			
			break;
		}
		case 3 :{
		    for (i=0;i<38;i++){
	    		if (read_POPcoef[i+3]!=POP_coef[i]) error=1;
			}			    
			break;
		}		
		case 4 :{
		    for (i=0;i<38;i++){
	    		if (read_POPcoef[i+3]!=POP_coef[i]) error=1;
			}			    			
			break;
		}
	} 
	return error;
}
/*****************************************************************************
  Function:      void DuSlic_startRingTripMeasurement(unsigned short selectChannel)
  Description: 	 DuSlic_startRingTripMeasurement() measures DC current on IT pin.
  Inputs:      	 unsigned short selectChannel: channel (0-3, 4 service phone)
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     Call this function not before DuSlic_VoltageMeasurement()!
  				 The function Measures DC current on IT pin.
  				 Measurents are in continius mode.
  				 The Measurement is used for automatic Ring Trip detection which
  				 autonumously switches off the ring current.
*******************************************************************************/
void DuSlic_startRingTripMeasurement(unsigned short selectChannel)
{
	/*
	1DH  Level Mettering Configuration Register 2

	Bit    7 6 5 4    3    2 	1      0
	value: 0 0 0 0    1    0 	1      0
	*/
	DuSlic_writeSOP(selectChannel, 0x1D, 0x05); //Measure DC current on IT
	while (dataLength>0);

	/*
	1EH  Level Mettering Configuration Register 3
	Bit    7 6 5 4    3    2 	1      0
	value: 0 0 0 0    0    0 	0      0
	*/
	DuSlic_writeSOP(selectChannel, 0x1E, 0x00);
	while (dataLength>0);

	/*
	1CH  Level Mettering Configuration Register 1
	Bit    7 6 5 4    3    2 	1      0
	value: 1 0 1 0    0    1 	1      0
	*/		
	DuSlic_writeSOP(selectChannel, 0x1C, 0x40); //Start Measure 
	while (dataLength>0);
}

/*****************************************************************************
  Function:      DuSlic_VoltageMeasurement
  Description: 	 The function Measures Voltages on TIP&RING lines.
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       None
  Return:      	 Bool true: It is a voltage on one of line; Bool False: No voltage
  Globals:       
  Important:     The function measures voltages on TIP&RING 
  				 Do not use it in time sencetive aplications.                 
*******************************************************************************/
bool DuSlic_VoltageMeasurement(unsigned short selectChannel){

unsigned int readResult1[5], readResult2[5], a=0;
unsigned char readLowByte=0,readHighByte=0;
unsigned int i;
bool voltage1Present = true;
bool voltage2Present = true;

	// 420ms delay
	timer_init(420000, e_Timer1);
	timer_wait(e_Timer1);
	
	/*
	1CH  Level Mettering Configuration Register 1

	Bit    7 6 5 4    3    2 	1      0
	value: 1 0 1 0    0    1 	1      0
	*/	
	DuSlic_writeSOP(selectChannel,0x1C,0x22);	
    while (dataLength>0);
    
	/*
	1DH  Level Mettering Configuration Register 2

	Bit    7 6 5 4    3    2 	1      0
	value: 0 0 0 0    1    0 	1      0
	*/
	DuSlic_writeSOP(selectChannel,0x1D,0x0A);//Measure on tip
	while (dataLength>0);
	/*
	1EH  Level Mettering Configuration Register 3

	Bit    7 6 5 4    3    2 	1      0
	value: 0 0 0 0    0    0 	0      0
	*/
	DuSlic_writeSOP(selectChannel,0x1E,0x00);
	while (dataLength>0);
	/*
	1CH  Level Mettering Configuration Register 1

	Bit    7 6 5 4    3    2 	1      0
	value: 1 0 1 0    0    1 	1      0
	*/	
	
	//DuSlic_writeSOP(selectChannel,0x1C,0x62);//Start Measure with intergator
	DuSlic_writeSOP(selectChannel,0x1C,0x22);//Start Measure without intergator
	while (dataLength>0);

	for (a=0;a<5;a++){

	    // 84ms delay
		timer_init(84000, e_Timer1);
		timer_wait(e_Timer1);
					    
	    DuSlic_readSOP(selectChannel,0x0D);
    	while (dataLength>0);
    	readHighByte=buf[3];
	
		DuSlic_readSOP(selectChannel,0x0E);
    	while (dataLength>0);
		readLowByte=buf[3];
	
	    readResult1[a]=(unsigned int)((readHighByte)<<8)+readLowByte;
	    
	}							
	
	DuSlic_writeSOP(selectChannel,0x1C,0x22);//Stop Measure	
	while (dataLength>0);					
    
	DuSlic_writeSOP(selectChannel,0x1D,0x0B);//Measure on Ring
	while (dataLength>0);

	//DuSlic_writeSOP(selectChannel,0x1C,0x62);//Start Measure with intergator
	DuSlic_writeSOP(selectChannel,0x1C,0x22);//Start Measure without intergator	
	while (dataLength>0);

	if (buf[3]!=0x00);	

	for (a=0;a<5;a++){
	    
		// 84ms delay
		timer_init(84000, e_Timer1);
		timer_wait(e_Timer1);
		
		
	    DuSlic_readSOP(selectChannel,0x0D);
    	while (dataLength>0);
    	readHighByte=buf[3];
	
		DuSlic_readSOP(selectChannel,0x0E);
    	while (dataLength>0);
		readLowByte=buf[3];
	
	    readResult2[a]=(unsigned int)((readHighByte)<<8)+readLowByte;
	    
	}				
			 			
	DuSlic_writeSOP(selectChannel,0x1C,0x00);//Stop Measure	
	while (dataLength>0);
	
	DuSlic_writeSOP(selectChannel,0x1D,0x00);
	while (dataLength>0);
	
	DuSlic_writeSOP(selectChannel,0x1E,0x00);
	while (dataLength>0);
	
	// Evaluate the measured values
	for ( i=0; i<=4; i++ )
	{
	    if ( readResult1[i] <= VOLTAGE_LEVEL )
	    	voltage1Present = false;
	}
	for ( i=0; i<=4; i++ )
	{
	    if ( readResult2[i] <= VOLTAGE_LEVEL )
	    	voltage2Present = false;
	}
	
	return (voltage1Present || voltage2Present);
			
}

/*****************************************************************************
  Function:      DuSlic_getRandom_long
  Description: 	 The function Measures Voltage on TIP line Channel 1 & gets 24bit random value.
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       None
  Return:      	 unsigned int: 24 bit random value
  Globals:       
  Important:     The function Measures Voltage on TIP line Channel 1 & gets 24bit random value.
  				 Do not use it in time sencetive aplications.                 
*******************************************************************************/

unsigned int DuSlic_getRandom_long(unsigned short selectChannel){

unsigned int readResult=0,a=0;
unsigned char readLowByte=0,k=0;

	// 420ms delay
	timer_init(420000, e_Timer1);
	timer_wait(e_Timer1);
	
	/*
	1CH  Level Mettering Configuration Register 1

	Bit    7 6 5 4    3    2 	1      0
	value: 1 0 1 0    0    1 	1      0
	*/	
	DuSlic_writeSOP(selectChannel,0x1C,0x22);	
    while (dataLength>0);
    
	/*
	1DH  Level Mettering Configuration Register 2

	Bit    7 6 5 4    3    2 	1      0
	value: 0 0 0 0    1    0 	1      0
	*/
	DuSlic_writeSOP(selectChannel,0x1D,0x0B);//Measure on RING
	while (dataLength>0);
	/*
	1EH  Level Mettering Configuration Register 3

	Bit    7 6 5 4    3    2 	1      0
	value: 0 0 0 0    0    0 	0      0
	*/
	DuSlic_writeSOP(selectChannel,0x1E,0x00);
	while (dataLength>0);
	/*
	1CH  Level Mettering Configuration Register 1

	Bit    7 6 5 4    3    2 	1      0
	value: 1 0 1 0    0    1 	1      0
	*/	
	
	DuSlic_writeSOP(selectChannel,0x1C,0x62);//Start Measure 
	while (dataLength>0);
	
	k=20;
	for (a=0;a<6;a++){

	    // 168ms delay
		timer_init(168000, e_Timer1);
		timer_wait(e_Timer1);
					    	
		DuSlic_readSOP(selectChannel,0x0E);
    	while (dataLength>0);
    	
		readLowByte=buf[3];
		readLowByte=readLowByte&0x0F;
	    readResult=(unsigned int)(readResult+(readLowByte<<k));
	    k=k-4;
	    
	}	
	
	DuSlic_writeSOP(selectChannel,0x1C,0x00);//Stop Measure	
	while (dataLength>0);
	
	DuSlic_writeSOP(selectChannel,0x1D,0x00);
	while (dataLength>0);
	
	DuSlic_writeSOP(selectChannel,0x1E,0x00);
	while (dataLength>0);
	
		
	return	readResult;			
}	
/*****************************************************************************
  Function:      DuSlic_readInt
  Description: 	 The function reads 4 Interrupt registers on selected channel
  Inputs:      	 selectChannel -  number of Channel, 
  Outputs:       None
  Return:      	 None
  Globals:       buf[] - buffers with Intdata
  Important:     The function reads 4 Interrupt registers on selected channel
  				 Use it in time sencetive aplications. 
  				 Check buf[3], buf[4],buf[5],buf[6] for results
  				 
				Register Description:
				07H INTREG1 Interrupt Register 1 (read only)
					INT-CH    HOOK    GNDK    GNKP     ICON VTRLIM OTEMP  Sync-Fail
				Bit    7        6       5       4        3    2 	1   	0
				value: 0		0		0		0		 0    0 	0       0

				08H INTREG2 Interrupt Register 2 (read only)
					LM-Thres  Ready   RSTAT   LM-OK     I/O pins
				Bit    7        6       5       4        3 2 1 0
				value: 0		0		0		0		 0 0 0 0

*******************************************************************************/

void DuSlic_readInt(unsigned short selectChannel){

unsigned int i=2;

	SPI_config();	
	read_Int[1]=0x07;
	for (i=2;i<=7;i++){
	   read_Int[i]=0x11;//fill with dummy bytes
	}		
    switch (selectChannel){
		case 0 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			read_Int[0]=0xC4;
			SPI_bytesRecieve(read_Int,8);
			break;
		}
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			read_Int[0]=0xCC;			
			SPI_bytesRecieve(read_Int,8);
			break;
		}
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			read_Int[0]=0xC4;
			SPI_bytesRecieve(read_Int,8);
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			read_Int[0]=0xCC;
			SPI_bytesRecieve(read_Int,8);
			break;
		}		
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			read_Int[0]=0xC4;
			SPI_bytesRecieve(read_Int,8);
    		break;
		}
	}	
}

/*****************************************************************************
  Function:      DuSlic_softresetDuSlic	
  Description: 	 Reset of selected DuSlic
  Inputs:      	 selectDuslic -  number of Duslic
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

/*
Soft reset of DuSclic(reset all configuration registers, CRAM data is not affected)
Bit 	7     6   5 4 3     2 1 0
Descr:  RD/WR      ch1/ch2  
Byte 1  0     1   0 0 0     0 0 0
*/
/*
void DuSlic_softreset(unsigned short selectDuslic){
    
    	switch (selectDuslic){
				case 0 :{
					*pSPI_FLG = FLS1;
					*pSPI_FLG = FLG1;
				    SPI_writebyte(0x20);
				   	*pSPI_FLG = 0x00;
					break;
				}
				case 1 :{
				    *pSPI_FLG = FLS2;
					*pSPI_FLG = FLG2;
				    SPI_writebyte(0x20);
				   	*pSPI_FLG = 0x00;
					break;
				}
				case 2 :{
				    *pSPI_FLG = FLS3;
					*pSPI_FLG = FLG3;
				    SPI_writebyte(0x20);
				   	*pSPI_FLG = 0x00;
					break;
				}
			}

}
*/


/*****************************************************************************
  Function:      DuSlic_softresetChannel
  Description: 	 The function Resets of selected AF Channel
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

/*
Soft reset for the specified channel A or B 
Bit 	7     6   5 4 3     2 1 0
Descr:  RD/WR      ch1/ch2  
Byte 1  0     1   0 0 0     0 0 1
*/
/*
void DuSlic_softresetChannel(unsigned short selectChannel){
    
    switch (selectChannel){
				case 0 :{
				    *pSPI_FLG = FLS1;
					*pSPI_FLG = FLG1;
					SPI_writebyteDMA(0x41,1);	
				    break;
				}
				case 1 :{
				    *pSPI_FLG = FLS1;
					*pSPI_FLG = FLG1;
				    SPI_writebyteDMA(0x49,1);	
				    break;
				}
				case 2 :{
				    *pSPI_FLG = FLS2;
					*pSPI_FLG = FLG2;
				    SPI_writebyteDMA(0x41,1);	
				    break;
				}
				case 3 :{   				    
				    *pSPI_FLG = FLS2;
					*pSPI_FLG = FLG2;
				    SPI_writebyteDMA(0x49,1);	
				    break;
				}
				case 4 :{
				    *pSPI_FLG = FLS3;
					*pSPI_FLG = FLG3;
					SPI_writebyteDMA(0x41,1);	
				    break;
				}
			}
}
*/
/*****************************************************************************
  Function:      DuSlic_resyncPCM
  Description: 	 The function Resync PCM interface of selected Duslic 
  Inputs:      	 selectChannel -  number of Channel
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

/*
Resychronization of the PCM interface
Bit 	7     6   5 4 3     2 1 0
Descr:  RD/WR      ch1/ch2  
Byte 1  0     1   0 0 0     0 1 0
*/
/*void DuSlic_resyncPCM(unsigned short selectDuslic){

	switch (selectDuslic){
				case 0 :{
					*pSPI_FLG = FLS1;
					*pSPI_FLG = FLG1;
				    break;
				}
				case 1 :{
					*pSPI_FLG = FLS2;
					*pSPI_FLG = FLG2;
					break;
				}
				case 2 :{
					*pSPI_FLG = FLS3;
					*pSPI_FLG = FLG3;
   				    break;
				}
			}
    SPI_writebyteDMA(0x42,1);	
}
*/

/*****************************************************************************
  Function:      DuSlic_startEDSP
  Description: 	 The function switches on the Enchanced DSP on DuSlic
  Inputs:      	 selectChannel -  number of Channel,
  Outputs:       None
  Return:      	 None
  Globals:        
  Important:     
*******************************************************************************/

void DuSlic_startEDSP(unsigned short selectChannel)	{
 
      switch (selectChannel){
		case 0 :{
		    DuSlic_writeSOP(0,0x06,0x80);
			while (dataLength>0);	
			break;
		}
		case 1 :{
		    DuSlic_writeSOP(1,0x06,0x80);
			while (dataLength>0);	
			break;
		}
		case 2 :{
		    DuSlic_writeSOP(2,0x06,0x80);
			while (dataLength>0);	
			break;
		}
		case 3 :{
		    DuSlic_writeSOP(3,0x06,0x80);
			while (dataLength>0);	
			break;
		}		
		case 4 :{
		    DuSlic_writeSOP(4,0x06,0x80);
			while (dataLength>0);	
			break;
		}
	}			
}


/*****************************************************************************
  Function:      startLEC()
  Description: 	 The function switches on the Line Echo Canceller on DuSlic
  Inputs:      	 selectChannel -  number of Channel,
  Outputs:       None
  Return:      	 None
  Globals:        
  Important:     
*******************************************************************************/

static void startLEC(unsigned short selectChannel)	{
 
      switch (selectChannel){
		case 0 :{
			DuSlic_writeSOP(0,0x19,0x0C); 
			break;
		}
		case 1 :{
		    DuSlic_writeSOP(1,0x19,0x0C);			
			break;
		}
		case 2 :{
		    DuSlic_writeSOP(2,0x19,0x0C);			
			break;
		}
		case 3 :{
			DuSlic_writeSOP(3,0x19,0x0C);			
			break;
		}		
		case 4 :{
		    DuSlic_writeSOP(4,0x19,0x0C);			
			break;
		}
	}			
}

/*****************************************************************************
  Function:      stopLEC()
  Description: 	 The function switches off the Line Echo Canceller on DuSlic
  Inputs:      	 selectChannel -  number of Channel,
  Outputs:       None
  Return:      	 None
  Globals:        
  Important:     
*******************************************************************************/

static void stopLEC(unsigned short selectChannel)	{
 
      switch (selectChannel){
		case 0 :{
			DuSlic_writeSOP(0,0x19,0x00); 			
			break;
		}
		case 1 :{
		    DuSlic_writeSOP(1,0x19,0x00);
			break;
		}
		case 2 :{
		    DuSlic_writeSOP(2,0x19,0x00);
			break;
		}
		case 3 :{
		    DuSlic_writeSOP(3,0x19,0x00);
			break;
		}		
		case 4 :{
		    DuSlic_writeSOP(4,0x19,0x00);
			break;
		}
	}			
}



/*****************************************************************************
  Function:      DuSlic_modePowerdown
  Description: 	 The function switch selected Duslic in Power Down mode.
  Inputs:      	 selectChannel -  number of Channel,
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     Power consumption is redused by omitting every second DC/DC switching cycle. -48VDC will be on Ring line.
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

void DuSlic_modePowerdown(unsigned short selectChannel){
    
unsigned char buf[1];

    SPI_config();
    
	switch (selectChannel){
		case 0 :{
    		// slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x00;
			SPI_bytesSend(buf, 1);   
			break;
		}
		case 1 :{
     		// slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x08;
			SPI_bytesSend(buf, 1);
		    break;
		}
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x00;
			SPI_bytesSend(buf, 1);
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x08;
			SPI_bytesSend(buf, 1);
			break;
		}		
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			buf[0]=0x00;
			SPI_bytesSend(buf, 1);   					
			break;
		}
	}	
}

/*****************************************************************************
  Function:      DuSlic_modeActive
  Description: 	 The function enables Active mode on selected channel. -48VDC will be on Ring line.
  Inputs:      	 selectChannel - number of DuSlic. 
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     The function enables Active mode on selected channel
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
  				 In this mode any talk operation can be performed
*******************************************************************************/

void DuSlic_modeActive(unsigned short selectChannel){
unsigned char buf[1];
   
   SPI_config();
	switch (selectChannel){
		case 0 :{		    
	       	// slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x02;
			SPI_bytesSend(buf, 1);   					
		    break;
		} 
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x0A;
			SPI_bytesSend(buf, 1);
				break;
		} 		
		case 2 :{
			// slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x02;
			SPI_bytesSend(buf, 1);
				break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x0A;
			SPI_bytesSend(buf, 1);
				break;
		}		
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			buf[0]=0x02;
			SPI_bytesSend(buf, 1);   					
			    break;
		}
	}
}

/*****************************************************************************
  Function:      DuSlic_modeActiveRing
  Description: 	 The function enables RINGING (Active mode Ringing) on selected channel
  Inputs:      	 selectChannel - number of Channel. 
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     The function enables RINGING (Active mode Ringing) on selected channel
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

void DuSlic_modeActiveRing(unsigned short selectChannel){
    
unsigned char buf[1];
 
   SPI_config();
	switch (selectChannel){
		case 0 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
			buf[0]=0x05;
    		SPI_bytesSend(buf, 1); 
			break;
		}
		case 1 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF02);
			*pSPI_FLG = (*pSPI_FLG & 0xFD02);
		    buf[0]=0x0D;
    		SPI_bytesSend(buf, 1); 
			break;
		}
		
		case 2 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x05;
    		SPI_bytesSend(buf, 1); 			
			break;
		}
		case 3 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF04);
			*pSPI_FLG = (*pSPI_FLG & 0xFB04);
			buf[0]=0x0D;
    		SPI_bytesSend(buf, 1); 			
			break;
		}
		case 4 :{
		    // slave select enable
			*pSPI_FLG = (*pSPI_FLG | 0xFF08);//chip3
			*pSPI_FLG = (*pSPI_FLG & 0xF708);//chip3
			buf[0]=0x05;
    		SPI_bytesSend(buf, 1); 
			break;
		}
	}	
}

/*****************************************************************************
  Function:      DuSlic_LinecardSwitchSelect
  Description: 	 The function enables/disables LinecardSwitch on selected channel
  Inputs:      	 selectChannel - number of Channel; selectState- 0/1
  Outputs:       None
  Return:      	 None
  Globals:       buf[5] array is used as data transmition array. 
  Important:     The function enables/disables LinecardSwitch on selected channel
  				 Use it in time sencetive aplications with polling like "if (DataLen==0)" for finish transfer.                 
*******************************************************************************/

void DuSlic_LinecardSwitchSelect(unsigned short selectChannel, unsigned short selectState){
    
    if (selectState==1){
        /*
		13H I/O Control Register 2
				IO-OEN       IO-DD
		Bit    7 6 5 4    3 2 1 0
		value: 0 0 1 1    0 0 0 0    
		output enable   write 0 to output
	    */  
	    DuSlic_writeSOP(selectChannel,0x13,0x30);
    } 
    else{
        /*
		13H I/O Control Register 2
				IO-OEN       IO-DD
		Bit    7 6 5 4    3 2 1 0
		value: 0 0 1 1    0 0 0 1    
		output enable   write 1 to output
	    */          
        switch (selectChannel){
			case 0 :{
			    DuSlic_writeSOP(selectChannel,0x13,0x31);
				break;
			}
			case 1 :{
			    DuSlic_writeSOP(selectChannel,0x13,0x32);
				break;
			}	
			case 2 :{
			    DuSlic_writeSOP(selectChannel,0x13,0x31);
				break;
			}
			case 3 :{
			    DuSlic_writeSOP(selectChannel,0x13,0x32);
			    break;
			}
		}	
    }	 

}

/*****************************************************************************
  Function:      DuSlic_OffHook
  Description: 	 The function writes 0 or 1 for Offhook or Onhook on selected channel
  Inputs:      	 selectChannel - number of Channel. selectState: 0-off,1 - on
  Outputs:       None
  Return:      	 None
  Globals:       None
  Important:     The function is used in FXO mode when Off/Onhook must be emulated on the Tel line  				 
*******************************************************************************/

void  DuSlic_OffHook(unsigned short selectChannel, unsigned short selectState){

	switch (selectChannel){
			case 0 :{
			    if (selectState==1) *pPORTDIO =(*pPORTDIO & 0xFFEF);
			    else *pPORTDIO =(*pPORTDIO | 0x0010);
			    break;
			}
			case 1 :{
			    if (selectState==1) *pPORTDIO =(*pPORTDIO & 0xFFDF);
			    else *pPORTDIO =(*pPORTDIO | 0x0020);
			    break;
			}
			
			case 2 :{
			    if (selectState==1) *pPORTDIO =(*pPORTDIO & 0xFFBF);
			    else *pPORTDIO =(*pPORTDIO | 0x0040);
			 	break;
			}
			case 3 :{		
			    if (selectState==1) *pPORTDIO =(*pPORTDIO & 0xFF7F);
			    else *pPORTDIO =(*pPORTDIO | 0x0080);
			 	break;
			}

		}	


}


/*****************************************************************************
  Function:      DuSlic_hardwareresetDuSlic	
  Description: 	 The function makes Hardware Reset of all 5 DuSlics
  Inputs:      	 void
  Outputs:       void
  Return:      	 None
  Globals:       None
  Important:     The function resets all 5 Duslics. Please make it better. It is done with simple delay   				 
*******************************************************************************/

void DuSlic_hardwarereset(void){
	
	*pPORTEIO =(*pPORTEIO | 0x0100); // Enable Duslics
	
	// 840ms delay
	timer_init(840000, e_Timer1);
	timer_wait(e_Timer1);
	
	*pPORTEIO =(*pPORTEIO & 0xFEFF); // Reset Duslics 
	
	// 840ms delay
	timer_init(840000, e_Timer1);
	timer_wait(e_Timer1);
	
	*pPORTEIO =(*pPORTEIO | 0x0100); // Enable Duslics	
	
	// 840ms delay
	timer_init(840000, e_Timer1);
	timer_wait(e_Timer1);	
}


/*****************************************************************************
  Function:      DuSlic_isOffHook
  Description: 	 The funktion detects On/Off hook on selected channel
  Inputs:      	 None
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     The funktion detects On/Off hook on selected channel   				 
*******************************************************************************/

bool DuSlic_isOffHook(unsigned short selectChannel){	 
	switch (selectChannel){
		case 0 :{
		    if ((interupt_2WireArray[0][0] & 0x40)==0x40) return true;
		    else return false;		    
		}
		case 1 :{
		    if ((interupt_2WireArray[1][0] & 0x40)==0x40) return true;
		    else return false;		    		    
		}	
		case 2 :{
		    if ((interupt_2WireArray[2][0] & 0x40)==0x40) return true;
		    else return false;		    
		}
		case 3 :{
		    if ((interupt_2WireArray[3][0] & 0x40)==0x40) return true;
		    else return false;		    		    
		}
		case 4 :{
		    if ((interupt_2WireArray[4][0] & 0x40)==0x40) return true;
		    else return false;
		}		
	}
	return false;
}

/*****************************************************************************
  Function:      bool DuSlic_isOffHookDelayed(unsigned short selectChannel)
  Description: 	 DuSlic_isOffHookDelayed() returns true if a Off-Hook was detected
  				 for at least 210ms and false otherwise. For the service
				 phone the immediate Off-Hook state is returned because the
				 service phone does not support pulse code dialing.
  Inputs:      	 unsigned short selectChannel: channel (0-3, 4 SPH)
  Outputs:       None
  Return:      	 bool: true if a Off-Hook was detected
  				 for at least 210ms and false otherwise.
  Globals:       gs_offHookDelay
  Important:     Must be called after DuSlic_Config()
*******************************************************************************/

bool DuSlic_isOffHookDelayed(unsigned short selectChannel)
{
	if ( selectChannel == SPH )
		return DuSlic_isOffHook(SPH);
	else
		return gs_offHookDelay[selectChannel].isOffHookDelayed;
}

/*****************************************************************************
  Function:      DuSlic_isOverTemperature
  Description: 	 The funktion detects Overtemperature on selected channel
  Inputs:      	 None
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     If Overtemperature detected SWITCH OFF The channel!
*******************************************************************************/

	bool DuSlic_isOverTemperature(unsigned short selectChannel){	 
	switch (selectChannel){
		case 0 :{
		    if ((interupt_2WireArray[0][0] & 0x02)==0x02) return true;
		    else return false;		    
		}
		case 1 :{
		    if ((interupt_2WireArray[1][0] & 0x02)==0x02) return true;
		    else return false;		    		    
		}	
		case 2 :{
		    if ((interupt_2WireArray[2][0] & 0x02)==0x02) return true;
		    else return false;		    
		}
		case 3 :{
		    if ((interupt_2WireArray[3][0] & 0x02)==0x02) return true;
		    else return false;		    		    
		}
		case 4 :{
		    if ((interupt_2WireArray[4][0] & 0x02)==0x02) return true;
		    else return false;
		}		
	}
	return false;
}

/*****************************************************************************
  Function:      DuSlic_isSync Fail
  Description: 	 The funktion detects Sync Fails on TDM BUS
  Inputs:      	 None
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     If Sync Fail detected: reset Duslics, restart SPORT1
*******************************************************************************/

bool DuSlic_isSyncFail(unsigned short selectChannel){	 
	switch (selectChannel){
		case 0 :{
		    if ((interupt_2WireArray[0][0] & 0x01)==0x01) return true;
		    else return false;		    
		}
		case 1 :{
		    if ((interupt_2WireArray[1][0] & 0x01)==0x01) return true;
		    else return false;		    		    
		}	
		case 2 :{
		    if ((interupt_2WireArray[2][0] & 0x01)==0x01) return true;
		    else return false;		    
		}
		case 3 :{
		    if ((interupt_2WireArray[3][0] & 0x01)==0x01) return true;
		    else return false;		    		    
		}
		case 4 :{
		    if ((interupt_2WireArray[4][0] & 0x01)==0x01) return true;
		    else return false;
		}		
	}
	return false;
}



/*****************************************************************************
  Function:      DuSlic_isEDSPFail
  Description: 	 The funktion detects EDSP Fails
  Inputs:      	 None
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     If EDSP Fails detected: reset Duslics, restart SPORT1
*******************************************************************************/

bool DuSlic_isEDSPFail(unsigned short selectChannel){	 
	switch (selectChannel){
		case 0 :{
		    if ((interupt_2WireArray[0][3] & 0x80)==0x80) return true;
		    else return false;		    
		}
		case 1 :{
		    if ((interupt_2WireArray[1][3] & 0x80)==0x80) return true;
		    else return false;		    		    
		}	
		case 2 :{
		    if ((interupt_2WireArray[2][3] & 0x80)==0x80) return true;
		    else return false;		    
		}
		case 3 :{
		    if ((interupt_2WireArray[3][3] & 0x80)==0x80) return true;
		    else return false;		    		    
		}
		case 4 :{
		    if ((interupt_2WireArray[4][3] & 0x80)==0x80) return true;
		    else return false;
		}		
	}
	return false;
}

/*****************************************************************************
  Function:      DuSlic_isRingTripThreshold
  Description: 	 The funktion detects DuSlic_isRingTripThreshold
  Inputs:      	 None
  Outputs:       None
  Return:      	 None
  Globals:       
  Important:     If EDSP Fails detected: reset Duslics, restart SPORT1
*******************************************************************************/

bool DuSlic_isRingTripThreshold(unsigned short selectChannel){	 
	switch (selectChannel){
		case 0 :{
		    if ((interupt_2WireArray[0][1] & 0x80)==0x80) return true;
		    else return false;		    
		}
		case 1 :{
		    if ((interupt_2WireArray[1][1] & 0x80)==0x80) return true;
		    else return false;		    		    
		}	
		case 2 :{
		    if ((interupt_2WireArray[2][1] & 0x80)==0x80) return true;
		    else return false;		    
		}
		case 3 :{
		    if ((interupt_2WireArray[3][1] & 0x80)==0x80) return true;
		    else return false;		    		    
		}
		case 4 :{
		    if ((interupt_2WireArray[4][1] & 0x80)==0x80) return true;
		    else return false;
		}		
	}
	return false;
}
