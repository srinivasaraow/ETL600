--------------------------------------------------------------------------
-- $Workfile: tb_dummy_rxd_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_dummy_rxd_behavior.vhd $
-- 
-- 13    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 12    14.01.05 15:55 Chalgyg
-- Don't generate data if mode 'o'.
-- 
-- 10    25.11.04 11:35 Gygax02
-- Initial RXD value set to '1'.
-- 
-- 8     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.STD_LOGIC_UNSIGNED.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_real_data_p.all;

ENTITY tb_dummy_rxd IS
   PORT( 
      testcase  : IN     t_testcase;
      rxc_dummy : OUT    std_logic_vector (3 DOWNTO 0);
      rxd_dummy : OUT    std_logic_vector (3 DOWNTO 0);
      busy      : OUT    std_logic
   );

-- Declarations

END tb_dummy_rxd ;

ARCHITECTURE behavior OF tb_dummy_rxd IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4

  signal rxc_dummy_int1 : std_logic := '0';
  signal rxc_dummy_int2 : std_logic := '0';
  signal rxc_dummy_int3 : std_logic := '0';
  signal rxc_dummy_int4 : std_logic := '0';
  signal rxd_dummy_int1 : std_logic := '0';
  signal rxd_dummy_int2 : std_logic := '0';
  signal rxd_dummy_int3 : std_logic := '0';
  signal rxd_dummy_int4 : std_logic := '0';
  signal busy1          : std_logic := '0';
  signal busy2          : std_logic := '0';
  signal busy3          : std_logic := '0';
  signal busy4          : std_logic := '0';
  
