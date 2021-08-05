--------------------------------------------------------------------------------
-- $Workfile: tb_p1ly_model_behave.vhd $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : 
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_p1ly_model_behave.vhd $
-- 
-- 3     11-07-06 9:08 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.report_assist.all;

ENTITY tb_P1LY_model IS
   PORT( 
      DTEN1         : IN     std_logic;
      DTEN2         : IN     std_logic;
      DTTX1         : IN     std_logic;
      DTTX2         : IN     std_logic;
      to_master_in  : IN     std_logic;
      to_slave_in   : IN     std_logic;
      DTRX1         : OUT    std_logic;
      DTRX2         : OUT    std_logic;
      to_master_out : OUT    std_logic;
      to_slave_out  : OUT    std_logic
   );

-- Declarations

END tb_P1LY_model ;

--
ARCHITECTURE behave OF tb_P1LY_model IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation


BEGIN
  
  
  DTRX1 <= to_master_in when DTEN1 = '0' else
           DTTX2;
  
  to_master_out <= DTTX1 when DTEN2 = '1' else
                   to_slave_in;
                   
  
  
  DTRX2 <= to_slave_in when DTEN2 = '0' else
           DTTX1;
  
  to_slave_out <= DTTX2 when DTEN1 = '1' else
                  to_master_in;
  
  
END behave;

