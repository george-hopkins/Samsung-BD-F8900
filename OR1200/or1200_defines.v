//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's definitions                                        ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/cores/or1k/                        ////
////                                                              ////
////  Description                                                 ////
////  Parameters of the OR1200 core                               ////
////                                                              ////
////  To Do:                                                      ////
////   - add parameters that are missing                          ////
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

// Revision 1.31  2002/12/08 08:57:56  lampret
// Added optional support for WB B3 specification (xwb_cti_o, xwb_bte_o). Made xwb_cab_o optional.
//
// Revision 1.30  2002/10/28 15:09:22  mohor
// Previous check-in was done by mistake.
//
// Revision 1.29  2002/10/28 15:03:50  mohor
// Signal scanb_sen renamed to scanb_en.
//
// Revision 1.28  2002/10/17 20:04:40  lampret
// Added BIST scan. Special VS RAMs need to be used to implement BIST.
//
// Revision 1.27  2002/09/16 03:13:23  lampret
// Removed obsolete comment.
//
// Revision 1.26  2002/09/08 05:52:16  lampret
// Added optional l.div/l.divu insns. By default they are disabled.
//
// Revision 1.25  2002/09/07 19:16:10  lampret
// If SR[CY] implemented with OR1200_IMPL_ADDC_MFD enabled, l.add/l.addi also set SR[CY].
//
// Revision 1.24  2002/09/07 05:42:02  lampret
// Added optional SR[CY]. Added define to enable additional (compare) flag modifiers. Defines are OR1200_IMPL_ADDC_MFD and OR1200_ADDITIONAL_FLAG_MODIFIERS_MFD.
//
// Revision 1.23  2002/09/04 00:50:34  lampret
// Now most of the configuration registers are updatded automatically based on defines in or1200_defines.v.
//
// Revision 1.22  2002/09/03 22:28:21  lampret
// As per Taylor Su suggestion all case blocks are full case by default and optionally (OR1200_CASE_DEFAULT_MFD) can be disabled to increase clock frequncy.
//
// Revision 1.21  2002/08/22 02:18:55  lampret
// Store buffer has been tested and it works. BY default it is still disabled until uClinux confirms correct operation on FPGA board.
//
// Revision 1.20  2002/08/18 21:59:45  lampret
// Disable SB until it is tested
//
// Revision 1.19  2002/08/18 19:53:08  lampret
// Added store buffer.
//
// Revision 1.18  2002/08/15 06:04:11  lampret
// Fixed Xilinx trace buffer address. REported by Taylor Su.
//
// Revision 1.17  2002/08/12 05:31:44  lampret
// Added OR1200_W_MFDB_RETRY_MFD. Moved WB registered outsputs / samples inputs into lower section.
//
// Revision 1.16  2002/07/14 22:17:17  lampret
// Added simple trace buffer [only for Xilinx Virtex target]. Fixed instruction fetch abort when new exception is recognized.
//
// Revision 1.15  2002/06/08 16:20:21  lampret
// Added defines for enabling generic FF based memory macro for register file.
//
// Revision 1.14  2002/03/29 16:24:06  lampret
// Changed comment about synopsys to _synopsys_ because synthesis was complaining about unknown directives
//
// Revision 1.13  2002/03/29 15:16:55  lampret
// Some of the warnings fixed.
//
// Revision 1.12  2002/03/28 19:25:42  lampret
// Added second type of Virtual Silicon two-port SRAM (for register file). Changed defines for VS STP RAMs.
//
// Revision 1.11  2002/03/28 19:13:17  lampret
// Updated defines.
//
// Revision 1.10  2002/03/14 00:30:24  lampret
// Added alternative for critical path in DU.
//
// Revision 1.9  2002/03/11 01:26:26  lampret
// Fixed async loop. Changed multiplier type for ASIC.
//
// Revision 1.8  2002/02/11 04:33:17  lampret
// Speed optimizations (removed duplicate _cyc_ and _stb_). Fixed D/IMMU cache-inhibit attr.
//
// Revision 1.7  2002/02/01 19:56:54  lampret
// Fixed combinational loops.
//
// Revision 1.6  2002/01/19 14:10:22  lampret
// Fixed OR1200_XILINX_RAM32X1D_MFD.
//
// Revision 1.5  2002/01/18 07:56:00  lampret
// No more low/high priority interrupts (PICPR removed). Added tick timer exception. Added exception prefix (SR[EPH]). Fixed single-step bug whenreading NPC.
//
// Revision 1.4  2002/01/14 09:44:12  lampret
// Default ASIC configuration does not sample WB inputs.
//
// Revision 1.3  2002/01/08 00:51:08  lampret
// Fixed typo. OR1200_REGISTERED_OUTPUTS_MFD was not defined. Should be.
//
// Revision 1.2  2002/01/03 21:23:03  lampret
// Uncommented OR1200_REGISTERED_OUTPUTS_MFD for FPGA target.
//
// Revision 1.1  2002/01/03 08:16:15  lampret
// New prefixes for RTL files, prefixed module names. Updated cache controllers and MMUs.
//
// Revision 1.20  2001/12/04 05:02:36  lampret
// Added OR1200_GENERIC_MULTP2_32X32_MFD and OR1200_ASIC_MFD_MULTP2_32X32_MFD
//
// Revision 1.19  2001/11/27 19:46:57  lampret
// Now FPGA and ASIC target are separate.
//
// Revision 1.18  2001/11/23 21:42:31  simons
// Program counter divided to PPC and NPC.
//
// Revision 1.17  2001/11/23 08:38:51  lampret
// Changed DSR/DRR behavior and exception detection.
//
// Revision 1.16  2001/11/20 21:30:38  lampret
// Added OR1200_REGISTERED_INPUTS_MFD.
//
// Revision 1.15  2001/11/19 14:29:48  simons
// Cashes disabled.
//
// Revision 1.14  2001/11/13 10:02:21  lampret
// Added 'setpc'. Renamed some signals (except_flushpipe into flushpipe etc)
//
// Revision 1.13  2001/11/12 01:45:40  lampret
// Moved flag bit into SR. Changed RF enable from constant enable to dynamic enable for read ports.
//
// Revision 1.12  2001/11/10 03:43:57  lampret
// Fixed exceptions.
//
// Revision 1.11  2001/11/02 18:57:14  lampret
// Modified virtual silicon instantiations.
//
// Revision 1.10  2001/10/21 17:57:16  lampret
// Removed params from generic_XX.v. Added translate_off/on in sprs.v and id.v. Removed spr_addr from dc.v and ic.v. Fixed CR+LF.
//
// Revision 1.9  2001/10/19 23:28:46  lampret
// Fixed some synthesis warnings. Configured with caches and MMUs.
//
// Revision 1.8  2001/10/14 13:12:09  lampret
// MP3 version.
//
// Revision 1.1.1.1  2001/10/06 10:18:36  igorm
// no message
//
// Revision 1.3  2001/08/17 08:01:19  lampret
// IC enable/disable.
//
// Revision 1.2  2001/08/13 03:36:20  lampret
// Added cfg regs. Moved all defines into one defines.v file. More cleanup.
//
// Revision 1.1  2001/08/09 13:39:33  lampret
// Major clean-up.
//
// Revision 1.2  2001/07/22 03:31:54  lampret
// Fixed RAM's oen bug. Cache bypass under development.
//
// Revision 1.1  2001/07/20 00:46:03  lampret
// Development version of RTL. Libraries are missing.
//
//

//
// Generate debug messages during simulation
//
//`define OR1200_VERBOSE_MFD

//  `define OR1200_ASIC_MFD
////////////////////////////////////////////////////////
//
// Typical configuration for an ASIC
//
`ifdef OR1200_ASIC_MFD


//
// Target ASIC memories
//
//`define OR1200_ARTISAN_SSP_MFD
//`define OR1200_ARTISAN_SDP_MFD
//`define OR1200_ARTISAN_STP_MFD
`define OR1200_VIRTUALSILICON_SSP_MFD
//`define OR1200_VIRTUALSILICON_STP_T1_MFD
//`define OR1200_VIRTUALSILICON_STP_T2_MFD

//
// Do not implement Data cache
//
//`define OR1200_NO_DC_MFD

