--------------------------------------------------------------------------
-- $Workfile: tb_join_rxd_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_join_rxd_behavior.vhd $
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

ENTITY tb_join_rxd IS
   PORT( 
      data_rxd1 : IN     std_logic_vector (7 DOWNTO 0);
      data_rxd2 : IN     std_logic_vector (7 DOWNTO 0);
      data_rxd3 : IN     std_logic_vector (7 DOWNTO 0);
      data_rxd4 : IN     std_logic_vector (7 DOWNTO 0);
      data_in   : OUT    t_channel_array
   );

-- Declarations

END tb_join_rxd ;

ARCHITECTURE behavior OF tb_join_rxd IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN

  data_in(0)  <= data_rxd1;
  data_in(1)  <= data_rxd2;
  data_in(2)  <= data_rxd3;
  data_in(3)  <= data_rxd4;
  
END ARCHITECTURE behavior;

