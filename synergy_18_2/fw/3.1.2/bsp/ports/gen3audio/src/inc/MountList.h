
#ifndef _MOUNTLIST
#define _MOUNTLIST

enum{
#ifdef NAND_INCLUDE
	ML_NAND,
#endif
#ifdef UHP_INCLUDE
	ML_UFD0,
	ML_UFD1,
#endif
#ifdef MMC_INCLUDE
	ML_MMC0,
	ML_MMC1,
#endif
#ifdef IPOD_UART_INCLUDE	
	ML_IPOD_UART,
#endif
#ifdef CD_INCLUDE
	ML_CD,
#endif
#if !defined(NAND_INCLUDE) && !defined(UHP_INCLUDE)&& !defined(MMC_INCLUDE)&& !defined(IPOD_UART_INCLUDE)&& !defined(CD_INCLUDE)
	ML_NULL_DEVICE,
#endif
	ML_MAX_MOUNTLIST, 
	ML_NO_DEVICE
};

enum{
	ML_FREE,
	ML_BUSY
};

typedef enum{
	ML_DISABLE,
	ML_ENABLE
}ML_Detect;

typedef enum{
	ML_FRONT,
	ML_REAR
}ML_Pos;

typedef enum{
	ML_FOREGROUND,
	ML_BACKGROUND
}ML_Back;

typedef enum{
	ML_NOT_MOUNT,
	ML_REMOVE,
	ML_DETECT,
	//ML_READY,
	ML_MOUNTING,
	ML_DBMAKE,
	//ML_CANCEL,
	ML_SUCCESS,
	ML_ERR
}ML_Status;

typedef struct _MountList{
	int devType;
	int detect_enable;
	int status;
	int background;
	int position;	//physical
	int loc;		//logical
} MountListStruc;




extern void ML_SetHostType(int dev, int host_dev, int devNum);
extern int ML_GetHostType(int dev);
extern int ML_GetHostDevNum(int dev);
extern int ML_GetLastDevice(void);
extern void ML_SetLastDevice(int dev);
extern void ML_CheckLastDevice(void);
extern void ML_ReadyMountDevice(void);
extern int ML_GetInitDevice(void);
extern void ML_SetInitDevice(int set);
extern int ML_Detectable(int dev);
extern int ML_GetDetect(int dev);
extern void ML_SetDetect(int dev, int enable);
extern int ML_GetBackground(int dev);
extern void ML_SetBackground(int dev, int back);
extern int ML_GetStatus(int dev);
extern void ML_SetStatus(int dev, int status);
extern int ML_GetTaskStatus(void);
extern void ML_SetTaskStatus(int status);
extern int ML_GetPos(int dev);
extern int ML_GetLoc(int dev);
extern void ML_SetLoc(int dev, int loc);
extern void ML_CancelDevice(int dev);
extern int ML_GetCancel(void);
extern void ML_SetCancel(int dev, int cancel);
extern int ML_GetMountingDevice(void);
extern void ML_SetInsertDevice(int dev);
extern void ML_SetDetectDevice(int dev, int devNum);
extern void ML_SetErrorDevice(int dev);
extern void ML_SetRemoveDevice(int dev, int devNum);
//extern void ML_ClearLastDevice(void);
extern void ML_WaitLastDevice(void);
extern void ML_SetBackgroundALL(int dev, int loc);
extern int ML_PowerOnDevice(int dev, int loc);
extern int ML_PowerOffDevice(int dev, int loc);
extern void ML_InsertDevice(int dev);
extern void ML_RemoveDevice(int dev);
extern void ML_MountDevice(void);
extern void ML_MountProcess(void);

#endif
