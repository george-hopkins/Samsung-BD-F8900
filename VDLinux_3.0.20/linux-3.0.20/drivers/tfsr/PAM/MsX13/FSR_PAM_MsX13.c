////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

/**
 *   @mainpage   Flex Sector Remapper : RFS_1.3.1_b046-LinuStoreIII_1.1.0_b016-FSR_1.1.1_b109_RC
 *
 *   @section Intro
 *       Flash Translation Layer for Flex-OneNAND and OneNAND
 *
 *    @section  Copyright
 *            COPYRIGHT. 2007 - 2008 SAMSUNG ELECTRONICS CO., LTD.
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
 * @file      FSR_PAM_MsX4.c
 * @brief     This file contain the Platform Adaptation Modules for MsX4
 * @author    Mstar
 * @date      26-MAR-2009
 * @remark
 * REVISION HISTORY
 * @n  26-MAR-2009 [Mstar] : first writing
 *
 */

#include "FSR.h"

/*****************************************************************************/
/* [PAM customization]                                                       */
/* The following 5 parameters can be customized                              */
/*                                                                           */
/* - FSR_ONENAND_PHY_BASE_ADDR                                               */
/* - FSR_ENABLE_WRITE_DMA                                                    */
/* - FSR_ENABLE_READ_DMA                                                     */
/* - FSR_ENABLE_FLEXOND_LFT                                                  */
/* - FSR_ENABLE_ONENAND_LFT                                                  */
/*                                                                           */
/*****************************************************************************/
/**< if FSR_ENABLE_FLEXOND_LFT is defined,
     Low level function table is linked with Flex-OneNAND LLD */
//#define     FSR_ENABLE_FLEXOND_LFT

/**< if FSR_ENABLE_ONENAND_LFT is defined,
     Low level function table is linked with OneNAND LLD */
#define     FSR_ENABLE_ONENAND_LFT

/**< if FSR_ENABLE_4K_ONENAND_LFT is defined,
     Low level function table is linked with OneNAND LLD */
//#define     FSR_ENABLE_4K_ONENAND_LFT

#define     FSR_ENABLE_NAND_LFT

// #if defined(FSR_LINUX_OAM)

    #if defined(TINY_FSR)
        /* Setting at Kernel configuration */
        #define     FSR_ONENAND_PHY_BASE_ADDR       CONFIG_TINY_FLASH_PHYS_ADDR // FIXME: fixed or "config TINY_FLASH_PHYS_ADDR @ drivers/tfsr/Kconfig"?
    #else
        #define     FSR_ONENAND_PHY_BASE_ADDR       CONFIG_FSR_FLASH_PHYS_ADDR  // FIXME: fixed or "config FSR_FLASH_PHYS_ADDR @ drivers/fsr/Kconfig"?
    #endif

    /**< if FSR_ENABLE_WRITE_DMA is defined, write DMA is enabled */
    #undef      FSR_ENABLE_WRITE_DMA
    /**< if FSR_ENABLE_READ_DMA is defined, read DMA is enabled */
    #undef      FSR_ENABLE_READ_DMA

// #endif // defined(FSR_LINUX_OAM)

#if defined(FSR_ENABLE_FLEXOND_LFT)
    #include "FSR_LLD_FlexOND.h"
#endif

#if defined(FSR_ENABLE_ONENAND_LFT)
    #include "FSR_LLD_OneNAND.h"
#endif

#if defined(FSR_ENABLE_4K_ONENAND_LFT)
    #include "FSR_LLD_4K_OneNAND.h"
#endif

#if defined(FSR_ENABLE_NAND_LFT)
#include "FSR_LLD_PureNAND.h"
#endif

/*****************************************************************************/
/* Global variables definitions                                              */
/*****************************************************************************/

/*****************************************************************************/
/* Local #defines                                                            */
/*****************************************************************************/
extern unsigned short MDrv_ONENAND_read_word(void __iomem *addr);
#if defined (CONFIG_MSTAR_TITANIA8) || defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_AMBER1) // TODO: Please add your platform here
#define OND_READ(nAddr) (MDrv_ONENAND_read_word((void *)&(nAddr))) // <-@@@
#else
#define OND_READ(nAddr) (nAddr        )
#endif

