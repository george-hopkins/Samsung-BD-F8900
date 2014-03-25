/**
* @file    gzhost.c
* @brief   Gunzip Manager is the efficient way to decompress the data 
*          through the HW/SW decompressor on multicore 
* Copyright:  Copyright(C) Samsung India Pvt. Ltd 2012. All Rights Reserved.
* @author  SISC: manish.s2
* @date    2012/05/04
* @desc    Generic gzManager Host 
* 		   2012/05/10 - Single HW decompressor support
* 		   2012/05/18 - Added direct call support
* 		   2012/05/24 - Added Async call support
* 		   2012/05/28 - Added Workqueue functionality for scheduling the work
* 		   2012/06/06 - Added support for handling contiguous buffer in the gzmanager
* 		   2012/06/12 - Added support for different block sizes
* 		   2012/09/03 - Added support for different compressor zlib & gzip
*					  - As of now gzip is part of gzManager &
*					  - zlib is optional if we choose then we use
* 		   2012/10/22 - Added GunzipManager version info.
*		   TBD: 1. DEBUG to be added
*				2. Error codes to be added 
*			    3. HW decompressor code to be added 
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/zlib.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/kernel_stat.h>
#include <linux/tick.h>

#include <linux/gzmanager.h>
#include "gzhost.h"

#ifndef arch_idle_time
#define arch_idle_time(cpu) 0
#endif

/* Its the host structure 
 * ghost = gunzip Manager host 
 * and its invisible to outer world 
 * so the term "ghost" suits em :)
 */
struct gzM_host *ghost;

static int gzM_init(void );
static void gzM_exit(void);

/************** Gunzip Decompressor Functions **********/
/** 
 * Compressors Supported 
 * Gzip & zlib as of now.
 */
extern const struct gzM_cd_ops gzM_gzip_ops;
#ifdef CONFIG_GZM_ZLIB
extern const struct gzM_cd_ops gzM_zlib_ops;
#endif

static const struct gzM_cd_ops *gzM_decomp[GZM_MAX_CD] = {
     &gzM_gzip_ops,
#ifdef CONFIG_GZM_ZLIB
     &gzM_zlib_ops
#endif
};


/************** Ghost Compressor/Decompressor  Registration Functions ********/

/**
* @brief    gzM_unregister_cp
* @remarks  The function will unregister the compressor with the host
*			TBD: compressor initialization and registration
* @args		struct gzM_host
* @return   SUCCESS : 0
*           FAILURE : Error code 
*/
int gzM_unregister_cp(struct gzM_host_ops *hops)
{
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	
	if(!hops){
		printk(KERN_EMERG"[%s] NO hops \n",__FUNCTION__);
		return -1;
	}
	hops->decompress = NULL;
	hops->sw_decompress = NULL;
	hops->hw_decompress = NULL;
	hops->hw_busy = NULL;
	kfree(hops);

	return 0;	

}

/**
* @brief    gzM_register_cp
* @remarks  The function will return the hops for the client
* @args		int ctype
* @return   SUCCESS : gzM_comp
*           FAILURE : Error code 
*/
struct gzM_host_ops *gzM_register_cp(int ctype)
{
	int i;
	struct gzM_host_ops *hops;

	printk(KERN_EMERG"[%s] type %s \n",__FUNCTION__,(ctype == GZM_CGZIP)?"GZM_CGZIP":"GZM_CZLIB");

	hops = kmalloc(sizeof(struct gzM_host_ops),GFP_KERNEL);
	if(NULL == hops){
		printk(KERN_EMERG"[%s] Error in allocating hops \n",__FUNCTION__);
		return NULL;
	}

	for(i=0;i<GZM_MAX_CD;i++){
		if(ghost->cp[i]->type == ctype){
			if(ghost->cp[i]->state == GZ_Q_EN){
				hops->decompress = ghost->cp[i]->gzM_decompress;
				hops->sw_decompress = ghost->cp[i]->gzM_sw_decompress;
				hops->hw_decompress = ghost->cp[i]->gzM_hw_decompress;
				hops->hw_busy = ghost->cp[i]->gzM_hw_busy;
				return hops;
			}else{
				printk(KERN_EMERG"[%s] Error the comp %d is disabled \n",__FUNCTION__,ctype);
			}
		}
	}
	
