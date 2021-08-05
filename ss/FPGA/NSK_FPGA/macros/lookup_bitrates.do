#lookup_bitrates.do

destroy wave
view wave
delete wave *
add wave /lookup_bitrates/clk
add wave /lookup_bitrates/reset_n
add wave /lookup_bitrates/brr
add wave /lookup_bitrates/brt_vec


restart -force
force -freeze -repeat 30ns clk 1 0, 0 15ns
force -freeze reset_n 1 0ns, 0 50ns, 1 100ns
force -freeze brr(0) X"00" 0ns, X"01" 100ns, X"02" 200 ns, X"03" 300ns
force -freeze brr(1) X"10" 0ns, X"11" 100ns, X"12" 200ns, X"13" 300ns
force -freeze brr(2) X"30" 0ns, X"21" 100ns, X"22" 200ns, X"23" 300ns
force -freeze brr(3) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(4) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(5) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(6) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(7) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(8) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(9) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns
force -freeze brr(10) X"40" 0ns, X"31" 100ns, X"32" 200ns, X"33" 300ns

run 500ns

