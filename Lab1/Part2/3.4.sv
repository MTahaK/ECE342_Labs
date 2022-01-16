module upcount #
(
parameter N = 20,
parameter Nbits = $clog2(N)
)
(
input clk,
input sreset,
output [Nbits-1:0] o_val,
input i_enable,
output o_last
);
logic [Nbits-1:0] count;

always_ff @ (posedge clk) begin
	if (sreset)
		count <= 'd0;
	else if (i_enable) begin
		if (o_last) count <= 'd0;
			else count <= count + 'd1;
	end
end

assign o_val = count;
assign o_last = count == N-1;
endmodule
