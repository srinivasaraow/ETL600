--------------------------------------------------------------------------------
-- VHDL Architecture NSK600_lib.collect_control.rtl
-- $Workfile: collect_control_rtl.vhd $
-- Project  : ETL600 / Subproject NSK600
-- Language : VHDL
-- Created by : unp00631.UNKNOWN (CH-W-2350)
-- Remarks  :
-- Copyright  : ABB Switzerland Ltd. 2004
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/collect_control_rtl.vhd $
-- 
-- 18    11-07-06 9:41 Chstrue
-- R4 implementation
-- 
-- 16    11-06-20 16:07 Chstrue
-- mux bit generation corrected for port sharing
-- 
-- 14    11-06-14 14:56 Chstrue
-- Digital voice transit implementation
-- 
-- 12    07-10-11 15:23 Chmaglo1
-- -- no description --
-- 
-- 7     6.10.06 16:55 Chmaglo1
-- -- no description --
-- 
-- 6     3.10.06 13:22 Chmaglo1
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
-- 57    14.06.05 14:19 Chmaglo1
-- Aenderung für control_nsk.dcd
-- 
-- 56    30.04.05 14:24 Chmaglo1
-- -- no description --
-- 
-- 27    19.01.05 16:44 Chmaglo1
-- -- no description --
-- 
-- 9     1.12.04 10:41 Unp00631
-- Migration Test 2
-- 
-- 1     15.10.04 17:34 Unp00631
-- before military service
-- 0     01.10.2004 14:41:39 Mathias Gloor (MGL)
-- File Generated
--
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY collect_control IS
   PORT( 
      brt_vec        : IN     t_brt_vec;
      ccr            : IN     t_register;
      clk            : IN     std_logic;
      pcr            : IN     t_register;
      reset_n        : IN     std_logic;
      rsig_nsk       : IN     std_logic_vector (3 DOWNTO 0);
      tcr            : IN     t_tdmregs;
      tpr            : IN     t_register;
      control_enet   : OUT    t_control_v24_x;
      control_g703   : OUT    t_control_v24_x;
      control_mux    : OUT    t_control_mux;
      control_nsk_1  : OUT    t_control_nsk;
      control_nsk_2  : OUT    t_control_nsk;
      control_nsk_3  : OUT    t_control_nsk;
      control_nsk_4  : OUT    t_control_nsk;
      control_ofdm   : OUT    t_control_v24_x;
      control_switch : OUT    t_control_switch;
      control_tdm    : OUT    t_control_tdm;
      control_v11    : OUT    t_control_v11;
      control_v24    : OUT    t_control_v24
   );

-- Declarations

END collect_control ;

--
--------------------------------------------------------------------------------
architecture rtl of collect_control is
  
-----------------------------------------------------------
-- Last edited:
--    110428: Lan only mode config bit included
--    110615: Generation of control_mux.mux_ch corrected concerning v.24 port sharing
--    110630: R4 implementation
  
  
  
--------------------------------------------------------------------------------
-- DECLARATIONS
--------------------------------------------------------------------------------

--  component record_config is
--    port (pcr_x, ccr_x, brr_x : in  std_logic_vector(6 downto 0);
--          control_x       : out t_control_v24_x );
--  end component;
  
