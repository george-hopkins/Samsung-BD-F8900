/** \file
 * This is the network dependent layer to handle network related functionality.
 * This file is tightly coupled to neworking frame work of linux 2.6.xx kernel.
 * The functionality carried out in this file should be treated as an example only
 * if the underlying operating system is not Linux. 
 * 
 * \note Many of the functions other than the device specific functions
 *  changes for operating system other than Linux 2.6.xx
 * \internal 
 *-----------------------------REVISION HISTORY-----------------------------------
 * Synopsys			01/Aug/2007				Created
 */


#if 0
//#include <linux/config.h>
#include <linux/version.h>   /* LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 33))
#include <generated/autoconf.h>
#else
#include <linux/autoconf.h>
#endif
#endif
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include <asm/io.h>
#include <asm/wbflush.h>

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/sockios.h>		// for socket ioctl lists
#include <linux/mii.h>

#include <linux/spinlock.h>
#include <asm/ntclk.h>
#include <nvt-clk.h>
#include <nvt-sys.h>

#include "synopGMAC_Host.h"
#include "synopGMAC_plat.h"
#include "synopGMAC_network_interface.h"
#include "synopGMAC_Dev.h"

#define IOCTL_READ_REGISTER  SIOCDEVPRIVATE+1
#define IOCTL_WRITE_REGISTER SIOCDEVPRIVATE+2
#define IOCTL_READ_IPSTRUCT  SIOCDEVPRIVATE+3
#define IOCTL_READ_RXDESC    SIOCDEVPRIVATE+4
#define IOCTL_READ_TXDESC    SIOCDEVPRIVATE+5
#define IOCTL_POWER_DOWN     SIOCDEVPRIVATE+6
#define IOCTL_CHK_CONN	     SIOCDEVPRIVATE+7
#define IOCTL_PHY_LOOPBACK    SIOCDEVPRIVATE+8

static void synopGMAC_giveup_tx_desc_queue(synopGMACdevice * gmacdev,struct pci_dev * pcidev, u32 desc_mode);
static irqreturn_t synopGMAC_intr_handler(int irq, void *dev_id);
static void synop_handle_transmit_over(struct net_device *netdev);
static void synop_handle_received_data(struct net_device *netdev);
static void synopGMAC_giveup_rx_desc_queue(synopGMACdevice * gmacdev, struct pci_dev *pcidev, u32 desc_mode);
static s32 synopGMAC_setup_rx_desc_queue(synopGMACdevice * gmacdev,struct pci_dev * pcidev,u32 no_of_desc, u32 desc_mode);
static s32 synopGMAC_setup_tx_desc_queue(synopGMACdevice * gmacdev,struct pci_dev * pcidev,u32 no_of_desc, u32 desc_mode);

static struct timer_list synopGMAC_cable_unplug_timer;
static u32 GMAC_Power_down; // This global variable is used to indicate the ISR whether the interrupts occured in the process of powering down the mac or not

/* Get HCLK */
extern unsigned long hclk;
extern raw_spinlock_t clock_gen_lock;

/*These are the global pointers for their respecive structures*/
extern synopGMACPciNetworkAdapter * synopGMACadapter;
extern synopGMACdevice	          * synopGMACdev;
extern struct net_dev             * synopGMACnetdev;
//extern struct pci_dev             * synopGMACpcidev;		// jay hsu

//static char Rx_Int_Flag = 0;	// jay hsu : 
static u8 Mac_Addr[6];		// jay hsu : save the current mac address

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
	#define NVT_NAPI_WEIGHT 64
	// jay hsu : 1 -> napi poll running, otherwise stop
	atomic_t napi_poll = ATOMIC_INIT(0);
	static struct napi_struct nvt_napi;
#endif

/*these are the global data for base address and its size*/
extern u8* synopGMACMappedAddr;
extern u32 synopGMACMappedAddrSize;
//extern u32 synop_pci_using_dac;		// jay hsu

/*Sample Wake-up frame filter configurations*/

u32 synopGMAC_wakeup_filter_config0[] = {
					0x00000000,	// For Filter0 CRC is not computed may be it is 0x0000
					0x00000000,	// For Filter1 CRC is not computed may be it is 0x0000
					0x00000000,	// For Filter2 CRC is not computed may be it is 0x0000
					0x5F5F5F5F,     // For Filter3 CRC is based on 0,1,2,3,4,6,8,9,10,11,12,14,16,17,18,19,20,22,24,25,26,27,28,30 bytes from offset
					0x09000000,     // Filter 0,1,2 are disabled, Filter3 is enabled and filtering applies to only multicast packets
					0x1C000000,     // Filter 0,1,2 (no significance), filter 3 offset is 28 bytes from start of Destination MAC address 
					0x00000000,     // No significance of CRC for Filter0 and Filter1
					0xBDCC0000      // No significance of CRC for Filter2, Filter3 CRC is 0xBDCC
					};
u32 synopGMAC_wakeup_filter_config1[] = {
					0x00000000,	// For Filter0 CRC is not computed may be it is 0x0000
					0x00000000,	// For Filter1 CRC is not computed may be it is 0x0000
					0x7A7A7A7A,	// For Filter2 CRC is based on 1,3,4,5,6,9,11,12,13,14,17,19,20,21,25,27,28,29,30 bytes from offset
					0x00000000,     // For Filter3 CRC is not computed may be it is 0x0000
					0x00010000,     // Filter 0,1,3 are disabled, Filter2 is enabled and filtering applies to only unicast packets
					0x00100000,     // Filter 0,1,3 (no significance), filter 2 offset is 16 bytes from start of Destination MAC address 
					0x00000000,     // No significance of CRC for Filter0 and Filter1
					0x0000A0FE      // No significance of CRC for Filter3, Filter2 CRC is 0xA0FE
					};
u32 synopGMAC_wakeup_filter_config2[] = {
					0x00000000,	// For Filter0 CRC is not computed may be it is 0x0000
					0x000000FF,	// For Filter1 CRC is computed on 0,1,2,3,4,5,6,7 bytes from offset
					0x00000000,	// For Filter2 CRC is not computed may be it is 0x0000
					0x00000000,     // For Filter3 CRC is not computed may be it is 0x0000
					0x00000100,     // Filter 0,2,3 are disabled, Filter 1 is enabled and filtering applies to only unicast packets
					0x0000DF00,     // Filter 0,2,3 (no significance), filter 1 offset is 223 bytes from start of Destination MAC address 
					0xDB9E0000,     // No significance of CRC for Filter0, Filter1 CRC is 0xDB9E
					0x00000000      // No significance of CRC for Filter2 and Filter3 
					};

/*
The synopGMAC_wakeup_filter_config3[] is a sample configuration for wake up filter. 
Filter1 is used here
Filter1 offset is programmed to 50 (0x32)
Filter1 mask is set to 0x000000FF, indicating First 8 bytes are used by the filter
Filter1 CRC= 0x7EED this is the CRC computed on data 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55

Refer accompanied software DWC_gmac_crc_example.c for CRC16 generation and how to use the same.
*/

u32 synopGMAC_wakeup_filter_config3[] = {
					0x00000000,	// For Filter0 CRC is not computed may be it is 0x0000
					0x000000FF,	// For Filter1 CRC is computed on 0,1,2,3,4,5,6,7 bytes from offset
					0x00000000,	// For Filter2 CRC is not computed may be it is 0x0000
					0x00000000,     // For Filter3 CRC is not computed may be it is 0x0000
					0x00000100,     // Filter 0,2,3 are disabled, Filter 1 is enabled and filtering applies to only unicast packets
					0x00003200,     // Filter 0,2,3 (no significance), filter 1 offset is 50 bytes from start of Destination MAC address 
					0x7eED0000,     // No significance of CRC for Filter0, Filter1 CRC is 0x7EED, 
					0x00000000      // No significance of CRC for Filter2 and Filter3 
					};
/**
 * Function used to detect the cable plugging and unplugging.
 * This function gets scheduled once in every second and polls
 * the PHY register for network cable plug/unplug. Once the 
 * connection is back the GMAC device is configured as per
 * new Duplex mode and Speed of the connection.
 * @param[in] u32 type but is not used currently. 
 * \return returns void.
 * \note This function is tightly coupled with Linux 2.6.xx.
 * \callgraph
 */

static void synopGMAC_linux_cable_unplug_function(u32 notused)
{
s32 status;
u16 data;
synopGMACPciNetworkAdapter *adapter = (synopGMACPciNetworkAdapter *)synopGMAC_cable_unplug_timer.data;
synopGMACdevice            *gmacdev = adapter->synopGMACdev;

init_timer(&synopGMAC_cable_unplug_timer);
synopGMAC_cable_unplug_timer.expires = CHECK_TIME + jiffies;
	
	//synopGMAC_resume_dma_tx(gmacdev);	// jay hsu : debug to flush tx frames
	
	//status = synopGMAC_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_SPECIFIC_STATUS_REG, &data);
	// jay hsu : check our phy status
	status = synopGMAC_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_STATUS_REG, &data);
	
//if((data & Mii_phy_status_link_up) == 0){
	// jay hsu
    if((status != -ESYNOPGMACNOERR) || ((data & Mii_Link) == 0))
{   
	if ( gmacdev->LinkState == 1 ) 
	{
		TR("No Link: %08x\n",data);
	}
  	gmacdev->LinkState = 0;
	gmacdev->DuplexMode = 0;
	gmacdev->Speed = 0;
	gmacdev->LoopBackMode = 0;
	netif_carrier_off(synopGMACadapter->synopGMACnetdev);		// Jay Hsu @ 20110713 : cable unplug

	add_timer(&synopGMAC_cable_unplug_timer);
}
else{
	if ( gmacdev->LinkState == 0 ) 
	{
		TR("Link UP: %08x\n",data);
	}
		
	if(!gmacdev->LinkState){
		//status = synopGMAC_check_phy_init(gmacdev);
		
		// jay hsu : check phy init speed for our Micrel phy
		synopGMAC_check_phy_init_for_10_100M(gmacdev);
		synopGMAC_mac_init(gmacdev);
		netif_carrier_on(synopGMACadapter->synopGMACnetdev);		// Jay Hsu @ 20110713 : cable plug

	}
	add_timer(&synopGMAC_cable_unplug_timer);
}
	
}

static void synopGMAC_linux_powerdown_mac(synopGMACdevice *gmacdev)
{
	TR0("Put the GMAC to power down mode..\n");
	// Disable the Dma engines in tx path
	GMAC_Power_down = 1;	// Let ISR know that Mac is going to be in the power down mode
	synopGMAC_disable_dma_tx(gmacdev);
	plat_delay(10000);		//allow any pending transmission to complete
	// Disable the Mac for both tx and rx
	synopGMAC_tx_disable(gmacdev);
	synopGMAC_rx_disable(gmacdev);
        plat_delay(10000); 		//Allow any pending buffer to be read by host
	//Disable the Dma in rx path
        synopGMAC_disable_dma_rx(gmacdev);

	//enable the power down mode
	//synopGMAC_pmt_unicast_enable(gmacdev);
	
	//prepare the gmac for magic packet reception and wake up frame reception
	synopGMAC_magic_packet_enable(gmacdev);
	synopGMAC_write_wakeup_frame_register(gmacdev, synopGMAC_wakeup_filter_config3);

	synopGMAC_wakeup_frame_enable(gmacdev);

	//gate the application and transmit clock inputs to the code. This is not done in this driver :).

	//enable the Mac for reception
	synopGMAC_rx_enable(gmacdev);

	//Enable the assertion of PMT interrupt
	synopGMAC_pmt_int_enable(gmacdev);
	//enter the power down mode
	synopGMAC_power_down_enable(gmacdev);
	return;
}

static void synopGMAC_linux_powerup_mac(synopGMACdevice *gmacdev)
{
	GMAC_Power_down = 0;	// Let ISR know that MAC is out of power down now
	if( synopGMAC_is_magic_packet_received(gmacdev))
	{
		TR("GMAC wokeup due to Magic Pkt Received\n");
	}
	if(synopGMAC_is_wakeup_frame_received(gmacdev))
	{
		TR("GMAC wokeup due to Wakeup Frame Received\n");
	}
	//Disable the assertion of PMT interrupt
	synopGMAC_pmt_int_disable(gmacdev);
	//Enable the mac and Dma rx and tx paths
	synopGMAC_rx_enable(gmacdev);
       	synopGMAC_enable_dma_rx(gmacdev);

	synopGMAC_tx_enable(gmacdev);
	synopGMAC_enable_dma_tx(gmacdev);
	return;
}
/**
  * This sets up the transmit Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Device is interested only after the descriptors are setup. Therefore this function
  * is not included in the device driver API. This function should be treated as an
  * example code to design the descriptor structures for ring mode or chain mode.
  * This function depends on the pcidev structure for allocation consistent dma-able memory in case of linux.
  * This limitation is due to the fact that linux uses pci structure to allocate a dmable memory
  *	- Allocates the memory for the descriptors.
  *	- Initialize the Busy and Next descriptors indices to 0(Indicating first descriptor).
  *	- Initialize the Busy and Next descriptors to first descriptor address.
  * 	- Initialize the last descriptor with the endof ring in case of ring mode.
  *	- Initialize the descriptors in chain mode.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in tx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note This function fails if allocation fails for required number of descriptors in Ring mode, but in chain mode
  *  function returns -ESYNOPGMACNOMEM in the process of descriptor chain creation. once returned from this function
  *  user should for gmacdev->TxDescCount to see how many descriptors are there in the chain. Should continue further
  *  only if the number of descriptors in the chain meets the requirements  
  */

