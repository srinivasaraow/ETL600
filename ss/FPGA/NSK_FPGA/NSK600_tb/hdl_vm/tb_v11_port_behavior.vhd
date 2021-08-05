--------------------------------------------------------------------------
-- $Workfile: tb_v11_port_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_v11_port_behavior.vhd $
-- 
-- 26    11-07-06 9:21 Chstrue
-- R4 implementation
-- 
-- 25    21.09.06 13:36 Chalgyg
-- Additional TB delay of G.703 port compensated.
-- 
-- 24    6.09.06 9:38 Chalgyg
-- - Maximum frequency deviation -0.7 ppm shifted, because of clock
-- inacurracy.
-- - Default start-up port clock speeded-up to 5 MHz.
-- 
-- 23    15.08.06 16:20 Chalgyg
-- V.11 Control signal now provides TX clock if enabled by frequency
-- offset in  slave mode.
-- 
-- 22    8.02.06 12:45 Chalgyg
-- Maximum allowable G.703 jitter increased to 5%.
-- 
-- 21    19.01.06 16:21 Chalgyg
-- G.703 interface integrated.
-- 
-- 20    10.01.06 9:07 Chalgyg
-- LAN interface is now supported.
-- 
-- 19    1.03.05 10:02 Chalgyg
-- - mux_sync_time introduced for configuration.
-- - Start-up clock bug removed.
-- - Delay measurement bug removed.
-- 
-- 18    24.02.05 15:18 Chalgyg
-- Wait bug removed.
-- 
-- 17    16.02.05 10:59 Chalgyg
-- Clock measurement delayed for 20 ms, if MUX is configured.
-- 
-- 16    15.02.05 15:37 Chalgyg
-- - Active edge now depends on CCR.IAC.
-- - Transmit and receive delay for MUX enlarged to 20 ms.
-- 
-- 15    15.02.05 11:59 Chalgyg
-- Delays sending and receiving data in MUX mode for 10 ms.
-- 
-- 14    7.02.05 11:51 Chalgyg
-- - Delay measurement added.
-- - Control handling is more sophisticated.
-- 
-- 13    2.02.05 15:12 Chalgyg
-- v11_c = '1' if ON. Like on the target.
-- 
-- 12    25.01.05 14:20 Chalgyg
-- Test vector removed.
-- 
-- 11    25.01.05 14:16 Chalgyg
-- Generate TXD delayed, because of wrong clock at the beginning.
-- 
-- 10    24.01.05 14:22 Chalgyg
-- Frequency deviation limit is not fix anymore.
-- 
-- 9     21.01.05 14:29 Chalgyg
-- Active clk edge now depending on CCR.ICP
-- 
-- 8     18.01.05 14:56 Chalgyg
-- TXC frequency detection enhanced.
-- Jitter detection added.
-- 
-- 6     21.10.04 13:23 Gygax02
-- -- no description --
-- 
-- 4     14.10.04 16:22 Gygax02
-- -- no description --
-- 
-- 3     8.10.04 14:34 Gygax02
-- -- no description --
-- 
-- 2     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 1     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_v24_if_p.all;
USE NSK600_tb.tb_v11_if_p.all;

ENTITY tb_v11_port IS
   GENERIC(
      id         : integer range 0 to 3
   );
   PORT( 
      testcase   : IN     t_testcase;
      v11_i      : IN     std_logic;
      v11_rxd    : IN     std_logic;
      v11_txc    : IN     std_logic;
      configured : IN     std_logic;
      v11_c      : OUT    std_logic;
      v11_txd    : OUT    std_logic;
      clockout   : OUT    std_logic;
      txd_busy   : OUT    std_logic;
      failure    : OUT    std_logic
   );
END ENTITY tb_v11_port ;

ARCHITECTURE behavior OF tb_v11_port IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4



  signal v11_txd_local     : std_logic := '1';
  signal glitch            : std_logic := '0';
  signal txc_period        : time      := 0 ps;
  signal jitter_gen        : time      := 0 ps;
  signal tx_clock          : std_logic := '0';
  signal tx_clock_quarter  : std_logic := '0';
  signal generate_busy     : std_logic := '0';
  signal receive_busy      : std_logic := '0';
  signal freq_error_found  : std_logic := '0';
  signal data_error_found  : std_logic := '0';
  signal delay_error_found : std_logic := '0';
  signal supress           : std_logic := '0';
  signal t_end             : time      := 10 ns;

