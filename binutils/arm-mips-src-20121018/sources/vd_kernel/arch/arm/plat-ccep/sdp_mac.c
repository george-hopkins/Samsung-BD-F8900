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
 * 2011.11.30,tukho.kim: bug fix, sdpGmac_txQptr_replacement and add mac dma hw info 0.962
 * 2011.12.07,tukho.kim: bug fix, sdpGmac_setTxQptr ring buffer 0.963
 * 2011.12.07,tukho.kim: bug fix, phy initialize by lpa register 0.964
 * 2011.12.14,tukho.kim: bug fix, control variable and hw memory 0.965
 * 2011.12.17,tukho.kim: bug fix, del_timer for smp 0.966
 * 2012.01.02,tukho.kim: bug fix, tx dma status error 0.967
 * 2012.01.02,tukho.kim: remove to set adcaps to limit 10MBps and full duplex 0.968
 */ 

#include <linux/time.h>		// for debugging
#include <linux/mii.h>		

//#include "sdpGmac_env.h"
#include "sdp_mac.h"

#define GMAC_DRV_VERSION	"0.968"	

// 8201 
#define PHY_RTL8201     0x001CC810
#define PHY_RTL8201E	0x001CC815
#define PHY_RTL8201F	0x001CC816

// 8211
#define PHY_RTL8211     0x001CC910
#define PHY_RTL8211E	0x001CC915

extern int sdp_get_revision_id(void);

static int sdpGmac_reset(struct net_device *pNetDev);
static int sdpGmac_getMacAddr(struct net_device *pNetDev, u8 *pAddr);
static void sdpGmac_setMacAddr(struct net_device *pNetDev, const u8 *pAddr );
static int sdpGmac_mdioRead(struct net_device *pNetDev, int phyId, int loc);
static void sdpGmac_mdioWrite(struct net_device *pNetDev , int phyId, int loc, int val);
static void sdpGmac_dmaInit(struct net_device *pNetDev);	
static void sdpGmac_gmacInit(struct net_device *pNetDev);	
static int sdpGmac_setRxQptr(SDP_GMAC_DEV_T* pGmacDev, const DMA_DESC_T* pDmaDesc, const u32 length2);
static int sdpGmac_getRxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc);
static int sdpGmac_setTxQptr(SDP_GMAC_DEV_T* pGmacDev, 
		const DMA_DESC_T* pDmaDesc, const u32 length2, const int offloadNeeded);
static int sdpGmac_getTxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc);
static void sdpGmac_clearAllDesc(struct net_device *pNetDev);

#ifdef CONFIG_SMP	//0.966
# define	SDP_DEL_TIMER(a)	del_timer_sync(a)
#else
# define	SDP_DEL_TIMER(a)	del_timer(a)
#endif


// 0.954
static inline void 
sdpGmac_skbAlign(struct sk_buff *pSkb) {	
	u32 align = (u32)pSkb->data & 3;

	switch(align){
		case 1:
		case 3:
			skb_reserve(pSkb, align);
			break;
		case 2:
			break;
		default:
			skb_reserve(pSkb, 2);
	}
} // 0.954 end

// Realtek 8201E
static inline void rtl82x1_phyReset(struct net_device *pNetDev)
{
	u16 phyVal;
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	u8 phyId = pGmacDev->phyId;

	DPRINTK_GMAC_FLOW("called\n");
// phy reset 
	phyVal = sdpGmac_mdioRead(pNetDev, phyId, MII_BMCR);
	phyVal = phyVal | (1 << 15);
	sdpGmac_mdioWrite(pNetDev, phyId, MII_BMCR, phyVal);	

	while(1){
		phyVal = sdpGmac_mdioRead(pNetDev, phyId, MII_BMCR);
		if(!(phyVal & (1 << 15))) break;
	}

//	if(pGmacDev->phyType != PHY_RTL8201E) return;

	// caution!!!! if change phy, Must check this part
	// Test regiset Clock output set 
	phyVal = sdpGmac_mdioRead(pNetDev, phyId, MII_PHYADDR);
	phyVal = phyVal & ~(1 << 11);
#if 1	// 0.9431
	phyVal = phyVal & ~(1 << 13);  // 0.9431 
#else 	// 0.9591
	phyVal = phyVal | (1 << 13);   // 0.9591
#endif
	sdpGmac_mdioWrite(pNetDev, phyId, MII_PHYADDR, phyVal);	

#ifdef CONFIG_ARCH_SDP1004
if(pGmacDev->revision == 0) {
	printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printk("!! Ethernet 10MBps-Half duplex fixed       !!\n");
	printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	sdpGmac_mdioWrite(pNetDev, phyId, MII_ADVERTISE, 0x21);	
}
#endif
	DPRINTK_GMAC_FLOW("exit\n");
}

static void sdpGmac_netDev_tx(struct net_device *pNetDev)
{
	SDP_GMAC_DEV_T	*pGmacDev = netdev_priv(pNetDev);

	struct net_device_stats *pNetStats = &pGmacDev->netStats;
	DMA_DESC_T	txDesc;

	u32 checkStatus;
	int descIndex;
	
	DPRINTK_GMAC_FLOW("called\n");

#if 1
	do{
		descIndex = sdpGmac_getTxQptr(pGmacDev, &txDesc);

		if(descIndex < 0) break;	// break condition
		if(!txDesc.data1) continue;	// not receive
	
		DPRINTK_GMAC_FLOW("Tx Desc %d for skb 0x%08x whose status is %08x\n",
					descIndex, txDesc.data1, txDesc.status);

		//TODO : IPC_OFFLOAD  ??? H/W Checksum	
		//
		//
		//
		
		dma_unmap_single(pGmacDev->pDev, txDesc.buffer1, 
				txDesc.length, DMA_FROM_DEVICE);
	
		dev_kfree_skb_irq((struct sk_buff*)txDesc.data1);		

		// ENH_MODE
		checkStatus = (txDesc.status & 0xFFFF) & DESC_ERROR;

		if(!checkStatus) {
			pNetStats->tx_bytes += txDesc.length & DESC_SIZE_MASK;
			pNetStats->tx_packets++;
		}
		else {	// ERROR
			pNetStats->tx_errors++;

			if(txDesc.status & (DESC_TX_LATE_COLL | DESC_TX_EXC_COLL)) 
				pNetStats->tx_aborted_errors++ ;
			if(txDesc.status & (DESC_TX_LOST_CARR | DESC_TX_NO_CARR)) 
				pNetStats->tx_carrier_errors++;
		}

		pNetStats->collisions += DESC_TX_COLLISION(txDesc.status);

	}while(1);

	netif_wake_queue(pNetDev);
#endif

	DPRINTK_GMAC_FLOW("exit\n");
	return;
}



