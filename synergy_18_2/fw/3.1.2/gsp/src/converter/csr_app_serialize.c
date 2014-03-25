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
#include "csr_app_prim.h"

#ifndef EXCLUDE_CSR_APP_MODULE

void CsrAppPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
CsrSize CsrAppBacklogReqSizeof(void *msg)
{
    CsrAppBacklogReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->menuName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->subMenuName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->logText));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */

#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
CsrUint8 *CsrAppBacklogReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAppBacklogReq *primitive;

    primitive = (CsrAppBacklogReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->menuName, sizeof(primitive->menuName));
    CsrMemCpySer(buffer, length, &primitive->subMenuName, sizeof(primitive->subMenuName));
    CsrMemCpySer(buffer, length, &primitive->logText, sizeof(primitive->logText));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->causePopup);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->causeBacklog);
    CsrUtf8StringSer(buffer, length, primitive->menuName);
    CsrUtf8StringSer(buffer, length, primitive->subMenuName);
    CsrUtf8StringSer(buffer, length, primitive->logText);

    return buffer;
}
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */

#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
void *CsrAppBacklogReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAppBacklogReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAppBacklogReq *) CsrPmemZalloc(sizeof(CsrAppBacklogReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->menuName), buffer, &offset, ((CsrSize) sizeof(primitive->menuName)));
    CsrMemCpyDes(((void *) &primitive->subMenuName), buffer, &offset, ((CsrSize) sizeof(primitive->subMenuName)));
    CsrMemCpyDes(((void *) &primitive->logText), buffer, &offset, ((CsrSize) sizeof(primitive->logText)));
    CsrUint8Des((CsrUint8 *) &primitive->causePopup, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->causeBacklog, buffer, &offset);
    CsrUtf8StringDes(&primitive->menuName, buffer, &offset);
    CsrUtf8StringDes(&primitive->subMenuName, buffer, &offset);
    CsrUtf8StringDes(&primitive->logText, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */

#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
void CsrAppBacklogReqSerFree(void *msg)
{
    CsrAppBacklogReq *primitive;

    primitive = (CsrAppBacklogReq *) msg;

    if(primitive->menuName != NULL)
    {
        CsrPmemFree(primitive->menuName);
    }
    if(primitive->subMenuName != NULL)
    {
        CsrPmemFree(primitive->subMenuName);
    }
    if(primitive->logText != NULL)
    {
        CsrPmemFree(primitive->logText);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */

#ifndef EXCLUDE_CSR_APP_REGISTER_CFM
CsrSize CsrAppRegisterCfmSizeof(void *msg)
{
    CsrAppRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_APP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_APP_REGISTER_CFM
CsrUint8 *CsrAppRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAppRegisterCfm *primitive;

    primitive = (CsrAppRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->controlId);

    return buffer;
}
#endif /* EXCLUDE_CSR_APP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_APP_REGISTER_CFM
void *CsrAppRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAppRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAppRegisterCfm *) CsrPmemZalloc(sizeof(CsrAppRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->controlId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_APP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
CsrSize CsrAppRegisterReqSizeof(void *msg)
{
    CsrAppRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->menuName) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->subMenuName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
CsrUint8 *CsrAppRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAppRegisterReq *primitive;

    primitive = (CsrAppRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrMemCpySer(buffer, length, &primitive->menuName, sizeof(primitive->menuName));
    CsrMemCpySer(buffer, length, &primitive->subMenuName, sizeof(primitive->subMenuName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menuIcon);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->subMenuIcon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->top);
    CsrUtf8StringSer(buffer, length, primitive->menuName);
    CsrUtf8StringSer(buffer, length, primitive->subMenuName);

    return buffer;
}
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
void *CsrAppRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAppRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAppRegisterReq *) CsrPmemZalloc(sizeof(CsrAppRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->menuName), buffer, &offset, ((CsrSize) sizeof(primitive->menuName)));
    CsrMemCpyDes(((void *) &primitive->subMenuName), buffer, &offset, ((CsrSize) sizeof(primitive->subMenuName)));
    CsrUint16Des((CsrUint16 *) &primitive->menuIcon, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->subMenuIcon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->top, buffer, &offset);
    CsrUtf8StringDes(&primitive->menuName, buffer, &offset);
    CsrUtf8StringDes(&primitive->subMenuName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
void CsrAppRegisterReqSerFree(void *msg)
{
    CsrAppRegisterReq *primitive;

    primitive = (CsrAppRegisterReq *) msg;

    if(primitive->menuName != NULL)
    {
        CsrPmemFree(primitive->menuName);
    }
    if(primitive->subMenuName != NULL)
    {
        CsrPmemFree(primitive->subMenuName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_APP_TAKE_CONTROL_IND
CsrSize CsrAppTakeControlIndSizeof(void *msg)
{
    CsrAppTakeControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_APP_TAKE_CONTROL_IND */

#ifndef EXCLUDE_CSR_APP_TAKE_CONTROL_IND
CsrUint8 *CsrAppTakeControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrAppTakeControlInd *primitive;

    primitive = (CsrAppTakeControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->controlId);

    return buffer;
}
#endif /* EXCLUDE_CSR_APP_TAKE_CONTROL_IND */

#ifndef EXCLUDE_CSR_APP_TAKE_CONTROL_IND
void *CsrAppTakeControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrAppTakeControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrAppTakeControlInd *) CsrPmemZalloc(sizeof(CsrAppTakeControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->controlId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_APP_TAKE_CONTROL_IND */

#endif /* EXCLUDE_CSR_APP_MODULE */
