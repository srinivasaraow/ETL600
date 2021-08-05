/*-------------------------------------------------------------------------*
 * $Workfile: NSK_FPGA.h $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA function declarations
 * Copyright    : ABB Switzerland Ltd. 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSK_DSP/H/NSK_FPGA.h $
 * 
 * 24    11-02-16 10:42 Chhamaa
 * Digital transit registers modified.
 * 
 * 23    10-11-24 16:09 Chhamaa
 * Defines for LAN OSR bits added.
 * 
 * 22    10-10-29 14:29 Chhamaa
 * New FPGA registers for voice transit added.
 * 
 * 19    08-02-25 9:55 Chtognu
 * Merged changes from ad.equ.branch:
 * ad. Equalizer functionality incl. Logger, scrambler, new filter,
 * removed preamble detection, alarm, processing time optimization, global
 * interrupt disabled during DAPSK fir function, FE path expantion
 * 
 * 18    15.11.07 17:02 Chtognu
 * New cases defined for the NSK_FPGA_change_port_bitrates() splitting
 * 
 * 17    2.11.07 16:13 Chhamaa
 * O4CV optimization.
 * 
 * 16    18.01.07 10:51 Chhamaa
 * O4CV stuff added.
 * 
 * 13    1.06.06 13:20 Chalgyg
 * New OSR register defines added for the G.703 interface.
 * 
 * 11    7.04.06 14:55 Chmibuh
 * new defines R1LA_LAN_CONNECTOR_STATUS_FLAG & R1LA_BOOT_STATUS_FLAG
 * 
 * 10    10.03.06 14:24 Chalgyg
 * LAN not booted and LAN not connected is written to the NSK alarm
 * struct.
 * 
 * 8     25.05.05 16:31 Chalgyg
 * Distribute calculation time for OFDM baudrate switching in 3 seperate
 * cycles.
 * 
 * 7     19.05.05 13:05 Chalgyg
 * InitFPGA() now checks if P1LX is plugged.
 * 
 * 6     17.05.05 16:16 Chalgyg
 * A function argument of NSK_FPGA_RD_OSR() changed.
 * 
 * 5     12.05.05 12:03 Chalgyg
 * - Define added.
 * - Function NSK_FPGA_RD_OSR() declaration updated.
 * 
 * 4     2.05.05 10:41 Chalgyg
 * FPGA TPR register added.
 * 
 * 3     10.02.05 9:46 Chalgyg
 * MUX Status inserted.
 * 
 * 2     1.02.05 12:00 Chalgyg
 * NSK_FPGA_WR_RSIG() changed.
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 14    17.01.05 16:34 Chalgyg
 * Only public items are in this header. Privat items are new in
 * NSK_FPGA_Types.h
 * 
 * 13    12.01.05 13:40 Chalgyg
 * OSR register read added.
 * 
 * 12    12.01.05 10:35 Chalgyg
 * New FPGA configuration structure.
 * 
 * 11    10.01.05 10:34 Chalgyg
 * FPGA address range changed to type.
 * 
 * 10    7.01.05 14:56 Chtognu
 * functions for new FPGA (05-01-05)
 * 
 * 9     26.07.04 16:27 Gnus01
 * 
 * 8     23.07.04 8:33 Gnus01
 * 
 * 7     22.07.04 16:05 Gygax02
 * 
 * 6     20.07.04 11:29 Gnus01
 * 
 * 5     16.07.04 9:17 Gygax02
 * 
 * 4     16.07.04 8:26 Gnus01
 * 
 * 3     9.07.04 15:26 Gygax02
 * 
 * 2     24.06.04 14:23 Gygax02
 * 
 * 1     21.06.04 11:21 Gygax02
 * Initial version
 *-------------------------------------------------------------------------*/

#ifndef NSK_FPGA_H
#define NSK_FPGA_H

/* INCLUDE FILES ***********************************************************/
#include "BASETYPE.H"
#include "NSK_DSP_Types.h"
#include "tdm_int_types.h"
#include "O4CV_Const.h"

/* PUBLIC DEFINES **********************************************************/
#define N_MUX_PORTS		11
#define N_NSK_PORTS		4
#define N_V24_PORTS		6 

// OSR register bits
#define AGGREGATE_PORT_LOS_STATUS_FLAG	0x00000002
#define G703_LOS_STATUS_FLAG			0x00000002

#define LAN_ALARM			BIT0						
#define LAN_NOT_BOOTED		BIT1
#define LAN_WARNING			BIT2

/* PUBLIC STRUCTURES *******************************************************/
typedef struct
{
	BOOL	TXD_RTS[N_NSK_MODEMS];
} t_FPGA_NSK_RTS ;

typedef struct
{
	BOOL    TXD_CTS[N_NSK_MODEMS];
} t_FPGA_NSK_RSIG;

typedef struct
{
	long	POE[N_NSK_MODEMS];
	
} t_FPGA_NSK_PCR;

