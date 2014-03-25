/*
 *	arch/arm/plat-ccep/sdp_mmc.h - Samsung DTV/BD MMC host controller driver
 *
 *	Copyright (C) 2010 Samsung Co.
 *  Created by tukho.kim@samsung.com (#0717)
 *
 *  20120320 drain.lee	add struct sdp_mmch_plat and
 *						change name DMA_DESC_T to MMCH_DMA_DESC_T
 *  20120417 drain.lee	add IP ver.2.41a regsters.
 *  20120614 drain.lee	add MMCH_IDSTS_INTR_ALL define.
 						add enum sdp_mmch_state.
 *  20120719 drain.lee	fix sdp_mmch_state_events.
 *  20120921 drain.lee	add, define MMCH_FIFOTH_DW_DMA_MTS
 *  20121212 drain.lee	remove dma_mask.
 *  20121221 drain.lee	fix compile warning.
 */

#ifndef __SDP_MMC_HOST_H
#define __SDP_MMC_HOST_H

#include <linux/semaphore.h>
#include <linux/mmc/host.h>


#define MMCH_DESC_NUM			512   		// 4096 * 32
#define MMCH_DESC_SIZE			(sizeof(MMCH_DMA_DESC_T) * MMCH_DESC_NUM)
#define NR_SG					MMCH_DESC_NUM

#define MMCH_CLKDIV_LIMIT		0xff 		// Max Divider Operation clock / ((0~255) * 2)


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
	MMCH_Reserved		= 0x74,	 	/** Reserved (old versionn compatible) */
	MMCH_UHS_REG		= 0x74,	 	/** UHS-1 Register */
	MMCH_RST_n			= 0x78,	 	/** card H/W reset */

/* DMA register */
	MMCH_BMOD			= 0x80,		/** Bus mode Register */
	MMCH_PLDMND			= 0x84,		/** Poll Demand */
	MMCH_DBADDR			= 0x88,		/** Descriptor Base Address */
	MMCH_IDSTS			= 0x8C,		/** Internal DMAC Status */
	MMCH_IDINTEN		= 0x90,	  	/** Internal DMAC Interrupt Enable */
	MMCH_DSCADDR		= 0x94,	  	/** Current Host Descriptor Address */
	MMCH_BUFADDR		= 0x98,		/** Current Host Buffer Address */
	MMCH_CARDTHRCTL		= 0x100,	/** Card Threshold Control Register new 2.41a */
	MMCH_BACKENDPWR		= 0x104,	/** Back-end Power Register new 2.41a */

	MMCH_FIFODAT		= 0x200,   	/** FIFO data read write */
} Controller_Reg ;

/* MMC Host Control register definitions */
#define MMCH_CTRL_USE_INTERNAL_DMAC					(0x1UL<<25)
#define MMCH_CTRL_ENABLE_OD_PULLUP 					(0x1UL<<24)
#define MMCH_CTRL_CEATA_DEVICE_INTERRUPT_STATUS		(0x1UL<<11)
#define MMCH_CTRL_SEND_AUTO_STOP_CCSD				(0x1UL<<10)
#define MMCH_CTRL_SEND_CCSD							(0x1UL<<9)
#define MMCH_CTRL_ABORT_READ_DATA					(0x1UL<<8)
#define MMCH_CTRL_SEND_IRQ_RESPONSE					(0x1UL<<7)
#define MMCH_CTRL_READ_WAIT							(0x1UL<<6)
#define MMCH_CTRL_DMA_ENABLE						(0x1UL<<5)
#define MMCH_CTRL_INT_ENABLE						(0x1UL<<4)
#define MMCH_CTRL_DMA_RESET							(0x1UL<<2)
#define MMCH_CTRL_FIFO_RESET						(0x1UL<<1)
#define MMCH_CTRL_CONTROLLER_RESET 					(0x1UL<<0)

/* Clock Divider register definitions */
#define MMCH_CLKDIV_3(x)			((x)<<24)
#define MMCH_CLKDIV_2(x)			((x)<<16)
#define MMCH_CLKDIV_1(x)			((x)<<8)
#define MMCH_CLKDIV_0(x)			((x)<<0)

/* Clock Enable register definitions */
#define MMCH_CLKENA_ALL_CCLK_ENABLE			(0xffffUL)

/* Timeout register definitions */
#define MMCH_TMOUT_DATA_TIMEOUT(x) 		((x)<<8)
#define MMCH_TMOUT_RESPONSE_TIMEOUT(x) 	((x)<<0)

