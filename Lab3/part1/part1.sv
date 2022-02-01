module mult_csm
(
	input [7:0] x,
	input [7:0] y,
	output [15:0] out
);
	logic [15:0] pp[9];
	assign pp[0] = '0;
	
	logic [16:0] cin[9];
	assign cin[0] = '0;

	// Write your nested generate for loops here.
	
	

	// Have a RCA to add the numbers in columns 8 through 15 of 
	// the final row of the multiplier to get out[16:8].
	
	
	
   	// Set the lower 8-bits of the final multiplier output. 
	assign out[7:0] = pp[8][7:0];		
		  
endmodule

// Full adder cell
module fa
(
	input x, y, cin,
	output s, cout
);
	assign s = x ^ y ^ cin;
	assign cout = x&y | x&cin | y&cin;
endmodule
