//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's register file inside CPU                           ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  Instantiation of register file memories                     ////
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

// Revision 1.2  2002/06/08 16:19:09  lampret
// Added generic flip-flop based memory macro instantiation.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.13  2001/11/20 18:46:15  simons
// Break point bug fixed
//
// Revision 1.12  2001/11/13 10:02:21  lampret
// Added 'setpc'. Renamed some signals (except_flushpipe into flushpipe etc)
//
// Revision 1.11  2001/11/12 01:45:40  lampret
// Moved flag bit into SR. Changed RF enable from constant enable to dynamic enable for read ports.
//
// Revision 1.10  2001/11/10 03:43:57  lampret
// Fixed exceptions.
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

module or1200_rf_mfd(
    // Clock and reset
    clk, rst_n,

    // Write i/f
    supv, wb_freeze, addrw, dataw, we, flushpipe,

    // Read i/f
    id_freeze, addra, addrb, dataa, datab, rda, rdb,

    // Debug
    spr_cs, spr_write, spr_addr, spr_dat_i, spr_dat_o
);

parameter dw = `OR1200_OPERAND_WIDTH_MFD;
parameter aw = `OR1200_REGFILE_ADDR_WIDTH_MFD;

//
// I/O
//

//
// Clock and reset
//
input                clk;
input                rst_n;

//
// Write i/f
//
input                supv;
input                wb_freeze;
input    [aw-1:0]    addrw;
input    [dw-1:0]    dataw;
input                we;
input                flushpipe;

//
// Read i/f
//
input                id_freeze;
input    [aw-1:0]    addra;
input    [aw-1:0]    addrb;
output   [dw-1:0]    dataa;
output   [dw-1:0]    datab;
input                rda;
input                rdb;

//
// SPR access for debugging purposes
//
input                spr_cs;
input                spr_write;
input    [31:0]      spr_addr;
input    [31:0]      spr_dat_i;
output   [31:0]      spr_dat_o;

//
// Internal wires and regs
//
wire    [dw-1:0]     from_rfa;
wire    [dw-1:0]     from_rfb;
reg     [dw:0]       dataa_saved;
reg     [dw:0]       datab_saved;
wire    [aw-1:0]     rf_addra;
wire    [aw-1:0]     rf_addrw;
wire    [dw-1:0]     rf_dataw;
wire                 rf_we;
wire                 spr_valid;
wire                 rf_ena;
wire                 rf_enb;
reg                  rf_we_allow;

