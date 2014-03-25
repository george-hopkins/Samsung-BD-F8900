/**
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Copyright (C) 2003-2010 Samsung Electronics                               *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License version 2 as         *
 * published by the Free Software Foundation.                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 */

/**
 * @version     LinuStoreIII_1.2.0_b040-FSR_1.2.1p1_b139_RTM
 * @file        FSR_PAM_Poseidon.c
 * @brief       This file contain the Platform Adaptation Modules for Poseidon
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
//#define     FSR_ENABLE_ONENAND_LFT

/**< if FSR_ENABLE_4K_ONENAND_LFT is defined, 
     Low level function table is linked with OneNAND LLD */
#define     FSR_ENABLE_4K_ONENAND_LFT

#if defined(FSR_WINCE_OAM)

    //#include <oal_memory.h>
    #include <omap2420_irq.h>

    #define     FSR_ONENAND_PHY_BASE_ADDR           ONENAND_BASEADDR
    //#define     FSR_ONENAND_VIR_BASE_ADDR           ((UINT32)OALPAtoVA(ONENAND_BASEADDR,FALSE))
    #define     FSR_ONENAND_VIR_BASE_ADDR           0xA0000000

    /**< if FSR_ENABLE_WRITE_DMA is defined, write DMA is enabled */
    #undef      FSR_ENABLE_WRITE_DMA
    /**< if FSR_ENABLE_READ_DMA is defined, read DMA is enabled */
    #undef      FSR_ENABLE_READ_DMA

    #define     IRQ_GPIO_72                         (IRQ_GPIO_64 + 8 )

#elif defined(FSR_SYMOS_OAM)

    #include <..\..\..\MD\Src\d_mednand.h>
    #include <Shared_gpio.h>

    #define     FSR_ONENAND_PHY_BASE_ADDR           (0xc0000000)

    /**< if FSR_ENABLE_WRITE_DMA is defined, write DMA is enabled */
    #undef      FSR_ENABLE_WRITE_DMA
    /**< if FSR_ENABLE_READ_DMA is defined, read DMA is enabled */
    #undef      FSR_ENABLE_READ_DMA

    const TUint KNandGpioInterrupt = 72;

    class TNandGpioInterrupt
    	{
    public:
    	/** Bind to a Gpio Pin */
    	TInt Bind(TInt aPinId);
    
    	static void NandInterrupt(TAny* aPtr, TOmapGpioPinHandle aHandle);
    
    	inline void Enable(TBool aEnable=ETrue) {iPinWrapper->EnableInterruptNotif(aEnable);}
        inline void AckInterruptNotif() {iPinWrapper->AckInterruptNotif();}
    private:
    	TOmapGpioPinHandle iPinHandle;
    	TOmapGpioInputPinWrapper* iPinWrapper;
    	};
    
    extern DMediaDriverNand* pMDNand;
    TNandGpioInterrupt* pNandGpioInterrupt;

#elif defined(FSR_LINUX_OAM)
    #if defined(TINY_FSR)
        /* Setting at Kernel configuration */
        #define     FSR_ONENAND_PHY_BASE_ADDR       CONFIG_TINY_FLASH_PHYS_ADDR
    #else
        #define     FSR_ONENAND_PHY_BASE_ADDR       CONFIG_FSR_FLASH_PHYS_ADDR
    #endif
#ifdef MULTI_VOLUME
    #define		FSR_2NDONENAND_PHY_BASE_ADDR		CONFIG_FSR_FLASH_PHYS_ADDR2
#endif

    /**< if FSR_ENABLE_WRITE_DMA is defined, write DMA is enabled */
    #undef      FSR_ENABLE_WRITE_DMA
    /**< if FSR_ENABLE_READ_DMA is defined, read DMA is enabled */
    #undef      FSR_ENABLE_READ_DMA

#else /* RTOS (such as Nucleus) or OSLess */

    #include <stdarg.h>
    //#include <stdio.h>
//    #include "OMAP2420.h"
    
    #define     FSR_ONENAND_PHY_BASE_ADDR           (0x00000000)
#if defined(MULTI_VOLUME) 
    #define		FSR_2NDONENAND_PHY_BASE_ADDR		(0x0A000000)
