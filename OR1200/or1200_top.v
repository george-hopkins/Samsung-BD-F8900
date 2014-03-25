//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200 Top Level                                            ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  OR1200 Top Level                                            ////
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

// Revision 1.10  2002/12/08 08:57:56  lampret
// Added optional support for WB B3 specification (xwb_cti_o, xwb_bte_o). Made xwb_cab_o optional.
//
// Revision 1.9  2002/10/17 20:04:41  lampret
// Added BIST scan. Special VS RAMs need to be used to implement BIST.
//
// Revision 1.8  2002/08/18 19:54:22  lampret
// Added store buffer.
//
// Revision 1.7  2002/07/14 22:17:17  lampret
// Added simple trace buffer [only for Xilinx Virtex target]. Fixed instruction fetch abort when new exception is recognized.
//
// Revision 1.6  2002/03/29 15:16:56  lampret
// Some of the warnings fixed.
//
// Revision 1.5  2002/02/11 04:33:17  lampret
// Speed optimizations (removed duplicate _cyc_ and _stb_). Fixed D/IMMU cache-inhibit attr.
//
// Revision 1.4  2002/02/01 19:56:55  lampret
// Fixed combinational loops.
//
// Revision 1.3  2002/01/28 01:16:00  lampret
// Changed 'void' nop-ops instead of insn[0] to use insn[16]. Debug unit stalls the tick timer. Prepared new flag generation for add and and insns. Blocked DC/IC while they are turned off. Fixed I/D MMU SPRs layout except WAYs. TODO: smart IC invalidate, l.j 2 and TLB ways.
//
// Revision 1.2  2002/01/18 07:56:00  lampret
// No more low/high priority interrupts (PICPR removed). Added tick timer exception. Added exception prefix (SR[EPH]). Fixed single-step bug whenreading NPC.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.13  2001/11/23 08:38:51  lampret
// Changed DSR/DRR behavior and exception detection.
//
// Revision 1.12  2001/11/20 00:57:22  lampret
// Fixed width of du_except.
//
// Revision 1.11  2001/11/18 08:36:28  lampret
// For GDB changed single stepping and disabled trap exception.
//
// Revision 1.10  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.9  2001/10/14 13:12:10  lampret
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
// Revision 1.2  2001/07/22 03:31:54  lampret
// Fixed RAM's oen bug. Cache bypass under development.
//
// Revision 1.1  2001/07/20 00:46:21  lampret
// Development version of RTL. Libraries are missing.
//
//

