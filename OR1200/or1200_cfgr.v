//////////////////////////////////////////////////////////////////////
////                                  ////
////  OR1200's VR, UPR and Configuration Registers        ////
////                                  ////
////  This file is part of the OpenRISC 1200 project          ////
////  http://www.opencores.org/cores/or1k/            ////
////                                  ////
////  Description                         ////
////  According to OR1K architectural and OR1200 specifications.  ////
////                                  ////
////  To Do:                              ////
////   - done                             ////
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

// Revision 1.3  2002/03/29 15:16:54  lampret
// Some of the warnings fixed.
//
// Revision 1.2  2002/01/14 06:18:22  lampret
// Fixed mem2reg bug in FAST implementation. Updated debug unit to work with new genpc/if.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.7  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.6  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:35  igorm
// no message
//
// Revision 1.1  2001/08/09 13:39:33  lampret
// Major clean-up.
//
// Revision 1.1  2001/07/20 00:46:21  lampret
// Development version of RTL. Libraries are missing.
//
//

module or1200_cfgr_mfd(
    // RISC Internal Interface
    spr_addr, spr_dat_o
);

//
// RISC Internal Interface
//
input    [31:0]    spr_addr;    // SPR Address
output   [31:0]    spr_dat_o;    // SPR Read Data

//
// Internal wires & registers
//
reg      [31:0]    spr_dat_o;    // SPR Read Data

`ifdef OR1200_CFGR_IMPLEMENTED_MFD

//
// Implementation of VR, UPR and configuration registers
//
`ifdef OR1200_SYS_FULL_DECODE_MFD
always @(spr_addr)
    if (!spr_addr[31:4])
