/**
 * @file NT568/nand_flash.h
 *
 * @brief nand flash controller register map
 *
 * @author Novatek SP/KSW
 *
 */


/*
 * static inlines
 */

#ifndef __NAND_H__
#define __NAND_H__
#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus

//#include 	"Type.h"

#define REG_SYS_PROGRAM_OPTION                  (*(volatile unsigned long *)(0xBC0C0014))

// =================================================================================================
#define REG_GPIO_BASE                           (0xBD0D0000)
// -------------------------------------------------------------------------------------------------
#define REG_BONDING_OPTION                      (*(volatile unsigned long *)(REG_GPIO_BASE+0x04))	
#define REG_MEM_BUS_SEL                         (*(volatile unsigned long *)(REG_GPIO_BASE+0x20))
#define REG_AGPIO0_SEL                          (*(volatile unsigned long *)(REG_GPIO_BASE+0x30))
#define REG_SYS_NAND_AGPIO4_ENABLE              (*(volatile unsigned long *)(REG_GPIO_BASE+0x70))


// =================================================================================================
#define REG_SYS_BASE                            (0xBC0D0000)
// -------------------------------------------------------------------------------------------------
#define REG_SYS_ENABLE                          (*(volatile unsigned long *)(REG_SYS_BASE+0x00))
#define REG_SYS_RESET                           (*(volatile unsigned long *)(REG_SYS_BASE+0x08))
#define REG_SYS_NAND_SWITCH                     (*(volatile unsigned long *)(REG_SYS_BASE+0x1A0))

#define SYS_ENABLE_NFC                          (1 << 13)
#define SYS_ENABLE_GPIO                         (1 << 24)
#define SYS_ENABLE_AGPIO0                       (1 << 25)
#define SYS_ENABLE_AGPIO4                       (1 << 29)

#define SYS_RESET_NFC                           (1 << 13)
#define SYS_RESET_GPIO                          (1 << 24)
#define SYS_RESET_AGPIO0                        (1 << 25)
#define SYS_RESET_AGPIO4                        (1 << 29)


// =================================================================================================

#define REG_NFC_BASE                            (0xBC048000)

// -------------------------------------------------------------------------------------------------
#define REG_NFC_CFG0                            (*(volatile unsigned long *)(REG_NFC_BASE+0x00))
#define REG_NFC_CFG1                            (*(volatile unsigned long *)(REG_NFC_BASE+0x04))
#define REG_NFC_CMD                             (*(volatile unsigned long *)(REG_NFC_BASE+0x08))
#define REG_NFC_TRAN_MODE                       (*(volatile unsigned long *)(REG_NFC_BASE+0x0c))
#define REG_NFC_CFG0                            (*(volatile unsigned long *)(REG_NFC_BASE+0x00))
#define REG_NFC_CFG1                            (*(volatile unsigned long *)(REG_NFC_BASE+0x04))
#define REG_NFC_CMD                             (*(volatile unsigned long *)(REG_NFC_BASE+0x08))
#define REG_NFC_TRAN_MODE                       (*(volatile unsigned long *)(REG_NFC_BASE+0x0c))
#define REG_NFC_COL_ADDR                        (*(volatile unsigned long *)(REG_NFC_BASE+0x10))
#define REG_NFC_ROW_ADDR                        (*(volatile unsigned long *)(REG_NFC_BASE+0x14))
#define REG_NFC_SW_RESET                        (*(volatile unsigned long *)(REG_NFC_BASE+0x18))
#define REG_NFC_RAND_ACC_CMD                    (*(volatile unsigned long *)(REG_NFC_BASE+0x1c))
#define REG_NFC_INT_ENABLE                      (*(volatile unsigned long *)(REG_NFC_BASE+0x20))
#define REG_NFC_INT_STAT                        (*(volatile unsigned long *)(REG_NFC_BASE+0x24))
#define REG_NFC_DATA_PORT                       (*(volatile unsigned long *)(REG_NFC_BASE+0x28))

