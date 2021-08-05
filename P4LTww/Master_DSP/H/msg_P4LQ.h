/*-------------------------------------------------------------------------*
 * $Workfile: msg_P4LQ.h $
 * Part of	: LCPLC
 * Language	: C
 * Created by	: 
 * Remarks	: ETL500 Transmission protocols, TM-NKT98-1576/_ 
 * Purpose	: Definiert die Message Typen für die Baugruppe P4LQ. 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/Master_DSP/H/msg_P4LQ.h $
 * 
 * 56    11-02-10 9:44 Chhamaa
 * MSG_MUX_SET_VOICE_TRANSIT_TEST_CFG added.
 * 
 * 55    11-01-17 16:34 Chmazar
 * 
 * 54    10-12-01 15:46 Chmazar
 * ETL600 R4 initial version
 * 
 * 53    9.06.10 11:53 Chhamaa
 * MSG_MUX_GET_VOICE_TRANSIT_STATUS added.
 * 
 * 50    08-02-25 10:56 Chtognu
 * Merged changes from ad.equ.branch:
 * MSG_NSK_GET_SHORT_STATUS and MSG_NSK_GET_ADAPT_EQU_TF added
 * 
 * 49    07-12-19 10:03 Chmabur1
 * Added message type to upload R1LA extended developer status
 * 
 * 45    24.03.06 11:16 Chmabur1
 * added MSG_R1LA_GET_FW_DOWNLOAD_REPORT
 * 
 * 44    13.03.06 11:51 Chmabur1
 * 
 * 43    13.03.06 9:15 Chmabur1
 * added define for MSG_R1LA_UPLOAD_EVENT_RECORDER
 * 
 * 42    10.03.06 15:58 Chrobif
 * Message names were renamed from LAN to R1LA and the message
 * MSG_R1LA_UPLOAD_DEVELOPER_STATUS was added.
 * 
 * 41    27.02.06 14:15 Chmibuh
 * new message CONVERT_RTC_DATA_FOR_READ
 * 
 * 39    12.10.05 12:14 Chchlee
 * 
 * 38    22.09.05 9:36 Chchlee
 * 
 * 37    15.07.05 15:38 Chmibuh
 * MSG_ENABLE_EMERGENCY_FW_UPDATE added
 * 
 * 36    13.06.05 7:33 Chchlee
 * reset counter message removed
 * 
 * 35    24.05.05 17:52 Chchlee
 * cleaned up
 * 
 * 34    4.05.05 19:30 Chadkou
 * 
 * 33    13.04.05 17:24 Chadkou
 * 
 * 32    23.02.05 14:08 Chhamaa
 * CHANGE_BETRIEBSKONFIGURATION_P4LQ renamed with SET_OPERATION_MODE
 * 
 * 31    17.02.05 18:36 Chhamaa
 * COMMISSIONING_CFG_SEND added
 * 
 * 30    26.01.05 13:13 Chchlee
 * Id s for erasing cfg and event sectors added
 * 
 * 29    14.01.05 15:00 Chhesch5
 * 
 * 28    14.01.05 7:44 Chhesch5
 * New message SET_EQUALIZER_CONFIG added
 *
 * 27    13.01.05 17:02 Chhamaa
 * New message GET_DEVICE_STATE added
 * 
 * 26    12.01.05 18:46 Chhesch5
 * 
 * 25    20.12.04 15:36 Chpeplu
 * define MSG_READ_NSD_DEVELOPER_STATUS added
 * 
 * 24    15.12.04 17:22 Chhesch5
 * 
 *-------------------------------------------------------------------------*/

#ifndef	_msg_P4LQ
#define	_msg_P4LQ

#include "basetype.h"

// **************************************************************************
// **************************************************************************
//       Note that the message ids are only 16-bits wide!
// **************************************************************************
// **************************************************************************

#define 	MESSAGE_ID_NONE						0x0000


