--------------------------------------------------------------------------
-- $Workfile: typedef_p.vhd $
-- Part of  : ETL600
-- Language : VHDL
-- Created by : Alexander Gygax, PTUKT2
-- Remarks  :  
-- Purpose  :   NSK600
-- Copyright  : ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/typedef_p.vhd $
-- 
-- 14    08-04-24 11:53 Chstrue
-- Version number incremented
-- 
-- 13    08-03-25 13:13 Chstrue
-- Version number incremented
-- 
-- 12    07-10-11 15:22 Chmaglo1
-- -- no description --
-- 
-- 6     3.10.06 12:45 Chmaglo1
-- -- no description --
-- 
-- 4     24.05.06 14:20 Chmaglo1
-- -- no description --
-- 
-- 3     9.01.06 15:40 Chalgyg
-- LAN port fixed. Now it can be operated without V.11 port 1.
-- 
-- 2     15.11.05 19:40 Chmaglo1
-- check in to new VSS path
-- 
-- 1     15.11.05 11:12 Chmaglo1
-- LAN interface added, 
-- incl. div. updates for port9 and port10
-- 
-- 68    28.10.05 11:14 Chmaglo1
-- -- no description --
-- 
-- 67    25.10.05 17:18 Chalgyg
-- Version 17
-- 
-- 66    10.10.05 10:42 Chalgyg
-- Version 16
-- 
-- 65    17.09.05 9:24 Chmaglo1
-- -- no description --
-- 
-- 63    12.07.05 15:31 Chalgyg
-- Changed Version to 14.
-- 
-- 62    14.06.05 14:42 Chalgyg
-- Version label changed to 13.
-- 
-- 61    14.06.05 14:18 Chmaglo1
-- Aenderung für control_nsk.dcd
-- 
-- 60    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 35    19.01.05 16:43 Chmaglo1
-- -- no description --
-- 
-- 17    1.12.04 10:41 Unp00631
-- Migration Test 2
-- 
-- 5     20.09.04 10:39 Unp00631
-- -- no description --
-- 
-- 4     9.09.04 15:18 Unp00631
-- -- no description --
-- 
-- 1     26.05.04 13:45 Unp00631
-- -- no description --
-- 
-- 1     21.05.04 14:33 Gygax02
-- Initial version
--
---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package typedef_p is

--------------------------------------------------------------------------------
-- CONSTANT Definitions
--------------------------------------------------------------------------------
  constant DEF_VERSION : std_logic_vector(7 DOWNTO 0):= X"2A";
  constant DEF_PREAMBLE: std_logic_vector(8 downto 0):= "110011010";-- LSB first
  constant PRE_LEN : integer := DEF_PREAMBLE'length;  -- Length of Preamble field in Mux Frame
  constant CH_MAX : integer := 26;  -- Maximum Count of Mux Channels
  constant CH_SYNC : integer := 16; -- Maximum Count of System Synchronous Channels (e.g. O4CV channels)
  constant ADD_LEN : integer := 2;  -- Length of ADD field in Mux Frame
  constant POS_LEN : integer := 3;  -- Length of POS field in Mux Frame

  constant DEF_SPEEDFRACT : integer := 1665;     -- 600ppm per speed_level step
  constant DEF_SPEEDFRACT_F : integer := 9998;   -- 100ppm per speed_level step
  constant BUF_NORMRANGE : integer := 4; -- Tolerated buffer level deviation causing no max regulation
  constant BUF_OFFSET : integer := 7; -- mux fifo offset (preload)
  

  -- V24 Signalling
  constant V24_OFF   : std_logic := '1';
  constant V24_ON    : std_logic := '0';
  constant V24_MARK  : std_logic := '1';
  constant V24_SPACE : std_logic := '0';

  -- FPGA Identification Register Address
  constant DEF_IDR     : integer := 0;
  
  -- NSK Functionality Registers (4 channels) Addresses
  constant DEF_TSIG    : integer := 1;
  constant DEF_TUD1    : integer := 2;
  constant DEF_TUD2    : integer := 3;
  constant DEF_TUD3    : integer := 4;
  constant DEF_TUD4    : integer := 5;
  constant DEF_RUD1    : integer := 6;
  constant DEF_RUD2    : integer := 7;
  constant DEF_RUD3    : integer := 8;
  constant DEF_RUD4    : integer := 9;
  constant DEF_RSIG    : integer := 10;
  
  -- port offset (OffSet CHannel n)
  constant DEF_OSCH0   : integer :=  20;    -- port configuration start address
  constant DEF_REGCNTCH: integer :=  5;     -- port register count
  constant DEF_OSCH11  : integer :=  80;    -- (0x50) tdm port config start address

  -- configuration register for each channel   
  constant DEF_PCR     : integer :=  0;     -- port configuration register
  constant DEF_CCR     : integer :=  1;     -- clock configuration register
  constant DEF_TPR     : integer :=  2;     -- transmission protocol register
  constant DEF_BRR     : integer :=  3;     -- bit-rate register
  -- status register for each channel
  constant DEF_OSR     : integer :=  4;     -- operational status register
  
