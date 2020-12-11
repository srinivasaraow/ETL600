/*-------------------------------------------------------------------------*
 * $Workfile: NSD_Types.h $
 * Part of      : ETL600 / NSD
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : NSD global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/NSD_Types.h $
 * 
 * 133   14.06.13 8:33 Chmaste
 * Tx input on-delay: enlarge range to 0..20 [ms]
 * 
 * 132   12-09-14 18:18 Chhamaa
 * Macros modified for 2.5 kHz TPS mode.
 * 
 * 131   12-09-07 17:31 Chhamaa
 * Define INT_TELPROT_MODE_HSD_25 for 2.5 kHz TPS mode added.
 * 
 * 130   14.03.12 11:00 Chthbae
 * AES Tx Trip monitoring changed
 * 
 * 129   11-03-14 15:51 Chmazar
 * 
 * 129   27.08.10 13:39 Chhamaa
 * Comment added for AES600_EXTENDED_MODE.
 * 
 * 127   13.11.08 13:49 Chhamaa
 * Comment for AES600_SIGNALING_MODE added.	
 * 
 * 126   12.09.08 17:11 Chsanoh
 * AES state mode for line protection command A introduced.
 * 
 * 125   6.06.08 15:54 Chsanoh
 * 3rd MAS implementation phase
 * 
 * 123   22.12.06 14:11 Chsanoh
 * Mezzanine check box for AES modes 3 and 4 added in developer mode
 * (<Configuration> <AES600...> <Modes>).
 * 
 * 121   14.12.06 13:30 Chmaflu
 * delete functionlibary.h
 * 
 * 120   14.12.06 11:51 Chmaflu
 * "NSD extended modification: 
 * Add 3 bitmasks for NSD, NSD extended TPE1, NSD extended TPE2 
 * Change old macros to check Teleprotection mode now with bitmask from
 * NSD
 * Add macro IS_NSD_EXTENDED, IS_NSD_EXTENDED_TPE1,IS_NSD_EXTENDED_TPE2"
 * 
 * 119   13.11.06 8:56 Chpeplu
 * Macro CNT_TPE corrected missing ")" added
 * 
 * 118   13.11.06 8:39 Chpeplu
 * New define ALL_LP_COMMANDS_A_F added 
 * 
 * 117   10.11.06 13:54 Chpeplu
 * New defines added (INT_TPE_MODE_MASK, INT_TPE1_MODE_EXT_MASK,
 * INT_TPE2_MODE_EXT_MASK) and CNT_TPE macro changed
 * 
 * 116   8.11.06 15:06 Chtognu
 * Order of the "reserved" variables in  t_NSD_Config struct changed.
 * Moved belove customTable struct in preparation for Custom Table
 * extentions
 * 
 * 115   10.10.06 15:03 Chsanoh
 * Defines NSD_G4AI_INFINITE_TX_MONIT_DURATION and
 * NSD_G4AI_INFINITE_RX_MONIT_DURATION removed.
 * 
 * 114   4.10.06 11:26 Chtognu
 * definition #define NSD_MODE_RXOUTDURPROL added
 * (changes labeled with gn_bif)
 * 
 * 113   21.09.06 15:39 Chsanoh
 * AES status view adapted.
 * 
 * 112   20.09.06 16:33 Chhamaa
 * Comment added.
 * 
 * 111   14.09.06 15:15 Chsanoh
 * TANG window and Rx guard signaling implemented in corresponding masks.
 * 
 * 110   12.09.06 14:33 Chhamaa
 * NSD_CMD_OUTPUTS_BLOCKED_ALARM added.
 * 
 * 109   8.09.06 16:12 Chhamaa
 * - RxGuardOnOffDelay added to NSD and AES config. structure
 * - TANG_DelayOnOff modified in AES config. structure
 * 
 * 108   1.09.06 12:53 Chhamaa
 * Tx command masks modified.
 * 
 * 107   31.08.06 10:20 Chsanoh
 * Macro IS_HSD600 adapted.
 * 
 * 106   29.08.06 11:40 Chhamaa
 * Define NO_OF_TONES_AES added.
 * 
 * 105   28.08.06 10:11 Chsanoh
 * Bug 375 corrected.
 * 
 * 104   17.08.06 17:30 Chsanoh
 * Bug 85 and 311 fixed.
 * 
 * 103   11.08.06 13:13 Chsanoh
 * Brackets at existing macros added.
 * 
 * 102   11.08.06 10:06 Chsanoh
 * Usage of defines INT_TELPROT_MODE_... harmonized.
 * 
 * 101   7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 101   7.07.06 18:50 Chhamaa
 * 
 * 99    28.04.06 14:54 Chsanoh
 * Bug 322 corrected.
 * 
 * 97    21.03.06 11:16 Chsanoh
 * '<Configuration> <NSD600...>, application of commands:
 * List box index does no longer correspond to command value written to
 * config. Newly, selected string item will be mapped to corresponding
 * command value.
 * 
 * 96    28.02.06 13:42 Chsanoh
 * #define NSD_G4AI_INFINITE_MONIT_DURATION adapted.
 * 
 * 95    24.02.06 16:14 Chsanoh
 * HSD detail configuration:
 * - Detail configuration enabled under <Configuration> <NSD600...>.
 * - Corresponding dialog adapted to HSD (new resource with ID
 * IDD_HSD600_SYSTEM).
 * 
 * Configuration view:
 * - Section "Pilot settings APLC channel 1" extended by "Power guard [W]"
 * and "Power guard [dBm]".
 * 
 * AGC blocking level:
 * - If persistent command transmission is enabled, the AGC blocking level
 * range is limited to 16 dB below the nominal pilot signal level.
 * 
 * Transmit and receive filters with a bandwidth of 4 kHz:
 * - HMI600 newly takes into account the 4 kHz Tx RF filters and the 4 kHz
 * Rx RF filters for calculation of the following Tx RF and Rx RF filter
 * parameters:
 *      - Default filter center frequency
 *      - Default filter bandwidth
 * - 4 kHz Tx RF filters and 4 kHz Rx RF filters tunable and testable
 * under <Equipment> <Tuning & Testing...> Remark: No Tx RF filter main
 * tables for 4 kHz available yet."
 * 
 * 94    31.01.06 16:21 Chpeplu
 * new defines for HSD600 added INTEGR_TIME_DETECTOR_HSD, MAX_INTEGR_TIME,
 * NO_OF_TONES_HSD	
 * 
 * 93    31.01.06 16:03 Chsanoh
 * Define INT_TELPROT_MODE_HSD added. Macros IS_TPE1_IN_APLC1(),
 * IS_NSD600(), CNT_TPE() adapted.
 * 
 * 91    26.10.05 17:17 Chpeplu
 * Cleand up -> but high risk task
 * 
 * 90    25.10.05 16:39 Chchlee
 * Cleaned uo
 * 
 * 89    25.10.05 15:51 Chchlee
 * Cleaned up
 * 
 * 88    11.10.05 18:02 Chchlee
 * 
 * 87    30.06.05 16:20 Chsanoh
 * 
 * 86    30.06.05 8:13 Chchlee
 * cleaned up
 * 
 * 85    22.06.05 16:25 Chchlee
 * 
 * 84    21.06.05 14:11 Chpeplu
 * new dwwfines CORR_TERM_LIN_480_4000HZ, CORR_TERM_LIN_GUARD_TO_TRIP
 * added
 * 
 * 83    23.05.05 7:06 Chchlee
 * 
 * 82    19.04.05 17:22 Chsanoh
 * 
 * 81    19.04.05 11:54 Chsanoh
 * 
 * 80    16.03.05 10:50 Chpttra1
 * 
 * 79    14.03.05 17:37 Chsanoh
 * guard weight added to AES config structure
 * 
 * 78    14.03.05 10:52 Chpeplu
 * new define ALL_LP_COMMANDS added
 * 
 * 77    9.03.05 13:34 Chhesch5
 * 
 * 76    9.03.05 11:12 Chhesch5
 * 
 * 75    9.03.05 10:15 Chpeplu
 * nested comment removed
 * 
 * 74    9.03.05 9:40 Chhesch5
 * new config structs
 * 
 * 73    8.03.05 16:10 Chpttra1
 * Optimization of AES structure is done.
 * 
 * 72    8.03.05 13:19 Chpttra1
 * 
 * 71    7.03.05 17:45 Chpttra1
 * AES structure was optimized
 * 
 * 70    4.03.05 16:21 Chhesch5
 * NsdMode and GuardLevel added
 * 
 * 69    4.03.05 13:58 Chhesch5
 * 
 * 68    2.03.05 14:24 Chhesch5
 * 
 * 67    2.03.05 10:34 Chhesch5
 * 
 * 66    1.03.05 8:21 Chpttra1
 * 
 * 65    28.02.05 14:49 Chhesch5
 * preparation for new config structs
 * 
 * 64    25.02.05 17:21 Chhesch5
 * 
 * 63    24.02.05 18:04 Chpttra1
 * New fields were added to new AES structure according to TRS for TPE
 * Type AES600 Release 1
 * 
 * 62    14.02.05 13:12 Chhesch5
 * 
 * 61    8.02.05 10:38 Chpeplu
 * define CORR_TERM_GUARD_TO_TRIP changed from 6.0 to 3.0
 * 
 * 60    7.02.05 11:42 Chpeplu
 * Union name u_NSD_AES added to remove warning 
 * 
 * 59    7.02.05 8:42 Ats
 * 
 * 57    4.02.05 13:37 Chhesch5
 * 
 * 56    3.02.05 17:25 Chhesch5
 * 
 * 55    3.02.05 16:19 Chhesch5
 * 
 * 54    2.02.05 17:39 Chsanoh
 * 
 * 53    1.02.05 15:23 Chpeplu
 * define NSD_TEST_SIGNAL_WARNING added
 * 
 * 52    25.01.05 17:29 Chhesch5
 * 
 * 51    25.01.05 11:36 Chhesch5
 * added defines for NSD local loop (test mode)
 *
 * 50    18.01.05 15:54 Chhesch5
 * 
 * 49    11.01.05 11:42 Chpeplu
 * define NOT_USED_23 replaced by NSD_RX_TRIP_DURATION_ALARM
 * 
 * 48    6.01.05 17:09 Chhesch5
 * 
 * 47    6.01.05 14:57 Chhesch5
 * 
 * 46    6.01.05 7:07 Chhesch5
 * 
 * 45    21.12.04 15:55 Chpeplu
 * struct t_NSD_Status moved to P4LT_Types.h
 * 
 * 44    20.12.04 10:16 Chhesch5
 * 
 * 43    16.12.04 7:26 Chpeplu
 * Struct t_NSD_DevelopperStatus_Type extended by
 * G4AI_TDM_Bus_Failure_CounterCh1/2
 * 
 * 42    14.12.04 17:01 Chhesch5
 * 
 * 41    3.12.04 13:23 Chpeplu
 * structs t_NSD_Status and t_NSD_DevelopperStatus_Type added
 * 
 * 40    2.12.04 13:16 Chpeplu
 * NSD Alarm Bits and Warning Bits defined
 * 
 * 39    29.11.04 8:48 Leeb02
 * 
 * 38    29.11.04 8:28 Leeb02
 * 
 * 37    9.11.04 11:24 Schnyd01
 * 
 * 36    14.10.04 15:49 Schnyd01
 * 
 * 35    23.09.04 18:14 Nohl01
 * 
 * 34    9.09.04 18:37 Leeb02
 * 
 * 33    9.09.04 17:33 Schnyd01
 * 
 * 32    8.09.04 14:04 Schnyd01
 * changed datatype "UnblockingThreshold" form long to float
 * 
 *-------------------------------------------------------------------------*/