--  procedure record_control_v24 (
--  variable pcr : in std_logic_vector;
--  variable ccr : in std_logic_vector;
--  variable brt_vec : in std_logic_vector;
--  signal control_v24 : out ) is 
--    return t_control_v24_x is
--       variable control_x : t_control_v24_x;
--  begin 
--    for i in 1 to 6 loop
--    -- reg PCR                              
--    control_v24(i).poe <= pcr(i)(0);
--    control_v24(i).poe <= pcr(i)(0);
--    control_v24(i).tpt <= pcr(i)(1);
--    control_v24(i).ltr <= pcr(i)(2);
--    control_v24(i).lrc <= pcr(i)(3);
--    control_v24(i).lrd <= pcr(i)(4);
--    control_v24(i).jac <= pcr(i)(5);
--    control_v24(i).das <= pcr(i)(6);
--    -- reg CCR
--    control_v24(i).mux <= ccr(i)(0);
--    control_v24(i).clr <= ccr(i)(1);
--    control_v24(i).fsk <= ccr(i)(2);
--    control_v24(i).asy <= ccr(i)(3);
--    control_v24(i).rxc <= ccr(i)(4);
--    control_v24(i).icp <= ccr(i)(5);
--    control_v24(i).ict <= ccr(i)(6);
--    -- reg BRR
--    control_v24(i).basecount  <= conv_integer(brt_vec(i)(31 downto 20));
--    control_v24(i).numerator  <= conv_integer(brt_vec(i)(19 downto 16));
--    control_v24(i).denominator<= conv_integer(brt_vec(i)(15 downto 12));
--    end loop;
--    return control_x;
--  end record_control_v24;  
--for U1: gen_v24 use entity work.gen_v24(rtl);
--
begin
--------------------------------------------------------------------------------
-- PROCESSES
--------------------------------------------------------------------------------
--  for i in 1 to 6 generate
--    U: gen_v24 port map (control_v24(1), pcr(1), ccr(1), brr(1));
--  end generate;
--
--------------------------------------------------------------------------------
-- CONCURRENT LOGIC
--------------------------------------------------------------------------------
control_nsk_1.dcd <= pcr(1)(0);
control_nsk_2.dcd <= pcr(2)(0);
control_nsk_3.dcd <= pcr(3)(0);
control_nsk_4.dcd <= pcr(4)(0);
control_nsk_1.cts <= rsig_nsk(0);
control_nsk_2.cts <= rsig_nsk(1);
control_nsk_3.cts <= rsig_nsk(2);
control_nsk_4.cts <= rsig_nsk(3);
control_nsk_1.fsk <= ccr(1)(1);
control_nsk_2.fsk <= ccr(2)(1);
control_nsk_3.fsk <= ccr(3)(1);
control_nsk_4.fsk <= ccr(4)(1);


control_switch.shd1 <= pcr(1)(7);
control_switch.shd2 <= pcr(2)(7);
control_switch.shd3 <= pcr(3)(7);
control_switch.shd4 <= pcr(4)(7);
--control_switch.shd1 <= pcr(1)(7);
--control_switch.shd2 <= pcr(2)(7);
--control_switch.shd5 <= pcr(5)(7);
--control_switch.shd6 <= pcr(6)(7);
control_switch.mux <= (tcr(0)(7) or tcr(1)(7) or tcr(2)(7) or tcr(3)(7) or tcr(4)(7) or tcr(5)(7) or tcr(6)(7) or tcr(7)(7) or 
                      tcr(8)(7) or tcr(9)(7) or tcr(10)(7) or tcr(11)(7) or tcr(12)(7) or tcr(13)(7) or tcr(14)(7) or tcr(15)(7)) &
                      ccr(10)(0) & ccr(9)(0)& ccr(8)(0) & ccr(7)(0)& ccr(6)(0)& ccr(5)(0)& ccr(4)(0)& ccr(3)(0)& ccr(2)(0)& ccr(1)(0);

control_mux.mux_ch <= tcr(15)(7) &
                      tcr(14)(7) &
                      tcr(13)(7) &
                      tcr(12)(7) &
                      tcr(11)(7) &
                      tcr(10)(7) &
                      tcr(9)(7) &
                      tcr(8)(7) &
                      tcr(7)(7) &
                      tcr(6)(7) &
                      tcr(5)(7) &
                      tcr(4)(7) &
                      tcr(3)(7) &
                      tcr(2)(7) &
                      tcr(1)(7) &
                      tcr(0)(7) &
                      ccr(10)(0) & 
                      ccr(9)(0)  & 
                      ccr(8)(0) & 
                      ccr(7)(0) & 
--changed for port sharing of ch1,2,3,4 instead of ch 1,2,5,6:
--                     (ccr(6)(0) and not pcr(6)(7)) & 
--                     (ccr(5)(0) and not pcr(5)(7)) & 
--                      ccr(4)(0) & 
--                      ccr(3)(0) & 
                      ccr(6)(0) & 
                      ccr(5)(0) & 
                     (ccr(4)(0) and not pcr(4)(7)) & 
                     (ccr(3)(0) and not pcr(3)(7)) & 
                     (ccr(2)(0) and not pcr(2)(7)) & 
                      ccr(1)(0);                            
