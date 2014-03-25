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

#include "eMMC.h"
#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

//=============================================================
#if (defined(eMMC_DRV_TV_LINUX)&&eMMC_DRV_TV_LINUX) // [FIXME] clone for your platform
// [FIXME] -->
U32 eMMC_hw_timer_delay(U32 u32us)
{
    #if 0

    U32 u32HWTimer = 0;
    U32 u32TimerLow = 0;
    U32 u32TimerHigh = 0;

    REG_FCIE_W(TIMER1_MAX_LOW, 0xFFFF);
    REG_FCIE_W(TIMER1_MAX_HIGH, 0xFFFF);
    REG_FCIE_W(TIMER1_ENABLE, 0);

    // Start PIU Timer1
    REG_FCIE_W(TIMER1_ENABLE, 0x1);

    // Get timer value
    while( u32HWTimer < (u32us*12) )
    {
        u32TimerLow = REG_FCIE(TIMER1_CAP_LOW);
        u32TimerHigh = REG_FCIE(TIMER1_CAP_HIGH);

        u32HWTimer = (u32TimerHigh<<16) | u32TimerLow;
    }

    REG_FCIE_W(TIMER1_ENABLE, 0);

    return (u32HWTimer/12);

    #else

    udelay(u32us);
    return u32us;

    #endif
}

//--------------------------------
// use to performance test
U32 eMMC_hw_timer_start(void)
{
    // Reset PIU Timer1
    REG_FCIE_W(TIMER1_MAX_LOW, 0xFFFF);
    REG_FCIE_W(TIMER1_MAX_HIGH, 0xFFFF);
    REG_FCIE_W(TIMER1_ENABLE, 0);

    // Start PIU Timer1
    REG_FCIE_W(TIMER1_ENABLE, 0x1);
    return 0;
}

U32 eMMC_hw_timer_tick(void)
{
    U32 u32HWTimer = 0;
    U32 u32TimerLow = 0;
    U32 u32TimerHigh = 0;

    // Get timer value
    u32TimerLow = REG_FCIE(TIMER1_CAP_LOW);
    u32TimerHigh = REG_FCIE(TIMER1_CAP_HIGH);

    u32HWTimer = (u32TimerHigh<<16) | u32TimerLow;

    //REG_FCIE_W(TIMER1_ENABLE, 0);

    return u32HWTimer;
}
//--------------------------------

