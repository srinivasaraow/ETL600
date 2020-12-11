/*-------------------------------------------------------------------------*
 * $Workfile	: O4CV_Types.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : O4CV definitions which are used in HMI600 and O4CV
 * Copyright    : ABB Switzerland Ltd., 2006
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/H/O4CV_Types.h $
 * 
 * 30    12-03-27 15:59 Chalnot
 * Define AF_INTERFACE_LEC_EN added.
 * 
 * 29    11-04-21 11:40 Chhamaa
 * Warning defines added.
 * 
 * 28    11-04-20 18:25 Chhamaa
 * New warning defines added.
 * 
 * 25    07-11-20 16:10 Chrobif
 * Comment added.
 * 
 * 24    9.11.07 14:15 Chsanoh
 * Initialization routines of O4CV and MUX data structures adapted. All
 * expressions 'bool', 'true', 'false' replaced by 'BOOL', 'TRUE',
 * 'FALSE'.
 * 
 * 23    07-09-25 9:48 Chalnot
 * Duslic alarms added.
 * 
 * 22    07-09-17 10:20 Chalnot
 * Define O4CV_EMERGENCY_FW_RUNNING added
 * 
 * 21    07-09-12 14:19 Chrobif
 * Member configVersion added in t_O4CV_Config.
 * 
 * 20    07-09-06 8:33 Chalnot
 * Warning DEFAULT_CONFIGURATION_ACTIVE added.
 * 
 * 19    07-09-04 14:25 Chalnot
 * Alarm defines added.
 * 
 * 18    07-08-23 16:49 Chrobif
 * Errors O4CV_ALARM_PAX_DRIVES_FXS_INTERFACE and 
 * O4CV_ALARM_DUSLIC_CONFIGURATION_FAILED added.
 * 
 * 17    23.07.07 15:10 Chhamaa
 * t_CPTonesCfg removed.
 * 
 * 16    07-06-29 10:51 Chmaflu
 * Change interface mode : let it start with 1
 * 
 * 15    7.05.07 10:17 Chrobif
 * Padding added to ensure that the configuration data structure is a
 * multiple of 4 bytes.
 * 
 * 14    7.05.07 10:08 Chhamaa
 * Defines N_CP_TONES and N_FREQ for CP tones added.
 * 
 * 13    23.04.07 8:59 Chrobif
 * define O4CV_N_FBFF_DATA_RATES added.
 * 
 * 12    16.04.07 14:14 Chrobif
 * Include of BASETYPE.H added.
 * 
 * 11    10.04.07 16:49 Chhamaa
 * t_O4CVStatus modified, alarm defines added.
 * 
 * 10    5.04.07 14:23 Chrobif
 * Padding added to t_O4CVConfig to ensure that it is equally aligned in
 * the HMI600 and on the O4CV.
 * 
 * 9     5.04.07 8:54 Chrobif
 * Description of file purpose modified.
 * 
 * 8     4.04.07 15:22 Chrobif
 * BasetypeO4CV.h included.
 * 
 * 7     4.04.07 10:08 Chrobif
 * Typedefs t_O4CVStatus and t_EquipmentSWID_O4CV added.
 * 
 * 6     15.03.07 15:41 Chhamaa
 * Define CH_CP_TONES_SAME_AS_CH1 added.
 * 
 * 5     5.02.07 14:01 Chhamaa
 * basicTimeSlot added.
 * 
 * 4     5.02.07 11:52 Chhamaa
 * CP tone timing removed.
 * 
 * 3     26.01.07 10:51 Chhamaa
 * Define AF_INTERFACE_EN added
 * 
 * 2     15.01.07 10:43 Chhamaa
 * O4CV settings added.
 * 
 * 1     9.01.07 15:57 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/*-------------------------------------------------------------------------*
 *	Documentaion
 *	(1)	When modifying this type make sure that sizeof(t_O4CVConfig) ==
 *		sum(all members in t_O4CVConfig). This guarantees that the
 *		configuration data struct is aligned identically in the HMI600
 *		and on the O4CV.
 *		Furthermore, make sure that sizeof(t_O4CVConfig) is a multiple of
 *		4 bytes.
 *
 *	(2)	When modifying this type make sure that sizeof(t_O4CVStatus) ==
 *		sum(all members in t_O4CVStatus). This guarantees that the
 *		status data struct is aligned identically in the HMI600
 *		and on the O4CV.
 *		Furthermore, make sure that sizeof(t_O4CVStatus) is a multiple of
 *		4 bytes.
 *
 *	(3)	This type was drived from t_EquipmentSWID in P4LT_Types.h.
 *		Do not modify this structure because the content of a
 *		variable of this type is uploaded by the HMI600 and bytewisely
 *		written to a variable of type t_EquipmentSWID.
 *-------------------------------------------------------------------------*/
 
#ifndef O4CV_TYPES_H
#define O4CV_TYPES_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"
#include "BASETYPE.H"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// defines for AF interface mode
#define AF_INTERFACE_HOTLINE			1 // FXS
#define AF_INTERFACE_REMOTE_SUB			2 // FXS	
#define AF_INTERFACE_2_WIRE_PAX			3 // FXO	
#define AF_INTERFACE_4_WIRE				4

// defines for AF interface settings
#define AF_INTERFACE_EN					BIT0					
#define AF_INTERFACE_INV_M_WIRE			BIT1
#define AF_INTERFACE_INV_E_WIRE			BIT2
#define AF_INTERFACE_G168_EN			BIT3
#define AF_INTERFACE_FILTER_EN			BIT4
#define AF_INTERFACE_LEC_EN				BIT5

// defines for channel settings
#define CH_INPUT_OUTPUT_FILTER_EN		BIT0
#define CH_DELAY_BUFFER_EN				BIT1

