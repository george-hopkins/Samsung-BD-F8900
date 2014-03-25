/****************************************************************************
 *   FileName    : TC_File.h
 *   Description : File System Abstraction Layer
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef	__TC_File_H__
#define	__TC_File_H__

#define K_FILE_INCLUDE

#ifndef K_FILE_INCLUDE
#ifndef NU_FILE_INCLUDE
#define	NU_FILE_INCLUDE
#endif
#endif

#ifdef	NU_FILE_INCLUDE
#include "pcdisk.h"
#else
#include "file.h"
#endif
#include "disk.h"

typedef	int	TC_STAT;
typedef	long long			TC_U64;
typedef	unsigned int		TC_U32;
typedef	unsigned long		TC_UL32;
typedef	unsigned short	TC_U16;
typedef	unsigned char		TC_U8;
typedef	int				TC_S32;
typedef	long				TC_SL32;
typedef	short			TC_S16;
typedef	signed char		TC_S8;
typedef	char *			TC_STR8;
typedef struct
{
	unsigned char	second;
	unsigned char	minute;
	unsigned char	hour;
	unsigned char	day;
	unsigned char	date;
	unsigned char	month;
	unsigned short	year;
} TC_DATETIME;

#ifdef	NU_FILE_INCLUDE

#ifdef INTERNAL_HIDDEN_STORAGE_INCLUDE		/* [1304] */
#define	INTERNAL_STORAGE_START_NO			0
#define	INTERNAL_STORAGE_END_NO			2
#else
#define	INTERNAL_STORAGE_START_NO		0
#define	INTERNAL_STORAGE_END_NO		3
#endif
#define	INTERNAL_HIDDEN_STORAGE_START_NO	3
#define	INTERNAL_HIDDEN_STORAGE_END_NO	3
#define	SD_MMC_STORAGE_START_NO		4
#define	SD_MMC_STORAGE_END_NO		7
#define	UFD_STORAGE_START_NO		8
#define	UFD_STORAGE_END_NO			9
#define	MS_STORAGE_START_NO			10
#define	MS_STORAGE_END_NO			13

#define	INTERNAL_STORAGE_1st	0
#define	INTERNAL_STORAGE_2nd	1
#define	INTERNAL_STORAGE_3rd	2
#define	INTERNAL_STORAGE_4th	3
#define	SD_MMC_STORAGE_1st		4
#define	SD_MMC_STORAGE_2nd		5
#define	SD_MMC_STORAGE_3rd		6
#define	SD_MMC_STORAGE_4th		7
#define	UFD_STORAGE_1st			8
#define	UFD_STORAGE_2nd			9

#ifdef	NU_FILE_INCLUDE
extern DISK_DEVICE	gDrive2Device[];		/* [1304] */
#endif

typedef	char			TC_DRIVE[4];
typedef	DROBJ		TC_DIR;
typedef	DROBJ *		TC_PDIR;
typedef	int			TC_DS;
typedef	DSTAT		TC_DENTRY;

#define	TC_ISDIRSP(X)		((X)->sfname[0] == '.')

#define	TC_StartDrive			"A:\\"
#define	TC_DriveNum			uNDRIVES

#define	TC_INTERNAL_DRIVE(X)	(INTERNAL_STORAGE_START_NO+(X))
#define	TC_MMC_DRIVE(X)		(SD_MMC_STORAGE_START_NO+(X))
#define	TC_UFD_DRIVE(X)		(UFD_STORAGE_START_NO+(X))
#define	TC_MS_DRIVE(X)			(MS_STORAGE_START_NO+(X))
#define	TC_ISINTDRIVE(X)		(((unsigned)(X)) <= INTERNAL_STORAGE_END_NO)
#define	TC_ISTypeFound(X, Y)	(pc_bdevsw[X].lock_no  == (Y))

#define	TC_UFD_DEVICE		UFD_IO_LOCK
#define	TC_HDD_DEVICE		HDD_IO_LOCK
#define	TC_NAND_DEVICE		NAND_IO_LOCK
#define	TC_TRIFLASH_DEVICE	SDMMC_IO_LOCK
#define	TC_SDMMC_DEVICE	SDMMC_IO_LOCK

