/*
 * multi.c -- Samsung Multifunction Composite gadget driver
 *
 * Copyright (C) 2009 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/fs.h>

#define DRIVER_DESC		"Samsung Multifunction Composite Gadget"

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Aman Deep");
MODULE_LICENSE("GPL");


/***************************** All the files... *****************************/

/*
 *  
 * kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 * 
 * we have used the multi.c composite driver for referance
 */

#include "composite.c"
#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"


static bool is_enable_usb0 = 0;
static bool is_enable_usb1 = 0;

#include "f_mass_storage.c"
#include "f_eem.c"
#include "u_ether.c"
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_STORAGE_MULTI_DEVICE_SUPPORT
/* mass storage gadget functionality */
struct msc_gadget {
        int                             nports;

        struct fsg_common               fsg_common;
        struct device                   dev;
};

static void sam_multi_msc_release(struct device *dev)
{
        /* Nothing needs to be done */
}

/* samsung multi function gadget functionality*/
struct sam_multi_gadget {

	struct usb_gadget 	*gadget;	/* usb gadget file */

	struct msc_gadget 	msc_gadget;	/* mass storage gadegt structure */	
//	struct dual_eem_gadget	*dual_eem;	/* dual eem gadegt */

	struct device 		dev;		/* device file */
};

static struct sam_multi_gadget sam_multi = {.msc_gadget.nports = 0 };

#endif

/* module parameters */
static int use_eem = 1;
static int use_storage = 1;
module_param(use_eem, int, S_IRUGO);
module_param(use_storage, int, S_IRUGO);


/***************************** Device Descriptor ****************************/

#define MULTI_VENDOR_NUM	0x1d6b	/* Linux Foundation */
#define MULTI_PRODUCT_NUM	0x0104	/* Multifunction Composite Gadget */


#define SAM_MULTI_CDC_CONFIG_NUM    1

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_INFO
/* Serial Number for gadget driver */
#define SAM_MULTI_SERIAL_NUMBER    "201204E8474144474554"
#endif 	//CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_INFO

static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,

	.bcdUSB =		cpu_to_le16(0x0200),

	.bDeviceClass =		USB_CLASS_MISC /* 0xEF */,
	.bDeviceSubClass =	2,
	.bDeviceProtocol =	1,

	/* Vendor and product id can be overridden by module parameters.  */
	.idVendor =		cpu_to_le16(MULTI_VENDOR_NUM),
	.idProduct =		cpu_to_le16(MULTI_PRODUCT_NUM),
};


static const struct usb_descriptor_header *otg_desc[] = {
	(struct usb_descriptor_header *) &(struct usb_otg_descriptor){
		.bLength =		sizeof(struct usb_otg_descriptor),
		.bDescriptorType =	USB_DT_OTG,

		/*
		 * REVISIT SRP-only hardware is possible, although
		 * it would not be called "OTG" ...
		 */
		.bmAttributes =		USB_OTG_SRP | USB_OTG_HNP,
	},
	NULL,
};


#define	SAM_MULTI_STRING_CDC_CONFIG_IDX 0

static struct usb_string strings_dev[] = {
	[SAM_MULTI_STRING_CDC_CONFIG_IDX].s   = "Multifunction with DUAL CDC EEM and Storage",
	{  } /* end of list */
};

static struct usb_gadget_strings *dev_strings[] = {
	&(struct usb_gadget_strings){
		.language	= 0x0409,	/* en-us */
		.strings	= strings_dev,
	},
	NULL,
};




/****************************** Configurations ******************************/

static struct fsg_module_parameters fsg_mod_data = { .stall = 1 };
FSG_MODULE_PARAMETERS(/* no prefix */, fsg_mod_data);

#ifndef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_STORAGE_MULTI_DEVICE_SUPPORT
static struct fsg_common fsg_common;
#endif

/********** CDC EEM **********/