#define REG_NFC_DIRCTRL                         (*(volatile unsigned long *)(REG_NFC_BASE+0x40))
#define REG_NFC_STAT                            (*(volatile unsigned long *)(REG_NFC_BASE+0x44))
#define REG_NFC_DMA_ADDR                        (*(volatile unsigned long *)(REG_NFC_BASE+0x50))
#define REG_NFC_DMA_CTRL                        (*(volatile unsigned long *)(REG_NFC_BASE+0x54))
#define REG_NFC_SYSCTRL                         (*(volatile unsigned long *)(REG_NFC_BASE+0x5c))

#define REG_NFC_RS_ECC(n)                       (*(volatile unsigned long *)(REG_NFC_BASE+0x60+4*(3&(n))))
#define REG_NFC_RS_ECC0                         REG_NFC_RS_ECC(0)
#define REG_NFC_RS_ECC1                         REG_NFC_RS_ECC(1)
#define REG_NFC_RS_ECC2                         REG_NFC_RS_ECC(2)
#define REG_NFC_RS_ECC3                         REG_NFC_RS_ECC(3)

#define REG_NFC_BCH_ERR_SET1(n)                 (*(volatile unsigned long *)(REG_NFC_BASE+0x60+4*(3&(n))))
#define REG_NFC_BCH_ERR_SET2(n)                 (*(volatile unsigned long *)(REG_NFC_BASE+0xdc+4*(7&(n))))

#define REG_NFC_BCH_ERR_1_2                     (*(volatile unsigned long *)(REG_NFC_BASE+0x60))
#define REG_NFC_BCH_ERR_3_4                     (*(volatile unsigned long *)(REG_NFC_BASE+0x64))
#define REG_NFC_BCH_ERR_5_6                     (*(volatile unsigned long *)(REG_NFC_BASE+0x68))
#define REG_NFC_BCH_ERR_7_8                     (*(volatile unsigned long *)(REG_NFC_BASE+0x6c))
#define REG_NFC_BCH_ERR_9_10                    (*(volatile unsigned long *)(REG_NFC_BASE+0xdc))
#define REG_NFC_BCH_ERR_11_12                   (*(volatile unsigned long *)(REG_NFC_BASE+0xe0))
#define REG_NFC_BCH_ERR_13_14                   (*(volatile unsigned long *)(REG_NFC_BASE+0xe4))
#define REG_NFC_BCH_ERR_15_16                   (*(volatile unsigned long *)(REG_NFC_BASE+0xe8))
#define REG_NFC_BCH_ERR_17_18                   (*(volatile unsigned long *)(REG_NFC_BASE+0xec))
#define REG_NFC_BCH_ERR_19_20                   (*(volatile unsigned long *)(REG_NFC_BASE+0xf0))
#define REG_NFC_BCH_ERR_21_22                   (*(volatile unsigned long *)(REG_NFC_BASE+0xf4))
#define REG_NFC_BCH_ERR_23_24                   (*(volatile unsigned long *)(REG_NFC_BASE+0xf8))

