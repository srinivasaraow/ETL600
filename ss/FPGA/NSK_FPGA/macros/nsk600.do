view wave

onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /tb_nsk600_top/i5/cs_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/wr_n
add wave -noupdate -format Logic /tb_nsk600_top/i5/rd_n
add wave -noupdate -format Literal /tb_nsk600_top/i5/dsp_data
add wave -noupdate -format Logic /tb_nsk600_top/i5/clk
add wave -noupdate -format Logic /tb_nsk600_top/i5/clk65
add wave -noupdate -format Logic /tb_nsk600_top/dsp_flag0
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Literal /tb_nsk600_top/testcase
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/txd1
add wave -noupdate -format Logic /tb_nsk600_top/rxd1
add wave -noupdate -format Logic /tb_nsk600_top/rck1
add wave -noupdate -format Logic /tb_nsk600_top/rts1
add wave -noupdate -format Logic /tb_nsk600_top/cts1
add wave -noupdate -format Logic /tb_nsk600_top/dcd1
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/txd2
add wave -noupdate -format Logic /tb_nsk600_top/rxd2
add wave -noupdate -format Logic /tb_nsk600_top/rck2
add wave -noupdate -format Logic /tb_nsk600_top/rts2
add wave -noupdate -format Logic /tb_nsk600_top/cts2
add wave -noupdate -format Logic /tb_nsk600_top/dcd2
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/txd3
add wave -noupdate -format Logic /tb_nsk600_top/rxd3
add wave -noupdate -format Logic /tb_nsk600_top/rck3
add wave -noupdate -format Logic /tb_nsk600_top/rts3
add wave -noupdate -format Logic /tb_nsk600_top/cts3
add wave -noupdate -format Logic /tb_nsk600_top/dcd3
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Logic /tb_nsk600_top/txd4
add wave -noupdate -format Logic /tb_nsk600_top/rxd4
add wave -noupdate -format Logic /tb_nsk600_top/rck4
add wave -noupdate -format Logic /tb_nsk600_top/rts4
add wave -noupdate -format Logic /tb_nsk600_top/cts4
add wave -noupdate -format Logic /tb_nsk600_top/dcd4
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Literal /tb_nsk600_top/dsp_status
add wave -noupdate -format Literal /tb_nsk600_top/v_24_status
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {1762067820 ps} 0}
WaveRestoreZoom {0 ps} {19114914811 ps}
configure wave -namecolwidth 169
configure wave -valuecolwidth 81
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
