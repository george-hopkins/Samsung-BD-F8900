/****************************************************************************
 *   FileName    : TCCresource.h
 *   Description : Project resource Header file
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef _TCCRESOURCE_H_
#define _TCCRESOURCE_H_

#if 0
#include "include/main.h"
#endif
#include "Kernel/telechips.h"
#if 0
#include "Resource/TC_Kernel.h"
#endif


//-----------------------------------------------
// Definitions
//-----------------------------------------------
#define 	VIDEO_LCD_INIT_FLAG		0x00000001
#define 	VIDEO_LCD_SYNC_FLAG		0x00000002
#define 	VIDEO_VIDEO_MODE_FLAG	0x00000004
#define 	VIDEO_CAMERA_MODE_FLAG	0x00000008
#define 	VIDEO_MP4ENC_MODE_FLAG	0x00000010

#define 	MAX_NUM_OF_IMAGE_BUFFER		2

#ifdef CD_INCLUDE
typedef struct CD_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short	usFileNum;
	unsigned short	usSyncOption;
} CD_COMMAND_t;
#endif

typedef struct DEMON_COMMAND_s
{
	/* for Operation */
	unsigned int 	usCommand;
	unsigned short  usState;
	unsigned short  usFileNum;
	unsigned short  usSyncOption;
} DEMON_COMMAND_t;

#ifdef USE_DISPLAY_OBJECT
typedef struct BI_COMMAND_s
{
	/* for Operation */
	unsigned short usCommand;
	unsigned short usType;
	unsigned int  param1;
	unsigned int  param2;
	unsigned int *func;
} BI_COMMAND_t;
#endif

typedef struct AUDIO_COMMAND_s
{
	/* for Operation */
	unsigned short 	usWaitingFlag;
	unsigned short 	usCommand;
	unsigned short	usFileNum;
	unsigned short	usSyncOption;
	unsigned long	ulTargetTime;
	long				usForceStopFlag;
} AUDIO_COMMAND_t;

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)

#if 0
typedef struct AUDIO2_COMMAND_s
{
	/* for Operation */
	unsigned short 	usCommand;
	unsigned short	usFileNum;
	long	ulTargetTime;
	unsigned short	usSyncOption;
} AUDIO2_COMMAND_t;
#endif

typedef		AUDIO_COMMAND_t	AUDIO2_COMMAND_t;

#endif

typedef struct DAB_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short	usFileNum;
	long	ulTargetTime;
	unsigned short	usSyncOption;
} DAB_COMMAND_t;

#ifdef CD_RIPPING_INCLUDE
typedef struct RIPPING_COMMAND_s
{
	/* for Operation */
	unsigned short 	usCommand;
	unsigned short	usFileNum;
	long	ulTargetTime;
	unsigned short	usSyncOption;
} RIPPING_COMMAND_t;


typedef struct RIPBUF_COMMAND_s
{
	/* for Operation */
	unsigned short 	usCommand;
	unsigned short	usFileNum;
	long	ulTargetTime;
	unsigned short	usSyncOption;
} RIPBUF_COMMAND_t;

#endif

typedef enum MOUNT_T_BASIC_CMD
{
	MOUNT_START_CMD,
	MOUNT_STOP_CMD,
	MOUNT_UNMOUNT_CMD,
	MOUNT_CHANGE_CMD,
	MOUNT_BACKSTART_CMD,
	MOUNT_USBH_ENUM_CMD,
	MOUNT_DISK_FORMAT_CMD,
	MOUNT_CMD_MAX
} MOUNT_CMD_ENUM;

typedef struct MOUNT_BASIC_COMMAND_s	 /* Mount Task */
{
	unsigned short		usWaitingFlag;
	unsigned short		usCommand;
	unsigned short		usExtInfo;
	unsigned short		usSyncOption;
	unsigned short		usDeviceNum;
}	MOUNT_BASIC_t;

typedef struct MOUNT_COMMAND_s	 /* Mount Task */
{
	/* for Operation */
	MOUNT_BASIC_t	tCommand[MOUNT_CMD_MAX];

} MOUNT_COMMAND_t;

#ifdef SDRAM_2M
typedef struct KEYACT_COMMAND_s
{
	/* for Operation */
	unsigned short 	usCommand;
	unsigned char 		ucKeyData;
	unsigned char		pad;
	unsigned short	usSyncOption;
} KEYACT_COMMAND_t;
#endif

typedef struct VIDEO_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short		usFileNum;
	int		sVideoFrameNumber;
	unsigned int		uiCommonData;
	unsigned short	usSyncOption;
} VIDEO_COMMAND_t;

#ifdef WMV_INCLUDE
typedef struct ASFPARS_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short	uiPID;
	int				istatus;
	unsigned short	usSyncOption;
} ASFPARS_COMMAND_t;
#endif

//#ifdef DxB_RECEIVER_INCLUDE

typedef	struct
{
	int				iPlayIdx;			//-1: no ch. is playing, others: the index of stUiDmbSvcEnt
	unsigned short	usEnsIdx;
	unsigned long	ulSvcIdx;
	unsigned short	usSubChIdx;
	int				iPrevIdx;
} TServiceSelInfo;

typedef struct DxB_DataSrvCtrl_COMMAND_s
{
	unsigned short	usCommand;
	unsigned short 	usSyncOption;
	//unsigned char		type;
	int 	ch_id;
	unsigned int 	BitRate;
	int 	eFirstDecode;
	int 	eSecondDecode;
	//struct PSERVICEDB *pService;
	//struct PCHANNELDB *pChannel;

} DxB_DataSrvCtrl_COMMAND_t;

/* CHANNEL_CONTROL_TASK */

/*---------------- DMB_DataSrv_TASK message --------------------*/
typedef struct DMB_SLS_COMMAND_s
{
	unsigned short usSyncOption;
	unsigned short usImageType;
} DXB_SImage_COMMAND_t;

typedef struct DMB_PAD_COMMAND_s
{
	unsigned short usSyncOption;
} DXB_String_COMMAND_t;

typedef struct DMB_BWS_COMMAND_s
{
	unsigned short usSyncOption;

	int 			uiChId;
	unsigned char 	*pucData;
	char 			*pContentsName;
	int 			uiDataLength;
} DMB_BWS_COMMAND_t;

typedef struct DMB_EPG_COMMAND_s
{
	unsigned short 	usSyncOption;

	int 			uiChId;
	unsigned char 	*pucData;
	char 			*pContentsName;
	int 			uiDataLength;
} DMB_EPG_COMMAND_t;

typedef struct DMB_TPEG_COMMAND_s
{
	unsigned short usSyncOption;
	int ch_id;
} DMB_TPEG_COMMAND_t;

typedef struct DMB_BIFS_COMMAND_s
{
	unsigned short usSyncOption;
} DMB_BIFS_COMMAND_t;

/*---------------- AUTO_SCAN_TASK message --------------------*/
typedef struct AUTO_SCAN_COMMAND_s
{
	unsigned short 	usCommand;
	int				istatus;
} AUTO_SCAN_COMMAND_t;

typedef struct SYNC_LAYER_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short	uiPID;
	int				istatus;
	unsigned short	usSyncOption;
} SYNC_LAYER_COMMAND_t;

typedef struct HEADER_DECODE_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short	uiPID;
	unsigned char	*ucTSData;
#ifdef DMB_EPG_INCLUDE
	int 			uiChId;
	unsigned char 	*pucData;
	char 			*pContentsName;
	int 			uiDataLength;
#endif
	unsigned short	usSyncOption;
} HEADER_DECODE_COMMAND_t;

