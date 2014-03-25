/* linux/arch/arm/plat-samsung/include/plat/regs-otg.h
 *
 * Copyright (C) 2012
 *
 */

#ifndef __SDP_OTG_REGS_H
#define __SDP_OTG_REGS_H

/* USB2.0 OTG Controller register */
#define SDP_USBOTG_PHYREG(x)		((x) + SDP_VA_HSPHY)
#define SDP_USBOTG_PHYPWR		SDP_USBOTG_PHYREG(0x0)
#define SDP_USBOTG_PHYCLK		SDP_USBOTG_PHYREG(0x4)
#define SDP_USBOTG_RSTCON		SDP_USBOTG_PHYREG(0x8)
#define SDP_USBOTG_PHY1CON		SDP_USBOTG_PHYREG(0x34)

/* USB2.0 OTG Controller register */
#define SDP_USBOTGREG(x) (x)
/*============================================================================================== */
	/* Core Global Registers */
#define SDP_UDC_OTG_GOTGCTL			SDP_USBOTGREG(0x000)		/* OTG Control & Status */
#define SDP_UDC_OTG_GOTGINT			SDP_USBOTGREG(0x004)		/* OTG Interrupt */
#define SDP_UDC_OTG_GAHBCFG			SDP_USBOTGREG(0x008)		/* Core AHB Configuration */
#define SDP_UDC_OTG_GUSBCFG			SDP_USBOTGREG(0x00C)		/* Core USB Configuration */
#define SDP_UDC_OTG_GRSTCTL			SDP_USBOTGREG(0x010)		/* Core Reset */
#define SDP_UDC_OTG_GINTSTS			SDP_USBOTGREG(0x014)		/* Core Interrupt */
#define SDP_UDC_OTG_GINTMSK			SDP_USBOTGREG(0x018)		/* Core Interrupt Mask */
#define SDP_UDC_OTG_GRXSTSR			SDP_USBOTGREG(0x01C)		/* Receive Status Debug Read/Status Read */
#define SDP_UDC_OTG_GRXSTSP			SDP_USBOTGREG(0x020)		/* Receive Status Debug Pop/Status Pop */
#define SDP_UDC_OTG_GRXFSIZ			SDP_USBOTGREG(0x024)		/* Receive FIFO Size */
#define SDP_UDC_OTG_GNPTXFSIZ		SDP_USBOTGREG(0x028)		/* Non-Periodic Transmit FIFO Size */
#define SDP_UDC_OTG_GNPTXSTS		SDP_USBOTGREG(0x02C)		/* Non-Periodic Transmit FIFO/Queue Status */

#define SDP_UDC_OTG_GHWCFG1			SDP_USBOTGREG(0x044)
#define SDP_UDC_OTG_GHWCFG2			SDP_USBOTGREG(0x048)
#define SDP_UDC_OTG_GHWCFG3			SDP_USBOTGREG(0x04C)
#define SDP_UDC_OTG_GHWCFG4			SDP_USBOTGREG(0x050)

#define SDP_UDC_OTG_HPTXFSIZ		SDP_USBOTGREG(0x100)		/* Host Periodic Transmit FIFO Size */
#define SDP_UDC_OTG_DIEPTXF(n)		SDP_USBOTGREG(0x104 + (n-1)*0x4)/* Device IN EP Transmit FIFO Size Register */

/*============================================================================================== */
/* Host Mode Registers */
/*------------------------------------------------ */
/* Host Global Registers */
#define SDP_UDC_OTG_HCFG			SDP_USBOTGREG(0x400)		/* Host Configuration */
#define SDP_UDC_OTG_HFIR			SDP_USBOTGREG(0x404)		/* Host Frame Interval */
#define SDP_UDC_OTG_HFNUM			SDP_USBOTGREG(0x408)		/* Host Frame Number/Frame Time Remaining */
#define SDP_UDC_OTG_HPTXSTS			SDP_USBOTGREG(0x410)		/* Host Periodic Transmit FIFO/Queue Status */
#define SDP_UDC_OTG_HAINT			SDP_USBOTGREG(0x414)		/* Host All Channels Interrupt */
#define SDP_UDC_OTG_HAINTMSK		SDP_USBOTGREG(0x418)		/* Host All Channels Interrupt Mask */

/*------------------------------------------------ */
/* Host Port Control & Status Registers */
#define SDP_UDC_OTG_HPRT			SDP_USBOTGREG(0x440)		/* Host Port Control & Status */

