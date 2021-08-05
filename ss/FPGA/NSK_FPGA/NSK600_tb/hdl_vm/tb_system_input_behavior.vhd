--------------------------------------------------------------------------
-- $Workfile: tb_system_input_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_system_input_behavior.vhd $
-- 
-- 15    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 14    08-04-22 16:27 Chstrue
-- interface adapted
-- 
-- 13    6.09.06 9:32 Chalgyg
-- clk32 precision enhanced to 1 fs.
-- 
-- 12    10.01.06 9:07 Chalgyg
-- WXYZ arithmetic bug removed.
-- 
-- 11    20.01.05 11:44 Chalgyg
-- LED watching disabled.
-- 
-- 9     13.12.04 13:53 Chalgyg
-- LED check added.
-- 
-- 8     8.12.04 10:54 Chalgyg
-- Reset removed to tb_dsp_if.
-- 
-- 6     19.08.04 11:05 Gygax02
-- -- no description --
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
LIBRARY std;
USE std.textio.all;
LIBRARY NSK600_tb;
USE NSK600_tb.report_assist.all;

ENTITY tb_system_input IS
   PORT( 
      testcase   : IN     t_testcase;
      clk32      : OUT    std_logic;
      configured : IN     std_logic;
      leds       : IN     std_logic_vector (4 DOWNTO 0)
   );

-- Declarations

END tb_system_input ;

ARCHITECTURE behavior OF tb_system_input IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN
  
  toggle_clk: process
  begin
    clk32  <= '0';
    wait for 15.258789 ns;
    clk32  <= '1';
    wait for 15.258789 ns;
  end process;


--  watch_leds: process
--    variable leds_old : std_logic_vector (4 DOWNTO 0) := (others=>'0');
--    variable compare  : std_logic_vector (4 DOWNTO 0) := (others=>'0');
--  begin
--    leds_old  := leds;
--    wait on leds;
--    compare   := leds xor leds_old;
--    for i in 0 to (leds'length-1) loop
--      if (compare(i) = '1') then
--        if (leds(i) = '1') then
--          printf("** LED" & id2chr(i) & " has turned on.");
--        else
--          printf("** LED" & id2chr(i) & " has turned off.");
--        end if;
--      end if;
--    end loop;
--  end process;
  
END ARCHITECTURE behavior;