#ifndef NSD_TYPES_H
#define NSD_TYPES_H


/* INCLUDE FILES ***********************************************************/

#include "basetype.h"
#include "constant.h"
#include "event_defs.h"		
#include "nsdCustomTable.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define MAX_NUMBER_OF_G4AI 7

#define	NO_TPES			2	// Number of TeleProtection Equipments (TPE1 and TPE2)

 
// Number of Tones
#define NO_OF_TONES		8	// used by HMI600
#define NO_OF_TONES_HSD	6	
#define NO_OF_TONES_AES 9	// number of tones for 20 AES commands


// #define ONE_MINUTE 	6400		//  60*106.666 im 100 Hz Takt
#define ONE_SECOND		5333		// die Zyklusszeit ist 1/5333						// used by HMI600	
#define ONE_MILESEC		   5		// Diese Angabe ist nicht ganz OK: müsste 5.3333	// used by HMI600
#define CYCLE_TIME		0.188		// Angabe in ms -> CYCLE_TIME=1/5333				// used by HMI600
 										
 
#define NOISE_BW_CORRECTION			   (4.0f*0.03356f)		// Korrekturfaktor zwischen Pilotfilter und 4KHz	// used by HMI600
 															// 134Hz/4000 Hz = PilotBW / MeasureBW 
                                                        	// 4* da die SNR Berechnung nur die halbe
															// Rauschleistung berücksichtigt