static int sdpGmac_netDev_rx(struct net_device *pNetDev)
{
	SDP_GMAC_DEV_T	*pGmacDev = netdev_priv(pNetDev);

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

		dma_unmap_single(pGmacDev->pDev, rxDesc.buffer1, 0, DMA_FROM_DEVICE);
	
		pSkb = (struct sk_buff*)rxDesc.data1;		
// 0.95
		checkStatus = rxDesc.status & (DESC_ERROR | DESC_RX_LEN_ERR);

// rx byte
		if(!checkStatus &&
		   (rxDesc.status & DESC_RX_LAST) && 
		   (rxDesc.status & DESC_RX_1ST) ) 
		{
			len = DESC_GET_FRAME_LEN(rxDesc.status) - 4; // without crc byte

			skb_put(pSkb,len);

			//TODO : IPC_OFFLOAD  ??? H/W Checksum	
			
			// Network buffer post process
			pSkb->dev = pNetDev;
			pSkb->protocol = eth_type_trans(pSkb, pNetDev);

			netif_rx(pSkb);		// post buffer to the network code 

			pNetDev->last_rx = jiffies;
			pNetStats->rx_packets++;
			pNetStats->rx_bytes += len;
			
		} else {	// ERROR
			pNetStats->rx_errors++;
			
			if(rxDesc.status & DESC_RX_OVERFLOW) pNetStats->rx_over_errors++;	// 0.95
			if(rxDesc.status & DESC_RX_COLLISION) pNetStats->collisions++;
			if(rxDesc.status & DESC_RX_WATCHDOG) pNetStats->rx_frame_errors++;	// 0.95
			if(rxDesc.status & DESC_RX_CRC) pNetStats->rx_crc_errors++ ;
//			if(rxDesc.status & DESC_RX_DRIBLING) pNetStats->rx_frame_errors++ ;	// 0.95
			if(rxDesc.status & DESC_RX_LEN_ERR) pNetStats->rx_length_errors++;	// 0.95

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
					   skb_tailroom(pSkb),
					   DMA_FROM_DEVICE);

		len = skb_tailroom(pSkb) + (SDP_GMAC_BUS >> 3) - 1;
		len &= SDP_GMAC_BUS_MASK;
		CONVERT_DMA_QPTR(rxDesc, len, rxDmaAddr, pSkb, 0, 0);

		if(sdpGmac_setRxQptr(pGmacDev, &rxDesc, 0) < 0){
			if(pSkb != pGmacDev->pRxDummySkb) {	// 0.940
				dev_kfree_skb_irq(pSkb);	
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
	
	int idx = 0;

	pDmaReg->operationMode &= ~B_RX_EN;
	pGmacReg->configuration &= ~B_RX_ENABLE;	
	
	for (idx = 0; idx < NUM_RX_DESC; idx++) {
		if(!(pRxDesc->status & DESC_OWN_BY_DMA)) {
			pGmacDev->rxNext = idx;
			pGmacDev->pRxDescNext = pRxDesc;
			pGmacDev->rxBusy = idx;
			pGmacDev->pRxDescBusy = pRxDesc;

			sdpGmac_netDev_rx(pNetDev);  // check buffer 
			
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

	}

	if(intrStatus & B_EARLY_TX) {
		DPRINTK_GMAC_ERROR("Early Tx Error\n");
		// TODO : 
	}

	if(intrStatus & B_RX_WDOG_TIMEOUT) {
		DPRINTK_GMAC_ERROR("Rx WatchDog timeout Error\n");
		// TODO : 
	}

	if(intrStatus & B_RX_STOP_PROC) {
		DPRINTK_GMAC_FLOW("Rx process stop\n");
		// TODO : 
	}

	if(intrStatus & B_RX_BUF_UNAVAIL) {
		DPRINTK_GMAC_FLOW("Rx buffer unavailable Error %d\n", nrChkDesc);
		sdpGmac_rxDescRecovery(pNetDev);	
	} 
	
	if(intrStatus & B_RX_OVERFLOW) {
		DPRINTK_GMAC_ERROR("Rx overflow Error\n");
		// TODO : 
	}

	if(intrStatus & B_TX_UNDERFLOW) {
		DPRINTK_GMAC_ERROR("Tx underflow Error\n");
		// TODO : tx descriptor fix
	}

	if(intrStatus & B_TX_JAB_TIMEOUT) {
		DPRINTK_GMAC_ERROR("Tx jabber timeout Error\n");
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
//	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;	
	SDP_GMAC_MMC_T *pMmcReg = pGmacDev->pMmcBase;	
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;	

	u32 intrStatus;
	int nrChkDesc = 0;

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

	if(intrStatus == 0) return IRQ_NONE;

	pDmaReg->status = intrStatus;	// Clear interrupt pending register 

	// DMA Self Clear bit is check
	
	if(intrStatus & B_TIME_STAMP_TRIGGER) {
		DPRINTK_GMAC("INTR: time stamp trigger\n");
	}

	if(intrStatus & B_PMT_INTR) {
		// TODO: make pmt resume function 
		DPRINTK_GMAC("INTR: PMT interrupt\n");
	}

	if(intrStatus & B_MMC_INTR) {
		// TODO: make reading mmc intr rx and tx register 
		// this register is hidden in Datasheet
		DPRINTK_GMAC_ERROR("INTR: MMC rx: 0x%08x\n", pMmcReg->intrRx);
		DPRINTK_GMAC_ERROR("INTR: MMC tx: 0x%08x\n", pMmcReg->intrTx);
		DPRINTK_GMAC_ERROR("INTR: MMC ipc offload: 0x%08x\n", pMmcReg->mmcIpcIntrRx);
	}

	if(intrStatus & B_LINE_INTF_INTR) {
		DPRINTK_GMAC("INTR: line interface interrupt\n");
	}

// Normal interrupt
	if(intrStatus & B_RX_INTR) {
		nrChkDesc = sdpGmac_netDev_rx(pNetDev);	
	}

// sample source don't support Early Rx, Tx
	if(intrStatus & B_EARLY_RX) {
	
	}

	if(intrStatus & B_TX_BUF_UNAVAIL) {
	
	}

	if(intrStatus & B_TX_INTR) {
		sdpGmac_netDev_tx(pNetDev);	
	}

// ABNORMAL Interrupt
	if(intrStatus & B_ABNORM_INTR_SUM) {
		sdpGmac_abnormal_intr_status (intrStatus, pNetDev);
	}

//	pDmaReg->status = intrStatus;	// Clear interrupt pending register 

	DPRINTK_GMAC_FLOW("exit\n");

	return IRQ_HANDLED;
}

/*
 *	sdpGmacBase.c 
 */

static int 
sdpGmac_mdioRead(struct net_device *pNetDev, int phyId, int loc)
{
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	unsigned long flags;
	u32 addr, data;


	DPRINTK_GMAC_FLOW ("called phyId: %d, loc: %02x\n", phyId, loc);

	addr = GMII_ADDR_READ (phyId, loc);

	spin_lock_irqsave(&pGmacDev->phy_lock, flags);		// 0.951

	while(pGmacReg->gmiiAddr & B_GMII_ADDR_BUSY);

	pGmacReg->gmiiAddr = addr;
	while(pGmacReg->gmiiAddr & B_GMII_ADDR_BUSY);

	data = pGmacReg->gmiiData & 0xFFFF;
	
	spin_unlock_irqrestore(&pGmacDev->phy_lock, flags);		// 0.951

	DPRINTK_GMAC_FLOW ("exit\n");

	return data;
}

static void 
sdpGmac_mdioWrite(struct net_device * pNetDev, int phyId, int loc, int val)
{
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	unsigned long flags;
	u32 addr, data;

	spin_lock_irqsave(&pGmacDev->phy_lock, flags);		// 0.951
//	DPRINTK_GMAC ("called phyId: %d, loc: %02x\n", phyId, loc);
	while(pGmacReg->gmiiAddr & B_GMII_ADDR_BUSY);

	pGmacReg->gmiiData = val & 0xFFFF;

	addr = GMII_ADDR_WRITE(phyId, loc);

	pGmacReg->gmiiAddr = addr;
	while(pGmacReg->gmiiAddr & B_GMII_ADDR_BUSY);

	data = pGmacReg->gmiiData & 0xFFFF;
	
	spin_unlock_irqrestore(&pGmacDev->phy_lock, flags);		// 0.951
//	DPRINTK_GMAC ("exit\n");

	return;
}

static int 
sdpGmac_reset(struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T * pDmaReg = pGmacDev->pDmaBase;
	
	u32 regVal;
	u16 phyVal;

/* version check */
	switch(pGmacReg->version & 0xFF) {
		case GMAC_SYNOP_VER:
		case (0x36):		// sdp1103, sdp1106
			PRINTK_GMAC("Find SDP GMAC ver %04x\n", pGmacReg->version);	
			break;
	        default:		
			DPRINTK_GMAC_ERROR("Can't find GMAC!!!!\n");
			return -SDP_GMAC_ERR;
			break;
	}

/* Porting issue by SoC and Platform */
	SDP_GMAC_PAD_INIT;

// set phy id
	pGmacDev->phyId = GMAC_PHY_ID;

	for (regVal=0; regVal < 100000; regVal++) {
		phyVal = sdpGmac_mdioRead(pNetDev, GMAC_PHY_ID, MII_PHYSID1);
		if(phyVal != 0xFFFF) break;
		udelay(10);					// wait to init phy
	}

	if(regVal && (phyVal != 0xFFFF)) {
		printk(KERN_INFO"[%s] To find phy takes time %d0us\n", __FUNCTION__, regVal);
	}

	pGmacDev->phyType = phyVal << 16;	
	phyVal = sdpGmac_mdioRead(pNetDev, GMAC_PHY_ID, MII_PHYSID2);
	pGmacDev->phyType |= phyVal;	


#if 1 //RTL8201E clock output
	switch (pGmacDev->phyType & 0xFFFFFFF0) {
		case (PHY_RTL8201):
			PRINTK_GMAC("Find RTL8201%c 100MBps Phy Device\n", 
				 ((pGmacDev->phyType & 0xF) + 'A' - 1));	
			rtl82x1_phyReset(pNetDev);
			break;
		case (PHY_RTL8211):
			PRINTK_GMAC("Find RTL8211%c 1GBps Phy Device\n",
				 ((pGmacDev->phyType & 0xF) + 'A' - 1));	
			rtl82x1_phyReset(pNetDev);
			pGmacDev->mii.supports_gmii = 1;
			break;
		default:
			DPRINTK_GMAC_ERROR("Not support PHY device 0x%08x\n", pGmacDev->phyType);
			return -1;
			break;			
	}
#endif
// For Debug end

/* sdpGmac Reset */
// DMA Reset 
	pDmaReg->busMode = B_SW_RESET; // '0x1'

// caution!!!! 
// if don't recive phy clock, this code is hang 
// 0.956
	udelay(5);	// need 3 ~ 5us 
	do {
		if(pDmaReg->busMode & B_SW_RESET) {
			DPRINTK_GMAC("check phy clock output register 0x19(0x1E40)\n");	
			msleep(1000);
		} else {	
			DPRINTK_GMAC("DMA reset is release(normal status)\n");	
			break;
		}
	} while(1);
// 0.956 end

// all interrupt disable 
	pGmacReg->intrMask = 0x20F;
	regVal = pGmacReg->interrupt; // clear status register 
	pDmaReg->intrEnable = 0;
	pDmaReg->status = 0x0001E7FF;  // clear status register 

// function define
	pGmacDev->hwSpeed = STATUS_SPEED_100;	// 0.953  must same to init configuration register 
	pGmacDev->hwDuplex = 1;		   	// must same to init configuration register	
	pGmacDev->msg_enable = NETIF_MSG_LINK;
	pGmacDev->oldBmcr = BMCR_ANENABLE;

	return retVal;
}

static int 
sdpGmac_getMacAddr(struct net_device *pNetDev, u8* pMacAddr)
{
	int retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	unsigned long flags;
	u32 macAddr[2] = {0, 0};

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock_irqsave(&pGmacDev->lock, flags);

	macAddr[0] = pGmacReg->macAddr_00_Low;
	macAddr[1] = pGmacReg->macAddr_00_High & 0xFFFF;

	spin_unlock_irqrestore(&pGmacDev->lock, flags);
	
	DPRINTK_GMAC_DBG("macAddrLow is 0x%08x\n", macAddr[0]);
	DPRINTK_GMAC_DBG ("macAddrHigh is 0x%08x\n", macAddr[1]);
	memcpy(pMacAddr, macAddr, N_MAC_ADDR);

	flags = macAddr[0] + macAddr[1] ;

	if(flags == 0)	retVal = -SDP_GMAC_ERR;
	else if ((macAddr[0] == 0xFFFFFFFF) &&
		 (macAddr[1] == 0x0000FFFF)) retVal = -SDP_GMAC_ERR;

	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;

}

static void 
sdpGmac_setMacAddr(struct net_device * pNetDev, const u8* pMacAddr)
{
	unsigned long flags;
	u32 macAddr[2] = {0, 0};
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	DPRINTK_GMAC_FLOW ("called\n");

	memcpy(macAddr, pMacAddr, N_MAC_ADDR);

	spin_lock_irqsave(&pGmacDev->lock, flags);

	pGmacReg->macAddr_00_Low = macAddr[0] ;
	pGmacReg->macAddr_00_High = macAddr[1] | (1 << 31);
#ifdef CONFIG_ARCH_SDP1106
	pGmacReg->macAddr_00_Low = macAddr[0]; // re-init

	pGmacReg->macAddr_15_Low = macAddr[0];
	pGmacReg->macAddr_15_High = macAddr[1] | (1 << 31);
#endif
	spin_unlock_irqrestore(&pGmacDev->lock, flags);
	
	DPRINTK_GMAC_FLOW ("exit\n");
}

static void 
sdpGmac_dmaInit(struct net_device * pNetDev)
{
	unsigned long flags;
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_DMA_T * pDmaReg = pGmacDev->pDmaBase;

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock_irqsave(&pGmacDev->lock, flags);

#if (SDP_GMAC_BUS > 32)  // word(bus width) skip length -> add 2 integer data to dma descriptor size 
	pDmaReg->busMode = B_FIX_BURST_EN | B_BURST_LEN(8) | B_DESC_SKIP_LEN(1);  
#else
	pDmaReg->busMode = B_FIX_BURST_EN | B_BURST_LEN(8) | B_DESC_SKIP_LEN(2);  
#endif
	DPRINTK_GMAC_DBG ("busMode set %08x\n", pDmaReg->busMode);

	pDmaReg->operationMode = B_TX_STR_FW | B_TX_THRESHOLD_192;
	DPRINTK_GMAC_DBG ("opMode set %08x\n", pDmaReg->operationMode);

	spin_unlock_irqrestore(&pGmacDev->lock, flags);
	
	DPRINTK_GMAC_FLOW ("exit\n");
}

static void 
sdpGmac_gmacInit(struct net_device * pNetDev)
{
	unsigned long flags;
	SDP_GMAC_DEV_T* pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T * pGmacReg = pGmacDev->pGmacBase;

	u32 regVal = 0;

	DPRINTK_GMAC_FLOW ("called\n");

	spin_lock_irqsave(&pGmacDev->lock, flags);

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

	spin_unlock_irqrestore(&pGmacDev->lock, flags);
	
	DPRINTK_GMAC_FLOW ("exit\n");
}


static int
sdpGmac_setRxQptr(SDP_GMAC_DEV_T* pGmacDev, const DMA_DESC_T* pDmaDesc, const u32 length2)
{
	int retVal = SDP_GMAC_OK;

	u32 rxNext = pGmacDev->rxNext;
	DMA_DESC_T* pRxDesc = pGmacDev->pRxDescNext;
	u32 length;
	
	unsigned long flag;


	DPRINTK_GMAC_FLOW("called \n");

	if(!sdpGmac_descEmpty(pRxDesc)) { // should empty
		DPRINTK_GMAC_FLOW("Error Not Empty \n");
		retVal =  -SDP_GMAC_ERR;
		goto __setRxQptr_out;
	}

	spin_lock_irqsave(&pGmacDev->lock, flag);

	if(sdpGmac_rxDescChained(pRxDesc)){
		// TODO
		PRINTK_GMAC("rx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		spin_unlock_irqrestore(&pGmacDev->lock, flag);
		goto __setRxQptr_out;

	} else {
		length = (pDmaDesc->length & DESC_SIZE_MASK);
		length |= (length2 & DESC_SIZE_MASK) << DESC_SIZE2_SHIFT;

		if(sdpGmac_lastRxDesc(pGmacDev, pRxDesc)) {
			length |= RX_DESC_END_RING;
		} 
// set descriptor
		pRxDesc->buffer1 = pDmaDesc->buffer1;
		pRxDesc->data1 = pDmaDesc->data1;

		pRxDesc->buffer2 = pDmaDesc->buffer2;
		pRxDesc->data2 = pDmaDesc->data2;

		pRxDesc->length = length;
		wmb();				// 0.958, 0.9592
		pRxDesc->status = DESC_OWN_BY_DMA; 	
		wmb();				// 0.9592

		if(length & RX_DESC_END_RING) {
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

	spin_unlock_irqrestore(&pGmacDev->lock, flag);

	retVal = rxNext;
// if set rx qptr is success, print status
	DPRINTK_GMAC_FLOW("rxNext: %02d  pRxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      rxNext,  (u32)pRxDesc, pRxDesc->status, 
		      pRxDesc->length, pRxDesc->buffer1, pRxDesc->buffer2,
		      pRxDesc->data1, pRxDesc->data2);
__setRxQptr_out:

	DPRINTK_GMAC("exit\n");
	return retVal;
}


// return rxIndex
static int
sdpGmac_getRxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc)
{
	int retVal = SDP_GMAC_OK;	

	u32 rxBusy = pGmacDev->rxBusy;
	DMA_DESC_T *pRxDesc = pGmacDev->pRxDescBusy;

	unsigned long flag;

	DPRINTK_GMAC_FLOW("called\n");
	
	if(pRxDesc->status & DESC_OWN_BY_DMA) {
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


	spin_lock_irqsave(&pGmacDev->lock, flag);

	if(sdpGmac_rxDescChained(pRxDesc)){
		PRINTK_GMAC("rx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		spin_unlock_irqrestore(&pGmacDev->lock, flag);
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

			pRxDesc->length = RX_DESC_END_RING;  // caution !!!!

			pGmacDev->rxBusy = 0;
			pGmacDev->pRxDescBusy = pGmacDev->pRxDesc;

		}
		else {
			memset(pRxDesc, 0, sizeof(DMA_DESC_T));
			pGmacDev->rxBusy = rxBusy + 1;
			pGmacDev->pRxDescBusy = pRxDesc + 1;
		}
	}

	spin_unlock_irqrestore(&pGmacDev->lock, flag);

	retVal = rxBusy;

// if set rx qptr is success, print status
	DPRINTK_GMAC_FLOW("rxBusy: %02d  pRxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      rxBusy,  (u32)pRxDesc, pRxDesc->status, 
		      pRxDesc->length, pRxDesc->buffer1, pRxDesc->buffer2,
		      pRxDesc->data1, pRxDesc->data2);

__getRxQptr_out:
	DPRINTK_GMAC("exit\n");

	return retVal;
}

#if 0
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
	
	unsigned long flag;

	opReg = pDmaReg->curHostTxDesc;
	status = pDmaReg->txDescListAddr;
	n_dmaTxCurrent = opReg - status;	
	n_dmaTxCurrent = n_dmaTxCurrent / sizeof(DMA_DESC_T);


	if(
	   ((txNext > txBusy) && ((txNext < n_dmaTxCurrent) || (n_dmaTxCurrent < txBusy))) || 	// 0.962	//0.965
	   ((txNext < txBusy) && ((txNext < n_dmaTxCurrent) && (n_dmaTxCurrent < txBusy)))	// 0.962	//0.965
	  ){
		DPRINTK_GMAC_ERROR("Run Replacement txqptr \n");

		opReg = pDmaReg->operationMode;
		pDmaReg->operationMode &= ~(B_TX_EN) ;

		spin_lock_irqsave(&pGmacDev->lock, flag);

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

		spin_unlock_irqrestore(&pGmacDev->lock, flag);
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
#endif

static int
sdpGmac_setTxQptr(SDP_GMAC_DEV_T* pGmacDev, 
		const DMA_DESC_T* pDmaDesc, const u32 length2, const int offloadNeeded)
{
	int retVal = SDP_GMAC_OK;

	u32 txNext = pGmacDev->txNext;
	DMA_DESC_T *pTxDesc = pGmacDev->pTxDescNext;
	u32 length;
	u32 status;

	unsigned long flag;
	
	DPRINTK_GMAC_FLOW("called\n");

	if(pTxDesc->status & DESC_OWN_BY_DMA) {
		DPRINTK_GMAC_FLOW("Err: DESC_OWN_BY_DMA\n");
		retVal = -SDP_GMAC_ERR;	
		goto __setTxQptr_out;
	}

#if 0
	if(!sdpGmac_descEmpty(pTxDesc)){
		DPRINTK_GMAC_ERROR("pTxDesc must be empty\n");
		retVal =  -SDP_GMAC_ERR;
		goto __setTxQptr_out;
	}
#endif

// 0.963
	status = txNext + 1;	
	status = status % NUM_TX_DESC;
	
	if(status == pGmacDev->txBusy){
	//	DPRINTK_GMAC_ERROR("Tx Desc is Full txNext:%d, txBusy:%d\n", txNext, pGmacDev->txBusy);
		retVal =  -SDP_GMAC_ERR;
		goto __setTxQptr_out;
	}
// 0.963 end
//
	spin_lock_irqsave(&pGmacDev->lock, flag);

	if(sdpGmac_txDescChained(pTxDesc)) {
		// TODO: chained 
		PRINTK_GMAC("tx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		spin_unlock_irqrestore(&pGmacDev->lock, flag);
		goto __setTxQptr_out;
	}
	else {
		u32 updateTxNext;		// 0.965

		length = (pDmaDesc->length & DESC_SIZE_MASK);
		length |= (length2 & DESC_SIZE_MASK) << DESC_SIZE2_SHIFT;

		status = DESC_OWN_BY_DMA | DESC_TX_1ST | DESC_TX_LAST | DESC_TX_INT_EN;	// status
	
		if (offloadNeeded) {
			PRINTK_GMAC("H/W Checksum Not Implenedted yet\n");
		}		

		if(sdpGmac_lastTxDesc(pGmacDev, pTxDesc)) {
			updateTxNext = pGmacDev->txNext + 1;	// 0.965
			if(updateTxNext != NUM_TX_DESC) 
				DPRINTK_GMAC_ERROR("Tx Desc number is wrong %d:%d\n",NUM_TX_DESC, pGmacDev->txNext);
			updateTxNext = 0;
			pGmacDev->pTxDescNext = pGmacDev->pTxDesc;
			status |= DESC_TX_END_RING;
		}
		else {
			updateTxNext = txNext + 1;		// 0.965
			pGmacDev->pTxDescNext = pTxDesc + 1;
		}

		pTxDesc->buffer1 = pDmaDesc->buffer1;
		pTxDesc->data1	= pDmaDesc->data1;
		pTxDesc->buffer2 = pDmaDesc->buffer2;
		pTxDesc->data2	= pDmaDesc->data2;
		pTxDesc->length = length;	// length		????
		wmb();						// 0.958, 0.9592
		pTxDesc->status = status;
		wmb();						// 0.9592
		if(pTxDesc->status != status) wmb();		// 0.965

		pGmacDev->txNext = updateTxNext;		// 0.965
		// ENH DESC
	}
	
	spin_unlock_irqrestore(&pGmacDev->lock, flag);

// if set tx qptr is success, print status
	DPRINTK_GMAC_FLOW("txNext: %02d  pTxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      txNext,  (u32)pTxDesc, pTxDesc->status, 
		      pTxDesc->length, pTxDesc->buffer1, pTxDesc->buffer2,
		      pTxDesc->data1, pTxDesc->data2);


	retVal = txNext;	

__setTxQptr_out:
	DPRINTK_GMAC("exit\n");
	return retVal;
}


// Read 
static int
sdpGmac_getTxQptr(SDP_GMAC_DEV_T* pGmacDev, DMA_DESC_T *pDmaDesc)
{
	int retVal = SDP_GMAC_OK;	

	u32 txBusy = pGmacDev->txBusy;
	u32 status;
	DMA_DESC_T *pTxDesc = pGmacDev->pTxDescBusy;
	unsigned long flag;

	DPRINTK_GMAC_FLOW ("called\n");
	
	if(pTxDesc->status & DESC_OWN_BY_DMA) {
		DPRINTK_GMAC_FLOW("Err: DESC_OWN_BY_DMA\n");
		retVal = -SDP_GMAC_ERR;	
		goto __getTxQptr_out;
	}
	
//	if(sdpGmac_descEmpty(pTxDesc)) {
	if(txBusy == pGmacDev->txNext) {
		DPRINTK_GMAC_FLOW("Err: Desc Empty\n");
		retVal = -SDP_GMAC_ERR;	
		goto __getTxQptr_out;
	}

	*pDmaDesc = *pTxDesc; // get Descriptor data	

	spin_lock_irqsave(&pGmacDev->lock,flag);

	if(sdpGmac_txDescChained(pTxDesc)){
		PRINTK_GMAC("tx chained desc Not Implenedted yet\n");
		retVal =  -SDP_GMAC_ERR;
		spin_unlock_irqrestore(&pGmacDev->lock,flag);
		goto __getTxQptr_out;
	}
	else {
		u32 updateTxBusy;		//0.965

		if(sdpGmac_lastTxDesc(pGmacDev, pTxDesc)) {
			DPRINTK_GMAC_FLOW ("Get desc last Tx desc\n");
			updateTxBusy = 0;	//0.965
			pGmacDev->pTxDescBusy = pGmacDev->pTxDesc;
			status = TX_DESC_END_RING;
		}
		else {
			updateTxBusy = txBusy + 1;	//0.965
			pGmacDev->pTxDescBusy = pTxDesc + 1;
			status = 0;
		}

#if 0	// 0.967-
                {
                        SDP_GMAC_DMA_T * pDmaReg = pGmacDev->pDmaBase;
                        u32 nr_Desc_num;
			unsigned long delay = jiffies + HZ/100; //10ms

                        do{
				nr_Desc_num = pDmaReg->curHostTxDesc - pDmaReg->txDescListAddr;
				nr_Desc_num = nr_Desc_num/sizeof(DMA_DESC_T);
				if(pGmacDev->txNext == nr_Desc_num) { 
					udelay(1); 
					break; 
				}
				if(time_after(jiffies, delay)) break;
			}while(txBusy == nr_Desc_num) ;
		}
		pTxDesc->length = 0;
		pTxDesc->data1 = 0;
		pTxDesc->buffer1 = 0;
		pTxDesc->data2 = 0;
		pTxDesc->buffer2 = 0;
		wmb();
		pTxDesc->status = status;
		wmb();
		if(pTxDesc->status != status) wmb();	//0.965
#endif
		pGmacDev->txBusy = updateTxBusy;	//0.965
	}

	spin_unlock_irqrestore(&pGmacDev->lock,flag);

	retVal = txBusy;

// if set rx qptr is success, print status
	DPRINTK_GMAC_FLOW("txBusy: %02d  pTxDesc: %08x status: %08x \n"
		      "length: %08x  buffer1: %08x buffer2: %08x \n"
		      "data1: %08x  data2: %08x\n",
		      txBusy,  (u32)pTxDesc, pTxDesc->status, 
		      pTxDesc->length, pTxDesc->buffer1, pTxDesc->buffer2,
		      pTxDesc->data1, pTxDesc->data2);

__getTxQptr_out:
	DPRINTK_GMAC_FLOW("exit\n");

	return retVal;
}


static void 
sdpGmac_clearAllDesc(struct net_device *pNetDev)
{

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	DMA_DESC_T *pDmaDesc;
	u32 length2;
	int i;
	int txBusy;



// RX DESC Clear
	pDmaDesc = pGmacDev->pRxDesc;

	for(i = 0; i < NUM_RX_DESC; i ++){
		length2 = DESC_GET_SIZE2(pDmaDesc->length);
		pDmaDesc->length &= (DESC_SIZE1_MASK | RX_DESC_END_RING);

		if((pDmaDesc->length) && (pDmaDesc->data1)){
			DPRINTK_GMAC("rx desc %d clear length %x, data1 %x\n", 
				 i, pDmaDesc->length, pDmaDesc->data1);
			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer1, 
					0, DMA_FROM_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data1);
			
			pDmaDesc->length &= RX_DESC_END_RING;			
		}

		if((length2) && (pDmaDesc->data2)){
			DPRINTK_GMAC("rx desc2 %d clear length2 %x, data2 %x\n", 
				 i, length2, pDmaDesc->data2);

			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer2, 
					0, DMA_FROM_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data2);
		}

		pDmaDesc++;
			
	}

	pGmacDev->pRxDesc[NUM_RX_DESC-1].length = RX_DESC_END_RING;
	DPRINTK_GMAC_FLOW("last rx desc length is 0x%08x\n", pGmacDev->pRxDesc[NUM_RX_DESC-1].length);

	pGmacDev->rxNext = 0;
	pGmacDev->rxBusy = 0;
	pGmacDev->pRxDescBusy = pGmacDev->pRxDesc;
	pGmacDev->pRxDescNext = pGmacDev->pRxDesc;

// TX DESC Clear
	pDmaDesc = pGmacDev->pTxDesc;
	txBusy = pGmacDev->txBusy;			// 0.967


	for(i = 0; i < NUM_TX_DESC; i ++){	

		if(txBusy == pGmacDev->txNext) break;		//0.967		
		pDmaDesc = pGmacDev->pTxDesc + txBusy;  	//0.967

		length2 = DESC_GET_SIZE2(pDmaDesc->length);
		pDmaDesc->length &= DESC_SIZE1_MASK;

		if((pDmaDesc->length) && (pDmaDesc->data1)){
			DPRINTK_GMAC("tx desc %d clear length %x, data1 %x\n", 
				 txBusy, pDmaDesc->length, pDmaDesc->data1);	//0.967

			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer1, 
					0, DMA_TO_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data1);
			pDmaDesc->length = 0;

			pDmaDesc->status &= TX_DESC_END_RING;
		}

		if((length2) && (pDmaDesc->data2)){
			DPRINTK_GMAC("tx desc2 %d clear length2 %x, data2 %x\n", 
				 txBusy, length2, pDmaDesc->data2);		// 0.967

			dma_unmap_single(pGmacDev->pDev, pDmaDesc->buffer2, 
					0, DMA_TO_DEVICE);
			dev_kfree_skb((struct sk_buff*) pDmaDesc->data2);
		}

//		pDmaDesc++;				// 0.967-
		txBusy++;				// 0.967
		if(txBusy >= NUM_TX_DESC) txBusy = 0;	// 0.967
	}
	
	pGmacDev->txNext = 0;
	pGmacDev->txBusy = 0;
	pGmacDev->pTxDescBusy = pGmacDev->pTxDesc;
	pGmacDev->pTxDescNext = pGmacDev->pTxDesc;

	return;
}



static int
rtl8201_phyCheckMedia(SDP_GMAC_DEV_T * pGmacDev)
{
        int 		phyId = pGmacDev->phyId;
        unsigned int 	phyVal;

	phyVal =  sdpGmac_mdioRead(pGmacDev->pNetDev, phyId, MII_BMCR);

	if(phyVal & BMCR_ANRESTART) return -1;	

	phyVal &= (BMCR_FULLDPLX | BMCR_SPEED100); 

	if(phyVal != pGmacDev->oldBmcr) {
		DPRINTK_GMAC_FLOW("phy 0x%04x, old 0x%04x\n", phyVal, pGmacDev->oldBmcr);
		pGmacDev->oldBmcr = phyVal;	
	}
	else {
		return -1;
	}

	if (phyVal & BMCR_FULLDPLX) {		// 0.951
		pGmacDev->hwDuplex = STATUS_DUPLEX_FULL;
	} else { // HALF Duplex
		pGmacDev->hwDuplex = STATUS_DUPLEX_HALF;
	}
	
	// TODO : check 10MBps , 100MBps phy status 
	if (phyVal & BMCR_SPEED100) {
		pGmacDev->hwSpeed = STATUS_SPEED_100;
	} else {
		pGmacDev->hwSpeed = STATUS_SPEED_10;
	}

	return 0;
}

static void 
sdpGmac_phyCheckMedia(struct net_device *pNetDev, int init)
{
        SDP_GMAC_DEV_T 	*pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T 	*pGmacReg = pGmacDev->pGmacBase;
	SDP_GMAC_DMA_T 	*pDmaReg = pGmacDev->pDmaBase;

	unsigned long	flags;
	int		idx;
	u32		regVal;
	int		lpa, lpa2 = 0;
	u32		hwSpeed, hwDuplex;
#if defined(CONFIG_ARCH_SDP1106)
	u32 phyVal = 0;
	int ret_mii_link_ok = 0;	
#endif
	DPRINTK_GMAC_FLOW("called\n");

#if defined(CONFIG_ARCH_SDP1106)
	ret_mii_link_ok = mii_link_ok(&pGmacDev->mii);

	if(!mii_check_media(&pGmacDev->mii, netif_msg_link(pGmacDev), init) &&
	   !ret_mii_link_ok&& !init){
		/* The beginning, It's during Phy Link Down for 4 sec, after phy reset : once time */
		
		if(pGmacDev->linkCheckTimeCount > LINK_CHECK_TIME_COUNT){
			return;
		}
		else if(++(pGmacDev->linkCheckTimeCount) == LINK_CHECK_TIME_COUNT){
			sdpGmac_mdioWrite(pNetDev, pGmacDev->phyId, MII_BMCR, (1 << 15));//write Phy Reset
			// wait phy init
			while(1){
				phyVal = sdpGmac_mdioRead(pNetDev, pGmacDev->phyId, MII_BMCR);
				if(!(phyVal & (1<<15))) break;
			}
		    
			// until auto neg 
			sdpGmac_mdioWrite(pNetDev, pGmacDev->phyId, MII_BMCR, BMCR_ANENABLE); //Write Phy Auto negotiation
			while(1){
				phyVal = sdpGmac_mdioRead(pNetDev, pGmacDev->phyId, MII_BMSR);
				if(!(phyVal & (1<<5))) break;
			}
			printk(KERN_ERR"[%s] Phy Reset, Auto negotiation. BMSR_LSTATUS value is 0x%x\n",__FUNCTION__,BMSR_LSTATUS);
			printk(KERN_ERR"[%s] Phy Reset, Auto negotiation. BMSR Status : 0x%04x\n", __FUNCTION__,sdpGmac_mdioRead(pNetDev, pGmacDev->phyId, MII_BMSR));
			printk(KERN_ERR"[%s] Phy Reset, Auto negotiation. BMCR Status : 0x%04x\n", __FUNCTION__,sdpGmac_mdioRead(pNetDev, pGmacDev->phyId, MII_BMCR));
		}
	   return; 
	}

	/* if mii link  is ok, skip the phy reset */
	if(ret_mii_link_ok){ 
		pGmacDev->linkCheckTimeCount = LINK_CHECK_TIME_COUNT + 1;	
	}
#else 
	if(!mii_check_media(&pGmacDev->mii, netif_msg_link(pGmacDev), init) &&
	   !mii_link_ok(&pGmacDev->mii) && !init) return; 
#endif
#ifdef CONFIG_ARCH_SDP1004
if(pGmacDev->revision == 0) {
	sdpGmac_mdioWrite(pNetDev, pGmacDev->phyId, MII_ADVERTISE, 0x21);	
	sdpGmac_mdioWrite(pNetDev, pGmacDev->phyId, MII_BMCR, 0x00);	
	udelay(5);
}
#endif
	if(pGmacDev->phyType == PHY_RTL8201E) {
		if(rtl8201_phyCheckMedia(pGmacDev) < 0) {
			if(!init) return;
		}
	} else {
		lpa  =  sdpGmac_mdioRead(pNetDev, pGmacDev->phyId, MII_LPA);
		if(pGmacDev->mii.supports_gmii) {
			lpa2 =  sdpGmac_mdioRead(pNetDev, pGmacDev->phyId, MII_STAT1000);
		}

		lpa = mii_nway_result(lpa & pGmacDev->mii.advertising);

		hwDuplex = pGmacDev->mii.full_duplex;		
		hwSpeed = (lpa2 & (LPA_1000FULL | LPA_1000HALF)) ? STATUS_SPEED_1000 :
				 lpa & (ADVERTISE_100FULL | ADVERTISE_100HALF) ? 
				STATUS_SPEED_100 : STATUS_SPEED_10;

		if((pGmacDev->hwDuplex == hwDuplex) &&
		   (pGmacDev->hwSpeed == hwSpeed) && !init) return;	// 0.964
		
		pGmacDev->hwDuplex = hwDuplex;
		pGmacDev->hwSpeed = hwSpeed;
	}

	/* duplex state has changed */
	spin_lock_irqsave(&pGmacDev->lock, flags);		// 0.951
	regVal = pGmacReg->configuration; 
	
	pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN) ;
	pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);	
	
	if (pGmacDev->hwDuplex == STATUS_DUPLEX_FULL) {
        	regVal |= B_DUPLEX_FULL;
	} else { // HALF Duplex
	        regVal &= ~B_DUPLEX_FULL;
	}
	
	switch(pGmacDev->hwSpeed) {
		case (STATUS_SPEED_1000):
	    		regVal &= ~B_PORT_MII;
	    		regVal |= B_SPEED_100M;
			break;
		case (STATUS_SPEED_10):
	    		regVal |= B_PORT_MII;
	    		regVal &= ~B_SPEED_100M;
			break;
		default:
	    		regVal |= B_PORT_MII;
	    		regVal |= B_SPEED_100M;
			break;
	}

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
	
	pGmacReg->configuration = regVal; 
	pDmaReg->operationMode |= B_TX_EN | B_RX_EN ;
	
	spin_unlock_irqrestore(&pGmacDev->lock, flags);	
	
	PRINTK_GMAC( "%s: Set %dMBps, %s-duplex \n", 
			pNetDev->name, 
			pGmacDev->hwSpeed, 
			((pGmacDev->hwDuplex == STATUS_DUPLEX_FULL) ? "full" : "half") ); 

	DPRINTK_GMAC_FLOW("exit\n");
}

static void 
sdpGmac_linkCheckTimer(unsigned long arg)
{
	struct net_device *pNetDev = (struct net_device*)arg;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	
	sdpGmac_phyCheckMedia(pNetDev, 0);

	pGmacDev->linkCheckTimer.expires = CFG_LINK_CHECK_TIME + jiffies;
	add_timer(&pGmacDev->linkCheckTimer);
// for test 
	if(!is_valid_ether_addr(pNetDev->dev_addr)) {
		DPRINTK_GMAC_ERROR("%s debug: no valid ethernet hw addr\n",pNetDev->name);
		return;
	}
}

static void 
sdpGmac_phyConf(struct work_struct *pWork)
{
	SDP_GMAC_DEV_T *pGmacDev = 
		container_of(pWork, SDP_GMAC_DEV_T, phyConf);
	struct net_device* pNetDev = pGmacDev->pNetDev;
	unsigned long flags;
	u8 phyId = pGmacDev->phyId;
	int phyCaps; 	// phy Capability
	int adCaps;	// ad Caps

	DPRINTK_GMAC_FLOW("called\n");

	spin_lock_irqsave(&pGmacDev->lock, flags);	

	if(pGmacDev->mii.force_media){
		// TODO	
		PRINTK_GMAC("%s: Not Implemeted yet\n", pNetDev->name);
		spin_unlock_irqrestore(&pGmacDev->lock, flags);
		goto __phyConf_out;
	}

	phyCaps = SDP_GMAC_GET_PHY_BMSR;

        if (!(phyCaps & BMSR_ANEGCAPABLE)) {
                PRINTK_GMAC("%s: Auto negotiation NOT supported\n", pNetDev->name);
		PRINTK_GMAC("%s: Not Implemeted yet\n",pNetDev->name);
		goto __phyConf_out;
        }

       /* CSMA capable w/ both pauses */
       adCaps = ADVERTISE_CSMA | ADVERTISE_PAUSE_CAP | ADVERTISE_PAUSE_ASYM;

        if (phyCaps & BMSR_100BASE4)
                adCaps |= ADVERTISE_100BASE4;
        if (phyCaps & BMSR_100FULL)
                adCaps |= ADVERTISE_100FULL;
        if (phyCaps & BMSR_100HALF)
                adCaps |= ADVERTISE_100HALF;
        if (phyCaps & BMSR_10FULL)
                adCaps |= ADVERTISE_10FULL;
        if (phyCaps & BMSR_10HALF)
                adCaps |= ADVERTISE_10HALF;

	// remove line 0.968

        /* Update our Auto-Neg Advertisement Register */
        SDP_GMAC_SET_PHY_ADV(adCaps);
        pGmacDev->mii.advertising = adCaps;

	udelay(10);

	DPRINTK_GMAC_FLOW("%s: Set value ADV %04x\n", pNetDev->name, adCaps);
	adCaps = SDP_GMAC_GET_PHY_ADV;
	DPRINTK_GMAC_FLOW("%s: Get value ADV %04x\n", pNetDev->name, adCaps);

// TODO: if do BMCR_ANRESTART, can't active RTL 8201E 
//	SDP_GMAC_SET_PHY_BMCR(BMCR_ANENABLE | BMCR_ANRESTART); 
	SDP_GMAC_SET_PHY_BMCR(BMCR_ANENABLE);

	spin_unlock_irqrestore(&pGmacDev->lock, flags);	

	sdpGmac_phyCheckMedia(pNetDev, 1);


__phyConf_out:
	DPRINTK_GMAC_FLOW("exit\n");
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

	netif_stop_queue(pNetDev);

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
#if 0
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
		goto __xmit_out;	// 0.967
	}

	/* start tx */
	pDmaReg->txPollDemand = 0;
	pNetDev->trans_start = jiffies;

	netif_wake_queue(pNetDev);   	// 0.967
__xmit_out:

	DPRINTK_GMAC_FLOW ("exit\n");
	return retVal;
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
	int	idx;

	u32 len;

	DPRINTK_GMAC_FLOW("%s: open called\n", pNetDev->name);

	if(!is_valid_ether_addr(pNetDev->dev_addr)) {
		DPRINTK_GMAC_ERROR("[SDP GMAC]%s: no valid ethernet hw addr\n",pNetDev->name);
		return -EINVAL;
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
					 skb_tailroom(pSkb), DMA_FROM_DEVICE);
		
//		CONVERT_DMA_QPTR(rxInitDesc, skb_tailroom(pSkb), rxDmaAddr, pSkb, 0, 0);
		CONVERT_DMA_QPTR(rxInitDesc, len, rxDmaAddr, pSkb, 0, 0);

		if(sdpGmac_setRxQptr(pGmacDev, &rxInitDesc, 0) < 0){
			dev_kfree_skb(pSkb);	
			break;
		}
	}while(1);

// check line status
	// call sdpGmac_phyConf	
	// speed, Duplex mode set
	sdpGmac_phyConf(&pGmacDev->phyConf);

	// Init link status timer 
	init_timer(&pGmacDev->linkCheckTimer);
	pGmacDev->linkCheckTimer.function = sdpGmac_linkCheckTimer;	
	pGmacDev->linkCheckTimer.data = (u32) pNetDev;	
	pGmacDev->linkCheckTimer.expires = (unsigned long)(CFG_LINK_CHECK_TIME + jiffies);	
	add_timer(&pGmacDev->linkCheckTimer);

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

	netif_start_queue(pNetDev);	

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

	// timer control
	SDP_DEL_TIMER(&pGmacDev->linkCheckTimer);

	DPRINTK_GMAC_FLOW("%s: close exit\n", pNetDev->name);
	return retVal;
}

static void
sdpGmac_netDev_setMulticastList (struct net_device *pNetDev)
{
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;

	u32 frameFilter;
	unsigned long flag;

	DPRINTK_GMAC_FLOW("called\n");

	spin_lock_irqsave(&pGmacDev->lock, flag);
	frameFilter = pGmacReg->frameFilter;
	spin_unlock_irqrestore(&pGmacDev->lock, flag);

	frameFilter &= ~( B_PROMISCUOUS_EN | 
			  B_PASS_ALL_MULTICAST | 
			  B_HASH_PERFECT_FILTER | 
			  B_HASH_MULTICAST );

	if (pNetDev->flags & IFF_PROMISC) {
		PRINTK_GMAC ("%s: PROMISCUOUS MODE\n", pNetDev->name);
		frameFilter |= B_PROMISCUOUS_EN;
	}

#if defined(CONFIG_ARCH_SDP1002) || defined(CONFIG_ARCH_SDP1106)
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

		DPRINTK_GMAC_FLOW ("%s: HASH MULTICAST %d \n", 
					pNetDev->name, pNetDev->mc_count);

		// clear mc list
#if defined(CONFIG_ARCH_SDP1106)
		for(i = 0; i < 14; i++) {
#else
		for(i = 0; i < 15; i++) {
#endif

			if(*mcRegHigh == 0) break;

			spin_lock_irqsave(&pGmacDev->lock, flag);
			*mcRegHigh = 0;
			*mcRegLow = 0;
			spin_unlock_irqrestore(&pGmacDev->lock, flag);

			mcRegLow += 2;
			mcRegHigh += 2;
		}

		// set 
		mcRegHigh = &pGmacReg->macAddr_01_High;
		mcRegLow = &pGmacReg->macAddr_01_Low;
#ifdef CONFIG_ARCH_SDP1002
//		mcRegLow += 2;
//		mcRegHigh += 2;
#endif
		//for(i = 0; i < pNetDev->mc_count; i++, cur_addr = cur_addr->next) {
		netdev_hw_addr_list_for_each(pred, &pNetDev->mc) {

			//DPRINTK_GMAC_FLOW("%s: cur_addr len is %d \n", pNetDev->name, cur_addr->dmi_addrlen);
			DPRINTK_GMAC_FLOW("%s: cur_addr is %d.%d.%d.%d.%d.%d \n", pNetDev->name, 
						pred->addr[0], pred->addr[1],
						pred->addr[2], pred->addr[3],
						pred->addr[4], pred->addr[5] );

			if(!(*pred->addr & 1)) continue;

			mcValLow = (u32*)(pred->addr);
			mcValHigh = mcValLow + 1;	// u32 pointer 
			
			spin_lock_irqsave(&pGmacDev->lock, flag);
			*mcRegLow = *mcValLow;
			*mcRegHigh = 0x80000000 | (*mcValHigh & 0xFFFF);
			spin_unlock_irqrestore(&pGmacDev->lock, flag);

			mcRegLow += 2;
			mcRegHigh += 2;

		}
	}
	else {	// clear
		int i;	
		volatile u32 *mcRegHigh = &pGmacReg->macAddr_01_High;
		volatile u32 *mcRegLow = &pGmacReg->macAddr_01_Low;

		// clear mc list
#if defined(CONFIG_ARCH_SDP1106)
		for(i = 0; i < 14; i++) {
#else
		for(i = 0; i < 15; i++) {
#endif
			if(*mcRegHigh == 0) break;

			spin_lock_irqsave(&pGmacDev->lock, flag);
			*mcRegHigh = 0;
			*mcRegLow = 0;
			spin_unlock_irqrestore(&pGmacDev->lock, flag);

			mcRegLow += 2;
			mcRegHigh += 2;
		}

	}


	spin_lock_irqsave(&pGmacDev->lock, flag);
	pGmacReg->frameFilter = frameFilter;
	spin_unlock_irqrestore(&pGmacDev->lock, flag);

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
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);

	unsigned long flags;

	DPRINTK_GMAC_FLOW("called\n");

	if(!netif_running(pNetDev))
		return -EINVAL;

	spin_lock_irqsave(&pGmacDev->lock, flags);

	retVal = generic_mii_ioctl(&pGmacDev->mii, if_mii(pRq), cmd, NULL);

	spin_unlock_irqrestore(&pGmacDev->lock, flags);

	DPRINTK_GMAC_FLOW("exit\n");
	
	return retVal;
}

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
	unsigned long flags;

	DPRINTK_GMAC("called\n");

	pCmd->maxtxpkt = 1;	// ????
	pCmd->maxrxpkt = 1;	// ????

	spin_lock_irqsave(&pGmacDev->lock, flags);
	retVal = mii_ethtool_gset(&pGmacDev->mii, pCmd);
	spin_unlock_irqrestore(&pGmacDev->lock, flags);

	DPRINTK_GMAC("exit\n");
	return retVal;
}

static int 
sdpGmac_ethtool_setsettings (struct net_device *pNetDev, struct ethtool_cmd *pCmd)
{
	int retVal = SDP_GMAC_OK;

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);		
	unsigned long flags;

	DPRINTK_GMAC_FLOW("called\n");

	spin_lock_irqsave(&pGmacDev->lock, flags);
	retVal = mii_ethtool_sset(&pGmacDev->mii, pCmd);
	spin_unlock_irqrestore(&pGmacDev->lock, flags);

	DPRINTK_GMAC_FLOW("exit\n");
	return retVal;
}

