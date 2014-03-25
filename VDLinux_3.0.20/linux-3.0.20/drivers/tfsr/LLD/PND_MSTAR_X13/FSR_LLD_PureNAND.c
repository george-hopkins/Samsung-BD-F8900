/**
 *   @mainpage   Flex Sector Remapper : LinuStoreIII_1.2.0_b035-FSR_1.2.1p1_b129_RTM
 *
 *   @section Intro
 *       Flash Translation Layer for Flex-OneNAND and OneNAND
 *
 *    @section  Copyright
 *            COPYRIGHT. 2008-2009 SAMSUNG ELECTRONICS CO., LTD.
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
 * @file      FSR_LLD_PureNAND.c
 * @brief     This file implements Low Level Driver for PureNAND
 * @author    Jinho Yi, Jinhyuck Kim
 * @date      18-AUG-2009
 * @remark
 * REVISION HISTORY
 * @n  20-OCT-2008 [NamOh Hwang] : First writing
 * @n  07-JUL-2009 [JinHo Yi]    : Modify for PureNAND support
 * @n  24-AUG-2009 [JinHyuck Kim]: Modify for tiny support and statistics
 *
 */

/******************************************************************************/
/* Header file inclusions                                                     */
/******************************************************************************/
#define     FSR_NO_INCLUDE_BML_HEADER
#define     FSR_NO_INCLUDE_STL_HEADER

#include <linux/delay.h> 
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/version.h>

#include    <FSR.h>
#include    "FSR_LLD_PureNAND.h"

#include    "FSR_LLD_MSTAR_X13.h"


#if defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_AMBER1) || defined(CONFIG_MSTAR_EMERALD)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38)
DEFINE_SEMAPHORE(ond_mutex);
#else 
DECLARE_MUTEX(ond_mutex);
#endif


EXPORT_SYMBOL(ond_mutex);

void nand_lock_fcie(void)
{
	down(&ond_mutex);
}
EXPORT_SYMBOL(nand_lock_fcie);

void nand_unlock_fcie(void)
{
	up(&ond_mutex);
}
EXPORT_SYMBOL(nand_unlock_fcie);

#define NC_LOCK_FCIE()		nand_lock_fcie()
#define NC_UNLOCK_FCIE()	nand_unlock_fcie()

#else
#define NC_LOCK_FCIE()
#define NC_UNLOCK_FCIE()
#endif

#define NAND_TIMEOUT_RETRY_CNT     5

/******************************************************************************/
/*   Local Configurations                                                     */
/*                                                                            */
/* - FSR_LLD_STRICT_CHK        : To check parameters strictly                 */
/* - FSR_LLD_STATISTICS        : To accumulate statistics.                    */
/* - FSR_LLD_LOGGING_HISTORY   : To log history                               */
/******************************************************************************/
#define     FSR_LLD_STRICT_CHK
//#define     FSR_LLD_STATISTICS
#define     FSR_LLD_LOGGING_HISTORY

/* Controller setting */
//#define     FSR_LLD_ENABLE_CNTRLLER_INT
#define     FSR_LLD_ENABLE_CNTRLLER_ECC

/******************************************************************************/
/* Local #defines                                                             */
/******************************************************************************/
#define     FSR_PND_MAX_DEVS                    (FSR_MAX_DEVS)

#define     FSR_PND_SUPPORTED_DIES              (1)

#define     FSR_PND_MAX_BADMARK                 (4)  /* the size of gnBadMarkValue*/
#define     FSR_PND_MAX_BBMMETA                 (2)  /* the size of gnBBMMetaValue*/

/* MACROs below are used for Deferred Check Operation                         */
#define     FSR_PND_PREOP_NONE                  (0x0000)
#define     FSR_PND_PREOP_READ                  (0x0001)
#define     FSR_PND_PREOP_CPBK_LOAD             (0x0002)
#define     FSR_PND_PREOP_CACHE_PGM             (0x0003)
#define     FSR_PND_PREOP_PROGRAM               (0x0004)
#define     FSR_PND_PREOP_CPBK_PGM              (0x0005)
#define     FSR_PND_PREOP_ERASE                 (0x0006)
#define     FSR_PND_PREOP_IOCTL                 (0x0007)
#define     FSR_PND_PREOP_HOTRESET              (0x0008)
#define     FSR_PND_PREOP_ADDRESS_NONE          (0xFFFF)
#define     FSR_PND_PREOP_FLAG_NONE             (FSR_LLD_FLAG_NONE)

#define     FSR_PND_BUSYWAIT_TIME_LIMIT         (0x100000)

#define     FSR_PND_FLUSHOP_CALLER_BASEBIT      (16)
#define     FSR_PND_FLUSHOP_CALLER_MASK         (0xFFFF0000)

#define     FSR_PND_BLOCK_SIZE_128KB            (1)
#define     FSR_PND_BLOCK_SIZE_256KB            (2)
#define     FSR_PND_BLOCK_SIZE_1MB            (3)

#define     FSR_PND_VALID_BLK_MARK              (0xFFFF) /* x16 */
#define     FSR_PND_VALID_BLK_MARK_X6              (0xFF) /* x16 */


#define     FSR_PND_ECC_READ_DISTURBANCE        (2)

#define     FSR_PND_SPARE_FREE_AREA              (3)	/* x16 */
#define     FSR_PND_SPARE_ECC_AREA              (5)	/* x16 */

#define 	FSR_PND_X8_BUS_WIDTH                (8)
#define     FSR_PND_X16_BUS_WIDTH               (16)

/* MACROs below are used for the statistic                                   */
#define     FSR_PND_STAT_SLC_PGM                (0x00000001)
#define     FSR_PND_STAT_LSB_PGM                (0x00000002)
#define     FSR_PND_STAT_MSB_PGM                (0x00000003)
#define     FSR_PND_STAT_ERASE                  (0x00000004)
#define     FSR_PND_STAT_SLC_LOAD               (0x00000005)
#define     FSR_PND_STAT_MLC_LOAD               (0x00000006)
#define     FSR_PND_STAT_RD_TRANS               (0x00000007)
#define     FSR_PND_STAT_WR_TRANS               (0x00000008)
#define     FSR_PND_STAT_WR_CACHEBUSY           (0x00000009)
#define     FSR_PND_STAT_FLUSH                  (0x0000000A)

/* Command type for statistics */
#define     FSR_PND_STAT_NORMAL_CMD             (0x0)
#define     FSR_PND_STAT_PLOAD                  (0x1)
#define     FSR_PND_STAT_CACHE_PGM              (0x2)

#define     FSR_PND_CACHE_BUSY_TIME             (25) /* usec */
#define     FSR_PND_RD_SW_OH                    (5)  /* usec */
#define     FSR_PND_WR_SW_OH                    (10) /* usec */

/* In sync burst mode, reading from DataRAM has overhead */
#define     FSR_PND_RD_TRANS_SW_OH              (14) /* usec */


/* For General PureNAND LLD */
#define     FSR_PND_4CYCLES                     (4)
#define     FSR_PND_5CYCLES                     (5)
#define     INVALID_ADDRESS                     (0xFFFFFFFF)


/* Command code for NAND Controller */
#define     FSR_PND_SET_READ_PAGE               (0x0)
#define     FSR_PND_SET_PROGRAM_PAGE            (0x1)
#define     FSR_PND_SET_ERASE                   (0x2)
#define     FSR_PND_SET_RANDOM_DATA_INPUT       (0x3)
#define     FSR_PND_SET_RANDOM_DATA_OUTPUT      (0x4)
#define     FSR_PND_SET_RESET                   (0x5)
#define     FSR_PND_SET_READ_ID                 (0x6)
#define     FSR_PND_SET_READ_PGM_STATUS         (0x7)
#define     FSR_PND_SET_READ_ERASE_STATUS       (0x8)
#define     FSR_PND_SET_READ_INTERRUPT_STATUS   (0x9)
#define     FSR_PND_SET_ECC_ON                  (0xA)
#define     FSR_PND_SET_ECC_OFF                 (0xB)

#define     FSR_PND_BUSY                        (0x0)
#define     FSR_PND_READY                       (0x1)

#define     FSR_PND_OP_READ                     (0x0)
#define     FSR_PND_OP_PGM                      (0x1)
#define     FSR_PND_OP_ERASE                    (0x2)

#if defined(_FSR_LLD_MSTAR_X13)
/* Status Register Definition */
#define FSR_PND_STATUS_PGM_FAIL                      (0x01)
#define FSR_PND_STATUS_ERASE_FAIL                    (0x01)
#define FSR_PND_STATUS_READY                         (0x40)
#endif

#if !defined(FSR_LLD_ENABLE_CNTRLLER_ECC)
#define FSR_PND_ENABLE_ECC                           (0x0)
#define FSR_PND_DISABLE_ECC                          (0x1)

#define SWECC_E_ERROR                                (1)
#define SWECC_N_ERROR                                (0)
#define SWECC_C_ERROR                                (-1)
#define SWECC_U_ERROR                                (-2)
#define SWECC_R_ERROR                                (-3)
#endif


/******************************************************************************/
/* Local typedefs                                                             */
/******************************************************************************/
typedef struct
{
    UINT16          nMID;
    UINT16          nDID;
    
    UINT16          nNumOfBlks;
    
    UINT16          nNumOfDies;
    UINT16          nNumOfPlanes;
    
    UINT16          nSctsPerPg;
    UINT16          nSparePerSct;
    
    UINT16          nPgsPerBlk;
    
    UINT16          nRsvBlksInDev;
     
    UINT8           nBWidth;        /* Nand Organization X8 or X16   */
    UINT8           n5CycleDev;	    /* If 5 cycle device, this is 1
	                                   otherwise, this is 0         */
	                                   
    /* 
     * TrTime, TwTime of MLC are array of size 2
     * First  element is for LSB TLoadTime, TProgTime
     * SecPND element is for MLB TLoadTime, TProgTime
     * Use macro FSR_LLD_IDX_LSB_TIME, FSR_LLD_IDX_MSB_TIME
     */
    UINT32          nSLCTLoadTime;      /**< Typical Load     operation time  */    
    UINT32          nSLCTProgTime;      /**< Typical Program  operation time  */    
    UINT32          nTEraseTime;        /**< Typical Erase    operation time  */

#ifdef NC_SEL_FCIE3
	volatile UINT16 nReg1B_SdioCtrl;
	volatile UINT16 nReg40_Signal;
	volatile UINT16 nReg48_Spare;
	volatile UINT16 nReg49_SpareSize;
	volatile UINT16 nReg50_EccCtrl;

	UINT8 OpCode_RW_AdrCycle;
	UINT8 OpCode_Erase_AdrCycle;


	//-----------------------------
	// NAND Info
	//-----------------------------
	UINT32 nPageByteCnt;
	UINT16 nSpareByteCnt;
	UINT16 nPageSectorCnt;
	UINT16 nSectorByteCnt;
	UINT16 nSectorSpareByteCnt;
	UINT16 nECCCodeByteCnt;
	
	UINT8 nBlkPageCntBits;
	UINT8 nPageByteCntBits;
	UINT8 nSpareByteCntBits;	
	UINT8 nPageSectorCntBits;
	UINT8 nSectorByteCntBits;
	UINT8 nSectorSpareByteCntBits;
		
	UINT32 nSectorByteCntMask;
	UINT32 nPageByteCntMask;
	UINT16 nBlkPageCntMask;
	UINT16 nSpareByteCntMask;
	UINT16 nPageSectorCntMask;

	UINT16 nECCType;	
#endif
} PureNANDSpec;

typedef struct
{
    UINT32          nShMemUseCnt;   
    
    /**< Previous operation data structure which can be shared among process  */
    UINT16          nPreOp[FSR_MAX_DIES];
    UINT16          nPreOpPbn[FSR_MAX_DIES];
    UINT16          nPreOpPgOffset[FSR_MAX_DIES];
    UINT32          nPreOpFlag[FSR_MAX_DIES];
    UINT32	    ECCnRE;
    
    /**< Pseudo DataRAM. FIXME: The size is 4KB, It is difficult to allocate shared memory  */
    UINT8           pPseudoMainDataRAM[ FSR_SECTOR_SIZE * X9_FSR_MAX_PHY_SCTS];
    UINT8           pPseudoSpareDataRAM[ FSR_SPARE_SIZE * X9_FSR_MAX_PHY_SCTS];    

} PureNANDShMem;

typedef struct
{
    UINT32          nBaseAddr;
    BOOL32          bOpen;

    PureNANDSpec   *pstPNDSpec;
    
    UINT8           nSftPgsPerBlk;
    UINT8           nMskPgsPerBlk;    
    UINT16          nDDPSelBaseBit;

    UINT16          nFlushOpCaller;
    
    UINT16          nNumOfDies;

#if !defined(FSR_LLD_ENABLE_CNTRLLER_ECC)
    UINT32          nECCOption;    
#endif

    UINT32          nWrTranferTime;    /**< Write transfer time              */
    UINT32          nRdTranferTime;    /**< Read transfer time               */

    UINT32          nIntID;
} PureNANDCxt;

/**
 * @brief Data structure of OneNAND statistics
 */
typedef struct
{
    UINT32          nNumOfSLCLoads; /**< The number of times of Load  operations */
    UINT32          nNumOfMLCLoads;
    
    UINT32          nNumOfSLCPgms;  /**< The number of times of SLC programs     */    
    UINT32          nNumOfLSBPgms;  /**< The number of times of LSB programs     */
    UINT32          nNumOfMSBPgms;  /**< The number of times of MSB programs     */
    
    UINT32          nNumOfCacheBusy;/**< The number of times of Cache Busy       */
    UINT32          nNumOfErases;   /**< The number of times of Erase operations */

    UINT32          nNumOfRdTrans;  /**< The number of times of Read  Transfer   */
    UINT32          nNumOfWrTrans;  /**< The number of times of Write Transfer   */

    UINT32          nPreCmdOption[FSR_MAX_DIES]; /** Previous command option     */

    INT32           nIntLowTime[FSR_MAX_DIES];
                                    /**< MDP : 0 
                                         DDP : Previous operation time           */

    UINT32          nRdTransInBytes;/**< The number of bytes transfered in read  */
    UINT32          nWrTransInBytes;/**< The number of bytes transfered in write */
} PureNANDStat;

typedef struct
{
    UINT8           nMID;
    UINT8           nDID;
    UINT8           n3rdIDData;
    UINT8           n4thIDData;
    UINT8           n5thIDData;
    UINT8           nPad0;
    UINT16          nPad1;
} NANDIDData;

#define OPTYPE_ERASE        1
#define OPTYPE_WRITE        2

#define NC_PAD_SWITCH(enable)    nand_pads_switch(enable);

extern unsigned int Chip_Query_ID(void);

/******************************************************************************/
/* Global variable definitions                                                */
/******************************************************************************/
PRIVATE const UINT16    gnBBMMetaValue[FSR_PND_MAX_BBMMETA]   =
                            { 0xFFFF,
                              FSR_LLD_BBM_META_MARK
                            };

PRIVATE const UINT16    gnBadMarkValue[FSR_PND_MAX_BADMARK]   =
                            { 0xFFFF,         /* FSR_LLD_FLAG_WR_NOBADMARK    */
                              0x2222,         /* FSR_LLD_FLAG_WR_EBADMARK     */
                              0x4444,         /* FSR_LLD_FLAG_WR_WBADMARK     */
                              0x8888,         /* FSR_LLD_FLAG_WR_LBADMARK     */
                            };

PRIVATE PureNANDCxt    *gpstPNDCxt  [FSR_PND_MAX_DEVS];


PureNANDShMem  *gpstPNDShMem[FSR_PND_MAX_DEVS];


#if defined (FSR_LLD_STATISTICS)
PRIVATE PureNANDStat   *gpstPNDStat[FSR_PND_MAX_DEVS];
PRIVATE UINT32          gnElapsedTime;
PRIVATE UINT32          gnDevsInVol[FSR_MAX_VOLS] = {0, 0};
#endif /* #if defined (FSR_LLD_STATISTICS) */

#if defined(FSR_LLD_LOGGING_HISTORY)
volatile PureNANDOpLog gstPNDOpLog[FSR_MAX_DEVS] =
{
    {0, {0,}, {0,}, {0,},{0,}},
    {0, {0,}, {0,}, {0,},{0,}},
    {0, {0,}, {0,}, {0,},{0,}},
    {0, {0,}, {0,}, {0,},{0,}}
};
#endif


/******************************************************************************/
/* Local constant definitions                                                 */
/******************************************************************************/
PRIVATE const PureNANDSpec gastPNDSpec[] = {
/******************************************************************************/
/*    nMID,                                                                   */
/*        nDID,                                                               */
/*               nNumOfBlks                                                   */
/*                   nNumOfDies                                               */
/*                      nNumOfPlanes                                          */
/*                         nSctsPerPg                                         */
/*                            nSparePerSct                                    */
/*                               nPgsPerBlk                                   */
/*                                  nRsvBlksInDev                             */
/*                                     nBWidth                                */
/*                                        n5CycleDev                          */
/*                                           nSLCTLoadTime                    */
/*                                              nSLCTProgTime                 */
/*                                                  nTEraseTime               */
/******************************************************************************/
    /* KF92G16Q2X */ /* KF92G16Q2W */
    { 0xEC, 0xBA,   2048, 1, 1, 4, 16, 64, 40, 16, FSR_PND_5CYCLES, 40, 250, 2000
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    },

    /* KF94G16Q2W */
    { 0xEC, 0xBC,   4096, 1, 1, 4, 16, 64, 80, 16, FSR_PND_5CYCLES, 40, 250, 2000
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    },


   /* K9GAG008UE */	
    //{  0xEC, 0xD5, 2048, 1, 1, 16, 27, 128, 120 ,gnPairPgMap ,gnLSBPgs ,8, FSR_PND_5CYCLES, 40, 250, 2000,10,14,20,24 },

	/* ESMT : F59L1G81A */
    {  0x92, 0xF1, 1024, 1, 1, 4, 16, 64, 20, 8, FSR_PND_4CYCLES, 40, 250, 2000
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    },
     
	/* K9F1G08U0D */
    {  0xEC, 0xF1, 1024, 1, 1, 4, 16, 64, 20, 8, FSR_PND_4CYCLES, 40, 250, 2000
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    },

    /* K9F2G08U0C */
    {  0xEC, 0xDA, 2048, 1, 1, 4, 16, 64, 40, 8, FSR_PND_5CYCLES, 40, 250, 2000
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    },

	/* H27U2G8F2CTR */
    {  0xAD, 0xDA, 2048, 1, 1, 4, 16, 64, 20, 8, FSR_PND_5CYCLES, 40, 250, 2000
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    },
        
    { 0x00, 0x00,      0, 0, 0, 0,  0,  0,  0,  0,               0,  0,  0,     0
    #ifdef NC_SEL_FCIE3
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    #endif
    }
};
/******************************************************************************/
/* Local function prototypes                                                  */
/******************************************************************************/
PRIVATE VOID    _ReadSpare          (FSRSpareBuf   *pstDest, 
                                     UINT8         *pSrc8);

PRIVATE VOID    _WriteSpare         (PureNANDSpec  *pstPNDSpec,
                                     UINT8		   *pDest8,
                                     FSRSpareBuf   *pstSrc,
                                     UINT32         nFlag);                             
                             
PRIVATE INT32   _StrictChk          (UINT32         nDev,
                                     UINT32         nPbn,
                                     UINT32         nPgOffset);

PRIVATE INT32   _SetDeviceInfo      (UINT32         nDev);



PRIVATE INT32	_SetNANDCtrllerCmd  (UINT32         nDev,
                                     UINT32         nDie,
                            		 UINT32         nRowAddress,
                            		 UINT32         nColAddress,
                            		 UINT32         nCommand);
                                     

#if defined (FSR_LLD_LOGGING_HISTORY)
PRIVATE VOID    _AddLog             (UINT32         nDev,
                                     UINT32         nDie);
#endif /* #if defined (FSR_LLD_LOGGING_HISTORY) */

PRIVATE VOID    _DumpSpareBuffer    (UINT32         nDev);

PRIVATE VOID    _DumpMainBuffer     (UINT32         nDev);

#if defined (FSR_LLD_STATISTICS)
PRIVATE VOID    _AddPNDStat         (UINT32       nDev,
                                     UINT32       nDie,
                                     UINT32       nType,
                                     UINT32       nBytes,
                                     UINT32       nCmdOption);
#endif /* #if defined (FSR_LLD_STATISTICS) */

UINT32 NC_Init (PureNANDSpec *pstPNDSpec);
UINT32 nand_pads_switch(UINT32 u32EnableFCIE);
UINT32 MSTAR_NAND_INIT(void);
UINT8 *NC_ReadID(void);

UINT32 NC_ReadPages (PureNANDSpec     *pstPNDSpec, UINT32 u32_PhyRowIdx, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf, UINT32 u32_PageCnt);
UINT32 NC_EraseBlk(PureNANDSpec     *pstPNDSpec, UINT32 u32_PhyRowIdx);
UINT32 NC_WritePages(PureNANDSpec     *pstPNDSpec, UINT32 u32_PhyRowIdx, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf, UINT32 u32_PageCnt);
UINT32 NC_Read_RandomIn (PureNANDSpec     *pstPNDSpec, UINT32 u32_PhyRow, UINT32 u32Col, UINT8 *pu8DataBuf, UINT32 u32DataByteCnt);
UINT32 NC_Write_RandomOut(PureNANDSpec *pstPNDSpec, UINT32 u32_PhyRow, UINT32 u32Col, UINT8 *pu8DataBuf, UINT32 u32DataByteCnt);

UINT32 nand_clock_setting(UINT32 u32ClkParam);
UINT32 NC_ResetFCIE(void);
UINT32 NC_ResetNandFlash(void);
UINT32 NC_WaitComplete(UINT16 u16_WaitEvent, UINT32 u32_MicroSec);
void NC_GetCIFD(UINT8 *pu8_Buf, UINT32 u32_CIFDPos, UINT32 u32_ByteCnt);
void NC_SetCIFD(UINT8 *pu8_Buf, UINT32 u32_CIFDPos, UINT32 u32_ByteCnt);
void NC_SetCIFD_Const(UINT8 u8_Data, UINT32 u32_CIFDPos, UINT32 u32_ByteCnt);
void NC_Config(PureNANDSpec *pstPNDSpec);
static UINT32 NC_CheckEWStatus(UINT8 u8_OpType);
UINT32 nand_hw_timer_delay(UINT32 u32usTick);
UINT32  NC_GetECCBits(void);
VOID  NC_ControlECC(BOOL32 ecc_on);

void NC_DisableECC(void);
void NC_EnableECC(void);

void print_ECC_Reg(void);
void NC_Dump_Registers(void);

/******************************************************************************/
/* Code Implementation                                                        */
/******************************************************************************/

/**
 * @brief           This function reads data from spare area of Pseudo DataRAM
 *
 * @param[out]      pstDest : pointer to the host buffer
 * @param[in]       pSrc    : pointer to the spare area of Pseudo DataRAM
 *
 * @return          none
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          This function reads from spare area of 1 page.
 *
 */
PRIVATE VOID  _ReadSpare(FSRSpareBuf *pstDest, 
                         UINT8       *pSrc8)
{
    UINT16 *pSrc16;

    FSR_STACK_VAR;

    FSR_STACK_END;	

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(pstDest: 0x%08x, pSrc8: 0x%08x)\r\n"),
        __FSR_FUNC__, pstDest, pSrc8));

    FSR_ASSERT(((UINT32) pstDest & 0x01) == 0);
    
    pSrc16 = (UINT16 *)pSrc8;

    pstDest->pstSpareBufBase->nBadMark       = *pSrc16++;
    pstDest->pstSpareBufBase->nBMLMetaBase0  = *pSrc16++;
    
	pstDest->pstSpareBufBase->nSTLMetaBase0  = (UINT32)(*pSrc16++ << 16);	
	pSrc16 += FSR_PND_SPARE_ECC_AREA;	
	pstDest->pstSpareBufBase->nSTLMetaBase0 |= *pSrc16++;

    pstDest->pstSpareBufBase->nSTLMetaBase1  = (UINT32)(*pSrc16++ << 16);
	pstDest->pstSpareBufBase->nSTLMetaBase1 |= *pSrc16++;
	
    pSrc16 += FSR_PND_SPARE_ECC_AREA;
    
    pstDest->pstSpareBufBase->nSTLMetaBase2  = (UINT32)(*pSrc16++ << 16);
	pstDest->pstSpareBufBase->nSTLMetaBase2 |= *pSrc16++;
	
	if(pstDest->nNumOfMetaExt > 0)
	{
		FSR_ASSERT(pstDest->nNumOfMetaExt == 1);
		
		/* Initialize meta extention #0 */
		FSR_OAM_MEMSET(&pstDest->pstSTLMetaExt[0], 0xFF, sizeof(FSRSpareBufExt));
		
		pstDest->pstSTLMetaExt->nSTLMetaExt0     = (UINT32)(*pSrc16++ << 16);
	    pSrc16 += FSR_PND_SPARE_ECC_AREA;	    		
	    pstDest->pstSTLMetaExt->nSTLMetaExt0    |= *pSrc16++;

		pstDest->pstSTLMetaExt->nSTLMetaExt1     = (UINT32)(*pSrc16++ << 16);		
		pstDest->pstSTLMetaExt->nSTLMetaExt1    |= *pSrc16++;		
	}
		
    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));
}