//
// Do not implement Insn cache
//
//`define OR1200_NO_IC_MFD

//
// Do not implement Data MMU
//
`define OR1200_NO_DMMU_MFD

//
// Do not implement Insn MMU
//
`define OR1200_NO_IMMU_MFD

//
// Select between ASIC optimized and generic multiplier
//
//`define OR1200_ASIC_MFD_MULTP2_32X32_MFD
`define OR1200_GENERIC_MULTP2_32X32_MFD

//
// Size/type of insn/data cache if implemented
//
// `define OR1200_IC_1W_4KB_MFD
`define OR1200_IC_1W_8KB_MFD
`define OR1200_DC_1W_4KB_MFD
//`define OR1200_DC_1W_8KB_MFD

`else


/////////////////////////////////////////////////////////
//
// Typical configuration for an FPGA
//

//
// Target FPGA memories
//
//   ALEX `define OR1200_XILINX_RAMB4_MFD
////   ALEX `define OR1200_XILINX_RAM32X1D_MFD
//`define OR1200_USE_RAM16X1D_FOR_RAM32X1D_MFD

//
// Do not implement Data cache
//
//`define OR1200_NO_DC_MFD

//
// Do not implement Insn cache
//
//`define OR1200_NO_IC_MFD

//
// Do not implement Data MMU
//
`define OR1200_NO_DMMU_MFD

//
// Do not implement Insn MMU
//
`define OR1200_NO_IMMU_MFD

//
// Select between ASIC and generic multiplier
//
// (Generic seems to trigger a bug in the Cadence Ncsim simulator)
//
//`define OR1200_ASIC_MFD_MULTP2_32X32_MFD
`define OR1200_GENERIC_MULTP2_32X32_MFD

//
// Size/type of insn/data cache if implemented
// (consider available FPGA memory resources)
//
//`define OR1200_IC_1W_4KB_MFD
`define OR1200_IC_1W_8KB_MFD
`define OR1200_DC_1W_4KB_MFD
//`define OR1200_DC_1W_8KB_MFD

`endif

`define SEC_IC_TAG_MFD

//////////////////////////////////////////////////////////
//
// Do not change below unless you know what you are doing
//

//
// Enable RAM BIST
//
// At the moment this only works for Virtual Silicon
// single port RAMs. For other RAMs it has not effect.
// Special wrapper for VS RAMs needs to be provided
// with scan flops to facilitate bist scan.
//
//`define OR1200_BIST_MFD

//
// Register OR1200 WISHBONE outputs
// (must be defined/enabled)
//
`define OR1200_REGISTERED_OUTPUTS_MFD

//
// Register OR1200 WISHBONE inputs
//
// (must be undefined/disabled)
//
//`define OR1200_REGISTERED_INPUTS_MFD

//
// Disable bursts if they are not supported by the
// memory subsystem (only affect cache line fill)
//
//`define OR1200_NO_BURSTS_MFD
//

//
// WISHBONE retry counter range
//
// 2^value range for retry counter. Retry counter
// is activated whenever *wb_rty_i is asserted and
// until retry counter expires, corresponding
// WISHBONE interface is deactivated.
//
// To disable retry counters and *wb_rty_i all together,
// undefine this macro.
//
//`define OR1200_W_MFDB_RETRY_MFD 7

//
// WISHBONE Consecutive Address Burst
//
// This was used prior to WISHBONE B3 specification
// to identify bursts. It is no longer needed but
// remains enabled for compatibility with old designs.
//
// To remove *wb_cab_o ports undefine this macro.
//
`define OR1200_W_MFDB_CAB_MFD

//
// WISHBONE B3 compatible interface
//
// This follows the WISHBONE B3 specification.
// It is not enabled by default because most
// designs still don't use WB b3.
//
// To enable *wb_cti_o/*wb_bte_o ports,
// define this macro.
//
//`define OR1200_W_MFDB_B3_MFD

//
// Enable additional synthesis directives if using
// _Synopsys_ synthesis tool
//
//`define OR1200_ADDITIONAL_SYNOPSYS_DIRECTIVES_MFD

//
// Enables default statement in some case blocks
// and disables Synopsys synthesis directive full_case
//
// By default it is enabled. When disabled it
// can increase clock frequency.
//
`define OR1200_CASE_DEFAULT_MFD

//
// Operand width / register file address width
//
// (DO NOT CHANGE)
//
`define OR1200_OPERAND_WIDTH_MFD		32
`define OR1200_REGFILE_ADDR_WIDTH_MFD	5

//
// l.add/l.addi/l.and and optional l.addc/l.addic
// also set (compare) flag when result of their
// operation equals zero
//
// At the time of writing this, default or32
// C/C++ compiler doesn't generate code that
// would benefit from this optimization.
//
// By default this optimization is disabled to
// save area.
//
//`define OR1200_ADDITIONAL_FLAG_MODIFIERS_MFD

//
// Implement l.addc/l.addic instructions and SR[CY]
//
// At the time of writing this, or32
// C/C++ compiler doesn't generate l.addc/l.addic
// instructions. However or32 assembler
// can assemble code that uses l.addc/l.addic insns.
//
// By default implementation of l.addc/l.addic
// instructions and SR[CY] is disabled to save
// area.
//
// [Because this define controles implementation
//  of SR[CY] write enable, if it is not enabled,
//  l.add/l.addi also don't set SR[CY].]
//
//`define OR1200_IMPL_ADDC_MFD

//
// Implement optional l.div/l.divu instructions
//
// By default divide instructions are not implemented
// to save area and increase clock frequency. or32 C/C++
// compiler can use soft library for division.
//
`define OR1200_IMPL_DIV_MFD

//
// Implement rotate in the ALU
//
// At the time of writing this, or32
// C/C++ compiler doesn't generate rotate
// instructions. However or32 assembler
// can assemble code that uses rotate insn.
// This means that rotate instructions
// must be used manually inserted.
//
// By default implementation of rotate
// is disabled to save area and increase
// clock frequency.
//
//`define OR1200_IMPL_ALU_ROTATE_MFD

//
// Type of ALU compare to implement
//
// Try either one to find what yields
// higher clock frequencyin your case.
//
//`define OR1200_IMPL_ALU_COMP1_MFD
`define OR1200_IMPL_ALU_COMP2_MFD

//
// Select between low-power (larger) multiplier or faster multiplier
//
//`define OR1200_LOWPWR_MULT_MFD

//
// Clock synchronization for RISC clk and WB divided clocks
//
// If you plan to run WB:RISC clock 1:1, you can comment these two
//
//`define OR1200_CLKDIV_2_SUPPORTED_MFD
//`define OR1200_CLKDIV_4_SUPPORTED_MFD

//
// Type of register file RAM
//
// Memory macro w/ two ports (see or1200_hdtp_32x32.v)
// `define OR1200_RFRAM_TWOPORT_MFD
//
// Memory macro dual port (see or1200_hddp_32x32.v)
// `define OR1200_RFRAM_DUALPORT_MFD
//
// ... otherwise generic (flip-flop based) register file

//
// Type of mem2reg aligner to implement.
//
// Once OR1200_IMPL_MEM2REG2_MFD yielded faster
// circuit, however with today tools it will
// most probably give you slower circuit.
//
`define OR1200_IMPL_MEM2REG1_MFD
//`define OR1200_IMPL_MEM2REG2_MFD

//
// ALUOPs
//
`define OR1200_ALUOP_WIDTH_MFD	4
`define OR1200_ALUOP_NOP_MFD	4'd4
/* Order defined by arith insns that have two source operands both in regs
   (see binutils/include/opcode/or32.h) */
`define OR1200_ALUOP_ADD_MFD	4'd0
`define OR1200_ALUOP_ADD_MFDC_MFD	4'd1
`define OR1200_ALUOP_SUB_MFD	4'd2
`define OR1200_ALUOP_AND_MFD	4'd3
`define OR1200_ALUOP_OR_MFD		4'd4
`define OR1200_ALUOP_XOR_MFD	4'd5
`define OR1200_ALUOP_MUL_MFD	4'd6
`define OR1200_ALUOP_SHROT_MFD	4'd8
`define OR1200_ALUOP_DIV_MFD	4'd9
`define OR1200_ALUOP_DIV_MFDU_MFD	4'd10
`define OR1200_ALUOP_FF1_MFD	4'd15
/* Order not specifically defined. */
`define OR1200_ALUOP_IMM_MFD	4'd11
`define OR1200_ALUOP_MOVHI_MFD	4'd12
`define OR1200_ALUOP_COMP_MFD	4'd13
`define OR1200_ALUOP_MTSR_MFD	4'd14
`define OR1200_ALUOP_MFSR_MFD	4'd15

