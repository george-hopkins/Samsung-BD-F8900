//////////////////////////////////////////////////////////////////////
////                                  ////
////  OR1200's Write-back Mux                     ////
////                                  ////
////  This file is part of the OpenRISC 1200 project          ////
////  http://www.opencores.org/cores/or1k/            ////
////                                  ////
////  Description                         ////
////  CPU's write-back stage of the pipeline              ////
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

// Revision 1.2  2002/03/29 15:16:56  lampret
// Some of the warnings fixed.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.8  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.7  2001/10/14 13:12:10  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:36  igorm
// no message
//
// Revision 1.2  2001/08/09 13:39:33  lampret
// Major clean-up.
//
// Revision 1.1  2001/07/20 00:46:23  lampret
// Development version of RTL. Libraries are missing.
//
//

module or1200_wbmux_mfd(
    // Clock and reset
    clk, rst_n,

    // Internal i/f
    wb_freeze, rfwb_op,
    muxin_a, muxin_b, muxin_c, muxin_d,
    muxout, muxreg, muxreg_valid
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
// Internal i/f
//
input                wb_freeze;
input    [`OR1200_RFWBOP_WIDTH_MFD-1:0]    rfwb_op;
input    [width-1:0] muxin_a;
input    [width-1:0] muxin_b;
input    [width-1:0] muxin_c;
input    [width-1:0] muxin_d;
output   [width-1:0] muxout;
output   [width-1:0] muxreg;
output               muxreg_valid;

//
// Internal wires and regs
//
reg      [width-1:0] muxout;
reg      [width-1:0] muxreg;
reg                  muxreg_valid;

//
// Registered output from the write-back multiplexer
//
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        muxreg <=  32'd0;
        muxreg_valid <=  1'b0;
    end
    else if (!wb_freeze) begin
        muxreg <=  muxout;
        muxreg_valid <=  rfwb_op[0];
    end
end

//
// Write-back multiplexer
//
always @( * )
begin : b_wbmux_0
/* synopsys resource r0 : map_to_module = "DW01_add", implementation = "csm", ops = "ADD_8"; */
`ifdef OR1200_ADDITIONAL_SYNOPSYS_DIRECTIVES_MFD
    case(rfwb_op[`OR1200_RFWBOP_WIDTH_MFD-1:1]) // synopsys parallel_case infer_mux
`else
    case(rfwb_op[`OR1200_RFWBOP_WIDTH_MFD-1:1]) // synopsys parallel_case
`endif
        2'b00: muxout = muxin_a;
        2'b01: begin
            muxout = muxin_b;
        end
        2'b10: begin
            muxout = muxin_c;
        end
        2'b11: begin
            muxout = muxin_d + 4'h8;    // synopsys label ADD_8
        end
    endcase
end

endmodule