	printk(KERN_EMERG"[%s] Error Didnt find any comp of type %d  \n",__FUNCTION__,ctype);
	kfree(hops);
	return NULL;
}


/************** Ghost Async Decompression Functions **********/
/**
* @brief    gzM_submit_data
* @remarks  The function is called from the client for submitting 
*			the data to the host/ client list so that the host 
*			thread will fetch the data from the client list and 
*			start decompression based on the priority.
* @return   SUCCESS : 0
*           FAILURE : Error code 
*/
int gzM_submit_data(struct gzM_queue_data *data)
{
	struct gzM_client *client;

#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif
	if((NULL == data)|| (NULL == data->client)){
		printk(KERN_EMERG"[%s]Error :: illegal data ptr\n",__FUNCTION__);
		return -1;	
	}

	client = data->client;

	spin_lock(&client->lock);	
	list_add(&data->list,&data->client->cdata);	
	ghost->nr_data++;
	spin_unlock(&client->lock);	

	spin_lock(&ghost->lock);	
	ghost->wait = 1;
	spin_unlock(&ghost->lock);	
	if((GHOST_SLEEPING == ghost->state))
		wake_up_interruptible(&ghost->wait_queue);
	return 0;
}
EXPORT_SYMBOL(gzM_submit_data);

/**
* @brief    gzM_register_client
* @remarks  The function will register the client with the host
*			this function will called from the client who wants
*			to register with the Host with client type & the 
*			flags for the method of decompression.
* @return   SUCCESS : client structure
*           FAILURE : Error code 
*/
struct gzM_client *gzM_register_client(enum gzM_type type,int gzM_flags)
{
	struct gzM_client *client = NULL;
	int index = 0;
#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif

	/* Check the ghost state before registration */
	client = kmalloc(sizeof(struct gzM_client), GFP_KERNEL|__GFP_ZERO); 
	if(NULL == client){
		printk(KERN_EMERG"[%s] No client allocated \n",__FUNCTION__);
		return NULL;
	}
	// Client has given the flags 
	if((useHW || gzM_flags)&& (!(useSW || gzM_flags))){
		printk(KERN_EMERG"[%s] Use direct call for the useHW case \n",__FUNCTION__);
		printk(KERN_EMERG"[%s] Moving to useHWSW \n",__FUNCTION__);
		client->flags |= useHWSW;
	}
	INIT_LIST_HEAD(&client->cdata);
	init_waitqueue_head(&client->wait_queue);
    spin_lock_init(&client->lock);

	/* do we need the client ops ?
	 * Priority of the client
	 * 0  = reserved 
	 * 1-10  = squashfs
	 * 11  = other module
	 * whereas Between the section it is First come first serve
	 */
	switch(type)
	{
		case GZ_HIGHPRIO:
			printk(KERN_EMERG"[%s] Currently it is reserved \n",__FUNCTION__);
			if(client)
				kfree(client);	
			return NULL;
		case GZ_SQUASHFS:
			printk(KERN_EMERG"[%s] Request for Squashfs \n",__FUNCTION__);
			index = 1;
			for(index=1; index <= GZM_MAX_SQFS;index++){
				if( NULL == ghost->client[index]){
					printk(KERN_EMERG"[%s] Squashfs is registering @ %d \n",__FUNCTION__,index);
					
					spin_lock(&ghost->lock);	
					client->prio = index;
					if(gzM_flags & useGZIP){
						client->flags |= useGZIP;
						client->hops = 	gzM_register_cp(GZM_CGZIP);
					}else if(gzM_flags & useZLIB){
						client->flags |= useZLIB;
						client->hops = 	gzM_register_cp(GZM_CZLIB);
					}else{
						printk(KERN_EMERG"[%s] Unsupported COMP %d\n",__FUNCTION__,gzM_flags);
						goto unlock;
					}
					if(NULL == client->hops){
						printk(KERN_EMERG"[%s] Unsupported COMP %d\n",__FUNCTION__,gzM_flags);
						goto unlock;
					}
					ghost->client[index] = client;
					ghost->nr_client++;
					spin_unlock(&ghost->lock);	

					break;
				}
			}
			if(index > GZM_MAX_SQFS){
				printk(KERN_EMERG"####[%s] All Squashfs index are filled @ %d \n",__FUNCTION__,index);
				// do cleanup here 
				// cant just return NULL from here 
				kfree(client);
				return NULL;
			}
			break;
		case GZ_OTHR:
			// lock on ghost 
			index = GZM_MAX_SQFS + 1; 
			for(index=GZM_MAX_SQFS + 1; index < GZM_MAX_CLIENTS;index++){
				if(NULL == ghost->client[index]){
					printk(KERN_EMERG"####[%s] Client is registering @ %d \n",__FUNCTION__,index);
					break;
				}
			}
			if(index >= 32 ){
				printk(KERN_EMERG"[%s] MAX registration limit reached  \n",__FUNCTION__);
				// either do the fragmentation on the ghost list OR
				// do the cleanup and return error 
				kfree(client);
				return NULL;
			}
			spin_lock(&ghost->lock);	
			ghost->client[index] = client;
			ghost->nr_client++;
			client->prio = index;
			spin_unlock(&ghost->lock);	
			printk(KERN_EMERG"[%s] Allocated at priority %d \n",__FUNCTION__,index);
			break;
	}
	
