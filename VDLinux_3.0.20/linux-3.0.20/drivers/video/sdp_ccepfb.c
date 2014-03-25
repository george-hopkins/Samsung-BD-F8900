/*
 * arch/arm/plat-ccep/video/sdp_ccepfb.c
 *	Created by tukho.kim@samsung.com
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 *	    SDP OSD/GP/SubGp Frame buffer
 *	    based on skeletonfb.c and others
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

#include "sdp_ccepfb.h"

#define DEMO_JUMP

static char *mode_option  __devinitdata;

#define DBG_CCEPFB(fmt,args...) 	// printk("[%s]: " fmt, __FUNCTION__, ##args)
#define ERR_CCEPFB(fmt,args...) 	printk(KERN_ERR"[%s]: " fmt, __FUNCTION__, ##args)
#define INFO_CCEPFB(fmt,args...) 	printk(KERN_INFO"[%s]: " fmt, __FUNCTION__, ##args)

#define SDP83_FB_HOR 				1920
#define SDP83_FB_VER                           1080
#define SDP83_FB_32BPP				4
#define NUM_BUFS                                  2

#define CONFIG_HV_FLIP_MODE

static u8 ccepfb_initcon_disable = 0;		// 1 - disable, 0 - enable

/* FB properties */
#define SDP_CCEPFB_BPP				32


#define CCEPFB_WRITEL(v,a) 			writel(v,a)
#define CCEPFB_READL(a) 			readl(a)

#ifdef CONFIG_ARCH_SDP1202 
#define REGISTER_ADDR_CONV(a)		(((a) & 0x00FFFFFF) | 0xFE000000)
#define REGISTER_WRITEL(v,a)		writel(v,(((a) & 0x00FFFFFF) | 0xFE000000))
#define REGISTER_READL(a)  			readl(((a) & 0x00FFFFFF) | 0xFE000000)
#else /* Fox B*/ 
#define REGISTER_ADDR_CONV(a)		(((a) & 0x00FFFFFF) | 0xd1000000)
#define REGISTER_WRITEL(v,a)		writel(v,(((a) & 0x00FFFFFF) | 0xd1000000))
#define REGISTER_READL(a)  			readl(((a) & 0x00FFFFFF) | 0xd1000000)
#endif

extern unsigned int sdp_sys_mem0_size ;
extern unsigned int sdp_sys_mem1_size ;
extern unsigned int sdp_sys_mem2_size ;

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
static Gfx_FlipMode_t GfxFlipMode = GFX_NO_FLIP;

typedef enum {
	LAYER_OSD = 0,	// osd plane
	LAYER_GP,	// graphic plane
	LAYER_SGP,	// sub-graphic plane
	LAST_LAYER
}FB_LAYER_T;

#ifdef CONFIG_ARCH_SDP1202 
typedef enum
{
     GFX_VIEW_SINGLE,
     GFX_VIEW_DUAL,
     GFX_VIEW_TRIPLE,
     GFX_VIEW_QUAD,
     GFX_VIEW_MAX,
}spIGfx_ViewMode;

typedef enum _Gfx_StereoScopicMode_t
{
	GFX_STEREO_OFF,   /*< StereoScopic mode off*/
	GFX_STEREO_2D_TO_3D,
	GFX_STEREO_SIDEBYSIDE,
	GFX_STEREO_TOP_BOTTOM,
	GFX_STEREO_FRAME_SEQUENTIAL,
	GFX_STEREO_FRAME_PACKING,
	GFX_STEREO_MALI_SIDEBYSIDE,
	GFX_STEREO_MALI_TOP_BOTTOM,
	GFX_STEREO_MALI_FRAME_SEQUENTIAL,
	GFX_STEREO_MALI_FRAME_PACKING,
	GFX_STEREO_MAX
} Gfx_StereoScopicMode_t, *pGfx_StereoScopicMode_t;
#endif

#ifdef CONFIG_ARCH_SDP1202 
static u32 g_sdp_ccepfb_membase;
#elif  CONFIG_ARCH_SDP1207 
static u32 g_sdp_ccepfb_membase[LAST_LAYER];
#endif

static struct fb_info * sdp_fbinfo[LAST_LAYER];

#ifdef CONFIG_ARCH_SDP1106
#define SDP_CCEPFB_MEMBASE (0xA0000000 + (436 << 20))
#elif CONFIG_ARCH_SDP1202 
u32 SDP_CCEPFB_MEMBASE; 
#elif CONFIG_ARCH_SDP1207 // FoxB
u32 SDP_CCEPFB_MEMBASE[LAST_LAYER]; 
#else
#define SDP_CCEPFB_MEMBASE (0x57000000)
#endif

#ifdef MB
#undef MB
#endif

#define MB(a) 	((a) << 20)

#if CONFIG_ARCH_SDP1202
extern int sdp_get_mem_cfg(int nType);
#endif



