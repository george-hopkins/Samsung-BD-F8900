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

#include "mstar_mci.h"

/******************************************************************************
 * Defines
 ******************************************************************************/
#define DRIVER_NAME					"mstar_mci"


/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
static void mstar_mci_enable(struct mstar_mci_host *host);
static void mstar_mci_send_command(struct mstar_mci_host *host, struct mmc_command *cmd);
static void mstar_mci_completed_command(struct mstar_mci_host *host, struct mmc_command * cmd);
static int  mstar_mci_pre_dma_transfer(struct mstar_mci_host *host, struct mmc_data *data, struct mstar_mci_host_next *next);


/*****************************************************************************
 * Define Static Global Variables
 ******************************************************************************/
#if defined(DMA_TIME_TEST) && DMA_TIME_TEST
static u32 total_read_dma_len = 0;
static u32 total_read_dma_time = 0;
static u32 total_write_dma_len = 0;
static u32 total_write_dma_time = 0;
#endif
static struct mstar_mci_host *curhost;
//static u32 max_start_bit_time = 0;

/******************************************************************************
 * Functions
 ******************************************************************************/
static int mstar_mci_get_dma_dir(struct mmc_data *data)
{
	if (data->flags & MMC_DATA_WRITE)
		return DMA_TO_DEVICE;
	else
		return DMA_FROM_DEVICE;
}

static void mstar_mci_pre_dma_read(struct mstar_mci_host *host)
{
    /* Define Local Variables */
    struct scatterlist  *sg = 0;
    struct mmc_command  *cmd = 0;
    struct mmc_data     *data = 0;
    u32                 u32_dmalen = 0;
    dma_addr_t          dmaaddr = 0;

    cmd = host->cmd;
    data = cmd->data;

    mstar_mci_pre_dma_transfer(host, data, NULL);

    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST
    eMMC_hw_timer_start();
    #endif

    sg = &data->sg[0];
    dmaaddr = (u32)sg_dma_address(sg);
    u32_dmalen = sg_dma_len(sg);
    u32_dmalen = ((u32_dmalen&0x1FF)?1:0) + u32_dmalen/512;

    if( cmd->opcode != 8 )
        g_eMMCDrv.u32_LastBlkNo = cmd->arg + u32_dmalen;

    if( dmaaddr >= MSTAR_MIU1_BUS_BASE)
    {
        dmaaddr -= MSTAR_MIU1_BUS_BASE;
        REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
    }
    else
    {
        dmaaddr -= MSTAR_MIU0_BUS_BASE;
        REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
    }

    #if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
    if( g_eMMCDrv.u32_Flag & FCIE_FLAG_DDR_MODE )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, BITS_8_R_TOGGLE_CNT);
        REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
        REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
		eMMC_hw_timer_delay(TIME_WAIT_FCIE_RST_TOGGLE_CNT); // Brian needs 2T
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
	}
    #endif

    REG_FCIE_W(FCIE_JOB_BL_CNT, u32_dmalen);
    REG_FCIE_W(FCIE_SDIO_ADDR0,(((u32)dmaaddr) & 0xFFFF));
    REG_FCIE_W(FCIE_SDIO_ADDR1,(((u32)dmaaddr) >> 16));
    REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
    eMMC_FCIE_FifoClkRdy(0);
    REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

}