/*------------------------------------------------ */
/* Host Channel-Specific Registers */
#define SDP_UDC_OTG_HCCHAR0			SDP_USBOTGREG(0x500)		/* Host Channel-0 Characteristics */
#define SDP_UDC_OTG_HCSPLT0			SDP_USBOTGREG(0x504)		/* Host Channel-0 Split Control */
#define SDP_UDC_OTG_HCINT0			SDP_USBOTGREG(0x508)		/* Host Channel-0 Interrupt */
#define SDP_UDC_OTG_HCINTMSK0		SDP_USBOTGREG(0x50C)		/* Host Channel-0 Interrupt Mask */
#define SDP_UDC_OTG_HCTSIZ0			SDP_USBOTGREG(0x510)		/* Host Channel-0 Transfer Size */
#define SDP_UDC_OTG_HCDMA0			SDP_USBOTGREG(0x514)		/* Host Channel-0 DMA Address */

/*============================================================================================== */
/* Device Mode Registers */
/*------------------------------------------------ */
/* Device Global Registers */
#define SDP_UDC_OTG_DCFG			SDP_USBOTGREG(0x800)		/* Device Configuration */
#define SDP_UDC_OTG_DCTL			SDP_USBOTGREG(0x804)		/* Device Control */
#define SDP_UDC_OTG_DSTS			SDP_USBOTGREG(0x808)		/* Device Status */
#define SDP_UDC_OTG_DIEPMSK			SDP_USBOTGREG(0x810)		/* Device IN Endpoint Common Interrupt Mask */
#define SDP_UDC_OTG_DOEPMSK			SDP_USBOTGREG(0x814)		/* Device OUT Endpoint Common Interrupt Mask */
#define SDP_UDC_OTG_DAINT			SDP_USBOTGREG(0x818)		/* Device All Endpoints Interrupt */
#define SDP_UDC_OTG_DAINTMSK		SDP_USBOTGREG(0x81C)		/* Device All Endpoints Interrupt Mask */
#define SDP_UDC_OTG_DTKNQR1			SDP_USBOTGREG(0x820)		/* Device IN Token Sequence Learning Queue Read 1 */
#define SDP_UDC_OTG_DTKNQR2			SDP_USBOTGREG(0x824)		/* Device IN Token Sequence Learning Queue Read 2 */
#define SDP_UDC_OTG_DVBUSDIS		SDP_USBOTGREG(0x828)		/* Device VBUS Discharge Time */
#define SDP_UDC_OTG_DVBUSPULSE		SDP_USBOTGREG(0x82C)		/* Device VBUS Pulsing Time */
//#define SDP_UDC_OTG_DTKNQR3			SDP_USBOTGREG(0x830)		/* Device IN Token Sequence Learning Queue Read 3 */
//#define SDP_UDC_OTG_DTKNQR4			SDP_USBOTGREG(0x834)		/* Device IN Token Sequence Learning Queue Read 4 */
#define SDP_UDC_OTG_DTHRCTL			SDP_USBOTGREG(0x830)
#define SDP_UDC_OTG_DIEPEMPMSK		SDP_USBOTGREG(0x834)


/*------------------------------------------------ */
/* Device Logical IN Endpoint-Specific Registers */
#define SDP_UDC_OTG_DIEPCTL(n)		SDP_USBOTGREG(0x900 + n*0x20)	/* Device IN Endpoint n Control */
#define SDP_UDC_OTG_DIEPINT(n)		SDP_USBOTGREG(0x908 + n*0x20)	/* Device IN Endpoint n Interrupt */
#define SDP_UDC_OTG_DIEPTSIZ(n)		SDP_USBOTGREG(0x910 + n*0x20)	/* Device IN Endpoint n Transfer Size */
#define SDP_UDC_OTG_DIEPDMA(n)		SDP_USBOTGREG(0x914 + n*0x20)	/* Device IN Endpoint n DMA Address */

/*------------------------------------------------ */
/* Device Logical OUT Endpoint-Specific Registers */
#define SDP_UDC_OTG_DOEPCTL(n)		SDP_USBOTGREG(0xB00 + n*0x20)	/* Device OUT Endpoint n Control */
#define SDP_UDC_OTG_DOEPINT(n)		SDP_USBOTGREG(0xB08 + n*0x20)	/* Device OUT Endpoint n Interrupt */
#define SDP_UDC_OTG_DOEPTSIZ(n)		SDP_USBOTGREG(0xB10 + n*0x20)	/* Device OUT Endpoint n Transfer Size */
#define SDP_UDC_OTG_DOEPDMA(n)		SDP_USBOTGREG(0xB14 + n*0x20)	/* Device OUT Endpoint n DMA Address */

