module upcount15
(
    input clk,
    input sreset, // sync reset
    output [3:0] o_val,
    input i_enable,
    output o_last,

);
