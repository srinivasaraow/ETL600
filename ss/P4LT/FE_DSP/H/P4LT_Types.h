/*-------------------------------------------------------------------------*
 * $Workfile: P4LT_Types.h $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : P4LT global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/P4LT_Types.h $
 * 
 * 179   19-05-23 9:21 Chthbae
 * Changes for final G4AR board
 * 
 * 178   21.09.12 8:38 Chmaste
 * 2.5 kHz NSD teleprotection system
 * 
 * 177   22.03.12 13:37 Chthbae
 * Filter constant changed
 * 
 * 176   1.03.12 12:11 Chthbae
 * Default settings for RF filters changed for R4
 * 
 * 175   13.12.11 10:11 Chthbae
 * Racktype added for mapping the racktype in Alarmpolling
 * 
 * 174   11-12-08 13:53 Chhamaa
 * ETL600 equipment type added to the alarmWords (used for alarm polling).
 * 
 * 173   11-09-07 13:51 Chhamaa
 * OP_MODE_COMMISSIONING_RF_TERMINATED added.
 * 
 * 172   11-05-23 13:50 Chmazar
 * Remove LAN_IS_R1LA_MASK and LAN_IS_R1LB_MASK. The P4LW configuration is
 * checked for R1LA/R1LB based on P1LX/P1LY bit
 * 
 * 171   10-11-25 11:35 Chmazar
 * Defines for R1LB
 * 
 * 170   10-11-24 16:13 Chhamaa
 * Defines for local RF loopback added.
 * 
 * 169   10-11-18 13:51 Chhamaa
 * Bitmasks for powerSupplyVoltage added.
 * 
 * 168   10-10-29 14:20 Chhamaa
 * t_SystemStatus modified.
 * 
 * 167   9.06.10 11:56 Chhamaa
 * t_SystemStatus modified.
 * 
 * 166   8.06.10 18:40 Chhamaa
 *  t_SystemConfig and t_P4LT_Status modified for voice transit.
 * 
 * 163   23.07.07 16:53 Chtognu
 * NSK adaptive Equalizer types added to: t_P4LT_Config, t_P4LT_Status
 * 
 * 162   07-07-06 13:59 Chmaflu
 * "Add methods GetP4LWConfigStructSize(..) and
 * GetP4LWStatusStructSize(..) 
 * Add defines for new Release 3.0 with O4CV
 * Add defines for Config Struct Size and Status Struct size for each
 * Comp. Version
 * Increase define for number of modules
 * Extend Compatibility tables with O4CV functionality
 * Update Compatibilty tables with current numbers"
 * 
 * 161   28.02.07 17:49 Chhamaa
 * BOARD_INFO_O4CV_BOARD_NUMBER_MASK added.
 * 
 * 160   27.02.07 14:30 Chhamaa
 * settings added to t_O4CV_Cfg data structure.
 * 
 * 159   30.01.07 17:32 Chhamaa
 * t_MUX_O4CV_Status added.
 * 
 * 158   26.01.07 10:42 Chhamaa
 * O4CVInterfaceSettings added to t_ServicesData
 * 
 * 157   12.01.07 13:53 Chhamaa
 * O4CVSettings added to t_ServicesData
 * 
 * 156   9.01.07 16:37 Chhamaa
 * #define O4CV_BOARD added
 * 
 * 154   16.10.06 8:43 Chchlee
 * 
 * 153   21.09.06 14:17 Chhamaa
 * ActualNoiseBudget removed from t_AES_Status
 * 
 * 152   23.08.06 16:57 Chchlee
 * EQUIPMENT_CFG_R1LA_PLUGGED added				
 * 
 * 151   11.08.06 16:27 Chchlee
 * LAN_SERVICES_MASK added
 * 
 * 150   9.08.06 16:48 Chchlee
 * LAN_SERVICES_MASK added
 * 
 * 148   5.04.06 10:09 Chsanoh
 * Auto-select standard filter introduced for Tx RF and Rx RF filter
 * tuning.
 * 
 * 147   17.03.06 15:24 Chrobif
 * Define LAN_SERVICES_OFF added.
 * 
 * 146   16.03.06 13:15 Chrobif
 * Bit masks for data member LANSettings (t_ServicesData) added.
 * 
 * 145   24.02.06 16:14 Chsanoh
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
 * 143   23.01.06 16:15 Chmibuh
 * Comment added
 * 
 * 142   19.01.06 16:48 Chmibuh
 * t_EquipmentSWID. Dummy1 -> t_EquipmentSWID.CRC_over_all_code
 * 
 * 141   26.10.05 13:59 Chchlee
 * OP_MODE_SYSTEM_TEST removed
 * 
 * 140   6.10.05 8:28 Chchlee
 * Comment added
 * 
 * 139   22.09.05 9:22 Chchlee
 * 
 * 138   10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 137   10.08.05 9:28 Chchlee
 * configuration version now reduced to 8 bits
 * 
 * 136   20.07.05 14:57 Chchlee
 * EquipmentSWIDrenamed to t_EquipmentSWID. EquipmentInformation and
 * EquipmentConfigInfo removed from t_EquipmentSWID
 * 
 * 135   13.07.05 8:27 Chchlee
 * t_SystemStatus cleaned up
 * 
 * 134   13.07.05 8:14 Chchlee
 * BOARD_HW_VERSION_MASK modified. BOARD_HW_ID_MASK removed
 * 
 * 133   11.07.05 15:20 Chsanoh
 * Calculation of default filter center frequencies adapted!
 * 
 * 132   30.06.05 17:58 Chmibuh
 * 
 * 131   30.06.05 8:29 Chpeplu
 * define LT_START -3 -> removed
 * 
 * 130   22.06.05 17:10 Chchlee
 * 
 * 129   6.06.05 14:57 Chmibuh
 * Piggyback defines removed
 * 
 * 128   3.06.05 10:10 Chmibuh
 * t_SystemStatus expanded (Serie Number)
 * 
 * 127   25.05.05 17:04 Chchlee
 * Development test settings replaced by 7 spare longs
 * 
 * 126   20.05.05 14:43 Chchlee
 * 
 * 125   17.05.05 17:02 Chsanoh
 * 
 * 124   12.05.05 18:11 Chsanoh
 * 
 * 123   10.05.05 15:37 Chsanoh
 * 
 * 122   9.05.05 18:07 Chsanoh
 * 
 * 121   9.05.05 16:38 Chsanoh
 * The following variables have been added to struct t_HWSettings:
 * 
 * long txFilterCenterFrequency;
 * long rxFilterCenterFrequency;
 * long txFilterBandwidthAndCoilEndStop;
 * long rxFilterBandwidth;
 * 
 * 4 dummy variables have been removed from struct t_P4LT_Config instead.
 * Therefore overall size of t_P4LT_Config remains the same!
 * 
*-------------------------------------------------------------------------*/

