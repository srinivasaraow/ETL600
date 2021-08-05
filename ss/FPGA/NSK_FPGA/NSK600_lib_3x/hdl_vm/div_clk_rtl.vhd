--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.div_clk.rtl
-- $Workfile: div_clk_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/div_clk_rtl.vhd $
-- 
-- 4     3.10.06 13:22 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:58 Chmaglo1
-- -- no description --
-- 
-- 18    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 1     27.09.04 11:38 Unp00631
-- -- no description --
-- 0     20.09.2004 15:10:39 Mathias Gloor (MGL)
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

ENTITY div_clk IS
   PORT( 
      clk      : IN     std_logic;
      reset_n  : IN     std_logic;
      enp_122m : OUT    std_logic
   );

-- Declarations

END div_clk ;

--
--------------------------------------------------------------------------------
architecture rtl of div_clk is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal cnt_268M : integer range 0 to 268435455; -- 2^28
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    cnt_268M <= 0;
    enp_122m <= '0';
  elsif clk'event and clk = '1' then
    if cnt_268M < 268435455 then
      cnt_268M <= cnt_268M + 1;
      enp_122m <= '0';
    else
      cnt_268M <= 0;
      enp_122m <= '1';
    end if;
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture rtl;
