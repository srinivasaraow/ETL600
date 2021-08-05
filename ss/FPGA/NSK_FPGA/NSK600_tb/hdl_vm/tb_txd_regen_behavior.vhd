--------------------------------------------------------------------------
-- $Workfile: tb_txd_regen_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_txd_regen_behavior.vhd $
-- 
-- 21    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 20    22.12.04 13:52 Chalgyg
-- Bug removed.
-- 
-- 19    21.12.04 9:29 Chalgyg
-- Zero counter bug removed.
-- 
-- 17    13.12.04 13:54 Chalgyg
-- Reset value of regenerated_txd inverted.
-- 
-- 16    8.12.04 10:55 Chalgyg
-- Startbit detection added in ADPSK mode.
-- 
-- 13    23.11.04 10:43 Gygax02
-- Suppression delay enlarged from 105 to 200 us.
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
USE ieee.STD_LOGIC_UNSIGNED.all;
USE ieee.NUMERIC_STD.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_dsp_if_p.all;
USE NSK600_tb.tb_config_p.all;
USE NSK600_tb.tb_real_data_p.all;

ENTITY tb_txd_regen IS
   PORT( 
      data_out        : IN     t_channel_array;
      clk_128k        : IN     std_logic;
      delayed_v24_txd : IN     std_logic_vector (3 DOWNTO 0);
      one_error_found : OUT    std_logic;
      testcase        : IN     t_testcase;
      new_data        : IN     std_logic
   );

-- Declarations

END tb_txd_regen ;

ARCHITECTURE behavior OF tb_txd_regen IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  signal regenerated_txd      : std_logic_vector(3 downto 0) := (others => '1');
  signal difference_f         : std_logic_vector(3 downto 0) := (others => '0');
  signal difference_f_delayed : std_logic_vector(3 downto 0) := (others => '0');
  signal difference_a         : std_logic_vector(3 downto 0) := (others => '0');
  signal count0               : std_logic_vector(6 downto 0) := (others => '0');
  signal count1               : std_logic_vector(6 downto 0) := (others => '0');
  signal count2               : std_logic_vector(6 downto 0) := (others => '0');
  signal count3               : std_logic_vector(6 downto 0) := (others => '0');
  signal detect               : std_logic_vector(3 downto 0) := (others => '0');
  signal data_error           : std_logic_vector(3 downto 0) := (others => '0');
  signal suppress             : std_logic                    := '0';
  signal i0                   : integer range 0 to 9         := 0;
  signal i1                   : integer range 0 to 9         := 0;
  signal i2                   : integer range 0 to 9         := 0;
  signal i3                   : integer range 0 to 9         := 0;     
  signal received_byte0       : std_logic_vector(9 downto 0) := (others => '0');      
  signal received_byte1       : std_logic_vector(9 downto 0) := (others => '0');      
  signal received_byte2       : std_logic_vector(9 downto 0) := (others => '0');      
  signal received_byte3       : std_logic_vector(9 downto 0) := (others => '0');      
  
  signal mark_new_testcase0   : std_logic                    := '0';
  signal mark_new_testcase1   : std_logic                    := '0';
  signal mark_new_testcase2   : std_logic                    := '0';
  signal mark_new_testcase3   : std_logic                    := '0';
  signal count_reseted0       : std_logic                    := '0';
  signal count_reseted1       : std_logic                    := '0';
  signal count_reseted2       : std_logic                    := '0';
  signal count_reseted3       : std_logic                    := '0';
  
