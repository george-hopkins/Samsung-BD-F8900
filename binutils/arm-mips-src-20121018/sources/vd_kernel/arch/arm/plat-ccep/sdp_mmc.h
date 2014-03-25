
/*
 *	arch/arm/plat-ccep/sdp_mmc.h - Samsung DTV/BD MMC host controller driver
 *
 *	Copyright (C) 2010 Samsung Co.
 *  Created by tukho.kim@samsung.com (#0717)
 */

#ifndef __SDP_MMC_HOST_H
#define __SDP_MMC_HOST_H

#include <linux/mmc/host.h>


#define MMCH_DESC_NUM			512   		// 4096 * 32 
#define MMCH_DESC_SIZE			(sizeof(DMA_DESC_T) * MMCH_DESC_NUM)
#define NR_SG					MMCH_DESC_NUM

#define MMCH_CLKDIV_LIMIT		0xff 		// Max Divider Operation clock / ((0~255) * 2)
#define MMCH_MIN_CLOCK			400000		// 400khz	specification 0 ~ 400Khz

/* TODO : using get_clock api in next project 20100926 */
#if defined(CONFIG_ARCH_SDP1001)
#define MMCH_PROCESSOR_CLK 	50000000	 // 50MHz.
#define MMCH_MAX_CLK		 50000000
#define MMCH_CAPS_DEF		 (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_8_BIT_DATA)
#elif defined(CONFIG_ARCH_SDP1002)
#define MMCH_PROCESSOR_CLK 	50000000	 // 50MHz.
#define MMCH_MAX_CLK		 50000000
#define MMCH_CAPS_DEF		 (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_8_BIT_DATA)
#elif defined(CONFIG_ARCH_SDP1004)
#define MMCH_PROCESSOR_CLK 	(166000000 >> 2) // 166MHz  divide 4
#define MMCH_MAX_CLK 	(166000000 >> 2) // 166MHz  divide 4
#define MMCH_CAPS_DEF		 (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_8_BIT_DATA)
#elif defined(CONFIG_ARCH_SDP1103)
#define MMCH_PROCESSOR_CLK 	100000000	 // TODO: 50MHz.
#define MMCH_MAX_CLK		50000000
#define MMCH_CAPS_DEF		 (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_8_BIT_DATA)
#elif defined(CONFIG_ARCH_SDP1105)
#define MMCH_PROCESSOR_CLK 	100000000	 // TODO: 50MHz.
#define MMCH_MAX_CLK		25000000
#define MMCH_CAPS_DEF		 (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_8_BIT_DATA | \
								MMC_CAP_1_8V_DDR	)
#elif defined(CONFIG_ARCH_SDP1106)
#define MMCH_PROCESSOR_CLK 	100000000	 // TODO: 50MHz.
#define MMCH_MAX_CLK		50000000
#define MMCH_CAPS_DEF		 (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_8_BIT_DATA)
#endif

/** 
  * Controller Register definitions
  * This is the enumeration of the registers on the host controller. The
  * individual values for the members are the offsets of the individual
  * registers. The registers need to be updated according to IP release 2.10
  */