/**
 * @brief           This function writes FSRSpareBuf into spare area of Pseudo DataRAM
 *
 * @param[in]       pstPNDSpec  : pointer to PND Spec structure
 * @param[in]       pDest8      : pointer to the spare area of Pseudo DataRAM
 * @param[in]       pstSrc      : pointer to the structure, FSRSpareBuf
 * @param[in]       nFlag       : whether to write spare buffer onto DataRAM or not.
 *
 * @return          none
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          This function writes FSRSpareBuf over spare area of 1 page
 *
 */
PRIVATE VOID
_WriteSpare(PureNANDSpec   *pstPNDSpec,
            UINT8		   *pDest8,
            FSRSpareBuf    *pstSrc,
            UINT32          nFlag)
{    
    UINT16 			*pDest16;  

    FSR_STACK_VAR;

    FSR_STACK_END;	

    //FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
    //    (TEXT("[PND:IN ] ++%s(pDest: 0x%08x, pstSrc: 0x%08x)\r\n"),
    //    __FSR_FUNC__, pDest, pstSrc));        
    
    FSR_OAM_MEMSET((VOID *) pDest8, 0xFF, (size_t)(pstPNDSpec->nSctsPerPg * FSR_SPARE_SIZE));

    if (nFlag & FSR_LLD_FLAG_USE_SPAREBUF)
    {   
        pDest16 = (UINT16 *)pDest8;
        
		*pDest16++ = pstSrc->pstSpareBufBase->nBadMark;  
        
        *pDest16++ = pstSrc->pstSpareBufBase->nBMLMetaBase0;

        *pDest16++ = (UINT16) (pstSrc->pstSpareBufBase->nSTLMetaBase0 >> 16);        
         pDest16  += FSR_PND_SPARE_ECC_AREA;
        *pDest16++ = (UINT16) (pstSrc->pstSpareBufBase->nSTLMetaBase0 & 0xFFFF);   
     
        *pDest16++ = (UINT16) (pstSrc->pstSpareBufBase->nSTLMetaBase1 >> 16);
        *pDest16++ = (UINT16) (pstSrc->pstSpareBufBase->nSTLMetaBase1 & 0xFFFF);  
        
         pDest16  += FSR_PND_SPARE_ECC_AREA;

        *pDest16++ = (UINT16) (pstSrc->pstSpareBufBase->nSTLMetaBase2 >> 16);
        *pDest16++ = (UINT16) (pstSrc->pstSpareBufBase->nSTLMetaBase2 & 0xFFFF);

		if(pstSrc->nNumOfMetaExt > 0)
		{
			FSR_ASSERT(pstSrc->nNumOfMetaExt == 1);
			
	        *pDest16++ = (UINT16) (pstSrc->pstSTLMetaExt->nSTLMetaExt0 >> 16);	        
	         pDest16  += FSR_PND_SPARE_ECC_AREA;
	        *pDest16++ = (UINT16) (pstSrc->pstSTLMetaExt->nSTLMetaExt0 & 0xFFFF);     

	        *pDest16++ = (UINT16) (pstSrc->pstSTLMetaExt->nSTLMetaExt1 >> 16);
	        *pDest16++ = (UINT16) (pstSrc->pstSTLMetaExt->nSTLMetaExt1 & 0xFFFF);
		}
    }
    
    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));
}



/**
 * @brief           This function checks the validity of parameter
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nPbn        : Physical Block  Number
 * @param[in]       nPgOffset   : Page Offset within a block
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark          Pbn is consecutive numbers within the device
 * @n               i.e. for DDP device, Pbn for 1st block in 2nd chip is not 0
 * @n               it is one more than the last block number in 1st chip
 *
 */
PRIVATE INT32
_StrictChk(UINT32       nDev,
           UINT32       nPbn,
           UINT32       nPgOffset)
{
    PureNANDCxt    *pstPNDCxt;
    PureNANDSpec   *pstPNDSpec;
    INT32           nLLDRe      = FSR_LLD_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
        (TEXT("[FND:OUT] ++%s(nDev:%d, nPbn:%d, nPgOffset:%d)\r\n"),
        __FSR_FUNC__, nDev, nPbn, nPgOffset));

    do
    {
        /* Check Device Number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid Device Number (nDev = %d)\r\n"),
                nDev));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }

        pstPNDCxt  = gpstPNDCxt[nDev];
        pstPNDSpec = pstPNDCxt->pstPNDSpec;

        /* Check Device Open Flag */
        if (pstPNDCxt->bOpen == FALSE32)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Device is not opened\r\n")));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            pstPNDCxt->bOpen: 0x%08x\r\n"),
                pstPNDCxt->bOpen));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }

        /* Check Block Out of Bound */
        if (nPbn >= pstPNDSpec->nNumOfBlks)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Pbn:%d >= pstPNDSpec->nNumOfBlks:%d\r\n"),
                nPbn, pstPNDSpec->nNumOfBlks));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }
        
        /* 
         * Check nPgOffset Out of Bound
         * in case of SLC, pageOffset is lower than 64 
         */
        if (nPgOffset >= pstPNDSpec->nPgsPerBlk)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Pg #%d in Pbn (#%d) is invalid\r\n"),
                nPgOffset, nPbn));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            nPgsPerBlk = %d\r\n"),
                pstPNDSpec->nPgsPerBlk));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }
    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));

    return (nLLDRe);
}

/**
 * @brief           This function initializes PureNAND Device Driver.
 *
 * @param[in]       nFlag   : FSR_LLD_FLAG_NONE
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_ALREADY_INITIALIZED
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          It initializes internal data structure
 *
 */
PUBLIC INT32
FSR_PND_Init(UINT32 nFlag)
{
            FsrVolParm       astPAParm[FSR_MAX_VOLS];
            FSRLowFuncTbl    astLFT[FSR_MAX_VOLS];
            FSRLowFuncTbl   *apstLFT[FSR_MAX_VOLS];
            PureNANDShMem   *pstPNDShMem;
    PRIVATE BOOL32           nInitFlg					= FALSE32;
            UINT32           nVol;
            UINT32           nPDev;
            UINT32           nIdx;
            UINT32           nDie;
            UINT32           nMemAllocType;
            UINT32           nMemoryChunkID;
            UINT32           nSharedMemoryUseCnt;
            INT32            nLLDRet   					= FSR_LLD_SUCCESS;
            INT32            nPAMRet   					= FSR_PAM_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nFlag:0x%08x)\r\n"),
        __FSR_FUNC__, nFlag));

    do
    {
        if (nInitFlg == TRUE32)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF | FSR_DBZ_ERROR,
                (TEXT("[PND:INF]   %s(nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF | FSR_DBZ_ERROR,
                (TEXT("            already initialized\r\n")));

            nLLDRet = FSR_LLD_ALREADY_INITIALIZED;
            break;
        }

        /* Local data structure */
        for (nPDev = 0; nPDev < FSR_PND_MAX_DEVS; nPDev++)
        {
            gpstPNDCxt[nPDev] = NULL;
        }

        nPAMRet  = FSR_PAM_GetPAParm(astPAParm);
        if (FSR_RETURN_MAJOR(nPAMRet) != FSR_PAM_SUCCESS)
        {
            nLLDRet = FSR_LLD_PAM_ACCESS_ERROR;
            break;
        }

        for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
        {
            FSR_OAM_MEMSET(&astLFT[nVol], 0x00, sizeof(FSRLowFuncTbl));
            apstLFT[nVol] = &astLFT[nVol];
        }

        nPAMRet = FSR_PAM_RegLFT(apstLFT);
        if (nPAMRet != FSR_PAM_SUCCESS)
        {
            nLLDRet = FSR_LLD_PAM_ACCESS_ERROR;
            break;
        }

        for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
        {
            /* 
             * Because BML calls LLD_Init() by volume,
             * FSR_PND_Init() only initializes shared memory of
             * corresponding volume
             */
            if ((apstLFT[nVol] == NULL) || apstLFT[nVol]->LLD_Init != FSR_PND_Init)
            {
                continue;
            }

            if (astPAParm[nVol].bProcessorSynchronization == TRUE32)
            {
                nMemAllocType = FSR_OAM_SHARED_MEM;
            }
            else
            {
                nMemAllocType = FSR_OAM_LOCAL_MEM;
            }

            nMemoryChunkID = astPAParm[nVol].nMemoryChunkID;

            for (nIdx = 0; nIdx < astPAParm[nVol].nDevsInVol; nIdx++)
            {
                nPDev = nVol * (FSR_MAX_DEVS / FSR_MAX_VOLS) + nIdx;


                pstPNDShMem = NULL;
                
#if !defined(WITH_TINY_FSR)
                gpstPNDShMem[nPDev] = (PureNANDShMem *) NULL;
#endif

                if(gpstPNDShMem[nPDev] == NULL)
                {
                    pstPNDShMem = (PureNANDShMem *) FSR_OAM_MallocExt(nMemoryChunkID,
                                                                      sizeof(PureNANDShMem),
                                                                      nMemAllocType);
                                                                      
                    if (pstPNDShMem == NULL)
                    {
                        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                            (TEXT("[PND:ERR]   %s(nFlag:0x%08x) / %d line\r\n"),
                            __FSR_FUNC__, nFlag, __LINE__));

                        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                            (TEXT("            pstPNDShMem is NULL\r\n")));

                        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                            (TEXT("            malloc failed!\r\n")));

                        nLLDRet = FSR_LLD_MALLOC_FAIL;
                        break;
                    }

                    if (((UINT32) pstPNDShMem & (0x03)) != 0)
                    {
                        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                            (TEXT("[PND:ERR]   %s(nFlag:0x%08x) / %d line\r\n"),
                            __FSR_FUNC__, nFlag, __LINE__));

                        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                            (TEXT("            pstPNDShMem is misaligned:0x%08x\r\n"),
                            pstPNDShMem));

                        nLLDRet = FSR_OAM_NOT_ALIGNED_MEMPTR;
                        break;
                    }

                    gpstPNDShMem[nPDev] = pstPNDShMem;

                    nSharedMemoryUseCnt = pstPNDShMem->nShMemUseCnt;
                    
                    /* PreOp init of single process */
                    if (astPAParm[nVol].bProcessorSynchronization == FALSE32)
                    {
                        /* Initialize shared memory used by LLD                       */
                        for (nDie = 0; nDie < FSR_MAX_DIES; nDie++)
                        {
                            pstPNDShMem->nPreOp[nDie]          = FSR_PND_PREOP_NONE;
                            pstPNDShMem->nPreOpPbn[nDie]       = FSR_PND_PREOP_ADDRESS_NONE;
                            pstPNDShMem->nPreOpPgOffset[nDie]  = FSR_PND_PREOP_ADDRESS_NONE;
                            pstPNDShMem->nPreOpFlag[nDie]      = FSR_PND_PREOP_FLAG_NONE;
                        }
                    }
                    /* PreOp init of dual process */
                    else
                    {
                        if ((nSharedMemoryUseCnt == 0) ||
                            (nSharedMemoryUseCnt == astPAParm[nVol].nSharedMemoryInitCycle))
                        {
                            pstPNDShMem->nShMemUseCnt = 0;
                            
                            /* Initialize shared memory used by LLD                       */
                            for (nDie = 0; nDie < FSR_MAX_DIES; nDie++)
                            {
                                pstPNDShMem->nPreOp[nDie]          = FSR_PND_PREOP_NONE;
                                pstPNDShMem->nPreOpPbn[nDie]       = FSR_PND_PREOP_ADDRESS_NONE;
                                pstPNDShMem->nPreOpPgOffset[nDie]  = FSR_PND_PREOP_ADDRESS_NONE;
                                pstPNDShMem->nPreOpFlag[nDie]      = FSR_PND_PREOP_FLAG_NONE;
                            }                            
                        }
                    }
                    
                    pstPNDShMem->nShMemUseCnt++;
                }
            } /* for (nIdx = 0; nIdx < astPAParm[nVol].nDevsInVol; nIdx++) */

            if (nLLDRet != FSR_LLD_SUCCESS)
            {
                break;
            }
        } /* for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++) */

        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            break;
        }

	MSTAR_NAND_INIT();
    
        nInitFlg = TRUE32;

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}

/**
 * @brief           This function searches PureNAND device spec
 * @n               and initialize context structure.
 *
 * @param[in]       nDev    : Physical Device Number (0 ~ 3)  
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_OPEN_FAILURE
 * @return          FSR_LLD_PAM_ACCESS_ERROR 
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark
 *
 */
PRIVATE INT32
_SetDeviceInfo(UINT32 nDev)
{
    PureNANDSpec      *pstPNDSpec;
    PureNANDCxt       *pstPNDCxt;
    INT32              nLLDRet      = FSR_LLD_SUCCESS;    
    UINT32             nShift;
    UINT32             nCnt         = 0;
    UINT8              nMID; /* Manufacture ID */
    UINT8              nDID; /* Device ID      */
    UINT8 *nID;
    
    FSR_STACK_VAR;

    FSR_STACK_END;
    
    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG, (TEXT("[PND:IN ] ++%s(nDev:%d)\r\n"),
                        __FSR_FUNC__, nDev));

    do
    {
	    pstPNDCxt = gpstPNDCxt[nDev];

        nID = NC_ReadID();
	 nMID = nID[0];
	 nDID = nID[1];

        FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF,
            (TEXT("            nDev=%d, nMID=0x%04x, nDID=0x%04x\r\n"),
            nDev, nMID, nDID));

        pstPNDCxt->pstPNDSpec = NULL;
        
        for (nCnt = 0; gastPNDSpec[nCnt].nMID != 0; nCnt++)
        {
            if ((nDID == gastPNDSpec[nCnt].nDID) &&
                (nMID == gastPNDSpec[nCnt].nMID))
            {
                pstPNDCxt->pstPNDSpec = (PureNANDSpec *) &gastPNDSpec[nCnt];
                break;
            }
            /* else continue */
        }

        if (pstPNDCxt->pstPNDSpec == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d / %d line\r\n"),
                __FSR_FUNC__, nDev, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Unknown Device\r\n")));

            nLLDRet = FSR_LLD_OPEN_FAILURE;
            break;
        }

        pstPNDSpec = pstPNDCxt->pstPNDSpec;

        nShift = 0;
        while(pstPNDSpec->nPgsPerBlk != (1 << nShift) )
        {
            nShift++;
        }

        pstPNDCxt->nSftPgsPerBlk = (UINT8)nShift;
        pstPNDCxt->nMskPgsPerBlk = (UINT8)(pstPNDSpec->nPgsPerBlk -1);

        /* 
         * When the device is DDP, BML does interleve between the chips
         * To prevent this, fix nNumOfDies as 1
         */
        pstPNDCxt->nNumOfDies = FSR_PND_SUPPORTED_DIES;
        
        nShift = 0;
        while ((pstPNDSpec->nNumOfBlks/pstPNDCxt->nNumOfDies) != (1 << nShift))
        {
            nShift++;
        }
        
        pstPNDCxt->nDDPSelBaseBit = (UINT16)nShift;

	NC_Init(pstPNDSpec);
		
    } while(0);

    return (nLLDRet);
}



/**
 * @brief           This function opens PureNAND device driver 
 *
 * @param[in]       nDev    : Physical Device Number (0 ~ 3)
 * @param[in]       pParam  : pointer to structure for configuration
 * @param[in]       nFlag   :
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_OPEN_FAILURE
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_MALLOC_FAIL
 * @return          FSR_LLD_ALREADY_OPEN
 * @return          FSR_OAM_NOT_ALIGNED_MEMPTR
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_Open(UINT32         nDev,
             VOID          *pParam,
             UINT32         nFlag)
{
            PureNANDCxt        *pstPNDCxt;                  
            FsrVolParm         *pstParm;
            UINT32              nIdx            = 0;
            UINT32              nMemoryChunkID;
            INT32               nLLDRet         = FSR_LLD_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d,nFlag:0x%x)\r\n"),
        __FSR_FUNC__, nDev, nFlag));

    do
    {
    
#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid Device Number (nDev = %d)\r\n"),
                nDev));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */
        
        if (pParam == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, pParam:0x%08x, nFlag:%d) / %d line\r\n"),
                __FSR_FUNC__, nDev, pParam, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            pParam is NULL\r\n")));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }

        pstParm     = (FsrVolParm *) pParam;
        pstPNDCxt   = gpstPNDCxt[nDev];

        if (pstPNDCxt == NULL)
        {
            nMemoryChunkID = nDev / (FSR_MAX_DEVS / FSR_MAX_VOLS);

            pstPNDCxt = (PureNANDCxt *) FSR_OAM_MallocExt(nMemoryChunkID,
                                                          sizeof(PureNANDCxt),
                                                          FSR_OAM_LOCAL_MEM);

            if (pstPNDCxt == NULL)
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("[PND:ERR]   %s(nDev:%d, pParam:0x%08x, nFlag:%d) / %d line\r\n"),
                    __FSR_FUNC__, nDev, pParam, nFlag, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            pstPNDCxt is NULL\r\n")));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            malloc failed!\r\n")));

                nLLDRet = FSR_LLD_MALLOC_FAIL;
                break;
            }

            if (((UINT32) pstPNDCxt & (0x03)) != 0)
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("[PND:ERR]   %s(nDev:%d, nFlag:0x%08x) / %d line\r\n"),
                    __FSR_FUNC__, nDev, nFlag, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            pstPNDCxt is misaligned:0x%08x\r\n"),
                    pstPNDCxt));

                nLLDRet = FSR_OAM_NOT_ALIGNED_MEMPTR;
                break;
            }

            gpstPNDCxt[nDev] = pstPNDCxt;

            FSR_OAM_MEMSET(pstPNDCxt, 0x00, sizeof(PureNANDCxt));

        } /* if (pstPNDCxt == NULL) */

#if defined(FSR_LLD_STATISTICS)
        gpstPNDStat[nDev] = (PureNANDStat *) FSR_OAM_Malloc(sizeof(PureNANDStat));

        if (gpstPNDStat[nDev] == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, pParam:0x%08x, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, pParam, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            gpstPNDStat[%d] is NULL\r\n"), nDev));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            malloc failed!\r\n")));

            nLLDRet = FSR_LLD_MALLOC_FAIL;
            break;
        }

        /* If the pointer has not 4-byte align address, return not-aligned pointer. */
        if (((UINT32) gpstPNDStat[nDev] & (sizeof(UINT32) - 1)) != 0)
        {
            FSR_DBZ_DBGMOUT(FSR_DBZ_ERROR, 
                               (TEXT("[PND:ERR]   gpstPNDStat[%d] is not aligned by 4-bytes\r\n"), nDev));
            nLLDRet = FSR_OAM_NOT_ALIGNED_MEMPTR;
            break;
        }

        FSR_OAM_MEMSET(gpstPNDStat[nDev], 0x00, sizeof(PureNANDStat));
#endif

        if (pstPNDCxt->bOpen == TRUE32)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF,
                (TEXT("[PND:INF]   %s(nDev:%d, pParam:0x%08x, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, pParam, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF,
                (TEXT("[PND:INF]   dev:%d is already open\r\n"), nDev));

            nLLDRet = FSR_LLD_ALREADY_OPEN;
            break;
        }

        /* Base address setting */
        nIdx = nDev & (FSR_MAX_DEVS/FSR_MAX_VOLS - 1);

        if (pstParm->nBaseAddr[nIdx] != FSR_PAM_NOT_MAPPED)
        {
            pstPNDCxt->nBaseAddr = pstParm->nBaseAddr[nIdx];

            FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF,
                (TEXT("[PND:INF]   %s(nDev:%d, pParam:0x%08x, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, pParam, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF,
                (TEXT("            pstPNDCxt->nBaseAddr: 0x%08x\r\n"),
                pstPNDCxt->nBaseAddr));
        }
        else
        {
            pstPNDCxt->nBaseAddr = FSR_PAM_NOT_MAPPED;
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, pParam:0x%08x, nFlag:0x%08x / %d line\r\n"),
                __FSR_FUNC__, nDev, pParam, nFlag, __LINE__));
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            PureNAND is not mapped\r\n")));
            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
	
        /* Set device info */
        nLLDRet = _SetDeviceInfo(nDev);
        if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
        {            
            break;
        }

        nIdx                    = nDev & (FSR_MAX_DEVS / FSR_MAX_VOLS -1);
        pstPNDCxt->nIntID       = pstParm->nIntID[nIdx];
        if ((pstPNDCxt->nIntID != FSR_INT_ID_NONE) && (pstPNDCxt->nIntID > FSR_INT_ID_NAND_7))
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:INF]   nIntID is out of range(nIntID:%d)\r\n"), pstPNDCxt->nIntID));
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:INF]   FSR_INT_ID_NAND_0(%d) <= nIntID <= FSR_INT_ID_NAND_7(%d)\r\n"),
                FSR_INT_ID_NAND_0, FSR_INT_ID_NAND_7));
            nLLDRet = FSR_LLD_INVALID_PARAM;            
            break;
        }

        FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF,
            (TEXT("[PND:INF]   pstPNDCxt->nIntID   :0x%04x / %d line\r\n"),
            pstPNDCxt->nIntID, __LINE__));
        
        pstPNDCxt->bOpen      = TRUE32;
        
#if defined (FSR_LLD_STATISTICS)
        gnDevsInVol[nDev / (FSR_MAX_DEVS / FSR_MAX_VOLS)]++;
#endif

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}


/**
 * @brief           This function closes Flex-OneNAND device driver
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nFlag       :
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_Close(UINT32         nDev,
              UINT32         nFlag)
{
    PureNANDCxt      *pstPNDCxt;
    UINT32            nMemoryChunkID;
    INT32             nLLDRet           = FSR_LLD_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d, nFlag:0x%08x)\r\n"),
        __FSR_FUNC__, nDev, nFlag));

    /* Note: Here LLD doesn't flush the previous operation, for BML flushes */
    
    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid Device Number (nDev = %d)\r\n"),
                nDev));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDCxt = gpstPNDCxt[nDev];

        if (pstPNDCxt != NULL)
        {
            pstPNDCxt->bOpen      = FALSE32;
            pstPNDCxt->pstPNDSpec = NULL;

            nMemoryChunkID = nDev / (FSR_MAX_DEVS / FSR_MAX_VOLS);

            FSR_OAM_FreeExt(nMemoryChunkID, pstPNDCxt, FSR_OAM_LOCAL_MEM);
            
            gpstPNDCxt[nDev] = NULL;
            
#if defined (FSR_LLD_STATISTICS)
            gnDevsInVol[nDev / (FSR_MAX_DEVS / FSR_MAX_VOLS)]--;
#endif

        }

#if defined(FSR_LLD_STATISTICS)
        if (gpstPNDStat[nDev] != NULL)
        {
            FSR_OAM_Free(gpstPNDStat[nDev]);
            gpstPNDStat[nDev] = NULL;
        }
#endif
        
    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function reads 1 page from PureNAND
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nPbn        : Physical Block  Number
 * @param[in]       nPgOffset   : Page Offset within a block
 * @param[out]      pMBuf       : Memory buffer for main  array of NAND flash
 * @param[out]      pSBuf       : Memory buffer for spare array of NAND flash
 * @param[in]       nFlag       : Operation options such as ECC_ON, OFF
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_PREV_READ_ERROR       | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               previous read error return value 
 * @return          FSR_LLD_PREV_2LV_READ_DISTURBANCE | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               previous read disturbance error return value
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          One function call of FSR_PND_Read() loads 1 page from
 * @n               PureNAND and moves data from DataRAM to pMBuf, pSBuf.
 *
 */
