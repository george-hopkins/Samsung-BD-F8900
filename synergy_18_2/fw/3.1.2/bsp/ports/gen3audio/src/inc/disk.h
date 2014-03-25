/****************************************************************************
 *   FileName    : Disk.h
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef __DISK_H__
#define __DISK_H__

/*******************************************************************************
 * DISK Interface Error Code Macro
 ******************************************************************************/
#if 1
#define		ENOTSUPPORT	(0x80000000UL | 0x10UL)		// Function does not support
#define		EINITFAIL 		(0x80000000UL | 0x11UL)		// Device Initialization Failed
#else
#define 	_ERR(x)	(0x80000000UL | (x))
#define		ENOTSUPPORT		_ERR(0x10UL)		// Function does not support
#define		EINITFAIL 		_ERR(0x11UL)		// Device Initialization Failed
#endif

/*****************************************************************************
 *	disk device type enumeration value
 *****************************************************************************/
typedef enum
{
	DISK_DEVICE_HDD=0,
	DISK_DEVICE_NAND,
	DISK_DEVICE_HUB,
	DISK_DEVICE_HUB_2,
	DISK_DEVICE_UHP,
	DISK_DEVICE_UHP_2,
	DISK_DEVICE_MMC,
	DISK_DEVICE_MMC_2,
	DISK_DEVICE_MTP,
	DISK_DEVICE_MTP_2,
	DISK_DEVICE_IPOD,
	DISK_DEVICE_IPOD_2,
	DISK_DEVICE_CD,
	DISK_DEVICE_TRIFLASH,
	DISK_DEVICE_MS,
#ifdef BLUETOOTH_INCLUDE
	DISK_DEVICE_BT_SYS,
	DISK_DEVICE_BT_AV,
	DISK_DEVICE_BT_HF,
#endif
	DISK_DEVICE_BT_ROM,

	DISK_DEVICE_SYS,
	MAX_DEVICE_NUM,
	NO_DEVICE_NUM
} DISK_DEVICE;


enum
{
	IPOD_DEVICE_PARTID	= 20,
	IPOD_DEVICE_PARTID_1	= 21,
	CD_DEVICE_PARTID		= 30,
	MTP_DEVICE_PARTID_1	= 40,
	MTP_DEVICE_PARTID_2	= 41,
	MTP_DEVICE_PARTID_3	= 42,
	MTP_DEVICE_PARTID_4	= 43
};


typedef enum
{
#ifdef HDD_INCLUDE
	DEVICE_HDD,
#endif
#ifdef MMC_INCLUDE
	DEVICE_MMC,
	DEVICE_MMC2,
#endif
#ifdef UHP_INCLUDE
	DEVICE_UHP,
	DEVICE_UHP2,
#endif
#ifdef NAND_INCLUDE
	DEVICE_NAND,
#endif
#ifdef TRIFLASH_INCLUDE
	DEVICE_TRIFLASH,
#endif
#ifdef MS_INCLUDE
	DEVICE_MS,
#endif
#ifdef CD_INCLUDE
	DEVICE_CD,
#endif
#if !defined(HDD_INCLUDE) && !defined(MMC_INCLUDE) && !defined(UHP_INCLUDE) && !defined(NAND_INCLUDE) && !defined(TRIFLASH_INCLUDE) && !defined(MS_INCLUDE) && !defined(CD_INCLUDE) 
	DEVICE_NONE,
#endif
	TOTAL_DEVICE_NUM
} DISK_NUM;

#define DISK_DEVICE_INTERNAL 0

/*****************************************************************************
 *	disk device property and sub-property enumeration value
 *****************************************************************************/
typedef enum
{
	DISK_DEVICE_UNLOCK=0,
	DISK_DEVICE_LOCK
} DISK_LOCK_FLAG;

typedef enum
{
	DISK_STATE_NOTMOUNTED,
	DISK_STATE_MOUNTSUCCEED,
	DISK_STATE_MOUNTCOMPLETE,
	DISK_STATE_MOUNTERR
} DISK_MOUNT_STATE;