typedef struct
{
	long	IDNT;
	long	TSIG;
	long	TUD[N_NSK_PORTS];
	long	RUD[N_NSK_PORTS];
	long	RSIG;
	long	PCR[N_MUX_PORTS];
	long	CCR[N_MUX_PORTS];
	long	TPR[N_MUX_PORTS];
	long	BRR[N_MUX_PORTS];
	long	OSR[N_MUX_PORTS];
	long	TCR[N_O4CV_CHANNELS];
	long	SPA;						// subprint assembly (none, P1LX, P1LXa, P1LY)
	long	GDTC;						// general digital transit config: slave: 0, master: 1
	long	VCE[N_O4CV_CHANNELS/8];		// voice channel enabled: 1, disabled 0
	long	VCTL[N_O4CV_CHANNELS/8];	// voice channel termination location: local: 0, transit: 1 (ch 0-7)
	long	VCTP[N_O4CV_CHANNELS/2];	// voice channel termination port number
	long	GDTS;						// general digital transit status, no sync/time out
	long	TND[N_O4CV_CHANNELS/8];		// digital transit no rx data (slave)
	long	FDCT;						// frame discard counter Tx for selected channel
	long	FICT;						// frame insertion counter Tx for selected channel
	long	FDCR;						// frame discard counter Rx for selected channel
	long	FICR;						// frame insertion counter Rx for selected channel

} t_FPGA_Registers;

typedef enum
{
	Case_OFDM_Port				= 0,
	Case_V11_Ports				= 1,
	Case_V24_Ports_P4LT			= 2,
	Case_V24_Ports_P4LU			= 3,
	Case_O4CV_Channels_Part1	= 4,
	Case_O4CV_Channels_Part2	= 5,
	Case_Reset_Channels_Part1	= 6,
	Case_Reset_Channels_Part2	= 7,
	Case_Reset_Channels_Part3	= 8,
	Case_Reset_Channels_Part4	= 9

} e_Case_Baudrate_Switch;

/* PUBLIC FUNCTIONS ********************************************************/
BOOL initFPGA(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr);

void NSK_FPGA_RD_TUDn(t_FPGA_Registers* FPGA_Registers_Ptr,t_FPGA_Registers* FPGA_Address_Ptr);
void NSK_FPGA_RD_TSIG(t_FPGA_NSK_RTS *writedata,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr);
void NSK_FPGA_WR_RUDall(t_FPGA_Registers* FPGA_Registers_Ptr,t_FPGA_Registers* FPGA_Address_Ptr);
void NSK_FPGA_WR_RSIG(t_FPGA_NSK_RSIG* Data_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,t_FPGA_Registers* FPGA_Address_Ptr);
void NSK_FPGA_WR_PCR(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_FPGA_NSK_PCR* Data_Ptr,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr);
//void NSK_FPGA_RD_OSR(t_NSK_DSP_Config* NSK_DSP_Config_Ptr,t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_Alarm* nskAlarm_Ptr,BOOL* nskJabberHalt_Ptr,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,long OSR_Counter);
void SEGMENT_EXT_CODE NSK_FPGA_RD_OSR(t_NSK_DSP_Config* NSK_DSP_Config_Ptr,t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_Alarm* nskAlarm_Ptr,BOOL* nskJabberHalt_Ptr,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,long OSR_Counter);

unsigned long SEGMENT_EXT_CODE NSK_FPGA_RD_GDTC_CCS(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr);
unsigned long SEGMENT_EXT_CODE NSK_FPGA_RD_GDTC_LVC(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr);
void SEGMENT_EXT_CODE NSK_FPGA_WR_GDTC_CCS(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, long channel);

void SEGMENT_EXT_CODE NSK_FPGA_resetTransitCounters(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr);
void SEGMENT_EXT_CODE NSK_FPGA_RD_transitCounters(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr);

void SEGMENT_EXT_CODE NSK_FPGA_enableTransitLoopback(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr);
void SEGMENT_EXT_CODE NSK_FPGA_disableTransitLoopback(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr);

void SEGMENT_EXT_CODE NSK_FPGA_RD_GDTS(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, t_NSK_Alarm* nskAlarm_Ptr);
void SEGMENT_EXT_CODE NSK_FPGA_RD_TND(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, t_NSK_Alarm* nskAlarm_Ptr, t_NSK_DSP_Status* NSK_DSP_Status_Ptr);



/* testMatlab
SEGMENT_EXT_CODE e_Case_Baudrate_Switch NSK_FPGA_change_port_bitrates(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr,unsigned long* OFDM_FBFF_state_ptr, t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,e_Case_Baudrate_Switch Case_To_Do);
// end testMatlab */

SEGMENT_EXT_CODE e_Case_Baudrate_Switch NSK_FPGA_change_port_bitrates(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr,unsigned long* OFDM_FBFF_state_ptr, t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,e_Case_Baudrate_Switch Case_To_Do);
//e_Case_Baudrate_Switch NSK_FPGA_change_port_bitrates(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr,unsigned long* OFDM_FBFF_state_ptr, t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,e_Case_Baudrate_Switch Case_To_Do);

// uncomment this definition when using testMatlab
#endif
/* End of File ***********************************************************/
