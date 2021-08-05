--------------------------------------------------------------------------------
-- $Workfile: tb_timing_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_timing_behave.vhd $
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

ENTITY tb_timing IS
   PORT( 
      Resetn   : IN     std_logic;
      clk32    : OUT    std_logic;
      clk32_s1 : IN     std_logic
   );

-- Declarations

END tb_timing ;

--
ARCHITECTURE behave OF tb_timing IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation



BEGIN
  
  ------------------uC clock-----------------
  toggle_clk: process
  begin
    clk32  <= '0';
    wait for c_clock_uC_zero_cycle;
    clk32  <= '1';
    wait for c_clock_uC_one_cycle;
  end process;
 
END behave;

