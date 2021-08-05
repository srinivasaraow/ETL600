onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {g.703 codec}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rx_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rx_p
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/enp_octet
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/tx_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/tx_p
add wave -noupdate -format Literal /tb_nsk600_top/i8/i0/check_txc_period/frequency
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/pat_tx
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/cnt_tx
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/cnt_rx
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rxp_vec
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rxn_vec
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/pol
add wave -noupdate -divider {g.703 tosync}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/control_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/enp_122m
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rts
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rxd
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/status_v24
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/tx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/always_on
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/asynchronous
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/basecount
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/buffer_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/bypass_uar
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/char_format
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/clockregulator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/cts_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/data_sensitive
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/dcd_i
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/denominator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/dout
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/en_rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/en_txc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/enp_rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/enp_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/inverse_clock_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/invert_all_circuits
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/invert_clock_polarity
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/jabber_controller
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/jabber_halt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/loop_cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/loop_tx_rx
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/mode
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/numerator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rts_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rts_synced
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rxc_int
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/tx_on
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txc_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txc_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txd_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txd_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txd_synced
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/enp_over16_internal
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rxc_int_last
add wave -noupdate -divider {tb_frequency measurement}
add wave -noupdate -format Literal /tb_nsk600_top/i8/i0/check_txc_period/frequency
add wave -noupdate -divider {g703 generate clock}
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/basecount
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/buffer_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/clockregulator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/denominator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/en_txc
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/numerator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/reset_n
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/rx_v24
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/enp_rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/rxc_int
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/cnt_base
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/cnt_deno
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/cnt_64
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/cnt_speedfract
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/rxc_last
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/rxc_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/rx_v24_on_last
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_gen_clk/start_level
add wave -noupdate -divider {V11 Port1}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/control_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/enp_122m
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rts
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rxd
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/status_v24
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/tx_v24
add wave -noupdate -divider {V11 Port2}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/control_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/enp_122m
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rts
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rxd
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/status_v24
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/tx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/always_on
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/asynchronous
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/basecount
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/buffer_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/bypass_uar
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/char_format
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/clockregulator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/cts_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/data_sensitive
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/dcd_i
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/denominator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/dout
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/en_rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/en_txc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/enp_rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/enp_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/inverse_clock_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/invert_all_circuits
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/invert_clock_polarity
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/jabber_controller
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/jabber_halt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/loop_cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/loop_tx_rx
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/mode
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/numerator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rts_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rts_synced
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rxc_int
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/tx_on
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/txc_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/txd_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/txd_i
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/txd_synced
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_2_v11/rxc_int_last
add wave -noupdate -divider {ofdm_fpga IF}
add wave -noupdate -format Logic /tb_nsk600_top/d0a
add wave -noupdate -format Logic /tb_nsk600_top/d2a
add wave -noupdate -format Logic /tb_nsk600_top/sclk0
add wave -noupdate -format Logic /tb_nsk600_top/sclk2
add wave -noupdate -format Logic /tb_nsk600_top/v11_control
add wave -noupdate -format Logic /tb_nsk600_top/v11_indication
add wave -noupdate -format Literal /tb_nsk600_top/i5/control_v11
add wave -noupdate -format Literal /tb_nsk600_top/i5/rx_v11_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/rx_v11_2
add wave -noupdate -format Literal /tb_nsk600_top/i5/status_v11
add wave -noupdate -format Literal /tb_nsk600_top/i5/tx_v11_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/tx_v11_2
add wave -noupdate -format Literal /tb_nsk600_top/i5/rx_ofdm
add wave -noupdate -format Literal /tb_nsk600_top/i5/tx_ofdm
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {20427116488 ps} 0} {{Cursor 2} {5294873 ps} 0} {{Cursor 3} {31044511795 ps} 0}
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
update
WaveRestoreZoom {0 ps} {8076777149 ps}
