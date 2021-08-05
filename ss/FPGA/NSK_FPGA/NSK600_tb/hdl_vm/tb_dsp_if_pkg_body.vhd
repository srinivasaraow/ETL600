--------------------------------------------------------------------------
-- $Workfile: tb_dsp_if_pkg_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_dsp_if_pkg_body.vhd $
-- 
-- 24    11-07-06 9:25 Chstrue
-- R4 implementation
-- 
-- 23    16.08.06 17:06 Chalgyg
-- TPR configuration enhanced for G.703 and V.11 interfaces.
-- 
-- 22    10.01.06 9:06 Chalgyg
-- LAN interface is now supported.
-- 
-- 21    18.04.05 15:59 Chalgyg
-- - TPR register added.
-- - Register address range adjusted.
-- 
-- 20    10.02.05 15:49 Chalgyg
-- Switched to DSP interface communication with address bus.
-- 
-- 19    2.02.05 15:09 Chalgyg
-- dsp_configure_fpga adapted.
-- 
-- 18    19.01.05 11:15 Chalgyg
-- CCR.MUX is only written if, PCR.POE = '1'.
-- 
-- 17    18.01.05 10:14 Chalgyg
-- V.11 Port 1 checks wether it is configured as MUX port or not.
-- 
-- 15    13.12.04 13:50 Chalgyg
-- procedure dsp_check_comm() changed.
-- 
-- 14    8.12.04 10:50 Chalgyg
-- Signal 'configured' added to procedure configure.
-- 
-- 13    7.12.04 14:23 Chalgyg
-- new procedure added.
-- 
-- 12    3.12.04 12:55 Chalgyg
-- -- no description --
-- 
-- 11    2.12.04 11:10 Chalgyg
-- Configuration procedure changed.
-- 
-- 10    24.11.04 10:23 Gygax02
-- RUD added in configuration of each testcase.
-- 
-- 9     20.10.04 11:46 Gygax02
-- -- no description --
-- 
-- 7     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 6     20.09.04 10:36 Unp00631
-- -- no description --
-- 
-- 5     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
PACKAGE BODY tb_dsp_if_p IS

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

  function int2slv(int_value : integer)
  return std_logic_vector is 
    variable result : std_logic_vector(5 downto 0); 
  begin 
    for i in 0 to 5 loop 
      if ((int_value/(2**i)) rem 2) = 0 then 
        result(i) := '0'; 
      else 
        result(i) := '1'; 
      end if; 
    end loop; 
    return result; 
  end int2slv; 

  function protocol2code(protocol : character; index : integer)    -- Transmission protocol to TPR register code
  return std_logic_vector is
    variable nr_bits: integer                      := 0;
    variable result : std_logic_vector(3 downto 0) := (others=>'0');
  begin
    if (protocol = 's') then
      result  := "1011";                      -- No start/stop bit orientation. 8 data bits.
    elsif (protocol = 'f') then
      nr_bits := nr_data_bits(index) + parity2nr(parity_bit(index)) + nr_stop_bits(index) - 1;
      case nr_bits is
        when 6  => result := "1001";
        when 7  => result := "1010";      
        when 8  => result := "1011";
        when 9  => result := "1100";
        when 10 => result := "1101";
        when others => printf("Configuration error in tb_ft1_patter_p.vhd!");
                       result := "1011";
      end case;
    end if;
   return result;
  end protocol2code;
       
  procedure dsp_write (
    constant  data      : in std_logic_vector(7 downto 0);
    constant  address   : in std_logic_vector(11 downto 0);
    signal    dsp_sig   : out t_dsp_port_sig;
    signal    dsp_data  : out std_logic_vector(7 downto 0)
  ) is
    variable twr: time;
  begin
    dsp_sig.dsp_cs_n  <= '0';
    dsp_sig.dsp_wr_n  <= '0';
    dsp_sig.dsp_rd_n  <= '1';
    dsp_sig.dsp_addr  <= address;
    dsp_data          <= data;
    twr  := 24.974 ns + (dsp_waitstates * 30.518 ns);
    wait for twr;
    dsp_sig.dsp_wr_n  <= '1';
    wait for 32.061 ns;
    dsp_sig.dsp_cs_n  <= '1';
    dsp_sig.dsp_addr  <= (others=>'0');
    dsp_data          <= (others=>'Z');
    wait for 12.465 ns;
  end dsp_write;
  
  procedure dsp_read (
    constant  address   : in std_logic_vector(11 downto 0);
    constant  dsp_data  : in std_logic_vector(7 downto 0);
    variable  data_out  : out std_logic_vector(7 downto 0);
    signal    dsp_sig   : out t_dsp_port_sig
  ) is
    variable trd: time;
  begin
    dsp_sig.dsp_cs_n  <= '0';
    dsp_sig.dsp_wr_n  <= '1';
    dsp_sig.dsp_rd_n  <= '0';
    dsp_sig.dsp_addr  <= address;
    trd  := 24.431 ns + (dsp_waitstates * 30.518 ns);
    wait for trd;
    dsp_sig.dsp_rd_n  <= '1';
    data_out          := dsp_data;
    wait for 32.061 ns;
    dsp_sig.dsp_cs_n  <= '1';
    dsp_sig.dsp_addr  <= (others=>'0');
    wait for 2.543 ns;
  end dsp_read;

  procedure dsp_passive (
    signal    dsp_data  : out std_logic_vector(7 downto 0);
    signal    dsp_sig   : out t_dsp_port_sig
  ) is
  begin
    dsp_sig.dsp_cs_n  <= '1';
    dsp_sig.dsp_wr_n  <= '1';
    dsp_sig.dsp_rd_n  <= '1';
    dsp_sig.dsp_addr  <= (others=>'0');
    dsp_data          <= (others => ('Z'));
    wait for 1 ns;
  end dsp_passive;
  
  procedure dsp_compare (
    constant  address        : in std_logic_vector(11 downto 0);
    constant  length         : in integer;
    signal    dsp_data       : inout std_logic_vector(7 downto 0);
    signal    dsp_sig        : out t_dsp_port_sig;
    variable  dsp_port_fail  : out std_logic
  ) is

  variable comp_data_in  : std_logic_vector(7 downto 0) := "10101100";
  variable comp_data_out : std_logic_vector(7 downto 0);

  begin
    dsp_write(comp_data_in,address,dsp_sig,dsp_data);
    wait for 1 ps;
    dsp_read(address,dsp_data,comp_data_out,dsp_sig);
    wait for 1 ps;
    if (comp_data_out((length-1) downto 0) /= comp_data_in((length-1) downto 0)) then
      assert (false)
        report "DSP interface communication failure at address: " & stdv2str(address)
        severity warning;
      dsp_port_fail := '1';
    else
      dsp_port_fail := '0';
    end if;
    wait for 1 ps;
  end dsp_compare;

  procedure dsp_check_comm (
    signal    dsp_data       : inout std_logic_vector(7 downto 0);
    signal    dsp_sig        : out t_dsp_port_sig;
    variable  dsp_port_fail  : out std_logic
  ) is

  variable failure_occured  : std_logic;
  variable error_flag       : std_logic := '0';
  variable version          : std_logic_vector(7 downto 0);

  begin
    dsp_read(IDNT_AD,dsp_data,version,dsp_sig);
    printf("FPGA has Version " & int2str(slv2int(version)));
    for i in 0 to 3 loop
      dsp_compare((RUD_AD(0)+int2slv(i)),RUD_LEN,dsp_data,dsp_sig,failure_occured);
      error_flag := error_flag or failure_occured;
    end loop;
    dsp_compare(RSIG_AD,RSIG_LEN,dsp_data,dsp_sig,failure_occured);
    error_flag := error_flag or failure_occured;
    for i in 0 to 10 loop
      dsp_compare((PCR_AD(0)+int2slv(5*i)),PCR_LEN,dsp_data,dsp_sig,failure_occured);
      error_flag := error_flag or failure_occured;
      dsp_compare((CCR_AD(0)+int2slv(5*i)),CCR_LEN,dsp_data,dsp_sig,failure_occured);
      error_flag := error_flag or failure_occured;
      dsp_compare((TPR_AD(0)+int2slv(5*i)),TPR_LEN,dsp_data,dsp_sig,failure_occured);
      error_flag := error_flag or failure_occured;
      dsp_compare((BRR_AD(0)+int2slv(5*i)),BRR_LEN,dsp_data,dsp_sig,failure_occured);
      error_flag := error_flag or failure_occured;
    end loop;
    dsp_port_fail  := error_flag;
    wait for 1 ns;
  end dsp_check_comm;

  procedure dsp_configure_fpga (
    constant  testcase       : in t_testcase;
    signal    dsp_data       : inout std_logic_vector(7 downto 0);
    signal    dsp_sig        : out t_dsp_port_sig;
    signal    configured     : out std_logic
  ) is
  
  begin
  -- BRR configuration
    for i in 0 to 3 loop    -- 4 V.24 ports initial value for RX direction
      dsp_write("10000000",RUD_AD(i),dsp_sig,dsp_data);
    end loop;
    dsp_write(baudrate2code(baudrate_value_ofdm(testcase)),BRR_AD(0),dsp_sig,dsp_data);     -- Configure Baudrate OFDM Modem.
    dsp_write(baudrate2code(baudrate_value(0,testcase)),BRR_AD(1),dsp_sig,dsp_data);        -- Configure Baudrate V.24 Ch1.
    dsp_write(baudrate2code(baudrate_value(1,testcase)),BRR_AD(2),dsp_sig,dsp_data);        -- Configure Baudrate V.24 Ch2.
    dsp_write(baudrate2code(baudrate_value(2,testcase)),BRR_AD(3),dsp_sig,dsp_data);        -- Configure Baudrate V.24 Ch3.
    dsp_write(baudrate2code(baudrate_value(3,testcase)),BRR_AD(4),dsp_sig,dsp_data);        -- Configure Baudrate V.24 Ch4.
    dsp_write(baudrate2code(baudrate_value(4,testcase)),BRR_AD(5),dsp_sig,dsp_data);        -- Configure Baudrate V.24 Ch5.
    dsp_write(baudrate2code(baudrate_value(5,testcase)),BRR_AD(6),dsp_sig,dsp_data);        -- Configure Baudrate V.24 Ch6.
    dsp_write(baudrate2code(baudrate_value_v11(0,testcase)),BRR_AD(7),dsp_sig,dsp_data);    -- Configure Baudrate V.11 Ch1.
    dsp_write(baudrate2code(baudrate_value_v11(1,testcase)),BRR_AD(8),dsp_sig,dsp_data);    -- Configure Baudrate V.11 Ch2.
    dsp_write(baudrate2code(baudrate_value_v11(2,testcase)),BRR_AD(9),dsp_sig,dsp_data);    -- Configure Baudrate G.703.
    dsp_write(baudrate2code(baudrate_value_v11(3,testcase)),BRR_AD(10),dsp_sig,dsp_data);   -- Configure Baudrate LAN.
  -- CCR configuration
    dsp_write((clock_config_registers(0,testcase) & "00"),CCR_AD(0),dsp_sig,dsp_data);  -- Configure Clock Configuration Registers OFDM modem.
    for i in 0 to 5 loop    -- 6 V.24 ports to configure
      dsp_write((clock_config_registers((i+1),testcase) & mode2code(mode(i,testcase),switchbox_modem(testcase))),CCR_AD(i+1),dsp_sig,dsp_data);
    end loop;
    dsp_write((clock_config_registers(7,testcase) & '0' & (v11_mode2code(mode_v11(testcase)) and port_config_registers(7,testcase)(0))),CCR_AD(7),dsp_sig,dsp_data); -- Configure Clock Configuration Registers of first V11 port.
    for i in 8 to 10 loop   -- Last 3 ports to configure
      dsp_write((clock_config_registers(i,testcase) & '0' & port_config_registers(i,testcase)(0)),CCR_AD(i),dsp_sig,dsp_data);
    end loop;
  -- TPR configuration
    for i in 0 to 5 loop    -- 6 V.24 ports to configure
      dsp_write(("0000" & protocol2code(pattern_protocol(testcase),i)),TPR_AD(i+1),dsp_sig,dsp_data);
    end loop;
    for i in 7 to 8 loop    -- 2 V.11 ports to configure