s32 synopGMAC_setup_tx_desc_queue(synopGMACdevice * gmacdev,struct pci_dev * pcidev,u32 no_of_desc, u32 desc_mode)
{
u32 i;

DmaDesc *first_desc = NULL;
DmaDesc *second_desc = NULL;
dma_addr_t dma_addr;
gmacdev->TxDescCount = 0;

if(desc_mode == RINGMODE){
	TR("Total size of memory required for Tx Descriptors in Ring Mode = 0x%08x\n",((sizeof(DmaDesc) * no_of_desc)));
	//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc) * no_of_desc,&dma_addr);	// jay hsu
	
	// jay hsu : alloc memory in kernel
	first_desc = sys_plat_alloc_memory(sizeof(DmaDesc) * no_of_desc);
	dma_addr = (dma_addr_t)first_desc;		// jay hsu : use cache virtual address instead of dma address
	
	// jay hsu : use dma-able address
	//first_desc = dma_alloc_coherent(NULL, sizeof(DmaDesc) * no_of_desc,&dma_addr, GFP_KERNEL|GFP_DMA);	
	//printk("jay debug : alloc tx dma_addr 0x%08x\n", (u32)dma_addr);
	//printk("jay debug : alloc tx first_desc 0x%08x\n", (u32)first_desc);	
	
	// jay debug : use cache
	//first_desc = (u32 *)((u32)first_desc-0x20000000);
	//printk("jay debug : alloc tx first desc 0x%08x\n", (u32)first_desc);
	
	// jay debug : use non-cache
	//first_desc = sys_plat_alloc_memory(sizeof(DmaDesc) * no_of_desc)+0x20000000;
	
	if(first_desc == NULL){
		TR("Error in Tx Descriptors memory allocation\n");
		return -ESYNOPGMACNOMEM;
	}
	// jay hsu : clear tx descriptors
	memset((char *)first_desc, 0, sizeof(DmaDesc) * no_of_desc);
	gmacdev->TxDescCount = no_of_desc;
	gmacdev->TxDesc      = first_desc;
	//gmacdev->TxDescDma   = (u32)first_desc;	// jay hsu
	gmacdev->TxDescDma   = virt_to_phys((u32 *)dma_addr);		// jay hsu : save the dma address
	
	for(i =0; i < gmacdev -> TxDescCount; i++){
		synopGMAC_tx_desc_init_ring(gmacdev->TxDesc + i, i == gmacdev->TxDescCount-1);
		TR("%02d %08x \n",i, (unsigned int)(gmacdev->TxDesc + i) );
	}
	
	// jay hsu : sync tx init descriptors to dram
	dma_cache_sync(NULL, first_desc,(sizeof(DmaDesc)*no_of_desc), DMA_TO_DEVICE);		// jay debug : use non-cache

}
else{
//Allocate the head descriptor
	//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
	
	// jay hsu : alloc memory in kernel
	first_desc = sys_plat_alloc_memory(sizeof(DmaDesc));
	dma_addr = (dma_addr_t)first_desc;		// jay hsu : use cache virtual address instead of dma address
	
	// jay hsu : use dma-able address
	//first_desc = dma_alloc_coherent(NULL, sizeof(DmaDesc),&dma_addr, GFP_KERNEL|GFP_DMA);	
	
	if(first_desc == NULL){
		TR("Error in Tx Descriptor Memory allocation in Ring mode\n");
		return -ESYNOPGMACNOMEM;
	}
	gmacdev->TxDesc       = first_desc;
	//gmacdev->TxDescDma    = (u32)first_desc;	// jay hsu
	gmacdev->TxDescDma    = virt_to_phys((u32 *)dma_addr);		// jay hsu : save the dma address

	TR("Tx===================================================================Tx\n");
	first_desc->buffer2   = gmacdev->TxDescDma;
 	first_desc->data2     = (u32)gmacdev->TxDesc;

	gmacdev->TxDescCount = 1;
	
	for(i =0; i <(no_of_desc-1); i++){
		//second_desc = plat_alloc_consistent_dmaable_memory(pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
		
		// jay hsu : alloc memory in kernel
		second_desc = sys_plat_alloc_memory(sizeof(DmaDesc));
		
		// jay hsu : use dma-able address
		//second_desc = dma_alloc_coherent(NULL, sizeof(DmaDesc),&dma_addr, GFP_KERNEL|GFP_DMA);	
		
		if(second_desc == NULL){	
			TR("Error in Tx Descriptor Memory allocation in Chain mode\n");
			return -ESYNOPGMACNOMEM;
		}
		first_desc->buffer2  = virt_to_phys((u32 *)dma_addr);
		first_desc->data2    = (u32)second_desc;
		
		second_desc->buffer2 = gmacdev->TxDescDma;
		second_desc->data2   = (u32)gmacdev->TxDesc;

	        synopGMAC_tx_desc_init_chain(first_desc);
		TR("%02d %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->TxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
		gmacdev->TxDescCount += 1;
		first_desc = second_desc;
	}
		
		synopGMAC_tx_desc_init_chain(first_desc);
		TR("%02d %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->TxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
	TR("Tx===================================================================Tx\n");
}

	gmacdev->TxNext = 0;
	gmacdev->TxBusy = 0;
	gmacdev->TxNextDesc = gmacdev->TxDesc;
	gmacdev->TxBusyDesc = gmacdev->TxDesc;
	gmacdev->BusyTxDesc  = 0; 

	return -ESYNOPGMACNOERR;
}


/**
  * This sets up the receive Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Device is interested only after the descriptors are setup. Therefore this function
  * is not included in the device driver API. This function should be treated as an
  * example code to design the descriptor structures in ring mode or chain mode.
  * This function depends on the pcidev structure for allocation of consistent dma-able memory in case of linux.
  * This limitation is due to the fact that linux uses pci structure to allocate a dmable memory
  *	- Allocates the memory for the descriptors.
  *	- Initialize the Busy and Next descriptors indices to 0(Indicating first descriptor).
  *	- Initialize the Busy and Next descriptors to first descriptor address.
  * 	- Initialize the last descriptor with the endof ring in case of ring mode.
  *	- Initialize the descriptors in chain mode.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in rx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note This function fails if allocation fails for required number of descriptors in Ring mode, but in chain mode
  *  function returns -ESYNOPGMACNOMEM in the process of descriptor chain creation. once returned from this function
  *  user should for gmacdev->RxDescCount to see how many descriptors are there in the chain. Should continue further
  *  only if the number of descriptors in the chain meets the requirements  
  */
s32 synopGMAC_setup_rx_desc_queue(synopGMACdevice * gmacdev,struct pci_dev * pcidev,u32 no_of_desc, u32 desc_mode)
{
u32 i;

DmaDesc *first_desc = NULL;
DmaDesc *second_desc = NULL;
dma_addr_t dma_addr;
gmacdev->RxDescCount = 0;

if(desc_mode == RINGMODE){
	TR("total size of memory required for Rx Descriptors in Ring Mode = 0x%08x\n",((sizeof(DmaDesc) * no_of_desc)));
	//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc) * no_of_desc, &dma_addr);	// jay hsu
	
	// jay hsu : alloc memory in kernel
	first_desc = sys_plat_alloc_memory(sizeof(DmaDesc) * no_of_desc);
	
	if(first_desc == NULL){
		TR("Error in Rx Descriptor Memory allocation in Ring mode\n");
		return -ESYNOPGMACNOMEM;
	}
	
	dma_addr = (dma_addr_t)first_desc;		// jay hsu : use cache virtual address instead of dma address
		
	memset((char *)first_desc, 0, sizeof(DmaDesc) * no_of_desc);
	
	// jay hsu : allocate the dma address
	//first_desc = dma_alloc_coherent(NULL, sizeof(DmaDesc) * no_of_desc, &dma_addr, GFP_KERNEL|GFP_DMA);	
	//printk("jay debug : alloc rx dma_addr 0x%08x\n", (u32)dma_addr);
	//printk("jay debug : alloc rx first desc 0x%08x\n", (u32)first_desc);
	
	// jay debug : use cache
	//first_desc = (u32 *)((u32)first_desc-0x20000000);
	//printk("jay debug : alloc rx first desc 0x%08x\n", (u32)first_desc);
	
	// jay debug : use non-cache
	//first_desc = sys_plat_alloc_memory(sizeof(DmaDesc) * no_of_desc)+0x20000000;
	
	// jay hsu : clear rx descriptors
	
/*	## change to uncache address to test ##	
	//first_desc = sys_plat_alloc_memory(sizeof(DmaDesc) * no_of_desc) + 0x20000000;	// jay hsu : use uncache address for debug
	//printk("jay debug : Use uncache address for rx 0x%08x\n", (u32)first_desc);	// jay hsu : debug
*/	
	gmacdev->RxDescCount = no_of_desc;
	gmacdev->RxDesc      = first_desc;
	//gmacdev->RxDescDma   = (u32)first_desc;	// jay hsu
	gmacdev->RxDescDma   = virt_to_phys((u32 *)dma_addr);			// jay hsu : save the dma address
	
	for(i =0; i < gmacdev -> RxDescCount; i++){
		synopGMAC_rx_desc_init_ring(gmacdev->RxDesc + i, i == gmacdev->RxDescCount-1);
		TR("%02d %08x \n",i, (unsigned int)(gmacdev->RxDesc + i));

	}
	
	// jay hsu : sync rx init descriptors to dram
	dma_cache_sync(NULL, first_desc,(sizeof(DmaDesc)*no_of_desc), DMA_TO_DEVICE);		// jay debug : use non-cache

}
else{
//Allocate the head descriptor
	//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
	
	// jay hsu : alloc memory in kernel
	first_desc = sys_plat_alloc_memory(sizeof(DmaDesc));	
	dma_addr = (dma_addr_t)first_desc;		// jay hsu : use cache virtual address instead of dma address
	
	//first_desc = dma_alloc_coherent(NULL, sizeof(DmaDesc), &dma_addr, GFP_KERNEL|GFP_DMA);	
	
	if(first_desc == NULL){
		TR("Error in Rx Descriptor Memory allocation in Ring mode\n");
		return -ESYNOPGMACNOMEM;
	}
	gmacdev->RxDesc       = first_desc;
	//gmacdev->RxDescDma    = (u32)first_desc;	// jay hsu
	gmacdev->RxDescDma    = virt_to_phys((u32 *)dma_addr);		// jay hsu : save the dma address

	TR("Rx===================================================================Rx\n");
	first_desc->buffer2   = gmacdev->RxDescDma;
	first_desc->data2     = (u32) gmacdev->RxDesc;

	gmacdev->RxDescCount = 1;
	
	for(i =0; i < (no_of_desc-1); i++){
		//second_desc = plat_alloc_consistent_dmaable_memory(pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
		
		// jay hsu : alloc memory in kernel
		second_desc = sys_plat_alloc_memory(sizeof(DmaDesc));
		
		// jay hsu : use dma-able address
		//second_desc = dma_alloc_coherent(NULL, sizeof(DmaDesc), &dma_addr, GFP_KERNEL|GFP_DMA);	
		
		if(second_desc == NULL){	
			TR("Error in Rx Descriptor Memory allocation in Chain mode\n");
			return -ESYNOPGMACNOMEM;
		}
		first_desc->buffer2  = virt_to_phys((u32 *)dma_addr);
		first_desc->data2    = (u32)second_desc;
		
		second_desc->buffer2 = gmacdev->RxDescDma;
		second_desc->data2   = (u32)gmacdev->RxDesc;

		synopGMAC_rx_desc_init_chain(first_desc);
		TR("%02d  %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->RxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
		gmacdev->RxDescCount += 1;
		first_desc = second_desc;
	}
                synopGMAC_rx_desc_init_chain(first_desc);
		TR("%02d  %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->RxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
	TR("Rx===================================================================Rx\n");

}

	gmacdev->RxNext = 0;
	gmacdev->RxBusy = 0;
	gmacdev->RxNextDesc = gmacdev->RxDesc;
	gmacdev->RxBusyDesc = gmacdev->RxDesc;

	gmacdev->BusyRxDesc   = 0; 

	return -ESYNOPGMACNOERR;
}

/**
  * This gives up the receive Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Once device's Dma is stopped the memory descriptor memory and the buffer memory deallocation,
  * is completely handled by the operating system, this call is kept outside the device driver Api.
  * This function should be treated as an example code to de-allocate the descriptor structures in ring mode or chain mode
  * and network buffer deallocation.
  * This function depends on the pcidev structure for dma-able memory deallocation for both descriptor memory and the
  * network buffer memory under linux.
  * The responsibility of this function is to 
  *     - Free the network buffer memory if any.
  *	- Fee the memory allocated for the descriptors.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in rx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note No referece should be made to descriptors once this function is called. This function is invoked when the device is closed.
  */
void synopGMAC_giveup_rx_desc_queue(synopGMACdevice * gmacdev, struct pci_dev *pcidev, u32 desc_mode)
{
u32 i;

DmaDesc *first_desc = NULL;
//dma_addr_t first_desc_dma_addr;
u32 status;
dma_addr_t dma_addr1;
dma_addr_t dma_addr2;
u32 length1;
u32 length2;
u32 data1;
u32 data2;

if(desc_mode == RINGMODE){
	for(i =0; i < gmacdev -> RxDescCount; i++){
		synopGMAC_get_desc_data(gmacdev->RxDesc + i, &status, &dma_addr1, &length1, &data1, &dma_addr2, &length2, &data2);
		if((length1 != 0) && (data1 != 0)){
			//pci_unmap_single(pcidev,dma_addr1,0,PCI_DMA_FROMDEVICE);		// jay hsu
			dev_kfree_skb_any((struct sk_buff *) data1);	// free buffer1
			TR("(Ring mode) rx buffer1 %08x of size %d from %d rx descriptor is given back\n",data1, length1, i);
		}
		if((length2 != 0) && (data2 != 0)){
			//pci_unmap_single(pcidev,dma_addr2,0,PCI_DMA_FROMDEVICE);		// jay hsu
			dev_kfree_skb_any((struct sk_buff *) data2);	//free buffer2
			TR("(Ring mode) rx buffer2 %08x of size %d from %d rx descriptor is given back\n",data2, length2, i);
		}
	}
	//plat_free_consistent_dmaable_memory(pcidev,(sizeof(DmaDesc) * gmacdev->RxDescCount),gmacdev->RxDesc,gmacdev->RxDescDma); //free descriptors memory	// jay hsu
	// jay hsu : free descriptors memory
	sys_plat_free_memory((u32 *)gmacdev->RxDesc);
	
	// jay hsu : free dma address
	//printk("jay debug : free vitural addr 0x%08x, dma_addr 0x%08x\n",(u32)gmacdev->RxDesc, gmacdev->RxDescDma);
	//dma_free_coherent(NULL, (sizeof(DmaDesc) * gmacdev->RxDescCount), gmacdev->RxDesc, gmacdev->RxDescDma);
	
	TR("Memory allocated %08x  for Rx Desriptors (ring) is given back\n",(u32)gmacdev->RxDesc);
}
else{
	TR("rx-------------------------------------------------------------------rx\n");
	first_desc          = gmacdev->RxDesc;
	//first_desc_dma_addr = gmacdev->RxDescDma;
	for(i =0; i < gmacdev -> RxDescCount; i++){
		synopGMAC_get_desc_data(first_desc, &status, &dma_addr1, &length1, &data1, &dma_addr2, &length2, &data2);
		TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",i,(u32)first_desc,first_desc->status,first_desc->length,first_desc->buffer1,first_desc->buffer2,first_desc->data1,first_desc->data2);
		if((length1 != 0) && (data1 != 0)){
			//pci_unmap_single(pcidev,dma_addr1,0,PCI_DMA_FROMDEVICE);	// jay hsu
			dev_kfree_skb_any((struct sk_buff *) data1);	// free buffer1
			TR("(Chain mode) rx buffer1 %08x of size %d from %d rx descriptor is given back\n",data1, length1, i);
		}
		//plat_free_consistent_dmaable_memory(pcidev,(sizeof(DmaDesc)),first_desc,first_desc_dma_addr); //free descriptors	// jay hsu
		// jay hsu : free descriptors memory
		sys_plat_free_memory((u32 *)first_desc);	// free descriptors
		
		// jay hsu : free dma address
		//dma_free_coherent(NULL, (sizeof(DmaDesc)), first_desc, first_desc_dma_addr);
		
		TR("Memory allocated %08x for Rx Descriptor (chain) at  %d is given back\n",data2,i);

		first_desc = (DmaDesc *)data2;
		//first_desc_dma_addr = dma_addr2;
	}

	TR("rx-------------------------------------------------------------------rx\n");
}
gmacdev->RxDesc    = NULL;
gmacdev->RxDescDma = 0;
return;
}

/**
  * This gives up the transmit Descriptor queue in ring or chain mode.
  * This function is tightly coupled to the platform and operating system
  * Once device's Dma is stopped the memory descriptor memory and the buffer memory deallocation,
  * is completely handled by the operating system, this call is kept outside the device driver Api.
  * This function should be treated as an example code to de-allocate the descriptor structures in ring mode or chain mode
  * and network buffer deallocation.
  * This function depends on the pcidev structure for dma-able memory deallocation for both descriptor memory and the
  * network buffer memory under linux.
  * The responsibility of this function is to 
  *     - Free the network buffer memory if any.
  *	- Fee the memory allocated for the descriptors.
  * @param[in] pointer to synopGMACdevice.
  * @param[in] pointer to pci_device structure.
  * @param[in] number of descriptor expected in tx descriptor queue.
  * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
  * \return 0 upon success. Error code upon failure.
  * \note No reference should be made to descriptors once this function is called. This function is invoked when the device is closed.
  */
void synopGMAC_giveup_tx_desc_queue(synopGMACdevice * gmacdev,struct pci_dev * pcidev, u32 desc_mode)
{
u32 i;

DmaDesc *first_desc = NULL;
//dma_addr_t first_desc_dma_addr;
u32 status;
dma_addr_t dma_addr1;
dma_addr_t dma_addr2;
u32 length1;
u32 length2;
u32 data1;
u32 data2;

if(desc_mode == RINGMODE){
	for(i =0; i < gmacdev -> TxDescCount; i++){
		synopGMAC_get_desc_data(gmacdev->TxDesc + i,&status, &dma_addr1, &length1, &data1, &dma_addr2, &length2, &data2);
		if((length1 != 0) && (data1 != 0)){
			//pci_unmap_single(pcidev,dma_addr1,0,PCI_DMA_TODEVICE);		// jay hsu
			dev_kfree_skb_any((struct sk_buff *) data1);	// free buffer1
			//TR("(Ring mode) tx buffer1 %08x of size %d from %d rx descriptor is given back\n",data1, length1, i);
		}
		if((length2 != 0) && (data2 != 0)){
			//pci_unmap_single(pcidev,dma_addr2,0,PCI_DMA_TODEVICE);		// jay hsu
			dev_kfree_skb_any((struct sk_buff *) data2);	//free buffer2
			//TR("(Ring mode) tx buffer2 %08x of size %d from %d rx descriptor is given back\n",data2, length2, i);
		}
	}
	//plat_free_consistent_dmaable_memory(pcidev,(sizeof(DmaDesc) * gmacdev->TxDescCount),gmacdev->TxDesc,gmacdev->TxDescDma); //free descriptors
	// jay hsu : free descriptors
	sys_plat_free_memory((u32 *)gmacdev->TxDesc);
	
	// jay hsu : free dma address
	//printk("jay debug : free vitural addr 0x%08x, dma_addr 0x%08x\n",(u32)gmacdev->TxDesc, gmacdev->TxDescDma);
	//dma_free_coherent(NULL, (sizeof(DmaDesc) * gmacdev->TxDescCount), gmacdev->TxDesc, gmacdev->TxDescDma);
	
	TR("Memory allocated %08x for Tx Desriptors (ring) is given back\n",(u32)gmacdev->TxDesc);
}
else{
	TR("tx-------------------------------------------------------------------tx\n");
	first_desc          = gmacdev->TxDesc;
	//first_desc_dma_addr = gmacdev->TxDescDma;
	for(i =0; i < gmacdev -> TxDescCount; i++){
		synopGMAC_get_desc_data(first_desc, &status, &dma_addr1, &length1, &data1, &dma_addr2, &length2, &data2);
		TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",i,(u32)first_desc,first_desc->status,first_desc->length,first_desc->buffer1,first_desc->buffer2,first_desc->data1,first_desc->data2);
		if((length1 != 0) && (data1 != 0)){
			//pci_unmap_single(pcidev,dma_addr1,0,PCI_DMA_TODEVICE);		// jay hsu
			dev_kfree_skb_any((struct sk_buff *) data2);	// free buffer1
			TR("(Chain mode) tx buffer1 %08x of size %d from %d rx descriptor is given back\n",data1, length1, i);
		}
		//plat_free_consistent_dmaable_memory(pcidev,(sizeof(DmaDesc)),first_desc,first_desc_dma_addr); //free descriptors	// jay hsu
		// jay hsu : free descriptors
		sys_plat_free_memory((u32 *)first_desc);
		
		// jay hsu : free dma address
		//dma_free_coherent(NULL, (sizeof(DmaDesc)), first_desc, first_desc_dma_addr);
		
		TR("Memory allocated %08x for Tx Descriptor (chain) at  %d is given back\n",data2,i);

		first_desc = (DmaDesc *)data2;
		//first_desc_dma_addr = dma_addr2;
	}
	TR("tx-------------------------------------------------------------------tx\n");

}
gmacdev->TxDesc    = NULL;
gmacdev->TxDescDma = 0;
return;
}


/**
 * Function to handle housekeeping after a packet is transmitted over the wire.
 * After the transmission of a packet DMA generates corresponding interrupt 
 * (if it is enabled). It takes care of returning the sk_buff to the linux
 * kernel, updating the networking statistics and tracking the descriptors.
 * @param[in] pointer to net_device structure. 
 * \return void.
 * \note This function runs in interrupt context
 */
void synop_handle_transmit_over(struct net_device *netdev)
{
	synopGMACPciNetworkAdapter *adapter;
	synopGMACdevice * gmacdev;
	//struct pci_dev *pcidev;		// jay hsu
	s32 desc_index;
	u32 data1, data2;
	u32 status;
	u32 length1, length2;
	u32 dma_addr1, dma_addr2;
#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif
	TR("%s called \n",__FUNCTION__);	// jay hsu : debug
	
	//adapter = netdev->priv;
	adapter = netdev_priv(netdev);		// use netdev_priv() @ 20100520
	if(adapter == NULL){
		TR("Unknown Device\n");
		return;
	}
	
	gmacdev = adapter->synopGMACdev;
	if(gmacdev == NULL){
		TR("GMAC device structure is missing\n");
		return;
	}

 	//pcidev  = (struct pci_dev *)adapter->synopGMACpcidev;			// jay hsu
	/*Handle the transmit Descriptors*/
	do {
#ifdef ENH_DESC_8W
	desc_index = synopGMAC_get_tx_qptr(gmacdev, &status, &dma_addr1, &length1, &data1, &dma_addr2, &length2, &data2,&ext_status,&time_stamp_high,&time_stamp_low);
        synopGMAC_TS_read_timestamp_higher_val(gmacdev, &time_stamp_higher);
#else
	desc_index = synopGMAC_get_tx_qptr(gmacdev, &status, &dma_addr1, &length1, &data1, &dma_addr2, &length2, &data2);
	
	// jay hsu : debug
	//printk("jay debug : ##Transmit over## desc_index %d, dma_addr1 0x%x, data1 0x%x, len %d\n", desc_index, (u32)dma_addr1, data1, length1);
	
#endif
	//desc_index = synopGMAC_get_tx_qptr(gmacdev, &status, &dma_addr, &length, &data1);
		if(desc_index >= 0 && data1 != 0){
			TR("Finished Transmit at Tx Descriptor %d for skb 0x%08x and buffer = %08x whose status is %08x \n", desc_index,data1,dma_addr1,status);
			#ifdef	IPC_OFFLOAD
			if(synopGMAC_is_tx_ipv4header_checksum_error(gmacdev, status)){
			TR("Harware Failed to Insert IPV4 Header Checksum\n");
			}
			if(synopGMAC_is_tx_payload_checksum_error(gmacdev, status)){
			TR("Harware Failed to Insert Payload Checksum\n");
			}
			#endif
		
			//printk("jay debug :[TX] free skb 0x%08x\n", (u32)data1);
			//pci_unmap_single(pcidev,dma_addr1,length1,PCI_DMA_TODEVICE);		// jay hsu
			dev_kfree_skb_any((struct sk_buff *)data1);

			if(synopGMAC_is_desc_valid(status)){
				adapter->synopGMACNetStats.tx_bytes += length1;
				adapter->synopGMACNetStats.tx_packets++;
				TR("<<tx_bytes>> 0x%lx\n", adapter->synopGMACNetStats.tx_bytes);
				TR("<<tx_packets>> %ld\n", adapter->synopGMACNetStats.tx_packets);
			}
			else {	
				TR("Error in Status %08x\n",status);
				printk("TX : Error in Status %08x\n",status);
				adapter->synopGMACNetStats.tx_errors++;
				adapter->synopGMACNetStats.tx_aborted_errors += synopGMAC_is_tx_aborted(status);
				adapter->synopGMACNetStats.tx_carrier_errors += synopGMAC_is_tx_carrier_error(status);
			}
		
			// jay hsu @ 20100717 : driver bug fixed. Only count the collision when desc_index isn't -1.
			adapter->synopGMACNetStats.collisions += synopGMAC_get_tx_collision_count(status);
			//printk("jay debug : tx status 0x%08x\n", status);
			//printk("jay debug : collisions %ld\n", adapter->synopGMACNetStats.collisions);
		
		}	
				
		//adapter->synopGMACNetStats.collisions += synopGMAC_get_tx_collision_count(status);
		//printk("jay debug : tx status 0x%08x\n", status);
		//printk("jay debug : collisions %ld\n", adapter->synopGMACNetStats.collisions);
		
	} while(desc_index >= 0);
	netif_wake_queue(netdev);
}




/**
 * Function to Receive a packet from the interface.
 * After Receiving a packet, DMA transfers the received packet to the system memory
 * and generates corresponding interrupt (if it is enabled). This function prepares
 * the sk_buff for received packet after removing the ethernet CRC, and hands it over
 * to linux networking stack.
 * 	- Updataes the networking interface statistics
 *	- Keeps track of the rx descriptors
 * @param[in] pointer to net_device structure. 
 * \return void.
 * \note This function runs in interrupt context.
 */

void synop_handle_received_data(struct net_device *netdev)
{
	synopGMACPciNetworkAdapter *adapter;
	synopGMACdevice * gmacdev;
	//struct pci_dev *pcidev;	// jay hsu
	s32 desc_index;
	
	u32 data1;
	u32 data2;
	u32 len;
	u32 status;
	u32 dma_addr1;
	u32 dma_addr2;
#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif
	//u32 length;
	
	struct sk_buff *skb; //This is the pointer to hold the received data
	
	TR("%s\n",__FUNCTION__);	
	
	//adapter = netdev->priv;
	adapter = netdev_priv(netdev);		// use netdev_priv() @ 20100520
	if(adapter == NULL){
		TR("Unknown Device\n");
		return;
	}
	
	gmacdev = adapter->synopGMACdev;
	if(gmacdev == NULL){
		TR("GMAC device structure is missing\n");
		return;
	}
	
 	//pcidev  = (struct pci_dev *)adapter->synopGMACpcidev;			// jay hsu
	/*Handle the Receive Descriptors*/
	do{
#ifdef ENH_DESC_8W
	desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,&dma_addr1,NULL, &data1,&dma_addr2,NULL,&data2,&ext_status,&time_stamp_high,&time_stamp_low);
	if(desc_index >0){ 
        synopGMAC_TS_read_timestamp_higher_val(gmacdev, &time_stamp_higher);
	TR("S:%08x ES:%08x DA1:%08x d1:%08x DA2:%08x d2:%08x TSH:%08x TSL:%08x TSHW:%08x \n",status,ext_status,dma_addr1, data1,dma_addr2,data2, time_stamp_high,time_stamp_low,time_stamp_higher);
	}
#else
	desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,&dma_addr1,NULL, &data1,&dma_addr2,NULL,&data2);
	// jay hsu : debug
	TR("##Handle Received## desc_index %d, dma_addr1 0x%x, data1 0x%x\n", desc_index, (u32)dma_addr1, data1);
	//printk("jay debug : ##Handle Received## desc_index %d, dma_addr1 0x%x, data1 0x%x\n", desc_index, (u32)dma_addr1, data1);
#endif
	//desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,&dma_addr,NULL, &data1);

		if(desc_index >= 0 && data1 != 0){
			TR("Received Data at Rx Descriptor %d for skb 0x%08x whose status is %08x\n",desc_index,data1,status);
			/*At first step unmapped the dma address*/
			//pci_unmap_single(pcidev,dma_addr1,0,PCI_DMA_FROMDEVICE);		// jay hsu
			skb = (struct sk_buff *)data1;
			if(synopGMAC_is_rx_desc_valid(status)){
				len =  synopGMAC_get_rx_desc_frame_length(status) - 4; //Not interested in Ethernet CRC bytes
				
				// jay debug : print the skb structure
//				printk("jay debug : [RX] put skb 0x%08x, len %d\n", (u32)skb, len);
//				printk("jay debug : skb->head %p\n", skb->head);
//				printk("jay debug : skb->data %p\n", skb->data);
//				printk("jay debug : skb->tail %p\n", skb->tail);
//				printk("jay debug : skb->end %p\n", skb->end);
				
				skb_put(skb,len);
			#ifdef IPC_OFFLOAD
				// Now lets check for the IPC offloading
				/*  Since we have enabled the checksum offloading in hardware, lets inform the kernel
				    not to perform the checksum computation on the incoming packet. Note that ip header 
  				    checksum will be computed by the kernel immaterial of what we inform. Similary TCP/UDP/ICMP
				    pseudo header checksum will be computed by the stack. What we can inform is not to perform
				    payload checksum. 		
   				    When CHECKSUM_UNNECESSARY is set kernel bypasses the checksum computation.		    
				*/
				
				TR("Checksum Offloading will be done now\n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;
				
				#ifdef ENH_DESC_8W
				if(synopGMAC_is_ext_status(gmacdev, status)){ // extended status present indicates that the RDES4 need to be probed
					TR("Extended Status present\n");
					if(synopGMAC_ES_is_IP_header_error(gmacdev,ext_status)){       // IP header (IPV4) checksum error
					//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
					TR("(EXTSTS)Error in IP header error\n");
					skb->ip_summed = CHECKSUM_NONE;     //Let Kernel compute the checkssum
					}	
					if(synopGMAC_ES_is_rx_checksum_bypassed(gmacdev,ext_status)){   // Hardware engine bypassed the checksum computation/checking
					TR("(EXTSTS)Hardware bypassed checksum computation\n");	
					skb->ip_summed = CHECKSUM_NONE;             // Let Kernel compute the checksum
					}
					if(synopGMAC_ES_is_IP_payload_error(gmacdev,ext_status)){       // IP payload checksum is in error (UDP/TCP/ICMP checksum error)
					TR("(EXTSTS) Error in EP payload\n");	
					skb->ip_summed = CHECKSUM_NONE;             // Let Kernel compute the checksum
					}				
				}
				else{ // No extended status. So relevant information is available in the status itself
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxNoChkError ){
					TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 4>  \n");
					skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass computing the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
					//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
					TR(" Error in 16bit IPV4 Header Checksum <Chk Status = 6>  \n");
					skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass the TCP/UDP checksum computation
					}				
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxLenLT600 ){
					TR("IEEE 802.3 type frame with Length field Lesss than 0x0600 <Chk Status = 0> \n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrPayLoadChkBypass ){
					TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 1>\n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxChkBypass ){
					TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 3>  \n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxPayLoadChkError ){
					TR(" TCP/UDP payload checksum Error <Chk Status = 5>  \n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
					//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
					TR(" Both IP header and Payload Checksum Error <Chk Status = 7>  \n");
					skb->ip_summed = CHECKSUM_NONE;	        //Let Kernel compute the Checksum
					}
				}
				#else	
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxNoChkError ){
				TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 4>  \n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass computing the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
				//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
				TR(" Error in 16bit IPV4 Header Checksum <Chk Status = 6>  \n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass the TCP/UDP checksum computation
				}				
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxLenLT600 ){
				TR("IEEE 802.3 type frame with Length field Lesss than 0x0600 <Chk Status = 0> \n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrPayLoadChkBypass ){
				TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 1>\n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxChkBypass ){
				TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 3>  \n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxPayLoadChkError ){
				TR(" TCP/UDP payload checksum Error <Chk Status = 5>  \n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
				//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
				TR(" Both IP header and Payload Checksum Error <Chk Status = 7>  \n");
				skb->ip_summed = CHECKSUM_NONE;	        //Let Kernel compute the Checksum
				}
				
				#endif
			#endif //IPC_OFFLOAD	
				skb->dev = netdev;
				skb->protocol = eth_type_trans(skb, netdev);
				netif_rx(skb);
				netdev->last_rx = jiffies;
				adapter->synopGMACNetStats.rx_packets++;
				adapter->synopGMACNetStats.rx_bytes += len;
			}
			else{
				/*Now the present skb should be set free*/
				dev_kfree_skb_any(skb);
				printk(KERN_CRIT "s: %08x\n",status);
				adapter->synopGMACNetStats.rx_errors++;
				adapter->synopGMACNetStats.collisions       += synopGMAC_is_rx_frame_collision(status);
				adapter->synopGMACNetStats.rx_crc_errors    += synopGMAC_is_rx_crc(status);
				adapter->synopGMACNetStats.rx_frame_errors  += synopGMAC_is_frame_dribbling_errors(status);
				adapter->synopGMACNetStats.rx_length_errors += synopGMAC_is_rx_frame_length_errors(status);
			}
			
			//Now lets allocate the skb for the emptied descriptor
			skb = dev_alloc_skb(netdev->mtu + ETHERNET_PACKET_EXTRA);
			if(skb == NULL){
				TR("SKB memory allocation failed \n");
				adapter->synopGMACNetStats.rx_dropped++;
			}
			
			//printk("jay debug : [RX] set skb 0x%08x\n", (u32)skb);			
			//dma_addr1 = pci_map_single(pcidev,skb->data,skb_tailroom(skb),PCI_DMA_FROMDEVICE);	// jay hsu			
			//desc_index = synopGMAC_set_rx_qptr(gmacdev,dma_addr1, skb_tailroom(skb), (u32)skb,0,0,0);	// jay hsu

			desc_index = synopGMAC_set_rx_qptr(gmacdev,virt_to_phys(skb->data), (u32)skb_tailroom(skb), (u32)skb,0,0,0);
			if(desc_index < 0){
				TR("Cannot set Rx Descriptor for skb %08x\n",(u32)skb);
				printk("Cannot set Rx Descriptor for skb %08x\n",(u32)skb);	// jay hsu : debug test it
				dev_kfree_skb_any(skb);
			}
		
			//Rx_Int_Flag = 0;
		}
	}while(desc_index >= 0);
	
	// jay hsu : test 
//	if ( Rx_Int_Flag == 1)
//	{
//		gmacdev->RxBusyDesc++;
//		printk("## RxBusyDesc add one## curent rx 0x%08x\n", (u32)gmacdev->RxBusyDesc);
//		Rx_Int_Flag = 0;
//	}

}


/**
 * Interrupt service routing.
 * This is the function registered as ISR for device interrupts.
 * @param[in] interrupt number. 
 * @param[in] void pointer to device unique structure (Required for shared interrupts in Linux).
 * @param[in] pointer to pt_regs (not used).
 * \return Returns IRQ_NONE if not device interrupts IRQ_HANDLED for device interrupts.
 * \note This function runs in interrupt context
 *
 */
//irqreturn_t synopGMAC_intr_handler(s32 intr_num, void * dev_id, struct pt_regs *regs)
irqreturn_t synopGMAC_intr_handler(int irq, void *dev_id)
{       
	/*Kernels passes the netdev structure in the dev_id. So grab it*/
        struct net_device *netdev;
        synopGMACPciNetworkAdapter *adapter;
        synopGMACdevice * gmacdev;
	//struct pci_dev *pcidev;	// jay hsu
        u32 interrupt,dma_status_reg;
	s32 status;
	//u32 dma_addr;

	TR("%s called \n",__FUNCTION__);	// jay hsu : debug
	
        netdev  = (struct net_device *) dev_id;
        if(netdev == NULL){
                TR("Unknown Device\n");
                return -1;
        }

        //adapter  = netdev->priv;
        adapter  = netdev_priv(netdev);		// use netdev_priv() @ 20100520
        if(adapter == NULL){
                TR("Adapter Structure Missing\n");
                return -1;
        }

        gmacdev = adapter->synopGMACdev;
        if(gmacdev == NULL){
                TR("GMAC device structure Missing\n");
                return -1;
        }

	//pcidev  = (struct pci_dev *)adapter->synopGMACpcidev;		// jay hsu
	/*Read the Dma interrupt status to know whether the interrupt got generated by our device or not*/
	dma_status_reg = synopGMACReadReg((u32 *)gmacdev->DmaBase, DmaStatus);
	
	if(dma_status_reg == 0)
		return IRQ_NONE;

        synopGMAC_disable_interrupt_all(gmacdev);

       	TR("%s:Dma Status Reg: 0x%08x\n",__FUNCTION__,dma_status_reg);

	// jay hsu : debug test start
	//printk("GMac(in) 0x%08x\n", *((volatile unsigned int *)0xbd05104c));       	
//    if ( dma_status_reg & DmaIntRxNoBuffer)
//    {
//    	//printk("[NTKETHMAC] : No RX Desc can be used!!\n");
//    	//printk("GMac current rx 0x%08x\n", *((volatile u32 *)0xbd05104c));
//    	//printk("driver current rx 0x%08x\n", (u32)gmacdev->RxBusyDesc);
//    }
//    // jay hsu : debug test end
	
	if(dma_status_reg & GmacPmtIntr){
		TR("%s:: Interrupt due to PMT module\n",__FUNCTION__);
		synopGMAC_linux_powerup_mac(gmacdev);
	}
	
	if(dma_status_reg & GmacMmcIntr){
		TR("%s:: Interrupt due to MMC module\n",__FUNCTION__);
		TR("%s:: synopGMAC_rx_int_status = %08x\n",__FUNCTION__,synopGMAC_read_mmc_rx_int_status(gmacdev));
		TR("%s:: synopGMAC_tx_int_status = %08x\n",__FUNCTION__,synopGMAC_read_mmc_tx_int_status(gmacdev));
	}

	if(dma_status_reg & GmacLineIntfIntr){
		TR("%s:: Interrupt due to GMAC LINE module\n",__FUNCTION__);
	}

	/*Now lets handle the DMA interrupts*/  
        interrupt = synopGMAC_get_interrupt_type(gmacdev);
       	TR("%s:Interrupts to be handled: 0x%08x\n",__FUNCTION__,interrupt);

        if(interrupt & synopGMACDmaError){
		//u8 mac_addr0[6] = DEFAULT_MAC_ADDRESS;//after soft reset, configure the MAC address to default value
		TR("%s::Fatal Bus Error Inetrrupt Seen\n",__FUNCTION__);
		synopGMAC_disable_dma_tx(gmacdev);
                synopGMAC_disable_dma_rx(gmacdev);
                
		synopGMAC_take_desc_ownership_tx(gmacdev);
		synopGMAC_take_desc_ownership_rx(gmacdev);
		
		synopGMAC_init_tx_rx_desc_queue(gmacdev);
		
		synopGMAC_reset(gmacdev);//reset the DMA engine and the GMAC ip
		
		synopGMAC_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, Mac_Addr); 
		synopGMAC_dma_bus_mode_init(gmacdev,DmaFixedBurstEnable| DmaBurstLength8 | DmaDescriptorSkip6 );
	 	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward);	
		synopGMAC_init_rx_desc_base(gmacdev);
		synopGMAC_init_tx_desc_base(gmacdev);
		synopGMAC_mac_init(gmacdev);
		synopGMAC_enable_dma_rx(gmacdev);
		synopGMAC_enable_dma_tx(gmacdev);

        }

	if(interrupt & synopGMACDmaRxNormal){
		TR("%s:: Rx Normal \n", __FUNCTION__);
		//printk("%s:: Rx Normal \n", __FUNCTION__);		// jay hsu : debug
				//Rx_Int_Flag = 1;
			
#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
		//printk("jay debug : call napi schedule\n");
		if (likely(napi_schedule_prep(&nvt_napi))) {
			atomic_set(&napi_poll, 1);
			//printk("jay debug : netif_schedule, napi_poll %d\n", atomic_read(&napi_poll));	// jay hsu
			__napi_schedule(&nvt_napi);
		}		
#else
		synop_handle_received_data(netdev);
#endif
/*			
			if ( use_napi )
			{			
				//atomic_inc(&napi_poll);
				atomic_set(&napi_poll, 1);
				//printk("call netif_rx_schedule, napi_poll %d\n", atomic_read(&napi_poll));	// jay hsu
				//netif_rx_schedule(netdev);		// jay hsu : not implement napi function yet @ 20100520
			}
			else
                synop_handle_received_data(netdev);
*/               
		//printk("deubg : receive end, current rx 0x%08x\n", (u32)gmacdev->RxBusyDesc);                
	}

        if(interrupt & synopGMACDmaRxAbnormal){
	        TR("%s::Abnormal Rx Interrupt Seen\n",__FUNCTION__);
//	        printk("%s::Abnormal Rx Interrupt Seen\n",__FUNCTION__);	// jay hsu : debug 
//	        printk("do flow control function\n");
	        printk("[NTKETHMAC] : Abnormal Rx, Do Flow Control!!\n");
	
			synopGMAC_tx_activate_flow_control(gmacdev);	// jay hsu : need deactive flow control in half-duplex @ 20100602
	
			// jay hsu : send flow control packets
			//*((volatile u32 *)0xbd050018) |= 0xffff0000;	// set the max time field of pause frame
			//*((volatile u32 *)0xbd050018) |= 0x1;			// trigger pause frame
			//printk("send flow control packets!!\n");
//		#if 1
//	
//	       if(GMAC_Power_down == 0){	// If Mac is not in powerdown
//                adapter->synopGMACNetStats.rx_over_errors++;
//		/*Now Descriptors have been created in synop_handle_received_data(). Just issue a poll demand to resume DMA operation*/
//		synopGMAC_resume_dma_rx(gmacdev);//To handle GBPS with 12 descriptors
//		}
//		#endif
	}


        if(interrupt & synopGMACDmaRxStopped){
        	TR("%s::Receiver stopped seeing Rx interrupts\n",__FUNCTION__); //Receiver gone in to stopped state
		#if 1
	        if(GMAC_Power_down == 0){	// If Mac is not in powerdown
		adapter->synopGMACNetStats.rx_over_errors++;
		do{
			struct sk_buff *skb = alloc_skb(netdev->mtu + ETHERNET_HEADER + ETHERNET_CRC, GFP_ATOMIC);
			if(skb == NULL){
				TR("%s::ERROR in skb buffer allocation Better Luck Next time\n",__FUNCTION__);
				break;
				//			return -ESYNOPGMACNOMEM;
			}
			
			//dma_addr = pci_map_single(pcidev,skb->data,skb_tailroom(skb),PCI_DMA_FROMDEVICE);	// jay hsu
			//status = synopGMAC_set_rx_qptr(gmacdev,dma_addr, skb_tailroom(skb), (u32)skb,0,0,0);	// jay hsu
			status = synopGMAC_set_rx_qptr(gmacdev,virt_to_phys(skb->data), (u32)skb_tailroom(skb), (u32)skb,0,0,0);
			TR("%s::Set Rx Descriptor no %08x for skb %08x \n",__FUNCTION__,status,(u32)skb);
			if(status < 0)
				dev_kfree_skb_any(skb);//changed from dev_free_skb. If problem check this again--manju
		
		}while(status >= 0);
		
		synopGMAC_enable_dma_rx(gmacdev);
		}
		#endif
	}

	if(interrupt & synopGMACDmaTxNormal){
		//xmit function has done its job
		TR("%s::Finished Normal Transmission \n",__FUNCTION__);
		synop_handle_transmit_over(netdev);//Do whatever you want after the transmission is over

		
	}

        if(interrupt & synopGMACDmaTxAbnormal){
		TR("%s::Abnormal Tx Interrupt Seen\n",__FUNCTION__);
		printk("%s::Abnormal Tx Interrupt Seen\n",__FUNCTION__);
		#if 1
	       if(GMAC_Power_down == 0){	// If Mac is not in powerdown
                synop_handle_transmit_over(netdev);
		}
		#endif
	}


	if(interrupt & synopGMACDmaTxStopped){
		TR("%s::Transmitter stopped sending the packets\n",__FUNCTION__);
		printk("%s::Transmitter stopped sending the packets\n",__FUNCTION__);
		#if 1
	       if(GMAC_Power_down == 0){	// If Mac is not in powerdown
		synopGMAC_disable_dma_tx(gmacdev);
                synopGMAC_take_desc_ownership_tx(gmacdev);
		
		synopGMAC_enable_dma_tx(gmacdev);
		netif_wake_queue(netdev);
		TR("%s::Transmission Resumed\n",__FUNCTION__);
		}
		#endif
	}

	// jay hsu : debug test
	//printk("GMac(out) 0x%08x\n", *((volatile unsigned int *)0xbd05104c));       	

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
	if ( atomic_read(&napi_poll)==1 )
		// jay hsu : ignore the rx complete
		synopGMAC_enable_interrupt(gmacdev,DmaIntEnable&(~DmaIntRxNormMask));
	else
		/* Enable the interrrupt before returning from ISR*/
		synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
#else
    	/* Enable the interrrupt before returning from ISR*/
		synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
#endif	

//	//printk("jay hsu : leave interrupt, poll %d\n", atomic_read(&napi_poll));
//	if ( use_napi && atomic_read(&napi_poll)==1 )
//	{
//		// jay hsu : ignore the rx complete
//		synopGMAC_enable_interrupt(gmacdev,DmaIntEnable&(~DmaIntRxNormMask));
//	}
//	else
//	{
//        /* Enable the interrrupt before returning from ISR*/
//        synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
//	}

	return IRQ_HANDLED;
}


/**
 * Function used when the interface is opened for use.
 * We register synopGMAC_linux_open function to linux open(). Basically this 
 * function prepares the the device for operation . This function is called whenever ifconfig (in Linux)
 * activates the device (for example "ifconfig eth0 up"). This function registers
 * system resources needed 
 * 	- Attaches device to device specific structure
 * 	- Programs the MDC clock for PHY configuration
 * 	- Check and initialize the PHY interface 
 *	- ISR registration
 * 	- Setup and initialize Tx and Rx descriptors
 *	- Initialize MAC and DMA
 *	- Allocate Memory for RX descriptors (The should be DMAable)
 * 	- Initialize one second timer to detect cable plug/unplug
 *	- Configure and Enable Interrupts
 *	- Enable Tx and Rx
 *	- start the Linux network queue interface
 * @param[in] pointer to net_device structure. 
 * \return Returns 0 on success and error status upon failure.
 * \callgraph
 */

s32 synopGMAC_linux_open(struct net_device *netdev)
{
	//u8 mac_addr0[6] = DEFAULT_MAC_ADDRESS;
	s32 status = 0;
	s32 retval = 0;
	s32 ijk;
	s32 count;
	//s32 reserve_len=2;
	//u32 dma_addr;
	struct sk_buff *skb;
        synopGMACPciNetworkAdapter *adapter;
        synopGMACdevice * gmacdev;
	//struct pci_dev *pcidev;		// jay hsu
	TR0("%s called \n",__FUNCTION__);
	//adapter = (synopGMACPciNetworkAdapter *) netdev->priv;
	adapter = (synopGMACPciNetworkAdapter *) netdev_priv(netdev);	// use netdev_priv() @ 2010520
	gmacdev = (synopGMACdevice *)adapter->synopGMACdev;
 	//pcidev  = (struct pci_dev *)adapter->synopGMACpcidev;		// jay hsu
	
	/*Now platform dependent initialization.*/

	/*Lets reset the IP*/
	//TR("adapter= %08x gmacdev = %08x netdev = %08x pcidev= %08x\n",(u32)adapter,(u32)gmacdev,(u32)netdev,(u32)pcidev);	// jay hsu
	//TR("<synopGMAC> reset GMac\n");	// jay hsu
	//synopGMAC_reset(gmacdev);			// jay hsu
	
	/*Attach the device to MAC struct This will configure all the required base addresses
	  such as Mac base, configuration base, phy base address(out of 32 possible phys )*/
	// jay hsu
	//synopGMAC_attach(synopGMACadapter->synopGMACdev,(u32) synopGMACMappedAddr + MACBASE,(u32) synopGMACMappedAddr + DMABASE, DEFAULT_PHY_BASE);

	/*Lets read the version of ip in to device structure*/	
	synopGMAC_read_version(gmacdev);
	
	// jay hsu : set mac address again
	synopGMAC_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, Mac_Addr); 
	
	synopGMAC_get_mac_addr(synopGMACadapter->synopGMACdev,GmacAddr0High,GmacAddr0Low, netdev->dev_addr); 
	/*Now set the broadcast address*/	
	for(ijk = 0; ijk <6; ijk++){
	netdev->broadcast[ijk] = 0xff;
	}

	for(ijk = 0; ijk <6; ijk++){
	TR("netdev->dev_addr[%d] = %02x and netdev->broadcast[%d] = %02x\n",ijk,netdev->dev_addr[ijk],ijk,netdev->broadcast[ijk]);
	}
	/*Check for Phy initialization*/
	synopGMAC_set_mdc_clk_div(gmacdev,(u32)s32MDC_CLOCK);
	gmacdev->ClockDivMdc = synopGMAC_get_mdc_clk_div(gmacdev);

#if 0	// jay hsu @ 20110214 : remove the phy reset
	// Jay Hsu : reset phy
	if ( synopGMAC_reset_phy(gmacdev) == -ESYNOPGMACPHYERR )
	{
		printk("[NTKETHMAC] : reset Micrel phy failed\n");
		//TR("## reset Micrel phy failed##\n");	
		return -ESYNOPGMACPHYERR;
	}
	else
		printk("[NTKETHMAC] : reset Micrel phy okay\n");
		//TR("## reset Micrel phy okay##\n");
#endif		
	
#if 0
	// jay hsu : force Micrel KSZ8041NL phy in 10Mbps full-duplex mode for vicki streaming
	printk("!!!Force phy run in 100Mbps half-duplex!!!\n");
	synopGMAC_set_phy_in_100M(gmacdev);
#endif

	// Jay Hsu : check phy id until phy reset procedure done
	count = 100;
	while (count--)
	{        
#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT
		if ( (synopGMAC_get_phy_id(gmacdev) == MICREL_PHY_ID1) ||  (synopGMAC_get_phy_id(gmacdev) == MICREL_PHY_ID2) )
#elif defined CONFIG_NVT_RTL_PHY_SUPPORT
		if ( (synopGMAC_get_phy_id(gmacdev) == RTL8201E_PHY_ID ) )
#endif		
		{
#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT			
			printk("[NTKETHMAC] : Micrel Ethernet Phy ready\n");
#elif defined CONFIG_NVT_RTL_PHY_SUPPORT
			printk("[NTKETHMAC] : Realtek Ethernet Phy ready\n");
#endif			
			break;
		}
		udelay(1);
	}
	if ( count < 0 )
	{
	 	TR("##Not Micrel phy##, can't open ethernet interface\n");
#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT	 	
	 	printk("[NTKETHMAC] : Not Micrel phy, can't initialize Micrel Ethernet Phy\n");
#elif defined CONFIG_NVT_RTL_PHY_SUPPORT
		printk("[NTKETHMAC] : Not Realtek phy, can't initialize Realtek Ethernet Phy\n");	
#endif	 	
	 	return ESYNOPGMACPHYERR;
	}

	//status = synopGMAC_check_phy_init(gmacdev);
	
	// jay hsu : check phy init speed for our Micrel phy
	synopGMAC_check_phy_init_for_10_100M(gmacdev);
	

	// jay hsu : need to install our irq function
#define GMAC_IRQ	36
	/*Request for an shared interrupt. Instead of using netdev->irq lets use pcidev->irq*/
	//if(request_irq (GMAC_IRQ/*pcidev->irq*/, synopGMAC_intr_handler, SA_SHIRQ | SA_INTERRUPT, netdev->name,netdev)){
	// use new IRQ definition @ 20100520
	if(request_irq (GMAC_IRQ/*pcidev->irq*/, synopGMAC_intr_handler, IRQF_DISABLED, netdev->name,netdev)){
 		TR0("Error in request_irq\n");
		goto error_in_irq;	
	}
	
	
	
 	//TR("%s owns a shared interrupt on line %d\n",netdev->name, pcidev->irq);	// jay hsu

	/*Set up the tx and rx descriptor queue/ring*/

	//synopGMAC_setup_tx_desc_queue(gmacdev,pcidev,TRANSMIT_DESC_SIZE, RINGMODE);
	synopGMAC_setup_tx_desc_queue(gmacdev,NULL,TRANSMIT_DESC_SIZE, RINGMODE);
//	synopGMAC_setup_tx_desc_queue(gmacdev,pcidev,TRANSMIT_DESC_SIZE, CHAINMODE);
	synopGMAC_init_tx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr


	//synopGMAC_setup_rx_desc_queue(gmacdev,pcidev,RECEIVE_DESC_SIZE, RINGMODE);
	synopGMAC_setup_rx_desc_queue(gmacdev,NULL,RECEIVE_DESC_SIZE, RINGMODE);
//	synopGMAC_setup_rx_desc_queue(gmacdev,pcidev,RECEIVE_DESC_SIZE, CHAINMODE);
	synopGMAC_init_rx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr

//	synopGMAC_dma_bus_mode_init(gmacdev,DmaFixedBurstEnable| DmaBurstLength8 | DmaDescriptorSkip2 );
//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength32 | DmaDescriptorSkip2 );
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame);	
	
//	synopGMAC_dma_bus_mode_init(gmacdev,DmaFixedBurstEnable | DmaBurstLength8 | DmaDescriptorSkip2 ); //pbl8 incrx
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl64);	