static void 
sdpGmac_ethtool_getdrvinfo (struct net_device *pNetDev, struct ethtool_drvinfo *pDrvInfo)
{

	DPRINTK_GMAC("called\n");

        strncpy(pDrvInfo->driver, ETHER_NAME, sizeof(pDrvInfo->driver));
//      strncpy(info->version, version, sizeof(info->version));
        strncpy(pDrvInfo->bus_info, dev_name(pNetDev->dev.parent), sizeof(pDrvInfo->bus_info));

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
//	unsigned long flags;
	u16 phyVal;
	u8 phyId = pGmacDev->phyId;

	DPRINTK_GMAC("called\n");

//	spin_lock_irqsave(&pGmacDev->lock, flags);
	retVal = mii_nway_restart(&pGmacDev->mii);

	/* SDP GMAC need source clock */
// TODO phy Id check 
	phyVal = sdpGmac_mdioRead(pNetDev, phyId, MII_PHYADDR);
	phyVal = phyVal & ~(1 << 11);
	sdpGmac_mdioWrite(pNetDev, phyId, MII_PHYADDR, phyVal);	

//	spin_unlock_irqrestore(&pGmacDev->lock, flags);

	DPRINTK_GMAC("exit\n");
	return retVal;
}


/* number of registers GMAC + MII */
static int 
sdpGmac_ethtool_getregslen (struct net_device *pNetDev)
{
	int retVal = SDP_GMAC_OK;

	DPRINTK_GMAC("called\n");

	retVal = sizeof(SDP_GMAC_T);
	retVal += sizeof(SDP_GMAC_MMC_T);
	retVal += sizeof(SDP_GMAC_TIME_STAMP_T);
	retVal += sizeof(SDP_GMAC_MAC_2ND_BLOCK_T);
	retVal += sizeof(SDP_GMAC_DMA_T);
	retVal += (32 << 2);  // MII address

	DPRINTK_GMAC("exit\n");

	return retVal;
}