#define REG_NFC_XTRA_DATA0                      (*(volatile unsigned long *)(REG_NFC_BASE+0x30))
#define REG_NFC_XTRA_DATA1                      (*(volatile unsigned long *)(REG_NFC_BASE+0x34))
#define REG_NFC_XTRA_DATA2                      (*(volatile unsigned long *)(REG_NFC_BASE+0x38))
#define REG_NFC_XTRA_DATA3                      (*(volatile unsigned long *)(REG_NFC_BASE+0x3c))
#define REG_NFC_XTRA_DATA4                      (*(volatile unsigned long *)(REG_NFC_BASE+0x7c))
#define REG_NFC_XTRA_DATA5                      (*(volatile unsigned long *)(REG_NFC_BASE+0x80))
#define REG_NFC_XTRA_DATA6                      (*(volatile unsigned long *)(REG_NFC_BASE+0x84))
#define REG_NFC_XTRA_DATA7                      (*(volatile unsigned long *)(REG_NFC_BASE+0x88))
#define REG_NFC_XTRA_DATA8                      (*(volatile unsigned long *)(REG_NFC_BASE+0x8c))
#define REG_NFC_XTRA_DATA9                      (*(volatile unsigned long *)(REG_NFC_BASE+0x90))
#define REG_NFC_XTRA_DATA10                     (*(volatile unsigned long *)(REG_NFC_BASE+0x94))
#define REG_NFC_XTRA_DATA11                     (*(volatile unsigned long *)(REG_NFC_BASE+0x98))
#define REG_NFC_XTRA_DATA12                     (*(volatile unsigned long *)(REG_NFC_BASE+0x9c))
#define REG_NFC_XTRA_DATA13                     (*(volatile unsigned long *)(REG_NFC_BASE+0xa0))
#define REG_NFC_XTRA_DATA14                     (*(volatile unsigned long *)(REG_NFC_BASE+0xa4))
#define REG_NFC_XTRA_DATA15                     (*(volatile unsigned long *)(REG_NFC_BASE+0xa8))
#define REG_NFC_XTRA_DATA16                     (*(volatile unsigned long *)(REG_NFC_BASE+0xac))
#define REG_NFC_XTRA_DATA17                     (*(volatile unsigned long *)(REG_NFC_BASE+0xb0))
#define REG_NFC_XTRA_DATA18                     (*(volatile unsigned long *)(REG_NFC_BASE+0xb4))
#define REG_NFC_XTRA_DATA19                     (*(volatile unsigned long *)(REG_NFC_BASE+0xb8))
#define REG_NFC_XTRA_DATA20                     (*(volatile unsigned long *)(REG_NFC_BASE+0xbc))
#define REG_NFC_XTRA_DATA21                     (*(volatile unsigned long *)(REG_NFC_BASE+0xc0))
#define REG_NFC_XTRA_DATA22                     (*(volatile unsigned long *)(REG_NFC_BASE+0xc4))
#define REG_NFC_XTRA_DATA23                     (*(volatile unsigned long *)(REG_NFC_BASE+0xc8))
#define REG_NFC_XTRA_DATA24                     (*(volatile unsigned long *)(REG_NFC_BASE+0xcc))
#define REG_NFC_XTRA_DATA25                     (*(volatile unsigned long *)(REG_NFC_BASE+0xd0))
#define REG_NFC_XTRA_DATA26                     (*(volatile unsigned long *)(REG_NFC_BASE+0xd4))
#define REG_NFC_XTRA_DATA27                     (*(volatile unsigned long *)(REG_NFC_BASE+0xd8))

#define REG_NFC_AHB_BURST_SIZE                  (*(volatile unsigned long *)(REG_NFC_BASE+0x78))

#define REG_NFC_XTRA_ADDR                       (*(volatile unsigned long *)(REG_NFC_BASE+0x104))
#define REG_NFC_SYSCTRL1                        (*(volatile unsigned long *)(REG_NFC_BASE+0x10C))
#define REG_NFC_Fine_Tune                       (*(volatile unsigned long *)(REG_NFC_BASE+0x110))
#define NFC_ERR_CNT0                            (*(volatile unsigned long *)(REG_NFC_BASE+0x114))
#define NFC_ERR_CNT1                            (*(volatile unsigned long *)(REG_NFC_BASE+0x118))
#define REG_NFC_SYSCTRL2                        (*(volatile unsigned long *)(REG_NFC_BASE+0x11C))


#define REG_NFC_RS_ECC(n)                       (*(volatile unsigned long *)(REG_NFC_BASE+0x60+4*(3&(n))))
#define REG_NFC_RS_ECC0                         REG_NFC_RS_ECC(0)
#define REG_NFC_RS_ECC1                         REG_NFC_RS_ECC(1)
#define REG_NFC_RS_ECC2                         REG_NFC_RS_ECC(2)
#define REG_NFC_RS_ECC3                         REG_NFC_RS_ECC(3)