#define	TC_Set_Drive(X, Y)	{(X)[0] = 'A'+(Y); (X)[1] = ':'; (X)[2] = '\\'; (X)[3] = 0; }
#define	TC_Inc_Drive(X, Y)	((X)[0] += (Y))
#define	TC_Get_DriveNo(X)	((X)[0] - 'A')

#define	TC_Get_Attribute(X)	((X)->fattribute)
#define	TC_Get_Cluster(X, Y)	(((Y)->fclusterhigh << 16) + ((Y)->fclusterlow))
#define	TC_Get_Size(X, Y)		((Y)->fsize)
#define	TC_Get_LFN(X)		((X)->lfname)
#define	TC_Get_SFName(X)	((X)->sfname)
#define	TC_Get_SFext(X)		((X)->fext)

#define	TC_O_USECACHE	PO_USECACHE        /* This Flag is only available at TC_O_RDONLY flag.
Using this flag, save the file cluster to memory after compressing.
in oder to increase the speed of accessing the file	*/
#define	TC_O_RDONLY	PO_RDONLY

#define	TC_O_WRONLY	PO_WRONLY
#define	TC_O_RDWR	PO_RDWR
#define	TC_O_APPEND	PO_APPEND
#define	TC_O_CREAT	PO_CREAT
#define	TC_O_FASTCREAT	PO_FASTCREAT
#define	TC_O_TRUNC	PO_TRUNC
#define	TC_O_EXCL	PO_EXCL
#define	TC_O_ENDIR	PO_ENDIR

#define	TC_A_READ	PS_IREAD
#define	TC_A_WRITE	PS_IWRITE

#define	TC_A_RDONLY	ARDONLY
#define	TC_A_HIDDEN	AHIDDEN
#define	TC_A_SYSTEM	ASYSTEM
#define	TC_A_VOLUME	AVOLUME
#define	TC_A_DIRENT	ADIRENT
#define	TC_A_ARCHIVE ARCHIVE
#define	TC_A_NORMAL	ANORMAL

#define	TC_MAX_PATH	EMAXPATH
#define	TC_MAX_DIR	2
#else
#define	TC_O_RDONLY	0x0000
#define	TC_O_WRONLY	0x0001
#define	TC_O_RDWR		0x0002
#define	TC_O_APPEND	0x0008
#define	TC_O_CREAT		0x0100
#define	TC_O_TRUNC		0x0200

/* File creation permissions for open */
/* Note: OCTAL */
#define	TC_A_READ		0x0400	/* Write permitted      */
#define	TC_A_WRITE		0x0200 	/* Read permitted. (Always true anyway)*/
//	#define 	TC_SUCCESS		0
typedef	TC_S32		TC_DRIVE;
typedef	int			TC_DIR;
typedef	int			TC_PDIR;
typedef	FDIRENTstruc TC_DS;
typedef	DIRENTstruc	TC_DENTRY;

/* #define	TC_ISDIRSP(X)		(0) */

#define	TC_StartDrive		1/* DISK_DEVICE_INTERNAL */
#define	TC_DriveNum			uNDRIVES

#define	TC_UFD_DEVICE			DISK_DEVICE_UHP
#define	TC_HDD_DEVICE			DISK_DEVICE_HDD
#define	TC_NAND_DEVICE		DISK_DEVICE_NAND
#define	TC_TRIFLASH_DEVICE	DISK_DEVICE_TRIFLASH
#define	TC_SDMMC_DEVICE		DISK_DEVICE_MMC
#if 1
TC_STAT TC_ISINTDRIVE(TC_U32 uDriveNo);
#else
#define	TC_ISINTDRIVE(X)		((X) == TC_StartDrive)
#endif

