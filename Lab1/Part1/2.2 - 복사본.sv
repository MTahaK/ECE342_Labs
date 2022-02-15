module FA1bit(a, b, cin, s, cout);
	input logic a, b, cin;
	output logic s, cout;
	assign s = cin^a^b;
	assign cout = (a&b)|(cin&a)|(cin&b);  
endmodule