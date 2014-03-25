//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's Data Cache top level                               ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  Instantiation of all DC blocks.                             ////
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

// Revision 1.6  2002/10/17 20:04:40  lampret
// Added BIST scan. Special VS RAMs need to be used to implement BIST.
//
// Revision 1.5  2002/08/18 19:54:47  lampret
// Added store buffer.
//
// Revision 1.4  2002/02/11 04:33:17  lampret
// Speed optimizations (removed duplicate _cyc_ and _stb_). Fixed D/IMMU cache-inhibit attr.
//
// Revision 1.3  2002/01/28 01:16:00  lampret
// Changed 'void' nop-ops instead of insn[0] to use insn[16]. Debug unit stalls the tick timer. Prepared new flag generation for add and and insns. Blocked DC/IC while they are turned off. Fixed I/D MMU SPRs layout except WAYs. TODO: smart IC invalidate, l.j 2 and TLB ways.
//
// Revision 1.2  2002/01/14 06:18:22  lampret
// Fixed mem2reg bug in FAST implementation. Updated debug unit to work with new genpc/if.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.10  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.9  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:35  igorm
// no message
//
// Revision 1.4  2001/08/13 03:36:20  lampret
// Added cfg regs. Moved all defines into one defines.v file. More cleanup.
//
// Revision 1.3  2001/08/09 13:39:33  lampret
// Major clean-up.
//
// Revision 1.2  2001/07/22 03:31:53  lampret
// Fixed RAM's oen bug. Cache bypass under development.
//
// Revision 1.1  2001/07/20 00:46:03  lampret
// Development version of RTL. Libraries are missing.
//
//
module or1200_dc_top_mfd(

    // Rst, clk and clock control
    clk, rst_n,

    // External i/f
    dcsb_dat_o, dcsb_adr_o, dcsb_cyc_o, dcsb_we_o, dcsb_sel_o, 
    dcsb_dat_i, dcsb_ack_i, dcsb_err_i,

    // Internal i/f
    dc_en,
    dcdmmu_adr_i, dcdmmu_cycstb_i, dcdmmu_ci_i,
    dcpu_we_i, dcpu_sel_i, dcpu_tag_i, dcpu_dat_i,
    dcpu_dat_o, dcpu_ack_o, dcpu_rty_o, dcdmmu_err_o, dcdmmu_tag_o,

    // SPRs
    spr_cs, spr_write, spr_dat_i,

    // Sram I/F
    dc_ram_dout, dc_ram_din, dc_ram_en, dc_ram_we, dc_ram_addr,
    dctag_sram_addr, dctag_sram_csn, dctag_sram_wen, dctag_sram_din, dctag_sram_dout
);
//////////////////////////////////////////////////////////////////////
parameter dctag_dw = `OR1200_DC_CI_INDXL_MFD-1-`OR1200_DCTAG_MFDL_MFD+1;
parameter dctag_aw = `OR1200_DCTAG_MFD;
parameter depth    = 1 << dctag_aw; 
//////////////////////////////////////////////////////////////////////
// I/O
input             clk;
input             rst_n;
///////////////////////////////////////////////////////
// External I/F
output  [31:0]    dcsb_dat_o;   // write data(32bit interface)
output  [31:0]    dcsb_adr_o;
output            dcsb_cyc_o;
output            dcsb_we_o;
output  [3:0]     dcsb_sel_o;
input   [63:0]    dcsb_dat_i;   // read data(64bit interface)
input             dcsb_ack_i;
input             dcsb_err_i;
///////////////////////////////////////////////////////
// Internal I/F
input             dc_en;
input   [31:0]    dcdmmu_adr_i;
input             dcdmmu_cycstb_i;
input             dcdmmu_ci_i;
input             dcpu_we_i;
input   [3:0]     dcpu_sel_i;
input   [3:0]     dcpu_tag_i;
input   [31:0]    dcpu_dat_i;
output  [31:0]    dcpu_dat_o;
output            dcpu_ack_o;
output            dcpu_rty_o;
output            dcdmmu_err_o;
output  [3:0]     dcdmmu_tag_o;
///////////////////////////////////////////////////////
// SPR access
input             spr_cs;
input             spr_write;
input   [31:0]    spr_dat_i;
///////////////////////////////////////////////////////
// Sram I/F
input   [63:0]    dc_ram_dout;
output            dc_ram_en;
output  [7:0]     dc_ram_we;
output  [63:0]    dc_ram_din;
output  [`OR1200_DCINDX_MFD-2:0] dc_ram_addr;
///////////////////////////////////////////////////////
output  [dctag_aw-1:0] dctag_sram_addr;
output                 dctag_sram_csn;
output                 dctag_sram_wen;
output  [dctag_dw-1:0] dctag_sram_din;
input   [dctag_dw-1:0] dctag_sram_dout;
//////////////////////////////////////////////////////////////////////
// Internal wires and regs
reg               tag_v;
wire    [`OR1200_DC_CI_INDXL_MFD-1-`OR1200_DCTAG_MFDL_MFD:0] tag;
wire    [63:0]    to_dcram;
wire    [63:0]    from_dcram;
wire    [31:0]    saved_addr;
wire    [7:0]     dcram_we;
wire              dctag_we;
wire    [31:0]    dc_addr;
wire              dcfsm_biu_read;
wire              dcfsm_biu_write;
reg               tagcomp_miss;
wire    [`OR1200_DCINDX_MFDH_MFD:`OR1200_DCLS_MFD] dctag_addr;
wire              dctag_en;
wire              dctag_v; 
wire              dc_inv;
wire              dcfsm_first_hit_ack;
wire              dcfsm_first_miss_ack;
wire              dcfsm_first_miss_err;
wire              dcfsm_burst;
wire              dcfsm_tag_we;
reg               r_b_ci_region;
//////////////////////////////////////////////////////////////////////
assign dcsb_adr_o = dc_addr;
assign dctag_we = dcfsm_tag_we;
assign dctag_addr = dc_addr[`OR1200_DCINDX_MFDH_MFD:`OR1200_DCLS_MFD];
assign dctag_en = dc_en && (dcdmmu_cycstb_i || dctag_we);
assign dcsb_dat_o = dcpu_dat_i;
//////////////////////////////////////////////////////////////////////
// Bypases of the DC when DC is disabled
assign dcsb_cyc_o   = dc_en ? (dcfsm_biu_read | dcfsm_biu_write) : dcdmmu_cycstb_i ; 
assign dcsb_we_o    = dc_en ? dcfsm_biu_write : dcpu_we_i;
assign dcsb_sel_o   = (dc_en & dcfsm_biu_read & !dcfsm_biu_write & !r_b_ci_region) ? 4'b1111 : dcpu_sel_i;
assign dcpu_rty_o   = ~dcpu_ack_o;
assign dcdmmu_tag_o = dcdmmu_err_o ? `OR1200_DTAG_BE_MFD : dcpu_tag_i;
//////////////////////////////////////////////////////////////////////
// DC/LSU normal and error termination
assign dcpu_ack_o     = dc_en ? (dcfsm_first_hit_ack | dcfsm_first_miss_ack) : dcsb_ack_i;
assign dcdmmu_err_o   = dc_en ? dcfsm_first_miss_err : dcsb_err_i;
//////////////////////////////////////////////////////////////////////
// Select between input data generated by LSU or by BIU
wire   [63:0]  w_dcpu_wr_dat_64 = dc_addr[2] ? {dcpu_dat_i, 32'd0} : {32'd0, dcpu_dat_i}; 
assign to_dcram = (dcfsm_biu_read) ? dcsb_dat_i : w_dcpu_wr_dat_64;
//////////////////////////////////////////////////////////////////////
reg           r_b_odd_wd;
always @(posedge clk or negedge rst_n)
if( !rst_n )
    r_b_odd_wd <=  1'b0;
else
    r_b_odd_wd <=  dcdmmu_adr_i[2];
//////////////////////////////////////////////////////////////////////
wire   [31:0]  w_from_dcram_32 = r_b_odd_wd ? from_dcram[63:32] : from_dcram[31:0];
wire   [31:0]  w_dcsb_dat_32   = r_b_odd_wd ? dcsb_dat_i[63:32]  : dcsb_dat_i[31:0];
assign dcpu_dat_o = (dcfsm_first_miss_ack | !dc_en) ? w_dcsb_dat_32 : w_from_dcram_32;
//////////////////////////////////////////////////////////////////////
wire          w_b_ci_region = |dcdmmu_adr_i[31:`OR1200_DC_CI_INDXL_MFD];
always @(posedge clk or negedge rst_n)
if( !rst_n )
    r_b_ci_region <=  1'b0;
