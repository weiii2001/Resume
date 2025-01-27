//	Title: MIPS Single-Cycle Processor
//	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
module mips_single( clk, rst );
	input clk, rst;
	
	// instruction bus
	wire[31:0] instr, instr_IF, instr_ID, instr_IF2;
	
	// break out important fields from instruction
	wire [5:0] opcode, funct, funct_EX;
    wire [4:0] rs, rt, rd, shamt;
    wire [15:0] immed;
    wire [31:0] extend_immed, b_offset, HiOut, LoOut;
    wire [25:0] jumpoffset;
	wire [63:0] MulAns;
	
	// datapath signals
    wire [4:0] rfile_wn;
    wire [31:0] rfile_rd1, rfile_rd2, rfile_wd, alu_b, alu_out, alu_ans, b_tgt, pc_next,
                pc, pc_incr, dmem_rdata, jump_addr, branch_addr, jump_addr_EX, jump_addr_MEM, muxa_out, muxb_out;

	wire [4:0] rd_EX, rt_EX, rfile_wn_MEM, rfile_wn_EX, rs_EX;
	wire [31:0] pc_ID, pc_EX, pc_MEM, pc_WB, extend_immed_EX, alu_out_MEM, alu_out_WB, dmem_rdata_WB,		
                rfile_rd2_EX, rfile_rd2_MEM, rfile_rd1_EX;
	// control signals
    wire RegWrite, Branch, PCSrc, MemRead, MemWrite, ALUSrc, Zero, Jump,
	     RegWrite_EX, Branch_EX, MemRead_EX, MemWrite_EX, ALUSrc_EX, Jump_EX,
		 RegWrite_MEM, Branch_MEM, MemRead_MEM, MemWrite_MEM, Zero_MEM, Jump_MEM,
		 RegWrite_WB, NOPSrc, en_pc;  
	wire [6:0] maxcount, maxcount_ID, maxcount_EX;
    wire [1:0] ALUOp, ALUOp_EX, RegDst, MemtoReg, RegDst_EX, MemtoReg_EX, MemtoReg_MEM, MemtoReg_WB, ALU_Out_Sel, forA, forB;
    wire [2:0] Operation;
	

// ==================================================================================================== 

    assign opcode = instr_ID[31:26];
    assign rs = instr_ID[25:21];
    assign rt = instr_ID[20:16];
    assign rd = instr_ID[15:11];
    assign shamt = instr_ID[10:6];
    assign funct = instr_ID[5:0];
    assign immed = instr_ID[15:0];
    assign jumpoffset = instr_ID[25:0];
	
// ==================================================================================================== 

	// jump offset shifter & concatenation
	assign jump_addr = { pc_incr[31:28], jumpoffset <<2 };

	
	// TotalALU......

// ==================================================================================================== 

//cycle1=========================================================================================== 
	
	
	mux2 #(32) PCMUX( .sel(PCSrc), .a(pc_incr), .b(b_tgt), .y(branch_addr) );
	
    mux2 #(32) JMUX( .sel(Jump), .a(branch_addr), .b(jump_addr), .y(pc_next) );	
	
	regPC PC( .clk(clk), .rst(rst), .en_reg(en_pc), .d_in(pc_next), .d_out(pc) );
	
	add32 PCADD( .a(pc), .b(32'd4), .result(pc_incr) );
	
	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr_IF) );

	nop_detector NOP( .clk(clk), .rst(rst), .instr(instr_IF), .en_pc(en_pc), .NOPSrc(NOPSrc), .maxcount(maxcount), .instr2(instr_IF2) );
	
	mux2 #(32) NOPMUX( .sel(NOPSrc), .a(instr_IF2), .b(32'd0), .y(instr) );