// cached/unchched segment
#define KSEG0_BASE	            (0x80000000)
#define KSEG1_BASE	            (0xA0000000)
#define KSEG0_SIZE	            0x20000000
#define KSEG1_SIZE	            0x20000000

// cached addr <-> unchched addr
#define KSEG02KSEG1(addr)       ((UINT32)(addr)|0x20000000)   // cached -> unchched
#define KSEG12KSEG0(addr)       ((UINT32)(addr)&~0x20000000)  // unchched -> cached

// virtual addr <-> physical addr
#define VA2PA(addr) 	        (((UINT32)addr) & 0x1FFFFFFF) // virtual -> physical
#define PA2KSEG0(addr) 	        (((UINT32)addr) | 0x80000000) // physical -> cached
#define PA2KSEG1(addr) 	        (((UINT32)addr) | 0xA0000000) // physical -> unchched

#define     ONENAND_DID_FLEX        (2)
#define     ONENAND_DID_SLC         (0)
#define     ONENAND_DID_MLC         (1)
#define     ONENAND_DIE_REG_OFFSET  (0x0001e002)
#define     FSR_FND_4K_PAGE         (0)
#define     FSR_OND_2K_PAGE         (1)
#define     FSR_OND_4K_PAGE         (2)

#define     DBG_PRINT(x)            FSR_DBG_PRINT(x)
#define     RTL_PRINT(x)            FSR_RTL_PRINT(x)


/*****************************************************************************/
/* Local typedefs                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* Static variables definitions                                              */
/*****************************************************************************/

