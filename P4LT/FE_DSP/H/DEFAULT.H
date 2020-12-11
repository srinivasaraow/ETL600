/*-------------------------------------------------------------------------*
 * $Workfile: DEFAULT.H $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: 
 * Remarks	: Must run on P4LW and HMI600 
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/DEFAULT.H $
 * 
 * 221   16-02-08 8:22 Chthbae
 * New Version 4.15
 * 
 * 220   15-01-15 9:40 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.14"
 * 
 * 219   14-02-05 11:31 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.13"
 * 
 * 218   28.10.13 15:05 Chmaste
 * Command output link failure - Hold time default value changed from 0 to
 * 1 for AES
 * 
 * 217   13-09-24 16:56 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.12"
 * 
 * 216   13-03-25 9:41 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.11"
 * 
 * 215   13-02-20 16:53 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.11 RC1"
 * 
 * 214   13-02-11 15:23 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.10"
 * 
 * 213   30.01.13 13:04 Chmaste
 * Changed default HSD boost ratio from 12 to 7
 * 
 * 212   12-10-09 15:53 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.10 RC1"
 * 
 * 211   21.09.12 8:38 Chmaste
 * 2.5 kHz NSD teleprotection system
 * 
 * 210   12-04-03 9:59 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00"
 * 
 * 209   12-03-23 15:08 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC14"
 * 
 * 208   14.03.12 8:58 Chmaste
 * Missing function return types fixed
 * 
 * 207   12-03-07 11:52 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC13"
 * 
 * 206   12-02-27 14:21 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC12"
 * 
 * 205   12-02-01 14:38 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC11"
 * 
 * 204   25.01.12 17:23 Chthbae
 * AF Channel sharing disabled at baudrates from 2400 or higher
 * 
 * 203   11-12-08 13:55 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC10"
 * 
 * 202   11-10-05 18:18 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC9"
 * 
 * 201   11-09-12 11:55 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC8"
 * 
 * 199   11-08-22 14:18 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC7"
 * 
 * 198   11-08-17 13:53 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC6"
 * 
 * 197   11-07-13 10:02 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC5"
 * 
 * 196   24.06.11 9:37 Chthbae
 * 
 * 195   11-06-16 16:14 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC4"
 * 
 * 194   11-05-27 17:42 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC3"
 * 
 * 193   11-05-09 18:38 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC2"
 * 
 * 192   11-05-02 11:54 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 RC1"
 * 
 * 191   11-03-24 14:37 Chhamaa
 * DEFAULT_PEP_VALUE set to 47 [dBm].
 * 
 * 190   11-03-24 14:31 Chhamaa
 * DEFAULT_SERVICES_RF_PEP set to 50.
 * 
 * 189   11-01-13 17:42 Chhamaa
 * For release 4.0:
 * - P4LT_CONFIGURATION_VERSION set to 6
 * - MMI_COMPATIBILITY_VERSION_OF_P4LT set to 1800
 * 
 * 188   10-12-01 15:09 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 04.00 Test_05"
 * 
 * 187   10-10-28 11:28 Chhamaa
 * DEFAULT_EQUIPMENT_TYPE added.
 * 
 * 184   12.12.08 10:53 Chhamaa
 * P4LT_CONFIGURATION_VERSION increased (set to 4)
 * 
 * 183   27.11.08 16:39 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20"
 * 
 * 182   18.11.08 14:29 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20 RC5"
 * 
 * 181   17.11.08 13:47 Chsanoh
 * - Default boost ratio AES new 6 dB instead of auto-select max.
 * - APLC 2 kHz und DPLC 2 kHz (ModAboveSpeech). With AES in APLC, pilot =
 * 2220 Hz; with NSD in APLC or no TPE in APLC, pilot = 2160 Hz.
 * - Signaling rate (normal /slow)
 * - Config view adaptations
 * - Cosmetics
 * 
 * 180   24.10.08 17:41 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20 RC4"
 * 
 * 179   13.10.08 15:09 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20 RC3"
 * 
 * 178   7.10.08 18:34 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20 RC2"
 * 
 * 177   26.09.08 11:02 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20 RC1"
 * 
 * 176   11.09.08 10:23 Chsanoh
 * Bug 451 fixed.
 * 
 * 175   10.09.08 9:12 Chsanoh
 * ETL pilot as AES or NSD guard fix at 2220 kHz for 2 kHz APLC and 2 kHz
 * DPLC channel allocation.
 * ETL pilot as AES guard lowest at 2220 kHz for 12 AES commands and any
 * channel allocation.
 * ETL pilot as AES guard lowest at 2460 kHz for 20 AES commands and any
 * channel allocation.
 * 
 * 174   24.07.08 15:54 Chhamaa
 * FW_VERSION_NAME set  to "ETL600 Version 03.20 Test04"
 * 
 * 173   25.06.08 11:13 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.20"
 * MMI_COMPATIBILITY_VERSION_OF_P4LT  set to 1600
 * 
 * 172   6.06.08 15:54 Chsanoh
 * 3rd MAS implementation phase
 * 
 * 171   9.05.08 17:25 Chsanoh
 * First MAS implementation phase
 * 
 * 169   08-04-25 15:51 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 7.1"
 * 
 * 168   08-04-24 9:19 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 7.0
 * 
 * 167   08-04-11 13:58 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.9"
 * 
 * 166   08-04-08 15:39 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.8"
 * 
 * 165   08-04-04 16:26 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.7"
 * 
 * 164   08-04-03 9:49 Chtognu
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.6"
 * 
 * 163   08-03-20 14:29 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.5
 * 
 * 162   08-03-18 14:43 Chtognu
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.4"
 * 
 * 161   08-03-07 17:00 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.3"
 * 
 * 160   08-03-06 17:01 Chsteic
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.2"
 * 
 * 159   08-02-29 13:19 Chtognu
 * FW_VERSION_NAME set to "ETL600 Version 03.10 adEQ TestV 6.1"
 * 
 * 158   08-02-25 10:55 Chtognu
 *  FW version and compatibilty version changed
 * 
 * 157   22.02.08 13:05 Chsanoh
 * All adaptive equalizer features added from branch "Source AdaptEqu"
 * labeled "adEQ TestV 5.0".
 * 
 * 155   20.02.08 10:23 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.05"
 * 
 * 154   1.02.08 9:59 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.03"
 * 
 * 153   4.12.07 10:04 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.02"
 * 
 * 152   2.11.07 16:30 Chhamaa
 * FW_VERSION_NAME set to "ETL600 Version 03.01"
 * 
 * 151   5.09.07 13:56 Chrearp
 * Added default values for the t_NSK_EqualizerCfg struct.
 * 
 * 150   23.07.07 16:51 Chtognu
 * NSK adaptive Equalizer struct added:
 * InitDefaultConfigP4LTNskEqualizer()
 * 
 * 149   07-07-08 16:04 Chmaflu
 * Modifications for added SNMP destination trap port
 * 
 * 148   07-07-04 9:36 Chmaflu
 * "move InitDefaultConfigO4CV(..) to c/cplusplus section
 * change MMI_COMPATIBILITY_VERSION_OF_P4LT to 1400"
 * 
 * 147   27.02.07 16:53 Chsanoh
 * Default TANG reset time increased from 10 to 50 ms.
 * 
 * 146   30.01.07 17:22 Chhamaa
 * InitDefaultConfigO4CV() added.
 * 
 * 145   22.01.07 18:26 Chmaflu
 * "Change one parameter Type of the function 
 * getTableEntryFromCustomTableFile(..) because of crash in HMI Release
 * build"
 * 
 * 144   19.01.07 15:32 Chhamaa
 * FW_VERSION_NAME and P4LT_CONFIGURATION_VERSION increased
 * 
 * 142   14.12.06 11:50 Chmaflu
 * "NSD extended modification:
 * Add ifdef __cplusplus to compile used functions successful under the
 * DSP and under the Visual C++ environment 
 * Add Defines for Teleprotection defaults 
 * Add another parameter t_NSDCustomTableFile struct to function
 * InitDefaultConfigNSD600(..) 
 * Change function  InitDefaultConfigNSD600(..) to add a customtable in
 * the case of NSD Extended is configured
 * Set different defaults for NSD and NSD Extended 
 * Add function getTableEntryFromCustomTableFile(..) which finds the right
 * customtable
 * for a custom modeId"
 * 
 * 141   4.10.06 9:53 Chsanoh
 * Values for defines DEFAULT_AES_LP_CMD_TX_INP_COM_DELAY_SIGNALING 
 * and DEFAULT_AES_LP_CMD_TX_INP_COM_DELAY_STEPPING interchanged.
 * 
 * 140   19.09.06 15:50 Chsanoh
 * Bug concerning TANG window default values changed.
 * 
 * 139   14.09.06 15:15 Chsanoh
 * TANG window and Rx guard signaling implemented in corresponding masks.
 * 
 * 138   13.09.06 17:01 Chsanoh
 * Bug 373 corrected.
 * 
 * 137   8.09.06 16:56 Chsanoh
 * - NSD, HSD, and AES configuration masks harmonized.
 * - Default values, read and write operations for AES group A and group B
 * tabs adapted.
 * Changes according to input from M. Strittmatter from 04.09.06.
 * 
 * 136   28.08.06 10:11 Chsanoh
 * Bug 375 corrected.
 * 
 * 135   25.08.06 19:15 Chchlee
 * DEFAULT_EQUIPMENT_ADDRESS = 1
 * 
 * 134   25.08.06 11:23 Chsanoh
 * Classes
 * - CAES600GroupB
 * - CAES600Interface
 * - CAES600Interface0
 * - CAES600Mode
 * - CAES600System
 * - CAES600Unblock
 * revised.
 * 
 * 133   16.08.06 14:57 Chhamaa
 * FW version and compatibilty version changed:
 * FW_VERSION_NAME   "ETL600 Version 02.00"
 * MMI_COMPATIBILITY_VERSION_OF_P4LT  1300
 * 
 * 131   21.07.06 13:37 Chsanoh
 * - Bug 324 corrected.
 * - P4LT firmware version increased to "ETL600 Version 01.42".
 * 
 * 130   28.04.06 13:36 Chsanoh
 * Allpass filter for group delay equalization activated as default for
 * all APLC channels.
 * 
 * 128   7.04.06 16:25 Chrobif
 * Note regarding use of InitDefaultConfigR1LA added.
 * 
 * 127   7.04.06 14:46 Chrobif
 * InitDefaultConfigR1LA added
 * 
 * 126   5.04.06 10:09 Chsanoh
 * Auto-select standard filter introduced for Tx RF and Rx RF filter
 * tuning.
 * 
 * 125   23.03.06 8:49 Chpeplu
 * define MMI_COMPATIBILITY_VERSION_OF_P4LT  1200 changed
 * 
 * 124   21.03.06 11:16 Chsanoh
 * '<Configuration> <NSD600...>, application of commands:
 * List box index does no longer correspond to command value written to
 * config. Newly, selected string item will be mapped to corresponding
 * command value.
 * 
 * 123   21.03.06 10:45 Chpeplu
 * defines changed:
 * FW_VERSION_NAME   "ETL600 Version 01.40"
 * FW_COMPATIBILITY_INDEX	    104
 * MMI_COMPATIBILITY_VERSION_OF_P4LT  	1101
 * 
 * 122   28.02.06 17:19 Chsanoh
 * Default value for "Timed Tx Power Reduction" newly amounts to 10s.
 * 
 * 121   24.02.06 16:14 Chsanoh
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
 * 120   14.02.06 17:38 Chsanoh
 * Bug 292 corrected.
 * 
 * 119   3.02.06 10:17 Chsanoh
 * HSD basic configuration, HSD custom table, HSD pilot frequency and
 * allpass filter, HSD speech filter (cut off frequency) for service phone
 * implemented.
 * 
 * 117   14.12.05 8:57 Chdocac
 * 
 * 116   3.11.05 17:02 Chchlee
 * P4LW FW Version 01.28
 * 
 * 115   31.10.05 8:48 Chdocac
 * 
 * 114   28.10.05 8:54 Chdocac
 * 
 * 113   26.10.05 15:39 Chdocac
 * 
 * 112   19.10.05 7:38 Chchlee
 * 
 * 111   18.10.05 9:45 Chchlee
 * 
 * 110   12.10.05 15:14 Chchlee
 * 
 * 109   6.10.05 8:35 Chchlee
 * Version 1.22
 * 
 * 108   5.10.05 12:47 Chchlee
 * 
 * 107   4.10.05 14:54 Chchlee
 * 
 * 106   3.10.05 18:38 Chmibuh
 * FW_VERSION_NAME_EMERGENCY added
 * 
 * 105   30.09.05 11:31 Chdocac
 * 
 * 104   29.09.05 7:14 Chchlee
 * 
 * 103   28.09.05 8:11 Chchlee
 * ETL600 Version 01.20+
 * 
 * 102   26.09.05 17:35 Chchlee
 * ETL600 Version 01.20
 * 
 * 101   26.09.05 13:38 Chchlee
 * ETL600 Version 01.19.1
 * 
 * 100   21.09.05 9:06 Chchlee
 * P4LW Firmware Version 1.19
 * 
 * 99    12.09.05 17:41 Chsanoh
 * Default alarms for R1BC changed. Default jumper settings for G4AI
 * changed.
 * 
 * 98    7.09.05 11:49 Chchlee
 * FW_VERSION_NAME updated
 * 
 * 97    5.09.05 17:25 Chchlee
 * Version updated to 1.18
 * 
 * 96    29.08.05 11:19 Chsanoh
 * DEFAULT_TX_NOMINAL_FREQUENCY and DEFAULT_RX_NOMINAL_FREQUENCY changed.
 * DEFAULT_POWER_SUPPLY_VOLTAGE changed.
 * 
 * 95    29.08.05 11:05 Chsanoh
 * DEFAULT_OFDM_WEIGHT changed.
 * 
 * 94    17.08.05 9:08 Chchlee
 * DEFAULT_OFDM_WEIGHT changed to high
 * 
 * 93    12.08.05 14:43 Chdocac
 * 
 * 92    10.08.05 15:19 Chchlee
 * DEFAULT_OFDM_MODE modified: RS and NB are now off by default
 * 
 *-------------------------------------------------------------------------*/