control_mux.poe     <=  tcr(15)(7) or tcr(14)(7) or tcr(13)(7) or tcr(12)(7) or tcr(11)(7) or tcr(10)(7) or tcr(9)(7) or tcr(8)(7) or 
                        tcr(7)(7) or tcr(6)(7) or tcr(5)(7) or tcr(4)(7) or tcr(3)(7) or tcr(2)(7) or tcr(1)(7) or tcr(0)(7) or 
                        ccr(10)(0) or ccr(9)(0) or ccr(8)(0) or ccr(7)(0) or ccr(6)(0)
                                or ccr(5)(0) or ccr(4)(0) or ccr(3)(0) or ccr(2)(0) or ccr(1)(0);
--control_mux.poe     <= '0' when (ccr(10)(0) & ccr(9)(0) & ccr(8)(0) & ccr(7)(0) & ccr(6)(0)
--                                & ccr(5)(0) & ccr(4)(0) & ccr(3)(0) & ccr(2)(0) & ccr(1)(0)
--                                = 0)  else
--                       '1';
--control_mux.poe     <= pcr(0)(0);
control_mux.tpt            <= pcr(0)(1);
control_mux.bitcounts      <= ( conv_integer(brt_vec(0)(11 downto 0)),  -- aggregate channel
                                conv_integer(brt_vec(1)(11 downto 0)),
                                conv_integer(brt_vec(2)(11 downto 0)),
                                conv_integer(brt_vec(3)(11 downto 0)),
                                conv_integer(brt_vec(4)(11 downto 0)),
                                conv_integer(brt_vec(5)(11 downto 0)),
                                conv_integer(brt_vec(6)(11 downto 0)),
                                conv_integer(brt_vec(7)(11 downto 0)),
                                conv_integer(brt_vec(8)(11 downto 0)),
                                conv_integer(brt_vec(9)(11 downto 0)),
                                conv_integer(brt_vec(10)(11 downto 0)),
                                conv_integer(tcr(0)(6 downto 0)),        -- 16 x TDM O4CV channels
                                conv_integer(tcr(1)(6 downto 0)),        
                                conv_integer(tcr(2)(6 downto 0)),
                                conv_integer(tcr(3)(6 downto 0)),
                                conv_integer(tcr(4)(6 downto 0)),
                                conv_integer(tcr(5)(6 downto 0)),
                                conv_integer(tcr(6)(6 downto 0)),
                                conv_integer(tcr(7)(6 downto 0)),
                                conv_integer(tcr(8)(6 downto 0)),
                                conv_integer(tcr(9)(6 downto 0)),
                                conv_integer(tcr(10)(6 downto 0)),
                                conv_integer(tcr(11)(6 downto 0)),
                                conv_integer(tcr(12)(6 downto 0)),
                                conv_integer(tcr(13)(6 downto 0)),
                                conv_integer(tcr(14)(6 downto 0)),
                                conv_integer(tcr(15)(6 downto 0))
                              );
control_mux.poscounts      <= ( 0,0,0,0,0,0,0,0,0,3,0); --t_poscount has range 0...15!! but here max 2bits will be needed
--record_control_v24(pcr, ccr, brt_vec, control_v24);

-- Port(0) ---------------------------------------------------------------------
  control_ofdm.poe <= pcr(0)(0);
  control_ofdm.tpt <= pcr(0)(1);
  control_ofdm.ltr <= pcr(0)(2);
  control_ofdm.lrc <= pcr(0)(3);
  control_ofdm.aon <= pcr(0)(4);
  control_ofdm.jac <= pcr(0)(5);
  control_ofdm.das <= pcr(0)(6);
  -- reg CCR
  control_ofdm.mux <= ccr(0)(0);
  control_ofdm.fsk <= ccr(0)(1);
  control_ofdm.clr <= ccr(0)(2);
  control_ofdm.asy <= ccr(0)(3);
  control_ofdm.rxc <= ccr(0)(4);
  control_ofdm.icp <= ccr(0)(5);
  control_ofdm.ict <= ccr(0)(6);
  control_ofdm.iac <= ccr(0)(7);
  -- reg TPR
  control_ofdm.chl <= conv_integer(tpr(0)(2 downto 0));
  control_ofdm.txc <= '0';
  -- reg BRR
  control_ofdm.basecount  <= conv_integer(brt_vec(0)(33 downto 20));
  control_ofdm.numerator  <= conv_integer(brt_vec(0)(19 downto 16));
  control_ofdm.denominator<= conv_integer(brt_vec(0)(15 downto 12));

