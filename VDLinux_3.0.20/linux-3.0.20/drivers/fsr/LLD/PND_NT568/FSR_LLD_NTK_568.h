/**
 *   @mainpage   Flex Sector Remapper : LinuStoreIII_1.2.0_b032-FSR_1.2.1p1_b129_RTM
 *
 *   @section Intro
 *       Flash Translation Layer for Flex-OneNAND and OneNAND
 *   
 *     @MULTI_BEGIN@ @COPYRIGHT_DEFAULT
 *     @section Copyright COPYRIGHT_DEFAULT
 *            COPYRIGHT. SAMSUNG ELECTRONICS CO., LTD.
 *                                    ALL RIGHTS RESERVED
 *     Permission is hereby granted to licensees of Samsung Electronics Co., Ltd. products
 *     to use this computer program only in accordance 
 *     with the terms of the SAMSUNG FLASH MEMORY DRIVER SOFTWARE LICENSE AGREEMENT.
 *     @MULTI_END@
 *
 *      
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
 * @n   28-JAN-2008 [JinHo Yi]        : First writing
 * @n   15-SEP-2009 [JinHyuck Kim] : Update for FSR LLD
 *
 */

#ifndef _FSR_LLD_NTK_568_
#define _FSR_LLD_NTK_568_

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
#define GPMC_BASE               0x6E000000

#define GPMC_SYSCONFIG          *(volatile UINT32 *)(GPMC_BASE + 0x010)
#define GPMC_SYSSTATUS          *(volatile UINT32 *)(GPMC_BASE + 0x014)
#define GPMC_IRQSTATUS          *(volatile UINT32 *)(GPMC_BASE + 0x018)
#define GPMC_IRQENABLE          *(volatile UINT32 *)(GPMC_BASE + 0x01C)
#define GPMC_TIMEOUT_CONTROL    *(volatile UINT32 *)(GPMC_BASE + 0x040)
#define GPMC_ERR_ADDRESS        *(volatile UINT32 *)(GPMC_BASE + 0x044)
#define GPMC_ERR_TYPE           *(volatile UINT32 *)(GPMC_BASE + 0x048)
#define GPMC_CONFIG             *(volatile UINT32 *)(GPMC_BASE + 0x050)
#define GPMC_STATUS             *(volatile UINT32 *)(GPMC_BASE + 0x054)

#define GPMC_CONFIG1_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x060 + (0x30 * x))
#define GPMC_CONFIG2_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x064 + (0x30 * x))
#define GPMC_CONFIG3_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x068 + (0x30 * x))
#define GPMC_CONFIG4_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x06C + (0x30 * x))
#define GPMC_CONFIG5_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x070 + (0x30 * x))
#define GPMC_CONFIG6_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x074 + (0x30 * x))
#define GPMC_CONFIG7_(x)        *(volatile UINT32 *)(GPMC_BASE + 0x078 + (0x30 * x))

#define GPMC_NAND_COMMAND_(x)   *(volatile UINT16 *)(GPMC_BASE + 0x07C + (0x30 * x))
#define GPMC_NAND_ADDRESS_(x)   *(volatile UINT16 *)(GPMC_BASE + 0x080 + (0x30 * x))
#define GPMC_NAND_DATA_(x)      (GPMC_BASE + 0x084 + (0x30 * x))

#define GPMC_PREFETCH_CONFIG1   *(volatile UINT32 *)(GPMC_BASE + 0x1E0)
#define GPMC_PREFETCH_CONFIG2   *(volatile UINT32 *)(GPMC_BASE + 0x1E4)
#define GPMC_PREFETCH_CONTROL   *(volatile UINT32 *)(GPMC_BASE + 0x1EC)
#define GPMC_PREFETCH_STATUS    *(volatile UINT32 *)(GPMC_BASE + 0x1F0)

#define GPMC_ECC_CONFIG         *(volatile UINT32 *)(GPMC_BASE + 0x1F4)
#define GPMC_ECC_CONTROL        *(volatile UINT32 *)(GPMC_BASE + 0x1F8)
#define GPMC_ECC_SIZE_CONFIG    *(volatile UINT32 *)(GPMC_BASE + 0x1FC)


