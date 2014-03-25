/***************************************************************************
 *
 *	arch/arm/mach-ccep/include/plat/sdp_gmac_reg.h
 *	Samsung Elecotronics.Co
 *	Created by tukho.kim	
 *
 * ************************************************************************/
/*
 * 2009.08.01: Created by tukho.kim@samsung.com
 * 2009.11.21,tukho.kim: modify netDev_rx routine 0.9500
 * 2009.12.02,tukho.kim: debug module init, exit  0.952
 */ 


#ifndef __SDP_GMAC_REG_H
#define __SDP_GMAC_REG_H	// 0.952

#include <mach/irqs.h>

// 0.952
#if defined(CONFIG_ARCH_SDP1001)
#  define PA_SDP_GMAC_BASE 	(0x30050000UL)
#  define PA_SDP_GMAC_MMC_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_MMC_BASE)
#  define PA_SDP_GMAC_TIME_STAMP_BASE     (PA_SDP_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE)
#  define PA_SDP_GMAC_MAC_2ND_BLOCK_BASE  (PA_SDP_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE)
#  define PA_SDP_GMAC_DMA_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_DMA_BASE)
#  define IRQ_SDP_GMAC		(15)
#  define GMAC_PHY_ID		(3) 	/* H/W Fixed */
#  define SDP_GMAC_BUS		32

#elif defined(CONFIG_ARCH_SDP1002)
#  define PA_SDP_GMAC_BASE 	(0x30050000UL)
#  define PA_SDP_GMAC_MMC_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_MMC_BASE)
#  define PA_SDP_GMAC_TIME_STAMP_BASE     (PA_SDP_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE)
#  define PA_SDP_GMAC_MAC_2ND_BLOCK_BASE  (PA_SDP_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE)
#  define PA_SDP_GMAC_DMA_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_DMA_BASE)
#  define IRQ_SDP_GMAC		(15)
#  define GMAC_PHY_ID		(3) 	/* H/W Fixed */
#  define SDP_GMAC_BUS		32

#elif defined(CONFIG_ARCH_SDP1004)
#  define PA_SDP_GMAC_BASE 	(0x300A0000UL)
#  define PA_SDP_GMAC_MMC_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_MMC_BASE)
#  define PA_SDP_GMAC_TIME_STAMP_BASE     (PA_SDP_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE)
#  define PA_SDP_GMAC_MAC_2ND_BLOCK_BASE  (PA_SDP_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE)
#  define PA_SDP_GMAC_DMA_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_DMA_BASE)
#  define IRQ_SDP_GMAC		(16)
#  define GMAC_PHY_ID		(3) 	/* H/W Fixed */
#  define SDP_GMAC_BUS		32

#elif defined(CONFIG_ARCH_SDP1103)
#  define PA_SDP_GMAC_BASE 	(0x30050000UL)
#  define PA_SDP_GMAC_MMC_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_MMC_BASE)
#  define PA_SDP_GMAC_TIME_STAMP_BASE     (PA_SDP_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE)
#  define PA_SDP_GMAC_MAC_2ND_BLOCK_BASE  (PA_SDP_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE)
#  define PA_SDP_GMAC_DMA_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_DMA_BASE)
#  define IRQ_SDP_GMAC		(15)
#  if defined(CONFIG_VDLP_VERSION_INFO)
#    define GMAC_PHY_ID		(3) 	/* H/W Fixed */
#  else
#    define GMAC_PHY_ID		(0) 	/* H/W Fixed */
#  endif
#  define SDP_GMAC_BUS		128

#elif defined(CONFIG_ARCH_SDP1105)
#  define PA_SDP_GMAC_BASE 	(0x300A0000UL)
#  define PA_SDP_GMAC_MMC_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_MMC_BASE)
#  define PA_SDP_GMAC_TIME_STAMP_BASE     (PA_SDP_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE)
#  define PA_SDP_GMAC_MAC_2ND_BLOCK_BASE  (PA_SDP_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE)
#  define PA_SDP_GMAC_DMA_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_DMA_BASE)
#  define IRQ_SDP_GMAC		(IRQ_GMAC)
#  define GMAC_PHY_ID		(3) 	/* H/W Fixed */
#  define SDP_GMAC_BUS		32	/* !!!! must check connection status of Gmac block */