typedef enum
{
	DISK_STATE_FREE,
	DISK_STATE_BUSY
} DISK_BUSY_STATE;

typedef enum
{
	DISK_STATE_INIT,
	DISK_STATE_POWERON,
	DISK_STATE_POWEROFF,
	DISK_STATE_STANDBY,
	DISK_STATE_IDLE,
	DISK_STATE_SLEEP,
	DISK_STATE_RESET
} DISK_POWER_STATE;

typedef enum
{
	DISK_MSC_DRV_NOT_SUPPORT,
	DISK_MSC_DRV_SUPPORT
} DISK_MSC_DRV_STATE;

typedef enum
{
	DISK_MOUNT_TYPE_INTERNAL,
	DISK_MOUNT_TYPE_EXTERNAL,
	DISK_MOUNT_TYPE_MAXIUM
} DISK_MOUNT_TYPE_STATE;


typedef	struct
{
	const char			*Name;
	DISK_LOCK_FLAG		LockFlag;
	DISK_DEVICE			DiskType;
	DISK_MOUNT_STATE 	MountState;
	DISK_BUSY_STATE		BusyState;
	DISK_MSC_DRV_STATE		MSCDrvSupport;
	DISK_MOUNT_TYPE_STATE	DrvMountType;
	int					PartitionIndex;
	int					DevLun;
	/* 
		Now it is used for iPod device.
		 You can use this handle.
	*/
	void					*Handle;
} DISK_PROPERTY;

/*******************************************************************************
 * DISK Interface Function Type Definitions
 ******************************************************************************/
typedef int (*tDeviceRwFunctions)(int, unsigned long, unsigned short, void *);

typedef int (*tDeviceWriteMultiStartFunctions)(unsigned long, unsigned long);
typedef int (*tDeviceWriteMultiFunctions)(int, unsigned long, unsigned short, void *);
typedef int (*tDeviceWriteMultiStopFunctions)(void);

//Kitty0308
typedef int (*tDeviceReadMultiStartFunctions)(unsigned long, unsigned long);
typedef int (*tDeviceReadMultiFunctions)(int, unsigned long, unsigned short, void *);
typedef int (*tDeviceReadMultiStopFunctions)(void);

typedef unsigned long (*tDeviceHiddenRWFunctions)(unsigned long , unsigned short, unsigned char *);
typedef int (*tDeviceHiddenClearPageFunctions)(unsigned long , unsigned long);

typedef int (*tDeviceIoctlFunctions)(int, void *);

/*******************************************************************************
 * DISK Interface Function Definitions
 ******************************************************************************/
typedef struct DeviceDriverStruct
{
	DISK_PROPERTY						Property;
	tDeviceRwFunctions 					ReadSector;
	tDeviceRwFunctions 					WriteSector;
	tDeviceReadMultiStartFunctions		ReadMultiStart;
	tDeviceReadMultiFunctions			ReadMultiSector;
	tDeviceReadMultiStopFunctions		ReadMultiStop;
	tDeviceWriteMultiStartFunctions		WriteMultiStart;
	tDeviceWriteMultiFunctions			WriteMultiSector;
	tDeviceWriteMultiStopFunctions		WriteMultiStop;
	tDeviceHiddenRWFunctions				HDReadSector;
	tDeviceHiddenRWFunctions				HDWriteSector;
	tDeviceHiddenClearPageFunctions		HDClearSector;
	tDeviceIoctlFunctions 				Ioctl;
} tDeviceDriver;

/*******************************************************************************
 * DISK Ioctl Function List ( Enumeration Value )
 *
 * 		DEV_INITIALIZE
 * 		Initialize Variable , Register and Hardware
 *
 * 		DEV_GET_DISKINFO
 * 		Get the environmant variables like head, cylinder, sector ...
 *
 * 		DEV_FORMAT_DISK
 * 		low level format command ( if it necessary )
 *
 * 		DEV_ERASE_INIT
 * 		prepare erasing command
 *
 * 		DEV_ERASE_BLOCK
 * 		erase sector command
 *
 * 		DEV_ERASE_CLOSE
 * 		finish erasing command
 *
 * 		DEV_WRITEBACK_ON_IDLE
 * 		flush data cache command while system is in idle state
 ******************************************************************************/
