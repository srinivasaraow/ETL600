--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.report_assist.arch_name
-- Project  : O4CV
-- Language : VHDL
-- Remarks  : 
-- Copyright  : ABB Switzerland Ltd. 2007
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--------------------------------------------------------------------------------
-- $Workfile: O4CV_tb_pkg.vhd $
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/O4CV_tb_pkg.vhd $ 
-- 
-- 8     11-07-05 16:03 Chstrue
-- R4 implementation
-- 
-- 7     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 6     07-10-16 9:38 Chmaglo1
-- -- no description --
-- 
-- 3     07-05-19 19:54 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-01 22:31:02 Mathias Gloor (CH-L-0012267)
-- File Generated 
--------------------------------------------------------------------------------
--
library std;
use std.textio.all;

library ieee;
use ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

-------------------------------------------------------------------------------
package report_assist IS
  
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4

  
-------------------------------------------------------------------------------
-- CONSTANT AND TYPE DECLARATIONS
-------------------------------------------------------------------------------
  constant TB_NAME : string := "O4CV_tb";
  constant PF_NAME : string := " " & TB_NAME & ": ";

  -- Speeds the Simulation up by 2^SIMULTIPLIER 
  constant SIMULTIPLIER : integer := 4; --use 0 for best reality, 4 for faster simulation

  -- Testbench Timing Parameter
  constant SYSCLK_FRQ : real      := 32.768e6;
  constant SYSCLK_PRD : time := 30.517578125 ns; --(1000000000.0/SYSCLK_FRQ); -- unit: nanoseconds
--  constant TIMEOUT    : integer := (100000000>>SIMULTIPLIER);  -- Testcase Timeout
  constant SIGNAL_ASSIGNMENT  : time   := 0 ns;
  
  constant DEF_OC_DELAY      : time := 10.0 ns; -- isolator inertial delay

  -- Type Declarations
--  type t_dcc_frame is array (0 to 3) of integer range 0 to 63;

  -- Test Vector Declarations:
  constant DEF_WF_5k6x3_00 : std_logic_vector(3*56-1 downto 0) := (others=>'1');
  constant DEF_WF_5k6x3_FF : std_logic_vector(3*56-1 downto 0) := (others=>'0');
  constant DEF_WF_5k6x3_AA : std_logic_vector(3*56-1 downto 0) := X"AAAAAAAAAAAAAA" & X"AAAAAAAAAAAAAA" & X"AAAAAAAAAAAAAA";
  constant DEF_WF_5k6x3_55 : std_logic_vector(3*56-1 downto 0) := X"55555555555555" & X"55555555555555" & X"55555555555555";
  constant DEF_WF_5k6x3_ABC: std_logic_vector(3*56-1 downto 0) := X"1BCDEFABCDEFAB" & X"2BCDEFABCDEFAB" & X"3BCDEFABCDEFAB";
  constant DEF_WF_5k6x3_RND: std_logic_vector(3*56-1 downto 0) := X"ABCDEFABCDEFAB" & X"ABCDEFABCDEFAB" & X"ABCDEFABCDEFAB";
  constant DEF_WF_5k6x3_123: std_logic_vector(3*56-1 downto 0) := X"01020304050607" & X"08090A0B0C0D0E" & X"0F101112131415";
  constant DEF_WF_5k6x3_F01: std_logic_vector(3*56-1 downto 0) := X"F011111111110F" & X"F022222222220F" & X"F033333333330F";
  
  constant DEF_WF_6k7x3_00 : std_logic_vector(3*67-1 downto 0) := (others=>'1');
  constant DEF_WF_6k7x3_FF : std_logic_vector(3*67-1 downto 0) := (others=>'0');
  constant DEF_WF_6k7x3_AA : std_logic_vector(3*67-1 downto 0) := X"AAAAAAAAAAAAAAAA"&B"101"&
                                                                  X"AAAAAAAAAAAAAAAA"&B"101"&
                                                                  X"AAAAAAAAAAAAAAAA"&B"101";
  constant DEF_WF_6k7x3_55 : std_logic_vector(3*67-1 downto 0) := X"5555555555555555"&B"010"&
                                                                  X"5555555555555555"&B"010"&
                                                                  X"5555555555555555"&B"010";
  constant DEF_WF_6k7x3_ABC: std_logic_vector(3*67-1 downto 0) := X"ABCDEFABCDEFABCD"&B"111"&
                                                                  X"ABCDEFABCDEFABCD"&B"101"&
                                                                  X"ABCDEFABCDEFABCD"&B"000";
  constant DEF_WF_6k7x3_REA: std_logic_vector(3*67-1 downto 0) := B"001"&X"0102030405060708"&
                                                                  B"010"&X"090A0B0C0D0F1011"&
                                                                  B"011"&X"1213141516171819";
  constant DEF_WF_6k7x3_RND: std_logic_vector(3*67-1 downto 0) := B"101"&X"45CAB891AFFAE2C6"&
                                                                  B"010"&X"A9B3232783C5BA00"&
                                                                  B"110"&X"BB4791B4C2ED607C";
  constant DEF_WF_6k7x3_123: std_logic_vector(3*67-1 downto 0) := '1'&X"0102030405060708090A0B0C0D0E0F10111213141516171819";
                                                                --X"45CAB891AFFAE2C6A9B3232783C5BA00BB4791B4C2ED607CF0"&'1';
--  constant DEF_WF_6k7x3_HJM : std_logic_vector(3*67-1 downto 0):= X"1122334455667788"&B"100"&
--                                                                  X"AAAAAAAAAAAAAAAA"&B"101"&
--                                                                  X"AAAAAAAAAAAAAAAA"&B"101";
-------------------------------------------------------------------------------
-- TESTCASE DECLARATIONS:
-------------------------------------------------------------------------------
  type t_testcase is    ('U',
                         TC_SYSTEM_INIT,
                         TC_CONFIG_IF,
                         TC_LEGACY_DATA,
                         TC_SHORT_LOOP,
                         TC_PAGE_INTERFERENCE_5,
                         TC_PAGE_INTERFERENCE_1,
                         TC_END
                         );

-------------------------------------------------------------------------------
-- PROCEDURE AND FUNCTION DECLARATIONS
-------------------------------------------------------------------------------
procedure printf(s : in string);
--  procedure sprintf(l : inout line; s : in string);
--  procedure lprintf(l : inout line);
--  procedure iprintf (l : inout line; i : in    integer);

--  function get_syncpat (
--    frame         : t_dcc_frame; 
--    table         : integer)
--  return std_logic_vector;
  -----------------------------------------------------------------------------
--  procedure pexit (
--    constant status : in integer;
--    constant op     : in string);

-------------------------------------------------------------------------------
END report_assist;
