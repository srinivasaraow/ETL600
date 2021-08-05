--------------------------------------------------------------------------------
-- $Workfile: tb_dt_mainprogram_behave.vhd $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : 
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_dt_mainprogram_behave.vhd $
-- 
-- 6     12-02-07 15:38 Chstrue
-- Adaption for rel 0x41.
-- 
-- 5     11-08-29 9:48 Chstrue
-- Master timeout verification included
-- 
-- 4     11-07-06 8:49 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;

LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.tb_dt_main_pkg.all;
USE DIG_TRANSIT_tb.basic_tb_pkg.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY tb_dt_mainprogram IS
   PORT( 
      AF_model_snk_status    : IN     t_AF_model_snk_status;
      AF_model_src_status    : IN     t_AF_model_src_status;
      MPI_Ack                : IN     t_MPI_Ack;
      AF_model_snk_config    : OUT    t_AF_model_snk_config;
      AF_model_src_config    : OUT    t_AF_model_src_config;
      TDM_model_config       : OUT    t_TDM_model_config;
      MPI_Req                : OUT    t_MPI_Req;
      Resetn                 : OUT    std_logic;
      TDM_model_status       : IN     t_TDM_model_status;
      clk32                  : IN     std_logic;
      clk32_s1               : IN     std_logic;
      MPI_Req_s1             : OUT    t_MPI_Req;
      MPI_Ack_s1             : IN     t_MPI_Ack;
      AF_model_snk_config_s1 : OUT    t_AF_model_snk_config;
      AF_model_snk_status_s1 : IN     t_AF_model_snk_status;
      AF_model_src_config_s1 : OUT    t_AF_model_src_config;
      AF_model_src_status_s1 : IN     t_AF_model_src_status;
      TDM_model_config_s1    : OUT    t_TDM_model_config;
      TDM_model_status_s1    : IN     t_TDM_model_status;
      clock_speed            : OUT    integer RANGE 0 TO 2;
      unplug                 : OUT    std_logic;
      loop_AF_link           : OUT    std_logic
   );

-- Declarations

END tb_dt_mainprogram ;

--
ARCHITECTURE behave OF tb_dt_mainprogram IS

-----------------------------------------------------------
-- Last edited:
--   120207: Expected FPGA Version of DUT adapted to new version 0x41
--   110815: adaptions for changed behaviour in case of ETL link loss
--   110802: Corrections for single run pass, FPGA ver 3.D
--   110630: R4 implementation
--           page switch provocation





  signal read_data     : std_logic_vector(7 downto 0);
  signal addr          : std_logic_vector(11 downto 0);
  signal data          : std_logic_vector(7 downto 0);
  signal resetn_i      : std_logic;
  
  signal test_case     : String (1 to 46) := "idle******************************************";
  signal test_proc     : String (1 to 15) := "idle***********";
  
  signal tc_selection  : std_logic_vector(1 to 9) := "111111110";
  --                              enable testcase:    123456789
  -- TC0: FPGA revision -- this test case is always enabled
  -- TC1: 16ch voice over digital transit
  -- TC2: 3ch voice to local TDM
  -- TC3: 16ch voice, 4 to local TDM, 12 to digital transit (multiplexed)
  -- TC4: 12ch voice, 6 to local TDM, 6 to digital transit (multiplexed, same ch. nr.)
  -- TC5: 16ch voice over digital transit, slave1 faster than master
  -- TC6: 16ch voice over digital transit, master faster than slave1
  -- TC7: 16ch voice over digital transit, check of AIS
  -- TC8: LAN-only mode
  -- Expected FPGA revision: change setting on line 346 in this file
  
  -- structure of this file:
  -- procedures for accessing the DSP interface
  -- procedures for configuring the master
  -- procedures for configuring the slave1
  -- procedures for configuring the aggregate frame models for the master
  -- procedures for configuring the aggregate frame models for the slave1
  -- procedures for configuring the TDM models for the master
  -- procedures for configuring the TDM models for the slave1
  -- procedures for the transmission checks
  -- 'main' functionality
  
BEGIN
  
  resetn      <= resetn_i;

  test_sequence: process
  
  --tb specific variables:
  variable v_error_found      : std_logic := '0';
  constant TDM_data_pattern01 : std_logic_vector(0 to 66) := "1001000000000000000000000000000000000000100000000000011100000000000";
  constant TDM_data_pattern02 : std_logic_vector(0 to 66) := "1010000000000000010000000000000000000000011100000000000000000000000";
  constant TDM_data_pattern03 : std_logic_vector(0 to 66) := "1011000100000000000000001001100000100000100010000000000000000111000";
  constant TDM_data_pattern04 : std_logic_vector(0 to 66) := "1100000000000000000000000000000100000000000000000000000000000111000";
  constant TDM_data_pattern05 : std_logic_vector(0 to 66) := "1101000000000000000000010100000100000000001110000000000000000000000";
  constant TDM_data_pattern06 : std_logic_vector(0 to 66) := "1001000000010100000000100000000000010000000000001000011100000000000";
  constant TDM_data_pattern07 : std_logic_vector(0 to 66) := "1010000000000000000000000000000000000000000000000011100000000000000";
  constant TDM_data_pattern08 : std_logic_vector(0 to 66) := "1011000000000000000000000000000000000000000000000000000000000111000";
  constant TDM_data_pattern09 : std_logic_vector(0 to 66) := "1100000000000000010000000000000000000000000000000000000000000111000";
  constant TDM_data_pattern10 : std_logic_vector(0 to 66) := "1101000000000001000000000010000000000100000000000011100000000000000";
  constant TDM_data_pattern11 : std_logic_vector(0 to 66) := "1001000010000000000000000001000001000000000000000000111000000000000";
  constant TDM_data_pattern12 : std_logic_vector(0 to 66) := "1010000000100000000000000000000000000000000000000011100000010000000";
  constant TDM_data_pattern13 : std_logic_vector(0 to 66) := "1011000100000000010000000000000100000000000000000000111000000000000";
  constant TDM_data_pattern14 : std_logic_vector(0 to 66) := "1100000000000000000000000100000001000000000100000000000000000111000";
  constant TDM_data_pattern15 : std_logic_vector(0 to 66) := "1101000100000000000000000000000000000000001000000011100000000000000";
  constant TDM_data_pattern16 : std_logic_vector(0 to 66) := "1001000000010000000000000000001000000000000100000100001110000000000";
 
-----------------------------------------------------------------------------
  procedure p_MPI_rd_access (
  --Description:
  --Performs a MPI read accesss to FPGA
    signal   Address      : in    std_logic_vector(11 downto 0);
    signal   Read_data    : out   std_logic_vector(7 downto 0);
    signal   test_proc    : inout String (1 to 15)
    ) is
  begin

    MPI_Req.Req         <= '1';
    MPI_Req.Addr        <= Address;
    MPI_Req.trans       <= MPI_FPGA_read;    
    MPI_Req.Write_data  <= (others => '0');
  
    while (MPI_Ack.Ack = '0') loop
      wait for c_clock_uC_cycle; 
    end loop ;
    
    Read_data <= MPI_Ack.Read_data;
  
    MPI_Req.Req         <= '0';
    MPI_Req.trans       <= MPI_idle;
    MPI_Req.Addr        <= (others => '0');
    MPI_Req.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle; 
    
  end p_MPI_rd_access;
-----------------------------------------------------------------------------
  procedure p_MPI_rd_access_s1 (
  --Description:
  --Performs a MPI read accesss to FPGA
    signal   Address      : in    std_logic_vector(11 downto 0);
    signal   Read_data    : out   std_logic_vector(7 downto 0);
    signal   test_proc    : inout String (1 to 15)
    ) is
  begin

    MPI_Req_s1.Req         <= '1';
    MPI_Req_s1.Addr        <= Address;
    MPI_Req_s1.trans       <= MPI_FPGA_read;    
    MPI_Req_s1.Write_data  <= (others => '0');
  
    while (MPI_Ack_s1.Ack = '0') loop
      wait for c_clock_uC_cycle_s1; 
    end loop ;
    
    Read_data <= MPI_Ack_s1.Read_data;
  
    MPI_Req_s1.Req         <= '0';
    MPI_Req_s1.trans       <= MPI_idle;
    MPI_Req_s1.Addr        <= (others => '0');
    MPI_Req_s1.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle_s1; 
    
  end p_MPI_rd_access_s1;
-----------------------------------------------------------------------------
  procedure p_MPI_rd_access_s1_overspeed (
  --Description:
  --Performs a MPI read accesss to FPGA
    signal   Address      : in    std_logic_vector(11 downto 0);
    signal   Read_data    : out   std_logic_vector(7 downto 0);
    signal   test_proc    : inout String (1 to 15)
    ) is
  begin

    MPI_Req_s1.Req         <= '1';
    MPI_Req_s1.Addr        <= Address;
    MPI_Req_s1.trans       <= MPI_FPGA_read;    
    MPI_Req_s1.Write_data  <= (others => '0');
  
    while (MPI_Ack_s1.Ack = '0') loop
      wait for c_clock_uC_cycle_s1_overspeed; 
    end loop ;
    
    Read_data <= MPI_Ack_s1.Read_data;
  
    MPI_Req_s1.Req         <= '0';
    MPI_Req_s1.trans       <= MPI_idle;
    MPI_Req_s1.Addr        <= (others => '0');
    MPI_Req_s1.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle_s1_overspeed; 
    
  end p_MPI_rd_access_s1_overspeed;
  -----------------------------------------------------------------------------
  procedure p_MPI_rd_access_s1_underspeed (
  --Description:
  --Performs a MPI read accesss to FPGA
    signal   Address      : in    std_logic_vector(11 downto 0);
    signal   Read_data    : out   std_logic_vector(7 downto 0);
    signal   test_proc    : inout String (1 to 15)
    ) is
  begin

    MPI_Req_s1.Req         <= '1';
    MPI_Req_s1.Addr        <= Address;
    MPI_Req_s1.trans       <= MPI_FPGA_read;    
    MPI_Req_s1.Write_data  <= (others => '0');
  
    while (MPI_Ack_s1.Ack = '0') loop
      wait for c_clock_uC_cycle_s1_underspeed; 
    end loop ;
    
    Read_data <= MPI_Ack_s1.Read_data;
  
    MPI_Req_s1.Req         <= '0';
    MPI_Req_s1.trans       <= MPI_idle;
    MPI_Req_s1.Addr        <= (others => '0');
    MPI_Req_s1.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle_s1_underspeed; 
    
  end p_MPI_rd_access_s1_underspeed;
-----------------------------------------------------------------------------
  procedure p_MPI_wr_access (
    --Description:
    --Performs a MPI write accesss to FPGA
    signal   Address      : in      std_logic_vector(11 downto 0);
    signal   Write_data   : in      std_logic_vector(7 downto 0);
    signal   test_proc    : inout   String (1 to 15)
  ) is
  begin

    MPI_Req.Req         <= '1';
    MPI_Req.Addr        <= Address;
    MPI_Req.trans       <= MPI_FPGA_write;    
    MPI_Req.Write_data  <= Write_data;
  
    while (MPI_Ack.Ack = '0') loop
      wait for c_clock_uC_cycle; 
    end loop ;
  
    MPI_Req.Req         <= '0';
    MPI_Req.trans       <= MPI_idle;
    MPI_Req.Addr        <= (others => '0');
    MPI_Req.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle; 
  end p_MPI_wr_access;
-----------------------------------------------------------------------------
  procedure p_MPI_wr_access_s1 (
    --Description:
    --Performs a MPI write accesss to FPGA
    signal   Address      : in      std_logic_vector(11 downto 0);
    signal   Write_data   : in      std_logic_vector(7 downto 0);
    signal   test_proc    : inout   String (1 to 15)
  ) is
  begin

    MPI_Req_s1.Req         <= '1';
    MPI_Req_s1.Addr        <= Address;
    MPI_Req_s1.trans       <= MPI_FPGA_write;    
    MPI_Req_s1.Write_data  <= Write_data;

    while (MPI_Ack_s1.Ack = '0') loop
      wait for c_clock_uC_cycle_s1;
    end loop ;
  
    MPI_Req_s1.Req         <= '0';
    MPI_Req_s1.trans       <= MPI_idle;
    MPI_Req_s1.Addr        <= (others => '0');
    MPI_Req_s1.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle_s1; 
  end p_MPI_wr_access_s1;
-----------------------------------------------------------------------------
  procedure p_MPI_wr_access_s1_overspeed (
    --Description:
    --Performs a MPI write accesss to FPGA
    signal   Address      : in      std_logic_vector(11 downto 0);
    signal   Write_data   : in      std_logic_vector(7 downto 0);
    signal   test_proc    : inout   String (1 to 15)
  ) is
  begin

    MPI_Req_s1.Req         <= '1';
    MPI_Req_s1.Addr        <= Address;
    MPI_Req_s1.trans       <= MPI_FPGA_write;    
    MPI_Req_s1.Write_data  <= Write_data;

    while (MPI_Ack_s1.Ack = '0') loop
      wait for c_clock_uC_cycle_s1_overspeed;
    end loop ;
  
    MPI_Req_s1.Req         <= '0';
    MPI_Req_s1.trans       <= MPI_idle;
    MPI_Req_s1.Addr        <= (others => '0');
    MPI_Req_s1.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle_s1_overspeed;
  end p_MPI_wr_access_s1_overspeed;  
-----------------------------------------------------------------------------
  procedure p_MPI_wr_access_s1_underspeed (
    --Description:
    --Performs a MPI write accesss to FPGA
    signal   Address      : in      std_logic_vector(11 downto 0);
    signal   Write_data   : in      std_logic_vector(7 downto 0);
    signal   test_proc    : inout   String (1 to 15)
  ) is
  begin

    MPI_Req_s1.Req         <= '1';
    MPI_Req_s1.Addr        <= Address;
    MPI_Req_s1.trans       <= MPI_FPGA_write;    
    MPI_Req_s1.Write_data  <= Write_data;

    while (MPI_Ack_s1.Ack = '0') loop
      wait for c_clock_uC_cycle_s1_underspeed;
    end loop ;
  
    MPI_Req_s1.Req         <= '0';
    MPI_Req_s1.trans       <= MPI_idle;
    MPI_Req_s1.Addr        <= (others => '0');
    MPI_Req_s1.Write_data  <= (others => '0');
    
    wait for 2*c_clock_uC_cycle_s1_underspeed;
  end p_MPI_wr_access_s1_underspeed;  
