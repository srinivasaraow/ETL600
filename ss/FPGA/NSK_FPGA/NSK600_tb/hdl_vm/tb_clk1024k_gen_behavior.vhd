--------------------------------------------------------------------------
-- $Workfile: tb_clk1024k_gen_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_clk1024k_gen_behavior.vhd $
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
USE NSK600_tb.tb_real_data_p.all;

ENTITY tb_clk1024k_gen IS
   PORT( 
      clk_1024k : OUT    std_logic
   );

-- Declarations

END tb_clk1024k_gen ;

ARCHITECTURE behavior OF tb_clk1024k_gen IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN
  gen_clk: process
  begin
    clk_1024k  <= '0';
    wait for 100 ns;
    clk_1024k  <= '1';
    wait for 488.28125 ns;
    clk_1024k  <= '0';
    wait for 388.28125 ns;
  end process;

END ARCHITECTURE behavior;

