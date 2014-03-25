/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* For SetEndOfFile et al under Windows */
#include <windows.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "csr_file.h"
#include "csr_dir.h"
#include "csr_util.h"
#include "csr_unicode.h"
#include "csr_pmem.h"

extern int errno;

static int returnMappedFlags(CsrFileOpenFlags mode)
{
    int ret = _O_BINARY;

    ret |= (mode & CSR_FILE_OPEN_FLAGS_APPEND) ? _O_APPEND : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_READ_ONLY) ? _O_RDONLY : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_READ_WRITE) ? _O_RDWR : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_WRITE_ONLY) ? _O_WRONLY : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_CREATE) ? _O_CREAT : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_TRUNCATE) ? _O_TRUNC : 0;
    ret |= (mode & CSR_FILE_OPEN_FLAGS_EXCL) ? _O_EXCL : 0;

    return ret;
}

static int returnMappedPerms(CsrFilePerms mode)
{
    int ret = 0;

    /* note that read and write flags are the same for all user/group/others and execute flags are ignored on windows */
    ret |= ((mode & CSR_FILE_PERMS_USER_READ) ? _S_IREAD : 0);
    ret |= ((mode & CSR_FILE_PERMS_USER_WRITE) ? _S_IWRITE : 0);
    ret |= ((mode & CSR_FILE_PERMS_USER_EXECUTE) ? 0 : 0);
    ret |= ((mode & CSR_FILE_PERMS_GROUP_READ) ? _S_IREAD : 0);
    ret |= ((mode & CSR_FILE_PERMS_GROUP_WRITE) ? _S_IWRITE : 0);
    ret |= ((mode & CSR_FILE_PERMS_GROUP_EXECUTE) ? 0 : 0);
    ret |= ((mode & CSR_FILE_PERMS_OTHERS_READ) ? _S_IREAD : 0);
    ret |= ((mode & CSR_FILE_PERMS_OTHERS_WRITE) ? _S_IWRITE : 0);
    ret |= ((mode & CSR_FILE_PERMS_OTHERS_EXECUTE) ? 0 : 0);

    return ret;
}

typedef struct
{
    int handle;
} csrFileHandle;

#define MAP_FILE_OPEN(result) (result != -1 ? CSR_RESULT_SUCCESS : \
                               errno == EACCES ? CSR_FILE_RESULT_NOT_ALLOWED : \
                               errno == EEXIST ? CSR_FILE_RESULT_ALREAD_EXISTS : \
                               errno == ENOENT ? CSR_FILE_RESULT_NOT_EXIST : \
                               CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileOpen(CsrFileHandle **handle, const CsrUtf8String *fileName, CsrFileOpenFlags flags, CsrFilePerms perms)
{
    CsrUtf16String *wFileName = CsrUtf82Utf16String(fileName);
    CsrInt32 wMode = returnMappedFlags(flags);
    CsrInt32 wPerms = returnMappedPerms(perms);
    csrFileHandle *wHandle = CsrPmemAlloc(sizeof(csrFileHandle));
    int fileHandle;

    fileHandle = _wopen(wFileName, wMode, wPerms);

    if (fileHandle >= 0)
    {
        wHandle->handle = fileHandle;
        *handle = wHandle;
    }
    else
    {
        CsrPmemFree(wHandle);
        *handle = NULL;
    }
    CsrPmemFree(wFileName);

    return MAP_FILE_OPEN(fileHandle);
}

#define MAP_FILE_CLOSE(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileClose(CsrFileHandle *handle)
{
    int result;
    csrFileHandle *wHandle = handle;
    result = _close(wHandle->handle);
    CsrPmemFree(handle);
    return MAP_FILE_CLOSE(result);
}

