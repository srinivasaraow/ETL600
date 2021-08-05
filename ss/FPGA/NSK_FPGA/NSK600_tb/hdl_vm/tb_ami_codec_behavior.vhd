--------------------------------------------------------------------------
-- $Workfile: tb_ami_codec_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_ami_codec_behavior.vhd $
-- 
-- 6     11-07-06 9:21 Chstrue
-- R4 implementation
-- 
-- 5     21.09.06 11:24 Chalgyg
-- Start of AMI code failure detection is delayed until MUX has startet up
-- properly.
-- 
-- 4     20.09.06 15:00 Chalgyg
-- G.703 pattern is sended right from the beginning, even when the data is
-- transmitted not until 20 ms after starting the testcase.
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_ofdm_fpga_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_v24_if_p.all;
USE NSK600_tb.tb_real_data_p.all;
USE ieee.std_logic_unsigned.all;
USE ieee.NUMERIC_STD.all;
USE NSK600_tb.tb_v11_if_p.all;

ENTITY tb_ami_codec IS
   PORT( 
      v11_c       : IN     std_logic;
      v11_txd     : IN     std_logic;
      tx_n        : IN     std_logic;
      tx_p        : IN     std_logic;
      v11_i       : OUT    std_logic;
      v11_rxd     : OUT    std_logic;
      v11_txc     : OUT    std_logic;
      rx_n        : OUT    std_logic;
      rx_p        : OUT    std_logic;
      clockout    : IN     std_logic;
      testcase    : IN     t_testcase;
      ami_failure : OUT    std_logic
   );

-- Declarations

END tb_ami_codec ;

--
ARCHITECTURE behavior OF tb_ami_codec IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  signal tx           : std_logic := '0';
  signal direction    : std_logic := '0';
  signal detect_dir   : std_logic := '0';
  signal rx_clock     : std_logic := '0';
  signal nr_of_edges  : integer range 0 to 8;
  signal rxd_fifo     : std_logic_vector (2 downto 0) := (others=>'1');
  signal int_rx_clock : std_logic := '0';
  signal pol_error    : std_logic := '0';
  signal oct_error    : std_logic := '0';
  signal cod_error    : std_logic := '0';
  signal oct_rx       : std_logic_vector (7 downto 0) := (others=>'0');
  signal oct_tx       : std_logic_vector (7 downto 0) := (others=>'0');
  signal violation    : std_logic := '0';
  signal viol_det     : std_logic := '0';

