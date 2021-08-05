--------------------------------------------------------------------------------
-- $Workfile: tb_AF_snk_model_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_AF_snk_model_behave.vhd $
-- 
-- 3     11-07-05 17:12 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_arith.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.tb_dt_main_pkg.all;
USE DIG_TRANSIT_tb.basic_tb_pkg.all;

ENTITY tb_AF_snk_model IS
   PORT( 
      AF_snk_config : IN    t_AF_model_snk_config;
      AF_snk_status : OUT   t_AF_model_snk_status;
      sport0_sclk   : IN    std_logic;
      sport0_d      : IN    std_logic
   );
   
constant DEF_PREAMB: std_logic_vector(8 downto 0) :=  "010110011";    -- for detecting the preamble, MSB first

END tb_AF_snk_model ;

--
ARCHITECTURE behave OF tb_AF_snk_model IS

-----------------------------------------------------------
-- Last edited:
--   110223: dsg@zuehlke.com: File Generated
--   110630: R4 implementation

  signal bitcount         : integer range 0 to 3199 := 0;
  signal framecount       : integer range 1 to AF_snk_status.RX_data'length := 1;
  signal preamb_searching : std_logic := '1';
  signal AF_snk_status_i  : t_AF_model_snk_status;

FUNCTION split_bitstream(AF_snk_config_i: t_AF_model_snk_config; data_i: std_logic_vector)    -- function to extract 16 single voice channels from the received aggregate frame
  RETURN t_TDM_model_voice_data IS VARIABLE voice_data: t_TDM_model_voice_data;
  variable temp_count     : integer range 0 to 3199;
BEGIN
  temp_count := AF_snk_config_i.ser_length + 12;
  voice_data := (others => (others => '0'));
  
  for i in 0 to 15 loop
    if AF_snk_config_i.voice_ch_en(i) = '1' then
      if AF_snk_config_i.voice_ch_rate(i) = '0' then
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

bit_sample_proc : process (sport0_sclk, AF_snk_config.enable)
begin
  if AF_snk_config.enable = '0' then                     -- reset
    AF_snk_status_i.RX_data <= (others => (others => '0') );
    AF_snk_status_i.preamble_detected <= '0';
    AF_snk_status_i.frame_captured <= '0';
    preamb_searching  <= '1';
    bitcount          <= 0;
    framecount        <= 1;
  elsif sport0_sclk'event and sport0_sclk = '0' then     -- sampling on falling edge
    if preamb_searching = '1' then
      if bitcount < 9 then                               -- when shift register empty (less than 8 bits received) filling up of RX_data for the first 8 bits
        AF_snk_status_i.RX_data(1)(bitcount) <= sport0_d;
        bitcount <= bitcount + 1;
        AF_snk_status_i.preamble_detected <= '0';
      elsif (AF_snk_status_i.RX_data(1)(0 to 8) = DEF_PREAMB(8 downto 0)) then  -- checking of preamble
        AF_snk_status_i.RX_data(1)(bitcount) <= sport0_d;
        bitcount <= bitcount + 1;
        AF_snk_status_i.preamble_detected <= '1';
        preamb_searching <= '0';
      else                                                -- if no preamble -> shift register to left (leave bitcounts = 9), waiting for a correct preamble (start of aggregate frame)
        AF_snk_status_i.RX_data(1)(0 to 8) <= AF_snk_status_i.RX_data(1)(1 to 8) & sport0_d;
        AF_snk_status_i.preamble_detected <= '0';
      end if;    
            
    else                                                  -- preamble detected: just filling the serial data into a register
      AF_snk_status_i.RX_data(framecount)(bitcount) <= sport0_d;
      if bitcount = 3199 then
        bitcount <= 0;
        if framecount = AF_snk_status_i.RX_data'length then
        framecount <= 1;
        else
        framecount <= framecount + 1;
        end if;
      else
        bitcount <= bitcount + 1;
      end if;
  
      if (bitcount = 9) and (AF_snk_status_i.RX_data(framecount)(0 to 8) = DEF_PREAMB(8 downto 0)) then
        AF_snk_status_i.preamble_detected <= '1';
      else
        AF_snk_status_i.preamble_detected <= '0';
      end if;
      
      if (bitcount = 3199) then                           -- complete frame captured (data may be available earlier when sending less serial data than 3200 bits)
        AF_snk_status_i.frame_captured <= '1';
      else
        AF_snk_status_i.frame_captured <= '0';
      end if;
    end if;      

    -- for debugging:
      AF_snk_status_i.rec_data_ser(0 to (AF_snk_config.ser_length + 3)) <= AF_snk_status_i.RX_data(framecount)(9 to (AF_snk_config.ser_length + 12));
      AF_snk_status_i.rec_data_ser((AF_snk_config.ser_length + 4) to 3199) <= AF_snk_status_i.RX_data(framecount)((AF_snk_config.ser_length + 13) to 3199) & "000000000";
      AF_snk_status_i.rec_voice_data <= split_bitstream(AF_snk_config, AF_snk_status_i.RX_data(framecount));  -- extract voice data in single channels
      
  end if;
  
  AF_snk_status <= AF_snk_status_i;
  
end process bit_sample_proc;
  
END behave;

