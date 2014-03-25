/**
* @file    gzHOST.h
* @brief   Gunzip Manager HOST is the efficient way to decompress the data 
*		   through the HW/SW decompressor on Single & Multicore 
* Copyright:  Copyright(C) Samsung India Pvt. Ltd 2012. All Rights Reserved.
* @author  SISC: manish.s2
* @date    2012/05/04
* @desc	   Generic gzManager Host 
*		   2012/05/28 Replaced the queue with the client link list
*		   2012/05/31 Final commenting of the code
*		   2012/09/03 Added changes for multiple compressions HW/SW
*					  Here we have modified a layer of compressor containing
*					  Both HW and SW. Changed the way we think.
*		   2012/10/22 Added Version Number in the file.
*		   2013/01/21 Increase the Squashfs mount count to 40 from 10
*					  thus changed the Max client count to 64.
*
*/
#ifndef __GZ_HOST__
#define __GZ_HOST__


#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/zlib.h>
#include <linux/gzmanager.h>

/* DEFINE */
/* GZM_WORKQ enabled for using the work queue in thread 
 * 			 disabled for using the direct call in the thread 
 * GZM_MAX_CLIENT currently maximum client are set to 32 
 */
#define GZM_LOAD_BALANCE
#define GZM_WORKQ
#define GZM_MAX_CLIENTS		(64)
#define GZM_MAX_SQFS		(40)
#define GZM_CGZIP			(1)
#define GZM_CZLIB			(2)
#ifdef CONFIG_GZM_ZLIB
#define GZM_MAX_CD			(2)
#else
#define GZM_MAX_CD			(1)
#endif
#define GZM_WQUEUE		"gzwqd"
#define MODULE_NAME     "Gunzip Manager"
#define DEBUG

#define GZM_VERSION		"v2.0.0"
/* States of the client 
 * These states will be helpful to detect 
 * current state of the client will be useful 
 * in future
 */
enum gzM_state{
	GHOST_DEAD = 0,
	GHOST_WAKEUP,
	//GHOST_HEKS,
	GHOST_RUNNING,
	GHOST_SLEEPING,
	GHOST_STALLED,
	GHOST_STOPPED
};

/* Currently we set the flags for enabling 
 * disabling some functionality 
 * its not use as of now 
 */
enum gzM_flag {
    GZ_Q_EN = 16,
    GZ_Q_DS = 32,
    GZ_Q_PN = 64
};
/* The HW/SW compressor decompressor structure 
 * Another addition for the compressors; So 
 * Now instead of HW seperately ghost has compressors
 * that might have different HW or SW decompressors. 
 * It might be we have more HW types in future :)
 * God knows
 */
struct gzM_cd_ops {
	int type;
	int hw;
    int state;
	int (*gzM_init) (void);
	int (*gzM_sw_compress)(char *ibuff, int ilength, char *opages[],int npages);
	int (*gzM_sw_decompress)(char *ibuff, int ilength, char *opages[],int npages);
	int (*gzM_hw_compress)(char *ibuff, int ilength, char *opages[],int npages);
	int (*gzM_hw_decompress)(char *ibuff, int ilength, char *opages[],int npages);
	int (*gzM_compress) (char *ibuff, int ilength, char *opages[],int npages);
	int (*gzM_decompress) (char *ibuff, int ilength, char *opages[],int npages);
	int (*gzM_hw_busy)(void);
	int (*gzM_release) (void);
};

/* Host is the core of the gzManager.
 * this host has all the details of the register clients
 * Every client has to register with the host 
 * It will request the decompression to host depending  
 * upon the flags and the type of client
 * Host will decide the priority based on the type 
 * and the decompression medium based on the flags 
 */
struct gzM_host {
	int res;
	int cp_res;
	int nr_client;
	int nr_data;
	int wait;
	enum gzM_state state;
	spinlock_t	lock;
	struct task_struct	*thread;
	const struct gzM_cd_ops	**cp;
	struct gzM_client	**client;
	struct workqueue_struct *wq;
    wait_queue_head_t wait_queue;
};


/* Functions */
//static int gzM_init();
//static void gzM_exit();
struct gzM_host_ops *gzM_register_cp(int index);
int gzM_unregister_cp(struct gzM_host_ops *cp);
//struct gzM_hw *gzM_register_hw(int type);
//void gzM_unregister_hw(struct gzM_hw *hw);
//int gzM_hw_busy(void);
void gzM_thread(void *data);
int gzM_idle_cpu(void);
#if 0
static int gzM_highmem();
#endif

#endif //__GZMANAGER__