#define MAP_FILE_WRITE(result, _bytesToWrite) (result == _bytesToWrite ? CSR_RESULT_SUCCESS : \
                                               errno == EBADF ? CSR_FILE_RESULT_NOT_ALLOWED : \
                                               errno == ENOSPC ? CSR_FILE_RESULT_NO_SPACE : \
                                               CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileWrite(const void *buffer, CsrSize bytesToWrite,
    CsrFileHandle *handle, CsrSize *written)
{
    csrFileHandle *wHandle = handle;
    *written = _write(wHandle->handle, buffer, (unsigned int) bytesToWrite);
    return MAP_FILE_WRITE(*written, bytesToWrite);
}

#define MAP_FILE_READ(result, _handle, _bytesToRead) ((result >= 0) ? ((((CsrSize) result) == _bytesToRead) ? CSR_RESULT_SUCCESS : (result == 0 ? CSR_FILE_RESULT_EOF : CSR_RESULT_SUCCESS)) : \
                                                      errno == EBADF ? CSR_FILE_RESULT_NOT_ALLOWED : \
                                                      CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileRead(void *buffer, CsrSize bytesToRead,
    CsrFileHandle *handle, CsrSize *bytesRead)
{
    csrFileHandle *wHandle = handle;
    int bytesActualRead = _read(wHandle->handle, buffer, (unsigned int) bytesToRead);
    *bytesRead = bytesActualRead;
    return MAP_FILE_READ(bytesActualRead, wHandle->handle, bytesToRead);
}

#define MAP_FILE_SEEK(result) (result >= 0 ? CSR_RESULT_SUCCESS : \
                               errno == EBADF ? CSR_FILE_RESULT_NOT_ALLOWED : \
                               CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileSeek(CsrFileHandle *handle, CsrInt32 offset, CsrInt32 relativeOffset)
{
    csrFileHandle *wHandle = handle;
    int o, result;

    switch (relativeOffset)
    {
        case CSR_SEEK_SET:
            o = SEEK_SET;
            break;

        case CSR_SEEK_CUR:
            o = SEEK_CUR;
            break;

        case CSR_SEEK_END:
            o = SEEK_END;
            break;

        default:
            o = SEEK_SET;
            break;
    }

    result = _lseek(wHandle->handle, offset, o);
    return MAP_FILE_SEEK(result);
}

#define MAP_FILE_FLUSH(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                errno == EBADF ? CSR_FILE_RESULT_NOT_ALLOWED : \
                                CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileFlush(CsrFileHandle *handle)
{
    csrFileHandle *wHandle = handle;
    int result;
    result = _commit(wHandle->handle);
    return MAP_FILE_FLUSH(result);
}

#define MAP_FILE_SET_END_OF_FILE(result) (result == TRUE ? CSR_RESULT_SUCCESS : \
                                          CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileSetEndOfFile(CsrFileHandle *handle)
{
    csrFileHandle *wHandle = handle;
    int result;
    HANDLE hand;
    hand = (HANDLE) _get_osfhandle(wHandle->handle);
    if (hand != INVALID_HANDLE_VALUE)
    {
        result = SetEndOfFile(hand);
    }
    else
    {
        result = FALSE;
    }
    return MAP_FILE_SET_END_OF_FILE(result);
}

#define MAP_FILE_PERMS(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                errno == ENOENT ? CSR_FILE_RESULT_NOT_EXIST : \
                                CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileSetPerms(const CsrUtf8String *name, CsrFilePerms perms)  /* NB: May also be used on directories */
{
    CsrUtf16String *wName = CsrUtf82Utf16String(name);
    CsrInt32 wPerms = returnMappedPerms(perms);
    int result = _wchmod(wName, wPerms);
    CsrPmemFree(wName);
    return MAP_FILE_PERMS(result);
}

#define MAP_FILE_REMOVE(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                 errno == EACCES ? CSR_FILE_RESULT_NOT_ALLOWED : \
                                 errno == ENOENT ? CSR_FILE_RESULT_NOT_EXIST : \
                                 CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileRemove(const CsrUtf8String *fileName)
{
    int result;
    CsrUtf16String *wFileName = CsrUtf82Utf16String(fileName);

    result = _wremove(wFileName);

    CsrPmemFree(wFileName);
    return MAP_FILE_REMOVE(result);
}

