//////////////////////////////////////////////////////////////////////
////                                  ////
////  OR1200's Top level multiplier and MAC               ////
////                                  ////
////  This file is part of the OpenRISC 1200 project          ////
////  http://www.opencores.org/cores/or1k/            ////
////                                  ////
////  Description                         ////
////  Multiplier is 32x32 however multiply instructions only      ////
////  use lower 32 bits of the result. MAC is 32x32=64+64.    ////
////                                  ////
////  To Do:                              ////
////   - make signed division better, w/o negating the operands   ////
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

// Revision 1.2  2002/09/08 05:52:16  lampret
// Added optional l.div/l.divu insns. By default they are disabled.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.3  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.2  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:38  igorm
// no message
//
//

module or1200_mult_mac_mfd(
    // Clock and reset
    clk, rst_n,

    // Multiplier/MAC interface
    ex_freeze, id_macrc_op, macrc_op, a, b, mac_op, alu_op, result, mac_stall_r,

    // SPR interface
    spr_cs, spr_write, spr_addr, spr_dat_i, spr_dat_o
);

parameter width = `OR1200_OPERAND_WIDTH_MFD;

//
// I/O
//

//
// Clock and reset
//
input                clk;
input                rst_n;

//
// Multiplier/MAC interface
//
input                ex_freeze;
input                id_macrc_op;
input                macrc_op;
input    [width-1:0] a;
input    [width-1:0] b;
input    [`OR1200_MACOP_WIDTH_MFD-1:0]    mac_op;
input    [`OR1200_ALUOP_WIDTH_MFD-1:0]    alu_op;
output   [width-1:0] result;
output               mac_stall_r;

//
// SPR interface
//
input                spr_cs;
input                spr_write;
input    [31:0]      spr_addr;
input    [31:0]      spr_dat_i;
output   [31:0]      spr_dat_o;

//
// Internal wires and regs
//
reg    [width-1:0]   result;
reg    [2*width-1:0] mul_prod_r;
reg    [2*width-1:0] mac_r;
wire   [2*width-1:0] mul_prod;
wire   [`OR1200_MACOP_WIDTH_MFD-1:0]    mac_op;
reg    [`OR1200_MACOP_WIDTH_MFD-1:0]    mac_op_r1;
reg    [`OR1200_MACOP_WIDTH_MFD-1:0]    mac_op_r2;
reg    [`OR1200_MACOP_WIDTH_MFD-1:0]    mac_op_r3;
reg                mac_stall_r;
reg    [width-1:0] x;
reg    [width-1:0] y;
wire               spr_maclo_we;
wire               spr_machi_we;
wire               alu_op_div_divu;
wire               alu_op_div;
reg                div_free;
`ifdef OR1200_IMPL_DIV_MFD
reg    [width-1:0] div_tmp;
reg    [5:0]       div_cntr;
`endif

//
// Combinatorial logic
//
assign spr_maclo_we = spr_cs & spr_write & spr_addr[`OR1200_MAC_ADDR_MFD];
assign spr_machi_we = spr_cs & spr_write & !spr_addr[`OR1200_MAC_ADDR_MFD];
assign spr_dat_o = spr_addr[`OR1200_MAC_ADDR_MFD] ? mac_r[31:0] : mac_r[63:32];
always @( alu_op_div or a )
begin : b_mac_3
/* synopsys resource r0 : map_to_module = "DW01_sub", implementation = "csm", ops = "SUB_4"; */
    x = alu_op_div & a[31] ? 32'b0 -/* synopsys label SUB_4 */ a : a; 
end
always @( alu_op_div or b )
begin : b_mac_4
/* synopsys resource r0 : map_to_module = "DW01_sub", implementation = "csm", ops = "SUB_5"; */
    y = alu_op_div & b[31] ? 32'b0 -/* synopsys label SUB_5 */ b : b; 
end