//================================================================================
//================================================================================
//TODO rename this.
#define ntk_readb(offset)       (*(volatile unsigned char *)(offset))
#define ntk_readw(offset)       (*(volatile unsigned short *)(offset))
#define ntk_readl(offset)       (*(volatile unsigned long *)(offset))

#define ntk_writeb(val, offset) (*(volatile unsigned char *)(offset) = val)
#define ntk_writew(val, offset) (*(volatile unsigned short *)(offset) = val)
#define ntk_writel(val, offset) (*(volatile unsigned long *)(offset) = val)

#define r8(offset)              (*(volatile unsigned char  *)(offset))
#define r16(offset)             (*(volatile unsigned short *)(offset))
#define r32(offset)             (*(volatile unsigned long  *)(offset))

#define w8(offset,val)          (*(volatile unsigned char  *)(offset) = (val))
#define w16(offset,val)         (*(volatile unsigned short *)(offset) = (val))
#define w32(offset,val)         (*(volatile unsigned long  *)(offset) = (val))

#define BIT_SET(offset,val)     (*(volatile unsigned long *)(offset) |= (val))
#define BIT_CLEAR(offset,val)   (*(volatile unsigned long *)(offset) &= ~(val))

//--------------------------------------------------------------------
// NOTE only in 72568
// TODO modify this for porting
#define NFC_568_INT                 (20)
#define NFC_568_INT_BIT             (1<<NFC_568_INT)
#define REG_IRQ_BASE                (0xBD0E0000)
#define REG_IRQ_MASK                (REG_IRQ_BASE+0x10)
#define REG_IRQ_TYPE                (REG_IRQ_BASE+0x18)

//--------------------------------------------------------------------
#define REG_STBC_BOOTSTRAP          (0xBC04023C)
#define REG_SYS_PROGRAM_OPTION      (0xBC0C0014) 

#define REG_SYS_BASE                (0xBC0D0000)
#define REG_SYS_ENABLE              (REG_SYS_BASE+0x00)
#define REG_SYS_RESET               (REG_SYS_BASE+0x08)
#define REG_SYS_NAND_SWITCH         (REG_SYS_BASE+0x1A0)

#define SYS_ENABLE_NFC                          (1 << 13)
#define SYS_ENABLE_GPIO                         (1 << 24)
#define SYS_ENABLE_AGPIO0                       (1 << 25)
#define SYS_ENABLE_AGPIO4                       (1 << 29)

#define SYS_RESET_NFC                           (1 << 13)
#define SYS_RESET_GPIO                          (1 << 24)
#define SYS_RESET_AGPIO0                        (1 << 25)
#define SYS_RESET_AGPIO4                        (1 << 29)


// =============================================================
#define SYS_ENABLE_NFC                  (1 << 13)
#define SYS_ENABLE_GPIO                 (1 << 24)
#define SYS_ENABLE_AGPIO0               (1 << 25)
#define SYS_ENABLE_AGPIO4               (1 << 29)
#define SYS_RESET_NFC                   (1 << 13)
#define SYS_RESET_GPIO                  (1 << 24)
#define SYS_RESET_AGPIO0                (1 << 25)
#define SYS_RESET_AGPIO4                (1 << 29)
// =================================================================================================
//Ken, 2011.03.23, 688 change 0xBB80XXXX-->0xbc04_8XXX
#define REG_NFC_BASE                    (0xBC048000)
//#define REG_NFC_BASE                  (0xBB800000)
// -------------------------------------------------------------------------------------------------
#define REG_NFC_CFG0            (REG_NFC_BASE+0x00)
#define REG_NFC_CFG1            (REG_NFC_BASE+0x04)
#define REG_NFC_CMD             (REG_NFC_BASE+0x08)
#define REG_NFC_TRAN_MODE       (REG_NFC_BASE+0x0c)
#define REG_NFC_COL_ADDR        (REG_NFC_BASE+0x10)
#define REG_NFC_ROW_ADDR        (REG_NFC_BASE+0x14)
#define REG_NFC_SW_RESET        (REG_NFC_BASE+0x18)
#define REG_NFC_RAND_ACC_CMD    (REG_NFC_BASE+0x1c)
#define REG_NFC_INT_ENABLE      (REG_NFC_BASE+0x20)
#define REG_NFC_INT_STAT        (REG_NFC_BASE+0x24)
#define REG_NFC_DATA_PORT       (REG_NFC_BASE+0x28)
#define REG_NFC_RBOOTADDR       (REG_NFC_BASE+0x2c)      //for 8051

