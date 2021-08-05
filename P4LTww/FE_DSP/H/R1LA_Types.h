/*-------------------------------------------------------------------------*
 * $Workfile: R1LA_Types.h $
 * Part of      : ETL600 / LAN
 * Language     : C
 * Created by   : 
 * Remarks      : Replacement for LAN_Types.h 
 * Purpose      : Definition of R1LA config and alarm structures
 * Copyright    : ABB Switzerland Ltd., 2006
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/R1LA/src/apps/Lan600/R1LA_Types.h $
 * 
 * 47    09-12-21 10:44 Chmabur1
 * CHG: set version number to 1.24
 * 
 * 46    08-12-01 9:07 Chmabur1
 * Firmware Version changed to 1.23
 * 
 * 45    08-11-10 11:01 Chmabur1
 * Firmware Version changed to 1.23 RC2
 * 
 * 44    08-10-17 16:31 Chmabur1
 * Version number changed
 * 
 * 43    08-10-16 16:09 Chmabur1
 * Max firmware size changed from 600 to 800 kB.
 * 
 * 42    08-10-16 13:28 Chmabur1
 * Changes due to merge from VoIP Branch into MAS Branch
 * 
 * 46    08-09-18 15:16 Chmabur1
 * R1LA_LANCONN_IS_ROHC_ENABLED finalized
 * 
 * 45    08-09-18 10:13 Chmabur1
 * Version number changed to 1.23
 * 
 * 44    18.09.08 10:12 Chsanoh
 * VOIP adaptations
 * 
 * 43    08-09-01 9:28 Chmabur1
 * Maximum firmware size changed from 400000 bytes to 600000 bytes
 * 
 * 42    08-06-06 9:05 Chmabur1
 * Version number changed to 2.00. R1LA configuration extended to hold
 * ROHC defines.
 * 
 * 40    08-04-24 14:52 Chmabur1
 * Version number changed to V1.21
 * 
 * 39    08-04-01 10:53 Chmabur1
 * R1LA Version 1.20
 * 
 * 38    08-03-07 15:11 Chmabur1
 * R1LA Version 1.19
 * 
 * 37    08-03-05 13:23 Chmabur1
 * SNMP sys location and sys name changed
 * 
 * 36    08-03-05 11:36 Chmabur1
 * R1LA Version 1.18
 * 
 * 35    08-02-22 9:42 Chmabur1
 * R1LA firmware version increased from 1.16 to 1.17
 * 
 * 34    07-12-21 16:36 Chmabur1
 * R1LA_EXT_DEVEL_STATUS_NEWLINE_CHAR changed from "\r\n" to "\n"
 * 
 * 33    07-12-19 13:11 Chmabur1
 * 
 * 
 * 32    07-12-19 10:02 Chmabur1
 * Added defines for extended developer status
 * 
 * 31    07-11-19 9:00 Chmabur1
 * New version 1.16
 * 
 * 30    07-11-08 13:30 Chmabur1
 * define for config version added (for compatibility concept)
 * 
 * 29    07-07-08 16:05 Chmaflu
 * "Modifications for added SNMP destination trap port
 * R1LA_MMI_COMPATIBILITY_VERSION increased to 1200"
 * 
 * 28    07-03-21 11:11 Chchlee
 * TCP inactiviy timeout removed
 *
 * 
 *-------------------------------------------------------------------------*/

#ifndef R1LA_TYPES_H
#define R1LA_TYPES_H

/* INCLUDE FILES ***********************************************************/
#include "basetype.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

// FT 1.2 protocol specific:
#define P4LW_REMMOTE_OFFSET						64
#define P4LW_BYPASS_OFFSET						128
#define R1LA_SUBADDRESS							51
#define R1LA_REMOTE_SUBADDRESS					(R1LA_SUBADDRESS+P4LW_REMMOTE_OFFSET) /* 115 */
#define R1LA_BYPASS_SUBADDRESS					(R1LA_SUBADDRESS+P4LW_BYPASS_OFFSET)  /* 179 */

// Filesystem definitions:
#define R1LA_MAX_FILENAME_LENGTH				30

// used to allocate a static buffer on the target. We do this because
// static memory allocation is simpler to test and debug:
#define R1LA_MAX_FIRMWARE_SIZE 					800000 /* [bytes] */ // Max. firmware size R1LA firmware > v. 1.22
#define R1LA_MAX_FIRMWARE_SIZE_OLD 				400000 /* [bytes] */ // Max. firmware size R1LA firmware <= v. 1.22
#define	R1LA_FW_VERS_800_KBYTE_FW_CAPABLE		"R1LA Version 1.22"

