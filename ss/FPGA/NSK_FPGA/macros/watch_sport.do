#watch_sport.do
#stimuli for framesync-hack since prototype4 boards !!!
delete wave *

restart -force
force -freeze -repeat 30517578fs clk 1 0, 0 15258789fs
force -freeze reset_n 1 0, 0 50, 1 100
#force -freeze -repeat 52.0833333us cnt_over16 0 0, 1 3.255us, 2 6.510us, 3 9.766us, 4 13.02us, 5 16.28us, 6 19.53us, 7 22.79us, 8 26.04us, 9 29.30us, 10 32.55us, 11 35.81us, 12 39.06us, 13 42.32us, 14 45.57us, 15 48.83us
#force -freeze -repeat 52.0833333us cnt_over16 0 0, 1 3.255us, 2 6.510us, 3 9.766us, 4 13.02us, 5 16.28us, 6 19.53us, 7 22.79us, 8 26.04us, 9 29.30us, 10 32.55us, 11 35.81us, 12 39.06us, 13 42.32us, 14 45.57us, 15 48.83us


run 1ms