/* Interrupt mask defines */
#define MMCH_INTMSK_EBE				(0x1UL<<15)
#define MMCH_INTMSK_ACD				(0x1UL<<14)
#define MMCH_INTMSK_SBE				(0x1UL<<13)
#define MMCH_INTMSK_HLE				(0x1UL<<12)
#define MMCH_INTMSK_FRUN			(0x1UL<<11)
#define MMCH_INTMSK_HTO				(0x1UL<<10)
#define MMCH_INTMSK_DRTO			(0x1UL<<9)
#define MMCH_INTMSK_RTO				(0x1UL<<8)
#define MMCH_INTMSK_DCRC			(0x1UL<<7)
#define MMCH_INTMSK_RCRC			(0x1UL<<6)
#define MMCH_INTMSK_RXDR			(0x1UL<<5)
#define MMCH_INTMSK_TXDR			(0x1UL<<4)
#define MMCH_INTMSK_DTO				(0x1UL<<3)
#define MMCH_INTMSK_CMD_DONE		(0x1UL<<2)
#define MMCH_INTMSK_RE 				(0x1UL<<1)
#define MMCH_INTMSK_CD 				(0x1UL<<0)
#define MMCH_INTMSK_SDIO_INTR		(0xffffUL<<16)
#define MMCH_INTMSK_SDIO_CARD(x)	(0x1UL<<(16+x))
#define MMCH_INTMSK_ALL_ENABLED		(0x0000ffffUL)

/* Masked Interrupt Status Register defines */
#define MMCH_MINTSTS_EBE			(0x1UL<<15)
#define MMCH_MINTSTS_ACD			(0x1UL<<14)
#define MMCH_MINTSTS_SBE			(0x1UL<<13)
#define MMCH_MINTSTS_HLE			(0x1UL<<12)
#define MMCH_MINTSTS_FRUN			(0x1UL<<11)
#define MMCH_MINTSTS_HTO			(0x1UL<<10)
#define MMCH_MINTSTS_DRTO			(0x1UL<<9)
#define MMCH_MINTSTS_RTO			(0x1UL<<8)
#define MMCH_MINTSTS_DCRC			(0x1UL<<7)
#define MMCH_MINTSTS_RCRC			(0x1UL<<6)
#define MMCH_MINTSTS_RXDR			(0x1UL<<5)
#define MMCH_MINTSTS_TXDR			(0x1UL<<4)
#define MMCH_MINTSTS_DTO			(0x1UL<<3)
#define MMCH_MINTSTS_CMD_DONE		(0x1UL<<2)
#define MMCH_MINTSTS_RE				(0x1UL<<1)
#define MMCH_MINTSTS_CD				(0x1UL<<0)
#define MMCH_MINTSTS_SDIO_INTR 		(0xffffUL<<16)
#define MMCH_MINTSTS_SDIO_CARD(x)	(0x1UL<<(16+x))
#define MMCH_MINTSTS_ALL_ENABLED	(0xffffffffUL)
#define MMCH_MINTSTS_ERROR_CMD		(MMCH_MINTSTS_RTO | MMCH_MINTSTS_RCRC | MMCH_MINTSTS_RE)
#define MMCH_MINTSTS_ERROR_DATA		( MMCH_MINTSTS_EBE | MMCH_MINTSTS_SBE | MMCH_MINTSTS_HLE \
									| MMCH_MINTSTS_FRUN | MMCH_MINTSTS_HTO | MMCH_MINTSTS_DRTO \
									| MMCH_MINTSTS_DCRC )
#define MMCH_MINTSTS_ERROR			( MMCH_MINTSTS_ERROR_CMD | MMCH_MINTSTS_ERROR_DATA )
#define MMCH_MINTSTS_NORMAL			( MMCH_MINTSTS_ACD | MMCH_MINTSTS_RXDR | MMCH_MINTSTS_TXDR \
									| MMCH_MINTSTS_DTO | MMCH_MINTSTS_CMD_DONE | MMCH_MINTSTS_CD)


