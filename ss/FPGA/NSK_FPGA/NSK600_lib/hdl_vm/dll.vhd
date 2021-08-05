--------------------------------------------------------------------------
-- $Workfile: dll.vhd $
-- Part of  	:  	ETL600
-- Language  	:  	VHDL
-- Created by  	:  	Alexander Gygax, PTUKT2
-- Remarks  	:  
-- Purpose  	:   	NSK600
-- Copyright  	:  	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dll.vhd $
-- 
-- 5     11-07-06 9:40 Chstrue
-- R4 implementation
-- 
-- 3     11-06-14 14:56 Chstrue
-- Digital voice transit implementation
-- 
-- 1     15.11.05 13:58 Chmaglo1
-- -- no description --
-- 
-- 5     8.07.04 16:27 Unp00631
-- -- no description --
-- 
-- 4     6.07.04 10:47 Gygax02
-- generics removed
-- 
-- 3     5.07.04 19:02 Unp00631
-- -- no description --
-- 
-- 2     25.06.04 11:15 Gygax02
-- -- no description --
-- 
-- 1     26.05.04 13:45 Unp00631
-- -- no description --
-- 
-- 1     21.05.04 14:33 Gygax02
-- Initial version
--
---------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.vital_timing.all;

library unisim;
use unisim.vcomponents.all;
--use unisim.smodel.all;


entity dll is
    port (clkin    : in  std_logic;
          reset_n  : in  std_logic;
          clk2x    : out std_logic;
          clk1x    : out std_logic;
          locked   : out std_logic);
end dll;

architecture structural of dll is

-----------------------------------------------------------
-- Last edited:
--   110630: no changes



signal reset        : std_logic;
signal clkin_w      : std_logic;
signal clk1x_dll    : std_logic;
signal clk2x_dll    : std_logic;
signal clk1xint     : std_logic;

  -- Component Declarations
  COMPONENT BUFG
  -- pragma synthesis_off
--  GENERIC (
--    TimingChecksOn : Boolean          := TRUE;
--    InstancePath   : STRING           := "*";
--    Xon            : Boolean          := TRUE;
--    MsgOn          : Boolean          := TRUE;
--    tpd_I_O        : VitalDelayType01 := (0.100 ns, 0.100 ns);
--    tipd_I         : VitalDelayType01 := (0.000 ns, 0.000 ns)
--  );
  -- pragma synthesis_on
  PORT (
    O : OUT    STD_ULOGIC ;
    I : IN     STD_ULOGIC 
  );
  END COMPONENT;
  COMPONENT CLKDLL
  -- pragma synthesis_off
--  GENERIC (
--    TimingChecksOn        : boolean          := FALSE;
--    InstancePath          : STRING           := "*";
--    Xon                   : boolean          := TRUE;
--    MsgOn                 : boolean          := TRUE;
--    tipd_CLKIN            : VitalDelayType01 := (0.000 ns, 0.000 ns);
--    tipd_CLKFB            : VitalDelayType01 := (0.000 ns, 0.000 ns);
--    tipd_RST              : VitalDelayType01 := (0.000 ns, 0.000 ns);
--    tpd_CLKIN_LOCKED      : VitalDelayType01 := (0.100 ns, 0.100 ns);
--    tperiod_CLKIN_POSEDGE : VitalDelayType   := 0.010 ns;
--    MAXPERCLKIN           : time             := 100 ns;
--    tpw_CLKIN_posedge     : VitalDelayType   := 0.010 ns;
--    tpw_CLKIN_negedge     : VitalDelayType   := 0.010 ns;
--    tpw_RST_posedge       : VitalDelayType   := 0.010 ns;
--    FACTORY_JF            : bit_vector       := X"00";
--    STARTUP_WAIT          : boolean          := false;
--    DUTY_CYCLE_CORRECTION : boolean          := FALSE;
--    CLKDV_DIVIDE          : real             := 2.0;
--    SIM_CLKIN_CYCLE_JITTER: time             := 100 ns
--  );
  -- pragma synthesis_on
  PORT (
    CLKIN  : IN     std_ulogic  := '0';
    CLKFB  : IN     std_ulogic  := '0';
    RST    : IN     std_ulogic  := '0';
    CLK0   : OUT    std_ulogic  := '0';
    CLK90  : OUT    std_ulogic  := '0';
    CLK180 : OUT    std_ulogic  := '0';
    CLK270 : OUT    std_ulogic  := '0';
    CLK2X  : OUT    std_ulogic  := '0';
    CLKDV  : OUT    std_ulogic  := '0';
    LOCKED : OUT    std_ulogic  := '0'
  );
  END COMPONENT;
  COMPONENT IBUFG
  -- pragma synthesis_off
--  GENERIC (
--    TimingChecksOn : Boolean          := false;
--    InstancePath   : STRING           := "*";
--    Xon            : Boolean          := True;
--    MsgOn          : Boolean          := False;
--    tpd_I_O        : VitalDelayType01 := (0.000 ns, 0.000 ns);
--    tipd_I         : VitalDelayType01 := (0.000 ns, 0.000 ns)
--  );
  -- pragma synthesis_on
  PORT (
    O : OUT    STD_ULOGIC ;
    I : IN     STD_ULOGIC 
  );
  END COMPONENT;



begin 
  reset  <=  not(reset_n);

  clkpad : IBUFG
    port map(I=>clkin,
             O=>clkin_w);

  dll : CLKDLL
    port map(CLKIN=>clkin_w,
             CLKFB=>clk1xint,
             RST=>reset,
             CLK0=>clk1x_dll,
             CLK90=>open,
             CLK180=>open,
             CLK270=>open,
             CLK2X=>clk2x_dll,
             CLKDV=>open,
             LOCKED=>locked);

  clk1xg : BUFG
    port map(I=>clk1x_dll,
             O=>clk1xint);
  clk2xg : BUFG
    port map(I=>clk2x_dll,
             O=>clk2x);

  clk1x <= clk1xint;

end structural;