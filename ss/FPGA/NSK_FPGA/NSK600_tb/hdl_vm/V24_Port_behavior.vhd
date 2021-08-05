--------------------------------------------------------------------------
-- $Workfile: V24_Port_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/V24_Port_behavior.vhd $
-- 
-- 39    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 38    6.09.06 13:50 Chalgyg
-- - Maximum frequency deviation -0.7 ppm shifted, because of clock
-- inacurracy.
-- - Delay is only measured if the first pattern is transmitted
-- sucessfully.
-- 
-- 37    26.04.05 14:10 Chalgyg
-- Handle CTS only, if port is configured for HW-handshaking.
-- 
-- 36    25.04.05 13:14 Chalgyg
-- - Delay measurement fixed.
-- Now it is possible to check characters downto 7 bits and characters
-- with all '0'.
-- 
-- 35    13.04.05 10:38 Chalgyg
-- Delay measurement debugged.
-- 
-- 34    12.04.05 11:25 Chalgyg
-- 4 debugging signals added.
-- 
-- 33    7.04.05 16:32 Chalgyg
-- FT1.2 protocol simulation included.
-- 
-- 32    1.03.05 10:03 Chalgyg
-- mux_sync_time introduced for configuration.
-- 
-- 31    24.02.05 15:23 Chalgyg
-- - RTS signalling enhanced.
-- - Take clock from DUT if present for data and delay check. Otherwise
-- take internal clock.
-- - Frequency measurement now works for rates under 200 bps.
-- 
-- 30    16.02.05 11:02 Chalgyg
-- - Enhanced for MUX operation.
-- - RTS improved.
-- 
-- 29    10.02.05 15:52 Chalgyg
-- control_signalling changed.
-- 
-- 28    2.02.05 15:15 Chalgyg
-- compare_v24 changed.
-- 
-- 27    24.01.05 14:23 Chalgyg
-- Frequency deviation limit is not fix anymore.
-- 
-- 26    14.01.05 15:57 Chalgyg
-- - Don't generate data if mode 'o'.
-- - Jitter must be lower than 1%.
-- 
-- 25    5.01.05 11:30 Chalgyg
-- It's now possible to not loop in FSK mode.
-- 
-- 24    20.12.04 11:27 Chalgyg
-- Start delay added.
-- 
-- 23    17.12.04 13:14 Chalgyg
-- Delay measurement added.
-- Data verification changed.
-- 
-- 21    13.12.04 13:55 Chalgyg
-- Wait with RTS toggling until DSP interface is configured.
-- 
-- 20    8.12.04 13:32 Chalgyg
-- In ADPSK mode always compare RXD with DSPdummy.
-- 
-- 19    8.12.04 10:57 Chalgyg
-- Wait for configured DUT before starting with datastream.
-- 
-- 18    3.12.04 12:55 Chalgyg
-- -- no description --
-- 
-- 17    2.12.04 11:06 Chalgyg
-- -- no description --
-- 
-- 16    1.12.04 14:56 Chalgyg
-- Bug removed.
-- 
-- 15    1.12.04 10:53 Chalgyg
-- Frequency detection improved.
-- 
-- 14    25.11.04 11:39 Gygax02
-- Clock detection improved.
-- 
-- 13    24.11.04 10:22 Gygax02
-- Frequency detection improved.
-- 
-- 12    23.11.04 10:49 Gygax02
-- process compare_v24 deleted and rxd detection made as before.
-- 
-- 10    19.11.04 14:32 Gygax02
-- Compare algorithm changed.
-- 
-- 8     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 7     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_v24_if_p.all;
USE NSK600_tb.tb_real_data_p.all;
USE NSK600_tb.tb_ft1_pattern_p.all;