//#endif	//DxB_RECEIVER_INCLUDE

#ifdef _MAKE_PRIVATE1_TASK_
typedef struct PRIVATE1_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned short	usSyncOption;

} PRIVATE1_COMMAND_t;
#endif /* _MAKE_PRIVATE1_TASK_ */

#ifdef CAPTION_INCLUDE

typedef struct DXB_SUBTITLE_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned int	uiSubtitleType;
	unsigned short	usSyncOption;
#ifdef	DVBT_SUBTITLE_INCLUDE
	unsigned short 	usCompositionPID;
	unsigned short	usAncillaryPID;
#endif	//DVBT_SUBTITLE_INCLUDE
} DxB_SUBTITLE_COMMAND_t, *pDxB_SUBTITLE_COMMAND_t;
#endif

typedef struct SIMAGE_COMMAND_s
{
	unsigned short	usCommand;
	unsigned short	usSyncOption;
	unsigned int    uiCommonData;
} SIMAGE_COMMAND_t;

/*---------------- CAAGENT message --------------------*/
#if defined(CAS_CAAGENT_INCLUDE) && defined(CAAGENT_USE_MESSAGE_Q)
typedef struct CAAGENT_COMMAND_s
{
	unsigned short 	usCommand;
	unsigned int 		uiCommonData;
	int 				iretvalue;
	unsigned short	usSyncOption;
} CAAGENT_COMMAND_t;
#endif

/*---------------- DREAMERICAS message --------------------*/
#if defined(DREAMERICAS_INCLUDE) && defined(DREAMERICAS_USE_MESSAGE_Q)
typedef struct DREAMERICAS_COMMAND_s
{
	unsigned short 	usCommand;
	short 			iResult;
	unsigned int		uiService_id;
	unsigned short	usService_flag;
	unsigned short	usSyncOption;
} DREAMERICAS_COMMAND_t;
#endif

typedef struct DEMUX_COMMAND_s
{
	/* for Operation */
	unsigned short 	usCommand;
	unsigned int 		uiCommonData;
	unsigned int		uiCommonData2;
	unsigned short	usSyncOption;
} DEMUX_COMMAND_t;


#define LCDPATH				0
#define MEMPATH				1
#define SNAPSHOT			1
#define PREVIEW_VIDEO		0

#define YUV420		0
#define YUV422		1
#define RGB_FORMAT	2

#define YUV422H		1	/* Hsize/2 - orginal */
#define YUV422V		2	/* Vsize/2 - rotate */
#define YUV422S0	3	// YUV422 Sequencial Mode 0 : Y0 Cb0 Y1 Cr0 Y2 Cb2 Y3 Cr2...
#define YUV422S1	4	// YUV422 Sequencial Mode 1 : Cb0 Y0 Cr0 Y1 Cb2 Y2 Cr2 Y3...

#define IsSupported	1
#define NotSupported	0

#if 0
#define PixelClockPeriod(X,Y)			(((X)*1000)/((Y)/10000))
#define ActiveDataTimeForLine(X,Y)	((((X)+8) * (Y)) / 1000)
#define HorizontalBlanking(X,Y)		(((X) * (Y)) / 1000)
#define TotalFrameTime(X,Y,Z)		((((X)+8)+(Y)) * (Z))

#define TargetFrameFreq(X,Y,Z)	((X) / ((Y) * ((Z)+1)))
#endif

typedef struct _LCD_CAPABILITY_INFO
{
	unsigned short	usCanUsedFunction;	/* This is indicated used function in Lcd module */

	unsigned short	usLcdModuleHorizontalSize;
	unsigned short	usLcdModuleVerticalSize;

	unsigned char	ucInputFormat;

	unsigned char	ucFrameFrequency;
	unsigned char	ucRotationValue;

	unsigned		uMasterClock;
	unsigned		uPrevMasterClock;
	unsigned		uPixelClock;
	unsigned char	ucDivValue;

	unsigned char	ucBusWidth;
	unsigned char	ucHoldTime;
	unsigned char	ucSetupTime;
	unsigned char	ucPulseWidth;

	unsigned char ucInterfaceType;
} LCD_CAPABILITY_INFO;

extern LCD_CAPABILITY_INFO stLcdCapabilityInfo;

typedef struct _LCD_CONFIG_INFO
{
	unsigned short	uiRealWidth;	/* Physical LCD X Resolution */
	unsigned short	uiRealHeight;	/* Physical LCD Y Resolution */
	unsigned char	uiColorDepth;	/* Physical LCD Color Depth */
	unsigned short	uiImageWidth;	/* Application Image X Resolution */
	unsigned short	uiImageHeight;	/* Application Image Y Resolution */
} LCD_CONFIG_INFO;

typedef struct	_IMAGE_BUF_INFO
{
	unsigned int	uiIsUsedFlag;
	unsigned int	uiPingPongFlag;
	unsigned int	uiLastFrameFlag;
	void	*vPtrY;
	void	*vPtrU;
	void	*vPtrV;
} IMAGE_BUF_INFO;

/*-----------------------------------------------------------------------*/
/* Audio structures */
typedef struct AUDIO_INFORMATION_s
{
	/* for Information */
	unsigned char	ucCodec;
	unsigned char	ucChannel;
	unsigned char	ucUseSubCore;
	unsigned int	uiSamplingRate;
	unsigned int	uiBitrate;
	unsigned long	uiDecodeTime;
	unsigned int	uiAudioCount;
	unsigned char	ucContainer;
#ifdef AVI_CONTAINER_INCLUDE
	short			sFormatTag;
#endif
	unsigned int	uiSuggestedBufferSize;
	unsigned char 	uiOpenTrackFailMode;
} AUDIO_INFORMATION_t;

extern AUDIO_INFORMATION_t 	stAudioInfo;

typedef struct
{
	unsigned char	ucTVModeFlag;
	unsigned char ucTVDispModeFlag;
	unsigned char ucTVOutIfModeFlag;
	unsigned char ucTvWrAddr;
	unsigned char ucTvEncType;
	unsigned int	uiTvHorizontalSize;
	unsigned int	uiTvVerticalSize;
} TV_INFO;

/* Video structures */
typedef struct	_VIDEO_INFO
{
	unsigned char	ucgCodec;
	unsigned int	uiBackgroundFileIndex;
	unsigned int	uiStatus;

	unsigned short	uiRealHeight;		//	Video_Real Height
	unsigned short	uiRealWidth;		//	Video_Real Width
	unsigned short	uiDisplayHeight;	//	(LCD) Display area
	unsigned short	uiDisplayWidth;		//	(LCD) Display area
	unsigned short	uiVideoHeight;		//	Video size (& 0xFFF0)
	unsigned short	uiVideoWidth;		//	Video size (& 0xFFF0)
	unsigned short	uiMemoryHeight;		//	Decode_Memory area (& 0xFFF8)
	unsigned short	uiMemoryWidth;		//	Decode_Memory area (& 0xFFF8)

	unsigned int	uiVideoHScale;
	unsigned int	uiVideoVScale;
	unsigned int 	uiVideoCount;
	unsigned int	uiFrameRate;
#ifdef AVI_CONTAINER_INCLUDE
	unsigned int	uiCompression;
#endif
	unsigned int	uiSuggestedBufferSize;
	unsigned char	ucSkipFrameCnt;
	TV_INFO		tvInfo;
} VIDEO_INFO;

#if 0
#define GET_16_MULTIPLE(param)			((((unsigned short)(param)) + 15) & 0xFFF0)
#define IS_16_MULTIPLE(param)			((((unsigned short)(param)) & 0x000F) == 0)
#endif

