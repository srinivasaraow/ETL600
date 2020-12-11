/*-------------------------------------------------------------------------*
 * $Workfile: NSK_FPGA.c $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA functions
 * Copyright    : ABB Switzerland Ltd. 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/NSK_DSP/C/NSK_FPGA.c $
 * 
 * 72    11-09-13 15:53 Chhamaa
 * Piggyback missing alarm bug fixed.
 * 
 * 71    11-08-17 13:42 Chhamaa
 * TransformV24ConfigToPCRValue(): setting of bits DAS and AON modified.
 * 
 * 70    11-06-10 15:58 Chhamaa
 * - PCR ICP bit set for V.24 ports
 * - Port sharing added for V.24 ports 3 and 4
 * 
 * 69    11-05-27 17:28 Chhamaa
 * NSK_FPGA_change_port_bitrates() modified for LAN only mode.
 * 
 * 68    11-05-09 16:46 Chhamaa
 * FPGA configuration for LAN only mode added.
 * 
 * 67    11-04-19 13:17 Chhamaa
 * Test function added: NSK_FPGA_RD_VoiceTransitReg().
 * 
 * 66    11-03-11 18:04 Chhamaa
 * initFPGA() modified for P4LX.
 * 
 * 65    11-02-16 10:41 Chhamaa
 * Modifications for digital transit.
 * 
 * 64    11-01-28 13:49 Chhamaa
 * Version handling for subprint P1LY modified.
 * 
 * 63    11-01-10 10:49 Chhamaa
 * Variable and defines of t_Mux_Cfg renamed.
 * 
 * 62    11-01-06 17:06 Chhamaa
 * O4CV defines renamed.
 * 
 * 61    10-11-24 16:07 Chhamaa
 * LAN alarm and warning added.
 * 
 * 60    10-10-29 14:30 Chhamaa
 * Initialisation of digital voice transit added.
 * 
 * 58    5.05.08 17:50 Chhamaa
 * New bitrate 11 kbps added. 
 * 
 * 56    08-02-25 9:56 Chtognu
 * Merged changes from ad.equ.branch:
 * NSK_FPGA_RD_OSR() and  NSK_FPGA_change_port_bitrates() set to external
 * memory
 * 
 * 55    07-12-17 15:59 Chtognu
 * Reset cases of the NSK_FPGA_change_port_bitrates() fixed
 * 
 * 54    15.11.07 17:05 Chtognu
 * NSK_FPGA_change_port_bitrates() splited: O4CV case, V24 Port case and
 * Reset section
 * 
 * 53    2.11.07 16:09 Chhamaa
 * NSK_FPGA_change_port_bitrates() modified: initialisation of O4CV
 * channels divided into two parts.
 * 
 * 52    22.03.07 15:02 Chhamaa
 * Function getO4CVDataRate() modified. 
 * 
 * 51    8.03.07 11:39 Chalgyg
 * G.703 datarate is now written to the HMI status.
 * 
 * 50    31.01.07 10:13 Chhamaa
 * O4CV status modified.
 * 
 * 49    18.01.07 10:45 Chhamaa
 * O4CV stuff added.
 * 
 * 47    11.10.06 10:37 Chchlee
 * Changed G.703 port handling. When MOD600 is not synchronized then every
 * PCR.POE (port enable) bit has to be reset. to allow a consistent
 * restart after resynchronization.
 *  
 * 
 * 50    9.10.06 12:15 Chmaglo1
 * changed G.703 port handling. When MOD600 is not synchronized then every
 * PCR.POE (port enable) bit has to be reset. to allow a consistent
 * restart after resynchronization.
 * 
 * 49    9.10.06 12:05 Chmaglo1
 * 
 * 48    26.09.06 15:46 Chalgyg
 * CCR.ASY at G.703 port is always 0.
 * 
 * 47    26.09.06 15:10 Chalgyg
 * CCR.CLR bit now depends on V11PORT_CFG_CONTROL_CLK_REGULATOR_ON_BIT
 * instead of V11PORT_CFG_CONTROL_CLK_SOURCE_BIT.
 * 
 * 46    4.09.06 11:24 Chmaglo1
 * Problem: TXC bit was not set correctly
 * Initial RegisterValue was  not set before in
 * TransformV11ConfigToTPRValue
 * 
 * 45    31.08.06 10:32 Chmaglo1
 * second OFDM-configuration replaced with G.703-configuration in initFPGA
 * 
 * 44    17.08.06 14:14 Chalgyg
 * - TPR.TXC bit in FPGA register is supported now.
 * - FPGA clock regulator of V.11 port depends on HMI clock source
 * settings (DCE or DTE).
 * - FPGA clock regulator of LAN port is always disabled.
 * - FPGA clock regulator of G.703 port is always enabled.
 * 
 * 43    6.06.06 9:41 Chalgyg
 * - NSK_FPGA_RD_OSR() can now be optimized.
 * - G.703 LOS alarm is only produced if the port is enabled.
 * 
 * 42    1.06.06 13:35 Chalgyg
 * - LAN port configuration refined.
 * - G.703 port configuration and signaling implemented.
 * - Some functions excluded from optimisation.
 * - Magic numbers replaced by enumeration types.
 * 
 * 40    7.04.06 14:54 Chmibuh
 * R1LA status flag forward added
 * 
 * 39    10.03.06 15:11 Chalgyg
 * LAN port is now supported.
 * The testfunction for 0-series P1LX hardware (Swap V.11 port 2 and LAN)
 * is commented out, so the HMI600 is required to support the LAN
 * interface.
 * 
 * 37    25.10.05 17:25 Chalgyg
 * PCR.AON is not set anymore for V.24 ports because this is now done
 * automatically in the FPGA, if PCR.DAS is set.
 * 
 * 36    19.10.05 15:19 Chchlee
 * NUMBER_OF_V11_PORTS_P* modified
 * 
 * 35    13.09.05 14:01 Chalgyg
 * - Bug #80 removed. No more slips occur in MUX mode.
 * - Testport enables MUX synchronisation delay measurement.
 * 
 * 34    22.08.05 8:29 Chalgyg
 * For V.24, PCR.AON is only set in MUX mode anymore.
 * 
 * 33    14.07.05 7:59 Chalgyg
 * CTS is turned off if port is disabled.
 * 
 * 32    5.07.05 11:08 Chmibuh
 * SubprintMissing = FALSE added
 * 
 * 31    28.06.05 18:46 Chmibuh
 * #include "FPGAHandling.h" is now beneath #include "DIAG.h" therefore
 * BOOL is now known in FPGAHandling.h
 * 
 * 30    23.06.05 16:20 Chalgyg
 * V.24 clock output is disabled forever.
 * 
 * 29    13.06.05 10:20 Chalgyg
 * Magic numbers removed.
 * 
 * 28    3.06.05 16:21 Chmibuh
 * at TransformV24ConfigToTPRValue()
 * [portnumber].control >> 6   changed to [portnumber].control >> 7
 * [portnumber].control >> 10 changed to [portnumber].control >> 11
 * 
 * 27    25.05.05 16:30 Chalgyg
 * Distribute calculation time for OFDM baudrate switching in 3 seperate
 * cycles.
 * 
 * 26    23.05.05 16:27 Chalgyg
 * All V.11 port signals are inverted to allow right indication
 * signalling.
 * 
 * 25    19.05.05 17:48 Chalgyg
 * Bug fixed.
 * 
 * 24    19.05.05 13:04 Chalgyg
 * Verification if P1LX is plugged, when those interfaces are configured
 * is added.
 * 
 * 23    17.05.05 16:15 Chalgyg
 *  Function NSK_FPGA_RD_OSR() improved.
 * 
 * 22    12.05.05 12:02 Chalgyg
 * - MUX bit in FPGA CCR register is only set if port is enabled and
 * baudrate is not 0.
 * - Bug in status assignments removed.
 * - Alarm generation implemented.
 * 
 * 21    2.05.05 10:39 Chalgyg
 * Support of FPGA TPR register.
 * 
 * 20    18.04.05 9:07 Chalgyg
 * Bug in TransformV24ConfigToCCRValue() removed.
 * 
 * 19    15.04.05 8:38 Chchlee
 * "portnumber < N_NSK_MODEMS" added
 * 
 * 18    13.04.05 11:08 Chalgyg
 * CCR Register added to MUX Status.
 * 
 * 17    4.04.05 12:58 Chalgyg
 * V.24 port now takes care of clock recovery configuration. RXC is only
 * enabled if needed or in MUX mode.
 * 
 * 16    1.03.05 15:02 Chalgyg
 * CCR.CLR bit on V.11 ports is only sat if OFDM modem is configured as
 * slave.
 * 
 * 15    1.03.05 11:51 Chalgyg
 * RTS always active in HMI sets PCR.AON and PCR.DAS
 * 
 * 14    18.02.05 11:03 Chalgyg
 * V.11 Clock is disabled, when OFDM modem is out of synchronisation.
 * 
 * 13    17.02.05 13:59 Chtognu
 * tests added
 * 
 * 12    16.02.05 14:19 Chtognu
 * Assert added for wrong datarate
 * 
 * 11    16.02.05 13:02 Chalgyg
 * Assert added for wrong datarate.
 * 
 * 10    16.02.05 12:56 Chalgyg
 * New datarates added.
 * 
 * 9     16.02.05 10:40 Chalgyg
 * FPGA Interface changed to addressbus mode.
 * 
 * 8     10.02.05 11:35 Chalgyg
 * Aggregate clock is only given out if requested from OFDM DSP.
 * 
 * 7     10.02.05 9:50 Chalgyg
 * FPGA status is given to HMI.
 * 
 * 6     9.02.05 9:40 Chalgyg
 * - Bugs Bunny removed.
 * - V.24 PCR.AON commented out, because of Port Sharing Device
 * interference.
 * 
 * 5     8.02.05 13:32 Chalgyg
 * - Handshaking issue corrected.
 * - Made Control/Indication of V.11 ports always on.
 * - Removed V.11 port inversion.
 * 
 * 4     3.02.05 14:13 Chalgyg
 * - Edge issue corrected.
 * - RTS for NSK inverted.
 * 
 * 3     1.02.05 12:00 Chalgyg
 * - NSK_FPGA_WR_RSIG() debugged.
 * - NSK_FPGA_change_port_bitrates() enhanced.
 * - Cosmetics
 * 
 * 2     27.01.05 16:28 Chalgyg
 * Configuration functions enhanced for proper V.11 FPGA configuration.
 * 
 * 18    19.01.05 15:09 Chalgyg
 * - V.11 edge configuration fixed.
 * - HW-Handshaking configuration fixed.
 * 
 * 17    17.01.05 16:31 Chalgyg
 * - V.24 port configuration enhanced. MUX ports don't interfere with NSK
 * ports anymore.
 * - V.11 port configuration added.
 * 
 * 16    12.01.05 13:40 Chalgyg
 * OSR register read added.
 * 
 * 15    12.01.05 10:34 Chalgyg
 * Configuration of FPGA enhanced.
 * 
 * 14    10.01.05 10:34 Chalgyg
 * - OFDM bitrate watching implemented.
 * - Some cosmetics.
 * 
 * 13    7.01.05 14:56 Chtognu
 * functions for new FPGA (05-01-05)
 * 
 * 12    18.10.04 11:44 Gnus01
 * 
 * 11    16.09.04 11:04 Gnus01
 * 
 * 10    26.07.04 16:26 Gnus01
 * 
 * 9     23.07.04 8:33 Gnus01
 * 
 * 8     22.07.04 16:04 Gygax02
 * 
 * 7     20.07.04 11:29 Gnus01
 * 
 * 6     16.07.04 9:17 Gygax02
 * 
 * 5     16.07.04 8:26 Gnus01
 * 
 * 4     9.07.04 15:26 Gygax02
 * 
 * 3     9.07.04 11:11 Gnus01
 * 
 * 2     24.06.04 14:22 Gygax02
 * 
 * 1     21.06.04 11:23 Gygax02
 * Initial version
 *-------------------------------------------------------------------------*/

