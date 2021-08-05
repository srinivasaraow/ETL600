--------------------------------------------------------------------------
-- $Workfile: tb_ofdm_fpga_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_ofdm_fpga_behavior.vhd $
-- 
-- 21    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 20    6.09.06 9:30 Chalgyg
-- - Maximum frequency deviation -0.7 ppm shifted, because of clock
-- inacurracy.
-- - Frequency measuring time in MUX mode doubled.
-- 
-- 19    7.04.05 16:29 Chalgyg
-- Check pattern only if no protocol is running.
-- 
-- 18    25.02.05 13:23 Chalgyg
-- Signal mark_low added for debugging purposes.
-- 
-- 17    24.02.05 15:15 Chalgyg
-- - MUX frame detection finished.
-- - Other initial value given for BER generator.
-- 
-- 16    15.02.05 11:57 Chalgyg
-- Frame detection prepared.
-- 
-- 15    2.02.05 15:11 Chalgyg
-- v11_indication = '1' all the time, like on the target.
-- 
-- 14    21.01.05 14:27 Chalgyg
-- sclk0/2 edges changed.
-- 
-- 13    19.01.05 11:16 Chalgyg
-- Frequency measurement enhanced.
-- 
-- 12    6.01.05 15:51 Chalgyg
-- d2a has default value '0'.
-- 
-- 10    13.12.04 13:52 Chalgyg
-- Frequency detection improved.
-- 
-- 9     8.12.04 11:13 Chalgyg
-- Signal 'configured' added.
-- 
-- 8     22.10.04 16:24 Gygax02
-- -- no description --
-- 
-- 7     21.10.04 13:05 Gygax02
-- Frequency detection and clock verification included.
-- 
-- 5     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 4     19.08.04 11:05 Gygax02
-- -- no description --
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
LIBRARY std;
USE std.textio.all;
LIBRARY NSK600_tb;
USE NSK600_tb.report_assist.all;

ENTITY tb_ofdm_fpga IS
   PORT( 
      configured       : IN     std_logic;
      d0a              : IN     std_logic;
      sclk0            : IN     std_logic;
      sclk2            : IN     std_logic;
      testcase         : IN     t_testcase;
      v11_control      : IN     std_logic;
      d2a              : OUT    std_logic;
      ofdm_fpga_status : OUT    std_logic_vector (1 DOWNTO 0);
      v11_indication   : OUT    std_logic
   );

-- Declarations

END tb_ofdm_fpga ;

--
ARCHITECTURE behavior OF tb_ofdm_fpga IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  signal real_count       : real;
  signal data_fail        : std_logic;
  signal sclk_fail        : std_logic;
  signal freq_fail        : std_logic;
  signal error_insert     : std_logic;
  signal data_busy        : std_logic := '0';
  signal clk_busy         : std_logic := '0';
  signal mark_low         : std_logic := '0';
    
