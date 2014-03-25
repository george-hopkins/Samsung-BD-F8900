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

//========================================================
// HAL pre-processors
//========================================================
#if IF_FCIE_SHARE_PINS
    #define eMMC_PAD_SWITCH(enable)         eMMC_pads_switch(enable);
#else
    // NULL to save CPU a JMP/RET time
#define eMMC_PAD_SWITCH(enable)
#endif

#if IF_FCIE_SHARE_CLK
    #define eMMC_CLK_SETTING(ClkParam)      eMMC_clock_setting(ClkParam);
#else
    // NULL to save CPU a JMP/RET time
    #define eMMC_CLK_SETTING(ClkParam)
#endif

#if IF_FCIE_SHARE_IP
    // re-config FCIE3 for NFIE mode
    #define eMMC_RECONFIG()                 eMMC_ReConfig();
#else
    // NULL to save CPU a JMP/RET time
    #define eMMC_RECONFIG()
#endif

#define eMMC_FCIE_CLK_DIS()                 REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN)

#define eMMC_IF_DDRT_TUNING()               (g_eMMCDrv.u32_Flag&FCIE_FLAG_DDR_TUNING)
#define eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT     2
#define eMMC_CMD_API_ERR_RETRY_CNT          2
#define eMMC_CMD_API_WAIT_FIFOCLK_RETRY_CNT 2

#define eMMC_CMD1_RETRY_CNT                 0x1000
#define eMMC_CMD3_RETRY_CNT                 0x5

//========================================================
// HAL APIs
//========================================================
U32 eMMC_FCIE_WaitEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec)
{
    U32 u32_err;

    #if defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE
    u32_err = eMMC_WaitCompleteIntr(u32_RegAddr, u16_Events, u32_MicroSec);
    #else
    u32_err = eMMC_FCIE_PollingEvents(u32_RegAddr, u16_Events, u32_MicroSec);
    #endif

    return u32_err;
}

U32 eMMC_FCIE_PollingEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec)
{
    volatile U32 u32_i;
    volatile U16 u16_val;

    for(u32_i=0; u32_i<u32_MicroSec; u32_i++)
    {
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        REG_FCIE_R(u32_RegAddr, u16_val);
        if(u16_Events == (u16_val & u16_Events))
            break;
    }

    if(u32_i == u32_MicroSec)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                   "eMMC Err: %u us, Reg.%04Xh: %04Xh, but wanted: %04Xh\n",
                   u32_MicroSec,
                   (u32_RegAddr-FCIE_MIE_EVENT)>>REG_OFFSET_SHIFT_BITS,
                   u16_val, u16_Events);

        REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_FCIE_DEBUG_MODE_MASK);
        REG_FCIE_SETBIT(FCIE_TEST_MODE, 1<<BIT_FCIE_DEBUG_MODE_SHIFT);

        if( (REG_FCIE(FCIE_DEBUG_BUS) & 0xFF) == 0x20 )
        {
            eMMC_FCIE_Reset();
            return u32_MicroSec;
        }
        else
        {
            eMMC_FCIE_ErrHandler_Stop();
            return eMMC_ST_ERR_INT_TO;
        }
    }

    return eMMC_ST_SUCCESS;
}

static U16 sgau16_eMMCDebugBus[100];
void eMMC_FCIE_DumpDebugBus(void)
{
    volatile U16 u16_reg, u16_i;
    U16 u16_idx = 0;

    memset(sgau16_eMMCDebugBus, 0xFF, sizeof(sgau16_eMMCDebugBus));
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n\n");
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n");

    for(u16_i=0; u16_i<8; u16_i++)
    {
        REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_FCIE_DEBUG_MODE_MASK);
        REG_FCIE_SETBIT(FCIE_TEST_MODE, u16_i<<BIT_FCIE_DEBUG_MODE_SHIFT);
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "Debug Mode: %Xh\n", u16_i);

        REG_FCIE_R(FCIE_DEBUG_BUS0, u16_reg);
        sgau16_eMMCDebugBus[u16_idx] = u16_reg;
        u16_idx++;
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "FCIE_DEBUG_BUS0: %04Xh\n", u16_reg);

        REG_FCIE_R(FCIE_DEBUG_BUS1, u16_reg);
        sgau16_eMMCDebugBus[u16_idx] = u16_reg;
        u16_idx++;
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "FCIE_DEBUG_BUS1: %04Xh\n", u16_reg);
    }
}