#if 1
#define	TC_Set_Drive(X, Y)	do { (X) = (Y); }while(0)
#define	TC_Inc_Drive(X, Y)	do { (X) += (Y); }while(0)
#define	TC_Get_DriveNo(X)	do { (X) = (X); }while(0)
#else
#define	TC_Set_Drive(X, Y)	((X) = (Y))
#define	TC_Inc_Drive(X, Y)	((X) += (Y))
#define	TC_Get_DriveNo(X)	((X))
#endif

#if 1
TC_U16 TC_Get_Attribute(const TC_DENTRY *pDENTRY);
TC_UL32 TC_Get_Cluster(const TC_DENTRY *pDENTRY, TC_U32 Y);
TC_SL32 TC_Get_Size(const TC_DS *pDS, TC_U32 Y);
TC_STR8 TC_Get_LFN(const TC_DS *pDS);
TC_STR8 TC_Get_SFName(const TC_DS *pDS);
TC_STR8 TC_Get_SFext(const TC_DS *pDS);
#else
#define	TC_Get_Attribute(X)	((X)->type)
#define	TC_Get_Cluster(X, Y)	((X)->startCluster)
#define	TC_Get_Size(X, Y)	((X)->fileSize)
#define	TC_Get_LFN(X)		((X)->lFileName)
#define	TC_Get_SFName(X)	((X)->sFileName)
#define	TC_Get_SFext(X)		((X)->sFileName+9)
#endif

#define	TC_A_RDONLY	FILE_READ_ONLY
#define	TC_A_HIDDEN	FILE_HIDDEN
#define	TC_A_SYSTEM	FILE_SYSTEM
#define	TC_A_VOLUME	FILE_VOLUME_ID
#define	TC_A_DIRENT	FILE_DIRECTORY
#define	TC_A_ARCHIVE	FILE_ARCHIVE
#define	TC_A_NORMAL	FILE_NORMAL

#define	TC_MAX_PATH	255
#define	TC_MAX_DIR		MAX_DIR
#endif

#if 1
TC_U32 TC_ISERR(TC_S32 X);
TC_U32 TC_ISOK(TC_S32 X);
TC_U32 TC_ISHERR(TC_S32 X);
TC_U32 TC_ISHOK(TC_S32 X);
TC_U32 TC_ISRWERR(TC_S32 X);
#else
#define	TC_ISERR(X)			((signed)(X) < 0)
#define	TC_ISOK(X)			((signed)(X) >= 0)

#define	TC_ISHERR(X)		((signed)(X) < 0)
#define	TC_ISHOK(X)			((signed)(X) >= 0)

#define	TC_ISRWERR(X)		((signed)(X) <= 0)
#endif

#define	TC_SEEK_SET		0
#define	TC_SEEK_CUR	1
#define	TC_SEEK_END	2

#define	TC_LOWLEVEL_YES	1
#define	TC_LOWLEVEL_NO		0

#define UNDEFINED_HANDLE    (-1)

TC_STAT TC_Initialize_FS(void);
TC_STAT TC_ClearFD(void);

