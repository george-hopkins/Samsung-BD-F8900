/*
 * linux/drivers/video/sdp_fb.c
 *	Copyright (c) Raghav, Vinay
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 *	    SDP83 Frame Buffer Driver
 *	    based on skeletonfb.c, sa1100fb.c and others
 *
 * ChangeLog
 *	- First version
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>


//GP IN DDRB for ATSC/DVB 64BIT_384MB
#define DDRB_BASE_ADDR                  0x70000000

//OSD Plane Physical AddressS
#define SDP83_GFX0_PHYWINBASE  (DDRB_BASE_ADDR+0x03500000)
//GFX Plane Physical AddressS
#define SDP83_GFX1_PHYWINBASE  (DDRB_BASE_ADDR+0x05500000)
//SP Plane Physical AddressS
#define SDP83_GFX2_PHYWINBASE  (DDRB_BASE_ADDR+0x06D00000)

/* FB properties */
//#define SDP83_FB_HOR		1280
//#define SDP83_FB_VER		720
#define SDP83_FB_HOR  1920
#define SDP83_FB_VER  1080
#define SDP83_FB_BPP		32
#define SDP83_FB_32BPP		4
#define SDP83_FB_SIZE         (SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP*NUM_BUFS)

#define NUM_BUFS			2 //android

//#define ENABLE_FB1              FALSE
//#define ENABLE_FB2              FALSE

#define CMN_SCALESTARTPOS0    (0xFE310024) /* (0xFE300024)*/
#define CMN_BASEADDR0          	   (0xFE31001C) /* (0xFE30001C)*/

#define CMN_SCALESTARTPOS1     (0xFE310024)
#define CMN_BASEADDR1          	   (0xFE31001C)

#define CMN_SCALESTARTPOS2     (0xFE311024)
#define CMN_BASEADDR2          	   (0xFE31101C)

#define SDP_FB_SET_MEM_BASE	0x100
#define SDP_FB_LENGTH           7		/*	length of the driver name	*/

typedef enum
{
	/*! normal mode*/
	GFX_NO_FLIP,
	/*! horizontal flip mode*/
	GFX_V_FLIP,
	/*! vertical flip mode*/
	GFX_H_FLIP,
	/*! horizontal & vertical flip mode*/
	GFX_HV_FLIP		
}Gfx_FlipMode_t;


static char driver_gfx0_name[]="sdp_fb0";
static char driver_gfx1_name[]="sdp_fb1";
static char driver_gfx2_name[]="sdp_fb2";

static struct platform_device *sdp_fb0_device;
static struct platform_device *sdp_fb1_device;
static struct platform_device *sdp_fb2_device;

static Gfx_FlipMode_t Gfx0FlipMode = GFX_NO_FLIP;
static Gfx_FlipMode_t Gfx1FlipMode = GFX_NO_FLIP;
static Gfx_FlipMode_t Gfx2FlipMode = GFX_NO_FLIP;

static unsigned int gSdpFbGfx0MemBase = SDP83_GFX0_PHYWINBASE;
static unsigned int gSdpFbGfx1MemBase = SDP83_GFX1_PHYWINBASE;
static unsigned int gSdpFbGfx2MemBase = SDP83_GFX2_PHYWINBASE;


static int sdp_fb_set_var(struct fb_var_screeninfo *pVarInfo)
{
    int ret=0;

    memset(pVarInfo, 0, sizeof(struct fb_var_screeninfo));
    pVarInfo->nonstd = 0;
    pVarInfo->activate = FB_ACTIVATE_NOW;
    pVarInfo->height = SDP83_FB_VER;
    pVarInfo->width = SDP83_FB_HOR;

    pVarInfo->pixclock = 0;
    pVarInfo->left_margin = 0;
    pVarInfo->right_margin = 0;
    pVarInfo->upper_margin = 0;
    pVarInfo->lower_margin = 0;
    pVarInfo->hsync_len = 0;
    pVarInfo->vsync_len = 0;
    pVarInfo->sync = 0;
    
    pVarInfo->vmode = FB_VMODE_NONINTERLACED;

    pVarInfo->xres	 = SDP83_FB_HOR ; 
    pVarInfo->xres_virtual    = SDP83_FB_HOR ; 
    pVarInfo->yres = SDP83_FB_VER ; 
    pVarInfo->yres_virtual    = (SDP83_FB_VER*NUM_BUFS); 
    pVarInfo->bits_per_pixel = SDP83_FB_BPP; 
    
    pVarInfo->xoffset = 0;
    pVarInfo->yoffset = 0;

    pVarInfo->grayscale = 0;

    pVarInfo->transp.length = 8;
    pVarInfo->red.length = 8;
    pVarInfo->green.length = 8;
    pVarInfo->blue.length = 8;

    pVarInfo->transp.offset = 24;
    pVarInfo->red.offset = 16;
    pVarInfo->green.offset = 8;
    pVarInfo->blue.offset = 0;

    
    return ret;
}

