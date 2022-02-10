`timescale 1ns/1ns
module mult_tb();
	logic [7:0] dut_x, dut_y;
	logic [15:0] dut_out;

  mult_csm DUT
  (
    .x(dut_x),
    .y(dut_y),
    .out(dut_out)
  );
	
	initial begin
      
      	// functionality checking
      	for (integer x = 0; x < 256; x++) begin
          	for (integer y = 0; y < 256; y++) begin
				
				logic [15:0] realprod;
				realprod = x*y;
				dut_x = x[7:0];
				dut_y = y[7:0];
				
				#5
              
		        if (realprod !== dut_out) begin
                    $display("Functionality Checking: Mismatch %0d * %0d = %0d, expected %0d", x, y, dut_out, realprod);
					$stop();
		        end

			end
        end
      	$display("Checking Passed");
		$stop();
	end

	

endmodule