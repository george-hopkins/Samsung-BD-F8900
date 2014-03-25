/***************************************************************************
 *
 *	arch/arm/plat-ccep/sdp_mac.c
 *	Samsung Elecotronics.Co
 *	Created by tukho.kim
 *
 * ************************************************************************/
/*
 * 2009.08.02,tukho.kim: Created by tukho.kim@samsung.com
 * 2009.08.31,tukho.kim: Revision 1.00
 * 2009.09.23,tukho.kim: re-define drv version 0.90
 * 2009.09.27,tukho.kim: debug dma_free_coherent and modify probe, initDesc 0.91
 * 2009.09.27,tukho.kim: add power management function drv version 0.92
 * 2009.09.28,tukho.kim: add retry count, when read mac address by i2c bus  0.921
 * 2009.10.06,tukho.kim: debug when rx sk buffer allocation is failed 0.93
 * 2009.10.19,tukho.kim: rx buffer size is fixed, ETH_DATA_LEN 1500 0.931
 * 2009.10.22,tukho.kim: debug when rx buffer alloc is failed 0.940
 * 2009.10.25,tukho.kim: recevice packet is error, not re-alloc buffer 0.941
 * 2009.10.27,tukho.kim: mac configuration initial value is "set watchdog disable 0.942
 * 2009.11.05,tukho.kim: debug to check tx descriptor in hardStart_xmit 0.943
 * 2009.11.18,tukho.kim: rtl8201E RXD clock toggle active, rtl8201E hidden register 0.9431
 * 2009.11.21,tukho.kim: modify netDev_rx routine 0.95
 * 2009.12.01,tukho.kim: debug phy check media, 0.951
 * 			 full <-> half ok, 10 -> 100 ok, 100 -> 10 need to unplug and plug cable
 * 2009.12.02,tukho.kim: debug module init, exit  0.952
 * 2009.12.15,tukho.kim: hwSpeed init value change to STATUS_SPEED_100 0.953
 * 2009.12.15,tukho.kim: when alloc rx buffer,  align rx->data pointer by 16 0.954
 * 2009.12.28,tukho.kim: remove to check magic code when read mac address by i2c 0.9541
 * 2010.01.13,tukho.kim: debug sdpGmac_rxDescRecovery function 0.955
 * 2010.01.23,tukho.kim: debug wait code about DMA reset in sdpGmac_reset  0.956
 * 2010.06.25,tukho.kim: rename file sdpGmacInit.c to sdp_mac.c and merge with sdpGmacBase.c  0.957
 * 2010.09.27,tukho.kim: bug fix, cortex shared device region(write buffer) 0.958
 * 2010.09.29,tukho.kim: bug fix, when insmod, No such device 0.959
 * 2010.09.29,tukho.kim: bug fix, remove reverse clock configuration 0.9591
 * 2011.04.29,tukho.kim: replace dsb with wmb (outer cache) 0.9592
 * 2011.11.18,tukho.kim: add function sdpGmac_txQptr_replacement and it run when meet to tx error 0.96
 * 2011.11.30,tukho.kim: buf fix, sdpGmac_txQptr_replacement and add mac dma hw info 0.962
 * 2011.12.07,tukho.kim: buf fix, sdpGmac_setTxQptr ring buffer 0.963
 * 2011.12.07,tukho.kim: buf fix, phy initialize by lpa register 0.964
 * 2011.12.14,tukho.kim: buf fix, control variable and hw memory 0.965
 * 2012.04.20,drain.lee: delete phy code, and use phy_device subsystem. change MAC addr setting sequence.  0.966
 * 2012.04.23,drain.lee: change to use NAPI subsystem.  0.967
 * 2012.05.11,drain.lee: interrupt error fix(use masked status). change reset timeout 5sec 0.968
 * 2012.05.14,drain.lee: move chip dependent code. 0.969
 * 2012.05.14,drain.lee: patch, linux-2.6.35.11 r322:323 0.970(modify txqptr control value and descriptor)
 * 2012.05.14,drain.lee: patch, linux-2.6.35.11 r335:336 0.971(2012/01/03 release version)
 * 2012.05.25,drain.lee: bug fix, rxDescRecovery error fix.
 * 2012.05.27,drain.lee: bug fix, scheduling while atomic in sdpGmac_netDev_ioctl().
 * 2012.07.23,drain.lee: bug fix, phy_device control flow.
 * 2012.07.24,drain.lee: bug fix, Phy device irq bug fix.
 * 2012.08.03,drain.lee: bug fix, Multicast mac address setting sequence. v0.975
 * 2012.08.03,drain.lee: change phy attach at open. v0.976
 * 2012.08.29,drain.lee: support Normal desc v0.977
 * 2012.08.29,drain.lee: buf fix, rx buf size error in Normal desc v0.978
 * 2012.10.05,drain.lee: add selftest(loopback test) for ethtool v0.979
 * 2012.10.18,drain.lee: change selftest(loopback test) use ETH_TEST_FL_EXTERNAL_LB flag v0.980
 * 2012.12.13,drain.lee: add debug dump in selftest. v0.981
 * 2012.12.14,drain.lee: bug fix, size of cache operation incorrect. v0.982
 * 2012.12.14,drain.lee: change debug dump v0.983
 * 2012.12.17,drain.lee: move dma mask setting to sdpxxxx.c file v0.984
 * 2012.12.21,drain.lee: fix compile warning v0.985
 * 2012.12.28,drain.lee: bug fix, tx unmap DMA_FROM_DEVICE -> DMA_TO_DEVICE v0.986
 * 2013.01.15,drain.lee: fix, selftest(ext lb error). v0.987
 * 2013.01.15,drain.lee: add reg dump in abnormal interrupt. v0.988
 * 2013.01.31,drain.lee: bugfix, desc ring corruption. v0.989
 * 2013.02.01,drain.lee: fix, prevent defect(Buffer not null terminated) v0.990
 */

#ifdef CONFIG_SDP_MAC_DEBUG
#define DEBUG
#endif

#include <linux/time.h>		// for debugging
#include <linux/mii.h>
#include <linux/phy.h>
#include "sdp_mac.h"

#define GMAC_DRV_VERSION	"0.990"

extern int sdp_get_revision_id(void);

static int sdpGmac_reset(struct net_device *pNetDev);
static int sdpGmac_getMacAddr(struct net_device *pNetDev, u8 *pAddr);
static void sdpGmac_setMacAddr(struct net_device *pNetDev, const u8 *pAddr );
static void sdpGmac_dmaInit(struct net_device *pNetDev);
static void sdpGmac_gmacInit(struct net_device *pNetDev);
static int sdpGmac_setRxQptr(SDP_GMAC_DEV_T* pGmacDev, const DMA_DESC_T* pDmaDesc, const u32 length2);
static int sdpGmac_getRxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc);
static int sdpGmac_setTxQptr(SDP_GMAC_DEV_T* pGmacDev,
		const DMA_DESC_T* pDmaDesc, const u32 length2, const int offloadNeeded);
static int sdpGmac_getTxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc);
static void sdpGmac_clearAllDesc(struct net_device *pNetDev);
static int sdpGmac_netDev_initDesc(SDP_GMAC_DEV_T* pGmacDev, u32 descMode);


struct sdp_mac_mdiobus_priv {
	u32 addr;
	u32 data;
};

static inline u32 sdp_mac_readl(const volatile void __iomem *addr)
{
	return readl(addr);
}

static inline void sdp_mac_writel(const u32 val, volatile void __iomem *addr)
{
	writel(val, addr);
}

static void sdp_mac_gmac_dump(SDP_GMAC_DEV_T *pGmacDev)
{
	u32 *pGmacBase = (u32 *)pGmacDev->pGmacBase;
	u32 gmac_reg_dump[0xC0/sizeof(u32)];
	resource_size_t phy_base =
		platform_get_resource(to_platform_device(pGmacDev->pDev), IORESOURCE_MEM, 0)->start;
	u32 i;

	for(i = 0; i < ARRAY_SIZE(gmac_reg_dump); i++) {
		gmac_reg_dump[i] = sdp_mac_readl(pGmacBase + i);
	}

	dev_info(pGmacDev->pDev, "===== SDP-MAC GMAC DUMP (VA: 0x%p, PA: 0x%08llx) =====\n",
		pGmacBase, (u64)phy_base);
	for(i = 0; i < ARRAY_SIZE(gmac_reg_dump); i+=4) {
		dev_info(pGmacDev->pDev, "0x%08llx: 0x%08x 0x%08x 0x%08x 0x%08x\n",
				(u64)phy_base+(i*sizeof(gmac_reg_dump[0])),
				gmac_reg_dump[i+0],
				gmac_reg_dump[i+1],
				gmac_reg_dump[i+2],
				gmac_reg_dump[i+3]
			  );
	}
	pr_info("\n");
}

static void sdp_mac_dma_dump(SDP_GMAC_DEV_T *pGmacDev)
{
	u32 *pDmaBase = (u32 *)pGmacDev->pDmaBase;
	u32 dma_reg_dump[0x60/sizeof(u32)];
	resource_size_t phy_base =
		platform_get_resource(to_platform_device(pGmacDev->pDev), IORESOURCE_MEM, 4)->start;
	u32 i;

	for(i = 0; i < ARRAY_SIZE(dma_reg_dump); i++) {
		dma_reg_dump[i] = sdp_mac_readl(pDmaBase + i);
	}

	dev_info(pGmacDev->pDev, "===== SDP-MAC DMA DUMP (VA: 0x%p, PA: 0x%08llx) =====\n",
		pDmaBase, (u64)phy_base);
	for(i = 0; i < ARRAY_SIZE(dma_reg_dump); i+=4) {
		dev_info(pGmacDev->pDev, "0x%08llx: 0x%08x 0x%08x 0x%08x 0x%08x\n",
				(u64)phy_base+(i*sizeof(dma_reg_dump[0])),
				dma_reg_dump[i+0],
				dma_reg_dump[i+1],
				dma_reg_dump[i+2],
				dma_reg_dump[i+3]
			  );
	}
	pr_info("\n");
}

static int sdp_mac_mdio_read(struct mii_bus *bus, int phy_id, int regnum)
{
	struct sdp_mac_mdiobus_priv *priv = bus->priv;

	while(sdp_mac_readl((void *)priv->addr) & B_GMII_ADDR_BUSY);

	sdp_mac_writel(GMII_ADDR_READ(phy_id, regnum), (void *)priv->addr);

	while(sdp_mac_readl((void *)priv->addr) & B_GMII_ADDR_BUSY);

	return sdp_mac_readl((void *)priv->data)&0xFFFF;
}
static int sdp_mac_mdio_write(struct mii_bus *bus, int phy_id, int regnum, u16 val)
{
	struct sdp_mac_mdiobus_priv *priv = bus->priv;

	while(sdp_mac_readl((void *)priv->addr) & B_GMII_ADDR_BUSY);


	sdp_mac_writel(val, (void *)priv->data);
	sdp_mac_writel(GMII_ADDR_WRITE(phy_id, regnum), (void *)priv->addr);

	while(sdp_mac_readl((void *)priv->addr) & B_GMII_ADDR_BUSY);

	return 0;
}

static phy_interface_t sdp_mac_get_interface(struct net_device *pNetDev)
{
	SDP_GMAC_DEV_T	*pGmacDev = netdev_priv(pNetDev);
	u32 hw_feature;

	hw_feature = pGmacDev->pDmaBase->hwfeature;

	switch(B_ACTPHYIF(hw_feature))
	{
	case 0:
		return PHY_INTERFACE_MODE_GMII;
	case 1:
		return PHY_INTERFACE_MODE_RGMII;
	case 2:
		return PHY_INTERFACE_MODE_SGMII;
	case 3:
		return PHY_INTERFACE_MODE_TBI;
	case 4:
		return PHY_INTERFACE_MODE_RMII;
	case 5:
		return PHY_INTERFACE_MODE_RTBI;
	case 6:
	case 7:
	default:
		dev_err(pGmacDev->pDev, "Not suppoted phy interface!!!(%#x)\n", B_ACTPHYIF(hw_feature));
		return 0xFF;
	}
}

// 0.954
static inline void
sdpGmac_skbAlign(struct sk_buff *pSkb) {
	u32 align = (u32)pSkb->data & 0x3UL;

	switch(align){
		case 1:
		case 3:
			skb_reserve(pSkb, (int)align);
			break;
		case 2:
			break;
		default:
			skb_reserve(pSkb, 2);
	}
} // 0.954 end

static void sdpGmac_netDev_tx(struct net_device *pNetDev, int budget)
{
	SDP_GMAC_DEV_T	*pGmacDev = netdev_priv(pNetDev);

	struct net_device_stats *pNetStats = &pGmacDev->netStats;
	DMA_DESC_T	txDesc;

	u32 checkStatus;
	int descIndex;
	int cleaned = 0;

	DPRINTK_GMAC_FLOW("called\n");

#if 1
	do{
		if(budget != 0 && cleaned >= budget) break;

		descIndex = sdpGmac_getTxQptr(pGmacDev, &txDesc);

		cleaned++;

		if(descIndex < 0) break;	// break condition
		if(!txDesc.data1) continue;	// not receive

		DPRINTK_GMAC_FLOW("Tx Desc %d for skb 0x%08x whose status is %08x\n",
					descIndex, txDesc.data1, txDesc.status);

		//TODO : IPC_OFFLOAD  ??? H/W Checksum
		//
		//
		//

		dma_unmap_single(pGmacDev->pDev, txDesc.buffer1,
				DESC_GET_SIZE1(txDesc.length), DMA_TO_DEVICE);

		dev_kfree_skb_any((struct sk_buff*)txDesc.data1);

		// ENH_MODE
		checkStatus = (txDesc.status & 0xFFFF) & TDES0_ES;

		if(!checkStatus) {
			pNetStats->tx_bytes += txDesc.length & DESC_SIZE_MASK;
			pNetStats->tx_packets++;
		}
		else {	// ERROR
			pNetStats->tx_errors++;

			if(txDesc.status & (TDES0_LCO | TDES0_EC))
				pNetStats->tx_aborted_errors++ ;
			if(txDesc.status & (TDES0_LC | TDES0_NC))
				pNetStats->tx_carrier_errors++;
		}

		pNetStats->collisions += TDES0_CC_GET(txDesc.status);

	}while(1);

	netif_wake_queue(pNetDev);

#endif
	DPRINTK_GMAC_FLOW("exit\n");
	return;
}


