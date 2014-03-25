//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's IC FSM                                             ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  Data cache state machine                                    ////
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

// Revision 1.7  2002/03/29 15:16:55  lampret
// Some of the warnings fixed.
//
// Revision 1.6  2002/03/28 19:10:40  lampret
// Optimized cache controller FSM.
//
// Revision 1.1.1.1  2002/03/21 16:55:45  lampret
// First import of the "new" XESS XSV environment.
//
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
// Revision 1.9  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from ic.v and ic.v. Fixed CR+LF.
//
// Revision 1.8  2001/10/19 23:28:46  lampret
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

`define OR1200_ICFSM_IDLE_MFD    4'b0001
`define OR1200_ICFSM_CFETCH_MFD    4'b0010
`define OR1200_ICFSM_LREFILL3_MFD    4'b0100
`define OR1200_ICFSM_IFETCH_MFD    4'b1000
`define OR1200_ICFSM_IDLE_MFD_BIT_MFD        0
`define OR1200_ICFSM_CFETCH_MFD_BIT_MFD        1
`define OR1200_ICFSM_LREFILL3_MFD_BIT_MFD    2
`define OR1200_ICFSM_IFETCH_MFD_BIT_MFD        3

//
// Data cache FSM for cache line of 16 bytes (4x singleword)
//

module or1200_ic_fsm_mfd(
    // Clock and reset
    clk, rst_n,

    // Internal i/f to top level IC
    ic_en, icimmu_cycstb_i, icimmu_ci_i,
    tagcomp_miss, biudata_valid, biudata_error, start_addr, saved_addr,
    icram_we, biu_read, first_hit_ack, first_miss_ack, first_miss_err,
    burst, tag_we
);

//
// I/O
//
input                clk;
input                rst_n;
input                ic_en;
input                icimmu_cycstb_i;
input                icimmu_ci_i;
input                tagcomp_miss;
input                biudata_valid;
input                biudata_error;
input    [31:0]            start_addr;
output    [31:0]            saved_addr;
output                     icram_we;
output                biu_read;
output                first_hit_ack;
output                first_miss_ack;
output                first_miss_err;
output                burst;
output                tag_we;

//
// Internal wires and regs
//
reg    [31:0]            saved_addr_r;
reg    [3:0]            state;
reg    [2:0]            cnt;
reg                hitmiss_eval;
reg                load;
reg                cache_inhibit;

reg                addr_tran; //address transition detected jtong
reg                biu_read_lat, biu_read_dly;
//
// Generate of ICRAM write enables
//
assign icram_we = {load & biudata_valid & !cache_inhibit};
assign tag_we = biu_read & biudata_valid & !cache_inhibit;

//
// BIU read and write
//
assign biu_read = (hitmiss_eval & tagcomp_miss) | (!hitmiss_eval & load);

//assign saved_addr = hitmiss_eval ? start_addr : saved_addr_r;
assign saved_addr = saved_addr_r;