// life sign toggle interfal in timer ticks (1 tick = 10 ms)
#define R1LA_LIFE_SIGN_TOGGLE_PERIOD			20

// R1LA boot time
#define R1LA_TIME_TO_BOOT_IN_MS					30000 /* [ms] */

// R1LA Equipment Identification:
#define R1LA_SW_IDENTIFICATION					"R1LA Version 1.24"
#define R1LA_BOOTLOADER_VERSION					"1.06" /* max 4 chars without zero-terminator */
#define R1LA_MMI_COMPATIBILITY_VERSION			1400
#define R1LA_HW_VERSION							0
#define R1LA_CONFIG_VERSION						1

// maximum transmission unit on ethernet side (maximum packet size):
#define R1LA_ETHERNET_MTU						1500

// TCP keepalive interval
#define RILA_TCP_KEEPALIVE_INTERVAL				30 /* [s] */

// HDLC interface configuration:
#define HDLC_SUBNET_MASK						"255.255.255.252"
#define HDLC_MASTER_IP 							"10.0.214.1"
#define HDLC_MASTER_MAC 						"aaaaaa"
#define HDLC_SLAVE_IP 							"10.0.214.2"
#define HDLC_SLAVE_MAC 							"bbbbbb"
#define MAC_SIZE 								6

// SNMP specific defines:
#define R1LA_ENTERPRISE_OID						{ 1,3,6,1,4,1,17268,2818,3,50 } /* 1.3.6.1.4.1.17268.2818.3.50 */
#define R1LA_ENTERPRISE_OID_LEN					10
#define R1LA_SNMP_SYS_CONTACT					"ABB Switzerland, Bruggerstrasse 72, CH-5400 Baden"
#define R1LA_SNMP_SYS_NAME						"ETL600"
#define R1LA_SNMP_SYS_LOCATION					"unknown"


// *****************************************************************************
// ** BEGIN R1LA COMMANDS DEFINITIONS **
// *****************************************************************************
#define R1LA_CMD_ACCEPT_CONFIG_DOWNLOAD			0x00000001
#define R1LA_CMD_ERASE_EVENT_LOGGER				0x00000002 /* added 13-03-06 */
#define R1LA_CMD_RESET_SYSTEM					0x00000003
#define R1LA_CMD_SNMP_PAUSE						0x00000004
#define R1LA_CMD_SNMP_RESUME					0x00000005
#define R1LA_CMD_ERASE_FLASH_FILESYSTEM			0x00000006
#define R1LA_CMD_UNLOCK_TRACE_OUTPUT			0x00000007
#define R1LA_CMD_LOCK_TRACE_OUTPUT				0x00000008
// *****************************************************************************
// ** END R1LA COMMANDS DEFINITIONS **
// *****************************************************************************


// *****************************************************************************
// ** BEGIN R1LA CONFIG STRUCTS **
// *****************************************************************************

//############################################################
// IMPORTANT: The order of the elements in the configuration
// struct is crucial!! This is due to memory alignment, where
// for variables smaller than 32 bits it is possibly more
// efficient to allocate 32 bits in the memory. This is
// generally no problem, since the size of memory is no
// concern. But we don't want to transmit superfluous bits,
// therefore the order of the elements is optimized. You must
// take care if you change anything!
//############################################################

/*
 * REMARK:
 * Representation of IP Addresses in unsigned long format:
 * Ip address a.b.c.d <=> unsigned long 0xabcd (a,b,c,d in 16 bit hex representation)
 * e.g. 10.0.0.5 <=> 0x0a000005
 *
 * Subnet mask: Ip address a.b.c.d/x, where x defines the number of ones in the
 * subnet mask.
 */

#define R1LA_MAX_NOF_IP_ROUTES					10
#define R1LA_MAX_NOF_MNGMT_STATIONS				 4
#define R1LA_MAX_NOF_ALARM_POLLING_DEVICES		64
#define R1LA_MAX_NOF_ALLOWED_CLIENTS			 4

