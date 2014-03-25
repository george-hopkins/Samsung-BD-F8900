#ifndef CSR_TESTCASES_H__
#define CSR_TESTCASES_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int testMutexCreateDestroy(void);
int testMutexLockUnlock(void);
int testThreadGlobalMutex(void);
int testThreadMutexLock(void);
int testThreadSleep(void);
int testThreadCreateTooMany(void);
int testEventWait(void);
int testEventWaitTimeout(void);
int testThreadGetHandle(void);

typedef struct
{
    int tc_no;
    int (*tc)(void);
    char tc_name[255];
} CsrFwextTcDefs;

CsrFwextTcDefs CsrTsFwext[] =
{
    {0,
     testMutexCreateDestroy,
     "testMutexCreateDestroy"},
    {1,
     testMutexLockUnlock,
     "testMutexLockUnlock"},
    {2,
     testEventWait,
     "testEventWait"},
    {3,
     testEventWaitTimeout,
     "testEventWaitTimeout"},
    {4,
     testThreadMutexLock,
     "testThreadMutexLock"},
    {5,
     testThreadGlobalMutex,
     "testThreadGlobalMutex"},
    {6,
     testThreadSleep,
     "testThreadSleep"},
    {7,
     testThreadCreateTooMany,
     "testThreadCreateTooMany"},
    {8,
     testThreadGetHandle,
     "testThreadGetHandle"},


    {0xff, NULL, ""}
};


#ifdef __cplusplus
}
#endif

#endif /* CSR_TESTCASES_H__ */
