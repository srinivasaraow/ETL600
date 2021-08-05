onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /tb_nsk600_top/clk32
add wave -noupdate -format Literal /tb_nsk600_top/testcase
add wave -noupdate -format Logic /tb_nsk600_top/rts1
add wave -noupdate -format Logic /tb_nsk600_top/cts1
add wave -noupdate -format Logic /tb_nsk600_top/dcd1
add wave -noupdate -format Logic /tb_nsk600_top/rck1
add wave -noupdate -format Logic /tb_nsk600_top/i5/cs_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/rd_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/wr_n
add wave -noupdate -format Literal /tb_nsk600_top/i5/nsk_adr
add wave -noupdate -format Literal /tb_nsk600_top/i5/nsk_data
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
add wave -noupdate -divider ofdmif
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i8_ofdmdspif/control_ofdm
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/rx_ofdm
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/con
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/ind
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/sport0_d
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/sport0_sclk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/sport2_d
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/sport2_sclk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/clk
add wave -noupdate -divider mux_ports
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/data_7
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/data_8
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/get_7
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/get_8
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/tx_ch7
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/tx_ch8
add wave -noupdate -divider enframer
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/bitcounts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/data_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/level_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/mux_ch_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/get_1
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/txd_agr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/pre
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/cnt_agr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/cnt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/get
add wave -noupdate -format Literal -radix binary -expand /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/data
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/frame_end
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/ch
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/current_state
add wave -noupdate -divider frame_sync_detection
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/preamble_check
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/reset_n
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/rx_agr
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/preamble_detected
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/preamble_buf
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/preamble_detected
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/sync_history
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_5_sync_detection/frame_sync
add wave -noupdate -divider deframer
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/current_state
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/next_state
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/cnt_agr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/cnt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/ch
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rxd
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/enp_rx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/mux_ch_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/reset_n
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_agr
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/preamble_check
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch1
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch10
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch2
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch3
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch4
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch5
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch6
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch7
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch8
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/rx_ch9
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/bitcounts
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/clk
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/control_v24
add wave -noupdate -divider port1
add wave -noupdate -format Logic /tb_nsk600_top/i5/rts1
add wave -noupdate -format Logic /tb_nsk600_top/i5/cts1
add wave -noupdate -format Logic /tb_nsk600_top/i5/dcd1
add wave -noupdate -format Logic /tb_nsk600_top/i5/txd1
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxc1
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxd1
add wave -noupdate -divider port2
add wave -noupdate -format Logic /tb_nsk600_top/i5/rts2
add wave -noupdate -format Logic /tb_nsk600_top/i5/cts2
add wave -noupdate -format Logic /tb_nsk600_top/i5/dcd2
add wave -noupdate -format Logic /tb_nsk600_top/i5/txd2
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxc2
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxd2
add wave -noupdate -divider port3
add wave -noupdate -format Logic /tb_nsk600_top/i5/rts3
add wave -noupdate -format Logic /tb_nsk600_top/i5/cts3
add wave -noupdate -format Logic /tb_nsk600_top/i5/dcd3
add wave -noupdate -format Logic /tb_nsk600_top/i5/txd3
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxc3
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxd3
add wave -noupdate -divider port4
add wave -noupdate -format Logic /tb_nsk600_top/i5/rts4
add wave -noupdate -format Logic /tb_nsk600_top/i5/cts4
add wave -noupdate -format Logic /tb_nsk600_top/i5/dcd4
add wave -noupdate -format Logic /tb_nsk600_top/i5/txd4
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxc4
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxd4
add wave -noupdate -divider port5
add wave -noupdate -format Logic /tb_nsk600_top/i5/rts5
add wave -noupdate -format Logic /tb_nsk600_top/i5/cts5
add wave -noupdate -format Logic /tb_nsk600_top/i5/dcd5
add wave -noupdate -format Logic /tb_nsk600_top/i5/txd5
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxc5
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxd5
add wave -noupdate -divider port6
add wave -noupdate -format Logic /tb_nsk600_top/i5/rts6
add wave -noupdate -format Logic /tb_nsk600_top/i5/cts6
add wave -noupdate -format Logic /tb_nsk600_top/i5/dcd6
add wave -noupdate -format Logic /tb_nsk600_top/i5/txd6
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxc6
add wave -noupdate -format Logic /tb_nsk600_top/i5/rxd6
add wave -noupdate -divider port7
add wave -noupdate -format Logic /tb_nsk600_top/i5/c1
add wave -noupdate -format Logic /tb_nsk600_top/i5/i1
add wave -noupdate -format Logic /tb_nsk600_top/i5/t1
add wave -noupdate -format Logic /tb_nsk600_top/i5/s1
add wave -noupdate -format Logic /tb_nsk600_top/i5/r1
add wave -noupdate -divider port8
add wave -noupdate -format Logic /tb_nsk600_top/i5/c2
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2
add wave -noupdate -format Logic /tb_nsk600_top/i5/t2
add wave -noupdate -format Logic /tb_nsk600_top/i5/s2
add wave -noupdate -format Logic /tb_nsk600_top/i5/r2
add wave -noupdate -divider {New Divider}
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {31449333065 ps} 0} {{Cursor 2} {11539870464 ps} 0}
WaveRestoreZoom {0 ps} {7633890394 ps}
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
