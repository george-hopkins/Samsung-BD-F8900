/**
* @file    gzgzip.c
* @brief   Gzip support for the Gunzip Manager to decompress the data 
*          buffers in gzip format  
* Copyright:  Copyright(C) Samsung India Pvt. Ltd 2012. All Rights Reserved.
* @author  SISC: manish.s2
* @date    2012/09/04
* @desc    Gzip decompressor 
* 		   2012/09/04 - Added support for different compressor gzip
*					  - Both hw and sw needs to be initialize here 
*
*/

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/buffer_head.h>
#include <linux/kernel_stat.h>
#include <linux/zlib.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/decompress/inflate.h>
#include "../../lib/zlib_inflate/inflate.h"

#include <linux/gzmanager.h>
#include "gzhost.h"

static int hw = 1;
extern int sdp_unzip_decompress(char *ibuff, int ilength, char *opages[], int npages);
extern atomic_t sdp_busy;

/************** Gunzip Decompressor Functions **********/
#if 0
/**
* @brief    gzM_register_hw
* @remarks  The generic function will register hw compressor 
*           depending upon type of compression
* @args     int ctype
* @return   SUCCESS : gzM_hw
*           FAILURE : Error code 
*/
struct gzM_hw *gzM_register_hw(int ctype)
{
    printk(KERN_EMERG"[%s] Doesn't support hw type %d \n",__FUNCTION__,ctype);
    return NULL;
}

/**
* @brief    gzM_unregister_hw
* @remarks  The generic function will deregister hw compressor 
*           depending upon type of compression
* @args     int ctype
* @return   SUCCESS : gzM_hw
*           FAILURE : Error code 
*/
void gzM_unregister_hw(struct gzM_hw *hw)
{
    printk(KERN_EMERG"[%s]\n",__FUNCTION__);
}
#endif
/**
* @brief    gzip_hw_busy
* @remarks  The function returns the status 
*			of the HW decompressor 
* @return   HW busy : 1
*           HW available : 0
*/
int gzip_hw_busy(void)
{
#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif
	/* Currently HW is always busy */
	return sdp_busy.counter;
}

/**
* @brief    gzip_release
* @remarks  The function free the gzip compressor  
*			memory and data type 
* @return   SUCCESS : 0
*           FAILURE : -1
*/
int gzip_release(void)
{
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}
/**
* @brief    gzip_init
* @remarks  The function initialize the gzip compressor /decompressor 
*			It will initialize the structure's  
* @return   SUCCESS : 0
*           FAILURE : -1
*/
int gzip_init(void)
{
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}

/**
* @brief    gzip_hw_compress
* @remarks  The function compress the data given 
*			on the of hw compressor 
* @return   SUCCESS : length of compressed data
*           FAILURE : Error code
*/
int gzip_hw_compress(char *ibuff, int ilength, char *opages[],int npages)
{

	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}

/**
* @brief    gzip_sw_compress
* @remarks  The function compress the data given 
*			on the cpu core instead of hw compressor 
* @return   SUCCESS : length of compressed data
*           FAILURE : Error code
*/
int gzip_sw_compress(char *ibuff, int ilength, char *opages[],int npages)
{

	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
	return 0;
}

/**
* @brief    gzip_sw_decompress
* @remarks  The function decompress the data given 
*			on the cpu core instead of hw decompressor 
* @return   SUCCESS : length of decompressed data
*           FAILURE : Error code
*/
int gzip_sw_decompress(char *ibuff, int ilength, char *opages[],int npages)
{

	int err =-1;
	int len = 0, page =0;
    z_stream *stream = NULL;

	/* Initialize the zlib stream */
	stream = kmalloc(sizeof(z_stream), GFP_KERNEL);
    if (stream == NULL){
        return -ENOMEM;
	}

	stream->avail_in = 0;
    stream->total_in = 0;
    stream->next_out = NULL;
    stream->avail_out = 0;
    stream->total_out = 0;

	//stream->workspace = kmalloc(zlib_inflate_workspacesize(),GFP_KERNEL);
	stream->workspace = vmalloc(zlib_inflate_workspacesize());
    if (stream->workspace == NULL){
		kfree(stream);
		return -ENOMEM;
    } 

 	/* verify the gzip header */
	if (ilength < 10 ||
	   ibuff[0] != 0x1f || ibuff[1] != 0x8b || ibuff[2] != 0x08) {
		printk(KERN_EMERG"Not a gzip file");
		goto release;
	}

	/* skip over gzip header (1f,8b,08... 10 bytes total +
	 * possible asciz filename)
	 */
	stream->next_in = ibuff + 10;
    stream->avail_in = ilength - 10;
	/* skip over asciz filename */
	if (ibuff[3] & 0x8) {
		printk(KERN_EMERG"skipping the filename avail_in %d \n",stream->avail_in);
		do {
			/*
			 * If the filename doesn't fit into the buffer,
			 * the file is very probably corrupt. Don't try
			 * to read more data.
			 */
			if (stream->avail_in == 0) {
				printk(KERN_EMERG"header error");
				goto release;
			}
			--stream->avail_in;
		} while (*stream->next_in++);
	}


    stream->next_out = opages[page++];
    stream->avail_out = PAGE_CACHE_SIZE;

	err = zlib_inflateInit2(stream, -MAX_WBITS);
    if (err != Z_OK) {
        printk("ERROR: inflateInit (%d)!", err);
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

    	//err = zlib_inflate(stream, Z_FINISH);
    	err = zlib_inflate(stream, 0);
	    if(err == Z_STREAM_END){
 #ifdef GZM_DEBUG
	        printk("[%s] c_size %d total->out %ld \n",__FUNCTION__, ilength,stream->total_out);
			break;
 #endif
		}

#ifdef GZM_DEBUG
		printk(KERN_EMERG"[%s] ######## After Inflate ######### \n",__FUNCTION__);
		printk(KERN_EMERG"[%s] avail_in [%d] avail_out [%d] \n",__FUNCTION__,stream->avail_in,stream->avail_out);
		printk(KERN_EMERG"[%s] next_in 0x%x next_out %#x\n",__FUNCTION__,stream->next_in,stream->next_out);
		printk(KERN_EMERG"[%s] total_in %d total_out %d\n",__FUNCTION__,stream->total_in,stream->total_out);
		printk(KERN_EMERG"[%s] pages[%d]= 0x%x \n",__FUNCTION__,page,opages[page-1]);
#endif

    } while (err == Z_OK );


    len = stream->total_out;

	 if (err != Z_STREAM_END) {
        printk("[%s]inflate error, data probably corrupt %d \n",__FUNCTION__,err);
		len = err;
    }
    err = zlib_inflateEnd(stream);
    if (err != Z_OK) {
		len = err;
        printk("zlib_inflate error, data probably corrupt\n");
    }

release: 

	if (stream){
		if (stream->workspace)
    	    //kfree(stream->workspace);
    	    vfree(stream->workspace);
    	kfree(stream);
	}
	/*
	if (pos)
		// add + 8 to skip over trailer 
		*pos = strm->next_in - zbuf+8;
	*/

#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s] len %d \n",__FUNCTION__, len);
#endif

    return len;
}