#define GUARD_LEVEL_DIVISOR			   42427				// Used by HMI600
															// 60000*0.70711 = 42427 (at 0dB)
#define CORR_TERM_480_4000_HZ		   9.2F					// Used by HMI600
															// Correction of bandwidth from 480Hz to 4kHz
															// (10*log10(4000/480))
#define CORR_TERM_LIN_480_4000HZ	   8.3F					// linear correction term bandwidth 480Hz to 4000Hz
#define CORR_TERM_GUARD_TO_TRIP		   3.0F					// Used by HMI600
															// Correction term for guard to trip level
#define CORR_TERM_LIN_GUARD_TO_TRIP	   2.0F					// linear correction term guard to trip
#define SINGLE_TONE_TO_DUAL_TONE_RATIO 3.0F					// Ratio between dual tone commands (NSD) to single
															// tone commands (AES, coded FSK) [dB]

 
///// Defines for NSD550Flags
#define NSD_PILOT			0x00000004	// used by HMI600  
#define AUTO_LOOPTEST_ON	0x00000040	// used by HMI600
#define	UNBLOCKING_THRES 	0x00008000	// used by HMI600
#define	NO_SPEECH_AVAIL		0x00010000	// used by HMI600

// defines for NSD_Mode
#define	NSD_MODE1		0x00001		// used by HMI600
#define	NSD_MODE2		0x00002		// used by HMI600
#define	NSD_MODE3		0x00003		// used by HMI600


// defines for NSD local loop (test mode)
#define	TPE1	1
#define	TPE2	2


#define	TPE_CLEAR_TEST_MODE		0
#define	TPE_SET_TEST_MODE		1



/* GLOBAL TYPE DECLARATIONS ************************************************/

                 
// structures
  

//// Defines for NSDAlarms:
#define TDM_ALARM          		    0x00000008		// hw alarm	    // used by HMI600

#define SNR                    	    0x00000010		// link alarm
#define RX_LEVEL                    0x00000020		// link alarm	// used by HMI600
#define CYCLIC_LOOP_TEST_FAIL       0x00000040		// link alarm	// used by HMI600

#define EVENTRECORDER_DISABLED		0x00001000		// hw alarm		// used by HMI600
#define GUARD_AND_TRIP				0x00002000		// link alarm	// used by HMI600
#define G4AI_HW_ALARM				0x00004000		// hw alarm		// used by HMI600

#define CUSTOM_TABLE_CHECKSUM_AL	0x00040000		// hw alarm		// used by HMI600

#define WARNING_NSD_TESTSIGNAL		0x08000000		// used by HMI600

#define WARNING_LT_FAILED			0x10000000		// used by HMI600			
#define WARNING_LT_OUT_OF_RANGE	    0x20000000		// used by HMI600
#define WARNING_LOCAL_LOOP_ON		0x40000000		// used by HMI600
#define WARNING_EREC_BLOCK_CHANGE	0x80000000		// used by HMI600

#define NSD_LINK_ALARM (SNR | RX_LEVEL)
#define NSD_LINK_ALARM2 (CYCLIC_LOOP_TEST_FAIL | GUARD_AND_TRIP)

#define NSD_WARNING (WARNING_LT_FAILED | WARNING_LT_OUT_OF_RANGE | WARNING_LOCAL_LOOP_ON | \
					WARNING_EREC_BLOCK_CHANGE | WARNING_NSD_TESTSIGNAL)

#define NSD_HW_ALARMS ( TDM_ALARM | EVENTRECORDER_DISABLED | G4AI_HW_ALARM | \
					CUSTOM_TABLE_CHECKSUM_AL)
//
//
// ************************************************************************************************
// ************************************************************************************************
// ********************   NSD600 defines used    **************************************************
// ************************************************************************************************
// ************************************************************************************************
//
// Defines for new NSD Alarm word in NSD600 status
/*
#define	LIFE_SIGNE_0					0x00000001
#define	LIFE_SIGNE_1					0x00000002
#define	LIFE_SIGNE_2					0x00000004
#define	NOT_USED_3						0x00000008
//
#define	NSD_SUM_HW_ALARM				0x00000010
#define NSD_SUM_LINK_ALARM				0x00000020
#define	NSD_SUMMARY_ALARM				0x00000040
#define	NSD_SUM_WARNING					0x00000080
//
#define	NOT_USED_8						0x00000100
#define	NOT_USED_9						0x00000200
#define	NOT_USED_10						0x00000400
#define	NOT_USED_11						0x00000800
//
#define	NSD_DATA_MEM_ERROR				0x00001000
#define	NSD_CRC_PROG_ERROR				0x00002000
#define	NSD_EXT_TDM_BUS_COM_ALARM		0x00004000
#define	NOT_USED_15						0x00008000
// 
	*/
#define	NSD_RX_GUARD_LOW_LEVEL_ALARM	0x00010000
#define	NSD_RX_GUARD_HIGH_LEVEL_ALARM	0x00020000
#define	NSD_SNR_ALARM					0x00040000
#define	NSD_NO_TRIP_AND_GUARD_ALARM		0x00080000
//
#define	NSD_TRIP_AND_GUARD_ALARM		0x00100000
#define	NSD_OUTPUT_SET_ALARM			0x00200000
#define	NSD_LOOP_TEST_ALARM				0x00400000
#define	NSD_RX_TRIP_DURATION_ALARM		0x00800000
//
#define	NSD_CMD_OUTPUTS_BLOCKED_ALARM	0x01000000
#define	NOT_USED_25						0x02000000
#define	NOT_USED_26						0x04000000
#define	NSD_TEST_SIGNAL_WARNING			0x08000000
//
#define	NSD_TEST_CONFIG_WARNING			0x10000000
#define NSD_LOOP_TEST_WARNING			0x20000000
#define	NSD_LOCAL_TEST_MODE_WARNING		0x40000000
#define	NSD_REMOTE_TEST_MODE_WARNING	0x80000000
//
// integr. length defines for fs=5.333kHz => 4.125ms
#define	INTEGR_TIME_DETECTOR		22
//integr. length defines for fs=5.333kHz 
// tev = 3.375ms
//#define INTEGR_TIME_DETECTOR_HSD	18
// tev = 4.125ms
//#define INTEGR_TIME_DETECTOR_HSD	22
// tev = 5.25ms
#define INTEGR_TIME_DETECTOR_HSD	28
//
// used for Array definition of integration sum length
// -> set to max value of INTEGR_TIME_DETECTOR or INTEGR_TIME_DETECTOR_HSD
//
#define MAX_INTEGR_TIME				34

