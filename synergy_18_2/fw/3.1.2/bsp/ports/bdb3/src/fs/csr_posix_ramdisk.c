/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_macro.h"
#include "csr_file.h"
#include "csr_dir.h"
#include "csr_panic.h"
#include "csr_unicode.h"

#include "csr_formatted_io.h"

#include "nucleus_file.h"

#include "platform/csr_interrupt.h"

static CsrUint32 ramdiskBuffer[32 * 1024 * 1024 / sizeof(CsrUint32)];
static CsrBool ramdiskReady = FALSE;

static CsrCharString *convertFsalName(const CsrUtf8String *inFile)
{
    CsrCharString *outFile, *p;
    const CsrUtf8String *pp;
    CsrSize nLen;

    /* Encode chars as two bytes per byte */
    nLen = CsrStrLen((CsrCharString *) inFile);
    nLen = nLen * 2 + 1;

    outFile = CsrPmemAlloc(nLen);

    for (p = outFile, pp = inFile;
         *pp != '\0';
         p++, pp++)
    {
        if (*pp == CSR_DIR_PATH_SEP)
        {
            *p = '\\';
        }
        else if ((*pp == '*') || (*pp == '.'))
        {
            /* Preserve wildcards */
            *p = *pp;
        }
        else
        {
            char c1, c2;

            /* ``big endian'' */
            c1 = (*pp & 0xf0) >> 4;
            c2 = *pp & 0x0f;

            CsrSnprintf(p, 3, "%1x%1x", c1, c2);
            p++;
        }
    }
    *p = '\0'; /* Nul-terminate */

    return outFile;
}

static CsrUtf8String *convertNFName(const CsrCharString *inFile)
{
    CsrUtf8String *outFile, *p;
    const CsrCharString *pp;
    CsrSize outLen;

    outLen = CsrStrLen(inFile);
    outFile = CsrPmemAlloc(outLen); /* ``//'' is forbidden, so this is ok. */

    for (p = outFile, pp = inFile;
         *pp != '\0';
         p++, pp++)
    {
        if (*pp == '\\')
        {
            *p = CSR_DIR_PATH_SEP;
        }
        else if (*pp == '.')
        {
            *p = *pp;
        }
        else if (*pp == '*')
        {
            *p = '*';
        }
        else
        {
            CsrHexStrToUint8(pp, p);
            pp++;
        }
    }
    *p = '\0'; /* Nul-terminate */

    return outFile;
}

static CsrDirFindStat *convertDSTAT(DSTAT *statObj)
{
    CsrDirFindStat *fileStat;

    fileStat = CsrPmemAlloc(sizeof(*fileStat));

    fileStat->name = (CsrUtf8String *) convertNFName(statObj->lfname);
    fileStat->mode = CSR_DIR_MODE_USER_READ_PERMISSION;

    if ((statObj->fattribute & ARDONLY) == 0)
    {
        fileStat->mode |= CSR_DIR_MODE_USER_WRITE_PERMISSION;
    }

    if (statObj->fattribute & ADIRENT)
    {
        fileStat->mode |= CSR_DIR_MODE_DIRECTORY;
    }
    else
    {
        fileStat->mode |= CSR_DIR_MODE_REGULAR_FILE;
    }

    fileStat->size = statObj->fsize;

    CsrMemSet(&fileStat->time, 0, sizeof(fileStat->time));

    return fileStat;
}

static UINT16 returnMappedFlags(CsrFileOpenFlags mode)
{
    UINT16 ret = 0;

    ret |= (mode & CSR_FILE_OPEN_FLAGS_APPEND) ? PO_APPEND : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_READ_ONLY) ? PO_RDONLY : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_READ_WRITE) ? PO_RDWR : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_WRITE_ONLY) ? PO_WRONLY : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_CREATE) ? PO_CREAT : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_TRUNCATE) ? PO_TRUNC : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_EXCL) ? PO_EXCL : 0;

    return ret;
}

