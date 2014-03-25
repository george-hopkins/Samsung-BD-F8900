//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's generate PC                                        ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  PC, interface to IC.                                        ////
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

// Revision 1.6  2002/03/29 15:16:55  lampret
// Some of the warnings fixed.
//
// Revision 1.5  2002/02/11 04:33:17  lampret
// Speed optimizations (removed duplicate _cyc_ and _stb_). Fixed D/IMMU cache-inhibit attr.
//
// Revision 1.4  2002/01/28 01:16:00  lampret
// Changed 'void' nop-ops instead of insn[0] to use insn[16]. Debug unit stalls the tick timer. Prepared new flag generation for add and and insns. Blocked DC/IC while they are turned off. Fixed I/D MMU SPRs layout except WAYs. TODO: smart IC invalidate, l.j 2 and TLB ways.
//
// Revision 1.3  2002/01/18 07:56:00  lampret
// No more low/high priority interrupts (PICPR removed). Added tick timer exception. Added exception prefix (SR[EPH]). Fixed single-step bug whenreading NPC.
//
// Revision 1.2  2002/01/14 06:18:22  lampret
// Fixed mem2reg bug in FAST implementation. Updated debug unit to work with new genpc/if.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.10  2001/11/20 18:46:15  simons
// Break point bug fixed
//
// Revision 1.9  2001/11/18 09:58:28  lampret
// Fixed some l.trap typos.
//
// Revision 1.8  2001/11/18 08:36:28  lampret
// For GDB changed single stepping and disabled trap exception.
//
// Revision 1.7  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.6  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:36  igorm
// no message
//
// Revision 1.1  2001/08/09 13:39:33  lampret
// Major clean-up.
//
//


module or1200_genpc_mfd(
    // Clock and reset
    clk, rst_n,

    // External i/f to IC
    icpu_adr_o, icpu_cycstb_o, icpu_sel_o, icpu_tag_o,
    icpu_rty_i, icpu_adr_i,

    // Internal i/f
    branch_op, except_type, except_prefix,
    branch_addrofs, lr_restor, flag, taken, except_start,
    binsn_addr, epcr, spr_dat_i, genpc_refetch,
    genpc_freeze, no_more_dslot
);

//
// I/O
//

//
// Clock and reset
//
input                clk;
input                rst_n;

//
// External i/f to IC
//
output   [31:0]      icpu_adr_o;
output               icpu_cycstb_o;
output   [3:0]       icpu_sel_o;
output   [3:0]       icpu_tag_o;
input                icpu_rty_i;
input    [31:0]      icpu_adr_i;

//
// Internal i/f
//
input    [`OR1200_BRANCHOP_WIDTH_MFD-1:0] branch_op;
input    [`OR1200_EXCEPT_WIDTH_MFD-1:0]   except_type;
input                except_prefix;
input    [31:2]      branch_addrofs;
input    [31:0]      lr_restor;
input                flag;
output               taken;
input                except_start;
input    [31:2]      binsn_addr;
input    [31:0]      epcr;
input    [31:0]      spr_dat_i;
input                genpc_refetch;
input                genpc_freeze;
input                no_more_dslot;

//
// Internal wires and regs
//
reg     [31:2]       pcreg;
reg     [31:0]       pc;
reg                  taken;    /* Set to in case of jump or taken branch */

//
// Address of insn to be fecthed
//
wire    cond = (!no_more_dslot & !except_start) & (icpu_rty_i | genpc_refetch);

reg    [31:0]            icpu_adr_o;
always @( cond or icpu_adr_i or pc )
case ( cond ) // synopsys infer_mux
  1'b1 : 
    icpu_adr_o = icpu_adr_i;
  1'b0 : 
    icpu_adr_o = pc;
endcase

//
// Control access to IC subsystem
//
assign icpu_cycstb_o = !genpc_freeze;
assign icpu_sel_o = 4'b1111;
assign icpu_tag_o = `OR1200_ITAG_NI_MFD;

//
// Async calculation of new PC value. This value is used for addressing the IC.
//
always @( * ) 
begin : b_genpc
    casex ({except_start, branch_op})    // synopsys parallel_case
        {1'b0, `OR1200_BRANCHOP_NOP_MFD}: 
        begin
            pc = {pcreg + 30'd1, 2'b0};
            taken = 1'b0;
        end
        {1'b0, `OR1200_BRANCHOP_J_MFD}: 
        begin
            pc = {branch_addrofs, 2'b0};
            taken = 1'b1;
        end
        {1'b0, `OR1200_BRANCHOP_J_MFDR_MFD}: 
        begin
            pc = lr_restor;
            taken = 1'b1;
        end
        {1'b0, `OR1200_BRANCHOP_BAL_MFD}: 
        begin
            pc = {binsn_addr + branch_addrofs, 2'b0};
            taken = 1'b1;
        end
        {1'b0, `OR1200_BRANCHOP_BF_MFD}:
            if (flag) 
            begin
                pc = {binsn_addr + branch_addrofs, 2'b0};
                taken = 1'b1;
            end
            else 
            begin
                pc = {pcreg + 30'd1, 2'b0};
                taken = 1'b0;
            end
        {1'b0, `OR1200_BRANCHOP_BNF_MFD}:
            if (flag) 
            begin
                pc = {pcreg + 30'd1, 2'b0};
                taken = 1'b0;
            end
            else 
            begin
                pc = {binsn_addr + branch_addrofs, 2'b0};
                taken = 1'b1;
            end
        {1'b0, `OR1200_BRANCHOP_RFE_MFD}: 
        begin
            pc = epcr;
            taken = 1'b1;
        end
        default: 
        begin
            pc = { {4{except_prefix}}, 16'h0000, except_type, 8'h00};
            taken = 1'b1;
        end
    endcase
end

//
// PC register
//
always @(posedge clk or negedge rst_n)
    if (!rst_n)
        pcreg <=  30'd63;
    else if (no_more_dslot | except_start | !genpc_freeze & !icpu_rty_i & !genpc_refetch)
        pcreg <=  pc[31:2];

endmodule