#elif defined(CONFIG_ARCH_SDP1106)
#  define PA_SDP_GMAC_BASE 	(0x30050000UL)
#  define PA_SDP_GMAC_MMC_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_MMC_BASE)
#  define PA_SDP_GMAC_TIME_STAMP_BASE     (PA_SDP_GMAC_BASE + SDP_GMAC_TIME_STAMP_BASE)
#  define PA_SDP_GMAC_MAC_2ND_BLOCK_BASE  (PA_SDP_GMAC_BASE + SDP_GMAC_MAC_2ND_BLOCK_BASE)
#  define PA_SDP_GMAC_DMA_BASE            (PA_SDP_GMAC_BASE + SDP_GMAC_DMA_BASE)
#  define IRQ_SDP_GMAC		(IRQ_GMAC)
#  if defined(CONFIG_VDLP_VERSION_INFO)
#    define GMAC_PHY_ID		(3) 	/* H/W Fixed */
#  else
#    define GMAC_PHY_ID		(0) 	/* H/W Fixed */
#  endif
#  define SDP_GMAC_BUS		128

#else
#  error
#endif
// 0.952 end

#define ETHER_NAME		"SDP_GMAC" // 0.952


#define N_REG_BASE			5

#define SDP_GMAC_BASE  			(0x0000)
#define SDP_GMAC_MMC_BASE		(0x0100)
#define SDP_GMAC_TIME_STAMP_BASE  	(0x0700)
#define SDP_GMAC_MAC_2ND_BLOCK_BASE	(0x0800)
#define SDP_GMAC_DMA_BASE 		(0x1000)

#define GMAC_USER_VER			(0x10)
#define GMAC_SYNOP_VER			(0x34)


/* Define for Ethernet */

// 6 byte Dest addr, 6 byte Src addr, 2 byte length/type
#define ETHER_HEADER			(14)
#define ETHER_CRC			(4)  // Ethernet CRC
#define ETHER_EXTRA			(2)  // ????
// Maximum length when received data is copied on to a new skb
#define ETHER_PACKET_COPY		(250)
// Preallocated length for the rx packets is MTU + ETHERNET_PACKET_EXTRA
#define ETHER_PACKET_EXTRA		(18)

#define VLAN_TAG			(4)  	//optional 802.1q VLAN Tag
#define MIN_ETHER_PAYLOAD		(46)  	//Minimum Ethernet payload size
#define MAX_ETHER_PAYLOAD		(1500)  //Maximum Ethernet payload size
#define JUMBO_FRAME_PAYLOAD		(9000)  //Jumbo frame payload size

#define TX_BUF_SIZE (ETHER_HEADER + ETHER_CRC + MAX_ETHER_PAYLOAD + VLAN_TAG)


/*========================================*/
/*           Define register              */
/*========================================*/
#include <linux/types.h>

typedef volatile struct {
	volatile u32 configuration;		/* 0x0000 */
	volatile u32 frameFilter;		/* 0x0004 */
	volatile u32 hashTableHigh;		/* 0x0008 */
	volatile u32 hashTableLow;		/* 0x000C */
	volatile u32 gmiiAddr;			/* 0x0010 */
	volatile u32 gmiiData;			/* 0x0014 */
	volatile u32 flowCtrl;			/* 0x0018 */
	volatile u32 vlanTag;			/* 0x001C */
	volatile u32 version;			/* 0x0020 */
	volatile u32 reserved0;			/* 0x0024 */
	volatile u32 remoteWakeUpFrameFilter;	/* 0x0028 */
	volatile u32 pmtCtrlStatus;		/* 0x002C */
	volatile u32 reserved1[2];		/* 0x0030,0x0034 */
	volatile u32 interrupt;			/* 0x0038 */
	volatile u32 intrMask;			/* 0x003C */
	volatile u32 macAddr_00_High;		/* 0x0040 */
	volatile u32 macAddr_00_Low;		/* 0x0044 */
	volatile u32 macAddr_01_High;		/* 0x0048 */
	volatile u32 macAddr_01_Low;		/* 0x004C */
	volatile u32 macAddr_02_High;	   	/* 0x0050 */
	volatile u32 macAddr_02_Low;		/* 0x0054 */
	volatile u32 macAddr_03_High;   	/* 0x0058 */
	volatile u32 macAddr_03_Low;		/* 0x005C */
	volatile u32 macAddr_04_High;	   	/* 0x0060 */
	volatile u32 macAddr_04_Low;		/* 0x0064 */
	volatile u32 macAddr_05_High;      	/* 0x0068 */
	volatile u32 macAddr_05_Low;		/* 0x006C */
	volatile u32 macAddr_06_High;	   	/* 0x0070 */
	volatile u32 macAddr_06_Low;		/* 0x0074 */
	volatile u32 macAddr_07_High;   	/* 0x0078 */
	volatile u32 macAddr_07_Low;		/* 0x007C */
	volatile u32 macAddr_08_High;	   	/* 0x0080 */
	volatile u32 macAddr_08_Low;		/* 0x0084 */
	volatile u32 macAddr_09_High;      	/* 0x0088 */
	volatile u32 macAddr_09_Low;		/* 0x008C */
	volatile u32 macAddr_10_High;		/* 0x0090 */
	volatile u32 macAddr_10_Low;		/* 0x0094 */
	volatile u32 macAddr_11_High;		/* 0x0098 */
	volatile u32 macAddr_11_Low;		/* 0x009C */
	volatile u32 macAddr_12_High;	   	/* 0x00A0 */
	volatile u32 macAddr_12_Low;		/* 0x00A4 */
	volatile u32 macAddr_13_High;   	/* 0x00A8 */
	volatile u32 macAddr_13_Low;		/* 0x00AC */
	volatile u32 macAddr_14_High;	   	/* 0x00B0 */
	volatile u32 macAddr_14_Low;		/* 0x00B4 */
	volatile u32 macAddr_15_High;      	/* 0x00B8 */
	volatile u32 macAddr_15_Low;		/* 0x00BC */
//	volatile u32 anCtrl;			/* 0x00C0 */ 
//	volatile u32 anStatus;			/* 0x00C4 */
//	volatile u32 autoNegoAdvr;		/* 0x00C8 */
//	volatile u32 autoNegoLinkPartner;	/* 0x00CC */
//	volatile u32 autoNegoExpan;		/* 0x00D0 */
//	volatile u32 tbiExtStatus;		/* 0x00D4 */
//	volatile u32 sgmiiRgmiiStatus;		/* 0x00D8 */
}SDP_GMAC_T;

