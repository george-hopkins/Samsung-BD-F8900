////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2012 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __eMMC_X12_LINUX__
#define __eMMC_X12_LINUX__

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/host.h>
#include <linux/scatterlist.h>
#include <mstar/mstar_chip.h>
#include <mach/io.h>
#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#endif
#include "chip_int.h"

#ifndef U32
#define U32  unsigned int
#endif
#ifndef U16
#define U16  unsigned short
#endif
#ifndef U8
#define U8   unsigned char
#endif
#ifndef S32
#define S32  signed long
#endif
#ifndef S16
#define S16  signed short
#endif
#ifndef S8
#define S8   signed char
#endif

//=====================================================
// HW registers
//=====================================================
#define REG_OFFSET_SHIFT_BITS               2U

#define REG_FCIE_U16(Reg_Addr)              ((*(volatile U16*)(Reg_Addr)))
#define GET_REG_ADDR(x, y)                  (((U32)x)+(((U32)y) << REG_OFFSET_SHIFT_BITS))

#define REG_FCIE(reg_addr)                  REG_FCIE_U16((U32)(reg_addr))
#define REG_FCIE_W(reg_addr, val)           REG_FCIE((U32)reg_addr) = (U16)(val)
#define REG_FCIE_R(reg_addr, val)           val = REG_FCIE((U32)reg_addr)
#define REG_FCIE_SETBIT(reg_addr, val)      REG_FCIE((U32)(reg_addr)) |= ((U16)(val))
//#define REG_FCIE_CLRBIT(reg_addr, val)      REG_FCIE((U32)(reg_addr)) &= ~(U16)((U16)(val))
#define REG_FCIE_CLRBIT(reg_addr, val)      REG_FCIE((U32)(reg_addr)) = (U16)(REG_FCIE(reg_addr) & (~(U16)(val)))

#define REG_FCIE_W1C(reg_addr, val)         REG_FCIE_W((U32)(reg_addr), REG_FCIE(reg_addr)&(val))

//------------------------------
#define RIU_PM_BASE                         (IO_ADDRESS(0x1F000000U))
#define RIU_BASE                            (IO_ADDRESS(0x1F200000U))

#define REG_BANK_FCIE0                      0x8980U // 1113h x 80h x 4 + 1F000000
#define REG_BANK_FCIE1                      0x89E0U
#define REG_BANK_FCIE2                      0x8A00U
#define REG_BANK_FCIE_CRC                   0x11D80U // (0x123B - 0x1000) x 80h

#define FCIE0_BASE                          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE0)
#define FCIE1_BASE                          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE1)
#define FCIE2_BASE                          GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE2)
#define FCIE_CRC_BASE                       GET_REG_ADDR(RIU_BASE, REG_BANK_FCIE_CRC)

#define FCIE_REG_BASE_ADDR                  FCIE0_BASE
#define FCIE_CIFC_BASE_ADDR                 FCIE1_BASE
#define FCIE_CIFD_BASE_ADDR                 FCIE2_BASE
#define FCIE_CRC_BASE_ADDR                  FCIE_CRC_BASE

#include "eMMC_reg.h"

#define FCIE_DEBUG_BUS0                     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x31)
#define FCIE_DEBUG_BUS1                     GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x32)

//------------------------------ [FIXME]: refer to Bring-up Note -->
#define REG_BANK_CLKGEN                     0x580U
#define CLKGEN_BASE                         GET_REG_ADDR(RIU_BASE, REG_BANK_CLKGEN)
#define reg_ckg_fcie_1X                     GET_REG_ADDR(CLKGEN_BASE, 0x64)
#define reg_ckg_fcie_4X                     GET_REG_ADDR(CLKGEN_BASE, 0x64)
#define BIT_FCIE_CLK_Gate                   BIT0
#define BIT_FCIE_CLK_Inverse                BIT1
#define BIT_FCIE_CLK_MASK                   (BIT2|BIT3|BIT4|BIT5)
#define BIT_FCIE_CLK_SHIFT                  2
#define BIT_FCIE_CLK_SEL                    BIT6 // 1: NFIE, 0: 12MHz
#define BIT_FCIE_CLK4X_Gate                 BIT8
#define BIT_FCIE_CLK4X_Inverse              BIT9
#define BIT_FCIE_CLK4X_MASK                 (BIT10|BIT11|BIT12)
#define BIT_FCIE_CLK4X_SHIFT                10

