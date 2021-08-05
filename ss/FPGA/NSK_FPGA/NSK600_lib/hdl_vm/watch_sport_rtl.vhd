--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.watch_sport.rtl
-- $Workfile: watch_sport_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/watch_sport_rtl.vhd $
-- 
-- 6     11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 4     11-06-14 14:56 Chstrue
-- Digital voice transit implementation
-- 
-- 2     24.05.06 14:19 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 22    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 3     27.09.04 11:40 Unp00631
-- -- no description --
-- 
-- 1     20.09.04 10:39 Unp00631
-- -- no description --
-- 0     17.09.2004 16:40:00 Mathias Gloor (MGL)
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

ENTITY watch_sport IS
   PORT( 
      clk             : IN     std_logic;
      reset_n         : IN     std_logic;
      sport_interrupt : IN     std_logic;
      enp_sport       : OUT    std_logic
   );

-- Declarations

END watch_sport ;

--
--------------------------------------------------------------------------------
architecture rtl of watch_sport is


-----------------------------------------------------------
-- Last edited:
--   110630: no changes

--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal sport_debounce : integer range 0 to 4095;
signal sport_intneg : std_logic;
signal sport_intpos : std_logic;
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
if reset_n = '0' then
    sport_debounce <= 0;
    enp_sport <= '0';
  elsif clk'event and clk = '1' then
    if sport_debounce>0 then
      sport_debounce <= sport_debounce - 1;
      enp_sport <= '0';
    elsif sport_intpos='1' or sport_intneg='1' then
      sport_debounce <= 3050;  -- exact would be every 3072 system clock cycles
      enp_sport <= '1';
    else
      sport_debounce <= 0;
      enp_sport <= '0';
    end if;
  end if;


  -- bugfix since prototype4 boards have framesync-clock inverted / 
  -- this means that fs input is related on wrong clock edge!
  if reset_n = '0' then
    sport_intpos<='0';
  elsif clk'event and clk = '1' then  -- !!! positive clk edge !!!
    sport_intpos <= sport_interrupt;
  end if;
  
  if reset_n = '0' then
    sport_intneg <= '0';
  elsif clk'event and clk = '0' then  -- !!! negative clk edge !!!
    sport_intneg <= sport_interrupt;
  end if;
  
  
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture rtl;
