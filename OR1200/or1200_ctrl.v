//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's Instruction decode                                 ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  Majority of instruction decoding is performed here.         ////
////                                                              ////
////  To Do:                                                      ////
////   - make it smaller and faster                               ////
////                                                              ////
////  Author(s):                                                  ////
////      - Damjan Lampret, lampret@opencores.org                 ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////

// Revision 1.7  2002/09/07 05:42:02  lampret
// Added optional SR[CY]. Added define to enable additional (compare) flag modifiers. Defines are OR1200_IMPL_ADDC_MFD and OR1200_ADDITIONAL_FLAG_MODIFIERS_MFD.
//
// Revision 1.6  2002/03/29 15:16:54  lampret
// Some of the warnings fixed.
//
// Revision 1.5  2002/02/01 19:56:54  lampret
// Fixed combinational loops.
//
// Revision 1.4  2002/01/28 01:15:59  lampret
// Changed 'void' nop-ops instead of insn[0] to use insn[16]. Debug unit stalls the tick timer. Prepared new flag generation for add and and insns. Blocked DC/IC while they are turned off. Fixed I/D MMU SPRs layout except WAYs. TODO: smart IC invalidate, l.j 2 and TLB ways.
//
// Revision 1.3  2002/01/18 14:21:43  lampret
// Fixed 'the NPC single-step fix'.
//
// Revision 1.2  2002/01/14 06:18:22  lampret
// Fixed mem2reg bug in FAST implementation. Updated debug unit to work with new genpc/if.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.14  2001/11/30 18:59:17  simons
// force_dslot_fetch does not work -  allways zero.
//
// Revision 1.13  2001/11/20 18:46:15  simons
// Break point bug fixed
//
// Revision 1.12  2001/11/18 08:36:28  lampret
// For GDB changed single stepping and disabled trap exception.
//
// Revision 1.11  2001/11/13 10:02:21  lampret
// Added 'setpc'. Renamed some signals (except_flushpipe into flushpipe etc)
//
// Revision 1.10  2001/11/12 01:45:40  lampret
// Moved flag bit into SR. Changed RF enable from constant enable to dynamic enable for read ports.
//
// Revision 1.9  2001/11/10 03:43:57  lampret
// Fixed exceptions.
//
// Revision 1.8  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.7  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:36  igorm
// no message
//
// Revision 1.2  2001/08/13 03:36:20  lampret
// Added cfg regs. Moved all defines into one defines.v file. More cleanup.
//
// Revision 1.1  2001/08/09 13:39:33  lampret
// Major clean-up.
//
//

module or1200_ctrl_mfd(
    // Clock and reset
    clk, rst_n,

    // Internal i/f
    id_freeze, ex_freeze, wb_freeze, flushpipe, if_insn, branch_op, branch_taken,
    rf_addra, rf_addrb, rf_rda, rf_rdb, alu_op, mac_op, shrot_op, comp_op, rf_addrw, rfwb_op,
    ex_insn, wb_insn, simm, branch_addrofs, lsu_addrofs, sel_a, sel_b, lsu_op,
    multicycle, spr_addrimm, wbforw_valid, sig_syscall, sig_trap,
    force_dslot_fetch, no_more_dslot, ex_void, id_macrc_op, ex_macrc_op, rfe, except_illegal
);

