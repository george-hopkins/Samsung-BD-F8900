/****************************************************************************
 *   FileName    : FSAPP.h
 *   Description : File System refer for K-FileSystem
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef __FSAPP_H__
#define __FSAPP_H__

#ifndef	FWDN_DOWNLOADER_INCLUDE
#include "FS/file.h"
#else	//FWDN_DOWNLOADER_INCLUDE
#include "FS/file.h"
#endif	//FWDN_DOWNLOADER_INCLUDE

#include "include/main.h"

#define BRWS_EXT_LENGTH		4

enum
{
	BRWS_EXT_MP3 = 0,
#ifdef WMA_INCLUDE
	BRWS_EXT_WMA,
#endif
	BRWS_EXT_MP2,
#ifdef WAV_INCLUDE
	BRWS_EXT_WAV,
#endif
#ifdef AAC_INCLUDE
	BRWS_EXT_AAC,
#endif
#ifdef MP4_DEC_INCLUDE
	BRWS_EXT_MP4,
#endif
#ifdef M2V_DEC_INCLUDE
	BRWS_EXT_M2V,
#endif
#ifdef WMV_INCLUDE
	BRWS_EXT_WMV,
#endif
#ifdef TES_CONTAINER_INCLUDE
	BRWS_EXT_TES,
#endif
#ifdef AAC_LC_INCLUDE
	BRWS_EXT_CAM,
#endif
#ifdef M3U_INCLUDE
	BRWS_EXT_M3U,
#endif
#ifdef JPG_DEC_INCLUDE
	BRWS_EXT_JPG,
#endif
#ifdef TXT_DEC_INCLUDE
	BRWS_EXT_TXT,
#endif
#ifdef BMP_DEC_INCLUDE
	BRWS_EXT_BMP,
#endif
#ifdef PNG_DEC_INCLUDE
	BRWS_EXT_PNG,
#endif
#ifdef GIF_DEC_INCLUDE
	BRWS_EXT_GIF,
#endif
#ifdef MPEG4_CONTAINER_INCLUDE
	BRWS_EXT_MPEG4,
#endif
#ifdef DAB_PLUS_INCLUDE
	BRWS_EXT_DABP,
#endif
#ifdef BLUETOOTH_INCLUDE
	BRWS_EXT_VCARD,
#endif
#if 0 /*#ifdef CD_INCLUDE*/
	BRWS_EXT_ROM,	//CDFW
#endif
	BRWS_EXT_MAX
};

/*****************************************************************************/
//	Disk Device POWER MODE : STANDBY,SLEEP,IDLE,ACTIVE
/*****************************************************************************/
/*
#define DISK_STANDBY     0	// accept command but the media is not immediately accessible.
#define DISK_IDLE            1	// accessible media as take some time
#define DISK_SLEEP          2	// ALL OFF. The drive requires a reset to be activated.
*/

#define	DEVICE_BUSY		1
#define	DEVICE_NOBUSY		0

#ifndef CLEAR
#define CLEAR				0
#endif
#ifndef DIRTY
#define DIRTY				1
#endif


typedef void (*FS_LCD_Display_Handler)( unsigned int Lun, unsigned int STATUS_CODE, unsigned long WParam );


/*****************************************************************************/
//	DEFINITION FOR FILE-SYSTEM MEMORY
/*****************************************************************************/
//#define	MAX_COPY_SIZE				(5*1024*1024)

/*****************************************************************************/
//	DEFINITION FOR MAXIMUM HANDLE OF FILES OR DIRECTORIES
/*****************************************************************************/
#define MAX_HANDLE		20	// Changed open file count: 5 -> 20
#define MAX_FD			MAX_HANDLE
#define MAX_DIR			10
//dir_num 1 : front play & DB
//dir_num 2 : front navi
//dir_num 3 : rear play & DB // BT
//dir_num 4 : rear navi	// FA(Factory Autocheck)
//dir_num 5 : IVR DB & ASR Main
//dir_num 6 : Mount
//dir_num 7 : file recover(create/delete/copy)
//dir_num 8 : Check IVR Size
//dir_num 9 : ASR Sub Task
//dir_num 10 : AAST(Automatic ASR System Tester)
#define MAX_PARTITION	10
/*****************************************************************************/
//	DEFAULT FILE FORMAT
/*****************************************************************************/
#define DEFAULT_ROOT_ENTRY_COUNT		512  // normal number of the root entry count
#define HIDDEN_SIZE						31	// NAND, UFD etc... But NOT HDD


/*****************************************************************************/
//	GLOBAL VARIABLES OR STRUCTURE
/*****************************************************************************/
extern int sCurrentCodec;

