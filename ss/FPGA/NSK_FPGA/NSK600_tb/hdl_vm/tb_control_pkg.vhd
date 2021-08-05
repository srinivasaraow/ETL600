--------------------------------------------------------------------------
-- $Workfile: tb_control_pkg.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_control_pkg.vhd $
-- 
-- 8     11-07-06 9:24 Chstrue
-- R4 implementation
-- 
-- 7     12.04.05 11:13 Chalgyg
-- Took FT1.2 pattern protocol transmission into simulation time
-- calculation account.
-- 
-- 6     1.03.05 10:05 Chalgyg
-- Simulation time estimation introduced.
-- 
-- 4     1.12.04 14:56 Chalgyg
-- Output comments cleaned.
-- 
-- 2     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_ft1_pattern_p.all;
library std;
use std.textio.all;

PACKAGE tb_control_p IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  type time_vector is array (0 to (t_testcase'high+1)) of real;

  function int2str(int: integer) return string;

  procedure estimate_time(
    variable longest_time : out time_vector
  );

  procedure printf(
    constant s      : in string
  );

  procedure generate_message (
    constant index  : in integer
  );

END tb_control_p;