// Mac Management Counter MMC
typedef volatile struct {
	volatile u32 control;			/* 0x0100 */
	volatile u32 intrRx;			/* 0x0104 */
	volatile u32 intrTx;			/* 0x0108 */
	volatile u32 intrMaskRx;		/* 0x010C */
	volatile u32 intrMaskTx;		/* 0x0110 */
	volatile u32 txOctetCountGb;		/* 0x0114 */
	volatile u32 txFrameCountGb;		/* 0x0118 */
	volatile u32 txBcFramesG;		/* 0x011C */
	volatile u32 txMcFramesG;		/* 0x0120 */
	volatile u32 tx64OctetsGb;		/* 0x0124 */
	volatile u32 tx65To127OctetsGb;		/* 0x0128 */
	volatile u32 tx128To255OctetsGb;	/* 0x012C */
	volatile u32 tx256To511OctetsGb;	/* 0x0130 */
	volatile u32 tx512To1023OctetsGb;	/* 0x0134 */
	volatile u32 tx1024ToMaxOctetsGb;	/* 0x0138 */
	volatile u32 txUcFramesGb;		/* 0x013C */ // Unicast Frames Tx
	volatile u32 txMcFramesGb;		/* 0x0140 */ // Multicast Frames Tx
	volatile u32 txBcFramesGb;		/* 0x0144 */ // Broadcast Frames Tx
	volatile u32 txUnderFlowError;		/* 0x0148 */ 
	volatile u32 txSingleColG;		/* 0x014C */ 
	volatile u32 txMultiColG;		/* 0x0150 */ 
	volatile u32 txDeferred;		/* 0x0154 */ 
	volatile u32 txLateCol;			/* 0x0158 */ 
	volatile u32 txExcessCol;		/* 0x015C */ 
	volatile u32 txCarrierErr;		/* 0x0160 */ 
	volatile u32 txOctetCountG;		/* 0x0164 */ 
	volatile u32 txFrameCountG;		/* 0x0168 */ 
	volatile u32 txExcessDef;		/* 0x016C */ 
	volatile u32 txPauseFrames;		/* 0x0170 */ 
	volatile u32 txVlanFrames;		/* 0x0174 */ 
	volatile u32 reserved0[2];		/* 0x0178 ~ 0x017C */
	volatile u32 rxFrameCountGB;		/* 0x0180 */
	volatile u32 rxOctetCountGB;		/* 0x0184 */
	volatile u32 rxOctetCountG;		/* 0x0188 */
	volatile u32 rxBCastFramesG;		/* 0x018C */
	volatile u32 rxMCastFramesG;		/* 0x0190 */
	volatile u32 rxCrcError;		/* 0x0194 */
	volatile u32 rxAlignError;		/* 0x0198 */
	volatile u32 rxRuntError;		/* 0x019C */
	volatile u32 rxJabberError;		/* 0x01A0 */
	volatile u32 rxUnderSizeG;		/* 0x01A4 */
	volatile u32 rxOverSizeG;		/* 0x01A8 */
	volatile u32 rx64octetsGB;		/* 0x01AC */
	volatile u32 rx65to127octetsGB;		/* 0x01B0 */
	volatile u32 rx128to255octetsGB;	/* 0x01B4 */
	volatile u32 rx256to511octetsGB;	/* 0x01B8 */
	volatile u32 rx512to1023octetsGB;	/* 0x01BC */
	volatile u32 rx1024toMaxoctetsGB;	/* 0x01C0 */
	volatile u32 rxUniFramesG;		/* 0x01C4 */
	volatile u32 rxLengthError;		/* 0x01C8 */
	volatile u32 rxOutOfRangeType;		/* 0x01CC */
	volatile u32 rxPauseFrames;		/* 0x01D0 */
	volatile u32 rxFifoOverFlow;		/* 0x01D4 */
	volatile u32 rxVlanFramesGB;		/* 0x01D8 */
	volatile u32 rxWatchDogError;		/* 0x01DC */
	volatile u32 reserved1[8];		/* 0x01E0 ~ 1FC */
	volatile u32 mmcIpcIntrMaskRx;		/* 0x0200 */
	volatile u32 reserved2;			/* 0x0204 */
	volatile u32 mmcIpcIntrRx;		/* 0x0208 */
	volatile u32 reserved3;			/* 0x020C */
	volatile u32 rxIpv4GdFrms;		/* 0x0210 */
	volatile u32 rxIpv4HdrErrFrms;		/* 0x0214 */
	volatile u32 rxIpv4NopayFrms;		/* 0x0218 */
	volatile u32 rxIpv4FragFrms;		/* 0x021C */
	volatile u32 rxIpv4UdsblFrms;		/* 0x0220 */
	volatile u32 rxIpv6GdFrms;		/* 0x0224 */
	volatile u32 rxIpv6HdrErrFrms;		/* 0x0228 */
	volatile u32 rxIpv6NopayFrms;		/* 0x022C */
	volatile u32 rxUdpGdFrms;		/* 0x0230 */
	volatile u32 rxUdpErrFrms;		/* 0x0234 */
	volatile u32 rxTcpGdFrms;		/* 0x0238 */
	volatile u32 rxTcpErrFrms;		/* 0x023C */
	volatile u32 rxIcmpGdFrms;		/* 0x0240 */
	volatile u32 rxIcmpErrFrms;		/* 0x0244 */
	volatile u32 reserved4[2];		/* 0x0248 ~ 24C */
	volatile u32 rxIpv4GdOctets;		/* 0x0250 */
	volatile u32 rxIpv4HdrErrOctets;	/* 0x0254 */
	volatile u32 rxIpv4NopayOctets;		/* 0x0258 */
	volatile u32 rxIpv4FragOctets;		/* 0x025C */
	volatile u32 rxIpv4UsdsblOctets;	/* 0x0260 */
	volatile u32 rxIpv6GdOctets;		/* 0x0264 */
	volatile u32 rxIpv6HdrErrOctets;	/* 0x0268 */
	volatile u32 rxIpv6NopayOctets;		/* 0x026C */
	volatile u32 rxUdpGdOctets;		/* 0x0270 */
	volatile u32 rxUdpErrOctets;		/* 0x0274 */
	volatile u32 rxTcpGdOctets;		/* 0x0278 */
	volatile u32 rxTcpErrOctets;		/* 0x027C */
	volatile u32 rxIcmpGdOctets;		/* 0x0280 */
	volatile u32 rxIcmpErrOctets;		/* 0x0284 */
}SDP_GMAC_MMC_T;