static UINT16 returnMappedPerms(CsrFilePerms mode)
{
    UINT16 ret = 0;

    /* note that read and write flags are the same for all user/group/others and execute flags are ignored on windows */
    ret |= (mode & CSR_FILE_PERMS_USER_READ) ? PS_IREAD : 0;
    ret |= (mode & CSR_FILE_PERMS_GROUP_READ) ? PS_IREAD : 0;
    ret |= (mode & CSR_FILE_PERMS_OTHERS_READ) ? PS_IREAD : 0;
    ret |= (mode & CSR_FILE_PERMS_GROUP_WRITE) ? PS_IWRITE : 0;
    ret |= (mode & CSR_FILE_PERMS_USER_WRITE) ? PS_IWRITE : 0;
    ret |= (mode & CSR_FILE_PERMS_OTHERS_WRITE) ? PS_IWRITE : 0;

    return ret;
}

static void csrRamDiskReadyEnsure(void)
{
    /* Opportunistic initialization. */
    if (ramdiskReady != TRUE)
    {
#if 0
        if (NU_Become_File_User() != YES)
        {
            *handle = NULL;

            return CSR_RESULT_FAILURE;
        }
#endif

        /* This is a horrid API. */
        ramdiskBuffer[0] = sizeof(ramdiskBuffer);
        file_init(ramdiskBuffer);

        ramdiskReady = TRUE;
    }
}

