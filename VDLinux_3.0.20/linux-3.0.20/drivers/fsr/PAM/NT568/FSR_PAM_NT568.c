/*
   %COPYRIGHT%
 */

/**
 * @version     LinuStoreIII_1.2.0_b035-FSR_1.2.1p1_b129_RTM
 * @file        FSR_PAM_Poseidon.c
 * @brief       This file is a basement for FSR adoption. It povides
 *              partition management, contexts management
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

#define     FSR_ENABLE_NAND_LFT

//#define     FSR_ONENAND_PHY_BASE_ADDR           CONFIG_FSR_FLASH_PHYS_ADDR
#define     FSR_ONENAND_PHY_BASE_ADDR           0
//Ken, 2010.09.27
//#define     FSR_NAND_PHY_BASE_ADDR              0
#define     FSR_NAND_PHY_BASE_ADDR              0

#if defined(FSR_ENABLE_NAND_LFT)
#include "FSR_LLD_PureNAND.h"
#endif

/*****************************************************************************/
/* Global variables definitions                                              */
/*****************************************************************************/

/*****************************************************************************/
/* Local #defines                                                            */
/*****************************************************************************/

#define     ONENAND_DID_SLC         (0)
#define     ONENAND_DID_MLC         (1)
#define     ONENAND_DID_FLEX        (2)

#define     ONENAND_DIE_REG_OFFSET  (0x0001e002)

#define     FSR_FND_4K_PAGE         (0)
#define     FSR_OND_2K_PAGE         (1)
#define     FSR_OND_4K_PAGE         (2)
#define     FSR_NAND_8K_PAGE        (3)
#define     FSR_NAND_2K_PAGE        (4)    // Macoto modify 0717

#define     DBG_PRINT(x)            FSR_DBG_PRINT(x)
#define     RTL_PRINT(x)            FSR_RTL_PRINT(x)

/*****************************************************************************/
/* Local typedefs                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* Static variables definitions                                              */
/*****************************************************************************/
PRIVATE FsrVolParm  gstFsrVolParm[FSR_MAX_VOLS];
PRIVATE BOOL32      gbPAMInit                   = FALSE32;
PRIVATE BOOL32      gbUseWriteDMA               = FALSE32;
PRIVATE BOOL32      gbUseReadDMA                = FALSE32;

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

#if 0
VOID
FSR_PAM_Memcpy(VOID *pDst, VOID *pSrc, UINT32 nLen)
{
     FSR_OAM_Memcpy(pDst, pSrc,nLen);
}
#endif



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
extern void HAL_ParFlash_Init(void);
PUBLIC INT32
FSR_PAM_Init(VOID)
{
    INT32   nRe = FSR_PAM_SUCCESS;
    UINT32  nPNDVirBaseAddr;

    FSR_STACK_VAR;

    FSR_STACK_END;

    if (gbPAMInit == TRUE32)
    {
	    //Ben, debug, Oct/04/2010.
	    HAL_ParFlash_Init();

	    return FSR_PAM_SUCCESS;
    }
    gbPAMInit     = TRUE32;

    HAL_ParFlash_Init();


    do {
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
        nPNDVirBaseAddr = FSR_OAM_Pa2Va(0x30020000);
        RTL_PRINT((TEXT("[PAM:   ]   NAND physical base address       : 0x%08x\r\n"), FSR_NAND_PHY_BASE_ADDR));
        RTL_PRINT((TEXT("[PAM:   ]   NAND virtual  base address       : 0x%08x\r\n"), nPNDVirBaseAddr));

        gstFsrVolParm[0].nBaseAddr[0] = nPNDVirBaseAddr;
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

        pstLFT[nVolIdx]->LLD_Init               = FSR_PND_Init;             //
        pstLFT[nVolIdx]->LLD_Open               = FSR_PND_Open;             //
        pstLFT[nVolIdx]->LLD_Close              = FSR_PND_Close;            //
        pstLFT[nVolIdx]->LLD_Erase              = FSR_PND_Erase;            //
        pstLFT[nVolIdx]->LLD_ChkBadBlk          = FSR_PND_ChkBadBlk;        //
        pstLFT[nVolIdx]->LLD_FlushOp            = FSR_PND_FlushOp;          //
        pstLFT[nVolIdx]->LLD_GetDevSpec         = FSR_PND_GetDevSpec;       //
        pstLFT[nVolIdx]->LLD_Read               = FSR_PND_Read;             //
        pstLFT[nVolIdx]->LLD_ReadOptimal        = FSR_PND_ReadOptimal;      //
        pstLFT[nVolIdx]->LLD_Write              = FSR_PND_Write;            //
        pstLFT[nVolIdx]->LLD_CopyBack           = FSR_PND_CopyBack;         //
        pstLFT[nVolIdx]->LLD_GetPrevOpData      = FSR_PND_GetPrevOpData;    //
        pstLFT[nVolIdx]->LLD_IOCtl              = FSR_PND_IOCtl;
        pstLFT[nVolIdx]->LLD_InitLLDStat        = FSR_PND_InitLLDStat;      //
        pstLFT[nVolIdx]->LLD_GetStat            = FSR_PND_GetStat;          //
        pstLFT[nVolIdx]->LLD_GetBlockInfo       = FSR_PND_GetBlockInfo;     //
        pstLFT[nVolIdx]->LLD_GetNANDCtrllerInfo = FSR_PND_GetNANDCtrllerInfo; //

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

    if (nSize >= (FSR_SECTOR_SIZE / 2))
    {
        if (gbUseWriteDMA == TRUE32)
        {
            FSR_OAM_WriteDMA((UINT32) pDst, (UINT32) pSrc, nSize);
        }
        else
        {
            FSR_OAM_MEMCPY((void *) pDst, pSrc, nSize);
        }
    }
    else
    {
        FSR_OAM_MEMCPY((void *) pDst, pSrc, nSize);
    }
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

	//TODO check this
#if 1
    if (nSize >= (FSR_SECTOR_SIZE / 2))
    {
        if (gbUseReadDMA == TRUE32)
        {
            FSR_OAM_ReadDMA((UINT32) pDst, (UINT32) pSrc, nSize);
        }
        else
        {
            FSR_OAM_MEMCPY(pDst, (void *) pSrc, nSize);
        }
    }
    else
    {
        FSR_OAM_MEMCPY(pDst, (void *) pSrc, nSize);
    }
#else
    FSR_PAM_Memcpy((VOID *)pDst, (VOID *)pSrc, nSize);
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

