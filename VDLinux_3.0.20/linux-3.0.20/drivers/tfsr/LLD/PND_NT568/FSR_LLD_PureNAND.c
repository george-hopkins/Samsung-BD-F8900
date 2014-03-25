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

#include    <FSR.h>
#include    "FSR_LLD_PureNAND.h"

#include    "FSR_LLD_NTK_568.h"


//TODO remove to use the correct settings
#define MAIN_DATA_SIZE	(FSR_SECTOR_SIZE * FSR_MAX_PHY_SCTS/2)
#define SPARE_DATA_SIZE	(FSR_SPARE_SIZE * FSR_MAX_PHY_SCTS/2)
#define CACHE_LINE		(64)

#define SUPPORT_FREE_RUN	
//#define RDUMP
void dump_ECC_Reg(void);
//#define TRACE
#ifdef TRACE
#define N_HALT_DEBUG(_fmt, args...)  \
	do{\
		FSR_OAM_DbgMsg("\n{%s%d}"_fmt, __FUNCTION__,__LINE__,args);\
	}\
while(0)
#else //#ifdef TRACE
#define N_HALT_DEBUG(_fmt, args...)  
#endif//#ifdef TRACE

#if defined(__KERNEL__) && !defined(__U_BOOT__)
#include <asm/wbflush.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/delay.h>

#define NFC_USE_INTERRUPT
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <asm/mips-boards/nvt72568.h>

#endif

#ifndef __KERNEL__
#define CACHE_INV(buf,len)	inv_dcache_range((buf),(len))
#define CACHE_WB(buf,len)	wb_dcache_range((buf),(len))
#else
#define CACHE_INV(buf,len) 	_dma_cache_inv((UINT32)(buf),(len))
#define CACHE_WB(buf,len) 	_dma_cache_wback_inv((UINT32)(buf),(len))
#endif

#ifndef __KERNEL__
#define EXPORT_SYMBOL(name)
#define external_sync()
#endif

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
#define     FSR_PND_BLOCK_SIZE_1MB              (3)

#define     FSR_PND_VALID_BLK_MARK              (0xFFFF) /* x16 */

#define     FSR_PND_ECC_READ_DISTURBANCE        (2)

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

#define FSR_PND_ENABLE_ECC                           (0x0)
#define FSR_PND_DISABLE_ECC                          (0x1)

#define SWECC_E_ERROR                                (1)
#define SWECC_N_ERROR                                (0)
#define SWECC_C_ERROR                                (-1)
#define SWECC_U_ERROR                                (-2)
#define SWECC_R_ERROR                                (-3)


/******************************************************************************/
/* Local typedefs                                                             */
/******************************************************************************/
/** @brief   Shared data structure for communication in Dual Core             */
typedef struct
{
	UINT32  nShMemUseCnt;

	/**< Previous operation data structure which can be shared among process  */
	UINT16  nPreOp[FSR_MAX_DIES];
	UINT16  nPreOpPbn[FSR_MAX_DIES];
	UINT16  nPreOpPgOffset[FSR_MAX_DIES];
	UINT32  nPreOpFlag[FSR_MAX_DIES];

#if defined (FSR_LLD_READ_PRELOADED_DATA)
	UINT32  bIsPreCmdLoad[FSR_MAX_DIES];
#endif

} FlexONDShMem;

typedef struct
{
	UINT32  nShMemUseCnt;

	/**< Previous operation data structure which can be shared among process  */
	UINT16  nPreOp[FSR_MAX_DIES];
	UINT16  nPreOpPbn[FSR_MAX_DIES];
	UINT16  nPreOpPgOffset[FSR_MAX_DIES];
	UINT32  nPreOpFlag[FSR_MAX_DIES];
} OneNANDShMem;

typedef struct
{
	UINT32  nShMemUseCnt;

	/**< Previous operation data structure which can be shared among process  */
	UINT16  nPreOp[FSR_MAX_DIES];
	UINT16  nPreOpPbn[FSR_MAX_DIES];
	UINT16  nPreOpPgOffset[FSR_MAX_DIES];
	UINT32  nPreOpFlag[FSR_MAX_DIES];
} OneNAND4kShMem;

struct FlexONDShMem *gpstFNDShMem[4];
struct OneNANDShMem *gpstONDShMem[4];
struct OneNAND4kShMem *gpstOND4kShMem[4];

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
					       otherwise, this is 0          */

	/* 
	 * TrTime, TwTime of MLC are array of size 2
	 * First  element is for LSB TLoadTime, TProgTime
	 * SecPND element is for MLB TLoadTime, TProgTime
	 * Use macro FSR_LLD_IDX_LSB_TIME, FSR_LLD_IDX_MSB_TIME
	 */
	UINT32          nSLCTLoadTime;      /**< Typical Load     operation time  */    
	UINT32          nSLCTProgTime;      /**< Typical Program  operation time  */    
	UINT32          nTEraseTime;        /**< Typical Erase    operation time  */
} PureNANDSpec;

