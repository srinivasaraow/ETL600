/*-------------------------------------------------------------------------*
 * $Workfile: NSK_Types.h $
 * Part of      : ETL600 / NSK Modem
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : NSK global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/NSK_Types.h $
 * 
 * 87    11-06-10 15:27 Chmazar
 * When P4LX board also ports 3 and 4 can be mapped (shared) to MOD600
 * modem
 * 
 * 86    11-05-04 9:00 Chmazar
 * For new R1LB LAN only mode added configuration bit
 * MUX_CFG_CONTROL_LAN_ONLY_MODE
 * 
 * 85    11-03-11 18:06 Chhamaa
 * NUMBER_OF_V11_PORTS_P4LX, NUMBER_OF_V24_PORTS_P4LX added.
 * 
 * 84    11-03-07 10:04 Chmazar
 * New control bit to configure P1LY with additional ports (2 x V11, 4 x
 * V24)
 * 
 * 83    11-02-15 18:02 Chhamaa
 * 
 * 82    11-02-10 9:46 Chhamaa
 * t_MUX_VoiceTransitTestCfg added.
 * 
 * 81    11-01-27 14:02 Chmazar
 * Added configuration parameter "numberOfVoiceTransitChannels" to
 * structure "t_Mux_Cfg".
 * 
 * 80    11-01-10 10:50 Chhamaa
 * Variable and defines of t_Mux_Cfg renamed.
 * 
 * 79    11-01-06 17:03 Chhamaa
 * O4CV defines renamed.
 * 
 * 78    10-12-01 16:20 Chmazar
 * HMI600 R4 initial version
 * 
 * 77    10-10-27 13:23 Chhamaa
 * Defines for voice transit added.
 * 
 * 76    10.06.10 13:29 Chhamaa
 * Comments added.
 * 
 * 75    9.06.10 11:54 Chhamaa
 * t_MUX_VoiceTransitDevStatus added.
 * 
 * 74    8.06.10 18:44 Chhamaa
 * New type t_MUX_VoiceTransitStatus and defines for voice transit added.
 * 
 * 72    22.09.08 13:59 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 72    08-05-07 14:43 Chtognu
 * Max equalizer length changed (bug fixed)
 * 
 * 71    08-05-02 15:43 Chsteic
 * '- changed DAPSK_AD_EQ_LENGTH_MAX  form 40 to 25
 * 
 * 70    08-03-04 10:32 Chsteic
 * '-inserted EQUpdate_Flag. For developing issues used.
 * -inserted NSK_ShortStatus_float and  NSK_ShortStatus_long array
 * - inserted SignalPowerRMax,SignalPowerIMax, normalization_factorMax for
 * the logger
 * 
 * 69    08-02-28 9:35 Chsteic
 * - Removed SNRLevelMin from NSK_DSP_STATUS
 * 
 * - Changend  static unsigned long TestVar to  static long TestVar in
 * NSK_Types.h"
 * 
 * 68    08-02-25 9:55 Chtognu
 * Merged changes from ad.equ.branch:
 * ad. Equalizer functionality incl. Logger, scrambler, new filter,
 * removed preamble detection, alarm, processing time optimization, global
 * interrupt disabled during DAPSK fir function, FE path expantion
 * 
 * 67    23.07.07 16:52 Chtognu
 * NSK adaptive Equalizer types added: t_ModemEqualizerConfig,
 * t_NSK_EqualizerStatus, t_NSK_EqualizerCfg
 * 
 * 66    8.03.07 16:24 Chmaflu
 * fix comment
 * 
 * 65    07-03-01 14:32 Chchlee
 * e_Port_Number moved from NSK_FPGA_Types.h to NSK_Types.h
 * 
 * 64    30.01.07 17:28 Chhamaa
 * t_MUX_O4CV_Status added.
 * 
 * 63    18.01.07 10:53 Chhamaa
 * O4CV stuff added.
 * 
 * 62    9.01.07 15:47 Chhamaa
 * O4CV configuration data added to t_Mux_Cfg.
 * 
 * 60    26.09.06 15:54 Chalgyg
 * V11PORT_CFG_CONTROL_CLK_REGULATOR_ON_BIT added
 * 
 * 59    12.09.06 10:30 Chchlee
 * Comment changed
 * 
 * 57    11.05.06 8:53 Chrobif
 * Configuration of G703 mux port added.
 * 
 * 55    10.03.06 14:58 Chalgyg
 * LAN port is now supported.
 * 
 * 53    19.10.05 15:15 Chchlee
 * 
 * 52    19.10.05 14:55 Chchlee
 * 
 * 51    19.10.05 14:50 Chchlee
 * NUMBER_OF_V11_PORTS_P* modified
 * 
 * 50    13.10.05 19:25 Chchlee
 * Cleaned up
 * 
 * 49    28.06.05 16:15 Chsanoh
 * 
 * 48    27.06.05 11:22 Chsanoh
 * 
 * 47    23.06.05 12:01 Chsanoh
 * 
 * 46    6.06.05 10:23 Chutpra
 * Comment why we don't use Mark and Space in Partiy anymore
 * 
 * 45    3.06.05 10:14 Chutpra
 * change protocol
 * 
 * 44    19.05.05 13:07 Chalgyg
 * Number of port defines changed.
 * 
 * 43    12.05.05 11:40 Chutpra
 * comment out old NSK Defines
 * 
 * 42    21.04.05 13:27 Chutpra
 * Fill existing V24PortCFG Bits with information from the t_modemConfig
 * from
 * the NSK
 * 
 * 41    13.04.05 11:08 Chalgyg
 * CCR Register added to MUX Status.
 * 
 * 40    8.04.05 9:10 Chutpra
 * Change NSK defines for StatusData 
 * 
 * 39    9.03.05 13:34 Chhesch5
 * 
 * 38    2.03.05 14:02 Chutpra
 * Change NSK Status Structure
 * 
 * 37    16.02.05 9:55 Chhesch5
 * added define V11PORT_CFG_CONTROL_CLK_SOURCE_BIT
 * 
 * 36    11.02.05 15:24 Chsanoh
 * 
 * 35    10.02.05 9:46 Chalgyg
 * t_Mux_Status inserted.
 * 
 * 34    8.02.05 13:32 Chalgyg
 * Cosmetics
 * 
 * 33    4.02.05 13:07 Chutpra
 * Change NSK_CFG_TX_DATA_DELAY_MIN from 1 to 0
 * 
 * 32    17.01.05 13:18 Chalgyg
 * V.11 Port config defines from OFDM_Types.h inserted.
 * 
 * 31    17.12.04 13:17 Chhamaa
 * 
 * 30    14.12.04 17:42 Chsanoh
 * 
 * 29    29.11.04 15:41 Gnus01
 * NSK600 status added
 * 
 * 28    18.11.04 16:15 Nohl01
 * 
 * 27    15.11.04 17:58 Nohl01
 * 
 * 26    15.11.04 17:46 Nohl01
 * 
 * 25    5.11.04 17:03 Nohl01
 * 
 * 24    27.10.04 17:34 Nohl01
 * 
 * 23    15.10.04 11:30 Nohl01
 * 
 * 22    20.09.04 17:27 Schnyd01
 * 
 * 21    16.09.04 11:01 Schnyd01
 * Added MUX600 config data
 * 
 * 20    13.09.04 13:49 Schnyd01
 * 
 * 19    13.09.04 8:37 Leeb02
 * Draft Mux Definitions
 * 
 * 18    10.09.04 15:21 Leeb02
 * 
 * 17    8.09.04 10:02 Leeb02
 * 
 * 16    8.09.04 9:11 Leeb02
 * preliminary Mux stuff added
 * 
 * 15    11.08.04 10:48 Nohl01
 * 
 * 14    9.07.04 8:13 Schnyd01
 * NSK_CFG_MODE_RX_CLK_RECOVERY extended to 2 bits
 * 
 * 13    9.07.04 8:05 Gnus01
 * NSK Modem Constants added
 * 
 * 12    30.04.04 9:53 Schnyd01
 * changed tx- and rxfrequeny (struct t_ModemConfig) from float to long
 * 
 * 11    21.04.04 17:40 Nohl01
 * 
 * 10    21.04.04 16:53 Schnyd01
 * 
 * 9     20.04.04 17:20 Schnyd01
 * 
 * 8     16.04.04 17:24 Nohl01
 * 
 * 7     8.04.04 10:19 Nohl01
 * 
 * 6     7.04.04 11:02 Leeb02
 * 
 * 5     30.03.04 9:15 Nohl01
 * Terminology "bitmask" changed to "bitfield" within comments.
 * 
 * 4     29.03.04 17:06 Nohl01
 * None-bitmask variables of type UINT32 changed to INT32. Bitmask
 * variables commented.
 * 
 * 3     17.03.04 8:55 Nohl01
 * Bug fix
 * 
 * 2     15.03.04 14:15 Maag01
 * 
 * 1     5.03.04 11:15 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef NSK_TYPES_H
#define NSK_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include "basetype.h"
#include "constant.h"		// for N_DATA_RATES

#include "O4CV_Const.h"		// for N_O4CV_CHANNELS		


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

#define N_NSK_MODEMS	                    4	    // number of NSK modems

#define NSK_MODEM1  	                    0	    // 1. NSK modem
#define NSK_MODEM2  	                    1	    // 2. NSK modem
#define NSK_MODEM3  	                    2	    // 3. NSK modem
#define NSK_MODEM4  	                    3	    // 4. NSK modem


#define	NSK_CFG_MODE_OFF_ON_PRIORITY_BIT0	BIT0	// 0: off, 1: on disconnetable, 2: on non-disconnectable 
#define	NSK_CFG_MODE_OFF_ON_PRIORITY_BIT1	BIT1	// MSB
#define	NSK_CFG_MODE_OFF_ON_PRIORITY_BITS	(NSK_CFG_MODE_OFF_ON_PRIORITY_BIT0 | NSK_CFG_MODE_OFF_ON_PRIORITY_BIT1)


#define	NSK_CFG_MODE_P2MULTIP_BIT			BIT13	// 1: point to multipoint operation enabled

#define	NSK_CFG_MODE_RX_CLK_RECOVERY_BIT0	BIT14	// 0: data regeneration off, 1: data regeneration on 
#define	NSK_CFG_MODE_RX_CLK_RECOVERY_BIT1	BIT15	// unused

#define	NSK_CFG_MODE_RX_CLK_RECOVERY_BITS	(NSK_CFG_MODE_RX_CLK_RECOVERY_BIT0 | \
											 NSK_CFG_MODE_RX_CLK_RECOVERY_BIT1)

// @CL@ Master DSP must turn off NSK modems which are not in a configured APLC channel!!

// @CL@ better take APLC_CH_n definions (0,1,2)
// @NS@ Bad idea! Meaning of numbers: 0: NSK600 not assigned to any APLC channel
//									  1: NSK600 assigned to APLC channel 1
//									  2: NSK600 assigned to APLC channel 2
//									  3: NSK600 assigned to APLC channel 3

#define	NSK_CFG_MODE_CH_ALLOCATION_BIT0		BIT17	// channel allocation: 0: none, 1: ch1, 2: ch2, 3: ch3
#define	NSK_CFG_MODE_CH_ALLOCATION_BIT1		BIT18	// MSB
#define NSK_CFG_MODE_CH_ALLOCATION_BITS		(NSK_CFG_MODE_CH_ALLOCATION_BIT0 | \
											 NSK_CFG_MODE_CH_ALLOCATION_BIT1)


// lower and upper limits of data types
#define NSK_CFG_TX_DATA_DELAY_MIN			0
#define NSK_CFG_TX_DATA_DELAY_MAX			30

#define DAPSK_AD_EQ_LENGTH			        25    // current length of the ad. equalizer
#define DAPSK_AD_EQ_LENGTH_MAX              27    // maximal length of the ad. equalizer 
												  // (!!! Max length must be higher than the current used length, speed dsp functions !!!)


/* GLOBAL TYPE DECLARATIONS ************************************************/

