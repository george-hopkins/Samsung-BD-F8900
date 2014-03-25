/**
 *   @mainpage   Flex Sector Remapper : LinuStoreIII_1.2.0_b035-FSR_1.2.1p1_b129_RTM
 *
 *   @section Intro
 *       Flash Translation Layer for Flex-OneNAND and OneNAND
 *    
 *    @section  Copyright
 *            COPYRIGHT. 2008-2009 SAMSUNG ELECTRONICS CO., LTD.               
 *                            ALL RIGHTS RESERVED                              
 *                                                                             
 *     Permission is hereby granted to licensees of Samsung Electronics        
 *     Co., Ltd. products to use or abstract this computer program for the     
 *     sole purpose of implementing a product based on Samsung                 
 *     Electronics Co., Ltd. products. No other rights to reproduce, use,      
 *     or disseminate this computer program, whether in part or in whole,      
 *     are granted.                                                            
 *                                                                             
 *     Samsung Electronics Co., Ltd. makes no representation or warranties     
 *     with respect to the performance of this computer program, and           
 *     specifically disclaims any responsibility for any damages,              
 *     special or consequential, connected with the use of this program.       
 *
 *     @section Description
 *
 */

/**
 * @file        FSR_LLD_OMAP2430.h
 * @brief       This file contain the Platform Adaptation Modules for OMAP2430
 * @author      JinHo Yi, JinHyuck Kim
 * @date        15-SEP-2009
 * @remark
 * REVISION HISTORY
 * @n   28-JAN-2008 [JinHo Yi] 	   : First writing
 * @n   15-SEP-2009 [JinHyuck Kim] : Update for FSR LLD
 *
 */

#ifndef _FSR_LLD_MSTAR_X13_
#define _FSR_LLD_MSTAR_X13_

#include "FSR.h"

#define NAND_DRV_LINUX						1

#define REG_BANK_CLKGEN						(0x580)
#define REG_BANK_CHIPTOP					(0xF00)
#define REG_BANK_FCIE0						(0x8980)
#define REG_BANK_FCIE1						(0x8A00)
#define REG_BANK_TIMER0						(0x1800)

#define RIU_PM_BASE							0xBF000000
#define RIU_BASE							0xBF200000

#define REG(Reg_Addr)						(*(volatile UINT16*)(Reg_Addr))
#define REG32(Reg_Addr)						(*(volatile UINT32*)(Reg_Addr))
#define REG_OFFSET_SHIFT_BITS				2 
#define GET_REG_ADDR(x, y)					(x+((y)<<REG_OFFSET_SHIFT_BITS))

#define MPLL_CLK_REG_BASE_ADDR				GET_REG_ADDR(RIU_BASE,REG_BANK_CLKGEN)
#define CHIPTOP_REG_BASE_ADDR				GET_REG_ADDR(RIU_BASE,REG_BANK_CHIPTOP)
#define FCIE_REG_BASE_ADDR					GET_REG_ADDR(RIU_BASE,REG_BANK_FCIE0)
#define FCIE_NC_CIFD_BASE					GET_REG_ADDR(RIU_BASE,REG_BANK_FCIE1)
#define TIMER0_REG_BASE_ADDR				GET_REG_ADDR(RIU_PM_BASE,REG_BANK_TIMER0)

#define REG_WRITE_UINT16(reg_addr, val)		REG(reg_addr) = val
#define REG_READ_UINT16(reg_addr, val)		val = REG(reg_addr)
#define HAL_WRITE_UINT16(reg_addr, val)		(REG(reg_addr) = val)
#define HAL_READ_UINT16(reg_addr, val)		val = REG(reg_addr)
#define REG_SET_BITS_UINT16(reg_addr, val)	REG(reg_addr) |= (val)
#define REG_CLR_BITS_UINT16(reg_addr, val)	REG(reg_addr) &= ~(val)
#define REG_W1C_BITS_UINT16(reg_addr, val)	REG_WRITE_UINT16(reg_addr, REG(reg_addr)&(val))

#define REG_SET_BITS_UINT32(reg_addr, mask, val)	REG32(reg_addr) = (((mask)<<16)|((val)&0xFFFF))
#define REG_CLR_BITS_UINT32(reg_addr, mask, val)	REG32(reg_addr) = ((mask)<<16)

#define NC_SEL_FCIE3						1 
#define FCIE4_DDR                           1

#define IF_IP_VERIFY						0