PUBLIC INT32
FSR_PND_Read(UINT32         nDev,
             UINT32         nPbn,
             UINT32         nPgOffset,
             UINT8         *pMBuf,
             FSRSpareBuf   *pSBuf,
             UINT32         nFlag)
{
    INT32       nLLDRet = FSR_LLD_SUCCESS;
    UINT32      nLLDFlag;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s()\r\n"), __FSR_FUNC__));

    do
    {
        nLLDFlag = ~((UINT32)FSR_LLD_FLAG_CMDIDX_MASK) & nFlag;
        
	nLLDRet  = FSR_PND_ReadOptimal(nDev,
                                       nPbn, nPgOffset,
                                       pMBuf, pSBuf,
                                       FSR_LLD_FLAG_1X_LOAD | nLLDFlag);
        if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
        {
            break;
	 }
		
        nLLDRet  = FSR_PND_ReadOptimal(nDev,
                                       nPbn, nPgOffset,
                                       pMBuf, pSBuf,
                                       FSR_LLD_FLAG_TRANSFER | nLLDFlag);
        if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
        {
            break;
        }
    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return nLLDRet;
}



/**
 * @brief           This function reads 1 page from PureNAND
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ )
 * @param[in]       nPbn        : Physical Block  Number
 * @param[in]       nPgOffset   : Page Offset within a block
 * @param[out]      pMBuf       : Memory buffer for main  array of NAND flash
 * @param[out]      pSBuf       : Memory buffer for spare array of NAND flash
 * @param[in]       nFlag       : Operation options such as ECC_ON, OFF
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_PREV_READ_ERROR       | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               previous read error return value
 * @return          FSR_LLD_PREV_2LV_READ_DISTURBANCE | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               previous read disturbance error return value
 *
 * @author          Jinho Yi
 * @version         1.2.1
 *
 */
PUBLIC INT32
FSR_PND_ReadOptimal(UINT32         nDev,
                    UINT32         nPbn,
                    UINT32         nPgOffset,
                    UINT8         *pMBufOrg,
                    FSRSpareBuf   *pSBufOrg,
                    UINT32         nFlag)
{
    PureNANDCxt      *pstPNDCxt;
    PureNANDSpec     *pstPNDSpec;
    PureNANDShMem    *pstPNDShMem;

    UINT32            nCmdIdx;

    /* Start sector offset from the start */
    UINT32            nStartOffset;

    /* End sector offset from the end     */
    UINT32            nEndOffset;
    UINT32            nDie;
    UINT32            nFlushOpCaller;
    UINT32            nRowAddress;
    UINT32            nColAddress      = 0;    
    INT32             nLLDRet          = FSR_LLD_SUCCESS;
    UINT8            *pMainDataRAM;
    UINT8            *pSpareDataRAM;
    
#if defined(FSR_LLD_STATISTICS)
    UINT32            nBytesTransferred = 0;
#endif

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
    (TEXT("[PND:IN ] ++%s(nDev:%d, nPbn:%d, nPgOffset:%d, pMBuf:0x%08x, pSBuf:0x%08x, nFlag:0x%08x)\r\n"),
    __FSR_FUNC__ , nDev, nPbn, nPgOffset, pMBufOrg, pSBufOrg, nFlag));
	
    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        nLLDRet = _StrictChk(nDev, nPbn, nPgOffset);
        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */
    
        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDSpec  = pstPNDCxt->pstPNDSpec;
        pstPNDShMem = gpstPNDShMem[nDev];
             
        nCmdIdx = (nFlag & FSR_LLD_FLAG_CMDIDX_MASK) >> FSR_LLD_FLAG_CMDIDX_BASEBIT;

        nDie    = nPbn >> pstPNDCxt->nDDPSelBaseBit;

        nFlushOpCaller = FSR_PND_PREOP_READ << FSR_PND_FLUSHOP_CALLER_BASEBIT;
        
        pMainDataRAM    = pstPNDShMem->pPseudoMainDataRAM;
        pSpareDataRAM   = pstPNDShMem->pPseudoSpareDataRAM;

        /* Transfer data */
        if (nFlag & FSR_LLD_FLAG_TRANSFER)
        {  

            nLLDRet = FSR_PND_FlushOp(nDev, nFlushOpCaller | nDie, nFlag);
           
            nStartOffset = (nFlag & FSR_LLD_FLAG_1ST_SCTOFFSET_MASK)
                            >> FSR_LLD_FLAG_1ST_SCTOFFSET_BASEBIT;

            nEndOffset   = (nFlag & FSR_LLD_FLAG_LAST_SCTOFFSET_MASK)
                            >> FSR_LLD_FLAG_LAST_SCTOFFSET_BASEBIT;

            if (pMBufOrg != NULL)
            {
                FSR_OAM_MEMCPY(pMBufOrg + nStartOffset *  FSR_SECTOR_SIZE,
                               pMainDataRAM + nStartOffset *  FSR_SECTOR_SIZE,
                               (pstPNDSpec->nSctsPerPg - nStartOffset - nEndOffset) *  FSR_SECTOR_SIZE);
                               
#if defined (FSR_LLD_STATISTICS)
                nBytesTransferred +=   FSR_SECTOR_SIZE *
                    (pstPNDSpec->nSctsPerPg - nStartOffset - nEndOffset);
#endif /* #if defined (FSR_LLD_STATISTICS) */

            }        

            if ((pSBufOrg != NULL) && (nFlag & FSR_LLD_FLAG_USE_SPAREBUF))
            {
                if ((nFlag & FSR_LLD_FLAG_DUMP_MASK) == FSR_LLD_FLAG_DUMP_OFF)
                {
                    _ReadSpare(pSBufOrg,
                               pSpareDataRAM);
                }
                else
                {
                	FSR_OAM_MEMCPY(pSBufOrg + nStartOffset * FSR_SPARE_SIZE,
                                   pSpareDataRAM + nStartOffset * FSR_SPARE_SIZE,
                                  (pstPNDSpec->nSctsPerPg - nStartOffset - nEndOffset) * FSR_SPARE_SIZE);                    
                }
                
#if defined (FSR_LLD_STATISTICS)
                if ((nFlag & FSR_LLD_FLAG_DUMP_MASK) == FSR_LLD_FLAG_DUMP_OFF)
                {
                    nBytesTransferred += (FSR_SPARE_BUF_SIZE_2KB_PAGE);
                }
                else
                {
                    nBytesTransferred +=
                        pstPNDSpec->nSctsPerPg * pstPNDSpec->nSparePerSct;
                }
#endif /* #if defined (FSR_LLD_STATISTICS) */

            }
            
            pstPNDShMem->nPreOp[nDie] = FSR_PND_PREOP_NONE;
        } /* if (nFlag & FSR_LLD_FLAG_TRANSFER) */

        if ((nCmdIdx == FSR_LLD_FLAG_1X_LOAD) ||
            (nCmdIdx == FSR_LLD_FLAG_1X_PLOAD))
        {

            nRowAddress  = nPbn << pstPNDCxt->nSftPgsPerBlk;
			nRowAddress |= (nPgOffset & pstPNDCxt->nMskPgsPerBlk);

            if ((nFlag & FSR_LLD_FLAG_ECC_MASK) == FSR_LLD_FLAG_ECC_ON)
            {
                _SetNANDCtrllerCmd(nDev, nDie, INVALID_ADDRESS, INVALID_ADDRESS, FSR_PND_SET_ECC_ON);
            }
            else
            {
                _SetNANDCtrllerCmd(nDev, nDie, INVALID_ADDRESS, INVALID_ADDRESS, FSR_PND_SET_ECC_OFF);
            }
        	pstPNDShMem->ECCnRE =  (UINT32)_SetNANDCtrllerCmd(nDev, nDie, nRowAddress, nColAddress, FSR_PND_SET_READ_PAGE);

            pstPNDShMem->nPreOp[nDie] = FSR_PND_PREOP_READ;
            
#if defined (FSR_LLD_STATISTICS)
            _AddPNDStat(nDev, nDie, FSR_PND_STAT_RD_TRANS, nBytesTransferred, FSR_PND_STAT_PLOAD);
#endif /* #if defined (FSR_LLD_STATISTICS) */

        } /* if ((nCmdIdx == FSR_LLD_FLAG_1X_LOAD) | (nCmdIdx == FSR_LLD_FLAG_1X_PLOAD)) */

        pstPNDShMem->nPreOpPbn[nDie]      = (UINT16) nPbn;
        pstPNDShMem->nPreOpPgOffset[nDie] = (UINT16) nPgOffset;
        pstPNDShMem->nPreOpFlag[nDie]     = nFlag;
        
#if defined (FSR_LLD_LOGGING_HISTORY)
        _AddLog(nDev, nDie);
#endif

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function writes data into PureNAND
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nPbn        : Physical Block  Number
 * @param[in]       nPgOffset   : Page Offset within a block
 * @param[in]       pMBufOrg    : Memory buffer for main  array of NAND flash
 * @param[in]       pSBufOrg    : Memory buffer for spare array of NAND flash
 * @param[in]       nFlag       : Operation options such as ECC_ON, OFF
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_PREV_WRITE_ERROR  | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               Error for normal program 
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_Write(UINT32       nDev,
              UINT32       nPbn,
              UINT32       nPgOffset,
              UINT8       *pMBufOrg,
              FSRSpareBuf *pSBufOrg,
              UINT32       nFlag)
{
    PureNANDCxt      *pstPNDCxt;
    PureNANDSpec     *pstPNDSpec;
    PureNANDShMem    *pstPNDShMem;
    UINT32            nBBMMetaIdx;
    UINT32            nBadMarkIdx;
    UINT32            nDie;
    UINT32            nFlushOpCaller;
    UINT32            nRowAddress;
    UINT32            nColAddress;
    UINT32            nMainSize;
    INT32             nLLDRet          = FSR_LLD_SUCCESS;
    UINT8            *pMainDataRAM;
    UINT8            *pSpareDataRAM;
    
#if defined(FSR_LLD_STATISTICS)
    UINT32            nBytesTransferred = 0;
#endif

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
    (TEXT("[PND:IN ] ++%s(nDev:%d, nPbn:%d, nPgOffset:%d, pMBufOrg: 0x%08x, pSBufOrg: 0x%08x, nFlag:0x%08x)\r\n"),
    __FSR_FUNC__, nDev, nPbn, nPgOffset, pMBufOrg, pSBufOrg, nFlag));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        nLLDRet = _StrictChk(nDev, nPbn, nPgOffset);

        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDSpec  = pstPNDCxt->pstPNDSpec;
        pstPNDShMem = gpstPNDShMem[nDev];

        nDie = nPbn >> pstPNDCxt->nDDPSelBaseBit;

        nRowAddress  = nPbn << pstPNDCxt->nSftPgsPerBlk;
        nRowAddress |= (nPgOffset & pstPNDCxt->nMskPgsPerBlk);

        nMainSize = (UINT32)pstPNDSpec->nSctsPerPg *  (UINT32)FSR_SECTOR_SIZE;
        
        nColAddress = 0;

        /* Default setting */
        pMainDataRAM    = pstPNDShMem->pPseudoMainDataRAM;
        pSpareDataRAM   = pstPNDShMem->pPseudoSpareDataRAM;

        /* Write Data into DataRAM */
        if (pMBufOrg != NULL)
        {            

#if defined (FSR_LLD_STATISTICS)
            nBytesTransferred += nMainSize;
#endif        

            FSR_OAM_MEMCPY(pMainDataRAM, pMBufOrg, nMainSize);
        }
        else if((pMBufOrg == NULL) && (pSBufOrg != NULL))
        {
            /* For bad handling */
            FSR_OAM_MEMSET(pMainDataRAM, 0xFF, nMainSize);
        }

        if (pSBufOrg != NULL)
        {
            if ((nFlag & FSR_LLD_FLAG_DUMP_MASK) == FSR_LLD_FLAG_DUMP_ON)
            {
                pSpareDataRAM = (UINT8 *) pSBufOrg;
            }
            else
            {
                /* 
                 * If FSR_LLD_FLAG_BBM_META_BLOCK of nFlag is set,
                 * write nBMLMetaBase0 of FSRSpareBufBase with 0xA5A5
                 */
                nBBMMetaIdx = (nFlag & FSR_LLD_FLAG_BBM_META_MASK) >>
                              FSR_LLD_FLAG_BBM_META_BASEBIT;
                              
                nBadMarkIdx = (nFlag & FSR_LLD_FLAG_BADMARK_MASK) >> 
                                    FSR_LLD_FLAG_BADMARK_BASEBIT;

                pSBufOrg->pstSpareBufBase->nBadMark      = gnBadMarkValue[nBadMarkIdx];
                pSBufOrg->pstSpareBufBase->nBMLMetaBase0 = gnBBMMetaValue[nBBMMetaIdx];

                /* 
                 * _WriteSpare does not care about bad mark which is written at
                 * the first word of sector 0 of the spare area 
                 * bad mark is written individually.
                 */                
                _WriteSpare(pstPNDSpec, pSpareDataRAM, pSBufOrg, nFlag);
            }
        }

        if (pMBufOrg != NULL && pSBufOrg == NULL)
        {
            if((nFlag & FSR_LLD_FLAG_BACKUP_DATA) != FSR_LLD_FLAG_BACKUP_DATA)
            {
            	FSR_OAM_MEMSET(pSpareDataRAM, 0xFF, (size_t)(pstPNDSpec->nSctsPerPg * FSR_SPARE_SIZE));
            }
        }
        
        if ((nFlag & FSR_LLD_FLAG_DUMP_MASK) == FSR_LLD_FLAG_DUMP_OFF)
        {
            /* 
             * Write bad mark of the block
             * bad mark is not written in _WriteSpare()
             */
            *((UINT16 *)pSpareDataRAM) = 
                gnBadMarkValue[(nFlag & FSR_LLD_FLAG_BADMARK_MASK) >> FSR_LLD_FLAG_BADMARK_BASEBIT];
        }

#if defined (FSR_LLD_STATISTICS)
        nBytesTransferred += (pstPNDSpec->nSctsPerPg * pstPNDSpec->nSparePerSct);
#endif /* #if defined (FSR_LLD_STATISTICS) */
        
        if ((nFlag & FSR_LLD_FLAG_ECC_MASK) == FSR_LLD_FLAG_ECC_ON)
        {
            _SetNANDCtrllerCmd(nDev, nDie, INVALID_ADDRESS, INVALID_ADDRESS, FSR_PND_SET_ECC_ON);
        }
        else
        {
            _SetNANDCtrllerCmd(nDev, nDie, INVALID_ADDRESS, INVALID_ADDRESS, FSR_PND_SET_ECC_OFF);
        }
        pstPNDShMem->ECCnRE = (UINT32)_SetNANDCtrllerCmd(nDev, nDie, nRowAddress, nColAddress, FSR_PND_SET_PROGRAM_PAGE);

        nFlushOpCaller = FSR_PND_PREOP_PROGRAM << FSR_PND_FLUSHOP_CALLER_BASEBIT;

	 pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_PROGRAM;
        pstPNDShMem->nPreOpPbn[nDie]      = (UINT16) nPbn;
        pstPNDShMem->nPreOpPgOffset[nDie] = (UINT16) nPgOffset;
        pstPNDShMem->nPreOpFlag[nDie]     = nFlag;

        nLLDRet = FSR_PND_FlushOp(nDev, nFlushOpCaller | nDie, nFlag);
        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            break;
        }
        
#if defined (FSR_LLD_LOGGING_HISTORY)
        _AddLog(nDev, nDie);
#endif

#if defined (FSR_LLD_STATISTICS)
        _AddPNDStat(nDev, nDie, FSR_PND_STAT_WR_TRANS, nBytesTransferred, FSR_PND_STAT_CACHE_PGM);
        _AddPNDStat(nDev, nDie, FSR_PND_STAT_SLC_PGM, 0, FSR_PND_STAT_CACHE_PGM);
#endif /* #if defined (FSR_LLD_STATISTICS) */

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function erase a block of PureNAND
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       pnPbn       : array of blocks, not necessarilly consecutive.
 * @n                             multi block erase will be supported in the future
 * @param[in]       nFlag       : FSR_LLD_FLAG_1X_ERASE
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_PREV_ERASE_ERROR | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               erase error for previous erase operation
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          As of now, supports only single plane, one block erase
 * @                Pbn is consecutive numbers within the device
 * @n               i.e. for DDP device, Pbn for 1st block in 2nd chip is not 0
 * @n               it is one more than the last block number in 1st chip
 *
 */
PUBLIC INT32
FSR_PND_Erase(UINT32  nDev,
              UINT32 *pnPbn,
              UINT32  nNumOfBlks,
              UINT32  nFlag)
{
    PureNANDCxt    *pstPNDCxt;
    PureNANDShMem  *pstPNDShMem;
    UINT32          nDie;
    UINT32          nPbn;    
    UINT32          nRowAddress;
    INT32           nLLDRet;

    do
    {
    
#if defined (FSR_LLD_STRICT_CHK)
        if (pnPbn == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, pnPbn:0x%08x, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, pnPbn, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            pnPbn is NULL\r\n")));
                
            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
        
        nLLDRet = _StrictChk(nDev, *pnPbn, 0);
        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDShMem = gpstPNDShMem[nDev];
        
        nPbn = *pnPbn;
        nDie = nPbn >> pstPNDCxt->nDDPSelBaseBit;

        nLLDRet = FSR_PND_FlushOp(nDev, nDie, nFlag);
        if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
        {
            break;
        }

        nRowAddress = nPbn << pstPNDCxt->nSftPgsPerBlk;

        pstPNDShMem->ECCnRE = (UINT32)_SetNANDCtrllerCmd(nDev, nDie, nRowAddress, INVALID_ADDRESS, FSR_PND_SET_ERASE);

        pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_ERASE;
        pstPNDShMem->nPreOpPbn[nDie]      = (UINT16) nPbn;
        pstPNDShMem->nPreOpPgOffset[nDie] = FSR_PND_PREOP_ADDRESS_NONE;
        pstPNDShMem->nPreOpFlag[nDie]     = nFlag;

#if defined (FSR_LLD_LOGGING_HISTORY)
        _AddLog(nDev, nDie);
#endif

#if defined (FSR_LLD_STATISTICS)
        _AddPNDStat(nDev, nDie, FSR_PND_STAT_ERASE, 0, FSR_PND_STAT_NORMAL_CMD);
#endif /* #if defined (FSR_LLD_STATISTICS) */

    } while(0);

    return (nLLDRet);
}



/**
 * @brief           This function copybacks 1 page.
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       pstCpArg    : pointer to the structure LLDCpBkArg 
 * @param[in]       nFlag       : FSR_LLD_FLAG_1X_CPBK_LOAD,
 * @n                             FSR_LLD_FLAG_1X_CPBK_PROGRAM
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_PREV_READ_ERROR       | {FSR_LLD_1STPLN_CURR_ERROR}
 * @n               uncorrectable read error for previous read operation
 * @return          FSR_LLD_PREV_2LV_READ_DISTURBANCE | {FSR_LLD_1STPLN_CURR_ERROR}
 * @return          FSR_LLD_PREV_WRITE_ERROR      | {FSR_LLD_1STPLN_PREV_ERROR}
 * @n               write error for previous write operation
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          This function loads data from PureNAND to Pseudo DataRAM,
 * @n               and randomly writes data into the DataRAM
 * @n               and program back into PureNAND
 *
 */
PUBLIC INT32
FSR_PND_CopyBack(UINT32         nDev,
                 LLDCpBkArg    *pstCpArg,
                 UINT32         nFlag)
{
    PureNANDCxt      *pstPNDCxt;
    PureNANDSpec     *pstPNDSpec;
    PureNANDShMem    *pstPNDShMem;

    LLDRndInArg      *pstRIArg; /* Random in argument */

    UINT32            nCmdIdx;
    UINT32            nCnt;
    UINT32            nPgOffset;
    UINT32            nDie;
    UINT32            nPbn;
    UINT32            nRndOffset;
    UINT32            nFlushOpCaller;
    UINT32            nRowAddress;    

    BOOL32            bSpareBufRndIn;
    FSRSpareBufBase   stSpareBufBase;
    FSRSpareBuf       stSpareBuf;
    FSRSpareBufExt    stSpareBufExt[FSR_MAX_SPARE_BUF_EXT];

    INT32             nLLDRet           = FSR_LLD_SUCCESS;

    UINT8            *pMainDataRAM;
    UINT8            *pSpareDataRAM;

#if defined (FSR_LLD_STATISTICS)
    UINT32            nBytesTransferred = 0;
#endif /* #if defined (FSR_LLD_STATISTICS) */

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d, nFlag:0x%08x)\r\n"),
        __FSR_FUNC__, nDev, nFlag));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        if (pstCpArg == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, pstCpArg:0x%08x, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, pstCpArg, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            pstCpArg is NULL\r\n")));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
        
        /* Check Device Number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid Device Number (nDev = %d)\r\n"),
                nDev));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDSpec  = pstPNDCxt->pstPNDSpec;
        pstPNDShMem = gpstPNDShMem[nDev];
        
        nCmdIdx = (nFlag & FSR_LLD_FLAG_CMDIDX_MASK) >> FSR_LLD_FLAG_CMDIDX_BASEBIT;

        pMainDataRAM  = pstPNDShMem->pPseudoMainDataRAM;
        pSpareDataRAM = pstPNDShMem->pPseudoSpareDataRAM;

        if (nCmdIdx == FSR_LLD_FLAG_1X_CPBK_LOAD)
        {
            FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_INF,
                (TEXT("[PND:INF]   %s(nDev:%d, nSrcPbn:%d, nSrcPg:%d, nFlag:0x%08x)\r\n"),
                __FSR_FUNC__, nDev, pstCpArg->nSrcPbn, pstCpArg->nSrcPgOffset, nFlag));

            /* 
             * Load phase of copyback() only checks the source block & page
             * offset. For BML does not fill the destination block & page
             * offset at this phase
             */
            nPbn      = pstCpArg->nSrcPbn;
            nPgOffset = pstCpArg->nSrcPgOffset;

#if defined (FSR_LLD_STRICT_CHK)
            nLLDRet = _StrictChk(nDev, nPbn, nPgOffset);
            if (nLLDRet != FSR_LLD_SUCCESS)
            {
                break;
            }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

            nDie = nPbn >> pstPNDCxt->nDDPSelBaseBit;

            nFlushOpCaller = FSR_PND_PREOP_CPBK_LOAD << FSR_PND_FLUSHOP_CALLER_BASEBIT;

            /* 
             * eNAND doesn't support read interleaving,
             * therefore, wait until interrupt status is ready for both die.
             * if device is DDP. 
             */
            if (pstPNDCxt->nNumOfDies == FSR_MAX_DIES)
            {
                FSR_PND_FlushOp(nDev,
                                nFlushOpCaller | ((nDie + 0x1) & 0x1),
                                nFlag | FSR_LLD_FLAG_REMAIN_PREOP_STAT);
            }
            
            nLLDRet = FSR_PND_FlushOp(nDev, nFlushOpCaller | nDie, nFlag);
            if ((FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS) &&
                (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_PREV_2LV_READ_DISTURBANCE))
            {
                break;
            }            

            nRowAddress  = nPbn << pstPNDCxt->nSftPgsPerBlk;
            nRowAddress |= (nPgOffset & pstPNDCxt->nMskPgsPerBlk);

            if ((nFlag & FSR_LLD_FLAG_ECC_MASK) == FSR_LLD_FLAG_ECC_ON)
            {
                _SetNANDCtrllerCmd(nDev, nDie, INVALID_ADDRESS, INVALID_ADDRESS, FSR_PND_SET_ECC_ON);
            }
            else
            {
                _SetNANDCtrllerCmd(nDev, nDie, INVALID_ADDRESS, INVALID_ADDRESS, FSR_PND_SET_ECC_OFF);
            }
            pstPNDShMem->ECCnRE = (UINT32)_SetNANDCtrllerCmd(nDev, nDie, nRowAddress, 0, FSR_PND_SET_READ_PAGE);
            
            /* Store the type of previous operation for the deferred check */
            pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_CPBK_LOAD;
            pstPNDShMem->nPreOpPbn[nDie]      = (UINT16) nPbn;
            pstPNDShMem->nPreOpPgOffset[nDie] = (UINT16) nPgOffset;
            pstPNDShMem->nPreOpFlag[nDie]     = nFlag;
            
#if defined (FSR_LLD_LOGGING_HISTORY)
            _AddLog(nDev, nDie);
#endif
            
#if defined (FSR_LLD_STATISTICS)
            _AddPNDStat(nDev, nDie, FSR_PND_STAT_SLC_LOAD, 0, FSR_PND_STAT_NORMAL_CMD);
#endif /* #if defined (FSR_LLD_STATISTICS) */

        }
        else if (nCmdIdx == FSR_LLD_FLAG_1X_CPBK_PROGRAM)
        {
            FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_INF,
            (TEXT("[PND:INF]   %s(nDev:%d, nDstPbn:%d, nDstPg:%d, nRndInCnt:%d, nFlag:0x%08x)\r\n"),
            __FSR_FUNC__, nDev, pstCpArg->nDstPbn, pstCpArg->nDstPgOffset, pstCpArg->nRndInCnt, nFlag));

            nPbn      = pstCpArg->nDstPbn;
            nPgOffset = pstCpArg->nDstPgOffset;
        