/* budget : maximum number of descs that processed in rx call.
			0 is inf
*/
static int sdpGmac_netDev_rx(struct net_device *pNetDev, int budget)
{
	SDP_GMAC_DEV_T	*pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	struct net_device_stats *pNetStats = &pGmacDev->netStats;
	struct sk_buff * pSkb;
	DMA_DESC_T	rxDesc;

	u32 checkStatus;
	u32 len;
	int descIndex;
	dma_addr_t rxDmaAddr;
	int nrChkDesc = 0;	// packet
	int alloc_error = 0;
	int dummy_count = 0;

	DPRINTK_GMAC_FLOW("called\n");

	do{

		/* check budget */
		if((budget != 0) && (nrChkDesc >= budget)) {
			break;
		}

		descIndex = sdpGmac_getRxQptr(pGmacDev, &rxDesc);

		if(descIndex < 0) {
			break;	// break condition
		}

		nrChkDesc++;

//		if(rxDesc.data1 == (u32)NULL) {	// 0.93, remove warning msg
		if(rxDesc.data1 == (u32)pGmacDev->pRxDummySkb) {	// 0.940
//			DPRINTK_GMAC_ERROR("rx skb is none\n");
//			continue;	// not receive
			dummy_count++;
			goto __alloc_rx_skb;	// 0.93
		}


		DPRINTK_GMAC("Rx Desc %d for skb 0x%08x whose status is %08x\n",
				descIndex, rxDesc.data1, rxDesc.status);

		if(RDES0_FL_GET(rxDesc.status) < DESC_GET_SIZE1(rxDesc.length)) {
			dma_unmap_single(pGmacDev->pDev, rxDesc.buffer1, RDES0_FL_GET(rxDesc.status), DMA_FROM_DEVICE);
		}
		else
		{
			dma_unmap_single(pGmacDev->pDev, rxDesc.buffer1, DESC_GET_SIZE1(rxDesc.length), DMA_FROM_DEVICE);
		}

		pSkb = (struct sk_buff*)rxDesc.data1;
// 0.95
		checkStatus = rxDesc.status & (RDES0_ES | RDES0_LE);

// rx byte
		if(!checkStatus &&
		   (rxDesc.status & RDES0_LS) &&
		   (rxDesc.status & RDES0_FS) )
		{
			len = RDES0_FL_GET(rxDesc.status) - 4; // without crc byte

			skb_put(pSkb,len);

			//TODO : IPC_OFFLOAD  ??? H/W Checksum

			// Network buffer post process
			pSkb->dev = pNetDev;
			pSkb->protocol = eth_type_trans(pSkb, pNetDev);

			napi_gro_receive(&pGmacDev->napi, pSkb);

			pNetDev->last_rx = jiffies;
			pNetStats->rx_packets++;
			pNetStats->rx_bytes += len;

		} else {	// ERROR
			pNetStats->rx_errors++;

			if(rxDesc.status & RDES0_OE) pNetStats->rx_over_errors++;	// 0.95
			if(rxDesc.status & RDES0_LC) pNetStats->collisions++;
			if(rxDesc.status & RDES0_RWT) pNetStats->rx_frame_errors++;	// 0.95
			if(rxDesc.status & RDES0_CE) pNetStats->rx_crc_errors++ ;
//			if(rxDesc.status & RDES0_DBE) pNetStats->rx_frame_errors++ ;	// 0.95
			if(rxDesc.status & RDES0_LE) pNetStats->rx_length_errors++;	// 0.95

			memset(pSkb->data, 0, ETH_DATA_LEN + ETHER_PACKET_EXTRA);	// buffer init

			goto __set_rx_qptr;					// 0.941
		}
//0.95 end

__alloc_rx_skb:
		len = ETH_DATA_LEN + ETHER_PACKET_EXTRA + 4;
		len += ((SDP_GMAC_BUS >> 3) - 1);
		len &= SDP_GMAC_BUS_MASK;

//		pSkb = dev_alloc_skb(pNetDev->mtu + ETHER_PACKET_EXTRA);
//
		if(alloc_error) {
			pSkb = NULL;
		} else {
			pSkb = dev_alloc_skb(len);	// 0.931 // 0.954
		}

		if (pSkb == NULL){
//			DPRINTK_GMAC_ERROR("could not allocate skb memory size %d, loop %d\n",
//						(ETH_DATA_LEN + ETHER_PACKET_EXTRA), nrChkDesc);	// 0.931

			pNetStats->rx_dropped++;
			// TODO : check this status
			//return nrChkDesc;
			pSkb = pGmacDev->pRxDummySkb;	// 0.940
			alloc_error++;
		}
		else {
			sdpGmac_skbAlign(pSkb); 	// 0.954
		}


__set_rx_qptr:		// 0.941
		rxDmaAddr = dma_map_single(pGmacDev->pDev,
					   pSkb->data,
					   (size_t)skb_tailroom(pSkb),
					   DMA_FROM_DEVICE);

		len = (u32)skb_tailroom(pSkb) + (SDP_GMAC_BUS >> 3) - 1;
		len &= SDP_GMAC_BUS_MASK;
		CONVERT_DMA_QPTR(rxDesc, len, rxDmaAddr, pSkb, 0, 0);

		if(sdpGmac_setRxQptr(pGmacDev, &rxDesc, 0) < 0){
			if(pSkb != pGmacDev->pRxDummySkb) {	// 0.940
				dev_kfree_skb_any(pSkb);
			}
			DPRINTK_GMAC_ERROR("Error set rx qptr\n");
			break;
		}
	}while(1);

	if(dummy_count){
		DPRINTK_GMAC_ERROR("rx skb is dummy buffer %d\n", dummy_count);
	}

	if(alloc_error){
		DPRINTK_GMAC_ERROR("skb memory allocation failed %d\n", alloc_error);
	}

	if(pGmacDev->is_rx_stop) {
		/* rx ring is not full. start rx */
		pDmaReg->operationMode |= B_RX_EN;
		pGmacReg->configuration |= B_RX_ENABLE;
		pGmacDev->is_rx_stop = false;
	}


	DPRINTK_GMAC_FLOW("exit\n");
	return nrChkDesc;
}

static inline void
sdpGmac_rxDescRecovery(struct net_device *pNetDev)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;
	DMA_DESC_T *pRxDesc = pGmacDev->pRxDesc;

	u32 idx = 0;

	pDmaReg->operationMode &= ~B_RX_EN;
	pGmacReg->configuration &= ~B_RX_ENABLE;

	for (idx = 0; idx < NUM_RX_DESC; idx++) {
		if(!(pRxDesc->status & RDES0_OWN)) {
			pGmacDev->rxNext = idx;
			pGmacDev->pRxDescNext = pRxDesc;
			pGmacDev->rxBusy = idx;
			pGmacDev->pRxDescBusy = pRxDesc;

			sdpGmac_netDev_rx(pNetDev, 0);  // check buffer

			break;
		}
		pRxDesc++;
	}

	idx = (pDmaReg->curHostRxDesc - pDmaReg->rxDescListAddr) / sizeof(DMA_DESC_T);

	DPRINTK_GMAC_FLOW("Recovery rx desc\n");
	DPRINTK_GMAC_FLOW("current rx desc %d \n", idx);

	pRxDesc = pGmacDev->pRxDesc;		// 0.955
	pGmacDev->rxNext = idx;
	pGmacDev->pRxDescNext = pRxDesc + idx;
	pGmacDev->rxBusy = idx;
	pGmacDev->pRxDescBusy = pRxDesc + idx;

	pGmacReg->configuration |= B_RX_ENABLE;
	pDmaReg->operationMode |= B_RX_EN;

	pGmacDev->netStats.rx_dropped++;  	// rx packet is dropped 1 packet
}

static void
sdpGmac_abnormal_intr_status (const u32 intrStatus, struct net_device* const pNetDev)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	if(intrStatus & B_FATAL_BUS_ERROR) {
		DPRINTK_GMAC_ERROR("Fatal Bus Error: \n");

		if(intrStatus & B_ERR_DESC)
			DPRINTK_GMAC_ERROR("\tdesc access error\n");
		else
			DPRINTK_GMAC_ERROR("\tdata buffer access error\n");

		if(intrStatus & B_ERR_READ_TR)
			DPRINTK_GMAC_ERROR("\tread access error\n");
		else
			DPRINTK_GMAC_ERROR("\twrite access error\n");

		if(intrStatus & B_ERR_TXDMA)
			DPRINTK_GMAC_ERROR("\ttx dma error\n");
		else
			DPRINTK_GMAC_ERROR("\trx dmaerror\n");

		//TODO : DMA re-initialize
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);

	}

	if(intrStatus & B_EARLY_TX) {
		DPRINTK_GMAC_ERROR("Early Tx Error\n");
		// TODO :
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);
	}

	if(intrStatus & B_RX_WDOG_TIMEOUT) {
		DPRINTK_GMAC_ERROR("Rx WatchDog timeout Error\n");
		// TODO :
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);
	}

	if(intrStatus & B_RX_STOP_PROC) {
		DPRINTK_GMAC_FLOW("Rx process stop\n");
		// TODO :
	}

	if(intrStatus & B_RX_BUF_UNAVAIL) {
		dev_err(pGmacDev->pDev, "Rx buffer unavailable!(rxBusy%u rxNext%u rxDMA%u)\n",
			pGmacDev->rxBusy, pGmacDev->rxNext,
			(pDmaReg->curHostRxDesc-pDmaReg->rxDescListAddr)/sizeof(DMA_DESC_T));

		/* stop rx */
		pGmacDev->is_rx_stop = true;
		pDmaReg->operationMode &= ~B_RX_EN;
		pGmacReg->configuration &= ~B_RX_ENABLE;

		pGmacDev->netStats.rx_dropped++;

		napi_schedule(&pGmacDev->napi);
//		sdpGmac_rxDescRecovery(pNetDev);
	}

	if(intrStatus & B_RX_OVERFLOW) {
		DPRINTK_GMAC_ERROR("Rx overflow Error\n");
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);
		// TODO :
	}

	if(intrStatus & B_TX_UNDERFLOW) {
		DPRINTK_GMAC_ERROR("Tx underflow Error\n");
		// TODO : tx descriptor fix
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);
	}

	if(intrStatus & B_TX_JAB_TIMEOUT) {
		DPRINTK_GMAC_ERROR("Tx jabber timeout Error\n");
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);
		// TODO :
	}

	if(intrStatus & B_TX_STOP_PROC) {
		DPRINTK_GMAC_FLOW("Tx process stop\n");
		// TODO :
	}

}


static irqreturn_t sdpGmac_intrHandler(int irq, void * devId)
{
	struct net_device *pNetDev = (struct net_device*)devId;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_MMC_T *pMmcReg = pGmacDev->pMmcBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	u32 intrStatus, maskedIntrStatus;
#if 0
	struct timeval tv_start, tv_end;

	do_gettimeofday(&tv_start);
#endif
	DPRINTK_GMAC_FLOW("called\n");

	if(unlikely(pNetDev == NULL)) {
		DPRINTK_GMAC_ERROR("Not register Network Device, please check System\n");
		return -SDP_GMAC_ERR;
	}

	if(unlikely(pGmacDev == NULL)) {
		DPRINTK_GMAC_ERROR("Not register SDP-GMAC, please check System\n");
		return -SDP_GMAC_ERR;
	}

	intrStatus = pDmaReg->status;

	/* use masked interrupts */
	maskedIntrStatus = intrStatus & (pDmaReg->intrEnable | ~B_INTR_ALL);

	if((maskedIntrStatus & B_INTR_ALL) == 0) {
		/* all pendig interrupt clear */
		pDmaReg->status = intrStatus;
		return IRQ_NONE;
	}


	// DMA Self Clear bit is check

	if(maskedIntrStatus & B_TIME_STAMP_TRIGGER) {
		DPRINTK_GMAC("INTR: time stamp trigger\n");
	}

	if(maskedIntrStatus & B_PMT_INTR) {
		// TODO: make pmt resume function
		DPRINTK_GMAC("INTR: PMT interrupt\n");
	}

	if(maskedIntrStatus & B_MMC_INTR) {
		// TODO: make reading mmc intr rx and tx register
		// this register is hidden in Datasheet
		DPRINTK_GMAC_ERROR("INTR: MMC rx: 0x%08x\n", pMmcReg->intrRx);
		DPRINTK_GMAC_ERROR("INTR: MMC tx: 0x%08x\n", pMmcReg->intrTx);
		DPRINTK_GMAC_ERROR("INTR: MMC ipc offload: 0x%08x\n", pMmcReg->mmcIpcIntrRx);
	}

	if(maskedIntrStatus & B_LINE_INTF_INTR) {
		DPRINTK_GMAC("INTR: line interface interrupt\n");
	}

	//for NAPI, Tx, Rx process
	if(maskedIntrStatus & (B_RX_INTR | B_TX_INTR)) {
		//first. disable tx, rx interrupt.
		pDmaReg->intrEnable &= ~(B_RX_INTR | B_TX_INTR);
		napi_schedule(&pGmacDev->napi);
	}

// sample source don't support Early Rx, Tx
	if(maskedIntrStatus & B_EARLY_RX) {

	}

	if(maskedIntrStatus & B_TX_BUF_UNAVAIL) {

	}

//	if(maskedIntrStatus & (B_TX_INTR)) {
//		sdpGmac_netDev_tx(pNetDev);
//	}

// ABNORMAL Interrupt
	if(maskedIntrStatus & B_ABNORM_INTR_SUM) {
		sdpGmac_abnormal_intr_status (maskedIntrStatus, pNetDev);
	}

	pDmaReg->status = intrStatus;	// Clear interrupt pending register

	DPRINTK_GMAC_FLOW("exit\n");
#if 0
	do_gettimeofday(&tv_end);

	{
		unsigned long us;

		us = (tv_end.tv_sec - tv_start.tv_sec) * 1000000 +
			     (tv_end.tv_usec - tv_start.tv_usec);

		dev_printk(KERN_DEBUG, pGmacDev->pDev,
			"GMAC ISR process time %3luus (intr 0x%08x)\n",
			us, maskedIntrStatus);
	}
#endif
	return IRQ_HANDLED;
}

static int
sdpGmac_reset(struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;
	int i;

	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T * pDmaReg = pGmacDev->pDmaBase;

/* version check */
	switch(pGmacReg->version & 0xFF) {
		case GMAC_SYNOP_VER:
		case (0x36):		// sdp1103, sdp1106
		case (0x37):		// sdp12xx
			DPRINTK_GMAC("Find SDP GMAC ver %04x\n", pGmacReg->version);
			break;
        default:
			DPRINTK_GMAC_ERROR("Can't find GMAC!!!!\n");
			return -SDP_GMAC_ERR;
			break;
	}

	/* chip dependent init code 0.969 */
	if(!pGmacDev->plat->init) {
		dev_warn(pGmacDev->pDev, "Board initialization code is not available!\n");
	} else {
		int init_ret = 0;
		/* call init code */
		init_ret = pGmacDev->plat->init();
		if(init_ret < 0) {
			dev_err(pGmacDev->pDev, "failed to board initialization!!(%d)\n", init_ret);
			return init_ret;
		}
	}

/* sdpGmac Reset */
// DMA Reset
	pDmaReg->busMode = B_SW_RESET; // '0x1'

	udelay(5);	// need 3 ~ 5us
	for(i = 1; pDmaReg->busMode & B_SW_RESET; i++) {
		if(i > 5) {
			DPRINTK_GMAC_ERROR("GMAC Reset Failed!!!\n");
			DPRINTK_GMAC("plz check phy clock output register 0x19(0x1E40)\n");
			return -SDP_GMAC_ERR;
		}
		msleep(1000);
		DPRINTK_GMAC("Retry GMAC Reset(%d)\n", i);
	}
	DPRINTK_GMAC("DMA reset is release(normal status)\n");


// all interrupt disable
	pGmacReg->intrMask = 0x60F;/* all disable */
	pGmacReg->interrupt = pGmacReg->interrupt; // clear status register
	pDmaReg->intrEnable = 0;
	pDmaReg->status = B_INTR_ALL;  // clear status register

// function define
	pGmacDev->hwSpeed = 0;	// 0.953  must same to init configuration register
	pGmacDev->hwDuplex = 0;		   	// must same to init configuration register
	pGmacDev->msg_enable = NETIF_MSG_LINK;
	pGmacDev->has_gmac = !!(pGmacDev->pDmaBase->hwfeature|B_GMIISEL);


	return retVal;
}

static int
sdpGmac_getMacAddr(struct net_device *pNetDev, u8* pMacAddr)
{
	int retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	u32 macAddr[2] = {0, 0};

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock_bh(&pGmacDev->lock);

	macAddr[0] = pGmacReg->macAddr_00_Low;
	macAddr[1] = pGmacReg->macAddr_00_High & 0xFFFF;

	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_DBG("macAddrLow is 0x%08x\n", macAddr[0]);
	DPRINTK_GMAC_DBG ("macAddrHigh is 0x%08x\n", macAddr[1]);
	memcpy(pMacAddr, macAddr, N_MAC_ADDR);

	if(macAddr[0] == 0 && macAddr[1] == 0)	retVal = -SDP_GMAC_ERR;
	else if ((macAddr[0] == 0xFFFFFFFF) &&
		 (macAddr[1] == 0x0000FFFF)) retVal = -SDP_GMAC_ERR;

	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;

}

static void
sdpGmac_setMacAddr(struct net_device * pNetDev, const u8* pMacAddr)
{
	u32 macAddr[2] = {0, 0};
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	DPRINTK_GMAC_FLOW ("called\n");

	memcpy(macAddr, pMacAddr, N_MAC_ADDR);

	spin_lock_bh(&pGmacDev->lock);

	/* sequence is important. 20120420 dongseok lee */
	pGmacReg->macAddr_00_High = macAddr[1];
	wmb();
	udelay(1);
	pGmacReg->macAddr_00_Low = macAddr[0];
	wmb();

	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_FLOW ("exit\n");
}

static void
sdpGmac_dmaInit(struct net_device * pNetDev)
{
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_DMA_T * pDmaReg = pGmacDev->pDmaBase;

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock_bh(&pGmacDev->lock);

#if (SDP_GMAC_BUS > 32)  // word(bus width) skip length -> add 2 integer data to dma descriptor size
	pDmaReg->busMode = B_FIX_BURST_EN | B_BURST_LEN(8) | B_DESC_SKIP_LEN(1);
#else
	pDmaReg->busMode = B_FIX_BURST_EN | B_BURST_LEN(8) | B_DESC_SKIP_LEN(2);
#endif
	DPRINTK_GMAC_DBG ("busMode set %08x\n", pDmaReg->busMode);

	pDmaReg->operationMode = B_TX_STR_FW | B_TX_THRESHOLD_192;
	DPRINTK_GMAC_DBG ("opMode set %08x\n", pDmaReg->operationMode);

	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_FLOW ("exit\n");
}