/*------------------------------------------------ */
/* Endpoint FIFO address */
#define SDP_UDC_OTG_EP0_FIFO		SDP_USBOTGREG(0x1000)
#define SDP_UDC_OTG_EP1_FIFO		SDP_USBOTGREG(0x2000)
#define SDP_UDC_OTG_EP2_FIFO		SDP_USBOTGREG(0x3000)
#define SDP_UDC_OTG_EP3_FIFO		SDP_USBOTGREG(0x4000)
#define SDP_UDC_OTG_EP4_FIFO		SDP_USBOTGREG(0x5000)
#define SDP_UDC_OTG_EP5_FIFO		SDP_USBOTGREG(0x6000)
#define SDP_UDC_OTG_EP6_FIFO		SDP_USBOTGREG(0x7000)
#define SDP_UDC_OTG_EP7_FIFO		SDP_USBOTGREG(0x8000)
#define SDP_UDC_OTG_EP8_FIFO		SDP_USBOTGREG(0x9000)
#define SDP_UDC_OTG_EP9_FIFO		SDP_USBOTGREG(0xA000)
#define SDP_UDC_OTG_EP10_FIFO		SDP_USBOTGREG(0xB000)
#define SDP_UDC_OTG_EP11_FIFO		SDP_USBOTGREG(0xC000)
#define SDP_UDC_OTG_EP12_FIFO		SDP_USBOTGREG(0xD000)
#define SDP_UDC_OTG_EP13_FIFO		SDP_USBOTGREG(0xE000)
#define SDP_UDC_OTG_EP14_FIFO		SDP_USBOTGREG(0xF000)
#define SDP_UDC_OTG_EP15_FIFO		SDP_USBOTGREG(0x10000)

/*===================================================================== */
/*definitions related to CSR setting */

/* SDP_UDC_OTG_GOTGCTL */
#define B_SESSION_VALID			(0x1<<19)
#define A_SESSION_VALID			(0x1<<18)

/* SDP_UDC_OTG_GAHBCFG */
#define PTXFE_HALF			(0<<8)
#define PTXFE_ZERO			(1<<8)
#define NPTXFE_HALF			(0<<7)
#define NPTXFE_ZERO			(1<<7)
#define MODE_SLAVE			(0<<5)
#define MODE_DMA			(1<<5)
#define BURST_SINGLE			(0<<1)
#define BURST_INCR			(1<<1)
#define BURST_INCR4			(3<<1)
#define BURST_INCR8			(5<<1)
#define BURST_INCR16			(7<<1)
#define GBL_INT_UNMASK			(1<<0)
#define GBL_INT_MASK			(0<<0)

/* SDP_UDC_OTG_GRSTCTL */
#define AHB_MASTER_IDLE			(1u<<31)
#define CORE_SOFT_RESET			(0x1<<0)

/* SDP_UDC_OTG_GINTSTS/SDP_UDC_OTG_GINTMSK core interrupt register */
#define INT_RESUME				(1u<<31)
#define INT_DISCONN				(0x1<<29)
#define INT_CONN_ID_STS_CNG		(0x1<<28)
#define INT_OUT_EP				(0x1<<19)
#define INT_IN_EP				(0x1<<18)
#define INT_ENUMDONE			(0x1<<13)
#define INT_RESET				(0x1<<12)
#define INT_SUSPEND				(0x1<<11)
#define INT_EARLY_SUSPEND		(0x1<<10)
#define INT_NP_TX_FIFO_EMPTY	(0x1<<5)
#define INT_RX_FIFO_NOT_EMPTY	(0x1<<4)
#define INT_SOF					(0x1<<3)
#define INT_DEV_MODE			(0x0<<0)
#define INT_HOST_MODE			(0x1<<1)
#define INT_GOUTNakEff			(0x01<<7)
#define INT_GINNakEff			(0x01<<6)

#define FULL_SPEED_CONTROL_PKT_SIZE	8
#define FULL_SPEED_BULK_PKT_SIZE	64

#define HIGH_SPEED_CONTROL_PKT_SIZE	64
#define HIGH_SPEED_BULK_PKT_SIZE	512

#define RX_FIFO_SIZE				(768)
#define INEP0TX_FIFO_START_ADDR		RX_FIFO_SIZE
#define INEP0TX_FIFO_SIZE			(216)
#define PTX_FIFO_SIZE				(768)