#define 	FW_UPDATE_MESSAGE_P4LQ				0x0107
#define 	PRG_FLASH_MESSAGE_P4LQ			 	0x0108
#define 	ERASE_FLASH_MESSAGE_P4LQ			0x0109
#define 	END_OF_PROG_MESSAGE_P4LQ			0x010a 
#define 	READ_FLOAT_ARRAY_MESSAGE			0x010f
#define 	SAMPLE_DATA_MESSAGE   				0x0110
#define 	READ_STATUS_ARRAY_MESSAGE_P4LQ  	0x0115
#define 	WRITE_STATUS_ARRAY_MESSAGE_P4LQ 	0x0116
#define		SAVE_DATA_TO_EPROM					0x0119
#define		RESET_BOARD		 		            0x011a

#define 	COMMISS_TUNING_TXRF_FILTER			0x011d
#define 	COMMISS_SETTING_TXRF_LEVEL			0x011e
#define 	COMMISS_SETTING_RXRF_LEVEL			0x011f
#define 	COMMISS_SETTING_NFRX_ADJUST			0x0120
#define		COMMISS_HYBRID_SET_TT_LEVEL			0x0123
#define		COMMISSIONING_CFG_SEND				0x0124


// messages for equalizer
#define 	GET_EQUALIZER_STATUS				0x0127
#define 	READ_EQUALIZER_CONFIG				0x0128
#define 	START_MEASUREMENT					0x0131
#define		GET_DEVICE_STATE					0x0134
#define		SET_EQUALIZER_CONFIG				0x0178	

#define		GET_EPROM_ACTUAL_STATUS_P4LQ		0x0133	// used in DFwUpdate::CheckFwUpdateFileName

#define 	SET_OPERATION_MODE					0x013E

// messages for RTC                                                      
#define 	UPLOAD_RTC_DATA						0x0142
#define 	DOWNLOAD_RTC_DATA					0x0143                                                         
#define		CONVERT_RTC_DATA_FOR_READ			0x0144 // used by LAN600 to get time & date without write access                                                       

#define		READ_ALARM_WORDS					0x0151 // used for alarm polling

#define 	READ_EQUIPMENT_INFO					0x0152 // returns g_p_Cfg->system.equipmentInfo structure

// defines for EventRecorder
#define		READ_EVENT							0x0158
#define		READ_EVENT_COUNTERS					0x0159
#define		RESET_EVENT_COUNTERS				0x0160 
#define		READ_ROM_COUNTERS					0x0163


#define		UPLOAD_P4LQ_CONFIGURATION_DATA		0x016b
#define		DOWNLOAD_P4LQ_CONFIGURATION_DATA	0x016c
#define		UPLOAD_P4LQ_STATUS_DATA				0x016d

#define		SAMPLE_DATA_MESSAGE_P4LQ			0x0170

// defines for password access
#define		DOWNLOAD_P4LQ_PASSWORD				0x0171
#define		CHANGE_P4LQ_PASSWORD				0x0172
#define		RESET_P4LQ_PASSWORD					0x0173

// test switches
#define		UPLOAD_P4LQ_TEST_SWITCHES			0x0174
#define		DOWNLOAD_P4LQ_TEST_SWITCHES			0x0175


// NSD600
#define		SEND_LOOPTEST						0x2067
#define		GET_LOOPTEST_TIME					0x2068
#define		READ_TEST_CONFIG_NSD				0x2069
#define		WRITE_TEST_CONFIG_NSD				0x206a
#define		SET_NSD_LOCAL_LOOP					0x2000
                         

#define 	READ_IDLE_COUNTERS	  		        0x3000
#define     USE_TEMPORARY_CFG					0x3001

#define     MSG_OFDM_GET_SHORT_STATUS			0x3002


// developper functions  
#define 	MSG_ERASE_EVENT_RECORDER			0x013B
#define     MSG_OFDM_GET_LONG_STATUS			0x3003
#define		MSG_READ_NSD_DEVELOPER_STATUS		0x3004
#define		MSG_ENABLE_EMERGENCY_FW_UPDATE		0x3005

#define     MSG_NSK_GET_SHORT_STATUS			0x3006
#define     MSG_NSK_GET_ADAPT_EQU_TF			0x3007	// NSK adaptive equalizer transfer function

#define		MSG_MUX_GET_VOICE_TRANSIT_STATUS	0x3008
#define		MSG_MUX_SET_VOICE_TRANSIT_TEST_CFG	0x3009