static void
sdpGmac_gmacInit(struct net_device * pNetDev)
{
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	u32 regVal = 0;

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock_bh(&pGmacDev->lock);

	// wd enable // jab enable
	// frame burst enable
#ifdef CONFIG_SDP_GMAC_GIGA_BIT
	regVal = B_FRAME_BURST_EN;  	// only GMAC, not support 10/100
#else
	regVal = B_PORT_MII;		// Port select mii
#endif
 	regVal |= B_SPEED_100M;		// 0.964
       	regVal |= B_DUPLEX_FULL;	// 0.964
	// jumbo frame disable // rx own enable // loop back off // retry enable
	// pad crc strip disable // back off limit set 0 // deferral check disable
	pGmacReg->configuration = regVal;
	DPRINTK_GMAC ("configuration set %08x\n", regVal);

// frame filter disable
#if 0
//	regVal = B_RX_ALL;
	// set pass control -> GmacPassControl 0 // broadcast enable
	// src addr filter disable // multicast disable  ????  // promisc disable
	// unicast hash table filter disable
//	pGmacReg->frameFilter = regVal;
	DPRINTK_GMAC ("frameFilter set %08x\n", regVal);
#endif

	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_FLOW ("exit\n");
}


static int
sdpGmac_setRxQptr(SDP_GMAC_DEV_T* pGmacDev, const DMA_DESC_T* pDmaDesc, const u32 length2)
{
	int retVal = SDP_GMAC_OK;

	u32 rxNext = 0xFFFFFFFF;
	DMA_DESC_T* pRxDesc = NULL;
	DMA_DESC_T rdes = {.status = 0,};

	DPRINTK_GMAC_FLOW("called \n");

	spin_lock(&pGmacDev->lock);

	rxNext = pGmacDev->rxNext;
	pRxDesc = pGmacDev->pRxDescNext;

	if(!sdpGmac_descEmpty(pRxDesc)) { // should empty
		DPRINTK_GMAC_FLOW("Error Not Empty \n");
		retVal =  -SDP_GMAC_ERR;
		goto __setRxQptr_out;
	}

	if(sdpGmac_rxDescChained(pRxDesc)){
		// TODO
		PRINTK_GMAC("rx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		goto __setRxQptr_out;

	} else {
		rdes.length = (pDmaDesc->length & DESC_SIZE_MASK);
		rdes.length |= (length2 & DESC_SIZE_MASK) << DESC_SIZE2_SHIFT;

		if(sdpGmac_lastRxDesc(pGmacDev, pRxDesc)) {
			RDES_RER_SET(rdes, 1);
		}
// set descriptor
		pRxDesc->buffer1 = pDmaDesc->buffer1;
		pRxDesc->data1 = pDmaDesc->data1;

		pRxDesc->buffer2 = pDmaDesc->buffer2;
		pRxDesc->data2 = pDmaDesc->data2;

		pRxDesc->length = rdes.length;
		wmb();				// 0.958, 0.9592
		pRxDesc->status = RDES0_OWN;
		wmb();				// 0.9592

		if(RDES_RER_GET(rdes)) {
			DPRINTK_GMAC_FLOW ("Set Last RX Desc no: %d length : 0x%08x\n",
					pGmacDev->rxNext , pRxDesc->length);
			pGmacDev->rxNext = 0;
			pGmacDev->pRxDescNext = pGmacDev->pRxDesc;
		}
		else {
			pGmacDev->rxNext = rxNext + 1;
			pGmacDev->pRxDescNext = pRxDesc + 1;
		}

	}

	retVal = (int)rxNext;
// if set rx qptr is success, print status
	DPRINTK_GMAC_FLOW("rxNext: %02d  pRxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      rxNext,  (u32)pRxDesc, pRxDesc->status,
		      pRxDesc->length, pRxDesc->buffer1, pRxDesc->buffer2,
		      pRxDesc->data1, pRxDesc->data2);
__setRxQptr_out:
	DPRINTK_GMAC("exit\n");
	spin_unlock(&pGmacDev->lock);
	return retVal;
}


// return rxIndex
static int
sdpGmac_getRxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc)
{
	int retVal = SDP_GMAC_OK;

	u32 rxBusy = 0xFFFFFFFF;
	DMA_DESC_T *pRxDesc = NULL;

	DPRINTK_GMAC_FLOW("called\n");

	spin_lock(&pGmacDev->lock);

	rxBusy = pGmacDev->rxBusy;
	pRxDesc = pGmacDev->pRxDescBusy;

	if(pRxDesc->status & RDES0_OWN) {
		DPRINTK_GMAC_FLOW("rx desc %d status is desc own by dma\n", rxBusy);
		retVal = -SDP_GMAC_ERR;
		goto __getRxQptr_out;
	}

	if(sdpGmac_descEmpty(pRxDesc)) {
		DPRINTK_GMAC_FLOW("rx desc %d status empty\n", rxBusy);
		retVal = -SDP_GMAC_ERR;
		goto __getRxQptr_out;
	}

	*pDmaDesc = *pRxDesc; // get Descriptor data

	if(sdpGmac_rxDescChained(pRxDesc)){
		PRINTK_GMAC("rx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		goto __getRxQptr_out;
	}
	else {
		if(sdpGmac_lastRxDesc(pGmacDev, pRxDesc)) {
			DPRINTK_GMAC_FLOW ("Last RX Desc\n");
			pRxDesc->status = 0;

			pRxDesc->data1 = 0;
			pRxDesc->buffer1 = 0;
			pRxDesc->data2 = 0;
			pRxDesc->buffer2 = 0;

			pRxDesc->length = 0;  // caution !!!!
			RDES_RER_SET(*pRxDesc, 1);

			pGmacDev->rxBusy = 0;
			pGmacDev->pRxDescBusy = pGmacDev->pRxDesc;

		}
		else {
			memset(pRxDesc, 0, sizeof(DMA_DESC_T));
			pGmacDev->rxBusy = rxBusy + 1;
			pGmacDev->pRxDescBusy = pRxDesc + 1;
		}
	}

	retVal = (int)rxBusy;

// if set rx qptr is success, print status
	DPRINTK_GMAC_FLOW("rxBusy: %02d  pRxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      rxBusy,  (u32)pRxDesc, pRxDesc->status,
		      pRxDesc->length, pRxDesc->buffer1, pRxDesc->buffer2,
		      pRxDesc->data1, pRxDesc->data2);

__getRxQptr_out:
	DPRINTK_GMAC("exit\n");
	spin_unlock(&pGmacDev->lock);
	return retVal;
}

#if 0/* commented 0.971 */
static int
sdpGmac_txQptr_replacement(SDP_GMAC_DEV_T* pGmacDev)
{
	u32 txNext = pGmacDev->txNext;
	u32 txBusy = pGmacDev->txBusy;
	u32 n_dmaTxCurrent;
	u32 opReg, status;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;
	DMA_DESC_T *pTxDesc, txDesc;
	int idx;

	opReg = pDmaReg->curHostTxDesc;
	status = pDmaReg->txDescListAddr;
	n_dmaTxCurrent = opReg - status;
	n_dmaTxCurrent = n_dmaTxCurrent / sizeof(DMA_DESC_T);


	if(
	   ((txNext > txBusy) && ((txNext <= n_dmaTxCurrent) || (n_dmaTxCurrent < txBusy))) || 	// 0.962	//0.965
	   ((txNext < txBusy) && ((txNext <= n_dmaTxCurrent) && (n_dmaTxCurrent < txBusy)))	// 0.962	//0.965
	  ){
		DPRINTK_GMAC_ERROR("Run Replacement txqptr \n");

		opReg = pDmaReg->operationMode;
		pDmaReg->operationMode &= ~(B_TX_EN) ;

		spin_lock_bh(&pGmacDev->lock);

		pTxDesc = pGmacDev->pTxDescBusy = pGmacDev->pTxDescNext = pGmacDev->pTxDesc + n_dmaTxCurrent;
		pGmacDev->txBusy = pGmacDev->txNext = n_dmaTxCurrent;

		for(idx = 0; idx < NUM_TX_DESC; idx++){
			if(!sdpGmac_descEmpty(pTxDesc)) {
				if(pTxDesc->status & DESC_OWN_BY_DMA) {
					if(sdpGmac_lastTxDesc(pGmacDev, pGmacDev->pTxDescNext)){
						*pGmacDev->pTxDescNext = *pTxDesc;
						pGmacDev->pTxDescNext->status |= TX_DESC_END_RING;
						pGmacDev->txNext = 0;

						pGmacDev->pTxDescNext = pGmacDev->pTxDesc;
					} else {
						*pGmacDev->pTxDescNext = *pTxDesc;
						pGmacDev->txNext++;
						pGmacDev->pTxDescNext++;
						status = 0;
					}

				} else {
					txDesc = *pTxDesc;

					if(txDesc.data1) {
						dma_unmap_single(pGmacDev->pDev, txDesc.buffer1, txDesc.length, DMA_FROM_DEVICE);
						dev_kfree_skb_irq((struct sk_buff*)txDesc.data1);
					}
				}

			}

			pTxDesc->length = 0;
			pTxDesc->data1 = 0;
			pTxDesc->buffer1 = 0;
			pTxDesc->data2 = 0;
			pTxDesc->buffer2 = 0;
			wmb();

			if(sdpGmac_lastTxDesc(pGmacDev, pTxDesc)){
				pTxDesc->status = TX_DESC_END_RING;
				pTxDesc = pGmacDev->pTxDesc;
			} else {
				pTxDesc->status = 0;
				pTxDesc++;
			}
			wmb();
		}

		spin_unlock_bh(&pGmacDev->lock);
		pDmaReg->operationMode = opReg;

		return SDP_GMAC_OK;
	} else if(txBusy == n_dmaTxCurrent){
		return SDP_GMAC_OK;	 // buffer full
	}

	DPRINTK_GMAC_ERROR("!!! DIFF: txNext : %d, dma tx : %d\n", txNext, n_dmaTxCurrent);
	DPRINTK_GMAC_ERROR("HW info: tx desc list addr : 0x%08x\n", status);
	DPRINTK_GMAC_ERROR("HW info: tx curr desc addr : 0x%08x\n", opReg);
//	DPRINTK_GMAC_ERROR("HW info: tx curr buff addr : 0x%08x\n", pDmaReg->curHostTxBufAddr);
//	DPRINTK_GMAC_ERROR("DATA info: tx desc list paddr : 0x%08x\n", pGmacDev->txDescDma);
//	DPRINTK_GMAC_ERROR("DATA info: tx desc list vaddr : 0x%08x\n", (u32)pGmacDev->pTxDesc);
//	DPRINTK_GMAC_ERROR("DATA info: tx next desc vaddr : 0x%08x\n", (u32)pGmacDev->pTxDescNext);
//	DPRINTK_GMAC_ERROR("DATA info: tx busy desc vaddr : 0x%08x\n", (u32)pGmacDev->pTxDescBusy);
	DPRINTK_GMAC_ERROR("DATA info: tx busy number : %d\n", txBusy);

	DPRINTK_GMAC_ERROR("HW info: dma status : 0x%08x\n", pDmaReg->status);			// 0.962
	DPRINTK_GMAC_ERROR("HW info: dma operation mode : 0x%08x\n", pDmaReg->operationMode);	// 0.962
	DPRINTK_GMAC_ERROR("HW info: dma miss frame buffer : 0x%08x\n", pDmaReg->missFrameBufOverCnt);	//0.962

	return  -SDP_GMAC_ERR;
}
#endif/* commented 0.971 */

static int
sdpGmac_setTxQptr(SDP_GMAC_DEV_T* pGmacDev,
		const DMA_DESC_T* pDmaDesc, const u32 length2, const int offloadNeeded)
{
	int retVal = SDP_GMAC_OK;

	u32 txNext = 0xFFFFFFFF;
	DMA_DESC_T *pTxDesc = 0;
	DMA_DESC_T td = {.status = 0,};
//	u32 length;
//	u32 status;

	DPRINTK_GMAC_FLOW("called\n");

	spin_lock(&pGmacDev->lock);

	txNext = pGmacDev->txNext;
	pTxDesc = pGmacDev->pTxDescNext;

	if(pTxDesc->status & TDES0_OWN) {
		DPRINTK_GMAC_FLOW("Err: DESC_OWN_BY_DMA\n");
		retVal = -SDP_GMAC_ERR;
		goto __setTxQptr_out;
	}

#if 0/* commented 0.971 */
	if(!sdpGmac_descEmpty(pTxDesc)){
		DPRINTK_GMAC_ERROR("pTxDesc must be empty\n");
		retVal =  -SDP_GMAC_ERR;
		goto __setTxQptr_out;
	}
#endif/* commented 0.971 */

// 0.963
	td.status = txNext + 1;
	td.status = td.status % NUM_TX_DESC;

	if(td.status == pGmacDev->txBusy){
	//	DPRINTK_GMAC_ERROR("Tx Desc is Full txNext:%d, txBusy:%d\n", txNext, pGmacDev->txBusy);
		retVal =  -SDP_GMAC_ERR;
		goto __setTxQptr_out;
	}
// 0.963 end
//


	if(sdpGmac_txDescChained(pTxDesc)) {
		// TODO: chained
		PRINTK_GMAC("tx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		spin_unlock(&pGmacDev->lock);
		goto __setTxQptr_out;
	}
	else {
		u32 updateTxNext;		// 0.965

		td.length = (pDmaDesc->length & DESC_SIZE_MASK);
		td.length |= (length2 & DESC_SIZE_MASK) << DESC_SIZE2_SHIFT;

		/* Set Buf size and Control flags */
#ifdef CONFIG_SDP_MAC_NORMAL_DESC
		td.status = TDES0_OWN;
		td.length |= TDES1_FS | TDES1_LS | TDES1_IC;
#else
		td.status = TDES0_OWN | TDES0_FS | TDES0_LS | TDES0_IC;
#endif
		if (offloadNeeded) {
			PRINTK_GMAC("H/W Checksum Not Implenedted yet\n");
		}

		if(sdpGmac_lastTxDesc(pGmacDev, pTxDesc)) {
			updateTxNext = pGmacDev->txNext + 1;	// 0.965
			if(updateTxNext != NUM_TX_DESC)
				DPRINTK_GMAC_ERROR("Tx Desc number is wrong %lu:%u\n",NUM_TX_DESC, pGmacDev->txNext);
			updateTxNext = 0;
			pGmacDev->pTxDescNext = pGmacDev->pTxDesc;
			TDES_TER_SET(td, 1);/* Set Rx End of ring */
		}
		else {
			updateTxNext = txNext + 1;		// 0.965
			pGmacDev->pTxDescNext = pTxDesc + 1;
		}

		pTxDesc->buffer1 = pDmaDesc->buffer1;
		pTxDesc->data1	= pDmaDesc->data1;
		pTxDesc->buffer2 = pDmaDesc->buffer2;
		pTxDesc->data2	= pDmaDesc->data2;
		pTxDesc->length = td.length;	// td.length		????
		wmb();						// 0.958, 0.9592
		pTxDesc->status = td.status;
		wmb();						// 0.9592
		if(pTxDesc->status != td.status) wmb();		// 0.965

		pGmacDev->txNext = updateTxNext;		// 0.965
		// ENH DESC
	}

// if set tx qptr is success, print td.status
	DPRINTK_GMAC_FLOW("txNext: %02d  pTxDesc: %08x td.status: %08x \n"
		      "td.length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      txNext,  (u32)pTxDesc, pTxDesc->status,
		      pTxDesc->length, pTxDesc->buffer1, pTxDesc->buffer2,
		      pTxDesc->data1, pTxDesc->data2);


	retVal = (int)txNext;

__setTxQptr_out:
	DPRINTK_GMAC("exit\n");
	spin_unlock(&pGmacDev->lock);
	return retVal;
}


// Read
static int
sdpGmac_getTxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc)
{
	int retVal = SDP_GMAC_OK;

	u32 txBusy = 0xFFFFFFFF;
	DMA_DESC_T *pTxDesc = NULL;

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock(&pGmacDev->lock);

	txBusy = pGmacDev->txBusy;
	pTxDesc = pGmacDev->pTxDescBusy;

	if(pTxDesc->status & TDES0_OWN) {
		DPRINTK_GMAC_FLOW("Err: TDES0_OWN\n");
		retVal = -SDP_GMAC_ERR;
		goto __getTxQptr_out;
	}

//	if(sdpGmac_descEmpty(pTxDesc)) {
	if(txBusy == pGmacDev->txNext) {
		DPRINTK_GMAC_FLOW("Err: Desc Empty\n");
		retVal = -SDP_GMAC_ERR;
		goto __getTxQptr_out;
	}

	*pDmaDesc = *pTxDesc; // copy Tx Descriptor data



	if(sdpGmac_txDescChained(pTxDesc)){
		PRINTK_GMAC("tx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		spin_unlock(&pGmacDev->lock);
		goto __getTxQptr_out;
	}
	else {
		u32 updateTxBusy;		//0.965

		if(sdpGmac_lastTxDesc(pGmacDev, pTxDesc)) {
			DPRINTK_GMAC_FLOW ("Get desc last Tx desc\n");
			updateTxBusy = 0;	//0.965
			pGmacDev->pTxDescBusy = pGmacDev->pTxDesc;
		}
		else {
			updateTxBusy = txBusy + 1;	//0.965
			pGmacDev->pTxDescBusy = pTxDesc + 1;
		}
		pGmacDev->txBusy = updateTxBusy;	//0.965
	}

	retVal = (int)txBusy;

// if set rx qptr is success, print status
	DPRINTK_GMAC_FLOW("txBusy: %02d  pTxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      txBusy,  (u32)pTxDesc, pTxDesc->status,
		      pTxDesc->length, pTxDesc->buffer1, pTxDesc->buffer2,
		      pTxDesc->data1, pTxDesc->data2);

__getTxQptr_out:
	DPRINTK_GMAC_FLOW("exit\n");
	spin_unlock(&pGmacDev->lock);
	return retVal;
}


static void
sdpGmac_clearAllDesc(struct net_device *pNetDev)
{

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	DMA_DESC_T *pDmaDesc;
	u32 length1, length2;
	int i;
	u32 txBusy;



// RX DESC Clear
	pDmaDesc = pGmacDev->pRxDesc;

	for(i = 0; i < NUM_RX_DESC; i ++){
		length1 = DESC_GET_SIZE1(pDmaDesc->length);
		length2 = DESC_GET_SIZE2(pDmaDesc->length);

		if((length1) && (pDmaDesc->data1)){
			DPRINTK_GMAC("rx desc %d clear length %x, data1 %x\n",
				 i, length1, pDmaDesc->data1);
			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer1,
					length1, DMA_FROM_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data1);
		}

		if((length2) && (pDmaDesc->data2)){
			DPRINTK_GMAC("rx desc2 %d clear length2 %x, data2 %x\n",
				 i, length2, pDmaDesc->data2);
			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer2,
					length2, DMA_FROM_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data2);
		}

		pDmaDesc->length = 0;
		pDmaDesc++;

	}

	RDES_RER_SET(pGmacDev->pRxDesc[NUM_RX_DESC-1], 1);
	DPRINTK_GMAC_FLOW("last rx desc length is 0x%08x\n", pGmacDev->pRxDesc[NUM_RX_DESC-1].length);

	pGmacDev->rxNext = 0;
	pGmacDev->rxBusy = 0;
	pGmacDev->pRxDescBusy = pGmacDev->pRxDesc;
	pGmacDev->pRxDescNext = pGmacDev->pRxDesc;

// TX DESC Clear
	pDmaDesc = pGmacDev->pTxDesc;
	txBusy = pGmacDev->txBusy;


	for(i = 0; i < NUM_TX_DESC; i ++){

		if(txBusy == pGmacDev->txNext) break;
		pDmaDesc = pGmacDev->pTxDesc + txBusy;

		length1 = DESC_GET_SIZE1(pDmaDesc->length);
		length2 = DESC_GET_SIZE2(pDmaDesc->length);

		if((length1) && (pDmaDesc->data1)){
			DPRINTK_GMAC("tx desc %d clear length %x, data1 %x\n",
				 txBusy, length1, pDmaDesc->data1);

			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer1,
					length1, DMA_TO_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data1);
		}

		if((length2) && (pDmaDesc->data2)){
			DPRINTK_GMAC("tx desc2 %d clear length2 %x, data2 %x\n",
				 txBusy, length2, pDmaDesc->data2);

			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer2,
					length2, DMA_TO_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data2);
		}

		pDmaDesc->status = 0;
		pDmaDesc->length = 0;
