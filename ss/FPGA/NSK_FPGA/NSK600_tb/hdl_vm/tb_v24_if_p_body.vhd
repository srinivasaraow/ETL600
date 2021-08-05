--------------------------------------------------------------------------
-- $Workfile: tb_v24_if_p_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_v24_if_p_body.vhd $
-- 
-- 17    11-07-06 9:31 Chstrue
-- R4 implementation
-- 
-- 16    25.04.05 13:07 Chalgyg
-- Delay measurement bug removed in sharing device mode over mux.
-- 
-- 15    13.04.05 10:37 Chalgyg
-- Delay measurement debugged.
-- 
-- 14    12.04.05 12:00 Chalgyg
-- TX- and RX preload buffer enlarged to 7 bits each.
-- 
-- 13    12.04.05 11:24 Chalgyg
-- Bit error detection enhanced for FT1.2 protocol transmission.
-- 
-- 12    7.04.05 16:31 Chalgyg
-- FT1.2 protocol simulation included. (analysis of returning data is not
-- implemented yet)
-- 
-- 11    24.02.05 15:19 Chalgyg
-- Data comparisson is now made for every single bit.
-- 
-- 10    16.02.05 13:04 Chalgyg
-- - Delay measurement improved.
-- - Abort counter enlarged for 20 ms MUX delay.
-- 
-- 9     2.02.05 15:14 Chalgyg
-- abort counter more sophisticated.
-- 
-- 8     17.12.04 13:13 Chalgyg
-- Delay measurement added.
-- Data verification changed.
-- 
-- 6     23.11.04 10:46 Gygax02
-- compare_rxd deleted. Not applicable without a clock.
-- 
-- 4     19.11.04 14:31 Gygax02
-- Compare algorithm changed.
-- 
-- 2     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
PACKAGE BODY tb_v24_if_p IS

  function id2chr(int: integer) return string is
    variable str: string(1 to 3);
  begin
    case int is
      when  0 => str := " 1 ";
      when  1 => str := " 2 ";
      when  2 => str := " 3 ";
      when  3 => str := " 4 ";
      when  4 => str := " 5 ";
      when  5 => str := " 6 ";
      when others => str := " ? ";
    end case;
    return str;
  end id2chr;

  function check_delay_func(
    delay    : integer;
    id       : integer;
    testcase : t_testcase
  ) return std_logic is
    variable  max_delay : integer := 0;
  begin
    if (mode(id,testcase) = 'a') then
      max_delay  := 250;
    elsif ((mode(id,testcase) = 'f') or ((mode(id,testcase) = 's') and (switchbox_modem(testcase) /= 'm'))) then
      max_delay  := 500;
    else
      --14->15 110705
      max_delay  := integer(real(baudrate_value_ofdm(testcase) - baudrate_value(id,testcase)) * 10000.0 / real(baudrate_value_ofdm(testcase)) + 14.0e6 / real(baudrate_value(id,testcase)));
    end if;
    if (delay > max_delay) then
      assert (false)
      report "The delay of V.24 port" & id2chr(id) & "is " & int2str(integer(delay)) & " us and should be smaller than " & int2str(integer(max_delay)) & " us."
        severity warning;
      return '1';
    else
      return '0';
    end if;
  end check_delay_func;

  procedure compare_v24 (
    constant  id           : in integer;
    constant  testcase     : in t_testcase;
    constant  pattern      : in std_logic_vector;
    constant  repetitions  : in integer;
    signal    rxd          : in std_logic;
    signal    clk          : in std_logic;
    signal    failure      : out std_logic;
    signal    bit_det      : out std_logic;    -- Just for debugging!
    signal    char_det     : out std_logic;    -- Just for debugging!
    signal    rep_det      : out std_logic;    -- Just for debugging!
    signal    nr_warn      : out integer       -- Just for debugging!
  ) is

    type data_vector is array (0 to (((ft1_pattern(id)'length) / nr_data_bits(id)) - 1)) of std_logic_vector((nr_data_bits(id)+parity2nr(parity_bit(id))+1) downto 0);

    variable config_vec     : std_logic_vector((((pattern'length + 2) * repetitions)-1) downto 0) := (others=>'0');
    variable rec_length     : integer := config_vec'length - 1;
    variable nr_of_warnings : integer := 0;
    variable ber            : real    := 0.0;
    variable data_to_receive: data_vector;
    variable bitposition    : integer := (ft1_pattern(0)'length-1);
    variable nr_of_chars    : integer := (((ft1_pattern(0)'length) / nr_data_bits(id)) - 1);
    variable bit_counter    : integer := (nr_data_bits(id) + parity2nr(parity_bit(id)) + 1);
  begin
    if (pattern_protocol(testcase) = 's') then      -- Pattern is streamed with 1 start, 8 data and 1 stopbit.
      for j in 1 to repetitions loop
        config_vec((((pattern'length + 2)*j)-1) downto ((pattern'length + 2)*(j-1))) := ('0' & pattern & '1');
      end loop;
      wait until (rxd='0') for 50 ms;
      for pos_counter in 0 to rec_length loop
        wait until (clk='0') for 10.001 ms;          -- Min. datarate is 100 Bps.
        if (config_vec(rec_length - pos_counter) /= rxd) then
          if (nr_of_warnings = 0) then
            assert (false)
              report "RXD error detected on V.24 port" & id2chr(id)
              severity warning;
          end if;
          nr_of_warnings := nr_of_warnings + 1;
          failure        <= '1';
        end if;
      end loop;
    elsif (pattern_protocol(testcase) = 'f') then   -- Pattern is streamed in FT1.2 protocol.
      for charcounter in 0 to nr_of_chars loop
        data_to_receive(charcounter)(bit_counter) := '0';      -- Add startbit.
        bit_counter := bit_counter - 1;
        data_to_receive(charcounter)((bit_counter) downto (bit_counter-(nr_data_bits(id)-1))) := ft1_pattern(id)(bitposition downto (bitposition-(nr_data_bits(id)-1)));  -- Add data bits.
        bit_counter := bit_counter - nr_data_bits(id);
        if (parity_bit(id) /= 'n') then                        -- Add parity bit if wanted.
          data_to_receive(charcounter)(bit_counter) := '1';    
          bit_counter := bit_counter - 1;
        end if;
        data_to_receive(charcounter)(bit_counter) := '1';      -- Add only one stopbit.
        bit_counter := nr_data_bits(id) + parity2nr(parity_bit(id)) + 1;
        bitposition := bitposition - nr_data_bits(id);
      end loop;
      for repeat in 0 to (nr_repetitions(id)-1) loop
        rep_det  <= '1';
        wait for 1 ps;
        rep_det  <= '0';
        for charcounter in 0 to nr_of_chars loop
          wait until (rxd='0') for 50 ms;
          char_det  <= '1';
          wait for 1 ps;
          char_det  <= '0';
          for bit_counter in (nr_data_bits(id) + parity2nr(parity_bit(id)) + 1) downto 0 loop
            wait until (clk='0') for 10.001 ms;          -- Min. datarate is 100 Bps.
            bit_det  <= '1';
            wait for 1 ps;
            bit_det  <= '0';
            if (data_to_receive(charcounter)(bit_counter) /= rxd) then
              if (nr_of_warnings = 0) then
                assert (false)
                  report "RXD error detected on V.24 port" & id2chr(id)
                  severity warning;
              end if;
              nr_of_warnings := nr_of_warnings + 1;
              nr_warn        <= nr_of_warnings;
              failure        <= '1';
            end if;
          end loop;
        end loop;
      end loop;
    end if;
    if (nr_of_warnings>0) then
      ber := real(nr_of_warnings) / real(rec_length + 1) * 1.0e6;
      assert (false)
        report int2str(nr_of_warnings) & " bit errors have been detected on V.24 port" & id2chr(id) & ", which is equivalent to a BER of " & int2str(integer(ber)) & "e-6"
        severity warning;
    end if;
  end compare_v24;

  procedure give_new_bit(
    constant id           : in integer; 
    constant testcase     : in t_testcase;
    signal   tx_clock     : in std_logic;
    signal   txd_local    : out std_logic;
    signal   txd_int      : out std_logic;
    signal   t_end_of_txd : out time;
    signal   t_set        : out std_logic
  ) is

    type data_vector is array (0 to (((ft1_pattern(id)'length) / nr_data_bits(id)) - 1)) of std_logic_vector ((nr_data_bits(id)+nr_stop_bits(id)+parity2nr(parity_bit(id))) downto 0);
    type cycle_vec   is array (0 to (nr_repetitions(id)-1)) of integer;
    type time_vec    is array (0 to (nr_repetitions(id)-1)) of time;
    type real_vec    is array (0 to (nr_repetitions(id)-1)) of real;

    procedure calculate_delay(
      constant id           : in integer;
      constant testcase     : in t_testcase;
      variable nr_cycles    : out cycle_vec;
      variable delay        : out time_vec
    ) is
      variable intermediate  : real_vec;
      variable intermediate2 : real_vec;
      variable shift         : real_vec;
    begin
      for i in 0 to (nr_cycles'length-1) loop
        if (i=0) then
          shift(i)      := 0.0;
        else
          shift(i)      := (real(integer(intermediate(i-1) * 10.0) rem 10) / 10.0);
        end if;
        intermediate(i) := pause_after_char(id,i) + shift(i);
        intermediate2(i):= real(integer(intermediate(i) * 10.0) rem 10) / 10.0;
        nr_cycles(i)    := integer(intermediate(i) - intermediate2(i));
        delay(i)        := 1 us * (1.0 / real(baudrate_value(id,testcase))) * intermediate2(i) * 1.0e6;
      end loop;
    end calculate_delay;

    variable data_to_send   : data_vector;
    variable bitposition    : integer         := (ft1_pattern(0)'length-1);
    variable poscounter     : integer         := (nr_data_bits(id) + nr_stop_bits(id) + parity2nr(parity_bit(id)));
    variable nr_of_chars    : integer         := (((ft1_pattern(0)'length) / nr_data_bits(id)) - 1);
    variable charcounter    : integer         := 0;
    variable one_bit        : std_logic       := '1';
    variable phase_shift    : time            := 0 ps;
    variable repeat         : integer         := 0;
    variable nr_cycles      : cycle_vec;
    variable delay          : time_vec;
    variable end_reached    : std_logic       := '0';
    variable delay_measured : std_logic       := '0';

  begin
    for charcounter in 0 to nr_of_chars loop
      data_to_send(charcounter)(poscounter) := '0';      -- Add startbit.
      poscounter := poscounter - 1;
      data_to_send(charcounter)((poscounter) downto (poscounter-(nr_data_bits(id)-1))) := ft1_pattern(id)(bitposition downto (bitposition-(nr_data_bits(id)-1)));  -- Add data bits.
      poscounter := poscounter - nr_data_bits(id);
      if (parity_bit(id) /= 'n') then                        -- Add parity bit if wanted.
        data_to_send(charcounter)(poscounter) := '1';    
        poscounter := poscounter - 1;
      end if;
      if (nr_stop_bits(id) > 1) then
        data_to_send(charcounter)(poscounter downto (poscounter-(nr_stop_bits(id)-1))) := (others=>'1');  -- Add stopbits.
      else
        data_to_send(charcounter)(poscounter) := '1';
      end if;
      poscounter  := nr_data_bits(id) + nr_stop_bits(id) + parity2nr(parity_bit(id));
      bitposition := bitposition - nr_data_bits(id);
    end loop;
    calculate_delay(id,testcase,nr_cycles,delay);
    for repeat in 0 to (nr_repetitions(id)-1) loop
      for charcounter in 0 to nr_of_chars loop
        for poscounter in (nr_data_bits(id) + nr_stop_bits(id) + parity2nr(parity_bit(id))) downto 0 loop
          wait until (tx_clock='0');
          wait for phase_shift;
          if (end_reached = '1' and delay_measured = '0') then
            t_end_of_txd   <= now;
            end_reached    := '0';
            delay_measured := '1';
            t_set        <= '1';
            wait for 1 ps;
            t_set        <= '0';
          end if;        
          one_bit      := data_to_send(charcounter)(poscounter);
          txd_local    <= one_bit;
          txd_int      <= one_bit;
        end loop;
        end_reached := '1';
      end loop;
      for i in 1 to nr_cycles(repeat) loop
        wait until (tx_clock='0');
      end loop;
      phase_shift  := delay(repeat);
    end loop;
    delay_measured := '0';
    end_reached    := '0';
  end give_new_bit;

END tb_v24_if_p;