static void mstar_mci_post_dma_read(struct mstar_mci_host *host)
{
    /* Define Local Variables */
    struct mmc_command	*cmd = 0;
    struct mmc_data		*data = 0;
    struct scatterlist	*sg = 0;
    U32                 i;
    u32                 u32_dmalen = 0;
    dma_addr_t          dmaaddr = 0;

    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST
    u32                 u32Ticks = 0;
    #endif

    cmd = host->cmd;
    data = cmd->data;

    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
    #endif

    if(eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
                            BIT_MMA_DATA_END | BIT_SD_DATA_END | BIT_CARD_DMA_END | BIT_MIU_LAST_DONE,
                            eMMC_GENERIC_WAIT_TIME) == eMMC_GENERIC_WAIT_TIME)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: r timeout \n");
        data->error = -ETIMEDOUT;
        goto POST_DMA_READ_END;
    }

    sg = &(data->sg[0]);
    data->bytes_xfered += sg->length;

    for(i=1; i<data->sg_len; i++)
    {
        sg = &(data->sg[i]);
        dmaaddr = sg_dma_address(sg);
        u32_dmalen = sg_dma_len(sg);

        g_eMMCDrv.u32_LastBlkNo += (u32_dmalen/512);

        if( dmaaddr >= MSTAR_MIU1_BUS_BASE)
        {
            dmaaddr -= MSTAR_MIU1_BUS_BASE;
            REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
        }
        else
        {
            dmaaddr -= MSTAR_MIU0_BUS_BASE;
            REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
        }

        REG_FCIE_W(FCIE_JOB_BL_CNT,(u32_dmalen/512));
        REG_FCIE_W(FCIE_SDIO_ADDR0,(((u32)dmaaddr) & 0xFFFF));
        REG_FCIE_W(FCIE_SDIO_ADDR1,(((u32)dmaaddr) >> 16));
        eMMC_FCIE_FifoClkRdy(0);

        eMMC_FCIE_ClearEvents();

        #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
        REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
        #endif

        REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);
        REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN);

        if(eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_MMA_DATA_END | BIT_SD_DATA_END | BIT_CARD_DMA_END | BIT_MIU_LAST_DONE,
                                eMMC_GENERIC_WAIT_TIME) == eMMC_GENERIC_WAIT_TIME)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: r timeout \n");
			data->error = -ETIMEDOUT;
			break;
        }

        data->bytes_xfered += sg->length;
    }

    POST_DMA_READ_END:

    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST
    u32Ticks = eMMC_hw_timer_tick();

    total_read_dma_len += data->bytes_xfered;
    total_read_dma_time += u32Ticks;
    #endif

    if(!data->host_cookie)
    {
        dma_unmap_sg(mmc_dev(host->mmc), data->sg, (int)data->sg_len, mstar_mci_get_dma_dir(data));
    }

    mstar_mci_completed_command(host, host->request->cmd); // copy back rsp for cmd with data

    if( host->request->stop )
    {
        mstar_mci_send_command(host, host->request->stop);
    }
    else
    {
        mmc_request_done(host->mmc, host->request);
    }
}

static void mstar_mci_dma_write(struct mstar_mci_host *host)
{
    struct mmc_command	*cmd = 0;
    struct mmc_data		*data = 0;
    struct scatterlist	*sg = 0;
    U32                 i;
    u32                 u32_dmalen = 0;
    dma_addr_t          dmaaddr = 0;

    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST
    u32                 u32Ticks = 0;
    #endif

    #if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
    if( g_eMMCDrv.u32_Flag & FCIE_FLAG_DDR_MODE )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, BITS_8_W_TOGGLE_CNT);
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
	}
    #endif

    cmd = host->cmd;
    data = cmd->data;

    mstar_mci_pre_dma_transfer(host, data, NULL);

    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST
    eMMC_hw_timer_start();
    #endif

    for(i=0; i<data->sg_len; i++)
    {
        sg = &(data->sg[i]);

        dmaaddr = sg_dma_address(sg);
        u32_dmalen = sg_dma_len(sg);

        if( dmaaddr >= MSTAR_MIU1_BUS_BASE)
        {
            dmaaddr -= MSTAR_MIU1_BUS_BASE;
            REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
        }
        else
        {
            dmaaddr -= MSTAR_MIU0_BUS_BASE;
            REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
        }

        REG_FCIE_W(FCIE_JOB_BL_CNT, (u32_dmalen / 512));
        REG_FCIE_W(FCIE_SDIO_ADDR0, (dmaaddr & 0xFFFF));
        REG_FCIE_W(FCIE_SDIO_ADDR1, (dmaaddr >> 16));
        REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
        eMMC_FCIE_FifoClkRdy(BIT_DMA_DIR_W);

        eMMC_FCIE_ClearEvents();

        #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
        REG_FCIE_SETBIT(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);
        #endif

        REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);
        REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);

        if(eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_MMA_DATA_END | BIT_SD_DATA_END | BIT_CARD_DMA_END | BIT_MIU_LAST_DONE,
                                eMMC_GENERIC_WAIT_TIME) == eMMC_GENERIC_WAIT_TIME)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: w timeout \n");
			data->error = -ETIMEDOUT;
        }

        #if defined(eMMC_DRV_X12_LINUX) && eMMC_DRV_X12_LINUX
        eMMC_FCIE_WaitD0High();
        #endif

        data->bytes_xfered += sg->length;
    }

    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST
    u32Ticks = eMMC_hw_timer_tick();

    total_write_dma_len += data->bytes_xfered;
    total_write_dma_time += u32Ticks;
    #endif

    if(!data->host_cookie)
    {
        dma_unmap_sg(mmc_dev(host->mmc), data->sg, (int)data->sg_len, mstar_mci_get_dma_dir(data));
    }

    mstar_mci_completed_command(host, host->request->cmd); // copy back rsp for cmd with data

    if( host->request->stop )
    {
        mstar_mci_send_command(host, host->request->stop);
    }
    else
    {
        mmc_request_done(host->mmc, host->request);
    }
}

