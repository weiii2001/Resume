`timescale 1ns/1ns
module Multiplier( clk, dataA, dataB, Multu, dataOut, reset );
	input clk, reset;
	input Multu;
	input [31:0] dataA ;
	input [31:0] dataB ;
	output reg [63:0] dataOut ;

	//   Signal ( 6-bits)?
	//   MULTU  : 25


	reg [63:0] temp ;
	reg [31:0] A ;
	reg [6:0] count ;
	reg state;
	
	always@( posedge clk or reset ) begin
		if ( reset ) begin
			temp = 64'b0 ;
			count = 6'b0;
			dataOut = 64'b0;
		end
		else begin
		
			if ( Multu ) begin
				temp[31:0] = dataB;
				temp[63:32] = 32'b0;
				state = 1'b1;
				A = dataA ;
				count = 6'd0;
			end
		
			if ( state == 1'b1 ) begin
				if ( count < 6'd32 ) begin
					if ( temp[0] == 1'b1 ) begin
						temp[63:32] = temp[63:32] + A ;
					end
					
					temp = temp >> 1;
				end
				else if ( count == 6'd32 ) begin
					dataOut = temp;
					state = 1'b0;
				end
				
				count = count + 1;
			end
		end
	/*
	乘法運算
	OUT的部分是要等control給你指令你才能夠把答案輸出到HILO暫存器
	*/

	end

endmodule