else
    r_b_ci_region <=  w_b_ci_region;
//////////////////////////////////////////////////////////////////////
// Tag comparison
always @( * ) 
begin
    if ((tag != saved_addr[`OR1200_DC_CI_INDXL_MFD-1:`OR1200_DCTAG_MFDL_MFD]) || !tag_v || r_b_ci_region)
        tagcomp_miss = 1'b1;
    else
        tagcomp_miss = 1'b0;
end
wire    dc_cycstb = dcdmmu_cycstb_i ; 
//////////////////////////////////////////////////////////////////////
// Instantiation of DC Finite State Machine
or1200_dc_fsm_mfd or1200_dc_fsm(
    .clk             (clk                 ),
    .rst_n           (rst_n               ),
    .dc_en           (dc_en               ),
    .dcdmmu_cycstb_i (dc_cycstb           ),
    .dcdmmu_ci_i     (r_b_ci_region       ),
    .dcpu_we_i       (dcpu_we_i           ),
    .dcpu_sel_i      (dcpu_sel_i          ),
    .tagcomp_miss    (tagcomp_miss        ),
    .biudata_valid   (dcsb_ack_i          ),
    .biudata_error   (dcsb_err_i          ),
    .start_addr      (dcdmmu_adr_i        ),
    .saved_addr      (saved_addr          ),
    .dcram_we        (dcram_we            ),
    .biu_read        (dcfsm_biu_read      ),
    .biu_write       (dcfsm_biu_write     ),
    .first_hit_ack   (dcfsm_first_hit_ack ),
    .first_miss_ack  (dcfsm_first_miss_ack),
    .first_miss_err  (dcfsm_first_miss_err),
    .burst           (dcfsm_burst         ),
    .tag_we          (dcfsm_tag_we        ),
    .dc_addr         (dc_addr             )
);
//////////////////////////////////////////////////////////////////////
// Instantiation of DC main memory
assign from_dcram  = dc_ram_dout;
assign dc_ram_en   = dc_en && (dcdmmu_cycstb_i || (|dcram_we));
assign dc_ram_din  = to_dcram;
assign dc_ram_addr = dc_addr[`OR1200_DCINDX_MFDH_MFD:3];
assign dc_ram_we   = dcram_we;
//////////////////////////////////////////////////////////////////////
// Instantiation of DC TAG memory
reg      [depth-1:0] r_tag_v;
integer              i;

always @(posedge clk or negedge rst_n)
if( !rst_n )
    for( i=0; i<depth; i=i+1)
        r_tag_v[i] <= 1'b0;
else if( dctag_en & dctag_we )
    r_tag_v[dctag_addr]  <=  1'b1;

always @(posedge clk or negedge rst_n)
if( !rst_n )
    tag_v  <= 1'b0;
else if( dctag_en )
    tag_v  <=  r_tag_v[dctag_addr];
///////////////////////////////////////////////////////
assign  dctag_sram_csn  = !dctag_en; 
assign  dctag_sram_wen  = !dctag_we; 
assign  dctag_sram_addr = dctag_addr; 
assign  dctag_sram_din  = dc_addr[`OR1200_DC_CI_INDXL_MFD-1:`OR1200_DCTAG_MFDL_MFD]; 
assign  tag = dctag_sram_dout;
//////////////////////////////////////////////////////////////////////
endmodule
