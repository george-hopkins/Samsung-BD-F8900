//////////////////////////////////////////////////////////////////////
////                                  ////
////  OR1200's interface to SPRs                  ////
////                                  ////
////  This file is part of the OpenRISC 1200 project          ////
////  http://www.opencores.org/cores/or1k/            ////
////                                  ////
////  Description                         ////
////  Decoding of SPR addresses and access to SPRs        ////
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

// Revision 1.9  2002/09/07 05:42:02  lampret
// Added optional SR[CY]. Added define to enable additional (compare) flag modifiers. Defines are OR1200_IMPL_ADDC_MFD and OR1200_ADDITIONAL_FLAG_MODIFIERS_MFD.
//
// Revision 1.8  2002/08/28 01:44:25  lampret
// Removed some commented RTL. Fixed SR/ESR flag bug.
//
// Revision 1.7  2002/03/29 15:16:56  lampret
// Some of the warnings fixed.
//
// Revision 1.6  2002/03/11 01:26:57  lampret
// Changed generation of SPR address. Now it is ORed from base and offset instead of a sum.
//
// Revision 1.5  2002/02/01 19:56:54  lampret
// Fixed combinational loops.
//
// Revision 1.4  2002/01/23 07:52:36  lampret
// Changed default reset values for SR and ESR to match or1ksim's. Fixed flop model in or1200_dpram_32x32 when OR1200_XILINX_RAM32X1D_MFD is defined.
//
// Revision 1.3  2002/01/19 09:27:49  lampret
// SR[TEE] should be zero after reset.
//
// Revision 1.2  2002/01/18 07:56:00  lampret
// No more low/high priority interrupts (PICPR removed). Added tick timer exception. Added exception prefix (SR[EPH]). Fixed single-step bug whenreading NPC.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.12  2001/11/23 21:42:31  simons
// Program counter divided to PPC and NPC.
//
// Revision 1.11  2001/11/23 08:38:51  lampret
// Changed DSR/DRR behavior and exception detection.
//
// Revision 1.10  2001/11/12 01:45:41  lampret
// Moved flag bit into SR. Changed RF enable from constant enable to dynamic enable for read ports.
//
// Revision 1.9  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.8  2001/10/14 13:12:10  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:36  igorm
// no message
//
// Revision 1.3  2001/08/13 03:36:20  lampret
// Added cfg regs. Moved all defines into one defines.v file. More cleanup.
//
// Revision 1.2  2001/08/09 13:39:33  lampret
// Major clean-up.
//
// Revision 1.1  2001/07/20 00:46:21  lampret
// Development version of RTL. Libraries are missing.
//
//

module or1200_sprs_mfd(
        // Clk & Rst
        clk, rst_n,

        // Internal CPU interface
        flagforw, flag_we, flag, cyforw, cy_we, carry,
        addrbase, addrofs, dat_i, alu_op, branch_op,
        epcr, eear, esr, except_started,
        to_wbmux, epcr_we, eear_we, esr_we, sr_we, to_sr, sr,
        spr_dat_cfgr, spr_dat_rf, spr_dat_mac,

        // From/to other RISC units
        spr_dat_pic, spr_dat_tt, spr_dat_pm,
//        spr_dat_dmmu, spr_dat_immu, spr_dat_du,
        spr_dat_dmmu, spr_dat_immu,
        spr_addr, spr_dat_o, spr_cs, spr_we
/*
        du_addr, du_dat_du, du_read,
        du_write, du_dat_cpu
*/

);

