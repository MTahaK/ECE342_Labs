module FA1bit(a, b, cin, s, cout);
	input logic a, b, cin;
	output logic s, cout;
	assign s = cin^a^b;
	assign cout = (a&b)|(cin&a)|(cin&b);  
endmodule

module RCAgen
(
	input logic [7:0] x,
	input logic [7:0] y,
	output logic [8:0] sum
);
	
	logic [8:0] cin;
	
	assign cin[0] = 1'b0;
	assign sum[8] = cin[8];
	
	genvar i;
	generate
		for  (i = 0; i < 8; i++) begin : adders
			FA1bit fa_inst ( .a(x[i]), .b(y[i]), .cin(cin[i]), .s(sum[i]), .cout(cin[i+1]) );
		end
	endgenerate
	endmodule
	