typedef struct
{
	UINT32          nShMemUseCnt;   

	/**< Previous operation data structure which can be shared among process  */
	UINT16          nPreOp[FSR_MAX_DIES];
	UINT16          nPreOpPbn[FSR_MAX_DIES];
	UINT16          nPreOpPgOffset[FSR_MAX_DIES];
	UINT32          nPreOpFlag[FSR_MAX_DIES];
	UINT32	        ECCnRE;

	/**< Pseudo DataRAM. FIXME: The size is 4KB, It is difficult to allocate shared memory  */
	UINT8           pPseudoMainDataRAM[MAIN_DATA_SIZE] __attribute__ ((aligned (64)));
	UINT8           pPseudoSpareDataRAM[SPARE_DATA_SIZE] __attribute__ ((aligned (64)));

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

#if defined (FSR_LLD_LOGGING_HISTORY) && !defined(FSR_OAM_RTLMSG_DISABLE)
PRIVATE const UINT8    *gpszLogPreOp[] =
{ (UINT8 *) "NONE ",
	(UINT8 *) "READ ",
	(UINT8 *) "CLOAD",
	(UINT8 *) "CAPGM",
	(UINT8 *) "PROG ",
	(UINT8 *) "CBPGM",
	(UINT8 *) "ERASE",
	(UINT8 *) "IOCTL",
	(UINT8 *) "HTRST",
};
#endif

PRIVATE PureNANDCxt    *gpstPNDCxt  [FSR_PND_MAX_DEVS];

#if defined(WITH_TINY_FSR)
#define LLD_FUNC					extern
extern  PureNANDShMem  *gpstPNDShMem[FSR_PND_MAX_DEVS];
extern UINT32 gEnableECC;
extern UINT32 gNFC_IRQInit;
#else
#define LLD_FUNC
PureNANDShMem  *gpstPNDShMem[FSR_PND_MAX_DEVS];
UINT32 gEnableECC = 1;
EXPORT_SYMBOL(gEnableECC);

//TODO modify
UINT32 gNFC_IRQInit = 0;
EXPORT_SYMBOL(gNFC_IRQInit);
#endif

/** @brief   Shared data structure for communication in Dual Core             */


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


UINT32 S_REG_NFC_CFG0 ;
UINT32 S_REG_NFC_CFG0_RR ;				//This CFG0 is setting for random read
UINT32 S_REG_NFC_CFG1 ;
UINT32 S_REG_NFC_SYSCTRL ;
UINT32 S_REG_NFC_SYSCTRL1 ;
UINT32 S_REG_NFC_SYSCTRL2 ;
UINT32 S_REG_NFC_Fine_Tune ;

/******************************************************************************/
/* Local constant definitions                                                 */
/******************************************************************************/
PRIVATE const PureNANDSpec gastPNDSpec[] = {

	//nMID, nDID,nNumOfBlks,nNumOfDies,nNumOfPlanes,nSctsPerPg,nSparePerSct,nPgsPerBlk,nRsvBlksInDev,nBWidth,      n5CycleDev,nSLCTLoadTime,nSLCTProgTime,nTEraseTime,
	/* KF92G16Q2X */ /* KF92G16Q2W */
	{ 0xEC, 0xBA,      2048,         1,           1,         4,          16,        64,           40,     16, FSR_PND_5CYCLES,           40,          250,      2000},
	/* KF94G16Q2W */
	{ 0xEC, 0xBC,      4096,         1,           1,         4,          16,        64,           80,     16, FSR_PND_5CYCLES,           40,          250,      2000},
	/* MT29F2G08ABAEAWP */
	{ 0x2C, 0xDA,      2048,         1,           1,         4,          16,        64,           40,      8, FSR_PND_5CYCLES,           25,          600,      3000},

	/* MT29F2G08ABAEAWP */
	{ 0x2C, 0xDC,      4096,         1,           1,         4,          16,        64,           80,      8, FSR_PND_5CYCLES,           25,          600,      3000},

	/* K9F2G08UXA */		
	{ 0xEC, 0xDA,      2048,         1,           1,         4,          16,        64,           40,      8, FSR_PND_5CYCLES,           40,          250,      2000},

	/* K9F1G08U0D */		
	{ 0xEC, 0xF1,      1024,         1,           1,         4,          16,        64,           20,      8, FSR_PND_4CYCLES,           40,          250,      2000},

	/* ESMT : F59L1G81A */		
	{ 0x92, 0xF1,      1024,         1,           1,         4,          16,        64,           20,      8, FSR_PND_4CYCLES,           40,          250,      2000},

	/* MT29F2GxxAxxEAxx NOTE FPGA test NADN FLASH*/
	{ 0x2C, 0xDA,      2048,         1,           2,         4,          16,        64,           40,     16, FSR_PND_5CYCLES,           40,          250,      2000},
	{ 0x00, 0x00,         0,         0,           0,         0,           0,         0,            0,      0,               0,            0,            0,         0},
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


#ifdef FSR_DEBUG
PUBLIC  INT32	_SetNANDCtrllerCmd  (UINT32         nDev,
#else
PRIVATE INT32	_SetNANDCtrllerCmd  (UINT32         nDev,
#endif
			UINT32         nDie,
			UINT32         nRowAddress,
			UINT32         nColAddress,
			UINT32         nCommand);



LLD_FUNC INT32 NTK_NFCON_ReadPage(UINT32 nDev, UINT32 nRowAddress, UINT32 nColAddress);
LLD_FUNC INT32 NTK_NFCON_ProgramPage(UINT32 nDev, UINT32 nRowAddress, UINT32 nColAddress);
LLD_FUNC INT32 NTK_NFCON_ERASE(UINT32 nDev, UINT32 nRowAddress, UINT32 nColAddress);


//===========================================================
// hal functions
//===========================================================

#ifdef NFC_USE_INTERRUPT
UINT32 NFC_IRQInit(UINT32 INT_FLAG);
struct semaphore NFC_IRQ;
#endif

LLD_FUNC UINT32 NFC_WaitSignal(UINT32 NFC_signalType);
LLD_FUNC UINT32 NFC_Init(void);
LLD_FUNC UINT32 NFC_NAND_REIN(void);
LLD_FUNC UINT32 NFC_ReadID(NANDIDData    *pstNANDID);
LLD_FUNC UINT32 NFC_SetType(PureNANDSpec  *pstPNDSpec);
LLD_FUNC UINT32 NFC_ReadPage_FreeRun( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf);
LLD_FUNC UINT32 NFC_WritePage_FreeRun( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr, UINT8 *dwBuffer, UINT8 *dwExtraBuffer);
LLD_FUNC UINT32 NFC_ReadPage(PureNANDSpec *pstPNDSpec, UINT32 dwPageAddr, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf);
LLD_FUNC UINT32 NFC_EraseBlock(PureNANDSpec *pstPNDSpec, UINT32 dwPageAddr);
LLD_FUNC UINT32 NFC_WritePage(PureNANDSpec *pstPNDSpec, UINT32 dwPageAddr, UINT8 *pu8_DataBuf, UINT8 *pu8_SpareBuf);
LLD_FUNC UINT32 NFC_Reset(void);
LLD_FUNC UINT8 NFC_ReadStatus(void);
void delay_us(unsigned long count);

#ifdef RDUMP
UINT32 NFC_debug = 0;
//TODO implement mode level
UINT32 setNFCHalDebug(UINT32 debug)
{
	NFC_debug = debug;
}
#endif

//===========================================================
//		Debug Function
//===========================================================
LLD_FUNC void hex_dump(UINT8 *pu8Data, UINT32 u32Length);
LLD_FUNC int printf(const char *format, ...);

//TODO remove this
UINT32 NAND_Page_Size = 2 * 1024 ;
UINT32 NAND_SubPage_Size = 512 ;
//TODO check the ChipEnable
UINT8  dwCE = 0;

void delay_us(unsigned long count)
{
#if defined(__KERNEL__) && !defined(__U_BOOT__)
	udelay(1);
#else
	unsigned long i;

	for (i = 0; i < (count * (1>>6)); i++) // make sure CPU_SPEED_MHZ > 64
	{
		// 64 "nop"s for CPU
		__asm__ __volatile__ ("nop; nop; nop; nop; nop; nop;");
	}
#endif
}
//NOTE no need porting this right now
//UINT32 NFC_Random_DataOutput (PureNANDSpec *pstPNDSpec, UINT32 dwSubPageAddr, UINT8 *dwBuffer, UINT8 *dwExtraBuffer, UINT32 *ECN);
//UINT32 NFC_Random_DataInput(PureNANDSpec *pstPNDSpec, UINT32 dwSubPageAddr, UINT8 *dwBuffer, UINT8 *dwExtraBuffer);
//TODO porting this to speed up next time.
//INT32 NFC_ReadPage_FR(PureNANDSpec *pstPNDSpec, UINT32 dwPageAddr, UINT8 *dwBuffer, UINT8 *dwExtraBuffer);
//UINT32 NFC_ReadPage_DMA(PureNANDSpec *pstPNDSpec, UINT32 dwPageAddr, UINT8 *dwBuffer, UINT8 *dwExtraBuffer);


#if defined (FSR_LLD_LOGGING_HISTORY)
PRIVATE VOID    _AddLog             (UINT32         nDev,
		UINT32         nDie);
#endif /* #if defined (FSR_LLD_LOGGING_HISTORY) */

VOID    _DumpCmdLog         (VOID);

#ifdef FSR_DEBUG
PUBLIC  VOID    _DumpSpareBuffer    (UINT32         nDev);
#else
PRIVATE VOID    _DumpSpareBuffer    (UINT32         nDev);
#endif

PRIVATE VOID    _DumpMainBuffer     (UINT32         nDev);

#if defined (FSR_LLD_STATISTICS)
PRIVATE VOID    _AddPNDStat         (UINT32       nDev,
		UINT32       nDie,
		UINT32       nType,
		UINT32       nBytes,
		UINT32       nCmdOption);
#endif /* #if defined (FSR_LLD_STATISTICS) */

#ifdef FSR_DEBUG
UINT8            *MainRAM;
UINT8            *SpareRAM;
UINT32		 nRow;
#endif

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

	pstDest->pstSpareBufBase->nSTLMetaBase0  = (*pSrc16++ << 16);	
	pSrc16 += FSR_PND_SPARE_ECC_AREA;	
	pstDest->pstSpareBufBase->nSTLMetaBase0 |= *pSrc16++;

	pstDest->pstSpareBufBase->nSTLMetaBase1  = (*pSrc16++ << 16);
	pstDest->pstSpareBufBase->nSTLMetaBase1 |= *pSrc16++;

	pSrc16 += FSR_PND_SPARE_ECC_AREA;

	pstDest->pstSpareBufBase->nSTLMetaBase2  = (*pSrc16++ << 16);
	pstDest->pstSpareBufBase->nSTLMetaBase2 |= *pSrc16++;

	if(pstDest->nNumOfMetaExt > 0)
	{
		FSR_ASSERT(pstDest->nNumOfMetaExt == 1);

		/* Initialize meta extention #0 */
		FSR_OAM_MEMSET(&pstDest->pstSTLMetaExt[0], 0xFF, sizeof(FSRSpareBufExt));

		pstDest->pstSTLMetaExt->nSTLMetaExt0     = (*pSrc16++ << 16);
		pSrc16 += FSR_PND_SPARE_ECC_AREA;	    		
		pstDest->pstSTLMetaExt->nSTLMetaExt0    |= *pSrc16++;

		pstDest->pstSTLMetaExt->nSTLMetaExt1     = (*pSrc16++ << 16);		
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

	FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
			(TEXT("[PND:IN ] ++%s(pDest: 0x%08x, pstSrc: 0x%08x)\r\n"),
			 __FSR_FUNC__, pDest8, pstSrc));        

	FSR_OAM_MEMSET((VOID *) pDest8, 0xFF, pstPNDSpec->nSctsPerPg * FSR_SPARE_SIZE);

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

		nInitFlg = TRUE32;

	} while (0);

	NFC_Init();

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
	NANDIDData         stIDData;
	INT32              nLLDRet      = FSR_LLD_SUCCESS;    
	UINT32             nShift;
	UINT32             nCnt         = 0;
	UINT8              nMID; /* Manufacture ID */
	UINT8              nDID; /* Device ID      */

	FSR_STACK_VAR;

	FSR_STACK_END;

	FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG, (TEXT("[PND:IN ] ++%s(nDev:%d)\r\n"),
				__FSR_FUNC__, nDev));

	do
	{
		pstPNDCxt = gpstPNDCxt[nDev];

		NFC_ReadID(&stIDData);

		nMID = stIDData.nMID;
		nDID = stIDData.nDID;

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
				NFC_SetType(pstPNDCxt->pstPNDSpec);
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

		pstPNDCxt->nSftPgsPerBlk = nShift;
		pstPNDCxt->nMskPgsPerBlk = pstPNDSpec->nPgsPerBlk -1;

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

		pstPNDCxt->nDDPSelBaseBit = nShift;
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
		nLLDFlag = ~FSR_LLD_FLAG_CMDIDX_MASK & nFlag;

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
				FSR_OAM_MEMCPY(pMBufOrg + nStartOffset * FSR_SECTOR_SIZE,
						pMainDataRAM + nStartOffset * FSR_SECTOR_SIZE,
						(pstPNDSpec->nSctsPerPg - nStartOffset - nEndOffset) * FSR_SECTOR_SIZE);

#if defined (FSR_LLD_STATISTICS)
				nBytesTransferred +=  FSR_SECTOR_SIZE *
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
			pstPNDShMem->ECCnRE =  _SetNANDCtrllerCmd(nDev, nDie, nRowAddress, nColAddress, FSR_PND_SET_READ_PAGE);

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

		//TODO check this
		nFlushOpCaller = FSR_PND_PREOP_PROGRAM << FSR_PND_FLUSHOP_CALLER_BASEBIT;

		nLLDRet = FSR_PND_FlushOp(nDev, nFlushOpCaller | nDie, nFlag);
		if (nLLDRet != FSR_LLD_SUCCESS)
		{
			break;
		}

		nRowAddress  = nPbn << pstPNDCxt->nSftPgsPerBlk;
		nRowAddress |= (nPgOffset & pstPNDCxt->nMskPgsPerBlk);

		nMainSize = pstPNDSpec->nSctsPerPg * FSR_SECTOR_SIZE;

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
				FSR_OAM_MEMSET(pSpareDataRAM, 0xFF, pstPNDSpec->nSctsPerPg * FSR_SPARE_SIZE);
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

		pstPNDShMem->ECCnRE = _SetNANDCtrllerCmd(nDev, nDie, nRowAddress, nColAddress, FSR_PND_SET_PROGRAM_PAGE);

		pstPNDShMem->nPreOp[nDie]         = FSR_PND_PREOP_PROGRAM;
		pstPNDShMem->nPreOpPbn[nDie]      = (UINT16) nPbn;
		pstPNDShMem->nPreOpPgOffset[nDie] = (UINT16) nPgOffset;
		pstPNDShMem->nPreOpFlag[nDie]     = nFlag;

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

		pstPNDShMem->ECCnRE = _SetNANDCtrllerCmd(nDev, nDie, nRowAddress, INVALID_ADDRESS, FSR_PND_SET_ERASE);

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
			pstPNDShMem->ECCnRE = _SetNANDCtrllerCmd(nDev, nDie, nRowAddress, 0, FSR_PND_SET_READ_PAGE);

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
					nRndOffset = pstRIArg->nOffset - FSR_LLD_CPBK_SPARE;

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
			pstPNDShMem->ECCnRE =  _SetNANDCtrllerCmd(nDev, nDie, nRowAddress, 0, FSR_PND_SET_PROGRAM_PAGE);

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
	INT32             nLLDRet;
	UINT16            nDQ;
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

		nDQ = *((UINT16 *)pstPNDShMem->pPseudoSpareDataRAM);

		if (nDQ != FSR_PND_VALID_BLK_MARK)
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

				nLLDRet = pstPNDShMem->ECCnRE;

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

#if !defined(FSR_OAM_RTLMSG_DISABLE)
						FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
								(TEXT("            at nDev:%d, nPbn:%d, nPgOffset:%d, nFlag:0x%08x\r\n"),
								 nDev, nPrevPbn, nPrevPgOffset, nPrevFlag));
#endif

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

#if !defined(FSR_OAM_RTLMSG_DISABLE)
						FSR_DBZ_RTLMOUT(FSR_DBZ_LLD_INF | FSR_DBZ_ERROR,
								(TEXT("            2Lv read disturbance at nPbn:%d, nPgOffset:%d, nFlag:0x%08x\r\n"),
								 nPrevPbn, nPrevPgOffset, nPrevFlag));
#endif

						_DumpSpareBuffer(nDev);
					}          
				break;

			case FSR_PND_PREOP_CACHE_PGM: 
			case FSR_PND_PREOP_PROGRAM:
			case FSR_PND_PREOP_CPBK_PGM:

				nLLDRet = pstPNDShMem->ECCnRE;

				if (nLLDRet != FSR_LLD_SUCCESS)
				{
					FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
							(TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
							 __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

					FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
							(TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
							 pstPNDCxt->nFlushOpCaller));

#if !defined(FSR_OAM_RTLMSG_DISABLE)
					FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
							(TEXT("            last write() call @ nDev = %d, nPbn = %d, nPgOffset = %d, nFlag:0x%08x\r\n"),
							 nDev, nPrevPbn, nPrevPgOffset, nPrevFlag));
