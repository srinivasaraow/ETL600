--------------------------------------------------------------------------------
-- VHDL Architecture O4CV_tb.tb_sport13.test
-- $Workfile: tb_sport13_test.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : CHMAGLO1.UNKNOWN (CH-L-0012267)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/O4CV_tb/hdl_vm/tb_sport13_test.vhd $
-- 
-- 9     11-07-05 16:31 Chstrue
-- R4 implementation
-- 
-- 8     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 7     07-10-16 9:38 Chmaglo1
-- -- no description --
-- 
-- 4     07-05-19 19:54 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-01 22:13:20 Mathias Gloor (MGL)
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

ENTITY tb_sport13 IS
   PORT( 
      testcase      : IN     t_testcase;
      tb_sport13_ok : OUT    std_logic;
      clk_32M       : IN     std_logic;
      tdm_fs        : OUT    std_logic;
      tdm_data      : INOUT  std_logic
   );

-- Declarations

END tb_sport13 ;

--
--------------------------------------------------------------------------------
architecture test of tb_sport13 is

-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4


--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------
constant MAX_STREAM : integer := 1200;

type t_cnt_4int is array (0 to 3) of integer;
signal cnt_327680 : t_cnt_4int := (others=>0);
signal cnt_txsubframe : t_cnt_4int := (others=>0);
signal cnt_rxsubframe : t_cnt_4int := (others=>0);
signal tx_30ms_start : std_logic_vector(3 downto 0) := (others=>'0');
signal rx_scan_start : std_logic_vector(3 downto 0) := (others=>'0');

type t_stream_all is array (0 to 15) of std_logic_vector(1199 downto 0);
signal stream_tx : t_stream_all := (others=>(others=>'0'));
type t_stream_b2b is array (0 to 15) of std_logic_vector(401 downto 0);
signal stream_rx : t_stream_b2b := (others=>(others=>'0'));
signal stream_tx_b2b : t_stream_b2b := (others=>(others=>'0'));

type t_ptr_16int is array (0 to 15) of integer range 0 to 1199;
signal subframe_length : t_ptr_16int := (others=>1);

signal rxframe10ms_ready : std_logic_vector(15 downto 0);