typedef struct {
	char name[sizeof(sdp_fbinfo[0]->fix.id)];  //fb_info -> id
	u16	  x;  		// width,  horizontal
	u16	  y;		// height, vertical
	u32   size;		// memory size
	u32	  offset;	// from base gp plane address
	u32   regbase;
}SDP_CCEPFB_INFO_T; 

static const SDP_CCEPFB_INFO_T sdp_ccepfb_info[LAST_LAYER] = {
#ifdef CONFIG_ARCH_SDP1202
     	{"sdp_osdfb", 1920, 1080, MB(32), MB(0), A_OSD_BASE},	 // OSD
	 {"sdp_mgpfb", 1920, 1080, MB(32), MB(32), A_GP_BASE},  // GP
	 {"sdp_sgpfb", 1920, 1080, MB(32), MB(32+32), A_SUBGP_BASE},	 // SP
#else     //FoxB
 	 {"sdp_osdfb", 1280, 720, MB(8), LAYER_OSD, A_OSD_BASE},	 // OSD
	 {"sdp_mgpfb", 1920, 1080, MB(16), LAYER_GP, A_GP_BASE},  // GP
	 {"sdp_sgpfb",  960,  540, MB(4),  LAYER_SGP, A_SUBGP_BASE},	 // SP
#endif

	};

static const SDP_CCEPFB_INFO_T sdp_ccepfb_info_UD[LAST_LAYER] = {
#ifdef CONFIG_ARCH_SDP1202
     	{"sdp_osdfb", 1920, 1080, MB(64), MB(0), A_OSD_BASE},	 // OSD
	 {"sdp_mgpfb", 1920, 1080, MB(64), MB(64), A_GP_BASE},  // GP
	 {"sdp_sgpfb", 1920, 1080, MB(64), MB(64+64), A_SUBGP_BASE},	 // SP
#else     //FoxB
 	 {"sdp_osdfb", 1280, 720, MB(8), LAYER_OSD, A_OSD_BASE},	 // OSD
	 {"sdp_mgpfb", 1920, 1080, MB(16), LAYER_GP, A_GP_BASE},  // GP
	 {"sdp_sgpfb",  960,  540, MB(4),  LAYER_SGP, A_SUBGP_BASE},	 // SP
#endif

	};

#ifdef CONFIG_FRAMEBUFFER_CONSOLE
#ifdef CONFIG_ARCH_SDP1202 // 1202 support Full HD sub gp plane
static const SDP_CCEPFB_INFO_T sdp_ccepfb_info_con[LAST_LAYER] = {
	 {"sdp_osdfb", 640, 1080, MB(32), MB(0), A_OSD_BASE},	 // OSD
	 {"sdp_mgpfb", 1920, 1080, MB(32), MB(32), A_GP_BASE},  // GP
	 {"sdp_sgpfb", 1920, 1080, MB(32), MB(32+32), A_SUBGP_BASE},	 // SP
};
static const SDP_CCEPFB_INFO_T sdp_ccepfb_info_con_UD[LAST_LAYER] = {
	 {"sdp_osdfb", 640, 1080, MB(32), MB(0), A_OSD_BASE},	 // OSD
	 {"sdp_mgpfb", 1920, 1080, MB(64), MB(32), A_GP_BASE},  // GP
	 {"sdp_sgpfb", 1920, 1080, MB(32), MB(32+64), A_SUBGP_BASE},	 // SP
};
#endif
#endif

#undef MB

typedef struct {
// h/w resource
	u32    		regbase; 
	u32    		osd_regbase; 

	u32    		phy_membase;		// fb memory physical base
	u32    		vir_membase;		// fb memory virtual base
	FB_LAYER_T 	layer;


// kernel fb subsystem resource
	const char * name;
	struct 		platform_device * pdev; 
	struct 		fb_info * p_fbinfo;

	u32   		pseudo_palette[256];
}SDP_CCEPFB_T;

static int ccepfb_setcolreg(unsigned regno, unsigned red, unsigned green,
			     unsigned blue, unsigned transp,
			     struct fb_info *info)
{

    /* grayscale works only partially under directcolor */
    if (info->var.grayscale) {
       /* grayscale = 0.30*R + 0.59*G + 0.11*B */
       red = green = blue = (red * 77 + green * 151 + blue * 28) >> 8;
    }

#define CNVT_TOHW(val,width) ((((val)<<(width))+0x7FFF-(val))>>16)
    red = CNVT_TOHW(red, info->var.red.length);
    green = CNVT_TOHW(green, info->var.green.length);
    blue = CNVT_TOHW(blue, info->var.blue.length);
    transp = CNVT_TOHW(transp, info->var.transp.length);
#undef CNVT_TOHW

    if (info->fix.visual == FB_VISUAL_TRUECOLOR ||
        info->fix.visual == FB_VISUAL_DIRECTCOLOR) {
            u32 v;

            if (regno >= 16)
                    return -EINVAL;

            v = (red << info->var.red.offset) |
                    (green << info->var.green.offset) |
                    (blue << info->var.blue.offset) |
                    (transp << info->var.transp.offset);

            ((u32*)(info->pseudo_palette))[regno] = v;
    }

