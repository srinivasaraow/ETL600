/*-------------------------------------------------------------------------*
 * $Workfile: CONSTANT.H $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: Hansjürg Bruderer
 * Remarks	:  
 * Purpose	: Global Constant Declaration  
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/CONSTANT.H $
 * 
 * 53    11-05-02 15:50 Chmazar
 * Constant NO_P7LC_MODULE_SLOTS moved to the class CRackSlotMapping
 * 
 * 52    11-03-23 15:42 Chhamaa
 * RF_PEP_MAX defines added for R4.
 * 
 * 51    11-03-14 15:54 Chmazar
 * Configurable rack slot mapping. The defines and constants were replaced
 * by the class CRackSlotMapping
 * 
 * 50    10-12-01 15:43 Chmazar
 * ETL600 R4 initial version
 * 
 * 47    07-10-04 19:39 Chrobif
 * Mapping of subaddresses to basic timeslots added.
 * 
 * 43    5.04.06 16:27 Chhamaa
 * Rollback
 * 
 * 41    24.03.06 17:59 Chrobif
 * Rack slot to subaddress mapping added.
 * 
 * 39    27.09.05 4:59 Chchlee
 * "UL" added to some constants
 * 
 * 38    30.06.05 8:13 Chchlee
 * default tdm timeout added
 * 
 * 37    27.05.05 16:04 Chpeplu
 * P4LT_RESET_TIME_IN_MS increased from 14s to 18s
 * 
 * 36    27.05.05 9:06 Chchlee
 * P4LT_RESET_TIME_IN_MS decreased from 18s to 14s
 * 
 * 35    13.05.05 15:51 Chchlee
 * P4LT_RESET_TIME_IN_MS increased from 15s to 18s
 * 
 * 34    22.04.05 17:40 Chsanoh
 * 
 * 33    22.04.05 14:25 Chsanoh
 * 
 * 32    19.04.05 11:54 Chsanoh
 * 
 * 31    9.02.05 15:10 Chhamaa
 * Defines for  RF power settings added
 * 
 * 30    22.12.04 16:24 Chsanoh
 * 
 * 29    29.09.04 9:29 Maag01
 * 
 * 27    24.09.04 16:16 Schnyd01
 * 
 * 26    24.09.04 12:06 Schnyd01
 * #define N_DATA_RATES changed from 6 to 5
 * 
 * 25    17.09.04 18:00 Schnyd01
 * 
 * 24    15.09.04 11:43 Rytz01
 * 
 * 23    9.09.04 18:36 Leeb02
 * TDM constants added
 * 
 * 22    8.09.04 11:31 Leeb02
 * P4LT reset time increased to 15 seconds
 * 
 * 21    8.09.04 10:02 Leeb02
 * 
 * 20    30.08.04 17:00 Maag01
 * 
 * 19    27.08.04 17:15 Leeb02
 * P4LT_RESET_TIME_IN_MS added
 * 
 * 18    11.08.04 14:31 Maag01
 * 
 * 14    18.06.04 13:38 Maag01
 * 
 * 13    4.06.04 9:37 Rytz01
 * IO_Buffers removed
 * 
 * 12    3.06.04 17:33 Rytz01
 * more HW-constants added (IO_Buffer ...)
 * 
 * 11    3.06.04 9:49 Rytz01
 * Hardware constants added
 * 
 * 10    28.05.04 17:44 Nohl01
 * 
 * 9     14.05.04 9:13 Bussli02
 * 
 * 8     13.05.04 10:03 Bussli02
 * 
 * 7     12.05.04 18:31 Maag01
 * 
 * 6     7.05.04 15:09 Maag01
 * 
 * 5     26.04.04 16:30 Leeb02
 * 
 * 4     22.04.04 13:26 Leeb02
 * 
 * 3     22.04.04 13:12 Leeb02
 * 
 * 2     7.04.04 10:28 Leeb02
 * 
 * 1     5.04.04 14:08 Leeb02

 *-------------------------------------------------------------------------*/
/*-----------------------GLOBAL CONSTANTS----------------------------------*/ 
#ifndef		_CONSTANT
#define		_CONSTANT


#define	N_DATA_RATES					    5		// number of data rates for MOD600 and MUX600


#define BASIC_SLOT_ID_P4LQ 0

#define NO_TDM_BASIC_TIMESLOTS		10	// Number of basic TDM time slots		// used by HMI600
#define NO_TDM_EXTENDED_TIMESLOTS	22	// Number of extended TDM time slots	// used by HMI600