static void mstar_mci_completed_command(struct mstar_mci_host *host, struct mmc_command * cmd)
{
    /* Define Local Variables */
    u16 u16_st, u16_i;
    u8* pTemp;
    //struct mmc_command *cmd = host->cmd;

    if( cmd->flags == MMC_RSP_R1B )
        eMMC_FCIE_WaitD0High();

    // retrun response from FCIE to mmc driver
    pTemp = (u8*)&(cmd->resp[0]);
    for(u16_i=0; u16_i < 15; u16_i++)
    {
        pTemp[(3 - (u16_i % 4)) + (4 * (u16_i / 4))] =
            (u8)(REG_FCIE(FCIE1_BASE+(((u16_i+1U)/2)*4)) >> (8*((u16_i+1)%2)));
    }

    /*eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 0, "RSP: %02X %02X %02X %02X %02X (%d)\n", \
        eMMC_FCIE_CmdRspBufGet(0), eMMC_FCIE_CmdRspBufGet(1), eMMC_FCIE_CmdRspBufGet(2), eMMC_FCIE_CmdRspBufGet(3), eMMC_FCIE_CmdRspBufGet(4), (eMMC_FCIE_CmdRspBufGet(3)&0x1E)>>1);*/

    u16_st = REG_FCIE(FCIE_SD_STATUS);
    if(u16_st & BIT_SD_FCIE_ERR_FLAGS)
    {
        if((u16_st & BIT_SD_RSP_CRC_ERR) && !(mmc_resp_type(cmd) & MMC_RSP_CRC))
        {
            cmd->error = 0;
        }
        else
        {   // [FIXME] do more: log & handling
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC: ST:%Xh, CMD:%u, retry:%u \n",
                       u16_st, cmd->opcode, cmd->retries);

            if(u16_st & (BIT_SD_RSP_TIMEOUT | BIT_SD_W_FAIL))
            {
                cmd->error = -ETIMEDOUT;
            }
            else if(u16_st & (BIT_SD_RSP_CRC_ERR | BIT_SD_R_CRC_ERR | BIT_SD_W_CRC_ERR))
            {
                cmd->error = -EILSEQ;
                eMMC_FCIE_ErrHandler_Stop();
            }
            else
            {
                cmd->error = -EIO;
            }
        }
    }
    else
    {
        cmd->error = 0;
    }

    REG_FCIE_W(FCIE_SD_STATUS, BIT_SD_FCIE_ERR_FLAGS);

}

static void mstar_mci_send_data(struct work_struct *work)
{
    struct mstar_mci_host *host = curhost;
    struct mmc_command *cmd;
    struct mmc_data *data;

    if( !host )
        return;

    cmd = host->cmd;

    if( !cmd )
        return;

    data = cmd->data;

    if( !data )
        return;

    if(data->flags & MMC_DATA_WRITE)
    {
        mstar_mci_dma_write(host);
    }
    else if(data->flags & MMC_DATA_READ)
    {
        mstar_mci_post_dma_read(host);
    }
}