void eMMC_DumpPadClk(void)
{
    //----------------------------------------------
    eMMC_debug(0, 0, "\n[clk setting]: %uKHz \n", g_eMMCDrv.u32_ClkKHz);
    eMMC_debug(0, 0, "FCIE 1X (0x%X):0x%04X\n", reg_ckg_fcie_1X, REG_FCIE_U16(reg_ckg_fcie_1X));
    eMMC_debug(0, 0, "FCIE 4X (0x%X):0x%04X\n", reg_ckg_fcie_4X, REG_FCIE_U16(reg_ckg_fcie_4X));
    eMMC_debug(0, 0, "MIU (0x%X):0x%04X\n", reg_ckg_MIU, REG_FCIE_U16(reg_ckg_MIU));
    eMMC_debug(0, 0, "MCU (0x%X):0x%04X\n", reg_ckg_MCU, REG_FCIE_U16(reg_ckg_MCU));

    //----------------------------------------------
    eMMC_debug(0, 0, "\n[pad setting]: ");
    switch(g_eMMCDrv.u32_Flag & FCIE_FLAG_PADTYPE_MASK)
    {
        case FCIE_FLAG_PADTYPE_DDR:
            eMMC_debug(0,0,"DDR\n");  break;
        case FCIE_FLAG_PADTYPE_SDR:
            eMMC_debug(0,0,"SDR\n");  break;
        case FCIE_FLAG_PADTYPE_BYPASS:
            eMMC_debug(0,0,"BYPASS\n");  break;
        default:
            eMMC_debug(0,0,"eMMC Err: Pad unknown\n");  eMMC_die("\n");
    }

    #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
    eMMC_debug(0, 0, "chiptop_0x02 (0x%X):0x%04X\n", reg_chiptop_0x02, REG_FCIE_U16(reg_chiptop_0x02));
    eMMC_debug(0, 0, "chiptop_0x42 (0x%X):0x%04X\n", reg_chiptop_0x42, REG_FCIE_U16(reg_chiptop_0x42));
    eMMC_debug(0, 0, "chiptop_0x64 (0x%X):0x%04X\n", reg_chiptop_0x64, REG_FCIE_U16(reg_chiptop_0x64));
    eMMC_debug(0, 0, "chiptop_0x6F (0x%X):0x%04X\n", reg_chiptop_0x6F, REG_FCIE_U16(reg_chiptop_0x6F));
    eMMC_debug(0, 0, "chiptop_0x10 (0x%X):0x%04X\n", reg_chiptop_0x10, REG_FCIE_U16(reg_chiptop_0x10));
    eMMC_debug(0, 0, "chiptop_0x1C (0x%X):0x%04X\n", reg_chiptop_0x1C, REG_FCIE_U16(reg_chiptop_0x1C));
    eMMC_debug(0, 0, "chiptop_0x6E (0x%X):0x%04X\n", reg_chiptop_0x6E, REG_FCIE_U16(reg_chiptop_0x6E));
    eMMC_debug(0, 0, "chiptop_0x5A (0x%X):0x%04X\n", reg_chiptop_0x5A, REG_FCIE_U16(reg_chiptop_0x5A));
    eMMC_debug(0, 0, "chiptop_0x50 (0x%X):0x%04X\n", reg_chiptop_0x50, REG_FCIE_U16(reg_chiptop_0x50));
    #elif defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX
    eMMC_debug(0, 0, "chiptop_0x12 (0x%X):0x%04X\n", reg_chiptop_0x12, REG_FCIE_U16(reg_chiptop_0x12));
    eMMC_debug(0, 0, "chiptop_0x64 (0x%X):0x%04X\n", reg_chiptop_0x64, REG_FCIE_U16(reg_chiptop_0x64));
    eMMC_debug(0, 0, "chiptop_0x40 (0x%X):0x%04X\n", reg_chiptop_0x40, REG_FCIE_U16(reg_chiptop_0x40));
    eMMC_debug(0, 0, "chiptop_0x10 (0x%X):0x%04X\n", reg_chiptop_0x10, REG_FCIE_U16(reg_chiptop_0x10));
    eMMC_debug(0, 0, "chiptop_0x1D (0x%X):0x%04X\n", reg_chip_dummy1, REG_FCIE_U16(reg_chip_dummy1));
    eMMC_debug(0, 0, "chiptop_0x6F (0x%X):0x%04X\n", reg_chiptop_0x6F, REG_FCIE_U16(reg_chiptop_0x6F));
    eMMC_debug(0, 0, "chiptop_0x5A (0x%X):0x%04X\n", reg_chiptop_0x5A, REG_FCIE_U16(reg_chiptop_0x5A));
    eMMC_debug(0, 0, "chiptop_0x6E (0x%X):0x%04X\n", reg_chiptop_0x6E, REG_FCIE_U16(reg_chiptop_0x6E));
    eMMC_debug(0, 0, "chiptop_0x08 (0x%X):0x%04X\n", reg_chiptop_0x08, REG_FCIE_U16(reg_chiptop_0x08));
    eMMC_debug(0, 0, "chiptop_0x09 (0x%X):0x%04X\n", reg_chiptop_0x09, REG_FCIE_U16(reg_chiptop_0x09));
    eMMC_debug(0, 0, "chiptop_0x0A (0x%X):0x%04X\n", reg_chiptop_0x0A, REG_FCIE_U16(reg_chiptop_0x0A));
    eMMC_debug(0, 0, "chiptop_0x0B (0x%X):0x%04X\n", reg_chiptop_0x0B, REG_FCIE_U16(reg_chiptop_0x0B));
    eMMC_debug(0, 0, "chiptop_0x0C (0x%X):0x%04X\n", reg_chiptop_0x0C, REG_FCIE_U16(reg_chiptop_0x0C));
    eMMC_debug(0, 0, "chiptop_0x0D (0x%X):0x%04X\n", reg_chiptop_0x0D, REG_FCIE_U16(reg_chiptop_0x0D));
    eMMC_debug(0, 0, "chiptop_0x50 (0x%X):0x%04X\n", reg_chiptop_0x50, REG_FCIE_U16(reg_chiptop_0x50));
    eMMC_debug(0, 0, "chiptop_0x65 (0x%X):0x%04X\n", reg_chip_config, REG_FCIE_U16(reg_chip_config));
    #endif

    eMMC_debug(0, 0, "\n");
}