#define NO_TDM_TIMESLOTS			(NO_TDM_BASIC_TIMESLOTS	+ NO_TDM_EXTENDED_TIMESLOTS)
										// Total number of TDM timeslots (basic and extended)	// used by HMI600

#define NO_O4LE_EXTENDED_TIMESLOTS	4	// Number of extended TDM time slots occupied by a O4LE	// used by HMI600
#define NO_G4AI_EXTENDED_TIMESLOTS	2	// Number of extended TDM time slots occupied by a G4AI	// used by HMI600

//M.Zarnovican 27.4.2011 Moved to class CRackSlotMapping, instead use static method CRackSlotMapping::GetNumberOfRackSlots(int rackType)
//#define NO_P7LC_MODULE_SLOTS		7	// Number of P7LC-slots for plugging the O4LE and G4AI	// used by HMI600	
										// modules
//#define NO_P7LF_MODULE_SLOTS		7
//#define NO_P7LH_MODULE_SLOTS		4
//M.Zarnovican 27.4.2011 The max number of mudules a rack can have is 7 (P7LC, P7LF racks). But rack P7LH can have only 4 modules. 
//Constant NO_P7LC_MODULE_SLOTS was used on many places (for instance O4LE boards array) where its replaced by MAX_NO_OF_MODULE_SLOTS
#define MAX_NO_OF_MODULE_SLOTS		7

#define NO_O4LE_APLC_CHANNELS		(NO_O4LE_EXTENDED_TIMESLOTS / 2)	// Number of APLC channels	// used by HMI600
																		// which can be served by
																		// each O4LE board

// Mapping of basic timeslots (BASIC_TS) to rackslots (RS) of rack P7LC
#define BASIC_TS_0_RS	11	// used by HMI600
#define BASIC_TS_1_RS	22	// used by HMI600
#define BASIC_TS_2_RS	28	// used by HMI600
#define BASIC_TS_3_RS	34	// used by HMI600
#define BASIC_TS_4_RS	54	// used by HMI600
#define BASIC_TS_5_RS	65	// used by HMI600
#define BASIC_TS_6_RS	71	// used by HMI600

// Sequence of P7LC-slots for plugging the O4LE and G4AI modules	// used by HMI600											
//M.Zarnovican 19.1.2011 Slot mapping depends on ETL600 (rack) type
//#define P7LC_MODULE_SLOT_SEQUENCE	{BASIC_TS_0_RS, BASIC_TS_1_RS, BASIC_TS_2_RS, BASIC_TS_3_RS, \
//									 BASIC_TS_4_RS, BASIC_TS_5_RS, BASIC_TS_6_RS}					

// Mapping of rackslots (RS) to subaddresses (SA)
#define RS_45_SA 0 // used by HMI600
#define RS_11_SA 1 // used by HMI600
#define RS_22_SA 2 // used by HMI600
#define RS_28_SA 3 // used by HMI600
#define RS_34_SA 4 // used by HMI600
#define RS_54_SA 5 // used by HMI600
#define RS_65_SA 6 // used by HMI600
#define RS_71_SA 7 // used by HMI600

// Mapping of subaddresses to basic timeslots
#define SUBADDRESS_TO_BASIC_TS(subaddress)	(subaddress-1)

// - Steuern die Zustände der Data Link Layer ( RS232, EOC, TDM )
// - Sowohl auf dem P4LT und den Slaves (G4AI, O4LE) 
#define EMPTY			10
#define RECEIVING		1
#define FULL			2
#define FORWARDED		3
#define EXECUTED		6
#define DISCARDED		7
#define IDLE			8
#define SENDING			9
#define TRANSMITTED     11

// general const
#define PI			3.14159265359
#define TWO_PI		6.28318530718
#define KILO		1000

// RF power settings
#define RF_PEP_MAX_10W		10
#define RF_PEP_MAX_12_5W	12
#define RF_PEP_MAX_20W		20
#define RF_PEP_MAX_25W		25
#define RF_PEP_MAX_40W		40
#define RF_PEP_MAX_50W		50
#define RF_PEP_MAX_80W		80
#define RF_PEP_MAX_100W	   100

// RF bandwidth of up/down-converter
#define RF_BANDWIDTH	32000	// [Hz]					

// sampling rates
#define RF_SAMPLE_FREQ      4096000.00000000000    		// rf sample frequency
#define AF_SAMPLE_FREQ		  10666.66666666667			// af sample frequency

