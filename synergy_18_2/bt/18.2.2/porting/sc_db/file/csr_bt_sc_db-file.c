/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>

#include "csr_file.h"
#include "csr_bt_sc_db.h"
#include "csr_bt_util.h"
#include "csr_bt_common.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_log_text.h"

#define LOG_TEXT_TASK_ID    ((CsrLogTextTaskId) CSR_BT_SC_IFACEQUEUE)
#define LOG_TEXT_SO_SCDB    0
#endif /* CSR_LOG_ENABLE */
//"/DiskA3/sc_db.db"
#define CSR_BT_SC_DB_FILE_NAME ((CsrUint8*)"sc_db.db")
#define CSR_BT_SC_DB_PERMS     (CSR_FILE_PERMS_USER_READ|CSR_FILE_PERMS_USER_WRITE)

static void *scDbFileHandle = NULL;

static CsrBool csrBtScDbFileCheckVersion(void)
{
    /* This test is only for version control of the SC_DB file and it
     * is IMPORTANT to note, that it is NOT necessary to implement
     * this feature */
    CsrUint32 version;
    CsrFileHandle *fileHandle;
    CsrResult res;
    CsrSize count;
    CsrFileOpenFlags flags;

    res = CsrFileOpen(&fileHandle,
                      CSR_BT_SC_DB_FILE_NAME,
                      CSR_FILE_OPEN_FLAGS_READ_ONLY,
                      CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);

    if(res == CSR_RESULT_SUCCESS)
    {
        version = 0;
        res = CsrFileRead(&version, CSR_BT_SC_DB_FILE_VERSION_SIZE, fileHandle, &count);
        CsrFileClose(fileHandle);

        if (res == CSR_RESULT_SUCCESS &&
            count == CSR_BT_SC_DB_FILE_VERSION_SIZE)
        {
            if (version == CSR_BT_SC_DB_FILE_VERSION)
            {
                /* Correct version */
                return TRUE;
            }
            else
            {
                CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Wrong version, 0x%08x", version));
            }
        }
        else
        {
            CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Read check fail"));
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Version check open fail"));
    }

    /* Open file, possibly truncating existing */
    flags = CSR_FILE_OPEN_FLAGS_WRITE_ONLY|CSR_FILE_OPEN_FLAGS_CREATE|CSR_FILE_OPEN_FLAGS_TRUNCATE;
    res = CsrFileOpen(&fileHandle,
                      CSR_BT_SC_DB_FILE_NAME,
                      flags,
                      CSR_BT_SC_DB_PERMS);

    /* Write version field */
    if(res == CSR_RESULT_SUCCESS)
    {
        count = 0;
        version = CSR_BT_SC_DB_FILE_VERSION;

        res = CsrFileWrite(&version, CSR_BT_SC_DB_FILE_VERSION_SIZE, fileHandle, &count);
        if(res != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Write version fail"));
        }
        CsrFileClose(fileHandle);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Create/truncate version fail"));
    }

    /* File error or version mismatch */
    return FALSE;
}

static CsrInt32 csrBtScDbFileGetPosition(CsrBtDeviceAddr *addr, CsrBool match)
{
    CsrBtScDbDeviceRecord dummyRecord;
    CsrFileHandle *fileHandle;
    CsrSize read;
    CsrResult res;
    CsrInt32 pos = -1;
    CsrInt32 i = 0;

    res = CsrFileOpen(&fileHandle,
                      CSR_BT_SC_DB_FILE_NAME,
                      CSR_FILE_OPEN_FLAGS_READ_ONLY,
                      CSR_BT_SC_DB_PERMS);

    /* Serious error, bail out */
    if(res != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Open %04X:%02X:%06X", addr->nap, addr->uap, addr->lap));
        CsrFileClose(fileHandle);
        return -1;
    }

    /* Look for an existing entry where we can write the entry: Scan
     * all records.  If the matching address is found, use that entry.
     * Otherwise, use the first 'empty' entry in the database */
    res = CsrFileSeek(fileHandle, CSR_BT_SC_DB_FILE_VERSION_SIZE, CSR_SEEK_SET);
    if(res != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Seek %04X:%02X:%06X", addr->nap, addr->uap, addr->lap));
        CsrFileClose(fileHandle);

        return -1;
    }

    do
    {
        CsrMemSet(&dummyRecord, 0, sizeof(CsrBtScDbDeviceRecord));
        res = CsrFileRead(&dummyRecord, sizeof(CsrBtScDbDeviceRecord), fileHandle, &read);

        if((res == CSR_RESULT_SUCCESS) &&
           (read == sizeof(CsrBtScDbDeviceRecord)))
        {
            if((dummyRecord.deviceAddr.lap == addr->lap) &&
               (dummyRecord.deviceAddr.nap == addr->nap) &&
               (dummyRecord.deviceAddr.uap == addr->uap))
            {
                /* Full match, stop */
                pos = i;
                CSR_LOG_TEXT_INFO((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Pos found %04X:%02X:%06X [pos: %d, match: %u] ",
                                   addr->nap, addr->uap, addr->lap, pos, match));
                break;
            }
            else if(!match && /* allow zero-match */
                    (pos == -1) && /* not found */
                    (dummyRecord.deviceAddr.lap == 0) && /* zero match */
                    (dummyRecord.deviceAddr.nap == 0) &&
                    (dummyRecord.deviceAddr.uap == 0))
            {
                /* First empty entry found. Use this position unless
                 * full match is found */
                pos = i;
            }
        }
        else
        {
            if(!match && (pos == -1))
            {
                /* End of file reached, and no 'empty position'
                 * found */
                pos = i;
            }
            CSR_LOG_TEXT_INFO((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Seek end %04X:%02X:%06X [pos: %d, match: %u] ",
                               addr->nap, addr->uap, addr->lap, pos, match));
            break;
        }

        i++;
    }
    while((res == CSR_RESULT_SUCCESS) &&
          (read == sizeof(CsrBtScDbDeviceRecord)));
    CsrFileClose(fileHandle);

    return pos;
}

CsrBool CsrBtScDbRead(CsrBtDeviceAddr *addr , CsrBtScDbDeviceRecord *theRecord)
{
    /* Should return TRUE and the information about the requested
     * device, if the device is not present in the data base return
     * FALSE */
    CsrFileHandle *fileHandle;
    CsrResult res;
    CsrSize read;

    if(!csrBtScDbFileCheckVersion())
    {
        /* Database empty */
        return FALSE;
    }

    res = CsrFileOpen(&fileHandle,
                      CSR_BT_SC_DB_FILE_NAME,
                      CSR_FILE_OPEN_FLAGS_READ_ONLY,
                      CSR_BT_SC_DB_PERMS);

    if(res == CSR_RESULT_SUCCESS)
    {
        /* Scan file: Read a property, check it, return true if
         * found. Continue until reading fails */
        res = CsrFileSeek(fileHandle, CSR_BT_SC_DB_FILE_VERSION_SIZE, CSR_SEEK_SET);
        if(res != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Seek %04X:%02X:%06X",
                                  addr->nap, addr->uap, addr->lap));
            CsrFileClose(fileHandle);
            return FALSE;
        }

        do
        {
            res = CsrFileRead(theRecord, sizeof(CsrBtScDbDeviceRecord), fileHandle, &read);
            if (res == CSR_RESULT_SUCCESS &&
                read == sizeof(CsrBtScDbDeviceRecord))
            {
                if (bd_addr_eq(&theRecord->deviceAddr, addr))
                {
                    CsrFileClose(fileHandle);
                    return TRUE ;
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Read %04X:%02X:%06X [res: %u, read: %u]",
                                      addr->nap, addr->uap, addr->lap, res, read));
            }
        }
        while((res == CSR_RESULT_SUCCESS) &&
              (read == sizeof(CsrBtScDbDeviceRecord)));
        CsrFileClose(fileHandle);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Open %04X:%02X:%06X",
                              addr->nap, addr->uap, addr->lap));
    }

    CSR_LOG_TEXT_INFO((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Read not found %04X:%02X:%06X",
                       addr->nap, addr->uap, addr->lap));
    return FALSE;
}

