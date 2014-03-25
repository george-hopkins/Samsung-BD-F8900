//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's Data Cache top level                               ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  Instantiation of all IC blocks.                             ////
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

// Revision 1.7  2002/10/17 20:04:40  lampret
// Added BIST scan. Special VS RAMs need to be used to implement BIST.
//
// Revision 1.6  2002/03/29 15:16:55  lampret
// Some of the warnings fixed.
//
// Revision 1.5  2002/02/11 04:33:17  lampret
// Speed optimizations (removed duplicate _cyc_ and _stb_). Fixed D/IMMU cache-inhibit attr.
//
// Revision 1.4  2002/02/01 19:56:54  lampret
// Fixed combinational loops.
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
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from ic.v and ic.v. Fixed CR+LF.
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
module or1200_ic_top_mfd(

    // Rst, clk and clock control
    clk, rst_n,
    // External i/f
    icbiu_adr_o, icbiu_cyc_o, 
    icbiu_dat_i, icbiu_ack_i, icbiu_abort_ack_i, icbiu_abort_adr_i, icbiu_err_i,

    // Internal i/f
    ic_en,
    icimmu_adr_i, icimmu_cycstb_i, 
    icpu_tag_i,
    icimmu_adr_o, 
    icpu_dat_o, icpu_ack_o, icimmu_rty_o, icimmu_err_o, icimmu_tag_o,

    // SPRs
    spr_cs, spr_write, spr_dat_i,

    // Sram I/F
    ic_ram_dout, ic_ram_din, ic_ram_en, ic_ram_we, ic_ram_addr,
    ictag_sram_addr, ictag_sram_csn, ictag_sram_wen, ictag_sram_din, ictag_sram_dout
);
//////////////////////////////////////////////////////////////////////
parameter ictag_dw = `OR1200_IC_CI_INDXL_MFD-1-`OR1200_ICTAG_MFDL_MFD+1;
parameter ictag_aw = `OR1200_ICTAG_MFD;
parameter depth    = 1 << ictag_aw; 
//////////////////////////////////////////////////////////////////////
// I/O
//////////////////////////////////////////////////////////////////////
// Clock and reset
input             clk;
input             rst_n;
///////////////////////////////////////////////////////
// External I/F
output  [31:0]    icbiu_adr_o;
output            icbiu_cyc_o;
input   [63:0]    icbiu_dat_i;
input             icbiu_ack_i;
input             icbiu_abort_ack_i;
input   [16:0]    icbiu_abort_adr_i;
input             icbiu_err_i;
///////////////////////////////////////////////////////
// Internal I/F
input             ic_en;
input             icimmu_cycstb_i;
input   [3:0]     icpu_tag_i;
input   [31:0]    icimmu_adr_i;
output  [31:0]    icimmu_adr_o;
output  [31:0]    icpu_dat_o;
output            icpu_ack_o;
output            icimmu_rty_o;
output            icimmu_err_o;
output  [3:0]     icimmu_tag_o;
///////////////////////////////////////////////////////
// SPR access
input             spr_cs;
input             spr_write;
input   [31:0]    spr_dat_i;
///////////////////////////////////////////////////////
// Sram I/F
input   [63:0]    ic_ram_dout;
output            ic_ram_en;
output            ic_ram_we;
output  [63:0]    ic_ram_din;
output  [`OR1200_ICINDX_MFD-2:0] ic_ram_addr;
///////////////////////////////////////////////////////
output  [ictag_aw-1:0] ictag_sram_addr;
output                 ictag_sram_csn;
output                 ictag_sram_wen;
output  [ictag_dw-1:0] ictag_sram_din;
input   [ictag_dw-1:0] ictag_sram_dout;
//////////////////////////////////////////////////////////////////////
// Internal wires and regs
reg               tag_v;
wire    [`OR1200_IC_CI_INDXL_MFD-1-`OR1200_ICTAG_MFDL_MFD:0] tag;
wire    [31:0]    saved_addr;
wire    [16:0]    saved_addr_mod;
wire              icram_we;
wire              ictag_we;
wire    [31:0]    ic_addr;
wire              icfsm_biu_read;
reg               tagcomp_miss;
wire    [`OR1200_ICINDX_MFDH_MFD:`OR1200_ICLS_MFD] ictag_addr;
wire              ictag_en;
wire              ictag_v; 
wire              ic_inv;
wire              icfsm_first_hit_ack;
wire              icfsm_first_miss_ack;
wire              icfsm_first_miss_err;
wire              icfsm_burst;
wire              icfsm_tag_we;
//////////////////////////////////////////////////////////////////////
// Registered Output Addresss which is transferred to CPU
reg     [31:0]    icimmu_adr_o;
always @(posedge clk or negedge rst_n)
if( !rst_n )
    icimmu_adr_o  <=  32'h0000_0100;
else
    icimmu_adr_o  <=  icimmu_adr_i;
