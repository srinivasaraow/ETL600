--------------------------------------------------------------------------
-- $Workfile: tb_v24_delay_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_v24_delay_behavior.vhd $
-- 
-- 10    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 8     2.12.04 11:16 Chalgyg
-- 
-- 6     7.10.04 17:09 Gygax02
-- -- no description --
-- 
-- 5     19.08.04 11:05 Gygax02
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

ENTITY tb_v24_delay IS
   PORT( 
      v24_txd_bus     : IN     std_logic_vector (9 DOWNTO 0);
      delayed_v24_end : OUT    std_logic_vector (3 DOWNTO 0);
      delayed_v24_txd : OUT    std_logic_vector (3 DOWNTO 0)
   );

-- Declarations

END tb_v24_delay ;

ARCHITECTURE behavior OF tb_v24_delay IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN

  delayed_v24_txd  <= transport v24_txd_bus(3 downto 0) after 93.75 us;
  delayed_v24_end  <= transport v24_txd_bus(7 downto 4) after 93.75 us;

END ARCHITECTURE behavior;