//
// I/O
//
input                    clk;
input                    rst_n;
input                    id_freeze;
input                    ex_freeze;
input                    wb_freeze;
input                    flushpipe;
input     [31:0]         if_insn;
output    [`OR1200_BRANCHOP_WIDTH_MFD-1:0]        branch_op;
input                                         branch_taken;
output    [`OR1200_REGFILE_ADDR_WIDTH_MFD-1:0]    rf_addrw;
output    [`OR1200_REGFILE_ADDR_WIDTH_MFD-1:0]    rf_addra;
output    [`OR1200_REGFILE_ADDR_WIDTH_MFD-1:0]    rf_addrb;
output                    rf_rda;
output                    rf_rdb;
output    [`OR1200_ALUOP_WIDTH_MFD-1:0]           alu_op;
output    [`OR1200_MACOP_WIDTH_MFD-1:0]           mac_op;
output    [`OR1200_SHROTOP_WIDTH_MFD-1:0]         shrot_op;
output    [`OR1200_RFWBOP_WIDTH_MFD-1:0]          rfwb_op;
output    [31:0]          ex_insn;
output    [31:0]          wb_insn;
output    [31:0]          simm;
output    [31:2]          branch_addrofs;
output    [31:0]          lsu_addrofs;
output    [`OR1200_SEL_WIDTH_MFD-1:0]             sel_a;
output    [`OR1200_SEL_WIDTH_MFD-1:0]             sel_b;
output    [`OR1200_LSUOP_WIDTH_MFD-1:0]           lsu_op;
output    [`OR1200_COMPOP_WIDTH_MFD-1:0]          comp_op;
output    [`OR1200_MULTICYCLE_WIDTH_MFD-1:0]      multicycle;
output    [15:0]          spr_addrimm;
input                     wbforw_valid;
output                    sig_syscall;
output                    sig_trap;
output                    force_dslot_fetch;
output                    no_more_dslot;
output                    ex_void;
output                    id_macrc_op;
output                    ex_macrc_op;
output                    rfe;
output                    except_illegal;

//
// Internal wires and regs
//
reg    [`OR1200_BRANCHOP_WIDTH_MFD-1:0]     pre_branch_op;
reg    [`OR1200_BRANCHOP_WIDTH_MFD-1:0]     branch_op;
reg    [`OR1200_ALUOP_WIDTH_MFD-1:0]        alu_op;
reg    [`OR1200_MACOP_WIDTH_MFD-1:0]        mac_op;
reg    [`OR1200_SHROTOP_WIDTH_MFD-1:0]      shrot_op;
reg    [31:0]             id_insn;
reg    [31:0]             ex_insn;
reg    [31:0]             wb_insn;
reg    [`OR1200_REGFILE_ADDR_WIDTH_MFD-1:0] rf_addrw;
reg    [`OR1200_REGFILE_ADDR_WIDTH_MFD-1:0] wb_rfaddrw;
reg    [`OR1200_RFWBOP_WIDTH_MFD-1:0]       rfwb_op;
reg    [31:0]             lsu_addrofs;
reg    [`OR1200_SEL_WIDTH_MFD-1:0]          sel_a;
reg    [`OR1200_SEL_WIDTH_MFD-1:0]          sel_b;
reg                       sel_imm;
reg    [`OR1200_LSUOP_WIDTH_MFD-1:0]        lsu_op;
reg    [`OR1200_COMPOP_WIDTH_MFD-1:0]       comp_op;
reg    [`OR1200_MULTICYCLE_WIDTH_MFD-1:0]   multicycle;
reg                       imm_signextend;
reg    [15:0]             spr_addrimm;
reg                       sig_syscall;
reg                       sig_trap;
reg                       except_illegal;
reg                       ex_macrc_op;
wire                      id_void;

//
// Register file read addresses
//
assign rf_addra = if_insn[20:16];
assign rf_addrb = if_insn[15:11];
assign rf_rda = if_insn[31];
assign rf_rdb = if_insn[30];

//
// Force fetch of delay slot instruction when jump/branch is preceeded by load/store
// instructions
//
// SIMON
// assign force_dslot_fetch = ((|pre_branch_op) & (|lsu_op));
assign force_dslot_fetch = 1'b0;
assign no_more_dslot = |branch_op & !id_void & branch_taken | (branch_op == `OR1200_BRANCHOP_RFE_MFD);
assign id_void = (id_insn[31:26] == `OR1200_OR32_NOP_MFD) & id_insn[16];
assign ex_void = (ex_insn[31:26] == `OR1200_OR32_NOP_MFD) & ex_insn[16];

//
// Sign/Zero extension of immediates
//
assign simm = (imm_signextend == 1'b1) ? {{16{id_insn[15]}}, id_insn[15:0]} : {{16'b0}, id_insn[15:0]};

//
// Sign extension of branch offset
//
assign branch_addrofs = {{4{ex_insn[25]}}, ex_insn[25:0]};

//
// l.macrc in ID stage
//
assign id_macrc_op = (id_insn[31:26] == `OR1200_OR32_MOVHI_MFD) & id_insn[16];

