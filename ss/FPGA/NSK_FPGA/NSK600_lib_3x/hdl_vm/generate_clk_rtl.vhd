--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.generate_clk.rtl
-- $Workfile: generate_clk_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/generate_clk_rtl.vhd $
-- 
-- 5     3.10.06 13:23 Chmaglo1
-- -- no description --
-- 
-- 2     2.05.06 13:28 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:58 Chmaglo1
-- -- no description --
-- 
-- 69    12.07.05 14:33 Chmaglo1
-- -- no description --
-- 
-- 68    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 41    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 21    29.11.04 11:03 Unp00631
-- -- no description --
-- 0     08.10.2004 13:29:11 Mathias Gloor (MGL)
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

ENTITY generate_clk IS
   PORT( 
      basecount      : IN     integer RANGE 0 TO 16383;
      buffer_enable  : IN     std_logic;
      clk            : IN     std_logic;
      clockregulator : IN     std_logic;
      denominator    : IN     integer RANGE 0 TO 15;
      en_txc         : IN     std_logic;
      level          : IN     integer RANGE 0 TO 4095;
      numerator      : IN     integer RANGE 0 TO 15;
      reset_n        : IN     std_logic;
      rx_v24         : IN     std_logic_vector (3 DOWNTO 0);
      cnt_over16     : OUT    integer RANGE 0 TO 15;
      enp_over16     : OUT    std_logic;
      enp_rxd        : OUT    std_logic;
      rxc_int        : OUT    std_logic
   );

-- Declarations

END generate_clk ;
--
--------------------------------------------------------------------------------
architecture rtl of generate_clk is

--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal cnt_base : integer range 0 to 16383;
signal cnt_deno : integer range 0 to 15;
signal cnt_64 : integer range 0 to 63;
signal cnt_speedfract : integer range 0 to 16383; -- divider for speed range
signal rxc_last : std_logic;
signal rxc_i : std_logic;
signal shift_info : std_logic_vector (7 downto 0);
signal start_level : integer range 0 to 4095;
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    cnt_base <= 0;
    cnt_deno <= 0;
    cnt_64 <= 0;
    cnt_speedfract <= 0;
    enp_over16 <= '0';
    enp_rxd <= '0';
    rxc_last <= V24_OFF;
    shift_info<="00000000";
    start_level <= BUF_OFFSET;   -- formerly "2" MG:03.03.2005
  elsif clk'event and clk = '1' then
--    if port_enable='0' then
--      cnt_base <= 0;
--      cnt_deno <= 0;
--      cnt_64 <= 0;
--      enp_over16 <= '0';
    ----------------------------------------------------------------------------
    -- fsk: DSP clock regeneration special handling
    if buffer_enable = '0' and rx_v24(0) = '1' then 
      cnt_base <= 0;
      cnt_deno <= 0;
      cnt_64 <= 0;
      if cnt_64/4 = 0 then
        enp_over16 <= '0';
      else
        enp_over16 <= '1';
      end if;
    ----------------------------------------------------------------------------
    -- External Clock is used to synchronize RXD and TXD
--    elsif en_txc = '1' and enp_txc = '1' then -- achtung problematisch
--      cnt_base <= 0;
--      cnt_deno <= 0;
--      cnt_64 <= 0;
--      enp_over16 <= '1';
    ----------------------------------------------------------------------------
    -- adaptive clocking option
    elsif clockregulator='1' and cnt_speedfract=0 then
        --and (start_level<BUF_OFFSET or start_level>BUF_OFFSET+BUF_NORMRANGE) then
      enp_over16 <= '0';
      if start_level < BUF_OFFSET-1 then     -- underspeed
        cnt_base <= cnt_base;
      elsif start_level > BUF_OFFSET+1 then  -- overspeed
        if cnt_base = basecount then
          cnt_base <= 2;
          if cnt_64 < 63 then
            cnt_64 <= cnt_64 + 1;
          else
            cnt_64 <= 0;
          end if;
          if (cnt_64 mod 4) = 3 then
            enp_over16 <= '1';
          end if;
        elsif cnt_base+1 = basecount then
          cnt_base <= 1;
          if cnt_64 < 63 then
            cnt_64 <= cnt_64 + 1;
          else
            cnt_64 <= 0;
          end if;
          if (cnt_64 mod 4) = 3 then
            enp_over16 <= '1';
          end if;
        else
          cnt_base <= cnt_base + 2;
          cnt_64 <= cnt_64;
        end if;
        
      end if;
    ----------------------------------------------------------------------------
    -- The most normal case; count until the basis count is reached
    elsif cnt_base < basecount then
      cnt_base <= cnt_base + 1;  
      cnt_deno <= cnt_deno;   
      cnt_64 <= cnt_64;
      enp_over16 <= '0';
    else
      -- decides between pulse duration one cycle longer or one cycle shorter
      if cnt_deno < numerator then
        cnt_base <= 0;
      else
        cnt_base <= 1;
      end if;
      -- increments the denominator counter
      if cnt_deno < denominator - 1 then
        cnt_deno <= cnt_deno + 1;
      else
        cnt_deno <= 0;
      end if;
      --------------------------------------------------------------------------
      -- the common divider of 64 is handled here and reused for speed adaptation
      if cnt_64 < 63 then
        cnt_64 <= cnt_64 + 1;
      else
        cnt_64 <= 0;
      end if;
      --------------------------------------------------------------------------
      -- generates enable pulse 16 times the bitrate
      if (cnt_64 mod 4) = 3 then
        enp_over16 <= '1';
      else
        enp_over16 <= '0';
      end if; 
          
    end if;
    
    ----------------------------------------------------------------------------
    -- counter used for adaptive clocking option
    if ((cnt_speedfract>DEF_SPEEDFRACT) 
        and (start_level<BUF_OFFSET-BUF_NORMRANGE/2 or start_level>BUF_OFFSET+BUF_NORMRANGE/2)) --gross regulation
        or
       (cnt_speedfract>DEF_SPEEDFRACT_F) --fine regulation
        then
      cnt_speedfract <= 0;
    else
      cnt_speedfract <= cnt_speedfract+1;
    end if;
    
    ----------------------------------------------------------------------------
    rxc_last <= rxc_i;
    if rxc_last='0' and rxc_i='1' then -- pos and neg edge 
      enp_rxd <= '1';       -- only pos edge is shown
--report "";      
    else
      enp_rxd <= '0';
    end if;

    ----------------------------------------------------------------------------
    shift_info <= rx_v24(2) & shift_info(7 downto 1);
    if shift_info="01111111" then  -- channel start detection
      start_level <= level;
    end if;
    
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
cnt_over16 <= cnt_64/4;

rxc_i <= '1' when cnt_64 < 32 else
         '0';
rxc_int <= rxc_i;
--
--------------------------------------------------------------------------------
end architecture rtl;
