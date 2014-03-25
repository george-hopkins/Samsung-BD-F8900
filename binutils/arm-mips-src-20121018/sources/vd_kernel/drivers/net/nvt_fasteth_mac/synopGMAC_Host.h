#ifndef SYNOP_GMAC_HOST_H
#define SYNOP_GMAC_HOST_H


#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include "synopGMAC_plat.h"
//#include "synopGMAC_pci_bus_interface.h"		// jay hsu
#include "synopGMAC_Dev.h"

typedef struct synopGMACAdapterStruct{

/*Device Dependent Data structur*/
synopGMACdevice * synopGMACdev;
/*Os/Platform Dependent Data Structures*/
//struct pci_dev * synopGMACpcidev;		// jay hsu
struct net_device *synopGMACnetdev;
struct net_device_stats synopGMACNetStats;
//u32 synopGMACPciState[16];			// jay hsu

} synopGMACPciNetworkAdapter;


#endif