CsrResult CsrFileOpen(CsrFileHandle **handle, const CsrUtf8String *fileName,
    CsrFileOpenFlags flags, CsrFilePerms perms)
{
    CsrCharString *nName;
    UINT16 nFlags, nMode;
    INT res, *fd;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    nName = convertFsalName(fileName);
    nFlags = returnMappedFlags(flags);
    nMode = returnMappedPerms(perms);

    res = NU_Open(nName, nFlags, nMode);
    if (res >= 0)
    {
        fd = CsrPmemAlloc(sizeof(*fd));
        *fd = res;
        *handle = fd;

        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        *handle = NULL;

        switch (res)
        {
            case NUF_NOFILE:
                rval = CSR_FILE_RESULT_NOT_EXIST;
                break;

            case NUF_NO_BLOCK:
            case NUF_NO_DISK:
            case NUF_NO_DROBJ:
            case NUF_NO_FINODE:
            case NUF_NO_MEMORY:
            case NUF_NO_PARTITION:
                rval = CSR_FILE_RESULT_NO_SPACE;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    CsrPmemFree(nName);

    return rval;
}

CsrResult CsrFileClose(CsrFileHandle *handle)
{
    INT *fd;
    STATUS res;
    CsrResult rval;

    fd = (INT *) handle;

    res = NU_Close(*fd);

    CsrPmemFree(fd);

    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    return rval;
}

CsrResult CsrFileWrite(const void *buffer, CsrSize bytesToWrite,
    CsrFileHandle *handle, CsrSize *written)
{
    INT *fd;
    INT32 res;
    CsrResult rval;

    fd = (INT *) handle;

    res = NU_Write(*fd, (CHAR *) buffer, (INT32) bytesToWrite);
    if (res == bytesToWrite)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        switch (res)
        {
            case NUF_NOSPC:
                rval = CSR_FILE_RESULT_NO_SPACE;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    *written = (CsrSize) res;

    return rval;
}

CsrResult CsrFileRead(void *buffer, CsrSize bytesToRead,
    CsrFileHandle *handle, CsrSize *bytesRead)
{
    INT *fd;
    INT32 res;
    CsrResult rval;

    fd = (INT *) handle;

    res = NU_Read(*fd, (CHAR *) buffer, (INT32) bytesToRead);
    if (res > 0)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else if (res == 0)
    {
        rval = CSR_FILE_RESULT_EOF;
    }
    else
    {
        switch (res)
        {
            case NUF_ACCES:
                rval = CSR_FILE_RESULT_READ_ONLY;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    *bytesRead = res;

    return rval;
}

CsrResult CsrFileSeek(CsrFileHandle *handle, CsrInt32 offset,
    CsrInt32 relativeOffset)
{
    INT *fd;
    INT32 res;
    CsrResult rval;
    UINT16 how;

    fd = (INT *) handle;

    switch (relativeOffset)
    {
        case CSR_SEEK_CUR:
            how = PSEEK_CUR;
            break;

        case CSR_SEEK_SET:
            how = PSEEK_SET;
            break;

        case CSR_SEEK_END:
            how = PSEEK_END;
            break;

        default:
            return CSR_RESULT_FAILURE;
    }

    res = NU_Seek(*fd, (INT32) offset, how);

    if (res >= 0)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    return rval;
}

CsrResult CsrFileFlush(CsrFileHandle *handle)
{
    INT *fd;
    STATUS res;
    CsrResult rval;

    fd = (INT *) handle;

    res = NU_Flush(*fd);

    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    return rval;
}

CsrResult CsrFileRemove(const CsrUtf8String *fileName)
{
    CsrCharString *nName;
    STATUS res;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    nName = convertFsalName(fileName);

    res = NU_Delete(nName);

    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    CsrPmemFree(nName);

    return rval;
}

CsrResult CsrFileTell(CsrFileHandle *handle, CsrUint32 *position)
{
    INT *fd;
    INT32 res;
    CsrResult rval;

    fd = (INT *) handle;

    res = NU_Seek(*fd, 0, PSEEK_CUR);

    if (res >= 0)
    {
        *position = (CsrUint32) res;
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    return rval;
}

CsrResult CsrFileSetEndOfFile(CsrFileHandle *handle)
{
    INT *fd;
    INT32 res;
    CsrResult rval;

    fd = (INT *) handle;

    res = NU_Seek(*fd, 0, PSEEK_CUR);

    if ((res >= 0) &&
        (NU_Truncate(*fd, res) == NU_SUCCESS))
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    return rval;
}

CsrResult CsrFileSetPerms(const CsrUtf8String *fileName,
    CsrFilePerms perms)
{
    CsrCharString *nName;
    CsrFilePerms wrPerms;
    CsrResult rval;
    STATUS res;
    UINT8 attr;

    csrRamDiskReadyEnsure();

    nName = convertFsalName(fileName);

    res = NU_Get_Attributes(&attr, nName);
    if (res == NU_SUCCESS)
    {
        wrPerms = CSR_FILE_PERMS_USER_WRITE;
        wrPerms |= CSR_FILE_PERMS_GROUP_WRITE;
        wrPerms |= CSR_FILE_PERMS_OTHERS_WRITE;

        if (perms & wrPerms)
        {
            attr &= ~ARDONLY;
        }
        else
        {
            attr |= ARDONLY;
        }

        res = NU_Set_Attributes(nName, attr);
    }

    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    CsrPmemFree(nName);

    return rval;
}

static CsrCharString *absFilePath(const CsrUtf8String *path)
{
    CsrCharString *rval;

    if (path[0] == CSR_DIR_PATH_SEP)
    {
        /* Source is absolute path so simply convert it. */

        rval = convertFsalName(path);
    }
    else
    {
        CsrResult res;
        CsrUtf8String *cwd;

        res = CsrDirGetCurrentWorkingDir(&cwd);
        if (res == NU_SUCCESS)
        {
            CsrUtf8String *tmp;
            CsrSize absPathLen, tmpLen;

            absPathLen = CsrUtf8StringLengthInBytes(cwd) +
                         sizeof(CSR_DIR_PATH_SEP) +
                         CsrUtf8StringLengthInBytes(path) + 1;

            tmp = CsrPmemAlloc(absPathLen);

            CsrStrCpy((CsrCharString *) tmp, (CsrCharString *) cwd);
            tmpLen = CsrUtf8StringLengthInBytes(tmp);
            if (tmp[tmpLen - 1] != CSR_DIR_PATH_SEP)
            {
                tmp[tmpLen] = CSR_DIR_PATH_SEP;
                tmp[tmpLen + 1] = '\0';
            }
            CsrStrCat((CsrCharString *) tmp, (CsrCharString *) path);

            rval = convertFsalName(tmp);
            CsrPmemFree(tmp);
        }
        else
        {
            rval = NULL;
        }
    }

    return rval;
}

static CsrCharString *nfDirName(const CsrCharString *path)
{
    CsrCharString *copy, *p, *sep;

    copy = CsrStrDup(path);

    for (sep = p = copy;
         *p != '\0';
         p++)
    {
        if (*p == '\\')
        {
            sep = p;
        }
    }

    if (sep == copy)
    {
        /* No path info. */
    }
    else if (*(sep + 1) == '\0')
    {
        /* Path is on the form ``asdf/qwer/'' */
    }
    else
    {
        /* Path ends with a file name.  Strip it off. */
        *(sep + 1) = '\0';
    }

    return copy;
}

/* operates on NF names */
static CsrCharString *nfBaseName(const CsrCharString *path)
{
    const CsrCharString *p, *sep;
    CsrCharString *rval;

    for (sep = p = path;
         *p != '\0';
         p++)
    {
        if (*p == '\\')
        {
            sep = p;
        }
    }

    if (*(sep + 1) == '\0')
    {
        /* Path ends with a dir separator. */
        rval = NULL;
    }
    else
    {
        /* Take path following last '\\'. */
        rval = CsrStrDup(sep + 1);
    }

    return rval;
}

CsrResult CsrFileRename(const CsrUtf8String *srcName,
    const CsrUtf8String *dstName)
{
    CsrCharString *absSourcePath, *absDstPath;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    absSourcePath = absFilePath(srcName);
    absDstPath = absFilePath(dstName);
    if ((absSourcePath != NULL) && (absDstPath != NULL))
    {
        CsrCharString *newName, *newDir;
        CsrCharString *dstFile;
        CsrUtf8String *oldcwd;
        STATUS res;

        newDir = nfDirName(absDstPath);
        newName = nfBaseName(absDstPath);
        if (newName != NULL)
        {
            dstFile = newName;
        }
        else
        {
            /* Destination is a directory so copy filename from source. */
            dstFile = nfBaseName(absSourcePath);
        }

        CsrDirGetCurrentWorkingDir(&oldcwd);

        NU_Set_Current_Dir(newDir);

        res = NU_Rename(absSourcePath, dstFile);

        if (res == NU_SUCCESS)
        {
            rval = CSR_RESULT_SUCCESS;
        }
        else
        {
            rval = CSR_RESULT_FAILURE;
        }

        CsrDirChange(oldcwd);
        CsrPmemFree(oldcwd);
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    CsrPmemFree(absSourcePath);
    CsrPmemFree(absDstPath);


    return rval;
}

CsrResult CsrDirGetCurrentWorkingDir(CsrUtf8String **dirName)
{
    CHAR cwd[EMAXPATH + 1];
    STATUS res;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    res = NU_Current_Dir("A:", cwd);

    if (res == NU_SUCCESS)
    {
        *dirName = convertNFName(cwd);
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        rval = CSR_RESULT_FAILURE;
    }

    return rval;
}

CsrResult CsrDirStat(const CsrUtf8String *path, CsrDirEntryStat *fileStat)
{
    CsrCharString *nPattern;
    STATUS res;
    DSTAT statObj;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    nPattern = convertFsalName(path);

    res = NU_Get_First(&statObj, nPattern);

    CsrPmemFree(nPattern);

    if (res == NU_SUCCESS)
    {
        fileStat->mode = CSR_DIR_MODE_USER_READ_PERMISSION;

        if ((statObj.fattribute & ARDONLY) == 0)
        {
            fileStat->mode |= CSR_DIR_MODE_USER_WRITE_PERMISSION;
        }

        if (statObj.fattribute & ADIRENT)
        {
            fileStat->mode |= CSR_DIR_MODE_DIRECTORY;
        }
        else
        {
            fileStat->mode |= CSR_DIR_MODE_REGULAR_FILE;
        }

        fileStat->size = statObj.fsize;

        CsrMemSet(&fileStat->time, 0, sizeof(fileStat->time));

        NU_Done(&statObj);
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        switch (res)
        {
            case NUF_NOFILE:
                rval = CSR_DIR_RESULT_NOT_EXIST;
                break;

            case NUF_INVNAME:
            case NUF_LONGPATH:
                rval = CSR_DIR_RESULT_INVALID_PATH;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    return rval;
}

CsrResult CsrDirMake(const CsrUtf8String *dirName)
{
    CsrCharString *nName;
    STATUS res;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    nName = convertFsalName(dirName);

    res = NU_Make_Dir(nName);
    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        switch (res)
        {
            case NUF_EXIST:
                rval = CSR_DIR_RESULT_ALREADY_EXIST;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    CsrPmemFree(nName);

    return rval;
}

CsrResult CsrDirRemove(const CsrUtf8String *dirName)
{
    CsrCharString *nName;
    STATUS res;
    CsrResult rval;

    csrRamDiskReadyEnsure();

    nName = convertFsalName(dirName);

    res = NU_Remove_Dir(nName);
    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        switch (res)
        {
            case NUF_NOFILE:
                rval = CSR_DIR_RESULT_NOT_EXIST;
                break;

            case NUF_NOEMPTY:
                rval = CSR_DIR_RESULT_NOT_EMPTY;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    CsrPmemFree(nName);

    return rval;
}

CsrResult CsrDirChange(const CsrUtf8String *dirName)
{
    CsrCharString *nDirName;
    CsrResult rval;
    STATUS res;

    csrRamDiskReadyEnsure();

    nDirName = convertFsalName(dirName);

    res = NU_Set_Current_Dir(nDirName);

    if (res == NU_SUCCESS)
    {
        rval = CSR_RESULT_SUCCESS;
    }
    else
    {
        switch (res)
        {
            case NUF_ACCES:
            case NUF_LONGPATH:
                rval = CSR_DIR_RESULT_INVALID_PATH;
                break;

            default:
                rval = CSR_RESULT_FAILURE;
                break;
        }
    }

    CsrPmemFree(nDirName);

    return rval;
}

struct csrDirStatList
{
    CsrDirFindStat        *fileStat;
    struct csrDirStatList *next;
};

struct csrDirStatHandle
{
    struct csrDirStatList *dsl;
};

CsrDirHandle *CsrDirFindFirst(const CsrUtf8String *searchPattern,
    CsrDirFindStat *dirStat)
{
    CsrDirHandle *dirHdl;
    CsrCharString *nPattern;
    STATUS res;
    DSTAT *statObj;

    csrRamDiskReadyEnsure();

    statObj = CsrPmemAlloc(sizeof(*statObj));
    nPattern = convertFsalName(searchPattern);

    res = NU_Get_First(statObj, nPattern);
    if (res == NU_SUCCESS)
    {
        struct csrDirStatHandle *hdl;
        struct csrDirStatList *dsl, *p;

        dsl = CsrPmemAlloc(sizeof(*dsl));
        dsl->next = NULL;
        dsl->fileStat = convertDSTAT(statObj);

        p = dsl;

        while (NU_Get_Next(statObj) == NU_SUCCESS)
        {
            p->next = CsrPmemAlloc(sizeof(*dsl));

            p = p->next;
            p->next = NULL;
            p->fileStat = convertDSTAT(statObj);
        }

        NU_Done(statObj);

        /* Copy first result out and return pointer to next as handle. */
        CsrMemCpy(dirStat, dsl->fileStat, sizeof(*dirStat));
        CsrPmemFree(dsl->fileStat);
        CsrPmemFree(dsl);

        hdl = CsrPmemAlloc(sizeof(*dsl));
        hdl->dsl = dsl->next;

        dirHdl = (CsrDirHandle *) hdl;
    }
    else
    {
        dirHdl = NULL;
    }
    CsrPmemFree(statObj);
    CsrPmemFree(nPattern);


    return dirHdl;
}

CsrResult CsrDirFindNext(CsrDirHandle *handle, CsrDirFindStat *dirStat)
{
    CsrResult rval;
    struct csrDirStatHandle *hdl;
    struct csrDirStatList *ds;

    hdl = (struct csrDirStatHandle *) handle;
    ds = hdl->dsl;

    if (ds != NULL)
    {
        struct csrDirStatList *next;

        rval = CSR_RESULT_SUCCESS;

        next = ds->next;

        CsrMemCpy(dirStat, ds->fileStat, sizeof(*dirStat));
        CsrPmemFree(ds->fileStat);
        CsrPmemFree(ds);

        hdl->dsl = next;
    }
    else
    {
        rval = CSR_DIR_RESULT_NOT_EXIST;
    }

    return rval;
}

CsrResult CsrDirFindClose(CsrDirHandle *handle)
{
    struct csrDirStatHandle *hdl;
    struct csrDirStatList *ds;

    hdl = (struct csrDirStatHandle *) handle;
    ds = hdl->dsl;

    while (ds != NULL)
    {
        struct csrDirStatList *next;

        next = ds->next;
        CsrPmemFree(ds->fileStat);
        CsrPmemFree(ds);
        ds = next;
    }
    CsrPmemFree(hdl);

    return CSR_RESULT_SUCCESS;
}