#ifndef P4LT_TYPES_H
#define P4LT_TYPES_H


/* INCLUDE FILES ***********************************************************/

#include "basetype.h"
#include "APLC_Types.h"
#include "OFDM_Types.h" 
#include "NSD_Types.h"
#include "NSK_Types.h"
#include "RF_Types.h"

/* GLOBAL CONSTANT DECLARATIONS ********************************************/

                                          
///////////////////////////////////////////////////////////////////////////////////
// Bit masks for operationMode
///////////////////////////////////////////////////////////////////////////////////

#define OP_MODE_NORMAL							0
#define OP_MODE_COMMISSIONING					1	// normal commissioning mode
#define OP_MODE_COMMISSIONING_RF_TERMINATED		2	// commissioning with RF termination
													// (output power has to be reduced)
 
///////////////////////////////////////////////////////////////////////////////////
// Bit masks for deviceState
///////////////////////////////////////////////////////////////////////////////////

#define DS_MEASURE_TX_CH1					BIT0
#define DS_MEASURE_TX_CH2					BIT1
#define DS_MEASURE_TX_CH3					BIT2
#define DS_MEASURE_RX_CH1					BIT3
#define DS_MEASURE_RX_CH2					BIT4
#define DS_MEASURE_RX_CH3					BIT5
#define DS_MEASURING_DATA_AVAILABLE_CH1		BIT6
#define DS_MEASURING_DATA_AVAILABLE_CH2		BIT7
#define DS_MEASURING_DATA_AVAILABLE_CH3		BIT8


#define CH_MEASUREMENT_RUNNING	(DS_MEASURE_TX_CH1 | DS_MEASURE_RX_CH1 | \
								 DS_MEASURE_TX_CH2 | DS_MEASURE_RX_CH2 | \
								 DS_MEASURE_TX_CH3 | DS_MEASURE_RX_CH3)							

                                                       
/////////////////////////////////////////////////////////
// some NSD600 Looptest defines 
// for communication PC - NSD600
#define		LT_CANCEL	-2	// used by HMI600
#define     LT_FAIL		-1	// used by HMI600

#define		LOOP_TEST_NSD_TPE1		1
#define		LOOP_TEST_NSD_TPE2		2
#define		LOOP_TEST_AES_TPE1		11	



/////////////////////////////////////////////////////////

#define EQUIPMENT_ID_LENGTH 19	// used by HMI600

typedef struct {

	long address;
	long identification[EQUIPMENT_ID_LENGTH];
	long configurationCounter;

} t_EquipmentInfo;	// used by HMI600


// Defines used for member variables of t_HWSettings

// Defines for long powerSupplyVoltage
#define VDC_48	(long)0
#define VAC_115	(long)1 
#define VAC_230	(long)2

#define PS_VOLTAGE_MASK				MASK_4LSB	 // bit0 - bit3
#define PS_REDUNDANT_MODULE_PLUGGED	BIT4		 // bit4

// Defines for unsigned long hybridAndP4RX_Info
#define HYBRIDINFO_IS_P3LB			BIT0		 // hybrid type: 0 = P3DA, 1 = P3LB
#define HYBRIDINFO_IS_125OHM		BIT1		 // cable impedance: 0 = 75Ohm, 1 = 125Ohm
#define P4RX_PLUGGED				BIT31
#define P4RX_UNPLUGGED				~P4RX_PLUGGED