void mstar_mci_send_command(struct mstar_mci_host *host, struct mmc_command *cmd)
{
    struct mmc_data *data;
    u32 u32_mie_int=0, u32_sd_ctl=0, u32_sd_mode;

    u32_sd_mode = host->sd_mod;

    eMMC_FCIE_ClearEvents();

    host->cmd = cmd;
    data = cmd->data;

    if(data)
    {
        data->bytes_xfered = 0;

        if (data->flags & MMC_DATA_READ)
        {
            u32_sd_ctl |= BIT_SD_DAT_EN;

            // Enable stoping read clock when using scatter list DMA
            if((data->sg_len > 1) && (cmd->opcode == 18))
                u32_sd_mode |= BIT_SD_DMA_R_CLK_STOP;
            else
                u32_sd_mode &= ~BIT_SD_DMA_R_CLK_STOP;

            mstar_mci_pre_dma_read(host);
        }
    }

    u32_sd_ctl |= BIT_SD_CMD_EN;
    u32_mie_int |= BIT_SD_CMD_END;

    //eMMC_GPIO60_Debug(0);
    //eMMC_printf("CMD%02d_%08Xh_%08Xh\n", cmd->opcode, cmd->arg, cmd->flags);

    REG_FCIE_W(FCIE_CIFC_ADDR(0), (((cmd->arg >> 24)<<8) | (0x40|cmd->opcode)));
    REG_FCIE_W(FCIE_CIFC_ADDR(1), ((cmd->arg & 0xFF00) | ((cmd->arg>>16)&0xFF)));
    REG_FCIE_W(FCIE_CIFC_ADDR(2), (cmd->arg & 0xFF));

    if(mmc_resp_type(cmd) == MMC_RSP_NONE)
    {
        u32_sd_ctl &= ~BIT_SD_RSP_EN;
        REG_FCIE_W(FCIE_RSP_SIZE, 0);
    }
    else
    {
        u32_sd_ctl |= BIT_SD_RSP_EN;
        if(mmc_resp_type(cmd) == MMC_RSP_R2)
        {
            u32_sd_ctl |= BIT_SD_RSPR2_EN;
            REG_FCIE_W(FCIE_RSP_SIZE, 16); /* (136-8)/8 */
        }
        else
        {
            REG_FCIE_W(FCIE_RSP_SIZE, 5); /*(48-8)/8 */
        }
    }

    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    eMMC_debug(eMMC_DEBUG_LEVEL_LOW,1,"eMMC: INT_EN: %Xh\n", u32_mie_int);
    REG_FCIE_W(FCIE_MIE_INT_EN, u32_mie_int);
    #endif

    REG_FCIE_W(FCIE_SD_MODE, u32_sd_mode);
    REG_FCIE_W(FCIE_SD_CTRL, u32_sd_ctl);

    // [FIXME]: retry and timing, and omre...
    if(eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_SD_CMD_END,
                            eMMC_GENERIC_WAIT_TIME) == eMMC_GENERIC_WAIT_TIME)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: cmd timeout \n");
		cmd->error = -ETIMEDOUT;
    }
#if 0
    printk("CIFC: %08Xh %08Xh %08Xh %08Xh\n", REG_FCIE(FCIE1_BASE), REG_FCIE(FCIE1_BASE+4), REG_FCIE(FCIE1_BASE+8), REG_FCIE(FCIE1_BASE+12));
    printk("RSP: %02X %02X %02X %02X %02X\n", eMMC_FCIE_CmdRspBufGet(0), eMMC_FCIE_CmdRspBufGet(1), eMMC_FCIE_CmdRspBufGet(2), eMMC_FCIE_CmdRspBufGet(3), eMMC_FCIE_CmdRspBufGet(4));
    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 0, "RSP: %02X %02X %02X %02X %02X (%d)\n", \
        eMMC_FCIE_CmdRspBufGet(0), eMMC_FCIE_CmdRspBufGet(1), eMMC_FCIE_CmdRspBufGet(2), eMMC_FCIE_CmdRspBufGet(3), eMMC_FCIE_CmdRspBufGet(4), (eMMC_FCIE_CmdRspBufGet(3)&0x1E)>>1);