#endif
				}
				break;

			case FSR_PND_PREOP_ERASE:        

				/* Previous error check */
				nLLDRet = pstPNDShMem->ECCnRE;

				if (nLLDRet != FSR_LLD_SUCCESS)
				{
					FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
							(TEXT("[PND:ERR]   %s(nDev:%d, nDie:%d, nFlag:0x%08x) / %d line\r\n"),
							 __FSR_FUNC__, nDev, nDie, nFlag, __LINE__));

					FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
							(TEXT("            pstPNDCxt->nFlushOpCaller : %d\r\n"),
							 pstPNDCxt->nFlushOpCaller));

#if !defined(FSR_OAM_RTLMSG_DISABLE)
					FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
							(TEXT("            last Erase() call @ nDev = %d, nPbn = %d, nFlag:0x%08x\r\n"),
							 nDev, nPrevPbn, nPrevFlag));
#endif
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

		pstDevSpec->nSLCPECycle         = 50000;
		pstDevSpec->nMLCPECycle         = 0;

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
				pstPNDSpec->nSctsPerPg * FSR_SECTOR_SIZE);

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
#ifdef FSR_DEBUG
PUBLIC  INT32	
#else
PRIVATE  INT32
#endif
_SetNANDCtrllerCmd (UINT32 nDev,
		UINT32 nDie,
		UINT32 nRowAddress,
		UINT32 nColAddress,
		UINT32 nCommand)
{
	UINT32	nCS;
	UINT32  nCycle;
	INT32   nRe         = FSR_LLD_SUCCESS;    

	nCS = nDev;

	nCycle = gpstPNDCxt[nDev]->pstPNDSpec->n5CycleDev;
#ifdef FSR_DEBUG
	PureNANDShMem    *pstPNDShMem;	
	pstPNDShMem = gpstPNDShMem[nDev];
	UINT8            *pMainDataRAM;
	UINT8            *pSpareDataRAM;

	MainRAM    = pstPNDShMem->pPseudoMainDataRAM;
	SpareRAM   = pstPNDShMem->pPseudoSpareDataRAM;
	nRow = nRowAddress;
#endif

	switch (nCommand)
	{
		case FSR_PND_SET_READ_PAGE:
			nRe = NTK_NFCON_ReadPage(nDev,nRowAddress, nColAddress);
			break;

		case FSR_PND_SET_PROGRAM_PAGE:
			nRe = NTK_NFCON_ProgramPage(nDev, nRowAddress, nColAddress);
			break;

		case FSR_PND_SET_ERASE:
			//FSR_OAM_DbgMsg("++NVT_NFCON_ErasePage Row:%d Block:%d\r\n",nRowAddress, nRowAddress >> 6);
			nRe = NTK_NFCON_ERASE(nDev, nRowAddress, nColAddress);

			break;

		case FSR_PND_SET_RANDOM_DATA_INPUT:
			break;

		case FSR_PND_SET_RANDOM_DATA_OUTPUT:
			break;

		case FSR_PND_SET_READ_PGM_STATUS:
		case FSR_PND_SET_READ_ERASE_STATUS:
		case FSR_PND_SET_READ_INTERRUPT_STATUS:
			break;

		case FSR_PND_SET_ECC_ON:
			gEnableECC = 1;
			break;
		case FSR_PND_SET_ECC_OFF:
			gEnableECC = 0;
			break;

		default:
			break;
	}

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
 * @brief           This function prints error context of TimeOut error
 * @n               when using Tiny FSR and FSR at the same time
 *
 * @param[in]       none
 *
 * @return          none
 *
 * @author          NamOh Hwang
 * @version         1.0.0
 * @remark
 *
 */
	VOID
_DumpCmdLog(VOID)
{

#if !defined(FSR_OAM_RTLMSG_DISABLE)
	PureNANDCxt        *pstPNDCxt       = NULL;
	PureNANDShMem      *pstPNDShMem     = NULL;
	UINT32              nDev;

#if defined(FSR_LLD_LOGGING_HISTORY)
	volatile    PureNANDOpLog      *pstPNDOpLog     = NULL;
	UINT32              nIdx;
	UINT32              nLogHead;
	UINT32              nPreOpIdx;
#endif

	FSR_STACK_VAR;

	FSR_STACK_END;

	FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG | FSR_DBZ_ERROR,
			(TEXT("[PND:IN ] ++%s()\r\n\r\n"), __FSR_FUNC__));

	for (nDev = 0; nDev < FSR_PND_MAX_DEVS; nDev++)
	{
		pstPNDCxt  = gpstPNDCxt[nDev];

		if (pstPNDCxt == NULL)
		{
			continue;
		}

		if (pstPNDCxt->bOpen == FALSE32)
		{
			continue;
		}

		pstPNDShMem = gpstPNDShMem[nDev];

		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR | FSR_DBZ_LLD_INF,
				(TEXT("[PND:INF]   pstPNDCxt->nFlushOpCaller : %d\r\n"),
				 pstPNDCxt->nFlushOpCaller));

#if defined (FSR_LLD_LOGGING_HISTORY)
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("[PND:INF]   start printing nLog      : nDev[%d]\r\n"), nDev));

		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("%5s  %7s  %10s  %10s  %10s\r\n"), TEXT("nLog"),
				 TEXT("preOp"), TEXT("prePbn"), TEXT("prePg"), TEXT("preFlag")));

		pstPNDOpLog = &gstPNDOpLog[nDev];

		nLogHead = pstPNDOpLog->nLogHead;
		for (nIdx = 0; nIdx < FSR_PND_MAX_LOG; nIdx++)
		{
			nPreOpIdx = pstPNDOpLog->nLogOp[nLogHead];
			FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
					(TEXT("%5d| %7s, 0x%08x, 0x%08x, 0x%08x\r\n"),
					 nLogHead, gpszLogPreOp[nPreOpIdx], pstPNDOpLog->nLogPbn[nLogHead],
					 pstPNDOpLog->nLogPgOffset[nLogHead], pstPNDOpLog->nLogFlag[nLogHead]));

			nLogHead = (nLogHead + 1) & (FSR_PND_MAX_LOG -1);
		}

		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("[PND:   ]   end   printing nLog      : nDev[%d]\r\n"), nDev));
#endif /* #if defined (FSR_LLD_LOGGING_HISTORY) */

		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("\r\n[PND:INF]   start printing PureNANDCxt: nDev[%d]\r\n"), nDev));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("            nPreOp           : [0x%08x, 0x%08x]\r\n"),
				 pstPNDShMem->nPreOp[0], pstPNDShMem->nPreOp[1]));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("            nPreOpPbn        : [0x%08x, 0x%08x]\r\n"),
				 pstPNDShMem->nPreOpPbn[0], pstPNDShMem->nPreOpPbn[1]));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("            nPreOpPgOffset   : [0x%08x, 0x%08x]\r\n"),
				 pstPNDShMem->nPreOpPgOffset[0], pstPNDShMem->nPreOpPgOffset[1]));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("            nPreOpFlag       : [0x%08x, 0x%08x]\r\n"),
				 pstPNDShMem->nPreOpFlag[0], pstPNDShMem->nPreOpFlag[1]));
		FSR_DBZ_RTLMOUT(FSR_DBZ_ERROR,
				(TEXT("            end   printing PureNANDCxt: nDev[%d]\r\n\r\n"), nDev));
	}

	FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG | FSR_DBZ_ERROR, (TEXT("[PND:OUT] --%s()\r\n"), __FSR_FUNC__));
#endif /* #if !defined(FSR_OAM_RTLMSG_DISABLE) */
}

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
#ifdef FSR_DEBUG
PUBLIC  VOID
#else
PRIVATE VOID
#endif
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
	nPageSize        = pstPNDCxt->pstPNDSpec->nSctsPerPg * FSR_SECTOR_SIZE;    

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

#if !defined(WITH_TINY_FSR)
//===========================================================
// hal functions
//===========================================================
/**
 * @brief           This function is used for NTK NAND Controller
 *                  this function read data from NAND flash
 * @return          FSR_PAM_SUCCESS
 * @author          SeungJun Heo
 * @version         0.0.1
 *
 */
