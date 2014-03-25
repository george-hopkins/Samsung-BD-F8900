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
#include "csr_ui_private_prim.h"

#ifndef EXCLUDE_CSR_UI_PRIVATE_MODULE

void CsrUiPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ
CsrSize CsrUiStatusHeadsetSetReqSizeof(void *msg)
{
    CsrUiStatusHeadsetSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ
CsrUint8 *CsrUiStatusHeadsetSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiStatusHeadsetSetReq *primitive;

    primitive = (CsrUiStatusHeadsetSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->headset1);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->headset2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ
void *CsrUiStatusHeadsetSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiStatusHeadsetSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiStatusHeadsetSetReq *) CsrPmemZalloc(sizeof(CsrUiStatusHeadsetSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->headset1, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->headset2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_STATUS_HEADSET_SET_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_CFM
CsrSize CsrUiUieCreateCfmSizeof(void *msg)
{
    CsrUiUieCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_CFM */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_CFM
CsrUint8 *CsrUiUieCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiUieCreateCfm *primitive;

    primitive = (CsrUiUieCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->elementType);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_CFM */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_CFM
void *CsrUiUieCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiUieCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiUieCreateCfm *) CsrPmemZalloc(sizeof(CsrUiUieCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->elementType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM
CsrSize CsrUiMenuGetcursorCfmSizeof(void *msg)
{
    CsrUiMenuGetcursorCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM
CsrUint8 *CsrUiMenuGetcursorCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuGetcursorCfm *primitive;

    primitive = (CsrUiMenuGetcursorCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM
void *CsrUiMenuGetcursorCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuGetcursorCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuGetcursorCfm *) CsrPmemZalloc(sizeof(CsrUiMenuGetcursorCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_REQ
CsrSize CsrUiMenuGetitemReqSizeof(void *msg)
{
    CsrUiMenuGetitemReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_REQ
CsrUint8 *CsrUiMenuGetitemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuGetitemReq *primitive;

    primitive = (CsrUiMenuGetitemReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_REQ
void *CsrUiMenuGetitemReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuGetitemReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuGetitemReq *) CsrPmemZalloc(sizeof(CsrUiMenuGetitemReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ
CsrSize CsrUiInputdialogGetReqSizeof(void *msg)
{
    CsrUiInputdialogGetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ
CsrUint8 *CsrUiInputdialogGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiInputdialogGetReq *primitive;

    primitive = (CsrUiInputdialogGetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputDialog);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ
void *CsrUiInputdialogGetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiInputdialogGetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiInputdialogGetReq *) CsrPmemZalloc(sizeof(CsrUiInputdialogGetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputDialog, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
CsrSize CsrUiIdlescreenSetReqSizeof(void *msg)
{
    CsrUiIdlescreenSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->heading) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->text) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK1) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK2));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
CsrUint8 *CsrUiIdlescreenSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiIdlescreenSetReq *primitive;

    primitive = (CsrUiIdlescreenSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->idleScreen);
    CsrMemCpySer(buffer, length, &primitive->heading, sizeof(primitive->heading));
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->textLength);
    CsrMemCpySer(buffer, length, &primitive->textSK1, sizeof(primitive->textSK1));
    CsrMemCpySer(buffer, length, &primitive->textSK2, sizeof(primitive->textSK2));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->heading);
    CsrUtf16StringSer(buffer, length, primitive->text);
    CsrUtf16StringSer(buffer, length, primitive->textSK1);
    CsrUtf16StringSer(buffer, length, primitive->textSK2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
void *CsrUiIdlescreenSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiIdlescreenSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiIdlescreenSetReq *) CsrPmemZalloc(sizeof(CsrUiIdlescreenSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->idleScreen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->heading), buffer, &offset, ((CsrSize) sizeof(primitive->heading)));
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrUint16Des((CsrUint16 *) &primitive->textLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->textSK1), buffer, &offset, ((CsrSize) sizeof(primitive->textSK1)));
    CsrMemCpyDes(((void *) &primitive->textSK2), buffer, &offset, ((CsrSize) sizeof(primitive->textSK2)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->heading, buffer, &offset);
    CsrUtf16StringDes(&primitive->text, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK1, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
void CsrUiIdlescreenSetReqSerFree(void *msg)
{
    CsrUiIdlescreenSetReq *primitive;

    primitive = (CsrUiIdlescreenSetReq *) msg;

    if(primitive->heading != NULL)
    {
        CsrPmemFree(primitive->heading);
    }
    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    if(primitive->textSK1 != NULL)
    {
        CsrPmemFree(primitive->textSK1);
    }
    if(primitive->textSK2 != NULL)
    {
        CsrPmemFree(primitive->textSK2);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
CsrSize CsrUiMenuAdditemReqSizeof(void *msg)
{
    CsrUiMenuAdditemReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->label) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->sublabel));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
CsrUint8 *CsrUiMenuAdditemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuAdditemReq *primitive;

    primitive = (CsrUiMenuAdditemReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->position);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->icon);
    CsrMemCpySer(buffer, length, &primitive->label, sizeof(primitive->label));
    CsrMemCpySer(buffer, length, &primitive->sublabel, sizeof(primitive->sublabel));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->label);
    CsrUtf16StringSer(buffer, length, primitive->sublabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
void *CsrUiMenuAdditemReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuAdditemReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuAdditemReq *) CsrPmemZalloc(sizeof(CsrUiMenuAdditemReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->position, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->icon, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->label), buffer, &offset, ((CsrSize) sizeof(primitive->label)));
    CsrMemCpyDes(((void *) &primitive->sublabel), buffer, &offset, ((CsrSize) sizeof(primitive->sublabel)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->label, buffer, &offset);
    CsrUtf16StringDes(&primitive->sublabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
void CsrUiMenuAdditemReqSerFree(void *msg)
{
    CsrUiMenuAdditemReq *primitive;

    primitive = (CsrUiMenuAdditemReq *) msg;

    if(primitive->label != NULL)
    {
        CsrPmemFree(primitive->label);
    }
    if(primitive->sublabel != NULL)
    {
        CsrPmemFree(primitive->sublabel);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ
CsrSize CsrUiStatusBatterySetReqSizeof(void *msg)
{
    CsrUiStatusBatterySetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ
CsrUint8 *CsrUiStatusBatterySetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiStatusBatterySetReq *primitive;

    primitive = (CsrUiStatusBatterySetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->percentage);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ
void *CsrUiStatusBatterySetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiStatusBatterySetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiStatusBatterySetReq *) CsrPmemZalloc(sizeof(CsrUiStatusBatterySetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->percentage, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_STATUS_BATTERY_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ
CsrSize CsrUiStatusRadiometerSetReqSizeof(void *msg)
{
    CsrUiStatusRadiometerSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ
CsrUint8 *CsrUiStatusRadiometerSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiStatusRadiometerSetReq *primitive;

    primitive = (CsrUiStatusRadiometerSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->percentage);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ
void *CsrUiStatusRadiometerSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiStatusRadiometerSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiStatusRadiometerSetReq *) CsrPmemZalloc(sizeof(CsrUiStatusRadiometerSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->percentage, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_STATUS_RADIOMETER_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND
CsrSize CsrUiInternalKeydownIndSizeof(void *msg)
{
    CsrUiInternalKeydownInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND
CsrUint8 *CsrUiInternalKeydownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiInternalKeydownInd *primitive;

    primitive = (CsrUiInternalKeydownInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND
void *CsrUiInternalKeydownIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiInternalKeydownInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiInternalKeydownInd *) CsrPmemZalloc(sizeof(CsrUiInternalKeydownInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_INTERNAL_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_EVENT_IND
CsrSize CsrUiEventIndSizeof(void *msg)
{
    CsrUiEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_EVENT_IND */

#ifndef EXCLUDE_CSR_UI_EVENT_IND
CsrUint8 *CsrUiEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiEventInd *primitive;

    primitive = (CsrUiEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->event);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->displayElement);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_EVENT_IND */

#ifndef EXCLUDE_CSR_UI_EVENT_IND
void *CsrUiEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiEventInd *) CsrPmemZalloc(sizeof(CsrUiEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->event, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->displayElement, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_EVENT_IND */

#ifndef EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ
CsrSize CsrUiMenuSetcursorReqSizeof(void *msg)
{
    CsrUiMenuSetcursorReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ
CsrUint8 *CsrUiMenuSetcursorReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuSetcursorReq *primitive;

    primitive = (CsrUiMenuSetcursorReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ
void *CsrUiMenuSetcursorReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuSetcursorReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuSetcursorReq *) CsrPmemZalloc(sizeof(CsrUiMenuSetcursorReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_SETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ
CsrSize CsrUiMenuGetcursorReqSizeof(void *msg)
{
    CsrUiMenuGetcursorReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ
CsrUint8 *CsrUiMenuGetcursorReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuGetcursorReq *primitive;

    primitive = (CsrUiMenuGetcursorReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ
void *CsrUiMenuGetcursorReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuGetcursorReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuGetcursorReq *) CsrPmemZalloc(sizeof(CsrUiMenuGetcursorReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETCURSOR_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ
CsrSize CsrUiDisplayGethandleReqSizeof(void *msg)
{
    CsrUiDisplayGethandleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ
CsrUint8 *CsrUiDisplayGethandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiDisplayGethandleReq *primitive;

    primitive = (CsrUiDisplayGethandleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ
void *CsrUiDisplayGethandleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiDisplayGethandleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiDisplayGethandleReq *) CsrPmemZalloc(sizeof(CsrUiDisplayGethandleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GET_REQ
CsrSize CsrUiMenuGetReqSizeof(void *msg)
{
    CsrUiMenuGetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GET_REQ
CsrUint8 *CsrUiMenuGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuGetReq *primitive;

    primitive = (CsrUiMenuGetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GET_REQ
void *CsrUiMenuGetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuGetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuGetReq *) CsrPmemZalloc(sizeof(CsrUiMenuGetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ
CsrSize CsrUiStatusBluetoothSetReqSizeof(void *msg)
{
    CsrUiStatusBluetoothSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ
CsrUint8 *CsrUiStatusBluetoothSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiStatusBluetoothSetReq *primitive;

    primitive = (CsrUiStatusBluetoothSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->active);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ
void *CsrUiStatusBluetoothSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiStatusBluetoothSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiStatusBluetoothSetReq *) CsrPmemZalloc(sizeof(CsrUiStatusBluetoothSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->active, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_STATUS_BLUETOOTH_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ
CsrSize CsrUiMenuRemoveallitemsReqSizeof(void *msg)
{
    CsrUiMenuRemoveallitemsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ
CsrUint8 *CsrUiMenuRemoveallitemsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuRemoveallitemsReq *primitive;

    primitive = (CsrUiMenuRemoveallitemsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ
void *CsrUiMenuRemoveallitemsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuRemoveallitemsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuRemoveallitemsReq *) CsrPmemZalloc(sizeof(CsrUiMenuRemoveallitemsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEALLITEMS_REQ */

#ifndef EXCLUDE_CSR_UI_KEYDOWN_IND
CsrSize CsrUiKeydownIndSizeof(void *msg)
{
    CsrUiKeydownInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_KEYDOWN_IND
CsrUint8 *CsrUiKeydownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiKeydownInd *primitive;

    primitive = (CsrUiKeydownInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->displayElement);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_KEYDOWN_IND
void *CsrUiKeydownIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiKeydownInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiKeydownInd *) CsrPmemZalloc(sizeof(CsrUiKeydownInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->displayElement, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_KEYDOWN_IND */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ
CsrSize CsrUiMenuRemoveitemReqSizeof(void *msg)
{
    CsrUiMenuRemoveitemReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ
CsrUint8 *CsrUiMenuRemoveitemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuRemoveitemReq *primitive;

    primitive = (CsrUiMenuRemoveitemReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ
void *CsrUiMenuRemoveitemReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuRemoveitemReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuRemoveitemReq *) CsrPmemZalloc(sizeof(CsrUiMenuRemoveitemReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_REMOVEITEM_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_SHOW_REQ
CsrSize CsrUiUieShowReqSizeof(void *msg)
{
    CsrUiUieShowReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_UIE_SHOW_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_SHOW_REQ
CsrUint8 *CsrUiUieShowReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiUieShowReq *primitive;

    primitive = (CsrUiUieShowReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->element);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listener);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->priority);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_UIE_SHOW_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_SHOW_REQ
void *CsrUiUieShowReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiUieShowReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiUieShowReq *) CsrPmemZalloc(sizeof(CsrUiUieShowReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->element, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listener, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->priority, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_UIE_SHOW_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
CsrSize CsrUiInputdialogSetReqSizeof(void *msg)
{
    CsrUiInputdialogSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->heading) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->message) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->text) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK1) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK2));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
CsrUint8 *CsrUiInputdialogSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiInputdialogSetReq *primitive;

    primitive = (CsrUiInputdialogSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputDialog);
    CsrMemCpySer(buffer, length, &primitive->heading, sizeof(primitive->heading));
    CsrMemCpySer(buffer, length, &primitive->message, sizeof(primitive->message));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->icon);
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->textLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keyMap);
    CsrMemCpySer(buffer, length, &primitive->textSK1, sizeof(primitive->textSK1));
    CsrMemCpySer(buffer, length, &primitive->textSK2, sizeof(primitive->textSK2));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->heading);
    CsrUtf16StringSer(buffer, length, primitive->message);
    CsrUtf16StringSer(buffer, length, primitive->text);
    CsrUtf16StringSer(buffer, length, primitive->textSK1);
    CsrUtf16StringSer(buffer, length, primitive->textSK2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
void *CsrUiInputdialogSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiInputdialogSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiInputdialogSetReq *) CsrPmemZalloc(sizeof(CsrUiInputdialogSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputDialog, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->heading), buffer, &offset, ((CsrSize) sizeof(primitive->heading)));
    CsrMemCpyDes(((void *) &primitive->message), buffer, &offset, ((CsrSize) sizeof(primitive->message)));
    CsrUint16Des((CsrUint16 *) &primitive->icon, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrUint16Des((CsrUint16 *) &primitive->textLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keyMap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->textSK1), buffer, &offset, ((CsrSize) sizeof(primitive->textSK1)));
    CsrMemCpyDes(((void *) &primitive->textSK2), buffer, &offset, ((CsrSize) sizeof(primitive->textSK2)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->heading, buffer, &offset);
    CsrUtf16StringDes(&primitive->message, buffer, &offset);
    CsrUtf16StringDes(&primitive->text, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK1, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
void CsrUiInputdialogSetReqSerFree(void *msg)
{
    CsrUiInputdialogSetReq *primitive;

    primitive = (CsrUiInputdialogSetReq *) msg;

    if(primitive->heading != NULL)
    {
        CsrPmemFree(primitive->heading);
    }
    if(primitive->message != NULL)
    {
        CsrPmemFree(primitive->message);
    }
    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    if(primitive->textSK1 != NULL)
    {
        CsrPmemFree(primitive->textSK1);
    }
    if(primitive->textSK2 != NULL)
    {
        CsrPmemFree(primitive->textSK2);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
CsrSize CsrUiMenuGetCfmSizeof(void *msg)
{
    CsrUiMenuGetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->heading) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK1) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK2));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
CsrUint8 *CsrUiMenuGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuGetCfm *primitive;

    primitive = (CsrUiMenuGetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrMemCpySer(buffer, length, &primitive->heading, sizeof(primitive->heading));
    CsrMemCpySer(buffer, length, &primitive->textSK1, sizeof(primitive->textSK1));
    CsrMemCpySer(buffer, length, &primitive->textSK2, sizeof(primitive->textSK2));
    CsrUtf16StringSer(buffer, length, primitive->heading);
    CsrUtf16StringSer(buffer, length, primitive->textSK1);
    CsrUtf16StringSer(buffer, length, primitive->textSK2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
void *CsrUiMenuGetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuGetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuGetCfm *) CsrPmemZalloc(sizeof(CsrUiMenuGetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->heading), buffer, &offset, ((CsrSize) sizeof(primitive->heading)));
    CsrMemCpyDes(((void *) &primitive->textSK1), buffer, &offset, ((CsrSize) sizeof(primitive->textSK1)));
    CsrMemCpyDes(((void *) &primitive->textSK2), buffer, &offset, ((CsrSize) sizeof(primitive->textSK2)));
    CsrUtf16StringDes(&primitive->heading, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK1, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
void CsrUiMenuGetCfmSerFree(void *msg)
{
    CsrUiMenuGetCfm *primitive;

    primitive = (CsrUiMenuGetCfm *) msg;

    if(primitive->heading != NULL)
    {
        CsrPmemFree(primitive->heading);
    }
    if(primitive->textSK1 != NULL)
    {
        CsrPmemFree(primitive->textSK1);
    }
    if(primitive->textSK2 != NULL)
    {
        CsrPmemFree(primitive->textSK2);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_REQ
CsrSize CsrUiDialogGetReqSizeof(void *msg)
{
    CsrUiDialogGetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_REQ
CsrUint8 *CsrUiDialogGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiDialogGetReq *primitive;

    primitive = (CsrUiDialogGetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dialog);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_REQ
void *CsrUiDialogGetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiDialogGetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiDialogGetReq *) CsrPmemZalloc(sizeof(CsrUiDialogGetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dialog, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ
CsrSize CsrUiDisplaySetinputmodeReqSizeof(void *msg)
{
    CsrUiDisplaySetinputmodeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ
CsrUint8 *CsrUiDisplaySetinputmodeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiDisplaySetinputmodeReq *primitive;

    primitive = (CsrUiDisplaySetinputmodeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->element);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ
void *CsrUiDisplaySetinputmodeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiDisplaySetinputmodeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiDisplaySetinputmodeReq *) CsrPmemZalloc(sizeof(CsrUiDisplaySetinputmodeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->element, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_SETINPUTMODE_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_DESTROY_REQ
CsrSize CsrUiUieDestroyReqSizeof(void *msg)
{
    CsrUiUieDestroyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_UIE_DESTROY_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_DESTROY_REQ
CsrUint8 *CsrUiUieDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiUieDestroyReq *primitive;

    primitive = (CsrUiUieDestroyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->element);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_UIE_DESTROY_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_DESTROY_REQ
void *CsrUiUieDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiUieDestroyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiUieDestroyReq *) CsrPmemZalloc(sizeof(CsrUiUieDestroyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->element, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_UIE_DESTROY_REQ */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM
CsrSize CsrUiDisplayGethandleCfmSizeof(void *msg)
{
    CsrUiDisplayGethandleCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM
CsrUint8 *CsrUiDisplayGethandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiDisplayGethandleCfm *primitive;

    primitive = (CsrUiDisplayGethandleCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM */

#ifndef EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM
void *CsrUiDisplayGethandleCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiDisplayGethandleCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiDisplayGethandleCfm *) CsrPmemZalloc(sizeof(CsrUiDisplayGethandleCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_DISPLAY_GETHANDLE_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
CsrSize CsrUiMenuSetitemReqSizeof(void *msg)
{
    CsrUiMenuSetitemReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->label) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->sublabel));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
CsrUint8 *CsrUiMenuSetitemReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuSetitemReq *primitive;

    primitive = (CsrUiMenuSetitemReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->icon);
    CsrMemCpySer(buffer, length, &primitive->label, sizeof(primitive->label));
    CsrMemCpySer(buffer, length, &primitive->sublabel, sizeof(primitive->sublabel));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->label);
    CsrUtf16StringSer(buffer, length, primitive->sublabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
void *CsrUiMenuSetitemReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuSetitemReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuSetitemReq *) CsrPmemZalloc(sizeof(CsrUiMenuSetitemReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->icon, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->label), buffer, &offset, ((CsrSize) sizeof(primitive->label)));
    CsrMemCpyDes(((void *) &primitive->sublabel), buffer, &offset, ((CsrSize) sizeof(primitive->sublabel)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->label, buffer, &offset);
    CsrUtf16StringDes(&primitive->sublabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
void CsrUiMenuSetitemReqSerFree(void *msg)
{
    CsrUiMenuSetitemReq *primitive;

    primitive = (CsrUiMenuSetitemReq *) msg;

    if(primitive->label != NULL)
    {
        CsrPmemFree(primitive->label);
    }
    if(primitive->sublabel != NULL)
    {
        CsrPmemFree(primitive->sublabel);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_REQ
CsrSize CsrUiUieCreateReqSizeof(void *msg)
{
    CsrUiUieCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_REQ
CsrUint8 *CsrUiUieCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiUieCreateReq *primitive;

    primitive = (CsrUiUieCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->elementType);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_CREATE_REQ
void *CsrUiUieCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiUieCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiUieCreateReq *) CsrPmemZalloc(sizeof(CsrUiUieCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->elementType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_UIE_CREATE_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
CsrSize CsrUiIdlescreenGetCfmSizeof(void *msg)
{
    CsrUiIdlescreenGetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->text));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
CsrUint8 *CsrUiIdlescreenGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiIdlescreenGetCfm *primitive;

    primitive = (CsrUiIdlescreenGetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrUtf16StringSer(buffer, length, primitive->text);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
void *CsrUiIdlescreenGetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiIdlescreenGetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiIdlescreenGetCfm *) CsrPmemZalloc(sizeof(CsrUiIdlescreenGetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrUtf16StringDes(&primitive->text, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
void CsrUiIdlescreenGetCfmSerFree(void *msg)
{
    CsrUiIdlescreenGetCfm *primitive;

    primitive = (CsrUiIdlescreenGetCfm *) msg;

    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_REQ
CsrSize CsrUiEventGetReqSizeof(void *msg)
{
    CsrUiEventGetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_EVENT_GET_REQ */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_REQ
CsrUint8 *CsrUiEventGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiEventGetReq *primitive;

    primitive = (CsrUiEventGetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->event);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_EVENT_GET_REQ */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_REQ
void *CsrUiEventGetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiEventGetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiEventGetReq *) CsrPmemZalloc(sizeof(CsrUiEventGetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->event, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_EVENT_GET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
CsrSize CsrUiMenuGetitemCfmSizeof(void *msg)
{
    CsrUiMenuGetitemCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->label) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->sublabel));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
CsrUint8 *CsrUiMenuGetitemCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuGetitemCfm *primitive;

    primitive = (CsrUiMenuGetitemCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->key);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->icon);
    CsrMemCpySer(buffer, length, &primitive->label, sizeof(primitive->label));
    CsrMemCpySer(buffer, length, &primitive->sublabel, sizeof(primitive->sublabel));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->label);
    CsrUtf16StringSer(buffer, length, primitive->sublabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
void *CsrUiMenuGetitemCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuGetitemCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuGetitemCfm *) CsrPmemZalloc(sizeof(CsrUiMenuGetitemCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->key, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->icon, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->label), buffer, &offset, ((CsrSize) sizeof(primitive->label)));
    CsrMemCpyDes(((void *) &primitive->sublabel), buffer, &offset, ((CsrSize) sizeof(primitive->sublabel)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->label, buffer, &offset);
    CsrUtf16StringDes(&primitive->sublabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */

#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
void CsrUiMenuGetitemCfmSerFree(void *msg)
{
    CsrUiMenuGetitemCfm *primitive;

    primitive = (CsrUiMenuGetitemCfm *) msg;

    if(primitive->label != NULL)
    {
        CsrPmemFree(primitive->label);
    }
    if(primitive->sublabel != NULL)
    {
        CsrPmemFree(primitive->sublabel);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ
CsrSize CsrUiIdlescreenGetReqSizeof(void *msg)
{
    CsrUiIdlescreenGetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ
CsrUint8 *CsrUiIdlescreenGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiIdlescreenGetReq *primitive;

    primitive = (CsrUiIdlescreenGetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->idleScreen);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ */

#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ
void *CsrUiIdlescreenGetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiIdlescreenGetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiIdlescreenGetReq *) CsrPmemZalloc(sizeof(CsrUiIdlescreenGetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->idleScreen, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
CsrSize CsrUiDialogSetReqSizeof(void *msg)
{
    CsrUiDialogSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->heading) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->message) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK1) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK2));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
CsrUint8 *CsrUiDialogSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiDialogSetReq *primitive;

    primitive = (CsrUiDialogSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dialog);
    CsrMemCpySer(buffer, length, &primitive->heading, sizeof(primitive->heading));
    CsrMemCpySer(buffer, length, &primitive->message, sizeof(primitive->message));
    CsrMemCpySer(buffer, length, &primitive->textSK1, sizeof(primitive->textSK1));
    CsrMemCpySer(buffer, length, &primitive->textSK2, sizeof(primitive->textSK2));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->heading);
    CsrUtf16StringSer(buffer, length, primitive->message);
    CsrUtf16StringSer(buffer, length, primitive->textSK1);
    CsrUtf16StringSer(buffer, length, primitive->textSK2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
void *CsrUiDialogSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiDialogSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiDialogSetReq *) CsrPmemZalloc(sizeof(CsrUiDialogSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dialog, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->heading), buffer, &offset, ((CsrSize) sizeof(primitive->heading)));
    CsrMemCpyDes(((void *) &primitive->message), buffer, &offset, ((CsrSize) sizeof(primitive->message)));
    CsrMemCpyDes(((void *) &primitive->textSK1), buffer, &offset, ((CsrSize) sizeof(primitive->textSK1)));
    CsrMemCpyDes(((void *) &primitive->textSK2), buffer, &offset, ((CsrSize) sizeof(primitive->textSK2)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->heading, buffer, &offset);
    CsrUtf16StringDes(&primitive->message, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK1, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
void CsrUiDialogSetReqSerFree(void *msg)
{
    CsrUiDialogSetReq *primitive;

    primitive = (CsrUiDialogSetReq *) msg;

    if(primitive->heading != NULL)
    {
        CsrPmemFree(primitive->heading);
    }
    if(primitive->message != NULL)
    {
        CsrPmemFree(primitive->message);
    }
    if(primitive->textSK1 != NULL)
    {
        CsrPmemFree(primitive->textSK1);
    }
    if(primitive->textSK2 != NULL)
    {
        CsrPmemFree(primitive->textSK2);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
CsrSize CsrUiInputdialogGetCfmSizeof(void *msg)
{
    CsrUiInputdialogGetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->text));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
CsrUint8 *CsrUiInputdialogGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiInputdialogGetCfm *primitive;

    primitive = (CsrUiInputdialogGetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrUtf16StringSer(buffer, length, primitive->text);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
void *CsrUiInputdialogGetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiInputdialogGetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiInputdialogGetCfm *) CsrPmemZalloc(sizeof(CsrUiInputdialogGetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrUtf16StringDes(&primitive->text, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
void CsrUiInputdialogGetCfmSerFree(void *msg)
{
    CsrUiInputdialogGetCfm *primitive;

    primitive = (CsrUiInputdialogGetCfm *) msg;

    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_EVENT_SET_REQ
CsrSize CsrUiEventSetReqSizeof(void *msg)
{
    CsrUiEventSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_EVENT_SET_REQ */

#ifndef EXCLUDE_CSR_UI_EVENT_SET_REQ
CsrUint8 *CsrUiEventSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiEventSetReq *primitive;

    primitive = (CsrUiEventSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->event);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listener);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_EVENT_SET_REQ */

#ifndef EXCLUDE_CSR_UI_EVENT_SET_REQ
void *CsrUiEventSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiEventSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiEventSetReq *) CsrPmemZalloc(sizeof(CsrUiEventSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->event, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listener, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_EVENT_SET_REQ */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_CFM
CsrSize CsrUiEventGetCfmSizeof(void *msg)
{
    CsrUiEventGetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_EVENT_GET_CFM */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_CFM
CsrUint8 *CsrUiEventGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiEventGetCfm *primitive;

    primitive = (CsrUiEventGetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->inputMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listener);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_EVENT_GET_CFM */

#ifndef EXCLUDE_CSR_UI_EVENT_GET_CFM
void *CsrUiEventGetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiEventGetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiEventGetCfm *) CsrPmemZalloc(sizeof(CsrUiEventGetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->inputMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listener, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_EVENT_GET_CFM */

#ifndef EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ
CsrSize CsrUiStatusWifiSetReqSizeof(void *msg)
{
    CsrUiStatusWifiSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ
CsrUint8 *CsrUiStatusWifiSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiStatusWifiSetReq *primitive;

    primitive = (CsrUiStatusWifiSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->active);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ */

#ifndef EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ
void *CsrUiStatusWifiSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiStatusWifiSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiStatusWifiSetReq *) CsrPmemZalloc(sizeof(CsrUiStatusWifiSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->active, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_STATUS_WIFI_SET_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_HIDE_REQ
CsrSize CsrUiUieHideReqSizeof(void *msg)
{
    CsrUiUieHideReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_UIE_HIDE_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_HIDE_REQ
CsrUint8 *CsrUiUieHideReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiUieHideReq *primitive;

    primitive = (CsrUiUieHideReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->element);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_UIE_HIDE_REQ */

#ifndef EXCLUDE_CSR_UI_UIE_HIDE_REQ
void *CsrUiUieHideReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiUieHideReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiUieHideReq *) CsrPmemZalloc(sizeof(CsrUiUieHideReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->element, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_UIE_HIDE_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
CsrSize CsrUiMenuSetReqSizeof(void *msg)
{
    CsrUiMenuSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->heading) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK1) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK2));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
CsrUint8 *CsrUiMenuSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiMenuSetReq *primitive;

    primitive = (CsrUiMenuSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->menu);
    CsrMemCpySer(buffer, length, &primitive->heading, sizeof(primitive->heading));
    CsrMemCpySer(buffer, length, &primitive->textSK1, sizeof(primitive->textSK1));
    CsrMemCpySer(buffer, length, &primitive->textSK2, sizeof(primitive->textSK2));
    CsrUtf16StringSer(buffer, length, primitive->heading);
    CsrUtf16StringSer(buffer, length, primitive->textSK1);
    CsrUtf16StringSer(buffer, length, primitive->textSK2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
void *CsrUiMenuSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiMenuSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiMenuSetReq *) CsrPmemZalloc(sizeof(CsrUiMenuSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->menu, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->heading), buffer, &offset, ((CsrSize) sizeof(primitive->heading)));
    CsrMemCpyDes(((void *) &primitive->textSK1), buffer, &offset, ((CsrSize) sizeof(primitive->textSK1)));
    CsrMemCpyDes(((void *) &primitive->textSK2), buffer, &offset, ((CsrSize) sizeof(primitive->textSK2)));
    CsrUtf16StringDes(&primitive->heading, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK1, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */

#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
void CsrUiMenuSetReqSerFree(void *msg)
{
    CsrUiMenuSetReq *primitive;

    primitive = (CsrUiMenuSetReq *) msg;

    if(primitive->heading != NULL)
    {
        CsrPmemFree(primitive->heading);
    }
    if(primitive->textSK1 != NULL)
    {
        CsrPmemFree(primitive->textSK1);
    }
    if(primitive->textSK2 != NULL)
    {
        CsrPmemFree(primitive->textSK2);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
CsrSize CsrUiDialogGetCfmSizeof(void *msg)
{
    CsrUiDialogGetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->heading) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->message) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK1) + CsrUtf16StringSerLen((CsrUtf16String*) primitive->textSK2));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
CsrUint8 *CsrUiDialogGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrUiDialogGetCfm *primitive;

    primitive = (CsrUiDialogGetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrMemCpySer(buffer, length, &primitive->heading, sizeof(primitive->heading));
    CsrMemCpySer(buffer, length, &primitive->message, sizeof(primitive->message));
    CsrMemCpySer(buffer, length, &primitive->textSK1, sizeof(primitive->textSK1));
    CsrMemCpySer(buffer, length, &primitive->textSK2, sizeof(primitive->textSK2));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK1);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionSK2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionBACK);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->actionDEL);
    CsrUtf16StringSer(buffer, length, primitive->heading);
    CsrUtf16StringSer(buffer, length, primitive->message);
    CsrUtf16StringSer(buffer, length, primitive->textSK1);
    CsrUtf16StringSer(buffer, length, primitive->textSK2);

    return buffer;
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
void *CsrUiDialogGetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrUiDialogGetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrUiDialogGetCfm *) CsrPmemZalloc(sizeof(CsrUiDialogGetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->heading), buffer, &offset, ((CsrSize) sizeof(primitive->heading)));
    CsrMemCpyDes(((void *) &primitive->message), buffer, &offset, ((CsrSize) sizeof(primitive->message)));
    CsrMemCpyDes(((void *) &primitive->textSK1), buffer, &offset, ((CsrSize) sizeof(primitive->textSK1)));
    CsrMemCpyDes(((void *) &primitive->textSK2), buffer, &offset, ((CsrSize) sizeof(primitive->textSK2)));
    CsrUint16Des((CsrUint16 *) &primitive->actionSK1, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionSK2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionBACK, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->actionDEL, buffer, &offset);
    CsrUtf16StringDes(&primitive->heading, buffer, &offset);
    CsrUtf16StringDes(&primitive->message, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK1, buffer, &offset);
    CsrUtf16StringDes(&primitive->textSK2, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */

#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
void CsrUiDialogGetCfmSerFree(void *msg)
{
    CsrUiDialogGetCfm *primitive;

    primitive = (CsrUiDialogGetCfm *) msg;

    if(primitive->heading != NULL)
    {
        CsrPmemFree(primitive->heading);
    }
    if(primitive->message != NULL)
    {
        CsrPmemFree(primitive->message);
    }
    if(primitive->textSK1 != NULL)
    {
        CsrPmemFree(primitive->textSK1);
    }
    if(primitive->textSK2 != NULL)
    {
        CsrPmemFree(primitive->textSK2);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */

#endif /* EXCLUDE_CSR_UI_PRIVATE_MODULE */