// service flag masks (for config variable 't_R1LA_Config>>serviceFlags'):
#define R1LA_SERVICES_OFF						0
#define	R1LA_HMIOVERLAN_SERVICE_ENABLE_MASK		BIT0
#define R1LA_IPROUTING_SERVICE_ENABLE_MASK		BIT1
#define R1LA_SNMP_SERVICE_ENABLE_MASK			BIT2
#define R1LA_LANSTATIONBUS_SERVICE_ENABLE_MASK	BIT3 /* for future releases */
#define R1LA_UNDEF_SERVICE_FLAG_0				BIT4
#define R1LA_UNDEF_SERVICE_FLAG_1				BIT5
#define R1LA_UNDEF_SERVICE_FLAG_2				BIT6
#define R1LA_UNDEF_SERVICE_FLAG_3				BIT7

// feature flag masks (for config variable 't_R1LA_OverLan_Config>>featureFlags'):
#define R1LA_HMI_OVER_LAN_FEATURES_OFF			0
#define	R1LA_IP_AUTHENTICATION_ENABLE_MASK		BIT0
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_2			BIT1
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_3			BIT2
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_4			BIT3
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_5			BIT4
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_6			BIT5
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_7			BIT6
#define R1LA_UNDEF_HMI_OVER_LAN_FLAG_8			BIT7

// Run modes for IP forwarding:
enum {
	R1LA_LANCONN_ROUTER_STATIC_MODE = 1,
	R1LA_LANCONN_ROUTER_FORWARD_ALL_MODE,
	R1LA_LANCONN_BRIDGE_TRANSPARENT_MODE,
	R1LA_LANCONN_BRIDGE_FILTERED_MODE
};

// Options for IP forwarting/bridging:
#define R1LA_LANCONN_OPT_VJHC_ENABLED			BIT0		/* van jacobson tcp/ip header compression */
#define R1LA_LANCONN_OPT_ROHC_ENABLED			BIT1		/* RoHC VoIP header compression */
#define DEFAULT_R1LA_LANCONN_OPT_HC				(R1LA_LANCONN_OPT_VJHC_ENABLED | R1LA_LANCONN_OPT_ROHC_ENABLED)
// 31 more bits...
#define R1LA_LANCONN_IS_HC_ENABLED(r1laConfig) \
( \
		R1LA_LANCONN_IS_VJHC_ENABLED(r1laConfig) | R1LA_LANCONN_IS_ROHC_ENABLED(r1laConfig)\
)
#define R1LA_LANCONN_IS_VJHC_ENABLED(r1laConfig) \
( \
  (r1laConfig)->bridgingIpRoutingConfig.options & R1LA_LANCONN_OPT_VJHC_ENABLED \
)
#define R1LA_LANCONN_IS_ROHC_ENABLED(r1laConfig) \
( \
		/* PRESTUDY: USE SAME FLAG AS FOR VJ */\
  (r1laConfig)->bridgingIpRoutingConfig.options & R1LA_LANCONN_OPT_ROHC_ENABLED \
)


// "read-only" - you cannot set mode to these values!:
#define R1LA_LANCONN_IS_ROUTER(r1laConfig) ( \
( (r1laConfig)->bridgingIpRoutingConfig.mode == R1LA_LANCONN_ROUTER_STATIC_MODE || \
		(r1laConfig)->bridgingIpRoutingConfig.mode == R1LA_LANCONN_ROUTER_FORWARD_ALL_MODE ) \
)
#define R1LA_LANCONN_IS_BRIDGE(r1laConfig) ( \
( (r1laConfig)->bridgingIpRoutingConfig.mode == R1LA_LANCONN_BRIDGE_TRANSPARENT_MODE || \
		(r1laConfig)->bridgingIpRoutingConfig.mode == R1LA_LANCONN_BRIDGE_FILTERED_MODE ) \
)

// Masks and macros for t_R1LA_Snmp_Config>>alarmPollingInterval
// Note: The member alarmPollingInterval of t_R1LA_Snmp_Config is a bit pattern which contains
//		 various fields:
//			+ Bit  0-5 : Minutes of "Daily at time (HH:MM) mode"
//			+ Bit  6-10: Hours of "Daily at time (HH:MM) mode"
//			+ Bit 11-21: Minutes of "Every x minutes" mode
//			+ Bit 30-31: Mode
//						 Bit31 Bit30
//							0	0		"Daily at time (HH:MM)" mode
//							0	1		"Hourly (1AM, 2AM, ...)" mode
//							1	0		"Every x minutes" mode
//		 Use the set and get macros below to access one of the fields.
//		 Examples:
//		 R1LA_ALARM_POLLING_SET_MINUTE(ulAlarmPollingInterval, ulMinute);
//		 ulMinute = R1LA_ALARM_POLLING_GET_MINUTE(ulAlarmPollingInterval);