//Ben+, June/10/2010.
#define REG_NFC_BCH_ECC(n)                      (*(volatile unsigned long *)(REG_NFC_BASE+0x60+4*(3&(n))))
#define REG_NFC_BCH_ECC0                        REG_NFC_BCH_ECC(0)
#define REG_NFC_BCH_ECC1                        REG_NFC_BCH_ECC(1)
#define REG_NFC_BCH_ECC2                        REG_NFC_BCH_ECC(2)
#define REG_NFC_BCH_ECC3                        REG_NFC_BCH_ECC(3)

//Ben+, June/10/2010.
#define REG_NFC_HMECC0_STS                      (*(volatile unsigned long *)(REG_NFC_BASE+0x70))

//Ben+, June/10/2010.
#define REG_NFC_HMECC1_STS                      (*(volatile unsigned long *)(REG_NFC_BASE+0x74))

//Ben+, June/10/2010.
#define REG_XD_CFG                              (*(volatile unsigned long *)(REG_NFC_BASE+0x78))

//Ben+, June/10/2010.
#define REG_NFC_XTRA_DATA4                      (*(volatile unsigned long *)(REG_NFC_BASE+0x7c)) 
#define REG_NFC_XTRA_DATA5                      (*(volatile unsigned long *)(REG_NFC_BASE+0x80)) 
#define REG_NFC_XTRA_DATA6                      (*(volatile unsigned long *)(REG_NFC_BASE+0x84)) 
#define REG_NFC_XTRA_DATA7                      (*(volatile unsigned long *)(REG_NFC_BASE+0x88)) 
#define REG_NFC_XTRA_DATA8                      (*(volatile unsigned long *)(REG_NFC_BASE+0x8c)) 
#define REG_NFC_XTRA_DATA9                      (*(volatile unsigned long *)(REG_NFC_BASE+0x90)) 
#define REG_NFC_XTRA_DATA10                     (*(volatile unsigned long *)(REG_NFC_BASE+0x94)) 
#define REG_NFC_XTRA_DATA11                     (*(volatile unsigned long *)(REG_NFC_BASE+0x98)) 
#define REG_NFC_XTRA_DATA12                     (*(volatile unsigned long *)(REG_NFC_BASE+0x9c)) 
#define REG_NFC_XTRA_DATA13                     (*(volatile unsigned long *)(REG_NFC_BASE+0xa0)) 
#define REG_NFC_XTRA_DATA14                     (*(volatile unsigned long *)(REG_NFC_BASE+0xa4)) 
#define REG_NFC_XTRA_DATA15                     (*(volatile unsigned long *)(REG_NFC_BASE+0xa8)) 
#define REG_NFC_XTRA_DATA16                     (*(volatile unsigned long *)(REG_NFC_BASE+0xac)) 
#define REG_NFC_XTRA_DATA17                     (*(volatile unsigned long *)(REG_NFC_BASE+0xb0)) 
#define REG_NFC_XTRA_DATA18                     (*(volatile unsigned long *)(REG_NFC_BASE+0xb4)) 
#define REG_NFC_XTRA_DATA19                     (*(volatile unsigned long *)(REG_NFC_BASE+0xb8)) 
#define REG_NFC_XTRA_DATA20                     (*(volatile unsigned long *)(REG_NFC_BASE+0xbc)) 
#define REG_NFC_XTRA_DATA21                     (*(volatile unsigned long *)(REG_NFC_BASE+0xc0)) 
#define REG_NFC_XTRA_DATA22                     (*(volatile unsigned long *)(REG_NFC_BASE+0xc4)) 
#define REG_NFC_XTRA_DATA23                     (*(volatile unsigned long *)(REG_NFC_BASE+0xc8)) 
#define REG_NFC_XTRA_DATA24                     (*(volatile unsigned long *)(REG_NFC_BASE+0xcc)) 
#define REG_NFC_XTRA_DATA25                     (*(volatile unsigned long *)(REG_NFC_BASE+0xd0)) 
#define REG_NFC_XTRA_DATA26                     (*(volatile unsigned long *)(REG_NFC_BASE+0xd4)) 
#define REG_NFC_XTRA_DATA27                     (*(volatile unsigned long *)(REG_NFC_BASE+0xd8))
#define REG_NFC_BCH_ECC4                        (*(volatile unsigned long *)(REG_NFC_BASE+0xdc))
#define REG_NFC_BCH_ECC5                        (*(volatile unsigned long *)(REG_NFC_BASE+0xe0))
#define REG_NFC_BCH_ECC6                        (*(volatile unsigned long *)(REG_NFC_BASE+0xe4))
#define REG_NFC_BCH_ECC7                        (*(volatile unsigned long *)(REG_NFC_BASE+0xe8))
#define REG_NFC_BCH_ECC8                        (*(volatile unsigned long *)(REG_NFC_BASE+0xec))
#define REG_NFC_BCH_ECC9                        (*(volatile unsigned long *)(REG_NFC_BASE+0xf0))
#define REG_NFC_BCH_ECC10                       (*(volatile unsigned long *)(REG_NFC_BASE+0xf4))
#define REG_NFC_BCH_ECC11                       (*(volatile unsigned long *)(REG_NFC_BASE+0xf8))