`ifdef OR1200_IMPL_DIV_MFD
assign alu_op_div = (alu_op == `OR1200_ALUOP_DIV_MFD);
assign alu_op_div_divu = alu_op_div | (alu_op == `OR1200_ALUOP_DIV_MFDU_MFD);
always @( * )
begin : b_mac_2
/* synopsys resource r0 : map_to_module = "DW01_sub", implementation = "csm", ops = "SUB_3"; */
    div_tmp = mul_prod_r[63:32] - y;    // synopsys label SUB_3
end
`else
assign alu_op_div = 1'b0;
assign alu_op_div_divu = 1'b0;
`endif

//
// Select result of current ALU operation to be forwarded
// to next instruction and to WB stage
//
always @( * )
begin : b_mac_1
/* synopsys resource r0 : map_to_module = "DW01_sub", implementation = "csm", ops = "SUB_2"; */
    casex(alu_op)    // synopsys parallel_case
`ifdef OR1200_IMPL_DIV_MFD
        `OR1200_ALUOP_DIV_MFD:
            result = a[31] ^ b[31] ? 32'b0 -/* synopsys label SUB_2 */ mul_prod_r[31:0] : mul_prod_r[31:0];
        `OR1200_ALUOP_DIV_MFDU_MFD,
`endif
        `OR1200_ALUOP_MUL_MFD: begin
            result = mul_prod_r[31:0];
        end
        default:
            result = mac_r[59:28];
    endcase
end
//
// Instantiation of the multiplier
//

`ifdef OR1200_ASIC_MFD_MULTP2_32X32_MFD
or1200_amultp2_32x32_mfd or1200_amultp2_32x32(
    .X(x),
    .Y(y),
    .RST(!rst_n),
    .CLK(clk),
    .P(mul_prod)
);
`else
or1200_gmultp2_32x32_mfd or1200_gmultp2_32x32(
    .X(x),
    .Y(y),
    .rst_n(rst_n),
    .clk(clk),
    .P(mul_prod)
);
`endif


//
// Registered output from the multiplier and
// an optional divider
//
always @(negedge rst_n or posedge clk)
    if (!rst_n) begin
        mul_prod_r <=  64'h0000_0000_0000_0000;
        div_free <=  1'b1;
`ifdef OR1200_IMPL_DIV_MFD
        div_cntr <=  6'b00_0000;
`endif
    end
`ifdef OR1200_IMPL_DIV_MFD
    else if (|div_cntr) begin
        if (div_tmp[31])
            mul_prod_r <=  {mul_prod_r[62:0], 1'b0};
        else
            mul_prod_r <=  {div_tmp[30:0], mul_prod_r[31:0], 1'b1};
        div_cntr <=  div_cntr - 1'b1;
    end
    else if (alu_op_div_divu && div_free) begin
        mul_prod_r <=  {31'b0, x[31:0], 1'b0};
        div_cntr <=  6'b10_0000;
        div_free <=  1'b0;
    end
`endif
    else if (div_free | !ex_freeze) begin
        mul_prod_r <=  mul_prod[63:0];
        div_free <=  1'b1;
    end

//
 // Propagation of l.mac opcode
//
always @(posedge clk or negedge rst_n)
    if (!rst_n)
        mac_op_r1 <=  `OR1200_MACOP_WIDTH_MFD'b0;
    else
        mac_op_r1 <=  mac_op;

//
// Propagation of l.mac opcode
//
always @(posedge clk or negedge rst_n)
    if (!rst_n)
        mac_op_r2 <=  `OR1200_MACOP_WIDTH_MFD'b0;
    else
        mac_op_r2 <=  mac_op_r1;

//
// Propagation of l.mac opcode
//
always @(posedge clk or negedge rst_n)
    if (!rst_n)
        mac_op_r3 <=  `OR1200_MACOP_WIDTH_MFD'b0;
    else
        mac_op_r3 <=  mac_op_r2;

//
// Implementation of MAC
//
always @(negedge rst_n or posedge clk)
begin : b_mac_0
/* synopsys resource r0 : map_to_module = "DW01_add", implementation = "csm", ops = "ADD_1"; */
/* synopsys resource r1 : map_to_module = "DW01_sub", implementation = "csm", ops = "SUB_1"; */
    if (!rst_n)
        mac_r <=  64'h0000_0000_0000_0000;
`ifdef OR1200_MAC_SPR_WE_MFD
    else if (spr_maclo_we)
        mac_r[31:0] <=  spr_dat_i;
    else if (spr_machi_we)
        mac_r[63:32] <=  spr_dat_i;
`endif
`ifdef OR1200_ASIC_MFD_MULTP2_32X32_MFD  // ADDED BY ALEX: MULT has different pipeline
    else if (mac_op_r2 == `OR1200_MACOP_MAC_MFD)
        mac_r <=  mac_r + mul_prod_r; // synopsys label ADD_1
    else if (mac_op_r2 == `OR1200_MACOP_MSB_MFD)
        mac_r <=  mac_r - mul_prod_r; // synopsys label SUB_1
`else
    else if (mac_op_r3 == `OR1200_MACOP_MAC_MFD)
        mac_r <=  mac_r + mul_prod_r; // synopsys label ADD_1
    else if (mac_op_r3 == `OR1200_MACOP_MSB_MFD)
        mac_r <=  mac_r - mul_prod_r;    // synopsys label SUB_1
`endif
    else if (macrc_op & !ex_freeze)
        mac_r <=  64'h0000_0000_0000_0000;
end
//
// Stall CPU if l.macrc is in ID and MAC still has to process l.mac instructions
// in EX stage (e.g. inside multiplier)
// This stall signal is also used by the divider.
//
always @(negedge rst_n or posedge clk)
    if (!rst_n)
        mac_stall_r <=  1'b0;
    else
        mac_stall_r <=  (|mac_op | (|mac_op_r1) | (|mac_op_r2)) & id_macrc_op
`ifdef OR1200_IMPL_DIV_MFD
                | (|div_cntr)
`endif
                ;
endmodule