U32 eMMC_pads_switch(U32 u32_FCIE_IF_Type)
{
    g_eMMCDrv.u32_Flag &= ~FCIE_FLAG_PADTYPE_MASK;

    #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX

    REG_FCIE_CLRBIT(reg_chiptop_0x6E, BIT6|BIT7);       // reg_emmc_config
    REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT8|BIT9|BIT10);

    #endif

    switch(u32_FCIE_IF_Type){
        case FCIE_eMMC_DDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: DDR\n");

            // set DDR mode
            #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX

            REG_FCIE_SETBIT(reg_chiptop_0x6E, BIT7);

            #elif defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX

            // Need to confirm with designer
            REG_FCIE_CLRBIT(FCIE_REG_2Dh, BIT0);

            #endif

            REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT8|BIT9);

            g_eMMCDrv.u32_Flag |= FCIE_FLAG_PADTYPE_DDR;
            break;

        case FCIE_eMMC_SDR:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: SDR\n");

            // set SDR mode
            #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX

            REG_FCIE_SETBIT(reg_chiptop_0x6E, BIT7);
            REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT9);

            #elif defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX

            REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT8);

            #endif

            g_eMMCDrv.u32_Flag |= FCIE_FLAG_PADTYPE_SDR;
            break;

        case FCIE_eMMC_BYPASS:
            eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0,"eMMC pads: BYPASS\n");

            // set bypass mode
            #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX

            REG_FCIE_SETBIT(reg_chiptop_0x6E, BIT6);
            REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT10);

            #elif defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX

            REG_FCIE_SETBIT(reg_chiptop_0x10, BIT8);
            REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT8|BIT10|BIT11);

            #endif

            g_eMMCDrv.u32_Flag |= FCIE_FLAG_PADTYPE_BYPASS;
            break;

        default:
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: unknown interface: %X\n",u32_FCIE_IF_Type);
            return eMMC_ST_ERR_INVALID_PARAM;
    }

    // set chiptop

    #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX

    REG_FCIE_CLRBIT(reg_chiptop_0x02, BIT0|BIT1);
    REG_FCIE_CLRBIT(reg_chiptop_0x42, BIT8|BIT10);
    REG_FCIE_CLRBIT(reg_chiptop_0x64, BIT0|BIT3|BIT4);
    REG_FCIE_CLRBIT(reg_chiptop_0x6F, BIT2|BIT3|BIT6|BIT7);
    REG_FCIE_CLRBIT(reg_chiptop_0x10, BIT1|BIT2);
    REG_FCIE_CLRBIT(reg_chiptop_0x1C, BIT12);
    REG_FCIE_CLRBIT(reg_chiptop_0x5A, BIT5);

    REG_FCIE_CLRBIT(reg_chiptop_0x50, BIT15);

    #endif

    return eMMC_ST_SUCCESS;
}

#if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
static U8 sgau8_FCIEClk_1X_To_4X_[0x10]= // index is 1X reg value
{
    0,
    BIT_FCIE_CLK4X_20M,
    BIT_FCIE_CLK4X_27M,
    0,
    BIT_FCIE_CLK4X_36M,
    BIT_FCIE_CLK4X_40M,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    BIT_FCIE_CLK4X_48M
};
#endif