typedef struct {

	unsigned long	centerFrequency;	// Tx center frequency [Hz] (bits 16..31)
										// Rx center frequency [Hz] (bits 0 ..15)
	float			weight;				// weight of NSK signal
	unsigned long	mode;			    // bitfield: NSK_CFG_MODE bits, see above

	long			bw_txDataDelay;		// bandwidth [Hz]     (bit 16..31)
	       	                            // tx data Delay [ms] (bit  0..15)
	// reserved for futur extensions
	long		Reserved1;
	long		Reserved2;

} t_ModemConfig;


// ********** begin DAPSK Adaptive Equalizer Config ********** 

// Definition for used "mode" bits:
#define AD_EQ_MODE_OFF					0
#define AD_EQ_MODE1_ON					1	
#define AD_EQ_MODE2_ON					2
#define AD_EQ_MODE3_ON					3
#define AD_EQ_MODE4_ON					4
#define AD_EQ_MODE_MASK					(BIT0 | BIT1 | BIT2 | BIT3)

#define AD_EQ_MODE_OP_RANGE_HIGH_ON				BIT4
#define AD_EQ_MODE_OP_RANGE_LOW_ON				BIT5

// Masks for "settings" variables:
#define AD_EQ_SETTINGS_FILTER_LENGTH_MASK		BYTE1
#define AD_EQ_SETTINGS_UPDARE_ITERATIO_MASK		BYTE2
#define AD_EQ_SETTINGS_STEP_LENGTH_MASK			BYTE3