typedef volatile struct {
	volatile u32 control;			/* 0x0700 */
	volatile u32 sub2ndInc;			/* 0x0704 */
	volatile u32 high;			/* 0x0708 */
	volatile u32 low;			/* 0x070C */
	volatile u32 updateHigh;		/* 0x0710 */
	volatile u32 updateLow;			/* 0x0714 */
	volatile u32 addend;			/* 0x0718 */
	volatile u32 targetTimeHigh;		/* 0x071C */
	volatile u32 targetTimeLow;		/* 0x0720 */
}SDP_GMAC_TIME_STAMP_T;

typedef volatile struct {
	volatile u32 macAddr_16_High;		/* 0x0800 */
	volatile u32 macAddr_16_Low;		/* 0x0804 */
	volatile u32 macAddr_17_High;		/* 0x0808 */
	volatile u32 macAddr_17_Low;		/* 0x080C */
	volatile u32 macAddr_18_High;	   	/* 0x0810 */
	volatile u32 macAddr_18_Low;		/* 0x0814 */
	volatile u32 macAddr_19_High;   	/* 0x0818 */
	volatile u32 macAddr_19_Low;		/* 0x081C */
	volatile u32 macAddr_20_High;	   	/* 0x0820 */
	volatile u32 macAddr_20_Low;		/* 0x0824 */
	volatile u32 macAddr_21_High;      	/* 0x0828 */
	volatile u32 macAddr_21_Low;		/* 0x082C */
	volatile u32 macAddr_22_High;	   	/* 0x0830 */
	volatile u32 macAddr_22_Low;		/* 0x0834 */
	volatile u32 macAddr_23_High;   	/* 0x0838 */
	volatile u32 macAddr_23_Low;		/* 0x083C */
	volatile u32 macAddr_24_High;	   	/* 0x0840 */
	volatile u32 macAddr_24_Low;		/* 0x0844 */
	volatile u32 macAddr_25_High;      	/* 0x0848 */
	volatile u32 macAddr_25_Low;		/* 0x084C */
	volatile u32 macAddr_26_High;		/* 0x0850 */
	volatile u32 macAddr_26_Low;		/* 0x0854 */
	volatile u32 macAddr_27_High;		/* 0x0858 */
	volatile u32 macAddr_27_Low;		/* 0x085C */
	volatile u32 macAddr_28_High;	   	/* 0x0860 */
	volatile u32 macAddr_28_Low;		/* 0x0864 */
	volatile u32 macAddr_29_High;   	/* 0x0868 */
	volatile u32 macAddr_29_Low;		/* 0x086C */
	volatile u32 macAddr_30_High;	   	/* 0x0870 */
	volatile u32 macAddr_30_Low;		/* 0x0874 */
	volatile u32 macAddr_31_High;      	/* 0x0878 */
	volatile u32 macAddr_31_Low;		/* 0x087C */
}SDP_GMAC_MAC_2ND_BLOCK_T;

