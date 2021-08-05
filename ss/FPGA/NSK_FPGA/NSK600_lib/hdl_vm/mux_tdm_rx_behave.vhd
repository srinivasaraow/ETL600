--------------------------------------------------------------------------------
-- $Workfile   : $
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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/mux_tdm_rx_behave.vhd $
-- 
-- 5     11-07-06 9:40 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY mux_tdm_rx IS
   PORT( 
      clk               : IN    std_logic;
      reset_n           : IN    std_logic;
      control_digtrans  : IN    t_control_digtrans;
      rxd_in            : IN    std_logic;
      rxd_out_tdm       : OUT   std_logic;
      rxd_out_dt        : OUT   std_logic;
      enp_rxd_in        : IN    std_logic;
      enp_rxd_out_tdm   : OUT   std_logic;
      enp_rxd_out_dt    : OUT   std_logic;
      wr_ptr_rx_in      : IN    integer range 0 to 16383;
      wr_ptr_rx_out_tdm : OUT   integer range 0 to 16383;
      wr_ptr_rx_out_dt  : OUT   integer range 0 to 16383
   );
END mux_tdm_rx;

--
ARCHITECTURE behave OF mux_tdm_rx IS
-----------------------------------------------------------
-- Last edited:
--   110304: dsg@zuehlke.com: File Generated
--   110630: R4 implementation



  signal rxd_out_tdm_i        : std_logic;
  signal rxd_out_dt_i         : std_logic;
  signal enp_rxd_out_tdm_i    : std_logic;
  signal enp_rxd_out_dt_i     : std_logic;
  signal wr_ptr_rx_out_tdm_i  : integer range 0 to 16383;
  signal wr_ptr_rx_out_dt_i   : integer range 0 to 16383;
BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                             -- reset
    rxd_out_tdm_i       <= '0';
    rxd_out_dt_i        <= '0';
    enp_rxd_out_tdm_i   <= '0';
    enp_rxd_out_dt_i    <= '0';
    wr_ptr_rx_out_tdm_i <= 0;
    wr_ptr_rx_out_dt_i  <= 0;
    
  elsif clk'event and clk = '1' then                -- sending on rising edge

    for i in 0 to 15 loop

      if (wr_ptr_rx_in >= (i * 1024)) and (wr_ptr_rx_in < ((i + 1) * 1024)) then
        if control_digtrans.vce(i) = '0' then         -- set data to '0' if channel is addressed but disabled
          rxd_out_tdm_i     <= '0';
          rxd_out_dt_i      <= '0';
          enp_rxd_out_tdm_i <= enp_rxd_in;
          enp_rxd_out_dt_i  <= enp_rxd_in;
        else
          if (control_digtrans.vctl(i) = '0') then    -- send data to TDM- or DT-path
            rxd_out_tdm_i     <= rxd_in;
            rxd_out_dt_i      <= '0';
            enp_rxd_out_tdm_i <= enp_rxd_in;
            enp_rxd_out_dt_i  <= '0'; 
          else
            rxd_out_tdm_i     <= '0';
            rxd_out_dt_i      <= rxd_in;
            enp_rxd_out_tdm_i <= '0';
            enp_rxd_out_dt_i  <= enp_rxd_in;
          end if;
        end if;
        
        if (control_digtrans.gdtc.mss = '0') then                                           -- configured as slave
          wr_ptr_rx_out_tdm_i <= wr_ptr_rx_in - i*1024 + 1024*control_digtrans.vctp(i+1);   -- mux TDM and DT
          wr_ptr_rx_out_dt_i  <= wr_ptr_rx_in - i*1024 + 1024*control_digtrans.vctp(i+1);
        else                                                                                -- configured as master
          wr_ptr_rx_out_tdm_i <= wr_ptr_rx_in - i*1024 + 1024*control_digtrans.vctp(i+1);   -- mux TDM
          wr_ptr_rx_out_dt_i  <= wr_ptr_rx_in;                                              -- do not change the destination for DT
        end if;
        
      end if; 
    
    end loop;
    
  end if;
end process clocked;

  rxd_out_tdm       <= rxd_out_tdm_i;
  rxd_out_dt        <= rxd_out_dt_i;
  enp_rxd_out_tdm   <= enp_rxd_out_tdm_i;
  enp_rxd_out_dt    <= enp_rxd_out_dt_i;
  wr_ptr_rx_out_tdm <= wr_ptr_rx_out_tdm_i;
  wr_ptr_rx_out_dt  <= wr_ptr_rx_out_dt_i;
    
END behave;