TC_STAT TC_Open(TC_STR8 pName, TC_U32 uFlag, TC_U32 uMode, TC_U16 uAttr, TC_U32 uDirNum);
TC_STAT TC_OpenUnicode(short *pName, TC_U32 uFlag, TC_U32 uMode, TC_U16 uAttr, TC_U32 uDirNum);
TC_STAT TC_OpenIndex(TC_U32 uIndex, TC_U32 uFlag, TC_U32 uMode, TC_U32 uDirNum);
TC_STAT TC_OpenWithCluster(TC_S32 part_id, TC_U32 start_cluster, TC_S32 size);
TC_STAT TC_CreateAPPDB(unsigned char* pName, TC_S32 size, TC_U32 uDirNum);
TC_STAT TC_Rename(TC_STR8 pCurName, TC_STR8 pNewName, TC_U32 uDirNum);
TC_STAT TC_RenameUnicode(short *pCurName, short *pNewName, TC_U32 uDirNum);
TC_STAT TC_Delete(TC_STR8 pName, TC_U32 uDirNum);
TC_STAT TC_DeleteUnicode(short *pName, TC_U32 uDirNum);
TC_STAT TC_DeleteIndex(TC_U32 uIndex, TC_U32 uDirNum);
TC_STAT TC_DeleteHandle(TC_S32 iHandle);
TC_STAT TC_Copy(TC_S32 dir_num, TC_S32 sHandle, long *copy_status);
TC_STAT TC_CopyIndex(TC_S32 dir_num, TC_S32 sHandle, long *copy_status, TC_U32 *index);
void TC_StopCopy(void);
TC_S32 TC_Read(TC_S32 iHandle, void *pBuff, TC_S32 iCount);
TC_S32 TC_Write(TC_S32 iHandle, void *pBuff, TC_S32 iCount);
TC_S32 TC_Seek(TC_S32 iHandle, TC_S32 iOffset, TC_S32 iOrigin);
TC_S32 TC_Tell(TC_S32 iHandle);
TC_S32 TC_Length(TC_S32 iHandle);
TC_STAT TC_Flush(TC_S32 iHandle);
TC_STAT TC_Flush_Sync(TC_S32 src_fd, TC_S32 dst_fd);
TC_STAT TC_Truncate(TC_S32 iHandle, TC_S32 iOffset);
TC_STAT TC_Close(TC_S32 iHandle);
TC_STAT TC_GetLBA(int nHandle, long offset, unsigned long *lba_addr, long *offset_in_sector);
TC_U32 TC_Get_StartCluster(TC_S32 iHandle);
TC_U32 TC_Get_StartSector(TC_S32 iHandle);
TC_U32 TC_GetFileHandle(TC_S32 dir_num, TC_DS *psFD);
TC_U32 TC_Get_FileIndex(TC_S32 iHandle, TC_U32 uDirNum);
TC_U32 TC_Get_Index(TC_DS *pDStruct);
TC_S32 TC_GetName(unsigned long index, int dir_num, int mode, TC_DS * pFDir, unsigned char * Lfn);
TC_STR8 TC_GetNameWithHandle(int handle, int mode, unsigned char * Lfn);
TC_DS *TC_pGetFD(TC_S32 nFD);
#if 1 || defined(BLUETOOTH_INCLUDE)
TC_STAT TC_EndOfFile(TC_U32 nHandle);
#endif

TC_STAT TC_Set_Default_Drive(TC_S32 iDriveNo);
TC_U32 TC_Get_Default_Drive(void);
TC_STAT TC_Set_Current_Cluster(TC_S32 iPartID, TC_U32 uCluster, TC_U32 uDirNum);
TC_STAT TC_Set_Current_Cluster_OnlySearch(TC_S32 iPartID, TC_U32 uCluster, TC_U32 uDirNum);
TC_STAT TC_Set_Current_Dir(TC_S32 iPartID, TC_STR8 pPath, TC_U32 uDirNum);
TC_STAT TC_Set_Current_DirUnicode(TC_S32 iPartID, short *pPath, TC_U32 uDirNum);
TC_STAT TC_Set_Current_DirIndex(TC_S32 iPartID, TC_S32 dir_id, TC_S32 uDirNum);
TC_STAT TC_Get_FirstUnicode(TC_DS *pDS, TC_DENTRY *pDENTRY, TC_U32 uDirNum);
TC_STAT TC_Get_NextUnicode(TC_DS *pDS, TC_DENTRY *pDENTRY, TC_U32 uDirNum);
#if 1 || defined(BLUETOOTH_INCLUDE)
TC_U32 TC_Get_SFNpack(TC_STR8 pSFName, const TC_DENTRY *pDENTRY);
#endif
void TC_SetDBMakeFlag(TC_S32 flag);
TC_STAT TC_RenameDir(TC_STR8 pCurName, TC_STR8 pNewName, TC_U32 uDirNum);
TC_STAT TC_RenameDirUnicode(short *pCurName, short *pNewName, TC_U32 uDirNum);
TC_STAT TC_Remove_Dir(TC_STR8 pName, TC_U32 uOption);
TC_STAT TC_Remove_DirUnicode(short *pName, TC_U32 uOption);
TC_STAT TC_Remove_DirIndex(TC_U32 uIndex, TC_U32 uOption);
TC_STAT TC_Make_Dir(TC_STR8 pName, TC_U32 uDirNum);
TC_STAT TC_Make_DirUnicode(short *pName, TC_U32 uDirNum);
TC_STAT TC_Make_HiddenDir(TC_STR8 pName, TC_U32 uDirNum);
TC_STAT TC_Refresh(TC_S32 dir_num);
TC_STAT TC_Change2ParentDir(TC_S32 dir_num);
TC_STAT TC_CheckRootDir(TC_S32 dir_num);
TC_DS *TC_pGetFDir(TC_S32 nFDir);

