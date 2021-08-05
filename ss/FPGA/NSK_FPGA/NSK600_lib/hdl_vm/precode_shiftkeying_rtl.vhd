--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.precode_shiftkeying.rtl
-- $Workfile: precode_shiftkeying_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/precode_shiftkeying_rtl.vhd $
-- 
-- 4     11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 1     15.11.05 13:59 Chmaglo1
-- -- no description --
-- 
-- 29    19.01.05 16:43 Chmaglo1
-- -- no description --
-- 
-- 11    1.12.04 10:41 Unp00631
-- Migration Test 2
-- 
-- 4     27.09.04 11:40 Unp00631
-- -- no description --
-- 
-- 2     20.09.04 10:39 Unp00631
-- -- no description --
-- 
-- 1     9.09.04 15:19 Unp00631
-- -- no description --
-- 0     27.08.2004 16:31:52 Mathias Gloor (MGL)
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

ENTITY precode_shiftkeying IS
   PORT( 
      clk               : IN     std_logic;
      cnt_96            : IN     integer RANGE 0 TO 127;
      control_nsk       : IN     t_control_nsk;
      enp_1024k         : IN     std_logic;
      enp_sport         : IN     std_logic;
      reset_n           : IN     std_logic;
      tx_nsk            : IN     std_logic_vector (2 DOWNTO 0);
      status_nsk        : OUT    t_status_nsk;
      tx_position_dspif : OUT    std_logic_vector (6 DOWNTO 0);
      txd_dspif         : OUT    std_logic
   );

-- Declarations

END precode_shiftkeying ;

--
--------------------------------------------------------------------------------
architecture rtl of precode_shiftkeying is

-----------------------------------------------------------
-- Last edited:
--   110630: no changes


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
constant BUF_MAX : integer := 1;
signal buf_enabl : std_logic_vector(buf_max downto 0);
signal buf_value : std_logic_vector(buf_max downto 0);  
signal txd_1u : std_logic;                              -- 1 us resolution
signal tx_pos : std_logic_vector(6 downto 0);           -- new txd edge position 
signal txd_dspif_i : std_logic;                       
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    buf_enabl <= (others=>'0');
    buf_value <= (others=>'1');
    tx_position_dspif <= (others => '1');
    tx_pos <= (others => '1');
    txd_dspif_i <= V24_OFF;
    txd_1u <= '1';
  elsif clk'event and clk = '1' then
    -- ADPSK buffer handling
    if buf_enabl(0) = '0' or enp_sport = '1' then
      buf_enabl <= tx_nsk(0) & buf_enabl(BUF_MAX downto 1);
      buf_value <= tx_nsk(1) & buf_value(BUF_MAX downto 1);
    elsif tx_nsk(0) = '1' then
      buf_enabl(BUF_MAX) <= tx_nsk(0);
      buf_value(BUF_MAX) <= tx_nsk(1);
    end if;
    -- FSK edge detection
    if enp_1024k = '1' and cnt_96 < 96 then  -- changed MG050103
      txd_1u <= tx_nsk(1);
      if txd_1u /= tx_nsk(1) then
        tx_pos(6 downto 0) <= std_logic_vector(to_unsigned(cnt_96,7));
      end if;
    end if;
    
    -- write into DSP registers
    if enp_sport='1' then
      if control_nsk.fsk='1' and txd_1u/=txd_dspif_i then -- FSK: transition appeared
        tx_position_dspif <= tx_pos;
        txd_dspif_i <= txd_1u;
      elsif control_nsk.fsk='0' and buf_enabl(0)='1' then -- ADPSK: bit available
        tx_position_dspif <= tx_pos;
        txd_dspif_i <= buf_value(0);
      else
        tx_position_dspif <= "1111111";  -- ADPSK: no bit available / FSK: no transition
      end if;        
    end if;

  end if;
end process;

--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
txd_dspif <= txd_dspif_i;
status_nsk.rts <= not tx_nsk(2);
--------------------------------------------------------------------------------
end architecture rtl;