// Defines for unsigned long R1BCMode
#define R1BC_MODE_RELAY1_ALARM1		BIT0		 // enable alarm 1 for relay 1
#define R1BC_MODE_RELAY1_ALARM2  	BIT1		 // enable alarm 2 for relay 1
#define R1BC_MODE_RELAY1_ALARM3	    BIT2		 // enable alarm 3 for relay 1

#define R1BC_MODE_RELAY2_ALARM1		BIT3		 // enable alarm 1 for relay 2
#define R1BC_MODE_RELAY2_ALARM2  	BIT4		 // enable alarm 2 for relay 2
#define R1BC_MODE_RELAY2_ALARM3	    BIT5		 // enable alarm 3 for relay 2

#define R1BC_MODE_RELAY3_ALARM1		BIT6		 // enable alarm 1 for relay 3
#define R1BC_MODE_RELAY3_ALARM2  	BIT7		 // enable alarm 2 for relay 3
#define R1BC_MODE_RELAY3_ALARM3	    BIT8		 // enable alarm 3 for relay 3

#define R1BC_MODE_RELAY4_ALARM1		BIT9		 // enable alarm 1 for relay 4
#define R1BC_MODE_RELAY4_ALARM2  	BIT10		 // enable alarm 2 for relay 4
#define R1BC_MODE_RELAY4_ALARM3	    BIT11	     // enable alarm 3 for relay 4

#define R1BC_MODE_RELAY5_ALARM1		BIT12	     // enable alarm 1 for relay 5
#define R1BC_MODE_RELAY5_ALARM2  	BIT13	     // enable alarm 2 for relay 5
#define R1BC_MODE_RELAY5_ALARM3	    BIT14    	 // enable alarm 3 for relay 5

#define R1BC_MODE_RELAY6_ALARM1		BIT15    	 // enable alarm 1 for relay 6
#define R1BC_MODE_RELAY6_ALARM2  	BIT16	     // enable alarm 2 for relay 6
#define R1BC_MODE_RELAY6_ALARM3	    BIT17	     // enable alarm 3 for relay 6

#define R1BC_MODE_RELAY7_ALARM1		BIT18	     // enable alarm 1 for relay 7
#define R1BC_MODE_RELAY7_ALARM2  	BIT19	     // enable alarm 2 for relay 7
#define R1BC_MODE_RELAY7_ALARM3	    BIT20	     // enable alarm 3 for relay 7

#define R1BC_MODE_RELAY8_ALARM1		BIT21	     // enable alarm 1 for relay 8
#define R1BC_MODE_RELAY8_ALARM2  	BIT22	     // enable alarm 2 for relay 8
#define R1BC_MODE_RELAY8_ALARM3	    BIT23	     // enable alarm 3 for relay 8

#define R1BC_PLUGGED				BIT31
#define R1BC_UNPLUGGED				~R1BC_PLUGGED

// Defines for unsigned long txFilterBandwidthAndCoilEndStop
#define TX_FILTER_BW				(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5)
#define TX_FILTER_BW_4KHZ			(unsigned long)0x00000004
#define TX_FILTER_BW_8KHZ			(unsigned long)0x00000008
#define TX_FILTER_BW_16KHZ			(unsigned long)0x00000010
#define TX_FILTER_BW_32KHZ			(unsigned long)0x00000020
#define TX_COIL1_END_STOP_POS_MASK	(unsigned long)0x00000F00	// Value coded end-stop 
																// position for coil1 (0...7)
#define TX_COIL2_END_STOP_POS_MASK	(unsigned long)0x0000F000	// Value coded end-stop 
																// position for coil2 (0...7)
#define TX_COIL1_NEUTRAL_POS		(unsigned long)0x00000400
#define TX_COIL2_NEUTRAL_POS		(unsigned long)0x00004000
#define TX_AUTO_SEL_STAND_FILTER	(unsigned long)0x80000000

// Defines for unsigned long rxFilterBandwidth
#define RX_FILTER_BW				(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5)
#define RX_FILTER_BW_4KHZ			(unsigned long)0x00000004
#define RX_FILTER_BW_8KHZ			(unsigned long)0x00000008
#define RX_FILTER_BW_16KHZ			(unsigned long)0x00000010
#define RX_FILTER_BW_32KHZ			(unsigned long)0x00000020
#define RX_AUTO_SEL_STAND_FILTER	(unsigned long)0x80000000

// Defines for txFilterCenterFrequency
#define FIRST_TX_FILTER_BARRIER_FREQUENCY			(long) 500000
#define FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_2		(long) 501000   // for 2 kHz
#define FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_2_5	(long) 500750   // for 2.5 kHz
#define FIRST_RX_FILTER_BARRIER_FREQUENCY_R4_4		(long) 500000   // for 4 kHz
#define FIRST_TX_FILTER_BARRIER_FREQUENCY_R4		(long) 500000
#define TX_FILTER_CENTER_FREQUENCY_STEP				(long)	  500
#define TX_FILTER_CENTER_FREQUENCY_ROUNDING_EDGE	(long)	  250


// Defines for rxFilterCenterFrequency
#define RX_FILTER_CENTER_FREQUENCY_STEP				(long)	  500
#define RX_FILTER_CENTER_FREQUENCY_ROUNDING_EDGE	(long)	  250