`else
always @(spr_addr)
`endif

        case(spr_addr[3:0])        // synopsys parallel_case
            `OR1200_SPRGRP_SYS_VR_MFD: begin
                spr_dat_o[`OR1200_VR_REV_MFD_BITS_MFD] = `OR1200_VR_REV_MFD;
                spr_dat_o[`OR1200_VR_RES1_MFD_BITS_MFD] = `OR1200_VR_RES1_MFD;
                spr_dat_o[`OR1200_VR_CFG_MFD_BITS_MFD] = `OR1200_VR_CFG_MFD;
                spr_dat_o[`OR1200_VR_VER_MFD_BITS_MFD] = `OR1200_VR_VER_MFD;
            end
            `OR1200_SPRGRP_SYS_UPR_MFD: begin
                spr_dat_o[`OR1200_UPR_UP_MFD_BITS_MFD] = `OR1200_UPR_UP_MFD;
                spr_dat_o[`OR1200_UPR_DCP_MFD_BITS_MFD] = `OR1200_UPR_DCP_MFD;
                spr_dat_o[`OR1200_UPR_ICP_MFD_BITS_MFD] = `OR1200_UPR_ICP_MFD;
                spr_dat_o[`OR1200_UPR_DMP_MFD_BITS_MFD] = `OR1200_UPR_DMP_MFD;
                spr_dat_o[`OR1200_UPR_IMP_MFD_BITS_MFD] = `OR1200_UPR_IMP_MFD;
                spr_dat_o[`OR1200_UPR_MP_MFD_BITS_MFD] = `OR1200_UPR_MP_MFD;
                spr_dat_o[`OR1200_UPR_DUP_MFD_BITS_MFD] = `OR1200_UPR_DUP_MFD;
                spr_dat_o[`OR1200_UPR_PCUP_MFD_BITS_MFD] = `OR1200_UPR_PCUP_MFD;
                spr_dat_o[`OR1200_UPR_PMP_MFD_BITS_MFD] = `OR1200_UPR_PMP_MFD;
                spr_dat_o[`OR1200_UPR_PICP_MFD_BITS_MFD] = `OR1200_UPR_PICP_MFD;
                spr_dat_o[`OR1200_UPR_TTP_MFD_BITS_MFD] = `OR1200_UPR_TTP_MFD;
                spr_dat_o[`OR1200_UPR_RES1_MFD_BITS_MFD] = `OR1200_UPR_RES1_MFD;
                spr_dat_o[`OR1200_UPR_CUP_MFD_BITS_MFD] = `OR1200_UPR_CUP_MFD;
            end
            `OR1200_SPRGRP_SYS_CPUCFGR_MFD: begin
                spr_dat_o[`OR1200_CPUCFGR_NSGF_MFD_BITS_MFD] = `OR1200_CPUCFGR_NSGF_MFD;
                spr_dat_o[`OR1200_CPUCFGR_HGF_MFD_BITS_MFD] = `OR1200_CPUCFGR_HGF_MFD;
                spr_dat_o[`OR1200_CPUCFGR_OB32S_MFD_BITS_MFD] = `OR1200_CPUCFGR_OB32S_MFD;
                spr_dat_o[`OR1200_CPUCFGR_OB64S_MFD_BITS_MFD] = `OR1200_CPUCFGR_OB64S_MFD;
                spr_dat_o[`OR1200_CPUCFGR_OF32S_MFD_BITS_MFD] = `OR1200_CPUCFGR_OF32S_MFD;
                spr_dat_o[`OR1200_CPUCFGR_OF64S_MFD_BITS_MFD] = `OR1200_CPUCFGR_OF64S_MFD;
                spr_dat_o[`OR1200_CPUCFGR_OV64S_MFD_BITS_MFD] = `OR1200_CPUCFGR_OV64S_MFD;
                spr_dat_o[`OR1200_CPUCFGR_RES1_MFD_BITS_MFD] = `OR1200_CPUCFGR_RES1_MFD;
            end
            `OR1200_SPRGRP_SYS_DMMUCFGR_MFD: begin
                spr_dat_o[`OR1200_DMMUCFGR_NTW_MFD_BITS_MFD] = `OR1200_DMMUCFGR_NTW_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_NTS_MFD_BITS_MFD] = `OR1200_DMMUCFGR_NTS_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_NAE_MFD_BITS_MFD] = `OR1200_DMMUCFGR_NAE_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_CRI_MFD_BITS_MFD] = `OR1200_DMMUCFGR_CRI_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_PRI_MFD_BITS_MFD] = `OR1200_DMMUCFGR_PRI_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_TEIRI_MFD_BITS_MFD] = `OR1200_DMMUCFGR_TEIRI_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_HTR_MFD_BITS_MFD] = `OR1200_DMMUCFGR_HTR_MFD;
                spr_dat_o[`OR1200_DMMUCFGR_RES1_MFD_BITS_MFD] = `OR1200_DMMUCFGR_RES1_MFD;
            end
            `OR1200_SPRGRP_SYS_IMMUCFGR_MFD: begin
                spr_dat_o[`OR1200_IMMUCFGR_NTW_MFD_BITS_MFD] = `OR1200_IMMUCFGR_NTW_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_NTS_MFD_BITS_MFD] = `OR1200_IMMUCFGR_NTS_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_NAE_MFD_BITS_MFD] = `OR1200_IMMUCFGR_NAE_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_CRI_MFD_BITS_MFD] = `OR1200_IMMUCFGR_CRI_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_PRI_MFD_BITS_MFD] = `OR1200_IMMUCFGR_PRI_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_TEIRI_MFD_BITS_MFD] = `OR1200_IMMUCFGR_TEIRI_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_HTR_MFD_BITS_MFD] = `OR1200_IMMUCFGR_HTR_MFD;
                spr_dat_o[`OR1200_IMMUCFGR_RES1_MFD_BITS_MFD] = `OR1200_IMMUCFGR_RES1_MFD;
            end
            `OR1200_SPRGRP_SYS_DCCFGR_MFD: begin
                spr_dat_o[`OR1200_DCCFGR_NCW_MFD_BITS_MFD] = `OR1200_DCCFGR_NCW_MFD;
                spr_dat_o[`OR1200_DCCFGR_NCS_MFD_BITS_MFD] = `OR1200_DCCFGR_NCS_MFD;
                spr_dat_o[`OR1200_DCCFGR_CBS_MFD_BITS_MFD] = `OR1200_DCCFGR_CBS_MFD;
                spr_dat_o[`OR1200_DCCFGR_CWS_MFD_BITS_MFD] = `OR1200_DCCFGR_CWS_MFD;
                spr_dat_o[`OR1200_DCCFGR_CCRI_MFD_BITS_MFD] = `OR1200_DCCFGR_CCRI_MFD;
                spr_dat_o[`OR1200_DCCFGR_CBIRI_MFD_BITS_MFD] = `OR1200_DCCFGR_CBIRI_MFD;
                spr_dat_o[`OR1200_DCCFGR_CBPRI_MFD_BITS_MFD] = `OR1200_DCCFGR_CBPRI_MFD;
                spr_dat_o[`OR1200_DCCFGR_CBLRI_MFD_BITS_MFD] = `OR1200_DCCFGR_CBLRI_MFD;
                spr_dat_o[`OR1200_DCCFGR_CBFRI_MFD_BITS_MFD] = `OR1200_DCCFGR_CBFRI_MFD;
                spr_dat_o[`OR1200_DCCFGR_CBWBRI_MFD_BITS_MFD] = `OR1200_DCCFGR_CBWBRI_MFD;
                spr_dat_o[`OR1200_DCCFGR_RES1_MFD_BITS_MFD] = `OR1200_DCCFGR_RES1_MFD;
            end
            `OR1200_SPRGRP_SYS_ICCFGR_MFD: begin
                spr_dat_o[`OR1200_ICCFGR_NCW_MFD_BITS_MFD] = `OR1200_ICCFGR_NCW_MFD;
                spr_dat_o[`OR1200_ICCFGR_NCS_MFD_BITS_MFD] = `OR1200_ICCFGR_NCS_MFD;
                spr_dat_o[`OR1200_ICCFGR_CBS_MFD_BITS_MFD] = `OR1200_ICCFGR_CBS_MFD;
                spr_dat_o[`OR1200_ICCFGR_CWS_MFD_BITS_MFD] = `OR1200_ICCFGR_CWS_MFD;
                spr_dat_o[`OR1200_ICCFGR_CCRI_MFD_BITS_MFD] = `OR1200_ICCFGR_CCRI_MFD;
                spr_dat_o[`OR1200_ICCFGR_CBIRI_MFD_BITS_MFD] = `OR1200_ICCFGR_CBIRI_MFD;
                spr_dat_o[`OR1200_ICCFGR_CBPRI_MFD_BITS_MFD] = `OR1200_ICCFGR_CBPRI_MFD;
                spr_dat_o[`OR1200_ICCFGR_CBLRI_MFD_BITS_MFD] = `OR1200_ICCFGR_CBLRI_MFD;
                spr_dat_o[`OR1200_ICCFGR_CBFRI_MFD_BITS_MFD] = `OR1200_ICCFGR_CBFRI_MFD;
                spr_dat_o[`OR1200_ICCFGR_CBWBRI_MFD_BITS_MFD] = `OR1200_ICCFGR_CBWBRI_MFD;
                spr_dat_o[`OR1200_ICCFGR_RES1_MFD_BITS_MFD] = `OR1200_ICCFGR_RES1_MFD;
            end
            `OR1200_SPRGRP_SYS_DCFGR_MFD: begin
                spr_dat_o[`OR1200_DCFGR_NDP_MFD_BITS_MFD] = `OR1200_DCFGR_NDP_MFD;
                spr_dat_o[`OR1200_DCFGR_WPCI_MFD_BITS_MFD] = `OR1200_DCFGR_WPCI_MFD;
                spr_dat_o[`OR1200_DCFGR_RES1_MFD_BITS_MFD] = `OR1200_DCFGR_RES1_MFD;
            end
            default: spr_dat_o = 32'h0000_0000;
        endcase
`ifdef OR1200_SYS_FULL_DECODE_MFD
    else
        spr_dat_o = 32'h0000_0000;
`endif

`else

//
// When configuration registers are not implemented, only
// implement VR and UPR
//
`ifdef OR1200_SYS_FULL_DECODE_MFD
always @(spr_addr)
    if (!spr_addr[31:4])
`else
always @( * )
`endif
        case(spr_addr[3:0])
            `OR1200_SPRGRP_SYS_VR_MFD: begin
                spr_dat_o[`OR1200_VR_REV_MFD_BITS_MFD] = `OR1200_VR_REV_MFD;
                spr_dat_o[`OR1200_VR_RES1_MFD_BITS_MFD] = `OR1200_VR_RES1_MFD;
                spr_dat_o[`OR1200_VR_CFG_MFD_BITS_MFD] = `OR1200_VR_CFG_MFD;
                spr_dat_o[`OR1200_VR_VER_MFD_BITS_MFD] = `OR1200_VR_VER_MFD;
            end
            `OR1200_SPRGRP_SYS_UPR_MFD: begin
                spr_dat_o[`OR1200_UPR_UP_MFD_BITS_MFD] = `OR1200_UPR_UP_MFD;
                spr_dat_o[`OR1200_UPR_DCP_MFD_BITS_MFD] = `OR1200_UPR_DCP_MFD;
                spr_dat_o[`OR1200_UPR_ICP_MFD_BITS_MFD] = `OR1200_UPR_ICP_MFD;
                spr_dat_o[`OR1200_UPR_DMP_MFD_BITS_MFD] = `OR1200_UPR_DMP_MFD;
                spr_dat_o[`OR1200_UPR_IMP_MFD_BITS_MFD] = `OR1200_UPR_IMP_MFD;
                spr_dat_o[`OR1200_UPR_MP_MFD_BITS_MFD] = `OR1200_UPR_MP_MFD;
                spr_dat_o[`OR1200_UPR_DUP_MFD_BITS_MFD] = `OR1200_UPR_DUP_MFD;
                spr_dat_o[`OR1200_UPR_PCUP_MFD_BITS_MFD] = `OR1200_UPR_PCUP_MFD;
                spr_dat_o[`OR1200_UPR_PMP_MFD_BITS_MFD] = `OR1200_UPR_PMP_MFD;
                spr_dat_o[`OR1200_UPR_PICP_MFD_BITS_MFD] = `OR1200_UPR_PICP_MFD;
                spr_dat_o[`OR1200_UPR_TTP_MFD_BITS_MFD] = `OR1200_UPR_TTP_MFD;
                spr_dat_o[`OR1200_UPR_RES1_MFD_BITS_MFD] = `OR1200_UPR_RES1_MFD;
                spr_dat_o[`OR1200_UPR_CUP_MFD_BITS_MFD] = `OR1200_UPR_CUP_MFD;
            end
            default: spr_dat_o = 32'h0000_0000;
        endcase
`ifdef OR1200_SYS_FULL_DECODE_MFD
    else
        spr_dat_o = 32'h0000_0000;
`endif

`endif

endmodule
