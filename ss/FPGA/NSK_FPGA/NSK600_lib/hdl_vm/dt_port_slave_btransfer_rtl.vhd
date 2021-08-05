--------------------------------------------------------------------------------
-- $Workfile   : $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : Sampling of the Rx signal until the startbit is detected. Then
--               collecting 8 databit and respons with 8 databit if channel 
--               is terminated on this slave. Timeout generation to detect the
--               end of the transit sequence and to generate the transmission lost 
--               status bit.
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_port_slave_btransfer_rtl.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation

-----------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_port_slave_btransfer IS
   PORT( 
      slave_input                 : IN     std_logic;
      clk                         : IN     std_logic;
      reset_n                     : IN     std_logic;
      rx_value                    : OUT    std_logic_vector (7 DOWNTO 0);
      put_rx_value                : OUT    std_logic;
      get_tx_data                 : OUT    std_logic;
      dt_ch_active_slave          : IN     std_logic_vector (15 DOWNTO 0);
      current_o4cv_ch             : OUT    std_logic_vector (3 DOWNTO 0);
      current_o4cv_ch_byte        : OUT    std_logic_vector (3 DOWNTO 0);
      o4cv_section_active         : OUT    std_logic;
      tx_data                     : IN     std_logic_vector (7 DOWNTO 0);
      slave_output                : OUT    std_logic;
      slave_output_enb            : OUT    std_logic;
      voice_burst_started_slave   : OUT    std_logic;
      voice_burst_finished_slave  : OUT    std_logic;
      slave_rx_timeout            : OUT    std_logic
   );

-- Declarations

END dt_port_slave_btransfer ;

--
ARCHITECTURE rtl OF dt_port_slave_btransfer IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation


  signal sample_rx_reg       : std_logic_vector(2 downto 0);
  signal rx_bit              : std_logic;
  signal rx_bit_f_edge       : std_logic;
  signal rx_shift_reg        : std_logic_vector(7 downto 0);
  signal searching_startbit  : std_logic;
  signal start_bit           : std_logic;
  signal first_byte          : std_logic;
  signal clock_counter       : unsigned(9 downto 0);
  signal bit_counter         : unsigned(3 downto 0);
  signal byte_counter        : unsigned(8 downto 0);
  signal tx_shift_reg        : std_logic_vector(7 downto 0);
  signal tx_bit              : std_logic;
  signal o4cv_section_active_i : std_logic;
  signal section_byte_count  : unsigned(3 downto 0);
  signal o4cv_ch_counter     : unsigned(3 downto 0);
  signal o4cv_ch_enabled     : std_logic;
  signal check_cfg           : std_logic;
  signal transfer_ongoing    : std_logic;
  signal channel_active      : std_logic;
  signal prepare_tx          : std_logic;
  signal rx_timeout_cnt      : unsigned(16 downto 0);
  
  
  constant O4CV_SECTION_START : integer := 287;
  constant O4CV_CHANNEL_SIZE  : integer := 9; 



