--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_v24_if.bhv
-- $Workfile: tb_v24_if_bhv.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_v24_if_bhv.vhd $
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
-- 0     2007-04-04 23:43:55 Mathias Gloor (MGL)
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

ENTITY tb_v24_if IS
   PORT( 
      cts1         : IN     std_logic;
      cts2         : IN     std_logic;
      cts3         : IN     std_logic;
      cts4         : IN     std_logic;
      cts5         : IN     std_logic;
      cts6         : IN     std_logic;
      dcd1         : IN     std_logic;
      dcd2         : IN     std_logic;
      dcd3         : IN     std_logic;
      dcd4         : IN     std_logic;
      dcd5         : IN     std_logic;
      dcd6         : IN     std_logic;
      rxc1         : IN     std_logic;
      rxc2         : IN     std_logic;
      rxc3         : IN     std_logic;
      rxc4         : IN     std_logic;
      rxc5         : IN     std_logic;
      rxc6         : IN     std_logic;
      rxd1         : IN     std_logic;
      rxd2         : IN     std_logic;
      rxd3         : IN     std_logic;
      rxd4         : IN     std_logic;
      rxd5         : IN     std_logic;
      rxd6         : IN     std_logic;
      testcase     : IN     t_testcase;
      rts1         : OUT    std_logic;
      rts2         : OUT    std_logic;
      rts3         : OUT    std_logic;
      rts4         : OUT    std_logic;
      rts5         : OUT    std_logic;
      rts6         : OUT    std_logic;
      tb_v24_if_ok : OUT    std_logic;
      txd1         : OUT    std_logic;
      txd2         : OUT    std_logic;
      txd3         : OUT    std_logic;
      txd4         : OUT    std_logic;
      txd5         : OUT    std_logic;
      txd6         : OUT    std_logic
   );

-- Declarations

END tb_v24_if ;

--
--------------------------------------------------------------------------------
architecture bhv of tb_v24_if is

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
    tb_v24_if_ok <= '1';
    rts1 <= '0';
    rts2 <= '0';
    rts3 <= '0';
    rts4 <= '0';
    rts5 <= '0';
    rts6 <= '0';
    txd1 <= '1';
    txd2 <= '1';
    txd3 <= '1';
    txd4 <= '1';
    txd5 <= '1';
    txd6 <= '1';
    wait on testcase;
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      tb_v24_if_ok <= '0';
      wait for SIGNAL_ASSIGNMENT;
      --assert (test_word = X"000D")
      --  report "corrupted management interface (BRR); wrong default value"
      --  severity failure;
      printf("_0m: System initialization done");
      tb_v24_if_ok <= '1';
    end if;

    
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture bhv;