extern VIDEO_INFO			stVideoInfo;

enum
{
	VIDEO_SEEKINFO_NEXTATEND =0,
	VIDEO_SEEKINFO_PAUSEATEND
};

enum
{
	VIDEO_SEEKINFO_MIDDLE = 0,
	VIDEO_SEEKINFO_FIRST,
	VIDEO_SEEKINFO_LAST
};

typedef struct _VIDEO_SEEKINFO
{
	unsigned 	int 	uiCodec;
	unsigned 	int	uiMode;				// VIDEO_SEEKINFO_NEXTATEND, VIDEO_SEEKINFO_PAUSEATEND
	unsigned int	uiStride;			// unit: ms
	unsigned int	uiFlag;
} VIDEO_SEEKINFO;

extern VIDEO_SEEKINFO	stVideoSeekInfo;

#define	VIDEO_SEEKSTRIDE_MAX		(30000)
#define	VIDEO_SEEKSTRIDE_MIN		(10000)
#define	VIDEO_SEEKSTRIDE_DEFAULT	(10000)

/* LCD structures */
typedef struct _LCD_INFO
{
	LCD_CONFIG_INFO LCD_Config;			/* LCD Config parameters */
	IMAGE_BUF_INFO	stImageBufInfo[MAX_NUM_OF_IMAGE_BUFFER]; 	/* Pingpong Even Buffer */
} LCD_INFO;

extern LCD_INFO LCD_Info;


#define MAX_NORMAL_PTS_DIFF_OF_VIDEO		(1000)	// UNIT : ms(millisecond).
#define MAX_NORMAL_PTS_DIFF_OF_AUDIO		(1000)	// UNIT : ms(millisecond).
#define MAX_ACCEPT_PTS_BETWEEN_STREAMS		(3000)	// UNIT : ms(millisecond).

#define TEMP_CHANGE_AUDIO_PTS				(20)	// Unit is ms(millisecond).
#define TEMP_CHANGE_VIDEO_PTS				(30)	// Unit is ms(millisecond).

#define TEMP_CHANGE_AUDIO_PTS		(20)	// Unit is ms(millisecond).
#define TEMP_CHANGE_VIDEO_PTS		(30)	// Unit is ms(millisecond).

#define MAX_TIME_OF_SUBTITLE_ACCEPT_PTS		(10000)	// Unit is ms(millisecond).
#define TEMP_CHANGE_SUBTITLE_PTS			(500)	// Unit is ms(millisecond).

#if 0
#define	M_GET_ABS_VALUE(x, y)		(((x) > (y)) ? (x) - (y) : (y) - (x))
#endif

#define INITIAL_INDEX_OFFSET		(-1)
#define INITIAL_OPENDML_ENTRY_INDEX	(0x00FFFFFF)

/* For TDMB Local Player */
typedef	struct
{
	/* BSAC or AACP or MP2*/
	int 			iAudioPts;
	unsigned int	ADSI_Length;
	char			ADSI[20];
	unsigned int	uiAudioLength;

	/* H264 or MPEG2*/
	int 			iVideoPts;
	unsigned int	SPS_Length;
	char			SPS[20];
	unsigned int	PPS_Length;
	char			PPS[10];
	unsigned int	uiVideoLength;

	/* H264 Encoded data pointer */
	unsigned char 	*pucH264EncodedData;

	/* SUBTITLE */
	int 			iSubtitlePts;
	unsigned int	uiSubtitleLength;
} DxB_INFO;

/* [1241]  This is for DMB recording and playback */
typedef struct
{
	/* Audio Codec */
	unsigned int	firstAudioPts;
	unsigned int	lastAudioPts;
	int				iStartAudioPTS;
	int				iPrevAudioPTS;
	int				iResyncAudioPTS;
	int				iTotalAudioPTS;

	/* Video Codec */
	unsigned int	firstVideoPts;
	unsigned int	lastVideoPts;
	int				iStartVideoPTS;
	int				iPrevVideoPTS;
	int				iResyncVideoPTS;
	int				iTotalVideoPTS;
} DxBREC_INFO;

extern DxB_INFO	stDxBLocalDecInfo;
extern DxBREC_INFO	stDxBRecPlaybackInfo;



//-----------------------------------------------
// Define Queues
//-----------------------------------------------
#ifdef CD_INCLUDE
#define	TCC_CD_Q_SIZE				10
extern TC_QUEUE TCC_CD_Q;
//extern void *TCC_CD_Msg[];
#endif

#ifdef USE_DISPLAY_OBJECT
#define	TCC_BI_Q_SIZE			10
extern TC_QUEUE TCC_BI_Q;
extern TC_SEMAPHORE TCC_BI_SEM;
#endif // USE_DISPLAY_OBJECT

#define	TCC_DEMON_Q_SIZE			10
extern TC_QUEUE TCC_DEMON_Q;
//extern void *TCC_DEMON_Msg[];

#ifdef HACO_INCLUDE
#define	DEMON_MSG_Q_DEPTH		100
#define	DEMON_MSG_Q_WIDTH		10
extern TC_QUEUE DEMON_MSG_Q;
#endif // HACO_INCLUDE

#define	TCC_AUDIO_Q_SIZE			2
extern TC_QUEUE TCC_AUDIO_Q;
//extern void *TCC_AUDIO_Msg[];

extern TC_QUEUE TCC_MOUNT_Q;
#define	TCC_MOUNT_Q_SIZE			20 	/* Mount Task */

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
#define	TCC_AUDIO2_Q_SIZE			2
extern TC_QUEUE TCC_AUDIO2_Q;
//extern void *TCC_AUDIO2_Msg[];
#endif

#ifdef CD_RIPPING_INCLUDE
#define	TCC_RIPPING_Q_SIZE		10
extern TC_QUEUE TCC_RIPPING_Q;
//extern void *TCC_RIPPING_Msg[];

#define	TCC_RIPBUF_Q_SIZE			10
extern TC_QUEUE TCC_RIPBUF_Q;
#endif

#ifdef SDRAM_2M
#define	TCC_KEYACT_Q_SIZE			10
extern TC_QUEUE TCC_KEYACT_Q;
#endif
#ifdef _MULTIMEDIA_PLAYER_
#define	TCC_VIDEO_Q_SIZE		1
extern TC_QUEUE TCC_VIDEO_Q;
#endif
#ifdef STILLIMAGE_INCLUDE
#define	TCC_SIMAGE_Q_SIZE			1
extern TC_QUEUE TCC_SIMAGE_Q;
#endif
#ifdef UHP_INCLUDE
#define	TCC_USB_Q_SIZE			20
extern TC_QUEUE TCC_USB_Q;
#endif
#ifdef WMV_INCLUDE
#define	TCC_ASF_PARSING_Q_SIZE			40
extern TC_QUEUE TCC_ASF_PARSING_Q;
#endif

#ifdef _MAKE_PRIVATE1_TASK_
#define PRIVATE1_TASK_QUEUESIZE			20
extern TC_QUEUE TCC_PRIVATE1_Q;
#endif // _MAKE_PRIVATE1_TASK_

#ifdef CAPTION_INCLUDE
#define SUBTITLE_TASK_QUEUESIZE			20
extern TC_QUEUE TCC_DXB_SUBTITLE_Q;
#endif

#define	TCC_DEMUX_Q_SIZE			10
extern TC_QUEUE TCC_DEMUX_Q;
//extern void *TCC_DEMUX_Msg[];

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
#define	TCC_DEMUX2_Q_SIZE			10
extern TC_QUEUE TCC_DEMUX2_Q;
#endif