static U16 sgau16_eMMCDebugReg[100];
void eMMC_FCIE_DumpRegisters(void)
{
    volatile U16 u16_reg;
    U16 u16_i;

    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n\nFCIE Reg:");

    for(u16_i=0 ; u16_i<0x60; u16_i++)
    {
        if(0 == u16_i%8)
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n%02Xh:| ", u16_i);

        REG_FCIE_R(GET_REG_ADDR(FCIE_REG_BASE_ADDR, u16_i), u16_reg);
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "%04Xh ", u16_reg);
    }

    printk("\n\nJOB_BL_CNT(0x0B)=\t%04X\n", REG_FCIE_U16(FCIE_JOB_BL_CNT));
	printk("TR_BK_CNT(0x0C)=\t%04X\n", REG_FCIE_U16(FCIE_TR_BK_CNT));

    REG_FCIE_SETBIT(FCIE_JOB_BL_CNT, BIT15);
    REG_FCIE_SETBIT(FCIE_JOB_BL_CNT, BIT14);
    printk("MIU side=\t%04X\n", REG_FCIE_U16(FCIE_TR_BK_CNT));

    REG_FCIE_CLRBIT(FCIE_JOB_BL_CNT, BIT14);
    printk("FCIE side=\t%04X\n", REG_FCIE_U16(FCIE_TR_BK_CNT));

    REG_FCIE_CLRBIT(FCIE_JOB_BL_CNT, BIT15);

    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n\nFCIE CIFC:");
    eMMC_FCIE_GetCIFC(0, 0x20, (U16*)sgau16_eMMCDebugReg);
    for(u16_i=0 ; u16_i<0x20; u16_i++)
    {
        if(0 == u16_i%8)
            eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n%02Xh:| ", u16_i);

        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "%04Xh ", sgau16_eMMCDebugReg[u16_i]);
    }
}

void eMMC_FCIE_ErrHandler_Stop(void)
{
    if(0==eMMC_IF_DDRT_TUNING())
    {
        //eMMC_DumpDriverStatus();
        eMMC_DumpPadClk();
        eMMC_FCIE_DumpRegisters();
        eMMC_FCIE_DumpDebugBus();
        eMMC_DumpCRCBank();
    }
    eMMC_die("\n");
}

void eMMC_FCIE_ErrHandler_Retry(void)
{
    g_eMMCDrv.u16_Reg10_Mode ^= BIT_SD_DATA_SYNC;
    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC retry by DATA_SYNC: %Xh, ori:%Xh\n",
               g_eMMCDrv.u16_Reg10_Mode&BIT_SD_DATA_SYNC, REG_FCIE(FCIE_SD_MODE));

    eMMC_FCIE_Init();
    if(0 == (g_eMMCDrv.u32_Flag & FCIE_FLAG_DDR_MODE))
    {
        eMMC_clock_setting(FCIE_SLOW_CLK);
    }
    else
    {
        //eMMC_FCIE_ApplyDDRTSet(g_eMMCDrv.DDRTable.u8_SetCnt-1);
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC retry by DDRT_SET_MIN\n");
    }
}

#define FCIE_WAIT_RST_DONE_D1               0x5555
#define FCIE_WAIT_RST_DONE_D2               0xAAAA
#define FCIE_WAIT_RST_DONE_CNT              1 // 3

