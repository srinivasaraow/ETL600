--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_system.test
-- $Workfile: tb_system_test.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_system_test.vhd $
-- 
-- 5     11-07-05 16:31 Chstrue
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
-- 0     2007-04-01 22:14:29 Mathias Gloor (MGL)
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

ENTITY tb_system IS
   PORT( 
      otr          : OUT    std_logic;
      otr_long     : IN     std_logic;
      leds         : IN     std_logic_vector (4 DOWNTO 0);
      testcase     : IN     t_testcase;
      tb_system_ok : OUT    std_logic;
      reset_n      : OUT    std_logic;
      clk_32M      : OUT    std_logic;
      tpads_in     : OUT    std_logic_vector (3 DOWNTO 0);
      tpads_out    : IN     std_logic_vector (7 DOWNTO 0)
   );

-- Declarations

END tb_system ;

--
--------------------------------------------------------------------------------
architecture test of tb_system is


-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal   clk   : std_logic        := '1';

--------------------------------------------------------------------------------
-- PROCEDURES AND FUNCTIONS
--------------------------------------------------------------------------------
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  testcase_proc : process
  begin
    tb_system_ok <= '1';
    otr <= '0';
    wait on testcase;
    
    -----------------------------------------------------------------------------
    -- TESTCASE for basic system initialization routines
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      tb_system_ok <= '0';
      reset_n  <= '0';
      wait for 5*SYSCLK_PRD;
      reset_n  <= '1';
      wait for 15*SYSCLK_PRD;  -- Wait for FPGA DLL locking
      printf("_0m: System initialization done");
      tb_system_ok <= '1';

    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    elsif testcase = TC_SHORT_LOOP then
      tb_system_ok <= '0';
      tpads_in <= "0010";
      wait for 30 ns; --SIGNAL_ASSIGNMENT;
      printf("_0m: FPGA on chip loop enabled (TDM-Bus-Side)");
      tb_system_ok <= '1';
      
 
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    elsif testcase = TC_LEGACY_DATA then
      tb_system_ok <= '0';
      tpads_in<="1000";
      wait for SIGNAL_ASSIGNMENT;
      printf("_0m: FPGA on chip loop enabled (OFDM-Modem-Side)");
      tb_system_ok <= '1';
      
      
    end if;

    
  end process;
  
  ------------------------------------------------------------------------------
  clock_proc : process 
  begin 
    clk<='1';
    wait for SYSCLK_PRD/2;
    clk<='0';
    wait for SYSCLK_PRD/2;
  end process;
  
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--  clk <= not clk after SYSCLK_PRD/2;  -- pay attention for sim. synchronism
  clk_32M <= clk;

--------------------------------------------------------------------------------
end architecture test;

