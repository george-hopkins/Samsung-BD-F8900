/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_fsal_prim.h"

#ifndef EXCLUDE_CSR_FSAL_MODULE

void CsrFsalPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM
CsrSize CsrFsalSessionDestroyCfmSizeof(void *msg)
{
    CsrFsalSessionDestroyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM
CsrUint8 *CsrFsalSessionDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalSessionDestroyCfm *primitive;

    primitive = (CsrFsalSessionDestroyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM
void *CsrFsalSessionDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalSessionDestroyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalSessionDestroyCfm *) CsrPmemZalloc(sizeof(CsrFsalSessionDestroyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_FSAL_RENAME_CFM
CsrSize CsrFsalRenameCfmSizeof(void *msg)
{
    CsrFsalRenameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_CFM */

#ifndef EXCLUDE_CSR_FSAL_RENAME_CFM
CsrUint8 *CsrFsalRenameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalRenameCfm *primitive;

    primitive = (CsrFsalRenameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_CFM */

#ifndef EXCLUDE_CSR_FSAL_RENAME_CFM
void *CsrFsalRenameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalRenameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalRenameCfm *) CsrPmemZalloc(sizeof(CsrFsalRenameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM
CsrSize CsrFsalDirFindCloseCfmSizeof(void *msg)
{
    CsrFsalDirFindCloseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM
CsrUint8 *CsrFsalDirFindCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirFindCloseCfm *primitive;

    primitive = (CsrFsalDirFindCloseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM
void *CsrFsalDirFindCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirFindCloseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirFindCloseCfm *) CsrPmemZalloc(sizeof(CsrFsalDirFindCloseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM
void CsrFsalDirFindCloseCfmSerFree(void *msg)
{
    CsrFsalDirFindCloseCfm *primitive;

    primitive = (CsrFsalDirFindCloseCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_CFM
CsrSize CsrFsalFileOpenCfmSizeof(void *msg)
{
    CsrFsalFileOpenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_CFM
CsrUint8 *CsrFsalFileOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileOpenCfm *primitive;

    primitive = (CsrFsalFileOpenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->size);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_CFM
void *CsrFsalFileOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileOpenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileOpenCfm *) CsrPmemZalloc(sizeof(CsrFsalFileOpenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->size, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_CFM
void CsrFsalFileOpenCfmSerFree(void *msg)
{
    CsrFsalFileOpenCfm *primitive;

    primitive = (CsrFsalFileOpenCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
CsrSize CsrFsalRemoveRecursivelyReqSizeof(void *msg)
{
    CsrFsalRemoveRecursivelyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->dir));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
CsrUint8 *CsrFsalRemoveRecursivelyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalRemoveRecursivelyReq *primitive;

    primitive = (CsrFsalRemoveRecursivelyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->dir, sizeof(primitive->dir));
    CsrUtf8StringSer(buffer, length, primitive->dir);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
void *CsrFsalRemoveRecursivelyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalRemoveRecursivelyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalRemoveRecursivelyReq *) CsrPmemZalloc(sizeof(CsrFsalRemoveRecursivelyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->dir), buffer, &offset, ((CsrSize) sizeof(primitive->dir)));
    CsrUtf8StringDes(&primitive->dir, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ
void CsrFsalRemoveRecursivelyReqSerFree(void *msg)
{
    CsrFsalRemoveRecursivelyReq *primitive;

    primitive = (CsrFsalRemoveRecursivelyReq *) msg;

    if(primitive->dir != NULL)
    {
        CsrPmemFree(primitive->dir);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
CsrSize CsrFsalDirMakeReqSizeof(void *msg)
{
    CsrFsalDirMakeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->dirName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
CsrUint8 *CsrFsalDirMakeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirMakeReq *primitive;

    primitive = (CsrFsalDirMakeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->dirName, sizeof(primitive->dirName));
    CsrUtf8StringSer(buffer, length, primitive->dirName);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
void *CsrFsalDirMakeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirMakeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirMakeReq *) CsrPmemZalloc(sizeof(CsrFsalDirMakeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->dirName), buffer, &offset, ((CsrSize) sizeof(primitive->dirName)));
    CsrUtf8StringDes(&primitive->dirName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_REQ
void CsrFsalDirMakeReqSerFree(void *msg)
{
    CsrFsalDirMakeReq *primitive;

    primitive = (CsrFsalDirMakeReq *) msg;

    if(primitive->dirName != NULL)
    {
        CsrPmemFree(primitive->dirName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ
CsrSize CsrFsalSessionCreateReqSizeof(void *msg)
{
    CsrFsalSessionCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ
CsrUint8 *CsrFsalSessionCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalSessionCreateReq *primitive;

    primitive = (CsrFsalSessionCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ
void *CsrFsalSessionCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalSessionCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalSessionCreateReq *) CsrPmemZalloc(sizeof(CsrFsalSessionCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_CFM
CsrSize CsrFsalRemoveCfmSizeof(void *msg)
{
    CsrFsalRemoveCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_CFM
CsrUint8 *CsrFsalRemoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalRemoveCfm *primitive;

    primitive = (CsrFsalRemoveCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_CFM
void *CsrFsalRemoveCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalRemoveCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalRemoveCfm *) CsrPmemZalloc(sizeof(CsrFsalRemoveCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
CsrSize CsrFsalStatReqSizeof(void *msg)
{
    CsrFsalStatReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->path));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */

#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
CsrUint8 *CsrFsalStatReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalStatReq *primitive;

    primitive = (CsrFsalStatReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->path, sizeof(primitive->path));
    CsrUtf8StringSer(buffer, length, primitive->path);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */

#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
void *CsrFsalStatReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalStatReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalStatReq *) CsrPmemZalloc(sizeof(CsrFsalStatReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->path), buffer, &offset, ((CsrSize) sizeof(primitive->path)));
    CsrUtf8StringDes(&primitive->path, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */

#ifndef EXCLUDE_CSR_FSAL_STAT_REQ
void CsrFsalStatReqSerFree(void *msg)
{
    CsrFsalStatReq *primitive;

    primitive = (CsrFsalStatReq *) msg;

    if(primitive->path != NULL)
    {
        CsrPmemFree(primitive->path);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_STAT_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
CsrSize CsrFsalDirFindFirstCfmSizeof(void *msg)
{
    CsrFsalDirFindFirstCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->entry.name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
CsrUint8 *CsrFsalDirFindFirstCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirFindFirstCfm *primitive;

    primitive = (CsrFsalDirFindFirstCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->entry.name, sizeof(primitive->entry.name));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_sec);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_min);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_hour);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_mday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_mon);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_year);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_wday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_yday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_isdst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->entry.time.utcTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.size);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->entry.mode);
    CsrUtf8StringSer(buffer, length, primitive->entry.name);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
void *CsrFsalDirFindFirstCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirFindFirstCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirFindFirstCfm *) CsrPmemZalloc(sizeof(CsrFsalDirFindFirstCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->entry.name), buffer, &offset, ((CsrSize) sizeof(primitive->entry.name)));
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_sec, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_min, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_hour, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_mday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_mon, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_year, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_wday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_yday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_isdst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->entry.time.utcTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.size, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->entry.mode, buffer, &offset);
    CsrUtf8StringDes(&primitive->entry.name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM
void CsrFsalDirFindFirstCfmSerFree(void *msg)
{
    CsrFsalDirFindFirstCfm *primitive;

    primitive = (CsrFsalDirFindFirstCfm *) msg;

    if(primitive->entry.name != NULL)
    {
        CsrPmemFree(primitive->entry.name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
CsrSize CsrFsalDirFindNextCfmSizeof(void *msg)
{
    CsrFsalDirFindNextCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->entry.name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
CsrUint8 *CsrFsalDirFindNextCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirFindNextCfm *primitive;

    primitive = (CsrFsalDirFindNextCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrMemCpySer(buffer, length, &primitive->entry.name, sizeof(primitive->entry.name));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_sec);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_min);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_hour);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_mday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_mon);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_year);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_wday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_yday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.time.tm_isdst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->entry.time.utcTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->entry.size);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->entry.mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUtf8StringSer(buffer, length, primitive->entry.name);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
void *CsrFsalDirFindNextCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirFindNextCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirFindNextCfm *) CsrPmemZalloc(sizeof(CsrFsalDirFindNextCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrMemCpyDes(((void *) &primitive->entry.name), buffer, &offset, ((CsrSize) sizeof(primitive->entry.name)));
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_sec, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_min, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_hour, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_mday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_mon, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_year, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_wday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_yday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.time.tm_isdst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->entry.time.utcTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->entry.size, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->entry.mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUtf8StringDes(&primitive->entry.name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM
void CsrFsalDirFindNextCfmSerFree(void *msg)
{
    CsrFsalDirFindNextCfm *primitive;

    primitive = (CsrFsalDirFindNextCfm *) msg;

    if(primitive->entry.name != NULL)
    {
        CsrPmemFree(primitive->entry.name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_REQ
CsrSize CsrFsalFileTellReqSizeof(void *msg)
{
    CsrFsalFileTellReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_REQ
CsrUint8 *CsrFsalFileTellReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileTellReq *primitive;

    primitive = (CsrFsalFileTellReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_REQ
void *CsrFsalFileTellReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileTellReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileTellReq *) CsrPmemZalloc(sizeof(CsrFsalFileTellReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_REQ
void CsrFsalFileTellReqSerFree(void *msg)
{
    CsrFsalFileTellReq *primitive;

    primitive = (CsrFsalFileTellReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ
CsrSize CsrFsalDirFindCloseReqSizeof(void *msg)
{
    CsrFsalDirFindCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ
CsrUint8 *CsrFsalDirFindCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirFindCloseReq *primitive;

    primitive = (CsrFsalDirFindCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ
void *CsrFsalDirFindCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirFindCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirFindCloseReq *) CsrPmemZalloc(sizeof(CsrFsalDirFindCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ
void CsrFsalDirFindCloseReqSerFree(void *msg)
{
    CsrFsalDirFindCloseReq *primitive;

    primitive = (CsrFsalDirFindCloseReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
CsrSize CsrFsalSessionCreateCfmSizeof(void *msg)
{
    CsrFsalSessionCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->currentWorkingDir));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
CsrUint8 *CsrFsalSessionCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalSessionCreateCfm *primitive;

    primitive = (CsrFsalSessionCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->currentWorkingDir, sizeof(primitive->currentWorkingDir));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUtf8StringSer(buffer, length, primitive->currentWorkingDir);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
void *CsrFsalSessionCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalSessionCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalSessionCreateCfm *) CsrPmemZalloc(sizeof(CsrFsalSessionCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->currentWorkingDir), buffer, &offset, ((CsrSize) sizeof(primitive->currentWorkingDir)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUtf8StringDes(&primitive->currentWorkingDir, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM
void CsrFsalSessionCreateCfmSerFree(void *msg)
{
    CsrFsalSessionCreateCfm *primitive;

    primitive = (CsrFsalSessionCreateCfm *) msg;

    if(primitive->currentWorkingDir != NULL)
    {
        CsrPmemFree(primitive->currentWorkingDir);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
CsrSize CsrFsalFileOpenReqSizeof(void *msg)
{
    CsrFsalFileOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->fileName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
CsrUint8 *CsrFsalFileOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileOpenReq *primitive;

    primitive = (CsrFsalFileOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->permissions);
    CsrMemCpySer(buffer, length, &primitive->fileName, sizeof(primitive->fileName));
    CsrUtf8StringSer(buffer, length, primitive->fileName);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
void *CsrFsalFileOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileOpenReq *) CsrPmemZalloc(sizeof(CsrFsalFileOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->permissions, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->fileName), buffer, &offset, ((CsrSize) sizeof(primitive->fileName)));
    CsrUtf8StringDes(&primitive->fileName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_OPEN_REQ
void CsrFsalFileOpenReqSerFree(void *msg)
{
    CsrFsalFileOpenReq *primitive;

    primitive = (CsrFsalFileOpenReq *) msg;

    if(primitive->fileName != NULL)
    {
        CsrPmemFree(primitive->fileName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_OPEN_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ
CsrSize CsrFsalFileCloseReqSizeof(void *msg)
{
    CsrFsalFileCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ
CsrUint8 *CsrFsalFileCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileCloseReq *primitive;

    primitive = (CsrFsalFileCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ
void *CsrFsalFileCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileCloseReq *) CsrPmemZalloc(sizeof(CsrFsalFileCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ
void CsrFsalFileCloseReqSerFree(void *msg)
{
    CsrFsalFileCloseReq *primitive;

    primitive = (CsrFsalFileCloseReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_CFM
CsrSize CsrFsalFileWriteCfmSizeof(void *msg)
{
    CsrFsalFileWriteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_CFM
CsrUint8 *CsrFsalFileWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileWriteCfm *primitive;

    primitive = (CsrFsalFileWriteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bytesWritten);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_CFM
void *CsrFsalFileWriteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileWriteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileWriteCfm *) CsrPmemZalloc(sizeof(CsrFsalFileWriteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bytesWritten, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_CFM
void CsrFsalFileWriteCfmSerFree(void *msg)
{
    CsrFsalFileWriteCfm *primitive;

    primitive = (CsrFsalFileWriteCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
CsrSize CsrFsalFileWriteReqSizeof(void *msg)
{
    CsrFsalFileWriteReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
CsrUint8 *CsrFsalFileWriteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileWriteReq *primitive;

    primitive = (CsrFsalFileWriteReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
void *CsrFsalFileWriteReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileWriteReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileWriteReq *) CsrPmemZalloc(sizeof(CsrFsalFileWriteReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint32Des((CsrUint32 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLen)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_WRITE_REQ
void CsrFsalFileWriteReqSerFree(void *msg)
{
    CsrFsalFileWriteReq *primitive;

    primitive = (CsrFsalFileWriteReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_WRITE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ
CsrSize CsrFsalDirFindNextReqSizeof(void *msg)
{
    CsrFsalDirFindNextReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ
CsrUint8 *CsrFsalDirFindNextReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirFindNextReq *primitive;

    primitive = (CsrFsalDirFindNextReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ
void *CsrFsalDirFindNextReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirFindNextReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirFindNextReq *) CsrPmemZalloc(sizeof(CsrFsalDirFindNextReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ
void CsrFsalDirFindNextReqSerFree(void *msg)
{
    CsrFsalDirFindNextReq *primitive;

    primitive = (CsrFsalDirFindNextReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_NEXT_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ
CsrSize CsrFsalSessionDestroyReqSizeof(void *msg)
{
    CsrFsalSessionDestroyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ
CsrUint8 *CsrFsalSessionDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalSessionDestroyReq *primitive;

    primitive = (CsrFsalSessionDestroyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ
void *CsrFsalSessionDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalSessionDestroyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalSessionDestroyReq *) CsrPmemZalloc(sizeof(CsrFsalSessionDestroyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_CFM
CsrSize CsrFsalDirMakeCfmSizeof(void *msg)
{
    CsrFsalDirMakeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_CFM
CsrUint8 *CsrFsalDirMakeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirMakeCfm *primitive;

    primitive = (CsrFsalDirMakeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_MAKE_CFM
void *CsrFsalDirMakeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirMakeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirMakeCfm *) CsrPmemZalloc(sizeof(CsrFsalDirMakeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_MAKE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_REQ
CsrSize CsrFsalFileReadReqSizeof(void *msg)
{
    CsrFsalFileReadReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_REQ
CsrUint8 *CsrFsalFileReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileReadReq *primitive;

    primitive = (CsrFsalFileReadReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bytesToRead);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_REQ
void *CsrFsalFileReadReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileReadReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileReadReq *) CsrPmemZalloc(sizeof(CsrFsalFileReadReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint32Des((CsrUint32 *) &primitive->bytesToRead, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_REQ
void CsrFsalFileReadReqSerFree(void *msg)
{
    CsrFsalFileReadReq *primitive;

    primitive = (CsrFsalFileReadReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
CsrSize CsrFsalPermissionsSetReqSizeof(void *msg)
{
    CsrFsalPermissionsSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->path));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
CsrUint8 *CsrFsalPermissionsSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalPermissionsSetReq *primitive;

    primitive = (CsrFsalPermissionsSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->path, sizeof(primitive->path));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->permissions);
    CsrUtf8StringSer(buffer, length, primitive->path);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
void *CsrFsalPermissionsSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalPermissionsSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalPermissionsSetReq *) CsrPmemZalloc(sizeof(CsrFsalPermissionsSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->path), buffer, &offset, ((CsrSize) sizeof(primitive->path)));
    CsrUint16Des((CsrUint16 *) &primitive->permissions, buffer, &offset);
    CsrUtf8StringDes(&primitive->path, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ
void CsrFsalPermissionsSetReqSerFree(void *msg)
{
    CsrFsalPermissionsSetReq *primitive;

    primitive = (CsrFsalPermissionsSetReq *) msg;

    if(primitive->path != NULL)
    {
        CsrPmemFree(primitive->path);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_REQ */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM
CsrSize CsrFsalPermissionsSetCfmSizeof(void *msg)
{
    CsrFsalPermissionsSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM
CsrUint8 *CsrFsalPermissionsSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalPermissionsSetCfm *primitive;

    primitive = (CsrFsalPermissionsSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM */

#ifndef EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM
void *CsrFsalPermissionsSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalPermissionsSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalPermissionsSetCfm *) CsrPmemZalloc(sizeof(CsrFsalPermissionsSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_PERMISSIONS_SET_CFM */

#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
CsrSize CsrFsalRenameReqSizeof(void *msg)
{
    CsrFsalRenameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->oldPath) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->newPath));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */

#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
CsrUint8 *CsrFsalRenameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalRenameReq *primitive;

    primitive = (CsrFsalRenameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->oldPath, sizeof(primitive->oldPath));
    CsrMemCpySer(buffer, length, &primitive->newPath, sizeof(primitive->newPath));
    CsrUtf8StringSer(buffer, length, primitive->oldPath);
    CsrUtf8StringSer(buffer, length, primitive->newPath);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */

#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
void *CsrFsalRenameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalRenameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalRenameReq *) CsrPmemZalloc(sizeof(CsrFsalRenameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->oldPath), buffer, &offset, ((CsrSize) sizeof(primitive->oldPath)));
    CsrMemCpyDes(((void *) &primitive->newPath), buffer, &offset, ((CsrSize) sizeof(primitive->newPath)));
    CsrUtf8StringDes(&primitive->oldPath, buffer, &offset);
    CsrUtf8StringDes(&primitive->newPath, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */

#ifndef EXCLUDE_CSR_FSAL_RENAME_REQ
void CsrFsalRenameReqSerFree(void *msg)
{
    CsrFsalRenameReq *primitive;

    primitive = (CsrFsalRenameReq *) msg;

    if(primitive->oldPath != NULL)
    {
        CsrPmemFree(primitive->oldPath);
    }
    if(primitive->newPath != NULL)
    {
        CsrPmemFree(primitive->newPath);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_RENAME_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM
CsrSize CsrFsalRemoveRecursivelyCfmSizeof(void *msg)
{
    CsrFsalRemoveRecursivelyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM
CsrUint8 *CsrFsalRemoveRecursivelyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalRemoveRecursivelyCfm *primitive;

    primitive = (CsrFsalRemoveRecursivelyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM
void *CsrFsalRemoveRecursivelyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalRemoveRecursivelyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalRemoveRecursivelyCfm *) CsrPmemZalloc(sizeof(CsrFsalRemoveRecursivelyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_RECURSIVELY_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
CsrSize CsrFsalDirFindFirstReqSizeof(void *msg)
{
    CsrFsalDirFindFirstReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->searchPattern));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
CsrUint8 *CsrFsalDirFindFirstReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirFindFirstReq *primitive;

    primitive = (CsrFsalDirFindFirstReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->searchPattern, sizeof(primitive->searchPattern));
    CsrUtf8StringSer(buffer, length, primitive->searchPattern);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
void *CsrFsalDirFindFirstReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirFindFirstReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirFindFirstReq *) CsrPmemZalloc(sizeof(CsrFsalDirFindFirstReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->searchPattern), buffer, &offset, ((CsrSize) sizeof(primitive->searchPattern)));
    CsrUtf8StringDes(&primitive->searchPattern, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ
void CsrFsalDirFindFirstReqSerFree(void *msg)
{
    CsrFsalDirFindFirstReq *primitive;

    primitive = (CsrFsalDirFindFirstReq *) msg;

    if(primitive->searchPattern != NULL)
    {
        CsrPmemFree(primitive->searchPattern);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_FIND_FIRST_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM
CsrSize CsrFsalFileCloseCfmSizeof(void *msg)
{
    CsrFsalFileCloseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM
CsrUint8 *CsrFsalFileCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileCloseCfm *primitive;

    primitive = (CsrFsalFileCloseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM
void *CsrFsalFileCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileCloseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileCloseCfm *) CsrPmemZalloc(sizeof(CsrFsalFileCloseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM
void CsrFsalFileCloseCfmSerFree(void *msg)
{
    CsrFsalFileCloseCfm *primitive;

    primitive = (CsrFsalFileCloseCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
CsrSize CsrFsalStatCfmSizeof(void *msg)
{
    CsrFsalStatCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->stat.name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
CsrUint8 *CsrFsalStatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalStatCfm *primitive;

    primitive = (CsrFsalStatCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->stat.name, sizeof(primitive->stat.name));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_sec);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_min);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_hour);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_mday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_mon);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_year);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_wday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_yday);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.time.tm_isdst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->stat.time.utcTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->stat.size);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->stat.mode);
    CsrUtf8StringSer(buffer, length, primitive->stat.name);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
void *CsrFsalStatCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalStatCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalStatCfm *) CsrPmemZalloc(sizeof(CsrFsalStatCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->stat.name), buffer, &offset, ((CsrSize) sizeof(primitive->stat.name)));
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_sec, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_min, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_hour, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_mday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_mon, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_year, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_wday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_yday, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.time.tm_isdst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->stat.time.utcTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->stat.size, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->stat.mode, buffer, &offset);
    CsrUtf8StringDes(&primitive->stat.name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */

#ifndef EXCLUDE_CSR_FSAL_STAT_CFM
void CsrFsalStatCfmSerFree(void *msg)
{
    CsrFsalStatCfm *primitive;

    primitive = (CsrFsalStatCfm *) msg;

    if(primitive->stat.name != NULL)
    {
        CsrPmemFree(primitive->stat.name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_STAT_CFM */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
CsrSize CsrFsalRemoveReqSizeof(void *msg)
{
    CsrFsalRemoveReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->path));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
CsrUint8 *CsrFsalRemoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalRemoveReq *primitive;

    primitive = (CsrFsalRemoveReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->path, sizeof(primitive->path));
    CsrUtf8StringSer(buffer, length, primitive->path);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
void *CsrFsalRemoveReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalRemoveReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalRemoveReq *) CsrPmemZalloc(sizeof(CsrFsalRemoveReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->path), buffer, &offset, ((CsrSize) sizeof(primitive->path)));
    CsrUtf8StringDes(&primitive->path, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */

#ifndef EXCLUDE_CSR_FSAL_REMOVE_REQ
void CsrFsalRemoveReqSerFree(void *msg)
{
    CsrFsalRemoveReq *primitive;

    primitive = (CsrFsalRemoveReq *) msg;

    if(primitive->path != NULL)
    {
        CsrPmemFree(primitive->path);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_REMOVE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
CsrSize CsrFsalDirChangeCfmSizeof(void *msg)
{
    CsrFsalDirChangeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->currentWorkingDir));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
CsrUint8 *CsrFsalDirChangeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirChangeCfm *primitive;

    primitive = (CsrFsalDirChangeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->currentWorkingDir, sizeof(primitive->currentWorkingDir));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUtf8StringSer(buffer, length, primitive->currentWorkingDir);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
void *CsrFsalDirChangeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirChangeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirChangeCfm *) CsrPmemZalloc(sizeof(CsrFsalDirChangeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->currentWorkingDir), buffer, &offset, ((CsrSize) sizeof(primitive->currentWorkingDir)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUtf8StringDes(&primitive->currentWorkingDir, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM
void CsrFsalDirChangeCfmSerFree(void *msg)
{
    CsrFsalDirChangeCfm *primitive;

    primitive = (CsrFsalDirChangeCfm *) msg;

    if(primitive->currentWorkingDir != NULL)
    {
        CsrPmemFree(primitive->currentWorkingDir);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_REQ
CsrSize CsrFsalFileSeekReqSizeof(void *msg)
{
    CsrFsalFileSeekReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_REQ
CsrUint8 *CsrFsalFileSeekReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileSeekReq *primitive;

    primitive = (CsrFsalFileSeekReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->offset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->origin);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_REQ
void *CsrFsalFileSeekReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileSeekReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileSeekReq *) CsrPmemZalloc(sizeof(CsrFsalFileSeekReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint32Des((CsrUint32 *) &primitive->offset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->origin, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_REQ
void CsrFsalFileSeekReqSerFree(void *msg)
{
    CsrFsalFileSeekReq *primitive;

    primitive = (CsrFsalFileSeekReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
CsrSize CsrFsalDirChangeReqSizeof(void *msg)
{
    CsrFsalDirChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->path));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
CsrUint8 *CsrFsalDirChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalDirChangeReq *primitive;

    primitive = (CsrFsalDirChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sessionId);
    CsrMemCpySer(buffer, length, &primitive->path, sizeof(primitive->path));
    CsrUtf8StringSer(buffer, length, primitive->path);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
void *CsrFsalDirChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalDirChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalDirChangeReq *) CsrPmemZalloc(sizeof(CsrFsalDirChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sessionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->path), buffer, &offset, ((CsrSize) sizeof(primitive->path)));
    CsrUtf8StringDes(&primitive->path, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */

#ifndef EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ
void CsrFsalDirChangeReqSerFree(void *msg)
{
    CsrFsalDirChangeReq *primitive;

    primitive = (CsrFsalDirChangeReq *) msg;

    if(primitive->path != NULL)
    {
        CsrPmemFree(primitive->path);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_DIR_CHANGE_REQ */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
CsrSize CsrFsalFileReadCfmSizeof(void *msg)
{
    CsrFsalFileReadCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
CsrUint8 *CsrFsalFileReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileReadCfm *primitive;

    primitive = (CsrFsalFileReadCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
void *CsrFsalFileReadCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileReadCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileReadCfm *) CsrPmemZalloc(sizeof(CsrFsalFileReadCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLen)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_READ_CFM
void CsrFsalFileReadCfmSerFree(void *msg)
{
    CsrFsalFileReadCfm *primitive;

    primitive = (CsrFsalFileReadCfm *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_READ_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_CFM
CsrSize CsrFsalFileSeekCfmSizeof(void *msg)
{
    CsrFsalFileSeekCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_CFM
CsrUint8 *CsrFsalFileSeekCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileSeekCfm *primitive;

    primitive = (CsrFsalFileSeekCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_CFM
void *CsrFsalFileSeekCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileSeekCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileSeekCfm *) CsrPmemZalloc(sizeof(CsrFsalFileSeekCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_SEEK_CFM
void CsrFsalFileSeekCfmSerFree(void *msg)
{
    CsrFsalFileSeekCfm *primitive;

    primitive = (CsrFsalFileSeekCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_SEEK_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_CFM
CsrSize CsrFsalFileTellCfmSizeof(void *msg)
{
    CsrFsalFileTellCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_CFM
CsrUint8 *CsrFsalFileTellCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrFsalFileTellCfm *primitive;

    primitive = (CsrFsalFileTellCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->handle, sizeof(primitive->handle));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->position);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_CFM
void *CsrFsalFileTellCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrFsalFileTellCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrFsalFileTellCfm *) CsrPmemZalloc(sizeof(CsrFsalFileTellCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handle), buffer, &offset, ((CsrSize) sizeof(primitive->handle)));
    CsrUint32Des((CsrUint32 *) &primitive->position, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_CFM */

#ifndef EXCLUDE_CSR_FSAL_FILE_TELL_CFM
void CsrFsalFileTellCfmSerFree(void *msg)
{
    CsrFsalFileTellCfm *primitive;

    primitive = (CsrFsalFileTellCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_FSAL_FILE_TELL_CFM */

#endif /* EXCLUDE_CSR_FSAL_MODULE */