#define BIT0								(1<<0)
#define BIT1								(1<<1)
#define BIT2								(1<<2)
#define BIT3								(1<<3)
#define BIT4								(1<<4)
#define BIT5								(1<<5)
#define BIT6								(1<<6)
#define BIT7								(1<<7)
#define BIT8								(1<<8)
#define BIT9								(1<<9)
#define BIT10								(1<<10)
#define BIT11								(1<<11)
#define BIT12								(1<<12)
#define BIT13								(1<<13)
#define BIT14								(1<<14)
#define BIT15								(1<<15)
#define BIT16								(1<<16)
#define BIT17								(1<<17)
#define BIT18								(1<<18)
#define BIT19								(1<<19)
#define BIT20								(1<<20)
#define BIT21								(1<<21)
#define BIT22								(1<<22)
#define BIT23								(1<<23)
#define BIT24								(1<<24)
#define BIT25								(1<<25)
#define BIT26								(1<<26)
#define BIT27								(1<<27)
#define BIT28								(1<<28)
#define BIT29								(1<<29)
#define BIT30								(1<<30)
#define BIT31								(1<<31)

#define ECC_TYPE_RS							1
#define ECC_TYPE_4BIT						2
#define ECC_TYPE_8BIT						3
#define ECC_TYPE_12BIT						4
#define ECC_TYPE_16BIT						5
#define ECC_TYPE_20BIT						6
#define ECC_TYPE_24BIT						7
#define ECC_TYPE_24BIT1KB					8
#define ECC_TYPE_32BIT1KB					9
#define ECC_TYPE_40BIT1KB					10

#define ECC_CODE_BYTECNT_RS					10
#if defined(FCIE4_DDR) && FCIE4_DDR
#define ECC_CODE_BYTECNT_4BIT				8
#define ECC_CODE_BYTECNT_8BIT				14
#else
#define ECC_CODE_BYTECNT_4BIT				7
#define ECC_CODE_BYTECNT_8BIT				13
#endif
#define ECC_CODE_BYTECNT_12BIT				20
#define ECC_CODE_BYTECNT_16BIT				26
#if defined(FCIE4_DDR) && FCIE4_DDR
#define ECC_CODE_BYTECNT_20BIT				34
#define ECC_CODE_BYTECNT_24BIT				40
#else
#define ECC_CODE_BYTECNT_20BIT				33
#define ECC_CODE_BYTECNT_24BIT				39
#endif
#define ECC_CODE_BYTECNT_24BIT1KB			42
#define ECC_CODE_BYTECNT_32BIT1KB			56
#define ECC_CODE_BYTECNT_40BIT1KB			70

#define IF_SPARE_AREA_DMA					0 // [CAUTION]

#define NC_MIE_EVENT						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x00)
#define NC_MIE_INT_EN						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x01)
#define NC_MMA_PRI_REG						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x02)
#define NC_MIU_DMA_SEL						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x03)
#define NC_CARD_DET							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x07)
#define NC_FORCE_INT						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x08)
#define NC_PATH_CTL							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0A)
#define NC_JOB_BL_CNT						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0B)
#define NC_TR_BK_CNT						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x0C)
#define NC_SDIO_CTL							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1B)
#define NC_SDIO_ADDR0						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1C)
#define NC_SDIO_ADDR1						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1D)
#define NC_R2N_STAT							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1F)
#define NC_R2N_CTRL							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x20)
#define NC_R2N_DATA_RD						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x21)
#define NC_R2N_DATA_WR						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x22)
#define NC_CIF_FIFO_CTL						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x25)
#define NC_SM_STS							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2C)
#define FCIE_NC_REORDER						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2D)
#define NC_MIU_OFFSET						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2E)
#define NC_REG_PAD_SWITCH					GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x2F)
#define NC_TEST_MODE						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x30)
#define NC_DEBUG_DBUS0						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x31)
#define NC_DEBUG_DBUS1						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x32)
#define NC_REG_33h							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x33)
#define NC_PWRSAVE_MASK						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x34)
#define NC_PWRSAVE_CTL						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x35)
#define NC_ECO_BUG_PATCH					GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x36)
#define NC_CELL_DELAY						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x37)
#define NC_MIU_WPRT_L1						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x38)
#define NC_MIU_WPRT_L0						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x39)
#define NC_MIU_WPRT_H1						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3A)
#define NC_MIU_WPRT_H0						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3B)
#define NC_MIU_WPRT_ER1						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3C)
#define NC_MIU_WPRT_ER0						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3D)
#define NC_FCIE_VERSION						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x3F)
#define NC_SIGNAL							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x40)
#define NC_WIDTH							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x41)
#define NC_STAT_CHK							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x42)
#define NC_AUXREG_ADR						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x43)
#define NC_AUXREG_DAT						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x44)
#define NC_CTRL								GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x45)
#define NC_ST_READ							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x46)
#define NC_PART_MODE						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x47)
#define NC_SPARE							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x48)
#define NC_SPARE_SIZE						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x49)
#define NC_ADDEND							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4A)
#define NC_SIGN_DAT							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4B)
#define NC_SIGN_ADR							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4C)
#define NC_SIGN_CTRL						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4D)
#define NC_SPARE_DMA_ADR0					GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4E)
#define NC_SPARE_DMA_ADR1					GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4F)
#define NC_ECC_CTRL							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x50)
#define NC_ECC_STAT0						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x51)   
#define NC_ECC_STAT1						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x52)   
#define NC_ECC_STAT2						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x53)   
#define NC_ECC_LOC							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x54)   
#define NC_RAND_R_CMD						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x55)   
#define NC_RAND_W_CMD						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x56)
#define NC_LATCH_DATA						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x57)
#define NC_DDR_CTRL							GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x58)
#define NC_LFSR_CTRL						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x59)
#define NC_NAND_TIMING						GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x5A)
#define NC_SER_DIN_BYTECNT_LW				GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x5B)
#define NC_SER_DIN_BYTECNT_HW				GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x5C)