//		pDmaDesc++;
		txBusy++;
		if(txBusy >= NUM_TX_DESC) {
			TDES_TER_SET(pGmacDev->pTxDesc[NUM_TX_DESC-1], 1);
			txBusy = 0;
		}
	}

	pGmacDev->txNext = 0;
	pGmacDev->txBusy = 0;
	pGmacDev->pTxDescBusy = pGmacDev->pTxDesc;
	pGmacDev->pTxDescNext = pGmacDev->pTxDesc;

	return;
}

static struct net_device_stats *
sdpGmac_netDev_getStats(struct net_device *pNetDev)
{
	struct net_device_stats *pRet;
	SDP_GMAC_DEV_T * pGmacDev = netdev_priv(pNetDev);

	DPRINTK_GMAC_FLOW("called\n");

	pRet = &pGmacDev->netStats;

	DPRINTK_GMAC_FLOW("exit\n");
	return pRet;
}


static int
sdpGmac_netDev_hardStartXmit(struct sk_buff *pSkb, struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	int offloadNeeded = 0;
	dma_addr_t txDmaAddr;
	DMA_DESC_T txDesc;

	int retry = 1;

	DPRINTK_GMAC_FLOW ("called\n");

	if(unlikely(pSkb == NULL)) {
		DPRINTK_GMAC_ERROR("xmit skb is NULL!!!\n");
		retVal = -SDP_GMAC_ERR;
		goto __xmit_out;
	}

	while((pGmacDev->txNext+1)%NUM_TX_DESC == pGmacDev->txBusy) {
		if (!netif_queue_stopped(pNetDev)) {
			netif_stop_queue(pNetDev);
		}
		if (netif_msg_tx_queued(pGmacDev)) {
			dev_printk(KERN_DEBUG, pGmacDev->pDev, "total retry %d times. tx desc is Full. txBusy %d, txNext %d\n", retry, pGmacDev->txBusy, pGmacDev->txNext);
		}
		retVal = NETDEV_TX_BUSY;
		goto __xmit_out;
	}

	if(pSkb->ip_summed == CHECKSUM_PARTIAL){
		// TODO:
		DPRINTK_GMAC_ERROR("H/W Checksum?? Not Implemente yet\n");
		offloadNeeded = 1;
	}

	txDmaAddr = dma_map_single(pGmacDev->pDev, pSkb->data,
					 pSkb->len, DMA_TO_DEVICE);

			//        length,  buffer1, data1, buffer2, data2
	CONVERT_DMA_QPTR(txDesc, pSkb->len, txDmaAddr, pSkb, 0, 0 );

	while(sdpGmac_setTxQptr(pGmacDev, &txDesc, 0, offloadNeeded) < 0) {
		if(retry > 0){
			retry--;
			udelay(300);		// 100MBps 1 packet 1515 byte -> 121.20 us
			continue;
		}
#if 0/* commented 0.971 */
		if(sdpGmac_txQptr_replacement(pGmacDev) < 0) { 	// 0.962
			DPRINTK_GMAC_ERROR("If network device  not work after this message,   \n");
			DPRINTK_GMAC_ERROR("Send log and Please call tukho.kim@samsung.com #0717: Tx DMA HANG \n");
		} else {
			DPRINTK_GMAC_ERROR("Tx Desc Full or Tx Descriptor replacement\n"); // 0.963
			udelay(150);		// 1 packet 1515 byte -> 121.20 us
			continue;
		}
#endif

		DPRINTK_GMAC_ERROR("Set Tx Descriptor is Failed\n");		// 0.963
		DPRINTK_GMAC_ERROR("No more Free Tx Descriptor\n");

		pGmacDev->netStats.tx_errors++;
		pGmacDev->netStats.tx_dropped++;
		dev_kfree_skb(pSkb);
//		break;			// 0.943
		goto __xmit_out;
	}

	/* start tx */
	pDmaReg->txPollDemand = 0;
	pNetDev->trans_start = jiffies;

__xmit_out:

	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;
}


static void
sdp_mac_adjust_link(struct net_device *dev)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(dev);
	SDP_GMAC_DMA_T 	*pDmaReg = pGmacDev->pDmaBase;
	SDP_GMAC_T 	*pGmacReg = pGmacDev->pGmacBase;
	struct phy_device *phydev = dev->phydev;
	int new_state = 0;
	u32 idx;

	if (phydev == NULL)
		return;

	spin_lock_bh(&pGmacDev->lock);
	if (phydev->link) {
		u32 ctrl = pGmacDev->pGmacBase->configuration;
		wmb();

		pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN);
		pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);

		/* Now we make sure that we can be in full duplex mode.
		 * If not, we operate in half-duplex mode. */
		if (phydev->duplex != pGmacDev->hwDuplex) {
			new_state = 1;
			if (!(phydev->duplex))
				ctrl &= ~B_DUPLEX_FULL;
			else
				ctrl |= B_DUPLEX_FULL;
			pGmacDev->hwDuplex = phydev->duplex;
		}
		/* Flow Control operation */
		if ((pGmacDev->flow_ctrl != SDP_MAC_FLOW_OFF) && phydev->pause)
			dev_warn(&dev->dev, "Not support Flow Control\n");

		if (phydev->speed != pGmacDev->hwSpeed) {
			new_state = 1;
			switch (phydev->speed) {
			case 1000:
				if (likely(pGmacDev->has_gmac))
					ctrl &= ~B_PORT_MII;
				break;
			case 100:
			case 10:
				if (pGmacDev->has_gmac) {
					ctrl |= B_PORT_MII;
					if (phydev->speed == SPEED_100) {
						ctrl |= B_SPEED_100M;
					} else {
						ctrl &= ~B_SPEED_100M;
					}
				} else {
					ctrl &= ~B_PORT_MII;
				}
				break;
			default:
				if (netif_msg_link(pGmacDev))
					dev_warn(&dev->dev, "Speed (%d) is not 10"
				       " or 100!\n", phydev->speed);
				break;
			}

			pGmacDev->hwSpeed = phydev->speed;
		}


		if(new_state) {
			// desc pointer set by DMA desc pointer
			// RX Desc
			idx = (pDmaReg->curHostRxDesc - pDmaReg->rxDescListAddr) / sizeof(DMA_DESC_T);
			if(idx < NUM_RX_DESC) {
				pGmacDev->rxNext = idx;
				pGmacDev->pRxDescNext = pGmacDev->pRxDesc + idx;
				pGmacDev->rxBusy = idx;
				pGmacDev->pRxDescBusy = pGmacDev->pRxDesc + idx;
			}
			// TX Desc
			idx = (pDmaReg->curHostTxDesc - pDmaReg->txDescListAddr) / sizeof(DMA_DESC_T);
			if(idx < NUM_TX_DESC) {
				pGmacDev->txNext = idx;
				pGmacDev->pTxDescNext = pGmacDev->pTxDesc + idx;
				pGmacDev->txBusy = idx;
				pGmacDev->pTxDescBusy = pGmacDev->pTxDesc + idx;
			}
		}
		pGmacDev->pGmacBase->configuration = ctrl;
		wmb();

		pDmaReg->operationMode |= (B_TX_EN | B_RX_EN);

		if (!pGmacDev->oldlink) {
			new_state = 1;
			pGmacDev->oldlink = 1;
		}
	} else if (pGmacDev->oldlink) {
		new_state = 1;
		pGmacDev->oldlink = 0;
		pGmacDev->hwSpeed = 0;
		pGmacDev->hwDuplex= 0;
	}

	spin_unlock_bh(&pGmacDev->lock);

	if (new_state && netif_msg_link(pGmacDev))
		phy_print_status(phydev);

}

static int
sdpGmac_netDev_open (struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_MMC_T *pMmcReg = pGmacDev->pMmcBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	struct sk_buff *pSkb;
	dma_addr_t  	rxDmaAddr;
	DMA_DESC_T	rxInitDesc;
	u32	idx;

	u32 len;

	DPRINTK_GMAC_FLOW("%s: open called\n", pNetDev->name);

	if(!is_valid_ether_addr(pNetDev->dev_addr)) {
		dev_err(&pNetDev->dev, "no valid ethernet hw addr\n");
		return -EINVAL;
	}

	//init Phy. use phy subsystem.
	{
		struct phy_device *phydev = pGmacDev->phydev;
		phy_interface_t phyinterface = sdp_mac_get_interface(pNetDev);
		int ret = 0;

		dev_dbg(pGmacDev->pDev, "Init Phy!!\n");

		if(!phydev) {
			dev_err(&pNetDev->dev, "Could not find to PHY\n");
			return -ENODEV;
		}

		ret = phy_connect_direct(pNetDev, phydev, sdp_mac_adjust_link, 0, phyinterface);
		if (ret < 0) {
			dev_err(&pNetDev->dev, "Could not attach to PHY\n");
			return PTR_ERR(phydev);
		}
		/* Stop Advertising 1000BASE Capability if interface is not GMII */
		if ((phyinterface == PHY_INTERFACE_MODE_MII) ||
		    (phyinterface == PHY_INTERFACE_MODE_RMII))
			phydev->advertising &= ~(u32)(SUPPORTED_1000baseT_Half |
						 SUPPORTED_1000baseT_Full);

		/*
		 * Broken HW is sometimes missing the pull-up resistor on the
		 * MDIO line, which results in reads to non-existent devices returning
		 * 0 rather than 0xffff. Catch this here and treat 0 as a non-existent
		 * device as well.
		 * Note: phydev->phy_id is the result of reading the UID PHY registers.
		 */
		if (phydev->phy_id == 0) {
			dev_err(&pNetDev->dev, "Could not attach to PHY(phy id is 0x0)\n");
			phy_disconnect(phydev);
			return -ENODEV;
		}

		phy_stop(phydev);

		dev_dbg(pGmacDev->pDev, "attached to PHY (UID 0x%x)"
			 " Link = %d\n", phydev->phy_id, phydev->link);
	}

	// desc pointer set by DMA desc pointer
	// RX Desc
	idx = (pDmaReg->curHostRxDesc - pDmaReg->rxDescListAddr) / sizeof(DMA_DESC_T);
	if(idx < NUM_RX_DESC) {
		pGmacDev->rxNext = idx;
		pGmacDev->pRxDescNext = pGmacDev->pRxDesc + idx;
		pGmacDev->rxBusy = idx;
		pGmacDev->pRxDescBusy = pGmacDev->pRxDesc + idx;
		DPRINTK_GMAC_FLOW("Rx Desc set %d\n", idx);
	}
	// TX Desc
	idx = (pDmaReg->curHostTxDesc - pDmaReg->txDescListAddr) / sizeof(DMA_DESC_T);
	if(idx < NUM_TX_DESC) {
		pGmacDev->txNext = idx;
		pGmacDev->pTxDescNext = pGmacDev->pTxDesc + idx;
		pGmacDev->txBusy = idx;
		pGmacDev->pTxDescBusy = pGmacDev->pTxDesc + idx;
		DPRINTK_GMAC_FLOW("Tx Desc set %d\n", idx);
	}

	len = ETH_DATA_LEN + ETHER_HEADER + ETHER_CRC + 4;
	len += ((SDP_GMAC_BUS >> 3) - 1);
	len &= SDP_GMAC_BUS_MASK;
	//skb buffer initialize ????	check
	do{
//		pSkb = dev_alloc_skb(pNetDev->mtu + ETHER_HEADER + ETHER_CRC);
		pSkb = dev_alloc_skb(len);	// 0.931 // 0.954

		if(pSkb == NULL){
                        DPRINTK_GMAC_ERROR("can't allocate sk buffer \n");
                        break;
		}

		sdpGmac_skbAlign(pSkb); 	// 0.954

		rxDmaAddr = dma_map_single(pGmacDev->pDev, pSkb->data,
					 (size_t)skb_tailroom(pSkb), DMA_FROM_DEVICE);

//		CONVERT_DMA_QPTR(rxInitDesc, skb_tailroom(pSkb), rxDmaAddr, pSkb, 0, 0);
		CONVERT_DMA_QPTR(rxInitDesc, len, rxDmaAddr, pSkb, 0, 0);

		if(sdpGmac_setRxQptr(pGmacDev, &rxInitDesc, 0) < 0){
			dev_kfree_skb(pSkb);
			break;
		}
	}while(1);

//	pGmacReg->configuration |= B_JUMBO_FRAME_EN ;
	pGmacReg->configuration |= B_WATCHDOG_DISABLE ; // 0.942

	//  MMC interrupt disable all
	//  TODO: check this block what use
	pMmcReg->intrMaskRx = 0x00FFFFFF;  // disable
	pMmcReg->intrMaskTx = 0x01FFFFFF;  // disable
	pMmcReg->mmcIpcIntrMaskRx = 0x3FFF3FFF;  // disable

	//  interrupt enable all
	pDmaReg->intrEnable = B_INTR_ENABLE_ALL;

	// tx, rx enable
	pGmacReg->configuration |= B_TX_ENABLE | B_RX_ENABLE;
	pDmaReg->operationMode |= B_TX_EN | B_RX_EN ;
	pGmacDev->is_rx_stop = false;

	if (pNetDev->phydev)
		phy_start(pNetDev->phydev);

	//napi enable
	napi_enable(&pGmacDev->napi);

	netif_start_queue(pNetDev);

	enable_irq(pNetDev->irq);

	DPRINTK_GMAC_FLOW("%s: exit\n", pNetDev->name);

	return retVal;
}

