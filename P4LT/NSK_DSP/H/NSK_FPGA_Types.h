/*-------------------------------------------------------------------------*
 * $Workfile: NSK_FPGA_Types.h $
 * Part of      : ETL600 / MUX600
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA Types
 * Copyright    : ABB Switzerland Ltd. 2005
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSK_DSP/H/NSK_FPGA_Types.h $
 * 
 * 25    11-05-09 16:44 Chhamaa
 * MASK_LOM added.
 * 
 * 24    11-02-16 10:43 Chhamaa
 * GDTC Register Masks added.
 * 
 * 22    5.05.08 17:52 Chhamaa
 * Define BRT_11000 added.
 * 
 * 20    2.11.07 16:15 Chhamaa
 * N_O4CV_CH replaced by N_O4CV_CHANNELS
 * 
 * 19    22.03.07 15:01 Chhamaa
 * O4CV channel bit rate defines added.
 * 
 * 18    07-03-01 14:32 Chchlee
 * e_Port_Number moved from NSK_FPGA_Types.h to NSK_Types.h
 * 
 * 17    18.01.07 10:51 Chhamaa
 * O4CV stuff added.
 * 
 * 15    5.09.06 10:31 Chalgyg
 * MASK_TXC added for new bit in FPGA register.
 * 
 * 14    17.08.06 14:03 Chalgyg
 * MASK_TXC added for new bit in FPGA register.
 * 
 * 13    1.06.06 13:18 Chalgyg
 * Function ListOfG703PortEnables() added.
 * 
 * 11    10.03.06 14:58 Chalgyg
 * LAN port is now supported.
 * 
 * 9     19.10.05 16:56 Chalgyg
 * Define for P1LXa added.
 * 
 * 8     19.10.05 15:20 Chchlee
 * NUMBER_OF_V*_PORTS_P* modified
 * 
 * 7     19.05.05 13:06 Chalgyg
 * Subprint detection was added.
 * 
 * 6     2.05.05 10:41 Chalgyg
 * FPGA TPR register masks added.
 * 
 * 5     16.02.05 12:59 Chalgyg
 * New Datarates added.
 * 
 * 4     8.02.05 13:32 Chalgyg
 * MASK_AON added.
 * 
 * 3     1.02.05 12:02 Chalgyg
 * - RSIG_MASK changed.
 * - ListOfV11PortDatarates Port 1 & 2 merged.
 * 
 * 2     27.01.05 16:26 Chalgyg
 * Some defines improved.
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 3     19.01.05 15:11 Chalgyg
 * 50 Hz bitrate define added.
 * 
 * 2     17.01.05 16:34 Chalgyg
 * Second version.
 * 
 * 1     17.01.05 15:20 Chalgyg
 * Initial Version
 * 
 *-------------------------------------------------------------------------*/

#ifndef NSK_FPGA_TYPES_H
#define NSK_FPGA_TYPES_H

/* INCLUDE FILES ***********************************************************/
#include "constant.h"
#include "NSK_Types.h"
#include "NSK_FPGA.h"
#include "O4CV_Const.h"

/* CONSTANT DECLARATIONS ********************************************/
#define RSIG_MASK				0x0000000F

#define INITIAL_STATE			0

#define FPGA_REGISTER_RESET		0x00000000
#define G703_SHIFT				6

/* CCR Register Masks ******************************************************/
#define MASK_MUX				0x00000001
#define MASK_FSK				0x00000002
#define MASK_CLR				0x00000004
#define MASK_ASY				0x00000008
#define MASK_RXC				0x00000010
#define MASK_ICP				0x00000020
#define MASK_ICT				0x00000040
#define MASK_IAC				0x00000080

/* TPR Register Masks ******************************************************/
#define MASK_CHL0				0x00000001
#define MASK_CHL1				0x00000002
#define MASK_CHL2				0x00000004
#define MASK_SBD				0x00000008
#define MASK_TXC				0x00000020

/* PCR Register Masks ******************************************************/
#define MASK_POE				0x00000001
#define MASK_TPT				0x00000002
#define MASK_LTR				0x00000004
#define MASK_LRC				0x00000008
#define MASK_AON				0x00000010
#define MASK_JAC				0x00000020
#define MASK_DAS				0x00000040
#define MASK_SHD				0x00000080
#define MASK_LOM				0x00000080	// LAN only mode for LAN port

/* TCR Register Masks ******************************************************/
#define MASK_TCR_MUX			0x00000080

/* GDTC Register Masks *****************************************************/
#define MASK_GDTC_MSS			0x00000001
#define MASK_GDTC_RDC			0x00000002
#define MASK_GDTC_RIC			0x00000004
#define MASK_GDTC_LVC			0x00000008
#define MASK_GDTC_CCS			0x000000F0
#define CCS_SHIFT				4

/* FPGA bitrate defines ****************************************************/
#define BRT_50					0x00000000
#define BRT_100					0x00000001
#define BRT_200					0x00000002
#define BRT_300					0x00000003
#define BRT_600					0x00000004
#define BRT_1200				0x00000005
#define BRT_2400				0x00000006
#define BRT_4800				0x00000007
#define BRT_9600				0x00000009
#define BRT_12000				0x0000000A
#define BRT_14400				0x0000000B
#define BRT_16000				0x0000000C
#define BRT_19200				0x0000000D
#define BRT_24000				0x0000000E
#define BRT_28800				0x0000000F
#define BRT_32000				0x00000010
#define BRT_36000				0x00000011
#define BRT_38400				0x00000012
#define BRT_40000				0x00000013
#define BRT_48000				0x00000014
#define BRT_56000				0x00000015
#define BRT_64000				0x00000017
#define BRT_72000				0x00000018
#define BRT_76800				0x00000019
#define BRT_80000				0x0000001A
#define BRT_96000				0x0000001B
#define BRT_112000				0x0000001C
#define BRT_128000				0x0000001E
#define BRT_144000				0x0000001F
#define BRT_153600				0x00000020
#define BRT_160000				0x00000021
#define BRT_192000				0x00000022
#define BRT_224000				0x00000023
#define BRT_256000				0x00000025
#define BRT_288000				0x00000026
#define BRT_307200				0x00000027
#define BRT_320000				0x00000028
#define BRT_11000				0x00000030

/* O4CV channel bit rate defines ***********************************************/
#define BRT_O4CV_OFF			0x00000000
#define BRT_O4CV_5300			0x00000038
#define BRT_O4CV_6300			0x00000043		

/* Subprint Version defines ****************************************************/
#define NO_SUBPRINT_PLUGGED		0x00000007
#define P1LX_VER0_PLUGGED		0x00000000
#define P1LX_VER1_PLUGGED		0x00000001


typedef struct
{
	long ListOfAggregateDatarates[N_DATA_RATES];
	long ListOfV24PortEnables[NUMBER_OF_V24_PORTS_P4LU][N_DATA_RATES];
	long ListOfV11PortDatarates[NUMBER_OF_V11_PORTS_P4LU][N_DATA_RATES];
	long ListOfG703PortEnables[N_DATA_RATES];
	long ListOfLANPortDatarates[N_DATA_RATES];
	long ListOfO4CVChDatarates[N_O4CV_CHANNELS][N_DATA_RATES];
} t_DatarateList;

#endif
/* End of File ***********************************************************/
