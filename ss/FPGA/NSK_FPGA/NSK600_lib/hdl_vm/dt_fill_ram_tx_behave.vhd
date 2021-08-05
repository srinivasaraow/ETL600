--------------------------------------------------------------------------------
-- $Workfile   : $
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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_fill_ram_tx_behave.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_fill_ram_tx IS
   PORT( 
      clk                 : IN    std_logic;
      reset_n             : IN    std_logic;
      page_copied         : IN    std_logic_vector(1 downto 0);
      frame_start_def     : IN    std_logic;
      data_dt_tx_in       : IN    std_logic;
      reset_dis_cnt       : IN    std_logic;
      tdm_page            : IN    std_logic;
      page_ready          : OUT   std_logic_vector(1 downto 0);
      data_dt_tx_out      : OUT   std_logic;
      ptr_dt_tx_ram1      : OUT   integer range 0 to 16383;   -- source data
      ptr_dt_tx_ram2      : OUT   integer range 0 to 16383;   -- destination data
      enp_dt_tx_ram2      : OUT   std_logic;
      frames_removed_tx   : OUT   integer range 0 to 255
   );
END dt_fill_ram_tx;

--
ARCHITECTURE behave OF dt_fill_ram_tx IS
  
-----------------------------------------------------------
-- Last edited:
--   110309: dsg@zuehlke.com: File Generated
--   110630: R4 implementation
  

  
  signal start_copy_i         : std_logic;
  signal page_ready_i         : std_logic_vector(1 downto 0);
  signal current_page_wr_i    : std_logic;
  signal bitcounter_i         : integer range 0 to 8191;
  signal data_dt_tx_out_i     : std_logic;
  signal ptr_dt_tx_ram1_i     : integer range 0 to 16383;
  signal ptr_dt_tx_ram2_i     : integer range 0 to 16383;
  signal ptr_dt_tx_ram2_i1    : integer range 0 to 16383;     -- 1 clk  delayed ptr_dt_tx_ram2_i
  signal ptr_dt_tx_ram2_i2    : integer range 0 to 16383;     -- 2 clks delayed ptr_dt_tx_ram2_i
  signal enp_dt_tx_ram2_i     : std_logic;
  signal enp_dt_tx_ram2_i1    : std_logic;                    -- 1 clk  delayed enp_dt_tx_ram2_i
  signal enp_dt_tx_ram2_i2    : std_logic;                    -- 2 clks delayed enp_dt_tx_ram2_i
  signal remove_frame_i       : std_logic;
  signal frames_removed_tx_i  : integer range 0 to 255;  
BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                                           -- reset
    start_copy_i          <= '0';
    page_ready_i          <= "00";
    current_page_wr_i     <= '0';
    bitcounter_i          <= 0;
    data_dt_tx_out_i      <= '0';
    ptr_dt_tx_ram1_i      <= 0;
    ptr_dt_tx_ram2_i      <= 0;
    ptr_dt_tx_ram2_i1     <= 0;
    ptr_dt_tx_ram2_i2     <= 0;
    enp_dt_tx_ram2_i      <= '0';
    enp_dt_tx_ram2_i1     <= '0';
    enp_dt_tx_ram2_i2     <= '0';
    remove_frame_i        <= '0';
    frames_removed_tx_i   <= 0;  

  elsif clk'event and clk = '1' then                              -- running on rising edge
    
    if (reset_dis_cnt = '1') then                                 -- reset frames deleted counter
      frames_removed_tx_i <= 0;
    end if;

    if (bitcounter_i = 8191) and (current_page_wr_i = '0') then   -- set flag for an available page if last bit was written
      page_ready_i      <= page_ready_i(1) & '1';                                           
    elsif (page_copied(0) = '1') then                             -- clear flag of available page if page was copied by 'dt_fill_2dt'
      page_ready_i      <= page_ready_i(1) & '0';
    end if;

    if (bitcounter_i = 8191) and (current_page_wr_i = '1') then   -- set flag for an available page if last bit was written
      page_ready_i      <= '1' & page_ready_i(0);                                           
    elsif (page_copied(1) = '1') then                             -- clear flag of available page if page was copied by 'dt_fill_2dt'
      page_ready_i      <= '0' & page_ready_i(0);
    end if;
    
    data_dt_tx_out_i    <= data_dt_tx_in;                         -- route input data to output (data in: 2clks after read_pointer valid)
    ptr_dt_tx_ram2_i1   <= ptr_dt_tx_ram2_i;                      -- -> delay write-pointer and enable pulse for 2 clks
    ptr_dt_tx_ram2_i2   <= ptr_dt_tx_ram2_i1;
    enp_dt_tx_ram2_i1   <= enp_dt_tx_ram2_i;
    enp_dt_tx_ram2_i2   <= enp_dt_tx_ram2_i1;
    
    if (frame_start_def = '1') then                               -- triggering copy-process -> defining starting conditions
      start_copy_i  <= '1';
      bitcounter_i  <= 0;

      if (page_ready_i(0) = '1' and page_ready_i(1) = '1') then   -- both pages already full -> remove a frame (no header modification)
        remove_frame_i    <= '1';
        if (frames_removed_tx_i < 255) then
          frames_removed_tx_i  <= frames_removed_tx_i + 1;
        end if;
      elsif (page_ready_i(0) = '1' and page_ready_i(1) = '0') then -- page 0 is full -> write to page 1
        remove_frame_i    <= '0';
        current_page_wr_i <= '1';
      else                                                        -- page 0 and maybe page 1 are available -> write in page 0
        remove_frame_i    <= '0';
        current_page_wr_i <= '0';
      end if;

    else
      if (start_copy_i = '1') then                                -- copy next bit of the whole page
        if (remove_frame_i = '1') then                            -- frame has to be removed -> do not copy anything
          start_copy_i        <= '0';
          remove_frame_i      <= '0';
          enp_dt_tx_ram2_i    <= '0';
        else                                                      -- frame has to be copied
          if (current_page_wr_i = '0') then                       -- destination pointer
            ptr_dt_tx_ram2_i  <= (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;      
          else
            ptr_dt_tx_ram2_i  <= 512 + (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;
          end if;

          if (tdm_page = '0') then                                -- source pointer (alternating page)
            ptr_dt_tx_ram1_i  <= (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;             
          else
            ptr_dt_tx_ram1_i  <= 512 + (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;
          end if;        
          
          enp_dt_tx_ram2_i    <= '1';
          bitcounter_i        <= (bitcounter_i + 1) MOD 8192;

          if (bitcounter_i = 8191) then                           -- page successfully copied
            start_copy_i        <= '0';
            enp_dt_tx_ram2_i    <= '0';
          end if;
        end if;
      end if;
    end if;      
  end if;
end process clocked;

    page_ready         <= page_ready_i;
    ptr_dt_tx_ram1     <= ptr_dt_tx_ram1_i;
    data_dt_tx_out     <= data_dt_tx_out_i;
    ptr_dt_tx_ram2     <= ptr_dt_tx_ram2_i2;
    enp_dt_tx_ram2     <= enp_dt_tx_ram2_i2;
    frames_removed_tx  <= frames_removed_tx_i; 

END behave;