BEGIN

  v11_i    <= '0';    -- Indication is not used
  
  tx       <= tx_p and tx_n;
  v11_txc  <= int_rx_clock;
  
  AMI_encoder: process
    variable oct_fifo_tx  : std_logic_vector (7 downto 0) := (others=>'0');
  begin
    if (pattern_v11(2,testcase) = "00000000" or port_config_registers(9,testcase)(0) = '0') then
      rx_n         <= '0';
      rx_p         <= '0';
      wait on testcase;
    else                                                   
      wait until (rx_clock = '1');
      if (v11_txd = '0') then
        oct_fifo_tx := oct_fifo_tx(6 downto 0) & '0';
        if (violation = '1') then
          oct_tx <= oct_fifo_tx;
        end if;
        if (direction = '0') then
          rx_n <= '0';
          rx_p <= '1';
          wait until (clockout = '1');
          rx_p <= '0';
          wait until (clockout = '1');
          rx_p <= '1';
          wait until (clockout = '1');
          rx_p <= '0';
        else
          rx_p <= '0';
          rx_n <= '1';
          wait until (clockout = '1');
          rx_n <= '0';
          wait until (clockout = '1');
          rx_n <= '1';
          wait until (clockout = '1');
          rx_n <= '0';
        end if;
      else
        oct_fifo_tx := oct_fifo_tx(6 downto 0) & '1';
        if (violation = '1') then
          oct_tx <= oct_fifo_tx;
        end if;
        if (direction = '0') then
          rx_n <= '0';
          rx_p <= '1';
          wait until (clockout = '1');
          wait until (clockout = '1');
          rx_p <= '0';
        else
          rx_p <= '0';
          rx_n <= '1';
          wait until (clockout = '1');
          wait until (clockout = '1');
          rx_n <= '0';
        end if;
      end if;
    end if;
  end process;

  generate_octet_timing: process
    variable counter : integer range 0 to 7 := 0;
  begin
    if (pattern_v11(2,testcase) = "00000000" or port_config_registers(9,testcase)(0) = '0') then
      wait on testcase;
    else                                                   
      wait until (rx_clock = '0');
      if (counter = 6) then
        counter    := counter + 1;
        violation  <= '1';
      elsif (counter = 7) then
        counter    := 0;
        direction  <= not(direction);
        violation  <= '0';
      else
        counter    := counter + 1;
        direction  <= not(direction);
        violation  <= '0';
      end if;
    end if;
  end process;

  generate_transmit_clock: process
    variable counter : integer range 0 to 3 := 0;
  begin
    if (pattern_v11(2,testcase) = "00000000" or port_config_registers(9,testcase)(0) = '0') then
      wait on testcase;
    else                                                   
      wait until (clockout = '1');
      if (counter = 0) then
        rx_clock <= not(rx_clock);
        counter  := counter + 1;
      else
        counter  := 0;
      end if;
    end if;
  end process;

  detect_direction: process
    variable nr_of_wrong_polarity: integer range 0 to 63 := 0;
    variable first_time          : std_logic             := '0';
    variable testcase_old        : t_testcase            := 0;
  begin
    if (pattern_v11(2,testcase) = "00000000" or port_config_registers(9,testcase)(0) = '0') then
      detect_dir <= '0';
      wait on testcase;
    else                                                   
      testcase_old := testcase;
      wait on tx;
      if ((tx_p = '1') and (tx_n = '0')) then
        detect_dir  <= '1';
      elsif ((tx_p = '0') and (tx_n = '1')) then
        detect_dir  <= '0';
      elsif ((tx_p = '0') and (tx_n = '0')) then
        if (first_time = '0') then
          assert (false)
          report "The polarity of TX-P and TX-N is inverted!"
            severity warning;
          pol_error <= '1';
          wait for 1 ns;
          pol_error <= '0';
          first_time := '1';
        end if;
      end if;
      if (testcase /= testcase_old) then
        first_time  := '0';
      end if;
    end if;
  end process;

  count_tx_edges: process
    variable last_dir     : std_logic            := '0';
    variable last_nr      : integer range 0 to 7 := 0;
    variable ti1          : time                 := 10 ns;
    variable ti2          : time                 := 10 ns;
    variable oct_fifo_rx  : std_logic_vector (7 downto 0) := (others=>'0');
    variable first_time   : std_logic            := '0';
    variable testcase_old : t_testcase           := 0;
  begin
    if (pattern_v11(2,testcase) = "00000000" or port_config_registers(9,testcase)(0) = '0') then
      nr_of_edges   <= 0;
      testcase_old  := testcase;
      wait on testcase;
    else                                                   
      if (testcase /= testcase_old) then
        wait for (mux_sync_time - 1 ms);         -- Let the MUX600 synchronise.
        first_time   := '0';
        testcase_old := testcase;
        wait until (viol_det = '0');
      end if;
      wait on tx;
      if (last_dir = detect_dir) then
        if (nr_of_edges < 8) then
          nr_of_edges <= nr_of_edges + 1;
        end if;
        ti2         := now - ti1;
        ti1         := now;
      else
        case nr_of_edges is
        when 0 =>
        when 1 =>      -- A '1' was received
          rxd_fifo <= "001";
          oct_fifo_rx := oct_fifo_rx(6 downto 0) & '1';
        when 3 =>      -- A '0' or two '1' were received
          if (ti2 > 4 us) then
            rxd_fifo <= "111";
            oct_fifo_rx := oct_fifo_rx(5 downto 0) & "11";
            oct_rx      <= oct_fifo_rx;
          else
            rxd_fifo <= "000";
            oct_fifo_rx := oct_fifo_rx(6 downto 0) & '0';
          end if;
        when 5 =>      -- A '1' and a '0' was received
          if (ti2 > 4 us) then
            rxd_fifo <= "101";
            oct_fifo_rx := oct_fifo_rx(5 downto 0) & "01";
            oct_rx      <= oct_fifo_rx;
          else
            rxd_fifo <= "110";
            oct_fifo_rx := oct_fifo_rx(5 downto 0) & "10";
            oct_rx      <= oct_fifo_rx;
          end if;
        when 7 =>      -- Two '0' were received
          rxd_fifo <= "100";
          oct_fifo_rx := oct_fifo_rx(5 downto 0) & "00";
          oct_rx      <= oct_fifo_rx;
        when others =>
          if (first_time = '0') then
            assert (false)
            report "AMI code error! Check the signaling at TX-N and TX-P."
              severity warning;
            cod_error <= '1';
            wait for 1 ns;
            cod_error <= '0';
            first_time := '1';
          end if;
        end case;
        nr_of_edges <= 0;
      end if;
      last_dir := detect_dir;
    end if;
  end process;

  give_out_rxd: process
    variable rxd_fifo_shadow : std_logic_vector (2 downto 0) := (others=>'1');
  begin
    if (pattern_v11(2,testcase) = "00000000" or port_config_registers(9,testcase)(0) = '0') then
      v11_rxd  <= '1';
      wait on testcase;
    else                                                   
      wait until (int_rx_clock = '1');
      if (rxd_fifo(2) = '0') then
        rxd_fifo_shadow(0) := rxd_fifo(0);
      else
        rxd_fifo_shadow(1 downto 0) := rxd_fifo(1 downto 0);
      end if;
      v11_rxd <= rxd_fifo_shadow(2);
      rxd_fifo_shadow(2 downto 1) := rxd_fifo_shadow(1 downto 0);
    end if;
  end process;
      
  generate_receive_clock: process
    variable ti            : time       := 10 ns;
    variable first_time    : std_logic  := '0';
    variable last_tx_0     : std_logic  := '1';
    variable last_tx_1     : std_logic  := '1';
    variable testcase_old  : t_testcase := 0;
  begin
    if (first_time = '0') then
      int_rx_clock  <= '1';
      wait for 7.8125 us;
      int_rx_clock  <= '0';
      last_tx_0     := tx;
      wait for 7.8125 us;
      last_tx_1     := tx;
      if (last_tx_0 /= last_tx_1) then
        first_time  := '1';
      end if;
    else
      testcase_old := testcase;
      wait on tx for 7.9 us;
      if ((now - ti) > 6 us) then
        int_rx_clock <= not(int_rx_clock);
        ti := now;
      end if;
      if (testcase /= testcase_old) then
        first_time  := '0';
      end if;
    end if;
  end process;

  determine_octet_timing: process
  begin
    wait on oct_rx;
    oct_error <= determine_octet_match(oct_rx,oct_tx);
    wait for 1 ns;
    if (oct_error = '1') then
      wait on testcase;
      oct_error <= '0';
    end if;
  end process;

  mark_error: process
  begin
    ami_failure  <= '0';
    wait until ((pol_error = '1') or (oct_error = '1') or (cod_error = '1'));
    ami_failure  <= '1';
    wait on testcase;
  end process;

  detect_violation: process
    variable direction_old : std_logic := '0';
  begin
    wait until (int_rx_clock = '1');
    if (direction = direction_old) then
      viol_det <= '1';
    else
      viol_det <= '0';
    end if;
    direction_old := direction;
  end process;

END ARCHITECTURE behavior;

