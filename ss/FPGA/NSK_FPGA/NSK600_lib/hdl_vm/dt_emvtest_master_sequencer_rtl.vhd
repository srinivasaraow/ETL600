--
-- VHDL Architecture NSK600_lib.dt_emvtest_master_sequencer.rtl
--
-- Created:
--          by - chstrue.UNKNOWN (CH-L-0014426)
--          at - 13:24:47 2010-10-13
--
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--


--On trigger, a sequence of 432 bytes (one byte in one transfer) is generated.
--One aggregate frame: 3200 bit -> 400 bytes + reserve bytes
--which have to be used when one channel data rate isn't a multiple of 8 bit.



LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_emvtest_master_sequencer IS
   PORT( 
      btransfer_done : IN     std_logic;
      clk            : IN     std_logic;
      reset_n        : IN     std_logic;
      start_sequence : IN     std_logic;
      byte_nbr       : OUT    std_logic_vector (8 DOWNTO 0);
      prep_data      : OUT    std_logic
   );

-- Declarations

END dt_emvtest_master_sequencer ;

--
ARCHITECTURE rtl OF dt_emvtest_master_sequencer IS

  signal byte_counter     : unsigned(8 downto 0);
  signal sequence_ongoing : std_logic;
  
BEGIN
  
  byte_nbr <= std_logic_vector(byte_counter);
  
  sequencer: process(reset_n, clk)
  begin
    if (reset_n = '0') then
      byte_counter     <= (others => '0');
      sequence_ongoing <= '0';
      prep_data        <= '0';
    elsif (CLK'event and CLK = '1') then
      prep_data <= '0';
      if sequence_ongoing = '0' then
        if start_sequence = '1' then
          prep_data        <= '1';
          sequence_ongoing <= '1';
          byte_counter     <= (others => '0');
        end if;
      else
        if byte_counter = 431 and btransfer_done = '1' then 
          sequence_ongoing <= '0';
        elsif btransfer_done = '1' then
          byte_counter <= byte_counter + 1;
          prep_data    <= '1';
        end if;
      end if;
    end if;
  end process;
  
  
END ARCHITECTURE rtl;

