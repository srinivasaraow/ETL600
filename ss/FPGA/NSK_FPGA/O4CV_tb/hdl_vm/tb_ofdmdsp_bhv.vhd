--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_ofdmdsp.bhv
-- $Workfile: tb_ofdmdsp_bhv.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_ofdmdsp_bhv.vhd $
-- 
-- 5     11-07-05 16:30 Chstrue
-- R4 implementation
-- 
-- 4     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 3     07-05-19 19:54 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-04 23:43:19 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY O4CV_tb;
USE O4CV_tb.report_assist.all;
LIBRARY std;
USE std.textio.all;

ENTITY tb_ofdmdsp IS
   PORT( 
      clk_32M       : IN     std_logic;
      con           : IN     std_logic;
      sport0_d      : IN     std_logic;
      sport0_sclk   : IN     std_logic;
      sport2_sclk   : IN     std_logic;
      testcase      : IN     t_testcase;
      ind           : OUT    std_logic;
      sport2_d      : OUT    std_logic;
      tb_ofdmdsp_ok : OUT    std_logic
   );

-- Declarations

END tb_ofdmdsp ;

--
--------------------------------------------------------------------------------
architecture bhv of tb_ofdmdsp is

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4



--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
-- PROCEDURES AND FUNCTIONS
--------------------------------------------------------------------------------
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  process
  begin
    tb_ofdmdsp_ok <= '1';
    ind <= '0';
    sport2_d <= '0';
    wait on testcase;
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      tb_ofdmdsp_ok <= '0';
      wait for SIGNAL_ASSIGNMENT;
      --assert (test_word = X"000D")
      --  report "corrupted interface (BRR); wrong default value"
      --  severity failure;
      printf("_0m: System initialization done");
      tb_ofdmdsp_ok <= '1';
    end if;

    
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture bhv;