#if defined (FSR_LLD_STRICT_CHK)
            nLLDRet = _StrictChk(nDev, nPbn, nPgOffset);
            if (nLLDRet != FSR_LLD_SUCCESS)
            {
                break;
            }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

            nDie = nPbn >> pstPNDCxt->nDDPSelBaseBit;

            nFlushOpCaller = FSR_PND_PREOP_CPBK_PGM << FSR_PND_FLUSHOP_CALLER_BASEBIT;

            nLLDRet = FSR_PND_FlushOp(nDev, nFlushOpCaller | nDie, nFlag);

            if ((FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS) &&
                (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_PREV_2LV_READ_DISTURBANCE))
            {
                break;
            }

            bSpareBufRndIn = FALSE32;
            for (nCnt = 0; nCnt < pstCpArg->nRndInCnt; nCnt++)
            {
                pstRIArg = pstCpArg->pstRndInArg + nCnt;
                if (pstRIArg->nOffset >= FSR_LLD_CPBK_SPARE)
                {
                    bSpareBufRndIn = TRUE32;

                    stSpareBuf.pstSpareBufBase  = &stSpareBufBase;
                    stSpareBuf.nNumOfMetaExt    = 1;
                    stSpareBuf.pstSTLMetaExt    = &stSpareBufExt[0];                   
                    _ReadSpare(&stSpareBuf,
                               pSpareDataRAM);
                    break;
                }
            }

            for (nCnt = 0; nCnt < pstCpArg->nRndInCnt; nCnt++)
            {
	         pstRIArg = pstCpArg->pstRndInArg + nCnt;

                /* In case copyback of spare area is requested */
                if (pstRIArg->nOffset >= FSR_LLD_CPBK_SPARE)
                {
                    nRndOffset = (UINT32)pstRIArg->nOffset - (UINT32)FSR_LLD_CPBK_SPARE;

                    if (nRndOffset >= (FSR_SPARE_BUF_BASE_SIZE))
                    {
                        /* Random-in to FSRSpareBufExt[] */
                        nRndOffset -= (FSR_SPARE_BUF_BASE_SIZE * 1);

                        FSR_ASSERT((pstRIArg->nNumOfBytes + nRndOffset) <= (FSR_SPARE_BUF_EXT_SIZE * FSR_MAX_SPARE_BUF_EXT));

                        /* Random-in to FSRSpareBuf */
                    
                        FSR_OAM_MEMCPY((UINT8 *) &(stSpareBuf.pstSTLMetaExt[0]) + nRndOffset,
                                       (UINT8 *) pstRIArg->pBuf,
                                        pstRIArg->nNumOfBytes);  
                    }
                    else                  
                    {
                        FSR_ASSERT((pstRIArg->nNumOfBytes + nRndOffset) <= FSR_SPARE_BUF_BASE_SIZE);

                        /* Random-in to FSRSpareBuf */
                        FSR_OAM_MEMCPY((UINT8 *) stSpareBuf.pstSpareBufBase + nRndOffset,
                                       (UINT8 *) pstRIArg->pBuf,
                                        pstRIArg->nNumOfBytes);  
                    }
                }
                else
                {
                    FSR_OAM_MEMCPY(pMainDataRAM + pstRIArg->nOffset,
                                   (UINT8 *) pstRIArg->pBuf,
                                   pstRIArg->nNumOfBytes);
                  
#if defined (FSR_LLD_STATISTICS)
                    nBytesTransferred += pstRIArg->nNumOfBytes;
#endif /* #if defined (FSR_LLD_STATISTICS) */                 

                }
            }

            if (bSpareBufRndIn == TRUE32)
            {
               _WriteSpare(pstPNDSpec,
                            pSpareDataRAM,
                            &stSpareBuf,
                            nFlag | FSR_LLD_FLAG_USE_SPAREBUF);

#if defined (FSR_LLD_STATISTICS)
                nBytesTransferred += FSR_SPARE_BUF_SIZE_2KB_PAGE;
#endif /* #if defined (FSR_LLD_STATISTICS) */

            }

            nRowAddress  = nPbn << pstPNDCxt->nSftPgsPerBlk;
            nRowAddress |= nPgOffset & pstPNDCxt->nMskPgsPerBlk;

            /* Set row & column address */
           pstPNDShMem->ECCnRE =  (UINT32)_SetNANDCtrllerCmd(nDev, nDie, nRowAddress, 0, FSR_PND_SET_PROGRAM_PAGE);

            /* Store the type of previous operation for the deferred check */
            pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_CPBK_PGM;
            pstPNDShMem->nPreOpPbn[nDie]      = (UINT16) nPbn;
            pstPNDShMem->nPreOpPgOffset[nDie] = (UINT16) nPgOffset;
            pstPNDShMem->nPreOpFlag[nDie]     = nFlag;
            
#if defined (FSR_LLD_STATISTICS)
            _AddPNDStat(nDev, nDie, FSR_PND_STAT_WR_TRANS, nBytesTransferred, FSR_PND_STAT_NORMAL_CMD);
            _AddPNDStat(nDev, nDie, FSR_PND_STAT_SLC_PGM, 0, FSR_PND_STAT_NORMAL_CMD);
#endif /* #if defined (FSR_LLD_STATISTICS) */

#if defined (FSR_LLD_LOGGING_HISTORY)
            _AddLog(nDev, nDie);
#endif

        }
        


    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function checks whether block is bad or not
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nPbn        : physical block number
 * @param[in]       nFlag       : FSR_LLD_FLAG_1X_CHK_BADBLOCK
 *
 * @return          FSR_LLD_INIT_GOODBLOCK
 * @return          FSR_LLD_INIT_BADBLOCK | {FSR_LLD_BAD_BLK_1STPLN}
 * @return          FSR_LLD_INVALID_PARAM
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          Pbn is consecutive numbers within the device
 * @n               i.e. for DDP device, Pbn for 1st block in 2nd chip is not 0
 * @n               it is one more than the last block number in 1st chip
 *
 */
PUBLIC INT32
FSR_PND_ChkBadBlk(UINT32         nDev,
                  UINT32         nPbn,
                  UINT32         nFlag)
{
    INT32             nLLDRet = FSR_LLD_SUCCESS;
    UINT8            nDQF, nDQL;
    PureNANDCxt      *pstPNDCxt;
    PureNANDShMem    *pstPNDShMem;
    UINT32            nDie;

    FSR_STACK_VAR;

    FSR_STACK_END;
    

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d, nPbn:%d, nFlag:0x%08x)\r\n"),
        __FSR_FUNC__, nDev, nPbn, nFlag));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        nLLDRet = _StrictChk(nDev, nPbn, 0);
        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nPbn:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nPbn, nFlag, __LINE__));            
            break;
        }
#endif

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDShMem = gpstPNDShMem[nDev];
        nDie        = nPbn >> pstPNDCxt->nDDPSelBaseBit;

        nLLDRet = FSR_PND_ReadOptimal(nDev,  
                                      nPbn,                  
                                      (UINT32) 0, 
                                      (UINT8 *) NULL,
                                      (FSRSpareBuf *) NULL,   
                                      (UINT32) (FSR_LLD_FLAG_ECC_OFF | FSR_LLD_FLAG_1X_LOAD));
        if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
        {
            break;
        }

        nLLDRet = FSR_PND_FlushOp(nDev, nDie, nFlag);

        if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
        {
        	break;
        }
    
		nDQF = *(pstPNDShMem->pPseudoSpareDataRAM);

		nLLDRet = FSR_PND_ReadOptimal(nDev,
				nPbn,
				(UINT32) 0,
				(UINT8 *) NULL,
				(FSRSpareBuf *) NULL,
				(UINT32) (FSR_LLD_FLAG_ECC_OFF | FSR_LLD_FLAG_1X_LOAD));
		if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
		{
			break;
		}

		nLLDRet = FSR_PND_FlushOp(nDev, nDie, nFlag);

		if (FSR_RETURN_MAJOR(nLLDRet) != FSR_LLD_SUCCESS)
		{
			break;
		}

		nDQL = *(pstPNDShMem->pPseudoSpareDataRAM);

		if (nDQF != FSR_PND_VALID_BLK_MARK_X6 || nDQL != FSR_PND_VALID_BLK_MARK_X6)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR | FSR_DBZ_LLD_INF,
                            (TEXT("[PND:INF]   %s(nDev:%d, nPbn:%d, nFlag:0x%08x) / %d line\r\n"),
                            __FSR_FUNC__, nDev, nPbn, nFlag, __LINE__));


            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR | FSR_DBZ_LLD_INF,
                            (TEXT("            nPbn = %d is a bad block\r\n"), nPbn));

            nLLDRet = FSR_LLD_INIT_BADBLOCK | FSR_LLD_BAD_BLK_1STPLN;
        }
        else
        {
            FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_INF,
                            (TEXT("[PND:INF]   %s(nDev:%d, nPbn:%d, nFlag:0x%08x) / %d line\r\n"),
                            __FSR_FUNC__, nDev, nPbn, nFlag, __LINE__));

            FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_INF,
                            (TEXT("            nPbn = %d is a good block\r\n"), nPbn));

            nLLDRet = FSR_LLD_INIT_GOODBLOCK;
        }

#if defined (FSR_LLD_STATISTICS)
        _AddPNDStat(nDev, nDie, FSR_PND_STAT_SLC_LOAD, 0, FSR_PND_STAT_NORMAL_CMD);
#endif /* #if defined (FSR_LLD_STATISTICS) */

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function flush previous operation
 *
 * @param[in]       nDev         : Physical Device Number (0 ~ 3)
 * @param[in]       nDie         : 0 is for 1st die
 * @n                            : 1 is for 2nd die
 * @param[in]       nFlag        :
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_PREV_WRITE_ERROR | {FSR_LLD_1STPLN_CURR_ERROR}
 * @return          FSR_LLD_PREV_ERASE_ERROR | {FSR_LLD_1STPLN_CURR_ERROR}
 * @return          FSR_LLD_INVALID_PARAM
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          This function completes previous operation,
 * @n               and returns error for previous operation
 * @n               After calling series of FSR_FND_Write() or FSR_FND_Erase() or
 * @n               FSR_FND_CopyBack(), FSR_FND_FlushOp() needs to be called.
 *
 */
PUBLIC INT32
FSR_PND_FlushOp(UINT32         nDev,
                UINT32         nDie,
                UINT32         nFlag)
{
            
    PureNANDCxt        *pstPNDCxt;
    PureNANDShMem      *pstPNDShMem;
    
    UINT32              nPrevOp;

#if !defined(FSR_OAM_RTLMSG_DISABLE)
    UINT32              nPrevPbn;
    UINT32              nPrevPgOffset;
#endif

    UINT32              nPrevFlag;    
    INT32               nLLDRet         = FSR_LLD_SUCCESS;    

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev: %d, nDieIdx: %d, nFlag: 0x%08x)\r\n"),
        __FSR_FUNC__, nDev, nDie, nFlag));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
            (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
            __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
            (TEXT("            Invalid Device Number (nDev = %d)\r\n"), nDev));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }
#endif

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDShMem = gpstPNDShMem[nDev];
        
        pstPNDCxt->nFlushOpCaller = (UINT16) (nDie >> FSR_PND_FLUSHOP_CALLER_BASEBIT);
        
        nDie = nDie & ~FSR_PND_FLUSHOP_CALLER_MASK;

        nPrevOp       = pstPNDShMem->nPreOp[nDie];
        
#if !defined(FSR_OAM_RTLMSG_DISABLE)
        nPrevPbn      = pstPNDShMem->nPreOpPbn[nDie];
        nPrevPgOffset = pstPNDShMem->nPreOpPgOffset[nDie];
#endif

        nPrevFlag     = pstPNDShMem->nPreOpFlag[nDie];

        switch (nPrevOp)
        {
        case FSR_PND_PREOP_NONE:
            /* DO NOT remove this code : 'case FSR_PND_PREOP_NONE:'
               for compiler optimization */
            break;

        case FSR_PND_PREOP_READ:
        case FSR_PND_PREOP_CPBK_LOAD:
            
            nLLDRet = (INT32)pstPNDShMem->ECCnRE;
            
               if (FSR_RETURN_MAJOR(nLLDRet) == FSR_LLD_PREV_READ_ERROR)
               {
                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
                        __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
                        pstPNDCxt->nFlushOpCaller));
                        
                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("            ECC Status : Uncorrectable\r\n")));

                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("            at nDev:%d, nPbn:%d, nPgOffset:%d, nFlag:0x%08x\r\n"),
                        nDev, nPrevPbn, nPrevPgOffset, nPrevFlag));
                        
			// Debug
		    //NC_Dump_Registers();

                    _DumpMainBuffer(nDev);
                    _DumpSpareBuffer(nDev);
                }
                else if(FSR_RETURN_MAJOR(nLLDRet) == FSR_LLD_PREV_2LV_READ_DISTURBANCE)
                {
                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
                        __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
                        pstPNDCxt->nFlushOpCaller));
                        
                    FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF | FSR_DBZ_ERROR,
                        (TEXT("            2Lv read disturbance at nPbn:%d, nPgOffset:%d, nFlag:0x%08x\r\n"),
                        nPrevPbn, nPrevPgOffset, nPrevFlag));
                            
                }
		else if(FSR_RETURN_MAJOR(nLLDRet) == FSR_LLD_PREV_1LV_READ_DISTURBANCE)
                {
                     FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
                        __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

                    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                        (TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
                        pstPNDCxt->nFlushOpCaller));
                        
                    FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF | FSR_DBZ_ERROR,
                        (TEXT("            1Lv read disturbance at nPbn:%d, nPgOffset:%d, nFlag:0x%08x\r\n"),
                        nPrevPbn, nPrevPgOffset, nPrevFlag));
                }
            break;

        case FSR_PND_PREOP_CACHE_PGM: 
        case FSR_PND_PREOP_PROGRAM:
        case FSR_PND_PREOP_CPBK_PGM:
                        
            nLLDRet = (INT32)pstPNDShMem->ECCnRE;
            
            if (nLLDRet != FSR_LLD_SUCCESS)
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
                    pstPNDCxt->nFlushOpCaller));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            last write() call @ nDev = %d, nPbn = %d, nPgOffset = %d, nFlag:0x%08x\r\n"),
                    nDev, nPrevPbn, nPrevPgOffset, nPrevFlag));
            }
            break;
            
        case FSR_PND_PREOP_ERASE:        
         
            /* Previous error check */
            nLLDRet = (INT32)pstPNDShMem->ECCnRE;
            
            if (nLLDRet != FSR_LLD_SUCCESS)
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
                    pstPNDCxt->nFlushOpCaller));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            last Erase() call @ nDev = %d, nPbn = %d, nFlag:0x%08x\r\n"),
                    nDev, nPrevPbn, nPrevFlag));
            }
            break;

        default:
            /* Other IOCtl operations is ignored */            
            break;
        }
        
        if (!(nFlag & FSR_LLD_FLAG_REMAIN_PREOP_STAT))
        {
            pstPNDShMem->nPreOp[nDie] = FSR_PND_PREOP_NONE;
        }
        
#if defined (FSR_LLD_STATISTICS)
        _AddPNDStat(nDev, nDie, FSR_PND_STAT_FLUSH, 0, FSR_PND_STAT_NORMAL_CMD);
#endif        
        
    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function provides block information.
 *
 * @param[in]       nDev        : Physical Device Number
 * @param[in]       nPbn        : Physical Block  Number
 * @param[out]      pBlockType  : whether nPbn is SLC block or MLC block
 * @param[out]      pPgsPerBlk  : the number of pages per block
 *
 * @return          FSR_LLD_SUCCESS
 * @n               FSR_LLD_INVALID_PARAM
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          i.e. SLC block, the number of pages per block
 *
 */
PUBLIC INT32
FSR_PND_GetBlockInfo(UINT32         nDev,
                     UINT32         nPbn,
                     UINT32        *pBlockType,
                     UINT32        *pPgsPerBlk)
{
    PureNANDSpec     *pstPNDSpec;

    INT32             nLLDRet     = FSR_LLD_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d, nPbn:%d)\r\n"),
        __FSR_FUNC__, nDev, nPbn));

    do
    {
    
#if defined (FSR_LLD_STRICT_CHK)
        nLLDRet = _StrictChk(nDev, nPbn, 0);

        if (nLLDRet != FSR_LLD_SUCCESS)
        {
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDSpec = gpstPNDCxt[nDev]->pstPNDSpec;

        /* This block is SLC block */
        if (pBlockType != NULL)
        {
            *pBlockType = FSR_LLD_SLC_BLOCK;
        }
        
        if (pPgsPerBlk != NULL)
        {
            *pPgsPerBlk = pstPNDSpec->nPgsPerBlk;
        }

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return nLLDRet;
}



/**
 * @brief           This function reports device information to upper layer.
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[out]      pstDevSpec  : pointer to the device spec
 * @param[in]       nFlag       :
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_OPEN_FAILURE
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_GetDevSpec(UINT32         nDev,
                   FSRDevSpec    *pstDevSpec,
                   UINT32         nFlag)
{
    PureNANDCxt    *pstPNDCxt;
    PureNANDSpec   *pstPNDSpec;

    UINT32          nDieIdx;
    INT32           nLLDRet     = FSR_LLD_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d,pstDevSpec:0x%x,nFlag:0x%08x)\r\n"),
        __FSR_FUNC__, nDev, pstDevSpec, nFlag));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid Device Number (nDev = %d)\r\n"),
                nDev));

            nLLDRet = FSR_LLD_INVALID_PARAM;
            break;
        }

        if (pstDevSpec == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, pstDevSpec:0x%08x, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, pstDevSpec, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            nDev:%d pstDevSpec is NULL\r\n"), nDev));

            nLLDRet = FSR_LLD_OPEN_FAILURE;
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDCxt  = gpstPNDCxt[nDev];
        pstPNDSpec = pstPNDCxt->pstPNDSpec;

        FSR_OAM_MEMSET(pstDevSpec, 0xFF, sizeof(FSRDevSpec));

        pstDevSpec->nNumOfBlks          = pstPNDSpec->nNumOfBlks;
        pstDevSpec->nNumOfPlanes        = pstPNDSpec->nNumOfPlanes;

        for (nDieIdx = 0; nDieIdx < pstPNDCxt->nNumOfDies; nDieIdx++)
        {
            pstDevSpec->nBlksForSLCArea[nDieIdx] =
                pstPNDSpec->nNumOfBlks / pstPNDCxt->nNumOfDies;
        }

        pstDevSpec->nSparePerSct        = pstPNDSpec->nSparePerSct;
        pstDevSpec->nSctsPerPG          = pstPNDSpec->nSctsPerPg;
        pstDevSpec->nNumOfBlksIn1stDie  =
            pstPNDSpec->nNumOfBlks / pstPNDCxt->nNumOfDies;

        pstDevSpec->nDID                = pstPNDSpec->nDID;

        pstDevSpec->nPgsPerBlkForSLC    = pstPNDSpec->nPgsPerBlk;
        pstDevSpec->nPgsPerBlkForMLC    = 0;
        pstDevSpec->nNumOfDies          = pstPNDCxt->nNumOfDies;
        pstDevSpec->nUserOTPScts        = 0;
        pstDevSpec->b1stBlkOTP          = FALSE32;
        pstDevSpec->nRsvBlksInDev       = pstPNDSpec->nRsvBlksInDev;
        pstDevSpec->pPairedPgMap        = NULL;
        pstDevSpec->pLSBPgMap           = NULL;
        pstDevSpec->nNANDType           = FSR_LLD_SLC_NAND;
        pstDevSpec->nPgBufToDataRAMTime = 0;
        pstDevSpec->bCachePgm           = FALSE32;
        pstDevSpec->nSLCTLoadTime       = 0;
        pstDevSpec->nMLCTLoadTime       = 0;
        pstDevSpec->nSLCTProgTime       = 0;
        pstDevSpec->nMLCTProgTime[0]    = 0;
        pstDevSpec->nMLCTProgTime[1]    = 0;
        pstDevSpec->nTEraseTime         = 0;

        /* Time for transfering 1 page in u sec */
        pstDevSpec->nWrTranferTime      = 0;
        pstDevSpec->nRdTranferTime      = 0;

        pstDevSpec->nSLCPECycle         = 5000;
        pstDevSpec->nMLCPECycle         = 1500;

    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}



/**
 * @brief           This function reads data from DataRAM of Flex-OneNAND
 *
 * @param[in]       nDev    : Physical Device Number (0 ~ 3)
 * @param[out]      pMBuf   : Memory buffer for main  array of NAND flash
 * @param[out]      pSBuf   : Memory buffer for spare array of NAND flash
 * @n               nDieIdx : 0 is for 1st die for DDP device
 * @n                       : 1 is for 2nd die for DDP device
 * @param[in]       nFlag   :
 *
 * @return          FSR_LLD_SUCCESS
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark          This function does not loads data from PureNAND
 * @n               it just reads data which lies on DataRAM
 *
 */
PUBLIC INT32
FSR_PND_GetPrevOpData(UINT32         nDev,
                      UINT8         *pMBuf,
                      FSRSpareBuf   *pSBuf,
                      UINT32         nDieIdx,
                      UINT32         nFlag)
{    
    PureNANDCxt        *pstPNDCxt;
    PureNANDShMem      *pstPNDShMem;
    PureNANDSpec       *pstPNDSpec;
    INT32               nLLDRe      = FSR_LLD_SUCCESS;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s() / nDie : %d / nFlag : 0x%x\r\n"), __FSR_FUNC__, nDieIdx, nFlag));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid Device Number (nDev = %d)\r\n"),
                nDev));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }

        /* nDie can be 0 (1st die) or 1 (2nd die) */
        if ((nDieIdx & 0xFFFE) != 0)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
                __FSR_FUNC__, nDev, nDieIdx, nFlag, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Invalid nDie Number (nDie = %d)\r\n"),
                nDieIdx));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }
#endif

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDSpec  = pstPNDCxt->pstPNDSpec;
        pstPNDShMem = gpstPNDShMem[nDev];

        FSR_OAM_MEMCPY(pMBuf,
                       pstPNDShMem->pPseudoMainDataRAM,
                       (size_t)(pstPNDSpec->nSctsPerPg *  FSR_SECTOR_SIZE));

        _ReadSpare(pSBuf,
                   pstPNDShMem->pPseudoSpareDataRAM);
    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[FND:OUT] --%s()\r\n"), __FSR_FUNC__));

    return nLLDRe;    
}



/**
 * @brief           This function does PI operation, OTP operation,
 * @n               reset, write protection. 
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nCode       : IO Control Command
 * @n                             FSR_LLD_IOCTL_PI_ACCESS
 * @n                             FSR_LLD_IOCTL_PI_READ
 * @n                             FSR_LLD_IOCTL_PI_WRITE
 * @n                             FSR_LLD_IOCTL_OTP_ACCESS
 * @n                             FSR_LLD_IOCTL_OTP_LOCK
 * @n                             in case IOCTL does with OTP protection,
 * @n                             pBufI indicates 1st OTP or OTP block or both
 * @n                             FSR_LLD_IOCTL_GET_OTPINFO
 * @n                             FSR_LLD_IOCTL_LOCK_TIGHT
 * @n                             FSR_LLD_IOCTL_LOCK_BLOCK
 * @n                             FSR_LLD_IOCTL_UNLOCK_BLOCK
 * @n                             FSR_LLD_IOCTL_UNLOCK_ALLBLK
 * @n                             FSR_LLD_IOCTL_GET_LOCK_STAT
 * @n                             FSR_LLD_IOCTL_HOT_RESET
 * @n                             FSR_LLD_IOCTL_CORE_RESET
 * @param[in]       pBufI       : Input Buffer pointer
 * @param[in]       nLenI       : Length of Input Buffer
 * @param[out]      pBufO       : Output Buffer pointer
 * @param[out]      nLenO       : Length of Output Buffer
 * @param[out]      pByteRet    : The number of bytes (length) of Output Buffer
 * @n                             as the result of function call
 *
 * @return          FSR_LLD_SUCCESS
 * @return          FSR_LLD_INVALID_PARAM
 * @return          FSR_LLD_IOCTL_NOT_SUPPORT
 * @return          FSR_LLD_WRITE_ERROR      | {FSR_LLD_1STPLN_CURR_ERROR}
 * @return          FSR_LLD_ERASE_ERROR      | {FSR_LLD_1STPLN_CURR_ERROR} 
 * @return          FSR_LLD_PI_PROGRAM_ERROR
 * @return          FSR_LLD_PREV_READ_ERROR  | {FSR_LLD_1STPLN_CURR_ERROR }
 *
 * @author          Jinho Yi
 * @version         1.2.1
 * @remark          OTP read, write is performed with FSR_FND_Write(), FSR_FND_ReadOptimal(),
 * @n               after OTP Access
 *
 */
