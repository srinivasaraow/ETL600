--------------------------------------------------------------------------------
-- $Workfile: tb_tdm_model_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_tdm_model_behave.vhd $
-- 
-- 2     11-07-06 9:06 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------






LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.tb_dt_main_pkg.all;
USE DIG_TRANSIT_tb.basic_tb_pkg.all;

ENTITY tb_TDM_model IS
   PORT( 
      clk_32M           : IN    std_logic;
      TDM_model_config  : IN    t_TDM_model_config;
      TDM_model_status  : OUT   t_TDM_model_status;
      tdm_fs            : OUT   std_logic;
      tdm_data          : INOUT std_logic
   );
   
constant DEF_SLOT_RX: integer :=  69;
constant DEF_SLOT_TX: integer :=  74;

END tb_TDM_model;

--
ARCHITECTURE behave OF tb_TDM_model IS

-----------------------------------------------------------
-- Last edited:
--   110223: dsg@zuehlke.com: File Generated
--   110630: R4 implementation, 
--           clk_count_int_10ms init vlaue adapted for page switch provocation


  type t_header_nr is array (0 to 3) of integer range 1 to 3;
  type t_voice_channels is array (0 to 15) of std_logic_vector(0 to 855);
  type t_bytecounter_rx is array (0 to 3) of integer range 0 to 107;

  signal TDM_model_status_i : t_TDM_model_status;
  signal generated_tx_data  : t_voice_channels;
  signal received_data      : t_voice_channels;
  signal clk_count_int_10ms : integer range 0 to 327680;
  signal clk_count_int_93us : integer range 0 to 3072;
  signal subframe_count_tx  : t_header_nr;              -- subframe header of each board when sending
  signal bytecounter_tx     : integer range 0 to 107;   -- current byte in 93us slot, max. 108 bytes in 10ms
  signal bytecounter_rx     : t_bytecounter_rx;         -- current byte in 93us slot while receiving (for each board)
  signal cnt_tdmslot        : integer range 0 to 96;    -- current timeslot
  signal cnt_tdmslotbit     : integer range 0 to 31;    -- current bit in timeslot
  signal cnt_stream_tx      : integer range 0 to 15;    -- voice channel selecting during sending
  signal cnt_stream_rx      : integer range 0 to 15;    -- voice channel selecting during receiving
  signal cnt_stream_bit_tx  : integer range 0 to 855;   -- bitcounter of TX-Data during sending
  signal cnt_stream_bit_rx  : integer range 0 to 855;   -- bitcounter of RX-Data while receiving
  signal board              : integer range 0 to 3;     -- board selection while receiving
  signal frame_counter      : integer range 0 to 255;   -- frame count since last reset (visible in data stream)

  FUNCTION generate_bitstream(TDM_model_config_i: t_TDM_model_config; subframe_count_tx_int: t_header_nr; frame_counter_int: integer)
    RETURN t_voice_channels IS VARIABLE data: t_voice_channels;
  BEGIN
    for i in 0 to 15 loop

      data(i) := (others => '0');   -- empty all streams

      if TDM_model_config_i.voice_ch_en(i) = '1' then

        if subframe_count_tx_int(i / 4) = 1 then                                      -- header information for subframe
          data(i)(0 to 2) := "010";
        else
          data(i)(0 to 2) := "101";          
        end if;
        
        data(i)(3 to 10)  := TDM_model_config_i.pattern(0 to 7);                      -- add fix pattern
        data(i)(11 to 18) := std_logic_vector(conv_unsigned(frame_counter_int, 8));   -- frame counter

        if TDM_model_config_i.voice_ch_rate(i) = '0' then                             -- data
          data(i)(19 to 55) := TDM_model_config_i.voice_data_src(i)(0 to 28) & std_logic_vector(conv_unsigned(frame_counter_int, 8));
        else
          data(i)(19 to 66) := TDM_model_config_i.voice_data_src(i)(0 to 39) & std_logic_vector(conv_unsigned(frame_counter_int, 8));
        end if;       

      end if;
    end loop;
    
  	RETURN data;
  END generate_bitstream;

BEGIN
  frame_10ms_proc : process (clk_32M, TDM_model_config.enable)
  begin

