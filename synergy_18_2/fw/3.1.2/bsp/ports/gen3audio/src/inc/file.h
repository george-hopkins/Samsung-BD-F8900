/*****************************************************************************/
// File Defition for TCC72x
//
//	Copyright	2003				Telechips, Inc.
//
// 2004. 02. 24. Updated			by doduri
/*****************************************************************************/

#ifndef __FILE_H__
#define __FILE_H__

//#define FS_TCC760x
//#define FS_TCC860x
//#define FS_TCC830x
#define FS_TCC800x
//#define FS_TCC801x

#if defined(FS_TCC860x) || defined(FS_TCC830x) || defined(FS_TCC800x)
#include "TC_Kernel.h"
#else
#include "TC_Kernel.h"
#endif

#if defined(FS_TCC860x) || defined(FS_TCC830x) || defined(FS_TCC800x) || defined(FS_TCC801x)
#define DDI_INTERFACE
#define NEW_INDEXING_METHOD
//#define FS_MALLOC_USED
#endif

#define ROOT_DIR_ID		0xFFFF0000U
/*****************************************************************************/
// MOUNT MODE
/*****************************************************************************/
#define MOUNT_NORMAL			0
#define MOUNT_SKIP_CLUSTER		1
//#define MOUNT_BACK_CLUSTER	2

/*****************************************************************************/
// FILE ACCESS MODE
/*****************************************************************************/
//#define FILE_OEXIST		0x00010000U		/* Open Existing File */
#define FILE_OTRUNC		0x00020000U		/* Truncate Existing File */
#define FILE_OAPPEND	0x00040000U		/* Open Exisging File at EOF */
#define FILE_OCREAT		0x00100000U		/* Create Unexisting File */
#define FILE_ORDONLY	0x00200000U
#define FILE_OWRONLY	0x00400000U
#define FILE_ORDWR		FILE_ORDONLY|FILE_OWRONLY

/*****************************************************************************/
// FILE ATTRIBUTE
/*****************************************************************************/
#define FILE_NORMAL		0x00
#define FILE_READ_ONLY	0x01
#define FILE_HIDDEN		0x02
#define FILE_SYSTEM		0x04
#define FILE_VOLUME_ID	0x08
#define FILE_DIRECTORY	0x10
#define FILE_ARCHIVE	0x20

#define FILE_READ	0x00
#define FILE_WRITE	0x01

/*****************************************************************************/
// FILE CREATE OPTION
/*****************************************************************************/
#define FILE_NO_TILDE	0x0100		/* do not make file with numeric-tail */
#define FILE_NO_CMP		0x0200

/*****************************************************************************/
// FILE HANDLER STRUCTURE.
/*****************************************************************************/
typedef struct _HANDLER
{
	int lock;	// used handle
	int used;
//	unsigned long usage; // Usage of this handle. Read, Write, Copy etc...
//	unsigned char *pEntry; // entry chunk pointer
//	short nEntry; // number of entry in entry chunk data. 32 [byte]
} HANDLERstruc;

/*****************************************************************************/
// FILE DESCRIPTOR STRUCTURE.
/*****************************************************************************/

// REFERENCE POINT OF FILE POINTER.
#define	FSEEK_SET	0	// file beginning
#define	FSEEK_CUR	1	// current FP position
#define	FSEEK_END	2	// file ending

/*****************************************************************************/
// SHORT DIRECTORY ENTRY STRUCTURE(32 BYTES DATA).
/*****************************************************************************/
//gcc
//typedef struct __attribute__((packed)) _SFN_DIR_ENTRY		// FAT 32 bytes for directory entry
typedef __packed struct _SFN_DIR_ENTRY		// FAT 32 bytes for directory entry
{
	unsigned char  Name[8];
	unsigned char  Extension[3];
	unsigned char  Attr;
	unsigned char  NTRes;
	unsigned char  CrtTimeTenth;
	unsigned short CrtTime;
	unsigned short CrtDate;
	unsigned short LstAccDate;
	unsigned short FstClusHI;
	unsigned short WrtTime;
	unsigned short WrtDate;
	unsigned short FstClusLO;
	unsigned long  FileSize;
} SFN_INFO;

