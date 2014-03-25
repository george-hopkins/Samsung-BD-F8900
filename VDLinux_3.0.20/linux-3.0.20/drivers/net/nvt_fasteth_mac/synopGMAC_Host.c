/**\file
 *  The top most file which makes use of synopsys GMAC driver code.
 *
 *  This file can be treated as the example code for writing a application driver
 *  for synopsys GMAC device using the driver provided by Synopsys.
 *  This exmple is for Linux 2.6.xx kernel 
 *  - Uses 32 bit 33MHz PCI Interface as the host bus interface
 *  - Uses Linux network driver and the TCP/IP stack framework
 *  - Uses the Device Specific Synopsys GMAC Kernel APIs
 *  \internal
 * ---------------------------REVISION HISTORY--------------------------------
 * Synopsys 			01/Aug/2007			Created
 */
 
//#include <linux/config.h>
#include <linux/version.h>   /* LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 33))
#include <generated/autoconf.h>
#else
#include <linux/autoconf.h>
#endif
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>

#include <linux/pci.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include <linux/proc_fs.h>		// jay hsu : add proc fs support
static struct proc_dir_entry *synopGMAC_info;

#include "synopGMAC_Host.h"
#include "synopGMAC_banner.h"
#include "synopGMAC_plat.h"
//#include "synopGMAC_pci_bus_interface.h"		// jay hsu
#include "synopGMAC_network_interface.h"
#include "synopGMAC_Dev.h"

/****************************************************/


/* Global declarations: these are required to handle 
   Os and Platform dependent functionalities        */

/*GMAC IP Base address and Size   */
u8 *synopGMACMappedAddr;
u32 synopGMACMappedAddrSize = 0;

/*global adapter gmacdev pcidev and netdev pointers */
synopGMACPciNetworkAdapter *synopGMACadapter;
//synopGMACdevice		   *synopGMACdev;

//struct pci_dev             *synopGMACpcidev;		// jay hsu

struct net_device          *synopGMACnetdev;

static int proc_read_synopGMAC_info(char *page, char **start, off_t off, int count, int *eof, void *data);
static int proc_write_synopGMAC_info(struct file *file, const char *buffer, unsigned long count, void *data);
static int __init SynopGMAC_Host_Interface_init(void);
static void __exit SynopGMAC_Host_Interface_exit(void);

/***************************************************/

int proc_read_synopGMAC_info(char *page, char **start, off_t off, int count, int *eof, void *data)
{
//	printk("MacBase 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->MacBase);
//	printk("DmaBase 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->DmaBase);
//	printk("RxDesc 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->RxDesc);
//	printk("RxDescDma 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->RxDescDma);
//	printk("RxBusyDesc 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->RxBusyDesc);
//	printk("TxDesc 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->TxDesc);
//	printk("TxDescDma 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->TxDescDma);
//	printk("TxBusyDesc 0x%08x\n", (u32)synopGMACadapter->synopGMACdev->TxBusyDesc);

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI	
	printk("[NTKETHMAC] : Proc info => USE NAPI\n");
	//printk("USE NAPI : %s\n", (use_napi==1)?"enable":"disable");
#else
	printk("[NTKETHMAC] : Proc info => NO NAPI\n");
#endif
	
	//printk("[NTKETHMAC] : Ethernet Driver Version %s, remove the phy reset\n", "20110214");
	printk("[NTKETHMAC] : Ethernet Driver Version %s For NT72558\n", "20110726");
	if ( synopGMACadapter->synopGMACdev->Speed == 0 )
		printk("[NTKETHMAC] : Proc info => Link Status : down\n");
	else
	{
		printk("[NTKETHMAC] : Proc info => Link Status : up\n");
		printk("[NTKETHMAC] : Proc info => Speed : %s %s\n", (synopGMACadapter->synopGMACdev->Speed==SPEED100)?"100Mbps":"10Mbps",
					(synopGMACadapter->synopGMACdev->DuplexMode==FULLDUPLEX)?"Full-Duplex":"Half-Duplex");
	}
		
	return 0;
}

int proc_write_synopGMAC_info(struct file *file, const char *buffer, unsigned long count, void *data)
{
	return 0;
}

int __init SynopGMAC_Host_Interface_init(void)
{

        int retval;

	TR0("**********************************************************\n");
	TR0("* Driver    :%s\n",synopGMAC_driver_string);
	TR0("* Version   :%s\n",synopGMAC_driver_version);
	TR0("* Copyright :%s\n",synopGMAC_copyright);
	TR0("**********************************************************\n");

        TR0("Initializing synopsys GMAC interfaces ..\n") ;

	// jay hsu : set reg base address
	synopGMACMappedAddr = (u8 *)SynopGMACBaseAddr;
	
#if 0	// jay hsu : use ahb/axi bus interface
        /* Initialize the bus interface for the hostcontroller E.g PCI in our case */
        if ((retval = synopGMAC_init_pci_bus_interface())) {
	        TR0("Could not initiliase the bus interface. Is PCI device connected ?\n");
                return retval;
        }
#endif
		
	/*Now we have got pdev structure from pci interface. Lets populate it in our global data structure*/	

        /* Initialize the Network dependent services */
	
        if((retval = synopGMAC_init_network_interface())){
		TR0("Could not initialize the Network interface.\n");
		return retval;
	}
	
	// jay hsu : add proc fs support
	synopGMAC_info = create_proc_entry("Mac_Info", 0644, NULL);
	if ( synopGMAC_info == NULL )
		return -ENOMEM;
	//synopGMAC_info->owner = THIS_MODULE;		// jay hsu : remove it at linux kernel 2.6.30 @ 20100520
	synopGMAC_info->read_proc = proc_read_synopGMAC_info;
	synopGMAC_info->write_proc = proc_write_synopGMAC_info;
	
      return 0 ;
}

void __exit SynopGMAC_Host_Interface_exit(void)
{

        TR0("Exiting synopsys GMAC interfaces ..\n") ;

        /* De-Initialize the Network dependent services */
        synopGMAC_exit_network_interface();
	TR0("Exiting synopGMAC_exit_network_interface\n");

#if 0	// jay hsu : use ahb/axi bus interface
        /* Initialize the bus interface for the hostcontroller E.g PCI in our case */
	synopGMAC_exit_pci_bus_interface();
        TR0("Exiting synpGMAC_exit_pci_bus_interface\n");
#endif

	// jay hsu : add proc fs support
	remove_proc_entry("Mac_Info", NULL);
        
}

module_init(SynopGMAC_Host_Interface_init);
module_exit(SynopGMAC_Host_Interface_exit);

//module_param(use_napi, int, 0);
MODULE_AUTHOR("Synopsys India");
MODULE_LICENSE("GPL/BSD");
MODULE_DESCRIPTION("SYNOPSYS GMAC NETWORK DRIVER WITH PCI INTERFACE");

