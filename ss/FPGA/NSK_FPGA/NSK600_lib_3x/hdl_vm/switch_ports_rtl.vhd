--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.switch_ports.rtl
-- $Workfile: switch_ports_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/switch_ports_rtl.vhd $
-- 
-- 9     07-10-11 15:23 Chmaglo1
-- -- no description --
-- 
-- 5     3.10.06 13:25 Chmaglo1
-- -- no description --
-- 
-- 2     9.01.06 15:40 Chalgyg
-- LAN port fixed. Now it can be operated without V.11 port 1.
-- 
-- 1     15.11.05 11:13 Chmaglo1
-- LAN interface added, 
-- incl. div. updates for port9 and port10
-- 
-- 45    30.04.05 14:25 Chmaglo1
-- -- no description --
-- 
-- 20    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 3     1.12.04 10:41 Unp00631
-- Migration Test 2
-- 
-- 1     9.09.04 15:19 Unp00631
-- -- no description --
-- 0     06.09.2004 17:32:43 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY switch_ports IS
   PORT( 
      clk            : IN     std_logic;
      control_switch : IN     t_control_switch;
      reset_n        : IN     std_logic;
      rx_ch1         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch10        : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch2         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch3         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch4         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch5         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch6         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch7         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch8         : IN     std_logic_vector (2 DOWNTO 0);
      rx_ch9         : IN     std_logic_vector (2 DOWNTO 0);
      rx_nsk_1       : IN     std_logic_vector (3 DOWNTO 0);
      rx_nsk_2       : IN     std_logic_vector (3 DOWNTO 0);
      rx_nsk_3       : IN     std_logic_vector (3 DOWNTO 0);
      rx_nsk_4       : IN     std_logic_vector (3 DOWNTO 0);
      rx_ofdm        : IN     std_logic_vector (2 DOWNTO 0);
      tx_agr         : IN     std_logic_vector (2 DOWNTO 0);
      tx_enet        : IN     std_logic_vector (2 DOWNTO 0);
      tx_g703        : IN     std_logic_vector (2 DOWNTO 0);
      tx_v11_1       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v11_2       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v24_1       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v24_2       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v24_3       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v24_4       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v24_5       : IN     std_logic_vector (2 DOWNTO 0);
      tx_v24_6       : IN     std_logic_vector (2 DOWNTO 0);
      rx_agr         : OUT    std_logic_vector (2 DOWNTO 0);
      rx_enet        : OUT    std_logic_vector (3 DOWNTO 0);
      rx_g703        : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v11_1       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v11_2       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v24_1       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v24_2       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v24_3       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v24_4       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v24_5       : OUT    std_logic_vector (3 DOWNTO 0);
      rx_v24_6       : OUT    std_logic_vector (3 DOWNTO 0);
      status_switch  : OUT    t_status_switch;
      tx_ch1         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch10        : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch2         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch3         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch4         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch5         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch6         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch7         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch8         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ch9         : OUT    std_logic_vector (2 DOWNTO 0);
      tx_nsk_1       : OUT    std_logic_vector (2 DOWNTO 0);
      tx_nsk_2       : OUT    std_logic_vector (2 DOWNTO 0);
      tx_nsk_3       : OUT    std_logic_vector (2 DOWNTO 0);
      tx_nsk_4       : OUT    std_logic_vector (2 DOWNTO 0);
      tx_ofdm        : OUT    std_logic_vector (3 DOWNTO 0)
   );

-- Declarations

END switch_ports ;

--
--------------------------------------------------------------------------------
architecture rtl of switch_ports is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal rx_switch_1 : std_logic_vector(3 downto 0);
signal rx_switch_2 : std_logic_vector(3 downto 0);
signal rx_switch_3 : std_logic_vector(3 downto 0);
signal rx_switch_4 : std_logic_vector(3 downto 0);
signal rx_switch_5 : std_logic_vector(3 downto 0);
signal rx_switch_6 : std_logic_vector(3 downto 0);
type t_serialdata is array (1 to 10) of std_logic_vector(2 downto 0);
signal rx_ch : t_serialdata;
type t_shift_info is array (1 to 10) of std_logic_vector(7 downto 0);
signal shift_info : t_shift_info;
signal rx_on : std_logic_vector(10 downto 1);

