/*-------------------------------------------------------------------------*
 * $Workfile: tdm_ext_types.h $
 * Part of	:	LC-PLC
 * Language	:	C
 * Created by:	B. Busslinger NKT1
 * Remarks	:  
 * Purpose	:	TDM-Bus global settings for Master and Slave cards
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/Master_DSP/H/tdm_ext_types.h $
 * 
 * 27    1.03.07 17:33 Chhamaa
 * #define BASIC_O4CV_SUBFRAME_START_BIT added.
 * 
 * 26    17.01.07 17:54 Chhamaa
 * Define CURRENT_DATA_RATE_IX_MASK added
 * 
 * 25    10.01.07 16:26 Chhamaa
 * Defines for O4CV added
 * 
 * 23    5.09.06 16:13 Chhamaa
 * Defines for summary acknowledge of AES tx and rx commands added.
 * 
 * 22    16.08.06 14:51 Chhamaa
 * AES Tx command bits added.
 * 
 * 18    12.10.05 13:10 Chchlee
 * Comments added
 * 
 * 17    27.09.05 15:04 Chchlee
 * ErrorStatus -> chksumErrorCnt
 * 
 * 16    8.09.05 16:37 Chchlee
 * obsolete BASIC_G4AI_SYS_... definitions removed
 * 
 * 15    6.07.05 13:47 Chchlee
 * 
 * 14    18.04.05 10:38 Chchlee
 * Now support of 5 O3LEs 
 * 
 * 13    2.03.05 9:18 Chharyt
 * All EXTENDED1_G4AI_TRIP_G_BIT renamed. Now bug free.
 * 
 * 12    2.03.05 8:15 Chhesch5
 * EXTENDED1_G4AI_TRIP_G_BIT renamed to EXTENDED1_G4AI_TRIP_P_BIT
 * 
 * 11    24.01.05 9:43 Chharyt
 * New defines for bit positions in additional7bits added.
 * 
 * 10    26.11.04 10:02 Chharyt
 * TDM_TO_G4AI_BIT_MASK adjusted/inserted otherwise no start, Guard,...
 * are possible.
 * 
 * 9     7.09.04 7:26 Leeb02
 * 
 * 8     6.09.04 9:06 Leeb02
 * New boost manager concept
 * 
 * 7     2.09.04 8:39 Leeb02
 * 
 * 6     1.09.04 7:41 Leeb02
 * 
 * 5     23.08.04 15:38 Leeb02
 * 
 * 4     3.08.04 11:16 Leeb02
 * 
 * 3     8.07.04 15:00 Bussli02
 * External TDM Bus from Master DSP working (basic time slots tested)
 * 
 * 2     26.04.04 16:54 Leeb02
 * 
 * 1     26.04.04 16:29 Leeb02
 * 
 * 2     22.04.04 10:07 Leeb02
 * #include "frame500.h" removed
 * 
 * 1     10.03.04 12:15 Nohl01
 * Initial version
 * Based on MMI500 v. 3.20
 *---------------------------------------------------------------------------*/



#ifndef TDM_EXT_TYPES_H
#define TDM_EXT_TYPES_H

//****************************************************************************
//
// Struct for Hard Timing Telegram Information, TX and RX
//
//****************************************************************************

typedef struct { 
	unsigned long	SlotID;
	unsigned long	Additional_7Bit;
	unsigned long	chksumErrorCnt;  // number of contiguous checksum errors
} t_TDM_HardTimingTelegramInformation;

// Struct in which all used timeslots are listed (basic and extended)
// we will have such a list for the O4LE basic an extended timeslots
// and for the G4AI basic and extended timeslots
typedef struct {
	long	nEntries;
	long	TimeslotNumber[NO_TDM_BASIC_TIMESLOTS];
} t_TDMTimeslotList;


//************************************************************************************
//
// BASIC TIME SLOTS Defines and Structs for O4LE
//
//************************************************************************************
#define MAX_N_O4LE_BOARDS 5

#define N_O4LE_CHANNELS   2
#define O4LE_CH_A         0
#define O4LE_CH_B         1

#define NO_TELEPHONY_AVAILABLE			0xffffffff			// No telephony port configured

// Defines for O4LE in the Additional_7Bits
#define	BOOSTREQ_CH1_SET				BIT4 
#define	BOOSTREQ_CH2_SET				BIT3 

#define	O4LE_BOOST_A_BIT                BIT2
#define	O4LE_BOOST_B_BIT                BIT3
#define	O4LE_BOOST_C_BIT                BIT4