#endif
    /**< if FSR_ENABLE_WRITE_DMA is defined, write DMA is enabled */
    #undef      FSR_ENABLE_WRITE_DMA
    /**< if FSR_ENABLE_READ_DMA is defined, read DMA is enabled */
    #undef      FSR_ENABLE_READ_DMA

    #define     INT_GPIO3_IRQ                       (31)
    #define     GPIO_8_OFFSET                       (8)
    #define     GPIO_72                             (1 << GPIO_8_OFFSET)     // gpio 72 -> gpio 8 (64+8)

#endif

#if defined(FSR_ENABLE_FLEXOND_LFT)
    #include "FSR_LLD_FlexOND.h"
#endif

#if defined(FSR_ENABLE_ONENAND_LFT)
    #include "FSR_LLD_OneNAND.h"
#endif

#if defined(FSR_ENABLE_4K_ONENAND_LFT)
    #include "FSR_LLD_4K_OneNAND.h"
#endif

/*****************************************************************************/
/* Global variables definitions                                              */
/*****************************************************************************/

/*****************************************************************************/
/* Local #defines                                                            */
/*****************************************************************************/
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
PRIVATE UINT32                  gbFlexOneNAND[FSR_MAX_VOLS] = {FSR_OND_2K_PAGE, FSR_OND_2K_PAGE};

#if defined(FSR_ENABLE_ONENAND_LFT)
PRIVATE volatile OneNANDReg     *gpOneNANDReg               = (volatile OneNANDReg *) 0;
#if defined(MULTI_VOLUME)
PRIVATE volatile OneNANDReg     *gp2ndOneNANDReg            = (volatile OneNANDReg *) 0;
#endif

#elif defined(FSR_ENABLE_4K_ONENAND_LFT)
PRIVATE volatile OneNAND4kReg   *gpOneNANDReg               = (volatile OneNAND4kReg *) 0;
#if defined(MULTI_VOLUME)
PRIVATE volatile OneNAND4kReg   *gp2ndOneNANDReg            = (volatile OneNAND4kReg *) 0;
#endif

#elif defined(FSR_ENABLE_FLEXOND_LFT)
PRIVATE volatile FlexOneNANDReg *gpOneNANDReg               = (volatile FlexOneNANDReg *) 0;
#if defined(MULTI_VOLUME)
PRIVATE volatile FlexOneNANDReg *gp2ndOneNANDReg            = (volatile FlexOneNANDReg *) 0;
#endif

#else
#error  Either FSR_ENABLE_FLEXOND_LFT or FSR_ENABLE_ONENAND_LFT should be defined
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern  VOID    memcpy32 (VOID       *pDst,
                          VOID       *pSrc,
                          UINT32     nSize);

#if defined(FSR_WINCE_OAM)
extern  UINT32  CheckMMU (VOID);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/

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
#if defined(MULTI_VOLUME)
    UINT32  n2ndONDVirBaseAddr;
#endif

    FSR_STACK_VAR;

    FSR_STACK_END;

    if (gbPAMInit == TRUE32)
    {
        return FSR_PAM_SUCCESS;
    }
    gbPAMInit     = TRUE32;

    RTL_PRINT((TEXT("[PAM:   ] ++%s\r\n"), __FSR_FUNC__));

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

#if defined(FSR_WINCE_OAM)
        /* For WCE/WM, FSR_OAM_Pa2Va does nothing. */
        if (((UINT32)CheckMMU()) & 0x01)
        {
            /* MMU is on. */
            nONDVirBaseAddr  = FSR_ONENAND_VIR_BASE_ADDR;
            RTL_PRINT((TEXT("[PAM:   ]   MMU is Enabled\r\n")));
        }
        else
        {
            nONDVirBaseAddr  = FSR_ONENAND_PHY_BASE_ADDR;
            RTL_PRINT((TEXT("[PAM:   ]   MMU is Disabed\r\n")));
        }
#else
        nONDVirBaseAddr  = FSR_OAM_Pa2Va(FSR_ONENAND_PHY_BASE_ADDR);
#if defined(MULTI_VOLUME)
        n2ndONDVirBaseAddr = FSR_OAM_Pa2Va(FSR_2NDONENAND_PHY_BASE_ADDR);	
#endif

