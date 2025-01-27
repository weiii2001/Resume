`timescale 1ns/1ns
module ALUControl( clk, Signal, SignaltoALU, SignaltoSHT, SignaltoMULTU, SignaltoMUX );
input clk ;
input [5:0] Signal ;
output [5:0] SignaltoALU ;
output [5:0] SignaltoSHT ;
output [5:0] SignaltoMULTU ;
output [5:0] SignaltoMUX ;

//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SLT  : 42
//   SLL  : 00
//   MULTU : 25


reg [5:0] temp ;
reg [6:0] counter ;


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
定義各種訊號
*/

always@( Signal )
begin
  if ( Signal == MULTU ) begin
    counter = 0;
  end
/*
如果訊號改變成除法 就把counter歸0
*/
end

always@( posedge clk )
begin
  temp = Signal ;
  if ( Signal == MULTU ) begin
    counter = counter + 1 ;
    if ( counter == 33 ) begin
      temp = 6'b111111 ; // Open HiLo reg for MULTU
      counter = 0 ;
    end
  end
/*
數32個clk然後開啟HiLo暫存器給除法器放值進去
*/
end


assign SignaltoALU = temp ;
assign SignaltoSHT = temp ;
assign SignaltoMULTU = temp ;
assign SignaltoMUX = temp ;


endmodule