BEGIN
  
  rxc_dummy  <= rxc_dummy_int4 & rxc_dummy_int3 & rxc_dummy_int2 & rxc_dummy_int1;
  rxd_dummy  <= rxd_dummy_int4 & rxd_dummy_int3 & rxd_dummy_int2 & rxd_dummy_int1;
  busy       <= busy1 or busy2 or busy3 or busy4;

  generate_rxc1: process
    
    variable ti            : time;
    variable testcase_old  : t_testcase;
     
  begin
    if (baudrate2config(baudrate_value(0,testcase)) = 0) then
      rxc_dummy_int1  <= '1';
      wait on testcase;
    else
      rxc_dummy_int1  <= not(rxc_dummy_int1);
      ti              := (52.083333 us * baudrate2config(baudrate_value(0,testcase))) / 2;
      testcase_old    := testcase;
      wait on testcase for ti;
      if (testcase /= testcase_old) then
        rxc_dummy_int1  <= '1';
        wait for 1 ns;
      end if;
    end if;
  end process;

  generate_rxc2: process
    
    variable ti            : time;
    variable testcase_old  : t_testcase;
     
  begin
    if (baudrate2config(baudrate_value(1,testcase)) = 0) then
      rxc_dummy_int2  <= '1';
      wait on testcase;
    else
      rxc_dummy_int2  <= not(rxc_dummy_int2);
      ti              := (52.083333 us * baudrate2config(baudrate_value(1,testcase))) / 2;
      testcase_old    := testcase;
      wait on testcase for ti;
      if (testcase /= testcase_old) then
        rxc_dummy_int2  <= '1';
        wait for 1 ns;
      end if;
    end if;
  end process;

  generate_rxc3: process
    
    variable ti            : time;
    variable testcase_old  : t_testcase;
     
  begin
    if (baudrate2config(baudrate_value(2,testcase)) = 0) then
      rxc_dummy_int3  <= '1';
      wait on testcase;
    else
      rxc_dummy_int3  <= not(rxc_dummy_int3);
      ti              := (52.083333 us * baudrate2config(baudrate_value(2,testcase))) / 2;
      testcase_old    := testcase;
      wait on testcase for ti;
      if (testcase /= testcase_old) then
        rxc_dummy_int3  <= '1';
        wait for 1 ns;
      end if;
    end if;
  end process;

  generate_rxc4: process

    variable ti            : time;
    variable testcase_old  : t_testcase;
  
  begin
    if (baudrate2config(baudrate_value(3,testcase)) = 0) then
      rxc_dummy_int4  <= '1';
      wait on testcase;
    else
      rxc_dummy_int4  <= not(rxc_dummy_int4);
      ti              := (52.083333 us * baudrate2config(baudrate_value(3,testcase))) / 2;
      testcase_old    := testcase;
      wait on testcase for ti;
      if (testcase /= testcase_old) then
        rxc_dummy_int4  <= '1';
        wait for 1 ns;
      end if;
    end if;
  end process;

  generate_rxd1: process
  
  variable data1  : std_logic_vector(9 downto 0) := "0000000000";
    
  begin
    if ((pattern_dsp_if(0,testcase) = "00000000") or (mode(0,testcase) = 'o')) then
      busy1           <= '0';
      rxd_dummy_int1  <= '1';
    else
      busy1  <= '1';
      data1  := '0' & pattern_dsp_if(0,testcase) & '1';  -- Startbit, 8 bit data and stopbit.
      wait for 100 us;
      for j in 1 to nr_of_repetitions_dsp(0,testcase) loop
        for i in 0 to (data1'length-1) loop
          wait until (rxc_dummy_int1='0');
          rxd_dummy_int1  <= data1((data1'length-1)-i);
        end loop;
      end loop;
      wait until (rxc_dummy_int1='0');
      rxd_dummy_int1  <= '1';
      busy1           <= '0';
    end if;
    wait on testcase;
  end process;

  generate_rxd2: process
  
  variable data2  : std_logic_vector(9 downto 0) := "0000000000";
  
  begin
    if (pattern_dsp_if(1,testcase) = "00000000") then
      busy2           <= '0';
      rxd_dummy_int2  <= '1';
    else
      busy2  <= '1';
      data2  := '0' & pattern_dsp_if(1,testcase) & '1';  -- Startbit, 8 bit data and stopbit.
      wait for 100 us;
      for j in 1 to nr_of_repetitions_dsp(1,testcase) loop
        for i in 0 to (data2'length-1) loop
          wait until (rxc_dummy_int2='0');
          rxd_dummy_int2  <= data2((data2'length-1)-i);
        end loop;
      end loop;
      wait until (rxc_dummy_int2='0');
      rxd_dummy_int2  <= '1';
      busy2           <= '0';
    end if;
    wait on testcase;
  end process;

  generate_rxd3: process
  
  variable data3  : std_logic_vector(9 downto 0) := "0000000000";
  
  begin
    if (pattern_dsp_if(2,testcase) = "00000000") then
      busy3           <= '0';
      rxd_dummy_int3  <= '1';
    else
      busy3  <= '1';
      data3  := '0' & pattern_dsp_if(2,testcase) & '1';
      wait for 100 us;
      for j in 1 to nr_of_repetitions_dsp(2,testcase) loop
        for i in 0 to (data3'length-1) loop
          wait until (rxc_dummy_int3='0');
          rxd_dummy_int3  <= data3((data3'length-1)-i);
        end loop;
      end loop;
      wait until (rxc_dummy_int3='0');
      rxd_dummy_int3  <= '1';
      busy3           <= '0';
    end if;
    wait on testcase;
  end process;

  generate_rxd4: process
  
  variable data4  : std_logic_vector(9 downto 0) := "0000000000";
  
  begin
    if (pattern_dsp_if(3,testcase) = "00000000") then
      busy4           <= '0';
      rxd_dummy_int4  <= '1';
    else
      busy4  <= '1';
      data4  := '0' & pattern_dsp_if(3,testcase) & '1';
      wait for 100 us;
      for j in 1 to nr_of_repetitions_dsp(3,testcase) loop
        for i in 0 to (data4'length-1) loop
          wait until (rxc_dummy_int4='0');
          rxd_dummy_int4  <= data4((data4'length-1)-i);
        end loop;
      end loop;
      wait until (rxc_dummy_int4='0');
      rxd_dummy_int4  <= '1';
      busy4           <= '0';
    end if;
    wait on testcase;
  end process;
  
END ARCHITECTURE behavior;