ENTITY V24_Port IS
   GENERIC(
      id         : integer range 0 to 5
   );
   PORT( 
      rxd        : IN     std_logic;
      testcase   : IN     t_testcase;
      cts        : IN     std_logic;
      dcd        : IN     std_logic;
      rck        : IN     std_logic;
      rxd_dsp    : IN     std_logic := '0';
      configured : IN     std_logic;
      txd        : OUT    std_logic;
      txd_int    : OUT    std_logic;
      txd_busy   : OUT    std_logic;
      rts        : OUT    std_logic;
      failure    : OUT    std_logic
   );
END V24_Port ;

ARCHITECTURE behavior OF V24_Port IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4




  signal txd_local         : std_logic;
  signal rts_int           : std_logic := '1';
  signal rts_intermed      : std_logic := '1';
  signal txd_busy_int      : std_logic;
  signal receive_busy      : std_logic;
  signal rts_busy          : std_logic;
  signal tx_clock          : std_logic;
  signal comp_clock        : std_logic;
  signal jitter            : time;
  signal glitch            : std_logic;
  signal data_error_found  : std_logic := '0';
  signal sig_error_found   : std_logic := '0';
  signal freq_error_found  : std_logic := '0';
  signal delay_error_found : std_logic := '0';
  signal t_dummy_set       : std_logic := '0';
  signal t_set             : std_logic := '0';
  signal t_end             : time      := 1 ns;
  signal t_end_of_txd      : time      := 1 ns;
  signal t_end_of_txd_dummy: time      := 1 ns;
  signal indication_sup    : std_logic := '0';
  signal compare_pattern   : std_logic_vector(7 downto 0);
  signal failure_debug     : std_logic := '0';
  signal repetitions       : integer   := 0;
  signal    bit_det      :  std_logic;
  signal    char_det     :  std_logic;
  signal    rep_det      :  std_logic;
  signal    nr_warn      :  integer;