INT32   NTK_NFCON_ReadPage(UINT32 nDev, UINT32 nRowAddress, UINT32 nColAddress)
{


	UINT8         *pMBuf;
	UINT8         *pSBuf;
	UINT32	        nCnt=0;
	//INT32          nNumOfTransfer;
	INT32          nLLDRe      = FSR_LLD_SUCCESS;             
	PureNANDCxt   *pstPNDCxt;
	PureNANDSpec  *pstPNDSpec;
	PureNANDShMem *pstPNDShMem;

	unsigned long testPattern=0xdeadbeef;

	FSR_STACK_VAR;

	FSR_STACK_END;

	pstPNDCxt   = gpstPNDCxt[nDev];
	pstPNDSpec  = pstPNDCxt->pstPNDSpec;
	pstPNDShMem = gpstPNDShMem[nDev];

	pMBuf = pstPNDShMem->pPseudoMainDataRAM;
	pSBuf = pstPNDShMem->pPseudoSpareDataRAM;


retry_read:
	*(volatile unsigned long*)(pMBuf+MAIN_DATA_SIZE-4) = testPattern;
	CACHE_WB(pMBuf+MAIN_DATA_SIZE-4, 4); /* flush testPattern to DRAM */

	//    FSR_OAM_DbgMsg("+ReadPage R:%d\r\n",nRowAddress);
	//NOTE no use first block first, look seem first block have different ecc.
	//Check First Page

	//SDP_NFCON_ChipAssert(nDev);

	//CACHE CONTROL
    CACHE_INV(pMBuf,MAIN_DATA_SIZE);
    CACHE_INV(pSBuf,SPARE_DATA_SIZE);

#ifdef SUPPORT_FREE_RUN
	nLLDRe = NFC_ReadPage_FreeRun(pstPNDSpec,nRowAddress,pMBuf,pSBuf);
#else
	nLLDRe = NFC_ReadPage(pstPNDSpec,nRowAddress,pMBuf, pSBuf);
#endif
	if( nLLDRe == FSR_LLD_PREV_READ_ERROR  )
	{
		FSR_OAM_DbgMsg("\n[NFC:ERR]Read Page Addr 0x%08x Fail",nRowAddress);
		return nLLDRe;
	}

	//external_sync();

	if(unlikely(*(volatile unsigned long*)(pMBuf+MAIN_DATA_SIZE-4) == testPattern))
	{
		msleep(1);
    	CACHE_INV(pMBuf+MAIN_DATA_SIZE-4, 4);
		if(unlikely(*(volatile unsigned long*)(pMBuf+MAIN_DATA_SIZE-4) == testPattern))
		{
			nCnt++;
			testPattern = testPattern^0x55555555;
			if(nCnt < 3)
			{
				printk( KERN_ALERT "[SA-BSP] ReadPage is currupted[%d] \n", nCnt);
				goto retry_read; /* If deadceef is stayed, retry ReadPage(Max, 3 times) */
			}
		}
	}

#ifdef RDUMP
	{
		INT32          nTESTRe      = FSR_LLD_SUCCESS;             
		FSR_OAM_DbgMsg("&");
		static UINT8 pP[MAIN_DATA_SIZE] __attribute__ ((aligned (64)));
		static UINT8 pS[SPARE_DATA_SIZE] __attribute__ ((aligned (64)));   
		CACHE_INV(pP,MAIN_DATA_SIZE);
		CACHE_INV(pS,SPARE_DATA_SIZE);
		nTESTRe = NFC_ReadPage_FreeRun(pstPNDSpec,nRowAddress,pP,pS);
		if(	(memcmp(pP,pMBuf,MAIN_DATA_SIZE) !=0 ) ||
			(memcmp(pS   ,pSBuf   ,SPARE_DATA_SIZE) !=0    )
			)
		{
			FSR_OAM_DbgMsg("\n\x1b[42mVERIFY FAIL Page Addr 0x%08x\x1b[0m",nRowAddress);
			FSR_OAM_DbgMsg("\npMBuf");
			hex_dump(pMBuf,MAIN_DATA_SIZE);
			FSR_OAM_DbgMsg("\npSBuf");
			hex_dump(pSBuf,SPARE_DATA_SIZE);
			FSR_OAM_DbgMsg("\npP");
			hex_dump(pP   ,MAIN_DATA_SIZE);
			FSR_OAM_DbgMsg("\npS");
			hex_dump(pS   ,SPARE_DATA_SIZE);
		}
	}
#endif

	return nLLDRe;
}
EXPORT_SYMBOL(NTK_NFCON_ReadPage);

/**
 * @brief           This function is used for NTK NAND Controller
 *                  this function program data from NAND flash
 * @return          FSR_PAM_SUCCESS
 * @author          SeungJun Heo
 * @version         0.0.1
 *
 */
INT32   NTK_NFCON_ProgramPage(UINT32 nDev, UINT32 nRowAddress, UINT32 nColAddress)
{
	UINT8         *pMBuf;
	UINT8         *pSBuf;
	//UINT32	        nCnt;
	//UINT32         nCS;
	//UINT8          nCycle;
	//INT32          nNumOfTransfer;
	INT32          nLLDRe      = FSR_LLD_SUCCESS;             
	PureNANDCxt   *pstPNDCxt;
	PureNANDSpec  *pstPNDSpec;
	PureNANDShMem *pstPNDShMem;

	//unsigned char szEccBuf[64];
	//int i;
	//UINT8          u8Buf;


	FSR_STACK_VAR;

	FSR_STACK_END;

	//nCS         = nDev;
	pstPNDCxt   = gpstPNDCxt[nDev];
	pstPNDSpec  = pstPNDCxt->pstPNDSpec;
	pstPNDShMem = gpstPNDShMem[nDev];

	pMBuf = pstPNDShMem->pPseudoMainDataRAM;
	pSBuf = pstPNDShMem->pPseudoSpareDataRAM;
	//nCycle = gpstPNDCxt[nDev]->pstPNDSpec->n5CycleDev;

	//	FSR_OAM_DbgMsg("+ProgramPage R:%d\r\n",nRowAddress);
	
	//CACHE CONTROL
	CACHE_WB(pMBuf,MAIN_DATA_SIZE );
	CACHE_WB(pSBuf,SPARE_DATA_SIZE);

#ifdef SUPPORT_FREE_RUN
	if(gEnableECC==1)
	{
		nLLDRe = NFC_WritePage_FreeRun(pstPNDSpec,nRowAddress,pMBuf,pSBuf);
	}
	else
#endif
	{
		nLLDRe = NFC_WritePage(pstPNDSpec,nRowAddress,pMBuf, pSBuf);
	}

#ifdef RDUMP
	{
		INT32          nTESTRe      = FSR_LLD_SUCCESS;             
		FSR_OAM_DbgMsg("$");
		static UINT8 pP[MAIN_DATA_SIZE] __attribute__ ((aligned (64)));
		static UINT8 pS[SPARE_DATA_SIZE] __attribute__ ((aligned (64)));   
		CACHE_INV(pP,MAIN_DATA_SIZE);
		CACHE_INV(pS,SPARE_DATA_SIZE);
		nTESTRe = NFC_ReadPage_FreeRun(pstPNDSpec,nRowAddress,pP,pS);
		if(	(memcmp(pP,pMBuf,MAIN_DATA_SIZE) !=0 ) ||
			(memcmp(pS   ,pSBuf   ,6) !=0    ) ||
			(memcmp(pS+16,pSBuf+16,6) !=0    ) ||
			(memcmp(pS+32,pSBuf+32,6) !=0    ) ||
			(memcmp(pS+48,pSBuf+48,6) !=0    )
			)
		{
			FSR_OAM_DbgMsg("\n\x1b[42mVERIFY FAIL Page Addr 0x%08x\x1b[0m",nRowAddress);
			FSR_OAM_DbgMsg("\npMBuf");
			hex_dump(pMBuf,MAIN_DATA_SIZE);
			FSR_OAM_DbgMsg("\npSBuf");
			hex_dump(pSBuf,SPARE_DATA_SIZE);
			FSR_OAM_DbgMsg("\npP");
			hex_dump(pP   ,MAIN_DATA_SIZE);
			FSR_OAM_DbgMsg("\npS");
			hex_dump(pS   ,SPARE_DATA_SIZE);
		}
	}
#endif 
	return nLLDRe;
}
EXPORT_SYMBOL(NTK_NFCON_ProgramPage);

