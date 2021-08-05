--------------------------------------------------------------------------
-- $Workfile: tb_diverse_rxd_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_diverse_rxd_behavior.vhd $
-- 
-- 8     11-07-06 9:20 Chstrue
-- R4 implementation
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

ENTITY tb_diverse_rxd IS
   PORT( 
      rxd_dummy : IN     std_logic_vector (3 DOWNTO 0);
      rxd1      : OUT    std_logic;
      rxd2      : OUT    std_logic;
      rxd3      : OUT    std_logic;
      rxd4      : OUT    std_logic
   );

-- Declarations

END tb_diverse_rxd ;


ARCHITECTURE behavior OF tb_diverse_rxd IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4

BEGIN

  rxd1  <= rxd_dummy(0);
  rxd2  <= rxd_dummy(1);
  rxd3  <= rxd_dummy(2);
  rxd4  <= rxd_dummy(3);

END ARCHITECTURE behavior;

