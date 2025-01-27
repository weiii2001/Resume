`timescale 1ns/1ns
module ALU( dataA, dataB, Signal, dataOut, reset );
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Signal ;
output [31:0] dataOut ;

//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SLT  : 42

wire [31:0] temp;
wire [30:0] c; 
wire [1:0] op;
wire cin, inv, less, set;

parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;

/*
定義各種訊號
*/
    
/*
reset訊號 如果是reset就做歸0
*/
	
	assign inv = ( Signal == ADD )?0:1;
	assign cin = ( Signal == ADD )?0:1;
	assign less = 1'b0;
	assign op = Signal[2] ? ( Signal[0] ? 2'b01 : 2'b00 ) : ( Signal[3] ? 2'b11 : 2'b10 );
	unitALU alu0( .sum(temp[0]), .cout(c[0]), .a(dataA[0]), .b(dataB[0]), .cin(cin), .less(set), .sel(op), .inv(inv) );
	unitALU alu1( .sum(temp[1]), .cout(c[1]), .a(dataA[1]), .b(dataB[1]), .cin(c[0]), .less(less), .sel(op), .inv(inv) );
	unitALU alu2( .sum(temp[2]), .cout(c[2]), .a(dataA[2]), .b(dataB[2]), .cin(c[1]), .less(less), .sel(op), .inv(inv) );
	unitALU alu3( .sum(temp[3]), .cout(c[3]), .a(dataA[3]), .b(dataB[3]), .cin(c[2]), .less(less), .sel(op), .inv(inv) );
	unitALU alu4( .sum(temp[4]), .cout(c[4]), .a(dataA[4]), .b(dataB[4]), .cin(c[3]), .less(less), .sel(op), .inv(inv) );
	unitALU alu5( .sum(temp[5]), .cout(c[5]), .a(dataA[5]), .b(dataB[5]), .cin(c[4]), .less(less), .sel(op), .inv(inv) );
	unitALU alu6( .sum(temp[6]), .cout(c[6]), .a(dataA[6]), .b(dataB[6]), .cin(c[5]), .less(less), .sel(op), .inv(inv) );
	unitALU alu7( .sum(temp[7]), .cout(c[7]), .a(dataA[7]), .b(dataB[7]), .cin(c[6]), .less(less), .sel(op), .inv(inv) );
	unitALU alu8( .sum(temp[8]), .cout(c[8]), .a(dataA[8]), .b(dataB[8]), .cin(c[7]), .less(less), .sel(op), .inv(inv) );
	unitALU alu9( .sum(temp[9]), .cout(c[9]), .a(dataA[9]), .b(dataB[9]), .cin(c[8]), .less(less), .sel(op), .inv(inv) );
	unitALU alu10( .sum(temp[10]), .cout(c[10]), .a(dataA[10]), .b(dataB[10]), .cin(c[9]), .less(less), .sel(op), .inv(inv) );
	unitALU alu11( .sum(temp[11]), .cout(c[11]), .a(dataA[11]), .b(dataB[11]), .cin(c[10]), .less(less), .sel(op), .inv(inv) );
	unitALU alu12( .sum(temp[12]), .cout(c[12]), .a(dataA[12]), .b(dataB[12]), .cin(c[11]), .less(less), .sel(op), .inv(inv) );
	unitALU alu13( .sum(temp[13]), .cout(c[13]), .a(dataA[13]), .b(dataB[13]), .cin(c[12]), .less(less), .sel(op), .inv(inv) );
	unitALU alu14( .sum(temp[14]), .cout(c[14]), .a(dataA[14]), .b(dataB[14]), .cin(c[13]), .less(less), .sel(op), .inv(inv) );
	unitALU alu15( .sum(temp[15]), .cout(c[15]), .a(dataA[15]), .b(dataB[15]), .cin(c[14]), .less(less), .sel(op), .inv(inv) );
	unitALU alu16( .sum(temp[16]), .cout(c[16]), .a(dataA[16]), .b(dataB[16]), .cin(c[15]), .less(less), .sel(op), .inv(inv) );
	unitALU alu17( .sum(temp[17]), .cout(c[17]), .a(dataA[17]), .b(dataB[17]), .cin(c[16]), .less(less), .sel(op), .inv(inv) );
	unitALU alu18( .sum(temp[18]), .cout(c[18]), .a(dataA[18]), .b(dataB[18]), .cin(c[17]), .less(less), .sel(op), .inv(inv) );
	unitALU alu19( .sum(temp[19]), .cout(c[19]), .a(dataA[19]), .b(dataB[19]), .cin(c[18]), .less(less), .sel(op), .inv(inv) );
	unitALU alu20( .sum(temp[20]), .cout(c[20]), .a(dataA[20]), .b(dataB[20]), .cin(c[19]), .less(less), .sel(op), .inv(inv) );
	unitALU alu21( .sum(temp[21]), .cout(c[21]), .a(dataA[21]), .b(dataB[21]), .cin(c[20]), .less(less), .sel(op), .inv(inv) );
	unitALU alu22( .sum(temp[22]), .cout(c[22]), .a(dataA[22]), .b(dataB[22]), .cin(c[21]), .less(less), .sel(op), .inv(inv) );
	unitALU alu23( .sum(temp[23]), .cout(c[23]), .a(dataA[23]), .b(dataB[23]), .cin(c[22]), .less(less), .sel(op), .inv(inv) );
	unitALU alu24( .sum(temp[24]), .cout(c[24]), .a(dataA[24]), .b(dataB[24]), .cin(c[23]), .less(less), .sel(op), .inv(inv) );
	unitALU alu25( .sum(temp[25]), .cout(c[25]), .a(dataA[25]), .b(dataB[25]), .cin(c[24]), .less(less), .sel(op), .inv(inv) );
	unitALU alu26( .sum(temp[26]), .cout(c[26]), .a(dataA[26]), .b(dataB[26]), .cin(c[25]), .less(less), .sel(op), .inv(inv) );
	unitALU alu27( .sum(temp[27]), .cout(c[27]), .a(dataA[27]), .b(dataB[27]), .cin(c[26]), .less(less), .sel(op), .inv(inv) );
	unitALU alu28( .sum(temp[28]), .cout(c[28]), .a(dataA[28]), .b(dataB[28]), .cin(c[27]), .less(less), .sel(op), .inv(inv) );
	unitALU alu29( .sum(temp[29]), .cout(c[29]), .a(dataA[29]), .b(dataB[29]), .cin(c[28]), .less(less), .sel(op), .inv(inv) );
	unitALU alu30( .sum(temp[30]), .cout(c[30]), .a(dataA[30]), .b(dataB[30]), .cin(c[29]), .less(less), .sel(op), .inv(inv) );
	unitALU alu31( .sum(temp[31]), .set(set), .a(dataA[31]), .b(dataB[31]), .cin(c[30]), .less(less), .sel(op), .inv(inv) );
		
assign dataOut = reset?32'b0:temp ;

endmodule