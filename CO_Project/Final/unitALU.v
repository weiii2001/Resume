module unitALU( sum, cout, set, a, b, cin, less, sel, inv );
    input a, b, cin, less, inv;
    input [1:0] sel;
    output cout, sum, set;
	
    wire e0, e1, e2, e3, xb;
    
    xor( xb, b, inv );
    and( e0, a, b );
    or( e1, a, b );
    FA fa( .sum(e2), .cout(cout), .a(a), .b(xb), .c(cin) );
	assign set = e2;
    assign e3 = less;
	mux4 muxalu( .out(sum), .op(sel), .e0(e0), .e1(e1), .e2(e2), .e3(e3) );
	
endmodule