/* Raw Interrupt Status Register defines */
#define MMCH_RINTSTS_EBE			(0x1UL<<15)
#define MMCH_RINTSTS_ACD			(0x1UL<<14)
#define MMCH_RINTSTS_SBE			(0x1UL<<13)
#define MMCH_RINTSTS_HLE			(0x1UL<<12)
#define MMCH_RINTSTS_FRUN			(0x1UL<<11)
#define MMCH_RINTSTS_HTO			(0x1UL<<10)
#define MMCH_RINTSTS_DRTO			(0x1UL<<9)
#define MMCH_RINTSTS_RTO			(0x1UL<<8)
#define MMCH_RINTSTS_DCRC			(0x1UL<<7)
#define MMCH_RINTSTS_RCRC			(0x1UL<<6)
#define MMCH_RINTSTS_RXDR			(0x1UL<<5)
#define MMCH_RINTSTS_TXDR			(0x1UL<<4)
#define MMCH_RINTSTS_DTO			(0x1UL<<3)
#define MMCH_RINTSTS_CMD_DONE		(0x1UL<<2)
#define MMCH_RINTSTS_RE				(0x1UL<<1)
#define MMCH_RINTSTS_CD				(0x1UL<<0)
#define MMCH_RINTSTS_SDIO_INTR 		(0xffffUL<<16)
#define MMCH_RINTSTS_SDIO_CARD(x)	(0x1UL<<(16+x))
#define MMCH_RINTSTS_ALL_ENABLED	(0xffffffffUL)
#define MMCH_RINTSTS_ERROR			\
				(MMCH_RINTSTS_HLE | MMCH_RINTSTS_FRUN | MMCH_RINTSTS_HTO | MMCH_RINTSTS_EBE |\
				 MMCH_RINTSTS_SBE | MMCH_RINTSTS_DCRC | MMCH_RINTSTS_RCRC | MMCH_RINTSTS_RE | \
				 MMCH_RINTSTS_RTO | MMCH_RINTSTS_DRTO)

/*	Status Register defines */
#define MMCH_STATUS_DMA_REQ					(0x1UL<<31)
#define MMCH_STATUS_DMA_ACK					(0x1UL<<30)
#define MMCH_STATUS_FIFO_COUNT 				(0x1UL<<17)
#define MMCH_STATUS_RESP_INDEX 				(0x1UL<<11)
#define MMCH_STATUS_DATA_STATE_MC_BUSY 		(0x1UL<<10)
#define MMCH_STATUS_DATA_BUSY				(0x1UL<<9)
#define MMCH_STATUS_DATA_3_STATUS			(0x1UL<<8)
#define MMCH_STATUS_CMD_FSM_STATES 			(0x1UL<<4)
#define MMCH_STATUS_FIFO_FULL				(0x1UL<<3)
#define MMCH_STATUS_FIFO_EMPTY 				(0x1UL<<2)
#define MMCH_STATUS_FIFO_TX_WATERMARK		(0x1UL<<1)
#define MMCH_STATUS_FIFO_RX_WATERMARK		(0x1UL<<0)
#define MMCH_STATUS_GET_FIFO_CNT(x)		(((x)>>17) & 0x1FFUL)
#define MMCH_STATUS_FIFO_SZ			128

/* CMD Register Defines */
#define MMCH_CMD_START_CMD 					(0x1UL<<31)
#define MMCH_CMD_USE_HOLD_REG				(0x1UL<<29)// new 2.41a
#define MMCH_CMD_BOOT_MODE 					(0x1UL<<27)
#define MMCH_CMD_DISABLE_BOOT				(0x1UL<<26)
#define MMCH_CMD_EXPECT_BOOT_ACK			(0x1UL<<25)
#define MMCH_CMD_ENABLE_BOOT				(0x1UL<<24)
#define MMCH_CMD_CCS_EXPECTED				(0x1UL<<23)
#define MMCH_CMD_READ_CEATA_DEVICE 			(0x1UL<<22)
#define MMCH_CMD_UPDATE_CLOCK_REGISTERS_ONLY	(0x1UL<<21)
#define MMCH_CMD_SEND_INITIALIZATION			(0x1UL<<15)
#define MMCH_CMD_STOP_ABORT_CMD				(0x1UL<<14)
#define MMCH_CMD_WARVDATA_COMPLETE 			(0x1UL<<13)
#define MMCH_CMD_SEND_AUTO_STOP				(0x1UL<<12)
#define MMCH_CMD_TRANSFER_MODE 				(0x1UL<<11)
#define MMCH_CMD_READ_WRITE					(0x1UL<<10)
#define MMCH_CMD_DATA_EXPECTED 				(0x1UL<<9)
#define MMCH_CMD_CHECK_RESPONSE_CRC			(0x1UL<<8)
#define MMCH_CMD_RESPONSE_LENGTH			(0x1UL<<7)
#define MMCH_CMD_RESPONSE_EXPECT			(0x1UL<<6)
#define MMCH_CMD_MAX_RETRIES				(20000)		// ????