#define R1LA_ALARM_POLLING_MODE_RANGE_MASK	   ((unsigned long)(BIT1|BIT0))
#define R1LA_ALARM_POLLING_MINUTE_RANGE_MASK   ((unsigned long)(BIT5|BIT4|BIT3|BIT2|BIT1|BIT0))
#define R1LA_ALARM_POLLING_HOUR_RANGE_MASK	   ((unsigned long)(BIT4|BIT3|BIT2|BIT1|BIT0))
#define R1LA_ALARM_POLLING_INTERVAL_RANGE_MASK ((unsigned long)(BIT11|BIT10|BIT9|BIT8|BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0))

#define R1LA_ALARM_POLLING_SHIFT_MODE	  30
#define R1LA_ALARM_POLLING_SHIFT_MINUTE	   0
#define R1LA_ALARM_POLLING_SHIFT_HOUR	   6
#define R1LA_ALARM_POLLING_SHIFT_INTERVAL 11

#define R1LA_ALARM_POLLING_SET_MODE(ulAlarmPollingInterval, ulMode)												\
			(	ulAlarmPollingInterval = 																								\
				(ulAlarmPollingInterval & ~(R1LA_ALARM_POLLING_MODE_RANGE_MASK << R1LA_ALARM_POLLING_SHIFT_MODE)) |										\
				((ulMode & R1LA_ALARM_POLLING_MODE_RANGE_MASK) << R1LA_ALARM_POLLING_SHIFT_MODE)				\
			)
#define R1LA_ALARM_POLLING_SET_MINUTE(ulAlarmPollingInterval, ulMinute)											\
			(	ulAlarmPollingInterval =																								\
				(ulAlarmPollingInterval & ~(R1LA_ALARM_POLLING_MINUTE_RANGE_MASK << R1LA_ALARM_POLLING_SHIFT_MINUTE)) |									\
				((ulMinute & R1LA_ALARM_POLLING_MINUTE_RANGE_MASK) << R1LA_ALARM_POLLING_SHIFT_MINUTE)			\
			)
#define R1LA_ALARM_POLLING_SET_HOUR(ulAlarmPollingInterval, ulHour)												\
			(	ulAlarmPollingInterval =																								\
				(ulAlarmPollingInterval & ~(R1LA_ALARM_POLLING_HOUR_RANGE_MASK << R1LA_ALARM_POLLING_SHIFT_HOUR)) |										\
				((ulHour & R1LA_ALARM_POLLING_HOUR_RANGE_MASK) << R1LA_ALARM_POLLING_SHIFT_HOUR)				\
			)
#define R1LA_ALARM_POLLING_SET_INTERVAL(ulAlarmPollingInterval, ulInterval)										\
			(	ulAlarmPollingInterval =																								\
				(ulAlarmPollingInterval & ~(R1LA_ALARM_POLLING_INTERVAL_RANGE_MASK << R1LA_ALARM_POLLING_SHIFT_INTERVAL)) |									\
				((ulInterval & R1LA_ALARM_POLLING_INTERVAL_RANGE_MASK) << R1LA_ALARM_POLLING_SHIFT_INTERVAL)	\
			)

#define R1LA_ALARM_POLLING_GET_MODE(ulAlarmPollingInterval)														       \
			(																									       \
			  ((ulAlarmPollingInterval) >> R1LA_ALARM_POLLING_SHIFT_MODE) & R1LA_ALARM_POLLING_MODE_RANGE_MASK         \
			)
#define R1LA_ALARM_POLLING_GET_MINUTE(ulAlarmPollingInterval)													       \
			(																									       \
			  ((ulAlarmPollingInterval) >> R1LA_ALARM_POLLING_SHIFT_MINUTE) & R1LA_ALARM_POLLING_MINUTE_RANGE_MASK 	   \
			)
#define R1LA_ALARM_POLLING_GET_HOUR(ulAlarmPollingInterval)														       \
			(																									       \
			  ((ulAlarmPollingInterval) >> R1LA_ALARM_POLLING_SHIFT_HOUR) & R1LA_ALARM_POLLING_HOUR_RANGE_MASK 		   \
			)
