--------------------------------------------------------------------------
-- $Workfile: tb_distr_lan_behaviour.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2005
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_distr_lan_behaviour.vhd $
-- 
-- 3     11-07-06 9:21 Chstrue
-- R4 implementation
-- 
-- 2     08-04-22 16:25 Chstrue
-- additional port definitions
-- 
-- 1     10.01.06 9:09 Chalgyg
-- Initial version
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_ofdm_fpga_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_v24_if_p.all;
USE NSK600_tb.tb_real_data_p.all;
USE ieee.std_logic_unsigned.all;
USE ieee.NUMERIC_STD.all;
USE NSK600_tb.tb_v11_if_p.all;

ENTITY tb_distr_lan IS
   PORT( 
      v11_c        : IN     std_logic;
      v11_txd      : IN     std_logic;
      v11_i        : OUT    std_logic;
      v11_rxd      : OUT    std_logic;
      v11_txc      : OUT    std_logic;
      netarm_porta : INOUT  std_logic_vector (7 DOWNTO 0)
   );

-- Declarations

END tb_distr_lan ;

--
ARCHITECTURE behavior OF tb_distr_lan IS

-----------------------------------------------------------
-- Last edited:
--   080422: StR additional port definitions
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN
  
  v11_txc          <= netarm_porta(0);
  v11_rxd          <= netarm_porta(3);
  v11_i            <= netarm_porta(2);  -- DCD
  netarm_porta(7)  <= v11_txd;
  netarm_porta(6)  <= '0'; -- LAN not booted    //v11_c;            -- DTR/GPIO-5
  netarm_porta(5)  <= '0'; -- LAN not connected
  netarm_porta(1)  <= '0';
  netarm_porta(4)  <= '0';
  
END ARCHITECTURE behavior;

