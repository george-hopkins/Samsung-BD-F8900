/***************************************************************************
 *
 * 		arch/arm/plat-ccep/video/sdp_ccepfb.h
 * 		created by tukho.kim@samsung.com
 *
 * *************************************************************************/


#ifndef __SDP_CCEPFB_H__
#define  __SDP_CCEPFB_H__

#ifdef CONFIG_ARCH_SDP1106
#define FLIP_X_POS		137
#define FLIP_Y_POS		22
#elif defined(CONFIG_ARCH_SDP1202)
#define FLIP_X_POS		124
#define FLIP_Y_POS		22
#elif (CONFIG_ARCH_SDP1207)
#define FLIP_X_POS		124
#define FLIP_Y_POS		22
#endif

#ifdef CONFIG_ARCH_SDP1207 //FoxB
#define A_OSD_BASE   (0x00400000)
#define A_GP_BASE   (0x00410000)
#define A_SUBGP_BASE  (0x00420000)
#else
/* register offset define */
#define A_OSD_BASE			(0x00300000)
#define A_GP_BASE			(0x00310000)
#define A_SUBGP_BASE		(0x00311000)
#endif
/* common */
#define O_DP_MODE			(0x00)
#define O_H_SCALE_RATIO 	(0x04)
#define O_V_SCALE_RATIO		(0x08)
#define O_PLANE_REORDER		(0x0C)
#define O_LVDS_CON			(0x10)
#define O_MUTE_COLOR		(0x14)
#define O_MUTE_COLOR_ON		(0x18)
#define O_BASE_ADDR			(0x1C)
#define O_CURR_DP_ADDR		(0x20)
#define O_H_START_PIXEL		(0x24)
#define O_INPUT_SIZE		(0x28)
#define O_OUTPUT_SIZE		(0x2C)
#define O_H_OFFSET_ADDR		(0x30)
#define O_CENTER_POS_OFFSET	(0x34)
#define O_TH_DP_LINE		(0x38)


#define O_OSD_SUBGP_BLEND		(0x58)
#define O_OSD_GP_BLEND			(0x5C)
#define O_OSD_OSD_BLEND			(0x60)
#define O_OSD_SUBGP_BLEND		(0x58)

#define O_OSD_UPDATE			(0x80)
#define O_OSD_OSD_HV_START		(0x8C)
#define O_OSD_GP_HV_START		(0x90)
#define O_OSD_SUBGP_HV_START	(0x94)


#define O_OSD_OSD_HV_SIZE		(0x98)
#define O_OSD_GP_HV_SIZE		(0x9C)
#define O_OSD_SUBGP_HV_SIZE		(0xA0)

#ifdef CONFIG_ARCH_SDP1202
#define O_OSD_MAIN_WIN_POS		(0xC4)
#define MAIN_WIN_POS		(0x0016008D)
#endif

#endif /* __SDP_CCEPFB_H__ */






