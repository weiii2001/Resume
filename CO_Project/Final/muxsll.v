`timescale 1ns/1ns
module muxsll  ( x, select, a0, a1 );
input [31:0] a0 ;
input [31:0] a1 ;
input select ;
output [31:0] x ;


    assign x = select ? a1 : a0;

endmodule