#define NC_CIFD_ADDR(u16_pos)				GET_REG_ADDR(FCIE_NC_CIFD_BASE, u16_pos) // 256 x 16 bits
#define NC_CIFD_BYTE_CNT					0x200 // 256 x 16 bits

#define NC_MAX_SECTOR_BYTE_CNT				(BIT12-1)
#define NC_MAX_SECTOR_SPARE_BYTE_CNT		(BIT8-1)
#define NC_MAX_TOTAL_SPARE_BYTE_CNT			(BIT11-1)

/* NC_MIE_EVENT 0x00 */
/* NC_MIE_INT_EN 0x01 */
#define BIT_MMA_DATA_END					BIT0  
#define R_MIU_WP_ERR						BIT8
#define BIT_NC_JOB_END						BIT9
#define BIT_NC_R2N_RDY						BIT10
#define BIT_NC_R2N_ECC						BIT12
#define BIT_MIU_LAST_DONE					BIT14

/* NC_MMA_PRI_REG 0x02 */
#define BIT_NC_DMA_DIR_W					BIT2
#define BIT_MIU_REQUEST_RST					BIT4 
#define BIT_NC_FIFO_CLKRDY					BIT5 
#define BIT_NC_MIU_BURST_MASK				(BIT10|BIT9|BIT8)
#define BIT_NC_MIU_BURST_CTRL				BIT10
#define BIT_NC_MIU_BURST_8					BIT_NC_MIU_BURST_CTRL
#define BIT_NC_MIU_BURST_16					(BIT8|BIT_NC_MIU_BURST_CTRL)
#define BIT_NC_MIU_BURST_32					(BIT9|BIT_NC_MIU_BURST_CTRL)

/* NC_MIU_DMA_SEL 0x03 */
#define BIT_NC_MIU_SELECT					BIT15

/* NC_CARD_DET 0x7 */
#define BIT_NC_NF_RBZ_STS					BIT8
/* NC_FORCE_INT 0x08 */
#define BIT_NC_JOB_INTR						BIT7

/* NC_PATH_CTL 0x0A */
#define BIT_MMA_EN							BIT0  
#define BIT_NC_EN							BIT5

/* NC_SDIO_CTL 0x1B */
#define MASK_SDIO_BLK_SIZE_MASK				(BIT12-1)
#define BIT_SDIO_BLK_MODE					BIT15

/* NC_R2N_CTRL_GET 0x1F */
#define BIT_RIU_RDY_MMA						BIT0

/* NC_R2N_CTRL_SET 0x20 */
#define BIT_R2N_MODE_EN						BIT0
#define BIT_R2N_DI_START					BIT1
#define BIT_R2N_DI_EN						BIT2
#define BIT_R2N_DI_END						BIT3
#define BIT_R2N_DO_START					BIT4
#define BIT_R2N_DO_EN						BIT5
#define BIT_R2N_DO_END						BIT6

/* NC_CIF_FIFO_CTL 0x25 */
#define BIT_CIFD_RD_REQ						BIT1

/* NC_TEST_MODE 0x30 */
#define BIT_FCIE_BIST_FAIL					(BIT0|BIT1|BIT2|BIT3)
#define BIT_FCIE_DEBUG_MODE_SHIFT			8
#define BIT_FCIE_DEBUG_MODE					(BIT10|BIT9|BIT8)
#define BIT_FCIE_SOFT_RST					BIT12
#define BIT_FCIE_PPFIFO_CLK					BIT14

/* NC_FCIE_VERSION 0x3F */
#define BIT_NFIE_INSIDE						BIT5

/* NC_SIGNAL 0x40 */
#define BIT_NC_CE_SEL_MASK					(BIT2-1)
#define BIT_NC_CE_H							BIT2  
#define BIT_NC_CE_AUTO						BIT3
#define BIT_NC_WP_H							BIT4
#define BIT_NC_WP_AUTO						BIT5
#define BIT_NC_CHK_RB_HIGH					BIT6
#define BIT_NC_CHK_RB_EDGEn					BIT7
#define DEF_REG0x40_VAL						BIT_NC_CE_H

