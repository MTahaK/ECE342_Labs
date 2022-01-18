`timescale 1ns/1ns
module upcount_tb();
// Generates a 50MHz clock.
logic clk;
initial clk = 1'b0;
always #10 clk = ~clk;

logic sreset;
logic dut_enable;
logic dut_last;
logic [3:0] dut_val;

logic count12_enable;
logic count112_enable;
logic count12_last;
logic count112_last;
logic [3:0] count12_val;
logic [6:0] count112_val;

upcount # (.N(16)) count16(.clk(clk), .sreset(sreset), .o_val(dut_val), .i_enable(dut_enable), .o_last(dut_last));

upcount # (.N(12)) count12
(
.clk(clk),
.sreset(sreset),
.o_val(count12_val),
.i_enable(count12_enable),
.o_last(count12_last));

upcount #(112) count112
(
.clk(clk),
.sreset(sreset),
.o_val(count112_val),
.i_enable(count112_enable),
.o_last(count112_last));


initial begin
	dut_enable = 1'b0;
	sreset = 1'b1;
	
	@(posedge clk);
	sreset = 1'b0;
	
	@(posedge clk);
	dut_enable = 1'b1;
	
	wait(dut_last);
	if (dut_val !== 'd15) begin
		$display("Error! Computer asserted o_last, but o_val was %d, instead of 15.", dut_val);
		$stop();
	end
	
	@(posedge clk);
	dut_enable = 1'b0;
	
	count12_enable = 1'b0;
	sreset = 1'b1;
	
	@(posedge clk);
	sreset = 1'b0;
	
	@(posedge clk);
	count12_enable = 1'b1;
	
	wait(count12_last);
	if (count12_val !== 'd11) begin
		$display("Error! Computer asserted o_last, but o_val was %d, instead of 11.", dut_val);
		$stop();
	end

	@(posedge clk);
	count12_enable = 1'b0;

	count112_enable = 1'b0;
	sreset = 1'b1;
	
	@(posedge clk);
	sreset = 1'b0;
	
	@(posedge clk);
	count112_enable = 1'b1;
	
	wait(count112_last);
	if (count112_val !== 'd111) begin
		$display("Error! Computer asserted o_last, but o_val was %d, instead of 111.", dut_val);
		$stop();
	end

	@(posedge clk);
	count112_enable = 1'b0;
	
	@(posedge clk);
	$stop();
end
endmodule