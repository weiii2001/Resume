module br_equal ( RD1, RD2, zero );

	input [31:0] RD1, RD2;
	output zero;
	reg zero;

	always @( RD1 or RD2 ) begin  
		if ( RD1 == RD2 ) zero <= 1'b1;
		else zero <= 1'b0;
	end
		
endmodule