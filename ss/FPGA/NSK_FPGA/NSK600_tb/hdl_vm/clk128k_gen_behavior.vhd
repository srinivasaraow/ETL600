--------------------------------------------------------------------------
-- $Workfile: clk128k_gen_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/clk128k_gen_behavior.vhd $
-- 
-- 8     11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 5     7.10.04 17:09 Gygax02
-- -- no description --
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
USE NSK600_tb.tb_real_data_p.all;

ENTITY clk128k_gen IS
   PORT( 
      clk_128k : OUT    std_logic
   );

-- Declarations

END clk128k_gen ;

ARCHITECTURE arch_name OF clk128k_gen IS
  
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4
  
BEGIN
  gen_clk: process
  begin
    clk_128k  <= '0';
    wait for 100 ns;
    clk_128k  <= '1';
    wait for 488.28125 ns;
    clk_128k  <= '0';
    wait for 388.28125 ns;
  end process;
END ARCHITECTURE arch_name;