#define MAP_FILE_RENAME(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                 errno == EACCES ? CSR_FILE_RESULT_NOT_ALLOWED : \
                                 errno == ENOENT ? CSR_FILE_RESULT_NOT_EXIST : \
                                 CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileRename(const CsrUtf8String *oldName, const CsrUtf8String *newName) /* NB: May also be used on directories */
{
    int result;
    CsrUtf16String *wOldName = CsrUtf82Utf16String(oldName);
    CsrUtf16String *wNewName = CsrUtf82Utf16String(newName);

    result = _wrename(wOldName, wNewName);

    CsrPmemFree(wOldName);
    CsrPmemFree(wNewName);
    return MAP_FILE_RENAME(result);
}

#define MAP_FILE_TELL(result) (result != -1L ? CSR_RESULT_SUCCESS : \
                               errno == EBADF ? CSR_FILE_RESULT_NOT_ALLOWED : \
                               CSR_FILE_RESULT_FAILURE)

CsrResult CsrFileTell(CsrFileHandle *handle, CsrUint32 *position)
{
    long rv;
    csrFileHandle *wHandle = handle;
    rv = _tell(wHandle->handle);

    if (rv >= 0)
    {
        *position = (CsrUint32) rv;
    }
    return MAP_FILE_TELL(rv);
}

#define MAP_DIR_STAT(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                              errno == ENOENT ? CSR_DIR_RESULT_NOT_EXIST : \
                              CSR_DIR_RESULT_FAILURE)

static CsrDirMode convertModeToDirMode(unsigned short st_mode)
{
    CsrDirMode mode;

    mode = st_mode & _S_IFDIR ? CSR_DIR_MODE_DIRECTORY : 0;
    mode |= st_mode & _S_IFREG ? CSR_DIR_MODE_REGULAR_FILE : 0;
    mode |= st_mode & _S_IREAD ? CSR_DIR_MODE_USER_READ_PERMISSION : 0;
    mode |= st_mode & _S_IWRITE ? CSR_DIR_MODE_USER_WRITE_PERMISSION : 0;
    mode |= st_mode & _S_IEXEC ? CSR_DIR_MODE_USER_EXECUTE : 0;
    /* The following is not applicable on windows
        mode |= st_mode & S_IRGRP   ? CSR_DIR_MODE_GROUP_READ_PERMISSION : 0;
        mode |= st_mode & S_IWGRP   ? CSR_DIR_MODE_GROUP_WRITE_PERMISSION : 0;
        mode |= st_mode & S_IXGRP   ? CSR_DIR_MODE_GROUP_EXECUTE : 0;
        mode |= st_mode & S_IROTH   ? CSR_DIR_MODE_OTHERS_READ_PERMISSION : 0;
        mode |= st_mode & S_IWOTH   ? CSR_DIR_MODE_OTHERS_WRITE_PERMISSION : 0;
        mode |= st_mode & S_IXOTH   ? CSR_DIR_MODE_OTHERS_EXECUTE : 0;
    */
    return mode;
}

static void convertToDirTm(CsrDirTm *out, time_t *atime)
{
    struct tm *calTime;

    /* Fill out time in UTC */
    calTime = gmtime(atime);
    out->tm_sec = calTime->tm_sec;
    out->tm_min = calTime->tm_min;
    out->tm_hour = calTime->tm_hour;
    out->tm_mday = calTime->tm_mday;
    out->tm_mon = calTime->tm_mon;
    out->tm_year = calTime->tm_year;
    out->tm_wday = calTime->tm_wday;
    out->tm_yday = calTime->tm_yday;
    out->tm_isdst = calTime->tm_isdst;
    out->utcTime = TRUE;
}