PUBLIC INT32
FSR_PND_IOCtl(UINT32         nDev,
              UINT32         nCode,
              UINT8         *pBufI,
              UINT32         nLenI,
              UINT8         *pBufO,
              UINT32         nLenO,
              UINT32        *pByteRet)
{
    /* Used to lock, unlock, lock-tight */
    LLDProtectionArg *pLLDProtectionArg;
    PureNANDCxt      *pstPNDCxt;
    PureNANDShMem    *pstPNDShMem;             
    UINT32            nDie             = 0xFFFFFFFF;
    UINT32            nPbn;
    INT32             nLLDRet          = FSR_LLD_SUCCESS;
    UINT16            nWrProtectStat;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:IN ] ++%s(nDev:%d, nCode:0x%08x)\r\n"),
        __FSR_FUNC__, nDev, nCode));

    do
    {

#if defined (FSR_LLD_STRICT_CHK)
        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
            (TEXT("[PND:ERR]   %s() / %d line\r\n"),
            __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
            (TEXT("            Invalid Device Number (nDev = %d)\r\n"), nDev));

            nLLDRet = (FSR_LLD_INVALID_PARAM);
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        pstPNDCxt   = gpstPNDCxt[nDev];
        pstPNDShMem = gpstPNDShMem[nDev];

        /* Interrupt should be enabled in I/O Ctl */
        FSR_OAM_ClrNDisableInt(pstPNDCxt->nIntID);

        switch (nCode)
        {
        case FSR_LLD_IOCTL_OTP_ACCESS:        
            nDie = 0;
            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_OTP_LOCK:
            nDie = 0;            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_OTP_GET_INFO:
            nDie = 0;
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_LOCK_TIGHT:
            if ((pBufI == NULL) || (nLenI != sizeof(LLDProtectionArg)) ||
                (pBufO == NULL) || (nLenO != sizeof(UINT32)))
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("[FND:ERR]   %s() / %d line\r\n"),
                    __FSR_FUNC__, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufI = 0x%08x, nLenI = %d\r\n"), pBufI, nLenI));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufO = 0x%08x, nLenO = %d\r\n"), pBufO, nLenO));

                nLLDRet = FSR_LLD_INVALID_PARAM;
                break;
            }

            pLLDProtectionArg = (LLDProtectionArg *) pBufI;

            nDie = pLLDProtectionArg->nStartBlk >> pstPNDCxt->nDDPSelBaseBit;

            FSR_ASSERT((nDie & ~0x1) == 0);
            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_LOCK_BLOCK:
            if ((pBufI == NULL) || (nLenI != sizeof(LLDProtectionArg)) ||
                (pBufO == NULL) || (nLenO != sizeof(UINT32)))
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("[FND:ERR]   %s() / %d line\r\n"),
                    __FSR_FUNC__, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufI = 0x%08x, nLenI = %d\r\n"), pBufI, nLenI));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufO = 0x%08x, nLenO = %d\r\n"), pBufO, nLenO));

                nLLDRet = FSR_LLD_INVALID_PARAM;
                break;
            }

            pLLDProtectionArg = (LLDProtectionArg *) pBufI;

            nDie = pLLDProtectionArg->nStartBlk >> pstPNDCxt->nDDPSelBaseBit;

            FSR_ASSERT((nDie & ~0x1) == 0);

            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_UNLOCK_BLOCK:
            if ((pBufI == NULL) || (nLenI != sizeof(LLDProtectionArg)) ||
                (pBufO == NULL) || (nLenO != sizeof(UINT32)))
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("[FND:ERR]   %s() / %d line\r\n"),
                    __FSR_FUNC__, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufI = 0x%08x, nLenI = %d\r\n"), pBufI, nLenI));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufO = 0x%08x, nLenO = %d\r\n"), pBufO, nLenO));

                nLLDRet = FSR_LLD_INVALID_PARAM;
                break;
            }

            pLLDProtectionArg = (LLDProtectionArg *) pBufI;

            nDie = pLLDProtectionArg->nStartBlk >> pstPNDCxt->nDDPSelBaseBit;

            FSR_ASSERT((nDie & ~0x1) == 0);
            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_UNLOCK_ALLBLK:
            if ((pBufI == NULL) || (nLenI != sizeof(LLDProtectionArg)) ||
               (pBufO == NULL) || (nLenO != sizeof(UINT32)))
            {
               FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                   (TEXT("[FND:ERR]   %s() / %d line\r\n"),
                   __FSR_FUNC__, __LINE__));

               FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                   (TEXT("            invalid parameter pBufI = 0x%08x, nLenI = %d\r\n"), pBufI, nLenI));

               FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                   (TEXT("            invalid parameter pBufO = 0x%08x, nLenO = %d\r\n"), pBufO, nLenO));

               nLLDRet = FSR_LLD_INVALID_PARAM;
               break;
            }            

            pLLDProtectionArg = (LLDProtectionArg *) pBufI;

            nDie = pLLDProtectionArg->nStartBlk >> pstPNDCxt->nDDPSelBaseBit;

            FSR_ASSERT((nDie & ~0x1) == 0);

            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_GET_LOCK_STAT:
            if ((pBufI == NULL) || (nLenI != sizeof(UINT32)) ||
                (pBufO == NULL) || (nLenO != sizeof(UINT32)))
            {
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("[FND:ERR]   %s() / %d line\r\n"),
                    __FSR_FUNC__, __LINE__));

                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("            invalid parameter pBufO = 0x%08x, nLenO = %d\r\n"), pBufO, nLenO));

                nLLDRet = FSR_LLD_INVALID_PARAM;
                break;
            }

            nPbn    = *(UINT32 *) pBufI;

            nDie = nPbn >> pstPNDCxt->nDDPSelBaseBit;

            FSR_ASSERT((nDie & ~0x1) == 0);

            nWrProtectStat = FSR_LLD_BLK_STAT_UNLOCKED;

            *(UINT32 *) pBufO = (UINT32) nWrProtectStat;
            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) sizeof(UINT32);
            }

            nLLDRet    = FSR_LLD_SUCCESS;
            break;

        case FSR_LLD_IOCTL_HOT_RESET:            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        case FSR_LLD_IOCTL_CORE_RESET:            
            if (pByteRet != NULL)
            {
                *pByteRet = (UINT32) 0;
            }
            break;

        default:
            nLLDRet = FSR_LLD_IOCTL_NOT_SUPPORT;
            break;
        }

        if ((nCode == FSR_LLD_IOCTL_HOT_RESET) || (nCode == FSR_LLD_IOCTL_CORE_RESET))
        {
            for (nDie = 0; nDie < pstPNDCxt->nNumOfDies; nDie++)
            {
                pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_IOCTL;
                pstPNDShMem->nPreOpPbn[nDie]      = FSR_PND_PREOP_ADDRESS_NONE;
                pstPNDShMem->nPreOpPgOffset[nDie] = FSR_PND_PREOP_ADDRESS_NONE;
                pstPNDShMem->nPreOpFlag[nDie]     = FSR_PND_PREOP_FLAG_NONE;
            }
        }
        else if ((nLLDRet != FSR_LLD_INVALID_PARAM) && (nLLDRet != FSR_LLD_IOCTL_NOT_SUPPORT))
        {
            FSR_ASSERT(nDie != 0xFFFFFFFF);

            pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_IOCTL;
            pstPNDShMem->nPreOpPbn[nDie]      = FSR_PND_PREOP_ADDRESS_NONE;
            pstPNDShMem->nPreOpPgOffset[nDie] = FSR_PND_PREOP_ADDRESS_NONE;
            pstPNDShMem->nPreOpFlag[nDie]     = FSR_PND_PREOP_FLAG_NONE;
        }
    } while (0);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRet : 0x%x\r\n"), __FSR_FUNC__, nLLDRet));

    return (nLLDRet);
}
           
/**
 * @brief           This function is used for pure NAND
 *                  this function set address and command for NAND flash
 *
 * @return          FSR_PAM_SUCCESS
 *
 * @author          JinHo Yi
 * @version         1.2.1
 *
 */
PRIVATE  INT32
_SetNANDCtrllerCmd (UINT32 nDev,
                    UINT32 nDie,
                    UINT32 nRowAddress,
                    UINT32 nColAddress,
                    UINT32 nCommand)
{

#if defined(_FSR_LLD_MSTAR_X13_)
	//UINT32	nCS;
	//UINT32  nCycle;
	PureNANDShMem    *pstPNDShMem;
	UINT8            *pMainDataRAM;
	UINT8            *pSpareDataRAM;
	PureNANDCxt      *pstPNDCxt;
	PureNANDSpec     *pstPNDSpec;
	INT32   nRe         = FSR_LLD_SUCCESS;    
	//nCS = nDev;

	//nCycle = gpstPNDCxt[nDev]->pstPNDSpec->n5CycleDev;
	pstPNDShMem = gpstPNDShMem[nDev];
	pstPNDCxt   = gpstPNDCxt[nDev];
	pstPNDSpec  = pstPNDCxt->pstPNDSpec;

	pMainDataRAM    = pstPNDShMem->pPseudoMainDataRAM;
	pSpareDataRAM   = pstPNDShMem->pPseudoSpareDataRAM;

	switch (nCommand)
	{
		case FSR_PND_SET_READ_PAGE:
			nRe = (INT32)NC_ReadPages(pstPNDSpec,nRowAddress, pMainDataRAM, pSpareDataRAM, 1);
			break;

		case FSR_PND_SET_PROGRAM_PAGE:
			nRe = (INT32)NC_WritePages(pstPNDSpec, nRowAddress, pMainDataRAM, pSpareDataRAM, 1);
			/* tADL is Min 100ns */
			break;

		case FSR_PND_SET_ERASE:
			nRe = (INT32)NC_EraseBlk(pstPNDSpec,nRowAddress);
			break;

		case FSR_PND_SET_RANDOM_DATA_INPUT:
			nRe = (INT32)NC_Read_RandomIn(pstPNDSpec,nRowAddress, pstPNDSpec->nPageByteCnt, pSpareDataRAM, pstPNDSpec->nSpareByteCnt);
			/* tADL is Min 100ns */
			break;

		case FSR_PND_SET_RANDOM_DATA_OUTPUT:
			nRe = (INT32)NC_Write_RandomOut(pstPNDSpec,nRowAddress, pstPNDSpec->nPageByteCnt, pSpareDataRAM, pstPNDSpec->nSpareByteCnt);
			break;

		case FSR_PND_SET_READ_PGM_STATUS:
		case FSR_PND_SET_READ_ERASE_STATUS:
		case FSR_PND_SET_READ_INTERRUPT_STATUS:
			break;

		case FSR_PND_SET_ECC_ON:            
			NC_EnableECC();            
			break;

		case FSR_PND_SET_ECC_OFF:
			NC_DisableECC();
			break;

		default:
			break;
	}
#endif
	return nRe;
}


/**
 * @brief           This function initialize the structure for LLD statistics
 *
 * @return          FSR_LLD_SUCCESS
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_InitLLDStat(VOID)
{    

#if defined (FSR_LLD_STATISTICS)
    PureNANDStat   *pstStat;
    UINT32          nVolIdx;
    UINT32          nDevIdx;
    UINT32          nDieIdx;
    
    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG, 
                   (TEXT("[PND:IN ] ++%s\r\n"), 
                   __FSR_FUNC__));
                   
    gnElapsedTime = 0;

    for (nVolIdx = 0; nVolIdx < FSR_MAX_VOLS; nVolIdx++)
    {
        for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVolIdx]; nDevIdx++)
        {
            pstStat = gpstPNDStat[nVolIdx * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx];

            pstStat->nNumOfSLCLoads    = 0;
            pstStat->nNumOfSLCPgms     = 0;
            pstStat->nNumOfCacheBusy   = 0;
            pstStat->nNumOfErases      = 0;
            pstStat->nNumOfRdTrans     = 0;
            pstStat->nNumOfWrTrans     = 0;
            pstStat->nRdTransInBytes   = 0;
            pstStat->nWrTransInBytes   = 0;
            pstStat->nNumOfMLCLoads    = 0;
            pstStat->nNumOfLSBPgms     = 0;
            pstStat->nNumOfMSBPgms     = 0;

            for (nDieIdx = 0; nDieIdx < FSR_MAX_DIES; nDieIdx++)
            {
                pstStat->nPreCmdOption[nDieIdx] = FSR_PND_STAT_NORMAL_CMD;
                pstStat->nIntLowTime[nDieIdx]   = 0;
            }
        }
    }
                   
    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRe : 0x%x\r\n"), __FSR_FUNC__, FSR_LLD_SUCCESS));        
#endif

    return FSR_LLD_SUCCESS;
}



/**
 * @brief           This function totals the time device consumed.
 *
 * @param[out]      pstStat : the pointer to the structure, FSRLLDStat
 *
 * @return          total busy time of whole device
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_GetStat(FSRLLDStat    *pstStat)
{
#if defined (FSR_LLD_STATISTICS)
    PureNANDCxt    *pstPNDCxt;
    PureNANDSpec   *pstPNDSpec;
    PureNANDStat   *pstPNDStat;
    UINT32          nVolIdx;
    UINT32          nDevIdx;
    UINT32          nTotalTime;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG, 
                   (TEXT("[PND:IN ] ++%s\r\n"), 
                   __FSR_FUNC__));
                   
    do
    {
        if (pstStat == NULL)
        {
            break;
        }

        FSR_OAM_MEMSET(pstStat, 0x00, sizeof(FSRLLDStat));

        nTotalTime    = 0;

        for (nVolIdx = 0; nVolIdx < FSR_MAX_VOLS; nVolIdx++)
        {
            for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVolIdx]; nDevIdx++)
            {
                pstPNDStat = gpstPNDStat[nVolIdx * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx];

                pstStat->nSLCPgms        += pstPNDStat->nNumOfSLCPgms;
                pstStat->nErases         += pstPNDStat->nNumOfErases;
                pstStat->nSLCLoads       += pstPNDStat->nNumOfSLCLoads;
                pstStat->nRdTrans        += pstPNDStat->nNumOfRdTrans;
                pstStat->nWrTrans        += pstPNDStat->nNumOfWrTrans;
                pstStat->nCacheBusy      += pstPNDStat->nNumOfCacheBusy;
                pstStat->nRdTransInBytes += pstPNDStat->nRdTransInBytes;
                pstStat->nWrTransInBytes += pstPNDStat->nWrTransInBytes;

                pstPNDCxt     = gpstPNDCxt[nDevIdx];
                pstPNDSpec    = pstPNDCxt->pstPNDSpec;

                nTotalTime   += pstStat->nSLCLoads          * pstPNDSpec->nSLCTLoadTime;
                nTotalTime   += pstStat->nSLCPgms           * pstPNDSpec->nSLCTProgTime;
                nTotalTime   += pstStat->nErases            * pstPNDSpec->nTEraseTime;

                /* pstPNDCxt->nRdTranferTime, pstPNDCxt->nWrTranferTime is time for transfering 2 bytes */
                nTotalTime   += pstStat->nRdTransInBytes    * pstPNDCxt->nRdTranferTime / 2 / 1000;
                nTotalTime   += pstStat->nWrTransInBytes    * pstPNDCxt->nWrTranferTime / 2 / 1000;
                nTotalTime   += pstStat->nCacheBusy         * FSR_PND_CACHE_BUSY_TIME;
            }
        }
    } while (0);
                   
    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRe : 0x%x\r\n"), __FSR_FUNC__, FSR_LLD_SUCCESS));
    return gnElapsedTime;
#else
    FSR_OAM_MEMSET(pstStat, 0x00, sizeof(FSRLLDStat));
    return 0;
#endif

}



/**
 * @brief           This function provides access information 
 *
 * @param[in]       nDev        : Physical Device Number
 * @param[out]      pLLDPltInfo : structure for platform information.
 *
 * @return          FSR_LLD_SUCCESS
 * @n               FSR_LLD_INVALID_PARAM
 * @n               FSR_LLD_OPEN_FAILURE
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark
 *
 */
PUBLIC INT32
FSR_PND_GetNANDCtrllerInfo(UINT32             nDev,
                           LLDPlatformInfo   *pLLDPltInfo)
{
    INT32           nLLDRe = FSR_LLD_SUCCESS;
    PureNANDCxt    *pstPNDCxt;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG, 
                   (TEXT("[PND:IN ] ++%s(nDev:%d)\r\n"), 
                   __FSR_FUNC__, nDev));
                   
    do
    {
    
#if defined (FSR_LLD_STRICT_CHK)
        if (pLLDPltInfo == NULL)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            pLLDPltInfo is NULL\r\n")));

            nLLDRe = FSR_LLD_INVALID_PARAM;
            break;
        }

        /* Check device number */
        if (nDev >= FSR_PND_MAX_DEVS)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
            (TEXT("[PND:ERR]   %s() / %d line\r\n"),
            __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
            (TEXT("            Invalid Device Number (nDev = %d)\r\n"), nDev));

            nLLDRe = (FSR_LLD_INVALID_PARAM);
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */    

        pstPNDCxt = gpstPNDCxt[nDev];

#if defined (FSR_LLD_STRICT_CHK)
        /* Check Device Open Flag */
        if (pstPNDCxt->bOpen == FALSE32)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("[PND:ERR]   %s() / %d line\r\n"),
                __FSR_FUNC__, __LINE__));

            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                (TEXT("            Device is not opened\r\n")));

            nLLDRe = FSR_LLD_OPEN_FAILURE;
            break;
        }
#endif /* #if defined (FSR_LLD_STRICT_CHK) */

        FSR_OAM_MEMSET(pLLDPltInfo, 0x00, sizeof(LLDPlatformInfo));

        /* Type of Device : FSR_LLD_SLC_ONENAND */
        pLLDPltInfo->nType            = FSR_LLD_SLC_NAND;
        /* Address of command register          */
        pLLDPltInfo->nAddrOfCmdReg    = (UINT32) NULL;
        /* Address of address register          */
        pLLDPltInfo->nAddrOfAdrReg    = (UINT32) NULL;
        /* Address of register for reading ID   */
        pLLDPltInfo->nAddrOfReadIDReg = (UINT32) NULL;
        /* Address of status register           */
        pLLDPltInfo->nAddrOfStatusReg = (UINT32) NULL;
        /* Command of reading Device ID         */
        pLLDPltInfo->nCmdOfReadID     = (UINT32) NULL; 
        /* Command of read page                 */
        pLLDPltInfo->nCmdOfReadPage   = (UINT32) NULL;
        /* Command of read status               */
        pLLDPltInfo->nCmdOfReadStatus = (UINT32) NULL;
        /* Mask value for Ready or Busy status  */
        pLLDPltInfo->nMaskOfRnB       = (UINT32) 0x40;
    } while(0);
    
    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_IF | FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s() / nLLDRe : 0x%x\r\n"), __FSR_FUNC__, nLLDRe));
                   
    return nLLDRe;
}



#if defined (FSR_LLD_LOGGING_HISTORY)
/**
 * @brief           This function leave a trace for the LLD function call
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3)
 * @param[in]       nDie        : Die(Chip) index
 *
 * @return          none
 *
 * @author          NamOh Hwang
 * @version         1.0.0
 * @remark
 *
 */
VOID
_AddLog(UINT32 nDev, UINT32 nDie)
{
             PureNANDShMem *pstPNDShMem;
    volatile PureNANDOpLog *pstPNDOpLog;

    UINT32 nLogHead;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG ,
        (TEXT("[PND:IN ] ++%s(nDie: %d)\r\n"), __FSR_FUNC__, nDie));

    pstPNDShMem = gpstPNDShMem[nDev];

    pstPNDOpLog = &gstPNDOpLog[nDev];

    nLogHead = pstPNDOpLog->nLogHead;

    pstPNDOpLog->nLogOp       [nLogHead]  = pstPNDShMem->nPreOp[nDie];
    pstPNDOpLog->nLogPbn      [nLogHead]  = pstPNDShMem->nPreOpPbn[nDie];
    pstPNDOpLog->nLogPgOffset [nLogHead]  = pstPNDShMem->nPreOpPgOffset[nDie];
    pstPNDOpLog->nLogFlag     [nLogHead]  = pstPNDShMem->nPreOpFlag[nDie];

    pstPNDOpLog->nLogHead = (nLogHead + 1) & (FSR_PND_MAX_LOG -1);

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
        (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));
}
#endif /* #if defined (FSR_LLD_LOGGING_HISTORY) */


/**
 * @brief           This function prints pseudo spare DATARAM
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3) 
 *
 * @return          none
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark
 *
 */
PRIVATE VOID
_DumpSpareBuffer(UINT32 nDev)
{
#if !defined(FSR_OAM_RTLMSG_DISABLE)
    UINT32          nSctIdx;
    UINT32          nIdx;    
    UINT16          nValue;
    PureNANDCxt    *pstPNDCxt;
    PureNANDShMem  *pstPNDShMem;
    UINT8          *pSpareDataRAM;
    UINT32          nSpareSizePerSct;
    UINT32          nDieIdx;
    
    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
    (TEXT("[PND:IN ] ++%s(nDev:%d)\r\n"),
    __FSR_FUNC__, nDev));
    
    pstPNDCxt        = gpstPNDCxt[nDev];
    pstPNDShMem      = gpstPNDShMem[nDev];
    pSpareDataRAM    = pstPNDShMem->pPseudoSpareDataRAM;
    nSpareSizePerSct = pstPNDCxt->pstPNDSpec->nSparePerSct;
        
    for (nDieIdx = 0; nDieIdx < pstPNDCxt->nNumOfDies; nDieIdx++)
    {
        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("          Dump Spare Buffer [die:%d]\r\n"), nDieIdx));
        for (nSctIdx = 0; nSctIdx < pstPNDCxt->pstPNDSpec->nSctsPerPg; nSctIdx++)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("[%02d] "), nSctIdx));
            for (nIdx = 0; nIdx < nSpareSizePerSct / sizeof(UINT16); nIdx++)
            {
                nValue = *((UINT16 *)(pSpareDataRAM + nSctIdx * nSpareSizePerSct)
                            + nIdx);
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("%04x "), nValue));
            }
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("\r\n")));
        }
    }
    
    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("\r\n")));

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG, (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));
#endif

}


/**
 * @brief           This function prints pseudo main DATARAM
 *
 * @param[in]       nDev        : Physical Device Number (0 ~ 3) 
 *
 * @return          none
 *
 * @author          Jinhyuck Kim
 * @version         1.2.1
 * @remark
 *
 */
PRIVATE VOID
_DumpMainBuffer(UINT32 nDev)
{
#if !defined(FSR_OAM_RTLMSG_DISABLE)
    UINT32          nDataIdx;
    UINT32          nIdx;    
    UINT16          nValue;
    PureNANDCxt    *pstPNDCxt;
    PureNANDShMem  *pstPNDShMem;
    UINT8          *pMainDataRAM;    
    UINT32          nDieIdx;
    UINT32          nPageSize;
    
    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
    (TEXT("[PND:IN ] ++%s(nDev:%d)\r\n"),
    __FSR_FUNC__, nDev));
    
    pstPNDCxt        = gpstPNDCxt[nDev];
    pstPNDShMem      = gpstPNDShMem[nDev];
    pMainDataRAM     = pstPNDShMem->pPseudoMainDataRAM;
    nPageSize        = (UINT32)pstPNDCxt->pstPNDSpec->nSctsPerPg *  (UINT32)FSR_SECTOR_SIZE;    
        
    for (nDieIdx = 0; nDieIdx < pstPNDCxt->nNumOfDies; nDieIdx++)
    {
        FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("          Dump Main Buffer [die:%d]\r\n"), nDieIdx));
        for (nDataIdx = 0; nDataIdx < nPageSize / (16 * sizeof(UINT16)); nDataIdx++)
        {
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("%04x: "), nDataIdx * 16));
            for (nIdx = 0; nIdx < 16; nIdx++)
            {
                nValue = *((UINT16 *)(pMainDataRAM) 
                            + nDataIdx * 16 + nIdx);
                FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
                    (TEXT("%04x "), nValue));
            }
            FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("\r\n")));
        }
    }
    
    FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR, (TEXT("\r\n")));

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG, (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));
#endif

}



