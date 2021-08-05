--
-- VHDL Package Body DIG_TRANSIT_tb.tb_dt_main_pkg
--
-- Created:
--          by - chstrue.UNKNOWN (CH-L-0014426)
--          at - 11:48:52 2011-02-22
--
-- using Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
PACKAGE BODY tb_dt_main_pkg IS

  --uC Master: 32.768MHz
  constant c_clock_uC_zero_cycle      : time := 15.258789 ns;
  constant c_clock_uC_one_cycle       : time := 15.258789 ns;
  constant c_clock_uC_cycle           : time := c_clock_uC_zero_cycle + c_clock_uC_one_cycle;

  --uC Slave 1: 32.768MHz
  constant c_clock_uC_zero_cycle_s1   : time := 15.258789 ns;
  constant c_clock_uC_one_cycle_s1    : time := 15.258789 ns;
  constant c_clock_uC_cycle_s1        : time := c_clock_uC_zero_cycle_s1 + c_clock_uC_one_cycle_s1;

  --uC Slave 1: 34.406MHz (+5%)
  constant c_clock_uC_zero_cycle_s1_overspeed  : time := 14.53218 ns;
  constant c_clock_uC_one_cycle_s1_overspeed   : time := 14.53218 ns;
  constant c_clock_uC_cycle_s1_overspeed       : time := c_clock_uC_zero_cycle_s1_overspeed + c_clock_uC_one_cycle_s1_overspeed;
  
  --uC Slave 1: 31.1296MHz (-5%)
  constant c_clock_uC_zero_cycle_s1_underspeed : time := 16.06188 ns;
  constant c_clock_uC_one_cycle_s1_underspeed  : time := 16.06188 ns;
  constant c_clock_uC_cycle_s1_underspeed      : time := c_clock_uC_zero_cycle_s1_underspeed + c_clock_uC_one_cycle_s1_underspeed;
 
END tb_dt_main_pkg;