#include "NSK_FPGA_Types.h"
#include "DIAG.h"
#include "FPGAHandling.h"
#include "Piggyback.h"
#include "O4CV_Const.h"


// Locals
static t_DatarateList	g_DatarateList;
static unsigned long NrOfV24Ports;
static unsigned long NrOfV11Ports;

// Functions
#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformV24ConfigToCCRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr, long portnumber)
{
	unsigned long RegisterValue;

	switch (NSK_DSP_Config_Ptr->muxCfg.ports.V24[portnumber].control & V24PORT_CFG_CONTROL_PORT_MODES)
	{
		case 0x00000000:							// Port disabled
			RegisterValue = FPGA_REGISTER_RESET;
			break;
		case V24PORT_CFG_CONTROL_PORT_MODE_0:		// ADPSK mode
			RegisterValue = FPGA_REGISTER_RESET;
			break;
		case V24PORT_CFG_CONTROL_PORT_MODE_1:		// FSK mode
			RegisterValue = MASK_FSK;
			break;
		case V24PORT_CFG_CONTROL_PORT_MODES:		// MUX mode
			RegisterValue = MASK_CLR;
			break;
		default:
			RegisterValue = FPGA_REGISTER_RESET;
			ASSERT(FALSE);
	}

	RegisterValue |= MASK_ASY | MASK_ICP;		// V.24 ports are always asynchronous | set TVP (pin 9)

	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformV24ConfigToTPRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr, long portnumber)
{
	unsigned long RegisterValue;
	unsigned long Intermediate;

	Intermediate  = (NSK_DSP_Config_Ptr->muxCfg.ports.V24[portnumber].control & V24PORT_CFG_MODE_DATA_BITS) / V24PORT_CFG_MODE_DATA_BITS_BIT0;		// Number of databits
	Intermediate += (NSK_DSP_Config_Ptr->muxCfg.ports.V24[portnumber].control & V24PORT_CFG_MODE_STOP_BITS) / V24PORT_CFG_MODE_STOP_BITS_BIT0;		// + Number of stopbits

	if ((NSK_DSP_Config_Ptr->muxCfg.ports.V24[portnumber].control & V24PORT_CFG_MODE_PARITY_BITS) == 0x00000000)	// No parity bit
	{
		Intermediate--;
	}

	if((Intermediate >= V24PORT_CFG_DATA_BITS_MIN) && (Intermediate <= (V24PORT_CFG_DATA_BITS_MAX+2)))  // Between 5 and 10
	{
		RegisterValue  = Intermediate - 5;		// 5 is the minimum number of bits in the FPGA register.
	}
	else
	{
		ASSERT(FALSE);
	}

	RegisterValue |= MASK_SBD;		// Start bit detection is always enabled.
	
	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformV11ConfigToCCRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr, long portnumber)
{
	unsigned long RegisterValue = FPGA_REGISTER_RESET;

	if (NSK_DSP_Config_Ptr->muxCfg.ports.V11[portnumber].control & V11PORT_CFG_ENABLE == V11PORT_CFG_ENABLE)
	{
		if ((NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_MUX_ENABLE) == MUX_CFG_CONTROL_MUX_ENABLE)
		{
			if ((NSK_DSP_Config_Ptr->muxCfg.ports.V11[portnumber].control & V11PORT_CFG_CONTROL_CLK_REGULATOR_ON_BIT) == V11PORT_CFG_CONTROL_CLK_REGULATOR_ON_BIT)
			{
				RegisterValue =	MASK_CLR;	// Enable clock regulator only if clock from DTE is used.
			}
		}
	}
	
	if ((NSK_DSP_Config_Ptr->muxCfg.ports.V11[portnumber].control & V11PORT_CFG_CONTROL_DOUT_EDGE_BIT) != V11PORT_CFG_CONTROL_DOUT_EDGE_BIT)
	{
		RegisterValue |= MASK_ICP;
	}
	if (((NSK_DSP_Config_Ptr->muxCfg.ports.V11[portnumber].control & V11PORT_CFG_CONTROL_DIN_EDGE_BIT) << 1) != (NSK_DSP_Config_Ptr->muxCfg.ports.V11[portnumber].control & V11PORT_CFG_CONTROL_DOUT_EDGE_BIT))
	{
		RegisterValue |= MASK_ICT;
	}

	RegisterValue |= MASK_IAC;				// Port is always inverted.

	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformV11ConfigToTPRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr, long portnumber)
{
	unsigned long RegisterValue = FPGA_REGISTER_RESET;

	if ((NSK_DSP_Config_Ptr->muxCfg.ports.V11[portnumber].control & V11PORT_CFG_CONTROL_CLK_SOURCE_BIT) == V11PORT_CFG_CONTROL_CLK_SOURCE_BIT)
	{
		RegisterValue =	MASK_TXC;		// Switch V.11 signaling input 'Control' to clock input for codirectional use.
	}

	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformLANConfigToCCRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr)
{
	unsigned long RegisterValue = FPGA_REGISTER_RESET;

	if ((NSK_DSP_Config_Ptr->muxCfg.ports.LAN.control & LAN_PORT_CFG_CONTROL_DOUT_EDGE_BIT) != LAN_PORT_CFG_CONTROL_DOUT_EDGE_BIT)
	{
		RegisterValue |= MASK_ICP;
	}
	if (((NSK_DSP_Config_Ptr->muxCfg.ports.LAN.control & LAN_PORT_CFG_CONTROL_DIN_EDGE_BIT) << 1) != (NSK_DSP_Config_Ptr->muxCfg.ports.LAN.control & LAN_PORT_CFG_CONTROL_DOUT_EDGE_BIT))
	{
		RegisterValue |= MASK_ICT;
	}

	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformLANConfigToPCRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr)
{
	unsigned long RegisterValue = FPGA_REGISTER_RESET;

	if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_LAN_ONLY_MODE)
	{
		RegisterValue |= MASK_LOM;
	}

	return RegisterValue;
}
#pragma optimize_as_cmd_line

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformV24ConfigToPCRValue(t_NSK_DSP_Config* NSK_DSP_Config_Ptr, long portnumber)
{
	unsigned long RegisterValue = FPGA_REGISTER_RESET;

	if (NSK_DSP_Config_Ptr->muxCfg.ports.V24[portnumber].control & V24PORT_CFG_CONTROL_ENABLE_JABBER_CTRL)
	{
		RegisterValue = MASK_JAC;		// PCR.JAC
	}

	// HMI600 mode		FPGA PCR bits
	//					DAS		AON
	// CTS always on	0		1
	// Data sensitive   1       0
	// HW handshaking   0       0

	switch (NSK_DSP_Config_Ptr->muxCfg.ports.V24[portnumber].control & V24PORT_CFG_CONTROL_HW_HANDSHAKING_MASK)
	{
		// CTS always on
		case 0:
			RegisterValue |= MASK_AON;
			break;

		// Data sensitive
		case V24PORT_CFG_CONTROL_HW_HANDSHAKING_0:
			RegisterValue |= MASK_DAS;
			break;

		// HW handshaking
		default:
			break;
	}

	switch (portnumber)
	{
		case 0 :
			if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT1)
			{
				RegisterValue |= MASK_SHD;	// PCR.SHD
			}
			break;
		case 1 :
			if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT2)
			{
				RegisterValue |= MASK_SHD;	// PCR.SHD
			}
			break;
		case 2 :
			if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT3)
			{
				RegisterValue |= MASK_SHD;	// PCR.SHD
			}
			break;
		case 3 :
			if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT4)
			{
				RegisterValue |= MASK_SHD;	// PCR.SHD
			}
			break;
		case 4 :
			if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT5)
			{
				RegisterValue |= MASK_SHD;	// PCR.SHD
			}
			break;
		case 5 :
			if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_SWITCHBOX_ENABLE_PORT6)
			{
				RegisterValue |= MASK_SHD;	// PCR.SHD
			}
			break;
		default:
			RegisterValue &= ~MASK_SHD;		// PCR.SHD
	}
	
	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE TransformRateToFPGARegisterValue(long Bitrate)
{
	unsigned long RegisterValue;

	switch (Bitrate)
	{
		case 0 :
			RegisterValue = BRT_1200;
			break;
		case 50 : 
			RegisterValue = BRT_50;
			break;
		case 100 : 
			RegisterValue = BRT_100;
			break;
		case 200 : 
			RegisterValue = BRT_200;
			break;
		case 300 : 
			RegisterValue = BRT_300;
			break;
		case 600 : 
			RegisterValue = BRT_600;
			break;
		case 1200 : 
			RegisterValue = BRT_1200;
			break;
		case 2400 : 
			RegisterValue = BRT_2400;
			break;
		case 4800 : 
			RegisterValue = BRT_4800;
			break;
		case 9600 : 
			RegisterValue = BRT_9600;
			break;
		case 11000 :
			RegisterValue = BRT_11000;
			break;
		case 12000 : 
			RegisterValue = BRT_12000;
			break;
		case 14400 : 
			RegisterValue = BRT_14400;
			break;
		case 16000 : 
			RegisterValue = BRT_16000;
			break;
		case 19200 : 
			RegisterValue = BRT_19200;
			break;
		case 24000 : 
			RegisterValue = BRT_24000;
			break;
		case 28800 : 
			RegisterValue = BRT_28800;
			break;
		case 32000 : 
			RegisterValue = BRT_32000;
			break;
		case 36000 : 
			RegisterValue = BRT_36000;
			break;
		case 38400 : 
			RegisterValue = BRT_38400;
			break;
		case 40000 : 
			RegisterValue = BRT_40000;
			break;
		case 48000 : 
			RegisterValue = BRT_48000;
			break;
		case 56000 : 
			RegisterValue = BRT_56000;
			break;
		case 64000 : 
			RegisterValue = BRT_64000;
			break;
		case 72000 : 
			RegisterValue = BRT_72000;
			break;
		case 76800 : 
			RegisterValue = BRT_76800;
			break;
		case 80000 : 
			RegisterValue = BRT_80000;
			break;
		case 96000 : 
			RegisterValue = BRT_96000;
			break;
		case 112000 : 
			RegisterValue = BRT_112000;
			break;
		case 128000 : 
			RegisterValue = BRT_128000;
			break;
		case 144000 : 
			RegisterValue = BRT_144000;
			break;
		case 153600 : 
			RegisterValue = BRT_153600;
			break;
		case 160000 : 
			RegisterValue = BRT_160000;
			break;
		case 192000 : 
			RegisterValue = BRT_192000;
			break;
		case 224000 : 
			RegisterValue = BRT_224000;
			break;
		case 256000 : 
			RegisterValue = BRT_256000;
			break;
		case 288000 : 
			RegisterValue = BRT_288000;
			break;
		case 307200 : 
			RegisterValue = BRT_307200;
			break;
		case 320000 : 
			RegisterValue = BRT_320000;
			break;
		default:
			ASSERT(FALSE);						
	}
	return RegisterValue;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
static long SEGMENT_EXT_CODE getO4CVDataRate(unsigned long chCfg, int dataRateIx)
{
	long dataRate;
	long muxDataRate;
	unsigned long dataRateMask = MASK_4LSB;

	// set bit mask for data rate
	dataRateMask = MASK_4LSB << N_BITS_VOICE_DATA_RATE*dataRateIx;

	// get data rate
	dataRate = (chCfg & dataRateMask) >> N_BITS_VOICE_DATA_RATE*dataRateIx;

	// select mux data rate
	switch(dataRate)
	{
		case CH_DR_OFF:			muxDataRate = BRT_O4CV_OFF;	 break;
		case CH_VOICE_DR_5300:	muxDataRate = BRT_O4CV_5300; break;
		case CH_VOICE_DR_6300:	muxDataRate = BRT_O4CV_6300; break;

		default: muxDataRate = BRT_O4CV_OFF;
	}

	return(muxDataRate);
}
#pragma optimize_as_cmd_line

#pragma optimize_off
static unsigned long SEGMENT_EXT_CODE getVoiceChEn(unsigned long chCfg)
{
	if (chCfg & VOICE_CH_MODE_MASK) 
		return(1);
	else
		return(0);
}
#pragma optimize_as_cmd_line

#pragma optimize_off
static unsigned long SEGMENT_EXT_CODE getLocalTransit(unsigned long chCfg)
{
	return((chCfg & VOICE_LOCAL_TRANSIT_MASK) >> VOICE_LOCAL_TRANSIT_SHIFT);
}
#pragma optimize_as_cmd_line

#pragma optimize_off
static unsigned long SEGMENT_EXT_CODE getTerminationPort(unsigned long chCfg)
{
	return((chCfg & VOICE_TERMINATION_PORT_MASK) >> VOICE_TERMINATION_PORT_SHIFT);
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void NSK_FPGA_RD_TUDn(t_FPGA_Registers* FPGA_Registers_Ptr,t_FPGA_Registers* FPGA_Address_Ptr)
{
	unsigned long i;

	for (i=0; i<N_NSK_PORTS; i++)
	{
		FPGA_Registers_Ptr->TUD[i]	= (*(long*)(FPGA_Address_Ptr->TUD[i])) & 0xFF;
	}
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
void NSK_FPGA_RD_TSIG(t_FPGA_NSK_RTS *writedata,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr)
{
	unsigned long i;
	
	FPGA_Registers_Ptr->TSIG = (*(long*)(FPGA_Address_Ptr->TSIG)) & 0xFF;

	for (i=0; i<N_NSK_PORTS; i++)
	{
		writedata->TXD_RTS[i] = (~(FPGA_Registers_Ptr->TSIG >> i)) & BIT0;
	}
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
void NSK_FPGA_WR_RUDall(t_FPGA_Registers* FPGA_Registers_Ptr,t_FPGA_Registers* FPGA_Address_Ptr)
{
	unsigned long i;
	
	for (i=0; i<N_NSK_PORTS; i++)
	{
		*(long*)(FPGA_Address_Ptr->RUD[i]) = FPGA_Registers_Ptr->RUD[i];
	}
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
void NSK_FPGA_WR_RSIG(t_FPGA_NSK_RSIG* Data_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,t_FPGA_Registers* FPGA_Address_Ptr)
{
	unsigned long writeFPGA = 0;

    writeFPGA =	                   (BIT0 & Data_Ptr->TXD_CTS[3]);
    writeFPGA = (writeFPGA << 1) | (BIT0 & Data_Ptr->TXD_CTS[2]);
    writeFPGA = (writeFPGA << 1) | (BIT0 & Data_Ptr->TXD_CTS[1]);
    writeFPGA = (writeFPGA << 1) | (BIT0 & Data_Ptr->TXD_CTS[0]);
    
	FPGA_Registers_Ptr->RSIG = (~writeFPGA) & RSIG_MASK;
	*(long*)(FPGA_Address_Ptr->RSIG) =  FPGA_Registers_Ptr->RSIG;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
void NSK_FPGA_WR_PCR(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_FPGA_NSK_PCR* Data_Ptr,t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	unsigned long i;

	for (i=0; i<NrOfV24Ports; i++)
	{
		if ((FPGA_Registers_Ptr->CCR[V24_Port_1+i] & MASK_MUX) != MASK_MUX)		// This V.24 port is not configured for MUX600
		{
			FPGA_Registers_Ptr->PCR[V24_Port_1+i] &= ~MASK_POE;
			if ((FPGA_Registers_Ptr->PCR[V24_Port_1+i] & MASK_SHD) == MASK_SHD)	// This V.24 port is configured as Sharing Device
			{
				FPGA_Registers_Ptr->PCR[V24_Port_1+i] |= Data_Ptr->POE[0];
			}
			else if (i < N_NSK_PORTS)											// This V.24 port is not configured as Sharing Device and neither port 5 nor port 6
			{
				FPGA_Registers_Ptr->PCR[V24_Port_1+i] |= Data_Ptr->POE[i];
			}
			*(long*)(FPGA_Address_Ptr->PCR[V24_Port_1+i]) = FPGA_Registers_Ptr->PCR[V24_Port_1+i];
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] &= ~PORT_PCR_MASK;
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] |= (FPGA_Registers_Ptr->PCR[V24_Port_1+i] << PORT_PCR_SHIFT) & PORT_PCR_MASK;
		}
	}
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_RD_OSR(t_NSK_DSP_Config* NSK_DSP_Config_Ptr,t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_Alarm* nskAlarm_Ptr,BOOL* nskJabberHalt_Ptr,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,long OSR_Counter)
//void NSK_FPGA_RD_OSR(t_NSK_DSP_Config* NSK_DSP_Config_Ptr,t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_Alarm* nskAlarm_Ptr,BOOL* nskJabberHalt_Ptr,t_FPGA_Registers* FPGA_Address_Ptr,t_FPGA_Registers* FPGA_Registers_Ptr,long OSR_Counter)

{
	FPGA_Registers_Ptr->OSR[OSR_Counter] = (*(long*)(FPGA_Address_Ptr->OSR[OSR_Counter])) & 0xFF;

	if (OSR_Counter == OFDM_Port)		// Get information from aggregate port.
	{
		if ((NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_MUX_ENABLE) == MUX_CFG_CONTROL_MUX_ENABLE)
		{
			if (FPGA_Registers_Ptr->OSR[OSR_Counter] & AGGREGATE_PORT_LOS_STATUS_FLAG)
			{
				nskAlarm_Ptr->muxLinkAlarm = TRUE;
			}
			else
			{
				nskAlarm_Ptr->muxLinkAlarm = FALSE;
			}
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[OSR_Counter] &= ~PORT_OSR_MASK;
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[OSR_Counter] |= FPGA_Registers_Ptr->OSR[OSR_Counter];
		}
		else
		{
			nskAlarm_Ptr->muxLinkAlarm = FALSE;
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[OSR_Counter] &= ~PORT_OSR_MASK;
		}
	}
	else if (OSR_Counter <= V24_Port_6)	// Get information from all V.24 ports.
	{
		if (NSK_DSP_Config_Ptr->muxCfg.ports.V24[OSR_Counter-1].control & V24PORT_CFG_CONTROL_PORT_MODES) // Port is enabled.
		{
			*(nskJabberHalt_Ptr+OSR_Counter-1) = FPGA_Registers_Ptr->OSR[OSR_Counter] & 0x01;
		}
		else																							  // Port is disabled.
		{
			*(nskJabberHalt_Ptr+OSR_Counter-1) = FALSE;
		}
	}
	else if (OSR_Counter == G703_Port)	// Get information from G.703 port.
	{
		if (NSK_DSP_Config_Ptr->muxCfg.ports.G703.control & G703_PORT_CFG_ENABLE)		// Port is enabled
		{
			if (FPGA_Registers_Ptr->OSR[OSR_Counter] & G703_LOS_STATUS_FLAG)
			{
				nskAlarm_Ptr->G703LOSAlarm = TRUE;
			}
			else
			{
				nskAlarm_Ptr->G703LOSAlarm = FALSE;
			}
		}
		else																			// Port is disabled
		{
			nskAlarm_Ptr->G703LOSAlarm = FALSE;
		}
	}
	else if (OSR_Counter == LAN_Port)	// Get information from LAN port.
	{
		// Forward the state of the LAN flags in any case - independent of the configuration. 
		// The Master DSP should decide what's to do with these information. Another reason is that
		// the Master DSP knows the complete configuration.
		if (FPGA_Registers_Ptr->OSR[OSR_Counter] & LAN_NOT_BOOTED)
		{
			nskAlarm_Ptr->LANNotBooted = TRUE;
		}
		else
		{
			nskAlarm_Ptr->LANNotBooted = FALSE;
		}

		if (FPGA_Registers_Ptr->OSR[OSR_Counter] & LAN_ALARM)
		{
			nskAlarm_Ptr->LANAlarm = TRUE;
		}
		else
		{
			nskAlarm_Ptr->LANAlarm = FALSE;
		}

		if (FPGA_Registers_Ptr->OSR[OSR_Counter] & LAN_WARNING)
		{
			nskAlarm_Ptr->LANWarning = TRUE;
		}
		else
		{
			nskAlarm_Ptr->LANWarning = FALSE;
		}
	}
}
#pragma optimize_as_cmd_line

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE NSK_FPGA_RD_GDTC_CCS(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	FPGA_Registers_Ptr->GDTC = (*(long*)(FPGA_Address_Ptr->GDTC)) & 0xFF;
	return(FPGA_Registers_Ptr->GDTC >> CCS_SHIFT);
}
#pragma optimize_as_cmd_line

#pragma optimize_off
unsigned long SEGMENT_EXT_CODE NSK_FPGA_RD_GDTC_LVC(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	FPGA_Registers_Ptr->GDTC = (*(long*)(FPGA_Address_Ptr->GDTC)) & 0xFF;

	if (FPGA_Registers_Ptr->GDTC & MASK_GDTC_LVC)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	};
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_WR_GDTC_CCS(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, long channel)
{
	FPGA_Registers_Ptr->GDTC = FPGA_Registers_Ptr->GDTC & ~MASK_GDTC_CCS;
	FPGA_Registers_Ptr->GDTC = FPGA_Registers_Ptr->GDTC | ((channel & 0xF) << CCS_SHIFT);
	(*(long*)(FPGA_Address_Ptr->GDTC)) = FPGA_Registers_Ptr->GDTC;
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_resetTransitCounters(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	// reset counters
	FPGA_Registers_Ptr->GDTC = FPGA_Registers_Ptr->GDTC | MASK_GDTC_RDC | MASK_GDTC_RIC;
	(*(long*)(FPGA_Address_Ptr->GDTC)) = FPGA_Registers_Ptr->GDTC;

	// start counters
	FPGA_Registers_Ptr->GDTC = FPGA_Registers_Ptr->GDTC & ~MASK_GDTC_RDC & ~MASK_GDTC_RIC;
	(*(long*)(FPGA_Address_Ptr->GDTC)) = FPGA_Registers_Ptr->GDTC;
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_RD_transitCounters(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	FPGA_Registers_Ptr->FDCT = (*(long*)(FPGA_Address_Ptr->FDCT)) & 0xFF;
	FPGA_Registers_Ptr->FICT = (*(long*)(FPGA_Address_Ptr->FICT)) & 0xFF;
	FPGA_Registers_Ptr->FDCR = (*(long*)(FPGA_Address_Ptr->FDCR)) & 0xFF;
	FPGA_Registers_Ptr->FICR = (*(long*)(FPGA_Address_Ptr->FICR)) & 0xFF;
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_enableTransitLoopback(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	// set LVC bit
	FPGA_Registers_Ptr->GDTC = FPGA_Registers_Ptr->GDTC | MASK_GDTC_LVC;
	(*(long*)(FPGA_Address_Ptr->GDTC)) = FPGA_Registers_Ptr->GDTC;

}
#pragma optimize_as_cmd_line

#pragma optimize_off

void SEGMENT_EXT_CODE NSK_FPGA_disableTransitLoopback(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	// clear LVC bit
	FPGA_Registers_Ptr->GDTC = FPGA_Registers_Ptr->GDTC & ~MASK_GDTC_LVC;
	(*(long*)(FPGA_Address_Ptr->GDTC)) = FPGA_Registers_Ptr->GDTC;
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_RD_GDTS(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, t_NSK_Alarm* nskAlarm_Ptr)
{
	FPGA_Registers_Ptr->GDTS = (*(long*)(FPGA_Address_Ptr->GDTS)) & 0xFF;

	if (FPGA_Registers_Ptr->GDTS)
	{
		nskAlarm_Ptr->transitNoSync = TRUE;
	}
	else
	{
		nskAlarm_Ptr->transitNoSync = FALSE;
	}
}
#pragma optimize_as_cmd_line

#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_RD_TND(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, t_NSK_Alarm* nskAlarm_Ptr, t_NSK_DSP_Status* NSK_DSP_Status_Ptr)
{
	FPGA_Registers_Ptr->TND[0] = (*(long*)(FPGA_Address_Ptr->TND[0])) & 0xFF;	// ch 0...7
	FPGA_Registers_Ptr->TND[1] = (*(long*)(FPGA_Address_Ptr->TND[1])) & 0xFF;	// ch 8...15

	if (FPGA_Registers_Ptr->TND[0] || FPGA_Registers_Ptr->TND[1])
	{
		nskAlarm_Ptr->transitMissingRxData = TRUE;
	}
	else
	{
		nskAlarm_Ptr->transitMissingRxData = FALSE;
	}

	NSK_DSP_Status_Ptr->MUX_VoiceTransit.missingRxData = FPGA_Registers_Ptr->TND[0];
	NSK_DSP_Status_Ptr->MUX_VoiceTransit.missingRxData |= FPGA_Registers_Ptr->TND[1] << SB8;

}
#pragma optimize_as_cmd_line


#pragma optimize_off
void SEGMENT_EXT_CODE NSK_FPGA_RD_VoiceTransitReg(t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	unsigned long i;

	for (i=0; i<N_O4CV_CHANNELS; i++)
	{
		FPGA_Registers_Ptr->TCR[i] = (*(long*)(FPGA_Address_Ptr->TCR[i])) & 0xFF;
	};

	//FPGA_Registers_Ptr->SPA = (*(long*)(FPGA_Address_Ptr->SPA)) & 0xFF;	// not used
	FPGA_Registers_Ptr->GDTC = (*(long*)(FPGA_Address_Ptr->GDTC)) & 0xFF;
	FPGA_Registers_Ptr->VCE[0] = (*(long*)(FPGA_Address_Ptr->VCE[0])) & 0xFF;
	FPGA_Registers_Ptr->VCE[1] = (*(long*)(FPGA_Address_Ptr->VCE[1])) & 0xFF;

	FPGA_Registers_Ptr->VCTL[0] = (*(long*)(FPGA_Address_Ptr->VCTL[0])) & 0xFF;
	FPGA_Registers_Ptr->VCTL[1] = (*(long*)(FPGA_Address_Ptr->VCTL[1])) & 0xFF;

	for (i=0; i<N_O4CV_CHANNELS/2; i++)
	{
		FPGA_Registers_Ptr->VCTP[i] = (*(long*)(FPGA_Address_Ptr->VCTP[i])) & 0xFF;
	}

	FPGA_Registers_Ptr->GDTS = (*(long*)(FPGA_Address_Ptr->GDTS)) & 0xFF;

	FPGA_Registers_Ptr->TND[0] = (*(long*)(FPGA_Address_Ptr->TND[0])) & 0xFF;	// ch 0...7
	FPGA_Registers_Ptr->TND[1] = (*(long*)(FPGA_Address_Ptr->TND[1])) & 0xFF;	// ch 8...15

	FPGA_Registers_Ptr->FDCT = (*(long*)(FPGA_Address_Ptr->FDCT)) & 0xFF;
	FPGA_Registers_Ptr->FICT = (*(long*)(FPGA_Address_Ptr->FICT)) & 0xFF;
	FPGA_Registers_Ptr->FDCR = (*(long*)(FPGA_Address_Ptr->FDCR)) & 0xFF;
	FPGA_Registers_Ptr->FICR = (*(long*)(FPGA_Address_Ptr->FICR)) & 0xFF;

}
#pragma optimize_as_cmd_line


#pragma optimize_off
void SEGMENT_EXT_CODE GenerateFPGAAdressRange(t_FPGA_Registers* FPGA_Address_Ptr)
{
	unsigned long i;

	FPGA_Address_Ptr->IDNT = 0x0C000000;
	FPGA_Address_Ptr->TSIG = 0x0C000001;
	FPGA_Address_Ptr->RSIG = 0x0C00000A;

	for (i=0; i<N_NSK_PORTS; i++)
	{
		FPGA_Address_Ptr->TUD[i] = 0x0C000002+i;
		FPGA_Address_Ptr->RUD[i] = 0x0C000006+i;
	} 

	for (i=0; i<N_MUX_PORTS; i++)
	{
		FPGA_Address_Ptr->PCR[i] = (0x0C000014+(i*5));
		FPGA_Address_Ptr->CCR[i] = (0x0C000015+(i*5));
		FPGA_Address_Ptr->TPR[i] = (0x0C000016+(i*5));
		FPGA_Address_Ptr->BRR[i] = (0x0C000017+(i*5));
		FPGA_Address_Ptr->OSR[i] = (0x0C000018+(i*5));
	}
	
	for (i=0; i<N_O4CV_CHANNELS; i++)
	{
		FPGA_Address_Ptr->TCR[i] = (0x0C000050+i);
	}

	// subprint assembly register (not used)
	FPGA_Address_Ptr->SPA = 0x0C000060;
	
	// digital transit registers
	FPGA_Address_Ptr->GDTC	  = 0x0C000061;
	FPGA_Address_Ptr->VCE[0]  = 0x0C000062;
	FPGA_Address_Ptr->VCE[1]  = 0x0C000063;
	FPGA_Address_Ptr->VCTL[0] = 0x0C000064;
	FPGA_Address_Ptr->VCTL[1] = 0x0C000065;

	for (i=0; i<N_O4CV_CHANNELS/2; i++)
	{
		FPGA_Address_Ptr->VCTP[i] = 0x0C000066+i;
	}
	
	FPGA_Address_Ptr->GDTS	 = 0x0C00006E;
	FPGA_Address_Ptr->TND[0] = 0x0C00006F;
	FPGA_Address_Ptr->TND[1] = 0x0C000070;
	
	FPGA_Address_Ptr->FDCT = 0x0C000071;
	FPGA_Address_Ptr->FICT = 0x0C000072;
	FPGA_Address_Ptr->FDCR = 0x0C000073;
	FPGA_Address_Ptr->FICR = 0x0C000074;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
void SEGMENT_EXT_CODE GenerateFPGADatarateLists(t_NSK_DSP_Config* NSK_DSP_Config_Ptr)
{
	unsigned long i;
	unsigned long j;

	for (i=0; i<N_DATA_RATES; i++)
	{
		g_DatarateList.ListOfAggregateDatarates[i] = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->ofdmCfg.dataRate[i]);
		g_DatarateList.ListOfV11PortDatarates[0][i]  = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->muxCfg.rate[i].V11_Rate[0]);
		g_DatarateList.ListOfV11PortDatarates[1][i]  = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->muxCfg.rate[i].V11_Rate[1]);
		g_DatarateList.ListOfLANPortDatarates[i]  = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->muxCfg.rate[i].LAN_Rate);

		for (j=0; j<NrOfV24Ports; j++)
		{
			g_DatarateList.ListOfV24PortEnables[j][i]  = (NSK_DSP_Config_Ptr->muxCfg.rate[i].portEnable & (BIT0 << j))  >> j;
		}

		g_DatarateList.ListOfG703PortEnables[i]  = (NSK_DSP_Config_Ptr->muxCfg.rate[i].portEnable & MUX_DATARATE_PORT_ENABLE_G703) >> G703_SHIFT;

		// set O4CV data rates
		for (j=0; j<N_O4CV_CHANNELS; j++)
		{
			g_DatarateList.ListOfO4CVChDatarates[j][i] = getO4CVDataRate(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[j], i);
		}
	}
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
BOOL SEGMENT_EXT_CODE initFPGA(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr, t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr)
{
	unsigned long i;
	unsigned long SPId;
	BOOL SubprintMissing = FALSE;

	SPId = ReadSubprintID();
	
	if ((SPId == PIGGYBACK_HW_ID_0) ||	// P1LX
		(SPId == PIGGYBACK_HW_ID_1))	// P1LXa
	{
		NrOfV11Ports = NUMBER_OF_V11_PORTS_P4LU;
		NrOfV24Ports = NUMBER_OF_V24_PORTS_P4LU;
	}
	else if (SPId == PIGGYBACK_HW_ID_2) // P1LY (rubric 1)
	{
		NrOfV11Ports = NUMBER_OF_V11_PORTS_P4LV;
		NrOfV24Ports = NUMBER_OF_V24_PORTS_P4LV;
	}
	else if (SPId == PIGGYBACK_HW_ID_3) // P1LY extended (rubric 2)
	{
		NrOfV11Ports = NUMBER_OF_V11_PORTS_P4LX;
		NrOfV24Ports = NUMBER_OF_V24_PORTS_P4LX;
	}
	else	// no subprint
	{
		NrOfV11Ports = NUMBER_OF_V11_PORTS_P4LT;
		NrOfV24Ports = NUMBER_OF_V24_PORTS_P4LT;
		if ((NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_PORTEXT_ENABLE) ||
			(NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_P1LY_ENABLE)	  ||
			(NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_P1LY_PORTEXT_ENABLE))
		{
			SubprintMissing = TRUE;
		}
	}

	GenerateFPGAAdressRange(FPGA_Address_Ptr);
	GenerateFPGADatarateLists(NSK_DSP_Config_Ptr);
	
	// Read FPGA Version Number
	FPGA_Registers_Ptr->IDNT = (*(long*)(FPGA_Address_Ptr->IDNT)) & 0xFF;
	NSK_DSP_Status_Ptr->MUX.FPGA_Version = FPGA_Registers_Ptr->IDNT;

	// Write subprint ID to FPGA (not used)
	FPGA_Registers_Ptr->SPA = SPId;
	//*(long*)(FPGA_Address_Ptr->SPA) = FPGA_Registers_Ptr->SPA;

	// Write Configuration to FPGA register structure
	// OFDM Port
	FPGA_Registers_Ptr->BRR[OFDM_Port] = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->ofdmCfg.dataRate[INITIAL_STATE]);
	FPGA_Registers_Ptr->CCR[OFDM_Port] = FPGA_REGISTER_RESET;
	FPGA_Registers_Ptr->TPR[OFDM_Port] = FPGA_REGISTER_RESET;
	FPGA_Registers_Ptr->PCR[OFDM_Port] = MASK_AON;		// Indication always on
	NSK_DSP_Status_Ptr->MUX.ComPortStatus[OFDM_Port] = ((FPGA_Registers_Ptr->CCR[OFDM_Port] << PORT_CCR_SHIFT) & PORT_CCR_MASK) | ((FPGA_Registers_Ptr->PCR[OFDM_Port] << PORT_PCR_SHIFT) & PORT_PCR_MASK);

	// All V.24 Ports
	for (i=0; i<NrOfV24Ports; i++)
	{
		if (NSK_DSP_Config_Ptr->muxCfg.ports.V24[i].control & V24PORT_CFG_CONTROL_PORT_MODES)	// Port is enabled.
		{
			FPGA_Registers_Ptr->BRR[V24_Port_1+i] = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->muxCfg.ports.V24[i].datarate);
			NSK_DSP_Status_Ptr->MUX.ComPortBitrate[V24_Port_1+i] = NSK_DSP_Config_Ptr->muxCfg.ports.V24[i].datarate;
			FPGA_Registers_Ptr->CCR[V24_Port_1+i] = TransformV24ConfigToCCRValue(NSK_DSP_Config_Ptr,i);
			FPGA_Registers_Ptr->TPR[V24_Port_1+i] = TransformV24ConfigToTPRValue(NSK_DSP_Config_Ptr,i);
			FPGA_Registers_Ptr->PCR[V24_Port_1+i] = TransformV24ConfigToPCRValue(NSK_DSP_Config_Ptr,i);
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] = ((FPGA_Registers_Ptr->CCR[V24_Port_1+i] << PORT_CCR_SHIFT) & PORT_CCR_MASK) | ((FPGA_Registers_Ptr->PCR[V24_Port_1+i] << PORT_PCR_SHIFT) & PORT_PCR_MASK);
		}
		else
		{
			FPGA_Registers_Ptr->BRR[V24_Port_1+i] = FPGA_REGISTER_RESET;
			NSK_DSP_Status_Ptr->MUX.ComPortBitrate[V24_Port_1+i] = INITIAL_STATE;
			FPGA_Registers_Ptr->CCR[V24_Port_1+i] = FPGA_REGISTER_RESET;
			FPGA_Registers_Ptr->TPR[V24_Port_1+i] = FPGA_REGISTER_RESET;
			FPGA_Registers_Ptr->PCR[V24_Port_1+i] = FPGA_REGISTER_RESET;
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] = INITIAL_STATE;
		}
	}

	// All V.11 Ports
	for (i=0; i<NrOfV11Ports; i++)
	{
		if (NSK_DSP_Config_Ptr->muxCfg.ports.V11[i].control & V11PORT_CFG_ENABLE)	// Port is enabled.
		{
			FPGA_Registers_Ptr->BRR[V11_Port_1+i] = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->muxCfg.rate[INITIAL_STATE].V11_Rate[i]);
			FPGA_Registers_Ptr->CCR[V11_Port_1+i] = TransformV11ConfigToCCRValue(NSK_DSP_Config_Ptr,i);
			FPGA_Registers_Ptr->TPR[V11_Port_1+i] = TransformV11ConfigToTPRValue(NSK_DSP_Config_Ptr,i);
			FPGA_Registers_Ptr->PCR[V11_Port_1+i] = MASK_AON;		// Control always on
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_1+i] = ((FPGA_Registers_Ptr->CCR[V11_Port_1+i] << PORT_CCR_SHIFT) & PORT_CCR_MASK) | ((FPGA_Registers_Ptr->PCR[V11_Port_1+i] << PORT_PCR_SHIFT) & PORT_PCR_MASK);
		}
		else
		{
			FPGA_Registers_Ptr->BRR[V11_Port_1+i] = FPGA_REGISTER_RESET;
			FPGA_Registers_Ptr->CCR[V11_Port_1+i] = FPGA_REGISTER_RESET;
			FPGA_Registers_Ptr->TPR[V11_Port_1+i] = FPGA_REGISTER_RESET;
			FPGA_Registers_Ptr->PCR[V11_Port_1+i] = FPGA_REGISTER_RESET;
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_1+i] = INITIAL_STATE;
		}
	}

	// LAN Port
	if (NSK_DSP_Config_Ptr->muxCfg.ports.LAN.control & LAN_PORT_CFG_ENABLE)	// Port is enabled.
	{
		FPGA_Registers_Ptr->BRR[LAN_Port] = TransformRateToFPGARegisterValue(NSK_DSP_Config_Ptr->muxCfg.rate[INITIAL_STATE].LAN_Rate);
		FPGA_Registers_Ptr->CCR[LAN_Port] = TransformLANConfigToCCRValue(NSK_DSP_Config_Ptr);
		FPGA_Registers_Ptr->TPR[LAN_Port] = FPGA_REGISTER_RESET;
		FPGA_Registers_Ptr->PCR[LAN_Port] = TransformLANConfigToPCRValue(NSK_DSP_Config_Ptr);
		NSK_DSP_Status_Ptr->MUX.ComPortStatus[LAN_Port] = ((FPGA_Registers_Ptr->CCR[LAN_Port] << PORT_CCR_SHIFT) & PORT_CCR_MASK) | ((FPGA_Registers_Ptr->PCR[LAN_Port] << PORT_PCR_SHIFT) & PORT_PCR_MASK);
	}
	else
	{
		FPGA_Registers_Ptr->BRR[LAN_Port] = FPGA_REGISTER_RESET;
		FPGA_Registers_Ptr->CCR[LAN_Port] = FPGA_REGISTER_RESET;
		FPGA_Registers_Ptr->TPR[LAN_Port] = FPGA_REGISTER_RESET;
		FPGA_Registers_Ptr->PCR[LAN_Port] = FPGA_REGISTER_RESET;
		NSK_DSP_Status_Ptr->MUX.ComPortStatus[LAN_Port] = INITIAL_STATE;
	}

	// G.703 Port
	if (NSK_DSP_Config_Ptr->muxCfg.ports.G703.control & G703_PORT_CFG_ENABLE)	// Port is enabled.
	{
		FPGA_Registers_Ptr->BRR[G703_Port] = BRT_64000;							// G.703 port is fixed to 64 kbps.
		NSK_DSP_Status_Ptr->MUX.ComPortBitrate[G703_Port] = 64000;
		FPGA_Registers_Ptr->CCR[G703_Port] = MASK_CLR | MASK_RXC;	// Clock regulator is enabled and RxC output always enabled.
		FPGA_Registers_Ptr->TPR[G703_Port] = MASK_CHL0 | MASK_CHL1 | MASK_TXC;	// G.703 port is fixed to 8 units between start and last stopbit. The clocking is always codirectional.
		FPGA_Registers_Ptr->PCR[G703_Port] = FPGA_REGISTER_RESET;
		NSK_DSP_Status_Ptr->MUX.ComPortStatus[G703_Port] = ((FPGA_Registers_Ptr->CCR[G703_Port] << PORT_CCR_SHIFT) & PORT_CCR_MASK) | ((FPGA_Registers_Ptr->PCR[G703_Port] << PORT_PCR_SHIFT) & PORT_PCR_MASK);
	}
	else
	{
		FPGA_Registers_Ptr->BRR[G703_Port] = FPGA_REGISTER_RESET;
		NSK_DSP_Status_Ptr->MUX.ComPortBitrate[G703_Port] = INITIAL_STATE;
		FPGA_Registers_Ptr->CCR[G703_Port] = FPGA_REGISTER_RESET;
		FPGA_Registers_Ptr->TPR[G703_Port] = FPGA_REGISTER_RESET;
		FPGA_Registers_Ptr->PCR[G703_Port] = FPGA_REGISTER_RESET;
		NSK_DSP_Status_Ptr->MUX.ComPortStatus[G703_Port] = INITIAL_STATE;
	}

	// O4CV channels
	for (i=0; i<N_O4CV_CHANNELS; i++)
	{
		FPGA_Registers_Ptr->TCR[i] = FPGA_REGISTER_RESET;
	}
	NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn = 0;		// all channels off

	//  digital transit
	if (NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_TRANSIT_MASTER_SLAVE)
	{
		FPGA_Registers_Ptr->GDTC = 1; // master, RDC = 0, RIC = 0, CCS = 0
	}
	else
	{
		FPGA_Registers_Ptr->GDTC = 0; // slave, RDC = 0, RIC = 0, CCS = 0
	};

	FPGA_Registers_Ptr->VCE[0] = 0;
	FPGA_Registers_Ptr->VCE[1] = 0;

	for (i=0; i<N_O4CV_CHANNELS/2; i++)
	{
		FPGA_Registers_Ptr->VCE[0] |= getVoiceChEn(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[i]) << i;
		FPGA_Registers_Ptr->VCE[1] |= getVoiceChEn(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[i+8]) << i;
	}


	FPGA_Registers_Ptr->VCTL[0] = 0;
	FPGA_Registers_Ptr->VCTL[1] = 0;

	for (i=0; i<N_O4CV_CHANNELS/2; i++)
	{
		FPGA_Registers_Ptr->VCTL[0] |= getLocalTransit(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[i]) << i;
		FPGA_Registers_Ptr->VCTL[1] |= getLocalTransit(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[i+8]) << i;
	}


	for (i=0; i<N_O4CV_CHANNELS/2; i++)
	{
		FPGA_Registers_Ptr->VCTP[i] = getTerminationPort(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[2*i]);
		FPGA_Registers_Ptr->VCTP[i] |= getTerminationPort(NSK_DSP_Config_Ptr->muxCfg.voiceChannel[2*i+1]) << N_BITS_VOICE_TERMINATION_PORT;
	}

	// Write Configuration to FPGA
	for (i=0; i<N_MUX_PORTS; i++)
	{
		*(long*)(FPGA_Address_Ptr->BRR[i]) = FPGA_Registers_Ptr->BRR[i];
		*(long*)(FPGA_Address_Ptr->CCR[i]) = FPGA_Registers_Ptr->CCR[i];
		*(long*)(FPGA_Address_Ptr->TPR[i]) = FPGA_Registers_Ptr->TPR[i];
		*(long*)(FPGA_Address_Ptr->PCR[i]) = FPGA_Registers_Ptr->PCR[i];
	}

	// Write O4CV channel config
	for (i=0; i<N_O4CV_CHANNELS; i++)
	{
		*(long*)(FPGA_Address_Ptr->TCR[i]) = FPGA_Registers_Ptr->TCR[i];	
	}

	// Write digital transit config
	*(long*)(FPGA_Address_Ptr->GDTC) = FPGA_Registers_Ptr->GDTC;

	*(long*)(FPGA_Address_Ptr->VCE[0]) = FPGA_Registers_Ptr->VCE[0];
	*(long*)(FPGA_Address_Ptr->VCE[1]) = FPGA_Registers_Ptr->VCE[1];

	*(long*)(FPGA_Address_Ptr->VCTL[0]) = FPGA_Registers_Ptr->VCTL[0];
	*(long*)(FPGA_Address_Ptr->VCTL[1]) = FPGA_Registers_Ptr->VCTL[1];

	for (i=0; i<N_O4CV_CHANNELS/2; i++)
	{
		*(long*)(FPGA_Address_Ptr->VCTP[i]) = FPGA_Registers_Ptr->VCTP[i];
	}

	return SubprintMissing;
}
#pragma optimize_as_cmd_line 