//
// Max. number of NSD600 commands depends on the used bits in the external tdm-bus allocations Command A-F 
// see \\CH-F-BACLUR1105\UT0$\AK\Dev\NT_COM\ProjectETL600\Technical Reports\P4LT external TDM Bus Data Comunication.doc
// or "TN-NKT00-1507_TDM_Bus_Concept_AECS.doc" 
// (persistent command not included)
#define MAX_NUMBER_LP_COMMANDS	6

// max. nubmer of currently used commands, 
#define MAX_NUMBER_USED_LP_COMMANDS	4		// HMI600 



// lower limit of UnblockingThreshold, in dB for use in HMI config mask	
#define	UNBLOCKING_THRESHOLD_LOWER			-20		
// upper limit of UnblockingThreshold, in dB for use in HMI config mask	
#define	UNBLOCKING_THRESHOLD_UPPER			-10		

// lower limit of Guard Alarm Threshold, in dB for use in HMI config mask		
#define	GUARD_ALARM_THRESHOLD_LOWER			3
// upper limit of Guard Alarm Threshold, in dB for use in HMI config mask	
#define	GUARD_ALARM_THRESHOLD_UPPER			12


// lower limit of RxOutComMonit_Duration and TxInpComMonit_Duration in s, used in HMI config mask		
#define	COM_MONIT_DURATION_LOWER			1
// upper limit of RxOutComMonit_Duration and TxInpComMonit_Duration in s, used in HMI config mask		
#define	COM_MONIT_DURATION_UPPER_R3				60
#define	COM_MONIT_DURATION_UPPER_R4				10
#define	COM_MONIT_DURATION_UPPER_AES_EXTENDED	10

//Default value of TxInpComMonit_Duration in s
#define	COM_MONIT_DURATION_DEFAULT				5

// lower limit of TimedTxPowerReduction in s, used in HMI config mask		
#define	TIMED_TX_POWER_REDUCTION_LOWER		1
// upper limit of TimedTxPowerReduction in s, used in HMI config mask		
#define	TIMED_TX_POWER_REDUCTION_UPPER		60


// lower limit of TxInpComDelay in ms, used in HMI config mask		
#define	TX_INP_COM_DELAY_LOWER				0
// upper limit of TxInpComDelay in ms, used in HMI config mask	
#define	TX_INP_COM_DELAY_UPPER				20


// lower limit of TxInpComDuration in ms, used in HMI config mask		
#define	TX_INP_COM_DURATION_LOWER			30
// upper limit of TxInpComDuration in ms, used in HMI config mask	
#define	TX_INP_COM_DURATION_UPPER			1000


// lower limit of TxInpComProlongation in ms, used in HMI config mask		
#define	TX_INP_COM_PROLONGATION_LOWER		0
// upper limit of TxInpComProlongation in ms, used in HMI config mask	
#define	TX_INP_COM_PROLONGATION_UPPER		3000


// lower limit of RxOutComDelay in ms, used in HMI config mask		
#define	RX_OUT_COM_DELAY_LOWER				0
// upper limit of RxOutComDelay in ms, used in HMI config mask	
#define	RX_OUT_COM_DELAY_UPPER				10


// lower limit of RxOutComDuration in ms, used in HMI config mask		
#define	RX_OUT_COM_DURATION_LOWER			30
// upper limit of RxOutComDuration in ms, used in HMI config mask	
#define	RX_OUT_COM_DURATION_UPPER			1000


// lower limit of RxOutComProlongation in ms, used in HMI config mask		
#define	RX_OUT_COM_PROLONGATION_LOWER		0
// upper limit of RxOutComProlongation in ms, used in HMI config mask	
#define	RX_OUT_COM_PROLONGATION_UPPER		3000


// frequency of NSD own guard without configured speech [Hz]
#define	NSD_OWN_GUARD_WITHOUT_SPEECH_FREQ	2240.0F
// NSD own guard frequency offset to configured speech [Hz]
#define	NSD_OWN_GUARD_OFFSET_TO_SPEECH_FREQ	240.0F


// frequency of AES own guard without configured speech [Hz]
#define	AES_OWN_GUARD_WITHOUT_SPEECH_FREQ	2240.0F
// AES own guard frequency offset to configured speech [Hz]
#define	AES_OWN_GUARD_OFFSET_TO_SPEECH_FREQ	240.0F
// frequnecy of AES with: - own guard 
//						  - enabled AES commands 1-20 [Hz]
#define	AES_OWN_GUARD_COMMANDS_1_20_FREQ	2440.0F


// Defines used for member variables of t_IntTeleprotConfig

// Defines for long "mode" in t_IntTeleprotConfig // used by HMI600
#define	INT_TELPROT_MODE_NONE						(long) 0	// No teleprotection	
#define INT_TELPROT_MODE_AES						(long) 1	// AES600					
#define INT_TELPROT_MODE_NSD1_APLC1					(long) 10	// NSD600 TPE1 in APLC1, no TPE2
#define INT_TELPROT_MODE_NSD1_DPLC					(long) 11	// NSD600 TPE1 in DPLC, no TPE2
#define INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC		(long) 12	// NSD600 TPE1 in APLC1,TPE2 in DPLC
#define INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2		(long) 13	// NSD600 TPE1 in APLC1,TPE2 in APLC2
#define INT_TELPROT_MODE_HSD						(long) 20	// HSD600 TPE1 in APLC1, no TPE2
#define INT_TELPROT_MODE_HSD_25						(long) 21	// HSD600 2.5kHz TPE1 in APLC1, no TPE2
//
#define INT_TPE_MODE_MASK							0x0000FFFF   //16 Bit
#define INT_TPE1_MODE_EXT_MASK						0x00FF0000   //8 Bit (<<16)
#define INT_TPE2_MODE_EXT_MASK						0xFF000000   //8 Bit (<<16)
//