#ifdef BLUETOOTH_INCLUDE
#define TCC_BT_SCHED_Q_SIZE 		2048	// 1000
extern TC_QUEUE BT_SCHED_Q;
extern void *BT_SCHED_Msg;

#define TCC_BT_UI_Q_SIZE 			128
extern TC_QUEUE BT_UI_Q;
extern void *BT_UI_Msg;

#define TCC_BT_APPL_AV_Q_SIZE 		2048	// 500
extern TC_QUEUE BT_APPL_AV_Q;
extern void *BT_APPL_AV_Msg;

#define TCC_BT_APPL_HF_Q_SIZE 		2048	// 500
extern TC_QUEUE BT_APPL_HF_Q;
extern void *BT_APPL_HF_Msg;
#ifdef BT_PBAP_CLIENT // Added by ara 2007.12.20
#define TCC_BT_APPL_PAC_Q_SIZE 	2048
extern TC_QUEUE BT_APPL_PAC_Q;
extern void *BT_APPL_PAC_Msg;
#endif

#ifdef BT_OPP_CLIENT // Added by ara on Version [08/04/00]
#define TCC_BT_APPL_OPC_Q_SIZE 	2048
extern TC_QUEUE BT_APPL_OPC_Q;
extern void *BT_APPL_OPC_Msg;
#endif

#ifdef BT_OPP_SERVER // Added by ara on Version [08/04/00]
#define TCC_BT_APPL_OPS_Q_SIZE 	2048
extern TC_QUEUE BT_APPL_OPS_Q;
extern void *BT_APPL_OPS_Msg;
#endif

#ifdef BLUETOOTH_FTP_CLIENT_INCLUDE
#define TCC_BT_APPL_FTC_Q_SIZE 	2048
extern TC_QUEUE BT_APPL_FTC_Q;
extern void *BT_APPL_FTC_Msg;
#endif

#ifdef BLUETOOTH_FTP_SERVER_INCLUDE
#define TCC_BT_APPL_FTS_Q_SIZE 	2048
extern TC_QUEUE BT_APPL_FTS_Q;
extern void *BT_APPL_FTS_Msg;
#endif

#ifdef BT_SPP_INCLUDE
#define TCC_BT_APPL_SPP_Q_SIZE 2048
extern TC_QUEUE BT_APPL_SPP_Q;
extern void *BT_APPL_SPP_Msg;
#endif
#endif

extern	TC_QUEUE TCC_MOUNT_Q;	/* Mount Task */

// Define Semaphores
extern TC_SEMAPHORE TCC_BOOT_SEM;
#ifdef CD_INCLUDE
extern TC_SEMAPHORE TCC_CD_SEM;
#endif
extern TC_SEMAPHORE TCC_DEMON_SEM;
extern TC_SEMAPHORE TCC_AUDIO_SEM;
#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern TC_SEMAPHORE TCC_AUDIO2_SEM;
#endif
#ifdef CD_RIPPING_INCLUDE
extern TC_SEMAPHORE TCC_RIPPING_SEM;
extern TC_SEMAPHORE TCC_RIPBUF_SEM;
#endif
#ifdef SDRAM_2M
extern TC_SEMAPHORE TCC_KEYACT_SEM;
#endif
#ifdef _MULTIMEDIA_PLAYER_
extern TC_SEMAPHORE TCC_VIDEO_SEM;
#endif
#ifdef STILLIMAGE_INCLUDE
extern TC_SEMAPHORE TCC_SIMAGE_SEM;
#endif
#ifdef WMV_INCLUDE
extern TC_SEMAPHORE	TCC_ASF_PARSING_SEM;
#endif

#ifdef _MAKE_PRIVATE1_TASK_
extern TC_SEMAPHORE TCC_PRIVATE1_SEM;
#endif // _MAKE_PRIVATE1_TASK_

#ifdef CAPTION_INCLUDE
extern TC_SEMAPHORE TCC_DXB_SUBTITLE_SEM;
#endif

extern TC_SEMAPHORE TCC_DEMUX_SEM;
#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern TC_SEMAPHORE TCC_DEMUX2_SEM;
#endif
extern TC_SEMAPHORE FCAPI_PLAY_LOCK_SEM;
extern TC_SEMAPHORE FCAPI_PLAY_MISS_CACHE;
extern TC_SEMAPHORE FCAPI_INDEX_LOCK_SEM;
extern TC_SEMAPHORE FCAPI_INDEX_MISS_CACHE;
extern TC_SEMAPHORE FCAPI_PLAY2_LOCK_SEM;
extern TC_SEMAPHORE FCAPI_PLAY2_MISS_CACHE;
extern TC_SEMAPHORE FCAPI_INDEX2_LOCK_SEM;
extern TC_SEMAPHORE FCAPI_INDEX2_MISS_CACHE;
extern TC_SEMAPHORE FCAPI_CDREC_LOCK_SEM;
extern TC_SEMAPHORE FCAPI_CDREC_MISS_CACHE;
extern TC_SEMAPHORE CAPP_SEM;
extern TC_SEMAPHORE FORCE_STOP_SEM;

#ifdef UHP_INCLUDE
extern TC_SEMAPHORE TCC_USBH0_SEM;
extern TC_SEMAPHORE TCC_USBH1_SEM;
#endif

#ifdef IPOD_INCLUDE
extern TC_SEMAPHORE TCC_IPOD0_SEM;
extern TC_SEMAPHORE TCC_IPOD1_SEM;
extern TC_SEMAPHORE TCC_IPOD2_SEM;
extern TC_SEMAPHORE TCC_IPOD3_SEM;
extern TC_SEMAPHORE TCC_IPOD4_SEM;
#endif

//extern TC_SEMAPHORE CODEC_SEM;
#ifdef BLUETOOTH_INCLUDE
extern TC_SEMAPHORE BT_SCHED_RESOURCE_SEM;
extern TC_SEMAPHORE BT_SCHED_TASK_SEM;
extern TC_SEMAPHORE BT_STATUS_SEM;
extern TC_SEMAPHORE BT_POWER_ON_OFF_SEM;
extern TC_SEMAPHORE BT_AUDIO_SEM;
#endif

// Event
extern TC_EVENT_FLAG	TCC_INT_EVENT;


// Stack
#define	BOOT_STACKSIZE				1024	//4096		// 16K	// CDTask
#ifdef CD_INCLUDE
#define	CD_STACKSIZE				8*1024	//4096		// 16K	// CDTask
#endif
#define	DEMON_STACKSIZE				32*1024	//16384		//64K	// DemonTask
#define	AUDIO_STACKSIZE				16*1024	//8192		//32K	// AudioTask
#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
#define	AUDIO2_STACKSIZE			16*1024	//8192		//32K	// Audio2Task
#endif
#ifdef CD_RIPPING_INCLUDE
#define	RIPPING_STACKSIZE			8*1024	//4096		//16K	// RippingTask
#define	RIPBUF_STACKSIZE			8*1024	//4096		//16K	// RibufTask
#endif
#define	VIDEO_STACKSIZE				5000
#ifdef STILLIMAGE_INCLUDE
#define	SIMAGE_STACKSIZE			10000	//5000
#endif
#ifdef UHP_INCLUDE
#define	USB_STACKSIZE				5000
#endif
#ifdef WMV_INCLUDE
#define	ASF_PARSING_STACKSIZE		16000 //8000
#endif