//
// Assert for cache hit first word ready
// Assert for cache miss first word stored/loaded OK
// Assert for cache miss first word stored/loaded with an error
//
assign first_hit_ack = ((state[`OR1200_ICFSM_CFETCH_MFD_BIT_MFD] == 1'b1) & hitmiss_eval) & !tagcomp_miss;
assign first_miss_ack = (state[`OR1200_ICFSM_CFETCH_MFD_BIT_MFD] == 1'b1) & biudata_valid&!addr_tran;
assign first_miss_err = (state[`OR1200_ICFSM_CFETCH_MFD_BIT_MFD] == 1'b1) & biudata_error;

//
// Assert burst when doing reload of complete cache line
//
assign burst = (state[`OR1200_ICFSM_CFETCH_MFD_BIT_MFD] == 1'b1) & tagcomp_miss & !cache_inhibit
        | (state[`OR1200_ICFSM_LREFILL3_MFD_BIT_MFD] == 1'b1);

//jtong, added for address transition problem
wire    addr_tran_ld = biu_read_lat && (start_addr != saved_addr_r); 
wire   addr_tran_clr  = addr_tran &&  (state[`OR1200_ICFSM_CFETCH_MFD_BIT_MFD] == 1'b1) & biudata_valid || !biu_read;
//flexible cache line size
wire[5:0]    csize = (6'd1 << (`OR1200_ICLS_MFD-3));
wire[31:3]   csize_mius = {23'd0,csize} - 29'd1,
        saved_addr_r_plus = saved_addr_r[31:3] + 1,
        saved_addr_r_n = saved_addr_r[31:3] & ~csize_mius  | saved_addr_r_plus & csize_mius;
always @(posedge clk or negedge rst_n) 
    if (!rst_n) 
    begin
        addr_tran    <= 1'b0;
        biu_read_lat    <= 1'b0;
        biu_read_dly    <= 1'b0;
    end
    else
    begin
        biu_read_dly    <= biu_read;
        if (biu_read && !biu_read_dly)
        biu_read_lat    <= 1'b1;
        else
        if (biudata_valid || biudata_error || !biu_read)
        biu_read_lat    <= 1'b0;

        if (addr_tran_clr) addr_tran       <= 1'b0;
        else
        if (addr_tran_ld) addr_tran       <= 1'b1;
    end
//
// Main IC FSM
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        state <=  `OR1200_ICFSM_IDLE_MFD;
        saved_addr_r <=  32'b0;
        hitmiss_eval <=  1'b0;
        load <=  1'b0;
        cnt <=  3'b000;
        cache_inhibit <=  1'b0;
    end
    else
    case (state)
        `OR1200_ICFSM_IDLE_MFD :
            if (ic_en & icimmu_cycstb_i) begin        // fetch
                state <=  `OR1200_ICFSM_CFETCH_MFD;
                saved_addr_r <=  start_addr;
                hitmiss_eval <=  1'b1;
                load <=  1'b1;
                cache_inhibit <=  1'b0;
            end
            else begin                            // idle
                hitmiss_eval <=  1'b0;
                load <=  1'b0;
                cache_inhibit <=  1'b0;
            end
        `OR1200_ICFSM_CFETCH_MFD: begin    // fetch
            if (icimmu_cycstb_i & icimmu_ci_i & !biu_read)
                cache_inhibit <=  1'b1;
            if (hitmiss_eval )
//                saved_addr_r[31:13] <=  start_addr[31:13];
                saved_addr_r[31:2] <=  start_addr[31:2];

            if ((!ic_en) ||
                (hitmiss_eval & !icimmu_cycstb_i) ||    // fetch aborted (usually caused by IMMU)
                (biudata_error) ||                        // fetch terminated with an error
                (cache_inhibit & biudata_valid)) begin    // fetch from cache-inhibited page
                state <=  `OR1200_ICFSM_IDLE_MFD;
                hitmiss_eval <=  1'b0;
                load <=  1'b0;
                cache_inhibit <=  1'b0;
            end
            //else if (tagcomp_miss & && biudata_valid) begin    // fetch missed, finish current external fetch and refill
            else if (!hitmiss_eval && biudata_valid) begin //jtong
                state <=  `OR1200_ICFSM_LREFILL3_MFD;
                saved_addr_r[31:3] <=  saved_addr_r_n;
                hitmiss_eval <=  1'b0;
                cnt <=  csize[2:0] - 3'd2;
                cache_inhibit <=  1'b0;
            end
            else if (!tagcomp_miss & !icimmu_ci_i) begin    // fetch hit, finish immediately
                saved_addr_r <=  start_addr;
                cache_inhibit <=  1'b0;
            end
                        
            else if (!icimmu_cycstb_i) begin    // fetch aborted (usually caused by exception)
                state <=  `OR1200_ICFSM_IDLE_MFD;
                hitmiss_eval <=  1'b0;
                load <=  1'b0;
                cache_inhibit <=  1'b0;
            end
            else                        // fetch in-progress
            begin
                hitmiss_eval <=  1'b0;
            end
        end
        `OR1200_ICFSM_LREFILL3_MFD : begin
            if (biudata_valid && (|cnt)) begin        // refill ack, more fetchs to come
                cnt <=  cnt - 'd1;
                saved_addr_r[31:3] <=  saved_addr_r_n;
            end
            else if (biudata_valid) begin            // last fetch of line refill
                state <=  `OR1200_ICFSM_IDLE_MFD;
                saved_addr_r <=  start_addr;
                hitmiss_eval <=  1'b0;
                load <=  1'b0;
            end
        end
        default:
            state <=  `OR1200_ICFSM_IDLE_MFD;
    endcase
end

endmodule