//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength8 | DmaDescriptorSkip2 );                      //pbl8 incr
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl64);	

//	synopGMAC_dma_bus_mode_init(gmacdev,DmaFixedBurstEnable | DmaBurstLength16 | DmaDescriptorSkip2 ); //pbl16 incrx
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl64);	

//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength16 | DmaDescriptorSkip2 );                      //pbl16 incr
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl64);	

//#ifdef ENH_DESC_8W
//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength32 | DmaDescriptorSkip2 | DmaDescriptor8Words ); //pbl32 incr with rxthreshold 128 and Desc is 8 Words
//#else
//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength32 | DmaDescriptorSkip2);                      //pbl32 incr with rxthreshold 128
//#endif

	// jay hsu : use burst length 8
	//synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength8 | DmaDescriptorSkip1);
	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength8 | DmaDescriptorSkip6);	// jay hsu : skip 6x8 bytes to get next descriptors
	
	// jay hsu : don't use Operate on second frame
	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaRxThreshCtrl128);
	//synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl128);	

//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength64 | DmaDescriptorSkip2 );                      //pbl64 incr with rxthreshold 128
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl128);	

//	synopGMAC_dma_bus_mode_init(gmacdev, DmaBurstLength64 | DmaDescriptorSkip2 );                      //pbl64 incr with rxthreshold 128
//	synopGMAC_dma_control_init(gmacdev,DmaStoreAndForward |DmaRxThreshCtrl128);	

	/*Initialize the mac interface*/

	synopGMAC_mac_init(gmacdev);
	synopGMAC_pause_control(gmacdev); // This enables the pause control in Full duplex mode of operation
	
	#ifdef IPC_OFFLOAD
	/*IPC Checksum offloading is enabled for this driver. Should only be used if Full Ip checksumm offload engine is configured in the hardware*/
	synopGMAC_enable_rx_chksum_offload(gmacdev);  	//Enable the offload engine in the receive path
	synopGMAC_rx_tcpip_chksum_drop_enable(gmacdev); // This is default configuration, DMA drops the packets if error in encapsulated ethernet payload
							// The FEF bit in DMA control register is configured to 0 indicating DMA to drop the errored frames.
	/*Inform the Linux Networking stack about the hardware capability of checksum offloading*/
	netdev->features = NETIF_F_HW_CSUM;
	#endif
	
     do{
		skb = alloc_skb(netdev->mtu + ETHERNET_HEADER + ETHERNET_CRC, GFP_ATOMIC);
		if(skb == NULL){
			TR0("ERROR in skb buffer allocation\n");
			break;
//			return -ESYNOPGMACNOMEM;
		}
//		skb_reserve(skb,reserve_len);
//		TR("skb = %08x skb->tail = %08x skb_tailroom(skb)=%08x skb->data = %08x\n",(u32)skb,(u32)skb->tail,(skb_tailroom(skb)),(u32)skb->data);
//		skb->dev = netdev;
				
		//dma_addr = pci_map_single(pcidev,skb->data,skb_tailroom(skb),PCI_DMA_FROMDEVICE);	// jay hsu
		//status = synopGMAC_set_rx_qptr(gmacdev,dma_addr, skb_tailroom(skb), (u32)skb,0,0,0);	// jay hsu
		status = synopGMAC_set_rx_qptr(gmacdev,virt_to_phys(skb->data), (u32)skb_tailroom(skb), (u32)skb,0,0,0);
		if(status < 0)
			dev_kfree_skb_any(skb);
			
	}while(status >= 0);
	

	TR("Setting up the cable unplug timer\n");
	init_timer(&synopGMAC_cable_unplug_timer);
	synopGMAC_cable_unplug_timer.function = (void *)synopGMAC_linux_cable_unplug_function;
	synopGMAC_cable_unplug_timer.data = (u32) adapter;
	synopGMAC_cable_unplug_timer.expires = CHECK_TIME + jiffies;
	add_timer(&synopGMAC_cable_unplug_timer);

	synopGMAC_clear_interrupt(gmacdev);
	/*
	Disable the interrupts generated by MMC and IPC counters.
	If these are not disabled ISR should be modified accordingly to handle these interrupts.
	*/	
	synopGMAC_disable_mmc_tx_interrupt(gmacdev, 0xFFFFFFFF);
	synopGMAC_disable_mmc_rx_interrupt(gmacdev, 0xFFFFFFFF);
	synopGMAC_disable_mmc_ipc_rx_interrupt(gmacdev, 0xFFFFFFFF);

	synopGMAC_enable_interrupt(gmacdev,DmaIntEnable);
	synopGMAC_enable_dma_rx(gmacdev);
	synopGMAC_enable_dma_tx(gmacdev);
	
	netif_start_queue(netdev);

