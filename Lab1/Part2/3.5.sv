`timescale 1ns/1ns
module upcount15_tb();
// Generates a 50MHz clock.
logic clk;
initial clk = 1'b0;
always #10 clk = ~clk;

logic sreset;
logic dut_enable;
logic dut_last;
logic [3:0] dut_val;

logic [3:0] count17_val;
logic [6:0] count112_val;

upcount15 DUT(.clk(clk), .sreset(sreset), .o_val(dut_val), .i_enable(dut_enable), .o_last(dut_last));

upcount # (.N(17)) count17
(
.clk(clk),
.sreset(sreset),
.o_val(count17_val),
.i_enable(count17_enable),
.o_last(count17_last));

upcount #(112) count112
(
.clk(clk),
.sreset(sreset),
.o_val(count112_val),
.i_enable(count112_enable),
.o_last(count112_last));


initial begin
	dut_enable = 1'b0;
	reset = 1'b1;
	
	@(posedge clk);
	reset = 1'b0;
	
	@(posedge clk);
	dut_enable = 1'b1;
	
	wait(dut_last);
	if (dut_val !== 4'd15) begin
		$display("Error! Computer asserted o_last, but o_val was %d, instead of 15.", dut_val);
		$stop();
	end
	
	@(posedge clk);
	$stop();
end
endmodule