module wallace_mult #(width=8) (
    input [7:0] x,
    input [7:0] y,
    output [15:0] out  	
);

// Wires corresponding to each level of the WTM. 
wire [15:0] s_lev[0:3][1:2];
wire [15:0] c_lev[0:3][1:2];

// Rest of your code goes here. 

//i0-i7 hard coded


logic [15:0] i0, i1, i2, i3, i4, i5, i6, i7;
logic cout_final;
assign i0 = {8'b0, x & {8{y[0]}}};
assign i1 = {7'b0, x & {8{y[1]}}, 1'b0};
assign i2 = {6'b0, x & {8{y[2]}}, 2'b0};
assign i3 = {5'b0, x & {8{y[3]}}, 3'b0};
assign i4 = {4'b0, x & {8{y[4]}}, 4'b0};
assign i5 = {3'b0, x & {8{y[5]}}, 5'b0};
assign i6 = {2'b0, x & {8{y[6]}}, 6'b0};
assign i7 = {1'b0, x & {8{y[7]}}, 7'b0};

CSA #(.width(16)) CSA1(.x(i0), .y(i1), .cin(i2), .sum(s_lev[0][1]), .cout(c_lev[0][1]));
CSA #(.width(16)) CSA2(.x(i3), .y(i4), .cin(i5), .sum(s_lev[0][2]), .cout(c_lev[0][2]));

CSA #(.width(16)) CSA3(.x(s_lev[0][1]), .y(c_lev[0][1] << 1), .cin(s_lev[0][2]), .sum(s_lev[1][1]), .cout(c_lev[1][1]));
CSA #(.width(16)) CSA4(.x(c_lev[0][2] << 1), .y(i6), .cin(i7), .sum(s_lev[1][2]), .cout(c_lev[1][2]));

CSA #(.width(16)) CSA5(.x(c_lev[1][1] << 1), .y(s_lev[1][1]), .cin(s_lev[1][2]), .sum(s_lev[2][1]), .cout(c_lev[2][1]));
  

CSA #(.width(16)) CSA6(.x(s_lev[2][1]), .y(c_lev[2][1] << 1), .cin(c_lev[1][2] << 1), .sum(s_lev[3][1]), .cout(c_lev[3][1]));


RCA #(.width(16)) RCA(.x(s_lev[3][1]), .y(c_lev[3][1] << 1), .cin(1'b0), .sum(out), .cout(cout_final));


endmodule


module full_adder(a, b, cin, s, cout);
	input logic a, b, cin;
	output logic s, cout;
	assign s = cin^a^b;
	assign cout = (a&b)|(cin&a)|(cin&b);  
endmodule

module RCA #(width = 16)
(
	input logic [width-1:0] x,
	input logic [width-1:0] y,
	input logic cin,
	output logic [width-1:0] sum,
	output logic cout

);
	
	logic [width:0] temp;
	assign temp[0] = cin;
	assign cout = temp[width];
	
	genvar i;
	generate
		for  (i = 0; i < width; i++) begin : ripple_carry
			full_adder fa_inst ( .a(x[i]), .b(y[i]), .cin(temp[i]), .s(sum[i]), .cout(temp[i+1]) );
		end
	endgenerate
endmodule

module CSA #(width = 16) 
(
	input [width-1:0] x,
	input [width-1:0] y,
	input [width-1:0] cin,
	output [width-1:0] sum,
	output [width-1:0] cout
);

	genvar i;
	generate
	for(i=0; i < width; i++) 
		begin: carry_save
		full_adder f_adder
		(
			.a(x[i]),
			.b (y[i]),
			.cin(cin[i]),
			.cout(cout[i]),
			.s(sum[i])
		);
		end
	endgenerate	
endmodule
