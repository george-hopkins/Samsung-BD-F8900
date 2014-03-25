#ifndef CSR_UI_UIEMANAGER_H__
#define CSR_UI_UIEMANAGER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_prim.h"
#include "csr_ui_ctrl.h"

#ifdef _cplusplus
extern "C" {
#endif

typedef struct MenuItem
{
    CsrUint16        key;
    CsrUint16        icon;
    CsrUint16       *label;
    CsrUint16       *sublabel;
    CsrUieHandle     parent;
    struct MenuItem *prev;
    struct MenuItem *next;
    CsrUieHandle     actionSK1;
    CsrUieHandle     actionSK2;
    CsrUieHandle     actionBACK;
    CsrUieHandle     actionDEL;
} MenuItem;

typedef struct Menu
{
    CsrUieType   type;
    CsrUieHandle handle;
    MenuItem    *items;
    MenuItem    *currentItem;
    CsrUint16    scrollGUI;
    CsrUint16    scrollTUI;
    CsrUint16   *heading;
    CsrUint16   *textSK1;
    CsrUint16   *textSK2;
} Menu;

typedef struct
{
    CsrUieType   type;
    CsrUieHandle handle;
    CsrUint16    inputMode;
    CsrSchedQid  listener;
} Event;

typedef struct
{
    CsrUieType   type;
    CsrUieHandle handle;
    CsrUint16    scrollGUI;
    CsrUint16    scrollTUI;
    CsrUint16   *heading;
    CsrUint16   *message;
    CsrUint16   *textSK1;
    CsrUint16   *textSK2;
    CsrUieHandle actionSK1;
    CsrUieHandle actionSK2;
    CsrUieHandle actionBACK;
    CsrUieHandle actionDEL;
} Dialog;

typedef struct
{
    CsrUieType   type;
    CsrUieHandle handle;
    CsrUint16   *heading;
    CsrUint16   *message;
    CsrUint16    icon;
    CsrUint16   *text;
    CsrUint16    textLength;
    CsrUint8     keyMap;
    CsrUint16   *textSK1;
    CsrUint16   *textSK2;
    CsrUieHandle actionSK1;
    CsrUieHandle actionSK2;
    CsrUieHandle actionBACK;
    CsrUieHandle actionDEL;
} InputDialog;

typedef struct
{
    CsrUieType   type;
    CsrUieHandle handle;
    CsrUint16   *heading;
    CsrUint16   *text;
    CsrUint16    textLength;
    CsrUint16   *textSK1;
    CsrUint16   *textSK2;
    CsrUieHandle actionSK1;
    CsrUieHandle actionSK2;
    CsrUieHandle actionBACK;
    CsrUieHandle actionDEL;
} IdleScreen;

CsrUint16 CsrUiUieMenuGetNumberOfItems(Menu *menu);
CsrUint16 CsrUiUieMenuGetCursorIndex(Menu *menu);
CsrUint16 CsrUiUieMenuGetCursorPosition(CsrUiInstanceData *instanceData, CsrUieHandle menu);
void CsrUiUieMenuSetCursorPosition(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key);
void *CsrUiUieHandle2Pointer(CsrUiInstanceData *instanceData, CsrUieHandle handle);
CsrUieHandle CsrUiUieCreate(CsrUiInstanceData *instanceData, CsrUieType type);
void CsrUiUieDestroy(CsrUiInstanceData *instanceData, CsrUieHandle handle);
void CsrUiUieDestroyAll(CsrUiInstanceData *instanceData);
void CsrUiUieMenuAdditem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key, CsrUint16 icon, CsrUint16 *label, CsrUint16 *sublabel, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL, CsrUint16 position);
void CsrUiUieMenuRemoveitem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key);
void CsrUiUieMenuRemoveallitems(CsrUiInstanceData *instanceData, CsrUieHandle menu);
void CsrUiUieMenuSetitem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key, CsrUint16 icon, CsrUint16 *label, CsrUint16 *sublabel, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL);
MenuItem *CsrUiUieMenuGetitem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key);

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_UIEMANAGER_H__ */