typedef volatile struct {
	volatile u32 busMode;			/* 0x1000 */
	volatile u32 txPollDemand;		/* 0x1004 */
	volatile u32 rxPollDemand;		/* 0x1008 */
	volatile u32 rxDescListAddr;		/* 0x100C */
	volatile u32 txDescListAddr;		/* 0x1010 */
	volatile u32 status;			/* 0x1014 */
	volatile u32 operationMode;		/* 0x1018 */
	volatile u32 intrEnable;		/* 0x101C */
	volatile u32 missFrameBufOverCnt;	/* 0x1020 */
	volatile u32 reserved0[9];		/* 0x1024 ~ 0x1044 */
	volatile u32 curHostTxDesc;		/* 0x1048 */
	volatile u32 curHostRxDesc;		/* 0x104C */
	volatile u32 curHostTxBufAddr;		/* 0x1050 */
	volatile u32 curHostRxBufAddr;		/* 0x1054 */
}SDP_GMAC_DMA_T;



/* SDP_GMAC_T Bit Define:  	*/
// configuration 	0x0000
#define B_TX_CONF_RGMII			(1 << 24)
#define B_WATCHDOG_DISABLE		(1 << 23)
#define B_JABBER_DISABLE		(1 << 22)
#define B_FRAME_BURST_EN		(1 << 21)
#define B_JUMBO_FRAME_EN		(1 << 20)
#define B_INTER_FRAME_GAP(a)		((((96 - a) >> 3) & 7) << 17) // [19:17]
#define B_TX_CARRIER_SENSE_DIS		(1 << 16)	// Disable carrier sense during tx
#define B_PORT_MII			(1 << 15)	// 100MBps is RO -> 1
#define B_SPEED_100M			(1 << 14) 
#define B_RX_OWN_DIS			(1 << 13)
#define B_LOOP_BACK_EN			(1 << 12)
#define B_DUPLEX_FULL			(1 << 11)
#define B_CHECKSUM_OFFLOAD		(1 << 10)
#define B_RETRY_DISABLE			(1 << 9)
#define B_LINK_UP			(1 << 8)
#define B_AUTO_CRC_STRIP		(1 << 7)
#define B_BACKOFF_LIMIT(a)		((a & 3) << 5)
#define B_DEFERRAL_CHECK		(1 << 4)
#define B_TX_ENABLE			(1 << 3)
#define B_RX_ENABLE			(1 << 2)

// frame filter		0x0004
#define	B_RX_ALL			(1 << 31)
#define B_HASH_PERFECT_FILTER		(1 << 10)
#define B_SRC_ADDR_FILT_EN		(1 << 9)
#define B_SA_INVERSE_FILTER		(1 << 8)
#define B_PASS_CTRL_FRAMES(a)		((a & 3) << 6)
#define B_BROADCAST_DIS			(1 << 5)
#define B_PASS_ALL_MULTICAST		(1 << 4)
#define B_DA_INVERSE_FILTER		(1 << 3)
#define B_HASH_MULTICAST		(1 << 2)
#define B_HASH_UNICAST			(1 << 1)
#define B_PROMISCUOUS_EN		(1)


// gmiiAddr, gmiiData 				/* 0x0010 */
#define B_GMII_ADDR_BUSY		(1)
#define B_GMII_WRITE			(1 << 1)
#define GMII_ADDR(phy, reg) 		(phy << 11) | (reg << 6) 
#define GMII_ADDR_READ(phy, reg) 	(GMII_ADDR(phy, reg) | B_GMII_ADDR_BUSY)
#define GMII_ADDR_WRITE(phy, reg) 	(GMII_ADDR_READ(phy, reg) | B_GMII_WRITE)