#pragma optimize_off
/* testMatlab
SEGMENT_EXT_CODE e_Case_Baudrate_Switch NSK_FPGA_change_port_bitrates(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr, unsigned long* OFDM_FBFF_state_ptr, t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, e_Case_Baudrate_Switch Case_To_Do)
// end testMatlab */
SEGMENT_EXT_CODE e_Case_Baudrate_Switch NSK_FPGA_change_port_bitrates(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr, unsigned long* OFDM_FBFF_state_ptr, t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, e_Case_Baudrate_Switch Case_To_Do)
//e_Case_Baudrate_Switch NSK_FPGA_change_port_bitrates(t_NSK_DSP_Status* NSK_DSP_Status_Ptr,t_NSK_DSP_Config* NSK_DSP_Config_Ptr, unsigned long* OFDM_FBFF_state_ptr, t_FPGA_Registers* FPGA_Address_Ptr, t_FPGA_Registers* FPGA_Registers_Ptr, e_Case_Baudrate_Switch Case_To_Do)
// uncomment this definition when using testMatlab
{
	unsigned long state;
	unsigned long i;
	static BOOL nowResetingModem = FALSE; // False = Modem not in Reset section
	e_Case_Baudrate_Switch Case_New;

	state = *OFDM_FBFF_state_ptr;

	static e_Case_Baudrate_Switch Case_To_Do_Int = Case_OFDM_Port;

	if (nowResetingModem == TRUE) // do the reset do not use the external Case_To_Do
	{
		// wait until reset finished
	}
	else
	{
		Case_To_Do_Int = Case_To_Do; // use the external Case_To_Do

		if (state < N_DATA_RATES)	// normal fall-back fall-forward function
		{	
			// don't change or interrupt
		}
		else // OFDM Modem connection is not in sync. Disable ports and clocks 
		{
			nowResetingModem = TRUE;
			Case_To_Do_Int = Case_Reset_Channels_Part1; // -> interrupt normal function and go to reset
		}
	}

	
	switch(Case_To_Do_Int)
	{
		case Case_OFDM_Port:
			FPGA_Registers_Ptr->PCR[OFDM_Port] &= ~MASK_POE;
			*(long*)(FPGA_Address_Ptr->PCR[OFDM_Port]) = FPGA_Registers_Ptr->PCR[OFDM_Port];		// Disable Port before changing the datarate.

			NSK_DSP_Status_Ptr->MUX.ComPortBitrate[OFDM_Port] = NSK_DSP_Config_Ptr->ofdmCfg.dataRate[state];
			FPGA_Registers_Ptr->BRR[OFDM_Port] = g_DatarateList.ListOfAggregateDatarates[state];
			*(long*)(FPGA_Address_Ptr->BRR[OFDM_Port]) = FPGA_Registers_Ptr->BRR[OFDM_Port];

			// Enable port 0 not until all other ports are enabled to guarantee defined FPGA states.

			if ((NSK_DSP_Status_Ptr->MUX.ComPortStatus[OFDM_Port] & PORT_OUT_ENABLE_SET) != PORT_OUT_ENABLE_SET)
			{
				NSK_DSP_Status_Ptr->MUX.ComPortStatus[OFDM_Port] |= PORT_OUT_ENABLE_SET | PORT_RXCLOCK_OUT_SET;
				FPGA_Registers_Ptr->CCR[OFDM_Port] |= MASK_RXC;
				*(long*)(FPGA_Address_Ptr->CCR[OFDM_Port]) = FPGA_Registers_Ptr->CCR[OFDM_Port];	// Enable Clock.
			}
				
			Case_New = Case_To_Do_Int = Case_V11_Ports;
			break;

		case Case_V11_Ports:
			for (i=0; i<NrOfV11Ports; i++)
			{
				if (NSK_DSP_Config_Ptr->muxCfg.ports.V11[i].control & V11PORT_CFG_ENABLE == V11PORT_CFG_ENABLE)
				{
					if (NSK_DSP_Config_Ptr->muxCfg.rate[state].V11_Rate[i])		// Port is enabled in this OFDM modem state, because baudrate is higher than 0.
					{
						FPGA_Registers_Ptr->PCR[V11_Port_1+i] &= ~MASK_POE;
						*(long*)(FPGA_Address_Ptr->PCR[V11_Port_1+i]) = FPGA_Registers_Ptr->PCR[V11_Port_1+i];

						NSK_DSP_Status_Ptr->MUX.ComPortBitrate[V11_Port_1+i] = NSK_DSP_Config_Ptr->muxCfg.rate[state].V11_Rate[i];
						FPGA_Registers_Ptr->BRR[V11_Port_1+i] = g_DatarateList.ListOfV11PortDatarates[i][state];
						*(long*)(FPGA_Address_Ptr->BRR[V11_Port_1+i]) = FPGA_Registers_Ptr->BRR[V11_Port_1+i];

						FPGA_Registers_Ptr->PCR[V11_Port_1+i] |= MASK_POE;
						*(long*)(FPGA_Address_Ptr->PCR[V11_Port_1+i]) = FPGA_Registers_Ptr->PCR[V11_Port_1+i];		// Enable Port after changing the datarate to guarantee defined FPGA states.

						if ((NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_MUX_ENABLE) == MUX_CFG_CONTROL_MUX_ENABLE)
						{
							FPGA_Registers_Ptr->CCR[V11_Port_1+i] |= MASK_MUX | MASK_RXC;
							NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_1+i] |= PORT_OUT_ENABLE_SET | PORT_TO_MUX_SET | PORT_RXCLOCK_OUT_SET;
						}
						else
						{
							FPGA_Registers_Ptr->CCR[V11_Port_1+i] |= MASK_RXC;
							NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_1+i] |= PORT_OUT_ENABLE_SET | PORT_RXCLOCK_OUT_SET;
						}
					}
					else
					{
						FPGA_Registers_Ptr->PCR[V11_Port_1+i] &= ~MASK_POE;
						*(long*)(FPGA_Address_Ptr->PCR[V11_Port_1+i]) = FPGA_Registers_Ptr->PCR[V11_Port_1+i];
						FPGA_Registers_Ptr->CCR[V11_Port_1+i] &= ~MASK_MUX & ~MASK_RXC;						// Do not forget to disable MUX and Clock-bit when disabling port!
						NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_1+i] &= PORT_OUT_ENABLE_CLEAR & PORT_TO_MUX_CLEAR & PORT_RXCLOCK_OUT_CLEAR;
						NSK_DSP_Status_Ptr->MUX.ComPortBitrate[V11_Port_1+i] = NSK_DSP_Config_Ptr->muxCfg.rate[state].V11_Rate[i];
					}
					*(long*)(FPGA_Address_Ptr->CCR[V11_Port_1+i]) = FPGA_Registers_Ptr->CCR[V11_Port_1+i];
				}
			}

			if (NSK_DSP_Config_Ptr->muxCfg.ports.LAN.control & LAN_PORT_CFG_ENABLE == LAN_PORT_CFG_ENABLE)
			{
				if (NSK_DSP_Config_Ptr->muxCfg.rate[state].LAN_Rate)		// Port is enabled in this OFDM modem state, because baudrate is higher than 0.
				{
					FPGA_Registers_Ptr->PCR[LAN_Port] &= ~MASK_POE;
					*(long*)(FPGA_Address_Ptr->PCR[LAN_Port]) = FPGA_Registers_Ptr->PCR[LAN_Port];

					NSK_DSP_Status_Ptr->MUX.ComPortBitrate[LAN_Port] = NSK_DSP_Config_Ptr->muxCfg.rate[state].LAN_Rate;
					FPGA_Registers_Ptr->BRR[LAN_Port] = g_DatarateList.ListOfLANPortDatarates[state];
					*(long*)(FPGA_Address_Ptr->BRR[LAN_Port]) = FPGA_Registers_Ptr->BRR[LAN_Port];

					FPGA_Registers_Ptr->PCR[LAN_Port] |= MASK_POE;
					*(long*)(FPGA_Address_Ptr->PCR[LAN_Port]) = FPGA_Registers_Ptr->PCR[LAN_Port];		// Enable Port after changing the datarate to guarantee defined FPGA states.

					if ((NSK_DSP_Config_Ptr->muxCfg.control & MUX_CFG_CONTROL_MUX_ENABLE) == MUX_CFG_CONTROL_MUX_ENABLE)
					{
						FPGA_Registers_Ptr->CCR[LAN_Port] |= MASK_MUX | MASK_RXC;
						NSK_DSP_Status_Ptr->MUX.ComPortStatus[LAN_Port] |= PORT_OUT_ENABLE_SET | PORT_TO_MUX_SET | PORT_RXCLOCK_OUT_SET;
					}
					else
					{
						FPGA_Registers_Ptr->CCR[LAN_Port] |= MASK_RXC;
						NSK_DSP_Status_Ptr->MUX.ComPortStatus[LAN_Port] |= PORT_OUT_ENABLE_SET | PORT_RXCLOCK_OUT_SET;
					}
				}
				else
				{
					FPGA_Registers_Ptr->PCR[LAN_Port] &= ~MASK_POE;
					*(long*)(FPGA_Address_Ptr->PCR[LAN_Port]) = FPGA_Registers_Ptr->PCR[LAN_Port];
					FPGA_Registers_Ptr->CCR[LAN_Port] &= ~MASK_MUX & ~MASK_RXC;						// Do not forget to disable MUX and Clock-bit when disabling port!
					NSK_DSP_Status_Ptr->MUX.ComPortStatus[LAN_Port] &= PORT_OUT_ENABLE_CLEAR & PORT_TO_MUX_CLEAR & PORT_RXCLOCK_OUT_CLEAR;
					NSK_DSP_Status_Ptr->MUX.ComPortBitrate[LAN_Port] = NSK_DSP_Config_Ptr->muxCfg.rate[state].LAN_Rate;
				}
				*(long*)(FPGA_Address_Ptr->CCR[LAN_Port]) = FPGA_Registers_Ptr->CCR[LAN_Port];
			}

			Case_New = Case_To_Do_Int = Case_V24_Ports_P4LT;
			break;

		case Case_V24_Ports_P4LT:
			for (i=0; i<NUMBER_OF_V24_PORTS_P4LT; i++)
			{
				if ((NSK_DSP_Config_Ptr->muxCfg.ports.V24[i].control & V24PORT_CFG_CONTROL_PORT_MODES) == V24PORT_CFG_CONTROL_PORT_MODES) // Port is in MUX mode
				{
					if (g_DatarateList.ListOfV24PortEnables[i][state])
					{
						FPGA_Registers_Ptr->PCR[V24_Port_1+i] |= MASK_POE;
						FPGA_Registers_Ptr->CCR[V24_Port_1+i] |= MASK_MUX;		// Do not set MUX-bit before enabling port!
						NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] |= PORT_OUT_ENABLE_SET | PORT_TO_MUX_SET;
					}
					else
					{
						FPGA_Registers_Ptr->PCR[V24_Port_1+i] &= ~MASK_POE;
						FPGA_Registers_Ptr->CCR[V24_Port_1+i] &= ~MASK_MUX;		// Do not forget to disable MUX-bit when disabling port!
						NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] &= PORT_OUT_ENABLE_CLEAR & PORT_TO_MUX_CLEAR;
					}
					*(long*)(FPGA_Address_Ptr->PCR[V24_Port_1+i]) = FPGA_Registers_Ptr->PCR[V24_Port_1+i];
					*(long*)(FPGA_Address_Ptr->CCR[V24_Port_1+i]) = FPGA_Registers_Ptr->CCR[V24_Port_1+i];
				}
			}

			if ((NSK_DSP_Config_Ptr->muxCfg.ports.G703.control & G703_PORT_CFG_ENABLE) == G703_PORT_CFG_ENABLE)
			{
				if (g_DatarateList.ListOfG703PortEnables[state])
				{
					FPGA_Registers_Ptr->PCR[G703_Port] |= MASK_POE;
					FPGA_Registers_Ptr->CCR[G703_Port] |= MASK_MUX;		// Do not set MUX-bit before enabling port!
					NSK_DSP_Status_Ptr->MUX.ComPortStatus[G703_Port] |= PORT_OUT_ENABLE_SET | PORT_TO_MUX_SET;
				}
				else
				{
					FPGA_Registers_Ptr->PCR[G703_Port] &= ~MASK_POE;
					FPGA_Registers_Ptr->CCR[G703_Port] &= ~MASK_MUX;		// Do not forget to disable MUX-bit when disabling port!
					NSK_DSP_Status_Ptr->MUX.ComPortStatus[G703_Port] &= PORT_OUT_ENABLE_CLEAR & PORT_TO_MUX_CLEAR;
				}
				*(long*)(FPGA_Address_Ptr->PCR[G703_Port]) = FPGA_Registers_Ptr->PCR[G703_Port];
				*(long*)(FPGA_Address_Ptr->CCR[G703_Port]) = FPGA_Registers_Ptr->CCR[G703_Port];
			}
				
			Case_New = Case_To_Do_Int = Case_V24_Ports_P4LU;
			break;

		case Case_V24_Ports_P4LU:

			if(NrOfV24Ports>NUMBER_OF_V24_PORTS_P4LT)
			{
				for (i=NUMBER_OF_V24_PORTS_P4LT; i<NrOfV24Ports; i++)
				{
					if ((NSK_DSP_Config_Ptr->muxCfg.ports.V24[i].control & V24PORT_CFG_CONTROL_PORT_MODES) == V24PORT_CFG_CONTROL_PORT_MODES) // Port is in MUX mode
					{
						if (g_DatarateList.ListOfV24PortEnables[i][state])
						{
							FPGA_Registers_Ptr->PCR[V24_Port_1+i] |= MASK_POE;
							FPGA_Registers_Ptr->CCR[V24_Port_1+i] |= MASK_MUX;		// Do not set MUX-bit before enabling port!
							NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] |= PORT_OUT_ENABLE_SET | PORT_TO_MUX_SET;
						}
						else
						{
							FPGA_Registers_Ptr->PCR[V24_Port_1+i] &= ~MASK_POE;
							FPGA_Registers_Ptr->CCR[V24_Port_1+i] &= ~MASK_MUX;		// Do not forget to disable MUX-bit when disabling port!
							NSK_DSP_Status_Ptr->MUX.ComPortStatus[V24_Port_1+i] &= PORT_OUT_ENABLE_CLEAR & PORT_TO_MUX_CLEAR;
						}
						*(long*)(FPGA_Address_Ptr->PCR[V24_Port_1+i]) = FPGA_Registers_Ptr->PCR[V24_Port_1+i];
						*(long*)(FPGA_Address_Ptr->CCR[V24_Port_1+i]) = FPGA_Registers_Ptr->CCR[V24_Port_1+i];
					}
				}
			}
			else
			{
				// do nothing
			}

				
			FPGA_Registers_Ptr->PCR[OFDM_Port] |= MASK_POE;
			*(long*)(FPGA_Address_Ptr->PCR[OFDM_Port]) = FPGA_Registers_Ptr->PCR[OFDM_Port];	// Enable port 0 in the very end to guarantee defined FPGA states.

			Case_New = Case_To_Do_Int = Case_O4CV_Channels_Part1;
			break;

		case Case_O4CV_Channels_Part1:

			for (i=0; i<(N_O4CV_CHANNELS/2); i++)		// channels 1 - 8
			{
				if ((NSK_DSP_Config_Ptr->muxCfg.voiceChannel[i] & VOICE_CH_MODE_MASK) &&	// channel enabled
					(g_DatarateList.ListOfO4CVChDatarates[i][state]))					// data rate != 0
				{
					FPGA_Registers_Ptr->TCR[i] = MASK_TCR_MUX | g_DatarateList.ListOfO4CVChDatarates[i][state];
					NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn |= BIT0 << i;		// channel on
				}
				else
				{
					FPGA_Registers_Ptr->TCR[i] = FPGA_REGISTER_RESET;
					NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn &= ~(BIT0 << i);	// channel off
				}
				*(long*)(FPGA_Address_Ptr->TCR[i]) = FPGA_Registers_Ptr->TCR[i];
			}

			Case_New = Case_To_Do_Int = Case_O4CV_Channels_Part2;
			break;

		case Case_O4CV_Channels_Part2: 

			for (i=(N_O4CV_CHANNELS/2); i<N_O4CV_CHANNELS; i++)	// channels 9 - 16
			{
				if ((NSK_DSP_Config_Ptr->muxCfg.voiceChannel[i] & VOICE_CH_MODE_MASK) &&	// channel enabled
					(g_DatarateList.ListOfO4CVChDatarates[i][state]))					// data rate != 0
				{
					FPGA_Registers_Ptr->TCR[i] = MASK_TCR_MUX | g_DatarateList.ListOfO4CVChDatarates[i][state];
					NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn |= BIT0 << i;		// channel on
				}
				else
				{
					FPGA_Registers_Ptr->TCR[i] = FPGA_REGISTER_RESET;
					NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn &= ~(BIT0 << i);	// channel off
				}
				*(long*)(FPGA_Address_Ptr->TCR[i]) = FPGA_Registers_Ptr->TCR[i];
			}

			Case_New = Case_To_Do_Int = Case_OFDM_Port;
			break;

