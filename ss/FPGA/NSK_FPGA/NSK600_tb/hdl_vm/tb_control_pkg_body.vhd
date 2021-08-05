--------------------------------------------------------------------------
-- $Workfile: tb_control_pkg_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_control_pkg_body.vhd $
-- 
-- 25    8.02.06 12:43 Chalgyg
-- Calculation time bug in ADPSK mode removed.
-- 
-- 24    26.01.06 15:53 Chalgyg
-- G.703 LOS signal path testing added.
-- 
-- 23    19.01.06 16:16 Chalgyg
-- G.703 interface integrated.
-- 
-- 22    10.01.06 9:06 Chalgyg
-- LAN interface is now supported.
-- 
-- 21    18.04.05 17:18 Chalgyg
-- - Line feed bug removed.
-- - Protocol bug removed.
-- 
-- 20    13.04.05 10:36 Chalgyg
-- Text for protocol transmission added.
-- 
-- 19    12.04.05 11:14 Chalgyg
-- Took FT1.2 pattern protocol transmission into simulation time
-- calculation account.
-- 
-- 18    30.03.05 13:31 Chalgyg
-- In loop_rts case, no time is calculated anymore in order to guarantee
-- no division through 0 occures.
-- 
-- 17    1.03.05 10:05 Chalgyg
-- Simulation time estimation introduced.
-- 
-- 16    24.02.05 15:11 Chalgyg
-- - comment for case loop_mixed added.
-- - check_mux_frame added.
-- 
-- 15    16.02.05 10:57 Chalgyg
-- -- no description --
-- 
-- 14    15.02.05 15:30 Chalgyg
-- Cosmetics
-- 
-- 13    15.02.05 11:55 Chalgyg
-- case loop_p_share enhanced.
-- 
-- 12    10.02.05 15:51 Chalgyg
-- loop_rts message changed.
-- 
-- 11    7.02.05 11:49 Chalgyg
-- Delay measurement added to v11_loop.
-- 
-- 10    18.01.05 14:54 Chalgyg
-- Message for V.11 loop added.
-- 
-- 9     17.12.04 13:19 Chalgyg
-- Testcase real_out added.
-- 
-- 7     13.12.04 13:49 Chalgyg
-- Message for loop_v24 added.
-- 
-- 6     8.12.04 13:33 Chalgyg
-- -- no description --
-- 
-- 5     3.12.04 12:56 Chalgyg
-- -- no description --
-- 
-- 4     1.12.04 14:56 Chalgyg
-- Output comments cleaned.
-- 
-- 2     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
PACKAGE BODY tb_control_p IS
  
  function int2chr(int: integer)
  return character is
    variable c: character;
  begin
    case int is
      when  0 => c := '0';
      when  1 => c := '1';
      when  2 => c := '2';
      when  3 => c := '3';
      when  4 => c := '4';
      when  5 => c := '5';
      when  6 => c := '6';
      when  7 => c := '7';
      when  8 => c := '8';
      when  9 => c := '9';
      when others => c := '?';
    end case;
    return c;
   end int2chr;

  function int2str(int: integer)
  return string is
    variable temp:      string(1 to 10);
    variable num:       integer;
    variable len:       integer := 1;
    variable power:     integer := 1;
  begin
    num     := int;
    while num >= 10 loop                         -- Determine how many
      len := len + 1;                            -- characters required
      num := num / 10;                           -- to represent the
    end loop ;                                   -- number.
    for i in len downto 1 loop                   -- Convert the number to
      temp(i) := int2chr(int/power mod 10);      -- a string starting
      power   := power * 10;                     -- with the right hand
    end loop ;                                   -- side.
    return temp(1 to len);
  end int2str;

  procedure estimate_time(
    variable longest_time : out time_vector
  ) is
    variable time            : real        := 0.0;
    variable longest_time_int: time_vector := (others=>0.0);
    variable pause_bits      : real        := 0.0;
  begin
    for j in 0 to t_testcase'high loop
      if (do_testcase(j) = '1') then
        for i in 0 to 5 loop    -- Calculate V.24 time
          if ((port_config_registers((i+1),j)(0) = '1') and (check_signalling(j) = '0')) then
            if (pattern_protocol(j) = 's') then      -- Pattern is streamed with 1 start, 8 data and 1 stopbit.
              if (nr_of_repetitions_v24(i,j) /= 0) then
                time := 1.0e3/real(baudrate_value(i,j))* real(nr_of_repetitions_v24(i,j)*(pattern_v24(i,j)'length+2)+1) + 281.25e-3 + 4.556e-3; -- Pattern transmit duration + transmission delay + configuration delay
              elsif ((nr_of_repetitions_dsp(i,j) /= 0) and (i <= 3)) then
                time := 1.0e3/real(baudrate_value(i,j))* real(nr_of_repetitions_dsp(i,j)*(pattern_dsp_if(i,j)'length+2)+1) + 281.25e-3 + 4.556e-3; -- Pattern transmit duration + transmission delay + configuration delay
              end if;
            elsif (pattern_protocol(j) = 'f') then   -- Pattern is streamed in FT1.2 protocol.
              pause_bits := 0.0;
              for k in 0 to (nr_repetitions(i)-2) loop
                pause_bits := pause_bits+pause_after_char(i,k);
              end loop;
              time := 1.0e3/real(baudrate_value(i,j))*(pause_bits+real(nr_repetitions(i)*(ft1_pattern(0)'length/nr_data_bits(i))*(1+nr_data_bits(i)+parity2nr(parity_bit(i))+nr_stop_bits(i))+1)) + 281.25e-3 + 4.556e-3; -- Pattern transmit duration + transmission delay + configuration delay
            end if;
            if ((mode(i,j) = 'm') or ((mode(i,j) = 's') and (switchbox_modem(j) = 'm'))) then
              time := time + real(mux_sync_time / 1 ms);  -- Add start delay for MUX synchronisation procedure.
              time := time - 281.25e-3 + (real(baudrate_value_ofdm(j) - baudrate_value(i,j)) * 10.0 / real(baudrate_value_ofdm(j)) + 6.0e3 / real(baudrate_value(i,j)));   -- Add delay over MUX600.
            end if;
            if (time > longest_time_int(j)) then
              longest_time_int(j) := time;
            end if;
          end if;
        end loop;
        for i in 0 to 3 loop    -- Calculate V.11, G.703 and LAN time
          if ((port_config_registers((i+7),j)(0) = '1') and (check_signalling(j) = '0')) then
            time := 1.0e3/real(baudrate_value_v11(i,j))* real(nr_of_repetitions_v11(i,j)*pattern_v11(i,j)'length+8);
            if (mode_v11(j) = 'm') then
              time := time + real(mux_sync_time / 1 ms);  -- Add start delay for MUX synchronisation procedure.
              time := time - (2.0e3/real(baudrate_value_v11(i,j))) + (real(baudrate_value_ofdm(j) - baudrate_value_v11(i,j)) * 10.0 / real(baudrate_value_ofdm(j)) + 6.0e3 / real(baudrate_value_v11(i,j)));  -- Add delay over MUX600.
            end if;
            if (time > longest_time_int(j)) then
              longest_time_int(j) := time;
            end if;
          end if;
        end loop;
        longest_time_int(longest_time_int'high) := longest_time_int(longest_time_int'high)+ longest_time_int(j);
      end if;
    end loop;
    longest_time := longest_time_int;
  end estimate_time;

  procedure printf(s : in string) is
    variable l : line;
  begin
    write(l, s);
    writeline(output, l);
  end printf;

  procedure generate_message (
    constant  index  : in integer
  ) is
  begin
    case dps_if_modus(index) is
      when reset       => printf(" " & LF & "- The DUT was reseted.");
      when dsp_if_com  => printf(" " & LF & "- DSP interface communication is ok.");
      when loop_fsk    => printf("NSK600 modem is in FSK mode" & LF & " ");
                          if (check_regenerated_txd(index) = '1') then
                            printf("- Data transfer from DUT to DSP via registers is ok.");
                          end if;
                          if (check_regenerated_rxd(index) = '1') then
                            printf("- RXDs on all V.24 interfaces are ok.");
                          end if;
                          if (check_rxc(index) = '1') then
                            printf("- V.24 output clock RXC is ok.");
                            printf("- V.24 output jitter is ok.");
                          end if;
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when out_adpsk   => printf("NSK600 modem is in ADPSK mode" & LF & " ");
                          printf("- RXDs on all V.24 interfaces are ok.");
                          if (check_rxc(index) = '1') then
                            printf("- V.24 output clock RXC is ok.");
                            printf("- V.24 output jitter is ok.");
                          end if;
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when real_out    => printf("Real data, collected from SHARC emulator was shifted out of RXD." & LF & " ");
                          printf("- No tests have been carried out! Watch the wave for further information.");
      when in_adpsk    => printf("NSK600 modem is in ADPSK mode" & LF & " ");
                          printf("- Data transfer from DUT to DSP via registers is ok.");
                          if (check_rxc(index) = '1') then
                            printf("- V.24 output clock RXC is ok.");
                            printf("- V.24 output jitter is ok.");
                          end if;
      when loop_rts    => printf("Device is in NSK600 mode" & LF & " ");
                          printf("- RTS and CTS are ok.");
      when loop_p_share=> if (switchbox_modem(index) = 'f') then
                            printf("Device is in port sharing mode over FSK modem");
                          elsif (switchbox_modem(index) = 'a') then
                            printf("Device is in port sharing mode over ADPSK modem");
                          elsif (switchbox_modem(index) = 'm') then
                            printf("Device is in port sharing mode over MUX600");
                          end if;
                          if (port_config_registers(7,index)(0) = '1') then
                            if (mode_v11(index) = 'm') then
                              printf("Additionally V.11 communication takes place over MUX600" & LF & " ");
                            else
                              printf("Additionally a V.11 channel is transmitted over MOD600" & LF & " ");
                            end if;
                            printf("- TXD to RXD loops on all V.11 ports are ok.");
                            if (check_rxc(index) = '1') then
                              printf("- V.11 output clock TXC is ok.");
                              printf("- V.11 output jitter is ok.");
                            end if;
                            printf("- Output clock on aggregate interface is ok.");
                          else
                            printf(" ");
                          end if;
                          printf("- TXD to RXD loops on all V.24 sharing devices are ok.");
                          if (check_rxc(index) = '1') then
                            printf("- V.24 output clock RXC is ok.");
                            printf("- V.24 output jitter is ok.");
                          end if;
                          printf(" ");
                          if (check_mux_frame(index) = '1') then
                            printf("- All channels in the MUX frame are ok.");
                          end if;
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when loop_v11    => printf("Device is in MOD600 mode" & LF & " ");
                          printf("- TXD to RXD loop on V.11 port 1 is ok.");
                          if (check_rxc(index) = '1') then
                            printf("- V.11 output clock TXC is ok.");
                            printf("- V.11 output jitter is ok.");
                          end if;
                          printf("- Output Clock on aggregate interface is ok.");
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when loop_mux_v11=> printf("Device is in MUX600 mode" & LF & " ");
                          printf("- TXD to RXD loops on all V.11 ports are ok.");
                          if (check_rxc(index) = '1') then
                            printf("- V.11 output clock TXC is ok.");
                            printf("- V.11 output jitter is ok.");
                          end if;
                          printf(" ");
                          if (check_mux_frame(index) = '1') then
                            printf("- All channels in the MUX frame are ok.");
                          end if;
                          printf("- Output clock on aggregate interface is ok.");
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when loop_mux_v24=> printf("Device is in MUX600 mode");
                          if (pattern_protocol(index) = 'f') then
                            printf("The transmitted data is embedded in a FT1.2 protocol");
                          end if;
                          printf(" ");
                          printf("- TXD to RXD loops on all V.24 ports are ok.");
                          if (check_rxc(index) = '1') then
                            printf("- V.24 output clock RXC is ok.");
                            printf("- V.24 output jitter is ok.");
                          end if;
                          printf(" ");
                          if ((check_mux_frame(index) = '1') and (pattern_protocol(index) /= 'f')) then
                            printf("- All channels in the MUX frame are ok.");
                          end if;
                          printf("- Output Clock on aggregate interface is ok.");
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when loop_mixed  => printf("Device is in MUX600 mode");
                          if (pattern_protocol(index) = 'f') then
                            printf("The transmitted data is embedded in a FT1.2 protocol");
                          end if;
                          printf(" ");
                          printf("- TXD to RXD loops on all V.24 ports are ok.");
                          printf("- TXD to RXD loops on all V.11 ports are ok.");
                          if (port_config_registers(9,index)(0) = '1') then
                            printf("- TX to RX loop on the G.703 port is ok.");
                            printf("- The octet timing is transparent and the polarity alright.");
                          end if;
                          if (port_config_registers(10,index)(0) = '1') then
                            printf("- TXD to RXD loop on the LAN port is ok.");
                          end if;
                          if (check_rxc(index) = '1') then
                            printf("- V.24 output clock RXC is ok.");
                            printf("- V.24 output jitter is ok.");
                            printf("- V.11 output clock TXC is ok.");
                            printf("- V.11 output jitter is ok.");
                            if (port_config_registers(9,index)(0) = '1') then
                              printf("- G.703 output clock is ok.");
                              printf("- G.703 output jitter is ok.");
                            end if;
                            if (port_config_registers(10,index)(0) = '1') then
                              printf("- LAN output clock TXC is ok.");
                              printf("- LAN output jitter is ok.");
                            end if;
                          end if;
                          printf(" ");
                          if ((check_mux_frame(index) = '1') and (pattern_protocol(index) /= 'f')) then
                            printf("- All channels in the MUX frame are ok.");
                          end if;
                          printf("- Output clock on aggregate interface is ok.");
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when loop_lan    => printf("Device is in MUX600 mode" & LF & " ");
                          printf("- TXD to RXD loop on the LAN port is ok.");
                          if (check_rxc(index) = '1') then
                            printf("- LAN output clock TXC is ok.");
                            printf("- LAN output jitter is ok.");
                          end if;
                          printf(" ");
                          if (check_mux_frame(index) = '1') then
                            printf("- All channels in the MUX frame are ok.");
                          end if;
                          printf("- Output clock on aggregate interface is ok.");
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when loop_g703   => printf("Device is in MUX600 mode" & LF & " ");
                          printf("- TX to RX loop on the G.703 port is ok.");
                          printf("- The octet timing is transparent and the polarity alright.");
                          printf("- LOS signaling works fine.");
                          if (check_rxc(index) = '1') then
                            printf("- G.703 output clock is ok.");
                            printf("- G.703 output jitter is ok.");
                          end if;
                          printf(" ");
                          if (check_mux_frame(index) = '1') then
                            printf("- All channels in the MUX frame are ok.");
                          end if;
                          printf("- Output clock on aggregate interface is ok.");
                          if (check_delay(index) = '1') then
                            printf("- Transmission delay is ok.");
                          end if;
      when others      => printf("- No comment available yet." & LF & " ");
    end case;
  end generate_message;
  
END tb_control_p;
