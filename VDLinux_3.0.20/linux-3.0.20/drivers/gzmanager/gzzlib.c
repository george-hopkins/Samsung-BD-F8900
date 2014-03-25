/**
* @file    gzzlib.c
* @brief   Zlib wrapper for the Gunzip Manager Compressor 
*		   To be modified further for the formatted way. 
* Copyright:  Copyright(C) Samsung India Pvt. Ltd 2012. All Rights Reserved.
* @author  SISC: manish.s2
* @date    2012/09/03
* @desc    zlib wrapper for the gzManager Host 
* 		   2012/09/03 - Added zlib wapper
*
*/

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/zlib.h>
#include <linux/kernel_stat.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/buffer_head.h>


#include <linux/gzmanager.h>
#include "gzhost.h"

static int gzlib_init(void);
static int gzlib_hw_busy(void);
static int gzlib_release(void);

static int hw =0;
/**
* @brief    gzlib_hw_busy
* @remarks  The function returns the status 
*			of the HW decompressor 
* @return   HW busy : 1
*           HW available : 0
*/
static int gzlib_hw_busy(void)
{
#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif
	/* Currently HW is always busy */
	return 1;
}

/**
* @brief    gzlib_release
* @remarks  The function free the gzip compressor  
*			memory and data type 
* @return   SUCCESS : 0
*           FAILURE : -1
*/
static int gzlib_release(void)
{
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}
/**
* @brief    gzlib_init
* @remarks  The function initialize the gzip compressor /decompressor 
*			It will initialize the structure's  
* @return   SUCCESS : 0
*           FAILURE : -1
*/
static int gzlib_init(void)
{
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}

/**
* @brief    gzlib_hw_compress
* @remarks  The function compress the data given 
*			on the of hw compressor 
* @return   SUCCESS : length of compressed data
*           FAILURE : Error code
*/
int gzlib_hw_compress(char *ibuff, int ilength, char *opages[],int npages)
{

	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}

/**
* @brief    gzlib_sw_compress
* @remarks  The function compress the data given 
*			on the cpu core instead of hw compressor 
* @return   SUCCESS : length of compressed data
*           FAILURE : Error code
*/
static int gzlib_sw_compress(char *ibuff, int ilength, char *opages[],int npages)
{

	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}


/**
* @brief    gzlib_sw_decompress
* @remarks  The function decompress the data given 
*			on the cpu core instead of hw decompressor 
* @return   SUCCESS : length of decompressed data
*           FAILURE : Error code
*/
static int gzlib_sw_decompress(char *ibuff, int ilength, char *opages[],int npages)
{

	int zlib_err;
	int len = 0, page =0;
    z_stream *stream = NULL;


	/* Initialize the zlib stream */
	stream = kmalloc(sizeof(z_stream), GFP_KERNEL);
    if (stream == NULL){
        return -ENOMEM;
	}

    //stream->workspace = kmalloc(zlib_inflate_workspacesize(),GFP_KERNEL);
    stream->workspace = vmalloc(zlib_inflate_workspacesize());
    if (stream->workspace == NULL){
		kfree(stream);
		return -ENOMEM;
    }


	stream->next_in = ibuff;
    stream->avail_in = ilength;
    stream->next_out = opages[page++];
    stream->avail_out = PAGE_CACHE_SIZE;

	zlib_err = zlib_inflateInit(stream);
	if (zlib_err != Z_OK) {
		printk(KERN_EMERG"zlib_inflateInit returned unexpected "
			"result 0x%x, length %d\n",
			zlib_err, ilength);
		goto release;
	}

   do {
 
        if (stream->avail_out == 0 ) {
            stream->next_out = opages[page++];
            stream->avail_out = PAGE_CACHE_SIZE;
        }
#ifdef GZM_DEBUG
		printk(KERN_EMERG"[%s] ######## Before Inflate ######### \n",__FUNCTION__);
		printk(KERN_EMERG"[%s] avail_in [%d] avail_out [%d] \n",__FUNCTION__,stream->avail_in,stream->avail_out);
		printk(KERN_EMERG"[%s] next_in 0x%x next_out %#x\n",__FUNCTION__,stream->next_in,stream->next_out);
		printk(KERN_EMERG"[%s] total_in %d total_out %d\n",__FUNCTION__,stream->total_in,stream->total_out);
		printk(KERN_EMERG"[%s] pages[%d]= 0x%x \n",__FUNCTION__,page,opages[page-1]);
#endif

        zlib_err = zlib_inflate(stream, Z_SYNC_FLUSH);

#ifdef GZM_DEBUG
		printk(KERN_EMERG"[%s] ######## After Inflate ######### \n",__FUNCTION__);
		printk(KERN_EMERG"[%s] avail_in [%d] avail_out [%d] \n",__FUNCTION__,stream->avail_in,stream->avail_out);
		printk(KERN_EMERG"[%s] next_in 0x%x next_out %#x\n",__FUNCTION__,stream->next_in,stream->next_out);
		printk(KERN_EMERG"[%s] total_in %d total_out %d\n",__FUNCTION__,stream->total_in,stream->total_out);
		printk(KERN_EMERG"[%s] pages[%d]= 0x%x \n",__FUNCTION__,page,opages[page-1]);
#endif

    } while (zlib_err == Z_OK );


    len = stream->total_out;

	 if (zlib_err != Z_STREAM_END) {
        printk("zlib_inflate error, data probably corrupt %d \n",zlib_err);
		len = zlib_err;
        goto release;
    }
    zlib_err = zlib_inflateEnd(stream);
    if (zlib_err != Z_OK) {
		len = zlib_err;
        printk("zlib_inflate error, data probably corrupt\n");
        goto release;
    }
 

release:

	if (stream){
        //kfree(stream->workspace);
        vfree(stream->workspace);
    	kfree(stream);
	}
	
#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s] len %d \n",__FUNCTION__, len);
#endif

    return len;
}


