`timescale 1ns/1ns
module MUX( ALUOut, HiOut, LoOut, Shifter, Signal, dataOut );
input [31:0] ALUOut ;
input [31:0] HiOut ;
input [31:0] LoOut ;
input [31:0] Shifter ;
input [5:0] Signal ;
output [31:0] dataOut ;

wire [31:0] temp ;

parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;

parameter SLL = 6'b000000;

parameter MULTU = 6'b011001;

parameter MFHI= 6'b010000;
parameter MFLO= 6'b010010;

/*
�w�q�U�ذT��
*/

assign temp = ( Signal == AND )? ALUOut:( Signal == OR )? ALUOut:( Signal == ADD )? ALUOut:( Signal == SUB )? ALUOut:( Signal == SLT )?
	   ALUOut:( Signal == MFHI )? HiOut:( Signal == MFLO )? LoOut:( Signal == SLL )? Shifter:32'b0;
	
/*===========
   ���n
===========
MFHI�MMFLO
�o��D���O�@�w�n�[�i��
�o�O��HILO�Ȧs�����ȿ�X�����O
�ЧO�ѰO
===========
   ���n
===========*/

assign dataOut = temp ;

endmodule