/* get all registers value GMAC + MII */
static void
sdpGmac_ethtool_getregs (struct net_device *pNetDev, struct ethtool_regs *pRegs, void *pBuf)
{

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
//	unsigned long flags;
	volatile u32 *pRegAddr;
	u32 *pData = (u32*) pBuf;
	u8 phyId = pGmacDev->phyId;
	u16 phyVal;
	int i, j = 0;

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
		phyVal = sdpGmac_mdioRead(pNetDev, phyId, i);
		pData[j++] = phyVal & 0xFFFF;
	}

	DPRINTK_GMAC_FLOW("exit\n");

	return;
}

// 0.92
static int sdpGmac_drv_suspend(struct platform_device *pDev, pm_message_t state)
{
	struct net_device *pNetDev = platform_get_drvdata(pDev);
	unsigned long flags;

	SDP_GMAC_DEV_T *pGmacDev ;
	SDP_GMAC_T *pGmacReg ;		
	SDP_GMAC_DMA_T *pDmaReg ;
	SDP_GMAC_POWER_T    *pPower ;

	if(pNetDev){
		pGmacDev = netdev_priv(pNetDev);
		pGmacReg = pGmacDev->pGmacBase;
		pDmaReg = pGmacDev->pDmaBase;
		pPower = &pGmacDev->power;

		spin_lock_irqsave(&pGmacDev->lock, flags);

		pPower->dma_busMode = pDmaReg->busMode;	
		pPower->dma_operationMode = pDmaReg->operationMode;	
		pPower->gmac_configuration = pGmacReg->configuration;
		pPower->gmac_frameFilter = pGmacReg->frameFilter;

		spin_unlock_irqrestore(&pGmacDev->lock, flags);

// backup mac address 
		memcpy((void *)pPower->gmac_macAddr, 
				(void *)&pGmacReg->macAddr_00_High,
				 	sizeof(pPower->gmac_macAddr));

		if(netif_running(pNetDev)){

			netif_device_detach(pNetDev);

			spin_lock_irqsave(&pGmacDev->lock, flags);

			pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN) ;
			pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);	

			// all interrupt disable
			pDmaReg->intrEnable = 0;	
			pDmaReg->status = pDmaReg->status;// Clear interrupt pending register 

			spin_unlock_irqrestore(&pGmacDev->lock, flags);

			SDP_DEL_TIMER(&pGmacDev->linkCheckTimer);
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

	int idx;

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

				rxDmaAddr = dma_map_single(pGmacDev->pDev, pSkb->data, skb_tailroom(pSkb), DMA_FROM_DEVICE);
		
//				CONVERT_DMA_QPTR(rxInitDesc, skb_tailroom(pSkb), rxDmaAddr, pSkb, 0, 0);
				CONVERT_DMA_QPTR(rxInitDesc, len, rxDmaAddr, pSkb, 0, 0);

				if(sdpGmac_setRxQptr(pGmacDev, &rxInitDesc, 0) < 0){
					dev_kfree_skb(pSkb);	
					break;
				}
			}while(1);

			// Init link status timer 
			init_timer(&pGmacDev->linkCheckTimer);
			add_timer(&pGmacDev->linkCheckTimer);

			//  interrupt enable all
			pDmaReg->intrEnable = B_INTR_ENABLE_ALL;

			// tx, rx enable
			netif_device_attach(pNetDev);
			pDmaReg->operationMode |= (B_TX_EN | B_RX_EN);	
			pGmacReg->configuration |= (B_TX_ENABLE | B_RX_ENABLE);
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

