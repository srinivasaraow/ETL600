--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.compare_position.rtl
-- $Workfile: compare_position_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/compare_position_rtl.vhd $
-- 
-- 4     11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 34    14.06.05 14:19 Chmaglo1
-- Aenderung für control_nsk.dcd
-- 
-- 33    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 29    19.01.05 16:43 Chmaglo1
-- -- no description --
-- 
-- 10    29.11.04 11:02 Unp00631
-- -- no description --
-- 
-- 2     20.09.04 10:39 Unp00631
-- -- no description --
-- 
-- 1     9.09.04 15:19 Unp00631
-- -- no description --
-- 0     30.08.2004 18:12:34 Mathias Gloor (MGL)
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

ENTITY compare_position IS
   PORT( 
      clk             : IN     std_logic;
      cnt_96          : IN     integer RANGE 0 TO 127;
      control_nsk     : IN     t_control_nsk;
      enp_1024k       : IN     std_logic;
      position_synced : IN     std_logic_vector (6 DOWNTO 0);
      reset_n         : IN     std_logic;
      rxd_synced      : IN     std_logic;
      rx_nsk          : OUT    std_logic_vector (3 DOWNTO 0)
   );

-- Declarations

END compare_position ;

--
--------------------------------------------------------------------------------
architecture rtl of compare_position is
-----------------------------------------------------------
-- Last edited:
--   110630: no changes

signal rx_i : std_logic_vector(1 downto 0);
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    rx_i <= "10";
  elsif clk'event and clk = '1' then
    if (cnt_96=position_synced and enp_1024k = '1') then
      rx_i(0) <= '1';
      rx_i(1) <= rxd_synced;
    else
      rx_i(0) <= '0';
      rx_i(1) <= rx_i(1);
    end if;
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
rx_nsk <= not control_nsk.cts & control_nsk.dcd & rx_i;  
--------------------------------------------------------------------------------
end architecture rtl;