/* NC_WIDTH 0x41 */
#define BIT_NC_DEB_SEL_SHIFT				12
#define BIT_NC_DEB_SEL						(BIT12|BIT13|BIT14)
#define BIT_NC_BCH_DEB_SEL					BIT15

/* NC_CTRL 0x45 */
#define BIT_NC_JOB_START					BIT0
#define BIT_NC_CIFD_ACCESS					BIT1
#define BIT_NC_IF_DIR_W						BIT3

/* NC_ST_READ 0x46 */
#define BIT_ST_READ_FAIL					BIT0
#define BIT_ST_READ_BUSYn					BIT6
#define BIT_ST_READ_PROTECTn				BIT7

/* NC_PART_MODE 0x47*/
#define BIT_PARTIAL_MODE_EN					BIT0
#define BIT_START_SECTOR_CNT_MASK			(BIT6|BIT5|BIT4|BIT3|BIT2|BIT1)
#define BIT_START_SECTOR_CNT_SHIFT			1
#define BIT_START_SECTOR_IDX_MASK			(BIT12|BIT11|BIT10|BIT9|BIT8|BIT7)
#define BIT_START_SECTOR_IDX_SHIFT			7

/* NC_SPARE 0x48 */
#define BIT_NC_SECTOR_SPARE_SIZE_MASK		(BIT8-1)
#define BIT_NC_SPARE_DEST_MIU				BIT8
#define BIT_NC_SPARE_NOT_R_IN_CIFD			BIT9
#define BIT_NC_RANDOM_RW_OP_EN				BIT11
#define BIT_NC_HW_AUTO_RANDOM_CMD_DISABLE	BIT12
#define BIT_NC_ONE_COL_ADDR					BIT13

/* NC_SIGN_CTRL 0x4D*/
#define BIT_NC_SIGN_CHECK_EN				BIT0
#define BIT_NC_SIGN_STOP_RUN				BIT1
#define BIT_NC_SIGN_CLR_STATUS				BIT2
#define BIT_NC_SIGN_DAT0_MISS				BIT3
#define BIT_NC_SIGN_DAT1_MISS				BIT4

/* NC_ECC_CTRL 0x50*/
#define BIT_NC_PAGE_SIZE_MASK				(BIT3-1)
#define BIT_NC_PAGE_SIZE_512Bn				BIT_NC_PAGE_SIZE_MASK
#define BIT_NC_PAGE_SIZE_2KB				BIT0
#define BIT_NC_PAGE_SIZE_4KB				BIT1
#define BIT_NC_PAGE_SIZE_8KB				(BIT1|BIT0)
#define BIT_NC_PAGE_SIZE_16KB				BIT2
#define BIT_NC_PAGE_SIZE_32KB				(BIT2|BIT0)
#define BIT_NC_ECC_TYPE_MASK				(BIT3|BIT4|BIT5|BIT6)
#define BIT_NC_ECC_TYPE_4b512Bn				BIT_NC_ECC_TYPE_MASK
#define BIT_NC_ECC_TYPE_8b512B				BIT3
#define BIT_NC_ECC_TYPE_12b512B				BIT4
#define BIT_NC_ECC_TYPE_24b1KB				(BIT5|BIT4)
#define BIT_NC_ECC_TYPE_32b1KB				(BIT5|BIT4|BIT3)
#define BIT_NC_ECC_TYPE_40b1KB				BIT6
#define BIT_NC_ECC_TYPE_RS					BIT6
#define BIT_NC_ECCERR_NSTOP					BIT7
#define BIT_NC_DYNAMIC_OFFCLK				BIT8
#define BIT_NC_ALL0xFF_ECC_CHECK			BIT9
#define BIT_NC_BYPASS_ECC					BIT10

/* NC_ECC_STAT0 0x51*/
#define BIT_NC_ECC_FAIL						BIT0
#define BIT_NC_ECC_MAX_BITS_MASK			(BIT6|BIT5|BIT4|BIT3|BIT2|BIT1)
#define BIT_NC_ECC_MAX_BITS_SHIFT			1
#define BIT_NC_ECC_SEC_CNT_MASK				(BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)

/* NC_ECC_STAT2 0x53*/
#define BIT_NC_ECC_FLGA_MASK				(BIT1|BIT0)
#define BIT_NC_ECC_FLGA_NOERR				0
#define BIT_NC_ECC_FLGA_CORRECT				BIT0
#define BIT_NC_ECC_FLGA_FAIL				BIT1
#define BIT_NC_ECC_FLGA_CODE				BIT_NC_ECC_FLGA_MASK
#define BIT_NC_ECC_CORRECT_CNT				(BIT7|BIT6|BIT5|BIT4|BIT3|BIT2)
#define BIT_NC_ECC_CNT_SHIFT				2
#define BIT_NC_ECC_SEL_LOC_MASK				(BIT12|BIT11|BIT10|BIT9|BIT8)
#define BIT_NC_ECC_SEL_LOC_SHIFT			8

