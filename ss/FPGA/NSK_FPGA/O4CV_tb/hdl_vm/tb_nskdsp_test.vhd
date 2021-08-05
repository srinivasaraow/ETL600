--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_nskdsp.test
-- $Workfile: tb_nskdsp_test.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_nskdsp_test.vhd $
-- 
-- 8     11-07-05 16:28 Chstrue
-- R4 implementation
-- 
-- 7     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 6     07-10-16 9:38 Chmaglo1
-- -- no description --
-- 
-- 3     07-05-19 19:54 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-01 22:14:02 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY O4CV_tb;
USE O4CV_tb.report_assist.all;
LIBRARY std;
USE std.textio.all;

ENTITY tb_nskdsp IS
   PORT( 
      cs_n         : OUT    std_logic;
      rd_n         : OUT    std_logic;
      wr_n         : OUT    std_logic;
      nsk_adr      : OUT    std_logic_vector (11 DOWNTO 0);
      nsk_data     : INOUT  std_logic_vector (7 DOWNTO 0);
      tb_nskdsp_ok : OUT    std_logic;
      testcase     : IN     t_testcase;
      clk_32M      : IN     std_logic
   );

-- Declarations

END tb_nskdsp ;

--
--------------------------------------------------------------------------------
architecture test of tb_nskdsp is

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
  constant MUX_ON : std_logic_vector(7 downto 0) := B"10000000";
  signal testword : std_logic_vector(7 downto 0) := (others=>'0');
  
--------------------------------------------------------------------------------
-- ARCHITECTURE PROCEDURES AND FUNCTIONS
--------------------------------------------------------------------------------


--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  process
    
    -----------------------------------------------------------------------------
    -- PROCESS SUBROUTINES (allows more global signal access than arch. declar.)
    -----------------------------------------------------------------------------
    
    -----------------------------------------------------------------------------
    -- rd_reg: read one register byte from a configuration register
    -----------------------------------------------------------------------------
    procedure rd_reg (
      constant addr      : in    std_logic_vector(11 downto 0);
      signal   value     : out   std_logic_vector(7 downto 0)
      ) is
    begin
      -- positive/negative clock edge shall be clarified since board clock domain reorganisation
      wait until clk_32M = '1';
      cs_n <= '0';
      rd_n <= '0';
      nsk_adr <= addr;
      wait until clk_32M = '1';
      wait until clk_32M = '1';
      wait until clk_32M = '1';
      cs_n <= '1';
      rd_n <= '1';
      nsk_adr <= (others => '0');
      value <= nsk_data(7 downto 0);
      wait until clk_32M = '1';
    end rd_reg;
  
    -----------------------------------------------------------------------------
    -- wr_reg: write one register byte to a configuration register
    -----------------------------------------------------------------------------
    procedure wr_reg (
      constant addr      : in    std_logic_vector(11 downto 0);
      constant value     : in    std_logic_vector(7 downto 0)
      ) is
    begin
      -- positive/negative clock edge shall be clarified since board clock domain reorganisation
      wait until clk_32M = '1';
      cs_n <= '0';
      wr_n <= '0';
      nsk_adr <= addr;
      nsk_data(7 downto 0) <= value;
      wait until clk_32M = '1';
      wr_n <= '1';
      wait until clk_32M = '1';
      cs_n <= '1';
      nsk_adr <= (others => '0');
      nsk_data(7 downto 0) <= (others=>'Z');
      wait until clk_32M = '1';
    end wr_reg;

    
  begin
    tb_nskdsp_ok <= '1';
    cs_n<='1';
    rd_n<='1';
    wr_n<='1';
    nsk_adr<=(others=>'0');
    nsk_data<=(others=>'Z');
    wait on testcase;
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      tb_nskdsp_ok <= '0';
      wait for SIGNAL_ASSIGNMENT;
      printf("_0m: System initialization done");
      tb_nskdsp_ok <= '1';

    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    elsif testcase = TC_CONFIG_IF then
      tb_nskdsp_ok <= '0';
      wr_reg(X"00A", B"00000100"); --CTS test
      wr_reg(X"00A", B"00000010"); --CTS test
      wr_reg(X"00A", B"00000001"); --CTS test
      wr_reg(X"00A", B"00001111"); --CTS test
      rd_reg(X"00A", testword); --CTS test
      assert (testword = B"00001111")
        report "corrupted configuration interface (RSIG); wrong readback value"
        severity failure;
      printf("_0m: Configuration Interface Test Done (Legacy-Part)");

      wr_reg(X"050", B"10101010");
      wr_reg(X"050", B"01010101");
      rd_reg(X"050", testword); 
      assert (testword = B"01010101")
        report "corrupted configuration interface (TCR11); wrong readback value"
        severity failure;
      wr_reg(X"05F", B"01010101");
      wr_reg(X"05F", B"10101010");
      rd_reg(X"05F", testword); 
      assert (testword = B"10101010")
        report "corrupted configuration interface (TCR26); wrong readback value"
        severity failure;
      printf("_0m: Configuration Interface Test Done (O4CV-Part)");
      