// Masks for "operatingRange" variables:
#define AD_EQ_OP_RANGE_LOW_MASK					(BYTE1 | BYTE2)
#define AD_EQ_OP_RANGE_HIGH_MASK				(BYTE3 | BYTE4)

typedef struct {

	unsigned long mode;					// to select the equalizeralgorithm or put it off
										// and to enable the "OperatingSNRRange"
										// (see definitons AD_EQ_MODE... above)

	unsigned long settings;				// ad. equalizer setting (see definitions AD_EQ_SETTINGS...)
										// length of the equalizer (L) [8bit] / 0 - 255 
	 									// update speed of the equalizer (n) [8bit] / 0 - 255
										// used for LMS algorithm [8bit] / 0 - 255

	unsigned long operatingRange;		// lower SNR limit of ad. Equalizer [16bit] / 0.0 - ..dB (in 0.1dB steps) 
										// --> stop equalization at worse SNR (threshold)
										// higher SNR limit of ad. Equalizer [16bit] / 0.0 - ..dB (in 0.1dB steps) 
										// --> stop equalization at good SNR (threshold) 
										// (see definitions AD_EQ_OP_RANGE...) 
	
	// reserved for futur extensions
	long		reservedL1;

} t_ModemEqualizerConfig;


// ********** begin MUX config ********** 