#define R1LA_ALARM_POLLING_GET_INTERVAL(ulAlarmPollingInterval)													       \
			(																							               \
			  ((ulAlarmPollingInterval) >> R1LA_ALARM_POLLING_SHIFT_INTERVAL) & R1LA_ALARM_POLLING_INTERVAL_RANGE_MASK \
			)

#define R1LA_ALARM_POLLING_MODE_DAILY_AT_TIME	0x00000000
#define R1LA_ALARM_POLLING_MODE_HOURLY			0x00000001
#define R1LA_ALARM_POLLING_MODE_EVERY_X_MINUTES 0x00000002

// Ethernet hardware settings (common->flags):
#define R1LA_ETHERNET_NEGOTIATION				BIT0 /* 0=auto, 1=manual */
#define R1LA_ETHERNET_DUPLEX_MODE				BIT1 /* 0=half, 1=full */
#define R1LA_ETHERNET_SPEED						BIT2 /* 0=10 Mbit, 1=100 Mbit */
			
// Common configuration values:
typedef struct { 
    unsigned long 	ipAddress;			// IP Address of the LAN600 Ethernet port
    unsigned long 	defaultGateway;		// IP default gateway of the LAN600 Ethernet port
    unsigned char 	subnetMask; 		// IP Netmask of the LAN600 Ethernet port
    unsigned char 	flags;				// Several flags, actually ETH full/half duplex and 10/100 MBit
    unsigned short int 	ETLaddress;		// FT 1.2 Address of the local ETL
    unsigned long 	serialBaudRate;		// Speed of RS232 communication between R1LA and P4LW
} t_R1LA_Common_Config;


// IP Forwarding configuration values:
typedef struct { 
    unsigned long 	destIpAddresses[R1LA_MAX_NOF_IP_ROUTES];	// Destination IP addresses (those !=0 are valid)
    unsigned long 	nextGateways[R1LA_MAX_NOF_IP_ROUTES]; 	    // Gateways where to forward the IP packets
    unsigned char 	destSubnetMasks[R1LA_MAX_NOF_IP_ROUTES];	// Destination IP Netmasks
    char	 		isMaster;									// Master/Slave config of LAN600, same as MOD600, 1=YES, 0=NO
	char			mode;										// Forwarding mode, see defines above.
	unsigned long	options;									// some options (definitions see above).
} t_R1LA_BridgingIpRouting_Config;
         

// SNMP configuration values:
typedef struct {
    unsigned long 	    alarmPollingMngmtStationIps[R1LA_MAX_NOF_MNGMT_STATIONS]; // IP addresses of SNMP management stations
																				  // note that the corresponding ports are found in t_R1LA_Config
    unsigned short int 	alarmPollingList[R1LA_MAX_NOF_ALARM_POLLING_DEVICES]; 	  // List of FT 1.2 ETL addresses to poll for alarms
    unsigned long 		alarmPollingInterval; 									  // Alarm Polling interval time (see remark above)
} t_R1LA_Snmp_Config;


typedef struct {
	unsigned short int	alarmPollingMngmtStationPorts [R1LA_MAX_NOF_MNGMT_STATIONS]; // port addresses of SNMP management stations
} t_R1LA_Snmp_Config_Ex;


// Hmi over LAN configuration values:
typedef struct { 
    unsigned short int 	tcpPort;								    // TCP port for HMI communication
    unsigned short int 	dummy1;						 
    unsigned char 	    subnetMasks[R1LA_MAX_NOF_ALLOWED_CLIENTS]; 	// Netmasks for IP Authentication
    unsigned long 	    ipAddresses[R1LA_MAX_NOF_ALLOWED_CLIENTS];	// IP addresses for IP Authentication (those !=0 are valid, if no one !=0 --> no authentication check)
	unsigned char	    featureFlags;								// This flag defines which HMI over LAN features are active (masks see above)
	unsigned char	    dummy2[3];
} t_R1LA_HmiOverLan_Config;


// Main R1LA configuration struct:
typedef struct {
    t_R1LA_Common_Config				commonConfig;
    t_R1LA_BridgingIpRouting_Config		bridgingIpRoutingConfig;
    t_R1LA_Snmp_Config					snmpConfig;
    t_R1LA_HmiOverLan_Config			hmiOverLanConfig;
    unsigned char						configVersion;				// Version number for compatibility check
    unsigned char						serviceFlags;				// these bits indicate, which of the 3 services are active, see defines above.
    t_R1LA_Snmp_Config_Ex				snmpConfigEx;  			    // field used to be for future extensions in V1.13
} t_R1LA_Config;