static __init int cdc_do_config(struct usb_configuration *c)
{
	int ret;

	if (gadget_is_otg(c->cdev->gadget)) {
		c->descriptors = otg_desc;
		c->bmAttributes |= USB_CONFIG_ATT_WAKEUP;
	}

    if (use_eem == 1) {
        dual_eem.eem_num = 0;        
	    ret = eem_bind_config(c);
	    if (ret < 0)
		    return ret;
    
        dual_eem.eem_num = 1;        
	    ret = eem_bind_config(c);
	    if (ret < 0)
		    return ret;
    }

    if (use_storage == 1) {
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_STORAGE_MULTI_DEVICE_SUPPORT
    	ret = fsg_bind_config(c->cdev, c, &sam_multi.msc_gadget.fsg_common);
#else
    	ret = fsg_bind_config(c->cdev, c, &fsg_common);
#endif
	    if (ret < 0)
		    return ret;
    }
	return 0;
}

static int cdc_config_register(struct usb_composite_dev *cdev)
{
	static struct usb_configuration config = {
		.bConfigurationValue	= SAM_MULTI_CDC_CONFIG_NUM,
		.bmAttributes		= USB_CONFIG_ATT_SELFPOWER,
	};

	config.label          = strings_dev[SAM_MULTI_STRING_CDC_CONFIG_IDX].s;
	config.iConfiguration = strings_dev[SAM_MULTI_STRING_CDC_CONFIG_IDX].id;

	return usb_add_config(cdev, &config, cdc_do_config);
}


/****************************** Gadget Bind ******************************/


static int __ref sam_multi_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int status, gcnum;

    dual_eem.cdev = cdev;
    if (use_eem) {
	    /* set up network link layer */
        the_dev = NULL;
        status = gether_setup(gadget, NULL);
	    if (status < 0)
		    return status;
	    
        dual_eem.dev[0] = the_dev;

	    /* set up second network link layer */
        the_dev = NULL;
        status = gether_setup(gadget, NULL);
	    if (status < 0) {
            goto fail1;
        }
        strcpy(the_dev->net->name, "usb1");
        dual_eem.dev[1] = the_dev;
    }

	/* set up mass storage function */
	if (use_storage) {
		void *retp;
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_STORAGE_MULTI_DEVICE_SUPPORT
		/* create a msc_gadget directory in sysfs */
		sam_multi.msc_gadget.dev.release = sam_multi_msc_release;
		sam_multi.msc_gadget.dev.parent = gadget->dev.parent->parent;
		dev_set_name (&sam_multi.msc_gadget.dev, "msc_gadget");
		
		status = device_register(&sam_multi.msc_gadget.dev);
                if (status) {
			WARNING(cdev, "msc_gadget directory is not created\n");
			put_device(&sam_multi.msc_gadget.dev);
                        goto fail2;
                }
		/* create port number 0 */
		sam_multi.msc_gadget.fsg_common.port_num = 0;
		sam_multi.msc_gadget.fsg_common.parent = &sam_multi.msc_gadget.dev;
		retp = fsg_common_from_params(&sam_multi.msc_gadget.fsg_common, cdev, &fsg_mod_data);
		if (IS_ERR(retp)) {
			status = PTR_ERR(retp);
			// Comment out for wrong file name crash at insmod
			//device_unregister(&sam_multi.msc_gadget.dev);
			goto fail2;
		}
		sam_multi.msc_gadget.nports = 1; /* number of ports */
#else
		retp = fsg_common_from_params(&fsg_common, cdev, &fsg_mod_data);
		if (IS_ERR(retp)) {
			status = PTR_ERR(retp);
			goto fail2;
		}
#endif
	}

	/* set bcdDevice */
	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0) {
		device_desc.bcdDevice = cpu_to_le16(0x0300 | gcnum);
	} else {
		WARNING(cdev, "controller '%s' not recognized\n", gadget->name);
		device_desc.bcdDevice = cpu_to_le16(0x0300 | 0x0099);
	}

	/* allocate string IDs */
	status = usb_string_ids_tab(cdev, strings_dev);
	if (unlikely(status < 0))
		goto fail3;

	status = cdc_config_register(cdev);
	if (unlikely(status < 0))
		goto fail3;

	/* we're done */
	dev_info(&gadget->dev, DRIVER_DESC "\n");
	return 0;


	/* error recovery */
