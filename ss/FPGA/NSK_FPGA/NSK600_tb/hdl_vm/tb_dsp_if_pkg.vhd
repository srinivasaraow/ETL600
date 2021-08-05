--------------------------------------------------------------------------
-- $Workfile: tb_dsp_if_pkg.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_dsp_if_pkg.vhd $
-- 
-- 20    11-07-06 9:27 Chstrue
-- -- no description --
-- 
-- 19    11-07-06 9:25 Chstrue
-- R4 implementation
-- 
-- 18    26.01.06 15:53 Chalgyg
-- G.703 LOS signal path testing added.
-- 
-- 17    18.04.05 15:59 Chalgyg
-- - TPR register added.
-- - Register address range adjusted.
-- 
-- 16    10.02.05 15:49 Chalgyg
-- Switched to DSP interface communication with address bus.
-- 
-- 15    14.01.05 11:02 Chalgyg
-- Preparation for future bitrate testcase commented out.
-- 
-- 13    13.12.04 13:50 Chalgyg
-- procedure dsp_check_comm() changed.
-- 
-- 12    8.12.04 10:50 Chalgyg
-- Signal 'configured' added to procedure configure.
-- 
-- 11    7.12.04 14:23 Chalgyg
-- new procedure added.
-- 
-- 10    24.11.04 10:23 Gygax02
-- RUD added in configuration of each testcase.
-- 
-- 9     20.10.04 11:46 Gygax02
-- -- no description --
-- 
-- 7     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------



LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.all;

library nsk600_tb;
use nsk600_tb.tb_config_p.all;
use nsk600_tb.tb_control_p.all;
use nsk600_tb.tb_ft1_pattern_p.all;

PACKAGE tb_dsp_if_p IS
-----------------------------------------------------------
-- Last edited:
--   110218, StR:
--      -FPGA configuration for V.11 clocking: changed procedure dsp_configure_fpga 
--       to set TXC bit in TPR register to '0', "no clock input"
--   110630: bug fix (of old bugs) and adapt to R4

  type t_channel_array is array (0 to 3) of std_logic_vector(7 downto 0);
  type adress_array is array (0 to 10) of std_logic_vector(11 downto 0);
  type rud_adress_array is array (0 to 3) of std_logic_vector(11 downto 0);
  
  constant RUD_LEN   : integer := 8;
  constant RSIG_LEN  : integer := 4;
  constant PCR_LEN   : integer := 8;
  constant CCR_LEN   : integer := 7;
  constant TPR_LEN   : integer := 4;
  constant BRR_LEN   : integer := 6;

  constant IDNT_AD  : std_logic_vector(11 DOWNTO 0):= "000000000000";
  constant TSIG_AD  : std_logic_vector(11 DOWNTO 0):= "000000000001";
  constant TUD1_AD  : std_logic_vector(11 DOWNTO 0):= "000000000010";
  constant TUD2_AD  : std_logic_vector(11 DOWNTO 0):= "000000000011";
  constant TUD3_AD  : std_logic_vector(11 DOWNTO 0):= "000000000100";
  constant TUD4_AD  : std_logic_vector(11 DOWNTO 0):= "000000000101";
  constant RUD_AD   : rud_adress_array  := ("000000000110","000000000111","000000001000","000000001001");
  constant RSIG_AD  : std_logic_vector(11 DOWNTO 0):= "000000001010";
  constant PCR_AD   : adress_array      := ("000000010100","000000011001","000000011110","000000100011","000000101000","000000101101","000000110010","000000110111","000000111100","000001000001","000001000110");
  constant CCR_AD   : adress_array      := ("000000010101","000000011010","000000011111","000000100100","000000101001","000000101110","000000110011","000000111000","000000111101","000001000010","000001000111");
  constant TPR_AD   : adress_array      := ("000000010110","000000011011","000000100000","000000100101","000000101010","000000101111","000000110100","000000111001","000000111110","000001000011","000001001000");
  constant BRR_AD   : adress_array      := ("000000010111","000000011100","000000100001","000000100110","000000101011","000000110000","000000110101","000000111010","000000111111","000001000100","000001001001");
  constant OSR_AD   : adress_array      := ("000000011000","000000011101","000000100010","000000100111","000000101100","000000110001","000000110110","000000111011","000001000000","000001000101","000001001010");

  type t_dsp_port_sig is record
    dsp_cs_n  : std_logic;
    dsp_wr_n  : std_logic;
    dsp_rd_n  : std_logic;
    dsp_addr  : std_logic_vector(11 downto 0);
  end record;

  function stdv2str(vec  : std_logic_vector) return string;

  function slv2int(slv : std_logic_vector) return integer;

  function int2slv(int_value : integer) return std_logic_vector;

  procedure dsp_write (
    constant  data      : in std_logic_vector(7 downto 0);
    constant  address   : in std_logic_vector(11 downto 0);
    signal    dsp_sig   : out t_dsp_port_sig;
    signal    dsp_data  : out std_logic_vector(7 downto 0)
  );

  procedure dsp_read (
    constant  address   : in std_logic_vector(11 downto 0);
    constant  dsp_data  : in std_logic_vector(7 downto 0);
    variable  data_out  : out std_logic_vector(7 downto 0);
    signal    dsp_sig   : out t_dsp_port_sig
  );

  procedure dsp_passive (
    signal    dsp_data  : out std_logic_vector(7 downto 0);
    signal    dsp_sig   : out t_dsp_port_sig
  );

  procedure dsp_compare (
    constant  address        : in std_logic_vector(11 downto 0);
    constant  length         : in integer;
    signal    dsp_data       : inout std_logic_vector(7 downto 0);
    signal    dsp_sig        : out t_dsp_port_sig;
    variable  dsp_port_fail  : out std_logic
  );

  procedure dsp_check_comm (
    signal    dsp_data       : inout std_logic_vector(7 downto 0);
    signal    dsp_sig        : out t_dsp_port_sig;
    variable  dsp_port_fail  : out std_logic
  );

  procedure dsp_configure_fpga (
    constant  testcase       : in t_testcase;
    signal    dsp_data       : inout std_logic_vector(7 downto 0);
    signal    dsp_sig        : out t_dsp_port_sig;
    signal    configured     : out std_logic
  );

  procedure dsp_read_all_TUD (
    signal    dsp_data  : inout std_logic_vector(7 downto 0);
    variable  data_out  : out t_channel_array;
    signal    dsp_sig   : out t_dsp_port_sig
  );

  procedure dsp_write_all_RUD (
    constant  data      : t_channel_array;
    signal    dsp_sig   : out t_dsp_port_sig;
    signal    dsp_data  : out std_logic_vector(7 downto 0)
  );

  procedure dsp_handle_flag0 (
    signal    flag0     : out std_logic
  );

  procedure reset_fpga (
    signal    fpga_reset_n: out std_logic
  );

END tb_dsp_if_p;
