/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_fsal_prim.h"
#include "csr_fsal_handler.h"

#include "csr_fsal_lib.h"
#include "csr_fsal_sef.h"
#include "csr_fsal_sef_utils.h"

#include "csr_pmem.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalLookupOpenFlags
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFileOpenFlags CsrFsalLookupOpenFlags(CsrFsalFileOpenFlags flags)
{
    CsrFileOpenFlags openFlags = 0;

    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_APPEND) ? CSR_FILE_OPEN_FLAGS_APPEND : 0;
    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_READ_ONLY) ? CSR_FILE_OPEN_FLAGS_READ_ONLY : 0;
    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_READ_WRITE) ? CSR_FILE_OPEN_FLAGS_READ_WRITE : 0;
    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_WRITE_ONLY) ? CSR_FILE_OPEN_FLAGS_WRITE_ONLY : 0;
    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_CREATE) ? CSR_FILE_OPEN_FLAGS_CREATE : 0;
    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_TRUNCATE) ? CSR_FILE_OPEN_FLAGS_TRUNCATE : 0;
    openFlags |= (flags & CSR_FSAL_OPEN_FLAGS_EXCL) ? CSR_FILE_OPEN_FLAGS_EXCL : 0;

    return openFlags;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalLookupPerms
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrFilePerms CsrFsalLookupPerms(CsrFsalFileOpenPerms perms)
{
    CsrFilePerms openPerms = 0;

    openPerms |= (perms & CSR_FSAL_OPEN_PERMS_READ) ? CSR_FILE_PERMS_USER_READ : 0;
    openPerms |= (perms & CSR_FSAL_OPEN_PERMS_WRITE) ? CSR_FILE_PERMS_USER_WRITE : 0;

    return openPerms;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalFileSize
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrResult CsrFsalFileSize(const CsrUtf8String *file, CsrSize *filesize)
{
    CsrDirEntryStat buffer;
    CsrResult result;
    result = CsrDirStat(file, &buffer);

    if (result == CSR_RESULT_SUCCESS)
    {
        *filesize = buffer.size;
        result = CSR_RESULT_SUCCESS;
    }
    else
    {
        result = CSR_FSAL_FILE_OP_FAILURE;
    }
    return result;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrFsalRemoveRecursively
 *
 *  DESCRIPTION
 *
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
CsrResult CsrFsalRemoveRecursively(CsrUtf8String *path)
{
    CsrDirFindStat findStat;
    CsrDirHandle *handle;
    CsrResult result = CSR_RESULT_SUCCESS;
    CsrUtf8String *searchPattern;
    CsrUint32 length = CsrUtf8StringLengthInBytes(path);

    if (path[length] == '/')
    {
        path[length] = '\0';
    }

    searchPattern = CsrUtf8StringConcatenateTexts(path, (CsrUtf8String *) "/*", NULL, NULL);

    if ((handle = CsrDirFindFirst(searchPattern, &findStat)) != NULL)
    {
        do
        {
            CsrUtf8String *newPath = CsrUtf8StringConcatenateTexts(path, (CsrUtf8String *) "/", findStat.name, NULL);

            if ((CsrUtf8StrCmp(findStat.name, (CsrUtf8String *) ".") == 0) || (CsrUtf8StrCmp(findStat.name, (CsrUtf8String *) "..") == 0)) /* skip . and .. */
            {
            }
            else
            {
                if (findStat.mode & CSR_DIR_MODE_DIRECTORY)
                {
                    result = CsrFsalRemoveRecursively(newPath);
                    if (result != CSR_RESULT_SUCCESS)
                    {
                        CsrPmemFree(searchPattern);
                        CsrPmemFree(newPath);
                        CsrPmemFree(findStat.name);
                        CsrDirFindClose(handle);

                        return CSR_RESULT_FAILURE;
                    }

                    result = CsrDirRemove(newPath);
                    if (result != CSR_RESULT_SUCCESS)
                    {
                        CsrPmemFree(searchPattern);
                        CsrPmemFree(newPath);
                        CsrPmemFree(findStat.name);
                        CsrDirFindClose(handle);

                        return CSR_RESULT_FAILURE;
                    }
                }
                else
                {
                    result = CsrFileRemove(newPath);
                    if (result != CSR_RESULT_SUCCESS)
                    {
                        CsrPmemFree(searchPattern);
                        CsrPmemFree(newPath);
                        CsrPmemFree(findStat.name);
                        CsrDirFindClose(handle);

                        return CSR_RESULT_FAILURE;
                    }
                }
            }
            CsrPmemFree(newPath);
            CsrPmemFree(findStat.name);
        } while (CsrDirFindNext(handle, &findStat) == CSR_RESULT_SUCCESS);

        result = CsrDirFindClose(handle);
        if (result != CSR_RESULT_SUCCESS)
        {
            CsrPmemFree(searchPattern);

            return CSR_RESULT_FAILURE;
        }
    }

    CsrPmemFree(searchPattern);

    return CSR_RESULT_SUCCESS;
}
