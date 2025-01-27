module Forward_unit ( rst, EX_MEM_rd, MEM_WB_rd, RegWrite_MEM, RegWrite_WB, rs, rt, forA, forB, alu_op );
  output reg [1:0] forA, forB;
  input [4:0] rs, rt, EX_MEM_rd, MEM_WB_rd ;
  input [1:0] alu_op;
  input rst, RegWrite_MEM, RegWrite_WB ;
  
  always @( rst or rs or EX_MEM_rd or MEM_WB_rd or RegWrite_MEM or RegWrite_WB ) begin
    if ( rst ) begin
	  forA <= 2'b00;
	end
    else begin 
      forA <= 2'b00;
	  if ( alu_op == 2'b00 ) forA <= 2'b00 ;
	  else if ( RegWrite_MEM && (EX_MEM_rd != 5'd0) && (EX_MEM_rd == rs) ) forA <= 2'b10 ; 
	  else if ( RegWrite_WB && (MEM_WB_rd != 5'd0) && (MEM_WB_rd == rs) ) forA <= 2'b01 ;
	  else forA <= 2'b00 ;
	end
  end
	
  always @( rst or rt or EX_MEM_rd or MEM_WB_rd or RegWrite_MEM or RegWrite_WB) begin
    if ( rst ) begin
	  forB <= 2'b00 ;
	end
    else begin
	  forB <= 2'b00 ;  
	  if ( alu_op == 2'b00 ) forB <= 2'b00 ;
	  else if ( RegWrite_WB && (MEM_WB_rd != 5'd0) && (MEM_WB_rd == rt) ) forB <= 2'b01 ;
	  else if ( RegWrite_MEM && (EX_MEM_rd != 5'd0) && (EX_MEM_rd == rt) ) forB <= 2'b10 ;
      else forB <= 2'b00 ;	  
	end

  end

endmodule