#ifdef _MAKE_PRIVATE1_TASK_
#define DXB_PRIVATE1_STACKSIZE		8000
#endif /* _MAKE_PRIVATE1_TASK_ */

#ifdef CAPTION_INCLUDE
#define DXB_SUBTITLE_STACKSIZE		8000
#endif

#ifdef BRWS_MINI_DB

#define	KEYACT_STACKSIZE			8*1024

#ifdef BLUETOOTH_INCLUDE	//Modified by ara on Version [08/04/00]
#if defined (BT_OPP_CLIENT) || defined (BT_OPP_SERVER)
#ifdef BT_PBAP_CLIENT
#define BT_SCHED_STACK_SIZE			40*1024		// 32K
#define BT_APPL_AV_STACK_SIZE		8*1024		// 8K
#define BT_APPL_HF_STACK_SIZE		8*1024		// 8K
#define BT_APPL_PAC_STACK_SIZE		8*1024		// 8K
#ifdef BT_OPP_CLIENT
#define BT_APPL_OPC_STACK_SIZE		8*1024		// 8K	
#endif
#ifdef BT_OPP_SERVER
#define BT_APPL_OPS_STACK_SIZE		8*1024		// 8K
#endif
#else
#define BT_SCHED_STACK_SIZE			32*1024		// 24K
#define BT_APPL_AV_STACK_SIZE		8*1024		// 8K
#define BT_APPL_HF_STACK_SIZE		8*1024		// 8K
#ifdef BT_OPP_CLIENT
#define BT_APPL_OPC_STACK_SIZE     	8*1024		// 8K
#endif
#ifdef BT_OPP_SERVER
#define BT_APPL_OPS_STACK_SIZE     	8*1024		// 8K
#endif
#endif 	// BT_PBAP_CLIENT	
#else
#ifdef BT_PBAP_CLIENT
#define BT_SCHED_STACK_SIZE			24*1024		// 24K
#define BT_APPL_AV_STACK_SIZE		8*1024		// 8K
#define BT_APPL_HF_STACK_SIZE		8*1024		// 8K
#define BT_APPL_PAC_STACK_SIZE		8*1024		// 8K
#else
#define BT_SCHED_STACK_SIZE			16*1024		// 16K
#define BT_APPL_AV_STACK_SIZE		8*1024		// 8K
#define BT_APPL_HF_STACK_SIZE		8*1024		// 8K
#endif 	// BT_PBAP_CLIENT	
#endif	//BT_OPP_CLIENT

#ifdef BLUETOOTH_FTP_CLIENT_INCLUDE
#define BT_APPL_FTC_STACK_SIZE		8*1024
#endif
#ifdef  BLUETOOTH_FTP_SERVER_INCLUDE
#define BT_APPL_FTS_STACK_SIZE		8*1024
#endif

#define HISR_BT_RX_STACKSIZE		1600	//400
#define HISR_BT_TX_STACKSIZE		1600	//400
#define HISR_BT_AV_FILTER_STACKSIZE	1600	//400
#endif

#else	/* BRWS_MINI_DB */

#ifdef BLUETOOTH_INCLUDE	//Modified by ara on Version [08/04/00]
#if defined (BT_OPP_CLIENT) || defined (BT_OPP_SERVER)
#ifdef BT_PBAP_CLIENT
#define BT_SCHED_STACK_SIZE			80*1024
#define BT_APPL_AV_STACK_SIZE		16*1024
#define BT_APPL_HF_STACK_SIZE		16*1024
#define BT_APPL_PAC_STACK_SIZE		16*1024
#ifdef BT_OPP_CLIENT
#define BT_APPL_OPC_STACK_SIZE		16*1024
#endif
#ifdef  BT_OPP_SERVER
#define BT_APPL_OPS_STACK_SIZE		16*1024
#endif
#else
#define BT_SCHED_STACK_SIZE			64*1024
#define BT_APPL_AV_STACK_SIZE		16*1024
#define BT_APPL_HF_STACK_SIZE		16*1024
#ifdef BT_OPP_CLIENT
#define BT_APPL_OPC_STACK_SIZE		16*1024
#endif
#ifdef BT_OPP_SERVER
#define BT_APPL_OPS_STACK_SIZE		16*1024
#endif
#endif 	// BT_PBAP_CLIENT	
#else
#ifdef BT_PBAP_CLIENT
#define BT_SCHED_STACK_SIZE			48*1024
#define BT_APPL_AV_STACK_SIZE		16*1024
#define BT_APPL_HF_STACK_SIZE		16*1024
#define BT_APPL_PAC_STACK_SIZE		16*1024
#else
#define BT_SCHED_STACK_SIZE			32*1024
#define BT_APPL_AV_STACK_SIZE		16*1024
#define BT_APPL_HF_STACK_SIZE		16*1024
#endif 	// BT_PBAP_CLIENT	
#endif	//BT_OPP_CLIENT

#ifdef BLUETOOTH_FTP_CLIENT_INCLUDE
#define BT_APPL_FTC_STACK_SIZE		16*1024
#endif
#ifdef  BLUETOOTH_FTP_SERVER_INCLUDE
#define BT_APPL_FTS_STACK_SIZE		16*1024
#endif

#ifdef BT_SPP_INCLUDE
#define BT_APPL_SPP_STACK_SIZE		16*1024
#endif

#define HISR_BT_RX_STACKSIZE		1600	//400
#define HISR_BT_TX_STACKSIZE		1600	//400
#define HISR_BT_AV_FILTER_STACKSIZE	1600	//400
#endif
#endif	/* BRWS_MINI_DB */

#define	DEMUX_STACKSIZE				16*1024	//8192		//32K	// DemuxTask
#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
#define	DEMUX2_STACKSIZE			16*1024	//8192		//32K	// Demux2Task
#endif
#define	SYSMON_STACKSIZE			8*1024	//4096		//16K	// SysMonTask
#ifdef _TOUCHSCREEN_INCLUDE_
#define TCC_TOUCHSCR_Q_SIZE			10
extern TC_QUEUE TCC_TOUCHSCREEN_Q;
#define TOUCHSCREEN_STACKSIZE		2000 
#endif

//jckim_0106
#define BT_STACKSIZE		4*1024 



#ifdef USE_DISPLAY_OBJECT
#define BI_STACKSIZE				2000 
#endif

#define	MOUNT_STACKSIZE				16*1024	 /* Mount Task */

extern TC_HISR HISR_SCALE_Control;
extern TC_HISR HISR_LCD_Control;

#ifdef HACO_INCLUDE
extern TC_HISR HISR_CAN_Control;
#endif // HACO_INCLUDE

/*
extern unsigned int Demon_Stack[];
extern unsigned int Audio_Stack[];
#ifdef CD_RIPPING_INCLUDE
extern unsigned int Ripping_Stack[];
#endif
extern unsigned int Demux_Stack[];
#ifdef CD_INCLUDE
extern unsigned int CD_Stack[];
#endif
extern unsigned int SysMon_Stack[];
*/