#ifdef	CONFIG_NVT_FASTETH_MAC_NAPI
	napi_enable(&nvt_napi);	
#endif

	return retval;

error_in_irq:
	/*Lets free the allocated memory*/
	plat_free_memory(gmacdev);
	return -ESYNOPGMACBUSY;
}

/**
 * Function used when the interface is closed.
 *
 * This function is registered to linux stop() function. This function is 
 * called whenever ifconfig (in Linux) closes the device (for example "ifconfig eth0 down").
 * This releases all the system resources allocated during open call.
 * system resources int needs 
 * 	- Disable the device interrupts
 * 	- Stop the receiver and get back all the rx descriptors from the DMA
 * 	- Stop the transmitter and get back all the tx descriptors from the DMA 
 * 	- Stop the Linux network queue interface
 *	- Free the irq (ISR registered is removed from the kernel)
 * 	- Release the TX and RX descripor memory
 *	- De-initialize one second timer rgistered for cable plug/unplug tracking
 * @param[in] pointer to net_device structure. 
 * \return Returns 0 on success and error status upon failure.
 * \callgraph
 */

s32 synopGMAC_linux_close(struct net_device *netdev)
{
	
//	s32 status = 0;
//	s32 retval = 0;
//	u32 dma_addr;
	synopGMACPciNetworkAdapter *adapter;
        synopGMACdevice * gmacdev;
	//struct pci_dev *pcidev;
	
	TR0("%s\n",__FUNCTION__);
	//adapter = (synopGMACPciNetworkAdapter *) netdev->priv;
	adapter = (synopGMACPciNetworkAdapter *) netdev_priv(netdev);	// use netdev_priv() @ 20100520
	if(adapter == NULL){
		TR0("OOPS adapter is null\n");
		return -1;
	}

	gmacdev = (synopGMACdevice *) adapter->synopGMACdev;
	if(gmacdev == NULL){
		TR0("OOPS gmacdev is null\n");
		return -1;
	}

	/*Disable all the interrupts*/
	synopGMAC_disable_interrupt_all(gmacdev);
	TR("the synopGMAC interrupt has been disabled\n");

	/*Disable the reception*/	
	synopGMAC_disable_dma_rx(gmacdev);
        synopGMAC_take_desc_ownership_rx(gmacdev);
	TR("the synopGMAC Reception has been disabled\n");

	/*Disable the transmission*/
	synopGMAC_disable_dma_tx(gmacdev);
        synopGMAC_take_desc_ownership_tx(gmacdev);

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI	
	napi_disable(&nvt_napi);	
#endif	
	
	TR("the synopGMAC Transmission has been disabled\n");
	netif_stop_queue(netdev);
	/*Now free the irq: This will detach the interrupt handler registered*/
	//free_irq(pcidev->irq, netdev);		// jay hsu
	
	// jay hsu : free GMAC irq	
#define GMAC_IRQ	36	
	free_irq(GMAC_IRQ, netdev);
	
	TR("the synopGMAC interrupt handler has been removed\n");
	
	/*Free the Rx Descriptor contents*/
	TR("Now calling synopGMAC_giveup_rx_desc_queue \n");
	//synopGMAC_giveup_rx_desc_queue(gmacdev, pcidev, RINGMODE);	// jay hsu
	synopGMAC_giveup_rx_desc_queue(gmacdev, NULL, RINGMODE);
	
//	synopGMAC_giveup_rx_desc_queue(gmacdev, pcidev, CHAINMODE);
	TR("Now calling synopGMAC_giveup_tx_desc_queue \n");
	//synopGMAC_giveup_tx_desc_queue(gmacdev, pcidev, RINGMODE);	// jay hsu
	synopGMAC_giveup_tx_desc_queue(gmacdev, NULL, RINGMODE);
	
//	synopGMAC_giveup_tx_desc_queue(gmacdev, pcidev, CHAINMODE);
	
	TR("Freeing the cable unplug timer\n");	
	del_timer(&synopGMAC_cable_unplug_timer);

	return -ESYNOPGMACNOERR;

//	TR("%s called \n",__FUNCTION__);
}

