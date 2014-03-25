/***************************************************************************
 *
 *	arch/arm/plat-sdp/sdpGmac/sdpGmac.h
 *	Samsung Elecotronics.Co
 *	Created by tukho.kim
 *
 * ************************************************************************/
/*
 * 2009.08.02: Created by tukho.kim@samsung.com
 * 2009.10.22,tukho.kim: debug when rx buffer alloc is failed 0.940
 * 2009.12.01,tukho.kim: debug phy check media, 0.951
 *                       full <-> half ok, 10 -> 100 ok, 100 -> 10 need to unplug and plug cable
 * 2012.04.23,drain.lee: add NAPI struct.
 * 2012.05.14,drain.lee: move chip dependent code. 0.969
 * 2012.07.23,drain.lee: add flow ctrl value 0.973
 * 2012.10.05,drain.lee: change debug print format v0.979
 * 2012.12.21,drain.lee: fix compile warning v0.985
 */


#ifndef __SDP_GMAC_H

/* define debug */

#ifdef CONFIG_SDP_MAC_DEBUG
#define DPRINTK_GMAC_DBG(fmt,args...)	printk(KERN_DEBUG "["ETHER_NAME" DBG %s] " fmt,__FUNCTION__,##args)
#define DPRINTK_GMAC_FLOW(fmt,args...)	printk(KERN_DEBUG "["ETHER_NAME" FLOW %s] " fmt,__FUNCTION__,##args)
#define DPRINTK_GMAC(fmt,args...)		printk(KERN_DEBUG "["ETHER_NAME" %s] " fmt,__FUNCTION__,##args)
#else
#define DPRINTK_GMAC_DBG(fmt,args...)	do {} while(0)
#define DPRINTK_GMAC_FLOW(fmt,args...)	do {} while(0)
#define DPRINTK_GMAC(fmt,args...)		do {} while(0)
#endif


#define DPRINTK_GMAC_ERROR(fmt,args...)	printk(KERN_ERR "["ETHER_NAME" %s] " fmt,__FUNCTION__,##args)
#define PRINTK_GMAC(fmt,args...)		printk(KERN_INFO "["ETHER_NAME"] " fmt,##args)

#define SDP_GMAC_OK 	0
#define SDP_GMAC_ERR 	1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/crc32.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/skbuff.h>
#include <linux/dma-mapping.h>

/* Ethernet header */
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

/* Io Header */
#include <asm/uaccess.h>
#include <asm/io.h>

#ifdef CONFIG_MAC_GET_I2C
#include <asm/plat-sdp/sdp_i2c_io.h>
#endif

#include <plat/sdp_gmac_reg.h>	// 0.952

//#define ETHER_NAME		"SDP_GMAC" // 0.952 move to sdpGmac_reg.h

#define N_MAC_ADDR	 	ETH_ALEN
#define DEFAULT_MAC_ADDR 	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}

#define STATUS_SPEED_10			(10)
#define STATUS_SPEED_100		(100)
#define STATUS_SPEED_1000		(1000)

#define STATUS_DUPLEX_HALF		(0)		// must be same value, mii driver
#define STATUS_DUPLEX_FULL		(1)		// must be same value, mii driver

#define STATUS_UNLINK			(0x00)
#define STATUS_LINK			(0x01)

#define CFG_LINK_CHECK_TIME		(HZ) 		// 1sec, 500ms Time

#if 0/* 120514 commented 0.969*/
#if defined(CONFIG_ARCH_SDP1004)
# define SDP_GMAC_PAD_INIT \
 	do { u32 val = *(volatile u32 *)(VA_IO_BASE0+0x00090890); \
		val |= (1 << 2); /* clock inverting */ \
 		*(volatile u32 *)(VA_IO_BASE0+0x00090890) = val; \
\
		val  = *(volatile u32 *)(VA_IO_BASE0+0x00090880); \
	    val &= ~(7 << 28); \
	    *(volatile u32 *)(VA_IO_BASE0+0x00090880) = val | (4 << 28); \
\
	    *(volatile u32 *)(VA_IO_BASE0+0x000D0008) = 1; \
\
	    val = *(volatile u32 *)(VA_IO_BASE0+0x000D0014); \
	    *(volatile u32 *)(VA_IO_BASE0+0x000D0014) = val & ~(1 << 16); \
	} while(0)
