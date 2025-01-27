module nop_detector( clk, rst, instr, en_pc, NOPSrc, maxcount, instr2 );
	input clk, rst;
	input [31:0] instr;
	output reg en_pc, NOPSrc;
	output reg [6:0] maxcount;
	output reg [31:0] instr2;
	
	reg [6:0] counter;
	
	always @ ( instr ) begin
		
        if ( instr[31:26] == 6'd0 && instr[5:0] == 6'd25 ) begin // multu
            maxcount <= 7'd32;
	        en_pc <= 0;
			instr2 <= instr;
        end
        else if ( instr[31:26] == 6'd4 ) begin // beq
            maxcount <= 7'd1;
	        en_pc <= 1'b0;
		    instr2 <= instr;
        end
        else if ( instr[31:26] == 6'd2 ) begin // j
            maxcount <= 7'd1;
	        en_pc <= 1'b0;
		    instr2 <= instr;
        end
		else if ( instr[31:26] == 6'd3 ) begin // jal
            maxcount <= 7'd1;
	        en_pc <= 1'b0;
		    instr2 <= instr;
        end
        else begin
		  maxcount <= 7'd0;	
	      en_pc <= 1'b1;
		  instr2 <= instr;
        end		
	end
	
	always @ ( posedge clk ) begin
	    if ( rst ) begin
	        counter <= 7'd0;
	   	    maxcount <= 7'd0;
			en_pc <= 1;
			NOPSrc <= 0;
			instr2 <= 32'b0;
	    end
	    else begin
		    if ( counter == 7'd0 && en_pc == 1'b1 ) NOPSrc <= 0;
			else NOPSrc <= 1;
			
	        counter <= counter + 1;
	        if ( counter == maxcount ) begin
	          en_pc <= 1;
		      counter <= 0;
			  maxcount <= 0;
			  NOPSrc <= 0;
	        end	
	    end	
	end


endmodule