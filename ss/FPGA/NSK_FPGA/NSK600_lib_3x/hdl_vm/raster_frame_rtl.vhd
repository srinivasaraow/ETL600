--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.raster_frame.rtl
-- $Workfile: raster_frame_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /Source/FPGA/NSK_FPGA/NSK600_lib/hdl_vm/raster_frame_rtl.vhd $
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 22    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 4     27.09.04 11:40 Unp00631
-- -- no description --
-- 
-- 2     20.09.04 10:39 Unp00631
-- -- no description --
-- 
-- 1     9.09.04 15:19 Unp00631
-- -- no description --
-- 0     25.08.2004 11:58:13 Mathias Gloor (MGL)
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

ENTITY raster_frame IS
   PORT( 
      clk       : IN     std_logic;
      enp_sport : IN     std_logic;
      reset_n   : IN     std_logic;
      cnt_96    : OUT    integer RANGE 0 TO 127;
      enp_1024k : OUT    std_logic
   );

-- Declarations

END raster_frame ;

--
--------------------------------------------------------------------------------
architecture rtl of raster_frame is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal cnt_32 : integer range 0 to 31;-- divides the system clock to 1/32000
signal cnt_96_i : integer range 0 to 127;

begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    enp_1024k <= '0';
    cnt_96_i <= 0;
    cnt_32 <= 0;
  elsif clk'event and clk = '1' then    
    if enp_sport = '1' then  -- TDM-bus frame_sync
      cnt_32 <= 0;
      enp_1024k <= '1';
      cnt_96_i <= 0;
    else
      if cnt_32 = 31 then    -- 1MHz raster
        cnt_32 <= 0;
        enp_1024k <= '1';
        if cnt_96_i = 96 then -- 0...96; 97 values possible
          cnt_96_i <= 96;  -- no action when sport_interrupt is missing
        else
          cnt_96_i <= cnt_96_i + 1;
        end if;
      else
        cnt_32 <= cnt_32 + 1;
        enp_1024k <= '0';
      end if;
    end if;

  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
cnt_96 <= cnt_96_i;

--------------------------------------------------------------------------------
end architecture rtl;