#define BLOCK_LENGTH						2       	// number of AF samples per block
#define BLOCK_LENGTH_8K 					4			// number of AF samples per block (8k Version)
#define NR_OF_CHAN 							2       	// number of channels (1 or 2)
#define BUFF_LEN							288			// BLOCK_LENGTH * 144 	   
#define BUFF_LEN_SYNC						289			// ergibt eine Phasenverschiebung von 12/1.536MHz=7.8us
														
#define DUMMYLOAD_DMA_LEN					480

#define P4LT_RESET_TIME_IN_MS				18000

//*****************************************************************************
// Hardware constants for DSPs
#define DSP_CLK_EXT_Hz						(16384000UL) // ADSP-21161N external clock frequency in Hz
#define DSP_CLK_INT_Hz						(98304000UL) // ADSP-21161N core clock frequency in Hz
#define DSP_CYCLES_PER_MAINLOOP				9216		 // in 10.667 kHz main loop
#define DSP_CYCLES_PER_MAINLOOP_5K			18432		 // in 5.333 kHz main loop

#define	IO_BUFFER_DSP						0x08000000	 //external I/O Buffer Address (/MS2)

// Integer constants
#define N_0		0
#define N_1		1
#define N_2		2
#define N_3		3
#define N_4		4
#define N_5		5
#define N_6		6
#define N_7		7
#define N_8		8
#define N_9		9
 		

#define FULLSCALE_CODEC               8192.0       
// Berechnung des Ausgangspegel:	
// max. Ausgangspegel: 3Vpp, mit -6dB Ausgangsabschwaechung: -16dBr
// d.h. 3Vpp = +/- 8192 digital = 1.06Veff. 
// -16dBr am Ausgang (== -16dBu): +/- 1 digital RxAfRaw[0] muss am -16dBr Punkt 122.8mVeff ergeben.
// ==> Korrekturfaktor = (122.8mVeff / 1.06Veff) * 8192) = 949.0355
// #define RXP4LB_LEVEL_ADJ              949.0355
// Plus 1 dB (1.122) vom P4LB -> 949.0355 * 1.12201 = 1064.835345
// Plus die Equalizer Vorverzerrung     
// falls dieses define verwendet wird, muss im .cpp file ein #include "eq_coeff.h" gemacht werden 
//#define  RXP4LB_LEVEL_ADJ ( 1064.835345 * absTransferFunctionP4LBArr[19] )  /// = 1174.4675   
// dies hat nicht funktioniert (muliple defined absTransferFunctionP4LBArr beim includen von "eq_coeff.h"
// darum wieder hardcodiert
#define  RXP4LB_LEVEL_ADJ 1174.4675

// Berechnung des Ausgangspegel:	
// max. Ausgangspegel: 3Vpp, mit -6dB Ausgangsabschwaechung: -16dBr
// d.h. 3Vpp = +/- 8192 digital = 1.06Veff. 
// -16dBr am Ausgang (== -16dBu): +/- 1 digital RxAfRaw[0] muss am -16dBr Punkt 122.8mVeff ergeben.
// ==> Korrekturfaktor = (122.8mVeff / 1.06Veff) * 8192) = 949.0355
// in dB 20LOG(8192/ 949.0355) = 18.7 + 1dB vom RX-Teil P4LQ
#define  RXP4LB_LEVEL_ADJ_IN_DB	19.72215F

// Testton Frequenz fuer AF-Baugruppen 
#define RX_AF_TESTTON_FREQU 		  800.0 

// Berechnung der Eingangspegel:	
// max. Eingagsspegel: 3Vpp, mit +6dB Eingangsverstärkung: 6Vpp am Eingang AD Wandler.
// d.h. 6Vpp = +/- 8192 digital. 
// -16dBu am Eingang (=0.3474135Vpp) muss +/- 1 digital TxAf[0] bzw. 0dB mi sample Tx Data ergeben.
// ==> Korrekturfaktor = 3Vpp / ( 0.3474135Vpp * 8192) = 1.05414E-03
#define TXP4LB_LEVEL_ADJ   			1.05414E-03  


#define LOCAL_LOOP_P4LB_LEVEL_ADJ		1.6		// Der Wert 1.6 wurde messtechnisch ermittelt
													// sodass RX Level an G4AA = -6dB ist

#define BUSY 1
#define FREE 0
   
#define TDM_SERVICE_TIMEOUT       1000          // 1000 * 1/10.66kHz = about 90ms

#endif // _CONSTANT