// NOTE !! currently, some of the MUX data are duplicated in t_OFDM_Config and t_NSK_Config


// number available V.11 and V.24 ports
#define	NUMBER_OF_V11_PORTS_P4LU	2 // number of V.11 ports 
#define NUMBER_OF_V11_PORTS_P4LT	1
#define NUMBER_OF_V11_PORTS_P4LV	1
#define NUMBER_OF_V11_PORTS_P4LX	2

#define	NUMBER_OF_V24_PORTS_P4LU	6 // number of V.24 ports 
#define NUMBER_OF_V24_PORTS_P4LT	2
#define NUMBER_OF_V24_PORTS_P4LV	2
#define NUMBER_OF_V24_PORTS_P4LX	4


// port indices for FPGA ports
typedef enum
{
	OFDM_Port	= 0,
	V24_Port_1	= 1,
	V24_Port_2	= 2,
	V24_Port_3	= 3,
	V24_Port_4	= 4,
	V24_Port_5	= 5,
	V24_Port_6	= 6,
	V11_Port_1	= 7,
	V11_Port_2	= 8,
	G703_Port	= 9,
	LAN_Port	= 10,
} e_Port_Number;

// Number of FPGA ports:
// 6xV.24, 2xV.11, G.703.1, LAN, Aggregate
#define N_COMMUNICATION_PORTS (LAN_Port + 1) 

#define		V11PORT_CFG_ENABLE							BIT0

// these definitions will be removed from t_OFDM_Config, later
#define		V11PORT_CFG_CONTROL_DIN_EDGE_BIT			BIT7	// D-IN  valid on positive(1) / negative (0) edge
#define		V11PORT_CFG_CONTROL_DOUT_EDGE_BIT			BIT8	// D-OUT valid on positive(1) / negative (0) edge

#define		V11PORT_CFG_CONTROL_CLK_SOURCE_BIT			BIT9	// clk from ETL (0, is default) / codirectional (1) 

#define		V11PORT_CFG_CONTROL_CLK_REGULATOR_ON_BIT    BIT10	// 0: clock master = clock regulator off (default) 
	                                                            // 1: clock slave  = clock regulator on

// Contains the V.11 settings that are independent from the MOD600 datarate
typedef struct 
{
	unsigned long control; 
} 
t_V11PortCfg;

// Bits encoding port mode
//  MODE_1		MODE_0
//    0           0			// port disabled (value: 0)
//    0           1			// DAPSK mode	 (value: 1) 
//    1           0			// FSK mode		 (value: 2)
//    1           1			// Mux mode		 (value: 3)
#define V24PORT_CFG_CONTROL_PORT_MODE_0					BIT0
#define V24PORT_CFG_CONTROL_PORT_MODE_1					BIT1
#define V24PORT_CFG_CONTROL_PORT_MODES		(V24PORT_CFG_CONTROL_PORT_MODE_0| \
											 V24PORT_CFG_CONTROL_PORT_MODE_1)

#define V24PORT_CFG_CONTROL_ENABLE_JABBER_CTRL			BIT2

// Bits encoding flow control
// HW_HANDSHAKING_1		HW_HANDSHAKING_0
//			0					0			// None (CTS always on)
//			1					0			// RTS/CTS & DCD (Hardware handshake)
//			0					1			// Data Sensitive
#define V24PORT_CFG_CONTROL_HW_HANDSHAKING_0			BIT3
#define V24PORT_CFG_CONTROL_HW_HANDSHAKING_1			BIT4
#define V24PORT_CFG_CONTROL_HW_HANDSHAKING_MASK		(V24PORT_CFG_CONTROL_HW_HANDSHAKING_0 | \
													 V24PORT_CFG_CONTROL_HW_HANDSHAKING_1)


/////////////////////////////////////////////////////
//---> moved from NSK t_ModemConfig
////////////////////////////////////////////////////

//Protocol

// 0: Character based protocol 
// 1: FT 1.2
// 2: Asynchronous DNP 3.0

#define	V24PORT_CFG_MODE_PROTOCOL_BIT_0			  	BIT5
#define	V24PORT_CFG_MODE_PROTOCOL_BIT_1			  	BIT6    
#define V24PORT_CFG_MODE_PROTOCOL_BIT_MASK          (V24PORT_CFG_MODE_PROTOCOL_BIT_0 | \
													 V24PORT_CFG_MODE_PROTOCOL_BIT_1)    