/* Hardware Configuration Register */
#define MMCH_HCON_MMC_ONLY 					(0x0UL<<0)
#define MMCH_HCON_SD_MMC					(0x1UL<<0)
#define MMCH_HCON_APB						(0x0UL<<6)
#define MMCH_HCON_AHB						(0x1UL<<6)
#define MMCH_HCON_DW_DMA					(0x1UL<<16)
#define MMCH_HCON_GENERIC_DMA				(0x2UL<<16)
#define MMCH_HCON_GE_DMA_DATA_WIDTH_16BITS 	(0x0UL<<18)
#define MMCH_HCON_GE_DMA_DATA_WIDTH_32BITS 	(0x1UL<<18)
#define MMCH_HCON_GE_DMA_DATA_WIDTH_64BITS 	(0x2UL<<18)
#define MMCH_HCON_FIFO_RAM_OUTSIDE 			(0x0UL<<21)
#define MMCH_HCON_FIFO_RAM_INSIDE			(0x1UL<<21)
#define MMCH_HCON_IMPLEMENT_NO_HOLD_REG		(0x0UL<<22)
#define MMCH_HCON_IMPLEMENT_HOLD_REG		(0x1UL<<22)
#define MMCH_HCON_SET_CLK_NO_FALSE_PATH		(0x0UL<<23)
#define MMCH_HCON_SET_CLK_FALSE_PATH		(0x1UL<<23)
#define MMCH_HCON_NO_AREA_OPTIMIZED			(0x0UL<<26)
#define MMCH_HCON_AREA_OPTIMIZED			(0x1UL<<26)

/* Internal DMAC Status Register */
#define MMCH_IDSTS_FSM_DMA_IDLE				(0x0UL<<13)
#define MMCH_IDSTS_FSM_DMA_SUSPEND 			(0x1UL<<13)
#define MMCH_IDSTS_FSM_DESC_RD 				(0x2UL<<13)
#define MMCH_IDSTS_FSM_DESC_CHK				(0x3UL<<13)
#define MMCH_IDSTS_FSM_DMA_RD_REQ_WAIT 		(0x4UL<<13)
#define MMCH_IDSTS_FSM_DMA_WR_REQ_WAIT 		(0x5UL<<13)
#define MMCH_IDSTS_FSM_DMA_RD				(0x6UL<<13)
#define MMCH_IDSTS_FSM_DMA_WR				(0x7UL<<13)
#define MMCH_IDSTS_FSM_DESC_CLOSE			(0x8UL<<13)
#define MMCH_IDSTS_EB 						(0x7UL<<10)	// Error bits
#define MMCH_IDSTS_EB_TRANS					(0x1UL<<10)	// Error bits
#define MMCH_IDSTS_EB_RECEIVE 				(0x2UL<<10)	// Error bits
#define MMCH_IDSTS_AIS 						(0x1UL<<9)
#define MMCH_IDSTS_NIS 						(0x1UL<<8)
#define MMCH_IDSTS_CES 						(0x1UL<<5)
#define MMCH_IDSTS_DU						(0x1UL<<4)
#define MMCH_IDSTS_FBE 						(0x1UL<<2)
#define MMCH_IDSTS_RI						(0x1UL<<1)
#define MMCH_IDSTS_TI						(0x1UL<<0)
#define MMCH_IDSTS_INTR_ALL		(MMCH_IDSTS_AIS | MMCH_IDSTS_NIS | MMCH_IDSTS_CES \
			| MMCH_IDSTS_DU | MMCH_IDSTS_FBE | MMCH_IDSTS_RI | MMCH_IDSTS_TI)

/* Card Type Register */
#define MMCH_CTYPE_NON_8BIT_MODE			(0x0UL<<16)
#define MMCH_CTYPE_8BIT_MODE				(0x1UL<<16)
#define MMCH_CTYPE_1BIT_MODE				(0x0UL<<0)
#define MMCH_CTYPE_4BIT_MODE				(0x1UL<<0)