#define REG_NFC_DIRCTRL         (REG_NFC_BASE+0x40)
#define REG_NFC_STAT            (REG_NFC_BASE+0x44)
#define REG_NFC_DMA_ADDR        (REG_NFC_BASE+0x50)
#define REG_NFC_DMA_CTRL        (REG_NFC_BASE+0x54)
#define REG_NFC_SYSCTRL         (REG_NFC_BASE+0x5c)
#define REG_NFC_RS_ECC0         (REG_NFC_BASE+0x60)
#define REG_NFC_RS_ECC1         (REG_NFC_BASE+0x64)
#define REG_NFC_RS_ECC2         (REG_NFC_BASE+0x68)
#define REG_NFC_RS_ECC3         (REG_NFC_BASE+0x6c)
#define REG_NFC_AHB_BURST_SIZE  (REG_NFC_BASE+0x78)
#define REG_NFC_51DPORT         (REG_NFC_BASE+0x100)
#define REG_NFC_XTRA_ADDR       (REG_NFC_BASE+0x104)
#define REG_NFC_MBOOTADDR       (REG_NFC_BASE+0x108)    //for mips
#define REG_NFC_SYSCTRL1        (REG_NFC_BASE+0x10c)
#define REG_NFC_Fine_Tune       (REG_NFC_BASE+0x110)
#define NFC_ERR_CNT0            (REG_NFC_BASE+0x114)
#define NFC_ERR_CNT1            (REG_NFC_BASE+0x118)
#define REG_NFC_SYSCTRL2        (REG_NFC_BASE+0x11C)    // 558 new register

#define REG_NFC_XTRA_DATA0      (REG_NFC_BASE+0x30)
#define REG_NFC_XTRA_DATA1      (REG_NFC_BASE+0x34)
#define REG_NFC_XTRA_DATA2      (REG_NFC_BASE+0x38)
#define REG_NFC_XTRA_DATA3      (REG_NFC_BASE+0x3c)

#define REG_NFC_XTRA_DATA4      (REG_NFC_BASE+0x7c)
#define REG_NFC_XTRA_DATA5      (REG_NFC_BASE+0x80)
#define REG_NFC_XTRA_DATA6      (REG_NFC_BASE+0x84)
#define REG_NFC_XTRA_DATA7      (REG_NFC_BASE+0x88)
#define REG_NFC_XTRA_DATA8      (REG_NFC_BASE+0x8c)
#define REG_NFC_XTRA_DATA9      (REG_NFC_BASE+0x90)
#define REG_NFC_XTRA_DATA10     (REG_NFC_BASE+0x94)
#define REG_NFC_XTRA_DATA11     (REG_NFC_BASE+0x98)
#define REG_NFC_XTRA_DATA12     (REG_NFC_BASE+0x9c)
#define REG_NFC_XTRA_DATA13     (REG_NFC_BASE+0xa0)
#define REG_NFC_XTRA_DATA14     (REG_NFC_BASE+0xa4)
#define REG_NFC_XTRA_DATA15     (REG_NFC_BASE+0xa8)
#define REG_NFC_XTRA_DATA16     (REG_NFC_BASE+0xac)
#define REG_NFC_XTRA_DATA17     (REG_NFC_BASE+0xb0)
#define REG_NFC_XTRA_DATA18     (REG_NFC_BASE+0xb4)
#define REG_NFC_XTRA_DATA19     (REG_NFC_BASE+0xb8)
#define REG_NFC_XTRA_DATA20     (REG_NFC_BASE+0xbc)
#define REG_NFC_XTRA_DATA21     (REG_NFC_BASE+0xc0)
#define REG_NFC_XTRA_DATA22     (REG_NFC_BASE+0xc4)
#define REG_NFC_XTRA_DATA23     (REG_NFC_BASE+0xc8)
#define REG_NFC_XTRA_DATA24     (REG_NFC_BASE+0xcc)
#define REG_NFC_XTRA_DATA25     (REG_NFC_BASE+0xd0)
#define REG_NFC_XTRA_DATA26     (REG_NFC_BASE+0xd4)
#define REG_NFC_XTRA_DATA27     (REG_NFC_BASE+0xd8)


