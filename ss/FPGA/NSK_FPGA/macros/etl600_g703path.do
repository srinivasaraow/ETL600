onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rx_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/rx_p
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/cnt_rx
add wave -noupdate -divider {octet timing}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/txd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/enp_rx_octet
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/tx_ch9
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/i_gen_ptr/wr_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/i_gen_ptr/rd_ptr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/level
add wave -noupdate -format Logic /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/enp_dout
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/i_gen_ptr/cnt_info
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/i_gen_ptr/shift_info
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_19_txfifo/i_gen_ptr/vec_info
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/cnt_pos
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/position
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/current_state
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_2_enframer/chpos
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i7_muxports/tx_agr
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/poscounts
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/rx_agr
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/cnt
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/chpos
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/sync_info
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/rx_ch9
add wave -noupdate -format Literal -expand /tb_nsk600_top/i5/i9_g703/rx_g703
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_rxfifo/i_gen_ptr/cnt_info
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_rxfifo/i_gen_ptr/shift_info
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_rxfifo/i_gen_ptr/vec_info
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_rxfifo/level
add wave -noupdate -format Literal /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_rxfifo/cnt_pos
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/i_rxfifo/enp_dout
add wave -noupdate -format Literal /tb_nsk600_top/i5/i7_muxports/i7_4_deframer/current_state
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/dcd
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/enp_rx_char
add wave -noupdate -format Literal -radix unsigned /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/cnt_tx
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_1_g703port/rxd
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/tx_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/i9_g703/i9_2_g703codec/tx_p
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {60134006573700 fs} 0} {{Cursor 2} {933099133810800 fs} 0} {{Cursor 3} {10264462520700 fs} 0}
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
WaveRestoreZoom {10263666473200 fs} {10267327309 ps}
