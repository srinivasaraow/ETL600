--------------------------------------------------------------------------
-- $Workfile: tb_ofdm_fpga_pkg.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_ofdm_fpga_pkg.vhd $
-- 
-- 5     11-07-06 9:29 Chstrue
-- R4 implementation
-- 
-- 4     25.02.05 13:22 Chalgyg
-- - Function give_first_0_pos() hived off from analyse_mux_frame().
-- - Signal mark_low added for debugging purposes.
-- 
-- 3     24.02.05 15:13 Chalgyg
-- Whole mux frame detection added.
-- 
-- 1     22.10.04 16:24 Gygax02
-- -- no description --
---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_control_p.all;


PACKAGE tb_ofdm_fpga_p IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  type integer_vect is array (0 to 9) of integer;

  function slv2real(slv : std_logic_vector) return real;
  function give_first_0_pos(slv : std_logic_vector) return integer;

  procedure analyse_mux_frame (
    signal   testcase          : in t_testcase;
    signal   sclk0             : in std_logic;
    signal   v11_control       : in std_logic;
    signal   d0a               : in std_logic;
    signal   mark_low          : out std_logic;
    signal   failure           : out std_logic
  );

END tb_ofdm_fpga_p;