//Original
//#define REG_NFC_BCH_ERR_SET1(n)	(*(volatile unsigned long *)(REG_NFC_BASE+0x60+4*(3&(n))))
//#define REG_NFC_BCH_ERR_SET2(n)	(*(volatile unsigned long *)(REG_NFC_BASE+0xdc+4*(7&(n))))
//Ben, by Ken, June/29/2011.
#define REG_NFC_BCH_ERR_SET1(n)	(REG_NFC_BASE+0x60+4*(3&(n)))
#define REG_NFC_BCH_ERR_SET2(n)	(REG_NFC_BASE+0xdc+4*(7&(n)))


// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG0
#define NFC_CFG0_TWH(n)                     ((0xf & (n)) << 28)
#define NFC_CFG0_TWL(n)                     ((0xf & (n)) << 24)
#define NFC_CFG0_TRH(n)                     ((0xf & (n)) << 20)
#define NFC_CFG0_TRL(n)                     ((0xf & (n)) << 16)
#define NFC_CFG0_TWW(n)                     ((0xf & (n)) << 12)
#define NFC_CFG0_TAR(n)                     ((0xf & (n)) << 8)
#define NFC_CFG0_TCLS_TCS_TALS_TALH(n)      ((0xf & (n)) << 4)
#define NFC_CFG0_DATAWIDTH_16               (1 << 2)
#define NFC_CFG0_ROW_ADDR_3CYCLES           (1 << 1)
#define NFC_CFG0_COL_ADDR_2CYCLES           (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG1
#define NFC_CFG1_READY_TO_BUSY_TIMEOUT(n)   ((0xf & (n)) << 24)
#define NFC_CFG1_ECC_ENCODE_RESET           (1 << 23)
#define NFC_CFG1_ECC_DECODE_RESET           (1 << 22)
#define NFC_CFG1_BCH_ECC_ENABLE             (1 << 21)
#define NFC_CFG1_LITTLE_ENDIAN_XTRA         (1 << 19)
#define NFC_CFG1_LITTLE_ENDIAN              (1 << 18)
#define NFC_CFG1_BCH_ENABLE_PL              (1 << 17)
#define NFC_CFG1_RS_ECC_ENABLE              (1 << 16)
#define NFC_CFG1_BUSY_TO_READY_TIMEOUT(n)   (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_CMD
#define NFC_CMD_CE_IDX(n)                   ((7 & (n)) << 28)
#define NFC_CMD_WP_NEG                      (1 << 27)
#define NFC_CMD_FREERUN                     (1 << 24)
#define NFC_CMD_DATA_PRESENT                (1 << 23)
#define NFC_CMD_ADDR_CYCLE(n)               ((7 & (n)) << 20)
#define NFC_CMD_ADDR_CYCLE_NONE             NFC_CMD_ADDR_CYCLE(0)
#define NFC_CMD_ADDR_CYCLE_DUMMY            NFC_CMD_ADDR_CYCLE(1)
#define NFC_CMD_ADDR_CYCLE_COL              NFC_CMD_ADDR_CYCLE(2)
#define NFC_CMD_ADDR_CYCLE_ROW              NFC_CMD_ADDR_CYCLE(4)
#define NFC_CMD_ADDR_CYCLE_COL_ROW          NFC_CMD_ADDR_CYCLE(6)
#define NFC_CMD_ADDR_CYCLE_COL_OTHER        NFC_CMD_ADDR_CYCLE(7)
#define NFC_CMD_END_WAIT_BUSY_TO_RDY        (1 << 19)
#define NFC_CMD_CYCLE(n)                    ((3 & (n)) << 16)
#define NFC_CMD_CYCLE_NONE                  NFC_CMD_CYCLE(0)
#define NFC_CMD_CYCLE_ONE                   NFC_CMD_CYCLE(1)
#define NFC_CMD_CYCLE_TWO                   NFC_CMD_CYCLE(2)
#define NFC_CMD_CODE1(n)                    ((0xff & (n)) << 8)
#define NFC_CMD_CODE0(n)                    (0xff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_TRAN_MODE
#define NFC_TRAN_MODE_KEEP_CE                   (1 << 31)
#define NFC_TRAN_MODE_CE_IDX(n)                 ((7 & (n)) << 28)
#if 0
#define NFC_CMD_WP_DISABLE                      (1 << 27)
#define NFC_TRAN_MODE_ECC_CHK                   (1 << 19)
#define NFC_TRAN_MODE_ECC_CMB                   (1 << 18)
#else
#define NFC_TRAN_MODE_ECC_CHK                   (1 << 27)
#define NFC_TRAN_MODE_ECC_CMB                   (1 << 26)
#endif
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(n)     ((3 & (n)) << 24)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_NONE   NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(0)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE    NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(2)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO    NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(3)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT(n)        ((3 & (n)) << 22)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_NONE      NFC_TRAN_MODE_XTRA_DATA_COUNT(0)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_8_LOW     NFC_TRAN_MODE_XTRA_DATA_COUNT(1)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_8_HIGH    NFC_TRAN_MODE_XTRA_DATA_COUNT(2)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_16        NFC_TRAN_MODE_XTRA_DATA_COUNT(3)
#define NFC_TRAN_MODE_END_WAIT_READY            (1 << 21)
#define NFC_TRAN_MODE_BLK_SIZE(n)               ((0x1fff & ((n) - 1)) << 8)
#define NFC_TRAN_MODE_ECC_ENABLE                (1 << 7)
#define NFC_TRAN_MODE_ECC_RESET                 (1 << 6)
#define NFC_TRAN_MODE_DATA_SEL(n)               ((3 & (n)) << 4)
#define NFC_TRAN_MODE_DATA_SEL_DATA_PORT        NFC_TRAN_MODE_DATA_SEL(0)
#define NFC_TRAN_MODE_DATA_SEL_DMA              NFC_TRAN_MODE_DATA_SEL(1)
#define NFC_TRAN_MODE_DATA_SEL_XTRA             NFC_TRAN_MODE_DATA_SEL(2)
#define NFC_TRAN_MODE_DATA_SEL_XTRA_ECC_CMB     NFC_TRAN_MODE_DATA_SEL(3)
#define NFC_TRAN_MODE_START_WAIT_RDY            (1 << 2)
#define NFC_TRAN_MODE_WRITE                     (1 << 1)

