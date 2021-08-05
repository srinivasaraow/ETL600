--
-- VHDL Architecture NSK600_lib.dt_emvtest_debugif.rtl
--
-- Created:
--          by - chstrue.UNKNOWN (CH-L-0014426)
--          at - 09:49:31 2010-10-20
--
-- using Mentor Graphics HDL Designer(TM) 2005.3 (Build 74)
--

--Serializer used for observation, with the Hyper Terminal:

--115kbit/s
--8bit
--one stop bit
--no parity
--no HW handshaking



LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
LIBRARY NSK600_lib;
USE NSK600_lib.typedef_p.all;

ENTITY dt_emvtest_debugif IS
   PORT( 
      clk            : IN     std_logic;
      debug_input    : IN     std_logic;
      reset_n        : IN     std_logic;
      start_transfer_debug : IN     std_logic;
      tx_value_debug : IN     std_logic_vector (7 DOWNTO 0);
      btransfer_debug_done : OUT    std_logic;
      debug_output   : OUT    std_logic;
      rx_value_debug : OUT    std_logic_vector (7 DOWNTO 0)
   );

-- Declarations

END dt_emvtest_debugif ;

--
ARCHITECTURE rtl OF dt_emvtest_debugif IS

  signal clk_counter      : unsigned(8 downto 0);
  signal bit_counter      : unsigned(4 downto 0);
  signal transfer_ongoing : std_logic;
  signal tx_bit           : std_logic;
  signal tx_shift_reg     : std_logic_vector(7 downto 0);
  signal rx_bit           : std_logic;
  signal rx_shift_reg     : std_logic_vector(7 downto 0);

BEGIN
  
    debug_output <= tx_bit;
    rx_bit       <= debug_input;
  
    tx_rx_transfer: process(reset_n, clk)
    begin
      if (reset_n = '0') then
        transfer_ongoing <= '0';
        clk_counter      <= (others => '0');
        bit_counter      <= (others => '0');
        tx_shift_reg     <= (others => '0');
        tx_bit           <= '1';
        rx_shift_reg     <= (others => '0');
        rx_value_debug   <= (others => '0');
        btransfer_debug_done   <= '0';
      elsif (CLK'event and CLK = '1') then
        --default: 
        btransfer_debug_done <= '0';
        if transfer_ongoing = '0' then
          if start_transfer_debug = '1' then
            clk_counter   <= (others => '0');
            bit_counter   <= (others => '0');
            transfer_ongoing <= '1';
            tx_shift_reg  <= tx_value_debug;
            tx_bit        <= '0';  --start bit!
          end if;
        else
          clk_counter <= clk_counter + 1;
          if clk_counter = 142 then 
            rx_shift_reg <= rx_bit & rx_shift_reg(7 downto 1);
          elsif clk_counter = 284 then
            clk_counter  <= (others => '0');
            bit_counter <= bit_counter + 1;
            tx_shift_reg <= '0' & tx_shift_reg(7 downto 1); 
            if bit_counter = 8 then
              tx_bit   <= '1'; --stop bit!
              rx_value_debug <= rx_shift_reg;
            elsif bit_counter = 9 then
              transfer_ongoing <= '0';
              btransfer_debug_done   <= '1';
            else
              tx_bit <= tx_shift_reg(0);
            end if;
          end if;
        end if;
      end if;
    end process;
  
  
  
END rtl;