-- Port(1) ---------------------------------------------------------------------
  control_v24(1).poe <= pcr(1)(0);
  control_v24(1).tpt <= pcr(1)(1);
  control_v24(1).ltr <= pcr(1)(2);
  control_v24(1).lrc <= pcr(1)(3);
  control_v24(1).aon <= pcr(1)(4);
  control_v24(1).jac <= pcr(1)(5);
  control_v24(1).das <= pcr(1)(6);
  -- reg CCR
  control_v24(1).mux <= ccr(1)(0);
  control_v24(1).fsk <= ccr(1)(1);
  control_v24(1).clr <= ccr(1)(2);
  control_v24(1).asy <= ccr(1)(3);
  control_v24(1).rxc <= ccr(1)(4);
  control_v24(1).icp <= ccr(1)(5);
  control_v24(1).ict <= ccr(1)(6);
  control_v24(1).iac <= ccr(1)(7);
  -- reg TPR
  control_v24(1).chl <= conv_integer(tpr(1)(2 downto 0));
  control_v24(1).txc <= '0';
  -- reg BRR
  control_v24(1).basecount  <= conv_integer(brt_vec(1)(33 downto 20));
  control_v24(1).numerator  <= conv_integer(brt_vec(1)(19 downto 16));
  control_v24(1).denominator<= conv_integer(brt_vec(1)(15 downto 12));
  
-- Port(2) ---------------------------------------------------------------------
  control_v24(2).poe <= pcr(2)(0);
  control_v24(2).tpt <= pcr(2)(1);
  control_v24(2).ltr <= pcr(2)(2);
  control_v24(2).lrc <= pcr(2)(3);
  control_v24(2).aon <= pcr(2)(4);
  control_v24(2).jac <= pcr(2)(5);
  control_v24(2).das <= pcr(2)(6);
  -- reg CCR
  control_v24(2).mux <= ccr(2)(0);
  control_v24(2).fsk <= ccr(2)(1);
  control_v24(2).clr <= ccr(2)(2);
  control_v24(2).asy <= ccr(2)(3);
  control_v24(2).rxc <= ccr(2)(4);
  control_v24(2).icp <= ccr(2)(5);
  control_v24(2).ict <= ccr(2)(6);
  control_v24(2).iac <= ccr(2)(7);
  -- reg TPR
  control_v24(2).chl <= conv_integer(tpr(2)(2 downto 0));
  control_v24(2).txc <= '0';
  -- reg BRR
  control_v24(2).basecount  <= conv_integer(brt_vec(2)(33 downto 20));
  control_v24(2).numerator  <= conv_integer(brt_vec(2)(19 downto 16));
  control_v24(2).denominator<= conv_integer(brt_vec(2)(15 downto 12));

-- Port(3) ---------------------------------------------------------------------
  control_v24(3).poe <= pcr(3)(0);
  control_v24(3).tpt <= pcr(3)(1);
  control_v24(3).ltr <= pcr(3)(2);
  control_v24(3).lrc <= pcr(3)(3);
  control_v24(3).aon <= pcr(3)(4);
  control_v24(3).jac <= pcr(3)(5);
  control_v24(3).das <= pcr(3)(6);
  -- reg CCR
  control_v24(3).mux <= ccr(3)(0);
  control_v24(3).fsk <= ccr(3)(1);
  control_v24(3).clr <= ccr(3)(2);
  control_v24(3).asy <= ccr(3)(3);
  control_v24(3).rxc <= ccr(3)(4);
  control_v24(3).icp <= ccr(3)(5);
  control_v24(3).ict <= ccr(3)(6);
  control_v24(3).iac <= ccr(3)(7);
  -- reg TPR
  control_v24(3).chl <= conv_integer(tpr(3)(2 downto 0));
  control_v24(3).txc <= '0';
  -- reg BRR
  control_v24(3).basecount  <= conv_integer(brt_vec(3)(33 downto 20));
  control_v24(3).numerator  <= conv_integer(brt_vec(3)(19 downto 16));
  control_v24(3).denominator<= conv_integer(brt_vec(3)(15 downto 12));