#endif
    if( data )
    {
        schedule_work(&host->async_work);
    }
    else
    {
        mstar_mci_completed_command(host, cmd); // copy back rsp for cmd without data
        mmc_request_done(host->mmc, host->request);
    }
}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
static int mstar_mci_pre_dma_transfer(struct mstar_mci_host *host, struct mmc_data *data, struct mstar_mci_host_next *next)
{
    U32 dma_len;

    /*if(!next && data->host_cookie &&
        data->host_cookie != host->next_data.mstar_cookie) {
        pr_warning("[%s] invalid cookie: data->host_cookie %d"
                   " host->next_data.mstar_cookie %d\n",
                   __func__, data->host_cookie, host->next_data.mstar_cookie);
        data->host_cookie = 0;
    }*/

    /*if(!next && data->host_cookie != host->next_data.mstar_cookie)
    {
        printk("no cookie map, cmd->opcode = %d\n", host->request->cmd->opcode);
        printk("host_cookie = %d, mstar_cookie = %d\n", data->host_cookie, host->next_data.mstar_cookie);
    }*/

    if( next || (!next && data->host_cookie != host->next_data.mstar_cookie) )
    {
        dma_len = (U32)dma_map_sg(mmc_dev(host->mmc), data->sg, (int)data->sg_len, mstar_mci_get_dma_dir(data));
    }
    else
    {
        dma_len = host->next_data.dma_len;
        host->next_data.dma_len = 0;
    }

    if (dma_len == 0)
        return -EINVAL;

    if(next)
    {
        next->dma_len = dma_len;
        data->host_cookie = ++next->mstar_cookie < 0 ? 1 : next->mstar_cookie;
    }
    // else
	//	host->dma_len = dma_len;

    return 0;
}

static void mstar_mci_pre_req(struct mmc_host *mmc, struct mmc_request *mrq, bool is_first_req)
{
    struct mstar_mci_host *host = mmc_priv(mmc);

    eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 1, "\n");

    if( mrq->data->host_cookie )
    {
        mrq->data->host_cookie = 0;
        return;
    }

    if (mstar_mci_pre_dma_transfer(host, mrq->data, &host->next_data))
        mrq->data->host_cookie = 0;
}

static void mstar_mci_post_req(struct mmc_host *mmc, struct mmc_request *mrq, int err)
{
    struct mstar_mci_host *host = mmc_priv(mmc);
    struct mmc_data *data = mrq->data;

    eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 1, "\n");

    if(data->host_cookie)
    {
        dma_unmap_sg(mmc_dev(host->mmc), data->sg, (int)data->sg_len, mstar_mci_get_dma_dir(data));
    }

    data->host_cookie = 0;
}

static int mstar_mci_abort_req(struct mmc_host *mmc, struct mmc_request *mrq)
{
    struct mstar_mci_host *host = mmc_priv(mmc);
    struct mmc_command *cmd;
    struct mmc_data *data;

    eMMC_debug(eMMC_DEBUG_LEVEL_LOW, 1, "\n");

    if( !host )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "host is null\n");
        return -EINVAL;
    }

    cmd = host->cmd;

    if( !cmd )
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "cmd is null\n");
        return -EINVAL;
    }

    data = cmd->data;

    if( mrq && (mrq != host->request) )
    {
        dev_dbg(mmc_dev(host->mmc), "Non matching abort request\n");
        return -EINVAL;
    }

    if( data )
    {
        // 1. diable MMA_EN
        if( REG_FCIE(FCIE_PATH_CTRL) & BIT_MMA_EN )
            REG_FCIE_CLRBIT(FCIE_PATH_CTRL, BIT_MMA_EN);
        else
            goto finish_abort;

        // 2. Wait MMA_END
        if(eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_MMA_DATA_END,
			eMMC_GENERIC_WAIT_TIME) == eMMC_GENERIC_WAIT_TIME)
        {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: wait abort DMA timeout \n");
				data->error = -ETIMEDOUT;
        }

        // 3. Reset FCIE
        mstar_mci_enable(host);
    }

    finish_abort:
    mmc_request_done(host->mmc, host->request);

    return 0;

}

static void mstar_mci_reset(struct mmc_host *mmc)
{
    struct mstar_mci_host *host = mmc_priv(mmc);

    mstar_mci_enable(host);
}
#endif

static void mstar_mci_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
    struct mstar_mci_host *host;

    eMMC_LockFCIE();

    host = mmc_priv(mmc);
    if (!mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mmc is NULL \n");
        goto LABEL_END;
    }
    if (!mrq)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mrq is NULL \n");
        goto LABEL_END;
    }

    host->request = mrq;

    mstar_mci_send_command(host, host->request->cmd);

    LABEL_END:
    eMMC_UnlockFCIE();

}