typedef enum
{
	/* Do Not Change below functions*/
	DEV_INITIALIZE = 0,
	DEV_GET_DISKINFO,
	DEV_FORMAT_DISK,
	DEV_ERASE_INIT,
	DEV_ERASE_BLOCK,
	DEV_ERASE_CLOSE,
	DEV_WRITEBACK_ON_IDLE,
	/* You can add new function from here */
	DEV_HIDDEN_READ_PAGE_4,
	DEV_GET_MAXMULTISECTOR,
	DEV_SET_MAXMULTISECTOR,
	DEV_SET_POWER,
	DEV_GET_POWER,
	DEV_BOOTCODE_READ_PAGE,
	DEV_BOOTCODE_WRITE_PAGE,
	DEV_SERIAL_PROCESS,
	DEV_GET_MAX_SECTOR_PER_BLOCK,
	DEV_GET_INSERTED,
	DEV_GET_INITED,
	DEV_GET_WRITE_PROTECT,
	DEV_SET_REMOVED,
	DEV_GET_PREV_STATUS,
	DEV_GET_PLAYABLE_STATUS,
	DEV_STOP_TRANSFER,
	DEV_TELL_DATASTARTSECTOR,
	DEV_CHECK_CRC_NANDBOOT_IMAGE_ROM,
	DEV_GET_HIDDEN_SIZE,
	DEV_GET_SUPPORT_FAT_FORMAT,		/* [1429] */
	DEV_EJECT_PREVALLOW,
	DEV_READ_TOC,
	DEV_READ_CDTEXT,
	DEV_SET_CMDOPTION,
	DEV_GET_CMDOPTION,
	DEV_SET_TRAYCTRL,
	DEV_MODE_SENSE,
	DEV_MODE_SELECT,
	DEV_SET_FORCESTOP,
	DEV_RELEASE_FORCESTOP,
	// For HUB
	DEV_REFRESH_HUB,
	DEV_GET_CURRENT_DEVTYPE,
	//************************
	DEV_GET_DEVICE_STATUS, /* Connect or Remove */
	DEV_END_OF_FUNCTION
} IOCTL_FUNCTIONS;

/*******************************************************************************
 * DISK Ioctl DEV_GET_DISKINFO Function Parameter structure
 ******************************************************************************/
typedef struct _ioctl_diskinfo_t
{
	unsigned long head;
	unsigned long cylinder;
	unsigned long sector;
	unsigned long sector_size;
} ioctl_diskinfo_t;

/*******************************************************************************
 * DISK Ioctl DEV_ERASE_INIT Function Parameter structure
 ******************************************************************************/
typedef struct _ioctl_diskeraseinit_t
{
	unsigned short		sector_per_cluster;
	unsigned long		data_start_sector;
} ioctl_diskeraseinit_t;

/*******************************************************************************
 * DISK Ioctl DEV_ERASE_BLOCK Function Parameter structure
 ******************************************************************************/
typedef struct _ioctl_diskerase_t
{
	unsigned long	current_cluster;
	unsigned long	content_fat;
} ioctl_diskerase_t;

/*******************************************************************************
 * DISK Ioctl DEV_HIDDEN_CLEAR_PAGE Function Parameter structure
 ******************************************************************************/
// typedef struct _ioctl_diskhdclear_t {
// 	unsigned long	start_page;
// 	unsigned long	end_page;
// }ioctl_diskhdclear_t;

/*******************************************************************************
 * DISK Ioctl DEV_HIDDEN_READ/WRITE_PAGE Function Parameter structure
 ******************************************************************************/
typedef struct _ioctl_diskhdread4_t
{
	unsigned long	start_page;
	unsigned long	page_offset;
	unsigned long	read_size;
	unsigned char	*buff;
} ioctl_diskhdread4_t;