fail3:
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_STORAGE_MULTI_DEVICE_SUPPORT
	if(use_storage) {
		fsg_common_put(&sam_multi.msc_gadget.fsg_common);
		 // Comment out for wrong file name crash at insmod
		//device_unregister(&sam_multi.msc_gadget.dev);
	}
#else
	if(use_storage) fsg_common_put(&fsg_common);
#endif
fail2:
	if(use_eem) {
        the_dev = dual_eem.dev[1];
         gether_cleanup();
        dual_eem.dev[1] = NULL;
    }
fail1:
	if(use_eem) {
        the_dev = dual_eem.dev[0];
        gether_cleanup();
        dual_eem.dev[0] = NULL;
    }
	return status;
}

static int __exit sam_multi_unbind(struct usb_composite_dev *cdev)
{
    if(use_eem) {
        the_dev = dual_eem.dev[0];
        gether_cleanup();
        dual_eem.dev[0] = NULL;

        the_dev = dual_eem.dev[1];
        gether_cleanup();
        dual_eem.dev[1] = NULL;
	}

#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_STORAGE_MULTI_DEVICE_SUPPORT
    if(use_storage) {
	fsg_common_put(&sam_multi.msc_gadget.fsg_common);
	//device_unregister(&sam_multi.msc_gadget.fsg_common.dev);
	device_unregister(&sam_multi.msc_gadget.dev);
    }
#else
    if(use_storage) fsg_common_put(&fsg_common);
#endif
	return 0;
}


/****************************** Some noise ******************************/
static struct usb_composite_driver sam_multi_driver = {
	.name		= "g_sam_multi",
	.dev		= &device_desc,
	.strings	= dev_strings,
	.unbind		= __exit_p(sam_multi_unbind),
	.iProduct	= DRIVER_DESC,
};

#ifdef  CONFIG_SAMSUNG_CDC_EEM_DEBUG
#define G_SAM_MULTI_DEBUG_FS		"samsung_gadget"
#define G_SAM_CDC_EEM_DEBUG_FS		"cdc_eem"
#define G_SAM_STORAGE_DEBUG_FS		"storage"

//CDC_EEM DEBUG
#define CDC_DEBUG_DST_ADDRESS			"dst_addr"
#define CDC_DEBUG_SRC_ADDRESS			"src_addr"
#define CDC_DEBUG_PING_TIME_OUT			"time_out"
#define CDC_DEBUG_PING_PACKET_SIZE		"data_size"
#define CDC_DEBUG_PING_INTERVAL			"ping_interval"
#define CDC_DEBUG_USB_PING_TEST			"usb_ping_test"
#define CDC_DEBUG_USB_THROUGHPUT_TEST	"usb_throughput_test"

#define CDC_DEBUG_CDC_INTF		"cdc_intf_"
#define MAX_CDC_INTF_SIZE		12		
#define NUM_CDC_INTERFACE		2

struct cdc_debug_intf
 {
	struct eth_dev  **ptr_eth;
	struct dentry   *cdc_intf_dir;	
	struct dentry 	*den_dst_addr;
	struct dentry 	*den_src_addr;
	struct dentry 	*den_time_out;
	struct dentry 	*den_interval;
	struct dentry 	*den_data_size;
	struct dentry 	*den_usb_ping_test;
	struct dentry 	*den_usb_speed_test;

	unsigned int	 dst_addr;
	unsigned int	 src_addr;
	unsigned int	 time_out;
	unsigned int	 interval;
	unsigned int	 data_size;
	unsigned int	 usb_ping_test;
	unsigned int	 usb_speed_test;
};

struct Sam_Debug
{
	struct dentry 		*sam_dir;
	struct dentry 		*storage_dir;
	struct cdc_debug_intf	cdc_debug[NUM_CDC_INTERFACE];
};

struct Sam_Debug  sam_debug;

atomic_t	   				timer_status;
atomic_t					received_packets;
atomic_t					transfred_packets;
unsigned int     			ping_test 				= 0;
unsigned int				throughput_test  		= 0;
unsigned long  				sent_speed_packets 		= 0;
unsigned long  				droped_speed_packets 	= 0;
unsigned long  				received_speed_packets  = 0;
static struct  timer_list 	speed_test_timer;

