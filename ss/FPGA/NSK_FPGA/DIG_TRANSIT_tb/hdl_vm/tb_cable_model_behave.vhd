--------------------------------------------------------------------------------
-- $Workfile: tb_cable_model_behave.vhd $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : 
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_cable_model_behave.vhd $
-- 
-- 4     11-07-05 17:15 Chstrue
-- -- no description --
-- 
-- 3     11-07-05 17:14 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.report_assist.all;

ENTITY tb_cable_model IS
   PORT( 
      A_in      : IN     std_logic;
      B_in      : IN     std_logic;
      clk_32M   : IN     std_logic;
      reset_n   : IN     std_logic;
      unplug    : IN     std_logic;
      A_delayed : OUT    std_logic;
      B_delayed : OUT    std_logic
   );

-- Declarations

END tb_cable_model ;

--
ARCHITECTURE behave OF tb_cable_model IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation


  signal delay_line_A : std_logic_vector(300 downto 0);
  signal delay_line_B : std_logic_vector(300 downto 0);
  
  
BEGIN
  
  A_delayed <= delay_line_A(80);
  B_delayed <= delay_line_B(80);
  
  rx_transfer: process(reset_n, clk_32M)
  begin
    if (reset_n = '0') then
      delay_line_A <= (others => '1');
      delay_line_B <= (others => '1');
    elsif (clk_32M'event and clk_32M = '1') then
      if (unplug = '0') then
        delay_line_A <= delay_line_A(299 downto 0) & A_in;
        delay_line_B <= delay_line_B(299 downto 0) & B_in;
      else
        delay_line_A <= (others => '1');
        delay_line_B <= (others => '1');
      end if;
    end if;
  end process;
  
  
END behave;

