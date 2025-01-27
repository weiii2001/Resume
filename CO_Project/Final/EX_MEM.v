module EX_MEM ( clk, rst, en_reg, d_in1, d_in2, d_in3, d_in4, d_in5, d_in6, d_in7, d_in8, d_in9, d_in10, d_in11, d_in12,
						   d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7, d_out8, d_out9, d_out10, d_out11, d_out12);
    input clk, rst, en_reg;
    input d_in1, d_in3, d_in4, d_in5, d_in6, d_in8;
	input [4:0] d_in11;
    input [1:0] d_in2;
    input[31:0]	d_in7, d_in9, d_in10, d_in12;
    output reg d_out1, d_out3, d_out4, d_out5, d_out6, d_out8; 
    output reg [1:0] d_out2;
	output reg[4:0] d_out11;
    output reg[31:0] d_out7, d_out9, d_out10, d_out12;

	
    always @( posedge clk ) begin
        if ( rst ) begin
			d_out1 <= 1'b0;
			d_out2 <= 1'b0;
			d_out3 <= 1'b0;
			d_out4 <= 1'b0;
			d_out5 <= 1'b0;
			d_out6 <= 1'b0;
			d_out7 <= 32'b0;
			d_out8 <= 1'b0;
			d_out9 <= 32'b0;
			d_out10 <= 32'b0;
			d_out11 <= 5'b0;
			d_out12 <= 32'b0;
		end	
        else if ( en_reg ) begin
			d_out1 <= d_in1;
			d_out2 <= d_in2;
			d_out3 <= d_in3;
			d_out4 <= d_in4;
			d_out5 <= d_in5;
			d_out6 <= d_in6;
			d_out7 <= d_in7;
			d_out8 <= d_in8;
			d_out9 <= d_in9;
			d_out10 <= d_in10;
			d_out11 <= d_in11;
			d_out12 <= d_in12;
		end	
    end

endmodule