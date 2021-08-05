--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.frame_sync_detection.rtl
-- $Workfile: frame_sync_detection_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/frame_sync_detection_rtl.vhd $
-- 
-- 5     07-10-11 15:23 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 50    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 25    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 7     29.11.04 11:03 Unp00631
-- -- no description --
-- 
-- 1     15.10.04 17:34 Unp00631
-- before military service
-- 0     07.10.2004 11:27:47 Mathias Gloor (MGL)
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

ENTITY frame_sync_detection IS
   PORT( 
      clk               : IN     std_logic;
      preamble_check    : IN     std_logic;
      reset_n           : IN     std_logic;
      rx_agr            : IN     std_logic_vector (2 DOWNTO 0);
      frame_sync        : OUT    std_logic;
      preamble_detected : OUT    std_logic;
      sync_history      : OUT    std_logic_vector (7 DOWNTO 0)
   );

-- Declarations

END frame_sync_detection ;

--
--------------------------------------------------------------------------------
architecture rtl of frame_sync_detection is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal preamble_buf : std_logic_vector(8 downto 0) := (others => '0');
signal sync_hist : std_logic_vector(7 downto 0) := (others => '0');
signal preamble_det_i : std_logic := '0';
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    preamble_buf <= (others => '0');
    frame_sync <= '0';
    sync_hist <= "00000000";
  elsif clk'event and clk = '1' then
    if rx_agr(0)='1' then
      preamble_buf <= rx_agr(1) & preamble_buf(8 downto 1);
    end if;

    if rx_agr(2)='0' then
      sync_hist <= (others=>'0');
    elsif preamble_check = '1' then
      sync_hist <= sync_hist(6 downto 0) & preamble_det_i;
    end if;

    if sync_hist(1 downto 0)="11" then
      frame_sync <= '1';
    elsif sync_hist(3 downto 0) = "0000" then
      frame_sync <= '0';
    end if;
      
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
preamble_det_i <= '1' when (rx_agr(1) & preamble_buf(7 downto 0)) = DEF_PREAMBLE else
                     '0';  
preamble_detected <= preamble_det_i;                   
sync_history <= sync_hist;                    
--
--------------------------------------------------------------------------------
end architecture rtl;