--------------------------------------------------------------------------------
-- PROCEDURES AND FUNCTIONS
--------------------------------------------------------------------------------

  -----------------------------------------------------------------------------
  -- gen_stream: generates a complete bitstream for one channel including one test
  -----------------------------------------------------------------------------
  function gen_stream (
    pattern : std_logic_vector) 
  return std_logic_vector is
    variable stream : std_logic_vector (MAX_STREAM-1 downto 0);
    
  begin
    stream := (others=>'0');
    for i in 0 to 2 loop -- once for every subframe
      stream(stream'high-i*400 downto stream'high-pattern'length/3+1-i*400) 
        := pattern(pattern'high-i*pattern'length/3 downto pattern'high-(i+1)*pattern'length/3+1);
    end loop;
    return stream;
  end gen_stream;

  -- function for checking tx vx. rx stream a back to back version is needed
  function gen_stream_b2b (
    pattern : std_logic_vector) 
  return std_logic_vector is
    variable stream : std_logic_vector (401 downto 0);
    
  begin
    stream := (others=>'0');
    stream(pattern'high downto pattern'low) := pattern;
    return stream;
  end gen_stream_b2b;


--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
  frame_10ms_proc : process (clk_32M)
  begin
    if clk_32M'event and clk_32M='1' then
      for board in 0 to 3 loop
        if tx_30ms_start(board)='1' then
          cnt_327680(board) <= 0;
          cnt_txsubframe(board) <= 0;
        else
          if cnt_327680(board)=327679 then
            cnt_327680(board) <= 0;
            if cnt_txsubframe(board)<2 then
              cnt_txsubframe(board) <= cnt_txsubframe(board)+1;
            else 
              cnt_txsubframe(board)<=0;
            end if;         
          else
            cnt_327680(board) <= cnt_327680(board) + 1;
          end if;
        end if;
      end loop;
    end if;
  end process frame_10ms_proc;
 
  --------------------------------------------------------------------------------
  tdm_93us_proc : process
    constant STARTSLOT_RX : integer := 69;
    constant STARTSLOT_TX : integer := 74;
    constant N_SLOTS : integer := 5;

    type t_tdmfrag is array (0 to 4) of std_logic_vector(31 downto 0);
    variable tdmfrag_tx : t_tdmfrag;
    variable tdmfrag_rx : t_tdmfrag;

    variable ptr_93us_txframe : t_ptr_16int := (others=>1);
    variable ptr_93us_rxframe : t_ptr_16int := (others=>1);
    variable ptr_rxbyte_start : t_ptr_16int := (others=>0);
    variable ch_rx : integer;
    variable cnt_bit_rxframe : t_ptr_16int := (others=>0);

  -----------------------------------------------------------------------------
  -- gen_tdmfrag: generates the concerning fragment for the next TDM-frame 
  function gen_tdmfrag (
    stream : t_stream_all;
    cnt_subframe : t_cnt_4int;
    ptr_fragment : t_ptr_16int;
    subframe_length : t_ptr_16int)
  return t_tdmfrag is
    variable fragment : t_tdmfrag;
  
    begin
      fragment(0) := (others=>'0');  -- default value
      for ch in 0 to 15 loop
        if ptr_fragment(ch)=0 and subframe_length(ch)>0 then 
          fragment(0)(ch/4) := '1'; -- start subframe marker
        end if;
      end loop;
      
      for ch in 0 to 15 loop
        if ptr_fragment(ch)<stream(ch)'length/3/8 then  -- not every TDM-frame fragment is showed in ptr_fragment since not every one includes relevant data
          fragment(ch/4+1)((8*(ch MOD 4)+7) downto (8*(ch MOD 4))) 
            := stream(ch)(stream(ch)'left - ptr_fragment(ch)*8 - 400*cnt_subframe(ch/4) downto stream(ch)'left-7- ptr_fragment(ch)*8 - 400*cnt_subframe(ch/4));
        else
          fragment(ch/4+1)((8*(ch MOD 4)+7) downto (8*(ch MOD 4))) := (others=>'0');
        end if;
            
      end loop;
      
      return fragment; 
    end gen_tdmfrag;
    
  begin -- runs once for every tdm-frame (93.75us)
    for ch in 0 to 15 loop
      if cnt_327680(ch/4)<3072 then  --each 10ms frame
        ptr_93us_txframe(ch) := 0;
      else
        ptr_93us_txframe(ch) := ptr_93us_txframe(ch)+1;
      end if;
      
    end loop;

    tdmfrag_tx := gen_tdmfrag(stream_tx, cnt_txsubframe, ptr_93us_txframe, subframe_length);
    
    for cnt_tdmslot in 0 to 95 loop
      for cnt_tdmbit in 31 downto 0 loop
        
        if cnt_tdmslot=0 and cnt_tdmbit=31 then 
          tdm_fs <= '1';  -- Frame Sync Pulse
        else 
          tdm_fs <= '0';
        end if;                
    ------------------------------------------------------------------------------
    -- TX part
        if cnt_tdmslot>=STARTSLOT_TX and cnt_tdmslot<STARTSLOT_TX+N_SLOTS then
          tdm_data <= tdmfrag_tx(cnt_tdmslot-STARTSLOT_TX)(cnt_tdmbit);
        else
          tdm_data <= 'Z';
        end if;
        wait until clk_32M = '1';  --master dsp writes on falling edge, reads on rising edge
        
    ------------------------------------------------------------------------------
    -- RX part
        if cnt_tdmslot>=STARTSLOT_RX and cnt_tdmslot<STARTSLOT_RX+N_SLOTS then
          tdmfrag_rx(cnt_tdmslot-STARTSLOT_RX)(cnt_tdmbit) := tdm_data;
          
          if cnt_tdmslot>STARTSLOT_RX then
            ch_rx:=(cnt_tdmslot-STARTSLOT_RX-1)*4 + cnt_tdmbit/8;
            if tdmfrag_rx(0)(ch_rx/4)='1' and cnt_tdmslot=STARTSLOT_RX+1+ch_rx/4 and cnt_tdmbit MOD 8 =7 then
              cnt_bit_rxframe(ch_rx):=0;
            elsif cnt_bit_rxframe(ch_rx)<subframe_length(ch_rx) then
              cnt_bit_rxframe(ch_rx):=cnt_bit_rxframe(ch_rx)+1;
            end if;
            if cnt_bit_rxframe(ch_rx)<subframe_length(ch_rx) then
              stream_rx(ch_rx)<=stream_rx(ch_rx)(stream_rx(ch_rx)'high-1 downto 0) & tdm_data;
            end if;
          end if;
        end if;
        wait until clk_32M = '0';  --master dsp writes on falling edge, reads on rising edge 
        
      end loop;
    end loop;


    for ch in 0 to 15 loop
      if tdmfrag_rx(0)(ch/4)='1' then
        ptr_93us_rxframe(ch):=0;
        if rx_scan_start(ch/4)='1' then
          cnt_rxsubframe(ch/4)<=0;
        else 
          cnt_rxsubframe(ch/4)<=cnt_rxsubframe(ch/4)+1;
        end if;
      elsif ptr_93us_rxframe(ch)<63 then
        ptr_93us_rxframe(ch):=ptr_93us_rxframe(ch)+1;
      end if;

      if ptr_93us_rxframe(ch)=(subframe_length(ch)+7)/8 then
        rxframe10ms_ready(ch)<='1';
      else
        rxframe10ms_ready(ch)<='0';
      end if;

    end loop;
  end process tdm_93us_proc;
  
  --------------------------------------------------------------------------------
  testcase_proc : process

  procedure check_onech_30ms (
    constant stream_ch : in std_logic_vector;
    constant pat   : in std_logic_vector) is
  begin  -- exit
--  assert (stream_ch(pattern'high downto pattern'low)=pattern()
--      or stream_ch(pattern'high+pattern'length/3 downto pattern'low+pattern'length/3)=pattern
--      or stream_ch(pattern'high+2*(pattern'length/3) downto pattern'low+2*(pattern'length/3))=pattern
--      )
    assert (stream_ch(pat'range)=pat
      or stream_ch(pat'range)=pat(2*(pat'length/3)-1 downto 0*(pat'length/3)) & pat(3*(pat'length/3)-1 downto 2*(pat'length/3))
      or stream_ch(pat'range)=pat(1*(pat'length/3)-1 downto 0*(pat'length/3)) & pat(3*(pat'length/3)-1 downto 1*(pat'length/3))
      )
      report "Channel " & "xy" & " incorrectly received #################################################### Failure"
      severity note;
  end check_onech_30ms;

  procedure check_allch_30ms (
    constant stream_rx : in t_stream_b2b;
    constant pattern   : in std_logic_vector) is
  begin  -- exit
    wait until rxframe10ms_ready(15)='1';
    for i in 0 to 15 loop
      check_onech_30ms(stream_rx(i), pattern);
    end loop;
  end check_allch_30ms;

  
  begin
    tb_sport13_ok <= '1';
    wait on testcase;
    tb_sport13_ok <= '0';
    tx_30ms_start <= (others=>'0');
    -----------------------------------------------------------------------------
    -- TESTCASE 
    -----------------------------------------------------------------------------
    if testcase = TC_SYSTEM_INIT then
      wait for SIGNAL_ASSIGNMENT;
--      report "Syst" & "testcase";
      printf("_0m: System initialization done");

    -----------------------------------------------------------------------------
    -- TESTCASE Short Loop simulates a communication via FPGA internal loop at TDM side
    -----------------------------------------------------------------------------
    elsif testcase = TC_SHORT_LOOP then
      printf("_0m: Short Loop Test started");
      subframe_length <= (0,0,0,56,0,0,0,0,0,0,0,0,67,0,0,0);
      stream_tx <= (others=>(others=>'0'));
      wait for 0.3 ms;
      stream_tx(12) <= gen_stream(DEF_WF_6k7x3_123);
      stream_tx(3) <= gen_stream(DEF_WF_5k6x3_123);
      tx_30ms_start<="1111"; wait until clk_32M='1'; tx_30ms_start<="0000";
      wait for 0.1 ms;
      tx_30ms_start(3)<='1'; wait until clk_32M='1'; tx_30ms_start(3)<='0';
      wait for 22 ms;
      printf("check first channel in tdm frame (ch3)");
      check_onech_30ms(stream_rx(3), DEF_WF_5k6x3_123);
      printf("check last channel in tdm frame (ch12)");
      check_onech_30ms(stream_rx(12), DEF_WF_6k7x3_123);      

    -----------------------------------------------------------------------------
    -- TESTCASE simulates one of several possible constellations of MUX/TDM-Data
    -----------------------------------------------------------------------------
    elsif testcase = TC_PAGE_INTERFERENCE_1 then
      printf("_0m: Starting new interference test");
      subframe_length <= (56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56);
      stream_tx <= (others=>(others=>'0'));
      tx_30ms_start(3 downto 0)<="1111"; wait until clk_32M='1'; tx_30ms_start(3 downto 0)<="0000";
      wait for 10 ms;
      stream_tx <= (others=>gen_stream(DEF_WF_5k6x3_123));
      stream_tx_b2b <= (others=>gen_stream_b2b(DEF_WF_5k6x3_123));
      tx_30ms_start(3 downto 0)<="1111"; wait until clk_32M='1'; tx_30ms_start(3 downto 0)<="0000";
      wait for 35 ms;
      check_allch_30ms(stream_rx, DEF_WF_5k6x3_123);

    -----------------------------------------------------------------------------
    -- TESTCASE simulates one of several possible constellations of MUX/TDM-Data
    -----------------------------------------------------------------------------
    elsif testcase = TC_PAGE_INTERFERENCE_5 then
      printf("_0m: Starting new interference test");
      subframe_length <= (67,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
      stream_tx <= (others=>(others=>'0'));
      stream_tx(0) <= gen_stream(DEF_WF_6k7x3_123);
      stream_tx_b2b(0) <= gen_stream_b2b(DEF_WF_6k7x3_123);
      
      tx_30ms_start(0)<='1'; wait until clk_32M='1'; tx_30ms_start(0)<='0';

      wait for 70 ms;
      check_onech_30ms(stream_rx(0), DEF_WF_6k7x3_123);      

    -----------------------------------------------------------------------------
    end if;
    tb_sport13_ok <= '1';

  end process testcase_proc;
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------
end architecture test;