//Data Bits
#define	V24PORT_CFG_MODE_DATA_BITS_BIT0				BIT7	// [5 .. 8]
#define	V24PORT_CFG_MODE_DATA_BITS_BIT1				BIT8	//
#define	V24PORT_CFG_MODE_DATA_BITS_BIT2				BIT9	//
#define	V24PORT_CFG_MODE_DATA_BITS_BIT3				BIT10	// MSB
#define	V24PORT_CFG_MODE_DATA_BITS				(V24PORT_CFG_MODE_DATA_BITS_BIT0 | V24PORT_CFG_MODE_DATA_BITS_BIT1 | \
												 V24PORT_CFG_MODE_DATA_BITS_BIT2 | V24PORT_CFG_MODE_DATA_BITS_BIT3)
//Stop Bits									
#define	V24PORT_CFG_MODE_STOP_BITS_BIT0				BIT11	// [1, 2]
#define	V24PORT_CFG_MODE_STOP_BITS_BIT1				BIT12	// MSB
#define	V24PORT_CFG_MODE_STOP_BITS				(V24PORT_CFG_MODE_STOP_BITS_BIT0 | V24PORT_CFG_MODE_STOP_BITS_BIT1)	


//Parity
#define	V24PORT_CFG_MODE_PARITY_BIT0				BIT13	// 0: none, 1: odd, 2: even, 3: mark(TG:not used anymore) 
#define	V24PORT_CFG_MODE_PARITY_BIT1				BIT14	// 4: space (TG:not used anymore)
#define	V24PORT_CFG_MODE_PARITY_BIT2				BIT15	// MSB
#define	V24PORT_CFG_MODE_PARITY_BITS			(V24PORT_CFG_MODE_PARITY_BIT0 | V24PORT_CFG_MODE_PARITY_BIT1 | \
												 V24PORT_CFG_MODE_PARITY_BIT2)

//Defines
#define V24PORT_CFG_DATA_BITS_MIN				5
#define V24PORT_CFG_DATA_BITS_MAX				8

#define V24PORT_CFG_STOP_BITS_MIN				1
#define V24PORT_CFG_STOP_BITS_MAX				2



typedef struct 
{
	unsigned long		datarate;
	unsigned long		control;	// see V24PORT_CFG_CONTROL_... definitions
} 
t_V24PortCfg;


// This structure defines the port settings for one MOD600 datarate (= aggregate datarate).
// Note: for ports that are used for FSK or DAPSK, the settings must be the same throughout all MOD600 datarates

#define MUX_DATARATE_PORT_ENABLE_V24_PORT1	BIT0
#define MUX_DATARATE_PORT_ENABLE_V24_PORT2	BIT1
#define MUX_DATARATE_PORT_ENABLE_V24_PORT3	BIT2
#define MUX_DATARATE_PORT_ENABLE_V24_PORT4	BIT3
#define MUX_DATARATE_PORT_ENABLE_V24_PORT5	BIT4
#define MUX_DATARATE_PORT_ENABLE_V24_PORT6	BIT5
#define MUX_DATARATE_PORT_ENABLE_G703		BIT6

typedef struct 
{
	unsigned long	portEnable;                    // see MUX_DATARATE_PORT_ENABLE_... definitions
	unsigned long   V11_Rate[NUMBER_OF_V11_PORTS_P4LU]; // 2 x V11 ports
	unsigned long	LAN_Rate;
}
t_MuxDatarate;


#define		G703_PORT_CFG_ENABLE	BIT0

typedef struct 
{
	unsigned long control; // see G703_PORT_CFG_... masks above
} 
t_G703PortCfg;

#define		LAN_PORT_CFG_ENABLE							BIT0

#define		LAN_PORT_CFG_CONTROL_DIN_EDGE_BIT			BIT7	// D-IN  valid on positive(1) / negative (0) edge
#define		LAN_PORT_CFG_CONTROL_DOUT_EDGE_BIT			BIT8	// D-OUT valid on positive(1) / negative (0) edge

#define		LAN_PORT_CFG_CONTROL_CLK_SOURCE_BIT			BIT9	// clk from DCE (0, is default) / clk from DTE (1) 

typedef struct
{
	unsigned long control;	// contents to be defined
}
t_LANPortCfg; 


typedef struct 
{
	t_V24PortCfg    V24[NUMBER_OF_V24_PORTS_P4LU]; // 6
	t_V11PortCfg	V11[NUMBER_OF_V11_PORTS_P4LU]; // 2
	t_G703PortCfg	G703;
	t_LANPortCfg	LAN;
} 
t_PortCfg;