    /* ... */
    return 0;
}


// supporting page filpping
static int ccepfb_set_par(struct fb_info *p_fbinfo)
{
	SDP_CCEPFB_T * p_ccepfb;
	u32 regval;

	DBG_CCEPFB("\n");

	if(!p_fbinfo) return -1;
	GfxFlipMode = p_fbinfo->var.rotate;
	
	p_ccepfb = (SDP_CCEPFB_T*)p_fbinfo->par;
  
#ifdef CONFIG_FRAMEBUFFER_CONSOLE 
	if(!ccepfb_initcon_disable) {
		regval = CCEPFB_READL(p_ccepfb->regbase + O_DP_MODE) | 0x01000011;	// mixer on, display on, scaler on
#if 1 // FLIP mode
		if(GfxFlipMode == GFX_HV_FLIP){
			regval = regval | 0x00000300;
		}
#endif 
		CCEPFB_WRITEL(regval, p_ccepfb->regbase + O_DP_MODE);
	}
#endif

#ifdef CONFIG_ARCH_SDP1202
	CCEPFB_WRITEL(0x1, p_ccepfb->regbase + O_H_START_PIXEL);
#else
        //CCEPFB_WRITEL(0x1, p_ccepfb->regbase + O_H_START_PIXEL);
#endif

    return 0; // Success
}


#ifdef CONFIG_ARCH_SDP1202
Gfx_StereoScopicMode_t GfxSteroModePri = GFX_STEREO_OFF;
Gfx_StereoScopicMode_t GfxSteroModeSnd = GFX_STEREO_OFF;
spIGfx_ViewMode GfxViewMode = GFX_VIEW_SINGLE;
EXPORT_SYMBOL(GfxSteroModePri);
EXPORT_SYMBOL(GfxSteroModeSnd);
EXPORT_SYMBOL(GfxViewMode);
#endif