//Ben, this one is removed, June/09/2010.
//#define REG_NFC_BCH_DEC_XTRA_DATA               (*(volatile unsigned long *)(REG_NFC_BASE+0xfc))

#define REG_NFC_51_DATA_PORT                    (*(volatile unsigned long *)(REG_NFC_BASE+0xfc))
#define REG_NFC_51_XTRA_ADDR                    (*(volatile unsigned long *)(REG_NFC_BASE+0x100))
#define REG_NFC_MIPS_BOOT_ADDR                  (*(volatile unsigned long *)(REG_NFC_BASE+0x104))


// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG0
#define NFC_CFG0_TWH(n)                         ((0xf & (n)) << 28)
#define NFC_CFG0_TWL(n)                         ((0xf & (n)) << 24)
#define NFC_CFG0_TRH(n)                         ((0xf & (n)) << 20)
#define NFC_CFG0_TRL(n)                         ((0xf & (n)) << 16)
#define NFC_CFG0_TWW(n)                         ((0xf & (n)) << 12)
#define NFC_CFG0_TAR(n)                         ((0xf & (n)) << 8)
#define NFC_CFG0_TCLS_TCS_TALS_TALH(n)          ((0xf & (n)) << 4)
#define NFC_CFG0_ROW_ADDR_3CYCLES               (1 << 1)
#define NFC_CFG0_COL_ADDR_2CYCLES               (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG1
#define NFC_CFG1_READY_TO_BUSY_TIMEOUT(n)       ((0xf & (n)) << 24)
#define NFC_CFG1_LITTLE_ENDIAN_XTRA             (1 << 19)
#define NFC_CFG1_LITTLE_ENDIAN                  (1 << 18)
#define NFC_CFG1_RS_ECC_ENABLE                  (1 << 16)
#define NFC_CFG1_BUSY_TO_READY_TIMEOUT(n)       (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_CMD
#define NFC_CMD_CE_IDX(n)                       ((7 & (n)) << 28)
#define NFC_CMD_WP_NEG							(1 << 27)
#define NFC_CMD_DATA_PRESENT                    (1 << 23)
#define NFC_CMD_ADDR_CYCLE(n)                   ((7 & (n)) << 20)
#define NFC_CMD_ADDR_CYCLE_NONE                 NFC_CMD_ADDR_CYCLE(0)
#define NFC_CMD_ADDR_CYCLE_DUMMY                NFC_CMD_ADDR_CYCLE(1)
#define NFC_CMD_ADDR_CYCLE_COL                  NFC_CMD_ADDR_CYCLE(2)
#define NFC_CMD_ADDR_CYCLE_ROW                  NFC_CMD_ADDR_CYCLE(4)
#define NFC_CMD_ADDR_CYCLE_COL_ROW              NFC_CMD_ADDR_CYCLE(6)
#define NFC_CMD_END_WAIT_BUSY_TO_RDY            (1 << 19)
#define NFC_CMD_CYCLE(n)                        ((3 & (n)) << 16)
#define NFC_CMD_CYCLE_NONE                      NFC_CMD_CYCLE(0)
#define NFC_CMD_CYCLE_ONE                       NFC_CMD_CYCLE(1)
#define NFC_CMD_CYCLE_TWO                       NFC_CMD_CYCLE(2)
#define NFC_CMD_CODE1(n)                        ((0xff & (n)) << 8)
#define NFC_CMD_CODE0(n)                        (0xff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_INT_ENABLE, REG_NFC_INT_STAT
#define NFC_INT_ERR                             ((0xf7f << 8))

