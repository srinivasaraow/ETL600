--------------------------------------------------------------------------------
-- $Workfile: tb_dt_main_pkg.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_dt_main_pkg.vhd $
-- 
-- 4     11-08-29 9:49 Chstrue
-- -- no description --
-- 
-- 3     11-07-06 8:46 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
PACKAGE tb_dt_main_pkg IS
-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation
--   110802: Corrections for single run pass

 

  
  type t_AF_model_snk_data is array (1 to 12) of std_logic_vector(0 to 3199);
  type t_TDM_model_voice_data is array (0 to 15) of std_logic_vector(0 to 66);
   
  type t_AF_model_snk_config is record
    enable  : std_logic;
    ser_length        : integer range 0 to 3199;      -- length of serial digital data
    voice_ch_en       : std_logic_vector(0 to 15);    -- voice channel enable
    voice_ch_rate     : std_logic_vector(0 to 15);    -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
  end record;
   
  type t_AF_model_snk_status is record
    preamble_detected : std_logic;
    RX_data           : t_AF_model_snk_data;
    frame_captured    : std_logic;
    rec_data_ser      : std_logic_vector(0 to 3199);  -- received data serial
    rec_voice_data    : t_TDM_model_voice_data;
  end record;
   
  type t_AF_model_src_config is record
    enable            : std_logic;
    ser_length        : integer range 0 to 3199;      -- length of serial digital data
    voice_ch_en       : std_logic_vector(0 to 15);    -- voice channel enable
    voice_ch_rate     : std_logic_vector(0 to 15);    -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    pattern           : std_logic_vector(7 downto 0); -- fix pattern (1 byte after subframe header)
  end record;
   
  type t_AF_model_src_status is record
    frame_started     : std_logic;
    frame_finished    : std_logic;
    sent_data         : std_logic_vector(0 to 3199);
    sent_data_ser     : std_logic_vector(0 to 3199);
    sent_voice_data   : t_TDM_model_voice_data;
  end record;
  
  type t_TDM_model_config is record
    enable            : std_logic;
    voice_ch_en       : std_logic_vector(0 to 15);    -- voice channel enable
    voice_ch_rate     : std_logic_vector(0 to 15);    -- data rate of the voice channel (0: 5.6k, 1: 6.7k)
    voice_data_src    : t_TDM_model_voice_data;       -- voice data to send to FPGA (64 or 53 bits, subframe header is automatically added, fill rest of vector with '0')
    pattern           : std_logic_vector(0 to 7);     -- fix pattern (1 byte after subframe header)
  end record;
  
  type t_TDM_model_status is record
    subframe_sent     : std_logic;
    sent_voice_data   : t_TDM_model_voice_data;     -- sent voice data to the FPGA (1st 3 bits: subframe header, 8 bits: fix pattern, 8 bits: frame counter
    voice_data_snk    : t_TDM_model_voice_data;     -- voice data received from FPGA (1st 3 bits: subframe header)
    voice_data_valid  : std_logic_vector(0 to 3);   -- data valid for ch 1-4 / 5-8 / 9-12 / 13-16
  end record;  
  
  
  type adress_array is array (0 to 10) of std_logic_vector(11 downto 0);
  type adress_array_TDM is array (0 to 15) of std_logic_vector(11 downto 0);
  
  constant BRR_AD   : adress_array      := ("000000010111","000000011100","000000100001","000000100110","000000101011","000000110000","000000110101","000000111010","000000111111","000001000100","000001001001");
  constant CCR_AD   : adress_array      := ("000000010101","000000011010","000000011111","000000100100","000000101001","000000101110","000000110011","000000111000","000000111101","000001000010","000001000111");
  constant PCR_AD   : adress_array      := ("000000010100","000000011001","000000011110","000000100011","000000101000","000000101101","000000110010","000000110111","000000111100","000001000001","000001000110");
  constant TCR_AD   : adress_array_TDM  := ("000001010000","000001010001","000001010010","000001010011","000001010100","000001010101","000001010110","000001010111","000001011000","000001011001","000001011010","000001011011","000001011100","000001011101","000001011110","000001011111");
  constant BRT56    : std_logic_vector(7 downto 0)   :=  X"B8";
  constant BRT67    : std_logic_vector(7 downto 0)   :=  X"C3";
  constant GDTC     : std_logic_vector(11 downto 0)  :=  "0000" & X"61";
  constant VCE0     : std_logic_vector(11 downto 0)  :=  "0000" & X"62";
  constant VCE1     : std_logic_vector(11 downto 0)  :=  "0000" & X"63";
  constant VCTL0    : std_logic_vector(11 downto 0)  :=  "0000" & X"64";
  constant VCTL1    : std_logic_vector(11 downto 0)  :=  "0000" & X"65";
  constant VCTP0    : std_logic_vector(11 downto 0)  :=  "0000" & X"66";
  constant VCTP1    : std_logic_vector(11 downto 0)  :=  "0000" & X"67";
  constant VCTP2    : std_logic_vector(11 downto 0)  :=  "0000" & X"68";
  constant VCTP3    : std_logic_vector(11 downto 0)  :=  "0000" & X"69";
  constant VCTP4    : std_logic_vector(11 downto 0)  :=  "0000" & X"6A";
  constant VCTP5    : std_logic_vector(11 downto 0)  :=  "0000" & X"6B";
  constant VCTP6    : std_logic_vector(11 downto 0)  :=  "0000" & X"6C";
  constant VCTP7    : std_logic_vector(11 downto 0)  :=  "0000" & X"6D";

 
  
    --uC: 32.768MHz
  constant c_clock_uC_zero_cycle                : time;
  constant c_clock_uC_one_cycle                 : time;
  constant c_clock_uC_cycle                     : time;
  constant c_clock_uC_zero_cycle_s1             : time;
  constant c_clock_uC_one_cycle_s1              : time;
  constant c_clock_uC_cycle_s1                  : time;
  constant c_clock_uC_zero_cycle_s1_overspeed   : time;
  constant c_clock_uC_zero_cycle_s1_underspeed  : time;
  constant c_clock_uC_one_cycle_s1_overspeed    : time;
  constant c_clock_uC_one_cycle_s1_underspeed   : time;
  constant c_clock_uC_cycle_s1_overspeed        : time;
  constant c_clock_uC_cycle_s1_underspeed       : time;
  
  --MPI.
  constant MPI_waitstates             : integer             :=  3;
  
  
  type t_MPI_access is (MPI_idle, MPI_FPGA_read, MPI_FPGA_write);

  type t_MPI_Req is record
    Req         : std_logic;
    Trans       : t_MPI_access;
    Addr        : std_logic_vector(11 downto 0);
    Write_data  : std_logic_vector(7 downto 0);
  end record;
        
  type t_MPI_Ack is record
    Ack         : std_logic;
    Read_data   : std_logic_vector(7 downto 0);
  end record;
  
  
  
END tb_dt_main_pkg;
