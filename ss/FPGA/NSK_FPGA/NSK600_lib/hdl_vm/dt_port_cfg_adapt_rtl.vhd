--------------------------------------------------------------------------------
-- $Workfile   : $
-- Project     : ETL600 / Subproject NSK600 / MUX600 FPGA
-- Language    : VHDL
-- Created by  : Stefan Rueegg,PSND2 / Daniel Steigmeier,Zuehlke Engineering AG
-- Remarks     : 
-- Copyright   : ABB Switzerland Ltd. 2004
-- Using       : Mentor Graphics HDL Designer(TM) 2010.2a (Build 7)
--
-- Description : Configuration of the digital voice transit channels is done 
--               based on the agregate channel numbering. For the function of the 
--               transit channels on the link, the configuration must be adapted.
--               Example: If the transit channel 0 is activated, the LT bit of 
--               the agregate channel which is mapped to the transit channel 0
--               must be set to transit. This "remapping" of the configuration 
--               is done in this unit.
--------------------------------------------------------------------------------
-- History:
-- $Log: /FPGA/NSK_FPGA/NSK600_lib/hdl_vm/dt_port_cfg_adapt_rtl.vhd $
-- 
-- 4     11-08-29 9:26 Chstrue
-- Master: Timeout implemented for dt burst generation
-- 
-- 3     11-07-06 9:40 Chstrue
-- R4 implementation


LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_port_cfg_adapt IS
   PORT( 
      clk                   : IN     std_logic;
      control_digtrans      : IN     t_control_digtrans;
      control_mux           : IN     t_control_mux;
      reset_n               : IN     std_logic;
      cfg_Master_Slave_n    : OUT    std_logic;
      dt_ch_active_master   : OUT    std_logic_vector (15 DOWNTO 0);
      dt_ch_active_slave    : OUT    std_logic_vector (15 DOWNTO 0);
      dt_ch_send_AIS_master : OUT    std_logic_vector (15 DOWNTO 0);
      dt_ch_send_AIS_slave  : OUT    std_logic_vector (15 DOWNTO 0);
      deframer_timeout      : IN     std_logic
   );

-- Declarations

END dt_port_cfg_adapt ;

--
ARCHITECTURE rtl OF dt_port_cfg_adapt IS

-----------------------------------------------------------
-- Last edited:
--   110630: R4 implementation
--   110719: Timeout implemented


BEGIN

  cfg_Master_Slave_n <= control_digtrans.gdtc.mss;
  
  clocked : process (clk, reset_n)
  begin
    if reset_n = '0' then                             -- reset
      dt_ch_active_master   <= (others => '0');
      dt_ch_send_AIS_master <= (others => '0');
      dt_ch_active_slave    <= (others => '0');
      dt_ch_send_AIS_slave  <= (others => '0');
  
    elsif clk'event and clk = '1' then                -- running on rising edge
  
      --master control signals:
      for i in 0 to 15 loop  --for each voice channel, set dt transit ch active bit, if
        if (control_digtrans.vce(i) = '1') and      --voice channel enabled
           (control_digtrans.vctl(i) = '1') then    --and sent to digital transit
          dt_ch_active_master(i) <= '1';
        else
          dt_ch_active_master(i) <= '0';
        end if;
      end loop;
  
      for i in 0 to 15 loop  --for each voice channel, set dt send AIS bit, if 
        if (control_digtrans.vce(i) = '1')  and     --voice channel enabled
           (control_digtrans.vctl(i) = '1') and     --and sent to digital transit
           ((control_mux.mux_ch(11 + i) = '0') 
                  or (deframer_timeout = '1')) then  --and channel detached from aggregate frame, or AF link lost
          dt_ch_send_AIS_master(i) <= '1';          --send AIS to slaves
        else
          dt_ch_send_AIS_master(i) <= '0';
        end if;
      end loop;
  
      --slave control signals:
      --default: 
      dt_ch_active_slave <= (others => '0');
      for i in 0 to 15 loop  --for each voice channel, set dt transit ch active bit, if
        if (control_digtrans.vce(i) = '1') and      --voice channel enabled
           (control_digtrans.vctl(i) = '1') then    --and sent to digital transit
          dt_ch_active_slave(control_digtrans.vctp(i+1)) <= '1';
        end if;
      end loop;
  
      --default: 
      dt_ch_send_AIS_slave <= (others => '0');
      for i in 0 to 15 loop  --for each voice channel, set dt send AIS bit, if 
        if (control_digtrans.vce(i) = '1') and      --voice channel enabled
           (control_digtrans.vctl(i) = '1') and     --and sent to digital transit
           (control_mux.mux_ch(11 + i) = '0') then  --and channel detached from aggregate frame
          dt_ch_send_AIS_slave(control_digtrans.vctp(i+1)) <= '1';
        end if;
      end loop;
    end if;
  end process clocked; 


END rtl;

