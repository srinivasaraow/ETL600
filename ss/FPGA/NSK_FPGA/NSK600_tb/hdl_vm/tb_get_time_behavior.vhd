--------------------------------------------------------------------------
-- $Workfile: tb_get_time_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_get_time_behavior.vhd $
-- 
-- 9     11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 7     8.12.04 11:01 Chalgyg
-- Don't loop data in ADPSK mode.
-- 
-- 5     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.STD_LOGIC_UNSIGNED.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
use nsk600_tb.tb_config_p.all;

ENTITY tb_get_time IS
   GENERIC(
      id         : integer range 0 to 3
   );
   PORT( 
      testcase  : IN     t_testcase;
      rxd       : IN     std_logic;
      rxc       : IN     std_logic;
      clk_1024k : IN     std_logic;
      data      : IN     std_logic_vector (7 Downto 0);
      data_rxd  : OUT    std_logic_vector (7 DOWNTO 0)
   );
END tb_get_time;

ARCHITECTURE behavior OF tb_get_time IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


	signal last	 : std_logic :='0';
  signal state	: std_logic :='0';
  signal pos	  : std_logic_vector(6 downto 0) := "0000000";

BEGIN
  sample_rxd: process
  begin
    wait until clk_1024k ='1';
    if ((loop_data_on_dsp_if(testcase) = '0') or (mode(id,testcase) = 'a')) then    -- Generate new data for RXD path.
      if ((rxc /= last) and (rxc = '0')) then
        pos    <= "1011111";
        state  <= rxd;
      else
        if ((pos > "0000000") and (pos <= "1011111")) then
          pos    <= pos-1;
        else
          pos    <= "1111111";
        end if;
      end if;
	    last	<= rxc;
    else                                             -- Loop TXD data to RXD path.
      state  <= data(7);
      pos    <= data(6 downto 0);
    end if;
	    
end process;

  give_out_sample: process
  begin
    wait until clk_1024k ='0';
    data_rxd  <= state & pos;
  end process;

END ARCHITECTURE behavior;