// -------------------------------------------------------------------------------------------------
// REG_NFC_STAT
#define NFC_STAT_RS_ECC_ERR(n)                  ((3 & (n)) << 20)
#define NFC_STAT_RS_ECC_ERR_MASK                NFC_STAT_RS_ECC_ERR(-1)
#define NFC_STAT_RS_ECC_ERR_NONE                NFC_STAT_RS_ECC_ERR(0)
#define NFC_STAT_RS_ECC_ERR_CORRECTABLE         NFC_STAT_RS_ECC_ERR(1)
#define NFC_STAT_RS_ECC_ERR_NONCORRECTABLE      NFC_STAT_RS_ECC_ERR(2)

// -------------------------------------------------------------------------------------------------
// REG_NFC_DMA_CTRL
#define NFC_DMA_CTRL_WRITE	(0<<16)
#define NFC_DMA_CTRL_READ	(1<<16)
#define NFC_DMA_CTRL_TRAN_BYTE_COUNT(n)         (0xffff & (n-1))

// -------------------------------------------------------------------------------------------------
// REG_NFC_RAND_ACC_CMD
#define NFC_RAND_ACC_CMD_CODE1(n)               ((0xff & (n)) << 24)
#define NFC_RAND_ACC_CMD_CODE0(n)               ((0xff & (n)) << 16)
#define NFC_RAND_ACC_CMD_COL_ADDR(n)            (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_INT_ENABLE, REG_NFC_INT_STAT
#define NFC_INT_ENABLE                          (1 << 31)				//only in REG_NFC_INT_ENABLE
#define NFC_INT_FREE_RUN_COMPLETE               (1 << 21)
#define NFC_INT_FR_UNCORECTABLE                 (1 << 20)
#define NFC_INT_ERR                             ((0xf7f << 8))
#define NFC_INT_ERR_RW   	                    ((0xe51 << 8))
#define NFC_INT_MMEMP_COMPLETE                  (1 << 7)
#define NFC_INT_ECC_DEC_COMPLETE                (1 << 6)
#define NFC_INT_RS_ECC_DEC_COMPLETE		        (1 << 6)
#define NFC_INT_PIO_READY                       (1 << 3)
#define NFC_INT_MEM_TRAN_COMPLETE               (1 << 2)
#define NFC_INT_DATA_TRAN_COMPLETE              (1 << 1)
#define NFC_INT_CMD_COMPLETE                    (1 << 0)