/*****************************************************************************/
// FILE OR DIRECTORY ENTRY STRUCTURE.
/*****************************************************************************/
/*
typedef struct _FD
{
	int part_id; // current partition index
	int file_id; // file id in a parent directory. NOT used anymore.
	unsigned long dirCluster; // number of the start cluster of a parent directory

	char lFileName[258];	// long file or dir name
	char sFileName[13];	// short file or dir name
	long fileSize;		// file size [byte].
	unsigned long startCluster;	// number of the start cluster of a file or directory

 	int pre_clusterCnt;
	 unsigned long cntCluster;

	int entryOffset; // short entry offset(slot) in current sector
	int entrySectorOffset; // current sector number in current directory
	unsigned long entryCluster; // just debugging

	//unsigned long clusterSize;	// cluster size of the current partition [byte]
	unsigned short type; // file type or attribute. (READ ONLY, HIDDEN, SYSTEM, DIRECTORY, ARCHIVE)
	unsigned long mode; // Access Mode of file. (READ, WRITE, REWRITE)
	unsigned long offset; // file pointer [byte]
	int refCnt; // referenct flag. lower 16 bit : referenct count, upper 16 bit : dirty bit
	SFN_INFO entry_node;
}FDstruc;

typedef struct _FDIRENT
{
	char lFileName[258];	// long file name
	char sFileName[13];	// short file name
	unsigned char ref;
	long fileSize;			// file size [byte]
	unsigned short type;	// file attribute.

	unsigned long startCluster; // cluster of the current file or directory
	unsigned long parentCluster;	// cluster of the parent directory

	int entryOffset; // short entry offset(slot) in current sector
	int entrySectorOffset; // current sector number in current directory
	unsigned long entryCluster; // just debugging

	int file_cnt;	// file count. only valid when FILE_Refresh() was excuted.
	int subdir_cnt; // sub-directory(folder) count. only valid when FILE_Refresh() was excuted.
	int part_id; // current partition index
	short entryCount; // total entry count
	SFN_INFO entry_node;
}FDIRENTstruc;
*/
#define FILE_LFN_LEN	(255 * 2)
#define FILE_SFN_LEN	12
typedef struct _FDIRENT
{
	int part_id;					//current partition index
	unsigned long startCluster;		//cluster of the current file or directory
	unsigned long parentCluster;	//cluster of the parent directory

	int entryOffset; 				//short entry offset(slot) in current sector
	int entrySectorOffset; 		//current sector number in current directory
	unsigned long entryCluster; 	//current cluster number
	unsigned long pre_entryCluster;

	unsigned long pre_clusterCnt;
	unsigned long curCluster;

	int file_cnt;					// file count. only valid when FILE_Refresh() was excuted.
	int subdir_cnt; 				// sub-directory(folder) count. only valid when FILE_Refresh() was excuted.
	long fileSize;		// file size [byte].

	unsigned short type; // file type or attribute. (READ ONLY, HIDDEN, SYSTEM, DIRECTORY, ARCHIVE)
	unsigned long mode;
	unsigned long offset;

	SFN_INFO entry_node;
	short entryCount; 			// total entry count
	int refCnt; 					// referenct flag. lower 16 bit : referenct count, upper 16 bit : dirty bit
	unsigned char ref;

	unsigned char *pEntry; // entry chunk pointer
	short nEntry; // number of entry in entry chunk data. 32 [byte]

	char lFileName[FILE_LFN_LEN + 2];			// long file name
	char sFileName[FILE_SFN_LEN + 1];			// short file name
} FDIRENTstruc;

/*****************************************************************************/
// FILE OR DIRECTORY ENTRY STRUCTURE. - for FILE_ReadDir()
/*****************************************************************************/
typedef struct _DIRENT
{
	SFN_INFO sEntry;	// short entry slot
	char *lFileName;	// pointer of the long file name
	char *sFileName;	// pointer of the short file name
	unsigned char LFNflag; // 0: no LFN, 1: has LFN
	unsigned short type;	// file attribute. file or directory
	unsigned long startCluster;	 // cluster of the directory or file
	unsigned int	offset;
	int part_id; // current partition index
} DIRENTstruc;

typedef struct _MFILE_INFO
{
	unsigned long pre_cluster;
	unsigned long curr_cluster;
	unsigned long end_cluster;		// write end cluster
	//unsigned long endnext_cluster;	// next content cluster of end cluster
	//int mode;					// FILE_READ, FILE_WRITE
} MFILEstruc;

#define FILE_FIND_FIRST				0x00
#define FILE_FIND_NEXT				0x01
#define FILE_SKIP_LFN				0x02
#define FILE_SAVE_S_ENTRY			0x04

