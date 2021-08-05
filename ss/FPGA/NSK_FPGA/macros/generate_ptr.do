#generate_ptr.do

delete wave *

add wave -noupdate -format Logic /generate_ptr/clk
add wave -noupdate -format Literal /generate_ptr/page
add wave -noupdate -format Literal -expand /generate_ptr/in_bus
add wave -noupdate -format Literal /generate_ptr/level
add wave -noupdate -format Literal /generate_ptr/wr_ptr_i
add wave -noupdate -format Literal /generate_ptr/rd_ptr_i
add wave -noupdate -format Literal /generate_ptr/cnt_info
add wave -noupdate -format Literal /generate_ptr/shift_info
add wave -noupdate -format Literal /generate_ptr/vec_info
add wave -noupdate -format Literal /generate_ptr/cnt_pos_i
add wave -noupdate -format Literal /generate_ptr/cnt_pos
add wave -noupdate -format Logic /generate_ptr/enp_dout


restart -force
force -freeze -repeat 30ns clk 1 0ns, 0 15ns
run 1 ns
#force -freeze sim:/generate_ptr/clk 1 0, 0 {15 ns} -r 30ns
force -freeze page "01" 0, "00" 50ns, "01" 100ns
force -freeze -repeat 6000ns in_bus(2) 0 0ns, 1 30ns
force -freeze -repeat 5970ns enp_dout 1 0ns, 0 30ns, 1 750ns, 0 780ns, 1 1500ns, 0 1530ns, 1 2250ns, 0 2280ns, 1 3000ns, 0 3030ns, 1 3750ns, 0 3780ns, 1 4500ns, 0 4530ns, 1 5250ns, 0 5280ns
run 390 ns

force -freeze in_bus(1) 1
force -freeze -repeat 750ns in_bus(0) 1 0ns, 0 30ns
run 0.6 ms

noforce enp_dout
force -freeze -repeat 6030ns enp_dout 1 0ns, 0 30ns, 1 750ns, 0 780ns, 1 1500ns, 0 1530ns, 1 2250ns, 0 2280ns, 1 3000ns, 0 3030ns, 1 3750ns, 0 3780ns, 1 4500ns, 0 4530ns, 1 5250ns, 0 5280ns
run 0.6 ms