#define MAX_NUMBER_AES_LP_COMMANDS		5					// number of AES LP commands (A..D & P)
#define MAX_NUMBER_AES_AES_COMMANDS		20					// number of AES commands


#define TANG_DELAY_ON_MASK			0x0000FFFF
#define TANG_DELAY_OFF_MASK			0xFFFF0000		 		

#define RX_GUARD_ON_DELAY_MASK		0x0000FFFF
#define RX_GUARD_OFF_DELAY_MASK		0xFFFF0000


// NSD600-specific configuration data
//
typedef struct {
	unsigned long	NsdMode;									// see defines NSD_MODE_*
	long			UsedCommands;								// UsedCommands 
	long			UnblockingPickupTime;						// in ms
	long			UnblockingPulsDuration;						// in ms
	float			UnblockingThreshold;						// UnblockingThreshold (-20..-10dB) 0.01 .. 0.1 linear
	float			PilotFrequency;								// frequency in Hz NSD own guard or ETL Pilot frequnecy 
	float			GuardLevel;									// lin. guard level (ETL pilot or NSD own guard)
	long			TimeBetweenCL_Test;							// time in hours
	float			BoostLevel;									// configured boost level linear (not in dB!)
	float			maxBoostLevelinDB;							// Maximum allowed boostlevel in dB (not used in P4LT)
	long			CommandApplication[MAX_NUMBER_LP_COMMANDS+1];		// 0 = off, 1 = blocking, 2 = permissive, 3 = direct (Command A-D )

	long			TxInpComDelay[MAX_NUMBER_LP_COMMANDS+1];			// 0-10ms for each command separately
	long			TxInpComDuration[MAX_NUMBER_LP_COMMANDS+1];			// fix duration not yet used for Rel. 1 (0ms ... 1000ms; 0 means as long as injected)
	long			TxInpComProlongation[MAX_NUMBER_LP_COMMANDS+1];		// not yet used for Rel. 1 (0ms ... 3000ms)
	long			TxInpComMonit_Duration[MAX_NUMBER_LP_COMMANDS+1];	// 1 ... 60sec for each command separately (0 means monitoring off)

	long			RxOutComDelay[MAX_NUMBER_LP_COMMANDS+1];			// not yet used for Rel. 1 (0ms ... 10ms)
	long			RxOutComDuration[MAX_NUMBER_LP_COMMANDS+1];			// fix output duration not yet used for Rel. 1 (0ms ... 1000ms) 0 means as long as received
	long			RxOutComProlongation[MAX_NUMBER_LP_COMMANDS+1];		// 0ms ... 3000ms for each command separately
	long			RxOutComMonit_Duration[MAX_NUMBER_LP_COMMANDS+1];	// 1s ... 60sec for each command separately  (0 means monitoring off)

	long			StartInputActivTime;								// default 1 sec
	long			StartInputMonit_Duration;							// 
	long			OutputsAtRxAlarm;									// see define for OutputsAtRxAlarm below
	long			OutputsAtRxAlarmPickupTime;							// 
	long			OutputsAtRxAlarmHoldTime;							// 
	float			SNR_Alarm_Threshold;								// SNR alarm threshold (0...12dB) 1.0 ...15.848
	float			Guard_Alarm_Threshold;								// Guard alarm threshold (+/-3 ... +/- 12dB) 0.5 ... 0.063096
	long			UsedExtTimeSlots;									// BIT0 == 1 --> G4AI in SLOT_N11 used with this TPE etc.
	
	t_PackedNSDCustomTable	customTable;
	long			Reserved3; // reserved for futur extensions --> PRIMARILY FOR CUSTOMTABLE EXTENTIONS
	long			Reserved4; // reserved for futur extensions --> PRIMARILY FOR CUSTOMTABLE EXTENTIONS


	long			TimedTxPowerReduction;
	unsigned long	RxGuardOnOffDelay;			// bits 0-15: rx guard on delay [ms], bits 16-31: rx guard off delay [ms]

} t_NSD_Config;	// used by HMI600



// Defines used for member variables of t_NSD_Config

// defines for OutputsAtRxAlarm
#define		OUTPUT_AT_RX_ALARM_UNAFFECTED							0
#define		OUTPUT_AT_RX_ALARM_DIRECT_TO_GUARD_OTHERS_TRIPPED		1
#define		OUTPUT_AT_RX_ALARM_FROZEN								2
#define		OUTPUT_AT_RX_ALARM_SET_TO_GUARD_STATE					3


// defines for Command Application
#define NO_OF_APPLICATIONS			4

#define	COMMAND_OFF					0x0000
#define	BLOCKING					0x0001
#define	PERMISSIVE					0x0002
#define	DIRECT						0x0003

// defines for UsedCommands
#define	USED_CMD_A_RX		BIT0
#define	USED_CMD_B_RX		BIT1
#define	USED_CMD_C_RX		BIT2
#define	USED_CMD_D_RX		BIT3
#define	USED_CMD_E_RX		BIT4
#define	USED_CMD_F_RX		BIT5
#define	USED_CMD_G_RX		BIT6
#define	USED_CMD_A_TX		BIT12
#define	USED_CMD_B_TX		BIT13
#define	USED_CMD_C_TX		BIT14
#define	USED_CMD_D_TX		BIT15
#define	USED_CMD_E_TX		BIT16
#define	USED_CMD_F_TX		BIT17
#define	USED_CMD_G_TX		BIT18		// rename to USED_CMD_P_TX

#define	ALL_LP_COMMANDS				0x0000000f	// LP cmd A-D
#define ALL_LP_COMMANDS_A_F			0x0000003f	// LP cmd A-F
#define ALL_LP_AES_TX_COMMANDS		0xfffff00f	// LP cmd A-D and AES tx cmd 1-20
#define ALL_LP_AES_RX_COMMANDS		0x0000F80f	// LP cmd A-D and AES rx cmd 1-20


#define NUM_OF_NSD_INST		2			// number of possible NSD600 instances	
#define NUM_OF_AES_INST		1			// number of possible AES600 instances	