static void mstar_mci_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
    /* Define Local Variables */
    struct mstar_mci_host *host = mmc_priv(mmc);

    if (!mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mmc is NULL \n");
        goto LABEL_END;
    }

    if (!ios)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: ios is NULL \n");
        goto LABEL_END;
    }

    eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0, "eMMC: clock: %u, bus_width %Xh \n",
               ios->clock, ios->bus_width);

    // ----------------------------------
    if (ios->clock == 0)
    {
        host->sd_mod = 0;
        eMMC_debug(eMMC_DEBUG_LEVEL_MEDIUM, 0, "eMMC Warn: disable clk \n");
        eMMC_clock_gating();
    }
    else
    {
        host->sd_mod = BIT_SD_DEFAULT_MODE_REG;

        if(ios->clock > CLK_400KHz)
        {
            eMMC_clock_setting(FCIE_DEFAULT_CLK);
        }
        else
        {
            eMMC_clock_setting(FCIE_SLOWEST_CLK);
        }
    }

    // ----------------------------------
    if (ios->bus_width == MMC_BUS_WIDTH_8)
    {
        host->sd_mod = (host->sd_mod & ~BIT_SD_DATA_WIDTH_MASK) | BIT_SD_DATA_WIDTH_8;
    }
    else if (ios->bus_width == MMC_BUS_WIDTH_4)
    {
        host->sd_mod = (host->sd_mod & ~BIT_SD_DATA_WIDTH_MASK) | BIT_SD_DATA_WIDTH_4;
    }
    else
    {
        host->sd_mod = (host->sd_mod & ~BIT_SD_DATA_WIDTH_MASK);
    }

    #if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
    if( host->mmc->ios.timing == MMC_TIMING_UHS_DDR50 )
    {
        eMMC_pads_switch(FCIE_eMMC_DDR);
        #if defined(MSTAR_DEMO_BOARD) && MSTAR_DEMO_BOARD
        REG_FCIE_W(FCIE_SM_STS, BIT_DQS_MODE_3_5T);
        #elif defined(SEC_X12_BOARD) && SEC_X12_BOARD
        REG_FCIE_W(FCIE_SM_STS, BIT_DQS_MODE_2_5T);
        #endif
        g_eMMCDrv.u32_Flag |= FCIE_FLAG_DDR_MODE;
        host->sd_mod &= ~BIT_SD_DATA_SYNC;
    }
    #endif

    LABEL_END:
    ;
}

static s32 mstar_mci_get_ro(struct mmc_host *mmc)
{
    s32 read_only = 0;

    if(!mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mmc is NULL \n");
        read_only = -EINVAL;
    }

    return read_only;
}

/*int mstar_mci_check_D0_status(void)
{
    u16 u16Reg = 0;

    u16Reg = REG_FCIE(FCIE_SD_STATUS);

    if(u16Reg & BIT_SD_D0 )
        return 1;
    else
        return 0;
}
EXPORT_SYMBOL(mstar_mci_check_D0_status);*/

static void mstar_mci_enable(struct mstar_mci_host *host)
{
    u32 u32_err;

    u32_err = eMMC_FCIE_Init();

    if(eMMC_ST_SUCCESS != u32_err)
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1, "eMMC Err: eMMC_FCIE_Init fail: %Xh \n", u32_err);
}

static void mstar_mci_disable(void)
{
    u32 u32_err;

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"\n");

    eMMC_clock_setting(FCIE_DEFAULT_CLK); // enable clk

    u32_err = eMMC_FCIE_Reset();
    if(eMMC_ST_SUCCESS != u32_err)
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1, "eMMC Err: eMMC_FCIE_Reset fail: %Xh\n", u32_err);

    eMMC_clock_gating();
}

#ifdef CONFIG_DEBUG_FS

#if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
static int mstar_mci_perf_show(struct seq_file *seq, void *v)
{
    #if defined(DMA_TIME_TEST) && DMA_TIME_TEST

    seq_printf(seq, "Total Read Length: 0x%08X\n", total_read_dma_len);
    seq_printf(seq, "Total Ticks of Read: 0x%08X\n", total_read_dma_time);

    seq_printf(seq, "Total Write Length: 0x%08X\n", total_write_dma_len);
    seq_printf(seq, "Total Tick of Write: 0x%08X\n", total_write_dma_time);

    #endif

    return 0;
}