typedef enum mmch_regs {
/* MMC Host controller register */
	MMCH_CTRL			= 0x00,		/** Control */
	MMCH_PWREN   		= 0x04,		/** Power-enable */
	MMCH_CLKDIV			= 0x08,		/** Clock divider */
	MMCH_CLKSRC			= 0x0C,		/** Clock source */
	MMCH_CLKENA			= 0x10,	 	/** Clock enable */
	MMCH_TMOUT			= 0x14,	 	/** Timeout */
	MMCH_CTYPE			= 0x18,	 	/** Card type */
	MMCH_BLKSIZ			= 0x1C,	 	/** Block Size */
	MMCH_BYTCNT			= 0x20,	 	/** Byte count */
	MMCH_INTMSK			= 0x24,	 	/** Interrupt Mask */
	MMCH_CMDARG			= 0x28,	 	/** Command Argument */
	MMCH_CMD			= 0x2C,	 	/** Command */
	MMCH_RESP0			= 0x30,	 	/** Response 0 */
	MMCH_RESP1			= 0x34,	 	/** Response 1 */
	MMCH_RESP2			= 0x38,	 	/** Response 2 */
	MMCH_RESP3			= 0x3C,	 	/** Response 3 */
	MMCH_MINTSTS  		= 0x40,	 	/** Masked interrupt status */
	MMCH_RINTSTS  		= 0x44,	 	/** Raw interrupt status */
	MMCH_STATUS			= 0x48,	 	/** Status */
	MMCH_FIFOTH			= 0x4C,	 	/** FIFO threshold */
	MMCH_CDETECT		= 0x50,	 	/** Card detect */
	MMCH_WRTPRT			= 0x54,	 	/** Write protect */
	MMCH_GPIO			= 0x58,	 	/** General Purpose IO */
	MMCH_TCBCNT			= 0x5C,	 	/** Transferred CIU byte count */
	MMCH_TBBCNT			= 0x60,	 	/** Transferred host/DMA to/from byte count */
	MMCH_DEBNCE			= 0x64,	 	/** Card detect debounce */
	MMCH_USRID			= 0x68,	 	/** User ID */
	MMCH_VERID			= 0x6C,	 	/** Version ID */
	MMCH_HCON			= 0x70,	 	/** Hardware Configuration */
	MMCH_Reserved		= 0x74,	 	/** Reserved */

/* DMA register */	
	MMCH_BMOD			= 0x80,		/** Bus mode Register */
	MMCH_PLDMND			= 0x84,		/** Poll Demand */
	MMCH_DBADDR			= 0x88,		/** Descriptor Base Address */
	MMCH_IDSTS			= 0x8C,		/** Internal DMAC Status */
	MMCH_IDINTEN		= 0x90,	  	/** Internal DMAC Interrupt Enable */
	MMCH_DSCADDR		= 0x94,	  	/** Current Host Descriptor Address */
	MMCH_BUFADDR		= 0x98,		/** Current Host Buffer Address */

	MMCH_FIFODAT		= 0x100,   	/** FIFO data read write */
} Controller_Reg ;

/* MMC Host Control register definitions */
#define MMCH_CTRL_USE_INTERNAL_DMAC					(0x1<<25)
#define MMCH_CTRL_ENABLE_OD_PULLUP 					(0x1<<24)
#define MMCH_CTRL_CEATA_DEVICE_INTERRUPT_STATUS		(0x1<<11)
#define MMCH_CTRL_SEND_AUTO_STOP_CCSD				(0x1<<10)
#define MMCH_CTRL_SEND_CCSD							(0x1<<9)
#define MMCH_CTRL_ABORT_READ_DATA					(0x1<<8)
#define MMCH_CTRL_SEND_IRQ_RESPONSE					(0x1<<7)
#define MMCH_CTRL_READ_WAIT							(0x1<<6)
#define MMCH_CTRL_DMA_ENABLE						(0x1<<5)
#define MMCH_CTRL_INT_ENABLE						(0x1<<4)
#define MMCH_CTRL_DMA_RESET							(0x1<<2)
#define MMCH_CTRL_FIFO_RESET						(0x1<<1)
#define MMCH_CTRL_CONTROLLER_RESET 					(0x1<<0)

/* Clock Divider register definitions */
#define MMCH_CLKDIV_3(x)			((x)<<24)
#define MMCH_CLKDIV_2(x)			((x)<<16)
#define MMCH_CLKDIV_1(x)			((x)<<8)
#define MMCH_CLKDIV_0(x)			((x)<<0)

/* Clock Enable register definitions */
#define MMCH_CLKENA_ALL_CCLK_ENABLE			(0xffff)

/* Timeout register definitions */
#define MMCH_TMOUT_DATA_TIMEOUT(x) 		((x)<<8)
#define MMCH_TMOUT_RESPONSE_TIMEOUT(x) 	((x)<<0)