CsrResult CsrDirGetCurrentWorkingDir(CsrUtf8String **dirName)
{
    CsrUtf16String buf[_MAX_PATH];

    if (_wgetcwd(buf, _MAX_PATH) != NULL) /* success */
    {
        /* We strip of the drive letter and lets root dir '/' be the root of the current drive.
         * If needed the porting can automatically extend the root '/' to be under the drive
         * letters if this is needed. In this case C:\ will be mapped in as '/c/' and D:\ as '/d/'
         * this however is not handled in this porting.
         * If the latter porting strategy is choosen buf should not be stripped of at offset 2 but
         * offset 0 and C:\ should be substituted with '/c/'
         */
        CsrUtf8String *path = CsrUtf16String2Utf8(&buf[2]);

        *dirName = path;

        /* we need to substitute backslashes with forward slashes */
        while (*path != '\0')
        {
            if (*path == '\\')
            {
                *path = CSR_DIR_PATH_SEP;
            }
            path++;
        }

        return CSR_RESULT_SUCCESS;
    }
    else /* it failed */
    {
        *dirName = NULL;

        return CSR_DIR_RESULT_FAILURE;
    }
}

CsrResult CsrDirStat(const CsrUtf8String *path, CsrDirEntryStat *fileStat)
{
    int result;
    struct _stat temp;
    CsrUtf16String *wFileName = CsrUtf82Utf16String(path);

    result = _wstat(wFileName, &temp);

    CsrPmemFree(wFileName);

    if (result == 0)
    {
        /* Fill out time in UTC */
        convertToDirTm(&fileStat->time, &temp.st_mtime);

        /* Convert mode */
        fileStat->mode = convertModeToDirMode(temp.st_mode);

        /* File size */
        fileStat->size = temp.st_size;
    }
    return MAP_DIR_STAT(result);
}

#define MAP_DIR_MAKE(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                              errno == EEXIST ? CSR_DIR_RESULT_ALREADY_EXIST : \
                              errno == ENOENT ? CSR_DIR_RESULT_NOT_EXIST : \
                              CSR_DIR_RESULT_FAILURE)

CsrResult CsrDirMake(const CsrUtf8String *dirName)
{
    int result;
    CsrUtf16String *wDirName = CsrUtf82Utf16String(dirName);
    result = _wmkdir(wDirName);
    CsrPmemFree(wDirName);
    return MAP_DIR_MAKE(result);
}

#define MAP_DIR_REMOVE(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                errno == ENOTEMPTY ? CSR_DIR_RESULT_NOT_EMPTY : \
                                errno == ENOENT ? CSR_DIR_RESULT_INVALID_PATH : \
                                CSR_DIR_RESULT_FAILURE)

CsrResult CsrDirRemove(const CsrUtf8String *dirName)
{
    int result;
    CsrUtf16String *wDirName = CsrUtf82Utf16String(dirName);
    result = _wrmdir(wDirName);
    CsrPmemFree(wDirName);
    return MAP_DIR_REMOVE(result);
}

#define MAP_DIR_CHANGE(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                errno == ENOENT ? CSR_DIR_RESULT_NOT_EXIST : \
                                CSR_DIR_RESULT_FAILURE)

CsrResult CsrDirChange(const CsrUtf8String *dirName)
{
    int result;
    CsrUtf16String *wDirName = CsrUtf82Utf16String(dirName);
    result = _wchdir(wDirName);
    CsrPmemFree(wDirName);
    return MAP_DIR_CHANGE(result);
}

typedef struct
{
    CsrIntptr      handle;
    CsrUtf8String *searchPattern;
} csrDir;

