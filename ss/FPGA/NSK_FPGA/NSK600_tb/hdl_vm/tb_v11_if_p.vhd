--------------------------------------------------------------------------
-- $Workfile: tb_v11_if_p.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_v11_if_p.vhd $
-- 
-- 10    11-07-06 9:30 Chstrue
-- R4 implementation
-- 
-- 9     19.01.06 16:19 Chalgyg
-- G.703 interface integrated.
-- 
-- 8     25.02.05 13:17 Chalgyg
-- tb_ofdm_fpga_p added.
-- 
-- 7     7.02.05 11:50 Chalgyg
-- Delay measurement added.
-- 
-- 6     24.01.05 14:21 Chalgyg
-- Package tb_v24_if_p added.
-- 
-- 4     21.10.04 13:07 Gygax02
-- Testvector removed.
-- 
-- 2     14.10.04 16:23 Gygax02
-- -- no description --
-- 
-- 1     8.10.04 14:34 Gygax02
-- -- no description --
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_v24_if_p.all;
USE NSK600_tb.tb_ofdm_fpga_p.all;

PACKAGE tb_v11_if_p IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4



  function check_v11_delay_func(
    delay    : integer;
    id       : integer range 0 to 3;
    testcase : t_testcase
  ) return std_logic;

  function calc_clk_period(
    id       : integer;
    testcase : t_testcase
  ) return time;

  function determine_octet_match(
    oct_rx  : std_logic_vector;
    oct_tx  : std_logic_vector
  ) return std_logic;

  procedure compare_v11 (
    constant  id           : in integer range 0 to 3;
    constant  testcase     : in t_testcase;
    signal    v11_rxd_int  : in std_logic;
    signal    clk          : in std_logic;
    signal    failure      : out std_logic
  );

END tb_v11_if_p;