typedef struct {

	long powerSupplyVoltage;						// bit0 - bit3: voltage; bit4: redundant module plugged
													// see bit masks PS_ above
	unsigned long hybridAndP4RX_Info;				// bitfield: see bit masks HYBRIDINFO_xx
													//			 see bit masks P4RX_xx
	unsigned long R1BCMode;							// bitfield: see bit masks R1BC_MODE_xx
													//			 see bit masks R1BC_xx
	long txFilterCenterFrequency;					// Hz
	long rxFilterCenterFrequency;					// Hz
	unsigned long txFilterBandwidthAndCoilEndStop;	// bitfield: see bit masks TX_FILTER_BW	 
	unsigned long rxFilterBandwidth;				// bitfield: see bit masks RX_FILTER_BW		

} t_HWSettings;	// used by HMI600


// Defines used for member variables of t_ServicesData

// Defines for "long type"
#define	SERVICES_TYPE_ETL		0	// ETL600 without internal teleprotection	// used by HMI600
#define	SERVICES_TYPE_ETL_NSD	1	// ETL600 with possible NSD600				// used by HMI600
#define SERVICES_TYPE_ETL_AES	2	// ETL600 with possible AES600				// used by HMI600

// Defines of bitmasks for "telephonyTeleprotMode[N_APLC_CHANNELS]"
#define		TELEPHONY_OFF					0	    // used by HMI600
#define		TELEPHONY_4_WIRE				BIT0	// used by HMI600
#define		TELEPHONY_2_WIRE				BIT1	// used by HMI600
#define		TELEPHONY_4_2_WIRE				BIT2	// used by HMI600
#define		EXTERNAL_RTC					BIT3	// used by HMI600
#define		EXTERNAL_TELEPROTECTION			BIT4	// used by HMI600
#define		TELEPHONY_SERVICE				BIT5	// used by HMI600
#define		TELEPHONY_E_M_WIRE_AVAILABLE	BIT6	// used by HMI600

#define		TELEPHONY_MASK					(TELEPHONY_4_WIRE | TELEPHONY_2_WIRE | \
											 TELEPHONY_4_2_WIRE | TELEPHONY_SERVICE | \
											 TELEPHONY_E_M_WIRE_AVAILABLE)				// used by HMI600

#define		TELEPHONY_4_WIRE_MASK			(TELEPHONY_4_WIRE | TELEPHONY_4_2_WIRE)		// used by HMI600

#define		LAN_SERVICES_OFF					0	 // used by HMI600
#define		LAN_HMIOVERLAN_SERVICE_ENABLE_MASK	BIT0 // used by HMI600
#define		LAN_IPROUTING_SERVICE_ENABLE_MASK	BIT1 // used by HMI600
#define		LAN_SNMP_SERVICE_ENABLE_MASK		BIT2 // used by HMI600
#define		LAN_TRAFFIC_FORWARDING_MODE_SWITCH	BIT3 // used by HMI600
#define		LAN_TRAFFIC_FORWARDING_MODE_ROUTER	BIT4 // used by HMI600
//#define		LAN_IS_R1LA_MASK					BIT5 // used by HMI600
//#define		LAN_IS_R1LB_MASK					BIT6 // used by HMI600

#define     LAN_SERVICES_MASK \
				(LAN_HMIOVERLAN_SERVICE_ENABLE_MASK |\
				 LAN_IPROUTING_SERVICE_ENABLE_MASK |\
				 LAN_SNMP_SERVICE_ENABLE_MASK |\
				 LAN_TRAFFIC_FORWARDING_MODE_SWITCH |\
				 LAN_TRAFFIC_FORWARDING_MODE_ROUTER)
//				 LAN_IS_R1LA_MASK |\
//				 LAN_IS_R1LB_MASK)

typedef struct {

	long	        type;											// constants SERVICES_TYPE_ETL_...
	long	        rfPEP;											// Peak Envelope Power [W]
																	// 10, 20, 40, 80 (for 10W, 20W, 40W, 80W)	
	unsigned long	telephonyTeleprotMode      [N_APLC_CHANNELS];	// bitfield, see above
	long			speechBandwidth            [N_APLC_CHANNELS];	// 0...3
	long			numberOfTeleoperationPorts [N_APLC_CHANNELS];	// 0... 20
	long			numberOfITS_Inputs         [NO_TPES];			// 0... 28
	long			numberOfITS_SolidOutputs   [NO_TPES];			// 0... 28
	long			numberOfITS_RelayOutputs   [NO_TPES];			// 0... 14
	long			TelephonySlotID            [N_APLC_CHANNELS];	// slot 0...9 , no telephony: <0 (-1)
	unsigned long	LANSettings;									// bitfield, see above (LAN_*)

} t_ServicesData;	// used by HMI600


// masks for t_O4CV_Cfg.numbersOfBoardsAndChs 
#define	O4CV_N_BOARDS_MASK				BYTE1	// 0... 4 number of O4CV boards
#define	O4CV_N_VOICE_CH_MASK			BYTE2	// 0...16 number of digital voice channels
#define	O4CV_N_AUDIO_CH_MASK			BYTE3	// 0...16 number of digital audio channels