/* DEFINE BIT of MMC register SDP_GMAC_MMC_T */
// control 	0x100
#define B_COUNTER_FREEZE		(1 << 3)
#define B_RESET_ON_READ			(1 << 2)
#define B_COUNTER_STOP_ROLLOVER		(1 << 1)
#define B_COUNTERS_RESET		(1)

// rx Intr
#define B_RX_WATCHDOG_ERR		(1 << 23)
#define B_RX_VAL_FRAMES_GB		(1 << 22)
#define B_RX_FIFO_OVERFLOW		(1 << 21)
#define B_RX_PAUSE_FRAMES		(1 << 20)
#define B_RX_OUT_OF_RANGE_TYPE		(1 << 19)
#define B_RX_LENGTH_ERROR		(1 << 18)
#define B_RX_UNICAST_FRAMES_GB		(1 << 17)
#define B_RX_1024_TO_MAX_OCTETS_GB	(1 << 16)


/* DEFINE BIT of DMA register SDP_GMAC_DMA_T */
// busMode 0x1000 
#define B_AAL 			(1 << 25)   		// Address Aligned Beats
#define B_4X_PBL		(1 << 24)		// 4xPBL Mode
#define B_USB			(1 << 23)		// Use Separate PBL
#define B_RPBL(a)		((a & 0x3F) << 17)	// RxDMA PBL  [22:17]
#define B_FIX_BURST_EN		(1 << 16)		// Fixed Burst Enable [16]
#define B_TX_PRIOR_RATIO(a)	(((a & 3) - 1) << 14)	// (PR) TX:RX DMA Priority ration a:1 
							// 0 ~ 4 [15:14]
#define B_BURST_LEN(a)  	((a & 0x3F) << 8)	// Burst length 0,1,2,4,8,16,32
							// [13:8]
#define B_DESC_SKIP_LEN(a)	((a & 0x1F) << 2)	// Descriptor skip length [6:2]
#define B_ARBIT_SCHEME_EN	(1 << 1)		// 1: Rx Over Tx, 0: depend on [15:14]
#define B_SW_RESET		(1 << 0)

//status 0x1014
#define B_TIME_STAMP_TRIGGER	(1 << 29)		// Self-Clear	RO
#define B_PMT_INTR		(1 << 28)		// Self-Clear	RO
#define B_MMC_INTR		(1 << 27)		// Self-Clear	RO
#define B_LINE_INTF_INTR	(1 << 26)		// Self-Clear	RO

#define B_ERR_DESC		(1 << 25)		// RO, 0: Data Buffer 1: DESC  
#define B_ERR_READ_TR		(1 << 24)		// RO, 0: Write, 1: Read 
#define B_ERR_TXDMA		(1 << 23)		// RO, 0: RxDma, 1: TxDma

#define B_TX_STATE(a)		((a & 7) >> 20)		// RO, 
#define B_RX_STATE(a)		((a & 7) >> 17)		// RO,

#define B_NORMAL_INTR_SUM	(1 << 16)		// RW-WC
#define B_ABNORM_INTR_SUM	(1 << 15)		// RW-WC
#define B_EARLY_RX		(1 << 14)		// RW-WC, Normal interrupt
#define B_FATAL_BUS_ERROR	(1 << 13)		// RW-WC, ABNormal -> [25:23]
#define B_EARLY_TX		(1 << 10)		// RW-WC, ABNormal
#define B_RX_WDOG_TIMEOUT	(1 << 9)		// RW-WC, ABNormal
#define B_RX_STOP_PROC		(1 << 8)		// RW-WC, ABNormal
#define B_RX_BUF_UNAVAIL	(1 << 7)		// RW-WC, ABNormal
#define B_RX_INTR		(1 << 6)		// RW-WC, Normal
#define B_TX_UNDERFLOW		(1 << 5)		// RW-WC, ABNormal
#define B_RX_OVERFLOW		(1 << 4)		// RW-WC, ABNormal
#define B_TX_JAB_TIMEOUT	(1 << 3)		// RW-WC, ABNormal
#define B_TX_BUF_UNAVAIL	(1 << 2)		// RW-WC, Normal
#define B_TX_STOP_PROC		(1 << 1)		// RW-WC, ABNormal
#define B_TX_INTR		(1 << 0)		// RW-WC, Normal



//operationMode 0x1018
#define B_TX_STR_FW		(1 << 21)		// Tx Store and forward [21]