/* Interrupt mask defines */
#define MMCH_INTMSK_EBE				(0x1<<15)
#define MMCH_INTMSK_ACD				(0x1<<14)
#define MMCH_INTMSK_SBE				(0x1<<13)
#define MMCH_INTMSK_HLE				(0x1<<12)
#define MMCH_INTMSK_FRUN			(0x1<<11)
#define MMCH_INTMSK_HTO				(0x1<<10)
#define MMCH_INTMSK_DRTO			(0x1<<9)
#define MMCH_INTMSK_RTO				(0x1<<8)
#define MMCH_INTMSK_DCRC			(0x1<<7)
#define MMCH_INTMSK_RCRC			(0x1<<6)
#define MMCH_INTMSK_RXDR			(0x1<<5)
#define MMCH_INTMSK_TXDR			(0x1<<4)
#define MMCH_INTMSK_DTO				(0x1<<3)
#define MMCH_INTMSK_CMD_DONE		(0x1<<2)
#define MMCH_INTMSK_RE 				(0x1<<1)
#define MMCH_INTMSK_CD 				(0x1<<0)
#define MMCH_INTMSK_SDIO_INTR		(0xffff<<16)
#define MMCH_INTMSK_SDIO_CARD(x)	(0x1<<(16+x))
#define MMCH_INTMSK_ALL_ENABLED		(0xffffffff)

/* Masked Interrupt Status Register defines */
#define MMCH_MINTSTS_EBE			(0x1<<15)
#define MMCH_MINTSTS_ACD			(0x1<<14)
#define MMCH_MINTSTS_SBE			(0x1<<13)
#define MMCH_MINTSTS_HLE			(0x1<<12)
#define MMCH_MINTSTS_FRUN			(0x1<<11)
#define MMCH_MINTSTS_HTO			(0x1<<10)
#define MMCH_MINTSTS_DRTO			(0x1<<9)
#define MMCH_MINTSTS_RTO			(0x1<<8)
#define MMCH_MINTSTS_DCRC			(0x1<<7)
#define MMCH_MINTSTS_RCRC			(0x1<<6)
#define MMCH_MINTSTS_RXDR			(0x1<<5)
#define MMCH_MINTSTS_TXDR			(0x1<<4)
#define MMCH_MINTSTS_DTO			(0x1<<3)
#define MMCH_MINTSTS_CMD_DONE		(0x1<<2)
#define MMCH_MINTSTS_RE				(0x1<<1)
#define MMCH_MINTSTS_CD				(0x1<<0)
#define MMCH_MINTSTS_SDIO_INTR 		(0xffff<<16)
#define MMCH_MINTSTS_SDIO_CARD(x)	(0x1<<(16+x))
#define MMCH_MINTSTS_ALL_ENABLED	(0xffffffff)
#define MMCH_MINTSTS_ERROR			( MMCH_MINTSTS_EBE | MMCH_MINTSTS_SBE | MMCH_MINTSTS_HLE \
									| MMCH_MINTSTS_FRUN | MMCH_MINTSTS_HTO | MMCH_MINTSTS_DRTO \
									| MMCH_MINTSTS_RTO | MMCH_MINTSTS_DCRC | MMCH_MINTSTS_RCRC \
									| MMCH_MINTSTS_RE )
#define MMCH_MINTSTS_NORMAL			( MMCH_MINTSTS_ACD | MMCH_MINTSTS_RXDR | MMCH_MINTSTS_TXDR \
									| MMCH_MINTSTS_DTO | MMCH_MINTSTS_CMD_DONE | MMCH_MINTSTS_CD)


