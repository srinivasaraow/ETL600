--------------------------------------------------------------------------------
-- $Workfile: basic_tb_pkg.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/basic_tb_pkg.vhd $
-- 
-- 4     11-07-05 17:04 Chstrue
-- -- no description --
-- 
-- 3     11-07-05 16:58 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;


LIBRARY ieee;
USE ieee.std_logic_1164.all;
library std;
use std.textio.all;

PACKAGE basic_tb_pkg IS
-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation

  
  procedure printf(
           constant s      : in string
                   );

  function stdv2str(
           vec  : std_logic_vector
                    ) return string;
  
  function slv2int(slv : std_logic_vector) return integer;
  
  function int2chr(int: integer) return character;

  function int2str(int: integer) return string;

-- convert std_logic_vector into a string in hex format. from  www.stefanvhdl.com/vhdl/vhdl/txt_util.vhd
      
  function hstr(slv: std_logic_vector) return string;
  
  




END basic_tb_pkg;
