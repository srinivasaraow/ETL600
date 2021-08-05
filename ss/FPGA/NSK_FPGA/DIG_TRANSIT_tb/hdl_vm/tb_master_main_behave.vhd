--------------------------------------------------------------------------------
-- $Workfile: tb_master_main_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_master_main_behave.vhd $
-- 
-- 4     11-07-06 11:20 Chstrue
-- corrections
-- 
-- 3     11-07-06 9:09 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.report_assist.all;

ENTITY tb_master_main IS
   PORT( 
      reset_n                 : OUT    std_logic;
      control_digtrans_master : OUT    t_control_digtrans;
      control_mux_master      : OUT    t_control_mux;
      control_digtrans_slave  : OUT    t_control_digtrans;
      unplug                  : OUT    std_logic;
      clk_32M                 : OUT    std_logic;
      control_mux_slave       : OUT    t_control_mux;
      disturb                 : OUT    std_logic
   );

-- Declarations

END tb_master_main ;


ARCHITECTURE behave OF tb_master_main IS
-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation
--   110706: corrections


  signal resetn_i      : std_logic        := '1';
  signal clk           : std_logic        := '1';
  signal test_case     : String (1 to 46) := "idle******************************************";
  signal test_proc     : String (1 to 15) := "idle***********";


BEGIN
  
  ------------------------------------------------------------------------------
  clock_proc : process 
  begin 
    clk<='1';
    wait for 15518 ps;--SYSCLK_PRD/2;
    clk<='0';
    wait for 15 ns;--SYSCLK_PRD/2;
  end process;
  ------------------------------------------------------------------------------
  disturb_proc : process 
  begin 
    disturb <= '0';
    wait for 21 ms;
    disturb <= '1';
    wait for 10 ms;
    disturb <= '0';
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--  clk <= not clk after SYSCLK_PRD/2;  -- pay attention for sim. synchronism
  clk_32M     <= clk;
  reset_n      <= resetn_i;
  
  
  
  
  
  
  
  
  test_sequence: process
  
    --tb specific variables:
    variable v_error_found     : std_logic := '0';

  begin

    test_case <= "----------------------------------------------";
    test_case <= "initialization                                ";

    --initialization of testbench internal signals:
    v_error_found   := '0';  
    test_proc <= "idle***********";
    

    
    
    --initialization of external signals:
    
    unplug <= '0';
    
    
    
    control_digtrans_master.gdtc.mss        <= '1';
    control_digtrans_master.vce             <= "1111111111111111";
    control_mux_master.mux_ch(26 downto 11) <= "1111111111111111";
--    control_digtrans_master.vctl            <= "1111111111111111";
    control_digtrans_master.vctl            <= "1111111100000000";
    control_digtrans_master.vctp            <= (others => 0);

    control_digtrans_slave.gdtc.mss         <= '0';
    control_digtrans_slave.vce              <= "1111111111111111";
    control_mux_slave.mux_ch(26 downto 11)  <= "1111111111111111";
    control_digtrans_slave.vctl             <= "1111111100000000";
--    control_digtrans_slave.vctp             <= (others => 0);
    control_digtrans_slave.vctp(1)          <= 0;
    control_digtrans_slave.vctp(2)          <= 1;
    control_digtrans_slave.vctp(3)          <= 2;
    control_digtrans_slave.vctp(4)          <= 3;
    control_digtrans_slave.vctp(5)          <= 4;
    control_digtrans_slave.vctp(6)          <= 5;
    control_digtrans_slave.vctp(7)          <= 6;
    control_digtrans_slave.vctp(8)          <= 7;
    control_digtrans_slave.vctp(9)          <= 8;
    control_digtrans_slave.vctp(10)         <= 9;
    control_digtrans_slave.vctp(11)         <= 10;
    control_digtrans_slave.vctp(12)         <= 11;
    control_digtrans_slave.vctp(13)         <= 12;
    control_digtrans_slave.vctp(14)         <= 13;
    control_digtrans_slave.vctp(15)         <= 14;
    control_digtrans_slave.vctp(16)         <= 15;
                                
    
    
    --perform FPGA reset:
    resetn_i <= '0';
    wait for 20*SYSCLK_PRD; 
    resetn_i <= '1';   
    wait for 100*SYSCLK_PRD; 
    
    printf("---------------------------------------------------------------------------" & LF );
    printf("Welcome to the TEPR FPGA test :)");
    printf("---------------------------------------------------------------------------" & LF & "Entering testcase " );



