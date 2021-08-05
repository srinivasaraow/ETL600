--------------------------------------------------------------------------------
-- $Workfile   : $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : Generation of the serial transmission signals of the byte 
--               transmission (Rx/Tx). 1 startbit, 1 stopbit, 8 databit
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_port_master_btransfer_rtl.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation


--------------------------------------------------------------------------------


LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_port_master_btransfer IS
   PORT( 
      master_input    : IN     std_logic;
      clk             : IN     std_logic;
      reset_n         : IN     std_logic;
      start_btransfer : IN     std_logic;
      tx_value        : IN     std_logic_vector (7 DOWNTO 0);
      master_output   : OUT    std_logic;
      btransfer_done  : OUT    std_logic;
      rx_value        : OUT    std_logic_vector (7 DOWNTO 0);
      put_rx_value    : OUT    std_logic;
      rx_byte_missing : OUT    std_logic;
      timeout_cnt_high : OUT   std_logic_vector(7 downto 0)
   );

-- Declarations

END dt_port_master_btransfer ;

--
ARCHITECTURE rtl OF dt_port_master_btransfer IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation



  signal tx_clk_counter         : unsigned(5 downto 0);
  signal tx_bit_counter         : unsigned(4 downto 0);
  signal tx_transfer_ongoing    : std_logic;
  signal tx_bit                 : std_logic;
  signal tx_first_bit           : std_logic;
  signal tx_shift_reg           : std_logic_vector(7 downto 0);
  signal sample_rx_reg          : std_logic_vector(2 downto 0);
  signal rx_bit_f_edge          : std_logic;
  signal rx_bit                 : std_logic;
  signal rx_shift_reg           : std_logic_vector(7 downto 0);
  signal rx_passive             : std_logic;
  signal rx_searching           : std_logic;
  signal rx_clock_counter       : unsigned(5 downto 0);
  signal rx_bit_counter         : unsigned(3 downto 0);
  signal rx_search_timeout_cnt  : unsigned(7 downto 0);
  
BEGIN
  
  --pin input: 
  master_output <= tx_bit;
  rx_bit        <= master_input;

  --detect falling edge on input signal:
  rx_bit_f_edge <= '1' when sample_rx_reg(2 downto 1) = "10" else
                   '0';

  tx_transfer: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      tx_transfer_ongoing <= '0';
      tx_clk_counter      <= (others => '0');
      tx_bit_counter      <= (others => '0');
      tx_shift_reg        <= (others => '0');
      tx_bit              <= '1';
      tx_first_bit        <= '0';
      btransfer_done      <= '0';

    elsif (CLK'event and CLK = '1') then
      --default: 
      btransfer_done <= '0';
      if tx_transfer_ongoing = '0' then
        if start_btransfer = '1' then
          tx_clk_counter      <= (others => '0');
          tx_bit_counter      <= (others => '0');
          tx_transfer_ongoing <= '1';
          tx_bit              <= '0';  --start bit!
          tx_first_bit        <= '1';
        end if;
      else
        tx_clk_counter <= tx_clk_counter + 1;
        if tx_clk_counter = 24 then 
          if tx_first_bit = '1' then
            tx_shift_reg  <= tx_value;  --there are 24 clock cycles to prepare data
            tx_first_bit  <= '0';
          end if;
        elsif tx_clk_counter = 48 then
          tx_clk_counter <= (others => '0');
          tx_bit_counter <= tx_bit_counter + 1;
          --insert high level for bits after the stop bit
          tx_shift_reg <= '1' & tx_shift_reg(7 downto 1); 
          if tx_bit_counter = 8 then
            tx_bit   <= '1'; --stop bit!
          elsif tx_bit_counter = 14 then  --additional time to handle the delay of the slaves
            tx_transfer_ongoing <= '0';
            btransfer_done      <= '1';
          else
            tx_bit <= tx_shift_reg(0); --after bit 8, high level is shifted out
          end if;
        end if;
      end if;
    end if;
  end process;

  rx_transfer: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      rx_byte_missing  <= '0';
      sample_rx_reg    <= (others => '1');
      rx_passive       <= '1';
      rx_searching     <= '1';
      rx_bit_counter   <= (others => '0');
      rx_clock_counter <= (others => '0');
      rx_shift_reg     <= (others => '0');
      rx_value         <= (others => '0');
      put_rx_value     <= '0';
      rx_search_timeout_cnt <= (others => '0');
      timeout_cnt_high    <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      --default: 
      rx_byte_missing <= '0';
      put_rx_value    <= '0';
      --pin input register:
      sample_rx_reg <= sample_rx_reg(1 downto 0) & rx_bit;
      if rx_passive = '1' then
        if start_btransfer = '1' then 
          rx_passive   <= '0';
          rx_searching <= '1';
          rx_search_timeout_cnt <= (others => '0');
        end if;
      else
        if rx_searching = '1' then
          if rx_search_timeout_cnt = "11110000" then 
            rx_byte_missing <= '1';
            rx_passive      <= '1';
            rx_searching    <= '0';
            rx_value        <= (others => '1'); --AIS
            put_rx_value    <= '1';
          else
            rx_search_timeout_cnt <= rx_search_timeout_cnt + 1;
          end if;
          if rx_bit_f_edge = '1' then
            --found start bit:
            rx_searching  <= '0';
            rx_bit_counter   <= (others => '0');
            rx_clock_counter <= (others => '0');
            timeout_cnt_high <= "0011" & std_logic_vector(rx_search_timeout_cnt(7 downto 4));
          end if;
        else
          rx_clock_counter <= rx_clock_counter + 1;
          if rx_clock_counter = 24 then  --middle of the bit periode
            --sample rx:
            rx_shift_reg <= sample_rx_reg(2) & rx_shift_reg(7 downto 1); 
          elsif rx_clock_counter = 48 then  --end of the bit periode
            rx_bit_counter <= rx_bit_counter + 1;
            rx_clock_counter <= (others => '0');
            if rx_bit_counter = 8 then 
              rx_value     <= rx_shift_reg;
              put_rx_value <= '1';
              rx_passive   <= '1';
              rx_searching <= '0';
            end if;  
          end if;
        end if;
      end if;
    end if;
  end process;

END rtl;

