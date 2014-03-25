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

#ifndef MSTAR_MCI_H
#define MSTAR_MCI_H

#include "eMMC.h"

/******************************************************************************
* Function define for this driver
******************************************************************************/

/******************************************************************************
* Register Address Base
******************************************************************************/
#define CLK_400KHz              400*1000
#define CLK_200MHz              200*1000*1000

#define eMMC_GENERIC_WAIT_TIME  (HW_TIMER_DELAY_1s*3) // 3 sec

/******************************************************************************
* Low level type for this driver
******************************************************************************/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
struct mstar_mci_host_next
{
    unsigned int                dma_len;
    s32                         mstar_cookie;
};
#endif

struct mstar_mci_host
{
    struct mmc_host             *mmc;
    struct mmc_command          *cmd;
    struct mmc_request          *request;

    U32						    sd_mod;

    #ifdef CONFIG_DEBUG_FS
    struct dentry               *debug_root;
    struct dentry               *debug_perf;
    #endif

    #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,20)
    struct mstar_mci_host_next  next_data;
    #endif

    struct work_struct          async_work;
};  /* struct mstar_mci_host*/

#endif