//
// MACOPs
//
`define OR1200_MACOP_WIDTH_MFD	2
`define OR1200_MACOP_NOP_MFD	2'b00
`define OR1200_MACOP_MAC_MFD	2'b01
`define OR1200_MACOP_MSB_MFD	2'b10

//
// Shift/rotate ops
//
`define OR1200_SHROTOP_WIDTH_MFD	2
`define OR1200_SHROTOP_NOP_MFD	2'd0
`define OR1200_SHROTOP_SLL_MFD	2'd0
`define OR1200_SHROTOP_SRL_MFD	2'd1
`define OR1200_SHROTOP_SRA_MFD	2'd2
`define OR1200_SHROTOP_ROR_MFD	2'd3

// Execution cycles per instruction
`define OR1200_MULTICYCLE_WIDTH_MFD	2
`define OR1200_ONE_CYCLE_MFD		2'd0
`define OR1200_TWO_CYCLES_MFD		2'd1

// Operand MUX selects
`define OR1200_SEL_WIDTH_MFD		2
`define OR1200_SEL_RF_MFD			2'd0
`define OR1200_SEL_IMM_MFD			2'd1
`define OR1200_SEL_EX_FORW_MFD		2'd2
`define OR1200_SEL_WB_FORW_MFD		2'd3

//
// BRANCHOPs
//
`define OR1200_BRANCHOP_WIDTH_MFD		3
`define OR1200_BRANCHOP_NOP_MFD		3'd0
`define OR1200_BRANCHOP_J_MFD		3'd1
`define OR1200_BRANCHOP_J_MFDR_MFD		3'd2
`define OR1200_BRANCHOP_BAL_MFD		3'd3
`define OR1200_BRANCHOP_BF_MFD		3'd4
`define OR1200_BRANCHOP_BNF_MFD		3'd5
`define OR1200_BRANCHOP_RFE_MFD		3'd6

//
// LSUOPs
//
// Bit 0: sign extend
// Bits 1-2: 00 doubleword, 01 byte, 10 halfword, 11 singleword
// Bit 3: 0 load, 1 store
`define OR1200_LSUOP_WIDTH_MFD		4
`define OR1200_LSUOP_NOP_MFD		4'b0000
`define OR1200_LSUOP_LBZ_MFD		4'b0010
`define OR1200_LSUOP_LBS_MFD		4'b0011
`define OR1200_LSUOP_LHZ_MFD		4'b0100
`define OR1200_LSUOP_LHS_MFD		4'b0101
`define OR1200_LSUOP_LWZ_MFD		4'b0110
`define OR1200_LSUOP_LWS_MFD		4'b0111
`define OR1200_LSUOP_LD_MFD		4'b0001
`define OR1200_LSUOP_SD_MFD		4'b1000
`define OR1200_LSUOP_SB_MFD		4'b1010
`define OR1200_LSUOP_SH_MFD		4'b1100
`define OR1200_LSUOP_SW_MFD		4'b1110

// FETCHOPs
`define OR1200_FETCHOP_WIDTH_MFD		1
`define OR1200_FETCHOP_NOP_MFD		1'b0
`define OR1200_FETCHOP_LW_MFD		1'b1

//
// Register File Write-Back OPs
//
// Bit 0: register file write enable
// Bits 2-1: write-back mux selects
`define OR1200_RFWBOP_WIDTH_MFD		3
`define OR1200_RFWBOP_NOP_MFD		3'b000
`define OR1200_RFWBOP_ALU_MFD		3'b001
`define OR1200_RFWBOP_LSU_MFD		3'b011
`define OR1200_RFWBOP_SPRS_MFD		3'b101
`define OR1200_RFWBOP_LR_MFD		3'b111

// Compare instructions
`define OR1200_COP_SFEQ_MFD       3'b000
`define OR1200_COP_SFNE_MFD       3'b001
`define OR1200_COP_SFGT_MFD       3'b010
`define OR1200_COP_SFGE_MFD       3'b011
`define OR1200_COP_SFLT_MFD       3'b100
`define OR1200_COP_SFLE_MFD       3'b101
`define OR1200_COP_X_MFD          3'b111
`define OR1200_SIGNED_COMPARE_MFD 'd3
`define OR1200_COMPOP_WIDTH_MFD	4

//
// TAGs for instruction bus
//
`define OR1200_ITAG_IDLE_MFD	4'h0	// idle bus
`define	OR1200_ITAG_NI_MFD		4'h1	// normal insn
`define OR1200_ITAG_BE_MFD		4'hb	// Bus error exception
`define OR1200_ITAG_PE_MFD		4'hc	// Page fault exception
`define OR1200_ITAG_TE_MFD		4'hd	// TLB miss exception

//
// TAGs for data bus
//
`define OR1200_DTAG_IDLE_MFD	4'h0	// idle bus
`define	OR1200_DTAG_ND_MFD		4'h1	// normal data
`define OR1200_DTAG_AE_MFD		4'ha	// Alignment exception
`define OR1200_DTAG_BE_MFD		4'hb	// Bus error exception
`define OR1200_DTAG_PE_MFD		4'hc	// Page fault exception
`define OR1200_DTAG_TE_MFD		4'hd	// TLB miss exception


//////////////////////////////////////////////
//
// ORBIS32 ISA specifics
//

// SHROT_OP position in machine word
`define OR1200_SHROTOP_POS_MFD		7:6

// ALU instructions multicycle field in machine word
`define OR1200_ALUMCYC_POS_MFD		9:8

//
// Instruction opcode groups (basic)
//
`define OR1200_OR32_J_MFD                 6'b000000
`define OR1200_OR32_J_MFDAL_MFD               6'b000001
`define OR1200_OR32_BNF_MFD               6'b000011
`define OR1200_OR32_BF_MFD                6'b000100
`define OR1200_OR32_NOP_MFD               6'b000101
`define OR1200_OR32_MOVHI_MFD             6'b000110
`define OR1200_OR32_XSYNC_MFD             6'b001000
`define OR1200_OR32_RFE_MFD               6'b001001
/* */
`define OR1200_OR32_J_MFDR_MFD                6'b010001
`define OR1200_OR32_J_MFDAL_MFDR_MFD              6'b010010
`define OR1200_OR32_MACI_MFD              6'b010011
/* */
`define OR1200_OR32_LWZ_MFD               6'b100001
`define OR1200_OR32_LBZ_MFD               6'b100011
`define OR1200_OR32_LBS_MFD               6'b100100
`define OR1200_OR32_LHZ_MFD               6'b100101
`define OR1200_OR32_LHS_MFD               6'b100110
`define OR1200_OR32_ADDI_MFD              6'b100111
`define OR1200_OR32_ADDI_MFDC_MFD             6'b101000
`define OR1200_OR32_ANDI_MFD              6'b101001
`define OR1200_OR32_ORI_MFD               6'b101010
`define OR1200_OR32_XORI_MFD              6'b101011
`define OR1200_OR32_MULI_MFD              6'b101100
`define OR1200_OR32_MFSPR_MFD             6'b101101
`define OR1200_OR32_SH_MFD_ROTI_MFD 	      6'b101110
`define OR1200_OR32_SFXX_MFDI_MFD             6'b101111
/* */
`define OR1200_OR32_MTSPR_MFD             6'b110000
`define OR1200_OR32_MACMSB_MFD            6'b110001
/* */
`define OR1200_OR32_SW_MFD                6'b110101
`define OR1200_OR32_SB_MFD                6'b110110
`define OR1200_OR32_SH_MFD                6'b110111
`define OR1200_OR32_ALU_MFD               6'b111000
`define OR1200_OR32_SFXX_MFD              6'b111001