#ifdef BLUETOOTH_INCLUDE
extern void *BT_SCHED_STACK;
extern void *BT_APPL_AV_STACK;
extern void *BT_APPL_HF_STACK;
#ifdef BT_PBAP_CLIENT
extern void *BT_APPL_PAC_STACK;
#endif // BT_PBAP_CLIENT
#ifdef BT_OPP_CLIENT
extern void *BT_APPL_OPC_STACK;
#endif // BT_OPP_CLIENT
#ifdef BT_OPP_SERVER
extern void *BT_APPL_OPS_STACK;
#endif // BT_OPP_SERVER
#ifdef BLUETOOTH_FTP_CLIENT_INCLUDE
extern void *BT_APPL_FTC_STACK;
#endif // BLUETOOTH_FTP_CLIENT_INCLUDE
#ifdef BLUETOOTH_FTP_SERVER_INCLUDE
extern void *BT_APPL_FTS_STACK;
#endif // BLUETOOTH_FTP_SERVER_INCLUDE
#ifdef BT_SPP_INCLUDE
extern void *BT_APPL_SPP_STACK;
#endif // BT_SPP_INCLUDE
extern void *HISR_BT_RX_STACK;
extern void *HISR_BT_TX_STACK;
extern void *HISR_BT_AV_FILTER_STACK;
#endif // BLUETOOTH_INCLUDE



//-----------------------------------------------
// Task Prototype
//-----------------------------------------------
extern void TCCMountTask(UNSIGNED argc, VOID *argv); /* Mount Task */
extern void TCCBootTask(UNSIGNED argc, VOID *argv);

#ifdef COMMON_COM_INCLUDE
extern void TCC_CommonComInit(void);
extern void TCCCommonComTask(UNSIGNED argc, VOID * argv);
#endif // COMMON_COM_INCLUDE

#ifdef CD_INCLUDE
extern void TCCCDTask(UNSIGNED argc, VOID *argv);
#endif // CD_INCLUDE

extern void TCCDemonTask(UNSIGNED argc, VOID *argv);
extern void TCCAudioTask(UNSIGNED argc, VOID *argv);

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern void TCCAudio2Task(UNSIGNED argc, VOID *argv);
#endif // DUAL_PLAYBACK_INCLUDE

#ifdef CD_RIPPING_INCLUDE
extern void TCCRippingTask(UNSIGNED argc, VOID *argv);
#if defined(FC_RIPPING) || defined(FS_RIPPING)
extern void TCCRipbufTask(UNSIGNED argc, VOID *argv);
#endif // FC_RIPPING
#endif // CD_RIPPING_INCLUDE

#ifdef SDRAM_2M
extern void TCCKeyActTask(UNSIGNED argc, VOID *argv);
#endif // SDRAM_2M
extern void TCCVideoTask(UNSIGNED argc, VOID *argv);
#ifdef STILLIMAGE_INCLUDE
extern void TCCSImageTask(UNSIGNED argc, VOID *argv);
#endif // STILLIMAGE_INCLUDE
#ifdef UHP_INCLUDE
extern void TCCUSBTask(UNSIGNED argc, VOID *argv);
#endif // UHP_INCLUDE
#ifdef WMV_INCLUDE
extern void TCCAsfParsingTask(UNSIGNED argc, VOID * argv);
#endif // WMV_INCLUDE

#ifdef _MAKE_PRIVATE1_TASK_
extern void TCCDxBPrivate1Task(UNSIGNED argc, VOID *argv);
#endif // _MAKE_PRIVATE1_TASK_

#ifdef CAPTION_INCLUDE
extern void TCCDxBSubtitleTask(UNSIGNED argc, VOID *argv);
#endif // CAPTION_INCLUDE

extern void TCCDemuxTask(UNSIGNED argc, VOID *argv);

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern void TCCDemux2Task(UNSIGNED argc, VOID *argv);
#endif // DUAL_PLAYBACK_INCLUDE

extern void TCCSysMonTask(UNSIGNED argc, VOID *argv);

#ifdef BLUETOOTH_INCLUDE
extern void TCCBluetoothSchedTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplAvTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplHfTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplPacTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplOpcTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplOpsTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplFtcTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplFtsTask(UNSIGNED argc, VOID *argv);
extern void TCCBluetoothApplSppTask(UNSIGNED argc, VOID *argv);
#endif // BLUETOOTH_INCLUDE

extern	void TCCFwdnTask(UNSIGNED argc, VOID *argv) ;

#ifdef CD_ALPINE_DRV
extern void CDControlTask(UNSIGNED argc, VOID * argv);
#endif // CD_ALPINE_DRV

#define	HISR_SCALE_STACKSIZE		1024
#define	HISR_LCD_STACKSIZE			800

#ifdef _TOUCHSCREEN_INCLUDE_
	extern void TCCTouchScreenTask(UNSIGNED argc, VOID *argv);
#endif // _TOUCHSCREEN_INCLUDE_

//jckim_0106
extern void BtMainTask(UNSIGNED argc, VOID *argv);


