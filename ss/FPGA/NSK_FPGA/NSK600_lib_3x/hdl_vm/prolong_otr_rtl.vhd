--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.prolong_otr.rtl
-- $Workfile: prolong_otr_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : chmaglo1.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /Source/FPGA/NSK_FPGA/NSK600_lib/hdl_vm/prolong_otr_rtl.vhd $
-- 
-- 1     15.11.05 11:58 Chmaglo1
-- LAN interface included,
-- div. updates belonging port9 and port10
-- 
-- 26    30.04.05 14:25 Chmaglo1
-- -- no description --
-- 
-- 1     19.01.05 16:44 Chmaglo1
-- -- no description --
-- 0     19.01.2005 15:55:45 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY prolong_otr IS
   PORT( 
      clk      : IN     std_logic;
      otr      : IN     std_logic;
      reset_n  : IN     std_logic;
      otr_long : OUT    std_logic
   );

-- Declarations

END prolong_otr ;

--
--------------------------------------------------------------------------------
architecture rtl of prolong_otr is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
constant DEF_PROLONG : integer := 16384000;

signal cnt_prolong : integer range 0 to 16777215;
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    cnt_prolong <= 0;
    otr_long <= '0';
  elsif clk'event and clk = '1' then
    if otr = '1' then
      cnt_prolong <= DEF_PROLONG;
      otr_long <= '1';
    else
      if cnt_prolong = 0 then
        otr_long <= '0';
      else
        cnt_prolong <= cnt_prolong - 1;
      end if;
    end if;
        
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture rtl;
