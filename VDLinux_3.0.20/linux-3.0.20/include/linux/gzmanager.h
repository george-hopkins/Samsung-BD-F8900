/**
* @file    gzManager.h
* @brief   Gunzip Manager is the efficient way to decompress the data 
*		   through the HW/SW decompressor on Single & Multicore 
* Copyright:  Copyright(C) Samsung India Pvt. Ltd 2012. All Rights Reserved.
* @author  SISC: manish.s2
* @date    2012/05/04
* @history 2012/09/03	-- Supported more compression types and expand flags
*						   added zlib and gzip support	
* @desc	   Generic gzManager Host 
*/
#ifndef __GZ_MANAGER__
#define __GZ_MANAGER__


#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/list.h>


/* GZM_BLOCKS_PER_SQBLK	 depends upon the block size of the 
 * device and the squashfs block size; 
 * currently its 128; we will make it dynamic depending upon 
 * the device block size and sqfs block size 
 */
#define GZM_PAGES_PER_SQBLK	(128) 
/* The GZM_BLK_BITS are derived from the Squashfs and 
 * supports 1M Max Squashfs block size
 * 1M / 4K = 256 / 32 = 8
 */
#define GZM_BLK_BITS		(8) 

/* gzM_type defines the type of the client 
 * it has currently HIGHPRIO which is reserved
 * SQuashfs has the highest priority then other 
 * later we can add the different priorities here 
 */
enum gzM_type {
	GZ_HIGHPRIO	= 1,
	GZ_SQUASHFS,
	GZ_OTHR
};
/* gzM_flags tell the type of decompression
 * currently we have useHW, useHWSW and useSW,
 * useBLKS is used for not waiting for the BLKS 
 * if useBLKS is set the the client should
 * submit many blocks to the gunzip Manager
 * currently client will use this flag for waiting 
 * on waitqueue 
 * Adding More details
 *  ------------------------------
 * |31| ....... |8|7|6|5|4|3|2|1|0| 
 *  ------------------------------
 *  Bits	Value	Meaning
 *	 0		  1		useHW	- Use HW decompressor
 *	 1		  1		useSW	- use SW decompressor
 *	 2		 0/1	useBLKS - Direct call =0 / Async =1
 *	 3		  - 	Reserved
 *	 4		  1 	Zlib compression
 *	 5		  1	 	gzip compression
 *	 6		  -		Reserved
 *	 7		  -
 *	 8		  -
 *	 ..		 ..
 *	 31		  - 	Reserved
 */ 
//enum gzM_flags {
#define		useHW	(1 << 0)
#define		useSW	(1 << 1)
#define 	useHWSW (useHW | useSW)
#define		useBLKS	(1 << 2)
 /* Compression Types */
#define		useGZIP	(1 << 4)
#define		useZLIB	(1 << 5)

/* gzm_queue_data is the data structure 
 * which will be used for giving the data
 * to the host for decompression.
 * its the client responsibility to fill this 
 * structure properly. Otherwise results in error
 * few of the variables can be changed in future 
 * currently the structure is specific to the squashfs 
 * filesystem.
 * The queue_data will be modified later as the 
 * client specific data will go in the private pointer
 */ 
struct gzM_queue_data {
	char **opages; 
	char *buff;
	struct gzM_client *client;
	int offset;
	int length;
	int npages;
	int error;
	int state;
	struct work_struct d_work;
	void *private;
	struct list_head list;
	char *hmem_tbl;
	int hmem_state[GZM_BLK_BITS];
	int (*end_decomp)(struct gzM_queue_data*);
};

/* host operations to be called from the client
 * these ops will be set by the host during 
 * client registration with the host
 */
struct gzM_host_ops {
	int (*decompress) (char *ibuff, int ilength, char *opages[],int npages);
	int (*sw_decompress)(char *ibuff, int ilength, char *opages[],int npages);
	int (*hw_decompress)(char *ibuff, int ilength, char *opages[],int npages);
	int (*hw_busy)(void);
};

/* client structure is to be allocated by the host 
 * during the registration just pass the type of client 
 * and the type of decompression needed.
 * We are thinking of another variable for the block size
 * and later the dependency can be on the device block size 
 * if virtual addresss is there :: working on that 
 */
struct gzM_client {
	/* Do not change the private data 
	 * this is for internal use 
	 */
	void *private; 
	struct gzM_client_ops *ops;
	int type;
	int flags;
	int prio;
	struct gzM_host_ops *hops;
	spinlock_t lock;
	struct list_head cdata;
	wait_queue_head_t wait_queue;
};

/* Functions */
struct gzM_client *gzM_register_client(enum gzM_type type,int gzM_flags);
int gzM_unregister_client(struct gzM_client *client);
/* this data structure is to be prepared by the module who registered the 
 * client with the host */
struct gzM_queue_data *gzM_data_prep(struct gzM_client *client,
	void **buffer, struct buffer_head **bh, int b, int offset, int length,
    int srclength, int page, int block_size);
int gzM_submit_data(struct gzM_queue_data *data);
#endif //__GZMANAGER__