#ifndef 	_DEFAULT
#define		_DEFAULT

#include "RF_Types.h"
#include "APLC_Types.h"
#include "NSD_Types.h"
#include "NSK_Types.h"
#include "P4LT_Types.h"
#include "R1LA_Types.h"
#include "AES600.h"
#include "math.h"

#define FW_VERSION_NAME					"ETL600 Version 04.15" // must be <= 40 characters long!
//
// Change number of FW_COMPATIBILITY_INDEX if functionality changes
// This number will be transmitted over the eoc channel and checked in the remote station.
// If the numbers are different the warning "inconsistent FW version on local and remote" shows. 
//
#define	FW_COMPATIBILITY_INDEX			105

// increment after each modification of the P4LT configuration structure (t_P4LT_Config)
#define	P4LT_CONFIGURATION_VERSION		7

#define MMI_COMPATIBILITY_VERSION_OF_P4LT  1900
//
#define DEFAULT_TESTCONFIGURATION_ACTIVE_TIME_IN_MINUTES	30	

#define DEFAULT_S_VALUE_SINGLE_CHANNEL	35
#define DEFAULT_S_VALUE_TWO_CHANNEL		7.0

// Default settings in config and Eprom structures
#define DEFAULT_RX_NOMINAL_FREQUENCY				100000
#define DEFAULT_TX_NOMINAL_FREQUENCY				132000
#define DEFAULT_HF_TX_PEGELKORREKTUR				0.50118723		// -6dB 
#define DEFAULT_HF_SUMME_S_WERTE_INVERS	  			(1.0 / DEFAULT_S_VALUE_SINGLE_CHANNEL)	
#define DEFAULT_KANAL_LAGE							CH1_R
#define DEFAULT_HF_BANDBREITE						4000	
#define	DEFAULT_PEP_VALUE							47
#define DEFAULT_PILOT_FREQUENCY						3840
#define DEFAULT_PILOT_FREQUENCY_8KHZ				7860
#define DEFAULT_PILOT_FREQUENCY_00					0		// Default pilot frequency for APLC channel of 0 kHz
#define DEFAULT_PILOT_FREQUENCY_20					1800	// Default pilot frequency for APLC channel of 2 kHz
#define DEFAULT_PILOT_FREQUENCY_20_APLC_20_DPLC		2160	// Default pilot frequency for ModAboveSpeech APLC channel of 2 kHz and DPLC channel of 2 kHz
															// with NSD or no TPE in APLC channel
