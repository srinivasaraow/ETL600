--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.map_tdm_rx.rtl
-- Project  : O4CV
-- Language : VHDL
-- Remarks  : 
-- Copyright: ABB Switzerland Ltd. 2007
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--------------------------------------------------------------------------------
-- $Workfile: map_tdm_rx_rtl.vhd $
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/map_tdm_rx_rtl.vhd $ 
-- 
-- 7     08-03-14 16:14 Chmaglo1
-- Re-checkin because of interface differences. 
-- No functional change.
-- 
-- 6     07-10-11 15:23 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-25 16:26 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-10 19:44 Chmaglo1
-- Variante: TDM Short Loop
-- 0     2007-02-16 21:53:00 Mathias Gloor (CH-L-0012267)
-- File Generated 
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;
USE ieee.std_logic_unsigned.all;

ENTITY map_tdm_rx IS
   PORT( 
      clk       : IN     std_logic;
      reset_n   : IN     std_logic;
      rx_tdm    : IN     std_logic_vector (2 DOWNTO 0);
      enp_rxd   : OUT    std_logic;
      rxd_in    : OUT    std_logic;
      rd_page   : OUT    std_logic_vector (3 DOWNTO 0);
      wr_ptr_rx : OUT    integer RANGE 0 TO 16383
   );

-- Declarations

END map_tdm_rx ;

--
--------------------------------------------------------------------------------
architecture rtl of map_tdm_rx is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
  signal cnt_info   : integer range 0 to 15 := 0;
  signal cnt_frameend: integer range 0 to 31 := 0;
  signal shift_info : std_logic_vector (7 downto 0) := (others=>'0');
  signal vec_info   : std_logic_vector (7 downto 0) := (others=>'0');
  signal wr_ptr_rx_i: integer range 0 to 16383 := 0;
  signal wr_page_i  : std_logic_vector(3 downto 0) := (others=>'0');
-- 

begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  process (clk, reset_n)
  begin
    if clk'event and clk = '1' then
      
      --------------------------------------------------------------------------
      -- collect port specific signalling information at each mux channel change
      shift_info <= rx_tdm(2) & shift_info(7 downto 1); --LSB first
      if cnt_info>0 then
        cnt_info<= cnt_info-1;
      elsif (shift_info(6)='0' and shift_info(7)='1' and rx_tdm(2)='0') then
        cnt_info<=8;
      end if;
      
      --------------------------------------------------------------------------
      -- Mux frame-end detection logic
      if rx_tdm(2)='1' then
        cnt_frameend <= 31;
      elsif cnt_frameend>0 then
        cnt_frameend <= cnt_frameend-1;
      end if;
      
      --------------------------------------------------------------------------
      -- buffer write access logic
      if cnt_info=2 or cnt_frameend=1 then
        vec_info<=shift_info;

        --initializing buffer write pointer at mux channel window start
        wr_ptr_rx_i<=conv_integer(shift_info(5 downto 2) & not wr_page_i(conv_integer(shift_info(5 downto 4))) &  "000000" & "000");

        --toggling the wr_page bit of the "before" transmitted channel the according mux window
        if shift_info(5 downto 4)/=vec_info(5 downto 4) or cnt_frameend=1 then  --if board window border or frameend then toggle ...
          wr_page_i(conv_integer(vec_info(5 downto 4))) <= not wr_page_i(conv_integer(vec_info(5 downto 4)));
        end if;

      elsif rx_tdm(0)='1' then
        wr_ptr_rx_i <= wr_ptr_rx_i + 1; -- increment buffer write pointer for each data bit
      end if;

    end if;
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
  wr_ptr_rx <= wr_ptr_rx_i;
  enp_rxd <= rx_tdm(0); -- when vec_info(7)='1' else '0';        
  rxd_in <= rx_tdm(1);  -- when vec_info(7)='1' else '0'; 
  rd_page <= wr_page_i;
-- 
--------------------------------------------------------------------------------
end architecture rtl;