INT32 NTK_NFCON_ERASE(UINT32 nDev, UINT32 nRowAddress, UINT32 nColAddress)
{
	INT32          nLLDRe      = FSR_LLD_SUCCESS;             
	PureNANDCxt   *pstPNDCxt;
	PureNANDSpec  *pstPNDSpec;


	FSR_STACK_VAR;

	FSR_STACK_END;

	pstPNDCxt   = gpstPNDCxt[nDev];
	pstPNDSpec  = pstPNDCxt->pstPNDSpec;

	nLLDRe = NFC_EraseBlock( pstPNDSpec,nRowAddress);

	return nLLDRe;
}
EXPORT_SYMBOL(NTK_NFCON_ERASE);
//TODO fix this timing
UINT32 NFC_Init( void )
{
	//UINT8 tWH, tWP, tRH, tRP, tWW, tAR, tCLS_tCS_tALS_tALH;
	//UINT8 bonding_sel, hclk_sel;
	//UINT32 hclk , bootval;
	UINT32 temp;
#ifdef RDUMP
#ifdef SUPPORT_FREE_RUN	
	FSR_OAM_DbgMsg("\n[NFC is FREE RUN Mode]");
#else
	FSR_OAM_DbgMsg("\n[NFC is DMA Mode]");
#endif
#endif

	//FSR_OAM_DbgMsg("\ngpstPNDShMem addr 0x%08x",gpstPNDShMem);
	w32(REG_NFC_SYSCTRL,0);

#ifdef NAND_16Bit    
	temp = ntk_readl(REG_NFC_SYSCTRL) & ( 1 << 31 );
#else
	//set up 8 bit nand
	temp = ntk_readl(REG_NFC_SYSCTRL) & ~( 1 << 31 );
#endif

	//setup nand active
	temp = temp & ~( 1 << 30 );
	//Ken, 2011.02.21, NfcRegP_InitDone
	temp = temp & ~( 1 << 5 );

	w32(REG_NFC_SYSCTRL,temp);

	// software reset host
	w32(REG_NFC_SW_RESET , 0x1F);
	while(ntk_readl(REG_NFC_SW_RESET) != 0);
	// clear interrupt status
	w32(REG_NFC_INT_ENABLE, 0x0);
	//REG_NFC_INT_ENABLE = 0xFFFFFFFF;
	w32( REG_NFC_INT_STAT,-1);
	w32( REG_NFC_SYSCTRL1,0x01);
	w32( REG_NFC_SYSCTRL2,r32(REG_NFC_SYSCTRL2)|16);

	//TODO check this
	BIT_SET(REG_NFC_SYSCTRL, NFC_SYS_CTRL_EXTRA_SIZE(16) & ~(1<<26) & ~(1<<27));

	switch (NAND_Page_Size) {
		case 2048:    //K9F2G08U0B, SLC, 256 MB, 1 dies
			NAND_Page_Size = 2048;
			BIT_SET(REG_NFC_SYSCTRL, NFC_SYS_CTRL_PAGE_2048 | NFC_SYS_CTRL_BLK_128K);
			break;
		case 4096:    //H27UAG8T2A
			NAND_Page_Size = 4096;
			BIT_SET(REG_NFC_SYSCTRL, NFC_SYS_CTRL_PAGE_4096 | NFC_SYS_CTRL_BLK_512K);
			break;
		case 8192:    //K9G8G08U0C, K9GAG08U0E
			NAND_Page_Size = 8192;
			BIT_SET(REG_NFC_SYSCTRL, NFC_SYS_CTRL_PAGE_8192 | NFC_SYS_CTRL_BLK_1M);
			break;
		default:
			NAND_Page_Size = 8192;
			BIT_SET(REG_NFC_SYSCTRL, NFC_SYS_CTRL_PAGE_8192 | NFC_SYS_CTRL_BLK_1M);
	}

	//TODO check CFG0 setting
	w32(REG_NFC_CFG0, 0x1010011B);

	w32(REG_NFC_CFG0, r32(REG_NFC_CFG0) & ~(1<<2));

	w32(REG_NFC_CFG1, NFC_CFG1_READY_TO_BUSY_TIMEOUT(-1) | 
			NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_LITTLE_ENDIAN | NFC_CFG1_BUSY_TO_READY_TIMEOUT(-1));

	BIT_CLEAR(REG_NFC_CFG1,(1<<17));
	BIT_CLEAR(REG_NFC_CFG1,(1<<21));

    w32(REG_NFC_Fine_Tune, 0xc0c0) ;  // 0901 modify for Timing
    w32(REG_NFC_SYSCTRL1, r32(REG_NFC_SYSCTRL1) | 1<<11 );    // 0901 modify for Read Timing
	//Dump_NFC_Init_Reg();
	S_REG_NFC_CFG0 = r32(REG_NFC_CFG0);
	S_REG_NFC_CFG0_RR = (S_REG_NFC_CFG0 & 0x00FFFF0F) | 0x72000020;
	S_REG_NFC_CFG1 = r32(REG_NFC_CFG1) ;
	S_REG_NFC_SYSCTRL = r32(REG_NFC_SYSCTRL) ;
	S_REG_NFC_SYSCTRL1 = r32(REG_NFC_SYSCTRL1);
	S_REG_NFC_SYSCTRL2 = r32(REG_NFC_SYSCTRL2);
	S_REG_NFC_Fine_Tune = r32(REG_NFC_Fine_Tune) ;

#ifdef NFC_USE_INTERRUPT
	sema_init(&NFC_IRQ,0);
	if(0 != NFC_IRQInit(0))
	{
		FSR_OAM_DbgMsg("\nInit NFC IRQ Fail");
	}
#endif

	return 0;    
}
EXPORT_SYMBOL(NFC_Init);

UINT32 NFC_NAND_REIN(void)
{

	w32(REG_NFC_SW_RESET,0x3);
	while(ntk_readl(REG_NFC_SW_RESET) & 0x03);
	w32(REG_NFC_INT_STAT,0xFFFFFFFF);		//clear status bits

	w32(REG_NFC_INT_ENABLE,NFC_INT_ENABLE);	//enable interrupt

	w32(REG_NFC_CFG0,S_REG_NFC_CFG0);
	w32(REG_NFC_CFG1,S_REG_NFC_CFG1);
	w32(REG_NFC_SYSCTRL,S_REG_NFC_SYSCTRL);
	w32(REG_NFC_SYSCTRL1,S_REG_NFC_SYSCTRL1);
	w32(REG_NFC_SYSCTRL2,S_REG_NFC_SYSCTRL2);
	w32(REG_NFC_Fine_Tune,S_REG_NFC_Fine_Tune);

	return 0;
}
EXPORT_SYMBOL(NFC_NAND_REIN);

UINT32 NFC_ReadID(NANDIDData    *pstNANDID)
{
	UINT32  device_id=0x00, device_id_ext=0x00;

	NFC_NAND_REIN();
	w32(REG_NFC_CFG0, S_REG_NFC_CFG0_RR);  // adjust timing for Freerun Read Page

	w32(REG_NFC_TRAN_MODE,0x00);
	w32(REG_NFC_TRAN_MODE,NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_BLK_SIZE(6));

	// clear interrupt status
	w32(REG_NFC_INT_STAT,0xFFFFFFFF);

	w32(REG_NFC_CMD,NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_DUMMY |
			NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x90));

#ifndef NFC_USE_INTERRUPT
	NFC_WaitSignal(NFC_INT_CMD_COMPLETE);			// wait for command complete
#endif
	NFC_WaitSignal(NFC_INT_DATA_TRAN_COMPLETE);		// wait for transfer complete

	if (!(r32(REG_NFC_INT_STAT) & NFC_INT_ERR))
	{
		device_id     = r32(REG_NFC_DATA_PORT);
		device_id_ext = r32(REG_NFC_DATA_PORT);

		pstNANDID->nMID         = device_id & 0xff;        
		pstNANDID->nDID         = (device_id >> 8) & 0xff; 
		pstNANDID->n3rdIDData   = (device_id >> 16) & 0xff;
		pstNANDID->n4thIDData   = (device_id >> 24) & 0xff;
		pstNANDID->n5thIDData   = device_id_ext & 0xff;    
		pstNANDID->nPad0        = (device_id_ext >> 8) & 0xff;

		//TODO modify return value
		return 0;
	}
	else
	{
		FSR_OAM_DbgMsg("[%s] Warning: Read ID FAILS\n", __FUNCTION__);

		//TODO modify return value
		return -1;
	}
}
EXPORT_SYMBOL(NFC_ReadID);

UINT8 NFC_ReadStatus(void)
{
	UINT32 ret;
	UINT32 temp;

	NFC_NAND_REIN();
	w32(REG_NFC_CFG0, S_REG_NFC_CFG0_RR);  // adjust timing for Freerun Read Page

	ntk_writel(ntk_readl(REG_NFC_CFG1) & (~NFC_CFG1_RS_ECC_ENABLE), REG_NFC_CFG1); 

	temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_BLK_SIZE(1); // transfer 1 byte (status)
	w32(REG_NFC_TRAN_MODE,temp);

	temp = NFC_INT_CMD_COMPLETE | NFC_INT_DATA_TRAN_COMPLETE;   // clear command complete & transfer complete status
	w32(REG_NFC_INT_STAT,temp);

	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_CYCLE_ONE | 
		NFC_CMD_CODE0(0x70);
	w32(REG_NFC_CMD,temp);

	NFC_WaitSignal(NFC_INT_ERR | NFC_INT_CMD_COMPLETE);
	NFC_WaitSignal(NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE);

	if (ntk_readl(REG_NFC_CFG1) & NFC_CFG1_LITTLE_ENDIAN){
		// little endian
		ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR) ? (ntk_readl(REG_NFC_DATA_PORT) & 0xff) : 0;
	}else{ 
		// big endian
		ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR) ? ntk_readl(REG_NFC_DATA_PORT) >> 24 : 0;
	}

	return ret;
}
EXPORT_SYMBOL(NFC_ReadStatus);

