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
upcount15 DUT(.clk(clk), .sreset(sreset), .o_val(dut_val), .i_enable(dut_enable), .o_last(dut_last));

initial begin
	dut_enable = 1'b0;
	sreset = 1'b1;
	
	@(posedge clk);
	sreset = 1'b0;
	
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
	
	
	
	
	
	
	