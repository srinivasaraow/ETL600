--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.lookup_bitrates.rtl
-- $Workfile: lookup_bitrates_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/lookup_bitrates_rtl.vhd $
-- 
-- 8     11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 5     11-03-08 13:59 Chstrue
-- top level with pin multiplexing
-- 
-- 4     08-03-18 16:58 Chmaglo1
-- comment added
-- 
-- 3     08-03-18 14:35 Chmaglo1
-- Update for 11kbit/s aggregate rate
-- 
-- 2     08-03-18 14:32 Chmaglo1
-- -- no description --
-- 
-- 1     15.11.05 11:12 Chmaglo1
-- LAN interface added, 
-- incl. div. updates for port9 and port10
-- 
-- 46    14.06.05 14:19 Chmaglo1
-- -- no description --
-- 
-- 45    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 17    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 8     29.11.04 11:03 Unp00631
-- -- no description --
-- 
-- 1     15.10.04 17:34 Unp00631
-- before military service
-- 0     29.09.2004 12:00:20 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
-- last edited: 2011-02-21/dsg@zuehlke.com: initialization fixed
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY lookup_bitrates IS
   PORT( 
      brr     : IN     t_register;
      clk     : IN     std_logic;
      reset_n : IN     std_logic;
      brt_vec : OUT    t_brt_vec
   );

-- Declarations

END lookup_bitrates ;

--
--------------------------------------------------------------------------------
architecture rtl of lookup_bitrates is

-----------------------------------------------------------
-- Last edited:
--   110630: no changes


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
type t_rom_bitrates is array (0 to 63) of std_logic_vector(35 downto 0);
constant rom_br_1 : t_rom_bitrates := (
X"2800_0_1_001",
X"1400_0_1_001",
X"0A00_0_1_002",
X"06AA_2_3_003",
X"0355_1_3_006",
X"01AA_2_3_00C",
X"00D5_1_3_018",
X"006A_2_3_030",
X"0047_1_9_048",
X"0035_1_3_060",
X"002A_2_3_078",
X"0023_5_9_090",
X"0020_0_1_0A0",
X"001A_2_3_0C0",
X"0015_1_3_0F0",
X"0011_7_9_120",
X"0010_0_1_140",
X"000E_2_9_168",
X"000D_1_3_180",
X"000C_4_5_190",
X"000A_2_3_1E0",
X"0009_1_7_230",
X"0008_8_9_240",
X"0008_0_1_280",
X"0007_1_9_2D0",
X"0006_2_3_300",
X"0006_2_5_320",
X"0005_1_3_3C0",
X"0004_4_7_460",
X"0004_4_9_480",
X"0004_0_1_500",
X"0003_5_9_5A0",
X"0003_1_3_600",
X"0003_1_5_640",
X"0002_2_3_780",
X"0002_2_7_8C0",
X"0002_2_9_900",
X"0002_0_1_A00",
X"0001_7_9_B40",
X"0001_2_3_C00",
X"0001_3_5_C80",
X"0001_1_3_F00",
X"0001_1_8_180",
X"0001_0_2_400",
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"002E_6_B_06E", -- 11kbit/s for special aggregate rate
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333", -- no good value
X"3333_3_3_333"  -- no good value
);

--attribute block_rom : boolean;
--attribute block_rom of rom_br_1 : signal is false;

signal brt_vec_i : t_brt_vec;
--attribute block_rom of brt_vec_i : signal is false;
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
  if reset_n = '0' then
    brt_vec_i <= (others=>(others=>'0'));
  elsif clk'event and clk = '1' then
--    for i in 0 to 10 loop
--      brt_vec_i(i)(35 downto 0) <= rom_br_1(conv_integer(brr(i)(5 downto 0)));
--    end loop;
      brt_vec_i<=(rom_br_1(conv_integer(brr(0)(5 downto 0))),
                  rom_br_1(conv_integer(brr(1)(5 downto 0))),
                  rom_br_1(conv_integer(brr(2)(5 downto 0))),
                  rom_br_1(conv_integer(brr(3)(5 downto 0))),
                  rom_br_1(conv_integer(brr(4)(5 downto 0))),
                  rom_br_1(conv_integer(brr(5)(5 downto 0))),
                  rom_br_1(conv_integer(brr(6)(5 downto 0))),
                  rom_br_1(conv_integer(brr(7)(5 downto 0))),
                  rom_br_1(conv_integer(brr(8)(5 downto 0))),
                  rom_br_1(conv_integer(brr(9)(5 downto 0))),
                  rom_br_1(conv_integer(brr(10)(5 downto 0)))
                  );
  end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
brt_vec <= brt_vec_i;
--brt_vec <= (rom_br_1(conv_integer(brr(0)(5 downto 0))),
--            rom_br_1(conv_integer(brr(1)(5 downto 0))),
--            rom_br_1(conv_integer(brr(2)(5 downto 0))),
--            rom_br_1(conv_integer(brr(3)(5 downto 0))),
--            rom_br_1(conv_integer(brr(4)(5 downto 0))),
--            rom_br_1(conv_integer(brr(5)(5 downto 0))),
--            rom_br_1(conv_integer(brr(6)(5 downto 0))),
--            rom_br_1(conv_integer(brr(7)(5 downto 0))),
--            rom_br_1(conv_integer(brr(8)(5 downto 0))),
--            rom_br_1(conv_integer(brr(9)(5 downto 0))),
--            rom_br_1(conv_integer(brr(10)(5 downto 0)))
--            );

--
--------------------------------------------------------------------------------
end architecture rtl;
