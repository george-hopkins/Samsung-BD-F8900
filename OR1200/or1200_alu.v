//////////////////////////////////////////////////////////////////////
////                                  ////
////  OR1200's ALU                        ////
////                                  ////
////  This file is part of the OpenRISC 1200 project          ////
////  http://www.opencores.org/cores/or1k/            ////
////                                  ////
////  Description                         ////
////  ALU                             ////
////                                  ////
////  To Do:                              ////
////   - make it smaller and faster                   ////
////                                  ////
////  Author(s):                          ////
////      - Damjan Lampret, lampret@opencores.org         ////
////                                  ////
//////////////////////////////////////////////////////////////////////
////                                  ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG         ////
////                                  ////
//// This source file may be used and distributed without     ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                  ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                           ////
////                                  ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                             ////
////                                  ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml             ////
////                                  ////
//////////////////////////////////////////////////////////////////////

// Revision 1.10  2002/09/08 05:52:16  lampret
// Added optional l.div/l.divu insns. By default they are disabled.
//
// Revision 1.9  2002/09/07 19:16:10  lampret
// If SR[CY] implemented with OR1200_IMPL_ADDC_MFD enabled, l.add/l.addi also set SR[CY].
//
// Revision 1.8  2002/09/07 05:42:02  lampret
// Added optional SR[CY]. Added define to enable additional (compare) flag modifiers. Defines are OR1200_IMPL_ADDC_MFD and OR1200_ADDITIONAL_FLAG_MODIFIERS_MFD.
//
// Revision 1.7  2002/09/03 22:28:21  lampret
// As per Taylor Su suggestion all case blocks are full case by default and optionally (OR1200_CASE_DEFAULT_MFD) can be disabled to increase clock frequncy.
//
// Revision 1.6  2002/03/29 16:40:10  lampret
// Added a directive to ignore signed division variables that are only used in simulation.
//
// Revision 1.5  2002/03/29 16:33:59  lampret
// Added again just recently removed full_case directive
//
// Revision 1.4  2002/03/29 15:16:53  lampret
// Some of the warnings fixed.
//
// Revision 1.3  2002/01/28 01:15:59  lampret
// Changed 'void' nop-ops instead of insn[0] to use insn[16]. Debug unit stalls the tick timer. Prepared new flag generation for add and and insns. Blocked DC/IC while they are turned off. Fixed I/D MMU SPRs layout except WAYs. TODO: smart IC invalidate, l.j 2 and TLB ways.
//
// Revision 1.2  2002/01/14 06:18:22  lampret
// Fixed mem2reg bug in FAST implementation. Updated debug unit to work with new genpc/if.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.10  2001/11/12 01:45:40  lampret
// Moved flag bit into SR. Changed RF enable from constant enable to dynamic enable for read ports.
//
// Revision 1.9  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.8  2001/10/19 23:28:45  lampret
// Fixed some synthesis warnings. Configured with caches and MMUs.
//
// Revision 1.7  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:35  igorm
// no message
//
// Revision 1.2  2001/08/09 13:39:33  lampret
// Major clean-up.
//
// Revision 1.1  2001/07/20 00:46:03  lampret
// Development version of RTL. Libraries are missing.
//
//

module or1200_alu_mfd(
    a, b, mult_mac_result, macrc_op,
    alu_op, shrot_op, comp_op,
    result, flagforw, flag_we,
    cyforw, cy_we, carry
);

parameter width = `OR1200_OPERAND_WIDTH_MFD;

//
// I/O
//
input    [width-1:0] a;
input    [width-1:0] b;
input    [width-1:0] mult_mac_result;
input                macrc_op;
input    [`OR1200_ALUOP_WIDTH_MFD-1:0]    alu_op;
input    [`OR1200_SHROTOP_WIDTH_MFD-1:0]  shrot_op;
input    [`OR1200_COMPOP_WIDTH_MFD-1:0]   comp_op;
output   [width-1:0] result;
output               flagforw;
output               flag_we;
output               cyforw;
output               cy_we;
input                carry;

