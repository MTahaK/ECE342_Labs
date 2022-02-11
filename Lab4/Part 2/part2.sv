module part2#(
    parameter E = 8,		  // Number of bits in the exponent
    parameter M = 23, 		  // Number of bits in the Mantissa
    parameter BITS = 1+M+E,	  // Total number of bits in the floating point number.
    parameter EB = 2**(E-1)-1 // Value of the bias, based on the exponent. Bias calculation taken from Wikipedia 
)(
	input [BITS - 1:0] X,
	input [BITS - 1:0] Y,
	output logic [BITS - 1:0] result,
	output logic zero, underflow, overflow, nan

);
	/* Need for X and Y: Sign bits, Exponent bits, Mantissa bits, */
	// trunc_index = 2*M + 1;
	logic [E-1:0] x_exp;
	logic [E-1:0] y_exp;
	logic [M:0] x_man;
	logic [M:0] y_man;
	logic [2*M+1 :0] man_prod;
	logic [M+1:0] man_trunc;
	logic [E:0] prod_exp;
	logic [E:0] orig_exp;
	logic [M:0] man;
	logic [E:0] exp_f;
	logic sign;


	assign x_exp = X[BITS-2:M];
	assign y_exp = Y[BITS-2:M];
	
	assign x_man = {1'b1, X[M-1:0]}; // Concatenate implicit one
	assign y_man = {1'b1, Y[M-1:0]}; // Concatenate implicit one
	
	assign man_prod = x_man * y_man;
	assign man_trunc = man_prod[2*M+1:M];
	assign orig_exp = x_exp + y_exp;
	assign prod_exp = x_exp + y_exp - EB;

	assign sign = X[BITS - 1] ^ Y[BITS - 1];
	assign man = (man_trunc[M+1] == 1'b1) ? man_trunc[M:1] : man_trunc[M-1:0];
	assign exp_f = (man_trunc[M+1] == 1'b1) ? (prod_exp + 1) : (prod_exp);

	// Logic in these operations are not dependent on a clock and do not 
	// require any memory. Therefore, no FFs are featured in this circuit - 
	// logic is purely combinational.

	always_comb begin
		

		// Case 1: Zero
		if(X[BITS-2:0] == 0 || Y[BITS-2:0] == 0 || man_prod == 0 && prod_exp == 0) 
		begin
			result[BITS-1:0] = 'b0;
			zero = 1'b1;
			underflow = 1'b0;
			overflow = 1'b0;
			nan = 1'b0;
		end

		// Case 2: Underflow
		else if(prod_exp < EB) 
		begin
			result[BITS-1:M] = 'b0; // Set all exp bits to 0
			result[M-1:0] = 'b1;    // For consistency, set all m bits to 1
			zero = 1'b0;
			underflow = 1'b1;
			overflow = 1'b0;
			nan = 1'b0;
		end
		
		// Case 3: NaN
		else if(prod_exp == 2*EB+1 && man_trunc != 0) 
		begin
			result[BITS-1] = sign;
			result[BITS-1:M] = EB;
			result[M-1:0] = 'b0;
			zero = 1'b0;
			underflow = 1'b0;
			overflow = 1'b0;
			nan = 1'b1;
		end
		// Case 4: Overflow
		else if(prod_exp >= 2*EB+1) 
		begin
			result[BITS-1] = sign;
			result[BITS-1:M] = EB;
			result[M-1:0] = 'b0;
			zero = 1'b0;
			underflow = 1'b0;
			overflow = 1'b1;
			nan = 1'b0;
		end

		// Continues here if no special cases apply
		else begin
			result[M-1:0] = man;
			result[BITS-2:M] = exp_f;
			result[BITS-1] = sign;
			zero = 1'b0;
			underflow = 1'b0;
			overflow = 1'b0;
			nan = 1'b0;
		end
	end
endmodule