	return client;
unlock:
	spin_unlock(&ghost->lock);	
	if(NULL != client){
		kfree(client);	
		client = NULL;
	} 
	return NULL;
}

/**
* @brief    gzM_uregister_client
* @remarks  The function will uregister the client with the host
*			this function will called from the client who wants
*			to uregister with the Host. 
* @args		struct gzM_client
* @return   SUCCESS : 0
*           FAILURE : Error code 
*/
int gzM_unregister_client(struct gzM_client *client)
{
	struct gzM_queue_data *data,*next;
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	if(NULL == client){
		printk(KERN_EMERG"[%s] client is not available \n",__FUNCTION__);
		return -1;
	}
	/* Check the ghost state here first*/
	// take the ghost lock 
	spin_lock(&ghost->lock);	
	ghost->client[client->prio] = NULL;
	ghost->nr_client--;
	spin_unlock(&ghost->lock);

	spin_lock(&client->lock);	
	// Check each entry in the list 
	// What to do with the data at this point ??
	list_for_each_entry_safe(data,next,&client->cdata,list){
		if(data){
			//printk(KERN_EMERG"data is available \n");
			// process the data or just dump it 
			list_del(&data->list);
		}
	}
	spin_unlock(&client->lock);	

	gzM_unregister_cp(client->hops);
	// Check the client state before clearing everything
	// it might be that the client is decompressing and 
	// we cleaned up
	if(NULL != client){
		kfree(client);	
		client = NULL;
	} 
	
	return 0;
}

/**
* @brief    gzM_unregister_clients
* @remarks  The function will unregister all the client with the host
*			this function will called from the host for unregistering
*			the clients from the Host.
* @args		struct gzM_host	
* @return   VOID 
*/
void gzM_unregister_clients(struct gzM_host *host)
{
	int i;
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	for(i=0;i< GZM_MAX_CLIENTS; i++){
		if(ghost->client[i])
			if(gzM_unregister_client(ghost->client[i]))
				printk(KERN_EMERG"[%s] Error in client :%d \n",__FUNCTION__,i);
	}
}

/**
* @brief    gzM_cpu_idle_time_usecs
* @remarks  The function will return the usecs time for the cpu 
* @args		int cpu
*			cputime64_t *wall
* @return   SUCCESS : 0
*           FAILURE : Error code 
*/
static cputime64_t gzM_cpu_idle_time_usecs(unsigned int cpu, cputime64_t *wall)
{
    cputime64_t idle_time;
    cputime64_t cur_wall_time;
    cputime64_t busy_time;
	//printk(KERN_EMERG"[%s]\n",__FUNCTION__);
 
    cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());
    busy_time = cputime64_add(kstat_cpu(cpu).cpustat.user,
            kstat_cpu(cpu).cpustat.system);
 
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.irq);
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.softirq);
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.steal);
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.nice);
 
    idle_time = cputime64_sub(cur_wall_time, busy_time);
    if (wall) 
        *wall = (cputime64_t)jiffies_to_usecs(cur_wall_time);
 
    return (cputime64_t)jiffies_to_usecs(idle_time);
}

