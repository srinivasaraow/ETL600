--
-- VHDL Architecture NSK600_lib.dt_emvtest_master_data.rtl
--
-- Created:
--          by - chstrue.UNKNOWN (CH-L-0014426)
--          at - 13:23:39 2010-10-13
--
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--

--A serial random bit stream is generated with a linear feedback register.
--The register content is used as tx data.
--When trigger to generate a new data byte arrives, it is forwarded to 
--trigger the serial transfer. After that, there is enought time to prepare the data
--before they are used from the serializer.

--To verify the received data, the tx data are stored in a memory. Each bit is verified,
--if they differ, the error is counted. When a expected byte is missing, the error counter
--is incremented by 8.

--The error counter is a BCD counter with 4 digits. BCD is used to generate ASCII which 
--will be sent to serial port 1 for observation.
--Hex 0x"0000" -> ASCI 0x"30" (concatenate a 0x3)

--The error counter is sent (and reset) to the debugging interface before a byte seqence 
--is started.

--After sending the 4 characters of the error counter, CR and LF is sent.



LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_emvtest_master_data IS
   PORT( 
      byte_nbr             : IN     std_logic_vector (8 DOWNTO 0);
      clk                  : IN     std_logic;
      prep_data            : IN     std_logic;
      put_rx_value         : IN     std_logic;
      reset_n              : IN     std_logic;
      rx_byte_missing      : IN     std_logic;
      rx_value             : IN     std_logic_vector (7 DOWNTO 0);
      start_btransfer      : OUT    std_logic;
      tx_value             : OUT    std_logic_vector (7 DOWNTO 0);
      start_sequence       : IN     std_logic;
      start_transfer_debug : OUT    std_logic;
      tx_value_debug       : OUT    std_logic_vector (7 DOWNTO 0);
      btransfer_debug_done : IN     std_logic;
      timeout_cnt_high     : IN     std_logic_vector (7 DOWNTO 0)
   );

-- Declarations

END dt_emvtest_master_data ;

--
ARCHITECTURE rtl OF dt_emvtest_master_data IS 

  signal prep_counter   : unsigned(2 downto 0);
  signal prep_ongoing   : std_logic;
  signal shift_enb      : std_logic;
  signal shift_reg      : std_logic_vector(7 downto 0);
  signal exor_1st       : std_logic;
  signal exor_2nd       : std_logic;
  signal exor_3rd       : std_logic;
  signal sample_tx_data : std_logic;
  signal data_mismatch  : std_logic;
  signal tx_data_mem_1  : std_logic_vector(7 downto 0);
  signal tx_data_mem_2  : std_logic_vector(7 downto 0);
  signal e_cnt_I        : unsigned(3 downto 0);
  signal e_cnt_X        : unsigned(3 downto 0);
  signal e_cnt_C        : unsigned(3 downto 0);
  signal e_cnt_M        : unsigned(3 downto 0);
  signal char_I         : std_logic_vector(7 downto 0);
  signal char_X         : std_logic_vector(7 downto 0);
  signal char_C         : std_logic_vector(7 downto 0);
  signal char_M         : std_logic_vector(7 downto 0);
  signal char_space     : std_logic_vector(7 downto 0);
  signal char_timeout   : std_logic_vector(7 downto 0);
  signal char_cr        : std_logic_vector(7 downto 0);
  signal char_lf        : std_logic_vector(7 downto 0);
  signal debug_seq_ong  : std_logic;
  signal debug_seq_cnt  : unsigned(3 downto 0);
  signal debug_transfer_ong : std_logiC;
  signal check_cnt          : unsigned(2 downto 0);
  signal check_ong          : std_logic;
  signal check_rx_data      : std_logic_vector(7 downto 0);
  signal check_tx_data      : std_logic_vector(7 downto 0);
  signal byte_missed        : std_logic;
  