//
//
//
assign rfe = (pre_branch_op == `OR1200_BRANCHOP_RFE_MFD) | (branch_op == `OR1200_BRANCHOP_RFE_MFD);

//
// Generation of sel_a
//
always @( * )
    if ((id_insn[20:16] == rf_addrw) && rfwb_op[0])
        sel_a = `OR1200_SEL_EX_FORW_MFD;
    else if ((id_insn[20:16] == wb_rfaddrw) && wbforw_valid)
        sel_a = `OR1200_SEL_WB_FORW_MFD;
    else
        sel_a = `OR1200_SEL_RF_MFD;

//
// Generation of sel_b
//
always @( * )
    if (sel_imm)
        sel_b = `OR1200_SEL_IMM_MFD;
    else if ((id_insn[15:11] == rf_addrw) && rfwb_op[0])
        sel_b = `OR1200_SEL_EX_FORW_MFD;
    else if ((id_insn[15:11] == wb_rfaddrw) && wbforw_valid)
        sel_b = `OR1200_SEL_WB_FORW_MFD;
    else
        sel_b = `OR1200_SEL_RF_MFD;

//
// l.macrc in EX stage
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        ex_macrc_op <=  1'b0;
    else if (!ex_freeze & id_freeze | flushpipe)
        ex_macrc_op <=  1'b0;
    else if (!ex_freeze)
        ex_macrc_op <=  id_macrc_op;
end

//
// Decode of spr_addrimm
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        spr_addrimm <=  16'h0000;
    else if (!ex_freeze & id_freeze | flushpipe)
        spr_addrimm <=  16'h0000;
    else if (!ex_freeze) begin
        case (id_insn[31:26])    // synopsys parallel_case
            // l.mfspr
            `OR1200_OR32_MFSPR_MFD: 
                spr_addrimm <=  id_insn[15:0];
            // l.mtspr
            default:
                spr_addrimm <=  {id_insn[25:21], id_insn[10:0]};
        endcase
    end
end

//
// Decode of multicycle
//
always @( * ) begin
  case (id_insn[31:26])        // synopsys parallel_case
`ifdef UNUSED
    // l.lwz
    `OR1200_OR32_LWZ_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.lbz
    `OR1200_OR32_LBZ_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.lbs
    `OR1200_OR32_LBS_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.lhz
    `OR1200_OR32_LHZ_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.lhs
    `OR1200_OR32_LHS_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.sw
    `OR1200_OR32_SW_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.sb
    `OR1200_OR32_SB_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
    
    // l.sh
    `OR1200_OR32_SH_MFD:
      multicycle = `OR1200_TWO_CYCLES_MFD;