/**
* @brief    gzM_idle_cpu
* @remarks  The function will return the idle cpu.
*			but if no cpu is idle it will calculate the load on each 
*			cpu and return the least loaded cpu 
* @args		VOID	
* @return   VOID 
*/
int gzM_idle_cpu()
{
	int i,cpu = 0;
	cputime64_t idle_1 =0,idle_2=0, last_time=0;
	//printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	for_each_online_cpu(i){
		if(idle_cpu(i)){
			cpu = i;
			break;
		}
		// now we have to calculate the load
		idle_1 = gzM_cpu_idle_time_usecs(i,&last_time);
		if(0 == i){
			idle_2 = idle_1;
			cpu = i;
		}
		if(idle_1 > idle_2){
			idle_2 = idle_1;
			cpu = i;
		}
		//printk(KERN_EMERG" idle cpu[%d]  = %llu \n",i,idle_1);
    }
	//printk(KERN_EMERG"least idle cpu [%d] = %llu \n",cpu,idle_2);
	//printk(KERN_EMERG"least idle cpu [%d] \n",cpu);
	return cpu;	
}

/**
* @brief    gzM_decompress_work
* @remarks  The function is the work function for the work_queue
*			Work queue threads will call this function for each data 
*			get the data from the work_structure and decompress the same
*			This function will call the end_decomp function and send the 
*			status through that function only.
* @args		struct work_struct
* @return   VOID 
*/
static void gzM_decompress_work(struct work_struct	*work)
{
    struct gzM_queue_data   *data =   
        container_of(work, struct gzM_queue_data, d_work);
	struct gzM_client *client = NULL;
                            
    if ((NULL == data)){
		printk(KERN_EMERG"[%s] No data available in work  \n",__FUNCTION__);
		return;
	}

	client = data->client;

    if (NULL != client){
		data->error = client->hops->decompress(data->buff+data->offset,\
							data->length,data->opages,data->npages);
		if(0 >= data->error ){
			printk(KERN_EMERG"[%s] Error in data \n",__FUNCTION__);
		}

		if(data->end_decomp)
			data->end_decomp(data);
	}else{
		printk(KERN_EMERG"[%s] No client available  \n",__FUNCTION__);
	}
}

/**
* @brief    gzM_thread
* @remarks  This the host thread which will iterates over the clients and 
*			check the data in the client list based on the priority 
*			The thread will  function will check the flags in each client 
*			and call the hw decompressor if the decompressor is not available 
*			the thread will call the sw decompressor and schedule the work in 
*			work queue 
* @args		data from the list of the client
* @return   VOID 
*/
void gzM_thread(void *arg)
{
	int index;
	int cpu;
	struct gzM_host *host = (struct gzM_host *)arg;
	struct gzM_queue_data *data = NULL, *next;
	struct gzM_client *client = NULL;
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	
	/* This is the main thread 
	 * check if any client is registered or not, for each client 
     * we need to check is its list has the data first index is 
	 * left black for the future use
	 * 2nd to 10th is for Squashfs 
	 * 11th to 32nd is for others and on as per the First Come First Serve basis 
	 */
	
	ghost->state = GHOST_RUNNING;
	while(!kthread_should_stop())
	{
		if(host->nr_client){
			/* check all the clients in the host */
			for(index = 0; index < GZM_MAX_CLIENTS; index++){
				spin_lock(&ghost->lock);	
				client = ghost->client[index];
				spin_unlock(&ghost->lock);	
				if( (NULL != client) && !list_empty(&client->cdata)){
					// we are not going to wait in the loop
					// we will fetch and submit to work queue 
					// so no need to worry 
					list_for_each_entry_safe(data,next,&client->cdata,list){
						if(NULL != data){
							//printk(KERN_EMERG"data is available \n");
							// process the data or just dump it 
							// using direct call here 
							spin_lock(&client->lock);	
							list_del(&data->list);
							ghost->nr_data--;
							spin_unlock(&client->lock);	
#ifdef GZM_WORKQ
							INIT_WORK(&data->d_work, gzM_decompress_work);
#ifndef GZM_LOAD_BALANCE
						    queue_work(ghost->wq, &data->d_work);	
#else // GZM_LOAD_BALANCE
							/* check the least loaded cpu 
							 * and offload the work on that cpu
							 */
							cpu = gzM_idle_cpu();
						    queue_work_on(cpu,ghost->wq, &data->d_work);
#endif // GZM_LOAD_BALANCE
#else  // GZM_WORKQ	
							data->error = client->hops->decompress(data->buff + data->offset,\
											data->ilength,data->opages,data->npages);
							if(data->end_decomp)
								data->end_decomp(data);
#endif // GZM_WORKQ
						}
					}
	        	}
			}
			if(ghost->nr_data)
				continue;
			spin_lock(&ghost->lock);	
			ghost->state = GHOST_SLEEPING;
			ghost->wait = 0;
			spin_unlock(&ghost->lock);	

			wait_event_interruptible(ghost->wait_queue, ghost->wait);
			ghost->state = GHOST_RUNNING;
		}else{

			ghost->state = GHOST_SLEEPING;
			wait_event_interruptible(ghost->wait_queue, ghost->nr_data);
			ghost->state = GHOST_RUNNING;
		}
	}
	ghost->state = GHOST_STALLED;
}