#elif defined(CONFIG_ARCH_SDP1103)
# define SDP_GMAC_PAD_INIT \
 	do { *(volatile u32 *)(VA_IO_BASE0+0x00090CDC) = 1; }while(0)
#elif defined(CONFIG_ARCH_SDP1105)
# define SDP_GMAC_PAD_INIT \
 	do { *(volatile u32 *)(VA_IO_BASE0+0x000D0008) = 1; }while(0)
#elif defined(CONFIG_ARCH_SDP1106)
# define SDP_GMAC_PAD_INIT \
 	do { *(volatile u32 *)(VA_IO_BASE0+0x00090CDC) = 1; }while(0)
#elif defined(CONFIG_ARCH_SDP1207)
# define SDP_GMAC_PAD_INIT \
 	do { *(volatile u32 *)(VA_IO_BASE0+0x000D0008) = 1; }while(0)
#else
# define SDP_GMAC_PAD_INIT
#endif
#endif/* 120514 commented 0.969*/


typedef struct {
	u32 dma_busMode;
	u32 dma_operationMode;
	u32 gmac_configuration;
	u32 gmac_frameFilter;
	u32 gmac_macAddr[32];
}SDP_GMAC_POWER_T;


typedef struct {
/* network resouce */
	struct net_device 	 	*pNetDev;
	struct net_device_stats		netStats;
	struct napi_struct		napi;

/* OS */
	/* mutex resource  */
	spinlock_t			lock;

/* Operation Resource*/
	int				hwDuplex;
	int 			hwSpeed;
	u32				msg_enable;

/* DMA resource - non-cached resource */
	struct device			*pDev;
	dma_addr_t			txDescDma;	// physical address
	dma_addr_t			rxDescDma;

	DMA_DESC_T			*pTxDesc;	// logical address
	DMA_DESC_T			*pRxDesc;

	DMA_DESC_T			*pTxDescBusy;
	DMA_DESC_T			*pRxDescBusy;
	DMA_DESC_T			*pTxDescNext;
	DMA_DESC_T			*pRxDescNext;

	struct	sk_buff			*pRxDummySkb;	// 0.940
//	struct	sk_buff			*currentRxSkb;
//	struct	sk_buff			*currentTxSkb;

//	u32				numTxDesc;
//	u32				numRxDesc;

	u32				txBusy;
	u32				rxBusy;

	u32				txNext;
	u32				rxNext;

/* Power Management resource */
	SDP_GMAC_POWER_T		power;

/* register resource */
	SDP_GMAC_T		 	*pGmacBase;
	SDP_GMAC_MMC_T		 	*pMmcBase;
	SDP_GMAC_TIME_STAMP_T    	*pTimeStampBase;
	SDP_GMAC_MAC_2ND_BLOCK_T 	*pMac2ndBlk;
	SDP_GMAC_DMA_T 		 	*pDmaBase;

#ifdef CONFIG_ARCH_SDP1004
	int				revision;
#endif

	struct mii_bus *mdiobus;
	struct phy_device *phydev;
	int has_gmac;
	int oldlink;
	int is_rx_stop;

	enum sdp_mac_flow_ctrl {
		SDP_MAC_FLOW_OFF = 0,
		SDP_MAC_FLOW_RX,
		SDP_MAC_FLOW_TX,
		SDP_MAC_FLOW_RXTX,
	} flow_ctrl;

	int pause_time;

	struct sdp_gmac_plat *plat;

}SDP_GMAC_DEV_T;

#endif /*  __SDP_GMAC_H */