#define DEFAULT_PILOT_FREQUENCY_20_APLC_20_DPLC_AES	2220	// Default pilot frequency for ModAboveSpeech APLC channel of 2 kHz and DPLC channel of 2 kHz
															// with AES in APLC channel
#define DEFAULT_PILOT_FREQUENCY_20_APLC_60_DPLC		2580	// Default pilot frequency for ModAboveSpeech APLC channel of 2 kHz and DPLC channel of 6 kHz
#define DEFAULT_PILOT_FREQUENCY_24					2280	// Default pilot frequency for APLC channel of 2.4 kHz
#define DEFAULT_PILOT_FREQUENCY_25                  2240    // Default pilot frequency for APLC channel of 2.5 kHz
#define DEFAULT_PILOT_FREQUENCY_32					3120	// Default pilot frequency for APLC channel of 3.2 kHz
#define DEFAULT_PILOT_FREQUENCY_40					3840	// Default pilot frequency for APLC channel of 4 kHz
#define DEFAULT_PILOT_FREQUENCY_DPLC				3840	// Default pilot frequency for DPLC channel of any 
															// bandwidth > 0 Hz
#define DEFAULT_TX_RX_BAND_ADJACENT_GAP		  (long)4000
#define DEFAULT_TX_RX_BAND_ADJACENT_GAP_2_5	  (long)3000