// Definitions for ComPortStatus
#define PORT_JABBER_HALT_SET      BIT0                          // FPGA OSR Reg. Bit0 JAH (for V.24)
#define PORT_JABBER_HALT_CLEAR    ~PORT_JABBER_HALT_SET			// "Jabber Halt"
#define PORT_SYNCHR_LOSS_SET      BIT1                          // FPGA OSR Reg. Bit1 LOS (for DPLC Multiplexer Frame, ev. G.703)
#define PORT_SYNCHR_LOSS_CLEAR    ~PORT_SYNCHR_LOSS_SET			// "Loss of Frame (Synchronisation)"
#define PORT_SYNCHR_HIST0_SET     BIT2                          // FPGA OSR Reg. Bit2 SHI (for DPLC Multiplexer Frame)
#define PORT_SYNCHR_HIST0_CLEAR   ~PORT_SYNCHR_HIST0_SET
#define PORT_SYNCHR_HIST1_SET     BIT3                          // FPGA OSR Reg. Bit3 SHI (for DPLC Multiplexer Frame)
#define PORT_SYNCHR_HIST1_CLEAR   ~PORT_SYNCHR_HIST1_SET
#define PORT_SYNCHR_HIST2_SET     BIT4                          // FPGA OSR Reg. Bit4 SHI (for DPLC Multiplexer Frame)
#define PORT_SYNCHR_HIST2_CLEAR   ~PORT_SYNCHR_HIST2_SET
#define PORT_SYNCHR_HIST3_SET     BIT5                          // FPGA OSR Reg. Bit5 SHI (for DPLC Multiplexer Frame)
#define PORT_SYNCHR_HIST3_CLEAR   ~PORT_SYNCHR_HIST3_SET
#define PORT_SYNCHR_HIST4_SET     BIT6                          // FPGA OSR Reg. Bit6 SHI (for DPLC Multiplexer Frame)
#define PORT_SYNCHR_HIST4_CLEAR   ~PORT_SYNCHR_HIST4_SET
#define PORT_SYNCHR_HIST5_SET     BIT7                          // FPGA OSR Reg. Bit7 SHI (for DPLC Multiplexer Frame)
#define PORT_SYNCHR_HIST5_CLEAR   ~PORT_SYNCHR_HIST5_SET

#define PORT_OUT_ENABLE_SET       BIT8                          // FPGA PCR Reg. Bit0 POE (for all ports)
#define PORT_OUT_ENABLE_CLEAR     ~PORT_OUT_ENABLE_SET			// "Port Output Enable"
#define PORT_TSTPTRN_ENABLE_SET   BIT9                          // FPGA PCR Reg. Bit1 TPT (for all ports)
#define PORT_TSTPTRN_ENABLE_CLEAR ~PORT_TSTPTRN_ENABLE_SET		// "Testpattern Operation"
#define PORT_TX2RX_LOOP_SET       BIT10                         // FPGA PCR Reg. Bit2 LTR (for all ports)
#define PORT_TX2RX_LOOP_CLEAR     ~PORT_Tx2RX_LOOP_SET			// "Tx to Rx Loop"
#define PORT_RTS2CTS_LOOP_SET     BIT11                         // FPGA PCR Reg. Bit3 LRC (for V.24, ev. V.11)
#define PORT_RTS2CTS_LOOP_CLEAR   ~PORT_RTS2CTS_LOOP_SET		// "RTS to CTS Loop"
#define PORT_AON_SET              BIT12                         // FPGA PCR Reg. Bit4 AON (for V.24)
#define PORT_AON_CLEAR            ~PORT_AON_SET					// "Signalling always On"
#define PORT_JABBBER_CTR_SET      BIT13                         // FPGA PCR Reg. Bit5 JAC (for V.24)
#define PORT_JABBBER_CTR_CLEAR    ~PORT_JABBBER_CTR_SET			// "Jabber Monitoring Enabled"
#define PORT_DATA_SENSITIVE_SET   BIT14                         // FPGA PCR Reg. Bit6 DAS (for V.24)
#define PORT_DATA_SENSITIVE_CLEAR ~PORT_DATA_SENSITIVE_SET		// "Data Sensitive Jabber"
#define PORT_SHARED_SET           BIT15                         // FPGA PCR Reg. Bit7 SHD (for V.24)
#define PORT_SHARED_CLEAR         ~PORT_SHARED_SET				// "Port Shared"

#define PORT_TO_MUX_SET     	  BIT16                         // FPGA CCR Reg. Bit0 MUX (for all ports)
#define PORT_TO_MUX_CLEAR         ~PORT_TO_MUX_SET				// "MUX channel enabled"
#define PORT_FSK_PRECODING_SET    BIT17                         // FPGA CCR Reg. Bit1 FSK (for V.24)
#define PORT_FSK_PRECODING_CLEAR  ~PORT_FSK_PRECODING_SET		// "FSK Precoding Enabled"
#define PORT_CLK_REGULATOR_SET    BIT18                         // FPGA CCR Reg. Bit2 CLR (for all ports)
#define PORT_CLK_REGULATOR_CLEAR  ~PORT_CLK_REGULATOR_SET		// "Clock Regulator Enabled"
#define PORT_ASYNC_TXD_SET        BIT19                         // FPGA CCR Reg. Bit3 ASY (for all ports)
#define PORT_ASYNC_TXD_CLEAR      ~PORT_ASYNC_TXD_SET			// "Async to Sync converter enabled for TxD"
#define PORT_RXCLOCK_OUT_SET      BIT20                         // FPGA CCR Reg. Bit4 RXC (for all ports)
#define PORT_RXCLOCK_OUT_CLEAR    ~PORT_RXCLOCK_OUT_SET			// "Clock Output RxC Enabled"
#define PORT_RXOUT_NEG_POL_SET    BIT21                         // FPGA CCR Reg. Bit5 ICP (for V.11 ports)
#define PORT_RXOUT_NEG_POL_CLEAR  ~PORT_RXOUT_NEG_POL_SET		// "RX Out Negative Active Edge"
#define PORT_TXIN_NEG_POL_SET     BIT22                         // FPGA CCR Reg. Bit6 ICT (for V.11 ports)
#define PORT_TXIN_NEG_POL_CLEAR   ~PORT_TXIN_NEG_POL_SET		// "TX In Active Edge Inversed to RX Out"
#define PORT_INVERT_ALL_SET       BIT23                         // FPGA CCR Reg. Bit7 IAC (for all ports)
#define PORT_INVERT_ALL_CLEAR     ~PORT_INVERT_ALL_SET			// "Invert all Circuits"

