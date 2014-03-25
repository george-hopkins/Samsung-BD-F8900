#ifndef _LINUX_ERRNO_H
#define _LINUX_ERRNO_H

#include <asm/errno.h>

#ifdef __KERNEL__

/*
 * These should never be seen by user programs.  To return one of ERESTART*
 * codes, signal_pending() MUST be set.  Note that ptrace can observe these
 * at syscall exit tracing, but they will never be left for the debugged user
 * process to see.
 */
#define ERESTARTSYS	512
#define ERESTARTNOINTR	513
#define ERESTARTNOHAND	514	/* restart if no handler.. */
#define ENOIOCTLCMD	515	/* No ioctl command */
#define ERESTART_RESTARTBLOCK 516 /* restart by calling sys_restart_syscall */

/* Defined for the NFSv3 protocol */
#define EBADHANDLE	521	/* Illegal NFS file handle */
#define ENOTSYNC	522	/* Update synchronization mismatch */
#define EBADCOOKIE	523	/* Cookie is stale */
#define ENOTSUPP	524	/* Operation is not supported */
#define ETOOSMALL	525	/* Buffer or request is too small */
#define ESERVERFAULT	526	/* An untranslatable error occurred */
#define EBADTYPE	527	/* Type not supported by server */
#define EJUKEBOX	528	/* Request initiated, but will not complete before timeout */
#define EIOCBQUEUED	529	/* iocb queued, will get completion event */
#define EIOCBRETRY	530	/* iocb queued, will trigger a retry */

/* 
 * SAMSUNG USB PATCH, by ksfree.kim
 */
#define SAMSUNG_PATCH_WITH_USB_HOTPLUG   	// patch for usb hotplug 
#define SAMSUNG_PATCH_WITH_USB_HOTPLUG_MREADER  // patch for usb multicard reader
#define SAMSUNG_PATCH_WITH_USB_ENHANCEMENT   	// stable patch for enhanced speed  and compatibility
#define SAMSUNG_PATCH_WITH_USB_XHCI_BUGFIX   	// BugFIX patch for xHCI
#define SAMSUNG_PATCH_WITH_USB_XHCI_ADD_DYNAMIC_RING_BUFFER   	// Add Dynamic RingBuffer patch for xHCI, This patch has dependance for "SAMSUNG_PATCH_WITH_USB_XHCI_BUGFIX"
#define SAMSUNG_PATCH_WITH_USB_XHCI_CODE_CLEANUP   	// Add code cleanup and debug patch for xHCI, This patch has dependance for "SAMSUNG_PATCH_WITH_USB_XHCI_ADD_DYNAMIC_RING_BUFFER"


//#define SAMSUNG_PATCH_WITH_USB_GADGET_ENHANCEMENT	// patch about 'scan_periodic'
//#define SAMSUNG_PATCH_WITH_USB_TEMP     // patch for usb compatibility, but this patch need to verify for the each linux version.
//#define SAMSUNG_PATCH_WITH_USB_NOTCHECK_SPEED		//Don't check high-speed for invalid speed info(bcdUSB). 
//#define SAMSUNG_PATCH_WITH_MOIP_HOTPLUG
//#define SAMSUNG_PATCH_WITH_HUB_BUGFIX			// patch for some hubs malfunction
//#define SAMSUNG_PATCH_WITH_NOT_SUPPORTED_DEVICE	//Alarm of supported usb device (USB 2.0 certification)
//#define KKS_DEBUG	printk
#define	KKS_DEBUG(f,a...)
#endif

#endif