static int __devinit sdpGmac_netDev_initDesc(SDP_GMAC_DEV_T* pGmacDev, u32 descMode)
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
};

static int __devinit sdpGmac_probe(struct net_device *pNetDev)
{
	int retVal = 0;
//	const u8 defaultMacAddr[N_MAC_ADDR] = DEFAULT_MAC_ADDR;
	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	u32 len;

	
	DPRINTK_GMAC ("called\n");

	spin_lock_init(&pGmacDev->lock);	
	spin_lock_init(&pGmacDev->phy_lock);

	/* Device reset MAC addr is cleared*/	
	if(sdpGmac_reset(pNetDev) < 0){
		DPRINTK_GMAC_ERROR("reset Error\n");
		retVal = -ENODEV;		// 0.952
		goto  __probe_out_err;		// 0.952
	}

	ether_setup(pNetDev);

	pNetDev->ethtool_ops = &sdpGmac_ethtool_ops;
	pNetDev->netdev_ops = &sdpGmac_netdev_ops;
	pNetDev->watchdog_timeo = 5 * HZ;
#if 0
	pNetDev->hard_start_xmit = sdpGmac_netDev_hardStartXmit;
	pNetDev->get_stats = sdpGmac_netDev_getStats;
	pNetDev->open = sdpGmac_netDev_open;
	pNetDev->stop = sdpGmac_netDev_close;
	pNetDev->set_multicast_list = sdpGmac_netDev_setMulticastList;
	pNetDev->set_mac_address = sdpGmac_netDev_setMacAddr;
	pNetDev->tx_timeout = sdpGmac_netDev_txTimeOut; // TODO
	pNetDev->change_mtu = sdpGmac_netDev_chgMtu;	// TODO
	pNetDev->do_ioctl = sdpGmac_netDev_ioctl;
#endif

// Phy configuration
	INIT_WORK(&pGmacDev->phyConf, sdpGmac_phyConf);
	pGmacDev->mii.phy_id_mask = 0x1F;
	pGmacDev->mii.reg_num_mask = 0x1F;
	pGmacDev->mii.force_media = 0;  // ???
	pGmacDev->mii.full_duplex = 0; 	// ???
	pGmacDev->mii.dev = pNetDev;
	pGmacDev->mii.mdio_read = sdpGmac_mdioRead;
	pGmacDev->mii.mdio_write = sdpGmac_mdioWrite;
	pGmacDev->mii.phy_id = pGmacDev->phyId;	 	// phy address 

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

	if(register_netdev(pNetDev) < 0) {
		DPRINTK_GMAC_ERROR ("register netdev error\n");
		retVal = -EPERM;
		goto __probe_out_err4;
	}
	
#if defined(CONFIG_MAC_GET_I2C)
	sdpGmac_getMacFromI2c(pNetDev);		// get mac address 
#elif defined(CONFIG_MAC_SET_BY_USER)
	sdpGmac_setMacByUser(pNetDev);
#endif 

	if(!is_valid_ether_addr(pNetDev->dev_addr)){
		PRINTK_GMAC("%s: Invalid ethernet MAC address. Please "
				"set using ifconfig\n", pNetDev->name);
	}
	else {
		PRINTK_GMAC ("%s: Ethernet address %02x:%02x:%02x:%02x:%02x:%02x\n",
		     pNetDev->name, *pNetDev->dev_addr, *(pNetDev->dev_addr+1),
		     *(pNetDev->dev_addr+2), *(pNetDev->dev_addr+3),
		     *(pNetDev->dev_addr+4), *(pNetDev->dev_addr+5));	

		sdpGmac_setMacAddr(pNetDev, (const u8*)pNetDev->dev_addr);
	}

	PRINTK_GMAC ("%s: IRQ is %d\n", pNetDev->name, pNetDev->irq);
	
	if(pGmacDev->phyType == 0) {
		DPRINTK_GMAC_ERROR("can't find PHY. Please check b'd\n");
		retVal = -ENODEV;
		goto __probe_out_err5;
	}

	DPRINTK_GMAC ("exit\n");
	return 0;

__probe_out_err5:				// 0.952
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

	printk(KERN_INFO"sdp GMAC 10/100M network driver ver %s\n", GMAC_DRV_VERSION);
// net device 
	pNetDev = alloc_etherdev(sizeof(SDP_GMAC_DEV_T));

        if (!pNetDev) {
                DPRINTK_GMAC_ERROR("%s: could not allocate device.\n", ETHER_NAME);
                retVal = -ENOMEM;
                goto __probe_drv_out;
        }
	SET_NETDEV_DEV(pNetDev, &pDev->dev);

	pNetDev->dma = (unsigned char) -1;
	pGmacDev = netdev_priv(pNetDev);
#ifdef CONFIG_ARCH_SDP1004
	pGmacDev->revision = sdp_get_revision_id();
	printk(KERN_INFO"[SDP MAC] Firenze revision %d\n", pGmacDev->revision);
#endif
	pGmacDev->pNetDev = pNetDev;
// need to request dma memory 
	pGmacDev->pDev = &pDev->dev;		
	
	pGmacDev->pGmacBase = NULL;		// 0.952
	pGmacDev->pMmcBase = NULL;		// 0.952
	pGmacDev->pTimeStampBase = NULL;	// 0.952
	pGmacDev->pMac2ndBlk = NULL;		// 0.952
	pGmacDev->pDmaBase = NULL;		// 0.952

// GMAC resource initialize 
	for (i = 0; i < N_REG_BASE; i++) {
		pRes = platform_get_resource(pDev, IORESOURCE_MEM, i);
		if(!pRes){	// 0.952
                	DPRINTK_GMAC_ERROR("%s: could not find device %d resource.\n", 
						ETHER_NAME, i);
			retVal = -ENODEV;
			goto __probe_drv_err;
		}
		sdpGmac_setMemBase(i, pRes, pGmacDev);
	}

	pNetDev->irq = platform_get_irq(pDev, 0);
	DPRINTK_GMAC_FLOW("IRQ is %d\n", pNetDev->irq);

// save resource about net driver 
	platform_set_drvdata(pDev, pNetDev);

// DMA mask
	pDev->dev.coherent_dma_mask = 0xFFFFFFFFUL;
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

	// timer control
	SDP_DEL_TIMER(&pGmacDev->linkCheckTimer);


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


#if 0
static void sdpGmac_recover(struct net_device *pNetDev) // 0.958
{
	unsigned long flags;

	SDP_GMAC_DEV_T *pGmacDev = netdev_priv(pNetDev);
	SDP_GMAC_T *pGmacReg = pGmacDev->pGmacBase;		
	SDP_GMAC_MMC_T *pMmcReg = pGmacDev->pMmcBase;		
	SDP_GMAC_DMA_T *pDmaReg = pGmacDev->pDmaBase;
	SDP_GMAC_POWER_T *pPower = &pGmacDev->power;
	u32	interrupt;
	int idx, count = 0;
	DMA_DESC_T *p_tx_desc = pGmacDev->pTxDesc;

	spin_lock_irqsave(&pGmacDev->lock, flags);

	pPower->dma_busMode = pDmaReg->busMode;	
	pPower->dma_operationMode = pDmaReg->operationMode;	
	pPower->gmac_configuration = pGmacReg->configuration;
	pPower->gmac_frameFilter = pGmacReg->frameFilter;

	spin_unlock_irqrestore(&pGmacDev->lock, flags);

// backup mac address 
	memcpy((void *)pPower->gmac_macAddr, 
				(void *)&pGmacReg->macAddr_00_High,
				 	sizeof(pPower->gmac_macAddr));

	spin_lock_irqsave(&pGmacDev->lock, flags);

	interrupt = pDmaReg->intrEnable;
	pDmaReg->operationMode &= ~(B_TX_EN | B_RX_EN) ;
	pGmacReg->configuration &= ~(B_TX_ENABLE | B_RX_ENABLE);	
	
	// all interrupt disable
	pDmaReg->intrEnable = 0;	
	pDmaReg->status = pDmaReg->status;// Clear interrupt pending register 

	spin_unlock_irqrestore(&pGmacDev->lock, flags);

	sdpGmac_netDev_rx(pNetDev);		// receive buffer
	sdpGmac_netDev_tx(pNetDev);		// transmission buffer

	printk("[DEBUG_MAC] current tx descriptor addr 0x%08x\n", pDmaReg->curHostTxDesc);

	count = 0;

	for(idx = 0; idx < TX_DESC_SIZE/sizeof(DMA_DESC_T); idx++){
		if(p_tx_desc->status & ~(DESC_TX_END_RING)){
			printk("[DEBUG_MAC] not clear tx descriptor addr 0x%08x\n", pGmacDev->txDescDma + (idx * sizeof(DMA_DESC_T)));
			printk("[DEBUG_MAC] not clear tx descriptor status 0x%08x\n", p_tx_desc->status);
			printk("[DEBUG_MAC] not clear tx descriptor length 0x%08x\n", p_tx_desc->length);
			if(p_tx_desc->status &  DESC_OWN_BY_DMA){
				p_tx_desc->status &= ~(DESC_TX_END_RING);
				*(pGmacDev->pTxDesc + count) = *p_tx_desc;
				count++;	
			}
			else {
				dma_unmap_single(pGmacDev->pDev, p_tx_desc->buffer1, p_tx_desc->length, DMA_FROM_DEVICE);
				dev_kfree_skb_irq((struct sk_buff*)p_tx_desc->data1);		
			}

			p_tx_desc->status &= DESC_TX_END_RING;
			p_tx_desc->length = 0;
			p_tx_desc->buffer1 = 0;
			p_tx_desc->buffer2 = 0;
			p_tx_desc->data1 = 0;
			p_tx_desc->data2 = 0;
		}
		p_tx_desc++;
	}
	p_tx_desc = pGmacDev->pTxDesc + (TX_DESC_SIZE/sizeof(DMA_DESC_T)) - 1;
	p_tx_desc->status |= DESC_TX_END_RING;

	pDmaReg->busMode = B_SW_RESET;	
	udelay(5);
	while(pDmaReg->busMode & B_SW_RESET);

	spin_lock_irqsave(&pGmacDev->lock, flags);

	pGmacDev->pTxDescBusy = pGmacDev->pTxDesc;
	pGmacDev->pTxDescNext = pGmacDev->pTxDesc + count;
	pGmacDev->pRxDescBusy = pGmacDev->pRxDesc;
	pGmacDev->pRxDescNext = pGmacDev->pRxDesc;

	pMmcReg->intrMaskRx = 0x00FFFFFF;  // disable
	pMmcReg->intrMaskTx = 0x01FFFFFF;  // disable 
	pMmcReg->mmcIpcIntrMaskRx = 0x3FFF3FFF;  // disable 
	pGmacReg->intrMask = 0x20F;

	pDmaReg->busMode = pPower->dma_busMode;	

	pDmaReg->txDescListAddr = pGmacDev->txDescDma;	
	pDmaReg->rxDescListAddr = pGmacDev->rxDescDma;	

	memcpy((void *)&pGmacReg->macAddr_00_High,
				(void *)pPower->gmac_macAddr, 
				 	sizeof(pPower->gmac_macAddr));

	pDmaReg->operationMode = pPower->dma_operationMode;	
	pGmacReg->configuration = pPower->gmac_configuration;
	pGmacReg->frameFilter = pPower->gmac_frameFilter;
	pDmaReg->intrEnable = interrupt;

	spin_unlock_irqrestore(&pGmacDev->lock, flags);

}
#endif




