--------------------------------------------------------------------------
-- $Workfile: tb_if_handle_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_if_handle_behavior.vhd $
-- 
-- 39    11-07-06 9:20 Chstrue
-- R4 implementation
-- 
-- 38    08-04-23 15:20 Chalgyg
-- Removed dsp_flag0.
-- 
-- 37    26.01.06 15:54 Chalgyg
-- G.703 LOS signal path testing added.
-- 
-- 36    19.01.06 16:17 Chalgyg
-- G.703 interface integrated.
-- 
-- 35    10.01.06 9:06 Chalgyg
-- LAN interface is now supported.
-- 
-- 34    16.06.05 10:40 Chalgyg
-- Signal configured is resetted after start of TB.
-- 
-- 33    25.02.05 14:07 Chalgyg
-- Some cases merged.
-- 
-- 32    15.02.05 11:56 Chalgyg
-- Bug removed.
-- 
-- 31    10.02.05 15:49 Chalgyg
-- Switched to DSP interface communication with address bus.
-- 
-- 30    19.01.05 15:33 Chalgyg
-- loop_mux case added.
-- 
-- 29    18.01.05 10:13 Chalgyg
-- The DUT will be configured in V.11 mode.
-- 
-- 28    14.01.05 11:05 Chalgyg
-- Preparation for future bitrate testcase commented out.
-- 
-- 27    23.12.04 11:15 Chalgyg
-- - Bug removed
-- - fs_jitter added
-- 
-- 26    21.12.04 9:26 Chalgyg
-- Signal new_data added.
-- 
-- 25    17.12.04 13:16 Chalgyg
-- Testcase real_out added.
-- 
-- 23    13.12.04 13:51 Chalgyg
-- reset value of data_out inverted.
-- 
-- 22    8.12.04 15:16 Chalgyg
-- Testcase 18 fixed.
-- 
-- 21    8.12.04 13:56 Chalgyg
-- Case Loop V.24 added.
-- 
-- 20    8.12.04 13:30 Chalgyg
-- Port sharing device case fixed.
-- 
-- 19    8.12.04 10:52 Chalgyg
-- Reset DUT after each testcase.
-- 
-- 18    7.12.04 14:25 Chalgyg
-- Disable V.24 ports between testcases.
-- 
-- 17    6.12.04 15:26 Chalgyg
-- case in_adpsk writes 0xFF to RUDn in every cycle.
-- 
-- 16    3.12.04 12:54 Chalgyg
-- Loop all channels in sharing device mode.
-- 
-- 15    2.12.04 11:03 Chalgyg
-- Made Flag 0 independent of case structure.
-- 
-- 14    24.11.04 10:22 Gygax02
-- RUD included in configuration of each testcase.
-- 
-- 12    7.10.04 17:09 Gygax02
-- -- no description --
-- 
-- 11    20.09.04 10:36 Unp00631
-- -- no description --
-- 
-- 10    19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
-- Last edited:
--    -110218, StR:
--         -Use of TDM_FS for generation of flag0_int in DSP IF handler unit





LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

library nsk600_tb;
use nsk600_tb.tb_dsp_if_p.all;
use nsk600_tb.tb_config_p.all;
use nsk600_tb.tb_real_data_p.all;

ENTITY tb_if_handle IS
   PORT(
      testcase       : IN     t_testcase;
      one_error_found: IN     std_logic;
      data_in        : IN     t_channel_array;
      busy_muxed     : IN     std_logic;
      dsp_cs_n       : OUT    std_logic;
      dsp_rd_n       : OUT    std_logic;
      dsp_status     : OUT    std_logic_vector (1 DOWNTO 0);
      dsp_wr_n       : OUT    std_logic;
      data_out       : OUT    t_channel_array := ("11111111","11111111","11111111","11111111");
      nsk_adr        : OUT    std_logic_vector (11 DOWNTO 0);
      fpga_reset_n   : OUT    std_logic;
      configured     : OUT    std_logic;
      new_data       : OUT    std_logic;
      los            : OUT    std_logic;
      dsp_data       : INOUT  std_logic_vector (7 DOWNTO 0);
      TDM_FS         : IN     std_logic
   );
END ENTITY tb_if_handle ;

ARCHITECTURE behavior OF tb_if_handle IS
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


  signal dsp_sig        : t_dsp_port_sig;
  signal flag0_int      : std_logic;
  