-- Port(4) ---------------------------------------------------------------------
  control_v24(4).poe <= pcr(4)(0);
  control_v24(4).tpt <= pcr(4)(1);
  control_v24(4).ltr <= pcr(4)(2);
  control_v24(4).lrc <= pcr(4)(3);
  control_v24(4).aon <= pcr(4)(4);
  control_v24(4).jac <= pcr(4)(5);
  control_v24(4).das <= pcr(4)(6);
  -- reg CCR
  control_v24(4).mux <= ccr(4)(0);
  control_v24(4).fsk <= ccr(4)(1);
  control_v24(4).clr <= ccr(4)(2);
  control_v24(4).asy <= ccr(4)(3);
  control_v24(4).rxc <= ccr(4)(4);
  control_v24(4).icp <= ccr(4)(5);
  control_v24(4).ict <= ccr(4)(6);
  control_v24(4).iac <= ccr(4)(7);
  -- reg TPR
  control_v24(4).chl <= conv_integer(tpr(4)(2 downto 0));
  control_v24(4).txc <= '0';
  -- reg BRR
  control_v24(4).basecount  <= conv_integer(brt_vec(4)(33 downto 20));
  control_v24(4).numerator  <= conv_integer(brt_vec(4)(19 downto 16));
  control_v24(4).denominator<= conv_integer(brt_vec(4)(15 downto 12));

-- Port(5) ---------------------------------------------------------------------
  control_v24(5).poe <= pcr(5)(0);
  control_v24(5).tpt <= pcr(5)(1);
  control_v24(5).ltr <= pcr(5)(2);
  control_v24(5).lrc <= pcr(5)(3);
  control_v24(5).aon <= pcr(5)(4);
  control_v24(5).jac <= pcr(5)(5);
  control_v24(5).das <= pcr(5)(6);
  -- reg CCR
  control_v24(5).mux <= ccr(5)(0);
  control_v24(5).fsk <= ccr(5)(1);
  control_v24(5).clr <= ccr(5)(2);
  control_v24(5).asy <= ccr(5)(3);
  control_v24(5).rxc <= ccr(5)(4);
  control_v24(5).icp <= ccr(5)(5);
  control_v24(5).ict <= ccr(5)(6);
  control_v24(5).iac <= ccr(5)(7);
  -- reg TPR
  control_v24(5).chl <= conv_integer(tpr(5)(2 downto 0));
  control_v24(5).txc <= '0';
  -- reg BRR
  control_v24(5).basecount  <= conv_integer(brt_vec(5)(33 downto 20));
  control_v24(5).numerator  <= conv_integer(brt_vec(5)(19 downto 16));
  control_v24(5).denominator<= conv_integer(brt_vec(5)(15 downto 12));

-- Port(6) ---------------------------------------------------------------------
  control_v24(6).poe <= pcr(6)(0);
  control_v24(6).tpt <= pcr(6)(1);
  control_v24(6).ltr <= pcr(6)(2);
  control_v24(6).lrc <= pcr(6)(3);
  control_v24(6).aon <= pcr(6)(4);
  control_v24(6).jac <= pcr(6)(5);
  control_v24(6).das <= pcr(6)(6);
  -- reg CCR
  control_v24(6).mux <= ccr(6)(0);
  control_v24(6).fsk <= ccr(6)(1);
  control_v24(6).clr <= ccr(6)(2);
  control_v24(6).asy <= ccr(6)(3);
  control_v24(6).rxc <= ccr(6)(4);
  control_v24(6).icp <= ccr(6)(5);
  control_v24(6).ict <= ccr(6)(6);
  control_v24(6).iac <= ccr(6)(7);
  -- reg TPR
  control_v24(6).chl <= conv_integer(tpr(6)(2 downto 0));
  control_v24(6).txc <= '0';
  -- reg BRR
  control_v24(6).basecount  <= conv_integer(brt_vec(6)(33 downto 20));
  control_v24(6).numerator  <= conv_integer(brt_vec(6)(19 downto 16));
  control_v24(6).denominator<= conv_integer(brt_vec(6)(15 downto 12));