--      rd_reg(X"060", testword);
--      assert (testword = B"00001111")
--        report "corrupted configuration interface (reset value of SPA); wrong readback value"
--        severity failure;
      rd_reg(X"061", testword);
      assert (testword = B"00000110")
        report "corrupted configuration interface (reset value of GDTC); wrong readback value"
        severity failure;
      rd_reg(X"066", testword);
      assert (testword = B"00010000")
        report "corrupted configuration interface (reset value of VCTL0); wrong readback value"
        severity failure;
      rd_reg(X"067", testword);
      assert (testword = B"00110010")
        report "corrupted configuration interface (reset value of VCTL1); wrong readback value"
        severity failure;
      rd_reg(X"068", testword);
      assert (testword = B"01010100")
        report "corrupted configuration interface (reset value of VCTL2); wrong readback value"
        severity failure;
      rd_reg(X"069", testword);
      assert (testword = B"01110110")
        report "corrupted configuration interface (reset value of VCTL3); wrong readback value"
        severity failure;
      rd_reg(X"06A", testword);
      assert (testword = B"10011000")
        report "corrupted configuration interface (reset value of VCTL4); wrong readback value"
        severity failure;
      rd_reg(X"06B", testword);
      assert (testword = B"10111010")
        report "corrupted configuration interface (reset value of VCTL5); wrong readback value"
        severity failure;
      rd_reg(X"06C", testword);
      assert (testword = B"11011100")
        report "corrupted configuration interface (reset value of VCTL6); wrong readback value"
        severity failure;
      rd_reg(X"06D", testword);
      assert (testword = B"11111110")
        report "corrupted configuration interface (reset value of VCTL7); wrong readback value"
        severity failure;
      
--      wr_reg(X"060", B"00000101");
--      wr_reg(X"060", B"00000000");
--      rd_reg(X"060", testword); 
--      assert (testword = B"00000000")
--        report "corrupted configuration interface (SPA); wrong readback value"
--        severity failure;
      printf("_0m: Configuration Interface for digital transit registers Test Done");
      
      wr_reg(X"05F", X"00");
      wr_reg(X"05F", X"00");
      tb_nskdsp_ok <= '1';

    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    elsif testcase = TC_LEGACY_DATA then
      tb_nskdsp_ok <= '0';
      --basic V.24 to be added
      wr_reg(X"00A", X"0f");
--      --basic V.11/1, enable channel
--      wr_reg(X"041", X"11");
--      wr_reg(X"042", X"91");
--      wr_reg(X"043", X"00");
--      wr_reg(X"044", X"09");
      printf("_0m: Legacy Data  Test done");
      tb_nskdsp_ok <= '1';

    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    elsif testcase = TC_PAGE_INTERFERENCE_1 then
      tb_nskdsp_ok <= '0';
      wr_reg(X"050", (MUX_ON or X"38")); --CH11
      wr_reg(X"051", (MUX_ON or X"38")); --CH12
      wr_reg(X"052", (MUX_ON or X"38")); --CH13
      wr_reg(X"053", (MUX_ON or X"38")); --CH14
      wr_reg(X"054", (MUX_ON or X"38")); --CH15
      wr_reg(X"055", (MUX_ON or X"38")); --CH16
      wr_reg(X"056", (MUX_ON or X"38")); --CH17
      wr_reg(X"057", (MUX_ON or X"38")); --CH18
      wr_reg(X"058", (MUX_ON or X"38")); --CH19
      wr_reg(X"059", (MUX_ON or X"38")); --CH20
      wr_reg(X"05A", (MUX_ON or X"38")); --CH21
      wr_reg(X"05B", (MUX_ON or X"38")); --CH22
      wr_reg(X"05C", (MUX_ON or X"38")); --CH23
      wr_reg(X"05D", (MUX_ON or X"38")); --CH24
      wr_reg(X"05E", (MUX_ON or X"38")); --CH25
      wr_reg(X"05F", (MUX_ON or X"38")); --CH26

      wr_reg(X"017", (X"25") ); --aggregate port 256kbit/s
      wr_reg(X"014", (X"11") ); --aggregate port enable
      printf("_0m: Full TDM load; all channels enabled");
      tb_nskdsp_ok <= '1';

    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    elsif testcase = TC_PAGE_INTERFERENCE_5 then
      tb_nskdsp_ok <= '0';
      wr_reg(X"014", (X"00") ); --aggregate port disable

      wr_reg(X"050", (MUX_ON or X"43")); --CH11
      wr_reg(X"051", (X"00")); --CH12
      wr_reg(X"052", (X"00")); --CH13
      wr_reg(X"053", (X"00")); --CH14
      wr_reg(X"054", (X"00")); --CH15
      wr_reg(X"055", (X"00")); --CH16
      wr_reg(X"056", (X"00")); --CH17
      wr_reg(X"057", (X"00")); --CH18
      wr_reg(X"058", (X"00")); --CH19
      wr_reg(X"059", (X"00")); --CH20
      wr_reg(X"05A", (X"00")); --CH21
      wr_reg(X"05B", (X"00")); --CH22
      wr_reg(X"05C", (X"00")); --CH23
      wr_reg(X"05D", (X"00")); --CH24
      wr_reg(X"05E", (X"00")); --CH25
      wr_reg(X"05F", (X"00")); --CH26

      wr_reg(X"017", (X"09") ); --aggregate port 9.6kbit/s
      wr_reg(X"014", (X"11") ); --aggregate port enable
      printf("_0m: Full TDM load; all channels enabled");
      tb_nskdsp_ok <= '1';


    end if;
    
  end process;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture test;