//end cycle1=========================================================================================== 

	IF_ID C1( .clk(clk), .rst(rst), .en_reg(1'b1), .d_in1(pc_incr), .d_in2(instr), .d_in3(en_pc), .d_in4(maxcount), .d_out1(pc_ID), .d_out2(instr_ID), .d_out3(en_pc_ID), .d_out4(maxcount_ID) );
	
//cycle2=========================================================================================== 
	
	// branch offset shifter
    assign b_offset = extend_immed << 2;
	
    add32 BRADD( .a(pc_ID), .b(b_offset), .result(b_tgt) );
	
	and BR_AND(PCSrc, Branch, Zero);
	
	br_equal equ(.zero(Zero), .RD1(rfile_rd1), .RD2(rfile_rd2) );
	
	// sign-extender
	sign_extend SignExt( .immed_in(immed), .ext_immed_out(extend_immed) );
	
    control_single CTL(.opcode(opcode), .RegDst(RegDst), .ALUSrc(ALUSrc), .MemtoReg(MemtoReg), 
                       .RegWrite(RegWrite), .MemRead(MemRead), .MemWrite(MemWrite), .Branch(Branch), 
                       .Jump(Jump), .ALUOp(ALUOp));

	reg_file RegFile( .clk(clk), .RegWrite(RegWrite_WB), .RN1(rs), .RN2(rt), .WN(rfile_wn), 
					  .WD(rfile_wd), .RD1(rfile_rd1), .RD2(rfile_rd2) );
	
//end cycle2=========================================================================================== 

	ID_EX C2(.clk(clk), .rst(rst), .en_reg(1'b1), .d_in1(RegWrite), .d_in2(MemtoReg), .d_in3(MemRead), .d_in4(MemWrite), .d_in6(ALUSrc), .d_in7(ALUOp), .d_in8(RegDst), .d_in9(en_pc_ID), .d_in10(BNE), .d_in11(Jump),  .d_in12(maxcount_ID), 
             .d_out1(RegWrite_EX), .d_out2(MemtoReg_EX), .d_out3(MemRead_EX), .d_out4(MemWrite_EX), .d_out6(ALUSrc_EX), .d_out7(ALUOp_EX), .d_out8(RegDst_EX), .d_out9(en_pc_EX), .d_out10(BNE_EX), .d_out11(Jump_EX), .d_out12(maxcount_EX),
			 .d_in13(extend_immed), .d_in14(rfile_rd1), .d_in15(rfile_rd2), .d_in16(pc_ID), .d_in17(rd), .d_in18(rt), .d_in19(funct), .d_in20(rs),
             .d_out13(extend_immed_EX), .d_out14(rfile_rd1_EX), .d_out15(rfile_rd2_EX), .d_out16(pc_EX), .d_out17(rd_EX), .d_out18(rt_EX), .d_out19(funct_EX), .d_out20(rs_EX));						

//cycle3=========================================================================================== 
	
	
    ALU ALU(.clk(clk), .reset(rst), .Signal(Operation), .dataA(muxa_out), .dataB(muxb_out), .dataOut(alu_ans) );
	
	mux2 #(32) ALUMUX( .sel(ALUSrc_EX), .a(rfile_rd2_EX), .b(extend_immed_EX), .y(alu_b) );	
	
	mux3 #(5) RFMUX( .sel(RegDst_EX), .a(rt_EX), .b(rd_EX), .c(5'd31), .y(rfile_wn_EX) );

	alu_ctl ALUCTL( .ALUOp(ALUOp_EX), .Funct(funct_EX), .ALUOperation(Operation), .Multu(Multu), .sel(ALU_Out_Sel) );
	
	mux3 #(32) CALMUX( .sel(ALU_Out_Sel), .a(alu_ans), .b(HiOut), .c(LoOut), .y(alu_out) );
	
	mux3 #(32) AMUX( .sel(forA), .a(rfile_rd1_EX), .b(rfile_wd), .c(alu_out_MEM), .y(muxa_out) ); // forA
	
	mux3 #(32) BMUX( .sel(forB), .a(alu_b), .b(rfile_wd), .c(alu_out_MEM), .y(muxb_out) ); // forB
	
	Multiplier Multiplier( .clk(clk), .dataA(muxa_out), .dataB(muxb_out), .dataOut(MulAns), .Multu(Multu), .reset(rst) );
	
	HiLo HiLo( .clk(clk), .MultuAns(MulAns), .HiOut(HiOut), .LoOut(LoOut), .reset(rst));
	
	Forward_unit Forward( .rst(rst), .EX_MEM_rd(rfile_wn_MEM), .MEM_WB_rd(rfile_wn), .RegWrite_MEM(RegWrite_MEM), .RegWrite_WB(RegWrite_WB), .rs(rs_EX), .rt(rt_EX), .forA(forA), .forB(forB), .alu_op(ALUOp_EX));

//end cycle3=========================================================================================== 

	EX_MEM C3(.clk(clk), .rst(rst), .en_reg(1'b1), .d_in1(RegWrite_EX), .d_in2(MemtoReg_EX), .d_in3(MemRead_EX), .d_in4(MemWrite_EX), .d_in6(Jump_EX), .d_in7(pc_EX),
			  .d_out1(RegWrite_MEM), .d_out2(MemtoReg_MEM), .d_out3(MemRead_MEM), .d_out4(MemWrite_MEM), .d_out6(Jump_MEM), .d_out7(pc_MEM),
			  .d_in9(alu_out), .d_in10(rfile_rd2_EX), .d_in11(rfile_wn_EX),
              .d_out9(alu_out_MEM), .d_out10(rfile_rd2_MEM), .d_out11(rfile_wn_MEM) );			  

//cycle4=========================================================================================== 
	
	
	memory DatMem( .clk(clk), .MemRead(MemRead_MEM), .MemWrite(MemWrite_MEM), .wd(rfile_rd2_MEM), .addr(alu_out_MEM), .rd(dmem_rdata) );	   
			
//end cycle4=========================================================================================== 

	MEM_WB C4(.clk(clk), .rst(rst), .en_reg(1'b1), .d_in1(RegWrite_MEM), .d_in2(MemtoReg_MEM), .d_in3(dmem_rdata), .d_in4(alu_out_MEM), .d_in5(rfile_wn_MEM), .d_in6(pc_MEM),
			  .d_out1(RegWrite_WB), .d_out2(MemtoReg_WB), .d_out3(dmem_rdata_WB), .d_out4(alu_out_WB), .d_out5(rfile_wn), .d_out6(pc_WB));
	
//cycle5=========================================================================================== 
	
    mux3 #(32) WRMUX( .sel(MemtoReg_WB), .a(alu_out_WB), .b(dmem_rdata_WB), .c(pc_WB), .y(rfile_wd) );

//end cycle5=========================================================================================== 
				  			
endmodule