BEGIN

  v11_txd     <= v11_txd_local xor glitch;
  txd_busy    <= generate_busy or receive_busy;
  clockout    <= tx_clock_quarter;
  v11_c       <= tx_clock when (baudrate_deviation_v11(id,testcase) /= 0) else
                 not(clock_config_registers(7+id,testcase)(5)) when (port_config_registers(7+id,testcase)(4) = '1') else
                 (generate_busy or receive_busy);
  
  generate_quarter_transmit_clock: process
    variable ti            : time := 100 ns;
    variable testcase_old  : t_testcase;
    variable first_time    : std_logic := '0';
  begin
    if (baudrate_deviation_v11(id,testcase) = 0) then      -- MUX is clock master (normal case)
      wait until (v11_txc = '1');
      tx_clock_quarter  <= '1';
      if (txc_period > 0 ps) then
        ti  := (txc_period / 8) + jitter_gen;
      end if;
      wait for ti;
      tx_clock_quarter  <= '0';
      for k in 1 to 3 loop
        wait for ti;
        tx_clock_quarter  <= '1';
        wait for ti;
        tx_clock_quarter  <= '0';
      end loop;
    else                                                   -- Testbench is clock master (G.703 case)
      if (first_time = '0') then
        first_time       := '1';
        tx_clock_quarter <= '1';
        ti               := calc_clk_period(id,testcase) + jitter_gen;
        wait for ti;
      end if;
      tx_clock_quarter     <= not(tx_clock_quarter);
      testcase_old := testcase;
      wait on testcase for ti;
      if (testcase /= testcase_old) then
        tx_clock_quarter   <= '1';
        first_time := '0';
        wait for 1 ns;
      end if;
    end if;
  end process;

  generate_transmit_clock: process
    variable counter : integer range 0 to 1 := 0;
  begin
    wait until (tx_clock_quarter = '1');
    if (counter = 0) then
      tx_clock <= not(tx_clock);
      counter  := counter + 1;
    else
      counter  := 0;
    end if;
  end process;

  generate_txd: process
    variable end_reached:    std_logic := '0';
    variable delay_measured: std_logic := '0';
  begin
    if (pattern_v11(id,testcase) = "00000000" or port_config_registers((7+id),testcase)(0) = '0') then
      generate_busy <= '0';
      v11_txd_local <= '1';
    else
      generate_busy <= '1';
      wait until (configured = '1');
      wait until (supress = '1');
      for j in 1 to nr_of_repetitions_v11(id,testcase) loop
        for i in 0 to (pattern_v11(id,testcase)'length-1) loop
          wait until (tx_clock=(clock_config_registers((7+id),testcase)(5) xnor clock_config_registers((7+id),testcase)(3)));
          if (end_reached = '1' and delay_measured = '0') then
            t_end          <= now;
            end_reached    := '0';
            delay_measured := '1';
          end if;        
          v11_txd_local  <= pattern_v11(id,testcase)((pattern_v11(id,testcase)'length-1)-i);
        end loop;
        end_reached := '1';
      end loop;
      wait until (tx_clock=(clock_config_registers((7+id),testcase)(5) xnor clock_config_registers((7+id),testcase)(3)));
      generate_busy  <= '0';
      v11_txd_local  <= '1';
      delay_measured := '0';
      end_reached    := '0';
    end if;
    wait on testcase;
  end process;

  generate_jitter: process
    variable ti:    time;
    variable sign:  integer := 1;
  begin
    if (jitter_frequency_v11(id,testcase) /= 0) then
      jitter_gen  <= 1 ns * (sign*jitter_amplitude_v11(id,testcase)/8);
      ti          := 1000 ms / (2*jitter_frequency_v11(id,testcase));
      sign        := -sign;
      wait on testcase for ti;
    else
      jitter_gen  <= 0 ns;
      wait on testcase;
    end if;
  end process;

  generate_glitch: process
    variable toff:  time;
    variable ton:   time;
  begin
    if ((glitch_on_duration_v11(id,testcase) /= 0) and (glitch_off_duration_v11(id,testcase) /= 0)) then
      glitch  <= '0';
      toff    := 1 us * glitch_off_duration_v11(id,testcase);
      wait for toff;
      glitch  <= '1';
      ton     := 1 ps * glitch_on_duration_v11(id,testcase);
      wait for ton;
    else
      glitch  <= '0';
      wait on testcase;
    end if;
  end process;

  compare_looped_data: process
  begin
    if (port_config_registers((7+id),testcase)(0) = '1') then
      receive_busy  <= '1';
      if (mode_v11(testcase) = 'm') then
        wait for 20 ms;        -- Let the MUX600 synchronise.
      else
        wait for 1 ps;
      end if;
      compare_v11(id,testcase,v11_rxd,v11_txc,data_error_found);
      wait for 1 ps;
    end if;
    data_error_found  <= '0';
    receive_busy      <= '0';
    wait on testcase;
  end process;

  check_txc_period: process
    variable ti1            : time := 10 us;
    variable ti2            : time := 10 us;
    variable frequency_sum  : real := 0.0;
    variable frequency      : real := 0.0;
    variable frequency_mean : real := 0.0;
    variable frequency_dev  : real := 0.0;
    variable index          : integer := 0;
    variable int_repetition : integer := 1;
    variable int_length     : integer := 1;    
    variable jitter         : real := 0.0;
    variable jitter_max     : real := 0.0;
    variable jitter_min     : real := 0.0;
    variable jitter_promill : real := 0.0;
  begin
    if (port_config_registers((7+id),testcase)(0) = '1') then  -- Only if this V.11 port and frequency check is enabled.
      if (supress = '0') then
        if (mode_v11(testcase) = 'm') then
          wait for mux_sync_time;         -- Let the MUX600 synchronise.
        end if;
        for i in 0 to 5 loop      -- let the clock stabilise for 6 cycles.
          ti1            := now;
          wait until (v11_txc = '1');
          ti2            := now;
          txc_period     <= ti2 - ti1;
        end loop;
        int_repetition := nr_of_repetitions_v11(id,testcase);
        int_length     := pattern_v11(id,testcase)'length;
        supress        <= '1';
        index          := 0;
        frequency_sum  := 0.0;
      end if;
      ti1            := now;
      wait until (v11_txc = '1');
      ti2            := now;
      txc_period     <= ti2 - ti1;
      wait for 1 ps;
      if (check_rxc(testcase) = '1') then
        if (txc_period > 0 ps) then
          frequency      := 1.0e12 / real(txc_period / 1 ps);
          frequency_sum  := frequency_sum + frequency;
          index          := index + 1;
          jitter         := 1.0e9 * ((1.0 / real(baudrate_value_v11(id,testcase))) - (1.0 /frequency));
          if (jitter_max < jitter) then
            jitter_max   := jitter;
          end if;
          if (jitter_min > jitter) then
            jitter_min   := jitter;
          end if;
        end if;
        if (index = (int_repetition*int_length)) then
          frequency_mean := frequency_sum / real(index);
          frequency_dev  := 1.0e6 * (1.0 - (frequency_mean / real(baudrate_value_v11(id,testcase))));
          case id is
          when 0|1 =>      -- V.11 port 1 or 2
            if ((frequency_dev > (real(max_v11_deviation)-0.7)) or (frequency_dev < (real(-1 * max_v11_deviation)-0.7))) then    -- Baudrate deviation has to be smaller than 700 ppm (less 0.7 ppm deviation from imprecise clock generation from TB).
              assert (false)
                report "Frequency of V.11 port" & id2chr(id) & "is " & int2str(integer(frequency_mean)) & " Hz and should be " & int2str(baudrate_value_v11(id,testcase)) & " Hz."
                severity warning;
              freq_error_found <= '1';
              wait for 1 ns;
              freq_error_found <= '0';
            end if;
          when 2 =>        -- G.703 port
            if ((frequency_dev > (real(max_g703_deviation)-0.7)) or (frequency_dev < (real(-1 * max_g703_deviation)-0.7))) then    -- Baudrate deviation has to be smaller than 700 ppm (less 0.7 ppm deviation from imprecise clock generation from TB).
              assert (false)
                report "Frequency of the G.703 port is " & int2str(integer(frequency_mean)) & " Hz and should be " & int2str(baudrate_value_v11(id,testcase)) & " Hz."
                severity warning;
              freq_error_found <= '1';
              wait for 1 ns;
              freq_error_found <= '0';
            end if;
          when 3 =>        -- LAN port
            if ((frequency_dev > (real(max_v11_deviation)-0.7)) or (frequency_dev < (real(-1 * max_v11_deviation)-0.7))) then    -- Baudrate deviation has to be smaller than 700 ppm (less 0.7 ppm deviation from imprecise clock generation from TB).
              assert (false)
                report "Frequency of the LAN port is " & int2str(integer(frequency_mean)) & " Hz and should be " & int2str(baudrate_value_v11(id,testcase)) & " Hz."
                severity warning;
              freq_error_found <= '1';
              wait for 1 ns;
              freq_error_found <= '0';
            end if;
          end case;
          jitter_promill := real(baudrate_value_v11(id,testcase)) * (jitter_max + (-1.0 * jitter_min)) * 1.0e-6;
          case id is
          when 0|1 =>      -- V.11 port 1 or 2
            if (jitter_promill > 10.0) then    -- Jitter has to be smaller than 1% of Baudrate
              assert (false)
              report "Maximum jitter of V.11 port" & id2chr(id) & "should be smaller than 1% of baudrate but is +" & int2str(integer(jitter_max)) & " / -" & int2str(integer(-1.0 * jitter_min)) & " ns which equals to " & int2str(integer(jitter_promill)) & " per mil."
                severity warning;
              freq_error_found <= '1';
              wait for 1 ns;
              freq_error_found <= '0';
            end if;
          when 2 =>        -- G.703 port
            if (jitter_promill > 50.0) then    -- Jitter has to be smaller than 5% of Baudrate
              assert (false)
              report "Maximum jitter of G.703 port should be smaller than 5% of baudrate but is +" & int2str(integer(jitter_max)) & " / -" & int2str(integer(-1.0 * jitter_min)) & " ns which equals to " & int2str(integer(jitter_promill)) & " per mil."
                severity warning;
              freq_error_found <= '1';
              wait for 1 ns;
              freq_error_found <= '0';
            end if;
          when 3 =>        -- LAN port
            if (jitter_promill > 10.0) then    -- Jitter has to be smaller than 1% of Baudrate
              assert (false)
              report "Maximum jitter of LAN port should be smaller than 1% of baudrate but is +" & int2str(integer(jitter_max)) & " / -" & int2str(integer(-1.0 * jitter_min)) & " ns which equals to " & int2str(integer(jitter_promill)) & " per mil."
                severity warning;
              freq_error_found <= '1';
              wait for 1 ns;
              freq_error_found <= '0';
            end if;
          end case;
          supress    <= '0';
          jitter_max := 0.0;
          jitter_min := 0.0;
          wait on testcase;
        end if;
      end if;
    else
      wait on testcase;
    end if;
  end process;

  measure_delay: process
    variable rxd_container: std_logic_vector(9 downto 0) := (others => '0');
    variable delay        : integer   := 0;
  begin
    if ((check_delay(testcase) = '1') and (port_config_registers(7+id,testcase)(0) = '1')) then
      wait until (v11_txc=(clock_config_registers((7+id),testcase)(5) xnor clock_config_registers((7+id),testcase)(3))); --(v11_txc=not(clock_config_registers((7+id),testcase)(3)));
      rxd_container := rxd_container(8 downto 0) & v11_rxd;
      if (rxd_container((pattern_v11(id,testcase)'length)-1 downto 0) = pattern_v11(id,testcase)) then
        wait until (v11_txc=(clock_config_registers((7+id),testcase)(5) xnor clock_config_registers((7+id),testcase)(3)));
        delay := (now - t_end) / 1 us;
        if (id = 2) then        -- G.703 port TB delay adds extra 70 us, which have to be subtracted from the measured result.
          delay := delay - 70;
        end if;
        delay_error_found <= check_v11_delay_func(delay,id,testcase);
        wait for 1 ns;
        delay_error_found <= '0';
        wait on testcase;
      end if;
    else
      wait on testcase;
    end if;
  end process;

  mark_error: process
  begin
    failure  <= '0';
    wait until ((data_error_found = '1') or (freq_error_found = '1') or (delay_error_found = '1'));
    failure  <= '1';
    wait on testcase;
  end process;

END ARCHITECTURE behavior;