U32 eMMC_clock_setting(U32 u32ClkParam)
{
    eMMC_PlatformResetPre();

    switch(u32ClkParam)	{
        case BIT_FCIE_CLK_300K:     g_eMMCDrv.u32_ClkKHz = 300;     break;
        case BIT_FCIE_CLK_20M:      g_eMMCDrv.u32_ClkKHz = 20000;   break;
        #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
        case BIT_FCIE_CLK_24M:      g_eMMCDrv.u32_ClkKHz = 24000;   break;
        #endif
        case BIT_FCIE_CLK_27M:      g_eMMCDrv.u32_ClkKHz = 27000;   break;
        #if !(defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING)
        case BIT_FCIE_CLK_32M:      g_eMMCDrv.u32_ClkKHz = 32000;   break;
        #endif
        case BIT_FCIE_CLK_36M:      g_eMMCDrv.u32_ClkKHz = 36000;   break;
        case BIT_FCIE_CLK_40M:      g_eMMCDrv.u32_ClkKHz = 40000;   break;
        #if !(defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING)
        case BIT_FCIE_CLK_43_2M:    g_eMMCDrv.u32_ClkKHz = 43200;   break;
        #endif
        case BIT_FCIE_CLK_48M:      g_eMMCDrv.u32_ClkKHz = 48000;   break;
        default:
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh\n", eMMC_ST_ERR_INVALID_PARAM);
            return eMMC_ST_ERR_INVALID_PARAM;
    }

    REG_FCIE_CLRBIT(reg_ckg_fcie_1X, (BIT_FCIE_CLK_Gate|BIT_FCIE_CLK_MASK));
    REG_FCIE_SETBIT(reg_ckg_fcie_1X, BIT_FCIE_CLK_SEL|(u32ClkParam<<BIT_FCIE_CLK_SHIFT));

    #if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
    if( g_eMMCDrv.u32_Flag & FCIE_FLAG_DDR_MODE )
	{
	    REG_FCIE_CLRBIT(reg_ckg_fcie_4X,
			BIT_FCIE_CLK4X_Gate|BIT_FCIE_CLK4X_MASK);
	    REG_FCIE_SETBIT(reg_ckg_fcie_4X,
			(sgau8_FCIEClk_1X_To_4X_[u32ClkParam]<<BIT_FCIE_CLK4X_SHIFT));

        #if defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX
		REG_FCIE_CLRBIT(FCIE_PATH_CTRL, BIT_SD_EN);
		//REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG,BIT_SD_DDR_EN);
		REG_FCIE_SETBIT(reg_chip_dummy1,BIT_DDR_TIMING_PATCH);
        REG_FCIE_SETBIT(reg_chip_dummy1,BIT_SW_RST_Z_EN);
		REG_FCIE_SETBIT(reg_chip_dummy1,BIT_SW_RST_Z);
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        REG_FCIE_CLRBIT(reg_chip_dummy1,BIT_SW_RST_Z);
		REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_SD_EN);
		REG_FCIE_SETBIT(FCIE_BOOT_CONFIG,BIT_SD_DDR_EN);
        #endif
	}
    #endif

    eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0, "clk:%uKHz, Param:%Xh, fcie_1X(%Xh):%Xh, fcie_4X(%Xh):%Xh\n",
               g_eMMCDrv.u32_ClkKHz, u32ClkParam,
               reg_ckg_fcie_1X, REG_FCIE_U16(reg_ckg_fcie_1X),
               reg_ckg_fcie_4X, REG_FCIE_U16(reg_ckg_fcie_4X));

    g_eMMCDrv.u16_ClkRegVal = (U16)u32ClkParam;
    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;
}

U32 eMMC_clock_gating(void)
{
    eMMC_PlatformResetPre();
    g_eMMCDrv.u32_ClkKHz = 0;
    REG_FCIE_W(reg_ckg_fcie_1X, BIT_FCIE_CLK_Gate);
    REG_FCIE_W(reg_ckg_fcie_4X, BIT_FCIE_CLK4X_Gate);
    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;
}

U8 gau8_FCIEClkSel[eMMC_FCIE_VALID_CLK_CNT]={
	BIT_FCIE_CLK_48M,
	BIT_FCIE_CLK_40M,
	BIT_FCIE_CLK_36M,
	BIT_FCIE_CLK_27M,
	BIT_FCIE_CLK_20M,
};

void eMMC_set_WatchDog(U8 u8_IfEnable)
{
    // do nothing
}

void eMMC_reset_WatchDog(void)
{
    // do nothing
}

U32 eMMC_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
    return (virt_to_phys((void *)u32_DMAAddr));
}

void eMMC_Invalidate_data_cache_buffer(U32 u32_addr, S32 s32_size)
{

}

void eMMC_flush_miu_pipe(void)
{

}

//---------------------------------------
#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
static DECLARE_WAIT_QUEUE_HEAD(fcie_wait);
static volatile U32 fcie_int = 0;
#endif

#if defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK
static DECLARE_WAIT_QUEUE_HEAD(emmc_busy_wait);
static volatile U32 emmc_busy_int = 0;
#endif

#if (defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE) || \
    (defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK)