#define reg_ckg_MCU                         GET_REG_ADDR(CLKGEN_BASE, 0x10)
#define reg_ckg_MIU                         GET_REG_ADDR(CLKGEN_BASE, 0x1D)

#define BIT_FCIE_CLK_XTAL                   0
#define BIT_FCIE_CLK_20M                    1
#define BIT_FCIE_CLK_27M                    2
#define BIT_FCIE_CLK_32M                    3
#define BIT_FCIE_CLK_36M                    4
#define BIT_FCIE_CLK_40M                    5
#define BIT_FCIE_CLK_43_2M                  6
#define BIT_FCIE_CLK_300K                   13
#define BIT_FCIE_CLK_48M                    15

#define BIT_FCIE_CLK4X_20M                  0
#define BIT_FCIE_CLK4X_27M                  1
#define BIT_FCIE_CLK4X_36M                  2
#define BIT_FCIE_CLK4X_40M                  3
#define BIT_FCIE_CLK4X_48M                  4

#define eMMC_FCIE_VALID_CLK_CNT             5 // FIXME
extern  U8 gau8_FCIEClkSel[];

//------------------------------
#define REG_BANK_CHIPTOP                    0xF00
#define PAD_CHIPTOP_BASE                    GET_REG_ADDR(RIU_BASE, REG_BANK_CHIPTOP)

#define reg_chiptop_0x12                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x12)
#define reg_chiptop_0x64                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x64)
#define reg_chiptop_0x40                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x40)
#define reg_chiptop_0x10                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x10)
#define reg_chiptop_0x6F                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x6F)
#define reg_chiptop_0x5A                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x5A)
#define reg_chiptop_0x6E                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x6E)
#define reg_chiptop_0x08                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x08)
#define reg_chiptop_0x09                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x09)
#define reg_chiptop_0x0A                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x0A)
#define reg_chiptop_0x0B                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x0B)
#define reg_chiptop_0x0C                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x0C)
#define reg_chiptop_0x0D                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x0D)
#define reg_chiptop_0x50                    GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x50)

#define reg_chip_config                     GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x65)

#define reg_chip_dummy1                     GET_REG_ADDR(PAD_CHIPTOP_BASE, 0x1D)
#define BIT_DDR_TIMING_PATCH                BIT8
#define BIT_SW_RST_Z                        BIT9
#define BIT_SW_RST_Z_EN                     BIT10

#define BIT_ROM_EMMC_MODE                   BIT1
#define BIT_EMMC_MODE_1                     BIT6
#define BIT_EMMC_MODE_3                     (BIT6|BIT7)

#define eMMC_RST_L()
#define eMMC_RST_H()

//------------------------------
#define REG_BANK_TIMER1                     0x1800
#define TIMER1_BASE                         GET_REG_ADDR(RIU_PM_BASE, REG_BANK_TIMER1)

#define TIMER1_ENABLE                       GET_REG_ADDR(TIMER1_BASE, 0x20)
#define TIMER1_HIT                          GET_REG_ADDR(TIMER1_BASE, 0x21)
#define TIMER1_MAX_LOW                      GET_REG_ADDR(TIMER1_BASE, 0x22)
#define TIMER1_MAX_HIGH                     GET_REG_ADDR(TIMER1_BASE, 0x23)
#define TIMER1_CAP_LOW                      GET_REG_ADDR(TIMER1_BASE, 0x24)
#define TIMER1_CAP_HIGH                     GET_REG_ADDR(TIMER1_BASE, 0x25)

