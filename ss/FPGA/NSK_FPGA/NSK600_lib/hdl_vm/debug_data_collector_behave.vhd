--------------------------------------------------------------------------------
-- $Workfile: debug_data_collector_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/debug_data_collector_behave.vhd $
-- 
-- 3     11-07-07 13:53 Chstrue
-- header added

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY debug_data_collector IS
   PORT( 
      btransfer_debug_done    : IN     std_logic;
      reset_n                 : IN     std_logic;
      clk                     : IN     std_logic;
      control_mux             : IN     t_control_mux;
      control_digtrans        : IN     t_control_digtrans;
      frame_start_enf         : IN     std_logic;
      cfg_local_transit_n_vec : IN     std_logic_vector (15 DOWNTO 0);
      tx_value_debug          : OUT    std_logic_vector (7 DOWNTO 0);
      status_digtrans         : IN     t_status_digtrans;
      start_transfer_debug    : OUT    std_logic
   );

-- Declarations

END debug_data_collector ;

--
ARCHITECTURE behave OF debug_data_collector IS
  
-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation  
  
  
  signal byte_cnt_i             : integer range 0 to 392;
  signal register_cnt_i         : integer range 80 to 119;
  signal tx_value_debug_i       : std_logic_vector(7 downto 0);
  signal start_transfer_debug_i : std_logic;
  signal current_data_i         : std_logic_vector(7 downto 0);
  signal transfer_in_progress_i : std_logic;

BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                                           -- reset
    byte_cnt_i              <= 0;
    register_cnt_i          <= 80;
    current_data_i          <= (others => '0');
    tx_value_debug_i        <= (others => '0');
    start_transfer_debug_i  <= '0';
    transfer_in_progress_i  <= '0';
    
  elsif clk'event and clk = '1' then                              -- running on rising edge

    if frame_start_enf  = '1' and byte_cnt_i = 392 then
      byte_cnt_i      <= 0;
      register_cnt_i  <= 80;
    end if;
    
    if btransfer_debug_done = '1' then
      transfer_in_progress_i  <= '0';
    end if;
    
    if byte_cnt_i < 390 then
      
      if transfer_in_progress_i = '0' then
        byte_cnt_i              <= byte_cnt_i + 1;
        transfer_in_progress_i  <= '1';
        
        if (byte_cnt_i MOD 10) = 0 then               -- send "R"
          tx_value_debug_i        <= x"52";
          start_transfer_debug_i  <= '1';
        end if;
        
        if (byte_cnt_i MOD 10) = 1 then               -- send address higher nibble
          tx_value_debug_i        <= "0011" & std_logic_vector(to_unsigned((register_cnt_i / 16), 4));
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 2 then               -- send address lower nibble
          if (register_cnt_i MOD 16) < 10 then
            tx_value_debug_i      <= "0011" & std_logic_vector(to_unsigned((register_cnt_i MOD 16), 4));
          else
            tx_value_debug_i      <= "0100" & std_logic_vector(to_unsigned((register_cnt_i MOD 16) - 9, 4));
          end if;
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 3 then               -- send ":"
          tx_value_debug_i        <= x"3A";
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 4 then               -- send " "
          tx_value_debug_i        <= x"20";
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 5 then               -- send value higher nibble
          if (to_integer(unsigned(current_data_i(7 downto 4)))) < 10 then
            tx_value_debug_i      <= "0011" & current_data_i(7 downto 4);
          else
            tx_value_debug_i      <= "0100" & std_logic_vector(to_unsigned(to_integer(unsigned(current_data_i(7 downto 4))) - 9, 4));
          end if;  
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 6 then               -- send value lower nibble
          if (to_integer(unsigned(current_data_i(3 downto 0)))) < 10 then
            tx_value_debug_i      <= "0011" & current_data_i(3 downto 0);
          else
            tx_value_debug_i      <= "0100" & std_logic_vector(to_unsigned(to_integer(unsigned(current_data_i(3 downto 0))) - 9, 4));
          end if;  
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 7 then               -- send " "
          tx_value_debug_i        <= x"20";
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 8 then               -- send "-"
          tx_value_debug_i        <= x"2D";
          start_transfer_debug_i  <= '1';
        end if;

        if (byte_cnt_i MOD 10) = 9 then               -- send " "
          tx_value_debug_i        <= x"20";
          start_transfer_debug_i  <= '1';
          register_cnt_i          <= register_cnt_i + 1;
        end if;

      else
        start_transfer_debug_i    <= '0';
      end if;
    end if;

    if byte_cnt_i = 390 then
      if (transfer_in_progress_i = '0') then
        byte_cnt_i                <= byte_cnt_i + 1;
        tx_value_debug_i          <= x"0D";             -- send CR
        start_transfer_debug_i    <= '1';
        transfer_in_progress_i    <= '1';
      else
        start_transfer_debug_i    <= '0';
      end if;
    end if;

    if byte_cnt_i = 391 then
      if (transfer_in_progress_i = '0') then
        byte_cnt_i                <= byte_cnt_i + 1;
        tx_value_debug_i          <= x"0A";             -- send LF
        start_transfer_debug_i    <= '1';
        transfer_in_progress_i    <= '1';
      else
        start_transfer_debug_i    <= '0';
      end if;
    end if;
    
    if byte_cnt_i = 392 then
      start_transfer_debug_i      <= '0';
    end if;
    
    case register_cnt_i is
      when 80     => current_data_i <= control_mux.mux_ch(11) & std_logic_vector(to_unsigned(control_mux.bitcounts(11), 7));
      when 81     => current_data_i <= control_mux.mux_ch(12) & std_logic_vector(to_unsigned(control_mux.bitcounts(12), 7));
      when 82     => current_data_i <= control_mux.mux_ch(13) & std_logic_vector(to_unsigned(control_mux.bitcounts(13), 7));
      when 83     => current_data_i <= control_mux.mux_ch(14) & std_logic_vector(to_unsigned(control_mux.bitcounts(14), 7));
      when 84     => current_data_i <= control_mux.mux_ch(15) & std_logic_vector(to_unsigned(control_mux.bitcounts(15), 7));
      when 85     => current_data_i <= control_mux.mux_ch(16) & std_logic_vector(to_unsigned(control_mux.bitcounts(16), 7));
      when 86     => current_data_i <= control_mux.mux_ch(17) & std_logic_vector(to_unsigned(control_mux.bitcounts(17), 7));
      when 87     => current_data_i <= control_mux.mux_ch(18) & std_logic_vector(to_unsigned(control_mux.bitcounts(18), 7));
      when 88     => current_data_i <= control_mux.mux_ch(19) & std_logic_vector(to_unsigned(control_mux.bitcounts(19), 7));
      when 89     => current_data_i <= control_mux.mux_ch(20) & std_logic_vector(to_unsigned(control_mux.bitcounts(20), 7));
      when 90     => current_data_i <= control_mux.mux_ch(21) & std_logic_vector(to_unsigned(control_mux.bitcounts(21), 7));
      when 91     => current_data_i <= control_mux.mux_ch(22) & std_logic_vector(to_unsigned(control_mux.bitcounts(22), 7));
      when 92     => current_data_i <= control_mux.mux_ch(23) & std_logic_vector(to_unsigned(control_mux.bitcounts(23), 7));
      when 93     => current_data_i <= control_mux.mux_ch(24) & std_logic_vector(to_unsigned(control_mux.bitcounts(24), 7));
      when 94     => current_data_i <= control_mux.mux_ch(25) & std_logic_vector(to_unsigned(control_mux.bitcounts(25), 7));
      when 95     => current_data_i <= control_mux.mux_ch(26) & std_logic_vector(to_unsigned(control_mux.bitcounts(26), 7));
      when 97     => current_data_i <= "00000" & control_digtrans.gdtc.ric & control_digtrans.gdtc.rdc & control_digtrans.gdtc.mss;
      when 98     => current_data_i <= control_digtrans.vce(7 downto 0);
      when 99     => current_data_i <= control_digtrans.vce(15 downto 8);
      when 100    => current_data_i <= control_digtrans.vctl(7 downto 0);
      when 101    => current_data_i <= control_digtrans.vctl(15 downto 8);
      when 102    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(2), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(1), 4));
      when 103    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(4), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(3), 4));
      when 104    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(6), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(5), 4));
      when 105    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(8), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(7), 4));
      when 106    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(10), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(9), 4));
      when 107    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(12), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(11), 4));
      when 108    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(14), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(13), 4));
      when 109    => current_data_i <= std_logic_vector(to_unsigned(control_digtrans.vctp(16), 4)) & std_logic_vector(to_unsigned(control_digtrans.vctp(15), 4));
      when 110    => current_data_i <= "0000000" & status_digtrans.gdts.nsto;
      when 111    => current_data_i <= status_digtrans.tnd(7 downto 0);
      when 112    => current_data_i <= status_digtrans.tnd(15 downto 8);
      when 113    => current_data_i <= std_logic_vector(to_unsigned(status_digtrans.fdct, 8));
      when 114    => current_data_i <= std_logic_vector(to_unsigned(status_digtrans.fict, 8));
      when 115    => current_data_i <= std_logic_vector(to_unsigned(status_digtrans.fdcr, 8));
      when 116    => current_data_i <= std_logic_vector(to_unsigned(status_digtrans.ficr, 8));
      when 117    => current_data_i <= cfg_local_transit_n_vec(7 downto 0);
      when 118    => current_data_i <= cfg_local_transit_n_vec(15 downto 8);
      when others => current_data_i <= x"00";
    end case;
 
  end if;
end process clocked;

  tx_value_debug        <= tx_value_debug_i;
  start_transfer_debug  <= start_transfer_debug_i;
   
  
END behave;

