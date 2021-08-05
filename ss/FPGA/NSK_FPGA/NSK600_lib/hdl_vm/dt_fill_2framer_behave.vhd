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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_fill_2framer_behave.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
--USE ieee.std_logic_arith.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_fill_2framer IS
   PORT( 
      clk                 : IN    std_logic;
      reset_n             : IN    std_logic;
      page_ready          : IN    std_logic_vector(1 downto 0);
      frame_start_enf     : IN    std_logic;
      data_dt_rx_in       : IN    std_logic;
      reset_ins_cnt       : IN    std_logic;
      tdm_page            : IN    std_logic;
      insert_AIS          : IN    std_logic;
      page_copied         : OUT   std_logic_vector(1 downto 0);
      data_dt_rx_out      : OUT   std_logic;
      ptr_dt_rx_ram1      : OUT   integer range 0 to 16383;       -- source data
      ptr_dt_rx_ram2      : OUT   integer range 0 to 16383;       -- destination data
      enp_dt_rx_ram2      : OUT   std_logic;
      frames_inserted_rx  : OUT   integer range 0 to 255
   );
END dt_fill_2framer;

--
ARCHITECTURE behave OF dt_fill_2framer IS

-----------------------------------------------------------
-- Last edited:
--   110309: dsg@zuehlke.com: File Generated
--   110630: no changes




  signal start_copy_i           : std_logic;
  signal current_page_copy_i    : std_logic;
  signal page_copied_i          : std_logic_vector(1 downto 0);
  signal insert_frame_i         : std_logic;
  signal bitcounter_i           : integer range 0 to 8191;
  signal data_source_i          : std_logic;                      -- '0': data from external RAM, '1': overwrite data with 'data_out_i'
  signal data_source_i1         : std_logic;
  signal data_out_i             : std_logic;
  signal data_out_i1            : std_logic;
  signal data_dt_rx_out_i       : std_logic;
  signal ptr_dt_rx_ram1_i       : integer range 0 to 16383;
  signal ptr_dt_rx_ram2_i       : integer range 0 to 16383;
  signal ptr_dt_rx_ram2_i1      : integer range 0 to 16383;       -- 1 clk  delayed ptr_dt_rx_ram2_i
  signal ptr_dt_rx_ram2_i2      : integer range 0 to 16383;       -- 2 clks delayed ptr_dt_rx_ram2_i
  signal enp_dt_rx_ram2_i       : std_logic;
  signal enp_dt_rx_ram2_i1      : std_logic;                      -- 1 clk  delayed enp_dt_rx_ram2_i
  signal enp_dt_rx_ram2_i2      : std_logic;                      -- 2 clks delayed enp_dt_rx_ram2_i
  signal frames_inserted_rx_i   : integer range 0 to 255;
  signal save_data_i            : std_logic;
  signal save_data_i1           : std_logic;                      -- 1 clk  delayed save_data_i
  signal save_data_ptr_i        : integer range 0 to 1071;
  signal save_data_ptr_i1       : integer range 0 to 1071;        -- 1 clk  delayed save_data_ptr_i
  signal old_subframes_i        : std_logic_vector(0 to 1071);    -- 16 channels with 67 bits each
BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                                           -- reset
    start_copy_i            <= '0';
    page_copied_i           <= "00";
    current_page_copy_i     <= '0';
    insert_frame_i          <= '0';
    bitcounter_i            <= 0;
    data_source_i           <= '0';
    data_source_i1          <= '0';
    data_out_i              <= '0';
    data_out_i1             <= '0';
    data_dt_rx_out_i        <= '0';
    ptr_dt_rx_ram1_i        <= 0;
    ptr_dt_rx_ram2_i        <= 0;
    ptr_dt_rx_ram2_i1       <= 0;
    ptr_dt_rx_ram2_i2       <= 0;
    enp_dt_rx_ram2_i        <= '0';
    enp_dt_rx_ram2_i1       <= '0';
    enp_dt_rx_ram2_i2       <= '0';
    frames_inserted_rx_i    <= 0;
    save_data_i             <= '0';
    save_data_i1            <= '0';
    save_data_ptr_i         <= 0;
    save_data_ptr_i1        <= 0;
    old_subframes_i         <= (others => '0'); 

  elsif clk'event and clk = '1' then                              -- running on rising edge    
 
    if (reset_ins_cnt = '1') then                                 -- reset frames inserted counter
      frames_inserted_rx_i <= 0;
    end if;

    data_source_i1        <= data_source_i;                       -- delay data read-in
    data_out_i1           <= data_out_i;                          -- delay internal generated data (so it behave like the synchronous RAM)
    if (data_source_i1 = '0') then
      data_dt_rx_out_i    <= data_dt_rx_in;                       -- data from external RAM
    else
      data_dt_rx_out_i    <= data_out_i1;                         -- internal (delayed) generated data (in case of subframe insertion)
    end if;    

    ptr_dt_rx_ram2_i1     <= ptr_dt_rx_ram2_i;                    -- delay RAM2-pointer and enable pulse for 2 clks
    ptr_dt_rx_ram2_i2     <= ptr_dt_rx_ram2_i1;
    enp_dt_rx_ram2_i1     <= enp_dt_rx_ram2_i;
    enp_dt_rx_ram2_i2     <= enp_dt_rx_ram2_i1;

    save_data_i1          <= save_data_i;                         -- data at data_dt_rx_in ready after 2 clk cycles after pointer ptr_dt_rx_ram1_i -> delay read-in
    save_data_ptr_i1      <= save_data_ptr_i;
    if (save_data_i1 = '1') then
      old_subframes_i(save_data_ptr_i1)  <= data_dt_rx_in;
    end if;
        
    if (frame_start_enf = '1') then                               -- triggering copy-process -> defining starting conditions
      start_copy_i  <= '1';
      bitcounter_i  <= 0;

      if (page_ready(0) = '0' and page_ready(1) = '0') then       -- both pages empty -> insert a frame with sending a modified header and 0's
        insert_frame_i    <= '1';
        if (frames_inserted_rx_i < 255) then
          frames_inserted_rx_i  <= frames_inserted_rx_i + 1;
        end if;
      elsif (page_ready(0) = '1' and page_ready(1) = '0') then    -- page 0 available
        insert_frame_i    <= '0';
        current_page_copy_i <= '0';
      elsif (page_ready(0) = '0' and page_ready(1) = '1') then    -- page 1 available
        insert_frame_i    <= '0';
        current_page_copy_i <= '1';
      else
        insert_frame_i    <= '0';
        current_page_copy_i <= NOT (current_page_copy_i);         -- both pages available -> take the older one
      end if;

    else
      if (start_copy_i = '1') then                                                        -- copy next bit of the whole page
        if (insert_frame_i = '1') then                                                      -- frame has to be inserted
          if (insert_AIS = '0') then                                                          -- digital transit connection is OK -> 'normal' header 
            if ((bitcounter_i) MOD 512) < 3 then                                              -- first 3 bits of each channel
              if (old_subframes_i(((bitcounter_i / 512)*67) to (((bitcounter_i / 512)*67 + 2))) = "010") or 
                 (old_subframes_i(((bitcounter_i / 512)*67) to (((bitcounter_i / 512)*67 + 2))) = "001") or
                 (old_subframes_i(((bitcounter_i / 512)*67) to (((bitcounter_i / 512)*67 + 2))) = "111") then  -- after 1st subframe
                data_source_i     <= '1';
                data_out_i        <= old_subframes_i((bitcounter_i/512)*67 + (bitcounter_i MOD 512));          -- passthrough (don't modify header)
              else                                                                                             -- after 2nd or 3rd subframe
                data_source_i     <= '1';
                data_out_i        <= NOT (old_subframes_i((bitcounter_i/512)*67 + (bitcounter_i MOD 512)));    -- invert header bits
              end if; 
            elsif ((bitcounter_i) MOD 512) < 67 then                                          -- bits 4 - 67 of each channel
              data_source_i       <= '1';
              data_out_i          <= old_subframes_i((bitcounter_i/512)*67 + (bitcounter_i MOD 512));
            else
              data_source_i       <= '1';
              data_out_i          <= '0';                                                     -- send 0's for the rest of the data (bits 68+)
            end if;
          else
            data_source_i       <= '1';
            data_out_i          <= '1';                                                     -- send 1's for all of the data (complete page)
          end if;
        else                                                                                -- no frame has to be inserted because new page available
          if (current_page_copy_i = '0') then
            ptr_dt_rx_ram1_i    <= (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;      
          else
            ptr_dt_rx_ram1_i    <= 512 + (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;
          end if;
          data_source_i     <= '0';

          if ((bitcounter_i) MOD 512) < 67 then                                              -- save first 67 bits of each channel (for possible header modification during next frame)
            save_data_i         <= '1';
            save_data_ptr_i     <= (bitcounter_i / 512)*67 + (bitcounter_i MOD 512);
          else
            save_data_i         <= '0';
          end if;
        end if;
        
        if (tdm_page = '0') then                                                            -- destination data (alternating page)
          ptr_dt_rx_ram2_i      <= 512 + (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;             
        else
          ptr_dt_rx_ram2_i      <= (bitcounter_i / 512) * 1024 + (bitcounter_i) MOD 512;
        end if;        
        enp_dt_rx_ram2_i      <= '1';
        bitcounter_i          <= (bitcounter_i + 1) MOD 8192;
        
        if (bitcounter_i = 8191) then                                                       -- page successfully copied -> signalize to block 'dt_fill_ram_rx'
          start_copy_i        <= '0';
          enp_dt_rx_ram2_i    <= '0';
          if (insert_frame_i = '0') then
            if (current_page_copy_i = '0') then
              page_copied_i   <= "01";
            else
              page_copied_i   <= "10";
            end if;
          end if;
        end if;
      else
        page_copied_i         <= "00";
      end if;
    end if;      

  end if;
end process clocked;

    page_copied         <= page_copied_i;
    data_dt_rx_out      <= data_dt_rx_out_i;
    ptr_dt_rx_ram1      <= ptr_dt_rx_ram1_i;
    ptr_dt_rx_ram2      <= ptr_dt_rx_ram2_i2;
    enp_dt_rx_ram2      <= enp_dt_rx_ram2_i2;
    frames_inserted_rx  <= frames_inserted_rx_i; 
      
END behave;