// defines for NsdMode
#define NSD_MODE_OWN_GUARD_USED					BIT0	// 0 = ETL-Pilot, 1 = NSD own guard
#define NSD_MODE_CYCLIC_LOOPTEST_ON				BIT1	// 
#define NSD_MODE_PERSISTENT_CMD_USED			BIT2	//
#define NSD_MODE_BOOST_RATIO					BIT3	// 0 = user select, 1 = auto select
#define	NSD_MODE_RXOUTDURPROL					BIT4	// (0 - Rx Output Prolongation, 1 - Rx Output Duration, default:0) 
#define NSD_INIT								(NSD_MODE_BOOST_RATIO)

// AES600 configuration data (based on AES600 TRS)
//
typedef struct {

	// line protection commands
	long		TxInpComDelay[MAX_NUMBER_AES_LP_COMMANDS];				// 0-10ms for each command separately
	long		TxInpComDuration[MAX_NUMBER_AES_LP_COMMANDS];			// Rel.2.{65} Fix input duration (0ms ... 1000ms, 0 means as long as received)
	long		TxInpComProlongation[MAX_NUMBER_AES_LP_COMMANDS];		// not yet used for Rel. 1 (0ms ... 3000ms)
	long		TxInpComMonit_Duration[MAX_NUMBER_AES_LP_COMMANDS];		// 1 ... 60 sec for each command separately (0 means monitoring off)
	long		RxOutComDelay[MAX_NUMBER_AES_LP_COMMANDS];				// Rel.2.{65} 0-10ms for each command separately
	long		RxOutComDuration[MAX_NUMBER_AES_LP_COMMANDS];			// fix output duration not yet used for Rel. 1 (0ms ... 1000ms) 0 means as long as received
	long		RxOutComProlongation[MAX_NUMBER_AES_LP_COMMANDS];		// 0ms ... 3000ms for each command separately
	long		RxOutComMonit_Duration[MAX_NUMBER_AES_LP_COMMANDS];		// Rel.2.{65} 1 ... 60 sec for each command separately (0 means monitoring off)

	// AES commands
	long		TxInpRxOutComParametersAES[MAX_NUMBER_AES_AES_COMMANDS];// Bit masks are in AES600.h
																		// first   4 bits: TxInpComDelayAES (0-10ms for each command separately)
																		// second 14 bits: Rel.2{65} TxInpComProlongationAES (input prolongataion (0ms ... 3000ms))
																		// third   6 bits: Rel.2{65} TxInpComMonit_DurationAES (1 ... 60 sec for each command separately (0 means monitoring off))
																		// fourth  4 bits: Rel.2{65} RxOutComDelayAES (0-10ms for each command separately)

	long		RxOutComParametersAES[MAX_NUMBER_AES_AES_COMMANDS];		// Bit masks are in AES600.h
																		// first  12 bits: RxOutComDurationAES (fix output duration (0ms ... 1000ms, 0 means as long as received))
																		// second 14 bits: RxOutComProlongationAES (output prolongataion (0ms ... 3000ms) )
																		// third   6 bits: Rel.2{65} RxOutComMonit_DurationAES (1 ... 60 sec for each command separately (0 means monitoring off))
	
	unsigned long	AES600_Mode;	// accamulative long for the following values:
									// bit 0  - AES600_AES_MODE								(0 - 12 cmd, 1 - 20 cmd, default:0)
									// bit 1  - AES600_T0									(0 - T01, 1 - T02, default:0)
									// bit 2  - AES600_T0_AES								(0 - T01, 1 - T02, default:0, Rel.2) // not used
									// bit 3  - AES600_LP_MODE								(0 - Mode 3,4, 1 - Mode 1,2, default:0)
									// bit 4  - AES600_SEC_DEP_CMD							(0 - icreased dependability, 1 - increased security, default:1)
									// bit 5  - AES600_SEC_DEP_CMD_AES						(0 - icreased dependability, 1 - increased security, default:1, Rel.2) // not used
									// bit 6  - AES600_RESET_RX_ALARM						(0 - automatically, 1 - manually, default:0)
									// bit 7  - AES600_NSD_ETL_GUARD						(0 - ETL pilot, 1 - own guard, default:0)
									// bit 8  - AES600_LOOPTEST_ON_OFF						(0 - off, 1 - on, default:0)
									// bit 9  - AES600_PERSIST_ON_OFF						(0 - off, 1 - on, default:0)
									// bit 10 - AES600_RXOUTDURPROL_A						(0 - Rx Output Prolongation, 1 - Rx Output Duration, default:0)
									// bit 11 - AES600_RXOUTDURPROL_B						(0 - Rx Output Prolongation, 1 - Rx Output Duration, default:1)
									// bit 12 - AES600_MODE_BOOST_RATIO					    (0 = user select, 1 = auto select)
									// bit 13 - AES600_TANG_ON_OFF							(0 = TANG window off, 1 = TANG window on)
									// bit 14 - AES600_DEVELOPER_MEZZANINE					(0 = Mezzanine bit will not be set on G4AI, 1 = Mezzanine bit will be set on G4AI)
									//														Only relevant in developer mode for AES-modes 3 and 4!
									// bit 15 - AES600_CMDA_STATE_MODE						(0 = state mode for command A of group A off, 1 = state mode for command A of group A on)
									// bit 16 - AES600_SIGNALING_MODE						(0 = normal (3 cycles), 1 = fast (2 cycles))
									// bit 17 - AES600_EXTENDED_MODE						(0 = normal mode (stepping), 1 = extended mode (state) with 4 independent line cmd
									// Bit masks are in AES600.h

	
//	long		AES_Mode;						// 0 <= 12 AES Commands, 1>= 12 AES Commands
//	long		T0;								// 0 = Tev1, 1 = Tev2
//	long		T0_AES;							// Rel 2. {13} "TSR for TPE Type AES600 Release1" 0 = Tev1, 1 = Tev2
//	long		LP_Mode;						// 0 = State transmission, 1 = Signalling mode
//	long		Sec_Dep_Commands;				// 0 = low, 1= high (0- inceased Dependability, 1- increased Security)
//	long		Sec_Dep_Commands_AES;			// Rel. 2. {13} "TSR for TPE Type AES600 Release1" 0 = low, 1= high
//	long		ResetRxAlarm;					// Reset SNR and level alarm: 0 -> automatically 1 -> manually	
//	long		NSD_ETL_Guard;					// 0 = ETL-Pilot 1 = NSD own guard
//	long		CyclicLoopTestOn_Off;			// 0 = off 1 = on
//	long		PersistentCommand_On_Off;		// 0 = off, 1 = on
//	long		RxOutDurProlong_GroupA;			// 0 - Rx Output Prolongation is used, 1 - Rx Output Duration is used to configure Group A commands
//	long		RxOutDurProlong_GroupB;			// 0 - Rx Output Prolongation is used, 1 - Rx Output Duration is used to configure Group B commands
	
	long		HoldTime_Tx_Rx_Ack;				// in s
	float		AES_dummy_2;					//
	long 		OutputsAtAlarm;					// AES command outputs during Rx alarm // UNAFFECTED = 0,QUIESCENT = 3 (not used for AES: BLOCK_PERM_TRIPPED=1, FREZZED=2), 

	long		UnblockingPickupTime;			// in ms
	long		UnblockingPulsDuration;			// in ms
	float		UnblockingThreshold;			// UnblockingThreshold (-20..-10dB) 0.01 .. 0.1 linear

	float		PilotFrequency;					// frequency in Hz NSD own guard or ETL Pilot frequnecy 
	float		GuardLevel;						// lin. guard level (ETL pilot or AES own guard)
	float		PilotFrequency_RX;				// Rel 2. {27} "TSR for TPE Type AES600 Release1"

	long		TimeBetweenCL_Test;				// time in hours
	float		BoostLevel;						// configured boost level linear (not in dB!)
	float		maxBoostLevelinDB;				// Maximum allowed boostlevel in dB (not used in P4LT)


	long		StartInputActivTime;			// default 1 sec
	long		StartInputMonit_Duration;		// 
	long		OutputsAtRxAlarm;				// see define for OutputsAtRxAlarm below
	long		OutputsAtRxAlarmPickupTime;		// 
	long		OutputsAtRxAlarmHoldTime;		// 
	float		SNR_Alarm_Threshold;			// SNR alarm threshold (0...12dB) 1.0 ...15.848
	float		Guard_Alarm_Threshold;			// Guard alarm threshold (+/-3 ... +/- 12dB) 0.5 ... 0.063096
	long		UsedExtTimeSlots;				// BIT0 == 1 --> G4AI in SLOT_N11 used for AES etc.

	unsigned long	TANG_DelayOnOff;			// bits 0-15: TANG on delay [ms], bits 16-31: TANG off delay [ms]
	unsigned long	RxGuardOnOffDelay;			// bits 0-15: rx guard on delay [ms], bits 16-31: rx guard off delay [ms]
		
	// reserved for future extensions
	long		Reserved1;
	long		Reserved2;
	long		Reserved3;
	long		Reserved4;	

} t_AES_Config;	// used by HMI600


