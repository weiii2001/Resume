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
�w�q�U�ذT��
*/

/*
���k�B��
OUT�������O�n��control���A���O�A�~����⵪�׿�X��HILO�Ȧs��
*/

always@( posedge clk or reset ) begin
    if ( reset ) begin
		temp = 64'b0 ;
		tmp = 64'b0 ;
    end
/*
reset�T�� �p�G�Oreset�N���k0
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
���k�B��
OUT�������O�n��control���A���O�A�~����⵪�׿�X��HILO�Ȧs��
*/

end


assign dataOut = temp ;

endmodule