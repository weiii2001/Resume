`timescale 1ns/1ns
module mux4( out, op, e0, e1, e2, e3 );

    input e0, e1, e2, e3;
	input[1:0] op;
    output out;
	
    assign  out = op[1] ? ( op[0] ? e3 : e2 ) : ( op[0] ? e1 : e0 );
	
endmodule