int read_single(struct seq_file *ptr_file, void *data)
{
	unsigned char address[16] = {0};

	snprintf(address,16, "%pI4",ptr_file->private);
	seq_printf(ptr_file,"%s",address);
	return 0;
}

int open_address(struct inode *ptr_inode, struct file *ptr_file)
{
	single_open(ptr_file,read_single,ptr_inode->i_private);
	return 0;
}

ssize_t write_address(struct file *file, const char __user *usr_ptr, size_t count, loff_t *index)
{
	char  		address[16];
	unsigned int	in_address;

	if(count == 0 || count > 16)
	   return 0;

	copy_from_user(address,usr_ptr,count);
	in_address = in_aton(address);
	*((unsigned int*)file->f_dentry->d_inode->i_private) = in_address;
	return count;
}

struct file_operations fops_address = {
		.open	 = open_address,
		.read    = seq_read,
		.write   = write_address,
		.release = single_release,
};

int debugfs_ping_test(struct cdc_debug_intf   *cdc_intf,struct net_device *dev)
{
    struct sam_debug_data dbg_data;
    struct eth_dev	  *eem_dev = netdev_priv(dev);
    unsigned int	  result;

    if(!netif_running(dev))
      return -EINVAL;

    dbg_data.data      = NULL;
    dbg_data.time_out  = cdc_intf->time_out;
    dbg_data.data_size = cdc_intf->data_size;
    snprintf(dbg_data.srcaddr.sa_data,16,"%pI4",&cdc_intf->src_addr);
    snprintf(dbg_data.dstaddr.sa_data,16,"%pI4",&cdc_intf->dst_addr);
    // Sending Debug Packets...
    result = send_debug_packet(dev,&dbg_data);

    // Waiting for Return Ack Packet
    if(cdc_intf->time_out > 0)
    {
      if(wait_event_interruptible_timeout(eem_dev->dbg_queue,eem_dev->ack_received == 1,HZ*cdc_intf->time_out) < 0)
	{
	  eem_dev->ack_received = 0;
	  return 0;
	}
	else
	{
	  eem_dev->ack_received = 0;
	  return -1;
	}
    }
    else
    {
      if( wait_event_interruptible(eem_dev->dbg_queue,eem_dev->ack_received == 1) == 0 )
      {
	 eem_dev->ack_received = 0;
	 return 0;
      }
      else
      {
	eem_dev->ack_received = 0;
	return -1;
      }
    }

    return 0;
}

int ping_read_single(struct seq_file *ptr_file, void *data)
{
//	seq_printf(ptr_file,"%d",*((unsigned int*)ptr_file->private));
	return 0;
}

int open_ping_test(struct inode *ptr_inode, struct file *ptr_file)
{
	single_open(ptr_file,ping_read_single,ptr_inode->i_private);
	return 0;
}

ssize_t write_ping_test(struct file *file, const char __user *usr_ptr, size_t count, loff_t *index)
{
	struct cdc_debug_intf   *cdc_intf = NULL;
	struct eth_dev		*eth      = NULL;
	int			value     = 0;
	unsigned int		infinite  = 0;
	unsigned long		timeout   = 0;	
	unsigned char		array[50] = {0};

	if(count == 0 || throughput_test == 1)
	   return 0;

	cdc_intf = (struct cdc_debug_intf *)file->f_dentry->d_inode->i_private;
	eth = *(cdc_intf->ptr_eth);
	if(!netif_running(eth->net))
	   return -EINVAL;


	if(eth->net != NULL)
	{
		copy_from_user(array,usr_ptr,count);
		array[count] = 0;
		value = simple_strtol(array,NULL,10);
		infinite = 0;
		if(value == 0)
		{
			value = 1;
			infinite = 1;
			printk(KERN_ALERT"Running Continues Ping\n");
		}
		ping_test = 1;
		while(value > 0)
		{
			if(debugfs_ping_test(cdc_intf,eth->net) < 0)
			{
				printk(KERN_ALERT"PING Failed\n");
				ping_test = 0;
				return 0;
			}
			
			if(!infinite)			
				value--;
			
			if(cdc_intf->interval > 0 && value > 0)
			{   
				timeout = msecs_to_jiffies(cdc_intf->interval);
				set_current_state(TASK_INTERRUPTIBLE);
				timeout = schedule_timeout(timeout);
				if(timeout > 0)
				   break;
			}
		}
	}
	ping_test = 0;
	return count;
}

