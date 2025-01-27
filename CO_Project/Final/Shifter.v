`timescale 1ns/1ns
module Shifter( dataA, dataB, dataOut);
input [31:0] dataA ; // 要位移的數
input [31:0] dataB ; // 要位移的量
output [31:0] dataOut ;

wire [31:0] s1, s2, s3, s4 ;


		// Shift by either 0 or 1 bits.
	    
	    muxsll shift0( s1, dataB[0:0], dataA, {dataA[30:0], 1'b0} );

	    // Shift by either 0 or 2 bits.
	    
	    muxsll shift1( s2, dataB[1:1], s1, {s1[29:0], 2'b0} );

	    // Shift by either 0 or 4 bits.
	    
	    muxsll shift2( s3, dataB[2:2], s2, {s2[27:0], 4'b0} );

	    // Shift by either 0 or 8 bits.
	    
	    muxsll shift3( s4, dataB[3:3], s3, {s3[23:0], 8'b0} );	
		
	    // Shift by either 0 or 16 bits.
	    
	    muxsll shift4( dataOut, dataB[4:4], s4, {s4[15:0], 16'b0} );			

endmodule