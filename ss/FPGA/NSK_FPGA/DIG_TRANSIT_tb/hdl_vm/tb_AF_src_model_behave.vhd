--------------------------------------------------------------------------------
-- $Workfile: tb_AF_src_model_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_AF_src_model_behave.vhd $
-- 
-- 3     11-07-05 17:13 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_arith.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.tb_dt_main_pkg.all;
USE DIG_TRANSIT_tb.basic_tb_pkg.all;

ENTITY tb_AF_src_model IS
   PORT( 
      AF_src_config : IN    t_AF_model_src_config;
      AF_src_status : OUT   t_AF_model_src_status;
      sport2_sclk   : IN    std_logic;
      sport2_d      : OUT   std_logic
   );

   constant DEF_PREAMB: std_logic_vector(8 downto 0) :=  "010110011";     -- preamble, MSB first
   type t_AF_src_int_bframe is array (0 to 3) of integer range 1 to 3;    
   type t_AF_src_int_tdm_data is array (0 to 15) of std_logic_vector(0 to 71);

END tb_AF_src_model ;

--
ARCHITECTURE behave OF tb_AF_src_model IS

-----------------------------------------------------------
-- Last edited:
--   110223: dsg@zuehlke.com: File Generated
--   110630: R4 implementation

  signal bitcount         : integer range 0 to 3199 := 0;                 -- current bit position of the aggregate stream
  signal AF_src_status_i  : t_AF_model_src_status;
  signal board_frame      : t_AF_src_int_bframe;                          -- current subframe of each O4CV board (are not subframe-synchronous to each other)
  signal frame_count      : integer range 0 to 255;                       -- current subframe counter since last enable of the AF_source

FUNCTION generate_bitstream(AF_src_config_i: t_AF_model_src_config; bf_int: t_AF_src_int_bframe; frame_count_int: integer range 0 to 255)
  RETURN std_logic_vector IS VARIABLE data: std_logic_vector(0 to 3199);
  variable serial_data    : std_logic_vector(0 to 3199);                  -- dummy data ahead of the voice data
  variable temp_data_int  : integer range 0 to 255 := 255;                -- current value of data byte (incremented every new byte)
  variable temp_data_slv  : std_logic_vector(7 downto 0);                 -- serialized bits of current data byte
  variable temp_tdm       : t_AF_src_int_tdm_data;
  variable temp_count     : integer range 0 to 3199;                      -- temporary counter
BEGIN
  
  for i in 0 to 399 loop            -- generate a serial data stream of 3200 bits (not all used), contents: "0102030405060708090A0B..."
    temp_data_int := ((temp_data_int + 1) MOD 256);
    temp_data_slv := conv_std_logic_vector(temp_data_int, 8);
    for j in 0 to 7 loop
      serial_data(i * 8 + j)  := temp_data_slv(7-j);
    end loop;
  end loop;
  
  for i in 0 to 15 loop        
    if bf_int(i/4) = 1 then         -- fill in TDM subframe marking, depending on current subframe of the O4CV-board
      temp_tdm(i)(0 to 2) := "010";
    else
      temp_tdm(i)(0 to 2) := "101";
    end if;

    temp_tdm(i)(3 to 10)  := AF_src_config_i.pattern(7 downto 0);

    temp_tdm(i)(11 to 18) := conv_std_logic_vector(frame_count_int, 8);
    
    for j in 0 to 5 loop            -- generate data: 6 x 8 bits = 48 (sometimes not all used), contents: "010203040506..."
      temp_data_int := ((temp_data_int + 1) MOD 256);
      temp_data_slv := conv_std_logic_vector(temp_data_int, 8);
      for k in 0 to 7 loop
        temp_tdm(i)(19 + j*8 + k)  := temp_data_slv(7-k);
      end loop;
    end loop;            
    
    if AF_src_config_i.voice_ch_en(i) = '0' then
      temp_tdm(i) := (others => '0');
    end if;
    
  end loop;

  -- assemble aggregate frame from preamble, serial data and voice channels (depending on the configuration) 
  data(0 to 3199) := (others => '0');
  data(0 to 8)    := DEF_PREAMB;
  data(9 to AF_src_config_i.ser_length + 8) := serial_data(0 to AF_src_config_i.ser_length - 1);
  data(AF_src_config_i.ser_length + 9) := '1';   -- overspeed indicator: true (SPED)
  data(AF_src_config_i.ser_length + 10) := '1';  -- 2 additional bits (ADD)
  data(AF_src_config_i.ser_length + 11) := '1';
  
  temp_count := AF_src_config_i.ser_length + 12;
  
  for i in 0 to 15 loop
    if AF_src_config_i.voice_ch_en(i) = '1' then
      if AF_src_config_i.voice_ch_rate(i) = '0' then
        for j in 0 to 55 loop                       -- fill in 56 bytes
          data(temp_count) := temp_tdm(i)(j);
          temp_count := temp_count + 1;
        end loop;
      else
        for j in 0 to 66 loop                       -- fill in 67 bytes
          data(temp_count) := temp_tdm(i)(j);
          temp_count := temp_count + 1;
        end loop;
      end if;
    end if;
  end loop;  
   
	RETURN data;