/* NC_RAND_W_CMD 0x56*/	/* for fine tune timing for tADL & random R/W */
#define BIT_NC_HW_DELAY_CNT_MASK			(BIT11|BIT10|BIT9|BIT8)
#define BIT_NC_HW_DELAY_CNT_SHIFT			8

/* NC_LATCH_DATA 0x57*/
#define BIT_NC_LATCH_DATA_MASK				(BIT2|BIT1|BIT0)
#define BIT_NC_LATCH_DATA_NORMAL			0
#define BIT_NC_LATCH_DATA_0_5_T				BIT0
#define BIT_NC_LATCH_DATA_1_0_T				BIT1
#define BIT_NC_LATCH_DATA_1_5_T				(BIT1|BIT0)
#define BIT_NC_LATCH_DATA_2_0_T				BIT2
#define BIT_NC_RD_STS_CNT_MASK				(BIT6|BIT5|BIT4)
#define BIT_NC_RD_STS_CNT_NORMAL			0
#define BIT_NC_RD_STS_CNT_1T				BIT4
#define BIT_NC_RD_STS_CNT_2T				BIT5

/* NC_DDR_CTRL 0x58 */
#define BIT_DDR_MASM               (BIT_DDR_ONFI|BIT_DDR_TOGGLE)
#define BIT_DDR_ONFI               BIT0
#define BIT_DDR_TOGGLE             BIT1
#define BIT_SDR_DIN_FROM_MACRO     BIT3
/* NC_LFSR_CTRL 0x59 */
#define BIT_TRR_CYCLE_CNT_MASK     (BIT0|BIT1|BIT2|BIT3)
#define BIT_TCS_CYCLE_CNT_MASK     (BIT4|BIT5|BIT6|BIT7)
#define BIT_TCS_CYCLE_CNT_SHIFT    4
#define BIT_SEL_PAGE_MASK          (BIT8|BIT9|BIT10|BIT11|BIT12|BIT13|BIT14)
#define BIT_SEL_PAGE_SHIFT         8
#define BIT_LFSR_ENABLE            BIT15

// AUX Reg Address
#define AUXADR_CMDREG8						0x08
#define AUXADR_CMDREG9						0x09
#define AUXADR_CMDREGA						0x0A
#define AUXADR_ADRSET						0x0B 
#define AUXADR_RPTCNT						0x18 // Pepeat Count
#define AUXADR_RAN_CNT						0x19 
#define AUXADR_RAN_POS						0x1A // offset
#define AUXADR_ST_CHECK						0x1B
#define AUXADR_IDLE_CNT						0x1C
#define AUXADR_INSTQUE						0x20

// OP Codes: Command
#define CMD_0x00							0x00 
#define CMD_0x30							0x01
#define CMD_0x35							0x02
#define CMD_0x90							0x03
#define CMD_0xFF							0x04
#define CMD_0x80							0x05
#define CMD_0x10							0x06
#define CMD_0x15							0x07
#define CMD_0x85							0x08
#define CMD_0x60							0x09
#define CMD_0xD0							0x0A
#define CMD_0x05							0x0B
#define CMD_0xE0							0x0C
#define CMD_0x70							0x0D
#define CMD_0x50							0x0E
#define CMD_0x01							0x0F
#define CMD_REG8							0x10
#define CMD_REG9							0x11
#define CMD_REGA							0x12

// OP Code: Address
#define OP_ADR_CYCLE_00						(4<<4)
#define OP_ADR_CYCLE_01						(5<<4)
#define OP_ADR_CYCLE_10						(6<<4)
#define OP_ADR_CYCLE_11						(7<<4)
#define OP_ADR_TYPE_COL						(0<<2)
#define OP_ADR_TYPE_ROW						(1<<2)
#define OP_ADR_TYPE_FUL						(2<<2)
#define OP_ADR_TYPE_ONE						(3<<2)
#define OP_ADR_SET_0						(0<<0)
#define OP_ADR_SET_1						(1<<0)
#define OP_ADR_SET_2						(2<<0)
#define OP_ADR_SET_3						(3<<0)

#define ADR_C2TRS0							(OP_ADR_CYCLE_00|OP_ADR_TYPE_ROW|OP_ADR_SET_0)
#define ADR_C2T1S0							(OP_ADR_CYCLE_00|OP_ADR_TYPE_ONE|OP_ADR_SET_0)
#define ADR_C3TRS0							(OP_ADR_CYCLE_01|OP_ADR_TYPE_ROW|OP_ADR_SET_0)
#define ADR_C3TFS0							(OP_ADR_CYCLE_00|OP_ADR_TYPE_FUL|OP_ADR_SET_0)
#define ADR_C4TFS0							(OP_ADR_CYCLE_01|OP_ADR_TYPE_FUL|OP_ADR_SET_0)
#define ADR_C4TFS1							(OP_ADR_CYCLE_01|OP_ADR_TYPE_FUL|OP_ADR_SET_1)
#define ADR_C5TFS0							(OP_ADR_CYCLE_10|OP_ADR_TYPE_FUL|OP_ADR_SET_0)
#define ADR_C6TFS0							(OP_ADR_CYCLE_11|OP_ADR_TYPE_FUL|OP_ADR_SET_0)