/*****************************************************************************/
// DISK INFORMATION STRUCTURE. - for File_Format()
/*****************************************************************************/
//gcc
//typedef struct __attribute__((packed)) _DISK_INFO
typedef __packed struct _DISK_INFO
{
	unsigned long head;
	unsigned long cylinder;
	unsigned long sector;
	unsigned long sector_size;
} DISKINFOstruc;

/*****************************************************************************/
// PARTITION (OR DRIVE) SIMPLE INFORMATION
/*****************************************************************************/
//gcc
//typedef struct __attribute__((packed)) _DRV_INFO
typedef __packed struct _DRV_INFO
{
	unsigned char Drv_Type;			// HDD_DRV, MMC_DRV, USB_DRV, NAND_DRV
	unsigned char FAT_Type;			// FAT32, FAT16, FAT32
	unsigned short Part_id;			// logical partition or drive index(number).
	unsigned short BytsPerSec;			// Count of bytes per sector. normally 512
	unsigned long	ClusterSize;			// Cluster Byte Size
	unsigned long	CountofClusters;		// total number of clusters
	unsigned long	UsedClusters;		// Used total data capacity [byte]

	unsigned long logical_first_sector;	// first sector relative to start of disk
	unsigned long HiddSec;			// count of hidden sectors
	unsigned short ReserveSec;		// Number of reserved sectors
	unsigned long RootDirSector;		// Root Start Sector
	unsigned long DataSector;			// Cluster Start Sector	2TH CLUSTER
} DRVINFOstruc;

/*****************************************************************************/
// FUNCTIONS IN FILE.C
/*****************************************************************************/
#if defined(FS_TCC860x) || defined(FS_TCC830x) || defined(FS_TCC800x) || defined(FS_TCC801x)
#define FS_SchedLock()	TC_SchedLock()
#define FS_SchedUnlock()	TC_SchedUnlock()
#define FS_TimeDly(a)	do { TC_TimeDly(a); }while(0)
#else
#define FS_SchedLock()	OSSchedLock()
#define FS_SchedUnlock()	OSSchedUnlock()
#define FS_TimeDly(a)	OSTimeDly(a)
#endif


extern void FILE_InitSystem(void);
extern void FILE_InitDriveInfo(void);
extern void FILE_InitVariable(void);
extern void FILE_InitFDIRstruc(FDIRENTstruc *pFDir);

extern FDIRENTstruc *FILE_pGetFD(int nFD);
extern FDIRENTstruc *FILE_pGetFDir(int nFDir);
extern int FILE_Open(int file_id, char *mode, int dir_num);
extern int FILE_OpenName(char *pName, char *mode, unsigned char attr, int dir_num);
extern int FILE_OpenNameLFN(short *pName, char *mode, unsigned char attr, int dir_num);
extern int FILE_OpenWithCluster(int part_id, unsigned long start_cluster, int size);
extern int FILE_Rename(int handle, char *pName);
//extern int FILE_RenameLFN(int nHandle, short *pName, int dir_num);
extern int FILE_RenameLFN(int handle, short *pName);
extern int FILE_Remove(int file_id, int dir_num);
extern int FILE_RemoveWithHandle(int nHandle);
extern int FILE_Copy(int dir_num, int sHandle, long *copy_status);
extern void FILE_StopCopy(void);
extern int FILE_Read(int nHandle, void *pBuff, int size);
extern int FILE_Write(int nHandle, void *pBuff, int size);
extern int FILE_WriteAPPDB(int nHandle, int size);
extern int FILE_GetLBA(int nHandle, long offset, unsigned long *lba_addr, long *offset_in_sector);
extern int FILE_Seek(int nHandle, long offset, int whence);
extern long FILE_Tell(int nHandle);
extern int FILE_Length(int nHandle);
extern int FILE_Truncate(int nHandle, unsigned long len);
extern int FILE_Flush(int nHandle);
extern int FILE_Flush_Sync(int src_fd, int dst_fd);
extern int FILE_Close(int nHandle);
extern unsigned long FILE_GetStartCluster(int handle);
extern unsigned long FILE_GetStartSector(int handle);
extern int FILE_GetFileHandle(int dir_num, FDIRENTstruc *psFD);