// DEFAULT_PILOT_LEVEL_LOW wird nur bei einem uninitialisiertem EPROM benötigt.
#define DEFAULT_PILOT_LEVEL_LOW					 	4.0e-4		// min. pilot level (-68 dB)  
#define DEFAULT_SNR_ALARM_THRESHOLD_IN_DB  			12
#define DEFAULT_SNR_ALARM_THRESHOLD_LIN				15.84893

#define DEFAULT_LEVEL_ALARM_TRESHOLD_IN_DB 			-70			// Alarm level threshold (pilot)
#define DEFAULT_LEVEL_ALARM_TRESHOLD_LIN			50e-9		// half value because only i or q channel is eval.  

#define DEFAULT_PILOT_LEVEL_ALARM_TRESHOLD_IN_DB 	-76        // AGC blocking level
#define DEFAULT_PILOT_LEVEL_ALARM_TRESHOLD_LIN		158.489e-6

// default testtone settings
#define DEFAULT_TESTTONE_FREQUENCY_IN_HZ			800
#define DEFAULT_TESTTONE_LEVEL_LIN					1.0F		// linear scale	
#define DEFAULT_TESTTONE_LEVEL_IN_DB				0.0F		// log scale

#define DEFAULT_RXAF_LEVEL_ADJUST_CH1				1.0			// Pegel wird nicht veraendert
#define DEFAULT_RXAF_LEVEL_ADJUST_CH2				1.0 