static int sdp_fb_set_fix(char *driver_name, struct fb_fix_screeninfo  *pFixInfo)
{
    int ret=0;
    memset(pFixInfo, 0, sizeof(struct fb_fix_screeninfo));
    strncpy(pFixInfo->id, driver_name,SDP_FB_LENGTH);

    pFixInfo->type = FB_TYPE_PACKED_PIXELS;
    pFixInfo->type_aux = 0;
    pFixInfo->visual = FB_VISUAL_DIRECTCOLOR;
    
    pFixInfo->xpanstep = 0;
    pFixInfo->ypanstep = 1; // page filpping
    pFixInfo->ywrapstep = 0;
    
    pFixInfo->line_length = SDP83_FB_HOR*(SDP83_FB_BPP/8);  
    pFixInfo->accel = FB_ACCEL_NONE;
	
    return ret;
}

// supporting page filpping
static int sdp_fb0_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
    unsigned int fb_base;

	/* Change the frame buffer base here */
    if(Gfx0FlipMode == GFX_V_FLIP || Gfx0FlipMode == GFX_HV_FLIP ) //Gfx Flip Case
    {
	if(var->yoffset != 0)
	{
		fb_base = (gSdpFbGfx0MemBase + var->xres*var->yres*SDP83_FB_32BPP*NUM_BUFS);
	}
	else
	{
		fb_base = (gSdpFbGfx0MemBase + var->xres*var->yres*SDP83_FB_32BPP);
	}
    }
    else //Gfx Non Flip Case
    {
	if(var->yoffset != 0)
	{
        	fb_base = (gSdpFbGfx0MemBase + var->xres*var->yres*SDP83_FB_32BPP);
	}
	else
	{
		fb_base = gSdpFbGfx0MemBase;
	}
    }
		
     __raw_writel(0x1, CMN_SCALESTARTPOS0);
     __raw_writel(fb_base, CMN_BASEADDR0);

    return 0; // Success
}


static int sdp_fb0_set_par(struct fb_info *info)
{
	Gfx0FlipMode = info->var.rotate;
	if(info->var.reserved[0] == SDP_FB_SET_MEM_BASE)
	{
		gSdpFbGfx0MemBase = info->var.reserved[1];
		info->fix.smem_start = gSdpFbGfx0MemBase;
		info->screen_base = ioremap(gSdpFbGfx0MemBase,SDP83_FB_SIZE);

		info->var.reserved[0] = 0;
		info->var.reserved[1] = 0;
	}
	return 0;
}

static int sdp_fb0_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{	
	// do nothing
	return 0;
}

static struct fb_ops sdp_fb0_ops = {
	.owner = THIS_MODULE,	
	.fb_pan_display = sdp_fb0_pan_display,
	.fb_check_var	= sdp_fb0_check_var, 
	.fb_set_par = sdp_fb0_set_par,
	
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};

// supporting page filpping
static int sdp_fb1_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
    unsigned int fb_base;

	/* Change the frame buffer base here */
    if(Gfx1FlipMode == GFX_V_FLIP || Gfx1FlipMode == GFX_HV_FLIP ) //Gfx Flip Case
    {
	    if(var->yoffset != 0)
		fb_base = (gSdpFbGfx1MemBase + SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP*NUM_BUFS);
	    else
		fb_base = (gSdpFbGfx1MemBase + SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP);
    }
    else //Gfx Non Flip Case
    {
	    if(var->yoffset != 0)
		fb_base = (gSdpFbGfx1MemBase + SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP);
	    else
		fb_base = gSdpFbGfx1MemBase;
    }
		
     __raw_writel(0x1, CMN_SCALESTARTPOS1);
     __raw_writel(fb_base, CMN_BASEADDR1);

    return 0; // Success
}