/**
 * Function to transmit a given packet on the wire.
 * Whenever Linux Kernel has a packet ready to be transmitted, this function is called.
 * The function prepares a packet and prepares the descriptor and 
 * enables/resumes the transmission.
 * @param[in] pointer to sk_buff structure. 
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and Error code on failure. 
 * \note structure sk_buff is used to hold packet in Linux networking stacks.
 */
s32 synopGMAC_linux_xmit_frames(struct sk_buff *skb, struct net_device *netdev)
{	
	s32 status = 0;
//	s32 counter =0;
	u32 offload_needed = 0;
	//u32 dma_addr;
	//u32 flags;
	synopGMACPciNetworkAdapter *adapter;
	synopGMACdevice * gmacdev;
	
	unsigned long irq_flags;		// jay hsu : add tx lock support
	
	//struct pci_dev * pcidev;	// jay hsu
	TR("%s called \n",__FUNCTION__);
	if(skb == NULL){
		TR0("skb is NULL What happened to Linux Kernel? \n ");
		return -1;
	}
	
	//adapter = (synopGMACPciNetworkAdapter *) netdev->priv;
	adapter = (synopGMACPciNetworkAdapter *) netdev_priv(netdev);	// use netdev_prvi() @ 20100520
	if(adapter == NULL)
		return -1;

	gmacdev = (synopGMACdevice *) adapter->synopGMACdev;
	if(gmacdev == NULL)
		return -1;

 	//pcidev  = (struct pci_dev *)adapter->synopGMACpcidev;			// jay hsu
	/*Stop the network queue*/	
	//netif_stop_queue(netdev); 	// jay debug : don't stop queue

	//printk("jay debug : skb->ip_summed 0x%x\n", skb->ip_summed);
	
		// jay hsu : 2.6.30 use UDP-lite, then skb->ip_summed will set by CHECKSUM_PARTIAL
		//if(skb->ip_summed == CHECKSUM_HW){
		if(skb->ip_summed == CHECKSUM_COMPLETE || skb->ip_summed == CHECKSUM_PARTIAL){
		/*	
		   In Linux networking, if kernel indicates skb->ip_summed = CHECKSUM_HW, then only checksum offloading should be performed
		   Make sure that the OS on which this code runs have proper support to enable offloading.
		*/
		offload_needed = 0x00000001;
		#if 0
		printk(KERN_CRIT"skb->ip_summed = CHECKSUM_HW\n");
		printk(KERN_CRIT"skb->h.th=%08x skb->h.th->check=%08x\n",(u32)(skb->h.th),(u32)(skb->h.th->check));
		printk(KERN_CRIT"skb->h.uh=%08x skb->h.uh->check=%08x\n",(u32)(skb->h.uh),(u32)(skb->h.uh->check));
		printk(KERN_CRIT"\n skb->len = %d skb->mac_len = %d skb->data = %08x skb->csum = %08x skb->h.raw = %08x\n",skb->len,skb->mac_len,(u32)(skb->data),skb->csum,(u32)(skb->h.raw));
		printk(KERN_CRIT"DST MAC addr:%02x %02x %02x %02x %02x %02x\n",*(skb->data+0),*(skb->data+1),*(skb->data+2),*(skb->data+3),*(skb->data+4),*(skb->data+5));
		printk(KERN_CRIT"SRC MAC addr:%02x %02x %02x %02x %02x %02x\n",*(skb->data+6),*(skb->data+7),*(skb->data+8),*(skb->data+9),*(skb->data+10),*(skb->data+11));
		printk(KERN_CRIT"Len/type    :%02x %02x\n",*(skb->data+12),*(skb->data+13));
		if(((*(skb->data+14)) & 0xF0) == 0x40){
			printk(KERN_CRIT"IPV4 Header:\n");
			printk(KERN_CRIT"%02x %02x %02x %02x\n",*(skb->data+14),*(skb->data+15),*(skb->data+16),*(skb->data+17));
			printk(KERN_CRIT"%02x %02x %02x %02x\n",*(skb->data+18),*(skb->data+19),*(skb->data+20),*(skb->data+21));
			printk(KERN_CRIT"%02x %02x %02x %02x\n",*(skb->data+22),*(skb->data+23),*(skb->data+24),*(skb->data+25));
			printk(KERN_CRIT"%02x %02x %02x %02x\n",*(skb->data+26),*(skb->data+27),*(skb->data+28),*(skb->data+29));
			printk(KERN_CRIT"%02x %02x %02x %02x\n\n",*(skb->data+30),*(skb->data+31),*(skb->data+32),*(skb->data+33));
			for(counter = 34; counter < skb->len; counter++)
				printk("%02X ",*(skb->data + counter));
		}
		else{
			printk(KERN_CRIT"IPV6 FRAME:\n");
			for(counter = 14; counter < skb->len; counter++)
				printk("%02X ",*(skb->data + counter));
		}
		#endif
		}


	// jay hsu : debug
	TR("##send frame## skb data 0x%x, len %d\n", (u32)skb->data, skb->len);
	TR("offload needed %d\n", offload_needed);
	
	/*Now we have skb ready and OS invoked this function. Lets make our DMA know about this*/
	//dma_addr = pci_map_single(pcidev,skb->data,skb->len,PCI_DMA_TODEVICE);		// jay hsu
	//status = synopGMAC_set_tx_qptr(gmacdev, dma_addr, skb->len, (u32)skb,0,0,0,offload_needed);		// jay hsu
	
	spin_lock_irqsave(&gmacdev->tx_lock, irq_flags);	// jay hsu @ 20100727 : start spin_lock protection
	//printk("jay debug :[TX] set skb 0x%08x\n", (u32)skb);
	status = synopGMAC_set_tx_qptr(gmacdev, virt_to_phys(skb->data), skb->len, (u32)skb,0,0,0,offload_needed);	
	spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);	// jay hsu @ 20100727 : end spin_lock protection
	
	if(status < 0){
		TR0("%s No More Free Tx Descriptors\n",__FUNCTION__);		
//		dev_kfree_skb (skb); //with this, system used to freeze.. ??
		netif_stop_queue(netdev);	// jay debug : if busy, we stop net queue
		// jay hsu : fix the errno @ 20100727
		return NETDEV_TX_BUSY;
	}
	
	/*Now force the DMA to start transmission*/	
	synopGMAC_resume_dma_tx(gmacdev);		
	//wbflush_ahb();		// jay hsu : flush read/write register commands
	
	netdev->trans_start = jiffies;
	
	/*Now start the netdev queue*/
	//netif_wake_queue(netdev);		// jay debug : don't stop queue
	
	return -ESYNOPGMACNOERR;
}