-- Port(7) ---------------------------------------------------------------------
  control_v11(1).poe <= pcr(7)(0);
  control_v11(1).tpt <= pcr(7)(1);
  control_v11(1).ltr <= pcr(7)(2);
  control_v11(1).lrc <= pcr(7)(3);
  control_v11(1).aon <= pcr(7)(4);
  control_v11(1).jac <= pcr(7)(5);
  control_v11(1).das <= pcr(7)(6);
  -- reg CCR
  control_v11(1).mux <= ccr(7)(0);
  control_v11(1).fsk <= ccr(7)(1);
  control_v11(1).clr <= ccr(7)(2);
  control_v11(1).asy <= ccr(7)(3);
  control_v11(1).rxc <= ccr(7)(4);
  control_v11(1).icp <= ccr(7)(5);
  control_v11(1).ict <= ccr(7)(6);
  control_v11(1).iac <= ccr(7)(7);
  -- reg TPR
  control_v11(1).chl <= conv_integer(tpr(7)(2 downto 0));
  control_v11(1).txc <= tpr(7)(5);
  -- reg BRR
  control_v11(1).basecount  <= conv_integer(brt_vec(7)(33 downto 20));
  control_v11(1).numerator  <= conv_integer(brt_vec(7)(19 downto 16));
  control_v11(1).denominator<= conv_integer(brt_vec(7)(15 downto 12));

-- Port(8) ---------------------------------------------------------------------
  control_v11(2).poe <= pcr(8)(0);
  control_v11(2).tpt <= pcr(8)(1);
  control_v11(2).ltr <= pcr(8)(2);
  control_v11(2).lrc <= pcr(8)(3);
  control_v11(2).aon <= pcr(8)(4);
  control_v11(2).jac <= pcr(8)(5);
  control_v11(2).das <= pcr(8)(6);
  -- reg CCR
  control_v11(2).mux <= ccr(8)(0);
  control_v11(2).fsk <= ccr(8)(1);
  control_v11(2).clr <= ccr(8)(2);
  control_v11(2).asy <= ccr(8)(3);
  control_v11(2).rxc <= ccr(8)(4);
  control_v11(2).icp <= ccr(8)(5);
  control_v11(2).ict <= ccr(8)(6);
  control_v11(2).iac <= ccr(8)(7);
  -- reg TPR
  control_v11(2).chl <= conv_integer(tpr(8)(2 downto 0));
  control_v11(2).txc <= tpr(8)(5);
  -- reg BRR
  control_v11(2).basecount  <= conv_integer(brt_vec(8)(33 downto 20));
  control_v11(2).numerator  <= conv_integer(brt_vec(8)(19 downto 16));
  control_v11(2).denominator<= conv_integer(brt_vec(8)(15 downto 12));

-- Port(9) G.703----------------------------------------------------------------
  control_g703.poe <= pcr(9)(0);
  control_g703.tpt <= pcr(9)(1);
  control_g703.ltr <= pcr(9)(2);
  control_g703.lrc <= pcr(9)(3);
  control_g703.aon <= pcr(9)(4);
  control_g703.jac <= pcr(9)(5);
  control_g703.das <= pcr(9)(6);
  -- reg CCR
  control_g703.mux <= ccr(9)(0);
  control_g703.fsk <= ccr(9)(1);
  control_g703.clr <= ccr(9)(2);
  control_g703.asy <= ccr(9)(3);
  control_g703.rxc <= ccr(9)(4);
  control_g703.icp <= ccr(9)(5);
  control_g703.ict <= ccr(9)(6);
  control_g703.iac <= ccr(9)(7);
  -- reg TPR
  control_g703.chl <= conv_integer(tpr(9)(2 downto 0));
  control_g703.txc <= '1'; --tpr(9)(5);
  -- reg BRR
  control_g703.basecount  <= conv_integer(brt_vec(9)(33 downto 20));
  control_g703.numerator  <= conv_integer(brt_vec(9)(19 downto 16));
  control_g703.denominator<= conv_integer(brt_vec(9)(15 downto 12));