// Overall configuration data for the ETL600-internal teleprotection system

typedef struct {
	unsigned long			mode;					// constants INT_TELPROT_MODE_...
	union  
	{
		t_NSD_Config		NSD[NUM_OF_NSD_INST];	// NSD600-specific configuration data
		t_AES_Config		AES;					// AES600-specific configuration data
	} u_NSD_AES; 
} t_IntTeleprotConfig;	



//
// NSD developper status
typedef struct {

	unsigned long		FW_Version;					// 0x0000xyzv -> Version xy.zv
	unsigned long		CommonStatusCh1;			// Bit definition see below (display hex value)
	float				SpeechPower_tev1Ch1;		// Speech power for Tev1 Ch1
	float				SpeechPower_tev2Ch1;		// Speechband power for Tev2 Ch1
	float				PowerLargestDet_Ch1;		// Largest detector output power Ch1
	float				PowerSecondDet_Ch1;			// Second largest Detector output power Ch1
	long				ActivFreq_LargestCh1;		// Activ frequency for largest detector Ch1(display decimal value)
	long				ActivFreq_SecondCh1;		// Activ frequency for second largest detector Ch1(display decimal value)
	float				InputGuardSignalCh1;		// Rx Guard signal input Ch1
	float				GuardMeanLevelCh1;			// Rx Guard mean level Ch1
	long				G4AI_InputCh1;				// Input from G4AI Ch1 (display  hexvalue)
	long				G4AI_OutputCh1;				// Output to G4AI Ch1 (display hex value)
	unsigned long		DeveloppmentStatusBitsCh1;	// Development status bits Ch1(display hexvalue)
	unsigned long		G4AI_TDM_Bus_Failure_Counter_Ch1[NO_TDM_BASIC_TIMESLOTS];	// CRC Failure forom G4AI for Channel 2
	//
	unsigned long		CommonStatusCh2;			// Bit definition see below
	float				SpeechPower_tev1Ch2;		// Speech power for Tev1 Ch1
	float				SpeechPower_tev2Ch2;		// Speechband power for Tev2 Ch1
	float				PowerLargestDet_Ch2;		// Largest detector output power Ch1
	float				PowerSecondDet_Ch2;			// Second largest Detector output power Ch1
	long				ActivFreq_LargestCh2;		// Activ frequency for largest detector Ch1(display decimal value)
	long				ActivFreq_SecondCh2;		// Activ frequency for second largest detector Ch1(display decimal value)
	float				InputGuardSignalCh2;		// Rx Guard signal input Ch1
	float				GuardMeanLevelCh2;			// Rx Guard mean level Ch1
	long				G4AI_InputCh2;				// Input from G4AI Ch1 (display  hexvalue)
	long				G4AI_OutputCh2;				// Output to G4AI Ch1 (display hex value)
	unsigned long		DeveloppmentStatusBitsCh2;	// Development status bits Ch1(display hexvalue)
	unsigned long		G4AI_TDM_Bus_Failure_Counter_Ch2[NO_TDM_BASIC_TIMESLOTS];	// CRC Failure forom G4AI for Channel 2
	//
	float				f_Development_1;			// display float value
	float				f_Development_2;			// display float value
	float				f_Development_3;			// display float value
	float				f_Development_4;			// display float value
	float				f_Development_5;			// display float value
	float				f_Development_6;			// display float value
	long				l_Development_1;			// display hex and decimal value
	long				l_Development_2;			// display hex and decimal value
	long				l_Development_3;			// display hex and decimal value
	long				l_Development_4;			// display hex and decimal value
	long				l_Development_5;			// display hex and decimal value
	long				l_Development_6;			// display hex and decimal value

} t_NSD_DevelopperStatus_Type;
//
// defines for CommonStatus in DevelopperStatus struct
#define					RX_GUARD_DEVELOPMENT		0x00000001
#define					UNBLOCKING_DEVELOPMENT		0x00000002



