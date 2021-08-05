--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.collect_status.rtl
-- $Workfile: collect_status_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/collect_status_rtl.vhd $
-- 
-- 3     26.01.06 14:36 Chalgyg
-- G.703 LOS signaling debugged.
-- 
-- 2     15.11.05 19:40 Chmaglo1
-- check in to new VSS path
-- 
-- 1     15.11.05 11:12 Chmaglo1
-- LAN interface added, 
-- incl. div. updates for port9 and port10
-- 
-- 38    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 21    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 9     29.11.04 16:20 Unp00631
-- -- no description --
-- 
-- 1     15.10.04 17:34 Unp00631
-- before military service
-- 0     15.10.2004 14:00:24 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY collect_status IS
   PORT( 
      clk           : IN     std_logic;
      reset_n       : IN     std_logic;
      status_enet   : IN     t_status_enet;
      status_g703   : IN     t_status_g703;
      status_mux    : IN     t_status_mux;
      status_nsk_1  : IN     t_status_nsk;
      status_nsk_2  : IN     t_status_nsk;
      status_nsk_3  : IN     t_status_nsk;
      status_nsk_4  : IN     t_status_nsk;
      status_ofdm   : IN     t_status_ofdm;
      status_switch : IN     t_status_switch;
      status_v11    : IN     t_status_v11;
      status_v24    : IN     t_status_v24;
      osr           : OUT    t_register;
      tsig_nsk      : OUT    std_logic_vector (3 DOWNTO 0)
   );

-- Declarations

END collect_status ;

--
--------------------------------------------------------------------------------
architecture rtl of collect_status is
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
process (clk, reset_n)
begin
if reset_n = '0' then
elsif clk'event and clk = '1' then
end if;
end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
osr(0)(1) <= status_mux.los;
osr(0)(7 downto 2) <= status_mux.sync_history(5 downto 0);
osr(1)(0) <= status_v24(1).jah;
osr(2)(0) <= status_v24(2).jah;
osr(3)(0) <= status_v24(3).jah;
osr(4)(0) <= status_v24(4).jah;
osr(5)(0) <= status_v24(5).jah;
osr(6)(0) <= status_v24(6).jah;

osr(9)(1) <= status_g703.los;
osr(10)(1 downto 0) <= status_enet.nbo & status_enet.nbc;

tsig_nsk(0) <= status_nsk_1.rts;
tsig_nsk(1) <= status_nsk_2.rts;
tsig_nsk(2) <= status_nsk_3.rts;
tsig_nsk(3) <= status_nsk_4.rts;
--
--------------------------------------------------------------------------------
end architecture rtl;