static int
sdpGmac_netDev_close (struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	DPRINTK_GMAC_FLOW("%s: close called\n", pNetDev->name);

	disable_irq(pNetDev->irq);

	netif_stop_queue(pNetDev);
	netif_carrier_off(pNetDev);

	/* napi */
	napi_disable(&pGmacDev->napi);

	// rx, tx disable
	pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN) ;
	pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);
	pGmacDev->is_rx_stop = true;

	// all interrupt disable
	pDmaReg->intrEnable = 0;
	pDmaReg->status = pDmaReg->status;	// Clear interrupt pending register

	// phy control
	if (pNetDev->phydev) {
		phy_stop(pNetDev->phydev);
		dev_dbg(pGmacDev->pDev, "disconnect to PHY (UID 0x%x)"
			 " Link = %d\n", pNetDev->phydev->phy_id, pNetDev->phydev->link);
		phy_disconnect(pNetDev->phydev);
	}

	// skb control
	sdpGmac_clearAllDesc(pNetDev);

	DPRINTK_GMAC_FLOW("%s: close exit\n", pNetDev->name);
	return retVal;
}

static void
sdpGmac_netDev_setMulticastList (struct net_device *pNetDev)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;

	u32 frameFilter;

	DPRINTK_GMAC_FLOW("called\n");

	spin_lock_bh(&pGmacDev->lock);
	frameFilter = pGmacReg->frameFilter;
	spin_unlock_bh(&pGmacDev->lock);

	frameFilter &= ~( B_PROMISCUOUS_EN |
			  B_PASS_ALL_MULTICAST |
			  B_HASH_PERFECT_FILTER |
			  B_HASH_MULTICAST );

	if (pNetDev->flags & IFF_PROMISC) {
		PRINTK_GMAC ("%s: PROMISCUOUS MODE\n", pNetDev->name);
		frameFilter |= B_PROMISCUOUS_EN;
	}

#ifdef CONFIG_ARCH_SDP1002
	else if(pNetDev->flags & IFF_ALLMULTI ||
			(netdev_mc_count(pNetDev) > 14)) {
#else
	else if(pNetDev->flags & IFF_ALLMULTI ||
			(netdev_mc_count(pNetDev) > 15)) {
#endif
		DPRINTK_GMAC_FLOW("%s: PASS ALL MULTICAST \n", pNetDev->name);
		frameFilter |= B_PASS_ALL_MULTICAST;
	}

	else if(netdev_mc_count(pNetDev)){

		int i;
		struct netdev_hw_addr *pred;
		volatile u32 *mcRegHigh = &pGmacReg->macAddr_01_High;
		volatile u32 *mcRegLow = &pGmacReg->macAddr_01_Low;
		u32 *mcValHigh;
		u32 *mcValLow;

		DPRINTK_GMAC_FLOW ("%s: HASH MULTICAST\n",
					pNetDev->name);

		// clear mc list
		i = 15;
		while (i--) {
			if(*mcRegHigh == 0) break;

			spin_lock_bh(&pGmacDev->lock);
			*mcRegHigh = 0;
			wmb();
			udelay(1);
			*mcRegLow = 0;
			wmb();
			spin_unlock_bh(&pGmacDev->lock);

			mcRegLow += 2;
			mcRegHigh += 2;
		}

		// set
		mcRegHigh = &pGmacReg->macAddr_01_High;
		mcRegLow = &pGmacReg->macAddr_01_Low;

		netdev_hw_addr_list_for_each(pred, &pNetDev->mc) {

			//DPRINTK_GMAC_FLOW("%s: cur_addr len is %d \n", pNetDev->name, cur_addr->dmi_addrlen);
			DPRINTK_GMAC_FLOW("%s: cur_addr is %d.%d.%d.%d.%d.%d \n", pNetDev->name,
						pred->addr[0], pred->addr[1],
						pred->addr[2], pred->addr[3],
						pred->addr[4], pred->addr[5] );

			if(!(*pred->addr & 1)) continue;

			mcValLow = (u32*)(pred->addr);
			mcValHigh = mcValLow + 1;	// u32 pointer

			spin_lock_bh(&pGmacDev->lock);
			*mcRegHigh = 0x80000000 | (*mcValHigh & 0xFFFF);
			wmb();
			udelay(1);
			*mcRegLow = *mcValLow;
			wmb();
			spin_unlock_bh(&pGmacDev->lock);

			mcRegHigh += 2;
			mcRegLow += 2;

		}
	}
	else {	// clear
		int i;
		volatile u32 *mcRegHigh = &pGmacReg->macAddr_01_High;
		volatile u32 *mcRegLow = &pGmacReg->macAddr_01_Low;

		// clear mc list
		i = 15;
		while (i--) {
			if(*mcRegHigh == 0) break;

			spin_lock_bh(&pGmacDev->lock);
			*mcRegHigh = 0;
			wmb();
			udelay(1);
			*mcRegLow = 0;
			wmb();
			spin_unlock_bh(&pGmacDev->lock);

			mcRegLow += 2;
			mcRegHigh += 2;
		}

	}


	spin_lock_bh(&pGmacDev->lock);
	pGmacReg->frameFilter = frameFilter;
	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_FLOW("exit\n");
	return;
}


static int
sdpGmac_netDev_setMacAddr (struct net_device *pNetDev, void *pEthAddr)
{
	int retVal = SDP_GMAC_OK;

	u8 *pAddr = (u8*)pEthAddr;

	DPRINTK_GMAC_FLOW("called\n");

	pAddr += 2;	// Etheret Mac is 6

	sdpGmac_setMacAddr(pNetDev, (const u8*)pAddr);
	sdpGmac_getMacAddr(pNetDev, (u8*)pAddr);

	PRINTK_GMAC ("%s: Ethernet address %02x:%02x:%02x:%02x:%02x:%02x\n",
		     pNetDev->name, *pAddr, *(pAddr+1), *(pAddr+2),
				*(pAddr+3), *(pAddr+4), *(pAddr+5));

	memcpy(pNetDev->dev_addr, pAddr, N_MAC_ADDR);

	DPRINTK_GMAC_FLOW("exit\n");
	return retVal;
}

static int
sdpGmac_netDev_ioctl (struct net_device *pNetDev, struct ifreq *pRq, int cmd)
{
	int retVal = SDP_GMAC_OK;
//	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);

	DPRINTK_GMAC_FLOW("called\n");

	if(!netif_running(pNetDev))
		return -EINVAL;

//	spin_lock_bh(&pGmacDev->lock);

//	retVal = generic_mii_ioctl(&pGmacDev->mii, if_mii(pRq), cmd, NULL);
	retVal = phy_mii_ioctl(pNetDev->phydev, pRq, cmd);

//	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_FLOW("exit\n");

	return retVal;
}


/*
 *  sdpGmac poll func for NAPI
 *  @napi : pointer to the napi structure.
 *  @budget : maximum number of packets that the current CPU can receive from
 *	      all interfaces.
 *  Description :
 *   This function implements the the reception process.
 *   Also it runs the TX completion thread
 */
static int sdpGmac_netDev_poll(struct napi_struct *napi, int budget)
{
	SDP_GMAC_DEV_T *pGmacDev = container_of(napi, SDP_GMAC_DEV_T, napi);
	int work_done = 0;

	dev_dbg(pGmacDev->pDev, "CPU%d, sdpGmac_netDev_poll Enter!\n", smp_processor_id());

	sdpGmac_netDev_tx(pGmacDev->pNetDev, budget);

	work_done = sdpGmac_netDev_rx(pGmacDev->pNetDev, budget);

	if (work_done < budget) {
		napi_complete(napi);
		pGmacDev->pDmaBase->intrEnable |= (B_RX_INTR | B_TX_INTR);
		dev_dbg(pGmacDev->pDev, "CPU%d, NAPI Complete workdone=%d\n", smp_processor_id(), work_done);
	}

	dev_dbg(pGmacDev->pDev, "CPU%d, sdpGmac_netDev_poll Exit!\n", smp_processor_id());

	return work_done;
}


#ifdef CONFIG_NET_POLL_CONTROLLER
/* Polling receive - used by NETCONSOLE and other diagnostic tools
 * to allow network I/O with interrupts disabled. */
static void
sdpGmac_netDev_poll_controller(struct net_device *dev)
{
	disable_irq(dev->irq);
	sdpGmac_intrHandler(dev->irq, dev);
	enable_irq(dev->irq);
}
#endif

// TODO:
#if 0
static int
sdpGmac_netDev_chgMtu(struct net_device *pNetDev, int newMtu)
{
	int retVal = SDP_GMAC_OK;

	DPRINTK_GMAC("called\n");

//  TODO

	DPRINTK_GMAC("exit\n");
	return retVal;
}


static void
sdpGmac_netDev_txTimeOut (struct net_device *pNetDev)
{
	DPRINTK_GMAC("called\n");

//  TODO

	DPRINTK_GMAC("exit\n");
	return;
}
#endif


/*
 * Ethernet Tool Support
 */
static int
sdpGmac_ethtool_getsettings (struct net_device *pNetDev, struct ethtool_cmd *pCmd)
{
	int retVal;

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);

	DPRINTK_GMAC("called\n");

	if(pNetDev->phydev == NULL) {
		dev_err(pGmacDev->pDev, "PHY is not registered.\n");
		return -ENODEV;
	}

	pCmd->maxtxpkt = 1;	// ????
	pCmd->maxrxpkt = 1;	// ????

//	spin_lock_bh(&pGmacDev->lock);
	retVal = phy_ethtool_gset(pNetDev->phydev, pCmd);

//	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC("exit\n");
	return retVal;
}

static int
sdpGmac_ethtool_setsettings (struct net_device *pNetDev, struct ethtool_cmd *pCmd)
{
	int retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);

	DPRINTK_GMAC_FLOW("called\n");

	if(pNetDev->phydev == NULL) {
		dev_err(pGmacDev->pDev, "PHY is not registered.\n");
		return -ENODEV;
	}

//	spin_lock_bh(&pGmacDev->lock);
	retVal = phy_ethtool_sset(pNetDev->phydev, pCmd);
//	spin_unlock_bh(&pGmacDev->lock);

	DPRINTK_GMAC_FLOW("exit\n");
	return retVal;
}

static void
sdpGmac_ethtool_getdrvinfo (struct net_device *pNetDev, struct ethtool_drvinfo *pDrvInfo)
{

	DPRINTK_GMAC("called\n");
		memset(pDrvInfo, 0, sizeof(*pDrvInfo));
        strncpy(pDrvInfo->driver, ETHER_NAME, sizeof(pDrvInfo->driver)-1);
		strncpy(pDrvInfo->version, GMAC_DRV_VERSION, sizeof(pDrvInfo->version)-1);
        strncpy(pDrvInfo->fw_version, "N/A", sizeof(pDrvInfo->fw_version)-1);
        strncpy(pDrvInfo->bus_info, dev_name(pNetDev->dev.parent), sizeof(pDrvInfo->bus_info)-1);

	DPRINTK_GMAC("exit\n");
	return;
}

static u32
sdpGmac_ethtool_getmsglevel (struct net_device *pNetDev)
{
	u32 retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	DPRINTK_GMAC("called\n");

	retVal = pGmacDev->msg_enable;

	DPRINTK_GMAC("exit\n");
	return retVal;
}

static void
sdpGmac_ethtool_setmsglevel (struct net_device *pNetDev, u32 level)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	DPRINTK_GMAC("called\n");

	pGmacDev->msg_enable = level;

	DPRINTK_GMAC("exit\n");

	return;
}



// phy reset
static int
sdpGmac_ethtool_nwayreset (struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);

	DPRINTK_GMAC("called\n");

	if(pNetDev->phydev == NULL) {
		dev_err(pGmacDev->pDev, "PHY is not registered.\n");
		return -ENODEV;
	}

	retVal = genphy_restart_aneg(pNetDev->phydev);

	DPRINTK_GMAC("exit\n");
	return retVal;
}


/* number of registers GMAC + MII */
static int
sdpGmac_ethtool_getregslen (struct net_device *pNetDev)
{
	int retVal = 0;

	DPRINTK_GMAC("called\n");

	retVal = (int)sizeof(SDP_GMAC_T);
	retVal += (int)sizeof(SDP_GMAC_MMC_T);
	retVal += (int)sizeof(SDP_GMAC_TIME_STAMP_T);
	retVal += (int)sizeof(SDP_GMAC_MAC_2ND_BLOCK_T);
	retVal += (int)sizeof(SDP_GMAC_DMA_T);
	retVal += (int)(32 << 2);  // MII address

	DPRINTK_GMAC("exit\n");

	return retVal;
}

/* get all registers value GMAC + MII */
static void
sdpGmac_ethtool_getregs (struct net_device *pNetDev, struct ethtool_regs *pRegs, void *pBuf)
{

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	volatile u32 *pRegAddr;
	u32 *pData = (u32*) pBuf;
	u16 phyVal;
	unsigned int i, j = 0;

	DPRINTK_GMAC_FLOW("called\n");

	pRegAddr = (volatile u32*)pGmacDev->pGmacBase;
	for(i = 0; i < (sizeof(SDP_GMAC_T) >> 2); i++)
		pData[j++] = *pRegAddr++;

	pRegAddr = (volatile u32*)pGmacDev->pMmcBase;
	for(i = 0; i < (sizeof(SDP_GMAC_MMC_T) >> 2); i++)
		pData[j++] = *pRegAddr++;

	pRegAddr = (volatile u32*)pGmacDev->pTimeStampBase;
	for(i = 0; i < (sizeof(SDP_GMAC_TIME_STAMP_T) >> 2); i++)
		pData[j++] = *pRegAddr++;

	pRegAddr = (volatile u32*)pGmacDev->pDmaBase;
	for(i = 0; i < (sizeof(SDP_GMAC_DMA_T) >> 2); i++)
		pData[j++] = *pRegAddr++;

	for(i = 0; i < 32; i++){
		phyVal = (u16)phy_read(pNetDev->phydev, i);
		pData[j++] = phyVal & 0xFFFF;
	}

	DPRINTK_GMAC_FLOW("exit\n");

	return;
}


enum sdp_mac_self_test_num {
	SDP_MAC_LB,
	SDP_PHY_LB,
	SDP_EXT_LB,
	SDP_END_LB,
};
static char sdp_mac_ethtool_self_test_string[][ETH_GSTRING_LEN] = {
#if 0
	{"link status test"},
#endif
	{"mac loopback test"},
	{"phy loopback test"},
	{"external loopback test"},
};

static int
sdp_mac_ethtool_get_sset_count(struct net_device *pNetDev, int sset_id)
{
	if(sset_id == ETH_SS_TEST) {
		return ARRAY_SIZE(sdp_mac_ethtool_self_test_string);
	}
	return -ENOTSUPP;
}