END generate_bitstream;

FUNCTION split_bitstream(AF_src_config_i: t_AF_model_src_config; data_i: std_logic_vector)    -- function to extract 16 single voice channels from the generated aggregate frame
  RETURN t_TDM_model_voice_data IS VARIABLE voice_data: t_TDM_model_voice_data;
  variable temp_count     : integer range 0 to 3199;
BEGIN
  
  temp_count := AF_src_config_i.ser_length + 12;
  voice_data := (others => (others => '0'));
  
  for i in 0 to 15 loop
    if AF_src_config_i.voice_ch_en(i) = '1' then
      if AF_src_config_i.voice_ch_rate(i) = '0' then
        for j in 0 to 55 loop                       -- fill in 56 bytes
          voice_data(i)(j) := data_i(temp_count);
          temp_count := temp_count + 1;
        end loop;
      else
        for j in 0 to 66 loop                       -- fill in 67 bytes
          voice_data(i)(j) := data_i(temp_count);
          temp_count := temp_count + 1;
        end loop;
      end if;
    end if;
  end loop;  
   
	RETURN voice_data;
END split_bitstream;

BEGIN  
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------

bit_send_proc : process (sport2_sclk, AF_src_config.enable)
begin

  if AF_src_config.enable = '0' then                     -- reset
    AF_src_status_i.frame_started <= '0';
    AF_src_status_i.frame_finished <= '0';
    AF_src_status_i.sent_data <= (others => '0');
    AF_src_status_i.sent_data_ser <= (others => '0');
    AF_src_status_i.sent_voice_data <= (others => (others => '0'));
    sport2_d <= '0';
    bitcount <= 0;
    board_frame <= (1, 2, 3, 1);
    frame_count <= 0;
    
  elsif sport2_sclk'event and sport2_sclk = '1' then     -- sending on rising edge

    if bitcount = 0 then
      frame_count <= frame_count + 1;
      AF_src_status_i.sent_data <= generate_bitstream(AF_src_config, board_frame, frame_count);                                       -- generate a new aggregate frame
      AF_src_status_i.sent_voice_data <= split_bitstream(AF_src_config, generate_bitstream(AF_src_config, board_frame, frame_count)); -- provide the separated voice data of the generated aggregate frame
      AF_src_status_i.frame_started <= '1';
      for i in 0 to 3 loop                        -- increments the board subframe header for each O4CV board
        if board_frame(i) = 3 then
          board_frame(i) <= 1;
        else
          board_frame(i) <= board_frame(i) + 1;
        end if;
      end loop;
    else
      AF_src_status_i.frame_started <= '0';
    end if;

    if bitcount = 3199 then                       -- last bit --> restart next bit
      AF_src_status_i.frame_finished <= '1';
      bitcount <= 0;
    else
      AF_src_status_i.frame_finished <= '0';      -- not first and not last bit
      bitcount <= bitcount + 1;
    end if;
  
    sport2_d <= AF_src_status_i.sent_data(bitcount);  
    
    -- for debugging:
      AF_src_status_i.sent_data_ser(0 to (AF_src_config.ser_length + 3)) <= AF_src_status_i.sent_data(9 to (AF_src_config.ser_length + 12));
      AF_src_status_i.sent_data_ser((AF_src_config.ser_length + 4) to 3199) <= AF_src_status_i.sent_data((AF_src_config.ser_length + 13) to 3199) & "000000000";

  end if;
  
  AF_src_status <= AF_src_status_i;
  
end process bit_send_proc; 
  
END behave;