//-----------------------------------------------
// Tasks names
//-----------------------------------------------
typedef enum	// CAUTION! Max is 0xFF
{
	TASK_PRIORITY_HISR = 0x02,
	TASK_PRIORITY_BOOT,

#ifdef	COMMON_COM_INCLUDE
	TASK_PRIORITY_COMMCOM,
#endif // COMMON_COM_INCLUDE

#if defined(_TOUCHSCREEN_INCLUDE_)
	TASK_PRIORITY_TOUCHSCREEN,
#endif // _TOUCHSCREEN_INCLUDE_

#ifdef HACO_INCLUDE
	TASK_ID_BOOT,			// = 0x06,
	TASK_TMR_ID,
	TASK_ID_UI,
	TASK_ID_PMA,

	TASK_ID_IPOD_COMM,

	TASK_ID_BT,		// for BT Task
	TASK_ID_BT_TX,	// BCSP Tx
	TASK_ID_BT_SCH,	// BCSP Scheduler
	TASK_ID_BT_RX,	// BCSP Rx
#ifdef PANDORA_INCLUDE
	TASK_ID_PANDORA_RX, //Pandora
	TASK_ID_PANDORA_SX,
	TASK_ID_PANDORA,
#endif	
	TASK_ID_CD,
	TASK_ID_RADIO,
	TASK_ID_IPOD,

#ifdef HACO_INCLUDE
	TASK_PRIORITY_DEMON,
#endif

//jckim_0106
	TASK_PRIORITY_BTSTACK,

	TASK_ID_DISP,
#ifdef SPEECH_RECOGNIZER
	TASK_PRIORITY_VH = TASK_ID_DISP,		// for VoiceHandler Main Task
	TASK_PRIORITY_SDS = TASK_PRIORITY_VH,		// for SDS Main Task
	TASK_PRIORITY_SDS_SUB1 = TASK_PRIORITY_VH,
	TASK_PRIORITY_SDS_SUB2 = TASK_PRIORITY_VH,	// for TTS Engine
#endif // SPEECH_RECOGNIZER

#ifdef XM_INCLUDE
	TASK_ID_XM_COMM,
	TASK_ID_XM,
	XM_DBG_TASK,
#endif // XM_INCLUDE

#ifdef SIRIUS_INCLUDE
	TASK_ID_SIRIUS_COMM,
	TASK_ID_SIRIUS,
	TASK_ID_SIRIUS_DBG,	
#endif // SIRIUS_INCLUDE

#ifdef DAB_PM_INCLUDE
	TASK_ID_DAB_COMM,
	TASK_ID_DAB,
	TASK_ID_DAB_DBG,
	TASK_ID_DAB_FWDN,
#endif

#ifdef CD_INCLUDE
	TASK_PRIORITY_CD,
#endif // CD_INCLUDE

#ifdef CD_ALPINE_DRV
	TASK_PRIORITY_CDCTRL,
#endif // CD_ALPINE_DRV

#ifdef TMU_INCLUDE
	TASK_ID_TMU,
#endif // TMU_INCLUDE

	TASK_ID_FA,

#endif // HACO_INCLUDE


	TASK_PRIORITY_ASF_PARSING,
	TASK_PRIORITY_SYNC_LAYER = TASK_PRIORITY_ASF_PARSING,
	TASK_PRIORITY_HEADER_DECODE = TASK_PRIORITY_ASF_PARSING,

	//TASK_PRIORITY_BOOT,
	TASK_PRIORITY_UI,
#ifndef HACO_INCLUDE
	TASK_PRIORITY_DEMON,
#endif

#ifdef USBH_EVENT_TASK_INCLUDE
	TASK_PRIORITY_USBH_EVENT,
#endif // USBH_EVENT_TASK_INCLUDE

#ifdef IPOD_INCLUDE
	TASK_PRIORITY_IPOD,
#endif // IPOD_INCLUDE

#ifdef BLUETOOTH_INCLUDE
	TASK_PRIORITY_BT_SCHED,
	TASK_PRIORITY_BT_APPL_AV = TASK_PRIORITY_BT_SCHED,
	TASK_PRIORITY_BT_APPL_HF = TASK_PRIORITY_BT_SCHED,
#ifdef BT_PBAP_CLIENT
	TASK_PRIORITY_BT_APPL_PB = TASK_PRIORITY_BT_SCHED,
#endif // BT_PBAP_CLIENT
#ifdef BT_OPP_CLIENT
	TASK_PRIORITY_BT_APPL_OPC = TASK_PRIORITY_BT_SCHED,
#endif // BT_OPP_CLIENT
#ifdef BT_OPP_SERVER
	TASK_PRIORITY_BT_APPL_OPS = TASK_PRIORITY_BT_SCHED,
#endif // BT_OPP_SERVER
#ifdef BLUETOOTH_FTP_CLIENT_INCLUDE
	TASK_PRIORITY_BT_APPL_FTC = TASK_PRIORITY_BT_SCHED,
#endif // BLUETOOTH_FTP_CLIENT_INCLUDE
#ifdef BLUETOOTH_FTP_SERVER_INCLUDE
	TASK_PRIORITY_BT_APPL_FTS = TASK_PRIORITY_BT_SCHED,
#endif // BLUETOOTH_FTP_SERVER_INCLUDE
#ifdef BT_SPP_INCLUDE
	TASK_PRIORITY_BT_APPL_SPP = TASK_PRIORITY_BT_SCHED,
#endif // BT_SPP_INCLUDE
#endif // BLUETOOTH_INCLUDE

#ifdef GPS_APPLICATION_INCLUDE
	TASK_PRIORITY_GPS_APPL,
#endif // GPS_APPLICATION_INCLUDE

	TASK_PRIORITY_AUDIO,


#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
	TASK_PRIORITY_AUDIO2 = TASK_PRIORITY_AUDIO,
#endif // DUAL_PLAYBACK_INCLUDE

#ifdef SPEECH_RECOGNIZER
	TASK_PRIORITY_AAST,
#endif

#ifdef CD_RIPPING_INCLUDE
	TASK_PRIORITY_RIPPING,
#endif // CD_RIPPING_INCLUDE

#ifdef SDRAM_2M
	TASK_PRIORITY_KEYACT,
#endif // SDRAM_2M

	TASK_PRIORITY_VIDEO,

#ifdef UHP_INCLUDE
	TASK_PRIORITY_USB,
#endif // UHP_INCLUDE	

#ifdef _MAKE_PRIVATE1_TASK_
	TASK_PRIORITY_DXBPRIVATE1 = TASK_PRIORITY_VIDEO+2,
#endif // _MAKE_PRIVATE1_TASK_

	TASK_PRIORITY_DXBSUBTITLE = TASK_PRIORITY_VIDEO+2,
	TASK_PRIORITY_TEST_DSMCC = TASK_PRIORITY_VIDEO+2,
	
	
	TASK_PRIORITY_DISP_TFT,

#ifdef CAN_INCLUDE
	TASK_PRIORITY_CAN,
#endif // CAN_INCLUDE

#ifdef STILLIMAGE_INCLUDE
	

#if 0


#ifdef USE_DISPLAY_OBJECT
	TASK_PRIORITY_BI = TASK_PRIORITY_SIMAGE,
#endif // USE_DISPLAY_OBJECT

#else // USE_DISPLAY_OBJECT
#ifdef USE_DISPLAY_OBJECT
	TASK_PRIORITY_BI,
#endif // USE_DISPLAY_OBJECT

#endif

	TASK_PRIORITY_SIMAGE,

#endif // USE_DISPLAY_OBJECT

	TASK_PRIORITY_DEMUX,
	

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
	TASK_PRIORITY_DEMUX2 = TASK_PRIORITY_DEMUX,
#endif // DUAL_PLAYBACK_INCLUDE

#ifdef CD_RIPPING_INCLUDE
	TASK_PRIORITY_RIPBUF = TASK_PRIORITY_DEMUX,
#endif // CD_RIPPING_INCLUDE

	TASK_PRIORITY_MOUNT = TASK_PRIORITY_DEMUX,	/* Mount Task */
	
#ifdef SPEECH_RECOGNIZER
		TASK_PRIORITY_VH_SUB = TASK_PRIORITY_DEMUX, // for VAD & VAE List Make
		TASK_PRIORITY_SDS_SUB3 = TASK_PRIORITY_VH_SUB,	// for ASR Init, TTS Init, DP
#endif // SPEECH_RECOGNIZER

	TASK_PRIORITY_LOW_PRIORITY,

#ifdef	CAS_CAAGENT_INCLUDE
	TASK_PRIORITY_CAAGENT = TASK_PRIORITY_RIPBUF+1,
#endif // CAS_CAAGENT_INCLUDE

#ifdef	DREAMERICAS_INCLUDE
	TASK_PRIORITY_DREAMERICAS = TASK_PRIORITY_RIPBUF+1,
#endif // DREAMERICAS_INCLUDE

#ifdef UART_CONSOLE_INCLUDE
	TASK_PRIORITY_CONSOLE,
#endif // UART_CONSOLE_INCLUDE

	TASK_PRIORITY_SYSMON,
	TASK_PRIORITY_TOCHECK
} TASK_NAME;



//-----------------------------------------------
// Time Slice for Tasks ( Ticks : 5*Tick(ms) )
//-----------------------------------------------
#define	BOOT_TIMESLICE			20

#ifdef CD_INCLUDE
#define	CD_TIMESLICE			20
#endif // CD_INCLUDE

#define	DEMON_TIMESLICE			20
#define	AUDIO_TIMESLICE			4

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
#define	AUDIO2_TIMESLICE		4
#endif // DUAL_PLAYBACK_INCLUDE

#ifdef CD_RIPPING_INCLUDE
#define	RIPPING_TIMESLICE		20
#define	RIPBUF_TIMESLICE		4
#endif // CD_RIPPING_INCLUDE

#define	VIDEO_TIMESLICE			20

#ifdef STILLIMAGE_INCLUDE
#define	SIMAGE_TIMESLICE		20
#endif // STILLIMAGE_INCLUDE

#ifdef UHP_INCLUDE
#define	USB_TIMESLICE			20
#endif // UHP_INCLUDE

#ifdef WMV_INCLUDE
#define	ASF_PARSING_TIMESLICE	20
#endif // WMV_INCLUDE

#ifdef _MAKE_PRIVATE1_TASK_
#define DXB_PRIVATE1_TIMESLICE	20
#endif // _MAKE_PRIVATE1_TASK_

#ifdef CAPTION_INCLUDE
#define DXB_SUBTITLE_TIMESLICE	20
#endif // CAPTION_INCLUDE

