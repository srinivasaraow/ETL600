
--
ARCHITECTURE behave OF tb_master_main IS


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

    cfg_Master_Slave_n_master <= '1';
    cfg_Master_Slave_n_slave  <= '0';
    
    cfg_local_transit_n_vec_master <= (others => '0');
    cfg_local_transit_n_vec_slave  <= "0101111111110010";
    --cfg_local_transit_n_vec_slave  <= "1010111101110101";
    
    
    --perform FPGA reset:
    resetn_i <= '0';
    wait for 20*SYSCLK_PRD; 
    resetn_i <= '1';   
    wait for 100*SYSCLK_PRD; 
    
    printf("---------------------------------------------------------------------------" & LF );
    printf("Welcome to the TEPR FPGA test :)");
    printf("---------------------------------------------------------------------------" & LF & "Entering testcase " );

    --select test procedures as requested:


    
    
    
    
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