// supporting page filpping
static int ccepfb_pan_display(struct fb_var_screeninfo *p_varinfo, struct fb_info *p_fbinfo)
{
	SDP_CCEPFB_T * p_ccepfb;
    u32 phy_membase;
	u32 regval;
	u32 xpos = FLIP_X_POS, ypos = FLIP_Y_POS; // default fixel
	u32 ud_yres_virtual=0;

	DBG_CCEPFB("\n");

	if(!p_fbinfo || !p_varinfo) {
		 ERR_CCEPFB("p_fbinfo or p_varinfo is NULL\n");
		 return -1;
	}

	p_ccepfb = (SDP_CCEPFB_T*)p_fbinfo->par;

	phy_membase = p_fbinfo->fix.smem_start;

#ifdef CONFIG_FRAMEBUFFER_CONSOLE 
	if(!ccepfb_initcon_disable) {
// only support 1:1 scale
   		CCEPFB_WRITEL(0x10000, p_ccepfb->regbase + O_H_SCALE_RATIO);	//
   		CCEPFB_WRITEL(0x10000, p_ccepfb->regbase + O_V_SCALE_RATIO);	//
    	CCEPFB_WRITEL(0x00210, p_ccepfb->regbase + O_PLANE_REORDER);	//
    	CCEPFB_WRITEL(0x00000, p_ccepfb->regbase + O_CENTER_POS_OFFSET); //
    	CCEPFB_WRITEL(0x0000A, p_ccepfb->regbase + O_TH_DP_LINE);		//

		regval = (p_varinfo->xres << 16) | p_varinfo->yres;
   	 	CCEPFB_WRITEL(regval, p_ccepfb->regbase + O_INPUT_SIZE);	// base address
	    CCEPFB_WRITEL(regval, p_ccepfb->regbase + O_OUTPUT_SIZE);	// base address

		regval = p_varinfo->xres >> 2;
		CCEPFB_WRITEL(regval, p_ccepfb->regbase + O_H_OFFSET_ADDR);

		if(GfxFlipMode == GFX_HV_FLIP) {
			xpos = 1920 + xpos - p_varinfo->xres;
			ypos = 1080 + ypos - p_varinfo->yres;  // test
			phy_membase += p_varinfo->xres * p_varinfo->yres * (p_varinfo->bits_per_pixel / 8);
		}

		regval = (p_varinfo->yres << 16) | p_varinfo->xres;

		switch (p_ccepfb->layer){
			case (LAYER_OSD):
				CCEPFB_WRITEL(regval,     p_ccepfb->osd_regbase + O_OSD_OSD_HV_SIZE);
				CCEPFB_WRITEL(0x00020080, p_ccepfb->osd_regbase + O_OSD_OSD_BLEND);
				CCEPFB_WRITEL((xpos | (ypos << 16)), p_ccepfb->osd_regbase + O_OSD_OSD_HV_START);
				break;
			case (LAYER_GP):
				CCEPFB_WRITEL(regval, 	  p_ccepfb->osd_regbase + O_OSD_GP_HV_SIZE);
				CCEPFB_WRITEL(0x00020080, p_ccepfb->osd_regbase + O_OSD_GP_BLEND);
				CCEPFB_WRITEL((xpos | (ypos << 16)), p_ccepfb->osd_regbase + O_OSD_GP_HV_START);
    			CCEPFB_WRITEL((CCEPFB_READL( p_ccepfb->regbase + O_DP_MODE)|0x11), p_ccepfb->regbase + O_DP_MODE);		//
				break;
			case (LAYER_SGP):
				CCEPFB_WRITEL(regval, 	  p_ccepfb->osd_regbase + O_OSD_SUBGP_HV_SIZE);
				CCEPFB_WRITEL(0x000200FF, p_ccepfb->osd_regbase + O_OSD_SUBGP_BLEND);
				CCEPFB_WRITEL((xpos | (ypos << 16)), p_ccepfb->osd_regbase + O_OSD_SUBGP_HV_START);
				break;
			default:
				break;
		}

		CCEPFB_WRITEL(0x04650898, p_ccepfb->osd_regbase + 0x84);   		// Raster size
#ifdef MAIN_WIN_POS
		CCEPFB_WRITEL(MAIN_WIN_POS, p_ccepfb->osd_regbase + O_OSD_MAIN_WIN_POS);   		// Raster size
#endif
	} 
	else 
#endif
	{

		
#ifdef CONFIG_ARCH_SDP1202
		unsigned int system_mem_size=0;
		system_mem_size = sdp_get_mem_cfg(1);

		if(((system_mem_size>>20)==2048)&&(GfxViewMode==GFX_VIEW_DUAL))//NTV Case
		{
			if(GfxFlipMode == GFX_HV_FLIP ) //Gfx Flip Case
			{
				if((GfxSteroModePri==GFX_STEREO_FRAME_PACKING)||(GfxSteroModeSnd==GFX_STEREO_FRAME_PACKING))
				{
					ud_yres_virtual = p_varinfo->yres;
				}
				else
				{
					ud_yres_virtual = p_varinfo->yres/2;
				}
			
				if(p_varinfo->yoffset == p_varinfo->yres) 
				{
					 phy_membase = (phy_membase  + (p_varinfo->xres *p_varinfo->yres* SDP83_FB_32BPP * NUM_BUFS)- (p_varinfo->xres*ud_yres_virtual*SDP83_FB_32BPP)-((p_varinfo->xres/2)*SDP83_FB_32BPP));
				}
				else
				{
					phy_membase = (phy_membase  + (p_varinfo->xres *p_varinfo->yres* SDP83_FB_32BPP)-(p_varinfo->xres *ud_yres_virtual* SDP83_FB_32BPP)-((p_varinfo->xres/2)*SDP83_FB_32BPP));
					
				}
			}
			else if(GfxFlipMode == GFX_V_FLIP) //Gfx Flip Case
			{
				if((GfxSteroModePri==GFX_STEREO_FRAME_PACKING)||(GfxSteroModeSnd==GFX_STEREO_FRAME_PACKING))
				{
					ud_yres_virtual = 0;
				}
				else
				{
					ud_yres_virtual = p_varinfo->yres/2;
				}
			
				if(p_varinfo->yoffset == p_varinfo->yres) 
				{
					phy_membase = (phy_membase  + 0x1e00 + (p_varinfo->xres *p_varinfo->yres* SDP83_FB_32BPP * NUM_BUFS)- (p_varinfo->xres*ud_yres_virtual*SDP83_FB_32BPP)-((p_varinfo->xres/2)*SDP83_FB_32BPP));
				}
				else
				{
					phy_membase = (phy_membase  + 0x1e00 + (p_varinfo->xres *p_varinfo->yres* SDP83_FB_32BPP)-(p_varinfo->xres *ud_yres_virtual* SDP83_FB_32BPP)-((p_varinfo->xres/2)*SDP83_FB_32BPP));
					
				}
			}
			else //Gfx Non Flip Case
			{
				if(p_varinfo->yoffset == p_varinfo->yres)
					phy_membase = (phy_membase + (p_varinfo->xres*p_varinfo->yres*SDP83_FB_32BPP));
				else
					phy_membase = phy_membase;
			}	
		}

		else
		{	//Basic Case
		   	if(GfxFlipMode == GFX_V_FLIP || GfxFlipMode == GFX_HV_FLIP ) //Gfx Flip Case
		    {
				if(p_varinfo->yoffset == p_varinfo->yres) 
			    {
					phy_membase = (phy_membase + (p_varinfo->xres*p_varinfo->yres*SDP83_FB_32BPP * NUM_BUFS));
				}
		        else
		        {
					phy_membase = (phy_membase + (p_varinfo->xres * p_varinfo->yres * SDP83_FB_32BPP));
		        }
		    }
		    else //Gfx Non Flip Case
		    {
				if(p_varinfo->yoffset == p_varinfo->yres)
		        	phy_membase = (phy_membase + (p_varinfo->xres*p_varinfo->yres*SDP83_FB_32BPP));
		       	else
		            phy_membase = phy_membase;
		    }	
		}
#else
		{	//Basic Case
	   		if(GfxFlipMode == GFX_V_FLIP || GfxFlipMode == GFX_HV_FLIP ) //Gfx Flip Case
	    	{
				if(p_varinfo->yoffset == p_varinfo->yres) 
				{
					phy_membase = (phy_membase + (p_varinfo->xres*p_varinfo->yres*SDP83_FB_32BPP * NUM_BUFS));
				}
				else
				{
					phy_membase = (phy_membase + (p_varinfo->xres * p_varinfo->yres * SDP83_FB_32BPP));
				}
			}
			else //Gfx Non Flip Case
			{
				if(p_varinfo->yoffset == p_varinfo->yres)
					phy_membase = (phy_membase + (p_varinfo->xres*p_varinfo->yres*SDP83_FB_32BPP));
				else
					phy_membase = phy_membase;
			}	
		}
#endif
	}

    CCEPFB_WRITEL(phy_membase, p_ccepfb->regbase + O_BASE_ADDR);	// base address 

#ifdef CONFIG_ARCH_SDP1207//FoxB
	unsigned int val;
	val = CCEPFB_READL(p_ccepfb->regbase + O_BASE_ADDR);	// base address     
	CCEPFB_WRITEL(0x1, p_ccepfb->regbase + O_OSD_UPDATE);	// base address 
#endif

    return 0; // Success
}