#endif

        RTL_PRINT((TEXT("[PAM:   ]   OneNAND physical base address       : 0x%08x\r\n"), FSR_ONENAND_PHY_BASE_ADDR));
        RTL_PRINT((TEXT("[PAM:   ]   OneNAND virtual  base address       : 0x%08x\r\n"), nONDVirBaseAddr));
#if defined(MULTI_VOLUME)
        RTL_PRINT((TEXT("[PAM:   ]   2nd OneNAND physical base address       : 0x%08x\r\n"), FSR_2NDONENAND_PHY_BASE_ADDR));
        RTL_PRINT((TEXT("[PAM:   ]   2nd OneNAND virtual  base address       : 0x%08x\r\n"), n2ndONDVirBaseAddr));
#endif


#if defined(FSR_ENABLE_ONENAND_LFT)
        gpOneNANDReg  = (volatile OneNANDReg *) nONDVirBaseAddr;
#if defined(MULTI_VOLUME)
        gp2ndOneNANDReg = (volatile OneNANDReg *) n2ndONDVirBaseAddr;
#endif

#elif defined(FSR_ENABLE_4K_ONENAND_LFT)
        gpOneNANDReg  = (volatile OneNAND4kReg *) nONDVirBaseAddr;
#if defined(MULTI_VOLUME)
        gp2ndOneNANDReg = (volatile OneNAND4kReg *) n2ndONDVirBaseAddr;
#endif

#elif defined(FSR_ENABLE_FLEXOND_LFT)
        gpOneNANDReg  = (volatile FlexOneNANDReg *) nONDVirBaseAddr;
#if defined(MULTI_VOLUME)
        gp2ndOneNANDReg  = (volatile FlexOneNANDReg *) n2ndONDVirBaseAddr;
#endif
#endif

        /* check manufacturer ID */
        if (gpOneNANDReg->nMID != 0x00ec)
        {
            /* ERROR : OneNAND address space is not mapped with address space of ARM core */
            RTL_PRINT((TEXT("[PAM:ERR] OneNAND address space is not mapped with address space of ARM core\r\n")));
            nRe = FSR_PAM_NAND_PROBE_FAILED;
            break;
        }

        /* check whether current attached OneNAND is Flex-OneNAND or OneNAND */
        if (((gpOneNANDReg->nDID >> 8) & 0x03) == ONENAND_DID_FLEX)
        {
            gbFlexOneNAND[0] = FSR_FND_4K_PAGE;

            RTL_PRINT((TEXT("[PAM:   ]   Flex-OneNAND nMID=0x%2x : nDID=0x%02x\r\n"), 
                    gpOneNANDReg->nMID, gpOneNANDReg->nDID));
        }
        else
        {
	        /* Check amount of data buffers whether 2 (2KB page) (1: 4KB page)*/
            if ((gpOneNANDReg->nBufAmount & 0x0F00) == 0x0100)
            {
                gbFlexOneNAND[0] = FSR_OND_4K_PAGE;
            } else if ((gpOneNANDReg->nBufAmount & 0x0F00) == 0x0200)
            {
                gbFlexOneNAND[0] = FSR_OND_2K_PAGE;
            } else
            {
                RTL_PRINT((TEXT("[PAM:ERR] Unsupported OneNAND. Size of Data Buffers is %d\r\n"),
					gpOneNANDReg->nBufAmount >> 8));
                nRe = FSR_PAM_NAND_PROBE_FAILED;
                break;
            }

            RTL_PRINT((TEXT("[PAM:   ]   OneNAND nMID=0x%2x : nDID=0x%02x Page size=%dKB\r\n"), 
                    gpOneNANDReg->nMID, gpOneNANDReg->nDID, (gpOneNANDReg->nBufAmount >> 8) == 1 ? 4 : 2));
        }
