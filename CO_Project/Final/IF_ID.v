module IF_ID ( clk, rst, en_reg, d_in1, d_in2, d_in3, d_in4, d_out1, d_out2, d_out3, d_out4 );
    input clk, rst, en_reg, d_in3;
	input [6:0]d_in4;
    input [31:0] d_in1, d_in2;
	
	output reg d_out3;
	output reg [6:0] d_out4;
    output reg [31:0] d_out1, d_out2;
	
    always @( posedge clk ) begin
        if ( rst ) begin
			d_out1 <= 32'b0;
			d_out2 <= 32'b0;
			d_out3 <= 1'b0;
			d_out4 <= 7'b0;
		end	
        else if ( en_reg ) begin
			d_out1 <= d_in1;
			d_out2 <= d_in2;
			d_out3 <= d_in3;
			d_out4 <= d_in4;
		end	
    end

endmodule