/****************************************************************************************
 *  check fb info variable function, what implement in this function? 
 ****************************************************************************************/

static int ccepfb_check_var(struct fb_var_screeninfo *p_varinfo, struct fb_info *p_fbinfo)
{	
	DBG_CCEPFB("\n");
	
	DBG_CCEPFB("width: %d, height: %d\n", p_varinfo->width, p_varinfo->height);
	DBG_CCEPFB("xres: %d, yres: %d\n", p_varinfo->xres, p_varinfo->yres);

	/*
 *	width, height -> output device resolution 
 *	xres, yres -> source screen resolution 
 * */	

// TODO: 

	return 0;
}

static inline void temp_display_init(void)
{
#if 1 //defined(DEMO_JUMP) 
#   ifdef CONFIG_ARCH_SDP1106
	REGISTER_WRITEL(2200   , 0x30910114);
	REGISTER_WRITEL(1125   , 0x30910118);
	REGISTER_WRITEL(2200   , 0x30220008);
	REGISTER_WRITEL(1920   , 0x30220000);
	REGISTER_WRITEL(1080   , 0x30220004);
	REGISTER_WRITEL(1920   , 0x3091011C);
	REGISTER_WRITEL(1080   , 0x30910120);
	REGISTER_WRITEL(14     , 0x309101D4);
	REGISTER_WRITEL(150    , 0x30910190);
	REGISTER_WRITEL(15     , 0x30910194);
	REGISTER_WRITEL(44     , 0x309101B8);
	REGISTER_WRITEL(4      , 0x309101BC);
	REGISTER_WRITEL(5      , 0x30220F1C);
	REGISTER_WRITEL(0x1011 , 0x30910000);
	REGISTER_WRITEL(1      , 0x30910004);
	REGISTER_WRITEL(0      , 0x30910084);
	REGISTER_WRITEL(1 	   , 0x30220E00);    //OSD plain active
#	elif CONFIG_ARCH_SDP1202
	void __iomem * base;
	u32 value;
	
	base = ioremap(0x18200000, (4 << 10));
	if(!base) { ERR_CCEPFB("DP 1st init ioremap failed\n"); return; } 
	CCEPFB_WRITEL(0x1	   , base + 0x0E00);
	CCEPFB_WRITEL(0x100	   , base + 0x0E1C);
	iounmap (base);
	base = ioremap(0x18910000, (4 << 10));
	if(!base) { ERR_CCEPFB("DP 2nd init ioremap failed\n"); return; } 
	CCEPFB_WRITEL(0x1	   , base + 0x0004);
	CCEPFB_WRITEL(0x0	   , base + 0x0088);

	value = CCEPFB_READL(base + 0x114) & 0xFFF;	
	value |= REGISTER_READL(0x10300224) & 0xFFFFF000;
	REGISTER_WRITEL(value, 0x10300224);

	value = CCEPFB_READL(base + 0x118) & 0xFFF;	
	REGISTER_WRITEL(value, 0x10300228);

	value = CCEPFB_READL(base + 0x114) & 0xFFF;	
	value |= (CCEPFB_READL(base + 0x118) & 0xFFF) << 16  ;	
	REGISTER_WRITEL(value, 0x10300088);

	iounmap (base);
#   endif 
#endif
}