/**
 * Function provides the network interface statistics.
 * Function is registered to linux get_stats() function. This function is 
 * called whenever ifconfig (in Linux) asks for networkig statistics
 * (for example "ifconfig eth0").
 * @param[in] pointer to net_device structure. 
 * \return Returns pointer to net_device_stats structure.
 * \callgraph
 */
struct net_device_stats *  synopGMAC_linux_get_stats(struct net_device *netdev)
{
TR("%s called \n",__FUNCTION__);
//return( &(((synopGMACPciNetworkAdapter *)(netdev->priv))->synopGMACNetStats) );
return( &(((synopGMACPciNetworkAdapter *)(netdev_priv(netdev)))->synopGMACNetStats) );
}

/**
 * Function to set multicast and promiscous mode.
 * @param[in] pointer to net_device structure. 
 * \return returns void.
 */
void synopGMAC_linux_set_multicast_list(struct net_device *netdev)
{
TR("%s called \n",__FUNCTION__);
//todo Function not yet implemented.
return;
}

/**
 * Function to set ethernet address of the NIC.
 * @param[in] pointer to net_device structure. 
 * @param[in] pointer to an address structure. 
 * \return Returns 0 on success Errorcode on failure.
 */
s32 synopGMAC_linux_set_mac_address(struct net_device *netdev, void * macaddr)
{

synopGMACPciNetworkAdapter *adapter = NULL;
synopGMACdevice * gmacdev = NULL;
struct sockaddr *addr = macaddr;

//adapter = (synopGMACPciNetworkAdapter *) netdev->priv;
adapter = (synopGMACPciNetworkAdapter *) netdev_priv(netdev);
if(adapter == NULL)
	return -1;

gmacdev = adapter->synopGMACdev;
if(gmacdev == NULL)
	return -1;

if(!is_valid_ether_addr(addr->sa_data))
	return -EADDRNOTAVAIL;
	
	// Jay Hsu @ 20100628 : only set mac address when net device is down. 
	if ( netif_running(netdev) )
		return -EBUSY;

	// jay hsu : save current mac address in dram
	memcpy(Mac_Addr, addr->sa_data, sizeof(Mac_Addr));
	synopGMAC_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, addr->sa_data); 
	synopGMAC_get_mac_addr(synopGMACadapter->synopGMACdev,GmacAddr0High,GmacAddr0Low, netdev->dev_addr); 