/* Bus Mode Register */
#define MMCH_BMOD_PBL_1TRANS				(0x0UL<<8)
#define MMCH_BMOD_PBL_4TRANS				(0x1UL<<8)
#define MMCH_BMOD_PBL_8TRANS				(0x2UL<<8)
#define MMCH_BMOD_PBL_16TRANS				(0x3UL<<8)
#define MMCH_BMOD_PBL_32TRANS				(0x4UL<<8)
#define MMCH_BMOD_PBL_64TRANS				(0x5UL<<8)
#define MMCH_BMOD_PBL_128TRANS 				(0x6UL<<8)
#define MMCH_BMOD_PBL_256TRANS 				(0x7UL<<8)
#define MMCH_BMOD_DE						(0x1UL<<7)
#define MMCH_BMOD_DSL(x)					(((x)*1UL)<<2)
#define MMCH_BMOD_FB						(0x1UL<<1)
#define MMCH_BMOD_SWR						(0x1UL<<0)

/* Internal DMAC Interrupt Enable Register Bit Definitions */
#define MMCH_IDMAC_AI 						(0x1UL<<9)	// Abnormal Interrupt Summary Enable/ Status			9
#define MMCH_IDMAC_NI 						(0x1UL<<8)	// Normal Interrupt Summary Enable/ Status				8
#define MMCH_IDMAC_CES						(0x1UL<<5)	// Card Error Summary Interrupt Enable/ status			5
#define MMCH_IDMAC_DU 						(0x1UL<<4)	// Descriptor Unavailabe Interrupt Enable /Status		4
#define MMCH_IDMAC_FBE						(0x1UL<<2)	// Fata Bus Error Enable/ Status						2
#define MMCH_IDMAC_RI 						(0x1UL<<1)	// Rx Interrupt Enable/ Status							1
#define MMCH_IDMAC_TI 						(0x1UL<<0)	// Tx Interrupt Enable/ Status							0


/* FIFO Threshold Watermark Register */
#define MMCH_FIFOTH_DW_DMA_MTS_1TRANS			(0x0UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_4TRANS			(0x1UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_8TRANS			(0x2UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_16TRANS 			(0x3UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_32TRANS 			(0x4UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_64TRANS 			(0x5UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_128TRANS			(0x6UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS_256TRANS			(0x7UL<<28)
#define MMCH_FIFOTH_DW_DMA_MTS(x)				((x)*1UL<<28)
#define MMCH_FIFOTH_RX_WMARK(x)					((x)*1UL<<16)
#define MMCH_FIFOTH_TX_WMARK(x)					((x)*1UL)

/* Operation Conditions Register (OCR) Register Definition */
#define MMCH_OCR_BYTE_MODE 				0x00000000UL
#define MMCH_OCR_SECTOR_MODE			0x40000000UL
#define MMCH_OCR_POWER_UP_STATUS		0x80000000UL
#define MMCH_OCR_27TO36					0x00ff8000UL

/* Operation Conditions Register (OCR) Register Definition */
#define MMCH_GET_FIFO_DEPTH(x) 	 		((((x)&0x0FFF0000)>>16)+1)
#define MMCH_GET_FIFO_COUNT(x) 			(((x)&0x3ffe0000)>>17)

#define MMCH_MAX_DIVIDER_VALUE 			(0xffUL)


#define DEFAULT_DEBNCE_VAL				(0x0FFFFFUL)

#define MMCH_SET_BITS(x,y)				((x)|=(y))
#define MMCH_UNSET_BITS(x,y)			((x)&=(~(y)))

#define MMCH_WAIT_COUNT					0xFFFFFFFUL

#define MMCH_IDMAC_MAX_BUFFER			0x1000UL 		// ????

#define MMCH_ENABLE_ALL					0xFFFFFFFFUL

#define MMCH_UPDATE_CLOCK			\
			MMCH_CMD_START_CMD | MMCH_CMD_UPDATE_CLOCK_REGISTERS_ONLY | MMCH_CMD_WARVDATA_COMPLETE

#define MMCH_XFER_FLAGS	(MMC_DATA_READ | MMC_DATA_WRITE)


#define MMCH_MINTSTS_EBE_BIT			(15)
#define MMCH_MINTSTS_ACD_BIT			(14)
#define MMCH_MINTSTS_SBE_BIT			(13)
#define MMCH_MINTSTS_HLE_BIT			(12)
#define MMCH_MINTSTS_FRUN_BIT			(11)
#define MMCH_MINTSTS_HTO_BIT			(10)
#define MMCH_MINTSTS_DRTO_BIT			(9)
#define MMCH_MINTSTS_RTO_BIT			(8)
#define MMCH_MINTSTS_DCRC_BIT			(7)
#define MMCH_MINTSTS_RCRC_BIT			(6)
#define MMCH_MINTSTS_RXDR_BIT			(5)
#define MMCH_MINTSTS_TXDR_BIT			(4)
#define MMCH_MINTSTS_DTO_BIT			(3)
#define MMCH_MINTSTS_CMD_DONE_BIT		(2)
#define MMCH_MINTSTS_RE_BIT				(1)
#define MMCH_MINTSTS_CD_BIT				(0)

