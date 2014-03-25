#ifndef _TC_MBOX_H_
#define _TC_MBOX_H_

#if 0
#include "Kernel/telechips.h"
#endif

/*****************************************************************************/
/* Define MBOX management functions.  */
/*****************************************************************************/
enum
{
	MBOX_STATE_INIT = 0,
	MBOX_STATE_NORMAL,
	MBOX_STATE_EMPTY,
	MBOX_STATE_FULL,
	MBOX_STATE_MAX
};

enum
{
	MBOX_RESULT_NO_MORE_SIZE = -8,
	MBOX_RESULT_NO_MORE_MEMORY = -7,
	MBOX_RESULT_NO_CREATE_QUEUE = -6,
	MBOX_RESULT_NO_READ = -5,
	MBOX_RESULT_NO_WRITE = -4,
	MBOX_RESULT_ERROR_READ = -3,
	MBOX_RESULT_ERROR_WRITE = -2,
	MBOX_RESULT_ERROR = -1,
	MBOX_RESULT_OK = 0,
	MBOX_RESULT_MAX = 1
};

typedef struct _TC_MBOX
{
	unsigned int mState;
	unsigned int mRead;
	unsigned int mWrite;
	unsigned char *pBuf;
	unsigned int mBufSize;
	TC_QUEUE	mQueue;
	void *pQBuf;
} TC_MBOX;

typedef struct _TC_MBOX_QDATA
{
	unsigned int mSize;
	void 		*mAddr;
} TC_MBOX_QDATA;

extern TC_MBOX UI_MBOX;
extern TC_MBOX DEMON_MBOX;

extern int TC_MBOX_Create(TC_MBOX *pMBox, unsigned int mbox_size, char *queue_name, unsigned int queue_size);
extern int TC_MBOX_Delete(TC_MBOX *pMBox);
extern int TC_MBOX_Reset(TC_MBOX *pMBox);
extern void *TC_MBOX_Get_WriteAddress(TC_MBOX *pMBox, unsigned int size, int *result);
extern int TC_MBOX_Update_Write(TC_MBOX *pMBox, unsigned int size);
extern int TC_MBOX_Update_Read(TC_MBOX *pMBox, unsigned int address, unsigned int size);
extern int TC_MBOX_Send_To_Queue(const TC_MBOX *pMBox, void *message);
extern int TC_MBOX_Receive_From_Queue(const TC_MBOX *pMBOX, void *message, unsigned int suspend);
extern unsigned int TC_MBOX_Get_DataSize(TC_MBOX *pMBox);

#endif

/* End of File */