--str      dsp_write(("00" & clock_config_registers(i,testcase)(0) & "00000"),TPR_AD(i),dsp_sig,dsp_data); -- Set ETL600 master or slave.
      dsp_write(("000" & "00000"),TPR_AD(i),dsp_sig,dsp_data); -- Set ETL600 master or slave.

    end loop;
    dsp_write("00100011",TPR_AD(9),dsp_sig,dsp_data);  -- G.703 port is slave and has 8 units per default.
  -- PCR configuration
    for i in 0 to 5 loop    -- 6 V.24 ports to configure
      dsp_write((share2code(mode(i,testcase)) & port_config_registers((i+1),testcase)),PCR_AD(i+1),dsp_sig,dsp_data);
    end loop;
    for i in 7 to 10 loop   -- Last 4 ports to configure
      dsp_write(('0' & port_config_registers(i,testcase)),PCR_AD(i),dsp_sig,dsp_data);
    end loop;
    dsp_write(('0' & port_config_registers(0,testcase)),PCR_AD(0),dsp_sig,dsp_data);          -- Configure Port Configuration Registers OFDM Modem.
    configured  <= '1';
    wait for 1 ps;
    configured  <= '0';
  end dsp_configure_fpga;
  
  procedure dsp_read_all_TUD (
    signal    dsp_data  : inout std_logic_vector(7 downto 0);
    variable  data_out  : out t_channel_array;
    signal    dsp_sig   : out t_dsp_port_sig
  ) is
  
  variable data_from_fpga  : std_logic_vector(7 downto 0);
  
  begin
    dsp_read(TUD1_AD,dsp_data,data_from_fpga,dsp_sig);
    data_out(0)  := data_from_fpga;
    dsp_read(TUD2_AD,dsp_data,data_from_fpga,dsp_sig);
    data_out(1)  := data_from_fpga;
    dsp_read(TUD3_AD,dsp_data,data_from_fpga,dsp_sig);
    data_out(2)  := data_from_fpga;
    dsp_read(TUD4_AD,dsp_data,data_from_fpga,dsp_sig);
    data_out(3)  := data_from_fpga;
  end dsp_read_all_TUD;

  procedure dsp_write_all_RUD (
    constant  data      : t_channel_array;
    signal    dsp_sig   : out t_dsp_port_sig;
    signal    dsp_data  : out std_logic_vector(7 downto 0)
  ) is
  
  begin
    for i in 0 to 3 loop    -- Write all four ports
      dsp_write(data(i),RUD_AD(i),dsp_sig,dsp_data);
    end loop;
  end dsp_write_all_RUD;

  procedure dsp_handle_flag0 (
    signal    flag0     : out std_logic
  ) is
  
  begin
    flag0  <= '1';
    wait for 100 ns;
    flag0  <= '0';
  end dsp_handle_flag0;

  procedure reset_fpga (
    signal    fpga_reset_n: out std_logic
  ) is
  
  begin
      fpga_reset_n  <= '0';
      wait for 5 us;
      fpga_reset_n  <= '1';
      wait for 5 us;
  end reset_fpga;

END tb_dsp_if_p;