// *****************************************************************************
// ** END R1LA CONFIG STRUCTS **
// *****************************************************************************


// *****************************************************************************
// ** BEGIN R1LA STATUS STRUCTS **
// *****************************************************************************
// --> NORMAL STATUS:
// Alarm Bits:
#define R1LA_ETHERNET_CABLE_DISCONNECTED				BIT0
#define R1LA_ALARM_POLLING_CYCLE_TOO_SHORT				BIT1
#define R1LA_TEMPORARY_CONFIGURATION_ACTIVE				BIT2

// R1LA status struct:
typedef struct  {
	unsigned long	alarmBits;
} t_R1LA_Status;

// --> DEVELOPER STATUS:
#define R1LA_NOF_DEVELOPER_STATUS_DATA_WORDS 			10

#define R1LA_DEVSTAT_FLAGS_LAN_CABLE_UNPLUGGED			BIT0
#define R1LA_DEVSTAT_FLAGS_ETH_ACCEPT_PACKETS			BIT1	// True if global variable ethAcceptPackets is true, false otherwise
#define R1LA_DEVSTAT_FLAGS_ETH_RX_ENABLE				BIT2	// True if bit RXEN in MAC Configuration Register 1 is true, false otherwise

// R1LA developer status struct:
typedef struct  {
	unsigned long		nofBoardStarts; // number of board start-ups
	unsigned short int	ft12GwTxQueueSize;
	unsigned short int	ft12GwEmptyQueueSize;
	unsigned long		flags;
	unsigned long		data[R1LA_NOF_DEVELOPER_STATUS_DATA_WORDS];
} t_R1LA_Developer_Status;


// --> EXTENDED DEVELOPER STATUS:
#define R1LA_EXT_DEVEL_STATUS_DATA_LEN_BYTES			5000
#define R1LA_EXT_DEVEL_STATUS_NEWLINE_CHAR				"\n"
#define R1LA_EXT_DEVEL_STATUS_VAR_NAME_MAX_LEN			30

// R1LA extended developer status data type:
typedef char t_R1LA_Ext_Devel_Status[R1LA_EXT_DEVEL_STATUS_DATA_LEN_BYTES];

// *****************************************************************************
// ** END R1LA STATUS STRUCTS **
// *****************************************************************************



// *****************************************************************************
// ** R1LA EVENT LOGGER DEFINITIONS **
// *****************************************************************************
#define R1LA_EVENT_LOGGER_MAX_LENGTH					30

#define R1LA_EVENT_FILENAME_MAX_LENGTH					20
#define R1LA_EVENT_DESCRIPTION_MAX_LENGTH				80

// R1LA event logger entry, represents one event:
typedef struct {
	char			fileName[R1LA_EVENT_FILENAME_MAX_LENGTH];
	unsigned long	lineNumber;
	char			eventDescription[R1LA_EVENT_DESCRIPTION_MAX_LENGTH];
} t_R1LA_Event_Logger_Entry;

// R1LA event logger data, contains the whole event logger information:
typedef struct {
	t_R1LA_Event_Logger_Entry		events[R1LA_EVENT_LOGGER_MAX_LENGTH];
	char							lastEventPos;
	char							resetCountWithSameEvent;
	char							previousCriticalEventFile[R1LA_EVENT_FILENAME_MAX_LENGTH];
	unsigned long					previousCriticalEventLineNumber;
} t_R1LA_Event_Logger_Data;

// *****************************************************************************
// ** END R1LA EVENT LOGGER DEFINITIONS **
// *****************************************************************************


// *****************************************************************************
// ** R1LA FIRMWARE DOWNLOAD DEFINITIONS **
// *****************************************************************************
// these defines are used in the message of type R1LA_MSG_GET_FW_DOWNLOAD_REPORT
#define R1LA_FWDL_REPORT_SUCCESSFUL						0
#define R1LA_FWDL_REPORT_WRONG_SIGNATURE				BIT0
#define R1LA_FWDL_REPORT_CRC_ERROR						BIT1
#define R1LA_FWDL_REPORT_FLASH_WRITE_ERROR				BIT2
// *****************************************************************************
// ** R1LA END FIRMWARE DOWNLOAD DEFINITIONS **
// *****************************************************************************

#endif
