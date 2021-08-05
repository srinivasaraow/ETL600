--------------------------------------------------------------------------
-- $Workfile: tb_ft1_pattern_p_body.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2004
---------------------------------------------------------------------------
--
-- $Log: /Source/FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_ft1_pattern_p_body.vhd $
-- 
-- 1     7.04.05 16:28 Chalgyg
-- Initial version.
-- 
---------------------------------------------------------------------------
PACKAGE BODY tb_ft1_pattern_p IS

  function parity2nr(parity: character) return integer is
    variable number_of_bits: integer;
  begin
    case parity is
      when  'n' => number_of_bits := 0;
      when  'e' => number_of_bits := 1;
      when  'o' => number_of_bits := 1;
      when others => number_of_bits := 0;
    end case;
    return number_of_bits;
  end parity2nr;

END tb_ft1_pattern_p;
