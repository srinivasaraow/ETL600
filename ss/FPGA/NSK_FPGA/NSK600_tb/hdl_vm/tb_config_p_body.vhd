--------------------------------------------------------------------------
-- $Workfile: tb_config_p_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_config_p_body.vhd $
-- 
-- 13    08-04-22 16:24 Chstrue
-- -- no description --
-- 
-- 12    21.02.05 12:06 Chalgyg
-- New datarates added.
-- 
-- 11    18.01.05 10:12 Chalgyg
-- v11_mode2code() added.
-- 
-- 9     13.12.04 13:47 Chalgyg
-- -- no description --
-- 
-- 8     20.10.04 11:45 Gygax02
-- -- no description --
-- 
-- 6     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 5     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
PACKAGE BODY tb_config_p IS

  function baudrate2code(baudrate: integer) return std_logic_vector is
    variable code: std_logic_vector(7 downto 0);
  begin
    case baudrate is
      when  0      => code := "00000000";
      when  100    => code := "00000001";
      when  200    => code := "00000010";
      when  300    => code := "00000011";
      when  600    => code := "00000100";
      when  1200   => code := "00000101";
      when  2400   => code := "00000110";
      when  4800   => code := "00000111";
      when  9600   => code := "00001001";
      when  11000  => code := "00110000";
      when  12000  => code := "00001010";
      when  14400  => code := "00001011";
      when  16000  => code := "00001100";
      when  19200  => code := "00001101";
      when  24000  => code := "00001110";
      when  28800  => code := "00001111";
      when  32000  => code := "00010000";
      when  36000  => code := "00010001";
      when  38400  => code := "00010010";
      when  40000  => code := "00010011";
      when  48000  => code := "00010100";
      when  56000  => code := "00010101";
      when  64000  => code := "00010111";
      when  72000  => code := "00011000";
      when  76800  => code := "00011001";
      when  80000  => code := "00011010";
      when  96000  => code := "00011011";
      when  112000 => code := "00011100";
      when  128000 => code := "00011110";
      when  144000 => code := "00011111";
      when  153600 => code := "00100000";
      when  160000 => code := "00100001";
      when  192000 => code := "00100010";
      when  224000 => code := "00100011";
      when  256000 => code := "00100101";
      when  288000 => code := "00100110";
      when  307200 => code := "00100111";
      when  320000 => code := "00101000";
      when others  => code := "00000000";
    end case;
    return code;
  end baudrate2code;

  function baudrate2config(baudrate: integer) return integer is
    variable config: integer range 0 to 192;
  begin
    case baudrate is
      when  0     => config := 0;
      when  100   => config := 192;
      when  200   => config := 96;
      when  300   => config := 64;
      when  600   => config := 32;
      when  1200  => config := 16;
      when  2400  => config := 8;
      when  4800  => config := 4;
      when  9600  => config := 2;
      when 19200  => config := 1;
      when others => config := 1;
    end case;
    return config;
  end baudrate2config;

  function mode2code(mode: character; switchbox: character) return std_logic_vector is
    variable code: std_logic_vector(1 downto 0);
  begin
    if (mode = 's') then
      case switchbox is
        when  'o'   => code := "00";        -- off
        when  'f'   => code := "10";        -- FSK
        when  'a'   => code := "00";        -- ADPSK
        when  'm'   => code := "01";        -- Multiplexer
        when others => code := "00";
      end case;
    else
      case mode is
        when  'o'   => code := "00";        -- off
        when  'f'   => code := "10";        -- FSK
        when  'a'   => code := "00";        -- ADPSK
        when  'm'   => code := "01";        -- Multiplexer
        when others => code := "00";
      end case;
    end if;
    return code;
  end mode2code;

  function v11_mode2code(v11_mode: character) return std_logic is
    variable code: std_logic;
  begin
    if (v11_mode = 'm') then
      code := '1';                          -- Multiplexer
    else
      code := '0';                          -- Direct OFDM connection
    end if;
    return code;
  end v11_mode2code;

  function share2code(mode: character) return std_logic is
    variable code: std_logic;
  begin
    if (mode = 's') then
      code := '1';                          -- Port share mode
    else
      code := '0';                          -- Port not shared
    end if;
    return code;
  end share2code;

  function deviation2time(deviation: integer) return time is
    variable temp: time;
  begin
    temp := 52.083333 us - (52.083333 us * (real(deviation) * 0.000001));
    return temp;
  end deviation2time;

END tb_config_p;
