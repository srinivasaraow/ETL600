onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /tb_nsk600_top/clk32
add wave -noupdate -format Literal /tb_nsk600_top/testcase
add wave -noupdate -format Logic /tb_nsk600_top/rts1
add wave -noupdate -format Logic /tb_nsk600_top/cts1
add wave -noupdate -format Logic /tb_nsk600_top/dcd1
add wave -noupdate -format Logic /tb_nsk600_top/rck1
add wave -noupdate -format Literal /tb_nsk600_top/i5/nsk_data
add wave -noupdate -divider tb_rxd
add wave -noupdate -format Logic /tb_nsk600_top/txd1
add wave -noupdate -format Logic /tb_nsk600_top/rxd1
add wave -noupdate -format Logic /tb_nsk600_top/txd2
add wave -noupdate -format Logic /tb_nsk600_top/rxd2
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
add wave -noupdate -divider rx_fifo
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/dout
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/enp_dout
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/in_bus
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/rd_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/wr_ptr
add wave -noupdate -divider tx_fifo
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
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_base
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_deno
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_64
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_gen_clk/cnt_speedfract
add wave -noupdate -divider gen_ptr
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/enp_dout
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/in_bus
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/rd_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/wr_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/wr_ptr_i
add wave -noupdate -format Literal /tb_nsk600_top/i5/i2_v24ports/i2_1_v24/i_rxfifo/i_gen_ptr/rd_ptr_i
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
add wave -noupdate -divider generate_ofdm_clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/basecount
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/numerator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/denominator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/clockregulator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/reset_n
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/enp_rxd
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/cnt_base
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/cnt_deno
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/cnt_64
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/cnt_speedfract
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/rxc_last
add wave -noupdate -format Logic /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/rxc_i
add wave -noupdate -format Literal /tb_nsk600_top/i5/i8_ofdmdspif/i8_1_ofdm/i_gen_clk/start_level
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
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/mux_ch_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/get_1
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/ch
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/get
add wave -noupdate -format Literal -radix binary -expand /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/data
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/frame_end
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/pre
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/cnt_agr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/cnt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/current_state
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/next_state
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/txd_agr
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
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/enp_122m
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rts
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/cts
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/rxd
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/status_v24
add wave -noupdate -divider v11_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/asynchronous
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/bypass_uar
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/clk
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/reset_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/txd_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/enp_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/txd_synced
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/txd_synced_int
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/txd_filt_last
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/smp_pos
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/current_state
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/next_state
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_asynctosync/enp_tx_int
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/tx_v24
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/asynchronous
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/basecount
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/bypass_uar
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/clockregulator
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/cnt_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/data_sensitive
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/denominator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/en_rxc
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/enp_over16
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/enp_rxd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/enp_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/invert_clock_polarity
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/jabber_controller
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/jabber_halt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/loop_cts
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/numerator
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/port_enable
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/tx_on
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/txd_filt
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/txd_synced
add wave -noupdate -divider {New Divider}
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/enp_dout
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/in_bus
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/page
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/dout
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/dout_ram
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/rd_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i3_v11ports/i3_1_v11/i_rxfifo/wr_ptr
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {126420915 ps} 0} {{Cursor 2} {10212375771 ps} 0}
WaveRestoreZoom {0 ps} {98817475605 ps}
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