--    wait for 25ms;

--    --initialization of case:
--    control_digtrans_master.vce             <= "0000000000001111";
--    control_mux_master.mux_ch(26 downto 11) <= "1111111110010011";
--    control_digtrans_master.vctl            <= "1111111110110101";
--    control_digtrans_master.vctp            <= (others => 0);
--
--    control_digtrans_slave.vce              <= "0000000000001111";
--    control_mux_slave.mux_ch(26 downto 11)  <= "1111111110010011";
--    control_digtrans_slave.vctl             <= "1111111111011001";
--    control_digtrans_slave.vctp             <= (others => 0);
--    control_digtrans_slave.vctp(1)          <= 0;
--    control_digtrans_slave.vctp(2)          <= 2;
--    control_digtrans_slave.vctp(3)          <= 1;
--    control_digtrans_slave.vctp(4)          <= 6;
--    control_digtrans_slave.vctp(5)          <= 4;
--    control_digtrans_slave.vctp(6)          <= 3;
--    control_digtrans_slave.vctp(7)          <= 5;

--    wait for 25ms;

--    --initialization of case for slave status:
--    control_digtrans_master.vce             <= "0000000001111100";
--    control_mux_master.mux_ch(26 downto 11) <= "1111111111111111";
--    control_digtrans_master.vctl            <= "1111111110000111";
--    control_digtrans_master.vctp            <= (others => 0);
--
--    control_digtrans_slave.vce              <= "0000000001111111";
--    control_mux_slave.mux_ch(26 downto 11)  <= "1111111110010011";
--    control_digtrans_slave.vctl             <= "1111111111001111";
--    control_digtrans_slave.vctp             <= (others => 0);
--    control_digtrans_slave.vctp(1)          <= 0;
--    control_digtrans_slave.vctp(2)          <= 2;
--    control_digtrans_slave.vctp(3)          <= 1;
--    control_digtrans_slave.vctp(4)          <= 6;
--    control_digtrans_slave.vctp(5)          <= 4;
--    control_digtrans_slave.vctp(6)          <= 3;
--    control_digtrans_slave.vctp(7)          <= 5;

    wait for 25ms;
    --initialization of case for master status:
    control_digtrans_master.vce             <= "0000000000000111";
    control_mux_master.mux_ch(26 downto 11) <= "0111110000111111";
    control_digtrans_master.vctl            <= "0000000000000000";
    control_digtrans_master.vctp            <= (others => 0);

    control_digtrans_slave.vce              <= "0000000000000000";
    control_mux_slave.mux_ch(26 downto 11)  <= "1110011110010011";
    control_digtrans_slave.vctl             <= "1101111110110010";
    control_digtrans_slave.vctp             <= (others => 0);
    control_digtrans_slave.vctp(1)          <= 0;
    control_digtrans_slave.vctp(2)          <= 1;
    control_digtrans_slave.vctp(3)          <= 2;
    control_digtrans_slave.vctp(4)          <= 3;
    control_digtrans_slave.vctp(5)          <= 4;
    control_digtrans_slave.vctp(6)          <= 5;
    control_digtrans_slave.vctp(7)          <= 6;
    control_digtrans_slave.vctp(8)          <= 7;
    control_digtrans_slave.vctp(9)          <= 8;
    control_digtrans_slave.vctp(10)         <= 9;
    control_digtrans_slave.vctp(11)         <= 10;
    control_digtrans_slave.vctp(12)         <= 11;
    control_digtrans_slave.vctp(13)         <= 12;
    control_digtrans_slave.vctp(14)         <= 13;
    control_digtrans_slave.vctp(15)         <= 14;
    control_digtrans_slave.vctp(16)         <= 15;


    wait for 25ms;
    --initialization of case for master status, no echo:
    control_digtrans_master.vce             <= "0111111111111111";
    control_mux_master.mux_ch(26 downto 11) <= "0111110000111111";
    control_digtrans_master.vctl            <= "1111111100000101";
    control_digtrans_master.vctp            <= (others => 0);

    control_digtrans_slave.vce              <= "0000000000000000";
    control_mux_slave.mux_ch(26 downto 11)  <= "1110011110010011";
    control_digtrans_slave.vctl             <= "1101111110110010";
    control_digtrans_slave.vctp             <= (others => 0);
    control_digtrans_slave.vctp(1)          <= 0;
    control_digtrans_slave.vctp(2)          <= 1;
    control_digtrans_slave.vctp(3)          <= 2;
    control_digtrans_slave.vctp(4)          <= 3;
    control_digtrans_slave.vctp(5)          <= 4;
    control_digtrans_slave.vctp(6)          <= 5;
    control_digtrans_slave.vctp(7)          <= 6;
    control_digtrans_slave.vctp(8)          <= 7;
    control_digtrans_slave.vctp(9)          <= 8;
    control_digtrans_slave.vctp(10)         <= 9;
    control_digtrans_slave.vctp(11)         <= 10;
    control_digtrans_slave.vctp(12)         <= 11;
    control_digtrans_slave.vctp(13)         <= 12;
    control_digtrans_slave.vctp(14)         <= 13;
    control_digtrans_slave.vctp(15)         <= 14;
    control_digtrans_slave.vctp(16)         <= 15;

    wait for 11ms;
    --initialization of case for master status, no echo:
    control_digtrans_master.vce             <= "1111111111111111";
    control_mux_master.mux_ch(26 downto 11) <= "0111110000111111";
    control_digtrans_master.vctl            <= "0111111100000101";
    control_digtrans_master.vctp            <= (others => 0);

    control_digtrans_slave.vce              <= "1000000000000000";
    control_mux_slave.mux_ch(26 downto 11)  <= "1110011110010011";
    control_digtrans_slave.vctl             <= "1101111110110010";
    control_digtrans_slave.vctp             <= (others => 0);
    control_digtrans_slave.vctp(1)          <= 0;
    control_digtrans_slave.vctp(2)          <= 1;
    control_digtrans_slave.vctp(3)          <= 2;
    control_digtrans_slave.vctp(4)          <= 3;
    control_digtrans_slave.vctp(5)          <= 4;
    control_digtrans_slave.vctp(6)          <= 5;
    control_digtrans_slave.vctp(7)          <= 6;
    control_digtrans_slave.vctp(8)          <= 7;
    control_digtrans_slave.vctp(9)          <= 8;
    control_digtrans_slave.vctp(10)         <= 9;
    control_digtrans_slave.vctp(11)         <= 10;
    control_digtrans_slave.vctp(12)         <= 11;
    control_digtrans_slave.vctp(13)         <= 12;
    control_digtrans_slave.vctp(14)         <= 13;
    control_digtrans_slave.vctp(15)         <= 14;
    control_digtrans_slave.vctp(16)         <= 15;

















    -----------------


    wait for 20000000*SYSCLK_PRD;  

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

--for long time tests like IRIG or toggling-LED uncomment following line.
wait for 50000000*SYSCLK_PRD; 



    assert false
        report "(not really a failure... for simulation result see above) but Simulation ended! what a pity, had fun...  :("
        severity failure;
  end process;

  
  
  
END behave;