//
// Internal wires and regs
//
reg    [width-1:0]   result;
reg    [width-1:0]   shifted_rotated;
reg                  flagforw;
reg                  flagcomp;
reg                  flag_we;
reg                  cy_we;
wire    [width-1:0]  comp_a;
wire    [width-1:0]  comp_b;
`ifdef OR1200_IMPL_ALU_COMP1_MFD
wire                 a_eq_b;
wire                 a_lt_b;
`endif
reg    [width-1:0]   result_sum;
`ifdef OR1200_IMPL_ADDC_MFD
wire    [width-1:0]  result_csum;
wire                 cy_csum;
`endif
wire    [width-1:0]  result_and;
reg                  cy_sum;
reg                  cyforw;
reg[31:0]            result_ff1;

//
// Combinatorial logic
//
assign comp_a = {a[width-1] ^ comp_op[3] , a[width-2:0]};
assign comp_b = {b[width-1] ^ comp_op[3] , b[width-2:0]};
`ifdef OR1200_IMPL_ALU_COMP1_MFD
assign a_eq_b = (comp_a == comp_b);
assign a_lt_b = (comp_a < comp_b);
`endif
always @( a or b )
begin : b_alu_0
/* synopsys resource r0 : map_to_module = "DW01_add", implementation = "csm", ops = "ADD_1"; */
    {cy_sum, result_sum} = a + b; // synopsys label ADD_1
end
`ifdef OR1200_IMPL_ADDC_MFD
assign {cy_csum, result_csum} = a + b + carry;
`endif
assign result_and = a & b;


//
// Central part of the ALU
//
always @(result_ff1 or alu_op or a or b or result_sum or result_and or macrc_op or shifted_rotated or mult_mac_result) 
begin : b_alu_1
/* synopsys resource r0 : map_to_module = "DW01_sub", implementation = "csm", ops = "SUB_1"; */
`ifdef OR1200_CASE_DEFAULT_MFD
    casex (alu_op)        // synopsys parallel_case
`else
    casex (alu_op)        // synopsys full_case parallel_case
`endif
        `OR1200_ALUOP_SHROT_MFD : begin 
                result = shifted_rotated;
        end
        `OR1200_ALUOP_ADD_MFD : begin
                result = result_sum;
        end
`ifdef OR1200_IMPL_ADDC_MFD
        `OR1200_ALUOP_ADD_MFDC_MFD : begin
                result = result_csum;
        end
`endif
        `OR1200_ALUOP_SUB_MFD : begin
                result = a - b;    // synopsys label SUB_1
        end
        `OR1200_ALUOP_XOR_MFD : begin
                result = a ^ b;
        end
        `OR1200_ALUOP_OR_MFD  : begin
                result = a | b;
        end
        `OR1200_ALUOP_IMM_MFD : begin
                result = b;
        end
        `OR1200_ALUOP_MOVHI_MFD : begin
                if (macrc_op) begin
                    result = mult_mac_result;
                end
                else begin
                    result = b << 16;
                end
        end
`ifdef OR1200_IMPL_DIV_MFD
        `OR1200_ALUOP_DIV_MFD,
        `OR1200_ALUOP_DIV_MFDU_MFD,
`endif
        `OR1200_ALUOP_MUL_MFD : begin
                result = mult_mac_result;
        end

        `OR1200_ALUOP_FF1_MFD : begin
                result = result_ff1;
        end

`ifdef OR1200_CASE_DEFAULT_MFD
        default: begin
`else
        `OR1200_ALUOP_COMP_MFD, `OR1200_ALUOP_AND_MFD
`endif
                result = result_and;
        end
    endcase
end

//
// Generate flag and flag write enable
//
always @( * ) begin
    casex (alu_op)        // synopsys parallel_case
