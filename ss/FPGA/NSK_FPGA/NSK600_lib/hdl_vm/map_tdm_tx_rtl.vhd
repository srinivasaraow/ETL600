--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.map_tdm_tx.rtl
-- -----------------------------------------------------------------------------
-- Copyright (c) ABB Switzerland Ltd 2007
-- -----------------------------------------------------------------------------
-- Project      : O4CV
-- Library      : NSK600_lib
-- Unit         : map_tdm_tx
-- Language     : VHDL
--------------------------------------------------------------------------------
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--
--------------------------------------------------------------------------------
-- $Workfile: map_tdm_tx_rtl.vhd $
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/map_tdm_tx_rtl.vhd $ 
-- 
-- 10    11-07-06 9:41 Chstrue
-- R4 implementation
--
-- 7     11-03-08 14:00 Chstrue
-- top level with pin multiplexing
-- 
-- 6     08-03-14 16:13 Chmaglo1
-- Re-checkin because of interface differences. 
-- No functional change.
-- 
-- 5     07-10-11 15:23 Chmaglo1
-- -- no description --
-- 0     2007-06-06 15:22:18 Mathias Gloor (CH-L-0012267)
-- File Generated 
--

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY map_tdm_tx IS
   PORT( 
      clk          : IN     std_logic;
      rd_ptr       : IN     integer RANGE 0 TO 16383;
      reset_n      : IN     std_logic;
      tx_tdm       : IN     std_logic_vector (2 DOWNTO 0);
      tdm_page_enf : IN     std_logic;
      enf_rd_en    : IN     std_logic;
      enp_txd      : OUT    std_logic;
      txd          : OUT    std_logic;
      wr_ptr_tx    : OUT    integer RANGE 0 TO 16383
   );

-- Declarations

END map_tdm_tx ;

--
--------------------------------------------------------------------------------
architecture rtl of map_tdm_tx is

--------------------------------------------------------------------------------
-- last edited: 
--   2011-02-21: dsg@zuehlke.com: initialization fixed
--   2011-06-27: dsg@zuehlke.com: changed 'rd_ptr_tx' to 'rd_ptr'
--   2011-06-29: dsg@zuehlke.com: changed extra page switch mechanism
--   2011-06-30: R4 implementation

--------------------------------------------------------------------------------