// REG_NFC_SYSCTRL
#define NFC_SYS_CTRL_FREE_RUN_EN                (1<<3)
#define NFC_SYS_CTRL_SUBPAGE_SIZE(n)            (((1 & (n)) << 27))
#define NFC_SYS_CTRL_SUBPAGE_512                NFC_SYS_CTRL_SUBPAGE_SIZE(0)
#define NFC_SYS_CTRL_SUBPAGE_1024               NFC_SYS_CTRL_SUBPAGE_SIZE(1)
#define NFC_SYS_CTRL_PAGE_SIZE(n)               (((3 & (n)) << 24))
#define NFC_SYS_CTRL_PAGE_2048                  NFC_SYS_CTRL_PAGE_SIZE(0)
#define NFC_SYS_CTRL_PAGE_4096                  NFC_SYS_CTRL_PAGE_SIZE(1)
#define NFC_SYS_CTRL_PAGE_8192                  NFC_SYS_CTRL_PAGE_SIZE(2)
#define NFC_SYS_CTRL_EXTRA_SIZE(n)              ((0x3F & (n)) << 8)
#define NFC_SYS_CTRL_ECC_SEL(n)                 ((1 & (n)) << 26)
#define NFC_SYS_CTRL_ECC_RS                     NFC_SYS_CTRL_ECC_SEL(0)
#define NFC_SYS_CTRL_ECC_BCH                    NFC_SYS_CTRL_ECC_SEL(1)
#define NFC_SYS_CTRL_BLK_SIZE(n)                (((3 & (n))) << 28)
#define NFC_SYS_CTRL_BLK_128K                   NFC_SYS_CTRL_BLK_SIZE(0)
#define NFC_SYS_CTRL_BLK_256K                   NFC_SYS_CTRL_BLK_SIZE(1)
#define NFC_SYS_CTRL_BLK_512K                   NFC_SYS_CTRL_BLK_SIZE(2)
#define NFC_SYS_CTRL_BLK_1M                     NFC_SYS_CTRL_BLK_SIZE(3)
#define NFC_SYS_CTRL_CONTROLLER_SEL(n)          ((1 & (n)) << 30)

// REG_NFC_SYSCTRL1      
#define NFC_SYS_CTRL1_FREE_RUN_EN               (1<<2)     
#define NFC_SYS_CTRL1_REMOVE_EXT_DATA           (1<<6)       
#define NFC_SYS_CTRL1_ECC_DEC_MODE              (1<<10)

// REG_NFC_SYSCTRL2      
#define NFC_SYS_CTRL2_BLANK_CHECK_EN            (1<<6)                
#define NFC_SYS_CTRL2_AUTO_CORRECT_DISABLE      (1<<7)
//================================================================================
//================================================================================


#define FSR_MAX_PHY_SCTS_683 16

#endif /* #define _FSR_LLD_NTK_568_ */