// Range for AdjustRXAFlevel
#define MIN_RXAF_LEVEL_ADJUST_IN_DB					-3.1F
#define MAX_RXAF_LEVEL_ADJUST_IN_DB					+3.1F

// Default for member variables of struct t_SystemConfig

#define DEFAULT_EQUIPMENT_ADDRESS					1
#define DEFAULT_MODIFICATION_COUNTER				0
#define DEFAULT_EQUIPMENT_TYPE						EQ_TYPE_ETL600_R4_4Slots

// Default for member variables of struct t_RF_Config

#define DEFAULT_RF_CHANNEL_MODE		RF_CHANNEL_MODE_ERECT
#define DEFAULT_RF_BANDWIDTH		(long) 4000

// Default for member variables of struct t_APLC_Config

#define DEFAULT_APLC_CHANNEL_MODE	APLC_CHANNEL_MODE_A1_40I

// Allpass filter for group delay equalization is set to disabled as default
// for all APLC channels with ETL600 Rel. 3.01.x.
#define	DEFAULT_AP_FILTER_CH1		AP_FILTER_CH1_OFF
#define	DEFAULT_AP_FILTER_CH2		AP_FILTER_CH2_OFF
#define	DEFAULT_AP_FILTER_CH3		AP_FILTER_CH3_OFF

// Default for member variables of struct t_OFDM_Config

#define DEFAULT_OFDM_CHANNEL_BW		(long) 0

// defaults for OFDM.mode: master,NB disable, RS disable, !! test mode 3 !! ,DIN_EDGE pos,DOUT_EDGE pos,
//							V11 mode,timeout low,N(orthogonality)=64,L(guard)=8, 
//							fallback/fallforward enabled
//                          MOD600 weight = high

#define DEFAULT_OFDM_MODE			(  OFDM_CFG_MODE_MASTER_BIT     |  /* OFDM_CFG_MODE_NB_ENABLE_BIT | */\
									 /*OFDM_CFG_MODE_RS_ENABLE_BIT  |*/   OFDM_CFG_MODE_DATARATE_BITS | \
									   OFDM_CFG_MODE_TESTMODE_SELECT_BITS | \
									   OFDM_CFG_MODE_ON_OFF_BIT |\
									   OFDM_CFG_MODE_WEIGHT_BIT1) // corresponds to MOD600 weight = high 


#define DEFAULT_OFDM_WEIGHT			 0.0F // Effective MOD600 weight will be calculated
										  // within function void CComputeSvalue::SetMOD600_Weight() 



// Default OFDM data rates
#define DEFAULT_OFDM_DATARATE_0			19200
#define DEFAULT_OFDM_DATARATE_1			16000
#define DEFAULT_OFDM_DATARATE_2			14400
#define DEFAULT_OFDM_DATARATE_3			12000
#define DEFAULT_OFDM_DATARATE_4			9600

// Default OFDM data rates for 2kHz DPLC
#define DEFAULT_OFDM_DATARATE_2KHZ_0	11000
#define DEFAULT_OFDM_DATARATE_2KHZ_1	11000
#define DEFAULT_OFDM_DATARATE_2KHZ_2	11000
#define DEFAULT_OFDM_DATARATE_2KHZ_3	11000
#define DEFAULT_OFDM_DATARATE_2KHZ_4	11000

// Default for member variables of struct t_IntTeleprotConfig