static void
sdp_mac_ethtool_get_strings(struct net_device *pNetDev, u32 stringset, u8 *data)
{
	unsigned int i;
	if(stringset == ETH_SS_TEST) {
		for(i = 0; i < ARRAY_SIZE(sdp_mac_ethtool_self_test_string); i++) {
			strncpy(data+(i*ETH_GSTRING_LEN), sdp_mac_ethtool_self_test_string[i], ETH_GSTRING_LEN);
		}
	}
}

static int
sdp_mac_setup_desc(SDP_GMAC_DEV_T *pGmacDev)
{
	struct sk_buff *pSkb = NULL;
	DMA_DESC_T dmaDesc;
	dma_addr_t dmaAddr;
	unsigned int alloc_len = ETH_FRAME_LEN + ETH_FCS_LEN + (SDP_GMAC_BUS >> 3);
	int err;
	unsigned int cnt;


	alloc_len += ((SDP_GMAC_BUS >> 3) - 1);
	alloc_len &= SDP_GMAC_BUS_MASK;

	//setup tx data
	pSkb = alloc_skb(alloc_len, GFP_KERNEL);

	if(pSkb == NULL){
		return -ENOMEM;
	}

	skb_put(pSkb, ETH_DATA_LEN);

	memset(pSkb->data, 0x5A, pSkb->len);
	for(cnt = 0; cnt < pSkb->len; cnt++)
		pSkb->data[cnt] = (unsigned char)(cnt & 0xFF);

	dmaAddr = dma_map_single(pGmacDev->pDev, pSkb->data,
					pSkb->len, DMA_TO_DEVICE);

	CONVERT_DMA_QPTR(dmaDesc, pSkb->len, dmaAddr, pSkb, 0, 0 );
	err = sdpGmac_setTxQptr(pGmacDev, &dmaDesc, 0, 0);
	if(err < 0) {
		dev_err(pGmacDev->pDev, "sdpGmac_setTxQptr error(%d)!\n", err);
	}


	//setup rx data
	pSkb = dev_alloc_skb(alloc_len);

	if(pSkb == NULL){
		return -ENOMEM;
	}

	memset(pSkb->data, 0xA5, ETH_FRAME_LEN);

	dmaAddr = dma_map_single(pGmacDev->pDev, pSkb->data,
					 ETH_FRAME_LEN, DMA_FROM_DEVICE);

	CONVERT_DMA_QPTR(dmaDesc, ETH_FRAME_LEN, dmaAddr, pSkb, 0, 0 );
	err = sdpGmac_setRxQptr(pGmacDev, &dmaDesc, 0);
	if(err < 0) {
		dev_err(pGmacDev->pDev, "sdpGmac_setRxQptr error(%d)!\n", err);
	}

	return 0;
}

static int
sdp_mac_clean_desc(SDP_GMAC_DEV_T *pGmacDev)
{
	struct net_device *pNetDev = pGmacDev->pNetDev;
	sdpGmac_clearAllDesc(pNetDev);
	return 0;
}

static int
sdp_mac_compare_frame(SDP_GMAC_DEV_T *pGmacDev, const char *header)
{
	struct sk_buff *pSkb_tx = NULL, *pSkb_rx = NULL;
	DMA_DESC_T dmaDesc_tx, dmaDesc_rx;
	int err;

	if(header == NULL) {
		header = "";
	}

	err = sdpGmac_getTxQptr(pGmacDev, &dmaDesc_tx);
	if(err < 0) {
		dev_err(pGmacDev->pDev, "tx no more frame\n");
		goto err0;
	}

	dev_dbg(pGmacDev->pDev, "get Tx frame!\n");
	pSkb_tx = (struct sk_buff *)dmaDesc_tx.data1;
	dma_unmap_single(pGmacDev->pDev, dmaDesc_tx.buffer1, pSkb_tx->len, DMA_TO_DEVICE);

	err = sdpGmac_getRxQptr(pGmacDev, &dmaDesc_rx);
	if(err < 0) {
		dev_err(pGmacDev->pDev, "rx no more frame\n");
		goto err0;
	}

	dev_dbg(pGmacDev->pDev, "get Rx frame!\n");
	pSkb_rx = (struct sk_buff *)dmaDesc_rx.data1;

	skb_put(pSkb_rx, RDES0_FL_GET(dmaDesc_rx.status));

	dma_unmap_single(pGmacDev->pDev, dmaDesc_rx.buffer1, RDES0_FL_GET(dmaDesc_rx.status), DMA_FROM_DEVICE);

	//print_hex_dump_bytes("Tx: ", DUMP_PREFIX_ADDRESS, pSkb_tx->data, pSkb_tx->len);
	//print_hex_dump_bytes("Rx: ", DUMP_PREFIX_ADDRESS, pSkb_rx->data, pSkb_rx->len);

	err = memcmp(pSkb_rx->data, pSkb_tx->data, pSkb_tx->len);
	if(err) {
		dev_err(pGmacDev->pDev, "%s data missmatch!!\n", header);
		dev_err(pGmacDev->pDev,
			"tx desc status %#x, rx desc status %#x, txlen %d rxlen %d\n",
			dmaDesc_tx.status, dmaDesc_rx.status, pSkb_tx->len, pSkb_rx->len);

		dev_err(pGmacDev->pDev, "Tx error(%s%s%s%s)\n",
			dmaDesc_tx.status&TDES0_LC?"LC ":"",
			dmaDesc_tx.status&TDES0_NC?"NC ":"",
			dmaDesc_tx.status&TDES0_LCO?"LCO ":"",
			dmaDesc_tx.status&TDES0_EC?"EC ":"");

		dev_err(pGmacDev->pDev, "Rx error(%s%s%s%s%s%s)\n",
			dmaDesc_rx.status&RDES0_LE ? "LE ":"",
			dmaDesc_rx.status&RDES0_OE ? "OE ":"",
			dmaDesc_rx.status&RDES0_LC ? "LC ":"",
			dmaDesc_rx.status&RDES0_RWT ? "RWT ":"",
			dmaDesc_rx.status&RDES0_DBE ? "DBE ":"",
			dmaDesc_rx.status&RDES0_CE ? "CE ":"");

		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);

		print_hex_dump(KERN_ERR, "Tx: ", DUMP_PREFIX_ADDRESS, 16, 1,
		       pSkb_tx->data, pSkb_tx->len, true);
		print_hex_dump(KERN_ERR, "Rx: ", DUMP_PREFIX_ADDRESS, 16, 1,
		       pSkb_rx->data, pSkb_rx->len, true);

		dev_err(pGmacDev->pDev, "phy addr txbuf: 0x%08x, rxbuf: 0x%08x\n",
			dmaDesc_tx.buffer1 , dmaDesc_rx.buffer1);

		err = -1000;
	}

	dev_kfree_skb(pSkb_rx);

	return err;

err0:
	return -ENODATA;
}

static void
sdp_mac_adjust_speed_mac(SDP_GMAC_DEV_T *pGmacDev)
{
	struct net_device *pNetDev = pGmacDev->pNetDev;
	struct phy_device *phydev = pGmacDev->phydev;

	u32 ctrl = pGmacDev->pGmacBase->configuration;
	wmb();

	/* Now we make sure that we can be in full duplex mode.
	 * If not, we operate in half-duplex mode. */
	if (!(phydev->duplex))
		ctrl &= ~B_DUPLEX_FULL;
	else
		ctrl |= B_DUPLEX_FULL;

	/* Flow Control operation */
	if ((pGmacDev->flow_ctrl != SDP_MAC_FLOW_OFF) && phydev->pause)
		dev_warn(&pNetDev->dev, "Not support Flow Control\n");


	switch (phydev->speed) {
	case 1000:
		if (likely(pGmacDev->has_gmac))
			ctrl &= ~B_PORT_MII;
		break;
	case 100:
	case 10:
		if (pGmacDev->has_gmac) {
			ctrl |= B_PORT_MII;
			if (phydev->speed == SPEED_100) {
				ctrl |= B_SPEED_100M;
			} else {
				ctrl &= ~B_SPEED_100M;
			}
		} else {
			ctrl &= ~B_PORT_MII;
		}
		break;
	default:
		if (netif_msg_link(pGmacDev))
			dev_warn(&pNetDev->dev, "Speed (%d) is not 10"
		       " or 100!\n", phydev->speed);
		return;
	}

	pGmacDev->pGmacBase->configuration = ctrl;
	wmb();
}

static void
sdp_mac_phy_regdump(SDP_GMAC_DEV_T *pGmacDev, const char *header)
{
	struct phy_device *phydev = pGmacDev->phydev;
	u32 i;

	if(header == NULL) {
		header = "";
	}

	dev_info(pGmacDev->pDev, "%s Phy Dump\n", header);
	for(i = 0; i < 0x20; i+=4) {
		dev_info(pGmacDev->pDev, "Phy Dump %2d: 0x%04x 0x%04x 0x%04x 0x%04x\n",
			i, phy_read(phydev, i+0), phy_read(phydev, i+1),
			phy_read(phydev, i+2), phy_read(phydev, i+3));
	}

}

/* return xfer time(us) */
static int
sdp_mac_mac_xfer(SDP_GMAC_DEV_T *pGmacDev, const char *header)
{
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;

	unsigned long timeout_ms = 10;
	unsigned long timeout;
	int err = 0;

	static struct timeval start, finish;

	if(header == NULL) {
		header = "";
	}

	do_gettimeofday(&start);

	pDmaReg->status = pDmaReg->status;//all clear

	pGmacReg->frameFilter|= B_RX_ALL;
	pDmaReg->operationMode |= B_TX_EN | B_RX_EN;
	pGmacReg->configuration |= B_TX_ENABLE | B_RX_ENABLE;

	timeout = jiffies + msecs_to_jiffies(timeout_ms);
	while(!(pDmaReg->status&B_TX_INTR)) {
		if(time_is_before_jiffies(timeout)) {
			dev_err(pGmacDev->pDev, "%s Tx timeout! %lums\n", header, timeout_ms);
			err = -1010;
			goto done;
		}
	}
	pDmaReg->status = B_TX_INTR;//clear
	dev_dbg(pGmacDev->pDev, "%s Tx done!\n", header);

	timeout = jiffies + msecs_to_jiffies(timeout_ms);
	while(!(pDmaReg->status&B_RX_INTR)) {
		if(time_is_before_jiffies(timeout)) {
			dev_err(pGmacDev->pDev, "%s Rx timeout! %lums\n", header, timeout_ms);
			err = -1020;
			goto done;
		}
	}
	pDmaReg->status = B_RX_INTR;//clear
	dev_dbg(pGmacDev->pDev, "%s Rx done!\n", header);


done:
	pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);
	pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN);
	pGmacReg->frameFilter &= ~B_RX_ALL;

	do_gettimeofday(&finish);

	if(err >= 0) {
		err = (int)(timeval_to_ns(&finish) - timeval_to_ns(&start)) / NSEC_PER_USEC;
	} else {
		sdp_mac_gmac_dump(pGmacDev);
		sdp_mac_dma_dump(pGmacDev);
		dev_err(pGmacDev->pDev,
			"%s tx desc status %#x, rx desc status %#x\n",
			header, pGmacDev->pTxDescBusy->status, pGmacDev->pRxDescBusy->status);
	}

	return err;
}

static int
sdp_mac_do_self_test(struct net_device *pNetDev, enum sdp_mac_self_test_num test)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;
	struct phy_device *phydev = pGmacDev->phydev;
	int err = 0, ret = 0;
	int time;

	dev_dbg(pGmacDev->pDev, "%s...\n",
		sdp_mac_ethtool_self_test_string[test]);

	phydev->autoneg = AUTONEG_DISABLE;
	phydev->speed = SPEED_100;
	phydev->duplex = DUPLEX_FULL;
	genphy_config_aneg(phydev);

	if(test == SDP_PHY_LB) phy_write(phydev, MII_BMCR, (u16)phy_read(phydev, MII_BMCR)|BMCR_LOOPBACK);

	sdpGmac_netDev_initDesc(pGmacDev, DESC_MODE_RING);
	sdp_mac_setup_desc(pGmacDev);

	genphy_read_status(phydev);
	sdp_mac_adjust_speed_mac(pGmacDev);
	if(test == SDP_MAC_LB) pGmacReg->configuration |= B_LOOP_BACK_EN;
	mdelay(100);

	if(test != SDP_MAC_LB) {
		unsigned long timeout_ms = 1000;
		unsigned long timeout = jiffies + msecs_to_jiffies(timeout_ms);

		while(genphy_read_status(phydev) >= 0) {
			if(phydev->link == 1) {
				mdelay(50);
				break;
			}
			if(time_is_before_jiffies(timeout)) {
				dev_err(pGmacDev->pDev, "%s link timeout! %lums\n", sdp_mac_ethtool_self_test_string[test], timeout_ms);
				err = -ETIMEDOUT;
				break;
			}
		}
	}

	if(err == 0) {
		time = sdp_mac_mac_xfer(pGmacDev, sdp_mac_ethtool_self_test_string[test]);
	} else {
		time = err;
	}

	if(time < 0) {
		err = time;
	} else {
		err = sdp_mac_compare_frame(pGmacDev, sdp_mac_ethtool_self_test_string[test]);
	}

	if(err < 0) {
		sdp_mac_phy_regdump(pGmacDev, sdp_mac_ethtool_self_test_string[test]);
		dev_info(pGmacDev->pDev, "%s fail(%d)!!!\n",
			sdp_mac_ethtool_self_test_string[test], err);

		ret = err;
	} else {
		ret = time;
	}

	if(test == SDP_MAC_LB) pGmacReg->configuration &= ~(B_LOOP_BACK_EN);
	if(test == SDP_PHY_LB) phy_write(phydev, MII_BMCR, (u16)(phy_read(phydev, MII_BMCR)&~BMCR_LOOPBACK));

	sdp_mac_clean_desc(pGmacDev);

	return ret;
}

static void
sdp_mac_ethtool_self_test(struct net_device *pNetDev, struct ethtool_test *test, u64 *data)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	struct phy_device *phydev = pGmacDev->phydev;
	bool if_running = netif_running(pNetDev);
	int err = 0;

	dev_dbg(pGmacDev->pDev, "Start self test!\n");

	if(test->len < ARRAY_SIZE(sdp_mac_ethtool_self_test_string)) {
		dev_err(pGmacDev->pDev, "not support result len!\n");
	}

	memset(data, 0, sizeof(u64)*test->len);

	if(test->flags & ETH_TEST_FL_OFFLINE) {
		int old_autoneg = phydev->autoneg;
		int old_speed = phydev->speed;
		int old_duplex = phydev->duplex;

		//Start Offline test!!
		if (if_running) {
			/* indicate we're in test mode */
			dev_close(pNetDev);
		}

		dev_dbg(pGmacDev->pDev, "Offline test entered\n");

		//external loopback test
		if(test->flags & ETH_TEST_FL_EXTERNAL_LB) {
			test->flags |= ETH_TEST_FL_EXTERNAL_LB_DONE;

			err = sdp_mac_do_self_test(pNetDev, SDP_EXT_LB);
			if(err < 0) {
				test->flags |= ETH_TEST_FL_FAILED;
			}
			data[SDP_EXT_LB] = (u64)err;

		}


		//phy loopback test
		err = sdp_mac_do_self_test(pNetDev, SDP_PHY_LB);
		if(err < 0) {
			test->flags |= ETH_TEST_FL_FAILED;
		}
		data[SDP_PHY_LB] = (u64)err;



		//MAC loopback test
		err = sdp_mac_do_self_test(pNetDev, SDP_MAC_LB);
		if(err < 0) {
			test->flags |= ETH_TEST_FL_FAILED;
		}
		data[SDP_MAC_LB] = (u64)err;



		//Phy restore
		phydev->autoneg = old_autoneg;
		phydev->speed = old_speed;
		phydev->duplex = old_duplex;
		genphy_config_aneg(phydev);

		//Reset GMAC H/W
		if(sdpGmac_reset(pNetDev) < 0){
			dev_err(pGmacDev->pDev, "GMAC H/W Reset error(%d)!\n", err);
			test->flags |= ETH_TEST_FL_FAILED;
		} else {
			err = sdpGmac_netDev_initDesc(pGmacDev, DESC_MODE_RING);
			if(err < 0) {
				dev_err(pGmacDev->pDev, "DMA Desc init error(%d)!\n", err);
				test->flags |= ETH_TEST_FL_FAILED;
			} else {
				//register initalize
				sdpGmac_dmaInit(pNetDev);
				sdpGmac_gmacInit(pNetDev);
				if(is_valid_ether_addr(pNetDev->dev_addr)) {
					sdpGmac_setMacAddr(pNetDev, (const u8*)pNetDev->dev_addr);
				}
			}
		}

		if (if_running) {
			dev_open(pNetDev);
		}
	}
	dev_dbg(pGmacDev->pDev, "End self test!\n");
}

