module mult
(
	input [7:0] x,
	input [7:0] y,
	output [15:0] out,
	output [15:0] pp[9]
);
	
	//logic [15:0] pp[9];
	assign pp[0] = '0;
	
	logic [16:0] cin[9];
	assign cin[0] = '0;

	// Write your nested generate for loops here.
	
	logic [15:0] xy[8];
	logic [16:8] cin_end;
	assign cin_end[8] = '0;
	
	genvar row, col;
	generate
		for(row = 0; row < 8; row++) 
			begin	
			for(col = 0; col < 16; col++) 
			begin
				if ((col == row) & (col < 8)) assign cin[col][row] = 1'b0;
				if(col < row) assign xy[row][col] = 1'b0;
				else if (col - row < 8) assign xy[row][col] = x[col - row] & y[row];
				else assign xy[row][col] = 1'b0;
			end
		end
	endgenerate
	
	/*
	genvar i, j;
	generate
		for(i = 1; i < 9; i++)
			begin
			for(j = 0; j < 16; j++)
				begin
				if (j < i - 1) assign pp[i][j] = pp[i-1][j];
    			else if (j >= i + 7) assign pp[i][j] = 0; 
				end
			end
	endgenerate
	*/

	genvar m, n;
	generate
		for(m = 0; m < 8; m++)
			begin
			for(n = m; n < m + 8; n++)
				begin			
				fa fa_inst_row
				(
					.x(pp[m][n]),
					.y(xy[m][n]),
					.cin(cin[m][n]),
					.cout(cin[m+1][n+1]),
					.s(pp[m+1][n])
				);
				end
			end
	endgenerate
	

	// Have a RCA to add the numbers in columns 8 through 15 of 
	// the final row of the multiplier to get out[16:8].
	
	genvar p;
	generate
		for(p = 8; p < 16; p++)
			begin
			fa fa_inst_fast
				(
					.a(pp[8][p]),
					.b(cin[8][p]),
					.cin(cin_end[p]),
					.cout(cin_end[p+1]),
					.s(out[p])
				);
			end
	endgenerate	
	
	
	
	
	
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