UINT32 NFC_ReadPage_FreeRun( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr,
		UINT8 *dwBuffer, UINT8 *dwExtraBuffer)
{

	UINT32  bResult = FSR_LLD_SUCCESS;
	UINT32  sNAND_Page_Size ;
	//UINT32  blank_ecc_error_cnt = 0;
	UINT32  subpage_cnt ;
	UINT32  blank_page;
	UINT8 ECN[8];
	UINT32 i;

	sNAND_Page_Size = pstPNDSpec->nSctsPerPg * FSR_SECTOR_SIZE ;

	subpage_cnt = pstPNDSpec->nSctsPerPg ;

	N_HALT_DEBUG("NFC_ReadPage Page:[0x%08x]", dwPageAddr);
	NFC_NAND_REIN();

	w32(REG_NFC_CFG0, S_REG_NFC_CFG0_RR);  // adjust timing for Freerun Read Page
	w32(REG_NFC_Fine_Tune,(r32(REG_NFC_Fine_Tune)|0xc0)) ;                      // Add for adjust read duty cycle
	w32(REG_NFC_SYSCTRL1, (r32(REG_NFC_SYSCTRL1)|0x800 ));                 // used for adjust latch time

	//reset ecc decode
	BIT_SET(REG_NFC_CFG1, (NFC_CFG1_ECC_DECODE_RESET));
	BIT_CLEAR(REG_NFC_CFG1, (NFC_CFG1_ECC_DECODE_RESET));

	// software reset host
	w32(REG_NFC_SW_RESET, 0x03);
	while(r32(REG_NFC_SW_RESET) & 0x03);

	BIT_SET(REG_NFC_SYSCTRL2, NFC_SYS_CTRL2_BLANK_CHECK_EN); // Set bit 6 : Check Blank function

	w32(REG_NFC_INT_STAT, -1);  // Clean Interrupt Status

	//enable Free run mechanism
	BIT_SET(REG_NFC_SYSCTRL1, NFC_SYS_CTRL1_FREE_RUN_EN); // Set Bit 2

	//set 1: for new free-run read: RS and BCH one page 1 time 
	BIT_SET(REG_NFC_SYSCTRL1, NFC_SYS_CTRL1_ECC_DEC_MODE); // Set Bit 10

	//set 1: disable "Remove extra 2 dummy bytes  of extra"
	BIT_SET(REG_NFC_SYSCTRL1, NFC_SYS_CTRL1_REMOVE_EXT_DATA); // Bit 6

	// Set Row Address
	w32(REG_NFC_ROW_ADDR, dwPageAddr );

	// Set Col Address to subpage0 (0)
	w32(REG_NFC_COL_ADDR, 0);


	// NFC_TRAN_MODE_XTRA_DATA_COUNT_16 : This config will not use when BCH is ready
	w32(REG_NFC_TRAN_MODE, NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO | 
			NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET | 
			NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(sNAND_Page_Size) | 
			NFC_TRAN_MODE_START_WAIT_RDY | NFC_TRAN_MODE_DATA_SEL_DMA);

	// Set random access command
	w32(REG_NFC_RAND_ACC_CMD, NFC_RAND_ACC_CMD_CODE1(0xe0) | NFC_RAND_ACC_CMD_CODE0(0x05) |
			NFC_RAND_ACC_CMD_COL_ADDR(sNAND_Page_Size));


	// Set DMA Destination
	w32(REG_NFC_DMA_ADDR, ((UINT32)dwBuffer) & 0x1fffffff );

	// Set XTRA Buffer
	w32(REG_NFC_XTRA_ADDR, ((UINT32)dwExtraBuffer) & 0x1fffffff );

	// Set DMA Control
	w32(REG_NFC_DMA_CTRL, NFC_DMA_CTRL_READ |( sNAND_Page_Size));  // Free Run mode does not need to -1

	BIT_SET(REG_NFC_CFG1, (NFC_CFG1_RS_ECC_ENABLE));    // Enable RS ECC
	if(gEnableECC==1)
	{
		BIT_CLEAR(REG_NFC_SYSCTRL2, (NFC_SYS_CTRL2_AUTO_CORRECT_DISABLE));    // Bit 7 // Enable Auto Correct
	}
	else
	{
		BIT_SET(REG_NFC_SYSCTRL2, (NFC_SYS_CTRL2_AUTO_CORRECT_DISABLE));    // Bit 7  // Disable Auto Correct
	}
	// Set command
	w32(REG_NFC_CMD, NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_WP_NEG |//NFC_CMD_FREERUN | 
			NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_END_WAIT_BUSY_TO_RDY | 
			NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | NFC_CMD_CODE0(0x00));

	// wait for system memory transfer complete 
#if 1 
	NFC_WaitSignal(NFC_INT_ERR_RW | NFC_INT_MEM_TRAN_COMPLETE);	// wait for system memory transfer complete   
#else 
	UINT32 dwTemp = 0;   
	while(!(r32(REG_NFC_INT_STAT) & (NFC_INT_ERR_RW | NFC_INT_MEM_TRAN_COMPLETE)))
	{
		printf("\nwait for transfer complete");
		dwTemp++;               
		if (dwTemp > 500)
		{
			FSR_OAM_DbgMsg("Warning : NFC_ReadPage_FR wait < NFC_INT_DATA_TRAN_COMPLETE > timeout! page=%d, i=%d\n", dwPageAddr, i);
			FSR_OAM_DbgMsg("REG_NFC_INT_STAT=%#lx\n", ntk_readl(REG_NFC_INT_STAT));

			w32(REG_NFC_CFG1, r32(REG_NFC_CFG1) & (~NFC_CFG1_RS_ECC_ENABLE));

			w32(REG_NFC_INT_STAT, 0xFFFFFFFF);

			w32(REG_NFC_SW_RESET, 0x03);
			while(r32(REG_NFC_SW_RESET) & 0x03);

			FSR_OAM_DbgMsg("NAND_REDO -- Error : CFG0 = 0x%x, !\n", ntk_readl(REG_NFC_CFG0));

			dwTemp = 0;
			return FSR_LLD_PREV_READ_ERROR;
		}
	}
#endif    

#ifdef RDUMP
	if(NFC_debug != 0)
	{
		dump_ECC_Reg();
	}
#endif
	if(gEnableECC==1)
	{
		for ( i=0; i<(subpage_cnt/2); i++)
		{
			ECN[i*2]= (ntk_readl(NFC_ERR_CNT0)>>5*i) & 0x07;
			ECN[i*2+1]= (ntk_readl(NFC_ERR_CNT1)>>5*i) & 0x07;
		}

#ifndef CONFIG_VD_RELEASE  // only debug case
		for ( i=0; i<subpage_cnt; i++ ){
			if ( ECN[i] != 0x0 )
			{
				FSR_OAM_DbgMsg("dwPageAddr:0x%x, ECN[0]:%d, ECN[1]:%d, ECN[2]:%d, ECN[3]:%d\r\n", dwPageAddr, ECN[0], ECN[1], ECN[2], ECN[3]);
				break;
			}
		}
#endif

		for ( i=0; i<subpage_cnt; i++ ){
			if ( ECN[i] >= 0x3 )
			{
#ifndef CONFIG_VD_RELEASE  // only debug case
 				FSR_OAM_DbgMsg("2LV Addr:0x%x, ECN[0]:%d, ECN[1]:%d, ECN[2]:%d, ECN[3]:%d\r\n", dwPageAddr, ECN[0], ECN[1], ECN[2], ECN[3]);
#endif
				return FSR_LLD_PREV_2LV_READ_DISTURBANCE;
			}
		}

		if ( r32(REG_NFC_INT_STAT) & NFC_INT_FR_UNCORECTABLE )
		{
			static UINT8 pP[MAIN_DATA_SIZE] __attribute__ ((aligned (64)));
			static UINT8 pS[SPARE_DATA_SIZE] __attribute__ ((aligned (64)));   

			FSR_OAM_DbgMsg("+NFC_INT_FR_UNCORECTABLE \r\n");
			FSR_OAM_DbgMsg("dwPageAddr:0x%x, ECN[0]:%d, ECN[1]:%d, ECN[2]:%d, ECN[3]:%d\r\n", dwPageAddr, ECN[0], ECN[1], ECN[2], ECN[3]);
			dump_ECC_Reg();


			gEnableECC=0;
			CACHE_INV(pP,MAIN_DATA_SIZE);
			CACHE_INV(pS,SPARE_DATA_SIZE);
			NFC_ReadPage_FreeRun(pstPNDSpec,dwPageAddr,pP,pS);

			FSR_OAM_DbgMsg("\npP");
			hex_dump(pP   ,MAIN_DATA_SIZE);
			FSR_OAM_DbgMsg("\npS");
			hex_dump(pS   ,SPARE_DATA_SIZE);

			gEnableECC=1;

			dump_stack();

#ifndef CONFIG_VD_RELEASE  // only debug case
			FSR_OAM_DbgMsg("\r\ninfinite loop... \r\n"); 
			while(1);
#endif
			return FSR_LLD_PREV_READ_ERROR;
		}
	}

	// Check blank page
	blank_page = ((r32(REG_NFC_INT_STAT))>>22) &0x01;
	if (blank_page == 0x01){
		//FSR_OAM_DbgMsg("+Blank Page \r\n");
	}

	bResult = (r32(REG_NFC_INT_STAT)) & NFC_INT_ERR;  

	if (bResult)
	{
		FSR_OAM_DbgMsg("+REG_NFC_INT_STAT Error !!\r\n");
		return FSR_LLD_PREV_READ_ERROR;
	}

#ifdef TEST_INTERRUPT_MODE
	//NFC_Interrupt_Disable();
#endif
	return FSR_LLD_SUCCESS;
}
EXPORT_SYMBOL(NFC_ReadPage_FreeRun);

UINT32 NFC_ReadPage( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr, 
		UINT8 *dwBuffer, UINT8 *dwExtraBuffer)
{
	INT32   dwSubPagesPerPage = NAND_Page_Size/NAND_SubPage_Size;
	UINT32   i;    

	N_HALT_DEBUG("NFC_ReadPage Page:[0x%08x]", dwPageAddr);
	NFC_NAND_REIN();
	w32(REG_NFC_CFG0, S_REG_NFC_CFG0_RR);  // adjust timing for Freerun Read Page

	BIT_CLEAR(REG_NFC_SYSCTRL2, (1<<10));

	for(i = 0; i < dwSubPagesPerPage; ++i)    
	{
		BIT_SET(REG_NFC_CFG1, (NFC_CFG1_ECC_DECODE_RESET));
		BIT_CLEAR(REG_NFC_CFG1, (NFC_CFG1_ECC_DECODE_RESET));

		w32(REG_NFC_SW_RESET, 0x03);
		while(r32(REG_NFC_SW_RESET) & 0x03);

		w32(REG_NFC_INT_STAT, -1);


		// Set Column Address
		w32(REG_NFC_COL_ADDR, NAND_SubPage_Size * i);
		w32(REG_NFC_TRAN_MODE, NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO | 
				NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | 
				NFC_TRAN_MODE_DATA_SEL_DMA);

		if(gEnableECC == 1)
		{
			BIT_SET(REG_NFC_CFG1,NFC_CFG1_RS_ECC_ENABLE);
			BIT_SET(REG_NFC_TRAN_MODE,NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET);
		}

		// Set random access command
		w32(REG_NFC_RAND_ACC_CMD, NFC_RAND_ACC_CMD_CODE1(0xe0) | NFC_RAND_ACC_CMD_CODE0(0x05) |
				NFC_RAND_ACC_CMD_COL_ADDR( 16 * i + NAND_Page_Size));

		// Set data and oob DMA address
		w32(REG_NFC_DMA_ADDR, ((NAND_SubPage_Size * i + (UINT32)dwBuffer) & 0x1fffffff));

		// Set DMA Control
		w32(REG_NFC_DMA_CTRL, NFC_DMA_CTRL_READ |( NAND_SubPage_Size - 1));

		if(i == 0)        
		{   // SubPage 0:           0x80, col, row, data(512), 0x85, col, data(16)            
			// Set Row Address
			w32(REG_NFC_ROW_ADDR, dwPageAddr );
			// Set command
			w32(REG_NFC_CMD, NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL_ROW | 
					NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | 
					NFC_CMD_CODE0(0x00));
		}
		else
		{
			// SubPage 1, 2, ... n: 0x85, col,      data(512), 0x85, col, data(16)           
			w32(REG_NFC_CMD, NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT |
					NFC_CMD_ADDR_CYCLE_COL | NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0xe0) | NFC_CMD_CODE0(0x05));
		}

#ifndef NFC_USE_INTERRUPT
		NFC_WaitSignal(NFC_INT_ERR | NFC_INT_CMD_COMPLETE);			// wait for command complete
		NFC_WaitSignal(NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE);	// wait for transfer complete
#endif
		NFC_WaitSignal(NFC_INT_ERR | NFC_INT_MEM_TRAN_COMPLETE);	// wait for system memory transfer complete   

		if(r32(REG_NFC_INT_STAT) & NFC_INT_ERR_RW)
		{
			w32(REG_NFC_CFG1, r32(REG_NFC_CFG1) & (~NFC_CFG1_RS_ECC_ENABLE));
			BIT_CLEAR(REG_NFC_SYSCTRL2, (1<<10));
			return FSR_LLD_PREV_READ_ERROR ;
		}
		else
		{
			FSR_OAM_MEMCPY(((UINT32*)dwExtraBuffer + i * 16), (UINT32*)REG_NFC_XTRA_DATA0, 16);
		}
		BIT_CLEAR(REG_NFC_CFG1, NFC_CFG1_RS_ECC_ENABLE);
	}

	return (r32(REG_NFC_INT_STAT) & NFC_INT_ERR) ? FSR_LLD_PREV_READ_ERROR : FSR_LLD_SUCCESS;
}
EXPORT_SYMBOL(NFC_ReadPage);