#if defined(MULTI_VOLUME)
        /* check manufacturer ID */
        if (gp2ndOneNANDReg->nMID != 0x00ec)
        {
            /* ERROR : OneNAND address space is not mapped with address space of ARM core */
            RTL_PRINT((TEXT("[PAM:ERR] 2nd OneNAND address space is not mapped with address space of ARM core\r\n")));
            nRe = FSR_PAM_NAND_PROBE_FAILED;
            break;
        }
        /* check whether current attached OneNAND is Flex-OneNAND or OneNAND */
        if (((gp2ndOneNANDReg->nDID >> 8) & 0x03) == ONENAND_DID_FLEX)
        {
            gbFlexOneNAND[1] = FSR_FND_4K_PAGE;

            RTL_PRINT((TEXT("[PAM:   ]   2nd Flex-OneNAND nMID=0x%2x : nDID=0x%02x\r\n"), 
                    gp2ndOneNANDReg->nMID, gp2ndOneNANDReg->nDID));
        }
        else
        {
	        /* Check amount of data buffers whether 2 (2KB page) (1: 4KB page)*/
            if ((gp2ndOneNANDReg->nBufAmount & 0x0F00) == 0x0100)
            {
                gbFlexOneNAND[1] = FSR_OND_4K_PAGE;
            } else if ((gp2ndOneNANDReg->nBufAmount & 0x0F00) == 0x0200)
            {
                gbFlexOneNAND[1] = FSR_OND_2K_PAGE;
            } else 
            {
                RTL_PRINT((TEXT("[PAM:ERR] Unsupported 2nd OneNAND. Size of Data Buffers is %d\r\n"),
					gp2ndOneNANDReg->nBufAmount >> 8));
                nRe = FSR_PAM_NAND_PROBE_FAILED;
                break;
            }

            RTL_PRINT((TEXT("[PAM:   ]  2nd OneNAND nMID=0x%2x : nDID=0x%02x Page size=%d\r\n"), 
                    gp2ndOneNANDReg->nMID, gp2ndOneNANDReg->nDID, (gp2ndOneNANDReg->nBufAmount >> 8) == 1 ? 4 : 2));
        }
#endif

        gstFsrVolParm[0].nBaseAddr[0] = nONDVirBaseAddr;
        gstFsrVolParm[0].nBaseAddr[1] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[0].nIntID[0]    = FSR_INT_ID_NAND_0;
        gstFsrVolParm[0].nIntID[1]    = FSR_INT_ID_NONE;
        gstFsrVolParm[0].nDevsInVol   = 1;
        gstFsrVolParm[0].bProcessorSynchronization = FALSE32;
        gstFsrVolParm[0].pExInfo      = NULL;
#if defined(MULTI_VOLUME)
        gstFsrVolParm[1].nBaseAddr[0] = n2ndONDVirBaseAddr;
        gstFsrVolParm[1].nBaseAddr[1] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[1].nIntID[0]    = FSR_INT_ID_NAND_1;;
        gstFsrVolParm[1].nIntID[1]    = FSR_INT_ID_NONE;
        gstFsrVolParm[1].nDevsInVol   = 1;
        gstFsrVolParm[1].pExInfo      = NULL;
#else
        gstFsrVolParm[1].nBaseAddr[0] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[1].nBaseAddr[1] = FSR_PAM_NOT_MAPPED;
        gstFsrVolParm[1].nIntID[0]    = FSR_INT_ID_NONE;;
        gstFsrVolParm[1].nIntID[1]    = FSR_INT_ID_NONE;
        gstFsrVolParm[1].nDevsInVol   = 0;
        gstFsrVolParm[1].bProcessorSynchronization = FALSE32;
        gstFsrVolParm[1].pExInfo      = NULL;
#endif
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

	for(nVolIdx = 0; nVolIdx < FSR_MAX_VOLS; nVolIdx++)
	{
        if (gstFsrVolParm[nVolIdx].nDevsInVol > 0)
        {
            if (gbFlexOneNAND[nVolIdx] == FSR_FND_4K_PAGE)
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
                RTL_PRINT((TEXT("[PAM:ERR] Vol %d LLDFuncTbl(FlexOneNAND) isn't linked : %s / line %d\r\n"),
																			nVolIdx, __FSR_FUNC__, __LINE__));
                return FSR_PAM_LFT_NOT_LINKED;
#endif
            }
            else if (gbFlexOneNAND[nVolIdx] == FSR_OND_2K_PAGE)
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
                RTL_PRINT((TEXT("[PAM:ERR] Vol %d LLDFuncTbl(OneNAND) isn't linked : %s / line %d\r\n"),
																		nVolIdx, __FSR_FUNC__, __LINE__));
                return FSR_PAM_LFT_NOT_LINKED;
#endif
            }
            else if (gbFlexOneNAND[nVolIdx] == FSR_OND_4K_PAGE)
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
                RTL_PRINT((TEXT("[PAM:ERR] Vol %d LLDFuncTbl(4K OneNAND) isn't linked : %s / line %d\r\n"),
																			nVolIdx, __FSR_FUNC__, __LINE__));
                return FSR_PAM_LFT_NOT_LINKED;
