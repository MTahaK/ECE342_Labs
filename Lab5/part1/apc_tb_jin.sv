`timescale 1ns/1ns
module apc_tb_jin();
    logic clock = 0;
    logic reset = 1;
    logic [2:0] avs_address = '0;
    logic avs_read = 0;
    logic avs_write = 0;    
    logic [31:0] avs_writedata = '0;
	
	logic [31:0] avs_readdata;
    logic avs_waitrequest;
	
	real start_time;
	real time_taken;
	
    avalon_fp_mult DUT
    (
        .clk(clock),
        .reset(reset),
        .avs_s1_address(avs_address),
        .avs_s1_read(avs_read),
        .avs_s1_write(avs_write),
        .avs_s1_writedata(avs_writedata),
        .avs_s1_readdata(avs_readdata),
        .avs_s1_waitrequest(avs_waitrequest)
    );

	// The clock is set this way so that the TB can change 
	// inputs independent of clock edges. This is to check
	// that readdata changes immediately and not just on 
	// a clock edge. 
 	always #1 clock = ~clock;
   
	task automatic apc_test(
		input [31:0] number1,
		input [31:0] number2,
		input [31:0] result
	);
	begin			 
		
		#4
		avs_writedata = number1;     
		avs_write = 1;
		avs_address = 0;
		#2
		avs_write = 0;
		#2
		avs_writedata = number2;     
		avs_write = 1;
		avs_address = 1;
		#2
		avs_write = 0;
		#2
		avs_writedata = 1; //start flag = 1
		avs_write = 1;
		avs_address = 2;
		#2
		avs_write = 0;
		#24
		avs_read = 1;
		avs_address = 3;
		#2
		avs_address = 4;
		#2
		avs_read = 0;
		// Save the time the avs_read was asserted. 
		start_time = $realtime;
				
		//wait (avs_readdata == 0);
		
		// How long did it take for waitrequest to be de-asserted? 
		//time_taken = $realtime - start_time;
		/*	
		if (time_taken < 22) begin
			$display("The waitrequest was de-asserted too soon.");
			$stop;
		end
			
		if (time_taken > 25) begin
			$display("The waitrequest was asserted for too long");
			$stop;
		end		
		*/
		/*
		#0.5
		if (avs_readdata != result) 
			$display("FAIL: %X * %X should be %X but got %X instead.", number1, number2, result, avs_readdata);
		else 
			$display("SUCCESS: %X * %X = %X", number1, number2, result);
		end
		avs_read = 0; 
		*/
	end
	endtask    
    
    initial begin
        #3
        reset = 0;
		
		apc_test(32'h4059999A, 32'h4194CCCD, 32'h427CF5C3); // 3.4 * 18.6 = 63.24

		#30

		apc_test(32'h41200000, 32'h425E0000, 32'h440AC000); // 10.0 * 55.5 = 555.0 
		// Add test cases for special cases here. 
		
		#30

		$stop();

    end
endmodule