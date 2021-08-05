--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.sync_tx.rtl
-- $Workfile: sync_tx_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : chmaglo1.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/sync_tx_rtl.vhd $
-- 
-- 4     3.10.06 13:24 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:58 Chmaglo1
-- -- no description --
-- 
-- 23    30.04.05 14:25 Chmaglo1
-- -- no description --
-- 0     25.01.2005 17:34:22 Mathias Gloor (MGL)
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

ENTITY sync_tx IS
   PORT( 
      clk        : IN     std_logic;
      reset_n    : IN     std_logic;
      rts_i      : IN     std_logic;
      txd_i      : IN     std_logic;
      rts_synced : OUT    std_logic;
      txd_filt   : OUT    std_logic;
      txc_filt   : OUT    std_logic;
      txc_i      : IN     std_logic
   );

-- Declarations

END sync_tx ;

--
--------------------------------------------------------------------------------
architecture rtl of sync_tx is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
  signal txd_samps: std_logic_vector(2 downto 0) := (others => V24_OFF);
  signal txc_samps: std_logic_vector(2 downto 0) := (others => V24_OFF);
  signal rts_samps: std_logic_vector(2 downto 0) := (others => V24_OFF);
--
begin  -- rtl

-------------------------------------------------------------------------------
-- PROCESSES
-------------------------------------------------------------------------------
  process (CLK, RESET_N)
  begin
    if RESET_N = '0' then
      txd_samps <= (others => '1');
      rts_samps <= (others => '1');
      txc_samps <= (others => '1');
      txd_filt <= '1';
      txc_filt <= '1';
    elsif CLK'event and CLK = '1' then
      --Multi Stage Synchronizer (against metastabilities)
      txd_samps <= txd_samps(1 downto 0) & txd_i;
      rts_samps <= rts_samps(1 downto 0) & rts_i;
      txc_samps <= txc_samps(1 downto 0) & txc_i;
--      if asynchronous='1' then

      txd_filt <= txd_samps(2);
      txc_filt <= txc_samps(2);

--      else
--        if txc_samps(2)=invert_clock_tx and txc_samps(1)/=invert_clock_tx then
--          txd_filt <= txd_samps(2);
--          enp_txc <= '1';
--        else
--          enp_txc <= '0';
--        end if;
--      end if; 
    end if;
  end process;
-------------------------------------------------------------------------------
-- CONCURRENT LOGIC
-------------------------------------------------------------------------------
  rts_synced <= rts_samps(1);
--
--------------------------------------------------------------------------------
end architecture rtl;