void CsrBtScDbWrite(CsrBtDeviceAddr *addr, CsrBtScDbDeviceRecord *theRecord)
{
    /* Should write information about a device into the data base, if
     * the device already exist, its entry should be updated */
    CsrFileHandle *fileHandle;
    CsrResult res;
    CsrSize read;
    CsrInt32 pos;
    CsrFileOpenFlags flags;

    /* Initialise file version */
    (void)csrBtScDbFileCheckVersion();

    pos = csrBtScDbFileGetPosition(addr, FALSE); /* may use empty slots */
    if(pos == -1)
    {
        /* Creating new database */
        flags = CSR_FILE_OPEN_FLAGS_WRITE_ONLY|CSR_FILE_OPEN_FLAGS_CREATE;
        pos = 0;
    }
    else
    {
        /* Existing database */
        flags = CSR_FILE_OPEN_FLAGS_WRITE_ONLY;
    }

    /* Open file for write */
    res = CsrFileOpen(&fileHandle,
                      CSR_BT_SC_DB_FILE_NAME,
                      flags,
                      CSR_BT_SC_DB_PERMS);

    if(res == CSR_RESULT_SUCCESS)
    {
        res = CsrFileSeek(fileHandle,
                    CSR_BT_SC_DB_FILE_VERSION_SIZE + pos * sizeof(CsrBtScDbDeviceRecord),
                    CSR_SEEK_SET);
        if(res != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Seek %04X:%02X:%06X",
                                  addr->nap, addr->uap, addr->lap));
        }

        res = CsrFileWrite(theRecord, sizeof(CsrBtScDbDeviceRecord), fileHandle, &read);
        if(res != CSR_RESULT_SUCCESS || read != sizeof(CsrBtScDbDeviceRecord))
        {
            CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Write %04X:%02X:%06X",
                                  addr->nap, addr->uap, addr->lap));
        }
        CsrFileClose(fileHandle);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Open %04X:%02X:%06X", addr->nap, addr->uap, addr->lap));
    }

}