/* Raw Interrupt Status Register defines */
#define MMCH_RINTSTS_EBE			(0x1<<15)
#define MMCH_RINTSTS_ACD			(0x1<<14)
#define MMCH_RINTSTS_SBE			(0x1<<13)
#define MMCH_RINTSTS_HLE			(0x1<<12)
#define MMCH_RINTSTS_FRUN			(0x1<<11)
#define MMCH_RINTSTS_HTO			(0x1<<10)
#define MMCH_RINTSTS_DRTO			(0x1<<9)
#define MMCH_RINTSTS_RTO			(0x1<<8)
#define MMCH_RINTSTS_DCRC			(0x1<<7)
#define MMCH_RINTSTS_RCRC			(0x1<<6)
#define MMCH_RINTSTS_RXDR			(0x1<<5)
#define MMCH_RINTSTS_TXDR			(0x1<<4)
#define MMCH_RINTSTS_DTO			(0x1<<3)
#define MMCH_RINTSTS_CMD_DONE		(0x1<<2)
#define MMCH_RINTSTS_RE				(0x1<<1)
#define MMCH_RINTSTS_CD				(0x1<<0)
#define MMCH_RINTSTS_SDIO_INTR 		(0xffff<<16)
#define MMCH_RINTSTS_SDIO_CARD(x)	(0x1<<(16+x))
#define MMCH_RINTSTS_ALL_ENABLED	(0xffffffff)
#define MMCH_RINTSTS_ERROR			\
				(MMCH_RINTSTS_HLE | MMCH_RINTSTS_FRUN | MMCH_RINTSTS_HTO | MMCH_RINTSTS_EBE |\
				 MMCH_RINTSTS_SBE | MMCH_RINTSTS_DCRC | MMCH_RINTSTS_RCRC | MMCH_RINTSTS_RE | \
				 MMCH_RINTSTS_RTO | MMCH_RINTSTS_DRTO)

/*	Status Register defines */
#define MMCH_STATUS_DMA_REQ					(0x1<<31)
#define MMCH_STATUS_DMA_ACK					(0x1<<30)
#define MMCH_STATUS_FIFO_COUNT 				(0x1<<17)
#define MMCH_STATUS_RESP_INDEX 				(0x1<<30)
#define MMCH_STATUS_DATA_STATE_MC_BUSY 		(0x1<<10)
#define MMCH_STATUS_DATA_BUSY				(0x1<<9)
#define MMCH_STATUS_DATA_3_STATUS			(0x1<<8)
#define MMCH_STATUS_CMD_FSM_STATES 			(0x1<<4)
#define MMCH_STATUS_FIFO_FULL				(0x1<<3)
#define MMCH_STATUS_FIFO_EMPTY 				(0x1<<2)
#define MMCH_STATUS_FIFO_TX_WATERMARK		(0x1<<1)
#define MMCH_STATUS_FIFO_RX_WATERMARK		(0x1<<0)

/* CMD Register Defines */
#define MMCH_CMD_START_CMD 					(0x1<<31)
#define MMCH_CMD_BOOT_MODE 					(0x1<<27)
#define MMCH_CMD_DISABLE_BOOT				(0x1<<26)
#define MMCH_CMD_EXPECT_BOOT_ACK			(0x1<<25)
#define MMCH_CMD_ENABLE_BOOT				(0x1<<24)
#define MMCH_CMD_CCS_EXPECTED				(0x1<<23)
#define MMCH_CMD_READ_CEATA_DEVICE 			(0x1<<22)
#define MMCH_CMD_UPDATE_CLOCK_REGISTERS_ONLY	(0x1<<21)
#define MMCH_CMD_SEND_INITIALIZATION			(0x1<<15)
#define MMCH_CMD_STOP_ABORT_CMD				(0x1<<14)
#define MMCH_CMD_WARVDATA_COMPLETE 			(0x1<<13)
#define MMCH_CMD_SEND_AUTO_STOP				(0x1<<12)
#define MMCH_CMD_TRANSFER_MODE 				(0x1<<11)
#define MMCH_CMD_READ_WRITE					(0x1<<10)
#define MMCH_CMD_DATA_EXPECTED 				(0x1<<9)
#define MMCH_CMD_CHECK_RESPONSE_CRC			(0x1<<8)
#define MMCH_CMD_RESPONSE_LENGTH			(0x1<<7)
#define MMCH_CMD_RESPONSE_EXPECT			(0x1<<6)
#define MMCH_CMD_MAX_RETRIES				(20000)		// ????

