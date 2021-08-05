--
-- VHDL Architecture NSK600_lib.dt_emvtest_slave_receiver.rtl
--
-- Created:
--          by - CHSTRUE.UNKNOWN (CH-L-0014426)
--          at - 13:16:51 2010-10-14
--
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_emvtest_slave_receiver IS
   PORT( 
      slave_input             : IN     std_logic;
      clk                     : IN     std_logic;
      reset_n                 : IN     std_logic;
      rx_value                : OUT    std_logic_vector (7 DOWNTO 0);
      put_rx_value            : OUT    std_logic;
      get_tx_data             : OUT    std_logic;
      cfg_local_transit_n_vec : IN     std_logic_vector (15 DOWNTO 0);
      current_o4cv_ch         : OUT    std_logic_vector (3 DOWNTO 0);
      current_o4cv_ch_byte    : OUT    std_logic_vector (3 DOWNTO 0);
      tx_data                 : IN     std_logic_vector (7 DOWNTO 0);
      slave_output            : OUT    std_logic;
      slave_output_enb        : OUT    std_logic
   );

-- Declarations

END dt_emvtest_slave_receiver ;

--
ARCHITECTURE rtl OF dt_emvtest_slave_receiver IS


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
  signal o4cv_section_active : std_logic;
  signal section_byte_count  : unsigned(3 downto 0);
  signal o4cv_ch_counter     : unsigned(3 downto 0);
  signal o4cv_ch_enabled     : std_logic;
  signal check_cfg           : std_logic;
  signal transfer_ongoing    : std_logic;
  signal channel_active      : std_logic;
  signal prepare_tx          : std_logic;
  
  
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
  
  
  tx_rx_transfer: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      searching_startbit  <= '1';
      start_bit           <= '0';
      first_byte          <= '1';
      get_tx_data         <= '0';
      rx_shift_reg        <= (others => '0');
      tx_shift_reg        <= (others => '0');
      rx_value            <= (others => '0');
      put_rx_value        <= '0';
      sample_rx_reg       <= (others => '0');
      byte_counter        <= (others => '0');
      bit_counter         <= (others => '0');
      clock_counter       <= (others => '0');
      tx_bit              <= '1';
      o4cv_section_active <= '0';
      o4cv_ch_counter     <= (others => '0');
      section_byte_count  <= (others => '0');
      o4cv_ch_enabled     <= '0';
      slave_output_enb    <= '0';
      check_cfg           <= '0';
      transfer_ongoing    <= '0';
      channel_active      <= '0';
      prepare_tx          <= '0';
    elsif (CLK'event and CLK = '1') then
      --default:
      check_cfg     <= '0';
      get_tx_data   <= '0';
      put_rx_value  <= '0';
      --pin input register:
      sample_rx_reg <= sample_rx_reg(1 downto 0) & rx_bit;
      if searching_startbit = '1' then 
        if clock_counter = "1111111111" then 
          --initialize:
          byte_counter        <= (others => '0');
          first_byte          <= '1';
          o4cv_section_active <= '0';
          section_byte_count  <= (others => '0');
          o4cv_ch_counter     <= (others => '0');
        else
          clock_counter <= clock_counter + 1;
        end if;
        if rx_bit_f_edge = '1' then
          check_cfg <= '1'; --check if channel is configured to be sent
          --found start bit:
          searching_startbit <= '0';
--          start_bit          <= '1';--
          prepare_tx         <= '1';
          bit_counter <= (others => '0');
          clock_counter <= (others => '0');

          if first_byte = '1' then
            byte_counter <= (others => '0');
            first_byte   <= '0';
          else
            byte_counter <= byte_counter + 1;
          end if;

          --check for o4cv area:
          if byte_counter = (O4CV_SECTION_START) then
            o4cv_section_active <= '1';
            section_byte_count  <= (others => '0');
            o4cv_ch_counter     <= (others => '0');
          end if;
          if o4cv_section_active = '1' then
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
        if o4cv_section_active = '1' then
          if cfg_local_transit_n_vec(0) = '1' and o4cv_ch_counter = 0 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(1) = '1' and o4cv_ch_counter = 1 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(2) = '1' and o4cv_ch_counter = 2 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(3) = '1' and o4cv_ch_counter = 3 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(4) = '1' and o4cv_ch_counter = 4 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(5) = '1' and o4cv_ch_counter = 5 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(6) = '1' and o4cv_ch_counter = 6 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(7) = '1' and o4cv_ch_counter = 7 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(8) = '1' and o4cv_ch_counter = 8 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(9) = '1' and o4cv_ch_counter = 9 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(10) = '1' and o4cv_ch_counter = 10 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(11) = '1' and o4cv_ch_counter = 11 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(12) = '1' and o4cv_ch_counter = 12 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(13) = '1' and o4cv_ch_counter = 13 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(14) = '1' and o4cv_ch_counter = 14 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
          if cfg_local_transit_n_vec(15) = '1' and o4cv_ch_counter = 15 then 
            get_tx_data      <= '1';
            slave_output_enb <= '1';
--            tx_bit           <= '0';--start bit
            channel_active   <= '1';
          end if;
        else
          --activate transmission for the databytes before the O4CV section:
          get_tx_data      <= '1';
          slave_output_enb <= '1';
--          tx_bit           <= '0';--start bit
          channel_active   <= '1';
        end if;
      end if;
      if transfer_ongoing = '1' then
        clock_counter <= clock_counter + 1;
        if clock_counter = 24 then  --middle of the bit periode
          --if channel_active = '1' then
            --sample rx:
            rx_shift_reg <= sample_rx_reg(2) & rx_shift_reg(7 downto 1); 
          --end if;
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
            --tx_bit       <= '1';--stop bit
            rx_value     <= rx_shift_reg;
            put_rx_value <= '1';
          end if;  
          if bit_counter = 9 then 
            tx_bit       <= '1';--stop bit
            --searching_startbit <= '1';
            --o4cv_ch_enabled    <= '0';
            --slave_output_enb   <= '0';
            --transfer_ongoing   <= '0';
            --channel_active     <= '0';
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

