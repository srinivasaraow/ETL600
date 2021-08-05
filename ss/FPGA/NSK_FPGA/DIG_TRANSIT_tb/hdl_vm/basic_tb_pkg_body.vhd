--
-- VHDL Package Body DIG_TRANSIT_tb.basic_tb_pkg
--
-- Created:
--          by - chstrue.UNKNOWN (CH-L-0014426)
--          at - 11:36:59 2011-02-22
--
-- using Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--



PACKAGE BODY basic_tb_pkg IS

  
    procedure printf(s : in string) is
      variable l : line;
    begin
      write(l, s);
      writeline(output, l);
    end printf;

  function stdv2str(vec  : std_logic_vector)
    return string is
      variable str: string(vec'left+1 downto 1);
    begin
      for i in vec'reverse_range loop
        case vec(i) is
          when 'U' => str(i+1):= 'U';
          when 'X' => str(i+1):= 'X';
          when '0' => str(i+1):= '0';
          when '1' => str(i+1):= '1';
          when 'Z' => str(i+1):= 'Z';
          when 'W' => str(i+1):= 'W';
          when 'L' => str(i+1):= 'L';
          when 'H' => str(i+1):= 'H';
          when others => str(i+1):= '-';
        end case;
      end loop;
      return str;
    end stdv2str;
  
  function slv2int(slv : std_logic_vector)    -- std_logic_vector to convert
    return integer is
        variable result : integer := 0;           -- conversion result
    begin
      for i in slv'range loop
        result  := result * 2;                  -- shift the variable to left
        case slv(i) is
          when '1' | 'H' => result := result + 1;
          when '0' | 'L' => result := result + 0;
          when others    => null;
        end case;
      end loop;
    return result;
  end slv2int;

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



-- converts a std_logic_vector into a hex string. from: www.stefanvhdl.com/vhdl/vhdl/txt_util.vhd

function hstr(slv: std_logic_vector) return string is
variable slv_loc : std_logic_vector(slv'left-slv'right downto 0);
variable hexlen: integer;
variable longslv : std_logic_vector(67 downto 0) := (others => '0');
variable hex : string(1 to 16);
variable fourbit : std_logic_vector(3 downto 0);
begin
slv_loc := slv;
hexlen := (slv_loc'left+1)/4;
if (slv_loc'left+1) mod 4 /= 0 then
hexlen := hexlen + 1;
end if;
longslv(slv_loc'left downto 0) := slv_loc;
for i in (hexlen -1) downto 0 loop
fourbit := longslv(((i*4)+3) downto (i*4));
case fourbit is
when "0000" => hex(hexlen -I) := '0';
when "0001" => hex(hexlen -I) := '1';
when "0010" => hex(hexlen -I) := '2';
when "0011" => hex(hexlen -I) := '3';
when "0100" => hex(hexlen -I) := '4';
when "0101" => hex(hexlen -I) := '5';
when "0110" => hex(hexlen -I) := '6';
when "0111" => hex(hexlen -I) := '7';
when "1000" => hex(hexlen -I) := '8';
when "1001" => hex(hexlen -I) := '9';
when "1010" => hex(hexlen -I) := 'A';
when "1011" => hex(hexlen -I) := 'B';
when "1100" => hex(hexlen -I) := 'C';
when "1101" => hex(hexlen -I) := 'D';
when "1110" => hex(hexlen -I) := 'E';
when "1111" => hex(hexlen -I) := 'F';
when "ZZZZ" => hex(hexlen -I) := 'z';
when "UUUU" => hex(hexlen -I) := 'u';
when "XXXX" => hex(hexlen -I) := 'x';
when others => hex(hexlen -I) := '?';
end case;
end loop;
return hex(1 to hexlen);
end hstr;  

  
  
  
  




END basic_tb_pkg;