// OP Code: Action
#define ACT_WAITRB							0x80 	
#define ACT_CHKSTATUS						0x81    
#define ACT_WAIT_IDLE						0x82	    
#define ACT_WAIT_MMA						0x83     
#define ACT_BREAK							0x88
#define ACT_SER_DOUT						0x90 // for column addr == 0       	
#define ACT_RAN_DOUT						0x91 // for column addr != 0	
#define ACT_SER_DIN							0x98 // for column addr == 0       
#define ACT_RAN_DIN							0x99 // for column addr != 0
#define ACT_PAGECOPY_US						0xA0 	 
#define ACT_PAGECOPY_DS						0xA1 	
#define ACT_REPEAT							0xB0

#define IF_FCIE_SHARE_PINS					1	// 1: need to nand_pads_switch at HAL's functions.

#define ENABLE_NAND_INTERRUPT_MODE			1

//=====================================================
// Nand Driver configs
//=====================================================
#define NAND_ENV_FPGA						1
#define NAND_ENV_ASIC						2
#define NAND_DRIVER_ENV						NAND_ENV_ASIC /* [CAUTION] */

#define UNFD_CACHE_LINE						0x80
//=====================================================
// tool-chain attributes
//=====================================================
#define UNFD_PACK0
#define UNFD_PACK1							__attribute__((__packed__))
#define UNFD_ALIGN0
#define UNFD_ALIGN1							__attribute__((aligned(UNFD_CACHE_LINE)))

//=====================================================
// debug option
//=====================================================
#define NAND_TEST_IN_DESIGN					0 

#ifndef NAND_DEBUG_MSG
#define NAND_DEBUG_MSG						0
#endif

#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
#define nand_printf							printk
#define printf								printk
#else
#define nand_printf(...)
#define printf(...)
#endif

//=====================================================
// HW Timer for Delay 
//=====================================================
#define TIMER0_ENABLE						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x10)
#define TIMER0_HIT							GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x11)
#define TIMER0_MAX_LOW						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x12)
#define TIMER0_MAX_HIGH						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x13)
#define TIMER0_CAP_LOW						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x14)
#define TIMER0_CAP_HIGH						GET_REG_ADDR(TIMER0_REG_BASE_ADDR, 0x15)

#define HW_TIMER_DELAY_1us					1
#define HW_TIMER_DELAY_10us					10
#define HW_TIMER_DELAY_100us				100
#define HW_TIMER_DELAY_1ms					(1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms					(5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms					(10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms				(100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms				(500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s					(1000 * HW_TIMER_DELAY_1ms)

//=====================================================
// Pads Switch
//=====================================================
#define reg_allpad_in						GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define reg_nf_en							GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)

#define reg_pcm_d_pe                        GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x0B)
#define reg_pcm_a_pe                        GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x0C)

#define NAND_MODE1                          (BIT8)
#define NAND_MODE2                          (BIT9)
#define NAND_MODE                           NAND_MODE1

#define reg_nand_mode                       GET_REG_ADDR(CHIPTOP_REG_BASE_ADDR, 0x50)
#define REG_NAND_MODE_MASK                  (BIT9|BIT8)

//=====================================================
// interrupt
//=====================================================
#define REG_INT_BASE                        0xbf203200
#define reg_hst3_irq_mask_15_0_             GET_REG_ADDR(REG_INT_BASE, 0x74)
#define reg_hst3_irq_polarity_15_0_         GET_REG_ADDR(REG_INT_BASE, 0x78)
#define reg_hst3_irq_status_15_0_           GET_REG_ADDR(REG_INT_BASE, 0x7C)

//=====================================================
// set FCIE clock
//=====================================================
#define NFIE_CLK_MASK						(BIT5|BIT4|BIT3|BIT2)
#define NFIE_CLK_XTAL						(0)
#define NFIE_CLK_5_4M						(BIT2)
#define NFIE_CLK_27M						(BIT3)
#define NFIE_CLK_32M						(BIT3|BIT2)
#define NFIE_CLK_36M						(BIT4)
#define NFIE_CLK_40M						(BIT4|BIT2)
#define NFIE_CLK_43M						(BIT4|BIT3)
#define NFIE_CLK_54M						(BIT4|BIT3|BIT2)
#define NFIE_CLK_62M						(BIT5)
#define NFIE_CLK_72M						(BIT5|BIT2)
#define NFIE_CLK_80M						(BIT5|BIT3)
#define NFIE_CLK_86M						(BIT5|BIT3|BIT2)
#define NFIE_CLK_SSC						(BIT5|BIT4)