#define TIMER_SET	 1
#define TIMER_CLEAR  0
void speed_timer_callback( unsigned long data )
{
	atomic_set(&timer_status,TIMER_CLEAR);
	return;
}

ssize_t write_throughput_test(struct file *file, const char __user *usr_ptr, size_t count, loff_t *index)
{
    int	  		    result     = 0;
    unsigned char	    array[50]  = {0};
    unsigned int	    value      = 0;
    unsigned long	    temp       = 0;
    unsigned long	    temp1      = 0;
    struct cdc_debug_intf   *cdc_intf  = NULL;
    struct sam_debug_data   dbg_data   = {0};
    struct eth_dev	    *eth       = NULL;


	if(count == 0 || ping_test == 1)
	   return 0;

	cdc_intf = (struct cdc_debug_intf *)file->f_dentry->d_inode->i_private;
	eth = *(cdc_intf->ptr_eth);
	if(!netif_running(eth->net))
		return -EINVAL;

    copy_from_user(array,usr_ptr,count);
    array[count] = 0;
    value = simple_strtol(array,NULL,10);
    setup_timer( &speed_test_timer, speed_timer_callback, 0 );

    // Start Throughput Test..
    dbg_data.data      = NULL;
    dbg_data.time_out  = 0;
    dbg_data.data_size = cdc_intf->data_size;
    snprintf(dbg_data.srcaddr.sa_data,16,"%pI4",&cdc_intf->src_addr);
    snprintf(dbg_data.dstaddr.sa_data,16,"%pI4",&cdc_intf->dst_addr);

    sent_speed_packets     = 0;
    received_speed_packets = 0;
    droped_speed_packets   = 0;
    atomic_set(&timer_status,TIMER_SET);
    atomic_set(&received_packets,0);
    atomic_set(&transfred_packets,0);

    temp				   = 1;
    throughput_test 	   = 1;
    mod_timer( &speed_test_timer, jiffies + msecs_to_jiffies(value*1000));
    // Sending Debug Packets...
    while(temp)
    {
    	result = send_debug_packet(eth->net,&dbg_data);
        if(result >= 0)
           sent_speed_packets++;
        else
           droped_speed_packets++;

        temp = atomic_read(&timer_status);
    }

    // Measure Throughput...
    msleep(1000);
    throughput_test = 0;
    temp  = atomic_read(&received_packets);
    temp1 = atomic_read(&transfred_packets);
    printk(KERN_DEBUG"Sent   Packets : %lu\n",sent_speed_packets);
    printk(KERN_DEBUG"Droped Packets : %lu\n",droped_speed_packets);
    printk(KERN_DEBUG"ACK    Packets : %lu\n",temp);
    printk(KERN_DEBUG"Un-ACK Packets : %lu\n",(sent_speed_packets - temp));
    printk(KERN_DEBUG"Tx-Com Packets : %lu\n",temp1);

    temp = (dbg_data.data_size + 32) * temp;	//Bytes
    temp = temp/1024;	     //KBytes
    temp = (temp*100)/1024;  //MBytes
    temp = temp/100; //Mbits
    printk(KERN_DEBUG"Throughput     : %luMbits/sec",(temp*8/value));
	printk(KERN_DEBUG"Throughput Test For %dsec\n",value);
    return count;
}

struct file_operations fops_ping_test = {
		.open	 = open_ping_test,
		.read    = seq_read,
		.write   = write_ping_test,
		.release = single_release,
};

struct file_operations fops_throughput_test = {
		.open	 = open_ping_test,
		.read    = seq_read,
		.write   = write_throughput_test,
		.release = single_release,
};