-----------------------------------------------------------------------------
  procedure p_MPI_testcase_FPGA_Version(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Chip version   ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: FPGA identity");
    
    addr <= x"000";
    wait for 1*c_clock_uC_cycle;
    v_exp_value := x"41";
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle; 
    printf("    read FPGA ID (hex): " & hstr(data));
    --check if version match to testbench:
    if data /= v_exp_value then 
      v_error_found := '1';
      printf("*********************        incorrect FPGA ID for testbench" & 
              ", expected value: " & hstr(v_exp_value));
    else
      printf("    correct read value, addr: " & hstr(addr) & ": " & hstr(data));
    end if;
    
    wait for 2*c_clock_uC_cycle;
  
  end p_MPI_testcase_FPGA_Version;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC1(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 1");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
 
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC1;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC2(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 2");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 disabled");                   -- disable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-3
    addr <= VCE0;    
    data <= "00000111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16 off
    addr <= VCE1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to TDM
    addr <= VCTL0;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to TDM
    addr <= VCTL1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
   
    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
    addr <= VCTP0;    
    data <= "00100011";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
    addr <= VCTP1;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
    addr <= VCTP2;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
    addr <= VCTP3;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC3(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 3");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable voice channel 1 on aggregate link
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable voice channel 2 on aggregate link
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable voice channel 3 on aggregate link
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 5.6kbit/s");     -- enable voice channel 4 on aggregate link
    addr <= TCR_AD(3);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 5.6kbit/s");     -- enable voice channel 5 on aggregate link
    addr <= TCR_AD(4);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable voice channel 6 on aggregate link
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable voice channel 7 on aggregate link
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(7), voice channel 8 enabled with 5.6kbit/s");     -- enable voice channel 8 on aggregate link
    addr <= TCR_AD(7);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable voice channel 9 on aggregate link
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable voice channel 10 on aggregate link
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable voice channel 11 on aggregate link
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable voice channel 12 on aggregate link
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(12), voice channel 13 enabled with 5.6kbit/s");     -- enable voice channel 13 on aggregate link
    addr <= TCR_AD(12);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable voice channel 14 on aggregate link
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable voice channel 15 on aggregate link
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable voice channel 16 on aggregate link
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;    

    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels 1-8: 1 and 5 to TDM
    addr <= VCTL0;    
    data <= "11101110";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels 9-16: 9 and 13 to TDM
    addr <= VCTL1;    
    data <= "11101110";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
   
    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
    addr <= VCTP0;    
    data <= "01000010";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
    addr <= VCTP1;    
    data <= "00110000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
    addr <= VCTP2;    
    data <= "01010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
    addr <= VCTP3;    
    data <= "01110110";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP4, voice channel termination port for channels 9/10");   -- channels: 9+10
    addr <= VCTP4;    
    data <= "10011111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP5, voice channel termination port for channels 11/12");   -- channels: 11+12
    addr <= VCTP5;    
    data <= "11001010";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP6, voice channel termination port for channels 13/14");   -- channels: 13+14
    addr <= VCTP6;    
    data <= "11011011";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP7, voice channel termination port for channels 15/16");   -- channels: 15+16
    addr <= VCTP7;    
    data <= "10001110";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC4(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 4");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable voice channel 1 on aggregate link
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable voice channel 2 on aggregate link
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable voice channel 3 on aggregate link
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 5.6kbit/s");     -- enable voice channel 4 on aggregate link
    addr <= TCR_AD(3);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 5.6kbit/s");     -- enable voice channel 5 on aggregate link
    addr <= TCR_AD(4);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(5), voice channel 6 disabled");     -- enable voice channel 6 on aggregate link
    addr <= TCR_AD(5);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable voice channel 7 on aggregate link
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(7), voice channel 8 enabled with 5.6kbit/s");     -- enable voice channel 8 on aggregate link
    addr <= TCR_AD(7);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(8), voice channel 9 disabled");     -- enable voice channel 9 on aggregate link
    addr <= TCR_AD(8);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable voice channel 10 on aggregate link
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable voice channel 11 on aggregate link
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 disabled");     -- enable voice channel 12 on aggregate link
    addr <= TCR_AD(11);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(12), voice channel 13 disabled");     -- enable voice channel 13 on aggregate link
    addr <= TCR_AD(12);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable voice channel 14 on aggregate link
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(14), voice channel 15 enabled with 5.6kbit/s");     -- enable voice channel 15 on aggregate link
    addr <= TCR_AD(14);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable voice channel 16 on aggregate link
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;    

    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= x"DF";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= x"E6";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels 1-8: 1 and 5 to TDM
    addr <= VCTL0;    
    data <= x"E3";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels 9-16: 9 and 13 to TDM
    addr <= VCTL1;    
    data <= x"D0";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
   
    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
    addr <= VCTP0;    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
    addr <= VCTP1;    
    data <= x"60";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
    addr <= VCTP2;    
    data <= x"0F";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
    addr <= VCTP3;    
    data <= x"76";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP4, voice channel termination port for channels 9/10");   -- channels: 9+10
    addr <= VCTP4;    
    data <= x"70";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP5, voice channel termination port for channels 11/12");   -- channels: 11+12
    addr <= VCTP5;    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP6, voice channel termination port for channels 13/14");   -- channels: 13+14
    addr <= VCTP6;    
    data <= x"E0";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTP7, voice channel termination port for channels 15/16");   -- channels: 15+16
    addr <= VCTP7;    
    data <= x"FE";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC5(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 5");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
 
    printf("  set TCR(8), voice channel 9 enabled with 5.6kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(9), voice channel 10 enabled with 5.6kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 5.6kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(12), voice channel 13 enabled with 5.6kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(13), voice channel 14 enabled with 5.6kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(14), voice channel 15 enabled with 5.6kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 5.6kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC6(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 6");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
 
    printf("  set TCR(8), voice channel 9 enabled with 5.6kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(9), voice channel 10 enabled with 5.6kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 5.6kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(12), voice channel 13 enabled with 5.6kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(13), voice channel 14 enabled with 5.6kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(14), voice channel 15 enabled with 5.6kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 5.6kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC6;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC7(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 7");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
 
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC7;
-----------------------------------------------------------------------------
----------------------------------------------------------------------------- 
  procedure p_configure_Master_TC8( 
    --Description: 
    --initialize the necessary registers of the master NSK600 for LAN only mode 
    variable v_error_found : inout std_logic; 
    signal   test_proc     : inout String (1 to 15)     
  ) is 
  variable v_exp_value   : std_logic_vector(7 downto 0); 
  begin 
    test_proc <= "idle***********"; 
    test_proc <= "Master config  "; 
    printf("---------------------------------------------------------------------------" & LF ); 
    printf("Testcase: Master configuration Testcase 8"); 

    printf("  set PCR(10)"); 
    addr <= PCR_AD(10);     
    data <= x"81"; 
    wait for 1*c_clock_uC_cycle; 
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data)); 
    p_MPI_wr_access (addr, data, test_proc); 
    wait for 200*c_clock_uC_cycle; 

    printf("  set CCR(10)"); 
    addr <= CCR_AD(10);     
    data <= x"30"; 
    wait for 1*c_clock_uC_cycle; 
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data)); 
    p_MPI_wr_access (addr, data, test_proc); 
    wait for 200*c_clock_uC_cycle; 

    printf("  set BRR(10)"); 
    addr <= BRR_AD(10);     
    data <= x"28"; 
    wait for 1*c_clock_uC_cycle; 
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data)); 
    p_MPI_wr_access (addr, data, test_proc); 
    wait for 200*c_clock_uC_cycle; 

    printf("  set PCR(0)"); 
    addr <= PCR_AD(0);     
    data <= x"11"; 
    wait for 1*c_clock_uC_cycle; 
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data)); 
    p_MPI_wr_access (addr, data, test_proc); 
    wait for 200*c_clock_uC_cycle; 

    printf("  set CCR(0)"); 
    addr <= CCR_AD(0);     
    data <= x"10"; 
    wait for 1*c_clock_uC_cycle; 
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data)); 
    p_MPI_wr_access (addr, data, test_proc); 
    wait for 200*c_clock_uC_cycle; 

    printf("  set BRR(0)"); 
    addr <= BRR_AD(0);     
    data <= x"28"; 
    wait for 1*c_clock_uC_cycle; 
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data)); 
    p_MPI_wr_access (addr, data, test_proc); 
    wait for 200*c_clock_uC_cycle; 

  end p_configure_Master_TC8; 