//Ben+, Sep/10/2010.

#define NFC_INT_RS_ECC_DEC_COMPLETE             (1 << 6)
#define NFC_INT_PIO_READY						(1 << 3)
#define NFC_INT_MEM_TRAN_COMPLETE               (1 << 2)
#define NFC_INT_DATA_TRAN_COMPLETE              (1 << 1)
#define NFC_INT_CMD_COMPLETE                    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_DMA_CTRL
#define NFC_DMA_CTRL_READ                       (1 << 16)

// -------------------------------------------------------------------------------------------------
// REG_NFC_TRAN_MODE
#define NFC_TRAN_MODE_KEEP_CE                   (1 << 31)
#define NFC_TRAN_MODE_CE_IDX(n)                 ((7 & (n)) << 28)
#define NFC_CMD_WP_DISABLE                      (1 << 27)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(n)     ((3 & (n)) << 24)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_NONE   NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(0)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE    NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(2)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO    NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(3)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT(n)        ((3 & (n)) << 22)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_NONE      NFC_TRAN_MODE_XTRA_DATA_COUNT(0)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_8_LOW     NFC_TRAN_MODE_XTRA_DATA_COUNT(1)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_8_HIGH    NFC_TRAN_MODE_XTRA_DATA_COUNT(2)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_16        NFC_TRAN_MODE_XTRA_DATA_COUNT(3)
#define NFC_TRAN_MODE_END_WAIT_READY			(1 << 21)
#define NFC_TRAN_MODE_ECC_CHK                   (1 << 19)
#define NFC_TRAN_MODE_ECC_CMB                   (1 << 18)
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
#define NFC_DMA_CTRL_TRAN_BYTE_COUNT(n)         (0xffff & ((n) - 1))

// -------------------------------------------------------------------------------------------------
// REG_NFC_RAND_ACC_CMD
#define NFC_RAND_ACC_CMD_CODE1(n)               ((0xff & (n)) << 24)
#define NFC_RAND_ACC_CMD_CODE0(n)               ((0xff & (n)) << 16)
#define NFC_RAND_ACC_CMD_COL_ADDR(n)            (0xffff & (n))

// -------------------------------------------------------------------------------------------------
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


#define BOOL  UINT8
#define BYTE  UINT8
#define WORD  UINT16
#define DWORD UINT32
#define u32   UINT32
//typedef unsigned long ulong;

#define ON		1
#define OFF		0
#define E_SYS	-1
#define E_OK	0

#define TRUE    1
#define FALSE   0

//UINT16 OneNAND_Asynchronous_Read( UINT16 read_addr );
void OneNAND_Asynchronous_Write( UINT16 read_addr, UINT16 data );
void delay_us(UINT32 count);
int PNFC_Init( void );
void OneNAND_Init( void );
//int Nvt_onenand_sync_read_bufferram(UINT32 u32Addr, UINT8 *pu8Data, UINT32 u32Size);   // Macoto mark
int OneNAND_Synchronous_Read( UINT16 read_addr , UINT16 burst_length, UINT16 burst_read_latency, UINT8 sync_read_clk_setting, UINT32 buffer );
BOOL OneNAND_Free_Run_Read_One_Page( WORD Target_Block_Addr, WORD Target_Page_Addr, WORD Target_Sector_Addr, DWORD buffer );
BOOL OneNAND_Free_Run_Read_By_Count( WORD Target_Block_Addr, WORD Target_Page_Addr, WORD Target_Sector_Addr, DWORD buffer, WORD count );
void NFC_Reset(void) ;


#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __NAND_H__