-- please see description in document "TN-PSND11-1541 Digital voice transit testbench.doc, chapter 6.1.4" for a detailed explanation

    if (TDM_model_config.enable = '0') then
      TDM_model_status_i.subframe_sent    <= '0';
      TDM_model_status_i.voice_data_snk   <= (others => (others => '0'));
      TDM_model_status_i.voice_data_valid <= (others => '0');
      clk_count_int_10ms                  <= 10000;
      clk_count_int_93us                  <= 0;
      bytecounter_tx                      <= 0;
      subframe_count_tx                   <= (1, 2, 3, 2);
      frame_counter                       <= 0;
      tdm_fs                              <= '0';
      tdm_data                            <= 'Z';
      cnt_tdmslotbit                      <= 31;
      generated_tx_data                   <= generate_bitstream(TDM_model_config, subframe_count_tx, frame_counter);
      received_data                       <= (others => (others => '0'));
      bytecounter_rx                      <= (107, 107, 107, 107);
      cnt_stream_rx                       <= 0;
      cnt_stream_bit_rx                   <= 0;

    elsif clk_32M'event and clk_32M='1' then
      if clk_count_int_10ms = 0 then                   -- generate new TDM-streams all 10ms
        generated_tx_data <= generate_bitstream(TDM_model_config, subframe_count_tx, frame_counter);
        clk_count_int_10ms <= clk_count_int_10ms + 1;
        TDM_model_status_i.subframe_sent <= '0';
      elsif clk_count_int_10ms = 327679 then           -- after sending TDM-stream set variables for new stream
        clk_count_int_10ms <= 0;
        frame_counter <= frame_counter + 1;
        for i in 0 to 3 loop
          if subframe_count_tx(i) = 3 then
            subframe_count_tx(i) <= 1;
          else
            subframe_count_tx(i) <= subframe_count_tx(i) + 1;
          end if;
        end loop;
        TDM_model_status_i.subframe_sent <= '1';
      else
        clk_count_int_10ms <= clk_count_int_10ms + 1;
        TDM_model_status_i.subframe_sent <= '0';
      end if;

      clk_count_int_93us <= (clk_count_int_93us + 1) MOD 3072;
      
      if clk_count_int_93us = 0 then
        tdm_fs <= '1';
        if clk_count_int_10ms <= 3071 then
          bytecounter_tx <= 0;
        else
          bytecounter_tx <= bytecounter_tx + 1;
        end if; 
      else
        tdm_fs <= '0';
      end if;
      
      cnt_tdmslot     <= clk_count_int_93us / 32;                       -- current time slot in a TDM frame of 93us
      cnt_tdmslotbit  <= (cnt_tdmslotbit + 1) MOD 32;                   -- current bit-position (0..31, 0..31, ...)

      if (clk_count_int_93us >= 32 * DEF_SLOT_RX) and (clk_count_int_93us < 32 * (DEF_SLOT_RX + 5)) then  -- in timeslot 69 - 73
        for i in 0 to 3 loop
          if (clk_count_int_93us = 32 * DEF_SLOT_RX + 31 - i) then      -- 'start of subframe'-detection of each board
            if (tdm_data = '1') then
              bytecounter_rx(i) <= 0;
              TDM_model_status_i.voice_data_valid(i) <= '1';
            else
              bytecounter_rx(i) <= (bytecounter_rx(i) + 1) MOD 108 ;
              TDM_model_status_i.voice_data_valid(i) <= '0';
            end if;
          end if;
        end loop;

        if (clk_count_int_93us >= 32 * (DEF_SLOT_RX + 1)) and (clk_count_int_93us < 32 * (DEF_SLOT_RX + 5)) then  -- in timeslot 70 - 73
          -- cnt_stream_rx:     temp signal that shows the assigned voice channel
          cnt_stream_rx       <= (((((clk_count_int_93us) / 32) - (DEF_SLOT_RX+1)) * 4 ) + ( ((30-cnt_tdmslotbit) MOD 32) / 8)) MOD 16;
          -- board:             temp signal that shows the current selected board (board 0 in timeslot 70, etc.)
          board               <= (((clk_count_int_93us) - 32 * (DEF_SLOT_RX + 1)) / 32 ) MOD 4;
          -- cnt_stream_bit_rx: temp signal that shows the current bit position in the data vector
          cnt_stream_bit_rx   <= (bytecounter_rx((((clk_count_int_93us) - 32 * (DEF_SLOT_RX + 1)) / 32 ) MOD 4) * 8 + (((cnt_tdmslotbit+1) MOD 32) MOD 8)) MOD 856;
          -- receive data(current voice channel)(bit position):
          received_data((((((clk_count_int_93us) / 32) - (DEF_SLOT_RX+1)) * 4 ) + ( ((30-cnt_tdmslotbit) MOD 32) / 8)) MOD 16)((bytecounter_rx((((clk_count_int_93us) - 32 * (DEF_SLOT_RX + 1)) / 32 ) MOD 4) * 8 + (((cnt_tdmslotbit+1) MOD 32) MOD 8)) MOD 856) <= tdm_data;
        end if;        
      end if;
      
      if (clk_count_int_93us >= 32 * DEF_SLOT_TX) and (clk_count_int_93us < 32 * (DEF_SLOT_TX + 5)) then    -- in timeslot 74-78
        if (clk_count_int_93us >= 32 * DEF_SLOT_TX) and (clk_count_int_93us < 32 * (DEF_SLOT_TX + 1)) then  -- in timeslot 74
          if (clk_count_int_93us >= 32 * DEF_SLOT_TX + 28) and (clk_count_int_93us <= 32 * DEF_SLOT_TX + 31) then -- bits 0..3
            if bytecounter_tx = 0 then
              tdm_data <= '1';
            else
              tdm_data <= '0';
             end if;
          else
            tdm_data <= '0';
          end if;      
        else                                                                                                -- in timeslot 75-78
          -- cnt_stream_tx:     temp signal that shows the assigned voice channel
          cnt_stream_tx   <= ((((clk_count_int_93us / 32) - (DEF_SLOT_TX+1)) * 4 ) + ( ((30-cnt_tdmslotbit) MOD 32) / 8)) MOD 16;
          -- cnt_stream_bit_rx: temp signal that shows the current bit position in the data vector
          cnt_stream_bit_tx <= bytecounter_tx * 8 + (((cnt_tdmslotbit+1) MOD 32) MOD 8);
          -- send data      (current voice channel)(bit position):
          tdm_data <= generated_tx_data(((((clk_count_int_93us / 32) - (DEF_SLOT_TX+1)) * 4 ) + ( ((30-cnt_tdmslotbit) MOD 32) / 8)) MOD 16)(bytecounter_tx * 8 + (((cnt_tdmslotbit+1) MOD 32) MOD 8)); 
        end if;
      else
        tdm_data <= 'Z';
      end if;
    
    for i in 0 to 15 loop
      TDM_model_status_i.voice_data_snk(i)(0 to 66) <= received_data(i)(0 to 66);                          -- received data in vector(67...855) are not used   
      TDM_model_status_i.sent_voice_data(i)(0 to 66) <= generated_tx_data(i)(0 to 66);                     -- sent data on the TDM bus
    end loop;
    
    TDM_model_status <= TDM_model_status_i;
    
  end if;
  
  end process frame_10ms_proc;

END behave;