/************** Ghost Registration Functions **********/
/**
* @brief    gzM_release_host
* @remarks  The function will deallocate the host
* @args		struct gzM_host
* @return   VOID 
*/
void gzM_release_host(struct gzM_host *ghost)
{
	/* need to take spinlock ?
	 To free it efficiently*/
	if(ghost){
		ghost->state = GHOST_DEAD;
		if(ghost->wq)
			destroy_workqueue(ghost->wq);
		if(ghost->client)
			kfree(ghost->client);
		kfree(ghost);
	}
}

/**
* @brief    gzM_init
* @remarks  The function will initialize the gunzip Manager
*			this function will called from module init 
*			for the ghost allocation and prepration
* @args		VOID
* @return   SUCCESS : 0
*           FAILURE : Error code 
*/
static int gzM_init(void)
{
	int i =0;
	int c =0;
	
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	printk(KERN_EMERG"Starting [%s]-%s\n",MODULE_NAME,GZM_VERSION);
	if(ghost){
		printk(KERN_EMERG"[%s] Gz Manager Already registerd: state %d \n",__FUNCTION__,ghost->state);
		return -(ghost->state);
	}else{
		ghost = kmalloc(sizeof(struct gzM_host), GFP_KERNEL);
		if(NULL == ghost){
			printk(KERN_EMERG"[%s] Not able to allocate GHOST  \n",__FUNCTION__);
			return -(ENOMEM);
		}
#if 0
		ghost->cp = kmalloc(GZM_MAX_COMP * sizeof(struct gzM_cd_ops*),GFP_KERNEL);
		if(NULL == ghost->hw){
			printk(KERN_EMERG"[%s] Not able to allocate HW \n",__FUNCTION__);
			goto exit;
		}
#endif
		ghost->client = kmalloc(GZM_MAX_CLIENTS * sizeof(struct gzM_client*),GFP_KERNEL);
		if(NULL == ghost->client){
			printk(KERN_EMERG"[%s] Not able to allocate clients \n",__FUNCTION__);
			// dont know what to do here 
			// Can we still work with the HW decompressor ?	
		}
		for(i=0; i < GZM_MAX_CLIENTS;i++)
			ghost->client[i] = NULL;
	}

	ghost->state = GHOST_WAKEUP;
	/*
	 1. Initialize host  
	 2. Create the gzM_thread 
	 3. Initialize the host structure
		a. Initialize the work queue.
	*/

	/* Need to initialize decompressor first 
	 * because if not able to do with the queue or 
	 * the thread we can use the direct call 
	 */
	ghost->cp = gzM_decomp;
	for(c=0;c<GZM_MAX_CD;c++){
		if(ghost->cp[c]->gzM_init()){
			printk(KERN_EMERG"ERROR in initializing the decompressor [%d] \n",c+1);
			//ghost->cp[c]->state = GZ_Q_DS;
			continue;
		}
		ghost->cp_res++;
	}
	if(!ghost->cp_res){
		printk(KERN_EMERG"ERROR NO decompressor [%d] \n",ghost->cp_res);
	}
	ghost->nr_client = 0;
	ghost->nr_data = 0;
	ghost->res = ghost->cp_res;
	ghost->res += num_online_cpus();

	// create the spin lock 
    spin_lock_init(&ghost->lock);
	init_waitqueue_head(&ghost->wait_queue);

#ifdef GZM_WORKQ
	// create the work queue
	//ghost.wq = create_workqueue(GZM_WQUEUE);
	ghost->wq = alloc_workqueue(GZM_WQUEUE,WQ_MEM_RECLAIM | WQ_HIGHPRI, 1);
	if(!ghost->wq){
		printk(KERN_EMERG"Not able to allocate work queue \n");
		// Need to check what we have to do here
		goto exit;
	}

#endif
	/* Create gzM_thread */
	ghost->thread = kthread_create((void *)gzM_thread, ghost, MODULE_NAME);
	if (IS_ERR(ghost->thread)){
		printk(KERN_ERR ": unable to start gunzip Manager thread\n");
        goto exit;
	}else{
		printk(KERN_INFO" ################## \n");
		printk(KERN_INFO "Created gzM thread \n");
	    printk(KERN_INFO"gHost is up!!  \n");
		// For simulation we will bind the 
		// thread to one core
	   	//kthread_bind(ghost->thread,1);
	   	wake_up_process(ghost->thread);
    }

	return 0;
exit:
	// do more cleanups
	gzM_release_host(ghost);
	return -1;
}

