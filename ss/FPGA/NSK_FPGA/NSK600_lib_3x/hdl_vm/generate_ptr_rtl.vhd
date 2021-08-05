--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.generate_ptr.rtl
-- $Workfile: generate_ptr_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/generate_ptr_rtl.vhd $
-- 
-- 6     9.10.06 11:53 Chmaglo1
-- -- no description --
-- 
-- 5     6.10.06 16:56 Chmaglo1
-- -- no description --
-- 
-- 4     3.10.06 13:23 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 13:58 Chmaglo1
-- -- no description --
-- 
-- 47    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 20    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 8     29.11.04 11:03 Unp00631
-- -- no description --
-- 
-- 1     15.10.04 17:34 Unp00631
-- before military service
-- 0     30.09.2004 12:49:53 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY generate_ptr IS
   PORT( 
      clk      : IN     std_logic;
      enp_dout : IN     std_logic;
      in_bus   : IN     std_logic_vector (2 DOWNTO 0);
      page     : IN     std_logic_vector (1 DOWNTO 0);
      level    : OUT    integer range 0 TO 4095;
      rd_ptr   : OUT    integer range 0 TO 4095;
      wr_ptr   : OUT    integer range 0 TO 4095;
      cnt_pos  : OUT    integer RANGE 0 TO 15
   );

-- Declarations

END generate_ptr ;

--
--------------------------------------------------------------------------------
architecture rtl of generate_ptr is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
signal wr_ptr_i : integer range 0 to 4095;
signal rd_ptr_i : integer range 0 to 16383;  -- allowing testpattern access
signal cnt_pos_i : integer range 0 to 15 := 0;
signal shift_info : std_logic_vector(7 downto 0);
signal vec_info : std_logic_vector(7 downto 0);
signal cnt_info : integer range 0 to 15;
signal level_i : integer range 0 to 4095;
signal enp_din : std_logic;
--signal reset_n_i : std_logic;
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, page)
--  variable enp_din : std_logic;
  variable no_data : std_logic;
--  variable level_i : integer range 0 to 4095;
--  variable cnt_info : integer range 0 to 15;
begin
--  enp_din := in_bus(0) and not in_bus_last(0);
--  no_data := not vec_info(1);
  no_data := not vec_info(7);
  if page(0) = '0' then
    wr_ptr_i <= BUF_OFFSET;
    rd_ptr_i <= 0;
    cnt_pos_i <= 0;
    level_i <= BUF_OFFSET;
    shift_info <= (others=>'0');
    vec_info <= (others=>'0');
    cnt_info<=0;
  elsif clk'event and clk = '1' then

    ----------------------------------------------------------------------------
    -- character start marking
    shift_info <= in_bus(2) & shift_info(7 downto 1);
    if cnt_info>0 then
      cnt_info<= cnt_info-1;
    elsif in_bus(2)='1' and shift_info(7)='0' then 
      cnt_info<=8;    
    end if;
    
    if cnt_info=1 then
      vec_info<=shift_info; -- just for debug purposes
      if (enp_dout/='1') then
        cnt_pos_i<=(conv_integer(shift_info(3 downto 1))+level_i) mod 16; -- to be updated for char stripping
      else
        cnt_pos_i<=(conv_integer(shift_info(3 downto 1))+level_i-1) mod 16;
      end if;
    elsif enp_dout='1' and level_i>1 then
      if cnt_pos_i=0 then
        cnt_pos_i<=7;                                      -- to be updated for char stripping; evt vec(7:5)
      else
        cnt_pos_i<=cnt_pos_i-1;
      end if;
    end if;
    
    ----------------------------------------------------------------------------
    -- testpattern generation (variation 2)
    if page="11" then
      if enp_dout='1' then  --and level_i>1 then
        if rd_ptr_i < 8191 then
          rd_ptr_i <= rd_ptr_i + 1;
        else
          rd_ptr_i <= 4096;
        end if;
      end if;
      level_i <= BUF_OFFSET;
--    ----------------------------------------------------------------------------
--    -- starting condition
    elsif no_data='1' then -- when sync switches ON, some bits preload of frame ahead
      wr_ptr_i <= BUF_OFFSET;
      rd_ptr_i <= 0;
      level_i <= BUF_OFFSET;
    ----------------------------------------------------------------------------
    -- fifo activated
    elsif page="01" then
      if enp_din='1' then
        if wr_ptr_i < 4095 then
          wr_ptr_i <= wr_ptr_i + 1;
        else
          wr_ptr_i <= BUF_OFFSET;
        end if;
      end if;
      
--      if no_data='1' and wr_ptr_i >= BUF_OFFSET then -- when sync switches ON, some bits preload of frame ahead
--          rd_ptr_i <= wr_ptr_i-BUF_OFFSET;
      if enp_dout='1' and level_i>1 then
        if rd_ptr_i < 4095 then
          rd_ptr_i <= rd_ptr_i + 1;
        else
          rd_ptr_i <= BUF_OFFSET;
        end if;
      end if;
      
--      if no_data='1' and wr_ptr_i >= BUF_OFFSET then -- when sync switches ON, preload bits of frame ahead
--          level_i := BUF_OFFSET;
      if enp_din='1' and enp_dout='0' then
--        if level_i < 4095 then
          level_i <= level_i+1;                          
--        else                        -- buffer overflow (will not happen)
--          level_i := 0;
--        end if;
      elsif enp_din='0' and enp_dout='1' and level_i>1 then
        level_i <= level_i-1;                 
      -- The following workaraound is introduced to tape a bad dsp-configuration procedure
      -- when after a reconfiguration the mux port 0 is not reenabled again!!!!!
      -- The workaround can not provide right data for the first respectively second frame after sync.
      -- synchronizes the level again in case of faulty operation.                 
      elsif rd_ptr_i=BUF_OFFSET then  
        level_i<=wr_ptr_i-BUF_OFFSET;
      end if;
            
    end if;      
  end if;
  
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
wr_ptr <= wr_ptr_i;
rd_ptr <= rd_ptr_i;
cnt_pos <= cnt_pos_i;
level <= level_i;
enp_din <= in_bus(0);

--
--------------------------------------------------------------------------------
end architecture rtl;