BEGIN
  
  txd           <= txd_local xor glitch;
  txd_busy      <= txd_busy_int or rts_busy or receive_busy;
  rts_intermed  <= (rts_int and (not(txd_busy_int or receive_busy))) when (mode(id,testcase) /= 's') else not(txd_busy_int);
  rts           <= rts_intermed when (port_config_registers((1+id),testcase)(6)='0') else '1';
  comp_clock    <= tx_clock when (clock_config_registers((1+id),testcase)(2) = '0') else rck;

  generate_transmit_clock: process
    variable ti            : time := 10 us;
    variable testcase_old  : t_testcase;
    variable first_time    : std_logic := '0';
  begin
    if (baudrate2config(baudrate_value(id,testcase)) = 0) then
      tx_clock   <= '1';
      first_time := '0';
      wait on testcase;
    else
      if ((first_time = '0') and (start_delay(testcase) /= 0)) then
        first_time := '1';
        ti         := start_delay(testcase) * 1 us;
        wait for ti;
      end if;
      tx_clock     <= not(tx_clock);
      ti           := ((deviation2time(baudrate_deviation(id,testcase)) * baudrate2config(baudrate_value(id,testcase))) + jitter) / 2;
      testcase_old := testcase;
      wait on testcase for ti;
      if (testcase /= testcase_old) then
        tx_clock   <= '1';
        first_time := '0';
        wait for 1 ns;
      end if;
    end if;
  end process;

  generate_txd: process
    variable datastream     : std_logic_vector(9 downto 0) := "0000000000";
    variable end_reached    : std_logic := '0';
    variable delay_measured : std_logic := '0';
  begin
    if ((pattern_v24(id,testcase) = "00000000") or (mode(0,testcase) = 'o')) then
      txd_busy_int  <= '0';
      txd_local     <= '1';
      txd_int       <= '1';
    else
      txd_busy_int  <= '1';
      datastream    := '0' & pattern_v24(id,testcase) & '1';  -- Startbit, 8 bit data and stopbit.
      txd_local     <= '1';
      txd_int       <= '1';
      wait until (configured = '1');
      if ((mode(id,testcase) = 'm') or ((mode(id,testcase) = 's') and (switchbox_modem(testcase) = 'm'))) then
        wait for mux_sync_time;    -- Let the MUX600 synchronise.
      end if;
      if (pattern_protocol(testcase) = 's') then    -- Stream the pattern with 1 start, 8 data and 1 stopbit.
        for j in 1 to nr_of_repetitions_v24(id,testcase) loop
          for i in 0 to (datastream'length-1) loop
            wait until (tx_clock='0');
            if (end_reached = '1' and delay_measured = '0') then
              t_end_of_txd   <= now;
              end_reached    := '0';
              delay_measured := '1';
              t_set        <= '1';
              wait for 1 ps;
              t_set        <= '0';
            end if;        
            txd_local    <= datastream((datastream'length-1)-i);
            txd_int      <= datastream((datastream'length-1)-i);
          end loop;
          end_reached := '1';
        end loop;
      elsif (pattern_protocol(testcase) = 'f') then  -- Fit data in FT1.2 protocol.
        give_new_bit(id,testcase,tx_clock,txd_local,txd_int,t_end_of_txd,t_set);
      end if;
      wait until (tx_clock='0');
      txd_busy_int  <= '0';
      delay_measured := '0';
      end_reached    := '0';
    end if;
    wait on testcase;
  end process;

  generate_jitter: process
    variable ti:    time;
    variable sign:  integer := 1;
  begin
    if (jitter_frequency(id,testcase) /= 0) then
      jitter  <= 1 ns * (sign*jitter_amplitude(id,testcase)/2);
      ti      := 1000 ms / (2*jitter_frequency(id,testcase));
      sign    := -sign;
      wait on testcase for ti;
    else
      jitter  <= 0 ns;
      wait on testcase;
    end if;
  end process;

  generate_glitch: process
    variable toff:  time;
    variable ton:   time;
  begin
    if ((glitch_on_duration(id,testcase) /= 0) and (glitch_off_duration(id,testcase) /= 0)) then
      glitch  <= '0';
      toff    := 1 us * glitch_off_duration(id,testcase);
      wait for toff;
      glitch  <= '1';
      ton     := 1 ps * glitch_on_duration(id,testcase);
      wait for ton;
    else
      glitch  <= '0';
      wait on testcase;
    end if;
  end process;

  mark_error: process
  begin
    failure  <= '0';
    wait until ((data_error_found = '1') or (sig_error_found = '1') or (freq_error_found = '1') or (delay_error_found = '1'));
    failure  <= '1';
    wait on testcase;
  end process;

  compare_looped_data: process
  begin
    if ((check_regenerated_rxd(testcase)= '1') and (mode(id,testcase) /= 'o')) then
      receive_busy  <= '1';
      if ((mode(id,testcase) = 'm') or ((mode(id,testcase) = 's') and (switchbox_modem(testcase) = 'm'))) then
        wait for mux_sync_time;    -- Let the MUX600 synchronise.
      else
        wait for 1 ps;
      end if;
      compare_v24(id,testcase,compare_pattern,repetitions,rxd,comp_clock,data_error_found,bit_det,char_det,rep_det,nr_warn);
      wait for 1 ps;
    end if;
    data_error_found  <= '0';
    receive_busy      <= '0';
    wait on testcase;
  end process;

 control_signalling: process                          -- changes RTS and waits until CTS and DCD changes as well.
    variable t1     : time;
    variable t2     : time;
    variable delta  : time;
    variable deltai : integer;
    variable lock   : std_logic := '0';
  begin
    if ((check_signalling(testcase) = '0') or (id > 3)) then
      rts_busy  <= '0';
      rts_int   <= '1';
    else
      rts_busy  <= '1';
      wait until (configured = '1');
      for i in 0 to 9 loop
        rts_int  <= not(rts_int);
        wait for 100 ns;
        wait until (rts_int = cts) for 93.9 us;       -- RTS must be looped within 93.75 us.
        if ((rts_int /= cts) and (lock = '0')) then
          report "CTS on channel" & id2chr(id) & "has errors"
            severity warning;
          sig_error_found <= '1';
          wait for 1 ns;
          sig_error_found <= '0';
          lock            := '1';
        end if;
      end loop;
      lock     := '0';
      rts_busy <= '0';
    end if;
    wait on testcase;
  end process;

  check_rck_period: process
    variable ti1            : time := 10 us;
    variable ti2            : time := 10 us;
    variable frequency_sum  : real := 0.0;
    variable frequency      : real := 0.0;
    variable frequency_mean : real := 0.0;
    variable frequency_dev  : real := 0.0;
    variable rck_period     : time := 0 ps;
    variable supress        : std_logic := '0';
    variable index          : integer := 0;
    variable jitter         : real := 0.0;
    variable jitter_max     : real := 0.0;
    variable jitter_min     : real := 0.0;
    variable jitter_prozent : real := 0.0;
  begin
    if ((clock_config_registers((id+1),testcase)(2) = '1') and (mode(id,testcase) /= 'o') and (check_rxc(testcase) = '1')) then  -- Only if RCK output is enabled.
      if (supress = '0') then
        if ((mode(id,testcase) = 'm') or ((mode(id,testcase) = 's') and (switchbox_modem(testcase) = 'm'))) then
          wait for mux_sync_time;       -- Let the MUX600 synchronise.
        end if;
        for i in 0 to 5 loop    -- let the clock stabilise for 6 cycles.
          wait until (rck = '1');
        end loop;
        supress       := '1';
        index         := 0;
        frequency_sum := 0.0;
      end if;
      ti1            := now;
      wait until (rck = '1');
      ti2            := now;
      rck_period     := ti2 - ti1;
      if (rck_period > 0 ps) then
        frequency      := 1.0e11 / real(rck_period / 10 ps);
        frequency_sum  := frequency_sum + frequency;
        index          := index + 1;
        jitter         := 1.0e9 * ((1.0 / real(baudrate_value(id,testcase))) - (1.0 /frequency));
        if (jitter_max < jitter) then
          jitter_max   := jitter;
        end if;
        if (jitter_min > jitter) then
          jitter_min   := jitter;
        end if;
      end if;
      if (index = (repetitions*(compare_pattern'length))) then
        frequency_mean := frequency_sum / real(index);
        frequency_dev  := 1.0e6 * (1.0 - (frequency_mean / real(baudrate_value(id,testcase))));
        if ((frequency_dev > (real(max_v24_deviation)-0.7)) or (frequency_dev < (real(-1 * max_v24_deviation)-0.7))) then    -- Baudrate deviation has to be smaller than 400 ppm (less 0.7 ppm deviation from imprecise clock generation from TB).
          assert (false)
            report "Frequency of V.24 port" & id2chr(id) & "is " & int2str(integer(frequency_mean)) & " Hz and should be " & int2str(baudrate_value(id,testcase)) & " Hz."
            severity warning;
          freq_error_found <= '1';
          wait for 1 ns;
          freq_error_found <= '0';
        end if;
        jitter_prozent := real(baudrate_value(id,testcase)) * (jitter_max + (-1.0 * jitter_min)) * 1.0e-6;
        if (jitter_prozent > 10.0) then    -- Jitter has to be smaller than 1% of Baudrate
          assert (false)
          report "Maximum jitter of V.24 port" & id2chr(id) & "should be smaller than 1% of baudrate but is +" & int2str(integer(jitter_max)) & " / -" & int2str(integer(-1.0 * jitter_min)) & " ns which equals to " & int2str(integer(jitter_prozent)) & " per mil."
            severity warning;
          freq_error_found <= '1';
          wait for 1 ns;
          freq_error_found <= '0';
        end if;
        supress    := '0';
        jitter_max := 0.0;
        jitter_min := 0.0;
        wait on testcase;
      end if;
    else
      wait on testcase;
    end if;
  end process;

  measure_delay: process
    variable rxd_container: std_logic_vector(9 downto 0) := (others => '1');
    variable delay        : integer   := 0;
  begin
    if ((check_delay(testcase) = '1') and (mode(id,testcase) /= 'o')) then
      wait until (comp_clock='1');
      rxd_container := rxd_container(8 downto 0) & rxd;
      if (((rxd_container(8 downto 0) = (compare_pattern & '1')) and (pattern_protocol(testcase) = 's')) or
          ((rxd_container(7 downto 0) = (compare_pattern(6 downto 0) & '1')) and (pattern_protocol(testcase) = 'f'))) then
        wait until (rxd = '0');
        if (t_end > 0 fs) then
          delay := (now - t_end) / 1 us;
          delay_error_found <= check_delay_func(delay,id,testcase);
          wait for 1 ns;
          delay_error_found <= '0';
          wait on testcase;
        end if;
      end if;
    else
      wait on testcase;
    end if;
  end process;

  generate_pattern: process    -- Generate pattern to compare for the delay measurement.
    variable comp_pat_buf: std_logic_vector((pattern_v24(id,testcase)'length-1) downto 0) := (others => '0');
  begin
    if (pattern_protocol(testcase) = 's') then    -- Pattern is streamed with 1 start, 8 data and 1 stopbit.
      case mode(id,testcase) is
        when 'f'     => if (loop_data_on_dsp_if(testcase) = '1') then
                          compare_pattern <= pattern_v24(id,testcase);
                          repetitions     <= nr_of_repetitions_v24(id,testcase);
                        else
                          compare_pattern <= pattern_dsp_if(id,testcase);       
                          repetitions     <= nr_of_repetitions_dsp(id,testcase);
                        end if;
        when 'm'     => compare_pattern <= pattern_v24(id,testcase);
                        repetitions     <= nr_of_repetitions_v24(id,testcase);
        when 'a'     => compare_pattern <= pattern_dsp_if(id,testcase);
                        repetitions     <= nr_of_repetitions_dsp(id,testcase);
        when 's'     => comp_pat_buf    := pattern_v24(0,testcase);
                        if (mode(1,testcase)= 's') then
                          comp_pat_buf    := comp_pat_buf or pattern_v24(1,testcase);
                        end if;
                        if (mode(2,testcase)= 's') then
                          comp_pat_buf    := comp_pat_buf or pattern_v24(2,testcase);
                        end if;
                        if (mode(3,testcase)= 's') then
                          comp_pat_buf    := comp_pat_buf or pattern_v24(3,testcase);
                        end if;
                        if (mode(4,testcase)= 's') then
                          comp_pat_buf    := comp_pat_buf or pattern_v24(4,testcase);
                        end if;
                        if (mode(5,testcase)= 's') then
                          comp_pat_buf    := comp_pat_buf or pattern_v24(5,testcase);
                        end if;
                        compare_pattern <= comp_pat_buf;
                        repetitions     <= nr_of_repetitions_v24(id,testcase);
        when others  => compare_pattern <= (others=>'0');
                        repetitions     <= 1;
      end case;
    elsif (pattern_protocol(testcase) = 'f') then   -- Pattern is streamed in FT1.2 protocol.
      compare_pattern(6 downto 0)  <= ft1_pattern(id)((ft1_pattern(id)'length-1) downto (ft1_pattern(id)'length-7));
      repetitions                  <= nr_repetitions(id) * (ft1_pattern(id)'length / nr_data_bits(id));
    end if;
    wait on testcase;
  end process;

  measure_time_dummy_rxd: process
  begin
    t_dummy_set         <= '0';
    wait until (rxd_dsp = '0');
    t_end_of_txd_dummy  <= now;
    t_dummy_set         <= '1';
    wait for 1 ns;
  end process;

  chose_end_time: process
  begin
    wait until ((t_set = '1') or (t_dummy_set = '1'));
    if ((mode(id,testcase) = 'm') or ((mode(id,testcase) = 'f') and (loop_data_on_dsp_if(testcase) = '1'))) then
      t_end <= t_end_of_txd;
    else
      t_end <= t_end_of_txd_dummy;
    end if;
  end process;

END ARCHITECTURE behavior;

