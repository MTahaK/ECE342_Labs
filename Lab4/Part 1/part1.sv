module part1(
	input [31:0] X,
	input [31:0] Y,
	output [31:0] result,
	output zero, underflow, overflow, nan
);

	// 1. XOR the sign bits of X & Y to get the sign of the result.
	
	logic sign;
	assign sign = X[31] ^ Y[31];
	
	// 2. Add the exponents of X & Y, then subtract 127 since we are adding 2*127 to the final number.
	
	logic [7:0] exponent;
	assign exponent  = X[30:23] + Y[30:23] - 127;
	
	// 3. Multiply the (23-bit) mantissas of X & Y . Along with the hidden 1 in each mantissa, this results in a 48-bit product.
	
	logic [47:0] mantissa;
	logic [23:0] mantissa_x;
	logic [23:0] mantissa_y;
	assign mantissa_x = {1'b1, X[22:0]};
	assign mantissa_y = {1'b1, Y[22:0]};
	assign mantissa =  mantissa_x * mantissa_y;
	
	//4. Round the 48-bit product by truncating the least significant 23-bits. You should be left with a 25-bit`rounded value' comprising 2 hidden bits and 23 mantissa bits.

	logic [22:0] mantissa_truncated;
	logic [1:0] mantissa_hidden;
	assign mantissa_hidden = mantissa [47:46];	// assign 2 MSBs to as hidden bits
	assign mantissa_truncated = mantissa [45:23]; // truncate the 23-LSBs	
	
	//5. As the Final mantissa should have a `hidden one', you must first normalize the rounded value. If this 
	//rounded value is greater than 1, shift the value right by 1 bit. Since shifting right by 1 bit is equal to a
	//division by two, this requires the exponent to be increased by 1. In decimal, for example, 23.05 x 10^3 is
	//the same as 2.305 x 10^4.
	
	logic [22:0] mantissa_final;
	logic [7:0] exponent_final;
	
	always_comb 
		begin
			if (mantissa_hidden > 2'b01)
				begin
					logic [24:0] mantissa_shifted;
					mantissa_shifted = {mantissa_hidden, mantissa_truncated} >> 1;	// if hidden MSB is 1 (11, 10), then shift the 25 mantissa bit (including the hidden bit) to right by 1
					mantissa_final = mantissa_shifted [22:0];
					exponent_final = exponent + 1'd1; // increase exponent by 1 decimal point
				end
			else
				begin
					mantissa_final = mantissa_truncated;
					exponent_final = exponent;	
				end
		end
	
	//6. As the 2 most significant bits of the mantissa are now normalized, they must be 01 and can therefore be ignored. The 1 is the hidden one in the final number.
	
	assign result = {sign, exponent_final, mantissa_final};
	
endmodule
	
	 