BEGIN
  
  --pin input/output: 
  rx_bit       <= slave_input;
  slave_output <= tx_bit;
  
  --detect falling edge on input signal:
  rx_bit_f_edge <= '1' when sample_rx_reg(2 downto 1) = "10" else
                   '0';
  
  current_o4cv_ch      <= std_logic_vector(o4cv_ch_counter);
  current_o4cv_ch_byte <= std_logic_vector(section_byte_count);
  o4cv_section_active  <= o4cv_section_active_i;
  
  tx_rx_transfer: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      searching_startbit         <= '1';
      start_bit                  <= '0';
      first_byte                 <= '1';
      get_tx_data                <= '0';
      rx_shift_reg               <= (others => '0');
      tx_shift_reg               <= (others => '0');
      rx_value                   <= (others => '0');
      put_rx_value               <= '0';
      sample_rx_reg              <= (others => '0');
      byte_counter               <= (others => '0');
      bit_counter                <= (others => '0');
      clock_counter              <= (others => '0');
      tx_bit                     <= '1';
      o4cv_section_active_i      <= '0';
      o4cv_ch_counter            <= (others => '0');
      section_byte_count         <= (others => '0');
      o4cv_ch_enabled            <= '0';
      slave_output_enb           <= '0';
      check_cfg                  <= '0';
      transfer_ongoing           <= '0';
      channel_active             <= '0';
      prepare_tx                 <= '0';
      voice_burst_started_slave  <= '0';
      voice_burst_finished_slave <= '0';
      slave_rx_timeout           <= '0';
      rx_timeout_cnt             <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      --default:
      check_cfg     <= '0';
      get_tx_data   <= '0';
      put_rx_value  <= '0';
      voice_burst_started_slave  <= '0';
      voice_burst_finished_slave <= '0';
      --pin input register:
      sample_rx_reg <= sample_rx_reg(1 downto 0) & rx_bit;
      if rx_timeout_cnt = "11111111111111111" then 
        slave_rx_timeout <= '1';
      else 
        rx_timeout_cnt <= rx_timeout_cnt + 1;
      end if;
      if searching_startbit = '1' then 
        if clock_counter = "1111111111" then 
          --initialize:
          byte_counter                <= (others => '0');
          first_byte                  <= '1';
          o4cv_section_active_i       <= '0';
          voice_burst_finished_slave  <= '1';
          section_byte_count          <= (others => '0');
          o4cv_ch_counter             <= (others => '0');
        else
          clock_counter <= clock_counter + 1;
        end if;
        if rx_bit_f_edge = '1' then
          --reset timeout counter:
          rx_timeout_cnt   <= (others => '0');
          slave_rx_timeout <= '0';
          check_cfg <= '1'; --check if channel is configured to be sent
          --found start bit:
          searching_startbit <= '0';
          prepare_tx         <= '1';
          bit_counter        <= (others => '0');
          clock_counter      <= (others => '0');
          
          if first_byte = '1' then
            byte_counter <= (others => '0');
            first_byte   <= '0';
          else
            byte_counter <= byte_counter + 1;
          end if;

          --check for o4cv area:
          if byte_counter = (O4CV_SECTION_START) then
            o4cv_section_active_i     <= '1';
            section_byte_count        <= (others => '0');
            o4cv_ch_counter           <= (others => '0');
            voice_burst_started_slave <= '1';
          end if;
          if o4cv_section_active_i = '1' then
            if section_byte_count = (O4CV_CHANNEL_SIZE - 1) then
              section_byte_count <= (others => '0');
              o4cv_ch_counter <= o4cv_ch_counter + 1;
            else
              section_byte_count <= section_byte_count + 1;
            end if;  
          end if; 
        end if;
      end if;
      if check_cfg = '1' then
        clock_counter <= clock_counter + 1;
        transfer_ongoing <= '1';
        if o4cv_section_active_i = '1' then
          get_tx_data      <= '1';--for all transit channels
          if dt_ch_active_slave(0) = '1' and o4cv_ch_counter = 0 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(1) = '1' and o4cv_ch_counter = 1 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(2) = '1' and o4cv_ch_counter = 2 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(3) = '1' and o4cv_ch_counter = 3 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(4) = '1' and o4cv_ch_counter = 4 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(5) = '1' and o4cv_ch_counter = 5 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(6) = '1' and o4cv_ch_counter = 6 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(7) = '1' and o4cv_ch_counter = 7 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(8) = '1' and o4cv_ch_counter = 8 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(9) = '1' and o4cv_ch_counter = 9 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(10) = '1' and o4cv_ch_counter = 10 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(11) = '1' and o4cv_ch_counter = 11 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(12) = '1' and o4cv_ch_counter = 12 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(13) = '1' and o4cv_ch_counter = 13 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(14) = '1' and o4cv_ch_counter = 14 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
          if dt_ch_active_slave(15) = '1' and o4cv_ch_counter = 15 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
            channel_active   <= '1';
          end if;
        else --o4cv section not active:
          --activate transmission for the databytes before the O4CV section:
          get_tx_data      <= '1';
          slave_output_enb <= '1';
          channel_active   <= '1';
        end if;
      end if;
      if transfer_ongoing = '1' then
        clock_counter <= clock_counter + 1;
        if clock_counter = 24 then  --middle of the bit periode
          --sample rx:
          rx_shift_reg <= sample_rx_reg(2) & rx_shift_reg(7 downto 1); 
        elsif clock_counter = 48 then  --end of the bit periode
          if channel_active = '1' then
            if prepare_tx = '1' then
              tx_bit     <= '0'; --start bit
              start_bit  <= '1';
              prepare_tx <= '0';
            else
              if start_bit = '1' then 
                tx_shift_reg <= '1' & tx_data(7 downto 1);
                tx_bit       <= tx_data(0);
              else
                tx_shift_reg <= '1' & tx_shift_reg(7 downto 1);
                tx_bit       <= tx_shift_reg(0);
              end if;
              start_bit <= '0';
            end if;
          end if;
          
          --end bit duration
          bit_counter   <= bit_counter + 1;
          clock_counter <= (others => '0');
          if bit_counter = 8 then 
            rx_value     <= rx_shift_reg;
            put_rx_value <= '1';
          end if;  
          if bit_counter = 9 then 
            tx_bit       <= '1'; --stop bit
          end if;  
          if bit_counter = 10 then 
            searching_startbit <= '1';
            o4cv_ch_enabled    <= '0';
            slave_output_enb   <= '0';
            transfer_ongoing   <= '0';
            channel_active     <= '0';
          end if;  
        end if;
      end if;
    end if;
  end process;
  
  
  
  
END rtl;