UINT32 NFC_WritePage_FreeRun( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr,
		UINT8 *dwBuffer, UINT8 *dwExtraBuffer)
{
	UINT32  bResult = FSR_LLD_SUCCESS;
	UINT8 bStatus;
	UINT32  sNAND_Page_Size ;

	if(gEnableECC != 1)			//NFC_WritePage_FreeRun doesnt support ECC OFF
	{
		FSR_OAM_DbgMsg("NFC_WritePage_FreeRun doesnt support ECC OFF\n");
		return FSR_LLD_PREV_READ_ERROR;
	}

	sNAND_Page_Size = pstPNDSpec->nSctsPerPg * FSR_SECTOR_SIZE ;

	N_HALT_DEBUG("NFC_WritePage_FreeRun Page:[0x%08x]", dwPageAddr);
	NFC_NAND_REIN();

	//reset ecc decode
	BIT_SET(REG_NFC_CFG1, (NFC_CFG1_ECC_DECODE_RESET));
	BIT_CLEAR(REG_NFC_CFG1, (NFC_CFG1_ECC_DECODE_RESET));

	// software reset host
	w32(REG_NFC_SW_RESET, 0x03);
	while(r32(REG_NFC_SW_RESET) & 0x03);

	BIT_SET(REG_NFC_SYSCTRL1 ,(1<<5) | (1<<2));

	w32(REG_NFC_INT_STAT, -1);  // Clean Interrupt Status

	// Set Row Address
	w32(REG_NFC_ROW_ADDR, dwPageAddr );

	// Set Col Address to subpage0 (0)
	w32(REG_NFC_COL_ADDR, 0);

	// Set DMA Destination
	w32(REG_NFC_DMA_ADDR, ((UINT32)dwBuffer) & 0x1fffffff );

	// Set XTRA Buffer
	w32(REG_NFC_XTRA_ADDR, ((UINT32)dwExtraBuffer) & 0x1fffffff );

	w32(REG_NFC_TRAN_MODE,NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(dwCE) | 
		NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE | NFC_TRAN_MODE_XTRA_DATA_COUNT_16 |
		NFC_TRAN_MODE_BLK_SIZE(sNAND_Page_Size) | NFC_TRAN_MODE_DATA_SEL_DMA | 
		NFC_TRAN_MODE_WRITE);

	BIT_SET(REG_NFC_TRAN_MODE , (NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET));
	BIT_SET(REG_NFC_CFG1 , NFC_CFG1_RS_ECC_ENABLE);

	// Set random access command
	w32(REG_NFC_RAND_ACC_CMD, NFC_RAND_ACC_CMD_CODE0(0x85) | NFC_RAND_ACC_CMD_COL_ADDR(sNAND_Page_Size));

	// Set command, Queer.....
	w32(REG_NFC_CMD , NFC_CMD_CE_IDX(dwCE) | NFC_CMD_WP_NEG | //NFC_CMD_FREERUN |
		NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL_OTHER | 
		NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x80));

	// Set DMA Control
	w32(REG_NFC_DMA_CTRL,  sNAND_Page_Size);  // Free Run mode does not need to -1

	// wait for transfer complete
#ifndef NFC_USE_INTERRUPT
	NFC_WaitSignal(NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE);	// wait for transfer complete
#endif
	NFC_WaitSignal(NFC_INT_ERR | NFC_INT_FREE_RUN_COMPLETE);	// wait for free run transfer complete   

	bResult = r32(REG_NFC_INT_STAT) & NFC_INT_ERR;
	if(bResult)
	{
		FSR_OAM_DbgMsg("NFC_WritePage_FreeRun Fail !!\n");
		return FSR_LLD_PREV_READ_ERROR;
	}

	// check status
	bStatus = NFC_ReadStatus();
	if(bStatus & 0x40)
	{// ready
		if(bStatus & 1)
		{// fail
			FSR_OAM_DbgMsg("Status Fail !!\n");
			return FSR_LLD_PREV_READ_ERROR;
		} // end of if(bStatus & 1)
	} // end of if(bStatus & 0x40)

	return FSR_LLD_SUCCESS;
}
EXPORT_SYMBOL(NFC_WritePage_FreeRun);

UINT32 NFC_WritePage( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr, 
		UINT8 *dwBuffer, UINT8 *dwExtraBuffer)
{
	INT32   dwSubPagesPerPage = NAND_Page_Size/NAND_SubPage_Size;
	UINT32   i;    

	N_HALT_DEBUG("NFC_WritePage Page:[0x%08x]", dwPageAddr);
	NFC_NAND_REIN();

	w32(REG_NFC_SW_RESET, 0x03);
	while(r32(REG_NFC_SW_RESET) != 0);

	for(i = 0; i < dwSubPagesPerPage; ++i)    
	{
		FSR_OAM_MEMCPY((UINT32*)REG_NFC_XTRA_DATA0, dwExtraBuffer + i * 16,16);

		w32(REG_NFC_INT_STAT, -1);

		BIT_SET(REG_NFC_CFG1, (NFC_CFG1_ECC_ENCODE_RESET));
		BIT_CLEAR(REG_NFC_CFG1, (NFC_CFG1_ECC_ENCODE_RESET));

		w32(REG_NFC_DMA_ADDR, ((NAND_SubPage_Size * i + (UINT32)dwBuffer) & 0x1fffffff));

		w32(REG_NFC_DMA_CTRL, NFC_DMA_CTRL_TRAN_BYTE_COUNT(NAND_SubPage_Size));

		w32(REG_NFC_COL_ADDR, NAND_SubPage_Size * i);

		w32(REG_NFC_RAND_ACC_CMD, NFC_RAND_ACC_CMD_CODE0(0x85) | NFC_RAND_ACC_CMD_COL_ADDR(16* i + NAND_Page_Size));

		if(gEnableECC == 1)
		{
			BIT_SET(REG_NFC_CFG1,NFC_CFG1_RS_ECC_ENABLE);
			w32(REG_NFC_TRAN_MODE, NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE |
					NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | NFC_TRAN_MODE_ECC_ENABLE |
					NFC_TRAN_MODE_ECC_RESET | NFC_TRAN_MODE_DATA_SEL_DMA | NFC_TRAN_MODE_WRITE);
		}
		else
		{
			BIT_CLEAR(REG_NFC_CFG1,NFC_CFG1_RS_ECC_ENABLE);
			w32(REG_NFC_TRAN_MODE, NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE |
					NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | NFC_TRAN_MODE_DATA_SEL_DMA |
					NFC_TRAN_MODE_WRITE);
		}

		if(i == 0)        
		{   
			// SubPage 0:           0x80, col, row, data(512), 0x85, col, data(16)            
			w32(REG_NFC_ROW_ADDR, dwPageAddr);
			w32(REG_NFC_CMD, NFC_TRAN_MODE_KEEP_CE | NFC_CMD_WP_NEG | NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT |
					NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x80));

		}        
		else        
		{   
			// SubPage 1, 2, ... n: 0x85, col,      data(512), 0x85, col, data(16)           
			w32(REG_NFC_CMD, NFC_TRAN_MODE_KEEP_CE | NFC_CMD_WP_NEG | NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT |
					NFC_CMD_ADDR_CYCLE_COL | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x85));

		}        

#ifndef NFC_USE_INTERRUPT
		NFC_WaitSignal(NFC_INT_ERR | NFC_INT_CMD_COMPLETE);			// wait for command complete
		NFC_WaitSignal(NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE);	// wait for transfer complete
#endif
		NFC_WaitSignal(NFC_INT_ERR | NFC_INT_MEM_TRAN_COMPLETE);	// wait for system memory transfer complete   

		if(r32(REG_NFC_INT_STAT) & NFC_INT_ERR)        
		{         
			//TODO add msg
			return FSR_LLD_PREV_WRITE_ERROR;        
		}
	}

	w32(REG_NFC_INT_STAT, -1);

	w32(REG_NFC_CMD, NFC_CMD_CE_IDX(dwCE) | NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x10));

	NFC_WaitSignal(NFC_INT_ERR | NFC_INT_CMD_COMPLETE);			// wait for command complete

	BIT_CLEAR(REG_NFC_CFG1, NFC_CFG1_RS_ECC_ENABLE);

	if(!(r32(REG_NFC_INT_STAT) & NFC_INT_ERR)){
		// check status
		UINT8 bStatus = NFC_ReadStatus();
		if(bStatus & 0x40)
		{   // ready
			if(bStatus & 1)
			{   // fail
				//TODO add msg
				return FSR_LLD_PREV_WRITE_ERROR;
			} // end of if(bStatus & 1)
		} // end of if(bStatus & 0x40)
#ifdef RDUMP
		//TODO this
		if(0)
		{
			INT32          nLLDRe      = FSR_LLD_SUCCESS;             
			NFC_ReadPage(pstPNDSpec,dwPageAddr,bufMtmp,bufStmp);
		}
#endif
		return FSR_LLD_SUCCESS;
	}else{
		return FSR_LLD_PREV_WRITE_ERROR; 
	}
}
EXPORT_SYMBOL(NFC_WritePage);

