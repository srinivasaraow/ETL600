--------------------------------------------------------------------------------
-- $Workfile: tb_mpi_model_behave.vhd $
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
-- $Log: /FPGA/NSK_FPGA/DIG_TRANSIT_tb/hdl_vm/tb_mpi_model_behave.vhd $
-- 
-- 3     11-07-06 9:09 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_arith.all;
LIBRARY DIG_TRANSIT_tb;
USE DIG_TRANSIT_tb.tb_dt_main_pkg.all;
USE DIG_TRANSIT_tb.basic_tb_pkg.all;




ENTITY tb_MPI_model IS
   PORT( 
      RESETn   : IN     std_logic;
      MPI_Req  : IN     t_MPI_Req;
      MPI_Ack  : OUT    t_MPI_Ack;
      MPI_CS_n : OUT    std_logic;
      MPI_Addr : OUT    std_logic_vector (11 DOWNTO 0);
      MPI_DATA : INOUT  std_logic_vector (7 DOWNTO 0);
      clock_uC : IN     std_logic;
      MPI_wr_n : OUT    std_logic;
      MPI_rd_n : OUT    std_logic
   );

-- Declarations

END tb_MPI_model ;

--
ARCHITECTURE behave OF tb_MPI_model IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation
  
  
  
 --------------------------------------------------------------------------------
 -- DECLARATIONS
 --------------------------------------------------------------------------------
--  signal LPB_access    : t_LPB_access;
--  signal LPB_Addr_FPGA : std_logic_vector(11 downto 0);
   
 --------------------------------------------------------------------------------
 -- ARCHITECTURE PROCEDURES AND FUNCTIONS
 --------------------------------------------------------------------------------

 
 --
 begin
 
--   LPB_Addr(15 downto 14) <= "00";  --reserve
--   LPB_Addr(13 downto 2)  <= LPB_Addr_FPGA;
--   LPB_Addr(1 downto 0)   <= "00";    

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
         variable trd: time;
       begin
         MPI_cs_n  <= '0';
         MPI_wr_n  <= '1';
         MPI_rd_n  <= '0';
         MPI_addr  <= addr;
         trd  := 24.431 ns + (MPI_waitstates * 30.518 ns);
         wait for trd;
         MPI_rd_n <= '1';
         value    <= MPI_data;
         wait for 32.061 ns;
         MPI_cs_n  <= '1';
         MPI_addr  <= (others=>'0');
         wait for 2.543 ns;
         wait until clock_uC = '1';
       end rd_reg;
   
     -----------------------------------------------------------------------------
     -- wr_reg: write one register byte to a configuration register
     -----------------------------------------------------------------------------
     procedure wr_reg (
       constant addr      : in    std_logic_vector(11 downto 0);
       constant value     : in    std_logic_vector(7 downto 0)
       ) is
         variable twr: time;
       begin
         MPI_cs_n  <= '0';
         MPI_wr_n  <= '0';
         MPI_rd_n  <= '1';
         MPI_addr  <= addr;
         MPI_data  <= value;
         twr  := 24.974 ns + (MPI_waitstates * 30.518 ns);
         wait for twr;
         MPI_wr_n  <= '1';
         wait for 32.061 ns;
         MPI_cs_n  <= '1';
         MPI_addr  <= (others=>'0');
         MPI_data  <= (others=>'Z');
         wait for 12.465 ns;
         wait until clock_uC = '1';
       end wr_reg;
 
     
   begin
     MPI_Ack.Ack       <= '0';
     MPI_Ack.Read_data <= (others => '0');

     MPI_wr_n  <= '1';
     MPI_rd_n  <= '1';
     MPI_CS_n  <= '1';
     MPI_Addr  <= (others => '0');
     
     MPI_DATA  <= (others => 'Z');
     
     while (true) loop
       wait until MPI_Req.Req = '1';
       wait until clock_uC'event and clock_uC = '1';
       if MPI_Req.trans = MPI_FPGA_write then
         wr_reg(MPI_Req.Addr, MPI_Req.Write_data); --Addr, Data
       elsif MPI_Req.trans = MPI_FPGA_read then
         rd_reg(MPI_Req.Addr, MPI_Ack.Read_data);
       else
         --failure...
       end if;
       MPI_Ack.Ack <= '1';
       wait until clock_uC = '1';
       MPI_Ack.Ack <= '0';
     end loop ;      
     
  end process;   

  
  
END behave;