#if defined (FSR_LLD_STATISTICS)
/**
 * @brief          This function is called within the LLD function
 *                 to total the busy time of the device
 *
 * @param[in]      nDev         : Physical Device Number (0 ~ 7)
 * @param[in]      nDieNum      : Physical Die  Number
 * @param[in]      nType        : FSR_PND_STAT_SLC_PGM
 *                                FSR_PND_STAT_LSB_PGM
 *                                FSR_PND_STAT_MSB_PGM
 *                                FSR_PND_STAT_ERASE
 *                                FSR_PND_STAT_LOAD
 *                                FSR_PND_STAT_RD_TRANS
 *                                FSR_PND_STAT_WR_TRANS
 * @param[in]      nBytes       : the number of bytes to transfer from/to DataRAM
 * @param[in]      nCmdOption   : command option such cache, superload
 *                                which can hide transfer time
 *
 *
 * @return         VOID
 *
 * @since          since v1.0.0
 *
 */
PRIVATE VOID
_AddPNDStat(UINT32  nDevNum,
            UINT32  nDieNum,
            UINT32  nType,
            UINT32  nBytes,
            UINT32  nCmdOption)
{
    PureNANDCxt  *pstPNDCxt  = gpstPNDCxt[nDevNum];
    PureNANDSpec *pstPNDSpec = pstPNDCxt->pstPNDSpec;
    PureNANDStat *pstPNDStat = gpstPNDStat[nDevNum];
    
    UINT32       nVol;
    UINT32       nDevIdx; /* Device index within a volume (0~4) */
    UINT32       nPDevIdx;/* Physical device index (0~7)        */
    UINT32       nDieIdx;
    UINT32       nNumOfDies;
    
    /* The duration of Interrupt Low when command is issued */
    INT32        nIntLowTime;
    INT32        nTransferTime;

    FSR_STACK_VAR;

    FSR_STACK_END;

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG, (TEXT("[PND:IN ] ++%s\r\n"), __FSR_FUNC__));

    pstPNDStat  = gpstPNDStat[nDevNum];
    nIntLowTime = pstPNDStat->nIntLowTime[nDieNum];

    switch (nType)
    {
    case FSR_PND_STAT_SLC_PGM:
        /* Add the number of times of SLC program */
        pstPNDStat->nNumOfSLCPgms++;
        
        if (nIntLowTime > 0)
        {
            /* Wait INT for previous operation */
            gnElapsedTime += nIntLowTime;
            
            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nIntLowTime;
                        }
                    }
                }
            }
        }

        /* 
         * If nCmdOption is CACHE, transfering time can be hided
         * store & use it later 
         */
        pstPNDStat->nPreCmdOption[nDieNum] = nCmdOption;
        pstPNDStat->nIntLowTime[nDieNum]   = FSR_PND_WR_SW_OH + pstPNDSpec->nSLCTProgTime;

        if (nCmdOption == FSR_PND_STAT_CACHE_PGM)
        {
            pstPNDStat->nNumOfCacheBusy++;
        }
        break;

    case FSR_PND_STAT_ERASE:
        pstPNDStat->nNumOfErases++;
        
        if (nIntLowTime > 0)
        {
            /* Wait INT for previous operation */
            gnElapsedTime += nIntLowTime;
            
            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nIntLowTime;
                        }
                    }
                }
            }
        }

        pstPNDStat->nIntLowTime[nDieNum] = pstPNDSpec->nTEraseTime;
        break;

    case FSR_PND_STAT_SLC_LOAD:
        pstPNDStat->nNumOfSLCLoads++;

        if(nIntLowTime > 0)
        {
            /* Wait INT for previous operation */
            gnElapsedTime += nIntLowTime;
            
            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nIntLowTime;
                        }
                    }
                }
            }
        }
        pstPNDStat->nIntLowTime[nDieNum] = FSR_PND_RD_SW_OH + pstPNDSpec->nSLCTLoadTime;
        
        pstPNDStat->nPreCmdOption[nDieNum] = nCmdOption;
        break;

    case FSR_PND_STAT_RD_TRANS:
        pstPNDStat->nNumOfRdTrans++;
        pstPNDStat->nRdTransInBytes  += nBytes;
        nTransferTime = nBytes * pstPNDCxt->nRdTranferTime / 2 / 1000;

        if (nBytes > 0)
        {
            /* Add s/w overhead */
            nTransferTime += FSR_PND_RD_TRANS_SW_OH;
        }

        if ((nCmdOption != FSR_PND_STAT_PLOAD) && (nIntLowTime > 0))
        {
            gnElapsedTime += nIntLowTime;

            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= /* sw overhead + */nIntLowTime;
                        }
                    }
                }
            }
        }

        gnElapsedTime += nTransferTime;

        for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
        {
            for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
            {
                nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                {
                    if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                    {
                        gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nTransferTime;
                    }
                }
            }
        }

        break;

    case FSR_PND_STAT_WR_TRANS:
        pstPNDStat->nNumOfWrTrans++;
        pstPNDStat->nWrTransInBytes  += nBytes;
        nTransferTime = nBytes * pstPNDCxt->nWrTranferTime / 2 / 1000;

        /* Cache operation can hide transfer time */
        if((pstPNDStat->nPreCmdOption[nDieNum] == FSR_PND_STAT_CACHE_PGM) && (nIntLowTime >= 0))
        {
            gnElapsedTime  += nTransferTime;

            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nTransferTime;
                        }
                    }
                }
            }

            if(nIntLowTime < nTransferTime)
            {
                /* Only some of transfer time can be hided */
                pstPNDStat->nIntLowTime[nDieNum] = 0;
            }
        }
        else /* Transfer time cannot be hided */
        {
            if(nIntLowTime > 0)
            {
                gnElapsedTime += nIntLowTime;

                for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
                {
                    for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                    {
                        nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                        nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                        for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                        {
                            if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                            {
                                gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nIntLowTime;
                            }
                        }
                    }
                }
            }

            gnElapsedTime  += nTransferTime;

            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nTransferTime;
                        }
                    }
                }
            }

            pstPNDStat->nIntLowTime[nDieNum] = 0;
        }
        break;

    case FSR_PND_STAT_FLUSH:
        if (nIntLowTime > 0)
        {
            /* Wait INT for previous operation */
            gnElapsedTime += nIntLowTime;
            
            for (nVol = 0; nVol < FSR_MAX_VOLS; nVol++)
            {
                for (nDevIdx = 0; nDevIdx < gnDevsInVol[nVol]; nDevIdx++)
                {
                    nPDevIdx   = nVol * FSR_MAX_DEVS / FSR_MAX_VOLS + nDevIdx;
                    nNumOfDies = gpstPNDCxt[nPDevIdx]->nNumOfDies;

                    for (nDieIdx = 0; nDieIdx < nNumOfDies; nDieIdx++)
                    {
                        if (gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] > 0)
                        {
                            gpstPNDStat[nPDevIdx]->nIntLowTime[nDieIdx] -= nIntLowTime;
                        }
                    }
                }
            }

            pstPNDStat->nIntLowTime[nDieNum] = 0;
        }
        break;

    default:
        FSR_ASSERT(0);
        break;
    }

    FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG, (TEXT("[PND:OUT] --%s\r\n"), __FSR_FUNC__));
}
#endif /* #if defined (FSR_LLD_STATISTICS) */


//=====================================================
// set FCIE clock
//=====================================================
UINT32 nand_clock_setting(UINT32 u32ClkParam)
{
	REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT_CLK_INVERSE|BIT_CLK_GATING);
	REG_CLR_BITS_UINT16(reg_ckg_fcie, NFIE_CLK_MASK);
	REG_SET_BITS_UINT16(reg_ckg_fcie, u32ClkParam);
	
	#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
	nand_printf("reg_ckg_fcie(%08X)=%08X\n", reg_ckg_fcie, REG(reg_ckg_fcie));
	#endif
	
	return 0;
}

UINT32 nand_translate_DMA_address_Ex(UINT32 u32_DMAAddr, UINT32 u32_ByteCnt)
{
	_dma_cache_wback_inv(u32_DMAAddr, u32_ByteCnt);
	return u32_DMAAddr;
}

UINT32 nand_hw_timer_delay(UINT32 u32usTick)
{
	UINT32 u32HWTimer = 0;
	UINT16 u16TimerLow = 0;
	UINT16 u16TimerHigh = 0;
	
	// reset HW timer
	REG_WRITE_UINT16(TIMER0_MAX_LOW, 0xFFFF);
	REG_WRITE_UINT16(TIMER0_MAX_HIGH, 0xFFFF);
	REG_WRITE_UINT16(TIMER0_ENABLE, 0);

	// start HW timer
	REG_SET_BITS_UINT16(TIMER0_ENABLE, 0x0001);

	while( u32HWTimer < 12*u32usTick ) // wait for u32usTick micro seconds
	{
		REG_READ_UINT16(TIMER0_CAP_LOW, u16TimerLow);
		REG_READ_UINT16(TIMER0_CAP_HIGH, u16TimerHigh);

		u32HWTimer = ((UINT32)u16TimerHigh<<16) | (UINT32)u16TimerLow;
	}

	REG_WRITE_UINT16(TIMER0_ENABLE, 0);

	return u32usTick;
}

//=====================================================
// Pads Switch
//=====================================================

// if pin-shared with Card IF, need to call before every JOB_START.
UINT32 nand_pads_switch(UINT32 u32EnableFCIE)
{
	UINT16     u16RegValue;

	if(u32EnableFCIE)
	{

		#if( NAND_MODE == NAND_MODE1 )
		{
			REG_SET_BITS_UINT16(reg_pcm_d_pe, 0xFF);
		}
		#elif( NAND_MODE == NAND_MODE2 )
		{
			REG_SET_BITS_UINT16(reg_pcm_a_pe, 0xFF);
		}
		#endif

	    // nand mode setting
		REG_READ_UINT16(reg_nand_mode, u16RegValue);
	    u16RegValue &= ~(REG_NAND_MODE_MASK);
		u16RegValue |= NAND_MODE;
		REG_WRITE_UINT16(reg_nand_mode, u16RegValue);

		REG_SET_BITS_UINT16(NC_REG_PAD_SWITCH, BIT11);

	}

	#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
	nand_printf("reg_pcm_d_pe(%08X)=%04X\n", reg_pcm_d_pe, REG(reg_pcm_d_pe));
	nand_printf("reg_pcm_a_pe(%08X)=%04X\n", reg_pcm_a_pe, REG(reg_pcm_a_pe));
	nand_printf("reg_nand_mode(%08X)=%04X\n", reg_nand_mode, REG(reg_nand_mode));
	nand_printf("NC_REG_PAD_SWITCH(%08X)=%04X\n", NC_REG_PAD_SWITCH, REG(NC_REG_PAD_SWITCH));
	#endif

	//REG_WRITE_UINT16(NC_PATH_CTL, BIT_NC_EN);

	return 0; // ok
}

static DECLARE_WAIT_QUEUE_HEAD(fcie_wait);
static int fcie_int = 0;

#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
static UINT16 u16CurNCMIEEvent = 0;			// Used to store current IRQ state

irqreturn_t NC_FCIE_IRQ(int irq, void *dummy)
{
	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);
	u16CurNCMIEEvent = 0;								// Reset the current IRQ state
	fcie_int = 1;
	wake_up(&fcie_wait);

	return IRQ_HANDLED;
}

UINT32 nand_WaitCompleteIntr(UINT16 u16_WaitEvent, UINT32 u32_MicroSec)
{
	UINT16 u16_Reg;
	UINT32 u32_Timeout = ((u32_MicroSec/1000) > 0) ? (u32_MicroSec/1000) : 1;	// timeout time
	UINT32 u32_Count;
	long ret;
	
	ret = wait_event_timeout(fcie_wait, (fcie_int == 1), ((long int)u32_Timeout));// wait at least 2 second for FCIE3 events
	if( ret == 0 )
	{
		printk("NAND IRQ timeout:%d jiffies!!!\n", u32_Timeout);
		NC_Dump_Registers();
		if(fcie_int == 0)
		{
			printk("FCIE IRQ not triggered, change to polling mode!!!\n");
			for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
			{
				REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);
				if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
					break;
			
				nand_hw_timer_delay(HW_TIMER_DELAY_1us);
			}
			if (u32_Count < u32_MicroSec)
				REG_W1C_BITS_UINT16(NC_MIE_EVENT, u16_WaitEvent); /*clear events*/
			else
				return UNFD_ST_ERR_R_TIMEOUT;			
		}
	}

	fcie_int = 0;

	return UNFD_ST_SUCCESS;
}

void nand_enable_intr_mode(void)
{
	int err = 0;
	
	if( (err = request_irq(13, NC_FCIE_IRQ, SA_INTERRUPT, "fcie", NULL)) < 0)
		printk("\033[31mFail to request NAND irq, !!!\033[m, err=%d\n", err);
	else
	REG_WRITE_UINT16(NC_MIE_INT_EN, BIT_NC_JOB_END);
}
#endif

UINT32 MSTAR_NAND_INIT(void)
{
	UINT32 u32_RetVal;

	REG_CLR_BITS_UINT16(reg_allpad_in, BIT15);
	#if defined(NAND_DEBUG_MSG) && NAND_DEBUG_MSG
	nand_printf("reg_allpad_in(%08X)=%04X\n", reg_allpad_in, REG(reg_allpad_in));
	#endif

	// pad switch
	nand_pads_switch(1);
	nand_clock_setting(FCIE3_SW_DEFAULT_CLK);

	// reset NC
	u32_RetVal = NC_ResetFCIE();
	if(UNFD_ST_SUCCESS != u32_RetVal)
	{
		printk("ERROR: NC_Init, ErrCode:%Xh \r\n", UNFD_ST_ERR_NO_NFIE);
		return u32_RetVal;
	}

    // enable NC
	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_NC_EN);

	REG_WRITE_UINT16(NC_SIGNAL,
		(BIT_NC_WP_AUTO | BIT_NC_WP_H | BIT_NC_CE_AUTO | BIT_NC_CE_H) &
		~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK));

	#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
	nand_enable_intr_mode();
	#endif

	NC_ResetNandFlash();

	return 0;
}

EXPORT_SYMBOL(MSTAR_NAND_INIT);

	
UINT8 au8_ID[NAND_ID_BYTE_CNT];
UINT8 *NC_ReadID(void)
{
	volatile UINT16 u16_Reg;
	
	NC_LOCK_FCIE();
	NC_PAD_SWITCH(1);

	REG_CLR_BITS_UINT16(NC_SPARE, BIT_NC_SPARE_DEST_MIU); //Make sure the spare destination is CIFD
	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ADR_C2T1S0 << 8) | CMD_0x90);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_RAN_DIN);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RAN_CNT);
	REG_WRITE_UINT16(NC_AUXREG_DAT, NAND_ID_BYTE_CNT);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0); /*offset 0*/

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_CIFD_ACCESS | BIT_NC_JOB_START);

	if(NC_WaitComplete(BIT_NC_JOB_END, DELAY_500ms_in_us) == DELAY_500ms_in_us)
	{
		printf("Error! Read ID timeout!\r\n");
		REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);
		printf("NC_MIE_EVENT: %Xh \r\n", u16_Reg);
		REG_READ_UINT16(NC_CTRL, u16_Reg);
		printf("NC_CTRL: %Xh \r\n", u16_Reg);
	}

	NC_GetCIFD(au8_ID, 0, NAND_ID_BYTE_CNT);

	NC_UNLOCK_FCIE();

	return au8_ID; // ok
}
EXPORT_SYMBOL(NC_ReadID);


UINT8 drvNand_CountBits(UINT32 u32_x)
{
    UINT8 u8_i = 0;

    while (u32_x) {
        u8_i++;
        u32_x = (u32_x >> 1);
    }
    return (UINT8)(u8_i-1);
}

#define WAIT_FIFOCLK_READY_CNT  0x10000
UINT32 NC_waitFifoClkReady(void)
{
	volatile UINT32 u32_i;
	volatile UINT16 u16_Reg;

	for(u32_i=0; u32_i<WAIT_FIFOCLK_READY_CNT; u32_i++)
	{
		REG_READ_UINT16(NC_MMA_PRI_REG, u16_Reg);
		if(u16_Reg & BIT_NC_FIFO_CLKRDY)
			break;
		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	if(WAIT_FIFOCLK_READY_CNT == u32_i)
	{
		printk("ERROR: FIFO CLK not ready \n");
		NC_ResetFCIE();
		
		return UNFD_ST_ERR_R_TIMEOUT;		
	}

	return UNFD_ST_SUCCESS;		

}

void NC_Dump_Registers(void)
{
	int    i, j;
	
	printk("reg_hst3_irq_mask_15_0_=x%04X\n", REG(reg_hst3_irq_mask_15_0_));
	printk("reg_hst3_irq_polarity_15_0_=x%04X\n", REG(reg_hst3_irq_polarity_15_0_));
	printk("reg_hst3_irq_status_15_0_=x%04X\n", REG(reg_hst3_irq_status_15_0_));
		
	// When FCIE3 timeouts, dump the FCIE3 clock setting
	printk("\nDump FCIE3 clock setting:\n");
	printk("Reg \033[34mreg_ckg_fcie\033[m=\033[31m%04X\033[m\n", REG(reg_ckg_fcie));

	// When FCIE3 timeouts, dump the MIU clock setting
	printk("\nDump FCIE3 clock setting:\n");
	printk("Reg \033[34mreg_ckg_miu\033[m=\033[31m%04X\033[m\n", REG(reg_ckg_miu));

	// When FCIE3 timeouts, dump the FCIE3 pad setting
	printk("\nDump FCIE3 pad setting:\n");
	printk("Reg \033[34mreg_allpad_in\033[m=\033[31m%04X\033[m\n", REG(reg_allpad_in));

	printk("Reg \033[34mreg_pcm_d_pe\033[m=\033[31m%04X\033[m\n", REG(reg_pcm_d_pe));
	printk("Reg \033[3reg_pcm_a_pe\033[m=\033[31m%04X\033[m\n", REG(reg_pcm_a_pe));
	printk("Reg \033[34reg_nand_mode\033[m=\033[31m%04X\033[m\n", REG(reg_nand_mode));
	printk("Reg \033[34NC_REG_PAD_SWITCH\033[m=\033[31m%04X\033[m\n", REG(NC_REG_PAD_SWITCH));

	// When FCIE3 timeouts, dump whole FCIE3 bank registers
	printk("\nDump FCIE3 bank:\n");
	for(i=0; i<0x60; i++)
	{
		printk("Reg \033[34m0x%02X\033[m=\033[31m%04X\033[m\n", (unsigned int)i, REG(GET_REG_ADDR(FCIE_REG_BASE_ADDR, (unsigned int)i)));
	}
	printk("\n");

	// When FCIE3 timeout, dump the debug ports to know the state
	printk("Dump FCIE3 debug ports:\n");
	for(i=0; i<=7; i++)
	{
		// Enable debug ports
		REG_CLR_BITS_UINT16(NC_TEST_MODE, BIT10|BIT9|BIT8);
		REG_SET_BITS_UINT16(NC_TEST_MODE, i<<8);

		if( i<=6 )
		{
			// Dump debug ports
			printk("\nDebug Mode \033[31m%d:\033[m\n", i);
			printk("DBus[15:0]=\033[34m%04X\033[m\n", REG(NC_DEBUG_DBUS0));
			printk("DBus[23:16]=\033[34m%04X\033[m\n", REG(NC_DEBUG_DBUS1));
		}
		else
		{
			for(j=0; j<8; j++)
			{
				// Dump each kind of data of mode 7
				printk("\nDebug Mode \033[31m%d-%d:\033[m\n", i, j);

				REG_CLR_BITS_UINT16(NC_WIDTH, BIT14|BIT13|BIT12);
				REG_SET_BITS_UINT16(NC_WIDTH, j<<12);

				// Dump debug ports
				printk("DBus[15:0]=\033[34m%04X\033[m\n", REG(NC_DEBUG_DBUS0));
				printk("DBus[23:16]=\033[34m%04X\033[m\n", REG(NC_DEBUG_DBUS1));
			}
		}
	}
}

void NC_ResetReconfig(PureNANDSpec *pstPNDSpec)
{
	NC_Dump_Registers();
	
	// Ensure the NC_EN is not disabled
	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_NC_EN);

	// Reset FCIE3
	printk("FCIE3 soft reset!!!\n");
	NC_ResetFCIE();

	// Reconfig FCIE3
	NC_Config(pstPNDSpec);

	// Reset NAND Flash
	NC_ResetNandFlash();


}

UINT32 NC_wait_MIULastDone(void)
{
	volatile UINT32 u32_retry=0;
	volatile UINT16 u16_Reg;

	while(u32_retry < DELAY_1s_in_us)
	{
		//REG_READ_UINT16(NC_PATH_CTL, u16_Reg);
		REG_READ_UINT16(NC_REG_MIU_LAST_DONE, u16_Reg);
		if((u16_Reg & BIT_MIU_LAST_DONE) == BIT_MIU_LAST_DONE)
		{
			break;
		}

		u32_retry++;
		nand_hw_timer_delay(1);
	}

	if(u32_retry == DELAY_1s_in_us)
	{
		printf("ERROR: Wait MIU timeout\r\n");
		NC_Dump_Registers();

		NC_ResetFCIE();
		return UNFD_ST_ERR_R_TIMEOUT;
	}

	return UNFD_ST_SUCCESS;
}