// defines for O4CV settings
#define BOARD_NUMBER_MASK				MASK_4LSB	// [1...4]
#define OP_MODE_BIT						BIT4		// 1: master, 0: slave (same as MOD600 operation mode)

// defines for O4CV warnings
#define O4CV_WARNING_NO_WARNING						0
#define O4CV_WARNING_MASK_LED						MASK_8LSB

// defines for O4CV warnings displayed by LED
#define O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE		BIT0
#define O4CV_WARNING_DEFAULT_CONFIGURATION_ACTIVE	BIT1
#define O4CV_WARNING_RESERVED_2						BIT2
#define O4CV_WARNING_RESERVED_3						BIT3
#define O4CV_WARNING_RESERVED_4						BIT4
#define O4CV_WARNING_RESERVED_5						BIT5
#define O4CV_WARNING_RESERVED_6						BIT6
#define O4CV_WARNING_RESERVED_7						BIT7

// defines for O4CV warnings NOT displayed by LED
#define O4CV_WARNING_SYNC_FAILURE_CH1				BIT8
#define O4CV_WARNING_SYNC_FAILURE_CH2				BIT9
#define O4CV_WARNING_SYNC_FAILURE_CH3				BIT10
#define O4CV_WARNING_SYNC_FAILURE_CH4				BIT11
#define O4CV_WARNING_AIS_ON_CH1						BIT12
#define O4CV_WARNING_AIS_ON_CH2						BIT13
#define O4CV_WARNING_AIS_ON_CH3						BIT14
#define O4CV_WARNING_AIS_ON_CH4						BIT15

// defines for O4CV alarms
#define O4CV_ALARM_NO_ALARM							0
#define O4CV_ALARM_TDM_BUS_ERROR					BIT0
#define O4CV_ALARM_INVALID_LICENSE					BIT1
#define O4CV_ALARM_INSUFFICIENT_LICENSE				BIT2
#define O4CV_ALARM_PAX_DRIVES_FXS_INTERFACE			BIT3
#define O4CV_ALARM_MEMORY_CRC_ERROR					BIT4
#define O4CV_ALARM_SLOT_MISMATCH					BIT5
#define O4CV_ALARM_EM_FW_ACTIVE						BIT6
#define O4CV_ALARM_SW_INIT_ERROR					BIT7
#define O4CV_ALARM_SW_RUNTIME_ERROR					BIT8
#define O4CV_ALARM_DUSLIC_CONFIGURATION_FAILED		BIT9
#define O4CV_ALARM_DUSLIC_SYNC_FAILED				BIT10
#define O4CV_ALARM_DUSLIC_DSP_FAILED				BIT11
#define O4CV_ALARM_OVERTEMPERATURE					BIT12
#define O4CV_ALARM_RESERVED_13						BIT13
#define O4CV_ALARM_RESERVED_14						BIT14
#define O4CV_ALARM_RESERVED_15						BIT15

// define for O4CV software identification
#define O4CV_SW_ID_LENGTH			10

// define for O4CV equipmentStateInfo
#define O4CV_EMERGENCY_FW_RUNNING 	BIT0

// number of fallback / fallforward data rates
#define O4CV_N_FBFF_DATA_RATES	5


/* PUBLIC TYPE DECLARATIONS ***************************************************/


typedef struct {

    // NOTE: Refer to Doc (1) when modifying this data structure
	UINT8 mode;				// see defines for AF interface mode
	UINT8 ringerFreq;			
	UINT8 impedanceInd;		// index for impedance
	UINT8 g168ReturnLoss;
	UINT8 filterInd;		// index for speech filter
	UINT8 settings;			// see defines for AF interface settings
	INT16 inputGain;
	INT16 outputGain;

} t_AfInterfaceCfg;


typedef struct {

    // NOTE: Refer to Doc (1) when modifying this data structure
	t_AfInterfaceCfg afInterface;
	UINT8 mode;								// see defines for channel mode	(see O4CV_Const.h)	
	UINT8 dataRate[O4CV_N_FBFF_DATA_RATES];	// see defines for channel data rate (see O4CV_Const.h)
	UINT8 settings;							// see defines for channel settings
	UINT8 padding;

} t_ChannelCfg;


typedef struct {

    // NOTE: Refer to Doc (1) when modifying this data structure
	t_AfInterfaceCfg afInterface;
	UINT8 enabled;			// TRUE, FALSE
	UINT8 padding;

} t_ServicePhoneCfg;


typedef struct {

    // NOTE: Refer to Doc (1) when modifying this data structure
	t_ChannelCfg channel[4];
	t_ServicePhoneCfg servicePhone;
	UINT8 settings;			// see defines for O4CV settings
	UINT8 basicTimeSlot;	// number of basic time slot
	UINT16 configVersion;	// configuration structure version

} t_O4CVConfig;


typedef struct
{

    // NOTE: Refer to Doc (2) when modifying this data structure
	UINT16 warnings;		// See definitions O4CV_WARNING_x above
	UINT16 alarms;			// See definitions O4CV_ALARMS_x above
	INT32  licenseNeeded;	// This member is obsolete. It is neither
							// used in O4CV FW nor in the HMI600

} t_O4CVStatus;


typedef struct
{

    // NOTE: Refer to Doc (3) when modifying this data structure
	INT32 swIdentification[O4CV_SW_ID_LENGTH];	// Defined at compile time
	INT32 crcOverAllCode;						// Not used in 04CV
	INT32 mmiCompatibilityVersion;				// Defined at compile time
	INT32 equipmentConfigInfo;					// Not used in 04CV
	INT32 equipmentStateInfo;					// Set at run time
	INT32 hwVersion;							// Defined at compile time
	INT32 licenseCode;							// Current license on iButton

} t_EquipmentSWID_O4CV;

#endif