/***************************************************************************************************
Macro:			CNT_TPE(IntTeleprotConfig, iCntTPE)
Description:	Counts the number of internal teleprotection equipments on the basis of the member
				variable "mode" of the struct t_IntTeleprotConfig.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.
				iCntTPE: Integer variable to store the number of internal teleprotection equipments.
Outputs:		iCntTPE: Integer variable containing the number of internal teleprotection
						 equipments.
Return:			None
Globals:		None	
Important:		-
***************************************************************************************************/
#define CNT_TPE(IntTeleprotConfig, iCntTPE) \
switch(IntTeleprotConfig.mode & INT_TPE_MODE_MASK) \
{ \
	case INT_TELPROT_MODE_AES: \
	case INT_TELPROT_MODE_NSD1_APLC1: \
	case INT_TELPROT_MODE_HSD: \
	case INT_TELPROT_MODE_HSD_25: \
	case INT_TELPROT_MODE_NSD1_DPLC:				iCntTPE = 1;	break; \
	case INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC:	\
	case INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2:	iCntTPE = 2;	break; \
	case INT_TELPROT_MODE_NONE: \
	default:										iCntTPE = 0;	break; \
} \



/***************************************************************************************************
Macro:			IS_NSD600(IntTeleprotConfig)
Description:	Determines whether NSD is configured as internal teleprotection system or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if NSD is configured, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD600(IntTeleprotConfig) \
	((((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD_25) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_DPLC) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2)) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD1_IN_APLC1(IntTeleprotConfig)
Description:	Determines whether NSD600 TPE1 is configured in APLC channel 1 or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro returns the value TRUE if NSD600 TPE1 is configured in APLC channel 1,
					  FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD1_IN_APLC1(IntTeleprotConfig) \
	((((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD_25) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2)) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD2_IN_APLC2(IntTeleprotConfig)
Description:	Determines whether NSD600 TPE2 is configured in APLC channel 2 or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro returns the value TRUE if NSD600 TPE2 is configured in APLC channel 2,
					  FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD2_IN_APLC2(IntTeleprotConfig) \
	 (((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD1_IN_DPLC(IntTeleprotConfig)
Description:	Determines whether NSD600 TPE1 is configured in DPLC channel or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro returns the value TRUE if NSD600 TPE1 is configured in DPLC channel,
					  FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD1_IN_DPLC(IntTeleprotConfig) \
	 (((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_DPLC) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD2_IN_DPLC(IntTeleprotConfig)
Description:	Determines whether NSD600 TPE2 is configured in DPLC channel or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro returns the value TRUE if NSD600 TPE2 is configured in DPLC channel,
					  FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD2_IN_DPLC(IntTeleprotConfig) \
	 (((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_HSD600(IntTeleprotConfig)
Description:	Determines whether HSD is configured as internal teleprotection system or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if HSD is configured, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_HSD600(IntTeleprotConfig) \
	(((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD) ? TRUE : FALSE) \


/***************************************************************************************************
Macro:			IS_HSD600_25(IntTeleprotConfig)
Description:	Determines whether HSD 2.5 kHz is configured as internal teleprotection system or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if HSD 2.5 kHz is configured, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_HSD600_25(IntTeleprotConfig) \
	(((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD_25) ? TRUE : FALSE) \


/***************************************************************************************************
Macro:			IS_AES600(IntTeleprotConfig)
Description:	Determines whether AES is configured as internal teleprotection system or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if AES is configured, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_AES600(IntTeleprotConfig) \
    (((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD600_EXTENDED(IntTeleprotConfig)
Description:	Determines whether NSDExtended is configured as internal teleprotection system or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if NSDExtended is configured, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD600_EXTENDED(IntTeleprotConfig) \
	(((IntTeleprotConfig.mode&((INT_TPE1_MODE_EXT_MASK|INT_TPE2_MODE_EXT_MASK))) > 0) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD600_EXTENDED_TPE1(IntTeleprotConfig)
Description:	Determines whether NSDExtended is configured as internal teleprotection system 
                inside TPE1 or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if NSDExtended in TPE1 is configured, 
				FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD600_EXTENDED_TPE1(IntTeleprotConfig) \
	(((IntTeleprotConfig.mode & INT_TPE1_MODE_EXT_MASK) > 0) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_NSD600_EXTENDED_TPE2(IntTeleprotConfig)
Description:	Determines whether NSDExtended is configured as internal teleprotection system 
                inside TPE2 or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if NSDExtended in TPE2 is configured, FALSE 
				if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_NSD600_EXTENDED_TPE2(IntTeleprotConfig) \
	(((IntTeleprotConfig.mode & INT_TPE2_MODE_EXT_MASK) > 0) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_TPE(IntTeleprotConfig)
Description:	Determines whether NSD or AES is configured as internal teleprotection system or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if NSD or AES is configured, FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_TPE(IntTeleprotConfig) \
	((IS_NSD600(IntTeleprotConfig) || IS_AES600(IntTeleprotConfig)) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_TPE1_IN_APLC1(IntTeleprotConfig)
Description:	Determines whether TPE1 (NSD600 or AES600) is configured in APLC channel 1 or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro returns the value TRUE if TPE1 is configured in APLC channel 1,
					  FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_TPE1_IN_APLC1(IntTeleprotConfig) \
	((((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_AES) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_HSD_25) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_DPLC) || \
	  ((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2)) ? TRUE : FALSE) \



/***************************************************************************************************
Macro:			IS_TPE2_IN_APLC2(IntTeleprotConfig)
Description:	Determines whether TPE2 (NSD600 or AES600) is configured in APLC channel 2 or not.
Inputs:			IntTeleprotConfig: Variable of type t_IntTeleprotConfig.				
Outputs:		None
Return:			BOOL: The macro reaches the value TRUE if TPE2 is configured in APLC channel 2,
					  FALSE if not.
Globals:		None	
Important:		-
***************************************************************************************************/
#define IS_TPE2_IN_APLC2(IntTeleprotConfig) \
	 (((IntTeleprotConfig.mode & INT_TPE_MODE_MASK) == INT_TELPROT_MODE_NSD1_APLC1_NSD2_APLC2) ? TRUE : FALSE) \



#define	BIT_0_SET	0x0001




#endif		// #ifndef NSD_TYPES_H