UINT32 NFC_EraseBlock( PureNANDSpec *pstPNDSpec , UINT32 dwPageAddr)
{
	UINT8 bStatus;

	UINT32 ret;

	//FSR_OAM_DbgMsg("NFC_EraseBlock: Block=%d\n", dwPageAddr/128);

	N_HALT_DEBUG("NFC_EraseBlock Page:[0x%08x]", dwPageAddr);
	NFC_NAND_REIN();

	w32(REG_NFC_INT_STAT,0xFFFFFFFF);

	w32(REG_NFC_ROW_ADDR,dwPageAddr);
	w32(REG_NFC_COL_ADDR,0x00);

	w32(REG_NFC_CMD,NFC_CMD_CE_IDX(dwCE) | NFC_CMD_WP_NEG | NFC_CMD_ADDR_CYCLE_ROW | 
			NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0xd0) | 
			NFC_CMD_CODE0(0x60));

	// wait for command complete        
	while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR| NFC_INT_CMD_COMPLETE)));

	ret = (ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW);
	if (ret)
	{

		FSR_OAM_DbgMsg("[%s] Erase Block with dwPageAddr =%d FAILS\n" , __FUNCTION__, dwPageAddr);

		return FSR_LLD_PREV_ERASE_ERROR;
	}

	while (1)
	{
		// check status
		bStatus = NFC_ReadStatus();

		if(bStatus & 0x40){   // ready
			if(bStatus & 0x01){   // fail
				NFC_Reset();
				FSR_OAM_DbgMsg("[%s] Erase block %d FAILs, INT_STAT=%#lx, Erase Status %02x\n" , __FUNCTION__, 
						dwPageAddr/128, ntk_readl(REG_NFC_INT_STAT), bStatus);
				return FSR_LLD_PREV_ERASE_ERROR;
			} // end of if(bStatus & 1)
			else
			{
				//Erase Block OK.
				break;
			}
		} // end of if(bStatus & 0x40)
	} // while( 1 )

	return FSR_LLD_SUCCESS;
}
EXPORT_SYMBOL(NFC_EraseBlock);

UINT32 NFC_Reset(void)
{
	UINT32 ret;
	UINT32 temp;

	w32(REG_NFC_INT_STAT,0xFFFFFFFF);

	w32(REG_NFC_CMD,NFC_CMD_CE_IDX(dwCE) | NFC_CMD_END_WAIT_BUSY_TO_RDY | 
			NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0xff));

	temp = 0;

	// wait for command complete
	while( !(ntk_readl(REG_NFC_INT_STAT)&(NFC_INT_ERR | NFC_INT_CMD_COMPLETE)) )
	{
		delay_us(1);
		temp++;

		if (temp > 500000)
		{
			break;
		}
	}

	ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR);

	if (ret){
		return 0;
	}else{
		return -1;
	}
}
EXPORT_SYMBOL(NFC_Reset);

UINT32 NFC_SetType(PureNANDSpec  *pstPNDSpec)
{
	if(pstPNDSpec->n5CycleDev == FSR_PND_4CYCLES)
	{
		w32(REG_NFC_CFG0, r32(REG_NFC_CFG0) & ~(1<<1));
		S_REG_NFC_CFG0 = r32(REG_NFC_CFG0);
	}
	return 0;
}
EXPORT_SYMBOL(NFC_SetType);

UINT32 NFC_WaitSignal(UINT32 NFC_signalType)
{
#ifdef NFC_USE_INTERRUPT
	BIT_SET(REG_NFC_INT_ENABLE,NFC_signalType);
#endif
	while(!(r32(REG_NFC_INT_STAT) & (NFC_signalType)))
	{
#ifdef NFC_USE_INTERRUPT
		down(&NFC_IRQ);
#endif
	}
	return 0;
}

#ifdef NFC_USE_INTERRUPT
irqreturn_t NFC_IRQhandler(int irq, void *dev_id)
{
	//FSR_OAM_DbgMsg("\nS:0x%08x E:0x%08x",r32(REG_NFC_INT_STAT),r32(REG_NFC_INT_ENABLE));
	w32(REG_NFC_INT_ENABLE,(~(r32(REG_NFC_INT_STAT))));
	up(&NFC_IRQ);
	return IRQ_HANDLED;
}

UINT32 NFC_IRQInit(UINT32 INT_FLAG)
{
	if(gNFC_IRQInit == 0)
	{
		if(request_irq (NFC_568_INT ,NFC_IRQhandler, IRQF_DISABLED, "NFC", NULL))
		{
			FSR_OAM_DbgMsg("Warning : Error in request nvt NAND irq!\n");
			return 1;
		}
		else
		{
			FSR_DBZ_DBGMOUT(FSR_DBZ_LLD_LOG,
					(TEXT("NAND IRQ done\r\n")));
			gNFC_IRQInit = 1;
		}
	}

	//hardware setting
	BIT_SET(REG_NFC_INT_ENABLE, NFC_INT_ENABLE);

	BIT_CLEAR(REG_IRQ_MASK, NFC_568_INT_BIT);		//clear mask
	BIT_CLEAR(REG_IRQ_TYPE, NFC_568_INT_BIT);		//set to level trigger

	return 0;
}
#endif // #ifdef NFC_USE_INTERRUPT

//============================================================
//		Debug Function
//============================================================
void dump_ECC_Reg(void)
{
	FSR_OAM_DbgMsg("\n========NAND reg dump========");
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_CFG0"        ,0x00,ntk_readl(REG_NFC_CFG0));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_CFG1"        ,0x04,ntk_readl(REG_NFC_CFG1));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_CMD"         ,0x08,ntk_readl(REG_NFC_CMD));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_TRAN_MODE"   ,0x0c,ntk_readl(REG_NFC_TRAN_MODE));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_COL_ADDR"    ,0x10,ntk_readl(REG_NFC_COL_ADDR));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_ROW_ADDR"    ,0x14,ntk_readl(REG_NFC_ROW_ADDR));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_INT_STAT"    ,0x24,ntk_readl(REG_NFC_INT_STAT));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_XTRA_DATA0"  ,0x30,ntk_readl(REG_NFC_XTRA_DATA0));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_XTRA_DATA1"  ,0x34,ntk_readl(REG_NFC_XTRA_DATA1));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_XTRA_DATA2"  ,0x38,ntk_readl(REG_NFC_XTRA_DATA2));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_XTRA_DATA3"  ,0x38,ntk_readl(REG_NFC_XTRA_DATA3));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_DMA_ADDR"    ,0x50,ntk_readl(REG_NFC_DMA_ADDR));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_DMA_CTRL"    ,0x54,ntk_readl(REG_NFC_DMA_CTRL));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_SYSCTRL"     ,0x5c,ntk_readl(REG_NFC_SYSCTRL));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_SYSCTRL1"   ,0x10c,ntk_readl(REG_NFC_SYSCTRL1));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_SYSCTRL2"   ,0x11c,ntk_readl(REG_NFC_SYSCTRL2));


	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_STAT"   ,0x44,ntk_readl(REG_NFC_STAT));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_RS_ECC0"   ,0x60,ntk_readl(REG_NFC_RS_ECC0));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_RS_ECC1"   ,0x64,ntk_readl(REG_NFC_RS_ECC1));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_RS_ECC2"   ,0x68,ntk_readl(REG_NFC_RS_ECC2));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","REG_NFC_RS_ECC3"   ,0x6c,ntk_readl(REG_NFC_RS_ECC3));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","NFC_ERR_CNT0"   ,0x114,ntk_readl(NFC_ERR_CNT0));
	FSR_OAM_DbgMsg("\n%-20s REG%02x: %08lx","NFC_ERR_CNT1"   ,0x118,ntk_readl(NFC_ERR_CNT1));
}
EXPORT_SYMBOL(dump_ECC_Reg);

#define ASCII_CODE(x)	((x >= 32) && (x <= 127) ? x : '.')
void hex_dump(UINT8 *pu8Data, UINT32 u32Length)
{
	int 	nIndex;
	UINT8	u8Index;
	UINT32	u32Count = 0;

	FSR_OAM_DbgMsg("\n----------------------- Buffer Addr -> %lX---------------------------",  (UINT32)pu8Data);
	FSR_OAM_DbgMsg("\n  offset  ");
	for(u8Index = 0; u8Index < 16; u8Index++)
		FSR_OAM_DbgMsg("%02X ", u8Index);
	for(nIndex = 0; (UINT32)nIndex < u32Length; nIndex += 16)
	{
		FSR_OAM_DbgMsg("\n%09X ",  nIndex);
		for(u8Index = 0; u8Index < 16; u8Index++, u32Count++)
		{
			if((UINT32)(nIndex + u8Index) < u32Length)
				FSR_OAM_DbgMsg("%02X ", pu8Data[nIndex + u8Index]);
			else if(u32Count < u32Length)
				FSR_OAM_DbgMsg("00 ");
			else FSR_OAM_DbgMsg("   ");
		}
		for(u8Index = 0; u8Index < 16; u8Index++)
		{
			if((UINT32)(nIndex + u8Index) < u32Length)
				FSR_OAM_DbgMsg("%c", ASCII_CODE(pu8Data[nIndex + u8Index]));
			else if(u32Count < u32Length)
				FSR_OAM_DbgMsg(".");
			else FSR_OAM_DbgMsg(" ");
		}
	}
	FSR_OAM_DbgMsg("\n");
}
#endif