#define BIT_SD_DEFAULT_MODE_REG             (BIT_SD_CLK_AUTO_STOP|BIT_SD_DATA_SYNC|BIT_SD_CLK_EN)

//=====================================================
// API declarations
//=====================================================
extern  U32 eMMC_hw_timer_delay(U32 u32us);

extern  U32 eMMC_hw_timer_start(void);
extern  U32 eMMC_hw_timer_tick(void);

#define FCIE_eMMC_DISABLE                   0
#define FCIE_eMMC_DDR                       1
#define FCIE_eMMC_SDR                       2
#define FCIE_eMMC_BYPASS                    3
#define FCIE_eMMC_TMUX                      4
#define FCIE_DEFAULT_PAD                    FCIE_eMMC_SDR // [FIXME]

extern  U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type);

extern  U32 eMMC_clock_setting(U32 u32ClkParam);
extern  U32 eMMC_clock_gating(void);
extern void eMMC_set_WatchDog(U8 u8_IfEnable);
extern void eMMC_reset_WatchDog(void);
extern  U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
extern void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size);
extern void eMMC_flush_miu_pipe(void);
extern  U32 eMMC_PlatformResetPre(void);
extern  U32 eMMC_PlatformResetPost(void);
extern  U32 eMMC_PlatformInit(void);
extern  U32 eMMC_CheckIfMemCorrupt(void);
extern void eMMC_DumpPadClk(void);
extern void eMMC_GPIO60_init(void);
extern void eMMC_GPIO60_Debug(U8 On);
extern void eMMC_GPIO61_init(void);
extern void eMMC_GPIO61_Debug(U8 On);
extern U16* eMMC_GetCRCCode(void);
extern void eMMC_DumpCRCBank(void);

extern  irqreturn_t eMMC_FCIE_IRQ(int irq, void *dummy); // [FIXME]
extern  U32 eMMC_WaitCompleteIntr(U32 u32_RegAddr, U16 u16_WaitEvent, U32 u32_MicroSec);

extern void eMMC_LockFCIE(void);
extern void eMMC_UnlockFCIE(void);

//=====================================================
// Driver configs
//=====================================================
#define eMMC_ST_PLAT                        0x80000000
// [CAUTION]: to detect DDR timiing parameters
#define IF_DETECT_eMMC_DDR_TIMING           1
// [CAUTION]: to verify IP and HAL code, defaut 0
#define IF_IP_VERIFY                        0 // [FIXME] -->

#define IF_FCIE_SHARE_PINS                  0 // 1: need to eMMC_pads_switch
#define IF_FCIE_SHARE_CLK                   0 // 1: need to eMMC_clock_setting
#define IF_FCIE_SHARE_IP                    0

//------------------------------
#define FICE_BYTE_MODE_ENABLE               1 // always 1
#define ENABLE_eMMC_INTERRUPT_MODE          1
#define ENABLE_eMMC_RIU_MODE                0 // for debug cache issue

#define ENABLE_FCIE_HW_BUSY_CHECK           1

/* For debugging purpose */
#define MMC_SPEED_TEST                      0

#if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
#define DMA_TIME_TEST                       0
#endif

#define eMMC_CACHE_LINE                     0x20 // [FIXME]

//=====================================================
// tool-chain attributes
//===================================================== [FIXME] -->
#define eMMC_PACK0
#define eMMC_PACK1                          __attribute__((__packed__))
#define eMMC_ALIGN0
#define eMMC_ALIGN1                         __attribute__((aligned(eMMC_CACHE_LINE)))
// <-- [FIXME]

//=====================================================
// debug option
//=====================================================
#define eMMC_TEST_IN_DESIGN                 0 // [FIXME]: set 1 to verify HW timer

#ifndef eMMC_DEBUG_MSG
#define eMMC_DEBUG_MSG                      1
#endif