void sam_create_cdc_debugfs(struct cdc_debug_intf  *cdc_intf,struct dentry *parent,int intf_id)
{
    unsigned char intf_name[MAX_CDC_INTF_SIZE] = {0};   
 
    // CDC Interface Directory...
    cdc_intf->ptr_eth	= &dual_eem.dev[intf_id];
    cdc_intf->time_out  = 0;
    cdc_intf->data_size = 64;

    snprintf(intf_name,MAX_CDC_INTF_SIZE,"%s%d",CDC_DEBUG_CDC_INTF,intf_id);
    cdc_intf->cdc_intf_dir = debugfs_create_dir(intf_name,parent);

    cdc_intf->den_usb_ping_test = debugfs_create_file(CDC_DEBUG_USB_PING_TEST,
						      0644,
						      cdc_intf->cdc_intf_dir,
						      cdc_intf,
						      &fops_ping_test);

    cdc_intf->den_usb_speed_test = debugfs_create_file(CDC_DEBUG_USB_THROUGHPUT_TEST,
						      	  0644,
						      	  cdc_intf->cdc_intf_dir,
						      	  cdc_intf,
						      	  &fops_throughput_test);

    cdc_intf->den_dst_addr = debugfs_create_file(CDC_DEBUG_DST_ADDRESS,
						 0644,
						 cdc_intf->cdc_intf_dir,
						 &cdc_intf->dst_addr,
						 &fops_address);

    cdc_intf->den_src_addr = debugfs_create_file(CDC_DEBUG_SRC_ADDRESS,
						0644,
						cdc_intf->cdc_intf_dir,
						&cdc_intf->src_addr,
						&fops_address);

    cdc_intf->den_time_out    = debugfs_create_u32(CDC_DEBUG_PING_TIME_OUT,
						 0644,
						 cdc_intf->cdc_intf_dir,
						 &cdc_intf->time_out);

    cdc_intf->den_interval    = debugfs_create_u32(CDC_DEBUG_PING_INTERVAL,
						 0644,
						 cdc_intf->cdc_intf_dir,
						 &cdc_intf->interval);

    cdc_intf->den_data_size = debugfs_create_u32(CDC_DEBUG_PING_PACKET_SIZE,
					 	 0644,
					 	 cdc_intf->cdc_intf_dir,
					 	 &cdc_intf->data_size);
}

void sam_remove_cdc_debugfs(struct cdc_debug_intf  *cdc_intf)
{
	debugfs_remove(cdc_intf->den_dst_addr);
	debugfs_remove(cdc_intf->den_src_addr);
	debugfs_remove(cdc_intf->den_time_out);
	debugfs_remove(cdc_intf->den_interval);
	debugfs_remove(cdc_intf->den_data_size);
	debugfs_remove(cdc_intf->den_usb_ping_test);
	debugfs_remove(cdc_intf->den_usb_speed_test);
	debugfs_remove(cdc_intf->cdc_intf_dir);
	return;
}
#endif

static int __init sam_multi_init(void)
{
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
    unsigned int index = 0;
#endif
    if(!((use_eem == 1) || (use_storage == 1) )) {
        printk("\n Error : module parameters value : not permitted ");    
        printk("\n Enable at least one function ");    
        return 0;
	}
#ifdef CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_INFO        
        	iSerialNumber = SAM_MULTI_SERIAL_NUMBER;
#endif 	//CONFIG_SAMSUNG_PATCH_WITH_USB_GADGET_INFO        	    
#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
    //Creating ProcFS Debug Interface folder...
    sam_debug.sam_dir = debugfs_create_dir(G_SAM_MULTI_DEBUG_FS,NULL);
    while(index < NUM_CDC_INTERFACE)
    {
      sam_create_cdc_debugfs(&sam_debug.cdc_debug[index],sam_debug.sam_dir,index);
      index++;
    }   
#endif
	return usb_composite_probe(&sam_multi_driver, sam_multi_bind);
}
module_init(sam_multi_init);

static void __exit sam_multi_exit(void)
{

#ifdef CONFIG_SAMSUNG_CDC_EEM_DEBUG
    unsigned int index = 0;
    while(index < NUM_CDC_INTERFACE)
    {
      sam_remove_cdc_debugfs(&sam_debug.cdc_debug[index]);
      index++;
    }   
    debugfs_remove(sam_debug.sam_dir);

#endif
	usb_composite_unregister(&sam_multi_driver);
}
module_exit(sam_multi_exit);