// 0.92
static int sdpGmac_drv_suspend(struct platform_device *pDev, pm_message_t state)
{
	struct net_device *pNetDev = platform_get_drvdata(pDev);

	SDP_GMAC_DEV_T *pGmacDev ;
	SDP_GMAC_T *pGmacReg ;
	SDP_GMAC_DMA_T *pDmaReg ;
	SDP_GMAC_POWER_T    *pPower ;

	if(pNetDev){
		pGmacDev = netdev_priv(pNetDev);
		pGmacReg = pGmacDev->pGmacBase;
		pDmaReg = pGmacDev->pDmaBase;
		pPower = &pGmacDev->power;

		spin_lock_bh(&pGmacDev->lock);

		pPower->dma_busMode = pDmaReg->busMode;
		pPower->dma_operationMode = pDmaReg->operationMode;
		pPower->gmac_configuration = pGmacReg->configuration;
		pPower->gmac_frameFilter = pGmacReg->frameFilter;

		spin_unlock_bh(&pGmacDev->lock);

// backup mac address
		memcpy((void *)pPower->gmac_macAddr,
				(void *)&pGmacReg->macAddr_00_High,
				 	sizeof(pPower->gmac_macAddr));

		if(netif_running(pNetDev)){

			netif_device_detach(pNetDev);

			spin_lock_bh(&pGmacDev->lock);

			pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN) ;
			pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);

			// all interrupt disable
			pDmaReg->intrEnable = 0;
			pDmaReg->status = pDmaReg->status;// Clear interrupt pending register

			spin_unlock_bh(&pGmacDev->lock);

			//napi disable
			napi_disable(&pGmacDev->napi);

			if (pNetDev->phydev) {
				phy_stop(pNetDev->phydev);
				genphy_suspend(pNetDev->phydev);
			}

// long time
			// skb control
			// TODO -> Tx buffer initial, Rx Buffer empty
			sdpGmac_clearAllDesc(pNetDev);
// long time
			// timer control
		}
	}

	return 0;
}

// 0.92
static void
sdpGmac_resume(SDP_GMAC_DEV_T *pGmacDev)
{

	SDP_GMAC_T 	 *pGmacReg = pGmacDev->pGmacBase;
 	SDP_GMAC_DMA_T 	 *pDmaReg  = pGmacDev->pDmaBase;
	SDP_GMAC_MMC_T 	 *pMmcReg  = pGmacDev->pMmcBase;
	SDP_GMAC_POWER_T *pPower   = &pGmacDev->power;

	u32 idx;

// resotore mac address
	memcpy( (void *)&pGmacReg->macAddr_00_High,
		(void *)pPower->gmac_macAddr,
		sizeof(pPower->gmac_macAddr));

#if 1
	sdpGmac_txInitDesc(pGmacDev->pTxDesc);
	sdpGmac_rxInitDesc(pGmacDev->pRxDesc);
	pDmaReg->txDescListAddr = pGmacDev->txDescDma; // set register
	pDmaReg->rxDescListAddr = pGmacDev->rxDescDma; // set register
#endif
	// RX Desc
	idx = (pDmaReg->curHostRxDesc - pDmaReg->rxDescListAddr) / sizeof(DMA_DESC_T);

	if(idx >= NUM_RX_DESC) idx = 0;

	pGmacDev->rxNext = idx;
	pGmacDev->rxBusy = idx;
	pGmacDev->pRxDescNext = pGmacDev->pRxDesc + idx;
	pGmacDev->pRxDescBusy = pGmacDev->pRxDesc + idx;

	// TX Desc
	idx = (pDmaReg->curHostTxDesc - pDmaReg->txDescListAddr) / sizeof(DMA_DESC_T);

	if(idx >= NUM_RX_DESC) idx = 0;

	pGmacDev->txNext = idx;
	pGmacDev->txBusy = idx;
	pGmacDev->pTxDescNext = pGmacDev->pTxDesc + idx;
	pGmacDev->pTxDescBusy = pGmacDev->pTxDesc + idx;

	//  MMC interrupt disable all
	pMmcReg = pGmacDev->pMmcBase;
	pMmcReg->intrMaskRx = 0x00FFFFFF;  // disable
	pMmcReg->intrMaskTx = 0x01FFFFFF;  // disable
	pMmcReg->mmcIpcIntrMaskRx = 0x3FFF3FFF;  // disable

	pDmaReg->busMode = pPower->dma_busMode;
	pDmaReg->operationMode = pPower->dma_operationMode & ~(B_TX_EN | B_RX_EN);
	pGmacReg->configuration = pPower->gmac_configuration & ~(B_TX_ENABLE | B_RX_ENABLE);
	pGmacReg->frameFilter = pPower->gmac_frameFilter;

	return;
}

// 0.92
static int
sdpGmac_drv_resume(struct platform_device *pDev)
{
	struct net_device *pNetDev = platform_get_drvdata(pDev);
	SDP_GMAC_DEV_T *pGmacDev;

	if(pNetDev){
		printk("[%s]%s resume\n", __FUNCTION__, pNetDev->name);

		pGmacDev = netdev_priv(pNetDev);

		sdpGmac_reset(pNetDev);   	// long time
		sdpGmac_resume(pGmacDev);

		if(netif_running(pNetDev)){
			SDP_GMAC_T     *pGmacReg = pGmacDev->pGmacBase;
 			SDP_GMAC_DMA_T *pDmaReg  = pGmacDev->pDmaBase;

			struct sk_buff  *pSkb;
			dma_addr_t  	rxDmaAddr;
			DMA_DESC_T	rxInitDesc;

			u32	len;

			len = ETH_DATA_LEN + ETHER_HEADER + ETHER_CRC + 4;
			len += ((SDP_GMAC_BUS >> 3) - 1);
			len &= SDP_GMAC_BUS_MASK;

			//skb buffer initialize ????	check
			do{
//				pSkb = dev_alloc_skb(pNetDev->mtu + ETHER_HEADER + ETHER_CRC);
				pSkb = dev_alloc_skb(len);   // 0.931 // 0.954

				if(pSkb == NULL){
                        		DPRINTK_GMAC_ERROR("could not allocate sk buffer \n");
                        		break;
				}
				sdpGmac_skbAlign(pSkb); // 0.954

				rxDmaAddr = dma_map_single(pGmacDev->pDev, pSkb->data, (size_t)skb_tailroom(pSkb), DMA_FROM_DEVICE);

//				CONVERT_DMA_QPTR(rxInitDesc, skb_tailroom(pSkb), rxDmaAddr, pSkb, 0, 0);
				CONVERT_DMA_QPTR(rxInitDesc, len, rxDmaAddr, pSkb, 0, 0);

				if(sdpGmac_setRxQptr(pGmacDev, &rxInitDesc, 0) < 0){
					dev_kfree_skb(pSkb);
					break;
				}
			}while(1);


			if (pNetDev->phydev) {
				genphy_suspend(pNetDev->phydev);
				phy_start(pNetDev->phydev);
			}

			//  interrupt enable all
			pDmaReg->intrEnable = B_INTR_ENABLE_ALL;

			// tx, rx enable
			netif_device_attach(pNetDev);
			pDmaReg->operationMode |= (B_TX_EN | B_RX_EN);
			pGmacReg->configuration |= (B_TX_ENABLE | B_RX_ENABLE);

			//napi enable
			napi_enable(&pGmacDev->napi);
		}

	}

	return 0;
}


#ifdef CONFIG_MAC_GET_I2C
#include <asm/plat-sdp/sdp_i2c_io.h>
#define SDPGMAC_I2C_MAGIC_LEN	4
//#define SDPGMAC_I2C_MAGIC_FACTOR	0xDEAFBEEF
#define SDPGMAC_I2C_MAGIC_USER		0xFAFEF0F0
#define SDPGMAC_READ_I2C_LEN		12
#define SDPGMAC_READ_I2C_RETRY		5		// 0.921

static inline void sdpGmac_getMacFromI2c(struct net_device *pNetDev)
{
	int i;
	u8 i2cSubAddr[CONFIG_MAC_I2C_SUBADDR_NR];
	u8 readData[SDPGMAC_READ_I2C_LEN];
	u8 *pMacAddr = (readData + 4);
	u32 *pFactory = (u32*)readData;

	struct sdp_i2c_packet_t getMacPacket =
			{
				.slaveAddr = CONFIG_MAC_I2C_SLV_ADDR & 0xFF,
				.subAddrSize = CONFIG_MAC_I2C_SUBADDR_NR,
				.udelay = 1000,
				.speedKhz = 400,
				.dataSize = SDPGMAC_READ_I2C_LEN,
				.pSubAddr = i2cSubAddr,
				.pDataBuffer = readData,
				.reserve[0] = 0,		// 0.94
				.reserve[1] = 0,		// 0.94
				.reserve[2] = 0,		// 0.94
				.reserve[3] = 0,		// 0.94
			};

// initial data to 0xFF
	for(i = 0 ; i < SDPGMAC_READ_I2C_LEN; i++)
		readData[i] = 0xFF;
// align
	switch(CONFIG_MAC_I2C_SUBADDR_NR){
		case(1):
			i2cSubAddr[0] = CONFIG_MAC_I2C_SUBADDR & 0xFF;
			break;
		case(2):
			i2cSubAddr[0] = (CONFIG_MAC_I2C_SUBADDR >> 8) & 0xFF;
			i2cSubAddr[1] = CONFIG_MAC_I2C_SUBADDR & 0xFF;
			break;
		case(3):
			i2cSubAddr[0] = (CONFIG_MAC_I2C_SUBADDR >> 16) & 0xFF;
			i2cSubAddr[1] = (CONFIG_MAC_I2C_SUBADDR >> 8) & 0xFF;
			i2cSubAddr[2] = CONFIG_MAC_I2C_SUBADDR & 0xFF;
			break;
		case(4):
			i2cSubAddr[0] = (CONFIG_MAC_I2C_SUBADDR >> 24) & 0xFF;
			i2cSubAddr[1] = (CONFIG_MAC_I2C_SUBADDR >> 16) & 0xFF;
			i2cSubAddr[2] = (CONFIG_MAC_I2C_SUBADDR >> 8) & 0xFF;
			i2cSubAddr[3] = CONFIG_MAC_I2C_SUBADDR & 0xFF;
			break;
		default:
			memcpy(pNetDev->dev_addr, pMacAddr, N_MAC_ADDR);
			return;
			break;
	}

	i = SDPGMAC_READ_I2C_RETRY;

	do {
		if(sdp_i2c_request(CONFIG_MAC_I2C_PORT,
				I2C_CMD_COMBINED_READ,
				&getMacPacket) >= 0) break;	// 0.921
	}while(i--);
#    if 0 	// 0.9541
	if(*pFactory == SDPGMAC_I2C_MAGIC_USER) {
		memcpy(pNetDev->dev_addr, pMacAddr, N_MAC_ADDR);
	}
	else {
		memset(pNetDev->dev_addr, 0xFF, N_MAC_ADDR);
	}
#    else
	memcpy(pNetDev->dev_addr, pMacAddr, N_MAC_ADDR);
#    endif 	// 0.9541

	return;
}
#else
static inline void sdpGmac_setMacByUser(struct net_device *pNetDev)
{
	u32 front_4Byte = CONFIG_MAC_FRONT_4BYTE;
	u32 end_2Byte = CONFIG_MAC_END_2BYTE;
	u8 *pDev_addr = pNetDev->dev_addr;

	pDev_addr[0] = (u8)((front_4Byte >> 24) & 0xFF);
	pDev_addr[1] = (u8)((front_4Byte >> 16) & 0xFF);
	pDev_addr[2] = (u8)((front_4Byte >> 8) & 0xFF);
	pDev_addr[3] = (u8)((front_4Byte) & 0xFF);
	pDev_addr[4] = (u8)((end_2Byte >> 8) & 0xFF);
	pDev_addr[5] = (u8)(end_2Byte & 0xFF);

}
#endif

static int sdpGmac_netDev_initDesc(SDP_GMAC_DEV_T* pGmacDev, u32 descMode)
{
	int retVal = SDP_GMAC_OK;
	SDP_GMAC_DMA_T* pDmaReg = pGmacDev->pDmaBase;

	DPRINTK_GMAC_FLOW ("called\n");

	if (descMode == DESC_MODE_RING) {
// Tx Desc initialize
		pGmacDev->txNext = 0;
		pGmacDev->txBusy = 0;
		pGmacDev->pTxDescNext = pGmacDev->pTxDesc;
		pGmacDev->pTxDescBusy = pGmacDev->pTxDesc;
		pDmaReg->txDescListAddr = pGmacDev->txDescDma; // set register

		sdpGmac_txInitDesc(pGmacDev->pTxDesc);
		DPRINTK_GMAC_FLOW("last TxDesc.status is 0x%08x\n",
				pGmacDev->pTxDesc[NUM_TX_DESC-1].status);

// Rx Desc initialize
		pGmacDev->rxNext = 0;
		pGmacDev->rxBusy = 0;
		pGmacDev->pRxDescNext = pGmacDev->pRxDesc;
		pGmacDev->pRxDescBusy = pGmacDev->pRxDesc;
		pDmaReg->rxDescListAddr = pGmacDev->rxDescDma; // set register

		sdpGmac_rxInitDesc(pGmacDev->pRxDesc);
		DPRINTK_GMAC_FLOW("last RxDesc.status is 0x%08x\n",
				pGmacDev->pRxDesc[NUM_RX_DESC-1].length);
	}
	else {
		DPRINTK_GMAC_ERROR("Not support CHAIN MODE yet\n");
		retVal = -SDP_GMAC_ERR;
	}

	DPRINTK_GMAC_FLOW("Tx Desc is 0x%08x\n",  (int)pGmacDev->pTxDesc);
	DPRINTK_GMAC_FLOW("Rx Desc is 0x%08x\n",  (int)pGmacDev->pRxDesc);

	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;
}

static const struct ethtool_ops sdpGmac_ethtool_ops = {
        .get_settings	= sdpGmac_ethtool_getsettings,
        .set_settings	= sdpGmac_ethtool_setsettings,
        .get_drvinfo	= sdpGmac_ethtool_getdrvinfo,
        .get_msglevel	= sdpGmac_ethtool_getmsglevel,
        .set_msglevel	= sdpGmac_ethtool_setmsglevel,
        .nway_reset	= sdpGmac_ethtool_nwayreset,
        .get_link  	= ethtool_op_get_link,
        .get_regs_len	= sdpGmac_ethtool_getregslen,
        .get_regs	= sdpGmac_ethtool_getregs,
        .self_test = sdp_mac_ethtool_self_test,
        .get_strings = sdp_mac_ethtool_get_strings,
        .get_sset_count = sdp_mac_ethtool_get_sset_count,
};

static const struct net_device_ops sdpGmac_netdev_ops = {
	.ndo_open		= sdpGmac_netDev_open,
        .ndo_stop               = sdpGmac_netDev_close,
        .ndo_start_xmit         = sdpGmac_netDev_hardStartXmit,
        .ndo_get_stats          = sdpGmac_netDev_getStats,
        .ndo_set_multicast_list = sdpGmac_netDev_setMulticastList,
        .ndo_do_ioctl           = sdpGmac_netDev_ioctl,
        .ndo_change_mtu         = eth_change_mtu,
        .ndo_validate_addr      = eth_validate_addr,
        .ndo_set_mac_address    = sdpGmac_netDev_setMacAddr,
#ifdef CONFIG_NET_POLL_CONTROLLER
		.ndo_poll_controller = sdpGmac_netDev_poll_controller,
#endif
};

static int sdpGmac_rtl8201_clkout_fixup(struct phy_device *phydev)
{
	const u32 RTL820x_TEST_REG = 0x19;
	u32 phyVal;

	dev_info(&phydev->dev, "RTL8201x Phy RMII mode tx clock output fixup.\n");

	// Test regiser, Clock output set
	phyVal = (u16)phy_read(phydev, RTL820x_TEST_REG);

	phyVal = phyVal & ~(1UL << 11);/*RMII_CLKIN*/
	phyVal = phyVal & ~(1UL << 13);

	phy_write(phydev, RTL820x_TEST_REG, (u16)phyVal);

	return 0;
}