parameter width = `OR1200_OPERAND_WIDTH_MFD;

//
// I/O Ports
//

//
// Internal CPU interface
//
input                clk;                           // Clock
input                rst_n;                         // Reset
input                flagforw;                      // From ALU
input                flag_we;                       // From ALU
output               flag;                          // SR[F]
input                cyforw;                        // From ALU
input                cy_we;                         // From ALU
output               carry;                         // SR[CY]
input    [width-1:0] addrbase;                      // SPR base address
input    [15:0]      addrofs;                       // SPR offset
input    [width-1:0] dat_i;                         // SPR write data
input    [`OR1200_ALUOP_WIDTH_MFD-1:0]    alu_op;       // ALU operation
input    [`OR1200_BRANCHOP_WIDTH_MFD-1:0] branch_op;    // Branch operation
input    [width-1:0] epcr;                          // EPCR0
input    [width-1:0] eear;                          // EEAR0
input    [`OR1200_SR_WIDTH_MFD-1:0  ]     esr;          // ESR0
input                except_started;                // Exception was started
output   [width-1:0] to_wbmux;                      // For l.mfspr
output               epcr_we;                       // EPCR0 write enable
output               eear_we;                       // EEAR0 write enable
output               esr_we;                        // ESR0 write enable
output               sr_we;                         // Write enable SR
output   [`OR1200_SR_WIDTH_MFD-1:0]       to_sr;        // Data to SR
output   [`OR1200_SR_WIDTH_MFD-1:0]       sr;           // SR
input    [31:0]      spr_dat_cfgr;                  // Data from CFGR
input    [31:0]      spr_dat_rf;                    // Data from RF
input    [31:0]      spr_dat_mac;                   // Data from MAC

//
// To/from other RISC units
//
input    [31:0]      spr_dat_pic;                   // Data from PIC
input    [31:0]      spr_dat_tt;                    // Data from TT
input    [31:0]      spr_dat_pm;                    // Data from PM
input    [31:0]      spr_dat_dmmu;                  // Data from DMMU
input    [31:0]      spr_dat_immu;                  // Data from IMMU
output   [31:0]      spr_addr;                      // SPR Address
output   [31:0]      spr_dat_o;                     // Data to unit
output   [31:0]      spr_cs;                        // Unit select
output               spr_we;                        // SPR write enable

//
// Internal regs & wires
//
reg      [`OR1200_SR_WIDTH_MFD-1:0]       sr;           // SR
reg                  write_spr;                     // Write SPR
reg                  read_spr;                      // Read SPR
reg      [width-1:0] to_wbmux;                      // For l.mfspr
wire                 cfgr_sel;                      // Select for cfg regs
wire                 rf_sel;                        // Select for RF
wire                 sr_sel;                        // Select for SR    
wire                 epcr_sel;                      // Select for EPCR0
wire                 eear_sel;                      // Select for EEAR0
wire                 esr_sel;                       // Select for ESR0
wire      [31:0]     sys_data;                      // Read data from system SPRs
wire                 du_access;                     // Debug unit access
wire      [`OR1200_ALUOP_WIDTH_MFD-1:0]   sprs_op;      // ALU operation
reg       [31:0]     unqualified_cs;                // Unqualified chip selects

//
// Generate sprs opcode
//
assign sprs_op = alu_op;