irqreturn_t eMMC_FCIE_IRQ(int irq, void *dummy)
{
    volatile u32 u32_Events;
    volatile u32 u32_mie_int;

    u32_Events = REG_FCIE(FCIE_MIE_EVENT);
    u32_mie_int = REG_FCIE(FCIE_MIE_INT_EN);

    #if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
    if((u32_Events & u32_mie_int) == BIT_SD_CMD_END)
    {
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_SD_CMD_END);

        fcie_int = 1;
        wake_up(&fcie_wait);
    }

    if((u32_Events & u32_mie_int) == BIT_MIU_LAST_DONE)
    {
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);

        fcie_int = 1;
        wake_up(&fcie_wait);
    }

    if((u32_Events & u32_mie_int) == BIT_CARD_DMA_END)
    {
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);

        fcie_int = 1;
        wake_up(&fcie_wait);
    }
    #endif

    #if defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK
    if((u32_Events & u32_mie_int) == BIT_SD_BUSY_END)
    {
        REG_FCIE_CLRBIT(FCIE_SD_CTRL, BIT_SD_BUSY_DET_ON);
        REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, BIT_SD_BUSY_END);

        emmc_busy_int = 1;
        wake_up(&emmc_busy_wait);
    }
    #endif

    return IRQ_HANDLED;
}

#if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
U32 eMMC_WaitCompleteIntr(U32 u32_RegAddr, U16 u16_WaitEvent, U32 u32_MicroSec)
{
    U32 u32_i;

    if( wait_event_timeout(fcie_wait, (fcie_int == 1), (long int)usecs_to_jiffies(u32_MicroSec)) == 0 )
    {
        // switch to polling
        for(u32_i=0; u32_i<u32_MicroSec; u32_i++)
        {
            if((REG_FCIE(FCIE_MIE_EVENT) & u16_WaitEvent) == u16_WaitEvent )
                break;
            eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        }

        if(u32_i == u32_MicroSec)
        {
            // If it is still timeout, check Start bit first
            REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_FCIE_DEBUG_MODE_MASK);
            REG_FCIE_SETBIT(FCIE_TEST_MODE, 1<<BIT_FCIE_DEBUG_MODE_SHIFT);

            if( (REG_FCIE(FCIE_DEBUG_BUS) & 0xFF) == 0x20 )
            {
				//eMMC_GPIO60_Debug(1);
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: lost start bit (3 sec), let mmc sub system to try again \n");

                eMMC_FCIE_Reset();
			    return u32_MicroSec;
            }
            else
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: events lose, WaitEvent: %Xh \n", u16_WaitEvent);
                eMMC_FCIE_ErrHandler_Stop();
                return eMMC_ST_ERR_INT_TO;
            }
        }
        else
        {
            REG_FCIE_CLRBIT(FCIE_MIE_INT_EN, u16_WaitEvent);
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Warn: interrupt lose but polling ok: %Xh \n", REG_FCIE(FCIE_MIE_EVENT));
        }
    }

    fcie_int = 0;

    return eMMC_ST_SUCCESS;
}
#endif
#endif

U32 eMMC_FCIE_WaitD0High(void)
{
    volatile U32 u32_cnt;
    U16 u16_read0, u16_read1;

    #if defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK

    // enable busy int
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_BUSY_DET_ON);
    REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_SD_BUSY_END);

    // wait event
    if(wait_event_timeout(emmc_busy_wait, (emmc_busy_int==1), (long int)usecs_to_jiffies(HW_TIMER_DELAY_1s*3))==0)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: wait busy int timeout\n");

        for(u32_cnt=0; u32_cnt < TIME_WAIT_DAT0_HIGH; u32_cnt++)
        {
            REG_FCIE_R(FCIE_SD_STATUS, u16_read0);
            eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
            REG_FCIE_R(FCIE_SD_STATUS, u16_read1);

            if((u16_read0&BIT_SD_CARD_D0_ST) && (u16_read1&BIT_SD_CARD_D0_ST))
                break;
        }

        if(TIME_WAIT_DAT0_HIGH == u32_cnt)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: wait D0 H timeout %u us\n", u32_cnt);
            return eMMC_ST_ERR_TIMEOUT_WAITD0HIGH;
        }
    }

    emmc_busy_int = 0;

    REG_FCIE_W1C(FCIE_MIE_EVENT, BIT_SD_BUSY_END);

    #else

    for(u32_cnt=0; u32_cnt < TIME_WAIT_DAT0_HIGH; u32_cnt++)
    {
        REG_FCIE_R(FCIE_SD_STATUS, u16_read0);
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        REG_FCIE_R(FCIE_SD_STATUS, u16_read1);

        if((u16_read0&BIT_SD_CARD_D0_ST) && (u16_read1&BIT_SD_CARD_D0_ST))
            break;
    }

    if(TIME_WAIT_DAT0_HIGH == u32_cnt)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: wait D0 H timeout %u us\n", u32_cnt);
        return eMMC_ST_ERR_TIMEOUT_WAITD0HIGH;
    }

    #endif

    return eMMC_ST_SUCCESS;

}