static int __devinit sdpGmac_probe(struct net_device *pNetDev)
{
	int retVal = 0;
//	const u8 defaultMacAddr[N_MAC_ADDR] = DEFAULT_MAC_ADDR;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	u32 len;

	struct mii_bus *sdp_mdiobus = NULL;
	struct sdp_mac_mdiobus_priv *mdiobus_priv = NULL;


	DPRINTK_GMAC ("called\n");

	spin_lock_init(&pGmacDev->lock);

	phy_register_fixup_for_uid(0x001CC810, 0x001ffff0, sdpGmac_rtl8201_clkout_fixup);

	//mdio bus register
	sdp_mdiobus = mdiobus_alloc();
	if(!sdp_mdiobus) {
		goto __probe_out_err;
	}
	sdp_mdiobus->name = "sdp_mdiobus";
	snprintf(sdp_mdiobus->id, MII_BUS_ID_SIZE, "sdp_mdio");
	sdp_mdiobus->phy_mask = pGmacDev->plat->phy_mask/*all addr 1 is ignored */;
	sdp_mdiobus->irq = kzalloc(sizeof(int)*PHY_MAX_ADDR, GFP_KERNEL);
	for(retVal = 0; retVal < PHY_MAX_ADDR; retVal++) sdp_mdiobus->irq[retVal] = PHY_POLL;
	sdp_mdiobus->read = sdp_mac_mdio_read;
	sdp_mdiobus->write = sdp_mac_mdio_write;
	mdiobus_priv = kzalloc(sizeof(struct sdp_mac_mdiobus_priv), GFP_KERNEL);
	mdiobus_priv->addr = (u32)&pGmacDev->pGmacBase->gmiiAddr;
	mdiobus_priv->data = (u32)&pGmacDev->pGmacBase->gmiiData;
	sdp_mdiobus->priv = mdiobus_priv;


	retVal = mdiobus_register(sdp_mdiobus);
	if(retVal < 0) {
		mdiobus_free(sdp_mdiobus);
		dev_err(pGmacDev->pDev, "mdiobus_register error %d\n", retVal);
		goto __probe_out_err;
	}
	pGmacDev->mdiobus = sdp_mdiobus;

	pGmacDev->oldlink = 0;

	pGmacDev->phydev = phy_find_first(pGmacDev->mdiobus);
	if (IS_ERR_OR_NULL(pGmacDev->phydev)) {
		dev_err(pGmacDev->pDev, "Could not find PHY\n");
		mdiobus_unregister(sdp_mdiobus);
		mdiobus_free(sdp_mdiobus);
		return -ENODEV;
	}

	/* Device reset MAC addr is cleared*/	
	if(sdpGmac_reset(pNetDev) < 0){
		dev_err(pGmacDev->pDev, "H/W Reset Error!!!\n");
		retVal = -ENODEV;		// 0.952
		goto  __probe_out_err;		// 0.952
	}

	ether_setup(pNetDev);

	/* init napi struct */
	netif_napi_add(pNetDev, &pGmacDev->napi, sdpGmac_netDev_poll, pGmacDev->plat->napi_weight);
	dev_dbg(pGmacDev->pDev, "initialized a napi context. weight=%d\n", pGmacDev->napi.weight);

	pNetDev->ethtool_ops = &sdpGmac_ethtool_ops;
	pNetDev->netdev_ops = &sdpGmac_netdev_ops;
	pNetDev->watchdog_timeo = 5 * HZ;


	len = ETH_DATA_LEN + ETHER_PACKET_EXTRA + 4;
	len += ((SDP_GMAC_BUS >> 3) - 1);
	len &= SDP_GMAC_BUS_MASK;
	pGmacDev->pRxDummySkb = dev_alloc_skb(len);   // 0.931  // 0.954

	if(pGmacDev->pRxDummySkb == NULL) {
		retVal = -ENOMEM;
		goto __probe_out_err;
	}

	sdpGmac_skbAlign(pGmacDev->pRxDummySkb); // 0.954

// net stats init
	pGmacDev->netStats.rx_errors 	    = 0;
	pGmacDev->netStats.collisions       = 0;
	pGmacDev->netStats.rx_crc_errors    = 0;
	pGmacDev->netStats.rx_frame_errors  = 0;
	pGmacDev->netStats.rx_length_errors = 0;

// TODO check RING MODE and CHAIN MODE
	pGmacDev->pTxDesc = dma_alloc_coherent(pGmacDev->pDev, TX_DESC_SIZE,
						&pGmacDev->txDescDma, GFP_ATOMIC);
	if(pGmacDev->pTxDesc == NULL) {
		retVal = -ENOMEM;		// 0.952
		goto __probe_out_err1;		// 0.952
	}

	pGmacDev->pRxDesc = dma_alloc_coherent(pGmacDev->pDev, RX_DESC_SIZE,
						&pGmacDev->rxDescDma, GFP_ATOMIC);

	if(pGmacDev->pRxDesc == NULL) {
		retVal = -ENOMEM;
		goto __probe_out_err2;
	}

	retVal = sdpGmac_netDev_initDesc(pGmacDev, DESC_MODE_RING);

	if(retVal < 0) {
		retVal = -EPERM;		// 0.952
		goto __probe_out_err3;		// 0.952
	}

// register initalize
	sdpGmac_dmaInit(pNetDev);
	sdpGmac_gmacInit(pNetDev);

// request interrupt resource
	retVal = request_irq(pNetDev->irq, sdpGmac_intrHandler, 0, pNetDev->name, pNetDev);
	if(retVal < 0) goto __probe_out_err3;	// 0.952

	disable_irq(pNetDev->irq);

	if(register_netdev(pNetDev) < 0) {
		dev_err(pGmacDev->pDev, "register netdev error\n");
		retVal = -EPERM;
		goto __probe_out_err4;
	}

#if defined(CONFIG_MAC_GET_I2C)
	sdpGmac_getMacFromI2c(pNetDev);		// get mac address
#elif defined(CONFIG_MAC_SET_BY_USER)
	sdpGmac_setMacByUser(pNetDev);
#endif

	if(!is_valid_ether_addr(pNetDev->dev_addr)){
		dev_warn(pGmacDev->pDev, "Invalid ethernet MAC address. Please "
				"set using ifconfig\n");
	}
	else {
		dev_info(pGmacDev->pDev, "Ethernet address %02x:%02x:%02x:%02x:%02x:%02x\n",
		     *pNetDev->dev_addr, *(pNetDev->dev_addr+1),
		     *(pNetDev->dev_addr+2), *(pNetDev->dev_addr+3),
		     *(pNetDev->dev_addr+4), *(pNetDev->dev_addr+5));

		sdpGmac_setMacAddr(pNetDev, (const u8*)pNetDev->dev_addr);
	}

	dev_info(pGmacDev->pDev, "IRQ is %d, use %s Mode\n", pNetDev->irq,
		sdp_mac_get_interface(pNetDev)==PHY_INTERFACE_MODE_RGMII?"RGMII":"GMII");



	DPRINTK_GMAC ("exit\n");
	return 0;

	unregister_netdev(pNetDev);
__probe_out_err4:				// 0.952
	free_irq(pNetDev->irq, pNetDev);
__probe_out_err3:				// 0.952
	dma_free_coherent(pGmacDev->pDev, RX_DESC_SIZE,
				pGmacDev->pRxDesc, pGmacDev->rxDescDma);
__probe_out_err2:				// 0.952
	dma_free_coherent(pGmacDev->pDev, TX_DESC_SIZE,
				pGmacDev->pTxDesc, pGmacDev->txDescDma);
__probe_out_err1:				// 0.952
	dev_kfree_skb(pGmacDev->pRxDummySkb);
__probe_out_err:
	return retVal;
}


static inline int sdpGmac_setMemBase(int id, struct resource *pRes, SDP_GMAC_DEV_T* pGmacDev)
{
	int	retVal = 0;
	void  	*remapAddr;
	size_t 	size = (pRes->end) - (pRes->start);


// TODO: request_mem_region

	if (id < N_REG_BASE) {
		remapAddr = (void*)ioremap_nocache(pRes->start, size);
	} else {
		DPRINTK_GMAC_ERROR("id is wrong \n");
		return retVal -1;
	}


	switch(id){
		case (0):
		   pGmacDev->pGmacBase = (SDP_GMAC_T*)remapAddr;
		   DPRINTK_GMAC ("GMAC remap is 0x%08x\n",(int) remapAddr);
		   break;

		case (1):
		   pGmacDev->pMmcBase = (SDP_GMAC_MMC_T*)remapAddr;
		   DPRINTK_GMAC ("Gmac manage count remap is 0x%08x\n",(int) remapAddr);
		   break;

		case (2):
		   pGmacDev->pTimeStampBase = (SDP_GMAC_TIME_STAMP_T*)remapAddr;
		   DPRINTK_GMAC ("time stamp remap is 0x%08x\n",(int) remapAddr);
		   break;

		case (3):
		   pGmacDev->pMac2ndBlk = (SDP_GMAC_MAC_2ND_BLOCK_T*)remapAddr;
		   DPRINTK_GMAC ("mac 2nd remap is 0x%08x\n",(int) remapAddr);
		   break;

		case (4):
		   pGmacDev->pDmaBase = (SDP_GMAC_DMA_T*)remapAddr;
		   DPRINTK_GMAC ("DMA remap is 0x%08x\n",(int) remapAddr);
		   break;

		default:
		   break;
	}

//	DPRINTK_GMAC ("exit\n");

	return retVal;
}

/* Linux probe and remove */
static int __devinit sdpGmac_drv_probe (struct platform_device *pDev)
{
	struct resource *pRes = pDev->resource;
	struct net_device *pNetDev;
	SDP_GMAC_DEV_T *pGmacDev;
	int retVal = 0;
	int i = 0;

	DPRINTK_GMAC_FLOW ("called\n");


	if(pRes == NULL) goto __probe_drv_out;

	dev_info(&pDev->dev, "SDP GMAC network driver ver %s\n", GMAC_DRV_VERSION);
// net device
	pNetDev = alloc_etherdev(sizeof(SDP_GMAC_DEV_T));

    if (!pNetDev) {
        dev_err(&pDev->dev, "could not allocate device.\n");
        retVal = -ENOMEM;
        goto __probe_drv_out;
    }
	SET_NETDEV_DEV(pNetDev, &pDev->dev);

	pNetDev->dma = (unsigned char) -1;
	pGmacDev = netdev_priv(pNetDev);
#ifdef CONFIG_ARCH_SDP1004
	pGmacDev->revision = sdp_get_revision_id();
	dev_info(&pDev->dev, "Firenze revision %d\n", pGmacDev->revision);
#endif
	pGmacDev->pNetDev = pNetDev;
// need to request dma memory
	pGmacDev->pDev = &pDev->dev;

	pGmacDev->pGmacBase = NULL;		// 0.952
	pGmacDev->pMmcBase = NULL;		// 0.952
	pGmacDev->pTimeStampBase = NULL;	// 0.952
	pGmacDev->pMac2ndBlk = NULL;		// 0.952
	pGmacDev->pDmaBase = NULL;		// 0.952

	/* set platform data! */
	pGmacDev->plat = dev_get_platdata(&pDev->dev);
	if(!pGmacDev->plat) {
		free_netdev(pNetDev);
		dev_err(&pDev->dev, "platform data is not exist!!!\n");
		return -EINVAL;
	}

// GMAC resource initialize
	for (i = 0; i < N_REG_BASE; i++) {
		pRes = platform_get_resource(pDev, IORESOURCE_MEM, (u32)i);
		if(!pRes){	// 0.952
			dev_err(&pDev->dev, "could not find device %d resource.\n", i);
			retVal = -ENODEV;
			goto __probe_drv_err;
		}
		sdpGmac_setMemBase(i, pRes, pGmacDev);
	}

	retVal = platform_get_irq(pDev, 0);
	if(retVal < 0) {
		goto __probe_drv_err;
	}
	pNetDev->irq = (u32)retVal;

// save resource about net driver
	platform_set_drvdata(pDev, pNetDev);

	retVal = sdpGmac_probe(pNetDev);

__probe_drv_err:
	if (retVal < 0) {
		if(pGmacDev->pGmacBase) iounmap(pGmacDev->pGmacBase);
		if(pGmacDev->pMmcBase) iounmap(pGmacDev->pMmcBase);	//0.952
		if(pGmacDev->pTimeStampBase) iounmap(pGmacDev->pTimeStampBase);
		if(pGmacDev->pMac2ndBlk) iounmap(pGmacDev->pMac2ndBlk);
		if(pGmacDev->pDmaBase) iounmap(pGmacDev->pDmaBase);
		free_netdev(pNetDev);
	}

__probe_drv_out:
	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;
}


static int __devexit sdpGmac_drv_remove (struct platform_device *pDev)
{
	struct net_device *pNetDev;
	SDP_GMAC_DEV_T *pGmacDev;
	SDP_GMAC_T *pGmacReg;
	SDP_GMAC_DMA_T *pDmaReg;

	DPRINTK_GMAC_FLOW ("called\n");

	if(!pDev) return 0;
	pNetDev = platform_get_drvdata(pDev);

	if(!pNetDev) return 0;
	pGmacDev = netdev_priv(pNetDev);

	unregister_netdev(pNetDev);

	pGmacReg = pGmacDev->pGmacBase;
	pDmaReg = pGmacDev->pDmaBase;

	netif_stop_queue(pNetDev);
	netif_carrier_off(pNetDev);

	// rx, tx disable
	pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN) ;
	pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);

	// all interrupt disable
	pDmaReg->intrEnable = 0;
	pDmaReg->status = pDmaReg->status;	// Clear interrupt pending register

	// phy control

	// skb control
	sdpGmac_clearAllDesc(pNetDev);

	pGmacDev->phydev = NULL;

 	if(pGmacDev->pRxDummySkb)
		dev_kfree_skb(pGmacDev->pRxDummySkb);	//0.940

	dma_free_coherent(pGmacDev->pDev, TX_DESC_SIZE,
				pGmacDev->pTxDesc, pGmacDev->txDescDma);

	dma_free_coherent(pGmacDev->pDev, RX_DESC_SIZE,
				pGmacDev->pRxDesc, pGmacDev->rxDescDma);

	free_irq(pNetDev->irq, pNetDev);
	iounmap(pGmacDev->pGmacBase);
	iounmap(pGmacDev->pTimeStampBase);
	iounmap(pGmacDev->pMac2ndBlk);
	iounmap(pGmacDev->pDmaBase);

	free_netdev(pNetDev);
	platform_set_drvdata(pDev, NULL);

	DPRINTK_GMAC_FLOW ("exit\n");

	return 0;
}

//static struct platform_device *gpSdpGmacPlatDev;

static struct platform_driver sdpGmac_device_driver = {
	.probe		= sdpGmac_drv_probe,
	.remove		= __devexit_p(sdpGmac_drv_remove),
	.driver		= {
		.name	= ETHER_NAME,
		.owner  = THIS_MODULE,
	},
#ifdef CONFIG_PM
	.suspend	= sdpGmac_drv_suspend,
	.resume		= sdpGmac_drv_resume,
#endif
};


/* Module init and exit */
static int __init sdpGmac_init(void)
{
	int retVal = 0;

	DPRINTK_GMAC_FLOW ("called\n");
#if 0
	gpSdpGmacPlatDev = platform_device_alloc(ETHER_NAME, -1);

	if(!gpSdpGmacPlatDev)
		return -ENOMEM;

	retVal = platform_device_add(gpSdpGmacPlatDev);
	if(retVal) goto __put_dev;
	retVal = platform_driver_register(&sdpGmac_device_driver);
	if(retVal == 0) goto __out;
	platform_device_del(gpSdpGmacPlatDev);
	__put_dev:
	platform_device_put(gpSdpGmacPlatDev);
__out:
#else
	retVal = platform_driver_register(&sdpGmac_device_driver);
	if(retVal) retVal = -ENODEV;		// 0.959
#endif
	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;
}


static void __exit sdpGmac_exit(void)
{
	DPRINTK_GMAC_FLOW ("called\n");

	platform_driver_unregister(&sdpGmac_device_driver);
//	platform_device_unregister(gpSdpGmacPlatDev);

	DPRINTK_GMAC_FLOW ("exit\n");
}


module_init(sdpGmac_init);
module_exit(sdpGmac_exit);

MODULE_AUTHOR("VD Division, Samsung Electronics Co.");
MODULE_LICENSE("GPL");

