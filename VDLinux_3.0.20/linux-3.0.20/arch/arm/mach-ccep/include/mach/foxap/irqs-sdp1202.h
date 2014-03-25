/*
 * irqs-sdp1202.h
 *
 * Copyright (c) 2012 Samsung Electronics
 * Seungjun Heo <seungjun.heo@samsung.com>
 *
 */

#ifndef _SDP1202_IRQS_H_
#define _SDP1202_IRQS_H_

#define MAX_GIC_NR	1

#define GIC_PPI(x)	((x) + 16)
#ifdef CONFIG_ARM_TRUSTZONE
#define IRQ_LOCALTIMER	GIC_PPI(13)		//irq 29 : non-secure timer(ES0)
#else
#define IRQ_LOCALTIMER	GIC_PPI(14)		//irq 30 : secure timer(ES0)
#endif

#define GIC_SPI(x)	((x) + 32)

#define NR_IRQS			256

#define IRQ_TSD		GIC_SPI(0)
#define IRQ_AIO		GIC_SPI(1)
#define IRQ_AE		GIC_SPI(2)
#define IRQ_JPEG	GIC_SPI(3)
#define IRQ_DISP	GIC_SPI(4)
#define IRQ_GA2D_MP	GIC_SPI(5)
#define IRQ_MFD		GIC_SPI(6)
#define IRQ_AVD		GIC_SPI(7)
#define IRQ_FROAD_MP	GIC_SPI(8)
#define IRQ_SE_MP	GIC_SPI(9)
#define IRQ_HENC	GIC_SPI(10)
#define IRQ_HDMIRX_4P	GIC_SPI(11)
#define IRQ_GA2D	GIC_SPI(20)
#define IRQ_GA2D_MMU	GIC_SPI(21)
#define IRQ_GA2D_PPMMU	GIC_SPI(22)
#define IRQ_GPU		GIC_SPI(23)
#define IRQ_GPU_MMU	GIC_SPI(24)
#define IRQ_GPU_JOB	GIC_SPI(25)
#define IRQ_GFX_CAP	GIC_SPI(26)
#define IRQ_OSDP	GIC_SPI(27)
#define IRQ_GZIP	GIC_SPI(28)
#define IRQ_SHDSP1	GIC_SPI(29)
#define IRQ_SHDSP2	GIC_SPI(30)
#define IRQ_FROAD	GIC_SPI(31)
#define IRQ_SE		GIC_SPI(32)
#define IRQ_DMA330	GIC_SPI(33)
#define IRQ_USB3	GIC_SPI(34)
#define IRQ_USB_OTG	GIC_SPI(35)
#define IRQ_USB0_VBUS	GIC_SPI(36)
#define IRQ_USB1_VBUS	GIC_SPI(37)
#define IRQ_USB_EHCI0	GIC_SPI(38)
#define IRQ_USB_EHCI1	GIC_SPI(39)
#define IRQ_USB_OHCI0	GIC_SPI(40)
#define IRQ_USB_OHCI1	GIC_SPI(41)
#define IRQ_EMAC	GIC_SPI(42)
#define IRQ_UART0	GIC_SPI(43)
#define IRQ_UART1	GIC_SPI(44)
#define IRQ_UART2	GIC_SPI(45)
#define IRQ_UART3	GIC_SPI(46)
#define IRQ_TIMER	GIC_SPI(47)
#define IRQ_TIMER0	GIC_SPI(47)
#define IRQ_TIMER1	GIC_SPI(48)
#define IRQ_TIMER2	GIC_SPI(49)
#define IRQ_TIMER3	GIC_SPI(50)
#define IRQ_IIC0	GIC_SPI(51)
#define IRQ_IIC1	GIC_SPI(52)
#define IRQ_IIC2	GIC_SPI(53)
#define IRQ_IIC3	GIC_SPI(54)
#define IRQ_IIC4	GIC_SPI(55)
#define IRQ_IIC5	GIC_SPI(56)
#define IRQ_IIC6	GIC_SPI(57)
#define IRQ_IIC7	GIC_SPI(58)
#define IRQ_SCI		GIC_SPI(59)
#define IRQ_MMCIF	GIC_SPI(60)
#define IRQ_IRR		GIC_SPI(61)
#define IRQ_RTC		GIC_SPI(62)
#define IRQ_IRB		GIC_SPI(63)
#define IRQ_SMCDMA	GIC_SPI(64)
#define IRQ_SPI		GIC_SPI(65)
#define IRQ_EXT0	GIC_SPI(66)
#define IRQ_EXT1	GIC_SPI(67)	
#define IRQ_EXT2	GIC_SPI(68)
#define IRQ_EXT3	GIC_SPI(69)
#define IRQ_EXT4	GIC_SPI(70)
#define IRQ_EXT5	GIC_SPI(71)
#define IRQ_EXT6	GIC_SPI(72)
#define IRQ_EXT7	GIC_SPI(73)
#define IRQ_CSSYS_CPU0	GIC_SPI(76)
#define IRQ_CSSYS_CPU1	GIC_SPI(77)
#define IRQ_CSSYS_CPU2	GIC_SPI(78)
#define IRQ_CSSYS_CPU3	GIC_SPI(79)
#define IRQ_PMU_CPU0	GIC_SPI(80)
#define IRQ_PMU_CPU1	GIC_SPI(81)
#define IRQ_PMU_CPU2	GIC_SPI(82)
#define IRQ_PMU_CPU3	GIC_SPI(83)
#define IRQ_AXIERR	GIC_SPI(84)
#define IRQ_INTERR	GIC_SPI(85)


#endif