#define DEFAULT_INT_TELEPROT_MODE	INT_TELPROT_MODE_NONE // long mode


// Default for member variables of struct t_NSD_Config

#define DEFAULT_NSD_MODE	INT_TELPROT_MODE_NONE // long mode

#define DEFAULT_NSD_TX_INP_COM_MONIT_DURATION		5
#define DEFAULT_NSD_TX_INP_COM_MONIT_DURATION_OFF	0
#define DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION		5
#define DEFAULT_NSD_RX_OUT_COM_MONIT_DURATION_OFF	0
#define DEFAULT_NSD_RX_OUT_DURATION_IF_ON			100 
#define DEFAULT_NSD_RX_OUT_DURATION_IF_OFF			0

// Rx Output Command Prolongation default depends on application 
//	--> blocking 0 ms, permissive 10 ms, direct 100 ms
#define DEFAULT_NSD_RX_OUTPUT_PROLONGATION_OFF			0
#define DEFAULT_NSD_RX_OUTPUT_PROLONGATION_BLOCKING		0	
#define DEFAULT_NSD_RX_OUTPUT_PROLONGATION_PERMISSIVE	10	
#define DEFAULT_NSD_RX_OUTPUT_PROLONGATION_DIRECT		100

#define DEFAULT_NSD_RX_OUTPUT_COM_DELAY_OFF				0
#define DEFAULT_NSD_RX_OUTPUT_COM_DELAY_BLOCKING		0
#define DEFAULT_NSD_RX_OUTPUT_COM_DELAY_PERMISSIVE		0
#define DEFAULT_NSD_RX_OUTPUT_COM_DELAY_DIRECT			0



#define DEFAULT_NSD_TX_INPUT_COM_DELAY_OFF				0
#define DEFAULT_NSD_TX_INPUT_COM_DELAY_BLOCKING		    0	
#define DEFAULT_NSD_TX_INPUT_COM_DELAY_PERMISSIVE	    0	
#define DEFAULT_NSD_TX_INPUT_COM_DELAY_DIRECT		    0


#define DEFAULT_NSD_TX_INPUT_COM_DURATION_OFF				0
#define DEFAULT_NSD_TX_INPUT_COM_DURATION_BLOCKING		    0	
#define DEFAULT_NSD_TX_INPUT_COM_DURATION_PERMISSIVE	    0	
#define DEFAULT_NSD_TX_INPUT_COM_DURATION_DIRECT		    0

	
#define DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_OFF				0
#define DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_BLOCKING		    0	
#define DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_PERMISSIVE	    0	
#define DEFAULT_NSD_TX_INPUT_COM_PROLONGATION_DIRECT		    0


#define DEFAULT_NSD_UNBLOCKING_PICKUP_TIME			20
#define DEFAULT_NSD_UNBLOCKING_PULS_DURATION		200		
#define DEFAULT_NSD_UNBLOCKING_THRESHOLD			(float)0.03981072	//-14 dB

#define DEFAULT_NSD_ETL_GUARD						0	// default is ETL Guard
#define DEFAULT_NSD_CYCLIC_LOOP_TEST_ON				0	// default is off
#define DEFAULT_NSD_CYCLIC_LOOP_TEST_TIME			6

#define DEFAULT_NSD_GUARD_ALARM_THRES				(float)0.1	//- 10 dB

#define DEFAULT_NSD_ALARM_HOLD_TIME					1
#define DEFAULT_NSD_ALARM_PICKUP_TIME				10

#define DEFAULT_NSD_OUT_FAILURE_HOLD				0
#define DEFAULT_NSD_OUT_FAILURE_PICKUP_TIME			10

#define DEFAULT_NSD_START_INPUT_ACTIVE_TIME			1
#define DEFAULT_NSD_TIMED_TX_POWER_REDUCTION_TIME	10

#define DEFAULT_HSD_BOOST_RATIO						(float)2.2387211385683396119549508524657	// linear value (10 ^ (7 dB / 20))

#define DEFAULT_TANG_SET_TIME						(unsigned long)200
#define DEFAULT_TANG_RESET_TIME						(unsigned long)50

#define DEFAULT_RX_GUARD_SIGNALING_EXTRA_OFF_DELAY	(unsigned long)5000
#define DEFAULT_RX_GUARD_SIGNALING_EXTRA_ON_DELAY	(unsigned long)10

// Default for member variables of struct t_AES_Config

#define DEFAULT_AES_OUT_FAILURE_HOLD				1
#define DEFAULT_AES_OUT_FAILURE_PICKUP_TIME			10

