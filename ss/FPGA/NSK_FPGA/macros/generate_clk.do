#generate_clk.do

delete wave *

add wave -noupdate -format Logic /generate_clk/clk
add wave -noupdate -format Logic /generate_clk/dspregenerator
add wave -noupdate -format Logic /generate_clk/en_rxc
add wave -noupdate -format Logic /generate_clk/clockregulator
add wave -noupdate -format Logic /generate_clk/invert_clock_polarity
add wave -noupdate -format Literal /generate_clk/level
add wave -noupdate -format Logic /generate_clk/port_enable
add wave -noupdate -format Logic /generate_clk/reset_n
add wave -noupdate -format Literal /generate_clk/rx_v24
add wave -noupdate -format Literal /generate_clk/cnt_over16
add wave -noupdate -format Logic /generate_clk/enp_over16
add wave -noupdate -format Literal /generate_clk/basecount
add wave -noupdate -format Literal /generate_clk/numerator
add wave -noupdate -format Literal /generate_clk/denominator
add wave -noupdate -format Literal /generate_clk/cnt_base
add wave -noupdate -format Literal /generate_clk/cnt_deno
add wave -noupdate -format Logic /generate_clk/enp_rxd
add wave -noupdate -format Logic /generate_clk/rxc
add wave -noupdate -format Literal /generate_clk/cnt_64
add wave -noupdate -format Literal /generate_clk/cnt_speedfract
add wave -noupdate -format Literal /generate_clk/speed_level
add wave -noupdate -format Logic /generate_clk/rxc_last
add wave -noupdate -format Logic /generate_clk/rxc_i
add wave -noupdate -format Literal /generate_clk/start_level
add wave -noupdate -format Literal /generate_clk/correction_done

restart -force
force -freeze -repeat 30517578fs clk 1 0, 0 15258789fs
force -freeze reset_n 1 0, 0 50, 1 100
force -freeze  basecount 1
force -freeze  en_rxc 1
force -freeze  invert_clock_polarity 0
force -freeze  dspregenerator 0
force -freeze  clockregulator 0
force -freeze  port_enable 1
force -freeze  level 2
force -freeze  rx_v24 "010"

# 72000 bit/s
force -freeze  basecount 7
force -freeze  numerator 1
force -freeze  denominator 9
run 70 us

# 224000 bit/s
force -freeze  basecount 2
force -freeze  numerator 2
force -freeze  denominator 7
run 250 us

# overspeed
force -freeze  clockregulator 1
force -freeze  level 0
run 500 us
force -freeze  level 1
run 500 us
force -freeze  level 2
run 500 us
force -freeze  level 3
run 500 us
force -freeze  level 4
run 500 us
force -freeze  level 5
run 500 us
force -freeze  clockregulator 0




# 256000 bit/s
force -freeze  basecount 2
force -freeze  numerator 0
force -freeze  denominator 1
run 25 us


# 288000 bit/s
force -freeze  basecount 1
force -freeze  numerator 7
force -freeze  denominator 9
run 20 us

# 320000 bit/s
force -freeze  basecount 1
force -freeze  numerator 3
force -freeze  denominator 5
run 15 us

# 50 bit/s
force -freeze  basecount 10240
force -freeze  numerator 0
force -freeze  denominator 1
run 100 ms