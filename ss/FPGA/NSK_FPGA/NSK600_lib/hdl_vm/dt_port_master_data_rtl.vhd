--------------------------------------------------------------------------------
-- $Workfile   : $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : -Unit is the interface between the serial RAM interface and the 
--                8 bit parallel interface of the byte transmission.
--               -AIS/no_config indication is also handled in this unit.
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_port_master_data_rtl.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation

-----------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_port_master_data IS
   PORT( 
      clk                          : IN     std_logic;
      dt_tx_data_bit_master        : IN     std_logic;
      prep_data                    : IN     std_logic;
      put_rx_value                 : IN     std_logic;
      reset_n                      : IN     std_logic;
      rx_byte_missing              : IN     std_logic;
      rx_value                     : IN     std_logic_vector (7 DOWNTO 0);
      voice_ch_byte_nbr            : IN     std_logic_vector (3 DOWNTO 0);
      voice_ch_nbr                 : IN     std_logic_vector (3 DOWNTO 0);
      dt_get_tx_data_bit_master    : OUT    std_logic;
      dt_put_rx_data_bit_master    : OUT    std_logic;
      dt_rx_data_bit_master        : OUT    std_logic;
      start_btransfer              : OUT    std_logic;
      tx_value                     : OUT    std_logic_vector (7 DOWNTO 0);
      voice_seq_ongoing            : IN     std_logic;
      dt_tx_current_ch_nbr_master  : OUT    std_logic_vector (3 DOWNTO 0);
      dt_tx_current_bit_nbr_master : OUT    std_logic_vector (6 DOWNTO 0);
      dt_rx_current_ch_nbr_master  : OUT    std_logic_vector (3 DOWNTO 0);
      dt_rx_current_bit_nbr_master : OUT    std_logic_vector (6 DOWNTO 0);
      dt_ch_active_master          : IN     std_logic_vector (15 DOWNTO 0);
      dt_ch_send_AIS_master        : IN     std_logic_vector (15 DOWNTO 0);
      no_slave_echo                : OUT    std_logic;
      missing_rx_sync_on_ch        : OUT    std_logic_vector (15 DOWNTO 0)
   );

-- Declarations

END dt_port_master_data ;

--
ARCHITECTURE rtl OF dt_port_master_data IS
  

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation

  
  
  
  signal get_ongoing     : std_logic;
  signal get_ongoing_m1  : std_logic;
  signal get_ongoing_m2  : std_logic;

  signal get_counter     : unsigned(2 downto 0);
  signal get_finished    : std_logic;
  signal bit_nbr         : unsigned(6 downto 0);
  signal sample_tx_value : std_logic;
  signal tx_value_i      : std_logic_vector(7 downto 0);
  
  signal put_ongoing     : std_logic;

  signal put_counter     : unsigned(2 downto 0);
  signal rx_bit_nbr      : unsigned(6 downto 0);
  signal rx_value_i      : std_logic_vector(7 downto 0);
  
--  signal no_rx_sync_on_ch : std_logic_vector(15 downto 0);
  signal still_sync_ok    : std_logic;
  signal yet_no_echo      : std_logic;
  