// Line protection commands
#define DEFAULT_AES_LP_CMD_TX_INP_COM_DELAY_SIGNALING 5
#define DEFAULT_AES_LP_CMD_TX_INP_COM_DELAY_STEPPING  0
#define DEFAULT_AES_LP_CMD_TX_INP_COM_PROL			  0
#define DEFAULT_AES_LP_CMD_TX_INP_COM_MONIT_DUR		  5
#define DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T01_ID	  0	// ID = Increased Dependability; IS = Increased Security
#define DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T01_IS	 10
#define DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T02_ID	100
#define DEFAULT_AES_LP_CMD_RX_OUT_COM_PROL_T02_IS	100
#define DEFAULT_AES_LP_CMD_RX_OUT_COM_DUR			100

// Persistent command
#define DEFAULT_AES_P_CMD_TX_INP_COM_DELAY			  0
#define DEFAULT_AES_P_CMD_TX_INP_COM_PROL			  0
#define DEFAULT_AES_P_CMD_TX_INP_COM_MONIT_DUR		  0	
#define DEFAULT_AES_P_CMD_RX_OUT_COM_PROL		     50
#define DEFAULT_AES_P_CMD_RX_OUT_COM_DUR		      0

// AES commands
#define DEFAULT_AES_AES_CMD_TX_INP_COM_DELAY		  5
#define DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T01_ID	  0	// ID = Increased Dependability; IS = Increased Security
#define DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T01_IS	 10
#define DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T02_ID	100
#define DEFAULT_AES_AES_CMD_RX_OUT_COM_PROL_T02_IS	100
#define DEFAULT_AES_AES_CMD_RX_OUT_COM_DUR		    100

// Default for member variables of struct t_NSK_Config


// Default for member variables of struct t_ModemConfig
// Defaults for NSK600 DEFAULT_MODEM_MODE:
// async,8 data bits, 1 stop bit, P2Multipe disabled,rx clk recovery disabled, 
//	RTS always active: disabled, ch alloc: 0 
#define DEFAULT_MODEM_MODE				(V24PORT_CFG_MODE_DATA_BITS_BIT3 | V24PORT_CFG_MODE_STOP_BITS_BIT0)
#define DEFAULT_NSK600_BITRATE			2400 //see NSK CONFIG ARRAY
#define DEFAULT_NSK600_BW				840  //see NSK CONFIG ARRAY
#define DEFAULT_NSK600_TX_CFREQ			900  //see NSK CONFIG_ARRAY		
#define DEFAULT_NSK600_RX_CFREQ			3600 //see NSK CONFIG_ARRAY
#define MAX_AF_CHANNEL_SHARING_RATE		2400
#define DEFAULT_NSK600_TX_DATA_DELAY	0

//Switch back to this datarate if configuration is wrong
#define LOWEST_DAPSK_DATARATE		  2400

//Default DataRate for MOD600
#define DEFAULT_MOD600_BITRATE		  2400

//Default control for V24 ports
#define DEFAULT_V24PORT_MODE		   (V24PORT_CFG_MODE_DATA_BITS_BIT3 | V24PORT_CFG_MODE_STOP_BITS_BIT0)



// Default for member variables of struct t_ServicesData

#define DEFAULT_SERVICES_TYPE		SERVICES_TYPE_ETL // long type 
#define DEFAULT_SERVICES_RF_PEP		(long) 50         // long rfPEP

// Default for member variables of struct t_HWSettings

#define DEFAULT_POWER_SUPPLY_VOLTAGE	VDC_48						// long powerSupplyVoltage
#define DEFAULT_TX_COIL1_POS			TX_COIL1_NEUTRAL_POS		// unsigned long txFilterBandwidthAndCoilEndStop
#define DEFAULT_TX_COIL2_POS			TX_COIL2_NEUTRAL_POS		// unsigned long txFilterBandwidthAndCoilEndStop
#define DEFAULT_TX_FILTER_SEL			TX_AUTO_SEL_STAND_FILTER	// unsigned long txFilterBandwidthAndCoilEndStop
#define DEFAULT_RX_FILTER_SEL			RX_AUTO_SEL_STAND_FILTER	// unsigned long rxFilterBandwidth

#define DEFAULT_R1BC_MODE_RELAY1		R1BC_MODE_RELAY1_ALARM1 // unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY2		R1BC_MODE_RELAY2_ALARM2	// unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY3		R1BC_MODE_RELAY3_ALARM3	// unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY4		R1BC_MODE_RELAY4_ALARM1	// unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY5		R1BC_MODE_RELAY5_ALARM1	// unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY6		R1BC_MODE_RELAY6_ALARM2	// unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY7		R1BC_MODE_RELAY7_ALARM3	// unsigned long R1BCMode
#define DEFAULT_R1BC_MODE_RELAY8		R1BC_MODE_RELAY8_ALARM1	// unsigned long R1BCMode

