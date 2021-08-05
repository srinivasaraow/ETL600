onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {TB sport13 internal sig}
add wave -noupdate -format Literal /tb_o4cv_top/i_master/testcase
add wave -noupdate -format Logic /tb_o4cv_top/i_master/tb_sport13_ok
add wave -noupdate -format Logic /tb_o4cv_top/i_master/clk_32m
add wave -noupdate -divider {Config IF}
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/cs_n
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/nsk_adr
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/rd_n
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/rx_dsp
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/tdm_fs
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/tx_dsp
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/wr_n
add wave -noupdate -format Literal /tb_o4cv_top/i_nskdsp/testword
add wave -noupdate -format Literal /tb_o4cv_top/nsk_data
add wave -noupdate -divider {TDM Bus}
add wave -noupdate -format Logic /tb_o4cv_top/tdm_data
add wave -noupdate -format Logic /tb_o4cv_top/tdm_fs
add wave -noupdate -format Logic /tb_o4cv_top/tdm_rxd
add wave -noupdate -format Logic /tb_o4cv_top/tdm_txd
add wave -noupdate -format Logic /tb_o4cv_top/tdm_wr_n
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/current_state
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/tpads_in
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/tpads_out
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/loop_tx
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/shf_tdm
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/loop_rx
add wave -noupdate -divider {TB sport13 Tx}
add wave -noupdate -format Literal /tb_o4cv_top/i_master/subframe_length
add wave -noupdate -format Literal -radix hexadecimal -expand /tb_o4cv_top/i_master/stream_tx
add wave -noupdate -format Literal /tb_o4cv_top/i_master/tx_30ms_start
add wave -noupdate -format Literal /tb_o4cv_top/i_master/tdm_tx_proc/ptr_93us_txframe
add wave -noupdate -format Literal /tb_o4cv_top/i_master/subframe_length
add wave -noupdate -format Literal -radix hexadecimal -expand /tb_o4cv_top/i_master/tdm_tx_proc/tdmfrag_tx
add wave -noupdate -format Literal -radix hexadecimal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/shf_tdm
add wave -noupdate -divider {TDM bus interface Tx}
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/reset_n
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/clk
add wave -noupdate -format Logic /tb_o4cv_top/i_master/tdm_fs
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/txd
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/en_tx
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/enp_txd
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i11_tdm/tx_tdm
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/cnt_3072
add wave -noupdate -divider {Pre Mux Tx}
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_3_maptx/cnt_tdmframe
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_3_maptx/cnt_tdmpos
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_3_maptx/wr_page
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_3_maptx/ptr_int
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_3_maptx/cnt_info
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_3_maptx/shift_info
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/enp_txd
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/txd
add wave -noupdate -format Literal -radix unsigned /tb_o4cv_top/i_nsk600_top/i7_muxports/wr_ptr_tx
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/rd_ptr_tx
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_21_ram_tdmtx/dout
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_21_ram_tdmtx/ram
add wave -noupdate -divider MUX-Config
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/bitcounts
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/mux_ch_enable
add wave -noupdate -divider {Mux Enframer}
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_2_enframer/current_state
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_2_enframer/cnt
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_2_enframer/cnt_agr
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/data_tdm_tx
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i7_muxports/tx_agr
add wave -noupdate -divider {Mux Deframer}
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/bitcounts
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/clk
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/frame_sync
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/mux_ch_enable
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/poscounts
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/preamble_detected
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/reset_n
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_agr
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/sync_history
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/preamble_check
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch1
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch2
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch3
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch4
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch5
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch6
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch7
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch8
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch9
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_ch10
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rx_tdm
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/ch
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/sync_info
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/cnt_agr
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/cnt
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/rxd
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/enp_rx
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/ch_start
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/enp_agr
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/chpos
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i7_muxports/i7_4_deframer/current_state
add wave -noupdate -divider {Map TDM Rx}
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/rx_tdm
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/enp_rxd
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/rxd_in
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/cnt_info
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/shift_info
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/vec_info
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/wr_ptr_rx_i
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/i11_tdm/rd_page
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_3_maprx/wr_ptr_rx
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_4_ram_tdmrx/ram
add wave -noupdate -divider {TDM Bus interface Rx}
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/cnt_tdmframe
add wave -noupdate -format Literal -radix unsigned /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/rd_ptr_rx
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/trig_page
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/shf_info
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/rxd
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/cnt_tdmpos
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/i11_tdm/i11_1_tdmsched/current_state
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/i11_tdm/tdm_wr_n
add wave -noupdate -format Logic /tb_o4cv_top/i_nsk600_top/tdm_rxd
add wave -noupdate -divider {Tb sport13 Rx}
add wave -noupdate -format Literal -radix hexadecimal -expand /tb_o4cv_top/i_master/tdm_tx_proc/tdmfrag_rx
add wave -noupdate -format Literal /tb_o4cv_top/i_master/tdm_tx_proc/ptr_93us_rxframe
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_master/rxframe10ms_ready
add wave -noupdate -format Literal /tb_o4cv_top/i_master/tdm_tx_proc/cnt_bit_rxframe
add wave -noupdate -format Literal /tb_o4cv_top/i_master/tdm_tx_proc/ch_rx
add wave -noupdate -format Literal -radix hexadecimal -expand /tb_o4cv_top/i_master/stream_tx_b2b
add wave -noupdate -format Literal -radix hexadecimal -expand /tb_o4cv_top/i_master/stream_rx
add wave -noupdate -divider {Legacy Data signals}
add wave -noupdate -format Logic /tb_o4cv_top/cts1
add wave -noupdate -format Logic /tb_o4cv_top/cts2
add wave -noupdate -format Logic /tb_o4cv_top/cts3
add wave -noupdate -format Logic /tb_o4cv_top/cts4
add wave -noupdate -format Literal -expand /tb_o4cv_top/i_nsk600_top/rx_ofdm
add wave -noupdate -format Literal /tb_o4cv_top/i_nsk600_top/rx_agr
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {76145691027 ps} 0} {{Cursor 2} {20286893543 ps} 0}
configure wave -namecolwidth 189
configure wave -valuecolwidth 692
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
WaveRestoreZoom {0 ps} {137130540228 ps}