#if 1 || defined(BLUETOOTH_INCLUDE)
TC_U32	TC_Get_DirInfo(TC_DS *pDStruct, TC_DENTRY *pDENTRY, TC_STR8 pName, TC_U32 uDirNum);
#endif

TC_STAT TC_Mount_All_Device(TC_U32 uDeviceType, TC_S32 lun, TC_S32 option);
TC_STAT TC_Mount_Device(TC_U32 uDeviceType, TC_S32 lun, TC_S32 option);
TC_STAT TC_Mount_All_Drive(void);
TC_STAT TC_Mount_Recover_Cluster(int part_id, unsigned long cluster);
void TC_StopMount(int set);
TC_STAT TC_CalcClusterComplete(int part_id);
TC_U32 TC_GetUsedClusters(int part_id);
/*#ifdef SKIP_FREE_CLUSTER
void TC_Mount_Reserve(void);
void TC_Mount_Resume(void);
void TC_Mount_Setting(void);
#endif*/
TC_STAT TC_Unmount_All_Device(TC_U32 uDeviceType);
TC_STAT TC_Unmount_Partition(TC_S32 part_id);
TC_STAT TC_Unmount_All_Drive(void);
TC_S32 TC_GetMountedDrive(void);
TC_STAT TC_Format(TC_U32 uDriveNo, TC_U32 uMode);
TC_STAT TC_DiskInfo(TC_S32 part_id, DRVINFOstruc *pDrive);
TC_UL32 TC_GetFSInfo(TC_S32 part_id);
TC_STAT TC_GetValidPartNum(void);
TC_U8 TC_GetFatType(TC_S32 part_id);
TC_U32 TC_TotalDiskCluster(TC_S32 part_id);
TC_U32 TC_AvailableDiskCluster(TC_S32 part_id);
TC_U32 TC_GetClusterSize(TC_S32 part_id);
TC_U32 TC_GetSectorSize(TC_S32 part_id);
void TC_UpdateDate(TC_U16 year, TC_U8 month, TC_U8 day);
void TC_UpdateTime(TC_U8 hours, TC_U8 min, TC_U8 sec);
TC_STAT TC_GetVolumeID_BR(TC_S32 part_id, TC_STR8 volumeID);
TC_STAT TC_GetVolumeID_Root(TC_U32 part_id, TC_STR8 volumeID);
TC_U8 *TC_SetVolumeLabel(void* VolLab);
TC_STAT TC_MountDev(unsigned char drvType, unsigned int lun);
TC_STAT TC_MountDriveIndex(unsigned char drvType, unsigned int lun, unsigned int part_index, int option);
TC_STAT TC_FormatIndex(unsigned char drvType, unsigned int lun, unsigned int part_index, unsigned char writeVolume);
TC_UL32 TC_GetFreeDiskSector(unsigned char drvType, unsigned int lun);
TC_STAT TC_CreatePartition(unsigned char drvType, unsigned int lun, unsigned long sec_num);
TC_STAT TC_CleanPartition(unsigned char drvType, unsigned int lun);

TC_U32 TC_EnterLOCK(TC_U32 uDeviceType);
TC_STAT TC_ExitLOCK(TC_U32 uDriveNo);
#endif
/* end of file */
