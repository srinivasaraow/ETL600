--------------------------------------------------------------------------
-- $Workfile: tb_v24_if_p.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_v24_if_p.vhd $
-- 
-- 11    11-07-06 9:31 Chstrue
-- R4 implementation
-- 
-- 10    12.04.05 11:21 Chalgyg
-- 4 debugging signals added.
-- 
-- 9     7.04.05 16:31 Chalgyg
-- FT1.2 protocol simulation included. (analysis of returning data is not
-- implemented yet)
-- 
-- 8     2.02.05 15:14 Chalgyg
-- abort counter more sophisticated.
-- 
-- 7     17.12.04 13:13 Chalgyg
-- Delay measurement added.
-- Data verification changed.
-- 
-- 5     23.11.04 10:46 Gygax02
-- compare_v24 deleted.
-- 
-- 4     19.11.04 14:31 Gygax02
-- Compare algorithm changed.
-- 
-- 2     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_ft1_pattern_p.all;

PACKAGE tb_v24_if_p IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4

  
  function id2chr(int: integer) return string;

  function check_delay_func(
    delay    : integer;
    id       : integer;
    testcase : t_testcase
  ) return std_logic;

  procedure compare_v24 (
    constant  id           : in integer;
    constant  testcase     : in t_testcase;
    constant  pattern      : in std_logic_vector;
    constant  repetitions  : in integer;
    signal    rxd          : in std_logic;
    signal    clk          : in std_logic;
    signal    failure      : out std_logic;
    signal    bit_det      : out std_logic;
    signal    char_det     : out std_logic;
    signal    rep_det      : out std_logic;
    signal    nr_warn      : out integer
  );

  procedure give_new_bit(
    constant id           : in integer; 
    constant testcase     : in t_testcase;
    signal   tx_clock     : in std_logic;
    signal   txd_local    : out std_logic;
    signal   txd_int      : out std_logic;
    signal   t_end_of_txd : out time;
    signal   t_set        : out std_logic
  );
         
END tb_v24_if_p;