/* Hardware Configuration Register */
#define MMCH_HCON_MMC_ONLY 					(0x0<<0)
#define MMCH_HCON_SD_MMC					(0x1<<0)
#define MMCH_HCON_APB						(0x0<<6)
#define MMCH_HCON_AHB						(0x1<<6)
#define MMCH_HCON_DW_DMA					(0x1<<16)
#define MMCH_HCON_GENERIC_DMA				(0x2<<16)
#define MMCH_HCON_GE_DMA_DATA_WIDTH_16BITS 	(0x0<<18)
#define MMCH_HCON_GE_DMA_DATA_WIDTH_32BITS 	(0x1<<18)
#define MMCH_HCON_GE_DMA_DATA_WIDTH_64BITS 	(0x2<<18)
#define MMCH_HCON_FIFO_RAM_OUTSIDE 			(0x0<<21)
#define MMCH_HCON_FIFO_RAM_INSIDE			(0x1<<21)
#define MMCH_HCON_IMPLEMENT_NO_HOLD_REG		(0x0<<22)
#define MMCH_HCON_IMPLEMENT_HOLD_REG		(0x1<<22)
#define MMCH_HCON_SET_CLK_NO_FALSE_PATH		(0x0<<23)
#define MMCH_HCON_SET_CLK_FALSE_PATH		(0x1<<23)
#define MMCH_HCON_NO_AREA_OPTIMIZED			(0x0<<26)
#define MMCH_HCON_AREA_OPTIMIZED			(0x1<<26)

/* Internal DMAC Status Register */
#define MMCH_IDSTS_FSM_DMA_IDLE				(0x0<<13)
#define MMCH_IDSTS_FSM_DMA_SUSPEND 			(0x1<<13)
#define MMCH_IDSTS_FSM_DESC_RD 				(0x2<<13)
#define MMCH_IDSTS_FSM_DESC_CHK				(0x3<<13)
#define MMCH_IDSTS_FSM_DMA_RD_REQ_WAIT 		(0x4<<13)
#define MMCH_IDSTS_FSM_DMA_WR_REQ_WAIT 		(0x5<<13)
#define MMCH_IDSTS_FSM_DMA_RD				(0x6<<13)
#define MMCH_IDSTS_FSM_DMA_WR				(0x7<<13)
#define MMCH_IDSTS_FSM_DESC_CLOSE			(0x8<<13)
#define MMCH_IDSTS_EB 						(0x7<<10)	// Error bits
#define MMCH_IDSTS_EB_TRANS					(0x1<<10)	// Error bits
#define MMCH_IDSTS_EB_RECEIVE 				(0x2<<10)	// Error bits
#define MMCH_IDSTS_AIS 						(0x1<<9)
#define MMCH_IDSTS_NIS 						(0x1<<8)
#define MMCH_IDSTS_CES 						(0x1<<5)
#define MMCH_IDSTS_DU						(0x1<<4)
#define MMCH_IDSTS_FBE 						(0x1<<2)
#define MMCH_IDSTS_RI						(0x1<<1)
#define MMCH_IDSTS_TI						(0x1<<0)

/* Card Type Register */
#define MMCH_CTYPE_NON_8BIT_MODE			(0x0<<16)
#define MMCH_CTYPE_8BIT_MODE				(0x1<<16)
#define MMCH_CTYPE_1BIT_MODE				(0x0<<0)
#define MMCH_CTYPE_4BIT_MODE				(0x1<<0)