//
// SPR access is valid when spr_cs is asserted and
// SPR address matches GPR addresses
//
assign spr_valid = spr_cs & (spr_addr[10:5] == `OR1200_SPR_RF_MFD);

//
// SPR data output is always from RF A
//
assign spr_dat_o = from_rfa;

//
// Operand A comes from RF or from saved A register
//
assign dataa = (dataa_saved[32]) ? dataa_saved[31:0] : from_rfa;

//
// Operand B comes from RF or from saved B register
//
assign datab = (datab_saved[32]) ? datab_saved[31:0] : from_rfb;

//
// RF A read address is either from SPRS or normal from CPU control
//
assign rf_addra = (spr_valid & !spr_write) ? spr_addr[4:0] : addra;

//
// RF write address is either from SPRS or normal from CPU control
//
assign rf_addrw = (spr_valid & spr_write) ? spr_addr[4:0] : addrw;

//
// RF write data is either from SPRS or normal from CPU datapath
//
assign rf_dataw = (spr_valid & spr_write) ? spr_dat_i : dataw;

//
// RF write enable is either from SPRS or normal from CPU control
//
always @(negedge rst_n or posedge clk)
    if (!rst_n)
        rf_we_allow <=  1'b1;
    else if (~wb_freeze)
        rf_we_allow <=  ~flushpipe;

assign rf_we = ((spr_valid & spr_write) | (we & ~wb_freeze)) & rf_we_allow & (supv | (|rf_addrw));

//
// CS RF A asserted when instruction reads operand A and ID stage
// is not stalled
//
assign rf_ena = rda & ~id_freeze | spr_valid;    // probably works with fixed binutils
// assign rf_ena = 1'b1;            // does not work with single-stepping
//assign rf_ena = ~id_freeze | spr_valid;    // works with broken binutils 

//
// CS RF B asserted when instruction reads operand B and ID stage
// is not stalled
//
assign rf_enb = rdb & ~id_freeze | spr_valid;
// assign rf_enb = 1'b1;
//assign rf_enb = ~id_freeze | spr_valid;    // works with broken binutils 

//
// Stores operand from RF_A into temp reg when pipeline is frozen
//
always @(posedge clk or negedge rst_n)
    if (!rst_n) begin
        dataa_saved <=  33'b0;
    end
    else if (id_freeze & !dataa_saved[32]) begin
        dataa_saved <=  {1'b1, from_rfa};
    end
    else if (!id_freeze)
        dataa_saved <=  33'b0;

//
// Stores operand from RF_B into temp reg when pipeline is frozen
//
always @(posedge clk or negedge rst_n)
    if (!rst_n) begin
        datab_saved <=  33'b0;
    end
    else if (id_freeze & !datab_saved[32]) begin
        datab_saved <=  {1'b1, from_rfb};
    end
    else if (!id_freeze)
        datab_saved <=  33'b0;

`ifdef OR1200_RFRAM_DUALPORT_MFD

//
// Instantiation of register file two-port RAM A
//
or1200_dpram_32x32_mfd rf_a(
    // Port A
    .clk_a(clk),
    .rst_a(rst_n),
    .ce_a(rf_ena),
    .oe_a(1'b1),
    .addr_a(rf_addra),
    .do_a(from_rfa),

    // Port B
    .clk_b(clk),
    .rst_b(rst_n),
    .ce_b(rf_we),
    .we_b(rf_we),
    .addr_b(rf_addrw),
    .di_b(rf_dataw)
);

//
// Instantiation of register file two-port RAM B
//
or1200_dpram_32x32_mfd rf_b(
    // Port A
    .clk_a(clk),
    .rst_a(rst_n),
    .ce_a(rf_enb),
    .oe_a(1'b1),
    .addr_a(addrb),
    .do_a(from_rfb),

    // Port B
    .clk_b(clk),
    .rst_b(rst_n),
    .ce_b(rf_we),
    .we_b(rf_we),
    .addr_b(rf_addrw),
    .di_b(rf_dataw)
);

`else
//
// Instantiation of generic (flip-flop based) register file
//
or1200_rfram_generic_mfd rf_a(
    // Clock and reset
    .clk(clk),
    .rst_n(rst_n),

    // Port A
    .ce_a(rf_ena),
    .addr_a(rf_addra),
    .do_a(from_rfa),

    // Port B
    .ce_b(rf_enb),
    .addr_b(addrb),
    .do_b(from_rfb),

    // Port W
    .ce_w(rf_we),
    .we_w(rf_we),
    .addr_w(rf_addrw),
    .di_w(rf_dataw)
);
/*
reg     [aw-1:0]     rd1_addr; 
reg     [aw-1:0]     rd2_addr; 
always @(posedge clk or negedge rst_n)
if( !rst_n )
begin
   rd1_addr <=  0;
   rd2_addr <=  0;
end
else
begin
   rd1_addr <=  rf_addra;
   rd2_addr <=  addrb;
end
DW_ram_2r_w_s_dff #(32, 32, 0) rf_a(
    .clk           (clk         ),
    .rst_n         (rst_n       ),
    .cs_n          (~rf_ena & ~rf_enb & ~rf_we), 
    .wr_n          (~rf_we      ),  
    .rd1_addr      (rd1_addr    ), 
    .rd2_addr      (rd2_addr    ),
    .wr_addr       (rf_addrw    ),  
    .data_in       (rf_dataw    ),  
    .data_rd1_out  (from_rfa    ),
    .data_rd2_out  (from_rfb    )
);
*/
`endif

endmodule
