--------------------------------------------------------------------------
-- $Workfile: tb_ft1_pattern_p.vhd $
-- Part of	    :	ETL600 / MUX600
-- Language    :	VHDL
-- Created by	 :	Alexander Gygax, PTUKT2
-- Remarks	    :  
-- Purpose	    : 
-- Copyright	  :	ABB Switzerland Ltd. 2005
---------------------------------------------------------------------------
--
-- $Log: /FPGA/NSK_FPGA/NSK600_tb/hdl_vm/tb_ft1_pattern_p.vhd $
-- 
-- 4     11-07-06 9:28 Chstrue
-- R4 implementation
-- 
-- 3     25.04.05 13:06 Chalgyg
-- -- no description --
-- 
-- 2     12.04.05 11:18 Chalgyg
-- Made all channels the same because DUT is not able to adjust the number
-- of bits yet.
-- 
-- 1     7.04.05 16:28 Chalgyg
-- Initial version.
-- 
---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package tb_ft1_pattern_p is
-----------------------------------------------------------
-- Last edited:
--   110630: bug fix (of old bugs) and adapt to R4

  type t_real_array is array (0 to 5 , 0 to 4) of real;
  type t_slv_vec is array (0 to 5) of std_logic_vector(71 downto 0);
  type t_int_vec is array (0 to 5) of integer;
  type t_char_vec is array (0 to 5) of character;
  
  constant ft1_pattern             : t_slv_vec     := (("111001101001010101000101010101011111110110010001110000101001100011111100"),
                                                       ("001010010101101100101010100101000000001010111101010000110101010111000111"),
                                                       ("100100101010010101100010111111011001010010010110100101010011110100111001"),
                                                       ("000000000000000001111111111111111111100101010101010011100000000010010000"),
                                                       ("111111111111010010100010010100101010010101010101010111011010010111010100"),
                                                       ("010010101101010101001010100101000100111011110100101001011111111010011010"));

  constant nr_data_bits            : t_int_vec     := ( 7,
                                                        8,
                                                        7,
                                                        8,
                                                        8,
                                                        9);
                                                        
  constant parity_bit              : t_char_vec    := ( 'n',
                                                        'o',
                                                        'n',
                                                        'e',
                                                        'n',
                                                        'o');

  constant nr_stop_bits            : t_int_vec     := ( 2,
                                                        1,
                                                        1,
                                                        2,
                                                        1,
                                                        1);

  constant pause_after_char        : t_real_array  := ((1.5, 2.2, 5.0, 21.3, 1.0),
                                                       (2.1, 4.3, 2.3, 12.4, 1.0),
                                                       (2.5, 1.7, 1.8, 14.8, 1.0),
                                                       (1.9, 2.2, 2.0,  6.1, 1.0),
                                                       (3.6, 1.0, 4.1,  9.3, 1.0),
                                                       (2.0, 3.0, 3.1,  4.5, 1.0));

  constant nr_repetitions          : t_int_vec     := ( 5,
                                                        5,
                                                        5,
                                                        5,
                                                        5,
                                                        5);
    
  function parity2nr(parity: character) return integer;

end tb_ft1_pattern_p;