#define B_TX_THRESH_CTRL(a) 	((a & 7) << 14)		// Tx threshold control [16:14]
#define B_TX_THRESHOLD_16 	(7 << 14)		// Tx threshold control 16
#define B_TX_THRESHOLD_24 	(6 << 14)		// Tx threshold control 24
#define B_TX_THRESHOLD_32 	(5 << 14)		// Tx threshold control 32
#define B_TX_THRESHOLD_40 	(4 << 14)		// Tx threshold control 40
#define B_TX_THRESHOLD_64 	(0 << 14)		// Tx threshold control 64
#define B_TX_THRESHOLD_128 	(1 << 14)		// Tx threshold control 128
#define B_TX_THRESHOLD_192 	(2 << 14)		// Tx threshold control 192
#define B_TX_THRESHOLD_256 	(3 << 14)		// Tx threshold control 256
#define B_TX_EN			(1 << 13)		// Start/Stop Tx Command

#define B_RX_THRESH_CTRL(a)	((a & 3) << 3)		// Rx Threshold control	[4:3]
#define B_RX_THRESHOLD_32	(1 << 3)	
#define B_RX_THRESHOLD_64	(0 << 3)
#define B_RX_THRESHOLD_96	(2 << 3)
#define B_RX_THRESHOLD_128	(3 << 3)
#define B_OPER_2ND_FRAME	(1 << 2)
#define B_RX_EN			(1 << 1)		// Start/Stop Rx 


//intrEnable
// same status [16:0]
#define B_INTR_ENABLE_ALL  ( 	  B_NORMAL_INTR_SUM	\
			  	| B_ABNORM_INTR_SUM \
				| B_FATAL_BUS_ERROR \
				| B_RX_WDOG_TIMEOUT \
				| B_RX_STOP_PROC    \
				| B_RX_INTR	    \
				| B_RX_BUF_UNAVAIL  \
				| B_TX_UNDERFLOW    \
				| B_RX_OVERFLOW	    \
				| B_TX_JAB_TIMEOUT  \
				| B_TX_BUF_UNAVAIL  \
				| B_TX_STOP_PROC    \
				| B_TX_INTR	    )

#if 0
				| B_EARLY_RX	    \
				| B_EARLY_TX	    \

#endif

/*==================================================*/
/*           Define Using DMA Descriptor            */
/*==================================================*/


// S/W configuration 
// number of DESCRIPTIN
#define NUM_TX_DESC		(1536)		// rx * 3
#define NUM_RX_DESC		(120)		// ???

#define TX_DESC_SIZE		(sizeof(DMA_DESC_T) * NUM_TX_DESC)
#define RX_DESC_SIZE		(sizeof(DMA_DESC_T) * NUM_RX_DESC)

// DESC MODE 
#define DESC_MODE_RING		(0)
#define DESC_MODE_CHAIN		(1)

// DESC RX size mask
#if (SDP_GMAC_BUS > 64)
#  define SDP_GMAC_BUS_MASK	(0xFFFFFFF0)		// 16 byte align 
#elif (SDP_GMAC_BUS > 32)	
#  define SDP_GMAC_BUS_MASK	(0xFFFFFFF8)		// 8 byte align
#else	
#  define SDP_GMAC_BUS_MASK	(0xFFFFFFFC)		// 4 byte align
#endif

typedef struct {
	u32	status;
	u32	length;

	u32	buffer1;
	u32	buffer2;

	u32 	data1;
	u32	data2;
#if (SDP_GMAC_BUS > 64)
	u32	timestamp_l;
	u32	timestamp_h;
#endif
}DMA_DESC_T;
	

// DESC COMMON STATUS
#define DESC_CHECK_BIT(a)	(1 << a)
#define DESC_OWN_BY_DMA		(1 << 31)		// [31] common

// RX DESC
// RX DESC STATUS
#define DESC_DA_FILTER_FAIL	(1 << 30)		// [30]
#define DESC_FRAME_LEN(a)	((a & 0x3FFF) << 16) 	// [29:16]
#define DESC_GET_FRAME_LEN(a)	((a >> 16) & 0x3FFF)	// [29:16]
#define DESC_ERROR		(1 << 15)		// [15]
#define DESC_RX_TRUNCATE	(1 << 14)
#define DESC_SA_FILTER_FAIL	(1 << 13)
#define DESC_RX_LEN_ERR		(1 << 12)
#define DESC_RX_DAMAGE		(1 << 11)
#define DESC_RX_OVERFLOW        DESC_RX_DAMAGE		// 0.95
#define DESC_RX_VLAN_TAG	(1 << 10)
#define DESC_RX_1ST		(1 << 9)
#define DESC_RX_LAST		(1 << 8)
#define DESC_RX_LONG_FRAME	(1 << 7)
#define DESC_RX_COLLISION	(1 << 6)
#define DESC_RX_FRAME_ETHER	(1 << 5)
#define DESC_RX_WATCHDOG	(1 << 4)
#define DESC_RX_MII_ERR		(1 << 3)
#define DESC_RX_DRIBLING	(1 << 2)
#define DESC_RX_CRC		(1 << 1)
#define DESC_RX_MAC_MATCH	(1)