#define PORT_OSR_MASK			  0x000000FF
#define PORT_PCR_MASK			  0x0000FF00
#define PORT_CCR_MASK			  0x00FF0000
#define PORT_PCR_SHIFT			  8
#define PORT_CCR_SHIFT			  16


#define INTERFACE_ALM_PORTx_BITMASK (PORT_JABBER_HALT_SET | \
                                     PORT_SYNCHR_LOSS_SET)		// Use these bits to create Data INTERFACE_ALM_PORTx
#define WARN_PORTx_TESTMODE_BITMASK (PORT_TSTPTRN_ENABLE_SET | \
                                     PORT_TX2RX_LOOP_SET    | \
                                     PORT_RTS2CTS_LOOP_SET)		// Use these bits to create Data WARN_PORTx_TESTMODE

//Note: ComPortBitrate and ComPortStatus are only valid if MOD600 is in sync!
typedef struct
{
    long            FPGA_Version;							// FPGA IDR Reg. Bit0-Bit7
    long            ComPortBitrate[N_COMMUNICATION_PORTS];	// FPGA BRR Reg.
    unsigned long   ComPortStatus[N_COMMUNICATION_PORTS];   // see above definitions "PORT_.."							
} t_MUX_Status;

typedef struct
{
	unsigned long	channelsOn;							// Bit0-Bit15: Ch1-Ch16 (0: off, 1: on)

} t_MUX_O4CV_Status;


#define VOICE_TRANSIT_NO_SYNC_BIT_MASK		BIT16;		// used on transit slave only
			
typedef struct
{
	unsigned long	missingRxData;	// bit0-bit15: ch1-ch16 (0: ok, 1: no data)

} t_MUX_VoiceTransitStatus;

// MUX voice transit developer status
#define N_BITS_FRAME_COUNTER	4		// 4 bit counter

typedef struct
{
	unsigned long loopbackOn;
	unsigned long testChannel;	
	unsigned long txFrameDiscardCounter;	
	unsigned long txFrameInsertCounter;	
	unsigned long rxFrameDiscardCounter;
	unsigned long rxFrameInsertCounter;

} t_MUX_VoiceTransitDevStatus;

// MUX voice transit test configuration
typedef struct
{
	unsigned long loopbackOn;
	unsigned long testChannel;

} t_MUX_VoiceTransitTestCfg;


#define MUX_CFG_CONTROL_MUX_ENABLE					BIT0
#define MUX_CFG_CONTROL_PSD_ENABLE					BIT1	// PSD = Port Sharing Device
#define MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT1		BIT2
#define MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT2		BIT3
#define MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT5		BIT4
#define MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT6		BIT5
#define MUX_CFG_CONTROL_PORTEXT_ENABLE				BIT6	// PORTEXT = port interface extension module P1LX
#define MUX_CFG_CONTROL_TRANSIT_MASTER_SLAVE		BIT7	// 0: slave; 1: master														
#define MUX_CFG_CONTROL_P1LY_ENABLE					BIT8	// P1LY extention board (used for R1LB)
#define MUX_CFG_CONTROL_P1LY_PORTEXT_ENABLE			BIT9	// P1LY extention board with additional ports
#define MUX_CFG_CONTROL_LAN_ONLY_MODE				BIT10	// LAN only mode R1LB
#define MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT3		BIT11
#define MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT4		BIT12

// defines for voiceChannel used in struct t_Mux_Cfg
#define VOICE_CH_DR0_MASK		(BIT0  |BIT1  |BIT2  |BIT3)		// values are defined in O4CV_Const.h
#define VOICE_CH_DR1_MASK		(BIT4  |BIT5  |BIT6  |BIT7)
#define VOICE_CH_DR2_MASK		(BIT8  |BIT9  |BIT10 |BIT11)
#define VOICE_CH_DR3_MASK		(BIT12 |BIT13 |BIT14 |BIT15)
#define VOICE_CH_DR4_MASK		(BIT16 |BIT17 |BIT18 |BIT19)
#define VOICE_CH_MODE_MASK		(BIT20 |BIT21)					// values are defined in O4CV_Const.h
#define N_BITS_VOICE_DATA_RATE	4