static int sdp_fb1_set_par(struct fb_info *info)
{
	Gfx1FlipMode = info->var.rotate;
	if(info->var.reserved[0] == SDP_FB_SET_MEM_BASE)
	{
		gSdpFbGfx1MemBase = info->var.reserved[1];
		info->fix.smem_start = gSdpFbGfx1MemBase;
		info->screen_base = ioremap(gSdpFbGfx1MemBase,SDP83_FB_SIZE);

		info->var.reserved[0] = 0;
		info->var.reserved[1] = 0;
	}
	return 0;
}

static int sdp_fb1_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{	
	// do nothing
	return 0;
}

static struct fb_ops sdp_fb1_ops = {
	.owner = THIS_MODULE,
	.fb_pan_display = sdp_fb1_pan_display,
	.fb_check_var	= sdp_fb1_check_var, 
	.fb_set_par = sdp_fb1_set_par,
	
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};

// supporting page filpping
static int sdp_fb2_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
    unsigned int fb_base;

	/* Change the frame buffer base here */
    if(Gfx2FlipMode == GFX_V_FLIP || Gfx2FlipMode == GFX_HV_FLIP ) //Gfx Flip Case
    {
	    if(var->yoffset != 0)
		fb_base = (gSdpFbGfx2MemBase + SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP*NUM_BUFS);
	    else
		fb_base = (gSdpFbGfx2MemBase + SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP);
    }
    else //Gfx Non Flip Case
    {
	    if(var->yoffset != 0)
		fb_base = (gSdpFbGfx2MemBase + SDP83_FB_HOR*SDP83_FB_VER*SDP83_FB_32BPP);
	    else
		fb_base = gSdpFbGfx2MemBase;
    }
		
     __raw_writel(0x1, CMN_SCALESTARTPOS2);
     __raw_writel(fb_base, CMN_BASEADDR2);

    return 0; // Success
}


static int sdp_fb2_set_par(struct fb_info *info)
{
	Gfx2FlipMode = info->var.rotate;
	if(info->var.reserved[0] == SDP_FB_SET_MEM_BASE)
	{
		gSdpFbGfx2MemBase = info->var.reserved[1];
		info->fix.smem_start = gSdpFbGfx2MemBase;
		info->screen_base = ioremap(gSdpFbGfx2MemBase,SDP83_FB_SIZE);

		info->var.reserved[0] = 0;
		info->var.reserved[1] = 0;
	}
	return 0;
}

static int sdp_fb2_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{	
	// do nothing
	return 0;
}

static struct fb_ops sdp_fb2_ops = {
	.owner = THIS_MODULE,	
	.fb_pan_display = sdp_fb2_pan_display,
	.fb_check_var	= sdp_fb2_check_var, 
	.fb_set_par = sdp_fb2_set_par,
	
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};
 
static int __init sdp_fb0_probe(struct platform_device *pdev)
{
    int ret=0;
    struct fb_info  *pfbinfo;

    pfbinfo = framebuffer_alloc(sizeof(struct fb_info), &pdev->dev);
    if (!pfbinfo) 
    {
        printk(KERN_ERR "SDP FB : frame Buffer allocation Fails\n");
        return -ENOMEM;
    }
    platform_set_drvdata(pdev, pfbinfo);

    sdp_fb_set_var((struct fb_var_screeninfo *)&pfbinfo->var);
    sdp_fb_set_fix(driver_gfx0_name, (struct fb_fix_screeninfo *)&pfbinfo->fix);

    pfbinfo->fix.smem_start = gSdpFbGfx0MemBase;
    pfbinfo->fix.smem_len = SDP83_FB_SIZE;

    pfbinfo->fbops = &sdp_fb0_ops;
    pfbinfo->flags = FBINFO_FLAG_DEFAULT;

    /*pfbinfo->screen_base = ioremap(gSdpFbGfx0MemBase,SDP83_FB_SIZE);

    if (!pfbinfo->screen_base) 
    {
        printk(KERN_ERR "SDP FB : abort, cannot ioremap video memory 0x%x @ 0x%lx\n",
        	pfbinfo->fix.smem_len, pfbinfo->fix.smem_start);
        ret = -EIO;
        goto dealloc_fb;
    }

    printk(KERN_INFO "SDP  FB: framebuffer at 0x%lx, mapped to 0x%p, \n ",
    	pfbinfo->fix.smem_start, pfbinfo->screen_base);
*/
	printk(KERN_INFO "SDP didnt do ioremap\n");
    ret = register_framebuffer(pfbinfo);
    if (ret < 0) 
    {
        printk(KERN_ERR "Failed to register framebuffer device: %d\n", ret);
        goto dealloc_fb;
    }

    printk(KERN_INFO "fb%d: %s frame buffer device\n",
    	pfbinfo->node, pfbinfo->fix.id);

    return ret;

    dealloc_fb:
        framebuffer_release(pfbinfo);

    return ret;
}