/////////////////////////////////////////////////////
//
// Exceptions
//
`define OR1200_EXCEPT_WIDTH_MFD 4
`define OR1200_EXCEPT_UNUSED_MFD		`OR1200_EXCEPT_WIDTH_MFD'hf
`define OR1200_EXCEPT_TRAP_MFD		`OR1200_EXCEPT_WIDTH_MFD'he
`define OR1200_EXCEPT_BREAK_MFD		`OR1200_EXCEPT_WIDTH_MFD'hd
`define OR1200_EXCEPT_SYSCALL_MFD		`OR1200_EXCEPT_WIDTH_MFD'hc
`define OR1200_EXCEPT_RANGE_MFD		`OR1200_EXCEPT_WIDTH_MFD'hb
`define OR1200_EXCEPT_ITLBMISS_MFD		`OR1200_EXCEPT_WIDTH_MFD'ha
`define OR1200_EXCEPT_DTLBMISS_MFD		`OR1200_EXCEPT_WIDTH_MFD'h9
`define OR1200_EXCEPT_INT_MFD		`OR1200_EXCEPT_WIDTH_MFD'h8
`define OR1200_EXCEPT_ILLEGAL_MFD		`OR1200_EXCEPT_WIDTH_MFD'h7
`define OR1200_EXCEPT_ALIGN_MFD		`OR1200_EXCEPT_WIDTH_MFD'h6
`define OR1200_EXCEPT_TICK_MFD		`OR1200_EXCEPT_WIDTH_MFD'h5
`define OR1200_EXCEPT_IPF_MFD		`OR1200_EXCEPT_WIDTH_MFD'h4
`define OR1200_EXCEPT_DPF_MFD		`OR1200_EXCEPT_WIDTH_MFD'h3
`define OR1200_EXCEPT_BUSERR_MFD		`OR1200_EXCEPT_WIDTH_MFD'h2
`define OR1200_EXCEPT_RESET_MFD		`OR1200_EXCEPT_WIDTH_MFD'h1
`define OR1200_EXCEPT_NONE_MFD		`OR1200_EXCEPT_WIDTH_MFD'h0


/////////////////////////////////////////////////////
//
// SPR groups
//

// Bits that define the group
`define OR1200_SPR_GROUP_BITS_MFD	15:11

// Width of the group bits
`define OR1200_SPR_GROUP_WIDTH_MFD 	5

// Bits that define offset inside the group
`define OR1200_SPR_OFS_BITS_MFD 10:0

// List of groups
`define OR1200_SPR_GROUP_SYS_MFD	5'd00
`define OR1200_SPR_GROUP_DMMU_MFD	5'd01
`define OR1200_SPR_GROUP_IMMU_MFD	5'd02
`define OR1200_SPR_GROUP_DC_MFD	5'd03
`define OR1200_SPR_GROUP_IC_MFD	5'd04
`define OR1200_SPR_GROUP_MAC_MFD	5'd05
`define OR1200_SPR_GROUP_DU_MFD	5'd06
`define OR1200_SPR_GROUP_PM_MFD	5'd08
`define OR1200_SPR_GROUP_PIC_MFD	5'd09
`define OR1200_SPR_GROUP_TT_MFD	5'd10


/////////////////////////////////////////////////////
//
// System group
//

//
// System registers
//
`define OR1200_SPR_CFGR_MFD		7'd0
`define OR1200_SPR_RF_MFD		6'd32	// 1024 >> 5
`define OR1200_SPR_NPC_MFD		11'd16
`define OR1200_SPR_SR_MFD		11'd17
`define OR1200_SPR_PPC_MFD		11'd18
`define OR1200_SPR_EPCR_MFD		11'd32
`define OR1200_SPR_EEAR_MFD		11'd48
`define OR1200_SPR_ESR_MFD		11'd64

//
// SR bits
//
`define OR1200_SR_WIDTH_MFD 16
`define OR1200_SR_SM_MFD   0
`define OR1200_SR_TEE_MFD  1
`define OR1200_SR_IEE_MFD  2
`define OR1200_SR_DCE_MFD  3
`define OR1200_SR_ICE_MFD  4
`define OR1200_SR_DME_MFD  5
`define OR1200_SR_IME_MFD  6
`define OR1200_SR_LEE_MFD  7
`define OR1200_SR_CE_MFD   8
`define OR1200_SR_F_MFD    9
`define OR1200_SR_CY_MFD   10	// Unused
`define OR1200_SR_OV_MFD   11	// Unused
`define OR1200_SR_OV_MFDE_MFD  12	// Unused
`define OR1200_SR_DSX_MFD  13	// Unused
`define OR1200_SR_EPH_MFD  14
`define OR1200_SR_F_MFDO_MFD   15
`define OR1200_SR_CID_MFD  31:28	// Unimplemented

// Bits that define offset inside the group
`define OR1200_SPROFS_BITS_MFD 10:0


/////////////////////////////////////////////////////
//
// Power Management (PM)
//

// Define it if you want PM implemented
//`define OR1200_PM_IMPLEMENTED_MFD

// Bit positions inside PMR (don't change)
`define OR1200_PM_PMR_SDF_MFD 3:0
`define OR1200_PM_PMR_DME_MFD 4
`define OR1200_PM_PMR_SME_MFD 5
`define OR1200_PM_PMR_DCGE_MFD 6
`define OR1200_PM_PMR_UNUSED_MFD 31:7

// PMR offset inside PM group of registers
`define OR1200_PM_OFS_PMR_MFD 11'b0

// PM group
`define OR1200_SPRGRP_PM_MFD 5'd8

// Define if PMR can be read/written at any address inside PM group
`define OR1200_PM_PARTIAL_DECODING_MFD

// Define if reading PMR is allowed
`define OR1200_PM_READREGS_MFD

// Define if unused PMR bits should be zero
`define OR1200_PM_UNUSED_ZERO_MFD


/////////////////////////////////////////////////////
//
// Debug Unit (DU)
//

// Define it if you want DU implemented
//`define OR1200_DU_IMPLEMENTED_MFD

// Define if you want trace buffer
// (for now only available for Xilinx Virtex FPGAs)
`ifdef OR1200_ASIC_MFD
`else
//`define OR1200_DU_TB_IMPLEMENTED_MFD
`endif

// Address offsets of DU registers inside DU group
`define OR1200_DU_OFS_DMR1_MFD 11'd16
`define OR1200_DU_OFS_DMR2_MFD 11'd17
`define OR1200_DU_OFS_DSR_MFD 11'd20
`define OR1200_DU_OFS_DRR_MFD 11'd21
`define OR1200_DU_OFS_TBADR_MFD 11'h0ff
`define OR1200_DU_OFS_TBIA_MFD 11'h1xx
`define OR1200_DU_OFS_TBIM_MFD 11'h2xx
`define OR1200_DU_OFS_TBAR_MFD 11'h3xx
`define OR1200_DU_OFS_TBTS_MFD 11'h4xx

// Position of offset bits inside SPR address
`define OR1200_DUOFS_BITS_MFD 10:0

// Define if you want these DU registers to be implemented
`define OR1200_DU_DMR1_MFD
`define OR1200_DU_DMR2_MFD
`define OR1200_DU_DSR_MFD
`define OR1200_DU_DRR_MFD

// DMR1 bits
`define OR1200_DU_DMR1_MFD_ST_MFD 22