#define DUTY_CYCLE_PATCH					0 // 1: to enlarge low width for tREA's worst case of 25ns
#if DUTY_CYCLE_PATCH 
#define FCIE3_SW_DEFAULT_CLK				NFIE_CLK_86M
#define FCIE_REG41_VAL						((1<<9)|(1<<3)) // RE,WR pulse, Low:High=2:1
#define REG57_ECO_FIX_INIT_VALUE			0
#else
#define FCIE3_SW_DEFAULT_CLK				NFIE_CLK_62M
#define FCIE_REG41_VAL						0               // RE,WR pulse, Low:High=1:1
#define REG57_ECO_FIX_INIT_VALUE			BIT_NC_LATCH_DATA_0_5_T	// delay 0.5T
#endif
#define FCIE3_SW_SLOWEST_CLK				NFIE_CLK_5_4M

//#define NAND_SEQ_ACC_TIME_TOL				10 //in unit of ns

#define reg_ckg_fcie						GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x64)
#define BIT_CLK_GATING                      BIT0
#define BIT_CLK_INVERSE                     BIT1

//=====================================================
// MIU
//=====================================================
#define reg_ckg_miu							GET_REG_ADDR(MPLL_CLK_REG_BASE_ADDR, 0x1f)

#define NC_REG_MIU_LAST_DONE				NC_MIE_EVENT

//===========================================================
// driver parameters
//===========================================================
#define NAND_ID_BYTE_CNT					15

//=====================================================
// misc. do NOT edit the following content.
//=====================================================
#define NAND_DMA_RACING_PATCH				0
#define NAND_DMA_PATCH_WAIT_TIME			10000 // us -> 10ms
#define NAND_DMA_RACING_PATTERN				(((UINT32)'D'<<24)|((UINT32)'M'<<16)|((UINT32)'B'<<8)|(UINT32)'N')

//===========================================================
// Time Dalay, do NOT edit the following content, for NC_WaitComplete use.
//===========================================================
#define DELAY_100us_in_us					100
#define DELAY_500us_in_us					500
#define DELAY_1ms_in_us						1000
#define DELAY_10ms_in_us					10000
#define DELAY_100ms_in_us					100000
#define DELAY_500ms_in_us					500000
#define DELAY_1s_in_us						1000000

#define WAIT_ERASE_TIME						DELAY_1s_in_us
#define WAIT_WRITE_TIME						DELAY_1s_in_us
#define WAIT_READ_TIME						DELAY_500ms_in_us
#define WAIT_RESET_TIME						DELAY_10ms_in_us

/*****************************************************************************/
/* NAND Controller  Masking value Definitions                                */
/*****************************************************************************/
#define GPMC_nCS0                           0
#define GPMC_nCS1                           1
#define GPMC_nCS2                           2
#define GPMC_nCS3                           3
#define GPMC_nCS4                           4
#define GPMC_nCS5                           5
#define GPMC_nCS6                           6
#define GPMC_nCS7                           7

#define WAITx_ACTIVE_HIGH                   0x1
#define NAND_FLASH_LIKE_DEVICES             0x1
#define WAIT_INPUT_PIN_IS_WAIT_(x)          (x)
#define NAND_FLASH_STATUS_BUSY_MASK_(x)     (1 << (x + 8))

/*****************************************************************************/
/* NAND Controller Register Address Definitions                              */
/*****************************************************************************/
#define GPMC_BASE							0x6E000000

#define GPMC_SYSCONFIG						*(volatile UINT32 *)(GPMC_BASE + 0x010)
#define GPMC_SYSSTATUS						*(volatile UINT32 *)(GPMC_BASE + 0x014)
#define GPMC_IRQSTATUS						*(volatile UINT32 *)(GPMC_BASE + 0x018)
#define GPMC_IRQENABLE						*(volatile UINT32 *)(GPMC_BASE + 0x01C)
#define GPMC_TIMEOUT_CONTROL				*(volatile UINT32 *)(GPMC_BASE + 0x040)
#define GPMC_ERR_ADDRESS					*(volatile UINT32 *)(GPMC_BASE + 0x044)
#define GPMC_ERR_TYPE						*(volatile UINT32 *)(GPMC_BASE + 0x048)
#define GPMC_CONFIG							*(volatile UINT32 *)(GPMC_BASE + 0x050)
#define GPMC_STATUS							*(volatile UINT32 *)(GPMC_BASE + 0x054)