/*
 *  Cleanup
 */

static int sdp_fb0_remove(struct platform_device *pdev)
{
    struct fb_info  *pfbinfo = platform_get_drvdata(pdev);
    if (pfbinfo) 
    {
        unregister_framebuffer(pfbinfo);
        framebuffer_release(pfbinfo);
    }
    return 0;
}

//fb1
static int __init sdp_fb1_probe(struct platform_device *pdev)
{
    int ret=0;
    struct fb_info  *pfbinfo;

    pfbinfo = framebuffer_alloc(sizeof(struct fb_info), &pdev->dev);
    if (!pfbinfo) 
    {
        printk(KERN_ERR "SDP FB : frame Buffer allocation Fails\n");
        return -ENOMEM;
    }
    platform_set_drvdata(pdev, pfbinfo);

    sdp_fb_set_var((struct fb_var_screeninfo *)&pfbinfo->var);
    sdp_fb_set_fix(driver_gfx1_name, (struct fb_fix_screeninfo *)&pfbinfo->fix);

    pfbinfo->fix.smem_start =  gSdpFbGfx1MemBase;
    pfbinfo->fix.smem_len =  SDP83_FB_SIZE;

    pfbinfo->fbops	= &sdp_fb1_ops;
    pfbinfo->flags = FBINFO_FLAG_DEFAULT;
/*
    pfbinfo->screen_base = ioremap(gSdpFbGfx1MemBase,SDP83_FB_SIZE);

    if (!pfbinfo->screen_base) 
    {
        printk(KERN_ERR "SDP FB : abort, cannot ioremap video memory 0x%x @ 0x%lx\n",
        pfbinfo->fix.smem_len, pfbinfo->fix.smem_start);
        ret = -EIO;
        goto dealloc_fb;
    }

    printk(KERN_INFO "SDP  FB: framebuffer at 0x%lx, mapped to 0x%p, \n ",
    pfbinfo->fix.smem_start, pfbinfo->screen_base);
*/
    printk(KERN_INFO "SDP didnt do ioremap\n");
    ret = register_framebuffer(pfbinfo);
    if (ret < 0) 
    {
        printk(KERN_ERR "Failed to register framebuffer device: %d\n", ret);
        goto dealloc_fb;
    }

    printk(KERN_INFO "fb%d: %s frame buffer device\n",
    pfbinfo->node, pfbinfo->fix.id);

    return ret;

    dealloc_fb:
        framebuffer_release(pfbinfo);

    return ret;
}

/*
 *  Cleanup
 */
static int sdp_fb1_remove(struct platform_device *pdev)
{
    struct fb_info  *pfbinfo = platform_get_drvdata(pdev);
    if (pfbinfo) 
    {
        unregister_framebuffer(pfbinfo);
        framebuffer_release(pfbinfo);
    }
    return 0;
}

