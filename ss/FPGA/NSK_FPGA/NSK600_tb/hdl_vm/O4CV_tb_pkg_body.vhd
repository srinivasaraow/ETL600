--------------------------------------------------------------------------------
-- VHDL Package Body O4CV_tb.report_assist.arch_name
--------------------------------------------------------------------------------
-- Copyright  : ABB Switzerland Ltd. 2007
--------------------------------------------------------------------------------
-- Project  	: O4CV
-- Library	: O4CV_tb
-- Unit  	: report_assist
-- Language 	: VHDL
--------------------------------------------------------------------------------
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--
--------------------------------------------------------------------------------
-- $Workfile: O4CV_tb_pkg_body.vhd $
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/O4CV_tb_pkg_body.vhd $ 
-- 
-- 1     11-07-06 9:22 Chstrue
-- R4 implementation
-- 
-- 3     08-03-18 12:06 Chmaglo1
-- Last Update before Holiday MGL;
-- TB running error free;
-- 
-- 2     07-05-19 19:54 Chmaglo1
-- -- no description --
-- 
-- 1     07-04-10 19:51 Chmaglo1
-- Initial Check in: Test VSS path settings
-- 0     2007-04-01 22:30:27 Mathias Gloor (CH-L-0012267)
-- File Generated 
--------------------------------------------------------------------------------
--
PACKAGE BODY report_assist IS
--------------------------------------------------------------------------------
-- PROCEDURE BODIES
--------------------------------------------------------------------------------
  -----------------------------------------------------------------------------
  -- Function Example Body
  -----------------------------------------------------------------------------
--  function get_syncpat (
--    frame          : t_dcc_frame; 
--    table          : integer) 
--    return std_logic_vector is
--    variable syncpat : std_logic_vector(0 to 16);
--  begin 
--    case table is
--      when 6 => return(                      -- table 7a 48 kbit/s
--                       frame(1)(1)
--                     & frame(2)(1)
--                     & frame(3)(1)
--                     & frame(4)(1));
--      when 7 => return(                      -- table 7b 56kbit/s
--                       frame(1)(1)
--                     & frame(2)(1)
--                     & frame(3)(1)
--                     & frame(4)(1)
--                     & frame(5)(1)
--                     & frame(6)(1)
--                     & frame(7)(1)
--                     & frame(8)(1));
--      when others =>  return "0";
--    end case;
--  end get_syncpat;
  
  ------------------------------------------------------------------------------
  -- Procedure Example Body
  ------------------------------------------------------------------------------
  -- purpose: print messages to stdout
--  procedure printf(s : in string) is
--    variable l : line;
--  begin
--    write(l, s);
--    writeline(output, l);
--  end printf;

--  -- purpose: write string to line
--  procedure sprintf (l : inout line; s : in string) is
--  begin  -- sprintf
--    write(l, s);
--  end sprintf;
--
--  -- purpose: write line to stdout
--  procedure lprintf (
--    l : inout line) is
--  begin  -- lprintf
--    writeline(output, l);
--  end lprintf;
--
--  -- purpose: write integer to line
--  procedure iprintf (
--    l : inout line;
--    i : in    integer) is
--  begin  -- iprintf
--    write(l, i);
--  end iprintf;

--  procedure pexit (
--    constant status : in integer;
--    constant op     : in string) is
--  begin  -- exit
--    -- stop simulation
--    if status = 0 then
--      assert false
--        report "Simulation Complete @" & op & "(This is not a failure.)"
--        severity failure;
--    else
--      printf(PF_NAME & " simulation error @" & op);
--      assert false
--        report "*** Simulation Error @" & op & " ***"
--        severity failure;
--    end if;
--  end pexit;

--------------------------------------------------------------------------------
END report_assist;
