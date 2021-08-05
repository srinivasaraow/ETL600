--------------------------------------------------------------------------
-- $Workfile: tb_diverse_rxc_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_diverse_rxc_behavior.vhd $
-- 
-- 7     11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 4     19.08.04 11:05 Gygax02
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

ENTITY tb_diverse_rxc IS
   PORT( 
      rxc_dummy : IN     std_logic_vector (3 DOWNTO 0);
      rxc1      : OUT    std_logic;
      rxc2      : OUT    std_logic;
      rxc3      : OUT    std_logic;
      rxc4      : OUT    std_logic
   );

-- Declarations

END tb_diverse_rxc ;

ARCHITECTURE behavior OF tb_diverse_rxc IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4

BEGIN
 
  rxc1  <= rxc_dummy(0);
  rxc2  <= rxc_dummy(1);
  rxc3  <= rxc_dummy(2);
  rxc4  <= rxc_dummy(3);

END ARCHITECTURE behavior;