PRIVATE FsrVolParm              gstFsrVolParm[FSR_MAX_VOLS];
PRIVATE BOOL32                  gbPAMInit                   = FALSE32;
PRIVATE BOOL32                  gbUseWriteDMA               = FALSE32;
PRIVATE BOOL32                  gbUseReadDMA                = FALSE32;
//PRIVATE UINT32                  gbFlexOneNAND[FSR_MAX_VOLS] = {FSR_OND_2K_PAGE, FSR_OND_2K_PAGE};
#if defined(FSR_ENABLE_ONENAND_LFT)
PRIVATE volatile OneNANDReg     *gpOneNANDReg               = (volatile OneNANDReg *) 0;
#elif defined(FSR_ENABLE_4K_ONENAND_LFT)
PRIVATE volatile OneNAND4kReg   *gpOneNANDReg               = (volatile OneNAND4kReg *) 0;
#elif defined(FSR_ENABLE_FLEXOND_LFT)
PRIVATE volatile FlexOneNANDReg *gpOneNANDReg               = (volatile FlexOneNANDReg *) 0;
#else
#error  Either FSR_ENABLE_FLEXOND_LFT or FSR_ENABLE_ONENAND_LFT should be defined
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern  VOID    memcpy32 (VOID       *pDst,
                          VOID       *pSrc,
                          UINT32     nSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
VOID
FSR_PAM_Memcpy(VOID *pDst, VOID *pSrc, UINT32 nLen);

VOID
FSR_PAM_Memcpy(VOID *pDst, VOID *pSrc, UINT32 nLen)
{


    memcpy32(pDst,pSrc,nLen);

}



/**
 * @brief           This function initializes PAM
 *                  this function is called by FSR_BML_Init
 *
 * @return          FSR_PAM_SUCCESS
 *
 * @author          SongHo Yoon
 * @version         1.0.0
 *
 */
PUBLIC INT32
FSR_PAM_Init(VOID)
{
    INT32   nRe = FSR_PAM_SUCCESS;
    UINT32  nONDVirBaseAddr;
    FSR_STACK_VAR;

    FSR_STACK_END;

    if (gbPAMInit == TRUE32)
    {
        return FSR_PAM_SUCCESS;
    }
    gbPAMInit     = TRUE32;

    RTL_PRINT((TEXT("[PAM:   ] ++%s\r\n"), __FSR_FUNC__));

    { // <-@@@
    	#if defined(CONFIG_MSTAR_TITANIA3)
        extern void HAL_ParFlash_Init(void);
        HAL_ParFlash_Init();
		#else
		//extern void HAL_ONIF_INIT(void);
		//HAL_ONIF_INIT();
		#endif
    } // <-@@@

    do
    {
#if defined(FSR_ENABLE_WRITE_DMA)
        gbUseWriteDMA = TRUE32;
#else
        gbUseWriteDMA = FALSE32;
#endif

#if defined(FSR_ENABLE_READ_DMA)
        gbUseReadDMA  = TRUE32;
#else
        gbUseReadDMA  = FALSE32;
#endif

        nONDVirBaseAddr  = PA2KSEG1(FSR_ONENAND_PHY_BASE_ADDR); // <-@@@

        RTL_PRINT((TEXT("[PAM:   ]   OneNAND physical base address       : 0x%08x\r\n"), FSR_ONENAND_PHY_BASE_ADDR));
        RTL_PRINT((TEXT("[PAM:   ]   OneNAND virtual  base address       : 0x%08x\r\n"), nONDVirBaseAddr));

#if defined(FSR_ENABLE_ONENAND_LFT)
        gpOneNANDReg  = (volatile OneNANDReg *) nONDVirBaseAddr;
#elif defined(FSR_ENABLE_4K_ONENAND_LFT)
        gpOneNANDReg  = (volatile OneNAND4kReg *) nONDVirBaseAddr;
#elif defined(FSR_ENABLE_FLEXOND_LFT)
        gpOneNANDReg  = (volatile FlexOneNANDReg *) nONDVirBaseAddr;
#endif

		#if 0
        /* check manufacturer ID */
        if (OND_READ(gpOneNANDReg->nMID) != 0x00ec) // <-@@@
        {
            /* ERROR : OneNAND address space is not mapped with address space of MIPS core */
            RTL_PRINT((TEXT("[PAM:ERR] OneNAND address space is not mapped with address space of MIPS core\r\n")));
            nRe = FSR_PAM_NAND_PROBE_FAILED;
            break;
        }

        /* check whether current attached OneNAND is Flex-OneNAND or OneNAND */
        if (((OND_READ(gpOneNANDReg->nDID) >> 8) & 0x03) == ONENAND_DID_FLEX) // <-@@@
        {
            gbFlexOneNAND[0] = FSR_FND_4K_PAGE;
            gbFlexOneNAND[1] = FSR_FND_4K_PAGE;

            RTL_PRINT((TEXT("[PAM:   ]   Flex-OneNAND nMID=0x%2x : nDID=0x%02x\r\n"),
                    OND_READ(gpOneNANDReg->nMID), OND_READ(gpOneNANDReg->nDID))); // <-@@@
        }
        else
        {
            gbFlexOneNAND[0] = FSR_OND_2K_PAGE;
            gbFlexOneNAND[1] = FSR_OND_2K_PAGE;

            /* FB mask for supporting Demux and Mux device. */
            if (((OND_READ(gpOneNANDReg->nDID) & 0xFB) == 0x50) || ((OND_READ(gpOneNANDReg->nDID) & 0xFB) == 0x68)) // <-@@@
            {
                gbFlexOneNAND[0] = FSR_OND_4K_PAGE;
                gbFlexOneNAND[1] = FSR_OND_4K_PAGE;
            }

            RTL_PRINT((TEXT("[PAM:   ]   OneNAND nMID=0x%2x : nDID=0x%02x\r\n"),
                    OND_READ(gpOneNANDReg->nMID), OND_READ(gpOneNANDReg->nDID))); // <-@@@
        }
		#endif

        gstFsrVolParm[0].nBaseAddr[0] = nONDVirBaseAddr;
        gstFsrVolParm[0].nBaseAddr[1] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[0].nIntID[0]    = FSR_INT_ID_NAND_0;
        gstFsrVolParm[0].nIntID[1]    = FSR_INT_ID_NONE;
        gstFsrVolParm[0].nDevsInVol   = 1;
        gstFsrVolParm[0].bProcessorSynchronization = FALSE32;
        gstFsrVolParm[0].pExInfo      = NULL;

        gstFsrVolParm[1].nBaseAddr[0] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[1].nBaseAddr[1] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[1].nIntID[0]    = FSR_INT_ID_NONE;;
        gstFsrVolParm[1].nIntID[1]    = FSR_INT_ID_NONE;
        gstFsrVolParm[1].nDevsInVol   = 0;
        gstFsrVolParm[1].bProcessorSynchronization = FALSE32;
        gstFsrVolParm[1].pExInfo      = NULL;

    } while (0);

    RTL_PRINT((TEXT("[PAM:   ] --%s\r\n"), __FSR_FUNC__));

    return nRe;
}

/**
 * @brief           This function returns FSR volume parameter
 *                  this function is called by FSR_BML_Init
 *
 * @param[in]       stVolParm[FSR_MAX_VOLS] : FsrVolParm data structure array
 *
 * @return          FSR_PAM_SUCCESS
 * @return          FSR_PAM_NOT_INITIALIZED
 *
 * @author          SongHo Yoon
 * @version         1.0.0
 *
 */
PUBLIC INT32
FSR_PAM_GetPAParm(FsrVolParm stVolParm[FSR_MAX_VOLS])
{
    FSR_STACK_VAR;

    FSR_STACK_END;

    if (gbPAMInit == FALSE32)
    {
        return FSR_PAM_NOT_INITIALIZED;
    }

    FSR_OAM_MEMCPY(&(stVolParm[0]), &gstFsrVolParm[0], sizeof(FsrVolParm));
    FSR_OAM_MEMCPY(&(stVolParm[1]), &gstFsrVolParm[1], sizeof(FsrVolParm));

    return FSR_PAM_SUCCESS;
}

/**
 * @brief           This function registers LLD function table
 *                  this function is called by FSR_BML_Open
 *
 * @param[in]      *pstLFT[FSR_MAX_VOLS] : pointer to FSRLowFuncTable data structure
 *
 * @return          none
 *
 * @author          SongHo Yoon
 * @version         1.0.0
 *
 */
PUBLIC INT32
FSR_PAM_RegLFT(FSRLowFuncTbl  *pstLFT[FSR_MAX_VOLS])
{
    UINT32  nVolIdx;
    FSR_STACK_VAR;

    FSR_STACK_END;

    if (gbPAMInit == FALSE32)
    {
        return FSR_PAM_NOT_INITIALIZED;
    }

    if (gstFsrVolParm[0].nDevsInVol > 0)
    {
        nVolIdx = 0;

		pstLFT[nVolIdx]->LLD_Init				= FSR_PND_Init;
		pstLFT[nVolIdx]->LLD_Open				= FSR_PND_Open;
		pstLFT[nVolIdx]->LLD_Close				= FSR_PND_Close;
		pstLFT[nVolIdx]->LLD_Erase				= FSR_PND_Erase;
		pstLFT[nVolIdx]->LLD_ChkBadBlk			= FSR_PND_ChkBadBlk;
		pstLFT[nVolIdx]->LLD_FlushOp			= FSR_PND_FlushOp;
		pstLFT[nVolIdx]->LLD_GetDevSpec 		= FSR_PND_GetDevSpec;
		pstLFT[nVolIdx]->LLD_Read				= FSR_PND_Read;
		pstLFT[nVolIdx]->LLD_ReadOptimal		= FSR_PND_ReadOptimal;
		pstLFT[nVolIdx]->LLD_Write				= FSR_PND_Write;
		pstLFT[nVolIdx]->LLD_CopyBack			= FSR_PND_CopyBack;
		pstLFT[nVolIdx]->LLD_GetPrevOpData		= FSR_PND_GetPrevOpData;
		pstLFT[nVolIdx]->LLD_IOCtl				= FSR_PND_IOCtl;
		pstLFT[nVolIdx]->LLD_InitLLDStat		= FSR_PND_InitLLDStat;
		pstLFT[nVolIdx]->LLD_GetStat			= FSR_PND_GetStat;
		pstLFT[nVolIdx]->LLD_GetBlockInfo		= FSR_PND_GetBlockInfo;
		pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_PND_GetNANDCtrllerInfo;
		
#if 0


        if (gbFlexOneNAND[0] == FSR_FND_4K_PAGE)
        {
#if defined(FSR_ENABLE_FLEXOND_LFT)
            pstLFT[nVolIdx]->LLD_Init               = FSR_FND_Init;
            pstLFT[nVolIdx]->LLD_Open               = FSR_FND_Open;
            pstLFT[nVolIdx]->LLD_Close              = FSR_FND_Close;
            pstLFT[nVolIdx]->LLD_Erase              = FSR_FND_Erase;
            pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_FND_ChkBadBlk;
            pstLFT[nVolIdx]->LLD_FlushOp            = FSR_FND_FlushOp;
            pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_FND_GetDevSpec;
            pstLFT[nVolIdx]->LLD_Read               = FSR_FND_Read;
            pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_FND_ReadOptimal;
            pstLFT[nVolIdx]->LLD_Write              = FSR_FND_Write;
            pstLFT[nVolIdx]->LLD_CopyBack           = FSR_FND_CopyBack;
            pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_FND_GetPrevOpData;
            pstLFT[nVolIdx]->LLD_IOCtl              = FSR_FND_IOCtl;
            pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_FND_InitLLDStat;
            pstLFT[nVolIdx]->LLD_GetStat            = FSR_FND_GetStat;
            pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_FND_GetBlockInfo;
            pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_FND_GetPlatformInfo;
#else
            RTL_PRINT((TEXT("[PAM:ERR] LowFuncTbl(FlexOneNAND) isn't linked : %s / line %d\r\n"), __FSR_FUNC__, __LINE__));
            return FSR_PAM_LFT_NOT_LINKED;
#endif
        }
        else if (gbFlexOneNAND[0] == FSR_OND_2K_PAGE)
        {
#if defined(FSR_ENABLE_ONENAND_LFT)
            pstLFT[nVolIdx]->LLD_Init               = FSR_OND_Init;
            pstLFT[nVolIdx]->LLD_Open               = FSR_OND_Open;
            pstLFT[nVolIdx]->LLD_Close              = FSR_OND_Close;
            pstLFT[nVolIdx]->LLD_Erase              = FSR_OND_Erase;
            pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_OND_ChkBadBlk;
            pstLFT[nVolIdx]->LLD_FlushOp            = FSR_OND_FlushOp;
            pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_OND_GetDevSpec;
            pstLFT[nVolIdx]->LLD_Read               = FSR_OND_Read;
            pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_OND_ReadOptimal;
            pstLFT[nVolIdx]->LLD_Write              = FSR_OND_Write;
            pstLFT[nVolIdx]->LLD_CopyBack           = FSR_OND_CopyBack;
            pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_OND_GetPrevOpData;
            pstLFT[nVolIdx]->LLD_IOCtl              = FSR_OND_IOCtl;
            pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_OND_InitLLDStat;
            pstLFT[nVolIdx]->LLD_GetStat            = FSR_OND_GetStat;
            pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_OND_GetBlockInfo;
            pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_OND_GetNANDCtrllerInfo;
#else
            RTL_PRINT((TEXT("[PAM:ERR] LowFuncTbl(OneNAND) isn't linked : %s / line %d\r\n"), __FSR_FUNC__, __LINE__));
            return FSR_PAM_LFT_NOT_LINKED;
#endif
        }
        else if (gbFlexOneNAND[0] == FSR_OND_4K_PAGE)
        {
#if defined(FSR_ENABLE_4K_ONENAND_LFT)
            pstLFT[nVolIdx]->LLD_Init               = FSR_OND_4K_Init;
            pstLFT[nVolIdx]->LLD_Open               = FSR_OND_4K_Open;
            pstLFT[nVolIdx]->LLD_Close              = FSR_OND_4K_Close;
            pstLFT[nVolIdx]->LLD_Erase              = FSR_OND_4K_Erase;
            pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_OND_4K_ChkBadBlk;
            pstLFT[nVolIdx]->LLD_FlushOp            = FSR_OND_4K_FlushOp;
            pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_OND_4K_GetDevSpec;
            pstLFT[nVolIdx]->LLD_Read               = FSR_OND_4K_Read;
            pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_OND_4K_ReadOptimal;
            pstLFT[nVolIdx]->LLD_Write              = FSR_OND_4K_Write;
            pstLFT[nVolIdx]->LLD_CopyBack           = FSR_OND_4K_CopyBack;
            pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_OND_4K_GetPrevOpData;
            pstLFT[nVolIdx]->LLD_IOCtl              = FSR_OND_4K_IOCtl;
            pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_OND_4K_InitLLDStat;
            pstLFT[nVolIdx]->LLD_GetStat            = FSR_OND_4K_GetStat;
            pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_OND_4K_GetBlockInfo;
            pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_OND_4K_GetPlatformInfo;
#else
            RTL_PRINT((TEXT("[PAM:ERR] LowFuncTbl(FlexOneNAND) isn't linked : %s / line %d\r\n"), __FSR_FUNC__, __LINE__));
            return FSR_PAM_LFT_NOT_LINKED;
#endif
        }
    }

    if (gstFsrVolParm[1].nDevsInVol > 0)
    {
        nVolIdx = 1;

        if (gbFlexOneNAND[1] == FSR_FND_4K_PAGE)
        {
#if defined(FSR_ENABLE_FLEXOND_LFT)
            pstLFT[nVolIdx]->LLD_Init               = FSR_FND_Init;
            pstLFT[nVolIdx]->LLD_Open               = FSR_FND_Open;
            pstLFT[nVolIdx]->LLD_Close              = FSR_FND_Close;
            pstLFT[nVolIdx]->LLD_Erase              = FSR_FND_Erase;
            pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_FND_ChkBadBlk;
            pstLFT[nVolIdx]->LLD_FlushOp            = FSR_FND_FlushOp;
            pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_FND_GetDevSpec;
            pstLFT[nVolIdx]->LLD_Read               = FSR_FND_Read;
            pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_FND_ReadOptimal;
            pstLFT[nVolIdx]->LLD_Write              = FSR_FND_Write;
            pstLFT[nVolIdx]->LLD_CopyBack           = FSR_FND_CopyBack;
            pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_FND_GetPrevOpData;
            pstLFT[nVolIdx]->LLD_IOCtl              = FSR_FND_IOCtl;
            pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_FND_InitLLDStat;
            pstLFT[nVolIdx]->LLD_GetStat            = FSR_FND_GetStat;
            pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_FND_GetBlockInfo;
            pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_FND_GetPlatformInfo;
#else
            RTL_PRINT((TEXT("[PAM:ERR] LowFuncTbl(FlexOneNAND) isn't linked : %s / line %d\r\n"), __FSR_FUNC__, __LINE__));
            return FSR_PAM_LFT_NOT_LINKED;
#endif
        }
        else if (gbFlexOneNAND[0] == FSR_OND_2K_PAGE)
        {
#if defined(FSR_ENABLE_ONENAND_LFT)
            pstLFT[nVolIdx]->LLD_Init               = FSR_OND_Init;
            pstLFT[nVolIdx]->LLD_Open               = FSR_OND_Open;
            pstLFT[nVolIdx]->LLD_Close              = FSR_OND_Close;
            pstLFT[nVolIdx]->LLD_Erase              = FSR_OND_Erase;
            pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_OND_ChkBadBlk;
            pstLFT[nVolIdx]->LLD_FlushOp            = FSR_OND_FlushOp;
            pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_OND_GetDevSpec;
            pstLFT[nVolIdx]->LLD_Read               = FSR_OND_Read;
            pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_OND_ReadOptimal;
            pstLFT[nVolIdx]->LLD_Write              = FSR_OND_Write;
            pstLFT[nVolIdx]->LLD_CopyBack           = FSR_OND_CopyBack;
            pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_OND_GetPrevOpData;
            pstLFT[nVolIdx]->LLD_IOCtl              = FSR_OND_IOCtl;
            pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_OND_InitLLDStat;
            pstLFT[nVolIdx]->LLD_GetStat            = FSR_OND_GetStat;
            pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_OND_GetBlockInfo;
            pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_OND_GetNANDCtrllerInfo;
#else
            RTL_PRINT((TEXT("[PAM:ERR] LowFuncTbl(OneNAND) isn't linked : %s / line %d\r\n"), __FSR_FUNC__, __LINE__));
            return FSR_PAM_LFT_NOT_LINKED;
#endif
        }
        else if (gbFlexOneNAND[0] == FSR_OND_4K_PAGE)
        {
#if defined(FSR_ENABLE_4K_ONENAND_LFT)
            pstLFT[nVolIdx]->LLD_Init               = FSR_OND_4K_Init;
            pstLFT[nVolIdx]->LLD_Open               = FSR_OND_4K_Open;
            pstLFT[nVolIdx]->LLD_Close              = FSR_OND_4K_Close;
            pstLFT[nVolIdx]->LLD_Erase              = FSR_OND_4K_Erase;
            pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_OND_4K_ChkBadBlk;
            pstLFT[nVolIdx]->LLD_FlushOp            = FSR_OND_4K_FlushOp;
            pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_OND_4K_GetDevSpec;
            pstLFT[nVolIdx]->LLD_Read               = FSR_OND_4K_Read;
            pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_OND_4K_ReadOptimal;
            pstLFT[nVolIdx]->LLD_Write              = FSR_OND_4K_Write;
            pstLFT[nVolIdx]->LLD_CopyBack           = FSR_OND_4K_CopyBack;
            pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_OND_4K_GetPrevOpData;
            pstLFT[nVolIdx]->LLD_IOCtl              = FSR_OND_4K_IOCtl;
            pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_OND_4K_InitLLDStat;
            pstLFT[nVolIdx]->LLD_GetStat            = FSR_OND_4K_GetStat;
            pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_OND_4K_GetBlockInfo;
            pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_OND_4K_GetPlatformInfo;
#else
            RTL_PRINT((TEXT("[PAM:ERR] LowFuncTbl(FlexOneNAND) isn't linked : %s / line %d\r\n"), __FSR_FUNC__, __LINE__));
            return FSR_PAM_LFT_NOT_LINKED;
#endif
        }

#endif

    }

    return FSR_PAM_SUCCESS;
}

/**
 * @brief           This function transfers data to NAND
 *
 * @param[in]      *pDst  : Destination array Pointer to be copied
 * @param[in]      *pSrc  : Source data allocated Pointer
 * @param[in]      *nSize : length to be transferred
 *
 * @return          none
 *
 * @author          SongHo Yoon
 * @version         1.0.0
 * @remark          pDst / pSrc address should be aligned by 4 bytes.
 *                  DO NOT USE memcpy of standard library
 *                  memcpy (RVDS2.2) can do wrong memory copy operation.
 *                  memcpy32 is optimized by using multiple load/store instruction
 *
 */
PUBLIC VOID
FSR_PAM_TransToNAND(volatile VOID *pDst,
                    VOID          *pSrc,
                    UINT32        nSize)
{
    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_ASSERT(((UINT32) pSrc & 0x03) == 0x00000000);
    FSR_ASSERT(((UINT32) pDst & 0x03) == 0x00000000);
    FSR_ASSERT(nSize > sizeof(UINT32));

	FSR_PAM_Memcpy((VOID *)pDst, pSrc, nSize);

}

/**
 * @brief           This function transfers data from NAND
 *
 * @param[in]      *pDst  : Destination array Pointer to be copied
 * @param[in]      *pSrc  : Source data allocated Pointer
 * @param[in]      *nSize : length to be transferred
 *
 * @return          none
 *
 * @author          SongHo Yoon
 * @version         1.0.0
 * @remark          pDst / pSrc address should be aligned by 4 bytes
 *                  DO NOT USE memcpy of standard library
 *                  memcpy (RVDS2.2) can do wrong memory copy operation
 *                  memcpy32 is optimized by using multiple load/store instruction
 *
 */
VOID
FSR_PAM_TransFromNAND(VOID          *pDst,
                      volatile VOID *pSrc,
                      UINT32         nSize)
{
    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_ASSERT(((UINT32) pSrc & 0x03) == 0x00000000);
    FSR_ASSERT(((UINT32) pDst & 0x03) == 0x00000000);
    FSR_ASSERT(nSize > sizeof(UINT32));

	FSR_PAM_Memcpy(pDst, (VOID *)pSrc, nSize);
}

/**
 * @brief           This function initializes the specified logical interrupt.
 *
 * @param[in]       nLogIntId : Logical interrupt id
 *
 * @return          FSR_PAM_SUCCESS
 *
 * @author          SongHo Yoon
 *
 * @version         1.0.0
 *
 * @remark          this function is used to support non-blocking I/O feature of FSR
 *
 */
PUBLIC INT32
FSR_PAM_InitInt(UINT32 nLogIntId)
{

    return FSR_PAM_SUCCESS;
}

/**
 * @brief           This function deinitializes the specified logical interrupt.
 *
 * @param[in]       nLogIntId : Logical interrupt id
 *
 * @return          FSR_PAM_SUCCESS
 *
 * @author          SongHo Yoon
 *
 * @version         1.0.0
 *
 * @remark          this function is used to support non-blocking I/O feature of FSR
 *
 */
PUBLIC INT32
FSR_PAM_DeinitInt(UINT32 nLogIntId)
{


    return FSR_PAM_SUCCESS;
}

/**
 * @brief           This function returns the physical interrupt ID from the logical interrupt ID
 *
 * @param[in]       nLogIntID : Logical interrupt id
 *
 * @return          physical interrupt ID
 *
 * @author          SongHo Yoon
 *
 * @version         1.0.0
 *
 */
UINT32
FSR_PAM_GetPhyIntID(UINT32  nLogIntID)
{


    return 0;
}

/**
 * @brief           This function enables the specified interrupt.
 *
 * @param[in]       nLogIntID : Logical interrupt id
 *
 * @return          FSR_PAM_SUCCESS
 *
 * @author          Seunghyun Han
 *
 * @version         1.0.0
 *
 */
INT32
FSR_PAM_ClrNEnableInt(UINT32 nLogIntID)
{
    switch (nLogIntID)
    {
    case FSR_INT_ID_NAND_0:
        break;
    default:
        break;
    }

    return FSR_PAM_SUCCESS;
}

/**
 * @brief           This function disables the specified interrupt.
 *
 * @param[in]       nLogIntID : Logical interrupt id
 *
 * @return          FSR_PAM_SUCCESS
 *
 * @author          Seunghyun Han
 *
 * @version         1.0.0
 *
 */
INT32
FSR_PAM_ClrNDisableInt(UINT32 nLogIntID)
{
    switch (nLogIntID)
    {
    case FSR_INT_ID_NAND_0:
        break;
    default:
        break;
    }

    return FSR_PAM_SUCCESS;
}


/**
 * @brief           This function creates spin lock for dual core.
 *
 * @param[out]     *pHandle : Handle of semaphore
 * @param[in]       nLayer  : 0 : FSR_OAM_SM_TYPE_BDD
 *                            0 : FSR_OAM_SM_TYPE_STL
 *                            1 : FSR_OAM_SM_TYPE_BML
 *                            2 : FSR_OAM_SM_TYPE_LLD
 *
 * @return          TRUE32   : this function creates spin lock successfully
 * @return          FALSE32  : fail
 *
 * @author          DongHoon Ham
 * @version         1.0.0
 * @remark          An initial count of spin lock object is 1.
 *                  An maximum count of spin lock object is 1.
 *
 */
BOOL32
FSR_PAM_CreateSL(UINT32  *pHandle, UINT32  nLayer)
{
    return TRUE32;
}

/**
 * @brief          This function acquires spin lock for dual core.
 *
 * @param[in]       nHandle : Handle of semaphore to be acquired
 * @param[in]       nLayer  : 0 : FSR_OAM_SM_TYPE_BDD
 *                            0 : FSR_OAM_SM_TYPE_STL
 *                            1 : FSR_OAM_SM_TYPE_BML
 *                            2 : FSR_OAM_SM_TYPE_LLD
 *
 * @return          TRUE32   : this function acquires spin lock successfully
 * @return          FALSE32  : fail
 *
 * @author          DongHoon Ham
 * @version         1.0.0
 *
 */
BOOL32
FSR_PAM_AcquireSL(UINT32  nHandle, UINT32  nLayer)
{
    return TRUE32;
}

/**
 * @brief           This function releases spin lock for dual core.
 *
 * @param[in]       nHandle : Handle of semaphore to be released
 * @param[in]       nLayer  : 0 : FSR_OAM_SM_TYPE_BDD
 *                            0 : FSR_OAM_SM_TYPE_STL
 *                            1 : FSR_OAM_SM_TYPE_BML
 *                            2 : FSR_OAM_SM_TYPE_LLD
 *
 * @return          TRUE32   : this function releases spin lock successfully
 * @return          FALSE32  : fail
 *
 * @author          DongHoon Ham
 * @version         1.0.0
 *
 */
BOOL32
FSR_PAM_ReleaseSL(UINT32  nHandle, UINT32  nLayer)
{
    return TRUE32;
}
