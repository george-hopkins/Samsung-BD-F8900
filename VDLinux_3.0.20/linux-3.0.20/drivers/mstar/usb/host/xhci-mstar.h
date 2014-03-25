/*
 * xHCI host controller driver
 *
 * Copyright (C) 2012 MStar Inc.
 *
 * Date: May 2012
 */

#ifndef _XHCI_MSTAR_H
#define _XHCI_MSTAR_H

#include "ehci-mstar.h"

// ----- Don't modify it !----------
#if defined(CONFIG_ARM) 
#define XHCI_PA_PATCH   1
#endif
#define XHCI_FLUSHPIPE_PATCH  1
#define XHCI_CHIRP_PATCH  1
//------------------------------

#define ENABLE_XHCI_SSC   1

//------ for test -----------------
#define XHCI_SS_CS_PATCH 0   
#define XHCI_ENABLE_DEQ  0
//--------------------------------


#define _MSTAR_U3PHY_DTOP_BASE (_MSTAR_USB_BASEADR+(0x22100*2))
#define _MSTAR_U3PHY_ATOP_BASE (_MSTAR_USB_BASEADR+(0x22200*2))
#define _MSTAR_U3UTMI_BASE     (_MSTAR_USB_BASEADR+(0x22300*2))
#define _MSTAR_U3INDCTL_BASE   (_MSTAR_USB_BASEADR+(0x22400*2))
#define _MSTAR_U3TOP_BASE      (_MSTAR_USB_BASEADR+(0x22500*2))
#define _MSTAR_U3BC_BASE       (_MSTAR_USB_BASEADR+(0x23600*2))
#define _MSTAR_XHCI_BASE       (_MSTAR_USB_BASEADR+(0x90000*2))


#endif	/* _XHCI_MSTAR_H */

