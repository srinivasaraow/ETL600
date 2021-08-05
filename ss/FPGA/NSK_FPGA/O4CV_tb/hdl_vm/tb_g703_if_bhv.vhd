--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_g703_if.bhv
-- $Workfile: tb_g703_if_bhv.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_g703_if_bhv.vhd $
-- 
-- 3     11-07-05 16:27 Chstrue
-- R4 implementation
-- 
-- 2     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-04 23:39:15 Mathias Gloor (MGL)
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

ENTITY tb_g703_if IS
   PORT( 
      testcase      : IN     t_testcase;
      tx_n          : IN     std_logic;
      tx_p          : IN     std_logic;
      los           : OUT    std_logic;
      rx_n          : OUT    std_logic;
      rx_p          : OUT    std_logic;
      tb_g703_if_ok : OUT    std_logic
   );

-- Declarations

END tb_g703_if ;

--
--------------------------------------------------------------------------------
architecture bhv of tb_g703_if is

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
    tb_g703_if_ok <= '1';
    los<='0';
    rx_n<='0';
    rx_p<='0';
    wait on testcase;
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      tb_g703_if_ok <= '0';
      wait for SIGNAL_ASSIGNMENT;
      --assert (test_word = X"000D")
      --  report "corrupted management interface (BRR); wrong default value"
      --  severity failure;
      printf("0m: System initialization done");
      tb_g703_if_ok <= '1';
    end if;

    
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture bhv;