void eMMC_FCIE_CheckResetDone(void)
{
    volatile U16 au16_tmp[FCIE_WAIT_RST_DONE_CNT], u16_i;

    while(1){
        for(u16_i=0; u16_i<FCIE_WAIT_RST_DONE_CNT; u16_i++)
        {
            REG_FCIE_W(FCIE_CIFC_ADDR(u16_i), FCIE_WAIT_RST_DONE_D1);
            REG_FCIE_R(FCIE_CIFC_ADDR(u16_i), au16_tmp[u16_i]);
        }
        for(u16_i=0; u16_i<FCIE_WAIT_RST_DONE_CNT; u16_i++)
            if(FCIE_WAIT_RST_DONE_D1 != au16_tmp[u16_i])
                break;
        if(FCIE_WAIT_RST_DONE_CNT == u16_i)
            break;
    }

    while(1){
        for(u16_i=0; u16_i<FCIE_WAIT_RST_DONE_CNT; u16_i++)
        {
            REG_FCIE_W(FCIE_CIFC_ADDR(u16_i), FCIE_WAIT_RST_DONE_D2);
            REG_FCIE_R(FCIE_CIFC_ADDR(u16_i), au16_tmp[u16_i]);
        }
        for(u16_i=0; u16_i<FCIE_WAIT_RST_DONE_CNT; u16_i++)
            if(FCIE_WAIT_RST_DONE_D2 != au16_tmp[u16_i])
                break;
        if(FCIE_WAIT_RST_DONE_CNT == u16_i)
            break;
    }
}

U32 eMMC_FCIE_Reset(void)
{
    U16 au16_tmp[FCIE_WAIT_RST_DONE_CNT];
    U32 u32_i, u32_j, u32_err = eMMC_ST_SUCCESS;
    U16 u16_clk = g_eMMCDrv.u16_ClkRegVal;
    U16 u16_temp;

    REG_FCIE_R(FCIE_CIFC_ADDR(0), u16_temp);

    eMMC_clock_setting(gau8_FCIEClkSel[0]); // speed up FCIE reset done
    REG_FCIE_W(FCIE_PATH_CTRL, BIT_SD_EN);

    // miu request reset - set
    REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

    // FCIE reset - prepare
    for(u32_i=0; u32_i<TIME_WAIT_FCIE_RESET; u32_i++)
    {
        for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
		    REG_FCIE_W(FCIE_CIFC_ADDR(u32_j), FCIE_WAIT_RST_DONE_D1+u32_j);

        for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
            REG_FCIE_R(FCIE_CIFC_ADDR(u32_j), au16_tmp[u32_j]);

        for(u32_j=0; u32_j<FCIE_WAIT_RST_DONE_CNT; u32_j++)
            if(FCIE_WAIT_RST_DONE_D1+u32_j != au16_tmp[u32_j])
                break;

        if(FCIE_WAIT_RST_DONE_CNT == u32_j)
            break;
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
    }
    if(TIME_WAIT_FCIE_RESET == u32_i)
    {
        u32_err = eMMC_ST_ERR_FCIE_NO_RIU;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: preset CIFC fail: %Xh \n", u32_err);
        eMMC_FCIE_ErrHandler_Stop();
    }

    // FCIE reset - set
    REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_FCIE_SOFT_RST_n); /* active low */
    // FCIE reset - wait
    for(u32_i=0; u32_i<TIME_WAIT_FCIE_RESET; u32_i++)
    {
        REG_FCIE_R(FCIE_CIFC_ADDR(0), au16_tmp[0]);
        REG_FCIE_R(FCIE_CIFC_ADDR(1), au16_tmp[1]);
        REG_FCIE_R(FCIE_CIFC_ADDR(2), au16_tmp[2]);
        if(0==au16_tmp[0] && 0==au16_tmp[1] && 0==au16_tmp[2])
            break;
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
    }
    if(TIME_WAIT_FCIE_RESET == u32_i){
        u32_err = eMMC_ST_ERR_FCIE_NO_CLK;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: reset CIFC fail: %Xh \n", u32_err);
        eMMC_FCIE_ErrHandler_Stop();
    }

    // FCIE reset - clear
    REG_FCIE_SETBIT(FCIE_TEST_MODE, BIT_FCIE_SOFT_RST_n);
    // FCIE reset - check
    eMMC_FCIE_CheckResetDone();
    // miu request reset - clear
    REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

    REG_FCIE_W(FCIE_CIFC_ADDR(0), u16_temp);

    eMMC_clock_setting(u16_clk);
    return u32_err;
}

