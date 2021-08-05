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
-- 21    12-02-07 15:39 Chstrue
-- Adaption for rel 0x41
-- 
-- 20    11-08-29 9:40 Chstrue
-- 110822: Ver 40
-- 
-- 19    11-07-06 9:44 Chstrue
-- R4 implementation
-- 
-- 15    11-06-14 14:54 Chstrue
-- Digital voice transit implementation
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
-----------------------------------------------------------
-- Last edited:
--   120201: Ver 41
--   110822: Ver 40
--   110630: R4 implementation


--------------------------------------------------------------------------------
-- CONSTANT Definitions
--------------------------------------------------------------------------------
  
  constant DEF_VERSION : std_logic_vector(7 DOWNTO 0):= X"41"  ; --last official mux was 2A
  
  --history:
  -----------------------------------
  --Ver 2A:
  --  Last official Version before digital transit implementation. (Labeled in source safe)
  -----------------------------------  
  --Ver 2B:
  --  Test version vor emc dig transit debugging
  -----------------------------------  
  --Ver 2C:
  --  digital transit version for EMC test
  -----------------------------------  
  --Ver 2D: 101123
  --  for EMC test, additional bit in OSR(10): warning from R1LB
  -----------------------------------  
  --Ver 2E: 110309 (dsg@zuehlke.com)
  --  new registers for digital transit
  --  TDM path changed for digital transit function
  -----------------------------------  
  --Ver 2F: 110329 (dsg@zuehlke.com)
  --  first digital transit implementation for basic tests
  -----------------------------------  
  --Ver 30: 110407 (dsg@zuehlke.com)
  --  added voice channel mapper for slave
  -----------------------------------  
  --Ver 31: 110411 (dsg@zuehlke.com)
  --  modified voice channel mapper for slave   
  -----------------------------------  
  --Ver 32: 110412 (dsg@zuehlke.com)
  --  modified mux_tdm_rx to prevent overwriting of channels if same channel nr. in TDM and DT path
  -----------------------------------  
  --Ver 33: 110415 (dsg@zuehlke.com)
  -- modified dt_fill_2framer and dt_fill_2dt (copies now the full frame when header manipulation)
  -- added debugging interface (registers are sent over RXD1_debug)
  -----------------------------------  
  --Ver 34: 110421 (dsg@zuehlke.com)
  -- modified dt_fill_ram_rx and dt_port (dt_fill_ram_rx gets now the information when a frame started and finished)
  -- this solved problems in the master RX-path for digital transit when the slave does not send a channel 1 or 16
  -----------------------------------  
  --Ver 35: 110428 (dsg@zuehlke.com)
  -- removed debugging interface
  -- bug: modified dt_fill_ram_rx (sensitive on rising edge of burst_finished because in slave this signal is longer than 1 clk cycle)
  -- improvement: dt_fill_ram_tx, pointer ptr_dt_tx_ram1_i uses the other page -> 10ms less delay
  -----------------------------------  
  --Ver 36: 110428 (dsg@zuehlke.com)
  -- added AIS and other additional error behaviour of the digital transit link
  -----------------------------------  
  --Ver 37: 110428 StR
  -- LAN only trial version    
  -----------------------------------  
  --Ver 38: 110509 (dsg@zuehlke.com)
  -- removed unused registers  
  -- inverted ETH_ALARM (changed to pull-up in UCF-file)
  -- set TX_AGR to '0' in state remainder of state-machine Enframer (to avoid AIS in misconfiguration of an ETL link)
  -- modified no sync signal (always 0 when no digital transit channel enabled)
  -----------------------------------  
  --Ver 00: 110523 (dsg@zuehlke.com)
  -- special version to simulate subframe losses and bit errors
  -----------------------------------  
  --Ver 39: 110525 StR
  -- Generation of MUX_LOS changed: In case of LAN only mode it is the inversion of the ethernet port output enable configuration bit
  -- (of control_enet.poe), otherwise it's the LOS based on the deframer sync.
  -- special version 00 behaviour commented to passivate. In supressor_behave.vhd, biterrorRX_behave.vhd, suppressorS_behave.vhd
  -----------------------------------  
  --Ver 3A: 110610 StR/dsg@zuehlke.com
  -- Generation of ETH_DCD changed: In case of LAN only mode it is the inversion of the ethernet port output enable configuration bit
  -- (of control_enet.poe), otherwise it's the dcd of the serial port based on the deframer sync.
  -- 1st bit of old_subframes_i in module dt_fill_2dt was not saved -> solved
  -- removed debug interface (dt_emvtest_debugif)
  -- disabled error functions for digital transit (functions from rev. 00, 110523)  
  -- Sharing device now on V.24 port: 1,2,3,4. Before: V.24 Port: 1,2,5,6
  -----------------------------------  
  --Ver 3B: 110616 StR
  -- Generation of control_mux.mux_ch corrected concerning V.24 port sharing
  -----------------------------------  
  --Ver 3C: 110704 StR
  -- map_tdm_tx: page switching mechanism changed (including bug)
  -- map_tdm_rx: data valid indication to tdm_schedule changed
  -- mux_ports:  readpointer from framer detached from map_tdm_tx, used signal from dt_2aggr instead.
  -- enframer:   interrupt point connected to preamble state to prevent toggling TDM page
  -----------------------------------  
  --Ver 3D: 110719 StR
  -- Timeout and self-start of master transit burst generation implemented. (Auto AIS insertion in 
  -- case of timeout)
  -----------------------------------  
  --Ver 3E: 110817 StR
  -- Correction for less than 25 baud: 
  --     -Serial_port: DCD generated not anymore regarding always_on
  --     -OFDM port  : con not anymore driven from DCD, now fix '0' because 
  --                   always on is set by the firmware. (Invert all circuits isn't set)
  -----------------------------------  
  --Ver 3F: 110819 StR
  -- DCD generation of OFDM port changed to original behaviour by modification outside the 
  -- serial_port component. (before Rel 3E) Made to satisfy testbench...
  -----------------------------------  
  --Ver 40: 110822 StR
  -- -Change of the serial port component: additional DCD_v24 signal for V.24 ports to solve the 
  --  "<25baud" bug. (DCD signal is again with always_on generated)
  -- -Update of serial_port wherever it is used.
  -- -OFDM port: DCD generation changed back to DCD from serial_port.
  -----------------------------------  
  --Ver 41: 120201 StR
  -- -Pull-up from pin ETH_WARNING changed to pull-down. (No changes in 
  --  VHDL, except version number)
  
  constant DEF_PREAMBLE: std_logic_vector(8 downto 0):= "110011010";-- LSB first
  constant PRE_LEN : integer := DEF_PREAMBLE'length;  -- Length of Preamble field in Mux Frame
  constant CH_MAX : integer  := 26;  -- Maximum Count of Mux Channels
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
  
  -- constant DEF_xx   : integer :=  96;    -- free register
  
  -- configuration register for digital transit
  constant DEF_GDTC    : integer :=  97;    -- general configuration register
  constant DEF_VCE0    : integer :=  98;    -- voice channel enable register for ch 8 down to 1
  constant DEF_VCE1    : integer :=  99;    -- voice channel enable register for ch 16 down to 9
  constant DEF_VCTL0   : integer :=  100;   -- voice channel termination location ch 8 down to 1
  constant DEF_VCTL1   : integer :=  101;   -- voice channel termination location ch 16 down to 9
  constant DEF_VCTP0   : integer :=  102;   -- voice channel termination port number for ch 2 and 1
  constant DEF_VCTP1   : integer :=  103;   -- voice channel termination port number for ch 4 and 3
  constant DEF_VCTP2   : integer :=  104;   -- voice channel termination port number for ch 6 and 5
  constant DEF_VCTP3   : integer :=  105;   -- voice channel termination port number for ch 8 and 7
  constant DEF_VCTP4   : integer :=  106;   -- voice channel termination port number for ch 10 and 9
  constant DEF_VCTP5   : integer :=  107;   -- voice channel termination port number for ch 12 and 11
  constant DEF_VCTP6   : integer :=  108;   -- voice channel termination port number for ch 14 and 13
  constant DEF_VCTP7   : integer :=  109;   -- voice channel termination port number for ch 16 and 15
  
  -- status register for digital transit
  constant DEF_GDTS    : integer :=  110;   -- general digital transit status register
  constant DEF_TND0    : integer :=  111;   -- no data status register for ch 8 down to 1
  constant DEF_TND1    : integer :=  112;   -- no data status register for ch 16 down to 9
  constant DEF_FDCT    : integer :=  113;   -- frame discard counter (TX) for selected channel
  constant DEF_FICT    : integer :=  114;   -- frame insertion counter (TX) for selected channel
  constant DEF_FDCR    : integer :=  115;   -- frame discard counter (RX) for selected channel
  constant DEF_FICR    : integer :=  116;   -- frame insertion counter (RX) for selected channel

  
-------------------------------------------------------------------------------
-- TYPE Definitions
--------------------------------------------------------------------------------

  type t_register is array (0 to 10) of std_logic_vector(7 downto 0);
  type t_brt_vec is array (0 to 10) of std_logic_vector(35 downto 0); -- bitrate vector
  type t_control_v24_x is record
    lom : std_logic;                        -- LAN only mode
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
    wrn : std_logic;
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
    shd3 : std_logic;                            -- port sharing device ch3 activated
    shd4 : std_logic;                            -- port sharing device ch4 activated   
--    shd5 : std_logic;                            -- port sharing device ch5 activated
--    shd6 : std_logic;                            -- port sharing device ch6 activated
    mux : std_logic_vector(11 downto 1);         -- switch ports to mux (currently v24 only)
  end record;
  
  type t_status_switch is record
    dummy : std_logic;
  end record;

--------------------------------------------------------------------------------
  type t_gdtc is record
    ric   : std_logic;
    rdc   : std_logic;
    mss   : std_logic;
  end record;

  type t_vctp is array (1 to 16) of integer range 0 to 15;

  type t_control_digtrans is record
    gdtc  : t_gdtc;
    vce   : std_logic_vector(15 downto 0);
    vctl  : std_logic_vector(15 downto 0);
    vctp  : t_vctp;
  end record;

  -- initial configuration for registers GDTC - VCTP7
  constant DEF_DIG_INIT : t_control_digtrans := ( ('1', '1', '0'), (others => '0'), (others => '0'), (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15) );

  type t_gdts is record
    nsto  : std_logic;
  end record;
  
  type t_status_digtrans is record
    gdts  : t_gdts;
    tnd   : std_logic_vector(15 downto 0);
    fdct  : integer range 0 to 255;
    fict  : integer range 0 to 255;
    fdcr  : integer range 0 to 255;
    ficr  : integer range 0 to 255;
  end record;
  
  type t_ram_ctrl_dt is record
    enp   : std_logic;
    data  : std_logic;
    addr  : integer range 0 to 16383;
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