--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    shift_info <= (others=>"00000000");
    rx_on <= (others=>'0');
    rx_switch_1 <= "0010";
    rx_switch_2 <= "0010";
    rx_switch_3 <= "0010";
    rx_switch_4 <= "0010";
    rx_switch_5 <= "0010";
    rx_switch_6 <= "0010";
    rx_v24_1 <= "0010";
    rx_v24_2 <= "0010";
    rx_v24_3 <= "0010";
    rx_v24_4 <= "0010";
    rx_v24_5 <= "0010";
    rx_v24_6 <= "0010";
    rx_v11_1 <= "0010";
    rx_v11_2 <= "0010";
    rx_g703 <= "0010";
    rx_enet <= "0010";
    tx_ch1 <= "010";
    tx_ch2 <= "010";
    tx_ch3 <= "010";
    tx_ch4 <= "010";
    tx_ch5 <= "010";
    tx_ch6 <= "010";
    tx_ch7 <= "010";
    tx_ch8 <= "010";
    tx_ch9 <= "010";
    tx_ch10 <= "010";
    tx_ofdm <= "0010";
    tx_nsk_1 <= "010";
    tx_nsk_2 <= "010";
    tx_nsk_3 <= "010";
    tx_nsk_4 <= "010";
    rx_agr <= "010";
    
  elsif clk'event and clk = '1' then
    for i in 1 to 10 loop --rx_ch´range loop
      shift_info(i) <= rx_ch(i)(2) & shift_info(i)(7 downto 1); -- needed for mux frame break filtering / level buffering
      if shift_info(i)="00000000" then
        rx_on(i) <= '0';
      else
        rx_on(i) <= '1';
      end if;
    end loop;

    --------------------------------------------------------------------------------
    -- MUX/NSK Switch 
    tx_nsk_1 <= tx_v24_1;
    if control_switch.mux(1) = '1' then  
      rx_switch_1 <= rx_on(1) & rx_ch1;
    else
      rx_switch_1 <= rx_nsk_1;
    end if;
    if control_switch.mux(2) = '1' then  
      rx_switch_2 <= rx_on(2) & rx_ch2;
    else
      rx_switch_2 <= rx_nsk_2;
    end if;
    if control_switch.mux(3) = '1' then  
      rx_switch_3 <= rx_on(3) & rx_ch3;
    else
      rx_switch_3 <= rx_nsk_3;
    end if;
    if control_switch.mux(4) = '1' then  
      rx_switch_4 <= rx_on(4) & rx_ch4;
    else
      rx_switch_4 <= rx_nsk_4;
    end if;
    rx_switch_5 <= rx_on(5) & rx_ch5;
    rx_switch_6 <= rx_on(6) & rx_ch6;
    rx_v11_2    <= rx_on(8) & rx_ch8;
    rx_g703     <= rx_on(9) & rx_ch9;
    rx_enet     <= rx_on(10) & rx_ch10;
    --------------------------------------------------------------------------------
    -- MUX/Transparent Switch 
    if control_switch.mux = "00000000000" then  
      rx_v11_1 <= '1' & rx_ofdm;
      tx_ofdm <= '1' & tx_v11_1(2 downto 1) & '0';  -- port0 is clock master
      rx_agr <= "010";
    else
      rx_v11_1 <= rx_on(7) & rx_ch7;
      tx_ofdm <= '1' & tx_agr(2 downto 1) & '0';
      rx_agr <= rx_ofdm;
    end if;
    --------------------------------------------------------------------------------
    -- Sharing Device
    rx_v24_1 <= rx_switch_1;
    if control_switch.shd2 = '1' then 
      rx_v24_2 <= (rx_switch_1(3) and not (tx_v24_1(2))) & rx_switch_1(2 downto 0); 
    else 
      rx_v24_2 <= rx_switch_2; 
    end if;
    rx_v24_3 <= rx_switch_3;
    rx_v24_4 <= rx_switch_4;
    if control_switch.shd5 = '1' then 
      rx_v24_5 <= (rx_switch_1(3) and not (tx_v24_1(2) or (tx_v24_2(2) and control_switch.shd2))) 
                  & rx_switch_1(2 downto 0); 
    else 
      rx_v24_5 <= rx_switch_5; 
    end if;
    if control_switch.shd6 = '1' then 
      rx_v24_6 <= (rx_switch_1(3) and not (tx_v24_1(2) or (tx_v24_2(2) and control_switch.shd2) or (tx_v24_5(2) and control_switch.shd5))) 
                  & rx_switch_1(2 downto 0); 
    else 
      rx_v24_6 <= rx_switch_6; 
    end if;
    
--    if control_switch.shd1='1' and tx_v24_1(2)='1' then 
    if tx_v24_1(2)='1' then 
      tx_nsk_1 <= tx_v24_1;
      tx_ch1 <= tx_v24_1;
    elsif control_switch.shd2='1' and tx_v24_2(2)='1' then 
      tx_nsk_1 <= tx_v24_2;
      tx_ch1 <= tx_v24_2;
    elsif control_switch.shd5='1' and tx_v24_5(2)='1' then 
      tx_nsk_1 <= tx_v24_5;
      tx_ch1 <= tx_v24_5;
    elsif control_switch.shd6='1' and tx_v24_6(2)='1' then 
      tx_nsk_1 <= tx_v24_6;
      tx_ch1 <= tx_v24_6;
    else
      tx_nsk_1 <= tx_v24_1;
      tx_ch1 <= tx_v24_1;
    end if;  

    --------------------------------------------------------------------------------
    -- Fix Mapping
    tx_nsk_2 <= tx_v24_2;
    tx_nsk_3 <= tx_v24_3;
    tx_nsk_4 <= tx_v24_4;
    tx_ch2 <= tx_v24_2;
    tx_ch3 <= tx_v24_3;
    tx_ch4 <= tx_v24_4;
    tx_ch5 <= tx_v24_5;
    tx_ch6 <= tx_v24_6;
    tx_ch7   <= tx_v11_1;
    tx_ch8   <= tx_v11_2;
    tx_ch9   <= tx_g703;
    tx_ch10  <= tx_enet;
    
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
rx_ch<=(rx_ch1,rx_ch2,rx_ch3,rx_ch4,rx_ch5,rx_ch6,rx_ch7,rx_ch8,rx_ch9,rx_ch10);

--------------------------------------------------------------------------------
end architecture rtl;