// masks for t_O4CV_Cfg.numbersOfAFInterfaces
#define	O4CV_N_2_WIRE_IF_MASK			BYTE1	// 0...16 number of 2-wire interfaces
#define	O4CV_N_4_WIRE_IF_MASK			BYTE2	// 0...16 number of 4-wire interfaces

// masks for t_O4CV_Cfg.settings
#define	O4CV_VOICE_DATA_RATE_MASK		MASK_4LSB		// defines for data rates see O4CV_Const.h  
#define	O4CV_SERVICE_PHONE_MODE_MASK	(BIT4 | BIT5)	// service phone modes see below

// O4CV service phone modes
#define	O4CV_SERVICE_PHONE_OFF			0
#define	O4CV_SERVICE_PHONE_2_WIRE		1	
#define	O4CV_SERVICE_PHONE_4_WIRE		2

typedef struct {

	unsigned long	numbersOfBoardsAndChs;		// numbers of boards and channels (see defines O4CV_N_ above)
	unsigned long	numbersOfAFInterfaces;		// numbers of 2- and 4-wire interfaces (see defines O4CV_N_ above)
	unsigned long	settings;					// voice data rate, sevice phone mode (see defines O4CV_N_ above)

} t_O4CV_Cfg;


// Board type definitions
#define NO_BOARD	0	// No board configured	// used by HMI600
#define	P4LQ_BOARD	1							// used by HMI600
#define	O4LE_BOARD	2							// used by HMI600
#define	G4AI_BOARD	3							// used by HMI600
#define O4CV_BOARD	4							// usde by HMI600

// Type of board: bit 0 - 7
#define BOARD_INFO_KIND_OF_BOARD_MASK	MASK_8LSB	// used by HMI600

// APLC channel allocation of O4LE AF channel A: bit 8 - 11.
//  Values: 0 = channel is not used							 
//          1 = channel is mapped to APLC channel 1
//          2 = channel is mapped to APLC channel 2
//          3 = channel is mapped to APLC channel 3
//!!WARNING: the values do not correspond to the definitions APLC_CHn defined in APLC_Types.h!!

#define BOARD_INFO_CH_ALLOC_AF_A_MASK		(BIT8|BIT9|BIT10|BIT11)   // 0x00000F00

// APLC channel allocation of O4LE AF channel B: bit 12 - 15
//  Values: same as above							 
#define BOARD_INFO_CH_ALLOC_AF_B_MASK		(BIT12|BIT13|BIT14|BIT15) // 0x0000F000

// O4CV board number: bit 16 - 19
#define BOARD_INFO_O4CV_BOARD_NUMBER_MASK	(BIT16|BIT17|BIT18|BIT19) // 0x000F0000	

typedef struct {

	unsigned long boardInfo;	// bitfield: see BOARD_INFO_ defines above

	// bitfield: contains the basic and extended TDM bus time slots for this board.
	// Example: for the first O4LE, bitMask typically contains the value 0x3c01, Speaking binary, that means:
	//          BIT0      (basic    time slot  0)
	//          BIT10-13  (extended time slots 10-13)
	// 
	unsigned long bitMask;		

} t_TDM_BusConfig;	// used by HMI600


  	
#define R1BCALM_LOCAL_PLC_HW_ALM                     BIT0
#define R1BCALM_LOCAL_PLC_LINK_ALM                   BIT1
#define R1BCALM_LOCAL_PLC_IF_ALM                     BIT2
#define R1BCALM_LOCAL_PLC_WARN                       BIT3

#define R1BCALM_LOCAL_TPE1_HW_ALM                    BIT4
#define R1BCALM_LOCAL_TPE1_LINK_ALM                  BIT5
#define R1BCALM_LOCAL_TPE1_IF_ALM                    BIT6
#define R1BCALM_LOCAL_TPE1_WARN                      BIT7

#define R1BCALM_LOCAL_TPE2_HW_ALM                    BIT8
#define R1BCALM_LOCAL_TPE2_LINK_ALM                  BIT9
#define R1BCALM_LOCAL_TPE2_IF_ALM                    BIT10
#define R1BCALM_LOCAL_TPE2_WARN                      BIT11

#define R1BCALM_LOCAL_DATA_HW_ALM                    BIT12
#define R1BCALM_LOCAL_DATA_LINK_ALM                  BIT13
#define R1BCALM_LOCAL_DATA_IF_ALM                    BIT14
#define R1BCALM_LOCAL_DATA_WARN                      BIT15

#define R1BCALM_REMOTE_PLC_HW_ALM                    (BIT0  << 16)
#define R1BCALM_REMOTE_PLC_LINK_ALM                  (BIT1  << 16)
#define R1BCALM_REMOTE_PLC_IF_ALM                    (BIT2  << 16)
#define R1BCALM_REMOTE_PLC_WARN                      (BIT3  << 16)

#define R1BCALM_REMOTE_TPE1_HW_ALM                   (BIT4  << 16)
#define R1BCALM_REMOTE_TPE1_LINK_ALM                 (BIT5  << 16)
#define R1BCALM_REMOTE_TPE1_IF_ALM                   (BIT6  << 16)
#define R1BCALM_REMOTE_TPE1_WARN                     (BIT7  << 16)

