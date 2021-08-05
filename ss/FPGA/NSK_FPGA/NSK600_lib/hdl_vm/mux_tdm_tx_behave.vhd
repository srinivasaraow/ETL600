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
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/mux_tdm_tx_behave.vhd $
-- 
-- 4     11-07-06 9:41 Chstrue
-- R4 implementation

--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY mux_tdm_tx IS
   PORT( 
      clk               : IN    std_logic;
      reset_n           : IN    std_logic;
      control_digtrans  : IN    t_control_digtrans;
      data_tdm_tx       : IN    std_logic;
      data_dt_tx        : IN    std_logic;
      rd_ptr_tx_in      : IN    integer range 0 to 16383;
      rd_ptr_tx_out_tdm : OUT   integer range 0 to 16383;
      rd_ptr_tx_out_dt  : OUT   integer range 0 to 16383;
      data_aggr_tx      : OUT   std_logic
   );
END mux_tdm_tx;

--
ARCHITECTURE behave OF mux_tdm_tx IS

-----------------------------------------------------------
-- Last edited:
--   110307: dsg@zuehlke.com: File Generated
--   110630: R4 implementation


  signal rd_ptr_tx_out_tdm_i   : integer range 0 to 16383;
  signal rd_ptr_tx_out_dt_i    : integer range 0 to 16383;
  signal data_aggr_tx_i        : std_logic;
BEGIN

clocked : process (clk, reset_n)
begin
  if reset_n = '0' then                             -- reset
    rd_ptr_tx_out_tdm_i <= 0;
    rd_ptr_tx_out_dt_i  <= 0;
    data_aggr_tx_i      <= '0';

  elsif clk'event and clk = '1' then                -- sending on rising edge

    for i in 0 to 15 loop

      if (rd_ptr_tx_in >= (i * 1024)) and (rd_ptr_tx_in < ((i + 1) * 1024)) then

        if (control_digtrans.gdtc.mss = '0') then                                           -- configured as slave
          rd_ptr_tx_out_tdm_i <= rd_ptr_tx_in - i*1024 + 1024*control_digtrans.vctp(i+1);   -- mux TDM and DT
          rd_ptr_tx_out_dt_i  <= rd_ptr_tx_in - i*1024 + 1024*control_digtrans.vctp(i+1);
        else                                                                                -- configured as master
          rd_ptr_tx_out_tdm_i <= rd_ptr_tx_in - i*1024 + 1024*control_digtrans.vctp(i+1);   -- mux TDM
          rd_ptr_tx_out_dt_i  <= rd_ptr_tx_in;                                              -- do not change the source address for DT
        end if;
        
        if control_digtrans.vce(i) = '0' then         -- set data for enframer to '0' if channel is addressed but disabled
          data_aggr_tx_i      <= '0';
        else
          if (control_digtrans.vctl(i) = '0') then    -- send data to enframer from TDM- or DT-path
            data_aggr_tx_i    <= data_tdm_tx;
          else
            data_aggr_tx_i    <= data_dt_tx;
          end if;
        end if;
        
      end if; 
    
    end loop;

  end if;
end process clocked;

  rd_ptr_tx_out_tdm <= rd_ptr_tx_out_tdm_i;
  rd_ptr_tx_out_dt  <= rd_ptr_tx_out_dt_i;
  data_aggr_tx      <= data_aggr_tx_i;
      
END behave;