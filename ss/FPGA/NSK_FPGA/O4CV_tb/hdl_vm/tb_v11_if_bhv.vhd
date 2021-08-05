--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_v11_if.bhv
-- $Workfile: tb_v11_if_bhv.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_v11_if_bhv.vhd $
-- 
-- 3     11-07-05 16:32 Chstrue
-- R4 implementation
-- 
-- 2     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-04 23:43:40 Mathias Gloor (MGL)
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

ENTITY tb_v11_if IS
   PORT( 
      i1           : IN     std_logic;
      i2           : IN     std_logic;
      r1           : IN     std_logic;
      r2           : IN     std_logic;
      s1           : IN     std_logic;
      s2           : IN     std_logic;
      testcase     : IN     t_testcase;
      c1           : OUT    std_logic;
      c2           : OUT    std_logic;
      t1           : OUT    std_logic;
      t2           : OUT    std_logic;
      tb_v11_if_ok : OUT    std_logic
   );

-- Declarations

END tb_v11_if ;

--
--------------------------------------------------------------------------------
architecture bhv of tb_v11_if is

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
    tb_v11_if_ok <= '1';
    t1<='0';
    c1<='0';
    t2<='0';
    c2<='0';
    
    wait on testcase;
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      tb_v11_if_ok <= '0';
      wait for SIGNAL_ASSIGNMENT;
      --assert (test_word = X"000D")
      --  report "corrupted management interface (BRR); wrong default value"
      --  severity failure;
      printf("_0m: System initialization done");
      tb_v11_if_ok <= '1';
    end if;

    
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture bhv;