/**
* @brief    gzip_hw_decompress
* @remarks  The function decompress the data given 
*			using the hw decompressor.
* @return   SUCCESS : length of decompressed data
*           FAILURE : Error code 
*/
int gzip_hw_decompress(char *ibuff, int ilength, char *opages[],int npages)
{
	int len = 0;
#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif
	/* check for the available HW */
	/* As this is integral part we can use ghost here 
	 * take care of this when adding other compressors */
	if(!hw){
		printk(KERN_EMERG"[%s]ERROR: No HW decompressor \n",__FUNCTION__);
		return 0;
	}
	if(gzip_hw_busy()){
		//printk(KERN_EMERG"[%s]ERROR: HW busy use sw_decompressor \n",__FUNCTION__);
		return 0;
	}
	/* Call the HW Decompress */
	len = sdp_unzip_decompress(ibuff,ilength,opages,npages);
	return len;	
}

/**
* @brief    gzip_compress
* @remarks  The function compress the data given using the 
*			hw compressor, if the HW compressor is not 
*			available the function then calls the sw compressor
* @return   SUCCESS : length of compressed data
*           FAILURE : Error code 
*/
int gzip_compress(char *ibuff, int ilength, char *opages[],int npages)
{
	int len = 0;

#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif

	/*Check hw busy status */
	if(hw && !gzip_hw_busy()){
		len = gzip_hw_compress(ibuff,ilength,opages,npages);
		if( 0 >= len){
			printk(KERN_EMERG"[%s]Error in HW shifting to SW decompressor \n",__FUNCTION__);
			return gzip_sw_compress(ibuff,ilength,opages,npages);
		}
	}else{
		/* use sw decompressor */
		return gzip_sw_compress(ibuff,ilength,opages,npages);
	}
	return len;
}

/**
* @brief    gzip_decompress
* @remarks  The function decompress the data given using the 
*			hw decompressor, if the HW decompressor is not 
*			available the function then calls the sw decompressor
* @return   SUCCESS : length of decompressed data
*           FAILURE : Error code 
*/
int gzip_decompress(char *ibuff, int ilength, char *opages[],int npages)
{
	int len = 0;

#ifdef GZM_DEBUG
	printk(KERN_EMERG"[%s]\n",__FUNCTION__);
#endif

	/*Check hw busy status */
	if(hw && !gzip_hw_busy()){
		len = gzip_hw_decompress(ibuff,ilength,opages,npages);
		if( 0 >= len){
			printk(KERN_EMERG"[%s]Error in HW shifting to SW decompressor \n",__FUNCTION__);
			return gzip_sw_decompress(ibuff,ilength,opages,npages);
		}
	}else{
		/* use sw decompressor */
		return gzip_sw_decompress(ibuff,ilength,opages,npages);
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
const struct gzM_cd_ops gzM_gzip_ops = {
	.type = GZM_CGZIP,
	.hw = 0,
	.state = GZ_Q_EN,
	.gzM_init = gzip_init,
	.gzM_sw_compress = gzip_sw_compress,
	.gzM_sw_decompress = gzip_sw_decompress,
	.gzM_hw_compress = gzip_hw_compress,
	.gzM_hw_decompress = gzip_hw_decompress,
	.gzM_compress = gzip_compress,
	.gzM_decompress = gzip_decompress,
	.gzM_hw_busy = gzip_hw_busy,
	.gzM_release = gzip_release
};