----------------------------------------------------------------------------- 
-----------------------------------------------------------------------------
  procedure p_configure_Master_TC9(
    --Description:
    --initialize the necessary registers of the master NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Master config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Master configuration Testcase 9");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 3)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 3)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 3)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 3)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 3)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable TDM Port 8 (voice channel 3)
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable TDM Port 9 (voice channel 3)
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable TDM Port 10 (voice channel 3)
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable TDM Port 11 (voice channel 3)
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable TDM Port 12 (voice channel 3)
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable TDM Port 13 (voice channel 3)
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable TDM Port 14 (voice channel 3)
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable TDM Port 15 (voice channel 3)
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;








    printf("  set GDTC, general digital transit configuration register");   -- master, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-3
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16 off
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to TDM
    addr <= VCTL0;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to TDM
    addr <= VCTL1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
   
--    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
--    addr <= VCTP0;    
--    data <= "00100011";
--    wait for 1*c_clock_uC_cycle;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle;

--    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
--    addr <= VCTP1;    
--    data <= "00000001";
--    wait for 1*c_clock_uC_cycle;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle;

--    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
--    addr <= VCTP2;    
--    data <= "00000000";
--    wait for 1*c_clock_uC_cycle;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle;
--
--    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
--    addr <= VCTP3;    
--    data <= "00000000";
--    wait for 1*c_clock_uC_cycle;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle;
    
--    printf("  set BRR(0) to 320 kbit/s");
--    addr <= BRR_AD(0);    
--    data <= x"28";
--    wait for 1*c_clock_uC_cycle;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle;

    printf("  set BRR(0) to 160 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"21";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;





    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
        
    wait for 2*c_clock_uC_cycle;
  
  end p_configure_Master_TC9;
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC1(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 1");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
 
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
   
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    wait for 2*c_clock_uC_cycle_s1;
  end p_configure_Slave1_TC1;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC2(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 2");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(3), voice channel 4 disabled");                   -- disable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-3
    addr <= VCE0;    
    data <= "00000111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16 off
    addr <= VCE1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to TDM
    addr <= VCTL0;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to TDM
    addr <= VCTL1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
   
    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
    addr <= VCTP0;    
    data <= "00100011";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
    addr <= VCTP1;    
    data <= "00000001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
    addr <= VCTP2;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
    addr <= VCTP3;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    wait for 2*c_clock_uC_cycle_s1;
  end p_configure_Slave1_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC3(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 3");
    
    printf("  set TCR(0), voice channel 1 enabled with 5.6kbit/s");     -- enable voice channel 1 on aggregate link
    addr <= TCR_AD(0);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(1), voice channel 2 enabled with 5.6kbit/s");     -- enable voice channel 2 on aggregate link
    addr <= TCR_AD(1);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable voice channel 3 on aggregate link
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable voice channel 4 on aggregate link
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(4), voice channel 5 enabled with 5.6kbit/s");     -- enable voice channel 5 on aggregate link
    addr <= TCR_AD(4);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable voice channel 6 on aggregate link
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable voice channel 7 on aggregate link
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable voice channel 8 on aggregate link
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable voice channel 9 on aggregate link
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable voice channel 10 on aggregate link
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable voice channel 11 on aggregate link
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable voice channel 12 on aggregate link
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable voice channel 13 on aggregate link
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable voice channel 14 on aggregate link
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable voice channel 15 on aggregate link
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(15), voice channel 16 enabled with 5.6kbit/s");     -- enable voice channel 16 on aggregate link
    addr <= TCR_AD(15);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;    

    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-4
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL0, voice channel termination location");   -- channels 1-8: 1 and 6 to TDM
    addr <= VCTL0;    
    data <= "11011110";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL1, voice channel termination location");   -- channels 9-16: 9 and 15 to TDM
    addr <= VCTL1;    
    data <= "10111110";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
   
    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
    addr <= VCTP0;    
    data <= "00110100";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
    addr <= VCTP1;    
    data <= "01100101";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
    addr <= VCTP2;    
    data <= "10000111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
    addr <= VCTP3;    
    data <= "00100001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP4, voice channel termination port for channels 9/10");   -- channels: 9+10
    addr <= VCTP4;    
    data <= "10010000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set VCTP5, voice channel termination port for channels 11/12");   -- channels: 11+12
    addr <= VCTP5;    
    data <= "11111110";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set VCTP6, voice channel termination port for channels 13/14");   -- channels: 13+14
    addr <= VCTP6;    
    data <= "10111101";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set VCTP7, voice channel termination port for channels 15/16");   -- channels: 15+16
    addr <= VCTP7;    
    data <= "10101100";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    wait for 2*c_clock_uC_cycle_s1;
  end p_configure_Slave1_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC4(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 4");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable voice channel 1 on aggregate link
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(1), voice channel 2 disabled");     -- enable voice channel 2 on aggregate link
    addr <= TCR_AD(1);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable voice channel 3 on aggregate link
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(3), voice channel 4 enabled with 5.6kbit/s");     -- enable voice channel 4 on aggregate link
    addr <= TCR_AD(3);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(4), voice channel 5 enabled with 5.6kbit/s");     -- enable voice channel 5 on aggregate link
    addr <= TCR_AD(4);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(5), voice channel 6 disabled");     -- enable voice channel 6 on aggregate link
    addr <= TCR_AD(5);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable voice channel 7 on aggregate link
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(7), voice channel 8 enabled with 5.6kbit/s");     -- enable voice channel 8 on aggregate link
    addr <= TCR_AD(7);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable voice channel 9 on aggregate link
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable voice channel 10 on aggregate link
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable voice channel 11 on aggregate link
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(11), voice channel 12 disabled");     -- enable voice channel 12 on aggregate link
    addr <= TCR_AD(11);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(12), voice channel 13 enabled with 5.6kbit/s");     -- enable voice channel 13 on aggregate link
    addr <= TCR_AD(12);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable voice channel 14 on aggregate link
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable voice channel 15 on aggregate link
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(15), voice channel 16 disabled");     -- enable voice channel 16 on aggregate link
    addr <= TCR_AD(15);    
    data <= x"00";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;    

    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-4
    addr <= VCE0;    
    data <= x"DD";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= x"77";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL0, voice channel termination location");   -- channels 1-8: 1 and 6 to TDM
    addr <= VCTL0;    
    data <= x"74";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL1, voice channel termination location");   -- channels 9-16: 9 and 15 to TDM
    addr <= VCTL1;    
    data <= x"D2";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
   
    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
    addr <= VCTP0;    
    data <= x"00";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
    addr <= VCTP1;    
    data <= x"60";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
    addr <= VCTP2;    
    data <= x"07";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
    addr <= VCTP3;    
    data <= x"F6";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTP4, voice channel termination port for channels 9/10");   -- channels: 9+10
    addr <= VCTP4;    
    data <= x"F7";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set VCTP5, voice channel termination port for channels 11/12");   -- channels: 11+12
    addr <= VCTP5;    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set VCTP6, voice channel termination port for channels 13/14");   -- channels: 13+14
    addr <= VCTP6;    
    data <= x"EE";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set VCTP7, voice channel termination port for channels 15/16");   -- channels: 15+16
    addr <= VCTP7;    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    wait for 2*c_clock_uC_cycle_s1;
  end p_configure_Slave1_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC5(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 5");
    
    wait until clk32_s1 = '1';
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
 
    printf("  set TCR(8), voice channel 9 enabled with 5.6kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(9), voice channel 10 enabled with 5.6kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(11), voice channel 12 enabled with 5.6kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(12), voice channel 13 enabled with 5.6kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(13), voice channel 14 enabled with 5.6kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(14), voice channel 15 enabled with 5.6kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set TCR(15), voice channel 16 enabled with 5.6kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
        
    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
   
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_overspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_overspeed;
        
    wait for 2*c_clock_uC_cycle_s1_overspeed;
  end p_configure_Slave1_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC6(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 6");
    
    wait until clk32_s1 = '1';
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
 
    printf("  set TCR(8), voice channel 9 enabled with 5.6kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(9), voice channel 10 enabled with 5.6kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(10), voice channel 11 enabled with 5.6kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(11), voice channel 12 enabled with 5.6kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(12), voice channel 13 enabled with 5.6kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(13), voice channel 14 enabled with 5.6kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(14), voice channel 15 enabled with 5.6kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set TCR(15), voice channel 16 enabled with 5.6kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT56;
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
        
    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
   
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1_underspeed (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1_underspeed;
        
    wait for 2*c_clock_uC_cycle_s1_underspeed;
  end p_configure_Slave1_TC6;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC7(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 7");
    
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 4)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 5)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 6)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 7)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 8)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
 
    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable TDM Port 8 (voice channel 9)
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable TDM Port 9 (voice channel 10)
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable TDM Port 10 (voice channel 11)
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable TDM Port 11 (voice channel 12)
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable TDM Port 12 (voice channel 13)
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable TDM Port 13 (voice channel 14)
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable TDM Port 14 (voice channel 15)
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable TDM Port 15 (voice channel 16)
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-8
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to DT
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to DT
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
   
    printf("  set BRR(0) to 320 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"28";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    wait for 2*c_clock_uC_cycle_s1;
  end p_configure_Slave1_TC7;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_Slave1_TC9(
    --Description:
    --initialize the necessary registers of the slave1 NSK600
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "Slave1 config  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Slave configuration Testcase 9");
        
    printf("  set TCR(0), voice channel 1 enabled with 6.7kbit/s");     -- enable TDM Port 0 (voice channel 1)
    addr <= TCR_AD(0);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(1), voice channel 2 enabled with 6.7kbit/s");     -- enable TDM Port 1 (voice channel 2)
    addr <= TCR_AD(1);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    
    printf("  set TCR(2), voice channel 3 enabled with 6.7kbit/s");     -- enable TDM Port 2 (voice channel 3)
    addr <= TCR_AD(2);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(3), voice channel 4 enabled with 6.7kbit/s");     -- enable TDM Port 3 (voice channel 3)
    addr <= TCR_AD(3);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(4), voice channel 5 enabled with 6.7kbit/s");     -- enable TDM Port 4 (voice channel 3)
    addr <= TCR_AD(4);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(5), voice channel 6 enabled with 6.7kbit/s");     -- enable TDM Port 5 (voice channel 3)
    addr <= TCR_AD(5);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(6), voice channel 7 enabled with 6.7kbit/s");     -- enable TDM Port 6 (voice channel 3)
    addr <= TCR_AD(6);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(7), voice channel 8 enabled with 6.7kbit/s");     -- enable TDM Port 7 (voice channel 3)
    addr <= TCR_AD(7);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(8), voice channel 9 enabled with 6.7kbit/s");     -- enable TDM Port 8 (voice channel 3)
    addr <= TCR_AD(8);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(9), voice channel 10 enabled with 6.7kbit/s");     -- enable TDM Port 9 (voice channel 3)
    addr <= TCR_AD(9);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(10), voice channel 11 enabled with 6.7kbit/s");     -- enable TDM Port 10 (voice channel 3)
    addr <= TCR_AD(10);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(11), voice channel 12 enabled with 6.7kbit/s");     -- enable TDM Port 11 (voice channel 3)
    addr <= TCR_AD(11);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(12), voice channel 13 enabled with 6.7kbit/s");     -- enable TDM Port 12 (voice channel 3)
    addr <= TCR_AD(12);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(13), voice channel 14 enabled with 6.7kbit/s");     -- enable TDM Port 13 (voice channel 3)
    addr <= TCR_AD(13);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(14), voice channel 15 enabled with 6.7kbit/s");     -- enable TDM Port 14 (voice channel 3)
    addr <= TCR_AD(14);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("  set TCR(15), voice channel 16 enabled with 6.7kbit/s");     -- enable TDM Port 15 (voice channel 3)
    addr <= TCR_AD(15);    
    data <= BRT67;
    wait for 1*c_clock_uC_cycle;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;















    printf("  set GDTC, general digital transit configuration register");   -- slave, start counting of all counters, loop disabled
    addr <= GDTC;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE0, voice channel enable");                 -- channels: 1-3
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCE1, voice channel enable");                 -- channels: 9-16 off
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL0, voice channel termination location");   -- channels: 1-8 to TDM
    addr <= VCTL0;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set VCTL1, voice channel termination location");   -- channels: 9-16 to TDM
    addr <= VCTL1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
   
--    printf("  set VCTP0, voice channel termination port for channels 1/2");   -- channels: 1+2
--    addr <= VCTP0;    
--    data <= "00100011";
--    wait for 1*c_clock_uC_cycle_s1;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access_s1 (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle_s1;
--
--    printf("  set VCTP1, voice channel termination port for channels 3/4");   -- channels: 3+4
--    addr <= VCTP1;    
--    data <= "00000001";
--    wait for 1*c_clock_uC_cycle_s1;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access_s1 (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle_s1;
--
--    printf("  set VCTP2, voice channel termination port for channels 5/6");   -- channels: 5+6
--    addr <= VCTP2;    
--    data <= "00000000";
--    wait for 1*c_clock_uC_cycle_s1;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access_s1 (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle_s1;
--
--    printf("  set VCTP3, voice channel termination port for channels 7/8");   -- channels: 7+8
--    addr <= VCTP3;    
--    data <= "00000000";
--    wait for 1*c_clock_uC_cycle_s1;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access_s1 (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle_s1;
        
--    printf("  set BRR(0) to 320 kbit/s");
--    addr <= BRR_AD(0);    
--    data <= x"28";
--    wait for 1*c_clock_uC_cycle_s1;
--    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
--    p_MPI_wr_access_s1 (addr, data, test_proc);
--    wait for 200*c_clock_uC_cycle_s1;

    printf("  set BRR(0) to 160 kbit/s");
    addr <= BRR_AD(0);    
    data <= x"21";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;









    printf("  set BRR(7), V.11 to 9.6 kbit/s");
    addr <= BRR_AD(7);    
    data <= x"09";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    printf("  set CCR(0) to ");
    addr <= CCR_AD(0);    
    data <= x"10";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    printf("  set CCR(7) to ");
    addr <= CCR_AD(7);    
    data <= "10010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(7) to ");
    addr <= PCR_AD(7);    
    data <= "00010001";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
  
    printf("  set PCR(0) to ");
    addr <= PCR_AD(0);    
    data <= x"01";
    wait for 1*c_clock_uC_cycle_s1;
    printf("      write to register address: " & hstr(addr) & ", value: " & hstr(data));
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
        
    wait for 2*c_clock_uC_cycle_s1;
  end p_configure_Slave1_TC9;
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC1(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC1: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100001";        --                                fix pattern of data (A = Master, 1 = TC 1)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC1;
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC2(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC2: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1110000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100010";        --                                fix pattern of data (A = Master, 2 = TC 2)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1110000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC3(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC3: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1110011011010111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100011";        --                                fix pattern of data (A = Master, 3 = TC 3)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1110011011010111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC4(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC4: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111101101100111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1110001011001101";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100100";        --                                fix pattern of data (A = Master, 4 = TC 4)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1111101101100111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1110001011001101";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC5(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC5: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100101";        --                                fix pattern of data (A = Master, 5 = TC 5)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC6(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC6: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100110";        --                                fix pattern of data (A = Master, 6 = TC 6)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC6;
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_TC7(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for master in TC7: ");
    
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100111";        --                                fix pattern of data (A = Master, 7 = TC 7)
    AF_model_src_config.enable          <= '1';
    AF_model_snk_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_AF_model_TC7;
-----------------------------------------------------------------------------
----------------------------------------------------------------------------- 
  procedure p_configure_AF_model_TC8( 
    --Description: 
    --checks if the FPGA version matches the testbench version 
    variable v_error_found : inout std_logic; 
    signal   test_proc     : inout String (1 to 15)     
  ) is 
  variable v_exp_value   : std_logic_vector(7 downto 0); 
  begin 
    test_proc <= "idle***********"; 
    test_proc <= "AF Model config"; 
    printf("---------------------------------------------------------------------------" & LF ); 
    printf("Testcase: Aggregate frame model configuration for master in TC8: "); 
    
    AF_model_src_config.ser_length      <= 3187;                -- integer range 0 to 3199;  -- length of serial digital data 
    AF_model_src_config.voice_ch_en     <= "0000000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable 
    AF_model_src_config.voice_ch_rate   <= "0000000000000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k) 
    AF_model_src_config.pattern         <= "10101000";        --                                fix pattern of data (A = Master, 8 = TC 8)
    AF_model_src_config.enable          <= '1'; 
    AF_model_snk_config.ser_length      <= 3187;                -- integer range 0 to 3199;  -- length of serial digital data 
    AF_model_snk_config.voice_ch_en     <= "0000000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable 
    AF_model_snk_config.voice_ch_rate   <= "0000000000000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k) 
    AF_model_snk_config.enable          <= '1'; 
    
    wait for 200*c_clock_uC_cycle; 
    printf("done"); 
  
  end p_configure_AF_model_TC8; 
----------------------------------------------------------------------------- 
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC1(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC1: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110001";        --                                fix pattern of data (B = Slave1, 1 = TC 1)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_AF_model_s1_TC1;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC2(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC2: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1110000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110010";        --                                fix pattern of data (B = Slave1, 2 = TC 2)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1110000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_AF_model_s1_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC3(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC3: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "0011011111111110";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110011";        --                                fix pattern of data (B = Slave1, 3 = TC 3)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "0011011111111110";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_AF_model_s1_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC4(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC4: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1011101111101110";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1010011011010110";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110100";        --                                fix pattern of data (B = Slave1, 4 = TC 4)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1011101111101110";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1010011011010110";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_AF_model_s1_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC5(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC5: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110101";        --                                fix pattern of data (B = Slave1, 5 = TC 5)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1_overspeed;
    printf("done");
  
  end p_configure_AF_model_s1_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC6(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC6: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110110";        --                                fix pattern of data (B = Slave1, 6 = TC 6)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1111111100000000";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1_underspeed;
    printf("done");
  
  end p_configure_AF_model_s1_TC6;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_AF_model_s1_TC7(
    --Description:
    --checks if the FPGA version matches the testbench version
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "AF Model config";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: Aggregate frame model configuration for slave1 in TC7: ");
    
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110111";        --                                fix pattern of data (B = Slave1, 7 = TC 7)
    AF_model_src_config_s1.enable          <= '1';
    AF_model_snk_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_snk_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_snk_config_s1.enable          <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_AF_model_s1_TC7;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC1(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC1: ");
    
    TDM_model_config.voice_ch_en              <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100001";        -- pattern E1, E = to master, TC1
    TDM_model_config.voice_data_src(0)        <= TDM_data_pattern16;
    TDM_model_config.voice_data_src(1)        <= TDM_data_pattern15;
    TDM_model_config.voice_data_src(2)        <= TDM_data_pattern14;
    TDM_model_config.voice_data_src(3)        <= TDM_data_pattern13;
    TDM_model_config.voice_data_src(4)        <= TDM_data_pattern12;
    TDM_model_config.voice_data_src(5 to 15)  <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC1;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC2(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC2: ");
    
    TDM_model_config.voice_ch_en              <= "0111000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100010";        -- pattern E2, E = to master, TC2
    TDM_model_config.voice_data_src(0)        <= TDM_data_pattern01;
    TDM_model_config.voice_data_src(1)        <= TDM_data_pattern02;
    TDM_model_config.voice_data_src(2)        <= TDM_data_pattern03;
    TDM_model_config.voice_data_src(3)        <= TDM_data_pattern04;
    TDM_model_config.voice_data_src(4 to 15)  <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC3(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC3: ");
    
    TDM_model_config.voice_ch_en              <= "0110000000010001";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1011111111101111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100011";        -- pattern E3, E = to master, TC3
    TDM_model_config.voice_data_src(0)        <= TDM_data_pattern08;
    TDM_model_config.voice_data_src(1)        <= TDM_data_pattern15;
    TDM_model_config.voice_data_src(2)        <= TDM_data_pattern16;
    TDM_model_config.voice_data_src(3 to 10)  <= (others => (others => '1'));
    TDM_model_config.voice_data_src(11)       <= TDM_data_pattern13;
    TDM_model_config.voice_data_src(12 to 14) <= (others => (others => '1'));
    TDM_model_config.voice_data_src(15)       <= TDM_data_pattern14;

    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC4(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC4: ");
    
    TDM_model_config.voice_ch_en              <= "1100001100000011";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1010100101010110";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100100";        -- pattern E4, E = to master, TC4
    TDM_model_config.voice_data_src(0)        <= TDM_data_pattern10;
    TDM_model_config.voice_data_src(1)        <= TDM_data_pattern14;
    TDM_model_config.voice_data_src(2 to 5)   <= (others => (others => '1'));
    TDM_model_config.voice_data_src(6)        <= TDM_data_pattern11;
    TDM_model_config.voice_data_src(7)        <= TDM_data_pattern13;
    TDM_model_config.voice_data_src(8 to 13)  <= (others => (others => '1'));
    TDM_model_config.voice_data_src(14)       <= TDM_data_pattern15;
    TDM_model_config.voice_data_src(15)       <= TDM_data_pattern12;

    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC5(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC5: ");
    
    TDM_model_config.voice_ch_en              <= "0000000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100101";        -- pattern E5, E = to master, TC5
    TDM_model_config.voice_data_src(0 to 15)  <= (others => (others => '1'));

    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC6(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC6: ");
    
    TDM_model_config.voice_ch_en              <= "0000000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100110";        -- pattern E6, E = to master, TC6
    TDM_model_config.voice_data_src(0 to 15)  <= (others => (others => '1'));

    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC6;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC7(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC7: ");
    
    TDM_model_config.voice_ch_en              <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11100111";        -- pattern E7, E = to master, TC7
    TDM_model_config.voice_data_src(0)        <= TDM_data_pattern01;
    TDM_model_config.voice_data_src(1)        <= TDM_data_pattern02;
    TDM_model_config.voice_data_src(2)        <= TDM_data_pattern03;
    TDM_model_config.voice_data_src(3)        <= TDM_data_pattern04;
    TDM_model_config.voice_data_src(4)        <= TDM_data_pattern05;
    TDM_model_config.voice_data_src(5)        <= TDM_data_pattern06;
    TDM_model_config.voice_data_src(6)        <= TDM_data_pattern07;
    TDM_model_config.voice_data_src(7)        <= TDM_data_pattern08;
    TDM_model_config.voice_data_src(8)        <= TDM_data_pattern09;
    TDM_model_config.voice_data_src(9)        <= TDM_data_pattern10;
    TDM_model_config.voice_data_src(10)       <= TDM_data_pattern11;
    TDM_model_config.voice_data_src(11)       <= TDM_data_pattern12;
    TDM_model_config.voice_data_src(12)       <= TDM_data_pattern13;
    TDM_model_config.voice_data_src(13)       <= TDM_data_pattern14;
    TDM_model_config.voice_data_src(14)       <= TDM_data_pattern15;
    TDM_model_config.voice_data_src(15)       <= TDM_data_pattern16;
    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC7;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_TC9(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for master configuration in TC9: ");
    
    TDM_model_config.voice_ch_en              <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.pattern                  <= "11101001";        -- pattern E9, E = to master, TC9
    TDM_model_config.voice_data_src(0)        <= TDM_data_pattern01;
    TDM_model_config.voice_data_src(1)        <= TDM_data_pattern02;
    TDM_model_config.voice_data_src(2)        <= TDM_data_pattern03;
    TDM_model_config.voice_data_src(3)        <= TDM_data_pattern04;
    TDM_model_config.voice_data_src(4)        <= TDM_data_pattern05;
    TDM_model_config.voice_data_src(5)        <= TDM_data_pattern06;
    TDM_model_config.voice_data_src(6)        <= TDM_data_pattern07;
    TDM_model_config.voice_data_src(7)        <= TDM_data_pattern08;
    TDM_model_config.voice_data_src(8)        <= TDM_data_pattern09;
    TDM_model_config.voice_data_src(9)        <= TDM_data_pattern10;
    TDM_model_config.voice_data_src(10)       <= TDM_data_pattern11;
    TDM_model_config.voice_data_src(11)       <= TDM_data_pattern12;
    TDM_model_config.voice_data_src(12)       <= TDM_data_pattern13;
    TDM_model_config.voice_data_src(13)       <= TDM_data_pattern14;
    TDM_model_config.voice_data_src(14)       <= TDM_data_pattern15;
    TDM_model_config.voice_data_src(15)       <= TDM_data_pattern16;


--    TDM_model_config.voice_data_src(4 to 15)  <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle;
    TDM_model_config.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle;
    printf("done");
  
  end p_configure_TDM_model_TC9;
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC1(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC1: ");
    
    TDM_model_config_s1.voice_ch_en              <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110001";        -- pattern F1, F = to slave1, TC1
    TDM_model_config_s1.voice_data_src(0)        <= TDM_data_pattern10;
    TDM_model_config_s1.voice_data_src(1)        <= TDM_data_pattern11;
    TDM_model_config_s1.voice_data_src(2)        <= TDM_data_pattern12;
    TDM_model_config_s1.voice_data_src(3)        <= TDM_data_pattern13;
    TDM_model_config_s1.voice_data_src(4)        <= TDM_data_pattern14;
    TDM_model_config_s1.voice_data_src(5 to 15)  <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle_s1;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_TDM_model_s1_TC1;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC2(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC2: ");
    
    TDM_model_config_s1.voice_ch_en              <= "0111000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110010";        -- pattern F2, F = to slave1, TC2
    TDM_model_config_s1.voice_data_src(0)        <= TDM_data_pattern05;
    TDM_model_config_s1.voice_data_src(1)        <= TDM_data_pattern06;
    TDM_model_config_s1.voice_data_src(2)        <= TDM_data_pattern07;
    TDM_model_config_s1.voice_data_src(3)        <= TDM_data_pattern08;
    TDM_model_config_s1.voice_data_src(4 to 15)  <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle_s1;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_TDM_model_s1_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC3(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC3: ");
    
    TDM_model_config_s1.voice_ch_en              <= "1000100010001000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111011111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110011";        -- pattern F3, F = to slave1, TC3
    TDM_model_config_s1.voice_data_src(0)        <= TDM_data_pattern12;
    TDM_model_config_s1.voice_data_src(1 to 3)   <= (others => (others => '1'));
    TDM_model_config_s1.voice_data_src(4)        <= TDM_data_pattern11;
    TDM_model_config_s1.voice_data_src(5 to 7)   <= (others => (others => '1'));
    TDM_model_config_s1.voice_data_src(8)        <= TDM_data_pattern10;
    TDM_model_config_s1.voice_data_src(9 to 11)  <= (others => (others => '1'));
    TDM_model_config_s1.voice_data_src(12)       <= TDM_data_pattern09;
    TDM_model_config_s1.voice_data_src(13 to 15) <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle_s1;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_TDM_model_s1_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC4(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC4: ");
    
    TDM_model_config_s1.voice_ch_en              <= "1100001100000011";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1010100101010110";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110100";        -- pattern F4, F = to slave1, TC4
    TDM_model_config_s1.voice_data_src(0)        <= TDM_data_pattern01;
    TDM_model_config_s1.voice_data_src(1)        <= TDM_data_pattern05;
    TDM_model_config_s1.voice_data_src(2 to 5)   <= (others => (others => '1'));
    TDM_model_config_s1.voice_data_src(6)        <= TDM_data_pattern02;
    TDM_model_config_s1.voice_data_src(7)        <= TDM_data_pattern04;
    TDM_model_config_s1.voice_data_src(8 to 13)  <= (others => (others => '1'));
    TDM_model_config_s1.voice_data_src(14)       <= TDM_data_pattern06;
    TDM_model_config_s1.voice_data_src(15)       <= TDM_data_pattern03;

    wait for 2*c_clock_uC_cycle_s1;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_TDM_model_s1_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC5(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC5: ");
    
    TDM_model_config_s1.voice_ch_en              <= "0000000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110101";        -- pattern F5, F = to slave1, TC5
    TDM_model_config_s1.voice_data_src(0 to 15)  <= (others => (others => '1'));

    wait for 2*c_clock_uC_cycle_s1_overspeed;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1_overspeed;
    printf("done");
  
  end p_configure_TDM_model_s1_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC6(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC6: ");
    
    TDM_model_config_s1.voice_ch_en              <= "0000000000000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110110";        -- pattern F6, F = to slave1, TC6
    TDM_model_config_s1.voice_data_src(0 to 15)  <= (others => (others => '1'));

    wait for 2*c_clock_uC_cycle_s1_underspeed;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1_underspeed;
    printf("done");
  
  end p_configure_TDM_model_s1_TC6;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC7(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC7: ");
    
    TDM_model_config_s1.voice_ch_en              <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11110111";        -- pattern F7, F = to slave1, TC7
    TDM_model_config_s1.voice_data_src(0)        <= TDM_data_pattern16;
    TDM_model_config_s1.voice_data_src(1)        <= TDM_data_pattern15;
    TDM_model_config_s1.voice_data_src(2)        <= TDM_data_pattern14;
    TDM_model_config_s1.voice_data_src(3)        <= TDM_data_pattern13;
    TDM_model_config_s1.voice_data_src(4)        <= TDM_data_pattern12;
    TDM_model_config_s1.voice_data_src(5)        <= TDM_data_pattern11;
    TDM_model_config_s1.voice_data_src(6)        <= TDM_data_pattern10;
    TDM_model_config_s1.voice_data_src(7)        <= TDM_data_pattern09;
    TDM_model_config_s1.voice_data_src(8)        <= TDM_data_pattern08;
    TDM_model_config_s1.voice_data_src(9)        <= TDM_data_pattern07;
    TDM_model_config_s1.voice_data_src(10)       <= TDM_data_pattern06;
    TDM_model_config_s1.voice_data_src(11)       <= TDM_data_pattern05;
    TDM_model_config_s1.voice_data_src(12)       <= TDM_data_pattern04;
    TDM_model_config_s1.voice_data_src(13)       <= TDM_data_pattern03;
    TDM_model_config_s1.voice_data_src(14)       <= TDM_data_pattern02;
    TDM_model_config_s1.voice_data_src(15)       <= TDM_data_pattern01;
    wait for 2*c_clock_uC_cycle_s1;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_TDM_model_s1_TC7;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_configure_TDM_model_s1_TC9(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value   : std_logic_vector(7 downto 0);
  begin
    test_proc <= "idle***********";
    test_proc <= "TDM config     ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase: TDM model for slave1 configuration in TC9: ");
    
    TDM_model_config_s1.voice_ch_en              <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    TDM_model_config_s1.voice_ch_rate            <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.pattern                  <= "11111001";        -- pattern F9, F = to slave1, TC9
    TDM_model_config_s1.voice_data_src(0)        <= TDM_data_pattern05;
    TDM_model_config_s1.voice_data_src(1)        <= TDM_data_pattern06;
    TDM_model_config_s1.voice_data_src(2)        <= TDM_data_pattern07;
    TDM_model_config_s1.voice_data_src(3)        <= TDM_data_pattern08;
    TDM_model_config_s1.voice_data_src(4)        <= TDM_data_pattern09;
    TDM_model_config_s1.voice_data_src(5)        <= TDM_data_pattern10;
    TDM_model_config_s1.voice_data_src(6)        <= TDM_data_pattern11;
    TDM_model_config_s1.voice_data_src(7)        <= TDM_data_pattern12;
    TDM_model_config_s1.voice_data_src(8)        <= TDM_data_pattern13;
    TDM_model_config_s1.voice_data_src(9)        <= TDM_data_pattern14;
    TDM_model_config_s1.voice_data_src(10)        <= TDM_data_pattern15;
    TDM_model_config_s1.voice_data_src(11)        <= TDM_data_pattern16;
    TDM_model_config_s1.voice_data_src(12)        <= TDM_data_pattern01;
    TDM_model_config_s1.voice_data_src(13)        <= TDM_data_pattern02;
    TDM_model_config_s1.voice_data_src(14)        <= TDM_data_pattern03;
    TDM_model_config_s1.voice_data_src(15)        <= TDM_data_pattern04;







--    TDM_model_config_s1.voice_data_src(4 to 15)  <= (others => (others => '1'));
    wait for 2*c_clock_uC_cycle_s1;
    TDM_model_config_s1.enable                   <= '1';
    
    wait for 200*c_clock_uC_cycle_s1;
    printf("done");
  
  end p_configure_TDM_model_s1_TC9;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC1(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value            : std_logic_vector(7 downto 0);
  variable v_stored_data_2slave   : t_TDM_model_voice_data;
  variable v_stored_data_2master  : t_TDM_model_voice_data;
  
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC1: check transmitted data on aggregate link");
    
    -- save generated data on AF to slave
    wait for 5 ms;

    -- save generated data on AF to master
    wait until AF_model_src_status.frame_finished = '1';       -- ca. 20.18ms
    v_stored_data_2master := AF_model_src_status.sent_voice_data;

    wait until AF_model_src_status_s1.frame_finished = '1';    -- ca. 20.35ms
    v_stored_data_2slave := AF_model_src_status_s1.sent_voice_data;
    
    -- check: generated AF to slave - over DT to master - sinked AF from master
    wait for 25 ms;
    wait until AF_model_snk_status.frame_captured = '1';      -- ca. 50.168ms
    if AF_model_snk_status.rec_voice_data = v_stored_data_2slave then
      printf("Congrats! Correct data from AF slave received at AF master (sent over DT)");
    else
      v_error_found := '1';
      report "data arrived at AF_sink not equal to data sent by AF_model"
      severity warning;
    end if;      

    -- check: generated AF to master - over DT to slave - sinked AF from slave
    wait until AF_model_snk_status_s1.frame_captured = '1';   -- ca. 50.34ms
    if AF_model_snk_status_s1.rec_voice_data = v_stored_data_2master then
      printf("Congrats! Correct data from AF master received at AF slave (sent over DT)");
    else
      v_error_found := '1';
      report "data arrived at AF_sink not equal to data sent by AF_model"
      severity warning;
    end if;     

    printf("Testcase 1 done");
  
  end p_check_Transmission_TC1;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC2(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value                        : std_logic_vector(7 downto 0);
  variable v_stored_data_from_master_AF       : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_AF       : t_TDM_model_voice_data;
  variable v_stored_data_from_master_TDM_25ms : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_TDM_25ms : t_TDM_model_voice_data;
  variable v_stored_data_from_master_TDM_35ms : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_TDM_35ms : t_TDM_model_voice_data;
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC2: check transmitted data between aggregate link and TDM-port (please wait...)");
    
    wait for 5 ms;
    v_stored_data_from_master_AF  := AF_model_src_status.sent_voice_data;
    v_stored_data_from_slave1_AF  := AF_model_src_status_s1.sent_voice_data;
    
    -- 21.19ms:
    wait until TDM_model_status.voice_data_valid(0) = '1';
    if (v_stored_data_from_master_AF(0)(0 to 66) = TDM_model_status.voice_data_snk(3)(0 to 66)) and
       (v_stored_data_from_master_AF(1)(0 to 66) = TDM_model_status.voice_data_snk(2)(0 to 66)) and
       (v_stored_data_from_master_AF(2)(0 to 66) = TDM_model_status.voice_data_snk(1)(0 to 66)) then
      printf("Congrats! Correct data from aggregate link to master received at TDM port");
    else
      v_error_found := '1';
      report "data arrived at TDM port not equal to data sent by AF_model to master"
      severity warning;
    end if;            

    -- 21.29ms:
    wait until TDM_model_status_s1.voice_data_valid(0) = '1';
    if (v_stored_data_from_slave1_AF(0)(0 to 66) = TDM_model_status_s1.voice_data_snk(3)(0 to 66)) and
       (v_stored_data_from_slave1_AF(1)(0 to 66) = TDM_model_status_s1.voice_data_snk(2)(0 to 66)) and
       (v_stored_data_from_slave1_AF(2)(0 to 66) = TDM_model_status_s1.voice_data_snk(1)(0 to 66)) then
      printf("Congrats! Correct data from aggregate link to slave1 received at TDM port");
    else
      v_error_found := '1';
      report "data arrived at TDM port not equal to data sent by AF_model to slave1"
      severity warning;
    end if;            

    wait for 5 ms;
    v_stored_data_from_master_TDM_25ms := TDM_model_status.sent_voice_data;
    v_stored_data_from_slave1_TDM_25ms := TDM_model_status_s1.sent_voice_data;

    wait for 10 ms;
    v_stored_data_from_master_TDM_35ms := TDM_model_status.sent_voice_data;
    v_stored_data_from_slave1_TDM_35ms := TDM_model_status_s1.sent_voice_data;
    
    -- 40.1ms
    wait until AF_model_snk_status.frame_captured = '1';
    if (AF_model_snk_status.rec_voice_data(0)(0 to 66) = v_stored_data_from_master_TDM_25ms(3)(0 to 66)) and
       (AF_model_snk_status.rec_voice_data(1)(0 to 66) = v_stored_data_from_master_TDM_35ms(2)(0 to 66)) and
       (AF_model_snk_status.rec_voice_data(2)(0 to 66) = v_stored_data_from_master_TDM_35ms(1)(0 to 66)) then
      printf("Congrats! Correct data from TDM port received at aggregate link of master");
    else
      v_error_found := '1';
      report "data arrived at AF_sink master not equal to data sent by TDM port "
      severity warning;
    end if;  

    -- 40.2ms
    wait until AF_model_snk_status_s1.frame_captured = '1';
    if (AF_model_snk_status_s1.rec_voice_data(0)(0 to 66) = v_stored_data_from_slave1_TDM_25ms(3)(0 to 66)) and
       (AF_model_snk_status_s1.rec_voice_data(1)(0 to 66) = v_stored_data_from_slave1_TDM_35ms(2)(0 to 66)) and
       (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66) = v_stored_data_from_slave1_TDM_35ms(1)(0 to 66)) then
      printf("Congrats! Correct data from TDM port received at aggregate link of slave1");
    else
      v_error_found := '1';
      report "data arrived at AF_sink slave 1 not equal to data sent by TDM port "
      severity warning;
    end if;             
    
    printf("Testcase 2 done");
  
  end p_check_Transmission_TC2;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC3(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value                        : std_logic_vector(7 downto 0);
  variable v_stored_data_from_master_AF_25ms  : t_TDM_model_voice_data;
  variable v_stored_data_from_master_AF_35ms  : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_AF_25ms  : t_TDM_model_voice_data;  
  variable v_stored_data_from_slave1_AF_35ms  : t_TDM_model_voice_data;  
  variable v_stored_data_from_master_TDM_45ms : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_TDM_45ms : t_TDM_model_voice_data;
  variable v_stored_data_from_master_TDM_55ms : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_TDM_55ms : t_TDM_model_voice_data;
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC3: check all transmitted data in TC3 (please wait...)");
    -- 25ms
    wait for 15 ms;
    v_stored_data_from_slave1_AF_25ms := AF_model_src_status_s1.sent_voice_data;
    v_stored_data_from_master_AF_25ms := AF_model_src_status.sent_voice_data;

    -- 35ms
    wait for 10 ms;
    v_stored_data_from_slave1_AF_35ms := AF_model_src_status_s1.sent_voice_data;
    v_stored_data_from_master_AF_35ms := AF_model_src_status.sent_voice_data;
    wait for 5 ms;

    -- 43.6ms
    wait until TDM_model_status.voice_data_valid(3) = '1';
    if (TDM_model_status.voice_data_snk(2)(0 to 66)  = v_stored_data_from_master_AF_35ms(0)(0 to 66))  and    -- AF sou m 1:  TDM m out 3
       (TDM_model_status.voice_data_snk(1)(0 to 55)  = v_stored_data_from_master_AF_35ms(4)(0 to 55))  and    -- AF sou m 5:  TDM m out 2
       (TDM_model_status.voice_data_snk(15)(0 to 66) = v_stored_data_from_master_AF_35ms(8)(0 to 66))    and  -- AF sou m 9:  TDM m out 16
       (TDM_model_status.voice_data_snk(11)(0 to 55) = v_stored_data_from_master_AF_35ms(12)(0 to 55)) then   -- AF sou m 13: TDM m out 12 (5.6k)
      printf("Congrats! Correct data from AF source of master received at TDM port of master");
    else
      v_error_found := '1';
      report "data arrived at TDM_sink master not equal to data sent by AF source of master"
      severity warning;
    end if;  
                
    -- 43.7ms
    wait until TDM_model_status_s1.voice_data_valid(3) = '1';
    if (TDM_model_status_s1.voice_data_snk(4)(0 to 55)  = v_stored_data_from_slave1_AF_35ms(0)(0 to 55))   and    -- AF sou s 1: TDM s out 5 (5.6k)
       (TDM_model_status_s1.voice_data_snk(8)(0 to 66)  = v_stored_data_from_slave1_AF_35ms(5)(0 to 66))   and    -- AF sou s 6: TDM s out 9
       (TDM_model_status_s1.voice_data_snk(0)(0 to 66)  = v_stored_data_from_slave1_AF_35ms(8)(0 to 66))   and    -- AF sou s 9: TDM s out 1
       (TDM_model_status_s1.voice_data_snk(12)(0 to 66) = v_stored_data_from_slave1_AF_35ms(14)(0 to 66))  then   -- AF sou s 15: TDM s out 13
      printf("Congrats! Correct data from AF source of slave1 received at TDM port of slave1");
    else
      v_error_found := '1';
      report "data arrived at TDM_sink slave1 not equal to data sent by AF source of slave1"
      severity warning;
    end if;   

    v_stored_data_from_master_TDM_45ms := TDM_model_status.sent_voice_data;
    v_stored_data_from_slave1_TDM_45ms := TDM_model_status_s1.sent_voice_data;

    wait until AF_model_src_status_s1.frame_finished = '1';     -- 49.97ms
    -- 55ms
    wait for 5 ms;
    v_stored_data_from_master_TDM_55ms := TDM_model_status.sent_voice_data;
    v_stored_data_from_slave1_TDM_55ms := TDM_model_status_s1.sent_voice_data;

    -- 59.7ms
    wait until AF_model_snk_status.frame_captured = '1';
    if (AF_model_snk_status.rec_voice_data(3)(0 to 55)  = v_stored_data_from_slave1_AF_25ms(1)(0 to 55))  and    -- AF sou s 2: AF sin m 4 (5.6k)
       (AF_model_snk_status.rec_voice_data(5)(0 to 66)  = v_stored_data_from_slave1_AF_25ms(2)(0 to 66))  and    -- AF sou s 3: AF sin m 6
       (AF_model_snk_status.rec_voice_data(6)(0 to 66)  = v_stored_data_from_slave1_AF_25ms(3)(0 to 66))  and    -- AF sou s 4: AF sin m 7
       (AF_model_snk_status.rec_voice_data(7)(0 to 55)  = v_stored_data_from_slave1_AF_25ms(4)(0 to 55))  and    -- AF sou s 5: AF sin m 8 (5.6k)
       (AF_model_snk_status.rec_voice_data(1)(0 to 66)  = v_stored_data_from_slave1_AF_25ms(6)(0 to 66))  and    -- AF sou s 7: AF sin m 2
       (AF_model_snk_status.rec_voice_data(2)(0 to 66)  = v_stored_data_from_slave1_AF_25ms(7)(0 to 66))  and    -- AF sou s 8: AF sin m 3
       (AF_model_snk_status.rec_voice_data(9)(0 to 66)  = v_stored_data_from_slave1_AF_25ms(9)(0 to 66))  and    -- AF sou s 10: AF sin m 10
       (AF_model_snk_status.rec_voice_data(14)(0 to 66) = v_stored_data_from_slave1_AF_25ms(10)(0 to 66)) and    -- AF sou s 11: AF sin m 15
       (AF_model_snk_status.rec_voice_data(15)(0 to 66) = v_stored_data_from_slave1_AF_25ms(11)(0 to 66)) and    -- AF sou s 12: AF sin m 16
       (AF_model_snk_status.rec_voice_data(13)(0 to 66) = v_stored_data_from_slave1_AF_25ms(12)(0 to 66)) and    -- AF sou s 13: AF sin m 14
       (AF_model_snk_status.rec_voice_data(11)(0 to 66) = v_stored_data_from_slave1_AF_25ms(13)(0 to 66)) and    -- AF sou s 14: AF sin m 12
       (AF_model_snk_status.rec_voice_data(10)(0 to 55) = v_stored_data_from_slave1_AF_25ms(15)(0 to 55)) then   -- AF sou s 16: AF sin m 11 (5.6k)
      printf("Congrats! Correct data from aggregate source of slave 1 received at aggregate port of master");
    else
      v_error_found := '1';
      report "data arrived at AF_sink master not equal to data sent by AF_source of slave 1"
      severity warning;
    end if;     

    if (AF_model_snk_status.rec_voice_data(0)(0 to 66)  = v_stored_data_from_master_TDM_45ms(2)(0 to 66)) and    -- AF sin m 1: TDM m in 3
       (AF_model_snk_status.rec_voice_data(4)(0 to 55)  = v_stored_data_from_master_TDM_55ms(1)(0 to 55)) and    -- AF sin m 5: TDM m in 2 (5.6k)
       (AF_model_snk_status.rec_voice_data(8)(0 to 66)  = v_stored_data_from_master_TDM_55ms(15)(0 to 66)) and   -- AF sin m 9: TDM m in 16
       (AF_model_snk_status.rec_voice_data(12)(0 to 55) = v_stored_data_from_master_TDM_55ms(11)(0 to 55)) then  -- AF sin m 13: TDM m in 12 (5.6k) 
      printf("Congrats! Correct data from TDM source of master received at aggregate port of master");
    else
      v_error_found := '1';
      report "data arrived at AF_sink master not equal to data sent by TDM of master"
      severity warning;
    end if;  
    
    -- 59.8ms
    wait until AF_model_snk_status_s1.frame_captured = '1';
    if (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66)  = v_stored_data_from_master_AF_25ms(1)(0 to 66))  and    -- AF sou m 2:  AF sin s 7     
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66)  = v_stored_data_from_master_AF_25ms(2)(0 to 66))  and    -- AF sou m 3:  AF sin s 8     
       (AF_model_snk_status_s1.rec_voice_data(1)(0 to 55)  = v_stored_data_from_master_AF_25ms(3)(0 to 55))  and    -- AF sou m 4:  AF sin s 2  (5.6k) 
       (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66)  = v_stored_data_from_master_AF_25ms(5)(0 to 66))  and    -- AF sou m 6:  AF sin s 3     
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 66)  = v_stored_data_from_master_AF_25ms(6)(0 to 66))  and    -- AF sou m 7:  AF sin s 4     
       (AF_model_snk_status_s1.rec_voice_data(4)(0 to 55)  = v_stored_data_from_master_AF_25ms(7)(0 to 55))  and    -- AF sou m 8:  AF sin s 5  (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(9)(0 to 66)  = v_stored_data_from_master_AF_25ms(9)(0 to 66))  and    -- AF sou m 10: AF sin s 10
       (AF_model_snk_status_s1.rec_voice_data(15)(0 to 55) = v_stored_data_from_master_AF_25ms(10)(0 to 55)) and    -- AF sou m 11: AF sin s 16 (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(13)(0 to 66) = v_stored_data_from_master_AF_25ms(11)(0 to 66)) and    -- AF sou m 12: AF sin s 14
       (AF_model_snk_status_s1.rec_voice_data(12)(0 to 66) = v_stored_data_from_master_AF_25ms(13)(0 to 66)) and    -- AF sou m 14: AF sin s 13
       (AF_model_snk_status_s1.rec_voice_data(10)(0 to 66) = v_stored_data_from_master_AF_25ms(14)(0 to 66)) and    -- AF sou m 15: AF sin s 11
       (AF_model_snk_status_s1.rec_voice_data(11)(0 to 66) = v_stored_data_from_master_AF_25ms(15)(0 to 66)) then   -- AF sou m 16: AF sin s 12
      printf("Congrats! Correct data from aggregate source of master received at aggregate port of slave1");
    else
      v_error_found := '1';
      report "data arrived at AF_sink slave 1 not equal to data sent by AF_source of master"
      severity warning;
    end if;     

    if (AF_model_snk_status_s1.rec_voice_data(8)(0 to 66)  = v_stored_data_from_slave1_TDM_55ms(0)(0 to 66)) and    -- TDM s in 1: AF sin s 9
       (AF_model_snk_status_s1.rec_voice_data(0)(0 to 55)  = v_stored_data_from_slave1_TDM_55ms(4)(0 to 55)) and    -- TDM s in 5: AF sin s 1 (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66)  = v_stored_data_from_slave1_TDM_55ms(8)(0 to 66)) and    -- TDM s in 9: AF sin s 6
       (AF_model_snk_status_s1.rec_voice_data(14)(0 to 66) = v_stored_data_from_slave1_TDM_55ms(12)(0 to 66)) then  -- TDM s in 13: AF sin s 15
      printf("Congrats! Correct data from TDM source of slave received at aggregate port of slave1");
    else
      v_error_found := '1';
      report "data arrived at AF_sink slave 1 not equal to data sent by TDM of slave"
      severity warning;
    end if;  

    printf("Testcase 3 done");
  
  end p_check_Transmission_TC3;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC4(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value                        : std_logic_vector(7 downto 0);
  variable v_stored_data_from_master_AF_25ms  : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_AF_15ms  : t_TDM_model_voice_data;  
  variable v_stored_data_from_slave1_AF_35ms  : t_TDM_model_voice_data;  
  variable v_stored_data_from_master_TDM_55ms : t_TDM_model_voice_data;
  variable v_stored_data_from_master_TDM_65ms : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_TDM_45ms : t_TDM_model_voice_data;
  variable v_stored_data_from_slave1_TDM_55ms : t_TDM_model_voice_data;
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC4: check all transmitted data in TC4 (please wait...)");

    -- 15ms
    wait for 6 ms;
    v_stored_data_from_slave1_AF_15ms := AF_model_src_status_s1.sent_voice_data;
            
    -- 22.9ms
    wait for 5 ms;
    wait until TDM_model_status_s1.voice_data_valid(3) = '1';
    if (TDM_model_status_s1.voice_data_snk(0)(0 to 66)  = v_stored_data_from_slave1_AF_15ms(0)(0 to 66))   and    -- AF sou s 1: TDM s out 1
       (TDM_model_status_s1.voice_data_snk(6)(0 to 55)  = v_stored_data_from_slave1_AF_15ms(3)(0 to 55))   and    -- AF sou s 4: TDM s out 7 (5.6k)
       (TDM_model_status_s1.voice_data_snk(15)(0 to 55) = v_stored_data_from_slave1_AF_15ms(7)(0 to 55))   and    -- AF sou s 8: TDM s out 16 (5.6k)
       (TDM_model_status_s1.voice_data_snk(7)(0 to 66)  = v_stored_data_from_slave1_AF_15ms(8)(0 to 66))   and    -- AF sou s 9: TDM s out 8
       (TDM_model_status_s1.voice_data_snk(1)(0 to 55)  = v_stored_data_from_slave1_AF_15ms(10)(0 to 55))  and    -- AF sou s 11: TDM s out 2 (5.6k)
       (TDM_model_status_s1.voice_data_snk(14)(0 to 66) = v_stored_data_from_slave1_AF_15ms(13)(0 to 66))  then   -- AF sou s 14: TDM s out 15
      printf("Congrats! Correct data from AF source of slave1 received at TDM port of slave1");
    else
      v_error_found := '1';
      report "data arrived at TDM_sink slave1 not equal to data sent by AF source of slave1"
      severity warning;
    end if;   

    v_stored_data_from_master_AF_25ms := AF_model_src_status.sent_voice_data;
    wait for 5 ms;

    -- 33ms
    wait until TDM_model_status.voice_data_valid(3) = '1';
    if (TDM_model_status.voice_data_snk(0)(0 to 66)  = v_stored_data_from_master_AF_25ms(2)(0 to 66))    and    -- AF sou m 3:  TDM m out 1
       (TDM_model_status.voice_data_snk(6)(0 to 55)  = v_stored_data_from_master_AF_25ms(3)(0 to 55))    and    -- AF sou m 7:  TDM m out 4  (5.6k)
       (TDM_model_status.voice_data_snk(15)(0 to 55) = v_stored_data_from_master_AF_25ms(4)(0 to 55))    and    -- AF sou m 5:  TDM m out 16 (5.6k)
       (TDM_model_status.voice_data_snk(7)(0 to 66)  = v_stored_data_from_master_AF_25ms(9)(0 to 66))    and    -- AF sou m 10: TDM m out 8
       (TDM_model_status.voice_data_snk(1)(0 to 55)  = v_stored_data_from_master_AF_25ms(10)(0 to 55))   and    -- AF sou m 11: TDM m out 2  (5.6k)
       (TDM_model_status.voice_data_snk(14)(0 to 66) = v_stored_data_from_master_AF_25ms(13)(0 to 66))   then   -- AF sou m 14: TDM m out 15
      printf("Congrats! Correct data from AF source of master received at TDM port of master");
    else
      v_error_found := '1';
      report "data arrived at TDM_sink master not equal to data sent by AF source of master"
      severity warning;
    end if;         
    
    -- 35ms
    wait for 2 ms;
    v_stored_data_from_slave1_AF_35ms := AF_model_src_status_s1.sent_voice_data;

    -- 45ms
    wait for 10 ms;
    v_stored_data_from_slave1_TDM_45ms := TDM_model_status_s1.sent_voice_data;
    
    -- 55ms
    wait for 10 ms;
    v_stored_data_from_master_TDM_55ms := TDM_model_status.sent_voice_data;
    v_stored_data_from_slave1_TDM_55ms := TDM_model_status_s1.sent_voice_data;

    -- 59.8ms
    wait until AF_model_snk_status_s1.frame_captured = '1';
    if (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66)  = v_stored_data_from_master_AF_25ms(0)(0 to 66))  and    -- AF sou m 1:  AF sin s 3
       (AF_model_snk_status_s1.rec_voice_data(4)(0 to 55)  = v_stored_data_from_master_AF_25ms(7)(0 to 55))  and    -- AF sou m 8:  AF sin s 5 (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66)  = v_stored_data_from_master_AF_25ms(6)(0 to 66))  and    -- AF sou m 7:  AF sin s 7
       (AF_model_snk_status_s1.rec_voice_data(9)(0 to 66)  = v_stored_data_from_master_AF_25ms(15)(0 to 66)) and    -- AF sou m 16: AF sin s 10
       (AF_model_snk_status_s1.rec_voice_data(12)(0 to 55) = v_stored_data_from_master_AF_25ms(14)(0 to 55)) and    -- AF sou m 15: AF sin s 13 (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(14)(0 to 66) = v_stored_data_from_master_AF_25ms(1)(0 to 66))  then   -- AF sou m 2:  AF sin s 15
      printf("Congrats! Correct data from aggregate source of master received at aggregate port of slave1");
    else
      v_error_found := '1';
      report "data arrived at AF_sink slave 1 not equal to data sent by AF_source of master"
      severity warning;
    end if;     

    if (AF_model_snk_status_s1.rec_voice_data(0)(0 to 66)  = v_stored_data_from_slave1_TDM_55ms(0)(0 to 66))  and    -- TDM s in 1:  AF sin s 1
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 55)  = v_stored_data_from_slave1_TDM_55ms(6)(0 to 55))  and    -- TDM s in 7:  AF sin s 4  (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 55)  = v_stored_data_from_slave1_TDM_55ms(15)(0 to 55)) and    -- TDM s in 16: AF sin s 8  (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(8)(0 to 66)  = v_stored_data_from_slave1_TDM_55ms(7)(0 to 66))  and    -- TDM s in 8:  AF sin s 9
       (AF_model_snk_status_s1.rec_voice_data(10)(0 to 55) = v_stored_data_from_slave1_TDM_55ms(1)(0 to 55))  and    -- TDM s in 2:  AF sin s 11 (5.6k)
       (AF_model_snk_status_s1.rec_voice_data(13)(0 to 66) = v_stored_data_from_slave1_TDM_55ms(14)(0 to 66)) then   -- TDM s in 15: AF sin s 14
      printf("Congrats! Correct data from TDM source of slave received at aggregate port of slave1");
    else
      v_error_found := '1';
      report "data arrived at AF_sink slave 1 not equal to data sent by TDM of slave"
      severity warning;
    end if;  

    -- 65ms
    wait for 5 ms;
    v_stored_data_from_master_TDM_65ms := TDM_model_status.sent_voice_data;

   -- 70.2ms
   wait until AF_model_snk_status.frame_captured = '1';
    if (AF_model_snk_status.rec_voice_data(0)(0 to 66)  = v_stored_data_from_slave1_AF_35ms(2)(0 to 66))  and    -- AF sou s 3: AF sin m 1  0
       (AF_model_snk_status.rec_voice_data(7)(0 to 55)  = v_stored_data_from_slave1_AF_35ms(4)(0 to 55))  and    -- AF sou s 5: AF sin m 8 (5.6k)  0
       (AF_model_snk_status.rec_voice_data(6)(0 to 66)  = v_stored_data_from_slave1_AF_35ms(6)(0 to 66))  and    -- AF sou s 7: AF sin m 7  0
       (AF_model_snk_status.rec_voice_data(15)(0 to 66) = v_stored_data_from_slave1_AF_35ms(9)(0 to 66))  and    -- AF sou s 10: AF sin m 16  2
       (AF_model_snk_status.rec_voice_data(14)(0 to 55) = v_stored_data_from_slave1_AF_35ms(12)(0 to 55)) and    -- AF sou s 13: AF sin m 15  0
       (AF_model_snk_status.rec_voice_data(1)(0 to 66)  = v_stored_data_from_slave1_AF_35ms(14)(0 to 66)) then   -- AF sou s 15: AF sin m 2  0
      printf("Congrats! Correct data from aggregate source of slave 1 received at aggregate port of master");
    else
      v_error_found := '1';
      report "data arrived at AF_sink master not equal to data sent by AF_source of slave 1"
      severity warning;
    end if;     

    if (AF_model_snk_status.rec_voice_data(2)(0 to 66)  = v_stored_data_from_master_TDM_55ms(0)(0 to 66))  and    -- AF sin m 3: TDM m in 1
       (AF_model_snk_status.rec_voice_data(3)(0 to 55)  = v_stored_data_from_master_TDM_65ms(6)(0 to 55))  and    -- AF sin m 4: TDM m in 7 (5.6k)
       (AF_model_snk_status.rec_voice_data(4)(0 to 55)  = v_stored_data_from_master_TDM_65ms(15)(0 to 55)) and    -- AF sin m 5: TDM m in 16 (5.6k)
       (AF_model_snk_status.rec_voice_data(9)(0 to 66)  = v_stored_data_from_master_TDM_65ms(7)(0 to 66))  and    -- AF sin m 10: TDM m in 8
       (AF_model_snk_status.rec_voice_data(10)(0 to 55) = v_stored_data_from_master_TDM_65ms(1)(0 to 55))  and    -- AF sin m 11: TDM m in 2 (5.6k)
       (AF_model_snk_status.rec_voice_data(13)(0 to 66) = v_stored_data_from_master_TDM_65ms(14)(0 to 66)) then   -- AF sin m 14: TDM m in 15 
      printf("Congrats! Correct data from TDM source of master received at aggregate port of master");
    else
      v_error_found := '1';
      report "data arrived at AF_sink master not equal to data sent by TDM of master"
      severity warning;
    end if;  

    printf("Testcase 4 done");
  
  end p_check_Transmission_TC4;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC5(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value            : std_logic_vector(7 downto 0);
  variable v_data_estimated       : t_TDM_model_voice_data;
  variable v_stored_data_2master  : t_TDM_model_voice_data;
  variable v_discard_counter      : std_logic_vector(7 downto 0);
  variable v_insertion_counter    : std_logic_vector(7 downto 0);
  variable v_stored_data_fromdt1  : t_TDM_model_voice_data;
  variable v_stored_data_fromdt2  : t_TDM_model_voice_data;
  variable v_stored_data_fromdt3  : t_TDM_model_voice_data;
  
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC5: check transmitted data on aggregate link");
    
    wait for 185 ms;
    -- save generated data on AF to slave
    v_stored_data_2master := AF_model_src_status.sent_voice_data;
    for i in 0 to 15 loop
      if ( (v_stored_data_2master(i)(0 to 2) = "101") or
           (v_stored_data_2master(i)(0 to 2) = "110") or
           (v_stored_data_2master(i)(0 to 2) = "000") ) then
        v_data_estimated(i)(0 to 2) := not (v_stored_data_2master(i)(0 to 2));
      else
        v_data_estimated(i)(0 to 2) := v_stored_data_2master(i)(0 to 2);
      end if;
      v_data_estimated(i)(3 to 66) := v_stored_data_2master(i)(3 to 66);
    end loop;

    wait for 20 ms;
    -- read fdct and ficr register in slave1 and save
    addr <= x"071";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    p_MPI_rd_access_s1_overspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    v_discard_counter := data;   
    printf("current FDCT register (hex): " & hstr(v_discard_counter));

    addr <= x"074";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    p_MPI_rd_access_s1_overspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    v_insertion_counter := data;   
    printf("current FICR register (hex): " & hstr(v_insertion_counter));

    -- check: generated AF to master - over DT to slave - sinked AF from slave 1 -> inserted frame with header manipulation
    wait for 20 ms;
    wait until AF_model_snk_status_s1.frame_captured = '1';      -- at 228.9ms
    if AF_model_snk_status_s1.rec_voice_data = v_data_estimated then
      printf("Congrats! Correct header manipulation done and received from AF slave1 at AF master (sent over DT)");
    else
      v_error_found := '1';
      report "header manipulation at data arrived at AF_sink of slave1 not correct"
      severity warning;
    end if;      

    -- read fdct and ficr register in slave1 again and check if incremented
    v_exp_value := std_logic_vector(to_unsigned( (to_integer(unsigned(v_discard_counter)) + 1), 8));
    addr <= x"071";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    p_MPI_rd_access_s1_overspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    v_discard_counter := data;   
    printf("current FDCT register (hex): " & hstr(v_discard_counter));
    
    if v_discard_counter /= v_exp_value then 
      v_error_found := '1';
      report "frame discard counter TX (FDCT) not incremented"
      severity warning;
    else
      printf("frame discard counter TX (FDCT) successfully incremented");
    end if;    
    
    v_exp_value := std_logic_vector(to_unsigned( (to_integer(unsigned(v_insertion_counter)) + 1), 8));
    addr <= x"074";
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    p_MPI_rd_access_s1_overspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_overspeed;
    v_insertion_counter := data;   
    printf("current FICR register (hex): " & hstr(v_insertion_counter));
    
    if v_insertion_counter /= v_exp_value then 
      v_error_found := '1';
      report "frame insertion counter RX (FICR) not incremented"
      severity warning;
    else
      printf("frame insertion counter RX (FICR) successfully incremented");
    end if;     
    
    -- check if a complete frame was dropped at sink of master
    wait until AF_model_snk_status.frame_captured = '1';      -- at 230.19ms
    v_stored_data_fromdt1 := AF_model_snk_status.rec_voice_data;
    wait until AF_model_snk_status.frame_captured = '0';

    wait until AF_model_snk_status.frame_captured = '1';      -- at 240.19ms
    v_stored_data_fromdt2 := AF_model_snk_status.rec_voice_data;
    wait until AF_model_snk_status.frame_captured = '0';

    wait until AF_model_snk_status.frame_captured = '1';      -- at 250.19ms
    v_stored_data_fromdt3 := AF_model_snk_status.rec_voice_data;
    wait until AF_model_snk_status.frame_captured = '0';

    if ( ((v_stored_data_fromdt1(0)(0 to 2)) = (v_stored_data_fromdt2(0)(0 to 2)))  and
         ((v_stored_data_fromdt2(0)(0 to 2)) = (v_stored_data_fromdt3(0)(0 to 2))) ) then
      printf("Congrats! Complete frame discarded (data from faster AF slave1 received at AF master sent over DT)");
    else
      v_error_found := '1';
      report "expected frame discarding was not successful"
      severity warning;
    end if;     

    printf("Testcase 5 done");
  
  end p_check_Transmission_TC5;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC6(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value            : std_logic_vector(7 downto 0);
  variable v_data_estimated       : t_TDM_model_voice_data;
  variable v_stored_data_2slave1  : t_TDM_model_voice_data;
  variable v_discard_counter      : std_logic_vector(7 downto 0);
  variable v_insertion_counter    : std_logic_vector(7 downto 0);
  variable v_stored_data_fromdt1  : t_TDM_model_voice_data;
  variable v_stored_data_fromdt2  : t_TDM_model_voice_data;
  variable v_stored_data_fromdt3  : t_TDM_model_voice_data;
  
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC6: check transmitted data on aggregate link");
    
    -- save generated data on AF to master
    wait for 301 ms;
    v_stored_data_2slave1 := AF_model_src_status_s1.sent_voice_data;

    for i in 0 to 15 loop
      if ( (v_stored_data_2slave1(i)(0 to 2) = "101") or
           (v_stored_data_2slave1(i)(0 to 2) = "110") or
           (v_stored_data_2slave1(i)(0 to 2) = "000") ) then
        v_data_estimated(i)(0 to 2) := not (v_stored_data_2slave1(i)(0 to 2));
      else
        v_data_estimated(i)(0 to 2) := v_stored_data_2slave1(i)(0 to 2);
      end if;
      v_data_estimated(i)(3 to 66) := v_stored_data_2slave1(i)(3 to 66);
    end loop;

    wait for 10 ms;                                                     -- at 320ms
    wait until clk32_s1 = '1';

    -- read fdcr and fict register in slave1 and save
    addr <= x"073";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    p_MPI_rd_access_s1_underspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    v_discard_counter := data;   
    printf("current FDCR register (hex): " & hstr(v_discard_counter));

    addr <= x"072";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    p_MPI_rd_access_s1_underspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    v_insertion_counter := data;   
    printf("current FICT register (hex): " & hstr(v_insertion_counter));
    
    -- save some data for checking frame discarding (later)
    wait for 15 ms;                                                   -- at 335ms
    wait until AF_model_snk_status_s1.frame_captured = '1';           -- at 337.19ms
    v_stored_data_fromdt1 := AF_model_snk_status_s1.rec_voice_data;
    wait until AF_model_snk_status_s1.frame_captured = '0';

    wait until AF_model_snk_status_s1.frame_captured = '1';           -- at 347.71ms
    v_stored_data_fromdt2 := AF_model_snk_status_s1.rec_voice_data;
    wait until AF_model_snk_status_s1.frame_captured = '0';
    
    -- check: generated AF to slave1 - over DT to master - sinked AF from master -> inserted frame with header manipulation
    wait until AF_model_snk_status.frame_captured = '1';              -- at 350.17ms
    if (AF_model_snk_status.rec_voice_data(0 to 15)(0 to 2)) = (v_data_estimated(0 to 15)(0 to 2)) then
      printf("Congrats! Correct header manipulation done and received from AF master at AF slave1 (sent over DT)");
    else
      v_error_found := '1';
      report "header manipulation at data arrived at AF_sink of master not correct"
      severity warning;
    end if;      

    -- read fdcr and fict register in slave1 again and check if incremented
    v_exp_value := std_logic_vector(to_unsigned( (to_integer(unsigned(v_discard_counter)) + 1), 8));
    addr <= x"073";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    p_MPI_rd_access_s1_underspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    v_discard_counter := data;   
    printf("current FDCR register (hex): " & hstr(v_discard_counter));
    
    if v_discard_counter /= v_exp_value then 
      v_error_found := '1';
      printf("frame discard counter RX (FDCR) not incremented");
    else
      printf("frame discard counter RX (FDCR) successfully incremented");
    end if;    
    
    v_exp_value := std_logic_vector(to_unsigned( (to_integer(unsigned(v_insertion_counter)) + 1), 8));
    addr <= x"072";
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    p_MPI_rd_access_s1_underspeed (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1_underspeed;
    v_insertion_counter := data;   
    printf("current FICT register (hex): " & hstr(v_insertion_counter));
    
    if v_insertion_counter /= v_exp_value then 
      v_error_found := '1';
      printf("frame insertion counter TX (FICT) not incremented");
    else
      printf("frame insertion counter TX (FICT) successfully incremented");
    end if;     
    
    -- check if a complete frame was dropped at sink of slave1
    wait until AF_model_snk_status_s1.frame_captured = '1';               -- at 358.24ms
    v_stored_data_fromdt3 := AF_model_snk_status_s1.rec_voice_data;
    wait until AF_model_snk_status_s1.frame_captured = '0';

    if ( ((v_stored_data_fromdt1(0)(0 to 2)) = (v_stored_data_fromdt2(0)(0 to 2)))  and
         ((v_stored_data_fromdt1(0)(0 to 2)) = (v_stored_data_fromdt3(0)(0 to 2))) ) then
      printf("Congrats! Complete frame discarded (data from faster AF master received at AF slave1 sent over DT)");
    else
      v_error_found := '1';
      report "expected frame discarding was not successful"
      severity warning;
    end if;     

    printf("Testcase 6 done");
  
  end p_check_Transmission_TC6;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC7(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value            : std_logic_vector(7 downto 0);
  variable v_data_estimated       : t_TDM_model_voice_data;
  variable v_stored_data_2slave1  : t_TDM_model_voice_data;
  variable v_stored_data_2master  : t_TDM_model_voice_data;
  variable v_discard_counter      : std_logic_vector(7 downto 0);
  variable v_insertion_counter    : std_logic_vector(7 downto 0);
--variable v_stored_data_fromdt1  : t_TDM_model_voice_data;
--variable v_stored_data_fromdt2  : t_TDM_model_voice_data;
--variable v_stored_data_fromdt3  : t_TDM_model_voice_data;
  
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC7: check transmitted data and registers");

    v_exp_value := x"01";   -- at 9ms: master has no sync on DT from slave1
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 9ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 9ms in master is correct");
    end if;    

    wait for 6 ms;       -- 15ms: slave1 has no data on DT --> check TND-registers

    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 15ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 15ms in slave1 is correct");
    end if;         
    
    wait for 20 ms;             -- 35ms: master and slave1 have now correct data streams on DT --> check TND-registers again
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 35ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 35ms in master is correct");
    end if;
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 35ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 35ms in master is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 35ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 35ms in master is correct");
    end if;     
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 35ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 35ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 35ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 35ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 35ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 35ms in slave1 is correct");
    end if;  
    
    wait for 24 ms;     -- at 59ms
    printf("disable 8 voice channels on the digital transit in slave1");
    addr <= VCE0;    
    data <= "10101010";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCE1;    
    data <= "10101010";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    
    wait for 16 ms;     -- at 75ms
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 75ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 75ms in master is correct");
    end if;
    v_exp_value := x"55";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 75ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 75ms in master is correct");
    end if;         
    v_exp_value := x"55";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 75ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 75ms in master is correct");
    end if;     
    
    wait for 4 ms;    -- at 79ms
    
    printf("disable all voice channels on the digital transit in slave1");
    addr <= VCE0;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCE1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;    
    
    wait for 10 ms;
    wait until AF_model_snk_status.frame_captured = '1';    -- at 90.1ms
    if (AF_model_snk_status.rec_voice_data(0)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(2)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(4)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(6)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(8)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(12)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(14)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS detected at master AF port on 8 channels");
    else
      v_error_found := '1';
      report "no AIS detected on masters AF port at 90ms"
      severity warning;
    end if;     
    
    wait for 5 ms;      -- at 95ms
    v_exp_value := x"01";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 95ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 95ms in master is correct");
    end if;
    v_exp_value := x"FF";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 95ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 95ms in master is correct");
    end if;         
    v_exp_value := x"FF";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 95ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 95ms in master is correct");
    end if;     
    
    wait for 10 ms;   -- at 105ms
    wait until AF_model_snk_status.frame_captured = '1';    -- at 110ms
    if (AF_model_snk_status.rec_voice_data(0)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(1)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(2)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(3)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(4)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(5)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(6)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(7)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(8)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(9)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(11)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(12)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(13)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(14)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(15)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS detected at master AF port on all 16 channels");
    else
      v_error_found := '1';
      report "no AIS detected on masters AF port at 110ms"
      severity warning;
    end if;         

    printf("re-enable all voice channels on digital transit in slave1");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;        

    printf("disable 8 voice channels on digital transit in master");
    addr <= VCE0;    
    data <= "01010101";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCE1;    
    data <= "01010101";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;   

    wait for 15 ms;      -- at 125ms
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 125ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 125ms in slave1 is correct");
    end if;         
    v_exp_value := x"AA";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 125ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 125ms in slave1 is correct");
    end if;         
    v_exp_value := x"AA";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 125ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 125ms in slave1 is correct");
    end if;         
    
    wait for 4 ms;     -- at 129ms
    
    printf("disable all voice channels on digital transit in master");
    addr <= VCE0;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCE1;    
    data <= "00000000";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;      

    wait for 16 ms;     -- at 145ms

    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 145ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 145ms in slave1 is correct");
    end if;         
    v_exp_value := x"FF";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 145ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 145ms in slave1 is correct");
    end if;         
    v_exp_value := x"FF";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 145ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 145ms in slave1 is correct");
    end if;     
    
    wait until AF_model_snk_status_s1.frame_captured = '1';    -- at 148.95ms
    if (AF_model_snk_status_s1.rec_voice_data(1)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(9)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(11)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(13)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(15)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") then
      printf("Congrats! Correct 0s detected at slave1 AF port on 8 voice channels");
    else
      v_error_found := '1';
      report "no 0s detected on slave1s AF port at 148ms"
      severity warning;
    end if;       

    unplug                <= '1';                             -- unplug digital transit cable at 149ms
    printf("re-enable all voice channels on digital transit in master");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;      

    wait for 15 ms;
    wait until AF_model_snk_status_s1.frame_captured = '1';    -- at 168.75ms
    if (AF_model_snk_status_s1.rec_voice_data(0)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(1)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(4)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(8)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(9)(0 to 66)  = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(10)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(11)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(12)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(13)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(14)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(15)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") then
      printf("Congrats! Correct 0s detected at slave1 AF port on all voice channels");
    else
      v_error_found := '1';
      report "no 0s detected on slave1s AF port at 168ms"
      severity warning;
    end if;      
        
    wait for 6 ms;   -- at 175ms
    v_exp_value := x"01";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 175ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 175ms in master is correct");
    end if;
    
    v_exp_value := x"01";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 175ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 175ms in slave1 is correct");
    end if;             

    wait for 10 ms;   -- at 185ms
    wait until AF_model_snk_status.frame_captured = '1';    -- at 190ms
    if (AF_model_snk_status.rec_voice_data(0)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(1)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(2)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(3)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(4)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(5)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(6)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(7)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(8)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(9)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(11)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(12)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(13)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(14)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(15)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS detected at master AF port on all voice channels");
    else
      v_error_found := '1';
      report "no AIS detected on masters AF port at 190ms"
      severity warning;
    end if;             

    wait until AF_model_snk_status_s1.frame_captured = '1';    -- at 190ms
    if (AF_model_snk_status_s1.rec_voice_data(0)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(1)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(4)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(8)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(9)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(11)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(12)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(13)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(14)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(15)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS detected at slave1 AF port on all voice channels");
    else
      v_error_found := '1';
      report "no AIS detected on slave1s AF port at 190ms"
      severity warning;
    end if;      
            
    unplug                <= '0';                             -- reconnect digital transit cable at 190ms

    printf("master: set 12 voice channels to digital transit and 4 to local TDM");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCTL0;    
    data <= "00001111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    printf("slave1: set 12 voice channels to digital transit and 4 to local TDM");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCTL1;    
    data <= "11110000";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    wait for 25 ms;   -- at 215ms
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 215ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 215ms in master is correct");
    end if;
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 215ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 215ms in master is correct");
    end if;         
    v_exp_value := x"0F";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 215ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 215ms in master is correct");
    end if;     
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 215ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 215ms in slave1 is correct");
    end if;         
    v_exp_value := x"F0";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 215ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 215ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 215ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 215ms in slave1 is correct");
    end if;      

    wait for 20 ms;   -- at 235ms
    wait until AF_model_snk_status.frame_captured = '1';    -- at 240ms
    if (AF_model_snk_status.rec_voice_data(8)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(9)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(11)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS detected at master AF port on 4 voice channels");
    else
      v_error_found := '1';
      report "no AIS detected on masters AF port at 240ms"
      severity warning;
    end if;         

    wait until AF_model_snk_status_s1.frame_captured = '1';    -- at 240ms
    if (AF_model_snk_status_s1.rec_voice_data(4)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") then
      printf("Congrats! Correct all-0 detected at slave1 AF port on 4 voice channels");
    else
      v_error_found := '1';
      report "no all-0 detected on slave1s AF port at 240ms"
      severity warning;
    end if;      
    
    AF_model_src_config.enable          <= '0';
    AF_model_src_config.voice_ch_en     <= "0000000011111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.enable          <= '0';
    AF_model_snk_config.voice_ch_en     <= "0000000011111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.enable       <= '0';
    AF_model_src_config_s1.voice_ch_en  <= "1111111100000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.enable       <= '0';
    AF_model_snk_config_s1.voice_ch_en  <= "1111111100000000";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.enable          <= '1';
    AF_model_src_config_s1.enable       <= '1';

    printf("master: set 12 voice channels to digital transit and 4 to local TDM, disable 8 channels on aggregate link");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCTL0;    
    data <= "00001111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    for n in 0 to 7 loop
      addr <= TCR_AD(n);    
      data <= X"00";
      wait for 1*c_clock_uC_cycle;
      p_MPI_wr_access (addr, data, test_proc);
      wait for 200*c_clock_uC_cycle;
    end loop;
    
    printf("slave1: set 12 voice channels to digital transit and 4 to local TDM, disable 8 channels on aggregate link");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCTL1;    
    data <= "11110000";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    for n in 0 to 7 loop
      addr <= TCR_AD(n+8);    
      data <= X"00";
      wait for 1*c_clock_uC_cycle_s1;
      p_MPI_wr_access_s1 (addr, data, test_proc);
      wait for 200*c_clock_uC_cycle_s1;
    end loop;

    wait for 9 ms;    -- at 249ms
    AF_model_snk_config.enable          <= '1';
    AF_model_snk_config_s1.enable       <= '1';


    wait for 16 ms;   -- at 265ms
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 265ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 265ms in master is correct");
    end if;
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 265ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 265ms in master is correct");
    end if;         
    v_exp_value := x"0F";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 265ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 265ms in master is correct");
    end if;     
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 265ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 265ms in slave1 is correct");
    end if;         
    v_exp_value := x"F0";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 265ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 265ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 265ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 265ms in slave1 is correct");
    end if;       
    
    wait for 10 ms;
           
    wait until AF_model_snk_status.frame_captured = '1';    -- at 280ms
    if (AF_model_snk_status.rec_voice_data(8)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(9)(0 to 66)  = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(11)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(12)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(13)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(14)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status.rec_voice_data(15)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS detected at master AF port on 8 voice channels");
    else
      v_error_found := '1';
      report "no AIS detected on masters AF port at 280ms"
      severity warning;
    end if; 

    wait until AF_model_snk_status_s1.frame_captured = '1';    -- at 280ms
    if (AF_model_snk_status_s1.rec_voice_data(0)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(1)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(4)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") and
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66) = "0000000000000000000000000000000000000000000000000000000000000000000") then
      printf("Congrats! Correct AIS on 4channels and all-0 on 4channels detected at slave1 AF port");
    else
      v_error_found := '1';
      report "no AIS or all-0 detected on slave1s AF port at 280ms"
      severity warning;
    end if;      

    AF_model_src_config.enable          <= '0';
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.enable          <= '0';
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.enable       <= '0';
    AF_model_src_config_s1.voice_ch_en  <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.enable       <= '0';
    AF_model_snk_config_s1.voice_ch_en  <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.enable          <= '1';
    AF_model_src_config_s1.enable       <= '1';

    printf("master: set 16 voice channels to digital transit, reenable all 16 channels on aggregate link");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle;
    p_MPI_wr_access (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle;

    for n in 0 to 15 loop
      addr <= TCR_AD(n);    
      data <= BRT67;
      wait for 1*c_clock_uC_cycle;
      p_MPI_wr_access (addr, data, test_proc);
      wait for 200*c_clock_uC_cycle;
    end loop;
    
    printf("slave1: set 16 voice channels to digital transit, reenable all 16 channels on aggregate link");
    addr <= VCE0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCE1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCTL0;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;
    addr <= VCTL1;    
    data <= "11111111";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_wr_access_s1 (addr, data, test_proc);
    wait for 200*c_clock_uC_cycle_s1;

    for n in 0 to 15 loop
      addr <= TCR_AD(n);    
      data <= BRT67;
      wait for 1*c_clock_uC_cycle_s1;
      p_MPI_wr_access_s1 (addr, data, test_proc);
      wait for 200*c_clock_uC_cycle_s1;
    end loop;

    wait for 9 ms;    -- at 289ms
    AF_model_snk_config.enable          <= '1';
    AF_model_snk_config_s1.enable       <= '1';

    wait for 10 ms;
    wait until AF_model_src_status.frame_finished = '1';              -- at 302.2ms
    v_stored_data_2slave1 := AF_model_src_status.sent_voice_data;
    wait until AF_model_src_status_s1.frame_finished = '1';           -- at 302.3ms
    v_stored_data_2master := AF_model_src_status_s1.sent_voice_data;

    wait for 25 ms;    
    wait until AF_model_snk_status_s1.frame_captured = '1';           -- at 332.3ms;
    if AF_model_snk_status_s1.rec_voice_data = v_stored_data_2slave1 then
      printf("Congrats! digital transit link from master to slave1 is up again");
    else
      v_error_found := '1';
      report "voice data arrived at AF_sink of slave1 not correct"
      severity warning;
    end if;   

    wait until AF_model_snk_status.frame_captured = '1';              -- at 342.2ms;
    if AF_model_snk_status.rec_voice_data = v_stored_data_2master then
      printf("Congrats! digital transit link from slave1 to master is up again");
    else
      v_error_found := '1';
      report "voice data arrived at AF_sink of master not correct"
      severity warning;
    end if;   

    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 342ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 342ms in master is correct");
    end if;
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 342ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 342ms in master is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 342ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 342ms in master is correct");
    end if;     
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 342ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 342ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 342ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 342ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 342ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 342ms in slave1 is correct");
    end if;  

    -- Masterseitig deaktivieren von AF
    AF_model_src_config.enable          <= '0';

    wait for 75 ms;
    wait until AF_model_snk_status_s1.frame_captured = '1';    -- at 422ms
    if (AF_model_snk_status_s1.rec_voice_data(0)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(1)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(2)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(3)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(4)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(5)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(6)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(7)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(8)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(9)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(10)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(11)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(12)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(13)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(14)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") and
       (AF_model_snk_status_s1.rec_voice_data(15)(0 to 66) = "1111111111111111111111111111111111111111111111111111111111111111111") then
      printf("Congrats! Correct AIS on all 16 channels detected at slave1 AF port");
    else
      v_error_found := '1';
      report "no AIS detected on slave1s AF port at 422ms"
      severity warning;
    end if;      

    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 422ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 422ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 422ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 422ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 422ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 422ms in slave1 is correct");
    end if;  

    wait for 5 ms;
    AF_model_src_config.enable          <= '1';
    printf("ETL link of master reconnected...");

    wait for 10 ms;
    wait until AF_model_src_status_s1.frame_finished = '1';           -- at 442.4ms
    v_stored_data_2master := AF_model_src_status_s1.sent_voice_data;
    wait until AF_model_src_status.frame_finished = '1';              -- at 447.4ms
    v_stored_data_2slave1 := AF_model_src_status.sent_voice_data;

    wait for 30 ms;    
    wait until AF_model_snk_status.frame_captured = '1';              -- at 482.2ms;
    if AF_model_snk_status.rec_voice_data = v_stored_data_2master then
      printf("Congrats! digital transit link from slave1 to master is up again");
    else
      v_error_found := '1';
      report "voice data at 482ms arrived at AF_sink of master not correct"
      severity warning;
    end if;   

    wait until AF_model_snk_status_s1.frame_captured = '1';           -- at 482.4ms;
    if AF_model_snk_status_s1.rec_voice_data = v_stored_data_2slave1 then
      printf("Congrats! digital transit link from master to slave1 is up again");
    else
      v_error_found := '1';
      report "voice data at 482ms arrived at AF_sink of slave1 not correct"
      severity warning;
    end if;   

    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 482ms in master is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 482ms in master is correct");
    end if;
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 482ms in master is not correct"
      severity warning;
    else
      printf("TND0 register at 482ms in master is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle;
    p_MPI_rd_access (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 482ms in master is not correct"
      severity warning;
    else
      printf("TND1 register at 482ms in master is correct");
    end if;     
    
    v_exp_value := x"00";
    addr <= x"06E";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "GDTS/NSTO register at 482ms in slave1 is not correct"
      severity warning;
    else
      printf("GDTS/NSTO register at 482ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"06F";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND0 register at 482ms in slave1 is not correct"
      severity warning;
    else
      printf("TND0 register at 482ms in slave1 is correct");
    end if;         
    v_exp_value := x"00";
    addr <= x"070";
    wait for 1*c_clock_uC_cycle_s1;
    p_MPI_rd_access_s1 (addr, data, test_proc);
    wait for 1*c_clock_uC_cycle_s1;
    if data /= v_exp_value then 
      v_error_found := '1';
      report "TND1 register at 482ms in slave1 is not correct"
      severity warning;
    else
      printf("TND1 register at 482ms in slave1 is correct");
    end if;  

    wait for 5 ms;
    
    printf("Testcase 7 done");
  
  end p_check_Transmission_TC7;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
  procedure p_check_Transmission_TC8(
    --Description:
    --none
    variable v_error_found : inout std_logic;
    signal   test_proc     : inout String (1 to 15)    
  ) is
  variable v_exp_value                : std_logic_vector(7 downto 0);
  variable v_stored_data_2master_5ms  : std_logic_vector(0 to 3187);
  variable v_stored_data_2master_15ms : std_logic_vector(0 to 3187);
  variable v_stored_data_2master_25ms : std_logic_vector(0 to 3187);
  
  begin
    test_proc <= "idle***********";
    test_proc <= "validate data  ";
    printf("---------------------------------------------------------------------------" & LF );
    printf("Testcase TC8: check looped data on LAN, aggregate link in -> out");
    
    -- save generated data on AF to master  (9ms)
    v_stored_data_2master_5ms := AF_model_src_status.sent_data_ser(0 to 3187);
    
    wait for 6 ms;
    v_stored_data_2master_15ms := AF_model_src_status.sent_data_ser(0 to 3187);

    -- check: generated AF to master - over Ethernet port loop - sinked AF from master
    wait until AF_model_snk_status.frame_captured = '1';      -- ca. 20ms
    if AF_model_snk_status.rec_data_ser(0 to 3187) = v_stored_data_2master_5ms then
      printf("Congrats! Correct data (1st package) from AF master received at AF master (looped over ethernet in LAN only mode)");
    else
      v_error_found := '1';
      report "data arrived at AF_sink not equal to data sent by AF_model (1st package)"
      severity warning;
    end if;      
    
    wait for 5 ms;
    v_stored_data_2master_25ms := AF_model_src_status.sent_data_ser(0 to 3187);
    
    -- check again: generated AF to master - over Ethernet port loop - sinked AF from master
    wait until AF_model_snk_status.frame_captured = '1';      -- ca. 30ms
    if AF_model_snk_status.rec_data_ser(0 to 3187) = v_stored_data_2master_15ms then
      printf("Congrats! Correct data (2nd package) from AF master received at AF master (looped over ethernet in LAN only mode)");
    else
      v_error_found := '1';
      report "data arrived at AF_sink not equal to data sent by AF_model (2nd package)"
      severity warning;
    end if;      
    
    wait for 5 ms;
    
    -- check again: generated AF to master - over Ethernet port loop - sinked AF from master
    wait until AF_model_snk_status.frame_captured = '1';      -- ca. 40ms
    if AF_model_snk_status.rec_data_ser(0 to 3187) = v_stored_data_2master_25ms then
      printf("Congrats! Correct data (3rd package) from AF master received at AF master (looped over ethernet in LAN only mode)");
    else
      v_error_found := '1';
      report "data arrived at AF_sink not equal to data sent by AF_model (3rd package)"
      severity warning;
    end if;      

    printf("Testcase 8 done");
  
  end p_check_Transmission_TC8;
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

  begin

    test_case <= "----------------------------------------------";
    test_case <= "initialization                                ";

    --initialization of testbench internal signals:
    v_error_found   := '0';  
    test_proc <= "idle***********";
    
    clock_speed           <= 0;                               -- start with standard clock speeds
    unplug                <= '0';                             -- digital transit cable is plugged
    loop_AF_link          <= '0';                             -- no loop on AF link
    MPI_Req.Req           <= '0';
    MPI_Req.trans         <= MPI_idle;
    MPI_Req.Addr          <= (others => '0');
    MPI_Req.Write_data    <= (others => '0');
    MPI_Req_s1.Req        <= '0';
    MPI_Req_s1.trans      <= MPI_idle;
    MPI_Req_s1.Addr       <= (others => '0');
    MPI_Req_s1.Write_data <= (others => '0');    
    
    --initialization of external signals:
    AF_model_src_config.enable          <= '0';
    AF_model_src_config.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config.pattern         <= "10100000";        --                                fix pattern of data (A = Master, 0 = TC 0)
    AF_model_snk_config.enable          <= '0';
    AF_model_snk_config.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config.enable             <= '0';
    
    AF_model_src_config_s1.enable          <= '0';
    AF_model_src_config_s1.ser_length      <= 95;                -- integer range 0 to 3199;    -- length of serial digital data
    AF_model_src_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_src_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    AF_model_src_config_s1.pattern         <= "10110000";        --                                fix pattern of data (B = Slave1, 0 = TC 0)
    AF_model_snk_config_s1.enable          <= '0';
    AF_model_snk_config_s1.voice_ch_en     <= "1111111111111111";-- std_logic_vector(0 to 15);  -- voice channel enable
    AF_model_snk_config_s1.voice_ch_rate   <= "1111111111111111";-- std_logic_vector(0 to 15);  -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    TDM_model_config_s1.enable             <= '0';


    --perform FPGA reset:
    resetn_i <= '0';
    wait for 100*c_clock_uC_cycle; 
    resetn_i <= '1';   
    wait for 200*c_clock_uC_cycle; 
    
    printf("---------------------------------------------------------------------------" & LF );
    printf("Welcome to the Digital Transit FPGA test :)");
    printf("---------------------------------------------------------------------------" & LF & "Entering testcase " );

    --select test procedures as requested:
    test_case <= "----------------------------------------------";
    test_case <= "test case: FPGA Version                       ";
    p_MPI_testcase_FPGA_Version(v_error_found, test_proc);
    resetn_i <= '0';
    AF_model_src_config.enable          <= '0';
    AF_model_snk_config.enable          <= '0';
    AF_model_src_config_s1.enable       <= '0';
    AF_model_snk_config_s1.enable       <= '0';
    TDM_model_config.enable             <= '1';
    TDM_model_config_s1.enable          <= '1';
    wait for 100*c_clock_uC_cycle; 
    resetn_i <= '1';   
    wait for 300*c_clock_uC_cycle;
    wait for 2 ms;

    --select test procedures as requested:
    if tc_selection(1) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 1: 16 voice channels over DT        ";
      resetn_i <= '0';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC1(v_error_found, test_proc);
      p_configure_AF_model_s1_TC1(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC1(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC1(v_error_found, test_proc);
      p_configure_Master_TC1(v_error_found, test_proc);
      p_configure_Slave1_TC1(v_error_found, test_proc);
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC1(v_error_found, test_proc);
      wait for 2 ms;
    end if;

    --select test procedures as requested:
    if tc_selection(2) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 2: MUX 3 voice channels             ";
      resetn_i <= '0';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC2(v_error_found, test_proc);
      p_configure_AF_model_s1_TC2(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC2(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC2(v_error_found, test_proc);
      p_configure_Master_TC2(v_error_found, test_proc);
      p_configure_Slave1_TC2(v_error_found, test_proc);
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC2(v_error_found, test_proc);
      wait for 2 ms;
    end if;
    
    --select test procedures as requested:
    if tc_selection(3) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 3: master & 1 slave -> complicated  ";
      resetn_i <= '0';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC3(v_error_found, test_proc);
      p_configure_AF_model_s1_TC3(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC3(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC3(v_error_found, test_proc);
      p_configure_Master_TC3(v_error_found, test_proc);
      p_configure_Slave1_TC3(v_error_found, test_proc);
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC3(v_error_found, test_proc);
      wait for 2 ms;
    end if;

    --select test procedures as requested:
    if tc_selection(4) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 4: master & 1 slave -> complicated 2";
      resetn_i <= '0';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC4(v_error_found, test_proc);
      p_configure_AF_model_s1_TC4(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC4(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC4(v_error_found, test_proc);
      p_configure_Master_TC4(v_error_found, test_proc);
      p_configure_Slave1_TC4(v_error_found, test_proc);
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC4(v_error_found, test_proc);
      wait for 2 ms;
    end if;
    
    --select test procedures as requested:
    if tc_selection(5) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 5: 16ch over DT-slave1 in overspeed ";
      resetn_i <= '0';
      wait for 100*c_clock_uC_cycle_s1;
      clock_speed <= 1;                               -- enable clock overspeed
      wait for 200*c_clock_uC_cycle_s1;
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 200*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC5(v_error_found, test_proc);
      p_configure_AF_model_s1_TC5(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC5(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC5(v_error_found, test_proc);
      p_configure_Master_TC5(v_error_found, test_proc);
      p_configure_Slave1_TC5(v_error_found, test_proc);
      p_check_Transmission_TC5(v_error_found, test_proc);
      resetn_i <= '0';
      wait for 100*c_clock_uC_cycle_s1;
      clock_speed <= 0;                               -- set clock back to normal speed for slave1
      wait for 200*c_clock_uC_cycle_s1;
      resetn_i <= '1';
      wait for 2 ms;    
    end if;

    --select test procedures as requested:
    if tc_selection(6) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 6: 16ch over DT-slave1 in underspeed";
      resetn_i <= '0';
      wait for 100*c_clock_uC_cycle_s1;
      clock_speed <= 2;                               -- enable clock underspeed
      wait for 200*c_clock_uC_cycle_s1;
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 200*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC6(v_error_found, test_proc);
      p_configure_AF_model_s1_TC6(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC6(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC6(v_error_found, test_proc);
      p_configure_Master_TC6(v_error_found, test_proc);
      p_configure_Slave1_TC6(v_error_found, test_proc);
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC6(v_error_found, test_proc);
      resetn_i <= '0';
      wait for 100*c_clock_uC_cycle_s1;
      clock_speed <= 0;                               -- set clock back to normal speed for slave1
      wait for 200*c_clock_uC_cycle_s1;
      resetn_i <= '1';
      wait for 2 ms; 
    end if;


    --select test procedures as requested:
    if tc_selection(7) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 7: AIS/loss of digital transit      ";
      resetn_i <= '0';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC7(v_error_found, test_proc);
      p_configure_AF_model_s1_TC7(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC7(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC7(v_error_found, test_proc);
      p_configure_Master_TC7(v_error_found, test_proc);
      p_configure_Slave1_TC7(v_error_found, test_proc);
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC7(v_error_found, test_proc);
    end if;


    --select test procedures as requested: 
    if tc_selection(8) = '1' then
      test_case <= "----------------------------------------------"; 
      test_case <= "test case 8: LAN only mode                    "; 
      resetn_i <= '0';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
      p_configure_AF_model_TC8(v_error_found, test_proc); 
      wait for 100*c_clock_uC_cycle; 
      TDM_model_config.enable             <= '1';   -- enable TDM ports to avoid warnings 
      TDM_model_config_s1.enable          <= '1'; 
      p_configure_Master_TC8(v_error_found, test_proc); 
      AF_model_snk_config.enable          <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
      AF_model_snk_config.enable          <= '1';
      AF_model_snk_config_s1.enable       <= '1';
      p_check_Transmission_TC8(v_error_found, test_proc);
    end if;
    
    
    --select test procedures as requested:
    if tc_selection(9) = '1' then
      test_case <= "----------------------------------------------";
      test_case <= "test case 9: TDM to AF frame                  ";
      resetn_i       <= '0';
      loop_AF_link   <= '1';
      AF_model_src_config.enable          <= '0';
      AF_model_snk_config.enable          <= '0';
      AF_model_src_config_s1.enable       <= '0';
      AF_model_snk_config_s1.enable       <= '0';
      TDM_model_config.enable             <= '0';
      TDM_model_config_s1.enable          <= '0';
      wait for 100*c_clock_uC_cycle; 
      resetn_i <= '1';   
      wait for 200*c_clock_uC_cycle;
--      p_configure_AF_model_TC2(v_error_found, test_proc);
--      p_configure_AF_model_s1_TC2(v_error_found, test_proc);
      wait for 100*c_clock_uC_cycle;
      p_configure_TDM_model_TC9(v_error_found, test_proc);
      p_configure_TDM_model_s1_TC9(v_error_found, test_proc);
      --adjust for page switch:
      wait for 2900 us;
      wait for 15 ms;

      p_configure_Master_TC9(v_error_found, test_proc);
      p_configure_Slave1_TC9(v_error_found, test_proc);
--      AF_model_snk_config.enable          <= '0';
--      AF_model_snk_config_s1.enable       <= '0';
      wait for 9 ms;
--      AF_model_snk_config.enable          <= '1';
--      AF_model_snk_config_s1.enable       <= '1';
--      p_check_Transmission_TC2(v_error_found, test_proc);
      wait for 2 ms;
      wait for 50 ms;
      loop_AF_link   <= '0';
      wait for 1 ms;
    end if;


    -----------------
    wait for 5 ms; 

    if (v_error_found = '0') then
      printf("---------------------------------------------------------------------------" & LF );
      printf("****Congratulation, no errors found in design!****");
      printf("---------------------------------------------------------------------------" & LF );
    else
      printf("---------------------------------------------------------------------------" & LF );
      printf("*********************    Simulation failed! :(");
      printf("---------------------------------------------------------------------------" & LF );
    end if;
    printf(" " & LF );
    printf("---------------------------------------------------------------------------" & LF );

    assert false
        report "(not really a failure... for simulation result see above) but Simulation ended! what a pity, had fun...  :("
        severity failure;
  end process;

  
END behave;