UINT32 NC_Init(PureNANDSpec *pstPNDSpec)
{
	UINT32 u32_RetVal;

	printk("\033[31mX9 TFSR\033[m\n");
	
	//----------------------------------
	// config NAND Driver - NAND Info
	//----------------------------------
	pstPNDSpec->nPageByteCnt = (UINT32)pstPNDSpec->nSctsPerPg * (UINT32)FSR_SECTOR_SIZE;
	pstPNDSpec->nSpareByteCnt = (UINT16)(pstPNDSpec->nSctsPerPg*((UINT16)FSR_SPARE_SIZE));
	pstPNDSpec->nSectorByteCnt = (UINT16)(pstPNDSpec->nPageByteCnt / pstPNDSpec->nSctsPerPg);	// sector size = 512bytes in SLC
	pstPNDSpec->nECCType = NAND_ECC_TYPE;
	
	nand_printf("pstPNDSpec->nPgsPerBlk %d\n", pstPNDSpec->nPgsPerBlk);
	nand_printf("pstPNDSpec->nPageByteCnt %d\n", pstPNDSpec->nPageByteCnt);
	nand_printf("pstPNDSpec->nSpareByteCnt %d\n", pstPNDSpec->nSpareByteCnt);
	nand_printf("pstPNDSpec->nSectorByteCnt %d\n", pstPNDSpec->nSectorByteCnt);
	
	pstPNDSpec->nBlkPageCntBits = drvNand_CountBits(pstPNDSpec->nPgsPerBlk);
	pstPNDSpec->nPageByteCntBits = drvNand_CountBits(pstPNDSpec->nPageByteCnt);
	pstPNDSpec->nSpareByteCntBits = drvNand_CountBits(pstPNDSpec->nSpareByteCnt);
	pstPNDSpec->nSectorByteCntBits = drvNand_CountBits(pstPNDSpec->nSectorByteCnt);
	
	pstPNDSpec->nBlkPageCntMask = (UINT16)((1<<pstPNDSpec->nBlkPageCntBits)-1);
	pstPNDSpec->nPageByteCntMask = (UINT32)((1<<pstPNDSpec->nPageByteCntBits)-1);
	pstPNDSpec->nSectorByteCntMask = (UINT32)((1<<pstPNDSpec->nSectorByteCntBits)-1);
	pstPNDSpec->nSpareByteCntMask = (UINT16)((1<<pstPNDSpec->nSpareByteCntBits)-1);

	pstPNDSpec->nPageSectorCnt = (UINT16)(pstPNDSpec->nPageByteCnt >> pstPNDSpec->nSectorByteCntBits);
	pstPNDSpec->nPageSectorCntBits = drvNand_CountBits(pstPNDSpec->nPageSectorCnt);
	pstPNDSpec->nPageSectorCntMask = (UINT16)((1<<pstPNDSpec->nPageSectorCntBits)-1);	
	pstPNDSpec->nSectorSpareByteCnt = (UINT16)(pstPNDSpec->nSpareByteCnt >> pstPNDSpec->nPageSectorCntBits);
	pstPNDSpec->nSectorSpareByteCntBits = drvNand_CountBits(pstPNDSpec->nSectorSpareByteCnt);

	if( pstPNDSpec->n5CycleDev == FSR_PND_4CYCLES )
	{
		pstPNDSpec->OpCode_RW_AdrCycle = ADR_C4TFS0;
		pstPNDSpec->OpCode_Erase_AdrCycle = ADR_C2TRS0;
	}
	else if( pstPNDSpec->n5CycleDev == FSR_PND_5CYCLES )
	{
		pstPNDSpec->OpCode_RW_AdrCycle = ADR_C5TFS0;
		pstPNDSpec->OpCode_Erase_AdrCycle = ADR_C3TRS0;
	}

    // disable NC
    REG_CLR_BITS_UINT16(NC_PATH_CTL, BIT_NC_EN);
	REG_WRITE_UINT16(NC_CTRL , 0);
	
	// reset NC
	u32_RetVal = NC_ResetFCIE();
	if(UNFD_ST_SUCCESS != u32_RetVal)
	{
		printk("ERROR: NC_Init, ErrCode:%Xh \r\n", UNFD_ST_ERR_NO_NFIE);
		return u32_RetVal;
	}

    // enable NC
	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_NC_EN);

	// config NC
	pstPNDSpec->nReg1B_SdioCtrl = pstPNDSpec->nSectorByteCnt +
	    ((pstPNDSpec->nSpareByteCnt >> pstPNDSpec->nPageSectorCntBits) & (~(UINT32)1));
	if(NC_MAX_SECTOR_BYTE_CNT < pstPNDSpec->nReg1B_SdioCtrl)
	{
		printk("ERROR: invalid Sector Size: %Xh bytes!\r\n", pstPNDSpec->nReg1B_SdioCtrl);
		return UNFD_ST_ERR_HAL_R_INVALID_PARAM;
	}
	pstPNDSpec->nReg1B_SdioCtrl |= BIT_SDIO_BLK_MODE;

	pstPNDSpec->nReg40_Signal =
		(BIT_NC_CE_AUTO|BIT_NC_CE_H|BIT_NC_WP_AUTO|BIT_NC_WP_H) &
		~(BIT_NC_CHK_RB_EDGEn | BIT_NC_CE_SEL_MASK);
	
	REG_WRITE_UINT16(NC_SIGNAL, pstPNDSpec->nReg40_Signal);

	pstPNDSpec->nReg48_Spare = (pstPNDSpec->nSpareByteCnt >> pstPNDSpec->nPageSectorCntBits) & ~(UINT32)1;
	if(NC_MAX_SECTOR_SPARE_BYTE_CNT < pstPNDSpec->nReg48_Spare)
	{
		printk("ERROR: invalid Sector Spare Size: %Xh bytes!\r\n", pstPNDSpec->nReg48_Spare);
		return UNFD_ST_ERR_HAL_R_INVALID_PARAM;
	}
	#if IF_SPARE_AREA_DMA
	pstPNDSpec->nReg48_Spare |= BIT_NC_SPARE_DEST_MIU;
	#endif

	if( pstPNDSpec->nPageByteCnt == 512 )
		pstPNDSpec->nReg48_Spare |= BIT_NC_ONE_COL_ADDR;

	pstPNDSpec->nReg49_SpareSize = pstPNDSpec->nSpareByteCnt & ~(UINT32)1;
	if(NC_MAX_TOTAL_SPARE_BYTE_CNT < pstPNDSpec->nReg49_SpareSize)
	{
		printk("ERROR: invalid Total Spare Size: %Xh bytes!\r\n", pstPNDSpec->nReg49_SpareSize);
		return UNFD_ST_ERR_HAL_R_INVALID_PARAM;
	}

	pstPNDSpec->nReg50_EccCtrl = 0;
	switch(pstPNDSpec->nPageByteCnt)
	{
		case 0x0200:  pstPNDSpec->nReg50_EccCtrl &= ~BIT_NC_PAGE_SIZE_512Bn;  break;
		case 0x0800:  pstPNDSpec->nReg50_EccCtrl |= BIT_NC_PAGE_SIZE_2KB;  break;
		case 0x1000:  pstPNDSpec->nReg50_EccCtrl |= BIT_NC_PAGE_SIZE_4KB;  break;
		case 0x2000:  pstPNDSpec->nReg50_EccCtrl |= BIT_NC_PAGE_SIZE_8KB;  break;
		case 0x4000:  pstPNDSpec->nReg50_EccCtrl |= BIT_NC_PAGE_SIZE_16KB;  break;
		case 0x8000:  pstPNDSpec->nReg50_EccCtrl |= BIT_NC_PAGE_SIZE_32KB;  break;
		default:
			printk("ERROR: invalid Page Size: %Xh bytes!\r\n", (unsigned int)pstPNDSpec->nPageByteCnt);
			return UNFD_ST_ERR_HAL_R_INVALID_PARAM;

	}
	switch(pstPNDSpec->nECCType)
	{
		case ECC_TYPE_RS:
			pstPNDSpec->nReg50_EccCtrl |= BIT_NC_ECC_TYPE_RS;
			pstPNDSpec->nECCCodeByteCnt = ECC_CODE_BYTECNT_RS;
			break;
		case ECC_TYPE_4BIT:
			pstPNDSpec->nReg50_EccCtrl &= ~BIT_NC_ECC_TYPE_4b512Bn;
			pstPNDSpec->nECCCodeByteCnt = ECC_CODE_BYTECNT_4BIT;
		    break;
		case ECC_TYPE_8BIT:
			pstPNDSpec->nReg50_EccCtrl |= BIT_NC_ECC_TYPE_8b512B;
			pstPNDSpec->nECCCodeByteCnt = ECC_CODE_BYTECNT_8BIT;
		    break;
		case ECC_TYPE_12BIT:
			pstPNDSpec->nReg50_EccCtrl |= BIT_NC_ECC_TYPE_12b512B;
			pstPNDSpec->nECCCodeByteCnt = ECC_CODE_BYTECNT_12BIT;
			break;
		case ECC_TYPE_24BIT1KB:
			pstPNDSpec->nReg50_EccCtrl |= BIT_NC_ECC_TYPE_24b1KB;
			pstPNDSpec->nECCCodeByteCnt = ECC_CODE_BYTECNT_24BIT1KB;
			break;
		case ECC_TYPE_32BIT1KB:
			pstPNDSpec->nReg50_EccCtrl |= BIT_NC_ECC_TYPE_32b1KB;
			pstPNDSpec->nECCCodeByteCnt = ECC_CODE_BYTECNT_32BIT1KB;
			break;
		default:
			printk("ERROR: invalid ECC Type: %Xh \r\n", pstPNDSpec->nECCType);
			return UNFD_ST_ERR_HAL_R_INVALID_PARAM;
			break;
	}
	pstPNDSpec->nReg50_EccCtrl |= BIT_NC_ECCERR_NSTOP;
	
	nand_printf("NAND_Info:\r\n BlkCnt:%Xh  BlkPageCnt:%Xh  PageByteCnt:%Xh  SpareByteCnt:%Xh \r\n",
		(unsigned int)pstPNDSpec->nNumOfBlks, (unsigned int)pstPNDSpec->nPgsPerBlk, (unsigned int)pstPNDSpec->nPageByteCnt, (unsigned int)pstPNDSpec->nSpareByteCnt);
	nand_printf(" BlkPageCntBits:%Xh  PageByteCntBits:%Xh  SpareByteCntBits:%Xh \r\n",
		(unsigned int)pstPNDSpec->nBlkPageCntBits, (unsigned int)pstPNDSpec->nPageByteCntBits, (unsigned int)pstPNDSpec->nSpareByteCntBits);
	nand_printf(" BlkPageCntMask:%Xh  PageByteCntMask:%Xh  SpareByteCntMask:%Xh \r\n",
		(unsigned int)pstPNDSpec->nBlkPageCntMask, (unsigned int)pstPNDSpec->nPageByteCntMask, pstPNDSpec->nSpareByteCntMask);
	nand_printf(" PageSectorCnt:%Xh  SectorByteCnt:%Xh  SectorSpareByteCnt:%Xh  ECCCodeByteCnt:%Xh \r\n",
		(unsigned int)pstPNDSpec->nPageSectorCnt, (unsigned int)pstPNDSpec->nSectorByteCnt, (unsigned int)pstPNDSpec->nSectorSpareByteCnt, (unsigned int)pstPNDSpec->nECCCodeByteCnt);
    nand_printf(" PageSectorCntBits:%Xh  SectorByteCntBits:%Xh  SectorSpareByteCntBits:%Xh  PageSectorCntMask:%Xh  SectorByteCntMask:%Xh \r\n",
		(unsigned int)pstPNDSpec->nPageSectorCntBits, (unsigned int)pstPNDSpec->nSectorByteCntBits, (unsigned int)pstPNDSpec->nSectorSpareByteCntBits, (unsigned int)pstPNDSpec->nPageSectorCntMask, (unsigned int)pstPNDSpec->nSectorByteCntMask);

	NC_Config(pstPNDSpec);

	return UNFD_ST_SUCCESS;
}

EXPORT_SYMBOL(NC_Init);


void NC_Config(PureNANDSpec *pstPNDSpec)
{
	REG_WRITE_UINT16(NC_SDIO_CTL, pstPNDSpec->nReg1B_SdioCtrl);
	
	REG_WRITE_UINT16(NC_SIGNAL, pstPNDSpec->nReg40_Signal);
	/*sector spare size*/
	REG_WRITE_UINT16(NC_SPARE, pstPNDSpec->nReg48_Spare);
	/* page spare size*/
	REG_WRITE_UINT16(NC_SPARE_SIZE, (UINT16)pstPNDSpec->nReg49_SpareSize);
	/* page size and ECC type*/
	REG_WRITE_UINT16(NC_ECC_CTRL, pstPNDSpec->nReg50_EccCtrl);

	nand_printf("NC_SDIO_CTL: %X \r\n", pstPNDSpec->nReg1B_SdioCtrl);
	nand_printf("NC_SIGNAL: %X \r\n", pstPNDSpec->nReg40_Signal);
	nand_printf("NC_SPARE: %X \r\n", pstPNDSpec->nReg48_Spare);
	nand_printf("NC_SPARE_SIZE: %X \r\n", pstPNDSpec->nReg49_SpareSize);
	nand_printf("NC_ECC_CTRL: %X \r\n", pstPNDSpec->nReg50_EccCtrl);
}

// can not corss block
UINT32 NC_WritePages
(
     PureNANDSpec *pstPNDSpec, UINT32 u32_PhyRowIdx, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf, UINT32 u32_PageCnt
)
{
	#if 0==IF_SPARE_AREA_DMA
	UINT16 u16_Tmp;
	#endif
	
	UINT32 u32_DataDMAAddr;
	#if IF_SPARE_AREA_DMA
	UINT32 u32_SpareDMAAddr=0;
	#endif

	UINT8  u8_RetryCnt_FifoClkRdy=0;

	UINT32 u32_Err;
	UINT32 nRet;

	// can not corss block
	if((u32_PhyRowIdx & pstPNDSpec->nBlkPageCntMask) + u32_PageCnt >
		pstPNDSpec->nNumOfBlks)
	{
		printk("ERROR: NC_ReadPages, ErrCode:%Xh \r\n", UNFD_ST_ERR_HAL_W_INVALID_PARAM);
		return (UINT32)FSR_LLD_INVALID_PARAM;
	}

	NC_LOCK_FCIE();
	NC_PAD_SWITCH(1);

nand_redo_write:

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END|BIT_MIU_LAST_DONE);

	u32_DataDMAAddr = nand_translate_DMA_address_Ex((UINT32)pu8_DataBuf, pstPNDSpec->nPageByteCnt*u32_PageCnt);
	
	#if IF_SPARE_AREA_DMA
	if(pu8_SpareBuf)
		u32_SpareDMAAddr = nand_translate_DMA_address_Ex((UINT32)pu8_SpareBuf, pstPNDSpec->nSpareByteCnt);
    REG_WRITE_UINT16(NC_SPARE_DMA_ADR0, u32_SpareDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_SPARE_DMA_ADR1, u32_SpareDMAAddr >>16);
	#endif

	REG_WRITE_UINT16(NC_JOB_BL_CNT, u32_PageCnt << pstPNDSpec->nPageSectorCntBits);
    REG_WRITE_UINT16(NC_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_SDIO_ADDR1, u32_DataDMAAddr >> 16);
	REG_SET_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);

	/* Check MIU clock is ready */
	if( (u32_Err=NC_waitFifoClkReady()) != UNFD_ST_SUCCESS )
	{
		NC_ResetReconfig(pstPNDSpec);

		if( ++u8_RetryCnt_FifoClkRdy < NAND_TIMEOUT_RETRY_CNT )
			goto nand_redo_write;
		else
		{
			//If soft reset still can not solve this problem, show an alert and return a error
			printk("\033[31mSoft reset over %d times\n, stop!\033[m\n", NAND_TIMEOUT_RETRY_CNT);
			NC_UNLOCK_FCIE();
			return (UINT32)(FSR_LLD_PREV_WRITE_ERROR  | FSR_LLD_1STPLN_CURR_ERROR);
		}
	}

	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pstPNDSpec->OpCode_RW_AdrCycle<< 8) | (CMD_0x80));
	REG_WRITE_UINT16(NC_AUXREG_DAT, (CMD_0x10 << 8) | ACT_SER_DOUT);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (CMD_0x70 << 8) | ACT_WAITRB);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_CHKSTATUS);
	//REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_REPEAT << 8) | ACT_CHKSTATUS);
	//REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
	//REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RPTCNT);
	//REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PageCnt - 1);
	
	#if 0==IF_SPARE_AREA_DMA
	if(pu8_SpareBuf)
	    for(u16_Tmp=0; u16_Tmp < pstPNDSpec->nPageSectorCnt; u16_Tmp++)
		    NC_SetCIFD(pu8_SpareBuf + pstPNDSpec->nSectorSpareByteCnt * u16_Tmp,
		           (UINT32)(pstPNDSpec->nSectorSpareByteCnt * u16_Tmp),
		           (UINT32)(pstPNDSpec->nSectorSpareByteCnt - pstPNDSpec->nECCCodeByteCnt));
	else
		for(u16_Tmp=0; u16_Tmp < pstPNDSpec->nPageSectorCnt; u16_Tmp++)
		    NC_SetCIFD_Const(0xFF, (UINT32)(pstPNDSpec->nSectorSpareByteCnt * u16_Tmp),
		           (UINT32)(pstPNDSpec->nSectorSpareByteCnt - pstPNDSpec->nECCCodeByteCnt));
    #endif

	if( (REG(NC_PATH_CTL) & BIT_NC_EN) == 0 )
	{
		printk("\033[31mBUG()!!! NAND controller is disabled before W job start!!!\033[m\n"); //If the nc_en is disabled, show an alert
		return (UINT32)(FSR_LLD_PREV_WRITE_ERROR  | FSR_LLD_1STPLN_CURR_ERROR);
	}
	
	REG_WRITE_UINT16(NC_CTRL,
		(BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START|BIT_NC_IF_DIR_W));

	if(NC_WaitComplete(BIT_NC_JOB_END, WAIT_WRITE_TIME) == WAIT_WRITE_TIME)
	{
		printk("Error: NC_WritePages Timeout, ErrCode:%Xh \r\n", UNFD_ST_ERR_W_TIMEOUT);

		NC_ResetReconfig(pstPNDSpec);
		
		NC_UNLOCK_FCIE();
		
		return (UINT32)(FSR_LLD_PREV_WRITE_ERROR | FSR_LLD_1STPLN_CURR_ERROR); // timeout
	}

	nRet = NC_CheckEWStatus(OPTYPE_WRITE);
	
	NC_UNLOCK_FCIE();
	return nRet;
}
EXPORT_SYMBOL(NC_WritePages);


// can not corss block
UINT32 NC_ReadPages
(
	PureNANDSpec *pstPNDSpec, UINT32 u32_PhyRowIdx, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf, UINT32 u32_PageCnt
)
{
	UINT16 u16_Tmp=0;
	
	UINT32 u32_DataDMAAddr;
	#if IF_SPARE_AREA_DMA
	UINT32 u32_SpareDMAAddr = 0;
	#endif

	UINT32 u32_Err;
	UINT8 u8_RetryCnt_FifoClkRdy=0, u8_RetryCnt_Events=0, u8_RetryCnt_ECCFail=0;

	UINT32 nRet = FSR_LLD_SUCCESS;

	NC_LOCK_FCIE();
	NC_PAD_SWITCH(1);

    // can not corss block
	if((u32_PhyRowIdx & pstPNDSpec->nBlkPageCntMask) + u32_PageCnt >
		pstPNDSpec->nNumOfBlks)
	{
		printk("ERROR: NC_ReadPages, ErrCode:%Xh \r\n", UNFD_ST_ERR_HAL_R_INVALID_PARAM);
		return (UINT32)FSR_LLD_INVALID_PARAM;
	}

nand_redo_read:

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END|BIT_MIU_LAST_DONE);

	u32_DataDMAAddr = nand_translate_DMA_address_Ex((UINT32)pu8_DataBuf, pstPNDSpec->nPageByteCnt*u32_PageCnt);

	#if IF_SPARE_AREA_DMA
	if(pu8_SpareBuf)
		u32_SpareDMAAddr = nand_translate_DMA_address_Ex((UINT32)pu8_SpareBuf, pstPNDSpec->nSpareByteCnt);
	REG_WRITE_UINT16(NC_SPARE_DMA_ADR0, u32_SpareDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_SPARE_DMA_ADR1, u32_SpareDMAAddr >>16);
	#endif

	REG_WRITE_UINT16(NC_JOB_BL_CNT, u32_PageCnt << pstPNDSpec->nPageSectorCntBits);

   	REG_WRITE_UINT16(NC_SDIO_ADDR0, u32_DataDMAAddr & 0xFFFF);
	REG_WRITE_UINT16(NC_SDIO_ADDR1, u32_DataDMAAddr >> 16);

	REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_NC_DMA_DIR_W);

	/* Check MIU clock is ready */
	if( (u32_Err=NC_waitFifoClkReady()) != UNFD_ST_SUCCESS)
	{
		NC_ResetReconfig(pstPNDSpec);

		if( ++u8_RetryCnt_FifoClkRdy < NAND_TIMEOUT_RETRY_CNT )
			goto nand_redo_read;
		else
		{
			//If soft reset still can not solve this problem, show an alert and return a error
			printk("\033[31mSoft reset over %d times\n, stop!\033[m\n", NAND_TIMEOUT_RETRY_CNT);

			NC_UNLOCK_FCIE();
			return (UINT32)(FSR_LLD_PREV_READ_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
		}
	}

	REG_SET_BITS_UINT16(NC_PATH_CTL, BIT_MMA_EN);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pstPNDSpec->OpCode_RW_AdrCycle<< 8) | (CMD_0x00));

	if(pstPNDSpec->nReg48_Spare& BIT_NC_ONE_COL_ADDR) // if a page 512B
	{
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_SER_DIN << 8) | ACT_WAITRB);
		REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
		//REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_REPEAT);
	}
	else
	{
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_SER_DIN);
		//REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_REPEAT << 8) | ACT_SER_DIN);
		//REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
	}
	//REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RPTCNT);
	//REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PageCnt - 1);

	// Ensure the NC_EN is not been disabled
	if( (REG(NC_PATH_CTL) & BIT_NC_EN) == 0 )
	{
		printk("\033[31mBUG()!!! NAND controller is disabled before R job start!!!\033[m\n"); //If the nc_en is disabled, show an alert
		return (UINT32)(FSR_LLD_PREV_READ_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
	}

	// check & set - patch for MIU busy case 
	// slow down FCIE clock to wait for MIU service, 
	// enhance the retry (more reliable) for error handling.
	if(0!=u8_RetryCnt_Events || 0!=u8_RetryCnt_ECCFail) 
	{
		nand_clock_setting(FCIE3_SW_SLOWEST_CLK);
		REG_WRITE_UINT16(NC_WIDTH, 0);
		//nand_printf("check & set - patch for MIU busy case \n");
	}

	REG_WRITE_UINT16(NC_CTRL, (BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START) & ~(BIT_NC_IF_DIR_W));

	if (NC_WaitComplete(BIT_NC_JOB_END, WAIT_READ_TIME * u32_PageCnt) ==
		(WAIT_READ_TIME * u32_PageCnt ))
	{
		printk("Error: NC_ReadPages Timeout %d, ErrCode:%Xh \r\n", u8_RetryCnt_Events+1,UNFD_ST_ERR_R_TIMEOUT);

		NC_ResetReconfig(pstPNDSpec);

		if( ++u8_RetryCnt_Events < NAND_TIMEOUT_RETRY_CNT )
        	goto nand_redo_read;
		else
		{
			printk("\033[31mSoft reset over %d times\n, stop!\033[m\n", NAND_TIMEOUT_RETRY_CNT); 
			
			//If soft reset still can not solve this problem, show an alert and return a error
			// restore the normal setting before return
			nand_clock_setting(FCIE3_SW_DEFAULT_CLK);
			REG_WRITE_UINT16(NC_WIDTH, FCIE_REG41_VAL);

			NC_UNLOCK_FCIE();
			
			return (UINT32)(FSR_LLD_PREV_READ_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
		}
	}

	// restore - patch for MIU busy case
	// for retry ok, restore the normal setting
	if(0!=u8_RetryCnt_Events || 0!=u8_RetryCnt_ECCFail) 
	{
		nand_clock_setting(FCIE3_SW_DEFAULT_CLK);
		REG_WRITE_UINT16(NC_WIDTH, FCIE_REG41_VAL);
		//nand_printf("restore - patch for MIU busy case \n");
	}

	// Check ECC
	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	if(u16_Tmp & BIT_NC_ECC_FAIL)
	{
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
			(TEXT("Error: NC_ReadPages ECC Fail,u32_PhyRowIdx : %u, @Sector %d\nReg51:%04Xh\n"), 
			u32_PhyRowIdx, (u16_Tmp>>8)&0x3F, u16_Tmp));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
			(TEXT("Reg52:%04Xh\n")
			, REG(NC_ECC_STAT1)));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
			(TEXT("Reg53:%04Xh\n"), REG(NC_ECC_STAT2)));

		// add retry for ECC error
		if( u8_RetryCnt_ECCFail < NAND_TIMEOUT_RETRY_CNT)
		{
			//NC_ResetReconfig(pstPNDSpec);
			u8_RetryCnt_ECCFail++;

			FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("Retry times : %X\n"), u8_RetryCnt_ECCFail));

			goto nand_redo_read;
		}
		else
		{
			FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("ERROR, NAND, RETRY_READ_ECC_FAIL over 3 times \n")));

			NC_UNLOCK_FCIE();
		}
		
		return (UINT32)(FSR_LLD_PREV_READ_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
	}
	else if((u16_Tmp&BIT_NC_ECC_MAX_BITS_MASK)>>BIT_NC_ECC_MAX_BITS_SHIFT)
	{
		nRet = (UINT32)(FSR_LLD_PREV_1LV_READ_DISTURBANCE | FSR_LLD_1STPLN_CURR_ERROR);
	}

	// check until MIU is done
	if( (u32_Err=NC_wait_MIULastDone()) != UNFD_ST_SUCCESS)
	{
		return (UINT32)(FSR_LLD_PREV_READ_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
	}

	#if 0==IF_SPARE_AREA_DMA
	if(pu8_SpareBuf)
		NC_GetCIFD(pu8_SpareBuf, 0, pstPNDSpec->nSpareByteCnt);
	#endif

	NC_UNLOCK_FCIE();
	
	return nRet;
}
EXPORT_SYMBOL(NC_ReadPages);