static int ccepfb_open(struct fb_info * p_fbinfo, int user)
{
	int retval = 0;
	SDP_CCEPFB_T * p_ccepfb = (SDP_CCEPFB_T *)p_fbinfo->par;

	if(p_fbinfo->screen_base == NULL){
    		p_fbinfo->screen_base = ioremap(p_fbinfo->fix.smem_start, p_fbinfo->fix.smem_len);
		
		if(!p_fbinfo->screen_base) {
			ERR_CCEPFB("%s ioremap failed\n", p_fbinfo->fix.id);	
	//		retval = -ENOMEM;
		} else {
			p_ccepfb->vir_membase = (u32)p_fbinfo->screen_base;	
			INFO_CCEPFB("%s framebuffer screen base 0x%08x\n", p_fbinfo->fix.id, p_ccepfb->vir_membase);
	
		}
	} 

#ifdef CONFIG_FRAMEBUFFER_CONSOLE 
	if(!ccepfb_initcon_disable) 
		temp_display_init();	
#endif

	return retval;
}



static struct fb_ops sdp_ccepfb_ops = {
	.owner 			= THIS_MODULE,	
	.fb_open 		= ccepfb_open, 
	.fb_pan_display = ccepfb_pan_display, 	// pan display 
	.fb_check_var	= ccepfb_check_var, 	// framebuffer check variable
	.fb_set_par 	= ccepfb_set_par,		// particular framebuffer
	.fb_setcolreg 	= ccepfb_setcolreg,
	
	.fb_fillrect 	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};


static int __init sdp_ccepfb_var_init(const SDP_CCEPFB_INFO_T* p_ccepfb_info, struct fb_var_screeninfo *p_varinfo)
{

	int retval = 0;

#ifdef CONFIG_ARCH_SDP1207	
	FB_LAYER_T 	layer;
    SDP_CCEPFB_MEMBASE[LAYER_OSD] = (0x80000000 + (( (sdp_sys_mem1_size/0x100000)) << 20));
    SDP_CCEPFB_MEMBASE[LAYER_GP]  = (0x40000000 + (( (sdp_sys_mem0_size/0x100000) + 20) << 20));
    SDP_CCEPFB_MEMBASE[LAYER_SGP] = (0x40000000 + (( (sdp_sys_mem0_size/0x100000) + 36 ) << 20));
	

	for(layer = LAYER_OSD; layer < LAST_LAYER; layer++) 
	{
       g_sdp_ccepfb_membase[layer]  = SDP_CCEPFB_MEMBASE[layer];
	}

#else
	unsigned int system_mem_size=0;

	system_mem_size = sdp_get_mem_cfg(1);

	if((system_mem_size>>20)==2048)
	{
		SDP_CCEPFB_MEMBASE = (0x40000000 + (( (sdp_sys_mem0_size/0x100000)) << 20));
	}
	else
	{
		SDP_CCEPFB_MEMBASE = 0x40000000;
	}

    g_sdp_ccepfb_membase = SDP_CCEPFB_MEMBASE;
#endif


    memset(p_varinfo, 0, sizeof(struct fb_var_screeninfo));

    p_varinfo->nonstd 	= 0;
    p_varinfo->activate = FB_ACTIVATE_NOW;
    p_varinfo->height 	= p_ccepfb_info->y;
    p_varinfo->width 	= p_ccepfb_info->x;
    p_varinfo->pixclock 	= 0;
    p_varinfo->left_margin 	= 0;
    p_varinfo->right_margin = 0;
    p_varinfo->upper_margin = 0;
    p_varinfo->lower_margin = 0;
    p_varinfo->hsync_len 	= 0;
    p_varinfo->vsync_len 	= 0;
    p_varinfo->sync 		= 0;
    
    p_varinfo->vmode 		= FB_VMODE_NONINTERLACED;

    p_varinfo->xres	 		= p_ccepfb_info->x; 
    p_varinfo->xres_virtual	= p_ccepfb_info->x; 
    p_varinfo->yres 		= p_ccepfb_info->y; 
    p_varinfo->yres_virtual = p_ccepfb_info->y * 2; 
    p_varinfo->bits_per_pixel = SDP_CCEPFB_BPP; 

    p_varinfo->xoffset 		= 0;
    p_varinfo->yoffset 		= 0;

    p_varinfo->grayscale 	= 0;

    p_varinfo->transp.length = 8;
    p_varinfo->red.length 	 = 8;
    p_varinfo->green.length  = 8;
    p_varinfo->blue.length   = 8;

    p_varinfo->transp.offset = 24;
    p_varinfo->red.offset 	 = 16;
    p_varinfo->green.offset  = 8;
    p_varinfo->blue.offset 	 = 0;
    p_varinfo->rotate         = 0;
	return retval;
}


