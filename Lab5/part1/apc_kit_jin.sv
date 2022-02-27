module avalon_fp_mult
(
	input clk,
	input reset,
	input [2:0] avs_s1_address,
	input avs_s1_read,
	input avs_s1_write,
	input [31:0] avs_s1_writedata,
	output logic [31:0] avs_s1_readdata,
	output logic avs_s1_waitrequest
);
	// 1. Create the signals to connect to the AVS peripheral registers. 	
	logic [31:0] input1_wire;
	logic [31:0] input2_wire;
	logic [31:0] op1_reg;
	logic [31:0] op2_reg;
	logic [31:0] result_wire;
	logic [31:0] result_reg;
	logic zero; // 011
	logic underflow; // 010
	logic nan; // 100
	logic overflow; // 001
	logic start_reg;
	logic enable;
	logic [2:0] status_reg; //000 means operation completed successfully 
	logic [3:0] count;
	logic done;

	// 2. This instantiates your FP multiplier. Make sure the .qip file is added to your project. 
	fp_mult fpm
	(
		.clk_en(enable),		
		.clock(clk),				
		.dataa(input1_wire),			
		.datab(input2_wire),			
		.nan(nan),			
		.overflow(overflow),	
		.result(result_wire),			
		.underflow(underflow),
		.zero(zero) 			
	);

	/* 3. Write code to handle the read and write operations.
		  It is best to start with write. Using the module signals,
		  you should determine when a write operation occurs and what 
		  register is being written to. 
	*/




	
	// Avalon write
	always_ff @ (posedge clk or posedge reset) begin
		if (reset == 1) begin
			input1_wire <= 0;
			input2_wire <= 0;
			op1_reg <= 0;
			op2_reg <= 0;
			result_wire <= 0;
			result_reg <= 0;
			zero <= 0;
			underflow <= 0;
			nan <= 0;
			overflow <= 0; 
			start_reg <= 0;
			status_reg <= 0;
			count <= 0;
			done <= 0;
			enable <= 0;
			avs_s1_waitrequest <= 0;
		end
		else if ((avs_s1_waitrequest == 0) && (avs_s1_write == 1)) begin
			if (avs_s1_address == 0)
				op1_reg <= avs_s1_writedata;
			else if (avs_s1_address == 1)
				op2_reg <= avs_s1_writedata; 
			else if (avs_s1_address == 2) begin
				start_reg <= 1; 
				avs_s1_waitrequest <= 1;
				input1_wire <= op1_reg;
				input2_wire <= op2_reg;
				enable <= 1;
			end
		end 
	end
	
  	always_ff @ (posedge clk) begin
		if (start_reg == 0)
			count <= 4'b0000;		
		else if ((avs_s1_waitrequest == 1) && (count != 4'b1011))
			count <= count + 4'b0001;
		else if (count == 4'b1011) begin //when it counts 11 times (goes from 0 to 11, technically counting 12 times)
			start_reg <= 0;
			avs_s1_waitrequest <= 0;
			enable <= 0;
			count <= 4'b0000;
			result_reg <= result_wire;
			if (overflow == 1) 
				status_reg <= 3'b001;
			else if (underflow == 1) 
				status_reg <= 3'b010;
			else if (zero == 1)
				status_reg <= 3'b011;
			else if (nan == 1)
				status_reg <= 3'b100;
			else
				status_reg <= 3'b000;	
		end
	end


	/* 4. Now write the code for a read operation. 
		  This should be much simpler than write as 
		  you should just need a single mux. 
    */
	always_ff @ (posedge clk or posedge reset) begin
		if ((avs_s1_waitrequest == 0) && (avs_s1_read == 1)) begin	
			if (avs_s1_address == 3) //read status 
				avs_s1_readdata <= result_reg;
			else if (avs_s1_address == 4)
				avs_s1_readdata <= {29'b0, status_reg};
		end
	end
	/* 5. It is best to deal with the single cycle case first and then
	change your design to work with waitrequest. 
	*/

endmodule