#endif
            }
        }
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

#if defined(FSR_LINUX_OAM)
	/* Linux */
	FSR_PAM_Memcpy((VOID *)pDst, (VOID *)pSrc, nSize);
#else
	/* OSless */
    if (nSize >= (FSR_SECTOR_SIZE / 2))
    {
        if (gbUseWriteDMA == TRUE32)
        {
            FSR_OAM_WriteDMA((UINT32) pDst, (UINT32) pSrc, nSize);
        }
        else
        {
            memcpy32((void *) pDst, pSrc, nSize);
        }
    }
    else
    {
        memcpy32((void *) pDst, pSrc, nSize);
    }
#endif
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

#if defined(FSR_LINUX_OAM)
	/* Linux */

	FSR_PAM_Memcpy((VOID *)pDst, (VOID *)pSrc, nSize);
#else
	/* OSless */
    if (nSize >= (FSR_SECTOR_SIZE / 2))
    {
        if (gbUseReadDMA == TRUE32)
        {
            FSR_OAM_ReadDMA((UINT32) pDst, (UINT32) pSrc, nSize);
        }
        else
        {
            memcpy32(pDst, (void *) pSrc, nSize);
        }
    }
    else
    {
        memcpy32(pDst, (void *) pSrc, nSize);
    }
#endif
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

#if defined(FSR_SYMOS_OAM)
        pNandGpioInterrupt->AckInterruptNotif(); /* Interrupt Clear   */
        pNandGpioInterrupt->Enable();            /* Interrupt Enable  */
#elif defined(FSR_WINCE_OAM)

#elif defined(FSR_LINUX_OAM)

#else /* for RTOS or OSLess*/

#endif
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

#if defined(FSR_SYMOS_OAM)
        pNandGpioInterrupt->AckInterruptNotif(); /* Interrupt Clear   */
        pNandGpioInterrupt->Enable(EFalse);      /* Interrupt Disable */
#elif defined(FSR_WINCE_OAM)

#elif defined(FSR_LINUX_OAM)

#else /* for RTOS or OSLess*/

#endif
        break;
    default:
        break;
    }

    return FSR_PAM_SUCCESS;
}


#if defined(FSR_SYMOS_OAM)
/**
 * @brief           This function is used only for SymOS.
 *
 * @param[in]       aPtr
 * @param[in]       aHandle
 *
 * @return          none
 *
 * @author          WooYoungYang
 *
 * @version         1.0.0
 *
 */
void TNandGpioInterrupt::NandInterrupt(TAny* aPtr, TOmapGpioPinHandle aHandle)
{
    FSR_PAM_ClrNDisableInt(FSR_INT_ID_NAND_0);
	pMDNand->iNandIreqDfc.Add();
}

/**
 * @brief           This function is used only for SymOS.
 *
 * @param[in]       aPinId
 *
 * @return          TInt
 *
 * @author          WooYoungYang
 *
 * @version         1.0.0
 *
 */
TInt TNandGpioInterrupt::Bind(TInt aPinId)
{
    iPinHandle = OmapGpioPinMgr::AcquirePin(aPinId);

    if (iPinHandle == KOmapGpioInvalidHandle)
        return KErrBadHandle;

	OmapGpioPinMgr::SetPinAsInput(iPinHandle);

    iPinWrapper = new TOmapGpioInputPinWrapper(iPinHandle);
    if (!iPinWrapper)
    {
        return KErrNoMemory;
    }

    // Configure the Intr on the falling edge
    iPinWrapper->SetEventDetection(EOmapGpioEDTEdge, EOmapGpioEDSUp);

    // Enable module wake-up on this pin (interrupt is also enabled)
    iPinWrapper->EnableWakeUpNotif();

    // Disable interrupt generation on the input pin
    iPinWrapper->EnableInterruptNotif(EFalse);

    return iPinWrapper->BindIsr(NandInterrupt, this);
}
#endif

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
