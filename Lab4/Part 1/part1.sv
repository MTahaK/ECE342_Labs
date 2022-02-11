module part1(
	input [31:0] X,
	input [31:0] Y,
	output [31:0] result,
	output logic [7:0] exponent_x,
	output logic [7:0] exponent_y,
	output logic [8:0] exponent_with_127,
	output logic [23:0] mantissa_x_hidden,
	output logic [23:0] mantissa_y_hidden,
	output logic [47:0] mantissa,
	output logic [22:0] mantissa_truncated,
	output logic [1:0] mantissa_hidden,
	output logic [22:0] mantissa_final,
	output logic [8:0] exponent_final,
	output logic [22:0] mantissa_x,
	output logic [22:0] mantissa_y,
	output logic zero, 
	output logic underflow, 
	output logic overflow, 
	output logic nan
);

	// 1. XOR the sign bits of X & Y to get the sign of the result.
	
	logic sign;
	assign sign = X[31] ^ Y[31];
	
	// 2. Add the exponents of X & Y, then subtract 127 since we are adding 2*127 to the final number.
	
	//logic [8:0] exponent;	// make the exponent 9bit to check for special cases: for overflow
	//logic [7:0] exponent_x;	// exponent_x and exponent_y created for special cases check
	//logic [7:0] exponent_y; 
	assign exponent_x = X[30:23];
	assign exponent_y = Y[30:23];
	//assign exponent  = exponent_x + exponent_y - 127;
	assign exponent_with_127 = exponent_x + exponent_y;
	
	// 3. Multiply the (23-bit) mantissas of X & Y . Along with the hidden 1 in each mantissa, this results in a 48-bit product.
	
	//logic [47:0] mantissa;
	//logic [23:0] mantissa_x_hidden; //24 bit mantissa value + 1bit hidden number
	//logic [23:0] mantissa_y_hidden; //24 bit mantissa value + 1bit hidden number
	//logic [22:0] mantissa_x;
	//logic [22:0] mantissa_y;
	assign mantissa_x = X[22:0];
	assign mantissa_y = Y[22:0];
	assign mantissa_x_hidden = {1'b1, X[22:0]}; 
	assign mantissa_y_hidden = {1'b1, Y[22:0]};
	assign mantissa =  mantissa_x_hidden * mantissa_y_hidden;
	
	//4. Round the 48-bit product by truncating the least significant 23-bits. You should be left with a 25-bit`rounded value' comprising 2 hidden bits and 23 mantissa bits.

	//logic [22:0] mantissa_truncated;
	//logic [1:0] mantissa_hidden;
	assign mantissa_hidden = mantissa [47:46];	// assign 2 MSBs to as hidden bits
	assign mantissa_truncated = mantissa [45:23]; // truncate the 23-LSBs	
	
	//5. As the Final mantissa should have a `hidden one', you must first normalize the rounded value. If this 
	//rounded value is greater than 1, shift the value right by 1 bit. Since shifting right by 1 bit is equal to a
	//division by two, this requires the exponent to be increased by 1. In decimal, for example, 23.05 x 10^3 is
	//the same as 2.305 x 10^4.
	
	//logic [22:0] mantissa_final;
	//logic [8:0] exponent_final; //also for the special cases check

	always_comb 
		begin	
			mantissa_final = mantissa_truncated;
			if (mantissa_hidden > 2'b01)
				begin
					logic [24:0] mantissa_shifted;
					mantissa_shifted = {mantissa_hidden, mantissa_truncated} >> 1;	// if hidden MSB is 1 (11, 10), then shift the 25 mantissa bit (including the hidden bit) to right by 1
					mantissa_final = mantissa_shifted [22:0];
					exponent_final = exponent_with_127 + 1'd1; // increase exponent by 1 decimal point
				end			
			//if (((exponent_x + exponent_y) >= 127) && ((exponent_x + exponent_y) < 382)) //normal cases
				//exponent_final = exponent_with_127 - 127;
			//Special Cases
			//Check both Input and Output for this case: X(mantissa_x, exponent_x), Y(mantissa_y, exponent_y), Final(mantissa_final, exponent_final)
			
			//Zero case
			if (((mantissa_x == 0) && (exponent_x == 0)) || ((mantissa_y == 0) && (exponent_y == 0)) || ((exponent_with_127 == 127) && (mantissa_final == 0)))
			//if ((exponent_with_127 == 127) & (mantissa_final == 0))
				begin
					exponent_final = 0;
					mantissa_final = 0;
					zero = 1'b1;		
				end
			//Underflow case
			else if (exponent_with_127 < 127)
				begin
					exponent_final = 0;
					mantissa_final = 23'b111_1111_1111_1111_1111_1111;  //set all mantissa bits to 1 just to differenciate with zero case
					underflow = 1'b1;
				end
			//Not a number case
			else if ((exponent_with_127 == 382) & (mantissa_final != 0))
				begin
					exponent_final = 255;
					mantissa_final = 0;
					nan = 1'b1;
				end
			//Overflow case
			else if (exponent_with_127 >= 382)
				begin
					exponent_final = 255;
					mantissa_final = 0;
					overflow = 1'b1;
				end
			else 
				exponent_final = exponent_with_127 - 127;
		end
	
	//6. As the 2 most significant bits of the mantissa are now normalized, they must be 01 and can therefore be ignored. The 1 is the hidden one in the final number.
	
	assign result = {sign, exponent_final [7:0], mantissa_final};
	
endmodule
	
	 