/* Enumeration speed */
#define USB_HIGH_30_60MHZ		(0x0<<1)
#define USB_FULL_30_60MHZ		(0x1<<1)
#define USB_LOW_6MHZ			(0x2<<1)
#define USB_FULL_48MHZ			(0x3<<1)

/* SDP_UDC_OTG_GRXSTSP STATUS */
#define OUT_PKT_RECEIVED		(0x2<<17)
#define OUT_TRANSFER_COMPLELTED		(0x3<<17)
#define SETUP_TRANSACTION_COMPLETED	(0x4<<17)
#define SETUP_PKT_RECEIVED		(0x6<<17)
#define GLOBAL_OUT_NAK			(0x1<<17)

/* SDP_UDC_OTG_DCTL device control register */
#define NORMAL_OPERATION		(0x1<<0)
#define SOFT_DISCONNECT			(0x1<<1)
#define TEST_CONTROL_MASK		(0x7<<4)
#define TEST_J_MODE			(0x1<<4)
#define TEST_K_MODE			(0x2<<4)
#define TEST_SE0_NAK_MODE		(0x3<<4)
#define TEST_PACKET_MODE		(0x4<<4)
#define TEST_FORCE_ENABLE_MODE		(0x5<<4)

/* SDP_UDC_OTG_DAINT device all endpoint interrupt register */
#define DAINT_OUT_BIT			(16)
#define DAINT_MASK			(0xFFFF)

/* SDP_UDC_OTG_DIEPCTL0/DOEPCTL0 device control IN/OUT endpoint 0 control register */
#define DEPCTL_EPENA			(0x1<<31)
#define DEPCTL_EPDIS			(0x1<<30)
#define DEPCTL_SETD1PID			(0x1<<29)
#define DEPCTL_SETD0PID			(0x1<<28)
#define DEPCTL_SNAK				(0x1<<27)
#define DEPCTL_CNAK				(0x1<<26)
#define DEPCTL_STALL			(0x1<<21)
#define DEPCTL_TYPE_BIT			(18)
#define DEPCTL_TXFNUM_BIT		(22)
#define DEPCTL_TXFNUM_MASK		(0xF<<22)
#define DEPCTL_TYPE_MASK		(0x3<<18)
#define DEPCTL_CTRL_TYPE		(0x0<<18)
#define DEPCTL_ISO_TYPE			(0x1<<18)
#define DEPCTL_BULK_TYPE		(0x2<<18)
#define DEPCTL_INTR_TYPE		(0x3<<18)
#define DEPCTL_NAKSTS           (0x1<<17)
#define DEPCTL_USBACTEP			(0x1<<15)
#define DEPCTL_NEXT_EP_BIT		(11)
#define DEPCTL_MPS_BIT			(0)
#define DEPCTL_MPS_MASK			(0x7FF)

#define DEPCTL0_MPS_64			(0x0<<0)
#define DEPCTL0_MPS_32			(0x1<<0)
#define DEPCTL0_MPS_16			(0x2<<0)
#define DEPCTL0_MPS_8			(0x3<<0)
#define DEPCTL_MPS_BULK_512		(512<<0)
#define DEPCTL_MPS_INT_MPS_16		(16<<0)

#define DIEPCTL0_NEXT_EP_BIT		(11)

/* SDP_UDC_OTG_DIEPCTLn/DOEPCTLn device control IN/OUT endpoint n control register */

/* SDP_UDC_OTG_DIEPMSK/DOEPMSK device IN/OUT endpoint common interrupt mask register */
/* SDP_UDC_OTG_DIEPINTn/DOEPINTn device IN/OUT endpoint interrupt register */
#define BACK2BACK_SETUP_RECEIVED	(0x1<<6)
#define INTKNEPMIS			(0x1<<5)
#define INTKN_TXFEMP			(0x1<<4)
#define NON_ISO_IN_EP_TIMEOUT		(0x1<<3)
#define CTRL_OUT_EP_SETUP_PHASE_DONE	(0x1<<3)
#define AHB_ERROR			(0x1<<2)
#define EPDISBLD			(0x1<<1)
#define TRANSFER_DONE			(0x1<<0)

/*DIEPTSIZ0 / DOEPTSIZ0 */

/* DEPTSIZ common bit */
#define DEPTSIZ_PKT_CNT_BIT 		(19)
#define DEPTSIZ_XFER_SIZE_BIT		(0)

#define DEPTSIZ_SETUP_PKCNT_1		(1<<29)
#define DEPTSIZ_SETUP_PKCNT_2		(2<<29)
#define DEPTSIZ_SETUP_PKCNT_3		(3<<29)

#endif
