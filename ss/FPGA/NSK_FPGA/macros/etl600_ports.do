onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /tb_nsk600_top/clk32
add wave -noupdate -format Logic /tb_nsk600_top/i5/fpga_reset_n
add wave -noupdate -format Literal /tb_nsk600_top/testcase
add wave -noupdate -format Logic /tb_nsk600_top/rts1
add wave -noupdate -format Logic /tb_nsk600_top/cts1
add wave -noupdate -format Logic /tb_nsk600_top/dcd1
add wave -noupdate -format Logic /tb_nsk600_top/rck1
add wave -noupdate -divider tb_rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/enp_sport
add wave -noupdate -format Logic /tb_nsk600_top/txd1
add wave -noupdate -format Logic /tb_nsk600_top/rxd1
add wave -noupdate -format Logic /tb_nsk600_top/txd2
add wave -noupdate -format Logic /tb_nsk600_top/rxd2
add wave -noupdate -divider dsp_if
add wave -noupdate -format Logic /tb_nsk600_top/i5/cs_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/rd_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/wr_n
add wave -noupdate -format Literal -radix hexadecimal /tb_nsk600_top/i5/i6_config/nsk_adr
add wave -noupdate -format Literal -radix hexadecimal -expand /tb_nsk600_top/dsp_data
add wave -noupdate -format Literal -radix hexadecimal /tb_nsk600_top/i5/i6_config/rx_nsk
add wave -noupdate -format Literal -radix hexadecimal /tb_nsk600_top/i5/i6_config/tx_nsk
add wave -noupdate -divider collect_control
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/control_mux
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/control_switch
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i6_config/control_v24
add wave -noupdate -divider collect_status
add wave -noupdate -format Logic /tb_nsk600_top/i5/i6_config/i6_2_collect_s/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/i6_2_collect_s/status_mux
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/i6_2_collect_s/status_switch
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i6_config/i6_2_collect_s/status_nsk_1
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i6_config/i6_2_collect_s/status_v24
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/i6_2_collect_s/osr
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i6_config/i6_2_collect_s/tsig_nsk
add wave -noupdate -divider nsk-coder
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/tx_nsk_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/i5_codepos/i5_1_codec/i5_1_1_precode/buf_enabl
add wave -noupdate -format Literal /tb_nsk600_top/i5/i5_codepos/i5_1_codec/i5_1_1_precode/buf_value
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/enp_sport
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/i5_codepos/i5_1_codec/cnt_96
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/i5_1_codec/enp_1024k
add wave -noupdate -format Literal /tb_nsk600_top/i5/i5_codepos/i5_1_codec/i5_1_1_precode/tx_pos
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/i5_codepos/i5_1_codec/tx_position_dspif
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/i5_1_codec/txd_dspif
add wave -noupdate -format Literal /tb_nsk600_top/i5/tx_pos_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/tx_pos_2
add wave -noupdate -divider nsk-decoder
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/rx_pos_1
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/rx_pos_2
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/enp_sport
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/i5_codepos/i5_1_codec/position_synced
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/i5_codepos/i5_1_codec/cnt_96
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/i5_1_codec/enp_1024k
add wave -noupdate -format Logic /tb_nsk600_top/i5/i5_codepos/i5_1_codec/rxd_synced
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i5_codepos/i5_1_codec/rx_nsk
add wave -noupdate -format Logic /tb_nsk600_top/dsp_flag0
add wave -noupdate -divider nsk_positionstreams
add wave -noupdate -divider lut_brt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/i6_4_lut_brt/brr
add wave -noupdate -format Logic /tb_nsk600_top/i5/i6_config/i6_4_lut_brt/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i6_config/i6_4_lut_brt/reset_n
add wave -noupdate -format Literal /tb_nsk600_top/i5/i6_config/i6_4_lut_brt/brt_vec
add wave -noupdate -divider rx_fifo
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/dout
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/enp_dout
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/in_bus
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/rd_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/wr_ptr
add wave -noupdate -divider generate_clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/basecount
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/numerator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/denominator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/rx_v24
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/enp_rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/rxc
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_base
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_deno
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_64
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_speedfract
add wave -noupdate -divider gen_ptr
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/enp_dout
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/in_bus
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/page
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/rd_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/wr_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/wr_ptr_i
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/rd_ptr_i
add wave -noupdate -divider async_to_sync
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/inverse_clock_tx
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/char_format
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/enp_over16
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/cnt_over16
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/last_start_pos
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/bypass_uar
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/txd_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/enp_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/txd_filt_last
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/smp_pos
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/cnt_comp
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/cnt_chbit
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/current_state
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/next_state
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/txd_synced
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_asynctosync/enp_tx_int
add wave -noupdate -divider jabber_control
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/data_sensitive
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/enp_122m
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/jabber_controller
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/loop_cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/rts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/rx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/jabber_halt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/tx_on
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/jabber_txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/jabber_active
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/jabber_halt_i
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_contr_jab/cnt_jab
add wave -noupdate -divider {New Divider}
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {29711327199 ps} 0} {{Cursor 2} {10404293 ps} 0}
WaveRestoreZoom {0 ps} {52237459934 ps}
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