--------------------------------------------------------------------------------
-- TYPE Definitions
--------------------------------------------------------------------------------
  type t_register is array (0 to 10) of std_logic_vector(7 downto 0);
  type t_brt_vec is array (0 to 10) of std_logic_vector(35 downto 0); -- bitrate vector
  type t_control_v24_x is record
    poe : std_logic;                        -- enable port (RXD) / equals DCD
    tpt : std_logic;                        -- testpattern generation enabled (test only)
    ltr : std_logic;                        -- loop TxD to RxD
    lrc : std_logic;                        -- loop RTS to CTS
    aon : std_logic;                        -- Signalling always ON
    jac : std_logic;                        -- jabber control enabled
    das : std_logic;                        -- data sensitive / hw-handshake off
    mux : std_logic;                        -- multiplexer channel enabled
    fsk : std_logic;                        -- fsk channel enabled
    clr : std_logic;                        -- clock regulator enabled
    asy : std_logic;                        -- asynchronous TxD enabled
    rxc : std_logic;                        -- enable rx clock
    txc : std_logic;                        -- enable tx clock
    icp : std_logic;                        -- invert clock polarity
    ict : std_logic;                        -- inverse clock edge Tx/Rx
    iac : std_logic;                        -- invert all circuits
    chl : integer range 0 to 7;             -- character length
    basecount   : integer range 0 to 16383; -- bitrate basecount  
    numerator   : integer range 0 to 15;    -- bitrate numerator  
    denominator : integer range 0 to 15;    -- bitrate denominator
  end record;
  type t_control_v24 is array (1 to 6) of t_control_v24_x;
  
  type t_status_v24_x is record
    jah : std_logic;                        -- jabber halt 
  end record;
  type t_status_v24 is array (1 to 6) of t_status_v24_x;

--------------------------------------------------------------------------------
  type t_control_nsk is record
    dcd : std_logic;
    cts : std_logic;
    fsk : std_logic;   
  end record;
  
  type t_status_nsk is record
    rts : std_logic;
  end record;

--------------------------------------------------------------------------------
  subtype t_control_v11_x is t_control_v24_x; 
  subtype t_status_v11_x is t_status_v24_x; 
  type t_control_v11 is array (1 to 2) of t_control_v11_x;
  type t_status_v11 is array (1 to 2) of t_status_v11_x;

--------------------------------------------------------------------------------
--  subtype t_control_ofdm is t_control_v24_x;  --makes syntax problems in port map
  subtype t_status_ofdm is t_status_v24_x;
  
--------------------------------------------------------------------------------
  subtype t_control_g703 is t_control_v24_x; 
 
  type t_status_g703 is record
    los : std_logic;
  end record;

----------------------------------------------------------------------------------
--  subtype t_control_enet is t_control_v24_x;  --makes syntax problems in port map
    
  type t_status_enet is record
    nbc : std_logic;
    nbo : std_logic;
  end record;


--------------------------------------------------------------------------------
  type t_bitcounts is array (0 to CH_MAX) of integer range 0 to 4095;
  type t_levels is array (0 to CH_MAX-CH_SYNC) of integer range 0 to 4095;
  type t_poscounts is array (0 to 10) of integer range 0 to 15;
  
  type t_control_mux is record
    poe : std_logic;                            -- release framer, deframer, sync and tx-fifos
    mux_ch : std_logic_vector(26 downto 1);     -- mux enable per port
    tpt : std_logic;                            -- testpattern generation enabled (test only)
    bitcounts  : t_bitcounts;                   -- amount of bits per channel and frame
    poscounts : t_poscounts;                    -- amount of bits for char position information
  end record;
  
  type t_status_mux is record
    los : std_logic; -- loss of sync
    sync_history : std_logic_vector(7 downto 0);-- for debugging purposes only  
  end record;
  
--------------------------------------------------------------------------------
  type t_cnt_tdmframe is array (0 to 3) of integer range 0 to 63;

  type t_tdmregs is array (0 to 15) of std_logic_vector(7 downto 0);
  type t_control_tdm_x is record
    mux : std_logic;                            -- testpattern generation enabled (test only)
    lpt : std_logic;                            -- loop tdm bus directly
  end record;
  type t_control_tdm is array (0 to 15) of t_control_tdm_x;

  type t_status_tdm is record
    dummy : std_logic;                            -- testpattern generation enabled (test only)
  end record;
  
--------------------------------------------------------------------------------
  type t_control_switch is record
    shd1 : std_logic;                            -- port sharing device ch1 activated
    shd2 : std_logic;                            -- port sharing device ch2 activated
    shd5 : std_logic;                            -- port sharing device ch5 activated
    shd6 : std_logic;                            -- port sharing device ch6 activated
    mux : std_logic_vector(11 downto 1);          -- switch ports to mux (currently v24 only)
  end record;
  
  type t_status_switch is record
    dummy : std_logic;
  end record;
 
-------------------------------------------------------------------------------
-- PROCEDURE AND FUNCTION DECLARATIONS
-------------------------------------------------------------------------------
--function generate_ram_pointer (
--    cnt_tdmframe : t_cnt_tdmframe;  
--    cnt_tdmpos    : integer range 0 to 128;
--    page :  std_logic_vector(15 downto 0)
--    )
--  return integer;
--
--function generate_ram_pointer (
--    cnt_tdmframe : t_cnt_tdmframe;  
--    cnt_tdmpos    : integer range 0 to 128;
--    page :  std_logic_vector(3 downto 0)
--    )
--  return integer;


end typedef_p;