//fb2
static int __init sdp_fb2_probe(struct platform_device *pdev)
{
    int ret=0;
    struct fb_info  *pfbinfo;

    pfbinfo = framebuffer_alloc(sizeof(struct fb_info), &pdev->dev);
    if (!pfbinfo) 
    {
        printk(KERN_ERR "SDP FB : frame Buffer allocation Fails\n");
        return -ENOMEM;
    }
    platform_set_drvdata(pdev, pfbinfo);

    sdp_fb_set_var((struct fb_var_screeninfo *)&pfbinfo->var);
    sdp_fb_set_fix(driver_gfx2_name, (struct fb_fix_screeninfo *)&pfbinfo->fix);

    pfbinfo->fix.smem_start =  gSdpFbGfx2MemBase;
    pfbinfo->fix.smem_len =  SDP83_FB_SIZE;

    pfbinfo->fbops	= &sdp_fb2_ops;
    pfbinfo->flags = FBINFO_FLAG_DEFAULT;
/*
    pfbinfo->screen_base = ioremap(gSdpFbGfx2MemBase,SDP83_FB_SIZE);

    if (!pfbinfo->screen_base) 
    {
        printk(KERN_ERR "SDP FB : abort, cannot ioremap video memory 0x%x @ 0x%lx\n",
        pfbinfo->fix.smem_len, pfbinfo->fix.smem_start);
        ret = -EIO;
        goto dealloc_fb;
    }

    printk(KERN_INFO "SDP  FB: framebuffer at 0x%lx, mapped to 0x%p, \n ",
    pfbinfo->fix.smem_start, pfbinfo->screen_base);
*/
    printk(KERN_INFO "SDP didnt do ioremap\n");
    ret = register_framebuffer(pfbinfo);
    if (ret < 0) 
    {
        printk(KERN_ERR "Failed to register framebuffer device: %d\n", ret);
        goto dealloc_fb;
    }

    printk(KERN_INFO "fb%d: %s frame buffer device\n",
    pfbinfo->node, pfbinfo->fix.id);

    return ret;

    dealloc_fb:
        framebuffer_release(pfbinfo);

    return ret;
}

/*
 *  Cleanup
 */
static int sdp_fb2_remove(struct platform_device *pdev)
{
    struct fb_info  *pfbinfo = platform_get_drvdata(pdev);
    if (pfbinfo) 
    {
        unregister_framebuffer(pfbinfo);
        framebuffer_release(pfbinfo);
    }
    return 0;
}

static struct platform_driver __refdata sdp_fb0_driver = {
	.probe = sdp_fb0_probe,
	.remove = sdp_fb0_remove,
	.driver = {
	.name = "sdp -GfxFB0",
	.owner = THIS_MODULE,
	},
};

static struct platform_driver __refdata sdp_fb1_driver = {
	.probe = sdp_fb1_probe,
	.remove = sdp_fb1_remove,
	.driver = {
	.name = "sdp -GfxFB1",
	.owner = THIS_MODULE,
	},
};

static struct platform_driver __refdata sdp_fb2_driver = {
	.probe = sdp_fb2_probe,
	.remove = sdp_fb2_remove,
	.driver = {
	.name = "sdp -GfxFB2",
	.owner = THIS_MODULE,
	},
};



int __init sdp_fb_init(void)
{
    int ret=0;

    //FB0
    ret = platform_driver_register(&sdp_fb0_driver);
    if(!ret)
    {
         sdp_fb0_device = platform_device_alloc("sdp -GfxFB0", 0);

        if (sdp_fb0_device)
            ret = platform_device_add(sdp_fb0_device);
        else
            ret = -ENOMEM;

        if (ret) 
        {
            platform_device_put(sdp_fb0_device);
            platform_driver_unregister(&sdp_fb0_driver);
        }

    }
    
    //FB1
    ret = platform_driver_register(&sdp_fb1_driver);
    if(!ret)
    {
         sdp_fb1_device = platform_device_alloc("sdp -GfxFB1", 0);

        if (sdp_fb1_device)
            ret = platform_device_add(sdp_fb1_device);
        else
            ret = -ENOMEM;

        if (ret) 
        {
            platform_device_put(sdp_fb1_device);
            platform_driver_unregister(&sdp_fb1_driver);
        }

    }

    //FB2
    ret = platform_driver_register(&sdp_fb2_driver);
    if(!ret)
    {
         sdp_fb2_device = platform_device_alloc("sdp -GfxFB2", 0);

        if (sdp_fb2_device)
            ret = platform_device_add(sdp_fb2_device);
        else
            ret = -ENOMEM;

        if (ret) 
        {
            platform_device_put(sdp_fb2_device);
            platform_driver_unregister(&sdp_fb2_driver);
        }

    }

    return ret;
}

static void __exit sdp_fb_cleanup(void)
{
    platform_device_unregister(sdp_fb0_device);
    platform_driver_unregister(&sdp_fb0_driver);

    platform_device_unregister(sdp_fb1_device);
    platform_driver_unregister(&sdp_fb1_driver); 

    platform_device_unregister(sdp_fb2_device);
    platform_driver_unregister(&sdp_fb2_driver); 
}

module_init(sdp_fb_init);
module_exit(sdp_fb_cleanup);


MODULE_AUTHOR("raghav, vinay @Samsung");
MODULE_DESCRIPTION("Framebuffer driver for the DTV ");
MODULE_LICENSE("GPL");
