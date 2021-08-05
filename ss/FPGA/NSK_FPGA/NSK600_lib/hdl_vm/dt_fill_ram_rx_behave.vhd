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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_fill_ram_rx_behave.vhd $
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_fill_ram_rx IS
   PORT( 
      clk                   : IN    std_logic;
      reset_n               : IN    std_logic;
      put_rx_data           : IN    std_logic;
      current_o4cv_ch       : IN    std_logic_vector(3 downto 0);
      current_o4cv_pos      : IN    std_logic_vector(6 downto 0);
      rx_data_in            : IN    std_logic;
      page_copied           : IN    std_logic_vector(1 downto 0);
      reset_dis_cnt         : IN    std_logic;
      burst_started         : IN    std_logic;
      burst_finished        : IN    std_logic;
      disturb               : IN    std_logic;
      page_ready            : OUT   std_logic_vector(1 downto 0);
      frames_removed_rx     : OUT   integer range 0 to 255;
      rx_data_out           : OUT   std_logic;
      enp_data_out          : OUT   std_logic;
      wr_ptr_ram            : OUT   integer range 0 to 16383
   );
END dt_fill_ram_rx;

--
ARCHITECTURE behave OF dt_fill_ram_rx IS
  
-----------------------------------------------------------
-- Last edited:
--   110309: dsg@zuehlke.com: File Generated
--   110630: R4 implementation

  
  
  
  signal current_page_wr_i    : std_logic;
  signal page_ready_i         : std_logic_vector(1 downto 0);
  signal frames_removed_rx_i  : integer range 0 to 255;
  signal rx_data_out_i        : std_logic;
  signal enp_data_out_i       : std_logic;
  signal wr_ptr_ram_i         : integer range 0 to 16383;
  signal burst_finished_old   : std_logic;
BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                             -- reset
    current_page_wr_i   <= '0';
    page_ready_i        <= "00";
    rx_data_out_i       <= '0';
    enp_data_out_i      <= '0';
    frames_removed_rx_i <= 0;
    wr_ptr_ram_i        <= 0;
    burst_finished_old  <= '0';
    
  elsif clk'event and clk = '1' then                -- running on rising edge

    burst_finished_old  <= burst_finished;
    
    if (reset_dis_cnt = '1') then                                                           -- reset frames removed counter
      frames_removed_rx_i <= 0;
    end if;
    
    if (page_copied(0) = '1') then                                                          -- clear flag of available page if page was copied by 'dt_fill_2framer'
      page_ready_i(0)   <= '0';
    elsif (burst_finished = '1') and (burst_finished_old = '0') and (current_page_wr_i = '0') then
      page_ready_i(0)   <= '1';                                                             -- set flag for an available page if last bit was written (rising edge of burst_finished)
    end if;

    if (page_copied(1) = '1') then                                                          -- clear flag of available page if page was copied by 'dt_fill_2framer'
      page_ready_i(1)   <= '0';
    elsif (burst_finished = '1') and (burst_finished_old = '0') and (current_page_wr_i = '1') then
      page_ready_i(1)   <= '1';                                                             -- set flag for an available page if last bit was written (rising edge of burst_finished)
    end if;
    
    if (burst_started = '1') then                                                           -- check for new frame start
      if (page_ready_i(0) = '0') then                                                       -- page 0 empty or both pages empty -> write in page 0
        wr_ptr_ram_i        <= 0;
        current_page_wr_i   <= '0';
      elsif (page_ready_i(1) = '0' ) then                                                   -- page 0 full but page 1 free
        wr_ptr_ram_i        <= 512;
        current_page_wr_i   <= '1';
      else                                                                                  -- both pages full -> discard frame while writing again in last page
        if (current_page_wr_i = '0') then
          wr_ptr_ram_i      <= 0;
        else
          wr_ptr_ram_i      <= 512;
        end if;
        if (frames_removed_rx_i < 255) then
          frames_removed_rx_i <= frames_removed_rx_i + 1;
        end if;
      end if;

    else                                                                                    -- waiting or writing other bits than ch0/bit0 (no header manipulation necessary)
      if (current_page_wr_i = '0') then                                                       -- choose page 0 or 1 for writing
        wr_ptr_ram_i        <= (to_integer(unsigned(current_o4cv_ch)) * 1024) + to_integer(unsigned(current_o4cv_pos));
      else
        wr_ptr_ram_i        <= (to_integer(unsigned(current_o4cv_ch)) * 1024) + 512 + to_integer(unsigned(current_o4cv_pos));
      end if;
    end if;
   enp_data_out_i      <= put_rx_data;
    
    if (disturb = '1') then
      rx_data_out_i   <= NOT (rx_data_in);
    else
      rx_data_out_i   <= rx_data_in;
    end if;
  end if;
end process clocked;

  page_ready        <= page_ready_i;
  frames_removed_rx <= frames_removed_rx_i;
  rx_data_out       <= rx_data_out_i;
  enp_data_out      <= enp_data_out_i;
  wr_ptr_ram        <= wr_ptr_ram_i;
      
END behave;