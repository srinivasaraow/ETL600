--------------------------------------------------------------------------
-- $Workfile: tb_busy_mux_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_busy_mux_behavior.vhd $
-- 
-- 11    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 10    19.01.06 16:15 Chalgyg
-- G.703 interface integrated.
-- 
-- 9     10.01.06 9:05 Chalgyg
-- LAN interface is now supported.
-- 
-- 7     2.12.04 11:17 Chalgyg
-- 
-- 5     8.10.04 14:33 Gygax02
-- -- no description --
-- 
-- 4     7.10.04 17:09 Gygax02
-- -- no description --
-- 
-- 3     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.STD_LOGIC_UNSIGNED.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_real_data_p.all;

ENTITY tb_busy_mux IS
   PORT( 
      busy            : IN     std_logic;
      delayed_v24_end : IN     std_logic_vector (3 DOWNTO 0);
      testcase        : IN     t_testcase;
      v11_busy        : IN     std_logic_vector (3 DOWNTO 0);
      v24_txd_bus     : IN     std_logic_vector (9 DOWNTO 0);
      busy_muxed      : OUT    std_logic
   );

-- Declarations

END tb_busy_mux ;

ARCHITECTURE behavior OF tb_busy_mux IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN
  busy_muxed <= (busy or v11_busy(3) or v11_busy(2) or v11_busy(1) or v11_busy(0)) when (loop_data_on_dsp_if(testcase) = '0') else
    (delayed_v24_end(3) or delayed_v24_end(2) or delayed_v24_end(1) or delayed_v24_end(0) or v24_txd_bus(9) or v24_txd_bus(8) or v24_txd_bus(7) or v24_txd_bus(6) or v24_txd_bus(5) or v24_txd_bus(4) or v11_busy(3) or v11_busy(2) or v11_busy(1) or v11_busy(0));

END ARCHITECTURE behavior;

