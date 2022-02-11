module part2#(
    parameter E = 8,		  // Number of bits in the exponent
    parameter M = 23, 		  // Number of bits in the Mantissa
    parameter BITS =1+M+E,	  // Total number of bits in the floating point number.
    parameter EB = 2**(E-1)-1 // Value of the bias, based on the exponent. Bias calculation taken from Wikipedia 
)(
	input [BITS - 1:0] X,
	input [BITS - 1:0] Y,
	output [BITS - 1:0] result
	output zero, underflow, overflow, nan

);
	/* Need for X and Y: Sign bits, Exponent bits, Mantissa bits, */
	// trunc_index = 2*M + 1;
	logic [E-1:0] x_exp;
	logic [E-1:0] y_exp;
	logic [M:0] x_man;
	logic [M:0] y_man;
	logic [M:0] mantissa;
	logic [2*M+1] man_prod;
	logic [M+1] man_trunc
	logic sign;
	logic [E:0] prod_exp


	assign x_exp = X[BITS-2:M] 
	assign y_exp = Y[BITS-2:M]
	
	assign x_man = {1'b1, X[M-1:0]}; // Concatenate implicit one
	assign y_man = {1'b1, Y[M-1:0]}; // Concatenate implicit one
	
	assign man_prod = x_man * y_man;
	assign man_trunc = man_prod[2*M+1:M];

	assign sign = X[BITS-1] ^ Y[BITS-1];
	
	assign prod_exp = x_exp + y_exp - EB;
	

	// Logic in these operations are not dependent on a clock and do not 
	// require any memory. Therefore, no FFs are featured in this circuit - 
	// logic is purely combinational.

	always_comb begin
		
	
	end

	





endmodule