U32 eMMC_FCIE_Init(void)
{
    U32 u32_err;

    eMMC_PlatformResetPre();

    // ------------------------------------------
    #if eMMC_TEST_IN_DESIGN
    {
        volatile U16 u16_i, u16_reg;
        // check timer clock
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Timer test, for 6 sec: ");
        for(u16_i = 6; u16_i > 0; u16_i--)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "%u ", u16_i);
            eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n");

        // check FCIE reg.30h
        REG_FCIE_R(FCIE_TEST_MODE, u16_reg);
        if(0)//u16_reg & BIT_FCIE_BIST_FAIL) /* Andersen: "don't care." */
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: Reg0x30h BIST fail: %04Xh \r\n", u16_reg);
            return eMMC_ST_ERR_BIST_FAIL;
        }
        if(u16_reg & BIT_FCIE_DEBUG_MODE_MASK)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: Reg0x30h Debug Mode: %04Xh \r\n", u16_reg);
            return eMMC_ST_ERR_DEBUG_MODE;
        }

        u32_err = eMMC_FCIE_Reset();
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: reset fail\n");
            eMMC_FCIE_ErrHandler_Stop();
            return u32_err;
        }
    }
    #endif // eMMC_TEST_IN_DESIGN

    // ------------------------------------------
    u32_err = eMMC_FCIE_Reset();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: reset fail: %Xh\n", u32_err);
        eMMC_FCIE_ErrHandler_Stop();
        return u32_err;
    }

    REG_FCIE_W(FCIE_MIE_INT_EN, 0);
    REG_FCIE_W(FCIE_MMA_PRI_REG, BIT_MIU_CLK_FREE_RUN);
    REG_FCIE_W(FCIE_PATH_CTRL, BIT_SD_EN);
    // all cmd are 5 bytes (excluding CRC)
    REG_FCIE_W(FCIE_CMD_SIZE, eMMC_CMD_BYTE_CNT);
    REG_FCIE_W(FCIE_SD_CTRL, 0);
    REG_FCIE_W(FCIE_SD_MODE, 0);
    #if FICE_BYTE_MODE_ENABLE
    // default sector size: 0x200
    REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | eMMC_SECTOR_512BYTE);
    #else
    REG_FCIE_W(FCIE_SDIO_CTRL, 0);
    #endif
    //REG_FCIE_W(FCIE_BOOT_CONFIG, 0); // set by eMMC_pads_switch
    //REG_FCIE_W(FCIE_PWR_SAVE_MODE, 0);
    eMMC_FCIE_ClearEvents();
    eMMC_PlatformResetPost();

    return eMMC_ST_SUCCESS;	// ok
}

void eMMC_FCIE_ClearEvents(void)
{
    volatile U16 u16_reg;
    REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
    while(1)
    {
        REG_FCIE_W(FCIE_MIE_EVENT, BIT_ALL_CARD_INT_EVENTS);
        REG_FCIE_R(FCIE_MIE_EVENT, u16_reg);
        if(0==(u16_reg&BIT_ALL_CARD_INT_EVENTS))
            break;
    }
    REG_FCIE_W1C(FCIE_SD_STATUS, BIT_SD_FCIE_ERR_FLAGS); // W1C
    REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);
}