extern FDIRENTstruc fd[MAX_FD];
extern HANDLERstruc fhandler[MAX_HANDLE];
extern MFILEstruc mfile[MAX_HANDLE];
extern int totalHdlr;
extern FDIRENTstruc fdir[MAX_DIR];

extern int max_dir;
extern int max_handle;
extern int max_partition;

extern char * const ppSupportFileList[BRWS_EXT_MAX];

#define ENTRY_SIZE		32    // 1 entry size [byte]
//#define ENTRY_CHUNK	11
//#define ENTRY_CHUNK	(255 / 13) + 2
#define ENTRY_CHUNK	21
#define ENTRY_BUFFER_SIZE	(ENTRY_CHUNK * ENTRY_SIZE)

#define EXTEND_ENTRY_BUFFER
#ifdef EXTEND_ENTRY_BUFFER
extern unsigned char entry_buffer[ENTRY_BUFFER_SIZE * MAX_HANDLE]; // entry chunk buffer
#else
extern unsigned char entry_buffer[ENTRY_BUFFER_SIZE]; // entry chunk buffer
#endif

//#ifdef SKIP_FREE_CLUSTER
/*#define MAX_CLUSTER_SIZE	65536  [byte] = maximum bytes per cluster	*/
//#define	MAX_CLUSTER_WORD_SIZE	65536/4
/* unsigned int fat_mount_buffer[65536/sizeof(int)];  64 [kbyte]; */
//#else
//#define	MAX_CLUSTER_WORD_SIZE	1
//#define	MAX_CLUSTER_WORD_SIZE	65536/4
//#endif

//extern unsigned int fat_mount_buffer[MAX_CLUSTER_WORD_SIZE];

#define ROOTDIR	0
#define _MUSICFILE 1
#define _NOTMUSICFILE -1
#define WITHNULL 0
#define _DIR		0
#define _FILE		1

extern int gDiskIdleTime;

extern FS_LCD_Display_Handler CALLBACK_LcdDisplayFS;
extern void	FS_DRV_Set_LCD_Display_Handler( FS_LCD_Display_Handler LCD_CallBackRoutine );


#ifndef HACO_INCLUDE
extern int 	FSAPP_GetTotalPlayFileNumber(int dir_num);
#endif
extern int 	FSAPP_CurrentPlayIndex2FileIndex(int PlayIndex, int dir_num);
extern int 	FSAPP_Strcmp(const unsigned char *src1, const unsigned char *src2, int attr);
extern int 	FSAPP_CorrectDir(unsigned char *src);
extern int 	FSAPP_GetAvailOffset(const unsigned char *filename, int dir_num);
#ifdef HACO_INCLUDE
extern int _FSAPP_GetAvailOffset(const unsigned char *filename, int dir_num);
#endif

//-------------------------------------------------------------

extern 	int 	FSAPP_InitDiskDevice(int init_DeviceID, int DevNo);
extern 	int    FSAPP_GetMaxCopySize(void);
extern unsigned char *FSAPP_GetFileBuffer(void);
extern void FSAPP_InitializeFS(void);
extern int FSAPP_CheckRootDir(int dir_num);
extern unsigned long FSAPP_GetDiskSector(unsigned char drvType, DISKINFOstruc *disk);
#ifdef MTP_HOST_INCLUDE
extern int FSAPP_CheckPartitionIDisMTP(int partID);
#endif

extern int	FSAPP_GetFlagOfDiskDataChanged( int DeviceNum );
extern void FSAPP_SetFlagOfDiskDataChanged( int DeviceNum, int SetValue );

extern int  FSAPP_DrawPercent_CALLBACK_GetEnable(void);
extern void FSAPP_DrawPercent_CALLBACK_SetEnable(int uiNewStatus);
extern int FSAPP_DrawPercent_CALLBACK_GetLocation(void);
extern void FSAPP_DrawPercent_CALLBACK_SetLocation(int uiLocation);
extern void FSAPP_DrawPercent_CALLBACK(unsigned short uiPercent);
extern int FSAPP_RecoverCluster(int part_id);
extern int FSAPP_SetClearCluster_CALLBACK(unsigned long start_cluster);
extern unsigned long FSAPP_GetClearCluster_CALLBACK(void);

extern int FSAPP_Recover(int part_id, char *dir_name, char *file_name);
extern int FSAPP_MakeStart(int sdir_num, char *sfile_name, int isUnicode, int part_id, char *dir_name, char *file_name);
extern int FSAPP_MakeEnd(int handle);

extern void* FS_Malloc(unsigned int a);
extern int FS_Dealloc(void* a);
#endif /* __FSAPP_H__ */

/* End of File */

