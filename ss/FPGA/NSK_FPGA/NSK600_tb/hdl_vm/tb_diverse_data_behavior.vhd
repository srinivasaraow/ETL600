--------------------------------------------------------------------------
-- $Workfile: tb_diverse_data_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_diverse_data_behavior.vhd $
-- 
-- 5     11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 2     19.08.04 11:05 Gygax02
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

ENTITY tb_diverse_data IS
   PORT( 
      data_out : IN     t_channel_array;
      data1    : OUT    std_logic_vector (7 DOWNTO 0);
      data2    : OUT    std_logic_vector (7 Downto 0);
      data3    : OUT    std_logic_vector (7 Downto 0);
      data4    : OUT    std_logic_vector (7 Downto 0)
   );

-- Declarations

END tb_diverse_data ;

ARCHITECTURE behavior OF tb_diverse_data IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN
  data1  <= data_out(0);
  data2  <= data_out(1);
  data3  <= data_out(2);
  data4  <= data_out(3);
END ARCHITECTURE behavior;