/**
* @brief    gzM_exit
* @remarks  The function will clear the gunzip Manager.
*			It will release all the clients and pending decompression
*			The function will unregister all the client & the hw 
*			decompressors with the host and release the host
* @args		VOID
* @return   VOID 
*/
static void gzM_exit(void)
{
	int i ;
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	/*check the ghost state */
	if(!ghost){
		printk(KERN_EMERG"[%s] These are rumours : No ghost exist \n",__FUNCTION__);
		return;
	}
	/* exit as per the state of the ghost */
	switch(ghost->state)
	{
		case GHOST_DEAD:
			printk(KERN_EMERG"[%s] ghost is no more \n",__FUNCTION__);
			break;
	    case GHOST_WAKEUP:
			printk(KERN_EMERG"[%s] ghost is waking up \n",__FUNCTION__);
			printk(KERN_EMERG"[%s] This is really creapy check out \n",__FUNCTION__);
			printk(KERN_EMERG"[%s] Cant help you out! \n",__FUNCTION__);
			return;
    	case GHOST_STOPPED:
		case GHOST_SLEEPING:
				// need to check here 
				// the should_stop before waking up 
				spin_lock(&ghost->lock);	
				ghost->wait = 1;
				spin_unlock(&ghost->lock);	
				wake_up_interruptible(&ghost->wait_queue);
    	case GHOST_RUNNING:
			printk(KERN_EMERG"[%s] Have to wait: \n",__FUNCTION__);
#if 0
			// We have put the spell on ghost 
			ghost->state = GHOST_HEKS;
			// now wait till it get stalled 
			// Heks is witch in germen
    	case GHOST_HEKS:
#endif
			if(ghost->thread)
				kthread_stop(ghost->thread);
			else
				ghost->state = GHOST_STALLED;

			while(ghost->state != GHOST_STALLED){
				// need to be careful here 
				// have to use wait wqueue or something
				schedule();
			}
	    case GHOST_STALLED:
			printk(KERN_EMERG"[%s] Killing the ghost: \n",__FUNCTION__);
			if(ghost->wq)
				destroy_workqueue(ghost->wq);
			gzM_unregister_clients(ghost);
			for(i=0;i<GZM_MAX_CD;i++){
				if(ghost->cp[i]->gzM_release()){
					printk(KERN_EMERG"ERROR in initializing the decompressor [%d] \n",i+1);
					//ghost->cp[i]->state = GZ_Q_DS;
					continue;
				}
				ghost->cp_res--;
				ghost->res--;
			}
			gzM_release_host(ghost);
	}

}

/**
* @brief    gzM_module_init
* @remarks  The function will initialize the gunzip Manager
* @args		VOID
* @return   SUCCESS : 0
*           FAILURE : Error code 
*/
static int __init gzm_module_init(void)
{
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	gzM_init();
    return 0;
}
 
/**
* @brief    gzM_module_exit
* @remarks  The function will exit the gunzip Manager 
* @args		VOID
* @return   VOID 
*/
static void __exit gzm_module_exit(void)
{
	printk(KERN_EMERG"[%s]",__FUNCTION__);	
	gzM_exit();

}

 
module_init(gzm_module_init);
module_exit(gzm_module_exit);
MODULE_AUTHOR("Manish Sharma <manish.s2@samsung.com>");
MODULE_LICENSE ("Copyright(C) Samsung India Pvt. Ltd 2012");