// DSR bits
`define OR1200_DU_DSR_MFD_WIDTH_MFD	14
`define OR1200_DU_DSR_MFD_RSTE_MFD	0
`define OR1200_DU_DSR_MFD_BUSEE_MFD	1
`define OR1200_DU_DSR_MFD_DPFE_MFD	2
`define OR1200_DU_DSR_MFD_IPFE_MFD	3
`define OR1200_DU_DSR_MFD_TTE_MFD	4
`define OR1200_DU_DSR_MFD_AE_MFD	5
`define OR1200_DU_DSR_MFD_IIE_MFD	6
`define OR1200_DU_DSR_MFD_IE_MFD	7
`define OR1200_DU_DSR_MFD_DME_MFD	8
`define OR1200_DU_DSR_MFD_IME_MFD	9
`define OR1200_DU_DSR_MFD_RE_MFD	10
`define OR1200_DU_DSR_MFD_SCE_MFD	11
`define OR1200_DU_DSR_MFD_BE_MFD	12
`define OR1200_DU_DSR_MFD_TE_MFD	13

// DRR bits
`define OR1200_DU_DRR_MFD_RSTE_MFD	0
`define OR1200_DU_DRR_MFD_BUSEE_MFD	1
`define OR1200_DU_DRR_MFD_DPFE_MFD	2
`define OR1200_DU_DRR_MFD_IPFE_MFD	3
`define OR1200_DU_DRR_MFD_TTE_MFD	4
`define OR1200_DU_DRR_MFD_AE_MFD	5
`define OR1200_DU_DRR_MFD_IIE_MFD	6
`define OR1200_DU_DRR_MFD_IE_MFD	7
`define OR1200_DU_DRR_MFD_DME_MFD	8
`define OR1200_DU_DRR_MFD_IME_MFD	9
`define OR1200_DU_DRR_MFD_RE_MFD	10
`define OR1200_DU_DRR_MFD_SCE_MFD	11
`define OR1200_DU_DRR_MFD_BE_MFD	12
`define OR1200_DU_DRR_MFD_TE_MFD	13

// Define if reading DU regs is allowed
`define OR1200_DU_READREGS_MFD

// Define if unused DU registers bits should be zero
`define OR1200_DU_UNUSED_ZERO_MFD

// DU operation commands
`define OR1200_DU_OP_READSPR_MFD 	3'd4
`define OR1200_DU_OP_WRITESPR_MFD	3'd5

// Define if IF/LSU status is not needed by devel i/f
`define OR1200_DU_STATUS_UNIMPLEMENTED_MFD

/////////////////////////////////////////////////////
//
// Programmable Interrupt Controller (PIC)
//

// Define it if you want PIC implemented
`define OR1200_PIC_IMPLEMENTED_MFD

// Define number of interrupt inputs (2-31)
`define OR1200_PIC_INTS_MFD 14

// Address offsets of PIC registers inside PIC group
`define OR1200_PIC_OFS_PICMR_MFD 2'd0
`define OR1200_PIC_OFS_PICSR_MFD 2'd2

// Position of offset bits inside SPR address
`define OR1200_PICOFS_BITS_MFD 1:0

// Define if you want these PIC registers to be implemented
`define OR1200_PIC_PICMR_MFD
`define OR1200_PIC_PICSR_MFD

// Define if reading PIC registers is allowed
`define OR1200_PIC_READREGS_MFD

// Define if unused PIC register bits should be zero
`define OR1200_PIC_UNUSED_ZERO_MFD


/////////////////////////////////////////////////////
//
// Tick Timer (TT)
//

// Define it if you want TT implemented
//`define OR1200_TT_IMPLEMENTED_MFD

// Address offsets of TT registers inside TT group
`define OR1200_TT_OFS_TTMR_MFD 1'd0
`define OR1200_TT_OFS_TTCR_MFD 1'd1

// Position of offset bits inside SPR group
`define OR1200_TTOFS_BITS_MFD 0

// Define if you want these TT registers to be implemented
`define OR1200_TT_TTMR_MFD
`define OR1200_TT_TTCR_MFD

// TTMR bits
`define OR1200_TT_TTMR_MFD_TP_MFD 27:0
`define OR1200_TT_TTMR_MFD_IP_MFD 28
`define OR1200_TT_TTMR_MFD_IE_MFD 29
`define OR1200_TT_TTMR_MFD_M_MFD 31:30

// Define if reading TT registers is allowed
`define OR1200_TT_READREGS_MFD


//////////////////////////////////////////////
//
// MAC
//
`define OR1200_MAC_ADDR_MFD		0	// MACLO 0xxxxxxxx1, MACHI 0xxxxxxxx0
`define OR1200_MAC_SPR_WE_MFD		// Define if MACLO/MACHI are SPR writable


//////////////////////////////////////////////
//
// Data MMU (DMMU)
//

//
// Address that selects between TLB TR and MR
//
`define OR1200_DTLB_TM_ADDR_MFD	7

//
// DTLBMR fields
//
`define	OR1200_DTLBMR_V_BITS_MFD	0
`define	OR1200_DTLBMR_CID_BITS_MFD	4:1
`define	OR1200_DTLBMR_RES_BITS_MFD	11:5
`define OR1200_DTLBMR_VPN_BITS_MFD	31:13

//
// DTLBTR fields
//
`define	OR1200_DTLBTR_CC_BITS_MFD	0
`define	OR1200_DTLBTR_CI_BITS_MFD	1
`define	OR1200_DTLBTR_WBC_BITS_MFD	2
`define	OR1200_DTLBTR_WOM_BITS_MFD	3
`define	OR1200_DTLBTR_A_BITS_MFD	4
`define	OR1200_DTLBTR_D_BITS_MFD	5
`define	OR1200_DTLBTR_URE_BITS_MFD	6
`define	OR1200_DTLBTR_UWE_BITS_MFD	7
`define	OR1200_DTLBTR_SRE_BITS_MFD	8
`define	OR1200_DTLBTR_SWE_BITS_MFD	9
`define	OR1200_DTLBTR_RES_BITS_MFD	11:10
`define OR1200_DTLBTR_PPN_BITS_MFD	31:13

//
// DTLB configuration
//
`define	OR1200_DMMU_PS_MFD		13					// 13 for 8KB page size
`define	OR1200_DTLB_INDX_MFDW_MFD	6					// 6 for 64 entry DTLB	7 for 128 entries
`define OR1200_DTLB_INDX_MFDL_MFD	`OR1200_DMMU_PS_MFD				// 13			13
`define OR1200_DTLB_INDX_MFDH_MFD	`OR1200_DMMU_PS_MFD+`OR1200_DTLB_INDX_MFDW_MFD-1	// 18			19
`define	OR1200_DTLB_INDX_MFD	`OR1200_DTLB_INDX_MFDH_MFD:`OR1200_DTLB_INDX_MFDL_MFD	// 18:13		19:13
`define OR1200_DTLB_TAG_MFDW_MFD	32-`OR1200_DTLB_INDX_MFDW_MFD-`OR1200_DMMU_PS_MFD	// 13			12
`define OR1200_DTLB_TAG_MFDL_MFD	`OR1200_DTLB_INDX_MFDH_MFD+1			// 19			20
`define	OR1200_DTLB_TAG_MFD		31:`OR1200_DTLB_TAG_MFDL_MFD			// 31:19		31:20
`define	OR1200_DTLBMRW_MFD		`OR1200_DTLB_TAG_MFDW_MFD+1			// +1 because of V bit
`define	OR1200_DTLBTRW_MFD		32-`OR1200_DMMU_PS_MFD+5			// +5 because of protection bits and CI

//
// Cache inhibit while DMMU is not enabled/implemented
//
// cache inhibited 0GB-4GB		1'b1
// cache inhibited 0GB-2GB		!dcpu_adr_i[31]
// cache inhibited 0GB-1GB 2GB-3GB	!dcpu_adr_i[30]
// cache inhibited 1GB-2GB 3GB-4GB	dcpu_adr_i[30]
// cache inhibited 2GB-4GB (default)	dcpu_adr_i[31]
// cached 0GB-4GB			1'b0
//
`ifdef OR1200_NO_DMMU_MFD
`define OR1200_DMMU_CI_MFD			dcpu_adr_cpu[31]
`else
`define OR1200_DMMU_CI_MFD			dcpu_adr_i[31]
`endif


//////////////////////////////////////////////
//
// Insn MMU (IMMU)
//

//
// Address that selects between TLB TR and MR
//
`define OR1200_ITLB_TM_ADDR_MFD	7

//
// ITLBMR fields
//
`define	OR1200_ITLBMR_V_BITS_MFD	0
`define	OR1200_ITLBMR_CID_BITS_MFD	4:1
`define	OR1200_ITLBMR_RES_BITS_MFD	11:5
`define OR1200_ITLBMR_VPN_BITS_MFD	31:13