#define VOICE_LOCAL_TRANSIT_MASK		BIT22							// 0: local; 1: transit
#define VOICE_LOCAL_TRANSIT_SHIFT		22

#define VOICE_TERMINATION_PORT_MASK		(BIT23 |BIT24 |BIT25 |BIT26)	// 0 - 15: port1 - port16
#define VOICE_TERMINATION_PORT_SHIFT	23
#define N_BITS_VOICE_TERMINATION_PORT	4					

typedef struct 
{
	unsigned long				control; // see SWITCHBOX_... definitions

	t_MuxDatarate				rate[N_DATA_RATES];
	t_PortCfg	   			    ports;
	unsigned long				voiceChannel[N_O4CV_CHANNELS];
	long						numberOfVoiceTransitChannels;

	// reserved for future extensions
	long		Reserved1;
	long		Reserved2;
	long		Reserved3;	
} 
t_Mux_Cfg;


//#define	NSK_ALARM_SNR						BIT0	// No usage anymore 
#define	NSK_NOT_SYNC						BIT1	// TRUE: DAPSK not synchronized 
//#define	NSK_ALARM_BUFFER_UNDERFLOW	        BIT2	// No usage anymore
//#define	NSK_ALARM_BUFFER_OVERFLOW	        BIT3	// No usage anymore
#define	NSK_DCD						        BIT4	// TRUE: Data detected
#define	NSK_RTS_FPGA_IN						BIT5	// TRUE: RTS on
#define	NSK_RTS_MODEM_IN					BIT6	// TRUE: RTS on
#define	NSK_ACQUISITION						BIT7	// TRUE: ACQUISITION successful
#define	NSK_CTS								BIT8	// TRUE: CTS on
//#define	NSK_ALARM_SYNC			            BIT9	// No usage anymore
#define	NSK_TESTFLAG5				        BIT10	// TESTFLAG not defined jet
#define	NSK_TESTFLAG6				        BIT11	// TESTFLAG not defined jet
#define	NSK_TESTFLAG7				        BIT12	// TESTFLAG not defined jet
#define	NSK_TESTFLAG8				        BIT13	// TESTFLAG not defined jet
#define	NSK_TESTFLAG9				        BIT14	// TESTFLAG not defined jet
#define	NSK_TESTFLAG10				        BIT15	// TESTFLAG not defined jet

#define NSK_ALARM_MASK						(NSK_ALARM_SNR | NSK_ALARM_SYNC | NSK_ALARM_BUFFER_UNDERFLOW | \
											 NSK_ALARM_BUFFER_OVERFLOW)
#define NSK_WARNING_MASK					(long) 0

typedef struct {

	float			SNRLevel[N_NSK_MODEMS];
	int             counterBufferOverflow[N_NSK_MODEMS];
	int             counterBufferUnderflow[N_NSK_MODEMS];
	long			statusFlags[N_NSK_MODEMS];

} t_NSK_Status;


typedef struct {

	float			errorCriterion[N_NSK_MODEMS];	// Mean-Squared-Error or Nonlineartiy of ad. equalizer
									
	// reserved for futur extensions
	long			reservedL1[N_NSK_MODEMS];

} t_NSK_EqualizerStatus;


typedef struct {

	t_ModemConfig	modem[N_NSK_MODEMS];

} t_NSK_Config;


typedef struct {

	t_ModemEqualizerConfig	equalizer[N_NSK_MODEMS];

} t_NSK_EqualizerCfg;


typedef struct 
{
    float re;
    float im;
} t_complex_float;

/// Developer functions for DAPSK adaptive equalizer
typedef struct {

	unsigned short		modemNumber;	///< modem number	
	unsigned long		aplcChannel;	///< APLC channel number
	unsigned long		datarate;		///< baudrate
	unsigned long		centerFrequency;///< center ferquency 

	float				signalQuality;	///< linear SNR
	float				adaptEquError;	///< abs(NL)^2
	float				adEqLength;		///< ad. EQ filter lenght in taps

	unsigned long		InitAcqFlagCounter;///< counts the fallback from detect to acquisition state
	unsigned long		DCDoffFlagCounter; ///< counts the DCD change from TRUE to FALSE
	long				TestVar; ///< counts the events of processing overload.
	float				NSK_ShortStatus_float[10];
	long				NSK_ShortStatus_long[5];

} t_NSK_ShortStatus;

/// NSK adaptive equalizer transfer function
typedef struct { 

    t_complex_float adEq_h[DAPSK_AD_EQ_LENGTH_MAX]; ///< filter coefficients of adaptive equalizer

} t_NSK_AdapEquTF;

#endif
