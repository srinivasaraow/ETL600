--------------------------------------------------------------------------------
-- $Workfile: tb_timing_s1_behave.vhd $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : 
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_timing_s1_behave.vhd $
-- 
-- 3     11-07-06 9:05 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;

LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.tb_dt_main_pkg.all;
USE DIG_TRANSIT_tb.basic_tb_pkg.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY tb_timing_s1 IS
   PORT( 
      Resetn      : IN  std_logic;
      clk32       : IN  std_logic;
      clock_speed : IN  integer range 0 to 2;
      clk32_s1    : OUT std_logic
   );

-- Declarations

END tb_timing_s1 ;

--
ARCHITECTURE behave OF tb_timing_s1 IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation


BEGIN

    toggle_clk_s1: process
    begin
      clk32_s1  <= '0';
      
      if clock_speed = 0 then
        wait for c_clock_uC_zero_cycle_s1;
      elsif clock_speed = 1 then
        wait for c_clock_uC_zero_cycle_s1_overspeed;
      else
        wait for c_clock_uC_zero_cycle_s1_underspeed;
      end if;    
      
      clk32_s1  <= '1';

      if clock_speed = 0 then
        wait for c_clock_uC_one_cycle_s1;
      elsif clock_speed = 1 then
        wait for c_clock_uC_one_cycle_s1_overspeed;
      else
        wait for c_clock_uC_one_cycle_s1_underspeed;
      end if;  

    end process;

END behave;

