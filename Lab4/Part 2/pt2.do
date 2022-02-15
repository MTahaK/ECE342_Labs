vlog *.sv
vsim -novopt part2tb
log *
add wave *
run -all