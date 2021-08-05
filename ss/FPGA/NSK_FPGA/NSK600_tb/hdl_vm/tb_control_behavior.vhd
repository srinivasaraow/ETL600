--------------------------------------------------------------------------
-- $Workfile: tb_control_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_control_behavior.vhd $
-- 
-- 25    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 24    19.01.06 16:16 Chalgyg
-- G.703 interface integrated.
-- 
-- 23    10.01.06 9:06 Chalgyg
-- LAN interface is now supported.
-- 
-- 22    26.04.05 16:08 Chalgyg
-- -- no description --
-- 
-- 21    26.04.05 15:23 Chalgyg
-- -- no description --
-- 
-- 20    1.03.05 10:04 Chalgyg
-- Simulation time estimation introduced.
-- 
-- 19    17.12.04 13:19 Chalgyg
-- Testcase real_out added.
-- 
-- 17    13.12.04 13:57 Chalgyg
-- Some libraries are added.
-- 
-- 15    1.12.04 14:55 Chalgyg
-- Output comments cleaned.
-- 
-- 14    22.10.04 16:24 Gygax02
-- -- no description --
-- 
-- 12    14.10.04 16:22 Gygax02
-- -- no description --
-- 
-- 11    7.10.04 17:09 Gygax02
-- -- no description --
-- 
-- 10    19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_ofdm_fpga_p.all;
USE NSK600_tb.tb_control_p.all;
USE NSK600_tb.tb_v24_if_p.all;
USE NSK600_tb.tb_real_data_p.all;
LIBRARY std;
USE std.textio.all;
LIBRARY NSK600_tb;
USE NSK600_tb.report_assist.all;

ENTITY tb_control IS
   PORT( 
      configured       : IN     std_logic;
      dsp_status       : IN     std_logic_vector (1 DOWNTO 0);
      g703_status      : IN     std_logic_vector (1 DOWNTO 0);
      lan_status       : IN     std_logic_vector (1 DOWNTO 0);
      ofdm_fpga_status : IN     std_logic_vector (1 DOWNTO 0);
      v_11_status      : IN     std_logic_vector (1 DOWNTO 0);
      v_24_status      : IN     std_logic_vector (1 DOWNTO 0);
      testcase         : OUT    t_testcase
   );

-- Declarations

END tb_control ;

ARCHITECTURE behavior OF tb_control IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  signal blocks_ready  : std_logic;
  signal block_error   : std_logic;
  
BEGIN
  blocks_ready  <= dsp_status(1) and v_24_status(1) and v_11_status(1) and lan_status(1) and g703_status(1) and ofdm_fpga_status(1);
  block_error   <= dsp_status(0) or v_24_status(0) or v_11_status(0) or lan_status(0) or g703_status(0) or ofdm_fpga_status(0);
  
  test_sequence: process
    variable i               : integer;
    variable error_found     : std_logic := '0';
    variable simulation_time : time_vector;
  begin
    estimate_time(simulation_time);
    printf("Welcome to the P4LT FPGA test");
    printf("- Total estimated simulation time is " & int2str(integer(simulation_time(simulation_time'high))) & " ms");
    printf("- Aproximate calculation time will be " & int2str(integer(simulation_time(simulation_time'high)*0.0847)) & " minutes");
    if ((simulation_time(simulation_time'high)*0.0847) < 10.0) then
      printf(LF & LF);
    elsif ((simulation_time(simulation_time'high)*0.0847) < 60.0) then
      printf("... better get a coffee now!" & LF & LF);
    elsif ((simulation_time(simulation_time'high)*0.0847) < 180.0) then
      printf("... consider power napping!" & LF & LF);
    else
      printf(" -> You can go home now... c u tomorrow!" & LF & LF);
    end if;
    for i in 0 to t_testcase'high loop
      if (do_testcase(i) = '1') then
        testcase  <= i;
        printf("---------------------------------------------------------------------------" & LF & "Entering testcase " & int2str(i));
        printf("Estimated simulation time is " & int2str(integer(simulation_time(i))) & " ms" & LF);
        wait until (blocks_ready = '1');
        if (block_error='0') then
          generate_message(i);
        else
          printf(" " & LF & "What a pity! Error in testcase " & int2str(i));
          error_found := '1';
        end if;
      end if;
    end loop;
    printf("---------------------------------------------------------------------------" & LF & " ");
    if (error_found = '0') then
      assert false
        report "CONGRATULATIONS!!! Simulation finished and no errors found!"
        severity failure;
    else
      assert false
        report "Simulation failed! :("
        severity failure;
    end if;
  end process;
    
END ARCHITECTURE behavior;