/**
* @brief    gzlib_hw_decompress
* @remarks  The function decompress the data given 
*			using the hw decompressor.
* @return   SUCCESS : length of decompressed data
*           FAILURE : Error code 
*/
static int gzlib_hw_decompress(char *ibuff, int ilength, char *opages[],int npages)
{
	int len = 0;

	//printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	
	/* check for the available HW */
	if(gzlib_hw_busy()){
		//printk(KERN_EMERG"[%s]ERROR: No HW decompressor \n",__FUNCTION__);
		return 0;
	}
	/* Call the HW Decompress */
	//hw_decompress(void *ibuffer, ilength, void *pages[], int npages);

	return len;	
}

/**
* @brief    gzlib_compress
* @remarks  The function compress the data given using the 
*			hw compressor, if the HW compressor is not 
*			available the function then calls the sw compressor
* @return   SUCCESS : length of compressed data
*           FAILURE : Error code 
*/
int gzlib_compress(char *ibuff, int ilength, char *opages[],int npages)
{
	int len = 0;

#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif

	/*Check hw busy status */
	if(hw && !gzlib_hw_busy()){
		len = gzlib_hw_compress(ibuff,ilength,opages,npages);
		if( 0 >= len){
			printk(KERN_EMERG"[%s]Error in HW shifting to SW decompressor \n",__FUNCTION__);
			return gzlib_sw_compress(ibuff,ilength,opages,npages);
		}
	}else{
		/* use sw decompressor */
		return gzlib_sw_compress(ibuff,ilength,opages,npages);
	}
	return len;
}

/**
* @brief    gzlib_decompress
* @remarks  The function decompress the data given using the 
*			hw decompressor, if the HW decompressor is not 
*			available the function then calls the sw decompressor
* @return   SUCCESS : length of decompressed data
*           FAILURE : Error code 
*/
int gzlib_decompress(char *ibuff, int ilength, char *opages[],int npages)
{
	int len = 0;

#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif

	/*Check hw busy status */
	if(hw && !gzlib_hw_busy()){
		len = gzlib_hw_decompress(ibuff,ilength,opages,npages);
		if( 0 >= len){
			printk(KERN_EMERG"[%s]Error in HW shifting to SW decompressor \n",__FUNCTION__);
			return gzlib_sw_decompress(ibuff,ilength,opages,npages);
		}
	}else{
		/* use sw decompressor */
		return gzlib_sw_decompress(ibuff,ilength,opages,npages);
	}
	return len;
}



/**
* @brief    gzM_cd_ops
* @remarks  The host operations those are to be given to the 
*			registered client so that client can call the direct 
*			calls. If the clients are calling the direct call
*			this its client responsibility to check the state 
*			of the hw decompressor or call te gzM_decompress 
*			directly. 
*/
const struct gzM_cd_ops gzM_zlib_ops = {
	.type = GZM_CZLIB,
	.hw = 0,
	.state = GZ_Q_EN,
	.gzM_init = gzlib_init,
	.gzM_sw_compress = gzlib_sw_compress,
	.gzM_sw_decompress = gzlib_sw_decompress,
	.gzM_hw_compress = gzlib_hw_compress,
	.gzM_hw_decompress = gzlib_hw_decompress,
	.gzM_compress = gzlib_compress,
	.gzM_decompress = gzlib_decompress,
	.gzM_hw_busy = gzlib_hw_busy,
	.gzM_release = gzlib_release
};