BEGIN

  difference_f(0)       <= (regenerated_txd(0) xor delayed_v24_txd(0)) when (mode(0,testcase) = 'f') else '0';
  difference_f(1)       <= (regenerated_txd(1) xor delayed_v24_txd(1)) when (mode(1,testcase) = 'f') else '0';
  difference_f(2)       <= (regenerated_txd(2) xor delayed_v24_txd(2)) when (mode(2,testcase) = 'f') else '0';
  difference_f(3)       <= (regenerated_txd(3) xor delayed_v24_txd(3)) when (mode(3,testcase) = 'f') else '0';
  difference_f_delayed  <= difference_f after 15.63 us when (suppress = '0') else "0000";    -- 15.625 us is the theoretical maximum delay (= 2 * 1/128 kHz)
  data_error            <= (difference_f_delayed or difference_a) after 1 ps when (check_regenerated_txd(testcase) = '1') else "0000";

  assert (data_error(0) /= '1')
    report "Regenerated TXD on channel 1 has errors"
    severity warning;
  assert (data_error(1) /= '1')
    report "Regenerated TXD on channel 2 has errors"
    severity warning;
  assert (data_error(2) /= '1')
    report "Regenerated TXD on channel 3 has errors"
    severity warning;
  assert (data_error(3) /= '1')
    report "Regenerated TXD on channel 4 has errors"
    severity warning;
  
  catch_transition0: process
  begin
    wait until (new_data ='0');
    if ((data_out(0)(6 downto 0) >= "0000000") and (data_out(0)(6 downto 0) <= "1111110")) then
      if (suppress = '0') then
        if (not((i0 = 0) and  (data_out(0)(7) = '1') and (mode(0,testcase) = 'a'))) then
          detect(0)  <= '1';
        end if;
      end if;
    end if;
    wait until clk_128k = '1';
    detect(0)  <= '0';
  end process;

  catch_transition1: process
  begin
    wait until (new_data ='0');
    if ((data_out(1)(6 downto 0) >= "0000000") and (data_out(1)(6 downto 0) <= "1111110")) then
      if (suppress = '0') then
        if (not((i1 = 0) and  (data_out(1)(7) = '1') and (mode(1,testcase) = 'a'))) then
          detect(1)  <= '1';
        end if;
      end if;
    end if;
    wait until clk_128k = '1';
    detect(1)  <= '0';
  end process;

  catch_transition2: process
  begin
    wait until (new_data ='0');
    if ((data_out(2)(6 downto 0) >= "0000000") and (data_out(2)(6 downto 0) <= "1111110")) then
      if (suppress = '0') then
        if (not((i2 = 0) and  (data_out(2)(7) = '1') and (mode(2,testcase) = 'a'))) then
          detect(2)  <= '1';
        end if;
      end if;
    end if;
    wait until clk_128k = '1';
    detect(2)  <= '0';
  end process;

  catch_transition3: process
  begin
    wait until (new_data ='0');
    if ((data_out(3)(6 downto 0) >= "0000000") and (data_out(3)(6 downto 0) <= "1111110")) then
      if (suppress = '0') then
        if (not((i3 = 0) and  (data_out(3)(7) = '1') and (mode(3,testcase) = 'a'))) then
          detect(3)  <= '1';
        end if;
      end if;
    end if;
    wait until clk_128k = '1';
    detect(3)  <= '0';
  end process;

  count_down0: process
  begin
    wait until clk_128k = '1';
    if (mode(0,testcase) = 'f') then          -- In FSK mode
      if (detect(0) = '1') then
        count0  <= data_out(0)(6 downto 0);
      elsif (count0 = "0000000") then
        count0  <= "1011111";
      else
        count0  <= count0 - 1;
     end if;
    elsif (mode(0,testcase) = 'a') then       -- In ADPSK mode
      if (detect(0) = '1') then
        received_byte0(9-i0)  <= data_out(0)(7);
        if (i0 >= 9) then
          i0 <= 0;
        else
          i0 <= i0+1;
        end if;
      elsif (mark_new_testcase0 = '1') then
        i0             <= 0;
        count_reseted0 <= '1';
        wait for 1 ns;
        count_reseted0 <= '0';
      end if;
    end if;
  end process;

  count_down1: process
  begin
    wait until clk_128k = '1';
    if (mode(1,testcase) = 'f') then          -- In FSK mode 
      if (detect(1) = '1') then
        count1  <= data_out(1)(6 downto 0);
      elsif (count1 = "0000000") then
        count1  <= "1011111";
      else
        count1  <= count1 - 1;
      end if;
    elsif (mode(1,testcase) = 'a') then       -- In ADPSK mode
      if (detect(1) = '1') then
        received_byte1(9-i1)  <= data_out(1)(7);
        if (i1 >= 9) then
          i1 <= 0;
        else
          i1 <= i1 + 1;
        end if;
      elsif (mark_new_testcase1 = '1') then
        i1             <= 0;
        count_reseted1 <= '1';
        wait for 1 ns;
        count_reseted1 <= '0';
      end if;
    end if;
  end process;

  count_down2: process
  begin
    wait until clk_128k = '1';
    if (mode(2,testcase) = 'f') then          -- In FSK mode 
      if (detect(2) = '1') then
        count2  <= data_out(2)(6 downto 0);
      elsif (count2 = "0000000") then
        count2  <= "1011111";
      else
        count2  <= count2 - 1;
      end if;
    elsif (mode(2,testcase) = 'a') then       -- In ADPSK mode
      if (detect(2) = '1') then
        received_byte2(9-i2)  <= data_out(2)(7);
        if (i2 >= 9) then
          i2 <= 0;
        else
          i2 <= i2 + 1;
        end if;
      elsif (mark_new_testcase2 = '1') then
        i2             <= 0;
        count_reseted2 <= '1';
        wait for 1 ns;
        count_reseted2 <= '0';
      end if;
    end if;
  end process;

  count_down3: process
  begin
    wait until clk_128k = '1';
    if (mode(3,testcase) = 'f') then          -- In FSK mode 
      if (detect(3) = '1') then
        count3  <= data_out(3)(6 downto 0);
      elsif (count3 = "0000000") then
        count3  <= "1011111";
      else
        count3  <= count3 - 1;
      end if;
    elsif (mode(3,testcase) = 'a') then       -- In ADPSK mode
      if (detect(3) = '1') then
        received_byte3(9-i3)  <= data_out(3)(7);
        if (i3 >= 9) then
          i3 <= 0;
        else
          i3 <= i3 + 1;
        end if;
      elsif (mark_new_testcase3 = '1') then
        i3             <= 0;
        count_reseted3 <= '1';
        wait for 1 ns;
        count_reseted3 <= '0';
      end if;
    end if;
  end process;

  give_out: process                          -- In FSK mode only
  begin
    wait until (clk_128k = '0');
    if ((count0 = "0000000") and (detect(0) = '0')) then
      regenerated_txd(0)  <= data_out(0)(7);
    end if;
    if ((count1 = "0000000") and (detect(1) = '0')) then
      regenerated_txd(1)  <= data_out(1)(7);
    end if;
    if ((count2 = "0000000") and (detect(2) = '0')) then
      regenerated_txd(2)  <= data_out(2)(7);
    end if;
    if ((count3 = "0000000") and (detect(3) = '0')) then
      regenerated_txd(3)  <= data_out(3)(7);
    end if;
  end process;

  mark_error: process
  begin
    one_error_found  <= '0';
    wait until ((data_error(3) = '1') or (data_error(2) = '1') or (data_error(1) = '1') or (data_error(0) = '1'));
    one_error_found  <= '1';
    wait on testcase;
  end process;

  suppress_error: process                     -- Make sure no error occurs at the beginning of a new testcase.
  begin
    wait on testcase;
    if (testcase >= 2) then
      suppress  <= '1';
      wait for 80 us;                        -- Configuration of FPGA plus one TDM cycle.
      suppress  <= '0';
    else
      suppress  <= '1';
    end if;
  end process;

  find_difference0: process                   -- In ADPSK mode only
    variable temp0  : std_logic_vector(9 downto 0);
  begin
    wait until (i0 = 0);
    if (mark_new_testcase0 = '0') then
      wait for 10 ns;
      temp0 := received_byte0 xor ('0' & pattern_v24(0,testcase) & '1');
      if (temp0 = "0000000000") then
        difference_a(0)  <= '0';
      else
        difference_a(0)  <= '1';
      end if;
    end if;
  end process;

  find_difference1: process                   -- In ADPSK mode only
    variable temp1  : std_logic_vector(9 downto 0);
  begin
    wait until (i1 = 0);
    if (mark_new_testcase1 = '0') then
      wait for 10 ns;
      temp1 := received_byte1 xor ('0' & pattern_v24(1,testcase) & '1');
      if (temp1 = "0000000000") then
        difference_a(1)  <= '0';
      else
        difference_a(1)  <= '1';
      end if;
    end if;
  end process;

  find_difference2: process                   -- In ADPSK mode only
    variable temp2  : std_logic_vector(9 downto 0);
  begin
    wait until (i2 = 0);
    if (mark_new_testcase2 = '0') then
      wait for 10 ns;
      temp2 := received_byte2 xor ('0' & pattern_v24(2,testcase) & '1');
      if (temp2 = "0000000000") then
        difference_a(2)  <= '0';
      else
        difference_a(2)  <= '1';
      end if;
    end if;
  end process;

  find_difference3: process                   -- In ADPSK mode only
    variable temp3  : std_logic_vector(9 downto 0);
  begin
    wait until (i3 = 0);
    if (mark_new_testcase3 = '0') then
      wait for 10 ns;
      temp3 := received_byte3 xor ('0' & pattern_v24(3,testcase) & '1');
      if (temp3 = "0000000000") then
        difference_a(3)  <= '0';
      else
        difference_a(3)  <= '1';
      end if;
    end if;
  end process;
  
  mark_new_tc0: process
  begin
    wait on testcase;
    mark_new_testcase0 <= '1';
    wait until (count_reseted0 = '1');
    mark_new_testcase0 <= '0';
  end process;

  mark_new_tc1: process
  begin
    wait on testcase;
    mark_new_testcase1 <= '1';
    wait until (count_reseted1 = '1');
    mark_new_testcase1 <= '0';
  end process;

  mark_new_tc2: process
  begin
    wait on testcase;
    mark_new_testcase2 <= '1';
    wait until (count_reseted2 = '1');
    mark_new_testcase2 <= '0';
  end process;

  mark_new_tc3: process
  begin
    wait on testcase;
    mark_new_testcase3 <= '1';
    wait until (count_reseted3 = '1');
    mark_new_testcase3 <= '0';
  end process;

END ARCHITECTURE behavior;