//
// Generate SPR address from base address and offset
// OR from debug unit address
//
assign spr_addr = addrbase | {16'h0000, addrofs};

//
// SPR is written by debug unit or by l.mtspr
//
assign spr_dat_o = dat_i;


//
// Write into SPRs when l.mtspr
//
assign spr_we = write_spr;

//
// Qualify chip selects
//
assign spr_cs = unqualified_cs & {32{read_spr | write_spr}};

//
// Decoding of groups
//
always @( * )
    case (spr_addr[`OR1200_SPR_GROUP_BITS_MFD])    // synopsys parallel_case
        `OR1200_SPR_GROUP_WIDTH_MFD'd00: unqualified_cs = 32'b00000000_00000000_00000000_00000001;
        `OR1200_SPR_GROUP_WIDTH_MFD'd01: unqualified_cs = 32'b00000000_00000000_00000000_00000010;
        `OR1200_SPR_GROUP_WIDTH_MFD'd02: unqualified_cs = 32'b00000000_00000000_00000000_00000100;
        `OR1200_SPR_GROUP_WIDTH_MFD'd03: unqualified_cs = 32'b00000000_00000000_00000000_00001000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd04: unqualified_cs = 32'b00000000_00000000_00000000_00010000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd05: unqualified_cs = 32'b00000000_00000000_00000000_00100000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd06: unqualified_cs = 32'b00000000_00000000_00000000_01000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd07: unqualified_cs = 32'b00000000_00000000_00000000_10000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd08: unqualified_cs = 32'b00000000_00000000_00000001_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd09: unqualified_cs = 32'b00000000_00000000_00000010_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd10: unqualified_cs = 32'b00000000_00000000_00000100_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd11: unqualified_cs = 32'b00000000_00000000_00001000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd12: unqualified_cs = 32'b00000000_00000000_00010000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd13: unqualified_cs = 32'b00000000_00000000_00100000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd14: unqualified_cs = 32'b00000000_00000000_01000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd15: unqualified_cs = 32'b00000000_00000000_10000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd16: unqualified_cs = 32'b00000000_00000001_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd17: unqualified_cs = 32'b00000000_00000010_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd18: unqualified_cs = 32'b00000000_00000100_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd19: unqualified_cs = 32'b00000000_00001000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd20: unqualified_cs = 32'b00000000_00010000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd21: unqualified_cs = 32'b00000000_00100000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd22: unqualified_cs = 32'b00000000_01000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd23: unqualified_cs = 32'b00000000_10000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd24: unqualified_cs = 32'b00000001_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd25: unqualified_cs = 32'b00000010_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd26: unqualified_cs = 32'b00000100_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd27: unqualified_cs = 32'b00001000_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd28: unqualified_cs = 32'b00010000_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd29: unqualified_cs = 32'b00100000_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd30: unqualified_cs = 32'b01000000_00000000_00000000_00000000;
        `OR1200_SPR_GROUP_WIDTH_MFD'd31: unqualified_cs = 32'b10000000_00000000_00000000_00000000;
    endcase

//
// SPRs System Group
//

//
// What to write into SR
//
assign to_sr[`OR1200_SR_F_MFDO_MFD:`OR1200_SR_OV_MFD] =
        (branch_op == `OR1200_BRANCHOP_RFE_MFD) ? esr[`OR1200_SR_F_MFDO_MFD:`OR1200_SR_OV_MFD] :
        (write_spr && sr_sel) ? {1'b1, spr_dat_o[`OR1200_SR_F_MFDO_MFD-1:`OR1200_SR_OV_MFD]}:
        sr[`OR1200_SR_F_MFDO_MFD:`OR1200_SR_OV_MFD];
assign to_sr[`OR1200_SR_CY_MFD] =
        (branch_op == `OR1200_BRANCHOP_RFE_MFD) ? esr[`OR1200_SR_CY_MFD] :
        cy_we ? cyforw :
        (write_spr && sr_sel) ? spr_dat_o[`OR1200_SR_CY_MFD] :
        sr[`OR1200_SR_CY_MFD];
assign to_sr[`OR1200_SR_F_MFD] =
        (branch_op == `OR1200_BRANCHOP_RFE_MFD) ? esr[`OR1200_SR_F_MFD] :
        flag_we ? flagforw :
        (write_spr && sr_sel) ? spr_dat_o[`OR1200_SR_F_MFD] :
        sr[`OR1200_SR_F_MFD];
assign to_sr[`OR1200_SR_CE_MFD:`OR1200_SR_SM_MFD] =
        (branch_op == `OR1200_BRANCHOP_RFE_MFD) ? esr[`OR1200_SR_CE_MFD:`OR1200_SR_SM_MFD] :
        (write_spr && sr_sel) ? spr_dat_o[`OR1200_SR_CE_MFD:`OR1200_SR_SM_MFD]:
        sr[`OR1200_SR_CE_MFD:`OR1200_SR_SM_MFD];

//
// Selects for system SPRs
//
assign cfgr_sel = (spr_cs[`OR1200_SPR_GROUP_SYS_MFD] && (spr_addr[10:4] == `OR1200_SPR_CFGR_MFD));
assign rf_sel = (spr_cs[`OR1200_SPR_GROUP_SYS_MFD] && (spr_addr[10:5] == `OR1200_SPR_RF_MFD));
assign sr_sel = (spr_cs[`OR1200_SPR_GROUP_SYS_MFD] && (spr_addr[10:0] == `OR1200_SPR_SR_MFD));
assign epcr_sel = (spr_cs[`OR1200_SPR_GROUP_SYS_MFD] && (spr_addr[10:0] == `OR1200_SPR_EPCR_MFD));
assign eear_sel = (spr_cs[`OR1200_SPR_GROUP_SYS_MFD] && (spr_addr[10:0] == `OR1200_SPR_EEAR_MFD));
assign esr_sel = (spr_cs[`OR1200_SPR_GROUP_SYS_MFD] && (spr_addr[10:0] == `OR1200_SPR_ESR_MFD));

//
// Write enables for system SPRs
//
assign sr_we = (write_spr && sr_sel) | (branch_op == `OR1200_BRANCHOP_RFE_MFD) | flag_we | cy_we;

assign epcr_we = (write_spr && epcr_sel);
assign eear_we = (write_spr && eear_sel);
assign esr_we = (write_spr && esr_sel);

//
// Output from system SPRs
//
assign sys_data = (spr_dat_cfgr & {32{read_spr & cfgr_sel}}) |
          (spr_dat_rf & {32{read_spr & rf_sel}}) |
          ({{32-`OR1200_SR_WIDTH_MFD{1'b0}},sr} & {32{read_spr & sr_sel}}) |
          (epcr & {32{read_spr & epcr_sel}}) |
          (eear & {32{read_spr & eear_sel}}) |
          ({{32-`OR1200_SR_WIDTH_MFD{1'b0}},esr} & {32{read_spr & esr_sel}});

//
// Flag alias
//
assign flag = sr[`OR1200_SR_F_MFD];

//
// Carry alias
//
assign carry = sr[`OR1200_SR_CY_MFD];

//
// Supervision register
//
always @(posedge clk or negedge rst_n)
    if (!rst_n)
        sr <=  {1'b1, {`OR1200_SR_WIDTH_MFD-2{1'b0}}, 1'b1};
    else if (except_started) begin
        sr[`OR1200_SR_SM_MFD] <=  1'b1;
        sr[`OR1200_SR_TEE_MFD] <=  1'b0;
        sr[`OR1200_SR_IEE_MFD] <=  1'b0;
        sr[`OR1200_SR_DME_MFD] <=  1'b0;
        sr[`OR1200_SR_IME_MFD] <=  1'b0;
    end
    else if (sr_we)
        sr <=  to_sr[`OR1200_SR_WIDTH_MFD-1:0];

//
// MTSPR/MFSPR interface
//
always @( * ) begin
    case (sprs_op)    // synopsys parallel_case
        `OR1200_ALUOP_MTSR_MFD : begin
            write_spr = 1'b1;
            read_spr = 1'b0;
            to_wbmux = 32'b0;
        end
        `OR1200_ALUOP_MFSR_MFD : begin
            casex (spr_addr[`OR1200_SPR_GROUP_BITS_MFD]) // synopsys parallel_case
                `OR1200_SPR_GROUP_TT_MFD:
                    to_wbmux = spr_dat_tt;
                `OR1200_SPR_GROUP_PIC_MFD:
                    to_wbmux = spr_dat_pic;
                `OR1200_SPR_GROUP_PM_MFD:
                    to_wbmux = spr_dat_pm;
                `OR1200_SPR_GROUP_DMMU_MFD:
                    to_wbmux = spr_dat_dmmu;
                `OR1200_SPR_GROUP_IMMU_MFD:
                    to_wbmux = spr_dat_immu;
                `OR1200_SPR_GROUP_MAC_MFD:
                    to_wbmux = spr_dat_mac;
//                `OR1200_SPR_GROUP_DU_MFD:
//                    to_wbmux = spr_dat_du;
                `OR1200_SPR_GROUP_SYS_MFD:
                    to_wbmux = sys_data;
                default:
                    to_wbmux = 32'b0;
            endcase
            write_spr = 1'b0;
            read_spr = 1'b1;
        end
        default : begin
            write_spr = 1'b0;
            read_spr = 1'b0;
            to_wbmux = 32'b0;
        end
    endcase
end

endmodule