extern int FILE_OpenDir(int part_id, int dir_id, int dir_num);
extern int FILE_OpenDirName(char *pName, int dir_num);
extern int FILE_OpenDirNameLFN(short *pName, int dir_num);
extern int FILE_OpenDirWithCluster(int part_id, unsigned long dir_cluster, int dir_num);
extern int FILE_OpenDirOnlySearch(int part_id, unsigned long dir_cluster, int dir_num);
extern int FILE_RenameDir(int dir_num, char *pName);
extern int FILE_RenameDirLFN(int dir_num, short *pName);
extern int FILE_RemoveDir(int dir_id, int dir_num);
extern int FILE_RemoveDirName(char *pName, int dir_num);
extern int FILE_RemoveDirNameLFN(short *pName, int dir_num);
extern int FILE_MakeDir(char *pName, int dir_num);
extern int FILE_MakeDirLFN(short *pName, int dir_num);
extern int FILE_MakeHideDir(char *pName, int dir_num);
extern int FILE_ReadDir(int mode, int dir_num, FDIRENTstruc *pFDir, DIRENTstruc *pEnt);
extern  int FILE_MoveDir(int nHandle, int sDir_num, int dDir_num);
extern int FILE_Refresh(int dir_num);
extern int FILE_Change2ParentDir(int dir_num);
extern int FILE_CheckRootDir(int dir_num);


#ifdef NEW_INDEXING_METHOD
extern int FILE_OpenWithIndex(unsigned long index, char *mode, int dir_num);
extern int FILE_OpenDirWithIndex(int part_id, unsigned long index, int dir_num);
extern int FILE_OpenDirOnlySearchWithIndex(int part_id, unsigned long index, int dir_num);
extern int FILE_RemoveWithIndex(unsigned long index, int dir_num);
extern int FILE_CopyIndex(int dir_num, int sHandle, long *copy_status, unsigned int *index);
//extern unsigned int FILE_GetIndex(FDIRENTstruc *FDir, int dir_num);
extern unsigned int FILE_GetIndex(FDIRENTstruc *FDir);
extern unsigned int FILE_GetIndexWithHandle(int dHandle);
extern int FILE_GetName(unsigned long index, int dir_num, int mode, FDIRENTstruc *pFDir, unsigned char *Lfn);
extern char *FILE_GetNameWithHandle(int handle, int mode, unsigned char *Lfn);
#endif


//extern int FILE_InitFS(void);
extern int FILE_MountDrive(unsigned char drvType, unsigned int lun, int option);
extern int FILE_MountDev(unsigned char drvType, unsigned int lun);
extern int FILE_MountDriveIndex(unsigned char drvType, unsigned int lun, unsigned int part_index, int option);
extern int FILE_RecoverCluster(int part_id, unsigned long start_cluster);
//extern void FILE_Mount_Resume(void);
//extern void FILE_Mount_Reserve(void);
//extern void FILE_Mount_Setting(void);
extern void FILE_StopMount(int set);
extern int FILE_CalcClusterComplete(int part_id);
//extern int FILE_UnmountDrive(unsigned char drvType);
extern int FILE_UnmountDevType(unsigned char drvType);
extern int FILE_UnmountPartID(int part_id);
extern int FILE_Format(unsigned char drvType, unsigned char writeVolume);
extern int FILE_FormatIndex(unsigned char drvType, unsigned int lun, unsigned int part_index, unsigned char writeVolume);
extern unsigned long FILE_GetFreeDiskSector(unsigned char drvType, unsigned int lun);
extern int FILE_CreatePartition(unsigned char drvType, unsigned int lun, unsigned long sec_num);
extern int FILE_CleanPartition(unsigned char drvType, unsigned int lun);
extern unsigned long FILE_TotalDiskCluster(int part_id);
extern unsigned long FILE_AvailableDiskCluster(int part_id);
extern unsigned long FILE_GetClusterSize(int part_id);
extern unsigned long FILE_GetSectorSize(int part_id);
extern unsigned char FILE_GetFatType(int part_id);
extern int FILE_DiskInfo(int part_id, DRVINFOstruc *pDrive);
extern unsigned long FILE_GetFSInfo(int part_id);
extern int FILE_GetValidPartNum(void);
extern void FILE_UpdateDate(unsigned short year, unsigned char month, unsigned char day);
extern void FILE_UpdateTime(unsigned char hours, unsigned char min, unsigned char sec);
extern unsigned char *FILE_SetVolumeLabel(void* VolLab);
extern unsigned char *FILE_GetVolumeLabel(void);
extern int FILE_GetVolumeID_BR(int part_id, unsigned char *volumeID);
extern int FILE_GetVolumeID_Root(int part_id, unsigned char *volumeID);
extern unsigned long FILE_GetUsedClusters(int part_id);
//extern void FILE_SetDBMakeFlag(int flag);

#endif
