`timescale 1ns/1ns
module part1_tb();

logic [31:0] X, Y, result;
logic zero, underflow, overflow, nan;
logic [7:0] exponent_x;
logic [7:0] exponent_y;
logic [8:0] exponent_with_127;
logic [23:0] mantissa_x_hidden;
logic [23:0] mantissa_y_hidden;
logic [47:0] mantissa;
logic [22:0] mantissa_truncated;
logic [1:0] mantissa_hidden;
logic [22:0] mantissa_final;
logic [7:0] exponent_final;
logic [22:0] mantissa_x;
logic [22:0] mantissa_y;

part1 DUT(X, Y, result, exponent_x, exponent_y, exponent_with_127, mantissa_x_hidden, mantissa_y_hidden, mantissa, mantissa_truncated, mantissa_hidden, mantissa_final, exponent_final,
mantissa_x, mantissa_y, zero, underflow, overflow, nan);

initial begin

	zero = 0;
	underflow = 0;
	overflow = 0;
	nan = 0;
	
	#5;

	X = 32'b00000000000000000000000000000000;
	Y = 32'b00000000000000000000000000000000;
	
	
	#5;
	
	zero = 0;
	underflow = 0;
	overflow = 0;
	nan = 0;
	
	#5;
	
	X = 32'b1100_0001_1001_0000_0000_0000_0000_0000;
	Y = 32'b0100_0001_0001_1000_0000_0000_0000_0000;
	
	// Result = 1100_0011_0010_1011_0000_0000_0000_0000
	
	#5;
	
	zero = 0;
	underflow = 0;
	overflow = 0;
	nan = 0;
	
	#5;
	
	//Zero case
	X = 32'b1000_0000_0000_0000_0000_0000_0000_0000; // E = 0, M = 0
	Y = 32'b0100_0001_0001_1111_0110_0000_0000_0000;

	#5;
	
	zero = 0;
	underflow = 0;
	overflow = 0;
	nan = 0;
	
	#5;
	
	//Underflow case
	X = 32'b1000_1111_0000_0000_1111_1111_0000_0000; //E = 30
	Y = 32'b0001_0100_0001_1000_0000_0000_1111_1111; //E = 40

	#5;
	
	zero = 0;
	underflow = 0;
	overflow = 0;
	nan = 0;
	
	#5;
	
	//Not-a-Number case
	X = 32'b1000_0000_0000_0000_0000_0000_0000_0000; // Zero
	Y = 32'b0111_1111_1000_0000_0000_0000_0000_0000; // Infinity
	
	#5;
	
	zero = 0;
	underflow = 0;
	overflow = 0;
	nan = 0;
	
	#5;
	
	//Overflow case
	X = 32'b1111_1111_1000_0000_1111_1111_0000_0000; //E = 256
	Y = 32'b0111_1111_1001_1000_0000_0000_1111_1111; //E = 256	
	
	#5;
	$stop();
	
end

endmodule
