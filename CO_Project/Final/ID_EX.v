module ID_EX ( clk, rst, en_reg, d_in1, d_in2, d_in3, d_in4, d_in5, d_in6, d_in7, d_in8, d_in9, d_in10, d_in11, d_in12, d_in13, d_in14, d_in15, d_in16, d_in17, d_in18, d_in19, d_in20,
						  d_out1, d_out2, d_out3, d_out4, d_out5, d_out6, d_out7, d_out8, d_out9, d_out10, d_out11, d_out12, d_out13, d_out14, d_out15, d_out16, d_out17, d_out18, d_out19, d_out20);
    input clk, rst, en_reg;
    input d_in1, d_in3, d_in4, d_in5, d_in6, d_in9, d_in10, d_in11;
	input [1:0] d_in7, d_in2, d_in8;
	input [6:0] d_in12;
	input [4:0]	d_in17, d_in18, d_in20;
	input [5:0] d_in19;
	input [31:0] d_in13, d_in14, d_in15, d_in16;
    output reg d_out1, d_out3, d_out4, d_out5, d_out6, d_out9, d_out10, d_out11; 
	output reg [1:0] d_out7, d_out2, d_out8;
	output reg [4:0] d_out17, d_out18, d_out20;
	output reg [5:0] d_out19;
	output reg [6:0] d_out12;
    output reg [31:0] d_out13, d_out14, d_out15, d_out16;
	
    always @( posedge clk ) begin
        if ( rst ) begin
			d_out1 <= 1'b0;
			d_out2 <= 1'b0;
			d_out3 <= 1'b0;
			d_out4 <= 1'b0;
			d_out5 <= 1'b0;
			d_out6 <= 1'b0;
			d_out7 <= 2'b0;
			d_out8 <= 1'b0;
			d_out9 <= 1'b0;
			d_out10 <= 1'b0;
			d_out11 <= 1'b0;
			d_out12 <= 7'd0;
			d_out13 <= 32'b0;
			d_out14 <= 32'b0;
			d_out15 <= 32'b0;
			d_out16 <= 32'b0;
			d_out17 <= 5'b0;
			d_out18 <= 5'b0;
			d_out19 <= 6'b0;
			d_out20 <= 5'b0;
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
			d_out13 <= d_in13;
			d_out14 <= d_in14;
			d_out15 <= d_in15;
			d_out16 <= d_in16;
			d_out17 <= d_in17;
			d_out18 <= d_in18;
			d_out19 <= d_in19;
			d_out20 <= d_in20;
		end	
    end

endmodule