void CsrBtScDbRemove(CsrBtDeviceAddr *addr)
{
    /* Should write information about a device into the data base, if
     * the device already exist, its entry should be updated */
    CsrFileHandle *fileHandle;
    CsrResult res;
    CsrSize read;
    CsrBtScDbDeviceRecord dummyRecord;
    CsrInt32 pos;

    if(!csrBtScDbFileCheckVersion())
    {
        /* Empty database */
        return;
    }

    pos = csrBtScDbFileGetPosition(addr, TRUE); /* must match */
    if(pos != -1)
    {
        /* Found. Rewind position to start of record and write an
         * empty entry */
        res = CsrFileOpen(&fileHandle,
                          CSR_BT_SC_DB_FILE_NAME,
                          CSR_FILE_OPEN_FLAGS_WRITE_ONLY,
                          CSR_BT_SC_DB_PERMS);
        if(res == CSR_RESULT_SUCCESS)
        {
            CsrMemSet(&dummyRecord, 0, sizeof(CsrBtScDbDeviceRecord));
            res = CsrFileSeek(fileHandle, CSR_BT_SC_DB_FILE_VERSION_SIZE + pos * sizeof(CsrBtScDbDeviceRecord), CSR_SEEK_SET);
            if(res != CSR_RESULT_SUCCESS)
            {
                CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Seek %04X:%02X:%06X",
                                      addr->nap, addr->uap, addr->lap));
            }

            res = CsrFileWrite(&dummyRecord, sizeof(CsrBtScDbDeviceRecord), fileHandle, &read);
            if(res != CSR_RESULT_SUCCESS || read != sizeof(CsrBtScDbDeviceRecord))
            {
                CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Write %04X:%02X:%06X",
                                      addr->nap, addr->uap, addr->lap));
            }

            CsrFileClose(fileHandle);
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Remove not found %04X:%02X:%06X",
                              addr->nap, addr->uap, addr->lap));
    }
}

CsrBool CsrBtScDbReadFirst(CsrBtScDbDeviceRecord *theRecord)
{
    CsrResult res;

    scDbFileHandle = NULL;
    if(!csrBtScDbFileCheckVersion())
    {
        return FALSE;
    }
    res = CsrFileOpen(&scDbFileHandle,
                      CSR_BT_SC_DB_FILE_NAME,
                      CSR_FILE_OPEN_FLAGS_READ_ONLY,
                      CSR_BT_SC_DB_PERMS);
    if(res != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Open %04X:%02X:%06X",
                              theRecord->deviceAddr.nap, theRecord->deviceAddr.uap, theRecord->deviceAddr.lap));
        return FALSE;
    }

    res = CsrFileSeek(scDbFileHandle, CSR_BT_SC_DB_FILE_VERSION_SIZE, CSR_SEEK_SET);
    if(res != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Seek %04X:%02X:%06X",
                              theRecord->deviceAddr.nap, theRecord->deviceAddr.uap, theRecord->deviceAddr.lap));
        return FALSE;
    }

    /* Read next entry, which will be the first... */
    return CsrBtScDbReadNext(theRecord);
}

CsrBool CsrBtScDbReadNext(CsrBtScDbDeviceRecord *theRecord)
{
    CsrResult res;
    CsrSize read;

    if(scDbFileHandle == NULL)
    {
        /* Not open */
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Next not found %04X:%02X:%06X",
                              theRecord->deviceAddr.nap, theRecord->deviceAddr.uap, theRecord->deviceAddr.lap));
        return FALSE;
    }

    /* Read first non-empty entry */
    do
    {
        res = CsrFileRead(theRecord, sizeof(CsrBtScDbDeviceRecord), scDbFileHandle, &read);
        if (res == CSR_RESULT_SUCCESS &&
            read == sizeof(CsrBtScDbDeviceRecord))
        {
            if (theRecord->deviceAddr.lap != 0 ||
                theRecord->deviceAddr.nap != 0 ||
                theRecord->deviceAddr.uap != 0)
            {
                return TRUE ;
            }
        }
        else
        {
            CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "Read %04X:%02X:%06X",
                                  theRecord->deviceAddr.nap, theRecord->deviceAddr.uap, theRecord->deviceAddr.lap));
        }
    }
    while((res == CSR_RESULT_SUCCESS) &&
          (read == sizeof(CsrBtScDbDeviceRecord)));
    CsrFileClose(scDbFileHandle);
    scDbFileHandle = NULL;
    return FALSE;
}