/*******************************************************************************
 * DISK Ioctl DEV_BOOTCODE_READ/WRITE_PAGE Function Parameter structure
 ******************************************************************************/
typedef struct _ioctl_diskrwpage_t
{
	unsigned long	start_page;
	unsigned long	rw_size;
	unsigned char	*buff;
} ioctl_diskrwpage_t;

/*******************************************************************************
 * DISK ForceStop Parameter Define : Bit Operation
 ******************************************************************************/
#define SYNC_FORCESTOP		0x00000001
#define NOSYNC_FORCESTOP	0x00000002



/*******************************************************************************
 * DISK Interface Function Definitions
 ******************************************************************************/
void 	*DISK_GetHandle( int drv_type);
void 	DISK_SetHandle( int drv_type, void *Handle);

int		DISK_FindDisk( int drv_type );
int		DISK_ReadSector( int drv_type, int lun, unsigned long lba_addr, unsigned short nSector, void *buff);
int		DISK_WriteSector( int drv_type, int lun, unsigned long lba_addr, unsigned short nSector, void *buff);

//Kitty0308
int		DISK_ReadMultiStart( int drv_type, int lba_addr, int size);

int		DISK_ReadMultiSector( int drv_type, int lun, unsigned long lba_addr, unsigned short nSector, void *buff);
int		DISK_ReadMultiStop( int drv_type);
int		DISK_WriteMultiStart( int drv_type, int lba_addr, int size);
int		DISK_WriteMultiSector( int drv_type, int lun, unsigned long lba_addr, unsigned short nSector, void *buff);
int		DISK_WriteMultiStop( int drv_type);
int		DISK_HDReadSector( unsigned int drv_type, unsigned long page_addr, unsigned short count, unsigned char *buff);
int		DISK_HDWriteSector(unsigned int drv_type,  unsigned long page_addr, unsigned short count, unsigned char *buff);
int		DISK_HDClearSector(unsigned int drv_type, unsigned long start_page_addr, unsigned long end_page_addr);
int		DISK_Ioctl( unsigned int drv_type, int function, void *param);
int		DISK_GetTotalDiskCount(void);
int		DISK_GetDiskType(int index);
int		DISK_SetDiskType(int old_DiskType, int new_DiskType);
int 		DISK_GetDiskTypeByPartID( int PartID );
int		DISK_GetSupportMSCDrive( int index );
int		DISK_GetDiskMountType( int index );
int		DISK_SetState(DISK_DEVICE disk,	DISK_MOUNT_STATE	state);
int		DISK_GetState(DISK_DEVICE disk);
int		DISK_SetDiskPartitionIndex(	DISK_DEVICE disk, int	index);
int		DISK_GetDiskPartitionIndex(	DISK_DEVICE disk);
int		DISK_SetDiskDevLun(DISK_DEVICE disk, int lDevLun);
int		DISK_GetDiskDevLun(DISK_DEVICE disk);
int		DISK_SetBusyState(DISK_DEVICE disk, DISK_BUSY_STATE busyFlag);
int		DISK_GetBusyState(DISK_DEVICE disk);

extern unsigned char *DISK_GetDeviceName(DISK_DEVICE disk);
int		DISK_SetDeviceName(DISK_DEVICE disk, unsigned char* ucName);

int DISK_GetDeviceUsingFS(DISK_DEVICE disk);

extern void DISK_Set_ForceStop(unsigned int disk, int FSProperty);
extern void DISK_Release_ForceStop(unsigned int disk, int FSProperty);
extern int DISK_Get_ForceStopStatus(unsigned int disk);

/*******************************************************************************
 * DISK List Array pre-definition ( in disk.c)
 ******************************************************************************/
extern tDeviceDriver DiskList[9];
#ifndef	FWDN_DOWNLOADER_INCLUDE
extern const unsigned int DISK_DefaultDriveType;
#else	//FWDN_DOWNLOADER_INCLUDE
extern unsigned int DISK_DefaultDriveType;
#endif 	//FWDN_DOWNLOADER_INCLUDE

#endif	// __DISK_H__