#ifdef BRWS_MINI_DB
#define	KEYACT_TIMESLICE		20
#endif // BRWS_MINI_DB

#ifdef BLUETOOTH_INCLUDE	//Modified by ara on Version [08/04/00]
#define BT_SCHED_TIMESLICE		20
#define BT_APPL_AV_TIMESLICE	20
#define BT_APPL_HF_TIMESLICE	20
#define BT_APPL_PAC_TIMESLICE	20
#define BT_APPL_OPC_TIMESLICE	20
#define BT_APPL_OPS_TIMESLICE	20
#define BT_APPL_FTC_TIMESLICE	20
#define BT_APPL_FTS_TIMESLICE	20
#define BT_APPL_SPP_TIMESLICE	20
#endif // BLUETOOTH_INCLUDE

#define	DEMUX_TIMESLICE			4
#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
#define	DEMUX2_TIMESLICE		4
#endif // DUAL_PLAYBACK_INCLUDE

#define MOUNT_TIMESLICE			4	/* Mount Task */

#define	SYSMON_TIMESLICE		20
#ifdef _TOUCHSCREEN_INCLUDE_
#define TOUCHSCREN_TIMESLICE	20
#endif // _TOUCHSCREEN_INCLUDE_

//jckim_0106
#define BTSTACK_TIMESLICE	20



//-----------------------------------------------
// Define Tasks
//-----------------------------------------------
extern TC_TASK BOOT_TASK;

#ifdef COMMON_COM_INCLUDE
extern TC_TASK COMMON_COM_TASK;
#endif // COMMON_COM_INCLUDE

#ifdef CD_INCLUDE
extern TC_TASK CD_TASK;
#endif // CD_INCLUDE

extern TC_TASK DEMON_TASK;
extern TC_TASK AUDIO_TASK;

#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern TC_TASK AUDIO2_TASK;
#endif // DUAL_PLAYBACK_INCLUDE

#ifdef CD_RIPPING_INCLUDE
extern TC_TASK RIPPING_TASK;
#endif // CD_RIPPING_INCLUDE

#ifdef SDRAM_2M
extern TC_TASK KEYACT_TASK;
#endif // SDRAM_2M

#ifdef _MULTIMEDIA_PLAYER_
extern TC_TASK VIDEO_TASK;
#endif // _MULTIMEDIA_PLAYER_

#ifdef STILLIMAGE_INCLUDE
extern TC_TASK SIMAGE_TASK;
#endif // STILLIMAGE_INCLUDE

#ifdef UHP_INCLUDE
extern TC_TASK USB_TASK;
#endif // UHP_INCLUDE

#ifdef WMV_INCLUDE
extern TC_TASK ASF_PARSING_TASK;
#endif // WMV_INCLUDE

#ifdef _MAKE_PRIVATE1_TASK_
extern TC_TASK DXB_PRIVATE1_TASK;
#endif // _MAKE_PRIVATE1_TASK_

#ifdef CAPTION_INCLUDE
extern TC_TASK DXB_SUBTITLE_TASK;
#endif // CAPTION_INCLUDE

extern TC_TASK DEMUX_TASK;
#if defined(DUAL_PLAYBACK_INCLUDE) || defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern	TC_TASK DEMUX2_TASK;
#endif // DUAL_PLAYBACK_INCLUDE

#ifdef CD_RIPPING_INCLUDE
extern TC_TASK RIPBUF_TASK;
#endif // CD_RIPPING_INCLUDE

extern TC_TASK SYSMON_TASK;

#ifdef BLUETOOTH_INCLUDE
extern TC_TASK BT_SCHED_TASK;
extern TC_TASK BT_APPL_AV_TASK;
extern TC_TASK BT_APPL_HF_TASK;
#ifdef BT_PBAP_CLIENT
extern TC_TASK BT_APPL_PAC_TASK;
#endif // BT_PBAP_CLIENT
#ifdef BT_OPP_CLIENT
extern TC_TASK BT_APPL_OPC_TASK;
#endif // BT_OPP_CLIENT
#ifdef BT_OPP_SERVER
extern TC_TASK BT_APPL_OPS_TASK;
#endif // BT_OPP_SERVER
#ifdef BLUETOOTH_FTP_CLIENT_INCLUDE
extern TC_TASK BT_APPL_FTC_TASK;
#endif // BLUETOOTH_FTP_CLIENT_INCLUDE
#ifdef BLUETOOTH_FTP_SERVER_INCLUDE
extern TC_TASK BT_APPL_FTS_TASK;
#endif // BLUETOOTH_FTP_SERVER_INCLUDE
#ifdef BT_SPP_INCLUDE
extern TC_TASK BT_APPL_SPP_TASK;
#endif // BT_SPP_INCLUDE
#endif // BLUETOOTH_INCLUDE

extern	TC_TASK MOUNT_TASK; /* Mount Task */

#ifdef _TOUCHSCREEN_INCLUDE_
extern TC_TASK TOUCHSCREEN_TASK;
#endif // _TOUCHSCREEN_INCLUDE_

//jckim_0106
extern TC_TASK BTMAINTASK;


#define MEM_INIT_VALUE		'A'	// Changed from 0xAB



//-----------------------------------------------
// Define Timer
//-----------------------------------------------
#define TCC_TIME_TIMER_ID		0x30
#define TCC_TIME_TIMER_TIME 	99 //198
#define KEY_TIMER_ID			0x10
#define KEY_RESCHEDULE_TIME 	4

#ifdef HACO_INCLUDE
#define RDS_TIMER_ID		0x11
#define RDS_TIMER_TIME	1	// 5ms
#else // HACO_INCLUDE
#define ANIMATION_TIMER_ID		0x11
#define ANIMATION_TIMER_TIME	2	// 10ms
#endif // HACO_INCLUDE

#ifdef BT_HFP_LOOPBACK_TEST
#define LOOPBACK_TIMER_ID		0x50
#define LOOPBACK_RESCHEDULE_TIME 	2
#endif // BT_HFP_LOOPBACK_TEST

extern TC_TIMER TCCTimeTimer;
extern TC_TIMER RdsCheckTimer;

extern VOID Timer_TCCTimefn(unsigned long);
extern TC_TIMER InputKeyTimer;

#ifndef HACO_INCLUDE
extern VOID Timer_InputKey_fn(unsigned long);
#endif // HACO_INCLUDE

extern TC_TIMER gUiAnimationTimer;
extern VOID Timer_Animation_fn(unsigned long);

#ifdef BT_HFP_LOOPBACK_TEST
extern TC_TIMER LoopbackTimer;
extern VOID Timer_LoopbackCallback(unsigned long temp);
#endif // BT_HFP_LOOPBACK_TEST



//-----------------------------------------------
// Define HISR
//-----------------------------------------------
#ifdef BLUETOOTH_INCLUDE
extern TC_HISR HISR_BT_Rx_Control;
extern TC_HISR HISR_BT_Tx_Control;
extern TC_HISR HISR_BT_Av_Filter_Control;
extern void NucleusRxIrqRoutine(void);
extern void NucleusTxIrqRoutine(void);
extern void NucleusAvFilterIrqRoutine(void);
#endif // BLUETOOTH_INCLUDE

#if defined(CROSSFADE_INCLUDE) || defined(GAPLESS_INCLUDE)
extern TC_HISR HISR_DAI_SetFR_Control;
#define HISR_DAI_SetFR_STACKSIZE	400
#endif // CROSSFADE_INCLUDE


#endif /* _TCCRESOURCE_H_ */