#define R1BCALM_REMOTE_TPE2_HW_ALM                   (BIT8  << 16)
#define R1BCALM_REMOTE_TPE2_LINK_ALM                 (BIT9  << 16)
#define R1BCALM_REMOTE_TPE2_IF_ALM                   (BIT10 << 16)
#define R1BCALM_REMOTE_TPE2_WARN                     (BIT11 << 16)

#define R1BCALM_REMOTE_DATA_HW_ALM                   (BIT12 << 16)
#define R1BCALM_REMOTE_DATA_LINK_ALM                 (BIT13 << 16)
#define R1BCALM_REMOTE_DATA_IF_ALM                   (BIT14 << 16)
#define R1BCALM_REMOTE_DATA_WARN                     (BIT15 << 16)

#define N_R1BC_ALM_BUSSES         3



typedef struct 
{
	unsigned long setupAndHoldTime;
	unsigned long enabledAlarmsAndWarnings; // R1BCALM_ definitions

} t_R1BC_Configuration;


// Equipment type
#define EQ_TYPE_ETL600_R3			0
#define EQ_TYPE_ETL600_R4_4Slots	1	// new RF units, new rack with 4 slots
#define EQ_TYPE_ETL600_R4_7Slots	2	// new RF units, new rack with 7 slots

// System settings
#define SYSTEM_SETTINGS_ENABLE_IRIG_B				    BIT0
#define SYSTEM_SETTINGS_ENABLE_EOC   			        BIT1
#define SYSTEM_SETTINGS_ENABLE_RX_FILTER_MONITORING     BIT2
#define SYSTEM_SETTINGS_ENABLE_AFC						BIT3
#define SYSTEM_SETTINGS_LINE_IMP_45_OHM					BIT4
#define SYSTEM_SETTINGS_LINE_IMP_75_OHM					BIT5
#define SYSTEM_SETTINGS_LINE_IMP_125_OHM				(BIT4 | BIT5)
#define SYSTEM_SETTINGS_LINE_IMPEDANCE_MASK				(BIT4 | BIT5)

#define G4AX_BORADTYPE_G4AI								0
#define G4AX_BORADTYPE_G4AR								1


typedef struct {

	t_EquipmentInfo				equipmentInfo;
	unsigned long				equipmentType;	// see above defines for EQ_TYPE_

	unsigned long				G4Ax_BoardType;	// 0 for G4AI, 1 for G4AR
	unsigned long				spare[5];		// for future extensions

	t_ServicesData				serviceData;
	t_TDM_BusConfig				TDM_Config[NO_TDM_BASIC_TIMESLOTS];
	t_RF_Config					RF;
	t_HWSettings				hwSettings;							 // stores hw settings in P4LT
	t_R1BC_Configuration		R1BC_Config[N_R1BC_ALM_BUSSES];

	unsigned long				configLinkAlarmRelay;				 // bitfield:
	unsigned long				configHWAlarmRelay;					 // bitfield:
	unsigned long               systemSettings;        // see above SYSTEM_SETTINGS_ definitions
	long						remainingActiveTestConfigTime;
	unsigned long				HMIBaudrate;

} t_SystemConfig;	// used by HMI600


//@remark Martin Fluck
//the size of the data struct t_P4LT_Config is part of the compatibilty concept.
//Please modify this size of the data struct inside the file "Compatibility.h"
//#define HMI600_COMP_VERS_P4LW_XXXX_CONFIG_STRUCT_SIZE in the case the data struct 
//from t_P4LT_Config has changed.

typedef struct {
	long						configVersion; // 8 LSBs contain configuration version. The rest is free.
	t_SystemConfig				system;
	t_APLC_Config				APLC;
	t_OFDM_Config				OFDM;
	t_IntTeleprotConfig			ITS;	// Internal Teleprotection System
	t_NSK_Config				NSK;
	t_Mux_Cfg					muxCfg;	// MUX configuration
	t_O4CV_Cfg					O4CVCfg;
	t_NSK_EqualizerCfg			NSKEqualizerCfg;

} t_P4LT_Config;	// used by HMI600	

	
////////////////////////////////////////////////////////////////////////////
// EquipmentSWID
////////////////////////////////////////////////////////////////////////////

// bit masks for HW version	
#define BOARD_HW_VERSION_MASK				(BIT0|BIT1|BIT2)
// Note: PIGGYBACK_HW_ID_MASK	is defined in piggyback.h

// defines for EquipmentStateInfo					
#define EQUIPMENT_STATE_OK					0
#define EQUIPMENT_STATE_EMERGENCY_FW		1
	
// defines for EquipmentConfigInfo
#define EQUIPMENT_CFG_N_APLC_CH0            BIT0 // number of APLC channels				
#define EQUIPMENT_CFG_N_APLC_CH1            BIT1
					
#define EQUIPMENT_CFG_INFO_HAS_DPLC         BIT2 // set if MOD600 configured				
#define EQUIPMENT_CFG_R1LA_PLUGGED          BIT3 // set if R1LA plugged				
				
#define SW_ID_LENGTH   10                // Länge des SWIdentification Strings	// used by HMI600 
										 // 10 long corresponds to 40 char.
