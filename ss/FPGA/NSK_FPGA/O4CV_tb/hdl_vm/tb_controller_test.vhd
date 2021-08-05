--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_controller.test
-- $Workfile: tb_controller_test.vhd $
-- Project  : O4CV
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2007
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_controller_test.vhd $
-- 
-- 5     11-07-05 16:26 Chstrue
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
-- 0     2007-04-01 22:07:07 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
--110630
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY O4CV_tb;
USE O4CV_tb.report_assist.all;
LIBRARY std;
USE std.textio.all;

ENTITY tb_controller IS
   PORT( 
      testcase      : INOUT  t_testcase;
      tb_nskdsp_ok  : IN     std_logic;
      tb_sport13_ok : IN     std_logic;
      tb_system_ok  : IN     std_logic;
      tb_g703_if_ok : IN     std_logic;
      tb_lan_if_ok  : IN     std_logic;
      tb_ofdmdsp_ok : IN     std_logic;
      tb_v24_if_ok  : IN     std_logic;
      tb_v11_if_ok  : IN     std_logic
   );

-- Declarations

END tb_controller ;

--
--------------------------------------------------------------------------------
architecture test of tb_controller is

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal everything_ok : std_logic;
--
begin
--------------------------------------------------------------------------------
-- PROCEDURES AND FUNCTIONS
--------------------------------------------------------------------------------
-- none so far
--
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  process
  begin
    testcase<='U';
    wait until everything_ok = '1';
    
    -------------------------------------------------------------------------------
    -- insert testcase assignments for running lonely testcases
    -------------------------------------------------------------------------------
    printf("=============================================================================");
    --    printf("*************** Starting Simulation Run at Testcase %2d *********************", testcase);
    printf("*************** Starting Simulation Run *************************************");
    printf("=============================================================================");
    -------------------------------------------------------------------------------
    -- BIG LOOP runs the whole testbench
    -------------------------------------------------------------------------------

    while testcase /= t_testcase'right loop
      testcase <= t_testcase'rightof(testcase);
      wait for SIGNAL_ASSIGNMENT;              -- assign signals
      
      printf("==========================================================");

--      report "_0m: testcase nr. " & t_testcase'image(testcase) & " started.";
      printf("_0m: testcase '" & t_testcase'image(testcase) & "' started at " & time'image(now) & ".");

      if testcase /= t_testcase'right then
        wait until everything_ok = '1';
      end if;


--      printf("_0m: testcase nr. %5d started at%15.3fus"); --, testcase, $realtime/1000);
--      report("testcase nr. " & testcase);
--      & testcase & "started at" & now & "fus";
           
--           & integer'image(i)           


--      printf("_0m:testcase nr. %5d started at%15.3fus", testcase, 1);
--      יייייי#SIGNAL_ASSIGNMENT;
--      ייייייi=$time;    
--      ייייייwhile(!everything_ok) 
--      יייייי#1000 if(($time-i)>TIMEOUT) begin
--      יייייייי$display("_0m: Testcase Timeout !!! ************************************************* (simulation stop) ");
--      יייייייי$stop;
--      ייייייend

    end loop;


--    printf("**** Last Testcase successfully passed  *************************************");
    printf("=============================================================================");
--    pexit(0,"Last testcase finished !!!");

    assert false
    report "CONGRATULATIONS!!! Simulation finished! Please check output for errors."
    severity failure; 

  end process;

-------------------------------------------------------------------------------
-- CONCURRENT LOGIC
-------------------------------------------------------------------------------
everything_ok <= tb_sport13_ok and
                 tb_nskdsp_ok and
                 tb_system_ok;
-------------------------------------------------------------------------------
end architecture test;

