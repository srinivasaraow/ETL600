--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.sync_in.rtl
-- $Workfile: sync_in_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/sync_in_rtl.vhd $
-- 
-- 4     11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 2     20.09.04 10:39 Unp00631
-- -- no description --
-- 
-- 1     9.09.04 15:19 Unp00631
-- -- no description --
-- 0     30.08.2004 18:08:12 Mathias Gloor (MGL)
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

ENTITY sync_in IS
   PORT( 
      clk               : IN     std_logic;
      enp_sport         : IN     std_logic;
      reset_n           : IN     std_logic;
      rx_position_dspif : IN     std_logic_vector (6 DOWNTO 0);
      rxd_dspif         : IN     std_logic;
      position_synced   : OUT    std_logic_vector (6 DOWNTO 0);
      rxd_synced        : OUT    std_logic
   );

-- Declarations

END sync_in ;

--
--------------------------------------------------------------------------------
architecture rtl of sync_in is

-----------------------------------------------------------
-- Last edited:
--   110630: no changes


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal position_synced_i : std_logic_vector(6 downto 0);
signal rxd_synced_i : std_logic;

begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    position_synced_i <= (others=>'1');
    rxd_synced_i <= '1';
  elsif clk'event and clk = '1' then
    if enp_sport='1' then
      position_synced_i <= rx_position_dspif;
      rxd_synced_i <= rxd_dspif;
    else
      position_synced_i <= position_synced_i;
      rxd_synced_i <= rxd_synced_i;
    end if;
    
    
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
rxd_synced <= rxd_synced_i;
position_synced <= position_synced_i;

--------------------------------------------------------------------------------
end architecture rtl;
