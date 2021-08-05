/*-------------------------------------------------------------------------*
 * $Workfile: OFDM_FPGA.h $
 * Part of      : ETL600 / System
 * Language     : C
 * Created by   : Alexander Gygax, PTUKT2
 * Remarks      :  
 * Purpose      : FPGA function declarations
 * Copyright    : ABB Switzerland Ltd. 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/H/OFDM_FPGA.h $
 * 
 * 1     19.01.05 15:44 Chchlee
 * 
 * 1     25.06.04 14:42 Gygax02
 * 
 * 2     21.06.04 13:46 Gygax02
 * Functions renamed
 * 
 * 1     21.06.04 11:17 Gygax02
 * Initial version
 *-------------------------------------------------------------------------*/

#ifndef OFDM_FPGA_H
#define OFDM_FPGA_H

typedef enum
{
	OFDM_Addr_SCLK	= 0x00,
	OFDM_Addr_SG703	= 0x01,
	OFDM_Addr_SV11	= 0x02,
	OFDM_Addr_SFPGA	= 0x03,
	OFDM_Addr_IDNT	= 0x04,
	OFDM_Addr_CCLK	= 0x10,
	OFDM_Addr_CG703	= 0x11,
	OFDM_Addr_CV11	= 0x12,
	OFDM_Addr_CFPGA	= 0x13
} e_FPGA_OFDM_Addr;

typedef enum
{
	Clk_Sel_fix	= 0x00,
	Clk_Sel_var	= 0x01
} e_Clk_Sel;

typedef struct
{
	long	Clock_State;
	long	G703_State;
	long	V11_State;
	long	FPGA_State;
	long	Identification;
	long	Clock_Config;
	long	G703_Config;
	long	V11_Config;
	long	FPGA_Config;
} t_FPGA_OFDM_State;

void segment("ext_code") OFDM_FPGA_RD_State_OFDM(t_FPGA_OFDM_State *state);
void segment("ext_code") OFDM_FPGA_RD_Config_OFDM(t_FPGA_OFDM_State *state);
void segment("ext_code") OFDM_FPGA_Clk_Sel(e_Clk_Sel Selection);

#endif
