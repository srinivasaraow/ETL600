--------------------------------------------------------------------------
-- $Workfile: tb_mux_v24_status_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_mux_v24_status_behavior.vhd $
-- 
-- 9     11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 7     2.12.04 11:04 Chalgyg
-- Enlarged txd_busy bus.
-- 
-- 6     23.11.04 10:54 Gygax02
-- -- no description --
-- 
-- 4     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 3     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_v24_if_p.all;

ENTITY tb_mux_v24_status IS
   PORT( 
      failure0      : IN     std_logic;
      failure1      : IN     std_logic;
      failure2      : IN     std_logic;
      failure3      : IN     std_logic;
      failure4      : IN     std_logic;
      failure5      : IN     std_logic;
      rxd_dummy     : IN     std_logic_vector (3 DOWNTO 0);
      testcase      : IN     t_testcase;
      txd_busy0     : IN     std_logic;
      txd_busy1     : IN     std_logic;
      txd_busy2     : IN     std_logic;
      txd_busy3     : IN     std_logic;
      txd_busy4     : IN     std_logic;
      txd_busy5     : IN     std_logic;
      txd_int0      : IN     std_logic;
      txd_int1      : IN     std_logic;
      txd_int2      : IN     std_logic;
      txd_int3      : IN     std_logic;
      txd_int4      : IN     std_logic;
      txd_int5      : IN     std_logic;
      rxd_dummy_int : OUT    std_logic_vector (5 DOWNTO 0);
      v24_txd_bus   : OUT    std_logic_vector (9 DOWNTO 0);
      v_24_status   : OUT    std_logic_vector (1 DOWNTO 0)
   );

-- Declarations

END tb_mux_v24_status ;

ARCHITECTURE behavior OF tb_mux_v24_status IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  signal rxd_dummy_int_0  : std_logic;
  signal rxd_dummy_int_1  : std_logic;
  signal rxd_dummy_int_4  : std_logic;
  signal rxd_dummy_int_5  : std_logic;  
  signal switch_1         : std_logic;
  signal switch_4         : std_logic;
  signal switch_5         : std_logic;
  signal switch_and       : std_logic;
  
  BEGIN
  
  v24_txd_bus     <= txd_busy5 & txd_busy4 & txd_busy3 & txd_busy2 & txd_busy1 & txd_busy0 & txd_int3 & txd_int2 & txd_int1 & txd_int0;
  v_24_status(1)  <= not(txd_busy5 or txd_busy4 or txd_busy3 or txd_busy2 or txd_busy1 or txd_busy0);
  v_24_status(0)  <= failure5 or failure4 or failure3 or failure2 or failure1 or failure0;
  switch_1        <= txd_int1 when (mode(1,testcase) = 's') else '1';
  switch_4        <= txd_int4 when (mode(4,testcase) = 's') else '1';
  switch_5        <= txd_int5 when (mode(5,testcase) = 's') else '1';
  switch_and      <= txd_int0 and switch_1 and switch_4 and switch_5; --and txd_int1 and txd_int4 and txd_int5;
  rxd_dummy_int_0 <= rxd_dummy(0) when (mode(0,testcase) /= 's') else switch_and;
  rxd_dummy_int_1 <= rxd_dummy(1) when (mode(1,testcase) /= 's') else switch_and;
  rxd_dummy_int_4 <= switch_and;
  rxd_dummy_int_5 <= switch_and;
  rxd_dummy_int   <= rxd_dummy_int_5 & rxd_dummy_int_4 & rxd_dummy(3) & rxd_dummy(2) & rxd_dummy_int_1 & rxd_dummy_int_0;
  
END ARCHITECTURE behavior;