TR("%s called \n",__FUNCTION__);
return 0;
}

/**
 * Function to change the Maximum Transfer Unit.
 * @param[in] pointer to net_device structure. 
 * @param[in] New value for maximum frame size.
 * \return Returns 0 on success Errorcode on failure.
 */
s32 synopGMAC_linux_change_mtu(struct net_device *netdev, s32 newmtu)
{
TR("%s called \n",__FUNCTION__);
//todo Function not yet implemented.
return 0;

}

/**
 * IOCTL interface.
 * This function is mainly for debugging purpose.
 * This provides hooks for Register read write, Retrieve descriptor status
 * and Retreiving Device structure information.
 * @param[in] pointer to net_device structure. 
 * @param[in] pointer to ifreq structure.
 * @param[in] ioctl command. 
 * \return Returns 0 on success Error code on failure.
 */
s32 synopGMAC_linux_do_ioctl(struct net_device *netdev, struct ifreq *ifr, s32 cmd)
{
s32 retval = 0;
u16 temp_data = 0;
synopGMACPciNetworkAdapter *adapter = NULL;
synopGMACdevice * gmacdev = NULL;
struct ifr_data_struct
{
	u32 unit;
	u32 addr;
	u32 data;
} *req;

struct mii_ioctl_data *mii_data;	// Jay Hsu @ 20100628 : for mii ioctl command
if(netdev == NULL)
	return -1;
if(ifr == NULL)
	return -1;

req = (struct ifr_data_struct *)ifr->ifr_data;

//adapter = (synopGMACPciNetworkAdapter *) netdev->priv;
adapter = (synopGMACPciNetworkAdapter *) netdev_priv(netdev);
if(adapter == NULL)
	return -1;

gmacdev = adapter->synopGMACdev;
if(gmacdev == NULL)
	return -1;
//TR("%s :: on device %s req->unit = %08x req->addr = %08x req->data = %08x cmd = %08x \n",__FUNCTION__,netdev->name,req->unit,req->addr,req->data,cmd);
// jay hsu:
//printk("%s :: on device %s req->unit = %08x req->addr = %08x req->data = %08x cmd = %08x \n",__FUNCTION__,netdev->name,req->unit,req->addr,req->data,cmd);

	// Jay Hsu @ 20100628 : check if the net device is up.
	if ( !netif_running(netdev) )
		return -ENODEV;
switch(cmd)
{
	case IOCTL_READ_REGISTER:		//IOCTL for reading IP registers : Read Registers
		if      (req->unit == 0)	// Read Mac Register
			req->data = synopGMACReadReg((u32 *)gmacdev->MacBase,req->addr);
		else if (req->unit == 1)	// Read DMA Register
			req->data = synopGMACReadReg((u32 *)gmacdev->DmaBase,req->addr);
		else if (req->unit == 2){	// Read Phy Register
			retval = synopGMAC_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,req->addr,&temp_data);
			req->data = (u32)temp_data;
			if(retval != -ESYNOPGMACNOERR)
			{
				TR("ERROR in Phy read\n");	
			}
		}
		break;

	case IOCTL_WRITE_REGISTER:		//IOCTL for reading IP registers : Read Registers
		if      (req->unit == 0)	// Write Mac Register
			synopGMACWriteReg((u32 *)gmacdev->MacBase,req->addr,req->data);
		else if (req->unit == 1)	// Write DMA Register
		{
			printk("GMAC driver : addr 0x%x, data 0x%x\n", req->addr, req->data);
			synopGMACWriteReg((u32 *)gmacdev->DmaBase,req->addr,req->data);
		}
		else if (req->unit == 2){	// Write Phy Register
			retval = synopGMAC_write_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,req->addr,(u16)(req->data));
			if(retval != -ESYNOPGMACNOERR)
			{
				TR("ERROR in Phy read\n");	
			}
		}
		//wbflush_ahb();		// jay hsu : flush read/write register commands				
		break;

	case IOCTL_READ_IPSTRUCT:		//IOCTL for reading GMAC DEVICE IP private structure
	        memcpy(ifr->ifr_data, gmacdev, sizeof(synopGMACdevice));
		break;

	case IOCTL_READ_RXDESC:			//IOCTL for Reading Rx DMA DESCRIPTOR
		memcpy(ifr->ifr_data, gmacdev->RxDesc + ((DmaDesc *) (ifr->ifr_data))->data1, sizeof(DmaDesc) );
		break;

	case IOCTL_READ_TXDESC:			//IOCTL for Reading Tx DMA DESCRIPTOR
		memcpy(ifr->ifr_data, gmacdev->TxDesc + ((DmaDesc *) (ifr->ifr_data))->data1, sizeof(DmaDesc) );
		break;
	case IOCTL_POWER_DOWN:
		if	(req->unit == 1){	//power down the mac
			TR("============I will Power down the MAC now =============\n");
			printk("============I will Power down the MAC now =============\n");
			// If it is already in power down don't power down again
			retval = 0;
			if(((synopGMACReadReg((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus)) & GmacPmtPowerDown) != GmacPmtPowerDown){
			synopGMAC_linux_powerdown_mac(gmacdev);			
			retval = 0;
			}
		}
		if	(req->unit == 2){	//Disable the power down  and wake up the Mac locally
			TR("============I will Power up the MAC now =============\n");
			printk("============I will Power up the MAC now =============\n");
			//If already powered down then only try to wake up
			retval = -1;
			if(((synopGMACReadReg((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus)) & GmacPmtPowerDown) == GmacPmtPowerDown){
			synopGMAC_power_down_disable(gmacdev);
			synopGMAC_linux_powerup_mac(gmacdev);
			retval = 0;
			}
		}
		break;
		
	case IOCTL_CHK_CONN:
		printk("link status %d\n", gmacdev->LinkState);
		memcpy(ifr->ifr_data, &gmacdev->LinkState, sizeof(gmacdev->LinkState));
		break;
	case IOCTL_PHY_LOOPBACK:
		if(req == NULL){
			printk("req is NULL\n");
			break;
		}
		
		if      (req->unit == 0){
			//printk("req->unit == 0\n");
			synopGMAC_phy_loopback(gmacdev,0);
}
		else{
			//printk("req->unit == 1\n");
			synopGMAC_phy_loopback(gmacdev,1);
}
		break;
		
	case SIOCGMIIPHY:
		mii_data = if_mii(ifr);
		mii_data->phy_id = gmacdev->PhyBase & 0x1f;
		//printk("jay debug : SIOCGMIIPHY phy_id 0x%x\n", mii_data->phy_id);
		break;
		
	case SIOCGMIIREG:
		mii_data = if_mii(ifr);
		synopGMAC_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,mii_data->reg_num,&mii_data->val_out);
		//printk("jay debug : SIOCGMIIREG reg 0x%x, val_out 0x%x\n", mii_data->reg_num, mii_data->val_out);
		break;
		
	case SIOCSMIIREG:
		if (!capable(CAP_NET_ADMIN))
     		return -EPERM;	
		mii_data = if_mii(ifr);
		synopGMAC_write_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,mii_data->reg_num,mii_data->val_in);
		//printk("jay debug : SIOCSMIIREG reg 0x%x, val_in 0x%x\n", mii_data->reg_num, mii_data->val_in);
		break;
	default:
		retval = -1;

}


return retval;
}

/**
 * Function to handle a Tx Hang.
 * This is a software hook (Linux) to handle transmitter hang if any.
 * We get transmitter hang in the device interrupt status, and is handled
 * in ISR. This function is here as a place holder.
 * @param[in] pointer to net_device structure 
 * \return void.
 */
void synopGMAC_linux_tx_timeout(struct net_device *netdev)
{
TR("%s called \n",__FUNCTION__);
printk("jay debug : %s called \n",__FUNCTION__);
//todo Function not yet implemented
return;
}

// jay hsu : polling function
#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
int synopGMAC_linux_poll(struct napi_struct *napi, int budget)
{	
	synopGMACPciNetworkAdapter *adapter;
	synopGMACdevice * gmacdev;
	struct net_device *netdev = napi->dev;
	//struct pci_dev *pcidev;	// jay hsu
	s32 desc_index;
	
	u32 data1;
	u32 data2;
	u32 len;
	u32 status;
	u32 dma_addr1;
	u32 dma_addr2;
#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif
	//u32 length;
	u32 polling_packets = 0;
	
	struct sk_buff *skb; //This is the pointer to hold the received data
	
	TR("%s\n",__FUNCTION__);	
	//printk("jay debug : enter synopGMAC_linux_poll, weight %d\n", budget);	// jay hsu
	
	//adapter = netdev->priv;
	adapter = netdev_priv(netdev);
	if(adapter == NULL){
		TR("Unknown Device\n");
		return 1;
	}
	
	gmacdev = adapter->synopGMACdev;
	if(gmacdev == NULL){
		TR("GMAC device structure is missing\n");
		return 1;
	}

 	//pcidev  = (struct pci_dev *)adapter->synopGMACpcidev;			// jay hsu
	/*Handle the Receive Descriptors*/
	do{
#ifdef ENH_DESC_8W
	desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,&dma_addr1,NULL, &data1,&dma_addr2,NULL,&data2,&ext_status,&time_stamp_high,&time_stamp_low);
	if(desc_index >0){ 
        synopGMAC_TS_read_timestamp_higher_val(gmacdev, &time_stamp_higher);
	TR("S:%08x ES:%08x DA1:%08x d1:%08x DA2:%08x d2:%08x TSH:%08x TSL:%08x TSHW:%08x \n",status,ext_status,dma_addr1, data1,dma_addr2,data2, time_stamp_high,time_stamp_low,time_stamp_higher);
	}
#else
	desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,&dma_addr1,NULL, &data1,&dma_addr2,NULL,&data2);
	// jay hsu : debug
	TR("##Handle Received## desc_index %d, dma_addr1 0x%x, data1 0x%x\n", desc_index, (u32)dma_addr1, data1);