typedef struct  {
	long SWIdentification[SW_ID_LENGTH]; 
	long CRC_over_all_code;         // upper 16 Bits are FREE; lower 16 Bits are CRC_over_all_code				
	long MMICompatibilityVersion;	// defines the min. MMI Version which has to be used to communicate with this board
									// do not move this variable inside t_EquipmentSWID !!
	long EquipmentConfigInfo;       // EquipmentConfigInfo
	long EquipmentStateInfo;		// normal or emergency FW is running.
	long HwVersion;
	long LicenseCode;				// Licence code, read from iButton. See licence.h for definition
} t_EquipmentSWID;	// used by HMI600


////////////////////////////////////////////////////////////////////////////
// Alarms and Warnings
////////////////////////////////////////////////////////////////////////////

typedef struct 	
{
	unsigned long    plcHwAlarms;
	unsigned long    plcLinkAlarms;
	unsigned long    plcIfAlarmsAndWarnings;
	unsigned long    dataAlarmsAndWarnings;
	unsigned long    tpe1AlarmsAndWarnings;
	unsigned long    tpe2AlarmsAndWarnings;

} t_AlarmsAndWarnings;


// Structures for alarm polling


// The ETL type is value coded within the 4 MSBs of the variable named dummy3 
// of struct t_AlarmWords.
// 0 = ETL500 
// 1 = ETL600

#define ALARM_POLLING_ETL500_TYPE  (0 << 28)
#define ALARM_POLLING_ETL600_TYPE  (1 << 28)

#define ALARM_POLLING_ETL_TYPE_MASK			BIT28
#define ALARM_POLLING_ETL600_EQ_TYPE_MASK	(BIT31 | BIT30 | BIT29)				// equipment type of ETL600

#define ALARM_POLLING_ETL600_EQ_TYPE_SHIFT	29

// ETL600 structure (Struct must have same size as in ETL500!)

typedef struct {

    // alarmsAndWarnings substitutes ETL500's 
	// Alarm_Status, Alarm_Output, RTAlarm_Status, RTAlarm_Output, NSD Alarms and RT_NSD_Alarms (6 longs)

	t_AlarmsAndWarnings	alarmsAndWarnings;		
	unsigned long	    alarmsFromTDMBus;
	unsigned long	    dummy2;
	unsigned long	    dummy3;      // contains ALARM_POLLING_ETL_TYPE_MASK
} t_AlarmWords;	// used by HMI600	


// ETL500 structure 
typedef struct	{
	long	Alarm_Status;
	long	Alarm_Output;
	long	RTAlarm_Status;
	long	RTAlarm_Output;
	long	NSD_Alarms;
	long	RT_NSD_Alarms;
	long	RTCrcSumDspA;
	long	AlarmsFromTDMBus;
	long	dummy2;
} t_AlarmWords_ETL500;	


////////////////////////////////////////////////////////////////////////////
// P4LT Status
////////////////////////////////////////////////////////////////////////////

#define R1BC_STATE_ALARM_BUS1		    BIT0
#define R1BC_STATE_ALARM_BUS2		    BIT1
#define R1BC_STATE_ALARM_BUS3		    BIT2

#define TDM_BUS_SLOT_COMMUNICATION_ALM	BIT0
#define TDM_BUS_BOARD_ALM				BIT2
#define TDM_BUS_BOARD_WARN				BIT1

#define LENGTH_SERIE_NUM_BOARD			3	// correspond to 12 char
#define LENGTH_SERIE_NUM_iBUTTON		2   // correspond to 8 char (6 char needed)

typedef struct  {
	long 	                dummy1;
	long 	                dummy2;		
	long 	                dummy3;		
	long	                alarmsFromTDMBus;
	long	                numberOfTDM_BusRestarts; // displayed only in HMI600 development version
	unsigned long 	        muxTransitMissingRxData; // bit0-bit15: ch1-ch16 (0: ok, 1: no data)
	t_AlarmsAndWarnings     alarmsAndWarnings;
	unsigned long           R1BC_State;
	unsigned long			deviceState;							
	unsigned long			serieNumber[LENGTH_SERIE_NUM_BOARD];	// Serie Numb. of P4LT/P4LU (FAUF -und LAUF Nummer of ENICS)	
	unsigned long			serieNumber_iButton[LENGTH_SERIE_NUM_iBUTTON];
	unsigned long			fw_version_atmel;						// FW Version of microcontroller
}t_SystemStatus;

typedef struct {
	t_OFDM_Status 		MOD600;
}t_DPLC_Status;

// NSD Status
typedef struct {
	long				ActualTransmissionTime;		// loop test time in us
	float				ActualGuardLevel;			// guard level linear
	float				ActualSNR;					// snr measured in guard channel linear value
	float				ActualSNRAlarmThreshold;	// actual snr alarm threshold depending on configured command application (blocking, perm. direct) linear
	unsigned long		NSD_Alarms;					// actual nsd alarms
} t_NSD_Status;