#define FIFO_CLK_RDY_CHECK_CNT              3
U32 eMMC_FCIE_FifoClkRdy(U8 u8_Dir)
{
    volatile U32 u32_cnt=0;
    volatile U16 au16_read[FIFO_CLK_RDY_CHECK_CNT], u16_i;

    for(u32_cnt=0; u32_cnt < TIME_WAIT_FIFOCLK_RDY; u32_cnt++)
    {
        REG_FCIE_R(FCIE_MMA_PRI_REG, au16_read[0]);
        if(u8_Dir == (au16_read[0]&BIT_DMA_DIR_W))
            break;
    }
    if(TIME_WAIT_FIFOCLK_RDY == u32_cnt)
        return eMMC_ST_ERR_TIMEOUT_FIFOCLKRDY;

    for(u32_cnt=0; u32_cnt < TIME_WAIT_FIFOCLK_RDY; u32_cnt++)
    {
        for(u16_i=0; u16_i<FIFO_CLK_RDY_CHECK_CNT; u16_i++)
        {
            REG_FCIE_R(FCIE_MMA_PRI_REG, au16_read[u16_i]);
            eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        }
        for(u16_i=0; u16_i<FIFO_CLK_RDY_CHECK_CNT; u16_i++)
            if(0 == (au16_read[u16_i] & BIT_FIFO_CLKRDY))
                break;
        if(FIFO_CLK_RDY_CHECK_CNT == u16_i)
            return eMMC_ST_SUCCESS;
    }

    if(TIME_WAIT_FIFOCLK_RDY == u32_cnt)
        return eMMC_ST_ERR_TIMEOUT_FIFOCLKRDY;

    return eMMC_ST_SUCCESS;
}

//U8 HalFcie_CmdRspBufGet(U8 u8addr)
U8 eMMC_FCIE_CmdRspBufGet(U8 u8Addr)
{
    U16 u16Reg;

    u16Reg = REG_FCIE(FCIE1_BASE+(U32)((u8Addr>>1)<<2));

    if(u8Addr&1)
    {
        return (U8)(u16Reg>>8);
    }
    else
    {
        return (U8)(u16Reg&0xFF);
    }
}

//void HalFcie_CmdRspBufSet(U8 u8addr, U8 u8value)
void eMMC_FCIE_CmdRspBufSet(U8 u8Addr, U16 u16Value)
{
    U16 u16Tmp;

    //FCIE_RIU_W16(MISC_TIMING, R_CIFC_RD_REQ);

    //pu32FcieCmdRsp[u8addr>>1] = 0x00;

    u16Tmp = REG_FCIE(FCIE1_BASE+(U32)((u8Addr>>1)<<2));; // read modify write

    //FCIE_RIU_W16(MISC_TIMING, 0);

    if(u8Addr & 1)
    {
        u16Tmp = (U16)((u16Value<<8) | (u16Tmp&0xFF));
    }
    else
    {
        u16Tmp = (U16)((u16Tmp&0xFF00) | u16Value);
    }

    REG_FCIE_W(FCIE1_BASE+(U32)((u8Addr>>1)<<2), u16Tmp);
    //pu32FcieCmdRsp[u8addr>>1] = u16Tmp;

    //_pu8CMD_RSP_BUF[u8addr] = u8value;
}

void eMMC_FCIE_GetCIFC(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
    U16 u16_i;

    for(u16_i=0; u16_i<u16_WordCnt; u16_i++)
        REG_FCIE_R(FCIE_CIFC_ADDR(u16_i), pu16_Buf[u16_i]);
}

void eMMC_FCIE_GetCIFD(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
    U16 u16_i;

    for(u16_i=0; u16_i<u16_WordCnt; u16_i++)
        REG_FCIE_R(FCIE_CIFD_ADDR(u16_i), pu16_Buf[u16_i]);
}

#endif

