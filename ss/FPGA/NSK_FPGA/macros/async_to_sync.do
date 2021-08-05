#async_to_sync.do
#stimuli for serial_port !!! not only async_to_sync
delete wave *

add wave -noupdate -format Logic /serial_port/i_asynctosync/asynchronous
add wave -noupdate -format Logic /serial_port/i_asynctosync/bypass_uar
add wave -noupdate -format Literal /serial_port/i_asynctosync/char_format
add wave -noupdate -format Logic /serial_port/i_asynctosync/clk
add wave -noupdate -format Literal /serial_port/i_asynctosync/cnt_over16
add wave -noupdate -format Logic /serial_port/i_asynctosync/enp_over16
add wave -noupdate -format Logic /serial_port/i_asynctosync/inverse_clock_tx
add wave -noupdate -format Logic /serial_port/i_asynctosync/reset_n
add wave -noupdate -format Logic /serial_port/i_asynctosync/txd_filt
add wave -noupdate -format Logic /serial_port/i_asynctosync/enp_tx
add wave -noupdate -format Logic /serial_port/i_asynctosync/txd_synced
add wave -noupdate -format Logic /serial_port/i_asynctosync/txd_synced_int
add wave -noupdate -format Logic /serial_port/i_asynctosync/txd_filt_last
add wave -noupdate -format Literal /serial_port/i_asynctosync/smp_pos
add wave -noupdate -format Literal /serial_port/i_asynctosync/last_start_pos
add wave -noupdate -format Literal /serial_port/i_asynctosync/cnt_comp
add wave -noupdate -format Literal /serial_port/i_asynctosync/cnt_chbit
add wave -noupdate -format Literal /serial_port/i_asynctosync/current_state
add wave -noupdate -format Literal /serial_port/i_asynctosync/next_state
add wave -noupdate -format Logic /serial_port/i_asynctosync/enp_tx_int
add wave -noupdate -format Literal /serial_port/i_gen_clk/basecount
add wave -noupdate -format Logic /serial_port/i_gen_clk/buffer_enable
add wave -noupdate -format Logic /serial_port/i_gen_clk/clk
add wave -noupdate -format Logic /serial_port/i_gen_clk/clockregulator
add wave -noupdate -format Literal /serial_port/i_gen_clk/denominator
add wave -noupdate -format Logic /serial_port/i_gen_clk/en_txc
add wave -noupdate -format Logic /serial_port/i_gen_clk/enp_txc
add wave -noupdate -format Literal /serial_port/i_gen_clk/level
add wave -noupdate -format Literal /serial_port/i_gen_clk/numerator
add wave -noupdate -format Logic /serial_port/i_gen_clk/port_enable
add wave -noupdate -format Logic /serial_port/i_gen_clk/reset_n
add wave -noupdate -format Literal /serial_port/i_gen_clk/rx_v24
add wave -noupdate -format Literal /serial_port/i_gen_clk/cnt_over16
add wave -noupdate -format Logic /serial_port/i_gen_clk/enp_over16
add wave -noupdate -format Logic /serial_port/i_gen_clk/enp_rxd
add wave -noupdate -format Logic /serial_port/i_gen_clk/rxc_int
add wave -noupdate -format Literal /serial_port/i_gen_clk/cnt_base
add wave -noupdate -format Literal /serial_port/i_gen_clk/cnt_deno
add wave -noupdate -format Literal /serial_port/i_gen_clk/cnt_64
add wave -noupdate -format Literal /serial_port/i_gen_clk/cnt_speedfract
add wave -noupdate -format Logic /serial_port/i_gen_clk/rxc_last
add wave -noupdate -format Logic /serial_port/i_gen_clk/rxc_i
add wave -noupdate -format Literal /serial_port/i_gen_clk/start_level
add wave -noupdate -format Literal /serial_port/tx_v24

restart -force
force -freeze -repeat 30517578fs clk 1 0, 0 15258789fs
force -freeze reset_n 1 0, 0 50, 1 100
force -freeze  char_format 3
force -freeze  bypass_uar 0
force -freeze  inverse_clock_tx 0
force -freeze  asynchronous 1
#force -freeze -repeat 52.0833333us cnt_over16 0 0, 1 3.255us, 2 6.510us, 3 9.766us, 4 13.02us, 5 16.28us, 6 19.53us, 7 22.79us, 8 26.04us, 9 29.30us, 10 32.55us, 11 35.81us, 12 39.06us, 13 42.32us, 14 45.57us, 15 48.83us
#force -freeze -repeat 52.0833333us cnt_over16 0 0, 1 3.255us, 2 6.510us, 3 9.766us, 4 13.02us, 5 16.28us, 6 19.53us, 7 22.79us, 8 26.04us, 9 29.30us, 10 32.55us, 11 35.81us, 12 39.06us, 13 42.32us, 14 45.57us, 15 48.83us

force -freeze  clockregulator 0
force -freeze  buffer_enable 1

force -freeze  port_enable 1
force -freeze  level 4
force -freeze  rx_v24 "010"
force -freeze  basecount 26
force -freeze  numerator 2
force -freeze  denominator 3
force -freeze  invert_all_circuits 0
force -freeze  mode "00"
force -freeze  rts 0



force -freeze txd 1 0
run 1us

#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us
#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us
#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us
#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us
#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us
#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us



#long underspeed message 
force -repeat 525us txd 0 0us, 1 468.8us
run 7.3ms
noforce txd
force txd 1 0
run 1 ms


#long overspeed message 
force -repeat 515us txd 0 0us, 1 468.8us
run 6.15ms
noforce txd
force txd 1 0
run 1 ms


#character wise with bit offset
force txd 0 0us, 1 468.8us
run 800us

force txd 0 0us, 1 468.8us
run 800us

force txd 0 0us, 1 468.8us
run 800us


#ultraprecision tests
#fewest underspeed
force -repeat 520.84us txd 0 0us, 1 468.8us
run 450ms
noforce txd
force txd 1 0
run 1ms

#fewest overspeed 
force -repeat 520.8us txd 0 0us, 1 468.8us
run 450ms
noforce txd
force txd 1 0
run 1ms

