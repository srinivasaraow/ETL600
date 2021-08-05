--------------------------------------------------------------------------
-- $Workfile: tb_v11_if_p_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_v11_if_p_body.vhd $
-- 
-- 16    11-07-06 9:30 Chstrue
-- R4 implementation
-- 
-- 15    8.02.06 12:43 Chalgyg
-- Bug removed.
-- 
-- 14    19.01.06 16:20 Chalgyg
-- G.703 interface integrated.
-- 
-- 13    12.04.05 11:59 Chalgyg
-- TX- and RX preload buffer enlarged to 7 bits each.
-- 
-- 12    25.02.05 13:19 Chalgyg
-- Position finding of first 0 in pattern was outsourced to function
-- give_first_0_pos().
-- 
-- 11    24.02.05 15:17 Chalgyg
-- Data comparisson is now made for every single bit.
-- 
-- 10    15.02.05 15:35 Chalgyg
-- - Delay checker is now exact.
-- - Active edge now depends on CCR.IAC.
-- - Max. delay for data checker increased to 20 ms.
-- 
-- 9     7.02.05 11:50 Chalgyg
-- Delay measurement added.
-- 
-- 8     25.01.05 14:19 Chalgyg
-- Made warning text more understandable.
-- 
-- 7     24.01.05 14:22 Chalgyg
-- Warning message added.
-- 
-- 6     21.01.05 14:28 Chalgyg
-- Active clk edge now depending on CCR.ICP.
-- 
-- 4     21.10.04 13:07 Gygax02
-- Testvector removed.
-- 
-- 2     14.10.04 16:23 Gygax02
-- -- no description --
-- 
-- 1     8.10.04 14:34 Gygax02
-- -- no description --
---------------------------------------------------------------------------
PACKAGE BODY tb_v11_if_p IS

  function check_v11_delay_func(
    delay    : integer;
    id       : integer range 0 to 3;
    testcase : t_testcase
  ) return std_logic is
    variable  max_delay : integer := 0;
  begin
    if (mode_v11(testcase) = 'o') then
      max_delay  := 500;
    else
      --14->15 110705
      max_delay  := integer(real(baudrate_value_ofdm(testcase) - baudrate_value_v11(id,testcase)) * 10000.0 / real(baudrate_value_ofdm(testcase)) + 14.0e6 / real(baudrate_value_v11(id,testcase)));
    end if;
    if (delay > max_delay) then
      case id is
      when 0|1 =>
        assert (false)
        report "The delay of V.11 port" & id2chr(id) & "is " & int2str(integer(delay)) & " us and should be smaller than " & int2str(integer(max_delay)) & " us."
          severity warning;
      when 2 =>
        assert (false)
        report "The delay of the G.703 port is " & int2str(integer(delay)) & " us and should be smaller than " & int2str(integer(max_delay)) & " us."
          severity warning;
      when 3 =>
        assert (false)
        report "The delay of the LAN port is " & int2str(integer(delay)) & " us and should be smaller than " & int2str(integer(max_delay)) & " us."
          severity warning;
      end case;
      return '1';
    else
      return '0';
    end if;
  end check_v11_delay_func;

  function calc_clk_period(
    id       : integer;
    testcase : t_testcase
  ) return time is
    variable ti        : time := 10 us;
    variable baudrate  : real := 19200.0;
    variable deviation : real := 0.0;
  begin
    baudrate  := real(baudrate_value_v11(id,testcase));
    deviation := real(baudrate_deviation_v11(id,testcase))* 1.0e-6;
    ti := 1 sec / (baudrate + (baudrate*deviation)) / 8;
    return ti;
  end calc_clk_period;

  function determine_octet_match(
    oct_rx  : std_logic_vector;
    oct_tx  : std_logic_vector
  ) return std_logic is
    variable failure    : std_logic := '0';
    variable copy_of_rx : std_logic_vector (((oct_rx'length)-1) downto 0) := (others=>'0');
  begin
    if (oct_rx = oct_tx) then
      failure := '0';                    -- Octet timing is matched.
    else
      copy_of_rx := oct_rx;
      for i in 1 to copy_of_rx'length loop
        copy_of_rx := copy_of_rx(((copy_of_rx'length)-2) downto 0) & copy_of_rx((copy_of_rx'length)-1);
        if (copy_of_rx = oct_tx) then    -- The bits are correct but the octet timing is displaced.
          assert (false)
          report "The octet timing is not transparent routed through MUX600!"
            severity warning;
          failure := '1';
        end if;
      end loop;
    end if;
    return failure;
  end determine_octet_match;

  procedure compare_v11 (
    constant  id           : in integer range 0 to 3;
    constant  testcase     : in t_testcase;
    signal    v11_rxd_int  : in std_logic;
    signal    clk          : in std_logic;
    signal    failure      : out std_logic
  ) is
    variable config_vec     : std_logic_vector(((pattern_v11(id,testcase)'length * nr_of_repetitions_v11(id,testcase))-1) downto 0) := (others=>'0');
    variable rec_length     : integer;
    variable pos_counter    : integer;
    variable nr_of_warnings : integer   := 0;
    variable ber            : real;
    variable first_0_pos    : integer   := 0;
    variable bit_sat        : std_logic := '0';
  begin
    rec_length    := config_vec'length - 1;
    first_0_pos   := give_first_0_pos(pattern_v11(id,testcase));
    for j in 1 to nr_of_repetitions_v11(id,testcase) loop
      config_vec((((pattern_v11(id,testcase)'length)*j)-1) downto ((pattern_v11(id,testcase)'length)*(j-1))) := pattern_v11(id,testcase);
    end loop;
    wait until (v11_rxd_int='0') for 50 ms;
    if (testcase = 17) or (testcase = 20) then
      wait until (v11_rxd_int='1') for 1 ms;
      wait until (v11_rxd_int='0') for 1 ms;
    end if;
    
    for pos_counter in first_0_pos to rec_length loop
      wait until (clk=(clock_config_registers((7+id),testcase)(5) xor clock_config_registers((7+id),testcase)(3))) for 105 us;          -- Min. datarate is 9.6 kBps.
      if (config_vec(rec_length - pos_counter) /= v11_rxd_int) then
        if (nr_of_warnings = 0) then
          case id is
          when 0|1 =>
            assert (false)
              report "RXD error detected on V.11 port" & id2chr(id)
              severity warning;
          when 2 =>
            assert (false)
            report "TX error detected on the G.703 port"
              severity warning;
          when 3 =>
            assert (false)
            report "RXD error detected on the LAN port"
              severity warning;
          end case;
        end if;
        nr_of_warnings := nr_of_warnings + 1;
        failure        <= '1';
      end if;
    end loop;
    if (nr_of_warnings>0) then
      ber := real(nr_of_warnings) / real(rec_length + 1) * 1.0e6;
      case id is
      when 0|1 =>
        assert (false)
        report int2str(nr_of_warnings) & " bit errors have been detected on V.11 port" & id2chr(id) & ", which is equivalent to a BER of " & int2str(integer(ber)) & "e-6"
          severity warning;
      when 2 =>
        assert (false)
        report int2str(nr_of_warnings) & " bit errors have been detected on the G.703 port, which is equivalent to a BER of " & int2str(integer(ber)) & "e-6"
          severity warning;
      when 3 =>
        assert (false)
        report int2str(nr_of_warnings) & " bit errors have been detected on the LAN port, which is equivalent to a BER of " & int2str(integer(ber)) & "e-6"
          severity warning;
      end case;
    end if;
  end compare_v11;

END tb_v11_if_p;