BEGIN
  
  dt_tx_current_bit_nbr_master <= std_logic_vector(bit_nbr);
  tx_value <= tx_value_i;
  
  get_data_process: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      start_btransfer             <= '0';
      get_finished                <= '0';
      dt_get_tx_data_bit_master   <= '0';
      get_ongoing                 <= '0';
      get_ongoing_m1              <= '0';
      get_ongoing_m2              <= '0';
      get_counter                 <= (others => '0');
      dt_tx_current_ch_nbr_master <= (others => '0');
      bit_nbr                     <= (others => '0');
      tx_value_i                  <= (others => '0');
      sample_tx_value             <= '0';
    elsif (CLK'event and CLK = '1') then
      get_ongoing_m1  <= get_ongoing;
      get_ongoing_m2  <= get_ongoing_m1;
      sample_tx_value <= get_ongoing_m2;
      start_btransfer <= get_finished;
      --default:
      get_finished <= '0';
      dt_get_tx_data_bit_master <= '0';

      --data sampling and manipulation:
      if sample_tx_value = '1' then
        if voice_seq_ongoing = '0' then
          tx_value_i <= (others => '1'); --data channels: send AIS,
        else
          if dt_ch_active_master(to_integer(unsigned(voice_ch_nbr))) = '0' then
            tx_value_i <= (others => '0'); --channel passive: send all-0
          elsif dt_ch_send_AIS_master(to_integer(unsigned(voice_ch_nbr))) = '1' then
            tx_value_i <= (others => '1'); --channel lost: send AIS all-1
          else
            tx_value_i <= tx_value_i(6 downto 0) & dt_tx_data_bit_master;
          end if;
        end if;
      end if;
      
      --genaration of get signals:
      if get_ongoing = '0' then
        if prep_data = '1' then
          get_ongoing          <= '1';
          get_counter          <= (others => '0');
          --tx_value_i           <= (others => '0');
          if voice_seq_ongoing = '1' then
            dt_get_tx_data_bit_master <= '1';
          end if;
          dt_tx_current_ch_nbr_master <= voice_ch_nbr;
          if voice_ch_byte_nbr = "0000" then
            bit_nbr <= (others => '0');
          else
            bit_nbr <= bit_nbr + 1;
          end if;
        end if;
      else
        if get_counter = 7 then 
          get_ongoing     <= '0'; 
          get_finished    <= '1';
          --tx data must be ready appr. 24 clock cycles later
          dt_get_tx_data_bit_master <= '0';         
        else
          dt_get_tx_data_bit_master <= voice_seq_ongoing;
          get_counter <= get_counter + 1;
          if voice_seq_ongoing = '1' then
            bit_nbr    <= bit_nbr + 1;
          end if;
        end if;
      end if;
    end if;
  end process;
  
  dt_put_rx_data_bit_master        <= put_ongoing;
  dt_rx_current_bit_nbr_master     <= std_logic_vector(rx_bit_nbr);
  dt_rx_data_bit_master            <= rx_value_i(7);
  
  put_data_process: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      put_ongoing                 <= '0';
      put_counter                 <= (others => '0');
      dt_rx_current_ch_nbr_master <= (others => '0');
      rx_bit_nbr                  <= (others => '0');
      rx_value_i                  <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if put_ongoing = '0' then
        if put_rx_value = '1' and voice_seq_ongoing = '1' then
          dt_rx_current_ch_nbr_master <= voice_ch_nbr;
          rx_value_i  <= rx_value;
          put_ongoing <= '1';
          put_counter <= (others => '0');
          if voice_ch_byte_nbr = "0000" then
            rx_bit_nbr <= (others => '0');
          else
            rx_bit_nbr <= rx_bit_nbr + 1;
          end if;
        end if;
      else
        if put_counter = 7 then
          put_ongoing <= '0';
        else
          put_counter <= put_counter + 1;
          rx_value_i <= rx_value_i(6 downto 0) & '0';
          rx_bit_nbr <= rx_bit_nbr + 1;
        end if;
      end if;
    end if;
  end process;
  
  
  data_status: process(reset_n, clk) 
  begin 
    if (reset_n = '0') then 
--      no_rx_sync_on_ch      <= (others => '0'); 
      still_sync_ok         <= '1'; 
      missing_rx_sync_on_ch <= (others => '0'); 
      no_slave_echo         <= '0'; 
      yet_no_echo           <= '1'; 
      no_slave_echo         <= '1'; 
    elsif (CLK'event and CLK = '1') then 
      if voice_seq_ongoing = '0' then 
        still_sync_ok <= '1'; 
        --reset for next frame: 
        yet_no_echo   <= '1'; 
      else 
        if put_rx_value = '1' then 
          if voice_ch_byte_nbr = "1000" then --last byte of channel 
            if voice_ch_nbr = "1111" then    --last byte of burst 
              if dt_ch_active_master = "0000000000000000" then --no channel enabled 
                no_slave_echo <= '0'; 
              else 
                if still_sync_ok = '1' and rx_byte_missing = '0' then 
                  no_slave_echo <= '0'; 
                else 
                  no_slave_echo <= yet_no_echo; 
                end if; 
              end if; 
            end if; 
            if still_sync_ok = '0' or rx_byte_missing = '1' then --no sync found at last byte 
              --no_rx_sync_on_ch(to_integer(unsigned(voice_ch_nbr))) <= '1'; 
              if dt_ch_active_master(to_integer(unsigned(voice_ch_nbr))) = '1' then 
                missing_rx_sync_on_ch(to_integer(unsigned(voice_ch_nbr))) <= '1'; 
              end if; 
            else 
              --no_rx_sync_on_ch(to_integer(unsigned(voice_ch_nbr))) <= '0'; 
              missing_rx_sync_on_ch(to_integer(unsigned(voice_ch_nbr))) <= '0'; 
              yet_no_echo <= '0'; 
            end if; 
            --reset for next channel: 
            still_sync_ok <= '1'; 
          else  --not the last byte of channel 
            if still_sync_ok = '1' then 
              if rx_byte_missing = '1' then 
                still_sync_ok <= '0'; 
              else 
                still_sync_ok <= '1'; 
              end if; 
            end if; 
          end if; 
        end if; 
      end if; 
    end if; 
  end process; 
 
  
END rtl;