BEGIN
  
  v11_indication      <= '1';
  ofdm_fpga_status(1) <= not(data_busy or clk_busy);
  
  check_sclks: process
  begin
    if (port_config_registers(0,testcase)(0) = '1') then
      if (sclk0 /= sclk2) then
        assert (false)
          report "The SPORT0 clock is not equal to the SPORT2 clock!"
          severity failure;
        sclk_fail <= '1';
        wait for 1 ns;
        sclk_fail <= '0';
      end if;
      wait for 100 ns;
    else
      sclk_fail <= '0';
      wait on testcase;
    end if;
  end process;

  check_sclk_period: process
    variable ti1            : time      := 10 us;
    variable ti2            : time      := 10 us;
    variable frequency      : real      := 0.0;
    variable frequency_sum  : real      := 0.0;
    variable frequency_mean : real      := 0.0;
    variable frequency_dev  : real      := 0.0;
    variable supress        : std_logic := '0';
    variable rxc_period     : time      := 0 ps;
    variable index          : integer   := 0;
    variable int_repetition : integer   := 1;
    variable int_length     : integer   := 1;    
    variable jitter         : real      := 0.0;
    variable jitter_max     : real      := 0.0;
    variable jitter_min     : real      := 0.0;
    variable jitter_promill : real      := 0.0;
  begin
    if (port_config_registers(0,testcase)(0) = '1') then
      clk_busy <= '1';
      if (supress = '0') then
        for i in 0 to 3 loop    -- let the clock stabilise for 4 cycles.
          wait until (sclk0 = '1');
        end loop;
        if (dps_if_modus(testcase) = loop_v11) then
          int_repetition := nr_of_repetitions_v11(0,testcase);
          int_length     := pattern_v11(0,testcase)'length;
        else
          int_repetition := 4;
          int_length     := 8;
        end if;
        supress       := '1';
        index         := 0;
        frequency_sum := 0.0;
      end if;
      ti1            := now;
      wait until (sclk0 = '1');
      ti2            := now;
      rxc_period     := ti2 - ti1;
      if (rxc_period > 0 ps) then
        frequency      := 1.0e12 / real(rxc_period / 1 ps);
        frequency_sum  := frequency_sum + frequency;
        index          := index + 1;
        jitter         := 1.0e9 * ((1.0 / real(baudrate_value_ofdm(testcase))) - (1.0 /frequency));
        if (jitter_max < jitter) then
          jitter_max   := jitter;
        end if;
        if (jitter_min > jitter) then
          jitter_min   := jitter;
        end if;
      end if;
      if (index = (int_repetition*int_length)) then
        frequency_mean := frequency_sum / real(index);
        frequency_dev  := 1.0 - (frequency_mean / real(baudrate_value_ofdm(testcase)));
        if ((frequency_dev > 0.0003993) or (frequency_dev < -0.0004007)) then    -- Baudrate deviation has to be smaller than 400 ppm (less 0.7 ppm deviation from imprecise clock generation from TB).
          assert (false)
            report "Frequency of aggregate port is " & int2str(integer(frequency_mean)) & " Hz and should be " & int2str(baudrate_value_ofdm(testcase)) & " Hz."
            severity warning;
          freq_fail <= '1';
          wait for 1 ns;
          freq_fail <= '0';
        end if;
        jitter_promill := real(baudrate_value_ofdm(testcase)) * (jitter_max + (-1.0 * jitter_min)) * 1.0e-6;
        if (jitter_promill > 10.0) then    -- Jitter has to be smaller than 1% of Baudrate
          assert (false)
          report "Maximum jitter of aggregate port should be smaller than 1% of baudrate but is +" & int2str(integer(jitter_max)) & " / -" & int2str(integer(-1.0 * jitter_min)) & " ns which equals to " & int2str(integer(jitter_promill)) & " per mil."
            severity warning;
          freq_fail <= '1';
          wait for 1 ns;
          freq_fail <= '0';
        end if;
        supress    := '0';
        jitter_max := 0.0;
        jitter_min := 0.0;
        clk_busy   <= '0';
        wait on testcase;
      end if;
    else
      clk_busy  <= '0';
      freq_fail <= '0';
      wait on testcase;
    end if;
  end process;

  extract_mux_frame: process
  begin
    if ((check_mux_frame(testcase) = '1') and (pattern_protocol(testcase) = 's')) then
      data_busy  <= '1';
      wait for mux_sync_time;     --dsg
--      wait for 39.5 ms;
      analyse_mux_frame(testcase,sclk0,v11_control,d0a,mark_low,data_fail);
      wait for 1 ns;
      data_fail  <= '0';
      data_busy  <= '0';
      wait on testcase;
    else
      data_busy  <= '0';
      wait on testcase;
    end if;
  end process;

  generate_random_pattern: process
    variable counter  : std_logic_vector(34 downto 0) := "01001001010101110101011010010110101";
  begin
		counter	    := counter(33 downto 0) & (counter(34) xnor counter (32));
		real_count  <= slv2real(counter);
		wait for 1 us;
  end process;
  
  insert_error: process
    variable temp_val  : std_logic := '0';
  begin
    if (port_config_registers(0,testcase)(0) = '1') then
      wait until (sclk0 = '0');
      if (real_count > (ofdm_port_ber(testcase) * 34.35973837e9)) then
        temp_val     := d0a;
      else
        temp_val     := not(d0a);
        error_insert <= '1';
      end if;
      wait until (sclk0 = '1');
      d2a          <= temp_val;
      error_insert <= '0';
    else
      d2a  <= '0';
      wait on testcase;
    end if;
  end process;

  mark_error: process
  begin
    ofdm_fpga_status(0) <= '0';
    wait until ((data_fail = '1') or (sclk_fail = '1') or (freq_fail = '1'));
    ofdm_fpga_status(0) <= '1';
    wait on testcase;
  end process;
  
END ARCHITECTURE behavior;