static int mstar_mci_perf_open(struct inode *inode, struct file *file)
{
	return single_open(file, mstar_mci_perf_show, inode->i_private);
}

static const struct file_operations mstar_mci_fops_perf = {
	.owner		= THIS_MODULE,
	.open		= mstar_mci_perf_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif

static void mstar_mci_debugfs_attach(struct mstar_mci_host *host)
{
    struct device *dev = mmc_dev(host->mmc);

    host->debug_root = debugfs_create_dir(dev_name(dev), NULL);

	if (IS_ERR(host->debug_root)) {
		dev_err(dev, "failed to create debugfs root\n");
		return;
	}

    #if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
    host->debug_perf = debugfs_create_file("FCIE_PERF", 0444,
					       host->debug_root, host,
					       &mstar_mci_fops_perf);

	if (IS_ERR(host->debug_perf))
		dev_err(dev, "failed to create debug regs file\n");
    #endif
}

static void mstar_mci_debugfs_remove(struct mstar_mci_host *host)
{
    #if defined(MMC_SPEED_TEST) && MMC_SPEED_TEST
    debugfs_remove(host->debug_perf);
    #endif
	debugfs_remove(host->debug_root);
}

#else

static inline void mstar_mci_debugfs_attach(struct mstar_mci_host *host) { }
static inline void mstar_mci_debugfs_remove(struct mstar_mci_host *host) { }

#endif

/* MSTAR Multimedia Card Interface Operations */
static const struct mmc_host_ops mstar_mci_ops =
{
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
    .pre_req =  mstar_mci_pre_req,
    .post_req = mstar_mci_post_req,
    .abort_req = mstar_mci_abort_req,
    .hw_reset = mstar_mci_reset,
    #endif
    .request =	mstar_mci_request,
    .set_ios =	mstar_mci_set_ios,
    .get_ro =	mstar_mci_get_ro,
};

static s32 mstar_mci_probe(struct platform_device *dev)
{
    /* Define Local Variables */
    struct mmc_host *mmc = 0;
    struct mstar_mci_host *host = 0;
    s32 s32_ret = 0;

    eMMC_LockFCIE();

    //eMMC_debug(eMMC_DEBUG_LEVEL_WARNING, 0, "\33[1;31meMMC testload built at %s on %s\33[m\n", __TIME__, __DATE__);

    // --------------------------------
    eMMC_PlatformInit();

    // --------------------------------
    if (!dev)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: dev is NULL \n");
        s32_ret = -EINVAL;
        goto LABEL_END;
    }

    mmc = mmc_alloc_host(sizeof(struct mstar_mci_host), &dev->dev);
    if (!mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: mmc_alloc_host fail \n");
        s32_ret = -ENOMEM;
        goto LABEL_END;
    }

    mmc->ops = &mstar_mci_ops;
    // [FIXME]->
    mmc->f_min              = CLK_400KHz;
    mmc->f_max              = CLK_200MHz;
    mmc->ocr_avail          = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 |
                              MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
    mmc->max_blk_count      = BIT_SD_JOB_BLK_CNT_MASK;
    mmc->max_blk_size       = 512; /* sector */
    mmc->max_req_size       = mmc->max_blk_count * mmc->max_blk_size;
    mmc->max_seg_size       = mmc->max_req_size;

    #if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,20)
    mmc->max_phys_segs      = 128;
    mmc->max_hw_segs        = 128;
    #else
    mmc->max_segs           = 1;
    #endif

    //---------------------------------------
    host                    = mmc_priv(mmc);
    curhost                 = host;
    host->mmc               = mmc;
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
    host->next_data.mstar_cookie  = 1;
    #endif

    //---------------------------------------
    mmc->caps               = MMC_CAP_8_BIT_DATA | MMC_CAP_MMC_HIGHSPEED | MMC_CAP_NONREMOVABLE | MMC_CAP_ERASE;

    #if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING
    mmc->caps               |= MMC_CAP_1_8V_DDR | MMC_CAP_UHS_DDR50;
    #endif

    INIT_WORK(&host->async_work, mstar_mci_send_data);

    //eMMC_GPIO60_init();

    mmc_add_host(mmc);

    #ifdef CONFIG_DEBUG_FS
    mstar_mci_debugfs_attach(host);
    #endif

    platform_set_drvdata(dev, mmc);

    mstar_mci_enable(host);

    #if (defined(ENABLE_eMMC_INTERRUPT_MODE)&&ENABLE_eMMC_INTERRUPT_MODE) || \
    (defined(ENABLE_FCIE_HW_BUSY_CHECK)&&ENABLE_FCIE_HW_BUSY_CHECK)
    s32_ret = request_irq(E_IRQ_NFIE, eMMC_FCIE_IRQ, 0, DRIVER_NAME, host);
    if (s32_ret)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: request_irq fail \n");
        mmc_free_host(mmc);
        goto LABEL_END;
    }
    #endif

    LABEL_END:
    eMMC_UnlockFCIE();

    return 0;
}