module or1200_top_mfd(

    // System
    clk, rst_n, pic_ints_i, 

    // Instruction WISHBONE INTERFACE
    iwb_ack_i, iwb_abort_ack_i, iwb_abort_adr_i, iwb_err_i, iwb_dat_i,
    iwb_cyc_o, iwb_adr_o, 
    ic_en,

    // Data WISHBONE INTERFACE
    dwb_ack_i, dwb_err_i, dwb_dat_i,
    dwb_cyc_o, dwb_adr_o, dwb_we_o, dwb_sel_o, dwb_dat_o,
    dc_en,
    
    // Sram I/F
    ic_ram_dout, ic_ram_din, ic_ram_en, ic_ram_we, ic_ram_addr,
    dc_ram_dout, dc_ram_din, dc_ram_en, dc_ram_we, dc_ram_addr,
    ictag_sram_addr, ictag_sram_csn, ictag_sram_wen, ictag_sram_din, ictag_sram_dout,
    dctag_sram_addr, dctag_sram_csn, dctag_sram_wen, dctag_sram_din, dctag_sram_dout,

    // For Debugging
    if_pc, ex_pc, if_insn, ex_insn, epcr
);
//////////////////////////////////////////////////////////////////////
parameter ictag_dw = `OR1200_IC_CI_INDXL_MFD-1-`OR1200_ICTAG_MFDL_MFD+1;
parameter ictag_aw = `OR1200_ICTAG_MFD;
parameter dctag_dw = `OR1200_DC_CI_INDXL_MFD-1-`OR1200_DCTAG_MFDL_MFD+1;
parameter dctag_aw = `OR1200_DCTAG_MFD;
parameter ppic_ints = `OR1200_PIC_INTS_MFD;
//////////////////////////////////////////////////////////////////////
// I/O
//////////////////////////////////////////////////////////////////////
// System
input                  clk;
input                  rst_n;
input  [ppic_ints-1:0] pic_ints_i;
///////////////////////////////////////////////////////
// Instruction WISHBONE interface
input            iwb_ack_i;    // normal termination
input            iwb_abort_ack_i; 
input  [16:0]    iwb_abort_adr_i; 
input            iwb_err_i;    // termination w/ error
input   [63:0]   iwb_dat_i;    // input data bus
output           iwb_cyc_o;    // cycle valid output
output  [31:0]   iwb_adr_o;    // address bus outputs
output           ic_en;
///////////////////////////////////////////////////////
// Data WISHBONE interface
input            dwb_ack_i;    // normal termination
input            dwb_err_i;    // termination w/ error
input   [63:0]   dwb_dat_i;    // input data bus(64bit interface)
output           dwb_cyc_o;    // cycle valid output
output  [31:0]   dwb_adr_o;    // address bus outputs
output           dwb_we_o;     // indicates write transfer
output  [3:0]    dwb_sel_o;    // byte select outputs
output  [31:0]   dwb_dat_o;    // output data bus(32bit interface)
output           dc_en;
///////////////////////////////////////////////////////
// Sram I/F
input   [63:0]    ic_ram_dout;
output            ic_ram_en;
output            ic_ram_we;
output  [63:0]    ic_ram_din;
output  [`OR1200_ICINDX_MFD-2:0] ic_ram_addr;
input   [63:0]    dc_ram_dout;
output            dc_ram_en;
output  [7:0]     dc_ram_we;
output  [63:0]    dc_ram_din;
output  [`OR1200_DCINDX_MFD-2:0] dc_ram_addr;
///////////////////////////////////////////////////////
output  [ictag_aw-1:0] ictag_sram_addr;
output                 ictag_sram_csn;
output                 ictag_sram_wen;
output  [ictag_dw-1:0] ictag_sram_din;
input   [ictag_dw-1:0] ictag_sram_dout;
output  [dctag_aw-1:0] dctag_sram_addr;
output                 dctag_sram_csn;
output                 dctag_sram_wen;
output  [dctag_dw-1:0] dctag_sram_din;
input   [dctag_dw-1:0] dctag_sram_dout;
///////////////////////////////////////////////////////
// For Debugging
output  [31:0]  if_pc;
output  [31:0]  ex_pc;
output  [31:0]  if_insn;
output  [31:0]  ex_insn;
output  [31:0]  epcr;
//////////////////////////////////////////////////////////////////////
// CPU's SPR access to various RISC units (shared wires)
wire            supv;
wire    [31:0]  spr_addr;
wire    [31:0]  spr_dat_cpu;
wire    [31:0]  spr_cs;
wire            spr_we;
///////////////////////////////////////////////////////
// DMMU and CPU
wire    [31:0]    spr_dat_dmmu;
///////////////////////////////////////////////////////
// CPU and data memory subsystem
wire              dc_en;
wire    [31:0]    dcpu_adr_cpu;
wire              dcpu_cycstb_cpu;
wire              dcpu_we_cpu;
wire    [3:0]     dcpu_sel_cpu;
wire    [3:0]     dcpu_tag_cpu;
wire    [31:0]    dcpu_dat_cpu;
wire    [31:0]    dcpu_dat_dc;
wire              dcpu_ack_dc;
wire              dcpu_rty_dc;
wire              dcpu_err_dc;
wire    [3:0]     dcpu_tag_dc;
///////////////////////////////////////////////////////
// IMMU and CPU
wire    [31:0]    spr_dat_immu;
///////////////////////////////////////////////////////
// CPU and insn memory subsystem
wire    [31:0]    icpu_adr_cpu;
wire    [31:0]    icpu_adr_ic;
wire    [31:0]    icpu_adr_immu;
wire              icpu_cycstb_cpu;
wire    [3:0]     icpu_sel_cpu;
wire    [3:0]     icpu_tag_cpu;
wire    [31:0]    icpu_dat_ic;
wire              icpu_ack_ic;
wire              icpu_err_ic;
wire    [3:0]     icpu_tag_ic;
wire              icpu_rty_ic;
///////////////////////////////////////////////////////
// Connection between CPU and PIC
wire    [31:0]    spr_dat_pic;
wire              pic_wakeup;
wire              sig_int;
//////////////////////////////////////////////////////////////////////
// Instantiation of Instruction Cache
//////////////////////////////////////////////////////////////////////
or1200_ic_top_mfd or1200_ic_top(

    .clk             (clk),
    .rst_n           (rst_n),

    // IC and CPU/IMMU
    .ic_en           (ic_en),
    
    .icimmu_adr_i    (icpu_adr_cpu),
    .icimmu_adr_o    (icpu_adr_ic),
    .icimmu_cycstb_i (icpu_cycstb_cpu),
    .icpu_tag_i      (icpu_tag_cpu),
    .icpu_dat_o      (icpu_dat_ic),
    .icpu_ack_o      (icpu_ack_ic),
    .icimmu_rty_o    (icpu_rty_ic),
    .icimmu_err_o    (icpu_err_ic),
    .icimmu_tag_o    (icpu_tag_ic),
   
    // SPR access
    .spr_cs          (spr_cs[`OR1200_SPR_GROUP_IC_MFD]),
    .spr_write       (spr_we),
    .spr_dat_i       (spr_dat_cpu),

    // IC and BIU
    .icbiu_adr_o     (iwb_adr_o),
    .icbiu_cyc_o     (iwb_cyc_o),
    .icbiu_dat_i     (iwb_dat_i),
    .icbiu_ack_i     (iwb_ack_i),
    .icbiu_abort_ack_i(iwb_abort_ack_i),
    .icbiu_abort_adr_i(iwb_abort_adr_i),
    .icbiu_err_i     (iwb_err_i),

    // Sram I/F
    .ic_ram_dout     (ic_ram_dout), 
    .ic_ram_din      (ic_ram_din), 
    .ic_ram_en       (ic_ram_en), 
    .ic_ram_we       (ic_ram_we), 
    .ic_ram_addr     (ic_ram_addr),
    .ictag_sram_addr (ictag_sram_addr), 
    .ictag_sram_csn  (ictag_sram_csn ), 
    .ictag_sram_wen  (ictag_sram_wen ), 
    .ictag_sram_din  (ictag_sram_din ), 
    .ictag_sram_dout (ictag_sram_dout)
);
//////////////////////////////////////////////////////////////////////
// Instantiation of CPU Core 
//////////////////////////////////////////////////////////////////////
or1200_cpu_mfd or1200_cpu(
    .clk            (clk),
    .rst_n          (rst_n),

    // Connection IC and IFETCHER inside CPU
    .ic_en          (ic_en),
    .icpu_adr_o     (icpu_adr_cpu),
    .icpu_cycstb_o  (icpu_cycstb_cpu),
    .icpu_sel_o     (icpu_sel_cpu),
    .icpu_tag_o     (icpu_tag_cpu),
    .icpu_dat_i     (icpu_dat_ic),
    .icpu_ack_i     (icpu_ack_ic),
    .icpu_adr_i     (icpu_adr_ic),
    .icpu_rty_i     (icpu_rty_ic),
    .icpu_err_i     (icpu_err_ic),
    .icpu_tag_i     (icpu_tag_ic),

    // Connection DC and CPU
    .dc_en          (dc_en),
    .dcpu_adr_o     (dcpu_adr_cpu),
    .dcpu_cycstb_o  (dcpu_cycstb_cpu),
    .dcpu_we_o      (dcpu_we_cpu),
    .dcpu_sel_o     (dcpu_sel_cpu),
    .dcpu_tag_o     (dcpu_tag_cpu),
    .dcpu_dat_o     (dcpu_dat_cpu),
    .dcpu_dat_i     (dcpu_dat_dc),
    .dcpu_ack_i     (dcpu_ack_dc),
    .dcpu_rty_i     (dcpu_rty_dc),
    .dcpu_err_i     (dcpu_err_dc),
    .dcpu_tag_i     (dcpu_tag_dc),
    
    // Connection PIC and CPU's EXCEPT
    .sig_int        (sig_int),
    .sig_tick       (1'b0),
    
    // SPRs
    .supv           (supv),
    .spr_addr       (spr_addr),
    .spr_dat_cpu    (spr_dat_cpu),
    .spr_dat_pic    (spr_dat_pic),
    .spr_dat_tt     (32'h0),
    .spr_dat_pm     (32'h0),
    
    .spr_dat_dmmu   (32'h0),
    .spr_dat_immu   (32'h0),
    .spr_cs         (spr_cs),
    .spr_we         (spr_we),

    // For Debugging
    .if_pc          (if_pc),
    .ex_pc          (ex_pc),
    .if_insn        (if_insn),
    .ex_insn        (ex_insn),
    .epcr           (epcr)
);
//////////////////////////////////////////////////////////////////////
// Instantiation of Data Cache
//////////////////////////////////////////////////////////////////////
or1200_dc_top_mfd or1200_dc_top(

    .clk            (clk),
    .rst_n          (rst_n),

    // DC and CPU/DMMU
    .dc_en          (dc_en),

    .dcdmmu_adr_i   (dcpu_adr_cpu),
    .dcdmmu_cycstb_i(dcpu_cycstb_cpu),
    .dcdmmu_ci_i    (`OR1200_DMMU_CI_MFD),
    .dcdmmu_err_o   (dcpu_err_dc),
    .dcdmmu_tag_o   (dcpu_tag_dc),

    .dcpu_we_i      (dcpu_we_cpu),
    .dcpu_sel_i     (dcpu_sel_cpu),
    .dcpu_tag_i     (dcpu_tag_cpu),
    .dcpu_dat_i     (dcpu_dat_cpu),
    .dcpu_dat_o     (dcpu_dat_dc),
    .dcpu_ack_o     (dcpu_ack_dc),
    .dcpu_rty_o     (dcpu_rty_dc),

    // SPR access
    .spr_cs         (spr_cs[`OR1200_SPR_GROUP_DC_MFD]),
    .spr_write      (spr_we),
    .spr_dat_i      (spr_dat_cpu),

    // DC and BIU
    .dcsb_dat_o     (dwb_dat_o),
    .dcsb_adr_o     (dwb_adr_o),
    .dcsb_cyc_o     (dwb_cyc_o),
    .dcsb_we_o      (dwb_we_o),
    .dcsb_sel_o     (dwb_sel_o),
    .dcsb_dat_i     (dwb_dat_i),
    .dcsb_ack_i     (dwb_ack_i),
    .dcsb_err_i     (dwb_err_i),

    // Sram I/F
    .dc_ram_dout    (dc_ram_dout), 
    .dc_ram_din     (dc_ram_din), 
    .dc_ram_en      (dc_ram_en), 
    .dc_ram_we      (dc_ram_we), 
    .dc_ram_addr    (dc_ram_addr),
    .dctag_sram_addr(dctag_sram_addr), 
    .dctag_sram_csn (dctag_sram_csn ), 
    .dctag_sram_wen (dctag_sram_wen ), 
    .dctag_sram_din (dctag_sram_din ), 
    .dctag_sram_dout(dctag_sram_dout)
);
//////////////////////////////////////////////////////////////////////
// Programmable interrupt controller
//////////////////////////////////////////////////////////////////////
or1200_pic_mfd or1200_pic(
    // RISC Internal Interface
    .clk          (clk),
    .rst_n        (rst_n),
    .spr_cs       (spr_cs[`OR1200_SPR_GROUP_PIC_MFD]),
    .spr_write    (spr_we),
    .spr_addr     (spr_addr),
    .spr_dat_i    (spr_dat_cpu),
    .spr_dat_o    (spr_dat_pic),
    .pic_wakeup   (pic_wakeup),
    .int          (sig_int), 

    // PIC Interface
    .pic_int      (pic_ints_i)
);
//////////////////////////////////////////////////////////////////////
endmodule