//
// ITLBTR fields
//
`define	OR1200_ITLBTR_CC_BITS_MFD	0
`define	OR1200_ITLBTR_CI_BITS_MFD	1
`define	OR1200_ITLBTR_WBC_BITS_MFD	2
`define	OR1200_ITLBTR_WOM_BITS_MFD	3
`define	OR1200_ITLBTR_A_BITS_MFD	4
`define	OR1200_ITLBTR_D_BITS_MFD	5
`define	OR1200_ITLBTR_SXE_BITS_MFD	6
`define	OR1200_ITLBTR_UXE_BITS_MFD	7
`define	OR1200_ITLBTR_RES_BITS_MFD	11:8
`define OR1200_ITLBTR_PPN_BITS_MFD	31:13

//
// ITLB configuration
//
`define	OR1200_IMMU_PS_MFD		13					// 13 for 8KB page size
`define	OR1200_ITLB_INDX_MFDW_MFD	6					// 6 for 64 entry ITLB	7 for 128 entries
`define OR1200_ITLB_INDX_MFDL_MFD	`OR1200_IMMU_PS_MFD				// 13			13
`define OR1200_ITLB_INDX_MFDH_MFD	`OR1200_IMMU_PS_MFD+`OR1200_ITLB_INDX_MFDW_MFD-1	// 18			19
`define	OR1200_ITLB_INDX_MFD	`OR1200_ITLB_INDX_MFDH_MFD:`OR1200_ITLB_INDX_MFDL_MFD	// 18:13		19:13
`define OR1200_ITLB_TAG_MFDW_MFD	32-`OR1200_ITLB_INDX_MFDW_MFD-`OR1200_IMMU_PS_MFD	// 13			12
`define OR1200_ITLB_TAG_MFDL_MFD	`OR1200_ITLB_INDX_MFDH_MFD+1			// 19			20
`define	OR1200_ITLB_TAG_MFD		31:`OR1200_ITLB_TAG_MFDL_MFD			// 31:19		31:20
`define	OR1200_ITLBMRW_MFD		`OR1200_ITLB_TAG_MFDW_MFD+1			// +1 because of V bit
`define	OR1200_ITLBTRW_MFD		32-`OR1200_IMMU_PS_MFD+3			// +3 because of protection bits and CI

//
// Cache inhibit while IMMU is not enabled/implemented
// Note: all combinations that use icpu_adr_i cause async loop
//
// cache inhibited 0GB-4GB		1'b1
// cache inhibited 0GB-2GB		!icpu_adr_i[31]
// cache inhibited 0GB-1GB 2GB-3GB	!icpu_adr_i[30]
// cache inhibited 1GB-2GB 3GB-4GB	icpu_adr_i[30]
// cache inhibited 2GB-4GB (default)	icpu_adr_i[31]
// cached 0GB-4GB			1'b0
//
`define OR1200_IMMU_CI_MFD			1'b0


/////////////////////////////////////////////////
//
// Insn cache (IC)
//

// 3 for 8 bytes, 4 for 16 bytes etc
//`define OR1200_ICLS_MFD		4
//`define OR1200_ICLS_MFD		5     // 32 byte per line
`define OR1200_ICLS_MFD		6     // 64 byte per line
//`define OR1200_ICLS_MFD		7     // 128 byte per line

`define OR1200_IC_CI_INDXL_MFD      20    // Cache Inhibit Region = upper 1MB
`define OR1200_IC_ACT_INDXH_MFD     23    // Active Region = under 16 MB

//
// IC configurations
//
`ifdef OR1200_IC_1W_4KB_MFD
`define OR1200_ICSIZE_MFD			12			// 4096
`define OR1200_ICINDX_MFD			`OR1200_ICSIZE_MFD-2	// 10
`define OR1200_ICINDX_MFDH_MFD			`OR1200_ICSIZE_MFD-1	// 11
`define OR1200_ICTAG_MFDL_MFD			`OR1200_ICINDX_MFDH_MFD+1	// 12
`define	OR1200_ICTAG_MFD			`OR1200_ICSIZE_MFD-`OR1200_ICLS_MFD	// 6 
`define	OR1200_ICTAG_MFD_W_MFD			21
`endif
`ifdef OR1200_IC_1W_8KB_MFD
`define OR1200_ICSIZE_MFD			13			// 8192
`define OR1200_ICINDX_MFD			`OR1200_ICSIZE_MFD-2	// 11
`define OR1200_ICINDX_MFDH_MFD			`OR1200_ICSIZE_MFD-1	// 12
`define OR1200_ICTAG_MFDL_MFD			`OR1200_ICINDX_MFDH_MFD+1	// 13
`define	OR1200_ICTAG_MFD			`OR1200_ICSIZE_MFD-`OR1200_ICLS_MFD	// 7 
`define	OR1200_ICTAG_MFD_W_MFD			20
`endif


/////////////////////////////////////////////////
//
// Data cache (DC)
//

// 3 for 8 bytes, 4 for 16 bytes etc
`define OR1200_DCLS_MFD		4

`define OR1200_DC_CI_INDXL_MFD      20    // Cache Inhibit Region = upper 1MB
`define OR1200_DC_ACT_INDXH_MFD     23    // Active Region = under 16 MB

// Define to perform store refill (potential performance penalty)
// `define OR1200_DC_STORE_REFILL_MFD

//
// DC configurations
//
`ifdef OR1200_DC_1W_4KB_MFD
`define OR1200_DCSIZE_MFD			12			// 4096
`define OR1200_DCINDX_MFD			`OR1200_DCSIZE_MFD-2	// 10
`define OR1200_DCINDX_MFDH_MFD			`OR1200_DCSIZE_MFD-1	// 11
`define OR1200_DCTAG_MFDL_MFD			`OR1200_DCINDX_MFDH_MFD+1	// 12
`define	OR1200_DCTAG_MFD			`OR1200_DCSIZE_MFD-`OR1200_DCLS_MFD	// 8
`define	OR1200_DCTAG_MFD_W_MFD			21
`endif
`ifdef OR1200_DC_1W_8KB_MFD
`define OR1200_DCSIZE_MFD			13			// 8192
`define OR1200_DCINDX_MFD			`OR1200_DCSIZE_MFD-2	// 11
`define OR1200_DCINDX_MFDH_MFD			`OR1200_DCSIZE_MFD-1	// 12
`define OR1200_DCTAG_MFDL_MFD			`OR1200_DCINDX_MFDH_MFD+1	// 13
`define	OR1200_DCTAG_MFD			`OR1200_DCSIZE_MFD-`OR1200_DCLS_MFD	// 9
`define	OR1200_DCTAG_MFD_W_MFD			20
`endif

/////////////////////////////////////////////////
//
// Store buffer (SB)
//

//
// Store buffer
//
// It will improve performance by "caching" CPU stores
// using store buffer. This is most important for function
// prologues because DC can only work in write though mode
// and all stores would have to complete external WB writes
// to memory.
// Store buffer is between DC and data BIU.
// All stores will be stored into store buffer and immediately
// completed by the CPU, even though actual external writes
// will be performed later. As a consequence store buffer masks
// all data bus errors related to stores (data bus errors
// related to loads are delivered normally).
// All pending CPU loads will wait until store buffer is empty to
// ensure strict memory model. Right now this is necessary because
// we don't make destinction between cached and cache inhibited
// address space, so we simply empty store buffer until loads
// can begin.
//
// It makes design a bit bigger, depending what is the number of
// entries in SB FIFO. Number of entries can be changed further
// down.
//
//`define OR1200_SB_IMPLEMENTED_MFD

//
// Number of store buffer entries
//
// Verified number of entries are 4 and 8 entries
// (2 and 3 for OR1200_SB_LOG_MFD). OR1200_SB_ENTRIES_MFD must
// always match 2**OR1200_SB_LOG_MFD.
// To disable store buffer, undefine
// OR1200_SB_IMPLEMENTED_MFD.
//
`define OR1200_SB_LOG_MFD		2	// 2 or 3
`define OR1200_SB_ENTRIES_MFD	4	// 4 or 8


/////////////////////////////////////////////////////
//
// VR, UPR and Configuration Registers
//
//
// VR, UPR and configuration registers are optional. If 
// implemented, operating system can automatically figure
// out how to use the processor because it knows 
// what units are available in the processor and how they
// are configured.
//
// This section must be last in or1200_defines.v file so
// that all units are already configured and thus
// configuration registers are properly set.
// 

// Define if you want configuration registers implemented
//`define OR1200_CFGR_IMPLEMENTED_MFD