#define GPMC_CONFIG1_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x060 + (0x30 * x))
#define GPMC_CONFIG2_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x064 + (0x30 * x))
#define GPMC_CONFIG3_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x068 + (0x30 * x))
#define GPMC_CONFIG4_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x06C + (0x30 * x))
#define GPMC_CONFIG5_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x070 + (0x30 * x))
#define GPMC_CONFIG6_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x074 + (0x30 * x))
#define GPMC_CONFIG7_(x)					*(volatile UINT32 *)(GPMC_BASE + 0x078 + (0x30 * x))

#define GPMC_NAND_COMMAND_(x)				*(volatile UINT16 *)(GPMC_BASE + 0x07C + (0x30 * x))
#define GPMC_NAND_ADDRESS_(x)				*(volatile UINT16 *)(GPMC_BASE + 0x080 + (0x30 * x))
#define GPMC_NAND_DATA_(x)					(GPMC_BASE + 0x084 + (0x30 * x))

#define GPMC_PREFETCH_CONFIG1				*(volatile UINT32 *)(GPMC_BASE + 0x1E0)
#define GPMC_PREFETCH_CONFIG2				*(volatile UINT32 *)(GPMC_BASE + 0x1E4)
#define GPMC_PREFETCH_CONTROL				*(volatile UINT32 *)(GPMC_BASE + 0x1EC)
#define GPMC_PREFETCH_STATUS				*(volatile UINT32 *)(GPMC_BASE + 0x1F0)

#define GPMC_ECC_CONFIG						*(volatile UINT32 *)(GPMC_BASE + 0x1F4)
#define GPMC_ECC_CONTROL					*(volatile UINT32 *)(GPMC_BASE + 0x1F8)
#define GPMC_ECC_SIZE_CONFIG				*(volatile UINT32 *)(GPMC_BASE + 0x1FC)

#define _FSR_LLD_MSTAR_X13
#define NAND_ECC_TYPE						ECC_TYPE_4BIT

#define X9_FSR_MAX_PHY_SCTS					8
#define FSR_LLD_ENABLE_CTRLLER_ECC


//===========================================================
// error codes
//===========================================================
#define  UNFD_ST_SUCCESS					0

#define  UNFD_ERRCODE_GROUP_MASK			0xF0000000

//===========================================================
// for LINUX functions
//===========================================================
#if defined(NAND_DRV_LINUX) && NAND_DRV_LINUX
#define  UNFD_ST_LINUX						0xC0000000
#define  UNFD_ST_ERR_NO_NFIE				(0x01 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_E_TIMEOUT				(0x07 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_W_TIMEOUT				(0x08 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_R_TIMEOUT				(0x09 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_ECC_FAIL				(0x0F | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_RST_TIMEOUT			(0x12 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_MIU_RPATCH_TIMEOUT		(0x13 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_HAL_R_INVALID_PARAM	(0x14 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_HAL_W_INVALID_PARAM	(0x15 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_R_TIMEOUT_RM			(0x16 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_ECC_FAIL_RM			(0x17 | UNFD_ST_LINUX)
#define  UNFD_ST_ERR_W_TIMEOUT_RM			(0x18 | UNFD_ST_LINUX)
#endif

//===========================================================
// for HAL functions
//===========================================================
#define  UNFD_ST_RESERVED					0xE0000000
#define  UNFD_ST_ERR_E_FAIL					(0x01 | UNFD_ST_RESERVED)
#define  UNFD_ST_ERR_W_FAIL					(0x02 | UNFD_ST_RESERVED)
#define  UNFD_ST_ERR_E_BUSY					(0x03 | UNFD_ST_RESERVED)
#define  UNFD_ST_ERR_W_BUSY					(0x04 | UNFD_ST_RESERVED)
#define  UNFD_ST_ERR_E_PROTECTED			(0x05 | UNFD_ST_RESERVED)
#define  UNFD_ST_ERR_W_PROTECTED			(0x06 | UNFD_ST_RESERVED)

//===========================================================
// for IP_Verify functions
//===========================================================
#define  UNFD_ST_IP							0xF0000000
#define  UNFD_ST_ERR_UNKNOWN_ID				(0x01 | UNFD_ST_IP)
#define  UNFD_ST_ERR_DATA_CMP_FAIL			(0x02 | UNFD_ST_IP)
#define  UNFD_ST_ERR_INVALID_PARAM			(0x03 | UNFD_ST_IP)
#define  UNFD_ST_ERR_INVALID_ECC_REG51		(0x04 | UNFD_ST_IP)
#define  UNFD_ST_ERR_INVALID_ECC_REG52		(0x05 | UNFD_ST_IP)
#define  UNFD_ST_ERR_INVALID_ECC_REG53		(0x06 | UNFD_ST_IP)
#define  UNFD_ST_ERR_SIGNATURE_FAIL			(0x07 | UNFD_ST_IP)

#endif /* #define _FSR_PAM_MSTAR_X13_ */