/* Bus Mode Register */
#define MMCH_BMOD_PBL_1TRANS				(0x0<<8)
#define MMCH_BMOD_PBL_4TRANS				(0x1<<8)
#define MMCH_BMOD_PBL_8TRANS				(0x2<<8)
#define MMCH_BMOD_PBL_16TRANS				(0x3<<8)
#define MMCH_BMOD_PBL_32TRANS				(0x4<<8)
#define MMCH_BMOD_PBL_64TRANS				(0x5<<8)
#define MMCH_BMOD_PBL_128TRANS 				(0x6<<8)
#define MMCH_BMOD_PBL_256TRANS 				(0x7<<8)
#define MMCH_BMOD_DE						(0x1<<7)
#define MMCH_BMOD_DSL(x)					((x)<<2)
#define MMCH_BMOD_FB						(0x1<<1)
#define MMCH_BMOD_SWR						(0x1<<0)

/* Internal DMAC Interrupt Enable Register Bit Definitions */
#define MMCH_IDMAC_AI 						(0x1<<9)	// Abnormal Interrupt Summary Enable/ Status			9
#define MMCH_IDMAC_NI 						(0x1<<8)	// Normal Interrupt Summary Enable/ Status				8
#define MMCH_IDMAC_CES						(0x1<<5)	// Card Error Summary Interrupt Enable/ status			5
#define MMCH_IDMAC_DU 						(0x1<<4)	// Descriptor Unavailabe Interrupt Enable /Status		4
#define MMCH_IDMAC_FBE						(0x1<<2)	// Fata Bus Error Enable/ Status						2
#define MMCH_IDMAC_RI 						(0x1<<1)	// Rx Interrupt Enable/ Status							1
#define MMCH_IDMAC_TI 						(0x1<<0)	// Tx Interrupt Enable/ Status							0


/* FIFO Threshold Watermark Register */
#define MMCH_FIFOTH_DW_DMA_MTS_1TRANS			(0x0<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_4TRANS			(0x1<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_8TRANS			(0x2<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_16TRANS 			(0x3<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_32TRANS 			(0x4<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_64TRANS 			(0x5<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_128TRANS			(0x6<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_256TRANS			(0x7<<28)
#define MMCH_FIFOTH_RX_WMARK(x)					((x)<<16)
#define MMCH_FIFOTH_TX_WMARK(x)					(x)

/* Operation Conditions Register (OCR) Register Definition */
#define MMCH_OCR_BYTE_MODE 				0x00000000
#define MMCH_OCR_SECTOR_MODE			0x40000000
#define MMCH_OCR_POWER_UP_STATUS		0x80000000
#define MMCH_OCR_27TO36					0x00ff8000

/* Operation Conditions Register (OCR) Register Definition */
#define MMCH_GET_FIFO_DEPTH(x) 	 		((((x)&0x0FFF0000)>>16)+1)
#define MMCH_GET_FIFO_COUNT(x) 			(((x)&0x3ffe0000)>>17)

#define MMCH_MAX_DIVIDER_VALUE 			(0xff)


#define DEFAULT_DEBNCE_VAL				(0x0FFFFF)

#define MMCH_SET_BITS(x,y)				((x)|=(y))
#define MMCH_UNSET_BITS(x,y)			((x)&=(~(y)))	  

#define MMCH_WAIT_COUNT					0xFFFFFFF

#define MMCH_IDMAC_MAX_BUFFER			0x1000 		// ????

#define MMCH_ENABLE_ALL					0xFFFFFFFF

#define MMCH_UPDATE_CLOCK			\
			MMCH_CMD_START_CMD | MMCH_CMD_UPDATE_CLOCK_REGISTERS_ONLY | MMCH_CMD_WARVDATA_COMPLETE

#define MMCH_XFER_FLAGS	(MMC_DATA_READ | MMC_DATA_WRITE)

/*
 DBADDR  = 0x88 : Descriptor List Base Address Register
 The DBADDR is the pointer to the first Descriptor
 The Descriptor format in Little endian with a 32 bit Data bus is as shown below 
		   --------------------------------------------------------------------------
	 DES0 | OWN (31)| Control and Status											 |
	   --------------------------------------------------------------------------
	 DES1 | Reserved |		   Buffer 2 Size		|		 Buffer 1 Size			 |
	   --------------------------------------------------------------------------
	 DES2 |  Buffer Address Pointer 1												 |
	   --------------------------------------------------------------------------
	 DES3 |  Buffer Address Pointer 2/Next Descriptor Address Pointer(CHAINED MODE)  |
	   --------------------------------------------------------------------------
*/