static CsrDirMode convertAttribToDirMode(unsigned attrib)
{
    CsrDirMode mode;

    if (attrib & _A_SUBDIR)
    {
        mode = CSR_DIR_MODE_DIRECTORY;
    }
    else /* case _A_NORMAL which is 0x00*/
    {
        mode = CSR_DIR_MODE_REGULAR_FILE;
        mode |= attrib & _A_RDONLY ? CSR_DIR_MODE_USER_READ_PERMISSION : (CSR_DIR_MODE_USER_READ_PERMISSION | CSR_DIR_MODE_USER_WRITE_PERMISSION);
        /* The following is not applicable on windows
            mode |= attrib & _S_IWRITE ? CSR_DIR_MODE_USER_WRITE_PERMISSION : 0;
            mode |= attrib & _S_IEXEC  ? CSR_DIR_MODE_USER_EXECUTE : 0;
            mode |= attrib & S_IRGRP   ? CSR_DIR_MODE_GROUP_READ_PERMISSION : 0;
            mode |= attrib & S_IWGRP   ? CSR_DIR_MODE_GROUP_WRITE_PERMISSION : 0;
            mode |= attrib & S_IXGRP   ? CSR_DIR_MODE_GROUP_EXECUTE : 0;
            mode |= attrib & S_IROTH   ? CSR_DIR_MODE_OTHERS_READ_PERMISSION : 0;
            mode |= attrib & S_IWOTH   ? CSR_DIR_MODE_OTHERS_WRITE_PERMISSION : 0;
            mode |= attrib & S_IXOTH   ? CSR_DIR_MODE_OTHERS_EXECUTE : 0;
        */
    }

    return mode;
}

CsrDirHandle *CsrDirFindFirst(const CsrUtf8String *searchPattern, CsrDirFindStat *dirStat)
{
    csrDir *dir = CsrPmemAlloc(sizeof(csrDir));
    CsrUtf16String *wSearchPattern = CsrUtf82Utf16String(searchPattern);
    struct _wfinddata_t dirData;

    dir->handle = _wfindfirst(wSearchPattern, &dirData);

    if (dir->handle != -1)
    {
        /* Fill out time in UTC */
        convertToDirTm(&dirStat->time, &dirData.time_write);

        dirStat->name = CsrUtf16String2Utf8(dirData.name);

        /* Convert mode */
        dirStat->mode = convertAttribToDirMode(dirData.attrib);

        dirStat->size = dirData.size;
    }
    else
    {
        CsrMemSet(dirStat, 0, sizeof(*dirStat));
        CsrPmemFree(dir);
        dir = NULL;
    }
    CsrPmemFree(wSearchPattern);
    return dir;
}

#define MAP_DIR_FIND_NEXT(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                   errno == ENOENT ? CSR_DIR_RESULT_NOT_EXIST : \
                                   CSR_DIR_RESULT_FAILURE)

CsrResult CsrDirFindNext(CsrDirHandle *handle, CsrDirFindStat *dirStat)
{
    struct _wfinddata_t dirData;
    int result;
    csrDir *dir = handle;

    result = _wfindnext(dir->handle, &dirData);

    if (result != -1)
    {
        /* Fill out time in UTC */
        convertToDirTm(&dirStat->time, &dirData.time_write);

        dirStat->name = CsrUtf16String2Utf8(dirData.name);

        /* Convert mode */
        dirStat->mode = convertAttribToDirMode(dirData.attrib);

        dirStat->size = dirData.size;
    }
    else
    {
        CsrMemSet(dirStat, 0, sizeof(*dirStat));
    }

    return MAP_DIR_FIND_NEXT(result);
}

#define MAP_DIR_FIND_CLOSE(result) (result == 0 ? CSR_RESULT_SUCCESS : \
                                    errno == ENOENT ? CSR_DIR_RESULT_NOT_EXIST : \
                                    CSR_DIR_RESULT_FAILURE)
CsrResult CsrDirFindClose(CsrDirHandle *handle)
{
    int result;
    csrDir *dir = handle;

    result = _findclose(dir->handle);

    CsrPmemFree(dir);
    return MAP_DIR_FIND_CLOSE(result);
}