// Default for member variables of struct t_SystemConfig

#define DEFAULT_HMI_BAUDRATE		57600	// 	unsigned long HMIBaudrate
#define DEFAULT_LINE_IMPEDANCE		SYSTEM_SETTINGS_LINE_IMP_45_OHM

// Default for member variables of struct BasicFunctionData

#define DEFAULT_CHANNEL_BANDWIDTH	BW4000 // long ChannelBandWidth

// Default for member variables of struct t_NSK_EqualizerCfg

#define DEFAULT_NSK_EQUALIZER_MODE	AD_EQ_MODE1_ON
#define DEFAULT_NSK_EQUALIZER_SETTINGS	657934
#define DEFAULT_NSK_EQUALIZER_OPERATING_RANGE	19660950


#define SNMP_TRAP_DESTINATION_PORT_DEFAULT		162
							


void InitDefaultConfigP4LTSystem(t_P4LT_Config* pP4LTConfig);
void InitDefaultConfigP4LTAplc(t_P4LT_Config* pP4LTConfig);
void InitDefaultConfigP4LTOfdm(t_P4LT_Config* pP4LTConfig);
void InitDefaultConfigP4LTNsk(t_P4LT_Config* pP4LTConfig);
void InitDefaultConfigP4LTMux(t_P4LT_Config* pP4LTConfig);
void InitDefaultConfigITS(t_P4LT_Config* pP4LT_Config);
void InitDefaultRF_FilterSettings(t_P4LT_Config* pP4LTConfig, BOOL* pbCovering);
void InitDefaultConfigAES600(t_IntTeleprotConfig* pConfig);
void InitDefaultConfigR1LA(t_R1LA_Config* pR1LA_Config);
//void InitDefaultConfigO4CV(t_P4LT_Config* pP4LTConfig);

// Note: The function InitDefaultConfigR1LA utilizes data types which are not 32 bits long
//		 (Example: unsigned char or short int). Do not use this function if your HW does not support this!


#ifdef __cplusplus
	extern "C" t_NSDExtCustomTableFileEntry getTableEntryFromCustomTableFile(unsigned long id,t_NSDExtCustomTableFile* tblFile);
	extern "C" void InitDefaultConfigNSD600(t_IntTeleprotConfig* pConfig,t_NSDExtCustomTableFile* nsdExtCustomTableFile);
	extern "C" void InitDefaultsNSD600_CmdsApplication(t_IntTeleprotConfig* pConfig);
	extern "C" void InitDefaultsNSD600_CmdApplication(t_IntTeleprotConfig* pConfig, int iTPE_inst, int iCmd);
	extern "C" void InitDefaultConfigO4CV(t_P4LT_Config* pP4LTConfig);
	extern "C" void InitDefaultConfigP4LTNskEqualizer(t_P4LT_Config* pP4LTConfig);
#else
	t_NSDExtCustomTableFileEntry getTableEntryFromCustomTableFile(unsigned long id,t_NSDExtCustomTableFile* tblFile);
	void InitDefaultConfigNSD600(t_IntTeleprotConfig* pConfig,t_NSDExtCustomTableFile* nsdExtCustomTableFile);
	void InitDefaultsNSD600_CmdsApplication(t_IntTeleprotConfig* pConfig);
	void InitDefaultsNSD600_CmdApplication(t_IntTeleprotConfig* pConfig, int iTPE_inst, int iCmd);
	void InitDefaultConfigO4CV(t_P4LT_Config* pP4LTConfig);
	void InitDefaultConfigP4LTNskEqualizer(t_P4LT_Config* pP4LTConfig);
#endif

/***************************************************************************************************
Macro:			MY_MAX(a, b) 
Description:	Returns the bigger of two values.
                Could not use __max() because it was not available on both platforms
Inputs:			a: 1st value
				b: 2nd value							
Outputs:		None
Return:			The bigger value of a and b.
Globals:		None	
Important:		-
***************************************************************************************************/
#define MY_MAX(a, b)  (((a) > (b)) ? (a) : (b))

/***************************************************************************************************
Macro:			SIGNUM(number)
Description:	Returns the sign of a value.
Inputs:			number: Value to get the sign from.								
Outputs:		None
Return:			-1 if passed value is negative, 1 if passed value is zero or positive.
Globals:		None	
Important:		-
***************************************************************************************************/
#define SIGNUM(number) (number >= 0 ? 1: -1)

#endif 		// _DEFAULT