#define MMCH_IDSTS_AIS_BIT 						(9)
#define MMCH_IDSTS_NIS_BIT 						(8)
#define MMCH_IDSTS_CES_BIT 						(5)
#define MMCH_IDSTS_DU_BIT						(4)
#define MMCH_IDSTS_FBE_BIT 						(2)
#define MMCH_IDSTS_RI_BIT						(1)
#define MMCH_IDSTS_TI_BIT						(0)

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
}MMCH_DMA_DESC_T;

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

/* sdp-mmc fsm state */
enum sdp_mmch_state {
	/* normal state */
	MMCH_STATE_IDLE = 0,
	MMCH_STATE_SENDING_CMD,/* in command send */
	MMCH_STATE_SENDING_DATA,/* in data xfer use dma*/
	MMCH_STATE_PROCESSING_DATA,/* in data processing */
	MMCH_STATE_SENDING_STOP,/* in send stop command */

	/* error state */
	MMCH_STATE_ERROR_CMD = 10,
	MMCH_STATE_ERROR_DATA,
};

enum sdp_mmch_xfer_mode {
	MMCH_XFER_DMA_MODE = 1,
	MMCH_XFER_PIO_MODE,
};

enum sdp_mmch_state_events {
	MMCH_EVENT_XFER_DONE			= 0,/* fifo to mem or mem to fifo data transfer is done */
	MMCH_EVENT_DATA_ERROR,
};

typedef struct _sdp_mmch_t {
/* linux mmc layer variable */
	struct mmc_host *mmc;
	struct mmc_request *mrq;	/* current process request */
	struct mmc_command *cmd;	/* current process command*/
//	struct mmc_data 	*data;
	unsigned long cmd_start_time;	/* CMD Start time(jiffies) */

/* sdp mmc host contoller  */
	struct platform_device *pdev;
	u8 	__iomem *base;
	struct 	resource *mem_res;
	unsigned int 	irq;
	enum sdp_mmch_xfer_mode xfer_mode;

/* Descrtiptor List Base address */
	MMCH_DMA_DESC_T * p_dmadesc_vbase;
	dma_addr_t dmadesc_pbase;

	enum sdp_mmch_state state;
	unsigned long state_event;

	u32 intr_status;/* MINTSTS */
	u32 dma_status;/* IDSTS */
	u32 data_error_status;/* data error */

	spinlock_t			lock;

	u32 mrq_data_size;/* request total bytes! */

	/* scatterlists */
	unsigned int		sg_len;
	struct scatterlist	*sg;

	/* use PIO */
	void	(*pio_push)(struct _sdp_mmch_t *, void *buf, size_t cnt);
	void	(*pio_pull)(struct _sdp_mmch_t *, void *buf, size_t cnt);
	u32 	pio_offset;

	u8		data_shift;
	u16		fifo_depth;

	struct timeval isr_time_pre;
	struct timeval isr_time_now;

//#ifdef CONFIG_PM
#if 1
/* PM mmc variable */
	unsigned int  pm_clock;
	unsigned char pm_bus_mode;
	unsigned char pm_bus_width;

	bool pm_is_valid_clk_delay;
	unsigned int pm_clk_delay[4]; // 4 array is enough
#endif

}SDP_MMCH_T;

/* Board platform data for sdp-mmc */
struct sdp_mmch_plat {
	u32 processor_clk;		/* input clock */
	u32 min_clk;
	u32 max_clk;
	u32 caps;				/* Capabilities */
	u8 fifo_depth;			/* MMC CORE fifo_depth, 0 is to use default. */
	int (*init)(SDP_MMCH_T *p_sdp_mmch);	/* board dependent init */
	bool force_pio_mode;		/* if true, MMC Running PIO Mode! */
	int (*get_ro)(u32 slot_id);
	int (*get_cd)(u32 slot_id);
	int (*get_ocr)(u32 slot_id);
//	int (*get_bus_wd)(u32 slot_id);
};

extern void sdp_mmch_debug_dump(struct mmc_host *mmc);

#endif	/* __SDP_MMC_HOST_H */
