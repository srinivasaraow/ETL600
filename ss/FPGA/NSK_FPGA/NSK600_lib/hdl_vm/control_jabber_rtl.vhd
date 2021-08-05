--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.control_jabber.rtl
-- $Workfile: control_jabber_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : chmaglo1.UNKNOWN (CH-W-2350)
-- Remarks  : 
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/control_jabber_rtl.vhd $ 
-- 
-- 13    11-07-06 9:39 Chstrue
-- R4 implementation
-- 
-- 10    11-03-08 13:58 Chstrue
-- top level with pin multiplexing
-- 
-- 9     07-10-11 15:23 Chmaglo1
-- -- no description --
-- 
-- 4     3.10.06 13:22 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 11:12 Chmaglo1
-- LAN interface added, 
-- incl. div. updates for port9 and port10
-- 
-- 5     25.10.05 17:19 Chalgyg
-- CTS signalling in port sharing device debugged.
-- 
-- 4     17.09.05 9:24 Chmaglo1
-- -- no description --
-- 
-- 2     12.07.05 15:27 Chmaglo1
-- -- no description --
-- 
-- 1     12.07.05 14:33 Chmaglo1
-- -- no description --
-- 0     12.07.2005 13:51:58 Mathias Gloor (MGL)
-- File Generated 
--

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY control_jabber IS
   PORT( 
      clk               : IN     std_logic;
      data_sensitive    : IN     std_logic;
      enp_122m          : IN     std_logic;
      enp_over16        : IN     std_logic;
      jabber_controller : IN     std_logic;
      loop_cts          : IN     std_logic;
      reset_n           : IN     std_logic;
      rts_synced        : IN     std_logic;
      rx_v24            : IN     std_logic_vector (3 DOWNTO 0);
      txd_filt          : IN     std_logic;
      cts_i             : OUT    std_logic;
      dcd_i             : OUT    std_logic;
      jabber_halt       : OUT    std_logic;
      tx_on             : OUT    std_logic
   );

-- Declarations

END control_jabber ;

--
--------------------------------------------------------------------------------
architecture rtl of control_jabber is
-----------------------------------------------------------
-- Last edited:
--   110224: StR: Rx_on removed from reset assignment
--   110221: dsg@zuehlke.com: initialization fixed
--   110630: no changes


signal txd_last : std_logic;
signal jabber_txd : std_logic;
signal cnt_txd : integer range 0 to 255;
signal jabber_active : std_logic;
signal jabber_halt_i : std_logic;
signal cnt_jab : integer range 0 to 15;
signal shift_info : std_logic_vector(7 downto 0);-- delayed rx_v24(2) needed for mux frame_break filtering / level buffering
signal rx_on : std_logic;

--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    jabber_halt_i <= '0';
    txd_last <= '0';
    jabber_txd <= '0';
    cnt_txd <= 255;
    jabber_halt_i <= '0';
    cnt_jab <= 0;
    shift_info <= (others=>'0');
    --rx_on <= '0';
  elsif clk'event and clk = '1' then
    shift_info <= rx_v24(2) & shift_info(7 downto 1); -- needed for mux frame break filtering / level buffering
    
    -- the following construct is used for data sensitive jabber control only
    txd_last <= txd_filt;
    if txd_filt = '0' and txd_last = '1' then -- negative edge detected
      cnt_txd <= 0;
      jabber_txd <= '1';
    else
      if enp_over16 = '1' then     -- actualization ones per bit
        if cnt_txd < 255 then        -- hold jabber_txd for 16 bits
          jabber_txd <= '1';
          cnt_txd <= cnt_txd + 1;
        else                         -- no more jabber on txd
          jabber_txd <= '0';
          cnt_txd <= cnt_txd;
        end if;
      end if;
    end if;

    if jabber_active='1' and jabber_controller='1' then
      if enp_122m = '1' then -- actualized every 8 sec
        if cnt_jab < 11 then         -- jabber active (96sec)
          cnt_jab <= cnt_jab + 1;
          jabber_halt_i <= '0';
        else                         -- jabber halt
          cnt_jab <= cnt_jab;
          jabber_halt_i <= '1';
        end if;
      end if;
    else                           -- currently no jabber
      cnt_jab <= 0;
      jabber_halt_i <= '0';
    end if;
    
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
jabber_active <= jabber_txd when data_sensitive = '1' else
                 not rts_synced; -- rts is low active
tx_on         <= jabber_active and not jabber_halt_i;                 
jabber_halt   <= jabber_halt_i;
rx_on         <= '0' when shift_info="00000000" else  -- filters the frame_start gap / level buffering
                 '1';
--dcd_i         <= not (rx_on and not enp_char);    -- evt. needed buffer level combination (itu-t v24 standard)
dcd_i         <= not rx_on;    -- evt. needed buffer level combination (itu-t v24 standard)
cts_i         <= V24_OFF when jabber_halt_i='1' else
                 V24_ON when data_sensitive='1' else
                 V24_OFF when rx_v24(3)='0' else
                 rts_synced;

--------------------------------------------------------------------------------
end architecture rtl;