static int __init sdp_fb_set_fix(const SDP_CCEPFB_INFO_T* p_ccepfb_info, struct fb_fix_screeninfo *p_fixinfo)
{
	int retval = 0;
	
   	memset(p_fixinfo, 0, sizeof(struct fb_fix_screeninfo));

    strncpy(p_fixinfo->id, p_ccepfb_info->name,sizeof(p_fixinfo->id));

    p_fixinfo->type 	= FB_TYPE_PACKED_PIXELS;
    p_fixinfo->visual 	= FB_VISUAL_TRUECOLOR;
    p_fixinfo->type_aux = 0;
    
    p_fixinfo->xpanstep  = 0;
    p_fixinfo->ypanstep  = 1; // page filpping
    p_fixinfo->ywrapstep = 0;
    
    p_fixinfo->line_length = p_ccepfb_info->x * (SDP_CCEPFB_BPP / 8);  
    p_fixinfo->accel 		= FB_ACCEL_NONE;

#ifdef CONFIG_ARCH_SDP1207	
	p_fixinfo->smem_start  = g_sdp_ccepfb_membase[p_ccepfb_info->offset];  
#else
	p_fixinfo->smem_start  =  g_sdp_ccepfb_membase + p_ccepfb_info->offset;  
#endif
	p_fixinfo->smem_len  =  p_ccepfb_info->size;  
	

    return retval;
}

#if CONFIG_ARCH_SDP1202
extern unsigned int sdp_gpio_get_value(unsigned int gpionum);
#endif

static int __init sdp_ccepfb_probe(struct platform_device *pdev)
{
    int 	retval = 0;
    struct  fb_info  *p_fbinfo;
	SDP_CCEPFB_T * p_ccepfb; 
	FB_LAYER_T 	layer;
	void *base[3];
	const SDP_CCEPFB_INFO_T * p_ccepfb_info;

#ifdef CONFIG_ARCH_SDP1202
	unsigned int system_mem_size=0;
#endif

	if(pdev == NULL) return 0;

   	platform_set_drvdata(pdev, sdp_fbinfo);

#ifdef CONFIG_ARCH_SDP1202
	system_mem_size = sdp_get_mem_cfg(1);

	if((system_mem_size>>20)==2048)
	{
#ifdef CONFIG_FRAMEBUFFER_CONSOLE
		p_ccepfb_info = (ccepfb_initcon_disable) ? (const)sdp_ccepfb_info_UD: (const)sdp_ccepfb_info_con_UD;
#else
		p_ccepfb_info = (const)sdp_ccepfb_info_UD;
#endif
	}
	else
#endif
	{
#ifdef CONFIG_FRAMEBUFFER_CONSOLE
		p_ccepfb_info = (ccepfb_initcon_disable) ? (const)sdp_ccepfb_info : (const)sdp_ccepfb_info_con;
#else
		p_ccepfb_info = (const)sdp_ccepfb_info;
#endif
	}


#ifdef CONFIG_ARCH_SDP1207	
	base[0] = ioremap(0x30400000, 0x1000);
	base[1] = ioremap(0x30410000, 0x1000);
	base[2] = ioremap(0x30420000, 0x1000);
#elif CONFIG_ARCH_SDP1202
	if(*(volatile u32*)0xFE090E04 & (1 << 5)){		// port 22, pin 5 -> '1':LED_FLIP , '0':PDP
		GfxFlipMode = GFX_HV_FLIP;
	}
#endif

	for(layer = LAYER_OSD; layer < LAST_LAYER; layer++) {

    	p_fbinfo = framebuffer_alloc(sizeof(SDP_CCEPFB_T), &pdev->dev);
    	if (!p_fbinfo) {
        	ERR_CCEPFB("%s frame buffer allocation fail\n", p_ccepfb_info[layer].name);
        	continue;
    	}

		sdp_fbinfo[layer] = p_fbinfo; // platform device resource
// h/w initialize -> set_par

    	sdp_ccepfb_var_init((p_ccepfb_info+layer), &p_fbinfo->var);   // struct var initialize
    	sdp_fb_set_fix((p_ccepfb_info+layer), &p_fbinfo->fix); 		// struct fixel initialize

		p_ccepfb = (SDP_CCEPFB_T*)p_fbinfo->par;
		p_ccepfb->p_fbinfo = p_fbinfo;
		p_ccepfb->pdev = pdev;
		p_ccepfb->layer = layer;
		p_ccepfb->name = p_ccepfb_info[layer].name;
		p_ccepfb->phy_membase = p_fbinfo->fix.smem_start;
		p_ccepfb->vir_membase = 0;
#ifdef CONFIG_ARCH_SDP1207	
		p_ccepfb->regbase = base[layer];
		p_ccepfb->osd_regbase = base[LAYER_OSD];
#else
		p_ccepfb->regbase = REGISTER_ADDR_CONV(p_ccepfb_info[layer].regbase);
		p_ccepfb->osd_regbase = REGISTER_ADDR_CONV(p_ccepfb_info[LAYER_OSD].regbase);
#endif

// fb_info init
    	p_fbinfo->pseudo_palette = p_ccepfb->pseudo_palette;
    	p_fbinfo->flags = FBINFO_FLAG_DEFAULT;
// TODO connect fops
    	p_fbinfo->fbops = &sdp_ccepfb_ops;

//    	p_fbinfo->screen_base = ioremap(p_fbinfo->fix.smem_start, p_fbinfo->fix.smem_len);
/*
	    if (!p_fbinfo->screen_base) {
        	ERR_CCEPFB("%s abort, cannot ioremap video memory 0x%x @ 0x%lx\n", 
						p_fbinfo->fix.id, p_fbinfo->fix.smem_len, p_fbinfo->fix.smem_start);
        	framebuffer_release(p_fbinfo);
			continue;
    	}
*/

//		retval = fb_find_mode(&p_fbinfo->var, p_fbinfo, "640x540@60", NULL, 0, NULL, 8);	// ????

#if CONFIG_ARCH_SDP1202
		if(GfxFlipMode == GFX_HV_FLIP){
			p_fbinfo->var.rotate = GFX_HV_FLIP;
		}
#endif

    	retval = register_framebuffer(p_fbinfo);
		if (retval < 0) {
        	ERR_CCEPFB("%s Failed to register framebuffer device: %d\n", p_fbinfo->fix.id, retval);
			framebuffer_release(p_fbinfo);
			return retval;	
    	}

    	INFO_CCEPFB("%s framebuffer at physical: 0x%08x, regbase: 0x%08x\n", 
			p_fbinfo->fix.id, p_fbinfo->fix.smem_start, p_ccepfb->regbase);
    	INFO_CCEPFB("fb%d: %s frame buffer device\n", p_fbinfo->node, p_fbinfo->fix.id);
	}

    return 0;
}