--  wr_reg(X"060", B"00000000");
  
  
BEGIN

  dsp_cs_n    <= dsp_sig.dsp_cs_n;
  dsp_wr_n    <= dsp_sig.dsp_wr_n;
  dsp_rd_n    <= dsp_sig.dsp_rd_n;
  nsk_adr     <= dsp_sig.dsp_addr;
  
  handle_dsp_if: process
    variable failure_occured       : std_logic;
    variable failure_occured_global: std_logic;
    variable all_TUD               : t_channel_array;
    variable sig_input             : std_logic_vector(7 downto 0);
    variable los_register          : std_logic_vector(7 downto 0);
  begin
    dsp_status  <= "00";
    
    dsp_write(B"00000000",X"060",dsp_sig,dsp_data);
    
    case dps_if_modus(testcase) is
    when reset =>                              -- Reset FPGA.
      dsp_passive(dsp_data,dsp_sig);
      failure_occured_global := '0';
      configured             <= '0';
      los                    <= '1';           -- G.703 Interface detects LOS
      wait for 1 ns;
    when dsp_if_com =>                         -- Check Communication between DSP and FPGA
      dsp_check_comm(dsp_data,dsp_sig,failure_occured);
      failure_occured_global := failure_occured;
      wait for 1 ns;
    when loop_fsk|loop_p_share|loop_mixed =>   -- Check TXD to RXD loop in NSK mode.
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      if (busy_muxed = '0') then
        wait until busy_muxed = '1';
      end if;
      while (busy_muxed = '1') loop
        wait until (flag0_int = '1');          -- Wait for SPORT13 interrupt.
        wait for 8 us;
        dsp_read_all_TUD(dsp_data,all_TUD,dsp_sig);
        data_out  <= all_TUD;
        new_data  <= '1';
        wait for 1 ps;
        new_data  <= '0';
        dsp_write_all_RUD(data_in,dsp_sig,dsp_data);
      end loop;
      failure_occured_global  := one_error_found;
      wait until (flag0_int = '1');
      wait for 1 ns;
    when out_adpsk =>                          -- Check RXD path in ADPSK mode
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      if (busy_muxed = '0') then
        wait until busy_muxed = '1';
      end if;
      while (busy_muxed = '1') loop
        wait until (flag0_int = '1');
        wait for 8 us;
        dsp_write_all_RUD(data_in,dsp_sig,dsp_data);
      end loop;
      failure_occured_global := '0';
      wait until (flag0_int = '1');
      wait for 1 ns;
    when real_out =>                           -- Check RXD path in ADPSK mode
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      for i in 0 to 99 loop
        wait until (flag0_int = '1');
        wait for 8 us;
        dsp_write(real_rxd(99-i),RUD_AD(0),dsp_sig,dsp_data);
      end loop;
      failure_occured_global := '0';
      wait until (flag0_int = '1');
      wait for 1 ns;
    when in_adpsk =>                           -- Check TXD path in ADPSK mode
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      if (busy_muxed = '0') then
        wait until busy_muxed = '1';
      end if;
      while (busy_muxed = '1') loop
        wait until (flag0_int = '1');
        wait for 8 us;
        dsp_read_all_TUD(dsp_data,all_TUD,dsp_sig);
        data_out  <= all_TUD;
        new_data  <= '1';
        wait for 1 ps;
        new_data  <= '0';
        dsp_write_all_RUD(("11111111","11111111","11111111","11111111"),dsp_sig,dsp_data); -- Give 0xff out to RUD in order to guarantee right timing of TUD.
      end loop;
      failure_occured_global := one_error_found;
      wait until (flag0_int = '1');
      wait for 1 ns;
    when loop_rts =>                             -- Check RTS to CTS Loop.
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      if (busy_muxed = '0') then
        wait until busy_muxed = '1';
      end if;
      while (busy_muxed = '1') loop
        wait until (flag0_int = '1');
        wait for 8 us;
        dsp_read(TSIG_AD,dsp_data,sig_input,dsp_sig);
        dsp_write(("0000" & sig_input(3 downto 0)),RSIG_AD,dsp_sig,dsp_data);
      end loop;
      failure_occured_global := '0';
      wait until (flag0_int = '1');
      wait for 1 ns;
    when loop_v11|loop_mux_v11|loop_mux_v24|loop_lan => -- MUX mode exclusively.
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      if (busy_muxed = '0') then
        wait until busy_muxed = '1';
      end if;
      while (busy_muxed = '1') loop
        dsp_passive(dsp_data,dsp_sig);
        wait for 1 us;
      end loop;
      failure_occured_global  := '0';
      wait until (flag0_int = '1');
      wait for 1 ns;
    when loop_g703 =>     -- G.703 Interface needs special handling because of LOS signaling
      los  <= '0';
      dsp_configure_fpga(testcase,dsp_data,dsp_sig,configured);
      if (busy_muxed = '0') then
        wait until busy_muxed = '1';
      end if;
      while (busy_muxed = '1') loop
        dsp_passive(dsp_data,dsp_sig);
        wait for 1 us;
      end loop;
      dsp_read(OSR_AD(9),dsp_data,los_register,dsp_sig);
      if (los_register(1) = '0') then
        los  <= '1';
        wait for 100 us;
        dsp_read(OSR_AD(9),dsp_data,los_register,dsp_sig);
        if (los_register(1) = '1') then
          failure_occured_global  := '0';
        else
        failure_occured_global  := '1';
        assert (false)
          report "DSP interface indicates no G.703 LOS, although LOS condition exists."
          severity warning;
        end if;
      else
        failure_occured_global  := '1';
        assert (false)
          report "DSP interface indicates G.703 LOS, although no LOS condition exists."
          severity warning;
      end if;
      los  <= '1';
      wait until (flag0_int = '1');
      wait for 1 ns;
    when others =>
      dsp_passive(dsp_data,dsp_sig);
      wait for 500 ns;
    end case;
    dsp_passive(dsp_data,dsp_sig);
    reset_fpga(fpga_reset_n);
    if (failure_occured_global = '0') then     -- Pass failure state to tb_control.
      dsp_status  <= "10";
    else
      dsp_status  <= "11";
    end if;      
    wait on testcase;
    wait for 1 ps;
  end process;


flag0_int <= tdm_fs;


--  handle_flag0: process
--    variable tmin  : time := 10 ns;
--    variable tplus : time := 10 ns;
--  begin
--    for i in 0 to 4 loop
--      dsp_handle_flag0(flag0_int);             -- Imitates SPORT13 interrupt from DSP.
--      tmin  := 93.65 us - fs_jitter;
--      wait for tmin;
--    end loop;
--    for i in 0 to 4 loop
--      dsp_handle_flag0(flag0_int);             -- Imitates SPORT13 interrupt from DSP.
--      tplus := 93.65 us + fs_jitter;
--      wait for tplus;
--    end loop;
--  end process;

END ARCHITECTURE behavior;