/* Define trace levels. */
#define eMMC_DEBUG_LEVEL_ERROR              (1)    /* Error condition debug messages. */
#define eMMC_DEBUG_LEVEL_WARNING            (2)    /* Warning condition debug messages. */
#define eMMC_DEBUG_LEVEL_HIGH               (3)    /* Debug messages (high debugging). */
#define eMMC_DEBUG_LEVEL_MEDIUM             (4)    /* Debug messages. */
#define eMMC_DEBUG_LEVEL_LOW                (5)    /* Debug messages (low debugging). */

/* Higer debug level means more verbose */
#ifndef eMMC_DEBUG_LEVEL
#define eMMC_DEBUG_LEVEL                    eMMC_DEBUG_LEVEL_HIGH//eMMC_DEBUG_LEVEL_LOW
#endif

#if defined(eMMC_DEBUG_MSG) && eMMC_DEBUG_MSG
#define eMMC_printf    printk                      // <-- [FIXME]
#define eMMC_debug(dbg_lv, tag, str, ...)	\
	do {	\
		if (dbg_lv > eMMC_DEBUG_LEVEL)				    \
			break;									    \
		else {										    \
			if (tag)								    \
				eMMC_printf("[ %s() Ln.%u ] ", __FUNCTION__, __LINE__);	\
													\
			eMMC_printf(str, ##__VA_ARGS__);		\
		} \
	} while(0)
#else /* eMMC_DEBUG_MSG */
#define eMMC_printf(...)
#define eMMC_debug(enable, tag, str, ...)   do{}while(0)
#endif /* eMMC_DEBUG_MSG */

#define eMMC_die(str) eMMC_printf("eMMC Die: %s() Ln.%u, %s \n", __FUNCTION__, __LINE__, str); \
	                  panic("\n");

#define eMMC_stop() \
	while(1)  eMMC_reset_WatchDog();

//=====================================================
// unit for HW Timer delay (unit of us)
//=====================================================
#define HW_TIMER_DELAY_1us                  1
#define HW_TIMER_DELAY_5us                  5
#define HW_TIMER_DELAY_10us                 10
#define HW_TIMER_DELAY_100us                100
#define HW_TIMER_DELAY_1ms                  (1000 * HW_TIMER_DELAY_1us)
#define HW_TIMER_DELAY_5ms                  (5    * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_10ms                 (10   * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_100ms                (100  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_500ms                (500  * HW_TIMER_DELAY_1ms)
#define HW_TIMER_DELAY_1s                   (1000 * HW_TIMER_DELAY_1ms)

//=====================================================
// set FCIE clock
//=====================================================
// [FIXME] -->
#define FCIE_SLOWEST_CLK                    BIT_FCIE_CLK_300K
#define FCIE_SLOW_CLK                       BIT_FCIE_CLK_20M
#define FCIE_DEFAULT_CLK                    BIT_FCIE_CLK_48M

//=====================================================
// misc
//=====================================================
//#define BIG_ENDIAN
#define LITTLE_ENDIAN

#if (defined(BIT_DQS_MODE_MASK) && (BIT_DQS_MODE_MASK != (BIT12|BIT13|BIT14)))

#undef BIT_DQS_MODE_MASK
#undef BIT_DQS_MODE_2T
#undef BIT_DQS_MODE_1_5T
#undef BIT_DQS_MODE_2_5T
#undef BIT_DQS_MODE_1T

#define BIT_DQS_MODE_MASK               (BIT12|BIT13|BIT14)
#define BIT_DQS_MODE_0T                 (0 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_0_5T               (1 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1T                 (2 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_1_5T               (3 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_2T                 (4 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_2_5T               (5 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_3T                 (6 << BIT_DQS_MDOE_SHIFT)
#define BIT_DQS_MODE_3_5T               (7 << BIT_DQS_MDOE_SHIFT)

#endif

#define MSTAR_DEMO_BOARD                0
#define SEC_X12_BOARD                   1

#endif /* __eMMC_X10P_LINUX__ */