#define RX_FRAME_LEN_MASK	(0x3FFF)
#define RX_DESC_END_RING	(0x00008000UL)




// TX DESC
// TX DESC STATUS
// [31] Common
#define DESC_TX_INT_EN		(1 << 30) 		// desc tx interrupt enable
#define DESC_TX_LAST		(1 << 29)
#define DESC_TX_1ST		(1 << 28)
#define DESC_TX_CRC_DIS		(1 << 27)
#define DESC_TX_PADD_DIS	(1 << 26)
// ???? [25:24]
#define DESC_TX_CIS(a)		((a & 3) << 22)
#define DESC_TX_END_RING	(1 << 21)
#define DESC_TX_CHAIN		(1 << 20)
// ???? [19:17]
#define DESC_TX_IPV4_ERR	(1 << 16)
// ???? [15]
#define DESC_TX_TIMEOUT		(1 << 14)
#define DESC_TX_FRAME_FLUSH	(1 << 13)
#define DESC_TX_PAY_ERR		(1 << 12)
#define DESC_TX_LOST_CARR	(1 << 11)		// lost carrier
#define DESC_TX_NO_CARR		(1 << 10)		// No carrier
#define DESC_TX_LATE_COLL	(1 << 9)		// Late collision
#define DESC_TX_EXC_COLL	(1 << 8)		// Exc collision
#define DESC_TX_VLAN_FRAME	(1 << 7)
#define DESC_TX_COLLISION(a)	((a >> 3) & 0xF)
#define DESC_TX_EXC_DEFERRAL	(1 << 2)		// Exc deferral
#define DESC_TX_UNDERFLOW	(1 << 1)		// 
#define DESC_TX_DEFERRED	(1 << 0)		

#define TX_CIS_BYPASS		DESC_TX_CIS(0)
#define TX_CIS_IPV4HDR_CS	DESC_TX_CIS(1)
#define TX_CIS_TCP_CS		DESC_TX_CIS(2)
#define TX_CIS_TCP_SEUDO_CS	DESC_TX_CIS(3)

#define TX_DESC_END_RING	DESC_TX_END_RING    	// (0x00200000UL)



// all bit set '0' and set last descriptor
// last rx desc length = 0x8000
// last tx desc status = 0x200000

#define sdpGmac_txInitDesc(pDmaDesc)  { \
		memset(pDmaDesc, 0, TX_DESC_SIZE); \
		pDmaDesc[NUM_TX_DESC-1].status = TX_DESC_END_RING; \
	}

#define sdpGmac_rxInitDesc(pDmaDesc)  { \
		memset(pDmaDesc, 0, RX_DESC_SIZE); \
		pDmaDesc[NUM_RX_DESC-1].length = RX_DESC_END_RING; \
	}


// Define Descriptor Only ENH

#define DESC_SIZE_MASK		(0x00001FFF)
#define DESC_SIZE1_MASK		(DESC_SIZE_MASK)

#define DESC_SIZE2_SHIFT	(16)
#define DESC_SIZE2_MASK		(DESC_SIZE_MASK << DESC_SIZE2_SHIFT)
#define DESC_GET_SIZE2(a)	((a >> 16) & 0x1FFF)

// TODO: Not Implemented yet 
#define sdpGmac_rxDescChained(pRxDesc)		(0)
#define sdpGmac_txDescChained(pTxDesc)		(0)

// length check size1 & size2
// Empty is 1, Not empty : 0
#define sdpGmac_descEmpty(pDmaDesc) \
		((pDmaDesc->length & 0x1FFF1FFF) ? 0 : 1)

// dma desc -> length RX END ring bit set or RxDesc == data2
#define sdpGmac_lastRxDesc(pGmacDev, pDmaDesc) \
		((pDmaDesc->length & RX_DESC_END_RING) ? 1 : \
		((u32)pGmacDev->pRxDesc == pDmaDesc->data2) ? 1 : 0)

// dma desc -> status  TX END ring bit check or TxDesc == data2
#define sdpGmac_lastTxDesc(pGmacDev, pDmaDesc) \
		((pDmaDesc->status & TX_DESC_END_RING) ? 1 : \
		((u32)pGmacDev->pTxDesc == pDmaDesc->data2) ? 1 : 0)


#define CONVERT_DMA_QPTR(dmaDesc,  Length, Buffer1, Data1, Buffer2, Data2) { \
	dmaDesc.length = Length; \
	dmaDesc.buffer1 = (u32)Buffer1; \
	dmaDesc.data1 = (u32)Data1; \
	dmaDesc.buffer2 = (u32)Buffer2; \
	dmaDesc.data2 = (u32)Data2; \
}
	



#endif /*  __SDP_GMAC_REG_H */