#define BASIC_O4LE_WARNING_BIT			BIT1				// Only from O4LE to P4LQ
#define BASIC_O4LE_HW_ALARM_BIT			BIT2				// Only from O4LE to P4LQ
#define BASIC_O4LE_E_OR_M_WIRE_BIT	    BIT5				// E&M Wire Bit on O4LE
#define SET_SLOW_MUTE_CH2_ON_TDM		BIT6				// Only from P4LQ to O4LE
#define SET_SLOW_MUTE_CH1_ON_TDM		BIT1				// Only from P4LQ to O4LE
#define SET_RTC_SYNC_BIT				BIT6				// Only from O4LE to P4LQ


//************************************************************************************
//
// BASIC TIME SLOTS Defines and Structs for O4CV
//
//************************************************************************************
#define MAX_N_O4CV_BOARDS 4

// Defines for O4CV in the Additional_7Bits
// to P4LW from O4CV
#define BASIC_O4CV_WARNING_BIT			BIT1
#define BASIC_O4CV_HW_ALARM_BIT			BIT2

// from P4LW to O4CV
#define CURRENT_DATA_RATE_IX_MASK		(BIT1 | BIT2 | BIT3)	// index of current data rate

// same defines in both directions
#define BASIC_O4CV_SUBFRAME_START_BIT	BIT6


///************************************************************************************
//
// BASIC and EXTENDED TIME SLOTS Defines for G4AI
//
//************************************************************************************
// Those are the bit position in the additional 7 bits

// To P4LT from G4AI
#define BASIC_G4AI_WARNING_BIT							BIT1
#define BASIC_G4AI_HW_ALARM_BIT							BIT2
#define BASIC_G4AI_TX_SINGLE_COMP_ALARM_BIT				BIT3
#define BASIC_G4AI_TX_CONT_CMD_ALARM_BIT				BIT4
#define BASIC_G4AI_RX_OVER_CURRENT_ALARM_BIT			BIT5

// From P4LT to G4AI
#define BASIC_G4AI_USER_ALARM_1							BIT1
#define BASIC_G4AI_USER_ALARM_2							BIT2
#define BASIC_G4AI_TPE_HW_ALARM							BIT3
#define BASIC_G4AI_TPE_LINK_ALARM						BIT4
#define BASIC_G4AI_TPE_SUMMARY_ALARM					BIT5

// same defines in both directions 
#define BASIC_G4AI_FRAME_ID_BIT							BIT6
//************************************************************************************


/* Used by G4AI and HMI, not transmitted via TDM-BUS*/
#define G4AI_LOCAL_HW_ALARM					BIT31
#define CLEAR_BASIC_G4AI_ALARMS				~(	BASIC_G4AI_USER_ALARM_1		 | \
												BASIC_G4AI_USER_ALARM_2		 | \
												BASIC_G4AI_TPE_HW_ALARM		 | \
												BASIC_G4AI_TPE_LINK_ALARM 	 | \
												BASIC_G4AI_TPE_SUMMARY_ALARM	)


/* EXTENDED TIME SLOTS Defines for G4AI */

// G4AI tx and rx ("inputs" and "outputs")
 #define EXTENDED1_G4AI_TRIP_A_BIT			BIT0			// Trip A Bit
#define EXTENDED1_G4AI_TRIP_B_BIT			BIT1			// Trip B Bit
#define EXTENDED1_G4AI_TRIP_C_BIT			BIT2			// Trip C Bit
#define EXTENDED1_G4AI_TRIP_D_BIT			BIT3			// Trip D Bit
#define EXTENDED1_G4AI_TRIP_E_BIT			BIT4			// Trip E Bit
#define EXTENDED1_G4AI_TRIP_F_BIT			BIT5			// Trip F Bit
#define EXTENDED1_G4AI_TRIP_P_BIT			BIT6			// Trip P Bit (persistent cmd.)

// G4AI tx ("inputs")
#define EXTENDED1_G4AI_START_BIT			BIT7			// Start bit from G4AI