UINT32 NC_Read_RandomIn
(
     PureNANDSpec *pstPNDSpec, UINT32 u32_PhyRow, UINT32 u32Col, UINT8 *pu8DataBuf, UINT32 u32DataByteCnt
)
{
	UINT32 u32_Tmp;

	#if IF_SPARE_AREA_DMA
	UINT32 u32_DataDMAAddr;
	#endif

	NC_LOCK_FCIE();
	NC_PAD_SWITCH(1);
	
	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END);

	#if IF_SPARE_AREA_DMA
    pstPNDSpec->nReg48_Spare &= ~BIT_NC_SPARE_DEST_MIU;
	#endif
	pstPNDSpec->nReg48_Spare |= BIT_NC_RANDOM_RW_OP_EN;
	REG_WRITE_UINT16(NC_SPARE, pstPNDSpec->nReg48_Spare);

	#if IF_SPARE_AREA_DMA
    REG_SET_BITS_UINT16(NC_SDIO_CTL, BIT_SDIO_BLK_MODE| u32DataByteCnt);

    HAL_WRITE_UINT16(NC_JOB_BL_CNT, 1);

	u32_DataDMAAddr = nand_translate_DMA_address_Ex((UINT32)pu8DataBuf, u32DataByteCnt);
	
    HAL_WRITE_UINT16(NC_SPARE_DMA_ADR0, u32_DataDMAAddr&0xFFFF);
    HAL_WRITE_UINT16(NC_SPARE_DMA_ADR1, u32_DataDMAAddr>>16);

    HAL_WRITE_UINT16(NC_MMA_PRI_REG, 0);
    HAL_WRITE_UINT16(NC_PATH_CTL, BIT_MMA_EN | BIT_NC_EN);
	#endif

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	if(pstPNDSpec->nReg48_Spare & BIT_NC_ONE_COL_ADDR)
		REG_WRITE_UINT16(NC_AUXREG_DAT, u32Col<<8);
	else
		REG_WRITE_UINT16(NC_AUXREG_DAT, u32Col);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRow & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRow >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pstPNDSpec->OpCode_RW_AdrCycle << 8) | (CMD_0x00));
	if(pstPNDSpec->nReg48_Spare & BIT_NC_ONE_COL_ADDR)
	{
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_RAN_DIN << 8) | ACT_WAITRB);
		REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);
	}
	else
	{
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0x30);
		REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8) | ACT_RAN_DIN);
	}

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RAN_CNT);
	u32_Tmp = u32DataByteCnt + (u32DataByteCnt & 0x01); // byet-count needs to be word-alignment
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_Tmp);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0); // offset;

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START);
	if(NC_WaitComplete(BIT_NC_JOB_END, WAIT_READ_TIME) == WAIT_READ_TIME)
	{
		printk("Err: drvNand_ReadRandomData_Ex - timeout \r\n");
		NC_ResetFCIE();
		NC_ResetNandFlash();
		#if IF_SPARE_AREA_DMA
        pstPNDSpec->nReg48_Spare |= BIT_NC_SPARE_DEST_MIU;
	    #endif
		pstPNDSpec->nReg48_Spare &= ~BIT_NC_RANDOM_RW_OP_EN;
		NC_Config(pstPNDSpec);

		NC_UNLOCK_FCIE();

		return (UINT32)(FSR_LLD_PREV_READ_ERROR | FSR_LLD_1STPLN_CURR_ERROR); // timeout
	}

	
	#if IF_SPARE_AREA_DMA
	REG_WRITE_UINT16(NC_SDIO_CTL, pstPNDSpec->nReg1B_SdioCtrl);
    pstPNDSpec->nReg48_Spare |= BIT_NC_SPARE_DEST_MIU;
	#else
	/* get data from CIFD */
	NC_GetCIFD(pu8DataBuf, 0, u32DataByteCnt);
	#endif
	pstPNDSpec->nReg48_Spare &= ~BIT_NC_RANDOM_RW_OP_EN;
	REG_WRITE_UINT16(NC_SPARE, pstPNDSpec->nReg48_Spare);

	NC_UNLOCK_FCIE();
	
	return FSR_LLD_SUCCESS;
}
EXPORT_SYMBOL(NC_Read_RandomIn);

UINT32 NC_Write_RandomOut
(
    PureNANDSpec *pstPNDSpec, UINT32 u32_PhyRow, UINT32 u32_Col, UINT8 *pu8_DataBuf, UINT32 u32_DataByteCnt
)
{
	UINT32 u32_Tmp;
	UINT32 u32Ret;

	NC_LOCK_FCIE();
	NC_PAD_SWITCH(1);

	REG_WRITE_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END|BIT_MMA_DATA_END);
    #if IF_SPARE_AREA_DMA
	pstPNDSpec->nReg48_Spare &= ~BIT_NC_SPARE_DEST_MIU;
    #endif
	pstPNDSpec->nReg48_Spare |= BIT_NC_RANDOM_RW_OP_EN;
	REG_WRITE_UINT16(NC_SPARE, pstPNDSpec->nReg48_Spare);

	// set data into CIFD
	NC_SetCIFD(pu8_DataBuf, 0, u32_DataByteCnt);
	if (u32_DataByteCnt & 1)
	{
		UINT8 au8_Tmp[1];
		au8_Tmp[0] = 0xFF; // pad a 0xFF
		NC_SetCIFD(au8_Tmp, u32_DataByteCnt, 1);
	}

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);

	if (pstPNDSpec->nReg48_Spare & BIT_NC_ONE_COL_ADDR)
		REG_WRITE_UINT16(NC_AUXREG_DAT, u32_Col<<8);
	else
		REG_WRITE_UINT16(NC_AUXREG_DAT, u32_Col);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRow & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRow >> 16);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (pstPNDSpec->OpCode_RW_AdrCycle << 8) | (CMD_0x80));
	REG_WRITE_UINT16(NC_AUXREG_DAT, (CMD_0x10 << 8) | ACT_RAN_DOUT);

    REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_BREAK << 8)| ACT_WAITRB);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_RAN_CNT);
	u32_Tmp = u32_DataByteCnt + (u32_DataByteCnt & 0x01); // byet-count needs to be word-alignment
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_Tmp);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);	// offset;

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_CIFD_ACCESS|BIT_NC_JOB_START|BIT_NC_IF_DIR_W);
	if (NC_WaitComplete(BIT_NC_JOB_END, WAIT_WRITE_TIME) == WAIT_WRITE_TIME)
	{
		printk("Error: NC_Write_RandomOut Timeout, ErrCode:%Xh \r\n", UNFD_ST_ERR_W_TIMEOUT);
        #if 0==IF_IP_VERIFY
		NC_ResetFCIE();
		NC_ResetNandFlash();
        	#if IF_SPARE_AREA_DMA
			pstPNDSpec->nReg48_Spare |= BIT_NC_SPARE_DEST_MIU;
        	#endif
		pstPNDSpec->nReg48_Spare &= ~BIT_NC_RANDOM_RW_OP_EN;
		NC_Config(pstPNDSpec);
		#endif

		NC_UNLOCK_FCIE();
		
		return (UINT32)FSR_LLD_PREV_WRITE_ERROR; // timeout
	}

    #if IF_SPARE_AREA_DMA
	pstPNDSpec->nReg48_Spare |= BIT_NC_SPARE_DEST_MIU;
    #endif
	pstPNDSpec->nReg48_Spare &= ~BIT_NC_RANDOM_RW_OP_EN;
	REG_WRITE_UINT16(NC_SPARE, pstPNDSpec->nReg48_Spare);
	
	u32Ret = NC_CheckEWStatus(OPTYPE_WRITE);

	NC_UNLOCK_FCIE();

	return u32Ret;
	
}
EXPORT_SYMBOL(NC_Write_RandomOut);


UINT32 NC_EraseBlk( PureNANDSpec     *pstPNDSpec , UINT32 u32_PhyRowIdx)
{
	UINT32 nRet;

	NC_LOCK_FCIE();
	NC_PAD_SWITCH(1);
	
	REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_ADRSET);
	REG_WRITE_UINT16(NC_AUXREG_DAT, 0);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx & 0xFFFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, u32_PhyRowIdx >> 16);

    REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT,	(pstPNDSpec->OpCode_Erase_AdrCycle << 8) | CMD_0x60);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0xD0);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_CHKSTATUS << 8) | CMD_0x70);
	REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);

	if( (REG(NC_PATH_CTL) & BIT_NC_EN) == 0 )
	{
		printk("\033[31mBUG()!!! NAND controller is disabled before E job start!!!\033[m\n"); //If the nc_en is disabled, show an alert
		return (UINT32)FSR_LLD_PREV_READ_ERROR;
	}
	
    REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);
	if(NC_WaitComplete(BIT_NC_JOB_END, WAIT_ERASE_TIME) == WAIT_ERASE_TIME)
	{
		printk("Error: NC_EraseBlk Timeout, ErrCode:%Xh \r\n", UNFD_ST_ERR_E_TIMEOUT);
		NC_ResetFCIE();
		NC_ResetNandFlash();
		NC_Config(pstPNDSpec);

		NC_UNLOCK_FCIE();
		
		return (UINT32)(FSR_LLD_PREV_ERASE_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
	}

	nRet = NC_CheckEWStatus(OPTYPE_ERASE);
	
	NC_UNLOCK_FCIE();
	
	return nRet;
}
EXPORT_SYMBOL(NC_EraseBlk);


UINT32 NC_ResetNandFlash(void)
{
	NC_PAD_SWITCH(1);
	
	REG_W1C_BITS_UINT16(NC_MIE_EVENT, BIT_NC_JOB_END);

	REG_WRITE_UINT16(NC_AUXREG_ADR, AUXADR_INSTQUE);
	REG_WRITE_UINT16(NC_AUXREG_DAT, (ACT_WAITRB << 8) | CMD_0xFF);
	REG_WRITE_UINT16(NC_AUXREG_DAT, ACT_BREAK);

	REG_WRITE_UINT16(NC_CTRL, BIT_NC_JOB_START);

	if(NC_WaitComplete(BIT_NC_JOB_END, WAIT_RESET_TIME) == WAIT_RESET_TIME)
	{
		printk("ERROR: NC_ResetNandFlash, ErrCode:%Xh \r\n", UNFD_ST_ERR_RST_TIMEOUT);
		return UNFD_ST_ERR_RST_TIMEOUT;
	}
	
	return UNFD_ST_SUCCESS;
}


UINT32 NC_WaitComplete(UINT16 u16_WaitEvent, UINT32 u32_MicroSec)
{
#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
	volatile UINT32 u32_Err;
#endif
	volatile UINT16 u16_Reg;

#if defined(ENABLE_NAND_INTERRUPT_MODE) && ENABLE_NAND_INTERRUPT_MODE
	REG_READ_UINT16(NC_MIE_INT_EN, u16_Reg);
	if(u16_Reg & u16_WaitEvent)
	{
		u32_Err = nand_WaitCompleteIntr(u16_WaitEvent, u32_MicroSec);
		if( u32_Err == UNFD_ST_SUCCESS )
			return 0;
		else
		{
			printk("[NC_DEBUG_DBUS0] : 0x%02X\r\n", REG(NC_DEBUG_DBUS0));
			printk("[NC_DEBUG_DBUS1] : 0x%02X\r\n", REG(NC_DEBUG_DBUS1));
			
			return u32_MicroSec;
		}
	}
	return u32_MicroSec;
#else
	volatile UINT32 u32_Count;
	for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
	{
		REG_READ_UINT16(NC_MIE_EVENT, u16_Reg);
		if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
			break;

		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
		//nand_reset_WatchDog();
	}

	if (u32_Count < u32_MicroSec)
		REG_W1C_BITS_UINT16(NC_MIE_EVENT, u16_WaitEvent); /*clear events*/
	else
	{
		printk("[NC_DEBUG_DBUS0] : 0x%02X\r\n", REG(NC_DEBUG_DBUS0));
		printk("[NC_DEBUG_DBUS1] : 0x%02X\r\n", REG(NC_DEBUG_DBUS1));
	}

	return u32_Count;
#endif

}

void NC_SetCIFD_Const(UINT8 u8_Data, UINT32 u32_CIFDPos, UINT32 u32_ByteCnt)
{
    UINT32 u32_i;
	volatile UINT16 u16_Tmp;

	if(u32_CIFDPos & 1)
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u16_Tmp &= 0x00FF;
		u16_Tmp += u8_Data << 8;
		REG_WRITE_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
	}

	for(u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++)
	{
		u16_Tmp = u8_Data + (u8_Data << 8);
		REG_WRITE_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}

	if(u32_ByteCnt - (u32_i<<1))
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		u16_Tmp = (u16_Tmp & 0xFF00) + u8_Data;
		REG_WRITE_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}
}


void NC_SetCIFD(UINT8 *pu8_Buf, UINT32 u32_CIFDPos, UINT32 u32_ByteCnt)
{
    UINT32 u32_i, u32_BufPos;
	volatile UINT16 u16_Tmp;

	if(u32_CIFDPos & 1)
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u16_Tmp &= 0x00FF;
		u16_Tmp += pu8_Buf[0] << 8;
		REG_WRITE_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
		u32_BufPos = 1;
	}
	else  u32_BufPos = 0;

	for(u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++)
	{
		u16_Tmp = pu8_Buf[(u32_i<<1)+u32_BufPos] +
			     (pu8_Buf[(u32_i<<1)+u32_BufPos+1] << 8);
		REG_WRITE_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}

	if(u32_ByteCnt - (u32_i<<1))
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		u16_Tmp = (u16_Tmp & 0xFF00) + pu8_Buf[(u32_i<<1)+u32_BufPos];
		REG_WRITE_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
	}
}

void NC_GetCIFD(UINT8 *pu8_Buf, UINT32 u32_CIFDPos, UINT32 u32_ByteCnt)
{
    UINT32 u32_i;
	UINT16 *ptr = (UINT16 *)pu8_Buf;

#if 1
	for(u32_i=0; u32_i<u32_ByteCnt; u32_i+=2)
		REG_READ_UINT16(NC_CIFD_ADDR((u32_CIFDPos+u32_i)>>1), ptr[u32_i>>1]);

#else
	UINT32 u32_BufPos;
	UINT16 u16_Tmp;

	if(u32_CIFDPos & 1)
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_CIFDPos>>1), u16_Tmp);
		pu8_Buf[0] = (UINT8)(u16_Tmp >> 8);
		u32_CIFDPos += 1;
		u32_ByteCnt -= 1;
		u32_BufPos = 1;
	}
	else
		u32_BufPos = 0;

	for(u32_i=0; u32_i<u32_ByteCnt>>1; u32_i++)
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (UINT8)u16_Tmp;
		pu8_Buf[(u32_i<<1)+u32_BufPos+1] = (UINT8)(u16_Tmp>>8);
	}

	if(u32_ByteCnt - (u32_i<<1))
	{
		REG_READ_UINT16(NC_CIFD_ADDR(u32_i+(u32_CIFDPos>>1)), u16_Tmp);
		pu8_Buf[(u32_i<<1)+u32_BufPos] = (UINT8)u16_Tmp;
	}
#endif
}


static UINT32 NC_CheckEWStatus(UINT8 u8_OpType)
{
	volatile UINT16 u16_Tmp;
	UINT32 u32_ErrCode = 0;

	REG_READ_UINT16(NC_ST_READ, u16_Tmp);

	if((u16_Tmp & BIT_ST_READ_FAIL) == 1) // if fail
	{
		if(OPTYPE_ERASE == u8_OpType)
			u32_ErrCode = (UINT32)(FSR_LLD_PREV_ERASE_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
		else if(OPTYPE_WRITE == u8_OpType)
			u32_ErrCode = (UINT32)(FSR_LLD_PREV_WRITE_ERROR | FSR_LLD_1STPLN_CURR_ERROR);

		printk("ERROR: NC_CheckEWStatus Fail, Nand St:%Xh, ErrCode:%Xh \r\n",
			REG(NC_ST_READ), (unsigned int)u32_ErrCode);
		return u32_ErrCode;
	}
	else if((u16_Tmp & BIT_ST_READ_BUSYn) == 0) // if busy
	{
		if(OPTYPE_ERASE == u8_OpType)
			u32_ErrCode = (UINT32)(FSR_LLD_PREV_ERASE_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
		else if(OPTYPE_WRITE == u8_OpType)
			u32_ErrCode = (UINT32)(FSR_LLD_PREV_WRITE_ERROR | FSR_LLD_1STPLN_CURR_ERROR);

		printk("ERROR: NC_CheckEWStatus Busy, Nand St:%Xh, ErrCode:%Xh \r\n",
			REG(NC_ST_READ), (unsigned int)u32_ErrCode);
		return u32_ErrCode;
	}
	else if((u16_Tmp & BIT_ST_READ_PROTECTn) == 0) // if protected
	{
		if(OPTYPE_ERASE == u8_OpType)
			u32_ErrCode = (UINT32)(FSR_LLD_PREV_ERASE_ERROR | FSR_LLD_1STPLN_CURR_ERROR);
		else if(OPTYPE_WRITE == u8_OpType)
			u32_ErrCode = (UINT32)(FSR_LLD_PREV_WR_PROTECT_ERROR | FSR_LLD_1STPLN_CURR_ERROR);

		printk("ERROR: NC_CheckEWStatus Protected, Nand St:%Xh, ErrCode:%Xh \r\n",
			REG(NC_ST_READ), (unsigned int)u32_ErrCode);
		return u32_ErrCode;
	}

	return FSR_LLD_SUCCESS;
}

UINT32 NC_PlatformResetPre(void)
{
	return UNFD_ST_SUCCESS;
}

UINT32 NC_PlatformResetPost(void)
{
	UINT16 u16_Reg;

	REG_READ_UINT16(FCIE_NC_CIFD_BASE, u16_Reg); // dummy read for CIFD clock

	#if defined(DUTY_CYCLE_PATCH)&&DUTY_CYCLE_PATCH
	REG_WRITE_UINT16(NC_WIDTH, FCIE_REG41_VAL);	// duty cycle 3:1 in 86Mhz
	#endif

	#if defined(REG57_ECO_FIX_INIT_VALUE)
	REG_WRITE_UINT16(NC_LATCH_DATA, REG57_ECO_FIX_INIT_VALUE);
	#endif

    return UNFD_ST_SUCCESS;
}

UINT32 NC_ResetFCIE(void)
{
	volatile UINT16 u16Reg, u16Cnt/*, u16Reg41h*/;

	NC_PlatformResetPre();

	REG_WRITE_UINT16(NC_SIGNAL, ~DEF_REG0x40_VAL);

    // soft reset
	REG_CLR_BITS_UINT16(NC_TEST_MODE, BIT_FCIE_SOFT_RST); /* active low */
	nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	REG_SET_BITS_UINT16(NC_TEST_MODE, BIT_FCIE_SOFT_RST);
	
	// make sure reset complete
	u16Cnt=0;
	do
	{
		nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	   	if(0x1000 == u16Cnt++)
		{
		    printf("ERROR: NC_ResetFCIE, ErrCode: 0x%03X\n", UNFD_ST_ERR_NO_NFIE);
		    return UNFD_ST_ERR_NO_NFIE;
		}

		REG_READ_UINT16(NC_SIGNAL, u16Reg);
	} while (DEF_REG0x40_VAL != u16Reg );

	// miu request reset
	REG_SET_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST); 
	nand_hw_timer_delay(HW_TIMER_DELAY_1us);
	REG_CLR_BITS_UINT16(NC_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

	NC_PlatformResetPost();
	
	return UNFD_ST_SUCCESS; // ok
}

/*--------------------------------------------------
  get ECC corrected bits count

return: 0xFFFFFFFF -> ECC uncorrectable error,
other: max ECC corrected bits (within the readed sectors).
--------------------------------------------------*/
UINT32  NC_GetECCBits(void)
{
	UINT32 u16_Tmp;

	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);

	return (u16_Tmp & BIT_NC_ECC_MAX_BITS_MASK) >> 1;
}

VOID  NC_ControlECC(BOOL32 ecc_on)
{
	UINT16 u16Tmp;
	
	if(ecc_on == FALSE32) {
		REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
		REG_WRITE_UINT16(NC_ECC_CTRL, u16Tmp | (0x400));
		REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
	}
	else if (ecc_on == TRUE32)
	{
		REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
		REG_WRITE_UINT16(NC_ECC_CTRL, u16Tmp & ~(0x400));
		REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
	}
}
EXPORT_SYMBOL(NC_ControlECC);

void NC_DisableECC(void)
{
	UINT16 u16Tmp;

	REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
	REG_WRITE_UINT16(NC_ECC_CTRL, u16Tmp | (0x400));
	REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);

}
EXPORT_SYMBOL(NC_DisableECC);

void NC_EnableECC(void)
{
	UINT16 u16Tmp;

	REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
	REG_WRITE_UINT16(NC_ECC_CTRL, u16Tmp & ~(0x400));
	REG_READ_UINT16(NC_ECC_CTRL, u16Tmp);
}
EXPORT_SYMBOL(NC_EnableECC);

#if 0  // MsX5 ECC
void NC_DisableECC(void)
{
	UINT32 u16_Tmp;
	//NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNand_get_DrvContext_address();

	#if 0
	REG_READ_UINT16(NC_ECC_CTRL, pstPNDSpec->nReg50_EccCtrl);
	pstPNDSpec->nReg50_EccCtrl |= BIT_NC_BYPASS_ECC;
	REG_WRITE_UINT16(NC_ECC_CTRL, pstPNDSpec->nReg50_EccCtrl);
	#else
	REG_READ_UINT16(NC_ECC_CTRL, u16_Tmp);
	u16_Tmp |= BIT_NC_BYPASS_ECC;
	REG_WRITE_UINT16(NC_ECC_CTRL, u16_Tmp);
	#endif
}

void NC_EnableECC(void)
{
	UINT32 u16_Tmp;
	//NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNand_get_DrvContext_address();

	#if 0
	REG_READ_UINT16(NC_ECC_CTRL, pstPNDSpec->nReg50_EccCtrl);
	pstPNDSpec->nReg50_EccCtrl &= (~BIT_NC_BYPASS_ECC);
	REG_WRITE_UINT16(NC_ECC_CTRL, pstPNDSpec->nReg50_EccCtrl);
	#else
	REG_READ_UINT16(NC_ECC_CTRL, u16_Tmp);
	u16_Tmp &= (~BIT_NC_BYPASS_ECC);
	REG_WRITE_UINT16(NC_ECC_CTRL, u16_Tmp);
	#endif
}
#endif

void print_ECC_Reg(void)
{
	UINT16 u16_Tmp=0;
	/*
	   Reg1B => SDIO block mode
	   Reg48 => spare size / ECC bypass
	   Reg49 => nand address
	   Reg50 => ECC Error NonStop

	   Reg4e => nand spare memory address[15:0]
	   Reg4f => nana spare memory address[27:16]
	   Reg51 => ECC uncorrectable error duing DMA
	   Reg52 => total sector count of which has ECC correctable error during DMA
	   Reg53 => Number of correctable error
	   Reg54 => ECC Error location
	*/
	/*
	#define NC_SDIO_CTL                         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x1B)
	#define NC_SPARE                            GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x48)
	#define NC_SPARE_SIZE                       GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x49)
	#define NC_ECC_CTRL                         GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x50)

	#define NC_SPARE_DMA_ADR0                   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4E)
	#define NC_SPARE_DMA_ADR1                   GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x4F)
	#define NC_ECC_STAT0                        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x51)
	#define NC_ECC_STAT1                        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x52)
	#define NC_ECC_STAT2                        GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x53)
	#define NC_ECC_LOC                          GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x54)
	*/
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT("\r\n")));

	REG_READ_UINT16(NC_SDIO_CTL, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg1b:%04Xh\r\n"), "NC_SDIO_CTL", u16_Tmp));

	REG_READ_UINT16(NC_SPARE, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg48:%04Xh\r\n"), "NC_SPARE", u16_Tmp));

	REG_READ_UINT16(NC_SPARE_SIZE, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg49:%04Xh\r\n"), "NC_SPARE_SIZE", u16_Tmp));

	REG_READ_UINT16(NC_ECC_CTRL, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg50:%04Xh\r\n"), "NC_ECC_CTRL", u16_Tmp));

	REG_READ_UINT16(NC_SPARE_DMA_ADR0, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg4e:%04Xh\r\n"), "NC_SPARE_DMA_ADR0 ", u16_Tmp));

	REG_READ_UINT16(NC_SPARE_DMA_ADR1, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg4f:%04Xh\r\n"), "NC_SPARE_DMA_ADR1", u16_Tmp));

	REG_READ_UINT16(NC_ECC_STAT0, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg51:%04Xh\r\n"), "NC_ECC_STAT0", u16_Tmp));

	REG_READ_UINT16(NC_ECC_STAT1, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg52:%04Xh\r\n"), "NC_ECC_STAT1", u16_Tmp));

	REG_READ_UINT16(NC_ECC_STAT2, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg53:%04Xh\r\n"), "NC_ECC_STAT2", u16_Tmp));

	REG_READ_UINT16(NC_ECC_LOC, u16_Tmp);
	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20sReg54:%04Xh\r\n"), "NC_ECC_LOC", u16_Tmp));

	FSR_DBZ_RTLMOUT(FSR_DBZ_INF, (TEXT(" %-20s%u\r\n"), "NC_GetECCBits", NC_GetECCBits()));
}
EXPORT_SYMBOL(print_ECC_Reg);
