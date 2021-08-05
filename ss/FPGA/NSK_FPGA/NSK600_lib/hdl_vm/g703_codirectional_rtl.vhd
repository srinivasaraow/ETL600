--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.g703_codirectional.rtl
-- $Workfile: g703_codirectional_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : chmaglo1.UNKNOWN (CH-W-0012670)
-- Remarks  : 
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/g703_codirectional_rtl.vhd $ 
-- 
-- 5     11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 2     3.10.06 13:24 Chmaglo1
-- -- no description --
-- 
-- 1     19.09.06 18:31 Chmaglo1
-- including v.11 and g.703; still buggy
-- 0     11.08.2006 13:13:57 Mathias Gloor (MGL)
-- File Generated 
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;
USE ieee.std_logic_arith.all;

ENTITY g703_codirectional IS
   PORT( 
      clk          : IN     std_logic;
      dcd          : IN     std_logic;
      enp_over16   : IN     std_logic;
      enp_tx_octet : IN     std_logic;
      reset_n      : IN     std_logic;
      rx_n         : IN     std_logic;
      rx_p         : IN     std_logic;
      txd          : IN     std_logic;
      enp_rx_octet : OUT    std_logic;
      rxc          : OUT    std_logic;
      rxd          : OUT    std_logic;
      tx_n         : OUT    std_logic;
      tx_p         : OUT    std_logic
   );

-- Declarations

END g703_codirectional ;

--
--------------------------------------------------------------------------------
architecture rtl of g703_codirectional is
  
-----------------------------------------------------------
-- Last edited:
--   110630: no changes

  
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal pat_tx : std_logic;
signal cnt_tx : std_logic_vector(7 downto 0); --integer range 0 to 255;
signal cnt_rx : std_logic_vector(4 downto 0); --integer range 0 to 31;
signal rxp_vec : std_logic_vector(1 downto 0);
signal rxn_vec : std_logic_vector(1 downto 0);
signal pol : std_logic;
-- 
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  process (clk, reset_n)
  begin
    if (reset_n = '0') then
      tx_p <= '1';
      tx_n <= '1';
      rxd <= '1';
      cnt_rx <= (others=>'0');
      cnt_tx <= (others=>'0');
      rxp_vec<=(others=>'0');
      rxn_vec<=(others=>'0');
      pol<='0';
      enp_rx_octet<='0';
    elsif (clk'event and clk = '1') then
      --Transmitter logic
--      if enp_tx_octet='1' and (cnt_tx/=128 and cnt_tx/=0 and cnt_tx/=112 and cnt_tx/=144 and cnt_tx/=16 and cnt_tx/=240) then -- respectively cnt_tx/=128 and cnt_tx/=127 ...
      if enp_tx_octet='1' and (cnt_tx<128 and cnt_tx/=16) then -- workaround: avoid jittered signal when value 16 
        cnt_tx<=(others=>'0');
      elsif enp_over16='1' then
        if cnt_tx>=255 then --TX: 0...255 counts 16 bits
          cnt_tx<=(others=>'0');
        else
          cnt_tx<=cnt_tx+1;
        end if;
        tx_p <= dcd or not (pat_tx and (cnt_tx(4) xor cnt_tx(7)));
        tx_n <= dcd or not (pat_tx and (not cnt_tx(4) xor cnt_tx(7)));
      end if;

      --Receiver logic
      if enp_over16='1' then        
        rxp_vec<=rxp_vec(0) & rx_p;
        rxn_vec<=rxn_vec(0) & rx_n;
        pol<=(pol or rxp_vec(1)) and not rxn_vec(1);
        if ((pol='1' and rxn_vec(1)='1') or (pol='0' and rxp_vec(1)='1')) then
          cnt_rx<=(others=>'0');
        else
          cnt_rx<=cnt_rx+1;              --RX: 0...31 counts 2 bits
        end if;
        if(cnt_rx(3 downto 0)=5) then
          rxd<=rxp_vec(1) or rxn_vec(1);
        end if;
        if cnt_rx=30 then
          enp_rx_octet<='1';
        end if;
      else
        enp_rx_octet<='0';
      end if;
      
    end if;
  end process;
  
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
pat_tx <= not cnt_tx(2) when txd='0' else
          not cnt_tx(3);
rxc <= cnt_rx(3);
-- 
--------------------------------------------------------------------------------
end architecture rtl;