void eMMC_GPIO60_init(void)
{
    *(volatile U16*)(IO_ADDRESS(0x1f205618)) &= (U16)(~(BIT0|BIT1));
}

void eMMC_GPIO60_Debug(U8 On)
{
    if(On)
    {
        *(volatile U16*)(IO_ADDRESS(0x1f205618)) |= BIT0; // rise the GPIO 60 high
    }
    else
    {
        *(volatile U16*)(IO_ADDRESS(0x1f205618)) &= (U16)(~BIT0); // fall the GPIO 60 down
    }
}

void eMMC_GPIO61_init(void)
{
    *(volatile U16*)(IO_ADDRESS(0x1f205618)) &= (U16)(~(BIT8|BIT9));
}

void eMMC_GPIO61_Debug(U8 On)
{
    if(On)
    {
        *(volatile U16*)(IO_ADDRESS(0x1f205618)) |= BIT8; // rise the GPIO 61 high
    }
    else
    {
        *(volatile U16*)(IO_ADDRESS(0x1f205618)) &= (U16)(~BIT8); // fall the GPIO 61 down
    }
}

#if defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX
U16* eMMC_GetCRCCode(void)
{
    U16 u16_i;
    U16 u16_CRCLen;

    if( g_eMMCDrv.u32_Flag & FCIE_FLAG_DDR_MODE )
    {
        if( g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8)
            u16_CRCLen = 16;
        else
            u16_CRCLen = 8;
    }
    else
    {
        if( g_eMMCDrv.u8_BUS_WIDTH == BIT_SD_DATA_WIDTH_8)
            u16_CRCLen = 8;
        else
            u16_CRCLen = 4;
    }

    for(u16_i=0; u16_i<u16_CRCLen; u16_i++)
        g_eMMCDrv.u16_LastBlkCRC[u16_i] = REG_FCIE(GET_REG_ADDR(FCIE_CRC_BASE_ADDR, u16_i));

    return &g_eMMCDrv.u16_LastBlkCRC[0];
}

//EXPORT_SYMBOL(eMMC_GetCRCCode);
#endif

void eMMC_DumpCRCBank(void)
{
    #if defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX
    U16 u16_i;
    volatile U16 u16_reg;

    eMMC_debug(0, 0, "\n[CRC]: ");
    eMMC_debug(0, 0, "\n blkno: %08Xh: ", g_eMMCDrv.u32_LastBlkNo);
    for(u16_i=0 ; u16_i<0x10; u16_i++)
    {
        if(0 == u16_i%8)
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n%02Xh:| ", u16_i);

        REG_FCIE_R(GET_REG_ADDR(FCIE_CRC_BASE_ADDR, u16_i), u16_reg);
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "%04Xh ", u16_reg);
    }
    #endif
}

//EXPORT_SYMBOL(eMMC_DumpCRCBank);


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
#if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
DEFINE_SEMAPHORE(ond_mutex);
#endif
#else
DECLARE_MUTEX(ond_mutex);
#endif

#if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
EXPORT_SYMBOL(ond_mutex);
#endif

void eMMC_LockFCIE(void)
{
    #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
    down(&ond_mutex);
    #endif
}

void eMMC_UnlockFCIE(void)
{
    #if defined(eMMC_DRV_X10P_LINUX) && eMMC_DRV_X10P_LINUX
    up(&ond_mutex);
    #endif
}

//---------------------------------------

U32 eMMC_PlatformResetPre(void)
{
    return eMMC_ST_SUCCESS;
}

U32 eMMC_PlatformResetPost(void)
{
    return eMMC_ST_SUCCESS;
}


U32 eMMC_PlatformInit(void)
{
    eMMC_pads_switch(FCIE_DEFAULT_PAD);
    eMMC_clock_setting(FCIE_SLOWEST_CLK);

    return eMMC_ST_SUCCESS;
}

// --------------------------------------------
eMMC_ALIGN0 eMMC_DRIVER g_eMMCDrv eMMC_ALIGN1;

//=============================================================
#else

  #error "Error! no platform functions."

#endif

#endif