--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal cnt_tdmframe : t_cnt_tdmframe;
  type t_ptr_int is array (0 to 15) of integer range 0 to 1023;
  signal ptr_int        : t_ptr_int; -- := (others=>(others=>0));
  signal cnt_tdmpos     : integer range 0 to 127;
  signal cnt_info       : integer range 0 to 15;
  signal wr_page        : std_logic_vector(15 downto 0);
  signal shift_info     : std_logic_vector(7 downto 0);
  signal sfr_info       : std_logic_vector (3 downto 0);
  signal valid_tdm_data : std_logic;
  signal ch1_reading    : std_logic;
  signal ch2_reading    : std_logic;
  signal ch3_reading    : std_logic;
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  process (clk, reset_n)
  begin
    sfr_info <= shift_info(2 downto 0) & tx_tdm(2);
    
    if reset_n = '0' then
      ptr_int     <= (others => 0);
      cnt_tdmpos  <= 0;
      cnt_info    <= 0;
      wr_page     <= (others => '0');
      shift_info  <= (others => '0');
      sfr_info    <= (others => '0');
    elsif clk'event and clk = '1' then
      shift_info <= shift_info(6 downto 0) & tx_tdm(2); --MSB first!!!
      
      --------------------------------------------------------------------------
      -- subframe start flag support logic
      if cnt_info>0 then
        cnt_info<= cnt_info-1;
      --elsif tx_tdm(2)='0' and shift_info(0)='1' then 
      elsif shift_info(1)='0' and shift_info(0)='1' then 
        cnt_info<=8;
      end if;
      
      if cnt_info=5 then
        for board in 0 to 3 loop
          if sfr_info(board)='1' then
            cnt_tdmframe(board)<=0;
            wr_page(4*board+3 downto 4*board) <= not wr_page(4*board+3 downto 4*board);
            ----aggregate stream slower than TDM-stream (10.667kHz*8bit per 10ms-frame) -> page change at tdm channel window start
            --if bitcounts(0)<=853 then
            --  wr_page(4*board+3 downto 4*board) <= not wr_page(4*board+3 downto 4*board);
            --end if;
          
          elsif cnt_tdmframe(board)<62 then -- 63 (resp. it's equivalent addr.) interferes with the default value for rd_ptr -> 62
            cnt_tdmframe(board)<=cnt_tdmframe(board)+1;
          end if;
        end loop;
      end if;
    
      --------------------------------------------------------------------------
      -- ram write pointer generation; basing on tdm_clk = fpga_clk
      if tx_tdm(0)='0' or cnt_tdmpos=127 then
        cnt_tdmpos<=0;
      else
        cnt_tdmpos<=cnt_tdmpos+1;
      end if;


--- debug help signal for simulation:
      valid_tdm_data  <= '0';
      ch1_reading     <= '0';
      ch2_reading     <= '0';
      ch3_reading     <= '0';

      for ch in 0 to 3 loop
        if (cnt_tdmframe(ch/4) < 9) then    -- first 9 TDM frames
          valid_tdm_data <= '1';
        end if;
      end loop;

      if (rd_ptr / 1024) = 1 then           -- channel 1 is reading
        ch1_reading <= '1';
      end if;
      if (rd_ptr / 1024) = 2 then           -- channel 2 is reading
        ch2_reading <= '1';
      end if;
      if (rd_ptr / 1024) = 3 then           -- channel 3 is reading
        ch3_reading <= '1';
      end if;
            
      --------------------------------------------------------------------------
      -- memory page selection for write and read access
      if tx_tdm(0)='1' then
        for ch in 0 to 15 loop
          
          if cnt_tdmpos/32=ch/4 --board
             and  (cnt_tdmpos/8) MOD 4 = 3-(ch MOD 4) then
            ptr_int(ch)<=conv_integer(wr_page(4*(cnt_tdmpos/32) + 3 -((cnt_tdmpos/8) MOD 4)))*64*8 + 
                              cnt_tdmframe(cnt_tdmpos/32)*8+ 
                              (cnt_tdmpos MOD 8); 

            ----aggregate stream faster than TDM-stream (10.667kHz*8bit per 10ms-frame) -> page change at tdm channel window end
            --if bitcounts(0)>853 then
            --  if cnt_tdmframe(cnt_tdmpos/32)*8 + (cnt_tdmpos MOD 8) --bit pointer per ram page
            --    = (bitcounts(ch+CH_MAX-CH_SYNC+1) -1) then
            --    wr_page(ch) <= not wr_page(ch);
            --  end if;
            --end if;
            
            
            --old variant, befor R4:
            -- extra page switch in case of read/write collision per channel    (old variant)
            -- if rd_ptr = ch*1024 + ptr_int(ch) then
            --   wr_page(ch) <= not wr_page(ch);
            -- end if;
            
            --new R4 implementation:
            if (cnt_tdmframe(ch/4) < 9) and (enf_rd_en = '1') and (ch = (rd_ptr/1024)) and (tdm_page_enf = wr_page(ch)) then
                -- TDM is writing valid voice data in RAM (first 9 TDM frames a 8 bit)
                                              -- aggregate enframer is reading
                                                                          -- enframer is reading from this voice channel
                                                                                                    -- channel write and read on the same page
              wr_page(ch) <= not wr_page(ch);                                                       -- change to opposite page for affected channel
            end if;
          end if;
        end loop;
      end if;  
                  
      --------------------------------------------------------------------------
    end if;
  end process;

--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
wr_ptr_tx <= conv_integer(
             std_logic_vector(to_unsigned((cnt_tdmpos/32) MOD 4, 2)) &    --O4CV-board (0..3)
             std_logic_vector(to_unsigned(3-(cnt_tdmpos/8) MOD 4, 2)) &   --O4CV-channel (0..3->3..0)
             wr_page((cnt_tdmpos/32)*4+(3-(cnt_tdmpos/8) MOD 4)) &        --page (0,1) (ch0..3->ch3..0)
             std_logic_vector(to_unsigned(cnt_tdmframe(cnt_tdmpos/32) MOD 64, 6)) & --tdmframe per muxframe (0..63)
             std_logic_vector(to_unsigned(cnt_tdmpos MOD 8, 3))           --bit per byte (0..7)
               );
--rd_page <= not wr_page;               
txd <= tx_tdm(1);
enp_txd <= tx_tdm(0);
-- 
--------------------------------------------------------------------------------
end architecture rtl;