static s32 __exit mstar_mci_remove(struct platform_device *dev)
{
    /* Define Local Variables */
    struct mmc_host *mmc = platform_get_drvdata(dev);
    struct mstar_mci_host *host = mmc_priv(mmc);
    s32 s32_ret = 0;

    eMMC_LockFCIE();

    if (!dev)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: dev is NULL\n");
        s32_ret = -EINVAL;
        goto LABEL_END;
    }

    if (!mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: mmc is NULL\n");
        s32_ret= -1;
        goto LABEL_END;
    }

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, remove +\n");

    #ifdef CONFIG_DEBUG_FS
    mstar_mci_debugfs_remove(host);
    #endif

    mmc_remove_host(mmc);

    mstar_mci_disable();

    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    free_irq(E_IRQ_NFIE, host);
    #endif

    mmc_free_host(mmc);
    platform_set_drvdata(dev, NULL);

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, remove -\n");

    LABEL_END:
    eMMC_UnlockFCIE();

    return s32_ret;
}


#ifdef CONFIG_PM
static s32 mstar_mci_suspend(struct platform_device *dev, pm_message_t state)
{
    /* Define Local Variables */
    struct mmc_host *mmc = platform_get_drvdata(dev);
    s32 ret = 0;

    if (mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, suspend +\n");
        ret = mmc_suspend_host(mmc);
    }

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, suspend -, %Xh\n", ret);

    return ret;
}

static s32 mstar_mci_resume(struct platform_device *dev)
{
    struct mmc_host *mmc = platform_get_drvdata(dev);
    s32 ret = 0;

    if (mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, resume +\n");
        ret = mmc_resume_host(mmc);
    }

    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC, resume -, %Xh\n", ret);

    return ret;
}
#endif  /* End ifdef CONFIG_PM */

/******************************************************************************
 * Define Static Global Variables
 ******************************************************************************/
static struct platform_driver mstar_mci_driver =
{
    .probe = mstar_mci_probe,
    .remove = __exit_p(mstar_mci_remove),

    #ifdef CONFIG_PM
    .suspend = mstar_mci_suspend,
    .resume = mstar_mci_resume,
    #endif

    .driver  =
    {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    },
};

static struct platform_device mstar_mci_device =
{
    .name =	DRIVER_NAME,
    .id = 0,
    .resource =	NULL,
    .num_resources = 0,
};


/******************************************************************************
 * Init & Exit Modules
 ******************************************************************************/
static s32 __init mstar_mci_init(void)
{
    int err = 0;
    eMMC_debug(eMMC_DEBUG_LEVEL_LOW,1,"\n");

    if((err = platform_device_register(&mstar_mci_device)) < 0)
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: platform_driver_register fail, %Xh\n", err);

    if((err = platform_driver_register(&mstar_mci_driver)) < 0)
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: platform_driver_register fail, %Xh\n", err);

    return err;
}

static void __exit mstar_mci_exit(void)
{
    platform_driver_unregister(&mstar_mci_driver);
}

rootfs_initcall(mstar_mci_init);
module_exit(mstar_mci_exit);

MODULE_LICENSE("Proprietary");
MODULE_DESCRIPTION("Mstar Multimedia Card Interface driver");
MODULE_AUTHOR("Hill.Sung/Hungda.Wang");