`ifdef OR1200_ADDITIONAL_FLAG_MODIFIERS_MFD
        `OR1200_ALUOP_ADD_MFD : begin
            flagforw = (result_sum == 32'h0000_0000);
            flag_we = 1'b1;
        end
`ifdef OR1200_IMPL_ADDC_MFD
        `OR1200_ALUOP_ADD_MFDC_MFD : begin
            flagforw = (result_csum == 32'h0000_0000);
            flag_we = 1'b1;
        end
`endif
        `OR1200_ALUOP_AND_MFD: begin
            flagforw = (result_and == 32'h0000_0000);
            flag_we = 1'b1;
        end
`endif
        `OR1200_ALUOP_COMP_MFD: begin
            flagforw = flagcomp;
            flag_we = 1'b1;
        end
        default: begin
            flagforw = 1'b0;
            flag_we = 1'b0;
        end
    endcase
end

//
// Generate SR[CY] write enable
//
always @( * ) begin
    casex (alu_op)        // synopsys parallel_case
`ifdef OR1200_IMPL_ADDC_MFD
        `OR1200_ALUOP_ADD_MFD : begin
            cyforw = cy_sum;
            cy_we = 1'b1;
        end
        `OR1200_ALUOP_ADD_MFDC_MFD: begin
            cyforw = cy_csum;
            cy_we = 1'b1;
        end
`endif
        default: begin
            cyforw = 1'b0;
            cy_we = 1'b0;
        end
    endcase
end

//
// Shifts and rotation
//
always @( * ) begin
    case (shrot_op)        // synopsys parallel_case
    `OR1200_SHROTOP_SLL_MFD :
                shifted_rotated = (a << b[4:0]);
        `OR1200_SHROTOP_SRL_MFD :
                shifted_rotated = (a >> b[4:0]);

`ifdef OR1200_IMPL_ALU_ROTATE_MFD
        `OR1200_SHROTOP_ROR_MFD :
                shifted_rotated = (a << (6'd32-{1'b0, b[4:0]})) | (a >> b[4:0]);
`endif
        default:
                shifted_rotated = ({32{a[31]}} << (6'd32-{1'b0, b[4:0]})) | a >> b[4:0];
    endcase
end



always @( * ) begin
result_ff1 = 
     a[31] ? 32'd32 :
     a[30] ? 32'd31 :
     a[29] ? 32'd30 :
     a[28] ? 32'd29 :
     a[27] ? 32'd28 :
     a[26] ? 32'd27 :
     a[25] ? 32'd26 :
     a[24] ? 32'd25 :
     a[23] ? 32'd24 :
     a[22] ? 32'd23 :
     a[21] ? 32'd22 :
     a[20] ? 32'd21 :
     a[19] ? 32'd20 :
     a[18] ? 32'd19 :
     a[17] ? 32'd18 :
     a[16] ? 32'd17 :
     a[15] ? 32'd16 :
     a[14] ? 32'd15 :
     a[13] ? 32'd14 :
     a[12] ? 32'd13 :
     a[11] ? 32'd12 :
     a[10] ? 32'd11 :
      a[9] ? 32'd10 :
      a[8] ? 32'd9 :
      a[7] ? 32'd8 :
      a[6] ? 32'd7 :
      a[5] ? 32'd6 :
      a[4] ? 32'd5 :
      a[3] ? 32'd4 :
      a[2] ? 32'd3 :
      a[1] ? 32'd2 : 
      a[0] ? 32'd1 : 32'd0; 
end

//
// First type of compare implementation
//
`ifdef OR1200_IMPL_ALU_COMP1_MFD
always @(comp_op or a_eq_b or a_lt_b) begin
    case(comp_op[2:0])    // synopsys parallel_case
        `OR1200_COP_SFEQ_MFD:
            flagcomp = a_eq_b;
        `OR1200_COP_SFNE_MFD:
            flagcomp = ~a_eq_b;
        `OR1200_COP_SFGT_MFD:
            flagcomp = ~(a_eq_b | a_lt_b);
        `OR1200_COP_SFGE_MFD:
            flagcomp = ~a_lt_b;
        `OR1200_COP_SFLT_MFD:
            flagcomp = a_lt_b;
        `OR1200_COP_SFLE_MFD:
            flagcomp = a_eq_b | a_lt_b;
        default:
            flagcomp = 1'b0;
    endcase
end
`endif

//
// Second type of compare implementation
//
`ifdef OR1200_IMPL_ALU_COMP2_MFD
always @( * ) begin
    case(comp_op[2:0])    // synopsys parallel_case
        `OR1200_COP_SFEQ_MFD:
            flagcomp = (comp_a == comp_b);
        `OR1200_COP_SFNE_MFD:
            flagcomp = (comp_a != comp_b);
        `OR1200_COP_SFGT_MFD:
            flagcomp = (comp_a > comp_b);
        `OR1200_COP_SFGE_MFD:
            flagcomp = (comp_a >= comp_b);
        `OR1200_COP_SFLT_MFD:
            flagcomp = (comp_a < comp_b);
        `OR1200_COP_SFLE_MFD:
            flagcomp = (comp_a <= comp_b);
        default:
            flagcomp = 1'b0;
    endcase
end
`endif

endmodule