//________ Reset Part ____________________________________________________________________________________

		case Case_Reset_Channels_Part1: 

			FPGA_Registers_Ptr->PCR[OFDM_Port] &= ~MASK_POE;
			*(long*)(FPGA_Address_Ptr->PCR[OFDM_Port]) = FPGA_Registers_Ptr->PCR[OFDM_Port];
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[OFDM_Port] &= PORT_OUT_ENABLE_CLEAR & PORT_RXCLOCK_OUT_CLEAR;
			FPGA_Registers_Ptr->CCR[OFDM_Port] &= ~MASK_RXC;
			*(long*)(FPGA_Address_Ptr->CCR[OFDM_Port]) = FPGA_Registers_Ptr->CCR[OFDM_Port];

			FPGA_Registers_Ptr->PCR[V11_Port_1] &= ~MASK_POE;
			*(long*)(FPGA_Address_Ptr->PCR[V11_Port_1]) = FPGA_Registers_Ptr->PCR[V11_Port_1];
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_1] &= PORT_OUT_ENABLE_CLEAR & PORT_RXCLOCK_OUT_CLEAR;
			FPGA_Registers_Ptr->CCR[V11_Port_1] &= ~MASK_RXC;
			*(long*)(FPGA_Address_Ptr->CCR[V11_Port_1]) = FPGA_Registers_Ptr->CCR[V11_Port_1];

			FPGA_Registers_Ptr->PCR[V11_Port_2] &= ~MASK_POE;
			*(long*)(FPGA_Address_Ptr->PCR[V11_Port_2]) = FPGA_Registers_Ptr->PCR[V11_Port_2];
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[V11_Port_2] &= PORT_OUT_ENABLE_CLEAR & PORT_RXCLOCK_OUT_CLEAR;
			FPGA_Registers_Ptr->CCR[V11_Port_2] &= ~MASK_RXC;
			*(long*)(FPGA_Address_Ptr->CCR[V11_Port_2]) = FPGA_Registers_Ptr->CCR[V11_Port_2];

			FPGA_Registers_Ptr->PCR[G703_Port] &= ~MASK_POE; 
			*(long*)(FPGA_Address_Ptr->PCR[G703_Port]) = FPGA_Registers_Ptr->PCR[G703_Port];
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[G703_Port] &= PORT_OUT_ENABLE_CLEAR;

			Case_New = Case_To_Do_Int = Case_Reset_Channels_Part2;

			break;

		case Case_Reset_Channels_Part2: 

			FPGA_Registers_Ptr->PCR[LAN_Port] &= ~MASK_POE;
			*(long*)(FPGA_Address_Ptr->PCR[LAN_Port]) = FPGA_Registers_Ptr->PCR[LAN_Port];
			NSK_DSP_Status_Ptr->MUX.ComPortStatus[LAN_Port] &= PORT_OUT_ENABLE_CLEAR & PORT_RXCLOCK_OUT_CLEAR;
			FPGA_Registers_Ptr->CCR[LAN_Port] &= ~MASK_RXC;
			*(long*)(FPGA_Address_Ptr->CCR[LAN_Port]) = FPGA_Registers_Ptr->CCR[LAN_Port];

			for (i=0; i<4; i++)		// channels 1 - 4
			{
				FPGA_Registers_Ptr->TCR[i] = FPGA_REGISTER_RESET;
				*(long*)(FPGA_Address_Ptr->TCR[i]) = FPGA_Registers_Ptr->TCR[i];
			}
			NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn = 0;		// all channels off
	

			Case_New = Case_To_Do_Int = Case_Reset_Channels_Part3;
			break;

		case Case_Reset_Channels_Part3: 

			for (i=4; i<10; i++)		// channels 4 - 10
			{
				FPGA_Registers_Ptr->TCR[i] = FPGA_REGISTER_RESET;
				*(long*)(FPGA_Address_Ptr->TCR[i]) = FPGA_Registers_Ptr->TCR[i];
			}
			
			NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn = 0;		// all channels off
	
			Case_New = Case_To_Do_Int = Case_Reset_Channels_Part4;
			break;

		case Case_Reset_Channels_Part4: 

			for (i=10; i<N_O4CV_CHANNELS; i++)	// channels 11 - 16
			{
				FPGA_Registers_Ptr->TCR[i] = FPGA_REGISTER_RESET;
				*(long*)(FPGA_Address_Ptr->TCR[i]) = FPGA_Registers_Ptr->TCR[i];
			}
			NSK_DSP_Status_Ptr->MUX_O4CV.channelsOn = 0;		// all channels off
	
			// move the following lines to the last reset case!
			nowResetingModem = FALSE; // reset flag
			Case_New = Case_To_Do_Int = Case_OFDM_Port; // return to normal operation
			break;

	}

	return Case_New;
}


/* End of File ***********************************************************/

