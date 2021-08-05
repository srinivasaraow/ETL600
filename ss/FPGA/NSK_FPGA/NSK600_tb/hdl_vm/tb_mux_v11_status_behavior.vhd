--------------------------------------------------------------------------
-- $Workfile: tb_mux_v11_status_behavior.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_mux_v11_status_behavior.vhd $
-- 
-- 9     11-07-06 9:21 Chstrue
-- R4 implementation
-- 
-- 8     19.01.06 16:17 Chalgyg
-- G.703 interface integrated.
-- 
-- 7     10.01.06 9:06 Chalgyg
-- LAN interface is now supported.
-- 
-- 4     14.10.04 16:22 Gygax02
-- -- no description --
-- 
-- 3     7.10.04 17:10 Gygax02
-- -- no description --
-- 
-- 2     19.08.04 11:31 Gygax02
-- -- no description --
-- 
-- 1     19.08.04 11:05 Gygax02
-- -- no description --
-- 
---------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_tb;
USE NSK600_tb.tb_config_p.all;

ENTITY tb_mux_v11_status IS
   PORT( 
      failure0    : IN     std_logic;
      failure1    : IN     std_logic;
      txd_busy0   : IN     std_logic;
      txd_busy1   : IN     std_logic;
      txd_busy3   : IN     std_logic;
      v11_busy    : OUT    std_logic_vector (3 DOWNTO 0);
      v_11_status : OUT    std_logic_vector (1 DOWNTO 0);
      txd_busy2   : IN     std_logic
   );

-- Declarations

END tb_mux_v11_status ;

ARCHITECTURE behavior OF tb_mux_v11_status IS

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


BEGIN
  
    v_11_status(1)  <= not(txd_busy1 or txd_busy0);
    v_11_status(0)  <= failure1 or failure0;
    v11_busy        <= txd_busy3 & txd_busy2 & txd_busy1 & txd_busy0;
  
END ARCHITECTURE behavior;