// Define if you want full address decode inside SYS group
//`define OR1200_SYS_FULL_DECODE_MFD

// Offsets of VR, UPR and CFGR registers
`define OR1200_SPRGRP_SYS_VR_MFD		4'h0
`define OR1200_SPRGRP_SYS_UPR_MFD		4'h1
`define OR1200_SPRGRP_SYS_CPUCFGR_MFD	4'h2
`define OR1200_SPRGRP_SYS_DMMUCFGR_MFD	4'h3
`define OR1200_SPRGRP_SYS_IMMUCFGR_MFD	4'h4
`define OR1200_SPRGRP_SYS_DCCFGR_MFD	4'h5
`define OR1200_SPRGRP_SYS_ICCFGR_MFD	4'h6
`define OR1200_SPRGRP_SYS_DCFGR_MFD	4'h7

// VR fields
`define OR1200_VR_REV_MFD_BITS_MFD		5:0
`define OR1200_VR_RES1_MFD_BITS_MFD		15:6
`define OR1200_VR_CFG_MFD_BITS_MFD		23:16
`define OR1200_VR_VER_MFD_BITS_MFD		31:24

// VR values
`define OR1200_VR_REV_MFD			6'h00
`define OR1200_VR_RES1_MFD			10'h000
`define OR1200_VR_CFG_MFD			8'h00
`define OR1200_VR_VER_MFD			8'h12

// UPR fields
`define OR1200_UPR_UP_MFD_BITS_MFD		0
`define OR1200_UPR_DCP_MFD_BITS_MFD		1
`define OR1200_UPR_ICP_MFD_BITS_MFD		2
`define OR1200_UPR_DMP_MFD_BITS_MFD		3
`define OR1200_UPR_IMP_MFD_BITS_MFD		4
`define OR1200_UPR_MP_MFD_BITS_MFD		5
`define OR1200_UPR_DUP_MFD_BITS_MFD		6
`define OR1200_UPR_PCUP_MFD_BITS_MFD		7
`define OR1200_UPR_PMP_MFD_BITS_MFD		8
`define OR1200_UPR_PICP_MFD_BITS_MFD		9
`define OR1200_UPR_TTP_MFD_BITS_MFD		10
`define OR1200_UPR_RES1_MFD_BITS_MFD		23:11
`define OR1200_UPR_CUP_MFD_BITS_MFD		31:24

// UPR values
`define OR1200_UPR_UP_MFD			1'b1
`ifdef OR1200_NO_DC_MFD
`define OR1200_UPR_DCP_MFD			1'b0
`else
`define OR1200_UPR_DCP_MFD			1'b1
`endif
`ifdef OR1200_NO_IC_MFD
`define OR1200_UPR_ICP_MFD			1'b0
`else
`define OR1200_UPR_ICP_MFD			1'b1
`endif
`ifdef OR1200_NO_DMMU_MFD
`define OR1200_UPR_DMP_MFD			1'b0
`else
`define OR1200_UPR_DMP_MFD			1'b1
`endif
`ifdef OR1200_NO_IMMU_MFD
`define OR1200_UPR_IMP_MFD			1'b0
`else
`define OR1200_UPR_IMP_MFD			1'b1
`endif
`define OR1200_UPR_MP_MFD			1'b1	// MAC always present
`ifdef OR1200_DU_IMPLEMENTED_MFD
`define OR1200_UPR_DUP_MFD			1'b1
`else
`define OR1200_UPR_DUP_MFD			1'b0
`endif
`define OR1200_UPR_PCUP_MFD			1'b0	// Performance counters not present
`ifdef OR1200_DU_IMPLEMENTED_MFD
`define OR1200_UPR_PMP_MFD			1'b1
`else
`define OR1200_UPR_PMP_MFD			1'b0
`endif
`ifdef OR1200_DU_IMPLEMENTED_MFD
`define OR1200_UPR_PICP_MFD			1'b1
`else
`define OR1200_UPR_PICP_MFD			1'b0
`endif
`ifdef OR1200_DU_IMPLEMENTED_MFD
`define OR1200_UPR_TTP_MFD			1'b1
`else
`define OR1200_UPR_TTP_MFD			1'b0
`endif
`define OR1200_UPR_RES1_MFD			13'h0000
`define OR1200_UPR_CUP_MFD			8'h00

// CPUCFGR fields
`define OR1200_CPUCFGR_NSGF_MFD_BITS_MFD	3:0
`define OR1200_CPUCFGR_HGF_MFD_BITS_MFD	4
`define OR1200_CPUCFGR_OB32S_MFD_BITS_MFD	5
`define OR1200_CPUCFGR_OB64S_MFD_BITS_MFD	6
`define OR1200_CPUCFGR_OF32S_MFD_BITS_MFD	7
`define OR1200_CPUCFGR_OF64S_MFD_BITS_MFD	8
`define OR1200_CPUCFGR_OV64S_MFD_BITS_MFD	9
`define OR1200_CPUCFGR_RES1_MFD_BITS_MFD	31:10

// CPUCFGR values
`define OR1200_CPUCFGR_NSGF_MFD		4'h0
`define OR1200_CPUCFGR_HGF_MFD		1'b0
`define OR1200_CPUCFGR_OB32S_MFD		1'b1
`define OR1200_CPUCFGR_OB64S_MFD		1'b0
`define OR1200_CPUCFGR_OF32S_MFD		1'b0
`define OR1200_CPUCFGR_OF64S_MFD		1'b0
`define OR1200_CPUCFGR_OV64S_MFD		1'b0
`define OR1200_CPUCFGR_RES1_MFD		22'h000000

// DMMUCFGR fields
`define OR1200_DMMUCFGR_NTW_MFD_BITS_MFD	1:0
`define OR1200_DMMUCFGR_NTS_MFD_BITS_MFD	4:2
`define OR1200_DMMUCFGR_NAE_MFD_BITS_MFD	7:5
`define OR1200_DMMUCFGR_CRI_MFD_BITS_MFD	8
`define OR1200_DMMUCFGR_PRI_MFD_BITS_MFD	9
`define OR1200_DMMUCFGR_TEIRI_MFD_BITS_MFD	10
`define OR1200_DMMUCFGR_HTR_MFD_BITS_MFD	11
`define OR1200_DMMUCFGR_RES1_MFD_BITS_MFD	31:12