static int sdp_ccepfb_remove(struct platform_device *pdev)
{
    struct fb_info  *p_fbinfo = platform_get_drvdata(pdev);


// TODO : all framebuffer remove
    if (p_fbinfo) 
    {
        unregister_framebuffer(p_fbinfo);
        framebuffer_release(p_fbinfo);
    }
    return 0;
}

static int sdp_ccepfb_suspend(struct platform_device *pdev, pm_message_t msg)
{
	struct fb_info * p_fbinfo = platform_get_drvdata(pdev);
	SDP_CCEPFB_T * p_ccepfb = (SDP_CCEPFB_T*)p_fbinfo->par; 

// TODO: suspend

	return 0;
}



static int sdp_ccepfb_resume(struct platform_device *pdev)
{
	struct fb_info * p_fbinfo = platform_get_drvdata(pdev);
	SDP_CCEPFB_T * p_ccepfb = (SDP_CCEPFB_T*)p_fbinfo->par; 

// TODO: resume

	return 0;
}

static struct platform_driver __refdata sdp_ccepfb_driver = {
	.probe 		= sdp_ccepfb_probe,
	.remove 	= sdp_ccepfb_remove,
	.suspend 	= sdp_ccepfb_suspend,
	.resume 	= sdp_ccepfb_resume,
	.driver 	= {
		.name 	= "sdp-ccepfb",
		.owner 	= THIS_MODULE,
	},
};

static struct platform_device * sdp_ccepfb_device;


#ifndef MODULE
int __init sdp_ccepfb_setup(char *this_opt)
{
    /* Parse user speficied options (`video=sdp_ccepfb:') */
	char *options;

	if (!this_opt || !*this_opt)
		return 1;

	while ((options = strsep(&this_opt, ",")) != NULL) {
		if (!strncmp(options, "noinit_fbcon", 7)) {
			options += 7;
			INFO_CCEPFB("deactivate virtual console");
			ccepfb_initcon_disable = 1;		// 1 - disable, 0 - enable
		}
	}
	return 1;
}

__setup("ccepfb=", sdp_ccepfb_setup);
#endif /* MODULE */


int __init sdp_ccepfb_init(void)
{
    int ret=0;

#ifndef MODULE
//	char * option = NULL;
	
//	if (fb_get_options("sdp_ccepfb", &option))
//			return -ENODEV;
	
//	sdp_ccepfb_setup(option);
#endif

    ret = platform_driver_register(&sdp_ccepfb_driver);
	if(!ret) {
		sdp_ccepfb_device = platform_device_register_simple("sdp-ccepfb", 0, NULL, 0);
		
		if ( IS_ERR(sdp_ccepfb_device) ) {
			platform_driver_unregister(&sdp_ccepfb_driver);
			ret = PTR_ERR(sdp_ccepfb_device);
		}
	}

    return ret;
}

static void __exit sdp_ccepfb_cleanup(void)
{
	platform_device_unregister(sdp_ccepfb_device);
    platform_driver_unregister(&sdp_ccepfb_driver);
}

module_init(sdp_ccepfb_init);
module_exit(sdp_ccepfb_cleanup);

MODULE_AUTHOR("tukho.kim@Samsung");
MODULE_DESCRIPTION("Framebuffer driver for SDP SoC");
MODULE_LICENSE("GPL");