// messages used by R1LA (LAN600)
#define     MSG_R1LA_UPLOAD_CONFIG				0x3101
#define     MSG_R1LA_DOWNLOAD_CONFIG			0x3102
#define     MSG_R1LA_UPLOAD_STATUS				0x3103
#define     MSG_R1LA_DOWNLOAD_FW				0x3104
#define     MSG_R1LA_SEND_COMMAND				0x3105
#define		MSG_R1LA_END_DOWNLOAD_FW			0x3106 //added 12-04-05
#define		MSG_R1LA_UPLOAD_DEVELOPER_STATUS	0x3107
#define		MSG_R1LA_UPLOAD_EVENT_LOGGER		0x3108 //added 13-03-06
#define 	MSG_R1LA_GET_FW_DOWNLOAD_REPORT		0x3109 //to check if fw dl succeeded, see R1LA_Types.h and firmwareDl.c
#define 	MSG_R1LA_UPLOAD_EXT_DEVEL_STATUS	0x310A //added 2007-12-19

// messages used by R1LB (LAN600), eventually by another new LAN board
//Basic services
#define     MSG_LAN_CONFIG_UPLOAD				0x3200
#define     MSG_LAN_CONFIG_DOWNLOAD				0x3201
#define     MSG_LAN_CONFIG_STORE_TO_FLASH		0x3202
#define     MSG_LAN_STATUS_UPLOAD				0x3203
#define     MSG_LAN_FW_DOWNLOAD					0x3204
#define     MSG_LAN_FW_DOWNLOAD_END				0x3205
#define     MSG_LAN_FW_DOWNLOAD_CONFIRM			0x3206
#define     MSG_LAN_FW_FILE_DELETE				0x3207
#define     MSG_LAN_SYSTEM_RESET				0x3208
//User administration
#define     MSG_LAN_USER_LOGIN					0x3210
#define     MSG_LAN_USER_LOGOUT					0x3211
#define     MSG_LAN_USER_TABLE_UPLOAD			0x3212
#define     MSG_LAN_USER_ADD					0x3213
#define     MSG_LAN_USER_DELETE					0x3214
#define     MSG_LAN_USER_PASSWORD_SET			0x3215
#define     MSG_LAN_USER_ACCESS_LEVEL_SET		0x3216
#define     MSG_LAN_USER_ACCESS_LEVEL_GET		0x3217
//Certificate administration
#define     MSG_LAN_CERTIFICATE_UPLOAD			0x3220
#define     MSG_LAN_CERTIFICATE_GENERATE		0x3221
#define     MSG_LAN_CERTIFICATE_GET_STATE		0x3222
//User activity logging
#define     MSG_LAN_UAL_FILE_UPLOAD				0x3230
#define     MSG_LAN_UAL_FILE_DELETE				0x3231
#define     MSG_LAN_UAL_UPLOAD_NUM_OF_ENTRIES	0x3232
//Alarm polling
#define     MSG_LAN_AP_PAUSE					0x3240
#define     MSG_LAN_AP_RESUME					0x3241
//RTC
#define     MSG_LAN_RTC_UPDATE					0x3250
//Miscellaneous
#define     MSG_LAN_SSL_KEEP_ALIVE				0x32A0

// P4LT-based channel simulator
#define		CS_GET								0x4001
#define		CS_SET								0x4002

// highest message id
#define     MAX_MESSAGE_ID						0xFFFF



// ------------------------------------------------------------------
// additional defines for Transmission protocols messages 
// ------------------------------------------------------------------

// defines for SAMPLE_DATA_MESSAGE_P4LQ signal path
#define SAMPLING_SIGNAL_PATH_APLC_1_TX		0
#define SAMPLING_SIGNAL_PATH_APLC_2_TX		1
#define SAMPLING_SIGNAL_PATH_APLC_1_RX		2
#define SAMPLING_SIGNAL_PATH_APLC_2_RX		3
#define SAMPLING_SIGNAL_PATH_RF_RX			4	// used with MMI500
#define SAMPLING_SIGNAL_PATH_APLC_3_TX		5
#define SAMPLING_SIGNAL_PATH_APLC_3_RX		6

#define SAMPLING_SIGNAL_PATH_IF_TX			10
#define SAMPLING_SIGNAL_PATH_IF_RX			11









#endif                