//////////////////////////////////////////////////////////////////////
// Bypases of the IC when IC is disabled
assign icbiu_cyc_o  = (ic_en) ? icfsm_biu_read : icimmu_cycstb_i;
assign icimmu_rty_o = ~icpu_ack_o & ~icimmu_err_o;
assign icimmu_tag_o = icimmu_err_o ? `OR1200_ITAG_BE_MFD : icpu_tag_i;
//////////////////////////////////////////////////////////////////////
// CPU normal and error termination
assign icpu_ack_o   = ic_en ? (icfsm_first_hit_ack | icfsm_first_miss_ack) : icbiu_ack_i;
assign icimmu_err_o = 1'b0;    // by woong
//////////////////////////////////////////////////////////////////////
// Select between claddr generated by IC FSM and addr[3:2] generated by LSU
assign icbiu_adr_o  = ic_addr;
assign saved_addr_mod = icbiu_abort_ack_i ? icbiu_abort_adr_i : saved_addr[19:3];
assign ic_addr      = (icfsm_biu_read | icbiu_abort_ack_i) ? {saved_addr[31:20], saved_addr_mod, saved_addr[2:0]} : icimmu_adr_i;
//////////////////////////////////////////////////////////////////////
// Select between data generated by ICRAM or passed by BIU
reg           r_b_odd_wd;
always @(posedge clk or negedge rst_n)
if( !rst_n )
    r_b_odd_wd <=  1'b0;
else
    r_b_odd_wd <=  icimmu_adr_i[2];
//////////////////////////////////////////////////////////////////////
wire   [31:0] from_icram       = r_b_odd_wd ? ic_ram_dout[63:32] : ic_ram_dout[31:0];
wire   [31:0] w_icbiu_dat_i_32 = r_b_odd_wd ? icbiu_dat_i[63:32] : icbiu_dat_i[31:0];
//////////////////////////////////////////////////////////////////////
assign icpu_dat_o   = icfsm_first_miss_ack | !ic_en ? w_icbiu_dat_i_32 : from_icram;
//////////////////////////////////////////////////////////////////////
wire          w_b_ci_region = |icimmu_adr_i[`OR1200_IC_ACT_INDXH_MFD:`OR1200_IC_CI_INDXL_MFD];
reg           r_b_ci_region;
always @(posedge clk or negedge rst_n)
if( !rst_n )
    r_b_ci_region <=  1'b0;
else
    r_b_ci_region <=  w_b_ci_region;
//////////////////////////////////////////////////////////////////////
// Tag comparison
always @( * ) 
begin
    if ((tag != saved_addr[`OR1200_IC_CI_INDXL_MFD-1:`OR1200_ICTAG_MFDL_MFD]) || !tag_v || r_b_ci_region)
        tagcomp_miss = 1'b1;
    else
        tagcomp_miss = 1'b0;
end
//////////////////////////////////////////////////////////////////////
// Instantiation of IC Finite State Machine
or1200_ic_fsm_mfd or1200_ic_fsm(
    .clk             (clk                 ),
    .rst_n           (rst_n               ),
    .ic_en           (ic_en               ),
    .icimmu_cycstb_i (icimmu_cycstb_i     ),
    .icimmu_ci_i     (r_b_ci_region       ),
    .tagcomp_miss    (tagcomp_miss        ),
    .biudata_valid   (icbiu_ack_i         ),
    .biudata_error   (icbiu_err_i         ),
    .start_addr      (icimmu_adr_i        ),
    .saved_addr      (saved_addr          ),
    .icram_we        (icram_we            ),
    .biu_read        (icfsm_biu_read      ),
    .first_hit_ack   (icfsm_first_hit_ack ),
    .first_miss_ack  (icfsm_first_miss_ack),
    .first_miss_err  (icfsm_first_miss_err),
    .burst           (icfsm_burst         ),
    .tag_we          (icfsm_tag_we        )
);
//////////////////////////////////////////////////////////////////////
// Instantiation of IC main memory
assign ic_ram_en   = ic_en && (icimmu_cycstb_i || icram_we);
assign ic_ram_din  = icbiu_dat_i;
assign ic_ram_addr = ic_addr[`OR1200_ICINDX_MFDH_MFD:3];
assign ic_ram_we   = icram_we || icbiu_abort_ack_i;
//////////////////////////////////////////////////////////////////////
// Simple assignments
assign ictag_we   = icfsm_tag_we || icbiu_abort_ack_i;
assign ictag_addr = ic_addr[`OR1200_ICINDX_MFDH_MFD:`OR1200_ICLS_MFD];
assign ictag_en   = ic_en && (icimmu_cycstb_i || ictag_we);
//////////////////////////////////////////////////////////////////////
// Instantiation of IC TAG memory
reg      [depth-1:0] r_tag_v;
integer              i;

always @(posedge clk or negedge rst_n)
if( !rst_n )
    for( i=0; i<depth; i=i+1)
        r_tag_v[i] <= 1'b0;
else if( ictag_en & ictag_we )
    r_tag_v[ictag_addr]  <=  1'b1;

always @(posedge clk or negedge rst_n)
if( !rst_n )
    tag_v  <= 1'b0;
else if( ictag_en )
    tag_v  <=  r_tag_v[ictag_addr];
///////////////////////////////////////////////////////
assign  ictag_sram_csn  = !ictag_en; 
assign  ictag_sram_wen  = !ictag_we; 
assign  ictag_sram_addr = ictag_addr; 
assign  ictag_sram_din  = ic_addr[`OR1200_IC_CI_INDXL_MFD-1:`OR1200_ICTAG_MFDL_MFD]; 
assign  tag = ictag_sram_dout;

//////////////////////////////////////////////////////////////////////

endmodule
