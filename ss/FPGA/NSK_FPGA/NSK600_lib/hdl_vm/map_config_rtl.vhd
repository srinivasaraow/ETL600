--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.map_config.rtl
-- $Workfile: map_config_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/map_config_rtl.vhd $
-- 
-- 8     11-07-06 9:39 Chstrue
-- R4 implementation
-- 
-- 5     11-03-08 13:58 Chstrue
-- top level with pin multiplexing
-- 
-- 4     3.10.06 13:23 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:58 Chmaglo1
-- -- no description --
-- 
-- 64    12.07.05 14:33 Chmaglo1
-- -- no description --
-- 
-- 63    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 37    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 18    29.11.04 11:03 Unp00631
-- -- no description --
-- 
-- 10    20.09.04 10:39 Unp00631
-- -- no description --
-- 0     15.09.2004 16:06:50 Mathias Gloor (MGL)
-- File Generated
--

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY map_config IS
   PORT( 
      clk                   : IN     std_logic;
      control_v24           : IN     t_control_v24_x;
      jabber_halt           : IN     std_logic;
      reset_n               : IN     std_logic;
      always_on             : OUT    std_logic;
      asynchronous          : OUT    std_logic;
      basecount             : OUT    integer RANGE 0 TO 16383;
      buffer_enable         : OUT    std_logic;
      bypass_uar            : OUT    std_logic;
      char_format           : OUT    integer RANGE 0 TO 7;
      clockregulator        : OUT    std_logic;
      data_sensitive        : OUT    std_logic;
      denominator           : OUT    integer RANGE 0 TO 15;
      en_rxc                : OUT    std_logic;
      en_txc                : OUT    std_logic;
      inverse_clock_tx      : OUT    std_logic;
      invert_all_circuits   : OUT    std_logic;
      invert_clock_polarity : OUT    std_logic;
      jabber_controller     : OUT    std_logic;
      loop_cts              : OUT    std_logic;
      loop_tx_rx            : OUT    std_logic;
      mode                  : OUT    std_logic_vector (1 DOWNTO 0);
      numerator             : OUT    integer RANGE 0 TO 15;
      status_v24            : OUT    t_status_v24_x
   );

-- Declarations

END map_config ;

--
--------------------------------------------------------------------------------
architecture rtl of map_config is

-----------------------------------------------------------
-- Last edited:
--   110221: dsg@zuehlke.com: initialization fixed
--   110630: no changes


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal mode_i : std_logic_vector(1 downto 0);
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    mode_i <= "00";
  elsif clk'event and clk = '1' then
    mode_i <= control_v24.tpt & ((control_v24.clr or control_v24.mux) and control_v24.poe);
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
  mode <= mode_i;
  buffer_enable <= control_v24.mux or control_v24.clr;
  clockregulator <= control_v24.clr;
--  port_enable <= control_v24.poe;
  loop_tx_rx <= control_v24.ltr;
  loop_cts <= control_v24.lrc;
  always_on <= control_v24.aon;
  bypass_uar <= control_v24.fsk;
  asynchronous <= control_v24.asy;

  jabber_controller <= control_v24.jac;    
  data_sensitive <= control_v24.das;
  en_rxc <= control_v24.rxc;
  en_txc <= control_v24.txc; -- changed MG060814
  invert_clock_polarity <= control_v24.icp;
  inverse_clock_tx <= control_v24.ict;
  invert_all_circuits <= control_v24.iac;
  char_format <= control_v24.chl;
  basecount <= control_v24.basecount;
  numerator <= control_v24.numerator;
  denominator <= control_v24.denominator;
  
  -- Status information
  status_v24.jah <= jabber_halt;
--
--------------------------------------------------------------------------------
end architecture rtl;