typedef struct {
	long				ActualTransmissionTime;		// loop test time in us
	float				ActualGuardLevel;			// guard level linear
	float				ActualSNR;					// snr measured in guard channel linear value
	float				ActualSNRAlarmThreshold;	// actual snr alarm threshold depending on configured command application (blocking, perm. direct) linear
	unsigned long		AES_Alarms;					// actual aes alarms
	long				AES_l_dummy_1;				// reserve 1 long
	float				AES_f_dummy_2;				// reserve 2 float
	float				AES_f_dummy_3;				// reserve 3 float
} t_AES_Status;

typedef struct {
	long			mode;							// constants INT_TELPROT_MODE_...
	unsigned long	Actual_CRC_NSD_DSP;				// actual CRC of NSD-DSP Firmware
	t_NSD_Status	NSD_Status[NUM_OF_NSD_INST];	// NSD Status for each channel
	t_AES_Status	AES_Status;						// AES Status 
} t_IntTeleprotStatus;


//@remark Martin Fluck
//the size of the data struct t_P4LT_Status is part of the compatibilty concept.
//Please modify this size of the data struct inside the file "Compatibility.h"
//#define HMI600_COMP_VERS_P4LW_XXXX_STATUS_STRUCT_SIZE in the case the data struct 
//from t_P4LT_Status has changed.


typedef struct  {

	t_SystemStatus 			system;
	t_APLC_Status			APLC;
	t_DPLC_Status			DPLC;	
	t_NSK_Status			NSK;
	t_MUX_Status			MUX;
	t_IntTeleprotStatus		ITS;
	t_MUX_O4CV_Status		MUX_O4CV;
	t_NSK_EqualizerStatus	NSKEqualizer;

}t_P4LT_Status;


////////////////////////////////////////////////////////////////////////////
// Time and Date
////////////////////////////////////////////////////////////////////////////

// String size defines and typedefs for RTC
#define RTC_TIME_STRING_SIZE   				10
#define RTC_DATE_STRING_SIZE   				10
#define RTC_TIME_STRING_SIZE_PACKED			2	// used by HMI600
#define RTC_DATE_STRING_SIZE_PACKED			2	// used by HMI600                           
#define RTC_READ_TIME_AND_DATE				1	// used by HMI600
#define RTC_WRITE_TIME_AND_DATE				2	// used by HMI600


typedef struct  {
	unsigned long  	timeStringPacked[RTC_TIME_STRING_SIZE_PACKED];
	unsigned long  	dateStringPacked[RTC_DATE_STRING_SIZE_PACKED];
	long	timeValue;    
	long 	RTCmode;
} t_TimeDate;	// used by HMI600


////////////////////////////////////////////////////////////////////////////
// Test switches
////////////////////////////////////////////////////////////////////////////

#define TEST_SWITCH_DISABLE_AGC_CH1			BIT0
#define TEST_SWITCH_DISABLE_AGC_CH2			BIT1
#define TEST_SWITCH_DISABLE_AGC_CH3			BIT2

#define TEST_SWITCH_DISABLE_PILOT_CH1		BIT3
#define TEST_SWITCH_DISABLE_PILOT_CH2		BIT4
#define TEST_SWITCH_DISABLE_PILOT_CH3		BIT5

#define TEST_SWITCH_ENABLE_TESTTONE_CH1		BIT6
#define TEST_SWITCH_ENABLE_TESTTONE_CH2		BIT7
#define TEST_SWITCH_ENABLE_TESTTONE_CH3		BIT8

#define TEST_SWITCH_LOCAL_RF_LOOPBACK_ON	BIT9

// bit mask without RF loopack
#define TEST_SWITCHES_BIT_MASK (MASK_8LSB | BIT8)			

// max and min values for test tone frequency in 4 kHz APLC channel
#define MAX_TESTTONE_FREQ_4K		4000
#define MIN_TESTTONE_FREQ_4K		300

// max and min values for test tone level in dB
#define MAX_TESTTONE_LEVEL_DB		0
#define MIN_TESTTONE_LEVEL_DB		-20


typedef struct {
	unsigned long       testSwitches; // normal mode is all test switches off
	t_TestToneConfig	testTone[N_APLC_CHANNELS];
	unsigned long       remainingActiveTestConfigTime;
} t_TestPlcLinkData;	// used by HMI600



/* PUBLIC MACRO DEFINITIONS ***************************************************/



/***************************************************************************************************
Macro:			GET_RF_BANDWIDTH(P4LT_Config, lRF_Bandwidth)
Description:	Gets the RF bandwidth in Hz. This is the sum of all available APLC 
				and DPLC channels on the basis of the struct t_P4LT_Config.
Inputs:			t_P4LT_Config	P4LT configuration of type t_P4LT_Config

Outputs:		lRF_Bandwidth	Long variable containing the RF bandwith 
Return:			None
Globals:		None	
Important:		-
***************************************************************************************************/
#define GET_RF_BANDWIDTH(P4LT_Config, lRF_Bandwidth) \
	GET_APLC_BANDWIDTH((P4LT_Config)->APLC, (lRF_Bandwidth)) \
	(lRF_Bandwidth) = (lRF_Bandwidth) + (P4LT_Config)->OFDM.channelBW; \
// end of macro GET_RF_BANDWIDTH


/* PUBLIC FUNCTIONS ***********************************************************/


#endif