BEGIN
  
  tx_value <= shift_reg(7 downto 0);
  
  exor_1st <= shift_reg(2) xor shift_reg(0);
  exor_2nd <= exor_1st xor shift_reg(3); 
  exor_3rd <= exor_2nd xor shift_reg(5); 
  
  data_process: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      start_btransfer <= '0'; 
      prep_counter    <= (others => '0'); 
      prep_ongoing    <= '0';   
      shift_reg       <= (others => '1'); 
      shift_enb       <= '0';
      sample_tx_data  <= '0';
    elsif (CLK'event and CLK = '1') then
      if shift_enb = '1' then 
        shift_reg <= exor_3rd & shift_reg(7 downto 1);--"00001111";--
      end if;
      --default:
      start_btransfer <= '0';
      shift_enb       <= '0';
      sample_tx_data  <= '0';  
      if prep_ongoing = '0' then
        if prep_data = '1' then
          prep_counter    <= (others => '0');
          prep_ongoing    <= '1';
          start_btransfer <= '1';
          --tx data must be ready appr. 24 clock cycles later 
          shift_enb       <= '1';
        end if;
      else
        if prep_counter = 7 then 
          prep_ongoing <= '0'; 
          sample_tx_data <= '1';         
        else
          shift_enb       <= '1';
          prep_counter <= prep_counter + 1;
        end if;
      end if;
    end if;
  end process;
  
  data_chck_process: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      tx_data_mem_1 <= (others => '0');
      tx_data_mem_2 <= (others => '0');
      data_mismatch <= '0';
      check_cnt     <= (others => '0');
      check_ong     <= '0';
      check_rx_data <= (others => '0');
      check_tx_data <= (others => '0');
      byte_missed   <= '0';
    elsif (CLK'event and CLK = '1') then
      data_mismatch <= '0';
      if sample_tx_data = '1' then 
        tx_data_mem_1 <= shift_reg(7 downto 0);
        tx_data_mem_2 <= tx_data_mem_1;
      end if;
      if check_ong = '0' then
        if put_rx_value = '1' or rx_byte_missing = '1' then
          check_ong <= '1';
          --if a byte is missing, the error counter counts 8 errors
          byte_missed <= rx_byte_missing;
          check_cnt <= (others => '0');
          check_rx_data <= rx_value;
          check_tx_data <= tx_data_mem_2;
        end if;
      else
        if check_cnt = 0 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(0) = check_tx_data(0) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 1 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(1) = check_tx_data(1) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 2 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(2) = check_tx_data(2) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 3 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(3) = check_tx_data(3) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 4 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(4) = check_tx_data(4) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 5 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(5) = check_tx_data(5) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 6 then 
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(6) = check_tx_data(6) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        elsif check_cnt = 7 then 
          check_ong   <= '0';
          byte_missed <= '0';
          if byte_missed = '1' then
            data_mismatch <= '1';
          else
            if check_rx_data(7) = check_tx_data(7) then
              data_mismatch <= '0';
            else
              data_mismatch <= '1'; 
            end if;
          end if;
        end if;
        check_cnt <= check_cnt + 1;
      end if;
    end if;
  end process;
  
  error_counter_process: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      start_transfer_debug <= '0';
      tx_value_debug       <= (others => '0');
      e_cnt_I              <= (others => '0');
      e_cnt_X              <= (others => '0');
      e_cnt_C              <= (others => '0');
      e_cnt_M              <= (others => '0');
      char_I               <= (others => '0');
      char_X               <= (others => '0');
      char_C               <= (others => '0');
      char_M               <= (others => '0');
      char_space           <= (others => '0');
      char_timeout         <= (others => '0');
      char_cr              <= (others => '0');
      char_lf              <= (others => '0');
      debug_seq_ong        <= '0';
      debug_seq_cnt        <= (others => '0');
      debug_transfer_ong   <= '0';
    elsif (CLK'event and CLK = '1') then
      start_transfer_debug <= '0';
      if debug_seq_ong = '0' then
        debug_seq_cnt <= (others => '0');
        if start_sequence = '1' then 
          debug_seq_ong <= '1';
          e_cnt_I <= (others => '0');
          e_cnt_X <= (others => '0'); 
          e_cnt_C <= (others => '0');
          e_cnt_M <= (others => '0');
          char_I  <= "0011" & std_logic_vector(e_cnt_I);
          char_X  <= "0011" & std_logic_vector(e_cnt_X);
          char_C  <= "0011" & std_logic_vector(e_cnt_C);
          char_M  <= "0011" & std_logic_vector(e_cnt_M);
          char_space   <= "00110000";
          char_timeout <= "0011" & timeout_cnt_high(3 downto 0);
          char_cr <= "00001101";--D
          char_lf <= "00001010";--A
        end if;
      else  --debug_seq_ong = '1'
        if debug_seq_cnt = 0 then
          tx_value_debug <= char_M;
        elsif debug_seq_cnt = 1 then
          tx_value_debug <= char_C;
        elsif debug_seq_cnt = 2 then
          tx_value_debug <= char_X;
        elsif debug_seq_cnt = 3 then
          tx_value_debug <= char_I;
        elsif debug_seq_cnt = 4 then
          tx_value_debug <= char_space;
        elsif debug_seq_cnt = 5 then
          tx_value_debug <= char_timeout;
        elsif debug_seq_cnt = 6 then
          tx_value_debug <= char_cr;
        elsif debug_seq_cnt = 7 then
          tx_value_debug <= char_lf;
        end if;
        if debug_transfer_ong = '0' then
          start_transfer_debug <= '1';
          debug_transfer_ong   <= '1';
        else
          if btransfer_debug_done = '1' then
            debug_transfer_ong <= '0';
            if debug_seq_cnt = 7 then
              debug_seq_ong <= '0';
            else
              debug_seq_cnt <= debug_seq_cnt + 1;
            end if;
          end if;
        end if;
      end if;
      if data_mismatch = '1' then --count error:
        if e_cnt_I = 9 then
          e_cnt_I <= (others => '0');
          if e_cnt_X = 9 then
            e_cnt_X <= (others => '0'); 
            if e_cnt_C = 9 then 
              e_cnt_C <= (others => '0');
              if e_cnt_M = 9 then
                e_cnt_M <= (others => '0');
              else
                e_cnt_M <= e_cnt_M + 1;
              end if;
            else
              e_cnt_C <= e_cnt_C + 1;
            end if;
          else
            e_cnt_X <= e_cnt_X + 1;
          end if;
        else
          e_cnt_I <= e_cnt_I + 1;
        end if;
      end if;
    end if;
  end process;
  
  
  
END ARCHITECTURE rtl;