// DMMUCFGR values
`ifdef OR1200_NO_DMMU_MFD
`define OR1200_DMMUCFGR_NTW_MFD		2'h0	// Irrelevant
`define OR1200_DMMUCFGR_NTS_MFD		3'h0	// Irrelevant
`define OR1200_DMMUCFGR_NAE_MFD		3'h0	// Irrelevant
`define OR1200_DMMUCFGR_CRI_MFD		1'b0	// Irrelevant
`define OR1200_DMMUCFGR_PRI_MFD		1'b0	// Irrelevant
`define OR1200_DMMUCFGR_TEIRI_MFD		1'b0	// Irrelevant
`define OR1200_DMMUCFGR_HTR_MFD		1'b0	// Irrelevant
`define OR1200_DMMUCFGR_RES1_MFD		20'h00000
`else
`define OR1200_DMMUCFGR_NTW_MFD		2'h0	// 1 TLB way
`define OR1200_DMMUCFGR_NTS_MFD 3'h`OR1200_DTLB_INDX_MFDW_MFD	// Num TLB sets
`define OR1200_DMMUCFGR_NAE_MFD		3'h0	// No ATB entries
`define OR1200_DMMUCFGR_CRI_MFD		1'b0	// No control register
`define OR1200_DMMUCFGR_PRI_MFD		1'b0	// No protection reg
`define OR1200_DMMUCFGR_TEIRI_MFD		1'b1	// TLB entry inv reg impl.
`define OR1200_DMMUCFGR_HTR_MFD		1'b0	// No HW TLB reload
`define OR1200_DMMUCFGR_RES1_MFD		20'h00000
`endif

// IMMUCFGR fields
`define OR1200_IMMUCFGR_NTW_MFD_BITS_MFD	1:0
`define OR1200_IMMUCFGR_NTS_MFD_BITS_MFD	4:2
`define OR1200_IMMUCFGR_NAE_MFD_BITS_MFD	7:5
`define OR1200_IMMUCFGR_CRI_MFD_BITS_MFD	8
`define OR1200_IMMUCFGR_PRI_MFD_BITS_MFD	9
`define OR1200_IMMUCFGR_TEIRI_MFD_BITS_MFD	10
`define OR1200_IMMUCFGR_HTR_MFD_BITS_MFD	11
`define OR1200_IMMUCFGR_RES1_MFD_BITS_MFD	31:12

// IMMUCFGR values
`ifdef OR1200_NO_IMMU_MFD
`define OR1200_IMMUCFGR_NTW_MFD		2'h0	// Irrelevant
`define OR1200_IMMUCFGR_NTS_MFD		3'h0	// Irrelevant
`define OR1200_IMMUCFGR_NAE_MFD		3'h0	// Irrelevant
`define OR1200_IMMUCFGR_CRI_MFD		1'b0	// Irrelevant
`define OR1200_IMMUCFGR_PRI_MFD		1'b0	// Irrelevant
`define OR1200_IMMUCFGR_TEIRI_MFD		1'b0	// Irrelevant
`define OR1200_IMMUCFGR_HTR_MFD		1'b0	// Irrelevant
`define OR1200_IMMUCFGR_RES1_MFD		20'h00000
`else
`define OR1200_IMMUCFGR_NTW_MFD		2'h0	// 1 TLB way
`define OR1200_IMMUCFGR_NTS_MFD 3'h`OR1200_ITLB_INDX_MFDW_MFD	// Num TLB sets
`define OR1200_IMMUCFGR_NAE_MFD		3'h0	// No ATB entry
`define OR1200_IMMUCFGR_CRI_MFD		1'b0	// No control reg
`define OR1200_IMMUCFGR_PRI_MFD		1'b0	// No protection reg
`define OR1200_IMMUCFGR_TEIRI_MFD		1'b1	// TLB entry inv reg impl
`define OR1200_IMMUCFGR_HTR_MFD		1'b0	// No HW TLB reload
`define OR1200_IMMUCFGR_RES1_MFD		20'h00000
`endif

// DCCFGR fields
`define OR1200_DCCFGR_NCW_MFD_BITS_MFD		2:0
`define OR1200_DCCFGR_NCS_MFD_BITS_MFD		6:3
`define OR1200_DCCFGR_CBS_MFD_BITS_MFD		7
`define OR1200_DCCFGR_CWS_MFD_BITS_MFD		8
`define OR1200_DCCFGR_CCRI_MFD_BITS_MFD		9
`define OR1200_DCCFGR_CBIRI_MFD_BITS_MFD	10
`define OR1200_DCCFGR_CBPRI_MFD_BITS_MFD	11
`define OR1200_DCCFGR_CBLRI_MFD_BITS_MFD	12
`define OR1200_DCCFGR_CBFRI_MFD_BITS_MFD	13
`define OR1200_DCCFGR_CBWBRI_MFD_BITS_MFD	14
`define OR1200_DCCFGR_RES1_MFD_BITS_MFD	31:15

// DCCFGR values
`ifdef OR1200_NO_DC_MFD
`define OR1200_DCCFGR_NCW_MFD		3'h0	// Irrelevant
`define OR1200_DCCFGR_NCS_MFD		4'h0	// Irrelevant
`define OR1200_DCCFGR_CBS_MFD		1'b0	// Irrelevant
`define OR1200_DCCFGR_CWS_MFD		1'b0	// Irrelevant
`define OR1200_DCCFGR_CCRI_MFD		1'b1	// Irrelevant
`define OR1200_DCCFGR_CBIRI_MFD		1'b1	// Irrelevant
`define OR1200_DCCFGR_CBPRI_MFD		1'b0	// Irrelevant
`define OR1200_DCCFGR_CBLRI_MFD		1'b0	// Irrelevant
`define OR1200_DCCFGR_CBFRI_MFD		1'b1	// Irrelevant
`define OR1200_DCCFGR_CBWBRI_MFD		1'b0	// Irrelevant
`define OR1200_DCCFGR_RES1_MFD		17'h00000
`else
`define OR1200_DCCFGR_NCW_MFD		3'h0	// 1 cache way
`define OR1200_DCCFGR_NCS_MFD (`OR1200_DCTAG_MFD)	// Num cache sets
`define OR1200_DCCFGR_CBS_MFD (`OR1200_DCLS_MFD-4)	// 16 byte cache block
`define OR1200_DCCFGR_CWS_MFD		1'b0	// Write-through strategy
`define OR1200_DCCFGR_CCRI_MFD		1'b1	// Cache control reg impl.
`define OR1200_DCCFGR_CBIRI_MFD		1'b1	// Cache block inv reg impl.
`define OR1200_DCCFGR_CBPRI_MFD		1'b0	// Cache block prefetch reg not impl.
`define OR1200_DCCFGR_CBLRI_MFD		1'b0	// Cache block lock reg not impl.
`define OR1200_DCCFGR_CBFRI_MFD		1'b1	// Cache block flush reg impl.
`define OR1200_DCCFGR_CBWBRI_MFD		1'b0	// Cache block WB reg not impl.
`define OR1200_DCCFGR_RES1_MFD		17'h00000
`endif

// ICCFGR fields
`define OR1200_ICCFGR_NCW_MFD_BITS_MFD		2:0
`define OR1200_ICCFGR_NCS_MFD_BITS_MFD		6:3
`define OR1200_ICCFGR_CBS_MFD_BITS_MFD		7
`define OR1200_ICCFGR_CWS_MFD_BITS_MFD		8
`define OR1200_ICCFGR_CCRI_MFD_BITS_MFD		9
`define OR1200_ICCFGR_CBIRI_MFD_BITS_MFD	10
`define OR1200_ICCFGR_CBPRI_MFD_BITS_MFD	11
`define OR1200_ICCFGR_CBLRI_MFD_BITS_MFD	12
`define OR1200_ICCFGR_CBFRI_MFD_BITS_MFD	13
`define OR1200_ICCFGR_CBWBRI_MFD_BITS_MFD	14
`define OR1200_ICCFGR_RES1_MFD_BITS_MFD	31:15

// ICCFGR values
`ifdef OR1200_NO_IC_MFD
`define OR1200_ICCFGR_NCW_MFD		3'h0	// Irrelevant
`define OR1200_ICCFGR_NCS_MFD 		4'h0	// Irrelevant
`define OR1200_ICCFGR_CBS_MFD 		1'b0	// Irrelevant
`define OR1200_ICCFGR_CWS_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CCRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CBIRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CBPRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CBLRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CBFRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CBWBRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_RES1_MFD		17'h00000
`else
`define OR1200_ICCFGR_NCW_MFD		3'h0	// 1 cache way
`define OR1200_ICCFGR_NCS_MFD (`OR1200_ICTAG_MFD)	// Num cache sets
`define OR1200_ICCFGR_CBS_MFD (`OR1200_ICLS_MFD-4)	// 16 byte cache block
`define OR1200_ICCFGR_CWS_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_CCRI_MFD		1'b1	// Cache control reg impl.
`define OR1200_ICCFGR_CBIRI_MFD		1'b1	// Cache block inv reg impl.
`define OR1200_ICCFGR_CBPRI_MFD		1'b0	// Cache block prefetch reg not impl.
`define OR1200_ICCFGR_CBLRI_MFD		1'b0	// Cache block lock reg not impl.
`define OR1200_ICCFGR_CBFRI_MFD		1'b1	// Cache block flush reg impl.
`define OR1200_ICCFGR_CBWBRI_MFD		1'b0	// Irrelevant
`define OR1200_ICCFGR_RES1_MFD		17'h00000
`endif

// DCFGR fields
`define OR1200_DCFGR_NDP_MFD_BITS_MFD		2:0
`define OR1200_DCFGR_WPCI_MFD_BITS_MFD		3
`define OR1200_DCFGR_RES1_MFD_BITS_MFD		31:4

// DCFGR values
`define OR1200_DCFGR_NDP_MFD		3'h0	// Zero DVR/DCR pairs
`define OR1200_DCFGR_WPCI_MFD		1'b0	// WP counters not impl.
`define OR1200_DCFGR_RES1_MFD		28'h0000000