#endif
	//desc_index = synopGMAC_get_rx_qptr(gmacdev, &status,&dma_addr,NULL, &data1);

		if(desc_index >= 0 && data1 != 0){
			TR("Received Data at Rx Descriptor %d for skb 0x%08x whose status is %08x\n",desc_index,data1,status);
			/*At first step unmapped the dma address*/
			//pci_unmap_single(pcidev,dma_addr1,0,PCI_DMA_FROMDEVICE);		// jay hsu

			skb = (struct sk_buff *)data1;
			if(synopGMAC_is_rx_desc_valid(status)){
				len =  synopGMAC_get_rx_desc_frame_length(status) - 4; //Not interested in Ethernet CRC bytes
				skb_put(skb,len);
			#ifdef IPC_OFFLOAD
				// Now lets check for the IPC offloading
				/*  Since we have enabled the checksum offloading in hardware, lets inform the kernel
				    not to perform the checksum computation on the incoming packet. Note that ip header 
  				    checksum will be computed by the kernel immaterial of what we inform. Similary TCP/UDP/ICMP
				    pseudo header checksum will be computed by the stack. What we can inform is not to perform
				    payload checksum. 		
   				    When CHECKSUM_UNNECESSARY is set kernel bypasses the checksum computation.		    
				*/
	
				TR("Checksum Offloading will be done now\n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;
				
				#ifdef ENH_DESC_8W
				if(synopGMAC_is_ext_status(gmacdev, status)){ // extended status present indicates that the RDES4 need to be probed
					TR("Extended Status present\n");
					if(synopGMAC_ES_is_IP_header_error(gmacdev,ext_status)){       // IP header (IPV4) checksum error
					//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
					TR("(EXTSTS)Error in IP header error\n");
					skb->ip_summed = CHECKSUM_NONE;     //Let Kernel compute the checkssum
					}	
					if(synopGMAC_ES_is_rx_checksum_bypassed(gmacdev,ext_status)){   // Hardware engine bypassed the checksum computation/checking
					TR("(EXTSTS)Hardware bypassed checksum computation\n");	
					skb->ip_summed = CHECKSUM_NONE;             // Let Kernel compute the checksum
					}
					if(synopGMAC_ES_is_IP_payload_error(gmacdev,ext_status)){       // IP payload checksum is in error (UDP/TCP/ICMP checksum error)
					TR("(EXTSTS) Error in EP payload\n");	
					skb->ip_summed = CHECKSUM_NONE;             // Let Kernel compute the checksum
					}				
				}
				else{ // No extended status. So relevant information is available in the status itself
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxNoChkError ){
					TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 4>  \n");
					skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass computing the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
					//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
					TR(" Error in 16bit IPV4 Header Checksum <Chk Status = 6>  \n");
					skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass the TCP/UDP checksum computation
					}				
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxLenLT600 ){
					TR("IEEE 802.3 type frame with Length field Lesss than 0x0600 <Chk Status = 0> \n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrPayLoadChkBypass ){
					TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 1>\n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxChkBypass ){
					TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 3>  \n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxPayLoadChkError ){
					TR(" TCP/UDP payload checksum Error <Chk Status = 5>  \n");
					skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
					}
					if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
					//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
					TR(" Both IP header and Payload Checksum Error <Chk Status = 7>  \n");
					skb->ip_summed = CHECKSUM_NONE;	        //Let Kernel compute the Checksum
					}
				}
				#else	
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxNoChkError ){
				TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 4>  \n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass computing the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
				//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
				TR(" Error in 16bit IPV4 Header Checksum <Chk Status = 6>  \n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;	//Let Kernel bypass the TCP/UDP checksum computation
				}				
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxLenLT600 ){
				TR("IEEE 802.3 type frame with Length field Lesss than 0x0600 <Chk Status = 0> \n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrPayLoadChkBypass ){
				TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 1>\n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxChkBypass ){
				TR("Ip header and TCP/UDP payload checksum Bypassed <Chk Status = 3>  \n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxPayLoadChkError ){
				TR(" TCP/UDP payload checksum Error <Chk Status = 5>  \n");
				skb->ip_summed = CHECKSUM_NONE;	//Let Kernel compute the Checksum
				}
				if(synopGMAC_is_rx_checksum_error(gmacdev, status) == RxIpHdrChkError ){
				//Linux Kernel doesnot care for ipv4 header checksum. So we will simply proceed by printing a warning ....
				TR(" Both IP header and Payload Checksum Error <Chk Status = 7>  \n");
				skb->ip_summed = CHECKSUM_NONE;	        //Let Kernel compute the Checksum
				}
				
				#endif
			#endif //IPC_OFFLOAD	
				skb->dev = netdev;
				skb->protocol = eth_type_trans(skb, netdev);
				//netif_rx(skb);
				netif_receive_skb(skb);				

				netdev->last_rx = jiffies;
				adapter->synopGMACNetStats.rx_packets++;
				adapter->synopGMACNetStats.rx_bytes += len;		
				
				polling_packets++;		// jay hsu : increase packet counters
			}
			else{
				/*Now the present skb should be set free*/
				dev_kfree_skb_any(skb);
				printk(KERN_CRIT "status : %08x\n",status);
				adapter->synopGMACNetStats.rx_errors++;
				adapter->synopGMACNetStats.collisions       += synopGMAC_is_rx_frame_collision(status);
				adapter->synopGMACNetStats.rx_crc_errors    += synopGMAC_is_rx_crc(status);
				adapter->synopGMACNetStats.rx_frame_errors  += synopGMAC_is_frame_dribbling_errors(status);
				adapter->synopGMACNetStats.rx_length_errors += synopGMAC_is_rx_frame_length_errors(status);
			}
			
			//Now lets allocate the skb for the emptied descriptor
			skb = dev_alloc_skb(netdev->mtu + ETHERNET_PACKET_EXTRA);
			if(skb == NULL){
				TR("SKB memory allocation failed \n");
				adapter->synopGMACNetStats.rx_dropped++;
			}
						
			//dma_addr1 = pci_map_single(pcidev,skb->data,skb_tailroom(skb),PCI_DMA_FROMDEVICE);	// jay hsu
			
			//desc_index = synopGMAC_set_rx_qptr(gmacdev,dma_addr1, skb_tailroom(skb), (u32)skb,0,0,0);	// jay hsu
			desc_index = synopGMAC_set_rx_qptr(gmacdev,virt_to_phys(skb->data), skb_tailroom(skb), (u32)skb,0,0,0);
			if(desc_index < 0){
				TR("Cannot set Rx Descriptor for skb %08x\n",(u32)skb);
				printk("Cannot set Rx Descriptor for skb %08x\n",(u32)skb);	// jay hsu : debug test it
				dev_kfree_skb_any(skb);
			}
		}
	}while(desc_index >= 0 && ( polling_packets < (budget-1) ));
	

	// exit napi mode
	//__napi_complete(napi);
	napi_complete(napi);	
	atomic_set(&napi_poll, 0);
	//printk(" jay debug : finished polling function, napi_poll %d\n", atomic_read(&napi_poll));		// jay hsu
	synopGMAC_enable_interrupt(gmacdev, synopGMAC_get_interrupt_mask(gmacdev)|DmaIntRxNormMask);

		
	return polling_packets;
}
#endif

// jay hsu : add netdev_ops
static const struct net_device_ops synopGMAC_Netdev_Ops = {
	.ndo_open				= synopGMAC_linux_open,
	.ndo_stop				= synopGMAC_linux_close,
	.ndo_start_xmit			= synopGMAC_linux_xmit_frames,
	.ndo_get_stats			= synopGMAC_linux_get_stats,
	.ndo_set_multicast_list	= synopGMAC_linux_set_multicast_list,
	.ndo_set_mac_address	= synopGMAC_linux_set_mac_address,
	.ndo_change_mtu			= synopGMAC_linux_change_mtu,
	.ndo_do_ioctl			= synopGMAC_linux_do_ioctl,
	.ndo_tx_timeout			= synopGMAC_linux_tx_timeout,
//#ifdef CONFIG_NET_POLL_CONTROLLER
//	.ndo_poll_controller	= e100_netpoll,
//#endif
};

/**
 * Function to initialize the Linux network interface.
 * 
 * Linux dependent Network interface is setup here. This provides 
 * an example to handle the network dependent functionality.
 *
 * \return Returns 0 on success and Error code on failure.
 */
s32 __init synopGMAC_init_network_interface(void)
{
	s32 err;
	struct net_device *netdev;

	TR("Now Going to Call register_netdev to register the network interface for GMAC core\n");
	/*
	Lets allocate and set up an ethernet device, it takes the sizeof the private structure. This is mandatory as a 32 byte 
	allignment is required for the private data structure.
	*/
	netdev = alloc_etherdev(sizeof(synopGMACPciNetworkAdapter));
	if(!netdev){
	err = -ESYNOPGMACNOMEM;
	goto err_alloc_etherdev;
	}
	
	//synopGMACadapter = netdev->priv;
	synopGMACadapter = netdev_priv(netdev);
	synopGMACadapter->synopGMACnetdev = netdev;
	//synopGMACadapter->synopGMACpcidev = synopGMACpcidev;		// jay hsu
	synopGMACadapter->synopGMACdev    = NULL;
	
	/*Allocate Memory for the the GMACip structure*/
	synopGMACadapter->synopGMACdev = (synopGMACdevice *) plat_alloc_memory(sizeof (synopGMACdevice));
	if(!synopGMACadapter->synopGMACdev){
	TR0("Error in Memory Allocataion #1\n");
	}
	
//	// jay hsu : malloc Tx buffer
//	synopGMACadapter->synopGMACdev->TxBuffer = plat_alloc_memory(2000);	// large than 1518 ethernet frame
//	if (!synopGMACadapter->synopGMACdev->TxBuffer)
//	{
//		TR0("Error in Memory Allocataion #2\n");
//	}

	TR("## memory allocation at 0x%x\n", (u32)synopGMACadapter->synopGMACdev);
	TR("## GMac Base Addr at 0x%x\n", (u32)synopGMACMappedAddr);
	
	// jay hsu : initialize the ethernet phy
	SysPlat_Init_Ethernet_Phy();	

	/*Attach the device to MAC struct This will configure all the required base addresses
	  such as Mac base, configuration base, phy base address(out of 32 possible phys )*/
	synopGMAC_attach(synopGMACadapter->synopGMACdev,(u32) synopGMACMappedAddr + MACBASE,(u32) synopGMACMappedAddr + DMABASE, DEFAULT_PHY_BASE);
	
	// Jay Hsu : to initialize the mac address
	SysPlat_Init_Mac_Addr();
	
	synopGMAC_reset(synopGMACadapter->synopGMACdev);

#if 0	// jay hsu
	if(synop_pci_using_dac){
	TR("netdev->features = %08x\n",netdev->features);
	TR("synop_pci_using dac is %08x\n",synop_pci_using_dac);
	netdev->features |= NETIF_F_HIGHDMA;
	TR("netdev->features = %08x\n",netdev->features);
	}
#endif
	
//	netdev->open = &synopGMAC_linux_open; 
//	netdev->stop = &synopGMAC_linux_close;
//	netdev->hard_start_xmit = &synopGMAC_linux_xmit_frames;
//	netdev->get_stats = &synopGMAC_linux_get_stats;
//	netdev->set_multicast_list = &synopGMAC_linux_set_multicast_list;
//	netdev -> set_mac_address = &synopGMAC_linux_set_mac_address;
//	netdev -> change_mtu = &synopGMAC_linux_change_mtu;
//	netdev -> do_ioctl = &synopGMAC_linux_do_ioctl;
//	netdev -> tx_timeout = &synopGMAC_linux_tx_timeout; 
	//SET_ETHTOOL_OPS(netdev, &e100_ethtool_ops);
	
	// jay hsu : refer e100.c net driver
	netdev->netdev_ops = &synopGMAC_Netdev_Ops;
	netdev->watchdog_timeo = 5 * HZ;
	
	// jay hsu @ 20100727 : init tx spin_lock
	spin_lock_init(&synopGMACadapter->synopGMACdev->tx_lock);
 
	// jay hsu : set MDC clock by ahb auto-detection function
	//if ( KER_CLK_GetClockRate(EN_KER_CLK_AHB_BUS) > 100 )
	// jay hsu : no ntclk.ko support. Add later. @ 20100520
	if ( (hclk/1000/1000) >= 90 )
	{
		printk("[NTKETHMAC] : Set MDC clock 100~150MHz\n");
		s32MDC_CLOCK = GmiiCsrClk1;
	}
	else
	{
		printk("[NTKETHMAC] : Set MDC clock 60~100MHz\n");
		s32MDC_CLOCK = GmiiCsrClk0;
	}
	
	/*Now start the network interface*/
	printk("[NTKETHMAC] : Now Registering the netdevice\n");
	TR("Now Registering the netdevice\n");
	if((err = register_netdev(netdev)) != 0) {
		TR0("Error in Registering netdevice\n");
		return err;
	}  
	
#ifdef CONFIG_NVT_FASTETH_MAC_NAPI	
	printk("[NTKETHMAC] : Init with NAPI support\n");
	netif_napi_add(netdev, &nvt_napi, synopGMAC_linux_poll, NVT_NAPI_WEIGHT);
	
//	printk("[NTKETHMAC] : USE NAPI = %d\n", use_napi);
//	if ( use_napi )
//	 {
//	 	printk("USE NAPI mechinism...\n");
//	 	// jay hsu : need implement napi
//	 	//netdev->poll = 	&synopGMAC_linux_poll;
//	 	//netdev->weight = 64;	// for 100Mbs
//	 }
#endif	
	
 	return 0;
err_alloc_etherdev:
	TR0("Problem in alloc_etherdev()..Take Necessary action\n");
	return err;
}


/**
 * Function to initialize the Linux network interface.
 * Linux dependent Network interface is setup here. This provides 
 * an example to handle the network dependent functionality.
 * \return Returns 0 on success and Error code on failure.
 */
void __exit synopGMAC_exit_network_interface(void)
{
	TR0("Now Calling network_unregister\n");
	unregister_netdev(synopGMACadapter->synopGMACnetdev);	
}

void SysPlat_Init_Ethernet_Phy(void)
{
	unsigned long flags;

	// jay hsu : set Mac mpll speed to 50MHz
	/* MPLL setting */
	SYS_CLK_SetMpll(0xaa, 0x085555);

	/* Clock output  */
	*((volatile u32 *)0xbd0f0020) |= 0x00000080;

	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	/* MAC clock enable */
	*((volatile u32 *)0xbd020044) |= 0x00008000;

	/* MAC clock inverse */
	*((volatile u32 *)0xbd020044) |= 0x00000040;

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);

	/* Mac default enable */
	/* GPIO default enable */

	// jay hsu : ethernet phy reset
	*((volatile u32 *)(NVT_PHY_RESET_GPIO_ADDR + 0x8)) |= (1UL << NVT_PHY_RESET_GPIO_BIT);
	*((volatile u32 *)(NVT_PHY_RESET_GPIO_ADDR + 0x0)) = (1UL << NVT_PHY_RESET_GPIO_BIT);

	mdelay(1);		// jay hsu : delay 1 msec at least to let phy latch the strap pins

	*((volatile u32 *)(NVT_PHY_RESET_GPIO_ADDR + 0x4)) = (1UL << NVT_PHY_RESET_GPIO_BIT);
}

// Jay Hsu : Finally we need to initialize the value of mac address from flash or eeprom.
void SysPlat_Init_Mac_Addr(void)
{
#define DEFAULT_MAC_ADDRESS {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

	u8 default_mac_addr[6] = DEFAULT_MAC_ADDRESS;

	memcpy(Mac_Addr, default_mac_addr, sizeof(Mac_Addr));
	
	return;
}

/*
module_init(synopGMAC_init_network_interface);
module_exit(synopGMAC_exit_network_interface);

MODULE_AUTHOR("Synopsys India");
MODULE_LICENSE("GPL/BSD");
MODULE_DESCRIPTION("SYNOPSYS GMAC DRIVER Network INTERFACE");

EXPORT_SYMBOL(synopGMAC_init_pci_bus_interface);
*/
