`timescale 1ns/1ns
module Multiplier( clk, dataA, dataB, Signal, dataOut, reset );
input clk ;
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Signal ;
output [63:0] dataOut ;

//   Signal ( 6-bits)?
//   MULTU  : 25


reg [63:0] tmp;
reg [63:0] temp ;
parameter MULTU = 6'b011001;
parameter OUT = 6'b111111;
/*
定義各種訊號
*/

/*
乘法運算
OUT的部分是要等control給你指令你才能夠把答案輸出到HILO暫存器
*/

always@( posedge clk or reset ) begin
    if ( reset ) begin
		temp = 64'b0 ;
		tmp = 64'b0 ;
    end
/*
reset訊號 如果是reset就做歸0
*/
    else begin
		case ( Signal )
  		MULTU:
		begin 
		    if ( tmp == 64'b0 )	begin
			    tmp[31:0] = dataB;
				tmp[63:32] = 32'b0;
			end
			if ( tmp[0] == 1 ) begin
				tmp[63:32] = tmp[63:32] + dataA;
			end
			tmp = tmp >> 1;
		end
  		OUT:
		begin
			temp = tmp;
			tmp = 64'b0;
		end
		endcase
    end
/*
乘法運算
OUT的部分是要等control給你指令你才能夠把答案輸出到HILO暫存器
*/

end


assign dataOut = temp ;

endmodule