typedef volatile struct {
	u32 status;		 	/* control and status information of descriptor */
	u32 length;		 	/* buffer sizes								 */
	u32 buffer1_paddr;	/* physical address of the buffer 1			 */  
	u32 buffer2_paddr;	/* physical address of the buffer 2	or Nest Descriptor address	 */  
}DMA_DESC_T;

enum DmaDescriptorDES0	  // Control and status word of DMA descriptor DES0 
{
	 DescOwnByDma		   = 0x80000000,   /* (OWN)Descriptor is owned by DMA engine			  31   */
	 DescCardErrSummary    = 0x40000000,   /* Indicates EBE/RTO/RCRC/SBE/DRTO/DCRC/RE			  30   */
	 DescEndOfRing		   = 0x00000020,   /* A "1" indicates End of Ring for Ring Mode 		  05   */
	 DescSecAddrChained    = 0x00000010,   /* A "1" indicates DES3 contains Next Desc Address	  04   */
	 DescFirstDesc		   = 0x00000008,   /* A "1" indicates this Desc contains first			  03
											  buffer of the data									   */
	 DescLastDesc		   = 0x00000004,   /* A "1" indicates buffer pointed to by this this	  02
											  Desc contains last buffer of Data 					   */
	 DescDisInt 		   = 0x00000002,   /* A "1" in this field disables the RI/TI of IDSTS	  01
											  for data that ends in the buffer pointed to by 
											  this descriptor										   */	  
};

enum DmaDescriptorDES1	  // Buffer's size field of Descriptor
{
	 DescBuf2SizMsk 	  = 0x03FFE000,    /* Mask for Buffer2 Size 						   25:13   */
	 DescBuf2SizeShift	  = 13, 		   /* Shift value for Buffer2 Size							   */
	 DescBuf1SizMsk 	  = 0x00001FFF,    /* Mask for Buffer1 Size 						   12:0    */
	 DescBuf1SizeShift	  = 0,			   /* Shift value for Buffer2 Size							   */
};

typedef struct {
/* linux mmc layer variable */
	struct mmc_host *mmc;
	struct mmc_request *mrq;
//	struct mmc_command 	*cmd;
//	struct mmc_data 	*data;
	struct mmc_request *req_mrq;	// 111204

/* sdp mmc host contoller  */
	void 	__iomem *base;
	struct 	resource *mem_res;
	int 	irq;

/* Descrtiptor List Base address */
	DMA_DESC_T * p_dmadesc_vbase;
	dma_addr_t dmadesc_pbase;

	unsigned int	xfer;
	unsigned int 	flags;

	spinlock_t			lock;
	struct semaphore 	mutex;

	u32 read;
	u8 *buffer; 				//buffer of the PIO Read/Write operation
	u32 buffer_bytes_count; 	//transition size of the PIO Read/Write operation
	u32 mrq_data_size;
	int nr_intr_mrq;

/* TODO : move to mach file in next project 20100926 */
	u64 dma_mask;

	/* scatterlists */
	unsigned int		sg_len;
	struct scatterlist	*sg;

//#ifdef CONFIG_PM
#if 1
/* PM mmc variable */
	unsigned int  pm_clock;
	unsigned char pm_bus_mode;
	unsigned char pm_bus_width;

	unsigned int pm_clk_delay[4]; // 4 array is enough
#endif

}SDP_MMCH_T;

typedef struct {
	/* get_cd()/get_wp() may sleep */
	int (*get_cd)(int module);
	int (*get_ro)(int module);
}SDP_MMC_CONFIG;

#endif	/* __SDP_MMC_HOST_H */






