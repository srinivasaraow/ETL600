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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_fill_2dt_behave.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_fill_2dt IS
   PORT( 
      clk                 : IN    std_logic;
      reset_n             : IN    std_logic;
      get_tx_data         : IN    std_logic;
      current_o4cv_ch     : IN    std_logic_vector(3 downto 0);
      current_o4cv_pos    : IN    std_logic_vector(6 downto 0);
      reset_ins_cnt       : IN    std_logic;
      tx_data_in          : IN    std_logic;
      page_ready          : IN    std_logic_vector(1 downto 0);
      frames_inserted_tx  : OUT   integer range 0 to 255;
      tx_data_out         : OUT   std_logic;
      page_copied         : OUT   std_logic_vector(1 downto 0);
      rd_ptr_ram          : OUT   integer range 0 to 16383
   );
END dt_fill_2dt;

--
ARCHITECTURE behave OF dt_fill_2dt IS

-----------------------------------------------------------
-- Last edited:
--   110317: (dsg@zuehlke.com): File Generated
--   110630: R4 implementation



  signal current_page_rd_i      : std_logic;
  signal page_copied_i          : std_logic_vector(1 downto 0);
  signal frames_inserted_tx_i   : integer range 0 to 255;
  signal tx_data_out_i          : std_logic;
  signal data_source_i          : std_logic;                      -- '0': data from external RAM, '1': overwrite data with 'data_out_i'
  signal data_source_i1         : std_logic;                      -- 1 clk delayed data_source_i
  signal data_out_i             : std_logic;
  signal data_out_i1            : std_logic;                      -- 1 clk delayed data_out_i
  signal rd_ptr_ram_i           : integer range 0 to 16383;
  signal insert_frame_i         : std_logic;
  signal save_data_i            : std_logic;
  signal save_data_i1           : std_logic;                      -- 1 clk delayed save_data_i
  signal save_data_ptr_i        : integer range 0 to 1071;
  signal save_data_ptr_i1       : integer range 0 to 1071;        -- 1 clk delayed save_data_ptr_i
  signal old_subframes_i        : std_logic_vector(0 to 1071);    -- 16 channels with 67 bits each
BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                                           -- reset
    current_page_rd_i     <= '0';
    page_copied_i         <= "00";
    tx_data_out_i         <= '0';
    data_source_i         <= '0';
    data_source_i1        <= '0';
    data_out_i            <= '0';
    data_out_i1           <= '0';
    frames_inserted_tx_i  <= 0;
    rd_ptr_ram_i          <= 0;
    insert_frame_i        <= '0';
    save_data_i           <= '0';
    save_data_i1          <= '0';
    save_data_ptr_i       <= 0;
    save_data_ptr_i1      <= 0;
    old_subframes_i       <= (others => '0');
    
  elsif clk'event and clk = '1' then                              -- running on rising edge

    if (reset_ins_cnt = '1') then                                 -- reset frames inserted counter
      frames_inserted_tx_i <= 0;
    end if;
    
    data_source_i1        <= data_source_i;                       -- delay data read-in
    data_out_i1           <= data_out_i;                          -- delay internal generated data (so it behave like the synchronous RAM)
    if (data_source_i1 = '0') then
      tx_data_out_i       <= tx_data_in;                          -- data from external RAM
    else
      tx_data_out_i       <= data_out_i1;                         -- internal (delayed) generated data (in case of subframe insertion)
    end if;    

    save_data_i1          <= save_data_i;                         -- data at tx_data_in ready after 2 clk cycles after pointer rd_ptr_ram_i -> delay read-in
    save_data_ptr_i1      <= save_data_ptr_i;
    if (save_data_i1 = '1') then
      old_subframes_i(save_data_ptr_i1)  <= tx_data_in;
    end if;
    
    if (get_tx_data = '1') and (current_o4cv_ch = "0000") and (current_o4cv_pos = "0000000") then      -- check for new frame start

      if (page_ready(0) = '0' and page_ready(1) = '0') then       -- both pages empty -> insert a frame with sending a modified header and 0's
        insert_frame_i    <= '1';
        if (frames_inserted_tx_i < 255) then
          frames_inserted_tx_i  <= frames_inserted_tx_i + 1;
        end if;
        if (old_subframes_i(0 to 2) = "010") or 
           (old_subframes_i(0 to 2) = "001") or
           (old_subframes_i(0 to 2) = "111") then                 -- after 1st subframe
          data_source_i   <= '1';
          data_out_i      <= old_subframes_i(0);                  -- passthrough (don't modify header)
        else                                                      -- after 2nd or 3rd subframe
          data_source_i   <= '1';
          data_out_i      <= NOT (old_subframes_i(0));            -- invert header bits
        end if;
      elsif (page_ready(0) = '1' and page_ready(1) = '0') then    -- page 0 available
        insert_frame_i    <= '0';
        current_page_rd_i <= '0';
        data_source_i     <= '0';
        rd_ptr_ram_i      <= 0;
        save_data_i       <= '1';
        save_data_ptr_i   <= 0;
      elsif (page_ready(0) = '0' and page_ready(1) = '1') then    -- page 1 available
        insert_frame_i    <= '0';
        current_page_rd_i <= '1';
        data_source_i     <= '0';
        rd_ptr_ram_i      <= 512;        
        save_data_i       <= '1';
        save_data_ptr_i   <= 0;
      else
        insert_frame_i    <= '0';
        current_page_rd_i <= NOT (current_page_rd_i);             -- both pages available -> take the older one
        data_source_i     <= '0';
        if (current_page_rd_i = '0') then
          rd_ptr_ram_i    <= 512; 
        else
          rd_ptr_ram_i    <= 0;
        end if;
        save_data_i       <= '1';
        save_data_ptr_i   <= 0;
      end if;

    elsif (get_tx_data = '1') then
      
      if (insert_frame_i = '1') then
        if ((to_integer(unsigned(current_o4cv_pos))) < 3) then
          if (old_subframes_i(((to_integer(unsigned(current_o4cv_ch)))*67) to (((to_integer(unsigned(current_o4cv_ch)))*67) + 2)) = "010") or 
             (old_subframes_i(((to_integer(unsigned(current_o4cv_ch)))*67) to (((to_integer(unsigned(current_o4cv_ch)))*67) + 2)) = "001") or
             (old_subframes_i(((to_integer(unsigned(current_o4cv_ch)))*67) to (((to_integer(unsigned(current_o4cv_ch)))*67) + 2)) = "111") then    -- after 1st subframe
            data_source_i <= '1';
            data_out_i    <= old_subframes_i((to_integer(unsigned(current_o4cv_ch))) * 67 + to_integer(unsigned(current_o4cv_pos)));               -- passthrough (don't modify header)
          else                                                    -- after 2nd or 3rd subframe
            data_source_i <= '1';                                 -- internal data generation: invert header bits
            data_out_i    <= NOT (old_subframes_i((to_integer(unsigned(current_o4cv_ch))) * 67 + to_integer(unsigned(current_o4cv_pos))));         -- invert header bits
          end if; 
        elsif ((to_integer(unsigned(current_o4cv_pos))) < 67) then
          data_source_i <= '1';
          data_out_i    <= old_subframes_i((to_integer(unsigned(current_o4cv_ch))) * 67 + to_integer(unsigned(current_o4cv_pos)));                 -- send payload from last frame again
        else
          data_source_i   <= '1';                                 -- internal data generation: send 0's after first 67 bits
          data_out_i      <= '0';
        end if;
      else
        data_source_i     <= '0';                                 -- forward data from RAM (no frame insertion)
        if ((to_integer(unsigned(current_o4cv_pos))) < 67) then   -- save first 67 bits of each channel (for frame insertion)
          save_data_i       <= '1';
          save_data_ptr_i   <= (to_integer(unsigned(current_o4cv_ch))) * 67 + to_integer(unsigned(current_o4cv_pos));
        else
          save_data_i       <= '0';
        end if;
      end if;
      
      if (current_page_rd_i = '0') then                           -- choose page 0 or 1 for reading
        rd_ptr_ram_i      <= (to_integer(unsigned(current_o4cv_ch)) * 1024) + to_integer(unsigned(current_o4cv_pos));
      else
        rd_ptr_ram_i      <= (to_integer(unsigned(current_o4cv_ch)) * 1024) + 512 + to_integer(unsigned(current_o4cv_pos));
      end if;

      if (current_o4cv_ch = "1111") and (current_o4cv_pos = "1000111") and (insert_frame_i = '0') then
        if (current_page_rd_i = '0') then
          page_copied_i <= "01";
        else
          page_copied_i <= "10";
        end if;        
      end if;    
 
    else
      page_copied_i <= "00";
    end if;
 
  end if;
end process clocked;

  page_copied         <= page_copied_i;
  frames_inserted_tx  <= frames_inserted_tx_i;
  tx_data_out         <= tx_data_out_i;
  rd_ptr_ram          <= rd_ptr_ram_i;
      
END behave;