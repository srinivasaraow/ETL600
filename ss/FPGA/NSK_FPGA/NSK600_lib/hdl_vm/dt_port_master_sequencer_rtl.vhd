--------------------------------------------------------------------------------
-- $Workfile   : $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : When the deframer starts with a new frame, the DT transmission 
--               sequence is started in the master. The transmission sequence
--               consists of 432 bytes, capable to transit a complete agregate
--               frame of 3200kbit/s.
--               The trigger signal from the deframer is supervised. If it fails, 
--               the unit starts to generate triggers itself with a periode of
--               app 10ms.
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_port_master_sequencer_rtl.vhd $
-- 
-- 4     11-08-29 9:25 Chstrue
-- Master: Timeout implemented for dt burst generation
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation

-----------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_port_master_sequencer IS
   PORT( 
      btransfer_done              : IN     std_logic;
      clk                         : IN     std_logic;
      reset_n                     : IN     std_logic;
      voice_ch_nbr                : OUT    std_logic_vector (3 DOWNTO 0);
      voice_ch_byte_nbr           : OUT    std_logic_vector (3 DOWNTO 0);
      prep_data                   : OUT    std_logic;
      voice_seq_ongoing           : OUT    std_logic;
      dt_start_sequence           : IN     std_logic;
      voice_burst_started_master  : OUT    std_logic;
      voice_burst_finished_master : OUT    std_logic;
      deframer_timeout            : OUT    std_logic
   );

-- Declarations

END dt_port_master_sequencer ;

--
ARCHITECTURE rtl OF dt_port_master_sequencer IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation
--   110715: Start trigger supervision implemented


  signal byte_counter         : unsigned(8 downto 0);
  signal sequence_ongoing     : std_logic;
  signal voice_seq_ongoing_i  : std_logic;
  signal voice_ch_nbr_i       : unsigned(3 downto 0);
  signal voice_ch_byte_nbr_i  : unsigned(3 downto 0);
  --start signal from supervision:
  signal start_seq_sved       : std_logic;
  signal deframer_running     : std_logic;
  signal trig_counter         : unsigned(18 downto 0);
BEGIN
  
  voice_seq_ongoing <= voice_seq_ongoing_i;
  voice_ch_nbr      <= std_logic_vector(voice_ch_nbr_i);
  voice_ch_byte_nbr <= std_logic_vector(voice_ch_byte_nbr_i);
  
  --byte_nbr <= std_logic_vector(byte_counter);
  
  deframer_timeout <= not(deframer_running);
  
  --generating a trigger all appr. 10ms
    
  trigger_supervision: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      trig_counter     <= "1011111111111111111"; --makes more than 10ms
      start_seq_sved   <= '0';
      deframer_running <= '0';
    elsif (CLK'event and CLK = '1') then
      
      --default:
      start_seq_sved   <= '0';
      
      if dt_start_sequence = '1' then
        start_seq_sved   <= '1';
        trig_counter     <= "1011111111111111111"; --makes more than 10ms, for time out
        deframer_running <= '1';
      else
        if deframer_running = '1' then
          if trig_counter = 0 then
            deframer_running <= '0';
            trig_counter     <= "1001111111111111011";  --makes 10ms
            start_seq_sved   <= '1';
          else
            trig_counter     <= trig_counter - 1;
          end if;
        else  --deframer not running:
          if trig_counter = 0 then
            trig_counter     <= "1001111111111111011";  --makes 10ms
            start_seq_sved   <= '1';
          else
            trig_counter     <= trig_counter - 1;
          end if;
        end if;
      end if;
    end if;
  end process;
  
  
  
  sequencer: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      byte_counter         <= (others => '0');
      voice_ch_nbr_i       <= (others => '0');
      voice_ch_byte_nbr_i  <= (others => '0');
      sequence_ongoing     <= '0';
      voice_seq_ongoing_i  <= '0';
      prep_data            <= '0';
      voice_burst_started_master  <= '0';
      voice_burst_finished_master <= '0';
    elsif (CLK'event and CLK = '1') then
      --default:
      prep_data            <= '0';
      voice_burst_started_master  <= '0';
      voice_burst_finished_master <= '0';
      if sequence_ongoing = '0' then
        if start_seq_sved = '1' then
          prep_data        <= '1';
          sequence_ongoing <= '1';
          byte_counter     <= (others => '0');
        end if;
      else
        if byte_counter = 431 and btransfer_done = '1' then 
          sequence_ongoing     <= '0';
          voice_seq_ongoing_i  <= '0';
          voice_burst_finished_master <= '1';
        elsif btransfer_done = '1' then
          if voice_seq_ongoing_i = '0' then
            if byte_counter = 287 then
              voice_burst_started_master <= '1';
              voice_seq_ongoing_i <= '1';
              voice_ch_nbr_i      <= (others => '0');
              voice_ch_byte_nbr_i <= (others => '0');
            end if;
          else
            prep_data    <= '1';
            if voice_ch_byte_nbr_i = 8 then
              voice_ch_nbr_i      <= voice_ch_nbr_i + 1;
              voice_ch_byte_nbr_i <= (others => '0');
            else
              voice_ch_byte_nbr_i <= voice_ch_byte_nbr_i +1;
            end if;
          end if;
          prep_data    <= '1';
          byte_counter <= byte_counter + 1;
        end if;
      end if;
    end if;
  end process;
  
  
END ARCHITECTURE rtl;

