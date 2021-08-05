--------------------------------------------------------------------------------
-- $Workfile   : $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : Test functions for integration testing. For more details
--               refer to doc 1KHW001487E_ MUX600 FPGA Design Description.doc


-- NOTE: TO DISABLE TESTFUNCTION: Set SIGNAL disable_dt_test_mode to '1' on upper level


--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_test_mode_rtl.vhd $
-- 
-- 4     11-07-06 9:40 Chstrue
-- R4 implementation

-----------------------------------------------------------



LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_test_mode IS
   PORT( 
      clk                       : IN    std_logic;
      reset_n                   : IN    std_logic;
      control_digtrans          : IN    t_control_digtrans;
      disable_dt_test_mode      : IN    std_logic;
      frame_start_def           : IN    std_logic;
      frame_start_enf           : IN    std_logic;
      dt_put_rx_data_bit        : IN    std_logic;
      frame_start_supp_dt_port  : OUT   std_logic;
      frame_start_def_dt_buffer : OUT   std_logic;
      frame_start_enf_dt_buffer : OUT   std_logic;
      disturb_dt_buffer         : OUT   std_logic
   );

-- Declarations

END dt_test_mode ;

--
ARCHITECTURE rtl OF dt_test_mode IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation

-- NOTE: TO DISABLE TESTFUNCTION: Set SIGNAL disable_dt_test_mode to '1' on upper level

  signal counter_frames     : integer range 0 to 500;
  signal frame_start_supp_i : std_logic;
  signal frame_start_defs_i : std_logic;
  signal frame_start_enfs_i : std_logic;
  signal counter_bits       : integer range 0 to 10000;
  signal disturb_i          : std_logic;

BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                             -- reset
    counter_frames      <= 0;
    frame_start_supp_i  <= '0';
    counter_bits        <= 0;
    disturb_i           <= '0';
    frame_start_defs_i  <= '0';
    frame_start_enfs_i  <= '0';
        
  elsif clk'event and clk = '1' then                -- running on rising edge

    if disable_dt_test_mode = '1' then              -- test mode disabled
      frame_start_supp_i <= frame_start_def;
      disturb_i          <= '0';
      frame_start_defs_i <= frame_start_def;
      frame_start_enfs_i <= frame_start_enf;

    else                                            -- test mode enabled
      if frame_start_def = '1' then
        counter_frames <= (counter_frames + 1) MOD 500;
      end if;
      
      -- suppress digital transit trigger of master
      if (control_digtrans.vce(14) = '1') and (control_digtrans.vce(15) = '0') and (control_digtrans.gdtc.mss = '1') then
        if counter_frames = 0 then
          frame_start_supp_i <= '0';
        else
          frame_start_supp_i <= frame_start_def;
        end if;
      else
        frame_start_supp_i <= frame_start_def;
      end if;

      -- suppress digital transit trigger in slave
      if (control_digtrans.vce(14) = '0') and (control_digtrans.vce(15) = '1') and (control_digtrans.gdtc.mss = '0') then
        if counter_frames = 0 then
          frame_start_defs_i  <= '0';
          frame_start_enfs_i  <= '0';
        else
          frame_start_defs_i <= frame_start_def;
          frame_start_enfs_i <= frame_start_enf;
        end if;
      else
        frame_start_defs_i <= frame_start_def;
        frame_start_enfs_i <= frame_start_enf;
      end if;

      -- insert bit errors in master or slave RX path of digital transit
      if dt_put_rx_data_bit = '1' then
        counter_bits <= (counter_bits + 1) MOD 10000;
      end if;
      
      if (control_digtrans.vce(12) = '1') and (control_digtrans.vce(13) = '0') then
        if (counter_bits MOD 100) = 0 then
          disturb_i  <= '1';
        else
          disturb_i  <= '0';
        end if;
      elsif (control_digtrans.vce(12) = '0') and (control_digtrans.vce(13) = '1') then
        if (counter_bits MOD 1000) = 0 then
          disturb_i  <= '1';
        else
          disturb_i  <= '0';
        end if;    
      elsif (control_digtrans.vce(10) = '1') and (control_digtrans.vce(11) = '0') then
        if counter_bits = 0 then
          disturb_i  <= '1';
        else
          disturb_i  <= '0';
        end if;  
      else
        disturb_i  <= '0';
      end if;

    end if;

  end if;

end process clocked;

  frame_start_supp_dt_port  <= frame_start_supp_i;
  disturb_dt_buffer         <= disturb_i;
  frame_start_def_dt_buffer <= frame_start_defs_i;
  frame_start_enf_dt_buffer <= frame_start_enfs_i;  

END rtl;