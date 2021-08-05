--------------------------------------------------------------------------
-- $Workfile: tb_ofdm_fpga_pkg_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_ofdm_fpga_pkg_body.vhd $
-- 
-- 7     11-07-06 9:29 Chstrue
-- R4 implementation
-- 
-- 7     29.06.11 12:00 dsg@zuehlke.com
-- bug in fill_channel solved when start bit is in part2.
-- 
-- 6     10.01.06 9:06 Chalgyg
-- LAN interface is now supported.
-- 
-- 5     30.03.05 9:20 Chalgyg
-- - Problem with data, shorter than 1 frame is solved.
-- - V.11 data can now start with a '1' as well.
-- 
-- 4     25.02.05 13:22 Chalgyg
-- Signal mark_low added for debugging purposes.
-- 
-- 3     24.02.05 15:12 Chalgyg
-- Whole mux frame detection added.
-- 
-- 1     22.10.04 16:24 Gygax02
-- -- no description --
---------------------------------------------------------------------------
--110630
PACKAGE BODY tb_ofdm_fpga_p IS
  
  function slv2real(slv : std_logic_vector)    -- std_logic_vector to convert
  return real is
    variable result : real := 0.0;               -- conversion result
  begin
    for i in slv'range loop
      result  := result * 2.0;                     -- shift the variable to left
      case slv(i) is
        when '1' | 'H' => result := result + 1.0;
        when '0' | 'L' => result := result + 0.0;
        when others    => null;
      end case;
    end loop;
    return result;
  end;

  function give_first_0_pos(slv : std_logic_vector)
  return integer is
    variable result      : integer   := -1;
    variable bit_detected: std_logic := '0';
  begin
    for i in 0 to (slv'length-1) loop
      if ((slv((slv'length-1)-i) = '0') and (bit_detected = '0')) then
        result       := i;
        bit_detected := '1';
      end if;
    end loop;
    return result;
  end;

  procedure analyse_mux_frame (
    signal   testcase          : in t_testcase;
    signal   sclk0             : in std_logic;
    signal   v11_control       : in std_logic;
    signal   d0a               : in std_logic;
    signal   mark_low          : out std_logic;
    signal   failure           : out std_logic
  ) is

    type ch_container is record
      ch1 : std_logic_vector (((baudrate_value(0,testcase) / 100) + 1) downto 0);
      ch2 : std_logic_vector (((baudrate_value(1,testcase) / 100) + 1) downto 0);
      ch3 : std_logic_vector (((baudrate_value(2,testcase) / 100) + 1) downto 0);
      ch4 : std_logic_vector (((baudrate_value(3,testcase) / 100) + 1) downto 0);
      ch5 : std_logic_vector (((baudrate_value(4,testcase) / 100) + 1) downto 0);
      ch6 : std_logic_vector (((baudrate_value(5,testcase) / 100) + 1) downto 0);
      ch7 : std_logic_vector (((baudrate_value_v11(0,testcase) / 100) + 1) downto 0);
      ch8 : std_logic_vector (((baudrate_value_v11(1,testcase) / 100) + 1) downto 0);
      ch9 : std_logic_vector (((baudrate_value_v11(2,testcase) / 100) + 1) downto 0);
      ch10: std_logic_vector (((baudrate_value_v11(3,testcase) / 100) + 1) downto 0);
    end record ch_container;
  
    type ch_rec_pattern is record
      ch1 : std_logic_vector (((pattern_v24(0,testcase)'length+2)*nr_of_repetitions_v24(0,testcase)-1) downto 0);
      ch2 : std_logic_vector (((pattern_v24(1,testcase)'length+2)*nr_of_repetitions_v24(1,testcase)-1) downto 0);
      ch3 : std_logic_vector (((pattern_v24(2,testcase)'length+2)*nr_of_repetitions_v24(2,testcase)-1) downto 0);
      ch4 : std_logic_vector (((pattern_v24(3,testcase)'length+2)*nr_of_repetitions_v24(3,testcase)-1) downto 0);
      ch5 : std_logic_vector (((pattern_v24(4,testcase)'length+2)*nr_of_repetitions_v24(4,testcase)-1) downto 0);
      ch6 : std_logic_vector (((pattern_v24(5,testcase)'length+2)*nr_of_repetitions_v24(5,testcase)-1) downto 0);
      ch7 : std_logic_vector (((pattern_v11(0,testcase)'length)*nr_of_repetitions_v11(0,testcase)-1) downto 0);
      ch8 : std_logic_vector (((pattern_v11(1,testcase)'length)*nr_of_repetitions_v11(1,testcase)-1) downto 0);
      ch9 : std_logic_vector (((pattern_v11(2,testcase)'length)*nr_of_repetitions_v11(2,testcase)-1) downto 0);
      ch10: std_logic_vector (((pattern_v11(3,testcase)'length)*nr_of_repetitions_v11(3,testcase)-1) downto 0);
    end record ch_rec_pattern;

    function find_enabled_channels(testcase : t_testcase)
    return std_logic_vector is
      variable result : std_logic_vector(9 downto 0) := (others=>'0');
    begin
      for i in 0 to 5 loop
        if (mode(i,testcase) = 'm') then
          result(i) := '1';
        end if;
      end loop;
      if (switchbox_modem(testcase) = 'm') then
        result(0) := '1';
      end if;
      if (mode_v11(testcase) = 'm') then
        if (port_config_registers(7,testcase)(0) = '1') then
          result(6) := '1';
        end if;
        if (port_config_registers(8,testcase)(0) = '1') then
          result(7) := '1';
        end if;
        if (port_config_registers(9,testcase)(0) = '1') then
          result(8) := '1';
        end if;
        if (port_config_registers(10,testcase)(0) = '1') then
          result(9) := '1';
        end if;
      end if;
      return result;
    end;
  
    function extract_channel_length(testcase : t_testcase; ch_enable : std_logic_vector)
    return integer_vect is
      variable result : integer_vect := (others=>0);
    begin
      for i in 0 to 5 loop
        if (ch_enable(i) = '1') then
          result(i) := (baudrate_value(i,testcase) / 100) + 1;
        end if;
      end loop;
      for i in 6 to 9 loop
        if (ch_enable(i) = '1') then
          result(i) := (baudrate_value_v11((i-6),testcase) / 100) + 1;
        end if;
      end loop;
      return result;
    end;
  
    function get_container_from_record(index : integer; length : integer; recrd : ch_container)
    return std_logic_vector is
      variable result : std_logic_vector(length downto 0) := (others=>'0');
    begin
      case index is
        when  0 => result := recrd.ch1;
        when  1 => result := recrd.ch2;
        when  2 => result := recrd.ch3;
        when  3 => result := recrd.ch4;
        when  4 => result := recrd.ch5;
        when  5 => result := recrd.ch6;
        when  6 => result := recrd.ch7;
        when  7 => result := recrd.ch8;
        when  8 => result := recrd.ch9;
        when  9 => result := recrd.ch10;
        when others => result := recrd.ch1;
      end case;
      return result;
    end;
  
    function get_rec_pattern_from_record(index : integer; length : integer; recrd : ch_rec_pattern)
    return std_logic_vector is
      variable result : std_logic_vector((length-1) downto 0) := (others=>'0');
    begin
      case index is
        when  0 => result := recrd.ch1;
        when  1 => result := recrd.ch2;
        when  2 => result := recrd.ch3;
        when  3 => result := recrd.ch4;
        when  4 => result := recrd.ch5;
        when  5 => result := recrd.ch6;
        when  6 => result := recrd.ch7;
        when  7 => result := recrd.ch8;
        when  8 => result := recrd.ch9;
        when  9 => result := recrd.ch10;
        when others => result := recrd.ch1;
      end case;
      return result;
    end;

    function generate_boundary_indication(bit_counter : integer; ch_lo : integer_vect; ch_enable : std_logic_vector; frame_length : integer)
    return std_logic is
    variable result : std_logic := '0';
    begin
      for i in 0 to 9 loop
        if (ch_enable(i) = '1') then
          if (bit_counter = (frame_length-ch_lo(i))) then
            result := '1';
          end if;
        end if;
      end loop;
      return result;
    end;
  
    procedure save_rec_pattern_into_record (
      constant  index      : in integer;
      constant  slv        : in std_logic_vector;
      constant  hi         : in integer;
      constant  lo         : in integer;
      variable  recrd      : inout ch_rec_pattern
    ) is
    begin
      case index is
        when  0 => recrd.ch1(hi downto lo) := slv;
        when  1 => recrd.ch2(hi downto lo) := slv;
        when  2 => recrd.ch3(hi downto lo) := slv;
        when  3 => recrd.ch4(hi downto lo) := slv;
        when  4 => recrd.ch5(hi downto lo) := slv;
        when  5 => recrd.ch6(hi downto lo) := slv;
        when  6 => recrd.ch7(hi downto lo) := slv;
        when  7 => recrd.ch8(hi downto lo) := slv;
        when  8 => recrd.ch9(hi downto lo) := slv;
        when  9 => recrd.ch10(hi downto lo):= slv;
        when others => recrd.ch1(hi downto lo) := slv;
      end case;
    end save_rec_pattern_into_record;
  
    procedure fill_channel (
      constant testcase          : in t_testcase;
      constant container         : in std_logic_vector;
      constant last_pos          : in integer;
      constant received_cont     : in integer;
      constant pattern_length    : in integer;
      constant index             : in integer;
      variable stop_pos          : out integer;
      variable rec_pattern       : inout ch_rec_pattern;
      variable nr_rec_containers : out integer
    ) is
      variable bit_detected          : std_logic := '0';
      variable container_start_pos   : integer;
      variable start_pos             : integer;
      variable stop_pos_int          : integer;
      variable cont_part_1           : std_logic_vector((container'length-4) downto 0);
      variable cont_part_2           : std_logic_vector(1 downto 0);
      variable pattern_start_pos     : integer;
    begin
      if (received_cont = 0) then
        cont_part_1         := container((container'length-1) downto 3);
        cont_part_2         := container(1 downto 0);
        if (index > 5) then             -- V.11 interface could start with a '1';
          pattern_start_pos   := give_first_0_pos(pattern_v11((index-6),testcase));
        else
          pattern_start_pos   := 0;
        end if;
        container_start_pos := give_first_0_pos(cont_part_1) - pattern_start_pos;
        
        if ((container_start_pos <= -1) and (container(2) = '1')) then    -- No zero in 1st part of container, check in 2nd part.
          container_start_pos := give_first_0_pos(cont_part_2) - pattern_start_pos;
         if (container_start_pos > -1) then
            container_start_pos := container_start_pos + (container'length-2);
          end if;
        end if;

        if (container_start_pos > -1) then    -- Found a zero in container, start filling container to pattern record.
          start_pos     := last_pos-1;
          stop_pos_int  := start_pos-((container'length-1)-container_start_pos)+3;
          if (stop_pos_int >= 2) then    -- Container is smaller than the rest of the pattern
            if stop_pos_int < pattern_length then
              save_rec_pattern_into_record(index,container(((container'length-1)-container_start_pos) downto 3),(pattern_length-1),stop_pos_int,rec_pattern);
              if (container(2) = '1') then
                stop_pos_int := stop_pos_int - 2;
                  save_rec_pattern_into_record(index,container(1 downto 0),(stop_pos_int+1),stop_pos_int,rec_pattern);
              end if;
            else
              if (container(2) = '1' and container(0) = '0') then
                stop_pos_int := stop_pos_int - 3;
                  save_rec_pattern_into_record(index,container(0 downto 0),stop_pos_int,stop_pos_int,rec_pattern);
              end if;
            end if;
          else                           -- Container is bigger than the rest of the pattern
            save_rec_pattern_into_record(index,container(((container'length-1)-container_start_pos) downto (((container'length-1)-container_start_pos)-start_pos)),(pattern_length-1),0,rec_pattern);
          end if;
          nr_rec_containers := received_cont + 1;
          stop_pos          := stop_pos_int;
          wait for 1 ps;
        else
          nr_rec_containers := received_cont;
          stop_pos          := last_pos;
        end if;
      else
        start_pos     := last_pos-1;
        stop_pos_int  := start_pos-(container'length-1)+3;

        if (stop_pos_int >= 2) then    -- Container is smaller than the rest of the pattern
          save_rec_pattern_into_record(index,container((container'length-1) downto 3),start_pos,stop_pos_int,rec_pattern);
          if (container(2) = '1') then
            stop_pos_int := stop_pos_int - 2;
            save_rec_pattern_into_record(index,container(1 downto 0),(stop_pos_int+1),stop_pos_int,rec_pattern);
          end if;
        else                           -- Container is bigger than the rest of the pattern
          save_rec_pattern_into_record(index,container((container'length-1) downto ((container'length-1)-start_pos)),start_pos,0,rec_pattern);
        end if;
        nr_rec_containers := received_cont + 1;
        stop_pos          := stop_pos_int;
        wait for 1 ps;
      end if;
    end fill_channel;
  
    procedure compare_frame_v11 (
      constant  id           : in integer;
      constant  testcase     : in t_testcase;
      constant  rec_pattern  : in ch_rec_pattern;
      constant  length       : in integer;
      variable  failure      : out std_logic
    ) is
      variable config_vec     : std_logic_vector(((pattern_v11(id,testcase)'length * nr_of_repetitions_v11(id,testcase))-1) downto 0) := (others=>'0');
    begin
      for j in 1 to nr_of_repetitions_v11(id,testcase) loop
        config_vec((((pattern_v11(id,testcase)'length)*j)-1) downto ((pattern_v11(id,testcase)'length)*(j-1))) := pattern_v11(id,testcase);
      end loop;
      if (config_vec /= get_rec_pattern_from_record((id+6),length,rec_pattern)) then
        assert (false)
          report "MUX frame error detected on channel " & int2str(id+7)
          severity warning;
        failure := '1';
      else
        failure := '0';
      end if;
    end compare_frame_v11;
  
    procedure compare_frame_v24 (
      constant  id           : in integer;
      constant  testcase     : in t_testcase;
      constant  rec_pattern  : in ch_rec_pattern;
      constant  length       : in integer;
      variable  failure      : out std_logic
    ) is
      variable config_vec     : std_logic_vector((((pattern_v24(id,testcase)'length+2) * nr_of_repetitions_v24(id,testcase))-1) downto 0) := (others=>'0');
    begin
      for j in 1 to nr_of_repetitions_v24(id,testcase) loop
        config_vec((((pattern_v24(id,testcase)'length+2)*j)-1) downto ((pattern_v24(id,testcase)'length+2)*(j-1))) := ('0' & pattern_v24(id,testcase) & '1');
      end loop;
      if (config_vec /= get_rec_pattern_from_record(id,length,rec_pattern)) then
        assert (false)
          report "MUX frame error detected on channel " & int2str(id+1)
          severity warning;
        failure := '1';
        assert (false)
          report "length: " & integer'image(length) 
          severity note;
      else
        failure := '0';
      end if;
    end compare_frame_v24;
  
    procedure generate_pos (
      constant  frame_length   : in integer;
      constant  channel_length : in integer_vect;
      constant  ch_enable      : in std_logic_vector;
      variable  ch_hi          : out integer_vect;
      variable  ch_lo          : out integer_vect
    ) is
      variable last_ch      : integer      := 0;
      variable first_ch_sat : std_logic    := '0';
      variable ch_hi_int    : integer_vect := (others=>0);
      variable ch_lo_int    : integer_vect := (others=>0);
    begin
      for i in 0 to 9 loop
        if (ch_enable(i) = '1') then
          if (first_ch_sat = '0') then
            ch_hi_int(i) := frame_length;
            first_ch_sat := '1';
          else
            ch_hi_int(i) := ch_lo_int(last_ch) - 1;
          end if;
          ch_lo_int(i) := ch_hi_int(i) - channel_length(i);
          last_ch      := i;
        end if;
      end loop;
      ch_hi := ch_hi_int;
      ch_lo := ch_lo_int;
    end generate_pos;
  
    procedure generate_pattern_lengths (
      constant  testcase           : in t_testcase;
      constant  ch_enable          : in std_logic_vector;
      variable  rec_pattern_length : out integer_vect;
      variable  last_pos           : out integer_vect
    ) is
      variable  rec_pattern_length_int : integer_vect := (others=>0);
    begin
      for i in 0 to 5 loop
        if (ch_enable(i) = '1') then
          rec_pattern_length_int(i) := ((pattern_v24(i,testcase)'length+2) * nr_of_repetitions_v24(i,testcase));
        end if;
      end loop;
      for i in 6 to 9 loop
        if (ch_enable(i) = '1') then
          rec_pattern_length_int(i) := ((pattern_v11((i-6),testcase)'length) * nr_of_repetitions_v11((i-6),testcase));
        end if;
      end loop;
      rec_pattern_length := rec_pattern_length_int;
      last_pos           := rec_pattern_length_int;
    end generate_pattern_lengths;
  
    procedure save_container_into_record (
      constant  index      : in integer;
      constant  slv        : in std_logic_vector;
      variable  recrd      : inout ch_container
    ) is
    begin
      case index is
        when  0 => recrd.ch1 := slv;
        when  1 => recrd.ch2 := slv;
        when  2 => recrd.ch3 := slv;
        when  3 => recrd.ch4 := slv;
        when  4 => recrd.ch5 := slv;
        when  5 => recrd.ch6 := slv;
        when  6 => recrd.ch7 := slv;
        when  7 => recrd.ch8 := slv;
        when  8 => recrd.ch9 := slv;
        when  9 => recrd.ch10:= slv;
        when others => recrd.ch1 := slv;
      end case;
    end save_container_into_record;

    variable preambel          : std_logic_vector (8 downto 0)   := (others=>'0');
    variable frame_container   : std_logic_vector (((baudrate_value_ofdm(testcase) / 100) - 10) downto 0)  := (others=>'0');
    variable frame_length      : integer;
    variable bit_counter       : integer                         := 0;
    variable container         : ch_container                    := ((others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'));
    variable received_pattern  : ch_rec_pattern                  := ((others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'),(others=>'0'));
    variable rec_pattern_length: integer_vect;
    variable channel_length    : integer_vect;
    variable ch_hi             : integer_vect;
    variable ch_lo             : integer_vect;
    variable finished          : std_logic_vector(9 downto 0)    := (others => '0');
    variable last_pos          : integer_vect                    := (others=>0);
    variable received_cont     : integer_vect                    := (others=>0);
    variable stop_pos          : integer_vect                    := (others=>0);
    variable nr_rec_containers : integer_vect                    := (others=>0);
    variable frame_receiving   : std_logic                       := '0';
    variable frame_failure     : std_logic_vector(9 downto 0)    := (others => '0');
    variable ch_enable         : std_logic_vector(9 downto 0)    := (others => '0');

  begin
    ch_enable        := find_enabled_channels(testcase);
    channel_length   := extract_channel_length(testcase,ch_enable);
    frame_length     := (baudrate_value_ofdm(testcase) / 100) - 10;
    generate_pos(frame_length,channel_length,ch_enable,ch_hi,ch_lo);
    generate_pattern_lengths(testcase,ch_enable,rec_pattern_length,last_pos);
    finished         := not(ch_enable);
    while (finished /= "1111111111") loop
      wait until ((sclk0 = '0') or (v11_control = '1'));
      if (frame_receiving = '0') then
        preambel := preambel(7 downto 0) & d0a;
        if (preambel = "010110011") then
          frame_receiving := '1';
        end if;
      else
        frame_container((frame_container'length - 1) - bit_counter) := d0a;
        bit_counter := bit_counter + 1;
        mark_low    <= generate_boundary_indication(bit_counter,ch_lo,ch_enable,frame_length);
        if ((bit_counter = frame_length) or (v11_control = '1')) then
          frame_receiving := '0';
          bit_counter     := 0;
          for i in 0 to 9 loop
            if ((ch_enable(i) = '1') and (finished(i) = '0')) then
              save_container_into_record(i,frame_container(ch_hi(i) downto ch_lo(i)),container);
              fill_channel(testcase,get_container_from_record(i,channel_length(i),container),last_pos(i),received_cont(i),rec_pattern_length(i),i,stop_pos(i),received_pattern,nr_rec_containers(i));
              if(stop_pos(i) < 2) then
                if (i <= 5) then
                  compare_frame_v24(i,testcase,received_pattern,rec_pattern_length(i),frame_failure(i));
                else
                  compare_frame_v11((i-6),testcase,received_pattern,rec_pattern_length(i),frame_failure(i));
                end if;
                finished(i) := '1';
              else
                last_pos(i)      := stop_pos(i);
                received_cont(i) := nr_rec_containers(i);
              end if;
            end if;
          end loop;
        end if;
      end if;
    end loop;
    failure  <= frame_failure(9) or frame_failure(8) or frame_failure(7) or frame_failure(6) or frame_failure(5) or frame_failure(4) or frame_failure(3) or frame_failure(2) or frame_failure(1) or frame_failure(0);
    wait for 1 ps;
  end analyse_mux_frame;

END tb_ofdm_fpga_p;