`endif    
    // ALU instructions except the one with immediate
    `OR1200_OR32_ALU_MFD:
      multicycle = id_insn[`OR1200_ALUMCYC_POS_MFD];
    
    // Single cycle instructions
    default: begin
      multicycle = `OR1200_ONE_CYCLE_MFD;
    end
    
  endcase
  
end

//
// Decode of imm_signextend
//
always @( * ) begin
  case (id_insn[31:26])        // synopsys parallel_case

    // l.addi
    `OR1200_OR32_ADDI_MFD:
        imm_signextend = 1'b1;

    // l.addic
    `OR1200_OR32_ADDI_MFDC_MFD:
        imm_signextend = 1'b1;

    // l.xori
    `OR1200_OR32_XORI_MFD:
        imm_signextend = 1'b1;

    // l.muli
    `OR1200_OR32_MULI_MFD:
        imm_signextend = 1'b1;

    // l.maci
    `OR1200_OR32_MACI_MFD:
        imm_signextend = 1'b1;

    // SFXX insns with immediate
    `OR1200_OR32_SFXX_MFDI_MFD:
        imm_signextend = 1'b1;

    // Instructions with no or zero extended immediate
    default: begin
        imm_signextend = 1'b0;
    end

endcase

end

//
// LSU addr offset
//
always @( * ) begin
    lsu_addrofs[10:0] = ex_insn[10:0];
    case(lsu_op)    // synopsys parallel_case
        `OR1200_LSUOP_SW_MFD, `OR1200_LSUOP_SH_MFD, `OR1200_LSUOP_SB_MFD : 
            lsu_addrofs[31:11] = {{16{ex_insn[25]}}, ex_insn[25:21]};
        default : 
            lsu_addrofs[31:11] = {{16{ex_insn[15]}}, ex_insn[15:11]};
    endcase
end

//
// Register file write address
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        rf_addrw <=  5'd0;
    else if (!ex_freeze & id_freeze)
        rf_addrw <=  5'd00;
    else if (!ex_freeze)
        case (pre_branch_op)    // synopsys parallel_case
            `OR1200_BRANCHOP_J_MFDR_MFD, `OR1200_BRANCHOP_BAL_MFD:
                rf_addrw <=  5'd09;    // link register r9
            default:
                rf_addrw <=  id_insn[25:21];
        endcase
end

//
// rf_addrw in wb stage (used in forwarding logic)
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        wb_rfaddrw <=  5'd0;
    else if (!wb_freeze)
        wb_rfaddrw <=  rf_addrw;
end

//
// Instruction latch in id_insn
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        id_insn <=  {`OR1200_OR32_NOP_MFD, 26'h041_0000};
        else if (flushpipe)
                id_insn <=  {`OR1200_OR32_NOP_MFD, 26'h041_0000};        // id_insn[16] must be 1
    else if (!id_freeze) begin
        id_insn <=  if_insn;
    end
end

//
// Instruction latch in ex_insn
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        ex_insn <=  {`OR1200_OR32_NOP_MFD, 26'h041_0000};
    else if (!ex_freeze & id_freeze | flushpipe)
        ex_insn <=  {`OR1200_OR32_NOP_MFD, 26'h041_0000};    // ex_insn[16] must be 1
    else if (!ex_freeze) begin
        ex_insn <=  id_insn;
    end
end

//
// Instruction latch in wb_insn
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        wb_insn <=  {`OR1200_OR32_NOP_MFD, 26'h041_0000};
    else if (flushpipe)
        wb_insn <=  {`OR1200_OR32_NOP_MFD, 26'h041_0000};    // wb_insn[16] must be 1
    else if (!wb_freeze) begin
        wb_insn <=  ex_insn;
    end
end

//
// Decode of sel_imm
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        sel_imm <=  1'b0;
    else if (!id_freeze) begin
        case (if_insn[31:26])        // synopsys parallel_case

            // j.jalr
            `OR1200_OR32_J_MFDAL_MFDR_MFD:
                sel_imm <=  1'b0;
            
            // l.jr
            `OR1200_OR32_J_MFDR_MFD:
                sel_imm <=  1'b0;
            
            // l.rfe
            `OR1200_OR32_RFE_MFD:
                sel_imm <=  1'b0;
            
            // l.mfspr
            `OR1200_OR32_MFSPR_MFD:
                sel_imm <=  1'b0;
            
            // l.mtspr
            `OR1200_OR32_MTSPR_MFD:
                sel_imm <=  1'b0;
            
            // l.sys, l.brk and all three sync insns
            `OR1200_OR32_XSYNC_MFD:
                sel_imm <=  1'b0;
            
            // l.mac/l.msb
            `OR1200_OR32_MACMSB_MFD:
                sel_imm <=  1'b0;

            // l.sw
            `OR1200_OR32_SW_MFD:
                sel_imm <=  1'b0;
            
            // l.sb
            `OR1200_OR32_SB_MFD:
                sel_imm <=  1'b0;
            
            // l.sh
            `OR1200_OR32_SH_MFD:
                sel_imm <=  1'b0;
            
            // ALU instructions except the one with immediate
            `OR1200_OR32_ALU_MFD:
                sel_imm <=  1'b0;
            
            // SFXX instructions
            `OR1200_OR32_SFXX_MFD:
                sel_imm <=  1'b0;
            
            // l.nop
            `OR1200_OR32_NOP_MFD:
                sel_imm <=  1'b0;

            // All instructions with immediates
            default: begin
                sel_imm <=  1'b1;
            end
          
        endcase
    end
end

//
// Decode of except_illegal
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        except_illegal <=  1'b0;
    else if (!ex_freeze & id_freeze | flushpipe)
        except_illegal <=  1'b0;
    else if (!ex_freeze) begin
        case (id_insn[31:26])        // synopsys parallel_case

            `OR1200_OR32_J_MFD,
            `OR1200_OR32_J_MFDAL_MFD,
            `OR1200_OR32_J_MFDAL_MFDR_MFD,
            `OR1200_OR32_J_MFDR_MFD,
            `OR1200_OR32_BNF_MFD,
            `OR1200_OR32_BF_MFD,
            `OR1200_OR32_RFE_MFD,
            `OR1200_OR32_MOVHI_MFD,
            `OR1200_OR32_MFSPR_MFD,
            `OR1200_OR32_XSYNC_MFD,
            `OR1200_OR32_MACI_MFD,
            `OR1200_OR32_LWZ_MFD,
            `OR1200_OR32_LBZ_MFD,
            `OR1200_OR32_LBS_MFD,
            `OR1200_OR32_LHZ_MFD,
            `OR1200_OR32_LHS_MFD,
            `OR1200_OR32_ADDI_MFD,
            `OR1200_OR32_ADDI_MFDC_MFD,
            `OR1200_OR32_ANDI_MFD,
            `OR1200_OR32_ORI_MFD,
            `OR1200_OR32_XORI_MFD,
            `OR1200_OR32_MULI_MFD,
            `OR1200_OR32_SH_MFD_ROTI_MFD,
            `OR1200_OR32_SFXX_MFDI_MFD,
            `OR1200_OR32_MTSPR_MFD,
            `OR1200_OR32_MACMSB_MFD,
            `OR1200_OR32_SW_MFD,
            `OR1200_OR32_SB_MFD,
            `OR1200_OR32_SH_MFD,
            `OR1200_OR32_ALU_MFD,
            `OR1200_OR32_SFXX_MFD,
            `OR1200_OR32_NOP_MFD:
                except_illegal <=  1'b0;

            // Illegal and OR1200 unsupported instructions
            default:
                except_illegal <=  1'b1;
      endcase
    end
end

//
// Decode of alu_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        alu_op <=  `OR1200_ALUOP_NOP_MFD;
    else if (!ex_freeze & id_freeze | flushpipe)
        alu_op <=  `OR1200_ALUOP_NOP_MFD;
    else if (!ex_freeze) begin
        case (id_insn[31:26])        // synopsys parallel_case
          
            // l.j
            `OR1200_OR32_J_MFD:
                alu_op <=  `OR1200_ALUOP_IMM_MFD;
            
            // j.jal
            `OR1200_OR32_J_MFDAL_MFD:
                alu_op <=  `OR1200_ALUOP_IMM_MFD;
            
            // l.bnf
            `OR1200_OR32_BNF_MFD:
                alu_op <=  `OR1200_ALUOP_NOP_MFD;
            
            // l.bf
            `OR1200_OR32_BF_MFD:
                alu_op <=  `OR1200_ALUOP_NOP_MFD;
            
            // l.movhi
            `OR1200_OR32_MOVHI_MFD:
                alu_op <=  `OR1200_ALUOP_MOVHI_MFD;
            
            // l.mfspr
            `OR1200_OR32_MFSPR_MFD:
                alu_op <=  `OR1200_ALUOP_MFSR_MFD;
            
            // l.mtspr
            `OR1200_OR32_MTSPR_MFD:
                alu_op <=  `OR1200_ALUOP_MTSR_MFD;
            
            // l.addi
            `OR1200_OR32_ADDI_MFD:
                alu_op <=  `OR1200_ALUOP_ADD_MFD;
            
            // l.addic
            `OR1200_OR32_ADDI_MFDC_MFD:
                alu_op <=  `OR1200_ALUOP_ADD_MFDC_MFD;
            
            // l.andi
            `OR1200_OR32_ANDI_MFD:
                alu_op <=  `OR1200_ALUOP_AND_MFD;
            
            // l.ori
            `OR1200_OR32_ORI_MFD:
                alu_op <=  `OR1200_ALUOP_OR_MFD;
            
            // l.xori
            `OR1200_OR32_XORI_MFD:
                alu_op <=  `OR1200_ALUOP_XOR_MFD;
            
            // l.muli
            `OR1200_OR32_MULI_MFD:
                alu_op <=  `OR1200_ALUOP_MUL_MFD;
            
            // Shift and rotate insns with immediate
            `OR1200_OR32_SH_MFD_ROTI_MFD:
                alu_op <=  `OR1200_ALUOP_SHROT_MFD;
            
            // SFXX insns with immediate
            `OR1200_OR32_SFXX_MFDI_MFD:
                alu_op <=  `OR1200_ALUOP_COMP_MFD;
            
            // ALU instructions except the one with immediate
            `OR1200_OR32_ALU_MFD:
                alu_op <=  id_insn[3:0];
            
            // SFXX instructions
            `OR1200_OR32_SFXX_MFD:
                alu_op <=  `OR1200_ALUOP_COMP_MFD;
            
            // Default
            default: begin
                alu_op <=  `OR1200_ALUOP_NOP_MFD;
            end
        endcase
    end
end

//
// Decode of mac_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        mac_op <=  `OR1200_MACOP_NOP_MFD;
    else if (!ex_freeze & id_freeze | flushpipe)
        mac_op <=  `OR1200_MACOP_NOP_MFD;
    else if (!ex_freeze)
        case (id_insn[31:26])        // synopsys parallel_case

            // l.maci
            `OR1200_OR32_MACI_MFD:
                mac_op <=  `OR1200_MACOP_MAC_MFD;

            // l.nop
            `OR1200_OR32_MACMSB_MFD:
                mac_op <=  id_insn[1:0];

            // Illegal and OR1200 unsupported instructions
            default: begin
                mac_op <=  `OR1200_MACOP_NOP_MFD;
            end          

        endcase
    else
        mac_op <=  `OR1200_MACOP_NOP_MFD;
end

//
// Decode of shrot_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        shrot_op <=  `OR1200_SHROTOP_NOP_MFD;
    else if (!ex_freeze & id_freeze | flushpipe)
        shrot_op <=  `OR1200_SHROTOP_NOP_MFD;
    else if (!ex_freeze) begin
        shrot_op <=  id_insn[`OR1200_SHROTOP_POS_MFD];
    end
end

//
// Decode of rfwb_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        rfwb_op <=  `OR1200_RFWBOP_NOP_MFD;
    else  if (!ex_freeze & id_freeze | flushpipe)
        rfwb_op <=  `OR1200_RFWBOP_NOP_MFD;
    else  if (!ex_freeze) begin
        case (id_insn[31:26])        // synopsys parallel_case

            // j.jal
            `OR1200_OR32_J_MFDAL_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LR_MFD;
            
            // j.jalr
            `OR1200_OR32_J_MFDAL_MFDR_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LR_MFD;
            
            // l.movhi
            `OR1200_OR32_MOVHI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
              
            // l.mfspr
            `OR1200_OR32_MFSPR_MFD:
                rfwb_op <=  `OR1200_RFWBOP_SPRS_MFD;
            
            // l.lwz
            `OR1200_OR32_LWZ_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LSU_MFD;
            
            // l.lbz
            `OR1200_OR32_LBZ_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LSU_MFD;
            
            // l.lbs
            `OR1200_OR32_LBS_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LSU_MFD;
            
            // l.lhz
            `OR1200_OR32_LHZ_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LSU_MFD;
            
            // l.lhs
            `OR1200_OR32_LHS_MFD:
                rfwb_op <=  `OR1200_RFWBOP_LSU_MFD;
            
            // l.addi
            `OR1200_OR32_ADDI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // l.addic
            `OR1200_OR32_ADDI_MFDC_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // l.andi
            `OR1200_OR32_ANDI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // l.ori
            `OR1200_OR32_ORI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // l.xori
            `OR1200_OR32_XORI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // l.muli
            `OR1200_OR32_MULI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // Shift and rotate insns with immediate
            `OR1200_OR32_SH_MFD_ROTI_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // ALU instructions except the one with immediate
            `OR1200_OR32_ALU_MFD:
                rfwb_op <=  `OR1200_RFWBOP_ALU_MFD;
            
            // Instructions w/o register-file write-back
            default: begin
                rfwb_op <=  `OR1200_RFWBOP_NOP_MFD;
            end

        endcase
    end
end

//
// Decode of pre_branch_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        pre_branch_op <=  `OR1200_BRANCHOP_NOP_MFD;
    else if (flushpipe)
        pre_branch_op <=  `OR1200_BRANCHOP_NOP_MFD;
    else if (!id_freeze) begin
        case (if_insn[31:26])        // synopsys parallel_case
          
            // l.j
            `OR1200_OR32_J_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_BAL_MFD;
            
            // j.jal
            `OR1200_OR32_J_MFDAL_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_BAL_MFD;
            
            // j.jalr
            `OR1200_OR32_J_MFDAL_MFDR_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_J_MFDR_MFD;
            
            // l.jr
            `OR1200_OR32_J_MFDR_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_J_MFDR_MFD;
            
            // l.bnf
            `OR1200_OR32_BNF_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_BNF_MFD;
            
            // l.bf
            `OR1200_OR32_BF_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_BF_MFD;
            
            // l.rfe
            `OR1200_OR32_RFE_MFD:
                pre_branch_op <=  `OR1200_BRANCHOP_RFE_MFD;
            
            // Non branch instructions
            default: begin
                pre_branch_op <=  `OR1200_BRANCHOP_NOP_MFD;
            end
        endcase
    end
end

//
// Generation of branch_op
//
always @(posedge clk or negedge rst_n)
    if (!rst_n)
        branch_op <=  `OR1200_BRANCHOP_NOP_MFD;
    else if (!ex_freeze & id_freeze | flushpipe)
        branch_op <=  `OR1200_BRANCHOP_NOP_MFD;        
    else if (!ex_freeze)
        branch_op <=  pre_branch_op;

//
// Decode of lsu_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        lsu_op <=  `OR1200_LSUOP_NOP_MFD;
    else if (!ex_freeze & id_freeze | flushpipe)
        lsu_op <=  `OR1200_LSUOP_NOP_MFD;
    else if (!ex_freeze)  begin
      case (id_insn[31:26])        // synopsys parallel_case
        
          // l.lwz
          `OR1200_OR32_LWZ_MFD:
              lsu_op <=  `OR1200_LSUOP_LWZ_MFD;
          
          // l.lbz
          `OR1200_OR32_LBZ_MFD:
              lsu_op <=  `OR1200_LSUOP_LBZ_MFD;
          
          // l.lbs
          `OR1200_OR32_LBS_MFD:
              lsu_op <=  `OR1200_LSUOP_LBS_MFD;
          
          // l.lhz
          `OR1200_OR32_LHZ_MFD:
              lsu_op <=  `OR1200_LSUOP_LHZ_MFD;
          
          // l.lhs
          `OR1200_OR32_LHS_MFD:
              lsu_op <=  `OR1200_LSUOP_LHS_MFD;
          
          // l.sw
          `OR1200_OR32_SW_MFD:
              lsu_op <=  `OR1200_LSUOP_SW_MFD;
          
          // l.sb
          `OR1200_OR32_SB_MFD:
              lsu_op <=  `OR1200_LSUOP_SB_MFD;
          
          // l.sh
          `OR1200_OR32_SH_MFD:
              lsu_op <=  `OR1200_LSUOP_SH_MFD;
          
          // Non load/store instructions
          default: begin
              lsu_op <=  `OR1200_LSUOP_NOP_MFD;
          end
      endcase
    end
end

//
// Decode of comp_op
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        comp_op <=  4'd0;
    end else if (!ex_freeze & id_freeze | flushpipe)
        comp_op <=  4'd0;
    else if (!ex_freeze)
        comp_op <=  id_insn[24:21];
end

//
// Decode of l.sys
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        sig_syscall <=  1'b0;
    else if (!ex_freeze & id_freeze | flushpipe)
        sig_syscall <=  1'b0;
    else if (!ex_freeze) begin
        sig_syscall <=  (id_insn[31:23] == {`OR1200_OR32_XSYNC_MFD, 3'b000});
    end
end

//
// Decode of l.trap
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        sig_trap <=  1'b0;
    else if (!ex_freeze & id_freeze | flushpipe)
        sig_trap <=  1'b0;
    else if (!ex_freeze) begin
        sig_trap <=  (id_insn[31:23] == {`OR1200_OR32_XSYNC_MFD, 3'b010});
    end
end

endmodule