#define EXTENDED1_G4AI_AES_TX_CMD_20_BIT	BIT12			// tx AES command 20
#define EXTENDED1_G4AI_AES_TX_CMD_19_BIT	BIT13			// tx AES command 19
#define EXTENDED1_G4AI_AES_TX_CMD_18_BIT	BIT14			// tx AES command 18
#define EXTENDED1_G4AI_AES_TX_CMD_17_BIT	BIT15			// tx AES command 17
#define EXTENDED1_G4AI_AES_TX_CMD_16_BIT	BIT16			// tx AES command 16
#define EXTENDED1_G4AI_AES_TX_CMD_15_BIT	BIT17			// tx AES command 15
#define EXTENDED1_G4AI_AES_TX_CMD_14_BIT	BIT18			// tx AES command 14
#define EXTENDED1_G4AI_AES_TX_CMD_13_BIT	BIT19			// tx AES command 13
#define EXTENDED1_G4AI_AES_TX_CMD_12_BIT	BIT20			// tx AES command 12
#define EXTENDED1_G4AI_AES_TX_CMD_11_BIT	BIT21			// tx AES command 11
#define EXTENDED1_G4AI_AES_TX_CMD_10_BIT	BIT22			// tx AES command 10
#define EXTENDED1_G4AI_AES_TX_CMD_9_BIT		BIT23			// tx AES command 9
#define EXTENDED1_G4AI_AES_TX_CMD_8_BIT		BIT24			// tx AES command 8
#define EXTENDED1_G4AI_AES_TX_CMD_7_BIT		BIT25			// tx AES command 7
#define EXTENDED1_G4AI_AES_TX_CMD_6_BIT		BIT26			// tx AES command 6
#define EXTENDED1_G4AI_AES_TX_CMD_5_BIT		BIT27			// tx AES command 5
#define EXTENDED1_G4AI_AES_TX_CMD_4_BIT		BIT28			// tx AES command 4
#define EXTENDED1_G4AI_AES_TX_CMD_3_BIT		BIT29			// tx AES command 3
#define EXTENDED1_G4AI_AES_TX_CMD_2_BIT		BIT30			// tx AES command 2
#define EXTENDED1_G4AI_AES_TX_CMD_1_BIT		BIT31			// tx AES command 1


// G4AI rx ("outputs")
#define EXTENDED1_G4AI_UNBLOCKING_BIT		BIT7			// Unblocking from NSD600 (P4LW)
#define EXTENDED1_G4AI_RX_GUARD_BIT			BIT8			// RX Guard bit from NSD600 (P4LW)

#define EXTENDED1_G4AI_ACC_TRIP_A			BIT9			// acknowledge for Tx command A
#define EXTENDED1_G4AI_ACC_TRIP_B			BIT10			// acknowledge for Tx command B
#define EXTENDED1_G4AI_ACC_TRIP_C			BIT11			// acknowledge for Tx command C
#define EXTENDED1_G4AI_ACC_TRIP_D			BIT12			// acknowledge for Tx command D
#define EXTENDED1_G4AI_ACC_TRIP_E			BIT13			// acknowledge for Tx command E
#define EXTENDED1_G4AI_ACC_TRIP_F			BIT14			// acknowledge for Tx command F
#define EXTENDED1_G4AI_ACC_TRIP_P			BIT15			// acknowledge for Tx command P (persistent cmd.)

#define EXTENDED1_G4AI_AES_TX_CMD_ACK_SUM	BIT9			// summary acknowledge for AES tx commands
#define EXTENDED1_G4AI_AES_RX_CMD_ACK_SUM	BIT10			// summary acknowledge for AES rx commands


#define EXTENDED1_G4AI_AECS_5BIT_MASK	0x0000f800			// Mask for AECS commands

#define EXT1_ALL_ACTIVE_COMMANDS        (EXTENDED1_G4AI_TRIP_A_BIT | EXTENDED1_G4AI_TRIP_B_BIT |\
										 EXTENDED1_G4AI_TRIP_C_BIT | EXTENDED1_G4AI_TRIP_D_BIT |\
										 EXTENDED1_G4AI_TRIP_E_BIT | EXTENDED1_G4AI_TRIP_F_BIT |\
										 EXTENDED1_G4AI_TRIP_P_BIT )


#define EXT1_ALL_ACTIVE_CMDS_AND_START  (EXT1_ALL_ACTIVE_COMMANDS | EXTENDED1_G4AI_START_BIT)

// all bits for G4AI TDM bus
#define	TDM_TO_G4AI_BIT_MASK			(EXTENDED1_G4AI_TRIP_A_BIT | EXTENDED1_G4AI_TRIP_B_BIT |\
										 EXTENDED1_G4AI_TRIP_C_BIT | EXTENDED1_G4AI_TRIP_D_BIT |\
										 EXTENDED1_G4AI_TRIP_E_BIT | EXTENDED1_G4AI_TRIP_F_BIT |\
										 EXTENDED1_G4AI_TRIP_P_BIT | EXTENDED1_G4AI_UNBLOCKING_BIT |\
										 EXTENDED1_G4AI_RX_GUARD_BIT |\
										 EXTENDED1_G4AI_ACC_TRIP_A | EXTENDED1_G4AI_ACC_TRIP_B | \
										 EXTENDED1_G4AI_ACC_TRIP_C | EXTENDED1_G4AI_ACC_TRIP_D | \
										 EXTENDED1_G4AI_ACC_TRIP_E | EXTENDED1_G4AI_ACC_TRIP_F | \
										 EXTENDED1_G4AI_ACC_TRIP_P )

#endif