-- Port(10) Ethernet-------------------------------------------------------------
  control_enet.poe <= pcr(10)(0);
  control_enet.tpt <= pcr(10)(1);
  control_enet.ltr <= pcr(10)(2);
  control_enet.lrc <= pcr(10)(3);
  control_enet.aon <= pcr(10)(4);
  control_enet.jac <= pcr(10)(5);
  control_enet.das <= pcr(10)(6);
  --LAN Only mode:
  control_enet.lom <= pcr(10)(7);
  -- reg CCR
  control_enet.mux <= ccr(10)(0);
  control_enet.fsk <= ccr(10)(1);
  control_enet.clr <= ccr(10)(2);
  control_enet.asy <= ccr(10)(3);
  control_enet.rxc <= ccr(10)(4);
  control_enet.icp <= ccr(10)(5);
  control_enet.ict <= ccr(10)(6);
  control_enet.iac <= ccr(10)(7);
  -- reg TPR
  control_enet.chl <= conv_integer(tpr(10)(2 downto 0));
  control_enet.txc <= '0'; --tpr(10)(5);
  -- reg BRR
  control_enet.basecount  <= conv_integer(brt_vec(10)(33 downto 20));
  control_enet.numerator  <= conv_integer(brt_vec(10)(19 downto 16));
  control_enet.denominator<= conv_integer(brt_vec(10)(15 downto 12));

-- Port(11 bis) TDM-------------------------------------------------------------
control_tdm(0).mux <= tcr(0)(7);
control_tdm(1).mux <= tcr(1)(7);
control_tdm(2).mux <= tcr(2)(7);
control_tdm(3).mux <= tcr(3)(7);
control_tdm(4).mux <= tcr(4)(7);
control_tdm(5).mux <= tcr(5)(7);
control_tdm(6).mux <= tcr(6)(7);
control_tdm(7).mux <= tcr(7)(7);
control_tdm(8).mux <= tcr(8)(7);
control_tdm(9).mux <= tcr(9)(7);
control_tdm(10).mux <= tcr(10)(7);
control_tdm(11).mux <= tcr(11)(7);
control_tdm(12).mux <= tcr(12)(7);
control_tdm(13).mux <= tcr(13)(7);
control_tdm(14).mux <= tcr(14)(7);
control_tdm(15).mux <= tcr(15)(7);

control_tdm(0).lpt <= not tcr(0)(7);
control_tdm(1).lpt <= not tcr(1)(7);
control_tdm(2).lpt <= not tcr(2)(7);
control_tdm(3).lpt <= not tcr(3)(7);
control_tdm(4).lpt <= not tcr(4)(7);
control_tdm(5).lpt <= not tcr(5)(7);
control_tdm(6).lpt <= not tcr(6)(7);
control_tdm(7).lpt <= not tcr(7)(7);
control_tdm(8).lpt <= not tcr(8)(7);
control_tdm(9).lpt <= not tcr(9)(7);
control_tdm(10).lpt <= not tcr(10)(7);
control_tdm(11).lpt <= not tcr(11)(7);
control_tdm(12).lpt <= not tcr(12)(7);
control_tdm(13).lpt <= not tcr(13)(7);
control_tdm(14).lpt <= not tcr(14)(7);
control_tdm(15).lpt <= not tcr(15)(7);

--control_tdm(0).lpt <= not tcr(0)(7);
--control_tdm(1).lpt <= tcr(1)(6);
--control_tdm(2).lpt <= tcr(2)(6);
--control_tdm(3).lpt <= tcr(3)(6);
--control_tdm(4).lpt <= tcr(4)(6);
--control_tdm(5).lpt <= tcr(5)(6);
--control_tdm(6).lpt <= tcr(6)(6);
--control_tdm(7).lpt <= tcr(7)(6);
--control_tdm(8).lpt <= tcr(8)(6);
--control_tdm(9).lpt <= tcr(9)(6);
--control_tdm(10).lpt <= tcr(10)(6);
--control_tdm(11).lpt <= tcr(11)(6);
--control_tdm(12).lpt <= tcr(12)(6);
--control_tdm(13).lpt <= tcr(13)(6);
--control_tdm(14).lpt <= tcr(14)(6);
--control_tdm(15).lpt <= tcr(15)(6);

--
--------------------------------------------------------------------------------
end architecture rtl;
