--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.map_mux_conf.rtl
-- $Workfile: map_mux_conf_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/map_mux_conf_rtl.vhd $
-- 
-- 9     07-10-11 15:23 Chmaglo1
-- -- no description --
-- 
-- 4     3.10.06 13:24 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 47    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 23    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 7     29.11.04 11:03 Unp00631
-- -- no description --
-- 
-- 1     15.10.04 17:34 Unp00631
-- before military service
-- 0     06.10.2004 17:33:00 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY map_mux_conf IS
   PORT( 
      clk           : IN     std_logic;
      control_mux   : IN     t_control_mux;
      reset_n       : IN     std_logic;
      bitcounts     : OUT    t_bitcounts;
      mux_ch_enable : OUT    std_logic_vector (26 DOWNTO 0);
      poscounts     : OUT    t_poscounts;
      testpattern   : OUT    std_logic
   );

-- Declarations

END map_mux_conf ;

--
--------------------------------------------------------------------------------
architecture rtl of map_mux_conf is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
if reset_n = '0' then
elsif clk'event and clk = '1' then
end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--mux_enable <= control_mux.tpt & control_mux.poe;
testpattern<=control_mux.tpt;

mux_ch_enable <= control_mux.mux_ch & control_mux.poe;
bitcounts <= control_mux.bitcounts;
poscounts <= control_mux.poscounts;

--
--------------------------------------------------------------------------------
end architecture rtl;
